/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#pragma once

#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include "util/tc_thread_pool.h"
#include "util/tc_timeprovider.h"
#include "util/tc_cron.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_timer.h
 * @brief 定时器类, 用于设置定时器, 可以定时回调function
 * 使用说明:
 * 设定函数的定时时间, 定时时间到, 则回调
 * 可以设定回调线程数, 默认只有一个, 底层用tc_thread_pool实现线程池的
 * 任何可以丢进tc_thread_pool的function对象(std::bind的对象), 都可以丢该tc_timer
 *
 */          
/////////////////////////////////////////////////


/**
 * @brief  定时器类
 */
class TC_TimerBase
{
protected:

	struct Func
	{
		Func( uint64_t fireMillseconds, uint32_t uniqueId) :  _fireMillseconds(fireMillseconds), _uniqueId(uniqueId) { }
		~Func() {}
		std::function<void()>   _func;
		uint64_t                _fireMillseconds = 0;	//事件触发时间
        TC_Cron                 _cron;  //crontab
        uint32_t                _uniqueId = 0;
	};

    typedef std::unordered_set<uint64_t> EVENT_SET;

    typedef std::unordered_map<uint64_t, shared_ptr<Func>> MAP_EVENT;

    typedef map<uint64_t, unordered_set<uint64_t>> MAP_TIMER;

public:

    /**
     * 析构
     */
    virtual ~TC_TimerBase();

    /**
     * 事件总个数
     * @return
     */
    size_t count();

    /**
     * 重复事件个数
     * @return
     */
    size_t repeatCount();

    /**
     * @brief 指定fireMillseconds时间执行
     * @param fireMillseconds, 触发时间(毫秒)
     * @return 返回事件Id
     */
    template <class F, class... Args>
    int64_t postAtTime(int64_t fireMillseconds, F &&f, Args &&... args)
    {
	    return post(create(fireMillseconds, 0,"", f, args...));
    }

    /**
     * @brief 延时delayMillseconds时间执行
     * @param delayMillseconds, 延时时间
     * @return 返回事件Id
     */
    template <class F, class... Args>
    int64_t postDelayed(int64_t delayMillseconds, F &&f, Args &&... args)
    {
        uint64_t fireMillseconds = TC_TimeProvider::getInstance()->getNowMs() + delayMillseconds;

        return post(create(fireMillseconds, 0,"", f, args...));
    }

    /**
     * @brief 重复repeatTime(毫秒)执行一次, 注意repeat是上一次执行完, 才会重新触发下一次事件的计时
     * @param repeatTime, 重复时间
     * @param execNow, 第一次是否马上执行
     * @return 返回事件Id
     */
    template <class F, class... Args>
    int64_t postRepeated(int64_t repeatTime, bool execNow, F &&f, Args &&... args)
    {
        uint64_t fireMillseconds;

        if(execNow) {
            fireMillseconds = TC_TimeProvider::getInstance()->getNowMs();
        }else {
            fireMillseconds = TC_TimeProvider::getInstance()->getNowMs() + repeatTime;
        }

		return post(create(fireMillseconds, repeatTime,"", f, args...),true);
	}

    /**
     * @brief
     * @param cronexpr, crontab 语法  <seconds> <minutes> <hours> <days of month> <months> <days of week> ，具体例子参考 tc_cron.h
     * @return 返回事件Id
     */
    // CRON	Description
    // * * * * * *	Every second
    // */5 * * * * *	Every 5 seconds
    // 0 */5 */2 * * *	Every 5 minutes, every 2 hours
    // 0 */2 */2 * */2 */2	Every 2 minutes, every 2 hours, every 2 days of the week, every 2 months
    // 0 15 10 * * * 	10:15 AM every day
    // 0 0/5 14 * * *	Every 5 minutes starting at 2 PM and ending at 2:55 PM, every day
    // 0 10,44 14 * 3 WED	2:10 PM and at 2:44 PM every Wednesday of March
    // 0 15 10 * * MON-FRI	10:15 AM every Monday, Tuesday, Wednesday, Thursday and Friday
    // 0 0 12 1/5 * *	12 PM every 5 days every month, starting on the first day of the month
    // 0 11 11 11 11 *	Every November 11th at 11:11 AM
    template <class F, class... Args>
    int64_t postCron(const string&cronexpr, F&& f, Args&&... args)
    {
	    return post(create(0, 0,cronexpr, f, args...),true);
    }

    /**
     * 删除事件
     * @param uniqId
     */
    void erase(int64_t uniqId);

    /**
     * 下一次定时器的时间
     */
    int64_t nextTimer() const { return _nextTimer; }

