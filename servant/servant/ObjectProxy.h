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

#ifndef __TARS_OBJECT_PROXY_H_
#define __TARS_OBJECT_PROXY_H_

#include "servant/Communicator.h"
#include "servant/Message.h"
#include "servant/AdapterProxy.h"
#include "servant/EndpointInfo.h"
#include "servant/EndpointF.h"
#include "servant/AppProtocol.h"
#include "servant/StatReport.h"
#include "servant/Global.h"
#include "util/tc_timeout_queue_noid.h"

namespace tars
{
class EndpointManager;

///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
/**
 * 每个objectname在每个客户端网络线程中有唯一一个objectproxy
 * 
 */
class ObjectProxy : public TC_HandleBase, public TC_ThreadMutex
{
public:
    /**
     * 构造函数
     * @param pCommunicatorEpoll
     * @param sObjectProxyName
     * @param setName 指定set调用的setid
     */
    ObjectProxy(CommunicatorEpoll * pCommunicatorEpoll, ServantProxy *servantProxy, const string & sObjectProxyName, const string& setName="");

    /**
     * 析构函数
     */
    ~ObjectProxy();

    /**
     * initialize
     */
    void initialize(bool rootServant);

    /**
     * 加载locator
     */
    int loadLocator();
	
    /**
     * 方法调用
     * @param req
     * @return int
     */
    void invoke(ReqMessage *  msg);

    /**
     * 发送积攒在obj的数据
     */
    void doInvoke();

	/**
     *
     * @param active
     * @param inactive
     */
	void onNotifyEndpoints(const set<EndpointInfo> & active,const set<EndpointInfo> & inactive);


    /**
     * connected
     */
	void onConnect(AdapterProxy *adapterProxy);

	/**
     * 获取所有的adapter
     */
    void mergeStat(map<StatMicMsgHead, StatMicMsgBody> & mStatMicMsg);

    /**
     * 检查Obj的队列超时
     */
    void doTimeout();

    /**
     * 发起tars_ping请求
     */
    void doKeepAlive();

    /**
     * 获取CommunicatorEpoll*
     */
    inline CommunicatorEpoll *getCommunicatorEpoll() { return _communicatorEpoll; }

    /**
     * 获取object名称
     * @return const string&
     */
    inline const string & name() const { return _name; }

	/**
	 * address
	 * @return
	 */
	inline const string &hash() const { return _hash; }

    /**
     * address
     * @return
     */
    inline const string &address() const { return _address; }

    /**
     * reconnect
     * @param second
     */
    inline void reconnect(int second) { _reConnectSecond = second; }

	/**
	 * reconnect
	 * @param second
	 */
	inline int reconnect() { return _reConnectSecond; }

	/**
     * 判断此obj是否走按set规则调用流程，如果是直连方式，即使服务端是启用set的，也不认为是按set规则调用的
     */
    inline bool isInvokeBySet() const { return _isInvokeBySet; }

    /**
     * 获取按set规则调用的set名称
     */
    inline const string& getInvokeSetName() const { return _invokeSetId; }


    /**
     * 获取servantproxy
     */
    inline ServantProxy * getServantProxy() { return _servantProxy; }

	/**
	 * 获取servantproxy
	 */
	inline ServantProxy * getRootServantProxy()
	{
		if(_servantProxy->_rootPrx)
		{
			return _servantProxy->_rootPrx.get();
		}
		return _servantProxy;
	}

    /**
     *
     * @return
     */
	inline const shared_ptr<EndpointManager> &getEndpointManager()
	{
    	return _endpointManger;
	}

    /**
     * get all adapter proxy
     * @return
     */
	const vector<AdapterProxy*> & getAdapters();

	/**
	 *
	 * @param ep
	 */
	void onSetInactive(const EndpointInfo& ep);

protected:

	/**
	 * 处理请求异常
	 *
	 */
	void doInvokeException(ReqMessage * msg);

    void prepareConnection(AdapterProxy *adapterProxy);

    friend class AdapterProxy;
private:
    /*
     * 客户端网络线程的指针
     */
	CommunicatorEpoll 		 			*_communicatorEpoll;

    /*
     * [obname]#hash@tcp -h xxxx -p xxx
     */
    string                                _name;

    /**
     * obname#[hash]@tcp -h xxxx -p xxx
     * ever hash has one connection
     */
    string                                _hash;

	/**
	 * obname#hash@[tcp -h xxxx -p xxx]
	 * ever hash has one connection
	 */
    string                                _address;

    /**
     * obj full name
     */
    string                                _sObjectProxyName;

    /*
     * 按set规则调用的set名称
     */
    string                                _invokeSetId;

    /*
     * 是否是按set规则调用
     */
    bool                                  _isInvokeBySet;


    /**
     * reconnect, 0: not reconnect
     */
    int                                   _reConnectSecond = 0;


    /*
     * 结点路由管理类
     */
    std::shared_ptr<EndpointManager>      _endpointManger;

    /*
     * 超时队列(连接建立前)
     */
    TC_TimeoutQueueNoID<ReqMessage *>     _reqTimeoutQueue;
    
    /*
     * ServantProxy
     */
    ServantProxy *                        _servantProxy = NULL;
};
///////////////////////////////////////////////////////////////////////////////////
}
#endif
