#ifndef __SipMsgDecode_h
#define __SipMsgDecode_h

#include <string>
#include <memory>
#include "SipMsgCommon.h"
#include "SipMsgDecodedPart.h"
#include "tinyxml2.h"
#include "SipProxy/src/SipRegisterSubscribe.h"


class SipMsgDecode;
typedef std::shared_ptr<SipMsgDecode> SipMsgDecodePtr;

class SipMsgDecode
{
public:
    SipMsgDecode();
    virtual ~SipMsgDecode();

    /****************************** decode **********************************/
    /* decode */
    unsigned char decode(const std::string &strBuffer,int dwBufLen);

    bool encode();
    /* get sip version */
    int getVersion();

    /* judge the sip is request */
    bool isRequest();

    /* sip message get max forwards */
    bool getMaxForward(int &iMaxForward);

    /*
    get sip method
    if request, according to the request line
    if response, according to the CSeq header
    ucMethod type of "EN_SIP_METHOD"
    */
    bool getMethod(unsigned char &ucMethod);

    /* get min expires */
    bool getMinExpires(int &dwExpires);

    /**
    * @brief get request uri
    *
    * @param [out] strUserInfo
    * @param [out] strHostname
    * @param [out] dwPort
    *
    * @return type see EN_SIP_REQ_URI
    */
    unsigned char getReqUri(std::string &strUserInfo, std::string &strHostname, int &dwPort, int & reqUriType);

    /**
    * @brief get request uri.
    *
    * @param [out] ppstReqUri
    */
    bool getReqUriX(ST_SIP_REQ_URI **ppstReqUri);

    /* get response code */
    bool getRspStatusCode(unsigned int &iRspCode);

    /* get response code */
    bool getRspStatusCode2(int &iRspCode);

    /* get response reason */
    bool getRspReason(std::string &strRspReason);
    bool getRspReason(std::string &text, unsigned int &protoType, unsigned int &cause);

    /* get header From or To Header Tag */
    bool getHdrFromToTag(bool bFrom,std::string &strFromTag);

    /**
    * get header From uri
    * example: From: "13957854053"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>
    */
    bool getFromUri(std::string &strDispname, std::string &strUsername,
        std::string &strHostname, int &dwPort, unsigned char &ucReqUri);

    /**
    * @brief get header From uri.
    *
    * @param [out] ucUriType, see type of EN_SIP_ADDR_SPEC.
    * @param [out] pstUri, the address of uri.
    */
    bool getFromUriX(unsigned char &ucUriType, void **ppstUri);

    /**
    * @brief get header To sip uri or sips uri.
    *
    * @param [out] strDispName.
    * @param [out] strUserName.
    * @param [out] strHostName.
    * @param [out] iPort.
    */
    bool getHdrToSipUri(std::string &strDispName, std::string &strUserName,
        std::string &strHostName, int iPort);

    /**
    * @brief get header To tel uri.
    *
    * @param [out] strDispName.
    * @param [out] strNumber.
    */
    bool getHdrToTelUri(std::string &strDispName, std::string &strNumber);

    /**
    * @brief get From or To header info.
    *
    * @param [out] strDispName.
    * @param [out] strNumber.
    */
    bool getHdrFromToUri(bool bFrom,std::string &strDispname, std::string &strUsername,
        std::string &strHostname, int &dwPort, unsigned char &ucReqUri);



    /* get header Call-ID */
    bool getCallId(std::string &strNumber, std::string &strHost);

    /**
    * @brief get header Contact name-addr and addr-spec
    * @example: Contact: <sip:test.prdy194@192.168.1.84:36133;transport=TCP;ob>;
    *
    * @param [out] stNaSpec.
    */
    bool getContactNaSpec(ST_SIP_NA_SPEC &stNaSpec);

    /**
    * @brief get header Contact addr-spec
    * @example: Contact: <sip:test.prdy194@192.168.1.84:36133;transport=TCP;ob>;
    *
    * @param [out] stAddrSpec.
    */
    bool getContactAddrSpec(ST_SIP_ADDR_SPEC &stAddrSpec);

    /**
    * @brief get header Contact name-addr
    * @example: Contact: <sip:test.prdy194@192.168.1.84:36133;transport=TCP;ob>;
    *
    * @param [out] stNameAddr.
    */
    bool getContactNameAddr(ST_SIP_NAME_ADDR &stNameAddr);

