#include "SipRtpImp.h"
#include "servant/Application.h"
#include "util/tc_common.h"
#include "util/tc_thread.h"
#include "util/tc_timeprovider.h"
#include "util/tc_file.h"
#include "Common/CallExCommon.h"
//////////////////////////////////////////////////////
void SipRtpImp::initialize()
{
    m_iMinPort = 0;
    m_iMaxPort = 0;
    m_bMainServiceReady = false;
    m_bServiceActibed = false;
    m_minNmsPort = 28800;
    m_maxNmsPort = 28810;
    m_LastSchdTimeInMs = 0;
    m_BaseSchdTimeInMs = 5 * 1000;
    m_lastMinute = 0;

    m_maxExpiredTime = 12 * 60 * 60 * 1000;
    m_noAcceptedTimeout = 3 * 60 * 1000;
    m_noMediaTimeout = 3 * 60 * 1000;
    m_mediaEmptyPkgChkInterval = 1000;
    m_mediaEmptyPkgChkMinTime = 5;
    m_mediaEmptyPkgChkPercent = 0.9;

    m_mediaEmptyPkgCheckPres = false;

    
    m_strSDP =  CONST_SDP_STRING;
    if (!SipMsgCommon::SipMsg_Init())
    {
        TLOGERROR("[RtpGateway] *** onActivate() ***, SipMsgCommon::SipMsg_Init() return FALSE!" << endl);
        m_bMainServiceReady = false;
        return;
    }

    // m_jssmmeManager = std::make_shared<JssmmeManager>();
    // m_jssmmeManager->init();
    // getSdpParams(m_sdpCodec);
    // m_jssmmeMaxChannel = m_jssmmeManager->getMaxNumOfChannels();

    m_jssmmeInterface = std::make_shared<SipRtpGateway::RtpGatewayInterfaceI>();
    if (!m_jssmmeInterface)
    {
        TLOGERROR("[RtpGateway] *** onActivate() ***, m_jssmmeInterface is null" << endl);
        m_bMainServiceReady = false;
        return;
    }
    m_jssmmeInterface->create();

    TLOGINFO("[RtpGateway] ****** onActivate()******* jssmme max channel:" << m_jssmmeMaxChannel << ",m_strSDP:\n" << m_strSDP << endl);
    m_strMainObjId = ServerConfig::Application + "." + ServerConfig::ServerName + ".SipRtpObj";
    m_threadPool.init(1);
    m_threadPool.start();
    m_threadPool.exec(std::bind(ExceptionAndStatisticThreadFunc, this));
    m_bServiceActibed = true;
}

//////////////////////////////////////////////////////
void SipRtpImp::destroy()
{
    // 停止线程池
    m_threadPool.stop();
}

void * SipRtpImp::ExceptionAndStatisticThreadFunc(void * arg)
{
    SipRtpImp * prtpGateway = (SipRtpImp *)arg;
    while(1)
    {
        prtpGateway->setLogsAndStatistics();
        TC_Common::sleep(10);
    }
}

void SipRtpImp::setLogsAndStatistics()
{
    // _application->setStatisticsLong("Sip.Content.Remain", RtpContentI::m_lTotalContentNum);
    // _application->setStatisticsLong("Sip.Content.TotalCreated", RtpContentI::m_lTotalNewContNum);
    // _application->setStatisticsLong("Sip.Content.TotalDeleted",RtpContentI::m_iTotalDeleContNum);

    // _application->setStatisticsLong("Sip.Content.Arc.Remain", RtpArcContentI::m_lTotalArcContentNum);
    // _application->setStatisticsLong("Sip.Content.Arc.TotalCreated", RtpArcContentI::m_lTotalNewArcContNum);
    // _application->setStatisticsLong("Sip.Content.Arc.TotalDeleted", RtpArcContentI::m_iTotalDeleArcContNum);

    // _application->setStatisticsLong("Sip.Content.Channel.Remain", Rtp2RtpChannel::m_lTotalChannelNum);
    // _application->setStatisticsLong("Sip.Content.Channel.TotalCreated", Rtp2RtpChannel::m_lTotalNewChannelNum);
    // _application->setStatisticsLong("Sip.Content.Channel.TotalDeleted", Rtp2RtpChannel::m_iTotalDeleChannelNum);

    // _application->setStatisticsLong("Sip.Content.RtpRelay.Remain", Rtp2RtpRelayer::m_lTotalRelayNum);
    // _application->setStatisticsLong("Sip.Content.RtpRelay.TotalCreated", Rtp2RtpRelayer::m_lTotalNewRelayNum);
    // _application->setStatisticsLong("Sip.Content.RtpRelay.TotalDeleted", Rtp2RtpRelayer::m_iTotalDeleRelayNum);


    // //GatewayConnecntion num
    // _application->setStatisticsLong("Sip.Connection.Remain",SipGateway::GatewayConnection::_totalNum);
    // _application->setStatisticsLong("Sip.Connection.TotalCreated",SipGateway::GatewayConnection::_newNum);
    // _application->setStatisticsLong("Sip.Connection.TotalDeleted",SipGateway::GatewayConnection::_deleteNum);

    // int freePortNum = 0, totalPortNum = 0, totalPopedNum = 0, totalInseredNum = 0;
    // m_portManager.getPortNumbers(totalPortNum, freePortNum, totalPopedNum, totalInseredNum);

    // _application->setStatisticsLong("Sip.MediaPort.Free",freePortNum);
    // _application->setStatisticsLong("Sip.MediaPort.Total",totalPortNum);
    // _application->setStatisticsLong("Sip.MediaPort.Poped",totalPopedNum);
    // _application->setStatisticsLong("Sip.MediaPort.Inserted",totalInseredNum);

    // _application->setStatisticsLong("Sip.MediaPort.MinPort", m_iMinPort);
    // _application->setStatisticsLong("Sip.MediaPort.MaxPort", m_iMaxPort);
    // _application->setStatisticsLong("Sip.NmsPort.MinPort", m_minNmsPort);
    // _application->setStatisticsLong("Sip.NmsPort.MaxPort", m_maxNmsPort);
    // _application->setStatistics("Sip.Config.ListenIp", m_strListenIp);
    // _application->setStatistics("Sip.Config.AppMediaCodes", m_appMediaCodes);
    // _application->setStatistics("Sip.Config.SipMediaCodes", m_sipMediaCodes);
    // _application->setStatistics("Sip.Config.PrivateIp", m_strPrivateIp);
    // _application->setStatisticsLong("Sip.Config.ThreadChannel", m_jssmmeMaxChannelsPerThread);

    // if (m_jssmmeManager)
    // {
    //     _application->setStatisticsLong("Sip.Channel.Count", m_jssmmeManager->m_channelCount);
    //     _application->setStatisticsLong("Sip.ChannelId.Count", m_jssmmeManager->m_channelIdCount);
    // }
}


bool SipRtpImp::freeListeningMediaPorts(const std::vector<int> & vecPorts, const std::string & callId)
{
    std::string strTemp = "Ports :";
    for (unsigned int i = 0; i< vecPorts.size(); i++)
    {
        strTemp += std::to_string(vecPorts.at(i)) + " ;";
    }
    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called.callId : " << callId << " ," << strTemp << endl);

    for (unsigned int i = 0; i < vecPorts.size(); i++)
    {
        if ( vecPorts.at(i) >= m_iMinPort &&  vecPorts.at(i) <= m_iMaxPort)
        {
            m_portManager.insertAFreeEvenPort(callId, vecPorts.at(i));
        }
    }

    return true;
}

bool SipRtpImp::allocFreeEvenPorts(const std::string & strCallId, int portNum, std::vector<int> & vectPorts)
{
    vectPorts.clear();
    for (int i = 0; i < portNum; i++)
    {
        int newPort = 0;
        if (!m_portManager.getAFreeEvenPort(strCallId, newPort))
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! m_portManager.getAFreeEvenPort() return FALSE! call id:" << strCallId << endl);
            return false;
        }
        vectPorts.push_back(newPort);
    }
    return true;
}

// void SipRtpImp::onSchd()
// {
//     Content::ContentServerI::onSchd();
//     int64_t now = TNOWMS;
//     int64_t diffTime = (now - m_LastSchdTimeInMs);
//     int64_t thisMinute = now / (60 * 1000);
//     if(diffTime < m_BaseSchdTimeInMs)
//         return;

//     int percent = _application->isOutOfService()? -1 : registerRtpResource();
//     Content::ContentServerI::setFreePercent(percent);

//     m_LastSchdTimeInMs = now;
//     if (thisMinute > m_lastMinute)
//     {
//         clearExceptArcContents();
//         clearRtp2RtpRelays();
//         m_lastMinute = thisMinute;

//         if (m_jssmmeManager)
//         {
//             m_jssmmeManager->clearExpiredChannels();
//         }
//     }
// }

// void SipRtpImp::onUpdateConfigs()
// {
//     Content::ContentServerI::onUpdateConfigs();
//     if (!m_bServiceActibed)
//         return;

//     int minPort = 0, maxPort = 0, minNmsPort = 0, maxNmsPort = 0;
//     bool result1 = false, result2 = false, result3 = false;
//     std::string strListIp, privateIp;

//     //获取配置的最小、最大端口，如果没有配置的话，默认为21000 - 35000
//     result1 = _application->getAppConfigAsInt("MinReceiveMediaPort", minPort);
//     result2 = _application->getAppConfigAsInt("MaxReceiveMediaPort", maxPort);
//     result3 = _application->getAppConfig("ListenRTPIp", strListIp);
//     result1 = _application->getAppConfigAsInt("MinNmsPort", minNmsPort);
//     result1 = _application->getAppConfigAsInt("MaxNmsPort", maxNmsPort);

//     int maxChannelsPerThread = 0;
//     bool result = _application->getAppConfigAsInt("MaxChannelsPerThread", maxChannelsPerThread);
//     if (result)
//     {
//         if (m_jssmmeManager && m_jssmmeMaxChannelsPerThread != maxChannelsPerThread)
//         {
//             m_jssmmeMaxChannelsPerThread = maxChannelsPerThread;
//             m_jssmmeManager->setMaxChannelsPerThread(maxChannelsPerThread);
//         }
//     }

//     int noAcceptedTimeout = 0;
//     result = _application->getAppConfigAsInt("AcceptedTimeout", noAcceptedTimeout);
//     if (result)
//     {
//         if (noAcceptedTimeout > 0 && noAcceptedTimeout <= 20)
//             m_noAcceptedTimeout = noAcceptedTimeout * 60 * 1000;
//     }
//     else
//         m_noAcceptedTimeout = 3 * 60 * 1000;

//     int noMedia = 0;
//     result = _application->getAppConfigAsInt("RtpMediaTimeout", noMedia);
//     if (result)
//     {
//         if (noMedia > 0 && noMedia <= 5)
//             m_noMediaTimeout = noMedia * 60 * 1000;
//     }
//     else
//         m_noMediaTimeout = 3 * 60 * 1000;

//     noMedia = 0;
//     result = _application->getAppConfigAsInt("RtpNoMediaTimeout", noMedia);
//     if (result)
//     {
//         m_acceptedNoMediaTimeout = noMedia * 1000;
//     }
//     else
//         m_acceptedNoMediaTimeout = 30 * 1000;

//     int temMin = 0,temMax = 0;
//     if (minPort > 0 && maxPort > 0)
//     {
//         temMin =  minPort;
//         temMax =  maxPort;
//         m_bMainServiceReady = true;
//     }
//     else
//     {
//         TLOGWARN("[RtpGateway] SipRtpImp::onUpdateConfigs() called! getAppConfig() return minPort:" << temMin << " ,maxPort:" << temMax << endl);
//         temMin =  21000;
//         temMax =  50000;
//     }

//     if (minNmsPort > 0 && maxNmsPort > 0)
//     {
//         if (maxNmsPort > minNmsPort)
//         {
//             if (m_minNmsPort != minNmsPort)
//             {
//                 m_minNmsPort = minNmsPort;
//                 m_maxNmsPort = minNmsPort;
//                 TLOGINFO("[RtpGateway] ****** onUpdateConfigs() ******* (), m_minNmsPort:" << m_minNmsPort << " ,m_maxNmsPort : " << m_maxNmsPort << endl);
//             }
//         }
//     }

//     if (temMin != m_iMinPort || temMax != m_iMaxPort)
//     {
//         m_iMinPort = temMin;
//         m_iMaxPort = temMax;
//         m_portManager.initFreeReceivingPorts(m_iMinPort, m_iMaxPort, m_minNmsPort, m_maxNmsPort);               //Init Free ports list
//         TLOGINFO("[RtpGateway] ****** onUpdateConfigs() ******* to InitFreeReceivingPorts(), minPort:" << m_iMinPort << " ,maxPort : " << m_iMaxPort << endl);
//     }

//     if(strListIp != m_strListenIp)
//     {
//         if(strListIp.empty())
//         {
//             TLOGWARN("[RtpGateway] SipRtpImp::onUpdateConfigs() called! listen ip is EMPTY!!" << endl);
//             m_strListenIp = "127.0.0.1";
//         }
//         else
//         {
//             m_strListenIp = strListIp;
//             m_bMainServiceReady = true;
//         }
//         TLOGINFO("[RtpGateway] ****** onUpdateConfigs()*******   ,ListenIp:" << m_strListenIp << endl);
//     }

//     std::string appMediaCodec;
//     result = _application->getAppConfig("AppMediaCodes", appMediaCodec);
//     if (result)
//     {
//         if(appMediaCodec != m_appMediaCodes)
//         {
//             m_appMediaCodes = appMediaCodec;
//             std::set<std::string> strCfgs;
//             decodeCodecCfg(appMediaCodec, strCfgs);

//             StSdpMediaDesc desc;
//             getSdpParams(strCfgs, desc);
//             m_appSdpDesc = desc;

//             TLOGINFO("[RtpGateway] ****** onUpdateConfigs()*******   ,m_appMediaCodes:" << m_appMediaCodes << endl);
//         }
//     }
//     else
//     {
//         m_appMediaCodes.clear();
//     }

//     std::string sipMediaCodec;
//     result = _application->getAppConfig("SipMediaCodes", sipMediaCodec);
//     if (result)
//     {
//         if(sipMediaCodec != m_sipMediaCodes)
//         {
//             m_sipMediaCodes = sipMediaCodec;
//             std::set<std::string> strCfgs;
//             decodeCodecCfg(sipMediaCodec, strCfgs);

//             StSdpMediaDesc desc;
//             getSdpParams(strCfgs, desc);
//             m_sipSdpDesc = desc;

//             TLOGINFO("[RtpGateway] ****** onUpdateConfigs()*******   ,m_sipMediaCodes:" << m_sipMediaCodes << endl);
//         }
//     }
//     else
//     {
//         m_sipMediaCodes.clear();
//     }

//     if(_application->getAppConfig("ListenRTPPrivateIp", privateIp))
//     {
//         m_strPrivateIp = privateIp;
//     }

//     int maxChannel = 0;
//     if(_application->getAppConfigAsInt("MaxChannel", maxChannel))
//     {
//         m_maxCahnnel = maxChannel;
//     }
//     else
//     {
//         m_maxCahnnel = m_jssmmeMaxChannel > JSSMME_MEDIA_CHANNEL_RESERVE ? m_jssmmeMaxChannel - JSSMME_MEDIA_CHANNEL_RESERVE : m_jssmmeMaxChannel;
//     }

//     int enptyPkg = 0;
//     if(_application->getAppConfigAsInt("MediaEmptyPkgChk", enptyPkg))
//     {
//         m_mediaEmptyPkgCheckPres = enptyPkg ? true : false;
//     }
//     else
//     {
//         m_mediaEmptyPkgCheckPres = true;
//     }

//     int enptyPkgInterval = 0;
//     if(_application->getAppConfigAsInt("MediaEmptyPkgChkIntvl", enptyPkgInterval))
//     {
//         m_mediaEmptyPkgChkInterval = enptyPkgInterval;
//     }
//     else
//     {
//         m_mediaEmptyPkgChkInterval = 1000;
//     }

//     int emptyPkgMinTime = 0;
//     if (_application->getAppConfigAsInt("MediaEmptyPkgChkMinTime", emptyPkgMinTime))
//     {
//         m_mediaEmptyPkgChkMinTime = emptyPkgMinTime;
//     }
//     else
//     {
//         m_mediaEmptyPkgChkMinTime = 5;
//     }

//     int emptyPkgPecent = 0;
//     if (_application->getAppConfigAsInt("MediaEmptyPkgPercent", emptyPkgPecent))
//     {
//         m_mediaEmptyPkgChkPercent = emptyPkgPecent / 100.0;
//     }
//     else
//     {
//         m_mediaEmptyPkgChkPercent = 0.9;
//     }

//     std::map<std::string, std::string> params;
//     std::string strDomainId;
//     if (_application->getAppConfig(APPCFG_DOMAIN_ID, strDomainId))
//     {
//         m_strDomainId = strDomainId;
//         params.insert(make_pair(APPCFG_DOMAIN_ID, strDomainId));
//     }
//     else
//     {
//         m_strDomainId.clear();
//     }

//     std::string domainName;
//     if (_application->getAppConfig(APPCFG_DOMAIN_NAME, domainName))
//     {
//         m_strDomainName = domainName;
//         params.insert(make_pair(APPCFG_DOMAIN_NAME, domainName));
//     }
//     else
//     {
//         m_strDomainName.clear();
//     }

//     std::string recordPath;
//     if (_application->getAppConfig("RecordPath", recordPath))
//     {
//         if (m_strRecordPath != recordPath)
//         {
//             if (!TC_File::isFileExist(recordPath))
//             {
//                 if(TC_File::makeDirRecursive(recordPath))
//                 {
//                     m_strRecordPath = recordPath;
//                     TLOGINFO("[RtpGateway] ****** onUpdateConfigs()*******   , path change, makeDir success, recordPath" << recordPath << endl);
//                 }
//                 else
//                     TLOGWARN("[RtpGateway] ****** onUpdateConfigs()*******   , makeDir failed, recordPath" << recordPath << endl);
//             }
//             else
//                 m_strRecordPath = recordPath;
//         }
//     }
//     else if (m_strRecordPath.empty())
//     {
//         if (!TC_File::isFileExist(CALL_MEDIA_RECORD_DIR))
//         {
//             if(TC_File::makeDirRecursive(CALL_MEDIA_RECORD_DIR))
//             {
//                 m_strRecordPath = CALL_MEDIA_RECORD_DIR;
//                 TLOGINFO("[RtpGateway] ****** onUpdateConfigs()*******   , makeDir default success, recordPath" << m_strRecordPath << endl);
//             }
//             else
//             {
//                 TLOGERROR("[RtpGateway] *** onUpdateConfigs() ***, create record dir failed" << endl);
//             }
//         }
//         else
//             m_strRecordPath = CALL_MEDIA_RECORD_DIR;
//     }

//     addExtraEndpointParams(params);
// }

bool SipRtpImp::isMainServiceReady(std::string& reason)
{
    return m_bMainServiceReady;
}

