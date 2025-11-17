#ifndef _SipRtpImp_H_
#define _SipRtpImp_H_

#include "servant/Application.h"
#include "SipRtp.h"
#include <atomic>

#include "SipGateway/src/SipGatewayConnection.h"
#include "SipGateway/src/SipMsg.h"
#include "RtpGatewayCommon.h"
#include "RtpGatewayInterfaceI.h"
#include "MediaStreamReceivingPortsManager.h"
#include "util/tc_thread_pool.h"

#include "watermelon/common_types.h"
#include "watermelon/voe_network.h"
#include "watermelon/common_types.h"
#include "watermelon/voe_errors.h"
#include "watermelon/voe_base.h"
#include "watermelon/voe_codec.h"
#include "watermelon/voe_volume_control.h"
#include "watermelon/voe_dtmf.h"
#include "watermelon/voe_rtp_rtcp.h"
#include "watermelon/voe_audio_processing.h"
#include "watermelon/voe_file.h"
#include "watermelon/voe_video_sync.h"
#include "watermelon/voe_encryption.h"
#include "watermelon/voe_hardware.h"
#include "watermelon/voe_external_media.h"
#include "watermelon/voe_network.h"
#include "watermelon/voe_neteq_stats.h"

class RtpSender;
class Rtp2RtpChannel;
class Rtp2RtpRelayer;
class JssmmeManager;
class JssmmeChannel;
class SipRtpImp;

typedef std::shared_ptr<RtpSender> RtpSenderPtr;
typedef std::shared_ptr<Rtp2RtpChannel> Rtp2RtpChannelPtr;
typedef std::shared_ptr<Rtp2RtpRelayer> Rtp2RtpRelayerPtr;
typedef std::shared_ptr<JssmmeManager> JssmmeManagerPtr;
typedef std::shared_ptr<JssmmeChannel> JssmmeChannelPtr;
typedef std::shared_ptr<SipRtpImp> SipRtpImpPtr;

//一个结构体对象对应一个SDP中的媒体流
struct SDPMediaReceiveIpAndPort
{
    std::string         m_strIp;
    int                 m_iPort;
    bool                m_bIsAudio;
    bool                m_bForbitedStream;      //如果端口为0，则为禁止接收媒体流
    int                 m_localPort;
};

enum StreamType
{
    AudioType = 0,
    VideoType = 1
};

enum SessionMediaType
{
    kSessionMediaTypeUnset = -1,
    kSessionMediaTypeAudio = 0,
    kSessionMediaTypeVideo = 1
};

enum SessionUpdatedType
{
    kSessionMediaNoChange = -1,
    kSessionAudioPortChange = 0,
    kSessionVideoPortChange = 1,
    kSessionVideoOpened = 2,
    kSessionVideoClosed = 3,
    kSessionMediaIpChange = 4
};

enum JssmmeCallBackType
{
    kSessionCallbackInvalid = -1,
    kSessionCallback = 0,
    kContentCallback,
    kArcCallback
};

enum JssmmeMediaNegoStatus
{
    kMediaNegoInvalid = -1,
    kMediaNegoFinished
};

struct SessionUpdateEvent
{
    SessionUpdatedType      m_updateType;

    SessionMediaType        m_oldMediaType;
    SessionMediaType        m_newMediaType;

    int                     m_oldLocalAudioPort;
    int                     m_oldLocalVideoPort;
    std::vector<int>             m_oldLocalPorts;

    int                     m_oldSBCAudioPort;
    int                     m_oldSBCVideoPort;
    std::vector<int>             m_oldSBCPorts;

    int                     m_newSBCAudioPort;
    int                     m_newSBCVideoPort;

    std::string             m_oldSBCIp;
    std::string             m_newSBCIp;
};

class RtpSender : virtual public SipRtpGateway::RtpGatewayMsgSenderInterface
{
public:
    RtpSender(const SipRtpImpPtr & server, const std::string & listenIp, const std::string & callId, JssmmeManagerPtr jssmmeManager = 0);
        virtual ~RtpSender();

