#include "SipRouterImp.h"
#include "servant/Application.h"
#include "JsonUtil.h"

using namespace std;

//////////////////////////////////////////////////////
void SipRouterImp::initialize()
{
    m_bMainServiceReady = false;
    m_bMasterThdDown = false;
    m_bIsOfflineRoute = false;

    m_tranProtoclName = "udp";

    m_LastSchdTimeInMs = TNOWMS;
    m_BaseSchdTimeInMs = 1000;
    m_lastFailedTime = 0;
    m_baseFailedTime = 30 * 1000;
    m_maxFailedTime = 5 * 60 * 1000;
    m_iSerialNumber = 0;
    m_responseType = 0;

    m_driver = Common::NetDriver::create(10240, 2);
    m_httpServerManager = new SipENUMProxy::HttpServerManager(m_driver, 0, 16*1024, this);
    m_httpClientManager = new SipENUMProxy::HttpClientManager(m_driver, 16*1024, "");

}

//////////////////////////////////////////////////////
void SipRouterImp::destroy()
{
    m_bMasterThdDown = true;

}

bool SipRouterImp::onActivate()
{
    TLOGINFO("[ENUMProxy] ****** onActivate() ******" << endl);

    m_strMainObjId = ServerConfig::ServerName + "." + ServerConfig::Application + ".SipRouterObj";
    m_strAppName = ServerConfig::Application;

    // m_resourceAgent = Application::getCommunicator()->stringToProxy<VoipApp::ResourceServerPrx>("VoipApp.ResourceServer.ResourceServerObj");
    // if (!m_resourceAgent)
    //     return false;

    m_bMainServiceReady = true;

    TLOGINFO("[ENUMProxy] ****** onActivate() ****** : m_strAppName: " << m_strAppName << " ,m_strMainObjId:" << m_strMainObjId << endl);

    return true;
}

void SipRouterImp::onSchd()
{
    int64_t now = TNOWMS;
    int64_t thisHour = now / (3600 * 1000);
    int64_t thisMinute = now / (60 * 1000);
    int64_t diffTime1 = (now - m_LastSchdTimeInMs);

    if(diffTime1 < m_BaseSchdTimeInMs || m_bMasterThdDown)
        return;

    //registerEnumproxyToResource();

    m_httpServerManager->checkTimeout();
    m_httpClientManager->checkTimeout();

    if (thisMinute > m_lastMinute)
    {
        m_lastMinute = thisMinute;
        clearExpiredHttpRequestRecords();
        getCallRoutingSize();
    }

    m_LastSchdTimeInMs = now;
}

// void SipRouterImp::onUpdateConfigs()
// {
//     std::string httpAddr;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_HTTP_REQ_ADDR, httpAddr))
//     {
//         m_strHttpReqAddr = httpAddr;
//     }

//     std::string authKey;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_AUTHKEY, authKey))
//     {
//         m_strAuthKey = authKey;
//     }

//     std::string channel;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_CHANNEL, channel))
//     {
//         m_strChannel = channel;
//     }

//     std::string midUser;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_MID_USER, midUser))
//     {
//         m_strMidUser = midUser;
//     }
//     else
//     {
//         m_strMidUser.clear();
//     }
//     std::string origSrcUser;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_ORIN_SRC_USER, origSrcUser))
//     {
//         m_strOrigSrcUser = origSrcUser;
//     }
//     else
//     {
//         m_strOrigSrcUser.clear();
//     }
//     std::string origDstUser;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_ORIN_DST_USER, origDstUser))
//     {
//         m_strOrigDstUser = origDstUser;
//     }
//     else
//     {
//         m_strOrigDstUser.clear();
//     }

//     std::string chargeUser;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_CHARGE_USER, chargeUser))
//     {
//         m_strChargeUser = chargeUser;
//     }
//     else
//     {
//         m_strChargeUser.clear();
//     }

//     std::string entIp;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_IP, entIp))
//     {
//         m_strEntIp = entIp;
//     }
//     else
//     {
//         m_strEntIp.clear();
//     }
//     int entPort;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_PORT, entPort))
//     {
//         m_iEntPort = entPort;
//     }
//     else
//     {
//         m_iEntPort = 5060;
//     }

//     std::string latGw;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_LAST_GW_IP, latGw))
//     {
//         m_strLastGwIp = latGw;
//     }
//     else
//     {
//         m_strLastGwIp.clear();
//     }

