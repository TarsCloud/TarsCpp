#include "SipDbImp.h"
#include "servant/Application.h"
#include "servant/RemoteLogger.h"
#include <string>
#include <cctype>

#include "util/tc_common.h"
#include "DbConn/DbConn.h"
#include "SipProxyDbCommon.h"

using namespace DbConn::SQL;
//using namespace Redis;

//////////////////////////////////////////////////////
void SipDbImp::initialize()
{
    m_bServiceActibed = false;
    m_lastSchdTime = 0;
    m_baseSchdTime = 5 * 1000;
    //m_redis = 0;
    m_keyPrifixCall = "sip:call:";

}

//////////////////////////////////////////////////////
void SipDbImp::destroy()
{
    //destroy servant here:
    //...
}

// void SipDbImp::onSchd()
// {
//     int64_t curTime = TNOWMS;
//     if ((curTime - m_lastSchdTime) > m_baseSchdTime)
//     {
//         setStatisticInfo();
//         m_lastSchdTime = curTime;
//     }

//     std::string data = TC_TIMESTR("%d_%02d", TNOWMS);
//     if (data != m_date)
//     {
//         m_date = data;
//         _application->setStatistics("Confing.Date", m_date);
//     }

//     if (_sql)
//         _sql->schd();
// }

// void SipDbImp::onUpdateConfigs()
// {
//     Service::ServiceManagerI::onUpdateConfigs();
//     if (!m_bServiceActibed)
//         return;

//     std::string sqlUri;
//     std::string  reason;
//     if (_application->getAppConfig("MysqlUri", sqlUri))
//     {
//         if (_sqlUri != sqlUri)
//         {
//             _sql = ConnPool::createForMysql(_application, sqlUri);
//             if (0 == _sql)
//             {
//                 TLOGERROR("[SipProxyDb] createForMysql () return  FALSE!" << endl);
//                 return;
//             }
//             if (createDatabase(SIP_PROXY_DB_DATABASE_NAME, reason) == false)
//             {
//                 TLOGERROR("[SipProxyDb] createDatabase()  called. Create Database table falied, reason :" << reason << endl);
//                 return;
//             }

//             _sql = ConnPool::createForMysql(_application, sqlUri, SIP_PROXY_DB_DATABASE_NAME);
//             if (_sql != 0)
//             {
//                 TLOGINFO("[SipProxyDb] ---> [MySqlUri|DbName] = [" << sqlUri << "|" << SIP_PROXY_DB_DATABASE_NAME << "]" << endl);

//                 _sqlUri = sqlUri;

//                 //Create tables
//                 if (!CreateTable(SipProxyDbCommon::SipTableAccout,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable()  called. Create SipAccount table falied, reason :" << reason << endl);
//                 }

//                 if (!CreateTable(SipProxyDbCommon::SipTableRegInfo,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create Register info table falied, reason :" << reason << endl);
//                 }

//                 //if (!CreateTable(SipTableSubInfo,  reason))
//                 //{
//                 //    UTIL_LOG_ERR("SipProxyDb", "CreateTable () called. Create Subscribe info table falied, reason :" + reason);
//                 //}

//                 if (!CreateTable(SipProxyDbCommon::SipTableDomain,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create Domain info table falied, reason :" << reason << endl);
//                 }

//                 if (!CreateTable(SipProxyDbCommon::SipTableSipProxyConfig,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create SipProxy Accessed info table falied, reason :" << reason << endl);
//                 }
//                 if (!CreateTable(SipProxyDbCommon::SipTableRtpGatewayConfig,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create RtpGateway Accessed info table falied, reason :" << reason << endl);
//                 }
//                 if (!CreateTable(SipProxyDbCommon::SipTableNationsIp,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create Sip Call list table falied, reason :" << reason << endl);
//                 }
//                 if (!CreateTable(SipProxyDbCommon::SipTableCallSipPolicy,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create Sip Call Policy table falied, reason :" << reason << endl);
//                 }
//                 //if (!CreateTable(SipTableDomainIM,  reason))
//                 //{
//                 //    UTIL_LOG_ERR("SipProxyDb", "CreateTable () called. Create IM domain config table falied, reason :" + reason);
//                 //}
//                 //if (!CreateTable(SipTableSipProxyConfigIM,  reason))
//                 //{
//                 //    UTIL_LOG_ERR("SipProxyDb", "CreateTable () called. Create IM proxy config table falied, reason :" + reason);
//                 //}

//                 if (!CreateTable(SipProxyDbCommon::SipTableIpWhiteList,  reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create Sip Ip whitelist table falied, reason :" << reason << endl);
//                 }

//                 if (!CreateTable(SipProxyDbCommon::SipTableNumberRouting, reason))
//                 {
//                     TLOGERROR("[SipProxyDb] CreateTable () called. Create Sip num routing table falied, reason :" << reason << endl);
//                 }
//             }
//             else
//             {
//                 TLOGWARN("[SipProxyDb] createForMysql()  return FALSE." << endl);
//             }
//         }
//     }
//     else
//     {
//         TLOGWARN("[SipProxyDb] _application->getAppConfig()  return FALSE." << endl);
//     }
//     std::string redisUri;
//     if (m_redisUri.empty()
//         && _application->getAppConfig("RedisUri", redisUri)
//         && !redisUri.empty()
//         && (m_redis = Cli::create(_application, redisUri)))
//         m_redisUri = redisUri;
//     if (m_redis)
//         m_redis->onUpdateConfigs();
// }
         
tars::Bool SipDbImp::GetDomainAccessInfo(const std::string & tableName,vector<VoipApp::DomainAccessedConfig> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_)
{
    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        reason = "conn==0";
        return false;
    }
    conn->autoCommit(true);
    int64_t startTime = TNOWMS;
    std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_DOMAIN) ? SipProxyDbCommon::SIP_TABLE_DOMAIN_2 : tableName;
    std::string statName = newTbName + std::string(__FUNCTION__);

    StmtPtr stmt = conn->findStmt(statName);
    if (!stmt) stmt = conn->insertStmt(statName, " SELECT domain_id_app_id, domain_name, app_key, relation_type, authorited,  charged, reg_when_logined,"
        " forbit_failed_re_reg, max_failed_re_reg_times, allow_re_reg_interval,   failed_re_reg_interval,  i_sbc_access,sip_bind_type, core_net_id, callee_prefix, callee_prefix_priority,   conf_account_prefix,"
        " sbc_ip, sbc_port,registrar_ip, registrar_port, from_sip_addr_type,to_sip_addr_type  FROM t_" + newTbName);

    std::string domianAppKey, domainName, appKey, relationType, coreNetId, callee_prefix, confRegPrefix,   sbc_ip, registarIp;
    int  iAutorited = 0, iCharged = 0, iRegWhenLogined = 0, iISBC_Link = 0,iSipBindType = 0, frobitFailedReReg = 0, maxFailedReREgTimes = 0, allowReRegInterval = 0, failedReRegInterval = 0 ;
    int port = 0, registarPort = 0, iCalleePrefixPriority = 0;
    int seq = 0, fromSipAddrType = 0, toSipAddrType = 0;

    stmt->bindResult(seq++, domianAppKey);
    stmt->bindResult(seq++, domainName);
    stmt->bindResult(seq++, appKey);
    stmt->bindResult(seq++, relationType);
    stmt->bindResult(seq++, iAutorited);

    stmt->bindResult(seq++, iCharged);
    stmt->bindResult(seq++, iRegWhenLogined);
    stmt->bindResult(seq++, frobitFailedReReg);
    stmt->bindResult(seq++, maxFailedReREgTimes);
    stmt->bindResult(seq++, allowReRegInterval);

    stmt->bindResult(seq++, failedReRegInterval);
    stmt->bindResult(seq++, iISBC_Link);
    stmt->bindResult(seq++, iSipBindType);
    stmt->bindResult(seq++, coreNetId);
    stmt->bindResult(seq++, callee_prefix);
    stmt->bindResult(seq++, iCalleePrefixPriority);

    stmt->bindResult(seq++, confRegPrefix);
    stmt->bindResult(seq++, sbc_ip);
    stmt->bindResult(seq++, port);
    stmt->bindResult(seq++, registarIp);
    stmt->bindResult(seq++, registarPort);

    stmt->bindResult(seq++, fromSipAddrType);
    stmt->bindResult(seq++, toSipAddrType);
    if (stmt->execute())
    {
        auto parseIntIfDigits = [](const std::string& value) -> int
        {
            std::string trimmed = TC_Common::trim(value);
            if (trimmed.empty())
            {
                return 0;
            }

            for (char ch : trimmed)
            {
                if (!std::isdigit(static_cast<unsigned char>(ch)))
                {
                    return 0;
                }
            }

            return TC_Common::strto<int>(trimmed);
        };

        while (stmt->fetch() == 0)
        {
            bool biSBC_link = iISBC_Link > 0 ? true : false;
            bool bAuthorited = iAutorited > 0 ? true : false;
            bool bCharged = iCharged > 0 ? true : false;
            bool bRegWhenLogined = iRegWhenLogined > 0 ? true : false;
            bool bForbitFailedReReg = frobitFailedReReg > 0 ? true : false;
            bool bOpenFailedRegInterval = allowReRegInterval > 0 ? true : false;

            VoipApp::DomainAccessedConfig config;
            config.m_strDomainIdAppId = TC_Common::trim(domianAppKey);
            config.m_strDomainName = TC_Common::trim(domainName);
            config.m_strAppKey = TC_Common::trim(appKey);
            config.m_strRelationType = TC_Common::trim(relationType);
            config.m_bAuthorited = bAuthorited;
            config.m_bCharged = bCharged;
            config.m_bRegWhenLogined = bRegWhenLogined;
            config.m_bForbitFailedReReg = bForbitFailedReReg;
            config.m_iMaxFailedReRegTimes = maxFailedReREgTimes;
            config.m_bAllowReRegInterval = bOpenFailedRegInterval;
            config.m_iFailedReRegInterval = failedReRegInterval;
            config.m_bISBCAccessed = biSBC_link;
            config.m_sipBindType = iSipBindType;
            config.m_strCoreNetIdName = TC_Common::trim(coreNetId);
            config.m_strCalleePrefix = TC_Common::trim(callee_prefix);
            config.m_iCalleePrefixPriority = iCalleePrefixPriority;
            config.m_strConfAccoutPrefix = TC_Common::trim(confRegPrefix);
            config.m_strSBCIp = TC_Common::trim(sbc_ip);
            config.m_iSBCPort = port;
            config.m_strRegistarIp = TC_Common::trim(registarIp);
            config.m_iRegistarPort = registarPort;
            config.m_fromSipAddrType = fromSipAddrType;
            config.m_toSipAddrType = toSipAddrType;

            vectDomainAccessed.push_back(config);
        }

        int64_t endTime = TNOWMS;
        updateCallTime("GetDomainAccessInfo_begin", endTime - startTime);
        return true;
    }

    int64_t endTime = TNOWMS;
    updateCallTime("GetDomainAccessInfo_begin", endTime - startTime);

    reason = "db-error:find1:" + stmt->getLastReason();
    return true;
}
tars::Bool SipDbImp::GetDomainAccessInfo2(const std::string & tableName,vector<VoipApp::DomainAccessedConfig2> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_)
{

    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        reason = "conn==0";
        return false;
    }
    conn->autoCommit(true);
    int64_t startTime = TNOWMS;
    std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_DOMAIN) ? SipProxyDbCommon::SIP_TABLE_DOMAIN_2 : tableName;
    std::string statName = newTbName + std::string(__FUNCTION__);

    StmtPtr stmt = conn->findStmt(statName);
    if (!stmt) stmt = conn->insertStmt(statName, " SELECT domain_id_app_id, domain_name, app_key, relation_type, authorited,  charged, reg_when_logined,"
        " forbit_failed_re_reg, max_failed_re_reg_times, allow_re_reg_interval,   failed_re_reg_interval,  i_sbc_access,sip_bind_type, core_net_id, callee_prefix, callee_prefix_priority,   conf_account_prefix,"
        " sbc_ip, sbc_port,registrar_ip, registrar_port, from_sip_addr_type,to_sip_addr_type  FROM t_" + newTbName);

    std::string domianAppKey, domainName, appKey, relationType, coreNetId, callee_prefix, confRegPrefix,   sbc_ip, registarIp;
    int  iAutorited = 0, iCharged = 0, iRegWhenLogined = 0, iISBC_Link = 0,iSipBindType = 0, frobitFailedReReg = 0, maxFailedReREgTimes = 0, allowReRegInterval = 0, failedReRegInterval = 0 ;
    int port = 0, registarPort = 0, iCalleePrefixPriority = 0;
    int seq = 0, fromSipAddrType = 0, toSipAddrType = 0;

    stmt->bindResult(seq++, domianAppKey);
    stmt->bindResult(seq++, domainName);
    stmt->bindResult(seq++, appKey);
    stmt->bindResult(seq++, relationType);
    stmt->bindResult(seq++, iAutorited);

    stmt->bindResult(seq++, iCharged);
    stmt->bindResult(seq++, iRegWhenLogined);
    stmt->bindResult(seq++, frobitFailedReReg);
    stmt->bindResult(seq++, maxFailedReREgTimes);
    stmt->bindResult(seq++, allowReRegInterval);

    stmt->bindResult(seq++, failedReRegInterval);
    stmt->bindResult(seq++, iISBC_Link);
    stmt->bindResult(seq++, iSipBindType);
    stmt->bindResult(seq++, coreNetId);
    stmt->bindResult(seq++, callee_prefix);
    stmt->bindResult(seq++, iCalleePrefixPriority);

    stmt->bindResult(seq++, confRegPrefix);
    stmt->bindResult(seq++, sbc_ip);
    stmt->bindResult(seq++, port);
    stmt->bindResult(seq++, registarIp);
    stmt->bindResult(seq++, registarPort);

    stmt->bindResult(seq++, fromSipAddrType);
    stmt->bindResult(seq++, toSipAddrType);
    if (stmt->execute())
    {
        while (stmt->fetch() == 0)
        {
            bool biSBC_link = iISBC_Link > 0 ? true : false;
            bool bAuthorited = iAutorited > 0 ? true : false;
            bool bCharged = iCharged == 1 ? true : false;
            bool bRegWhenLogined = iRegWhenLogined > 0 ? true : false;
            bool bForbitFailedReReg = frobitFailedReReg > 0 ? true : false;
            bool bOpenFailedRegInterval = allowReRegInterval > 0 ? true : false;

            VoipApp::DomainAccessedConfig2 config;
            config.m_strDomainIdAppId = TC_Common::trim(domianAppKey);
            config.m_strDomainName = TC_Common::trim(domainName);
            config.m_strAppKey = TC_Common::trim(appKey);
            config.m_strRelationType = TC_Common::trim(relationType);
            config.m_bAuthorited = bAuthorited;
            config.m_bCharged = bCharged;
            config.m_iAuthorited = iAutorited;
            config.m_iCharged = iCharged;
            config.m_bRegWhenLogined = bRegWhenLogined;
            config.m_bForbitFailedReReg = bForbitFailedReReg;
            config.m_iMaxFailedReRegTimes = maxFailedReREgTimes;
            config.m_bAllowReRegInterval = bOpenFailedRegInterval;
            config.m_iFailedReRegInterval = failedReRegInterval;
            config.m_bISBCAccessed = biSBC_link;
            config.m_sipBindType = iSipBindType;
            config.m_strCoreNetIdName = TC_Common::trim(coreNetId);
            config.m_strCalleePrefix = TC_Common::trim(callee_prefix);
            config.m_iCalleePrefixPriority = iCalleePrefixPriority;
            config.m_strConfAccoutPrefix = TC_Common::trim(confRegPrefix);
            config.m_strSBCIp = TC_Common::trim(sbc_ip);
            config.m_iSBCPort = port;
            config.m_strRegistarIp = TC_Common::trim(registarIp);
            config.m_iRegistarPort = registarPort;
            config.m_fromSipAddrType = fromSipAddrType;
            config.m_toSipAddrType = toSipAddrType;

            vectDomainAccessed.push_back(config);
        }

        int64_t endTime = TNOWMS;
        updateCallTime("GetDomainAccessInfo2_begin", endTime - startTime);
        return true;
    }

    int64_t endTime = TNOWMS;
    updateCallTime("GetDomainAccessInfo2_begin", endTime - startTime);

    reason = "db-error:find1:" + stmt->getLastReason();

    return true;
}