tars::Bool SipRtpImp::allocRtpResource(tars::Bool isCaller,const map<std::string, std::string> & params,map<std::string, std::string> &retValus,tars::TarsCurrentPtr _current_)
{
    std::string reason;
    Rtp2RtpRelayerPtr pRelay = 0;
    std::map<std::string, std::string> inParams(params),outParams;
    bool result = false, qos = false, callRecord = false, isNegoFinished = false;
    int mrfpPres = 0, isPrivateIp = 0;
    std::string sdp,callId, caller, callee, st;

    do
    {
        map<std::string,std::string> ::const_iterator iteKey = params.find(CallEx::CALL_SERVER_MAP_KEY_CALL_ID_EX);
        if (iteKey != params.end())
        {
            callId = iteKey->second;
        }
        else
        {
            reason = "callId not found;";
            break;
        }

        iteKey = params.find(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP);
        if (iteKey != params.end())
        {
            sdp = iteKey->second;
            if (sdp.empty())
            {
                reason = "sdp is empty.";
                break;
            }
        }
        else
        {
            reason = "RseEx.SDP not Found!";
            break;
        }

        iteKey = params.find(CallEx::CALL_SERVER_MAP_KEY_SDK_ISNEW);
        if (iteKey != params.end())
        {
            try {
                mrfpPres = TC_Common::strto<int>(iteKey->second);
            } catch (...) {
                mrfpPres = 0;
            }
        }

        iteKey = params.find(SipCloudGateway::SIP_GW_RPC_PARAMS_IS_PRIVATE_IP);
        if (iteKey != params.end())
        {
            try {
                isPrivateIp = TC_Common::strto<int>(iteKey->second);
            } catch (...) {
                isPrivateIp = 0;
            }
        }

        iteKey = params.find(SipCloudGateway::SIP_GW_RPC_PARAMS_USE_QOS);
        if (iteKey != params.end())
        {
            qos = true;
        }

        iteKey = params.find(SipCloudGateway::SIP_GW_RPC_PARAMS_CALL_RECORD);
        if (iteKey != params.end())
        {
            callRecord = true;
            qos = true;
        }

        iteKey = params.find(SipGateway::SIP_ACCOUNT_CHARGE_KEY_CALLER);
        if (iteKey != params.end())
        {
            caller = iteKey->second;
        }

        iteKey = params.find(SipGateway::SIP_ACCOUNT_CHARGE_KEY_CALLEE);
        if (iteKey != params.end())
        {
            callee = iteKey->second;
        }

        iteKey = params.find(SipGateway::SIP_ACCOUNT_CHARGE_KEY_TIME);
        if (iteKey != params.end())
        {
            time_t timeValue = TC_Common::strto<time_t>(iteKey->second);
            st = TC_Common::tm2str(timeValue, "%Y%m%d%H%M%S");
        }

        iteKey = params.find(SipCloudGateway::SIP_GW_RPC_PARAMS_MEDIA_NEGO_FINISHED);
        if (iteKey != params.end())
        {
            isNegoFinished = true;
        }

        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, call id: " << callId << ",isCaller:" << isCaller << ",mrfpPres:" << mrfpPres << ",isPrivateIp:" << isPrivateIp
            << ", callRecord:" << callRecord << ", qos:" << qos << ", isNegoFinished:" << isNegoFinished << ", caller:" << caller << ", callee:" << callee << ",sdp:\n" << sdp << endl);

        int iCaller = isCaller ? 1 : 0;
        {
            tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexRtpRelays);
            SipRtpImp::Rtp2RtpRelaysMap::iterator itId = m_mapRtpRelays.find(callId);
            if (itId != m_mapRtpRelays.end())
            {
                pRelay = itId->second;
                Rtp2RtpChannelPtr pChannel = pRelay->getChannel(iCaller);
                if (pChannel)
                {
                    if (pChannel->getSdp(true) == sdp)
                    {
                        std::string negoSdp = pChannel->getSdp(false);
                        if (negoSdp.empty())
                        {
                            reason = "media nego failed";
                            break;
                        }

                        outParams[CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP] = negoSdp;
                        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, get channel,and sdp not changed, call id: " << callId << ",return:\n" << pChannel->getSdp(false) << endl);
                        break;
                    }
                    else
                    {
                        pChannel->refreshMediaSteamConnection(sdp);
                        std::vector<int> vectPorts = isCaller ? pChannel->getCalleeRecvPorts() : pChannel->getCallerRecvPorts();

                        Rtp2RtpChannelPtr pChannelCallee = pRelay->getChannel(!iCaller);
                        bool isPrivateIp = pChannelCallee ? pChannelCallee->getPrivateIpProps() : pChannel->getPrivateIpProps();

                        std::string modifyedSdp;
                        std::string listenIp = isPrivateIp ? m_strPrivateIp : m_strListenIp;
                        if (!SipMsg::CreateSDPWithNewPort2(sdp, vectPorts, listenIp, modifyedSdp))
                        {
                            reason = "CreateSDPWithNewPort failed";
                            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, SipMsg::CreateSDPWithNewPort() return FALSE! strCallId:" << callId << endl);
                            break;
                        }
                        pChannel->setSdp(sdp, true);
                        pChannel->setSdp(modifyedSdp, false);
                        outParams[CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP] = modifyedSdp;
                        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, sdp changed, call id: " << callId << ", modifyedSdp:\n" << modifyedSdp << endl);

                        if (pRelay->getQosPres() && isNegoFinished)
                        {
                            if (isCaller)
                            {
                                processReConnMediaNego(pChannel, pChannelCallee, callId, modifyedSdp, pRelay->getCallRecordPres());
                            }
                            else
                            {
                                processReConnMediaNego(pChannelCallee, pChannel, callId, modifyedSdp, pRelay->getCallRecordPres());
                            }
                        }
                        break;
                    }
                }
            }
            else
            {
                pRelay = std::make_shared<Rtp2RtpRelayer>(shared_from_this(), callId, qos, m_mediaEmptyPkgChkInterval, m_mediaEmptyPkgCheckPres, callRecord);
                m_mapRtpRelays.insert(make_pair(callId, pRelay));
            }
        }

        Rtp2RtpChannelPtr channel = pRelay->getChannel(iCaller);
        if (!channel)
        {
            if (iCaller)
            {
                Rtp2RtpChannelPtr pChannel = pRelay->createChannel(iCaller, sdp);

                bool callerUsePrivateIp = (isPrivateIp == 1 || isPrivateIp == 3) ? true : false;
                bool calleeUsePrivateIp = (isPrivateIp == 2 || isPrivateIp == 3) ? true : false;
                pChannel->setPrivateIpProps(callerUsePrivateIp);
                pChannel->setCalleePrivateIpProps(calleeUsePrivateIp);
                pChannel->setCallNumber(true, caller);
                pChannel->setCallNumber(false, callee);
                pChannel->setRecordTime(st);

                inParams[CallEx::CALL_SERVER_MAP_KEY_CALL_ID_EX] = callId;
                inParams[CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP] = sdp;

                std::vector<int> vectMrfpPorts, vectCallerPorts, vectCalleePorts;
                result = rtpConnectionBuildCaller(pChannel, inParams, outParams, vectMrfpPorts, vectCallerPorts, vectCalleePorts, calleeUsePrivateIp, reason);
                if (!result)
                {
                    reason = "rtpConnectionBuildCaller failed";
                    break;
                }

                std::map<std::string, std::string>::iterator itKey = outParams.find(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP);
                if (itKey == outParams.end())
                {
                    reason = "callee sdp offer not found";
                    break;
                }
                pChannel->setCalleeSdp(itKey->second, false);
                pChannel->setCallerRecvPorts(vectCallerPorts);
                pChannel->setCalleeRecvPorts(vectCalleePorts);

                if(mrfpPres)
                {
                    pChannel->setMrfpRecvPorts(vectMrfpPorts);

                    std::map<std::string, std::string>::iterator itKey = outParams.find(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP);
                    if (itKey == outParams.end())
                    {
                        reason = "mrfp sdp not found";
                        break;
                    }
                    pChannel->setSdp(itKey->second, false);

                    Rtp2RtpChannelPtr pMrfpChannel = pRelay->createChannel(2, sdp);
                    result = rtpConnectionBuildMrfp(pMrfpChannel, outParams, vectMrfpPorts, callId, reason);
                    if (result)
                    {
                        pMrfpChannel->setPeerChannel(pChannel);
                    }
                }
            }
            else
            {
                Rtp2RtpChannelPtr pChannelCaller = pRelay->getChannel(!iCaller);
                if (!pChannelCaller)
                {
                    reason = "channel not found for caller:" + std::to_string(iCaller);
                    break;
                }

                Rtp2RtpChannelPtr pChannel = pRelay->createChannel(iCaller, sdp);
                pChannel->setPrivateIpProps(pChannelCaller->getCalleePrivateIpProps());
                std::string sdpOffer = pChannelCaller->getCalleeSdp(false);
                std::vector<int> vectCallerPorts = pChannelCaller->getCallerRecvPorts();
                pChannel->setCallerRecvPorts(vectCallerPorts);
                std::vector<int> vectCalleePorts = pChannelCaller->getCalleeRecvPorts();
                result = rtpConnectionBuild2(pChannel, inParams, outParams, sdp, vectCalleePorts, reason);
                if (!result)
                {
                    reason = "callee rtpConnectionBuild2 failed!";
                    break;
                }

                bool usePrivateIp = pChannelCaller->getPrivateIpProps();
                std::string listenIp = usePrivateIp ? m_strPrivateIp : m_strListenIp;
                std::string strNewSdp;
                result = SipMsg::CreateSDPWithNewPort(sdp, vectCallerPorts, listenIp, strNewSdp);
                if (!result)
                {
                    reason = "caller createSdp failed!";
                    break;
                }
                pChannelCaller->setSdp(strNewSdp, false);
                pChannel->setSdp(sdpOffer, false);
                outParams[CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP] = strNewSdp;

                if (pRelay->getQosPres())
                {
                    if(!processMediaNego(pChannelCaller, pChannel, callId, sdp, pRelay->getCallRecordPres(), isNegoFinished))
                    {
                        reason = "processMediaNego failed!";
                        break;
                    }
                }
            }

            if (!pRelay->getQosPres())
            {
                if (pRelay->getChannel(iCaller) && pRelay->getChannel(!iCaller))
                {
                    (pRelay->getChannel(iCaller))->setPeerChannel(pRelay->getChannel(!iCaller));
                    (pRelay->getChannel(!iCaller))->setPeerChannel(pRelay->getChannel(iCaller));
                }
            }
        }
    } while (0);

    outParams[SipRtpGateway::RTP_GW_RTP_GATEWAY_OID] = m_strMainObjId;
    result = reason.empty();
    if (!result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, call id: " << callId << ",reason:" << reason << endl);
    }
    return result;
}

tars::Bool SipRtpImp::releaseRtpResource(const std::string & callId,tars::TarsCurrentPtr _current_)
{
    std::string reason;
    do
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexRtpRelays);
        SipRtpImp::Rtp2RtpRelaysMap::iterator iterCallId = m_mapRtpRelays.find(callId);
        if (iterCallId == m_mapRtpRelays.end())
        {
            reason = "m_mapRtpRelays not found.";
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, call id: " << callId << ",reason:" << reason << endl);
            break;
        }
        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, call id: " << callId << endl);

        iterCallId->second->clear();
        m_mapRtpRelays.erase(iterCallId);

    } while (0);

    bool result = reason.empty();
    if (!result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, call id: " << callId << ",reason:" << reason << endl);
    }
    return result;
}

tars::Bool SipRtpImp::updateRecordStatus(const std::string & callId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    std::string reason, path;
    do
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexRtpRelays);
        SipRtpImp::Rtp2RtpRelaysMap::iterator itor = m_mapRtpRelays.find(callId);
        if (itor == m_mapRtpRelays.end())
        {
            reason = "m_mapRtpRelays not found.";
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, call id: " << callId << ",reason:" << reason << endl);
            break;
        }

        Rtp2RtpRelayerPtr ptr = itor->second;
        if (!ptr)
        {
            reason = "Rtp2RtpRelayerPtr is null";
            break;
        }

        Rtp2RtpChannelPtr channel = ptr->getChannel(0);
        if (!channel)
        {
            reason = "callee channel is null";
            break;
        }
        channel->setAnswerTime();

        channel = ptr->getChannel(1);
        if (!channel)
        {
            reason = "caller channel is null";
            break;
        }
        channel->setAnswerTime();

        if (ptr->getCallRecordPres())
        {
            if (!channel->getChannelPres() || channel->getRecordPres())
            {
                reason = "channel not created or record started";
                break;
            }

            path = m_strRecordPath + "/" + channel->getRecordTime() + "_" + callId + "_" + channel->getCallNumber(true) + "_" + channel->getCallNumber(false) + ".wav";
            if(!m_jssmmeInterface->startCallRecord(channel->getChannelId(), path))
            {
                reason = "startCallRecord failed";
                break;
            }

            channel->setRecordPres(true);
        }
    } while (0);

    if (!reason.empty())
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called failed callId: " << callId << ", reason:" << reason << endl);
        return false;
    }

    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called success call id: " << callId << ", path:" << path << endl);
    return true;
}


void SipRtpImp::clearRtp2RtpRelays()
{
    int64_t curTicks = TNOWMS;
    tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexRtpRelays);
    Rtp2RtpRelaysMap::iterator itId = m_mapRtpRelays.begin();
    for (; itId != m_mapRtpRelays.end(); )
    {
        int64_t createTicks = itId->second->getCreateTicks();
        int64_t lastActiveTicks = itId->second->getLastActiveTicks();
        int64_t diff1 = curTicks - createTicks;
        int64_t diff2 = curTicks - lastActiveTicks;

        bool notActived = (lastActiveTicks < 0 && diff1 > m_noAcceptedTimeout) ? true : false;
        bool dialogClosed = (diff2 >=  m_noMediaTimeout && lastActiveTicks > 0) ? true : false;
        if (notActived || dialogClosed)
        {
            std::string callId = itId->second->getCallId();
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called.callId:" << callId << ",notActived:" << notActived << ",or dialog closed:" << dialogClosed << endl);

            itId->second->clear();
            m_mapRtpRelays.erase(itId++);
        }
        else
        {
            itId++;
        }
    }
}

bool SipRtpImp::rtpConnectionBuild(RtpSenderPtr  pSender, const std::map<std::string, std::string>& params,std::map<std::string, std::string>& outParams,std::string & reason)
{
    bool result = true;
    std::string sipGatewayOid;
    std::vector<int> vectNewPorts;
    int iConfgSDPPortNum = 0;
    std::string strNewConfigSDP;
    int newPort = 0;
    std::string strCallId ;
    std::string strParams;

    do
    {

        strParams = SipMsgCommon::encodeMapParasLog(params);
        std::map<std::string, std::string> ::const_iterator iterCallId = params.find(CallEx::CALL_SERVER_MAP_KEY_CALL_ID_EX);
        if (iterCallId != params.end())
        {
            strCallId = iterCallId->second;
        }
        strNewConfigSDP = m_strSDP;
        //pSender->setApplication(_application, m_strListenIp);
        pSender->setCallId(strCallId);

        std::map<std::string, std::string> ::const_iterator iterCallerSDP = params.find(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP); //�查看是否带入SDP参数
        if (iterCallerSDP != params.end())
        {
            m_strSDP = iterCallerSDP->second.empty() ? m_strSDP : iterCallerSDP->second;
        }
        else
        {
            reason = "RseEx.SDP not Found!";
            break;
        }
        TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "() called! rpc call id:" << strCallId << ",m_strListenIp:" << m_strListenIp << ",strPrams:" << strParams << " ,m_strSDP : \n" << m_strSDP << " \n m_strSDP SIZE :" << m_strSDP.size() << endl);

        if (SipMsg::GetSDPMediaPortNum(m_strSDP, iConfgSDPPortNum))
        {
            for (int i = 0; i < iConfgSDPPortNum; i++)
            {
                if (!m_portManager.getAFreeEvenPort(strCallId, newPort))
                {
                    reason = "getAFreeEvenPort() return FALSE!";
                    break;
                }
                vectNewPorts.push_back(newPort);
            }
            if (false == reason.empty())
            {
                break;
            }
        }
        else
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::GetSDPMediaPortNum() return FALSE! call id:" << strCallId << " ,SDP:\n" << m_strSDP << endl);
            m_portManager.getAFreeEvenPort(strCallId, newPort);
            vectNewPorts.push_back(newPort);
        }
        pSender->addLocalRecvPorts(vectNewPorts);

        std::map<std::string, std::string> ::const_iterator iter = params.find(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP); //查看是否带入SDP参数
        std::string strCsSDP, strAppSDP;
        if (iter == params.end())
        {
            //voip to cs
            if (!SipMsg::CreateSDPWithNewPort(m_strSDP, vectNewPorts, m_strListenIp, strNewConfigSDP))
            {
                reason = "voip -> cs, CreateSDPWithNewPort() return FALSE when sdp offer is :" + m_strSDP;
                break;
            }
            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP, strNewConfigSDP));
            pSender->setCaller(1);
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, voip -> cs, call id:  " << strCallId << " ,modified app SDP:\n" << strNewConfigSDP << endl);
        }
        else
        {
            pSender->setCaller(0);
            //cs  to voip
            std::vector<SDPMediaInfo> vectSdpMediaInfo;
            std::string strAppSDP, strCsSipMsg, strSdpIp;
            bool result = false;

            strCsSDP = iter->second;
            iter = params.find(CallEx::CALL_SERVER_MAP_KEY_SIPGW_ID);
            if (iter != params.end())
            {
                sipGatewayOid = iter->second;
            }
            if(false == SipMsg::CreateSipMsgFromSDP(strCsSDP, strCsSipMsg))
            {
                reason = "cs -> voip, CreateSipMsgFromSDP() return FALSE with sdp of cs:" + strCsSDP;
                break;
            }
            if (false == SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strCsSipMsg, vectSdpMediaInfo))
            {
                reason = "cs -> voip, getMediaIpAndPortsOfSDPinSipMsg() return FALSE with cs sip msg:" + strCsSipMsg;
                break;
            }
            if (vectSdpMediaInfo.size() < 1)
            {
                reason = "cs -> voip, get ip and ports failed with cs sip msg:" + strCsSipMsg;
                break;
            }
            result = pSender->generateMediaStreamConnectionToSBCEx(vectSdpMediaInfo);
            if (false == result)
            {
                reason = "cs -> voip, failed to build connection to CS!";
                break;
            }

            if (sipGatewayOid.empty())
            {
                std::map<std::string, std::string> ::const_iterator iterCalleeAnsweredSDP = params.find(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP);
                if (iterCalleeAnsweredSDP == params.end())
                {
                    reason = "cs -> voip, RseEx.SDP not found!";
                    break;
                }

                strAppSDP = iterCalleeAnsweredSDP->second;

                if (!SipMsg::CreateSDPWithNewPort(strAppSDP, vectNewPorts, m_strListenIp, strNewConfigSDP))
                {
                    reason = "cs -> voip, CreateSDPWithNewPort() return FALSE with sdp of app:" + strAppSDP;
                    break;
                }
                outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP, strNewConfigSDP));
                TLOGINFO("[RtpGateway] " << __FUNCTION__ << "called, cs -> voip,call id:  " << strCallId << " ,modified app SDP:\n" << strNewConfigSDP << endl);
            }
            else
            {
                pSender->setSipGatewayOid(sipGatewayOid);
                std::string hostPorts;
                bool result = SipMsgCommon::encodeRtpHostPorts(m_strListenIp, vectNewPorts, hostPorts);
                if (false == result)
                {
                    reason = "encodeRtpHostPorts() return FALSE.";
                    break;
                }
                outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_HOSTPORTS, hostPorts));
                TLOGINFO("[RtpGateway] " << __FUNCTION__ << "called, cs -> voip, call id:  " << strCallId << " ,sipGatewayOid:" << sipGatewayOid << ",ip and ports:" << hostPorts << endl);
            }
        }
        outParams["rtpGw.ip"] = m_strListenIp;
    } while (0);

    if (false == reason.empty())
    {
        outParams.insert(make_pair(CallEx::CALL_SERVER_TERMINATE_REASON_KEY, reason));
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "called,and return FALSE! call id:" << strCallId << ",reason:" << reason << endl);
    }
    return reason.empty() ? true : false;
}

