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

#ifndef _TC_COROUTINES_H_
#define _TC_COROUTINES_H_

#include <cstddef>
#include <list>
#include <set>
#include <deque>
#include <map>
#include <functional>

#include "util/tc_fcontext.h"
#include "util/tc_thread_queue.h"
#include "util/tc_monitor.h"
#include "util/tc_thread.h"
#include "util/tc_epoller.h"

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/**
 * @file  tc_coroutine.h
 * @brief  协程操作封装类
 * @brief  coroutine encapsulation class
 *
 * 设计说明:
 * - 每个线程可以有多个协程, 协程底层使用的boost的几个主要的宏来实现协程间的跳转
 * - 协程需要使用的栈, 提前就要分配好, 通常启动协程的时候需要设置
 * - 协程调度运行是通过epoller来实现的, 这样可以完美的和网络IO关联到一起, 在网络IO过程中方便的完成协程的切换, 这个是当前携程调度的核心
 *
 * 主要类说明如下:
 * - TC_CoroutineInfo, 协程信息类, 每个协程都对应一个TC_CoroutineInfo对象, 协程切换本质就是切换TC_CoroutineInfo对象, 正常情况下业务不需要感知该对象
 * - TC_CoroutineScheduler, 协程调度器类, 负责管理和调度协程, 本质上就是管理和调度TC_CoroutineInfo
 * - TC_Coroutine, 协程类, 继承于线程类(TC_Thread), 用来给业务快速使用协程
 *
 * TC_CoroutineScheduler详细说明:
 * - 该类是协程调度的核心, 业务使用上, 框架需要和这个类打交道, 业务上除非自己来实现协程管理逻辑, 否则通常可以不深入了解该类的实现
 * - 注意每个协程都是需要使用栈空间的, 因此TC_CoroutineScheduler有init来初始化: <总共内存大小,栈大小>, 栈大小通常使用128k, 两者相除就是该调度协程器, 最大调度的协程个数
 * - 该类对象使用线程私有变量保存, 可以通过静态函数来创建/获取/设置
 * - 每个线程都有自己的调度器对象, 调度器对象只能调度自身线程的协程, 调度过程(运行run)本质上就是阻塞在线程的过程(run不会退出, 直到有terminate调用)
 * - 调度过程简单的理解就是: 检查是否有需要执行的协程, 有则执行之, 没有则等待在epoll对象上, 直到有唤醒或者超时
 * - 调度器底层使用tc_epoller来完成协程的切换, 等待和阻塞等操作, 可以和网络IO无缝粘合, 因此可以通过TC_CoroutineScheduler对象拿到TC_Epoller指针, 并用于网络IO上
 * - 由于网络IO也是用相同的epoller对象, 因此可以做到当有数据发送/接受时, 唤醒epoll对象, 从而完成协程的切换
 * - 协程启动通过: go 函数来完成
 * - 协程在运行中, 主要使用三个函数来完成, 调度控制: yield/sleep/put
 *
 * TC_Coroutine详细说明:
 * - 使用线程模拟协程组, 即多个协程同时被创建出来
 * - 业务可以直接继承这个类, 使用时, 首先要调用setCoroInfo方法设置协程的基本信息
 * - 实现这个类的: handle 方法, 然后类似启动线程一样(start)方法即可, 会同时有多个协程执行handle方法
 * - 调用start函数, 启动线程, 同时会创建(iNum, iMaxNum)个协程
 * - terminate结束
 */
/////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/**
 * @brief  协程异常类
 */
struct TC_CoroutineException : public TC_Exception
{
	TC_CoroutineException(const string &buffer) : TC_Exception(buffer){};
	TC_CoroutineException(const string &buffer, int err) : TC_Exception(buffer, err){};
	~TC_CoroutineException() throw() {};
};

/////////////////////////////////////////////
/**
 * 协程使用的栈内容信息
 */
struct stack_context
{
	std::size_t  size;
	void*        sp;

