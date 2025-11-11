#ifndef __SipDialog_h
#define __SipDialog_h

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "servant/Application.h"
#include "util/tc_thread_mutex.h"
#include "util/tc_timeprovider.h"
#include "SipGatewayCommon.h"
#include "SipMsgDecodedPart.h"
#include "SipMsgCommon.h"
#include "SipProxy/src/SipRegisterSubscribe.h"
#include "SipMsgDecode.h"


namespace SipGateway
{

    //media type
    enum MediaType
    {
        kAudioType = 0,
        kVideoType = 1
    };

    //define the sip dialog status
    enum SipDialogState
    {
        kSipDialogStateInit,                            //新建
        kSipDialogStateEarly,                           //180回复
        kSipDialogStateCancing,                         //正在取消，即已发送CANCEL
        kSipDialogStateCanced,                          //已取消
        kSipDialogState200Ok,                           //200 OK
        kSipDialogStateACKConfirmed,                    //ACK
        kSipDialogStateTermnating,                      //正在 结束
        kSipDialogStateTermed,                          //已经 结束
        kSipDialogStateErase,                            //临时增加状态，从集合中清除
    };

    //define the SIP to RPC Release type when Sip Gateway call Call server function "release"
    enum CallReleaseType
    {
        SipToRpcReleaseTypeNormal,                      //正常释放，即接受BYE 消息后调用
        SipToRpcReleaseTypeCreateFailed,                //Sip Gateway call the create of Call Server failed
        SipToRpcReleaseTypeNo200OK,                     //振铃无应答
        SipToRpcReleaseTypeSend180Failed,               //发送180或者200失败，可能是网络原因，所以无法通知SIP端
        SipToRpcReleaseTypeNo180,                       //调用Call Server的Create之后，等待调用onWaitingAcception超时
        SipToRpcReleaseTypeDisOrder,                    //调用onWaitingAcception和onAccept错序
        SipToRpcReleaseTypeCANCELED,                    //SIP 主动取消呼叫

        RpcTpSipReleaseTypeBusy,                        //Rpc 端呼叫 Sip端，Sip端忙，返回486，调用Call Server 的release
        RpcToSipReleaseTypeNo180,                       //Rpc端呼叫 Sip端， Sip Gateway超时等待180消息，则调用Call Server的release，以及发送CANCEL给SIP 端
        RpcToSipReleaseTypeNo200,
        RpcToSipReleaseTypeCallWaitingAccpFailed,       //Rpc call Sip, Sip Gateway call waitingAcception failed.
        RpcToSipReleaseTypeCallAccpFailed,              //......
        RpcToSipReleaseType403InvalidUser,
        RpcToSipReleaseType487,
        RpcToSipReleaseTypeNot200OK,
        ReleaseTypeForceCloseBoth,                      //两个方向强制结束
        ReleaseTypeForceCloseCaller,                    //结束主叫
        ReleaseTypeForceCloseCallee,                    //结束被叫
    };


    //定义超时类型
    enum TimeOutType
    {
        TimeOutTypeNo,                                  //无异常
        TimeOutTypeSipToRpcNo180,
        TimeOutTypeSipToRpcNo200,
        TimeOutTypeRpcToSipNo180,
        TimeOutTypeRpcToSipNo200,
        TimeOutTypeNomalTerminated,                      //已结束，但未清除SipDialog等对象、资源
        TimeOutTypeReSendTimeOut,                        //重发超过最大次数
        TimeOutTypeExceptionDealTimeOut,                 //异常处理超时
        TimeOutTypeExceedMaxDuration,                    //对象生存超出规定最大时长
    };

    //定义认证类型
    enum SipTimeoutType
    {
        kSipTimeoutTypeNone = 0,
        kSipTimeoutTypeNoMedia,
        kSipTimeoutTypeCleanRes
    };

    enum AuthType
    {
        AuthNone = 0,
        Auth401 = 1,
        Auth407 = 2
    };

    //定义SDP协商过程
    enum SDPNegotiateStatus
    {
        SDPInit = 0,                                    //初始化状态   ：尚未 收/发 offer
        SDPNegotiating = 1,                             //正在协商状态 ：已经 收/发 offer
        SDPNegotiated = 2                               //协商完成     ：已经 收/发 answer
    };

    enum SDPAnsweredType
    {
        kSDPUnAnswered = -1,
        kSDPEarlyAnswered = 0,
        kSDP200OKAnswered = 1
    };

    enum DialogRole
    {
        kDialogRoleInvalid = 0,
        kDialogRoleVisitor,
        kDialogRoleCalleeAlloc,
        kDialogRoleCalleeConsult,
        kDialogRoleOneStepTrans,
    };

