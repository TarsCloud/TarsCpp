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

#include "servant/ObjectProxy.h"
#include "servant/Communicator.h"
#include "servant/CommunicatorEpoll.h"
#include "servant/Global.h"
#include "servant/EndpointManager.h"
#include "servant/AppCache.h"
#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "servant/RemoteLogger.h"

namespace tars
{
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
ObjectProxy::ObjectProxy(CommunicatorEpoll *pCommunicatorEpoll, ServantProxy *servantProxy, const string & sObjectProxyName,const string& setName)
: _communicatorEpoll(pCommunicatorEpoll)
, _sObjectProxyName(sObjectProxyName)
, _invokeSetId(setName)
, _isInvokeBySet(false)
, _servantProxy(servantProxy)
{
    string::size_type pos = sObjectProxyName.find_first_of('@');

    if(pos != string::npos)
    {
        _name       = sObjectProxyName.substr(0,pos);
	    _address    = sObjectProxyName.substr(pos+1);
    }
    else
    {
        _name = sObjectProxyName;
        //启用set或者指定set调用
        if(ClientConfig::SetOpen || !_invokeSetId.empty())
        {
            //指定set调用时，指定set的优先级最高
            _invokeSetId  = _invokeSetId.empty()?ClientConfig::SetDivision:_invokeSetId;
            _isInvokeBySet = true;
        }
    }

    //#, hash模式, 创建多个连接的情况(http)
	pos = _name.find_first_of('#');

	if(pos != string::npos)
	{
		_hash = _name.substr(pos+1);
		_name = _name.substr(0,pos);
	}

}

ObjectProxy::~ObjectProxy()
{

}

void ObjectProxy::initialize(bool rootServant)
{
    _endpointManger.reset(new EndpointManager(this, _communicatorEpoll->getCommunicator(), _communicatorEpoll->isFirstNetThread()));

    _endpointManger->init(_sObjectProxyName, _invokeSetId, rootServant);

	_hasInitialize = true;
}

const vector<AdapterProxy*> & ObjectProxy::getAdapters()
{
	return _endpointManger->getAdapters();
}

int ObjectProxy::loadLocator()
{
    if(_endpointManger->getDirectProxy())
    {
        //直接连接
        return 0;
    }

    string locator = _communicatorEpoll->getCommunicator()->getProperty("locator");

    if (locator.find_first_not_of('@') == string::npos)
    {
        TLOGERROR("[Locator is not valid:" << locator << "]" << endl);

        return -1;
    }

    QueryFPrx prx = _communicatorEpoll->getCommunicator()->stringToProxy<QueryFPrx>(locator);

    _endpointManger->setLocatorPrx(prx);

    return 0;
}

void ObjectProxy::invoke(ReqMessage * msg)
{
    TLOGTARS("[ObjectProxy::invoke, objname:" << _name << ", begin...]" << endl);

    //选择一个远程服务的Adapter来调用
    AdapterProxy * pAdapterProxy = NULL;
    bool bFirst = _endpointManger->selectAdapterProxy(msg, pAdapterProxy);

    if(bFirst)
    {
        //判断是否请求过主控
        bool bRet = _reqTimeoutQueue.push(msg,msg->request.iTimeout+msg->iBeginTime);

        assert(bRet);

        //把数据缓存在obj里面
        TLOGTARS("[ObjectProxy::invoke, objname:" << _name << ", select adapter proxy not valid (have not invoke reg)]" << endl);

        return;
    }

    if(!pAdapterProxy)
    {
        TLOGERROR("[ObjectProxy::invoke, objname:"<< _name << ", selectAdapterProxy is null]"<<endl);

        msg->response->iRet = TARSADAPTERNULL;

        doInvokeException(msg);
        return ;
    }

	msg->adapter = pAdapterProxy;

    //连接还没有建立, 暂时先放队列里面
	if(!msg->adapter->trans()->hasConnected())
    {

		bool bRet = _reqTimeoutQueue.push(msg, this->_servantProxy->tars_connect_timeout() + msg->iBeginTime);

		assert(bRet);

        //把数据缓存在obj里面
        TLOGTARS("[ObjectProxy::invoke, " << _name << ", select adapter proxy not connected (have not invoke reg)]" << endl);
        return;
    }

    pAdapterProxy->invoke(msg);
}

void ObjectProxy::prepareConnection(AdapterProxy *adapterProxy)
{
    while(!_reqTimeoutQueue.empty())
    {
        TLOGTARS("[ObjectProxy::doInvoke, " << _name << ", conection queue pop size:" << _reqTimeoutQueue.size() << "]" << endl);

        ReqMessage * msg = NULL;
        _reqTimeoutQueue.pop(msg);

        assert(msg != NULL);

        //第一个请求包，adapter必然为NULL，如果需要hash，则重新选择一次
        if (msg->adapter == NULL && msg->data._hash)
        {
            //选取的adapter和之前的不一样(hash的原因), 需要重新选择一个远程服务的Adapter来调用
            _endpointManger->selectAdapterProxy(msg, adapterProxy);

            if (!adapterProxy)
            {
                //这里肯定是请求过主控
                TLOGERROR("[ObjectProxy::doInvoke, " << _name << ", selectAdapterProxy is null]" << endl);
                msg->response->iRet = TARSADAPTERNULL;
                doInvokeException(msg);
                return;
            }

            msg->adapter = adapterProxy;
        }
        else
        {
            msg->adapter = adapterProxy;
        }

        adapterProxy->invoke(msg);
    }
    
}

void ObjectProxy::onConnect(AdapterProxy *adapterProxy)
{
    prepareConnection(adapterProxy);
}

void ObjectProxy::onNotifyEndpoints(const set<EndpointInfo> & active,const set<EndpointInfo> & inactive)
{
	if(this->getRootServantProxy()) {
		this->getRootServantProxy()->onNotifyEndpoints(_communicatorEpoll, active, inactive);
	}
}

//主控查询到地址后过来的
void ObjectProxy::doInvoke()
{
	assert(this->getCommunicatorEpoll()->getThreadId() == this_thread::get_id());

	TLOGTARS("[ObjectProxy::doInvoke, objname:" << _name << ", begin...]" << endl);

    for(auto it = _reqTimeoutQueue.begin(); it != _reqTimeoutQueue.end(); ++it)
	{
		ReqMessage * msg = (*it).ptr;

		AdapterProxy* adapterProxy;

		//选择一个远程服务的Adapter来调用, selectAdapterProxy会发起连接
		_endpointManger->selectAdapterProxy(msg, adapterProxy);
	}
}

void ObjectProxy::doInvokeException(ReqMessage * msg)
{
    //单向调用出现异常直接删除请求
    if(msg->eType == ReqMessage::ONE_WAY)
    {
        delete msg;
        return;
    }

    //标识请求异常
    msg->eStatus = ReqMessage::REQ_EXC;

    if (msg->adapter)
    {
        msg->adapter->stat(msg);
    }

    if(msg->eType == ReqMessage::SYNC_CALL)
    {
        if(!msg->sched)
        {
            assert(msg->pMonitor);

			msg->pMonitor->notify();
        }
        else
        {
            msg->sched->put(msg->iCoroId);
        }

        return;
    }

    if(msg->callback)
    {
		if(!msg->sched)
		{
			if(msg->callback->getNetThreadProcess())
			{
				ReqMessagePtr msgPtr = msg;
				//如果是本线程的回调，直接本线程处理
				//比如获取endpoint
				try
				{
					msg->callback->dispatch(msgPtr);
				}
				catch(exception & e)
				{
					TLOGERROR("[ObjectProxy::doInvokeException exp:"<<e.what()<<" ,line:"<<__LINE__<<endl);
				}
				catch(...)
				{
					TLOGERROR("[ObjectProxy::doInvokeException exp:unknown line:|"<<__LINE__<<endl);
				}
			}
			else
			{
                //先确保adapter 非null
                if (msg->adapter)
                {
                    //异步回调，放入回调处理线程中
                    _communicatorEpoll->pushAsyncThreadQueue(msg);
                }
                else
                {
                    TLOGERROR("[ObjectProxy::doInvokeException push adapter is null|" << __LINE__ << endl);
                    delete msg;
                    msg = NULL;
                }
			}
		}
		else
		{
  
			CoroParallelBasePtr ptr = msg->callback->getCoroParallelBasePtr();
			if(ptr)
			{
				ptr->insert(msg);
				if(ptr->checkAllReqReturn())
				{
					msg->sched->put(msg->iCoroId);
				}
			}
			else
			{
				TLOGERROR("[ObjectProxy::doInvokeException coro parallel callback error,obj:" << _name << endl);
				delete msg;
				msg = NULL;
			}
        
		}
    }
}

void ObjectProxy::doTimeout()
{
	if(!_hasInitialize)
	{
		return;
	}
	assert(this->getCommunicatorEpoll()->getThreadId() == this_thread::get_id());

	const vector<AdapterProxy*> & vAdapterProxy = _endpointManger->getAdapters();

    for(size_t iAdapter=0; iAdapter< vAdapterProxy.size();++iAdapter)
    {
        if(vAdapterProxy[iAdapter] != NULL)
        {
            vAdapterProxy[iAdapter]->doTimeout();
        }
    }

    ReqMessage * reqInfo = NULL;
    while(_reqTimeoutQueue.timeout(reqInfo))
    {
        reqInfo->response->iRet = TARSINVOKETIMEOUT;

        doInvokeException(reqInfo);
    }
}

void ObjectProxy::doKeepAlive()
{
	if(!_hasInitialize)
	{
		return;
	}
	assert(this->getCommunicatorEpoll()->getThreadId() == this_thread::get_id());

	const vector<AdapterProxy*> & vAdapterProxy = _endpointManger->getActiveAdapters(true);

    for(size_t iAdapter=0; iAdapter< vAdapterProxy.size();++iAdapter)
    {
        if(vAdapterProxy[iAdapter] != NULL)
        {
            vAdapterProxy[iAdapter]->doKeepAlive();
        }
    }
}

void ObjectProxy::mergeStat(map<StatMicMsgHead, StatMicMsgBody> & mStatMicMsg)
{
	if(!_hasInitialize)
	{
		return;
	}
	assert(this->getCommunicatorEpoll()->getThreadId() == this_thread::get_id());

	const vector<AdapterProxy*> & vAdapterProxy = _endpointManger->getAdapters();
    for(size_t iAdapter=0; iAdapter< vAdapterProxy.size();++iAdapter)
    {
        if(vAdapterProxy[iAdapter] != NULL)
        {
            vAdapterProxy[iAdapter]->mergeStat(mStatMicMsg);
        }
    }
}

void ObjectProxy::onSetInactive(const EndpointInfo& ep)
{
	if(!_hasInitialize)
	{
		return;
	}
	assert(this->getCommunicatorEpoll()->getThreadId() == this_thread::get_id());

	const vector<AdapterProxy*> & vAdapterProxy = _endpointManger->getAdapters();
	for(size_t iAdapter=0; iAdapter< vAdapterProxy.size();++iAdapter)
	{
        if(vAdapterProxy[iAdapter] != NULL)
        {
            if (vAdapterProxy[iAdapter]->endpoint() == ep)
            {
                vAdapterProxy[iAdapter]->onSetInactive();
            }
        }
	}
}

void ObjectProxy::close()
{
    if(!_hasInitialize)
    {
        return;
    }
    assert(this->getCommunicatorEpoll()->getThreadId() == this_thread::get_id());

    const vector<AdapterProxy*> & vAdapterProxy = _endpointManger->getAdapters();
    for(size_t iAdapter=0; iAdapter< vAdapterProxy.size();++iAdapter)
    {
        if(vAdapterProxy[iAdapter] != NULL)
        {
            vAdapterProxy[iAdapter]->onClose();
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////
}
