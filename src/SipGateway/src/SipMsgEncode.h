#ifndef __SipMsgEncode_h
#define __SipMsgEncode_h

#include <string>
#include <memory>
#include "SipMsgCommon.h"
#include "avatar/avatar.h"


class SipMsgEncode;
typedef std::shared_ptr<SipMsgEncode> SipMsgEncodePtr;

class SipMsgEncode
{
public:
    SipMsgEncode();
    virtual ~SipMsgEncode();

    /* create sip request */
    bool createReq();

    /* create sip response */
    bool createRsp();

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
    bool fillReqLine(unsigned char ucUriType, unsigned char ucMethod,
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

    bool fillReqLineByUri( ZUCHAR ucMethod,const std::string & strUri) ;
    bool fillReqLineSipUriParasCxtId(const vector<ST_SIP_URI_PARM> & params, std::string & reason);
    bool fillReqLineSipUriParasCxtId(const map<std::string, std::string> params, std::string & reason);


    /**
    * @fill request line.
    * @example: REGISTER sip:yes.my:5060 SIP/2.0
    *
    * @param [in] ucMethod, see type of EN_SIP_METHOD
    * @param [in] ucUriType, see type of EN_SIP_ADDR_SPEC
    * @param [in] pstUri.
    */
    bool fillReqLineBySipX(unsigned char ucMethod, unsigned char ucUriType, void *pstUri);

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

    /**
    * @fill response status line.
    * @example: SIP/2.0 100 Trying.
    *
    * @param [in] iStatusCode.
    */
    bool fillStatusLine(int iStatusCode);

    /**
    * @brief SIP fill request line by IP address, method see EN_SIP_METHOD.
    *
    *
    * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
    * @param [in] pstUserInfo User information string.
    * @param [in] strDotIpAddr IP address, dot string ,example 192.168.102.65.
    * @param [in] iPort
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillReqLineByIp( ZUCHAR ucMethod, 	ST_ZOS_SSTR *pstUserInfo, const std::string & strDotIpAddr, const int & iPort);



    /**
    * @brief SIP fill request line by SIP uri, method see EN_SIP_METHOD.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
    * @param [in] pstSipUri SIP URI.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillReqLineBySipUri(ZUCHAR ucMethod, ST_SIP_SIP_URI *pstSipUri);



    /**
    * @fill header Via.
    * @example: Via: SIP/2.0/TCP 192.168.1.84:50987;rport;branch=z9hG4bKPjHc3UnWSp1ExQbzpocqv43L5CbpfVAn7X
    *
    * @param [in] ucTptType, @ref EN_SIP_TPT.
    * @param [in] strHostname
    * @param [in] iPort
    * @param [in] iRport, if iRport == 0, the value of rport is empty.
    * @param [in] strBranch
    */
    bool fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int iPort,ZBOOL bRport,
        int iRport, const std::string &strBranch);