    enum ChargeType
    {
        kChargeTypeNone = 0,
        kChargeTypePayment = 1,
        kChargeTypeCallBack,
        kChargeTypeCallBackBlackList
    };

    enum ReleaseType
    {
        kReleaseByApp = 0,
        kReleaseBySip,
        kReleaseByServer,
        kReleaseByCaller,
        kReleaseByCallee,
    };

    //定义超时时间
    static int64_t TimeOutSipToRpcNo180 = 2 * 60 * 1000;
    static int64_t TimeOutSipToRpcNo200 = 4 * 60 * 1000;
    static int64_t TimeOutRpcToSipNo180 = 2 * 60 * 1000;
    static int64_t TimeOutRpcToSipNo200 = 4 * 60 * 1000;
    static int64_t TimeOutExceptionDeal = 2 * 60 * 1000;
    static int64_t TimeOutNomalTerminate = 60 * 1000;
    static int64_t TimeOutMaxDialogActiveDuration =  12 * 60 * 60 * 1000;

    static int          MaxReSendTimes = 5;
    static int          MaxReSendInterval = 5 * 1000;
    static int          MAX_RECORD_WRITE_THROLD = 50;


    //                     SipDialogId
    class SipDialogId;
    typedef std::shared_ptr<SipDialogId> SipDialogIdPtr;

    class SipDialogId
    {
    public:
        std::string              _sipCallId;
        std::string              _sipLocalTag;
        std::string              _sipRemoteTag;
    private:
        std::mutex               _mutex;
    public:
        SipDialogId();
        SipDialogId(const std::string & id, const std::string & ltag, const std::string & rtag);
        SipDialogId(const SipDialogIdPtr& dId) ;
        ~SipDialogId();

    public:
        static SipDialogIdPtr create(const std::string & cid, const std::string & ltag, const std::string & rtag);
        static bool isParameterValid(const std::string & cid, const std::string & ltag, const std::string & rtag);

    public:
        std::string string() const;
        static SipDialogIdPtr fromString(const std::string & str);

        bool isValid ()const;
        bool isEqual(const SipDialogIdPtr& id);
    };

    //                       SipDialog                          
    class MediaTimerExecutor
    {
    public:
        virtual void mediaConnStatusTimeout(const std::string & sipDialogId) = 0;
        virtual void cleanSipResourceTimeout(const std::string & sipDialogId) = 0;
    };
    typedef std::shared_ptr<MediaTimerExecutor> MediaTimerExecutorPtr;

    //                       SipDialog
    class SipDialog;
    typedef std::shared_ptr<SipDialog> SipDialogPtr;
    class SipDialog: public std::enable_shared_from_this<SipDialog>
    {
    public:
        void onTimeout();

    private:
        std::string                     m_sipDialogId;
        tars::CurrentPtr                m_onAcceptedCallPtr;
        MediaTimerExecutorPtr           m_mediaTimerExecutorPtr;
        std::mutex                      m_mutex;
    public:
        SipMsgDecodedPart               _sipMsgDecodePart;
        SipProxy::SipRegisterSubscribePtr       _sipRegisterSub;

        static std::atomic<int>         m_totalNum;
        static std::atomic<int>         m_totalNewNum;
        static std::atomic<int>         m_totalDeleteNum;
        static std::atomic<int>         m_totalExceptionNum;

        static void PirntOutSipDialogNum();
        static void autoAddExceptionNum();

    private:
        SipDialogState              m_sipDialogStatus;              //Sip Dialog status
        CallReleaseType             m_sipToRpcReleaseType;          //Sip Gateway�调用CallServer的 release，释放类型
        bool                        m_bSipCallRpc;                  //标志是否是SIP  Call RPC ,用以在相同的180或者200超时时选择不同的操作
        TimeOutType                 m_timeOutType;                  //超时类型
        int64_t                m_createTime;                   //创建时间点
        int64_t                m_curStatusInitTime;            //进入当前对话状态时间点
        int64_t                m_answeredTime;                 //接听时间
        int64_t                m_enterExceptionTime;           //进入异常时间点,设置一个处理异常时间值，异常进入超过这个值则认为异常处理过程异常，强行清除SipDialog等对象、资源
        int64_t                m_terminateTime;                //进入结束状态时间点
        int64_t                m_ereaseTime;                   //清除时间点

        std::string              m_strRandomId;                   //随机字符串，用于在重复创建时区分
        std::string              m_strSipGatewayId;              //负责处理本对话的SIpGateway服务id或者名称，即第一次接收SipProxy发送的INVITE，或者组建之后再发送
        std::string              m_strSipProxyId;                //进行本通话的用户Client所绑定的SipProxy的Id

