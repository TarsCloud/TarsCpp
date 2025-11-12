#include <thread>
#include <chrono>
#include <random>

#include "servant/Application.h"
#include "servant/PropertyReport.h"
#include "util/tc_timeprovider.h"
#include "util/tc_uuid_generator.h"

#include "SipRelayImp.h"
#include "SipGW2Common.h"
//#include "SipGateway/src/SipMsgCommon.h"
#include "SipGateway/src/SipMsgDecode.h"
#include "SipGateway/src/SipMsgDecodedPart.h"
#include "SipCallController/src/SipGW3Common.h"
#include "SipCallController/src/SipDialog2.h"
#include "ENUMProxy/src/SipRouter.h"
#include "SipCallController/src/SipCallController.h"

using namespace std;

int getRandom(int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, max);
    return dis(gen);
}

// 统一的后台任务线程函数
void SipRelayImp::AccessConfigReadThreadFunc()
{
    int64_t lastSchdTime = 0;           // 上次执行onSchd的时间（5秒周期）
    int64_t lastAccessConfigTime = 0;   // 上次执行AccessConfig的时间（10秒周期）
    int64_t lastHeartbeatTime = 0;      // 上次执行心跳的时间（30秒周期）
    int64_t lastMinute = 0;             // 上次执行minute的时间
    int64_t lastHour = 0;               // 上次执行hour的时间

    while(1)
    {
        int64_t now = TNOWMS;
        
        // 每5秒执行一次：定时调度任务
        if (now - lastSchdTime >= 5000)
        {
            //onSchd();
            loadMgcfSipGatewayResources();
            loadSipGw3Resources();
            loadEnumproxyResources();
            lastSchdTime = now;
        }
        
        // 每10秒执行一次：配置读取和统计上报
        if (now - lastAccessConfigTime >= 10000)
        {
            buildConnectionToMgcfs();
            setLogsAndStatistics();
            getCloudSipGwIdAndIp();
            lastAccessConfigTime = now;
        }
        
        // 每30秒执行一次：心跳
        if (now - lastHeartbeatTime >= 30000)
        {
            try
            {
                int ret = m_registryPrx->heartbeat(m_strMainObjId);
                if (ret == -2)
                {
                    // 服务不存在，重新注册
                    TLOGWARN("Service not found in registry, re-register" << endl);
                    registerMgcfSipGatewayToResource();
                }
            }
            catch (exception& e)
            {
                TLOGERROR("Heartbeat failed: " << e.what() << endl);
            }
            lastHeartbeatTime = now;
        }
        
        // 每分钟执行一次的任务
        if (now - lastMinute >= 60000)
        {
            readSipGatewayAccessedConfigInfo();
            sendOptionToMgcf();
            lastMinute = now;
            m_remianMsgs.cleanExpiredRemains();
        }

        // 每小时执行一次的任务
        if (now - lastHour >= 3600000)
        {
            m_sipDialogRecordManager.exceptionRecordDeals();
            lastHour = now;
        }
        // 休眠1秒，避免CPU空转
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void SipRelayImp::initialize()
{
    m_LastSchdTimeInMs = TNOWMS;
    m_BaseSchdTimeInMs = 1000;      //ms

    m_bServiceActivated = false;
    m_bMasterThdDown = false;
    m_bTcpProUsed = false;
    m_transProType = SipGateway::kTransUDP;
    m_tranProtoclName = "udp";
    m_transProChangeType = SipGateway::kTransChangeNone;

    m_iLocalListenPort = 0;
    m_iLocalSendPort = 0;
    m_iCurSendPort = 0;

    m_lastMinute = 0;
    m_lastHour = 0;
    m_Listener = 0;

    m_strToMgcfSipDialogId = "mgcf";
    m_connectionCallIdManager.setToSBCSipDialogId(m_strToMgcfSipDialogId);

    if (!SipMsgCommon::SipMsg_Init())
    {
        TLOGERROR("SipMsg_Init() failed" << endl);
        m_bServiceActivated = false;
        return;
    }

    m_strMainObjId = ServerConfig::Application + "." + ServerConfig::ServerName + ".SipRelayObj";
    m_strAppName = ServerConfig::Application;
    m_strDataCenter = "";// TODO: 从配置读取

    m_routerPrx = Application::getCommunicator()->stringToProxy<VoipApp::SipRouterPrx>("VoipApp.ENUMProxy.SipRouterObj");
    m_sipProxyDbAgent = Application::getCommunicator()->stringToProxy<VoipApp::SipDbPrx>("VoipApp.SipProxyDb.SipDbObj");

    // 初始化Registry代理
    m_registryPrx = Application::getCommunicator()->stringToProxy<VoipApp::SipRegPrx>("VoipApp.SipRegistry.SipRegObj");

    // 注册服务
    registerMgcfSipGatewayToResource();

    // 启动统一的后台任务线程（包含：定时调度、配置读取、统计上报、心跳）
    std::thread(&SipRelayImp::AccessConfigReadThreadFunc, this).detach();
    TLOGINFO("SipRelayImp initialized, mainObjId: " << m_strMainObjId << "m_strAppName: " << m_strAppName << endl);
}

//////////////////////////////////////////////////////
void SipRelayImp::destroy()
{
    //std::lock_guard<std::mutex> lock(this);
    m_bMasterThdDown = true;
    SipMsgCommon::SipMsg_Destory();
    // 从Registry注销服务
    try {
        if (m_registryPrx) {
            m_registryPrx->unregisterService(m_strMainObjId);
            TLOGINFO("Unregister service from registry: " << m_strMainObjId << endl);
        }
    } catch (exception& e) {
        TLOGERROR("Unregister service failed: " << e.what() << endl);
    }

    TLOGINFO("SipRelayImp destroyed" << endl);
}

// 配置热加载回调
void SipRelayImp::onConfigReload()
{
    TLOGINFO("onConfigReload called, reloading configuration..." << endl);
    
    if (reloadConfig()) {
        TLOGWARN("Configuration changed, re-registering to registry..." << endl);
        registerMgcfSipGatewayToResource();
        TLOGINFO("Re-registered with new configuration" << endl);
    } else {
        TLOGDEBUG("Configuration not changed, skip re-register" << endl);
    }
}

// 重新加载配置（返回true表示配置有变化）
bool SipRelayImp::reloadConfig()
{
    TC_Config conf;
    conf.parseFile(ServerConfig::BasePath + ServerConfig::ServerName + ".conf");
    
    // 读取新配置
    std::string newDataCenter = conf.get("/main<DataCenter>", "");
    std::string newLocalIp = conf.get("/main<LocalIp>", "");
    int newListenPort = TC_Common::strto<int>(conf.get("/main<LocalListenPort>", "0"));
    std::string newProtocol = conf.get("/main<TransProtocol>", "udp");
    
    // 检测配置是否变化
    bool changed = false;
    
    if (m_tranProtoclName != newProtocol) {
        TLOGINFO("Protocol changed: " << m_tranProtoclName << " -> " << newProtocol << endl);
        changed = true;
    }
    
    if (m_iLocalListenPort != newListenPort) {
        TLOGINFO("Listen port changed: " << m_iLocalListenPort << " -> " << newListenPort << endl);
        changed = true;
    }
    
    if (m_strDataCenter != newDataCenter) {
        TLOGINFO("DataCenter changed: " << m_strDataCenter << " -> " << newDataCenter << endl);
        changed = true;
    }
    
    if (m_strLocalIp != newLocalIp) {
        TLOGINFO("Local IP changed: " << m_strLocalIp << " -> " << newLocalIp << endl);
        changed = true;
    }
    
    // 应用新配置
    if (changed)
    {
        m_strDataCenter = newDataCenter;
        m_strLocalIp = newLocalIp;
        m_iLocalListenPort = newListenPort;
        m_tranProtoclName = newProtocol;
        
        TLOGINFO("Config applied - protocol:" << m_tranProtoclName 
                << ", ip:" << m_strLocalIp 
                << ", port:" << m_iLocalListenPort 
                << ", dc:" << m_strDataCenter << endl);
    }
    
    return changed;
}

tars::Bool SipRelayImp::eraseSipDlgIdAndConnection(const std::string & sipGatewayId,const std::string & dialogId,tars::TarsCurrentPtr _current_)
{
    TLOGINFO(std::string(__FUNCTION__) << "() called, sipGatewayId:" << sipGatewayId << ", dialogId:" << dialogId << endl);

    ereaseSipDialogRecordBySipDialogID(dialogId);
    return true;
}

tars::Bool SipRelayImp::getIntranetIp(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,std::string &intranetIp,tars::TarsCurrentPtr _current_)
{
    TLOGINFO(std::string(__FUNCTION__) << "() called, sipGatewayId:" << sipGatewayId << endl);
    mgcfSipGatewayOid = m_strMainObjId;
    intranetIp = m_strLocalHost;
    return true;
}

tars::Bool SipRelayImp::getMgcfConnectStatus(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,map<std::string, std::string> &params,tars::TarsCurrentPtr _current_)
{
    TLOGINFO(std::string(__FUNCTION__) << "() called, sipGatewayId:" << sipGatewayId << endl);

    mgcfSipGatewayOid = m_strMainObjId;

    std::lock_guard<std::mutex> lock(m_mutexMgcfStatus);
    if (m_mapMgcfStatus.empty())
    {
        return false;
    }

    std::map<std::string, stSbcConn>::iterator it = m_mapMgcfStatus.begin();
    for (; it != m_mapMgcfStatus.end(); it++)
    {
        params.insert(make_pair(it->first, std::to_string(it->second.times)));
    }
    return true;
}
tars::Bool SipRelayImp::recvSipMsgFromCloudSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    TLOGINFO(std::string(__FUNCTION__) << "() called, sipGatewayId:" << sipGatewayId << ", connectionId:" << connectionId << ",sipMsg:\n" << sipMsg << endl);

    if (sipMsg.empty())
    {
        TLOGERROR(std::string(__FUNCTION__) << "() called, sipGatewayId:" << sipGatewayId << ", connectionId:" << connectionId << ",sipMsg:\n" << sipMsg << endl);
        return false;
    }

    //std::string mgcfIp;
    //int mgcfPort = 0;
    //SipMsgCommon::decodeToSBCConnectionId(connectionId, mgcfIp, mgcfPort);
    //if (mgcfIp == m_strLocalIp || mgcfIp == m_strLocalHost)
    //{
    //    __call->setReason("mgcfIp error!");
    //    SipRelayImp::recvSipMsgFromCloudSipGateway_end(__call, false);
    //    return;
    //}

    std::string sipDialogId, callId, reason;
    unsigned char ucMethod = 0;
    bool req = false;
    unsigned int ulStatusCode = 0;
    SipProxy::SipDialogRecord sipDialogRecord;

    if (SipMsg::getSipMsgSipDialogId(sipMsg, sipDialogId, callId, ucMethod, req, ulStatusCode))
    {
        if (ucMethod == EN_SIP_METHOD_INVITE && req)
        {
            insertSipDialogRecord(callId, sipDialogId, sipGatewayId, m_strMainObjId);
        }
    }

    if (!sendEncodedMessage(sipMsg, sipMsg.size(), connectionId, req, reason))
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called. sendEncodedMessage err, callId:" << callId << endl);
        return false;
    }
    return true;
}

