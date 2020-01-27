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

#ifndef __TC_THREAD_POOL_H_
#define __TC_THREAD_POOL_H_

#include "util/tc_platform.h"
#include "util/tc_thread.h"
#include <future>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <mutex>
#include <utility>

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_thread_pool.h
 * @brief 线程池类,采用c++11来实现了
 * 使用说明:
 * TC_ThreadPool tpool;
 * tpool.init(5);   //初始化线程池线程数 
 * //启动线程有两种方式
 * //第一种, 直接启动
 * tpool.start();
 * //第二种, 启动时指定初始化函数, 比如定义函数
 * void testFunction(int i)
 * {
 *     cout << i << endl;
 * }
 * tpool.start(testFunction, 5);    //start的第一函数是std::bind返回的函数(std::function), 后面跟参数
 * //将任务丢到线程池中
 * tpool.exec(testFunction, 10);    //参数和start相同
 * //等待线程池结束, 有两种方式:
 * //第一种等待线程池中无任务
 * tpool.waitForAllDone(1000);      //参数<0时, 表示无限等待(注意有人调用stop也会推出)
 * //第二种等待外部有人调用线程池的stop函数
 * tpool.waitForStop(1000);
 * //此时: 外部需要结束线程池是调用
 * tpool.stop();
 * 注意:
 * TC_ThreadPool::exec执行任务返回的是个future, 因此可以通过future异步获取结果, 比如:
 * int testInt(int i)
 * {
 *     return i;
 * }
 * auto f = tpool.exec(testInt, 5);
 * cout << f.get() << endl;   //当testInt在线程池中执行后, f.get()会返回数值5
 *
 * class Test
 * {
 * public:
 *     int test(int i);
 * };
 * Test t;
 * auto f = tpool.exec(std::bind(&Test::test, &t, std::placeholders::_1), 10);
 * //返回的future对象, 可以检查是否执行
 * cout << f.get() << endl;
 * @author  jarodruan@upchina.com
 */
/////////////////////////////////////////////////
/**
* @brief 线程异常
*/
struct TC_ThreadPool_Exception : public TC_Exception
{
    TC_ThreadPool_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_ThreadPool_Exception(const string &buffer, bool err) : TC_Exception(buffer, err){};
    ~TC_ThreadPool_Exception() throw(){};
};

/**
 * @brief 用通线程池类(采用c++11实现)
 *  
 * 使用方式说明:
 * 具体示例代码请参见:examples/util/example_tc_thread_pool.cpp
 */
class TC_ThreadPool
{
  public:
    /**
     * @brief 构造函数
     *
     */
    TC_ThreadPool();

    /**
     * @brief 析构, 会停止所有线程
     */
    ~TC_ThreadPool();

    /**
     * @brief 初始化. 
     *  
     * @param num 工作线程个数
     */
    void init(size_t num);

    /**
     * @brief 获取线程个数.
     *
     * @return size_t 线程个数
     */
    size_t getThreadNum()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        
        return _threads.size();
    }

    /**
     * @brief 获取当前线程池的任务数
     *
     * @return size_t 线程池的任务数
     */
    size_t getJobNum()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _tasks.size();
    }

    /**
     * @brief 停止所有线程, 会等待所有线程结束
     */
    void stop();

    /**
     * @brief 启动所有线程
     */
    void start();

    /**
     * @brief 用线程池启用任务(F是function, Args是参数)
     *  
     * @param ParentFunctor
     * @param tf
     * @return 返回任务的future对象, 可以通过这个对象来获取返回值
     */
    template <class F, class... Args>
    auto exec(F &&f, Args &&... args) -> std::future<decltype(f(args...))>
    {
        //定义返回值类型
        using RetType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<RetType> res = task->get_future();

        std::unique_lock<std::mutex> lock(_mutex); 

        _tasks.emplace([task]() { (*task)(); });

        _condition.notify_one();

        return res;

    }

    /**
     * @brief 等待当前任务队列中, 所有工作全部结束(队列无任务).
     *
     * @param millsecond 等待的时间(ms), -1:永远等待
     * @return           true, 所有工作都处理完毕 
     *                   false,超时退出
     */
    bool waitForAllDone(int millsecond = -1);

//    /**
//     * @brief 可以等待所有线程结束
//     *
//     * @param millsecond 等待的时间(ms), -1:永远等待
//     * @return           true, 通知了stop
//     *                   false,超时退出
//     */
//    bool waitForStop(int millsecond = -1);

  protected:
    /**
     * @brief 获取任务
     *
     * @return std::function<void()>
     */
    bool get(std::function<void()> &task);

    /**
     * @brief 线程池是否退出
     */
    bool isTerminate() { return _bTerminate; }

    /**
     * @brief 线程运行态
     */
    void run();

  protected:

    /**
     * 任务队列
     */
    queue<std::function<void()>> _tasks;

    /**
     * 工作线程
     */
    std::vector<std::thread*> _threads;

    std::mutex                _mutex;

    std::condition_variable   _condition;

    size_t                    _threadNum;

    bool                      _bTerminate;

    std::atomic<int>          _atomic{0};
};
}
#endif

