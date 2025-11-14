#ifndef __SipGatewayCommon_h
#define __SipGatewayCommon_h

//#include "servant/Application.h"
#include <string>
#include <map>

namespace SipGateway
{
    enum SipRequestMethodType
    {
        kSipRequestMethodInvalid = -1,
        kSipRequestMethodINVITE,
        kSipRequestMethodACK,
        kSipRequestMethodBYE,
        kSipRequestMethodPRACK,
        kSipRequestMethodREGISTER,
        kSipRequestMethodSUBSCRIBE,
        kSipRequestMethodNOTIFY,
        kSipRequestMethodUPDATE,
        kSipRequestMethodMESSAGE,
        kSipRequestMethodCANCEL,
        kSipRequestMethodINFO,
        kSipRequestMethodOPTIONS
    };

    enum SipResponseCodeType
    {
        kSipResponseCodeInvalid = -1,
        kSipResponseCode100Trying,
        kSipResponseCode180Ringing,
        kSipResponseCode183SessionProgress,
        kSipResponseCode1xx,
        kSipResponseCode200OK,
        kSipResponseCode202,
        kSipResponseCode2xx,
        kSipResponseCode400,
        kSipResponseCode401,
        kSipResponseCode403,
        kSipResponseCode404,                //注册服务地址没找到
        kSipResponseCode407,                //代理服务器要求认证
        kSipResponseCode408,
        kSipResponseCode423,
        kSipResponseCode480,
        kSipResponseCode481,
        kSipResponseCode482,                //检测到环路
        kSipResponseCode486,
        kSipResponseCode487,
        kSipResponseCode488,
        kSipResponseCode491,
        kSipResponseCode500,
        kSipResponseCode4xx,
        kSipResponseCode5xx,
        kSipResponseCode604,                //Does Not Exist Anywhere "Server Internal Error"
    };

    //Sip account register type, use the logined column replace
    enum SipAccountRegisterType
    {
        kRegisterTypeInvalid = -1,
        kRegisterTypeUnReg = 0,         //not allowed to register
        kRegisterTypePerioReg = 1,      //periodically register
        kRegisterTypeMandated = 2,       //restful http, mandated but not periodically register
        kRegisterTypeUnMandated = 3,     //restful http, un-mandated
        kRegisterTypeCancelMandated = 4,  //restful http, cancel mandated periodically register
        kRegisterTypeMandatedPeriodReg = 5, //inner status, mandated periodically register
        kRegisterTypeInvokeMandatedPeriodReg = 6, //invoke periodically mandated register
    };

    enum SipCallStatusType
    {
        kSipCallStatusTypeInitiated = 0,
        kSipCallStatusTypeRinging,
        kSipCallStatusTypeInProgress,
        kSipCallStatusTypeFailed,
        kSipCallStatusTypeCompleted
    };

    enum InterceptType
    {
        kInterceptTypeNone = 0,
        kInterceptTypeBlackList,
        kInterceptTypeHarass
    };

    //话单错误原因
    enum SipCallErrorCode
    {
        kSipCallError0 = 0,                             //success
        kSipCallError100 = 100,                         //find SIP resource failed;
        kSipCallError101 = 101,                         //call JMDS create func failed;
        kSipCallError102 = 102,                         //get conference record failed;
        kSipCallError103 = 103,                         //no heart beat;
        kSipCallError104 = 104,                         //no INVITE send or recv
        kSipCallError105 = 105,                         //send message to sbc failed;
        kSipCallError106 = 106,                         //callee no response
        kSipCallError107 = 107,                         //call CallServerEx create func failed
        kSipCallError108 = 108,                         //call CallServerEx waitingAcception func failed
        kSipCallError109 = 109,                         //call CallServerEx accept func failed
        kSipCallError110 = 110,                         //dialog not exist
        kSipCallError111 = 111,                         //time out for no 180
        kSipCallError112 = 112,                         //time out for no 200
        kSipCallError113 = 113,                         //onInvited failed.
        kSipCallError114 = 114,                         //dialog time out
        kSipCallError115 = 115,                         //transfer onInvite failed
        kSipCallError116 = 116,                         //generate id failed
        kSipCallError117 = 117,                         //get callee prefix failed
        kSipCallError118 = 118,                         //alloc rtp resource failed.
        kSipCallError119 = 119,                         //repeated call
        kSipCallError120 = 120,                         //caller account format error
        kSipCallError121 = 121,                         //callee account format error
        kSipCallError122 = 122,                         //not sufficient funds
        kSipCallError123 = 123,                         //Domain not configured
        kSipCallError124 = 124,                         //not alive SipProxy
        kSipCallError125 = 125,                         //SipProxy not configured
        kSipCallError126 = 126,                         //account fee expired
        kSipCallError127 = 127,                         //interface exception
        kSipCallError128 = 128,                         //inner error
        kSipCallError129 = 129,                         //no sdp
        kSipCallError130 = 130,                         //num routing param error
        kSipCallError131 = 131,                         //call CallServerEx release func failed
        kSipCallError132 = 132,                         //callee not exist
        kSipCallError133 = 133,                         //callee busy