tars::Bool SipDbImp::GetRtpGatewayconfigInfo(const std::string & tableName,vector<VoipApp::RtpGatewayAccessedConfig> &rtpGatewayConfigLst,tars::TarsCurrentPtr _current_)
{
//     int64_t heartBeatTime = 0;

//     ConnPtr conn = _sql->getConnection();
//     if (!conn)
//     {
//         TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ",reason: conn == 0" << endl);
//         return false;
//     }
//     conn->autoCommit(true);
//     int64_t startTime = TNOWMS;
//     std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_RTPGW_CONFIG) ? SipProxyDbCommon::SIP_TABLE_RTPGW_CONFIG_2 : tableName;
//     std::string statName = newTbName + std::string(__FUNCTION__);

//     StmtPtr stmt = conn->findStmt(statName);
//     if (!stmt) stmt = conn->insertStmt(statName,
//         " SELECT rtp_gateway_name, listen_ip, min_media_port, max_media_port, area_code, nation_code, rtp_gateway_id FROM t_" + newTbName);

//     std::string rtpGatewayName,  listenIp, rtpGatewayId;
//     int minMediaPort = 0, maxMediaPort = 0, areaCode = 0, nationCode = 0;

//     stmt->bindResult(0, rtpGatewayName);
//     stmt->bindResult(1, listenIp);
//     stmt->bindResult(2, minMediaPort);
//     stmt->bindResult(3, maxMediaPort);
//     stmt->bindResult(4, areaCode);
//     stmt->bindResult(5, nationCode);
//     stmt->bindResult(6, rtpGatewayId);

//     if (stmt->execute())
//     {
//         while (stmt->fetch() == 0)
//         {
//             rtpGatewayConfigLst.push_back(VoipApp::RtpGatewayAccessedConfig(TC_Common::trim(rtpGatewayName), TC_Common::trim(listenIp), minMediaPort, maxMediaPort, areaCode, nationCode, TC_Common::trim(rtpGatewayId), heartBeatTime));
//         }

//         int64_t endTime = TNOWMS;
//         updateCallTime("GetRtpGatewayconfigInfo_begin", endTime - startTime);

//         TLOGINFO("[SipProxyDb] " << __FUNCTION__ << " rtpGatewList size:" << rtpGatewayConfigLst.size() << endl);
//         return true;
//     }

//     int64_t endTime = TNOWMS;
//     updateCallTime("GetRtpGatewayconfigInfo_begin", endTime - startTime);

//     TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":reason=" << "db-error:find1:" << stmt->getLastReason() << endl);
     return true;
}

tars::Bool SipDbImp::GetSipProxyAccessCoreNetInfo(const std::string & tableName,vector<VoipApp::SipProxyAccessedCoreNetConfig> &vectSipProxyAccessedConfig,std::string &reason,tars::TarsCurrentPtr _current_)
{
    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        reason = "conn==0";
        return false;
    }
    conn->autoCommit(true);
    int64_t startTime = TNOWMS;
    std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG) ? SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG_2 : tableName;
    std::string statName = newTbName + std::string(__FUNCTION__);
    StmtPtr stmt = conn->findStmt(statName);
    if (!stmt) stmt = conn->insertStmt(statName," SELECT sip_proxy_name, listen_ip, listen_port, send_ip, send_port, i_sbc_access, core_net_id, sbc_ip, sbc_port, proxy_id FROM t_" + newTbName);

    std::string sipProxyName,  coreNetId, sbc_ip, proxy_id, listenIp, sendIp;
    int SBCport = 0,  iISBC_Link = 0, listenPort = 0, sendPort = 0;
    bool bISBC_link = false;
    int seq = 0;
    stmt->bindResult(seq++, sipProxyName);
    stmt->bindResult(seq++, listenIp);
    stmt->bindResult(seq++, listenPort);
    stmt->bindResult(seq++, sendIp);
    stmt->bindResult(seq++, sendPort);
    stmt->bindResult(seq++, iISBC_Link);
    stmt->bindResult(seq++, coreNetId);
    stmt->bindResult(seq++, sbc_ip);
    stmt->bindResult(seq++, SBCport);
    stmt->bindResult(seq++, proxy_id);

    if (stmt->execute())
    {
        while (stmt->fetch() == 0)
        {
            bISBC_link = iISBC_Link > 0 ? true : false;
            VoipApp::SipProxyAccessedCoreNetConfig coreNetConfig;
            coreNetConfig.m_strSipProxyName = TC_Common::trim(sipProxyName);
            coreNetConfig.m_bISBCAccessed = bISBC_link;
            coreNetConfig.m_strSipProxyId = TC_Common::trim(proxy_id);
            coreNetConfig.m_strCoreNetIdName = TC_Common::trim(coreNetId);
            coreNetConfig.m_strSBCIp = TC_Common::trim(sbc_ip);
            coreNetConfig.m_iSBCPort = SBCport;
            coreNetConfig.m_listenIp = TC_Common::trim(listenIp);
            coreNetConfig.m_listenPort = listenPort;
            coreNetConfig.m_sendIp = sendIp;
            coreNetConfig.m_sendPort = sendPort;

            vectSipProxyAccessedConfig.push_back(coreNetConfig);
        }

        int64_t endTime = TNOWMS;
        updateCallTime("GetSipProxyAccessCoreNetInfo_begin", endTime - startTime);

        return true;
    }

    int64_t endTime = TNOWMS;
    updateCallTime("GetSipProxyAccessCoreNetInfo_begin", endTime - startTime);

    reason = "db-error:find1:" + stmt->getLastReason();
    return true;
}
tars::Bool SipDbImp::SetRtpGatewayAccessedId(const std::string & tableName,const std::string & rtpGatewayName,const std::string & rtpGatewayId,std::string &reason,tars::TarsCurrentPtr _current_)
{
    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        reason = "conn==0";
        return false;
    }
    conn->autoCommit(true);
    int64_t startTime =TNOWMS;
    std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_RTPGW_CONFIG) ? SipProxyDbCommon::SIP_TABLE_RTPGW_CONFIG_2 : tableName;
    std::string statName = newTbName + std::string(__FUNCTION__);

    StmtPtr stmt = conn->findStmt(statName);
    if (!stmt) stmt = conn->insertStmt(statName, "UPDATE t_" + newTbName +  " SET rtp_gateway_id=? WHERE rtp_gateway_name=?");

    stmt->bindParam(0, rtpGatewayId);
    stmt->bindParam(1, rtpGatewayName);

    if (stmt->execute())
    {
        int64_t endTime = TNOWMS;
        updateCallTime("SetRtpGatewayAccessedId_begin", endTime - startTime);
        return true;
    }

    int64_t endTime = TNOWMS;
    updateCallTime("SetRtpGatewayAccessedId_begin", endTime - startTime);

    reason = "db-error:find1:" + stmt->getLastReason();
    return true;
}
tars::Bool SipDbImp::SetSipProxyAccessedId(const std::string & tableName,const std::string & sipProxyName,const std::string & sipProxyid,std::string &reason,tars::TarsCurrentPtr _current_)
{
    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        reason = "conn==0";
        return false;
    }
    conn->autoCommit(true);
    int64_t startTime = TNOWMS;
    std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG) ? SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG_2 : tableName;
    std::string statName = newTbName + std::string(__FUNCTION__);

    StmtPtr stmt = conn->findStmt(statName);
    if (!stmt) stmt = conn->insertStmt(statName, "UPDATE t_" + newTbName +  " SET proxy_id=? WHERE sip_proxy_name=?");

    stmt->bindParam(0, sipProxyid);
    stmt->bindParam(1, sipProxyName);

    if (stmt->execute())
    {
        int64_t endTime = TNOWMS;
        updateCallTime("SetSipProxyAccessedId_begin", endTime - startTime);
        return true;
    }

    int64_t endTime = TNOWMS;
    updateCallTime("SetSipProxyAccessedId_begin", endTime - startTime);

    reason = "db-error:find1:" + stmt->getLastReason();
    return true;
}
tars::Bool SipDbImp::UpdateCallSipPolicys(const std::string & tableName,const vector<VoipApp::CallSipPolicyOperation> & OpSequence,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    DbConn::SQL::ConnPtr conn = 0;
    std::string reason;

    try
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ",reason: conn == 0" << endl);
            throw false;
        }
        conn->autoCommit(true);
        std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_ACCESS_IMS_POLICY) ? SipProxyDbCommon::SIP_TABLE_ACCESS_IMS_POLICY_2 : tableName;
        std::string strTableName = "t_" + newTbName;

        bool ret = UpdateCallSipPolicyConfigToDb(strTableName,conn, OpSequence, mysqlErrno, reason);
        if(ret == false)
        {
            if(mysqlErrno == 1146)
            {
                ret = CreateTable(SipProxyDbCommon::SipTableCallSipPolicy, reason);
                if(ret)
                {
                    if(!UpdateCallSipPolicyConfigToDb(strTableName,conn, OpSequence, mysqlErrno, reason))
                    {
                        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "UpdateCallSipPolicyConfigToDb() failed after createTable()" << endl);
                        throw false;
                    }
                }
                else
                {
                    TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "createTable(SipTableCallSipPolicy ) failed" << endl);
                    throw false;
                }
            }
            else
            {
                TLOGWARN("[SipProxyDb] " << __FUNCTION__ <<  "UpdateCallSipPolicyConfigToDb() failed" << endl);
                throw false;
            }
        }
        throw true;
    }
    catch(bool ex)
    {
        if(ex == false)
        {
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ", reason: " << reason << endl);
            return false;
        }
    }
    return true;
}