// 定时调度函数 (原框架500ms调用一次)
void SipRelayImp::onSchd()
{
    int64_t now = TNOWMS;
    int64_t thisHour = now / (3600 * 1000);
    int64_t thisMinute = now / (60 * 1000);
    int64_t diffTime1 = (now - m_LastSchdTimeInMs);

    // if(diffTime1 < m_BaseSchdTimeInMs)
    //     return;

    //registerMgcfSipGatewayToResource();
    loadMgcfSipGatewayResources();
    loadSipGw3Resources();
    loadEnumproxyResources();
    
    // 检查是否需要重新注册和加载资源
    //m_bOutOfService = Application::isOutOfService();
    //if (!m_bOutOfService)
    //{
        // registerMgcfSipGatewayToResource();
        // loadMgcfSipGatewayResources();
        // loadSipGw3Resources();
        // loadEnumproxyResources();
    //}

    m_LastSchdTimeInMs = now;
    
    // 每分钟执行一次的任务
    if (thisMinute > m_lastMinute)
    {
        readSipGatewayAccessedConfigInfo();
        sendOptionToMgcf();
        m_lastMinute = thisMinute;
        m_remianMsgs.cleanExpiredRemains();
    }

    // 每小时执行一次的任务
    if (thisHour > m_lastHour)
    {
        m_sipDialogRecordManager.exceptionRecordDeals();
        m_lastHour = thisHour;
    }
}

// bool SipRelayImp::getMainService(Common::ObjectServerPtr& service,std::string& name,bool &famous)
// {
//     // service = this;
//     // name = SIP_GW2_SERVICE_NAME;
//     // famous = true;
//     return true;
// }

//void SipRelayImp::onUpdateConfigs()
//{
    // if (!m_bServiceActivated)
    // {
    //     return;
    // }

    // int tcpFlag = 0;
    // bool result = _application->getAppConfigAsInt(SIP_GW_START_UP_PARAMS_TCP_USED, tcpFlag);
    // if (result)
    // {
    //     m_tranProtoclName = tcpFlag > 0 ? "tcp" : "udp";
    //     m_bTcpProUsed = tcpFlag > 0 ? true : false;
    // }
    // else
    // {
    //     m_tranProtoclName = "udp";
    //     m_bTcpProUsed = false;
    // }

    // std::string dstReqIpPort;
    // result = _application->getAppConfig(SIP_GW_START_UP_PARAMS_DST_REQ_IP_PORT, dstReqIpPort);
    // if (dstReqIpPort.empty())
    //     m_destReqIpPort.clear();
    // else
    //     m_destReqIpPort = dstReqIpPort;

    // std::string dstRespIpPort;
    // result = _application->getAppConfig(SIP_GW_START_UP_PARAMS_DST_RESP_IP_PORT, dstRespIpPort);
    // if (dstRespIpPort.empty())
    //     m_destRespIpPort.clear();
    // else
    //     m_destRespIpPort = dstRespIpPort;

    // std::string entIp;
    // if(_application->getAppConfig(SIP_GW_START_UP_PARAMS_ENTERPRISE_IP, entIp))
    // {
    //     m_strEntIp = entIp;
    // }

    // std::string enterPrefix;
    // if (_application->getAppConfig(SIP_GW_START_UP_PARAMS_ENTERPRISE_PREFIX, enterPrefix))
    // {
    //     if (enterPrefix != m_strEnterprisePrefix)
    //     {
    //         std::lock_guard<std::mutex> lk(m_mutexEnterprisePrefix);
    //         m_strEnterprisePrefix = enterPrefix;
    //         SipCloudGateway::SipDlg::decodeKeyValPairsConfig5(enterPrefix, m_mapEnterprisePrefix);
    //         TLOGINFO(std::string(__FUNCTION__) << "() called. m_strEnterprisePrefix:" << m_strEnterprisePrefix << endl);
    //     }
    // }
    // else
    // {
    //     if (!m_strEnterprisePrefix.empty())
    //     {
    //         std::lock_guard<std::mutex> lc(m_mutexEnterprisePrefix);
    //         m_strEnterprisePrefix.clear();
    //         m_mapEnterprisePrefix.clear();
    //     }
    // }

    // SipGateway::TransProtocolType preType = m_transProType;
    // SipGateway::TransProtocolType nowType = m_bTcpProUsed ? SipGateway::kTransTCP : SipGateway::kTransUDP;
    // if (nowType != preType)
    // {
    //     std::lock_guard<std::mutex> lock(m_mutexTransProChange);
    //     if (m_transProChangeType == SipGateway::kTransChangeNone)
    //     {
    //         if (preType == SipGateway::kTransTCP && nowType == SipGateway::kTransUDP)
    //         {
    //             m_transProChangeType = SipGateway::kTransChangeTCP2UDP;
    //         }
    //         else
    //         {
    //             m_transProChangeType = SipGateway::kTransChangeUDP2TCP;
    //         }
    //         std::thread(ChangeTransProtocolThreadFunc, this).detach();
    //     }
    // }
//}

//bool SipRelayImp::isMainServiceReady(std::string& reason)
//{
//    //_mainAdapter->addServer(SIP_GW2_SERVICE_NAME_CLIENT, this, true);

//    return m_bServiceActivated;
//}
bool SipRelayImp::onSipMessageRequest(const SipGateway::SipRequestMethodType reqMethod, const SipProxy::SipRegisterSubscribePtr & ptr, void * pConnection)
{
    return true;
}

bool SipRelayImp::onSipMessageResponse(const SipGateway::SipRequestMethodType reqMethod, const SipGateway::SipResponseCodeType & rspCode, const SipProxy::SipRegisterSubscribePtr & ptr, void * pConnection, bool InvalidMsg)
{
    return true;
}

bool SipRelayImp::onOthersSipMessageTransfer(const std::string &strMsgBuffer,int iBufLen, const std::string & sipDialogId,
    const std::string & callId, bool bInitInvite, void * pConnection, bool exceptionConnection, const std::string & toMgcfConnectionId, bool bReq, unsigned char method,  const std::map<std::string, std::string> & others)
{
    std::string strConnectionId;
    SipGateway::GatewayConnection* rawPtr = static_cast<SipGateway::GatewayConnection*>(pConnection);
    SipGateway::GatewayConnectionPtr ptr = rawPtr ? rawPtr->shared_from_this() : nullptr;
    if (!ptr)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called. dynamicCast()  return FALSE! callId:" << callId << " ,msg:\n" << strMsgBuffer << endl);
        return false;
    }

    if (!exceptionConnection)
    {
        strConnectionId = ptr->getConnectionId();
        insertGatewayConnection(strConnectionId, ptr, sipDialogId);
    }

    if (method == EN_SIP_METHOD_OPTIONS)
    {
        if (bReq)
        {
            std::lock_guard<std::mutex> lock(m_mutexSipGwIds);
            if (m_vectSipGwIds.size() == 0)
            {
                TLOGWARN("no alive CloudSipGateway callId:" << callId << endl);
                return false;
            }

            std::string sipGatewayId = m_vectSipGwIds.at(getRandom(m_vectSipGwIds.size() - 1));

            sendSipMsgToCloudSipGateway(sipGatewayId, strConnectionId, callId, true, strMsgBuffer,toMgcfConnectionId, sipDialogId, others);
        }
        else
        {
            std::string connectId, remoteIp, remotePort;
            std::map<std::string, std::string>::const_iterator itor = others.find("Remote.Ip");
            if(itor != others.end())
            {
                remoteIp = itor->second;
            }
            itor = others.find("Remote.Port");
            if(itor != others.end())
            {
                remotePort = itor->second;
            }

            connectId = remoteIp + ":" + remotePort;
            std::lock_guard<std::mutex> lock(m_mutexMgcfStatus);
            map<std::string, stSbcConn>::iterator it = m_mapMgcfStatus.find(connectId);
            if (it != m_mapMgcfStatus.end())
            {
                it->second.times = 0;
                it->second.lastActive = TNOWMS;
            }
        }

        return true;
    }

    if (bInitInvite)
    {
        getRouterInfoFromJego(callId, strConnectionId, toMgcfConnectionId, strMsgBuffer, sipDialogId, pConnection, others);
    }
    else
    {
        SipProxy::SipDialogRecord sipDialogRecord;
        if (obtainSipDialogRecord(callId, sipDialogRecord))
        {
            sendSipMsgToCloudSipGateway(sipDialogRecord.m_sipGetwayId, strConnectionId, callId, true, strMsgBuffer,toMgcfConnectionId, sipDialogId, others);
        }
        else
        {
            TLOGWARN(std::string(__FUNCTION__) << "() called. obtainSipDialogRecord return FALSE! callId:" << callId << " ,msg:\n" << strMsgBuffer << endl);
            return false;
        }
    }

    return true;
}