        AuthType                    m_authType;                     //认证类型
        SDPNegotiateStatus          m_sdpNegoStatus;                //SDP 协商状态
        bool                        m_bSipToRpcAckWithSDP;          //标识是否Sip call RPC, ACK contain SDP information
        bool                        m_bISBCAccessed;                //是否属于I-SBC对接方式接入核心网
        std::string              m_strToSBCConnectionId;         //呼叫代理SipProxy与SBC连接的Connection的id
        bool                        m_bReleaseBySipApp;             //呼叫是否由Sip客户端结束

        std::string              m_strInviteSDP;
        bool                        m_bRefreshDialog;               //是否为刷新对话
        int64_t                m_lLastRefreshTime;             //上次刷新对话时间
        int                         m_iReSendTimes;                 //重发次数
        int64_t                m_lSendTime;                    //发送时间
        SipRequestMethodType        m_reSendMethod;                 //重发方法

        int64_t                m_releaseTime;                  //�Ҷ�ʱ�
        std::string              m_strSbcIp;
        int64_t                m_lAlertedTime;                 //����ʱ�
        int64_t                m_lEarlySessionTime;
        int64_t                m_lRingTime;
        int64_t                m_lAnswerTime;
        int64_t                m_lTerminateTime;
        ReleaseType                 m_releaseType;

        std::string              m_strCallerNumber;
        std::string              m_strCalleeNumber;
        std::string              m_strDisplayNumber;
        std::string              m_strCallerPrefix;
        std::string              m_strCalleePrefix;

        std::string              m_strSbcMediaIp;
        std::string              m_strRtcMediaIp;
        std::string              m_strRtcCC;
        std::string              m_strRtcIp;

        bool                        m_bReSendBye;                   //�收到的SBC端重发的BYE，本端SipDialog对象已析构，直接返回200 OK BYE

        int64_t                m_lRpcCallId;                   //rpc call id
        std::string              m_strDomainIdAppId;             //DomainId : AppId
        std::string              m_strCoreNetId;                 //CoreNet id

        bool                        m_bSessionFreshDialog;          //�本对话对象是否作为刷新会话存在
        EN_SIP_METHOD               m_refreshMethod;
        MediaType                   m_mediaType;

        SDPAnsweredType             m_appToCsSDPAnsweredType;       //app to cs, SDP协商类型

        ChargeType                  m_chargeType;
        std::string              m_interceptType;
        std::string              m_blacklistType;
        int                         m_frequency;

        std::string              m_rtpGwCp;                      //rtpGw cp address
        std::string              m_rtpGwIp;                      //rtpGw ip
        std::string              m_sbcIp;                        //sbc ip
        std::string              m_sdpOffer;
        std::string              m_sdpAnswer;
        std::string              m_appSdpOffer;
        std::string              m_appSdpAnswer;
        std::string              m_sipSdpOffer;
        std::string              m_sipSdpAnswer;
        std::string              m_strSdpIp;

        std::string              m_content;
        std::string              m_endpoint;
        std::string              m_routerHost;
        int                         m_routerPort;

        bool                        m_rtpAllocBySipGw;
        bool                        m_sbcSupportUpdate;

        bool                        m_newCallFlow;
        std::string              m_callServerExOid;
        bool                        m_sdpAnserSet;
        bool                        m_bHoldOn;

        std::map<std::string, std::string>           m_mapEpSdps;
        std::mutex                     m_mutexEpSdps;
        std::string              m_hostPorts;
        bool                        m_isNewSdk;
        std::string              m_originDialogId;
        int                         m_remainMinute;
        std::string              m_originCalleeNumber;
        std::string              m_sipClientFakeIp;
        std::string              m_rtpGwArcOid;
        std::string              m_rtpGwId;
        std::vector<SipMsgDecodePtr>     m_vectDecodes;

        std::string              m_calleeNationCode;
        int                         m_failedTimes;
        std::map<std::string, std::string>           m_params;
        std::string              m_callerUid;
        std::string              m_ocsErrorCode;
        int                         m_iRseq;
        std::string              m_calleeRealm;
        bool                        m_bMediaChecked;
        std::mutex                     m_mutexCallSbcIps;
        std::vector<std::string>      m_vectCallSbcIps;
        std::string              m_mediaConnFailedReason;
        std::string              m_byeReason;

        bool                        m_bAlertedFlag;

