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

#include "servant/ServantProxy.h"
#include "servant/ServantHandle.h"
#include "servant/StatReport.h"
#include "servant/Application.h"
#include "servant/BaseF.h"
#include "servant/EndpointManager.h"
#include "servant/Message.h"
#include "servant/ProxyInfo.h"
#include "servant/RemoteLogger.h"

namespace tars
{

thread_local shared_ptr<ServantProxyThreadData> ServantProxyThreadData::g_sp;

SeqManager *ServantProxyThreadData::_pSeq = new SeqManager(MAX_CLIENT_NOTIFYEVENT_NUM);

///////////////////////////////////////////////////////////////
SeqManager::SeqManager(uint16_t iNum)
{
    assert(iNum < MAX_UNSIGN_SHORT);
    assert(iNum > 0);
    _p = NULL;
    _p = new SeqInfo[iNum];
    assert(_p);

    //0xff表示结束
    _free     = 0;
    _freeTail = iNum - 1;

    for (uint16_t i = 0; i < (uint16_t)iNum; i++)
    {
        _p[i].free = true;
        _p[i].next = i + 1;
    }
    _p[iNum - 1].next = MAX_UNSIGN_SHORT;
    _num              = iNum;
}

uint16_t SeqManager::get()
{
    TC_LockT<TC_SpinLock> lock(_mutex);

    assert(_free != MAX_UNSIGN_SHORT);

    assert(_p[_free].free);

    uint16_t buf = _free;

    if (_free == _freeTail)
    {
        assert(_p[buf].next == MAX_UNSIGN_SHORT);
        _freeTail = MAX_UNSIGN_SHORT;
    }

    _free = _p[buf].next;

    _p[buf].free = false;

    return buf;
}

void SeqManager::del(uint16_t iSeq)
{
    TC_LockT<TC_SpinLock> lock(_mutex);

    assert(iSeq < _num);
    assert(!_p[iSeq].free);

    _p[iSeq].next = MAX_UNSIGN_SHORT;
    if (MAX_UNSIGN_SHORT == _freeTail)
    {
        _free = iSeq;
    }
    else
    {
        _p[_freeTail].next = iSeq;
    }
    _p[iSeq].free = true;
    _freeTail     = iSeq;
}

///////////////////////////////////////////////////////////////
ServantProxyThreadData::ServantProxyThreadData()
: _queueInit(false)
, _reqQNo(0)
, _netSeq(0)
, _netThreadSeq(-1)
, _hash(false)
, _conHash(false)
, _hashCode(-1)
, _dyeing(false)
, _hasTimeout(false)
, _timeout(0)
, _sched(NULL)
, _objectProxyNum(0)
{
}

ServantProxyThreadData::~ServantProxyThreadData()
{
    try
    {
        if(_queueInit)
        {
            for(size_t i=0;i<_objectProxyNum;++i)
            {
                if(_objectProxyOwn.get()[i])
                {
                    ReqMessage * msg = new ReqMessage();
                    msg->eType = ReqMessage::THREAD_EXIT;

                    bool bEmpty = false;
                    _reqQueue[i]->push_back(msg, bEmpty);

                    _objectProxyOwn.get()[i]->getCommunicatorEpoll()->notifyDel(_reqQNo);
                }
            }
            _queueInit = false;

        }

        _pSeq->del(_reqQNo);
    }
    catch (...)
    {
    }
}

ServantProxyThreadData *ServantProxyThreadData::getData()
{
    if (!g_sp)
    {
        g_sp.reset(new ServantProxyThreadData());
        g_sp->_reqQNo = _pSeq->get();
    }
    return g_sp.get();
}

void ServantProxyThreadData::reset()
{
	return g_sp.reset();
}

///////////////////////////////////////////////////////////////////////////////////////////

ServantProxyCallback::ServantProxyCallback()
    : _bNetThreadProcess(false)
{
}

int ServantProxyCallback::dispatch(ReqMessagePtr msg)
{
    return onDispatch(msg);
}

///////////////////////////////////////////////////////////////////////////////////////////

int HttpServantProxyCallback::onDispatch(ReqMessagePtr msg)
{
    if (msg->response->iRet != TARSSERVERSUCCESS)
    {
        return onDispatchException(msg->request, *msg->response);
    }

    return onDispatchResponse(msg->request, *msg->response);
}

HttpServantProxyCallback::HttpServantProxyCallback(const HttpCallbackPtr &cb)
    : _httpCb(cb)
{
}

int HttpServantProxyCallback::onDispatchException(const RequestPacket &request, const ResponsePacket &response)
{
    if (_httpCb)
        return _httpCb->onHttpResponseException(response.iRet);

    return 0;
}

int HttpServantProxyCallback::onDispatchResponse(const RequestPacket &request, const ResponsePacket &response)
{
    assert(response.sBuffer.size() == sizeof(shared_ptr<TC_HttpResponse>));

    shared_ptr<TC_HttpResponse> rsp = *(shared_ptr<TC_HttpResponse> *)(response.sBuffer.data());

    if (_httpCb)
        return _httpCb->onHttpResponse(rsp);

    return 0;
}

///////////////////////////////////////////////////////////////
void coroWhenAll(const CoroParallelBasePtr &ptr)
{
    if(!ptr->checkAllReqSend())
    {
        TLOGERROR("[coroWhenAll use coro invoke interface's num not equal ptr set value]"<<endl);
        throw TarsUseCoroException("use coro invoke interface's num not equal ptr set value");
    }

    //线程私有数据
    ServantProxyThreadData *pSptd = ServantProxyThreadData::getData();
    assert(pSptd != NULL);

    if (!pSptd->_sched)
    {
        TLOGERROR("[coroWhenAll no open coroutine mode]"<<endl);
        throw TarsUseCoroException("coroWhenAll not open coroutine mode");
    }

    pSptd->_sched->yield(false);

    vector<ReqMessage *> vMsg = ptr->getAllReqMessage();

    for (size_t i = 0; i < vMsg.size(); ++i)
    {
        ReqMessagePtr msgPtr = vMsg[i];
        vMsg[i]->callback->dispatch(msgPtr);
    }
}

///////////////////////////////////////////////////////////////
string ServantProxy::STATUS_DYED_KEY      = "STATUS_DYED_KEY";

string ServantProxy::STATUS_GRID_KEY      = "STATUS_GRID_KEY";

string ServantProxy::STATUS_RESULT_CODE   = "STATUS_RESULT_CODE";

string ServantProxy::STATUS_RESULT_DESC   = "STATUS_RESULT_DESC";

string ServantProxy::STATUS_SETNAME_VALUE = "STATUS_SETNAME_VALUE";

string ServantProxy::STATUS_TRACK_KEY     = "STATUS_TRACK_KEY";

////////////////////////////////////
ServantProxy::ServantProxy(Communicator * pCommunicator, ObjectProxy ** ppObjectProxy, size_t iClientThreadNum)
: _communicator(pCommunicator)
, _objectProxy(ppObjectProxy)
, _objectProxyNum(iClientThreadNum)
, _syncTimeout(DEFAULT_SYNCTIMEOUT)
, _asyncTimeout(DEFAULT_ASYNCTIMEOUT)
, _id(0)
, _masterFlag(false)
, _minTimeout(100)
{
    _objectProxyOwn.reset(ppObjectProxy);
    _endpointInfo.reset(new EndpointManagerThread(pCommunicator, (*_objectProxy)->name()));

    for (size_t i = 0; i < _objectProxyNum; ++i)
    {
        (*(_objectProxy + i))->setServantProxy(this);
    }

    _minTimeout = pCommunicator->getMinTimeout();
    if (_minTimeout < 1)
    {
        _minTimeout = 1;
    }
}

ServantProxy::~ServantProxy()
{
    if (_objectProxy)
    {
        //set _objectProxy to NULL
        for (size_t i = 0; i < _objectProxyNum; i++)
        {
            _objectProxy[i] = NULL;
        }
        _objectProxy = NULL;
    }
}

string ServantProxy::tars_name() const
{
    if (_objectProxyNum >= 1 && (*_objectProxy != NULL))
    {
        return (*_objectProxy)->name();
    }
    return "NULL";
}

string ServantProxy::tars_full_name() const
{
    if (_objectProxyNum >= 1 && (*_objectProxy != NULL))
    {
        return (*_objectProxy)->name() + "#" + (*_objectProxy)->hash() + "@" + (*_objectProxy)->address();
    }
    return "NULL";
}


void ServantProxy::tars_reconnect(int second)
{
    if (_objectProxyNum >= 1 && (*_objectProxy != NULL))
    {
        (*_objectProxy)->reconnect(second);
    }
}

TC_Endpoint ServantProxy::tars_invoke_endpoint()
{
    ServantProxyThreadData *td = ServantProxyThreadData::getData();

    if (td)
    {
        return TC_Endpoint(td->_szHost);
    }
    return TC_Endpoint();
}

void ServantProxy::tars_set_proxy(ServantProxy::SERVANT_PROXY type, const TC_Endpoint &ep, const string &user, const string &pass)
{
    switch (type)
    {
        case PROXY_SOCK4:
            _proxyPointer.reset(new ProxySock4(ep));
            break;
        case PROXY_SOCK5:
            _proxyPointer.reset(new ProxySock5(ep, user, pass));
            break;
        case PROXY_HTTP:
            _proxyPointer.reset(new ProxyHttp(ep, user, pass));
            break;
    }
}

void ServantProxy::tars_timeout(int msecond)
{
    {
        TC_LockT<TC_ThreadMutex> lock(*this);
        //保护，超时时间不能小于_minTimeout毫秒
        _syncTimeout = (msecond < _minTimeout)?_minTimeout:msecond;
    }
}

int ServantProxy::tars_timeout() const
{
    return _syncTimeout;
}

int ServantProxy::tars_connect_timeout() const
{
    return _connTimeout;
}

void ServantProxy::tars_connect_timeout(int conTimeout)
{
    if(conTimeout < 100)
    {
        conTimeout = 100;
    }

    if(conTimeout > 5000)
    {
        conTimeout = 5000;
    }
    _connTimeout = conTimeout;
    for (size_t i = 0; i < _objectProxyNum; ++i)
    {
        (*(_objectProxy + i))->setConTimeout(conTimeout);
    }
}

void ServantProxy::tars_async_timeout(int msecond)
{
    {
        TC_LockT<TC_ThreadMutex> lock(*this);
        //保护，超时时间不能小于_minTimeout毫秒
        _asyncTimeout = (msecond < _minTimeout) ? _minTimeout : msecond;
    }
}

int ServantProxy::tars_async_timeout() const
{
    return _asyncTimeout;
}

void ServantProxy::tars_set_custom_callback(ServantProxy::custom_callback callback)
{
    _callback = callback;
}

void ServantProxy::tars_enable_callback_hash()
{
    _callbackHash = true;
}

void ServantProxy::tars_connection_serial(int connectionSerial)
{
    assert(!_rootPrx);
    _connectionSerial = connectionSerial;
}

int ServantProxy::tars_connection_serial() const
{
	if(_rootPrx) {
		return _rootPrx->tars_connection_serial();
	}

	return _connectionSerial;
}

void ServantProxy::tars_set_protocol(SERVANT_PROTOCOL protocol, int connectionSerial)
{
	ProxyProtocol proto;

	switch(protocol)
	{
		case PROTOCOL_HTTP1:
			proto.requestFunc   = ProxyProtocol::http1Request;
			proto.responseFunc  = ProxyProtocol::http1Response;

			if(connectionSerial <= 0)
				connectionSerial = DEFAULT_CONNECTION_SERIAL;
			break;
#if TARS_HTTP2
		case PROTOCOL_HTTP2:
			proto.requestFunc   = ProxyProtocol::http2Request;
			proto.responseFunc  = ProxyProtocol::http2Response;
            connectionSerial    = 0;
			break;
        case PROTOCOL_GRPC:
			proto.requestFunc   = ProxyProtocol::grpcRequest;
			proto.responseFunc  = ProxyProtocol::grpcResponse;
            connectionSerial    = 0;
			break;
#endif
		case PROTOCOL_TARS:
		default:
			proto.requestFunc   = ProxyProtocol::tarsRequest;
			proto.responseFunc  = ProxyProtocol::tarsResponse;
			break;
	}
	tars_set_protocol(proto, connectionSerial);
}

void ServantProxy::tars_set_protocol(const ProxyProtocol& protocol, int connectionSerial)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    for (size_t i = 0; i < _objectProxyNum; ++i)
    {
        (*(_objectProxy + i))->setProxyProtocol(protocol);
    }

