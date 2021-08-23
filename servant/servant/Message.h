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

#ifndef __TARS_MESSAGE_H_
#define __TARS_MESSAGE_H_

#include <util/tc_network_buffer.h>
#include "util/tc_autoptr.h"
#include "util/tc_monitor.h"
#include "util/tc_loop_queue.h"
#include "servant/Global.h"

namespace tars
{

/**

 * 超时一定比率后进行切换
 * 设置超时检查参数
 * 计算到某台服务器的超时率, 如果连续超时次数或者超时比例超过阀值
 * 默认60s内, 超时调用次数>=2, 超时比率0.5或者连续超时次数>5,
 * 则失效
 * 服务失效后, 请求将尽可能的切换到其他可能的服务器, 并每隔tryTimeInterval尝试一次, 如果成功则认为恢复
 * 如果其他服务器都失效, 则随机选择一台尝试
 * 如果是灰度状态的服务, 服务失效后如果请求切换, 也只会转发到相同灰度状态的服务
 * @uint16_t minTimeoutInvoke, 计算的最小的超时次数, 默认2次(在checkTimeoutInterval时间内超过了minTimeoutInvoke, 才计算超时)
 * @uint32_t frequenceFailInvoke, 连续失败次数
 * @uint32_t checkTimeoutInterval, 统计时间间隔, (默认60s, 不能小于30s)
 * @float radio, 超时比例 > 该值则认为超时了 ( 0.1<=radio<=1.0 )
 * @uint32_t tryTimeInterval, 重试时间间隔
 */
struct CheckTimeoutInfo
{
    /*
     * 构造函数
     */
    CheckTimeoutInfo() 
    : minTimeoutInvoke(2)
    , checkTimeoutInterval(60)
    , frequenceFailInvoke(2)
    , minFrequenceFailTime(5)
    , radio(0.5)
    , tryTimeInterval(10)
    , maxConnectExc(1)
    {
    }

    /*
     * 判断超时屏蔽时，计算的最小的超时次数
     */
    uint16_t minTimeoutInvoke;

    /*
     * 判断超时屏蔽时，时间间隔
     */
    uint32_t checkTimeoutInterval;       

    /*
     * 判断因连续失败而进行屏蔽时，连续失败的次数
     */
    uint32_t frequenceFailInvoke;

    /*
     * 判断因连续失败而进行屏蔽时，最小的时间间隔
     */
    uint32_t minFrequenceFailTime;

    /*
     * 超时的比率
     */
    float    radio;

    /*
     * 重试间隔
     */
    uint32_t tryTimeInterval;

    /*
     * 最大连接异常次数
     */
    uint32_t maxConnectExc;
};


#define IS_MSG_TYPE(m, t) ((m & t) != 0)
#define SET_MSG_TYPE(m, t) do { (m |= t); } while (0);
#define CLR_MSG_TYPE(m, t) do { (m &=~t); } while (0);

struct ThreadPrivateData
{
    /**
     * hash属性,客户端每次调用都进行设置
     */
    bool           _hash        = false;                            //是否普通取模hash
    bool           _conHash     = false;                          //是否一致性hash
    int64_t        _hashCode    = -1;                        //hash值

    /**
     * 染色信息
     */
    bool           _dyeing      = false;                          //标识当前线程是否需要染色
    string         _dyeingKey;                        //染色的key值

    /**
     * 允许客户端设置接口级别的超时时间,包括同步和异步、单向
     */
    int            _timeout     = 0;                         //超时时间

    /**
     * 保存调用后端服务的地址信息
     */
    string         _szHost;                       //调用对端地址

    /**
     * cookie
     */
    map<string, string>  _cookie;          // cookie内容

};

struct ReqMonitor;

struct ReqMessage : public TC_HandleBase
{
    //调用类型
    enum CallType
    {
        SYNC_CALL = 1, //同步
        ASYNC_CALL,    //异步
        ONE_WAY,       //单向
//        THREAD_EXIT    //线程退出的标识
    };

    //请求状态
    enum ReqStatus
    {
        REQ_REQ = 0,    //状态正常,正在请求中
        REQ_RSP,        //请求已经发出去
        REQ_TIME,       //请求超时
        REQ_EXC         //客户端请求异常

    };


	/*
	 * 构造函数
	 */
	ReqMessage()
		: eStatus(ReqMessage::REQ_REQ)
		, eType(SYNC_CALL)
		, bFromRpc(false)
		, callback(NULL)
		, proxy(NULL)
		, pObjectProxy(NULL)
		, adapter(NULL)
		, pMonitor(NULL)
		, iBeginTime(TNOWMS)
		, iEndTime(0)
		, bPush(false)
		, sched(NULL)
		, iCoroId(0)
	{
	}

    /*
     * 析构函数
     */
    ~ReqMessage();

    /*
     * 初始化
     */
    void init(CallType eCallType, ServantProxy *proxy);

    ReqStatus                   eStatus;        //调用的状态
    CallType                    eType;          //调用类型
    bool                        bFromRpc        = false;       //是否是第三方协议的rcp_call，缺省为false
    RequestPacket               request;        //请求消息体
    shared_ptr<ResponsePacket>  response;       //响应消息体
	shared_ptr<TC_NetWorkBuffer::Buffer> sReqData;       //请求消息体
    ServantProxyCallbackPtr     callback;       //异步调用时的回调对象
    ServantProxy                *proxy          = NULL;
    ObjectProxy                 *pObjectProxy   = NULL;  //调用端的proxy对象
    AdapterProxy                *adapter        = NULL;       //调用的adapter

	ReqMonitor                  *pMonitor       = NULL;      //用于同步的monitor

    int64_t                     iBeginTime      = 0;     //请求时间
    int64_t                     iEndTime        = 0;       //完成时间

    bool                        bPush           = false;          //push back 消息

    bool                        bTraceCall;     // 是否需要调用链追踪
    string                      sTraceKey;      // 调用链key

    shared_ptr<TC_CoroutineScheduler>      sched;
    int                         iCoroId         = 0;

    std::function<void()>       deconstructor;  //析构时调用

    ThreadPrivateData           data;     //线程数据
};

/**
 * 用于同步调用时的条件变量
 */
struct ReqMonitor
{
	ReqMessage 				*msg = NULL;
	std::mutex 				mutex;
	std::condition_variable cond;
	bool               		bMonitorFin     = false;    //同步请求timewait是否结束

	ReqMonitor(ReqMessage *m) : msg(m) {}

	void wait();
	void notify();
};

typedef TC_AutoPtr<ReqMessage>  ReqMessagePtr;
typedef TC_LoopQueue<ReqMessage *>  ReqInfoQueue;

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