//     std::string entIp2;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_IP2, entIp2))
//     {
//         m_strEntIp2 = entIp2;
//     }
//     else
//     {
//         m_strEntIp2.clear();
//     }

//     int entPort2;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_PORT2, entPort2))
//     {
//         m_iEntPort2 = entPort2;
//     }
//     else
//     {
//         m_iEntPort2 = 5060;
//     }

//     std::string latGw2;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_LAST_GW_IP2, latGw2))
//     {
//         m_strLastGwIp2 = latGw2;
//     }
//     else
//     {
//         m_strLastGwIp2.clear();
//     }

//     std::string caller;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_CALLER, caller))
//     {
//         m_strCaller = caller;
//     }
//     else
//     {
//         m_strCaller.clear();
//     }
//     std::string callee;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_CALLEE, callee))
//     {
//         m_strCallee = callee;
//     }
//     else
//     {
//         m_strCallee.clear();
//     }

//     std::string from;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_FROM, from))
//     {
//         m_strFrom = from;
//     }
//     else
//     {
//         m_strFrom.clear();
//     }

//     std::string to;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_TO, to))
//     {
//         m_strTo = to;
//     }
//     else
//     {
//         m_strTo.clear();
//     }

//     std::string displayName;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_DISPLAY_NAME, displayName))
//     {
//         m_strDisplayName = displayName;
//     }
//     else
//     {
//         m_strDisplayName.clear();
//     }

//     std::string routeId;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_ROUTE_ID, routeId))
//     {
//         m_strRouteId = routeId;
//     }
//     else
//     {
//         m_strRouteId.clear();
//     }

//     std::string atomId;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_ATOM_ID, atomId))
//     {
//         m_strAtomId = atomId;
//     }
//     else
//     {
//         m_strAtomId.clear();
//     }

//    int serviceType;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_SERVICE_TYPE, serviceType))
//     {
//         m_iServiceType = serviceType;
//     }
//     else
//     {
//         m_iServiceType = 0;
//     }
//     int record;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_RECORD, record))
//     {
//         m_iRecord = record;
//     }
//     else
//     {
//         m_iRecord = 0;
//     }
//     int beep;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_BEEP, beep))
//     {
//         m_iBeep = beep;
//     }
//     else
//     {
//         m_iBeep = 0;
//     }
//     int resultCode;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_RESULT, resultCode))
//     {
//         m_iResultCode = resultCode;
//     }
//     else
//     {
//         m_iResultCode = 0;
//     }

//     int inbound;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_SIP_INBOUND, inbound))
//     {
//         m_iInbound = inbound;
//     }
//     else
//     {
//         m_iInbound = 0;
//     }

//     int outbound;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_SIP_OUTBOUND, outbound))
//     {
//         m_iOutbound = outbound;
//     }
//     else
//     {
//         m_iOutbound = 0;
//     }

//     std::string listenIp;
//     if (_application->getAppConfig(ENUM_PROXY_START_UP_PARAMS_LISTEN_IP, listenIp))
//     {
//         m_strListenIp = listenIp;
//     }

//     int responseType;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_RESPONSE_TYPE, responseType))
//     {
//         m_responseType = responseType;
//     }
//     else
//     {
//         m_responseType = 0;
//     }

//     int maxFailedTime;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_MAX_FAILED_TIME, maxFailedTime))
//     {
//         m_maxFailedTime = maxFailedTime * 60 * 1000;
//     }
//     else
//     {
//         m_maxFailedTime = 5 * 60 * 1000;
//     }

//     int baseFailedTime;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_REQUEST_INTERVAL, baseFailedTime))
//     {
//         m_baseFailedTime = baseFailedTime * 1000;
//     }
//     else
//     {
//         m_baseFailedTime = 30 * 1000;
//     }

//     int numberTypeFlag = 0;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_NUMBER_TYPE_FLAG, numberTypeFlag))
//     {
//         m_iNumTypeFlag = numberTypeFlag;
//     }
//     else
//     {
//         m_iNumTypeFlag = 0;
//     }

//     int isOfflineRoute = 0;
//     if (_application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_OFFLINE_ROUTE, isOfflineRoute))
//     {
//         m_bIsOfflineRoute = isOfflineRoute ? true : false;
//     }
//     else
//     {
//         m_bIsOfflineRoute = false;
//     }

//     int iLocalListenPort = 0;
//     bool result = _application->getAppConfigAsInt(ENUM_PROXY_START_UP_PARAMS_LISTEN_PORT, iLocalListenPort);
//     if (result && iLocalListenPort != m_iLocalListenPort)
//     {
//         if (m_Listener)
//         {
//             m_Listener->close();
//         }

