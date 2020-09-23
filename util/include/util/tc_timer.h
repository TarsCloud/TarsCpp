#ifndef	__TC_TIMER_H_
#define __TC_TIMER_H_

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
class TC_Timer
{
protected:
	struct Func
	{
		Func( uint64_t fireMillseconds) :  _fireMillseconds(fireMillseconds) { }
		~Func() {}
		std::function<void()>   _func;
		uint64_t                _fireMillseconds = 0;	//事件触发时间
        TC_Cron                 _cron;  //crontab
	};

	typedef std::unordered_set<uint64_t> EVENT_SET;

	typedef std::unordered_map<uint64_t, shared_ptr<Func>> MAP_EVENT;

	typedef map<uint64_t, unordered_set<uint64_t>> MAP_TIMER;

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
		//定义返回值类型
		using RetType = decltype(f(args...));

		auto task = std::make_shared < std::packaged_task
			< RetType() >> (std::bind(std::forward<F>(f), std::forward<Args>(args)...));

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
		//定义返回值类型
		using RetType = decltype(f(args...));

		auto task = std::make_shared < std::packaged_task
			                               < RetType() >> (std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		uint64_t fireMillseconds;

		if(execNow) {
			fireMillseconds = TC_TimeProvider::getInstance()->getNowMs();
		}else {
			fireMillseconds = TC_TimeProvider::getInstance()->getNowMs() + repeatTime;
		}

		return post(create(fireMillseconds, repeatTime,"", f, args...));
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
        //定义返回值类型
        using RetType = decltype(f(args...));

        auto task = std::make_shared < std::packaged_task
            < RetType() >> (std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  
        return post(create(0, 0,cronexpr, f, args...));
    }

	/**
	 * 删除事件
	 * @param uniqId
	 */
	void erase(int64_t uniqId);

protected:
	template <class F, class... Args>
	shared_ptr<Func> create(int64_t fireMillseconds, int64_t repeatTime, const string & cronexpr, F &&f, Args &&... args)
	{
		//定义返回值类型
		using RetType = decltype(f(args...));

		auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		shared_ptr<Func> fPtr = std::make_shared<Func>(fireMillseconds);

        if (!cronexpr.empty())
        {
            fPtr->_cron = TC_Cron::makecron(cronexpr);
            fPtr->_fireMillseconds = TC_Cron::nextcron(fPtr->_cron) * 1000;
        }

		//单次任务
		if (repeatTime == 0 && !fPtr->_cron.isset)
		{
            fPtr->_func = [task]() {
                (*task)();
                task->reset();
            };
		}
		else
		{
            fPtr->_func = [task, this, fPtr, repeatTime]() {
                (*task)();
                task->reset();
                if (fPtr->_cron.isset)
                {
                    fPtr->_fireMillseconds = TC_Cron::nextcron(fPtr->_cron, fPtr->_fireMillseconds / 1000) * 1000;
                    this->post(fPtr);
                }
                else if (repeatTime > 0)
                {
                    fPtr->_fireMillseconds = TC_TimeProvider::getInstance()->getNowMs() + repeatTime;
                    this->post(fPtr);
                }
            };
		}

		return fPtr;
	}

	int64_t post(const shared_ptr<Func> &event);

	void fireEvent(const EVENT_SET &el);

	void run();

protected:
	std::mutex          _mutex;

	std::condition_variable _cond;

	bool        _terminate = false;

	MAP_EVENT   _mapEvent;      //id, 事件

	MAP_TIMER   _mapTimer;      //时间, 事件

	TC_ThreadPool _tpool;
};

}

#endif

