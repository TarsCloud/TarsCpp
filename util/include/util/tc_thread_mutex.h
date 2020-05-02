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

#ifndef __TC_THREAD_MUTEX_H
#define __TC_THREAD_MUTEX_H

#include "util/tc_lock.h"
#include "util/tc_platform.h"
#include <mutex>
#include <atomic>

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_thread_mutex.h 
 * @brief 线程锁互斥类(兼容TAF4.x版本, 底层直接封装了c++11, 从而跨平台兼容)
 *  
 * @author jarodruan@upchina.com  
 */
             
/////////////////////////////////////////////////
class TC_ThreadCond;

/**
* @brief 线程锁 . 
*  
* 不可重复加锁，即同一个线程不可以重复加锁 
*  
* 通常不直接使用，和TC_Monitor配合使用，即TC_ThreadLock; 
*/
class UTIL_DLL_API TC_ThreadMutex
{
public:

    TC_ThreadMutex();
    virtual ~TC_ThreadMutex();

    /**
     * @brief 加锁
     */
    void lock() const;

    /**
     * @brief 尝试锁
     * 
     * @return bool
     */
    bool tryLock() const;

    /**
     * @brief 解锁
     */
    void unlock() const;

protected:

    // noncopyable
    TC_ThreadMutex(const TC_ThreadMutex&) = delete;
    void operator=(const TC_ThreadMutex&) = delete;

    friend class TC_ThreadCond;

protected:
    mutable std::mutex _mutex;
};

/**
* @brief 线程锁类. 
*  
* 采用线程库实现
**/
class UTIL_DLL_API TC_ThreadRecMutex
{
public:

    /**
    * @brief 构造函数
    */
    TC_ThreadRecMutex();

    /**
    * @brief 析够函数
    */
    virtual ~TC_ThreadRecMutex();

    /**
	* @brief 锁, 调用pthread_mutex_lock. 
	*  
    */
    void lock() const;

    /**
	* @brief 解锁, pthread_mutex_unlock. 
	*  
    */
    void unlock() const;

    /**
	* @brief 尝试锁, 失败抛出异常. 
	*  
    * return : true, 成功锁; false 其他线程已经锁了
    */
    bool tryLock() const;

protected:

	/**
     * @brief 友元类
     */
    friend class TC_ThreadCond;

private:
    /**
    锁对象
    */
    mutable recursive_mutex _mutex;
};

}
#endif