    _connectionSerial = connectionSerial;
}

ProxyProtocol ServantProxy::tars_get_protocol()
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    return (*(_objectProxy + 0))->getProxyProtocol();
}

void ServantProxy::tars_set_sockopt(int level, int optname, const void * optval, SOCKET_LEN_TYPE optlen)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    for (size_t i = 0; i < _objectProxyNum; ++i)
    {
        (*(_objectProxy + i))->setSocketOpt(level, optname, optval, optlen);
    }
}

void ServantProxy::tars_set_check_timeout(const CheckTimeoutInfo& checkTimeoutInfo)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    for (size_t i = 0; i < _objectProxyNum; ++i)
    {
        (*(_objectProxy + i))->checkTimeoutInfo().minTimeoutInvoke     = checkTimeoutInfo.minTimeoutInvoke;
        (*(_objectProxy + i))->checkTimeoutInfo().checkTimeoutInterval = checkTimeoutInfo.checkTimeoutInterval;
        (*(_objectProxy + i))->checkTimeoutInfo().frequenceFailInvoke  = checkTimeoutInfo.frequenceFailInvoke;
        (*(_objectProxy + i))->checkTimeoutInfo().minFrequenceFailTime = checkTimeoutInfo.minFrequenceFailTime;
        (*(_objectProxy + i))->checkTimeoutInfo().radio                = checkTimeoutInfo.radio;
        (*(_objectProxy + i))->checkTimeoutInfo().tryTimeInterval      = checkTimeoutInfo.tryTimeInterval;
    }
}