bool SipRtpImp::rtpConnectionBuild(RtpSenderPtr pSender, const std::map<std::string, std::string>& params,std::map<std::string, std::string>& outParams,std::string & reason,  JssmmeChannelPtr & pChannel)
{
    bool result = false;
    std::string sipGatewayOid;
    vector<int> vectNewPorts;
    int iConfgSDPPortNum = 0;
    std::string strNewConfigSDP;
    int newPort = 0;
    std::string strCallId ;
    std::string strParams;

    do 
    {
        strParams = SipMsgCommon::encodeMapParasLog(params);
        map<std::string, std::string> ::const_iterator iterCallId = params.find(CallEx::CALL_SERVER_MAP_KEY_CALL_ID_EX);
        if (iterCallId != params.end())
        {
            strCallId = iterCallId->second;
        }
        strNewConfigSDP = m_strSDP;
        //pSender->setApplication(_application, m_strListenIp);
        pSender->setCallId(strCallId);

        map<std::string, std::string> ::const_iterator iterCallerSDP = params.find(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP); //查看是否带入SDP参数
        if (iterCallerSDP != params.end())
        {
            m_strSDP = iterCallerSDP->second.empty() ? m_strSDP : iterCallerSDP->second;
        }
        else
        {
            reason = "RseEx.SDP not Found!";
            break;
        }

        map<std::string, std::string> ::const_iterator iterOid = params.find(CallEx::CALL_SERVER_MAP_KEY_SIPGW_ID);
        if (iterOid != params.end())
        {
            sipGatewayOid = iterOid->second;
            pSender->setSipGatewayOid(sipGatewayOid);
        }

        TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "() called! rpc call id:" << strCallId << ",m_strListenIp:" << m_strListenIp << ",strPrams:" << strParams << " ,m_strSDP : \n" << m_strSDP << " \n m_strSDP SIZE :" << m_strSDP.size() << endl);

        if (SipMsg::GetSDPMediaPortNum(m_strSDP, iConfgSDPPortNum))
        {
            for (int i = 0; i < iConfgSDPPortNum; i++)
            {
                if (!m_portManager.getAFreeEvenPort(strCallId, newPort))
                {
                    reason = "getAFreeEvenPort() return FALSE!";
                    break; 
                }
                vectNewPorts.push_back(newPort);
            }
            if (false == reason.empty())
            {
                break;
            }
        }
        else
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::GetSDPMediaPortNum() return FALSE! call id:" << strCallId << " ,SDP:\n" << m_strSDP << endl);
            m_portManager.getAFreeEvenPort(strCallId, newPort);
            vectNewPorts.push_back(newPort);
        }
        pSender->addLocalRecvPorts(vectNewPorts);

        map<std::string, std::string> ::const_iterator iter = params.find(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP); //查看是否带入SDP参数
        std::string strCsSDP, strAppSDP;
        if (iter == params.end())
        {
            //nego app sdp
            pChannel->m_appSdpOffer = m_strSDP;

            std::string strNewSdp;
            StSdpSessDesc stSdp, stNegoSdp;
            StSdpCodec sdpCode;
            SipMsg::readSdpParams(m_strSDP, stSdp);
            SipMsg::negoSdp(stSdp, stNegoSdp, m_appSdpDesc, sdpCode);
            if (sdpCode.iPayload == ZMAXUCHAR)
            {
                TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::negoSdp return FALSE! call id:" << strCallId << " ,SDP:\n" << m_strSDP << endl);
                reason == "SipMsg::negoSdp app failed";
                break;
            }

            StSdpCodec dtmfCode;
            int dtmfPayload = SipMsg::getDtmfPayload(stSdp, dtmfCode);
            if (dtmfPayload)
                pChannel->m_appDtmfPayload = dtmfPayload;

            int redPayload = SipMsg::getRedPayload(stSdp);
            pChannel->m_appRedPayload = redPayload;

            result = SipMsg::createSdp(stNegoSdp, m_strListenIp, vectNewPorts, strNewSdp);
            if (!result)
            {
                reason = "voip -> cs, app sdp answer createSdp failed!";
                break;
            }

            pChannel->m_appSdpAnswer = strNewSdp;

            vector<StSdpCodec> vectSendCodec;
            vectSendCodec.push_back(sdpCode);
            pChannel->m_vectAppSendCodec = vectSendCodec;

            //voip to cs
            StSdpMediaDesc stCfgSdp;
            getAllSdpCodecCfgs(false, stCfgSdp);
            SipMsg::updateSdpCodec(stCfgSdp, dtmfCode);

            StSdpSessDesc stSdpNewSess = stNegoSdp;
            SipMsg::updateMediaDesc(stSdpNewSess, stCfgSdp);

            result = SipMsg::createSdp(stSdpNewSess, m_strListenIp, vectNewPorts,strNewConfigSDP);
            if (!result)
            {
                reason = "voip -> cs, sip sdp offer createSdp failed!";
                break;
            }

            pChannel->m_sipSdpOffer = strNewConfigSDP;
            pChannel->m_appMediaNegoStatus = kMediaNegoFinished;

            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP, strNewSdp));
            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP, strNewConfigSDP));

            pSender->setCaller(true);
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, voip -> cs, call id:  " << strCallId << ", sipGatewayOid:" << sipGatewayOid << " , appSdpAnswer:\n" << strNewSdp << ", sipSdpOffer:" << strNewConfigSDP << endl);
        }
        else
        {
            pSender->setCaller(false);
            //cs  to voip
            vector<SDPMediaInfo> vectSdpMediaInfo;
            std::string strAppSDP, strCsSipMsg, strSdpIp;

            strCsSDP = iter->second;
            pChannel->m_sipSdpOffer = strCsSDP;

            if(false == SipMsg::CreateSipMsgFromSDP(strCsSDP, strCsSipMsg))
            {
                reason = "cs -> voip, CreateSipMsgFromSDP() return FALSE with sdp of cs:" + strCsSDP;
                break;
            }
            if (false == SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strCsSipMsg, vectSdpMediaInfo))
            {
                reason = "cs -> voip, getMediaIpAndPortsOfSDPinSipMsg() return FALSE with cs sip msg:" + strCsSipMsg;
                break;
            }
            if (vectSdpMediaInfo.size() < 1)
            {
                reason = "cs -> voip, get ip and ports failed with cs sip msg:" + strCsSipMsg;
                break;
            }
            result = pSender->generateMediaStreamConnectionToSBCEx(vectSdpMediaInfo);
            if (!result)
            {
                reason = "cs -> voip, failed to build connection to CS!";
                break;
            }

            //nego cs sdp
            std::string strNewSdp;
            StSdpSessDesc stSdp, stNegoSdp;
            StSdpCodec sdpCode;
            SipMsg::readSdpParams(strCsSDP, stSdp);
            SipMsg::negoSdp(stSdp, stNegoSdp, m_sipSdpDesc, sdpCode);
            if (sdpCode.iPayload == ZMAXUCHAR)
            {
                TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::negoSdp sip sdp return FALSE! call id:" << strCallId << " ,SDP:\n" << m_strSDP << endl);
                reason == "SipMsg::negoSdp sip failed";
                break;
            }

            StSdpCodec dtmfCode;
            int dtmfPayload = SipMsg::getDtmfPayload(stSdp, dtmfCode);
            if (dtmfPayload)
                pChannel->m_sipDtmfPayload = dtmfPayload;

            int redPayload = SipMsg::getRedPayload(stSdp);
            pChannel->m_sipRedPayload = redPayload;

            result = SipMsg::createSdp(stNegoSdp, m_strListenIp, vectNewPorts, strNewSdp);
            if (!result)
            {
                reason = "cs -> voip,  sip sdp answer createSdp failed!";
                break;
            }

            pChannel->m_sipSdpAnswer = strNewSdp;

            vector<StSdpCodec> vectSendCodec;
            vectSendCodec.push_back(sdpCode);
            pChannel->m_vectSipSendCodec = vectSendCodec;

            StSdpMediaDesc stCfgSdp;
            getAllSdpCodecCfgs(true, stCfgSdp);
            SipMsg::updateSdpCodec(stCfgSdp, dtmfCode);

            StSdpSessDesc stSdpNewSess = stNegoSdp;
            SipMsg::updateMediaDesc(stSdpNewSess, stCfgSdp);
            result = SipMsg::createSdp(stSdpNewSess, m_strListenIp, vectNewPorts,strNewConfigSDP);
            if (!result)
            {
                reason = "cs -> voip, app sdp offer createSdp failed!";
                break;
            }

            pChannel->m_appSdpOffer = strNewConfigSDP;

            pChannel->m_sipMediaNegoStatus = kMediaNegoFinished;

            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP, strNewConfigSDP));
            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP, strNewSdp));

            std::string hostPorts;
            bool result = SipMsgCommon::encodeRtpHostPorts(m_strListenIp, vectNewPorts, hostPorts);
            if (false == result)
            {
                reason = "encodeRtpHostPorts() return FALSE.";
                break;
            }
            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_HOSTPORTS, hostPorts));

            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "called, cs -> voip,call id:  " << strCallId << ",pChannel->m_sipDtmfPayload:" << pChannel->m_sipDtmfPayload << ", sipSdpAnswer:\n" << strNewSdp << " ,appSdpOffer:\n" << strNewConfigSDP << endl);
        }
        outParams["rtpGw.ip"] = m_strListenIp;
    } while (0);

    if (false == reason.empty())
    {
        outParams.insert(make_pair(CallEx::CALL_SERVER_TERMINATE_REASON_KEY, reason));
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "called,and return FALSE! call id:" << strCallId << ",reason:" << reason << endl);
    }
    return reason.empty() ? true : false;
}

bool SipRtpImp::rtpConnectionBuildCaller(RtpSenderPtr pSender, const std::map<std::string, std::string>& params, std::map<std::string, std::string>& outParams, vector<int> & vectMrfpPorts, vector<int> & vectCallerPorts, vector<int> & vectCalleePorts, bool isPrivateIp, std::string & reason)
{
    bool result = true;
    std::string sipGatewayOid;
    std::vector<int> vectNewPorts;
    int iConfgSDPPortNum = 0;
    std::string strNewConfigSDP;
    int newPort = 0, mrfpPres = 0;
    std::string strCallId ;
    std::string strParams;
    std::vector<SDPMediaInfo> vectSdpMediaInfo;

    do
    {
        strParams = SipMsgCommon::encodeMapParasLog(params);
        map<std::string, std::string> ::const_iterator iterCallId = params.find(CallEx::CALL_SERVER_MAP_KEY_CALL_ID_EX);
        if (iterCallId != params.end())
        {
            strCallId = iterCallId->second;
        }
        strNewConfigSDP = m_strSDP;
        //pSender->setApplication(_application, m_strListenIp);
        pSender->setCallId(strCallId);
        std::map<std::string, std::string> ::const_iterator iterCallerSDP = params.find(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP); //查看是否带入SDP参数
        if (iterCallerSDP != params.end())
        {
            m_strSDP = iterCallerSDP->second.empty() ? m_strSDP : iterCallerSDP->second;
        }
        else
        {
            reason = "RseEx.SDP not Found!";
            break;
        }

        iterCallerSDP = params.find(CallEx::CALL_SERVER_MAP_KEY_SDK_ISNEW);
        if (iterCallerSDP != params.end())
        {
            mrfpPres = TC_Common::strto<int>(iterCallerSDP->second);
            //mrfpPres = iterCallerSDP->second.toInt(0);
        }

        if (SipMsg::GetSDPMediaPortNum(m_strSDP, iConfgSDPPortNum))
        {
            for (int i = 0; i < iConfgSDPPortNum; i++)
            {
                if (!m_portManager.getAFreeEvenPort(strCallId, newPort))
                {
                    reason = "getAFreeEvenPort() return FALSE!";
                    break;
                }
                vectNewPorts.push_back(newPort);
            }
        }
        else
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::GetSDPMediaPortNum() return FALSE! call id:" << strCallId << " ,SDP:\n" << m_strSDP << endl);
            m_portManager.getAFreeEvenPort(strCallId, newPort);
            vectNewPorts.push_back(newPort);
        }
        pSender->addLocalRecvPorts(vectNewPorts);

        //voip to cs
        std::string strAppSDP, strCsSipMsg, strSdpIp;
        bool result = false;
        if(false == SipMsg::CreateSipMsgFromSDP(m_strSDP, strCsSipMsg))
        {
            reason = "to caller, CreateSipMsgFromSDP() return FALSE with sdp of cs:" + m_strSDP;
            break;
        }
        if (false == SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strCsSipMsg, vectSdpMediaInfo))
        {
            reason = "to caller, getMediaIpAndPortsOfSDPinSipMsg() return FALSE with cs sip msg:" + strCsSipMsg;
            break;
        }
        if (vectSdpMediaInfo.size() < 1)
        {
            reason = "to caller, get ip and ports failed with cs sip msg:" + strCsSipMsg;
            break;
        }
        result = pSender->generateMediaStreamConnectionToSBCEx(vectSdpMediaInfo);
        if (false == result)
        {
            reason = "to caller, failed to build connection to CS!";
            break;
        }

        std::string strNewSdp;
        if (mrfpPres)
        {
            StSdpSessDesc stSdp, stNegoSdp;
            StSdpCodec sdpCode;
            SipMsg::readSdpParams(m_strSDP, stSdp);
            SipMsg::negoMrfpSdp(stSdp, stNegoSdp, m_sdpCodec, sdpCode);
            if (sdpCode.iPayload == ZMAXUCHAR)
            {
                TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::negoSdp return FALSE! call id:" << strCallId << " ,SDP:\n" << m_strSDP << endl);
                reason == "SipMsg::negoSdp mrfp failed";
                break;
            }
            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_SIP_STATUS_CODE, std::to_string(sdpCode.iPayload)));

            result = SipMsg::createSdp(stNegoSdp, m_strListenIp, vectNewPorts, strNewSdp);
            if (!result)
            {
                reason = "voip -> cs, mrfp createSdp failed!";
                break;
            }
            outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP, strNewSdp));

            newPort = 0;
            if (!m_portManager.getAFreeEvenPort(strCallId, newPort))
            {
                reason = "mrfp getAFreeEvenPort() return FALSE!";
                break;
            }

            vectMrfpPorts.push_back(newPort);
            outParams["rtpGw.port"] = std::to_string(newPort);
            outParams["rtpGw.ip"] = m_strListenIp;
        }
        pSender->setCaller(1);
        vectCallerPorts.swap(vectNewPorts);

        //分配被叫媒体
        vector<int> vectNewPorts2;
        newPort = 0;
        for (int i = 0; i < iConfgSDPPortNum; i++)
        {
            if (!m_portManager.getAFreeEvenPort(strCallId, newPort))
            {
                reason = "getAFreeEvenPort() return FALSE!";
                break;
            }
            vectNewPorts2.push_back(newPort);
        }

        if (strNewSdp.empty())
            strNewSdp = m_strSDP;

        std::string ip = isPrivateIp ? m_strPrivateIp : m_strListenIp;
        if (!SipMsg::CreateSDPWithNewPort(strNewSdp, vectNewPorts2, ip, strNewConfigSDP))
        {
            reason = " to callee voip -> cs, CreateSDPWithNewPort() return FALSE when sdp offer is :" + m_strSDP;
            break;
        }
        outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP, strNewConfigSDP));
        vectCalleePorts.swap(vectNewPorts2);

        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, callee voip -> cs, call id: " << strCallId << ", isPrivateIp:" << isPrivateIp << " ,modified app SDP:\n" << strNewConfigSDP << endl);
    } while (0);

    if (!reason.empty())
    {
        outParams.insert(make_pair(CallEx::CALL_SERVER_TERMINATE_REASON_KEY, reason));
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "called,and return FALSE! call id:" << strCallId << ",reason:" << reason << endl);
    }
    return reason.empty() ? true : false;
}

bool SipRtpImp::rtpConnectionBuild2(RtpSenderPtr pSender, const std::map<std::string, std::string>& params, std::map<std::string, std::string>& outParams, const std::string & sdp, vector<int> &vectPorts, std::string & reason)
{
    bool result = true;
    std::string sipGatewayOid;
    vector<int> vectNewPorts;
    int iConfgSDPPortNum = 0;
    std::string strNewConfigSDP, strSipMsgWithSDP;
    int newPort = 0;
    std::string strCallId ;
    std::string strParams;
    do
    {
        strParams = SipMsgCommon::encodeMapParasLog(params);
        map<std::string, std::string> ::const_iterator iterCallId = params.find(CallEx::CALL_SERVER_MAP_KEY_CALL_ID_EX);
        if (iterCallId != params.end())
        {
            strCallId = iterCallId->second;
        }
        //pSender->setApplication(_application, m_strListenIp);
        pSender->setCallId(strCallId);

        pSender->addLocalRecvPorts(vectPorts);

        if (!SipMsg::CreateSipMsgFromSDP(sdp, strSipMsgWithSDP))
        {
            reason = "SipMsg::CreateSipMsgFromSDP() return FALSE";
            break;
        }

        //step 2 : 创建被叫媒体通道
        vector<SDPMediaInfo> vectSdpMediaInfo;
        if (false == SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strSipMsgWithSDP, vectSdpMediaInfo))
        {
            reason = "cs -> voip, getMediaIpAndPortsOfSDPinSipMsg() return FALSE with cs sip msg:" + sdp;
            break;
        }
        if (vectSdpMediaInfo.size() < 1)
        {
            reason = "cs -> voip, get ip and ports failed with cs sip msg:" + sdp;
            break;
        }
        result = pSender->generateMediaStreamConnectionToSBCEx(vectSdpMediaInfo);
        if (false == result)
        {
            reason = "cs -> voip, failed to build connection to CS!";
            break;
        }
        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, call id: " << strCallId << " ,callee sdp offer:\n" << strNewConfigSDP << endl);
        pSender->setCaller(0);
        outParams["rtpGw.ip"] = m_strListenIp;
        outParams[SipRtpGateway::RTP_GW_RTP_GATEWAY_OID] = m_strMainObjId;
    } while (0);
    if (false == reason.empty())
    {
        outParams.insert(make_pair(CallEx::CALL_SERVER_TERMINATE_REASON_KEY, reason));
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << " called,and return FALSE! cs -> voip call id:" << strCallId << ",reason:" << reason << endl);
    }
    return reason.empty() ? true : false;
}

bool SipRtpImp::rtpConnectionBuildMrfp(Rtp2RtpChannelPtr pSender, std::map<std::string, std::string>& outParams, vector<int> & vectPorts, const std::string & strCallId, std::string & reason)
{
    if (vectPorts.size() == 0)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "called, vectPorts == 0" << endl);
        return false;
    }

    //pSender->setApplication(_application, m_strListenIp);
    pSender->setCallId(strCallId);
    pSender->setCaller(2);
    pSender->addLocalRecvPorts(vectPorts);
    int port = vectPorts.at(0);
    return pSender->generateRtpListen(port);
}

bool SipRtpImp::getSdpParams(StSdpMediaDesc & sdpCodec)
{
    int size = ZOS_GET_TABLE_SIZE(SipRtpGateway::astSdpCodecTbl);
    for (int i = 0; i < size; i++)
    {
        if(SipRtpGateway::astSdpCodecTbl[i].iPayload == SipRtpGateway::EN_DFT_PAYLOAD_TEL_EVNT)
        {
            sdpCodec._dtmfCodec.iPayload = SipRtpGateway::astSdpCodecTbl[i].iPayload;
            sdpCodec._dtmfCodec.iBitRate = SipRtpGateway::astSdpCodecTbl[i].iSplRate;
            sdpCodec._dtmfCodec.pcName = SipRtpGateway::astSdpCodecTbl[i].pcName;
            sdpCodec._dtmfCodec.ucEncoding = EN_SDP_ENCODING_TEL_EVNT;
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "called, insert dtmf payload:" << sdpCodec._dtmfCodec.iPayload << ",dtmf pcName:" << sdpCodec._dtmfCodec.pcName << endl);
        }
        else
        {
            ZUCHAR ucEncoding = SipMsg::sdpGetStaticEncoding(SipRtpGateway::astSdpCodecTbl[i].iPayload);

            StSdpCodec codec;
            codec.iPayload = SipRtpGateway::astSdpCodecTbl[i].iPayload;
            codec.iBitRate = SipRtpGateway::astSdpCodecTbl[i].iSplRate;
            codec.pcName = SipRtpGateway::astSdpCodecTbl[i].pcName;
            codec.ucEncoding = ucEncoding;
            sdpCodec._vecCodec.push_back(codec);

            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "called, insert payload:" << codec.iPayload << ",pcName:" << codec.pcName << endl);
        }
    }

    return true;
}

bool SipRtpImp::rtpConnectionBuild2(RtpSenderPtr pSender, const std::string & callId, const std::map<std::string, std::string>& params,std::map<std::string, std::string>& outParams,std::string & reason,  JssmmeChannelPtr & pChannel)
{
    bool result = false;
    std::string sipGatewayOid;
    std::vector<int> vectNewPorts;
    int iConfgSDPPortNum = 0;
    std::string strNewConfigSDP;
    int newPort = 0;

    do 
    {
        //pSender->setApplication(_application, m_strListenIp);
        pSender->setCallId(callId);

        std::string appSdpOffer, sipSdpOffer, appSdpAnswer;
        std::map<std::string, std::string> ::const_iterator iterKey = params.find(SipGateway::SIP_GW_MAP_KEY_APP_SDP_OFFER);
        if (iterKey != params.end())
        {
            appSdpOffer = iterKey->second;
        }
        else
        {
            reason = "appSdpOffer not Found!";
            break;
        }

        iterKey = params.find(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP);
        if (iterKey != params.end())
        {
            sipSdpOffer = iterKey->second;
        }
        else
        {
            reason = "sipSdpOffer not Found!";
            break;
        }

        iterKey = params.find(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP);
        if (iterKey != params.end())
        {
            appSdpAnswer = iterKey->second;
        }
        else
        {
            reason = "appSdpAnswer not Found!";
            break;
        }

        iterKey = params.find(CallEx::CALL_SERVER_MAP_KEY_SIPGW_ID);
        if (iterKey != params.end())
        {
            sipGatewayOid = iterKey->second;
            pSender->setSipGatewayOid(sipGatewayOid);
        }

        if (SipMsg::GetSDPMediaPortNum(sipSdpOffer, iConfgSDPPortNum))
        {
            for (int i = 0; i < iConfgSDPPortNum; i++)
            {
                if (!m_portManager.getAFreeEvenPort(callId, newPort))
                {
                    reason = "getAFreeEvenPort() return FALSE!";
                    break; 
                }
                vectNewPorts.push_back(newPort);
            }
            if (!reason.empty())
            {
                break;
            }
        }
        else
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::GetSDPMediaPortNum() return FALSE! call id:" << callId << " ,SDP:\n" << sipSdpOffer << endl);
            m_portManager.getAFreeEvenPort(callId, newPort);
            vectNewPorts.push_back(newPort);
        }
        pSender->addLocalRecvPorts(vectNewPorts);
        pSender->setCaller(true);

        pChannel->m_appSdpOffer = appSdpOffer;
        pChannel->m_appSdpAnswer = appSdpAnswer;
        pChannel->m_sipSdpOffer = sipSdpOffer;

        //nego app sdp
        StSdpSessDesc stSdp;
        StSdpCodec sdpCode;
        SipMsg::readSdpParams(appSdpAnswer, stSdp);

        StSdpCodec dtmfCode;
        int dtmfPayload = SipMsg::getDtmfPayload(stSdp, dtmfCode);
        if (dtmfPayload)
            pChannel->m_appDtmfPayload = dtmfPayload;

        int redPayload = SipMsg::getRedPayload(stSdp);
        pChannel->m_appRedPayload = redPayload;

        std::vector<StSdpCodec> vectSendCodec;
        SipMsg::getAcPayload(stSdp, vectSendCodec);
        vectSendCodec.push_back(sdpCode);
        pChannel->m_vectAppSendCodec = vectSendCodec;

        //nego sip sdp
        std::vector<SDPMediaInfo> vectSdpMediaInfo;
        std::string strCsSipMsg;
        if(!SipMsg::CreateSipMsgFromSDP(sipSdpOffer, strCsSipMsg))
        {
            reason = "cs -> voip, CreateSipMsgFromSDP() return FALSE with sdp of cs:" + sipSdpOffer;
            break;
        }
        if (!SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strCsSipMsg, vectSdpMediaInfo))
        {
            reason = "cs -> voip, getMediaIpAndPortsOfSDPinSipMsg() return FALSE with cs sip msg:" + strCsSipMsg;
            break;
        }
        if (vectSdpMediaInfo.size() < 1)
        {
            reason = "cs -> voip, get ip and ports failed with cs sip msg:" + strCsSipMsg;
            break;
        }
        result = pSender->generateMediaStreamConnectionToSBCEx(vectSdpMediaInfo);
        if (!result)
        {
            reason = "cs -> voip, failed to build connection to CS!";
            break;
        }

        //nego cs sdp
        StSdpSessDesc stCsSdp, stNegoSdp;
        StSdpCodec sdpCsCode;
        SipMsg::readSdpParams(sipSdpOffer, stCsSdp);
        SipMsg::negoSdp(stCsSdp, stNegoSdp, m_sipSdpDesc, sdpCsCode);
        if (sdpCsCode.iPayload == ZMAXUCHAR)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::negoSdp sip sdp return FALSE! call id:" << callId << " ,SDP:\n" << sipSdpOffer << endl);
            reason == "SipMsg::negoSdp sip failed";
            break;
        }

        StSdpCodec dtmfCsCode;
        dtmfPayload = SipMsg::getDtmfPayload(stCsSdp, dtmfCsCode);
        if (dtmfPayload)
            pChannel->m_sipDtmfPayload = dtmfPayload;

        redPayload = SipMsg::getRedPayload(stCsSdp);
        pChannel->m_sipRedPayload = redPayload;

        std::string strNewSdp;
        result = SipMsg::createSdp(stNegoSdp, m_strListenIp, vectNewPorts, strNewSdp);
        if (!result)
        {
            reason = "cs -> voip,  sip sdp answer createSdp failed!";
            break;
        }

        pChannel->m_sipSdpAnswer = strNewSdp;

        std::vector<StSdpCodec> vectSipSendCodec;
        vectSipSendCodec.push_back(sdpCsCode);
        pChannel->m_vectSipSendCodec = vectSipSendCodec;
        pChannel->m_sipMediaNegoStatus = kMediaNegoFinished;

        outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RSEEX_SDP, strNewSdp));
        outParams.insert(make_pair(CallEx::CALL_SERVER_MAP_KEY_RTPGW_SDP, strNewSdp));

        TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, cs -> voip, callId:  " << callId << ", sipGatewayOid:" << sipGatewayOid << ",pChannel->m_sipDtmfPayload:" << pChannel->m_sipDtmfPayload << ", sipSdpAnswer:\n" << strNewSdp << endl);
        outParams["rtpGw.ip"] = m_strListenIp;
        outParams[SipRtpGateway::RTP_GW_RTP_GATEWAY_OID] = m_strMainObjId;
    } while (0);

    if (false == reason.empty())
    {
        outParams.insert(make_pair(CallEx::CALL_SERVER_TERMINATE_REASON_KEY, reason));
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "called,and return FALSE! callId:" << callId << ",reason:" << reason << endl);
    }
    return reason.empty() ? true : false;
}

