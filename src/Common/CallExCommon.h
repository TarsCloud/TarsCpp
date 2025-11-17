#ifndef __CallServerEx_Common_h
#define __CallServerEx_Common_h

#include <string>

namespace CallEx
{
    //static const std::string RPC_CALL_SERVER_SWAP_DIALOGS_FILE	= ".cs_dialogs.swap";
    //static const std::string RPC_CALL_SERVER_SWAP_DIALOGS_FILE_POSTFIX = ".dialogs_swap.bin";

    // --------------------  CONST Var  --------------------  
    // Can be modified
    static const int            RPC_SERIAL_NUMBER_BASE              = 100;

    static const int64_t   RPC_CALLID_BROADCAST                = 0x7FFFFFFFFFFFFFFFL;
    static const int            RPC_CALL_TALKING_PATRY_LIMITED      = 2;
    static const std::string	RPC_CALL_MAGIC_USER_DATA            = "fR34-=Magic=-03hn";

    static const int            RPC_CALL_CDR_DB_ACCESS_CONTINUOUS_FAILED_TIMES_LIMITED  = 16;
    static const int            RPC_CALL_CDR_MEMORY_RECORDS_HOLDEN_NUMBER_LIMITED       = 4096;

    static const int            RPC_CALL_MCDR_DB_RETRIEVE_NUMBER_LIMITED                = 10;

    static const int            RPC_CALL_KEEPALIVE_ON_TALKING_DEATH_JUDGMENT_PERIODS    = 3;
    static const std::string RPC_CALL_KEEPALIVE_INTERVAL_KEY_IN_PARAM                = "keepAlive";
    static const std::string RPC_CALL_KEEPALIVE_POLICY_KEY_IN_PARAM                  = "alivePolicy";
    static const std::string RPC_CALL_KEEPALIVE_ACCEPT_DURATION_KEY_IN_PARAM         = "RseEx.AcceptDuration";
    static const std::string RPC_CALL_KEEPALIVE_CONN_DURATION_KEY_IN_PARAM           = "RseEx.ConnDuration";
    static const std::string RPC_CALL_SERVER_MAP_KEY_CALL_NODE_TIME                  = "RseEx.CallNodeTime";     // onReleased2/release 携带媒体协商时间字段
    static const std::string RPC_CALL_SDK_VERSION_KEY                                = "DeviceInfo.SDK_VERSION"; //终端传入SDK版本号字段
    static const int            RPC_CALL_SDK_MINIMUM_VERSION                            = 8913231; // 用于判断终端是否支持onReleased2的SDK最小版本号


    // --------------------  App Config Item Key & Default Value --------------------
    // Can be modified
    static const std::string RPC_CALL_CONFIG_ITEM_JUSTALK_DOMAINID_NAME        = "global.justalk.com.DomainId";
    static const int            RPC_CALL_CONFIG_ITEM_JUSTALK_DOMAINID_DEFAULT     = 9999;

    static const std::string RPC_CALL_CONFIG_ITEM_RESOURCE_SERVER_NAME         = "CallServer.ResourceService";
    static const std::string RPC_CALL_CONFIG_ITEM_RESOURCE_SERVER_DEFAULT      = "Resource";

    static const std::string RPC_CALL_CONFIG_ITEM_KEEPALIVE_GLOBAL_POLICY_NAME = "KeepAlive.GlobalPolicy";

    static const std::string RPC_CALL_CONFIG_ITEM_KEEPALIVE_DOMAINS_POLICY_NAME = "KeepAlive.DomainsPolicy";
    static const std::string RPC_CALL_CONFIG_ITEM_CALL_SIP_DIST_POLICY = "CallSip.RtpNearCaller";
    static const std::string RPC_CALL_CONFIG_ITEM_FORCE_END_POINT = "ForceEndPoint";
    static const std::string RPC_CALL_CONFIG_ITEM_ALLOC_EP_TIMES = "AllocEpTimes";