        //OCS
        kSipCallError3006 = 3006,                         //Account in black list
        kSipCallError3007 = 3007,                         //SIM card abnormal
        kSipCallError4012 = 4012,                         //Time limited

        //MEDIA
        kSipCallError700 = 700,                         //released by app and app no media
        kSipCallError701 = 701,                         //released by app and cs no media
        kSipCallError702 = 702,                         //released by app without calling connect
        kSipCallError703 = 703,                         //released by app in 2s
        kSipCallError704 = 704,                         //released by app and app/cs no media
        kSipCallError705 = 705,                         //released by app and app empty pkg
        kSipCallError706 = 706,                         //released by app and cs empty pkg
        kSipCallError707 = 707,                         //released by app and app/cs empty pkg

        kSipCallError710 = 710,                         //released by cs and app no media
        kSipCallError711 = 711,                         //released by cs and cs no media
        kSipCallError712 = 712,                         //released by cs without calling connect
        kSipCallError713 = 713,                         //released by cs in 2s
        kSipCallError714 = 714,                         //released by cs and app/cs no media
        kSipCallError715 = 715,                         //released by cs and app empty pkg
        kSipCallError716 = 716,                         //released by cs and cs empty pkg
        kSipCallError717 = 717,                         //released by cs and app/cs empty pkg

        kSipCallError720 = 720,                         //released by server and app no media
        kSipCallError721 = 721,                         //released by server and cs no media
        kSipCallError722 = 722,                         //released by server without calling connect
        kSipCallError724 = 724,                         //released by server and app/cs no media
        kSipCallError725 = 725,                         //released by server and app empty pkg
        kSipCallError726 = 726,                         //released by server and cs empty pkg
        kSipCallError727 = 727,                         //released by server and app/cs empty pkg

        kSipCallError800 = 800,                         //release time > accepted time
        kSipCallError801 = 801,                         //release by media channel create failed
    };

#define SIPGW_LOG_DBG(log) TLOGDEBUG("[SipGateway] " << log << endl)
#define SIPGW_LOG_WRN(log) TLOGWARN("[SipGateway] " << log << endl)
#define SIPGW_LOG_IFO(log) TLOGINFO("[SipGateway] " << log << endl)
#define SIPGW_LOG_ERR(log) TLOGERROR("[SipGateway] " << log << endl)

#define SIPGW_LOGFMT_DBG(fmt,...) TLOGDEBUG("[SipGateway] " << fmt << endl)
#define SIPGW_LOGFMT_WRN(fmt,...) TLOGWARN("[SipGateway] " << fmt << endl)
#define SIPGW_LOGFMT_IFO(fmt,...) TLOGINFO("[SipGateway] " << fmt << endl)
#define SIPGW_LOGFMT_ERR(fmt,...) TLOGERROR("[SipGateway] " << fmt << endl)

    // Sip
    static const std::string SIP_GW_NAME_YES_DOMAIN = "YesDomain";
    static const std::string SIP_GW_DEFAULT_YES_DOMAIN = "yes.my";

    static const std::string SIP_GW_NAME_YES_REALM = "Realm";
    static const std::string SIP_GW_DEFAULT_YES_REALM = "ytlcomms.net";

    static const std::string SIP_GW_SERVICE_NAME = "SipGateway";
    static const std::string SIP_GW_SERVICE_NAME_EX = "SipGatewayEx";
    static const std::string SIP_GW_SERVICE_NAME_CLIENT = "#SipGateway";
    static const std::string SIP_GW_SESSION_NAME = "<SipGateway>";
    static const std::string SIP_GW_CALL_PARAMS_OID = "object";