bool SipRtpImp::decodeCodecCfg(const std::string & cfg, std::set<std::string> & setCfgs)
{
    std::string codecCfgs(cfg);
    int pos = codecCfgs.find(";");
    while(pos > 0)
    {
        std::string str = codecCfgs.substr(0, pos);
        setCfgs.insert(codecCfgs.substr(0, pos));
        codecCfgs = codecCfgs.substr(pos +1);
        pos = codecCfgs.find(";");
    }

    if (codecCfgs.size() > 0)
    {
        setCfgs.insert(codecCfgs);
    }
}

bool SipRtpImp::getSdpParams(std::set<std::string> & strCfgs, StSdpMediaDesc & sdpCodec)
{
    int size = ZOS_GET_TABLE_SIZE(SipRtpGateway::astSdpCodecTbl);
    std::vector<int> acPayload;
    bool isRed = false;

    std::set<std::string>::iterator itor = strCfgs.begin();
    for (; itor != strCfgs.end(); itor++)
    {
        std::string code= *itor;
        for (int i = 0; i < size; i++)
        {
            if (code == SipRtpGateway::astSdpCodecTbl[i].pcName)
            {
                if(SipRtpGateway::astSdpCodecTbl[i].iPayload == SipRtpGateway::EN_DFT_PAYLOAD_AUDIO_RED)
                {
                    isRed = true;
                    sdpCodec._redCodec.stRed.ucRedPt = SipRtpGateway::astSdpCodecTbl[i].iPayload;
                    sdpCodec._redCodec.iPayload = SipRtpGateway::astSdpCodecTbl[i].iPayload;
                    sdpCodec._redCodec.iBitRate = SipRtpGateway::astSdpCodecTbl[i].iSplRate;
                    sdpCodec._redCodec.pcName = SipRtpGateway::astSdpCodecTbl[i].pcName;
                    sdpCodec._redCodec.ucEncoding = EN_SDP_ENCODING_ULPFEC;
                }
                else if(SipRtpGateway::astSdpCodecTbl[i].iPayload == SipRtpGateway::EN_DFT_PAYLOAD_TEL_EVNT)
                {
                    sdpCodec._dtmfCodec.iPayload = SipRtpGateway::astSdpCodecTbl[i].iPayload;
                    sdpCodec._dtmfCodec.iBitRate = SipRtpGateway::astSdpCodecTbl[i].iSplRate;
                    sdpCodec._dtmfCodec.pcName = SipRtpGateway::astSdpCodecTbl[i].pcName;
                    sdpCodec._dtmfCodec.ucEncoding = EN_SDP_ENCODING_TEL_EVNT;
                }
                else
                {
                    acPayload.push_back(SipRtpGateway::astSdpCodecTbl[i].iPayload);

                    ZUCHAR ucEncoding = SipMsg::sdpGetStaticEncoding(SipRtpGateway::astSdpCodecTbl[i].iPayload);

                    StSdpCodec codec;
                    codec.iPayload = SipRtpGateway::astSdpCodecTbl[i].iPayload;
                    codec.iBitRate = SipRtpGateway::astSdpCodecTbl[i].iSplRate;
                    codec.pcName = SipRtpGateway::astSdpCodecTbl[i].pcName;
                    codec.ucEncoding = ucEncoding;
                    sdpCodec._vecCodec.push_back(codec);
                }
                break;
            }
        }
    }

    if (isRed)
    {
        for (int i = 0; i < acPayload.size(); i++)
        {
            int pl = acPayload.at(i);
            sdpCodec._redCodec.stRed.aucPtLst[sdpCodec._redCodec.stRed.ucPtCnt++] = pl;
        }
    }
    
    return true;
}

void SipRtpImp::getAllSdpCodecCfgs(bool isApp, StSdpMediaDesc & stCfgSdp)
{
    stCfgSdp = isApp? m_appSdpDesc: m_sipSdpDesc;
}

void SipRtpImp::mediaChannelStatusNotify(const std::string &callId, const std::string & sipGatewayId, const std::string &reason)
{
    // class mediaChannelStatusNotify_async : virtual public Common::AgentAsync
    // {
    // public:
    //     mediaChannelStatusNotify_async(const std::string &callId, const std::string & sipGatewayId)
    //         :_callId(callId), _sipGatewayId(sipGatewayId){}

    //     void cmdResult(int rslt,const Common::IputStreamPtr & iput,const Common::ObjectPtr& userdata)
    //     {
    //         if (!SipGateway::SipGatewayNotifyAgent::mediaChannelStatusNotify_end(rslt, iput))
    //         {
    //             UTIL_LOG_WRN("RtpGateway", "mediaChannelStatusNotify failed callId:" + _callId + ", sipGatewayId:" + _sipGatewayId + ", reason:" + Common::ObjectAgent::getLastReason());
    //         }
    //     }

    // private:
    //     const std::string _callId;
    //     const std::string _sipGatewayId;
    // };

    // UTIL_LOG_IFO("RtpGateway", "mediaChannelStatusNotify callId:" + callId + ", sipGatewayId:" + sipGatewayId);

    // SipGateway::SipGatewayNotifyAgent agent = _application->createAgent(sipGatewayId);
    // agent.mediaChannelStatusNotify_begin(new mediaChannelStatusNotify_async(callId, sipGatewayId), callId, reason, std::map<std::string, std::string>());
}

bool SipRtpImp::getMediaEmptyPkgChk()
{
    return m_mediaEmptyPkgCheckPres;
}

int SipRtpImp::getMediaEmptyPkgChkInterval()
{
    return m_mediaEmptyPkgChkInterval;
}

double SipRtpImp::getMediaEmptyPkgChkPercent()
{
    return m_mediaEmptyPkgChkPercent;
}

int SipRtpImp::getMediaEmptyPkgMinTime()
{
    return m_mediaEmptyPkgChkMinTime;
}

void SipRtpImp::processReConnMediaNego(Rtp2RtpChannelPtr& callerChannel, Rtp2RtpChannelPtr& calleeChannel, const std::string& callId, const std::string& sdp, bool isRecord)
{
    if(!decodeSdp(callerChannel, sdp))
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << "() called, decodeSdp caller sdp failed callId: " << callId << endl);
        return;
    }

    if(!decodeSdp(calleeChannel, sdp))
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << "() called, decodeSdp calleeChannel sdp failed callId: " << callId << endl);
        return;
    }

    int callerDtmf = callerChannel->getDtmfPayload();
    int calleeDtmf = calleeChannel->getDtmfPayload();
    int callerChannelId = callerChannel->getChannelId();
    int calleeChannelId = calleeChannel->getChannelId();

    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, callId: " << callId << ", callerChannelId:" << callerChannelId << ", calleeChannelId:" << calleeChannelId
        << ", callerDtmf:" << callerDtmf << ", calleeDtmf:" << calleeDtmf << ", isRecord:" << isRecord << endl);

    m_jssmmeInterface->disconnectChannel(callerChannelId, calleeChannelId);

    if (callerDtmf > 0 && calleeDtmf > 0)
        m_jssmmeInterface->connectChannel(callerChannelId, calleeChannelId, callerDtmf, calleeDtmf);
    else
        m_jssmmeInterface->connectChannel(callerChannelId, calleeChannelId, -1, -1);
}

bool SipRtpImp::processMediaNego(Rtp2RtpChannelPtr& callerChannel, Rtp2RtpChannelPtr& calleeChannel, const std::string& callId, const std::string& sdp, bool isRecord, bool isNegoFinished)
{
    if(!decodeSdp(callerChannel, sdp))
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << "() called, decodeSdp caller sdp failed callId: " << callId << endl);
        return false;
    }

    if (!createChannels(callerChannel))
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << "() called, create caller Channels failed callId: " << callId << endl);
        return false;
    }

    if(!decodeSdp(calleeChannel, sdp))
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << "() called, decodeSdp calleeChannel sdp failed callId: " << callId << endl);
        return false;
    }

    if (!createChannels(calleeChannel))
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << "() called, create callee Channels failed callId: " << callId << endl);
        return false;
    }

    int callerDtmf = callerChannel->getDtmfPayload();
    int calleeDtmf = calleeChannel->getDtmfPayload();
    int callerChannelId = callerChannel->getChannelId();
    int calleeChannelId = calleeChannel->getChannelId();

    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, callId: " << callId << ", callerChannelId:" << callerChannelId << ", calleeChannelId:" << calleeChannelId
        << ", callerDtmf:" << callerDtmf << ", calleeDtmf:" << calleeDtmf << ", isRecord:" << isRecord << ", isNegoFinished:" << isNegoFinished << endl);

    if (callerDtmf > 0 && calleeDtmf > 0)
        m_jssmmeInterface->connectChannel(callerChannelId, calleeChannelId, callerDtmf, calleeDtmf);
    else
        m_jssmmeInterface->connectChannel(callerChannelId, calleeChannelId, -1, -1);

    callerChannel->setChannelPres(true);
    calleeChannel->setChannelPres(true);

    if (isNegoFinished)
    {
        callerChannel->setAnswerTime();
        calleeChannel->setAnswerTime();
    }

    if (isRecord && !callerChannel->getRecordPres() && isNegoFinished)
    {
        std::string path = m_strRecordPath + "/" + callerChannel->getRecordTime() + "_" + callId + "_" + callerChannel->getCallNumber(true) + "_" + callerChannel->getCallNumber(false) + ".wav";
        if (!m_jssmmeInterface->startCallRecord(callerChannelId, path))
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << " called, startCallRecord failed callId:" << callId << ", path:" << path << endl);
            return true;
        }
        callerChannel->setRecordPres(true);
        TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, startCallRecord success callId:" << callId << ", path:" << path << endl);
    }

    return true;
}

bool SipRtpImp::decodeSdp(Rtp2RtpChannelPtr& pChannel, const std::string& sdp)
{
    StSdpSessDesc stSdp;
    StSdpCodec sdpCode;
    SipMsg::readSdpParams(sdp, stSdp);

    std::vector<StSdpCodec> acCode;
    SipMsg::getAcPayload(stSdp, acCode);
    if (acCode.empty())
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, acCode is empty, callId:" << pChannel->getCallId() << endl);
        return false;
    }

    pChannel->setAcPayload(acCode);

    StSdpCodec dtmfCode;
    int dtmfPayload = SipMsg::getDtmfPayload(stSdp, dtmfCode);
    if (dtmfPayload)
        pChannel->setDtmfPayload(dtmfPayload);

    int redPayload = SipMsg::getRedPayload(stSdp);
    pChannel->setRedPayload(redPayload);

    return true;
}

bool SipRtpImp::createChannels(Rtp2RtpChannelPtr& pChannel)
{
    int channelId = m_jssmmeInterface->createChannel();
    if (channelId < 0)
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, channel create failed, callId:" << pChannel->getCallId() << endl);
        return false;
    }

    pChannel->setChannelId(channelId);

    std::vector<StSdpCodec> vectSendCodec = pChannel->getAcPayload();

    if (vectSendCodec.size() <= 0)
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, vectSendCodec == 0, callId:" << pChannel->getCallId() << endl);
        return false;
    }

    StSdpCodec codec = vectSendCodec.at(0);
    if (codec.pcName.empty())
    {
        TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called,codec.pcName is empty, callId:" << pChannel->getCallId() << endl);
        return false;
    }

    m_jssmmeInterface->setSendCodec(channelId, codec.pcName);
    m_jssmmeInterface->setCallBack(channelId, *pChannel);

    int red = pChannel->getRedPayload();
    if (red > 0)
    {
        m_jssmmeInterface->setFecStatus(channelId, red);
        m_jssmmeInterface->setSendRedCodec(channelId, red);
    }

    TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, callId:" << pChannel->getCallId() << ", channelId:" << channelId << endl);
    return true;
}

SipRtpGateway::RtpGatewayInterfaceIPtr SipRtpImp::getJssmmeInterface()
{
    return m_jssmmeInterface;
}

RtpSender::RtpSender(const SipRtpImpPtr & server, const std::string & listenIp, const std::string & callId, JssmmeManagerPtr jssmmeManager)
:m_listenIp(listenIp), m_strCallId(callId)
{
    m_pServer = server;
    m_createTicks = TNOWMS;

    m_audioToSBC = 0;
    m_audioToApp = 0;
    m_sendToSBCPackets = 0;
    m_sendToSBCFailedPackets = 0;
    m_sendToAppPackets = 0;
    m_sendToAppFailedPackets = 0;

    m_audio_RTP_ConnectionPtr = 0;
    m_audio_RTCP_ConnectionPtr = 0;
    m_video_RTP_ConnectionPtr = 0;
    m_video_RTCP_ConnectionPtr = 0;

    m_bInitConnected = false;
    m_csCallApp = true;
    m_sessionMediaType = kSessionMediaTypeUnset;

    m_startTimeToApp = 0;
    m_lastTimeToApp = 0;
    m_startTimeToSbc = 0;
    m_lastTimeToSbc = 0;
    m_isCaller = false;

    m_jssmmeManager = jssmmeManager;

    TLOGINFO("[RtpGateway] RtpSender() called, callId:" << m_strCallId << endl);
}

RtpSender::~RtpSender()
{
    TLOGINFO("[RtpGateway] ~RtpSender() called, callId:" << m_strCallId << endl);
}

SipGateway::GatewayConnectionPtr  RtpSender::newAGatewayConnectionEx(int localPort, const std::string & strIp, int remotePort, SipGateway::TransType transType)
{
    // 使用虚函数获取 shared_ptr，避免菱形继承问题
    SipGateway::GatewayConnectionPtr pConnec = std::make_shared<SipGateway::GatewayConnection>(getSharedPtr(), transType);
    Common::NetSenderPtr psender;// = m_application->getDriver()->connect("udp","", localPort, strIp, remotePort, pConnec, true);
    if (psender == 0)
    {
        std::string reason = ",errno:" + std::to_string(errno);
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called,CAN NOT connected,localPort:" << localPort << ",remote ip:" << strIp << " ,port:" << remotePort << " ,call id:" << m_strCallId << ",reason:" << reason << endl);
        return 0;
    }
    pConnec->setSender(psender);

    std::string strRemoteHost,strLocalHost;
    int iRemotePort, iLocalPort = 0;
    if (psender != 0)
    {
        psender->getRemote(strRemoteHost,iRemotePort);
        psender->getLocal(strLocalHost, iLocalPort);
    }

    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called,local ip:" << strLocalHost << ", port:" << localPort
        << ",remote ip:" << strIp << " ,port:" << iRemotePort << " ,call id:" << m_strCallId << endl);

    return pConnec;
}

bool RtpSender::insertAConnection(const SipGateway::GatewayConnectionPtr & ptr)
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> temLock(m_mutexConnetionMap);
    std::string strConnectionId = ptr->getConnectionId();
    std::map<std::string, SipGateway::GatewayConnectionPtr>::iterator iterConnection = m_mapConnections.find(strConnectionId);
    if (iterConnection != m_mapConnections.end())
        return false;

    m_mapConnections.insert(make_pair(strConnectionId, ptr));
    return true;
}

bool RtpSender::removeAllConnections()
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> temLock(m_mutexConnetionMap);
    if (m_mapConnections.size() == 0)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called : there is no connection exists in the map, call id::" << m_strCallId << endl);
        return false;
    }

    SipGateway::GatewayConnectionPtr p;
    std::map<std::string,  SipGateway::GatewayConnectionPtr>::iterator iterConnection = m_mapConnections.begin();
    for (; iterConnection != m_mapConnections.end(); )
    {
        {
            p = iterConnection->second;
            if (p)
                p->clearUp();
        }
        m_mapConnections.erase(iterConnection++);
    }
    return true;
}

bool RtpSender::insertExceptionConnection(const  SipGateway::GatewayConnectionPtr & pConnection)
{
    if (0 == pConnection)
        return false;

    std::string strConnectionId = pConnection->getConnectionId();
    tars::TC_LockT<tars::TC_ThreadRecMutex> temLock(m_mutexExcepConnection);
    std::map<std::string, SipGateway::GatewayConnectionPtr>::iterator iterConnection = m_mapExcepConnetions.find(strConnectionId);
    if (iterConnection != m_mapExcepConnetions.end())
        return false;

    m_mapExcepConnetions.insert(make_pair(strConnectionId, pConnection));
    return true;
}

bool RtpSender::removeAllExcepConnections()
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> temLock(m_mutexExcepConnection);
    if (m_mapExcepConnetions.size() == 0)
        return true;

    SipGateway::GatewayConnectionPtr p;
    std::map<std::string,  SipGateway::GatewayConnectionPtr>::iterator iterConnection = m_mapExcepConnetions.begin();
    for (; iterConnection != m_mapExcepConnetions.end(); )
    {
        {
            p = iterConnection->second;
            if (p)
                p->clearUp();
        }
        m_mapExcepConnetions.erase(iterConnection++);
    }
    return true;
}

// void RtpSender::setApplication(const tars::ApplicationPtr & _application  ,const std::string & listenIp)
// {
//     m_application = _application;
//     m_strListenIp = listenIp;
// }

bool RtpSender::generateMediaStreamConnectionToSBCEx(const std::vector<SDPMediaInfo> & vectSdpMediaInfo)
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexConnections);
    int savedRtpSeq = 0;
    bool vedioDialog = false;

    if (m_vectLocalRecMediaPorts.size() == 0)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, no available ports ,call id:" << m_strCallId << endl);
        return false;
    }

    //会话描述列表循环
    for (unsigned  int i = 0; i < vectSdpMediaInfo.size(); i++)
    {
        SDPMediaInfo temMediaInfo = vectSdpMediaInfo.at(i);
        std::string strIp = temMediaInfo._strIp;
        m_sbcIp = strIp;

        //单个会话描述中的媒体描述列表循环
        for (unsigned int k = 0; k < temMediaInfo._vecMediaPort.size(); k ++)
        {
            SDPMediaPort medioPort = temMediaInfo._vecMediaPort.at(k);
            SDPMediaReceiveIpAndPort medioReceivePort;
            medioReceivePort.m_strIp = strIp;
            medioReceivePort.m_bIsAudio = medioPort._bIsAudio;
            medioReceivePort.m_bForbitedStream = medioPort._bMediaForbit;
            medioReceivePort.m_iPort = medioPort._port;

            int localRtpPort = m_vectLocalRecMediaPorts.at(savedRtpSeq);
            int localRtcpPort = m_vectLocalRecMediaPorts.at(savedRtpSeq + 1);

            if (medioPort._bIsAudio)
            {
                //音频 RTP
                if (medioPort._port == 0)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, Audio port == 0, call id:" << m_strCallId << endl);
                    continue;
                }

                m_audio_RTP_ConnectionPtr = newAGatewayConnectionEx(localRtpPort, strIp, medioPort._port, SipGateway::TransTypeAudio);
                if (0 == m_audio_RTP_ConnectionPtr)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, newAGatewayConnectionEx()return FALSE, audio RTP ! remote port: "
                        << medioPort._port << " ,call id:" << m_strCallId << endl);
                    return false;
                }
                bool result = insertAConnection(m_audio_RTP_ConnectionPtr);
                if (!result)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTP FAILED!local port:"
                        << localRtpPort << ",remote port: " << medioPort._port << " ,call id:" << m_strCallId << endl);
                }
                m_vectRemoteRecMediaPorts.push_back(medioPort._port);

                //音频 RTCP
                m_audio_RTCP_ConnectionPtr = newAGatewayConnectionEx(localRtcpPort, strIp, medioPort._port + 1, SipGateway::TransTypeAudio);
                if (0 == m_audio_RTCP_ConnectionPtr)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, newAGatewayConnectionEx()return FALSE, audio RTCP ! remote port: "
                        << (medioPort._port + 1) << " ,call id:" << m_strCallId << endl);
                    return false;
                }
                result = insertAConnection( m_audio_RTCP_ConnectionPtr);
                if (!result)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTCP FAILED! local port:"
                        << localRtcpPort << ",remote port: " << (medioPort._port +1) << " ,call id:" << m_strCallId << endl);
                }
                m_vectRemoteRecMediaPorts.push_back(medioPort._port + 1);

                TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, Audio Call,NEW RTP connetion, local port:" << localRtpPort << ",remote port:" << medioPort._port <<
                    ",NEW RTCP connection,local port:" << localRtcpPort << ",remote port: " << (medioPort._port + 1) << " ,call id:" << m_strCallId << endl);
            }
            else
            {
                //视频 RTP
                if (medioPort._port == 0)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, Video port == 0, call id:" << m_strCallId << endl);
                    continue;
                }

                m_video_RTP_ConnectionPtr = newAGatewayConnectionEx(localRtpPort, strIp, medioPort._port, SipGateway::TransTypeVideo);
                if (0 == m_video_RTP_ConnectionPtr)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, newAGatewayConnectionEx()return FALSE, videa RTP ! remote port: "
                        << medioPort._port << " ,call id:" << m_strCallId << endl);
                    return false;
                }
                bool result = insertAConnection(m_video_RTP_ConnectionPtr);
                if (!result)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection videa RTP FAILED! local port:"
                        << localRtpPort << ",remote port: " << medioPort._port << " ,call id:" << m_strCallId << endl);
                }
                m_vectRemoteRecMediaPorts.push_back(medioPort._port);

                //视频 RTCP
                m_video_RTCP_ConnectionPtr = newAGatewayConnectionEx(localRtcpPort, strIp, medioPort._port + 1, SipGateway::TransTypeVideo);
                if (0 == m_video_RTCP_ConnectionPtr)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, newAGatewayConnectionEx()return FALSE, videa RTCP ! remote port: "
                        << (medioPort._port + 1) << " ,call id:" << m_strCallId << endl);
                    return false;
                }
                result = insertAConnection(m_video_RTCP_ConnectionPtr);
                if (!result)
                {
                    TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection videa RTCP FAILED! local port:"
                        << localRtcpPort << ",remote port: " << (medioPort._port +1) << " ,call id:" << m_strCallId << endl);
                }
                m_vectRemoteRecMediaPorts.push_back(medioPort._port + 1);

                TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, Video Call,NEW RTP connetion, local port:" << localRtpPort << ",remote port:" << medioPort._port <<
                    ",NEW RTCP connection,local port:" << localRtcpPort << ",remote port: " << (medioPort._port + 1) << " ,call id:" << m_strCallId << endl);

                vedioDialog = true;
            }

            medioReceivePort.m_localPort = localRtpPort;
            m_vectMedioIpAndPorts.push_back(medioReceivePort);
            savedRtpSeq = savedRtpSeq + 2;

        }//iterate media description
    }//iterate session description

    m_sessionMediaType = vedioDialog ? kSessionMediaTypeVideo : kSessionMediaTypeAudio;
    m_bInitConnected = true;
    return true;
}