    /* get code of header Cseq */
    bool getHdrCseq(int &dwCseq);

    /* get header Call-ID, return ST_SIP_CALLID */
    bool getHdrCallId(ST_SIP_CALLID & stCallId);

    /* get header Content-Length */
    bool getContentLength(int &iContentLen);

    /**
    * @brief get header WWW-Authenticate
    *
    * @param [out] ucAlgo, see type of EN_SIP_ALGO_VAL
    * @param [out] strRealm
    * @param [out] strNonce
    * @param [out] strOpaque
    * @param [out] ucQop, see type of EN_SIP_QOP_VAL
    * @param [out] strDomain
    *
    * @return type see EN_SIP_REQ_URI
    */
    bool getWwwAuthen(unsigned char &ucAlgo, std::string &strRealm,
        std::string &strNonce, std::string &strOpaque,
        unsigned char &ucQop, std::string &strDomain);

    /* get header expires */
    bool getExpires(int &dwExpires);

    /* get header WWW-Authenticate */
    bool getWwwDigest(ST_SIP_DIGEST &stDigest);

    //get Proxy-Authenticate Digest
    bool getProxyDigest(ST_SIP_DIGEST &stDigest);

    bool getDigest(ST_SIP_DIGEST &stDigest, unsigned long respCode);

    //get qop from 401 register
    bool getDigestQopX(ST_SIP_DIGEST *pstDigest, ZUCHAR *pucQopType);

    //get Digest realm
    bool getDigestRealm(  ST_SIP_DIGEST *pstDigest, std::string & realm);

    //get Digest nonce parameter
    bool getDigestNonce(  ST_SIP_DIGEST *pstDigest, std::string & nonce);

    //get Digest opaque parameter
    bool getDigestOpaque(  ST_SIP_DIGEST *pstDigest, std::string & opaque);

    //get Digest Algo
    bool getDigestAlgo(  ST_SIP_DIGEST *pstDigest, unsigned char & algoType);



    /*
    get extend header P-Associated-URI, see the example
    example: P-Associated-URI: <tel:0183087872>
    */
    bool getNumber(std::string &strNumber);

    /* get header user agent */
    bool getUserAgent(std::string &strName, std::string &strVer,
        std::string &strComm);

    /* get branch of header Via */
    bool getViaBranch(std::string &strBranch);

    /**
    * @brief SIP get the uri of via header
    *
    * @
    * @param [out] stSipHost uri of via header
    * @param [out] dwPort port of uri header
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrViaUri(std::string& stIpv4,  int & dwPort);

    /**
    * @brief get header Via other value except branch
    * @example: Via: SIP/2.0/TCP 192.168.8.101:53769;received=223.104.1.97;rport=12396;
    *
    * @param [out] ucTptType, @ref EN_SIP_TPT.
    * @param [out] strHost
    * @param [out] iPort
    * @param [out] strRecvHost
    * @param [out] iRport
    */
    bool getHdrViaOther(unsigned char &ucTptType, std::string &strHost,
        int &iPort,std::string &strRecvHost, int &iRport);

    bool getHdrVia(vector<ViaPart> & viaHeaders);
    bool getHdrViaParams(vector<ST_SIP_VIA_PARMS> & params);
    bool getHrdVia(ST_SIP_HDR_VIA & stVia);
    bool getHrdVias(vector<ST_SIP_HDR_VIA> & vias);


    /* get body */
    std::string getBody();

    /**
    * @brief get SDP audio description.
    * @example: m=audio 37076 RTP/AVP 114 116 103 105 0 8 18 104 97.
    *
    * @param [out] iPort.
    * @param [out] ucProtoType, @ref EN_SDP_PROTO_TYPE.
    * @param [out] iFmtVec.
    */
    bool getSdpAudioDesc(int &iPort, unsigned char &ucProtoType, vector<int> &iFmtVec);

    /**
    * @brief get SDP video description.
    * @example: m=video 37076 RTP/AVP 114 116 103 105 0 8 18 104 97.
    *
    * @param [out] iPort.
    * @param [out] ucProtoType, @ref EN_SDP_PROTO_TYPE.
    * @param [out] iFmtVec.
    */
    bool getSdpVideoDesc(int &iPort, unsigned char &ucProtoType, vector<int> &iFmtVec);

