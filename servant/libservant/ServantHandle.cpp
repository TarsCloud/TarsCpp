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
#include "servant/TarsNodeF.h"
#ifdef _USE_OPENTRACKING
#include "servant/text_map_carrier.h"
#endif

namespace tars
{

/////////////////////////////////////////////////////////////////////////
//
ServantHandle::ServantHandle()
: _coroSched(NULL)
{
    
}

ServantHandle::~ServantHandle()
{
    map<string, ServantPtr>::iterator it = _servants.begin();

    while(it != _servants.end())
    {
        try
        {
            it->second->destroy();
        }
        catch(exception &ex)
        {
            TLOGERROR("[TARS]ServantHandle::destroy error:" << ex.what() << endl);
        }
        catch(...)
        {
            TLOGERROR("[TARS]ServantHandle::destroy unknown exception error" << endl);
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
    initialize();

    if(!ServerConfig::OpenCoroutine)
    {
        handleImp();
    }
    else
    {
        unsigned int iThreadNum = getEpollServer()->getLogicThreadNum();

        size_t iCoroutineNum = (ServerConfig::CoroutineMemSize > ServerConfig::CoroutineStackSize) ? (ServerConfig::CoroutineMemSize / (ServerConfig::CoroutineStackSize * iThreadNum) ) : 1;
        if(iCoroutineNum < 1)
            iCoroutineNum = 1;

        startHandle();

        _coroSched = new CoroutineScheduler();
        _coroSched->init(iCoroutineNum, ServerConfig::CoroutineStackSize);
        _coroSched->setHandle(this);

        _coroSched->createCoroutine(std::bind(&ServantHandle::handleRequest, this));

        ServantProxyThreadData * pSptd = ServantProxyThreadData::getData();

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

// void ServantHandle::handleRequest()
// {
//     bool bYield = false;
//     while (!getEpollServer()->isTerminate())
//     {
//         bool bServerReqEmpty = false;

//         {
//             TC_ThreadLock::Lock lock(_handleGroup->monitor);

//             if (allAdapterIsEmpty() && allFilterIsEmpty())
//             {
//                 if(_coroSched->getResponseCoroSize() > 0)
//                 {
//                     bServerReqEmpty = true;
//                 }
//                 else
//                 {
//                     _handleGroup->monitor.timedWait(3000);
//                 }
//             }
//         }

//         //上报心跳
//         heartbeat();

//         //为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
//         handleAsyncResponse();

//         handleCustomMessage(true);

//         if(bServerReqEmpty)
//         {
//             _coroSched->yield();

//             continue;
//         }
        
//         bYield = false;

//         TC_EpollServer::tagRecvData* recv = NULL;

//         map<string, TC_EpollServer::BindAdapterPtr>& adapters = _handleGroup->adapters;

//         for (map<string, TC_EpollServer::BindAdapterPtr>::iterator it = adapters.begin(); it != adapters.end(); ++it)
//         {
//             TC_EpollServer::BindAdapterPtr& adapter = it->second;

//             try
//             {
//                 bool bFlag = true;
//                 int    iLoop = 100;
//                 while(bFlag && iLoop > 0)
//                 {
//                     --iLoop;

//                     if(adapter->waitForRecvQueue(recv, 0))
//                     {
//                         bYield = true;

//                         //上报心跳
//                         heartbeat();

//                         //为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
//                            handleAsyncResponse();

//                         TC_EpollServer::tagRecvData& stRecvData = *recv;

//                         int64_t now = TNOWMS;

//                         stRecvData.adapter = adapter;

//                         //数据已超载 overload
//                         if (stRecvData.isOverload)
//                         {
//                             handleOverload(stRecvData);
//                             delete recv;
//                             recv = NULL;
//                         }
//                         //关闭连接的通知消息
//                         else if (stRecvData.isClosed)
//                         {
//                             handleClose(stRecvData);
//                             delete recv;
//                             recv = NULL;
//                         }
//                         //数据在队列中已经超时了
//                         else if ( (now - stRecvData.recvTimeStamp) > (int64_t)adapter->getQueueTimeout())
//                         {
//                             handleTimeout(stRecvData);
//                             delete recv;
//                             recv = NULL;
//                         }
//                         else
//                         {
//                             uint32_t iRet = _coroSched->createCoroutine(std::bind(&ServantHandle::handleRecvData, this, recv));
//                             if(iRet == 0)
//                             {
//                                 handleOverload(stRecvData);
//                                 delete recv;
//                                 recv = NULL;
//                             }
//                         }
//                         handleCustomMessage(false);
//                     }
//                     else
//                     {
//                         bFlag = false;
//                         bYield = false;
//                     }
//                 }

//                 if(iLoop == 0)
//                     bYield = false;
//             }
//             catch (exception &ex)
//             {
//                 if(recv)
//                 {
//                     close(recv->uid, recv->fd);
//                     delete recv;
//                     recv = NULL;
//                 }

//                 getEpollServer()->error("[Handle::handleImp] error:" + string(ex.what()));
//             }
//             catch (...)
//             {
//                 if(recv)
//                 {
//                     close(recv->uid, recv->fd);
//                     delete recv;
//                     recv = NULL;
//                 }

//                 getEpollServer()->error("[Handle::handleImp] unknown error");
//             }
//         }

//         if(!bYield)
//         {
//             _coroSched->yield();
//         }
//     }
// }


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

// void ServantHandle::handleRecvData(TC_EpollServer::tagRecvData *stRecvData)
void ServantHandle::handleRecvData(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    try
    {
        TarsCurrentPtr current = createCurrent(data);

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
        TLOGERROR("[TARS]ServantHandle::handleRecvData exception:" << ex.what() << endl);
    }
    catch(...)
    {
        TLOGERROR("[TARS]ServantHandle::handleRecvData unknown exception error" << endl);
    }
}

void ServantHandle::handleAsyncResponse()
{
    ReqMessagePtr resp;

    map<string, ServantPtr>::iterator it = _servants.begin();

    while (it != _servants.end())
    {
        while (it->second->getResponseQueue().pop_front(resp, 0))
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
                TLOGERROR("[TARS][ServantHandle::doResponse ex:" << e.what() << "]" << endl);
            }
            catch (...)
            {
                TLOGERROR("[TARS][ServantHandle::doResponse ex.]" << endl);
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
            TLOGERROR("[TARS][ServantHandle::doCustemMessage ex:" << e.what() << "]" << endl);
        }
        catch (...)
        {
            TLOGERROR("[TARS][ServantHandle::doCustemMessage ex.]" << endl);
        }

        ++it;
    }
}

void ServantHandle::handleCustomMessage(bool bExpectIdle)
{
    for (map<string, ServantPtr>::iterator it = _servants.begin(); it != _servants.end(); it++)
    {
        //业务处理附加的自有消息
        try
        {
            it->second->doCustomMessage(bExpectIdle);
            it->second->doCustomMessage();
        }
        catch (exception& e)
        {
            TLOGERROR("[TARS][ServantHandle::doCustemMessage ex:" << e.what() << "]" << endl);
        }
        catch (...)
        {
            TLOGERROR("[TARS][ServantHandle::doCustemMessage ex.]" << endl);
        }
    }
}

bool ServantHandle::allFilterIsEmpty()
{
    map<string, ServantPtr>::iterator it = _servants.begin();

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
    // map<string, TC_EpollServer::BindAdapterPtr>::iterator adpit;

    // map<string, TC_EpollServer::BindAdapterPtr>& adapters = _handleGroup->adapters;

    // for (adpit = adapters.begin(); adpit != adapters.end(); ++adpit)
    // {
    //     ServantPtr servant = ServantHelperManager::getInstance()->create(adpit->first);

    //     if (servant)
    //     {
    //         _servants[servant->getName()] = servant;
    //     }
    //     else
    //     {
    //         TLOGERROR("[TARS]ServantHandle initialize createServant ret null, for adapter `" + adpit->first + "`" << endl);
    //     }
    // }

    ServantPtr servant = ServantHelperManager::getInstance()->create(_bindAdapter->getName());

    if (servant)
    {
        _servants[servant->getName()] = servant;
    }
    else
    {
        TLOGERROR("[TAF]ServantHandle initialize createServant ret null, for adapter `" +_bindAdapter->getName() + "`" << endl);
    }

    map<string, ServantPtr>::iterator it = _servants.begin();

    if(it == _servants.end())
    {
        TLOGERROR("[TARS]initialize error: no servant exists." << endl);

        TarsRemoteNotify::getInstance()->report("initialize error: no servant exists.");

        exit(-1);
    }

    while(it != _servants.end())
    {
        try
        {
            it->second->setHandle(this);

            it->second->initialize();

            TLOGTARS("[TARS][" << it->second->getName() << "] initialize" << endl);
        }
        catch(exception &ex)
        {
            TLOGERROR("[TARS]initialize error:" << ex.what() << endl);

            TarsRemoteNotify::getInstance()->report("initialize error:" + string(ex.what()));

            exit(-1);
        }
        catch(...)
        {
            TLOGERROR("[TARS]initialize unknown exception error" << endl);

            TarsRemoteNotify::getInstance()->report("initialize error");

            exit(-1);
        }
        ++it;
    }
}

// void ServantHandle::heartbeat()
// {
//     time_t fcur = TNOW;

//     map<string, TC_EpollServer::BindAdapterPtr>::iterator it;

//     map<string, TC_EpollServer::BindAdapterPtr>& adapters = _handleGroup->adapters;

//     for (it = adapters.begin(); it != adapters.end(); ++it)
//     {
//         if (abs(fcur - it->second->getHeartBeatTime()) > HEART_BEAT_INTERVAL)
//         {
//             it->second->setHeartBeatTime(fcur);

//             TARS_KEEPALIVE(it->second->getName());

//             //上报连接数 比率
//             if (it->second->_pReportConRate)
//             {
//                 it->second->_pReportConRate->report(it->second->getNowConnection()*1000/it->second->getMaxConns());
//             }

//             //有队列, 且队列长度>0才上报
//             if (it->second->_pReportQueue)
//             {
//                 it->second->_pReportQueue->report(it->second->getRecvBufferSize());
//             }
//         }
//     }
// }


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

TarsCurrentPtr ServantHandle::createCurrent(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TarsCurrentPtr current = new TarsCurrent(this);

    try
    {
        current->initialize(data);
    }
    catch (TarsDecodeException &ex)
    {
        TLOGERROR("[TARS]ServantHandle::handle request protocol decode error:" << ex.what() << endl);
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

            TLOGERROR("[TARS]ServantHandle::handle queue timeout:"
                         << current->_request.sServantName << "|"
                         << current->_request.sFuncName << "|"
                         << data->recvTimeStamp()  << "|"
                         << data->adapter()->getQueueTimeout() << "|"
                         << current->_request.iTimeout << "|"
                         << now << "|" << data->ip() << ":" << data->port() << endl);

            current->sendResponse(TARSSERVERQUEUETIMEOUT);

            return NULL;
        }
    }

    return current;
}

TarsCurrentPtr ServantHandle::createCloseCurrent(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TarsCurrentPtr current = new TarsCurrent(this);

    current->initializeClose(data);
    current->setReportStat(false);
    current->setCloseType(data->closeType());
    return current;
}

void ServantHandle::handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TLOGTARS("[TARS]ServantHandle::handleClose,adapter:" << data->adapter()->getName()
                << ",peer:" << data->ip() << ":" << data->port() << endl);

    TarsCurrentPtr current = createCloseCurrent(data);

    map<string, ServantPtr>::iterator sit = _servants.find(current->getServantName());

    if (sit == _servants.end())
    {
        TLOGERROR("[TARS]ServantHandle::handleClose,adapter:" << data->adapter()->getName()
                << ",peer:" << data->ip() << ":" << data->port()<<", " << current->getServantName() << " not found" << endl);

        return;
    }


    try
    {
        //业务逻辑处理
        sit->second->doClose(current);
    }
    catch(exception &ex)
    {
        TLOGERROR("[TARS]ServantHandle::handleClose " << ex.what() << endl);

        return;
    }
    catch(...)
    {
        TLOGERROR("[TARS]ServantHandle::handleClose unknown error" << endl);

        return;
    }
}

void ServantHandle::handleTimeout(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TarsCurrentPtr current = createCurrent(data);

    if (!current) return;

    //上报超时数目
    if(data->adapter()->_pReportTimeoutNum)
        data->adapter()->_pReportTimeoutNum->report(1);

    TLOGERROR("[TARS]ServantHandle::handleTimeout adapter '"
                 << data->adapter()->getName()
                 << "', recvtime:" << data->recvTimeStamp() << "|"
                 << ", timeout:" << data->adapter()->getQueueTimeout()
                 << ", id:" << current->getRequestId() << endl);

    if (current->getBindAdapter()->isTarsProtocol())
    {
        current->sendResponse(TARSSERVERQUEUETIMEOUT);
    }
}

void ServantHandle::handleOverload(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TarsCurrentPtr current = createCurrent(data);

    if (!current) return;

    TLOGERROR("[TARS]ServantHandle::handleOverload adapter '"
                 << data->adapter()->getName()
                 << "',overload:-1,queue capacity:"
                 << data->adapter()->getQueueCapacity()
                 << ",id:" << current->getRequestId() << endl);

    if (current->getBindAdapter()->isTarsProtocol())
    {
        current->sendResponse(TARSSERVEROVERLOAD);
    }
}

void ServantHandle::handle(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TarsCurrentPtr current = createCurrent(data);

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


#ifdef _USE_OPENTRACKING
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

bool ServantHandle::processDye(const TarsCurrentPtr &current, string& dyeingKey)
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
        TLOGTARS("[TARS] servant got a dyeing request, message_type set" << current->getMessageType() << endl);

        if (dyeingIt != current->getRequestStatus().end())
        {
            TLOGTARS("[TARS] servant got a dyeing request, dyeing key:" << dyeingIt->second << endl);

            dyeingKey = dyeingIt->second;
        }
        return true;
    }

    //servant已经被染色, 开启染色日志
    if (ServantHelperManager::getInstance()->isDyeing())
    {
        map<string, string>::const_iterator dyeingKeyIt = current->getRequestStatus().find(ServantProxy::STATUS_GRID_KEY);

        if (dyeingKeyIt != current->getRequestStatus().end() &&
            ServantHelperManager::getInstance()->isDyeingReq(dyeingKeyIt->second, current->getServantName(), current->getFuncName()))
        {
            TLOGTARS("[TARS] dyeing servant got a dyeing req, key:" << dyeingKeyIt->second << endl);

            dyeingKey = dyeingKeyIt->second;

            return true;
        }
    }

    return false;
}

bool ServantHandle::checkValidSetInvoke(const TarsCurrentPtr &current)
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
            TLOGTARS("[TARS] servant got a setname request, setname key:" << setIt->second << endl);

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
                    TLOGERROR("[TARS]ServantHandle::checkValidSetInvoke|"
                            << current->getIp() << "|"
                            << current->getMessageType() << "|"
                            << current->getServantName() << "|"
                            << current->getFuncName() << "|client:"
                            << ClientConfig::SetDivision << "|server:"
                            << sSetName << endl);
                    current->sendResponse(TARSINVOKEBYINVALIDESET);
                    return false;
                }
            }
        }
        else
        {
            TLOGERROR("[TARS]ServantHandle::checkValidSetInvoke|"
                            << current->getIp() << "|"
                            << current->getMessageType() << "|"
                            << current->getServantName() << "|"
                            << current->getFuncName() << "|client:"
                            << ClientConfig::SetDivision << "|server:"
                            << sSetName << endl);
            current->sendResponse(TARSINVOKEBYINVALIDESET);
            return false;
        }
    }

    //没有按set规则调用
    return true;
}