void SipRelayImp::getRouterInfoFromJego(const std::string & callId, const std::string & strConnectionId, const std::string & toMgcfConnectionId, const std::string & msg, const std::string & sipDialogId,  void * pConnection, const std::map<std::string, std::string> & params)
{
    std::string callerSipUri, calleeSipUri, natIp, natPort, entIp, callForwarding;
    std::map<std::string, std::string>::const_iterator itorKey = params.find("Remote.Ip");
    if (itorKey != params.end())
    {
        natIp = itorKey->second;
    }
    itorKey = params.find("Remote.Port");
    if (itorKey != params.end())
    {
        natPort = itorKey->second;
    }

    getSipUri(true, callerSipUri, msg, natIp);
    getSipUri(false, calleeSipUri, msg, natIp);
    getCallForwarding(callForwarding, msg);

    class getRouterInfoFromJego_async : public VoipApp::SipRouterPrxCallback
    {
    public:
        getRouterInfoFromJego_async(SipRelayImp* ptr, const std::string & callId, const std::string & strConnectionId, const std::string & toMgcfConnectionId,
            const std::string & msg, const std::string & sipDialogId, const std::string & natIp, const std::string & natPort, void * pConnection)
            :_server(ptr),
            _callId(callId),
            _strConnectionId(strConnectionId),
            _toMgcfConnectionId(toMgcfConnectionId),
            _msg(msg),
            _sipDialogId(sipDialogId),
            _natIp(natIp),
            _natPort(natPort),
            _pConnection(pConnection)
        {
        }
        virtual void callback_getRouteInfo2(tars::Bool ret,  const VoipApp::JegoRouterInfo& info,  const map<std::string, std::string>& outParams)
        { 
            std::string sipGatewayId, errorReason;
            std::map<std::string, std::string> param;
            SipProxy::SipDialogRecord sipDialogRecord;
            if(ret)
            {
                bool bSipDialogIdExis = _server->checkCallIdHostExist(_callId, sipDialogRecord);
                if (bSipDialogIdExis)
                {
                    TLOGWARN("***** getRouterInfoFromJego() repeated responsed callId:" << _callId << endl);
                    return;
                }
                else
                {
                    _server->getCloudSipGatewayId(info, sipGatewayId);
                }

                std::string from, to, numberType;
                std::map<std::string, std::string>::const_iterator itor = outParams.find(SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_FROM);
                if (itor != outParams.end())
                {
                    from = itor->second;
                }
                itor = outParams.find(SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_TO);
                if (itor != outParams.end())
                {
                    to = itor->second;
                }
                itor = outParams.find(SipCloudGateway::SIP_GW_RPC_PARAMS_JEGO_NUMBER_TYPE_FLAG);
                if (itor != outParams.end())
                {
                    numberType = itor->second;
                }
                param["RESULT_CODE"] = TC_Common::tostr(info.resultCode);
                param["LAST_GW"] = info.gwAddr;
                param["CHARGE_USER"] = info.chargeAccount;
                param["DST_CALL_USER"] = info.callee;
                param["CALL_ID"] = info.callId;
                param["SRC_CALL_USER"] = info.caller;
                param["FROM"] = from;
                param["TO"] = to;
                param["DST_ENT_ADDR"] = info.enterpriseIp;
                param["DST_ENT_PORT"] = TC_Common::tostr(info.enterprisePort);
                param["CALLING_E164_NUMBER"] = info.number;
                param["Remote.Ip"] = _natIp;
                param["Remote.Port"] = _natPort;
                param["MIDDLE_CALL_USER"] = info.midCallUser;
                param["ORIG_SRC_CALL_USER"] = info.origSrcUser;
                param["ORIG_DST_CALL_USER"] = info.origDstUser;
                param["ROUTE_ID"] = info.routeId;
                param["ATOM_ID"] = info.atomId;
                param["PRICE_NUMBER"] = info.priceNumber;
                param["RECORD"] = TC_Common::tostr(info.record);
                param["BEEP"] = TC_Common::tostr(info.beep);
                param["SERVICE_TYPE"] = TC_Common::tostr(info.serviceType);
                param["RESULT_CODE"] = TC_Common::tostr(info.resultCode);
                param["FORCE_SIP_INBOUND"] = TC_Common::tostr(info.sipInbound);
                param["FORCE_SIP_OUTBOUND"] = TC_Common::tostr(info.sipOutbound);
                param["JEGO_ROUTE_LIST"] = TC_Common::tostr((int)info.routeInfoList.size());
                param["NUMBER_TYPE_FLAG"] =  numberType;

                std::string routeMsg;
                for (int i = 0; i < info.routeInfoList.size(); i++)
                {
                    std::string lastGw = "LAST_GW" + TC_Common::tostr(i);
                    std::string dstAddr = "DST_ENT_ADDR" + TC_Common::tostr(i);
                    std::string dstPort = "DST_ENT_PORT" + TC_Common::tostr(i);
                    VoipApp::RouteInfo route = info.routeInfoList.at(i);
                    param[lastGw] = route.gwAddr;
                    param[dstAddr] = route.ipAddr;
                    param[dstPort] = TC_Common::tostr(route.port);

                    routeMsg = route.gwAddr +":" + route.ipAddr + "-" + TC_Common::tostr(route.port) + "\n";
                }

                TLOGINFO("***** getRouterInfoFromJego(), NEW call, \n callId:" << _callId << "\n sipGatewayId:" << sipGatewayId << "\n gwAddr:" << info.gwAddr
                    << "\n caller:" << info.caller << "\n callee:" << info.callee << "\n from:" << from << "\n to:" << to << "\n chargeAccount:" << info.chargeAccount << "\n displayName:" << info.number
                    << "\n entIp:" << info.enterpriseIp << "\n entPort:" << info.enterprisePort << "\n middleUser:" << info.midCallUser << "\n originSrcUser:" << info.origSrcUser << "\n originDstUser:" << info.origDstUser
                    << "\n routeId:" << info.routeId << "\n atomId:" << info.atomId << "\n result:" << info.resultCode << "\n record:" << info.record << "\n numberType:" << numberType
                    << "\n serviceType:" << info.serviceType << "\n beep:" << info.beep << "\n routeMsg:" << routeMsg << endl);

                _server->insertSipDialogRecord2(_callId, _sipDialogId, sipGatewayId, _server->m_strMainObjId, _pConnection, false);
                _server->sendSipMsgToCloudSipGateway(sipGatewayId, _strConnectionId, _callId, true, _msg, _toMgcfConnectionId, _sipDialogId, param);

            }
            else
            {
                param["Remote.Ip"] = _natIp;
                param["Remote.Port"] = _natPort;

                {
                    std::lock_guard<std::mutex> lock(_server->m_mutexSipGwIds);
                    if (_server->m_vectSipGwIds.size() == 0)
                    {
                        TLOGWARN("getRouterInfoFromJego() called ERR, callId:" << _callId << ", reason: m_vectSipGwIds is 0 " << endl);
                        return;
                    }
                }

                sipGatewayId = _server->m_vectSipGwIds.at(getRandom(_server->m_vectSipGwIds.size() - 1));

                _server->sendSipMsgToCloudSipGateway(sipGatewayId, _strConnectionId, _callId, true, _msg, _toMgcfConnectionId, _sipDialogId, param);
                TLOGWARN("getRouterInfoFromJego() called ERR, callId:" << _callId << endl);
            }
                
        }
        virtual void callback_getRouteInfo2_exception(tars::Int32 ret)
        {
             // 网络、超时等框架层异常
            // std::string errorMsg;
            // switch(ret)
            // {
            //     case tars::TARSASYNCCALLTIMEOUT:
            //     case tars::TARSINVOKETIMEOUT:
            //         errorMsg = "Timeout";
            //         break;
            //     case tars::TARSPROXYCONNECTERR:
            //         errorMsg = "Connection error";
            //         break;
            //     case tars::TARSSERVERNOSERVENT:
            //         errorMsg = "Server not available";
            //         break;
            //     default:
            //         errorMsg = "Exception code: " + TC_Common::tostr(ret);
            //         break;
            // }
            
            TLOGERROR("getRouterInfoFromJego() exception, callId:" << _callId  << ", error:" << std::to_string(ret) << endl);
        }

    private:
        SipRelayImp* _server;
        const std::string          _callId;
        const std::string          _msg;
        const std::string          _strConnectionId;
        const std::string          _toMgcfConnectionId;
        const std::string          _sipDialogId;
        const std::string          _natIp;
        const std::string          _natPort;
        void *                     _pConnection;
    };

    std::lock_guard<std::mutex> lock(m_mutexEnumproxyIds);
    if (m_vectEnumproxyIds.empty())
    {
        TLOGWARN("getRouterInfoFromJego() called, not alive ENUMProxy callId:" << callId << endl);
    }
    std::map<std::string, std::string> rpcParams;
    rpcParams[SipCloudGateway::SIP_GW_RPC_PARAMS_INTERFACE_IS_NEW] = "1";
    m_routerPrx->async_getRouteInfo2(new getRouterInfoFromJego_async(this, callId, strConnectionId, toMgcfConnectionId, msg, sipDialogId, natIp, natPort, pConnection), m_strMainObjId, callerSipUri, calleeSipUri, callId, callForwarding, m_strLocalIp, natIp, rpcParams);
}

bool SipRelayImp::getSipUri(bool isCaller, std::string & sipUri, const std::string & sipMsg, std::string & enterIp)
{
    SipMsgDecodePtr pstDecode = std::make_shared<SipMsgDecode>();
    unsigned char ucResult = pstDecode->decode(sipMsg, sipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called,  pstDecode->decode() return FALSE.sipMsg:\n" << sipMsg << endl);
        return false;
    }

    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    int iPort = 0;

    if(pstDecode->getHdrFromToUri(isCaller, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        if (isCaller)
        {
            bool isAnonymous = sipUriIsAnonymous(strUserName);
            if(isAnonymous)
            {
                pstDecode->getHdrPAssertdedID(strUserName, strHost, iPort);
            }
        }
    }

    std::lock_guard<std::mutex> lock(m_mutexEnterprisePrefix);
    std::map<std::string, std::vector<VoipApp::enterPrefix>>::iterator itor = m_mapEnterprisePrefix.find(enterIp);

    if (itor != m_mapEnterprisePrefix.end())
    {
        int prefixBit = 0;
        std::string prefix;

        std::vector<VoipApp::enterPrefix> prefixInfo = itor->second;
        if (prefixInfo.size() < 1)
            return false;

        prefixBit = prefixInfo.at(0)._numberBit;
        if (prefixBit > 0)
        {
            prefix = prefixInfo.at(0)._prefix;
            strUserName = prefix + strUserName.substr(prefixBit);
        }

        sipUri = "tel:" + strUserName;
    }
    else
    {
        if (strUserName.compare(0, 2, "00") == 0)
        {
            sipUri = "tel:+" + strUserName.substr(2, strUserName.size() - 2);
        }
        else if (strUserName.compare(0, 1, "+") != 0)
        {
            sipUri = "tel:+" + strUserName;
        }
        else
        {
            sipUri = "tel:" + strUserName;
        }
    }

    return true;
}

bool SipRelayImp::getDiversionInfo(const std::string & diversion, std::string & sipuri)
{
    int pos1 = diversion.find("<");
    int pos2 = diversion.find(">");
    if (pos1 < 0 && pos2 < 0)
    {
        sipuri = diversion;
        return true;
    }

    std::string uri= diversion.substr(pos1 + 1, pos2 - pos1 - 1);
    int pos3 = uri.find(";");
    if (pos3 > 0)
    {
        sipuri = uri.substr(0, pos3);
    }
    else
    {
        sipuri = uri;
    }

    return true;
}

bool SipRelayImp::getCallForwarding(std::string & sipUri, const std::string & sipMsg)
{
    SipMsgDecodePtr pstDecode = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstDecode->decode(sipMsg, sipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called,  pstDecode->decode() return FALSE.sipMsg:\n" << sipMsg << endl);
        return false;
    }

    std::string diversion;
    if (pstDecode->getHdrExt(SIP_GW_CALL_FORWARDING_DIVERSION, diversion))
    {
        getDiversionInfo(diversion, sipUri);
        TLOGINFO(std::string(__FUNCTION__) << "() called, callForwording:" << sipUri << endl);
    }

    return true;
}

bool SipRelayImp::onOthersSipMessageTransferEx(const std::string & sipMsg, int iBufLen, const std::string & callIdHost, bool bReqInvite,unsigned char method, unsigned int resCode,
    const std::string & fromUsername, const std::string & toUserName, const std::string & registarIpName, void * pConnection, const std::string & toMgcfConectionId)
{
    return false;
}

bool SipRelayImp::saveExceptionConnection(void * pConnection)
{
    SipGateway::GatewayConnection* rawPtr = static_cast<SipGateway::GatewayConnection*>(pConnection);
    SipGateway::GatewayConnectionPtr ptr = rawPtr ? rawPtr->shared_from_this() : nullptr;
    if (!ptr)
    {
        return false;
    }
    if (ptr->getToSBCConnection())
        return false;

    return m_connectionCallIdManager.insertExceptionalConnection(ptr->getConnectionId(), ptr);
}

SipProxy::SipRegisterSubscribePtr SipRelayImp::getSipRegisterSubscribe(const std::string & sipUri, const std::string & callIdNum, SipGateway::SipRequestMethodType methodType)
{
    return 0;
}

std::string SipRelayImp::getSipProxyId()
{
    //return m_strMainObjId;
    return "";
}

bool SipRelayImp::getConnectionIdToSBC(const std::string & sipAccount, const std::string & registerHost, std::string & toMgcfConntionId)
{
    std::lock_guard<std::mutex> lock(m_mutexVectSipGateway);
    if(m_vectSipGatewayAccessedConfig.size() == 0)
        return false;

    for (unsigned int i = 0; i < m_vectSipGatewayAccessedConfig.size(); i++)
    {
        const VoipApp::SipProxyAccessedCoreNetConfig & domainCfg = m_vectSipGatewayAccessedConfig.at(i);
        if (domainCfg.m_strSBCIp == registerHost)
        {
            SipMsgCommon::generateToSBCConnectionId(domainCfg.m_strSBCIp, domainCfg.m_iSBCPort, toMgcfConntionId);
            return true;
        }
    }

    TLOGWARN(std::string(__FUNCTION__) << "() called. Access MGCF NOT exist the Config with: registerHost:" << registerHost << ", sipAccount:" << sipAccount << endl);

    return false;
}

bool SipRelayImp::checkCallIdHostExist(const std::string & callIdHost, SipProxy::SipDialogRecord & sipDialogRecord)
{
    return obtainSipDialogRecord(callIdHost, sipDialogRecord);
}

bool SipRelayImp::transferPingPongMsg(const std::string &strMsgBuffer,int iBufLen, bool isPing, void * pConnection)
{
    SipGateway::GatewayConnection* rawPtr = static_cast<SipGateway::GatewayConnection*>(pConnection);
    SipGateway::GatewayConnectionPtr ptr = rawPtr ? rawPtr->shared_from_this() : nullptr;
    if (!ptr)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called,dynamicCast() return FALSE! msg:\n" << strMsgBuffer << endl);
        return false;
    }

    if (isPing)
        return ptr->send((unsigned char*)SipMgcfGateway::SIP_GW_LINUX_PONG.c_str(), SipMgcfGateway::SIP_GW_LINUX_PONG.size());
    return true;
}

