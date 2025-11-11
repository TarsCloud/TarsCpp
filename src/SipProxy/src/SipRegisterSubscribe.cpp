#include <random>

#include "SipRegisterSubscribe.h"
#include "servant/Application.h"
#include "util/tc_timeprovider.h"
#include "util/tc_uuid_generator.h"

namespace
{
    // Helper function to replace Common::getRand
    int getRandom(int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, max - 1);
        return dis(gen);
    }
}

namespace SipProxy
{
    SipRegisterSubscribe::SipRegisterSubscribe()
    {
        m_iExpire = 0;
        m_ucQopType = 0;
        m_iNc = 1;
        m_bQopExist = false;

        m_lCreateTime = TNOWMS;
        m_eRegStatus = SipRegStatusRecv200;
        m_lCurStatusInitTime = TNOWMS;
        m_iRepeatTimes = 0;
        m_eTimeOutType = RegTimeOutNone;

        m_strId = TC_UUIDGenerator::getInstance()->genID();
        m_iRandomSeq = getRandom(1000000);
        m_lLastRegsiterTime = 0;
        m_algo = 0;
        m_iRegistrarPort = 0;
        m_iClientPort = 0;
        m_sipMethod = SipGateway::kSipRequestMethodREGISTER;
        m_bTmpReg = false;
        m_freshCall = 0;
        m_refreshTicks = 0;
        m_nextSipRefreshTicks = 0;
        m_regTmBuffer = 300;

        ++m_totalRegNum;
        ++m_totalNewRegNum;

        TLOGINFO("SipRegisterSubscribe", "=====> SipRegisterSubscribe() called ! m_strId:" + m_strId  + " ,m_iRandomSeq:," +std::to_string(m_iRandomSeq));
    }


    SipRegisterSubscribe::SipRegisterSubscribe(const std::string & sipUri,const std::string & registerId, const std::string & passWord,
        const std::string & authName, const std::string & sipProxyId, SipGateway::SipRequestMethodType methodType, int regBffer)
        :m_sipUri(sipUri),m_strSipRegisterId(registerId), m_strPassWord(passWord), m_strAuthName(authName),m_strSipProxyId(sipProxyId)
    {
        m_sipMethod = methodType;
        m_ucQopType = 0;
        m_iNc = 1;
        m_bQopExist = false;
        m_iExpire = 0;
        m_lLastRegsiterTime = 0;

        m_sipMsgDecodePart._callIdPart._strNumber = TC_UUIDGenerator::getInstance()->genID() + "_" +  std::to_string(TNOWMS);
        m_sipMsgDecodePart._fromPart._strTag = TC_UUIDGenerator::getInstance()->genID();
        m_strId = m_sipMsgDecodePart._callIdPart._strNumber;

        m_lCreateTime = TNOWMS;
        m_eRegStatus = SipRegStatusRecv200;
        m_lCurStatusInitTime = TNOWMS;
        m_iRepeatTimes = 0;
        m_eTimeOutType = RegTimeOutNone;

        m_iRandomSeq = getRandom(1000000);
        m_algo = 0;
        m_iRegistrarPort = 0;
        m_iClientPort = 0;
        m_bTmpReg = false;
        m_freshCall = 0;
        m_refreshTicks = 0;
        m_nextSipRefreshTicks = 0;
        m_regTmBuffer = regBffer;

        std::string usedModulName = getUseModulName(methodType);
        TLOGINFO("SipRegisterSubscribe", "=====> SipRegisterSubscribe() called : sipUri:" + sipUri + " ,module:" + usedModulName + " ,m_strId:" + m_strId + " ,m_iRandomSeq:," +std::to_string(m_iRandomSeq));

        if (m_sipMethod == SipGateway::kSipRequestMethodREGISTER)
        {
            ++m_totalRegNum;
            ++m_totalNewRegNum;
        }
        else if (m_sipMethod == SipGateway::kSipRequestMethodSUBSCRIBE)
        {
            ++m_totalSubNum;
            ++m_totalNewSubNum;
        }
        else if (m_sipMethod == SipGateway::kSipRequestMethodNOTIFY)
        {
            ++m_totalNotNum;
            ++m_totalNewNotNum;
        }
    }

    SipRegisterSubscribe::SipRegisterSubscribe(const std::string & sipUri,const std::string & registerId, const std::string & passWord,
        const std::string & authName, const std::string & sipProxyId, int64_t httpRegisterTime, SipGateway::SipRequestMethodType methodType, int regBffer)
        :m_sipUri(sipUri),m_strSipRegisterId(registerId),m_strPassWord(passWord),m_strAuthName(authName),m_strSipProxyId(sipProxyId)
    {
        m_lHttpRegisterTime = httpRegisterTime;
        m_sipMethod = methodType;
        m_ucQopType = 0;
        m_iNc = 1;
        m_bQopExist = false;
        m_iExpire = 0;
        m_lLastRegsiterTime = 0;

        m_sipMsgDecodePart._callIdPart._strNumber = TC_UUIDGenerator::getInstance()->genID() + "_" +  std::to_string(TNOWMS);
        m_sipMsgDecodePart._fromPart._strTag = TC_UUIDGenerator::getInstance()->genID();
        m_strId = m_sipMsgDecodePart._callIdPart._strNumber;

        m_lCreateTime = TNOWMS;
        m_eRegStatus = SipRegStatusRecv200;
        m_lCurStatusInitTime = TNOWMS;
        m_iRepeatTimes = 0;
        m_eTimeOutType = RegTimeOutNone;

        std::string usedModulName = getUseModulName(methodType);
        m_iRandomSeq = getRandom(1000000);
        m_algo = 0;
        m_iRegistrarPort = 0;
        m_iClientPort = 0;
        m_bTmpReg = false;
        m_freshCall = 0;
        m_refreshTicks = 0;
        m_nextSipRefreshTicks = 0;
        m_regTmBuffer = regBffer;

        TLOGINFO("SipRegisterSubscribe", "=====> SipRegisterSubscribe() called : sipUri:" + sipUri + " ,module:" + usedModulName  + " ,m_strId:" + m_strId + " ,m_iRandomSeq:," +std::to_string(m_iRandomSeq) );

        if (m_sipMethod == SipGateway::kSipRequestMethodREGISTER)
        {
            ++m_totalRegNum;
            ++m_totalNewRegNum;
        }
        else if (m_sipMethod == SipGateway::kSipRequestMethodSUBSCRIBE)
        {
            ++m_totalSubNum;
            ++m_totalNewSubNum;
        }
        else if (m_sipMethod == SipGateway::kSipRequestMethodNOTIFY)
        {
            ++m_totalNotNum;
            ++m_totalNewNotNum;
        }
    }