        //for AcdGateway
        std::string              m_callSerialNum;
        std::string              m_ccsvrOid;
        std::string              m_confId;
        std::string              m_roomId;
        std::string              m_confServerOid;
        std::string              m_serviceTelNum;
        std::string              m_uuiData;
        DialogRole                  m_dialogRole;
        std::string              m_callerSession;
        std::string              m_jsmSerialNo;
        std::string              m_tokenVal;
        std::vector<int>                 m_ports;
        int                         m_iErrorCode;
        int                         m_iMediaCode;
        int                         m_iOtherCode;
        int                         m_iCauseCode;

        std::string              m_realm;

    public:

        std::string              m_strCallerUserAccount;
        std::string              m_strCalleeUserAccount;

        //std::string              m_strCallerNumber;
        //std::string              m_strCalleeNumber;
        std::shared_ptr<tars::TC_Timer>            m_waitTimer;
        std::shared_ptr<tars::TC_Timer>            m_releaseTimer;
        int                         m_acceptedNoMediaTimeout;
        SipTimeoutType              m_sipTimeOutType;

        std::string              m_strCallBackCallerNumber;
        std::string              m_strCallBackCalleeNumber;

        SipDialog(int a){}
        SipDialog();
        SipDialog(const SipDialogIdPtr & did);
        virtual ~SipDialog();

    public:
        static SipDialogPtr create(const SipDialogIdPtr & did);
        const std::string id();

        bool getCallerUserAccount(std::string & caller);
        bool getCalleeUserAccount(std::string & callee);
        bool getPureUserAccount(const std::string & callAccount, std::string & number);
        bool getFormatUserAccount(const std::string & pureNumber, std::string & userAccount);

        static SipRequestMethodType SipMethodToSipDialogMethod(EN_SIP_METHOD enSipMethod);
        static EN_SIP_METHOD  SipDialogMethodToSipMethod(SipRequestMethodType enSipMethod);

        static int SipDialogResponseStatusToSipStatus(SipResponseCodeType enSipDialogStatus);
        static SipResponseCodeType SipStatusToSipDialogResponseStatus(int iStatus);

        void pushOnAcceptedCallPtr(const tars::CurrentPtr & ptr);
        const tars::CurrentPtr popOnAcceptedCallPtr();

        void setCallReleaseType(const CallReleaseType & releaseType);
        CallReleaseType getSipToRpcReleaseType();

        void setSipDialogStatus(const SipDialogState & dialogStatus);
        SipDialogState getSipDialogStatus();

        void setCurStatusInitTime(const int64_t & lInitTime);
        int64_t getCurStatusInitTime();

        void setSipCallRpcProp(bool bSipCallRpc);
        bool getSipCallRpcProp();

        void setReleaseBySipApp(bool bBySip);
        bool getReleaseBySipApp();

        void setTimeOutType(TimeOutType type);
        TimeOutType getTimeOutType();

        int64_t getEnterExceptionTime();
        int64_t getCreateTime();

        void setSipGatewayId(const std::string & gwId);
        void setSipProxyId(const std::string & spId);
        std::string getSipGatewayId();
        std::string getSipProxyId();

        AuthType getAuthType();
        void setAuthType(AuthType type);
        SDPNegotiateStatus getSdpNegotiateStatus();
        void setSdpNegotiateStatus(SDPNegotiateStatus status);
        int getAcceptRpcResourceParameter();
        bool getSipToRpcACKWithSDP();
        void setSipToRpcACKWithSDP(bool bWith);

        void formatDomainUserAccount(const std::string & relationType,const std::string & nationCode,
            const std::string & sipAccount, const std::string & domainId, const std::string & domainName,  bool caller);
        void setDomainUserAccount(const std::string & account, bool bCaller);
        void getDomainUserAccount(std::string & account,bool bCaller);
        void setCallNumber(const std::string & number, bool bCaller);
        std::string getCallNumber(bool bCaller);
        void setISBCAccessed(bool bISBCAccessed);
        bool getISBCAccessed();

        void setChargeAccount(const std::string & number, bool bCaller);
        std::string getChargeAccount(bool bCaller);

        void setToSBCConnectionId(const std::string & connectionId);
        std::string getToSBCConnectionId();
        void setSbcIp(const std::string & sbcIp);
        std::string getSbcIp();

        void setIsFreshDialog(bool isFreshDialog);
        bool getIsFreshDialog();

        void setLastRefreshTime(int64_t refreshTime);
        int64_t getLastFreshTime();

        void RepeatTimeAutoIncrease();
        int getReSendTimes();
        void reSetRepeatTimes(SipRequestMethodType reSendMethod);

        void setReSendMethod(SipRequestMethodType reSendMethod);
        SipRequestMethodType getReSendMethod();

        void setSendTime();
        int64_t getSendTime();