bool SipRelayImp::toSBCConnectionClosed(const std::string & connectionId, const std::string & localHost, int localPort, const std::string & remoteHost, int remotePort, const std::string & transProtocol)
{
    m_remianMsgs.cleanRemainMsg(connectionId);

    removeConnection(m_strToMgcfSipDialogId, connectionId);

    TLOGINFO(std::string(__FUNCTION__) << "() called, to be rebuild,connectionId: " << connectionId << ",localHost:"
        << localHost << ",port:" << localPort << ",remoteHost:" << remoteHost << ",port:" << remotePort << ",protocal:" << transProtocol << endl);

    std::string reason;
    int useSendPort = 0;
    int port = localPort;
    if (m_iSendPortChangeType == 1)
        port++;
    else if (m_iSendPortChangeType == 2)
        port = 0;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (m_connectionCallIdManager.checkConnectionIdExist(m_strToMgcfSipDialogId, connectionId))
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called, connection to Mgcf exist, id: " << connectionId << ",localHost:" << localHost << ",port:" << localPort << ",protocal:" << transProtocol << endl);
        return false;
    }
    else
    {
        SipGateway::GatewayConnectionPtr pConn = connectMgcfEx(transProtocol, localHost, port, remoteHost, remotePort, connectionId, useSendPort, reason);
        if (!pConn)
        {
            TLOGWARN(std::string(__FUNCTION__) << "() called, to be rebuild failed,connectionId: " << connectionId << ",reason:" << reason << endl);
            return false;
        }
    }


    return true;
}

// bool SipRelayImp::onSipMessageRequest2(const SipMessageGateway::SipMessagePtr & ptr, void * pConnection)
// {
//     return true;
// }

// bool SipRelayImp::onSipMessageResponse2(const std::string & msg, const SipGateway::SipRequestMethodType reqMethod, unsigned int rspCode,  const SipMessageGateway::SipMessagePtr & ptr, void * pConnection, bool InvalidMsg)
// {
//     return true;
// }

// SipMessageGateway::SipMessagePtr SipRelayImp::getSipMessage(const std::string & dstUri, const std::string & callIdHost)
// {
//     return 0;
// }

bool SipRelayImp::checkIpInWhitelist(const std::string & remoteIp)
{
    return true;
}

// void SipRelayImp::onProcEvent(const Common::ObjectPtr& event)
// {
//     SipGateway::MsgEventPtr msgEvent = SipGateway::MsgEventPtr::dynamicCast(event);
//     if(msgEvent == 0)
//     {
//         TLOGWARN(std::string(__FUNCTION__) << "() called. dynamicCast failed!" << endl);
//         return;
//     }
//     else
//     {
//         const std::string dataStr = msgEvent->_msg;
//         int dataLen = dataStr.size();
//         const std::string remoteHost = msgEvent->_remoteIp;
//         int remotePort = msgEvent->_remotePort;
//         const std::string connectionId = msgEvent->_connectionId;
//         SipGateway::GatewayConnectionPtr pSipConnection = msgEvent->_conn;

//         if (!pSipConnection)
//             return;

//         SipGateway::SipMsgDecodeResultType decodeResult = SipMsg::onReceiveSipMsgFromMGCF(dataStr, dataStr.size(), this, pSipConnection.get(), remoteHost, remotePort);
//         if (decodeResult != SipGateway::kSipMsgDecodeSuccessed)
//             TLOGWARN(std::string(__FUNCTION__) << "(), SipMsg::onReceiveSipMsgFromCloud() return FALSE.result:" << decodeResult << ",msg:\n" << dataStr << endl);

//     }
// }

bool SipRelayImp::sendSipMsgToCloudSipGateway(const std::string & sipGatewayId, const std::string & connectionId, const std::string & callId,  bool bInitInvite,
    const std::string & sipMsg, const std::string & toMgcfConnectionId, const std::string & sipDialogId, const std::map<std::string, std::string> & params)
{
    TLOGDEBUG("SipRelayImp::sendSipMsgToCloudSipGateway()  _sipDialogId:" << sipDialogId << ", connectionId:" << connectionId << ", toMgcfConnectionId:" << toMgcfConnectionId << ", bInitInvite:" << bInitInvite << endl);

    class RecvSipMsgFromMgcfSipGateway_async : public VoipApp::SipControllerPrxCallback
    {
    public:
        RecvSipMsgFromMgcfSipGateway_async(const std::string & connectionId, const  std::string & callId, const std::string & sipMsg,
            const std::string & toMgcfConnectionId, bool bInitInvite, SipRelayImp* MgcfSipGatewayI,const std::string  & sipDialogId)
            :_connectionId(connectionId), _callId(callId), _sipMsg(sipMsg) ,_toMgcfConnectionId(toMgcfConnectionId)
            ,_bInitInvite(bInitInvite) ,_MgcfSipGatewayIPtr(MgcfSipGatewayI), _sipDialogId(sipDialogId){}

        virtual void callback_recvSipMsgFromMgcfSipGateway(tars::Bool ret)
        { 
            TLOGINFO("callback_recvSipMsgFromMgcfSipGateway _sipDialogId:" << _sipDialogId << " ,ret :" << std::to_string(ret) << " ,_sipMsg:\n " << _sipMsg << endl);
            if (_bInitInvite)
            {
                _MgcfSipGatewayIPtr->ereaseSipDialogRecordAndConnection(_sipDialogId);
            }
        }
        virtual void callback_recvSipMsgFromMgcfSipGateway_exception(tars::Int32 ret)
        { 
            TLOGWARN("callback_recvSipMsgFromMgcfSipGateway_exception _sipDialogId:" << _sipDialogId << " ,ret :" << std::to_string(ret) << " ,_sipMsg:\n " << _sipMsg << endl);
        }

    private:
        std::string              _connectionId;
        std::string              _callId;
        std::string              _sipMsg;
        std::string              _toMgcfConnectionId;
        bool                        _bInitInvite;
        SipRelayImp*    _MgcfSipGatewayIPtr;
        const std::string         _sipDialogId;
    };

    VoipApp::SipControllerPrx prx = Application::getCommunicator()->stringToProxy<VoipApp::SipControllerPrx>(sipGatewayId);
    prx->async_recvSipMsgFromMgcfSipGateway(new RecvSipMsgFromMgcfSipGateway_async(connectionId, callId, sipMsg, toMgcfConnectionId, bInitInvite, this, sipDialogId),
        m_strMainObjId, sipMsg, toMgcfConnectionId, params);

    return true;
}

bool SipRelayImp::sendEncodedMessage(const std::string & strMsg, int iSize, const std::string & connectionId,  bool req, std::string & reason)
{
    reason.clear();
    std::string dstConId(connectionId);
    if (req && false == m_destReqIpPort.empty())
        dstConId = m_destReqIpPort;
    else if (false == req && false == m_destRespIpPort.empty())
        dstConId = m_destRespIpPort;

    bool reBuildCon = false;
    SipGateway::GatewayConnectionPtr pConn = 0;
    std::string tips;

    do
    {
        if (dstConId.empty())
        {
            reason = "dstConId is EMPTY.";
            break;
        }
        pConn = m_connectionCallIdManager.getConnection(m_strToMgcfSipDialogId, dstConId);
        if (pConn == 0)
        {
            tips = "GetConnection() return FALSE.";
            reBuildCon = true;
            break;
        }

        if(pConn->checkSendValid() == true)
        {
            return pConn->send((unsigned char*)strMsg.c_str(), strMsg.size());
        }
        else
        {
            tips = "the sender of Connection is NULL.";
            reBuildCon = true;
            break;
        }
    } while (0);

    if (reBuildCon)
    {
        removeConnection(m_strToMgcfSipDialogId, dstConId);
            TLOGWARN(std::string(__FUNCTION__) << "() called, " << tips << ",to be rebuild,dstConId: " << dstConId << ",SipMSg:\n" << strMsg << endl);
        std::string mgcfIp;
        int mgcfPort = 0;
        if (SipMsgCommon::decodeToSBCConnectionId(dstConId, mgcfIp, mgcfPort) == false)
        {
            reason = "decode dstConId failed, id:" + dstConId + ",mgcfIp" + mgcfIp + ",mgcfPort" + TC_Common::tostr(mgcfPort);
        }
        else
        {
            std::string localHost;
            std::vector<VoipApp::SipProxyAccessedCoreNetConfig> sipGatewayCfgs;
            getAllSipGatewayCfgs(sipGatewayCfgs);
            for (unsigned int i = 0; i < sipGatewayCfgs.size(); i++)
            {
                VoipApp::SipProxyAccessedCoreNetConfig cfg = sipGatewayCfgs.at(i);
                if (cfg.m_strSBCIp == mgcfIp && cfg.m_iSBCPort == mgcfPort && TC_Common::trim(cfg.m_strSipProxyName) == m_strAppName)
                {
                    localHost = cfg.m_sendIp;
                    break;
                }
            }

            return newAConnectionToMgcf(localHost, mgcfIp, mgcfPort, strMsg, true, reason);
        }
    }

    if (reason.empty() == false)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() return FALSE, reason:" << reason << ",SipMSg:\n" << strMsg << endl);
        return false;
    }
    return true;
}

bool SipRelayImp::sendEncodedMessage2(const std::string & strMsg, int iSize, void * pConnection, std::string & reason)
{
    return true;
}

bool SipRelayImp::sendEncodedMessage(const std::string & strMsg, int iSize, bool bRequest, SipGateway::SipRequestMethodType reqMethod, const SipGateway::SipDialogPtr & sipDlg)
{
    std::string sipDialogId = sipDlg->id();
    std::string connectionId = sipDlg->getToSBCConnectionId();
    std::string reason;

    bool ret = sendEncodedMessage(strMsg, iSize, connectionId,  true,  reason);
    if (!ret)
        TLOGWARN(std::string(__FUNCTION__) << "() called, sipDialogId:" << sipDialogId << ",reason:" << reason << endl);

    return ret;
}

