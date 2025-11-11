#ifndef _SIP_REGISTER_H_
#define _SIP_REGISTER_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

#include "SipGateway/src/SipMsgDecodedPart.h"
#include "SipGateway/src/SipMsgCommon.h"
#include "SipGateway/src/SipGatewayCommon.h"
#include "SipProxy/src/SipProxyCommon.h"
#include "SipProxyDb/src/SipDb.h"


namespace SipProxy
{
    //Sip 注册状态
    enum SipRegisterStatus
    {
        SipRegStatusRegPaused = -1,                                 //暂停注册
        SipRegStatusSendRegNoAuth = 0,                              //已发生REGISTER 无鉴权
        SipRegStatusRecvDigest,                                     //收到401/401鉴权响应
        SipRegStatusSendRegWithAuth,                                //已发REGISTER，带鉴权信息
        SipRegStatusRecv100,                                        //100 trying
        SipRegStatusRecv200,                                        //200 OK
        SipRegStatusRecvFailed,                                     //其他响应
        SipregStatusMaxDuration                                     //生存期超时
    };

    //Sip注册超时 类型
    enum RegTimeOutType
    {
        RegTimeOutNone,                                             //无超时
        RegTimeOutNoResponceNoAuth,                                 //无响应，需重发注册
        RegTimeOutNoResponceWithAuth,                               //无响应，需重发注册（携带鉴权）
        RegTimeOut100Trying,                                        //100 Trying超时
        RegTimeOutRegFailed,
        //RegTimeOutMaxDuration                                     //最大生命期超时
    };

    //核心网对接SBC相关信息
    struct CoreNetInfo
    {
        CoreNetInfo(){}

        CoreNetInfo(const std::string & coreNetId, const std::string & sbcIp, int sbcPort, const std::string & registarHost, int registarPort)
        {
            m_strCoreNetId = coreNetId;
            m_strSBCIp = sbcIp;
            m_port = sbcPort;
            m_strRegistarIp = registarHost;
            m_iRegistarPort = registarPort;
        }

        std::string              m_strCoreNetId;
        std::string              m_strSBCIp;
        int                         m_port;
        std::string              m_strRegistarIp;
        int                         m_iRegistarPort;
    };

    class SipRegisterSubscribe;
    typedef std::shared_ptr<SipRegisterSubscribe> SipRegisterSubscribePtr;

    class SipRegisterSubscribe
    {

        //construction
    public:
        SipRegisterSubscribe();
        SipRegisterSubscribe(const std::string & sipUri, const std::string & registerId, const std::string & passWord, const std::string & authName, const std::string & sipProxyId, SipGateway::SipRequestMethodType methodType, int regBffer);
        SipRegisterSubscribe(const std::string & sipUri,const std::string & registerId, const std::string & passWord, const std::string & authName, const std::string & sipProxyId, int64_t httpRegisterTime, SipGateway::SipRequestMethodType methodType, int regBffer);
        static SipRegisterSubscribePtr create(const std::string & sipUri, const std::string & registerId, const std::string & passWord,
            const std::string & authName, const std::string & sipProxyId, SipGateway::SipRequestMethodType methodType, int regBffer);

        virtual ~SipRegisterSubscribe();

        //date members
    private:
        std::string              m_sipUri;                       //sipUri of a sip account, format: 15802586775@ebupt.com
        std::string              m_strId;                        //object id
        std::string              m_strSipRegisterId;             //注册的Sip账号
        std::string              m_strPassWord;                  //密码
        std::string              m_strAuthName;                  //鉴权名称

        std::string              m_strSipProxyId;
        int64_t                m_lHttpRegisterTime;            //the time that SipGateway transfered the http register request
        int64_t                m_lLastRegsiterTime;            //the last time that receive the 200 REGISTER OK from the SBC
        int                         m_iExpire;
        int64_t                m_lLastFailedTime;

        std::string              m_strRegistarIp;                //the Registrar ip
        int                         m_iRegistrarPort;
        std::string              m_strClientIp;
        int                         m_iClientPort;
        std::string              m_strSendIp;