    SipRegisterSubscribe:: ~SipRegisterSubscribe()
    {
        std::string usedModulName = getUseModulName(m_sipMethod);
        TLOGINFO("SipRegisterSubscribe", "=====> ~SipRegisterSubscribe() called : sipUri:" + m_sipUri + " ,module:" + usedModulName + ",is tmp reg:" + std::to_string(m_bTmpReg)
            + " ,m_strSipProxy id:" + m_strSipProxyId  + " ,m_strId:" + m_strId + " ,m_iRandomSeq:," + std::to_string (m_iRandomSeq));

        if (m_sipMethod == SipGateway::kSipRequestMethodREGISTER)
        {
            --m_totalRegNum;
            ++m_totalDeleteRegNum;
        }
        else if (m_sipMethod == SipGateway::kSipRequestMethodSUBSCRIBE)
        {
            --m_totalSubNum;
            ++m_totalDeleteSubNum;
        }
        else if (m_sipMethod == SipGateway::kSipRequestMethodNOTIFY)
        {
            --m_totalNotNum;
            ++m_totalDeleteNotNum;
        }
    }


    SipRegisterSubscribePtr SipRegisterSubscribe::create(const std::string & sipUri,const std::string & registerId,
        const std::string & passWord, const std::string & authName, const std::string & sipProxyId, SipGateway::SipRequestMethodType methodType, int regBffer)
    {
        return std::make_shared<SipRegisterSubscribe>(sipUri, registerId, passWord, authName, sipProxyId, methodType, regBffer);
    }

    std::string  SipRegisterSubscribe::getId()
    {
        return m_strId;
    }

    void SipRegisterSubscribe::setExpire(int expire)
    {
        m_iExpire = expire;
    }
    int SipRegisterSubscribe::getExpire()
    {
        return m_iExpire;
    }

    void SipRegisterSubscribe::setHttpReqTime(int64_t reqTime)
    {
        m_lHttpRegisterTime = reqTime;
    }

    void SipRegisterSubscribe::setLastRegisterTime(int64_t lastTime)
    {
        m_lLastRegsiterTime  = lastTime;
        TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, sipUri:" + m_sipUri +  " ,m_strId:" + m_strId + " ,m_iRandomSeq:," +std::to_string(m_iRandomSeq));
    }

    void SipRegisterSubscribe::setIpsAndPorts(const std::string & clientIp, const int & clientPort, const std::string & registrarIp, const int & iRegistrarPort, const std::string & sendIp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_strClientIp = clientIp;
        m_iClientPort = clientPort;
        m_strRegistarIp = registrarIp;
        m_iRegistrarPort = iRegistrarPort;
        m_strSendIp = sendIp;
    }

    void SipRegisterSubscribe::getIpsAndPorts(std::string & clientIp, int & clientPort, std::string & registrarIp, int & registrarPort,std::string & sendIp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        clientIp = m_strClientIp;
        clientPort = m_iClientPort;
        registrarIp = m_strRegistarIp;
        registrarPort = m_iRegistrarPort;
        sendIp = m_strSendIp;
    }

    void SipRegisterSubscribe::setProxyIpsAndPorts(const std::string & listenIp, const int & listenPort, const std::string & sendIp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_strClientIp = listenIp;
        m_iClientPort = listenPort;
        m_strSendIp = sendIp;
    }

    std::string SipRegisterSubscribe::getClientIp()
    {
        return m_strClientIp;
    }

    std::string SipRegisterSubscribe::getSipUri()
    {
        return m_sipUri;
    }

