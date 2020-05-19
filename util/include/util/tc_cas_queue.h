#ifndef __TC_CAS_QUEUE_H_
#define __TC_CAS_QUEUE_H_

#include <deque>
#include <vector>
#include <cassert>
#include "util/tc_lock.h"
#include "util/tc_spin_lock.h"

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/**
 * @file tc_cas_queue.h
 * @brief Lock-Free Thread
 * @brief 线程无锁
 *  
 * @author ruanshudong@qq.com
 */
       
/////////////////////////////////////////////////
/**
 * @brief Thread Safe Queue
 * @brief 线程安全队列
 */
template<typename T, typename D = deque<T> >
class TC_CasQueue
{
public:
    TC_CasQueue():_size(0){};

public:

    typedef D queue_type;

	/**
	 * @brief Getting data from the head without data throwing exception
     * @brief 从头部获取数据, 没有数据抛出异常
	 *
	 * @param t
	 * @return bool: true, get data ; false, no data
     * @return bool: true, 获取了数据, false, 无数据
	 */
	T front();

    /**
     * @brief Get data from the head
     * @brief 从头部获取数据
     *
     * @param t 
     * @return bool: true, get data ; false, no data
     * @return bool: true, 获取了数据, false, 无数据
     */
    bool pop_front(T& t);

	/**
	 * @brief Get data from the head
     * @brief 从头部获取数据
	 *
	 * @return bool: true, get data ; false, no data
     * @return bool: true, 获取了数据, false, 无数据
	 */
	bool pop_front();

    /**
	 * @brief Put data to the back end of the queue. 
     * @brief 放数据到队列后端. 
	 *  
     * @param t
     */
    void push_back(const T& t);

    /**
	 * @brief Put data to the back end of the queue.
     * @brief  放数据到队列后端. 
	 *  
     * @param vt
     */
    void push_back(const queue_type &qt);

    /**
	 * @brief Put data to the front end of the queue.
     * @brief  放数据到队列前端. 
	 *  
     * @param t
     */
    void push_front(const T& t);

    /**
	 * @brief Put data to the front end of the queue.
     * @brief  放数据到队列前端. 
	 *  
     * @param vt
     */
    void push_front(const queue_type &qt);

    /**
	 * @brief  Exchange Data
     * @brief  交换数据
	 *  
     * @param q
     * @return bool: true, data returned ; false, no data returned
     * @return 有数据返回true, 无数据返回false
     */
    bool swap(queue_type &q);

    /**
     * @brief  Size of the Queue
     * @brief  队列大小.
     *
     * @return size_t the size of the queue
     * @return size_t 队列大小
     */
    size_t size() const;

    /**
     * @brief  Clear Up the Queue
     * @brief  清空队列
     */
    void clear();

    /**
     * @brief  是否数据为空.
     * @brief  Determine whether the data is null or not.
     * @return bool 为空返回true，否则返回false
     * @return bool: true, is null ; false, not null
     */
    bool empty() const;

protected:
	TC_CasQueue(const TC_CasQueue&) = delete;
	TC_CasQueue(TC_CasQueue&&) = delete;
	TC_CasQueue& operator=(const TC_CasQueue&) = delete;
	TC_CasQueue& operator=(TC_CasQueue&&) = delete;

protected:
    /**
     * 队列
     * Queue
     */
    queue_type          _queue;

    /**
     * 队列长度
     * The length of a queue
     */
    size_t              _size;

	//锁Lock
	TC_SpinLock         _mutex;
};

template<typename T, typename D> T TC_CasQueue<T, D>::front()
{
	TC_LockT<TC_SpinLock> lock (_mutex);

	return  _queue.front();
}

template<typename T, typename D> bool TC_CasQueue<T, D>::pop_front(T& t)
{
    TC_LockT<TC_SpinLock> lock (_mutex);
    if (_queue.empty())
    {
        return false;
    }

    t = _queue.front();
    _queue.pop_front();
    assert(_size > 0);
    --_size;

    return true;
}


template<typename T, typename D> bool TC_CasQueue<T, D>::pop_front()
{
	TC_LockT<TC_SpinLock> lock (_mutex);
	if (_queue.empty())
	{
		return false;
	}

	_queue.pop_front();
	assert(_size > 0);
	--_size;

	return true;
}

template<typename T, typename D> void TC_CasQueue<T, D>::push_back(const T& t)
{
    TC_LockT<TC_SpinLock> lock (_mutex);
    _queue.push_back(t);
    ++_size;
}

template<typename T, typename D> void TC_CasQueue<T, D>::push_back(const queue_type &qt)
{
    TC_LockT<TC_SpinLock> lock (_mutex);

    typename queue_type::const_iterator it = qt.begin();
    typename queue_type::const_iterator itEnd = qt.end();
    while (it != itEnd) {
        _queue.push_back(*it);
        ++it;
        ++_size;
    }
}

template<typename T, typename D> void TC_CasQueue<T, D>::push_front(const T& t)
{
    TC_LockT<TC_SpinLock> lock (_mutex);

    _queue.push_front(t);

    ++_size;
}

template<typename T, typename D> void TC_CasQueue<T, D>::push_front(const queue_type &qt)
{
    TC_LockT<TC_SpinLock> lock (_mutex);

    typename queue_type::const_iterator it = qt.begin();
    typename queue_type::const_iterator itEnd = qt.end();
    while (it != itEnd) {
        _queue.push_front(*it);
        ++it;
        ++_size;

    }
}

template<typename T, typename D> bool TC_CasQueue<T, D>::swap(queue_type &q)
{
    TC_LockT<TC_SpinLock> lock (_mutex);

    if (_queue.empty()) {
        return false;
    }

    q.swap(_queue);

    _size = _queue.size();

    return true;
}

template<typename T, typename D> size_t TC_CasQueue<T, D>::size() const
{
	TC_LockT<TC_SpinLock> lock(_mutex);
    return _size;
}

template<typename T, typename D> void TC_CasQueue<T, D>::clear()
{
	TC_LockT<TC_SpinLock> lock(_mutex);
    _queue.clear();
    _size = 0;
}

template<typename T, typename D> bool TC_CasQueue<T, D>::empty() const
{
	TC_LockT<TC_SpinLock> lock(_mutex);
    return _queue.empty();
}

}
#endif