CheckTimeoutInfo ServantProxy::tars_get_check_timeout()
{
    CheckTimeoutInfo checkTimeoutInfo;

    if (_objectProxyNum > 0)
    {
        checkTimeoutInfo.minTimeoutInvoke     = (*_objectProxy)->checkTimeoutInfo().minTimeoutInvoke;
        checkTimeoutInfo.checkTimeoutInterval = (*_objectProxy)->checkTimeoutInfo().checkTimeoutInterval;
        checkTimeoutInfo.frequenceFailInvoke  = (*_objectProxy)->checkTimeoutInfo().frequenceFailInvoke;
        checkTimeoutInfo.minFrequenceFailTime = (*_objectProxy)->checkTimeoutInfo().minFrequenceFailTime;
        checkTimeoutInfo.radio                = (*_objectProxy)->checkTimeoutInfo().radio;
        checkTimeoutInfo.tryTimeInterval      = (*_objectProxy)->checkTimeoutInfo().tryTimeInterval;
    }

    return checkTimeoutInfo;
}

void ServantProxy::tars_ping()
{
    map<string, string> m;

    map<string, string> s;

	TarsOutputStream<BufferWriterVector> os;

	tars_invoke(TARSNORMAL, "tars_ping", os, m, s);
}


void ServantProxy::tars_async_ping()
{
	map<string, string> m;
    map<string, string> s;

	TarsOutputStream<BufferWriterVector> os;

	tars_invoke_async(TARSONEWAY, "tars_ping", os, m, s, NULL);
}