	stack_context()
		: size(0)
		, sp(0)
	{}
};

struct stack_traits
{
	static bool is_unbounded();

	static std::size_t page_size();

	static std::size_t default_size();

	static std::size_t minimum_size();

	static std::size_t maximum_size();

	static stack_context allocate(std::size_t);

	static void deallocate( stack_context &);
};


class TC_CoroutineScheduler;

///////////////////////////////////////////
/**
 * 协程信息类
 * 保存了协程的基本信息, 并将协程用链表的形式组织在一起
 */
class TC_CoroutineInfo
{
public:
	/**
 	* 协程的状态信息
 	*/
	enum CORO_STATUS
	{
		CORO_FREE       = 0,
		CORO_ACTIVE     = 1,
		CORO_AVAIL      = 2,
		CORO_INACTIVE   = 3,
		CORO_TIMEOUT    = 4
	};

	/////////////////////////////////////////////
	/*
	 * 协程内部使用的函数
	 */
	struct CoroutineFunc
	{
		std::function<void(void*, transfer_t)>  coroFunc;
		void*                                   args;
	};

    /**
     * 链表初始化
     */
    static inline void CoroutineHeadInit(TC_CoroutineInfo *coro)
    {
        coro->_next = coro;
        coro->_prev = coro;
    }

    /**
     * 链表是否为空
     */
    static inline bool CoroutineHeadEmpty(TC_CoroutineInfo *coro_head)
    {
        return coro_head->_next == coro_head;
    }

    /**
     * 插入
     */
    static inline void __CoroutineAdd(TC_CoroutineInfo *coro, TC_CoroutineInfo *prev, TC_CoroutineInfo *next)
    {
        next->_prev = coro;
        coro->_next = next;
        coro->_prev = prev;
        prev->_next = coro;
    }

    /**
     * 插入头部
     */
    static inline void CoroutineAdd(TC_CoroutineInfo *new_coro, TC_CoroutineInfo *coro_head)
    {
        __CoroutineAdd(new_coro, coro_head, coro_head->_next);
    }

    /**
     * 插入尾部
     */
    static inline void CoroutineAddTail(TC_CoroutineInfo *new_coro, TC_CoroutineInfo *coro_head)
    {
        __CoroutineAdd(new_coro, coro_head->_prev, coro_head);
    }

    /**
     * 删除
     */
    static inline void __CoroutineDel(TC_CoroutineInfo * prev, TC_CoroutineInfo * next)
    {
        next->_prev = prev;
        prev->_next = next;
    }

    /**
     * 删除
     */
    static inline void CoroutineDel(TC_CoroutineInfo *coro)
    {
        __CoroutineDel(coro->_prev, coro->_next);
        coro->_next = NULL;
        coro->_prev = NULL;
    }

    /**
     * 从一个链表移动到另外一个链表头部
     */
    static inline void CoroutineMove(TC_CoroutineInfo *coro, TC_CoroutineInfo *coro_head)
    {
        CoroutineDel(coro);
        CoroutineAdd(coro, coro_head);
    }

    /**
     * 从一个链表移动到另外一个链表尾部
     */
    static inline void CoroutineMoveTail(TC_CoroutineInfo *coro, TC_CoroutineInfo *coro_head)
    {
        CoroutineDel(coro);
        CoroutineAddTail(coro, coro_head);
    }

protected:
	//协程的入口函数
	static void corotineEntry(transfer_t q);

	//在协程里执行实际逻辑的入口函数
	static void corotineProc(void * args, transfer_t t);

public:
    /**
     * 构造函数
     */
    TC_CoroutineInfo();

    /**
     * 构造函数
     */
    TC_CoroutineInfo(TC_CoroutineScheduler* scheduler, uint32_t iUid, stack_context stack_ctx);

    /**
     * 析构函数
     */
    ~TC_CoroutineInfo();

    /**
     * 注册协程实际的处理函数
     */
    void registerFunc(const std::function<void ()> &callback);

