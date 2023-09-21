#pragma once

#include <deque>
#include <iostream>
#include <list>
#include <cassert>
#include <map>
#include <chrono>
#include <mutex>
#include <functional>

namespace tars
{
/**
 * 带队列模式的map, 并可以根据超时时间淘汰历史数据
 * - 向队列中Push的时候, 也会按照先后顺序排序
 * - 调用timeout, 会将超时的数据从内部删除
 */
template<class K, class T>
class TC_TimeoutQueueMap
{
public:

	struct PtrInfo;

	struct NodeInfo;

	typedef std::map<K, PtrInfo> data_type;

	typedef std::list<NodeInfo> time_type;

	typedef std::function<void(const K & k, T &)> data_functor;

	struct PtrInfo
	{
		T ptr;

		typename time_type::iterator timeIter;
	};

	struct NodeInfo
	{
		bool hasPoped;

		int64_t createTime;

		typename data_type::iterator dataIter;
	};

	/**
	 * @brief 超时队列，缺省5s超时.
	 *
	 * @param timeout 超时设定时间
	 * @param size
	 */
	TC_TimeoutQueueMap(int timeout = 5 * 1000)
		: _timeout(timeout)
	{
		_firstNoPopIter = _time.end();
	}

	/**
	 * @brief 设置超时时间(毫秒).
	 *
	 * @param timeout
	 */
	void setTimeout(time_t timeout)
	{
		_timeout = timeout;
	}

	/**
	 * @brief 获取超时时间
	 * @return [description]
	 */
	time_t getTimeout() const
	{
		return _timeout;
	}

	/**
	 * 清空数据
	 */
	void clear();

    /**
     * @brief 是否存在key
     *
     * @param k 指定的数据的key
     * @return bool, true: 存在, false: 不存在
     */
    bool has(const K & k);

    /**
     * @brief 是否存在key
     *
     * @param k 指定的数据的key
     * @return T, 如果获取不到, 则抛出异常
     */
    T get(const K & k);

    /**
	 * @brief 获取指定key的数据.
	 *
	 * @param k 指定的数据的key
	 * @param t 获取指定key的数据
	 * @return bool, true: 获取到数据, false: 没有获取到数据
	 */
	bool get(const K & k, T & t, bool bErase = true);

	/**
	 * @brief, 获取数据并更新时间链, 从而能够不超时
	 * @param k 指定的数据的key
	 * @param t 获取指定key的数据
	 * @return bool, true: 获取到数据, false: 没有获取到数据
	 */
	bool getAndRefresh(const K & k, T & t);

	/**
	 * @brief 删除.
	 *
	 * @param k 指定的数据的key
	 * @return bool, true: 数据存在, false: 数据不存在
	 */
	bool erase(const K & k);

	/**
	 * @brief 删除.
	 *
	 * @param k 指定的数据的key
	 * @param t 如果数据存在, 则返回删除的数据
	 * @return bool, true: 数据存在, false: 数据不存在
	 */
	bool erase(const K & k, T & t);

	/**
	 * @brief 设置消息到队列尾端, 如果key存在, 则覆盖之前的
	 *
	 * @param k      数据的key
	 * @param ptr    要插入到队列尾端的消息
	 */
	void push(const K & k, const T & ptr);

	/**
	 * @brief 超时删除数据
	 */
	void timeout();

	/**
	 * @brief 删除超时的数据，并用df对数据做处理
	 */
	void timeout(const data_functor & df);

	/**
	 * @brief 取出队列头部的消息
	 *
	 * @param t
	 * @return true: 存在数据, false: 没有数据
	 */
	bool pop(T & t);

    /**
     * 获取队列头部数据, 如果没有则抛出异常
     * @return
     */
    std::pair<K, T> front();

	/**
	 * @brief 队列中的数据.
	 *
	 * @return size_t
	 */
	size_t size() const
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _data.size();
	}

	/**
	 * @brief is empty
	 * @return
	 */
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _data.empty();
	}

protected:
	int64_t _timeout;
	data_type _data;
	time_type _time;
	typename time_type::iterator _firstNoPopIter;
	mutable std::mutex _mutex;
};

template<typename K, typename T>
void TC_TimeoutQueueMap<K, T>::clear()
{
	std::lock_guard<std::mutex> lock(_mutex);
	_data.clear();
	_time.clear();
	_firstNoPopIter = _time.end();
}


template<typename K, typename T>
bool TC_TimeoutQueueMap<K, T>::has(const K & k)
{
    std::lock_guard<std::mutex> lock(_mutex);
    typename data_type::iterator it = _data.find(k);

    return it != _data.end();
}

template<typename K, typename T>
T TC_TimeoutQueueMap<K, T>::get(const K & k)
{
    std::lock_guard<std::mutex> lock(_mutex);
    typename data_type::iterator it = _data.find(k);

    if(it != _data.end())
    {
        return it->second.ptr;
    }

    std::runtime_error("no data");
    return it->second.ptr;
}

template<typename K, typename T>
bool TC_TimeoutQueueMap<K, T>::get(const K & k, T & t, bool bErase)
{
	std::lock_guard<std::mutex> lock(_mutex);

	typename data_type::iterator it = _data.find(k);

	if (it == _data.end())
	{
		return false;
	}

	t = it->second.ptr;

	if (bErase)
	{
		if (_firstNoPopIter == it->second.timeIter)
		{
			++_firstNoPopIter;
		}
		_time.erase(it->second.timeIter);
		_data.erase(it);
	}

	return true;
}