    // --------------------  RPC Service Name  --------------------
    // Can be modified
    static const std::string RPC_CALL_SERVICE_NAME               = "#CallServerEx";
    static const std::string RPC_CALL_CLIENT_NAME                = "CallClientEx";

    static const std::string RPC_SIPGW_SERVICE_NAME              = "SipGateway";
    static const std::string RPC_SIPGWEX_SERVICE_NAME            = "SipGatewayEx"; // QoS改造后升级需要，新版呼叫服务使用"SipGatewayEx"
    static const std::string RPC_RTPGW_SERVICE_NAME              = "RtpGateway";
    static const std::string RPC_SIPGW_SESSION_ID                = "<SipGateway>";
    static const std::string RPC_CALL_PARAMS_CALLEE_SUBSYSTEM    = "calleeSubSystem";
    static const std::string RPC_CALL_PARAMS_CALLER_SUBSYSTEM    = "callerSubSystem";
    static const std::string RPC_CALL_PARAMS_SIP_CALLER          = "sipCallCaller";

    static const std::string RPC_CALL_PARAMS_INFORM_TYPE        = "informType"; //value: 1-183EarlyMedia, 2-...
    static const std::string RPC_CALL_PARAMS_INFORM_TYPE_183EARLY_MEDIA = "183EarlyMedia";
    static const std::string RPC_CALL_PARAMS_INFORM_TYPE_100TRYING = "100Trying";

    static const std::string RPC_OPEN_CALL_SERVICE_NAME          = "OpenCallServer";
    static const std::string RPC_OPEN_CALL_NOTIFY_SERVICE_NAME   = "OpenCallUser";
    static const std::string RPC_OPEN_CALL_NOTIFY_PROXY_NAME     = "RESTfulGateway";
    static const std::string RPC_OPEN_CALL_NOTIFY_SERVER_TYPE_NAME = "CallNotifyServer";
    static const std::string RPC_OPEN_CALL_NOTIFY_THIRD_AUTH_TYPE_NAME = "CallThirdAuthServer";
    static const std::string RPC_OPEN_CALL_NOTIFY_SERVICE_NAME_SIPGATEWAY    = "OpenSipGateway";
    static const std::string RPC_CALL_CDR_DB_SERVICE_NAME        = "CdrDb";

    // -------------------- CALL STATE FOR LOGRECORD  --------------------

    enum CallState
    {
        kCallStateInvalid = -1,
        kCallStateUnringed = 0,
        kCallStateRinged = 1,
        kCallStateAccepted = 2,
        kCallStateServerError = 3,
        kCallStateClientError = 4,
        kCallStateOtherError = 5,
    };

    // --------------------  OPEN_CALL TYPE  --------------------

    enum OpenCallNotifyType
    {
        kOpenCallNotifyInvalid = -1,
        kOpenCallNotifyNonuse,
        kOpenCallNotifyStatusNotify,
        kOpenCallNotifyThirdAuth,
        kOpenCallNotifyStatusNotifyAndThirdAuth
    };

    // --------------------  User Event EventID  --------------------
    // DO NOT MODIFY
    typedef enum CallServerUpperUserEvent
    {
        kCallServerUserEventInvalid = 0,
        kCallServerUserEventIncommingInvitation,
        kCallServerUserEventServerDenyIncommingInvitation,
        kCallServerUserEventOutgoingInvitation,
        kCallServerUserEventOutgoingInvitationFailed,
        kCallServerUserEventCalleeConfirmedInvitation,
        kCallServerUserEventCalleeRinging,
        kCallServerUserEventCalleeAnswered,
        kCallServerUserEventServerDenyCalleeAnswer,
        kCallServerUserEventCallerConfirmedAnswer,
        kCallServerUserEventCalleeBusy,
        kCallServerUserEventCalleeForwardingUnconditional,
        kCallServerUserEventCalleeForwardingOnBusy,
        kCallServerUserEventCalleeForwardingOnNoReply,
        kCallServerUserEventCallReleased,
        kCallServerUserEventPartyReleased,
        kCallServerUserEventOutgoingReferCaller,
        kCallServerUserEventOutgoingReferCallerFailed
    }CallServerUserEvent;