    //sip proxy
    static const std::string SIP_GP_SERVICE_NAME = "SipProxy";
    static const std::string RPC_OPEN_CALL_NOTIFY_SERVICE_NAME_SIPGATEWAY = "OpenSipGateway";
    static const std::string RPC_OPEN_CALL_NOTIFY_SERVICE_NAME_SIPGATEWAY_ENTRY = "OpenSipGatewayEntry";
    static const std::string SIP_GATEWAY_REGISTER_TYPE = "SipGatewayRegId";


    //CoreNet id
    static const std::string SIP_GW_CORENET_ID_CHINAMB = "ChinaMB";
    static const std::string SIP_GW_CORENET_ID_GLROY = "Glory";
    static const std::string SIP_GW_CORENET_ID_EBUPT = "ebupt";
    static const std::string SIP_GW_CORENET_ID_ZYZX = "Zyzx";            //�中移在线
    static const std::string SIP_GW_CORENET_ID_LANXIN = "LanXin";        //蓝信


    //Domain Name
    static const std::string SIP_GW_DOMAIN_NAME_CHINAMB = "justalk.com";
    static const std::string SIP_GW_DOMAIN_NAME_GLORY = "justalk.com";
    static const std::string SIP_GW_DOMAIN_NAME_EBUPT = "sample.cloud.justalk.com";

    //Register Host
    static const std::string SIP_GW_REGISTER_HOST_NAME_CHINAMB = "ims.ge.chinamobile.com";
    static const std::string SIP_GW_REGISTER_HOST_NAME_GLORY = "121.40.103.170";
    static const std::string SIP_GW_REGISTER_HOST_NAME_EBUPT = "ebupt.com";

    static const std::string SIP_GW_REGISTER_HOST_TEST_13 = "192.168.2.13";
    static const std::string SIP_GW_REGISTER_HOST_TEST = "192.168.2";

    //Domain Id
    static const std::string SIP_GW_DOMAIN_ID_EBUPT = "100167";
    static const std::string SIP_GW_DOMAIN_ID_JUSTALK = "9999";
    static const std::string SIP_GW_DOMAIN_ID_TEST_13 = "1";


    //session id
    static const std::string SIP_GW_SESSION_ID_UNIFIED = "<UnifiedSession>";
    static const std::string SIP_GW_SESSION_ID_PHONE = "<phone>";
    static const std::string SIP_GW_SESSION_ID_PC = "<pc>";

    //status-session id
    static const std::string SIP_GW_STATUS_SESSION_ID_UNIFIED_ONLINE = "Online/<UnifiedSession>";
    static const std::string SIP_GW_STATUS_SESSION_ID_UNIFIED_PUSH = "Push/<UnifiedSession>";
    static const std::string SIP_GW_STATUS_SESSION_ID_PHONE_ONLINE = "Online/<phone>";
    static const std::string SIP_GW_STATUS_SESSION_ID_PHONE_PUSH = "Push/<phone>";

    //app set the status key- value
    static const std::string SIP_GW_SIP_REGISTER_KEY = "SipFlag";         // 0-un register; 1 , 2 register or re-register
    static const std::string SIP_GW_SIP_APP_PARAM_SIP_NUMBER = "SipNumber";              //+8615802586775
    static const std::string SIP_GW_SIP_APP_PARAM_SIP_PWD = "SipPwd";                    //dh43he
    static const std::string SIP_GW_SIP_APP_PARAM_AUTH_NAME_PREFIX = "AuthnamePrefix";   //如CMIJEGOM
    static const std::string SIP_GW_SIP_APP_PARAM_CHANNEL_PREFIX = "ChannelId";           //如 kcc,sjyyt
    static const std::string SIP_GW_SIP_APP_PARAM_DELETE_INT_CODE = "DeleteIntCode";     //是否去除加号
    static const std::string SIP_GW_SIP_APP_PARAM_SIP_DOMAIN = "SipDomain";
    static const std::string SIP_GW_SIP_APP_PARAM_SIP_URI = "SipUri";
    static const std::string SIP_GW_SIP_APP_PARAM_AUTH_NAME = "SipAuthname";