        void setEarlySessionTime();
        int64_t getEarlySessionTime();
        void setRingTime();
        int64_t getRingTime();
        void setAnswerTime();
        int64_t getAnswerTime();
        void setTerminateTime();
        int64_t getTerminateTime();
        void setReleaseType(ReleaseType type);
        ReleaseType getReleaseType();

        void setCallerNumber(const std::string & number);
        std::string getCallerNumber();
        void setCalleeNumber(const std::string & number);
        std::string getCalleeNumber();
        void setDisplayNumber(const std::string & number);
        std::string getDisplayNumber();
        void setSbcMediaIp(const std::string & ip);
        std::string getSbcMediaIp();
        void setRtcMediaIp(const std::string & ip);
        std::string getRtcMediaIp();
        void setRtcCC(const std::string & cc);
        std::string getRtcCC();
        void setRtcIp(const std::string & ip);
        std::string getRtcIp();

        void setCallerPrefix(const std::string & prefix);
        std::string getCallerPrefix();
        void setCalleePrefix(const std::string & prefix);
        std::string getCalleePrefix();

        void setReSendBye(bool bReSendBye);
        bool getReSendBye();

        void setRpcCallId(int64_t rcpCallId);
        int64_t getRpcCallId();

        void setDomainIdAppId(const std::string & domainIdAppId);
        std::string getDomainIdAppId();

        void  setCoreNetId(const std::string & coreNetid);
        std::string getCoreNetId();

        void setSessionFreshDilogFlag(bool bFreshDialog);
        bool getSessionFreshDilogFlag();

        void setRefreshMethod(EN_SIP_METHOD freshMethod);
        EN_SIP_METHOD getRefreshMethod();

        void setAnseredTime(int64_t time);
        int64_t getAnseredTime();
        void setMediaType(MediaType type);
        MediaType getMediaType();

        void setSDPAnsweredType(SDPAnsweredType type);
        SDPAnsweredType getSDPAnsweredType();

        void setCalleeRtpGWCp(const std::string & calleeCp);
        std::string getCalleeRtpGwCp();

        void setRtpGwIp(const std::string & ip);
        std::string getRtpGwIp();

        void setSBCIp(const std::string & ip);
        std::string getSBCIp();

        void setSdpOffer(const std::string & sdp);
        std::string getSdpOffer();
        void setSdpAnswer(const std::string & sdp);
        std::string getSdpAnswer();

        void setAppSdpOffer(const std::string & sdp);
        std::string getAppSdpOffer();
        void setAppSdpAnswer(const std::string & sdp);
        std::string getAppSdpAnswer();
        void setSipSdpOffer(const std::string & sdp);
        std::string getSipSdpOffer();
        void setSipSdpAnswer(const std::string & sdp);
        std::string getSipSdpAnswer();
        void setSdpIp(const std::string & ip);
        std::string getSdpIp();

        void setContent(const std::string & content);
        std::string getContent();
        void setEndpoint(const std::string & endpoint);
        std::string getEndpoint();

        void setRouterHost(const std::string & host);
        std::string getRouterHost();
        void setRouterPort(int port);
        int getRouterPort();

        void setRtpAllocedByGw(bool bySipGw);
        bool getRtpAllocedByGw();

        void setSBCSupportUpdate(bool bSupport);
        bool getSBCSupportUpdate();

        void setProtocolType(unsigned char tptType);
        unsigned char getProtocolType();

        void setCallFlow(bool bNewFlow);
        bool getCallFlow();

        void setCallServerExOid(const std::string & id);
        std::string getCallServerExOid();

        void setSDPAnswerSetFlag(bool setFlag);
        bool getSDPAnswerSetFlag();

        void setHoldOnFlag(bool holdon);
        bool getHoldOnFlag();

        void insertAppEpAndSdp(const std::string & ep, const std::string & sdp);
        std::string getAppSdp(const std::string & ep);
        void setRtpGwHostPorts(const std::string & hostPorts);
        std::string getRtpGwHostPorts();

        void setIsPeerNewSdk(bool isNewSdk);
        bool getIsPeerNewSdk();

        void setOriginDialogId(const std::string & id);
        std::string getOriginDialogId();

        void setRemainMinute(int minutes);
        int getRemainMinute();
        std::string getSipCallId();

        void setOriginCalleeNum(const std::string & number);
        std::string getOriginCalleeNum();

        void setSipClientFakeIp(const std::string & ip);
        std::string getSipClientFakeIp();

        void setRtpGwArcOid(const std::string & oid);
        std::string getRtpGwArcOid();
        void setRtpGwId(const std::string & oid);
        std::string getRtpGwId();

        void pushbackDecodeObj(const SipMsgDecodePtr & obj);

