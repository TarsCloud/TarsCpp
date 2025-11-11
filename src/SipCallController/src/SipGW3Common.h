#ifndef __SIP_GW_Common_h__
#define __SIP_GW_Common_h__

#include <string>
#include "servant/Application.h"

namespace SipCloudGateway
{
    enum SipSendDirection
    {
        kSipSendToCloud = 0,
        kSipSendToGw2
    };

    enum JegoServiceType
    {
        EN_ENTERPRISE_OUTGOING = 1,             //SIP-GW3企业呼出
        EN_ENTERPRISE_INCOMMING,                //SIP-GW3企业呼入
        EN_ENTERPRISE_TO_ENTERPRISE,            //SIP-GW3 to SIP-GW3
        EN_APP_OUTGOING,                        //APP侧基本呼出
        EN_APP_INCOMMING,                       //APP侧基本呼入
        EN_ENTERPRISE_MIDDLE_CALL_FORWARDING,   //SIP-GW2企业强制呼转（中间号)
        EN_MGCF_CALL_FORWARDING,                //SIP-GW2个人强制呼转
        EN_ENTERPRISE_CALL_FORWARDING,          //SIP-GW3企业呼入&呼转接听
        EN_APP_CALL_FORWARDING                  //无忧行APP呼转接听模型
    };

    enum CallReleaseType
    {
        EN_CALL_RELEASE_BY_CALLER = 1,
        EN_CALL_RELEASE_BY_CALLEE,
        EN_CALL_RELEASE_BY_SERVER,
        EN_CALL_RELEASE_BY_BLACK_LIST_GLOBAL,
        EN_CALL_RELEASE_BY_BLACK_LIST_USER,
        EN_CALL_RELEASE_BY_OCS_BLACK_LIST,

        EN_CALL_RELEASE_BY_JEGO,
        EN_CALL_RELEASE_BY_OCS_OTHER_ERROR,
        EN_CALL_RELEASE_BY_OCS_TIME_LIMITED,
        EN_CALL_RELEASE_BY_OCS_SIM_ABNORMAL,
        EN_CALL_RELEASE_BY_OCS_UPDATE_TIME_OUT,
        EN_CALL_RELEASE_BY_SERVER_NO_MORE_TIME,
    };

    enum MediaNegotiationMethod
    {
        EN_MEDIA_NEGOTIATION_METHOD_INVITE = 0,
        EN_MEDIA_NEGOTIATION_METHOD_REINVITE,
        EN_MEDIA_NEGOTIATION_METHOD_ACK,
        EN_MEDIA_NEGOTIATION_METHOD_RING = 180,
        EN_MEDIA_NEGOTIATION_METHOD_EARLYSESSION = 183,
        EN_MEDIA_NEGOTIATION_METHOD_INVITE_OK = 200
    };

    static const std::string SIP_GW3_SERVICE_NAME = "CloudSipGateway";
    static const std::string SIP_GW_SERVICE_NAME_CLIENT = "#CloudSipGateway";
    static std::string strModName("CloudSipGateway");
    static char *  MOD_NAME = (char*)strModName.c_str();

#define SIPGW_LOG_DBG(log) TLOGDEBUG(log << endl)
#define SIPGW_LOG_WRN(log) TLOGWARN(log << endl)
#define SIPGW_LOG_IFO(log) TLOGINFO(log << endl)
#define SIPGW_LOG_ERR(log) TLOGERROR(log << endl)

#define SIPGW_LOGFMT_DBG(fmt,...) TLOGDEBUG(fmt << endl)
#define SIPGW_LOGFMT_WRN(fmt,...) TLOGWARN(fmt << endl)
#define SIPGW_LOGFMT_IFO(fmt,...) TLOGINFO(fmt << endl)
#define SIPGW_LOGFMT_ERR(fmt,...) TLOGERROR(fmt << endl)

    static const std::string ENUM_PROXY_REGISTER_TYPE = "ENUMProxyRegId";
    static const std::string ATS_REGISTER_TYPE = "AtsRegId";
    static const std::string SIP_GATEWAY_ENTRY_SERVIC_NAME = "SipGatewayEntry";

    static const std::string SIP_GW_TO_CLOUD_CCONNECTION_KEY = "SipDialogId_To_Cloud";
    static const std::string SIP_GW3_REGISTER_TYPE = "CloudSipGatewayRegId";