    /**
     * 设置协程的内存空间
     */
    void setStackContext(stack_context stack_ctx);

    /**
     * 获取协程的内存空间
     */
    inline stack_context& getStackContext() { return _stack_ctx; }

    /**
     * 获取协程所处的调度器
     */
    inline TC_CoroutineScheduler* getScheduler() { return _scheduler; }

    /**
     * 获取协程的标志
     */
    inline uint32_t getUid() const { return _uid; }

    /**
     * 设置协程的标志
     */
    inline void setUid(uint32_t iUid) { _uid = iUid; }

    /**
     * 获取协程的状态
     */
    inline CORO_STATUS getStatus() const { return _eStatus; }

    /**
     * 设置协程的状态
     */
    inline void setStatus(CORO_STATUS status) { _eStatus = status; }

    /**
     * 获取协程所处的上下文
     */
	inline fcontext_t getCtx() const { return _ctx; }
	inline void setCtx(fcontext_t ctx) { _ctx = ctx; }
public:
    /*
     * 双向链表指针
     */
    TC_CoroutineInfo*                _prev;
    TC_CoroutineInfo*                _next;

private:

    /*
     * 协程所属的调度器
     */
    TC_CoroutineScheduler*        _scheduler;

    /*I
     * 协程的标识
     */
    uint32_t                      _uid;

    /*
     * 协程的状态
     */
    CORO_STATUS                   _eStatus;

    /*
     * 协程的内存空间
     */
    stack_context                _stack_ctx;

    /*
     * 创建协程后，协程所在的上下文
     */
	fcontext_t					 _ctx = NULL;

    /*
     * 协程初始化函数入口函数
     */
    CoroutineFunc                _init_func;

    /*
     * 协程具体执行函数
     */
    std::function<void ()> 		_callback;
};

///////////////////////////////////////////
/**
 * 协程调度类
 */
class TC_CoroutineScheduler
{    
protected:
    static thread_local shared_ptr<TC_CoroutineScheduler> g_scheduler;

public:

    /**
     * 如果没有, 则创建(线程私有变量, 每个线程有一个)
     */ 
    static const shared_ptr<TC_CoroutineScheduler> &create();

    /**
     * 获取scheduler, 没有则返回null, (线程私有变量, 每个线程有一个)
     */ 
    static const shared_ptr<TC_CoroutineScheduler> &scheduler();

    /**
     * 释放协程调度器
     */
    static void reset();

    /**
     * 构造函数(每个线程最多有一个)
     */
    TC_CoroutineScheduler();

    /**
     * 析构函数
     */
    ~TC_CoroutineScheduler();

    /**
     * 初始化协程池的大小、以及协程的堆栈大小
     */
    void setPoolStackSize(uint32_t iPoolSize, size_t iStackSize);

    /**
     * 创建协程
     */
    uint32_t go(const std::function<void ()> &callback);

    /**
     * 通知循环醒过来
     */
    void notify();

    /**
     * 启动协程调度(没有活跃协程会阻塞, 阻塞在epoll上)
     */
    void run();

	/**
	 * 已经在运行中了
	 * @return
	 */
	bool isReady() const { return _ready; }

	/**
	 * 开启epoll模式, 调度器使用run时, 会阻塞在epollwait上, 该epoll对象可以用于网络
	 */
	inline TC_Epoller* getEpoller() { return _epoller; }

	/**
	 * 获取epoller对象
	 * @return
	 */
	TC_Epoller* epoller() { return _epoller; }

    /**
     * 当前协程放弃继续执行
     * @param bFlag: true, 会自动唤醒(等到下次协程调度, 都会再激活当前线程), false: 不再自动唤醒, 除非自己调度该协程(比如put到调度器中)
     */
    void yield(bool bFlag = true);

    /**
     * 当前协程休眠iSleepTime时间(单位:毫秒)，然后会被唤醒继续执行
     */
    void sleep(int millseconds);