    //relation type
    static const std::string SIP_GW_USERE_RELATION_TYPE_PHONE = "phone";
    static const std::string SIP_GW_USERE_RELATION_TYPE_USERNAME = "username";

    static const int SIP_GW_FRESH_APP_STATUS_PERIOD = 2 * 1000;             //SipGateway定时获取app账户登陆状态间隔
    static const int SIP_GW_MAX_STATUS_ACCIEVE_SIZE = 1000;                 //每次最多查询状态数

    static const int SIP_RTPGW_MAX_PORT_ALLOC_FAILED_TIMES = 20;            //RtpGateway申请端口监听时最大尝试次数

    //release reason
    static const std::string EN_MTC_CALL_TERM_STATUS_NORMAL = "1000";            //正常挂断电话，一般情况如下： 主叫取消呼叫。 通话过程中，任何一方的点击挂掉按钮造成的挂断。
    static const std::string EN_MTC_CALL_TERM_STATUS_BUSY = "1001";              //表示您呼的对象正在通话中：假设A和B正在通话中，这个时候C呼叫A，A理应通过挂断原因EN_MTC_CALL_TERM_STATUS_BUSY 来挂断C的呼叫，此时C 将收到这个挂掉原因。
    static const std::string EN_MTC_CALL_TERM_STATUS_DECLINE = "1002";           //表示被叫拒接电话：主叫呼叫被叫，被叫通过EN_MTC_CALL_TERM_STATUS_DECLINE来挂断这次呼叫，那么主叫将收到这个挂断原因。
    static const std::string EN_MTC_CALL_TERM_STATUS_TIMEOUT = "1100";           // 表示呼叫超时：主叫呼叫被叫，被叫一直未接，或者呼叫因为某种原因未达到被叫方，主叫将收到这个挂掉原因。
    static const std::string EN_MTC_CALL_TERM_STATUS_USER_OFFLINE = "1101";      //表示被叫不在线：主叫呼叫被叫，被叫端如果没有注册PUSH参数的话，那么主叫将收到服务器的此原因的挂断。
    static const std::string EN_MTC_CALL_TERM_STATUS_NOT_FOUND = "1102";         //表示被叫是未注册帐号：主叫呼叫被叫，若主叫收到服务器此原因的挂断那么说明被叫未注册帐号。
    static const std::string EN_MTC_CALL_TERM_STATUS_CALL_EACH = "1109";         //表示同时呼叫对方，此时被叫将收到此原因的挂断，说明已经有一个呼入到达了。
    static const std::string EN_MTC_CALL_TERM_STATUS_SERVER_RELEASE = "1110";    //表示呼叫被服务器释放。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_OTHER = "1200";       //服务器异常结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_SESSION_TIMER = "1202";   //会话定时器超时结束
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_FORBIDDEN = "1203";       //禁止呼叫。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_NOT_ACCEPTED = "1204";    //表示接听失败,对方将收到此原因的挂断。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_TEMP_UNAVAIL = "1205";    // 表示被叫临时不可用，被叫收到呼叫，由于某些原因无法生成相应的对象SDK内部自动挂断，此时主叫将收到此挂断原因。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_INTERNAL = "1207";        //表示接听失败，自己收到此原因的挂断。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_PEER_FAILED = "1212";     // 对方收到服务器异常结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_NO_EPCP_PARM = "1213";    // 表示没有EP或者CP参数导致媒体层建立失败， 此时主被叫都会收到这个原因的挂断。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_GONE = "1214";            //表示清理超长通话，主被叫都会收到此原因的挂断。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_SESSION_TOO_LONG = "1215"; // 通话超过最长时间限制。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_ACCEPT = "1218";          // 接听通话服务器返回失败。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_CREATE = "1220";          // 呼出通话服务器返回失败。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_ACCOUNT_SERVER = "1300";  //表示Account 服务返回失败而导致呼叫结束，主叫将收到这个原因的挂断。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_CALL_SERVER = "1400";     //呼叫服务器错误导致通话结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_CALL_SERVER_NO_RESOURCE = "1401"; //表示呼叫服务器无法再分配可用资源导致通话结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_CALL_SERVER_INTERNAL = "1402";    // 表示呼叫服务器内部发生错误导致通话结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_CALL_SERVER_ON_INVITED = "1403";  //表示新呼叫邀请失败，导致呼叫结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_CALL_SERVER_ON_WAITING_ACCEPTION = "1404";     //表示等待被叫用户接听失败，导致呼叫结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_CALL_SERVER_ON_ACCEPTE = "1405";               //表示被叫用户刚接听电话失败，导致呼叫结束。
    static const std::string EN_MTC_CALL_TERM_STATUS_ERROR_EP_SERVER = "1500";                            //表示服务器分配EP失败导致呼叫结束。