        void setCalleeNationCode(const std::string & code);
        std::string getCalleeNationCode();

        void addFailedTimes();
        int getFailedTimes();

        void setParams(const std::map<std::string, std::string> & params);
        std::map<std::string, std::string> getParams();
        std::string getCallIdHost();

        void setCallerUid(const std::string & uid);
        std::string getCallerUid();

        void setRseqUpdate();
        int getRseqUpdate();

        void setOCSErrorCode(const std::string & code);
        std::string getOCSErrorCode();

        void setBlacklistType(const std::string & type);
        std::string getBlacklistType();
        void setInterceptType(const std::string & type);
        std::string getInterceptType();
        void setFrequency(int code);
        int getFrequency();
        void setCalleeRealm(const std::string & realm);
        std::string getCalleeRealm();

        void setCallSbcIps(const std::string & ip);
        void getCallSbcIps(std::vector<std::string> & vectIps);

        bool setAlertedFlag();
        bool getAlertedFlag();
        void setMediaCheckFlag();
        int getMediaCheckFlag();
        void setMediaConnFailedReason(const std::string & reason);
        std::string getMediaConnFailedReason();
        void setByeReason(const std::string & reason);
        std::string getByeReason();

        // bool createTimer(const tars::Application * & application,  int time, SipTimeoutType type);
        // void reCreateTimer(const tars::Application * & application,  int time, SipTimeoutType type);
        SipTimeoutType getSipTimeOutType();
        //void closeTimer();
        //void stopTimer();

        //void createReleaseTimer(const tars::Application * & application, int time);
        //void closeReleaseTimer();

        void setChargeType(ChargeType type);
        ChargeType getChargeType();

        void setErrorCode(int code);
        int getErrorCode();
        void setMediaErrorCode(int code);
        int getMediaErrorCode();
        void setOtherErrorCode(int code);
        int getOtherErrorCode();

        void setErrorCauseCode(int code);
        int getErrorCauseCode();

        void setReleaseTime(int64_t time);
        int64_t getReleaseTime();

        //for AcdGateway
        void setCallSerialNum(const std::string & num);
        std::string getCallSerialNum();
        void setCcsvrOid(const std::string & id);
        std::string getCcsvrOid();
        void setConfId(const std::string & id);
        std::string getConfId();
        void setRoomId(const std::string & id);
        std::string getRoomId();
        void setServiceTelNum(const std::string & num);
        std::string getServiceTelNum();
        void setUuiData(const std::string & val);
        std::string getUuiData();
        void setDialogRole(DialogRole role);
        DialogRole getDialogRole();
        void setCallerSession(const std::string & session);
        std::string getCallerSession();
        void setConfServerOid(const std::string & oid);
        std::string getConfServerOid();
        void setJsmSerialNum(const std::string & no);
        std::string getJsmSerialNum();
        void setTokenVal(const std::string & val);
        std::string getTokenVal();
        void insertPorts(const std::vector<int> & ports);
        std::vector<int> getPorts();
        void setSipGatewayPtr(const MediaTimerExecutorPtr &ptr);
        void setMediaTimeOut(int time);

        void setDomainRealm(const std::string & realm);
        std::string getDomainRealm();

        static bool encodeDomainIdAppId(const std::string & domainId, const std::string & appId,std::string & domainIdAppId);
        static bool decodeDomainIdAppId(const std::string & domainIdAppId, std::string & domainId, std::string & appId);
        //copy from AccountId in AccountBase
        static void decodeAccountId(const std::string & accountId, std::string & identity, std::string & domain, std::string & type, bool & isUserId);
        static void decodeKeyValPairsConfig(const std::string & cfg, std::map<std::string, std::string> & pairs);
        static void decodeKeyValPairsConfig(const std::string& cfg, std::vector<std::string>& pairs);
    };

    //                          SipDialogManager
    class SipDialogManager
    {
    private:
        std::mutex                      m_mutex;
        typedef std::map<std::string, SipDialogPtr> SipDialogsMapType;
        SipDialogsMapType                      _sipDialogs;
        std::mutex                             _sipDialogsMutex;
        int64_t                           _TotalCreatedDialogsCount;


        int64_t                            _timeoutNo180;
        int64_t                            _timeoutNo200;
        int64_t                            _timeoutNormalClosed;
        int64_t                            _timeoutFlowNoAck;  //minutes

    public:
        SipDialogManager();

        int countSipDialogs();
        SipDialogPtr getSipDialog(const std::string & didStr);
        SipDialogPtr createSipDialog(const std::string & didStr);
        bool destroySipDialog(const std::string & didStr);