Common::NetReceiverPtr SipRelayImp::recvConnection(const Common::NetSenderPtr& sender)
{
    std::string strRemoteHost, connectId, strLocalHost;
    int iRemotePort = 0, iLocalPort = 0;
    sender->getRemote(strRemoteHost, iRemotePort);
    connectId = strRemoteHost + ":" + TC_Common::tostr(iRemotePort);

    sender->getLocal(strLocalHost, iLocalPort);

    SipGateway::GatewayConnectionPtr pConnec = m_connectionCallIdManager.getConnection(m_strToMgcfSipDialogId, connectId);
    if (pConnec)
    {
        TLOGINFO(std::string(__FUNCTION__) << "() called, remove connect, Remote Ip:" << strRemoteHost << ", Remote port: " << iRemotePort << ", local ip:" << strLocalHost << ", local port:" << iLocalPort << endl);
        m_connectionCallIdManager.removeConnection(m_strToMgcfSipDialogId, connectId);
    }

    SipProxy::SipProxyMsgCoderInterfacePtr decoder(this, [](SipProxy::SipProxyMsgCoderInterface*){});
    pConnec = std::shared_ptr<SipGateway::GatewayConnection>(new SipGateway::GatewayConnection(decoder, SipGateway::MgcfSipGatewayModul, m_tranProtoclName, "", iLocalPort, strRemoteHost, iRemotePort, connectId));
    pConnec->setSender(sender);
    pConnec->setConnectionId(connectId);
    pConnec->setToSBCConnection(true);
    m_connectionCallIdManager.insertConnection(m_strToMgcfSipDialogId, connectId, pConnec);

    TLOGINFO(std::string(__FUNCTION__) << "() called, new a connection connectId, Remote Ip:" << strRemoteHost << ", Remote port: " << iRemotePort << ", local ip:" << strLocalHost << ", local port:" << iLocalPort << endl);

    return pConnec;
}

void SipRelayImp::onConnReady()
{
}

void SipRelayImp::onConnClose()
{
}

bool SipRelayImp::insertGatewayConnection(const std::string & strConnectionId,  const SipGateway::GatewayConnectionPtr & pConnection, const std::string & regOrSipDialogId)
{
    return m_connectionCallIdManager.insertConnection(regOrSipDialogId, strConnectionId, pConnection);
}

// bool SipRelayImp::removeGatewayConnection(const SipMessageGateway::SipMessagePtr & ptr)
// {
//     if (ptr == 0)
//         return false;
//     return m_connectionCallIdManager.removeAllConnectionsOfSipDialog(ptr->getMessageId());
// }

bool SipRelayImp::removeConnection(const std::string & sipdialogId, const std::string & strConnectionId )
{
    return m_connectionCallIdManager.removeConnection(sipdialogId, strConnectionId);
}

// bool SipRelayImp::getMasterTrdDown()
// {
//     //std::lock_guard<std::mutex> lock(this);
//     return m_bMasterThdDown;
// }

// bool SipRelayImp::getOutOfService()
// {
//     return m_bOutOfService;
// }

// void SipRelayImp::changeTransProtocol()
// {
//     if (m_iLocalListenPort < 0 || m_tranProtoclName.empty())
//     {
//         TLOGWARN(std::string(__FUNCTION__) << "() called.service not ready, m_iLocalListenPort:" << m_iLocalListenPort << ",m_tranProtoclName:" << m_tranProtoclName << endl);
//         return;
//     }

//     SipGateway::TransProtocolType preType = m_transProType;
//     SipGateway::TransProtocolType newTransPro;
//     std::string preProtocolName = m_transProType == SipGateway::kTransUDP ? "udp" : "tcp";
//     {
//         std::lock_guard<std::mutex> lock(m_mutexTransProChange);
//         if (m_transProChangeType == SipGateway::kTransChangeNone)
//         {
//             TLOGWARN(std::string(__FUNCTION__) << "() called. transport protocol no changes!" << endl);
//             return;
//         }
//         else
//         {
//             if (SipGateway::kTransChangeTCP2UDP == m_transProChangeType)
//             {
//                 newTransPro = SipGateway::kTransUDP;
//                 m_tranProtoclName = "udp";
//             }
//             else
//             {
//                 newTransPro = SipGateway::kTransTCP;
//                 m_tranProtoclName = "tcp";
//             }
//             m_transProType = newTransPro;
//             m_transProChangeType = SipGateway::kTransChangeNone;
//         }
//     }

//     TLOGINFO(std::string(__FUNCTION__) << "() called. pre protocol :" << preProtocolName << ",to switch new protocol:" << m_tranProtoclName << ",m_iLocalListenPort:" << m_iLocalListenPort << endl);

//     buildConnectionToMgcfs(true);

//     if (m_Listener)
//     {
//         m_Listener->close();
//     }

//     m_Listener = _application->getDriver()->listen(m_tranProtoclName, "", m_iLocalListenPort, this, true);
//     if(m_Listener == 0)
//     {
//         TLOGERROR(std::string(__FUNCTION__) << "() called . Listen failed, m_strLocalIp="
//             <<  m_strLocalIp << ",m_iLocalListenPort:" << m_iLocalListenPort << ",protocol:" << m_tranProtoclName << endl);
//         return;
//     }

//     std::string localHost;
//     int localPort;
//     m_Listener->getLocal(localHost, localPort);
//     TLOGINFO(std::string(__FUNCTION__) << "() called ***,======> Listening ... listenIp: [" << localHost << ":" << localPort << "]" << ",protocol:" << m_tranProtoclName << endl);
// }

bool SipRelayImp::buildConnectionToMgcfs(bool transProtocalChanged)
{
    std::vector<VoipApp::SipProxyAccessedCoreNetConfig> sipGatewayCfgs;
    getAllSipGatewayCfgs(sipGatewayCfgs);
    if (sipGatewayCfgs.size() == 0)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called, config is empty." << endl);
        return false;
    }

    if (m_iLocalSendPort < 0 || m_tranProtoclName.empty())
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called, m_iLocalSendPort:" << m_iLocalSendPort << ",m_tranProtoclName:" << m_tranProtoclName << endl);
        return false;
    }

    for (unsigned int  i = 0; i < sipGatewayCfgs.size(); i++)
    {
        VoipApp::SipProxyAccessedCoreNetConfig & temConfig = sipGatewayCfgs.at(i);
        if (TC_Common::trim(temConfig.m_strSipProxyName) == m_strAppName)
        {
            std::string strConnecntionId;
            SipMsgCommon::generateToSBCConnectionId(temConfig.m_strSBCIp, temConfig.m_iSBCPort, strConnecntionId);

            buildConnectionToMgcf(strConnecntionId, transProtocalChanged, m_tranProtoclName, temConfig.m_sendIp, m_iLocalSendPort, temConfig.m_strSBCIp, temConfig.m_iSBCPort);
        }
    }

    return true;
}

bool SipRelayImp::buildConnectionToMgcf(const std::string & strConnecntionId, bool transProtocalChanged, const std::string& protocol,const std::string& localHost,int localPort,const std::string& mgcfIp,int mgcfPort)
{
    SipGateway::GatewayConnectionPtr pCon = m_connectionCallIdManager.getConnection(m_strToMgcfSipDialogId, strConnecntionId);
    if (pCon)
    {
        int sendPort = pCon->getLocalPort();
        if (localPort != sendPort || transProtocalChanged)
        {
            m_connectionCallIdManager.removeConnection(m_strToMgcfSipDialogId, strConnecntionId);
            TLOGINFO(std::string(__FUNCTION__) << "() called,to build connection:" << strConnecntionId << ",previous send port:"
                << sendPort << ",new port:" << localPort << ",change protocal:" << transProtocalChanged << endl);
            pCon = 0;
        }
        else
        {
            TLOGDEBUG(std::string(__FUNCTION__) << "() called, checkConnectionIdExist() return TRUE :" << strConnecntionId << endl);
            return true;
        }
    }

    std::string reason;
    int usedSendPort = 0;
    SipGateway::GatewayConnectionPtr pConnec = connectMgcfEx(m_tranProtoclName, localHost, localPort, mgcfIp, mgcfPort, strConnecntionId,usedSendPort, reason);
    if (!pConnec)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called,connect Mgcf failed, connectionId:" << strConnecntionId << ",protocol changed:"
            << transProtocalChanged << ",protocol:" << m_tranProtoclName << ",reason:" << reason << endl);
        return false;;
    }

    TLOGINFO(std::string(__FUNCTION__) << "() called. Insert a Connection. ConnectionId :" << strConnecntionId << ",regOrsipdialog id: "
        << m_strToMgcfSipDialogId << ",protocol changed:" << transProtocalChanged << ",protocol:" << m_tranProtoclName << endl);

    return true;
}

SipGateway::GatewayConnectionPtr SipRelayImp::connectMgcfEx(const std::string& protocol,const std::string& localHost,int localPort,const std::string& mgcfIp,int mgcfPort,
    const std::string & strConnecntionId, int & useSendPort, std::string & reason)
{
    SipGateway::GatewayConnectionPtr pConnec = connectMgcf(protocol, localHost, localPort, mgcfIp, mgcfPort, strConnecntionId, useSendPort, reason);
    if (!pConnec)
    {
        if (m_iSendPortChangeType == 1)
        {
            for (unsigned int i = 0; i < SipMgcfGateway::MAX_SEND_PORT_RETRY_TIMES; i++)
            {
                int port = localPort + i > 65000 ? 0 : localPort + i;
                pConnec = connectMgcf(protocol, localHost, port, mgcfIp, mgcfPort, strConnecntionId, useSendPort, reason);
                if (pConnec)
                {
                    break;
                }
            }
            TLOGWARN(std::string(__FUNCTION__) << "() called,connect Cloud failed for 20 times, connectionId:" << strConnecntionId << ",protocol:" << protocol << ",reason:" << reason << endl);
        }
        else
        {
            pConnec = connectMgcf(protocol, localHost, 0, mgcfIp, mgcfPort, strConnecntionId, useSendPort, reason);
            if (!pConnec)
            {
                TLOGWARN(std::string(__FUNCTION__) << "() called,connect Cloud failed, connectionId:" << strConnecntionId << ",protocol:" << protocol << ",reason:" << reason << endl);
                return 0;
            }
        }
    }

    if (pConnec)
    {
        m_iCurSendPort = useSendPort;
        m_connectionCallIdManager.insertToSBCConnectionId(strConnecntionId);
        if (!m_connectionCallIdManager.insertConnection(m_strToMgcfSipDialogId, strConnecntionId, pConnec))
        {
            TLOGWARN(std::string(__FUNCTION__) << "() called, m_connectionCallIdManager.InsertConnection() return FALSE. ConnectionId :"
                << strConnecntionId << ",regOrsipdialog id" << m_strToMgcfSipDialogId << ",protocol:" << m_tranProtoclName << endl);
        }
    }

    return pConnec;
}

SipGateway::GatewayConnectionPtr SipRelayImp::connectMgcf(const std::string& protocol,const std::string& localHost,int localPort,const std::string& mgcfIp,int mgcfPort,
    const std::string & strConnecntionId, int & usedLocalPort, std::string & reason)
{
    SipProxy::SipProxyMsgCoderInterfacePtr decoder(this, [](SipProxy::SipProxyMsgCoderInterface*){});
    SipGateway::GatewayConnectionPtr pConnec = std::shared_ptr<SipGateway::GatewayConnection>(new SipGateway::GatewayConnection(decoder, SipGateway::MgcfSipGatewayModul, protocol, localHost, localPort, mgcfIp, mgcfPort, strConnecntionId));
    //todo
    Common::NetSenderPtr psender; // = _application->getDriver()->connect(protocol, localHost, localPort, mgcfIp, mgcfPort, pConnec, true);

    if (psender == 0)
    {
        reason = "pConnec->_sender == 0,errno:" + TC_Common::tostr(errno);
        TLOGWARN(std::string(__FUNCTION__) << "() called, pConnec->_sender == 0. ConnectionId :" << strConnecntionId << ",localPort:" << localPort << endl);
        pConnec->clearUp();
        pConnec = 0;
        return 0;
    }

    pConnec->setSender(psender);
    pConnec->setConnectionId(strConnecntionId);
    pConnec->setToSBCConnection(true);

    std::string strRemoteHost,strLocalHost;
    int iRemotePort, iLocalPort = 0;
    psender->getRemote(strRemoteHost,iRemotePort);
    psender->getLocal(strLocalHost, iLocalPort);
    m_strLocalHost = strLocalHost;
    usedLocalPort = iLocalPort;
    pConnec->setConnectionInfos(localHost, localPort, mgcfIp, mgcfPort, protocol);

    TLOGINFO(std::string(__FUNCTION__) << "() called, a conntion built, protocal:" << protocol << ", input host:" << localHost << ",local host:" << strLocalHost << ",port:"
        << iLocalPort << ",remote host:" << strRemoteHost << ",port:" << iRemotePort << endl);
    return pConnec;
}

