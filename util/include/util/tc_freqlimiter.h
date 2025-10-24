#ifndef __TC_FREQLIMITER_H__
#define __TC_FREQLIMITER_H__

#include <mutex>
#include <unordered_map>
#include <string>
#include "util/tc_hash_fun.h"
#include "util/tc_timeprovider.h"
#include "util/tc_thread_rwlock.h"

using namespace std;
using namespace taf;

/**
 * 频率控制组件
 * 基于固定时间窗口实现
 * 线程安全
 * 在指定周期（period）内最多可以进行的最大次数（count）
 * maxCount 为最大允许的频率数
 * totalUser 为预估的key的总量，用来做过期数据清理，默认1,000,000， 对一般业务足够了，业务可以自己设置
 * 
 * 使用举例：
 * TC_FreqLimiter limiter(60, 10);  // 表示运行一个key 60s内最多10次
 * if(limiter.check("userA"))
 * {
 *      pass;
 * }
 * else
 * {
 *      refuse;
 * }
 * 
 * 业务如果一个进程（服务）中需要用到多个TC_FreqLimiter， 那么可以用 TC_FreqLimiterFactory, 通过不同的name来区分，初始化时一定要主动 init，否则就相当于没有频率控制
 * 注意：这里一个进程不宜有太多的name，理论上控制在100个以内
 * 例如：
 * TC_FreqLimiterFactory limiter;
 * limiter.getLimiter("interfaceA")->init(60, 10); // 表示该接口每个key 60s可以10次
* if(limiter.getLimiter("interfaceA")->check("userA"))
 * {
 *      pass;
 * }
 * else
 * {
 *      refuse;
 * }
 */
class TC_FreqLimiter
{
    struct KeyItem
    {
        time_t  beginTime;
        int     count;
        KeyItem()
        {
            beginTime = 0;
            count = 0;
        }
    };
    struct KeyItemGroup
    {
        time_t  checkTime;
        // 这里的map大小一直增长，有风险
        unordered_map<string, KeyItem>  items;
        
        KeyItemGroup()
        {
            checkTime = TNOW;
        }
    };

    // 群锁
    #define USER_LOCK(x) std::lock_guard<std::mutex> lock(*_locks[taf::hash<string>()(x)%_locks.size()]);
    #define USER_INDEX_LOCK(i) std::lock_guard<std::mutex> lock(*_locks[i]);
    

public:
    TC_FreqLimiter():_period(0), _maxCount(0), _totalUsers(1000000)
    {
    }

    TC_FreqLimiter(unsigned int period, int count)
    {
        init(period, count);
    }

    ~TC_FreqLimiter()
    {
        for (size_t i = 0; i < _locks.size(); i++)
        {
            delete _locks[i];
        }
        
        for (size_t i = 0; i < _counts.size(); i++)
        {
            delete _counts[i];
        }
    }

    /**
     * @period: 周期
     * @count： 周期内最大允许的次数
     * @group: 这里主要用来做群锁分组用， 如果服务请求量比较大，这里建议设置为服务的业务处理线程数
     */
    void init(unsigned int period, int count, int group = 1)
    {
        if (period == 0 || count <= 0 || group <= 0) 
        {
            std::runtime_error("TC_FreqLimiter: Must request positive amount of permits");
        }

        _period = period;
        _maxCount = count;

        for (int i = 0; i < group; i++)
        {
            _locks.push_back(new std::mutex());
            _counts.push_back(new KeyItemGroup());
        }    

        setTotalUsers(1000000); 
    }

    bool check(const string& key) 
    {
        if (_period == 0)
        {
            return true;
        }
        size_t index = taf::hash<string>()(key)%_counts.size();
        // USER_LOCK(key);
        USER_INDEX_LOCK(index);
        KeyItemGroup* group = _counts[index];
        if (group->items.find(key) != group->items.end())
        {
            if (group->items[key].beginTime + _period < TNOW)
            {
                group->items[key].beginTime = TNOW;
                group->items[key].count = 1;
            }
            else if(group->items[key].count < _maxCount)
            {
                group->items[key].count++;
            }
            else
            {
                return false;
            }
        }
        else
        {
            group->items[key].beginTime = TNOW;
            group->items[key].count = 1;
        }

        checkTimeout(group);

        return true;
    }

    // 设置预估的总用户数， 用来做过期数据清理
    void setTotalUsers(unsigned int total)
    {
        if (total < _counts.size() || _counts.size() == 0 || total < 10000)
        {
            return;
        }
        _totalUsers = total;
        _eachUsers = _totalUsers / _counts.size();
    }

protected:

    // 数据清理
    void checkTimeout(KeyItemGroup* group)
    {
        if (group->items.size() <= _eachUsers || group->checkTime + _period > TNOW)
        {
            // 做个保护， 如果在一个周期内就超过了总量， 也不会淘汰数据
            return;
        }

        group->checkTime = TNOW;
        for (auto it = group->items.begin(); it != group->items.end();)
        {
            if (it->second.beginTime + _period < group->checkTime)
            {
                // cout << "clear-timeout key:" << it->first << "|" << it->second.beginTime << "|" << group->checkTime << endl;
                group->items.erase(it++);
            }
            else
            {
                it++;
            }           
        }
    }

private:
    unsigned int    _period;
    int             _maxCount;
    
    //time_t  _beginTime;

    vector<std::mutex*>     _locks;
    //vector<std::unordered_map<string, int> >  _keyCount;
    vector<KeyItemGroup*>   _counts; 
    unsigned int    _totalUsers;    // 预估最多的用户数， 超过该值会做数据清理
    unsigned int    _eachUsers;     // 每个group用户数
};

class TC_FreqLimiterFactory
{

public:
    TC_FreqLimiterFactory()
    {}

    ~TC_FreqLimiterFactory()
    {
        for (auto it = _limiter.begin(); it != _limiter.end(); it++)
        {
            delete it->second;
        }
    }

    TC_FreqLimiter* getLimiter(const string& name)
    {
        {
            taf::TC_ThreadRLock lock(_rwLock);
            if (_limiter.find(name) != _limiter.end())
            {
                return _limiter[name];
            }
        }
        
        taf::TC_ThreadWLock lock(_rwLock);
        if (_limiter.find(name) != _limiter.end())
        {
            return _limiter[name];
        }
        TC_FreqLimiter* limiter = new TC_FreqLimiter();
        _limiter[name] = limiter;
        return limiter;
    }

private:
    taf::TC_ThreadRWLocker   _rwLock;
    std::unordered_map<string, TC_FreqLimiter*> _limiter;
};

#endif // !__TC_FREQLIMITER_H__
