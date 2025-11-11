#ifndef __SipDlg_h
#define __SipDlg_h

#include <atomic>
#include "servant/Application.h"
#include "SipGateway/src/SipGatewayCommon.h"
#include "SipGateway/src/SipMsgDecodedPart.h"
#include "SipGateway/src/SipMsgCommon.h"
#include "SipGateway/src/SipMsgDecode.h"
#include "SipGateway/src/SipDialog.h"
#include "SipProxy/src/SipRegisterSubscribe.h"
#include "SipGW3Common.h"
#include "SipController.h"
#include "ENUMProxy/src/SipRouter.h"


namespace SipCloudGateway
{
    //主叫方向
    enum CallerDirection
    {
        kSipFromCloud = 0,
        kSipFromGW
    };

    //sip
    enum SipDirection
    {
        kSipCloudToCloud,
        kSipCloudToGW,
        kSipGWToCloud,
        kSipGWToGW,
    };

    //mrfp
    enum MrfpRingStatus
    {
        kMrfpRingInit,
        kMrfpRingStarting,
        kMrfpRingStop,
        kMrfpRingOther,
    };
    //enterprise type
    enum EnterpriseType
    {
        kEnterpriseMgcf = 0,
        kEnterpriseTwilio,
        kEnterpriseVanage,
        kEnterpriseOther,
    };

    //定义SDP协商过程
    enum SDPNegotiateStatus
    {
        SDPInit = 0,                                    //初始化状态   ：尚未 收/发 offer
        SDPNegotiating = 1,                             //正在协商状态 ：已经 收/发 offer
        SDPNegotiateFailed = 2,                         //协商失败
        SDPNegotiated = 3                               //协商完成     ：已经 收/发 answer
    };

    struct stRouterInfo 
    {
        std::string  gwIp;
        std::string  mgcfIp;
        tars::Int32     mgcfPort;
        std::string  connectionId;
        std::string  gwOid;
        std::string  listenIp;
        tars::Int32     listenPort;
        SipDirection    direction;
        std::string  from;
        std::string  to;
        VoipApp::JegoRouterInfo info;
    };

    //定义超时时间
    static tars::Int64 TimeOutMaxDialogActiveDuration =  12 * 60 * 60 * 1000;
    static tars::Int32          MaxReSendTimes = 8;

    //                     SipDlgId
    class SipDlgId;
    typedef std::shared_ptr<SipDlgId> SipDlgIdPtr;

    class SipDlgId
    {
    public:
        std::string              _sipCallId;
        std::string              _sipLocalTag;
        std::string              _sipRemoteTag;
        std::mutex               _mutex;                          //通用互斥锁

    private:
        SipDlgId();
        SipDlgId(const std::string & id, const std::string & ltag, const std::string & rtag);
    public:

        SipDlgId(const SipDlgIdPtr& dId) ;
        ~SipDlgId();

    public:
        static SipDlgIdPtr create(const std::string & cid, const std::string & ltag, const std::string & rtag);
        static tars::Bool isParameterValid(const std::string & cid, const std::string & ltag, const std::string & rtag);

    public:
        std::string string() const;
        static SipDlgIdPtr fromString(const std::string & str);

        tars::Bool isValid ()const;
        tars::Bool isEqual(const SipDlgIdPtr& id);
    };

    class ChargeTimerExecutor
    {
    public:
        virtual void cleanSipResourceTimeout(const std::string & sipDialogId) = 0;
    };
    typedef std::shared_ptr<ChargeTimerExecutor> ChargeTimerExecutorPtr;

    //                       SipDialog
    class SipDlg;
    typedef std::shared_ptr<SipDlg> SipDlgPtr;
    class SipDlg
    {
    public:
        //void onTimeout(const Common::TimerPtr& timer);

    private:
        std::string                  m_SipDlgId;
        ChargeTimerExecutorPtr          m_chargeTimerExecutorPtr;

    public:
        SipMsgDecodedPart               _sipMsgDecodePart;
        SipProxy::SipRegisterSubscribePtr  _sipRegisterSub;