    /**
    * @brief get SDP origin description.
    * @example: o=+8613957854053 1023187643 1023187643 IN IP4 192.168.8.100.
    *
    * @param [out] strUsername.
    */
    bool getSdpOriginDesc(std::string &strUsername);

    /**
    * @brief SIP message get addr spec in From or To header.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
    * @param [out] ppstAddrSpec Addr-spec data strcture.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getFromToAddrSpec(ZBOOL bFromHdr, ST_SIP_ADDR_SPEC  & stAddrSpec);


    /**
    * @brief SIP get the uri of via header
    *
    * @
    * @param [in] bFrom : get user info from "From" or "To" HEader
    * @param [out] stUserInfo
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrFromToUserInfo(bool bFrom,ST_ZOS_SSTR & stUserInfo);

    bool getHdrFromToUserInfo(bool bFrom , std::string & strUserInfo);

    /**
    * @brief SIP get the uri of Contact header
    *
    * @
    * @param [out] strIpv4 uri(ip) of Contact header
    * @param [out] dwPort port of Contact header
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrContactUri(std::string& strIpv4,  int & dwPort);


    /**
    * @brief SIP get the SipUri of Contact header
    *
    * @
    * @param [out] stContactSipUri
    * @
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrContactSipUri(ST_SIP_SIP_URI & stContactSipUri, std::string & strNumber, EN_SIP_ADDR_SPEC & stAddrType);
    bool getHdrContactSipUriParams(vector<ST_SIP_URI_PARM> & params);
    bool getSipUriParams(const ST_SIP_SIP_URI & stSipUri, ST_SIP_URI_PARM_LST & params);

    /**
    * @brief SIP get the expires of Contact header
    *
    * @
    * @param [out] iExpires
    * @
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrContactExpires(int & iExpires);


    /**
    * @brief SIP get the sip uri info  of Contact header
    *
    * @
    * @param [out] iExpires
    * @
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrContactSipUri(std::string & strUserName, std::string & strHostName,int & iPort, ZCHAR & zcTpt);
    bool getHdrContactSipUri(std::string & strUserName, std::string & strHostName,int & iPort, ZCHAR & zcTpt, unsigned char & type);
    bool getHdrContactSipUri(ST_SIP_SIP_URI & stSipUri);

    bool getContactUri(std::string & strUri);

    /**
    * @brief SIP message get subscription state, state see EN_SIP_SUBSTA_VAL.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] pucStaType Subscribe status type, @ref EN_SIP_SUBSTA_VAL.
    * @param [out] piExpires Expires value.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrSubsSta(  ZUCHAR *pucStaType,std::string & strTypeName,ZUINT *piExpires);


    /**
    * @brief SIP message get event, pkgtype see EN_SIP_EVNT_PKG,
    *        temptype see EN_SIP_EVNT_TEMP.
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucPkgType Event package type, @ref EN_SIP_EVNT_PKG.
    * @param [out] pucTempType Event template type, @ref EN_SIP_EVNT_TEMP.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrEvent(ZUCHAR & pucPkgType,std::string & strPkgTypeName,ZUCHAR & pucTempType, std::string & strTmpTypeName);


    bool getEventId(std::string & eventId);

    /**
    * @brief SIP parameter pick up SIP-URI transport-param EN_SIP_TPT_PARM.
    *
    * @param [in] pstSipUri SIP URI.
    * @param [out] pucTpt Transport-param type, @ref EN_SIP_TPT_PARM.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool pickSipUriTpt(ST_SIP_SIP_URI *pstSipUri, ZUCHAR *pucTpt);


    /**
    * @brief SIP parameter  get the SipUri of Route or Record-Route Header
    *
    * @param [out] stSipUri SIP URI.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrRecordRouteParams(vector<ST_SIP_ROUTE_PARM> & stRouteParams);

    bool getHdrRecordRouteSipUri(bool bIsRecordRoute, ST_SIP_SIP_URI & stSipUri);

    bool getHdrRecordRouteSipUris(vector<ST_SIP_SIP_URI> & stSipUri);

    /**
    * @brief SIP parameter  get the SipUri of Route or Record-Route Header
    *
    * @param [out] stSipUri SIP URI.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrRecordRouteUri(bool bIsRecordRoute, std::string & strUserName, std::string & strHostName, int & dwPort);

    bool getHdrRecordRouteUri(bool bIsRecordRoute, std::string & strUserName, std::string & strHostName, int & dwPort,
        EN_SIP_URI_PARM & uriParamType, unsigned char & value1, unsigned int & value2, std::string & value3);

    bool fillHdrRecordRoute(ST_SIP_ROUTE_PARM & stRouteParams);
    bool fillHdrRecordRoute(const std::string &strUserName, const std::string &strHost, int iPort, unsigned char ucTptType);
    bool fillHdrRecordRouteBySipUri(const ST_SIP_SIP_URI & stSipUri);
    bool fillHdrRoute(ST_SIP_ROUTE_PARM & stRouteParams);
    bool fillHdrRoute(const std::string &strUserName, const std::string &strHost, int iPort, unsigned char ucTptType);

    bool getHdrRouteParams(vector<ST_SIP_ROUTE_PARM> & stRouteParams);
    bool getHdrRouteSipUris(vector<ST_SIP_SIP_URI> & stSipUri);
    bool delHdrRoute(const std::string & ip, const std::string & ip2, const std::string & ip3);
    /**
    * @brief SIP parameter  get Supported Header
    *
    * @param [out] vectType SIP URI.
    * @param [out] vectTypeName SIP URI.

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrSupported(vector<int> &vectType, vector<std::string> & vectTypeName);

    /**
    * @brief SIP parameter  get P-ASSERTED-ID Header
    *
    * @param [out] strDispInfo  telNum:TEL type, username SipSri, SipsUri type
    * @param [out] strHostName  is empty in TEL type

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrPAssertdedID(std::string & strDispInfo, std::string & strHostName, int & dwPort);
    bool getHdrPAssetedIDBySipUri(const ST_SIP_PASSERTEDID_VAL *pstAssetedVal, std::string & strUserInfo, std::string & strHostName, int & dwPort);
    bool getHdrPAssertedIDByTel(const ST_SIP_PASSERTEDID_VAL *pstAssetedVal, std::string & strTel);

    /**
    * @brief SIP parameter  get P-Called-Party-ID Header
    *
    * @param [out] strDispInfo  telNum:TEL type, username SipSri, SipsUri type
    * @param [out] strHostName  is empty in TEL type

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrPCalledPartyID(std::string & strDispInfo, std::string & strHostName, int & dwPort);

    bool getHdrPCalledPartyIDByTel(const ST_SIP_HDR_P_CALLED_PTY_ID *pstHdrPCalled, std::string & strTel);

    bool getHdrPCalledPartyIDBySipUri(const ST_SIP_HDR_P_CALLED_PTY_ID *pstHdrPCalled, std::string & strUserInfo, std::string & strHostName, int & dwPort);

    /**
    * @brief SIP parameter  get Conversation-ID Header
    *
    * @param [out] strConverstionId  telNum:TEL type, username SipSri, SipsUri type
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrConversationID(std::string & strConverstionId);

    /**
    * @brief SIP parameter  get Contribution-ID Header
    *
    * @param [out] strConverstionId  telNum:TEL type, username SipSri, SipsUri type
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrContributionID(std::string & strContributionId);

    /**
    * @brief SIP parameter  get Allow Header
    *
    * @param [out] vectType SIP URI.
    * @param [out] vectTypeName SIP URI.

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrAllow(vector<int> &vectType, vector<std::string> & vectTypeName);

    /**
    * @ decode NOTIFY request xml Body
    *
    * @param [out] vectType SIP URI.
    * @param [out] vectTypeName SIP URI.

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool decodeNotifyXmlBody( XmlReginfo & xmlReginfo);


    //get Xml Body
    bool getXmlBodyString(std::string & strXmlBody);

    //tinyXml2 decode Xml
    bool decodeNotifyXmlBodyTinyXml(const std::string & strXmlBody, XmlReginfo & xmlReginfo);


    int ReginfoStateConv(std::string stateIn);
    int RegistrationStateConv(std::string stateIn);
    int ContactStateConv(std::string stateIn);

    /**
    * @ decode 410/407 Challenge Digest request xml Body
    *
    * @param [out] vectType SIP URI.
    * @param [out] vectTypeName SIP URI.

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool decodeChallengeDigest( ST_SIP_DIGEST  stDigest, SipProxy::SipRegisterSubscribePtr pSipReg);

    //
    /**
    * @ dtry to get 200 Ok  REGISTER Authentication-Info: nextnonce
    *
    * @param [out] vectType SIP URI.
    * @param [out] vectTypeName SIP URI.

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getAuthenticationInfoNextNonce(std::string & nextNonce);


    /**
    * @brief SIP message get subscription reason, reason see EN_SIP_EVNT_REASON_VAL.
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getSubsReason(unsigned char & ucReasonType);


    //get ST_SIP_SIP_URI Headers
    bool getSipUriHeaders(ST_SIP_SIP_URI stSipUri, vector<ST_SIP_HDR> & vectHeaders);

    /**
    * @brief SIP
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getUriByAddrSpec(ST_SIP_ADDR_SPEC *pstAddrSpec, std::string & strUri);

    /**
    * @brief SIP
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getRseq(unsigned int & resNum);


    /**
    * @brief SIP  解析带SDP的SIP消息，获取媒体端口
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getSBCMediaIpAndPort(const std::string & sipMsgWithSDP, vector<SDPMediaInfo> & vectSdpMediaInfo);


    /**
    * @brief SIP  检查SIP消息中是否携带了SDP信息
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool checkSipMsgWithSDP(unsigned char ucMethod,bool isReq,  unsigned long statusCode, bool & bExistSDP,std::string & strSDP);

    /**
    * @brief SIP  获取SDP
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getSipMsgSDPBody(std::string & strSDP);

    /**
    * @brief SIP  decode Session-Expires Header
    *
    * @param [in] pstMsg SIP message.
    * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrSessionExpires(int & seconds, bool & refresherPre, bool & uasRefresh);

    bool getAllowOfMethod(unsigned char ucMethod);

    /**
    * @brief SIP  decode Content-Disposition Header
    *
    * @param [in] pstMsg SIP message.
    * @param [out] EN_SIP_DISP_TYPE_EXT type.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool getHdrContentDispEx(std::string & disp);

    //pick out sipUri params
    bool pickSipUriParams(ST_SIP_SIP_URI *pstSipUri, vector<ST_SIP_URI_PARM> & params);
    bool parmPickSipUriTpt(ST_SIP_SIP_URI *pstSipUri,unsigned char & ucTpt);
    bool parmPickSipUriUser(ST_SIP_SIP_URI *pstSipUri, unsigned char & ucUser);
    bool parmPickSipUriMethod(ST_SIP_SIP_URI *pstSipUri, unsigned char & ucMethod);
    bool parmPickSipUriSess(ST_SIP_SIP_URI *pstSipUri, unsigned char & ucSess);
    bool parmPickSipUriTtl(ST_SIP_SIP_URI *pstSipUri, unsigned int & ttl);
    bool copySipUri(ST_SIP_SIP_URI *pstDst, ST_SIP_SIP_URI *pstSrc);

    bool getHdrMinSe(ZUINT & second);
    bool getHdrExt(const std::string & name, std::string & val);

    bool getHdrHistoryInfo(std::string & name);

    bool fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int iPort,ZBOOL bRport,
        int iRport, const std::string &strBranch);

    bool fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
        int iRport, const std::string &strBranch, const std::string & received, int recvPort);
    /**
    * @fill header contact.
    *
    * @param [in] strUserName.
    * @param [in] strHost.
    * @param [in] iPort.
    * @param [in] ucTptType, @ref EN_SIP_TPT_PARM.
    */
    bool fillHdrContact(const std::string &strUserName,const std::string &strHost, int iPort, unsigned char ucTptType);
    bool fillHdrContactByAddrSpec(ST_SIP_ADDR_SPEC *pstAddrSpec, const std::string &strHost, int iPort);

