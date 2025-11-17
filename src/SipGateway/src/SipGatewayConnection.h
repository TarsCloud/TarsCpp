#ifndef __SipGatewayConnection_h
#define __SipGatewayConnection_h

#include <atomic>
#include <mutex>
#include <memory>
#include <vector>
#include <string>

#include "RtpGateway/src/RtpGatewayMsgSenderInterface.h"
//#include "SipGatewayEntry/SipGatewayEntryCommon.h"
#include "SipGateway/src/SipGatewayInterface.h"
#include "SipProxy/src/SipProxyInterface.h"
#include "SipCallController/src/CloudSipGatewayInterface.h"
#include "Common/Net.h"
#include "util/tc_timeprovider.h"

namespace SipGateway
{
    class GatewayConnection;
    typedef std::shared_ptr<GatewayConnection> GatewayConnectionPtr;

    enum TransType
    {
        TransTypeSip = 0,   //sip
        TransTypeAudio = 1, //audio
        TransTypeVideo = 2  //video
    };

    //定义使用SipGatewayConnection的模块
    enum ConnectionUsedModulType
    {
        SipGatewayModul = 0,
        SipProxyModul,
        RtpGatewayModul,
        SipGatewayEntryModule,
        SipMessageGatewayModule,
        AcdGatewayModule,
        CloudSipGatewayModul,
        MgcfSipGatewayModul
    };

    enum SipMsgDecodeResultType
    {
        kSipMsgDecodeFailed = 0,
        kSipMsgDecodeSuccessed = 1,
        kSipMsgDecodeToSave = 2
    };

    enum TransProtocolType
    {
        kTransTCP = 0,
        kTransUDP = 1
    };

    enum TransProtocolChangeType
    {
        kTransChangeNone = 0,
        kTransChangeTCP2UDP = 1,
        kTransChangeUDP2TCP
    };

    class MsgRemain
    {
    public:

        MsgRemain(const std::string & msg, const std::string & id):
          _msg(msg), _connectionId(id)
          {
              _ticks = TC_TimeProvider::getInstance()->getNowMs();
          }

          std::string       _msg;
          std::string       _connectionId;
          int64_t           _ticks;
    };

    class MsgEvent : virtual public MsgRemain
    {
    public:

        MsgEvent(const std::string & msg, const std::string & id, const std::string & remoteIp, int iRemotePort, GatewayConnectionPtr conn):
          MsgRemain(msg, id),_remoteIp(remoteIp), _remotePort(iRemotePort)
        {
            _conn = conn;
        }

        std::string       _remoteIp;
        int                  _remotePort;
        GatewayConnectionPtr _conn;
    };
    typedef std::shared_ptr<MsgEvent> MsgEventPtr;

    const std::string SIP_TRANS_PROTOCOL_TCP = "tcp";
    const std::string SIP_TRANS_PROTOCOL_UDP = "udp";

    class GatewayConnection: virtual public Common::NetReceiver,
                             public std::enable_shared_from_this<GatewayConnection>
    {
    private:

        SipProxy::SipProxyMsgCoderInterfacePtr          _sipProxyDecoder;
        SipRtpGateway::RtpGatewayMsgSenderInterfacePtr     _rtpGatewayDecoder;
        SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr    _sipGwDecoder;

        std::mutex                                _mutexSender;
        Common::NetSenderPtr                             _sender;

    public:

        GatewayConnection(const SipProxy::SipProxyMsgCoderInterfacePtr & decode, const Common::NetSenderPtr& sender, ConnectionUsedModulType modulType,const std::string& protocol);
        GatewayConnection(const SipProxy::SipProxyMsgCoderInterfacePtr &decode, ConnectionUsedModulType modulType);
        GatewayConnection(const SipProxy::SipProxyMsgCoderInterfacePtr &decode, ConnectionUsedModulType modulType,const std::string& protocol,
            const std::string& localHost,int localPort,const std::string& sbcIp,int sbcPort, const std::string & id);

        GatewayConnection(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & decode, const Common::NetSenderPtr& sender, ConnectionUsedModulType modulType,const std::string& protocol);
        GatewayConnection(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr &decode, ConnectionUsedModulType modulType);
        GatewayConnection(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr &decode, ConnectionUsedModulType modulType,const std::string& protocol,
            const std::string& localHost,int localPort,const std::string& sbcIp,int sbcPort, const std::string & id);

        GatewayConnection(const SipRtpGateway::RtpGatewayMsgSenderInterfacePtr & decode, const Common::NetSenderPtr& sender, ConnectionUsedModulType modulType);
        GatewayConnection(const SipRtpGateway::RtpGatewayMsgSenderInterfacePtr & decode, TransType transType);

        void constructCommonPart();
        virtual ~GatewayConnection();

        GatewayConnection(const GatewayConnection & connect);
        GatewayConnection & operator = (const GatewayConnection & connect);

    public:
        virtual void recv(const unsigned char *data,int dataLen);
        virtual void onConnReady() ;
        virtual void onConnClose();

        bool send(const unsigned char *data,int dataLen);
        bool setSender(const Common::NetSenderPtr& sender);
        Common::NetSenderPtr getSender();
        bool checkSendValid();

        void setEraseTime(std::string erasePath = "");
        int64_t getCreateTime();
        int64_t getEraseTime();
        std::string getErasePath();

        void setConnectionId(const std::string & id);
        std::string getConnectionId();
        void setToSBCConnection(bool bToSBC);
        bool getToSBCConnection();
        void setConnectionInfos(const std::string & localHost, int localPort, const std::string & remoteHost, int port, const std::string & transProtocol);
        void clearUp();
        SipGateway::TransProtocolType getTransProtocol();
        std::string getSBCIp();
        std::string getRemoteHost();
        int getLocalPort();

    public:

        std::string generateNewConnectionId();
        void setSipDialogId(const std::string & sipDialogId);
        std::string  getSipDialogId();
        ConnectionUsedModulType getUsedModule();

        static int getContentLength(std::string &data);
        static void unpackTcpStreamByBob(std::string &remainMsg, const std::string &oriMsg, vector<std::string> &validMsgs);

        void pushEvent(const std::string & msg, const std::string & remoteIp, int remotePort);
        int64_t getActiveTicks();

        static std::atomic<int>     _totalNum;
        static std::atomic<int>     _newNum;
        static std::atomic<int>     _deleteNum;

        int64_t                    _createTime;
        int64_t                    _eraseTime;
        bool                            _bOnClosed;

        std::string                  _randomId;
        std::string                  _strConnectionID;
        ConnectionUsedModulType         _eUsedModul;
        std::string                  _strSipDialogId;
        std::string                  _strErasePath;
        bool                            _bToSBC;
        TransType                       _eTransType;

        std::string                  _localHost;
        int                             _localPort;
        std::string                  _remoteHost;
        int                             _remotePort;
        SipGateway::TransProtocolType   _transProtocol;

        std::mutex               _mutexMsgBuffer;
        std::string                  _msgBuffer;

        std::mutex               _mutexCallProxying;
        bool                            _callProxying;
        //Common::ApplicationPtr          _application;
        //Common::AppEventArrayPtr        _eventArrayPtr;
        int64_t                    _lastActiveTicks;

    };

};

#endif //__SipConnectionManager_h