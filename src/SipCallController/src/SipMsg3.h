#ifndef __SipMessage3_h
#define __SipMessage3_h

#include <string>
#include <map>
#include <memory>

#include "SipGateway/SipMsgDecode.h"
#include "SipGateway/SipMsgEncode.h"
#include "SipGateway/SipMsgDecodedPart.h"
#include "SipGateway/SipGatewayConnection.h"
#include "SipCloudGateway/SipDialog2.h"
#include "SipCloudGateway/CloudSipGatewayInterface.h"


class SipMessage;
typedef std::shared_ptr<SipMessage> SipMessagePtr;

class SipMessage
{
public:
    static bool dealInviteRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealInviteResponce(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId,  const std::string & remoteIp, int remotePort, const SipMsgDecodePtr & pstSipMsgDe);

    static bool dealAckRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealBYERequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealBYEResponce(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort);

    static bool dealCANCELRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealCANCELResponce(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort);

    static bool dealUPDATERequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealUPDATEResponse(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort);

    static bool dealPRACKRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealPRACKResponce(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort);

    static bool dealINFORequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealRegisterRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static bool dealOptionsRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params);

    static SipGateway::SipMsgDecodeResultType onReceiveSipMsgFromCloud(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr &pCloudSipGateway, void* pSipConnection,const std::string & remoteIp, int remotePort);

    static bool SendRequestINVITE(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg ,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestINVITEWithAuth(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg ,SipGateway::AuthType, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestACK(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestACK2(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestBYE(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestCANCEL(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestCANCEL2(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestPACK(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool SendRequestUPDATE(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId);
    static bool sendSipMessageRequestMsg(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, EN_SIP_METHOD reqMethod, const std::string & msg, const std::string & recvConnId);
    static bool sendSipMessageResponseMsg(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, const int rspCode, EN_SIP_METHOD reqMethod, const std::string & msg, const std::string & recvConnId);
    static bool sendSipMessageRequestMsg2(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, EN_SIP_METHOD reqMethod, const std::string & msg, const std::string & recvConnId);
    static bool sendSipMessageResponceRegister(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, const int rspCode, EN_SIP_METHOD reqMethod, const std::string &msg, const std::string & recvConnId);
    static bool sendSipMessageResponceOPTIONS(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, const int rspCode, EN_SIP_METHOD reqMethod, const std::string &msg, const std::string & recvConnId);
    static bool sendSipMessage183(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, int rspCode, EN_SIP_METHOD reqMethod, const std::string & newSdp, const std::string & recvConnId);
    static bool sendSipMessage100(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, const std::string & recvConnId);

    static bool onReiceivedSipMsgFromSipGateway(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
        const std::string & sipGatewayId, const std::string & connectionId, const std::map<std::string, std::string> & params);

    static bool CreateSipMsgFromSDP(const std::string & strBody, std::string & strSipMsg);

    static bool getMediaIpAndPortsFromSipMsg(const std::string & strSipMsgWithSDP,  vector<SDPMediaInfo> & vectSdpMediaInfo);

    static bool GetIpFromSDPConnField(const std::string & strCsSDP, std::string & ip);
    static bool GetIpFromSdp(const SipMsgDecodePtr & pstSipMsgDe, std::string & sdpIp, int &sdpPort, vector<std::string> & vectCoding);

    static bool sipMessageValidCheck(const std::string & msg);

    static bool sendSipMessageRequestMsgBye(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, EN_SIP_METHOD reqMethod, const std::string & recvConnId);

    static bool sendSipMessageExeptionMsg(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg,
        const int rspCode, EN_SIP_METHOD reqMethod, const std::string & reason, const std::string & msg, const std::string & recvConnId, bool resend = true);
    static bool getSipMsgSipDialogId(const std::string & strSipMsg, std::string & sipDialogId, std::string & callId, unsigned char & ucMethod, bool & req,unsigned int & ulStatusCode);
    static bool getSipGatewayIpPort(SipCloudGateway::SipDlgPtr & dlg, std::string & gwIp, int & gwPort, bool & isCaller, const std::string & recvConnId);
    static bool getSipGatewayIpPort2(SipCloudGateway::SipDlgPtr & dlg, std::string & gwIp, int & gwPort, const std::string & recvConnId);

    static bool CreateSDPWithNewIp(const std::string & strOldSDP, const std::string & strNewIP, std::string & strNewSDP);

    static bool sipFromCaller(SipCloudGateway::SipDlgPtr & sipDlg);
    static bool decodeSipUri(const std::string & sipUri, int & type, std::string& userName, std::string& registarIp, int& port);
    static std::string updateSipMsgHead(bool isCaller, SipCloudGateway::SipDlgPtr & dlg, SipMsgDecodePtr & pstSipMsgDe, EN_SIP_METHOD reqMethod, bool reqLine = true,  bool pCalledParty = false, bool byeReq = false);

    static bool getDiversionInfo(const std::string & diversion, std::string & sipuri);

    static bool CreateSDPWithNewPort(const std::string & strOldSDP, const vector<int> vectNewPorts, const std::string & strNewIP, std::string & strNewSDP);
    static bool CreateSDPWithNewPort(const std::string & strOldSDP, int newAudioPort, int newVideoPort, const std::string & strNewIP, std::string & strNewSDP);
    static bool GetIpPortFromSDPConnField(const std::string & strCsSDP, std::string & ip, int & port);
    static bool getMediaIpAndPortsOfSDPinSipMsg(const std::string & strSipMsgWithSDP,  vector<SDPMediaInfo> & vectSdpMediaInfo);

    /* calculate md5 */
    static bool authCalcRsp(const SipMsgDecodePtr & pstSipMsgDe, const SipProxy::SipRegisterSubscribePtr & pSipReg, unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp);
    static bool authCalcRsp(const SipMsgDecodePtr & pstSipMsgDe, const std::string &strUsername, const std::string &strPassword, 	unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp);

    /* calculate A1 */
    static bool calcA1(const std::string &strUsername, const std::string &strPassword,
        const std::string &strRealm, std::string &strA1);

    /* calculate A2 */
    static bool calcA2(unsigned char ucMethod, ST_SIP_REQ_URI *pstReqUri,
        std::string &strA2);

    /* calculate KD */
    static bool calcKd(const std::string &strA1, const std::string &strA2,
        const std::string &strNonce, const std::string &strCnonce,
        const std::string strNonceCount, const std::string &strQop,
        std::string &strKd);

    static bool updateSdpWithIp(const std::string& oldSdp, const std::string& remoteIp, const std::string& sdpIp, std::string& sdpNew);
    static bool privateIpCheck(const std::string& remoteIp);
    static std::string getConnId(const std::string& remoteIp, int remotePort, const std::string& routeIp, int routePort,
        const std::string& contactIp, int contactPort);

};



#endif /* __SipMessage_h */