    //function members
public:
    // 提供虚函数让派生类提供 shared_ptr，避免菱形继承问题
    virtual SipRtpGateway::RtpGatewayMsgSenderInterfacePtr getSharedPtr() = 0;
    SipGateway::GatewayConnectionPtr  newAGatewayConnectionEx(int localPort, const std::string & strIp, int remotePort, SipGateway::TransType transType);

    bool insertAConnection(const SipGateway::GatewayConnectionPtr & ptr);
    bool removeAllConnections();
    bool insertExceptionConnection(const SipGateway::GatewayConnectionPtr & ptr);
    bool removeAllExcepConnections();

    //void setApplication(const tars::ApplicationPtr & _application ,const std::string & listenIp);
    bool generateMediaStreamConnectionToSBCEx(const std::vector<SDPMediaInfo> & vectSdpMediaInfo);
    bool refreshMediaSteamConnectionToSBC(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, const std::string& strCsSDP, std::string& strModifiedAppSDP);
    bool getSessionMediaChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent);
    bool getMediaIpAndType(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, std::string & sbcIp, bool & bVideo);

    bool generateSBCIpChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent);
    bool generateAudioPortChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent);
    bool generateVideoPortChangeEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent);
    bool generateVedioOpenedEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent);
    bool generateVedioClosedEvent(const std::vector<SDPMediaInfo> & vectSdpMediaInfo, SessionUpdateEvent & updateEvent);

    bool procAudioPortChangeEvent(SessionUpdateEvent updateEvent);
    bool procVideoPortChangeEvent(SessionUpdateEvent updateEvent);
    bool procVedioOpenedEvent(SessionUpdateEvent updateEvent);
    bool procVedioClosedEvent(SessionUpdateEvent updateEvent);
    bool procSBCIpChangeEvent(SessionUpdateEvent updateEvent);

    bool buildAudioConnectionToSBC(int localPort, const std::string & remoteIp, int remotePort, std::string & reason);
    bool buildVideoConnectionToSBC(int localPort, const std::string & remoteIp, int remotePort, std::string & reason);

    bool getReceiveMediaIpAndPorts(const std::string & strRpcSDPConfig,std::string & ip, std::vector<int> & vectPorts);
    bool sendRtpDataToSBC(const unsigned char* pBuffer, int size, unsigned char strmType);

    bool checkValidRtpOrRtcp(unsigned char ch);
    bool checkRtpOrRtcpEx(unsigned char ch, bool & bRtp);
    void setCallId(const std::string & strCallId);
    void addLocalRecvPorts(std::vector<int> evenPorts);
    void setSipGatewayOid(const std::string & oid);
    void conClose();
    // bool setSDPAnswer(const std::string& strCsSDP,const std::string& strAppSDP, const map<std::string, std::string>& params,std::string& strModifiedAppSDP,
    //     std::map<std::string, std::string>& retValus, JssmmeChannelPtr & pChannel, std::string & errReason);
    int64_t getCreateTicks();
    void setCaller(int isCaller);
    bool refreshMediaSteamConnection(const std::string& sdp);
    bool neoAppSdp(const std::string& sdp,std::string& newSdp, JssmmeChannelPtr & pChannel);

    //date members