bool RtpSender::refreshMediaSteamConnectionToSBC(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, const std::string& strCsSDP, std::string& strModifiedAppSDP)
{
    bool result = false;
    SessionUpdateEvent updateEvent;

    result = this->getSessionMediaChangeEvent(vectSdpMediaInfo, updateEvent);
    if (false == result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, getSessionMediaChangeEvent()return FALSE,call id:" << m_strCallId << ",m_updateType:" << updateEvent.m_updateType << endl);
        return false;
    }
    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called, getSessionMediaChangeEvent()return TRUE,call id:" 
        << m_strCallId << ",m_updateType:" << updateEvent.m_updateType << endl);

    switch(updateEvent.m_updateType)
    {
    case  kSessionAudioPortChange:
        result = this->procAudioPortChangeEvent(updateEvent);
        if (false == result)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, procAudioPortChangeEvent()return FALSE,call id:" << m_strCallId << endl);
            return false;
        }
        break;

    case kSessionVideoPortChange:
        result = this->procVideoPortChangeEvent(updateEvent);
        if (false == result)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, procVideoPortChangeEvent()return FALSE,call id:" << m_strCallId << endl);
            return false;
        }
        break;

    case kSessionVideoOpened:
        result = this->procVedioOpenedEvent(updateEvent);
        if (false == result)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, procVedioOpenedEvent()return FALSE,call id:" << m_strCallId << endl);
            return false;
        }
        break;

    case  kSessionVideoClosed:
        result = this->procVedioClosedEvent(updateEvent);
        if (false == result)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, procVedioClosedEvent()return FALSE,call id:" << m_strCallId << endl);
            return false;
        }
        break;

    case kSessionMediaIpChange:
        result = this->procSBCIpChangeEvent(updateEvent);
        if (false == result)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, procSBCIpChangeEvent()return FALSE,call id:" << m_strCallId << endl);
            return false;
        }
        break;

    case kSessionMediaNoChange:
        return true;
    }

    if (!strCsSDP.empty())
    {
        int newAudioPort = 0, newVideoPort = 0;
        for (unsigned int i = 0; i < m_vectMedioIpAndPorts.size(); i++)
        {
            SDPMediaReceiveIpAndPort temMediaInfo = m_vectMedioIpAndPorts.at(i);
            if (temMediaInfo.m_bIsAudio)
            {
                newAudioPort = temMediaInfo.m_localPort;
            }
            else
            {
                newVideoPort = temMediaInfo.m_localPort;
            }
        }
        if (!SipMsg::CreateSDPWithNewPort(strCsSDP, newAudioPort, newVideoPort, m_strListenIp, strModifiedAppSDP))
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, SipMsg::CreateSDPWithNewPort() return FALSE! strCallId:"
                << m_strCallId << ",newAudioPort:" << newAudioPort << ",newVideoPort:" << newVideoPort << endl);
            return false;
        }
        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called and return TRUE! strCallId:"
            << m_strCallId << ",media port size:" << int(m_vectMedioIpAndPorts.size()) << ",newAudioPort:"
            << newAudioPort << ",newVideoPort:" << newVideoPort << ",strModifiedAppSDP:\n" << strModifiedAppSDP << endl);
    }

    return true;
}

bool RtpSender::getSessionMediaChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent)
{
    bool result = false;
    std::string sbcIp;
    bool bVideo = false;
    SessionMediaType newSessionMediaType;

    result = this->getMediaIpAndType(vectSdpMediaInfo, sbcIp, bVideo);
    if (false == result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, getMediaIpAndType()return FALSE,call id:" << m_strCallId << endl);
        return false;
    }
    newSessionMediaType = bVideo ? kSessionMediaTypeVideo : kSessionMediaTypeAudio;

    if (m_sbcIp != sbcIp)
    {
        return this->generateSBCIpChangeEvent(vectSdpMediaInfo, updateEvent);
    }
    else if (m_sessionMediaType == kSessionMediaTypeAudio && newSessionMediaType == kSessionMediaTypeAudio)
    {
        return this->generateAudioPortChangeEvent(vectSdpMediaInfo, updateEvent);
    }
    else if (m_sessionMediaType == kSessionMediaTypeVideo && newSessionMediaType == kSessionMediaTypeVideo)
    {
        return this->generateVideoPortChangeEvent(vectSdpMediaInfo, updateEvent);
    }
    else if (m_sessionMediaType == kSessionMediaTypeAudio && newSessionMediaType == kSessionMediaTypeVideo)
    {
        return this->generateVedioOpenedEvent(vectSdpMediaInfo, updateEvent);
    }
    else if (m_sessionMediaType == kSessionMediaTypeVideo && newSessionMediaType == kSessionMediaTypeAudio)
    {
        return this->generateVedioClosedEvent(vectSdpMediaInfo, updateEvent);
    }
    else
    {
        updateEvent.m_updateType = kSessionMediaNoChange;
    }
    return true;
}

bool RtpSender::getMediaIpAndType(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, std::string & sbcIp, bool & bVideo)
{
    if (vectSdpMediaInfo.size() == 0)
        return false;

    bVideo = false;
    //session description iterate
    for (unsigned int i = 0; i < vectSdpMediaInfo.size(); i++)
    {
        SDPMediaInfo sdpMediaInfo = vectSdpMediaInfo.at(i);
        sbcIp = sdpMediaInfo._strIp;

        if (sdpMediaInfo._vecMediaPort.size() == 0)
            return false;

        for (unsigned int k = 0; k < sdpMediaInfo._vecMediaPort.size(); k++)
        {
            SDPMediaPort mediaPort = sdpMediaInfo._vecMediaPort.at(k);
            if (mediaPort._bIsAudio == false && mediaPort._bMediaForbit == false)
                bVideo = true;
        }
    }
    return true;
}

bool RtpSender::generateSBCIpChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent)
{
    bool result = false;
    std::string sbcIp;
    bool bVideo = false;
    SDPMediaInfo mediaInfo;

    this->getMediaIpAndType(vectSdpMediaInfo, sbcIp, bVideo);
    updateEvent.m_updateType = kSessionMediaIpChange;
    updateEvent.m_oldMediaType = m_sessionMediaType;
    updateEvent.m_newMediaType = bVideo ? kSessionMediaTypeVideo : kSessionMediaTypeAudio;
    updateEvent.m_oldLocalPorts = m_vectLocalRecMediaPorts;

    updateEvent.m_oldSBCIp = m_sbcIp;
    updateEvent.m_newSBCIp = sbcIp;
    mediaInfo = vectSdpMediaInfo.at(0);                 //默认只有一个会话描述
    m_sbcIp = sbcIp;
    m_sessionMediaType = updateEvent.m_newMediaType;

    for (unsigned int i = 0; i < mediaInfo._vecMediaPort.size(); i++)
    {
        SDPMediaPort mediaPort = mediaInfo._vecMediaPort.at(i);
        if (mediaPort._bIsAudio)
            updateEvent.m_newSBCAudioPort = mediaPort._port;
        else
            updateEvent.m_newSBCVideoPort = mediaPort._port;
    }
    return true;
}

bool RtpSender::generateAudioPortChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent)
{
    SDPMediaReceiveIpAndPort reveiveIpPort;
    bool bAudioExist = false;
    int oldSBCAudioPort = 0;
    int newSBCAudioPort = 0;
    SDPMediaInfo mediaInfo;
    std::string reason;

    mediaInfo = vectSdpMediaInfo.at(0);                 //默认只有一个会话描述
    updateEvent.m_updateType = kSessionAudioPortChange;
    for (unsigned int i = 0; i < m_vectMedioIpAndPorts.size(); i++)
    {
        if (m_vectMedioIpAndPorts.at(i).m_bIsAudio)
        {
            bAudioExist = true;
            reveiveIpPort = m_vectMedioIpAndPorts.at(i);
            break;
        }
    }

    try
    {
        if (false == bAudioExist)
        {
            reason = "cann't find local audio port info";
            throw false;
        }
        if (mediaInfo._vecMediaPort.size() == 0)
        {
            reason = "sbc sdp port num is 0";
            throw false;
        }

        oldSBCAudioPort = reveiveIpPort.m_iPort;
        for (unsigned int i = 0; i < mediaInfo._vecMediaPort.size(); i++)
        {
            SDPMediaPort mediaPort = mediaInfo._vecMediaPort.at(i);
            if (mediaPort._bIsAudio)
            {
                newSBCAudioPort = mediaPort._port;
                break;
            }
        }

        if (0 == newSBCAudioPort)
        {
            updateEvent.m_updateType = kSessionMediaNoChange;
            reason = "cann't find audio port info";
            throw false;
        }
        if (oldSBCAudioPort == newSBCAudioPort)
        {
            updateEvent.m_updateType = kSessionMediaNoChange;
            reason = "sbc audio port not changed,newSBCAudioPort:" + std::to_string(newSBCAudioPort);
            throw false;
        }

        updateEvent.m_oldLocalAudioPort = reveiveIpPort.m_localPort;
        updateEvent.m_oldSBCAudioPort = reveiveIpPort.m_iPort;
        updateEvent.m_newSBCAudioPort = newSBCAudioPort;
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << ",newSBCAudioPort:" << newSBCAudioPort << ",reason:" << reason << endl);
        }
        else
        {
            TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << ",newSBCAudioPort:" << newSBCAudioPort << endl);
        }
        return ex;
    }
}

bool RtpSender::generateVideoPortChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent)
{
    SDPMediaReceiveIpAndPort reveiveIpPort;
    bool bVideoExist = false;
    int oldSBCVideoPort = 0;
    int newSBCVideoPort = 0;
    SDPMediaInfo mediaInfo;
    std::string reason;

    mediaInfo = vectSdpMediaInfo.at(0);                 //默认只有一个会话描述
    updateEvent.m_updateType = kSessionVideoPortChange;
    for (unsigned int i = 0; i < m_vectMedioIpAndPorts.size(); i++)
    {
        if (false == m_vectMedioIpAndPorts.at(i).m_bIsAudio)
        {
            bVideoExist = true;
            reveiveIpPort = m_vectMedioIpAndPorts.at(i);
            break;
        }
    }

    try
    {
        if (false == bVideoExist)
        {
            reason = "cann't find local video port info";
            throw false;
        }
        if (mediaInfo._vecMediaPort.size() == 0)
        {
            reason = "sbc sdp port num is 0";
            throw false;
        }

        oldSBCVideoPort = reveiveIpPort.m_iPort;
        for (unsigned int i = 0; i < mediaInfo._vecMediaPort.size(); i++)
        {
            SDPMediaPort mediaPort = mediaInfo._vecMediaPort.at(i);
            if (false == mediaPort._bIsAudio)
            {
                newSBCVideoPort = mediaPort._port;
                break;
            }
        }

        if (0 == newSBCVideoPort)
        {
            updateEvent.m_updateType = kSessionMediaNoChange;
            reason = "cann't find video port info";
            throw false;
        }
        if (oldSBCVideoPort == newSBCVideoPort)
        {
            updateEvent.m_updateType = kSessionMediaNoChange;
            reason = "sbc video port not changed, newSBCAudioPort:" + std::to_string(newSBCVideoPort);
            throw false;
        }

        updateEvent.m_oldLocalVideoPort = reveiveIpPort.m_localPort;
        updateEvent.m_oldSBCVideoPort = reveiveIpPort.m_iPort;
        updateEvent.m_newSBCVideoPort = newSBCVideoPort;

        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        else
        {
            TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        return ex;
    }
}

bool RtpSender::generateVedioOpenedEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent)
{
    int newSBCVideoPort = 0;
    SDPMediaInfo mediaInfo;
    std::string reason;

    mediaInfo = vectSdpMediaInfo.at(0);                 //默认只有一个会话描述
    updateEvent.m_updateType = kSessionVideoOpened;

    try
    {
        if (mediaInfo._vecMediaPort.size() == 0)
        {
            reason = "sbc sdp port num is 0";
            throw false;
        }

        for (unsigned int i = 0; i < mediaInfo._vecMediaPort.size(); i++)
        {
            SDPMediaPort mediaPort = mediaInfo._vecMediaPort.at(i);
            if (false == mediaPort._bIsAudio)
            {
                newSBCVideoPort = mediaPort._port;
                break;
            }
        }

        if (0 == newSBCVideoPort)
        {
            updateEvent.m_updateType = kSessionMediaNoChange;
            reason = "cann't find video port info";
            throw false;
        }
        updateEvent.m_newSBCVideoPort = newSBCVideoPort;
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        else
        {
            TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        return ex;
    }
}

bool RtpSender::generateVedioClosedEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent)
{
    SDPMediaReceiveIpAndPort reveiveIpPort;
    bool bVideoExist = false;
    int oldSBCVideoPort = 0;
    SDPMediaInfo mediaInfo;
    std::string reason;

    mediaInfo = vectSdpMediaInfo.at(0);                                   //默认只有一个会话描述
    updateEvent.m_updateType = kSessionVideoClosed;
    for (unsigned int i = 0; i < m_vectMedioIpAndPorts.size(); i++)
    {
        if (false == m_vectMedioIpAndPorts.at(i).m_bIsAudio)
        {
            bVideoExist = true;
            reveiveIpPort = m_vectMedioIpAndPorts.at(i);
            break;
        }
    }

    try
    {
        if (false == bVideoExist)
        {
            reason = "cann't find local video port info";
            throw false;
        }
        oldSBCVideoPort = reveiveIpPort.m_iPort;
        updateEvent.m_oldLocalVideoPort = reveiveIpPort.m_localPort;
        updateEvent.m_oldSBCVideoPort = reveiveIpPort.m_iPort;
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << endl);
        }
        else
        {
            TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << endl);
        }
        return ex;
    }
}

bool RtpSender::procAudioPortChangeEvent(SessionUpdateEvent updateEvent)
{
    bool result = false;
    int newLocalAudioPort = 0;
    int newSBCAudioPort = 0;
    std::vector<int> vectPorts;
    std::string reason;

    try
    {
        if (m_vectLocalRecMediaPorts.size() < 2)
        {
            reason = "m_vectLocalRecMediaPorts error";
            throw false;
        }

        newSBCAudioPort = updateEvent.m_newSBCAudioPort;
        newLocalAudioPort = m_vectLocalRecMediaPorts.at(0);

        removeAllConnections();
        result = buildAudioConnectionToSBC(newLocalAudioPort, m_sbcIp, newSBCAudioPort, reason);
        if (!result)
        {
            throw false;
        }

        std::vector<SDPMediaReceiveIpAndPort>::iterator iterMediaIpPort = m_vectMedioIpAndPorts.begin();
        for ( ; iterMediaIpPort != m_vectMedioIpAndPorts.end(); iterMediaIpPort++)
        {
            if ((*iterMediaIpPort).m_bIsAudio)
            {
                (*iterMediaIpPort).m_localPort = newLocalAudioPort;
                (*iterMediaIpPort).m_iPort = newSBCAudioPort;
                break;
            }
        }

        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << ",newSBCAudioPort:" << newSBCAudioPort << ", reason:" << reason << endl);
        }
        else
        {
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << ",newSBCAudioPort:" << newSBCAudioPort << endl);
        }
        return ex;
    }
}

bool RtpSender::procVideoPortChangeEvent(SessionUpdateEvent updateEvent)
{
    bool result = false;
    int newLocalVideoPort = 0;
    int newSBCVideoPort = 0;
    std::vector<int> vectPorts;
    std::string reason;

    try
    {
        result = m_pServer->allocFreeEvenPorts(m_strCallId, 1, vectPorts);
        if (false == result)
        {
            reason = "allocFreeEvenPorts() return FALSE.";
            throw false;
        }

        if (vectPorts.size() == 0)
        {
            reason = "allocFreeEvenPorts() return empty vector.";
            throw false;
        }
        newLocalVideoPort = vectPorts.at(0);
        newSBCVideoPort = updateEvent.m_newSBCVideoPort;

        {
            tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexConnections);
            insertExceptionConnection(m_video_RTP_ConnectionPtr);
            insertExceptionConnection(m_video_RTCP_ConnectionPtr);
            result = buildVideoConnectionToSBC(newLocalVideoPort, m_sbcIp, newSBCVideoPort, reason);
            if (!result)
            {
                throw false;
            }
        }

        m_vectLocalRecMediaPorts.push_back(newLocalVideoPort);
        m_vectLocalRecMediaPorts.push_back(newLocalVideoPort + 1);

        std::vector<SDPMediaReceiveIpAndPort>::iterator iterMediaIpPort = m_vectMedioIpAndPorts.begin();
        for ( ; iterMediaIpPort != m_vectMedioIpAndPorts.end(); iterMediaIpPort++)
        {
            if ((*iterMediaIpPort).m_bIsAudio == false)
            {
                (*iterMediaIpPort).m_localPort = newLocalVideoPort;
                (*iterMediaIpPort).m_iPort = newSBCVideoPort;
                break;
            }
        }

        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        else
        {
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        return ex;
    }
}

bool RtpSender::procVedioOpenedEvent(SessionUpdateEvent updateEvent)
{
    bool result = false;
    int newLocalVideoPort = 0;
    int newSBCVideoPort = 0;
    std::vector<int> vectPorts;
    std::string reason;

    try
    {
        result = m_pServer->allocFreeEvenPorts(m_strCallId, 1, vectPorts);
        if (false == result)
        {
            reason = "allocFreeEvenPorts() return FALSE.";
            throw false;
        }

        if (vectPorts.size() == 0)
        {
            reason = "allocFreeEvenPorts() return empty vector.";
            throw false;
        }
        newLocalVideoPort = vectPorts.at(0);
        newSBCVideoPort = updateEvent.m_newSBCVideoPort;

        {
            tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexConnections);
            result = buildVideoConnectionToSBC(newLocalVideoPort, m_sbcIp, newSBCVideoPort, reason);
            if (!result)
            {
                throw false;
            }
        }
        m_vectLocalRecMediaPorts.push_back(newLocalVideoPort);
        m_vectLocalRecMediaPorts.push_back(newLocalVideoPort + 1);

        SDPMediaReceiveIpAndPort medioReceivePort;
        medioReceivePort.m_strIp = m_sbcIp;
        medioReceivePort.m_bIsAudio = false;
        medioReceivePort.m_bForbitedStream = false;
        medioReceivePort.m_iPort = newSBCVideoPort;
        medioReceivePort.m_localPort = newLocalVideoPort;
        m_vectMedioIpAndPorts.push_back(medioReceivePort);

        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        else
        {
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << ",newSBCVideoPort:" << newSBCVideoPort << endl);
        }
        return ex;
    }
}

bool RtpSender::procVedioClosedEvent(SessionUpdateEvent updateEvent)
{
    try
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexConnections);
        insertExceptionConnection(m_video_RTP_ConnectionPtr);
        insertExceptionConnection(m_video_RTCP_ConnectionPtr);
        if (m_video_RTP_ConnectionPtr != 0)
        {
            m_video_RTP_ConnectionPtr->clearUp();
            m_video_RTP_ConnectionPtr = 0;
        }
        if (m_video_RTCP_ConnectionPtr != 0)
        {
            m_video_RTCP_ConnectionPtr->clearUp();
            m_video_RTCP_ConnectionPtr = 0;
        }
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << endl);
        }
        else
        {
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << endl);
        }
        return ex;
    }
}