bool SipRelayImp::newAConnectionToMgcf(const std::string & localHost,const std::string & mgcfIp, const int & mgcfPort, const std::string & msg, bool bLocalPort, std::string & reason)
{
    std::string strConnecntionId;
    int usedSendPort = 0;
    SipMsgCommon::generateToSBCConnectionId(mgcfIp, mgcfPort, strConnecntionId);
    SipGateway::GatewayConnectionPtr pConnec = connectMgcfEx(m_tranProtoclName, localHost, m_iLocalSendPort, mgcfIp, mgcfPort, strConnecntionId, usedSendPort, reason);
    if (!pConnec)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called,connect mgcf failed, connectionId:" << strConnecntionId << ",protocol:" << m_tranProtoclName << ",reason:" << reason << endl);
        return false;
    }

    if(pConnec->checkSendValid() == true)
    {
        pConnec->send((unsigned char*)msg.c_str(), msg.size());
    }
    else
    {
        reason = std::string(__FUNCTION__) + "() called.checkSendValid() return FALSE. strConnecntionId:" + strConnecntionId;
        TLOGWARN("reason :" << reason << endl);
        return false;
    }

    TLOGINFO(std::string(__FUNCTION__) << "() called,send msg and Insert a Connection. ConnectionId :" << strConnecntionId << ",regOrsipdialog id: " << m_strToMgcfSipDialogId << endl);
    return true;
}

void SipRelayImp::readSipGatewayAccessedConfigInfo()
{
    class GetSipGatewayAccessInfo_async : public VoipApp::SipDbPrxCallback
    {
    public:
        GetSipGatewayAccessInfo_async(SipRelayImp* MgcfSipGatewayPtr)
            :_MgcfSipGatewayPtr(MgcfSipGatewayPtr){}

        virtual void callback_GetSipProxyAccessCoreNetInfo(tars::Bool ret,  const vector<VoipApp::SipProxyAccessedCoreNetConfig>& vectSipProxyAccessedConfig,  const std::string& reason)
        {
            if(!ret)
            {
                TLOGWARN("readSipGatewayAccessedConfigInfo() called. GetSipProxyAccessCoreNetInfo_end() return FALSE! reason : " << reason << endl);
                return;
            }
            TLOGDEBUG("readSipGatewayAccessedConfigInfo() called. GetSipProxyAccessCoreNetInfo_end() return TRUE! vectSipProxyAccessedConfig SIZE : " << vectSipProxyAccessedConfig.size() << endl);

            _MgcfSipGatewayPtr->onReadSipGatewayAccessedConfigInfo(vectSipProxyAccessedConfig);
        }

        virtual void callback_GetSipProxyAccessCoreNetInfo_exception(tars::Int32 ret)
        {
            TLOGERROR("GetSipProxyAccessCoreNetInfo exception, ret:" << ret << endl);
        }
    private:
        SipRelayImp* _MgcfSipGatewayPtr;
    };

    m_sipProxyDbAgent->async_GetSipProxyAccessCoreNetInfo(new GetSipGatewayAccessInfo_async(this), "sipproxy_config");
}

bool SipRelayImp::onReadSipGatewayAccessedConfigInfo(const std::vector<VoipApp::SipProxyAccessedCoreNetConfig> & vectSipGatewayConfigInfo)
{
    if (vectSipGatewayConfigInfo.size() == 0)
    {
        return true;
    }

    bool bConfigured = false;
    VoipApp::SipProxyAccessedCoreNetConfig sipProxyConfig, tmpConfig;
    vector<VoipApp::SipProxyAccessedCoreNetConfig> vectConfigs;
    set<std::string> mgcfIps;

    for (unsigned int i = 0; i < vectSipGatewayConfigInfo.size(); i++)
    {
        tmpConfig = vectSipGatewayConfigInfo.at(i);
        if (m_strMainObjId == tmpConfig.m_strSipProxyId && tmpConfig.m_strSipProxyName == m_strAppName)
        {
            sipProxyConfig = tmpConfig;
            bConfigured = true;
            vectConfigs.push_back(sipProxyConfig);
            mgcfIps.insert(sipProxyConfig.m_strSBCIp);
        }
    }

    {
        std::lock_guard<std::mutex> lock(m_mutexMgcfIps);
        mgcfIps.swap(m_setMgcfIps);
    }

    if (!bConfigured)
    {
        m_bServiceActivated = false;
        TLOGERROR(std::string(__FUNCTION__) << "() called. this MgcfSipGateway Not Configured! m_strMainObjId:" << m_strMainObjId << endl);
        return false;
    }

    if (vectConfigs.size() > 1)
    {
        int temListenPort = vectConfigs.at(0).m_listenPort;
        int temSendPort = vectConfigs.at(0).m_sendPort;
        std::string temListenIp = vectConfigs.at(0).m_listenIp;
        for (unsigned int i = 0; i < vectConfigs.size(); i++)
        {
            VoipApp::SipProxyAccessedCoreNetConfig temConfig = vectConfigs.at(i);
            if (temConfig.m_listenPort != temListenPort)
            {
                TLOGERROR(std::string(__FUNCTION__) << "() called. Listen Port in Configured Differs!" << endl);
                return false;
            }
            if (temConfig.m_sendPort != temSendPort)
            {
                TLOGERROR(std::string(__FUNCTION__) << "() called. Send Port in Configured Differs!" << endl);
                return false;
            }

            if (temConfig.m_listenIp != temListenIp)
            {
                TLOGERROR(std::string(__FUNCTION__) << "() called. Listen IP in Configured Differs!" << endl);
                return false;
            }
        }
    }

    {
        std::lock_guard<std::mutex> temLock(m_mutexVectSipGateway);
        m_vectSipGatewayAccessedConfig.swap(vectConfigs);
    }

    int iLocalListenPort = sipProxyConfig.m_listenPort;
    int iLocalSendPort = sipProxyConfig.m_sendPort;
    std::string listenIp = sipProxyConfig.m_listenIp;
    bool proxyChed = false;

    if (m_tranProtoclName.empty() || iLocalListenPort == 0)
    {
        TLOGWARN(std::string(__FUNCTION__) << "() called, m_tranProtoclName is empty." << endl);
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutexListen);
    if (m_iLocalListenPort != iLocalListenPort)
    {
        if(m_Listener)
            m_Listener->close();

        // m_Listener = _application->getDriver()->listen(m_tranProtoclName, "", iLocalListenPort, this, true);
        if(m_Listener == 0)
        {
            TLOGERROR(std::string(__FUNCTION__) << "(), Listen failed, Port change" << iLocalListenPort << ", m_iLocalListenPort:" << m_iLocalListenPort << ",protocol:" << m_tranProtoclName << endl);
            return false;
        }

        TLOGINFO(std::string(__FUNCTION__) << "(), Listen Port change" << iLocalListenPort << ", m_iLocalListenPort:" << m_iLocalListenPort << ",protocol:" << m_tranProtoclName << endl);

        m_iLocalListenPort = iLocalListenPort;
    }

    if (listenIp.empty() == false)
    {
        if(listenIp != m_strLocalIp)
        {
            proxyChed = true;
            m_strLocalIp = listenIp;
            TLOGINFO("*** read MgcfSipGatewayI config ***,======> Listening ip:" << m_strLocalIp << endl);
        }
    }

    if (m_strSendIp != sipProxyConfig.m_sendIp)
    {
        proxyChed = true;
        m_strSendIp = sipProxyConfig.m_sendIp;
        TLOGINFO("*** read MgcfSipGatewayI config ***,======> send ip:" << m_strSendIp << endl);
    }
    m_bServiceActivated = true;
    //sendPort
    if (iLocalSendPort > 0)
    {
        if (iLocalSendPort != m_iLocalSendPort)
        {
            proxyChed = true;
            m_iLocalSendPort = iLocalSendPort;
            TLOGINFO("*** read MgcfSipGatewayI config ***, SendPort: " << m_iLocalSendPort << endl);
        }
    }
    else
    {
        TLOGWARN("======> read MgcfSipGatewayI config, SendPort:" << iLocalSendPort << endl);
    }

    return true;
}

bool SipRelayImp::loadEnumproxyResources()
{
    // 构造查询条件
    VoipApp::QueryCondition condition;
    condition.serviceType = "SipRouter";
    // 可选：按数据中心过滤
    // condition.attributes["dataCenter"] = m_strDataCenter;

    //异步查询所有SipRelay服务
    class QueryCallback : public VoipApp::SipRegPrxCallback
    {
    public:
        QueryCallback(SipRelayImp* impl) : m_impl(impl) {}

        virtual void callback_queryServices(tars::Int32 ret, const std::vector<VoipApp::ServiceInfo>& services)
        {
            if (ret == 0)
            {
                TLOGINFO("Query success, found " << services.size() << " SipRelay services" << endl);
                m_impl->onResultLoadEnumproxyResources(services);
            }
        }

        virtual void callback_queryServices_exception(tars::Int32 ret) {
            TLOGERROR("Query services exception, ret:" << ret << endl);
        }
    private:
        SipRelayImp* m_impl;
    };

    m_registryPrx->async_queryServices(new QueryCallback(this), condition);

    TLOGDEBUG("Query all SipRelay services from Registry" << endl);
    return true;
}

bool SipRelayImp::onResultLoadEnumproxyResources(const std::vector<VoipApp::ServiceInfo> & vectResources)
{
    if(vectResources.empty())
        return false;

    std::lock_guard<std::mutex> lock(m_mutexEnumproxyIds);
    m_vectEnumproxyIds.clear();

    std::string strIds = "ids:\n";

    for (unsigned int i = 0; i < vectResources.size(); i++)
    {
        VoipApp::ServiceInfo resource = vectResources.at(i);
        m_vectEnumproxyIds.push_back(resource.serviceId);
        strIds += resource.serviceId + "\n";
    }
    TLOGDEBUG(std::string(__FUNCTION__) << "ids:" << strIds << endl);

    return true;
}

bool SipRelayImp::insertSipDialogRecord(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId)
{
    return m_sipDialogRecordManager.insertSipDialogRecord(callId, sipDialogId, sipGatewayId, sipProxyId);
}

bool SipRelayImp::insertSipDialogRecord2(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId,void * respCon, bool appCallCs)
{
    return m_sipDialogRecordManager.insertSipDialogRecord2(callId, sipDialogId, sipGatewayId, sipProxyId, respCon, appCallCs);
}

bool SipRelayImp::ereaseSipDialogRecord(const std::string & callId)
{
    TLOGINFO(std::string(__FUNCTION__) << "() called.erase the SipDialogId in the Set, callId :" << callId << endl);
    return m_sipDialogRecordManager.ereaseSipDialogRecord(callId);
}