        ST_SIP_DIGEST               m_stDigest;                     //401 Digest
        bool                        m_bQopExist;                    //401 qop exist flag
        unsigned char               m_ucQopType;                    //401 qop        //reference EN_SIP_QOP_VAL  0-EN_SIP_QOP_VAL_AUTH, 1-EN_SIP_QOP_VAL_AUTH_INT, 2-EN_SIP_QOP_VAL_OTHER
        std::string              m_strRealm;                     //realm
        std::string              m_strNonce;                     //401 nonce
        int                         m_iNc;                          //401 nc
        std::string              m_opaque;                       //401 opaque
        std::string              m_strDigestUserName;            //user name
        unsigned char               m_algo;                         //algorithm-value       reference EN_SIP_ALGO_VAL 0-EN_SIP_ALGO_VAL_MD5, 1- EN_SIP_ALGO_VAL_MD5_SESS,  2-EN_SIP_ALGO_VAL_MD5_OTHERS
        std::string              m_strResponse;                  //response
        std::string              m_strCNonce;                    //CNonce

        bool                        m_bClientInitReg;               //用以表示该注册是否由客户端直接发起注册

        int64_t                m_lCreateTime;                  //创建时间点
        SipRegisterStatus           m_eRegStatus;                   //注册状态
        int64_t                m_lCurStatusInitTime;           //进入当前状态时间点
        int                         m_iRepeatTimes;                 //重发次数
        RegTimeOutType              m_eTimeOutType;                 //超时类型

        CoreNetInfo                 m_coreNetInfo;
        bool                        m_bUpdatingProxyedPro;            //为防止SipProxy 更新 账号表Proxyed属性，SipProxyDb可能超过2秒才返回，此时在异步回调返回之前，SipProxy读取Sip账号表更新内存时发现未注册，则会更新注册对象的注册状态； 造成错误
        int                         m_iRandomSeq;
        bool                        m_bTmpReg;

        int64_t                m_refreshTicks;
        int64_t                m_nextSipRefreshTicks;
        std::shared_ptr<void>       m_freshCall;
        std::string              m_domainId;
        int                         m_regTmBuffer;

        std::mutex              m_mutex;
    public:
        SipMsgDecodedPart                       m_sipMsgDecodePart;
        SipGateway::SipRequestMethodType        m_sipMethod;

        //function member

    public:
        std::string getId();

        void setExpire(int expire);
        int getExpire();

        void setHttpReqTime(int64_t reqTime);
        void setLastRegisterTime(int64_t lastTime);

        void setIpsAndPorts(const std::string & clientIp, const int & clientPort, const std::string & registrarIp, const int & iRegistrarPort, const std::string & sendIp = "");
        void getIpsAndPorts(std::string & clientIp, int & clientPort, std::string & registrarIp, int & registrarPort,std::string & sendIp);
        void setProxyIpsAndPorts(const std::string & listenIp, const int & listenPort, const std::string & sendIp = "");

        std::string getClientIp();

        std::string getSipUri();
        void setSipUri(const std::string & sipUri);

        static std::string generateSipUri(const std::string & sipUsername, const std::string & registarIp);

        std::string getRegisterId();
        void setRegisterId(const std::string & regId);

        std::string getPassWord();
        void setPassWord(const std::string & passWord);

        void setDigest(const ST_SIP_DIGEST & stDigest);
        ST_SIP_DIGEST getDigest();

        void setDigestQopType(bool qopExist, unsigned char qopType);
        unsigned char getDigestQopType();
        bool getDigestQopExist();

        void setDigestRealm(const std::string & realm);
        std::string  getDigestRealm();

        void setDigestNonce(const std::string & nonce);
        std::string getdigestNonce();
        void setDigestNonceCount(int nc);
        int getDigestNonceCount();
        void increaseNonceCount();

        std::string getAuthName();
        void setAuthName(const std::string & authName);

        std::string getDigestUserName();
        void setDigestUserName(const std::string & userName);