    // --------------------  User Event MAP Key  --------------------
    // DO NOT MODIFY
    static const char * CALL_SERVER_MAP_KEY_CALL_ID                         = "CallId";
    static const char * CALL_SERVER_MAP_KEY_CALLSERVER_ID                   = "CallServerId";
    static const char * CALL_SERVER_MAP_KEY_STATISTICS_ID                   = "StatisticsId";
    static const char * CALL_SERVER_MAP_KEY_CALL_ACCESS_SERVER_CALLER_SIDE  = "CallAccessServerCallerSide";
    static const char * CALL_SERVER_MAP_KEY_CALL_ACCESS_SERVER_CALLEE_SIDE  = "CallAccessServerCalleeSide";
    static const char * CALL_SERVER_MAP_KEY_CALL_DOMAIN_ID                  = "CallDomainId";
    static const char * CALL_SERVER_MAP_KEY_CALL_APP_ID                     = "CallAppId";
    static const char * CALL_SERVER_MAP_KEY_CALL_ORIGINATE_TIMESTAMP        = "CallOrignateTime";
    static const char * CALL_SERVER_MAP_KEY_CALL_RINGING_TIMESTAMP          = "CallRingingTime";
    static const char * CALL_SERVER_MAP_KEY_CALL_ANSWER_TIMESTAMP           = "CallAnswerTime";
    static const char * CALL_SERVER_MAP_KEY_CALL_MEDIA_TYPE                 = "CallMediaType";
    static const char * CALL_SERVER_MAP_KEY_CALL_ORIGIN_TYPE                = "CallOriginType";
    static const char * CALL_SERVER_MAP_KEY_CALL_TALKING_DURATION           = "CallTalkingDuration";
    static const char * CALL_SERVER_MAP_KEY_CALL_INSERT_MCDR_WHEN_MISSED    = "InsertMcdrWhenMissedCall";
    static const char * CALL_SERVER_MAP_KEY_CALL_CALLER_HOST                = "CallCallerHost";
    static const char * CALL_SERVER_MAP_KEY_CALL_CALLEE_HOST                = "CallCalleeHost";
    static const char * CALL_SERVER_MAP_KEY_CALL_RPC_STATUS_CODE            = "CallRpcStatusCode";
    static const char * CALL_SERVER_MAP_KEY_CALL_RPC_STATUS_INFO            = "CallRpcStatusInfo";
    static const char * CALL_SERVER_MAP_KEY_CALL_KEEP_ALIVE_INFO            = "CallKeepAliveInfo";
    static const char * CALL_SERVER_MAP_KEY_CALL_KEEP_ALIVE_POLICY          = "CallKeepAlivePolicy";

    static const char * CALL_SERVER_MAP_KEY_CALL_ORIGINATE_CUSTOM_TAG       = "CallOrignateCustomTag";
    static const char * CALL_SERVER_MAP_KEY_CALL_ANSWER_CUSTOM_TAG          = "CallAnswerCustomTag";

    static const char * CALL_SERVER_MAP_KEY_CALLER_USER_ID                  = "CallerUserId";
    static const char * CALL_SERVER_MAP_KEY_CALLER_ACCOUNT_ID               = "CallerAccountId";
    static const char * CALL_SERVER_MAP_KEY_CALLER_DISPLAY_ID               = "CallerDisplayId";
    static const char * CALL_SERVER_MAP_KEY_CALLER_DISPLAY_BRIEF            = "CallerDisplayBrief";
    static const char * CALL_SERVER_MAP_KEY_CALLER_HOST_ADDRESS             = "CallerHostAddress";
    static const char * CALL_SERVER_MAP_KEY_FAKE_CALLER_NUMBER              = "FakeCallerNumber";

    static const char * CALL_SERVER_MAP_KEY_CALLEE_USER_ID                  = "CalleeUserId";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_ACCOUNT_ID               = "CalleeAccountId";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_DISPLAY_ID               = "CalleeDisplayId";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_DISPLAY_BRIEF            = "CalleeDisplayBrief";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_HOST_ADDRESS             = "CalleeHostAddress";