    static const int64_t SIP_GW_MAX_SIP_CALL_RECORD_DELETE_TIME = 24 * 60 * 60 * 1000;     //sip 通话记录清除最大间隔
    static const int64_t MAX_ACCOUNT_BIND_SIPURI_EXPIRED_TIME =  2 * 60 * 60 * 1000;      //account账号与SIP账号绑定关系保存期限；
    static const int     MAX_ACCOUNT_BIND_SIPURI_COUNT = 10000;                            //最大绑定关系个数
    static const int APP_INVOKE_SIP_REG_INTERVAL_PERIOD = 40 * 1000;
    static const int APP_INVOKE_SIP_REG_INTERVAL_TMP = 1 * 1000;
    static const int64_t MAX_CALL_STATISTIC_PEORIOD = 10 * 24 * 60 * 60 * 1000;                  //振铃率统计时间区间

    static const std::string SIP_CALL_RECORD_ID_TAIL = "_tail";
    static const int         SIP_GW_IP_INSTANCE_BUFFER = 1000;                                  //ip 距离阈值(km)

    static const std::string SIP_ACCOUNT_CHARGE_KEY_REASON = "reason";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_REMAIN_MINUTE = "remainingLength";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_USERID = "userId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_TO_PHONE_NO = "toPhoneNo";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALL_DIRECTION = "callDirection";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALL_DIRECTION_INBOUND = "Inbound";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALL_DIRECTION_OUTBOUND = "Outbound";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_RELATION_ID = "relationId";

    static const std::string SIP_ACCOUNT_CHARGE_KEY_TO_START_TIME = "startTime";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_TO_END_TIME = "endTime";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_TO_CALL_ID = "callId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CORE_NET_ID = "coreNetId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_NO_MEDIA_REASON = "reason";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_RELEASED_BY_APP = "released by app";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_RELEASED_BY_SIP = "released by sip";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_APP_NO_MEDIA = "app no media";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_SIP_NO_MEDIA = "sip no media";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_APP_SIP_NO_MEDIA = "app and sip no media";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_NOT_CAPABLE_OF_DETECTION = "not capable of detection";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_RELEASED_WITHOUT_ACCEPT = "released without accept";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_APP_EMPTY_PKG = "app empty pkg";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_SIP_EMPTY_PKG = "sip empty pkg";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_APP_SIP_EMPTY_PKG = "app and sip empty pkg";

    static const std::string SIP_ACCOUNT_CHARGE_COMMAND_REMAIN_MINUTE = "getCallLength";
    static const std::string SIP_ACCOUNT_CHARGE_COMMAND_CHARGING = "consumeCallMin";
    static const std::string SIP_ACCOUNT_CHARGE_COMMAND_END_CALL = "endCall";

    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALL_ID = "callId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_TIME = "time";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_ACCEPTED_TIME = "acceptedTime";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_FROM = "from";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_TO = "to";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_DOMAIN_ID = "domainId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_APP_ID = "appId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_ERR_CODE = "errCode";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_ERR_MSG = "errMsg";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_OPT_NAME = "operatorName";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_DURATION = "duration";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALLER = "caller";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALLEE = "callee";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALL_BACK = "callback";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_REALM = "realm";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_TYPE = "type";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_BLACKLIST_TYPE = "blacklistType";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_FREQUENCY = "frequency";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_RPC_CALLID = "rpcCallId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_SIP_CALLID = "sipCallId";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALLEE_REALM = "calleeRealm";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_AUTH_NAME = "APIKey";

    static const std::string SIP_ACCOUNT_CHARGE_KEY_RELEASE_TYPE = "releaseType";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_RELEASE_BEFORE_ACCEPT = "releaseBeforeAccept";
    static const std::string SIP_ACCOUNT_CHARGE_ERROR_REASON_NO_MORE_CREDIT = "no more credit";
    static const std::string SIP_ACCOUNT_CHARGE_KEY_CALL_BACK_PREFIX = "SipCallBack";

