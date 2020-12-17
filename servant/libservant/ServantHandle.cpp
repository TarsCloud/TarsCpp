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

#include "util/tc_thread_pool.h"
#include "util/tc_timeprovider.h"
#include "servant/ServantHandle.h"
#include "servant/Application.h"
#include "servant/ServantHelper.h"
#include "servant/AppProtocol.h"
#include "servant/BaseF.h"
#include "servant/KeepAliveNodeF.h"
#include "servant/Cookie.h"
#include "servant/Application.h"
#ifdef TARS_OPENTRACKING
#include "servant/text_map_carrier.h"
#endif

namespace tars
{

/////////////////////////////////////////////////////////////////////////
//
ServantHandle::ServantHandle(Application *application)
: _application(application),_coroSched(NULL)
{
    
}

ServantHandle::~ServantHandle()
{
    auto it = _servants.begin();

    while(it != _servants.end())
    {
        try
        {
            it->second->destroy();
        }
        catch(exception &ex)
        {
            TLOGERROR("[ServantHandle::destroy error:" << ex.what() << "]" << endl);
        }
        catch(...)
        {
            TLOGERROR("[ServantHandle::destroy unknown exception error]" << endl);
        }
        ++it;
    }

    if(_coroSched != NULL)
    {
        delete _coroSched;
        _coroSched = NULL;
    }
}

void ServantHandle::run()
{
	try
	{
		initialize();

	    if (!ServerConfig::OpenCoroutine)
	    {
	        handleImp();
	    }
	    else
	    {        
	        //by goodenpei, 判断是否启用顺序模式
	        _bindAdapter->initThreadRecvQueue(getHandleIndex());
        
	        size_t iThreadNum = getEpollServer()->getLogicThreadNum();

	        size_t iCoroutineNum = (ServerConfig::CoroutineMemSize > ServerConfig::CoroutineStackSize) ? (ServerConfig::CoroutineMemSize / (ServerConfig::CoroutineStackSize * iThreadNum)) : 1;
	        if (iCoroutineNum < 1) iCoroutineNum = 1;

			startHandle();

			_coroSched = new CoroutineScheduler();
			_coroSched->init(iCoroutineNum, ServerConfig::CoroutineStackSize);
			_coroSched->setHandle(this);

			_coroSched->createCoroutine(std::bind(&ServantHandle::handleRequest, this));

			ServantProxyThreadData *pSptd = ServantProxyThreadData::getData();

			assert(pSptd != NULL);

			pSptd->_sched = _coroSched;

			while (!getEpollServer()->isTerminate()) 
			{
				_coroSched->tars_run();
			}

			_coroSched->terminate();

			_coroSched->destroy();

			stopHandle();
		}
	}
	catch(exception &ex)
	{
		TLOGERROR("[ServantHandle::run exception error:" << ex.what() << "]" << endl);
		cerr << "ServantHandle::run exception error:" << ex.what() << endl;
	}
	catch(...)
	{
		TLOGERROR("[ServantHandle::run unknown exception error]" << endl);
		cerr << "ServantHandle::run unknown exception error]" << endl;
	}
}

void ServantHandle::handleRequest()
{
    bool bYield = false;
    while (!getEpollServer()->isTerminate())
    {
    	wait();

        //上报心跳
        heartbeat();

        //为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
        handleAsyncResponse();
        handleCustomMessage(true);

        bYield = false;

	    shared_ptr<TC_EpollServer::RecvContext> data;
        try
        {
            bool bFlag = true;
            int	iLoop = 100;
            while (bFlag && iLoop > 0)
            {
                --iLoop;
                if ((_coroSched->getFreeSize() > 0) && popRecvQueue(data))
                {
                    bYield = true;

                    //上报心跳
                    heartbeat();

                    //为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
                    handleAsyncResponse();

                    //数据已超载 overload
                    if (data->isOverload())
                    {
                        handleOverload(data);
                    }
                    //关闭连接的通知消息
                    else if (data->isClosed())
                    {
                        handleClose(data);
                    }
                    //数据在队列中已经超时了
                    else if ((TNOWMS - data->recvTimeStamp()) > (int64_t)_bindAdapter->getQueueTimeout())
                    {
                        handleTimeout(data);
                    }
                    else
                    {
                        uint32_t iRet = _coroSched->createCoroutine(std::bind(&ServantHandle::handleRecvData, this, data));
                        if (iRet == 0)
                        {
                            handleOverload(data);
                        }
                    }
                    handleCustomMessage(false);
                }
                else
                {
                    //_coroSched->yield();
                    bFlag = false;
                    bYield = false;
                }
            }

            if (iLoop == 0) bYield = false;
        }
        catch (exception& ex)
        {
            if (data)
            {
                close(data);
            }

            getEpollServer()->error("[Handle::handleImp] error:" + string(ex.what()));
        }
        catch (...)
        {
            if (data)
            {
                close(data);
            }

            getEpollServer()->error("[Handle::handleImp] unknown error");
        }
        if (!bYield)
        {
            _coroSched->yield();
        }
    }
}

void ServantHandle::handleRecvData(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    try
    {
	    CurrentPtr current = createCurrent(data);

	    if (!current)
        {
            return;
        }

        if (current->getBindAdapter()->isTarsProtocol())
        {
            handleTarsProtocol(current);
        }
        else
        {
            handleNoTarsProtocol(current);
        }
    }
    catch(exception &ex)
    {
        TLOGERROR("[ServantHandle::handleRecvData exception:" << ex.what() << "]" << endl);
    }
    catch(...)
    {
        TLOGERROR("[ServantHandle::handleRecvData unknown exception error]" << endl);
    }
}

void ServantHandle::handleAsyncResponse()
{
    ReqMessagePtr resp;

    auto it = _servants.begin();

    while (it != _servants.end())
    {
        while (it->second->getResponseQueue().pop_front(resp))
        {
            try
            {
                if (resp->response->iRet == TARSSERVERSUCCESS)
                {
                    it->second->doResponse(resp);
                }
                else if (resp->pObjectProxy == NULL)
                {
                    it->second->doResponseNoRequest(resp);
                }
                else
                {
                    it->second->doResponseException(resp);
                }
            }
            catch (exception& e)
            {
                TLOGERROR("[ServantHandle::doResponse ex:" << e.what() << "]" << endl);
            }
            catch (...)
            {
                TLOGERROR("[ServantHandle::doResponse error]" << endl);
            }
        }

        //业务处理附加的自有消息
        try
        {
            it->second->doCustomMessage(false);
            it->second->doCustomMessage();
        }
        catch (exception& e)
        {
            TLOGERROR("[ServantHandle::doCustemMessage ex:" << e.what() << "]" << endl);
        }
        catch (...)
        {
            TLOGERROR("[ServantHandle::doCustemMessage ex.]" << endl);
        }

        ++it;
    }
}

void ServantHandle::handleCustomMessage(bool bExpectIdle)
{
    for (auto it = _servants.begin(); it != _servants.end(); it++)
    {
        //业务处理附加的自有消息
        try
        {
            it->second->doCustomMessage(bExpectIdle);
            it->second->doCustomMessage();
        }
        catch (exception& e)
        {
            TLOGERROR("[ServantHandle::doCustemMessage ex:" << e.what() << "]" << endl);
        }
        catch (...)
        {
            TLOGERROR("[ServantHandle::doCustemMessage ex.]" << endl);
        }
    }
}

bool ServantHandle::allFilterIsEmpty()
{
    auto it = _servants.begin();

    while (it != _servants.end())
    {
        if (!it->second->getResponseQueue().empty())
        {
            return false;
        }
        ++it;
    }
    return true;
}

void ServantHandle::initialize()
{
    ServantPtr servant = _application->getServantHelper()->create(_bindAdapter->getName());

    if (servant)
    {
        _servants[servant->getName()] = servant;
    }
    else
    {
        TLOGERROR("[ServantHandle initialize createServant ret null, for adapter `" +_bindAdapter->getName() + "`]" << endl);
	    cerr << "ServantHandle initialize createServant ret null, for adapter `" +_bindAdapter->getName() + "`]" << endl;

	    RemoteNotify::getInstance()->report("initialize createServant error: no adapter:" + _bindAdapter->getName());

	    TC_Common::msleep(100);

	    exit(-1);
    }

    auto it = _servants.begin();

    if(it == _servants.end())
    {
        TLOGERROR("[initialize error: no servant exists]" << endl);

        RemoteNotify::getInstance()->report("initialize error: no servant exists.");

        TC_Common::msleep(100);

        exit(-1);
    }

    while(it != _servants.end())
    {
        try
        {
            it->second->setHandle(this);

            it->second->initialize();

            TLOGTARS("[" << it->second->getName() << " initialize]" << endl);
        }
        catch(exception &ex)
        {
            TLOGERROR("[initialize error:" << ex.what() << "]" << endl);

            RemoteNotify::getInstance()->report("initialize error:" + string(ex.what()));

	        TC_Common::msleep(100);

	        exit(-1);
        }
        catch(...)
        {
            TLOGERROR("[initialize unknown exception error]" << endl);

            RemoteNotify::getInstance()->report("initialize unknown exception error");

	        TC_Common::msleep(100);

	        exit(-1);
        }
        ++it;
    }
}

void ServantHandle::heartbeat()
{
    time_t fcur = TNOW;

    if (abs(fcur - _bindAdapter->getHeartBeatTime()) > HEART_BEAT_INTERVAL)
    {
        _bindAdapter->setHeartBeatTime(fcur);

        TARS_KEEPALIVE(_bindAdapter->getName());

	    //上报连接数 比率
        if (_bindAdapter->_pReportConRate)
        {
            _bindAdapter->_pReportConRate->report((int)(_bindAdapter->getNowConnection() * 1000 / _bindAdapter->getMaxConns()));
        }

        //有队列, 且队列长度>0才上报
        if (_bindAdapter->_pReportQueue)
        {
            _bindAdapter->_pReportQueue->report((int)_bindAdapter->getRecvBufferSize());
        }
    }
}

CurrentPtr ServantHandle::createCurrent(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    CurrentPtr current = new Current(this);

    try
    {
        current->initialize(data);
    }
    catch (TarsDecodeException &ex)
    {
        TLOGERROR("[ServantHandle::handle request protocol decode error:" << ex.what() << "]" << endl);
        close(data);
        return NULL;
    }

    //只有TARS协议才处理
    if(current->getBindAdapter()->isTarsProtocol())
    {
        int64_t now = TNOWMS;

        //数据在队列中的时间超过了客户端等待的时间(TARS协议)
        if (current->_request.iTimeout > 0 && (now - data->recvTimeStamp()) > current->_request.iTimeout)
        {
            //上报超时数目
            if(data->adapter()->_pReportTimeoutNum)
                data->adapter()->_pReportTimeoutNum->report(1);

            TLOGERROR("[ServantHandle::handle queue timeout:"
                         << current->_request.sServantName << ", func:"
                         << current->_request.sFuncName << ", recv time:"
                         << data->recvTimeStamp()  << ", queue timeout:"
                         << data->adapter()->getQueueTimeout() << ", timeout:"
                         << current->_request.iTimeout << ", now:"
                         << now << ", ip:" << data->ip() << ", port:" << data->port() << "]" << endl);

            current->sendResponse(TARSSERVERQUEUETIMEOUT);

            return NULL;
        }
    }

    return current;
}

CurrentPtr ServantHandle::createCloseCurrent(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    CurrentPtr current = new Current(this);

    current->initializeClose(data);
    current->setReportStat(false);
    current->setCloseType(data->closeType());
    return current;
}

void ServantHandle::handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TLOGTARS("[ServantHandle::handleClose,adapter:" << data->adapter()->getName() << ",peer:" << data->ip() << ":" << data->port() << "]"<< endl);

