#ifndef _SIP_PROXY_INTERFACE_H_
#define _SIP_PROXY_INTERFACE_H_

#include "SipRegisterSubscribe.h"
#include "SipGateway/src/SipGatewayCommon.h"
#include "SipGatewayIdRecordManager.h"
//#include "SipMessageGateway/SipMessage.h"

namespace SipProxy
{
    class SipProxyMsgCoderInterface
    {
    public:

        virtual bool onSipMessageRequest(const SipGateway::SipRequestMethodType reqMethod, const SipRegisterSubscribePtr & ptr, void * pConnection) = 0;
        virtual bool onSipMessageResponse(const SipGateway::SipRequestMethodType reqMethod, const SipGateway::SipResponseCodeType & rspCode, const SipRegisterSubscribePtr & ptr, void * pConnection, bool InvalidMsg = false) = 0;
        virtual bool onOthersSipMessageTransfer(const std::string &strMsgBuffer,int iBufLen, const std::string & sipDialogId,
            const std::string & callId, bool bInitInvite, void * pConnection, bool exceptionConnection, const std::string & toSBCConnectionId, bool bReq, unsigned char method, const std::map<std::string, std::string>& others) = 0;
        virtual bool onOthersSipMessageTransferEx(const std::string & sipMsg, int iBufLen, const std::string & callIdHost, bool bRequest, unsigned char method, unsigned int resCode,
            const std::string & fromUsername, const std::string & toUserName, const std::string & registarIpName, void * pConnection, const std::string & toSBCConectionId) = 0;

        virtual bool saveExceptionConnection(void * pConnection) = 0;
        virtual SipProxy::SipRegisterSubscribePtr getSipRegisterSubscribe(const std::string & sipUri, const std::string & callIdNum, SipGateway::SipRequestMethodType methodType) = 0;

        virtual std::string getSipProxyId() = 0;
        virtual bool getConnectionIdToSBC(const std::string & sipAccount, const std::string & registerHost, std::string & toSBCConntionId) = 0;
        virtual bool checkCallIdHostExist(const std::string & callIdHost, SipDialogRecord & sipDialogRecord) = 0;
        virtual bool transferPingPongMsg(const std::string &strMsgBuffer,int iBufLen, bool isPing, void * pConnection) = 0;
        virtual bool toSBCConnectionClosed(const std::string & id, const std::string & localHost, int localPort, const std::string & remoteHost, int port, const std::string & transProtocol) = 0;

        //virtual bool onSipMessageRequest2(const SipMessageGateway::SipMessagePtr & ptr, void * pConnection) = 0;
        //virtual bool onSipMessageResponse2(const std::string & msg, const SipGateway::SipRequestMethodType reqMethod, unsigned int rspCode,  const SipMessageGateway::SipMessagePtr & ptr, void * pConnection, bool InvalidMsg = false) = 0;
        //virtual SipMessageGateway::SipMessagePtr getSipMessage(const std::string & dstUri, const std::string & callIdHost) = 0;
        virtual bool checkIpInWhitelist(const std::string & remoteIp) = 0;

    };
    typedef std::shared_ptr<SipProxyMsgCoderInterface> SipProxyMsgCoderInterfacePtr;


    class SipProxyMsgSenderInterafece
    {
    public:
        virtual bool sendEncodedMessage(const std::string & strMsg, int iSize, const std::string & connectionId, bool req, std::string & reason) =0;
        virtual bool sendEncodedMessage2(const std::string & strMsg, int iSize, void * pConnection, std::string & reason) =0;

    };
    typedef std::shared_ptr<SipProxyMsgSenderInterafece> SipProxyMsgSenderInterafecePtr;

};

#endif




