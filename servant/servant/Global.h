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

#ifndef __TARS_GLOBAL_H_
#define __TARS_GLOBAL_H_

#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <deque>
#include <vector>
#include <memory>
#include "util/tc_platform.h"
#include "util/tc_clientsocket.h"
#include "util/tc_autoptr.h"
#include "util/tc_common.h"
#include "util/tc_logger.h"
#include "util/tc_thread_mutex.h"
#include "util/tc_coroutine.h"
#include "tup/RequestF.h"
#include "servant/BaseF.h"

using namespace std;

namespace tars
{
//////////////////////////////////////////////////////////////

const size_t MAX_CLIENT_THREAD_NUM          = 2048; //客户端最大网络线程数(线程模型: 网络线程数, 协程模型: 业务线程+网络线程)
const size_t MAX_CLIENT_ASYNCTHREAD_NUM     = 1024; //客户端每个网络线程拥有的最大异步线程数
const size_t MAX_CLIENT_NOTIFYEVENT_NUM     = 2048; //客户端每个网络线程拥有的最大通知事件的数目

//////////////////////////////////////////////////////////////
class Communicator;
class AdapterProxy;
class ServantProxy;
class ServantProxyCallback;
class ObjectProxy;
class Current;
class FDReactor;
class TC_Transceiver;
class StatFProxy;
class StatReport;
class ServantProxyFactory;
class AsyncProcThread;
class LocalRollLogger;
class RemoteConfig;
class RemoteTimeLogger;
class RemoteNotify;

typedef TC_AutoPtr<Communicator> CommunicatorPtr;
typedef TC_AutoPtr<ServantProxy> ServantPrx;
typedef TC_AutoPtr<ServantProxyCallback> ServantProxyCallbackPtr;
typedef TC_AutoPtr<ObjectProxy> ObjectPrx;
typedef TC_AutoPtr<Current> CurrentPtr;
typedef TC_AutoPtr<StatFProxy> StatFPrx;
typedef TC_AutoPtr<StatReport> StatReportPtr;
typedef TC_AutoPtr<FDReactor> FDReactorPtr;
typedef TC_AutoPtr<AsyncProcThread> AsyncProcThreadPtr;

typedef CurrentPtr TarsCurrentPtr;
typedef RemoteConfig TarsRemoteConfig;
typedef RemoteNotify TarsRemoteNotify;
typedef LocalRollLogger TarsRollLogger;
typedef RemoteTimeLogger TarsTimeLogger;

//////////////////////////////////////////////////////////////
/**
 * 定义TARS网络调用的异常基类
 */
struct TarsException : public TC_Exception
{
    TarsException(const string &buffer) : TC_Exception(buffer){};
    TarsException(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TarsException() throw(){};
    /**
     * 根据返回值抛出异常
     * @param ret
     * @param desc
     */
    static void throwException(int ret, const string& desc = "");
};

////////////////////////////////////////////////////////////////
// 定义网络异常


/**
 * Server编码异常
 */
struct TarsServerEncodeException : public TarsException
{
    TarsServerEncodeException(const string &buffer) : TarsException(buffer){};
    ~TarsServerEncodeException() throw(){};
};
/**
 * Server解码异常
 */
struct TarsServerDecodeException : public TarsException
{
    TarsServerDecodeException(const string &buffer) : TarsException(buffer){};
    ~TarsServerDecodeException() throw(){};
};

/**
 * Server无函数异常
 */
struct TarsServerNoFuncException : public TarsException
{
    TarsServerNoFuncException(const string &buffer) : TarsException(buffer){};
    ~TarsServerNoFuncException() throw(){};
};

/**
 * Server无对象异常
 */
struct TarsServerNoServantException : public TarsException
{
    TarsServerNoServantException(const string &buffer) : TarsException(buffer){};
    ~TarsServerNoServantException() throw(){};
};
/**
 * 消息在服务端队列超时
 */
struct TarsServerQueueTimeoutException : public TarsException
{
    TarsServerQueueTimeoutException(const string &buffer) : TarsException(buffer){};
    ~TarsServerQueueTimeoutException() throw(){};
};
/**
 * 连接异常
 */
struct TarsServerConnectionException : public TarsException
{
	TarsServerConnectionException(const string &buffer) : TarsException(buffer){};
	~TarsServerConnectionException() throw(){};
};
/**
 * 调用超时(连接都没有成功建立)
 */
struct TarsServerInvokeTimeoutException : public TarsException
{
	TarsServerInvokeTimeoutException(const string &buffer) : TarsException(buffer){};
	~TarsServerInvokeTimeoutException() throw(){};
};
/**
 * 服务端返回的未知值
 */
struct TarsServerUnknownException : public TarsException
{
    TarsServerUnknownException(const string &buffer) : TarsException(buffer){};
    ~TarsServerUnknownException() throw(){};
};


/**
 * 同步调用超时异常
 */
struct TarsSyncCallTimeoutException  : public TarsException
{
    TarsSyncCallTimeoutException (const string &buffer) : TarsException(buffer){};
    ~TarsSyncCallTimeoutException () throw(){};
};


/**
 * 访问 Registry 错误
 */
struct TarsRegistryException : public TarsException
{
    TarsRegistryException(const string &buffer) : TarsException(buffer){};
    ~TarsRegistryException() throw(){};
};


/**
 * 客户端队列满了
 */
struct TarsClientQueueException : public TarsException
{
    TarsClientQueueException(const string &buffer) : TarsException(buffer){};
    ~TarsClientQueueException() throw(){};
};

/**
 * 业务线程调用协程并行请求接口时，抛出的异常
 */
struct TarsUseCoroException : public TarsException
{
    TarsUseCoroException(const string &buffer) : TarsException(buffer){};
    ~TarsUseCoroException() throw(){};
};

/**
 * 通信器析构了
 */
struct TarsCommunicatorException : public TarsException
{
	TarsCommunicatorException(const string &buffer) : TarsException(buffer){};
	~TarsCommunicatorException() throw(){};
};
///////////////////////////////////////////////////////////////////
}
#endif
