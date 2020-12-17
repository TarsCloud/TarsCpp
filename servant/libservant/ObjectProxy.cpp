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

#include "servant/ObjectProxy.h"
#include "servant/Communicator.h"
#include "servant/Global.h"
#include "servant/EndpointManager.h"
#include "servant/AppCache.h"
#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "servant/RemoteLogger.h"

namespace tars
{
////////////////////////////////////////////////////////////////////////////////////////////
ObjectProxy::ObjectProxy(CommunicatorEpoll * pCommunicatorEpoll, const string & sObjectProxyName,const string& setName)
: _communicatorEpoll(pCommunicatorEpoll)
, _sObjectProxyName(sObjectProxyName)
, _invokeSetId(setName)
, _isInvokeBySet(false)
, _hasSetProtocol(false)
, _conTimeout(1000)
, _servantProxy(NULL)
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

	pos = _name.find_first_of('#');

	if(pos != string::npos)
	{
		_hash = _name.substr(pos+1);
		_name = _name.substr(0,pos);
	}

    _proxyProtocol.requestFunc = ProxyProtocol::tarsRequest;
    _proxyProtocol.responseFunc = ProxyProtocol::tarsResponse;

	_endpointManger.reset(new EndpointManager(this, _communicatorEpoll->getCommunicator(), _sObjectProxyName, _communicatorEpoll->isFirstNetThread(), _invokeSetId));

}

ObjectProxy::~ObjectProxy()
{
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

void ObjectProxy::setPushCallbacks(const ServantProxyCallbackPtr& cb)
{
    _pushCallback = cb;
}

ServantProxyCallbackPtr ObjectProxy::getPushCallback()
{
    return _pushCallback;
}

void ObjectProxy::setProxyProtocol(const ProxyProtocol& protocol)
{
    if(_hasSetProtocol)
    {
        return ;
    }

    _hasSetProtocol = true;
    _proxyProtocol  = protocol;
}

ProxyProtocol& ObjectProxy::getProxyProtocol()
{
    return _proxyProtocol;
}


void ObjectProxy::setSocketOpt(int level, int optname, const void *optval, SOCKET_LEN_TYPE optlen)
{
    SocketOpt socketOpt;

    socketOpt.level        = level;
    socketOpt.optname = optname;
    socketOpt.optval     = optval;
    socketOpt.optlen     = optlen;

    _socketOpts.push_back(socketOpt);
}

vector<SocketOpt>& ObjectProxy::getSocketOpt()
{
    return _socketOpts;
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
	if(!msg->adapter->getTransceiver()->hasConnected())
    {
        bool bRet = _reqTimeoutQueue.push(msg,msg->request.iTimeout+msg->iBeginTime);

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

	    if(msg->adapter == NULL && msg->bHash)
//		    if(msg->adapter != NULL && msg->adapter != adapterProxy)
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

void ObjectProxy::finishInvoke(ReqMessage * msg, AdapterProxy *adapterProxy)
{
    // TLOGERROR("[ObjectProxy::doInvoke,finishInvokeSize, adapter queue size:" << adapterProxy->getTimeoutQueue()->getSendListSize() <<", object queue size:" << _reqTimeoutQueue.size() << ", " << adapterProxy << endl);
    
	if(getRootServantProxy()->tars_connection_serial() > 0)
    {
        prepareConnection(adapterProxy);

        if(!adapterProxy->getTimeoutQueue()->sendListEmpty())
        {
	        //并行连接模式, 继续发起连接, 建立连接后, 会自动doInvoke发包
	        if(adapterProxy->getTransceiver()->hasConnected()) {
		        adapterProxy->doInvoke(true);
	        }else {
		        adapterProxy->checkActive(true);
	        }
        }
    }
}

void ObjectProxy::onNotifyEndpoints(const set<EndpointInfo> & active,const set<EndpointInfo> & inactive)
{
	if(_servantProxy) {
		_servantProxy->onNotifyEndpoints(this->_communicatorEpoll->getCommunicatorEpollId(), active, inactive, true);
	}
}

//主控查询到地址后过来的
void ObjectProxy::doInvoke()
{
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
    // TLOGTARS("[ObjectProxy::doInvokeException, objname:" << _name << "]" << endl);

    //单向调用出现异常直接删除请求
    if(msg->eType == ReqMessage::ONE_WAY)
    {
        delete msg;
        return;
    }

    //标识请求异常
    msg->eStatus = ReqMessage::REQ_EXC;

    if(msg->eType == ReqMessage::SYNC_CALL)
    {
        if(!msg->bCoroFlag)
        {
            assert(msg->pMonitor);

            TC_ThreadLock::Lock sync(*(msg->pMonitor));

            msg->pMonitor->notify();
            msg->bMonitorFin = true;
        }
        else
        {
            msg->sched->put(msg->iCoroId);
        }

        return;
    }

    if(msg->callback)
    {
		if(!msg->bCoroFlag)
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
//        TLOGERROR("[ObjectProxy::doTimeout, objname:" << _name << ", queue timeout error]" << endl);

        reqInfo->response->iRet = TARSINVOKETIMEOUT;

        doInvokeException(reqInfo);
    }
}

void ObjectProxy::mergeStat(map<StatMicMsgHead, StatMicMsgBody> & mStatMicMsg)
{
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
	const vector<AdapterProxy*> & vAdapterProxy = _endpointManger->getAdapters();
	for(size_t iAdapter=0; iAdapter< vAdapterProxy.size();++iAdapter)
	{
		if(vAdapterProxy[iAdapter] != NULL && vAdapterProxy[iAdapter]->endpoint() == ep)
		{
			vAdapterProxy[iAdapter]->onSetInactive();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
}