public:

    std::vector<SDPMediaReceiveIpAndPort>             m_vectMedioIpAndPorts;
    SipGateway::GatewayConnectionPtr             m_audio_RTP_ConnectionPtr;
    SipGateway::GatewayConnectionPtr             m_audio_RTCP_ConnectionPtr;

    SipGateway::GatewayConnectionPtr             m_video_RTP_ConnectionPtr;
    SipGateway::GatewayConnectionPtr             m_video_RTCP_ConnectionPtr;
    tars::TC_ThreadRecMutex                     m_mutexConnections;

    std::map<std::string, SipGateway::GatewayConnectionPtr>   m_mapConnections;
    tars::TC_ThreadRecMutex                             m_mutexConnetionMap;

    std::map<std::string, SipGateway::GatewayConnectionPtr> m_mapExcepConnetions;
    tars::TC_ThreadRecMutex                            m_mutexExcepConnection;

    //tars::ApplicationPtr                         m_application;
    std::string                                 m_strListenIp;
    std::vector<int>                                 m_vectLocalRecMediaPorts;
    std::vector<int>                                 m_vectRemoteRecMediaPorts;
    std::string                                 m_strCallId;

    std::vector<int>                                 m_vectLocalRecMrfpMediaPorts;
    std::string                                 m_sbcIp;
    SessionMediaType                            m_sessionMediaType;

    bool                                        m_bInitConnected;
    std::string                                 m_listenIp;

    int                                         m_audioToApp;
    int                                         m_audioToSBC;
    int                                         m_sendToSBCPackets;
    int                                         m_sendToSBCFailedPackets;
    int                                         m_sendToAppPackets;
    int                                         m_sendToAppFailedPackets;

    int64_t                                  m_startTimeToApp;
    int64_t                                  m_lastTimeToApp;
    int64_t                                  m_startTimeToSbc;
    int64_t                                  m_lastTimeToSbc;
    int64_t                                  m_appLastTimeRecvMedia;
    int64_t                                  m_sipLastTimeRecvMedia;

    std::string                                 m_sipGatewayOid;
    bool                                        m_csCallApp;
    SipRtpImpPtr                              m_pServer;
    int64_t                                  m_createTicks;
    int                                         m_isCaller;
    JssmmeManagerPtr                            m_jssmmeManager;

    tars::TC_ThreadRecMutex                 m_mutex;
};