ServantProxy* ServantProxy::tars_hash(int64_t key)
{
    ServantProxyThreadData * pSptd = ServantProxyThreadData::getData();

    assert(pSptd != NULL);

    pSptd->_hash     = true;
    pSptd->_hashCode = key;

    return this;
}

//ServantProxy* ServantProxy::tars_direct()
//{
//	ServantProxyThreadData *pSptd = ServantProxyThreadData::getData();
//
//	pSptd->_direct = true;
//	return this;
//}

ServantProxy* ServantProxy::tars_consistent_hash(int64_t key)
{
    ServantProxyThreadData *pSptd = ServantProxyThreadData::getData();

    assert(pSptd != NULL);

    pSptd->_hash     = true;
    pSptd->_conHash  = true;
    pSptd->_hashCode = key;

    return this;
}

void ServantProxy::tars_clear_hash()
{
}

ServantProxy* ServantProxy::tars_set_timeout(int msecond)
{
    ServantProxyThreadData * pSptd = ServantProxyThreadData::getData();
    assert(pSptd != NULL);

    pSptd->_hasTimeout = true;
    pSptd->_timeout    = msecond;

    return this;
}

uint32_t ServantProxy::tars_gen_requestid()
{
    uint32_t i = ++_id;
    if (i == 0)
    {
        i = ++_id;
    }

    return i;
}

void ServantProxy::tars_set_push_callback(const ServantProxyCallbackPtr & cb)
{
    for (size_t i = 0; i < _objectProxyNum; ++i)
    {
        (*(_objectProxy + i))->setPushCallbacks(cb);
    }
}