    /**
    * @fill header From.
    * @example: From: "test.prdy194"<sip:test.prdy194@yes.my>;tag=3LB7vpZYexGzCDfnER8UZDCrftRB7ck7ataga30
    *
    * @param [in] strDispname
    * @param [in] strUserInfo
    * @param [in] strHostname
    * @param [in] iPort, if there is no port, set iPort = 0.
    * @param [in] strTag, if there is no tag, set strTag = "".
    */
    bool updateHdrFromToDisplayName(bool bFrom, std::string &strDispName);

    /**
    * @brief SIP message fill data body.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
    * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
    * @param [in] zData The Body data.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillBodyDataStr(ZUCHAR ucMtype, ZUCHAR ucMSubType, ST_ZOS_SSTR *pstStr);

    //get P-Preferred-Identity
    bool getHdrPPreferId(std::string & displayName, std::string & strUserInfo, std::string & strHostName, int & iPort);

    //delete P-Preferred-Identity
    bool delHdrPPreferId();

    //fill P-Asserted-Identity
    bool fillHdrPAssertedId(std::string & displayName, const std::string & userName, const std::string  & host, const int & port);
    bool fillHdrPAssertedIdByTel(const std::string & userName);

    bool ParmFillTelUri(ST_SIP_TEL_URI *pstTelUri, const std::string & userName);
    bool ParmFillSipUri(ST_SIP_SIP_URI *pstSipUri, const std::string & userName, const std::string & strHostname, int iPort);

    bool fillHdrContact2(const std::string &strUserName,const std::string &strHost, int iPort, unsigned char ucTptType);

    bool fillHdrVia2(unsigned char ucTptType, const std::string &strHostname, int dwPort, ZBOOL bRport,
        int iRport, const std::string &strBranch);

    bool fillHdrVia2(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
        int iRport, const std::string &strBranch, const std::string & received, int recvPort);
    bool fillHdrVia3(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
        int iRport, const std::string &strBranch, const std::string & received, int recvPort,
        const vector<ST_SIP_VIA_PARMS> & prams);

    bool fillHdrViaExtParams(const vector<ST_SIP_VIA_PARMS> & prams);

    bool getHdrById(int ucType);

    bool delHdrById(int ucType);

    bool delHdrVia(const std::string &ip, const std::string &ip2, const std::string &ip3);

    /**
    * @fill header From.
    * @example: From: "test.prdy194"<sip:test.prdy194@yes.my>;tag=3LB7vpZYexGzCDfnER8UZDCrftRB7ck7ataga30
    *
    * @param [in] strDispname
    * @param [in] strUserInfo
    * @param [in] strHostname
    * @param [in] iPort, if there is no port, set iPort = 0.
    * @param [in] strTag, if there is no tag, set strTag = "".
    */
    bool fillHdrFromToByName(bool bFrom, const std::string &strDispname,const std::string &strUserInfo,
        const std::string &strHostname, int iPort, const std::string &strTag);