//         m_Listener = _application->getDriver()->listen(m_tranProtoclName, "", iLocalListenPort, this, true);
//         if(m_Listener == 0)
//         {
//             m_iLocalListenPort = 0;
//             TLOGERROR("[ENUMProxy] ======> Listen() called. Listen failed, Port=" << std::to_string(iLocalListenPort)
//                 << ",m_iLocalListenPort:" << std::to_string(iLocalListenPort) << ",protocol:" << m_tranProtoclName << endl);
//             return;
//         }

//         m_iLocalListenPort = iLocalListenPort;

//         std::string localHost;
//         int localPort;
//         m_Listener->getLocal(localHost, localPort);
//         m_strLocalIp = localHost;

//         if (m_tranProtoclName != "tcp")
//         {
//             if (m_httpServerManager->listen(listenIp, iLocalListenPort))
//                 TLOGINFO("[ENUMProxy] *** Listen() ***,======>TCP Listening ... listenIp: ["<< localHost << ":" << std::to_string(localPort) << "]" << endl);
//             else
//             {
//                 m_iLocalListenPort = 0;
//                 TLOGWARN("[ENUMProxy] *** Listen() ***,======>TCP Listening Failed... listenIp: ["<< localHost << ":" << std::to_string(localPort) << "]" << endl);
//             }
//         }
//     }
// }

// void SipRouterImp::registerEnumproxyToResource()
// {
//     Common::Resource resource;
//     Common::EndpointVec endpointVec;
//     _mainAdapter->getEndpoints(endpointVec);

//     class updateResource_async : public Common::AgentAsync
//     {
//     public:
//         updateResource_async(){}
//         void cmdResult(int rslt,const Common::IputStreamPtr& iput,const Common::ObjectPtr& userdata)
//         {
//             std::string reason;
//             if (!Common::ResourceServerAgent::updateResource_end(rslt, iput))
//             {
//                 reason = Common::ObjectAgent::getLastReason();
//                 TLOGWARN("[ENUMProxy] registerEnumproxyToResource() called. updateResource_end() return FALSE! reason : " << reason << endl);
//                 return;
//             }
//         }
//     private:
//     };

//     resource._endpoints = endpointVec;
//     resource._freePercent = 1;
//     resource._identity = m_strMainObjId;

//     m_resourceAgent.updateResource_begin(new updateResource_async(), ENUM_PROXY_REGISTER_TYPE, resource);
// }

//HttpRequestReceiver
bool SipRouterImp::onHttpRequest(const std::string & content, const Json::Value & request, const SipENUMProxy::HttpServerSessionPtr & httpServer)
{
    if (m_responseType)
        return true;

    Json::Value val;
    int serial = 0;
    std::string callId, time, caller, callee, reason;
    if (!SipENUMProxy::JsonUtil::jsonTextDecode(content, callId, serial, time, caller, callee, reason))
    {
        TLOGWARN("[ENUMProxy] " << std::string(__FUNCTION__) + "() called failed reason:" + reason + ", content:" + content << endl);
        httpServer->onSendReply(true, 0, val);
        return true;
    }

    VoipApp::JegoRouterInfo info;
    info.version = 0;
    info.type = "";
    info.time = time;
    info.caller = m_strCaller;
    info.callee = m_strCallee;
    info.callId = callId;
    info.gwAddr = m_strLastGwIp;
    info.number = m_strDisplayName;
    info.chargeAccount = m_strChargeUser;
    info.enterpriseIp = m_strEntIp;
    info.enterprisePort = m_iEntPort;
    info.midCallUser = m_strMidUser;
    info.origSrcUser = m_strOrigSrcUser;
    info.origDstUser = m_strOrigDstUser;
    info.routeId = m_strRouteId;
    info.atomId = m_strAtomId;
    info.priceNumber = "";
    info.resultCode = m_iResultCode;
    info.serviceType = m_iServiceType;
    info.record = m_iRecord;
    info.beep = m_iBeep;
    info.sipInbound = m_iInbound;
    info.sipOutbound = m_iOutbound;
    info.routeInfoList = std::vector<VoipApp::RouteInfo>();

    SipENUMProxy::JsonUtil::jsonTextEncode(info, serial, m_strFrom, m_strTo, m_strLastGwIp2, m_strEntIp2, m_iEntPort2, m_iNumTypeFlag, val);

    httpServer->onSendReply(true, 0, val);
    return true;
}