bool RtpSender::procSBCIpChangeEvent(SessionUpdateEvent updateEvent)
{
    bool result = false;
    int newLocalAudioPort = 0;
    int newSBCAudioPort = 0;
    int newLocalVideoPort = 0;
    int newSBCVideoPort = 0;
    std::vector<int> vectPorts;
    std::string reason;
    bool bVideo = false;
    int newPortNum = 0;

    try
    {
        bVideo = updateEvent.m_newMediaType == kSessionMediaTypeVideo ? true : false;
        newPortNum = bVideo ? 2 : 1;

        if (m_vectLocalRecMediaPorts.size() < newPortNum * 2)
        {
            reason = "m_vectLocalRecMediaPorts error";
            throw false;
        }

        newLocalAudioPort = m_vectLocalRecMediaPorts.at(0);
        newSBCAudioPort = updateEvent.m_newSBCAudioPort;

        if (bVideo)
        {
            newLocalVideoPort = m_vectLocalRecMediaPorts.at(2);
            newSBCVideoPort = updateEvent.m_newSBCVideoPort;
        }

        removeAllConnections();
        result = buildAudioConnectionToSBC(newLocalAudioPort, m_sbcIp, newSBCAudioPort, reason);
        if (!result)
        {
            throw false;
        }
        m_vectMedioIpAndPorts.clear();

        SDPMediaReceiveIpAndPort medioReceivePortAudio;
        medioReceivePortAudio.m_strIp = m_sbcIp;
        medioReceivePortAudio.m_bIsAudio = true;
        medioReceivePortAudio.m_bForbitedStream = false;
        medioReceivePortAudio.m_iPort = newSBCAudioPort;
        medioReceivePortAudio.m_localPort = newLocalAudioPort;
        m_vectMedioIpAndPorts.push_back(medioReceivePortAudio);

        if (bVideo)
        {
            result = buildVideoConnectionToSBC(newLocalVideoPort, m_sbcIp, newSBCVideoPort, reason);
            if (!result)
            {
                throw false;
            }

            SDPMediaReceiveIpAndPort medioReceivePortVideo;
            medioReceivePortVideo.m_strIp = m_sbcIp;
            medioReceivePortVideo.m_bIsAudio = false;
            medioReceivePortVideo.m_bForbitedStream = false;
            medioReceivePortVideo.m_iPort = newSBCVideoPort;
            medioReceivePortVideo.m_localPort = newLocalVideoPort;
            m_vectMedioIpAndPorts.push_back(medioReceivePortVideo);
        }
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, return FALSE,call id:" << m_strCallId << ", reason:" << reason << endl);
        }
        else
        {
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called, call id:" << m_strCallId << endl);
        }
        return ex;
    }
}

bool RtpSender::buildAudioConnectionToSBC(int localPort, const std::string & remoteIp, int remotePort, std::string & reason)
{
    //音频 RTP
    m_audio_RTP_ConnectionPtr = newAGatewayConnectionEx(localPort, m_sbcIp, remotePort, SipGateway::TransTypeAudio);
    if (0 == m_audio_RTP_ConnectionPtr)
    {
        reason = "newAGatewayConnectionEx()return FALSE, audio RTP ! remote port: " + std::to_string(remotePort);
        return false;
    }
    bool result = insertAConnection(m_audio_RTP_ConnectionPtr);
    if (!result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTP FAILED!local port:"
            << localPort << ",remote port: " << remotePort << " ,call id:" << m_strCallId << endl);
    }
    m_vectRemoteRecMediaPorts.push_back(remotePort);
    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called,insert connection audio RTP connection! local port:"
        << localPort << ",remote port: " << remotePort << " ,call id:" << m_strCallId << endl);

    //音频 RTCP
    m_audio_RTCP_ConnectionPtr = newAGatewayConnectionEx(localPort + 1, m_sbcIp, remotePort + 1, SipGateway::TransTypeAudio);
    if (0 == m_audio_RTCP_ConnectionPtr)
    {
        reason = "newAGatewayConnectionEx()return FALSE, audio RTCP ! remote port: " + std::to_string(remotePort + 1);
        return false;
    }
    result = insertAConnection(m_audio_RTCP_ConnectionPtr);
    if (!result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTCP FAILED! local port:"
            << localPort << ",remote port: " << (remotePort +1) << " ,call id:" << m_strCallId << endl);
    }
    m_vectRemoteRecMediaPorts.push_back(remotePort + 1);
    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTCP connection! local port:"
        << localPort << ",remote port: " << (remotePort + 1) << " ,call id:" << m_strCallId << endl);

    return true;
}

bool RtpSender::buildVideoConnectionToSBC(int newLocalVideoPort, const std::string & remoteIp, int newSBCVideoPort, std::string & reason)
{
    //视频 RTP
    m_video_RTP_ConnectionPtr = newAGatewayConnectionEx(newLocalVideoPort, m_sbcIp, newSBCVideoPort, SipGateway::TransTypeVideo);
    if (0 == m_video_RTP_ConnectionPtr)
    {
        reason = "newAGatewayConnectionEx()return FALSE, video RTP ! remote port: " + std::to_string(newSBCVideoPort);
        return false;
    }
    bool result = insertAConnection(m_video_RTP_ConnectionPtr);
    if (!result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection video RTP FAILED! local port:"
            << newLocalVideoPort << ",remote port: " << newSBCVideoPort << " ,call id:" << m_strCallId << endl);
    }
    m_vectRemoteRecMediaPorts.push_back(newSBCVideoPort);
    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called,insert connection video RTP connection! local port:"
        << newLocalVideoPort << ",remote port: " << newSBCVideoPort << " ,call id:" << m_strCallId << endl);

    //视频 RTCP
    m_video_RTCP_ConnectionPtr = newAGatewayConnectionEx(newLocalVideoPort + 1, m_sbcIp, newSBCVideoPort + 1, SipGateway::TransTypeVideo);
    if (0 == m_video_RTCP_ConnectionPtr)
    {
        reason = "newAGatewayConnectionEx()return FALSE, video RTCP ! remote port: " + std::to_string(newSBCVideoPort + 1);
        return false;
    }
    result = insertAConnection(m_video_RTCP_ConnectionPtr);
    if (!result)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection video RTCP FAILED! local port:"
            << (newLocalVideoPort +1) << ",remote port: " << (newSBCVideoPort +1) << " ,call id:" << m_strCallId << endl);
    }
    m_vectRemoteRecMediaPorts.push_back(newSBCVideoPort + 1);
    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, insert connection video RTCP connection! local port:"
        << (newLocalVideoPort +1) << ",remote port: " << (newSBCVideoPort + 1) << " ,call id:" << m_strCallId << endl);

    return true;
}

bool RtpSender::getReceiveMediaIpAndPorts(const std::string & strRpcSDPConfig,std::string & ip, std::vector<int> & vectPorts)
{
    std::vector<SDPMediaInfo> vectSdpMediaInfo;
    SDPMediaInfo defaultSDPsess;

    if (!SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strRpcSDPConfig, vectSdpMediaInfo))
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, SipMsg::getMediaIpAndPortsOfSDPinSipMsg() return FALSE!" << endl);
        return false;
    }

    if (vectSdpMediaInfo.size() < 1)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, vectSdpMediaInfo size < 1!" << endl);
        return false;
    }

    //一般只有一个会话描述。且目前只考虑音频通信，即只有一个m 行
    defaultSDPsess = vectSdpMediaInfo.at(0);
    ip = defaultSDPsess._strIp;                         //listen ip
    vectPorts.clear();
    for (unsigned int  n = 0; n < defaultSDPsess._vecMediaPort.size(); n ++)
    {
        SDPMediaPort temM = defaultSDPsess._vecMediaPort.at(n);
        if (!temM._bMediaForbit)
        {
            vectPorts.push_back(temM._port);
        }
    }
    return true;
}

bool RtpSender::sendRtpDataToSBC(const unsigned char* pBuffer, int size, unsigned char strmType)
{
    if (pBuffer == 0)
    {
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called.pBuffer == 0, with call id :" << m_strCallId << ",size:" << int(size) << endl);
        return false;
    }
    char ch1 = pBuffer[0];
    char ch2 = pBuffer[1];

    bool bRtp = false;
    if (checkValidRtpOrRtcp(ch1) == false)
        return false;
    if (checkRtpOrRtcpEx(ch2, bRtp) == false)
        return false;

    tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexConnections);
    SipGateway::GatewayConnectionPtr  sendConnection;
    if (strmType == AudioType)
        sendConnection  = bRtp ? m_audio_RTP_ConnectionPtr : m_audio_RTCP_ConnectionPtr;                //1 表示RTP数据
    else
        sendConnection  = bRtp ? m_video_RTP_ConnectionPtr : m_video_RTCP_ConnectionPtr;                //1 表示RTP数据

    if (0 == sendConnection)
    {
        TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "() called.cann't find the Connection, with call id :" << m_strCallId << ",strmType:" << int(strmType) << endl);
        return false;
    }

    if (!m_startTimeToSbc)
        m_startTimeToSbc = TNOWMS;
    m_lastTimeToSbc = TNOWMS;

    if (sendConnection->send(pBuffer, size))
    {
        if (strmType == AudioType)
            m_audioToSBC++;
        m_sendToSBCPackets++;
    }
    else
        m_sendToSBCFailedPackets++;
    return true;
}

bool RtpSender::checkValidRtpOrRtcp(unsigned char ch)
{
    bool valid = false;
    int result = (ch >> 6) & 2;
    valid = result == 2 ? true : false;
    return valid;
}

bool RtpSender::checkRtpOrRtcpEx(unsigned char ch, bool & bRtp)
{
    int result = (ch & 0x7F);
    if((ch >= 192 && ch <= 195) || (ch >= 200 && ch <= 213))
    {
        bRtp = false;
        return true;
    }
    else if((result >= 0 && result <= 34) || (result >= 96 && result <= 127))
    {
        bRtp = true;
        return true;
    }
    return false;
}

void RtpSender::setCallId(const std::string & strCallId)
{
    m_strCallId = strCallId;
}

void RtpSender::addLocalRecvPorts(std::vector<int> evenPorts)
{
    for (unsigned int i = 0;i < evenPorts.size(); i++)
    {
        m_vectLocalRecMediaPorts.push_back(evenPorts.at(i));
        m_vectLocalRecMediaPorts.push_back(evenPorts.at(i) + 1);
    }
}

void RtpSender::setSipGatewayOid(const std::string & oid)
{
    m_sipGatewayOid = oid;
}

void RtpSender::conClose()
{
    m_pServer->freeListeningMediaPorts(m_vectLocalRecMediaPorts, m_strCallId);

    m_audio_RTP_ConnectionPtr = 0;
    m_audio_RTCP_ConnectionPtr = 0;
    m_video_RTP_ConnectionPtr = 0;
    m_video_RTCP_ConnectionPtr = 0;

    removeAllConnections();
    removeAllExcepConnections();
    m_pServer = 0;

    TLOGINFO("[RtpGateway] RtpSender::conClose() called, callId:" << m_strCallId << ",Packet num:\n send to SBC:" << m_sendToSBCPackets << ",failed num:" << m_sendToSBCFailedPackets 
        << ",audio:" << m_audioToSBC << ",\nto APP:" << m_sendToAppPackets << ",failed:" << m_sendToAppFailedPackets << ",audio:" << m_audioToApp
        << ",isCaller:" << m_isCaller << endl);
}

// bool RtpSender::setSDPAnswer(const std::string& strCsSDP,const std::string& strAppSDP, const std::map<std::string, std::string>& params,std::string& strModifiedAppSDP,
//                         std::map<std::string, std::string>& retValus, JssmmeChannelPtr & pChannel, std::string & errReason)
// {
//     std::vector<SDPMediaInfo> vectSdpMediaInfo;
//     std::string strSipMsgWithSDP;

//     do
//     {
//         tars::TC_LockT<tars::TC_ThreadRecMutex> lockThis(m_mutex);
//         if (!SipMsg::CreateSipMsgFromSDP(strCsSDP, strSipMsgWithSDP))
//         {
//             errReason = "SipMsg::CreateSipMsgFromSDP() return FALSE";
//             break;
//         }
//         TLOGDEBUG("[RtpGateway] " << __FUNCTION__ << "() called! call id:" << m_strCallId << " ,strSipWithSDP : \n" << strSipMsgWithSDP << endl);

//         m_csCallApp = false;
//         if (!SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strSipMsgWithSDP, vectSdpMediaInfo))
//         {
//             errReason = "SipMsg::getMediaIpAndPortsOfSDPinSipMsg() return FALSE";
//             break;
//         }
//         if (vectSdpMediaInfo.size() < 1)
//         {
//             errReason = "failed to achieve media port from sdp of sbc.";
//             break;
//         }
//         if (m_bInitConnected)
//         {
//             this->refreshMediaSteamConnectionToSBC(vectSdpMediaInfo, strCsSDP, strModifiedAppSDP);
//             if (pChannel)
//             {
//                 std::string strNewSdp;
//                 StSdpSessDesc stSdp, stNegoSdp;
//                 StSdpCodec sdpCode;
//                 SipMsg::readSdpParams(strCsSDP, stSdp);
//                 SipMsg::negoSdp(stSdp, stNegoSdp, m_pServer->m_sipSdpDesc, sdpCode);
//                 if (sdpCode.iPayload == ZMAXUCHAR)
//                 {
//                     TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called! SipMsg::negoSdp sip sdp return FALSE!,SDP:\n" << strCsSDP << endl);
//                     errReason == "SipMsg::negoSdp sip failed";
//                     break;
//                 }

//                 StSdpCodec dtmfCode;
//                 int dtmfPayload = SipMsg::getDtmfPayload(stSdp, dtmfCode);
//                 int redPayload = SipMsg::getRedPayload(stSdp);

//                 bool ret = SipMsg::createSdp(stNegoSdp, m_strListenIp, m_vectLocalRecMediaPorts, strModifiedAppSDP);
//                 if (!ret)
//                 {
//                     errReason = "createSdp failed";
//                 }
//                 m_pServer->m_jssmmeManager->createChannels(pChannel, errReason, sdpCode, dtmfPayload, redPayload);
//             }
//             else
//             {
//                 TLOGERROR("[RtpGateway] RtpSender::::setSDPAnswer() called, no channel" << endl);
//                 errReason = "no channel";
//             }
//         }
//         else
//         {
//             this->generateMediaStreamConnectionToSBCEx(vectSdpMediaInfo);

//             if (pChannel)
//             {
//                 StSdpSessDesc stSdp;
//                 StSdpCodec sdpCode;
//                 SipMsg::readSdpParams(strCsSDP, stSdp);
//                 StSdpCodec dtmfCode;
//                 int payload = SipMsg::getDtmfPayload(stSdp, dtmfCode);
//                 if (payload)
//                     pChannel->m_sipDtmfPayload = dtmfCode.iPayload;

//                 std::vector<StSdpCodec> AcCode;
//                 SipMsg::getAcPayload(stSdp, AcCode);
//                 pChannel->m_vectSipSendCodec = AcCode;

//                 int redPayload = SipMsg::getRedPayload(stSdp);
//                 pChannel->m_sipRedPayload = redPayload;

//                 pChannel->m_sipSdpAnswer = strCsSDP;
//                 strModifiedAppSDP = pChannel->m_appSdpAnswer;
//                 pChannel->m_sipMediaNegoStatus = kMediaNegoFinished;
//                 m_pServer->m_jssmmeManager->createChannels(pChannel, errReason);
//             }
//         }

//     } while (0);

//     return errReason.empty() ? true : false;
// }

bool RtpSender::refreshMediaSteamConnection(const std::string & sdp)
{
    std::vector<SDPMediaInfo> vectSdpMediaInfo;
    std::string strSipMsgWithSDP, errReason;

    do
    {
        if (!SipMsg::CreateSipMsgFromSDP(sdp, strSipMsgWithSDP))
        {
            errReason = "SipMsg::CreateSipMsgFromSDP() return FALSE";
            break;
        }
        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called! call id:" << m_strCallId << " ,strSipWithSDP : \n" << strSipMsgWithSDP << endl);

        if (!SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strSipMsgWithSDP, vectSdpMediaInfo))
        {
            errReason = "SipMsg::getMediaIpAndPortsOfSDPinSipMsg() return FALSE";
            break;
        }

        if (vectSdpMediaInfo.size() < 1 || m_vectLocalRecMediaPorts.size() < 1)
        {
            errReason = "failed to achieve media port from sdp of sbc.";
            break;
        }

        SessionUpdateEvent updateEvent;
        bool result = this->getSessionMediaChangeEvent(vectSdpMediaInfo, updateEvent);
        if (!result)
        {
            TLOGWARN("[RtpGateway] " << __FUNCTION__ << "()called, getSessionMediaChangeEvent()return FALSE,call id:" << m_strCallId << ",m_updateType:" << updateEvent.m_updateType << endl);
            break;
        }
        TLOGINFO("[RtpGateway] " << __FUNCTION__ << "()called, getSessionMediaChangeEvent() return TRUE,call id:" << m_strCallId << ",m_updateType:" << updateEvent.m_updateType << endl);

        if (updateEvent.m_updateType == kSessionMediaIpChange || updateEvent.m_updateType == kSessionAudioPortChange)
        {
            tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexConnections);
            insertExceptionConnection(m_audio_RTP_ConnectionPtr);
            insertExceptionConnection(m_audio_RTCP_ConnectionPtr);

            int localPort = m_vectLocalRecMediaPorts.at(0);

            //音频 RTP
            m_audio_RTP_ConnectionPtr = newAGatewayConnectionEx(localPort, m_sbcIp, updateEvent.m_newSBCAudioPort, SipGateway::TransTypeAudio);
            if (0 == m_audio_RTP_ConnectionPtr)
            {
                errReason = "newAGatewayConnectionEx()return FALSE, audio RTP ! remote port: " + std::to_string(updateEvent.m_newSBCAudioPort);
                break;
            }
            result = insertAConnection(m_audio_RTP_ConnectionPtr);
            if (!result)
            {
                TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTP FAILED!local port:"
                    << localPort << ",remote port: " << updateEvent.m_newSBCAudioPort << " ,call id:" << m_strCallId << endl);
            }
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called,insert connection audio RTP connection! local port:"
                << localPort << ",remote port: " << updateEvent.m_newSBCAudioPort << " ,call id:" << m_strCallId << endl);

            //音频 RTCP
            m_audio_RTCP_ConnectionPtr = newAGatewayConnectionEx(localPort + 1, m_sbcIp, updateEvent.m_newSBCAudioPort + 1, SipGateway::TransTypeAudio);
            if (!m_audio_RTCP_ConnectionPtr)
            {
                errReason = "newAGatewayConnectionEx()return FALSE, audio RTCP ! remote port: " + std::to_string(updateEvent.m_newSBCAudioPort + 1);
                break;
            }
            result = insertAConnection(m_audio_RTCP_ConnectionPtr);
            if (!result)
            {
                TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTCP FAILED! local port:"
                    << localPort << ",remote port: " << (updateEvent.m_newSBCAudioPort +1) << " ,call id:" << m_strCallId << endl);
            }
            TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, insert connection audio RTCP connection! local port:"
                << localPort << ",remote port: " << (updateEvent.m_newSBCAudioPort + 1) << " ,call id:" << m_strCallId << endl);
        }
    } while (0);

    return errReason.empty() ? true : false;
}

bool RtpSender::neoAppSdp(const std::string& sdp,std::string& newSdp, JssmmeChannelPtr & pChannel)
{
    StSdpSessDesc stSdpParams, origSdpParams, stNegoSdp;
    StSdpCodec sdpCode;
    SipMsg::readSdpParams(sdp, stSdpParams);
    SipMsg::negoSdp(stSdpParams, stNegoSdp, pChannel->m_vectAppSendCodec);
    return SipMsg::createSdp(stNegoSdp, m_strListenIp, m_vectLocalRecMediaPorts, newSdp);
}

int64_t RtpSender::getCreateTicks()
{
    return m_createTicks;
}

void RtpSender::setCaller(int isCaller)
{
    m_isCaller = isCaller;
}

//*************************************** Rtp2RtpChannel *************
Rtp2RtpChannel::Rtp2RtpChannel(const SipRtpImpPtr& server, const std::string & callId, int isCaller, const std::string & sdp, bool qosPres, int interval, bool emptyPkgCheck):
    RtpSender(server, "", callId)
{
    m_server = server;
    m_callId= callId;
    m_isCaller = isCaller;
    m_qos = qosPres;
    m_mediaEmptyPkgCheckPres = emptyPkgCheck;
    m_mediaEmptyPkgChkInterval = interval;
    m_lastActiveTicks = -1;
    m_sdp = sdp;
    m_bPrivateIp = false;
    m_bCalleeUsePrivateIp = false;
    m_channelId = -1;
    m_dtmfPayload = -1;
    m_redPayload = -1;
    m_mediaLstCheckTime = 0;
    m_mediaEmptyPkgNum = 0;
    m_rtpPackageNum = 0;
    m_rtcpPackageNum = 0;
    m_answerTime = 0;
    m_recordStarted = false;
    m_channelCreated = false;

    m_lTotalChannelNum++;
    m_lTotalNewChannelNum++;
}

Rtp2RtpChannel::~Rtp2RtpChannel()
{
    TLOGINFO("[Rtp2RtpChannel] " << __FUNCTION__ << "() called,m_isCaller:" << m_isCaller << ",call id:" << m_callId << ",startTimeToApp:" << m_startTimeToApp << ",last:" << m_lastTimeToApp << ",startTimeToSbc:" << m_startTimeToSbc << ",last:" << m_lastTimeToSbc << endl);

    m_lTotalChannelNum--;
    m_iTotalDeleChannelNum++;
}

// 实现基类的虚函数，提供 shared_ptr
SipRtpGateway::RtpGatewayMsgSenderInterfacePtr Rtp2RtpChannel::getSharedPtr()
{
    // 使用 shared_from_this() 获取 Rtp2RtpChannel 的 shared_ptr，然后转换为基类指针
    return std::static_pointer_cast<SipRtpGateway::RtpGatewayMsgSenderInterface>(shared_from_this());
}