class Rtp2RtpChannel : virtual public Common::NetReceiver,
                        virtual public RtpSender,
                        virtual public jssmme::Transport,
                        public std::enable_shared_from_this<Rtp2RtpChannel>
{
public:
    Rtp2RtpChannel(const SipRtpImpPtr& server, const std::string & callId, int isCaller, const std::string & sdp, bool qosPres, int interval, bool emptyPkgCheck);
    
    // 实现基类的虚函数，提供 shared_ptr
    virtual SipRtpGateway::RtpGatewayMsgSenderInterfacePtr getSharedPtr() override;
    ~Rtp2RtpChannel();

public:
    virtual bool receivedSBCRtpMsg(const std::string & strMsg, int size, bool bAudio, void * pConnection);

    virtual Common::NetReceiverPtr recvConnection(const Common::NetSenderPtr& sender);
    virtual void onConnReady();
    virtual void onConnClose();

    virtual int SendPacket(int channel, const void *data, int len, unsigned int level, unsigned int timestamp);
    virtual int SendRTCPPacket(int channel, const void *data, int len);

public:
    bool connectToRemote(const std::string & remoteIp, int remotePort, const std::string & localIp, int localPort);
    void setPeerChannel(const Rtp2RtpChannelPtr & peer);
    Rtp2RtpChannelPtr getPeerChannel();
    void clear();

    bool sendDataToPeer(const std::string & strMsg, int size, bool bAudio);
    int64_t getLastActiveTicks();
    void setSdp(const std::string & sdp, bool origin);
    std::string getSdp(bool origin);

    void setCalleeSdp(const std::string & sdp,  bool origin);
    std::string getCalleeSdp(bool origin);
    bool generateRtpListen(const int ports);
    void setCallerRecvPorts(std::vector<int> &ports);
    std::vector<int> getCallerRecvPorts();
    void setCalleeRecvPorts(std::vector<int> &ports);
    std::vector<int> getCalleeRecvPorts();
    void setMrfpRecvPorts(std::vector<int> &ports);
    std::vector<int> getMrfpRecvPorts();

    void setPrivateIpProps(bool isPrivate);
    bool getPrivateIpProps();

    void setCalleePrivateIpProps(bool isPrivate);
    bool getCalleePrivateIpProps();

    void setChannelId(int id);
    int getChannelId();

    void setCallNumber(bool isCaller, const std::string& number);
    std::string getCallNumber(bool isCaller);

    void setRecordTime(const std::string& time);
    std::string getRecordTime();

    void setAnswerTime();
    int64_t getAnswerTime();

    bool getEmptyPkgCheckPres();
    std::string getCallId();

    void setRecordPres(bool isRecord);
    bool getRecordPres();

    void setChannelPres(bool isCreated);
    bool getChannelPres();

    void setRedPayload(int pl);
    int getRedPayload();

    void setDtmfPayload(int pl);
    int getDtmfPayload();

    void setAcPayload(std::vector<StSdpCodec>& pl);
    std::vector<StSdpCodec> getAcPayload();

private:

    SipRtpImpPtr                m_server;
    std::string                 m_callId;
    Rtp2RtpChannelPtr           m_peer;
    tars::TC_ThreadRecMutex     m_mutexPeer;
    int                         m_isCaller;
    std::string                 m_sdp;
    std::string                 m_modifyedSdp;
    std::string                 m_calleeSdp;
    std::string                 m_modifyedCalleeSdp;

    std::string                 m_remoteHost;
    int                         m_remotePort;
    std::string                 m_localHost;
    int                         m_localPort;
    int64_t                  m_lastActiveTicks;
    Common::NetSenderPtr          m_audio_RTP_Listen_ConnectionPtr;
    Common::NetSenderPtr          m_audio_RTCP_Listen_ConnectionPtr;
    Common::NetSenderPtr          m_video_RTP_Listen_ConnectionPtr;
    Common::NetSenderPtr          m_video_RTCP_Listen_ConnectionPtr;
    std::string                 m_strName;
    std::vector<int>                 m_vectCallerRecvPorts;
    std::vector<int>                 m_vectCallerRecvPorts2;
    std::vector<int>                 m_vectCalleeRecvPorts;
    std::vector<int>                 m_vectCalleeRecvPorts2;
    std::vector<int>                 m_vectMrfpRecvPorts;
    bool                        m_bPrivateIp;
    bool                        m_bCalleeUsePrivateIp;

    int                         m_channelId;
    int                         m_dtmfPayload;
    int                         m_redPayload;
    std::vector<StSdpCodec>     m_vectSendCodec;

    tars::TC_ThreadRecMutex     m_mutexMediaCheckTime;
    int64_t                  m_mediaLstCheckTime;
    bool                        m_mediaEmptyPkgCheckPres;
    int                         m_mediaEmptyPkgNum;
    int                         m_mediaEmptyPkgChkInterval;
    bool                        m_qos;

    std::string                 m_callerNumber;
    std::string                 m_calleeNumber;
    std::string                 m_recordTime;
    bool                        m_recordStarted;
    bool                        m_channelCreated;
    int64_t                  m_answerTime;
    int                         m_rtpPackageNum;
    int                         m_rtcpPackageNum;

public:
    static  std::atomic<int>    m_lTotalChannelNum;
    static  std::atomic<int>    m_lTotalNewChannelNum;
    static  std::atomic<int>    m_iTotalDeleChannelNum;
};


class Rtp2RtpRelayer
{
public:

    Rtp2RtpRelayer()
    {
        m_server = 0;
        m_caller = 0;
        m_callee = 0;
        m_mrfp = 0;
    }

    Rtp2RtpRelayer(const SipRtpImpPtr& server, const std::string & callId, bool qos, int interval, bool emptyPkgCheck, bool callRecord);
    ~Rtp2RtpRelayer();

    Rtp2RtpChannelPtr createChannel(int caller, const std::string & sdp);
    Rtp2RtpChannelPtr getChannel(int caller);
    void clear();
    int64_t getLastActiveTicks();
    int64_t getCreateTicks();
    std::string getCallId();
    bool getQosPres();
    bool getCallRecordPres();

    private:

    std::string                 m_callId;
    SipRtpImpPtr                m_server;
    tars::TC_ThreadRecMutex     m_mutexChannels;
    Rtp2RtpChannelPtr           m_caller;
    Rtp2RtpChannelPtr           m_callee;
    Rtp2RtpChannelPtr           m_mrfp;
    int64_t                  m_createTicks;
    int                         m_mediaEmptyPkgChkInterval;
    bool                        m_qos;
    bool                        m_mediaEmptyPkgCheckPres;
    bool                        m_callRecord;