        std::string getDigestOpaque();
        void setDigestOpaque(const std::string & opaque);

        unsigned char getDigestAlgo();
        void setDigestAlgo(unsigned char algo);

        std::string getDigestResponse();
        void setDigestResponse(const std::string & response);

        std::string getDigestCNonce();
        void setDigestCNonce(const std::string & cnonce);

        std::string getSipProxyId();
        void setSipProxyId(const std::string & proxyId);

        int64_t getLastHttpReqTime();
        int64_t getLastRegTime();

        int64_t getCreateTime();

        void setRegisterStuatus(SipRegisterStatus regStatus, const std::string & path);
        SipRegisterStatus getRegisterStatus();
        int64_t getCurStatusInitTime();

        void repeatTimeAutoIncrease();
        int getRepeatTimes();

        void setTimeOutType(RegTimeOutType typeOutType);
        RegTimeOutType getTimeOutType();

        void setRespCodeType(SipGateway::SipResponseCodeType respCode);
        SipGateway::SipResponseCodeType getRespCodeType();

        //converter
        bool sipRegisterSubToDbStruc(VoipApp::SipRegInfoStruc & regStruc);
        bool dbStrucToSipRegisterSub(const VoipApp::SipRegInfoStruc & regStruc);

        std::string getUseModulName(SipGateway::SipRequestMethodType methodType);
        void setCoreNetInfo(const std::string & coreNetId, const std::string & sbcIp, int sbcPort, const std::string & registarIp, int registarPort);
        CoreNetInfo getCoreNetInfo();

        void setUpdatingProxyedProperty(bool bUpdatingProxyed);
        bool getUpdatingProxyedProperty();
        void updateSipInfo(const VoipApp::SipAccountInfo & sipAccountInfo);

        void setTmpRegFlag(bool tmpReg);
        bool getTmpRegFlag();

        void setSipFreshCall(const std::shared_ptr<void> & freshCall);
        std::shared_ptr<void> getSipFreshCall();
        int64_t getRefreshTicks();

        void refreshBasicRegInfo();
        void forceReRegister(const std::string & path, int repeatedTimes = 0);

        void setDomainId(const std::string & domainId);
        std::string getDomainId();

        int64_t getNextRefreshTicks();

        //Test
        static std::atomic<int>      m_totalRegNum;
        static std::atomic<int>      m_totalNewRegNum;
        static std::atomic<int>      m_totalDeleteRegNum;

        static std::atomic<int>      m_totalSubNum;
        static std::atomic<int>      m_totalNewSubNum;
        static std::atomic<int>      m_totalDeleteSubNum;

        static std::atomic<int>      m_totalNotNum;
        static std::atomic<int>      m_totalNewNotNum;
        static std::atomic<int>      m_totalDeleteNotNum;
    };


    class SipRegisterSubscribeManager
    {

    private:
        std::map<std::string, SipRegisterSubscribePtr>                m_mapSipRegisters;      //key-sipUri,format:15802586775@ebupt.com,
        std::mutex                                            m_mutexSipRegisters;

        std::map<std::string, SipRegisterSubscribePtr>                m_mapTmpSipRegisters;
        std::mutex                                            m_mutexTmpSipRegisters;

        SipGateway::SipRequestMethodType                            m_sipMethod;            //REGISTER, SUBSCRIBE, NOTIFY object
        std::string                                              m_moduleName;
        std::map<std::string, std::string>                         m_mapRegUri;
        std::mutex                                            m_mutexRegUri;

        std::mutex                                            m_mutex;

    public:
        bool insertSipRegisterSubscribe(const std::string & sipUri, const SipRegisterSubscribePtr & sipRegisterPtr, bool tmpReg);
        bool removeSipRegisterSubscribe(const std::string & sipUri, bool tmpReg, const std::string & callPath);
        bool removeSipRegisterSubscribe2(const std::set<std::string> & sipUris, bool tmpReg, const std::string & callPath);