        bool removeSipDialog(const std::string & didStr);
        bool insertSipDialog(const std::string & didStr, const SipDialogPtr & dialog);
        bool getTimeOutSipDialog(std::vector<SipDialogPtr> & vectSipDialogs, int & iSipToRpc180, int & iSipToRpc200, int & iRpcToSip180, int & iRpcToSip200,int & iNomalTerm, int & iExcepDeal);
        bool getExceedMaxDurationSipDialog(std::vector<SipDialogPtr> & vectSipDialogs);

        void pintOutSipDialogMapSize();
        bool checkRefreshSessionRequest(const SipDialogPtr & pDialog , bool & bRefresh,  SipDialogPtr & pOriginalDialog, bool & bRefreshPre, bool & bUacRefresh);
        void getToBeRefreshedSipDialogs(std::vector<SipDialogPtr> & vectSipDialogs);
        void getToBeReSendSipDialogs(std::vector<SipDialogPtr> & vectSipDialogs);
        bool getOriginalSipDialog(const SipDialogPtr & pDialog,  SipDialogPtr & pOriginalDialog);

        SipDialogPtr getSipDialog2(const std::string & callIdHost);
        SipDialogPtr getSipDialog3(const std::string &exludeId, const std::string & callIdHost);

        void setTimeoutFor180(int64_t times);
        void setTimeoutFor200(int64_t times);
        void setTimeoutForClosed(int64_t times);
        void setTimeoutForFlowNoAck(int64_t times);
        int getApp2SipDialogNum();
    };

    //MappingTable
    class MappingTable
    {
    private:
        //�保存 RPC Call Id 到 Sip Dialog Id的映射
        typedef std::map<int64_t, std::string> RpcToSipMappingType;
        RpcToSipMappingType                   _mappingTableRpcToSip;
        std::mutex                            _mappingTableRpcToSipMutex;

        //保存 Sip Dialog Id 到 RPC Call Id的映射
        typedef std::map<std::string, int64_t> SipToRpcMappingType;
        SipToRpcMappingType                   _mappingTableSipToRpc;
        std::mutex                            _mappingTableSipToRpcMutex;
        std::mutex                            _mutex;
    public:
        int64_t rpcCallId(const std::string & didStr);
        std::string sipDialogId(int64_t id);

        bool insertMappingItem(int64_t cid, const std::string & did);
        bool deleteMappingItem(const std::string & did);
        bool deleteMappingItem(const int64_t id);

    public:
        int countMappingTableSipToRpc();
        int countMappingTableRpcToSip();

        //TEST
        static int              _rpcToSipNum;
        static int              _sipToRpcNum;
        static void PintOutRpcToSipTableNum();
        static void PintOutSipToRpcTableNum();
    };

    class AsyncInvokeUserData;
    typedef std::shared_ptr<AsyncInvokeUserData> AsyncInvokeUserDataPtr;
    class AsyncInvokeUserData
    {
    public:
        int64_t                    _rpcCallId;
        int64_t                    _rpcMethodId;
        std::string                  _sipDialogId;

    public:
        AsyncInvokeUserData();
        ~AsyncInvokeUserData(){};
        AsyncInvokeUserData(const int64_t callId, const int64_t methodId, const std::string & did);
        static AsyncInvokeUserDataPtr fromString(const std::string & str);
        std::string toString();
    };

    class SipDialogIdGenerator
    {
    private:
        static std::mutex       __g_serial_mutex;
        static unsigned int                   __g_serial;
    public:
        static SipDialogIdPtr generate(const int serverId, const std::string & serverName);
        static SipDialogIdPtr generate(const int serverId, const std::string & serverName, const std::string & sipGatewayId, const std::string & sipProxyId);
    };

    class Z62RadixTransfer
    {
    public:
        static char Z62DigitalToChar(int val);
        static int  Z62CharToDigital(char ch, int defaultVal);
    public:
        static std::string DecimalToZ62(int64_t dec);
        static int64_t Z62ToDecimal(const std::string & z52, const int64_t defaultVal);
    };

    //....................... SIP call record error code ..............................
    class SipDialogDbRecord
    {
    public:

        std::string                      m_domainId;
        std::string                      m_appId;
        bool                                m_finished;
        VoipApp::SipCallRecord           m_dbRecord;
    };

    //************************************* ims core net statistic ************************************
    enum SipCallResultType
    {
        kSipCallInit = 0,
        kSipCallRinging = 1,
        kSipCallAccepted = 2,
        kSipCallFailed = 3,
    };

    struct StatisticStruct
    {
        int             m_times;
        int             m_rings;
        int             m_accepted;
        int             m_failed;
        double          m_ringRate;
        double          m_acceptRate;
        double          m_failedRate;

