#ifndef _SipRouterImp_H_
#define _SipRouterImp_H_

#include "servant/Application.h"
#include "SipRouter.h"
#include "Common/Net.h"
#include "HttpServerSession.h"
#include "HttpClientSession.h"
#include "CallRoutingManager.h"

static const std::string str =
"{"
"\"uip_version\": 2,\r\n"
"\"uip_head\": {\r\n"
        "\"AUTH_KEY\": \"cOUlfIimjcA5SyvdSDGDPtVdzoqxn0NLGI#v4Km+7OiVXvtWD2zyazl7o2B8Q37Epq\",\r\n"
        "\"CHANNEL\": \"JUPHOON\",\r\n"
        "\"METHOD\": \"JEGO_CALL_REQ\",\r\n"
        "\"SERIAL\": 10,\r\n"
        "\"RESULT_CODE\": 1,\r\n"
        "\"RESULT_DESC\": \"OK\"\r\n"
        "},\r\n"
"\"uip_body\": {\r\n"
    "\"TYPE\": \"ANSWER\",\r\n"
    "\"TIME\": \"2018.08.24 08:04:03\",\r\n"
    "\"CALL_ID\": \"20200220132415abcd\",\r\n"
    "\"DST_ARRAY\": [{\r\n"
            "\"DST_GW\": \"10.20.1.30\",\r\n"
            "\"DST_ENT_ADDR\": \"10.20.2.39\",\r\n"
            "\"DST_ENT_PORT\": 5060\r\n"
        "},\r\n"
        "{\r\n"
            "\"DST_GW\": \"10.20.1.31\",\r\n"
            "\"DST_ENT_ADDR\": \"10.20.2.31\"\r\n"
         "}\r\n"
         "],\r\n"
    "\"SRC_CALL_USER\": \"sip:+8614712345678@kcc.cmi.chinamobile.com\",\r\n"
    "\"DST_CALL_USER\": \"tel:+8614721239876\",\r\n"
    "\"CHARGE_USER\": \"sip:+8614712345690@kcc.cmi.chinamobile.com\",\r\n"
    "\"CALLING_E164_NUMBER\": \"+8614799999999\",\r\n"
    "\"ORIG_SRC_CALL_USER\": \"sip:+8614712345678@kcc.cmi.chinamobile.com\",\r\n"
    "\"RECORD\": 1,\r\n"
    "\"BEEP\": 1001,\r\n"
    "\"ROUTE_ID\": \"CALLOUT_HK\",\r\n"
    "\"ATOM_ID\": \"MGCF_CALLOUT1\",\r\n"
    "\"PRICE_NUMBER\": \"tel:+85258051234\",\r\n"
    "\"FORCE_SIP_OUTBOUND\": 1\r\n"
    "}\r\n"
"}\r\n";

static const std::string ENUM_SIP_PROXY_SERVICE_NAME = "ENUMProxy";
static const std::string ENUM_SIP_PROXY_SERVICE_NAME_CLIENT = "#ENUMProxy";
static const std::string ENUM_PROXY_REGISTER_TYPE = "ENUMProxyRegId";

static const std::string ENUM_PROXY_START_UP_PARAMS_HTTP_REQ_ADDR = "Enum.RequestUri";
static const std::string ENUM_PROXY_START_UP_PARAMS_AUTHKEY = "Enum.AuthKey";
static const std::string ENUM_PROXY_START_UP_PARAMS_CHANNEL = "Enum.Channel";
static const std::string ENUM_PROXY_START_UP_PARAMS_TCP_USED = "Enum.TcpUsed";
static const std::string ENUM_PROXY_START_UP_PARAMS_LISTEN_IP = "Enum.ListenIp";
static const std::string ENUM_PROXY_START_UP_PARAMS_LISTEN_PORT = "Enum.ListenPort";
static const std::string ENUM_PROXY_START_UP_PARAMS_MAX_FAILED_TIME = "Enum.MaxFailedTime";
static const std::string ENUM_PROXY_START_UP_PARAMS_REQUEST_INTERVAL = "Enum.RequestInterval";
static const std::string ENUM_PROXY_START_UP_PARAMS_OFFLINE_ROUTE = "Enum.OfflineRoute";