bool SipRelayImp::obtainSipDialogRecord(const std::string & callId, SipProxy::SipDialogRecord & sipDialogRecord)
{
    return m_sipDialogRecordManager.obtainSipDialogRecord(callId, sipDialogRecord);
}

bool SipRelayImp::updateSipDialogRecordGwId(const std::string & callId, const std::string & sipGatewayObjId)
{
    TLOGDEBUG("updateSipDialogRecordGwId() called.to update the sipGateway obj id of this call ,callId: " << callId << ",sipGatewayObjId:" << sipGatewayObjId << endl);
    return m_sipDialogRecordManager.updateSipGatewayId(callId, sipGatewayObjId);
}

bool SipRelayImp::ereaseSipDialogRecordAndConnection(const std::string & sipDialogId)
{
    TLOGINFO(std::string(__FUNCTION__) << "() called, to be Removed SipDialog Id :" << sipDialogId << endl);
    ereaseSipDialogRecordBySipDialogID(sipDialogId);
    return m_connectionCallIdManager.removeAllConnectionsOfSipDialog(sipDialogId);
}

bool SipRelayImp::ereaseSipDialogRecordBySipDialogID(const std::string & sipDialogId)
{
    TLOGDEBUG(std::string(__FUNCTION__) << "() called. erase the SipDialogId in the Set, sipDialogId :" << sipDialogId << endl);
    return m_sipDialogRecordManager.ereaseSipDialogRecordBySipDialogId(sipDialogId);
}

void SipRelayImp::getAllSipGatewayCfgs(std::vector<VoipApp::SipProxyAccessedCoreNetConfig> & cfgs)
{
    std::lock_guard<std::mutex> lock(m_mutexVectSipGateway);
    cfgs = m_vectSipGatewayAccessedConfig;
}

int SipRelayImp::getMgcfSipGatewayCfgsSize()
{
    std::lock_guard<std::mutex> lock(m_mutexVectSipGateway);
    return m_vectSipGatewayAccessedConfig.size();
}

bool SipRelayImp::getSipGatewayIdByRouterIp(const std::string &routerIp, std::string &sipGatewayId, std::string &sipGatewayIp, int &sipGatewayPort, std::string &mgcfIp, int & mgcfPort)
{
    if(m_vectSipGatewayAccessedConfig.size() == 0)
        return false;

    for (unsigned int i = 0; i < m_vectSipGatewayAccessedConfig.size(); i++)
    {
        VoipApp::SipProxyAccessedCoreNetConfig config = m_vectSipGatewayAccessedConfig.at(i);
        if(config.m_strSBCIp == routerIp)
        {
            sipGatewayId = config.m_strSipProxyId;
            sipGatewayIp = config.m_listenIp;
            sipGatewayPort = config.m_listenPort;
            mgcfIp = config.m_strSBCIp;
            mgcfPort = config.m_iSBCPort;
            return true;
        }
    }

    return false;
}

bool SipRelayImp::getCallerAndCalleeSipUri(std::string & caller, std::string & callee, const std::string & sipMsg)
{
    return SipMsg::getCallerAndCalleeSipUri(caller, callee, sipMsg);
}

void SipRelayImp::registerMgcfSipGatewayToResource()
{
    //构造服务注册信息
    VoipApp::ServiceInfo serviceInfo;
    serviceInfo.serviceId = m_strMainObjId;
    serviceInfo.serviceType = "SipRelay";
    serviceInfo.updateTime = TNOW;
    serviceInfo.ttl = 90;  // 心跳超时90秒
    serviceInfo.freePercent = 100;
    //设置服务属性（协议类型、数据中心等）
    // serviceInfo.attributes["protocol"] = m_tranProtoclName;
    // serviceInfo.attributes["dataCenter"] = m_strDataCenter;
    // serviceInfo.attributes["serverIp"] = m_strLocalIp;

    //设置endpoint信息
    VoipApp::ServiceEndpoint endpoint;
    endpoint.host = m_strLocalIp;
    endpoint.port = m_iLocalListenPort;
    endpoint.params["DOMAIN"] = m_strDataCenter;
    endpoint.params["IP"] = m_strLocalIp;
    serviceInfo.endpoints = endpoint;
    
    //异步查询所有SipRelay服务
    class RegistryCallback : public VoipApp::SipRegPrxCallback
    {
    public:
        RegistryCallback(){}
        
        virtual void callback_registerService(tars::Int32 ret)
        {
            if (ret == 0)
            {
                TLOGINFO("register success, found " << endl);
            }
        }
        
        virtual void callback_registerService_exception(tars::Int32 ret)
        {
            TLOGERROR("register services exception, ret:" << ret << endl);
        }
    };

    //异步注册
    m_registryPrx->async_registerService(new RegistryCallback(), serviceInfo);
    
    TLOGINFO("Register service: " << m_strMainObjId 
            << ", protocol:" << m_tranProtoclName
            << ", ip:" << m_strLocalIp 
            << ", port:" << m_iLocalListenPort << endl);
}

bool SipRelayImp::loadMgcfSipGatewayResources()
{
    //构造查询条件
    VoipApp::QueryCondition condition;
    condition.serviceType = "SipRelay";
    // 可选：按数据中心过滤
    //condition.attributes["dataCenter"] = m_strDataCenter;
    
    //异步查询所有SipRelay服务
    class QueryCallback : public VoipApp::SipRegPrxCallback
    {
    public:
        QueryCallback(SipRelayImp* impl) : m_impl(impl) {}
        
        virtual void callback_queryServices(tars::Int32 ret, const std::vector<VoipApp::ServiceInfo>& services)
        {
            if (ret == 0)
                {
                TLOGINFO("Query success, found " << services.size() << " SipRelay services" << endl);
                m_impl->onResultLoadSipGwResource2(services);
            }
        }
        
        virtual void callback_queryServices_exception(tars::Int32 ret) {
            TLOGERROR("Query services exception, ret:" << ret << endl);
        }
    private:
        SipRelayImp* m_impl;
    };
    
    m_registryPrx->async_queryServices(new QueryCallback(this), condition);
    
    TLOGDEBUG("Query all SipRelay services from Registry" << endl);
    return true;
}

void SipRelayImp::onResultLoadSipGwResource2(const std::vector<VoipApp::ServiceInfo> & vectResources)
{
    if(vectResources.empty())
        return;

    std::lock_guard<std::mutex> lock(m_mutexMgcfSipGatewayIds);
    m_mapMgcfSipGatewayIds.clear();

    std::string strIds = "Service IDs:\n";
    
    for (const auto& resource : vectResources)
    {
        strIds += resource.serviceId + "\n";

        if (resource.freePercent < 0)
            continue;

        std::string ip, dc;
        std::map<std::string, std::string>::const_iterator itParam = resource.endpoints.params.find("domain");
        if (itParam != resource.endpoints.params.end())
        {
            dc = itParam->second;
        }

        itParam = resource.endpoints.params.find("ip");
        if (itParam != resource.endpoints.params.end())
        {
            ip = itParam->second;
        }
        m_mapMgcfSipGatewayIds.insert(make_pair(ip, dc));
    }
    
    TLOGDEBUG("onResultLoadSipGwResource2:\n" << strIds << endl);
}

bool SipRelayImp::loadSipGw3Resources()
{

    //构造查询条件
    VoipApp::QueryCondition condition;
    condition.serviceType = "SipController";
    // 可选：按数据中心过滤
    //condition.attributes["dataCenter"] = m_strDataCenter;

    //异步查询所有SipRelay服务
    class QueryCallback : public VoipApp::SipRegPrxCallback
    {
    public:
        QueryCallback(SipRelayImp* impl) : m_impl(impl) {}

        virtual void callback_queryServices(tars::Int32 ret, const std::vector<VoipApp::ServiceInfo>& services)
        {
            if (ret == 0)
            {
                TLOGINFO("Query success, found " << services.size() << " SipRelay services" << endl);
                m_impl->onResultLoadSipGwResource(services);
            }
        }

        virtual void callback_queryServices_exception(tars::Int32 ret) {
            TLOGERROR("Query services exception, ret:" << ret << endl);
        }
    private:
        SipRelayImp* m_impl;
    };

    m_registryPrx->async_queryServices(new QueryCallback(this), condition);

    TLOGDEBUG("Query all SipRelay services from Registry" << endl);
    return true;
}

bool SipRelayImp::onResultLoadSipGwResource(const std::vector<VoipApp::ServiceInfo> & vectResources)
{
    if(vectResources.empty())
        return false;

    std::lock_guard<std::mutex> lock(m_mutexSipGwIds);
    m_vectSipGwIds.clear();

    std::string strIds = "ids:\n";

    for (unsigned int i = 0; i < vectResources.size(); i++)
    {
        VoipApp::ServiceInfo resource = vectResources.at(i);
        m_vectSipGwIds.push_back(resource.serviceId);
        strIds += resource.serviceId + "\n";
    }
}