        StatisticStruct()
        {
            m_times = 0;
            m_rings = 0;
            m_accepted = 0;
            m_failed = 0;
            m_ringRate = 0;
            m_acceptRate = 0;
            m_failedRate = 0;
        }
    };

    class SipCallRecord
    {
    public:

        SipCallRecord(const std::string & dialogId, const std::string & domainId, const std::string & coreNetId, const std::string & calleeNationCode);
        ~SipCallRecord();

        static std::string generateId(const std::string & dialogId, const std::string & coreNetId);

        std::string      m_id;
        std::string      m_sipDialogId;
        std::string      m_domainId;
        std::string      m_coreNetId;
        std::string      m_calleeNationCode;
        int64_t        m_insertTicks;

        bool                m_ringed;
        bool                m_accepted;
        bool                m_failed;
    };
    typedef std::shared_ptr<SipCallRecord> SipCallRecordPtr;

    class DomainStatistic
    {
    public:

        DomainStatistic(const std::string & domainId);
        void insertStatisticRecord(const std::string & dialogId, const std::string & domainId, const std::string & coreNetId, const std::string & calleeNationCode);
        bool updateResultType(const std::string & dialogId, const std::string & coreNetId, SipCallResultType typeVal);
        bool getHighestRingingCoreNet(const std::string & calleeNationCode, const std::string & excludedCoreNet, std::string & coreNetId);

        void clearTimeoutRecords();
        void executeStatistic();
        void setStatisticPeriod(const int64_t stPeriod);
        std::string getStatisticInfo();

        std::string      m_domainId;
        int64_t        m_statisticPeriod;

        std::mutex             m_mutexRecordsList;
        std::list<SipCallRecordPtr> m_listRecords;
        std::mutex             m_mutexRecordsMap;
        std::map<std::string, SipCallRecordPtr> m_mapRecords;

        typedef std::map<std::string, StatisticStruct> CoreNetRates;  //key:coreNetId
        typedef std::map<std::string, CoreNetRates > CalleeRates;     //key:callee nation code

        std::mutex             m_mutexStatistics;
        CalleeRates         m_mapStatistics;
    };
    typedef std::shared_ptr<DomainStatistic> DomainStatisticPtr;

    class SipCallStatisticRecordManager
    {
    public:

        SipCallStatisticRecordManager();

        void insertStatisticRecord(const std::string & domainId, const std::string & dialogId, const std::string & coreNetId, const std::string & calleeNationCode);
        bool updateResultType(const std::string & domainId, const std::string & dialogId, const std::string & coreNetId, SipCallResultType typeVal);
        bool getHighestRingingCoreNet(const std::string & domainId, const std::string & calleeNationCode,const std::string & excludedCoreNet, std::string & coreNetId);
        void setStatisticPeriod(const int64_t stPeriod);

        void clearTimeoutRecords();
        void executeStatistic();
        void onSchd();
        void printf();

    private:

        std::mutex                                   m_mutexStatistics;
        std::map<std::string, DomainStatisticPtr > m_statistics; //key -domainId

        int64_t    m_statisBaseSchdTime;
        int64_t    m_statisLastSchdTime;

        int64_t    m_statisPrintBaseSchdTime;
        int64_t    m_statisPrintLastSchdTime;
    };

    struct SbcLineInfo
    {
        std::string      m_coreNetId;
        bool                m_iSbc;

        bool operator<(const SbcLineInfo&obj) const;
        bool operator==(const SbcLineInfo&obj) const;
        bool operator>(const SbcLineInfo&obj) const;
    };


    //force response INVITE
    struct ForceResInvite
    {
        std::string      m_domainId;
        int                 m_tmpRspCode;
        int                 m_finalResCode;
        int                 m_deleyTime; //s

        ForceResInvite()
        {}

        ForceResInvite(const std::string & domainId, int tmpCode, int finalCode, int delay);
        bool operator<(const ForceResInvite&obj) const;
        bool operator==(const ForceResInvite&obj) const;
        bool operator>(const ForceResInvite&obj) const;
    };

    struct ForceResRecord
    {
        ForceResRecord(){}

        ForceResRecord(const std::string & dialogId, int tmpCode, int finalCode, int deleySecs)
        {
            m_dialogId = dialogId;
            m_tmpRspCode = tmpCode;
            m_finalRspCode = finalCode;
            m_delaySecs = deleySecs;
            m_ticks = TNOWMS;
        }

        std::string      m_dialogId;
        int64_t        m_ticks;
        int                 m_tmpRspCode;
        int                 m_finalRspCode;
        int                 m_delaySecs;
    };




};

#endif //__SipDialog_h