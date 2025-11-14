#ifndef __SipMsg_h
#define __SipMsg_h

#include "SipMsgDecode.h"
#include "SipMsgEncode.h"
#include "SipDialog.h"
#include "SipGatewayConnection.h"
#include "SipGatewayInterface.h"
#include "SipProxy/src/SipRegisterSubscribe.h"
#include "SipProxy/src/SipProxyInterface.h"
#include "RtpGateway/src/RtpGatewayCommon.h"

class SipMsg;
typedef std::shared_ptr<SipMsg> SipMsgPtr;

class SipMsg
{
public:
    /**
    * @create first register.
    *
    * @param [out] pstEnSipMsg.
    */
    static bool createFirstRegister(SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create second register by 401.
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool createSecondRegister(const SipMsgDecodePtr &pstDeSipMsg, SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create SUBSCRIBE sip msg by 200 OK register response
    *
    * @param [in] pstDeSip200.
    * @param [out] pstEnSipMsg.
    */
    static bool createSubsBy200(const SipMsgDecodePtr &pstDeSip200, SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create INVITE sip msg
    *
    * @param [in] strFromNumber.
    * @param [in] strToNumber.
    *
    * @param [out] pstEnSipMsg.
    */
    static bool createInvite(const std::string &strFromNumber, const std::string &strToNumber,const std::string & strMediaListenIp,
        SipMsgEncodePtr &pstSipMsgEn);

    /**
    * @create 100 trying sip msg.
    *
    * @param [in] pstSipMsgDe.
    *
    * @param [out] pstSipMsgEn.
    */
    static bool create100Trying(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn);

    /**
    * @create 180 ringing sip msg.
    *
    * @param [in] pstSipMsgDe.
    *
    * @param [out] pstSipMsgEn.
    */
    static bool create180Ringing(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn, std::string & strToTag);

    /**
    * @create 200 sip msg by INVITE.
    *
    * @param [in] pstSipMsgDe.
    *
    * @param [out] pstSipMsgEn.
    */
    static bool create200ByInvite(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn,const std::string & strToTag);

    /* calculate md5 */
    static bool authCalcRsp(const SipMsgEncodePtr & pstSipMsgEn, const SipProxy::SipRegisterSubscribePtr & pSipReg, unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp);

    static bool authCalcRsp(const SipMsgEncodePtr & pstSipMsgEn, const std::string &strUsername, const std::string &strPassword, 	unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp);


    static bool testCalcResponse(const std::string & username, const std::string & passWord, const std::string & nonce,
        const std::string & realm,const std::string & cnonce, ST_SIP_REQ_URI *pstReqUri);

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

    /**
    * @copy header contact to response.
    *
    * @param [in] pstSipMsgDe.
    *
    * @param [out] pstSipMsgEn.
    */
    static bool cpyHdrContact(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn);

    static bool printDecodeMsg(const SipMsgDecodePtr &pstSipMsgDe);

    static bool printEncodeMsg(const SipMsgEncodePtr *pstSipMsgEn);

    static bool createFirstRegister_xjp(SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create Notify sip msg by 200 OK of  Subscribe response
    *
    * @param [in] pstDeSip200.
    * @param [out] pstEnSipMsg.
    */
    static bool createNotifyBySub200(const SipMsgDecodePtr &pstSub200, SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create second register by 200 of invite.
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool createAck(const SipMsgDecodePtr &pstDeSipMsg, SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create second register by 200 of invite.
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    *
    */
    static bool decodeNotify(const SipMsgDecodePtr &pstDeSipMsg);

    /**
    * @create first register.
    *
    * @param [out] pstEnSipMsg.
    */
    static bool createMessage(SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create BYE by the 200 OK or Invite
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool createBye(const SipMsgDecodePtr &pstDeSipMsg, SipMsgEncodePtr &pstEnSipMsg);

    /**
    * @create BYE by the 200 OK or Invite
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool createResponce(const SipMsgDecodePtr &pstDeSipMsg, SipMsgEncodePtr &pstEnSipMsg,int responceCode);

    /**
    * @
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool decodeInviteResponce(const std::string & msg);

    /**
    * @decode msg
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool decodeSipMsg(const std::string & msg, SipGateway::SipDialogPtr & pstSipDialog);

    /**
    * @
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool encodeSipMsgResponce(const SipGateway::SipDialogPtr & pstSipDialog, const SipGateway::SipResponseCodeType rspCode, const SipGateway::SipRequestMethodType reqMethod, std::string & msg);

    /**
    * @
    *
    * @param [in] pstDeSipMsg, this is a 401 sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool getMsgCallIdHost(const std::string & sipMsg, std::string & callIdHost, bool & bReq, bool & bInvite);

    static bool sipMessageValidCheck(const std::string & msg);

    static bool __sendSipMessageResponseSipMsg(int rspCode, const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    //================================== 以下方法暴露给SipGateWay使用 ===================================//

    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool sendSipMessageRequestSipMsg(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg,const  SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @be rpc call sip ,encode Responce, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool sendSipMessageResponseSipMsg(const SipGateway::SipResponseCodeType rspCode, const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);
    static bool sendSipMessageResponseSipMsg2(int rspCode, const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);
    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestINVITE(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestINVITE_WithAuth(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg,SipGateway:: AuthType authType, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestACK(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg,const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestBYE(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg,const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestCANCEL(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestPACK(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @be rpc call sip ,encode Request, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestUPDATE(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway);

    /**
    * @ get the SipGateway service name from Sip message send by SipProxy,with format like "SipGateway5-2".
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool getSipGWServiceNameFromSipMsg(const std::string & sipMsg, const std::string & currServiceName, std::string & serviceName);

    //收到SBC发送的Sip消息，decode，如果是REGISTER或者SUBSCRIBE，则进一步解析，将头域值存储在SipProxy中的集合中，否则转发至SIpGateway服务
    static SipGateway::SipMsgDecodeResultType onReceiveSipMsgFromSBC(const std::string &strBuffer,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const std::string & remoteIp, int remotePort);
    static SipGateway::SipMsgDecodeResultType onReceiveSipMsgFromMGCF(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const std::string & remoteIp, int remotePort);

    /**
    * @be rpc call sip ,encode Request REGISTER, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestREGISTER_UnAoth(const SipProxy::SipRegisterSubscribePtr & pSipReg,const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy);

    /**
    * @be rpc call sip ,encode Request REGISTER, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestREGISTER_WithAoth(const SipProxy::SipRegisterSubscribePtr & pSipReg, const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy, const SipGateway::SipResponseCodeType & rspCode, bool refreshRegister);

    /**
    * @be rpc call sip ,encode Request SUBSCRIBE, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SendRequestSUBSCRIBE(const SipProxy::SipRegisterSubscribePtr & pSipReg, EN_SIP_EVNT_PKG eventType, const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy, bool refreshSubs);

    /*
    * @be SIpProxy�代替Client 发送注册以及订阅请求至SBC，并接收响应； SBC不会将REGISTER或者SUBSCRIBE请求发送给SIpProxy
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool dealRegisterResponceFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);

    /*
    * @be SIpProxy代替Client 发送注册以及订阅请求至SBC，并接收响应； SBC不会将REGISTER或者SUBSCRIBE请求发送给SIpProxy
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool dealSubscribeResponceFromSBC(const std::string &strBuffer,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);

    /*
    * @be 为防止 Connection 的异常，需要将接收到的除上述 REGISTER 和SUBSCRIBE响应之外的SIP消息进一步处理，然后转发至SIpGateway服务进程
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool dealOthersSipMessgaeFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort);

    /*
    *  处理转发收到的非注册 定义等SIP消息至ConfDelivery服务
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool dealOthersSipMessgaeFromSBC_Ex(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);

	static bool dealOthersSipMessgaeFromMgcf(const std::string &strSipMsg,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort);

    /*
    * @be 处理 NOTIFY请求，并校对是否已订阅， RFC3265 3.3.4
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool dealNotifyRequestFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);

    /*
    * @be 处理 OPTIONS 请求
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool dealOPTIONSRequestFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);


    static bool dealOPTIONSResponseFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);

    /**
    * @be rpc call sip ,encode Responce, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool sendRegisterSubscribeResponse(const SipGateway::SipResponseCodeType rspCode, const SipGateway::SipRequestMethodType reqMethod, const SipProxy::SipRegisterSubscribePtr & pSipReg,  const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy);

    /**
    * @be rpc call sip ,encode Responce, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool sendOPTIONSResponse(const SipProxy::SipRegisterSubscribePtr & pSipReg, const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy);

    static bool SendRequestOPTIONS(const SipProxy::SipRegisterSubscribePtr & pSipReg, const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy);

    /**
    * @be rpc call sip ,decode sip msg,and return the SipDialog id
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool getSipMsgSipDialogId(const std::string & strSipMsg, std::string & sipDialogId, std::string & callId, unsigned char & ucMethod, bool & req,unsigned int & ulStatusCode);

    /**
    * @be rpc call sip ,decode sip msg,and return the ip and ports that receive the media stream
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool getMediaIpAndPortsOfSDPinSipMsg(const std::string & strSipMsgWithSDP,  vector<SDPMediaInfo> & vectSdpMediaInfo);

    /**
    * @be generate Sip msg from contain the SDP
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool CreateSipMsgFromSDP(const std::string & strBody, std::string & strSipMsg);

    /**
    * @be negotiate the SDP, generate answer from offer by comparing the offer with local SDP ability
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool SDPNegotiation(const std::string & strSipMsgOffer, const std::string & strSipMsgLocalCapacity, std::string & negotiatedSDP);

    /**
    * @be negotiate the media session
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool NegotiateMedialDesc(const ST_SDP_SESS_DESC & offerSessDesc, const ST_SDP_SESS_DESC & localSessDesc, ST_SDP_SESS_DESC & answerSessDesc, SipMsgEncodePtr & pEncoder);

    /**
    * @be replace the media receiving port
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool CreateSDPWithNewPort(const std::string & strOldSDP, const vector<int> vectNewPorts,const std::string & strNewIP,  std::string & strNewSDP);
    static bool CreateSDPWithNewPort2(const std::string & strOldSDP,  const vector<int> vectNewPorts,const std::string & strNewIP,  std::string & strNewSDP);

    /**
    * @be replace the media receiving port
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool CreateSDPWithNewPort(const std::string & strOldSDP, int newAudioPort, int newVideoPort, const std::string & strNewIP,  std::string & strNewSDP);

    /**
    * @be get the number of media port in SDP
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool GetSDPMediaPortNum(const std::string & strSDP, int & portNum);

    /**
    * @be get the number of media port in SDP
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool GetSipSDPMediaNumber(const std::string & strSipWithSDP, int & num);

    /**
    * @be replace the media ports with new ports
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool ReplaceSDPWithNewPorts(const std::string & strSipWithOldPorts,const vector<int> & vectPorts, std::string & strSipWithNewPorts);

    /**
    * @be get SDP
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool GetSipMsgSDPBody(const std::string & sipMsg, std::string & strSDP);

    /**
    * @be 替换字符串，替换ip以及音频端口号
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    //static bool CreateSDPWithNewPort(const std::string & strOldSDP, int iNewPort,const std::string & strNewIP, std::string & strNewSDP);

    /**
    * @be 取 SipProxy Id 中的服务名称，如 SipProxy@SipProxy.CN01.0.Main， 取 SipProxy.CN01.0
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    static bool getBriefSipProxyId(const std::string & strLongSipProxyId, std::string & strBriefSipProxyid);

    static bool CheckSDPMediaIsVedio(const std::string & sdp, bool & bVedio);

    static bool GetIpFromSDPConnField(const std::string & sdp, std::string & ip);

    /**
    * @decode the sip msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool onReiceivedSipMsgFromSipProxy(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const std::map<std::string, std::string> & params);

    /**
    * @decode the INVITE response msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sip msg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealInviteResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe);

    /**
    * @decode the INVITE request msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealInviteRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params);

    /**
    * @decode the ACK responce msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealAckRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params);


    /**
    * @decode the BYE Request msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealBYERequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params);

    /**
    * @decode the BYE Responce msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealBYEResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe);

    /**
    * @decode the CANCEL Request msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealCANCELRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params);

    /**
    * @decode the CANCEL response msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealCANCELResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe);

    /**
    * @decode the UPDATE Request msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealUPDATERequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params);

    /**
    * @decode the UPDATE Request msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealUPDATEResponse(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe);

    /**
    * @decode the PRACK Request msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealPRACKRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params);

    /**
    * @decode the PRACK response msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealPRACKResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe);

    /**
    * @decode the INFO Request msg from SipProxy
    * param [in] strBuffer, the received sip msg.
    * param [in] iBufLen, the length of sipmsg.
    * @param [out] pstEnSipMsg.
    */
    static bool dealINFORequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
        const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params);

    /**
    * @be rpc call sip ,encode Request MESSAGE, for example rpc call.
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    //static bool SendRequestMESSAGE(const SipMessageGateway::SipMessagePtr & pMessage,const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy);

    /*
    * @be SIpProxy代替Client 发送 Message，并接收响应； SBC不会将REGISTER或者SUBSCRIBE请求发送给SIpProxy
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    //static bool dealMessageResponceFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);

    /*
    * @be 接收Message
    * param [in] reqMethod,
    * param [in] dlg,
    * @param [out] pstEnSipMsg.
    */
    //static bool dealMessageRequestFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe);

    //static bool sendMessageResponseToSBC(unsigned int rspCode, const SipMessageGateway::SipMessagePtr & pMessage,const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy);

    static SipGateway::SipMsgDecodeResultType onReceiveSipMsgFromSBC2(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pGateway, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection);

	static bool getCallerAndCalleeSipUri(std::string & caller, std::string & callee, const std::string & sipMsg);

    static bool sipUriValidCheck(const std::string & sipUri, std::string& userName, std::string& registarIp, int& port);
    //static SipMessageGateway::SipMessagePtr createSipMessage(const std::string caller, const std::string callee, const std::string msgBody, const std::string & strIp, int port, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy);
    static bool privateIpCheck(const std::string & remoteIp);

    static bool getPayloadName(int payload, std::string & plName);

    static bool readSdpParams(const std::string & strSdp, StSdpSessDesc & stSdp);

    static bool updateMediaDesc(StSdpSessDesc & stSdpNewSess, StSdpMediaDesc & sdpCodec);
    static bool updateSdpCodec(StSdpMediaDesc & sdpCodec, StSdpCodec & sdpCode);

    static bool createSdp(StSdpSessDesc &stSdp, std::string & ip, vector<int> & ports, std::string & strNewSDP);
    static bool createSdp(StSdpSessDesc &stSdp, std::string & ip, std::string & strNewSDP);

    static ZUCHAR sdpGetStaticEncoding(int ucPayload);
    static bool sdpGetEncoding(ZBOOL bIsAudio, ZUCHAR ucPayload, ZUCHAR *pucEncoding, int *piClockRate);

    static ZUCHAR sdpFindAfRtpmap(ST_SDP_MDESC *pstMdesc, ZUCHAR ucEncoding);
    static ZUCHAR sdpGetAfRtpmap(ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, int *piClockRate);
    static bool sdpAcGetAfFmtp(ST_SDP_MDESC *pstMdesc, StSdpCodec &pstCodec);
    static bool sdpDtmfSetAfFmtp(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec);
    static bool sdpAcSetAfFmtp(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec);

    static bool sdpAcReadSdp(ST_SDP_MDESC *pstMdesc, StSdpSessDesc & desc, StSdpMediaDesc & mediaDesc);
    static bool sdpRedReadSdp(ST_SDP_MDESC *pstMdesc, StSdpMediaDesc & des);
    static bool sdpDtmfReadSdp(ST_SDP_MDESC *pstMdesc, StSdpMediaDesc & desc);

    static bool sdpAcFillSdp(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec);
    static bool sdpRedFillSdp(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec);
    static bool sdpDtmfFillSdp(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec);

    static int getRedPayload(StSdpSessDesc & stSdpDesc);
    static void getAcPayload(StSdpSessDesc & stSdpDesc, vector<StSdpCodec> & codec);
    static bool getDtmfPayload(vector<StSdpCodec> &sdpCodec, StSdpCodec & sdpCode);
    static int getDtmfPayload(StSdpSessDesc & stSdpDesc, StSdpCodec & codec);

    static void negoSdp(StSdpSessDesc & stSdpDesc, StSdpSessDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp, StSdpCodec & codec);
    static void negoMrfpSdp(StSdpSessDesc & stSdpDesc, StSdpSessDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp, StSdpCodec & codec);
    static void negoMrfpAcSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp);
    static void negoAcSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp);
    static void negoRedSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp, int payload);

    static void negoSdp(StSdpSessDesc & stSdpDesc, StSdpSessDesc & stNegoSdp, vector<StSdpCodec> & codec);
    static void negoAcSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp, vector<StSdpCodec> & stCfgSdp);

    static bool sdpVcReadSdp(ST_SDP_MDESC *pstMdesc, StSdpSessDesc & desc, StSdpMediaDesc & mediaDesc);
    static bool sdpVcGetAfFmtp(ST_SDP_MDESC *pstMdesc, StSdpCodec &pstCodec);
};



#endif /* __SipMsg_h */