    /**
    * @brief SIP fill from/to header by SIPs hostname.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
    * @param [in] pstDispName Display name string.
    * @param [in] pstUserInfo User information string.
    * @param [in] pstHostname Host name string.
    * @param [in] iPort Port value.
    * @param [in] pstTag Tag string of From or To header.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool  fillHdrFromToBySipsName(bool bFromHdr, const std::string & strDispName, const std::string & strUserInfo, const std::string & strHostname,
        int iPort, const std::string & strTag);

    /**
    * @fill header To tel uri.
    * @example: To: "8616000000330"<tel:+8616000000330>.
    *
    * @param [in] strDispName
    * @param [in] strNumber
    */
    bool fillHdrFromToTelUri(bool bFromHdr,const std::string &strDispName, const std::string &strNumber,
        const std::string &strTag);

    /**
    * @fill header From.
    * @example: From: "test.prdy194"<sip:test.prdy194@yes.my>;tag=3LB7vpZYexGzCDfnER8UZDCrftRB7ck7ataga30
    *
    * @param [in] strDispname
    * @param [in] strUserInfo
    * @param [in] strHostname
    * @param [in] iPort, if there is no port, set iPort = 0.
    * @param [in] strTag, if there is no tag, set strTag = "".
    * @param [in] ucReqUri,  addr-spec type EN_SIP_ADDR_SPEC
    */
    bool fillHdrFromToByName(bool bFrom, const std::string &strDispname,const std::string &strUserInfo,
        const std::string &strHostname, int iPort, const std::string &strTag, unsigned char &ucReqUriType);