        static std::atomic<tars::Int32>              m_totalNum;
        static std::atomic<tars::Int32>              m_totalNewNum;
        static std::atomic<tars::Int32>              m_totalDeleteNum;
        static std::atomic<tars::Int32>              m_totalExceptionNum;

        static void PirntOutSipDialogNum();
        static void autoAddExceptionNum();

    private:
        SipGateway::SipDialogState  m_sipDialogStatus;              //Sip Dialog status
        SipGateway::TimeOutType     m_timeOutType;                  //超时类型
        tars::Int64            m_createTime;                   //创建时间点
        tars::Int64            m_curStatusInitTime;            //进入当前对话状态时间点
        tars::Int64            m_answeredTime;                 //接听时间
        tars::Int64            m_enterExceptionTime;           //进入异常时间点,设置一个处理异常时间值，异常进入超过这个值则认为异常处理过程异常，强行清除SipDialog等对象、资源
        tars::Int64            m_terminateTime;                //进入结束状态时间点
        tars::Int64            m_ereaseTime;                   //清除时间点

        SipGateway::AuthType        m_authType;                     //认证类型
        SDPNegotiateStatus          m_sdpNegoStatus;                //SDP 协商状态

        std::mutex                  m_mutex;                          //通用互斥锁
        vector<SipMsgDecodePtr>      m_vectSipMsgCodec;

        tars::Bool                  m_bRefreshDialog;               //是否为刷新对话
        tars::Int64            m_lLastRefreshTime;             //上次刷新对话时间
        tars::Int32                 m_iReSendTimes;                 //重发次数
        tars::Int32                 m_iReSendTotalTimes;                 //重发次数
        tars::Int64            m_lSendTime;                    //发送时间
        SipGateway::SipRequestMethodType        m_reSendMethod;                 //重发方法

        tars::Bool                  m_bSessionFreshDialog;          //本对话对象是否作为刷新会话存在
        EN_SIP_METHOD               m_refreshMethod;
        SipGateway::MediaType       m_mediaType;

        std::string              m_originSdpOffer;
        std::string              m_sdpOffer;
        std::string              m_originSdpAnswer;
        std::string              m_sdpAnswer;
        std::string              m_strSdpIp;

        std::string              m_strDomainId;
        std::string              m_strMeetingId;
        std::string              m_strOriginalCallId;
        std::string              m_strLastGwIp;
        std::string              m_strEnterpriseIp;

        std::string              m_originDialogId;
        std::string              m_originCalleeNumber;
        tars::Int32                 m_failedTimes;
        tars::Bool                  m_bResendInvite;

        std::string              m_strSipGatewayId;         //记录sip-gw id
        std::string              m_strSipGatewayId1;         //记录主叫sip-gw id
        std::string              m_strSipGatewayId2;         //记录被叫sip-gw id
        std::string              m_strConnectionId;          //记录当前sip消息来源
        std::string              m_strConnectId1;            //记录主叫connectionId
        std::string              m_strConnectId2;            //记录被叫connectionId
        std::string              m_strCallerRouteConnectId;

        std::mutex           m_mutexMultiRoute;
        vector<stRouterInfo>        m_vectMultiRoute;
        tars::Int32                 m_iRouteIndex;

        std::string              m_strCallerOriginName;      //记录主叫原始display name
        std::string              m_strCallerDisplayName;

        std::string              m_strCalleeNumber;
        std::string              m_strModifiedCalleeNumber;  //记录修改后的被叫号码

        std::string              m_strCallerNumber;          //记录主叫号码
        std::string              m_strMiddleDisplayName;
        std::string              m_strMiddleNumber;
        std::string              m_strMiddleHost;
        tars::Int32                 m_iMiddlePort;
        tars::Int32                 m_iMiddleReqType;

        std::string              m_strCallerFromDisplayName;
        std::string              m_strCallerFromNumber;
        std::string              m_strCallerFromHost;
        tars::Int32                 m_iCallerFromPort;
        tars::Int32                 m_iCallerFromReqType;