static const std::string ENUM_PROXY_START_UP_PARAMS_MID_USER = "MidUser";
static const std::string ENUM_PROXY_START_UP_PARAMS_ORIN_SRC_USER= "OrigSrcUser";
static const std::string ENUM_PROXY_START_UP_PARAMS_ORIN_DST_USER = "OrigDstUser";
static const std::string ENUM_PROXY_START_UP_PARAMS_CHARGE_USER = "ChargeUser";
static const std::string ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_IP = "EntIp";
static const std::string ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_PORT = "EntPort";
static const std::string ENUM_PROXY_START_UP_PARAMS_LAST_GW_IP = "LatGw";
static const std::string ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_IP2 = "EntIp2";
static const std::string ENUM_PROXY_START_UP_PARAMS_ENTERPRISE_PORT2 = "EntPort2";
static const std::string ENUM_PROXY_START_UP_PARAMS_LAST_GW_IP2 = "LatGw2";
static const std::string ENUM_PROXY_START_UP_PARAMS_CALLER = "Caller";
static const std::string ENUM_PROXY_START_UP_PARAMS_CALLEE = "Callee";
static const std::string ENUM_PROXY_START_UP_PARAMS_ROUTE_ID = "RouteId";
static const std::string ENUM_PROXY_START_UP_PARAMS_ATOM_ID = "AtomId";
static const std::string ENUM_PROXY_START_UP_PARAMS_DISPLAY_NAME = "DispalyName";
static const std::string ENUM_PROXY_START_UP_PARAMS_SERVICE_TYPE = "ServiceType";
static const std::string ENUM_PROXY_START_UP_PARAMS_RECORD = "Record";
static const std::string ENUM_PROXY_START_UP_PARAMS_BEEP = "Beep";
static const std::string ENUM_PROXY_START_UP_PARAMS_RESULT = "ResultCode";
static const std::string ENUM_PROXY_START_UP_PARAMS_SIP_INBOUND = "SipInbound";
static const std::string ENUM_PROXY_START_UP_PARAMS_SIP_OUTBOUND = "SipOutbound";
static const std::string ENUM_PROXY_START_UP_PARAMS_FROM = "From";
static const std::string ENUM_PROXY_START_UP_PARAMS_TO = "To";
static const std::string ENUM_PROXY_START_UP_PARAMS_NUMBER_TYPE_FLAG = "NumberTypeFlag";
static const std::string ENUM_PROXY_START_UP_PARAMS_RESPONSE_TYPE = "ResponseType";


class SipRouterImp : public VoipApp::SipRouter,
                    virtual public Common::NetReceiver,
                    virtual public SipENUMProxy::HttpRequestReceiver,
                    virtual public SipENUMProxy::HttpRespReceiver
{
public:
    /**
     *
     */
    virtual ~SipRouterImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    bool onActivate();
    void onSchd();

    //void onUpdateConfigs();

    //HttpRequestReceiver
    virtual bool onHttpRequest(const std::string & content, const Json::Value & request, const SipENUMProxy::HttpServerSessionPtr & httpServer);
    //HttpRespReceiver
    virtual void onHttpResponse(bool ret, int64_t tid, const std::string & serialNum, const std::string & resCode, const std::string & content, const std::string & reason);

    virtual tars::Bool getRouteInfo(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getRouteInfo2(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,map<std::string, std::string> &outParams,tars::TarsCurrentPtr _current_) override;

public:
    //void registerEnumproxyToResource();

    bool sendHttpRequest(const tars::TarsCurrentPtr& current, const std::string& caller,const std::string& callee,const std::string& callId, const std::string& callForwording,
            const std::string& sipgatewayIp, const std::string& enterpriseIp, const std::map<std::string, std::string>& params);

    void updateReqFailedTime(int64_t time);
    int64_t getLastFailedTime();

    void clearExpiredHttpRequestRecords();
    void getCallRoutingSize();

    Common::NetDriverPtr                    m_driver;
    Common::NetSenderPtr                    m_Listener;
    std::string                             m_strAppName;
    std::string                             m_strMainObjId;
    bool                                    m_bMainServiceReady;
    bool                                    m_bMasterThdDown;

    //Common::ResourceServerAgent             m_resourceAgent;
    SipENUMProxy::HttpServerManagerPtr                    m_httpServerManager;
    SipENUMProxy::HttpClientManagerPtr                    m_httpClientManager;
    SipENUMProxy::HttpReqItemManager                      m_httpReqManager;
    //OfflineRoutingManager                   m_callRoutingManager;

    std::string                             m_tranProtoclName;
    std::string                             m_strListenIp;
    int                                     m_iLocalListenPort;
    std::string                             m_strLocalIp;

    int                                     m_iSerialNumber;
    std::string                             m_strHttpReqAddr;
    std::string                             m_strAuthKey;
    std::string                             m_strChannel;

    int64_t                                 m_LastSchdTimeInMs;
    int64_t                                 m_BaseSchdTimeInMs;
    int64_t                                 m_lastMinute;

    std::string                             m_strMidUser;
    std::string                             m_strOrigSrcUser;
    std::string                             m_strOrigDstUser;
    std::string                             m_strLastGwIp;
    std::string                             m_strChargeUser;
    std::string                             m_strEntIp;
    int                                     m_iEntPort;
    std::string                             m_strCaller;
    std::string                             m_strCallee;
    std::string                             m_strFrom;
    std::string                             m_strTo;
    std::string                             m_strDisplayName;
    std::string                             m_strRouteId;
    std::string                             m_strAtomId;
    int                                     m_iServiceType;
    int                                     m_iRecord;
    int                                     m_iBeep;
    int                                     m_iResultCode;
    int                                     m_iInbound;
    int                                     m_iOutbound;
    int                                     m_responseType;
    int                                     m_iNumTypeFlag;

    std::string                             m_strLastGwIp2;
    std::string                             m_strEntIp2;
    int                                     m_iEntPort2;

    tars::TC_ThreadMutex                    m_mutexLastFailedTime;
    int64_t                                 m_lastFailedTime;
    int                                     m_maxFailedTime;
    int                                     m_baseFailedTime;

    bool                                    m_bIsOfflineRoute;
};
/////////////////////////////////////////////////////
#endif
