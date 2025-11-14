#ifndef _SIP_GATEWAY_INTERFACE_H_
#define _SIP_GATEWAY_INTERFACE_H_

#include "SipGatewayCommon.h"
#include "SipDialog.h"

namespace SipGateway
{
    class SipMsgCodecInterface
    {
    public:

        virtual SipGateway::SipDialogPtr getSipDialogPtr(const std::string & strSipDialogId) = 0;
        virtual bool ereaseSipProxyExceptionConnection(const std::string & sipDialogId, const std::string & sipProxyId) = 0;
        virtual bool onSipMessageRequest(const SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg,  const std::string & sipProxyId, const std::string & connectionId, const std::string & sdp, const std::map<std::string, std::string> & params) =0;
        virtual bool onSipMessageResponse(int iResCode, const SipRequestMethodType forReqMethod, SipGateway::SipDialogPtr & dlg, const std::string & sipProxyId, const std::string & connectionId, const std::string & sdp) = 0;
        virtual bool updateFeeChargedTimer(const std::string & callIdHost, bool req, unsigned char method, int resCode) =0;
    };
    typedef std::shared_ptr<SipMsgCodecInterface> SipMsgCodecInterfacePtr;

    class SipMsgSenderInterface
    {
    public:

        virtual bool sendEncodedMessage(const std::string & strMsg, int iSize, bool bRequest, SipGateway::SipRequestMethodType reqMethod, const SipDialogPtr & dlg) =0;
    };
    typedef std::shared_ptr<SipMsgSenderInterface> SipMsgSenderInterfacePtr;


    struct SipAccountProxyInfos
    {
        std::string  _strSipAccount;
        std::string  _authName;
        std::string  _password;
        std::string  _strSipProxyId;
        bool         _bISBCAccessed;
        std::string  _sbcIp;
        int          _sbcPort;
        std::string  _sipProxyIp;
        int          _sipProxyLisPort;
        std::string  _registarIp;
        int          _registarPort;
        std::string  _coreNetId;
        std::string  _calleePrefix;
        int          _charged;
        int          _remainMinute;
        unsigned char   _fromSipAddrType;
        unsigned char   _toSipAddrType;

        SipAccountProxyInfos()
        {
            _sbcPort = 0;
            _sipProxyLisPort = 0;
            _registarPort = 0;
            _charged = 0;
            _remainMinute = 0;
            _fromSipAddrType = EN_SIP_ADDR_SPEC_SIP;
            _toSipAddrType = EN_SIP_ADDR_SPEC_SIP;
        }
    };

    class AchieveSipAccountInfosResult
    {
    public:
        virtual void onAchieveSipAccountInfosResult(bool bSuccess, const SipAccountProxyInfos & accountInfos, int errCode, SipDialogPtr & sipDlg, const std::string & reason, const std::string & display, const std::string & caller, const std::string & callee) = 0;
    };
    typedef std::shared_ptr<AchieveSipAccountInfosResult> AchieveSipAccountInfosResultPtr;

    class AllocContentResult
    {
    public:
        virtual void onContentAlloced(const std::string &cp, const std::map<std::string, std::string> & params, const std::string & reason) = 0;
    };
    typedef std::shared_ptr<AllocContentResult> AllocContentResultPtr;

    class AllocEndpointResult
    {
    public:
        virtual void onEndpointAlloced(const std::string & ep,const std::string & cp, const std::map<std::string, std::string> & params, const std::string & reason) = 0;
    };
    typedef std::shared_ptr<AllocEndpointResult> AllocEndpointResultPtr;

    class AllocRtpGwArcResult
    {
    public:
        virtual void onRtpGwArcAlloced(const std::string & host, int port, const std::map<std::string, std::string> & params, const std::string & reason) = 0;
    };
    typedef std::shared_ptr<AllocRtpGwArcResult> AllocRtpGwArcResultPtr;

    class GetSipAccountInfoExResult
    {
    public:
        virtual void onGetSipAccountInfoExResult(bool bSuccess, const VoipApp::SipAccountInfo & account, const std::string & newProxyId,const std::string & reason) = 0;
    };
    typedef std::shared_ptr<GetSipAccountInfoExResult> GetSipAccountInfoExResultPtr;

    class GetAvailableSipAccountResult
    {
    public:
        virtual void onGetAvailableSipAccountResult(bool bSuccess, const VoipApp::SipAccountInfo & account, bool isFreeStyle,const std::string & sipProxyId, const std::string & reason) = 0;
    };
    typedef std::shared_ptr<GetAvailableSipAccountResult> GetAvailableSipAccountResultPtr;


    class AllocRtpGwCpResourceResult
    {
    public:
        virtual void onAllocRtpGwCpResourceResult(bool bSuccess, const std::map<std::string, std::string>& params,const std::string & reason) = 0;
    };
    typedef std::shared_ptr<AllocRtpGwCpResourceResult> AllocRtpGwCpResourceResultPtr;

    class QueryRemainMinuteResult
    {
    public:
        virtual void onQueryRemainMinuteResult(bool bSuccess, const std::map<std::string, std::string> & response,const std::string & reason) = 0;
    };
    typedef std::shared_ptr<QueryRemainMinuteResult> QueryRemainMinuteResultPtr;

    class QueryCallBackServerResult
    {
    public:
        virtual void onCallBackServerResult(bool bSuccess, const std::map<std::string, std::string> & response,const std::string & reason) = 0;
    };
    typedef std::shared_ptr<QueryCallBackServerResult> QueryCallBackServerResultPtr;

    class QueryCallBackBlacklistResult
    {
    public:
        virtual void onQueryCallBackBlacklistResult(bool bSuccess, const std::map<std::string, std::string> & response,const std::string & reason) = 0;
    };
    typedef std::shared_ptr<QueryCallBackBlacklistResult> QueryCallBackBlackListResultPtr;

    class GetAppSdpResult
    {
    public:
        virtual void onGetAppSdpResult(const std::string & appSdp, const std::map<std::string, std::string> & params, const std::string & reason) = 0;
    };
    typedef std::shared_ptr<GetAppSdpResult> GetAppSdpResultResultPtr;

    class AchieveRtpResourceResult
    {
    public:
        virtual void onAchieveRtpResourceResult(const std::string & appSdp, const std::map<std::string, std::string> & params, const std::string & reason) = 0;
    };
    typedef std::shared_ptr<AchieveRtpResourceResult> AchieveRtpResourceResultPtr;
};

#endif

