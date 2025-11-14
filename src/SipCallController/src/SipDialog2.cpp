#include "SipGW3Common.h"
#include "SipDialog2.h"
//#include "CloudSipGatewayI.h"
#include "SipGateway/src/SipGWCommon.h"
#include "util/tc_common.h"

namespace SipCloudGateway
{
    tars::Bool SipDlgId::isParameterValid(const std::string & id, const std::string & ltag, const std::string & rtag)
    {
        if (id.empty())
        {
            SIPGW_LOG_WRN("SipDlgId, Call-Id: empty");
            return false;
        }

        if (ltag.empty())
        {
            SIPGW_LOG_WRN("SipDlgId, local-Tag: empty");
            return false;
        }

        if (rtag.empty())
        {
            SIPGW_LOG_DBG("SipDlgId, remote-Tag: empty");
        }

        return true;
    }

    SipDlgIdPtr SipDlgId::create(const std::string & cid, const std::string & ltag, const std::string & rtag)
    {
        if (isParameterValid(cid, ltag, rtag) == false)
        {
            SIPGW_LOG_WRN("SipDlgId::isParameterValid() failed, cid="+cid+",ltag="+ltag+",rtag="+rtag);
            return 0;
        }
        return std::shared_ptr<SipDlgId>(new SipDlgId(cid, ltag, rtag));
    }

    SipDlgId::SipDlgId(const std::string & id, const std::string & ltag, const std::string & rtag):
    _sipCallId(id), _sipLocalTag(ltag), _sipRemoteTag(rtag)
    {}

    SipDlgId::SipDlgId(const SipDlgIdPtr& did)
    {
        if (did==0)
        {
            SIPGW_LOG_WRN("SipDlgId::SipDlgId() , input did NULL");
            return;
        }
        if (did->isValid() == false)
        {
            SIPGW_LOG_WRN("SipDlgId::SipDlgId() , input did INVALID");
            return;
        }
        _sipCallId = did->_sipCallId;
        _sipLocalTag = did->_sipLocalTag;
        _sipRemoteTag = did->_sipRemoteTag;
    }

    SipDlgId::~SipDlgId()
    {
    }

    tars::Bool SipDlgId::isValid ()const
    {
        if ((_sipCallId.empty()) || (_sipLocalTag.empty()))
            return false;
        return true;
    }

    std::string SipDlgId::string ()const
    {
        if (isValid() == false)
            return "<INVALID>";

        const std::string info = _sipCallId + "\\" + _sipLocalTag + "\\" + _sipRemoteTag;
        return info;
    }

    SipDlgIdPtr SipDlgId::fromString(const std::string & str)
    {
        if (str == "<INVALID>")
        {
            return 0;
        }

        //Must have two ",", such as "a,b,c,d" or "a,b,,"
        int pos1 = str.find("\\");
        int pos2 = str.rfind("\\");
        if ((pos1 < 0)||(pos2 < 0))
        {
            return 0;
        }

        if (pos1 >= pos2)
        {
            return 0;
        }

        std::string cid = str.substr(0, pos1);
        std::string rtag = str.substr(pos2 + 1);
        std::string ltag = str.substr(pos1 + 1, pos2 - pos1 -1);
        if (cid.empty() || ltag.empty())
        {
            return 0;
        }
        else
        {
            return std::shared_ptr<SipDlgId>(new SipDlgId(cid, ltag, rtag));
        }
    }

    tars::Bool SipDlgId::isEqual(const SipDlgIdPtr& id)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (id == 0)
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

    SipDlg::SipDlg(const SipDlgIdPtr & did)
    {
        int64_t curTime = TNOWMS;
        m_createTime  = curTime;
        m_curStatusInitTime  = curTime;
        m_sipDialogStatus = SipGateway::kSipDialogStateInit;                //初始状态
        m_timeOutType = SipGateway::TimeOutTypeNo;
        m_enterExceptionTime = 0;                                //初始无异常进入
        m_terminateTime = curTime;                               //结束状态时间
        m_ereaseTime =curTime;

        if (did == 0)
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
            m_SipDlgId = did->string();
        }

        m_totalNum.fetch_add(1);
        m_totalNewNum.fetch_add(1);

        _sipRegisterSub = std::shared_ptr<SipProxy::SipRegisterSubscribe>(new SipProxy::SipRegisterSubscribe());
        m_authType = SipGateway::AuthNone;
        m_sdpNegoStatus = SDPInit;
        m_bRefreshDialog = false;
        m_lLastRefreshTime = 0;
        m_iReSendTimes = 0;
        m_iReSendTotalTimes = 0;
        m_lSendTime = 0;
        m_lAcceptedTime = 0;

        m_bSessionFreshDialog = false;
        m_mediaType = SipGateway::kAudioType;
        m_failedTimes = 0;

        m_lLastChargeTime = 0;
        m_lAlertingTime = 0;
        m_iSessionId = 0;
        m_iCCId = 0;
        m_iCCStatusType = 0;
        m_iRealReqTime = 180;
        m_iTotalTime = 0;
        m_iUserdTime = 0;

        m_bFromRoutePres = false;
        m_bToRoutePres = false;
        m_bFromNATPres = false;
        m_bToNATPres = false;
        m_bStartOcsPres = false;
        m_bStopCharfePres = false;
        m_bResendInvite = false;

        m_callerEnterpriseType = kEnterpriseOther;
        m_calleeEnterpriseType = kEnterpriseOther;
        m_releaseType = EN_CALL_RELEASE_BY_SERVER;

        m_bIsRecord = false;
        m_bMrfpInitPres = false;
        m_mrfpRingStatus = kMrfpRingInit;
        m_iToNATPort = 0;
        m_iFromNATPort = 0;
        m_iCloudPort = 0;
        m_iMgcfPort = 0;

        m_iCallerFromPort = 0;
        m_iCallerToPort = 0;
        m_iCallerFromPortByJego = 0;
        m_iCallerToPortByJego = 0;
        m_iBeep = 0;
        m_iBeepErrCode = 0;
        m_bBeepPres = false;
        m_bResendPres = false;
        m_iServiceType = 0;
        m_bRelease = true;
        m_bRecordRoute = true;

