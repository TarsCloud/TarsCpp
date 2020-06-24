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

#ifndef _TC_LOCK_H
#define _TC_LOCK_H

#include <string>
#include <stdexcept>
#include <cerrno>
#include "util/tc_ex.h"

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/**
 * @file tc_lock.h 
 * @brief  锁类 
 * @brief  Lock Class
 */           
/////////////////////////////////////////////////


/**
* @brief  锁异常
* @brief  Lock exception
*/
struct TC_Lock_Exception : public TC_Exception
{
    TC_Lock_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_Lock_Exception() throw() {};
};

/**
 * @brief  锁模板类其他具体锁配合使用，
 * 构造时候加锁，析够的时候解锁
 * @brief  Lock template class with other specific locks, 
 * lock when construction, unlock when analysis is sufficient
 */
template <typename T>
class TC_LockT
{
public:

    /**
     * @brief  构造函数，构造时枷锁
     * @brief  Constructor, flail when constructing
     *  
     * @param mutex 锁对象
     * @param mutex Lock Object
     */
    TC_LockT(const T& mutex) : _mutex(mutex)
    {
        _mutex.lock();
        _acquired = true;
    }

    /**
     * @brief  析构，析构时解锁
     * @brief  Destructor, unlock at destruct
     */
    virtual ~TC_LockT()
    {
        if (_acquired)
        {
            _mutex.unlock();
        }
    }

    /**
     * @brief  上锁, 如果已经上锁,则抛出异常
     * @brief  Lock, throw an exception if already locked
     */
    void acquire() const
    {
        if (_acquired)
        {
            throw TC_Lock_Exception("thread has locked!");
        }
        _mutex.lock();
        _acquired = true;
    }

    /**
     * @brief  尝试上锁.
     * @brief  Try to lock.
     *
     * @return  成功返回true，否则返回false
     * @return  If it is successful,return true,otherwise return false
     */
    bool tryAcquire() const
    {
        _acquired = _mutex.tryLock();
        return _acquired;
    }

    /**
     * @brief  释放锁, 如果没有上过锁, 则抛出异常
     * @brief  Release lock, throw exception if no lock is on
     */
    void release() const
    {
        if (!_acquired)
        {
            throw TC_Lock_Exception("thread hasn't been locked!");
        }
        _mutex.unlock();
        _acquired = false;
    }

    /**
     * @brief  是否已经上锁.
     * @brief  Is locked or not
     *
     * @return  返回true已经上锁，否则返回false
     * @return  Returning true indicates that it is locked; otherwise returning false
     */
    bool acquired() const
    {
        return _acquired;
    }

protected:

    /**
	 * @brief 构造函数
	 * 用于锁尝试操作，与TC_LockT相似
     * @brief Constructor
     * For lock attempt operations, with TC_LockT Similarity
	 *  
     */
    TC_LockT(const T& mutex, bool) : _mutex(mutex)
    {
        _acquired = _mutex.tryLock();
    }

private:

    // Not implemented; prevents accidental use.
    TC_LockT(const TC_LockT&);
    TC_LockT& operator=(const TC_LockT&);

protected:
    /**
     * 锁对象
     * Lock object
     */
    const T&        _mutex;

    /**
     * 是否已经上锁
     * Is it locked or not
     */
    mutable bool _acquired;
};

/**
 * @brief  尝试上锁
 * @brief  Attempt to lock
 */
template <typename T>
class TC_TryLockT : public TC_LockT<T>
{
public:

    TC_TryLockT(const T& mutex) : TC_LockT<T>(mutex, true)
    {
    }
};

/**
 * @brief  空锁, 不做任何锁动作
 * @brief  Empty lock, no lock action
 */
class TC_EmptyMutex
{
public:
    /**
	* @brief  写锁.
    * @brief  Write lock
	*  
    * @return int, 0 正确
    * @return int  0  right
    */
    int lock()  const   {return 0;}

    /**
    * @brief  解写锁
    * @brief  Unlock
    */
    int unlock() const  {return 0;}

    /**
	* @brief  尝试解锁. 
    * @brief  Try to unlock
	*  
    * @return int, 0 正确
    * @return int, 0 right
    */
    bool trylock() const {return true;}
};

/**
 * @brief  读写锁读锁模板类
 * 构造时候加锁，析够的时候解锁
 * @brief  Read-Write Lock Read Lock Template Class
 * Lock when construction, unlock when analysis is sufficient
 */

template <typename T>
class TC_RW_RLockT
{
public:
    /**
	 * @brief  构造函数，构造时枷锁
     * @brief  Constructor, flail when constructing
	 *
     * @param lock 锁对象
     * @param lock lock object
     */
	TC_RW_RLockT(T& lock)
		: _rwLock(lock),_acquired(false)
	{
		_rwLock.readLock();
		_acquired = true;
	}

    /**
	 * @brief 析构时解锁
     * @brief Unlock at destruct time
     */
	~TC_RW_RLockT()
	{
		if (_acquired)
		{
			_rwLock.unReadLock();
		}
	}
private:
	/**
	 *锁对象
     *lock object
	 */
	const T& _rwLock;

    /**
     * 是否已经上锁
     * Is it locked or not
     */
    mutable bool _acquired;

	TC_RW_RLockT(const TC_RW_RLockT&);
	TC_RW_RLockT& operator=(const TC_RW_RLockT&);
};

template <typename T>
class TC_RW_WLockT
{
public:
    /**
	 * @brief  构造函数，构造时枷锁
     * @brief  Constructor, Constructive Flail
	 *
     * @param lock 锁对象
     * @param lock Lock Object
     */
	TC_RW_WLockT(T& lock)
		: _rwLock(lock),_acquired(false)
	{
		_rwLock.writeLock();
		_acquired = true;
	}
    /**
	 * @brief 析构时解锁
     * @brief Unlock at destruct time
     */
	~TC_RW_WLockT()
	{
		if(_acquired)
		{
			_rwLock.unWriteLock();
		}
	}
private:
	/**
	 *锁对象
     *Lock Object
	 */
	const T& _rwLock;
    /**
     * 是否已经上锁
     * Is it locked or not
     */
    mutable bool _acquired;

	TC_RW_WLockT(const TC_RW_WLockT&);
	TC_RW_WLockT& operator=(const TC_RW_WLockT&);
};

};
#endif