    /**
    * @fill request line.
    * @example: REGISTER sip:yes.my:5060 SIP/2.0
    *
    * @param [in] ucUriType, see type of EN_SIP_ADDR_SPEC_...
    * @param [in] ucMethod, see type of EN_SIP_METHOD.
    * @param [in] strUserInfo, if there is no userinfor, set strUserInfo = "".
    * @param [in] strHost.
    * @param [in] iPort.
    */
    bool updateReqLine(unsigned char ucUriType, unsigned char ucMethod,
        const std::string &strUserInfo, const std::string &strHost,
        int iPort);

    /**
    * @fill request line.
    * @example: REGISTER sip:yes.my:5060 SIP/2.0
    *
    * @param [in] ucMethod, see type of EN_SIP_METHOD.
    * @param [in] strUserInfo, if there is no userinfor, set strUserInfo = "".
    * @param [in] strHostname.
    * @param [in] iPort.
    */
    bool fillReqLineBySip(unsigned char ucMethod,const std::string &strUserInfo,
        const std::string &strHostname,int iPort);
    /**
    * @fill request sips line.
    * @example: REGISTER sips:yes.my:5060 SIP/2.0
    *
    * @param [in] ucMethod, see type of EN_SIP_METHOD.
    * @param [in] strUserInfo.
    * @param [in] strHostname.
    * @param [in] iPort.
    */
    bool fillReqLineBySips(unsigned char ucMethod,const std::string &strUserInfo,
        const std::string &strHostname,int iPort);