void ServantProxy::invoke(ReqMessage * msg, bool bCoroAsync)
{
    msg->proxy         = this;
    msg->response->iRet = TARSSERVERUNKNOWNERR;

    //线程私有数据
    ServantProxyThreadData *pSptd = ServantProxyThreadData::getData();
    assert(pSptd != NULL);

    msg->bHash     = pSptd->_hash;
    msg->bConHash  = pSptd->_conHash;
    msg->iHashCode = pSptd->_hashCode;

    //hash每次调用完成都要清掉，不用透传
    pSptd->_hash    = false;
    pSptd->_conHash = false;

    //染色需要透传
    msg->bDyeing    = pSptd->_dyeing;
    msg->sDyeingKey = pSptd->_dyeingKey;

    if (msg->bDyeing)
    {
        TLOGTARS("[ServantProxy::invoke, set dyeing, key=" << pSptd->_dyeingKey << endl);
    }

    msg->cookie       = pSptd->_cookie;

#ifdef TARS_OPENTRACKING
    msg->trackInfoMap = pSptd->_trackInfoMap;
#endif

    //设置超时时间
    msg->request.iTimeout = (ReqMessage::SYNC_CALL == msg->eType) ? _syncTimeout : _asyncTimeout;

    //判断是否针对接口级设置超时
    if (pSptd->_hasTimeout)
    {
        msg->request.iTimeout = (pSptd->_timeout > 0) ? pSptd->_timeout : msg->request.iTimeout;
        pSptd->_hasTimeout    = false;
    }

    ObjectProxy *pObjProxy = NULL;
    ReqInfoQueue *pReqQ    = NULL;

    //选择网络线程
    selectNetThreadInfo(pSptd, pObjProxy, pReqQ);

    //调用发起时间
    msg->iBeginTime   = TNOWMS;
    msg->pObjectProxy = pObjProxy;

    //如果是按set规则调用
    if (pObjProxy && pObjProxy->isInvokeBySet())
    {
        SET_MSG_TYPE(msg->request.iMessageType, TARSMESSAGETYPESETNAME);
        msg->request.status[ServantProxy::STATUS_SETNAME_VALUE] = pObjProxy->getInvokeSetName();

        TLOGTARS("[ServantProxy::invoke, " << msg->request.sServantName << ", invoke with set,"<<pObjProxy->getInvokeSetName()<<"]" << endl);
    }

    //同步调用 new 一个ReqMonitor
    assert(msg->pMonitor == NULL);
    if (msg->eType == ReqMessage::SYNC_CALL)
    {
        msg->bMonitorFin = false;

        if (pSptd->_sched)
        {
            msg->bCoroFlag = true;
            msg->sched     = pSptd->_sched;
            msg->iCoroId   = pSptd->_sched->getCoroutineId();
        }
        else
        {
            msg->pMonitor = new ReqMonitor;
        }
    }

    if (ReqMessage::ASYNC_CALL == msg->eType)
    {
        //是否是协程的并行请求
        if (bCoroAsync)
        {
            if (pSptd->_sched)
            {
                CoroParallelBasePtr coroPtr = msg->callback->getCoroParallelBasePtr();
                if (coroPtr)
                {
                    coroPtr->incReqCount();

                    msg->bCoroFlag = true;
                    msg->sched     = pSptd->_sched;
                    msg->iCoroId   = pSptd->_sched->getCoroutineId();
                }
                else
                {
                    TLOGERROR("[ServantProxy::invoke use coroutine's callback not set CoroParallelBasePtr]"<<endl);
                    delete msg;
                    msg = NULL;
                    throw TarsUseCoroException("use coroutine's callback not set CoroParallelBasePtr");
                }
            }
            else
            {
                TLOGERROR("[ServantProxy::invoke coroutine mode invoke not open]"<<endl);
                delete msg;
                msg = NULL;
                throw TarsUseCoroException("coroutine mode invoke not open");
            }
        }
    }

    //通知网络线程
    bool bEmpty = false;
    bool bSync  = (msg->eType == ReqMessage::SYNC_CALL);

    if (!pReqQ->push_back(msg, bEmpty))
    {
        TLOGERROR("[ServantProxy::invoke msgQueue push_back error num:" << pSptd->_netSeq << "]" << endl);

        delete msg;
        msg = NULL;

        pObjProxy->getCommunicatorEpoll()->notify(pSptd->_reqQNo, pReqQ);

        throw TarsClientQueueException("client queue full");
    }

    pObjProxy->getCommunicatorEpoll()->notify(pSptd->_reqQNo, pReqQ);

    //异步调用 另一个线程delele msg 如果是异步后面不能再用msg了

    if (bSync)
    {
        if (!msg->bCoroFlag)
        {
            if (!msg->bMonitorFin)
            {
                TC_ThreadLock::Lock lock(*(msg->pMonitor));

                //等待直到网络线程通知过来
                if (!msg->bMonitorFin)
                {
                    msg->pMonitor->wait();
                }
            }
        }
        else
        {
            msg->sched->yield(false);
        }

        //判断eStatus来判断状态
        assert(msg->eStatus != ReqMessage::REQ_REQ);

//        TLOGTARS("[ServantProxy::invoke line: " << __LINE__ << " status: " << msg->eStatus << ", ret: " <<msg->response->iRet << endl);

		if(msg->adapter) {
			pSptd->_szHost = msg->adapter->endpoint().desc();
		}

        if(msg->eStatus == ReqMessage::REQ_RSP && msg->response->iRet == TARSSERVERSUCCESS)
        {
            //成功
            return;
        }

        ostringstream os;
        if (msg->eStatus == ReqMessage::REQ_TIME)
        {
            //超时
            os << "[ServantProxy::invoke timeout:" << msg->request.iTimeout;
        }
        else
        {
            os << "[ServantProxy::invoke errno:" << msg->response->iRet << ",info:" << msg->response->sResultDesc;
        }

        os << ",servant:" << msg->pObjectProxy->name() << ",func:" << msg->request.sFuncName;

        if (msg->adapter)
            os << ",adapter:" << msg->adapter->endpoint().desc();

        os << ",reqid:" << msg->request.iRequestId << "]";

        if (msg->eStatus == ReqMessage::REQ_TIME)
        {
            //超时
            delete msg;
            msg = NULL;

            throw TarsSyncCallTimeoutException(os.str());
        }

        //异常调用
        int ret = msg->response->iRet;

        delete msg;
        msg = NULL;

        TarsException::throwException(ret, os.str());
    }
}

//////////////////////////////////////////////////////////////////
void ServantProxy::tars_invoke_async(char  cPacketType,
                                    const string &sFuncName,
                                    TarsOutputStream<BufferWriterVector> &buf,
                                    const map<string, string>& context,
                                    const map<string, string>& status,
                                    const ServantProxyCallbackPtr& callback,
                                    bool  bCoro)
{
    ReqMessage *msg = new ReqMessage();

    msg->init(callback?ReqMessage::ASYNC_CALL:ReqMessage::ONE_WAY);
    msg->callback = callback;

    msg->request.iVersion = TARSVERSION;
    msg->request.cPacketType = (callback ? cPacketType : TARSONEWAY);
	msg->request.sFuncName = sFuncName;
    msg->request.sServantName = (*_objectProxy)->name();

    buf.swap(msg->request.sBuffer);

    msg->request.context      = context;
    msg->request.status       = status;
    msg->request.iTimeout     = _asyncTimeout;
    
//    // 在RequestPacket中的context设置主调信息
//    if(_masterFlag)
//    {
//        msg->request.context.insert(std::make_pair(TARS_MASTER_KEY,ClientConfig::ModuleName)); //TARS_MASTER_KEY  clientConfig.ModuleName
//    }

    checkDye(msg->request);

    checkCookie(msg->request);
    servant_invoke(msg, bCoro);
}


