#ifndef __SIP_GW2_Common_h__
#define __SIP_GW2_Common_h__

#include <string>
#include "servant/Application.h"

namespace SipMgcfGateway
{
    static const std::string SIP_GW2_SERVICE_NAME = "MgcfSipGateway";
    static const std::string SIP_GW2_SERVICE_NAME_CLIENT = "#MgcfSipGateway";
    static std::string strModName("MgcfSipGateway");
    static char *  MOD_NAME = (char*)strModName.c_str();

#define SIPGW_LOG_DBG(log) TLOGDEBUG("[MgcfSipGateway] " << log << endl)
#define SIPGW_LOG_WRN(log) TLOGWARN("[MgcfSipGateway] " << log << endl)
#define SIPGW_LOG_IFO(log) TLOGINFO("[MgcfSipGateway] " << log << endl)
#define SIPGW_LOG_ERR(log) TLOGERROR("[MgcfSipGateway] " << log << endl)

#define SIPGW_LOGFMT_DBG(fmt,...) TLOGDEBUG("[MgcfSipGateway] " << fmt << endl)
#define SIPGW_LOGFMT_WRN(fmt,...) TLOGWARN("[MgcfSipGateway] " << fmt << endl)
#define SIPGW_LOGFMT_IFO(fmt,...) TLOGINFO("[MgcfSipGateway] " << fmt << endl)
#define SIPGW_LOGFMT_ERR(fmt,...) TLOGERROR("[MgcfSipGateway] " << fmt << endl)

    static const std::string SIP_GW_START_UP_PARAMS_TCP_USED = "Mgcf.TCPUsed";
    static const std::string SIP_GW_START_UP_PARAMS_LISTEN_IP = "ListenIP";
    static const std::string SIP_GW_START_UP_PARAMS_LISTEN_PORT = "ListenPort";
    static const std::string SIP_GW_START_UP_PARAMS_SEND_PORT = "SendPort";
    static const std::string SIP_GW_START_UP_PARAMS_DST_RESP_IP_PORT = "RespIpPort";
    static const std::string SIP_GW_START_UP_PARAMS_DST_REQ_IP_PORT = "ReqIpPort";
    static const std::string SIP_GW_START_UP_PARAMS_MGCF_IP_CONVERGENCE = "MgcfIpConvergence";
    static const std::string SIP_GW_START_UP_PARAMS_MGCF_IP_CONVERGENCE2 = "MgcfIpConvergence2";
    static const std::string SIP_GW_START_UP_PARAMS_ENTERPRISE_IP = "EnterpriseIp";
    static const std::string SIP_GW_START_UP_PARAMS_ENTERPRISE_PREFIX = "Mgcf.EnterprisePrefix";

    static const std::string SIP_GW_TO_MGCF_CCONNECTION_KEY = "SipDialogId_To_Mgcf";
    static const std::string SIP_GW_INIT_INVITE = "initInvite";
    static const std::string SIP_GW_REGISTER_TYPE = "MgcfSipGatewayRegId";

    static const int MAX_SEND_PORT_RETRY_TIMES = 30;
    static const std::string SIP_GW_LINUX_PING = "\r\n\r\n";
    static const std::string SIP_GW_LINUX_PONG = "\r\n";

};

#endif //__SIP_GW2_Common_h__