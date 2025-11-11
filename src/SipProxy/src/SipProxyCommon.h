
#ifndef __SipProxyCommon_h
#define __SipProxyCommon_h

#include <string>

namespace SipProxy
{

    static const int PROXY_PERIOD_FROM_DB = 60 * 1000;                          //proxy period from mysql db 

    static const bool SIP_PX_REG_WHEN_LOGINED = true;                           //SipProxy  标示是否登录后才代理注册
    static const bool SIP_PX_FORBIT_FAILED_RE_REG = true;                       //ForbitFailedReReg
    static const int SIP_PX_MEX_FAILED_RE_REG_TIMES = 5000;                     //最多代理注册失败次数
    static const int SIP_PX_ALLOW_RE_REG_INTERVAL =  1;                         //是否设置代理注册失败后再次代理注册间隔
    static const int SIP_PX_RE_REG_INTERVARL = 2;                               //代理注册失败间隔

    static const int SIP_REGISTER_PERIOD = 3600;                                //SIP注册有效期
    static const int SIP_REGISTER_FAILED_PERIOD = 10;                           //超过失败次数后的注册间隔 （分钟）

    //定义超时时间
    static const int MAX_REGISTER_REPEAT_TIME = 10 * 1000;                      //重发时间间隔
    static const int MAX_REGISTER_REPEAT_TIMES = 4;                             //最大重发次数

    // 1 min * 6; 5 min * 10; 1 h
    static const int REGISTER_RETRY_INTERVAL_TIMES_5 = 5;
    static const int REGISTER_RETRY_INTERVAL_LESS_THAN_5 = 60 * 1000;
    static const int REGISTER_RETRY_INTERVAL_TIMES_10 = 10;
    static const int REGISTER_RETRY_INTERVAL_LESS_THAN_10 = 5 * 60 * 1000;
    static const int REGISTER_RETRY_INTERVAL_MORE_THAN_10 = 60 * 60 * 1000;

    static const int MAX_SIP_URI_ACHIEVE_PAGE_SIZE = 1000;
    static const int MAX_SIP_INFO_ACHIEVE_PAGE_SIZE = 1000;
    static const int CHASH_LIST_REPLICES = 150;
    static const int64_t SIP_URI_CHASH_LIST_EXPIRED_TIME = 12 * 60 * 60 * 1000;
    static const int MAX_SEND_PORT_RETRY_TIMES = 30;
    static const int MAX_QUERY_TIMES_PER_PERIOD = 20;

    static const int PROXY_SMS_MAX_LENGTH = 10;

    static const std::string PROXY_REGISTER_TYPE = "SipProxyRegId";
    static const std::string GATEWAY_ENTRY_REGISTER_TYPE = "SipGwEntryRegId";
    static const std::string PROXY_APP_CALLEE_PREFIX = "calleePrefix";
    static const std::string SIP_PROXY_TO_SBC_CCONNECTION_KEY = "SipDialogId_To_SBC";

    static const std::string SIP_PROXY_SERVIC_NAME = "SipProxy";
    static const std::string SIP_PROXY_REASON_SIP_REFRESH_TOO_MUCH = "freshed too frequently.";
    static const std::string SIP_PROXY_REASON_NO_CHASH_LIST = "CHash list not exist";
    static const std::string SIP_PROXY_NOTIFY_GENERAL_PARAM_PREFIX = "NotifyPrefix_";
    static const std::string SIP_PROXY_LINUX_PING = "\r\n\r\n";
    static const std::string SIP_PROXY_LINUX_PONG = "\r\n";
    static const std::string SIP_PROXY_SIP_ACCOUNT_TABLE_PREFIX = "account_";
    static const std::string SIP_PROXY_SIP_REF_INFO_TABLE_PREFIX = "reginfo_";
    static const std::string SIP_PROXY_SIP_REF_SUBS_TABLE_PREFIX = "subinfo_";
    static const std::string DOUBLE_CRLF = "\r\n\r\n";
    static const std::string DOUBLE_CRLF_Linux = "\n\n";