//////////////////////////////////////////////////////////////////
void ServantProxy::tars_invoke_async(char  cPacketType,
                                    const string &sFuncName,
                                    const vector<char> &buf,
                                    const map<string, string>& context,
                                    const map<string, string>& status,
                                    const ServantProxyCallbackPtr& callback,
                                    bool  bCoro)
{
    ReqMessage * msg = new ReqMessage();

    msg->init(callback?ReqMessage::ASYNC_CALL:ReqMessage::ONE_WAY);
    msg->callback = callback;

    msg->request.iVersion = TARSVERSION;
    msg->request.cPacketType = (callback ? cPacketType : TARSONEWAY);
	msg->request.sFuncName = sFuncName;
    msg->request.sServantName = (*_objectProxy)->name();
    msg->request.sBuffer = buf;
    msg->request.context      = context;
    msg->request.status       = status;
    msg->request.iTimeout     = _asyncTimeout;
    
//    // 在RequestPacket中的context设置主调信息
//    if(_masterFlag)
//    {
//        msg->request.context.insert(std::make_pair(TARS_MASTER_KEY,ClientConfig::ModuleName)); //TARS_MASTER_KEY  clientConfig.ModuleName
//    }

    checkDye(msg->request);

    checkCookie(msg->request);
    servant_invoke(msg, bCoro);
}

shared_ptr<ResponsePacket> ServantProxy::tars_invoke(char  cPacketType,
                              const string& sFuncName,
                              const vector<char>& buf,
                              const map<string, string>& context,
                              const map<string, string>& status)
                            //   ResponsePacket& rsp)
{
    ReqMessage *msg = new ReqMessage();

    msg->init(ReqMessage::SYNC_CALL);

    msg->request.iVersion = TARSVERSION;
    msg->request.cPacketType = cPacketType;
	msg->request.sFuncName = sFuncName;
    msg->request.sServantName = (*_objectProxy)->name();

    msg->request.sBuffer = buf;
    msg->request.context      = context;
    msg->request.status       = status;
    msg->request.iTimeout     = _syncTimeout;

//    // 在RequestPacket中的context设置主调信息
//    if(_masterFlag)
//    {
//        msg->request.context.insert(std::make_pair(TARS_MASTER_KEY,ClientConfig::ModuleName));
//    }

    checkDye(msg->request);

    checkCookie(msg->request);
    servant_invoke(msg, false);

    shared_ptr<ResponsePacket> rsp = msg->response;
    // rsp = msg->response;

    delete msg;
    msg = NULL;

    return rsp;

}


shared_ptr<ResponsePacket> ServantProxy::tars_invoke(char  cPacketType,
                              const string& sFuncName,
                              TarsOutputStream<BufferWriterVector>& buf,
                              const map<string, string>& context,
                              const map<string, string>& status)
                            //   ResponsePacket& rsp)
{
    ReqMessage * msg = new ReqMessage();

    msg->init(ReqMessage::SYNC_CALL);

    msg->request.iVersion = TARSVERSION;
    msg->request.cPacketType = cPacketType;
	msg->request.sFuncName = sFuncName;
    msg->request.sServantName = (*_objectProxy)->name();

    buf.swap(msg->request.sBuffer);
    msg->request.context      = context;
    msg->request.status       = status;
    msg->request.iTimeout     = _syncTimeout;

//    // 在RequestPacket中的context设置主调信息
//    if(_masterFlag)
//    {
//        msg->request.context.insert(std::make_pair(TARS_MASTER_KEY,ClientConfig::ModuleName));
//    }

    checkDye(msg->request);

    checkCookie(msg->request);
    servant_invoke(msg, false);

    shared_ptr<ResponsePacket> rsp = msg->response;
    // rsp = msg->response;

    delete msg;
    msg = NULL;

    return rsp;

}
//////////////////////////////////////////////////////////////////////////////
//服务端是非tars协议，通过rpc_call调用
void ServantProxy::rpc_call(uint32_t iRequestId, 
                            const string& sFuncName,
                            const char* buff, 
                            uint32_t len,
                            ResponsePacket& rsp)
{
    ReqMessage *msg = new ReqMessage();

    msg->init(ReqMessage::SYNC_CALL);
    msg->bFromRpc             = true;
    msg->request.sServantName = (*_objectProxy)->name();
    msg->request.sFuncName    = sFuncName;
    msg->request.iRequestId   = iRequestId;

    msg->request.sBuffer.assign(buff, buff + len);

    servant_invoke(msg, false);

    rsp = *msg->response.get();

    delete msg;
    msg = NULL;
}

void ServantProxy::rpc_call_async(uint32_t iRequestId, 
                                  const string& sFuncName,
                                  const char* buff, 
                                  uint32_t len, 
                                  const ServantProxyCallbackPtr& callback, 
                                  bool  bCoro)
{
    ReqMessage *msg = new ReqMessage();

    msg->init(callback ? ReqMessage::ASYNC_CALL : ReqMessage::ONE_WAY);

    msg->bFromRpc             = true;
    msg->callback             = callback;
    msg->request.sServantName = (*_objectProxy)->name();
    msg->request.sFuncName    = sFuncName;

    msg->request.iRequestId = iRequestId;

    msg->request.sBuffer.assign(buff, buff + len);

    servant_invoke(msg, bCoro);
}