bool Rtp2RtpChannel::receivedSBCRtpMsg(const std::string & strMsg, int size, bool bAudio, void * pConnection)
{
    m_lastActiveTicks = TNOWMS;

    if (bAudio && m_qos && m_isCaller != 2)
    {
        char ch2 = ((unsigned char *)strMsg.c_str())[1];
        bool bRtp = false;
        if (!checkRtpOrRtcpEx(ch2, bRtp))
            return false;

        const unsigned char* msg = (const unsigned char*)strMsg.c_str();
        int len = strMsg.size();

        SipRtpGateway::RtpGatewayInterfaceIPtr ptr = m_server->getJssmmeInterface();
        if (bRtp)
        {
            {
                int64_t curr = TNOWMS;

                tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexMediaCheckTime);
                if (getEmptyPkgCheckPres() && (curr - m_mediaLstCheckTime) >= m_mediaEmptyPkgChkInterval)
                {
                    m_mediaLstCheckTime = curr;
                    unsigned int level = 0;
                    int ret = ptr->mediaEmptyPktCheck(m_channelId, level);
                    if (ret == 0 && level <= 8)
                    {
                        m_mediaEmptyPkgNum++;
                    }
                }
            }

            if (m_answerTime)
                m_rtpPackageNum++;

            if (ptr)
                ptr->receivedRTPPacket(m_channelId, msg, size);
        }
        else
        {
            if (m_answerTime)
                m_rtcpPackageNum++;

            if (ptr)
                ptr->receivedRTCPPacket(m_channelId, msg, size);
        }
    }
    else
    {
        Rtp2RtpChannelPtr pPeer = 0;
        {
            tars::TC_LockT<tars::TC_ThreadRecMutex> lk(m_mutexPeer);
            pPeer = m_peer;
        }

        if (!pPeer)
        {
            TLOGDEBUG("[Rtp2RtpChannel] " << __FUNCTION__ << "() called,pPeer is null, size:" << size << ",call id:" << m_callId << endl);
            return false;
        }

        // pConnection 是 void*，需要转换为 GatewayConnection* 然后使用 shared_from_this()
        SipGateway::GatewayConnection* pConn = static_cast<SipGateway::GatewayConnection*>(pConnection);
        SipGateway::GatewayConnectionPtr ptr;
        if (pConn)
        {
            try {
                ptr = pConn->shared_from_this();
            } catch (const std::bad_weak_ptr&) {
                // 如果对象不是由 shared_ptr 管理的，ptr 将保持为空
                ptr.reset();
            }
        }
        if (ptr)
        {
            Common::NetSenderPtr temSender = ptr->getSender();
            if (temSender != 0)
            {
                this->insertAConnection(ptr);
            }
            else
            {
                insertExceptionConnection(ptr);
                TLOGWARN("[Rtp2RtpChannel] " << __FUNCTION__ << "() called! ptr->_sender == 0 ! call id:" << m_strCallId << endl);
            }
        }
        return pPeer->sendDataToPeer(strMsg, size, bAudio);
    }
}

Common::NetReceiverPtr Rtp2RtpChannel::recvConnection(const Common::NetSenderPtr& sender)
{
    // 明确指定使用 Rtp2RtpChannel 的 shared_from_this()，避免与 RtpSender 的 shared_from_this() 产生歧义
    return std::make_shared<SipGateway::GatewayConnection>(Rtp2RtpChannel::shared_from_this(), sender, SipGateway::RtpGatewayModul);
}

void Rtp2RtpChannel::onConnReady()
{
}

void Rtp2RtpChannel::onConnClose()
{
}

int Rtp2RtpChannel::SendPacket(int channel, const void *data, int len, unsigned int level, unsigned int timestamp)
{
    std::string msg((const char *)data, len);
    return sendDataToPeer(msg, len, true);
}

int Rtp2RtpChannel::SendRTCPPacket(int channel, const void *data, int len)
{
    std::string msg((const char *)data, len);
    return sendDataToPeer(msg, len, true);
}

bool Rtp2RtpChannel::connectToRemote(const std::string & remoteIp, int remotePort, const std::string & localIp, int localPort)
{
    //to do...
    return true;
}

void Rtp2RtpChannel::setPeerChannel(const Rtp2RtpChannelPtr & peer)
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexPeer);
    m_peer = peer;
    TLOGINFO("[Rtp2RtpChannel] " << __FUNCTION__ << "() called,call id:" << m_strCallId << ",m_isCaller:" << m_isCaller << "--> peer:" << peer->m_isCaller << endl);
}

Rtp2RtpChannelPtr Rtp2RtpChannel::getPeerChannel()
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexPeer);
    return m_peer;
}

void Rtp2RtpChannel::clear()
{
    if (m_vectCalleeRecvPorts.size() > 0)
        m_pServer->freeListeningMediaPorts(m_vectCalleeRecvPorts, m_strCallId);

    if (m_vectMrfpRecvPorts.size() > 0)
        m_pServer->freeListeningMediaPorts(m_vectMrfpRecvPorts, m_strCallId);

    conClose();

    tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexPeer);
    m_peer = 0;

    if (m_audio_RTP_Listen_ConnectionPtr)
    {
        m_audio_RTP_Listen_ConnectionPtr->close();
        m_audio_RTP_Listen_ConnectionPtr = 0;
    }

    if (m_audio_RTCP_Listen_ConnectionPtr)
    {
        m_audio_RTCP_Listen_ConnectionPtr->close();
        m_audio_RTCP_Listen_ConnectionPtr = 0;
    }

    if (m_video_RTP_Listen_ConnectionPtr)
    {
        m_video_RTP_Listen_ConnectionPtr->close();
        m_video_RTP_Listen_ConnectionPtr = 0;
    }

    if (m_video_RTCP_Listen_ConnectionPtr)
    {
        m_video_RTCP_Listen_ConnectionPtr->close();
        m_video_RTCP_Listen_ConnectionPtr = 0;
    }

    SipRtpGateway::RtpGatewayInterfacePtr ptr = m_server->getJssmmeInterface();
    if (ptr && m_qos)
    {
        std::string statisInfo;
        unsigned short fraction_lost = 0;
        unsigned int cumulative_lost = 0, average_lost = 0, extended_max = 0, jitter = 0;
        int rtt_ms = 0;
        ptr->getSentRTCPStatistics(m_channelId, fraction_lost, cumulative_lost, average_lost, extended_max, jitter, rtt_ms);
        statisInfo += "send rtcp statistics:";
        statisInfo += "\n\t fraction_lost:" + std::to_string((int)fraction_lost);
        statisInfo += "\n\t cumulative_lost:" + std::to_string((int)cumulative_lost);
        statisInfo += "\n\t average_lost:" + std::to_string((int)average_lost);
        statisInfo += "\n\t extended_max:"+ std::to_string((int)extended_max);
        statisInfo += "\n\t rtt_ms:" + std::to_string(rtt_ms);

        cumulative_lost = 0, average_lost = 0, extended_max = 0, jitter = 0;
        ptr->getReceivedRTCPStatistics(m_channelId, fraction_lost, cumulative_lost, average_lost, extended_max, jitter, rtt_ms);
        statisInfo += "\nrecv rtcp statistics:";
        statisInfo += "\n\t fraction_lost:" + std::to_string((int)fraction_lost);
        statisInfo += "\n\t cumulative_lost:" + std::to_string((int)cumulative_lost);
        statisInfo += "\n\t average_lost:" + std::to_string((int)average_lost);
        statisInfo += "\n\t extended_max:"+ std::to_string((int)extended_max);
        statisInfo += "\n\t rtt_ms:" + std::to_string(rtt_ms);

        cumulative_lost = 0, average_lost = 0, extended_max = 0;
        ptr->getRTPStatistics(m_channelId, cumulative_lost, average_lost, extended_max);
        statisInfo += "\nrtp statistics:";
        statisInfo += "\n\t averageJitterMs:" + std::to_string((int)cumulative_lost);
        statisInfo += "\n\t maxJitterMs:" + std::to_string((int)average_lost);
        statisInfo += "\n\t discardedPackets:"+ std::to_string((int)extended_max);

        cumulative_lost = 0, average_lost = 0, extended_max = 0, jitter = 0;
        ptr->getTotalDataCounters(m_channelId, cumulative_lost, average_lost, extended_max, jitter);
        statisInfo += "\ntotal data cnts statistics:";
        statisInfo += "\n\t bytesSent:" + std::to_string((int)cumulative_lost);
        statisInfo += "\n\t packetsSent:" + std::to_string((int)average_lost);
        statisInfo += "\n\t bytesReceived:"+ std::to_string((int)extended_max);
        statisInfo += "\n\t packetsReceived:" + std::to_string((int)jitter);

        statisInfo += "\nrecv empty pkg:" + std::to_string(m_mediaEmptyPkgNum);
        statisInfo += "\nrecv rtp pkg after answering:" + std::to_string(m_rtpPackageNum);
        statisInfo += "\nrecv rtcp pkg after answering:" + std::to_string(m_rtcpPackageNum);

        TLOGINFO("[Rtp2RtpChannel] " << __FUNCTION__ << "() called,call id:" << m_strCallId << ", channelId:" << m_channelId << ", statisInfo:\n" << statisInfo << endl);
    }

    m_server = 0;
}

bool Rtp2RtpChannel::sendDataToPeer(const std::string & strMsg, int size, bool bAudio)
{
    m_lastActiveTicks = TNOWMS;
    unsigned char strmType = bAudio ? 0 : 1;
    return sendRtpDataToSBC((unsigned char*)strMsg.c_str(), size, strmType);
}

int64_t Rtp2RtpChannel::getLastActiveTicks()
{
    return m_lastActiveTicks;
}

void Rtp2RtpChannel::setSdp(const std::string & sdp,  bool origin)
{
    if (origin)
        m_sdp = sdp;
    else
        m_modifyedSdp = sdp;
}
std::string Rtp2RtpChannel::getSdp(bool origin)
{
    return origin ? m_sdp : m_modifyedSdp;
}

void Rtp2RtpChannel::setCalleeSdp(const std::string & sdp,  bool origin)
{
    if (origin)
        m_calleeSdp = sdp;
    else
        m_modifyedCalleeSdp = sdp;
}
std::string Rtp2RtpChannel::getCalleeSdp(bool origin)
{
    return origin ? m_calleeSdp : m_modifyedCalleeSdp;
}

bool Rtp2RtpChannel::generateRtpListen(const int port)
{
    m_audio_RTP_Listen_ConnectionPtr;// = m_application->getDriver()->listen("udp", "", port, this, true);
    if (m_audio_RTP_Listen_ConnectionPtr == 0)
    {
        std::string reason = ",errno:" + std::to_string(errno);
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called,CAN NOT connected psender2,localRtpPort:" << port << " ,call id:" << m_strCallId << ",reason:" << reason << endl);
        return false;
    }

    int rtcpPort = port + 1;
    m_audio_RTCP_Listen_ConnectionPtr;// = m_application->getDriver()->listen("udp", "", rtcpPort, this, true);
    if (m_audio_RTCP_Listen_ConnectionPtr == 0)
    {
        std::string reason = ",errno:" + std::to_string(errno);
        TLOGWARN("[RtpGateway] " << __FUNCTION__ << "() called,CAN NOT connected psender2,localRtcpPort:" << rtcpPort << " ,call id:" << m_strCallId << ",reason:" << reason << endl);
        return false;
    }
    TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, Audio Call, listen local port:" << port << " ,call id:" << m_strCallId << endl);
    return true;
}

void Rtp2RtpChannel::setCallerRecvPorts(std::vector<int> &ports)
{
    for (unsigned int i = 0;i < ports.size(); i++)
    {
        m_vectCallerRecvPorts.push_back(ports.at(i));
        m_vectCallerRecvPorts.push_back(ports.at(i) + 1);
    }
    m_vectCallerRecvPorts2 = ports;
}

std::vector<int> Rtp2RtpChannel::getCallerRecvPorts()
{
    return m_vectCallerRecvPorts2;
}

void Rtp2RtpChannel::setCalleeRecvPorts(std::vector<int> &ports)
{
    for (unsigned int i = 0;i < ports.size(); i++)
    {
        m_vectCalleeRecvPorts.push_back(ports.at(i));
        m_vectCalleeRecvPorts.push_back(ports.at(i) + 1);
    }

    m_vectCalleeRecvPorts2 = ports;
}

std::vector<int> Rtp2RtpChannel::getCalleeRecvPorts()
{
    return m_vectCalleeRecvPorts2;
}

void Rtp2RtpChannel::setMrfpRecvPorts(std::vector<int> &ports)
{
for (unsigned int i = 0;i < ports.size(); i++)
{
    m_vectMrfpRecvPorts.push_back(ports.at(i));
    m_vectMrfpRecvPorts.push_back(ports.at(i) + 1);
}
}

std::vector<int> Rtp2RtpChannel::getMrfpRecvPorts()
{
    return m_vectMrfpRecvPorts;
}

void Rtp2RtpChannel::setPrivateIpProps(bool isPrivate)
{
    m_bPrivateIp = isPrivate;
}

bool Rtp2RtpChannel::getPrivateIpProps()
{
    return m_bPrivateIp;
}

void Rtp2RtpChannel::setCalleePrivateIpProps(bool isPrivate)
{
    m_bCalleeUsePrivateIp = isPrivate;
}

bool Rtp2RtpChannel::getCalleePrivateIpProps()
{
    return m_bCalleeUsePrivateIp;
}

void Rtp2RtpChannel::setChannelId(int id)
{
    m_channelId = id;
}
int Rtp2RtpChannel::getChannelId()
{
    return m_channelId;
}

void Rtp2RtpChannel::setCallNumber(bool isCaller, const std::string& number)
{
    if (isCaller)
        m_callerNumber = number;
    else
        m_calleeNumber = number;
}

std::string Rtp2RtpChannel::getCallNumber(bool isCaller)
{
    return isCaller ? m_callerNumber : m_calleeNumber;
}

void Rtp2RtpChannel::setRecordTime(const std::string& time)
{
    m_recordTime = time;
}

std::string Rtp2RtpChannel::getRecordTime()
{
    return m_recordTime;
}

void Rtp2RtpChannel::setAnswerTime()
{
    if (!m_answerTime)
        m_answerTime = TNOWMS;
}
int64_t Rtp2RtpChannel::getAnswerTime()
{
    return m_answerTime;
}

bool Rtp2RtpChannel::getEmptyPkgCheckPres()
{
    return (m_answerTime && m_channelCreated && m_mediaEmptyPkgCheckPres) ? true : false;
}

std::string Rtp2RtpChannel::getCallId()
{
    return m_callId;
}

void Rtp2RtpChannel::setRecordPres(bool isRecord)
{
    m_recordStarted = isRecord;
}
bool Rtp2RtpChannel::getRecordPres()
{
    return m_recordStarted;
}

void Rtp2RtpChannel::setChannelPres(bool isCreated)
{
    m_channelCreated = isCreated;
}
bool Rtp2RtpChannel::getChannelPres()
{
    return m_channelCreated;
}

void Rtp2RtpChannel::setRedPayload(int pl)
{
    m_redPayload = pl;
}
int Rtp2RtpChannel::getRedPayload()
{
    return m_redPayload;
}

void Rtp2RtpChannel::setDtmfPayload(int pl)
{
    m_dtmfPayload = pl;
}
int Rtp2RtpChannel::getDtmfPayload()
{
    return m_dtmfPayload;
}

void Rtp2RtpChannel::setAcPayload(std::vector<StSdpCodec>& pl)
{
    m_vectSendCodec = pl;
}
std::vector<StSdpCodec> Rtp2RtpChannel::getAcPayload()
{
    return m_vectSendCodec;
}

std::atomic<int> Rtp2RtpChannel::m_lTotalChannelNum{0};
std::atomic<int> Rtp2RtpChannel::m_lTotalNewChannelNum{0};
std::atomic<int> Rtp2RtpChannel::m_iTotalDeleChannelNum{0};

//*************************  Rtp2RtpRelayer ********************
Rtp2RtpRelayer::Rtp2RtpRelayer(const SipRtpImpPtr& server, const std::string & callId, bool qos, int interval, bool emptyPkgCheck, bool callRecord)
{
    m_server = server;
    m_callId = callId;
    m_qos = qos;
    m_callRecord = callRecord;
    m_mediaEmptyPkgChkInterval = interval;
    m_mediaEmptyPkgCheckPres = emptyPkgCheck;
    m_createTicks = TNOWMS;

    m_lTotalRelayNum++;
    m_lTotalNewRelayNum++;

    TLOGINFO("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, callId:" << m_callId << ", m_mediaEmptyPkgChkInterval:" << m_mediaEmptyPkgChkInterval << endl);
}


Rtp2RtpRelayer::~Rtp2RtpRelayer()
{
    m_lTotalRelayNum--;
    m_iTotalDeleRelayNum++;

    TLOGDEBUG("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, callId:" << m_callId << endl);
}

Rtp2RtpChannelPtr Rtp2RtpRelayer::createChannel(int caller, const std::string & sdp)
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> lk(m_mutexChannels);
    if (1 == caller)
    {
        if (!m_caller)
        {
            m_caller = std::make_shared<Rtp2RtpChannel>(m_server, m_callId, caller, sdp, m_qos, m_mediaEmptyPkgChkInterval, m_mediaEmptyPkgCheckPres);
            TLOGINFO("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, create caller channel, callId:" << m_callId << ", caller:" << caller << ",sdp:\n" << sdp << endl);
        }
        else
            TLOGWARN("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, caller channel is already exist, callId:" << ", caller:" << caller << m_callId << endl);
        return m_caller;
    }
    else if (0 == caller)
    {
        if (!m_callee)
        {
            m_callee = std::make_shared<Rtp2RtpChannel>(m_server, m_callId, caller, sdp, m_qos, m_mediaEmptyPkgChkInterval, m_mediaEmptyPkgCheckPres);
            TLOGINFO("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, create callee channel, callId:" << m_callId << ", caller:" << caller << ",sdp:\n" << sdp << endl);
        }
        else
            TLOGWARN("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, callee channel is already exist, callId:" << m_callId << ", caller:" << caller << endl);
        return m_callee;
    }
    else
    {
        if (!m_mrfp)
        {
            m_mrfp = std::make_shared<Rtp2RtpChannel>(m_server, m_callId, caller, sdp, false, 1000, false);
            TLOGINFO("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, create mrfp channel, callId:" << m_callId << ", caller:" << caller << ",sdp:\n" << sdp << endl);
    }
        else
            TLOGWARN("[Rtp2RtpRelayer] " << __FUNCTION__ << "() called, mrfp channel is already exist, callId:" << m_callId << ", caller:" << caller << endl);
        return m_mrfp;
    }
}

Rtp2RtpChannelPtr Rtp2RtpRelayer::getChannel(int caller)
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> lk(m_mutexChannels);
    if (caller == 0)
    {
        return m_callee;
    }
    else if (caller == 1)
    {
        return m_caller;
    }
    else
    {
        return m_mrfp;
    }
}

int64_t Rtp2RtpRelayer::getCreateTicks()
{
    return m_createTicks;
}

void Rtp2RtpRelayer::clear()
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> lk(m_mutexChannels);
    if (m_caller)
        m_caller->clear();

    if (m_callee)
        m_callee->clear();

    if (m_mrfp)
        m_mrfp->clear();

    if(m_qos)
    {
        SipRtpGateway::RtpGatewayInterfacePtr ptr = m_server->getJssmmeInterface();
        if(ptr)
        {
            if (m_caller)
            {
                int channelId = m_caller->getChannelId();
                if(m_callRecord)
                    ptr->stopCallRecord(channelId);

                if (m_caller->getChannelPres() && m_callee)
                    ptr->disconnectChannel(channelId, m_callee->getChannelId());
                else
                    ptr->delChannel(channelId);
            }

            if (m_callee)
            {
                ptr->delChannel(m_callee->getChannelId());
            }
        }
    }

    m_caller = 0;
    m_callee = 0;
    m_mrfp = 0;
    m_server = 0;
}

int64_t Rtp2RtpRelayer::getLastActiveTicks()
{
    tars::TC_LockT<tars::TC_ThreadRecMutex> lk(m_mutexChannels);
    int64_t tick1 = -1, tick2 = -1;

    if (m_caller)
        tick1 = m_caller->getLastActiveTicks();
    if (m_callee)
        tick2 = m_callee->getLastActiveTicks();

    return tick1 < tick2 ? tick1 : tick2;
}

std::string Rtp2RtpRelayer::getCallId()
{
    return m_callId;
}

bool Rtp2RtpRelayer::getQosPres()
{
    return m_qos;
}

bool Rtp2RtpRelayer::getCallRecordPres()
{
    return m_callRecord;
}

std::atomic<int> Rtp2RtpRelayer::m_lTotalRelayNum{0};
std::atomic<int> Rtp2RtpRelayer::m_lTotalNewRelayNum{0};
std::atomic<int> Rtp2RtpRelayer::m_iTotalDeleRelayNum{0};


// JssmmeManager::JssmmeManager()
// {
//     m_channelCount = 0;
//     m_channelIdCount = 0;
//     m_expiredTime = 12 * 60 * 60 * 1000;
// }

// JssmmeManager::~JssmmeManager()
// {

// }

// bool JssmmeManager::init()
// {
//     TLOGINFO("[RtpGateway] JssmmeManager::init() called," << endl);

//     _VoE = jssmme::VoiceEngine::Create();
//     _Base= jssmme::VoEBase::GetInterface(_VoE);
//     _Codec = jssmme::VoECodec::GetInterface(_VoE);
//     _Volume = jssmme::VoEVolumeControl::GetInterface(_VoE);
//     _Dtmf = jssmme::VoEDtmf::GetInterface(_VoE);
//     _RtpRtcp = jssmme::VoERTP_RTCP::GetInterface(_VoE);
//     _Apm = jssmme::VoEAudioProcessing::GetInterface(_VoE);
//     _Netw = jssmme::VoENetwork::GetInterface(_VoE);
//     _File = jssmme::VoEFile::GetInterface(_VoE);
//     _Vsync = jssmme::VoEVideoSync::GetInterface(_VoE);
//     _Encr = jssmme::VoEEncryption::GetInterface(_VoE);
//     _Hardware = jssmme::VoEHardware::GetInterface(_VoE);
//     _Xmedia = jssmme::VoEExternalMedia::GetInterface(_VoE);
//     _Neteqst = jssmme::VoENetEqStats::GetInterface(_VoE);

//     _Apm->SetEcStatus(false);
//     _Apm->SetAgcStatus(false);

//     /* init voe base */
//     _Base->Init();
//     _Base->SetTraceFilter(0x1f);
//     _Base->SetTraceFile("./mme.log", 0, 10);
// }