    bool fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int iPort, ZBOOL bRport,
        int iRport, const std::string &strBranch, const std::string & received, int recvPort);
    bool fillHdrViaExtParams(const vector<ST_SIP_VIA_PARMS> & prams);
    bool fillHdrVia(const ST_SIP_HDR_VIA & stVia);
    bool fillHdrVias(const vector<ST_SIP_HDR_VIA> & stVias);
    bool fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
        int iRport, const std::string &strBranch, const std::string & received, int recvPort,
        const vector<ST_SIP_VIA_PARMS> & prams);

    /**
    * @fill header Supported.
    * @example: Supported: path,gruu.
    *
    * @param [in] iSuptFlags, vector element see type of SIP_TAG_OPT_ADD_100REL.
    */
    bool fillHdrSupted(vector<int> iSuptFlag);

    /*
    fill Route header
    example: Route: <sip:proxy11.yes.my;transport=tcp;lr>
    */
    bool fillHdrRoute(ST_SIP_ROUTE_PARM & stRouteParams);

    bool fillHdrRoute(const std::string &strUserName, const std::string &strHost,
        int iPort, unsigned char ucTptType, std::string & reason);

    bool fillHdrRoute(const std::string &strUserName, const std::string &strHost, int iPort, unsigned char ucTptType);

    bool fillHdrRouteBySipUri(const vector<ST_SIP_SIP_URI> & stSipUri, bool bSipCallRpc = false);

    /**
    * @fill header Record-Route.
    * @example: Record-Route: <sip:DIAG_13_0_0d00002c@117.136.239.32:5460;transport=TCP;lr>.
    *
    * @param [in] strUserName.
    * @param [in] strHost.
    * @param [in] iPort.
    * @param [in] ucTptType, @ref EN_SIP_TPT_PARM_TYPE.
    */
    bool fillHdrRecordRoute(const std::string &strUserName, const std::string &strHost,
        int iPort, unsigned char ucTptType);
    bool fillHdrRecordRoute(ST_SIP_ROUTE_PARM & stRouteParams);

    bool fillHdrRecordRouteBySipUri(const ST_SIP_SIP_URI & stSipUri);
    /*
    fill Forwards header
    example: Max-Forwards: 70
    */
    bool fillHdrForwards(int iMaxForwards);

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
    * @fill header call-id
    *
    * @param [in] strCallNumber.
    * @param [in] strCallHost, if there is no call host, set strCallHost = "".
    */
    bool fillHdrCallId(const std::string &strCallNumber, const std::string &strCallHost);


    /**
    * @brief SIP message fill Call-ID header.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] pstCallId Call-ID data structure.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrCallId( ST_SIP_CALLID *pstCallId);


    /**
    * @fill header cseq
    *
    * @param [in] ucMethod, see type EN_SIP_METHOD.
    * @param [in] iCseq.
    */
    bool fillHdrCseq(unsigned char ucMethod, int iCseq);

    /**
    * @fill header Authorization
    *
    * @param [in] stAuth.
    */
    bool fillHdrAuthor(StSipAuthor stAuth);


    /**
    * @fill header Authorization, after 401 message
    *
    * @param [in] stDigest digest send by sevrer (401 message)
    * @param [in] strUserName
    * @param [in] strPassWd
    * @param [in] ucMethod
    * @param [in] stReqUri
    */
    bool fillHdrAuthor(const ST_SIP_DIGEST &stDigest, const std::string & strUserName,const std::string & strPassWd,
        const unsigned char & ucMethod, const ST_SIP_REQ_URI & stReqUri);


    /**
    * @fill header Authorization
    *
    * @param [in] stDigestResp
    */
    bool fillHdrAuthor(const ST_SIP_DIGEST_RSP & stDigestResp, int rspCode);


    /**
    * @fill header contact.
    *
    * @param [in] strUserName.
    * @param [in] strHost.
    * @param [in] iPort.
    * @param [in] ucTptType, @ref EN_SIP_TPT_PARM.
    */
    bool fillHdrContact(const std::string &strUserName,const std::string &strHost, int iPort, unsigned char ucTptType);

    /**
    * @fill header contact to sip msg
    *
    * @param [in] stAddrSpec, @ref ST_SIP_ADDR_SPEC.
    * @param [in] strHost.
    * @param [in] iPort.
    */
    bool fillHdrContactByAddrSpec(ST_SIP_ADDR_SPEC *pstAddrSpec, const std::string &strHost,
        int iPort);

    /**
    * @fill header contact by tel
    *
    * @param [in] stAddrSpec, @ref ST_SIP_ADDR_SPEC.
    * @param [in] strHost.
    * @param [in] iPort.
    */
    bool fillHdrContactByTel(std::string strDispName, std::string strNumber);


    /**
    * @fill contact uri param transport.
    *
    * @param [in] zMemBuf.
    * @param [in] pstAddrSpec, @ref ST_SIP_ADDR_SPEC.
    * @param [in] ucTptType, @ref EN_SIP_TPT_PARM.
    */
    bool parmFillAddrSpecTpt(void *zMemBuf, ST_SIP_ADDR_SPEC *pstAddrSpec, unsigned char ucTptType);

    /**
    * @fill contact uri param transport.
    *
    * @param [in] zMemBuf.
    * @param [in] pstAddrSpec, @ref ST_SIP_ADDR_SPEC.
    * @param [in] ucTptType, @ref EN_SIP_TPT_PARM.
    */
    bool parmFillSipUriTpt(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, unsigned char ucTptType);
    bool parmFillSipUriLr(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri);
    bool parmFillSipUriOthers(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, const ST_ZOS_SSTR & name, const ST_ZOS_SSTR & zosValue);
    bool parmFillSipUriOthers(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, const std::string & strName, const std::string & strValue);

    /**
    * @fill contact uri param transport.
    *
    * @param [in] zMemBuf.
    * @param [in] pstAddrSpec, @ref ST_SIP_ADDR_SPEC.
    * @param [in] ucTptType, @ref EN_SIP_TPT_PARM.
    */
    bool parmFillSipUriUSer(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, unsigned char ucUser);


    bool ParmFillSipUri(ST_SIP_SIP_URI *pstSipUri, const std::string & userName, const std::string &  strHostname, int iPort);
    bool ParmFillTelUri(ST_SIP_TEL_URI *pstTelUri, const std::string & userName);

    bool parmFillSipUriOthersEx(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, const std::map<std::string, std::string> & params);


    /**
    * @add param to header contact
    *
    * @param [in] iCapOpt, @ref CT_CAP_IM, if there is more than one cap, use "|".
    */
    bool parmFillContact(int iCapOpt);

    /**
    * @add param expires to header contact
    *
    * @param [in] iExpires.
    */
    bool parmFillContactExpires(int iExpires);

    /*
    fill User-Agent header
    example: User-Agent: Yes Life/1.2.86 (Android 4.4.2)
    */
    bool fillHdrUserAgent(const std::string &strName, const std::string strVersion, const std::string &strComm);

    /**
    * @fill header server.
    * @example: Server: CPM-client/OMA1.0 MSP/1.0.
    *
    * @param [in] strName.
    * @param [in] strVer.
    * @param [in] strComm.
    */
    bool fillHdrServer(const std::string &strName, const std::string &strVer, const std::string &strComm);

    /**
    * @brief fill Event header.
    *
    * @param [in] ucPkgType Event pacakge type, @ref EN_SIP_EVNT_PKG.
    * @param [in] ucTempType Event template type, @ref EN_SIP_EVNT_TEMP.
    */
    bool fillHdrEvnt(unsigned char ucPkgType, unsigned char ucTempType);

    /**
    * @brief fill SUBSCRIBE Event header id parameter.
    *
    * @param [in] ucPkgType Event pacakge type, @ref EN_SIP_EVNT_PKG.
    * @param [in] ucTempType Event template type, @ref EN_SIP_EVNT_TEMP.
    */
    bool fillHdrEvntId(std::string evntId);


    /**
    * @brief fill Expires header.
    *
    * @param [in] iExpires.
    */
    bool fillHdrExpires(int iExpires);

    /**
    * @brief fill header Session-Expires.
    * @example: Session-Expires: 1800;refresher=uas
    *
    * @param [in] bIsRefresher.
    * @param [in] bIsUasRefresher.
    * @param [in] iExpires.
    */
    bool fillHdrSessExpires(bool bIsRefresher, bool bIsUasRefresher, int iExpires);

    /**
    * @brief fill header Privacy.
    * @example: Privacy: id
    *
    * @param [in] ucPrivVal, @ref EN_SIP_PRIV_VAL.
    */
    bool fillHdrPrivacy(unsigned char ucPrivVal);

    /**
    * @brief fill header Content-Disposition.
    * @example: Content-Disposition: session.
    *
    * @param [in] strContentDisp.
    */
    bool fillHdrContentDisp(const std::string &strContentDisp);

    /**
    * @fill header Allow
    *
    * @param [in] viSuptFlag, vector element see type of SIP_ALLOW_HDR_ADD_INVITE.
    */
    bool fillHdrAllow(vector<int> viSuptFlag);

    /**
    * @brief SIP message fill Accept Header, mtype see EN_SIP_MTYPE
    *
    * @param [in] ucMType Value of m-type, @ref EN_SIP_MTYPE.
    * @param [in] ucSType Value of sub-type, @ref EN_SIP_MSUBTYPE.
    */
    bool fillHdrAcpt(unsigned char ucMType, unsigned char ucSType);

    /**
    * @brief SIP message fill Accept-Encoding Header
    *
    * @param [in] ucMType Value of m-type, @ref EN_SIP_MTYPE.
    * @param [in] ucSType Value of sub-type, @ref EN_SIP_MSUBTYPE.
    */
    bool fillHdrAcptEncoding(const vector<std::string> & encodings);

    /**
    * @brief SIP message fill Accept-Language Header
    *
    * @param [in] ucMType Value of m-type, @ref EN_SIP_MTYPE.
    * @param [in] ucSType Value of sub-type, @ref EN_SIP_MSUBTYPE.
    */
    bool fillHdrAcptLang(const vector<std::string> & languages);


    /**
    * @brief SIP message fill content-type Header.
    *
    * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
    * @param [in] strMtypeExt Extension m-type.
    * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
    * @param [in] strMSubtypeExt Extension sub-type.
    */
    bool fillHdrContentType(unsigned char ucMtype, const std::string &strMtypeExt,
        unsigned char ucMSubtype, const std::string &strMSubtypeExt);


    /**
    * @brief SIP message fill Content-Type header,
    *        media type see EN_SIP_MTYPE, sub type see EN_SIP_MSUBTYPE.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
    * @param [in] pstMtypeExt Extension m-type.
    * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
    * @param [in] pstMSubtypeExt Extension sub-type.
    * @param [in] pcBoundary Boundary string.
    * @param [in] wLen Boundary string length.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */ //  ZOS_NSTR(SIP_MSG_MPART_BOUNDARY)
    bool fillHdrContentTypeX(ZUCHAR ucMtype, const std::string &strMtypeExt, ZUCHAR ucMSubtype, const std::string &strMSubtypeExt, ZCHAR *pcBoundary, ZUSHORT wLen);




    /**
    * @brief SIP message fill SDP body
    *
    * @param [in] stSessDesc.
    */
    bool fillBodySdp(StSdpSessDesc stSessDesc);

    /**
    * @brief create a new sdp.
    *
    * @param [out] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    */
    bool createSdpMsg(ST_SDP_SESS_DESC **ppstSdpMsg);

    /**
    * @brief set version of sdp.
    *
    * @param [in] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    * @param [in] iVersion.
    */
    bool fillSdpVersion(ST_SDP_SESS_DESC *pstSdpMsg, int iVersion);

    /**
    * @brief set origin field of sdp.
    *
    * @param [in] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    * @param [in] pstOrigin, @ref StSdpOrigin.
    */
    bool fillSdpOrigin(ST_SDP_SESS_DESC *pstSdpMsg, StSdpOrigin *pstOrigin);

    /**
    * @brief set session name of sdp.
    *
    * @param [in] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    * @param [in] strSessName.
    */
    bool fillSdpSessName(ST_SDP_SESS_DESC *pstSdpMsg, const std::string &strSessName);


    /**
    * @brief set connection of sdp.
    *
    * @param [in] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    * @param [in] stConnectionIngo.
    */
    bool fillSdpSessConnection(ST_SDP_SESS_DESC *pstSdpMsg, const StSdpConnInfo &stConnectionIngo);



    /**
    * @brief add session time description to sdp.
    *
    * @param [in] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    * @param [in] pstTimeField, @ref StSdpTimeField.
    */
    bool fillSdpSessTime(ST_SDP_SESS_DESC *pstSdpMsg, StSdpTimeField *pstTimeField);

    /**
    * @brief add session attribution description to sdp.
    *
    * @param [in] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    * @param [in] ucType, @ref EN_SDP_ATTR_TYPE.
    */
    bool fillSdpSessAttr(ST_SDP_SESS_DESC *pstSdpMsg, unsigned char ucType);

    /**
    * @brief add media description to sdp.
    *
    * @param [in] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    * @param [in] pstMediaDesc, @ref StSdpMediaDesc.
    */
    bool fillSdpMediaDesc(ST_SDP_SESS_DESC *pstSdpMsg, StSdpMediaDesc *pstMediaDesc);

    bool fillSdpInfoFild(ST_SDP_SESS_DESC *pstSdpMsg, const std::string &strSessName);


    /**
    * @brief set media format.
    *
    * @param [in] pstMf, @ref ST_SDP_MF.
    * @param [in] ucPayload, @ref EN_RTP_PAYLOAD_TYPE.
    */
    bool parmFillSdpMediaFmt(ST_SDP_MF *pstMf, unsigned char ucPayload);

    bool parmFillSdpMediaRtpmap(ST_SDP_MDESC *pstMdesc, unsigned char ucPayload,
        int iClockRate, unsigned char ucEncodingType);

    bool parmFillSdpMediaRtcp(ST_SDP_MDESC *pstMdesc, int iPort);

    bool parmFillSdpMediaRtcpMux(ST_SDP_MDESC *pstMdesc, bool bRtcpMux);

    bool parmFillSdpMediaRtcpFb(ST_SDP_MDESC *pstMdesc, unsigned char ucPayload,
        unsigned char ucIdType, unsigned char ucParm);

    /**
    * @brief set attribute-field direct.
    *
    * @param [in] pstMdesc, @ref ST_SDP_MDESC.
    * @param [in] ucDirect, @ref EN_SDP_DIRECT_TYPE_TYPE.
    */
    bool parmFillSdpMediaDirect(ST_SDP_MDESC *pstMdesc, unsigned char ucDirect);

    /**
    * @brief set media attribute-field ptime.
    *
    * @param [in] pstMdesc, @ref ST_SDP_MDESC.
    * @param [in] iPTime.
    */
    bool parmFillSdpMediaPTime(ST_SDP_MDESC *pstMdesc, int iPTime);

    /**
    * @brief set media attribute-field setup.
    *
    * @param [in] pstMdesc, @ref ST_SDP_MDESC.
    * @param [in] ucType, @ref EN_SDP_SETUP_TYPE.
    */
    bool parmFillSdpMediaSetup(ST_SDP_MDESC *pstMdesc, unsigned char ucType);

    /* encode _pstMsg to std::string */
    bool encode();

    /************************************* static *****************************************/
    /* fill param nonce to ST_SIP_DIGEST_SRP */
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


    //xjp add ===============================================================

    bool fillHdrFromToByUri(ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
        ST_ZOS_SSTR *pstUri, ST_ZOS_SSTR *pstTag);



    bool  FillHdrContact(ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_SIP_URI *pstSipUri);

    /**
    * @brief SIP fill Contact header by SIP-URI.
    *
    * @param [in] stSipUri SIP message.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrContactBySipUri(const ST_SIP_SIP_URI & stSipUri);

    /**
    * @brief SIP fill from/to header by SIP URI.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
    * @param [in] pstDispName Display name string.
    * @param [in] pstSipUri SIP URI.
    * @param [in] pstTag Tag string of From or To header.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrFromOrToBySipUri( ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,	ST_SIP_SIP_URI *pstSipUri, ST_ZOS_SSTR *pstTag);


    /**
    * @brief SIP parameter fill contact expires.
    *
    * @param [in] zBufId Buffer request memory from.
    * @param [in] pstParm Parameter of contact.
    * @param [in] iExpires Expires value.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool ParmFillContactExpires( ZUINT iExpires);


    ///* mgf fill cpim message with imdn request */
    //bool fillCpimMsgImdnX( ST_SIP_SIP_URI *pstLclUri,  ST_SIP_SIP_URI *pstPartpUri, ZCHAR *pcBody, ZUINT iBoydSize,
    // ST_ZOS_SYS_TIME *pstTime, ZUINT iImdnType, ZCHAR *pcCType,
    // ZCHAR **ppcMsg, ZUINT *piMsgSize, ST_ZOS_SSTR *pstIMsgId);

    /**
    * @brief SIP fill header P-Preferred-Identity header.
    *
    * @param [in] strDispName  dispaly name
    *
    *
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrPPreferredIdentity(const std::string & strDispName);

    /**
    * @brief SIP fill header P-Preferred-Identity header.
    *
    * @param [in] strDispName  dispaly name
    *
    *
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrPPreferredIdentity(const std::string & strDispName,const std::string & userName, const std::string  & host, const int & port);

    bool fillHdrPAssertedIdentity(const std::string & strDispName,const std::string & userName, const std::string  & host, const int & port);

    bool fillHdrPAssertedIdByTel(const std::string & userName);

    /**
    * @brief SIP fill header Accept-Contact header.
    *
    * @param [in] paramFlags  Accept-contact params,.without gen value  EN_SIP_AC_PARMS_
    *
    * @param [in]  mapGenParas , gen params
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrAcceptContact(vector<int> paramFlags, map<std::string,std::string> mapGenParas);

    /**
    * @brief SIP fill header P-Preferred-Identity header param, cann't set the gen value.
    * @param [in]  zBufId Buffer request memory from.
    * @param [in]  pstAcVal Ac-value of +g.oma.SIP-im.large-message.
    * @param [in]  ucType  EN_SIP_AC_PARMS type,find  it for details
    *
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool paramFillAcceptContactParam(ZUBUF zBufId, ST_SIP_AC_VAL *pstAcVal, ZUCHAR ucType);


    /**
    * @brief SIP message fill P-Early-Media header header.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] iParmType Early media type, @ref EN_SIP_P_EARLY_MEDIA_PARM.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrPEarlyMedia( ZUINT iParmType);


    /**
    * @brief SIP message fill Subscription-State header.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] ucState Subscription status type, @ref EN_SIP_SUBSTA_VAL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrSubsSta( ZUCHAR ucState);

    /**
    * @brief SIP message fill Subscription-State header.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] ucState Subscription status type, @ref EN_SIP_SUBSTA_VAL.
    * @param [in] ucReason Subscription reason type, @ref EN_SIP_EVNT_REASON_VAL.
    * @param [in] iExpires Subscription expire value.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrSubsStaY( ZUCHAR ucState,  ZUCHAR ucReason, ZUINT iExpires);

    /**
    * @brief SIP message fill Subscription-State header.
    * @param [in] iExpires Subscription expire value.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool HdrSubsStaAddExpires( ZUINT iExpires);

    /**
    * @brief SIP message fill Content-Disposition header.
    *
    *
    * @param [in] pcDisp The content disposition string.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrContentDisp(ZCHAR *pcDisp);

    /**
    * @brief SIP Accept header add method.
    *
    * @param [in] zBufId Buffer request memory from.
    * @param [in] pstAcpt Accept header data structure.
    * @param [in] ucMType Value of m-type, @ref EN_SIP_MTYPE.
    * @param [in] ucSType Value of sub-type, @ref EN_SIP_MSUBTYPE.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool HdrAcptAddRange( ZUCHAR ucMType, ZUCHAR ucSType);


    /**
    * @brief  sdp message set attribute-field of amr fmtp, EN_SDP_ENCODING_AMR  encoding
    *
    * @param [in] pstMdesc
    * @param [in] ucPayload
    * @param [in] ucModeSet  Refer to EN_SDP_ATTR_FMTP_MODE_SET_0 and multi parameters should be | operation
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool SdpMsgSetAfFmtpAmr( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, ZUCHAR ucModeSet);


    /**
    * @brief  sdp message set attribute-field of amr fmtp, EN_SDP_ENCODING_AMR_WB  encoding
    *
    * @param [in] pstMdesc
    * @param [in] ucPayload
    * @param [in] ucModeSet  Refer to EN_SDP_ATTR_FMTP_MODE_SET_0 and multi parameters should be | operation
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool  SdpMsgSetAfFmtpAmrWb( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, ZUCHAR ucModeSet, ZUCHAR ucEncoding);


    /**
    * @brief  sdp message set attribute-field of amr fmtp, EN_SDP_ENCODING_ILBC  encoding
    *
    * @param [in] pstMdesc
    * @param [in] ucPayload
    * @param [in] ucMode  Refer to EN_SDP_ILBC_TYPE
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool SdpMsgSetAfFmtpILBC( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, ZUCHAR ucMode);

    /**
    * @brief  sdp message set attribute-field of amr fmtp, EN_SDP_ENCODING_RED  encoding
    *
    * @param [in] pstMdesc
    * @param [in] ucPayload
    * @param [in] ucMode  Refer to EN_SDP_ILBC_TYPE
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool SdpMsgSetAfFmtpRed( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload,  char aucPtLst[13] );


    /* sdp message exist media-descriptions, type see EN_SDP_MEDIA_TYPE */
    bool SdpMsgExistMdesc(ST_SDP_SESS_DESC *pstMsg, ZUCHAR ucMediaType, ST_SDP_MDESC **ppstMdesc);



    /**
    * @brief  sdp message set attribute-field of amr fmtp, EN_SDP_ATTR_PTIME
    * @param [in] pstMdesc
    * @param [in] iPTime
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool SdpMsgSetAfPTime( ST_SDP_MDESC *pstMdesc, ZULONG iPTime);

    /**
    * @brief  sdp message set attribute-field of amr fmtp, EN_SDP_ATTR_MAXPTIME
    * @param [in] pstMdesc
    * @param [in] iPTime
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool SdpMsgSetAfMaxPTime( ST_SDP_MDESC *pstMdesc, ZULONG iMaxPTime);


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
    bool fillBodyData( ZUCHAR ucMtype,ZUCHAR ucMSubtype, ZDBUF zMsgBuf );


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
    bool fillBodySDPDataStr(ZUCHAR ucMtype, ZUCHAR ucMSubType, std::string strTem);



    /**
    * @brief SIP message fill data body Content-Type: application/reginfo+xml.
    *
    * @param [in] pstDoc
    * @param [in] zMsgBuf
    * @param [out] pXmlStr, the xml string
    * @param [out] iStrLen,  the xml string length .
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillReginfoXmlBody(ST_DOM_DOC **pstDoc, ZDBUF *zMsgBuf, char *pXmlStr, int & iStrLen);


    bool SetXmlNodeAttrAndChildNodes(ST_DOM_DOC *pstDoc,ST_DOM_ELEM * pParentElem, XmlNode * pParentNode);


    XmlNode GenerateTestXmlNodes();


    /**
    * @brief Copy header "From" or "To" of SIP message.
    *
    * @param [in] zBufId Buffer Id..
    * @param [out] pstDst Destination data structure.
    * @param [in] pstSrc Source data structure.
    *
    * @retval ZOK Copy data successfully.
    * @retval ZFAILED Otherwise.
    */
    bool  CpyHdrFromTo(  ST_SIP_HDR_FROM_TO *pstDst, ST_SIP_HDR_FROM_TO *pstSrc);




    /**
    * @brief Copy header list from SIP message.
    *
    * @param [in] zBufId Buffer Id..
    * @param [out] pstHdrLst Destination header list.
    * @param [in] pstMsg Source SIP message.
    * @param [in] ucType Header type to copy, @ref EN_SIP_HDR.
    *
    * @retval ZOK Copy data successfully.
    * @retval ZFAILED Otherwise.
    */
    bool CpyHdrFromMsg( ST_SIP_MSG *pstMsgDst, ST_SIP_MSG *pstMsgSrc,	ZUCHAR ucType);


    /**
    * @brief SIP parameter fill contact +g.oma.SIP-im.large-message.
    *
    * @param [in] zBufId Buffer request memory from.
    * @param [in] pstParm Parameter of contact.
    * @param [in] pstVal sip.instance value string.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool ParmFillContactSipInstance(ST_ZOS_SSTR *pstVal);


    /**
    * @brief SIP  Copy the decoded record-Route to encoded Route
    *
    * @param [in] pstMsgDst
    * @param [in] pstMsgSrc
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool CopyRecorRouteToRoute(ST_SIP_MSG *pstMsgDst, ST_SIP_MSG *pstMsgSrc);

    /**
    * @brief Fill Max-Forwards header
    *
    * @param [in] lMaxForwards
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrMaxForwards(ZULONG lMaxForwards);

    /**
    * @brief Fill a SIP message header as extension header.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] pcName Header name string.
    * @param [in] pcStr Header string value.
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrExtHdr(ZCHAR *pcName, ZCHAR *pcStr);

    /**
    * @brief Fill a SIP message header  P-Called-Party-ID
    *
    * @param [in] strDispName SIP message.
    * @param [in] pcName Header name string.
    * @param [in] pcStr Header string value.
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrPCalledPartyIdByTel(const std::string & strGlobalNumber,const std::string & strParmName, const std::string & strParmValue);


    bool fillHdrPCalledPartyIdBySipUri(const ST_SIP_SIP_URI & stSipUri, bool bSip);


    /**
    * @brief Fill a SIP message header  P-Asserted-Identity
    *
    * @param [in] stNaSpec SIP message.

    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrPAssertedIdentity(const ST_SIP_NA_SPEC & stNaSpec);

    /**
    * @brief Fill a SIP message header  P-Asserted-Service
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool  fillHdrPAssertedService(const std::string & strSer);

    /**
    * @brief Fill a SIP message header  Contribution-ID
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrContributionID(const std::string & strContId);


    /**
    * @brief Fill a SIP message header  Conversation-ID
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrConversationID(const std::string & strConvertId);

    /**
    * @brief Fill a SIP message header  Conversation-ID
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrDate();

    /**
    * @brief Fill a SIP message header  Conversation-ID
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrRAck(ZUINT iCseqNum,ZUINT iRspNum, ZUCHAR ucMethod);

    /**
    * @brief Fill a SIP message header  Content-Disposition header
    *
    * @retval ZOK Fill header successfully.
    * @retval ZFAILED Fill header failed.
    */
    bool fillHdrContentDisposition(const std::string & strDisp);

    bool fillHdrExt(const std::string & name, const std::string & val);
    bool fillHdrExts(const std::map<std::string, std::string> & headers);
    bool fillFromToPram(const std::string & name, const std::string & val);
    bool fillFromToPrams(const std::map<std::string, std::string> & params);
    bool fillSipUriParams(ST_SIP_SIP_URI *pstSipUri, vector<ST_SIP_URI_PARM> & params);

    /**
    * @brief SIP message fill require header, flag see SIP_TAG_OPT_ADD_100REL...
    *
    * @param [in] pstMsg SIP message.
    * @param [in] iSuptFlag Support option flag, @ref SIP_TAG_OPT_ADD_100REL.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    bool fillHdrRequire(ZUINT iSuptFlag);
    bool fillHdrMinSe(ZUINT secounds);

    bool fillHdrReason(const std::string &reason);

public:

    ST_SIP_MSG *            _pstMsg;
    std::string          _strMsg;

    //void* operator new(size_t size);
    //void operator delete(void* p);
};

#endif /* __SipMsgEncode_h */