void ServantHandle::handleTarsProtocol(const TarsCurrentPtr &current)
{
    TLOGTARS("[TARS]ServantHandle::handleTarsProtocol current:"
                << current->getIp() << "|"
                << current->getPort() << "|"
                << current->getMessageType() << "|"
                << current->getServantName() << "|"
                << current->getFuncName() << "|"
                << current->getRequestId() << "|"
                << TC_Common::tostr(current->getRequestStatus())<<endl);

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
#ifdef _USE_OPENTRACKING
    //处理tracking信息
    processTracking(current);
#endif
    map<string, ServantPtr>::iterator sit = _servants.find(current->getServantName());

    if (sit == _servants.end())
    {
        current->sendResponse(TARSSERVERNOSERVANTERR);
#ifdef _USE_OPENTRACKING
        finishTracking(TARSSERVERNOSERVANTERR, current);
#endif
        return;
    }

    int ret = TARSSERVERUNKNOWNERR;

    string sResultDesc = "";

    vector<char> buffer;
    try
    {
        //业务逻辑处理
        ret = sit->second->dispatch(current, buffer);
    }
    catch(TarsDecodeException &ex)
    {
        TLOGERROR("[TARS]ServantHandle::handleTarsProtocol " << ex.what() << endl);

        ret = TARSSERVERDECODEERR;

        sResultDesc = ex.what();
    }
    catch(TarsEncodeException &ex)
    {
        TLOGERROR("[TARS]ServantHandle::handleTarsProtocol " << ex.what() << endl);

        ret = TARSSERVERENCODEERR;

        sResultDesc = ex.what();
    }
    catch(exception &ex)
    {
        TLOGERROR("[TARS]ServantHandle::handleTarsProtocol " << ex.what() << endl);

        ret = TARSSERVERUNKNOWNERR;

        sResultDesc = ex.what();
    }
    catch(...)
    {
        TLOGERROR("[TARS]ServantHandle::handleTarsProtocol unknown error" << endl);

        ret = TARSSERVERUNKNOWNERR;

        sResultDesc = "handleTarsProtocol unknown exception error";
    }

    //单向调用或者业务不需要同步返回
    if (current->isResponse())
    {
        current->sendResponse(ret, buffer, TarsCurrent::TARS_STATUS(), sResultDesc);
    }
#ifdef _USE_OPENTRACKING
    finishTracking(ret, current);
#endif
}

void ServantHandle::handleNoTarsProtocol(const TarsCurrentPtr &current)
{
    TLOGTARS("[TARS]ServantHandle::handleNoTarsProtocol current:"
        << current->getIp() << "|"
        << current->getPort() << "|"
        << current->getServantName() << endl);

    map<string, ServantPtr>::iterator sit = _servants.find(current->getServantName());

    assert(sit != _servants.end());

    vector<char> buffer;

    try
    {
        //业务逻辑处理
        sit->second->dispatch(current, buffer);
    }
    catch(exception &ex)
    {
        TLOGERROR("[TARS]ServantHandle::handleNoTarsProtocol " << ex.what() << endl);
    }
    catch(...)
    {
        TLOGERROR("[TARS]ServantHandle::handleNoTarsProtocol unknown error" << endl);
    }

    if (current->isResponse())
    {
        current->sendResponse((const char*)(&buffer[0]), buffer.size());
    }
}

////////////////////////////////////////////////////////////////////////////
}
