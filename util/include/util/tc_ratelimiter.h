#ifndef _tc_rate_limiter_h_
#define _tc_rate_limiter_h_

#include <mutex>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include "util/tc_thread_rwlock.h"


/**
 * 流量控制组件（适用于接口级别的流量控制，不适用用户级别频率控制）
 * 基于令牌桶算法实现
 * 线程安全
 * rate 为速率（每秒运行放行的数量，可以理解为运行的 qps）
 * maxPermits 为最大允许数量，即流量峰值，可以简单理解为运行最的最大缓冲，如果不设置默认为3倍的rate，可以理解为允许3s的队列积压
 * 使用时，可以用阻塞试，也可以用非阻塞时 超过频率则返回失败
 * 
 * 使用举例：
 * 对taf服务端流量控制，建议使用非阻塞式，即 tryAquire(); 对于客户端发请求，则使用阻塞式，即 aquire();
 * TC_RateLimiter limiter(100);
 * if(limiter.tryAquire())
 * {
 *      pass;
 * }
 * else
 * {
 *      refuse;
 * }
 * 
 * 业务如果一个进程（服务）中需要用到多个TC_RateLimiter， 那么可以用TC_RateLimiterFactory, 通过不同的name来区分，初始化时一定要主动 setRate，否则就相当于没有频率控制
 * 注意：这里一个进程不宜有太多的name，理论上控制在100个以内
 * 例如：
 * TC_RateLimiterFactory limiter;
 * limiter.getLimiter("interfaceA")->setRate(100);
* if(limiter.getLimiter("interfaceA")->tryAquire())
 * {
 *      pass;
 * }
 * else
 * {
 *      refuse;
 * }
 * 
 * limiter.getLimiter("interfaceB")->setRate(200);
* if(limiter.getLimiter("interfacB")->tryAquire())
 * {
 *      pass;
 * }
 * else
 * {
 *      refuse;
 * }
 * 
 */

class TC_RateLimiter 
{
public:
    TC_RateLimiter():_interval(0), _maxPermits(0), _storedPermits(0), _nextFree(0) 
    {
    }

    TC_RateLimiter(double rate):_interval(0), _maxPermits(0), _storedPermits(0), _nextFree(0) 
    {
        setRate(rate);
    }

    long aquire(int permits = 1)
    {
        if (permits <= 0) 
        {
            std::runtime_error("TC_RateLimiter: Must request positive amount of permits");
        }

        std::chrono::microseconds waitTime = claimNext(permits);
        std::this_thread::sleep_for(waitTime);

        return waitTime.count() / 1000.0;
    }

    /**
     * permits: 一次申请的令牌数，默认 1
     * timeout: 
     */
    bool tryAquire(int permits = 1, int timeout = 0)
    {
        using namespace std::chrono;

        unsigned long long now = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();

        if (_nextFree > now + timeout * 1000)
        {
            return false;
        }     
        else 
        {
            aquire(permits);
        }

        return true;
    }

    // 设置速率
    void setRate(double rate)
    {
        if (rate <= 0) 
        {
            throw std::runtime_error("TC_RateLimiter: Rate must be greater than 0");
        }

        std::lock_guard<std::mutex> lock(_mutex);
        _interval = 1000000.0 / rate;
        _maxPermits = rate * 3;
    }

    void setMaxPermits(int permits)
    {
        if (permits < 0)
        {
            _maxPermits = 0;
        }
        else
        {
            _maxPermits = permits;
        }
    }

    double getRate() const
    {
        return 1000000.0 / _interval;
    }

protected:
    void sync(unsigned long long now)
    {
        if (now > _nextFree) 
        {
            _storedPermits = std::min(_maxPermits, _storedPermits + (now - _nextFree) / _interval);
            _nextFree = now;
        }
    }
    std::chrono::microseconds claimNext(double permits)
    {
        using namespace std::chrono;

        std::lock_guard<std::mutex> lock(_mutex);

        unsigned long long now = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();

        sync(now);

        unsigned long long wait = _nextFree - now;

        double stored = std::min(permits, _storedPermits);
        double fresh = permits - stored;

        long nextFree = (long)(fresh * _interval);

        _nextFree += nextFree;

        _storedPermits -= stored;

        return microseconds(wait);
    }

private:
    std::mutex _mutex;
    double _interval;
    double _maxPermits;
    double _storedPermits;
    unsigned long long _nextFree;
};

/**
 * 创建具体频率控制对象，每个业务（name）频控维护一个对象
 * 这里业务根据实际情况来定义，比如： 要对某个接口进行流量控制，
 * 
 */
class TC_RateLimiterFactory
{

public:
    TC_RateLimiterFactory()
    {}

    ~TC_RateLimiterFactory()
    {
        for (auto it = _limiter.begin(); it != _limiter.end(); it++)
        {
            delete it->second;
        }
    }

    TC_RateLimiter* getLimiter(const string& name)
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
        TC_RateLimiter* limiter = new TC_RateLimiter();
        _limiter[name] = limiter;
        return limiter;
    }

private:
    taf::TC_ThreadRWLocker   _rwLock;
    std::unordered_map<string, TC_RateLimiter*> _limiter;
};

#endif