    static const std::string SIP_GW_START_UP_PARAMS_TCP_USED = "Cloud.TCPUsed";
    static const std::string SIP_GW_START_UP_PARAMS_LISTEN_PORT = "Cloud.ListenPort";
    static const std::string SIP_GW_START_UP_PARAMS_LISTEN_IP = "Cloud.ListenIp";
    static const std::string SIP_GW_START_UP_PARAMS_SEND_PORT = "Cloud.SendPort";
    static const std::string SIP_GW_START_UP_PARAMS_CORE_DOMAIN_ID = "Cloud.DomainId";
    static const std::string SIP_GW_START_UP_PARAMS_CALL_RECORD_EXPIRED_SECS = "Cloud.CallRecordExpiredSecs";
    static const std::string SIP_GW_START_UP_PARAMS_CORE_NET_ID = "Cloud.CoreNetId";
    static const std::string SIP_GW_START_UP_PARAMS_USE_PUBLIC_IP = "Cloud.UsePublicIp";
    static const std::string SIP_GW_START_UP_PARAMS_FILTER_CALLEE_PREFIX = "Cloud.FilterCalleePrefix";
    static const std::string SIP_GW_START_UP_PARAMS_SIP_ACCOUNT_READ_INTERVAL = "Cloud.SipAcReadInterval";        //minutes
    static const std::string SIP_GW_START_UP_PARAMS_OCS_CHARGE = "Cloud.OcsCharge";              //ocs 计费标识  0计费
    static const std::string SIP_GW_START_UP_PARAMS_CLEAN_DIALOG = "Cloud.CleanDialog";
    static const std::string SIP_GW_START_UP_PARAMS_HOST_NAME = "Cloud.HostName";
    static const std::string SIP_GW_START_UP_PARAMS_ENTERPRISE_IP = "Cloud.EntIp";             //企业ip
    static const std::string SIP_GW_START_UP_PARAMS_SIPGATEWAY1_IP = "Cloud.SipGwIp";            //sip-gw1 ip
    static const std::string SIP_GW_START_UP_PARAMS_SIP_DESTINATION_IP = "Cloud.SipDstIp";
    static const std::string SIP_GW_START_UP_PARAMS_DEFAULT_CALLER_NUMBER = "Cloud.DefaultCallerNumber";
    static const std::string SIP_GW_START_UP_PARAMS_AUTH_INFO = "Cloud.AuthInfo";
    static const std::string SIP_GW_START_UP_PARAMS_ENTERPRISE_PREFIX = "Cloud.EnterprisePrefix";
    static const std::string SIP_GW_START_UP_PARAMS_EXTRA_DISPLAY_NUMBER = "Cloud.ExtraDispNumber";
    static const std::string SIP_GW_START_UP_PARAMS_TIMER_INTERVAL = "Cloud.TimerInterval";
    static const std::string SIP_GW_START_UP_PARAMS_ALLOC_RTP_BY_DC = "Cloud.AllocRtpByDc";
    static const std::string SIP_GW_START_UP_PARAMS_RECORD_NUMBERS = "Cloud.RecordNumbers";
    static const std::string SIP_GW_START_UP_PARAMS_RECORD_ENTERPRISE_IP = "Cloud.RecordIps";
    static const std::string SIP_GW_START_UP_PARAMS_QOS_ENTERPRISE_IP = "Cloud.UseQosIps";
    static const std::string SIP_GW_START_UP_PARAMS_RTP_REGION = "Cloud.RtpRegion";

    static const int MAX_SEND_PORT_RETRY_TIMES = 30;
    static const std::string SIP_GW_LINUX_PING = "\r\n\r\n";
    static const std::string SIP_GW_LINUX_PONG = "\r\n";

    static const std::string SIP_GW_REF_FILE_SIP_CALL_POLICY = "SipCallPolicy.cfg";
    static const std::string SIP_GW_REF_FILE_SIP_NATION_REGION = "SipNationRegion.cfg";

    static const std::string SIP_GW_RPC_PARAMS_INTERFACE_IS_NEW = "NewRpc";
    static const std::string SIP_GW_RPC_PARAMS_JEGO_FROM = "From";
    static const std::string SIP_GW_RPC_PARAMS_JEGO_TO = "To";
    static const std::string SIP_GW_RPC_PARAMS_JEGO_NUMBER_TYPE_FLAG = "NumberTypeFlag";

    static const std::string SIP_GW_RPC_PARAMS_IS_PRIVATE_IP = "IsPrivateIp";
    static const std::string SIP_GW_RPC_PARAMS_USE_QOS = "Qos";
    static const std::string SIP_GW_RPC_PARAMS_MEDIA_NEGO_FINISHED = "IsNegoFinished";
    static const std::string SIP_GW_RPC_PARAMS_CALL_RECORD = "callRecord";

    static const std::string SIP_ACCOUNT_CHARGE_KEY_BLACKLIST_TYPE_USER = "user";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_BLACKLIST_TYPE_GLOBAL = "global";

    static const int OCS_ERROR_TIME_LIMITED = 4012;
    static const int OCS_ERROR_SIM_ABNORMAL = 3007;
    static const int OCS_ERROR_BLACK_LIST = 3006;

    static const int MAX_RESEND_TIMES = 3;
    static const int MAX_RESEND_TOTOL_TIMES = 6;
    static const int RESEND_TIMES_INTERVAL = 2;

};

#endif //__SIP_GW_Common_h__