        SipRegisterSubscribePtr getSipRegisterSubscribe(const std::string & sipUri, bool tmpReg);
        SipRegisterSubscribePtr getSipRegisterSubscribe2(const std::string & sipUri, const std::string & callIdNum);

        SipRegisterSubscribePtr createSipRegisterSubscribe(const std::string & registerId, const std::string & passWord, const std::string & authName,
            const std::string & registarIp, const std::string & sipProxyId, SipGateway::SipRequestMethodType methodType, bool tmpReg, int regBffer);

        void getRegisterSubscribeMapSize(int & periodSize, int & tmpSize);
        bool getTimeOutRegisterSubscribe(std::vector<SipRegisterSubscribePtr> & vectRegiSubs);

        void setMethodType(SipGateway::SipRequestMethodType methodType, const std::string & moduleName);
        SipGateway::SipRequestMethodType getMethodType();

        bool getToBeExpiredRegistrationsAccounts(std::vector<SipRegisterSubscribePtr> & regObjList);
        bool getExpiredRegistrationAccounts(std::vector<SipRegisterSubscribePtr> & regObjList, std::vector<SipRegisterSubscribePtr> & refreshTimeoutRegs);
        void updateSipAccountInfo(const vector<VoipApp::SipAccountInfo> & vectSipAccountInfo, bool tcpUsed);

        bool checkRegSipUriExist(const std::string & sipUri);
        bool insertRegSipUri(const std::string & sipUri);
        bool eraseRegSipUri(const std::string & sipUri);
        int getObjSize();
        void clearAllObjs();
        void forceToReRegister(const std::string & sbcIp, const std::string & path);
        void updateProxyIpsPorts(const std::string & listenIp, const int & listenPort, const std::string & sendIp = "");
    };


    struct TmpAccount
    {
        std::string _sipAccount;
        std::string _passwd;
        std::string _appAccount;
        std::string _authName;

        TmpAccount(const std::string & account, const std::string & passwd,const std::string & appAccount, const std::string & authName)
        {
            _sipAccount = account;
            _passwd = passwd;
            _appAccount = appAccount;
            _authName = authName;
        }
    };

    struct RegisterItem
    {
        RegisterItem()
        {
            m_iPort = 0;
            m_iLogined = 0;
            m_iSBCport = 0;
            m_iRegistarPort = 0;
            m_iFailedRegTimes = 0;
            m_lLastFailedRegTime = 0;
            m_lLoginedTime = 0;
            m_proxyedTime = 0;
        }

        std::string      m_sipUri;       //format: ClientId@registarIp, 13925420232@ebupt.com
        std::string      m_strAccount;
        std::string      m_strDomainId;
        std::string      m_strDomainName;
        std::string      m_strPassWord;
        std::string      m_strAuthName;

        std::string      m_strClientIp;
        int                 m_iPort;
        int                 m_iLogined;
        bool                m_bPermitedToProxy;
        std::string      m_strCoreNetId;

        std::string      m_strSBCIp;
        int                 m_iSBCport;
        std::string      m_strRegistarIp;
        int                 m_iRegistarPort;
        int                 m_iFailedRegTimes;

        int64_t        m_lLastFailedRegTime;
        int64_t        m_lLoginedTime;
        int64_t        m_proxyedTime;
    };

    struct MessageAbstructPolicy
    {
        std::string      m_domainId;
        int                 m_abstrackLen;
        std::string      m_mask;

        MessageAbstructPolicy()
        {}

        MessageAbstructPolicy(const std::string & domainId, int len, std::string mask)
        {
            m_domainId = domainId;
            m_abstrackLen = len;
            m_mask = mask;
        }

        bool operator<(const MessageAbstructPolicy&obj) const
        {
            return m_domainId < obj.m_domainId;
        }

        bool operator==(const MessageAbstructPolicy&obj) const
        {
            return m_domainId == obj.m_domainId;
        }

        bool operator>(const MessageAbstructPolicy&obj) const
        {
            return m_domainId > obj.m_domainId;
        }
    };

};



#endif