        m_iRouteIndex = 0;
        m_iCallerRefreshCseq = 0;
        m_iCalleeRefreshCseq = 0;
        m_iOcsErrCode = 0;
    }

    SipDlg::~SipDlg()
    {
        m_totalNum.fetch_sub(1);
        m_totalDeleteNum.fetch_add(1);

        SIPGW_LOG_IFO("******  ~SipDlg() ****** called!, SipDialogId :" + id());
    }

    // void SipDlg::onTimeout(const Common::TimerPtr& timer)
    // {
    //     if(m_chargeTimerExecutorPtr)
    //         m_chargeTimerExecutorPtr->cleanSipResourceTimeout(m_SipDlgId);

    //     closeTimer();
    // }

    SipDlgPtr SipDlg::create(const SipDlgIdPtr & did)
    {
        if (did == 0)
        {
            SIPGW_LOG_WRN("SipDlg(), INPUT did is NULL");
            return 0 ;
        }

        if (did->isValid() == false)
        {
            SIPGW_LOG_WRN("SipDlg(), INPUT did is INVALID");
            return 0 ;
        }
        return std::shared_ptr<SipDlg>(new SipDlg(did));
    }

    const std::string SipDlg::id()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_SipDlgId;
    }

    tars::Bool SipDlg::getPureUserAccount(const std::string & callAccount, std::string & number)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        //get number from string like [username:018508408345@test.com]
        tars::Int32 pos1 = callAccount.find(":");
        tars::Int32 pos2 = callAccount.find("@");
        if (pos1 != -1 && pos2 != -1)
        {
            number = callAccount.substr(pos1 + 1,pos2 -pos1 -1);
            return true;
        }
        return false;
    }

    void SipDlg::setSipDialogStatus(const SipGateway::SipDialogState & dialogStatus)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipDialogStatus = dialogStatus;
        m_curStatusInitTime = TNOWMS;

        if (dialogStatus == SipGateway::kSipDialogStateTermed)
            m_terminateTime = m_curStatusInitTime;
        else if (dialogStatus == SipGateway::kSipDialogStateACKConfirmed)
            m_lLastRefreshTime = m_curStatusInitTime;
        else if (dialogStatus == SipGateway::kSipDialogState200Ok)
            m_answeredTime = m_curStatusInitTime;
    }

    SipGateway::SipDialogState SipDlg::getSipDialogStatus()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipDialogStatus;
    }

    void SipDlg::setCurStatusInitTime(const tars::Int64 & lInitTime)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_curStatusInitTime = lInitTime;
    }
    tars::Int64 SipDlg::getCurStatusInitTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_curStatusInitTime;
    }

    void SipDlg::setTimeOutType(SipGateway::TimeOutType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_timeOutType = type;
        if (type != SipGateway::TimeOutTypeExceptionDealTimeOut)
            m_enterExceptionTime = TNOWMS;
        TLOGDEBUG(std::string(__FUNCTION__) << "() called. SipDlg id:" << m_SipDlgId << ",type:" << int(type) << endl);
    }

    SipGateway::TimeOutType SipDlg::getTimeOutType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_timeOutType;
    }

    tars::Int64 SipDlg::getEnterExceptionTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_enterExceptionTime;
    }

    tars::Int64 SipDlg::getTerminateTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_terminateTime;
    }

    tars::Int64 SipDlg::getCreateTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_createTime;
    }

    std::atomic<tars::Int32>  SipDlg::m_totalNum(0);
    std::atomic<tars::Int32>  SipDlg::m_totalNewNum(0);
    std::atomic<tars::Int32>  SipDlg::m_totalDeleteNum(0);
    std::atomic<tars::Int32> SipDlg::m_totalExceptionNum(0);

    void SipDlg::PirntOutSipDialogNum()
    {
        TLOGINFO(std::string(__FUNCTION__) << "() called, Total Num is : " << m_totalNum.load() << endl);
    }

    void SipDlg::autoAddExceptionNum()
    {
        m_totalExceptionNum.fetch_add(1);
    }

    SipGateway::AuthType SipDlg::getAuthType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_authType;
    }

    void SipDlg::setAuthType(SipGateway::AuthType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_authType = type;
    }

    SDPNegotiateStatus SipDlg::getSDPNegotiateStatus()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sdpNegoStatus;
    }

    void SipDlg::setSDPNegotiateStatus(SDPNegotiateStatus status)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sdpNegoStatus = status;
    }

    void SipDlg::setIsFreshDialog(tars::Bool isFreshDialog)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bRefreshDialog =  isFreshDialog;
    }
    tars::Bool SipDlg::getIsFreshDialog()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bRefreshDialog ;
    }

    void SipDlg::setLastRefreshTime(tars::Int64 refreshTime)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_lLastRefreshTime = refreshTime;
    }
    tars::Int64 SipDlg::getLastFreshTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_lLastRefreshTime;
    }

    void SipDlg::setRefreshCseq(tars::Int32 cseq, tars::Bool isCaller)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (isCaller)
            m_iCallerRefreshCseq = cseq;
        else
            m_iCalleeRefreshCseq = cseq;
    }
    tars::Int32 SipDlg::getRefreshCseq(tars::Bool isCaller)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return isCaller ? m_iCallerRefreshCseq : m_iCalleeRefreshCseq;
    }

    void SipDlg::RepeatTimeAutoIncrease()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iReSendTimes++;
        m_iReSendTotalTimes++;
    }

    tars::Int32 SipDlg::getReSendTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iReSendTimes;
    }

    tars::Int32 SipDlg::getReSendTotalTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iReSendTotalTimes;
    }

    void SipDlg::reSetRepeatTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iReSendTimes = 0;
    }

    void SipDlg::setReSendMethod(SipGateway::SipRequestMethodType reSendMethod)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_reSendMethod = reSendMethod;
    }

    SipGateway::SipRequestMethodType SipDlg::getReSendMethod()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_reSendMethod ;
    }

    void SipDlg::setSendTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_lSendTime = TNOWMS;
    }
    tars::Int64 SipDlg::getSendTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_lSendTime;
    }

    void SipDlg::setResendInvite(tars::Bool resend)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bResendInvite = resend;
    }
    tars::Bool SipDlg::getResendInvite()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bResendInvite;
    }

    void SipDlg::addFailedTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_failedTimes ++;
    }

    tars::Int32 SipDlg::getFailedTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_failedTimes;
    }

    void SipDlg::setSessionFreshDilogFlag(tars::Bool bFreshDialog)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bSessionFreshDialog = bFreshDialog;
    }

    tars::Bool SipDlg::getSessionFreshDilogFlag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bSessionFreshDialog;
    }

    void SipDlg::setRefreshMethod(EN_SIP_METHOD refreshMethod)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_refreshMethod = refreshMethod;
    }

    EN_SIP_METHOD SipDlg::getRefreshMethod()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_refreshMethod;
    }

    void SipDlg::setMediaType(SipGateway::MediaType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_mediaType = type;
    }

    SipGateway::MediaType SipDlg::getMediaType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_mediaType;
    }

    void SipDlg::setSdpOffer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sdpOffer = sdp;
    }

    std::string SipDlg::getSdpOffer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sdpOffer;
    }

    void SipDlg::setOrginSdpOffer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_originSdpOffer = sdp;
    }
    std::string SipDlg::getOrginSdpOffer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_originSdpOffer;
    }
    void SipDlg::setSdpAnswer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sdpAnswer = sdp;
    }
    std::string SipDlg::getSdpAnswer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sdpAnswer;
    }
    void SipDlg::setOrginSdpAnswer(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_originSdpAnswer = sdp;
    }
    std::string SipDlg::getOrginSdpAnswer()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_originSdpAnswer;
    }

    void SipDlg::setSipGatewayId(const std::string & gwId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSipGatewayId = gwId;
    }

    void SipDlg::setSipGatewayId1(const std::string & gwId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSipGatewayId1 = gwId;
    }

    void SipDlg::setSipGatewayId2(const std::string & gwId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSipGatewayId2 = gwId;
    }

    void SipDlg::setSipGatewayConnectId1(const std::string & connectId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strConnectId1 = connectId;
    }

    void SipDlg::setSipGatewayConnectId2(const std::string & connectId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strConnectId2 = connectId;
    }

    void SipDlg::setCallerRouteConnectId(const std::string & connectId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCallerRouteConnectId = connectId;
    }
    std::string SipDlg::getCallerRouteConnectId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCallerRouteConnectId;
    }

    void SipDlg::setMultiRouter(const vector<stRouterInfo>& route)
    {
        std::lock_guard<std::mutex> lock(m_mutexMultiRoute);
        m_vectMultiRoute = route;
    }

    void SipDlg::setMultiRouter(stRouterInfo & route)
    {
        std::lock_guard<std::mutex> lock(m_mutexMultiRoute);
        m_vectMultiRoute.push_back(route);
    }

    void SipDlg::getMultiRouter(vector<stRouterInfo> & route)
    {
        std::lock_guard<std::mutex> lock(m_mutexMultiRoute);
        route = m_vectMultiRoute;
    }

    tars::Bool SipDlg::getMultiRouterByIndex(tars::Int32 index, stRouterInfo & route)
    {
        std::lock_guard<std::mutex> lock(m_mutexMultiRoute);
        if (index < m_vectMultiRoute.size())
        {
            route = m_vectMultiRoute.at(index);
            return true;
        }

        return false;
    }

    void SipDlg::setMultiConnectIdIndex()
    {
        std::lock_guard<std::mutex> lock(m_mutexMultiRoute);
        m_iRouteIndex++;
    }

    tars::Int32 SipDlg::getMultiConnectIdIndex()
    {
        std::lock_guard<std::mutex> lock(m_mutexMultiRoute);
        return m_iRouteIndex;
    }

    std::string SipDlg::getSipGatewayId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSipGatewayId;
    }

    std::string SipDlg::getSipGatewayId1()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSipGatewayId1;
    }

    std::string SipDlg::getSipGatewayId2()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSipGatewayId2;
    }

    std::string SipDlg::getSipGatewayConnectId1()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strConnectId1;
    }

    std::string SipDlg::getSipGatewayConnectId2()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strConnectId2;
    }

    //void SipDlg::setCallerDisplayName(std::string  name)
    //{
    //    std::lock_guard<std::mutex> lock(m_mutex);
    //    m_strCallerDisplayName = name;
    //}

    void SipDlg::setCallerOriginName(std::string name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCallerOriginName = name;
    }

    //std::string SipDlg::getCallerDisplayName()
    //{
    //    std::lock_guard<std::mutex> lock(m_mutex);
    //    return m_strCallerDisplayName;
    //}

    std::string SipDlg::getCallerOriginName()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCallerOriginName;
    }

    void SipDlg::setModifiedCalleeNumber(const std::string & number)
    {
        m_strModifiedCalleeNumber = number;
    }

    std::string SipDlg::getModifiedCalleeNumber()
    {
        return m_strModifiedCalleeNumber;
    }

    void SipDlg::setCallerNumber(const std::string & number)
    {
        m_strCallerNumber = number;
    }

    std::string SipDlg::getCallerNumber()
    {
        return m_strCallerNumber;
    }

    void SipDlg::setCalleeNumber(const std::string & number)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCalleeNumber = number;
    }
    std::string SipDlg::getCalleeNumber()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCalleeNumber;
    }

    void SipDlg::setCallerDisplayName(std::string name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCallerDisplayName = name;
    }
    std::string SipDlg::getCallerDisplayName()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCallerDisplayName;
    }

    void SipDlg::setRtpGwOid(const std::string & oid)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_rtpGwOid = oid;
    }

    std::string SipDlg::getRtpGwOid()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_rtpGwOid;
    }

    void SipDlg::setDirection(const CallerDirection & direction)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_direction = direction;
    }

    CallerDirection SipDlg::getDirection()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_direction;
    }

    void SipDlg::setSipDirection(const SipDirection & direction)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipDirection = direction;
    }

    SipDirection SipDlg::getSipDirection()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipDirection;
    }

    void SipDlg::setCallerDirection(const CallerDirection & direction)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_callerDirection = direction;
    }

    CallerDirection SipDlg::getCallerDirection()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_callerDirection;
    }

    void SipDlg::setFromTag(std::string tag)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strFromTag = tag;
    }

    std::string SipDlg::getFromTag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strFromTag;
    }

    void SipDlg::setCallerToTag(std::string tag)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCallerToTag = tag;
    }
    std::string SipDlg::getCallerToTag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCallerToTag;
    }
    void SipDlg::setCalleeToTag(std::string tag)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCalleeToTag = tag;
    }
    std::string SipDlg::getCalleeToTag()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCalleeToTag;
    }
    void SipDlg::setCallIdHost(std::string callIdHost)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCallIdHost = callIdHost;
    }

    std::string SipDlg::getCallIdHost()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCallIdHost;
    }
    void SipDlg::setOriginCalleeNum(std::string  number)
    {
        m_originCalleeNumber = number;
    }

    std::string SipDlg::getOriginCalleeNum()
    {
        return m_originCalleeNumber;
    }

    void SipDlg::setContactIp(std::string ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strContactIp = ip;
    }

    std::string SipDlg::getContactIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strContactIp;
    }

    void SipDlg::setCallerIp(std::string ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCallerIp = ip;
    }

    std::string SipDlg::getCallerIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCallerIp;
    }

    void SipDlg::setCallerPort(tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iCloudPort = port;
    }

    tars::Int32 SipDlg::getCallerPort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iCloudPort;
    }

    void SipDlg::setCalleeIp(std::string ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCalleeIp = ip;
    }

    std::string SipDlg::getCalleeIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCalleeIp;
    }

    void SipDlg::setCalleePort(tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iMgcfPort = port;
    }

    tars::Int32 SipDlg::getCalleePort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iMgcfPort;
    }

    void SipDlg::setGw2Ip(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strGw2Ip = ip;
    }

    std::string SipDlg::getGw2Ip()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strGw2Ip;
    }

    void SipDlg::setGw2Port(tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iGw2Port = port;
    }

    tars::Int32 SipDlg::getGw2Port()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iGw2Port;
    }


    void SipDlg::setGw2Ip2(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strGw2Ip2 = ip;
    }

    std::string SipDlg::getGw2Ip2()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strGw2Ip2;
    }

    void SipDlg::setGw2Port2(tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iGw2Port2 = port;
    }

    tars::Int32 SipDlg::getGw2Port2()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iGw2Port2;
    }
    void SipDlg::setMiddleGwIp(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMiddleGwIp = ip;
    }
    std::string SipDlg::getMiddleGwIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMiddleGwIp;
    }
    void SipDlg::setMiddleGwPort(tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iMiddleGwPort = port;
    }
    tars::Int32 SipDlg::getMiddleGwPort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iMiddleGwPort;
    }

    void SipDlg::setGw3Ip(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strGw3Ip = ip;
    }

    std::string SipDlg::getGw3Ip()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strGw3Ip;
    }
    void SipDlg::setGw3Port(tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iGw3Port = port;
    }

    tars::Int32 SipDlg::getGw3Port()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iGw3Port;
    }

    void SipDlg::setSipMsg(const std::string & msg)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSipMsg = msg;
    }

    std::string SipDlg::getSipMsg()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSipMsg;
    }

    void SipDlg::setMrfpMsg(const std::string & msg)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strSipMsg = msg;
    }

    std::string SipDlg::getMrfpMsg()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strSipMsg;
    }

    void SipDlg::setResendMsg(const std::string & msg, const std::string & gwOid, const std::string & connId, const std::string & recvConnId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_strResendMsg = msg;
        m_strResendGWOid = gwOid;
        m_strResendConId = connId;
        m_strRecvConnId = recvConnId;
    }

    void SipDlg::getResendMsg(std::string & msg, std::string & gwOid, std::string & connId, std::string & recvConnId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        msg = m_strResendMsg;
        gwOid = m_strResendGWOid;
        connId = m_strResendConId;
        recvConnId = m_strRecvConnId;
    }

    void SipDlg::setCCId(tars::Int32 ccId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iCCId = ccId;
    }

    tars::Int32 SipDlg::getCCId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iCCId;
    }

    void SipDlg::setSessionId(tars::Int32 sesionId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iSessionId = sesionId;
    }
    tars::Int32 SipDlg::getSessionId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iSessionId;
    }

    void SipDlg::setChargedTime(tars::Int64 acceptedTime)
    {
        m_lLastChargeTime = acceptedTime;
    }

    tars::Int64 SipDlg::getLastChargedTime()
    {
        return m_lLastChargeTime;
    }

    void SipDlg::setSdpIp(const std::string & ip)
    {
        m_strSdpIp = ip;
    }

    std::string SipDlg::getSdpIp()
    {
        return m_strSdpIp;
    }

    void SipDlg::setAcceptedTime(const tars::Int64 & time)
    {
        m_lAcceptedTime = time;
    }

    tars::Int64 SipDlg::getAcceptedTime()
    {
        return m_lAcceptedTime;
    }
    void SipDlg::setAlertingTime(const tars::Int64 & time)
    {
        m_lAlertingTime = time;
    }
    tars::Int64 SipDlg::getAlertingTime()
    {
        return m_lAlertingTime;
    }

    void SipDlg::setTermingTime()
    {
        m_lTermingTime = TNOWMS;
    }

    tars::Int64 SipDlg::getTermingTime()
    {
        return m_lTermingTime;
    }

    void SipDlg::setCCStatusType(tars::Int32 iCCStatusType)
    {
        m_iCCStatusType = iCCStatusType;
    }

    tars::Int32 SipDlg::getCCStatusType()
    {
        return m_iCCStatusType;
    }

    void SipDlg::setRealReqTime(tars::Int32 time)
    {
         m_iRealReqTime = time;
    }

    tars::Int32 SipDlg::getRealReqTime()
    {
        return m_iRealReqTime;
    }

    void SipDlg::setTotalTime(tars::Int32 time)
    {
        m_iTotalTime = time;
    }

    tars::Int32 SipDlg::getTotalTime()
    {
        return m_iTotalTime;
    }

    void SipDlg::setUsedTime(tars::Int32 time)
    {
        m_iUserdTime += time;
    }

    tars::Int32 SipDlg::getUsedTime()
    {
        return m_iUserdTime;
    }

    void SipDlg::setFromRoute(tars::Bool bRoute)
    {
        m_bFromRoutePres = bRoute;
    }

    tars::Bool SipDlg::getFromRoute()
    {
        return m_bFromRoutePres;
    }

    void SipDlg::setToRoute(tars::Bool bRoute)
    {
        m_bToRoutePres = bRoute;
    }

    tars::Bool SipDlg::getToRoute()
    {
        return m_bToRoutePres;
    }

    void SipDlg::setFromNATPres(tars::Bool bflag)
    {
        m_bFromNATPres = bflag;
    }
    tars::Bool SipDlg::getFromNATPres()
    {
        return m_bFromNATPres;
    }
    void SipDlg::setToNATPres(tars::Bool bflag)
    {
        m_bToNATPres = bflag;
    }
    tars::Bool SipDlg::getToNATPres()
    {
        return m_bToNATPres;
    }
    void SipDlg::setFromNATIp(const std::string & ip)
    {
        m_strFromNATIp = ip;
    }
    std::string SipDlg::getFromNATIp()
    {
        return m_strFromNATIp;
    }
    void SipDlg::setFromNATPort(tars::Int32 iPort)
    {
        m_iFromNATPort = iPort;
    }
    tars::Int32 SipDlg::getFromNATPort()
    {
        return m_iFromNATPort;
    }
    void SipDlg::setToNATIp(const std::string & ip)
    {
        m_strToNATIp = ip;
    }
    std::string SipDlg::getToNATIp()
    {
        return m_strToNATIp;
    }
    void SipDlg::setToNATPort(tars::Int32 iPort)
    {
        m_iToNATPort = iPort;
    }
    tars::Int32 SipDlg::getToNATPort()
    {
        return m_iToNATPort;
    }
    void SipDlg::setDomainId(const std::string & domainId)
    {
        m_strDomainId = domainId;
    }

    std::string SipDlg::getDomainId()
    {
        return m_strDomainId;
    }

    void SipDlg::setChargeAccount(const std::string & account)
    {
        m_strChargeAccount = account;
    }

    std::string SipDlg::getChargeAccount()
    {
        return m_strChargeAccount;
    }

    void SipDlg::setConnectionId(const std::string & id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strConnectionId = id;
    }

    std::string SipDlg::getConnectionId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strConnectionId;
    }

    void SipDlg::setAtsOid(const std::string & id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strAtsOid = id;
    }

    std::string SipDlg::getAtsOid()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strAtsOid;
    }

    void SipDlg::setPAIName(const std::string & name, const std::string & host)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (host.empty())
            m_strPAIName = "<tel:" + name + ">";
        else
            m_strPAIName = "<sip:" + name + "@" + host + ">";
    }

    std::string SipDlg::getPAIName()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strPAIName;
    }

    void SipDlg::setMeetingId(const std::string & id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMeetingId = id;
    }

    std::string SipDlg::getMeetingId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMeetingId;
    }

    void SipDlg::setJegoParams(const std::string & lastGwIp, const std::string & enterIp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strLastGwIp = lastGwIp;
        m_strEnterpriseIp = enterIp;
    }
    void SipDlg::getJegoParams(std::string & lastGwIp, std::string & enterIp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        lastGwIp =  m_strLastGwIp;
        enterIp = m_strEnterpriseIp;
    }
    void SipDlg::setOriginalCallId(const std::string & callId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strOriginalCallId = callId;
    }
    std::string SipDlg::getOriginalCallId()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strOriginalCallId;
    }
    void SipDlg::setMiddleCallParams(const tars::Int32 type, const std::string & displayName, const std::string & userName, const std::string & host, const tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iMiddleReqType = type;
        m_strMiddleDisplayName = displayName;
        m_strMiddleNumber = userName;
        m_strMiddleHost = host;
        m_iMiddlePort = port;
    }
    void SipDlg::getMiddleCallParams(tars::Int32 & type, std::string & displayName, std::string & userName, std::string & host, tars::Int32 & port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        type  = m_iMiddleReqType;
        displayName = m_strMiddleDisplayName;
        userName = m_strMiddleNumber;
        host = m_strMiddleHost;
        port = m_iMiddlePort;
    }
    void SipDlg::setCallerParams(tars::Bool isFrom, const tars::Int32 type, const std::string & displayName, const std::string & userName, const std::string & host, const tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (isFrom)
        {
            m_iCallerFromReqType = type;
            m_strCallerFromDisplayName = displayName;
            m_strCallerFromNumber = userName;
            m_strCallerFromHost = host;
            m_iCallerFromPort = port;
        }
        else
        {
            m_iCallerToReqType = type;
            m_strCallerToDisplayName = displayName;
            m_strCallerToNumber = userName;
            m_strCallerToHost = host;
            m_iCallerToPort = port;
        }
    }

    void SipDlg::getCallerParams(tars::Bool isFrom, tars::Int32 & type, std::string & displayName, std::string & userName, std::string & host, tars::Int32 & port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (isFrom)
        {
            type  = m_iCallerFromReqType;
            displayName = m_strCallerFromDisplayName;
            userName = m_strCallerFromNumber;
            host = m_strCallerFromHost;
            port = m_iCallerFromPort;
        }
        else
        {
            type  = m_iCallerToReqType;
            displayName = m_strCallerToDisplayName;
            userName = m_strCallerToNumber;
            host = m_strCallerToHost;
            port = m_iCallerToPort;
        }
    }

    void SipDlg::setCallerParamsByJego(tars::Bool isFrom, const tars::Int32 type, const std::string & displayName, const std::string & userName, const std::string & host, const tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (isFrom)
        {
            m_iCallerFromReqTypeByJego = type;
            m_strCallerFromDisplayNameByJego = displayName;
            m_strCallerFromNumberByJego = userName;
            m_strCallerFromHostByJego = host;
            m_iCallerFromPortByJego = port;
        }
        else
        {
            m_iCallerToReqTypeByJego = type;
            m_strCallerToDisplayNameByJego = displayName;
            m_strCallerToNumberByJego = userName;
            m_strCallerToHostByJego = host;
            m_iCallerToPortByJego = port;
        }
    }
    void SipDlg::getCallerParamsByJego(tars::Bool isFrom, tars::Int32 & type, std::string & displayName, std::string & userName, std::string & host, tars::Int32 & port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (isFrom)
        {
            type  = m_iCallerFromReqTypeByJego;
            displayName = m_strCallerFromDisplayNameByJego;
            userName = m_strCallerFromNumberByJego;
            host = m_strCallerFromHostByJego;
            port = m_iCallerFromPortByJego;
        }
        else
        {
            type  = m_iCallerToReqTypeByJego;
            displayName = m_strCallerToDisplayNameByJego;
            userName = m_strCallerToNumberByJego;
            host = m_strCallerToHostByJego;
            port = m_iCallerToPortByJego;
        }
    }

    void SipDlg::updateSipUriParams(tars::Bool isFrom, const tars::Int32 type, const std::string & host, const tars::Int32 port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (isFrom)
        {
            m_iCallerFromReqTypeByJego = type;
            m_strCallerFromHostByJego = host;
            m_iCallerFromPortByJego = port;
        }
        else
        {
            m_iCallerToReqTypeByJego = type;
            m_strCallerToHostByJego = host;
            m_iCallerToPortByJego = port;
        }
    }

    void SipDlg::setMediaCoding(vector<std::string> &coding)
    {
        std::lock_guard<std::mutex> lock(m_mutexMediaCoding);
        m_vectMediaCoding = coding;
    }

    vector<std::string> SipDlg::getMediaCoding()
    {
        std::lock_guard<std::mutex> lock(m_mutexMediaCoding);
        return m_vectMediaCoding;
    }
    void SipDlg::setMrfpMediaCoding(tars::Int32 conding)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iMrfpMediaCoding = conding;
    }
    tars::Int32 SipDlg::getMrfpMediaCoding()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iMrfpMediaCoding;
    }
    void SipDlg::setMrfpSdp(const std::string & sdp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMrfpSdp = sdp;
    }
    std::string SipDlg::getMrfpSdp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMrfpSdp;
    }

    void SipDlg::setQosIps(set<std::string>& ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_vectQosIps = ip;
    }

    set<std::string> SipDlg::getQosIps()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_vectQosIps;
    }

    // void SipDlg::createTimer(const Common::ApplicationExPtr & application, const ChargeTimerExecutorPtr & ptr, tars::Int32 time)
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (m_waitTimer)
    //         m_waitTimer->close();

    //     m_chargeTimerExecutorPtr = ptr;
    //     m_waitTimer = application->createTimer(this);
    //     m_waitTimer->start(time);
    // }

    // void SipDlg::closeTimer()
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     if (m_waitTimer)
    //     {
    //         m_waitTimer->close();
    //         m_waitTimer = 0;
    //     }
    // }

    void SipDlg::pushbackDecodeObj(const SipMsgDecodePtr & obj)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_vectSipMsgCodec.push_back(obj);
    }

    void SipDlg::setBranch(const std::string & branch)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strViaBranch = branch;
    }

    std::string SipDlg::getBranch()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strViaBranch;
    }

    void SipDlg::setCallForwarding(const std::string & number)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCallForwarding = number;
    }

    std::string SipDlg::getCallForwarding()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCallForwarding;
    }

    void SipDlg::setOCSChargePres(tars::Bool pres)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bStartOcsPres = pres;
    }

    tars::Bool SipDlg::getOCSChargePres()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bStartOcsPres;
    }

    void SipDlg::setStopChargePres(tars::Bool pres)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bStopCharfePres = pres;
    }

    tars::Bool SipDlg::getStopChargePres()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bStopCharfePres;
    }

    void SipDlg::setCallerEnterpriseType(EnterpriseType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_callerEnterpriseType = type;
    }
    EnterpriseType SipDlg::getCallerEnterpriseType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_callerEnterpriseType;
    }
    void SipDlg::setCalleeEnterpriseType(EnterpriseType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_calleeEnterpriseType = type;
    }
    EnterpriseType SipDlg::getCalleeEnterpriseType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_calleeEnterpriseType;
    }
    void SipDlg::setMiddleCalleeNumber(const std::string & number)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMiddleCallNumber = number;
    }
    std::string SipDlg::getMiddleCalleeNumber()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMiddleCallNumber;
    }
    void SipDlg::setMiddleCallConnection(const std::string & connectionId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMiddleCallConnectionId = connectionId;
    }
    std::string SipDlg::getMiddleCallConnection()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMiddleCallConnectionId;
    }
    void SipDlg::setRecord(tars::Bool isRecord)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bIsRecord = isRecord;
    }
    tars::Bool SipDlg::getRecord()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bIsRecord;
    }
    void SipDlg::setMrfpSdpIp(const std::string & ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMrfpSdpIp = ip;
    }
    std::string SipDlg::getMrfpSdpIp()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMrfpSdpIp;
    }
    void SipDlg::setMrfpSdpPort(const std::string & ports)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMrfpSdpPorts = ports;
    }
    std::string SipDlg::getMrfpSdpPort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMrfpSdpPorts;
    }
    void SipDlg::setMrfpCodec(const std::string & code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strMrfpCode = code;
    }
    std::string SipDlg::getMrfpCodec()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strMrfpCode;
    }
    void SipDlg::setMrfpBeepErrCode(tars::Int32 code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iBeepErrCode = code;
    }
    tars::Int32 SipDlg::getMrfpBeepErrCode()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iBeepErrCode;
    }
    void SipDlg::setMrfpInitPres(tars::Bool pres)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bMrfpInitPres = pres;
    }
    tars::Bool SipDlg::getMrfpInitPres()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bMrfpInitPres;
    }
    void SipDlg::setBeep(tars::Int32 beep)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iBeep = beep;
    }
    tars::Int32 SipDlg::getBeep()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iBeep;
    }
    void SipDlg::setBeepPres(tars::Bool beepPres)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bBeepPres = beepPres;
    }
    tars::Bool SipDlg::getBeepPres()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bBeepPres;
    }
    void SipDlg::setResendPres(tars::Bool resendPres)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bResendPres = resendPres;
    }
    tars::Bool SipDlg::getResendPres()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bResendPres;
    }
    void SipDlg::setMrfpRingStatus(MrfpRingStatus status)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_mrfpRingStatus = status;
    }
    MrfpRingStatus SipDlg::getMrfpRingStatus()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_mrfpRingStatus;
    }
    void SipDlg::setServiceType(tars::Int32 type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iServiceType = type;
    }
    tars::Int32 SipDlg::getServiceType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iServiceType;
    }
    void SipDlg::setRelease(tars::Bool release)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bRelease = release;
    }
    tars::Int32 SipDlg::getRelease()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bRelease;
    }

    void SipDlg::setRecordRoute(tars::Bool isRecordRoute)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bRecordRoute = isRecordRoute;
    }
    tars::Int32 SipDlg::getRecordRoute()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_bRecordRoute;
    }

    void SipDlg::setReleaseType(CallReleaseType type)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_releaseType = type;
    }
    CallReleaseType SipDlg::getReleaseType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_releaseType;
    }

    void SipDlg::setOcsErrorCode(tars::Int32 code)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iOcsErrCode = code;
    }
    tars::Int32 SipDlg::getOcsErrorCode()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iOcsErrCode;
    }

    void SipDlg::setCmiCovsInfo(const std::string &val)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strCmiCovsInfo = val;
    }

    std::string SipDlg::getCmiCovsInfo()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strCmiCovsInfo;
    }

    tars::Bool SipDlg::decodeDomainIdAppId(const std::string & domainIdAndAppId, std::string & domainId, std::string & appId)
    {
        //format :  9999:0
        tars::Int32  pos = domainIdAndAppId.find(":");
        if (pos < 0)
            return false;
        domainId = domainIdAndAppId.substr(0,pos);
        appId = domainIdAndAppId.substr(pos + 1, domainIdAndAppId.size() - (pos + 1));
        return true;
    }

    void SipDlg::decodeKeyValPairsConfig(const std::string & cfg, std::map<std::string, std::string> & pairs)
    {
        //"102286:12002;102303:1000";
        if (cfg.empty())
            return;

        tars::Int32 pos1 = cfg.find(":");
        tars::Int32 pos2 = cfg.find(";");
        if (pos1 < 0)
            return;

        tars::Int32 curPos = 0;
        while (pos1 > 0)
        {
            std::string subStr;
            if (pos2 > 0)
                subStr = cfg.substr(curPos, pos2 - curPos);
            else
                subStr = cfg.substr(curPos);

            pos1 = subStr.find(":");
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

    void SipDlg::decodeKeyValPairsConfig2(const std::string & cfg, std::vector<std::string> & pairs)
    {
        //"102286;1000;";
        if (cfg.empty())
            return;

        int pos1 = cfg.find(";");
        if (pos1 < 0)
            return;

        tars::Int32 curPos = 0;
        while (pos1 > 0)
        {
            std::string prefix;
            prefix = cfg.substr(curPos, pos1 - curPos);
            pairs.push_back(prefix);
            curPos = pos1 + 1;
            pos1 = cfg.find(";", curPos);
            //UTIL_LOG_IFO("CloudSipGateway", "****** prefix:" + prefix);
        }
    }

    void SipDlg::decodeKeyValPairsConfig3(const std::string & cfg, vector<tars::Int32> & pairs)
    {
        if (cfg.empty())
            return;
        int pos1 = cfg.find(";");
        if (pos1 <= 0)
            return;
        tars::Int32 curPos = 0;
        while (pos1 > 0)
        {
            std::string prefix;
            prefix = cfg.substr(curPos, pos1 - curPos);
            tars::Int32 port = TC_Common::strto<tars::Int32>(prefix);
            pairs.push_back(port);
            curPos = pos1 + 1;
            pos1 = cfg.find(";", curPos);
        }
    }

    void SipDlg::decodeKeyValPairsConfig4(const std::string & cfg, map<std::string,VoipApp::authInfo> & mapInfo)
    {
        //"1:st123-#ss84;2:77u21-khe67!";
        if (cfg.empty())
            return;

        std::string tmp = cfg;
            tars::Int32 pos = tmp.find(";");
        while(pos > 0)
        {
            std::string subStr = tmp.substr(0, pos);

            tars::Int32 pos2 = cfg.find(":");
            tars::Int32 pos3 = cfg.find("-");
            if(pos2 < 0 || pos3 < 0)
                break;

            std::string enterType = subStr.substr(0, pos2);

            VoipApp::authInfo info;
            info._authName = subStr.substr(pos2 + 1, pos3 - pos2 - 1);
            info._authPwd = subStr.substr(pos3 + 1, subStr.size() - pos3 - 1);
            mapInfo.insert(make_pair(enterType, info));

            TLOGINFO(std::string(__FUNCTION__) << "() called. enterType:" << enterType << ", authName:" << info._authName << ", pwd" << info._authPwd << endl);

            tmp = tmp.substr(pos + 1, tmp.size() - pos -1);
            pos = tmp.find(";");
        }
    }

    void SipDlg::decodeKeyValPairsConfig(const std::string & cfg, vector<std::string> & pairs)
    {
        if (cfg.empty())
            return;
        int pos1 = cfg.find(";");
        if (pos1 <= 0)
            return;
        tars::Int32 curPos = 0;
        while (pos1 > 0)
        {
            std::string prefix;
            prefix = cfg.substr(curPos, pos1 - curPos);
            pairs.push_back(prefix);
            curPos = pos1 + 1;
            pos1 = cfg.find(";", curPos);
        }
    }

    void SipDlg::decodeKeyValPairsConfig5(const std::string & cfg, std::map<std::string, vector<VoipApp::enterPrefix>> & mapInfo)
    {
        //"192.168.1.1:2,+86-3,00;192.168.1.2:2,+86,3,00;"
        if (cfg.empty())
            return;

        mapInfo.clear();
        std::string tmp = cfg;
            tars::Int32 pos = tmp.find(";");
        while(pos > 0)
        {
            std::string subStr = tmp.substr(0, pos);

            int pos2 = tmp.find(":");
            if(pos2 < 0)
                break;

            std::string enterIp = subStr.substr(0, pos2);
            std::string prefixStr = subStr.substr(pos2 + 1, pos - pos2 - 1);
            vector<VoipApp::enterPrefix> vecPrefix;
            tars::Int32 pos4 = prefixStr.find(",");
            tars::Int32 index = 0;
            while(pos4 > 0)
            {
                int pos3 = prefixStr.find("-");
                VoipApp::enterPrefix prefix;
                if (pos3 > 0)
                {
                    std::string tmpStr = prefixStr.substr(0, pos3);
                    prefix._numberBit = TC_Common::strto<tars::Int32>(tmpStr.substr(0, pos4));
                    prefix._prefix = tmpStr.substr(pos4 + 1, pos3 - pos4 - 1);
                    vecPrefix.push_back(prefix);
                    prefixStr = prefixStr.substr(pos3 + 1, prefixStr.size() - pos3 - 1);
                }
                else
                {
                    prefix._numberBit = TC_Common::strto<tars::Int32>(prefixStr.substr(0, pos4));
                    prefix._prefix = prefixStr.substr(pos4 + 1, prefixStr.size() - pos4 - 1);
                    vecPrefix.push_back(prefix);
                    prefixStr = prefixStr.substr(pos4 + 1, prefixStr.size() - pos4 - 1);
                }
                pos4 = prefixStr.find(",");
            }
            mapInfo.insert(make_pair(enterIp, vecPrefix));

            tmp = tmp.substr(pos + 1, tmp.size() - pos -1);
            pos = tmp.find(";");
        }
    }

    //------------------------------------------------
    SipDlgManager::SipDlgManager()
    {
        _TotalCreatedDialogsCount = 0;

        _timeoutNo180 = 2 * 60 * 1000;
        _timeoutNo200 = 4 * 60 * 1000;
        _timeoutNormalClosed = 2 * 60 * 1000;
    }

    tars::Int32 SipDlgManager::countSipDialogs()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        return _sipDialogs.size();
    }

    SipDlgPtr SipDlgManager::getSipDialog(const std::string & didStr)
    {
        SipDlgIdPtr did = SipDlgId::fromString(didStr);
        if (did == 0)
            return 0;

        if (did->isValid()==false)
            return 0;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.empty())
            return 0;

        SipDialogsMapType::iterator it = _sipDialogs.find(did->string());
        if (it != _sipDialogs.end())
            return it->second;
        return 0;
    }

    SipDlgPtr SipDlgManager::getSipDialog3(tars::Int32 iSessId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.empty())
            return 0;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            tars::Int32 strTemp = pSipDialog->getCCId();
            if (strTemp == iSessId)
                return pSipDialog;
        }
        return 0;
    }

    SipDlgPtr SipDlgManager::getSipDialogByCallId(const std::string & callId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        if (_sipDialogs.empty())
            return 0;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            std::string strTemp = pSipDialog->getCallIdHost();
            if (strTemp == callId)
                return pSipDialog;
        }
        return 0;
    }

    tars::Bool SipDlgManager::destroySipDialog(const std::string & didStr)
    {
        SipDlgIdPtr did = SipDlgId::fromString(didStr);
        if (did == 0)
            return false;

        if (did->isValid() == false)
            return false;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        SipDialogsMapType::iterator it = _sipDialogs.find(did->string());
        if (it == _sipDialogs.end())
        {
            TLOGWARN(std::string(__FUNCTION__) << "(), NOT found, SipDialog id :" << did->string() << endl);
            return false;
        }
        _sipDialogs.erase(it);
        return true;
    }

    SipDlgPtr SipDlgManager::createSipDialog(const std::string & didStr)
    {
        SipDlgIdPtr did = SipDlgId::fromString(didStr);
        if (did == 0)
            return 0;

        if (did->isValid()==false)
            return 0;

        std::lock_guard<std::mutex> lock(m_mutex);
        SipDlgPtr dialog = getSipDialog(didStr);
        if (dialog)
        {
            TLOGWARN(std::string(__FUNCTION__) << "(),duplicated key in insert,key=" << didStr << endl);
            return 0;
        }

        dialog = SipDlg::create(did);
        if (!dialog)
        {
            TLOGWARN(std::string(__FUNCTION__) << "(),create failed,key=" << didStr << endl);
            return 0;
        }

        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        _sipDialogs.insert(SipDialogsMapType::value_type(didStr, dialog));
        TLOGINFO("{" << did->string() << "}****** SipDlgManager::createSipDialog Have INSERTed  Sip dialog" << endl);

        _TotalCreatedDialogsCount ++;
        return dialog;
    }

    tars::Bool SipDlgManager::removeSipDialog(const std::string & didStr)
    {
        SipDlgIdPtr did = SipDlgId::fromString(didStr);
        if (did == 0)
            return false;

        if (did->isValid()==false)
            return false;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        SipDialogsMapType::iterator it = _sipDialogs.find(didStr);
        if (it == _sipDialogs.end())
        {
            TLOGWARN(std::string(__FUNCTION__) << "(), dialog NOT found:" << didStr << endl);
            return false;
        }
        _sipDialogs.erase(it);
        return true;
    }

    tars::Bool SipDlgManager::insertSipDialog(const std::string & didStr, const SipDlgPtr & dialog)
    {
        SipDlgIdPtr did = SipDlgId::fromString(didStr);
        if (did == 0)
            return false;

        if (did->isValid()==false)
            return false;

        std::lock_guard<std::mutex> lock(m_mutex);
        SipDlgPtr hasDialog = getSipDialog(didStr);
        if (hasDialog)
        {
            TLOGWARN(std::string(__FUNCTION__) << "(),duplicated key in insert,key=" << didStr << endl);
            return false;
        }

        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        _sipDialogs.insert(SipDialogsMapType::value_type(didStr, dialog));
        _TotalCreatedDialogsCount ++;
        return true;
    }

    tars::Bool SipDlgManager::getTimeOutSipDialog(vector<SipDlgPtr> & vectSipDialogs, tars::Int32 & iSipToRpc180, tars::Int32 & iSipToRpc200, tars::Int32 & iRpcToSip180, tars::Int32 & iRpcToSip200,tars::Int32 & iNomalTerm, tars::Int32 & iExcepDeal)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            SipGateway::SipDialogState temStatus = pSipDialog->getSipDialogStatus();
            int64_t lInitStatusTime = pSipDialog->getCurStatusInitTime();
            int64_t lCurTime = TNOWMS;
            SipGateway::TimeOutType timeOutType = pSipDialog->getTimeOutType();
            bool refreshDialog = pSipDialog->getSessionFreshDilogFlag();

            if (timeOutType == SipGateway::TimeOutTypeNo)
            {
                if (temStatus == SipGateway::kSipDialogStateInit)
                {
                    int64_t  diffTime = lCurTime - lInitStatusTime;
                    if (diffTime > _timeoutNo180)
                    {
                        pSipDialog->setTimeOutType(SipGateway::TimeOutTypeSipToRpcNo180);
                        vectSipDialogs.push_back(pSipDialog);
                        iSipToRpc180 ++;

                        SipDlg::autoAddExceptionNum();
                    }
                }
                else if (temStatus == SipGateway::kSipDialogStateEarly)
                {
                    int64_t  diffTime = lCurTime - lInitStatusTime;
                    if (diffTime > _timeoutNo200)
                    {
                        pSipDialog->setTimeOutType(SipGateway::TimeOutTypeSipToRpcNo200);
                        vectSipDialogs.push_back(pSipDialog);
                        iSipToRpc200 ++;

                        SipDlg::autoAddExceptionNum();
                    }
                }
                else if (temStatus == SipGateway::kSipDialogStateTermed || temStatus == SipGateway::kSipDialogStateCanced)
                {
                    int64_t  terminateTime = pSipDialog->getTerminateTime();
                    int64_t  diffTime = lCurTime - terminateTime;
                    if (diffTime > _timeoutNormalClosed)
                    {
                        pSipDialog->setTimeOutType(SipGateway::TimeOutTypeNomalTerminated);
                        vectSipDialogs.push_back(pSipDialog);
                        iNomalTerm ++;
                    }
                }
                else if (temStatus == SipGateway::kSipDialogStateCancing || temStatus == SipGateway::kSipDialogStateTermnating)
                {
                    int64_t  terminateTime = pSipDialog->getTermingTime();
                    int64_t  diffTime = lCurTime - terminateTime;
                    if (diffTime > _timeoutNormalClosed)
                    {
                        pSipDialog->setTimeOutType(SipGateway::TimeOutTypeNomalTerminated);
                        vectSipDialogs.push_back(pSipDialog);
                        iNomalTerm ++;
                    }
                }
            }
            else if (SipGateway::TimeOutTypeExceedMaxDuration == timeOutType)
            {
                vectSipDialogs.push_back(pSipDialog);
            }
            else
            {
                int64_t  enterExceptionTime = pSipDialog->getEnterExceptionTime();
                int64_t  diffTime = lCurTime - enterExceptionTime;
                if (diffTime > _timeoutNormalClosed)
                {
                    pSipDialog->setTimeOutType(SipGateway::TimeOutTypeExceptionDealTimeOut);
                    vectSipDialogs.push_back(pSipDialog);
                    iExcepDeal ++;
                }
            }
        }

        return true;
    }

    tars::Bool SipDlgManager::getExceedMaxDurationSipDialog(vector<SipDlgPtr> & vectSipDialgs)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            int64_t lCurTime = TNOWMS;
            int64_t lCreateTime = pSipDialog->getCreateTime();
            if ((lCurTime - lCreateTime) > TimeOutMaxDialogActiveDuration)
            {
                vectSipDialgs.push_back(pSipDialog);

                std::string strSipDlgId = pSipDialog->id();
                SipGateway::SipDialogState  statusType = pSipDialog->getSipDialogStatus();

                time_t createTimeSec = lCreateTime / 1000;
                int createTimeMs = lCreateTime % 1000;
                std::string strCreateTime = TC_Common::tm2str(createTimeSec, "%Y-%m-%d %H:%M:%S") + ":" + TC_Common::outfill(TC_Common::tostr(createTimeMs), '0', 3, false);

                int64_t  enterExceptionTime = pSipDialog->getEnterExceptionTime();
                time_t exceptionTimeSec = enterExceptionTime / 1000;
                int exceptionTimeMs = enterExceptionTime % 1000;
                std::string strExceptionTime = TC_Common::tm2str(exceptionTimeSec, "%Y-%m-%d %H:%M:%S") + ":" + TC_Common::outfill(TC_Common::tostr(exceptionTimeMs), '0', 3, false);

                SIPGW_LOG_IFO("SipDialog Exceed TimeOutMaxDialogActiveDuration SipDialog, id :" + strSipDlgId + " ,STATUS Type: " + TC_Common::tostr((int)statusType)
                    + " ,TIMEOUT Type: " + TC_Common::tostr((int)pSipDialog->getTimeOutType())
                    + " ,CREATE Time: " + strCreateTime + " ,ENTER EXCEPTION Time: " + strExceptionTime);

                pSipDialog->setTimeOutType(SipGateway::TimeOutTypeExceedMaxDuration);
            }
        }
        return true;
    }

    tars::Bool SipDlgManager::getChargeTimingOutSipDialog(vector<SipDlgPtr> & vectSipDialogs)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        int64_t lCurTime = TNOWMS;
        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            int64_t lLastTime = pSipDialog->getLastChargedTime();
            if (lLastTime == 0)
                continue;

            SipGateway::SipDialogState status = pSipDialog->getSipDialogStatus();
            if (status == SipGateway::kSipDialogStateTermed || status == SipGateway::kSipDialogStateTermnating || status == SipGateway::kSipDialogStateErase)
                continue;

            tars::Int32 totalTime = pSipDialog->getTotalTime();
            int64_t diffTime = lCurTime - lLastTime;
            if (totalTime > 0)
            {
                if (diffTime >= totalTime * 1000)
                {
                    pSipDialog->setReleaseType(EN_CALL_RELEASE_BY_SERVER_NO_MORE_TIME);
                    vectSipDialogs.push_back(pSipDialog);
                }
                else
                {
                     if(totalTime >= 10)
                     {
                         if (diffTime >= (totalTime - 10) * 1000)
                             vectSipDialogs.push_back(pSipDialog);
                     }
                     else if(totalTime >= 5)
                     {
                         if (diffTime >= (totalTime - 5) * 1000)
                            vectSipDialogs.push_back(pSipDialog);
                     }
                }
            }
            else if (diffTime >= 5000)
            {
                pSipDialog->setReleaseType(EN_CALL_RELEASE_BY_OCS_UPDATE_TIME_OUT);
                vectSipDialogs.push_back(pSipDialog);
            }
        }
        return true;
    }

    tars::Bool SipDlgManager::getMrfpTimeOutSipDialog(vector<SipDlgPtr> & vectSipDialogs)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);
        if (_sipDialogs.size() == 0)
            return false;
        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;
            int64_t lInitStatusTime = pSipDialog->getCreateTime();
            int64_t lCurTime = TNOWMS;
            int64_t  diffTime = lCurTime - lInitStatusTime;
            if (diffTime > 60 * 1000)
            {
                vectSipDialogs.push_back(pSipDialog);
            }
        }

        return true;
    }

    tars::Bool SipDlgManager::getToBeResendSipDialogs(vector<SipDlgPtr> & vectSipDialogs)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        int64_t curTime = TNOWMS;
        SipDialogsMapType::iterator it = _sipDialogs.begin();
        SipDlgPtr pSipDialog = 0;

        for (; it != _sipDialogs.end(); it++)
        {
            pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            int64_t sendTime = pSipDialog->getSendTime();
            if (sendTime == 0)
                continue;

            if (pSipDialog->getResendInvite())
            {
                SipGateway::SipDialogState status = pSipDialog->getSipDialogStatus();
                int64_t diffTime = curTime - sendTime;
                if((status == SipGateway::kSipDialogStateInit) && diffTime > 1000 || (status == SipGateway::kSipDialogState200Ok) && diffTime >= 1000)
                    vectSipDialogs.push_back(pSipDialog);
            }
        }

        return true;
    }

    tars::Bool SipDlgManager::checkRefreshSessionRequest(const SipDlgPtr & pDialog , tars::Bool & bRefresh,  SipDlgPtr & pOriginalDialog,  tars::Bool & bRefreshPre, tars::Bool & bUacRefresh)
    {
        if (pDialog == 0)
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
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            std::string strTemp = pSipDialog->_sipMsgDecodePart._callIdPart._strNumber + "@" + pSipDialog->_sipMsgDecodePart._callIdPart._strHost;
            if (strTemp == strCallId)
            {
                pOriginalDialog = pSipDialog;
                bRefresh = true;
                SIPGW_LOG_DBG(std::string(__FUNCTION__) + "() called, strCallId id:" + strCallId + " ,bRefresh = TRUE");

                SipGateway::SipDialogState dialogStatus = pSipDialog->getSipDialogStatus();
                if (dialogStatus <= SipGateway::kSipDialogState200Ok)
                {
                    bRefresh = false;
                    return true;
                }

                pDialog->setAuthType(pSipDialog->getAuthType());

                bRefreshPre = pDialog->_sipMsgDecodePart._sessionExpiresPart._bRefreshPre;
                bUacRefresh = pDialog->_sipMsgDecodePart._sessionExpiresPart._bUAS_refresh;

                pSipDialog->_sipMsgDecodePart._sessionExpiresPart = pDialog->_sipMsgDecodePart._sessionExpiresPart;
                pSipDialog->_sipMsgDecodePart._bodyPart._strBody = pSipDialog->getSdpOffer();

                pDialog->setIsFreshDialog(true);
                pSipDialog->setIsFreshDialog(true);

                return true;
            }
        }

        SIPGW_LOG_DBG(std::string(__FUNCTION__) + "() called, strCallId id:" + strCallId + " ,bRefresh = FALSE");
        return true;
    }


















    tars::Bool SipDlgManager::getOriginalSipDialog(const SipDlgPtr & pDialog,  SipDlgPtr & pOriginalDialog)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return false;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        std::string strCallId = pDialog->_sipMsgDecodePart._callIdPart._strNumber + "@" + pDialog->_sipMsgDecodePart._callIdPart._strHost;
        SipDlgPtr pSipDialog;
        for (; it != _sipDialogs.end(); it++)
        {
            pSipDialog = it->second;
            if (pSipDialog == 0)
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

    SipDlgPtr SipDlgManager::getSipDialog2(const std::string & callIdHost)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return 0;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            std::string strTemp = pSipDialog->getCallIdHost();
            if (strTemp == callIdHost)
                return pSipDialog;
        }
        return 0;
    }

    void SipDlgManager::setTimeoutFor180(tars::Int64 times)
    {
        _timeoutNo180 = times;
    }

    void SipDlgManager::setTimeoutFor200(tars::Int64 times)
    {
        _timeoutNo200 = times;
    }

    void SipDlgManager::setTimeoutForClosed(tars::Int64 times)
    {
        _timeoutNormalClosed = times;
    }

    void SipDlgManager::pintOutSipDialogMapSize()
    {
        TLOGINFO(std::string(__FUNCTION__) << "() called,  the _sipDialogs SIZE is : " << _sipDialogs.size() << endl);
    }

    void SipDlgManager::printAllSipDialog()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lockDialogs(_sipDialogsMutex);

        if (_sipDialogs.size() == 0)
            return;

        SipDialogsMapType::iterator it = _sipDialogs.begin();
        for (; it != _sipDialogs.end(); it++)
        {
            SipDlgPtr pSipDialog = it->second;
            if (pSipDialog == 0)
                continue;

            std::string strTemp = pSipDialog->getCallIdHost();
            std::string strTempId = pSipDialog->id();
            TLOGWARN("***** SipDlgManager::printAllSipDialog callId:" << strTemp << ", dialogId:" << strTempId << endl);
        }
    }

    tars::Bool SipDlgDbRecordManager::insertRecord(const std::string & rpcCallId, const std::string & sipCallId, const std::string & fromUser, const std::string & toUser, const std::string & toPhone,
            const std::string & domainId, const std::string & appId, tars::Bool bConfDialog, tars::Bool bAppToSip, tars::Int64 st, tars::Int64 et, tars::Int32 td,const std::string & sipGtwId,
            const std::string & sipProxyId, const std::string & coreNetId, tars::Int32 mediaType, const std::string  & confUri, const std::string & confMid, tars::Int32 errCode, tars::Bool finished)
    {
        VoipApp::SipCallRecord dbRecord;

        dbRecord.m_st = st;
        dbRecord.m_et = et;
        dbRecord.m_fromUser = fromUser;
        dbRecord.m_toUser = toUser;
        dbRecord.m_toPhone = toPhone;
        dbRecord.m_rpcCallId = rpcCallId;
        dbRecord.m_sipCallId = sipCallId;
        dbRecord.m_td = td;
        dbRecord.m_type = bConfDialog ? 1 : 0;
        dbRecord.m_direction = bAppToSip ? 0 : 1;
        dbRecord.m_confUri = confUri;
        dbRecord.m_confMid = confMid;
        dbRecord.m_cmt = mediaType;
        dbRecord.m_sipGatewayId = sipGtwId;
        dbRecord.m_sipProxyId = sipProxyId;
        dbRecord.m_coreNetId = coreNetId;
        dbRecord.m_errCode = errCode;

        SipGateway::SipDialogDbRecord record;
        record.m_dbRecord = dbRecord;
        record.m_domainId = domainId;
        record.m_appId = appId;
        record.m_finished = finished;

        std::lock_guard<std::mutex> lock(m_mutexRecords);
        m_mapRecords[rpcCallId] = record;

        TLOGINFO(std::string(__FUNCTION__) << "() called, CloudSipGatewayId id:" << sipGtwId << " ,sipGateway2Id:" << sipProxyId << endl);
        return true;
    }


    tars::Bool SipDlgDbRecordManager::exportRecords(map<std::string, SipGateway::SipDialogDbRecord> & callRecords)
    {
        std::lock_guard<std::mutex> lock(m_mutexRecords);
        map<std::string, SipGateway::SipDialogDbRecord>::iterator iterCallId = m_mapRecords.begin();
        for (; iterCallId != m_mapRecords.end(); )
        {
            if (iterCallId->second.m_finished)
            {
                callRecords.insert(make_pair(iterCallId->first, iterCallId->second));
                m_mapRecords.erase(iterCallId++);
            }
            else
            {
                iterCallId++;
            }
        }
        return true;
    }

    tars::Bool SipDlgDbRecordManager::roolBackRecords(const map<std::string, SipGateway::SipDialogDbRecord> & callRecords)
    {
        std::lock_guard<std::mutex> lock(m_mutexRecords);
        m_mapRecords.insert(callRecords.begin(), callRecords.end());
        return true;
    }

    tars::Int32 SipDlgDbRecordManager::getSize()
    {
        std::lock_guard<std::mutex> lock(m_mutexRecords);
        return m_mapRecords.size();
    }

    tars::Bool SipDlgDbRecordManager::checkRepeatCall(const std::string & fromUser, const std::string & toUser, const std::string & toPhone,std::string & rpcCallId, std::string & sipDialogId)
    {
        std::lock_guard<std::mutex> lock(m_mutexRecords);
        map<std::string, SipGateway::SipDialogDbRecord>::iterator iterCallId = m_mapRecords.begin();
        for (; iterCallId != m_mapRecords.end(); iterCallId++)
        {
            SipGateway::SipDialogDbRecord record = iterCallId->second;
            if (record.m_dbRecord.m_fromUser == fromUser && record.m_finished == false)
            {
                sipDialogId = record.m_dbRecord.m_sipCallId;
                return true;
            }
        }
        return false;
    }

    void SipDlgDbRecordManager::exceptionRecordsDeal()
    {
        std::lock_guard<std::mutex> lock(m_mutexRecords);
        int64_t curTime = TNOWMS;
        tars::Int32 num = 0;
        map<std::string, SipGateway::SipDialogDbRecord>::iterator iterCallId = m_mapRecords.begin();
        for (; iterCallId != m_mapRecords.end(); iterCallId++)
        {
            SipGateway::SipDialogDbRecord record = iterCallId->second;
            int64_t diffTime = curTime - record.m_dbRecord.m_st * 1000;
            if (diffTime > TimeOutMaxDialogActiveDuration)
            {
                num++;
                iterCallId->second.m_finished = true;
            }
        }
        if(num > 0)
        {
            TLOGINFO("exception num:" << num << endl);
        }
    }
};