    static const char * CALL_SERVER_MAP_KEY_CALLER_lOGRECORD_PREFIX         = "FROM_";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_lOGRECORD_PREFIX         = "TO_";

    static const char * CALL_SERVER_MAP_KEY_CALLER_PREFERRED_ACCOUNT_ID     = "CallerPreferredAid";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_PREFERRED_ACCOUNT_ID     = "CalleePreferredAid";

    static const char * CALL_SERVER_MAP_KEY_INVITATION_ORIGINATOR_ACCOUNT_ID= "InvitationOriginatorAccountId";
    static const char * CALL_SERVER_MAP_KEY_INVITATION_ORIGINATOR_SESSION_ID= "InvitationOriginatorSessionId";
    static const char * CALL_SERVER_MAP_KEY_CALLER_MEDIA                    = "CallerMedia";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_MEDIA                    = "CalleeMedia";
    static const char * CALL_SERVER_MAP_KEY_DENY_INVITATION_BY              = "DenyInvitationBy";
    static const char * CALL_SERVER_MAP_KEY_DENY_INVITATION_REASON          = "DenyInvitationReason";
    static const char * CALL_SERVER_MAP_KEY_RELEASE_CALL_BY                 = "ReleaseCallBy";
    static const char * CALL_SERVER_MAP_KEY_RELEASE_CALL_REASON             = "ReleaseCallReason";
    static const char * CALL_SERVER_MAP_KEY_RELEASE_CALL_CODE               = "ReleaseCallCode";
    static const char * CALL_SERVER_MAP_KEY_RELEASE_PARTY_BY                = "ReleasePartyBy";
    static const char * CALL_SERVER_MAP_KEY_RELEASE_PARTY_SUFFER_ACCOUNT_ID = "ReleasePartySufferAccountId";
    static const char * CALL_SERVER_MAP_KEY_RELEASE_PARTY_SUFFER_SESSION_ID = "ReleasePartySufferSessionId";
    static const char * CALL_SERVER_MAP_KEY_RELEASE_PARTY_REASON            = "ReleasePartyReason";
    static const char * CALL_SERVER_MAP_KEY_UPDATE_SENDER                   = "UpdateSender";
    static const char * CALL_SERVER_MAP_KEY_UPDATE_RECVER                   = "UpdateRecver";

    static const char * CALL_SERVER_MAP_KEY_CLIENT_HOST = "RseEx.ClientHost";
    static const char * CALL_SERVER_MAP_KEY_CLIENT_PORT = "RseEx.ClientPort";

    //for SIP
    static const char * CALL_SERVER_MAP_KEY_RSEEX_SDP                       = "RseEx.SDP";
    static const char * CALL_SERVER_MAP_KEY_RSEEX_UPDATE_TYPE               = "RseEx.UpdateType";
    static const char * CALL_SERVER_MAP_KEY_RTPGW_SDP                       = "rtpGateway.SDP";
    static const char * CALL_SERVER_MAP_KEY_RTPGW_CP                        = "rtpGateway.cp";
    static const char * CALL_SERVER_MAP_KEY_CALLSERVER_OID                  = "callServerEx.oid";
    static const char * CALL_SERVER_MAP_KEY_SIPGW_ID                        = "userData.sipGatewayId";
    static const char * CALL_SERVER_MAP_KEY_SIPPRROXY_ID                    = "userData.sipProxyId";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_NUMBER                   = "userData.calleeNumber";
    static const char * CALL_SERVER_MAP_KEY_NOTIFY_CALLEE_NUMBER            = "Notify.CalleeNumber";

    static const char * CALL_SERVER_MAP_KEY_CALLEE_EP                       = "calleeEp";
    static const char * CALL_SERVER_MAP_KEY_CALLEE_SDP                      = "calleeSDP";
    static const char * CALL_SERVER_MAP_KEY_RTPGW_HOSTPORTS                 = "rtpGateway.hostPorts";
    static const char * CALL_SERVER_MAP_KEY_SDP_OFFER                       = "SDPOffer";

