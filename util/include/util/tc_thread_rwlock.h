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

#ifndef __TC_THREAD_RWLOCK_H
#define __TC_THREAD_RWLOCK_H

#include <mutex>
#include <condition_variable>
#include "util/tc_lock.h"
#include "util/tc_platform.h"
#include <functional>

using namespace std;

namespace tars
{

/////////////////////////////////////////////////
/**
 * @file tc_thread_rwlock.h
 * @brief 读写锁
 * @author ruanshudong@qq.com
 */

/////////////////////////////////////////////////

class UTIL_DLL_API TC_ThreadRWLocker
{
protected:

	class TC_SharedMutex
	{
	public:
		//获取读锁
		void readLock()
		{
			//加锁, 直到可读
			unique_lock<mutex> lck(_mutex);
			_cond.wait(lck, std::bind(&TC_SharedMutex::isReadCond, this));
			_readCount++;
		}

		bool tryReadLock()
		{
			unique_lock<mutex> lck(_mutex);
			if(_cond.wait_for(lck, std::chrono::seconds(0)) == std::cv_status::timeout)
			{
				return false;
			}

			if(isReadCond())
			{
				_readCount++;
				return true;
			}

			return false;
		}

		//解读锁
		void unReadLock()
		{
			unique_lock<mutex> lck(_mutex);
			_readCount--;
			_cond.notify_all();
		}
		
		void writeLock()
		{
			unique_lock<mutex> lck(_mutex);
			_cond.wait(lck, bind([](const bool *is_w, const size_t *read_c) -> bool
			{
				return false == *is_w && 0 == *read_c;
			}, &_isWrite, &_readCount));

			_isWrite = true;
		}

		bool tryWriteLock()
		{
			unique_lock<mutex> lck(_mutex);
			if(_cond.wait_for(lck, std::chrono::seconds(0)) == std::cv_status::timeout)
			{
				return false;
			}

			if(isWriteCond())
			{
				_isWrite = true;
				return true;
			}

			return false;
		}

		void unWriteLock()
		{
			unique_lock<mutex> lck(_mutex);
			_isWrite = false;
			_cond.notify_all();
		}
	private:
		// 锁
		mutex _mutex;
		// 条件变量
		condition_variable _cond;
		// 是否在写
		bool _isWrite = false;
		// 读者数量
		size_t _readCount = 0;
		// 读条件
		bool isReadCond() const
		{
			return false == _isWrite;
		}
		// 写条件
		bool isWriteCond() const
		{
			return false == _isWrite && 0 == _readCount;
		}
	};

public:
	/**
     * @brief 构造函数
     */
	TC_ThreadRWLocker();

    /**
     * @brief 析够函数
     */
	~TC_ThreadRWLocker();

	/**
	 *@brief 读锁定
	 *return : 
	 */
	void readLock() const;

	/**
	 *@brief 写锁定,
	 *return : 失败
	 */
	void writeLock() const;

	/**
	 *@brief 尝试读锁定,
	 *return : 
	 */
	bool tryReadLock() const;

	/**
	 *@brief 尝试写锁定,
	 *return : 
	 */
	bool tryWriteLock() const ;

	/**
	 *@brief 解锁
	 *return : 
	 */
	void unReadLock() const;

	/**
	 *@brief 解锁
	 *return : 
	 */	
	void unWriteLock() const;

private:

	// mutable pthread_rwlock_t m_sect;

	mutable TC_SharedMutex _mutex;
	// noncopyable
	TC_ThreadRWLocker(const TC_ThreadRWLocker&);
	TC_ThreadRWLocker& operator=(const TC_ThreadRWLocker&);

};

typedef TC_RW_RLockT<TC_ThreadRWLocker> TC_ThreadRLock;
typedef TC_RW_WLockT<TC_ThreadRWLocker> TC_ThreadWLock;

}
#endif