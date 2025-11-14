#include "SipGatewayCommon.h"
#include "SipDialog.h"
//#include "SipGatewayI.h"
#include "util/tc_uuid_generator.h"
#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstdio>

namespace SipGateway
{
    // Helper function to trim whitespace from both ends of a string
    static inline std::string trim(const std::string& str) {
        auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
            return std::isspace(ch);
        });
        auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
            return std::isspace(ch);
        }).base();
        return (start < end) ? std::string(start, end) : std::string();
    }

    // -----------------------------------------------

    bool SipDialogId::isParameterValid(const std::string & id, const std::string & ltag, const std::string & rtag)
    {
        if (id.empty())
        {
            SIPGW_LOG_WRN("SipDialogId, Call-Id: empty");
            return false;
        }

        if (ltag.empty())
        {
            SIPGW_LOG_WRN("SipDialogId, local-Tag: empty");
            return false;
        }

        if (rtag.empty())
        {
            SIPGW_LOG_DBG("SipDialogId, remote-Tag: empty");
        }

        return true;
    }

    SipDialogIdPtr SipDialogId::create(const std::string & cid, const std::string & ltag, const std::string & rtag)
    {
        if (isParameterValid(cid, ltag, rtag) == false)
        {
            SIPGW_LOG_WRN("SipDialogId::isParameterValid() failed, cid="+cid+",ltag="+ltag+",rtag="+rtag);
            return nullptr;
        }
        return std::make_shared<SipDialogId>(cid, ltag, rtag);
    }

    SipDialogId::SipDialogId()
    {
        _sipCallId = tars::TC_Common::tostr(tars::TC_UUIDGenerator::getInstance()->genID()) + "@SipGW";
        _sipLocalTag = tars::TC_Common::tostr(tars::TC_UUIDGenerator::getInstance()->genID());
    }

    SipDialogId::SipDialogId(const std::string & id, const std::string & ltag, const std::string & rtag):
    _sipCallId(id), _sipLocalTag(ltag), _sipRemoteTag(rtag)
    {}

    SipDialogId::SipDialogId(const SipDialogIdPtr& did)
    {
        if (did==nullptr)
        {
            SIPGW_LOG_WRN("SipDialogId::SipDialogId() , input did NULL");
            return;
        }
        if (did->isValid() == false)
        {
            SIPGW_LOG_WRN("SipDialogId::SipDialogId() , input did INVALID");
            return;
        }
        _sipCallId = did->_sipCallId;
        _sipLocalTag = did->_sipLocalTag;
        _sipRemoteTag = did->_sipRemoteTag;
    }

    SipDialogId::~SipDialogId()
    {
    }

    bool SipDialogId::isValid ()const
    {
        if ((_sipCallId.empty()) || (_sipLocalTag.empty()))
            return false;
        return true;
    }

    std::string SipDialogId::string ()const
    {
        if (isValid() == false)
            return "<INVALID>";

        const std::string info = _sipCallId + "\\" + _sipLocalTag + "\\" + _sipRemoteTag;
        return info;
    }

    SipDialogIdPtr SipDialogId::fromString(const std::string & str)
    {
        if (str == "<INVALID>")
        {
            return nullptr;
        }

        //Must have two ",", such as "a,b,c,d" or "a,b,,"
        int pos1 = str.find("\\");
        int pos2 = str.rfind("\\");
        if ((pos1 < 0)||(pos2 < 0))
        {
            return nullptr;
        }

        if (pos1 >= pos2)
        {
            return nullptr;
        }

        std::string cid = str.substr(0, pos1);
        std::string rtag = str.substr(pos2 + 1);
        std::string ltag = str.substr(pos1 + 1, pos2 - pos1 -1);
        if (cid.empty() || ltag.empty())
        {
            return nullptr;
        }
        else
        {
            return std::make_shared<SipDialogId>(cid, ltag, rtag);
        }
    }

    bool SipDialogId::isEqual(const SipDialogIdPtr& id)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (id == nullptr)
            return false;

        if (id->isValid() == false)
            return false;

        if ((id->_sipCallId != _sipCallId) ||
            (id->_sipLocalTag != _sipLocalTag) ||
            (id->_sipRemoteTag != _sipRemoteTag))
            return false;
        else
            return true;
    }

    //------------------------------------------------

    SipDialog::SipDialog(const SipDialogIdPtr & did)
    {
        int64_t curTime = tars::TC_TimeProvider::getInstance()->getNowMs();
        m_onAcceptedCallPtr = nullptr;
        m_sipToRpcReleaseType = SipToRpcReleaseTypeNormal;      //默认正常释放
        m_createTime  = curTime;
        m_curStatusInitTime  = curTime;
        m_sipDialogStatus = kSipDialogStateInit;                //初始状态
        m_bSipCallRpc = false;
        m_timeOutType = TimeOutTypeNo;
        m_enterExceptionTime = 0;                                //初始无异常进入
        m_sipTimeOutType = kSipTimeoutTypeNone;
        m_enterExceptionTime = 0;
        m_terminateTime = curTime;
        m_enterExceptionTime = 0;
        m_ereaseTime =curTime;

        if (did == nullptr)
        {
            //Log
            SIPGW_LOG_WRN("SipDialog(), INPUT did is NULL");
            //return;
        }
        else
        {
            if (did->isValid() == false)
            {
                SIPGW_LOG_WRN("SipDialog(), INPUT did is INVALID");
                //return;
            }
            m_sipDialogId = did->string();
        }

        m_totalNum++;
        m_totalNewNum++;

        m_strRandomId = tars::TC_Common::tostr(tars::TC_UUIDGenerator::getInstance()->genID());
        _sipRegisterSub = std::make_shared<SipProxy::SipRegisterSubscribe>();
        m_authType = AuthNone;
        m_sdpNegoStatus = SDPInit;
        m_bSipToRpcAckWithSDP = false;
        m_bRefreshDialog = false;
        m_lLastRefreshTime = curTime;
        m_iReSendTimes = 0;
        m_lSendTime = 0;
        m_lRingTime = 0;
        m_lEarlySessionTime = 0;
        m_lAnswerTime = 0;
        m_lTerminateTime = 0;

        m_bReSendBye = false;
        m_bSessionFreshDialog = false;
        m_answeredTime = 0;
        m_releaseTime = 0;
        m_bISBCAccessed = false;
        m_bReleaseBySipApp = false;
        m_mediaType = kAudioType;
        m_chargeType = kChargeTypeNone;
        m_appToCsSDPAnsweredType = kSDPUnAnswered;
        m_rtpAllocBySipGw = false;
        m_sbcSupportUpdate = false;

        m_newCallFlow = false;
        m_sdpAnserSet = false;
        m_bHoldOn = false;
        m_remainMinute = 0;
        m_lRpcCallId = 0;

        m_failedTimes = 0;
        m_dialogRole = kDialogRoleInvalid;
        m_releaseType = kReleaseByServer;

        m_iRseq = 1;
        m_iErrorCode = 0;
        m_iMediaCode = 0;
        m_iOtherCode = 0;
        m_iCauseCode = 0;
        m_frequency = 0;

        m_bAlertedFlag = false;
        m_mediaTimerExecutorPtr = nullptr;
        m_waitTimer = nullptr;
        m_releaseTimer = nullptr;
        m_acceptedNoMediaTimeout = 0;
        m_bMediaChecked = false;

        m_iMediaCode = 0;
        m_iOtherCode = 0;
        m_iCauseCode = 0;

        m_releaseType = kReleaseByServer;
    }

    SipDialog::~SipDialog()
    {
        if (m_onAcceptedCallPtr)
        {
#ifdef SIPGATEWAY
            SipGatewayI::onAccepted_end(m_onAcceptedCallPtr, false,  std::map<std::string, std::string>());
#endif
            m_onAcceptedCallPtr = nullptr;
        }

        m_vectDecodes.clear();
        m_totalNum--;
        m_totalDeleteNum++;

        SIPGW_LOG_IFO("******  ~SipDialog() ****** called!, SipDialogId :" + id());
    }

    void SipDialog::onTimeout()
    {
        if(m_mediaTimerExecutorPtr)
        {
            if (m_sipTimeOutType == kSipTimeoutTypeNoMedia)
            {
                m_mediaTimerExecutorPtr->mediaConnStatusTimeout(m_sipDialogId);
            }
            else if (m_sipTimeOutType == kSipTimeoutTypeCleanRes)
            {
                m_mediaTimerExecutorPtr->cleanSipResourceTimeout(m_sipDialogId);
            }
        }

        //closeTimer();
    }

    void SipDialog::pushOnAcceptedCallPtr(const tars::CurrentPtr & ptr)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_onAcceptedCallPtr)
        {
#ifdef SIPGATEWAY
            SipGatewayI::onAccepted_end(m_onAcceptedCallPtr, false,  std::map<std::string, std::string>());
#endif
            m_onAcceptedCallPtr = nullptr;
        }
        m_onAcceptedCallPtr = ptr;
    }

    const tars::CurrentPtr SipDialog::popOnAcceptedCallPtr()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        tars::CurrentPtr tmp = m_onAcceptedCallPtr;
        m_onAcceptedCallPtr = nullptr;
        return tmp;
    }


    SipDialogPtr SipDialog::create(const SipDialogIdPtr & did)
    {
        if (did == nullptr)
        {
            SIPGW_LOG_WRN("SipDialog(), INPUT did is NULL");
            return nullptr ;
        }

        if (did->isValid() == false)
        {
            SIPGW_LOG_WRN("SipDialog(), INPUT did is INVALID");
            return nullptr ;
        }
        SIPGW_LOG_IFO("{"+did->string()+"}****** Have CREATED SIP Dialog **********");
        return std::make_shared<SipDialog>(did);
    }

    const std::string SipDialog::id()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipDialogId;
    }

    bool SipDialog::getCallerUserAccount(std::string & caller)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string & user = _sipMsgDecodePart._fromPart._strUserName;
        if (user.empty())
            return false;
        char buf[256];
        snprintf(buf, sizeof(buf), "[username:%s@test.com]", user.c_str());
        caller = buf;
        return true;
    }

    bool SipDialog::getCalleeUserAccount(std::string & callee)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string & user = _sipMsgDecodePart._toPart._strUserName;
        if (user.empty())
            return false;

        char buf[256];
        snprintf(buf, sizeof(buf), "[username:%s@test.com]", user.c_str());
        callee = buf;
        return true;
    }

    bool SipDialog::getPureUserAccount(const std::string & callAccount, std::string & number)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        //get number from string like [username:018508408345@test.com]
        int pos1 = callAccount.find(":");
        int pos2 = callAccount.find("@");
        if (pos1 != -1 && pos2 != -1)
        {
            number = callAccount.substr(pos1 + 1,pos2 -pos1 -1);
            return true;
        }
        return false;
    }

    bool SipDialog::getFormatUserAccount(const std::string & pureNumber, std::string & userAccount)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string & user = _sipMsgDecodePart._toPart._strUserName;
        if (pureNumber.empty())
            return false;

        char buf[256];
        snprintf(buf, sizeof(buf), "[username:%s@test.com]", pureNumber.c_str());
        userAccount = buf;
        return true;
    }

    SipRequestMethodType SipDialog::SipMethodToSipDialogMethod(EN_SIP_METHOD enSipMethod)
    {
        switch(enSipMethod)
        {
        case EN_SIP_METHOD_INVITE:
            return kSipRequestMethodINVITE;

        case EN_SIP_METHOD_ACK:
            return kSipRequestMethodACK;

        case EN_SIP_METHOD_BYE:
            return kSipRequestMethodBYE;

        case EN_SIP_METHOD_PRACK:
            return kSipRequestMethodPRACK;

        case EN_SIP_METHOD_REGISTER:
            return kSipRequestMethodREGISTER;

        case EN_SIP_METHOD_SUBS:
            return kSipRequestMethodSUBSCRIBE;

        case EN_SIP_METHOD_NOTIFY:
            return kSipRequestMethodNOTIFY;

        case EN_SIP_METHOD_UPDATE:
            return kSipRequestMethodUPDATE;

        case EN_SIP_METHOD_MSG:
            return kSipRequestMethodMESSAGE;

        case EN_SIP_METHOD_CANCEL:
            return kSipRequestMethodCANCEL;

        case EN_SIP_METHOD_INFO:
            return kSipRequestMethodINFO;

        case EN_SIP_METHOD_OPTIONS:
            return kSipRequestMethodOPTIONS;

        default:
            return kSipRequestMethodInvalid;
        }
    }

    EN_SIP_METHOD SipDialog:: SipDialogMethodToSipMethod(SipRequestMethodType enSipMethod)
    {
        switch(enSipMethod)
        {
        case kSipRequestMethodINVITE:
            return EN_SIP_METHOD_INVITE ;

        case kSipRequestMethodACK :
            return EN_SIP_METHOD_ACK;

        case kSipRequestMethodBYE :
            return EN_SIP_METHOD_BYE;

        case kSipRequestMethodPRACK :
            return EN_SIP_METHOD_PRACK;

        case kSipRequestMethodREGISTER :
            return EN_SIP_METHOD_REGISTER;

        case kSipRequestMethodSUBSCRIBE :
            return EN_SIP_METHOD_SUBS;

        case kSipRequestMethodNOTIFY :
            return EN_SIP_METHOD_NOTIFY;

        case kSipRequestMethodUPDATE :
            return EN_SIP_METHOD_UPDATE;

        case kSipRequestMethodMESSAGE :
            return EN_SIP_METHOD_MSG;

        case kSipRequestMethodCANCEL:
            return EN_SIP_METHOD_CANCEL;

        case kSipRequestMethodINFO:
            return EN_SIP_METHOD_INFO;

        case kSipRequestMethodOPTIONS:
            return EN_SIP_METHOD_OPTIONS;

        default:
            return EN_SIP_METHOD_EXT;
        }
    }

    int SipDialog::SipDialogResponseStatusToSipStatus(SipResponseCodeType enSipDialogStatus)
    {
        switch(enSipDialogStatus)
        {
        case kSipResponseCode100Trying:
            return 100;

        case kSipResponseCode180Ringing:
            return 180;

        case kSipResponseCode183SessionProgress:
            return 183;

        case kSipResponseCode200OK:
            return 200;

        case kSipResponseCode202:
            return 202;

        case kSipResponseCode1xx:
            return 181;

        case kSipResponseCode2xx:
            return 201;

        case kSipResponseCode400:
            return 400;

        case kSipResponseCode401:
            return 401;

        case kSipResponseCode403:
            return 403;

        case kSipResponseCode404:
            return 404;

        case kSipResponseCode407:
            return 407;

        case kSipResponseCode408:
            return 408;

        case kSipResponseCode423:
            return 423;

        case kSipResponseCode480:
            return 480;

        case kSipResponseCode481:
            return 481;

        case kSipResponseCode482:
            return 482;

        case kSipResponseCode486:
            return 486;

        case kSipResponseCode487:
            return 487;

        case kSipResponseCode488:
            return 488;

        case kSipResponseCode491:
            return 491;

        case kSipResponseCode4xx:
            return 499;

        case kSipResponseCode500:
            return 500;

        case kSipResponseCode5xx:
            return 599;

        case kSipResponseCode604:
            return 604;

        case kSipResponseCodeInvalid:
            return -1;

        default:
            return -1;
        }
    }

    SipResponseCodeType SipDialog::SipStatusToSipDialogResponseStatus(int iStatus)
    {
        switch(iStatus)
        {
        case 100:
            return kSipResponseCode100Trying;

        case 180:
            return kSipResponseCode180Ringing;

        case 183:
            return kSipResponseCode183SessionProgress;

        case 200:
            return kSipResponseCode200OK;

        case 202:
            return kSipResponseCode202;

        case 400:
            return kSipResponseCode400;

        case 401:
            return kSipResponseCode401;

        case 403:
            return kSipResponseCode403;

        case 404:
            return kSipResponseCode404;

        case 407:
            return kSipResponseCode407;

        case 408:
            return kSipResponseCode408;

        case 423:
            return kSipResponseCode423;

        case 480:
            return kSipResponseCode480;

        case 481:
            return kSipResponseCode481;

        case 482:
            return kSipResponseCode482;

        case 486:
            return kSipResponseCode486;

        case 487:
            return kSipResponseCode487;

        case 488:
            return kSipResponseCode488;

        case 491:
            return kSipResponseCode491;

        case 500:
            return kSipResponseCode500;

        case 604:
            return kSipResponseCode604;

        default:
            if (iStatus >= 181 && iStatus < 183)
            {
                return kSipResponseCode1xx;
            }
            else if (iStatus > 200 && iStatus < 300)
            {
                return kSipResponseCode2xx;
            }
            else if (iStatus > 400 && iStatus < 500)
            {
                return kSipResponseCode4xx;
            }
            else if (iStatus >= 501 && iStatus < 600)
            {
                return kSipResponseCode5xx;
            }
            else
            {
                return kSipResponseCodeInvalid;
            }
        }
    }

    void SipDialog::setCallReleaseType(const CallReleaseType & releaseType)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipToRpcReleaseType = releaseType;
    }
    CallReleaseType SipDialog::getSipToRpcReleaseType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipToRpcReleaseType;
    }

    void SipDialog::setSipDialogStatus(const SipDialogState & dialogStatus)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_sipDialogStatus >= dialogStatus)
            return;

        m_sipDialogStatus = dialogStatus;
        m_curStatusInitTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }

    SipDialogState SipDialog::getSipDialogStatus()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipDialogStatus;
    }

    void SipDialog::setCurStatusInitTime(const int64_t & lInitTime)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_curStatusInitTime = lInitTime;
    }
    int64_t SipDialog::getCurStatusInitTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_curStatusInitTime;
    }

    void SipDialog::setSipCallRpcProp(bool bSipCallRpc)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bSipCallRpc = bSipCallRpc;
    }
    bool SipDialog::getSipCallRpcProp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bSipCallRpc;
    }

    void SipDialog::setReleaseBySipApp(bool bBySip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bReleaseBySipApp = bBySip;
    }

    bool SipDialog::getReleaseBySipApp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bReleaseBySipApp;
    }

    void SipDialog::setReleaseType(ReleaseType releaseType)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_releaseType = releaseType;
    }
    
    ReleaseType SipDialog::getReleaseType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_releaseType; 
    }

    void SipDialog::setTimeOutType(TimeOutType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_timeOutType = type;
        if (type != TimeOutTypeExceptionDealTimeOut)
            m_enterExceptionTime = tars::TC_TimeProvider::getInstance()->getNowMs();
        TLOGINFO("[SipDialog] " << __FUNCTION__ << "() called. sipDialog id:" << m_sipDialogId << ",type:" << int(type) << endl);
    }

    TimeOutType SipDialog::getTimeOutType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_timeOutType;
    }

    int64_t SipDialog::getEnterExceptionTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_enterExceptionTime;
    }

    int64_t SipDialog::getCreateTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_createTime;
    }

    std::atomic<int>  SipDialog::m_totalNum(0);
    std::atomic<int>  SipDialog::m_totalNewNum(0);
    std::atomic<int>  SipDialog::m_totalDeleteNum(0);
    std::atomic<int>  SipDialog::m_totalExceptionNum(0);

    void SipDialog::PirntOutSipDialogNum()
    {
        TLOGINFO("[SipDialog] " << __FUNCTION__ << "() called, Total Num is : " << m_totalNum << endl);
    }

    void SipDialog::autoAddExceptionNum()
    {
        m_totalExceptionNum++;
    }

    void SipDialog::setSipGatewayId(const std::string & gwId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSipGatewayId = gwId;
    }

    void SipDialog::setSipProxyId(const std::string & spId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSipProxyId = spId;
    }

    std::string SipDialog::getSipGatewayId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSipGatewayId;
    }

    std::string SipDialog::getSipProxyId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSipProxyId;
    }

    AuthType SipDialog::getAuthType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_authType;
    }

    void SipDialog::setAuthType(AuthType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_authType = type;
    }

    SDPNegotiateStatus SipDialog::getSdpNegotiateStatus()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sdpNegoStatus;
    }

    void SipDialog::setSdpNegotiateStatus(SDPNegotiateStatus status)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sdpNegoStatus = status;
    }

    int SipDialog::getAcceptRpcResourceParameter()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_bSipCallRpc)
        {
            switch(m_sdpNegoStatus)
            {
            case SDPInit:
                return 0;

            case SDPNegotiating:
                return 1;                   //�收/发 offer，协商中， 分配CP

            case SDPNegotiated:             //收/发 answer,协商完成，设置SBC媒体流接收端口和ip
                return 2;
            }
        }
        return 0;
    }

    bool SipDialog::getSipToRpcACKWithSDP()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bSipToRpcAckWithSDP;
    }
    void SipDialog::setSipToRpcACKWithSDP(bool bWith)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bSipToRpcAckWithSDP = bWith;
    }

    void SipDialog::formatDomainUserAccount(const std::string & relationType,const std::string & nationCode,
        const std::string & sipAccount, const std::string & domainId,const std::string & domainName,  bool caller)
    {
        std::string temRelationTYpe(relationType) ;
        std::string temSipAccount(sipAccount);
        std::string temDomainName(domainName);

        std::lock_guard<std::mutex> lock(m_mutex);
        std::string domainAccount;
        domainAccount = "[" + trim(temRelationTYpe);
        domainAccount += ":";
        domainAccount += trim(temSipAccount);
        domainAccount += "@";
        domainAccount += trim(temDomainName);
        domainAccount += "]";

        if (caller)
            m_strCallerUserAccount = domainAccount.c_str();
        else
            m_strCalleeUserAccount = domainAccount.c_str();
    }

    void SipDialog::setDomainUserAccount(const std::string & account, bool bCaller)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (bCaller)
            m_strCallerUserAccount = account;
        else
            m_strCalleeUserAccount = account;
    }

    void SipDialog::getDomainUserAccount(std::string & account, bool bCaller)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (bCaller)
            account = m_strCallerUserAccount ;
        else
            account = m_strCalleeUserAccount;
    }

    void SipDialog::setCallNumber(const std::string & number, bool bCaller)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (bCaller)
            m_strCallerNumber = number;
        else
            m_strCalleeNumber = number;
    }

    std::string SipDialog::getCallNumber(bool bCaller)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return bCaller ? m_strCallerNumber : m_strCalleeNumber;
    }

    void SipDialog::setChargeAccount(const std::string & number, bool bCaller)
    {
        if (bCaller)
            m_strCallBackCallerNumber = number;
        else
            m_strCallBackCalleeNumber = number;
    }

    std::string SipDialog::getChargeAccount(bool bCaller)
    {
        return bCaller ? m_strCallBackCallerNumber : m_strCallBackCalleeNumber;
    }

    void SipDialog::setISBCAccessed(bool bISBCAccessed)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bISBCAccessed = bISBCAccessed;
    }

    bool SipDialog::getISBCAccessed()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bISBCAccessed;
    }

    void SipDialog::setToSBCConnectionId(const std::string & connectionId)
    {
        if (connectionId.empty())
            return;

        std::lock_guard<std::mutex> lock(m_mutex);
        m_strToSBCConnectionId = connectionId;
    }

    std::string SipDialog::getToSBCConnectionId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strToSBCConnectionId ;
    }

    void SipDialog::setSbcIp(const std::string & sbcIp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSbcIp = sbcIp;
    }

    std::string SipDialog::getSbcIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSbcIp;
    }

    void SipDialog::setIsFreshDialog(bool isFreshDialog)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bRefreshDialog =  isFreshDialog;
    }
    bool SipDialog::getIsFreshDialog()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bRefreshDialog ;
    }

    void SipDialog::setLastRefreshTime(int64_t refreshTime)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_lLastRefreshTime = refreshTime;
    }
    int64_t SipDialog::getLastFreshTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_lLastRefreshTime;
    }

    void SipDialog::RepeatTimeAutoIncrease()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iReSendTimes ++;
    }

    int SipDialog::getReSendTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iReSendTimes;
    }

    void SipDialog::reSetRepeatTimes(SipRequestMethodType reSendMethod)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (reSendMethod != m_reSendMethod)
            return;

        TLOGDEBUG("[SipDialog] " << std::string(__FUNCTION__) + "() called : cur Times:" +  TC_Common::tostr(m_iReSendTimes) << endl);
        m_iReSendTimes = 0;
    }

    void SipDialog::setReSendMethod(SipRequestMethodType reSendMethod)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_reSendMethod = reSendMethod;
    }
    SipRequestMethodType SipDialog::getReSendMethod()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_reSendMethod ;
    }

    void SipDialog::setSendTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_lSendTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }
    int64_t SipDialog::getSendTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_lSendTime;
    }

    void SipDialog::setEarlySessionTime()
    {
        if (!m_lEarlySessionTime)
            m_lEarlySessionTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }
    int64_t SipDialog::getEarlySessionTime()
    {
        return m_lEarlySessionTime;
    }

    void SipDialog::setRingTime()
    {
        if (!m_lRingTime)
            m_lRingTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }
    int64_t SipDialog::getRingTime()
    {
        return m_lRingTime;
    }

    void SipDialog::setAnswerTime()
    {
        if (!m_lAnswerTime)
            m_lAnswerTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }
    int64_t SipDialog::getAnswerTime()
    {
        return m_lAnswerTime;
    }

    void SipDialog::setTerminateTime()
    {
        if (!m_lTerminateTime)
            m_lTerminateTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }
    int64_t SipDialog::getTerminateTime()
    {
        return m_lTerminateTime;
    }

    void SipDialog::setCallerNumber(const std::string & number)
    {
        m_strCallerNumber = number;
    }

    std::string SipDialog::getCallerNumber()
    {
        return m_strCallerNumber;
    }

    void SipDialog::setCalleeNumber(const std::string& number)
    {
        m_strCalleeNumber = number;
    }
    std::string SipDialog::getCalleeNumber()
    {
        return m_strCalleeNumber;
    }

    void SipDialog::setDisplayNumber(const std::string& number)
    {
        m_strDisplayNumber = number;
    }
    std::string SipDialog::getDisplayNumber()
    {
        return m_strDisplayNumber;
    }

    void SipDialog::setSbcMediaIp(const std::string& ip)
    {
        m_strSbcMediaIp = ip;
    }
    std::string SipDialog::getSbcMediaIp()
    {
        return m_strSbcMediaIp;
    }

    void SipDialog::setRtcMediaIp(const std::string& ip)
    {
        m_strRtcMediaIp = ip;
    }
    std::string SipDialog::getRtcMediaIp()
    {
        return m_strRtcMediaIp;
    }
    void SipDialog::setRtcIp(const std::string& ip)
    {
        m_strRtcIp = ip;
    }
    std::string SipDialog::getRtcIp()
    {
        return m_strRtcIp;
    }
    void SipDialog::setRtcCC(const std::string& cc)
    {
        m_strRtcCC = cc;
    }
    std::string SipDialog::getRtcCC()
    {
        return m_strRtcCC;
    }

    void SipDialog::setCallerPrefix(const std::string& prefix)
    {
        m_strCallerPrefix = prefix;
    }
    std::string SipDialog::getCallerPrefix()
    {
        return m_strCallerPrefix;
    }

    void SipDialog::setCalleePrefix(const std::string& prefix)
    {
        m_strCalleePrefix = prefix;
    }
    std::string SipDialog::getCalleePrefix()
    {
        return m_strCalleePrefix;
    }

    void SipDialog::setReSendBye(bool bReSendBye)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bReSendBye = bReSendBye;
    }
    bool SipDialog::getReSendBye()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bReSendBye;
    }

    void SipDialog::setRpcCallId(int64_t rcpCallId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_lRpcCallId = rcpCallId;
    }

    int64_t SipDialog::getRpcCallId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_lRpcCallId;
    }

    void SipDialog::setDomainIdAppId(const std::string & domainIdAppId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strDomainIdAppId = domainIdAppId;
    }

    std::string SipDialog::getDomainIdAppId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strDomainIdAppId;
    }

    void  SipDialog::setCoreNetId(const std::string & coreNetid)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCoreNetId = coreNetid;
    }

    std::string SipDialog::getCoreNetId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCoreNetId;
    }

    void SipDialog::setSessionFreshDilogFlag(bool bFreshDialog)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bSessionFreshDialog = bFreshDialog;
    }

    bool SipDialog::getSessionFreshDilogFlag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bSessionFreshDialog;
    }

    void SipDialog::setRefreshMethod(EN_SIP_METHOD refreshMethod)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_refreshMethod = refreshMethod;
    }

    EN_SIP_METHOD SipDialog::getRefreshMethod()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_refreshMethod;
    }

    void SipDialog::setAnseredTime(int64_t time)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_answeredTime)
            m_answeredTime = time;
    }

    int64_t SipDialog::getAnseredTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_answeredTime;
    }

    void SipDialog::setReleaseTime(int64_t time)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_releaseTime)
            m_releaseTime = time;
    }

    int64_t SipDialog::getReleaseTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_releaseTime;
    }

    void SipDialog::setMediaType(MediaType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_mediaType = type;
    }

    MediaType SipDialog::getMediaType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_mediaType;
    }

    void SipDialog::setSDPAnsweredType(SDPAnsweredType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_appToCsSDPAnsweredType != kSDPUnAnswered)
            return;
        m_appToCsSDPAnsweredType = type;
    }

    SDPAnsweredType SipDialog::getSDPAnsweredType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_appToCsSDPAnsweredType;
    }

    void SipDialog::setCalleeRtpGWCp(const std::string & calleeCp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_rtpGwCp = calleeCp;
    }
    std::string SipDialog::getCalleeRtpGwCp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_rtpGwCp;
    }

    void SipDialog::setRtpGwIp(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_rtpGwIp = ip;
    }
    std::string SipDialog::getRtpGwIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_rtpGwIp;
    }

    void SipDialog::setSBCIp(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sbcIp = ip;
    }

    std::string SipDialog::getSBCIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sbcIp;
    }

    void SipDialog::setSdpOffer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sdpOffer = sdp;
    }

    std::string SipDialog::getSdpOffer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sdpOffer;
    }

    void SipDialog::setSdpAnswer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sdpAnswer = sdp;
    }
    std::string SipDialog::getSdpAnswer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sdpAnswer;
    }

    void SipDialog::setAppSdpOffer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_appSdpOffer = sdp;
    }

    std::string SipDialog::getAppSdpOffer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_appSdpOffer;
    }

    void SipDialog::setAppSdpAnswer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_appSdpAnswer = sdp;
    }
    std::string SipDialog::getAppSdpAnswer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_appSdpAnswer;
    }

    void SipDialog::setSipSdpOffer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipSdpOffer = sdp;
    }

    std::string SipDialog::getSipSdpOffer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipSdpOffer;
    }

    void SipDialog::setSipSdpAnswer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipSdpAnswer = sdp;
    }
    std::string SipDialog::getSipSdpAnswer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipSdpAnswer;
    }

    void SipDialog::setSdpIp(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSdpIp = ip;
    }

    std::string SipDialog::getSdpIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSdpIp;
    }

    void SipDialog::setContent(const std::string & content)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_content = content;
    }
    std::string SipDialog::getContent()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_content;
    }

    void SipDialog::setEndpoint(const std::string & endpoint)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_endpoint = endpoint;
    }
    std::string SipDialog::getEndpoint()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_endpoint;
    }

    void SipDialog::setRouterHost(const std::string & host)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_routerHost = host;
    }
    std::string SipDialog::getRouterHost()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_routerHost;
    }

    void SipDialog::setRouterPort(int port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_routerPort = port;
    }
    int SipDialog::getRouterPort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_routerPort;
    }

    void SipDialog::setRtpAllocedByGw(bool bySipGw)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_rtpAllocBySipGw = bySipGw;
    }

    bool SipDialog::getRtpAllocedByGw()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_rtpAllocBySipGw;
    }

    void SipDialog::setSBCSupportUpdate(bool bSupport)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sbcSupportUpdate = bSupport;
    }

    bool SipDialog::getSBCSupportUpdate()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sbcSupportUpdate;
    }

    void SipDialog::setProtocolType(unsigned char tptType)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        _sipMsgDecodePart._viaPart.setTptType(tptType);
    }
    unsigned char SipDialog::getProtocolType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return _sipMsgDecodePart._viaPart.getTptType();
    }

    void SipDialog::setCallFlow(bool bNewFlow)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_newCallFlow = bNewFlow;
    }
    bool SipDialog::getCallFlow()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_newCallFlow;
    }

    void SipDialog::setCallServerExOid(const std::string & id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_callServerExOid = id;
    }

    std::string SipDialog::getCallServerExOid()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_callServerExOid;
    }

    void SipDialog::setSDPAnswerSetFlag(bool setFlag)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sdpAnserSet = setFlag;
    }

    bool SipDialog::getSDPAnswerSetFlag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sdpAnserSet;
    }

    void SipDialog::setHoldOnFlag(bool holdon)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bHoldOn = holdon;
    }

    bool SipDialog::getHoldOnFlag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bHoldOn;
    }

    void SipDialog::insertAppEpAndSdp(const std::string & ep, const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutexEpSdps);
        m_mapEpSdps[ep] = sdp;
    }

    std::string SipDialog::getAppSdp(const std::string & ep)
    {
        std::lock_guard<std::mutex> lock(m_mutexEpSdps);
        std::map<std::string, std::string>::iterator iterEp = m_mapEpSdps.find(ep);
        return iterEp == m_mapEpSdps.end() ? "" : iterEp->second;
    }

    void SipDialog::setRtpGwHostPorts(const std::string & hostPorts)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_hostPorts = hostPorts;
    }

    std::string SipDialog::getRtpGwHostPorts()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_hostPorts;
    }

    void SipDialog::setIsPeerNewSdk(bool isNewSdk)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isNewSdk = isNewSdk;
    }
    bool SipDialog::getIsPeerNewSdk()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_isNewSdk;
    }

    void SipDialog::setOriginDialogId(const std::string & id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_originDialogId = id;
    }
    std::string SipDialog::getOriginDialogId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_originDialogId;
    }

    void SipDialog::setRemainMinute(int minutes)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_remainMinute = minutes;
    }
    int SipDialog::getRemainMinute()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_remainMinute;
    }

    std::string SipDialog::getSipCallId()
    {
        return _sipMsgDecodePart._callIdPart.getCallIdString();
    }

    void SipDialog::setOriginCalleeNum(const std::string & number)
    {
        m_originCalleeNumber = number;
    }
    std::string SipDialog::getOriginCalleeNum()
    {
        return m_originCalleeNumber;
    }

    void SipDialog::setSipClientFakeIp(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipClientFakeIp = ip;
    }

    std::string SipDialog::getSipClientFakeIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipClientFakeIp;
    }

    void SipDialog::setRtpGwArcOid(const std::string & oid)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_rtpGwArcOid = oid;
    }
    std::string SipDialog::getRtpGwArcOid()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_rtpGwArcOid;
    }

    void SipDialog::setRtpGwId(const std::string & oid)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_rtpGwId = oid;
    }
    std::string SipDialog::getRtpGwId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_rtpGwId;
    }

    void SipDialog::pushbackDecodeObj(const SipMsgDecodePtr & obj)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_vectDecodes.push_back(obj);
    }

    void SipDialog::setCalleeNationCode(const std::string & code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_calleeNationCode = code;
    }
    std::string SipDialog::getCalleeNationCode()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_calleeNationCode;
    }

    void SipDialog::addFailedTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_failedTimes ++;
    }

    int SipDialog::getFailedTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_failedTimes;
    }

    void SipDialog::setParams(const std::map<std::string, std::string> & params)
    {
        m_params = params;
    }

    std::map<std::string, std::string> SipDialog::getParams()
    {
        return m_params;
    }

    std::string SipDialog::getCallIdHost()
    {
        return _sipMsgDecodePart._callIdPart.getCallIdString();
    }

    void SipDialog::setCallerUid(const std::string & uid)
    {
        m_callerUid = uid;
    }

    std::string SipDialog::getCallerUid()
    {
        return m_callerUid;
    }

    void SipDialog::setRseqUpdate()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iRseq++;
    }

    int SipDialog::getRseqUpdate()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iRseq;
    }

    void SipDialog::setOCSErrorCode(const std::string & code)
    {
        m_ocsErrorCode = code;
    }
    std::string SipDialog::getOCSErrorCode()
    {
        return m_ocsErrorCode;
    }

    void SipDialog::setChargeType(ChargeType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_chargeType = type;
    }

    ChargeType SipDialog::getChargeType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_chargeType;
    }

    void SipDialog::setBlacklistType(const std::string& type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_blacklistType = type;
    }

    std::string SipDialog::getBlacklistType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_blacklistType;
    }

    void SipDialog::setInterceptType(const std::string& type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_interceptType = type;
    }

    std::string SipDialog::getInterceptType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_interceptType;
    }

    void SipDialog::setFrequency(int frequency)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_frequency = frequency;
    }

    int SipDialog::getFrequency()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_frequency;
    }

    void SipDialog::setCalleeRealm(const std::string & realm)
    {
        m_calleeRealm = realm;
    }

    std::string SipDialog::getCalleeRealm()
    {
        return m_calleeRealm;
    }

    void SipDialog::setCallSbcIps(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutexCallSbcIps);
        m_vectCallSbcIps.push_back(ip);
    }

    void SipDialog::getCallSbcIps(vector<std::string> & vectIps)
    {
        std::lock_guard<std::mutex> lock(m_mutexCallSbcIps);
        vectIps = m_vectCallSbcIps;
    }


    bool SipDialog::setAlertedFlag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        bool ret = m_bAlertedFlag ? false : true;
        m_bAlertedFlag = true;
        return ret;
    }

    void SipDialog::setMediaCheckFlag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bMediaChecked = true;
    }
    int SipDialog::getMediaCheckFlag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bMediaChecked;
    }

    void SipDialog::setMediaConnFailedReason(const std::string & reason)
    {
        m_mediaConnFailedReason = reason;
    }
    std::string SipDialog::getMediaConnFailedReason()
    {
        return m_mediaConnFailedReason;
    }

    void SipDialog::setByeReason(const std::string & reason)
    {
        m_byeReason = reason;
    }
    std::string SipDialog::getByeReason()
    {
        return m_byeReason;
    }

    // bool SipDialog::createTimer(const Common::ApplicationExPtr & application, int time, SipTimeoutType type)
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (!m_waitTimer)
    //     {
    //         m_sipTimeOutType = type;
    //         m_waitTimer = application->createTimer(this);
    //         m_waitTimer->start(time);
    //         return true;
    //     }

    //     return false;
    // }

    // void SipDialog::reCreateTimer(const Common::ApplicationExPtr & application, int time, SipTimeoutType type)
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (m_waitTimer)
    //         m_waitTimer->close();

    //     m_sipTimeOutType = type;
    //     m_waitTimer = application->createTimer(this);
    //     m_waitTimer->start(time);
    // }

    SipTimeoutType SipDialog::getSipTimeOutType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipTimeOutType;
    }

    // void SipDialog::closeTimer()
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (m_waitTimer)
    //     {
    //         m_sipTimeOutType = kSipTimeoutTypeNone;
    //         m_waitTimer->close();
    //         m_waitTimer = nullptr;
    //     }
    // }

    // void SipDialog::stopTimer()
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (m_waitTimer)
    //     {
    //         m_waitTimer->stop();
    //     }
    // }

    // void SipDialog::createReleaseTimer(const Common::ApplicationExPtr & application, int time)
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (!m_releaseTimer)
    //     {
    //         m_releaseTimer = application->createTimer(this);
    //         m_releaseTimer->start(time);
    //     }
    // }

    // void SipDialog::closeReleaseTimer()
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (m_releaseTimer)
    //     {
    //         m_releaseTimer->close();
    //         m_releaseTimer = nullptr;
    //     }
    // }

    void SipDialog::setErrorCode(int code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iErrorCode = code;
    }
    int SipDialog::getErrorCode()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iErrorCode; 
    }

    void SipDialog::setMediaErrorCode(int code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iMediaCode = code;
    }
    int SipDialog::getMediaErrorCode()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iMediaCode; 
    }

    void SipDialog::setOtherErrorCode(int code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iOtherCode = code;
    }
    int SipDialog::getOtherErrorCode()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iOtherCode; 
    }

    void SipDialog::setErrorCauseCode(int code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iCauseCode = code;
    }
    int SipDialog::getErrorCauseCode()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iCauseCode; 
    }

    //...................AcdGateway
    void SipDialog::setCallSerialNum(const std::string & num)
    {
        m_callSerialNum = num;
    }
    std::string SipDialog::getCallSerialNum()
    {
        return m_callSerialNum;
    }

    void SipDialog::setCcsvrOid(const std::string & id)
    {
        m_ccsvrOid = id;
    }
    std::string SipDialog::getCcsvrOid()
    {
        return m_ccsvrOid;
    }

    void SipDialog::setConfId(const std::string & id)
    {
        m_confId = id;
    }
    std::string SipDialog::getConfId()
    {
        return m_confId;
    }

    void SipDialog::setRoomId(const std::string & id)
    {
        m_roomId = id;
    }
    std::string SipDialog::getRoomId()
    {
        return m_roomId;
    }

    void SipDialog::setServiceTelNum(const std::string & num)
    {
        m_serviceTelNum = num;
    }
    std::string SipDialog::getServiceTelNum()
    {
        return m_serviceTelNum;
    }

    void SipDialog::setUuiData(const std::string & val)
    {
        m_uuiData = val;
    }

    std::string SipDialog::getUuiData()
    {
        return m_uuiData;
    }

    void SipDialog::setDialogRole(DialogRole role)
    {
        m_dialogRole = role;
    }

    DialogRole SipDialog::getDialogRole()
    {
        return m_dialogRole;
    }

    void SipDialog::setCallerSession(const std::string & session)
    {
        m_callerSession = session;
    }
    std::string SipDialog::getCallerSession()
    {
        return m_callerSession;
    }

    void SipDialog::setConfServerOid(const std::string & oid)
    {
        m_confServerOid = oid;
    }
    std::string SipDialog::getConfServerOid()
    {
        return m_confServerOid;
    }

    void SipDialog::setJsmSerialNum(const std::string & no)
    {
        m_jsmSerialNo = no;
    }
    std::string SipDialog::getJsmSerialNum()
    {
        return m_jsmSerialNo;
    }

    void SipDialog::setTokenVal(const std::string & val)
    {
        m_tokenVal = val;
    }
    std::string SipDialog::getTokenVal()
    {
        return m_tokenVal;
    }

    void SipDialog::insertPorts(const vector<int> & ports)
    {
        m_ports = ports;
    }

    vector<int> SipDialog::getPorts()
    {
        return m_ports;
    }

    void SipDialog::setDomainRealm(const std::string & realm)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_realm = realm;
    }

    std::string SipDialog::getDomainRealm()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_realm;
    }

    void SipDialog::setSipGatewayPtr(const MediaTimerExecutorPtr &ptr)
    {
        m_mediaTimerExecutorPtr = ptr;
    }

    void SipDialog::setMediaTimeOut(int time)
    {
        m_acceptedNoMediaTimeout = time;
    }

    bool SipDialog::encodeDomainIdAppId(const std::string & domainId, const std::string & appId,std::string & domainIdAppId)
    {
        if (domainId.empty() || appId.empty())
            return false;

        domainIdAppId = domainId + ":" + appId;
        return true;
    }

    bool SipDialog::decodeDomainIdAppId(const std::string & domainIdAndAppId, std::string & domainId, std::string & appId)
    {
        //format :  9999:0
        int  pos = domainIdAndAppId.find(":");
        if (pos < 0)
            return false;
        domainId = domainIdAndAppId.substr(0,pos);
        appId = domainIdAndAppId.substr(pos + 1, domainIdAndAppId.size() - (pos + 1));
        return true;
    }

    void SipDialog::decodeAccountId(const std::string & accountId, std::string & identity, std::string & domain, std::string & type, bool & isUserId)
    {
        identity.clear();
        domain.clear();
        type.clear();
        isUserId = false;

        int pos1,pos2,size;

        size = accountId.size();
        if (size < 2 || accountId[0] != '[' || accountId[size-1] != ']')
        {
            identity = accountId;
            isUserId = true;
            return;
        }

        pos1 = accountId.find(':',1);
        if (pos1 < 0)
        {
            pos1 = accountId.rfind('@');
            if (pos1 < 0)
            {
                identity = accountId.substr(1,size-2);
                isUserId = false;
                return;
            }
            identity = accountId.substr(1,pos1-1);
            domain = accountId.substr(pos1+1,size-pos1-2);
            isUserId = false;
            return;
        }

        type = accountId.substr(1,pos1-1);pos1++;
        if (type == "uid")
        {
            identity = accountId.substr(pos1,size-pos1-1);
            isUserId = true;

            size = identity.size();
            if (size < 2 || (identity[0] == '<' && identity[size-1] == '>'))
                return;
            pos1 = identity.find('@');
            if (pos1 < 0) return;

            domain = identity.substr(pos1+1);
            identity = identity.substr(0,pos1);
            return;
        }

        isUserId = false;
        if (type == "email")
        {
            pos2 = accountId.find('@',pos1);
            if (pos2 < 0 || (pos2 = accountId.find('@',pos2+1)) < 0)
            {
                identity = accountId.substr(pos1,size-pos1-1);
                return;
            }
            identity = accountId.substr(pos1,pos2-pos1);
            domain = accountId.substr(pos2+1,size-pos2-2);
            return;
        }
        pos2 = accountId.find('@',pos1);
        if (pos2 < 0)
        {
            identity = accountId.substr(pos1,size-pos1-1);
            return;
        }
        identity = accountId.substr(pos1,pos2-pos1);
        domain = accountId.substr(pos2+1,size-pos2-2);
    }

    void SipDialog::decodeKeyValPairsConfig(const std::string & cfg, std::map<std::string, std::string> & pairs)
    {
        //"102286:0-12002;102303:1-1000";
        if (cfg.empty())
            return;

        int pos1 = cfg.find("-");
        int pos2 = cfg.find(";");
        if (pos1 < 0)
            return;

        int curPos = 0;
        while (pos1 > 0)
        {
            std::string subStr;
            if (pos2 > 0)
                subStr = cfg.substr(curPos, pos2 - curPos);
            else
                subStr = cfg.substr(curPos);

            pos1 = subStr.find("-");
            std::string domainId = subStr.substr(0, pos1);
            std::string defaultCaller = subStr.substr(pos1 + 1);
            pairs.insert(make_pair(domainId, defaultCaller));

            if (pos2 < 0)
                break;

            curPos = pos2 + 1;
            pos1 = cfg.find(":", curPos);
            pos2 = cfg.find(";", curPos);
        }
    }

    void SipDialog::decodeKeyValPairsConfig(const std::string& cfg, std::vector<std::string>& pairs)
    {
        if (cfg.empty())
            return;

        int pos1 = cfg.find(";");
        if (pos1 < 0)
            return;

        int curPos = 0;
        while (pos1 > 0)
        {
            std::string prefix;
            prefix = cfg.substr(curPos, pos1 - curPos);
            pairs.push_back(prefix);
            curPos = pos1 + 1;
            pos1 = cfg.find(";", curPos);
        }
    }

    //------------------------------------------------

    int64_t MappingTable::rpcCallId(const std::string & didStr)
    {
        if(didStr.empty())
            return -1;

        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if(did == nullptr)
            return -1;

        if(did->isValid() == false)
            return -1;

        std::lock_guard<std::mutex> lock(_mutex);
        std::lock_guard<std::mutex> temlock(_mappingTableSipToRpcMutex);

        SipToRpcMappingType::const_iterator it = _mappingTableSipToRpc.find(didStr);
        if(it == _mappingTableSipToRpc.end())
            return -1;

        return it->second;
    }

    std::string MappingTable::sipDialogId(int64_t cid)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::lock_guard<std::mutex> temlock(_mappingTableRpcToSipMutex);
        RpcToSipMappingType::const_iterator it = _mappingTableRpcToSip.find(cid);
        if (it == _mappingTableRpcToSip.end())
            return "";

        return (it->second);
    }

    bool MappingTable::insertMappingItem(int64_t cid, const std::string & didStr)
    {
        if (cid <= 0)
        {
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + "(),cid INVALID" << endl);
            return false;
        }

        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if (did == nullptr)
        {
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + "(),did NULL" << endl);
            return false;
        }

        if (did->isValid() == false)
        {
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + ", did INVALID" << endl);
            return false;
        }

        std::lock_guard<std::mutex> lock(_mutex);

        const int64_t callIdExists = rpcCallId(didStr);
        if (callIdExists >= 0) //has one
        {
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + ", already has one mapping item for SipDialogId:["+did->string()+"->"+TC_Common::tostr(callIdExists)+"]" + ",new callid:" + TC_Common::tostr(cid) << endl);
            return false;
        }

        std::string dialogIdExists = sipDialogId(cid);
        if (dialogIdExists.empty() == false) //has one
        {
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + ", already has one mapping item for RpcCallId:["+TC_Common::tostr(cid)+"->"+ dialogIdExists +"]" + ",new dialogId:" + didStr << endl);
            return false;
        }

        int count1 =0, count2=0;
        {
            std::pair<SipToRpcMappingType::iterator, bool > ret1;
            if (1)
            {
                std::lock_guard<std::mutex> lock(_mappingTableSipToRpcMutex);
                std::pair<SipToRpcMappingType::iterator, bool > ret1 = _mappingTableSipToRpc.insert(SipToRpcMappingType::value_type(didStr, cid));
                if(ret1.second == false)
                {
                    TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + ", insert FAILED for item for SipDialogId:["+didStr+"->"+TC_Common::tostr(cid)+"]" << endl);
                    return false;
                }
                _sipToRpcNum ++;
                count1 = _mappingTableSipToRpc.size();
            }

            if (1)
            {
                std::lock_guard<std::mutex> lock(_mappingTableRpcToSipMutex);
                std::pair<RpcToSipMappingType::iterator, bool > ret2 = _mappingTableRpcToSip.insert(RpcToSipMappingType::value_type(cid, didStr));
                if (ret2.second == false)
                {
                    TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + ", insert FAILED for item for RpcCallId:["+TC_Common::tostr(cid)+"->"+didStr+"]" << endl);

                    if (1) //rollback
                    {
                        TLOGDEBUG("[SipDialog] " << std::string(__FUNCTION__) + ", Roll back to erase Item:["+ret1.first->first+"->"+ TC_Common::tostr(ret1.first->second)+"]" << endl);
                        std::lock_guard<std::mutex> lock(_mappingTableSipToRpcMutex);
                        _mappingTableSipToRpc.erase(ret1.first);
                    }
                    return false;
                }
                _rpcToSipNum ++;
                count2 = _mappingTableRpcToSip.size();
            }
        }

        if (count1 != count2)
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + ", Count Sip2Rpc("+TC_Common::tostr(count1)+") != Count Rpc2Sip("+TC_Common::tostr(count2)+")" << endl);

        TLOGDEBUG("[SipDialog] " << std::string(__FUNCTION__) + ", insert a  item, callId : " + TC_Common::tostr(cid) + ", sipDialogID : " + didStr << endl);
        return true;
    }

    bool MappingTable::deleteMappingItem(const int64_t cid)
    {
        if (cid < 0)
            return false;

        std::lock_guard<std::mutex> lock(_mutex);
        int count1 =0, count2=0;
        RpcToSipMappingType::iterator it = _mappingTableRpcToSip.find(cid);
        if (it != _mappingTableRpcToSip.end())
        {
            if (it->second.empty())
                return false;

            SipToRpcMappingType::iterator it2 = _mappingTableSipToRpc.find(it->second);
            if (it2 != _mappingTableSipToRpc.end())
            {
                if(1)
                {
                    std::lock_guard<std::mutex> lock(_mappingTableSipToRpcMutex);
                    _mappingTableSipToRpc.erase(it2);
                    count1 = _mappingTableSipToRpc.size();
                    _sipToRpcNum --;
                }

                if(1)
                {
                    std::lock_guard<std::mutex> lock(_mappingTableRpcToSipMutex);
                    _mappingTableRpcToSip.erase(it);
                    count2 = _mappingTableRpcToSip.size();
                    _rpcToSipNum --;
                }
            }
            else
            {
                TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + "(), cant found:["+it->second+"->"+TC_Common::tostr(cid)+"]" << endl);
            }
        }

        if (count1 != count2)
        {
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + "(), Count1 != Count2" << endl);
        }
        return true;
    }

    bool MappingTable::deleteMappingItem(const std::string & didStr)
    {
        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if (did==0)
            return false;

        if (did->isValid() == false)
            return false;

        std::lock_guard<std::mutex> lock(_mutex);
        int count1 =0, count2=0;

        std::lock_guard<std::mutex> temlock(_mappingTableSipToRpcMutex);
        SipToRpcMappingType::iterator it = _mappingTableSipToRpc.find(did->string());
        if (it != _mappingTableSipToRpc.end())
        {
            RpcToSipMappingType::iterator it2 = _mappingTableRpcToSip.find(it->second);
            if (it2 != _mappingTableRpcToSip.end())
            {
                std::lock_guard<std::mutex> lock(_mappingTableRpcToSipMutex);
                _mappingTableRpcToSip.erase(it2);
                count1 = _mappingTableRpcToSip.size();
            }
            else
            {
                TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + "(), cant found:["+TC_Common::tostr(it->second)+"->"+did->string()+"]" << endl);
            }
            _mappingTableSipToRpc.erase(it);
            count2 = _mappingTableSipToRpc.size();
        }

        if (count1 != count2)
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + "(),Count1 != Count2" << endl);
        return true;
    }

    int MappingTable::countMappingTableSipToRpc()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::lock_guard<std::mutex> temlock(_mappingTableSipToRpcMutex);
        return _mappingTableSipToRpc.size();
    }

    int MappingTable::countMappingTableRpcToSip()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::lock_guard<std::mutex> temlock(_mappingTableRpcToSipMutex);
        return _mappingTableRpcToSip.size();
    }

    int MappingTable ::_rpcToSipNum = 0;
    int MappingTable::_sipToRpcNum = 0;

    void MappingTable::PintOutRpcToSipTableNum()
    {
        TLOGINFO("[MappingTable] PintOutRpcToSipTableNum() called,  the total is :" << _rpcToSipNum << endl);
    }
    void MappingTable::PintOutSipToRpcTableNum()
    {
        TLOGINFO("[MappingTable] PintOutSipToRpcTableNum() called,  the total is : " << _sipToRpcNum << endl);
    }

    //------------------------------------------------
    SipDialogManager::SipDialogManager()
    {
        _TotalCreatedDialogsCount = 0;

        _timeoutNo180 = 2 * 60 * 1000;
        _timeoutNo200 = 4 * 60 * 1000;
        _timeoutNormalClosed = 2 * 60 * 1000;
        _timeoutFlowNoAck = 60 * 60 * 1000;
    }

    int SipDialogManager::countSipDialogs()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        return _sipDialogs.size();
    }

    SipDialogPtr SipDialogManager::getSipDialog(const std::string & didStr)
    {
        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if (did == nullptr)
            return nullptr;

        if (did->isValid()==false)
            return nullptr;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.empty())
            return nullptr;

        SipDialogsMapType::iterator it = _sipDialogs.find(did->string());
        if (it != _sipDialogs.end())
            return it->second;
        return nullptr;
    }

    bool SipDialogManager::destroySipDialog(const std::string & didStr)
    {
        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if (did == nullptr)
            return false;

        if (did->isValid() == false)
            return false;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        SipDialogsMapType::iterator it = _sipDialogs.find(did->string());
        if (it == _sipDialogs.end())
        {
            TLOGWARN("[SipDialogManager] " << std::string(__FUNCTION__) + "(), NOT found, SipDialog id :" + did->string() << endl);
            return false;
        }
        _sipDialogs.erase(it);
        return true;
    }

    SipDialogPtr SipDialogManager::createSipDialog(const std::string & didStr)
    {
        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if (did == nullptr)
            return nullptr;

        if (did->isValid()==false)
            return nullptr;

        std::lock_guard<std::mutex> lock(m_mutex);
        SipDialogPtr dialog = getSipDialog(didStr);
        if (dialog)
        {
            TLOGWARN("[SipDialogManager] " << std::string(__FUNCTION__) + "(),duplicated key in insert,key="+ didStr << endl);
            return nullptr;
        }

        dialog = SipDialog::create(did);
        if (!dialog)
        {
            TLOGWARN("[SipDialogManager] " << std::string(__FUNCTION__) + "(),SipDialog::create failed,key="+ didStr << endl);
            return nullptr;
        }

        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        _sipDialogs.insert(SipDialogsMapType::value_type(didStr, dialog));
        _TotalCreatedDialogsCount ++;
        return dialog;
    }

    bool SipDialogManager::removeSipDialog(const std::string & didStr)
    {
        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if (did == nullptr)
            return false;

        if (did->isValid()==false)
            return false;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        SipDialogsMapType::iterator it = _sipDialogs.find(didStr);
        if (it == _sipDialogs.end())
        {
            TLOGWARN("[SipDialogManager] " << std::string(__FUNCTION__) + "(), dialog NOT found:" + didStr << endl);
            return false;
        }
        _sipDialogs.erase(it);
        return true;
    }

    bool SipDialogManager::insertSipDialog(const std::string & didStr, const SipDialogPtr & dialog)
    {
        SipDialogIdPtr did = SipDialogId::fromString(didStr);
        if (did == nullptr)
            return false;

        if (did->isValid()==false)
            return false;

        std::lock_guard<std::mutex> lock(m_mutex);
        SipDialogPtr hasDialog = getSipDialog(didStr);
        if (hasDialog)
        {
            TLOGWARN("[SipDialogManager] " << std::string(__FUNCTION__) + "(),duplicated key in insert,key="+ didStr << endl);
            return false;
        }

        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        _sipDialogs.insert(SipDialogsMapType::value_type(didStr, dialog));
        SIPGW_LOG_IFO("{"+did->string()+"}****** Have INSERTed  Sip dialog");
        _TotalCreatedDialogsCount ++;
        return true;
    }

    void SipDialogManager::pintOutSipDialogMapSize()
    {
        TLOGINFO("[SipDialogManager] " << __FUNCTION__ << "() called,  the _sipDialogs SIZE is : " << _sipDialogs.size() << endl);
    }

    bool SipDialogManager::getTimeOutSipDialog(vector<SipDialogPtr> & vectSipDialogs, int & iSipToRpc180, int & iSipToRpc200, int & iRpcToSip180, int & iRpcToSip200,int & iNomalTerm, int & iExcepDeal)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDialogPtr pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            if (pSipDialog->getSipTimeOutType() == kSipTimeoutTypeCleanRes)
                continue;

            SipDialogState temStatus = pSipDialog->getSipDialogStatus();
            int64_t lInitStatusTime = pSipDialog->getCurStatusInitTime();
            int64_t lCurTime = tars::TC_TimeProvider::getInstance()->getNowMs();
            TimeOutType timeOutType = pSipDialog->getTimeOutType();
            bool refreshDialog = pSipDialog->getSessionFreshDilogFlag();

            if (timeOutType == TimeOutTypeNo && false == refreshDialog)
            {
                if (temStatus == kSipDialogStateInit)
                {
                    int64_t  diffTime = lCurTime - lInitStatusTime;
                    if (pSipDialog->getSipCallRpcProp())
                    {
                        if (diffTime > _timeoutNo180)
                        {
                            pSipDialog->setTimeOutType(TimeOutTypeSipToRpcNo180);
                            vectSipDialogs.push_back(pSipDialog);
                            iSipToRpc180 ++;

                            SipDialog::autoAddExceptionNum();
                        }
                    }
                    else
                    {
                        if (diffTime > _timeoutNo180)
                        {
                            pSipDialog->setTimeOutType(TimeOutTypeRpcToSipNo180);
                            vectSipDialogs.push_back(pSipDialog);
                            iRpcToSip180 ++;

                            SipDialog::autoAddExceptionNum();
                        }
                    }
                }
                else if (temStatus == kSipDialogStateEarly)
                {
                    int64_t  diffTime = lCurTime - lInitStatusTime;
                    if (pSipDialog->getSipCallRpcProp())
                    {
                        if (diffTime > _timeoutNo200)
                        {
                            pSipDialog->setTimeOutType(TimeOutTypeSipToRpcNo200);
                            vectSipDialogs.push_back(pSipDialog);
                            iSipToRpc200 ++;

                            SipDialog::autoAddExceptionNum();
                        }
                    }
                    else
                    {
                        if (diffTime > _timeoutNo200)
                        {
                            pSipDialog->setTimeOutType(TimeOutTypeRpcToSipNo200);
                            vectSipDialogs.push_back(pSipDialog);
                            iRpcToSip200 ++;

                            SipDialog::autoAddExceptionNum();
                        }
                    }
                }
                else if (temStatus == kSipDialogStateTermed || temStatus == kSipDialogStateCanced)
                {
                    int64_t  terminateTime = pSipDialog->getTerminateTime();
                    int64_t  diffTime = lCurTime - terminateTime;
                    if (diffTime > _timeoutNormalClosed)
                    {
                        pSipDialog->setTimeOutType(TimeOutTypeNomalTerminated);
                        vectSipDialogs.push_back(pSipDialog);
                        iNomalTerm ++;
                    }
                }
                else if (temStatus == kSipDialogStateInit || temStatus == kSipDialogStateCancing || temStatus == kSipDialogStateTermnating)
                {
                    if (pSipDialog->getReSendTimes() > MaxReSendTimes)
                    {
                        pSipDialog->setTimeOutType(TimeOutTypeReSendTimeOut);
                        vectSipDialogs.push_back(pSipDialog);

                        SipDialog::autoAddExceptionNum();
                    }
                }
            }
            else if (TimeOutTypeExceedMaxDuration ==timeOutType)
            {
                vectSipDialogs.push_back(pSipDialog);
            }
            else
            {
                int64_t  enterExceptionTime = pSipDialog->getEnterExceptionTime();
                int64_t  diffTime = lCurTime - enterExceptionTime;
                if (diffTime > _timeoutNormalClosed)
                {
                    pSipDialog->setTimeOutType(TimeOutTypeExceptionDealTimeOut);
                    vectSipDialogs.push_back(pSipDialog);
                    iExcepDeal ++;
                }
            }
        }
        return true;
    }

    bool SipDialogManager::getExceedMaxDurationSipDialog(vector<SipDialogPtr> & vectSipDialgs)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDialogPtr pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            int64_t lCurTime = tars::TC_TimeProvider::getInstance()->getNowMs();
            int64_t lCreateTime = pSipDialog->getCreateTime();
            if ((lCurTime - lCreateTime) > TimeOutMaxDialogActiveDuration)
            {
                vectSipDialgs.push_back(pSipDialog);

                std::string strSipDialogId = pSipDialog->id();
                CallReleaseType releaseType = pSipDialog->getSipToRpcReleaseType();
                SipDialogState  statusType = pSipDialog->getSipDialogStatus();

                time_t createTimeSec = lCreateTime / 1000;
                int createTimeMs = lCreateTime % 1000;
                std::string strCreateTime = tars::TC_Common::tm2str(createTimeSec, "%Y-%m-%d %H:%M:%S") + ":" + tars::TC_Common::tostr(createTimeMs);
                
                int64_t  enterExceptionTime = pSipDialog->getEnterExceptionTime();
                time_t exceptionTimeSec = enterExceptionTime / 1000;
                int exceptionTimeMs = enterExceptionTime % 1000;
                std::string strExceptionTime = tars::TC_Common::tm2str(exceptionTimeSec, "%Y-%m-%d %H:%M:%S") + ":" + tars::TC_Common::tostr(exceptionTimeMs);

                SIPGW_LOG_IFO("SipDialog Exceed TimeOutMaxDialogActiveDuration SipDialog, id :" + strSipDialogId + " ,STATUS Type: " + TC_Common::tostr(statusType)
                    +  " ,RELEASE Type: " + TC_Common::tostr(releaseType)  + " ,TIMEOUT Type: " + TC_Common::tostr(pSipDialog->getTimeOutType()) +  " ,SIPcallRPC: " + TC_Common::tostr(pSipDialog->getSipCallRpcProp())
                    + " ,CREATE Time: " + strCreateTime + " ,ENTER EXCEPTION Time: " + strExceptionTime);

                pSipDialog->setTimeOutType(TimeOutTypeExceedMaxDuration);
            }
        }
        return true;
    }

    bool SipDialogManager::checkRefreshSessionRequest(const SipDialogPtr & pDialog , bool & bRefresh,  SipDialogPtr & pOriginalDialog,  bool & bRefreshPre, bool & bUacRefresh)
    {
        if ( pDialog == nullptr)
        {
            SIPGW_LOG_WRN(std::string(__FUNCTION__) + "() called, pDialog == 0 !" );
            return false;
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        if (_sipDialogs.size() == 0)
            return false;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        std::string strCallId = pDialog->_sipMsgDecodePart._callIdPart._strNumber + "@" + pDialog->_sipMsgDecodePart._callIdPart._strHost;
        bRefresh = false;
        SIPGW_LOG_DBG(std::string(__FUNCTION__) + "() called, new SipDialog strCallId id:" + strCallId );

        for (; it != _sipDialogs.end(); it++)
        {
            SipDialogPtr pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            std::string strTemp = pSipDialog->_sipMsgDecodePart._callIdPart._strNumber + "@" + pSipDialog->_sipMsgDecodePart._callIdPart._strHost;
            if (strTemp == strCallId)
            {
                pOriginalDialog = pSipDialog;
                bRefresh = true;
                SIPGW_LOG_DBG(std::string(__FUNCTION__) + "() called, strCallId id:" + strCallId + " ,bRefresh = TRUE");

                if (pDialog->_sipRegisterSub == nullptr)
                {
                    SIPGW_LOG_WRN(std::string(__FUNCTION__) + "() called, pDialog->_sipRegisterSub == 0:" + strCallId );
                    return false;
                }
                if (pSipDialog->_sipRegisterSub == nullptr)
                {
                    SIPGW_LOG_WRN(std::string(__FUNCTION__) + "() called, pSipDialog->_sipRegisterSub == 0:" + strCallId );
                    return false;
                }

                SipDialogState dialogStatus = pSipDialog->getSipDialogStatus();
                if (dialogStatus <= kSipDialogState200Ok)
                {
                    bRefresh = false;
                    return true;
                }

                std::string  clientIp, registrarIp;
                int clientPort = 0, registrarPort = 0;
                std::string sendIp;
                pSipDialog->_sipRegisterSub->getIpsAndPorts(clientIp, clientPort, registrarIp, registrarPort, sendIp);
                pDialog->_sipRegisterSub->setIpsAndPorts(clientIp, clientPort, registrarIp, registrarPort);

                pDialog->setSipProxyId(pSipDialog->getSipProxyId());
                pDialog->setSipGatewayId(pSipDialog->getSipGatewayId());
                pDialog->setToSBCConnectionId(pSipDialog->getToSBCConnectionId());
                pDialog->setAuthType(pSipDialog->getAuthType());

                bRefreshPre = pDialog->_sipMsgDecodePart._sessionExpiresPart._bRefreshPre;
                bUacRefresh = pDialog->_sipMsgDecodePart._sessionExpiresPart._bUAS_refresh;

                pSipDialog->_sipMsgDecodePart._sessionExpiresPart = pDialog->_sipMsgDecodePart._sessionExpiresPart;
                pSipDialog->_sipMsgDecodePart._bodyPart._strBody = pSipDialog->getSdpOffer();

                pDialog->setIsFreshDialog(true);
                pDialog->setOriginDialogId(pOriginalDialog->id());

                return true;
            }
        }

        SIPGW_LOG_DBG(std::string(__FUNCTION__) + "() called, strCallId id:" + strCallId + " ,bRefresh = FALSE");
        return true;
    }

    void SipDialogManager::getToBeRefreshedSipDialogs(vector<SipDialogPtr> & vectSipDialogs)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        SipDialogPtr pSipDialog = nullptr;

        for (; it != _sipDialogs.end(); it++)
        {
            pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            SipDialogState dialogStatus = pSipDialog->getSipDialogStatus();
            if (dialogStatus != kSipDialogStateACKConfirmed)
                continue;

            int64_t curTime = tars::TC_TimeProvider::getInstance()->getNowMs();
            int64_t refreshTime = pSipDialog->getLastFreshTime();
            int64_t expiresTime =  pSipDialog->_sipMsgDecodePart._sessionExpiresPart._iSeconds;
            bool bRefreshPre = pSipDialog->_sipMsgDecodePart._sessionExpiresPart._bRefreshPre;
            bool bUasRefresh =  pSipDialog->_sipMsgDecodePart._sessionExpiresPart._bUAS_refresh;
            bool bSipToRpc = pSipDialog->getSipCallRpcProp();

            if (!bRefreshPre)
                continue;

            if (bSipToRpc)
            {
                //sip to rpc, uac is the sip
                if (bUasRefresh == false)
                    continue;
                else
                {
                    int64_t diffTime = (curTime - refreshTime)/1000;    //s
                    if (diffTime > (expiresTime /2))
                        vectSipDialogs.push_back(pSipDialog);
                }
            }
            else
            {
                //rpc to sip, rpc is the uac
                if (bUasRefresh == false)
                {
                    int64_t diffTime = (curTime - refreshTime)/1000;    //s
                    if (diffTime > (expiresTime /2))
                        vectSipDialogs.push_back(pSipDialog);
                }
            }
        }
    }

    void SipDialogManager::getToBeReSendSipDialogs(vector<SipDialogPtr> & vectSipDialogs)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        SipDialogPtr pSipDialog = nullptr;

        for (; it != _sipDialogs.end(); it++)
        {
            pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            SipDialogState status = pSipDialog->getSipDialogStatus();
            int64_t sendTime = pSipDialog->getSendTime();
            int repeatedTimes = pSipDialog->getReSendTimes();
            bool bSipToRpc = pSipDialog->getSipCallRpcProp();

            if (repeatedTimes > MaxReSendTimes)
                continue;

            if (status == kSipDialogStateInit || status == kSipDialogStateCancing || status == kSipDialogStateTermnating)
            {
                int64_t curTime = tars::TC_TimeProvider::getInstance()->getNowMs();
                int64_t diffTime = curTime - sendTime;
                if (diffTime > MaxReSendInterval && sendTime != 0)
                    vectSipDialogs.push_back(pSipDialog);
            }

            if (bSipToRpc && status == kSipDialogState200Ok)
            {
                int64_t curTime = tars::TC_TimeProvider::getInstance()->getNowMs();
                int64_t diffTime = curTime - sendTime;
                if (diffTime > MaxReSendInterval && sendTime != 0)
                    vectSipDialogs.push_back(pSipDialog);
            }
        }
    }

    bool SipDialogManager::getOriginalSipDialog(const SipDialogPtr & pDialog,  SipDialogPtr & pOriginalDialog)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        std::string strCallId = pDialog->_sipMsgDecodePart._callIdPart._strNumber + "@" + pDialog->_sipMsgDecodePart._callIdPart._strHost;
        SipDialogPtr pSipDialog;
        for (; it != _sipDialogs.end(); it++)
        {
            pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            std::string strTemp = pSipDialog->_sipMsgDecodePart._callIdPart._strNumber + "@" + pSipDialog->_sipMsgDecodePart._callIdPart._strHost;
            if (strTemp == strCallId)
            {
                pOriginalDialog = pSipDialog;
                return true;
            }
        }
        return false;
    }

    SipDialogPtr SipDialogManager::getSipDialog2(const std::string & callIdHost)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return nullptr;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDialogPtr pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            std::string strTemp = pSipDialog->getCallIdHost();
            if (strTemp == callIdHost)
                return pSipDialog;
        }
        return nullptr;
    }

    SipDialogPtr SipDialogManager::getSipDialog3(const std::string & exludeId, const std::string & callIdHost)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return nullptr;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDialogPtr pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            const std::string strTemp = pSipDialog->getCallIdHost();
            const std::string id = pSipDialog->id();
            if (exludeId.empty())
            {
                if (strTemp == callIdHost)
                    return pSipDialog;
            }
            else
            {
                if (strTemp == callIdHost && id != exludeId)
                    return pSipDialog;
            }
        }
        return nullptr;
    }

    void SipDialogManager::setTimeoutFor180(int64_t times)
    {
        _timeoutNo180 = times;
    }

    void SipDialogManager::setTimeoutFor200(int64_t times)
    {
        _timeoutNo200 = times;
    }

    void SipDialogManager::setTimeoutForClosed(int64_t times)
    {
        _timeoutNormalClosed = times;
    }

    void SipDialogManager::setTimeoutForFlowNoAck(int64_t times)
    {
        _timeoutFlowNoAck = times;
    }

    int SipDialogManager::getApp2SipDialogNum()
    {
        int num = 0;

        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDialogPtr pSipDialog = it->second;
            if (pSipDialog == nullptr)
                continue;

            if (pSipDialog->getSipCallRpcProp())
                continue;
            num++;
        }
        return num;
    }

    // ------------------------------------------------------------------------------------------------------------

    AsyncInvokeUserData::AsyncInvokeUserData(const int64_t callId, const int64_t methodId, const std::string & did)
    {
        _rpcCallId = callId;
        _rpcMethodId = methodId;
        _sipDialogId = did;
    }

    AsyncInvokeUserDataPtr AsyncInvokeUserData::fromString(const std::string & str)
    {
        //Must have two ",", such as "a,b,c,d" or "a,b,,"
        int pos1 = str.find(",");
        int pos2 = str.rfind(",");
        if ((pos1 < 0)||(pos2 < 0))
            return nullptr;

        if(pos1 >= pos2)
            return nullptr;

        std::string strA = str.substr(0, pos1);
        std::string strC = str.substr(pos2 + 1);
        std::string strB = str.substr(pos1 + 1, pos2 - pos1 -1);

        if (strA.empty() || strB.empty())
            return nullptr;

        int64_t callId = -1;
        try {
            callId = tars::TC_Common::strto<int64_t>(strA);
        } catch (...) {
            return nullptr;
        }

        int64_t methodId = -1;
        try {
            methodId = tars::TC_Common::strto<int64_t>(strB);
        } catch (...) {
            return nullptr;
        }
        return std::shared_ptr<AsyncInvokeUserData>(new AsyncInvokeUserData(callId, methodId, strC));
    }

    std::string AsyncInvokeUserData::toString()
    {
        std::string strA = std::to_string(_rpcCallId);
        std::string strB = std::to_string(_rpcMethodId);
        return (strA + "," + strB + "," + _sipDialogId);
    }

    // ---------------------------------------------------------

    unsigned int SipDialogIdGenerator::__g_serial = 1;
    std::mutex SipDialogIdGenerator::__g_serial_mutex;

    SipDialogIdPtr SipDialogIdGenerator::generate(const int serverId, const std::string & serverName)
    {
        if (serverId < 0)
            return nullptr;

        std::lock_guard<std::mutex> lock(__g_serial_mutex);

        int64_t serverPrefix = (((int64_t)serverId) & 0xFFFF) << 48;
        int64_t timestamp = (((int64_t)(tars::TC_TimeProvider::getInstance()->getNowMs() / 1000)) & 0xFFFFFFFFL) << 16;
        int64_t serial = ((int64_t)(__g_serial ++)) & 0xFFFF;
        int64_t callIdNumber = (serverPrefix + timestamp + serial);

        //时间，序号，serverId @serverName
        std::string sipCallId = Z62RadixTransfer::DecimalToZ62(callIdNumber);
        std::string strRandom = tars::TC_Common::tostr(tars::TC_Common::now2us());
        sipCallId += strRandom;
        if (!serverName.empty())
            sipCallId = sipCallId + "@" + serverName;

        std::string sipLocalTag = SipMsgCommon::createFromTag();
        return SipDialogId::create(sipCallId, sipLocalTag, "");
    }

    SipDialogIdPtr  SipDialogIdGenerator::generate(const int serverId, const std::string & serverName, const std::string & sipGatewayId, const std::string & sipProxyId)
    {
        if(serverId < 0)
            return nullptr;

        std::lock_guard<std::mutex> lock(__g_serial_mutex);

        int64_t serverPrefix = (((int64_t)serverId) & 0xFFFF) << 48;
        int64_t timestamp = (((int64_t)(tars::TC_TimeProvider::getInstance()->getNowMs() / 1000)) & 0xFFFFFFFFL) << 16;
        int64_t serial = ((int64_t)(__g_serial ++)) & 0xFFFF;
        int64_t callIdNumber = (serverPrefix + timestamp + serial);

        //时间，序号，serverId @serverName
        std::string sipCallId = Z62RadixTransfer::DecimalToZ62(callIdNumber);
        std::string strRandom = tars::TC_Common::tostr(tars::TC_Common::now2us());
        sipCallId += strRandom;
        if(!serverName.empty())
            sipCallId = sipCallId + "@" + serverName;

        std::string strSgSpId = sipGatewayId  + "+" + sipProxyId + "+";
        std::string strFromTag = strSgSpId + SipMsgCommon::createFromTag();

        return SipDialogId::create(sipCallId, strFromTag, "");
    }

    // ---------------------------------------------------------
    char Z62RadixTransfer::Z62DigitalToChar(int val)
    {
        if ((val >= 62)||(val < 0))
            return 0;

        //0-9, A-Z, a-z
        if (val < 10) //0-9
            return (val + '0');

        if (val >= 36)
            return (val - 36 + 'a');
        return (val - 10 + 'A');
    }

    int Z62RadixTransfer::Z62CharToDigital(char ch, int defaultVal)
    {
        if ((ch >= '0')&&(ch <= '9'))
            return ch - '0';
        else if ((ch >= 'A')&&(ch <= 'Z'))
            return ch - 'A' + 10;
        else if ((ch >= 'a')&&(ch <= 'z'))
            return ch - 'a' + 36;
        else
            return defaultVal;
    }

    std::string Z62RadixTransfer::DecimalToZ62(int64_t dec)
    {
        std::string ret;
        bool isNegative = (dec < 0) ? true : false;
        if (isNegative) dec = -1 * dec;

        int c = 0;
        do
        {
            c = dec % 62;
            dec = dec / 62;
            char ch = Z62DigitalToChar(c);
            if(ret.empty())
                ret = ch;
            else
                ret = ch + ret;
        }while(dec);

        if (isNegative)
            ret = "-" + ret;

        return ret;
    }

    int64_t Z62RadixTransfer::Z62ToDecimal(const std::string & t62, const int64_t defaultVal)
    {
        std::string tZ62 = trim(t62);

        if (tZ62.empty())
            return defaultVal;

        char firstCh = tZ62[0];
        bool isNegtive = (firstCh == '-') ? true : false;

        char ch;
        int max = tZ62.size() - 1;
        int64_t val = 0;

        int idx = (isNegtive) ? 1 : 0;
        for (; idx <=max; idx ++)
        {
            val = val * 62;

            ch = tZ62[idx];
            if ((ch >= '0') && (ch <= '9'))
            {
                val += (ch - '0');
            }
            else if ((ch >= 'A') && (ch <= 'Z'))
            {
                val += (ch - 'A' + 10);
            }
            else if ((ch >= 'a') && (ch <= 'z'))
            {
                val += (ch - 'a' + 36);
            }
            else
            {
                return defaultVal;
            }
        }
        return (isNegtive ? (-1 * val) : val);
    }

    //************************************* ims core net statistic ************************************

    SipCallRecord::SipCallRecord(const std::string & dialogId, const std::string & domainId, const std::string & coreNetId, const std::string & calleeNationCode)
    {
        m_id = generateId(dialogId, coreNetId);   //dialogId + ":" + coreNetId;
        m_sipDialogId = dialogId;
        m_domainId = domainId;
        m_coreNetId = coreNetId;
        m_calleeNationCode = calleeNationCode;
        m_insertTicks = tars::TC_TimeProvider::getInstance()->getNowMs();

        m_ringed = false;
        m_accepted = false;
        m_failed = false;

        TLOGDEBUG("[SipDialog] " << std::string(__FUNCTION__) + ",m_id:" + m_id << endl);
    }

    std::string SipCallRecord::generateId(const std::string & dialogId, const std::string & coreNetId)
    {
        return dialogId + ":" + coreNetId;
    }

    SipCallRecord::~SipCallRecord()
    {
        TLOGDEBUG("[SipDialog] " << std::string(__FUNCTION__) + "id:" + m_id << endl);
    }

    DomainStatistic::DomainStatistic(const std::string & domainId)
    {
        m_domainId = domainId;
        m_statisticPeriod = MAX_CALL_STATISTIC_PEORIOD;
    }

    void DomainStatistic::insertStatisticRecord(const std::string & dialogId, const std::string & domainId, const std::string & coreNetId, const std::string & calleeNationCode)
    {
        std::string id = SipCallRecord::generateId(dialogId, coreNetId);
        SipCallRecordPtr pRecord = nullptr;
        {
            std::lock_guard<std::mutex> lock2(m_mutexRecordsMap);
            map<std::string, SipCallRecordPtr>::iterator itId = m_mapRecords.find(id);
            if (itId != m_mapRecords.end())
            {
                TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + "exist id:" + id << endl);
                return;
            }
            pRecord = std::shared_ptr<SipCallRecord>(new SipCallRecord(dialogId, domainId, coreNetId, calleeNationCode));
            m_mapRecords.insert(make_pair(pRecord->m_id, pRecord));
            TLOGDEBUG("[SipDialog] " << std::string(__FUNCTION__) + ",domainId:" +domainId + ",id:" + id << endl);
        }

        {
            std::lock_guard<std::mutex> lock1(m_mutexRecordsList);
            m_listRecords.push_back(pRecord);
        }
    }

    bool DomainStatistic::updateResultType(const std::string & dialogId, const std::string & coreNetId, SipCallResultType typeVal)
    {
        std::lock_guard<std::mutex> lock(m_mutexRecordsMap);
        std::string id = SipCallRecord::generateId(dialogId, coreNetId);
        map<std::string, SipCallRecordPtr>::iterator itId = m_mapRecords.find(id);
        if (itId == m_mapRecords.end())
        {
            TLOGWARN("[SipDialog] " << std::string(__FUNCTION__) + ",not found id:" + id << endl);
            return false;
        }

        switch(typeVal)
        {
        case kSipCallRinging:
            itId->second->m_ringed = true;
            break;

        case kSipCallAccepted:
            itId->second->m_ringed = true;
            itId->second->m_accepted = true;
            break;

        case kSipCallFailed:
            itId->second->m_failed = true;
            break;
        }

        TLOGDEBUG("[SipDialog] " << std::string(__FUNCTION__) + ",dialogId:" + dialogId + ",core net:" + coreNetId + ",typeVal:" + TC_Common::tostr(typeVal) << endl);
        return true;
    }

    bool DomainStatistic::getHighestRingingCoreNet(const std::string & calleeNationCode,const std::string & excludedCoreNet, std::string & coreNetId)
    {
        std::lock_guard<std::mutex> lck(m_mutexStatistics);
        CalleeRates::iterator itCallee = m_mapStatistics.find(calleeNationCode);
        if (itCallee == m_mapStatistics.end())
            return false;

        if (itCallee->second.size() == 0)
            return false;

        CoreNetRates & rates = itCallee->second;
        double maxRate = 0;
        coreNetId.clear();

        CoreNetRates::iterator itCoreNet = rates.begin();
        for (; itCoreNet != rates.end(); itCoreNet++)
        {
            if (itCoreNet->second.m_ringRate > maxRate && excludedCoreNet != itCoreNet->first)
                coreNetId = itCoreNet->first;
            else
                maxRate = itCoreNet->second.m_ringRate;
        }

        return coreNetId.empty() ? false : true;
    }

    void DomainStatistic::clearTimeoutRecords()
    {
        vector<std::string> ids;

        int64_t curTicks = tars::TC_TimeProvider::getInstance()->getNowMs();
        {
            std::lock_guard<std::mutex> lock(m_mutexRecordsList);
            list<SipCallRecordPtr>::iterator itList = m_listRecords.begin();
            for (; itList != m_listRecords.end(); )
            {
                if ((curTicks - (*itList)->m_insertTicks) > m_statisticPeriod)
                {
                    ids.push_back((*itList)->m_id);
                    m_listRecords.erase(itList++);
                }
                else
                {
                    break;
                }
            }
        }

        if (ids.size() == 0)
            return;

        {
            std::lock_guard<std::mutex> lock(m_mutexRecordsMap);
            for (unsigned int i = 0; i < ids.size(); i++)
            {
                map<std::string, SipCallRecordPtr>::iterator itId = m_mapRecords.find(ids.at(i));
                if (itId != m_mapRecords.end())
                    m_mapRecords.erase(itId);
            }
        }
    }

    void DomainStatistic::executeStatistic()
    {
        CalleeRates statistics;

        {
            std::lock_guard<std::mutex> lck(m_mutexRecordsList);
            list<SipCallRecordPtr>::iterator itRecord = m_listRecords.begin();
            for (; itRecord != m_listRecords.end(); itRecord++)
            {
                if (*itRecord == nullptr)
                    continue;

                std::string calleeCode = (*itRecord)->m_calleeNationCode;
                std::string coreNetId = (*itRecord)->m_coreNetId;

                CalleeRates::iterator itCallee = statistics.find(calleeCode);
                if (itCallee == statistics.end())
                {
                    CoreNetRates coreNetRates;
                    StatisticStruct st;
                    st.m_times = 1;

                    if ((*itRecord)->m_ringed)
                        st.m_rings = 1;
                    if ((*itRecord)->m_accepted)
                        st.m_accepted = 1;
                    if ((*itRecord)->m_failed)
                        st.m_failed = 1;

                    st.m_ringRate = st.m_rings / ((double)st.m_times) * 100;
                    st.m_acceptRate = st.m_accepted / ((double)st.m_times) * 100;
                    st.m_failedRate = st.m_failed / ((double)st.m_times) * 100;

                    coreNetRates[coreNetId] = st;
                    statistics[calleeCode] = coreNetRates;
                }
                else
                {
                    CoreNetRates & rates = itCallee->second;
                    CoreNetRates::iterator itCoreNet = rates.find(coreNetId);
                    if (itCoreNet == rates.end())
                    {
                        StatisticStruct st;
                        st.m_times = 1;

                        if ((*itRecord)->m_ringed)
                            st.m_rings = 1;
                        if ((*itRecord)->m_accepted)
                            st.m_accepted = 1;
                        if ((*itRecord)->m_failed)
                            st.m_failed = 1;

                        st.m_ringRate = st.m_rings / ((double)st.m_times) * 100;
                        st.m_acceptRate = st.m_accepted / ((double)st.m_times) * 100;
                        st.m_failedRate = st.m_failed / ((double)st.m_times) * 100;

                        rates[coreNetId] = st;
                    }
                    else
                    {
                        StatisticStruct & st = itCoreNet->second;
                        st.m_times ++;

                        if ((*itRecord)->m_ringed)
                            st.m_rings ++;
                        if ((*itRecord)->m_accepted)
                            st.m_accepted ++;
                        if ((*itRecord)->m_failed)
                            st.m_failed ++;

                        st.m_ringRate = st.m_rings / ((double)st.m_times) * 100;
                        st.m_acceptRate = st.m_accepted / ((double)st.m_times) * 100;
                        st.m_failedRate = st.m_failed / ((double)st.m_times) * 100;
                    }
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexStatistics);
            statistics.swap(m_mapStatistics);
        }
    }

    void DomainStatistic::setStatisticPeriod(const int64_t stPeriod)
    {
        m_statisticPeriod = stPeriod;
    }

    std::string DomainStatistic::getStatisticInfo()
    {
        std::string logs;
        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        CalleeRates::iterator itCallee = m_mapStatistics.begin();
        for (; itCallee != m_mapStatistics.end(); itCallee++)
        {
            logs += "\n\t callee:" + itCallee->first;
            CoreNetRates & coreRates = itCallee->second;
            CoreNetRates::iterator itCore = coreRates.begin();
            for (; itCore != coreRates.end(); itCore++)
            {
                logs += "\n\t coreNetId:" + itCore->first;
                logs += "\n\t times:"  + std::to_string(itCore->second.m_times);
                logs += "\n\t ringing rate:" + std::to_string(itCore->second.m_ringRate) + "%";
                logs += "\n\t accept rate:" + std::to_string(itCore->second.m_acceptRate) + "%";
            }
            logs += "\r\n";
        }
        logs += "\r\n";
        return logs;
    }


    SipCallStatisticRecordManager::SipCallStatisticRecordManager()
    {
         m_statisBaseSchdTime = 1 * 60 * 1000;
         m_statisLastSchdTime = 0;

         m_statisPrintBaseSchdTime = 10 * 60 * 1000;
         m_statisPrintLastSchdTime = 0;
    }

    void SipCallStatisticRecordManager::insertStatisticRecord(const std::string & domainId, const std::string & dialogId, const std::string & coreNetId, const std::string & calleeNationCode)
    {
        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        map<std::string, DomainStatisticPtr >::iterator itDomain = m_statistics.find(domainId);
        if (itDomain == m_statistics.end())
        {
            DomainStatisticPtr pdomainSt = std::shared_ptr<DomainStatistic>(new DomainStatistic(domainId));
            pdomainSt->insertStatisticRecord(dialogId, domainId, coreNetId, calleeNationCode);
            m_statistics[domainId] = pdomainSt;
        }
        else
        {
            itDomain->second->insertStatisticRecord(dialogId, domainId, coreNetId, calleeNationCode);
        }
    }

    bool SipCallStatisticRecordManager::updateResultType(const std::string & domainId, const std::string & dialogId, const std::string & coreNetId, SipCallResultType typeVal)
    {
        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        map<std::string, DomainStatisticPtr >::iterator itDomain = m_statistics.find(domainId);
        if (itDomain == m_statistics.end())
            return false;
        else
            return itDomain->second->updateResultType(dialogId, coreNetId, typeVal);
    }

    bool SipCallStatisticRecordManager::getHighestRingingCoreNet(const std::string & domainId, const std::string & calleeNationCode, const std::string & excludedCoreNet, std::string & coreNetId)
    {
        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        map<std::string, DomainStatisticPtr >::iterator itDomain = m_statistics.find(domainId);
        if (itDomain == m_statistics.end())
            return false;
        else
            return itDomain->second->getHighestRingingCoreNet(calleeNationCode, excludedCoreNet, coreNetId);
    }

    void SipCallStatisticRecordManager::setStatisticPeriod(const int64_t stPeriod)
    {
        if (stPeriod < 60 * 60 * 1000)
            return;

        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        map<std::string, DomainStatisticPtr >::iterator itDomain = m_statistics.begin();
        for (; itDomain != m_statistics.end(); itDomain++)
            itDomain->second->setStatisticPeriod(stPeriod);
    }

    void SipCallStatisticRecordManager::clearTimeoutRecords()
    {
        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        map<std::string, DomainStatisticPtr >::iterator itDomain = m_statistics.begin();
        for (; itDomain != m_statistics.end(); itDomain++)
            itDomain->second->clearTimeoutRecords();
    }

    void SipCallStatisticRecordManager::executeStatistic()
    {
        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        map<std::string, DomainStatisticPtr >::iterator itDomain = m_statistics.begin();
        for (; itDomain != m_statistics.end(); itDomain++)
            itDomain->second->executeStatistic();
    }

    void SipCallStatisticRecordManager::onSchd()
    {
        int64_t curTicks = tars::TC_TimeProvider::getInstance()->getNowMs();
        int64_t diff =curTicks - m_statisLastSchdTime;
        if (diff < m_statisBaseSchdTime)
            return;

        clearTimeoutRecords();
        executeStatistic();
        m_statisBaseSchdTime = curTicks;

        diff = curTicks - m_statisPrintLastSchdTime;
        if (diff < m_statisPrintBaseSchdTime)
            return;

        printf();
        m_statisPrintLastSchdTime = curTicks;
    }

    void SipCallStatisticRecordManager::printf()
    {
        std::lock_guard<std::mutex> lock(m_mutexStatistics);
        map<std::string, DomainStatisticPtr >::iterator itDomain = m_statistics.begin();
        for (; itDomain != m_statistics.end(); itDomain++)
        {
            std::string logs = " ----------------- \nDomainId:" + itDomain->first + "\n";

            DomainStatisticPtr pDomain = itDomain->second;
            std::string domainStatistic = pDomain->getStatisticInfo();
            logs += domainStatistic;

            if (false == logs.empty())
                TLOGINFO("[SipDialog] ----------------- ringing statistics:" << logs << endl);
        }
    }

    bool SbcLineInfo::operator<(const SbcLineInfo& obj) const
    {
        if (this == &obj)
            return true;

        if (m_coreNetId < obj.m_coreNetId)
            return true;
        else
            return false;
    }

    bool SbcLineInfo::operator==(const SbcLineInfo& obj) const
    {
        if (this == &obj)
            return true;

        if (m_coreNetId == obj.m_coreNetId)
            return true;
        else
            return false;
    }

    bool SbcLineInfo::operator>(const SbcLineInfo&obj) const
    {
        if (this == &obj)
            return true;

        if (m_coreNetId > obj.m_coreNetId)
            return true;
        else
            return false;
    }

    ForceResInvite::ForceResInvite(const std::string & domainId, int tmpCode, int finalCode, int delay)
    {
        m_domainId = domainId;
        m_tmpRspCode = tmpCode;
        m_finalResCode = finalCode;
        m_deleyTime = delay;
    }

    bool ForceResInvite::operator<(const ForceResInvite&obj) const
    {
        if (this == &obj)
            return true;

        return m_domainId < obj.m_domainId ? true : false;
    }

    bool ForceResInvite::operator==(const ForceResInvite&obj) const
    {
        if (this == &obj)
            return true;

        return m_domainId == obj.m_domainId ? true : false;
    }

    bool ForceResInvite::operator>(const ForceResInvite&obj) const
    {
        if (this == &obj)
            return true;

        return m_domainId > obj.m_domainId ? true : false;
    }

};

