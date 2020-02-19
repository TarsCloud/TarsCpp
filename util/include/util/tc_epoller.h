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
 */
/////////////////////////////////////////////////

/**
* @brief epoll异常类
*/
struct TC_Epoller_Exception : public TC_Exception
{
   TC_Epoller_Exception(const string &buffer, int err) : TC_Exception(buffer, err) {};
   ~TC_Epoller_Exception() {};
};

/**
 * @brief epoller操作类，已经默认采用了EPOLLET方式做触发 
 */
class TC_Epoller
{
public:
	/**
	 * @brief 通知epoll从wait中醒过来 
	 */
    class NotifyInfo
    {
	public:
        NotifyInfo();
		~NotifyInfo();

		/**
		 * 初始化
		 */	
        void init(TC_Epoller *ep);

		/**
		 * 添加关联数据
		 */ 
		void add(uint64_t data);

		/**
		 * 通知notify醒过来
		 */ 
        void notify();

		/**
		 * 释放掉
		 */ 
        void release();

		/**
		 * 获取通知fd
		 */ 
		int notifyFd();

		/**
		 * 清除通知, 否则会一直响应
		 */ 
		// void clear();
	protected:
        //通知fd
        TC_Socket _notify;
		TC_Socket _notifyClient;
		// bool      _clear = true;
		TC_Epoller *_ep;

		uint64_t _data;		//关联到通知句柄的事件

    };

	/**
	 * @brief 构造函数. 
	 *  
     * @param bEt 默认是ET模式，当状态发生变化的时候才获得通知
	 */
	TC_Epoller();

	/**
     * @brief 析够函数.
	 */
	~TC_Epoller();

	/**
	 * @brief 生成epoll句柄. 
	 *  
     * @param max_connections epoll服务需要支持的最大连接数
	 */
	void create(int max_connections);

	/**
	 * @brief 释放资源
	 *  
     * @param 
	 */
	void close();

	/**
	 * @brief 添加监听句柄. 
	 *  
     * @param fd    句柄
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
     *              
	 */
	void add(SOCKET_TYPE fd, uint64_t data, int32_t event);

	/**
	 * @brief 修改句柄事件. 
	 *  
     * @param fd    句柄
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
	 */
	void mod(SOCKET_TYPE fd, uint64_t data, int32_t event);

	/**
	 * @brief 删除句柄事件. 
	 *  
     * @param fd    句柄
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
	 */
	void del(SOCKET_TYPE fd, uint64_t data, int32_t event);

	/**
	 * @brief 等待时间. 
	 *  
	 * @param millsecond 毫秒 
     * @return int       有事件触发的句柄数
	 */
	int wait(int millsecond);

	/**
     * @brief 获取被触发的事件.
	 *
	 * @return struct epoll_event&被触发的事件
	 */
	epoll_event& get(int i);// { assert(_pevs != 0); return _pevs[i]; }

	/**
     * @brief 是否有读事件
	 *
	 * @return
	 */		
	static bool readEvent(const epoll_event &ev);

	/**
     * @brief 是否有写事件
	 *
	 * @return
	 */		
	static bool writeEvent(const epoll_event &ev);

	/**
     * @brief 是否有异常事件
	 *
	 * @return
	 */		
	static bool errorEvent(const epoll_event &ev);

	/**
     * @brief 获取低位/高位数据
	 * @param high: true:高位, false:低位
	 * @return
	 */	
	static uint32_t getU32(const epoll_event &ev, bool high);

	/**
     * @brief 获取64bit数据
	 * @return
	 */	
	static uint64_t getU64(const epoll_event &ev);

protected:

	/**
	 * @brief 控制epoll，将EPOLL设为边缘触发EPOLLET模式 
     * @param fd    句柄，在create函数时被赋值
     * @param data  辅助的数据, 可以后续在epoll_event中获取到
     * @param event 需要监听的事件
	 * @param op    EPOLL_CTL_ADD： 注册新的fd到epfd中； 
	 * 			    EPOLL_CTL_MOD：修改已经注册的fd的监听事件； 
	 * 			    EPOLL_CTL_DEL：从epfd中删除一个fd； 
	 *  
	 */
 	void ctrl(SOCKET_TYPE fd, uint64_t data, uint32_t events, int op);

protected:

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
	 */
	int	_max_connections;

	/**
     * 事件集
	 */
	epoll_event *_pevs;
};

}
#endif