    /**
     * 放入需要唤醒的协程, 将协程放入到调度器中, 马上会被调度器调度
     */
    void put(uint32_t iCoroId);

    /**
     * 协程切换
     */
	void switchCoro(TC_CoroutineInfo *to);

    /**
     * 停止
     */
    void terminate();

    /**
     * 资源销毁
     */
    void destroy();

    /**
     * 协程调度是否已经结束
     * @return
     */
    bool isTerminate() const { return _epoller->isTerminate(); }

    /**
     * 协程是否用完了
     * @return
     */
    bool full();

    /**
     * 获取最大的协程数目
     */
    inline uint32_t getPoolSize() { return _poolSize; }

    /**
     * 获取当前已经创建的协程数目
     */
    inline uint32_t getCurrentSize() { return _currentSize; }

    /**
     * 获取请求响应回来的协程数目
     */
    inline size_t getResponseCoroSize() { return _activeCoroQueue.size(); }

    /**
     * 获取理论上空闲的协程数目
     */
    inline uint32_t getFreeSize() { return _poolSize - _usedSize; }

    /**
     * 减少正在使用的协程数目
     */
    inline void decUsedSize() { --_usedSize; }

    /**
     * 增加正在使用的协程数目
     */
    inline void incUsedSize() { ++_usedSize; }

    /**
     * 是否在主协程中
     */
    inline bool isMainCoroutine() { return _currentCoro->getUid() == 0; }
    
    /**
     * 调度器中的主协程
     */
    inline TC_CoroutineInfo& getMainCoroutine() { return _mainCoro; }

    /**
     * 设置主协程
     */
	inline void setMainCtx(fcontext_t ctx) { _mainCoro.setCtx(ctx); }

    /**
     * 当前协程的标识Id
     */
    inline uint32_t getCoroutineId() { return _currentCoro->getUid(); }

    /**
     * 设置当前所有协程执行完毕时的回调
     */
    inline void setNoCoroutineCallback(std::function<void(TC_CoroutineScheduler*)> noCoroutineCallback) { _noCoroutineCallback = noCoroutineCallback; }

    friend class TC_CoroutineInfo;

protected:

	/**
	 * 初始化
	 */
	void init();

	/**
	 * 释放所有协程资源
	 */
	void createCoroutineInfo(size_t poolSize);

    /**
     * 产生协程id
     */
    uint32_t generateId();

    /**
     * 增加协程池的大小
     */
    int increaseCoroPoolSize();

    /**
     * 唤醒需要运行的协程
     */
    void wakeup();

    /**
     * 唤醒自己放弃运行的协程
     */
    void wakeupbyself();

    /**
     * 唤醒休眠的协程
     */
    void wakeupbytimeout();

    /**
     * 放到active的协程链表中
     */
    void moveToActive(TC_CoroutineInfo *coro);

    /**
     * 放到avail的协程链表中
     */
    void moveToAvail(TC_CoroutineInfo *coro);

    /**
     * 放到inactive的协程链表中
     */
    void moveToInactive(TC_CoroutineInfo *coro);

    /**
     * 放到超时等待的协程链表中
     */
    void moveToTimeout(TC_CoroutineInfo *coro);

    /**
     * 放到空闲的协程链表中
     */
    void moveToFreeList(TC_CoroutineInfo *coro);

private:

    /*
     * 协程池的大小
     */
    uint32_t                _poolSize = 1000;

    /*
     * 协程的栈空间大小
     */
    size_t                  _stackSize = 128*1024;

    /*
     * 当前已经创建的协程数
     */
    uint32_t                _currentSize;

    /*
     * 正在使用的协程数
     */
    uint32_t                _usedSize;

    /*
     * 产生协程Id的变量
     */
    uint32_t                _uniqId;

    /*
     * 主协程
     */
    TC_CoroutineInfo        _mainCoro;

    /*
     * 当前运行的协程
     */
    TC_CoroutineInfo*       _currentCoro;

    /*
     * 存放所有协程的数组指针
     */
    TC_CoroutineInfo**      _all_coro = NULL;

