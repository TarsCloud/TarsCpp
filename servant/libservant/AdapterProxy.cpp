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

#include "servant/AdapterProxy.h"
#include "servant/Communicator.h"
#include "servant/CommunicatorEpoll.h"
#include "servant/StatReport.h"
#include "servant/RemoteLogger.h"
#include "tup/tup.h"
#include "servant/StatF.h"

// #ifdef TARS_OPENTRACKING
// #include "servant/text_map_carrier.h"
// #endif

namespace tars
{
    
std::atomic<int> AdapterProxy::_idGen;

AdapterProxy::AdapterProxy(ObjectProxy * pObjectProxy, const EndpointInfo &ep, Communicator* pCom)
: _communicator(pCom)
, _objectProxy(pObjectProxy)
, _ep(ep)
, _activeStateInReg(true)
, _activeStatus(true)
, _totalInvoke(0)
, _timeoutInvoke(0)
, _nextFinishInvokeTime(0)
, _frequenceFailInvoke(0)
, _frequenceFailTime(0)
, _nextRetryTime(0)
, _nextKeepAliveTime(0)
//, _connTimeout(false)
, _connExc(false)
, _connExcCnt(0)
, _timeoutLogFlag(true)
, _noSendQueueLimit(100000)
, _id((++_idGen))
{
    _timeoutQueue.reset(new TC_TimeoutQueueNew<ReqMessage*>());

    if(pObjectProxy->getCommunicatorEpoll())
    {
        _noSendQueueLimit = pObjectProxy->getCommunicatorEpoll()->getNoSendQueueLimit();
    }

    if(_communicator)
    {
        _timeoutLogFlag = _communicator->getTimeoutLogFlag();
    }

#if TARS_SSL
    if (ep.isSsl())
    {
        _trans.reset(new TC_SSLTransceiver(pObjectProxy->getCommunicatorEpoll()->getEpoller(), ep.getEndpoint()));
    } 
    else if (ep.isTcp())
    {
        _trans.reset(new TC_TCPTransceiver(pObjectProxy->getCommunicatorEpoll()->getEpoller(), ep.getEndpoint()));
    }
    else
    {
        _trans.reset(new TC_UDPTransceiver(pObjectProxy->getCommunicatorEpoll()->getEpoller(), ep.getEndpoint()));
    }
#else
    if (ep.isUdp())
    {
        _trans.reset(new TC_UDPTransceiver(pObjectProxy->getCommunicatorEpoll()->getEpoller(), ep.getEndpoint()));
    } 
    else
    {
        _trans.reset(new TC_TCPTransceiver(pObjectProxy->getCommunicatorEpoll()->getEpoller(), ep.getEndpoint()));
    }
#endif

    _trans->initializeClient(std::bind(&AdapterProxy::onCreateCallback, this, std::placeholders::_1), 
        std::bind(&AdapterProxy::onCloseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&AdapterProxy::onConnectCallback, this, std::placeholders::_1),
        std::bind(&AdapterProxy::onRequestCallback, this, std::placeholders::_1),
        std::bind(&AdapterProxy::onParserCallback, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&AdapterProxy::onOpensslCallback, this, std::placeholders::_1),
	    std::bind(&AdapterProxy::onCompletePackage, this, std::placeholders::_1));

    _trans->setClientAuthCallback(std::bind(&AdapterProxy::onSendAuthCallback, this, std::placeholders::_1), 
        std::bind(&AdapterProxy::onVerifyAuthCallback, this, std::placeholders::_1, std::placeholders::_2));


//    if (!_endpoint.isTcp())
//    {
//        _checkTransInterval = 10;    //udp端口10秒检查一次, 避免影响用户请求
//    }

    //初始化stat的head信息
    initStatHead();
}

AdapterProxy::~AdapterProxy()
{
}

shared_ptr<TC_ProxyInfo> AdapterProxy::onCreateCallback(TC_Transceiver* trans)
{
	// LOG_CONSOLE_DEBUG << "fd:" << trans->fd() << ", " << trans << endl;

    _objectProxy->getCommunicatorEpoll()->addFd(this);

    trans->setConnTimeout(_objectProxy->getRootServantProxy()->tars_connect_timeout());

    if(_objectProxy->getRootServantProxy()->getProxyInfo())
    {
        return TC_ProxyInfo::createProxyInfo(*_objectProxy->getRootServantProxy()->getProxyInfo());
    }
    return NULL;
}

std::shared_ptr<TC_OpenSSL> AdapterProxy::onOpensslCallback(TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG << "fd:" << trans->fd() << ", " << trans << endl;
    return _objectProxy->getCommunicatorEpoll()->getCommunicator()->newClientSSL(_objectProxy->name());
}

void AdapterProxy::onCloseCallback(TC_Transceiver* trans, TC_Transceiver::CloseReason reason, const string &err)
{
    if(_objectProxy->getRootServantProxy()->tars_get_push_callback())
    {
		_objectProxy->getRootServantProxy()->tars_get_push_callback()->onClose();
    }

    int second =_objectProxy->reconnect();

    if(second > 0) 
    {
        _objectProxy->getCommunicatorEpoll()->reConnect(TNOWMS + second * 1000, trans);
        TLOGERROR("[trans close:" << _objectProxy->name() << "," << trans->getConnectEndpoint().toString() << ", reconnect:" << second << "]" << endl);
    }   
}

void AdapterProxy::onConnectCallback(TC_Transceiver* trans)
{
	// LOG_CONSOLE_DEBUG << "fd:" << trans->fd() << ", " << trans << endl;
    addConnExc(false);

    if(_objectProxy->getRootServantProxy()->tars_get_push_callback())
    {
		_objectProxy->getRootServantProxy()->tars_get_push_callback()->onConnect(trans->getConnectEndpoint());
    }

	_objectProxy->onConnect(this);
}

void AdapterProxy::onRequestCallback(TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG  << "fd:" << trans->fd() << ", " << trans << endl;
    doInvoke();
}

TC_NetWorkBuffer::PACKET_TYPE AdapterProxy::onParserCallback(TC_NetWorkBuffer& buff, TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG  << "fd:" << trans->fd() << ", " << trans<< endl;
    try
    {
        shared_ptr<ResponsePacket> rsp = std::make_shared<ResponsePacket>();

        TC_NetWorkBuffer::PACKET_TYPE ret = _objectProxy->getRootServantProxy()->tars_get_protocol().responseFunc(buff, *rsp.get());

        if(ret == TC_NetWorkBuffer::PACKET_FULL || ret == TC_NetWorkBuffer::PACKET_FULL_CLOSE)
        {
            finishInvoke(rsp);
        }
        
        return ret;
    }
    catch(exception &ex)
    {
		TLOG_ERROR(ex.what() << ", obj: " << _objectProxy->name() << ", desc:" << _trans->getConnectionString()<< endl);
    }

    return TC_NetWorkBuffer::PACKET_ERR;
}

void AdapterProxy::onCompletePackage(TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG  << "fd:" << trans->fd() << ", " << trans << endl;
	if(_objectProxy->getRootServantProxy()->tars_connection_serial() > 0)
	{
		_objectProxy->prepareConnection(this);

		if(!_timeoutQueue->sendListEmpty())
		{
			//并行连接模式, 继续发起连接, 建立连接后, 会自动doInvoke发包
			if(_trans->hasConnected())
			{
				doInvoke();
			}
			else
			{
				checkActive(true);
			}
		}
	}
}

shared_ptr<TC_NetWorkBuffer::Buffer> AdapterProxy::onSendAuthCallback(TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG  << "fd:" << trans->fd() << ", " << trans << endl;

    // 走框架的AK/SK认证
    BasicAuthInfo info;
    info.sObjName      = _objectProxy->name();
    info.sAccessKey    = _objectProxy->getCommunicatorEpoll()->getCommunicator()->getServantProperty(_objectProxy->name(), "accesskey");
    info.sSecretKey    = _objectProxy->getCommunicatorEpoll()->getCommunicator()->getServantProperty(_objectProxy->name(), "secretkey");

//    std::string out = tars::defaultCreateAuthReq(info);

    const int kAuthType = 0x40;
    RequestPacket request;
    request.sFuncName       = "InnerAuthServer";
    request.sServantName    = "authServant";
    request.iVersion        = TARSVERSION;
    request.iRequestId      = 1;
    request.cPacketType     = TARSNORMAL;
    request.iMessageType    = kAuthType;
    request.sBuffer			= tars::defaultCreateAuthReq(info);//.assign(out.begin(), out.end());

    return _objectProxy->getRootServantProxy()->tars_get_protocol().requestFunc(request, trans);
}

TC_NetWorkBuffer::PACKET_TYPE AdapterProxy::onVerifyAuthCallback(TC_NetWorkBuffer &buff, TC_Transceiver*trans)
{
    shared_ptr<ResponsePacket> rsp = std::make_shared<ResponsePacket>();

    TC_NetWorkBuffer::PACKET_TYPE ret = _objectProxy->getRootServantProxy()->tars_get_protocol().responseFunc(buff, *rsp.get());

    if(ret == TC_NetWorkBuffer::PACKET_FULL)
    {
//		std::string ret(rsp->sBuffer.begin(), rsp->sBuffer.end());
        if(TC_Port::strncasecmp(rsp->sBuffer.data(), "AUTH_SUCC", rsp->sBuffer.size()) == 0)
        {
            return TC_NetWorkBuffer::PACKET_FULL;
        }   
        return TC_NetWorkBuffer::PACKET_ERR;
    }
    
    return ret;
}

void AdapterProxy::initStatHead()
{
	vector <string> vtSetInfo;
	if(!ClientConfig::SetDivision.empty() && StatReport::divison2SetInfo(ClientConfig::SetDivision, vtSetInfo)) 	{
		//主调(client)启用set
		_statHead.masterName = StatReport::trimAndLimitStr(ClientConfig::ModuleName + "." + vtSetInfo[0] + vtSetInfo[1] + vtSetInfo[2] + "@" + ClientConfig::TarsVersion, StatReport::MAX_MASTER_NAME_LEN);
	}
	else
	{
		_statHead.masterName = StatReport::trimAndLimitStr(ClientConfig::ModuleName + "@" + ClientConfig::TarsVersion, StatReport::MAX_MASTER_NAME_LEN);
	}

    string sSlaveSet = _ep.setDivision();
    const string sSlaveName = getSlaveName(_objectProxy->name());

    if (!sSlaveSet.empty() && StatReport::divison2SetInfo(sSlaveSet, vtSetInfo)) //被调启用set
    {
        _statHead.slaveSetName   = vtSetInfo[0];
        _statHead.slaveSetArea   = vtSetInfo[1];
        _statHead.slaveSetID     = vtSetInfo[2];
        _statHead.slaveName      = StatReport::trimAndLimitStr(sSlaveName + "." + vtSetInfo[0] + vtSetInfo[1] + vtSetInfo[2], StatReport::MAX_MASTER_NAME_LEN);
    }
    else
    {
        _statHead.slaveName = StatReport::trimAndLimitStr(sSlaveName, StatReport::MAX_MASTER_NAME_LEN);
    }

    _statHead.slaveIp = StatReport::trimAndLimitStr(_ep.host(), StatReport::MAX_MASTER_IP_LEN);
    _statHead.slavePort = _ep.port();

    _statHead.returnValue  = 0;
}

string AdapterProxy::getSlaveName(const string& sSlaveName)
{
    string::size_type pos = sSlaveName.find(".");
    if (pos != string::npos)
    {
        pos = sSlaveName.find(".", pos + 1);
        if (pos != string::npos)
        {
            return sSlaveName.substr(0, pos);
        }
    }

    return  sSlaveName;
}

void AdapterProxy::onConnect()
{
	_objectProxy->onConnect(this);
}

int AdapterProxy::invoke_connection_serial(ReqMessage * msg)
{
	assert(msg->eType != ReqMessage::ONE_WAY);

	msg->sReqData = _objectProxy->getRootServantProxy()->tars_get_protocol().requestFunc(msg->request, _trans.get());

	msg->request.iRequestId = _timeoutQueue->generateId();

	if(!_requestMsg && _timeoutQueue->sendListEmpty())
	{
		int ret = _trans->sendRequest(msg->sReqData);

		if(ret == TC_Transceiver::eRetOk || ret == TC_Transceiver::eRetFull)
		{
			TLOGTARS("[AdapterProxy::invoke_connection_serial push (send) obj: " << _objectProxy->name() << ", desc:" << _trans->getConnectionString() << ", id: " << msg->request.iRequestId << endl);

			_requestMsg = msg;

			bool bFlag = _timeoutQueue->push(msg, msg->request.iRequestId, msg->request.iTimeout + msg->iBeginTime);
			if (!bFlag)
			{
				TLOGERROR("[AdapterProxy::invoke_connection_serial fail1 : insert timeout queue fail,queue size:" << _timeoutQueue->size() << ",id: " << msg->request.iRequestId << "," << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);

				_requestMsg  = NULL;
				msg->eStatus = ReqMessage::REQ_EXC;

				finishInvoke(msg);
			}

			return 0;
		}
		else if(ret == TC_Transceiver::eRetError)
		{
			//发送出错了
			_requestMsg  = NULL;

			msg->eStatus = ReqMessage::REQ_EXC;

			finishInvoke(msg);

			return -1;
		}
	}

	//数据没有发送
	TLOGTARS("[AdapterProxy::invoke_connection_serial push (no send) " << _objectProxy->name() << ", " << _trans->getConnectionString() << ",id " << msg->request.iRequestId << ", " << _requestMsg << endl);

	//数据没有发送
	bool bFlag = _timeoutQueue->push(msg, msg->request.iRequestId, msg->request.iTimeout + msg->iBeginTime, false);
	if (!bFlag)
	{
		TLOGERROR("[AdapterProxy::invoke_connection_serial fail2 : insert timeout queue fail,queue size:" << _timeoutQueue->size() << ", id: " << msg->request.iRequestId << ", " << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);
		msg->eStatus = ReqMessage::REQ_EXC;

		finishInvoke(msg);
	}

	return 0;
}

int AdapterProxy::invoke_connection_parallel(ReqMessage * msg)
{
	msg->sReqData = _objectProxy->getRootServantProxy()->tars_get_protocol().requestFunc(msg->request, _trans.get());

	//当前队列是空的, 且是连接复用模式, 交给连接发送数据
	//连接连上 buffer不为空  发送数据成功
	if (_timeoutQueue->sendListEmpty())
	{
		int ret = _trans->sendRequest(msg->sReqData);

		if(ret == TC_Transceiver::eRetOk || ret == TC_Transceiver::eRetFull)
		{
			TLOGTARS("[AdapterProxy::invoke_connection_parallel push (send) obj: " << _objectProxy->name() << ", desc:" << _trans->getConnectionString() << ", id: " << msg->request.iRequestId << endl);

            //请求发送成功了 处理采样
            //这个请求发送成功了。单向调用直接返回
            if (msg->eType == ReqMessage::ONE_WAY)
            {
        // #ifdef TARS_OPENTRACKING
        //     finishTrack(msg);
        // #endif			
                delete msg;
                msg = NULL;

				return 0;
			}

			bool bFlag = _timeoutQueue->push(msg, msg->request.iRequestId, msg->request.iTimeout + msg->iBeginTime);
			if (!bFlag)
			{
				TLOGERROR("[AdapterProxy::invoke_connection_parallel fail1 : insert timeout queue fail,queue size:" << _timeoutQueue->size() << ",id: " << msg->request.iRequestId << "," << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);
				msg->eStatus = ReqMessage::REQ_EXC;

				finishInvoke(msg);
			}

			return 0;
		}
		else if(ret == TC_Transceiver::eRetError)
		{
			TLOGTARS("[AdapterProxy::invoke_connection_parallel send request failed,queue size:" << _timeoutQueue->size() << ",id: " << msg->request.iRequestId << "," << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);

			//发送出错了
			msg->eStatus = ReqMessage::REQ_EXC;

			finishInvoke(msg);

			return -1;
		}
	}

	//没有发送数据
	TLOGTARS("[AdapterProxy::invoke_connection_parallel push (no send) " << _objectProxy->name() << ", " << _trans->getConnectionString() << ",id " << msg->request.iRequestId << endl);

	//之前还没有数据没发送 或者 请求发送失败了, 进队列
	bool bFlag = _timeoutQueue->push(msg, msg->request.iRequestId, msg->request.iTimeout + msg->iBeginTime, false);
	if (!bFlag)
	{
		TLOGERROR("[AdapterProxy::invoke_connection_parallel fail2 : insert timeout queue fail,queue size:" << _timeoutQueue->size() << ", id: " << msg->request.iRequestId << ", " << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);
		msg->eStatus = ReqMessage::REQ_EXC;

		finishInvoke(msg);
	}

	return 0;
}

int AdapterProxy::invoke(ReqMessage * msg)
{
    assert(_trans != NULL);

    TLOGTARS("[AdapterProxy::invoke " << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);

    //未发链表有长度限制
    if (_timeoutQueue->getSendListSize() > _noSendQueueLimit)
    {
        TLOGERROR("[AdapterProxy::invoke fail,ReqInfoQueue.size(" << _timeoutQueue->getSendListSize() << ") > " << _noSendQueueLimit << "," << _objectProxy->name() << "," << _trans->getConnectionString() << "]" << endl);

        msg->eStatus = ReqMessage::REQ_EXC;

        msg->response->iRet = tars::TARSSENDREQUESTERR;
        finishInvoke(msg);
        return 0;
    }

    //生成requestid
    //tars调用 而且 不是单向调用
    if (!msg->bFromRpc)
    {
        msg->request.iRequestId = _timeoutQueue->generateId();
    }

// #ifdef TARS_OPENTRACKING
// 	startTrack(msg);
// #endif

    if(_objectProxy->getRootServantProxy()->tars_connection_serial() > 0)
    {
	    return invoke_connection_serial(msg);
    }
	else
	{
	    return invoke_connection_parallel(msg);
    }
}

void AdapterProxy::doInvoke_serial()
{
	if(_requestMsg != NULL || _timeoutQueue->sendListEmpty())
    {
        //有请求 or 发送队列是空的, 当前请求不再发送 等epoll事件通知 在doRequest中发送
        TLOGTARS("[AdapterProxy::doInvoke_serial not send obj:" << _objectProxy->name() << ",desc:" << _trans->getConnectionString() << ", send size:" << _timeoutQueue->getSendListSize() << ", " << _requestMsg << endl);
        return;
    }

    ReqMessage * msg = NULL;

    _timeoutQueue->getSend(msg);

    int iRet = _trans->sendRequest(msg->sReqData);

    if(iRet == TC_Transceiver::eRetError)
    {
        _requestMsg = NULL;

        _timeoutQueue->popSend(true);

        msg->response->iRet = TARSSENDREQUESTERR;

        TLOGTARS("[AdapterProxy::doInvoke_serial sendRequest failed, obj:" << _objectProxy->name() << ",desc:" << _trans->getConnectionString() << ",id:" << msg->request.iRequestId << ", ret:" << iRet << endl);

        finishInvoke(msg);  

    }
    else if(iRet == TC_Transceiver::eRetOk || iRet == TC_Transceiver::eRetFull)
    {
        _requestMsg = msg;
        //从发送send 队列中清掉, 但是保留在定时队列中
        _timeoutQueue->popSend(false);
    }
}

void AdapterProxy::doInvoke_parallel()
{
	while(!_timeoutQueue->sendListEmpty())
	{
		ReqMessage * msg = NULL;

		_timeoutQueue->getSend(msg);

		int iRet = _trans->sendRequest(msg->sReqData);

		//发送失败 or 没有发送
		if (iRet == TC_Transceiver::eRetError)
		{
			TLOGTARS("[AdapterProxy::doInvoke_parallel sendRequest failed, obj:" << _objectProxy->name() << ",desc:" << _trans->getConnectionString() << ",id:" << msg->request.iRequestId << ", ret:" << iRet << endl);
			return;
		}

		if (iRet == TC_Transceiver::eRetNotSend)
		{
			TLOGTARS("[AdapterProxy::doInvoke_parallel sendRequest not send, obj:" << _objectProxy->name() << ",desc:" << _trans->getConnectionString() << ",id:" << msg->request.iRequestId << ", ret:" << iRet << endl);
			return;
		}

		//发送完成，要从队列里面清掉
		_timeoutQueue->popSend(msg->eType == ReqMessage::ONE_WAY);
		if (msg->eType == ReqMessage::ONE_WAY)
		{
			delete msg;
			msg = NULL;
		}

		//发送buffer已经满了 要返回
		if (iRet == TC_Transceiver::eRetFull)
		{
			return;
		}
	}
}

void AdapterProxy::doInvoke()
{
	if(_objectProxy->getRootServantProxy()->tars_connection_serial() > 0)
	{
		doInvoke_serial();
	}
	else
	{
		doInvoke_parallel();
	}
}

void AdapterProxy::finishInvoke(bool bFail)
{
    TLOGTARS("[AdapterProxy::finishInvoke(bool), " << _objectProxy->name() << ", " << _trans->getConnectionString() << "," << bFail << "]" << endl);

    time_t now = TNOW;

    const CheckTimeoutInfo& info = _objectProxy->getRootServantProxy()->tars_check_timeout_info();

    //处于异常状态 已经屏蔽
    if (!_activeStatus)
    {
        if (!bFail)
        {
            //重试成功,恢复正常状态
            _activeStatus = true;

            //连续失败次数清零
            _frequenceFailInvoke = 0;

            _nextFinishInvokeTime = now + info.checkTimeoutInterval;

            _frequenceFailInvoke = 0;

            _totalInvoke = 1;

            _timeoutInvoke = 0;

            _trans->setIsConnTimeout(false);

            _connExc              = false;

            TLOGTARS("[AdapterProxy::finishInvoke(bool), " << _objectProxy->name() << ", " << _trans->getConnectionString() << ", retry ok]" << endl);
        }
        else
        {
            //结点已经屏蔽 过来失败的包不用处理
            TLOGTARS("[AdapterProxy::finishInvoke(bool), " << _objectProxy->name() << ", " << _trans->getConnectionString() << ", retry fail]" << endl);
        }
        return;
    }

    ++_totalInvoke;

    if (bFail)
    {
        //调用失败

        //失败次数+1
        ++_timeoutInvoke;

        //连续失败时间间隔重新计算
        if (0 == _frequenceFailInvoke)
        {
            _frequenceFailTime = now + info.minFrequenceFailTime;
        }
        //连续失败次数加1
        _frequenceFailInvoke++;

        //检查是否到了连续失败次数,且至少在5s以上
        if (_frequenceFailInvoke >= info.frequenceFailInvoke && now >= _frequenceFailTime)
        {
            setInactive();

             _activeStatus = false;

	        resetRetryTime();

            TLOGERROR("[AdapterProxy::finishInvoke(bool) objname:"<< _objectProxy->name() 
                    << ",desc:" << _trans->getConnectionString()
                    << ",disable frequenceFail,freqtimeout:" << _frequenceFailInvoke
                    << ",timeout:"<< _timeoutInvoke
                    << ",total:" << _totalInvoke << endl);
            return ;
        }
    }
    else
    {
        _frequenceFailInvoke = 0;
    }

    //判断一段时间内的超时比例
    if (now > _nextFinishInvokeTime)
    {
        _nextFinishInvokeTime = now + info.checkTimeoutInterval;

        if (bFail && _timeoutInvoke >= info.minTimeoutInvoke && _timeoutInvoke >= info.radio * _totalInvoke)
        {
            setInactive();
            TLOGERROR("[AdapterProxy::finishInvoke(bool), "
                      << _objectProxy->name() << "," << _trans->getConnectionString()
                      << ",disable radioFail,freqtimeout:" << _frequenceFailInvoke
                      << ",timeout:" << _timeoutInvoke
                      << ",total:" << _totalInvoke << "] " << endl);
        }
        else
        {
            //每一分钟清空一次
            _totalInvoke   = 0;

            _timeoutInvoke = 0;
        }
    }
}

void AdapterProxy::resetRetryTime(bool next)
{
	if(next) {
		_nextRetryTime = TNOW + _objectProxy->getRootServantProxy()->tars_check_timeout_info().tryTimeInterval;
	}else {
		_nextRetryTime = TNOW;
	}
}

bool AdapterProxy::checkActive(bool connecting)
{
    time_t now = TNOW;

    TLOGTARS("[AdapterProxy::checkActive,"
                << _objectProxy->name() << "," << _trans->getConnectionString() << ","
                << (_activeStatus ? "active" : "inactive")
                << (connecting ? ", connecting" : "")
                << ", freqtimeout:" << _frequenceFailInvoke
                << ", timeout:" << _timeoutInvoke
                << ", connExcCnt:" << _connExcCnt
                << ", total:" << _totalInvoke << "]" << endl);



    //失效且没有到下次重试时间, 直接返回不可用
    if ((!_activeStatus) && (now < _nextRetryTime))
    {
        TLOGTARS("[AdapterProxy::checkActive,not reach retry time ," << _objectProxy->name() << ","
                                                                            << _trans->getConnectionString() << endl);
        return false;
    }

    if (!_activeStatus)
    {
	    resetRetryTime();
    }

    //连接没有建立或者连接无效, 重新建立连接
    if (!_trans->isValid()) 
    {
        try
        {
            _trans->connect();
        }
        catch (exception & ex) 
        {
            _activeStatus = false;
            _trans->close();

            TLOGERROR("[AdapterProxy::checkActive connect obj:" << _objectProxy->name() << ",desc:" << _trans->getConnectionString() << ", ex:" << ex.what() << endl);
        }
    }

    if(connecting && _activeStatus) {
    	//hash模式, 且是第一次连接(_activeStatus=true, 即没有失败过), 返回已经连接或者正在连接的, 这样保证第一次hash不会错且连接挂过以后, 不会马上就使用, 直到连接成功才使用!
	    return (_trans->hasConnected() || _trans->isConnecting());
    }
    else {
	    return _trans->hasConnected();
    }
}

void AdapterProxy::onSetInactive()
{
	_activeStatus = false;

	resetRetryTime();

	//需要关闭连接
	_trans->close();
}

//屏蔽结点
void AdapterProxy::setInactive()
{
	onSetInactive();

	_objectProxy->getRootServantProxy()->onSetInactive(_ep);

    TLOGTARS("[AdapterProxy::setInactive, " << _objectProxy->name() << ", " << _trans->getConnectionString() << ", inactive]" << endl);
}

void AdapterProxy::finishInvoke_serial(shared_ptr<ResponsePacket> & rsp)
{
	TLOGTARS("[AdapterProxy::finishInvoke_serial, " << _objectProxy->name() << ", " << _trans->getConnectionString() << ", id:" << rsp->iRequestId << "]" << endl);

	if (!_requestMsg)
	{
		if(_timeoutLogFlag)
		{
			TLOGERROR("[AdapterProxy::finishInvoke_serial,"
				          << _objectProxy->name() 
				          << ", get req-ptr NULL,may be timeout,id:"
				          << rsp->iRequestId << ",desc:" << _trans->getConnectionString() << "]" << endl);
		}
		return;
	}

	ReqMessage * msg = _requestMsg;

	//这里的队列中的发送链表中的数据可能已经在timeout的时候删除了，因此可能会core，在erase中要加判断
	//获取请求信息
	bool retErase = _timeoutQueue->erase(_requestMsg->request.iRequestId, msg);

	assert(retErase);
	assert(_requestMsg == msg);
	assert(msg->eType != ReqMessage::ONE_WAY);
	assert(msg->eStatus == ReqMessage::REQ_REQ);

	_requestMsg = NULL;

	msg->eStatus = ReqMessage::REQ_RSP;

	msg->response = rsp;

	finishInvoke(msg);

}

void AdapterProxy::finishInvoke_parallel(shared_ptr<ResponsePacket> & rsp)
{
	TLOGTARS("[AdapterProxy::finishInvoke_parallel, " << _objectProxy->name() << ", " << _trans->getConnectionString() << ", id:" << rsp->iRequestId << "]" << endl);

	ReqMessage * msg = NULL;

	if (rsp->iRequestId == 0)
	{
		//requestid 为0 是push消息, push callback is null
		if (!_objectProxy->getRootServantProxy()->tars_get_push_callback())
		{
			TLOGERROR("[AdapterProxy::finishInvoke(BasePacket)， request id is 0, pushcallback is null, " << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);
            throw TarsDecodeException("request id is 0, pushcallback is null, obj: " + _objectProxy->name() + ", desc: " + _trans->getConnectionString());
		}
		msg = new ReqMessage();
		msg->eStatus = ReqMessage::REQ_RSP;
		msg->eType = ReqMessage::ASYNC_CALL;
		msg->bFromRpc     = true;
		msg->bPush = true;
		msg->proxy = _objectProxy->getServantProxy();
		msg->pObjectProxy = _objectProxy;
		msg->adapter = this;
		msg->callback = _objectProxy->getRootServantProxy()->tars_get_push_callback();
	}
	else
	{
		//这里的队列中的发送链表中的数据可能已经在timeout的时候删除了，因此可能会core，在erase中要加判断
		//获取请求信息
		bool retErase = _timeoutQueue->erase(rsp->iRequestId, msg);

		//找不到此id信息
		if (!retErase)
		{
			if (_timeoutLogFlag)
			{
				TLOGERROR("[AdapterProxy::finishInvoke_parallel,"
					          << _objectProxy->name() 
					          << ",get req-ptr NULL,may be timeout,id:"
					          << rsp->iRequestId << ",desc:" << _trans->getConnectionString() << "]" << endl);
			}
			return ;
		}

		assert(msg->eStatus == ReqMessage::REQ_REQ);

		msg->eStatus = ReqMessage::REQ_RSP;
	}

	msg->response = rsp;

	finishInvoke(msg);
}

void AdapterProxy::finishInvoke(shared_ptr<ResponsePacket> & rsp)
{
	if(_objectProxy->getRootServantProxy()->tars_connection_serial() > 0)
	{
		finishInvoke_serial(rsp);
	}
	else
	{
		finishInvoke_parallel(rsp);
	}
}

void AdapterProxy::finishInvoke(ReqMessage * msg)
{
    // assert(msg->eStatus != ReqMessage::REQ_REQ);

    TLOGTARS("[AdapterProxy::finishInvokeMsg " << _objectProxy->name() << ", " << _trans->getConnectionString() << " ,id:" << msg->response->iRequestId << "]" << endl);

// #ifdef TARS_OPENTRACKING
// 	finishTrack(msg);
// #endif

    //单向调用
    if (msg->eType == ReqMessage::ONE_WAY)
    {
        TLOGTARS("[AdapterProxy::finishInvokeMsg " << _objectProxy->name() << ", " << _trans->getConnectionString()
                 << " ,id:" << msg->response->iRequestId
                 << " ,one way call]" << endl);
        delete msg;
        msg = NULL;
        return ;
    }

    //stat 上报调用统计
    stat(msg);

    //超时屏蔽统计,异常不算超时统计
    if (msg->eStatus != ReqMessage::REQ_EXC && !msg->bPush)
    {
        finishInvoke(msg->response->iRet != TARSSERVERSUCCESS);
    }
    //同步调用，唤醒ServantProxy线程
    if (msg->eType == ReqMessage::SYNC_CALL)
    {
        if (!msg->sched)
        {
            assert(msg->pMonitor);

            msg->pMonitor->notify();
        }
        else
        {
            msg->sched->put(msg->iCoroId);
        }

        return ;
    }

    //异步调用
    if (msg->eType == ReqMessage::ASYNC_CALL)
    {
        if(!msg->sched)
        {
            if (msg->callback->getNetThreadProcess())
            {
                //如果是本线程的回调，直接本线程处理
                //比如获取endpoint
                ReqMessagePtr msgPtr = msg;
                try
                {
                    msg->callback->dispatch(msgPtr);
                }
                catch (exception & e)
                {
                    TLOGERROR("[AdapterProxy::finishInvoke(ReqMessage) exp:" << e.what() << " ,line:" << __LINE__ << "]" << endl);
                }
                catch (...)
                {
                    TLOGERROR("[AdapterProxy::finishInvoke(ReqMessage) exp:unknown line:|" << __LINE__ << "]" << endl);
                }
            }
            else
            {
                //异步回调，放入回调处理线程中
                _objectProxy->getCommunicatorEpoll()->pushAsyncThreadQueue(msg);
            }
        }
        else
        {
            CoroParallelBasePtr ptr = msg->callback->getCoroParallelBasePtr();
            if (ptr)
            {
                ptr->insert(msg);
                if (ptr->checkAllReqReturn())
                {
                    msg->sched->put(msg->iCoroId);
                }
            }
            else
            {
                TLOGERROR("[AdapterProxy::finishInvoke(ReqMessage) coro parallel callback error,obj:" << _objectProxy->name() << ",endpoint:" << _trans->getConnectionString() << " ,id:" << msg->response->iRequestId << "]" << endl);
                delete msg;
                msg = NULL;
            }
        }
        return;
    }

    assert(false);

    return;
}

void AdapterProxy::doTimeout()
{
    ReqMessage * msg;

    while (_timeoutQueue->timeout(msg))
    {
	    TLOGTARS("[AdapterProxy::doTimeout, " << _objectProxy->name() << ", " << _trans->getConnectionString() << ", id:" << msg->request.iRequestId << ", status:" << msg->eStatus << "]" << endl);

//        assert(msg->eStatus == ReqMessage::REQ_REQ);

		if(msg == _requestMsg)
		{
			_requestMsg = NULL;
			//timeout, close
			_trans->close();
		}

        msg->eStatus = ReqMessage::REQ_TIME;

        //有可能是单向调用超时了
        if (msg->eType == ReqMessage::ONE_WAY)
        {
            delete msg;
            msg = NULL;
            continue;
        }

        //如果是异步调用超时
        if (msg->eType == ReqMessage::ASYNC_CALL)
        {
            //_connExcCnt大于0说明是网络连接异常引起的超时
            msg->response->iRet = (_connExcCnt > 0 ? TARSPROXYCONNECTERR : TARSASYNCCALLTIMEOUT);
        }

        finishInvoke(msg);
    }
}

void AdapterProxy::doKeepAlive()
{
    if (!checkActive(false))
    {
        return;
    }

    time_t now = TNOW;
    if (now < _nextKeepAliveTime)
    {
        return;
    }

    _nextKeepAliveTime = now + _communicator->getKeepAliveInterval();
    TLOGTARS("[AdapterProxy::doKeepAlive, " << _objectProxy->name() << ", " << _trans->getConnectionString() << "]" << endl);

    ReqMessage *msg = new ReqMessage();

    msg->init(ReqMessage::ONE_WAY, _objectProxy->getServantProxy());
    msg->callback = NULL;

    msg->request.iVersion = TARSVERSION;
    msg->request.cPacketType = TARSONEWAY;
    msg->request.sFuncName = "tars_ping";
    msg->request.sServantName = _objectProxy->name();

    msg->request.iTimeout     = ServantProxy::DEFAULT_ASYNCTIMEOUT;

    msg->proxy         = _objectProxy->getServantProxy();
    msg->response->iRet = TARSSERVERUNKNOWNERR;

    //调用发起时间
    msg->iBeginTime   = TNOWMS;
    msg->pObjectProxy = _objectProxy;

    invoke(msg);
}


// #ifdef TARS_OPENTRACKING
// void AdapterProxy::startTrack(ReqMessage * msg)
// {
//     if(!_communicator->_traceManager)
//     {
//         TLOGTARS("tracer info is null, just return" << endl);
//         return;
//     }

//     string functionName = msg->request.sFuncName;
//     std::unique_ptr<opentracing::Span> span;
    
//     if(msg->trackInfoMap.empty())  //start a new track
//     {
//         //std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
//         // _communicator->_traceManager->_tracer->StartSpan(functionName, {opentracing::StartTimestamp(t1)});
//         span = _communicator->_traceManager->_tracer->StartSpan(functionName);
       
//     }else{
//         TextMapCarrier carrier1(msg->trackInfoMap);
//         auto span_context_maybe = _communicator->_traceManager->_tracer->Extract(carrier1);
//         assert(span_context_maybe);

//         //std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
//         //_communicator->_traceManager->_tracer->StartSpan(functionName, {opentracing::ChildOf(span_context_maybe->get()), opentracing::StartTimestamp(t1)});
//         span = _communicator->_traceManager->_tracer->StartSpan(functionName, {opentracing::ChildOf(span_context_maybe->get())});
//     }
//     //将调用链信息注入到request的status中
//     std::unordered_map<std::string, std::string> text_map;
//     TextMapCarrier carrier(text_map);
//     auto err = _communicator->_traceManager->_tracer->Inject(span->context(), carrier);
//     assert(err);
//     std::string contxt = read_span_context(text_map);
    
//     _spanMap[msg->request.iRequestId].reset(span.release());
//     //_spanMap.insert(std::move(make_pair(msg->request.iRequestId, std::move(span))));

//     msg->request.status[ServantProxy::STATUS_TRACK_KEY] = contxt;
//     SET_MSG_TYPE(msg->request.iMessageType, tars::TARSMESSAGETYPETRACK);
// }

// void AdapterProxy::finishTrack(ReqMessage * msg)
// {
//     map<int,std::unique_ptr<opentracing::Span>>::iterator spanIter = _spanMap.find(msg->request.iRequestId);

//     //report span info to zipkin collector 
//     if(spanIter != _spanMap.end())
//     {
//         if(msg->eType == ReqMessage::ONE_WAY)
//         {
//             spanIter->second->SetTag("Retcode", 0);
//         }
//         else
//         {
//             spanIter->second->SetTag("Retcode",msg->response->iRet);
//         }
        
//         spanIter->second->Finish();
//         _spanMap.erase(msg->response->iRequestId);
//     }
// }
// #endif

void AdapterProxy::stat(ReqMessage * msg)
{
    if (msg->bPush)
    {
        return ;
    }

    StatMicMsgBody body;
//    int64_t sptime = 0;

    msg->iEndTime = TNOWMS;

    //包体信息.
    if(msg->eStatus == ReqMessage::REQ_RSP && TARSSERVERSUCCESS == msg->response->iRet)
    {
        body.count = 1;
		int64_t sptime = (msg->iEndTime >= msg->iBeginTime) ? (msg->iEndTime - msg->iBeginTime) : 10000;

        body.totalRspTime = body.minRspTime = body.maxRspTime = sptime;
    }
    else if (msg->eStatus == ReqMessage::REQ_TIME)
    {
        body.timeoutCount = 1;
    }
    else
    {
        body.execCount = 1;
    }

    auto it = _statBody.find(msg->request.sFuncName);
    if (it != _statBody.end())
    {
        merge(body, it->second);
    }
    else
    {
        _communicator->getStatReport()->getIntervCount(body.maxRspTime, body);
        _statBody[msg->request.sFuncName] = body;
    }
}

void AdapterProxy::merge(const StatMicMsgBody& inBody, StatMicMsgBody& outBody/*out*/)
{
    outBody.count                += inBody.count;
    outBody.timeoutCount         += inBody.timeoutCount;
    outBody.execCount            += inBody.execCount;
    outBody.totalRspTime         += inBody.totalRspTime;
    if (outBody.maxRspTime < inBody.maxRspTime )
    {
        outBody.maxRspTime = inBody.maxRspTime;
    }
    //非0最小值
    if (outBody.minRspTime == 0 || (outBody.minRspTime > inBody.minRspTime && inBody.minRspTime != 0))
    {
        outBody.minRspTime = inBody.minRspTime;
    }
    _communicator->getStatReport()->getIntervCount(inBody.maxRspTime, outBody);
}

void AdapterProxy::mergeStat(map<StatMicMsgHead, StatMicMsgBody> & mStatMicMsg)
{
    auto iter = _statBody.begin();

    for (; iter != _statBody.end(); ++iter)
    {
        _statHead.interfaceName = iter->first;
        //有数据就放到map里面
        if (iter->second.count != 0
                || iter->second.timeoutCount != 0
                || iter->second.execCount != 0)
        {
            //判断是否已经有相同的数据了，需要汇总
            auto it = mStatMicMsg.find(_statHead);
            if (it != mStatMicMsg.end())
            {
                merge(iter->second, it->second);
            }
            else
            {
                mStatMicMsg[_statHead] = iter->second;
            }
        }
    }

    //清空数据
    _statBody.clear();
}

void AdapterProxy::addConnExc(bool bExc)
{
    if (bExc)
    {
        if(!_connExc && ++_connExcCnt >= _objectProxy->getRootServantProxy()->tars_check_timeout_info().maxConnectExc)
        {
//            TLOGERROR("[AdapterProxy::addConnExc ep: " << _trans->getConnectionString() << " connect exception! (connect error)]" << endl);

            setInactive();
            _connExc = true;
        }
    }
    else
    {
//        if (_connExc)
//        {
//            TLOGERROR("[AdapterProxy::addConnExc ep: " << _trans->getConnectionString() << " connect exception change to succ]" << endl);
//        }
        _connExc = false;
        _connExcCnt = 0;

        if (!_activeStatus)
        {
            _activeStatus = true;
        }
    }
}

}