    CurrentPtr current = createCloseCurrent(data);

    auto sit = _servants.find(current->getServantName());

    if (sit == _servants.end())
    {
        TLOGERROR("[ServantHandle::handleClose,adapter:" << data->adapter()->getName()
                << ",peer:" << data->ip() << ":" << data->port()<<", " << current->getServantName() << " not found]" << endl);

        return;
    }

    try
    {
        //业务逻辑处理
        sit->second->doClose(current);
    }
    catch(exception &ex)
    {
        TLOGERROR("[ServantHandle::handleClose " << ex.what() << "]" << endl);

        return;
    }
    catch(...)
    {
        TLOGERROR("[ServantHandle::handleClose unknown error]" << endl);

        return;
    }
}

void ServantHandle::handleTimeout(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    CurrentPtr current = createCurrent(data);

    if (!current) return;

    //上报超时数目
    if(data->adapter()->_pReportTimeoutNum)
        data->adapter()->_pReportTimeoutNum->report(1);

    TLOGERROR("[ServantHandle::handleTimeout adapter '"
                 << data->adapter()->getName()
                 << "', recvtime:" << data->recvTimeStamp() << "|"
                 << ", timeout:" << data->adapter()->getQueueTimeout()
                 << ", id:" << current->getRequestId() << "]" << endl);

    if (current->getBindAdapter()->isTarsProtocol())
    {
        current->sendResponse(TARSSERVERQUEUETIMEOUT);
    }
}

