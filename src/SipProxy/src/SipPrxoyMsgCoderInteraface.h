#ifndef _SiPPrxoy_msg_Coder_Interface_H_
#define  _SiPPrxoy_msg_Coder_Interface_H_

#include "SipRegisterSubscribe.h"

#include "SipGateway/SipGatewayCommon.h"
#include "SipGatewayIdRecordManager.h"


namespace SipProxy
{
    class SipProxyMsgCoderInterface :  virtual public Common::Shared
    {
    public:

        virtual bool onSipMessageRequest(const SipGateway::SipRequestMethodType reqMethod,   const SipRegisterSubscribePtr & ptr, void * pConnection) = 0;
        virtual bool onSipMessageResponse(const SipGateway::SipRequestMethodType reqMethod, const SipGateway::SipResponseCodeType & rspCode,  const SipRegisterSubscribePtr & ptr, void * pConnection, bool InvalidMsg = false) = 0;
        virtual bool onOthersSipMessageTransfer(const Common::String &strMsgBuffer,int iBufLen, const Common::String & sipDialogId,
            const Common::String & callId, bool bInitInvite,  void * pConnection, bool exceptionConnection, const Common::String & toSBCConnectionId, bool bReq, unsigned char method) = 0;
        virtual bool onOthersSipMessageTransferEx(const Common::String & sipMsg, int iBufLen, const Common::String & callIdHost, bool bRequest, unsigned char method, unsigned int resCode,
            const Common::String & fromUsername, const Common::String & toUserName, const Common::String & registarIpName,  void * pConnection, const Common::String & toSBCConectionId) = 0;

        virtual bool saveExceptionConnection( void * pConnection) = 0;
        virtual SipProxy::SipRegisterSubscribePtr getSipRegisterSubscribe(const Common::String & sipUri, SipGateway::SipRequestMethodType methodType) = 0;

        virtual Common::String getSipProxyId() = 0;
        virtual bool getConnectionIdToSBC(const Common::String & sipAccount, const Common::String & registerHost, Common::String & toSBCConntionId) = 0;
        virtual bool checkCallIdHostExist(const Common::String & callIdHost, SipDialogRecord & sipDialogRecord) = 0;
        virtual bool transferPingPongMsg(const Common::String &strMsgBuffer,int iBufLen, bool isPing, void * pConnection) = 0;
        virtual bool toSBCConnectionClosed(const Common::String & id, const Common::String & localHost, int localPort, const Common::String & remoteHost, int port, const Common::String & transProtocol) = 0;

    };

    typedef Common::Handle<SipProxyMsgCoderInterface> SipProxyMsgCoderInterfacePtr;


};


#endif