        std::string              m_strCallerToDisplayName;
        std::string              m_strCallerToNumber;
        std::string              m_strCallerToHost;
        tars::Int32                 m_iCallerToPort;
        tars::Int32                 m_iCallerToReqType;

        std::string              m_strCalleeFromDisplayName;
        std::string              m_strCalleeFromNumber;
        std::string              m_strCalleeFromHost;
        tars::Int32                 m_iCalleeFromPort;
        tars::Int32                 m_iCalleeFromReqType;

        std::string              m_strCalleeToDisplayName;
        std::string              m_strCalleeToNumber;
        std::string              m_strCalleeToHost;
        tars::Int32                 m_iCalleeToPort;
        tars::Int32                 m_iCalleeToReqType;

        std::string              m_strCallerFromDisplayNameByJego;
        std::string              m_strCallerFromNumberByJego;
        std::string              m_strCallerFromHostByJego;
        tars::Int32                 m_iCallerFromPortByJego;
        tars::Int32                 m_iCallerFromReqTypeByJego;

        std::string              m_strCallerToDisplayNameByJego;
        std::string              m_strCallerToNumberByJego;
        std::string              m_strCallerToHostByJego;
        tars::Int32                 m_iCallerToPortByJego;
        tars::Int32                 m_iCallerToReqTypeByJego;

        std::string              m_strCalleeFromDisplayNameByJego;
        std::string              m_strCalleeFromNumberByJego;
        std::string              m_strCalleeFromHostByJego;
        tars::Int32                 m_iCalleeFromPortByJego;
        tars::Int32                 m_iCalleeFromReqTypeByJego;

        std::string              m_strCalleeToDisplayNameByJego;
        std::string              m_strCalleeToNumberByJego;
        std::string              m_strCalleeToHostByJego;
        tars::Int32                 m_iCalleeToPortByJego;
        tars::Int32                 m_iCalleeToReqTypeByJego;

        tars::Int32                 m_iCallerRefreshCseq;
        tars::Int32                 m_iCalleeRefreshCseq;

        std::string              m_rtpGwOid;                 //rtpGatewyOid
        CallerDirection             m_direction;                //每次收到sip信息修改，判断sip消息来源
        CallerDirection             m_callerDirection;          //主叫来源
        SipDirection                m_sipDirection;             //sip收发方向
        std::string              m_strFromTag;
        std::string              m_strCallerToTag;
        std::string              m_strCalleeToTag;
        std::string              m_strCallIdHost;
        std::string              m_strCallerIp;              //cloud ip
        tars::Int32                 m_iCloudPort;              //cloud port
        std::string              m_strCalleeIp;              //mgcf ip
        tars::Int32                 m_iMgcfPort;              //mgcf port
        std::string              m_strGw2Ip;                 //to gw2 ip
        tars::Int32                 m_iGw2Port;                 //to gw2 port
        std::string              m_strGw2Ip2;                //from gw2 ip
        tars::Int32                 m_iGw2Port2;                //from gw2 port
        std::string              m_strMiddleGwIp;                //from gw2 ip
        tars::Int32                 m_iMiddleGwPort;                //from gw2 port
        std::string              m_strGw3Ip;
        tars::Int32                 m_iGw3Port;

        std::string              m_strSipMsg;
        std::string              m_strMrfpMsg;
        std::string              m_strResendMsg;
        std::string              m_strResendGWOid;
        std::string              m_strResendConId;
        std::string              m_strRecvConnId;
        std::string              m_strAtsOid;                //ats oid
        std::string              m_strContactIp;              //contact ip
        std::string              m_strPAIName;
        std::string              m_strViaBranch;
        std::string              m_strCallForwarding;
        EnterpriseType              m_callerEnterpriseType;
        EnterpriseType              m_calleeEnterpriseType;