void ServantHandle::handleOverload(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    CurrentPtr current = createCurrent(data);

    if (!current) return;

    TLOGERROR("[ServantHandle::handleOverload adapter '"
                 << data->adapter()->getName()
                 << "',overload:-1,queue capacity:"
                 << data->adapter()->getQueueCapacity()
                 << ",id:" << current->getRequestId() << "]" << endl);

    if (current->getBindAdapter()->isTarsProtocol())
    {
        current->sendResponse(TARSSERVEROVERLOAD);
    }
}

void ServantHandle::handle(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    CurrentPtr current = createCurrent(data);

	if (!current) return;

    if (current->getBindAdapter()->isTarsProtocol())
    {
        handleTarsProtocol(current);
    }
    else
    {
        handleNoTarsProtocol(current);
    }
}


#ifdef TARS_OPENTRACKING
void ServantHandle::processTracking(const TarsCurrentPtr &current)
{
    if(!(Application::getCommunicator()->_traceManager))
    {
        return;
    }
    ServantProxyThreadData * sptd = ServantProxyThreadData::getData();
    assert(sptd);

    if(!sptd)
    {
        return;
    }

    //提取packet中的span信息，更新为被调的span信息后设置到sptd->_trackInfoMap;
    sptd->_trackInfoMap.clear();
    
    if (IS_MSG_TYPE(current->getMessageType(), tars::TARSMESSAGETYPETRACK))
    {
        map<string, string>::const_iterator trackinfoIter = current->getRequestStatus().find(ServantProxy::STATUS_TRACK_KEY);
        TLOGTARS("[TARS] servant got a tracking request, message_type set" << current->getMessageType() << endl);
        if (trackinfoIter != current->getRequestStatus().end())
        {
            TLOGTARS("[TARS] servant got a tracking request, tracking key:" << trackinfoIter->second << endl);
            string context = trackinfoIter->second;
            char szBuffer[context.size() + 1];
            memset(szBuffer, 0x00, context.size() + 1);
            memcpy(szBuffer, context.c_str(), context.size());
            
            std::unordered_map<std::string, std::string> text_map;
            write_span_context(text_map, szBuffer);

            TextMapCarrier carrier(text_map);
            auto tracer = Application::getCommunicator()->_traceManager->_tracer;
            auto span_context_maybe = tracer->Extract(carrier);
            if(!span_context_maybe)
            {
                //error
                TLOGERROR("[TARS] servant got a tracking request, but extract the span context fail");
                return ;
            }

            string funcName = current->getFuncName();
            auto child_span = tracer->StartSpan(funcName, {opentracing::ChildOf(span_context_maybe->get())});
            
            //text_map.clear();
            auto err = tracer->Inject(child_span->context(), carrier);
            assert(err);

            sptd->_trackInfoMap = text_map;

            _spanMap[current->getRequestId()].reset(child_span.release());

            return ;

        }
    }

    return ;

}