    void SipRegisterSubscribe::setSipUri(const std::string & sipUri)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipUri = sipUri;
    }

    std::string SipRegisterSubscribe::generateSipUri(const std::string & sipUsername, const std::string & registarIp)
    {
        std::string sipUri = sipUsername + "@" + registarIp;
        return sipUri;
    }

    std::string SipRegisterSubscribe::getRegisterId()
    {
        return m_strSipRegisterId;
    }

    void SipRegisterSubscribe::setRegisterId(const std::string & regId)
    {
        m_strSipRegisterId = regId;
    }

    std::string SipRegisterSubscribe::getPassWord()
    {
        return m_strPassWord;
    }

    void SipRegisterSubscribe::setPassWord(const std::string & passWord)
    {
        m_strPassWord = passWord;
    }

    void SipRegisterSubscribe::setDigest(const ST_SIP_DIGEST & stDigest)
    {
        m_stDigest = stDigest;
    }

    ST_SIP_DIGEST SipRegisterSubscribe::getDigest()
    {
        return m_stDigest;
    }

    void SipRegisterSubscribe::setDigestQopType(bool qopExist, unsigned char qopType)
    {
        m_bQopExist = qopExist;
        m_ucQopType = qopType;
    }
    unsigned char SipRegisterSubscribe::getDigestQopType()
    {
        return m_ucQopType;
    }

    bool SipRegisterSubscribe::getDigestQopExist()
    {
        return m_bQopExist;
    }

    void  SipRegisterSubscribe::setDigestRealm(const std::string & realm)
    {
        m_strRealm = realm;
    }

    std::string  SipRegisterSubscribe::getDigestRealm()
    {
        return m_strRealm;
    }

    void  SipRegisterSubscribe::setDigestNonce(const std::string & nonce)
    {
        m_strNonce = nonce;
    }

    std::string  SipRegisterSubscribe::getdigestNonce()
    {
        return m_strNonce;
    }

    void  SipRegisterSubscribe::setDigestNonceCount(int nc)
    {
        m_iNc = nc;
    }

    int  SipRegisterSubscribe::getDigestNonceCount()
    {
        return m_iNc;
    }

    void SipRegisterSubscribe::increaseNonceCount()
    {
        m_iNc++;
    }

    std::string SipRegisterSubscribe::getAuthName()
    {
        return m_strAuthName;
    }
    void SipRegisterSubscribe::setAuthName(const std::string & authName)
    {
        m_strAuthName = authName;
    }

    std::string SipRegisterSubscribe::getDigestUserName()
    {
        return m_strDigestUserName;
    }
    void SipRegisterSubscribe::setDigestUserName(const std::string & userName)
    {
        m_strDigestUserName = userName;
    }

    std::string SipRegisterSubscribe::getDigestOpaque()
    {
        return m_opaque;
    }
    void SipRegisterSubscribe::setDigestOpaque(const std::string & opaque)
    {
        m_opaque = opaque;
    }

    unsigned char  SipRegisterSubscribe::getDigestAlgo()
    {
        return m_algo;
    }
    void  SipRegisterSubscribe::setDigestAlgo(unsigned char algo)
    {
        m_algo = algo;
    }

    std::string  SipRegisterSubscribe::getDigestResponse()
    {
        return m_strResponse;
    }

    void SipRegisterSubscribe::setDigestResponse(const std::string & response)
    {
        m_strResponse = response;
    }

    std::string SipRegisterSubscribe::getDigestCNonce()
    {
        return m_strCNonce;
    }
    void SipRegisterSubscribe::setDigestCNonce(const std::string & cnonce)
    {
        m_strCNonce = cnonce;
    }

    std::string SipRegisterSubscribe::getSipProxyId()
    {
        return m_strSipProxyId;
    }

    void SipRegisterSubscribe::setSipProxyId(const std::string & proxyId)
    {
        m_strSipProxyId = proxyId;
    }

    int64_t SipRegisterSubscribe::getLastHttpReqTime()
    {
        return m_lHttpRegisterTime;
    }

    int64_t SipRegisterSubscribe::getLastRegTime()
    {
        return m_lLastRegsiterTime;
    }

    int64_t SipRegisterSubscribe::getCreateTime()
    {
        return m_lCreateTime;
    }

    void SipRegisterSubscribe::setRegisterStuatus(SipRegisterStatus regStatus, const std::string & path)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_eRegStatus = regStatus;
        m_lCurStatusInitTime = TNOWMS;

        if (regStatus == SipRegStatusRecv200)
        {
            m_iRepeatTimes = 0;
            int dealy = 0;
            if (m_iExpire <= m_regTmBuffer || (m_iExpire / 2 - m_regTmBuffer) < 0)
                dealy = m_iExpire / 2;
            else
                dealy = m_iExpire / 2 + getRandom((m_iExpire / 2 - m_regTmBuffer)) + 60;

            m_nextSipRefreshTicks = TC_TimeProvider::getInstance()->getNowMs() + dealy * 1000;
            TLOGINFO("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, sipUri:"
                + m_sipUri + ",next register time:" + TC_Common::tm2str((m_lCurStatusInitTime + dealy * 1000) / 1000, "%Y-%m-%d %H:%M:%S") + ",last:" +
                TC_Common::tm2str(m_lLastRegsiterTime / 1000, "%Y-%m-%d %H:%M:%S") + ",tmp:" + std::to_string(m_bTmpReg));
            m_lLastRegsiterTime = m_lCurStatusInitTime;
        }

        std::string strCreateTime = TC_Common::tm2str(m_lCurStatusInitTime / 1000, "%Y-%m-%d %H:%M:%S");
        TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, sipUri:" + m_sipUri  + ",curTime:" + strCreateTime + ",regStatus:" + std::to_string(int(regStatus)) + ",path:" + path);
    }

    SipRegisterStatus SipRegisterSubscribe::getRegisterStatus()
    {
        return m_eRegStatus;
    }

    int64_t SipRegisterSubscribe::getCurStatusInitTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_lCurStatusInitTime ;
    }

    void SipRegisterSubscribe::repeatTimeAutoIncrease()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_iRepeatTimes++;
    }

    int SipRegisterSubscribe::getRepeatTimes()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_iRepeatTimes;
    }

    void SipRegisterSubscribe::setTimeOutType(RegTimeOutType typeOutType)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_eTimeOutType = typeOutType;
    }

    RegTimeOutType SipRegisterSubscribe::getTimeOutType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_eTimeOutType;
    }

    bool SipRegisterSubscribe::sipRegisterSubToDbStruc(VoipApp::SipRegInfoStruc & regStruc)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        regStruc._sipUri = m_sipUri;
        regStruc._sipAccount = m_strSipRegisterId;
        regStruc._passWord = m_strPassWord;
        regStruc._authName = m_strAuthName;
        regStruc._sipProxyId = m_strSipProxyId;
        regStruc._lastHttpRegTime = m_lHttpRegisterTime;

        regStruc._lastRegTime = m_lLastRegsiterTime;
        regStruc._expires = m_iExpire;
        regStruc._registarIp = m_sipMsgDecodePart._reqLinePart._strHostName;
        regStruc._registarPort = m_sipMsgDecodePart._reqLinePart._dwPort;
        regStruc._clientIp = m_strClientIp;

        regStruc._clientPort = m_iClientPort;
        regStruc._callId =  m_sipMsgDecodePart._callIdPart._strNumber + "@"+  m_sipMsgDecodePart._callIdPart._strHost;
        regStruc._fromTag = m_sipMsgDecodePart._fromPart._strTag;
        regStruc._toTag = m_sipMsgDecodePart._toPart._strTag;
        regStruc._CSeqNum = m_sipMsgDecodePart._cseqPart._iCseqVal;

        regStruc._viaBranch = m_sipMsgDecodePart._viaPart._strBranch;
        regStruc._event = (int) m_sipMsgDecodePart._eventPart._pucPkgType;
        regStruc._eventId = m_sipMsgDecodePart._eventPart._id;
        regStruc._realm = m_strRealm;
        regStruc._nonce = m_strNonce;

        regStruc._Nc = m_iNc;
        regStruc._opaque = m_opaque;
        regStruc._response = m_strResponse;
        regStruc._qopExist = m_bQopExist ? 1 : 0;
        regStruc._qopValue = m_ucQopType;

        regStruc._Cnonce = m_strCNonce;
        regStruc._reqUserInfo = m_sipMsgDecodePart._reqLinePart._strUserName;
        regStruc._reqUrl = m_sipMsgDecodePart._reqLinePart._reqUri;
        regStruc._coreNetId = m_coreNetInfo.m_strCoreNetId;
        regStruc._SBCIp = m_coreNetInfo.m_strSBCIp;

        regStruc._SBCPort = m_coreNetInfo.m_port;
        regStruc._algo = m_algo;

        return true;
    }

    bool SipRegisterSubscribe::dbStrucToSipRegisterSub(const VoipApp::SipRegInfoStruc & regStruc)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_sipUri = regStruc._sipUri;
        m_strSipRegisterId = regStruc._sipAccount;
        m_strPassWord  = regStruc._passWord;
        m_strAuthName  = regStruc._authName;
        m_strSipProxyId  = regStruc._sipProxyId;
        m_lHttpRegisterTime  = regStruc._lastHttpRegTime;

        m_lLastRegsiterTime = regStruc._lastRegTime ;
        m_iExpire = regStruc._expires ;
        m_sipMsgDecodePart._reqLinePart._strHostName = regStruc._registarIp;
        m_strRegistarIp = regStruc._registarIp;
        m_sipMsgDecodePart._reqLinePart._dwPort = regStruc._registarPort;
        m_iRegistrarPort = regStruc._registarPort;
        m_strClientIp =  regStruc._clientIp;

        m_iClientPort = regStruc._clientPort ;
        int pos1 = regStruc._callId.find("@");
        std::string callNumber = regStruc._callId.substr(0, pos1);
        std::string callHost = regStruc._callId.substr(pos1 + 1, regStruc._callId.size() - pos1 - 1);
        m_sipMsgDecodePart._callIdPart._strNumber = callNumber;
        m_sipMsgDecodePart._callIdPart._strHost = callHost;
        m_sipMsgDecodePart._fromPart._strTag = regStruc._fromTag;
        m_sipMsgDecodePart._toPart._strTag =regStruc._toTag;
        m_sipMsgDecodePart._cseqPart._iCseqVal =  regStruc._CSeqNum;

        m_sipMsgDecodePart._viaPart._strBranch =regStruc._viaBranch;
        m_sipMsgDecodePart._eventPart._pucPkgType =  regStruc._event ;
        m_sipMsgDecodePart._eventPart._id = regStruc._eventId ;
        m_strRealm =  regStruc._realm ;
        m_strNonce  = regStruc._nonce;

        m_iNc = regStruc._Nc ;
        m_opaque  = regStruc._opaque;
        m_strResponse = regStruc._response ;
        m_bQopExist  = regStruc._qopExist;
        m_ucQopType =  regStruc._qopValue;

        m_strCNonce  = regStruc._Cnonce;
        m_sipMsgDecodePart._reqLinePart._strUserName  = regStruc._reqUserInfo;
        m_sipMsgDecodePart._reqLinePart._reqUri  = regStruc._reqUrl;
        m_coreNetInfo.m_strCoreNetId = regStruc._coreNetId;
        m_coreNetInfo.m_strSBCIp = regStruc._SBCIp;

        m_coreNetInfo.m_port = regStruc._SBCPort;
        m_algo = regStruc._algo;

        return true;
    }

    std::string SipRegisterSubscribe::getUseModulName(SipGateway::SipRequestMethodType methodType)
    {
        switch(methodType)
        {
        case SipGateway::kSipRequestMethodREGISTER:
            return "REGISTER";

        case SipGateway::kSipRequestMethodSUBSCRIBE:
            return "SUBSCRIBE";

        case SipGateway::kSipRequestMethodNOTIFY:
            return "NOTIFY";

        case SipGateway::kSipRequestMethodOPTIONS:
            return "OPTIONS";

        default:
            return "Others";
        }
    }

    void SipRegisterSubscribe::setCoreNetInfo(const std::string & coreNetId, const std::string & sbcIp, int sbcPort, const std::string & registarIp, int registarPort)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_coreNetInfo.m_strCoreNetId = coreNetId;
        m_coreNetInfo.m_strSBCIp = sbcIp;
        m_coreNetInfo.m_port = sbcPort;
        m_coreNetInfo.m_strRegistarIp = registarIp;
        m_coreNetInfo.m_iRegistarPort = registarPort;
    }

    CoreNetInfo SipRegisterSubscribe::getCoreNetInfo()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_coreNetInfo;
    }

    void SipRegisterSubscribe::updateSipInfo(const VoipApp::SipAccountInfo & sipAccountInfo)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_strPassWord = sipAccountInfo.m_strPassWord;
        m_strAuthName = sipAccountInfo.m_strAuthName;

        m_coreNetInfo.m_strSBCIp = sipAccountInfo.m_strSBCIp;
        m_coreNetInfo.m_port = sipAccountInfo.m_iSBCport;
        m_coreNetInfo.m_strRegistarIp = sipAccountInfo.m_strRegistarIp;
        m_coreNetInfo.m_iRegistarPort = sipAccountInfo.m_iRegistarPort;
    }

    void SipRegisterSubscribe::setTmpRegFlag(bool tmpReg)
    {
        m_bTmpReg = tmpReg;
    }

    bool SipRegisterSubscribe::getTmpRegFlag()
    {
        return m_bTmpReg;
    }

    void SipRegisterSubscribe::setSipFreshCall(const std::shared_ptr<void> & freshCall)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_freshCall = freshCall;
        if (freshCall)
            m_refreshTicks = TC_TimeProvider::getInstance()->getNowMs();
        else
            m_refreshTicks = 0;
    }
    std::shared_ptr<void> SipRegisterSubscribe::getSipFreshCall()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_freshCall;
    }

    int64_t SipRegisterSubscribe::getRefreshTicks()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_refreshTicks;
    }

    void SipRegisterSubscribe::refreshBasicRegInfo()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipMsgDecodePart._callIdPart._strNumber = TC_UUIDGenerator::getInstance()->genID() + "_" +  std::to_string(TNOWMS);
        m_sipMsgDecodePart._fromPart._strTag = TC_UUIDGenerator::getInstance()->genID();
        m_sipMsgDecodePart._cseqPart._iCseqVal = 1;
    }

    void SipRegisterSubscribe::forceReRegister(const std::string & path,int repeatedTimes)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_eRegStatus = SipRegStatusSendRegNoAuth;
        m_lCurStatusInitTime = TNOWMS - 2 * REGISTER_RETRY_INTERVAL_MORE_THAN_10;
        m_iRepeatTimes = repeatedTimes;

        refreshBasicRegInfo();

        TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, sipUri:" + m_sipUri + ",path:" + path);
    }

    void SipRegisterSubscribe::setDomainId(const std::string & domainId)
    {
        m_domainId = domainId;
    }

    std::string SipRegisterSubscribe::getDomainId()
    {
        return m_domainId;
    }

    int64_t SipRegisterSubscribe::getNextRefreshTicks()
    {
        return m_nextSipRefreshTicks;
    }


    //Test
    std::atomic<int>     SipRegisterSubscribe::m_totalRegNum(0);
    std::atomic<int>     SipRegisterSubscribe::m_totalNewRegNum(0);
    std::atomic<int>     SipRegisterSubscribe::m_totalDeleteRegNum(0);

    std::atomic<int>     SipRegisterSubscribe::m_totalSubNum(0);
    std::atomic<int>     SipRegisterSubscribe::m_totalNewSubNum(0);
    std::atomic<int>     SipRegisterSubscribe::m_totalDeleteSubNum(0);

    std::atomic<int>     SipRegisterSubscribe::m_totalNotNum(0);
    std::atomic<int>     SipRegisterSubscribe::m_totalNewNotNum(0);
    std::atomic<int>     SipRegisterSubscribe::m_totalDeleteNotNum(0);


    //SipRegisterManager function --------------------------------

    bool SipRegisterSubscribeManager::insertSipRegisterSubscribe(const std::string & sipUri, const SipRegisterSubscribePtr & sipRegisterPtr, bool tmpReg)
    {
        if (!tmpReg)
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.find(sipUri);
            if (iter != m_mapSipRegisters.end())
            {
                TLOGWARN("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, failed to insert a SipRegisterPtr, sipUri id:" + sipUri + ",tmp:" + std::to_string(tmpReg));
                return false;
            }
            m_mapSipRegisters.insert(make_pair(sipUri, sipRegisterPtr));
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.find(sipUri);
            if (iter != m_mapTmpSipRegisters.end())
            {
                TLOGWARN("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, failed to insert a SipRegisterPtr,sipUri id:" + sipUri + ",tmp:" + std::to_string(tmpReg));
                return false;
            }
            m_mapTmpSipRegisters.insert(make_pair(sipUri, sipRegisterPtr));
        }

        TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, insert a SipRegisterPtr, sipUri: "
            + sipUri + ",tmp:" + std::to_string(tmpReg) + ", now the total is :" + std::to_string((int) m_mapSipRegisters.size()) + ",m_moduleName:" + m_moduleName);
        return true;
    }


    bool SipRegisterSubscribeManager::removeSipRegisterSubscribe(const std::string & sipUri, bool tmpReg, const std::string & callPath)
    {
        if (!tmpReg)
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.find(sipUri);
            if (iter == m_mapSipRegisters.end())
            {
                return false;
            }
            m_mapSipRegisters.erase(iter);
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.find(sipUri);
            if (iter == m_mapTmpSipRegisters.end())
            {
                return false;
            }
            m_mapTmpSipRegisters.erase(iter);
        }

        TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, removed a SipRegisterPtr, sipUri: "
            + sipUri + ",tmp:" + std::to_string(tmpReg) +  " ,callPath:" + callPath + ",m_moduleName:" + m_moduleName +  ", now the total is :" + std::to_string((int) m_mapSipRegisters.size()));
        return true;
    }

    bool SipRegisterSubscribeManager::removeSipRegisterSubscribe2(const set<std::string> & sipUris, bool tmpReg, const std::string & callPath)
    {
        int expNum = 0;
        if (!tmpReg)
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            set<std::string>::const_iterator itUri = sipUris.begin();
            for (; itUri != sipUris.end(); itUri++)
            {
                const std::string & sipUri = *itUri;
                map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.find(sipUri);
                if (iter == m_mapSipRegisters.end())
                {
                    expNum++;
                    continue;
                }
                m_mapSipRegisters.erase(iter);
                TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, removed a SipRegisterPtr, sipUri: "
                    + sipUri + ",tmp:" + std::to_string(tmpReg) +  " ,callPath:" + callPath + ",m_moduleName:" + m_moduleName);
            }
        }
        else
        {
            int expNum = 0;
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            set<std::string>::const_iterator itUri = sipUris.begin();
            for (; itUri != sipUris.end(); itUri++)
            {
                const std::string & sipUri = *itUri;
                map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.find(sipUri);
                if (iter == m_mapTmpSipRegisters.end())
                {
                    expNum++;
                    continue;
                }
                m_mapTmpSipRegisters.erase(iter);
                TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, removed a SipRegisterPtr, sipUri: "
                    + sipUri + ",tmp:" + std::to_string(tmpReg) +  " ,callPath:" + callPath + ",m_moduleName:" + m_moduleName);
            }
        }

        if (expNum > 0)
            TLOGWARN("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, exception sipUri size: " + std::to_string(expNum)
            + ",total:" + std::to_string(int(sipUris.size()))  + ",tmp:" + std::to_string(tmpReg) +  " ,callPath:" + callPath + ",m_moduleName:" + m_moduleName);

        return true;
    }

    SipRegisterSubscribePtr SipRegisterSubscribeManager::getSipRegisterSubscribe(const std::string & sipUri, bool tmpReg)
    {
        if (!tmpReg)
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.find(sipUri);
            if (iter == m_mapSipRegisters.end())
            {
                return 0;
            }
            return iter->second;
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.find(sipUri);
            if (iter == m_mapTmpSipRegisters.end())
            {
                return 0;
            }
            return iter->second;
        }
    }

    SipRegisterSubscribePtr SipRegisterSubscribeManager::getSipRegisterSubscribe2(const std::string & sipUri, const std::string & callIdNum)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.find(sipUri);
            if (iter != m_mapSipRegisters.end())
            {
                if (iter->second->m_sipMsgDecodePart._callIdPart._strNumber == callIdNum)
                    return iter->second;
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.find(sipUri);
            if (iter != m_mapTmpSipRegisters.end())
            {
                if (iter->second->m_sipMsgDecodePart._callIdPart._strNumber == callIdNum)
                    return iter->second;
            }
        }
        return 0;
    }

    SipRegisterSubscribePtr SipRegisterSubscribeManager::createSipRegisterSubscribe(const std::string & registerId,  const std::string & passWord,
        const std::string & authName, const std::string & registarIp, const std::string & sipProxyId, SipGateway::SipRequestMethodType  methodType, bool tmpReg, int regBffer)
    {
        std::string sipUri = SipRegisterSubscribe::generateSipUri(registerId, registarIp);
        SipRegisterSubscribePtr ptr = getSipRegisterSubscribe(sipUri,tmpReg);
        if (ptr)
        {
            TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, getSipRegisterSubscribe() return FALSE. SipRegisterPtr, sipUri: " + sipUri + ",m_moduleName:" + m_moduleName);
            return 0;
        }

        int64_t curTime = TNOWMS;
        ptr = std::make_shared<SipRegisterSubscribe>(sipUri, registerId, passWord, authName, sipProxyId, curTime, methodType, regBffer);
        insertSipRegisterSubscribe(sipUri, ptr, tmpReg);
        ptr->setTmpRegFlag(tmpReg);

        TLOGDEBUG("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, insert a SipRegisterPtr, sipUri: " + sipUri + ",m_moduleName:" + m_moduleName + ",tmpReg:" + std::to_string(tmpReg));
        return ptr;
    }

    void SipRegisterSubscribeManager::getRegisterSubscribeMapSize(int & periodSize, int & tmpSize)
    {
        periodSize = m_mapSipRegisters.size();
        tmpSize = m_mapTmpSipRegisters.size();
    }

    bool SipRegisterSubscribeManager::getTimeOutRegisterSubscribe(vector<SipRegisterSubscribePtr> & vectRegiSubs)
    {
        vectRegiSubs.clear();
        std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
        if (m_mapSipRegisters.size() == 0)
            return true;

        map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.begin();
        for ( ; iter != m_mapSipRegisters.end(); iter++)
        {
            SipRegisterSubscribePtr ptr = iter->second;
            if (ptr != 0)
            {
                int64_t diffStatusTime = TNOWMS - ptr->getCurStatusInitTime();
                int repeatedTimes = ptr->getRepeatTimes();
                int fixedInterval = 0;

                if (repeatedTimes <= REGISTER_RETRY_INTERVAL_TIMES_5)
                    fixedInterval = REGISTER_RETRY_INTERVAL_LESS_THAN_5;
                else if (REGISTER_RETRY_INTERVAL_TIMES_5 < repeatedTimes && repeatedTimes <= REGISTER_RETRY_INTERVAL_TIMES_10)
                    fixedInterval = REGISTER_RETRY_INTERVAL_LESS_THAN_10;
                else
                    fixedInterval = REGISTER_RETRY_INTERVAL_MORE_THAN_10;

                if (m_sipMethod == SipGateway::kSipRequestMethodREGISTER)
                {
                    switch(ptr->getRegisterStatus())
                    {
                    case SipRegStatusSendRegNoAuth:
                        if (diffStatusTime >= fixedInterval)
                        {
                            ptr->setTimeOutType(RegTimeOutNoResponceNoAuth);
                            vectRegiSubs.push_back(ptr);
                        }
                        break;

                    case SipRegStatusRecvDigest:
                        break;

                    case SipRegStatusSendRegWithAuth:
                        if (diffStatusTime >= fixedInterval)
                        {
                            ptr->setTimeOutType(RegTimeOutNoResponceWithAuth);
                            vectRegiSubs.push_back(ptr);
                        }
                        break;

                    case SipRegStatusRecvFailed:
                        if (diffStatusTime >= fixedInterval)
                        {
                            ptr->setTimeOutType(RegTimeOutRegFailed);
                            vectRegiSubs.push_back(ptr);
                        }
                        break;

                    default:
                        break;
                    }
                }
            }
        }
        return true;
    }

    void SipRegisterSubscribeManager::setMethodType(SipGateway::SipRequestMethodType methodType, const std::string & moduleName)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sipMethod = methodType;
        m_moduleName = moduleName;
    }

    SipGateway::SipRequestMethodType  SipRegisterSubscribeManager::getMethodType()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_sipMethod ;
    }

    bool SipRegisterSubscribeManager::getToBeExpiredRegistrationsAccounts(vector<SipRegisterSubscribePtr> & regObjList)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
        if (m_mapSipRegisters.size() == 0)
            return true;

        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();
        map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.begin();
        for ( ; iter != m_mapSipRegisters.end(); iter++)
        {
            SipRegisterSubscribePtr ptr = iter->second;
            if (ptr != 0)
            {
                int64_t curTime = TNOWMS;
                int64_t lastRegTime = ptr->getLastRegTime();
                if (lastRegTime == 0)
                    continue;

                if (ptr->getTmpRegFlag())
                    continue;

                int64_t refreshTicks = ptr->getNextRefreshTicks();
                int expires = ptr->getExpire();
                int64_t tagetAreaStart = lastRegTime + (expires / 2) * 1000;
                int64_t tagetAreaEnd = lastRegTime + expires * 1000;

                //if (curTime > tagetAreaStart && curTime < tagetAreaEnd)
                //{
                //    regObjList.push_back(ptr);
                //}

                if (curTicks >= refreshTicks && curTime < tagetAreaEnd)
                    regObjList.push_back(ptr);
            }
        }

        return true;
    }

    bool SipRegisterSubscribeManager::getExpiredRegistrationAccounts(vector<SipRegisterSubscribePtr> & regObjList, vector<SipRegisterSubscribePtr> & refreshTimeoutRegs)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.begin();
            for ( ; iter != m_mapSipRegisters.end(); iter++)
            {
                SipRegisterSubscribePtr ptr = iter->second;
                if (ptr != 0)
                {
                    if (ptr->getRefreshTicks() > 0 && ptr->getSipFreshCall() && (TC_TimeProvider::getInstance()->getNowMs() - ptr->getRefreshTicks()) > 45 * 1000)
                        refreshTimeoutRegs.push_back(ptr);

                    int64_t curTime = TNOWMS;
                    int64_t lastRegTime = ptr->getLastRegTime();
                    if (lastRegTime == 0)
                        continue;

                    int expires = ptr->getExpire();
                    int64_t tagetAreaEnd = lastRegTime + expires * 1000;
                    if (curTime > tagetAreaEnd && expires > 0)
                    {
                        regObjList.push_back(ptr);
                    }
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.begin();
            for ( ; iter != m_mapTmpSipRegisters.end(); iter++)
            {
                SipRegisterSubscribePtr ptr = iter->second;
                if (ptr != 0)
                {
                    if (ptr->getRefreshTicks() > 0 && (TC_TimeProvider::getInstance()->getNowMs() - ptr->getRefreshTicks()) > 45 * 1000)
                        refreshTimeoutRegs.push_back(ptr);

                    int64_t curTime = TNOWMS;
                    int64_t lastRegTime = ptr->getLastRegTime();
                    if (lastRegTime == 0)
                        continue;

                    int expires = ptr->getExpire();
                    int64_t tagetAreaEnd = lastRegTime + expires * 1000;
                    if (curTime > tagetAreaEnd)
                    {
                        regObjList.push_back(ptr);
                    }
                }
            }
        }

        return true;
    }

    void SipRegisterSubscribeManager::updateSipAccountInfo(const vector<VoipApp::SipAccountInfo> & vectSipAccountInfo, bool tcpUsed)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            for (unsigned int i = 0; i < vectSipAccountInfo.size(); i++)
            {
                VoipApp::SipAccountInfo accountInfo = vectSipAccountInfo.at(i);
                std::string sipUri = accountInfo.m_sipUri;

                map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.find(sipUri);
                if (iter == m_mapSipRegisters.end())
                {
                    continue;
                }
                else
                {
                    std::string sipUri = iter->first;
                    iter->second->updateSipInfo(accountInfo);
                    int repeatedTimes = tcpUsed ? (REGISTER_RETRY_INTERVAL_TIMES_5 + 1) : 0;
                    iter->second->forceReRegister(std::string(__FUNCTION__), repeatedTimes);
                    TLOGINFO("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, sipUri: " + sipUri);

                    if (accountInfo.m_iPermitedProxyed == 0)
                    {
                        m_mapSipRegisters.erase(iter);
                    }
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            for (unsigned int i = 0; i < vectSipAccountInfo.size(); i++)
            {
                VoipApp::SipAccountInfo accountInfo = vectSipAccountInfo.at(i);
                std::string sipUri = accountInfo.m_sipUri;

                map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.find(sipUri);
                if (iter == m_mapTmpSipRegisters.end())
                {
                    continue;
                }
                else
                {
                    std::string sipUri = iter->first;
                    iter->second->updateSipInfo(accountInfo);
                    int repeatedTimes = tcpUsed ? (REGISTER_RETRY_INTERVAL_TIMES_5 + 1) : 0;
                    iter->second->forceReRegister(std::string(__FUNCTION__), repeatedTimes);
                    TLOGINFO("SipRegisterSubscribe", std::string(__FUNCTION__) + "() called, tmp REGISTER obj, sipUri: " + sipUri);

                    if (accountInfo.m_iPermitedProxyed == 0)
                    {
                        m_mapTmpSipRegisters.erase(iter);
                    }
                }
            }
        }
    }


    bool SipRegisterSubscribeManager::checkRegSipUriExist(const std::string & sipUri)
    {
        std::lock_guard<std::mutex> lock(m_mutexRegUri);
        map<std::string, std::string>::iterator iterUri = m_mapRegUri.find(sipUri);
        return iterUri == m_mapRegUri.end() ? false : true;
    }

    bool SipRegisterSubscribeManager::insertRegSipUri(const std::string & sipUri)
    {
        std::lock_guard<std::mutex> lock(m_mutexRegUri);
        map<std::string, std::string>::iterator iterUri = m_mapRegUri.find(sipUri);
        if (iterUri == m_mapRegUri.end())
        {
            m_mapRegUri.insert(make_pair(sipUri, sipUri));
            return true;
        }
        return false;
    }

    bool SipRegisterSubscribeManager::eraseRegSipUri(const std::string & sipUri)
    {
        std::lock_guard<std::mutex> lock(m_mutexRegUri);
        map<std::string, std::string>::iterator iterUri = m_mapRegUri.find(sipUri);
        if (iterUri == m_mapRegUri.end())
        {
            return false;
        }
        m_mapRegUri.erase(iterUri);
        return true;
    }

    int SipRegisterSubscribeManager::getObjSize()
    {
        int num = 0;
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            num += (int)m_mapSipRegisters.size();
        }

        {
            std::lock_guard<std::mutex> lck(m_mutexTmpSipRegisters);
            num += (int)m_mapTmpSipRegisters.size();
        }
        return num;
    }

    void SipRegisterSubscribeManager::clearAllObjs()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutexRegUri);
            m_mapRegUri.clear();
        }
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            m_mapSipRegisters.clear();
        }
    }

    void SipRegisterSubscribeManager::forceToReRegister(const std::string & sbcIp, const std::string & path)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
        if (m_mapSipRegisters.size() == 0)
            return;

        map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.begin();
        for ( ; iter != m_mapSipRegisters.end(); iter++)
        {
            SipRegisterSubscribePtr ptr = iter->second;
            if (ptr != 0)
                if (ptr->getCoreNetInfo().m_strSBCIp == sbcIp)
                    ptr->forceReRegister(path, REGISTER_RETRY_INTERVAL_TIMES_5 + 1);
        }
    }

    void SipRegisterSubscribeManager::updateProxyIpsPorts(const std::string & listenIp, const int & listenPort, const std::string & sendIp)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutexSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapSipRegisters.begin();
            for ( ; iter != m_mapSipRegisters.end(); iter++)
            {
                SipRegisterSubscribePtr ptr = iter->second;
                if (ptr != 0)
                    ptr->setProxyIpsAndPorts(listenIp, listenPort, sendIp);
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexTmpSipRegisters);
            map<std::string, SipRegisterSubscribePtr>::iterator iter = m_mapTmpSipRegisters.begin();
            for ( ; iter != m_mapTmpSipRegisters.end(); iter++)
            {
                SipRegisterSubscribePtr ptr = iter->second;
                if (ptr != 0)
                    ptr->setProxyIpsAndPorts(listenIp, listenPort, sendIp);
            }
        }

    }

};