//HttpRespReceiver
void SipRouterImp::onHttpResponse(bool ret, int64_t tid, const std::string & serialNum, const std::string & resCode, const std::string & content, const std::string & reason)
{
    TLOGDEBUG("[ENUMProxy] " << std::string(__FUNCTION__) + "() called, content:" + content + ",tid:" + std::to_string(tid) << endl);

    VoipApp::JegoRouterInfo info;
    std::map<std::string, std::string> params;

    SipENUMProxy::HttpRequestItemPtr pRecord = m_httpReqManager.getHttpRequestRecord(tid);
    if (!pRecord)
    {
        TLOGWARN("[ENUMProxy] " << std::string(__FUNCTION__) + "() called, get record failed,tid:" + std::to_string(tid) + ",content:" + content << endl);
        return;
    }

    if (!ret)
    {
        int64_t curr = TNOWMS;
        int64_t diff = curr - pRecord->m_reqTime;
        bool result = false;
        if (reason == "http-connection-error")
        {
            updateReqFailedTime(curr);

            // if(m_bIsOfflineRoute && diff < m_baseFailedTime)
            //     result = m_callRoutingManager.getCallRoutingRecord(pRecord->m_callId, pRecord->m_caller, pRecord->m_callee, pRecord->m_enterIp, pRecord->m_gwAddr, info);
        }

        TLOGWARN("[ENUMProxy] " << std::string(__FUNCTION__) + "() called failed,tid:" + std::to_string(tid) + ",reason:" + reason + ", cache result:" + std::to_string(result) + ", diff:" + std::to_string(diff) << endl);

        VoipApp::SipRouter::async_response_getRouteInfo2(pRecord->m_current, result, info, params);
        m_httpReqManager.removeHttpRequestRecord(tid);
        return;
    }

    updateReqFailedTime(0);

    std::string desc;
    int serial = 0;
    std::string errReason;
    bool result = SipENUMProxy::JsonUtil::jsonTextDecode(content, info, serial, desc, params, errReason);
    if (!result)
    {
        errReason = "decode http response failed";

        TLOGWARN("[ENUMProxy] " << std::string(__FUNCTION__) + "() called, jsonTextDecode failed,tid:" + std::to_string(tid) + ",content:" + content + ", callId:" + info.callId << endl);

        VoipApp::SipRouter::async_response_getRouteInfo2(pRecord->m_current, false, info, params);
        m_httpReqManager.removeHttpRequestRecord(tid);
        return;
    }

    std::string from, to, numberType;
    std::map<std::string, std::string>::iterator itor = params.find(SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_FROM);
    if (itor != params.end())
    {
        from = itor->second;
    }
    itor = params.find(SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_TO);
    if (itor != params.end())
    {
        to = itor->second;
    }

    itor = params.find(SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_NUMBER_TYPE_FLAG);
    if (itor != params.end())
    {
        numberType = itor->second;
    }

    TLOGINFO("[ENUMProxy] " << std::string(__FUNCTION__) + "() called, content:" + content + "\n tid:" + std::to_string(tid) + "\n callId:" + info.callId + "\n callee:" + info.callee + "\n caller:" + info.caller + "\n from:" + from + "\n to:" + to
        +"\n mgcfAddr:" + info.gwAddr + "\n chargeAccount:" + info.chargeAccount + "\n displayName:" + info.number + "\n serial:" + std::to_string(serial)+ "\n desc:" + desc + "\n version:" + std::to_string(info.version)
        + "\n type:" + info.type + "\n time:" + info.time + "\n enterpriseIp:" + info.enterpriseIp + "\n enterprisePort:" + std::to_string(info.enterprisePort) + "\n middleUser:" + info.midCallUser + "\n originSrcUser:" + info.origSrcUser
        + "\n originDstUser:" + info.origDstUser + "\n routeId:" + info.routeId + "\n atomId" + info.atomId + "\n priceNumber:" + info.priceNumber + "\n record:" + std::to_string(info.record) + "\n beep:"+ std::to_string(info.beep) + "\n numberType:" + numberType
        + "\n serviceType:" + std::to_string(info.serviceType) + "\n resultCode:" + std::to_string(info.resultCode) + "\n routeInfoList:" + std::to_string((int)info.routeInfoList.size()) << endl);

    VoipApp::SipRouter::async_response_getRouteInfo2(pRecord->m_current, true, info, params);

    // if(m_bIsOfflineRoute)
    //     m_callRoutingManager.insertCallRoutingRecord(pRecord->m_caller, pRecord->m_callee, pRecord->m_enterIp, pRecord->m_gwAddr, info);

    m_httpReqManager.removeHttpRequestRecord(tid);
}