ServantPrx ServantProxy::getServantPrx(ReqMessage *msg)
{
    if (_servantId == 0)
    {
        std::lock_guard<std::mutex> m(_servantMutex);

        if (_servantId == 0 && _servantList.empty())
        {
            for (int i = 1; i < _connectionSerial; ++i)
            {
                string obj = tars_name() + "#" + TC_Common::tostr(i);
                if (!(*_objectProxy)->address().empty())
                {
                    obj += "@" + (*_objectProxy)->address();
                }

                ServantPrx prx = _communicator->stringToProxy<ServantPrx>(obj);
                prx->tars_set_protocol(tars_get_protocol());
                prx->tars_connect_timeout(tars_connect_timeout());
                prx->tars_timeout(tars_timeout());
                prx->tars_async_timeout(tars_async_timeout());

                prx->_rootPrx = this;

                _servantList.push_back(prx);
            }
        }
    }

    if (_connectionSerial != 0)
    {
        assert(int(_servantList.size() + 1) == _connectionSerial);
    }

    int id = _servantId++ % (_servantList.size() + 1);

    if (id == 0)
    {
        return this;
    }

    return _servantList[(id - 1)];
}

void ServantProxy::tars_update_endpoints(const set<EndpointInfo> &active, const set<EndpointInfo> &inactive)
{
    _communicator->notifyUpdateEndpoints(this, active, inactive);
}

void ServantProxy::onNotifyEndpoints(size_t netThreadSeq, const set<EndpointInfo> &active, const set<EndpointInfo> &inactive, bool fromInner)
{
    assert(netThreadSeq < _objectProxyNum);

    for (size_t i = 0; i < _servantList.size(); i++)
    {
        _servantList[i]->_objectProxy[netThreadSeq]->getEndpointManager()->updateEndpoints(active, inactive);
    }

    if (!fromInner)
    {
        _objectProxy[netThreadSeq]->getEndpointManager()->updateEndpoints(active, inactive);
    }
}

void ServantProxy::onSetInactive(const EndpointInfo &ep)
{
    if (!_rootPrx)
        return;

    for (size_t i = 0; i < _rootPrx->_servantList.size(); i++)
    {
        ServantPrx &prx = _rootPrx->_servantList[i];

        for (size_t i = 0; i < prx->_objectProxyNum; ++i)
        {
            prx->_objectProxy[i]->onSetInactive(ep);
        }
    }
}

int ServantProxy::servant_invoke(ReqMessage *msg, bool bCoroAsync)
{
    ServantPrx prx = getServantPrx(msg);

    if (msg->callback)
    {
        msg->callback->setServantPrx(prx);
    }

    prx->invoke(msg, bCoroAsync);

    return 0;
}

void ServantProxy::http_call(const string &funcName, shared_ptr<TC_HttpRequest> &request, shared_ptr<TC_HttpResponse> &response)
{
    ReqMessage *msg = new ReqMessage();

    msg->init(ReqMessage::SYNC_CALL);

    msg->bFromRpc = true;

    msg->request.sServantName = (*_objectProxy)->name();
    msg->request.sFuncName    = funcName;

    msg->request.sBuffer.resize(sizeof(shared_ptr<TC_HttpRequest>));

    msg->deconstructor = [msg] {
        shared_ptr<TC_HttpRequest> &data = *(shared_ptr<TC_HttpRequest> *)(msg->request.sBuffer.data());
        data.reset();

        if (!msg->response->sBuffer.empty())
        {
            shared_ptr<TC_HttpResponse> &rsp = *(shared_ptr<TC_HttpResponse> *)(msg->response->sBuffer.data());
            //主动reset一次
            rsp.reset();

            msg->response->sBuffer.clear();
        }
    };

    shared_ptr<TC_HttpRequest> &data = *(shared_ptr<TC_HttpRequest> *)(msg->request.sBuffer.data());

    data = request;

    servant_invoke(msg, false);

    response = *(shared_ptr<TC_HttpResponse> *)(msg->response->sBuffer.data());

    delete msg;
    msg = NULL;
}

