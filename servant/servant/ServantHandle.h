﻿/**
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

#ifndef __TARS_SERVANT_HANDLE_H
#define __TARS_SERVANT_HANDLE_H

#include <map>
#include <string>
#include <memory>
#include <deque>
#include "util/tc_platform.h"
#include "util/tc_monitor.h"
#include "util/tc_epoll_server.h"
#include "servant/Servant.h"
#include "servant/StatReport.h"

// #ifdef TARS_OPENTRACKING
// #include "opentracing/span.h"
// #endif

namespace tars
{
class Application;

//////////////////////////////////////////////////////////////////////////////
/**
 * 处理网络请求线程
 */
class ServantHandle : public TC_EpollServer::Handle
{
public:
    /**
     * 定义常量
     */
    enum
    {
        HEART_BEAT_INTERVAL = 10, /**心跳间隔时间**/
    };

    /**
     * 构造
     */
    ServantHandle(Application *application);

    /**
     * 析够
     */
    ~ServantHandle();

    /**
     * get Application
     * @return
     */
	Application *getApplication() { return _application; }

    /**
     * 获取实际的servant
     * @return
     */
    ServantPtr getServant() { return _servant; }
protected:
    /**
     * 线程初始化
     */
    void initialize();

    /**
     * 逻辑处理
     * @param stRecvData
     */
    virtual void handle(const shared_ptr<TC_EpollServer::RecvContext> &data);

    /**
     * 超时处理
     * @param stRecvData
     */
    virtual void handleTimeout(const shared_ptr<TC_EpollServer::RecvContext> &data);

    /**
     * overload 处理
     * @param stRecvData
     */
    virtual void handleOverload(const shared_ptr<TC_EpollServer::RecvContext> &data);

    /**
     * close 事件处理
     * @param stRecvData
     */
    virtual void handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data);

	/**
	 * handleFilter拆分的第一部分，处理异步调用队列
	 */
	virtual void handleAsyncResponse();

    /**
     * handleFilter拆分的第二部分，处理用户自有数据
     * 非游戏逻辑可忽略bExpectIdle参数
     */
    virtual void handleCustomMessage(bool bExpectIdle = false);

    /**
     * 心跳
     */
    virtual void heartbeat();

    /**
     * 检查servant有没有resp消息待处理
     * @return bool
     */
    virtual bool allFilterIsEmpty();

    /**
     * 创建上下文
     * @param stRecvData
     * @return Current*
     */
	CurrentPtr createCurrent(const shared_ptr<TC_EpollServer::RecvContext> &data);

	/**
     * 创建闭连接时的关上下文
     * @param stRecvData
     * @return JceCurrent*
     */
    CurrentPtr createCloseCurrent(const shared_ptr<TC_EpollServer::RecvContext> &data);

    /**
     * 处理Tars协议
     *
     * @param current
     */
    void handleTarsProtocol(const CurrentPtr &current);

    /**
     * 处理非Tars协议
     *
     * @param current
     */
    void handleNoTarsProtocol(const CurrentPtr &current);


// #ifdef TARS_OPENTRACKING
//     /**
//      * 处理TARS下的调用链逻辑
//      *
//      * @param current
//      */
//     void processTracking(const CurrentPtr &current);


//     void finishTracking(int ret, const CurrentPtr &current);
// #endif
    /**
     * 处理TARS下的染色逻辑
     *
     * @param current
     */
    bool processDye(const CurrentPtr &current, string& dyeingKey);

    /**
     * 处理TARS下的调用链追踪
     *
     * @param current
     */
    bool processTrace(const CurrentPtr &current);

    /**
     * 处理cookie
     */
    bool processCookie(const CurrentPtr &current, map<string, string> &cookie);

    /**
     * 检查set调用合法性
     *
     * @param current
     * @return bool 如果调用合法返回true，如果调用非法则返回false
     */
	bool checkValidSetInvoke(const CurrentPtr &current);

    /**
     * 上报监控统计
     * @param data
     * @param current
     */
    void reportReqTime(const shared_ptr<TC_EpollServer::RecvContext> &data, const CurrentPtr &current);
protected:
	/**
	 * application
	 */
	Application *_application = NULL;

    /**
     * 处理对象
     */
    ServantPtr _servant;


// #ifdef TARS_OPENTRACKING
//     map<int,std::unique_ptr<opentracing::Span>> _spanMap;
// #endif
};

typedef TC_AutoPtr<ServantHandle> ServantHandlePtr;
///////////////////////////////////////////////////////////
}
#endif