    /**
     * 判断循环是否还存在
     * @param uniqId
     */
    bool exist(int64_t uniqId ,bool repeat = false);

    /**
     * 清空当前所有任务
     */
    void clear();

protected:
    template <class F, class... Args>
    shared_ptr<Func> create(int64_t fireMillseconds, int64_t repeatTime, const string & cronexpr, F &&f, Args &&... args)
    {
        //定义返回值类型
        using RetType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		shared_ptr<Func> fPtr = std::make_shared<Func>(fireMillseconds, genUniqueId());

        if (!cronexpr.empty())
        {
            fPtr->_cron = TC_Cron::makecron(cronexpr);
            fPtr->_fireMillseconds = TC_Cron::nextcron(fPtr->_cron) * 1000;
        }

		//单次任务
		if (repeatTime == 0 && !fPtr->_cron.isset)
		{
			weak_ptr<Func> wPtr = fPtr;

            fPtr->_func = [task, this, wPtr]() {
                (*task)();
                task->reset();

				shared_ptr<Func> p = wPtr.lock();

				if(p)
				{
					std::unique_lock<std::mutex> lock(_mutex);
					_tmpEvent.erase(p->_uniqueId);
				}

            };
		}
		else
		{
			weak_ptr<Func> wPtr = fPtr;

            fPtr->_func = [task, this, wPtr, repeatTime]() {
                (*task)();
                task->reset();

                shared_ptr<Func> p = wPtr.lock();
                if(p)
				{
					{
						std::unique_lock<std::mutex> lock(_mutex);
						_tmpEvent.erase(p->_uniqueId);
					}

					if (this->exist(p->_uniqueId, true))
					{
						if (p->_cron.isset)
						{
							p->_fireMillseconds =
									TC_Cron::nextcron(p->_cron, p->_fireMillseconds / 1000) * 1000;
							this->post(p);
						}
						else if (repeatTime > 0)
						{
							p->_fireMillseconds = TC_TimeProvider::getInstance()->getNowMs() + repeatTime;
							this->post(p);
						}
					}
				}
            };
		}

        return fPtr;
    }

	int64_t post(const shared_ptr<Func> &event ,bool repeat = false);

    /**
     * 触发事件
     * @param ms: 如果没有事件了, 默认休息时间
     * @return >=0, 下一次等待时间(如果事件了, 则是ms+TNOWMS)
     */
    int64_t fireEvents(int64_t ms);

    /**
     * 触发定时器, 并返回需要等待的时间
     * @param el, 返回事件集
     * @return -1: 无定时时间了, >0: 下一次需要触发的时间间隔
     */
    int64_t getEvents(EVENT_SET &el);

    /**
     * 实际执行事件
     * @param func
     */
    virtual void onFireEvent(std::function<void()> func) = 0;

    /**
     * 增加了一个最近的定时器, 需要触发wait唤醒, 等到到最新的时间上
     * 注意回调该函数时, 加了锁的, 因此这个函数中需要尽快处理
     */
    virtual void onAddTimer() = 0;

    /**
     * 生成时间的唯一id
     * @return
     */
	uint32_t genUniqueId();

protected:
    std::mutex          _mutex;

    std::condition_variable _cond;

    int64_t     _nextTimer = -1;//下一次需要触发的时间, <0:  表示无下一次事件

    MAP_EVENT   _mapEvent;      //id, 事件

	MAP_EVENT   _tmpEvent;      //id, 事件

    MAP_TIMER   _mapTimer;      //时间, 事件

	atomic_uint _increaseId = {0};
	
	set<int64_t> _repeatIds; //循环任务的所有ID

};


/**
 * @brief  定时器类
 */
class TC_Timer : public TC_TimerBase
{
public:

	/**
	 * 析构
	 */
	~TC_Timer();

	/**
	 * 系统定时器
	 * @param numThread, 回调线程数, 默认是1个
	 */
	void startTimer(int numThread = 1);

	/**
	 * 停止定时器
	 * 如果当前有定时任务正在执行, 会等执行完毕
	 */
	void stopTimer();

	/**
	 * @brief
	 * @return num(RUNNER_EVENT), num(ALL_EVENT), num(REPEAT_EVENT)
	 */
	tuple<int64_t, int64_t, int64_t> status();


protected:
    virtual void onFireEvent(std::function<void()> func);

    /**
     * 增加了一个最近的定时器, 需要触发wait唤醒, 等到到最新的时间上
     */ 
    virtual void onAddTimer();

	void run();

protected:
	bool        _terminate = false;

	TC_ThreadPool _tpool;
};

}