void ServantHandle::finishTracking(int ret, const TarsCurrentPtr &current)
{
    int requestId = current->getRequestId();
    
    if(_spanMap.find(requestId) != _spanMap.end())
    {
        auto spanIter = _spanMap.find(requestId);
        spanIter->second->SetTag("Retcode", ret);
        spanIter->second->Finish();

        _spanMap.erase(requestId);
    }
}

#endif

bool ServantHandle::processDye(const CurrentPtr &current, string& dyeingKey)
{
    //当前线程的线程数据
    ServantProxyThreadData* sptd = ServantProxyThreadData::getData();

    if (sptd)
    {
        sptd->_dyeingKey = "";
    }

    //当前请求已经被染色, 需要打印染色日志
    map<string, string>::const_iterator dyeingIt = current->getRequestStatus().find(ServantProxy::STATUS_DYED_KEY);

    if (IS_MSG_TYPE(current->getMessageType(), tars::TARSMESSAGETYPEDYED))
    {
        TLOGTARS("[servant got a dyeing request, message_type set: " << current->getMessageType() << "]" << endl);

        if (dyeingIt != current->getRequestStatus().end())
        {
            TLOGTARS("[servant got a dyeing request, dyeing key: " << dyeingIt->second << "]" << endl);

            dyeingKey = dyeingIt->second;
        }
        return true;
    }

	//servant已经被染色, 开启染色日志
	if (_application->getServantHelper()->isDyeing())
	{
		map<string, string>::const_iterator dyeingKeyIt = current->getRequestStatus().find(ServantProxy::STATUS_GRID_KEY);

		if (dyeingKeyIt != current->getRequestStatus().end() &&
			_application->getServantHelper()->isDyeingReq(dyeingKeyIt->second, current->getServantName(), current->getFuncName()))
		{
			TLOGTARS("[TARS] dyeing servant got a dyeing req, key:" << dyeingKeyIt->second << endl);

			dyeingKey = dyeingKeyIt->second;

			return true;
		}
	}

    return false;
}