    static const char * CALL_SERVER_MAP_KEY_SDK_DIRECT_TO_SIP               = "SDK.DirectToSip";
    static const char * CALL_SERVER_MAP_KEY_SDK_ISNEW                       = "SDK.IsNew";
    static const char * CALL_SERVER_MAP_KEY_ALERT_TYPE                      = "RseEx.AlertType";
    static const char * CALL_SERVER_MAP_KEY_ALERT_RING                      = "Ring";
    static const char * CALL_SERVER_MAP_KEY_ALERT_QUEUED                    = "Queued";
    static const char * CALL_SERVER_MAP_KEY_SDP_CALLER                      = "sdp.caller";
    static const char * CALL_SERVER_MAP_KEY_SDP_CALLEE                      = "sdp.callee";

    static const char * CALL_SERVER_MAP_KEY_CP_CALLEE                       = "cp.callee";
    static const char * CALL_SERVER_MAP_KEY_CP_CALLER                       = "cp.caller";
    static const char * CALL_SERVER_MAP_KEY_EP_CALLEE                       = "ep.callee";
    static const char * CALL_SERVER_MAP_KEY_EP_CALLER                       = "ep.caller";
    static const char * CALL_SERVER_MAP_KEY_RTP_GW_OID                      = "rtpGateway.oid";
    static const char * CALL_SERVER_MAP_KEY_CALL_ID_EX                      = "callId";
    static const char * CALL_SERVER_MAP_KEY_RTP_GW_ALLOC_POLICY             = "rtpGW.policy";
    static const char * CALL_SERVER_MAP_KEY_DIALOG_TYPE                     = "dialog.type";
    static const char * CALL_SERVER_MAP_KEY_MEDIA_ENDPOINT_TRANS            = "media.endptTranfered";
    static const char * CALL_SERVER_MAP_KEY_SIP_STATUS_CODE                 = "sipStatusCode";
    static const char * CALL_SERVER_MAP_KEY_GET_PREFIX                      = "getSipPrefix";
    static const char * CALL_SERVER_MAP_KEY_OCS_ERROR_CODE                  = "ocsCode";

    static const char * CALL_SERVER_MAP_KEY_SDK_MEDIA_CONN_DURATION             = "mediaConn";  // 阿里云日志展示字段
    static const char * CALL_SERVER_MAP_KEY_SDK_CALL_IN                         = "CallIn";     // 被叫接受邀请的时间
    static const char * CALL_SERVER_MAP_KEY_SDK_CALL_Out                        = "CallOut";    // 主叫呼叫成功的时间
    static const char * CALL_SERVER_MAP_KEY_SDK_CALL_ALERT                      = "CallAlert";  // 主叫收到onWaitingAcception时间
    static const char * CALL_SERVER_MAP_KEY_SDK_CALL_CONN                       = "CallConn";   // 被叫调用accept/主叫收到onAccepted时间（媒体开始协商）
    static const char * CALL_SERVER_MAP_KEY_SDK_CALL_TALK                       = "CallTalk";   // 主/被叫媒体连通时间
    static const char * CALL_SERVER_MAP_KEY_SDK_CALL_REALSE                     = "CallRealse"; // 调用release和收到onReleased/onReleased2时间

    // --------------------  Accept carry content and endpint --------------------
    static const char * CALL_SERVER_CALL_PARAMETER_CONTENT                  = "content";
    static const char * CALL_SERVER_CALL_PARAMETER_ENDPOINT                 = "endpoint";