        std::string                 m_strMiddleCallNumber;          //中间号号码
        std::string                 m_strMiddleCallConnectionId;    //中间号entry
        tars::Bool                  m_bIsRecord;                    //录音
        std::string                 m_strMrfpSdpIp;                 //MRFP 媒体ip
        std::string                 m_strMrfpSdpPorts;              //MRFP 媒体ports
        std::string                 m_strMrfpSdp;
        std::string                 m_strMrfpCode;
        tars::Bool                  m_bMrfpInitPres;
        tars::Int32                 m_iBeep;
        tars::Int32                 m_iBeepErrCode;
        tars::Bool                  m_bBeepPres;
        tars::Bool                  m_bResendPres;
        MrfpRingStatus              m_mrfpRingStatus;
        tars::Int32                 m_iServiceType;
        tars::Bool                  m_bRelease;
        tars::Int32                 m_iMrfpMediaCoding;
        std::mutex                  m_mutexMediaCoding;
        vector<std::string>         m_vectMediaCoding;
        //ocs计费
        tars::Int32                 m_iSessionId;
        tars::Int32                 m_iCCId;
        tars::Int32                 m_iCCStatusType;
        tars::Int32                 m_iRealReqTime;
        tars::Int64                 m_lLastChargeTime;      //上一次计费时间
        tars::Int64                 m_lAlertingTime;        //振铃时间
        tars::Int64                 m_lAcceptedTime;        //接听时间
        tars::Int64                 m_lTermingTime;
        std::string                 m_strChargeAccount;     //计费账号
        tars::Int32                 m_iTotalTime;           //批价剩余时长
        tars::Int32                 m_iUserdTime;           //实际使用时长
        tars::Bool                  m_bStartOcsPres;        //开始计费标识
        tars::Bool                  m_bStopCharfePres;

        tars::Bool                  m_bFromRoutePres;       //主叫sip包含route标识
        tars::Bool                  m_bToRoutePres;         //被叫响应sip包含route标识

        tars::Bool                  m_bFromNATPres;         //主叫sip使用内网NAT标识
        tars::Bool                  m_bToNATPres;           //被叫响应sip使用内网NAT标识
        std::string              m_strFromNATIp;         //NAT ip
        tars::Int32                 m_iFromNATPort;         //NAT port
        std::string              m_strToNATIp;           //NAT ip
        tars::Int32                 m_iToNATPort;           //NAT port

        tars::Bool                  m_bRecordRoute;

        CallReleaseType             m_releaseType;
        //Common::TimerPtr            m_waitTimer;

        tars::Int32                 m_iOcsErrCode;

        set<std::string>         m_vectQosIps;

        std::string              m_strCmiCovsInfo;

        SipDlg(int a){}

    private:
        SipDlg();
        SipDlg(const SipDlgIdPtr & did);

    public:
        virtual ~SipDlg();
        static SipDlgPtr create(const SipDlgIdPtr & did);
        const std::string id();

        tars::Bool getPureUserAccount(const std::string & callAccount, std::string & number);

        void setSipDialogStatus(const SipGateway::SipDialogState & dialogStatus);
        SipGateway::SipDialogState getSipDialogStatus();

        void setCurStatusInitTime(const tars::Int64 & lInitTime);
        tars::Int64 getCurStatusInitTime();

        void setTimeOutType(SipGateway::TimeOutType type);
        SipGateway::TimeOutType getTimeOutType();

        tars::Int64 getEnterExceptionTime();
        tars::Int64 getTerminateTime();
        tars::Int64 getCreateTime();

        SipGateway::AuthType getAuthType();
        void setAuthType(SipGateway::AuthType type);
        SDPNegotiateStatus getSDPNegotiateStatus();
        void setSDPNegotiateStatus(SDPNegotiateStatus status);

        void setIsFreshDialog(tars::Bool isFreshDialog);
        tars::Bool getIsFreshDialog();

        void setLastRefreshTime(tars::Int64 refreshTime);
        tars::Int64 getLastFreshTime();
        void setRefreshCseq(tars::Int32 cseq, tars::Bool isCaller);
        tars::Int32 getRefreshCseq(tars::Bool isCaller);