bool ServantHandle::processCookie(const CurrentPtr &current, map<string, string> &cookie)
{
	const static string STATUS = "STATUS_";

	std::for_each(current->getRequestStatus().begin(), current->getRequestStatus().end(),[&](const map<string, string>::value_type& p){
		if(p.first.size() > STATUS.size() && TC_Port::strncasecmp(p.first.c_str(), STATUS.c_str(), STATUS.size()) == 0) {
			return;
		}
		cookie.insert(make_pair(p.first, p.second));
	});

	return !cookie.empty();
}

bool ServantHandle::checkValidSetInvoke(const CurrentPtr &current)
{
    /*是否允许检查合法性*/
    if (ServerConfig::IsCheckSet == 0)
    {
        //不检查
        return true;
    }

    bool isSetInvoke = IS_MSG_TYPE(current->getMessageType(), tars::TARSMESSAGETYPESETNAME);
    //客户端按set规则调用且服务端启用set
    if (isSetInvoke && ClientConfig::SetOpen)
    {
        /**
         * 合法性规则:
         * 1 客户端set名称与服务端set在同一分组,eg, test.s.1 <-> test.s.1
         * 2 客户端set名称与服务端set在同一地区,eg, test.s.* <-> test.s.1 | test.s.2 | test.s.*
         * 3 客户端set名称与服务端set属于不同名称,eg,test1.s.1 <->test2.n.2
         * 4 1,2,3条件都不满足，则认为该调用不合法
         */
        map<string, string>::const_iterator setIt = current->getRequestStatus().find(ServantProxy::STATUS_SETNAME_VALUE);
        string sSetName("");

        if (setIt != current->getRequestStatus().end())
        {
            TLOGTARS("[servant got a setname request, setname key:" << setIt->second << "]" << endl);

            sSetName = setIt->second;

            if (ClientConfig::SetDivision == sSetName)
            {
                return true;
            }
            else
            {
                //属于同一地区是也属于合法调用
                string setArea1 = ClientConfig::SetDivision.substr(0,ClientConfig::SetDivision.find_last_of("."));
                string setArea2 = sSetName.substr(0,sSetName.find_last_of("."));
                if (setArea1 == setArea2)
                {
                    return true;
                }
                else if (ClientConfig::SetDivision.substr(0,ClientConfig::SetDivision.find_first_of(".")) !=
                         sSetName.substr(0,sSetName.find_first_of(".")))
                {
                    //属于不同的set之间调用也属于合法
                    return true;
                }
                else
                {
                    TLOGERROR("[ServantHandle::checkValidSetInvoke|"
                            << current->getIp() << "|"
                            << current->getMessageType() << "|"
                            << current->getServantName() << "|"
                            << current->getFuncName() << "|client:"
                            << ClientConfig::SetDivision << "|server:"
                            << sSetName << "]" << endl);
                    current->sendResponse(TARSINVOKEBYINVALIDESET);
                    return false;
                }
            }
        }
        else
        {
            TLOGERROR("[ServantHandle::checkValidSetInvoke|"
                            << current->getIp() << "|"
                            << current->getMessageType() << "|"
                            << current->getServantName() << "|"
                            << current->getFuncName() << "|client:"
                            << ClientConfig::SetDivision << "|server:"
                            << sSetName << "]" << endl);
            current->sendResponse(TARSINVOKEBYINVALIDESET);
            return false;
        }
    }

    //没有按set规则调用
    return true;
}