    /**
    * @fill request sips line.
    * @example: INVITE tel:+8612312341234 SIP/2.0
    *
    * @param [in] ucMethod, see type of EN_SIP_METHOD.
    * @param [in] strNumber.
    */
    bool fillReqLineByTel(unsigned char ucMethod, const std::string &strNumber);

    bool fillHdrExt(const std::string & name, const std::string & val);
    ZVOID * Sip_CreateMsgHdr2(ST_SIP_MSG *pstMsg, ZUCHAR ucType);
    ZVOID * Sip_HdrLstCreateHdr2(ZUBUF zBufId, ST_SIP_MSG_HDR_LST *pstHdrLst, ZUCHAR ucType);

    bool authDRspFillNonce(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strNonce);
    bool authDRspFillUsername(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strUsername);
    bool authDRspFillUri(ST_SIP_DIGEST_RSP *pstRsp, ST_SIP_REQ_URI *pstReqUri);
    bool authDRspFillRsp(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strResponse);
    bool authDRspFillCnonce(ST_SIP_DIGEST_RSP *pstRsp, ST_ZOS_SSTR *pstCnonce);
    bool authDRspFillNonceCount(ST_SIP_DIGEST_RSP *pstRsp, ST_ZOS_SSTR *pstCount);
    bool authDRspFillRealm(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strRealm);
    bool authDRspFillAlgo(ST_SIP_DIGEST_RSP *pstRsp, unsigned char algoType);
    bool authDRspFillQop(ST_SIP_DIGEST_RSP *pstRsp, unsigned char qopType);
    bool authDRspFillQpaque(ST_SIP_DIGEST_RSP *pstRsp, const std::string & opaque);

    bool fillHdrCseq(unsigned char ucMethod, int iCseq);
    bool fillHdrAuthor(const ST_SIP_DIGEST_RSP & stDigestResp, int rspCode);
    bool fillHdrAuthor(const ST_SIP_DIGEST &stDigest, const std::string & strUserName,const std::string & strPassWd,
        const unsigned char & ucMethod, const ST_SIP_REQ_URI & stReqUri);

    bool fillHdrContentType(unsigned char ucMtype, const std::string &strMtypeExt,
        unsigned char ucMSubtype, const std::string &strMSubtypeExt);

private:
    /* get userinfo from ST_SIP_REQ_URI */
    std::string getUserInfoFromUri(ST_SIP_REQ_URI stReqUri);

    /* get hostname from ST_SIP_REQ_URI */
    std::string getHostnameFromUri(ST_SIP_REQ_URI stReqUri);

    /* get port from ST_SIP_REQ_URI */
    int getPortFromUri(ST_SIP_REQ_URI stReqUri);

public:
    ST_SIP_MSG                      *_pstMsg;
    std::string                     _strMsg;
    int                             _iMsgLen;
};

#endif /* __SipMsgDecode_h */