        void RepeatTimeAutoIncrease();
        tars::Int32 getReSendTimes();
        void reSetRepeatTimes();
        tars::Int32 getReSendTotalTimes();

        void setReSendMethod(SipGateway::SipRequestMethodType reSendMethod);
        SipGateway::SipRequestMethodType getReSendMethod();

        void setSendTime();
        tars::Int64 getSendTime();

        void setResendInvite(tars::Bool resend);
        tars::Bool getResendInvite();

        void addFailedTimes();
        tars::Int32 getFailedTimes();

        void setSessionFreshDilogFlag(tars::Bool bFreshDialog);
        tars::Bool getSessionFreshDilogFlag();

        void setRefreshMethod(EN_SIP_METHOD freshMethod);
        EN_SIP_METHOD getRefreshMethod();

        void setMediaType(SipGateway::MediaType type);
        SipGateway::MediaType getMediaType();

        void setSdpOffer(const std::string & sdp);
        std::string getSdpOffer();
        void setOrginSdpOffer(const std::string & sdp);
        std::string getOrginSdpOffer();
        void setSdpAnswer(const std::string & sdp);
        std::string getSdpAnswer();
        void setOrginSdpAnswer(const std::string & sdp);
        std::string getOrginSdpAnswer();

        void setSipGatewayId(const std::string & gwId);
        void setSipGatewayId1(const std::string & gwId);
        void setSipGatewayId2(const std::string & gwId);
        void setSipGatewayConnectId1(const std::string & connectId);
        void setSipGatewayConnectId2(const std::string & connectId);

        void setCallerRouteConnectId(const std::string & connectId);
        std::string getCallerRouteConnectId();

        std::string getSipGatewayId();
        std::string getSipGatewayId1();
        std::string getSipGatewayId2();
        std::string getSipGatewayConnectId1();
        std::string getSipGatewayConnectId2();

        void setMultiRouter(const vector<stRouterInfo>& route);
        void setMultiRouter(stRouterInfo & route);
        void getMultiRouter(vector<stRouterInfo> & route);
        tars::Bool getMultiRouterByIndex(tars::Int32 index, stRouterInfo & route);

        void setMultiConnectIdIndex();
        tars::Int32 getMultiConnectIdIndex();

        void setCallerOriginName(std::string connectId);
        std::string getCallerOriginName();

        void setCallerDisplayName(std::string name);
        std::string getCallerDisplayName();

        void setCallerNumber(const std::string & number);
        std::string getCallerNumber();

        void setCalleeNumber(const std::string & number);
        std::string getCalleeNumber();

        void setMiddleCallParams(const tars::Int32 type, const std::string & displayName, const std::string & userName, const std::string & host, const tars::Int32 port);
        void getMiddleCallParams(tars::Int32 & type, std::string & displayName, std::string & userName, std::string & host, tars::Int32 & port);
        void setCallerParams(tars::Bool isFrom, const tars::Int32 type, const std::string & displayName, const std::string & userName, const std::string & host, const tars::Int32 port);
        void getCallerParams(tars::Bool isFrom, tars::Int32 & type, std::string & displayName, std::string & userName, std::string & host, tars::Int32 & port);
        void setCallerParamsByJego(tars::Bool isFrom, const tars::Int32 type, const std::string & displayName, const std::string & userName, const std::string & host, const tars::Int32 port);
        void getCallerParamsByJego(tars::Bool isFrom, tars::Int32 & type, std::string & displayName, std::string & userName, std::string & host, tars::Int32 & port);
        void updateSipUriParams(tars::Bool isFrom, const tars::Int32 type, const std::string & host, const tars::Int32 port);

        void setBranch(const std::string & branch);
        std::string getBranch();

        void setCallForwarding(const std::string & number);
        std::string getCallForwarding();
        void setMiddleCalleeNumber(const std::string & number);
        std::string getMiddleCalleeNumber();
        void setMiddleCallConnection(const std::string & connectionId);
        std::string getMiddleCallConnection();
        void setRecord(tars::Bool isRecord);
        tars::Bool getRecord();