    static const int SIP_GW_DEFAULT_ROUTER_ID = 2000;
    static const int SIP_GW_DEFAULT_CLIENT_ID = 999999;

    static const std::string SIP_GW_MAP_KEY_SIP_URI_PREFIX = "sipUri.";
    static const std::string SIP_GW_MAP_KEY_EXT_HEADER_PREFIX = "extHeader.";
    static const std::string SIP_GW_MAP_KEY_FROM_PARAM_PREFIX = "fromParam.";
    static const std::string SIP_GW_MAP_KEY_CALLER_TYPE = "callerType";
    static const std::string SIP_GW_MAP_KEY_EARLY_MEDIA_SDP_ANSWER = "SDPAnswer";
    static const std::string SIP_GW_MAP_KEY_EARLY_MEDIA_SDP_INPROGRESS = "183EarlyMedia";
    static const std::string SIP_GW_MAP_KEY_EARLY_MEDIA_SDP_RING = "180EarlyMedia";
    static const std::string SIP_GW_MAP_KEY_EARLY_MEDIA_SDP = "sdp";

    static const std::string SIP_GW_MAP_KEY_SIP_CALL_APP = "sipCallApp";
    static const std::string SIP_GW_MAP_KEY_APP_SDP_OFFER = "appSdpOffer";
    static const std::string SIP_GW_MAP_KEY_CALL_RECORD = "callRecord";
    static const std::string SIP_GW_MAP_KEY_MEDIA_STATISTIC = "mediaStatistic";

    static const std::string SIP_GW_START_UP_PARAMS_DEFAULT_CALLER = "DefaultCallerNum";
    static const std::string SIP_GW_START_UP_PARAMS_DEFAULT_CALLEE = "DefaultCalleeNum";
    static const std::string SIP_GW_START_UP_PARAMS_MAX_BIND_CAPACITY = "MaxBindCapacity";
    static const std::string SIP_GW_START_UP_PARAMS_MAX_BIND_EXPIRED_TIME = "BindExpiredTime";
    static const std::string SIP_GW_START_UP_PARAMS_ALLOW_REFRESH_APP_STATUS = "AllowedFreshStatus";
    static const std::string SIP_GW_START_UP_PARAMS_IP_DISTANCE_BUFFER = "IpDistanceBuffer";
    static const std::string SIP_GW_START_UP_PARAMS_ALLOW_UPDATE_DEMIA = "AllowedUpdateMedia";
    static const std::string SIP_GW_START_UP_PARAMS_RING_STATISTIC_PERIOD = "RingingStaPeriod";
    static const std::string SIP_GW_START_UP_PARAMS_FORCE_RESP_INVITE_CFG = "ForceRespInviteCfg";
    static const std::string SIP_GW_START_UP_PARAMS_ALLOWED_REPEART_CALL = "AllowedRepeatCall";
    static const std::string SIP_GW_START_UP_PARAMS_FORCE_NO_CHARGE_CFG = "ForceNoChargeCfg";
    static const std::string SIP_GW_START_UP_PARAMS_LOCATE_SIPPROXY = "Sip.LocateSipProxyByDC";
    static const std::string SIP_GW_START_UP_PARAMS_NUM_ROUTING_DOMAINS = "Sip.NumRoutingDomains";
    static const std::string SIP_GW_START_UP_PARAMS_DOMAIN_REALM = "Sip.DomainRealm";
    static const std::string SIP_GW_START_UP_PARAMS_FORCE_ASBC_TO_ISBC_CFG = "Sip.ForceAsbcToIsbcCfg";
    static const std::string SIP_GW_START_UP_PARAMS_INTERCEPT_CALLS = "Sip.InterceptCalls";
    static const std::string SIP_GW_START_UP_PARAMS_MEDIA_TIMEOUT = "Sip.MediaTimeOut";
    static const std::string SIP_GW_START_UP_PARAMS_CLEAN_SIP_DIALOG_TIMEOUT = "Sip.CleanSipDlgTimeOut";
    static const std::string SIP_GW_START_UP_PARAMS_AUTHNAME_AND_PWD = "Sip.AuthNamePwd";
    static const std::string SIP_GW_START_UP_PARAMS_RECORD_CALLER_NUMBERS = "Sip.RecordNumbers";
    static const std::string SIP_GW_START_UP_PARAMS_RECORD_DOMAINS = "Sip.RecordDomains";
    static const std::string SIP_GW_START_UP_PARAMS_ONLY_NEW_TABLE_RECORD = "Sip.OnlyNewTableRecord";
    static const std::string SIP_GW_START_UP_PARAMS_TIME_OUT_NO_180 = "TimeoutNo180";
    static const std::string SIP_GW_START_UP_PARAMS_TIME_OUT_NO_200 = "TimeoutNo200";
    static const std::string SIP_GW_START_UP_PARAMS_TIME_OUT_NO_CLOSED = "TimeoutNoOthers";


