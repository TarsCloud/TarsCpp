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

#ifndef __TC_COROUTINE_QUEUE_H_
#define __TC_COROUTINE_QUEUE_H_

#include <deque>
#include <vector>
#include <cassert>
#include <mutex>
#include <unordered_set>
#include "util/tc_coroutine.h"

using namespace std;

namespace tars
{
    /////////////////////////////////////////////////
/** 
 * @file tc_coroutine_queue.h
 * @brief 协程队列类, 用于跨协程交互数据
 *
 * 使用说明:
 * - TC_CoroutineQueue的目的在于在多个协程间交互数据
 * - 一个协程push数据到队列, 另外的协程通过调用exec来完成数据获取, 如果没有数据, 则当前协程yield, 直到有数据被唤醒
 * - 注意可以跨不同线程的协程传递数据
 * @author ruanshudong@qq.com
 */
       
/////////////////////////////////////////////////
/**
 * @brief 线程/协程安全队列, 必须在协程执行, 否则会异常!
 */
template<typename T, typename D = deque<T> >
class TC_CoroutineQueue
{
public:
    TC_CoroutineQueue() {};

    typedef D queue_type;

    /**
     * @brief 当队列有数据时, 则执行函数, 如果没有数据则协程yield, 如果调用terminate, exec才返回
     *
     * @param t 
     */
    void exec(std::function<void(const T &)>);

    /**
     * @brief 结束队列, 注意会唤醒所有等待在队列上的协程(执行exec而阻塞的协程)
     */
    void terminate();

    /**
     * @brief 通知等待在队列上面的线程都醒过来
     */
    void notifyT();

    /**
	 * @brief 放数据到队列后端. 
	 *  
     * @param t
     */
    void push_back(const T& t, bool notify = true);

    /**
	 * @brief  放数据到队列后端. 
	 *  
     * @param vt
     */
    void push_back(const queue_type &qt, bool notify = true);

    /**
	 * @brief  放数据到队列前端. 
	 *  
     * @param t
     */
    void push_front(const T& t, bool notify = true);

    /**
	 * @brief  放数据到队列前端. 
	 *  
     * @param vt
     */
    void push_front(const queue_type &qt, bool notify = true);

    /**
	 * @brief  交换数据
	 *  
     * @param q
     * @param 是否等待有数据
     * @return 有数据返回true, 无数据返回false
     */
    bool swap(queue_type &q);

    /**
     * @brief  队列大小.
     *
     * @return size_t 队列大小
     */
    size_t size() const;

    /**
     * @brief  清空队列
     */
    void clear();

    /**
     * @brief  是否数据为空.
     *
     * @return bool 为空返回true，否则返回false
     */
    bool empty() const;

protected:
	TC_CoroutineQueue(const TC_CoroutineQueue&) = delete;
	TC_CoroutineQueue(TC_CoroutineQueue&&) = delete;
	TC_CoroutineQueue& operator=(const TC_CoroutineQueue&) = delete;
	TC_CoroutineQueue& operator=(TC_CoroutineQueue&&) = delete;

protected:
    /**
     * 队列
     */
    queue_type          _queue;

	//锁
    mutable std::mutex _mutex;

    /**
     * 协程调度器
     */
	unordered_set<shared_ptr<TC_CoroutineScheduler>> _schedulers;

	/**
	 * 结束协程exec
	 */
	bool _terminate = false;
};

//template<typename T, typename D> T TC_CoroutineQueue<T, D>::front()
//{
//	std::unique_lock<std::mutex> lock(_mutex);
//
//	return  _queue.front();
//}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::terminate()
{
	_terminate = true;

	{
		std::lock_guard<std::mutex> lock(_mutex);
		for(auto scheduler : _schedulers)
		{
			scheduler->notify();
		}
	}
}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::exec(std::function<void(const T &)> func)
{
	assert(TC_CoroutineScheduler::scheduler() != NULL);

	auto scheduler = TC_CoroutineScheduler::scheduler();

	bool flag;

	do
	{
		T t;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_queue.empty()) {
				flag = false;
			}
			else
			{
				flag = true;
				t = _queue.front();
				_queue.pop_front();
			}
		}

		if(!flag)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_schedulers.insert(scheduler);
			}

			scheduler->yield();

			{
				std::lock_guard<std::mutex> lock(_mutex);
				_schedulers.erase(scheduler);
			}
		}
		else
		{
			try { func(t); } catch(...) {}
		}

	}while(!_terminate && !scheduler->isTerminate());
}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::notifyT()
{
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if(!_schedulers.empty())
		{
			(*_schedulers.begin())->notify();
		}
	}
}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::push_back(const T& t, bool notify)
{
    if(notify) {
	    {
		    std::unique_lock<std::mutex> lock(_mutex);

		    _queue.push_back(t);
	    }

	    notifyT();

    }
    else
    {
        std::lock_guard<std::mutex> lock (_mutex);
        _queue.push_back(t);
    }
}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::push_back(const queue_type &qt, bool notify)
{
    if(notify) {
	    {
		    std::unique_lock<std::mutex> lock(_mutex);

		    typename queue_type::const_iterator it = qt.begin();
		    typename queue_type::const_iterator itEnd = qt.end();
		    while (it != itEnd) {
			    _queue.push_back(*it);
			    ++it;
		    }
	    }

	    notifyT();
    }
    else
    {
        std::lock_guard<std::mutex> lock (_mutex);

        typename queue_type::const_iterator it = qt.begin();
        typename queue_type::const_iterator itEnd = qt.end();
        while (it != itEnd) {
            _queue.push_back(*it);
            ++it;
        }
    }
}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::push_front(const T& t, bool notify)
{
    if(notify) {
	    {
		    std::unique_lock<std::mutex> lock(_mutex);

		    _queue.push_front(t);
	    }

	    notifyT();
    }
    else
    {
        std::lock_guard<std::mutex> lock (_mutex);

        _queue.push_front(t);
    }
}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::push_front(const queue_type &qt, bool notify)
{
    if(notify) {
	    {
		    std::unique_lock<std::mutex> lock(_mutex);

		    typename queue_type::const_iterator it = qt.begin();
		    typename queue_type::const_iterator itEnd = qt.end();
		    while (it != itEnd) {
			    _queue.push_front(*it);
			    ++it;
		    }
	    }

	    notifyT();
    }
    else
    {
        std::lock_guard<std::mutex> lock (_mutex);

        typename queue_type::const_iterator it = qt.begin();
        typename queue_type::const_iterator itEnd = qt.end();
        while (it != itEnd) {
            _queue.push_front(*it);
            ++it;
        }
    }
}

template<typename T, typename D> bool TC_CoroutineQueue<T, D>::swap(queue_type &q)
{
    {
        std::lock_guard<std::mutex> lock (_mutex);

        if (_queue.empty()) {
            return false;
        }

        q.swap(_queue);

        return true;
    }
}

template<typename T, typename D> size_t TC_CoroutineQueue<T, D>::size() const
{
	std::lock_guard<std::mutex> lock(_mutex);
    return _queue.size();
}

template<typename T, typename D> void TC_CoroutineQueue<T, D>::clear()
{
	std::lock_guard<std::mutex> lock(_mutex);
    _queue.clear();
}

template<typename T, typename D> bool TC_CoroutineQueue<T, D>::empty() const
{
	std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
}

}
#endif

