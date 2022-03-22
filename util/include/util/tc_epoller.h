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

#ifndef	__TC_EPOLLER_H_
#define __TC_EPOLLER_H_

#include "util/tc_platform.h"
#include "util/tc_socket.h"
#include "util/tc_timer.h"
#include "util/tc_logger.h"
#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#if TARGET_PLATFORM_IOS
#include "sys/event.h"

const int EPOLLIN = 0x0001;
const int EPOLLOUT = 0x0004;
const int EPOLLERR = 0x0008;

typedef kevent64_s epoll_event;

#else
#include "sys/epoll.h"
#endif


namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file  tc_epoller.h 
 * @brief  epoll操作封装类 
 * @brief  Epoll operation encapsulation class
 * 使用说明如下:
 * 1 TC_Epoller协程配合, 即tc_coroutine底层其实有一个epoller对象, 协程的切换都是通过epoller来进行的, 之所以这样设计是为了保证协程中网络IO调用的时候能顺利完成协程的切换
 * 2 TC_Epoller继承至TC_TimerBase, 即它本身也是一个定时器, 即可以用做timer
 * 3 TC_Epoller需要调用create来完成初始化
 * 4 TC_Epoller中有一个子类EpollInfo, 该子类非常重要, 当需要用epoll操控某个句柄是, 你可以通过epoll.createEpollInfo创建对象
 * 5 拥有EpollInfo对象后, 可以通过它的registerCallback来注册epoll事件以及回调, 当有对应的事件产生时, 会触发回调
 * 6 registerCallback第二参数表示需要add的事件, 如果业务不需要registerCallback, 可以用epoller的add函数去添加事件
 * 7 EpollInfo有一个cookie方法, 可以在EpollInfo中存入一个指针和析构函数, 会在EpollInfo析构是调用, 一般用来保证cookie设置的对象有机会被释放到
 * 8 EpollInfo它通过epoller.createEpollInfo()创建出来后, 需要持有住, 直到句柄被close后, 调用Epoller::releaseEpollInfo来释放掉
 * 9 TC_Epoller对象的loop方法, 会发起一个epoll wait的事件循环, 会阻塞当前线程
 * 10 TC_Epoller对象的done方法, 会执行一次epoll wait事件, 如果没有任何事件发生, 则只会等待最后ms毫秒(参数确定)
 * 11 TC_Epoller对象中的notify方法, 可以主动唤醒epoll wait
 */
/////////////////////////////////////////////////

/**
 * @brief epoller操作类，已经默认采用了EPOLLET方式做触发
 * @brief epoller operation class, EPOLLET has been used by default for triggering 
 */
class TC_Epoller : public TC_TimerBase
{

public:
    class EpollInfo : public enable_shared_from_this<EpollInfo>
	{
	public:
		/**
		 * 构造函数
		 * @param epoller
		 * @param fd
		 */
		EpollInfo(TC_Epoller* epoller, int fd) : _epoller(epoller), _fd(fd)
		{
		}

		/**
		 * 析构
		 */
		~EpollInfo();

		//注意: 返回false, 表示socket有问题, 框架如果发现是false, 则epoller不再监听socket的事件
		typedef std::function<bool(const shared_ptr<TC_Epoller::EpollInfo> &)> EVENT_CALLBACK;

		/**
		 * 句柄
		 * @return
		 */
		inline int fd() const { return _fd; }

		/**
		 * 是否有效
		 * @return
		 */
		inline bool valid() const { return _fd != INVALID_SOCKET; }

		/**
		 * 设置cookie和析构器, 可以在EpollInfo析构时调用
		 * @param p
		 * @param deconstructor
		 */
		inline void cookie(void *p, function<void(void*)> deconstructor = function<void(void*)>()) 
		{ 
			_cookie = p; 
			_deconstructor = deconstructor;
		}

		/**
		 * 获取cookie
		 * @return
		 */
		inline void *cookie() { return _cookie; }

		/**
		 * 通用callback, 只要任何事件来了, 都会回到一次
		 */
		void setCallback(const std::function<void(const shared_ptr<TC_Epoller::EpollInfo> &)> &callback) { _callback = callback; }

		/**
		 * registry event callback
		 * @param callbacks: <EPOLLIN/EPOLLOUT/EPOLLERR, EVENT_CALLBACK>
		 * @param events: 需要add的事件, 如果为0, 则不add事件
		 */
		void registerCallback(const map<uint32_t, EVENT_CALLBACK> & callbacks, uint32_t events);

		/**
		 * 清除所有callback
		 */
		void clearCallback();

		/**
		 * @brief 添加监听句柄
		 *
		 * @param events
		 */
		void add(uint32_t events);