    // --------------------  REL reason carried in params of onReleased()  --------------------
    // DO NOT MODIFY
    // The Following String is Recognize by Client SDK. // DO NOT MODIFY
    static const std::string CALL_SERVER_TERMINATE_REASON_KEY                        = "reason";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_ONINVITED_TIMEOUT          = "Timeout:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RINGING_TIMEOUT            = "Timeout:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_ACCOUNT_NO_SESSIONS        = "No Sessions:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_ACCOUNT_NOT_EXISTS         = "Not Exists:";
    // The Above String is Recognize by Client SDK.  // DO NOT MODIFY
    // ------------------------------------------------------------------------------------------
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CALLEE_ACCOUNT_ID_INVALID  = "Callee AccountId Invalid";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CALLER_ACCOUNT_ID_INVALID  = "Caller AccountId Invalid";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CALLID_NOT_FOUND           = "CallId Not Found";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_NO_PERMIT_FOR_PARTY        = "No Permit For Requestor";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_NO_PERMIT_MORE_CALLER      = "No Permit More Caller:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_NO_PERMIT_MORE_CALLEE      = "No Permit More Callee:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_NO_PERMIT_MORE_PARTY       = "No Permit More Party:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CALL_PARTY_NOT_FOUND       = "Requestor ID Not Found";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CS_SIP_RESOURCE_ERROR      = "CallServer SIP Resource Error";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CS_RESOURCE_INSUFFICIENT   = "CallServer Resource Insufficient";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CS_INTERNAL_ERROR          = "CallServer Internal Error";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_PEER_RELEASED              = "Peer Released";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CAllSBLACKLIST             = "CallsBlackList";

    static const std::string CALL_SERVER_TERMINATE_DUE_TO_SUPER_LONG_TALKING         = "Super Long Talking Clean";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_KEEP_ALIVE_EXPIRED         = "Keep Alive Expired";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_INACTIVE_CALL              = "Inactive Call Clean";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RINGING_TIMEOUT_CLEAN      = "Ringing Time-out Clean";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_SAME_SESSION_MUTUAL        = "Same Session Mutual";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_USER_TERMINATE             = "User Terminate";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_OPENCALL_TERMINATE         = "OpenCallServer Terminate";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_THIRD_AUTH_CALL_FAILED      = "Third Auth Call Failed:";

    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CONCURRENT_CALLS_EXCEED    = "Concurrent Calls Exceed";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_SAFE_DOG_FAILED            = "Safe Dog Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_LICENSE_SERVER_FAILED      = "License Server Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_QUANTA_SERVER_FAILED       = "Quanta Server Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_DOMAIN_OR_APP_LACK         = "Domain or App Lack";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_DOMAIN_OR_APP_NOT_MATCH    = "Domain or App Not Match";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ALLOC_ENDPOINT_FAILED  = "RPC allocEndpoint() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ALLOC_CONTENT_FAILED  = "RPC allocContent() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_GET_USERID_FAILED      = "RPC getUserId() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_GET_SESSIONS_FAILED    = "RPC getSessions() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ALLOC_RTP_GW_ARC_FAILED  = "RPC allocArcAddr() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_GET_USERID_AND_SESSIONS_FAILED    = "RPC getUserIdAndSessions() Failed:";

    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_BASE_            = "RPC onInvited() Timeout ";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_PUSH_OK          = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_BASE_ + "Push OK";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_PUSH_FAILED      = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_BASE_ + "Push Failed";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_NO_PUSH_TEMPLATE = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_BASE_ + "No Push Template";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_NO_NEED_PUSH     = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_BASE_ + "No Need Push";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_NOTIFY_OK        = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_TIMEOUT_BASE_ + "Notify OK";

    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_BASE_              = "RPC onRefer() Timeout ";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_PUSH_OK            = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_BASE_ + "Push OK";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_PUSH_FAILED        = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_BASE_ + "Push Failed";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_NO_PUSH_TEMPLATE   = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_BASE_ + "No Push Template";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_NO_NEED_PUSH       = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_BASE_ + "No Need Push";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_NOTIFY_OK          = 
        CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_TIMEOUT_BASE_ + "Notify OK";

    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONWAIT_FAILED          = "RPC onWaitingAcception() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONACCEPTED_FAILED      = "RPC onAccepted() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINVITED_FAILED       = "RPC onInvited() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONREFER_FAILED         = "RPC onRefer() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONINFORMED_FAILED      = "RPC onInformed() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONRELEASED_FAILED      = "RPC onReleased() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_RPC_ONUPDATED_FAILED       = "RPC onUpdated() Failed:";
    static const std::string CALL_SERVER_TERMINATE_DUE_TO_CREATE_AGENT_FAILED        = "createAgent() Failed";



