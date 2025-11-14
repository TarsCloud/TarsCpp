#ifndef _SIP_CLOUD_INTERFACE_H_
#define _SIP_CLOUD_INTERFACE_H_

#include "SipDialog2.h"
#include "ENUMProxy/src/SipRouter.h"

namespace SipCloudGateway
{
    class CloudSipGatewayMsgCoderInterface
    {
    public:
        virtual bool onSipMessageRequest(EN_SIP_METHOD reqMethod, SipDlgPtr & sipDlg,  const std::string & sipGatewayId, const std::string & connectionId, const std::string & sdp, const std::map<std::string, std::string> & params, const std::string & msg, const std::string & recvConnId) = 0;
        virtual bool onSipMessageResponse(int iResCode, EN_SIP_METHOD forReqMethod, SipDlgPtr & sipDlg, const std::string & sipGatewayId, const std::string & sdp, const std::string & msg, const std::string & recvConnId) = 0;
        virtual bool sendEncodeSipMsg(const std::string & strMsg, int iSize, bool bRequest, EN_SIP_METHOD reqMethod, SipDlgPtr & sipDlg, const std::string & recvConnId, bool reSend = false) = 0;
        virtual SipDlgPtr getSipDlgPtr2(const std::string & strSipDialogId) = 0;
        virtual SipDlgPtr getSipDlgPtrByCallId(const std::string & callId) = 0;
        virtual bool checkIpInWhitelist(const std::string & remoteIp) = 0;
        virtual bool toCloudConnectionClosed(const std::string & id, const std::string & localHost, int localPort, const std::string & remoteHost, int port, const std::string & transProtocol) = 0;
        virtual bool privateIpCheck(const std::string & remoteIp) = 0;
        virtual bool isLocalConn(const std::string & sbcIp) = 0;
    };
    typedef std::shared_ptr<CloudSipGatewayMsgCoderInterface> CloudSipGatewayMsgCoderInterfacePtr;

    class AlloRtpResourceResult
    {
    public:
        virtual void onAlloRtpResourceResult(bool bSuccess, const std::map<std::string, std::string> & params,const std::string & reason) = 0;
    };
    typedef std::shared_ptr<AlloRtpResourceResult> AlloRtpResourceResultPtr;

    class InsertCallIdRecordResult
    {
    public:
        virtual void onInsertCallIdRecordResult(bool bSuccess, const std::map<std::string, std::string> & params,const std::string & reason) = 0;
    };
    typedef std::shared_ptr<InsertCallIdRecordResult> InsertCallIdRecordResultPtr;

    class GetCallIdRecordResult
    {
    public:
        virtual void onGetCallIdRecordResult(bool bSuccess, const std::string & serviceId, const std::string & reason) = 0;
    };
    typedef std::shared_ptr<GetCallIdRecordResult> GetCallIdRecordResultPtr;

    class GetJegoRouteInfoResult
    {
    public:
        virtual void onGetJegoRouteInfoResult(bool bSuccess, VoipApp::JegoRouterInfo info, std::map<std::string, std::string> &outPrams, std::string & reason) = 0;
    };
    typedef std::shared_ptr<GetJegoRouteInfoResult> GetJegoRouteInfoResultPtr;

    class GetOcsChargeStatusResult
    {
    public:
        virtual void onGetOcsChargeStatusResult(bool bSuccess, std::string & reason) = 0;
    };
    typedef std::shared_ptr<GetOcsChargeStatusResult> GetOcsChargeStatusResultPtr;

};

#endif