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

#ifndef __TC_EPOLLER_H_
#define __TC_EPOLLER_H_

#include "util/tc_platform.h"
#include "util/tc_socket.h"
#include <cassert>

#if TARGET_PLATFORM_IOS
#include "sys/event.h"

const int EPOLLIN = 0x0001;
const int EPOLLOUT = 0x0002;
const int EPOLLERR = 0x0004;

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
 */
/////////////////////////////////////////////////

/**
* @brief epoll异常类
* @brief epoll exception class
*/
struct TC_Epoller_Exception : public TC_Exception
{
   TC_Epoller_Exception(const string &buffer, int err) : TC_Exception(buffer, err) {};
   ~TC_Epoller_Exception() {};
};

/**
 * @brief epoller操作类，已经默认采用了EPOLLET方式做触发 
 * @brief epoller operation class, EPOLLET has been used by default for triggering 
 */
class TC_Epoller
{
public:
	/**
	 * @brief 通知epoll从wait中醒过来 
	 * @brief Notice epoll to wake up from 'wait'
	 */
    class NotifyInfo
    {
	public:
        NotifyInfo();
		~NotifyInfo();

		/**
		 * 初始化
		 * Initialization
		 */	
        void init(TC_Epoller *ep);

		/**
		 * 添加关联数据
		 * Add corresponding data
		 */ 
		void add(uint64_t data);

		/**
		 * 通知notify醒过来
		 * Notice notify to wake up
		 */ 
        void notify();

		/**
		 * 释放掉
		 * Diposit
		 */ 
        void release();

		/**
		 * 获取通知fd
		 * Get notifyFd
		 */ 
		int notifyFd();


	protected:
        //通知fd
        TC_Socket _notify;
		TC_Epoller *_ep;

		/*Events associated with the notification handle*/
		uint64_t _data;		//关联到通知句柄的事件

    };

	/**
	 * @brief 构造函数. 
	 * @brief Constructor Function
	 *  
     * @param bEt 默认是ET模式，当状态发生变化的时候才获得通知
	 * @param bEt The default is et mode, which is notified when the status changes
	 */
	TC_Epoller();

	/**
     * @brief 析够函数.
	 * @brief Destructor
	 */
	~TC_Epoller();

	/**
	 * @brief 生成epoll句柄. 
	 * @brief Generate epoll handle.
	 *  
     * @param max_connections epoll服务需要支持的最大连接数
	 * @param max_connections Maximum number of connections the epoll service needs to support
	 */
	void create(int max_connections);

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
	 * @brief 添加监听句柄. 
	 * @brief Add listening handle.
	 *  
     * @param fd    句柄
	 * @param fd    handle
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
	 * @param data  auxiliary data that can be obtained in epoll_event subsequently
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
	 * @param event Events to be listened on EPOLLIN|EPOLLOUT
     *              
	 */
	int add(SOCKET_TYPE fd, uint64_t data, int32_t event);

	/**
	 * @brief 修改句柄事件. 
	 * @brief Modify handle event
	 *  
     * @param fd    句柄
	 * @param fd    handle
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
	 * @param data  auxiliary data that can be obtained in epoll_event subsequently
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
	 * @param event Events to be listened on EPOLLIN|EPOLLOUT
	 */
	int mod(SOCKET_TYPE fd, uint64_t data, int32_t event);

	/**
	 * @brief 删除句柄事件. 
	 * @brief Delete handle event.
	 *  
     * @param fd    句柄
	 * @param fd    handle
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
	 * @param data  auxiliary data that can be obtained in epoll_event subsequently
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
	 * @param event Events to be listened on EPOLLIN|EPOLLOUT
	 */
	int del(SOCKET_TYPE fd, uint64_t data, int32_t event);

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
	epoll_event& get(int i);// { assert(_pevs != 0); return _pevs[i]; }

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

protected:
	/**
	 * 默认开启ET模式
	 */
	bool    _enableET = true;

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
};

}
#endif