        void setModifiedCalleeNumber(const std::string & number);
        std::string getModifiedCalleeNumber();

        void setRtpGwOid(const std::string & oid);
        std::string getRtpGwOid();

        void setDirection(const CallerDirection & direction);
        CallerDirection getDirection();

        void setSipDirection(const SipDirection & direction);
        SipDirection getSipDirection();

        void setCallerDirection(const CallerDirection & direction);
        CallerDirection getCallerDirection();

        void setFromTag(std::string tag);
        std::string getFromTag();
        void setCallerToTag(std::string tag);
        std::string getCallerToTag();
        void setCalleeToTag(std::string tag);
        std::string getCalleeToTag();

        void setCallIdHost(std::string callIdHost);
        std::string getCallIdHost();

        void setOriginCalleeNum(std::string  number);
        std::string getOriginCalleeNum();

        void setContactIp(std::string ip);
        std::string getContactIp();

        void setCallerIp(std::string ip);
        std::string getCallerIp();

        void setCallerPort(tars::Int32 port);
        tars::Int32 getCallerPort();

        void setCalleeIp(std::string ip);
        std::string getCalleeIp();

        void setCalleePort(tars::Int32 port);
        tars::Int32 getCalleePort();

        void setGw2Ip(const std::string & ip);
        std::string getGw2Ip();
        void setGw3Ip(const std::string & ip);
        std::string getGw3Ip();

        void setGw2Port(tars::Int32 port);
        tars::Int32 getGw2Port();
        void setGw3Port(tars::Int32 port);
        tars::Int32 getGw3Port();

        void setGw2Ip2(const std::string & ip);
        std::string getGw2Ip2();
        void setGw2Port2(tars::Int32 port);
        tars::Int32 getGw2Port2();

        void setMiddleGwIp(const std::string & ip);
        std::string getMiddleGwIp();
        void setMiddleGwPort(tars::Int32 port);
        tars::Int32 getMiddleGwPort();
        void setMrfpSdpIp(const std::string & ip);
        std::string getMrfpSdpIp();
        void setMrfpSdpPort(const std::string & ports);
        std::string getMrfpSdpPort();
        void setMrfpCodec(const std::string & code);
        std::string getMrfpCodec();

        void setSipMsg(const std::string & msg);
        std::string getSipMsg();
        void setMrfpMsg(const std::string & msg);
        std::string getMrfpMsg();
        void setResendMsg(const std::string & msg, const std::string & gwOid, const std::string & connId, const std::string & recvConnId);
        void getResendMsg(std::string & msg, std::string & gwOid, std::string & connId, std::string & recvConnId);

        void setCCId(tars::Int32 ccId);
        tars::Int32 getCCId();

        void setSessionId(tars::Int32 sesionId);
        tars::Int32 getSessionId();

        void setChargedTime(tars::Int64 acceptedTime);
        tars::Int64 getLastChargedTime();

        void setSdpIp(const std::string & ip);
        std::string getSdpIp();

        void setAcceptedTime(const tars::Int64 & time);
        tars::Int64 getAcceptedTime();
        void setAlertingTime(const tars::Int64 & time);
        tars::Int64 getAlertingTime();

        void setTermingTime();
        tars::Int64 getTermingTime();

        void setCCStatusType(tars::Int32 iCCStatusType);
        tars::Int32 getCCStatusType();

        void setRealReqTime(tars::Int32 time);
        tars::Int32 getRealReqTime();

        void setTotalTime(tars::Int32 time);
        tars::Int32 getTotalTime();
        void setUsedTime(tars::Int32 time);
        tars::Int32 getUsedTime();

        void setFromRoute(tars::Bool bRoute);
        tars::Bool getFromRoute();
        void setToRoute(tars::Bool bRoute);
        tars::Bool getToRoute();