void SipRelayImp::getCloudSipGwIdAndIp()
{
    checkServerAlive();

    class getSipGwIdAndIp_async : public VoipApp::SipControllerPrxCallback
    {
    public:
        getSipGwIdAndIp_async(SipRelayImp* ptr)
            : _ptr(ptr)
        {
        }
        
        virtual void callback_getCloudSipGatewayIp(tars::Bool ret, const std::string& sipGatewayOid, const std::string& sipGatewayIp)
        {
            if (ret)
            {
                TLOGDEBUG("getCloudSipGatewayIp success, SipGateway: " << sipGatewayOid << " -> " << sipGatewayIp << endl);

                std::lock_guard<std::mutex> lock(_ptr->m_mutexSipGwIdAndIps);
                std::map<std::string, std::string>::iterator itor = _ptr->m_mapSipGwIdAndIps.find(sipGatewayOid);
                if (itor == _ptr->m_mapSipGwIdAndIps.end())
                    _ptr->m_mapSipGwIdAndIps.insert(make_pair(sipGatewayOid, sipGatewayIp));
                else
                    itor->second = sipGatewayIp;
            }
            else
            {
                TLOGWARN("getCloudSipGatewayIp failed" << endl);
            }
        }
        
        virtual void callback_getCloudSipGatewayIp_exception(tars::Int32 ret)
        {
            TLOGWARN("getCloudSipGatewayIp exception, error code: " << ret << endl);
        }

    private:
        SipRelayImp* _ptr;
    };

    std::lock_guard<std::mutex> lock(m_mutexSipGwIds);
    for (unsigned int i = 0; i < m_vectSipGwIds.size(); i++)
    {
        std::string id = m_vectSipGwIds.at(i);

        VoipApp::SipControllerPrx prx = Application::getCommunicator()->stringToProxy<VoipApp::SipControllerPrx>(id);
        prx->async_getCloudSipGatewayIp(new getSipGwIdAndIp_async(this), id);
    }
    // public:
    //     GetSipGwIdAndIp_async(SipRelayImp* ptr, const std::string& serviceId)
    //         : _ptr(ptr), _serviceId(serviceId)
    //     {
    //     }

    //     virtual void callback_getCloudSipGatewayIp(tars::Bool ret, const std::string& sipGatewayOid, const std::string& sipGatewayIp)
    //     {
    //         if (ret)
    //         {
    //             std::lock_guard<std::mutex> lock(_ptr->m_mutexSipGwIdAndIps);
    //             std::map<std::string, std::string>::iterator itor = _ptr->m_mapSipGwIdAndIps.find(sipGatewayOid);
    //             if (itor == _ptr->m_mapSipGwIdAndIps.end())
    //             {
    //                 _ptr->m_mapSipGwIdAndIps.insert(make_pair(sipGatewayOid, sipGatewayIp));
    //                 TLOGDEBUG("Add SipGateway: " << sipGatewayOid << " -> " << sipGatewayIp << endl);
    //             }
    //             else
    //             {
    //                 itor->second = sipGatewayIp;
    //                 TLOGDEBUG("Update SipGateway: " << sipGatewayOid << " -> " << sipGatewayIp << endl);
    //             }
    //         }
    //         else
    //         {
    //             TLOGWARN("getCloudSipGatewayIp failed for service: " << _serviceId << endl);
    //         }
    //     }

    //     virtual void callback_getCloudSipGatewayIp_exception(tars::Int32 ret)
    //     {
    //         TLOGWARN("getCloudSipGatewayIp exception for service: " << _serviceId 
    //                 << ", error code: " << ret << endl);
    //     }

    // private:
    //     SipRelayImp* _ptr;
    //     std::string _serviceId;
    // };

    // // 方案1: 从Registry查询所有SipController服务（推荐）
    // // 通过Registry获取所有活跃的SipController服务实例
    // class QuerySipControllerCallback : public VoipApp::SipRegPrxCallback
    // {
    // public:
    //     QuerySipControllerCallback(SipRelayImp* impl) : m_impl(impl) {}
        
    //     virtual void callback_queryServices(tars::Int32 ret, const std::vector<VoipApp::ServiceInfo>& services)
    //     {
    //         if (ret == 0)
    //         {
    //             TLOGINFO("Found " << services.size() << " SipController services" << endl);
                
    //             // 遍历所有查询到的SipController服务，获取它们的IP
    //             for (const auto& service : services)
    //             {
    //                 try
    //                 {
    //                     VoipApp::SipControllerPrx prx = Application::getCommunicator()
    //                         ->stringToProxy<VoipApp::SipControllerPrx>(service.serviceId);
                        
    //                     prx->async_getCloudSipGatewayIp(
    //                         new GetSipGwIdAndIp_async(m_impl, service.serviceId), 
    //                         service.serviceId);
                        
    //                     TLOGDEBUG("Query IP for service: " << service.serviceId << endl);
    //                 }
    //                 catch (exception& e)
    //                 {
    //                     TLOGERROR("Create proxy failed for service: " << service.serviceId 
    //                              << ", error: " << e.what() << endl);
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             TLOGWARN("Query SipController services failed, ret: " << ret << endl);
    //         }
    //     }
        
    //     virtual void callback_queryServices_exception(tars::Int32 ret)
    //     {
    //         TLOGERROR("Query SipController services exception, ret:" << ret << endl);
    //     }
        
    // private:
    //     SipRelayImp* m_impl;
    // };
    
    // // 查询所有SipController服务
    // VoipApp::QueryCondition condition;
    // condition.serviceType = "SipController";
    // // 可选：按数据中心过滤
    // // condition.attributes["dataCenter"] = m_strDataCenter;
    
    // m_registryPrx->async_queryServices(new QuerySipControllerCallback(this), condition);
    
    // TLOGDEBUG("Query all SipController services for IP mapping" << endl);
}

void SipRelayImp::checkServerAlive()
{
    std::set<std::string> setSipGwIds;
    {
        std::lock_guard<std::mutex> lock(m_mutexSipGwIds);
        setSipGwIds.insert(m_vectSipGwIds.begin(), m_vectSipGwIds.end());
    }

    std::lock_guard<std::mutex> lock(m_mutexSipGwIdAndIps);
    std::map<std::string, std::string>::iterator itor = m_mapSipGwIdAndIps.begin();
    for (; itor != m_mapSipGwIdAndIps.end();)
    {
        if (setSipGwIds.find(itor->first) != setSipGwIds.end())
            ++itor;
        else
        {
            TLOGWARN(std::string(__FUNCTION__) << " erase id:" << itor->first << endl);
            m_mapSipGwIdAndIps.erase(itor++);
        }
    }
}

void SipRelayImp::setLogsAndStatistics()
{
    // TARS_REPORT_SET("Sip.Connection.Remain", SipGateway::GatewayConnection::_totalNum);
    // TARS_REPORT_SET("Sip.Connection.TotalCreated", SipGateway::GatewayConnection::_newNum);
    // TARS_REPORT_SET("Sip.Connection.TotalRemoved", SipGateway::GatewayConnection::_deleteNum);
    
    // TARS_REPORT_SET("Sip.Config.ListenIp", m_strLocalIp);
    // TARS_REPORT_SET("Sip.Config.ListenPort", m_iLocalListenPort);
    // TARS_REPORT_SET("Sip.Config.SendPort", m_iLocalSendPort);
    // TARS_REPORT_SET("Sip.Config.Size", getMgcfSipGatewayCfgsSize());
    // TARS_REPORT_SET("Sip.Connection.SendPort", m_iCurSendPort);
    
    // TARS_REPORT_SET("Sip.Dialog.Insert", m_sipDialogRecordManager.getTotalInsertIdNum());
    // TARS_REPORT_SET("Sip.Dialog.Remove", m_sipDialogRecordManager.getTotalEreaseIdNum());
    // TARS_REPORT_SET("Sip.Dialog.Remain", m_sipDialogRecordManager.getRecordSize());
}

bool SipRelayImp::getOptionStatus(const std::string & connId)
{
    int64_t curr = TNOWMS;
    std::lock_guard<std::mutex> lock(m_mutexMgcfStatus);
    map<std::string, stSbcConn>::iterator it = m_mapMgcfStatus.find(connId);
    if (it == m_mapMgcfStatus.end())
    {
        stSbcConn conn;
        conn.times = 1;
        conn.lastActive = 0;
        conn.lastSendTime = curr;
        m_mapMgcfStatus.insert(make_pair(connId, conn));
    }
    else
    {
        stSbcConn conn = it->second;
        if(curr - conn.lastSendTime < 30000)
        {
            TLOGWARN(std::string(__FUNCTION__) << ",options in 30s connId:" << connId << ", lastActive:" << conn.lastActive << ", lastSendTime:" << conn.lastSendTime << endl);
            return false;
        }

        it->second.lastSendTime = curr;

        if(conn.times > 2)
        {
            TLOGWARN(std::string(__FUNCTION__) << " remove connection counts:" << conn.times << ", connId:" << connId
                << ", lastActive:" << conn.lastActive << ", lastSendTime:" << conn.lastSendTime << endl);

            it->second.times = 0;
            m_connectionCallIdManager.removeConnection(m_strToMgcfSipDialogId, connId);
        }
        else
        {
            it->second.times++;
        }
    }

    return true;
}

void SipRelayImp::sendOptionToMgcf()
{
    std::vector<VoipApp::SipProxyAccessedCoreNetConfig> sipGatewayCfgs;
    getAllSipGatewayCfgs(sipGatewayCfgs);

    for (unsigned int i = 0; i < sipGatewayCfgs.size(); i++)
    {
        VoipApp::SipProxyAccessedCoreNetConfig cfg = sipGatewayCfgs.at(i);
        std::string connId = cfg.m_strSBCIp + ":" + TC_Common::tostr(cfg.m_iSBCPort);

        SipGateway::GatewayConnectionPtr pConn = m_connectionCallIdManager.getConnection(m_strToMgcfSipDialogId, connId);
        if (!pConn)
            continue;

        if(!getOptionStatus(connId))
            continue;

        std::string sipOptons;
        sipOptons += "OPTIONS sip:" + m_strLocalIp + ":" + TC_Common::tostr(m_iLocalListenPort) + " SIP/2.0\r\n";
        sipOptons += "To:<sip:anonymous@" + m_strLocalIp + ">\r\n";
        sipOptons += "From: \"anonymous \"<sip:anonymous@" + m_strLocalIp + ":" + TC_Common::tostr(m_iLocalListenPort) + ">;tag=f755-8068-db20-787e\r\n";
        TC_UUIDGenerator uuidGen;
        sipOptons += "Call-ID: " + uuidGen.genID() + "@SipGateway\r\n";
        sipOptons += "CSeq: 1000 OPTIONS\r\n";
        sipOptons += "Max-Forwards: 70\r\n";
        sipOptons += "Via: SIP/2.0/UDP "+ m_strLocalIp +":"+  TC_Common::tostr(m_iLocalListenPort) + ";" + "branch=z9hG4bKa8fbde060a633a270332c55132c6bf\r\n";
        sipOptons += "Content-Length: 0\r\n\r\n";

        std::string reason;
        sendEncodedMessage(sipOptons, sipOptons.size(), connId, true, reason);
    }
}

void SipRelayImp::decodeMgcfIp(const std::string & cfg, set<std::string> & mgcfIps)
{
    //"192.168.1.1;192.168.1.2;";
    if (cfg.empty())
        return;

    int pos1 = cfg.find(";");
    if (pos1 < 0)
        return;

    int curPos = 0;
    while (pos1 > 0)
    {
        std::string ips;
        ips = cfg.substr(curPos, pos1 - curPos);
        mgcfIps.insert(ips);
        curPos = pos1 + 1;
        pos1 = cfg.find(";", curPos);
        TLOGINFO("****** mgcf ips:" << ips << endl);
    }
}

bool SipRelayImp::sipUriIsAnonymous(const std::string & sipUri)
{
    for(unsigned int i = 0; i < sipUri.size(); i++)
    {
        if ((sipUri[i] < '0' || sipUri[i] > '9') && sipUri[i] != '+')
        {
            return true;
        }
    }

    return false;
}

void SipRelayImp::getCloudSipGatewayId(const VoipApp::JegoRouterInfo& info, std::string& sipGatewayId)
{
    if (info.routeInfoList.empty())
    {
        if(getCloudSipGatewayIdByIp(info.gwAddr, sipGatewayId))
            return;
    }
    else
    {
        for (int i = 0; i < info.routeInfoList.size(); i++)
        {
            if(getCloudSipGatewayIdByIp(info.routeInfoList.at(i).gwAddr, sipGatewayId))
                return;
        }
    }

    std::lock_guard<std::mutex> lk(m_mutexSipGwIds);
    if (m_vectSipGwIds.empty())
    {
        TLOGWARN("getCloudSipGatewayId() error, no alive gateway" << endl);
        return;
    }

    sipGatewayId = m_vectSipGwIds.at(getRandom(m_vectSipGwIds.size() - 1));
}

bool SipRelayImp::getCloudSipGatewayIdByIp(const std::string& ip, std::string& sipGatewayId)
{
    std::vector<std::string> vectSipGatewayIds;
    std::string dc = getDcByIp(ip);

    std::lock_guard<std::mutex> lock(m_mutexSipGwIdAndIps);
    std::map<std::string, std::string>::iterator itor = m_mapSipGwIdAndIps.begin();
    for (; itor != m_mapSipGwIdAndIps.end(); itor++)
    {
        const std::string& id = itor->first;
        if (ip == itor->second)
        {
            sipGatewayId = id;
            return true;
        }

        if (!dc.empty() && id.find(dc) > 0)
            vectSipGatewayIds.push_back(id);
    }

    if (!vectSipGatewayIds.empty())
    {
        sipGatewayId = vectSipGatewayIds.at(getRandom(vectSipGatewayIds.size() - 1));
        return true;
    }

    return false;
}

std::string SipRelayImp::getDcByIp(const std::string& ip)
{
    std::lock_guard<std::mutex> lock(m_mutexMgcfSipGatewayIds);
    std::map<std::string, std::string>::iterator itor = m_mapMgcfSipGatewayIds.find(ip);
    if (itor != m_mapMgcfSipGatewayIds.end())
    {
        return itor->second;
    }
    return "";
}