		/**
		 * @brief 修改句柄事件.
		 *
		 * @param events 需要监听的事件EPOLLIN|EPOLLOUT
		 */
		void mod(uint32_t events);

		/**
		 * @brief 删除句柄事件, 会释放EpollInfo, del后, 不能再使用EpollInfo
		 *
		 * @param events  通常传0
		 */
		void del(uint32_t events);

	protected:

		/**
		 * this to data
		 * @return
		 */
		inline uint64_t data() { return (uint64_t)this; }

		/**
		 * 关联的句柄无效了, 释放掉
		 */
		void release();

		friend class TC_Epoller;

	protected:

	    /**
		 * 触发事件
		 * @param event
		 * @return true: 事件都正常执行完, false: 事件执行有异常(句柄关闭)
		 */
	    bool fireEvent(uint32_t event);

	protected:

		TC_Epoller*				_epoller;

		int						_fd;

		void*					_cookie;

		function<void(void*)>	_deconstructor;

		EVENT_CALLBACK 	        _callbacks[3];

		std::function<void(const shared_ptr<TC_Epoller::EpollInfo> &)> _callback;
	};

    /**
     * @brief 通知epoll从wait中醒过来
     */
    class NotifyInfo
    {
    public:

        /**
         *
         */
        ~NotifyInfo();

        /**
         * 初始化
         */
        void init(TC_Epoller *epoller);

        /**
         * 获取通知fd
         */
        inline int notifyFd() { return _notify.getfd(); }

        /**
         * 获取epoll info, 该指针不需要自己手工delete, epoller loop or NotifyInfo析构中会释放
         * @return
         */
        inline shared_ptr<EpollInfo> &getEpollInfo() { return _epollInfo; }

    protected:
        TC_Epoller* _epoller = NULL;

        //
        shared_ptr<EpollInfo>  _epollInfo;

        //通知fd
        TC_Socket   _notify;
    };
public:

	/**
	 * @brief 构造函数. 
	 * @brief Constructor Function
	 *  
	 */
	TC_Epoller();

	/**
     * @brief 析够函数.
	 * @brief Destructor
	 */
	~TC_Epoller();

	/**
	 * 设置名称
	 * @param name, 给测试用, 知道具体是哪个epoll
	 */
	void setName(const string &name = "") { _name = name; }

	/**
	 * @brief 生成epoll句柄. 
	 * @brief Generate epoll handle.
	 *  
     * @param max_connections epoll服务需要支持的最大连接数(Maximum number of connections the epoll service needs to support)
	 * @param createNotify, if you what call epoller.notify() to wakeup epoll from epoll wait, you should set createNotify to true;
	 */
	void create(int max_connections, bool createNotify = true);

	/**
	 * disable et模式
	 */
	void enableET(bool enable) { _enableET = enable; };

	/**
	 * @brief 释放资源
	 *  
     * @param 
	 */
	void close();

	/**
	 * @brief 添加监听句柄, 这种模式不能指定epoll关联的数据(通过EpollInfo)
	 * 如果需要关联数据, 则需要用create函数
	 *
	 * @param fd
     * @param data   data
	 * @param events
	 */
    void add(SOCKET_TYPE fd, uint64_t data, uint32_t events);

	/**
	 * @brief 修改句柄事件, 这种模式不能指定epoll关联的数据(通过EpollInfo)
	 *
     * @param fd    句柄
     * @param data   data
     * @param events 需要监听的事件EPOLLIN|EPOLLOUT
	 */
	void mod(SOCKET_TYPE fd, uint64_t data, uint32_t events);

    /**
     * @brief 删除句柄事件, 这种模式不能指定epoll关联的数据(通过EpollInfo)
     *
     * @param data   data
     * @param fd    句柄
     */
    void del(SOCKET_TYPE fd, uint64_t data, uint32_t events);

	/**
	 * @brief 添加监听句柄, 会创建EpollInfo, 并将EpollInfo设置为当前fd的data.
	 * EpollInfo被TC_Epoller管理, 连接关闭时自动释放, 使用者需要保持EpollInfo的智能智能指针, 当fd关闭时, 调用releaseEpollInfo
     * @param fd    句柄
     * @return EpollInfo
	 */
	shared_ptr<EpollInfo> createEpollInfo(SOCKET_TYPE fd);

	/**
	 * 释放EpollInfo
	 * @param epollInfo
	 */
	void releaseEpollInfo(const shared_ptr<EpollInfo> &epollInfo);

	/**
	 * @brief 等待时间. 
	 * @brief wait time
	 *  
	 * @param millsecond 毫秒 
     * @return int       有事件触发的句柄数
	 * @return int       Number of handles triggered by events
	 */
	int wait(int millsecond);