    static const std::string SIP_GW_CALL_REASON_NO_ALIVE_SIPPROXY = "no alive SipProxy service.";
    static const std::string SIP_GW_CALL_REASON_NO_SIP_FLAG_VALUE = "check SipFlag value failed.";
    static const std::string SIP_GW_CALL_REASON_NO_SIPGW_ID = "userData.sipGatewayId not found in params.";
    static const std::string SIP_GW_CALL_REASON_CALL_RPC_FAILED = "call rpc interface failed.";
    static const std::string SIP_GW_CALL_REASON_DECODE_SIP_MSG_FAILED = "decode sip msg failed.";
    static const std::string SIP_GW_CALL_REASON_PARAM_NOT_FOUND_PREFIX= "param not found:";

    static const std::string SIP_GW_RPC_REASON_NO_SIPFLAG = "SipFlag status not found.";
    static const std::string SIP_GW_RPC_REASON_NO_DOMAIN_CONFIG = "get domain config failed.";
    static const std::string SIP_GW_RPC_REASON_CALCULATE_SIPURI_FAILED = "calculate sipuri failed.";
    static const std::string SIP_GW_RPC_REASON_MEDIA_CHANNEL_CREATE_FAILED = "media channel create failed";

    static const std::string SIP_GW_RPC_PARAM_SIP_CALL_ID = "SipCallId";
    static const std::string SIP_GW_RPC_PARAM_CORE_NET_ID = "CoreNetId";
    static const std::string SIP_GW_RPC_PARAM_ERROR_CODE = "ErrorCode";
    static const std::string SIP_GW_RPC_PARAM_SDK_MINIMUM_VERSION = "8913232";// version > 8913231 use onReleased2_begin
    static const std::string SIP_GW_RPC_PARAM_DEVICE_INFO_CC = "DeviceInfo.CC";
    static const std::string SIP_GW_RPC_PARAM_DEVICE_INFO_SIPPROXY_ID = "DeviceInfo.SipProxyId";
    static const std::string SIP_GW_RPC_PARAM_DEVICE_INFO_CALLER_NUMBER = "DeviceInfo.CallerNumber";
    static const std::string SIP_GW_RPC_PARAM_DEVICE_INFO_CALLEE_NUMBER = "DeviceInfo.CalleeNumber";
    static const std::string SIP_GW_RPC_PARAM_RTP_GW_OID  = "rtpGateway.id";
    static const std::string SIP_GW_RPC_PARAM_RTP_SBC_IP  = "rtpGateway.sbcIp";
    static const std::string SIP_GW_RPC_PARAM_RTP_SDP_IP  = "rtpGateway.sdpIp";

    static const std::string SIP_GW_REF_FILE_SIP_CALL_CFG = "SipCallRealm.cfg";
    static const std::string SIP_GW_RPC_PARAM_SIP_TYPE = "RseEx.SIPType";
    static const std::string SIP_GW_RPC_PARAM_RSEEX_INFO = "RseEx.Info";

    static const int SIP_GW_RESEND_INVITE_TIMES = 3;


    class ErrorCodeManager
    {
    public:
        ErrorCodeManager();

        void sipErrCodeDescInit();
        void serverErrCodeDescInit();
        void mediaErrCodeDescInit();
        void otherErrCodeDescInit();
        void mediaErrCodeInit();

        std::string getErrorCodeDesc(int errCode);
        int getMediaErrorCode(int type, const std::string& reason);

    private:
        std::map<int, std::string> m_mapErrCodeDesc;

        typedef std::map<std::pair<int, std::string>, int> ErrCodeMap;
        ErrCodeMap  m_mediaErrCodeMap;
    };

};

#endif