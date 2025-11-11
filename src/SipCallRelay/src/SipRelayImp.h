#ifndef _SipRelayImp_H_
#define _SipRelayImp_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <mutex>

#include "servant/Application.h"
#include "SipRelay.h"

#include "SipGateway/src/ConnectionCallIdManager.h"
#include "SipProxy/src/SipGatewayIdRecordManager.h"
#include "SipProxy/src/SipRegisterSubscribe.h"
#include "SipProxyDb/src/SipDb.h"
#include "SipGateway/src/SipMsgCommon.h"
#include "SipGateway/src/SipMsg.h"
#include "SipGateway/src/SipMsgType.h"
#include "Common/Net.h"


struct stSbcConn
{
    int times;
    int64_t lastActive;
    int64_t lastSendTime;
};
/**
 *
 *
 */
class SipRelayImp : public VoipApp::SipRelay,
        virtual public SipGateway::SipMsgSenderInterface,
        virtual public SipProxy::SipProxyMsgSenderInterafece,
        virtual public SipProxy::SipProxyMsgCoderInterface,
        virtual public Common::NetReceiver,
        virtual public tars::TC_ThreadRecMutex
{
public:
    /**
     *
     */
    virtual ~SipRelayImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     * 配置变更通知（Tars框架回调）
     * 当配置文件通过tars web平台更新时会触发
     */
    virtual void onConfigReload();
    /**
     *
     */
    virtual tars::Bool eraseSipDlgIdAndConnection(const std::string & sipGatewayId,const std::string & dialogId,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getIntranetIp(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,std::string &intranetIp,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getMgcfConnectStatus(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,map<std::string, std::string> &params,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool recvSipMsgFromCloudSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;

    //SipProxyMsgCoderInterface
    virtual bool onSipMessageRequest(const SipGateway::SipRequestMethodType reqMethod, const SipProxy::SipRegisterSubscribePtr & ptr, void * pConnection);
    virtual bool onSipMessageResponse(const SipGateway::SipRequestMethodType reqMethod, const SipGateway::SipResponseCodeType & rspCode, const SipProxy::SipRegisterSubscribePtr & ptr, void * pConnection, bool InvalidMsg = false);
    virtual bool onOthersSipMessageTransfer(const std::string &strMsgBuffer,int iBufLen, const std::string & sipDialogId,
        const std::string & callId, bool bInitInvite, void * pConnection, bool exceptionConnection, const std::string & toMgcfConnectionId, bool bReq, unsigned char method,  const std::map<std::string, std::string> & others);

    virtual bool onOthersSipMessageTransferEx(const std::string & sipMsg, int iBufLen, const std::string & callIdHost, bool bRequest, unsigned char method, unsigned int resCode,
        const std::string & fromUsername, const std::string & toUserName, const std::string & registarIpName, void * pConnection, const std::string & toMgcfConectionId);

    virtual bool saveExceptionConnection(void * pConnection);
    virtual SipProxy::SipRegisterSubscribePtr getSipRegisterSubscribe(const std::string & sipUri, const std::string & callIdNum, SipGateway::SipRequestMethodType methodType);

    virtual std::string getSipProxyId();
    virtual bool getConnectionIdToSBC(const std::string & sipAccount, const std::string & registerHost, std::string & toMgcfConntionId);
    virtual bool checkCallIdHostExist(const std::string & callIdHost, SipProxy::SipDialogRecord & sipDialogRecord);
    virtual bool transferPingPongMsg(const std::string &strMsgBuffer,int iBufLen, bool isPing, void * pConnection);
    virtual bool toSBCConnectionClosed(const std::string & id, const std::string & localHost, int localPort, const std::string & remoteHost, int port, const std::string & transProtocol);

    //virtual bool onSipMessageRequest2(const SipMessageGateway::SipMessagePtr & ptr, void * pConnection);
    //virtual bool onSipMessageResponse2(const std::string & msg, const SipGateway::SipRequestMethodType reqMethod, unsigned int rspCode,  const SipMessageGateway::SipMessagePtr & ptr, void * pConnection, bool InvalidMsg = false);
    //virtual SipMessageGateway::SipMessagePtr getSipMessage(const std::string & dstUri, const std::string & callIdHost);
    virtual bool checkIpInWhitelist(const std::string & remoteIp);

    //AppEventProcessor
    //virtual void onProcEvent(const Common::ObjectPtr& event);

    //SipProxyMsgSenderInterface
    virtual bool sendEncodedMessage(const std::string & strMsg, int iSize, const std::string & connectionId,  bool req, std::string & reason);
    virtual bool sendEncodedMessage2(const std::string & strMsg, int iSize, void * pConnection, std::string & reason);

    //SipGateway::SipMsgSenderInterface
    virtual bool sendEncodedMessage(const std::string & strMsg, int iSize, bool bRequest, SipGateway::SipRequestMethodType reqMethod, const SipGateway::SipDialogPtr & sipDlg);

    virtual Common::NetReceiverPtr recvConnection(const Common::NetSenderPtr& sender);
    virtual void onConnReady();
    virtual void onConnClose();

    //virtual void cmdResult(int rslt,const Common::IputStreamPtr& input,const Common::ObjectPtr& userdata);

public:
    bool insertGatewayConnection(const std::string & strConnectionId,  const SipGateway::GatewayConnectionPtr & pConnection, const std::string & regOrSipDialogId);
    //bool removeGatewayConnection(const SipMessageGateway::SipMessagePtr & ptr);

    bool removeConnection(const std::string & sipdialogId, const std::string & strConnectionId);

    bool getMasterTrdDown();
    bool getOutOfService();

    //void changeTransProtocol();

    bool buildConnectionToMgcfs(bool transProtocalChanged = false);
    bool buildConnectionToMgcf(const std::string & strConnecntionId, bool transProtocalChanged, const std::string& protocol,const std::string& localHost,int localPort,const std::string& mgcfIp, int mgcfPort);

    bool sendSipMsgToCloudSipGateway(const std::string & sipGatewayId, const std::string & connectionId, const std::string & callId,  bool bInitInvite,
        const std::string & sipMsg, const std::string & toMgcfConnectionId, const std::string & sipDialogId, const std::map<std::string, std::string> & params);

    SipGateway::GatewayConnectionPtr connectMgcfEx(const std::string& protocol,const std::string& localHost,int localPort,const std::string& mgcfIp,int mgcfPort,
        const std::string & strConnecntionId, int & useSendPort, std::string & reason);
    SipGateway::GatewayConnectionPtr connectMgcf(const std::string& protocol,const std::string& localHost,int localPort,const std::string& mgcfIp,int mgcfPort,
        const std::string & strConnecntionId, int & usedLocalPort, std::string & reason);

    bool newAConnectionToMgcf(const std::string & localHost,const std::string & mgcfIp, const int & mgcfPort, const std::string & msg, bool bLocalPort, std::string & reason);

    void readSipGatewayAccessedConfigInfo();
    bool onReadSipGatewayAccessedConfigInfo(const std::vector<VoipApp::SipProxyAccessedCoreNetConfig> & vectSipProxyConfigInfo);

    bool insertSipDialogRecord(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId);
    bool insertSipDialogRecord2(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId,void * respCon, bool appCallCs);
    bool ereaseSipDialogRecord(const std::string & callId);
    bool obtainSipDialogRecord(const std::string & callId, SipProxy::SipDialogRecord & sipDialogRecord);
    bool updateSipDialogRecordGwId(const std::string & callId, const std::string & sipGatewayObjId);
    bool ereaseSipDialogRecordAndConnection(const std::string & sipDialogId);
    bool ereaseSipDialogRecordBySipDialogID(const std::string & sipDialogId);

    void getAllSipGatewayCfgs(std::vector<VoipApp::SipProxyAccessedCoreNetConfig> & cfgs);
    int getMgcfSipGatewayCfgsSize();
    bool getSipGatewayIdByRouterIp(const std::string &routerIp, std::string &sipGatewayId, std::string &sipGatewayIp, int &sipGatewayPort, std::string & cloudIp, int & cloudPort);
    bool getCallerAndCalleeSipUri(std::string & caller, std::string & callee, const std::string & sipMsg);

    void registerMgcfSipGatewayToResource();
    bool loadMgcfSipGatewayResources();
    void onResultLoadSipGwResource2(const std::vector<tars::EndpointF> & vectResources);
    bool loadSipGw3Resources();
    bool onResultLoadSipGwResource(const std::vector<tars::EndpointF> & vectResources);

    bool loadEnumproxyResources();
    bool onResultLoadEnumproxyResources(const std::vector<tars::EndpointF> & vectResources);

    void getRouterInfoFromJego(const std::string & callId, const std::string & strConnectionId, const std::string & toMgcfConnectionId,
        const std::string & msg, const std::string & sipDialogId, void * pConnection,  const std::map<std::string, std::string> & params);
    bool getSipUri(bool isCaller, std::string & sipUri, const std::string & sipMsg, std::string & enterIp);
    bool getCallForwarding(std::string & sipUri, const std::string & sipMsg);
    bool getDiversionInfo(const std::string & diversion, std::string & sipuri);

    void setLogsAndStatistics();
    bool getOptionStatus(const std::string & connId);
    void sendOptionToMgcf();
    void getCloudSipGwIdAndIp();
    void checkServerAlive();
    void decodeMgcfIp(const std::string & cfg, std::set<std::string> & mgcfIps);
    bool sipUriIsAnonymous(const std::string & sipUri);
    void getCloudSipGatewayId(const VoipApp::JegoRouterInfo& info, std::string& sipGatewayId);
    bool getCloudSipGatewayIdByIp(const std::string& ip, std::string& sipGatewayId);
    std::string getDcByIp(const std::string& ip);

    // 线程函数
    void AccessConfigReadThreadFunc();
    void ChangeTransProtocolThreadFunc();

    bool reloadConfig();
private:
    //const Common::ApplicationExPtr          m_application;
    std::string                             m_strAppName;
    Common::NetSenderPtr                    m_Listener;
    Common::NetDriverPtr                    m_listenNetDriver;
    std::mutex                              m_mutexListen;

    std::string                             m_strMainObjId;
    std::string                             m_serviceFullName;
    std::string                             m_strDataCenter;
    bool                                    m_bMasterThdDown;
    bool                                    m_bOutOfService;
    bool                                    m_bServiceActivated;

    bool                                    m_bTcpProUsed;
    SipGateway::TransProtocolType           m_transProType;
    std::string                             m_tranProtoclName;

    std::string                             m_strLocalHost;
    std::string                             m_strLocalIp;
    int                                     m_iLocalListenPort;
    std::string                             m_strSendIp;
    int                                     m_iLocalSendPort;
    int                                     m_iCurSendPort;
    std::string                             m_destReqIpPort;
    std::string                             m_destRespIpPort;
    std::string                             m_strMgcfIp;
    std::string                             m_strMgcfIp1;
    std::string                             m_strMgcfIp2;

    std::string                             m_strEntIp;

    std::mutex                              m_mutexTransProChange;
    SipGateway::TransProtocolChangeType     m_transProChangeType;

    SipProxy::SipDialogRecordManager        m_sipDialogRecordManager;
    SipGateway::ConnectionCallIdManager     m_connectionCallIdManager;
    SipGateway::MsgRemainManager            m_remianMsgs;
    std::string                             m_strToMgcfSipDialogId;

    int64_t                                 m_LastSchdTimeInMs;
    int64_t                                 m_BaseSchdTimeInMs;
    int64_t                                 m_lastHour;
    int64_t                                 m_lastMinute;

    int                                     m_iSendPortChangeType;

    std::set<std::string>                   m_setMgcfIps;
    std::mutex                              m_mutexMgcfIps;
    std::set<std::string>                   m_setVirtualMgcfIps;
    std::mutex                              m_mutexVirtualMgcfIps;

    VoipApp::SipDbPrx                       m_sipProxyDbAgent;
    std::vector<VoipApp::SipProxyAccessedCoreNetConfig>  m_vectSipGatewayAccessedConfig;
    std::mutex                              m_mutexVectSipGateway;
    //VoipApp::ResourceServerPrx              m_resourceAgent;

    std::vector<std::string>                m_vectSipGwIds;
    std::mutex                              m_mutexSipGwIds;

    std::mutex                              m_mutexMgcfStatus;
    std::map<std::string, stSbcConn>        m_mapMgcfStatus;

    std::mutex                              m_mutexEnumproxyIds;
    std::vector<std::string>                m_vectEnumproxyIds;

    std::mutex                              m_mutexSipGwIdAndIps;
    std::map<std::string, std::string>      m_mapSipGwIdAndIps;

    std::mutex                              m_mutexMgcfSipGatewayIds;
    std::map<std::string, std::string>      m_mapMgcfSipGatewayIds;

    std::string                             m_strEnterprisePrefix;
    std::mutex                              m_mutexEnterprisePrefix;
    std::map<std::string, std::vector<VoipApp::enterPrefix>>      m_mapEnterprisePrefix;
};
/////////////////////////////////////////////////////
#endif