void ServantProxy::http_call_async(const string &funcName, shared_ptr<TC_HttpRequest> &request, const HttpCallbackPtr &cb, bool bCoro)
{
    ReqMessage *msg = new ReqMessage();

    msg->init(ReqMessage::ASYNC_CALL);

    msg->bFromRpc = true;

    msg->request.sServantName = (*_objectProxy)->name();
    msg->request.sFuncName    = funcName;

    msg->request.sBuffer.resize(sizeof(shared_ptr<TC_HttpRequest>));

    msg->deconstructor = [msg] {
        shared_ptr<TC_HttpRequest> &data = *(shared_ptr<TC_HttpRequest> *)(msg->request.sBuffer.data());
        data.reset();

        if (!msg->response->sBuffer.empty())
        {
            shared_ptr<TC_HttpResponse> &rsp = *(shared_ptr<TC_HttpResponse> *)(msg->response->sBuffer.data());
            //主动reset一次
            rsp.reset();

            msg->response->sBuffer.clear();
        }
    };

    *(shared_ptr<TC_HttpRequest> *)(msg->request.sBuffer.data()) = request;

    ServantProxyCallbackPtr callback = new HttpServantProxyCallback(cb);

    msg->callback = callback;

    servant_invoke(msg, bCoro);
}

//选取一个网络线程对应的信息
void ServantProxy::selectNetThreadInfo(ServantProxyThreadData *pSptd, ObjectProxy *&pObjProxy, ReqInfoQueue *&pReqQ)
{
    //指针为空 就new一个
    if (!pSptd->_queueInit)
    {
        for (size_t i = 0; i < _objectProxyNum; ++i)
        {
            pSptd->_reqQueue[i] = new ReqInfoQueue(_objectProxy[0]->getCommunicatorEpoll()->getNoSendQueueLimit());
        }
        pSptd->_objectProxyNum = _objectProxyNum;
        pSptd->_objectProxyOwn = _objectProxyOwn;
        pSptd->_queueInit      = true;
    }

    if (_objectProxyNum == 1)
    {
        pObjProxy = *_objectProxy;
        pReqQ     = pSptd->_reqQueue[0];
    }
    else
    {
        if (pSptd->_netThreadSeq >= 0)
        {
            //网络线程发起的请求
            assert(pSptd->_netThreadSeq < static_cast<int>(_objectProxyNum));

            pObjProxy = *(_objectProxy + pSptd->_netThreadSeq);
            pReqQ     = pSptd->_reqQueue[pSptd->_netThreadSeq];
        }
        else
        {
            //用线程的私有数据来保存选到的seq
            pObjProxy = *(_objectProxy + pSptd->_netSeq);
            pReqQ     = pSptd->_reqQueue[pSptd->_netSeq];
            pSptd->_netSeq++;

            if (pSptd->_netSeq == _objectProxyNum)
                pSptd->_netSeq = 0;
        }
    }
}

void ServantProxy::checkDye(RequestPacket& req)
{
    //线程私有数据
    ServantProxyThreadData *pSptd = ServantProxyThreadData::getData();
    assert(pSptd != NULL);
    if (pSptd && pSptd->_dyeing)
    {
        SET_MSG_TYPE(req.iMessageType, TARSMESSAGETYPEDYED);

        req.status[ServantProxy::STATUS_DYED_KEY] = pSptd->_dyeingKey;
    }
}

void ServantProxy::checkCookie(RequestPacket& req)
{
    //线程私有数据
    ServantProxyThreadData *pSptd = ServantProxyThreadData::getData();
    assert(pSptd != NULL);

    std::for_each(pSptd->_cookie.begin(), pSptd->_cookie.end(), [&](map<string, string>::value_type &p) {
        req.status.insert(make_pair(p.first, p.second));
    });
}


void ServantProxy::tars_endpoints(vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint)
{
    _endpointInfo->getEndpoint(activeEndPoint, inactiveEndPoint);
}

void ServantProxy::tars_endpointsAll(vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint)
{
    _endpointInfo->getEndpointByAll(activeEndPoint, inactiveEndPoint);
}

void ServantProxy::tars_endpointsBySet(const string & sName,vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint)
{
    _endpointInfo->getEndpointBySet(sName, activeEndPoint, inactiveEndPoint);
}

void ServantProxy::tars_endpointsByStation(const string & sName,vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint)
{
    _endpointInfo->getEndpointByStation(sName, activeEndPoint, inactiveEndPoint);
}

vector<TC_Endpoint> ServantProxy::tars_endpoints(const std::string & sStation)
{
    vector<TC_Endpoint> activeEndPoint;
    vector<TC_Endpoint> inactiveEndPoint;
    _endpointInfo->getTCEndpoint(activeEndPoint, inactiveEndPoint);
    return activeEndPoint;
}

void ServantProxy::tars_endpoints(const std::string & sStation, vector<TC_Endpoint> & vecActive, vector<TC_Endpoint> & vecInactive)
{
    _endpointInfo->getTCEndpointByStation(sStation, vecActive, vecInactive);
}

vector<TC_Endpoint> ServantProxy::getEndpoint()
{
    vector<TC_Endpoint> activeEndPoint;
    vector<TC_Endpoint> inactiveEndPoint;
    _endpointInfo->getTCEndpoint(activeEndPoint, inactiveEndPoint);
    return activeEndPoint;
}

vector<TC_Endpoint> ServantProxy::getEndpoint4All()
{
    vector<TC_Endpoint> activeEndPoint;
    vector<TC_Endpoint> inactiveEndPoint;
    _endpointInfo->getTCEndpointByAll(activeEndPoint, inactiveEndPoint);
    return activeEndPoint;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
}