tars::Bool SipDbImp::UpdateDomainAccessConfig(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    std::string reason;
    DbConn::SQL::ConnPtr conn = 0;

    try
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            reason =  "conn_error: getConnection() return NULL";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":reason=" << reason << endl);
            throw false;
        }
        conn->autoCommit(true);
        std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_DOMAIN) ? SipProxyDbCommon::SIP_TABLE_DOMAIN_2 : tableName;
        std::string strTableName = "t_" + newTbName;

        bool ret = UpdateDomainAccessConfigToDb(strTableName,conn, OpSequence, insertedTime, mysqlErrno, reason);
        if(ret == false)
        {
            if(mysqlErrno == 1146)
            {
                ret = CreateTable(SipProxyDbCommon::SipTableDomain, reason);
                if(ret)
                {
                    if(!UpdateDomainAccessConfigToDb(strTableName,conn, OpSequence, insertedTime,  mysqlErrno, reason))
                    {
                        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "UpdateDomainAccessConfigToDb() failed after createTable()" << endl);
                        throw false;
                    }
                }
                else
                {
                    TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << "createTable( SipTableDomain ) failed" << endl);
                    throw false;
                }
            }
            else
            {
                TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << "UpdateDomainAccessConfigToDb() failed" << endl);
                throw false;
            }
        }

        throw true;
    }
    catch(bool ex)
    {
        if(ex == false)
        {
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << "UpdateDomainAccessConfigToDb() failed" << endl);
            return false;
        }
    }
    return true;
}

tars::Bool SipDbImp::UpdateDomainAccessConfig2(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation2> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    std::string reason;
    DbConn::SQL::ConnPtr conn = 0;

    try
    {
        conn = _sql->getConnection();
        if (!conn) 
        {
            reason =  "conn_error: getConnection() return NULL";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":reason=" << reason << endl);
            throw false;
        }
        conn->autoCommit(true);
        std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_DOMAIN) ? SipProxyDbCommon::SIP_TABLE_DOMAIN_2 : tableName;
        std::string strTableName = "t_" + newTbName;

        bool ret = UpdateDomainAccessConfigToDb2(strTableName,conn, OpSequence, insertedTime, mysqlErrno, reason);
        if(ret == false)
        {
            if(mysqlErrno == 1146)
            {
                ret = CreateTable(SipProxyDbCommon::SipTableDomain, reason);  
                if(ret)
                {
                    if(!UpdateDomainAccessConfigToDb2(strTableName,conn, OpSequence, insertedTime,  mysqlErrno, reason))
                    {
                        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "UpdateDomainAccessConfigToDb() failed after createTable()" << endl);
                        throw false;
                    }
                }
                else
                {
                    TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "createTable( SipTableDomain ) failed" << endl);
                    throw false;
                }
            }
            else
            {
                TLOGWARN("[SipProxyDb] " << __FUNCTION__ <<  "UpdateDomainAccessConfigToDb() failed" << endl);
                throw false;
            }
        }

        throw true;
    }
    catch(bool ex)
    {
        if(ex == false)
        {
            reason = "UpdateDomainAccessConfigToDb2() failed";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            return false;
        }
    }
    return true;
}
tars::Bool SipDbImp::UpdateSipProxyAccessConfig(const std::string & tableName,const vector<VoipApp::SipProxyConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    std::string reason;
    DbConn::SQL::ConnPtr conn = 0;

    try
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            reason =  "conn_error: getConnection() return NULL";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":reason=" << reason << endl);
            throw false;
        }
        conn->autoCommit(true);

        std::string newTbName = (tableName == SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG) ? SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG_2 : tableName;
        std::string strTableName = "t_" + newTbName;

        bool ret = UpdateSipProxyAccessConfigToDb(strTableName,conn, OpSequence, insertedTime, mysqlErrno, reason);
        if(ret == false)
        {
            if(mysqlErrno == 1146)
            {
                ret = CreateTable(SipProxyDbCommon::SipTableDomain, reason);
                if(ret)
                {
                    if(!UpdateSipProxyAccessConfigToDb(strTableName,conn, OpSequence, insertedTime,  mysqlErrno, reason))
                    {
                        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "UpdateSipProxyAccessConfigToDb() failed after createTable()" << endl);
                        throw false;
                    }
                }
                else
                {
                    TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "createTable( SipTableDomain ) failed" << endl);
                    throw false;
                }
            }
            else
            {
                TLOGWARN("[SipProxyDb] " << __FUNCTION__ <<  "UpdateSipProxyAccessConfigToDb() failed" << endl);
                throw false;
            }
        }

        throw true;
    }
    catch(bool ex)
    {
        if(ex == false)
        {
            reason = "UpdateSipProxyAccessConfigToDb() failed";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            return false;
        }
    }
    return true;
}
tars::Bool SipDbImp::getDomainLineConfig(const std::string & domainId,const std::string & appId,vector<VoipApp::DomainLineConfig> &config,std::string &reason,tars::TarsCurrentPtr _current_)
{
    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << " called conn=0" << endl);
        reason = "conn==0";
        return false;
    }

    conn->autoCommit(true);
    int64_t startTime = TNOWMS;
    std::string statName = SipProxyDbCommon::SIP_TABLE_DOMAIN_2 + std::string(__FUNCTION__);

    StmtPtr stmt = conn->findStmt(statName);
    if (!stmt) stmt = conn->insertStmt(statName, " SELECT domain_id_app_id, core_net_id, sbc_ip, sbc_port FROM t_" + SipProxyDbCommon::SIP_TABLE_DOMAIN_2);

    std::string domainIdAppId, coreNetId, sbc_ip;
    int port = 0, seq = 0;

    stmt->bindResult(seq++, domainIdAppId);
    stmt->bindResult(seq++, coreNetId);
    stmt->bindResult(seq++, sbc_ip);
    stmt->bindResult(seq++, port);

    if (stmt->execute())
    {
        std::string domainIdAppIdTmp = domainId + ":"+ appId;
        while (stmt->fetch() == 0)
        {
            std::string domain, app;
            decodeDomainIdAppId(domainIdAppId, domain, app);

            if (TC_Common::strto<int>(domain) <= 0)
            {
                continue;
            }

            if (app != "*")
            {
                if (TC_Common::strto<int>(app) == 0 && app.size() > 1)
                {
                    continue;
                }

                char ch;
                ch = app.at(0);
                if (ch == '0' && app.size() > 1)
                {
                    continue;
                }
            }

            if (domainIdAppId == domainIdAppIdTmp || (appId == "*" && domain == domainId))
            {
                VoipApp::DomainLineConfig lineConfig;
                lineConfig.domainId = domainId;
                lineConfig.appId = appId;
                lineConfig.coreNetId = TC_Common::trim(coreNetId);
                lineConfig.sbcIp = TC_Common::trim(sbc_ip);
                lineConfig.sbcPort = std::to_string(port);
                config.push_back(lineConfig);
            }
        }

        bool ret = true;
        if (config.size() == 0)
        {
            TLOGWARN("[SipProxyDb] getDomainLineConfig vectDomainConfig = 0, domainId:" << domainId << ", appId:" << appId << endl);

            reason = "domain config not exist " + stmt->getLastReason();
            ret = false;
        }

        int64_t endTime = TNOWMS;
        updateCallTime("getDomainLineConfig_begin", endTime - startTime);
        return ret;
    }

    int64_t endTime = TNOWMS;
    updateCallTime("getDomainLineConfig_begin", endTime - startTime);

    reason = "db-error:find1:" + stmt->getLastReason();
    return true;
}

tars::Bool SipDbImp::getNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const std::string & callee,std::string &displayName,std::string &coreNetId,std::string &reason,tars::TarsCurrentPtr _current_)
{
    int64_t lastInsertTime = 0;
    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        reason = "conn==0";
        return false;
    }
    conn->autoCommit(true);
    int64_t startCallTime = TNOWMS;
    std::string statName = tableName + std::string(__FUNCTION__);

    std::string cmd = " SELECT display_number, core_net_id FROM t_" + tableName + " WHERE domain_id_app_id=? AND callee_number =? ;";

    StmtPtr stmt = conn->findStmt(statName);
    if (!stmt) stmt = conn->insertStmt(statName, cmd);
    stmt->bindParam(0, domainIdAppId);
    stmt->bindParam(1, callee);

    int seq = 0;
    stmt->bindResult(seq++, displayName);
    stmt->bindResult(seq++, coreNetId);

    if (stmt->execute())
    {
        while (stmt->fetch() == 0)
        {
            break;
        }
    }
    else
    {
        reason = "db-error:find1:" + stmt->getLastReason();
        int64_t endTime = TNOWMS;
        updateCallTime("getNumberRouting_begin", endTime - startCallTime);
        return false;
    }
    int64_t endTime = TNOWMS;
    updateCallTime("getNumberRouting_begin", endTime - startCallTime);
    return true;
}

tars::Bool SipDbImp::getSipCallRecordEx(const std::string & domainId,const std::string & appId,const std::string & rcpCallId,VoipApp::SipCallRecord &record,tars::TarsCurrentPtr _current_)
{
    std::string tableName = getRecordTableName(domainId,appId);
    std::string reason;

    ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        reason = "conn==0";
        return false;
    }
    conn->autoCommit(true);
    std::string stateName =  tableName + "_getSipCallRecordEx";

    StmtPtr stmt = conn->findStmt(stateName);
    if (!stmt) stmt = conn->insertStmt(stateName,
        " SELECT st,et,from_user,to_user,to_phone ,  rpc_call_id,sip_call_id,td,call_type,direction,    conf_uri,conf_mid,cmt,sip_gateway_id,proxy_id,  core_net_id,err_code  FROM "
        + tableName + "  WHERE rpc_call_id =? ;");

    stmt->bindParam(0, rcpCallId);

    std::string fromUser, toUser, toPhone, rpcCallId, sipCallId,  confUri, confMid, sipGatewayId, sipPrxoyId,  coreNetId;
    int64_t st = 0, et = 0;
    int td = 0, callType = 0, direction = 0, cmt = 0, errCode = 0, seq = 0;

    stmt->bindResult(seq++, st);
    stmt->bindResult(seq++, et);
    stmt->bindResult(seq++, fromUser);
    stmt->bindResult(seq++, toUser);
    stmt->bindResult(seq++, toPhone);

    stmt->bindResult(seq++, rpcCallId);
    stmt->bindResult(seq++, sipCallId);
    stmt->bindResult(seq++, td);
    stmt->bindResult(seq++, callType);
    stmt->bindResult(seq++, direction);

    stmt->bindResult(seq++, confUri);
    stmt->bindResult(seq++, confMid);
    stmt->bindResult(seq++, cmt);
    stmt->bindResult(seq++, sipGatewayId);
    stmt->bindResult(seq++, sipPrxoyId);

    stmt->bindResult(seq++, coreNetId);
    stmt->bindResult(seq++, errCode);

    if (stmt->execute())
    {
        if (stmt->fetch() == 0)
        {
            record.m_st = st;
            record.m_et = et;
            record.m_fromUser = fromUser;
            record.m_toUser = toUser;
            record.m_toPhone = toPhone;

            record.m_rpcCallId = rpcCallId;
            record.m_sipCallId = sipCallId;
            record.m_td = td;
            record.m_type = callType;
            record.m_direction = direction;

            record.m_confUri = confUri;
            record.m_confMid = confMid;
            record.m_cmt = cmt;
            record.m_sipGatewayId = sipGatewayId;
            record.m_sipProxyId = sipPrxoyId;

            record.m_coreNetId =coreNetId;
            record.m_errCode = errCode;
        }
        return true;
    }

    reason = "db-error:find1:" + stmt->getLastReason();
    return false;
}

tars::Bool SipDbImp::insertCallRecords(const std::string & tableName,const std::string & domainId,const std::string & appId,const vector<VoipApp::CallRecord> & records,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    std::string reason;
    DbConn::SQL::ConnPtr conn = 0;

    try
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            reason = "conn_error: getConnection_Mysql() return NULL";
            throw false;
        }

        if(!insertCallRecordsToDb(conn, tableName, domainId, appId, records, mysqlErrno, reason))
        {
            throw false;
        }
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            return false;
        }
    }
    return true;
}
tars::Bool SipDbImp::insertNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    DbConn::SQL::ConnPtr conn = 0;

    try
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            reason =  "conn_error: getConnection() return NULL";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":reason=" << reason << endl);
            throw false;
        }
        conn->autoCommit(true);
        std::string strTableName = "t_" + tableName;

        bool ret = UpdateNumRoutingConfigToDb(strTableName,conn, numRouting, VoipApp::kOperationCategoryAdd, TNOWMS, mysqlErrno, reason);
        if(ret == false)
        {
            if(mysqlErrno == 1146)
            {
                ret = CreateTable(SipProxyDbCommon::SipTableNumberRouting, reason);
                if(ret)
                {
                    if(!UpdateNumRoutingConfigToDb(strTableName,conn, numRouting, VoipApp::kOperationCategoryAdd, TNOWMS,  mysqlErrno, reason))
                    {
                        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "UpdateDomainAccessConfigToDb() failed after createTable()" << endl);
                        throw false;
                    }
                }
                else
                {
                    TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "createTable( SipTableDomain ) failed" << endl);
                    throw false;
                }
            }
            else
            {
                TLOGWARN("[SipProxyDb] " << __FUNCTION__ <<  "UpdateDomainAccessConfigToDb() failed" << endl);
                throw false;
            }
        }

        throw true;
    }
    catch(bool ex)
    {
        if(ex == false)
        {
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            return false;
        }
    }
    return true;
}

tars::Bool SipDbImp::insertRecordsBatch(const std::string & domainId,const std::string & appId,const vector<VoipApp::SipCallRecord> & records,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    std::string reason;
    DbConn::SQL::ConnPtr conn = 0;

    try
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            reason = "conn_error: getConnection_Mysql() return NULL";
            throw false;
        }

        if(!insertSipRecordsToDb(conn, domainId, appId, records, mysqlErrno, reason))
        {
            throw false;
        }
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            return false;
        }
    }
    return true;
}

