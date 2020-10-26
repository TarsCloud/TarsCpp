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

#ifndef __TC_THREAD_H_
#define __TC_THREAD_H_

#include <sys/types.h>
#include <signal.h>
// #include <pthread.h>
#include <thread>
#include "util/tc_ex.h"
#include "util/tc_monitor.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_thread.h 
 * @brief  线程类(兼容TAF4.x版本, 底层直接封装了c++11 thread, 从而跨平台兼容)
 *  
 * @author ruanshudong@qq.com  
 */          
/////////////////////////////////////////////////

/**
 * @brief  线程控制异常类
 */
struct TC_ThreadThreadControl_Exception : public TC_Exception
{
    TC_ThreadThreadControl_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_ThreadThreadControl_Exception() throw() {};
};

/**
 * @brief  线程控制类
 */
class TC_ThreadControl
{
public:

    /**
	 * @brief  构造, 表示当前运行的线程，
	 *  		join和detach在不能在该对象上调用
	*/
    explicit TC_ThreadControl(std::thread *th);

    /**
     * @brief  等待当前线程结束, 不能在当前线程上调用
     */
    void join();

    /**
     * @brief  detach, 不能在当前线程上调用
     */
    void detach();

    /**
     * @brief  获取当前线程id.
     *
     * @return pthread_t当前线程id
     */
    std::thread::id id() const;

    /**
	 * @brief  休息ms时间. 
	 *  
     * @param millsecond 休息的时间，具体ms数字
     */
    static void sleep(int64_t millsecond);
    
    /**
     * @brief  交出当前线程控制权
     */
    static void yield();

private:

    std::thread *_th;
};

/**
 *
 */
class TC_Runable
{
public:
    virtual ~TC_Runable(){};
    virtual void run() = 0;
};

/**
 * @brief 线程基类. 
 * 线程基类，所有自定义线程继承于该类，同时实现run接口即可, 
 *  
 * 可以通过TC_ThreadContorl管理线程。
 */
class TC_Thread : public TC_Runable
{
public:

	/**
     * @brief  构造函数
	 */
	TC_Thread();

	/**
     * @brief  析构函数
	 */
	virtual ~TC_Thread();

	/**
     * @brief  线程运行
	 */
	TC_ThreadControl start();

    /**
     * @brief  获取线程控制类.
     *
     * @return ThreadControl
     */
    TC_ThreadControl getThreadControl();

    /**
     * @brief  线程是否存活.
     *
     * @return bool 存活返回true，否则返回false
     */
    bool isAlive() const;

	/**
     * @brief  获取线程id.
	 *
	 * @return std::thread::id 线程id
	 */
	std::thread::id id() { return _th->get_id(); }

	/**
     * @brief  获取线程
	 *
	 * @return std::thread::id 线程id
	 */
    std::thread* getThread() { return _th; }

	/**
     * @brief  获取当前线程ID, 用size_t返回
	 *
	 * @return 
	 */
    static size_t CURRENT_THREADID();
protected:

	/**
	 * @brief  静态函数, 线程入口. 
	 *  
	 * @param pThread 线程对象
	 */
	static void threadEntry(TC_Thread *pThread);

	/**
     * @brief  运行
	 */
    virtual void run() = 0;

protected:
    /**
     * 是否在运行
     */
    bool            _running;

    /**
     * 线程锁
     */
    TC_ThreadLock   _lock;

    //当前线程
    std::thread     *_th;
};

}
#endif