    /*
     * 活跃的协程链表
     */
    TC_CoroutineInfo        _active;

    /*
     * 可用的协程链表
     */
    TC_CoroutineInfo        _avail;

    /*
     * 不活跃的协程链表
     */
    TC_CoroutineInfo        _inactive;

    /*
     * 超时的协程链表
     */
    TC_CoroutineInfo        _timeout;

    /*
     * 空闲的协程链表
     */
    TC_CoroutineInfo        _free;

    /*
     * 需要激活的协程队列，其他线程使用，用来激活等待结果的协程
     */
	deque<uint32_t>        _activeCoroQueue;

	/*
	 * 需要激活的协程队列，本线程使用
	 */
    list<uint32_t>        _needActiveCoroId;

    /*
     * 存放超时的协程
     */
    multimap<int64_t, uint32_t> _timeoutCoroId;

    /**
     * epoller
     */
    TC_Epoller*             _epoller = NULL;

    /**
     * 当协程都处理完毕后的回调
     */ 
    std::function<void(TC_CoroutineScheduler*)> _noCoroutineCallback;

    /**
     * 是否正在运行中
     */
    bool                    _ready = false;
};

/**
 * 对线程进行包装的协程类，主要用于在自己起的线程中使用协程,
 * 使用方式:
 * 1 业务可以继承这个类
 * 2 实现handleCoroutine函数(协程具体执行代码), 开发在这里面可以再启动更多的其他协程
 * 3 调用start函数, 启动线程, 同时会创建iNum个协程, 调度器中最多存在iPoolSize个协程同时运行
 * 4 terminate结束
 */
class TC_Coroutine : public TC_Thread
{
public:
    /**
     * 构造函数
     */
    TC_Coroutine();

    /**
     * 析构函数
     */
    virtual ~TC_Coroutine();

    /**
     * 初始化
     * @iNum, 表示同时会启动多少个协程，即会有多少个coroFunc运行的协程
     * @iPoolSize，表示这个线程调度器最多包含的协程个数
     * @iStackSize，协程的栈大小
     */
    void setCoroInfo(uint32_t iNum, uint32_t iPoolSize, size_t iStackSize);

    /**
     * 创建协程，在已经创建的协程中使用
     * 返回值为协程的id，大于0，表示成功，，小于等于0，表示失败
     */
    uint32_t go(const std::function<void ()> &coroFunc);

    /**
     * 当前协程自己放弃执行,会自动被调度器唤醒
     * 在已经创建的协程中使用
     */
    void yield();

    /**
     * 当前协程休眠iSleepTime时间(单位:毫秒)，时间到了，会自动被调度器唤醒
     * 在已经创建的协程中使用
     */
    void sleep(int millseconds);

    /**
     * 获取设置的最大协程的数目
     */
    uint32_t getMaxCoroNum() { return _maxNum; }

    /**
     * 获取启动时，设置的协程的数目
     */
    uint32_t getCoroNum() { return _num; }

    /**
     * 设置协程的栈大小
     */
    size_t getCoroStackSize() { return _stackSize; }

    /**
     * 停止
     */
    void terminate();

protected:
    /**
     * 线程处理方法
     */
    virtual void run();

    /**
     *  静态函数, 协程入口. 
     */
    static void coroEntry(TC_Coroutine *pCoro);

    /**
     * 协程运行的函数，根据_num的数目，会启动_num个这个函数
     */
    virtual void handle() = 0;

protected:
    /**
     * 线程已经启动, 进入协程处理前调用
     */
    virtual void initialize() {}

    /**
     * 所有协程停止运行之后，线程退出之前时调用
     */
    virtual void destroy() {}

    /**
     * 具体的处理逻辑
     */
    virtual void handleCoro();

protected:
    shared_ptr<TC_CoroutineScheduler> _coroSched;
    uint32_t            _num;
    uint32_t            _maxNum;
    size_t              _stackSize;
};

}

#endif