	/**
     * @brief 获取被触发的事件.
	 * @brief Get the triggered handle
	 *
	 * @return struct epoll_event&被触发的事件
	 * @return Struct epoll_event& triggered event
	 */
	epoll_event& get(int i);

	/**
	 * 空闲处理
	 * @param callback
	 */
	void idle(std::function<void()> callback) { _idleCallbacks.push_back(callback); }

	/**
	 * 循环(wait心跳间隔, 默认1000ms)
	 */
	void loop(uint64_t ms = 1000);

	/**
	 * 处理一次(wait心跳间隔, 默认1000ms)
	 */
	void done(uint64_t ms = 1000);

	/**
	 * 通知epoll wait醒过来
	 * 注意只有针对create时, 第二个参数传入true的epoll对象才有效!
	 */
	void notify();

	/**
	 * 同步调用, 等待epoll的run线程中同步处理函数以后再回调!超级有用的函数!!!
	 * 注意性能: 中间要分配句柄, 可能性能没那么高!
	 * @param func
	 */
	void syncCallback(const std::function<void()> &func, int64_t millseconds = -1);

	/**
	 * 异步调用, func丢给epoll的run线程中处理函数
	 * 注意性能: 中间要分配句柄, 可能性能没那么高!
	 * @param func
	 */
	void asyncCallback(const std::function<void()>& func);

	/**
	 * 退出循环
	 */
	void terminate();

	/**
	 * 重置状态, 不在是terminate的
	 */
	void reset();

	/**
	 * 是否结束
	 * @return
	 */
	bool isTerminate() const { return _terminate; }

	/**
     * @brief 是否有读事件
	 * @brief whether it have event to read
	 *
	 * @return
	 */		
	static bool readEvent(const epoll_event &ev);

	/**
     * @brief 是否有写事件
	 * @brief whether it have event to write
	 *
	 * @return
	 */		
	static bool writeEvent(const epoll_event &ev);

	/**
     * @brief 是否有异常事件
	 * @brief whether it have eception event
	 *
	 * @return
	 */		
	static bool errorEvent(const epoll_event &ev);

	/**
     * @brief 获取低位/高位数据
	 * @brief Get low/high bit data
	 * @param high: true:高位, false:低位
	 * @param high: true:high level, false:low level
	 * @return
	 */	
	static uint32_t getU32(const epoll_event &ev, bool high);

	/**
     * @brief 获取64bit数据
	 * @brief Get 64 bit data
	 * @return
	 */	
	static uint64_t getU64(const epoll_event &ev);

protected:

	/**
	 * @brief 控制epoll，将EPOLL设为边缘触发EPOLLET模式 
	 * @brief Control epoll, set EPOLL to Edge Trigger EPOLLET mode
     * @param fd    句柄，在create函数时被赋值
	 * @param fd    Handle, assigned when creating function
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
	 * @param data  auxiliary data that can be obtained in epoll_event subsequently
     * @param event 需要监听的事件
	 * @param event the event to be listened
	 * @param op    EPOLL_CTL_ADD： 注册新的fd到epfd中； 
	 * 			    EPOLL_CTL_MOD：修改已经注册的fd的监听事件； 
	 * 			    EPOLL_CTL_DEL：从epfd中删除一个fd； 
	 * @param op    EPOLL_CTL_ADD：Register new FD into EPFD
	 * 				EPOLL_CTL_MOD：Modify the monitoring events for registered fd
	 * 				EPOLL_CTL_DEL：Delete an FD from epfd
	 *  
	 */
	int ctrl(SOCKET_TYPE fd, uint64_t data, uint32_t events, int op);

 	/**
 	 * 执行定时时间
 	 * @param func
 	 */
	virtual void onFireEvent(std::function<void()> func);

	/**
	 * 增加了一个最近的定时器, 需要触发wait唤醒, 等到到最新的时间上
	 */
	virtual void onAddTimer();

	friend class EpollInfo;

protected:
	/**
	 * epoll名称, 主要给debug使用
	 */
	string  _name;
	/**
	 * 是否退出循环
	 */
	bool    _terminate = false;

	/**
	 * 默认开启ET模式
	 */
	bool    _enableET = true;

	/**
	 * 通知循环退出
	 */
	NotifyInfo*  _notify = NULL;

    /**
     * 	epoll
     */
#if TARGET_PLATFORM_WINDOWS
	void* _iEpollfd;
#else
    int _iEpollfd;
#endif

	/**
     * 最大连接数
	 * The max amount of connections
	 */
	int	_max_connections;

	/**
     * 事件集
	 * Event Set
	 */
	epoll_event *_pevs;

	/**
	 * 空闲处理
	 */
	vector<std::function<void()>> _idleCallbacks;
};

}
#endif