tars::Bool SipDbImp::removeNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_)
{
    unsigned int mysqlErrno = 0;
    DbConn::SQL::ConnPtr conn = 0;

    try
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            reason =  "conn_error: getConnection() return NULL";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":reason=" << reason << endl);
            throw false;
        }
        conn->autoCommit(true);
        std::string strTableName = "t_" + tableName;

        bool ret = UpdateNumRoutingConfigToDb(strTableName,conn, numRouting, VoipApp::kOperationCategoryRemove, TNOWMS, mysqlErrno, reason);
        if(ret == false)
        {
            if(mysqlErrno == 1146)
            {
                ret = CreateTable(SipProxyDbCommon::SipTableNumberRouting, reason);
                if(ret)
                {
                    if(!UpdateNumRoutingConfigToDb(strTableName,conn, numRouting, VoipApp::kOperationCategoryRemove, TNOWMS,  mysqlErrno, reason))
                    {
                        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "UpdateDomainAccessConfigToDb() failed after createTable()" << endl);
                        throw false;
                    }
                }
                else
                {
                    TLOGWARN("[SipProxyDb] " << __FUNCTION__ << "createTable( SipTableDomain ) failed" << endl);
                    throw false;
                }
            }
            else
            {
                TLOGWARN("[SipProxyDb] " << __FUNCTION__ <<  "UpdateDomainAccessConfigToDb() failed" << endl);
                throw false;
            }
        }

        throw true;
    }
    catch(bool ex)
    {
        if(ex == false)
        {
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            return false;
        }
    }
    return true;
}

tars::Bool SipDbImp::updateDomainLineConfig(const std::string & domainId,const std::string & appId,const std::string & coreNetId,const std::string & origSbcIp,const std::string & origSbcPort,const std::string & dstSbcIp,const std::string & dstSbcPort,std::string &reason,tars::TarsCurrentPtr _current_)
{
    ConnPtr conn;
    try 
    {
        conn = _sql->getConnection();
        if (!conn)
        {
            reason = "conn == 0";
            throw false;
        }

        bool r = conn->autoCommit(false);
        if(!r)
        {
            reason = conn->getLastReason();
            throw false;
        }

        int64_t startTime = TNOWMS;
        std::string statement = "UPDATE t_" + SipProxyDbCommon::SIP_TABLE_DOMAIN_2;
        statement += " SET sbc_ip='" + dstSbcIp + "',";
        statement += " sbc_port='" + dstSbcPort + "'";
        statement += " WHERE sbc_ip=\'" + origSbcIp + "\' AND sbc_port=\'" + origSbcPort + "\' AND core_net_id=\'" + coreNetId + "\';";

        bool executeResult = conn->execute(statement);
        if(!executeResult)
        {
            reason = "update domain cfg failed " + std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
            throw false;
        }

        int64_t rows = conn->affectedRows();
        if (rows == 0)
        {
            reason = "update domain cfg failed no affected rows";
            throw false;
        }

        std::string statement2 = "UPDATE t_" + SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG_2;
        statement2 += " SET sbc_ip='" + dstSbcIp + "',";
        statement2 += " sbc_port='" + dstSbcPort + "'";
        statement2 += " WHERE sbc_ip=\'" + origSbcIp + "\' AND sbc_port=\'" + origSbcPort + "\' AND core_net_id=\'" + coreNetId + "\';";

        executeResult = conn->execute(statement2);
        if(!executeResult)
        {
            reason = "update proxy cfg failed" + std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
            throw false;
        }

        rows = conn->affectedRows();
        if (rows == 0)
        {
            reason = "update proxy cfg failed no affected rows";
            throw false;
        }

        conn->commit();
        conn->autoCommit(true);
        throw true;
    }
    catch(bool ex)
    {
        if(!ex)
        {
            TLOGERROR("[SipProxyDb] updateDomainLineConfig failed, domainId" << domainId << ", origSbcIp:" << origSbcIp << ", reason:" << reason << endl);
            if (conn)
            {
                conn->rollback();
                conn->autoCommit(true);
            }
        }

        return ex;
    }
    return true;
}