    static const std::string SIP_PROXY_PARAMS_MAP_KEY_CORENET_ID = "CoreNetId";
    static const std::string SIP_PROXY_PARAMS_MAP_KEY_DOMAIN_ID = "DomainId";
    static const std::string SIP_PROXY_PARAMS_MAP_KEY_APP_ID = "AppId";
    static const std::string SIP_PROXY_PARAMS_MAP_KEY_RELATION_TYPE = "RelationType";
    static const std::string SIP_PROXY_PARAMS_MAP_KEY_APP_KEY = "AppKey";
    static const std::string SIP_PROXY_PARAMS_MAP_KEY_DELETE_ACCOUNTS = "DeleteAccounts";
    static const std::string SIP_PROXY_PARAMS_MAP_STATUS = "Status";
    static const std::string SIP_PROXY_PARAMS_MAP_NEW_SIP_IMPORT_INTERFACE = "NewSipImport";

    static const std::string SIP_PROXY_CALL_PARAMS_INNER_CALL = "ServerInnerCall";
    static const std::string SIP_PROXY_DOMAIN_NAME_POSTFIX = ".cloud.justalk.com";

    static const std::string SIP_PROXY_START_UP_PARAMS_DEBUG_STATIS = "DebugStatistic";
    static const std::string SIP_PROXY_START_UP_PARAMS_TCP_USED = "TCPUsed";
    static const std::string SIP_PROXY_START_UP_PARAMS_TCP_PROBE_INTERVAL = "TCPProbeInterval";
    static const std::string SIP_PROXY_START_UP_PARAMS_SEND_PORT_AUTO_INCREASE = "SendPortChangeType";
    static const std::string SIP_PROXY_START_UP_PARAMS_DST_REQ_IP_PORT = "DestReqIpPort";
    static const std::string SIP_PROXY_START_UP_PARAMS_DST_RESP_IP_PORT = "DestRespIpPort";
    static const std::string SIP_PROXY_START_UP_PARAMS_NEED_NOT_REG = "NeedNotReg";
    static const std::string SIP_PROXY_START_UP_PARAMS_SIP_REGISTER_INTERVAL = "SipRegInterval";
    static const std::string SIP_PROXY_START_UP_PARAMS_KICK_OFF_APP = "KickOffApp";
    static const std::string SIP_PROXY_START_UP_PARAMS_SEND_ON_LINE_MSG = "SendOnLineMsg";
    static const std::string SIP_PROXY_START_UP_PARAMS_CHECK_IP_WHITELIST = "IPWhitelistOpen";
    static const std::string SIP_PROXY_START_UP_PARAMS_ALIVE_CHECK_TIMES = "AliveCheckTimes";
    static const std::string SIP_PROXY_START_UP_PARAMS_PEPAIR_READ_INTERVAL = "RepairReadInterval";  //hour
    static const std::string SIP_PROXY_START_UP_PARAMS_PEPAIR_INTERVAL = "RepairRmvInterval";  //minute
    static const std::string SIP_PROXY_START_UP_PARAMS_PEPAIR_RATE = "RepairRate";  //minute
    static const std::string SIP_PROXY_START_UP_PARAMS_BATCH_SIZE = "BatchSize";
    static const std::string SIP_PROXY_START_UP_PARAMS_BATCH_SLEEP_SECS = "BatchSleepSecs";  //seconds
    static const std::string SIP_PROXY_START_UP_PARAMS_ACCOUNT_ISOLATED = "AccountIsolated";  //seconds
    static const std::string SIP_PROXY_START_UP_PARAMS_APP_EMP_REGISTER = "AppTmpRegister";  //seconds
    static const std::string SIP_PROXY_START_UP_PARAMS_APP_DOMAIN_CFG_ADD = "DomainCfgAdd";
    static const std::string SIP_PROXY_START_UP_PARAMS_SEND_PORT_LEN = "SendPortLen";
    static const std::string SIP_PROXY_START_UP_PARAMS_REG_TIME_BUFFER = "RegTimeBuffer";
    static const std::string SIP_PROXY_START_UP_PARAMS_RST_RE_REGISTER = "RstReRigster";
    static const std::string SIP_PROXY_START_UP_PARAMS_NEW_ACCOUNT_LOAD_INTERVAL = "NewAcLoadInterval";
    static const std::string SIP_PROXY_START_UP_PARAMS_HTTP_NOTIFY_CFG = "HttpNotifyCfg";
    static const std::string SIP_PROXY_START_UP_PARAMS_WRITE_REG_INFO = "WriteRegInfo";
    static const std::string SIP_PROXY_START_UP_PARAMS_JMDS_SELECT_METHOD = "JmdsSelectMethod";
    static const std::string SIP_PROXY_START_UP_PARAMS_IVVR = "Ivvr";
    static const std::string SIP_PROXY_START_UP_PARAMS_IVVR_INVOKE = "IvvrInvoke";
    static const std::string SIP_PROXY_START_UP_PARAMS_IVVR_CORENET_ID = "IvvrCoreNetId";
    static const std::string SIP_PROXY_START_UP_PARAMS_FORCE_ASBC_TO_ISBC_CFG = "Sip.ForceAsbcToIsbcCfg";
    static const std::string SIP_PROXY_START_UP_PARAMS_ALLOW_ISBC_CREATE_HASH = "Sip.AllowIsbcCreateHash";