void ServantHandle::handleTarsProtocol(const CurrentPtr &current)
{
    TLOGTARS("[ServantHandle::handleTarsProtocol current:"
                << current->getIp() << "|"
                << current->getPort() << "|"
                << current->getMessageType() << "|"
                << current->getServantName() << "|"
                << current->getFuncName() << "|"
                << current->getRequestId() << "|"
                << TC_Common::tostr(current->getRequestStatus()) << "]"<<endl);

    //检查set调用合法性
    if(!checkValidSetInvoke(current))
    {
        return;
    }

    //处理染色消息
    string dyeingKey = "";
    TarsDyeingSwitch dyeSwitch;
    if (processDye(current, dyeingKey))
    {
        dyeSwitch.enableDyeing(dyeingKey);
    }

    //处理cookie
    map<string, string> cookie;
    CookieOp cookieOp;
    if (processCookie(current, cookie))
    {
        cookieOp.setCookie(cookie);
        current->setCookie(cookie);
    }

#ifdef TARS_OPENTRACKING
    //处理tracking信息
    processTracking(current);
#endif
    auto sit = _servants.find(current->getServantName());

    if (sit == _servants.end())
    {
        current->sendResponse(TARSSERVERNOSERVANTERR);
#ifdef TARS_OPENTRACKING
        finishTracking(TARSSERVERNOSERVANTERR, current);
#endif
        return;
    }

    int ret = TARSSERVERUNKNOWNERR;

    string sResultDesc = "";

	ResponsePacket response;
//    vector<char> buffer;
    try
    {
        //业务逻辑处理
        ret = sit->second->dispatch(current, response.sBuffer);
    }
    catch(TarsDecodeException &ex)
    {
        TLOGERROR("[ServantHandle::handleTarsProtocol " << ex.what() << "]" << endl);

        ret = TARSSERVERDECODEERR;

        sResultDesc = ex.what();
    }
    catch(TarsEncodeException &ex)
    {
        TLOGERROR("[ServantHandle::handleTarsProtocol " << ex.what() << "]" << endl);

        ret = TARSSERVERENCODEERR;

        sResultDesc = ex.what();
    }
    catch(exception &ex)
    {
        TLOGERROR("[ServantHandle::handleTarsProtocol " << ex.what() << "]" << endl);

        ret = TARSSERVERUNKNOWNERR;

        sResultDesc = ex.what();
    }
    catch(...)
    {
        TLOGERROR("[ServantHandle::handleTarsProtocol unknown error]" << endl);

        ret = TARSSERVERUNKNOWNERR;

        sResultDesc = "handleTarsProtocol unknown exception error";
    }

    //单向调用或者业务不需要同步返回
    if (current->isResponse())
    {
        current->sendResponse(ret, response, Current::TARS_STATUS(), sResultDesc);
    }
#ifdef TARS_OPENTRACKING
    finishTracking(ret, current);
#endif
}

void ServantHandle::handleNoTarsProtocol(const TarsCurrentPtr &current)
{
    TLOGTARS("[ServantHandle::handleNoTarsProtocol current:"
        << current->getIp() << "|"
        << current->getPort() << "|"
        << current->getServantName() << "]" << endl);

	auto sit = _servants.find(current->getServantName());

	assert(sit != _servants.end());

	vector<char> buffer;

    try
    {
        //业务逻辑处理
        sit->second->dispatch(current, buffer);
    }
    catch(exception &ex)
    {
        TLOGERROR("[ServantHandle::handleNoTarsProtocol " << ex.what() << "]" << endl);
    }
    catch(...)
    {
        TLOGERROR("[ServantHandle::handleNoTarsProtocol unknown error]" << endl);
    }

	if (current->isResponse() && !buffer.empty())
	{
		current->sendResponse((const char *)(buffer.data()), (uint32_t)buffer.size());
	}
}

////////////////////////////////////////////////////////////////////////////
}