bool SipDbImp::UpdateDomainAccessConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::DomainAccessConfigOperation>&  opSequence,
    const int64_t & insert_time, unsigned int & mysqlErrno, std::string & reason)
{
    mysqlErrno = 0;
    reason.clear();

    if(conn == 0)
    {
        reason = "INPUT parameter of 'conn' is NULL";
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    if(opSequence.empty())
    {
        return true;
    }

    bool r = conn->autoCommit(false);
    if( !r )
    {
        reason = conn->getLastReason();
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }
    int64_t startTime = TNOWMS;

    VoipApp::OperationCategory op;
    std::string statName;
    DbConn::SQL::StmtPtr stmt = 0;
    std::string statement;

    bool bAllOperationOK = true;
    std::vector<VoipApp::DomainAccessConfigOperation>::const_iterator it = opSequence.begin();
    for(; it != opSequence.end(); ++it)
    {
        op = it->m_category;
        if((op <= VoipApp::kOperationCategoryUnknown) || (op > VoipApp::kOperationCategoryModify))
        {
            reason = "op is INVALID";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            bAllOperationOK = false;
            break;
        }

        std::string domainIdAppId, domainName, appKey, relationType, coreNetId, calleePrefix, confPrefix, sbcIp, registarIp;
        int iAuthorited = 0, iCharged = 0, iRegWhenLogined = 0, iForbitFailedReReg = 0, iMaxFailedReRegTimes = 0 , iAllowReRegInterval = 0, iFailedReRegInterval = 0, iI_SBCAccess = 0, iSipBindType = 0, iSBCPort = 0, iREgistrarPort = 0, iCalleePrefixPrio = 0;
        VoipApp::DomainAccessedConfig domainConfig = it->m_domainConfig;
        int fromSipAddrType = 0, toSipAddrType= 0;

        domainIdAppId = domainConfig.m_strDomainIdAppId;
        domainName = domainConfig.m_strDomainName;
        appKey = domainConfig.m_strAppKey;
        relationType = domainConfig.m_strRelationType;
        iAuthorited = domainConfig.m_bAuthorited ? 1 : 0;
        iCharged = domainConfig.m_bCharged ? 1 : 0;
        iRegWhenLogined = domainConfig.m_bRegWhenLogined ? 1 : 0;
        iForbitFailedReReg = domainConfig.m_bForbitFailedReReg ? 1: 0;
        iMaxFailedReRegTimes = domainConfig.m_iMaxFailedReRegTimes;
        iAllowReRegInterval = domainConfig.m_bAllowReRegInterval ? 1 : 0;
        iFailedReRegInterval = domainConfig.m_iFailedReRegInterval;
        iI_SBCAccess = domainConfig.m_bISBCAccessed ? 1 : 0;
        iSipBindType = domainConfig.m_sipBindType;
        coreNetId = domainConfig.m_strCoreNetIdName;
        calleePrefix = domainConfig.m_strCalleePrefix;
        confPrefix = domainConfig.m_strConfAccoutPrefix;
        sbcIp = domainConfig.m_strSBCIp;
        iSBCPort = domainConfig.m_iSBCPort;
        registarIp = domainConfig.m_strRegistarIp;
        iREgistrarPort = domainConfig.m_iRegistarPort;
        iCalleePrefixPrio = domainConfig.m_iCalleePrefixPriority;
        fromSipAddrType = domainConfig.m_fromSipAddrType;
        toSipAddrType = domainConfig.m_toSipAddrType;

        std::string statName = "UpdateDomainConfigToDb_" + domainIdAppId + "_op_" + std::to_string((int)op);

        if(op == VoipApp::kOperationCategoryAdd)
        {
            statement = "INSERT INTO ";
            statement += tableName;
            statement += " (domain_id_app_id, domain_name, app_key, relation_type, authorited, charged, reg_when_logined, forbit_failed_re_reg, max_failed_re_reg_times, allow_re_reg_interval, failed_re_reg_interval, i_sbc_access, sip_bind_type, core_net_id, callee_prefix, callee_prefix_priority, conf_account_prefix,sbc_ip, sbc_port,"
                " registrar_ip, registrar_port, from_sip_addr_type, to_sip_addr_type) VALUES (";
            statement += "\'" + domainIdAppId + "\',";
            statement += "\'" + domainName + "\',";
            statement += "\'" + appKey + "\',";
            statement += "\'" + relationType + "\',";
            statement += std::to_string(iAuthorited)+ ",";
            statement += std::to_string(iCharged)+ ",";
            statement += std::to_string(iRegWhenLogined)+ ",";
            statement += std::to_string(iForbitFailedReReg)+ ",";
            statement += std::to_string(iMaxFailedReRegTimes)+ ",";
            statement += std::to_string(iAllowReRegInterval)+ ",";
            statement += std::to_string(iFailedReRegInterval)+ ",";
            statement += std::to_string(iI_SBCAccess)+ ",";
            statement += std::to_string(iSipBindType)+ ",";
            statement += "\'" + coreNetId + "\',";
            statement += "\'" + calleePrefix + "\',";
            statement += std::to_string(iCalleePrefixPrio)+ ",";
            statement += "\'" + confPrefix + "\',";
            statement += "\'" + sbcIp + "\',";
            statement += std::to_string(iSBCPort)+ ",";
            statement += "\'" + registarIp + "\',";
            statement += std::to_string(iREgistrarPort) + ",";
            statement += std::to_string(fromSipAddrType) + ",";
            statement += std::to_string(toSipAddrType);

            statement +=  ") ON DUPLICATE KEY UPDATE from_sip_addr_type=VALUES(from_sip_addr_type),to_sip_addr_type=VALUES(to_sip_addr_type);";
        }
        else if(op == VoipApp::kOperationCategoryModify)
        {
            statement = "UPDATE ";
            statement += tableName;

            statement += " SET domain_name='" + domainName + "',";
            statement += " app_key='" + appKey + "',";
            statement += " relation_type='" + relationType + "',";
            statement += " authorited=" + std::to_string(iAuthorited) + ",";
            statement += " charged=" + std::to_string(iCharged) + ",";

            statement += " reg_when_logined=" + std::to_string(iRegWhenLogined) + ",";
            statement += " forbit_failed_re_reg=" + std::to_string(iForbitFailedReReg) + ",";
            statement += " max_failed_re_reg_times=" + std::to_string(iMaxFailedReRegTimes) + ",";
            statement += " allow_re_reg_interval=" + std::to_string(iAllowReRegInterval) + ",";
            statement += " failed_re_reg_interval=" + std::to_string(iFailedReRegInterval) + ",";

            statement += " i_sbc_access=" + std::to_string(iI_SBCAccess) + ",";
            statement += " sip_bind_type=" + std::to_string(iSipBindType) + ",";
            statement += " core_net_id='" + coreNetId + "',";
            statement += " callee_prefix='" + calleePrefix + "',";
            statement += " callee_prefix_priority=" + std::to_string(iCalleePrefixPrio) + ",";
            statement += " conf_account_prefix='" + confPrefix + "'";

            statement += " WHERE domain_id_app_id=\'" + domainIdAppId + "\' AND sbc_ip=\'" + sbcIp + "\' AND registrar_ip=\'" + registarIp + "\' AND callee_prefix=\'" + calleePrefix + "\';";
        }
        else if(op == VoipApp::kOperationCategoryRemove)
        {
            statement = "DELETE FROM ";
            statement += tableName;
            statement += " WHERE domain_id_app_id=\'" + domainIdAppId + "\' AND sbc_ip=\'" + sbcIp + "\' AND registrar_ip=\'" + registarIp + "\' AND callee_prefix=\'" + calleePrefix + "\';";
        }

        bool executeResult = conn->execute(statement);
        if(!executeResult) // execute failed
        {
            mysqlErrno = (int)conn->getLastReasonCode();
            reason = std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
            TLOGWARN("[SipProxyDb] SQL execute failed,  reason=" << reason << ", statement=" << statement << endl);
            bAllOperationOK = false;
            break;
        }
    }

    int64_t endTime = TNOWMS;
    updateCallTime("UpdateDomainAccessConfigToDb", endTime - startTime);

    if(bAllOperationOK == false)
    {
        conn->rollback();
        conn->autoCommit(true);
        return false;
    }

    conn->commit();
    conn->autoCommit(true);
    return true;
}

bool SipDbImp::UpdateDomainAccessConfigToDb2(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::DomainAccessConfigOperation2>&  opSequence, 
    const int64_t & insert_time, unsigned int & mysqlErrno, std::string & reason)
{
    mysqlErrno = 0;
    reason.clear();

    if(conn == 0)
    {
        reason = "INPUT parameter of 'conn' is NULL";
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    if(opSequence.empty())
    {
        return true;
    }

    bool r = conn->autoCommit(false);
    if( !r )
    {
        reason = conn->getLastReason();
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }
    int64_t startTime = TNOWMS;

    VoipApp::OperationCategory op;
    std::string statName;
    DbConn::SQL::StmtPtr stmt = 0;
    std::string statement;

    bool bAllOperationOK = true;
    std::vector<VoipApp::DomainAccessConfigOperation2>::const_iterator it = opSequence.begin();
    for(; it != opSequence.end(); ++it)
    {
        op = it->m_category;
        if((op <= VoipApp::kOperationCategoryUnknown) || (op > VoipApp::kOperationCategoryModify))
        {
            reason = "op is INVALID";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            bAllOperationOK = false;
            break;
        }

        std::string domainIdAppId, domainName, appKey, relationType, coreNetId, calleePrefix, confPrefix, sbcIp, registarIp;
        int iAuthorited = 0, iCharged = 0, iRegWhenLogined = 0, iForbitFailedReReg = 0, iMaxFailedReRegTimes = 0 , iAllowReRegInterval = 0, iFailedReRegInterval = 0, iI_SBCAccess = 0, iSipBindType = 0, iSBCPort = 0, iREgistrarPort = 0, iCalleePrefixPrio = 0;
        VoipApp::DomainAccessedConfig2 domainConfig = it->m_domainConfig;
        int fromSipAddrType = 0, toSipAddrType= 0;

        domainIdAppId = domainConfig.m_strDomainIdAppId;
        domainName = domainConfig.m_strDomainName;
        appKey = domainConfig.m_strAppKey;
        relationType = domainConfig.m_strRelationType;
        iAuthorited = domainConfig.m_iAuthorited;
        iCharged = domainConfig.m_iCharged;
        iRegWhenLogined = domainConfig.m_bRegWhenLogined ? 1 : 0;
        iForbitFailedReReg = domainConfig.m_bForbitFailedReReg ? 1: 0;
        iMaxFailedReRegTimes = domainConfig.m_iMaxFailedReRegTimes;
        iAllowReRegInterval = domainConfig.m_bAllowReRegInterval ? 1 : 0;
        iFailedReRegInterval = domainConfig.m_iFailedReRegInterval;
        iI_SBCAccess = domainConfig.m_bISBCAccessed ? 1 : 0;
        iSipBindType = domainConfig.m_sipBindType;
        coreNetId = domainConfig.m_strCoreNetIdName;
        calleePrefix = domainConfig.m_strCalleePrefix;
        confPrefix = domainConfig.m_strConfAccoutPrefix;
        sbcIp = domainConfig.m_strSBCIp;
        iSBCPort = domainConfig.m_iSBCPort;
        registarIp = domainConfig.m_strRegistarIp;
        iREgistrarPort = domainConfig.m_iRegistarPort;
        iCalleePrefixPrio = domainConfig.m_iCalleePrefixPriority;
        fromSipAddrType = domainConfig.m_fromSipAddrType;
        toSipAddrType = domainConfig.m_toSipAddrType;

        std::string statName = "UpdateDomainConfigToDb_" + domainIdAppId + "_op_" + std::to_string((int)op);

        if(op == VoipApp::kOperationCategoryAdd)
        {
            statement = "INSERT INTO ";
            statement += tableName;
            statement += " (domain_id_app_id, domain_name, app_key, relation_type, authorited, charged, reg_when_logined, forbit_failed_re_reg, max_failed_re_reg_times, allow_re_reg_interval, failed_re_reg_interval, i_sbc_access, sip_bind_type, core_net_id, callee_prefix, callee_prefix_priority, conf_account_prefix,sbc_ip, sbc_port,"
                " registrar_ip, registrar_port, from_sip_addr_type, to_sip_addr_type) VALUES (";
            statement += "\'" + domainIdAppId + "\',";
            statement += "\'" + domainName + "\',";
            statement += "\'" + appKey + "\',";
            statement += "\'" + relationType + "\',";
            statement += std::to_string(iAuthorited)+ ",";
            statement += std::to_string(iCharged)+ ",";
            statement += std::to_string(iRegWhenLogined)+ ",";
            statement += std::to_string(iForbitFailedReReg)+ ",";
            statement += std::to_string(iMaxFailedReRegTimes)+ ",";
            statement += std::to_string(iAllowReRegInterval)+ ",";
            statement += std::to_string(iFailedReRegInterval)+ ",";
            statement += std::to_string(iI_SBCAccess)+ ",";
            statement += std::to_string(iSipBindType)+ ",";
            statement += "\'" + coreNetId + "\',";
            statement += "\'" + calleePrefix + "\',";
            statement += std::to_string(iCalleePrefixPrio)+ ",";
            statement += "\'" + confPrefix + "\',";
            statement += "\'" + sbcIp + "\',";
            statement += std::to_string(iSBCPort)+ ",";
            statement += "\'" + registarIp + "\',";
            statement += std::to_string(iREgistrarPort) + ",";
            statement += std::to_string(fromSipAddrType) + ",";
            statement += std::to_string(toSipAddrType);

            statement +=  ") ON DUPLICATE KEY UPDATE from_sip_addr_type=VALUES(from_sip_addr_type),to_sip_addr_type=VALUES(to_sip_addr_type);";
        }
        else if(op == VoipApp::kOperationCategoryModify)
        {
            statement = "UPDATE ";
            statement += tableName;

            statement += " SET domain_name='" + domainName + "',";
            statement += " app_key='" + appKey + "',";
            statement += " relation_type='" + relationType + "',";
            statement += " authorited=" + std::to_string(iAuthorited) + ",";
            statement += " charged=" + std::to_string(iCharged) + ",";

            statement += " reg_when_logined=" + std::to_string(iRegWhenLogined) + ",";
            statement += " forbit_failed_re_reg=" + std::to_string(iForbitFailedReReg) + ",";
            statement += " max_failed_re_reg_times=" + std::to_string(iMaxFailedReRegTimes) + ",";
            statement += " allow_re_reg_interval=" + std::to_string(iAllowReRegInterval) + ",";
            statement += " failed_re_reg_interval=" + std::to_string(iFailedReRegInterval) + ",";

            statement += " i_sbc_access=" + std::to_string(iI_SBCAccess) + ",";
            statement += " sip_bind_type=" + std::to_string(iSipBindType) + ",";
            statement += " core_net_id='" + coreNetId + "',";
            statement += " callee_prefix='" + calleePrefix + "',";
            statement += " callee_prefix_priority=" + std::to_string(iCalleePrefixPrio) + ",";
            statement += " conf_account_prefix='" + confPrefix + "'";

            statement += " WHERE domain_id_app_id=\'" + domainIdAppId + "\' AND sbc_ip=\'" + sbcIp + "\' AND registrar_ip=\'" + registarIp + "\' AND callee_prefix=\'" + calleePrefix + "\';";
        }
        else if(op == VoipApp::kOperationCategoryRemove)
        {
            statement = "DELETE FROM ";
            statement += tableName;
            statement += " WHERE domain_id_app_id=\'" + domainIdAppId + "\' AND sbc_ip=\'" + sbcIp + "\' AND registrar_ip=\'" + registarIp + "\' AND callee_prefix=\'" + calleePrefix + "\';";
        }

        bool executeResult = conn->execute(statement);
        if(!executeResult) // execute failed
        {
            mysqlErrno = (int)conn->getLastReasonCode();
            reason = std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
            TLOGWARN("[SipProxyDb] SQL execute failed,  reason=" << reason << ", statement=" << statement << endl);
            bAllOperationOK = false;
            break;
        }
    }

    int64_t endTime = TNOWMS;
    updateCallTime("UpdateDomainAccessConfigToDb", endTime - startTime);

    if(bAllOperationOK == false)
    {
        conn->rollback();
        conn->autoCommit(true);
        return false;
    }

    conn->commit();
    conn->autoCommit(true);
    return true;
}

bool SipDbImp::UpdateSipProxyAccessConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::SipProxyConfigOperation> & opSequence, 
    const int64_t & insert_time, unsigned int & mysqlErrno, std::string & reason)
{
    mysqlErrno = 0;
    reason.clear();

    if(conn == 0)
    {
        reason = "INPUT parameter of 'conn' is NULL";
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    if(opSequence.empty())
    {
        return true;
    }

    bool r = conn->autoCommit(false);
    if( !r )
    {
        reason = conn->getLastReason();
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    int64_t startTime = TNOWMS;

    VoipApp::OperationCategory op;
    std::string statName;
    DbConn::SQL::StmtPtr stmt = 0;
    std::string statement;

    bool bAllOperationOK = true;
    std::vector<VoipApp::SipProxyConfigOperation>::const_iterator it = opSequence.begin();
    for(; it != opSequence.end(); ++it)
    {
        op = it->m_category;
        if((op <= VoipApp::kOperationCategoryUnknown) || (op > VoipApp::kOperationCategoryModify))
        {
            reason = "op is INVALID";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            bAllOperationOK = false;
            break;
        }

        std::string sipProxyName, listenIp, coreNetIdName, sbcIp, sendIp;
        int lisetnPort = 0, sendPort = 0, iSBC_Access = 0, sbcPort = 0;

        VoipApp::SipProxyAccessedCoreNetConfig sipProxyCfg = it->m_sipProxyConfig;
        sipProxyName = sipProxyCfg.m_strSipProxyName;
        listenIp = sipProxyCfg.m_listenIp;
        lisetnPort = sipProxyCfg.m_listenPort;
        sendIp = sipProxyCfg.m_sendIp;
        sendPort = sipProxyCfg.m_sendPort;
        iSBC_Access = sipProxyCfg.m_bISBCAccessed ? 1 : 0;
        coreNetIdName = sipProxyCfg.m_strCoreNetIdName;
        sbcIp = sipProxyCfg.m_strSBCIp;
        sbcPort = sipProxyCfg.m_iSBCPort;

        std::string statName = "UpdateSipProxyAccessConfigToDb_" + sipProxyName + sbcIp + "_op_" + std::to_string((int)op);

        if(op == VoipApp::kOperationCategoryAdd)
        {
            statement = "INSERT INTO ";
            statement += tableName;
            statement += " (sip_proxy_name, listen_ip, listen_port, send_ip, send_port, i_sbc_access, core_net_id, sbc_ip, sbc_port) VALUES (";
            statement += "\'" + sipProxyName + "\',";
            statement += "\'" + listenIp + "\',";
            statement += std::to_string(lisetnPort)+ ",";
            statement += "\'" + sendIp + "\',";
            statement += std::to_string(sendPort)+ ",";
            statement += std::to_string(iSBC_Access)+ ",";
            statement += "\'" + coreNetIdName + "\',";
            statement += "\'" + sbcIp + "\',";
            statement += std::to_string(sbcPort)+ "";

            statement +=  ");";
        }
        else if(op == VoipApp::kOperationCategoryModify)
        {
            statement = "UPDATE ";
            statement += tableName;

            statement += " SET listen_ip='" + listenIp + "',";
            statement += " listen_port=" + std::to_string(lisetnPort) + ",";
            statement += " send_port=" + std::to_string(sendPort) + ",";
            statement += " i_sbc_access=" + std::to_string(iSBC_Access) + ",";
            statement += " sbc_port=" + std::to_string(sbcPort);

            statement += " WHERE sip_proxy_name=\'" + sipProxyName + "\' AND core_net_id=\'" + coreNetIdName + "\' AND sbc_ip=\'" + sbcIp + "\';";
        }
        else if(op == VoipApp::kOperationCategoryRemove)
        {
            statement = "DELETE FROM ";
            statement += tableName;
            statement += " WHERE sip_proxy_name=\'" + sipProxyName + "\' AND core_net_id=\'" + coreNetIdName + "\' AND sbc_ip=\'" + sbcIp + "\';";
        }

        bool executeResult = conn->execute(statement);
        if(!executeResult) // execute failed
        {
            mysqlErrno = (int)conn->getLastReasonCode();
            reason = std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
            TLOGWARN("[SipProxyDb] SQL execute failed, errorno=" << (int)mysqlErrno << ", reason=" << reason << ", statement=" << statement << endl);
            bAllOperationOK = false;
            break;
        }
    }

    int64_t endTime = TNOWMS;
    updateCallTime("UpdateSipProxyAccessConfigToDb", endTime - startTime);

    if(bAllOperationOK == false)
    {
        conn->rollback();
        conn->autoCommit(true);
        return false;
    }

    conn->commit();
    conn->autoCommit(true);
    return true;
}

bool SipDbImp::UpdateCallSipPolicyConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::CallSipPolicyOperation> & opSequence,
    unsigned int & mysqlErrno, std::string & reason)
{
    mysqlErrno = 0;
    reason.clear();

    if(conn == 0)
    {
        reason = "INPUT parameter of 'conn' is NULL";
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    if(opSequence.empty())
    {
        return true;
    }

    bool r = conn->autoCommit(false);
    if( !r )
    {
        reason = conn->getLastReason();
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    int64_t startTime = TNOWMS;
    VoipApp::OperationCategory op;
    std::string statName;
    DbConn::SQL::StmtPtr stmt = 0;
    std::string statement;

    bool bAllOperationOK = true;
    for(unsigned int i = 0; i < opSequence.size(); i++)
    {
        VoipApp::CallSipPolicyOperation policyOpera = opSequence.at(i);
        op = policyOpera.m_OpCategory;
        if((op <= VoipApp::kOperationCategoryUnknown) || (op > VoipApp::kOperationCategoryModify))
        {
            reason = "op is INVALID";
            TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
            bAllOperationOK = false;
            break;
        }

        std::string domainIdAppId = policyOpera.m_policy.m_domainIdAppId;
        int policyType = policyOpera.m_policy.m_type;
        std::string policyValue = policyOpera.m_policy.m_value;
        std::string statName = "UpdateCallSipPolicyConfigToDb_" + tableName + "_op_" + std::to_string((int)op);
        if(op == VoipApp::kOperationCategoryAdd)
        {
            statement = "INSERT INTO ";
            statement += tableName;
            statement += " (domain_id_app_id, policy_type, policy_value) VALUES (";
            statement += "\'" + domainIdAppId + "\',";
            statement += std::to_string(policyType)+ ",";
            statement += "\'" + policyValue + "\'";
            statement +=  ")";
            statement += " ON DUPLICATE KEY UPDATE policy_type=VALUES(policy_type),policy_value=VALUES(policy_value);";
        }
        else if(op == VoipApp::kOperationCategoryModify)
        {
            statement = "UPDATE ";
            statement += tableName;

            statement += " SET policy_type=" + std::to_string(policyType) + ",";
            statement += " policy_value='" + policyValue + "'";
            statement += " WHERE domain_id_app_id=\'" + domainIdAppId + "\';";
        }
        else if(op == VoipApp::kOperationCategoryRemove)
        {
            statement = "DELETE FROM ";
            statement += tableName;
            statement += " WHERE domain_id_app_id=\'" + domainIdAppId + "\';";
        }

        bool executeResult = conn->execute(statement);
        if(!executeResult) // execute failed
        {
            mysqlErrno = (int)conn->getLastReasonCode();
            reason = std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
            TLOGWARN("[SipProxyDb] SQL execute failed, errorno=" << (int)mysqlErrno << ", reason=" << reason << ", statement=" << statement << endl);
            bAllOperationOK = false;
            break;
        }
    }

    int64_t endTime = TNOWMS;
    updateCallTime(std::string(__FUNCTION__), endTime - startTime);

    if(bAllOperationOK == false)
    {
        conn->rollback();
        conn->autoCommit(true);
        return false;
    }

    conn->commit();
    conn->autoCommit(true);
    return true;
}

//functions -------------------------------------------------------------------------------------------
bool SipDbImp:: CreateTable(SipProxyDbCommon::SipTableType tableType, std::string & reason, std::string tableName)
{
    if (_sql == 0)
    {
        TLOGWARN("[SipProxyDb] CreateTable() called. _sql == 0, tableType :" << (int)tableType << endl);
        return false;
    }

    DbConn::SQL::ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        TLOGWARN("[SipProxyDb] CreateTable() called. conn == 0, tableType :" << (int)tableType << endl);
        return 0;
    }

    std::string strTableName;
    DbConn::SQL::ResultPtr res;

    switch(tableType)
    {
    case SipProxyDbCommon::SipTableAccout:
        strTableName = tableName.empty() ?  "t_" + SipProxyDbCommon::SIP_TABLE_ACCOUNT_2 : tableName;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS " + strTableName +
            "(" \
            "__id__ BIGINT NOT NULL AUTO_INCREMENT COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            " sip_uri  VARCHAR(200)        NOT NULL COMMENT 'sip账号'," \
            " sip_account  VARCHAR(100)    NOT NULL COMMENT '核心网SIP账号'," \
            " domain_name  VARCHAR(60)    NOT NULL  COMMENT '域名'," \
            " domain_id  VARCHAR(60)    NOT NULL    COMMENT '域id'," \
            " app_id  VARCHAR(60)    NOT NULL      COMMENT '应用Id'," \
            " core_net_id  VARCHAR(60)    NOT NULL  COMMENT '核心网id'," \
            " pwd  VARCHAR(60)    NOT NULL          COMMENT '用户密码'," \
            " auth_name  VARCHAR(100)    NOT NULL   COMMENT '鉴权名'," \
            " sip_proxy_name  VARCHAR(60)    NOT NULL   COMMENT 'SipProxy服务名'," \
            " app_account  VARCHAR(200)  default \"\"   COMMENT '绑定的APP账号'," \
            " relation_type  VARCHAR(60)    NOT NULL    COMMENT '账号关系类型'," \
            " in_use  TINYINT  default  0           COMMENT '账号使用状态'," \
            " proxyed  TINYINT  default  0          COMMENT '是否代理注册'," \
            " logined  TINYINT  default  0          COMMENT 'sip账号登录状态'," \
            " permited  TINYINT  default  1         COMMENT '当前账号是否被允许被代理注册'," \
            " proxy_id  VARCHAR(60)                 COMMENT 'sip账号代理服务id'," \
            " sbc_ip  VARCHAR(60)    NOT NULL       COMMENT '核心网ip'," \
            " sbc_port  INT    NOT NULL  default  0 COMMENT '核心网端口'," \
            " registrar_ip  VARCHAR(100)            COMMENT '账号注册ip'," \
            " registrar_port  INT                   COMMENT '账号注册端口'," \
            " from_sip_addr_type  TINYINT    NOT NULL  default  0   COMMENT '发起号码sip地址类型'," \
            " to_sip_addr_type  TINYINT  NOT NULL  default  0       COMMENT '接收号码sip地址类型'," \
            " insert_time BIGINT default  0         COMMENT '账号插入时间戳'," \
            " insert_date_time  VARCHAR(100)        COMMENT '账号插入时间'," \
            " failed_reg_times INT   default  0     COMMENT '注册失败次数'," \
            " failed_reg_time BIGINT default  0     COMMENT '注册失败时间戳'," \
            " failed_date_time  VARCHAR(100)        COMMENT '注册失败时间'," \
            " login_time BIGINT default  0          COMMENT '登录时间戳'," \
            " login_date_time  VARCHAR(100)         COMMENT '登录时间'," \
            " in_use_time BIGINT default  0         COMMENT '使用时间戳'," \
            " in_use_date_time  VARCHAR(100)        COMMENT '使用时间'," \
            " proxyed_time BIGINT default  0        COMMENT '注册时间成功戳'," \
            " proxyed_date_time  VARCHAR(100)       COMMENT '注册成功时间'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX(sip_uri), INDEX core_net_insert_time(core_net_id, insert_time), INDEX core_net_proxy_time(core_net_id, proxyed_time)" \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT '账号信息表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }

        break;

    case SipProxyDbCommon::SipTableRegInfo:
    case SipProxyDbCommon::SipTableSubInfo :
        strTableName = tableType == SipProxyDbCommon::SipTableRegInfo ? SipProxyDbCommon::SIP_TABLE_REGISTER_INFO_2 : SipProxyDbCommon::SIP_TABLE_SUBSCRIBE_INFO_2;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "( "\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT  COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            "sip_uri  VARCHAR(60)    NOT NULL       COMMENT 'sip账号'," \
            " sip_account  VARCHAR(60)    NOT NULL  COMMENT '核心网SIP账号'," \
            " pwd  VARCHAR(60)    NOT NULL          COMMENT '用户密码'," \
            " auth_name  VARCHAR(60)    NOT NULL    COMMENT '鉴权名'," \
            " proxy_id  VARCHAR(60)    NOT NULL     COMMENT 'SipProxy服务id'," \
            " http_req_time  BIGINT                 COMMENT '请求时间'," \
            " reged_time  BIGINT                    COMMENT '注册时间戳'," \
            " reged_date_time  VARCHAR(60)          COMMENT '注册时间'," \
            " expires  INT                          COMMENT '过期时间'," \
            " registrar_ip  VARCHAR(100)            COMMENT '注册服务ip'," \
            " registrar_port  INT                   COMMENT '注册服务端口'," \
            " client_ip   VARCHAR(60)               COMMENT '本地ip'," \
            " client_port   INT                     COMMENT '本地端口'," \
            " call_id   VARCHAR(60)                 COMMENT '通话id'," \
            " from_tag   VARCHAR(60)                COMMENT '主叫tag'," \
            " to_tag   VARCHAR(60)                  COMMENT '被叫tag'," \
            " cseq_num   INT                        COMMENT 'sip序列号'," \
            " via_branch   VARCHAR(100)             COMMENT 'sip branch'," \
            " event_type   INT                      COMMENT '事件类型'," \
            " event_id   VARCHAR(30)                COMMENT '事件id'," \
            " realm   VARCHAR(100)                  COMMENT '注册realm'," \
            " nonce   VARCHAR(100)                  COMMENT '注册nonce'," \
            " nc   INT                              COMMENT '注册nc'," \
            " opaque    VARCHAR(100)                COMMENT '注册opaque'," \
            " response  VARCHAR(100)                COMMENT '注册response'," \
            " qop_exist   TINYINT                   COMMENT 'qop标识'," \
            " qop_value   INT                       COMMENT '注册qop'," \
            " cnonce    VARCHAR(100)                COMMENT '注册cnonce'," \
            " req_user_info    VARCHAR(500)         COMMENT '用户信息'," \
            " req_uri    VARCHAR(500)               COMMENT '请求用户sip账号'," \
            " core_net_id  VARCHAR(60)              COMMENT '核心网id'," \
            " sbc_ip  VARCHAR(60)                   COMMENT '核心网ip'," \
            " sbc_port  INT    default  0           COMMENT '核心网端口'," \
            " algo  INT    default  0               COMMENT '注册algo'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX sip_uri(sip_uri), INDEX proxy_id(proxy_id)" \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT '用户注册表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableDomain :
        strTableName = SipProxyDbCommon::SIP_TABLE_DOMAIN_2;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "("\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT  COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            " domain_id_app_id  VARCHAR(120)  NOT NULL  COMMENT '接入网domainId与appId的组合'," \
            " domain_name  VARCHAR(60)    NOT NULL      COMMENT '域名'," \
            " app_key  VARCHAR(60)    NOT NULL          COMMENT '接入的APP的appKey'," \
            " relation_type  VARCHAR(60)    NOT NULL    COMMENT '账号关系类型'," \
            " authorited TINYINT default  1             COMMENT '是否授权接入'," \
            " charged TINYINT    default  0             COMMENT '计费标识'," \
            " reg_when_logined TINYINT   default  1     COMMENT '是否在登陆之后才允许代理账号注册'," \
            " forbit_failed_re_reg TINYINT   default  1     COMMENT '是否禁止注册失败之后再次发起注册'," \
            " max_failed_re_reg_times INT   default  5000   COMMENT '允许最多失败次数'," \
            " allow_re_reg_interval TINYINT   default  1    COMMENT '是否允许失败之后再次发起注册最小间隔'," \
            " failed_re_reg_interval TINYINT   default  2   COMMENT '注册失败之后再次发起注册间隔'," \
            " i_sbc_access TINYINT    NOT NULL default  0   COMMENT 'i-sbc接入标识'," \
            " sip_bind_type TINYINT    NOT NULL default  0  COMMENT 'sip绑定类型'," \
            " core_net_id  VARCHAR(60)    NOT NULL default \"\" COMMENT '核心网id'," \
            " callee_prefix  VARCHAR(60)  NOT NULL default \"\"  COMMENT '被叫前缀'," \
            " callee_prefix_priority  TINYINT    NOT NULL  default  0   COMMENT '被叫前缀优先级'," \
            " conf_account_prefix  VARCHAR(60)  default \"\"    COMMENT '会议前缀'," \
            " sbc_ip  VARCHAR(60)    NOT NULL   COMMENT '核心网ip'," \
            " sbc_port  INT    NOT NULL  default  0 COMMENT '核心网端口'," \
            " registrar_ip  VARCHAR(100)   default \"\"     COMMENT '注册ip'," \
            " registrar_port  INT  default  0               COMMENT '注册端口'," \
            " from_sip_addr_type  TINYINT    NOT NULL  default  0   COMMENT '主叫地址类型'," \
            " to_sip_addr_type  TINYINT    NOT NULL  default  0     COMMENT '被叫地址类型'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE KEY(domain_id_app_id, sbc_ip, registrar_ip, callee_prefix)" \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT '域对接信息配置表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableSipProxyConfig :
        strTableName = SipProxyDbCommon::SIP_TABLE_PROXY_CONFIG_2;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "( "\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT  COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            "sip_proxy_name VARCHAR(60)    NOT NULL             COMMENT '服务名称'," \
            " listen_ip  VARCHAR(60)    NOT NULL                COMMENT '监听ip'," \
            " listen_port  INT    NOT NULL  default  0          COMMENT '监听端口'," \
            " send_ip  VARCHAR(60)   default \"\"               COMMENT '发送ip'," \
            " send_port  INT    NOT NULL  default  0            COMMENT '发送端口'," \
            " i_sbc_access TINYINT    NOT NULL                  COMMENT 'i-sbc标识'," \
            " core_net_id  VARCHAR(60)    NOT NULL default \"\" COMMENT '核心网id'," \
            " sbc_ip  VARCHAR(60)    NOT NULL                   COMMENT '核心网ip'," \
            " sbc_port  INT    NOT NULL  default  0             COMMENT '核心网端口'," \
            " proxy_id  VARCHAR(60)                             COMMENT '服务id'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE KEY(sip_proxy_name,sbc_ip,sbc_port)" \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8   COMMENT '核心网配置表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableRtpGatewayConfig:
        strTableName = SipProxyDbCommon::SIP_TABLE_RTPGW_CONFIG_2;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "("\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT  COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP  COMMENT '插表时间'," \
            " rtp_gateway_name VARCHAR(60)    NOT NULL       COMMENT '服务名称'," \
            " listen_ip  VARCHAR(60)    NOT NULL             COMMENT '监听ip'," \
            " min_media_port  INT    NOT NULL  default  0    COMMENT '最小端口'," \
            " max_media_port  INT    NOT NULL  default  0    COMMENT '最大端口'," \
            " area_code TINYINT    NOT NULL                  COMMENT '地区码'," \
            " nation_code INT    NOT NULL                    COMMENT '国家码'," \
            " rtp_gateway_id  VARCHAR(60)  default \"\"      COMMENT '服务id'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE KEY(rtp_gateway_name)" \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8  COMMENT '媒体配置表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipCallStatistic:
        strTableName = tableName;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS " + strTableName +
            "(" \
            "__id__ BIGINT NOT NULL AUTO_INCREMENT  COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            " st BIGINT default  0                          COMMENT '起始时间'," \
            " et BIGINT default  0                          COMMENT '结束时间'," \
            " from_user  VARCHAR(100)  NOT NULL             COMMENT '主叫号码'," \
            " to_user  VARCHAR(100)  NOT NULL               COMMENT '被叫号码'," \
            " to_phone  VARCHAR(100)                        COMMENT '被叫手机号码'," \
            " rpc_call_id VARCHAR(60)    NOT NULL           COMMENT '通话id'," \
            " sip_call_id VARCHAR(200)    NOT NULL          COMMENT 'sip通话id'," \
            " td INT default  0                             COMMENT '通话时长'," \
            " call_type TINYINT default  0                  COMMENT '呼叫类型'," \
            " direction TINYINT default  0                  COMMENT '呼叫方向'," \
            " conf_uri VARCHAR(100)  default \"\"           COMMENT '会议uri'," \
            " conf_mid VARCHAR(100)  default \"\"           COMMENT '会议mid'," \
            " cmt TINYINT default  0                        COMMENT '媒体类型'," \
            " sip_gateway_id  VARCHAR(100)                  COMMENT 'SipGateway服务id'," \
            " proxy_id  VARCHAR(100)                        COMMENT 'SipProxy服务id'," \
            " core_net_id  VARCHAR(100)                     COMMENT '核心网id'," \
            " err_code INT default  0                       COMMENT '错误码'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX(rpc_call_id), INDEX(__ctime__) " \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT '统计信息表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableCallRecord:
        strTableName = tableName;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS " + strTableName +
            "(" \
            "__id__ BIGINT NOT NULL AUTO_INCREMENT  COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            " st BIGINT default 0                   COMMENT '起始时间'," \
            " eat BIGINT default 0                  COMMENT '早媒体时间'," \
            " rt BIGINT default 0                   COMMENT '振铃时间'," \
            " at BIGINT default 0                   COMMENT '接听时间'," \
            " et BIGINT default 0                   COMMENT '结束时间'," \
            " td INT default 0                      COMMENT '通话时长'," \
            " rpc_call_id VARCHAR(60) NOT NULL      COMMENT 'rtc通话id'," \
            " rtc_caller VARCHAR(100) NOT NULL      COMMENT 'rtc主叫号码'," \
            " rtc_callee VARCHAR(100) NOT NULL      COMMENT 'rtc被叫号码'," \
            " rtc_user_id VARCHAR(32)               COMMENT 'rtc主叫uid'," \
            " rtc_cc VARCHAR(16) NOT NULL           COMMENT 'rtc主叫国家码'," \
            " rtc_ip VARCHAR(60) NOT NULL           COMMENT 'rtc终端ip'," \
            " rtc_display VARCHAR(32) NOT NULL      COMMENT 'rtc主叫显号'," \
            " cmt TINYINT default  0                COMMENT '媒体类型'," \
            " direction TINYINT default 1           COMMENT '呼叫方向'," \
            " conf_uri VARCHAR(100)                 COMMENT '会议uri'," \
            " conf_mid VARCHAR(100)                 COMMENT '会议mid'," \
            " caller VARCHAR(100) NOT NULL          COMMENT '主叫号码'," \
            " callee VARCHAR(100) NOT NULL          COMMENT '被叫号码'," \
            " caller_prefix VARCHAR(16)             COMMENT '主叫前缀'," \
            " callee_prefix VARCHAR(16)             COMMENT '被叫前缀'," \
            " sip_call_id VARCHAR(200) NOT NULL     COMMENT 'sip通话id'," \
            " core_net_id VARCHAR(32)               COMMENT '核心网id'," \
            " sbc_ip VARCHAR(60)                    COMMENT 'sbc地址'," \
            " sbc_port INT default 0                COMMENT 'sbc端口'," \
            " sbc_media_ip VARCHAR(60)              COMMENT 'sbc媒体地址'," \
            " charge_number VARCHAR(100)            COMMENT '计费号码'," \
            " release_type TINYINT default 2        COMMENT '挂断类型'," \
            " sip_gateway_id VARCHAR(100)           COMMENT 'SipGateway服务id'," \
            " proxy_id VARCHAR(100)                 COMMENT 'SipProxy服务id'," \
            " sip_error_code INT default 0          COMMENT '错误码'," \
            " sip_error_desc VARCHAR(100)           COMMENT '错误码描述'," \
            " sip_error_cause INT default 0         COMMENT '错误原因'," \
            " media_error_code INT default 0        COMMENT '媒体错误码'," \
            " media_error_desc VARCHAR(100)         COMMENT '媒体错误码描述'," \
            " other_error_code INT default 0        COMMENT '其他错误'," \
            " other_error_desc VARCHAR(100)         COMMENT '其他错误描述'," \
            " params_ext VARCHAR(256)               COMMENT '预留参数'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX(rpc_call_id,sip_call_id), INDEX(__ctime__) " \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT '话单信息表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableNationsIp:
        strTableName = SipProxyDbCommon::SIP_TABLE_NATIONS_IP_2;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "("\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT                  COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            "nation_code VARCHAR(60)    NOT NULL                    COMMENT '国家码'," \
            " ip  VARCHAR(60)    NOT NULL                           COMMENT 'IP地址'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX(nation_code) " \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8   COMMENT '国家码配置表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableCallSipPolicy:
        strTableName = SipProxyDbCommon::SIP_TABLE_ACCESS_IMS_POLICY_2;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "("\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT                      COMMENT '自增主键'," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP     COMMENT '插表时间'," \
            "domain_id_app_id VARCHAR(60)    NOT NULL                   COMMENT '接入网domainId与appId的组合'," \
            "policy_type TINYINT default  -1                            COMMENT '策略类型'," \
            "policy_value  VARCHAR(60) default \"\"                     COMMENT '策略值'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX(domain_id_app_id) " \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT '呼叫策略表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableDomainIM :
        strTableName = SipProxyDbCommon::SIP_TABLE_DOMAIN_IM;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "("\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP," \
            "domain_id_app_id  VARCHAR(120)  NOT NULL  ," \
            "domain_name  VARCHAR(60)    NOT NULL," \
            " authorited TINYINT    default  1 ," \
            " core_net_id  VARCHAR(60)    NOT NULL default \"\"," \
            " sbc_ip  VARCHAR(60)    NOT NULL default \"\"," \
            " sbc_port  INT    NOT NULL  default  0," \
            " registrar_ip  VARCHAR(100)   default \"\" ," \
            " registrar_port  INT   default  0  ," \
            " sbc_access_type  INT    NOT NULL  default  0," \
            " msg_mtype  TINYINT    NOT NULL  default  1," \
            " msg_mtype_ext  VARCHAR(60)    NOT NULL default \"\"," \
            " msg_msub_type  TINYINT    NOT NULL  default  5," \
            " msg_msub_type_ext  VARCHAR(60)    NOT NULL default \"\"," \
            " content_type_deal  TINYINT    NOT NULL  default  0," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE KEY(domain_id_app_id, sbc_ip, registrar_ip)" \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8;", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableSipProxyConfigIM :
        strTableName = SipProxyDbCommon::SIP_TABLE_PROXY_IM_CONFIG_2;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "( "\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP," \
            " gateway_entry_name VARCHAR(60)    NOT NULL," \
            " listen_ip  VARCHAR(60)    NOT NULL default \"\"," \
            " listen_port  INT    NOT NULL  default  0," \
            " send_ip  VARCHAR(60)   default \"\" ," \
            " send_port  INT    NOT NULL  default  0," \
            " core_net_id  VARCHAR(60)    NOT NULL default \"\"," \
            " sbc_ip  VARCHAR(60)    NOT NULL," \
            " sbc_port  INT    NOT NULL  default  0," \
            " gateway_entry_id  VARCHAR(60)," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE KEY(gateway_entry_name,sbc_ip,sbc_port)" \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8;", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableIpWhiteList:
        strTableName = SipProxyDbCommon::SIP_TABLE_PROXY_IP_WHITE_LIST;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "("\
            "__id__ BIGINT NOT NULL AUTO_INCREMENT," \
            "__ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP," \
            "company VARCHAR(60)    NOT NULL," \
            " ip  VARCHAR(60)    NOT NULL," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX(ip) " \
            ") ENGINE=INNODB DEFAULT CHARSET=utf8;", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
        break;

    case SipProxyDbCommon::SipTableNumberRouting:
        strTableName = SipProxyDbCommon::SIP_TABLE_NUMBER_ROUTING;
        if (!conn->execute("CREATE TABLE IF NOT EXISTS t_" + strTableName +
            "(" \
            " __id__ BIGINT NOT NULL AUTO_INCREMENT COMMENT '自增主键'," \
            " __ctime__ TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '插表时间'," \
            " domain_id_app_id VARCHAR(60) NOT NULL COMMENT '应用id'," \
            " callee_number VARCHAR(60) NOT NULL COMMENT '被叫号码'," \
            " display_number VARCHAR(60) DEFAULT \"\" COMMENT '来电显号'," \
            " core_net_id VARCHAR(60) NOT NULL COMMENT '线路id'," \
            " PRIMARY KEY(__id__)," \
            " UNIQUE INDEX domain_account(domain_id_app_id, callee_number)" \
            " ) ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT '号码路由表';", res))
        {
            reason = "create_" + strTableName + "_error:" + conn->getLastReason();
            return false;
        }
    }

    return true;
}

bool SipDbImp::createDatabase(const std::string & name, std::string & reason)
{
    if (_sql == 0)
    {
        TLOGWARN("[SipProxyDb] createDatabase() called. _sql == 0, name :" << name << endl);
        return false;
    }

    DbConn::SQL::ConnPtr conn = _sql->getConnection();
    if (!conn)
    {
        TLOGWARN("[SipProxyDb] createDatabase() called. conn == 0, name :" << name << endl);
        return 0;
    }
    conn->autoCommit(true);
    bool executeResult = false;

    std::string statement = "CREATE DATABASE IF NOT EXISTS " + name + " default character set BINARY;";
    int mysqlErrno = 0;

    std::string statName = "createDatabase";
    DbConn::SQL::StmtPtr stmt = conn->insertStmt(statName, statement);

    executeResult = stmt->execute();
    if (executeResult == false)
    {
        mysqlErrno = stmt->getLastReasonCode();
        reason = stmt->name() + "_execute_error:" + stmt->getLastReason();

        TLOGWARN("[SipProxyDb] SQL execute failed, errorno=" << (int)mysqlErrno << ", reason=" << reason << ", statement=" << statement << endl);
    }
    else
    {
        mysqlErrno = 0;
        reason.clear();
    }

    conn->removeStmt(statName);

    return executeResult;
}

void SipDbImp::updateCallTime(const std::string & interfaceName, int64_t period)
{
    //TEST....
    return;

    std::lock_guard<tars::TC_ThreadRecMutex> lock(m_mutexCallStatis);
    map<std::string, CallStatistic>	::iterator iterCall = m_mapCallStatis.find(interfaceName);
    if (iterCall == m_mapCallStatis.end())
    {
        CallStatistic callSta;
        callSta.m_interfaceName = interfaceName;
        callSta.m_callTimes = 1;
        callSta.m_maxPeriod = period;
        callSta.m_avgPeriod = period;
        callSta.m_totalPeriod = period;

        m_mapCallStatis.insert(make_pair(interfaceName, callSta));

        TLOGDEBUG("[SipProxyDb]  ****** updateCallTime ****** called, Insert funcName:" << interfaceName << endl);
    }
    else
    {
        iterCall->second.m_callTimes ++;
        iterCall->second.m_maxPeriod = iterCall->second.m_maxPeriod > period ? iterCall->second.m_maxPeriod : period;
        iterCall->second.m_totalPeriod = iterCall->second.m_totalPeriod + period;
        iterCall->second.m_avgPeriod = iterCall->second.m_totalPeriod / iterCall->second.m_callTimes;
        TLOGDEBUG("[SipProxyDb]  ****** updateCallTime ****** called, Update funcName:" << interfaceName << endl);
    }
}

void SipDbImp::setStatisticInfo()
{
    map<std::string, CallStatistic> mapInfos;
    {
        std::lock_guard<tars::TC_ThreadRecMutex> lock(m_mutexCallStatis);
        mapInfos = m_mapCallStatis;
    }

    map<std::string, CallStatistic>::iterator iterFunc = mapInfos.begin();
    for (; iterFunc != mapInfos.end(); iterFunc++)
    {
        std::string funcName = iterFunc->first;
        CallStatistic statis = iterFunc->second;

        // _application->setStatisticsLong("AA_" + funcName + "_Times", statis.m_callTimes);
        // _application->setStatisticsLong("AA_" + funcName + "_maxPeriod" , statis.m_maxPeriod);
        // _application->setStatisticsLong("AA_" + funcName + "_avgPeriod" , statis.m_avgPeriod);
    }
}

bool SipDbImp::tryExecuteInsert(const DbConn::SQL::ConnPtr& conn, const std::string& tableName, SipProxyDbCommon::SipTableType tableType, const std::string& statement, unsigned int& mysqlErrno, std::string& reason)
{
    std::string mysqlCmd = "INSERT INTO " + tableName + statement;

    if (conn->execute(mysqlCmd))
        return true;

    mysqlErrno = (int)conn->getLastReasonCode();
    if (mysqlErrno == 1146)
    {
        if (CreateTable(tableType, reason, tableName))
        {
            mysqlErrno = 0;
            if (conn->execute(mysqlCmd))
                return true;

            mysqlErrno = (int)conn->getLastReasonCode();
            reason = "Table created but execute failed: " + conn->getLastReason();
        }
        else
        {
            reason = "Failed to create table: " + reason;
        }
    }
    else
    {
        reason = "Execute failed: " + conn->getLastReason();
    }
    return false;
}

bool SipDbImp::insertSipRecordsToDb(const DbConn::SQL::ConnPtr& conn, const std::string& domainId, const std::string& appId, const std::vector<VoipApp::SipCallRecord>& records,
    unsigned int& mysqlErrno, std::string& reason)
{
    if (records.empty())
        return true;

    if (!conn)
    {
        reason = "conn == 0";
        return false;
    }

    if(!conn->autoCommit(false))
    {
        reason = conn->getLastReason();
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    do
    {
        std::string tableName = getRecordTableName(domainId, appId);
        std::string newTableName = tableName + "_" + m_date;
        std::string statement = "(st," \
            " et," \
            " from_user," \
            " to_user," \
            " to_phone," \
            " rpc_call_id," \
            " sip_call_id," \
            " td," \
            " call_type," \
            " direction," \
            " conf_uri," \
            " conf_mid," \
            " cmt," \
            " sip_gateway_id," \
            " proxy_id," \
            " core_net_id," \
            " err_code)" \
            " VALUES ";

        int recordSize = records.size();
        for (int i = 0; i < recordSize; i++)
        {
            VoipApp::SipCallRecord record = records.at(i);

            statement += "(";
            statement += std::to_string(record.m_st) + ",";
            statement += std::to_string(record.m_et) + ",";
            statement += "\"" +  record.m_fromUser + "\"" + ",";
            statement += "\"" +  record.m_toUser + "\"" + ",";
            statement += "\"" +  record.m_toPhone + "\"" + ",";
            statement += "\"" +  record.m_rpcCallId + "\"" + ",";
            statement += "\"" +  record.m_sipCallId + "\"" + ",";
            statement += std::to_string(record.m_td) + ",";
            statement += std::to_string(record.m_type) + ",";
            statement += std::to_string(record.m_direction) + ",";
            statement += "\"" +  record.m_confUri + "\"" + ",";
            statement += "\"" +  record.m_confMid + "\"" + ",";
            statement += std::to_string(record.m_cmt) + ",";
            statement += "\"" +  record.m_sipGatewayId + "\"" + ",";
            statement += "\"" +  record.m_sipProxyId + "\"" + ",";
            statement += "\"" +  record.m_coreNetId + "\"" + ",";
            statement += std::to_string(record.m_errCode);
            statement += ")";

            if (i != (recordSize - 1))
            {
                statement += ",";
            }
        }

        statement += " ON DUPLICATE KEY UPDATE err_code=VALUES(err_code);";

        if (!tryExecuteInsert(conn, newTableName, SipProxyDbCommon::SipCallStatistic, statement, mysqlErrno, reason))
            break;

    } while (0);

    if (!reason.empty())
    {
        TLOGERROR("[SipProxyDb]  insertSipRecordsToDb() failed. reason:" << reason << endl);
        conn->rollback();
        conn->autoCommit(true);
        return false;
    }

    conn->commit();
    conn->autoCommit(true);
    return true;
}

bool SipDbImp::insertCallRecordsToDb(const DbConn::SQL::ConnPtr& conn, const std::string& tableName, const std::string& domainId, const std::string& appId,
    const std::vector<VoipApp::CallRecord>& records, unsigned int& mysqlErrno, std::string& reason)
{
    mysqlErrno = 0;
    reason.clear();
    if (records.size() == 0)
        return true;

    if (0 == conn)
    {
        reason = "0 == conn";
        return false;
    }

    bool r = conn->autoCommit(false);
    if(!r)
    {
        reason = conn->getLastReason();
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    std::string statement = "(st," \
        " eat," \
        " rt," \
        " at," \
        " et," \
        " td," \
        " rpc_call_id," \
        " rtc_caller," \
        " rtc_callee," \
        " rtc_user_id," \
        " rtc_cc," \
        " rtc_ip," \
        " rtc_display," \
        " cmt," \
        " direction," \
        " conf_uri," \
        " conf_mid," \
        " caller," \
        " callee," \
        " caller_prefix," \
        " callee_prefix," \
        " sip_call_id," \
        " core_net_id," \
        " sbc_ip," \
        " sbc_port," \
        " sbc_media_ip," \
        " charge_number," \
        " release_type," \
        " sip_gateway_id," \
        " proxy_id," \
        " sip_error_code," \
        " sip_error_desc," \
        " sip_error_cause," \
        " media_error_code," \
        " media_error_desc," \
        " other_error_code," \
        " other_error_desc," \
        " params_ext)" \
        " VALUES ";

    int recordSize = records.size();
    for (int i = 0; i < recordSize; i++)
    {
        VoipApp::CallRecord record = records.at(i);

        statement += "(";
        statement += std::to_string(record.m_st) + ",";
        statement += std::to_string(record.m_est) + ",";
        statement += std::to_string(record.m_rt) + ",";
        statement += std::to_string(record.m_at) + ",";
        statement += std::to_string(record.m_et) + ",";
        statement += std::to_string(record.m_td) + ",";
        statement += "\"" +  record.m_rpcCallId + "\"" + ",";
        statement += "\"" + record.m_rtcCaller + "\"" + ",";
        statement += "\"" + record.m_rtcCallee + "\"" + ",";
        statement += "\"" + record.m_rtcUserId + "\"" + ",";
        statement += "\"" + record.m_rtcCc + "\"" + ",";
        statement += "\"" + record.m_rtcIp + "\"" + ",";
        statement += "\"" + record.m_rtcDisplay + "\"" + ",";
        statement += std::to_string(record.m_cmt) + ",";
        statement += std::to_string(record.m_direction) + ",";
        statement += "\"" + record.m_confUri + "\"" + ",";
        statement += "\"" + record.m_confMid + "\"" + ",";
        statement += "\"" + record.m_caller + "\"" + ",";
        statement += "\"" + record.m_callee + "\"" + ",";
        statement += "\"" + record.m_callerPrefix + "\"" + ",";
        statement += "\"" + record.m_calleePrefix + "\"" + ",";
        statement += "\"" + record.m_sipCallId + "\"" + ",";
        statement += "\"" + record.m_coreNetId + "\"" + ",";
        statement += "\"" + record.m_sbcIp + "\"" + ",";
        statement += std::to_string(record.m_sbcPort) + ",";
        statement += "\"" + record.m_sbcMediaIp + "\"" + ",";
        statement += "\"" + record.m_chargeNumber + "\"" + ",";
        statement += std::to_string(record.m_releaseType) + ",";
        statement += "\"" + record.m_sipGatewayId + "\"" + ",";
        statement += "\"" + record.m_sipProxyId + "\"" + ",";
        statement += std::to_string(record.m_errCode) + ",";
        statement += "\"" + record.m_errDesc + "\"" + ",";
        statement += std::to_string(record.m_errCauseCode) + ",";
        statement += std::to_string(record.m_mediaErrCode) + ",";
        statement += "\"" + record.m_mediaErrDesc + "\"" + ",";
        statement += std::to_string(record.m_otherErrCode) + ",";
        statement += "\"" + record.m_otherErrDesc + "\"" + ",";
        statement += "\"" + record.m_paramsExt + "\"";
        statement += ")";

        if (i != (recordSize - 1))
        {
            statement += ",";
        }
    }

    statement += " ON DUPLICATE KEY UPDATE sip_error_code=VALUES(sip_error_code), sip_error_desc=VALUES(sip_error_desc);";

    std::string mysqlCmd;
    do
    {
        std::string newTableName = tableName + "_" + m_date;
        mysqlCmd = "INSERT INTO " + newTableName + statement;
        if (!conn->execute(mysqlCmd))
        {
            mysqlErrno = (int)conn->getLastReasonCode();
            if(mysqlErrno == 1146)
            {
                if(CreateTable(SipProxyDbCommon::SipTableCallRecord, reason, newTableName))
                {
                    mysqlErrno = 0;

                    if(!conn->execute(mysqlCmd))
                    {
                        reason = "create new table success, but execute failed reason:" + reason;
                        break;
                    }
                }
                else
                {
                    reason = "create new table failed:" + reason;
                    break;
                }
            }
            else
            {
                reason = std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
                break;
            }
        }
    } while (0);

    if (!reason.empty())
    {
        TLOGERROR("[SipProxyDb]  insertCallRecordsToDb() failed. mysqlCmd:" << mysqlCmd << ", reason:" << reason << endl);
        conn->rollback();
        conn->autoCommit(true);
        return false;
    }

    conn->commit();
    conn->autoCommit(true);
    return true;
}

std::string SipDbImp::getRecordTableName(const std::string& domainId,const std::string& appId)
{
    std::string tableName = "t_" + SipProxyDbCommon::SIP_TABLE_CALL_STATISTIC_2 + "_" + domainId + "_" + appId;
    return tableName;
}

bool SipDbImp::UpdateNumRoutingConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::NumRouting>& opSequence, int type,
    const int64_t & insert_time, unsigned int & mysqlErrno, std::string & reason)
{
    mysqlErrno = 0;
    reason.clear();

    if(conn == 0)
    {
        reason = "INPUT parameter of 'conn' is NULL";
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    if(opSequence.empty())
    {
        return true;
    }

    bool r = conn->autoCommit(false);
    if(!r)
    {
        reason = conn->getLastReason();
        TLOGWARN("[SipProxyDb] " << __FUNCTION__ << ":" << reason << endl);
        return false;
    }

    int64_t startTime = TNOWMS;

    std::string statName;
    DbConn::SQL::StmtPtr stmt = 0;
    std::string statement;

    bool bAllOperationOK = true;
    std::vector<VoipApp::NumRouting>::const_iterator it = opSequence.begin();
    for(unsigned int i = 0; i < opSequence.size(); i++)
    {
        VoipApp::NumRouting numRouting = opSequence.at(i);

        std::string statName = "UpdateNumRoutingConfigToDb_" + numRouting.m_domainIdAppId + numRouting.m_callee;

        if(type == VoipApp::kOperationCategoryAdd)
        {
            statement = "INSERT INTO ";
            statement += tableName;
            statement += " (domain_id_app_id, callee_number, display_number, core_net_id) VALUES (";
            statement += "\'" + numRouting.m_domainIdAppId + "\',";
            statement += "\'" + numRouting.m_callee + "\',";
            statement += "\'" + numRouting.m_displayName + "\',";
            statement += "\'" + numRouting.m_coreNetId + "\',";
            statement +=  ") ON DUPLICATE KEY UPDATE domain_id_app_id=VALUES(domain_id_app_id),callee_number=VALUES(callee_number),display_number=VALUES(display_number), core_net_id=VALUES(core_net_id);";
        }
        else if(type == VoipApp::kOperationCategoryModify)
        {
            statement = "UPDATE ";
            statement += tableName;
            statement += " SET display_number='" + numRouting.m_displayName + "',";
            statement += " core_net_id=" + numRouting.m_coreNetId + ",";
            statement += " WHERE domain_id_app_id=\'" + numRouting.m_domainIdAppId + "\' AND callee_number=\'" + numRouting.m_callee + "\';";
        }
        else if(type == VoipApp::kOperationCategoryRemove)
        {
            statement = "DELETE FROM ";
            statement += tableName;
            statement += " WHERE domain_id_app_id=\'" + numRouting.m_domainIdAppId + "\' AND callee_number=\'" + numRouting.m_callee + "\';";
        }

        bool executeResult = conn->execute(statement);
        if(!executeResult)
        {
            mysqlErrno = (int)conn->getLastReasonCode();
            reason = std::to_string((int)conn->getLastReasonCode()) + ":" + conn->getLastReason();
            TLOGWARN("[SipProxyDb] SQL execute failed, errorno=" << (int)mysqlErrno << ", reason=" << reason << ", statement=" << statement << endl);
            bAllOperationOK = false;
            break;
        }
    }

    int64_t endTime = TNOWMS;
    updateCallTime("UpdateNumRoutingConfigToDb", endTime - startTime);

    if(!bAllOperationOK)
    {
        conn->rollback();
        conn->autoCommit(true);
        return false;
    }

    conn->commit();
    conn->autoCommit(true);
    return true;
}

bool SipDbImp::decodeDomainIdAppId(const std::string & domainIdAndAppId, std::string & domainId, std::string & appId)
{
    int  pos = domainIdAndAppId.find(":");
    if (pos < 0)
        return false;
    domainId = domainIdAndAppId.substr(0,pos);
    appId = domainIdAndAppId.substr(pos + 1, domainIdAndAppId.size() - (pos + 1));
    return true;
}