// bool JssmmeManager::term()
// {
//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called," << endl);

//     if (!_VoE) 
//         return false;

//     /* terminate voe base */
//     _Base->Terminate();
//     _Base->Release();
//     _Codec->Release();
//     _Volume->Release();
//     _Dtmf->Release();
//     _RtpRtcp->Release();
//     _Apm->Release();
//     _Netw->Release();
//     _File->Release();
//     _Vsync->Release();
//     _Encr->Release();
//     _Hardware->Release();
//     _Xmedia->Release();
//     _Neteqst->Release();

//     /* delete voice engine */
//     JssMmeDeleteEngine(VoiceEngine,_VoE);
//     return true;
// }

// bool JssmmeManager::disconnect(const std::string & callId)
// {
//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, callId:" << callId << endl);

//     JssmmeChannelPtr pChannel = getChannel(callId);
//     if (!pChannel)
//     {
//         return false;
//     }

//     if (pChannel->m_bRecord)
//         _File->StopRecordingCall(pChannel->m_appChannelId);

//     _Base->DisconnectChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId);
//     int ret = _Base->DeleteChannel(pChannel->m_sipChannelId);
//     if (!ret)
//         m_channelIdCount--;

//     ret = _Base->DeleteChannel(pChannel->m_appChannelId);
//     if (!ret)
//         m_channelIdCount--;

//     deleteChannel(callId);
//     return true;
// }

// int JssmmeManager::mediaEmptyPktCheck(int channelId, unsigned int &level)
// {
//     return _Volume->GetSpeechOutputLevelFullRange(channelId, level);
// }

// int JssmmeManager::setSendCodec(int channelId, vector<StSdpCodec> & sdpCode)
// {
//     for (int j = 0; j <sdpCode.size(); j++)
//     {
//         StSdpCodec code = sdpCode.at(j);
//         if (code.pcName.empty())
//             continue;

//         for (int i = _Codec->NumOfCodecs() - 1; i >= 0; --i)
//         {
//             jssmme::CodecInst codec;
//             _Codec->GetCodec(i, codec);

//             std::string localCode = codec.plname;
//             int pos = localCode.find(code.pcName);
//             if (pos >= 0)
//             {
//                 TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, channelId:" << channelId << ", jssmme local codec.plname:" << localCode << endl);
//                 _Codec->SetRecPayloadType(channelId, codec);
//                 _Codec->SetSendCodec(channelId, codec);
//                 break;
//             }
//         }
//     }
// }

// int JssmmeManager::setSendRedCodec(int channelId, int iPayload)
// {
//     for (int i = _Codec->NumOfCodecs() - 1; i >= 0; --i)
//     {
//         jssmme::CodecInst codec;
//         _Codec->GetCodec(i, codec);

//         //red/fec
//         if (strcmp("red",codec.plname) == 0)
//         {
//             TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, red channelId:" << channelId << ", code.pcName:" << codec.plname << endl);

//             codec.pltype = iPayload;
//             _Codec->SetRecPayloadType(channelId, codec);
//         }
//     }
// }

// int JssmmeManager::getCodecParams(std::set<std::string> codec, vector<StSdpCodec> & sdpCode)
// {
//     std::set<std::string>::iterator itor = codec.begin();
//     for (; itor != codec.end(); itor++)
//     {
//         for (int i = _Codec->NumOfCodecs() - 1; i >= 0; --i)
//         {
//             jssmme::CodecInst codec;
//             _Codec->GetCodec(i, codec);

//             TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, codec.pltype:" << codec.pltype << ", codec.rate:" << codec.rate << ", codec.plname:" << codec.plname
//                 << ", codec.pacsize:" << codec.pacsize << ", codec.plfreq:" << codec.plfreq << endl);

//             std::string plName = codec.plname;
//             if (*itor == plName)
//             {
//                 StSdpCodec code;
//                 code.iPayload = codec.pltype;
//                 code.pcName = codec.plname;
//                 code.iBitRate = codec.plfreq;
//                 sdpCode.push_back(code);
//             }
//         }
//     }
// }

// bool JssmmeManager::insertChannel(const std::string & callId, JssmmeChannelPtr &channel)
// {
//     tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexCallIdChannel);
//     callIdJssmmeChannelMap::iterator itor = m_mapCallIdJssmmeChannels.find(callId);
//     if (itor == m_mapCallIdJssmmeChannels.end())
//     {
//         m_mapCallIdJssmmeChannels.insert(make_pair(callId, channel));
//         m_channelCount++;
//     }

//     return true;
// }

// JssmmeChannelPtr JssmmeManager::getChannel(const std::string & callId)
// {
//     tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexCallIdChannel);
//     callIdJssmmeChannelMap::iterator itor = m_mapCallIdJssmmeChannels.find(callId);
//     if (itor != m_mapCallIdJssmmeChannels.end())
//     {
//         return itor->second;
//     }

//     return 0;
// }

// bool JssmmeManager::updateChannel(const std::string & callId, int channelId, bool isCaller)
// {
//     tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexCallIdChannel);
//     callIdJssmmeChannelMap::iterator itor = m_mapCallIdJssmmeChannels.find(callId);
//     if (itor != m_mapCallIdJssmmeChannels.end())
//     {
//         if(isCaller)
//             itor->second->m_sipChannelId = channelId;
//         else
//             itor->second->m_appChannelId = channelId;

//         return true;
//     }

//     return false;
// }

// int JssmmeManager::createChannel()
// {
//     int channelId = _Base->CreateChannel();
//     if (channelId > 0)
//         m_channelIdCount++;

//     return channelId;
// }

// int JssmmeManager::getMaxNumOfChannels()
// {
//     return _Base->MaxNumOfChannels();
// }

// int JssmmeManager::setMaxChannelsPerThread(int channels)
// {
//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, channels:" << channels << endl);
//     return _Base->SetMaxChannelsPerThread(channels);
// }

// int JssmmeManager::delChannel(int channelId)
// {
//     int ret = _Base->DeleteChannel(channelId);
//     if (!ret)
//         m_channelIdCount--;

//     return ret;
// }

// int JssmmeManager::delChannel(int appChannelId, int sipChannelId)
// {
//     _Base->DisconnectChannel(appChannelId, sipChannelId);

//     int ret = _Base->DeleteChannel(appChannelId);
//     if (!ret)
//         m_channelIdCount--;

//     ret = _Base->DeleteChannel(sipChannelId);
//     if (!ret)
//         m_channelIdCount--;
// }

// void JssmmeManager::deleteChannel(const std::string & callId)
// {
//     tars::TC_LockT<tars::TC_ThreadRecMutex> lc(m_mutexCallIdChannel);
//     callIdJssmmeChannelMap::iterator itor = m_mapCallIdJssmmeChannels.find(callId);
//     if (itor != m_mapCallIdJssmmeChannels.end())
//     {
//         m_mapCallIdJssmmeChannels.erase(itor);
//         m_channelCount--;
//     }
// }

// int JssmmeManager::setCallBack(int channelId, jssmme::Transport &callback)
// {
//     return _Netw->RegisterExternalTransport(channelId, callback);
// }

// int JssmmeManager::connectChannel(int appChannel, int sipChannel, int appDtmf, int sipDtmf)
// {
//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, appChannel:" << appChannel << ", sipChannel:" << sipChannel << ", appDtmf:" << appDtmf << ", sipDtmf:" << sipDtmf << endl);
//     return _Base->ConnectChannel(appChannel, sipChannel, appDtmf, sipDtmf);
// }

// int JssmmeManager::setLocalReceiver(int channelId, int port)
// {
//     return _Base->SetLocalReceiver(channelId, port);
// }

// int JssmmeManager::setSendDestination(int channel1, int port, const std::string & ip)
// {
//     return _Base->SetSendDestination(channel1, port, ip.c_str());
// }

// int JssmmeManager::receivedRTPPacket(int channelId, const unsigned char * msg, int size)
// {
//     return _Netw->ReceivedRTPPacket(channelId, msg, size);
// }

// int JssmmeManager::receivedRTCPPacket(int channelId, const unsigned char * msg, int size)
// {
//     return _Netw->ReceivedRTCPPacket(channelId, msg, size);
// }

// int JssmmeManager::setFecStatus(int channelId, int red)
// {
//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called,channelId:" << channelId << ", red:" << red << endl);

//     return _RtpRtcp->SetFECStatus(channelId, jssmme::kFecAuto, red);
// }

// bool JssmmeManager::createChannels(JssmmeChannelPtr pChannel, std::string & reason)
// {
//     if (pChannel->m_vectSipSendCodec.size() == 0 || pChannel->m_vectAppSendCodec.size() == 0 || pChannel->m_channelCreated)
//     {
//         TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, media nego failed app size:" << (int)pChannel->m_vectAppSendCodec.size()
//             << ", sip size:" << (int)pChannel->m_vectSipSendCodec.size() << ", isCreated:" << pChannel->m_channelCreated << endl);
//         return false;
//     }

//     if (pChannel->m_appMediaNegoStatus != kMediaNegoFinished || pChannel->m_sipMediaNegoStatus != kMediaNegoFinished)
//     {
//         TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, media nego incomplete, callId:" << pChannel->m_callId << endl);
//         return false;
//     }

//     if (pChannel->m_sipCallBackType == kArcCallback)
//     {
//         if (pChannel->m_sipArcCallBack == 0 || pChannel->m_appArcCallBack == 0)
//         {
//             TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, media channel create incomplete, callId:" << pChannel->m_callId << endl);
//             return false;
//         }
//     }
//     else
//     {
//         if (pChannel->m_sipContentCallBack == 0 || pChannel->m_appSessionCallBack == 0)
//         {
//             TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, media channel create incomplete, callId:" << pChannel->m_callId << endl);
//             return false;
//         }
//     }

//     int sipChannelId = createChannel();
//     if (sipChannelId < 0)
//     {
//         reason = SipGateway::SIP_GW_RPC_REASON_MEDIA_CHANNEL_CREATE_FAILED;
//         TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, sip channel create failed, callId:" << pChannel->m_callId << endl);
//         return false;
//     }

//     pChannel->m_sipChannelId = sipChannelId;
//     std::string arcType;
//     if (pChannel->m_sipCallBackType == kArcCallback)
//     {
//         arcType += "sip arc cb";
//         if (!pChannel->m_sipArcCallBack)
//         {
//             delChannel(sipChannelId);
//             return false;
//         }
//         setCallBack(sipChannelId, *(pChannel->m_sipArcCallBack));
//     }
//     else
//     {
//         arcType += "sip content cb";
//         if (!pChannel->m_sipContentCallBack)
//         {
//             delChannel(sipChannelId);
//             return false;
//         }
//         setCallBack(sipChannelId, *(pChannel->m_sipContentCallBack));
//     }

//     if (pChannel->m_sipRedPayload > 0)
//     {
//         setFecStatus(sipChannelId, pChannel->m_sipRedPayload);
//         setSendRedCodec(sipChannelId, pChannel->m_sipRedPayload);
//     }

//     setSendCodec(sipChannelId, pChannel->m_vectSipSendCodec);

//     int appChannelId = createChannel();
//     if (appChannelId < 0)
//     {
//         reason = SipGateway::SIP_GW_RPC_REASON_MEDIA_CHANNEL_CREATE_FAILED;
//         TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called, app channel create failed, callId:" << pChannel->m_callId << endl);
//         delChannel(sipChannelId);
//         return false;
//     }
//     pChannel->m_appChannelId = appChannelId;

//     if (pChannel->m_appCallBackType == kArcCallback)
//     {
//         arcType += ", app arc cb";
//         if (!pChannel->m_appArcCallBack)
//         {
//             delChannel(sipChannelId);
//             delChannel(appChannelId);
//             return false;
//         }

//         setCallBack(appChannelId, *(pChannel->m_appArcCallBack));
//     }
//     else
//     {
//         arcType += ", app session cb";
//         if (!pChannel->m_appSessionCallBack)
//         {
//             delChannel(sipChannelId);
//             delChannel(appChannelId);
//             return false;
//         }
//         setCallBack(appChannelId, *(pChannel->m_appSessionCallBack));
//     }

//     if (pChannel->m_appRedPayload > 0)
//     {
//         setFecStatus(appChannelId, pChannel->m_appRedPayload);
//         setSendRedCodec(appChannelId, pChannel->m_appRedPayload);
//     }

//     setSendCodec(appChannelId, pChannel->m_vectAppSendCodec);

//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, appChannel:" << appChannelId << ", sipChannel:" << sipChannelId << ", appDtmf:" << pChannel->m_appDtmfPayload << ", sipDtmf:" << pChannel->m_sipDtmfPayload
//         << ", appRed:" << pChannel->m_appRedPayload << ", sipRed:" << pChannel->m_sipRedPayload << ", appSendCodec:" << pChannel->m_vectAppSendCodec.at(0).iPayload << ", sipSendCodec:" << pChannel->m_vectSipSendCodec.at(0).iPayload
//         << ", callId:" << pChannel->m_callId << ", arcType:" << arcType << endl);

//     if (pChannel->m_appDtmfPayload > 0 && pChannel->m_sipDtmfPayload > 0)
//         _Base->ConnectChannel(appChannelId, sipChannelId, pChannel->m_appDtmfPayload, pChannel->m_sipDtmfPayload);
//     else
//         _Base->ConnectChannel(appChannelId, sipChannelId, -1, -1);

//     if (pChannel->m_bRecord && pChannel->m_at && !pChannel->m_recordStarted)
//     {
//         startCallRecord(pChannel, pChannel->m_appChannelId);
//     }

//     pChannel->m_channelCreated = true;
//     return true;
// }

// bool JssmmeManager::createChannels(JssmmeChannelPtr pChannel, std::string & reason, StSdpCodec & acPayload, int dtmfPayload, int redPayload)
// {
//     if (!pChannel->m_channelCreated)
//     {
//         vector<StSdpCodec> vectStSdpCodec;
//         vectStSdpCodec.push_back(acPayload);
//         pChannel->m_vectSipSendCodec = vectStSdpCodec;
//         pChannel->m_sipDtmfPayload = dtmfPayload;
//         pChannel->m_sipRedPayload = redPayload;
//         return true;
//     }

//     if(acPayload.iPayload < 0 || pChannel->m_vectSipSendCodec.size() == 0)
//     {
//         return false;
//     }

//     int payload = pChannel->m_vectSipSendCodec.at(0).iPayload;

//     if (acPayload.iPayload == payload && pChannel->m_sipRedPayload == redPayload && pChannel->m_sipDtmfPayload == dtmfPayload)
//     {
//         return false;
//     }

//     _Base->DisconnectChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId);
//     if (redPayload > 0)
//     {
//         setFecStatus(pChannel->m_sipChannelId, redPayload);
//         setSendRedCodec(pChannel->m_sipChannelId, redPayload);
//     }

//     vector<StSdpCodec> vectStSdpCodec;
//     vectStSdpCodec.push_back(acPayload);
//     setSendCodec(pChannel->m_sipChannelId, vectStSdpCodec);
//     pChannel->m_vectSipSendCodec = vectStSdpCodec;
//     pChannel->m_sipDtmfPayload = dtmfPayload;
//     pChannel->m_sipRedPayload = redPayload;

//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, appChannel:" << pChannel->m_appChannelId << ", sipChannel:" << pChannel->m_sipChannelId << ", appDtmf:" << pChannel->m_appDtmfPayload
//         << ", sipDtmf:" << pChannel->m_sipDtmfPayload << ", sipRed:" << pChannel->m_sipRedPayload << ", appSendCodec:" << pChannel->m_vectAppSendCodec.at(0).iPayload << ", sipSendCodec:" << pChannel->m_vectSipSendCodec.at(0).iPayload
//         << ", callId:" << pChannel->m_callId << endl);

//     if (pChannel->m_appDtmfPayload > 0 && pChannel->m_sipDtmfPayload > 0)
//         _Base->ConnectChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId, pChannel->m_appDtmfPayload, pChannel->m_sipDtmfPayload);
//     else
//         _Base->ConnectChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId, -1, -1);

//     pChannel->m_channelCreated = true;
//     return true;
// }

// bool JssmmeManager::recreateChannels(const std::string & callId)
// {
//     JssmmeChannelPtr pChannel = getChannel(callId);
//     if (!pChannel)
//     {
//         TLOGWARN("[RtpGateway] " << __FUNCTION__ << " pChannel == 0, callId:" << callId << endl);
//         return false;
//     }

//     if (!pChannel->m_channelCreated)
//     {
//         TLOGWARN("[RtpGateway] " << __FUNCTION__ << " called, channel is not ready callId:" << callId << endl);
//         return false;
//     }

//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, callId:" << callId << endl);

//     _Base->DisconnectChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId);

//     if (pChannel->m_appDtmfPayload > 0 && pChannel->m_sipDtmfPayload > 0)
//         _Base->ConnectChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId, pChannel->m_appDtmfPayload, pChannel->m_sipDtmfPayload);
//     else
//         _Base->ConnectChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId, -1, -1);

//     if (pChannel->m_bRecord && !pChannel->m_recordStarted)
//     {
//         startCallRecord(pChannel, pChannel->m_appChannelId);
//     }
//     return true;
// }

// int JssmmeManager::GetCodecByName(const char *pcName, jssmme::CodecInst *pCodec, int iFreq)
// {
//     int i, iCnt;

//     iCnt = _Codec->NumOfCodecs();
//     for (i = 0; i < iCnt; i++)
//     {
//         if (_Codec->GetCodec(i, *pCodec) != 0)
//             continue;

//         TLOGINFO("[RtpGateway] " << __FUNCTION__ << "() called, codec.plname:" << pCodec->plname 
//             << ", pcName:" << pcName << ", codec.plfreq:" << pCodec->plfreq << ", iFreq:" << iFreq << endl);

//         if (strcmp(pcName, pCodec->plname) == 0)
//         {
//             if (iFreq == -1 || iFreq == pCodec->plfreq)
//                 return 0;
//         }
//     }

//     return -1;
// }

// void JssmmeManager::clearExpiredChannels()
// {
//     int64_t curTime = Common::getCurTimeMs();
//     tars::TC_LockT<tars::TC_ThreadRecMutex> lk(m_mutexCallIdChannel);
//     callIdJssmmeChannelMap::iterator itId = m_mapCallIdJssmmeChannels.begin();
//     for (; itId != m_mapCallIdJssmmeChannels.end(); )
//     {
//         if (!itId->second)
//         {
//             m_channelCount--;
//             m_mapCallIdJssmmeChannels.erase(itId++);
//             continue;
//         }

//         JssmmeChannelPtr pChannel = itId->second;
//         if ((curTime - pChannel->m_createTime) > m_expiredTime)
//         {
//             m_channelCount--;
//             TLOGINFO("[RtpGateway] clearExpiredChannels() called, callId:" << itId->first << ", m_appChannelId" << pChannel->m_appChannelId << ", m_sipChannelId" << pChannel->m_sipChannelId << endl);
//             delChannel(pChannel->m_appChannelId, pChannel->m_sipChannelId);
//             m_mapCallIdJssmmeChannels.erase(itId++);
//         }
//         else
//         {
//             itId++;
//         }
//     }
// }

// bool JssmmeManager::setAnswerTime(const std::string & callId)
// {
//     JssmmeChannelPtr pChannel = getChannel(callId);
//     if (!pChannel)
//     {
//         TLOGWARN("[RtpGateway] " << __FUNCTION__ << " pChannel == 0, callId:" << callId << endl);
//         return false;
//     }

//     pChannel->m_at = Common::getCurTimeMs();
//     return true;
// }

// bool JssmmeManager::startCallRecord(JssmmeChannelPtr& pChannel, int channelId)
// {
//     jssmme::CodecInst stCodec;
//     int ret = GetCodecByName("PCMA", &stCodec, 8000);
//     if (ret < 0)
//     {
//         TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called,GetCodecByName failed callId:" << pChannel->m_callId << ", ret:" << ret << endl);
//     }
    
//     std::string path = pChannel->m_recordPath + "/" + pChannel->m_st + "_" + pChannel->m_callId + "_" + pChannel->m_caller + "_" + pChannel->m_callee + ".wav";
//     ret = _File->StartRecordingCall(channelId, path.c_str(), &stCodec);
//     if (ret < 0)
//     {
//         TLOGERROR("[RtpGateway] " << __FUNCTION__ << " called,StartRecordingCall failed callId:" << pChannel->m_callId << ", ret:" << ret << endl);
//     }

//     pChannel->m_recordStarted = true;
//     TLOGINFO("[RtpGateway] " << __FUNCTION__ << " called, callId:" << pChannel->m_callId << ", record path:" << path << endl);

//     return ret < 0 ? false : true;
// }

// bool JssmmeChannel::getEmptyPkgCheckPres()
// {
//     return (m_at && m_channelCreated && m_mediaEmptyPkgCheckPres && (TNOWMS - m_at >= 2000)) ? true : false;
// }

//================== JssmmeChannel =========================
JssmmeChannel::JssmmeChannel(const std::string & callId)
{
    m_callId = callId;
    m_sipChannelId = 0;
    m_appChannelId = 0;
    m_appDtmfPayload = -1;
    m_sipDtmfPayload = 0;
    m_appRedPayload = 0;
    m_sipRedPayload = 0;
    m_channelCreated = false;
    m_bRecord = false;
    m_recordStarted= false;
    m_createTime = TNOWMS;
    m_at = 0;

    // m_appCallBackType = kSessionCallbackInvalid;
    // m_sipCallBackType = kSessionCallbackInvalid;
    m_appMediaNegoStatus = kMediaNegoInvalid;
    m_sipMediaNegoStatus = kMediaNegoInvalid;
}

JssmmeChannel::~JssmmeChannel()
{
    TLOGINFO("[JssmmeChannel] " << __FUNCTION__ << "() called, callId:" << m_callId << endl);
}

int JssmmeChannel::getChannel(bool isCaller)
{
    return isCaller ? m_sipChannelId: m_appChannelId;
}