        void setFromNATPres(tars::Bool bflag);
        tars::Bool getFromNATPres();
        void setToNATPres(tars::Bool bflag);
        tars::Bool getToNATPres();
        void setFromNATIp(const std::string & ip);
        std::string getFromNATIp();
        void setFromNATPort(tars::Int32 iPort);
        tars::Int32 getFromNATPort();
        void setToNATIp(const std::string & ip);
        std::string getToNATIp();
        void setToNATPort(tars::Int32 iPort);
        tars::Int32 getToNATPort();
        void setDomainId(const std::string & domainId);
        std::string getDomainId();

        void setChargeAccount(const std::string & account);
        std::string getChargeAccount();

        void setConnectionId(const std::string & id);
        std::string getConnectionId();

        void setAtsOid(const std::string & id);
        std::string getAtsOid();

        void setPAIName(const std::string & name, const std::string & host = "");
        std::string getPAIName();

        void setMeetingId(const std::string & id);
        std::string getMeetingId();

        void setOCSChargePres(tars::Bool pres);
        tars::Bool getOCSChargePres();
        void setStopChargePres(tars::Bool pres);
        tars::Bool getStopChargePres();

        void setCallerEnterpriseType(EnterpriseType type);
        EnterpriseType getCallerEnterpriseType();
        void setCalleeEnterpriseType(EnterpriseType type);
        EnterpriseType getCalleeEnterpriseType();
        void setOriginalCallId(const std::string & callId);
        std::string getOriginalCallId();
        void setJegoParams(const std::string & lastGwIp, const std::string & enterIp);
        void getJegoParams(std::string & lastGwIp, std::string & enterIp);
        void setBeep(tars::Int32 beep);
        tars::Int32 getBeep();
        void setBeepPres(tars::Bool beepPres);
        tars::Bool getBeepPres();
        void setMrfpBeepErrCode(tars::Int32 code);
        tars::Int32 getMrfpBeepErrCode();
        void setMrfpInitPres(tars::Bool pres);
        tars::Bool getMrfpInitPres();
        void setResendPres(tars::Bool resendPres);
        tars::Bool getResendPres();
        void setMrfpRingStatus(MrfpRingStatus status);
        MrfpRingStatus getMrfpRingStatus();
        void setServiceType(tars::Int32 type);
        tars::Int32 getServiceType();
        void setRelease(tars::Bool release);
        tars::Int32 getRelease();
        void setRecordRoute(tars::Bool isRecordRoute);
        tars::Int32 getRecordRoute();

        void setReleaseType(CallReleaseType type);
        CallReleaseType getReleaseType();

        void setOcsErrorCode(tars::Int32 code);
        tars::Int32 getOcsErrorCode();

        void setMediaCoding(vector<std::string> &coding);
        vector<std::string> getMediaCoding();
        void setMrfpMediaCoding(tars::Int32 conding);
        tars::Int32 getMrfpMediaCoding();
        void setMrfpSdp(const std::string & ip);
        std::string getMrfpSdp();

        void setQosIps(set<std::string>& ip);
        set<std::string> getQosIps();

        // void createTimer(const Common::ApplicationExPtr & application, const ChargeTimerExecutorPtr & ptr, tars::Int32 time);
        // void closeTimer();

        void setCmiCovsInfo(const std::string &val);
        std::string getCmiCovsInfo();

        void pushbackDecodeObj(const SipMsgDecodePtr & obj);

        static tars::Bool decodeDomainIdAppId(const std::string & domainIdAndAppId, std::string & domainId, std::string & appId);
        static void decodeKeyValPairsConfig(const std::string & cfg, std::map<std::string, std::string> & pairs);
        static void decodeKeyValPairsConfig2(const std::string & cfg, std::vector<std::string> & pairs);
        static void decodeKeyValPairsConfig3(const std::string & cfg, vector<tars::Int32> & pairs);
        static void decodeKeyValPairsConfig4(const std::string & cfg, map<std::string,VoipApp::authInfo> & mpInfo);
        static void decodeKeyValPairsConfig(const std::string & cfg, vector<std::string> & pairs);
        static void decodeKeyValPairsConfig5(const std::string & cfg, std::map<std::string, vector<VoipApp::enterPrefix>> & mapInfo);
    };