tars::Bool SipRouterImp::getRouteInfo(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipRouterImp::getRouteInfo2(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,map<std::string, std::string> &outParams,tars::TarsCurrentPtr current)
{
    int64_t failedTime = getLastFailedTime();
    int64_t diff = TNOWMS - failedTime;

    TLOGINFO("[ENUMProxy] " << std::string(__FUNCTION__) + "() called, m_strHttpReqAddr:" + m_strHttpReqAddr + ", callId:" + callId + ", gwOid:" + gwOid + ", failedTime:" + std::to_string(failedTime) + ",diff:" + std::to_string(diff)
        + ",m_baseFailedTime:" + std::to_string(m_baseFailedTime) + ", m_maxFailedTime:" + std::to_string(m_maxFailedTime) << endl);

    // 快速路径：使用离线路由缓存，直接同步返回
    // if(failedTime && (diff >= m_baseFailedTime && diff <= m_maxFailedTime))
    // {
    //     bool result = m_callRoutingManager.getCallRoutingRecord(callId, caller, callee, enterpriseIp, sipgatewayIp, info);
    //     if(!result)
    //     {
    //         TLOGWARN("[ENUMProxy] " << std::string(__FUNCTION__) + "() offline route not found, callId:" + callId << endl);
    //     }
    //     outParams = params;
    //     return result;
    // }

    // 超出最大失败窗口，重置失败时间
    // if (failedTime && diff > m_maxFailedTime)
    // {
    //     updateReqFailedTime(0);
    // }

    // 异步路径：发送 HTTP，稍后在 onHttpResponse 中通过 async_response_getRouteInfo2 回包
    if (!sendHttpRequest(current, caller, callee, callId, callForwording, sipgatewayIp, enterpriseIp, params))
    {
        TLOGWARN("[ENUMProxy] " << std::string(__FUNCTION__) + "() sendHttpRequest failed, callId:" + callId << endl);
        return false;
    }

    // 告诉 Tars 不要在返回时自动应答，本次调用由 async_response_getRouteInfo2 负责回包
    current->setResponse(false);
    return true;
}


bool SipRouterImp::sendHttpRequest(const tars::TarsCurrentPtr& current, const std::string& caller, const std::string& callee, const std::string& callId, const std::string& callForwording,
    const std::string& sipgatewayIp, const std::string& enterpriseIp, const std::map<std::string, std::string>& params)
{
    if(m_iSerialNumber > 65530)
        m_iSerialNumber = 0;

    m_iSerialNumber++;

    int64_t startTime = TNOWMS;
    std::string reportTime = tars::TC_Common::tm2str(static_cast<time_t>(startTime / 1000), "%Y-%m-%d %H:%M:%S");
    std::string text = SipENUMProxy::JsonUtil::jsonTextEncode(m_strAuthKey, m_iSerialNumber,m_strChannel, caller, callee, callId, callForwording, sipgatewayIp, enterpriseIp, reportTime);
    int64_t tid = m_httpReqManager.generateTid();

    m_httpClientManager->sendRequest(m_strHttpReqAddr, 0, this, tid, text, std::to_string(m_iSerialNumber), SipENUMProxy::kHttpReqTypeOthers);

    m_httpReqManager.insertHttpReqRecord(current, tid, callId, caller, callee, enterpriseIp, sipgatewayIp);
    return true;
}

void SipRouterImp::updateReqFailedTime(int64_t time)
{
    tars::TC_LockT<tars::TC_ThreadMutex> lock(m_mutexLastFailedTime);
    if (!time)
    {
        m_lastFailedTime = time;
    }
    else if (!m_lastFailedTime)
    {
        m_lastFailedTime = time;
    }
}

int64_t SipRouterImp::getLastFailedTime()
{
    tars::TC_LockT<tars::TC_ThreadMutex> lock(m_mutexLastFailedTime);
    return m_lastFailedTime;
}

void SipRouterImp::clearExpiredHttpRequestRecords()
{
    m_httpReqManager.clearExpiredHttpRequestRecords();
}

void SipRouterImp::getCallRoutingSize()
{
    //_application->setStatisticsLong("ENUM.RouteRecord.Size", m_callRoutingManager.getsize());
}