template<typename K, typename T>
bool TC_TimeoutQueueMap<K, T>::getAndRefresh(const K & k, T & t)
{
	std::lock_guard<std::mutex> lock(_mutex);

	typename data_type::iterator it = _data.find(k);

	if (it == _data.end())
	{
		return false;
	}

	t = it->second.ptr;

	//从时间队列中删除
	if (_firstNoPopIter == it->second.timeIter)
	{
		++_firstNoPopIter;
	}
	_time.erase(it->second.timeIter);

	//再插入到时间队列末尾
	NodeInfo ni;

	ni.createTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	ni.dataIter = it;

	ni.hasPoped = false;

	_time.push_back(ni);

	typename time_type::iterator tmp = _time.end();

	--tmp;

	it->second.timeIter = tmp;

	if (_firstNoPopIter == _time.end())
	{
		_firstNoPopIter = tmp;
	}

	return true;
}

template<typename K, typename T>
void TC_TimeoutQueueMap<K, T>::push(const K & k, const T & ptr)
{
	std::lock_guard<std::mutex> lock(_mutex);

	typename data_type::iterator it = _data.find(k);

	//如果重复插入的数据, 先从时间队列中删除
	if (it != _data.end())
	{
		if (_firstNoPopIter == it->second.timeIter)
		{
			_firstNoPopIter++;
		}
		_time.erase(it->second.timeIter);
		it->second.ptr = ptr;
	}
	else
	{
		PtrInfo pi;
		pi.ptr = ptr;

		//写入新数据, 返回新的迭代器内容
		std::pair<typename data_type::iterator, bool> result;

		result = _data.insert(make_pair(k, pi));

		it = result.first;
	}

	//更新时间队列
	NodeInfo ni;

	ni.createTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	ni.dataIter = it;

	ni.hasPoped = false;

	_time.push_back(ni);

	typename time_type::iterator tmp = _time.end();

	--tmp;

	it->second.timeIter = tmp;

	if (_firstNoPopIter == _time.end())
	{
		_firstNoPopIter = tmp;
	}
}

template<typename K, typename T>
void TC_TimeoutQueueMap<K, T>::timeout()
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	while (true)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		typename time_type::iterator it = _time.begin();

		if (it != _time.end() && ms - it->createTime > _timeout)
		{
			_data.erase(it->dataIter);

			if (_firstNoPopIter == it)
			{
				++_firstNoPopIter;
			}
			_time.erase(it);
		}
		else
		{
			break;
		}
	}
}

template<typename K, typename T>
void TC_TimeoutQueueMap<K, T>::timeout(const data_functor & df)
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	while (true)
	{
		{
			_mutex.lock();

			typename time_type::iterator it = _time.begin();

			if (it != _time.end() && ms - it->createTime > _timeout)
			{
				auto v = *it->dataIter;

				_data.erase(it->dataIter);

				if (_firstNoPopIter == it)
				{
					_firstNoPopIter++;
				}
				_time.erase(it);

				_mutex.unlock();

				try { df(v.first, v.second.ptr); } catch (...) {}
			}
			else
			{
				_mutex.unlock();
				break;
			}
		}

	}
}

template<typename K, typename T>
bool TC_TimeoutQueueMap<K, T>::erase(const K & k)
{
	std::lock_guard<std::mutex> lock(_mutex);

	typename data_type::iterator it = _data.find(k);

	if (it == _data.end())
	{
		return false;
	}

	if (_firstNoPopIter == it->second.timeIter)
	{
		_firstNoPopIter++;
	}
	_time.erase(it->second.timeIter);

	_data.erase(it);

	return true;
}

template<typename K, typename T>
bool TC_TimeoutQueueMap<K, T>::erase(const K & k, T & t)
{
	std::lock_guard<std::mutex> lock(_mutex);

	typename data_type::iterator it = _data.find(k);

	if (it == _data.end())
	{
		return false;
	}

	t = it->second.ptr;

	if (_firstNoPopIter == it->second.timeIter)
	{
		_firstNoPopIter++;
	}
	_time.erase(it->second.timeIter);

	_data.erase(it);

	return true;
}

template<typename K, typename T>
bool TC_TimeoutQueueMap<K, T>::pop(T & ptr)
{
	std::lock_guard<std::mutex> lock(_mutex);

	if (_time.empty())
	{
		return false;
	}

	typename time_type::iterator it = _time.begin();

	if (it->hasPoped == true)
	{
		it = _firstNoPopIter;
	}

	if (it == _time.end())
	{
		return false;
	}

	assert(it->hasPoped == false);

	ptr = it->dataIter->second.ptr;

	it->hasPoped = true;

	_firstNoPopIter = it;

	++_firstNoPopIter;

	return true;
}

template<typename K, typename T>
std::pair<K, T> TC_TimeoutQueueMap<K, T>::front()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_time.empty())
    {
        throw std::runtime_error("no data exists");
    }

    typename time_type::iterator it = _time.begin();

    if (it->hasPoped == true)
    {
        it = _firstNoPopIter;
    }

    if (it == _time.end())
    {
        throw std::runtime_error("no data exists");
    }

    assert(it->hasPoped == false);

    return std::make_pair(it->dataIter->first, it->dataIter->second.ptr);
}
}