    //                          SipDlgManager
    class SipDlgManager
    {
    private:
        typedef map<std::string, SipDlgPtr> SipDialogsMapType;
        SipDialogsMapType                  _sipDialogs;
        std::mutex                         m_mutex;                          //通用互斥锁
        std::mutex                         _sipDialogsMutex;
        tars::Int64                        _TotalCreatedDialogsCount;

        tars::Int64                        _timeoutNo180;
        tars::Int64                        _timeoutNo200;
        tars::Int64                        _timeoutNormalClosed;

    public:
        SipDlgManager();

        tars::Int32 countSipDialogs();
        SipDlgPtr getSipDialog(const std::string & didStr);
        SipDlgPtr getSipDialog3(tars::Int32 sessId);
        SipDlgPtr getSipDialogByCallId(const std::string & callId);
        SipDlgPtr createSipDialog(const std::string & didStr);
        tars::Bool destroySipDialog(const std::string & didStr);

        tars::Bool removeSipDialog(const std::string & didStr);
        tars::Bool insertSipDialog(const std::string & didStr, const SipDlgPtr & dialog);
        tars::Bool getTimeOutSipDialog(vector<SipDlgPtr> & vectSipDialogs, tars::Int32 & iSipToRpc180, tars::Int32 & iSipToRpc200, tars::Int32 & iRpcToSip180, tars::Int32 & iRpcToSip200,tars::Int32 & iNomalTerm, tars::Int32 & iExcepDeal);
        tars::Bool getExceedMaxDurationSipDialog(vector<SipDlgPtr> & vectSipDialogs);
        tars::Bool getChargeTimingOutSipDialog(vector<SipDlgPtr> & vectSipDialogs);
        tars::Bool getMrfpTimeOutSipDialog(vector<SipDlgPtr> & vectSipDialogs);
        tars::Bool getToBeResendSipDialogs(vector<SipDlgPtr> & vectSipDialogs);

        tars::Bool checkRefreshSessionRequest(const SipDlgPtr & pDialog , tars::Bool & bRefresh,  SipDlgPtr & pOriginalDialog, tars::Bool & bRefreshPre, tars::Bool & bUacRefresh);
        tars::Bool getOriginalSipDialog(const SipDlgPtr & pDialog,  SipDlgPtr & pOriginalDialog);

        SipDlgPtr getSipDialog2(const std::string & callIdHost);
        void pintOutSipDialogMapSize();
        void printAllSipDialog();

        void setTimeoutFor180(tars::Int64 times);
        void setTimeoutFor200(tars::Int64 times);
        void setTimeoutForClosed(tars::Int64 times);
    };

    class SipDlgDbRecordManager
    {
    public:
        tars::Bool insertRecord(const std::string & rpcCallId, const std::string & sipCallId, const std::string & fromUser, const std::string & toUser, const std::string & toPhone,
            const std::string & domainId, const std::string & appId, tars::Bool bConfDialog, tars::Bool bAppToSip, tars::Int64 st, tars::Int64 et, tars::Int32 td,const std::string & sipGtwId,
            const std::string & sipProxyId, const std::string & coreNetId, tars::Int32 mediaType, const std::string  & confUri, const std::string & confMid, tars::Int32 errCode, tars::Bool finished = false);

        tars::Bool exportRecords(map<std::string, SipGateway::SipDialogDbRecord> & callRecords);
        tars::Bool roolBackRecords(const map<std::string, SipGateway::SipDialogDbRecord> & callRecords);
        tars::Int32 getSize();
        tars::Bool checkRepeatCall(const std::string & fromUser, const std::string & toUser, const std::string & toPhone,std::string & rpcCallId, std::string & sipDialogId);
        void exceptionRecordsDeal();

    private:

        std::mutex                                    m_mutexRecords;
        map<std::string, SipGateway::SipDialogDbRecord>  m_mapRecords;
    };
};

#endif //__SipDialog2_h