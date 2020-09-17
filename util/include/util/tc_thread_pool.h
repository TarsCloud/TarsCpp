#pragma once

#include "util/tc_platform.h"
#include "util/tc_thread.h"
#include "util/tc_timeprovider.h"
#include <future>
#include <functional>
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
    TC_ThreadPool_Exception(const string& buffer) : TC_Exception(buffer) {};
    TC_ThreadPool_Exception(const string& buffer, bool err) : TC_Exception(buffer, err) {};
    ~TC_ThreadPool_Exception() throw() {};
};

/**
* @brief 用通线程池类(采用c++11实现)
*
* 使用方式说明:
* 具体示例代码请参见:examples/util/example_tc_thread_pool.cpp
*/
class TC_ThreadPool
{
protected:
    struct TaskFunc
    {
        TaskFunc(uint64_t expireTime) : _expireTime(expireTime)
        { }

        std::function<void()>   _func;
        int64_t                _expireTime = 0;	//超时的绝对时间
    };
    typedef shared_ptr<TaskFunc> TaskFuncPtr;
public:
    /**
    * @brief 构造函数
    *
    */
    TC_ThreadPool();

    /**
    * @brief 析构, 会停止所有线程
    */
    virtual ~TC_ThreadPool();

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
    auto exec(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        return exec(0,f,args...);
    }

    /**
    * @brief 用线程池启用任务(F是function, Args是参数)
    *
    * @param 超时时间 ，单位ms (为0时不做超时控制) ；若任务超时，此任务将被丢弃
    * @param bind function
    * @return 返回任务的future对象, 可以通过这个对象来获取返回值
    */
    template <class F, class... Args>
    auto exec(int64_t timeoutMs, F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        int64_t expireTime =  (timeoutMs == 0 ? 0 : TNOWMS + timeoutMs);
        //定义返回值类型
        using RetType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        TaskFuncPtr fPtr = std::make_shared<TaskFunc>(expireTime);
        fPtr->_func = [task]() {
            (*task)();
        };

        std::unique_lock<std::mutex> lock(_mutex);
        _tasks.push(fPtr);
        _condition.notify_one();

        return task->get_future();;
    }

    /**
    * @brief 等待当前任务队列中, 所有工作全部结束(队列无任务).
    *
    * @param millsecond 等待的时间(ms), -1:永远等待
    * @return           true, 所有工作都处理完毕
    *                   false,超时退出
    */
    bool waitForAllDone(int millsecond = -1);

protected:
    /**
    * @brief 获取任务
    *
    * @return TaskFuncPtr
    */
    bool get(TaskFuncPtr&task);

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
    queue<TaskFuncPtr> _tasks;

    /**
    * 工作线程
    */
    std::vector<std::thread*> _threads;

    std::mutex                _mutex;

    std::condition_variable   _condition;

    size_t                    _threadNum;

    bool                      _bTerminate;

    std::atomic<int>          _atomic{ 0 };
};


//根据keyhash的线程池
class TC_ThreadPoolHash
{
public:
    TC_ThreadPoolHash();
    virtual ~TC_ThreadPoolHash();

    TC_ThreadPoolHash(const TC_ThreadPoolHash&) = delete;
    TC_ThreadPoolHash& operator = (const TC_ThreadPoolHash&) = delete;

    /**
    * @brief 初始化.
    *
    * @param num 工作线程个数
    */
    void init(size_t num);


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
    * @param hashkey 根据次key保证将同样的任务hash到同样的队列中处理
    * @param tf
    * @return 返回任务的future对象, 可以通过这个对象来获取返回值
    */
    template <class F, class... Args>
    auto exec(const string& hashkey, F&& f, Args&&... args)->std::future<decltype(f(args...))>
    {
        return exec(hashkey,0, f, args...);
    }

    /**
    * @brief 用线程池启用任务(F是function, Args是参数)
    * @param 超时时间 ，单位ms (为0时不做超时控制) ；若任务超时，此任务将被丢弃
    * @param hashkey 根据次key保证将同样的任务hash到同样的队列中处理
    * @param tf
    * @return 返回任务的future对象, 可以通过这个对象来获取返回值
    */
    template <class F, class... Args>
    auto exec(const string& hashkey,int64_t timeoutMs, F&& f, Args&&... args)->std::future<decltype(f(args...))>
    {
        TC_ThreadPool* thread = selectThread(hashkey);
        if (thread)
        {
            return thread->exec(timeoutMs,f, args...);
        }
        else
        {
            throw TC_ThreadPool_Exception("[TC_ThreadPoolHash::start] no worker thread!");
        }
    }

protected:
    TC_ThreadPool* selectThread(const string& hashkey);

protected:
private:
    vector<TC_ThreadPool*> _pools;

};

}