    public:
    static  std::atomic<int>                  m_lTotalRelayNum;
    static  std::atomic<int>                  m_lTotalNewRelayNum;
    static  std::atomic<int>                  m_iTotalDeleRelayNum;
};

/**
 *
 *
 */
class SipRtpImp : public VoipApp::SipRtp
                , public std::enable_shared_from_this<SipRtpImp>
{
public:
    /**
     *
     */
    virtual ~SipRtpImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */

     bool onActivate();
     void onDeactivate();
     void onShutdown();
     void onSchd();

     //bool getMainService(tars::ObjectServerPtr& service,std::string& name,bool &famous);
     void onUpdateConfigs();
     bool isMainServiceReady(std::string& reason);
     //void locateServer_begin(const std::string& mode,const tars::ServerId& serverId,const tars::ServerLocateResultPtr& result){}

    virtual tars::Bool allocRtpResource(tars::Bool isCaller,const map<std::string, std::string> & params,map<std::string, std::string> &retValus,tars::TarsCurrentPtr _current_) override;

    virtual tars::Bool releaseRtpResource(const std::string & callId,tars::TarsCurrentPtr _current_) override;

    virtual tars::Bool updateRecordStatus(const std::string & callId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;

public:
    static void * ExceptionAndStatisticThreadFunc(void * arg);
    int registerRtpResource();

    void clearRtp2RtpRelays();
    bool rtpConnectionBuild(RtpSenderPtr pSender, const map<std::string, std::string>& params,map<std::string, std::string>& outParams,std::string & reason);
    bool rtpConnectionBuild(RtpSenderPtr  pSender, const map<std::string, std::string>& params,map<std::string, std::string>& outParams,std::string & reason, JssmmeChannelPtr & pChannel);
    bool rtpConnectionBuild2(RtpSenderPtr pSender, const map<std::string, std::string>& params, map<std::string, std::string>& outParams, const std::string & sdp, vector<int> &vectPorts, std::string & reason);
    bool rtpConnectionBuildCaller(RtpSenderPtr pSender, const map<std::string, std::string>& params, map<std::string, std::string>& outParams, vector<int> & vectMrfpPorts, vector<int> & vectCallerPorts, vector<int> & vectCalleePorts, bool isPrivateIp, std::string & reason);
    bool rtpConnectionBuildMrfp(Rtp2RtpChannelPtr pSender, map<std::string, std::string>& outParams, vector<int> & vectPorts, const std::string & strCallId, std::string & reason);
    bool rtpConnectionBuild2(RtpSenderPtr pSender, const std::string & callId, const map<std::string, std::string>& params,map<std::string, std::string>& outParams,std::string & reason,  JssmmeChannelPtr & pChannel);

    bool decodeCodecCfg(const std::string & cfg, set<std::string> & setCfgs);
    bool JssmmeInit();
    bool getSdpParams(set<std::string> & strCfgs, StSdpMediaDesc & sdpCodec);
    void getAllSdpCodecCfgs(bool isApp, StSdpMediaDesc & stCfgSdp);
    void mediaChannelStatusNotify(const std::string &callId, const std::string & sipGatewayId, const std::string &reason);
    bool getMediaEmptyPkgChk();
    int getMediaEmptyPkgChkInterval();
    int getMediaEmptyPkgMinTime();
    double getMediaEmptyPkgChkPercent();

    bool getSdpParams(StSdpMediaDesc & sdpCodec);

    void processReConnMediaNego(Rtp2RtpChannelPtr& callerChannel, Rtp2RtpChannelPtr& calleeChannel, const std::string& callId, const std::string& sdp, bool isRecord);
    bool processMediaNego(Rtp2RtpChannelPtr& callerChannel, Rtp2RtpChannelPtr& calleeChannel, const std::string& callId, const std::string& sdp, bool isRecord = false, bool isNegoFinished = false);
    bool decodeSdp(Rtp2RtpChannelPtr& pChannel, const std::string& sdp);
    bool createChannels(Rtp2RtpChannelPtr& pChannel);
    SipRtpGateway::RtpGatewayInterfaceIPtr getJssmmeInterface();

public:
    JssmmeManagerPtr                                    m_jssmmeManager;
    std::string                                         m_appMediaCodes;
    std::string                                         m_sipMediaCodes;
    StSdpMediaDesc                                      m_appSdpDesc;
    StSdpMediaDesc                                      m_sipSdpDesc;

private:
    std::string                                         m_strSDP;                   //RPC 配置的SDP信息, 待获取配置项或者直接写定字符串
    SipRtpGateway::MediaStreamReceivingPortsManager            m_portManager;
    int                                                 m_iMinPort;                 //最小RTP媒体接收端口
    int                                                 m_iMaxPort;
    std::string                                         m_strListenIp;
    std::string                                         m_strPrivateIp;
    std::string                                         m_strDomainId;
    std::string                                         m_strDomainName;

    int64_t                                          m_LastSchdTimeInMs;         //定时查询设定
    int64_t                                          m_BaseSchdTimeInMs;
    int64_t                                          m_lastMinute;

    bool                                                m_bMainServiceReady;
    bool                                                m_bServiceActibed;

    int                                                 m_minNmsPort;
    int                                                 m_maxNmsPort;
    std::string                                         m_strMainObjId;

    tars::TC_ThreadRecMutex                             m_mutexRtpRelays;
    typedef map<std::string, Rtp2RtpRelayerPtr>         Rtp2RtpRelaysMap;
    Rtp2RtpRelaysMap                                    m_mapRtpRelays;

    int                                                 m_noMediaTimeout;
    int                                                 m_noAcceptedTimeout;
    bool                                                m_mediaEmptyPkgCheckPres;

    StSdpMediaDesc                                      m_sdpCodec;
    std::string                                         m_strRecordPath;
    SipRtpGateway::RtpGatewayInterfaceIPtr                             m_jssmmeInterface;

    int                                                 m_acceptedNoMediaTimeout;
    int                                                 m_mediaEmptyPkgChkInterval;
    int                                                 m_mediaEmptyPkgChkMinTime;
    double                                              m_mediaEmptyPkgChkPercent;
    int64_t                                          m_maxExpiredTime;
    bool                                                m_bMediaEmptyPkgChk;

    int                                                 m_jssmmeMaxChannel;
    int                                                 m_maxCahnnel;
    int                                                 m_jssmmeMaxChannelsPerThread;

    tars::TC_ThreadPool                                 m_threadPool;  // 线程池用于异常和统计处理

    //function members
public:

    bool freeListeningMediaPorts(const vector<int> & vecPorts, const std::string & callId);
    bool allocFreeEvenPorts(const std::string & strCallId, int portNum, std::vector<int> & vectPorts);
    void setLogsAndStatistics();   
};


class JssmmeChannel
{
public:
    JssmmeChannel(const std::string & callId);
    ~JssmmeChannel();

    int getChannel(bool caller);
    bool getEmptyPkgCheckPres();

public:
    std::string                 m_callId;
    RtpSenderPtr                m_server;
    tars::TC_ThreadRecMutex     m_mutexChannels;
    int                         m_sipChannelId;
    int                         m_appChannelId;
    std::string                 m_appSdpOffer;
    std::string                 m_appSdpAnswer;
    std::string                 m_sipSdpOffer;
    std::string                 m_sipSdpAnswer;
    int64_t                     m_createTime;
    bool                        m_channelCreated;
    bool                        m_recordStarted;
    bool                        m_mediaEmptyPkgCheckPres;
    int                         m_mediaEmptyPkgChkInterval;
    int                         m_sipDtmfPayload;
    int                         m_appDtmfPayload;
    int                         m_appRedPayload;
    int                         m_sipRedPayload;

    std::vector<StSdpCodec>          m_vectAppSendCodec;
    std::vector<StSdpCodec>          m_vectSipSendCodec;

    JssmmeMediaNegoStatus       m_appMediaNegoStatus;
    JssmmeMediaNegoStatus       m_sipMediaNegoStatus;

    bool                        m_bRecord;
    std::string                 m_recordPath;
    std::string                 m_caller;
    std::string                 m_callee;
    std::string                 m_st;

    int64_t                  m_at;
};

// class JssmmeManager
// {
// public:
//     JssmmeManager();
//     virtual ~JssmmeManager();

//     bool init();
//     bool term();
//     int createChannel();
//     int createChannel(const SipRtpImpPtr& server, const std::string & callId, bool caller, const std::string & sdp);
//     int setMaxChannelsPerThread(int channels);
//     int getMaxNumOfChannels();
//     int getChannel(const std::string & callId, bool caller);
//     JssmmeChannelPtr getChannel(const std::string & callId);
//     bool updateChannel(const std::string & callId, int channelId, bool isCaller);
//     int delChannel(int appChannelId, int sipChannelId);
//     int delChannel(int channelId);
//     void deleteChannel(const std::string & callId);
//     int setCallBack(int channelId, jssmme::Transport & callback);
//     int connectChannel(int appChannel, int sipChannel, int appDtmf, int sipDtmf);
//     bool disconnect(const std::string & callId);

//     bool insertChannel(const std::string & callId, JssmmeChannelPtr & channel);
//     int setSendCodec(int channelId, std::vector<StSdpCodec> & sdpCode);
//     int setSendRedCodec(int channelId, int iPayload);
//     int setLocalReceiver(int channelId, int port);
//     int setSendDestination(int channel1, int port, const std::string & ip);

//     int receivedRTPPacket(int channelId, const unsigned char *msg, int size);
//     int receivedRTCPPacket(int channelId, const unsigned char *msg, int size);

//     int setFecStatus(int channelId, int red);
//     int getCodecParams(set<std::string> codec, std::vector<StSdpCodec> & sdpCode);
//     bool createChannels(JssmmeChannelPtr pChannel, std::string & reason);
//     bool createChannels(JssmmeChannelPtr pChannel, std::string & reason, StSdpCodec &acPayload, int dtmfPayload = -1, int redPayload = -1);
//     bool recreateChannels(const std::string & callId);

//     void clearExpiredChannels();
//     int GetCodecByName(const char *pcName, jssmme::CodecInst *pCodec, int iFreq);

//     int mediaEmptyPktCheck(int channelId, unsigned int &level);
//     bool setAnswerTime(const std::string & callId);
//     bool startCallRecord(JssmmeChannelPtr& pChannel, int channelId);

// private:
//     jssmme::VoiceEngine* _VoE;
//     jssmme::VoEBase* _Base;
//     jssmme::VoECodec* _Codec;
//     jssmme::VoEVolumeControl* _Volume;
//     jssmme::VoEDtmf* _Dtmf;
//     jssmme::VoERTP_RTCP* _RtpRtcp;
//     jssmme::VoEAudioProcessing* _Apm;
//     jssmme::VoENetwork* _Netw;
//     jssmme::VoEFile* _File;
//     jssmme::VoEVideoSync* _Vsync;
//     jssmme::VoEEncryption* _Encr;
//     jssmme::VoEHardware* _Hardware;
//     jssmme::VoEExternalMedia* _Xmedia;
//     jssmme::VoENetEqStats* _Neteqst;

//     int64_t m_expiredTime;

// public:
//     std::atomic<int>                      m_channelCount;
//     std::atomic<int>                      m_channelIdCount;

// public:
//     tars::TC_ThreadRecMutex                            m_mutexCallIdChannel;
//     typedef std::map<std::string, JssmmeChannelPtr>         callIdJssmmeChannelMap;
//     callIdJssmmeChannelMap                             m_mapCallIdJssmmeChannels;

// };

/////////////////////////////////////////////////////
#endif