    static const std::string SIP_PROXY_RPC_PARAMS_SIP_STATUS = "SipStatus";
    static const std::string SIP_PROXY_RPC_PARAMS_SIP_URI = "SipUri";
    static const std::string SIP_PROXY_RPC_PARAMS_CONTACT_HOST = "ContactHost";
    static const std::string SIP_PROXY_RPC_PARAMS_PCALLED_PARTY_ID = "PCalledPartyId";
    static const std::string SIP_PROXY_RPC_PARAMS_REASON_ACCOUNT_NOT_PROXYED = "NotProxyed";
    static const std::string SIP_PROXY_RPC_PARAMS_REASON_BIND_SIPACCOUNT_NOT_EXIST = "BindSipAcNotExist";

    static const std::string SIP_PROXY_ACCOUNT_DOMAIN_ID = "DomainId";
    static const std::string SIP_PROXY_ACCOUNT_DOMAIN_NAME = "DomainName";
    static const std::string SIP_PROXY_ACCOUNT_APP_ID = "AppId";
    static const std::string SIP_PROXY_ACCOUNT_CORE_NET_ID = "CoreNetId";
    static const std::string SIP_PROXY_ACCOUNT_PASSWD = "PassWd";
    static const std::string SIP_PROXY_ACCOUNT_AUTH_NAME = "AuthName";
    static const std::string SIP_PROXY_ACCOUNT_PROXY_NAME = "ProxyName";
    static const std::string SIP_PROXY_ACCOUNT_APP_ACCOUNT = "AppAccount";
    static const std::string SIP_PROXY_ACCOUNT_RELATION_TYPE = "RelationType";
    static const std::string SIP_PROXY_ACCOUNT_PROXYED = "Proxyed";
    static const std::string SIP_PROXY_ACCOUNT_LOGINED = "Logined";
    static const std::string SIP_PROXY_ACCOUNT_SBC_IP = "SbcIp";
    static const std::string SIP_PROXY_ACCOUNT_SBC_PORT = "SbcPort";
    static const std::string SIP_PROXY_ACCOUNT_REGISTAR_IP = "RegistarIp";
    static const std::string SIP_PROXY_ACCOUNT_REGISTAR_PORT = "RegistarPort";
    static const std::string SIP_PROXY_ACCOUNT_FROM_TYPE = "FromType";
    static const std::string SIP_PROXY_ACCOUNT_TO_TYPE = "ToType";
    static const std::string SIP_PROXY_ACCOUNT_PERMITED_PROXY = "PermitedToProxy";
    static const std::string SIP_PROXY_ACCOUNT_I_SBC_ACCESS = "I_SBCAccess";
    static const std::string SIP_PROXY_ACCOUNT_CHARGED = "Charged";
    static const std::string SIP_PROXY_ACCOUNT_FORCE_PERIOD_REG = "ForcePeriodReg";

    static const std::string SIP_PROXY_PARAMS_SIP_FROM = "SipFrom";
    static const std::string SIP_PROXY_PARAMS_SIP_TO = "SipTo";
    static const std::string SIP_PROXY_PARAMS_SIP_HOST = "SipHost";
    static const std::string SIP_PROXY_PARAMS_SIP_CONTENT = "SipContent";
    static const std::string SIP_PROXY_PARAMS_SIP_CALLID = "SipCallId";
    static const std::string SIP_PROXY_PARAMS_SBC_IP = "SbcIp";

    static const std::string SIP_PROXY_START_UP_PARAMS_SMS_PASSWORD = "SmsPassword";

}

#endif