    // --------------------  Input Params Key-Name with External Module  --------------------
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_MEDIA_TYPE             = "mediaType";
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_CALLEE_DISPLAY_ID      = "calleeDisplay";
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_CALLER_DISPLAY_ID      = "callerDisplay";
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_CALLEE_DISPLAY_BRIEF   = "calleeDisplayName";
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_CALLER_DISPLAY_BRIEF   = "callerDisplayName";

    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_PREFERRED_ACCOUNT_ID   = "PreferredAid";
    static const char * CALL_SERVER_CALL_ANSWER_PARAMETER_PREFERRED_ACCOUNT_ID      = "PreferredAid";

    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_CUSTOM_TAG             = "customTag";
    static const char * CALL_SERVER_CALL_ANSWER_PARAMETER_CUSTOM_TAG                = "customTag";

    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_PROXY_DOMAINID         = "Proxy.DomainId";
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_PROXY_APPID            = "Proxy.AppId";
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_PROXY_SERVER_TYPE      = "Proxy.ServerType";
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_USER_STATISTIC_ID      = "RseEx.StatisticsId";
    static const char * CALL_SERVER_CALL_TERMINATE_PARAMETER_USER_TERMINATE_REASON  = "RseEx.Reason";
    static const char * CALL_SERVER_CALL_TERMINATE_PARAMETER_OPENCALL_TERMINATE_DESCRIPTION = "RseEx.Description";

    //if value of key has contained 'MtcCallNoLogKey' , then NOT record into MCDR
    static const char * CALL_SERVER_CALL_ORIGINATE_PARAMETER_INSERT_MCDR_WHEN_MISSED= "RseEx.UserData";

    //value of key "RseEx.PubAddr",means the IP must used when alloc resource
    static const char * CALL_SERVER_CALL_EX_PARM_KEY_PUBLICADDR_KEY                 = "RseEx.PubAddr";

    // -----------------------
    // format timeout-error:<sub reason>:xxx
    // 1)send with tcp-keepalive channel
    static const char * RPC_TIMEOUT_SUB_REASON_NOTIFY_SUCCESS                       = "notify-success";
    // 2)send with push channel, and 3rd server return success
    static const char * RPC_TIMEOUT_SUB_REASON_PUSH_SUCCESS                         = "push-success";
    // 3)send with push channel, and 3rd server return failed
    static const char * RPC_TIMEOUT_SUB_REASON_PUSH_FAILED                          = "push-failed";
    // 4)want to 3rd push, but client didnt register push parameter
    static const char * RPC_TIMEOUT_SUB_REASON_NO_PUSH_TEMPLETE                     = "no-push-templete";
    // 5)the rpc message is normal, no need to send with 3rd push
    static const char * RPC_TIMEOUT_SUB_REASON_NO_NEED_PUSH                         = "no-need-push";

    // -----------------------
    static const char * OPENCALL_NOTIFY_FAILED_REASON_DOMAIN_INVALID                = "getDomainName error";
    static const char * OPENCALL_NOTIFY_FAILED_REASON_INVALID_SERVER_URL            = "invalid-notify-url";
    static const char * OPENCALL_NOTIFY_FAILED_REASON_CAN_NOT_CONNECT_RUL           = "can-not-connect-url";
    static const char * OPENCALL_NOTIFY_FAILED_REASON_INVALID_REPLY_CONTENT         = "invalid-notify-replay-content";

    static const int CALL_SERVER_RELEASED_CALLID_CACHE_SIZE                         = 4000;
    static const std::string CALL_SERVER_REF_FILE_CALL_SIP = "CallSip.cfg";
    static const std::string CALL_SERVER_REF_FILE_CALL_SERVERS = "CallServers.cfg";


};

#endif