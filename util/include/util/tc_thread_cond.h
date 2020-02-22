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

#ifndef _TC_THREAD_COND_H
#define _TC_THREAD_COND_H

#include <cerrno>
#include <iostream>
#include <condition_variable>
#include "util/tc_platform.h"
#include "util/tc_ex.h"

namespace tars
{
/////////////////////////////////////////////////
/**
 * @file tc_thread_cond.h 
 * @brief 线程锁以及条件变量类(兼容TAF4.x版本, 底层直接封装了c++11, 从而跨平台兼容)
 *  
 * @author  jarodruan@upchina.com
 */             
/////////////////////////////////////////////////
class TC_ThreadMutex;

/**
 *  @brief 线程信号条件类, 所有锁可以在上面等待信号发生
 *  
 *  和TC_ThreadMutex、TC_ThreadRecMutex配合使用,
 *  
 *  通常不直接使用，而是使用TC_ThreadLock/TC_ThreadRecLock;
 */
class TC_ThreadCond
{
public:

    /**
     *  @brief 构造函数
     */
    TC_ThreadCond();

    /**
     *  @brief 析构函数
     */
    ~TC_ThreadCond();

    /**
     *  @brief 发送信号, 等待在该条件上的一个线程会醒
     */
    void signal();

    /**
     *  @brief 等待在该条件的所有线程都会醒
     */
    void broadcast();

    /**
	 *  @brief 无限制等待.
	 *  
     * @param M
     */
    template<typename Mutex>
    void wait(const Mutex& mutex) const
    {
        _cond.wait(mutex._mutex);
    }

    /**
	 * @brief 等待时间. 
	 *  
	 * @param M 
     * @return bool, false表示超时, true:表示有事件来了
     */
    template<typename Mutex>
    bool timedWait(const Mutex& mutex, int millsecond) const
    {
        if(_cond.wait_for(mutex._mutex, std::chrono::milliseconds(millsecond))==std::cv_status::timeout)
        {
            return false;
        }
        return true;
    }

protected:
    // Not implemented; prevents accidental use.
    TC_ThreadCond(const TC_ThreadCond&);
    TC_ThreadCond& operator=(const TC_ThreadCond&);

private:

    /**
     * 线程条件
     */
    mutable std::condition_variable_any _cond;

};

}

#endif

