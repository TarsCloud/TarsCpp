#include "SipMsg.h"
#include "SipProxy/src/SipProxyCommon.h"
#include "util/tc_md5.h"
#include "util/tc_uuid_generator.h"

/* create the first register */
bool SipMsg::createFirstRegister(SipMsgEncodePtr &pstSipMsgEn)
{
    std::string strBranch, strUserInfo, strHostname;
    int iLocalPort, iRemotePort, iRport;
    unsigned char ucMethod, ucTptType;
    vector<int> iSuptFlagVec;
    SipMsgEncodePtr pstSipMsg;

    /* create sip msg encode */
    pstSipMsg = std::make_shared<SipMsgEncode>();
    if (!pstSipMsg->createReq())
    {
        return false;
    }

    /* fill request line */
    ucMethod = EN_SIP_METHOD_REGISTER;
    strUserInfo = "";
    strHostname = SIP_PROXY_DEFAULT_YES_DOMAIN;
    iRemotePort = SIP_PROXY_DEFAULT_SIP_REMOTE_PORT;
    pstSipMsg->fillReqLineBySip(ucMethod,strUserInfo,strHostname,iRemotePort);

    /* fill Via header */
    ucTptType = EN_SIP_TPT_UDP;
    strHostname = "192.168.2.97";
    iLocalPort = SIP_PROXY_DEFAULT_SIP_LOCAL_PORT;
    iRport = 0;
    strBranch = SipMsgCommon::createBranch();
    pstSipMsg->fillHdrVia(ucTptType, strHostname, iLocalPort,true, iRport, strBranch);

    /* fill Route header */
    /* TODO */

    /* fill Max-Forwards */
    pstSipMsg->fillHdrForwards(70);

    /* fill From header */
    strUserInfo = "test.prdy194";
    strHostname = SIP_PROXY_DEFAULT_YES_DOMAIN;
    std::string strTag = tars::TC_UUIDGenerator::getInstance()->genID();
    pstSipMsg->fillHdrFromToByName(true,"", strUserInfo, strHostname, 0, strTag);

    /* fill To header */
    pstSipMsg->fillHdrFromToByName(false, "", strUserInfo, strHostname, 0, "");

    /* fill Call-ID header */
    std::string strNumber = tars::TC_UUIDGenerator::getInstance()->genID();
    pstSipMsg->fillHdrCallId(strNumber, strHostname);

    /* fill CSeq header */
    pstSipMsg->fillHdrCseq(ucMethod, 0);

    /* fill Authorization header */
    StSipAuthor stAuth;
    stAuth._ucDigst = 1;
    stAuth._strUsername = "test.prdy194_ht1@yes.my";
    stAuth._strRealm = "ytlcomms.net";
    stAuth._strNonce = "";
    stAuth._strResponse = "";
    stAuth._ucReqUri = 0;
    pstSipMsg->fillHdrAuthor(stAuth);

    /* fill User-Agent header */
    std::string strName = "Yes";
    std::string strVersion = "Life/1.2.86";
    std::string strComm = "Android 4.4.2";
    pstSipMsg->fillHdrUserAgent(strName, strVersion, strComm);

    /* fill Contact header */
    pstSipMsg->fillHdrContact("+8612345678", "127.0.0.1", 5000, EN_SIP_TPT_PARM_UDP);

    /* fill Expires header */
    int dwExpires = 3600;
    pstSipMsg->fillHdrExpires(dwExpires);

    /* fill Allow header */
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_ACK);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    pstSipMsg->fillHdrAllow(iSuptFlagVec);

    pstSipMsg->encode();

    return  pstSipMsgEn == pstSipMsg;

}

/* create the second register, see Mrf_AuthReplyCln */
bool SipMsg::createSecondRegister(const SipMsgDecodePtr &pstSrcSipMsg, SipMsgEncodePtr &pstDstSipMsg)
{
    unsigned char ucMethod, ucTptType;
    std::string strNumber, strUserInfo, strBranch;
    std::string strHostname, strDispName, strTag;
    int iRemotePort, iLocalPort, iCseq;
    unsigned int lRspCode = 0;
    ST_SIP_DIGEST_RSP stRsp;
    ST_SIP_DIGEST stDigest;
    ST_SIP_REQ_URI stReqUri;
    vector<int> iSuptFlagVec;
    void *zCredBuf;

    ucTptType = EN_SIP_TPT_UDP;
    strHostname = SIP_PROXY_DEFAULT_YES_DOMAIN;
    iRemotePort = SIP_PROXY_DEFAULT_SIP_REMOTE_PORT;
    iLocalPort = SIP_PROXY_DEFAULT_SIP_LOCAL_PORT;

    /* check the code */
    pstSrcSipMsg->getRspStatusCode(lRspCode);
    pstSrcSipMsg->getMethod(ucMethod);
    if(lRspCode != 401 || pstSrcSipMsg->isRequest() || ucMethod != EN_SIP_METHOD_REGISTER)
    {
        TLOGWARN("SipMsg", "dwCode != 401, can not create second register");
        return false;
    }
    if(!pstDstSipMsg)
    {
        pstDstSipMsg = std::make_shared<SipMsgEncode>();
    }
    pstDstSipMsg->createReq();

    /* fill request line */
    ucMethod = EN_SIP_METHOD_REGISTER;
    strUserInfo = "user/123";
    strUserInfo = "";
    pstDstSipMsg->fillReqLineBySip(ucMethod,strUserInfo,strHostname,iRemotePort);
    stReqUri = pstDstSipMsg->_pstMsg->u.stReqLine.stReqUri;

    {/* add authorization header */
        /* allocate memory */
        zCredBuf = Zos_DbufCreate(ZNULL,ZDBUF_TYPE_STRUCT,64);
        if(!zCredBuf)
        {
            TLOGERROR("SipMsg", "error: add authorization failed, alloc zCredBuf failed");
            return false;
        }

        /* create digest response list */
        ABNF_LIST_CREATE(&stRsp.stDigRspLst);

        /* copy response from challenge */
        if(pstSrcSipMsg->getWwwDigest(stDigest))
        {
            Sip_CpyDigestToRsp(zCredBuf, &stRsp, &stDigest);
        }
        //Mrf_AuthCalcRsp           Mrf_SipAddAuthor
        /* calculate md5 */
        authCalcRsp(pstDstSipMsg, strUserInfo, "123",  EN_SIP_METHOD_REGISTER, &stDigest, &stReqUri, &stRsp);

        /* fill authorization header */
        pstDstSipMsg->fillHdrAuthor(stRsp, 401);
    }

    /* route header */
    Sip_DeleteMsgHdr(pstDstSipMsg->_pstMsg, EN_SIP_HDR_ROUTE);

    /* fill Via header */
    strHostname = "192.168.2.97";
    strBranch = SipMsgCommon::createBranch();
    pstDstSipMsg->fillHdrVia(ucTptType, strHostname, iLocalPort, true,0, strBranch);

    /* fill Max-Forwards */
    pstDstSipMsg->fillHdrForwards(70);

    /* fill From header */
    strUserInfo = "test.prdy194";
    strHostname = SIP_PROXY_DEFAULT_YES_DOMAIN;
    strTag = tars::TC_UUIDGenerator::getInstance()->genID();
    pstDstSipMsg->fillHdrFromToByName(true, strDispName, strUserInfo, strHostname, 0, strTag);

    /* fill To header */
    pstDstSipMsg->fillHdrFromToByName(false, strDispName, strUserInfo, strHostname, 0, "");

    /* fill Call-ID header */
    pstSrcSipMsg->getCallId(strNumber, strHostname);
    pstDstSipMsg->fillHdrCallId(strNumber, strHostname);

    /* fill CSeq header */
    pstSrcSipMsg->getHdrCseq(iCseq);
    pstDstSipMsg->fillHdrCseq(ucMethod, iCseq + 1);

    /* fill User-Agent header */
    std::string strName = "Yes";
    std::string strVersion = "Life/1.2.86";
    std::string strComm = "Android 4.4.2";
    pstDstSipMsg->fillHdrUserAgent(strName, strVersion, strComm);


    /* fill Contact header */
    ST_SIP_ADDR_SPEC stAddrSpec;
    if(pstSrcSipMsg->getContactAddrSpec(stAddrSpec))
    {
        pstDstSipMsg->fillHdrContactByAddrSpec(&stAddrSpec, "", 0);
    }
    else
    {
        pstDstSipMsg->fillHdrContact("+8612345678", "127.0.0.1", 5000, EN_SIP_TPT_PARM_TCP);
    }
    pstDstSipMsg->parmFillContact(CT_CAP_IM | CT_CAP_VIDEOCALL | CT_CAP_VOICECALL);
    pstDstSipMsg->parmFillContactExpires(3600);

    /* fill Expires header */
    int dwExpires = 3600;
    pstDstSipMsg->fillHdrExpires(dwExpires);

    /* fill Allow header */
    iSuptFlagVec.clear();
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_MSG);
    pstDstSipMsg->fillHdrAllow(iSuptFlagVec);

    return pstDstSipMsg->encode();
}

bool SipMsg::createSubsBy200(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn)
{
    std::string strDispName, strUsername, strHostname, strFromTag, strToTag;
    std::string strCallNumber, strCallHost, strBranch, strRecvHost;
    unsigned char ucMethod, ucUriType, ucTptType;
    int  iPort, iRport;
    unsigned int ulRspCode = 0;
    vector<int> viSuptFlag;

    /* check pstSipMsgDe */
    if(!pstSipMsgDe)
        return false;
    if(pstSipMsgDe->getRspStatusCode(ulRspCode))
    {
        if(ulRspCode != 200)
            return false;
    }

    /* init pstSipMsgEn */
    if(!pstSipMsgEn)
    {
        pstSipMsgEn = std::make_shared<SipMsgEncode>();
    }
    pstSipMsgEn->createReq();

    /* fill request line */
    ucMethod = EN_SIP_METHOD_SUBS;
    if(pstSipMsgDe->getFromUri(strDispName, strUsername, strHostname, iPort, ucUriType))
    {
        pstSipMsgEn->fillReqLine(ucUriType, ucMethod, strUsername, strHostname, iPort);
    }

    /* fill From and To */
    strFromTag = tars::TC_UUIDGenerator::getInstance()->genID();
    pstSipMsgEn->fillHdrFromToByName(true, "", strUsername, strHostname, iPort, strFromTag);
    pstSipMsgEn->fillHdrFromToByName(false, "", strUsername, strHostname, iPort, "");

    /* fill Contact */
    ST_SIP_ADDR_SPEC stAddrSpec;
    if(pstSipMsgDe->getContactAddrSpec(stAddrSpec))
    {
        pstSipMsgEn->fillHdrContactByAddrSpec(&stAddrSpec, "", 0);
        pstSipMsgEn->parmFillContact(CT_CAP_IM | CT_CAP_VIDEOCALL | CT_CAP_VOICECALL);
    }

    /* fill Support */
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_PATH);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    /* fill header event */
    pstSipMsgEn->fillHdrEvnt(EN_SIP_EVNT_PKG_REG, EN_SIP_EVNT_TEMP_OTHER);

    /* fill header Allow */
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_ALLOW_HDR_ADD_MSG);
    viSuptFlag.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    pstSipMsgEn->fillHdrAllow(viSuptFlag);

    /* fill header Accept */
    pstSipMsgEn->fillHdrAcpt(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_REGINFO_XML);

    /* fill header Expires */
    pstSipMsgEn->fillHdrExpires(3600);

    /* fill header Call-ID */
    pstSipMsgDe->getCallId(strCallNumber, strCallHost);
    pstSipMsgEn->fillHdrCallId(strCallNumber, strCallHost);

    /* fill header CSeq */
    pstSipMsgEn->fillHdrCseq(ucMethod, 1);

    /* fill header Max-Forwards */
    pstSipMsgEn->fillHdrForwards(70);

    /* fill header User-Agent */
    pstSipMsgEn->fillHdrUserAgent("CPM-client", "OMA1.0","");
    pstSipMsgEn->fillHdrUserAgent("MSP", "1.0","");


    /* fill header Via */
    pstSipMsgDe->getHdrViaOther(ucTptType, strHostname, iPort, strRecvHost, iRport);
    strBranch = SipMsgCommon::createBranch();
    pstSipMsgEn->fillHdrVia(ucTptType, strHostname, iPort, true,0, strBranch);

    return  pstSipMsgEn->encode();
}

/* create INVITE sip msg */
bool SipMsg::createInvite(const std::string &strFromNumber, const std::string &strToNumber,const std::string & strMediaListenIp,
                          SipMsgEncodePtr &pstSipMsgEn)
{
    unsigned ucMethod, ucTptType;
    std::string strHost, strCallHost;
    std::string strFromDispname, strToDispName, strContactHost;
    std::string strFromTag, strBranch;
    std::string strUAName, strUAVersion, strUAComm;
    int iPort;
    vector<int> iSuptVec;

    ucMethod = EN_SIP_METHOD_INVITE;
    strFromDispname = "13957854053";
    strFromTag = tars::TC_UUIDGenerator::getInstance()->genID();
    strToDispName = "8616000000330";
    strHost = "gd.ims.mnc000.mcc460.3gppnetwork.org";
    strContactHost = "192.168.8.100";
    iPort = 53890;
    strCallHost = "192.168.8.100";
    strUAName = "CPM-client";
    strUAVersion = "OMA1.0";
    //strUAComm = "MSP/1.0";
    if(!pstSipMsgEn)
    {
        pstSipMsgEn = std::make_shared<SipMsgEncode>();
    }
    pstSipMsgEn->createReq();

    /* fill request line */
    pstSipMsgEn->fillReqLineByTel(ucMethod, strToNumber);

    /* fill from header */
    pstSipMsgEn->fillHdrFromToByName(true,strFromDispname, strFromNumber, strHost, 0, strFromTag);

    /* fill to header */
    pstSipMsgEn->fillHdrFromToByName(false, strToDispName, strToNumber, strHost, 0, "");

    /* fill P-Preferred-Identity header*/
    pstSipMsgEn->fillHdrPPreferredIdentity(strFromDispname);

    /*fill Accept-Contact Header */
    vector<int> paramFlags;
    paramFlags.push_back(EN_SIP_AC_PARMS_G_3GPP_CS_VOICE);
    paramFlags.push_back(EN_SIP_AC_PARMS_G_OMA_SIP_IM);
    //paramFlags.push_back(EN_SIP_AC_PARMS_G_3GPP_APP_REF);

    map<std::string ,std::string> geneParas;
    geneParas.insert(make_pair("DDHJX", "87@jdsjjk88jhikldsahh"));
    geneParas.insert(make_pair("+g.3gpp.icsi-ref", "urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"));
    geneParas.insert(make_pair("AAA", ""));
    pstSipMsgEn->fillHdrAcceptContact(paramFlags, geneParas);

    /* fill P-Early-Media Header  EN_SIP_P_EARLY_MEDIA_PARM*/
    pstSipMsgEn->fillHdrPEarlyMedia(EN_SIP_P_EARLY_MEDIA_PARM_SUPPORTED);

    /* fill contact header */
    pstSipMsgEn->fillHdrContact(strFromNumber, strHost, iPort, EN_SIP_TPT_PARM_TCP);
    pstSipMsgEn->parmFillContactExpires(3600);

    /* fill Supported heder */
    iSuptVec.push_back(SIP_TAG_OPT_ADD_100REL);
    iSuptVec.push_back(SIP_TAG_OPT_ADD_JOIN);
    iSuptVec.push_back(SIP_TAG_OPT_ADD_NOREFERSUB);
    iSuptVec.push_back(SIP_TAG_OPT_ADD_REPLACES);
    pstSipMsgEn->fillHdrSupted(iSuptVec);

    /* fill call-id header */
    pstSipMsgEn->fillHdrCallId(tars::TC_UUIDGenerator::getInstance()->genID(), strCallHost);

    /* fill cseq header */
    pstSipMsgEn->fillHdrCseq(ucMethod, 1);

    /* fill max-forwards header */
    pstSipMsgEn->fillHdrForwards(70);

    /* fill user-agent header */
    pstSipMsgEn->fillHdrUserAgent(strUAName, strUAVersion, "");
    pstSipMsgEn->fillHdrUserAgent("MSP", "1.0", "");

    /* fill via header */
    ucTptType = EN_SIP_TPT_TCP;
    strBranch = SipMsgCommon::createBranch();
    pstSipMsgEn->fillHdrVia(ucTptType, strContactHost, iPort, true,0, strBranch);

    /* fill content-type header */
    pstSipMsgEn->fillHdrContentType(EN_SIP_MTYPE_DISC_APP, "", EN_SIP_MSUBTYPE_SDP, "");

    int flag = 1;
    if( flag )
    {/* fill SDP body */
        /* set origin */
        StSdpOrigin stOrigin;
        SipMsgCommon::setArcSdpOrigin(EN_SDP_NET_IN, EN_SDP_ADDR_IPV4, 1023187643, 1023187643, strMediaListenIp, "+8613957854053", stOrigin);

        /* set connection info */
        StSdpConnInfo stConnInfo;
        SipMsgCommon::setArcSdpConnInfo(EN_SDP_NET_IN, EN_SDP_ADDR_IPV4, strMediaListenIp, stConnInfo);

        /* set time */
        StSdpTimeField stTimeField;
        stTimeField._lStartTime = 0;
        stTimeField._lStopTime = 0;

        /* set media */
        StSdpMediaDesc stMediaDescAudio;
        SipMsgCommon::setArcSdpMediaDesc(EN_SDP_MEDIA_AUDIO, EN_SDP_PROTO_RTP_AVP, 37076, stMediaDescAudio);
        //114 116 103 105 0 8 18 104 97
        stMediaDescAudio._strFmtVec.push_back(114);
        stMediaDescAudio._strFmtVec.push_back(116);
        stMediaDescAudio._strFmtVec.push_back(103);
        stMediaDescAudio._strFmtVec.push_back(105);
        stMediaDescAudio._strFmtVec.push_back(0);
        stMediaDescAudio._strFmtVec.push_back(8);
        stMediaDescAudio._strFmtVec.push_back(18);
        stMediaDescAudio._strFmtVec.push_back(104);
        stMediaDescAudio._strFmtVec.push_back(97);

        /* set StSdpSessDesc */
        StSdpSessDesc stSessDesc;
        stSessDesc._iVer = 0;
        stSessDesc._strSessName = "-123";

        SipMsgCommon::setArcSdpSessDescX(&stOrigin, &stConnInfo, &stTimeField, stSessDesc);
        stSessDesc._stMediaVec.push_back(stMediaDescAudio);
    }
    else
    {
        //SipMsgCommon::str2ZosSstr(strBody, stBodyStr);
        TLOGWARN("SipMsg", "SipMsg::createInvite() called.CONST_SDP_STRING : \n" + CONST_SDP_STRING);
        std::string strSDP = CONST_SDP_STRING;
        bool resultSdp = pstSipMsgEn->fillBodySDPDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strSDP);                  //有问题！！！！！
        if (!resultSdp)
        {
            TLOGWARN("SipMsg", "SipMsg::createInvite() called.fillBodySDPDataStr return FALSE!");
            return false;
        }
    }
    return pstSipMsgEn->encode();
}

bool SipMsg::create100Trying(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn)
{
    std::string strBranch, strHost, strRecvHost;
    std::string strDispName, strNumber, strUserName;
    std::string strFromTag;
    unsigned char ucTptType, ucUriType, ucMethod;
    int iPort, iRport, iCseq;
    void *zMemBuf;

    /* check the pointer */
    if(!pstSipMsgDe) return false;
    if(!pstSipMsgEn)
    {
        pstSipMsgEn = std::make_shared<SipMsgEncode>();
        pstSipMsgEn->createRsp();
    }
    zMemBuf = pstSipMsgEn->_pstMsg->zMemBuf;

    /* fill status line */
    pstSipMsgEn->fillStatusLine(100);

    /* fill header via */
    pstSipMsgDe->getViaBranch(strBranch);
    pstSipMsgDe->getHdrViaOther(ucTptType, strHost, iPort, strRecvHost, iRport);
    pstSipMsgEn->fillHdrVia(ucTptType, strHost, iPort,true, iRport, strBranch);

    /* fill header to */
    pstSipMsgDe->getHdrToTelUri(strDispName, strNumber);
    pstSipMsgEn->fillHdrFromToTelUri(false, strDispName, strNumber, "");

    /* fill header from */
    pstSipMsgDe->getHdrFromToTag(true, strFromTag);
    pstSipMsgDe->getFromUri(strDispName, strUserName, strHost, iPort, ucUriType);
    pstSipMsgEn->fillHdrFromToByName(true, strDispName, strUserName, strHost, iPort, strFromTag);

    /* fill header call-id */
    pstSipMsgDe->getCallId(strNumber, strHost);
    pstSipMsgEn->fillHdrCallId(strNumber, strHost);

    /* fill header cseq */
    pstSipMsgDe->getMethod(ucMethod);
    pstSipMsgDe->getHdrCseq(iCseq);
    pstSipMsgEn->fillHdrCseq(ucMethod, iCseq);

    /* fill header user-agent */
    pstSipMsgEn->fillHdrUserAgent("ZTE-B200", "", "");

    return pstSipMsgEn->encode();
}

/* create 180 ringing sip msg */
bool SipMsg::create180Ringing(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn, std::string & strToTag)
{
    std::string strBranch, strHost, strRecvHost;
    std::string strDispName, strNumber, strUserName;
    std::string strFromTag;
    unsigned char ucTptType, ucUriType, ucMethod;
    int iPort, iRport, iCseq;
    void *zMemBuf;

    /* check the pointer */
    if(!pstSipMsgDe) return false;

    /* init pstSipMsgEn */
    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    pstSipMsgEn->createRsp();
    zMemBuf = pstSipMsgEn->_pstMsg->zMemBuf;

    /* fill status line */
    pstSipMsgEn->fillStatusLine(180);

    /* fill header via */
    pstSipMsgDe->getViaBranch(strBranch);
    pstSipMsgDe->getHdrViaOther(ucTptType, strHost, iPort, strRecvHost, iRport);
    pstSipMsgEn->fillHdrVia(ucTptType, strHost, iPort,true, iRport, strBranch);

    /* fill header to */

    pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucUriType);
    strToTag =  SipMsgCommon::createToTag();
    pstSipMsgEn->fillHdrFromToTelUri(false, strDispName, strNumber, strToTag);

    /* fill header from */
    pstSipMsgDe->getHdrFromToTag(true, strFromTag);
    pstSipMsgDe->getFromUri(strDispName, strUserName, strHost, iPort, ucUriType);
    pstSipMsgEn->fillHdrFromToByName(true, strDispName, strUserName, strHost, iPort, strFromTag, ucUriType);

    /* fill header call-id */
    pstSipMsgDe->getCallId(strNumber, strHost);
    pstSipMsgEn->fillHdrCallId(strNumber, strHost);

    /* fill header cseq */
    pstSipMsgDe->getMethod(ucMethod);
    pstSipMsgDe->getHdrCseq(iCseq);
    pstSipMsgEn->fillHdrCseq(ucMethod, iCseq);

    /* fill header contact */
    pstSipMsgEn->fillHdrContact("", "117.136.239.32", 5460, EN_SIP_TPT_PARM_TCP);

    /* fill header supported */
    vector<int> iSuptFlagVec;
    iSuptFlagVec.push_back(SIP_TAG_OPT_ADD_100REL);
    pstSipMsgEn->fillHdrSupted(iSuptFlagVec);

    /* fill header allow */
    iSuptFlagVec.clear();
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_REGISTER);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_ACK);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_BYE);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_REFER);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_INFO);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_MSG);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_SUBS);
    pstSipMsgEn->fillHdrAllow(iSuptFlagVec);

    /* fill header server */
    pstSipMsgEn->fillHdrServer("CPM-client", "OMA1.0", "MSP/1.0");

    /* fill header accept */
    pstSipMsgEn->fillHdrAcpt(EN_SIP_MTYPE_DISC_APP,EN_SIP_MSUBTYPE_SDP);
    pstSipMsgEn->HdrAcptAddRange(EN_SIP_MTYPE_COMP_MULTI,EN_SIP_MSUBTYPE_MIXED);

    return pstSipMsgEn->encode();
}

bool SipMsg::create200ByInvite(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn,const std::string & strToTag)
{
    std::string strBranch, strHost, strRecvHost;
    std::string strDispName, strNumber, strUserName;
    std::string strFromTag;
    unsigned char ucTptType, ucUriType;
    int iPort, iRport;
    void *zMemBuf;

    /* check the pointer */
    if(!pstSipMsgDe) return false;

    /* init pstSipMsgEn */
    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    pstSipMsgEn->createRsp();
    zMemBuf = pstSipMsgEn->_pstMsg->zMemBuf;

    /* fill status line */
    pstSipMsgEn->fillStatusLine(200);

    /* fill header via */
    pstSipMsgDe->getViaBranch(strBranch);
    pstSipMsgDe->getHdrViaOther(ucTptType, strHost, iPort, strRecvHost, iRport);
    pstSipMsgEn->fillHdrVia(ucTptType, strHost, iPort,true, iRport, strBranch);

    /* fill header to */
    //pstSipMsgDe->getHdrToTelUri(strDispName, strNumber);
    pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucUriType);
    pstSipMsgEn->fillHdrFromToByName(false, strDispName, strUserName, strHost, iPort, strFromTag, ucUriType);

    /* fill header from */
    pstSipMsgDe->getHdrFromToTag(true, strFromTag);
    pstSipMsgDe->getFromUri(strDispName, strUserName, strHost, iPort, ucUriType);
    pstSipMsgEn->fillHdrFromToByName(true, strDispName, strUserName, strHost, iPort, strFromTag, ucUriType);


    //  /* fill header call-id */
    //  pstSipMsgDe->getCallId(strNumber, strHost);
    //  pstSipMsgEn->fillHdrCallId(strNumber, strHost);

    //  /* fill header cseq */
    //  pstSipMsgDe->getMethod(ucMethod);
    //  pstSipMsgDe->getHdrCseq(iCseq);
    //  pstSipMsgEn->fillHdrCseq(ucMethod, iCseq);

    //  /* fill header contact */
    //  pstSipMsgEn->fillHdrContact("Anonymous", "117.136.239.32", 5460, EN_SIP_TPT_PARM_TCP);

    //  /* fill header Record-Route */
    //  pstSipMsgEn->fillHdrRecordRoute("DIAG_13_0_0d00002c", "117.136.239.32", 5460, EN_SIP_TPT_PARM_TCP);

    //  /* fill header supported */
    //  vector<int> iSuptFlagVec;
    //  iSuptFlagVec.push_back(SIP_TAG_OPT_ADD_100REL);
    //  iSuptFlagVec.push_back(SIP_TAG_OPT_ADD_REPLACES);
    //  pstSipMsgEn->fillHdrSupted(iSuptFlagVec);

    //  /* fill header Session-Expires */
    //  pstSipMsgEn->fillHdrSessExpires(true, true, 1800);

    //  /* Privacy */
    //  pstSipMsgEn->fillHdrPrivacy(EN_SIP_PRIV_VAL_ID);

    //  /* fill header allow */
    //  iSuptFlagVec.clear();
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_REGISTER);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_ACK);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_BYE);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_REFER);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_INFO);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_MSG);
    //  iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_SUBS);
    //  pstSipMsgEn->fillHdrAllow(iSuptFlagVec);

    //  ///* fill header server */
    //  pstSipMsgEn->fillHdrServer("CPM-client", "OMA1.0", "MSP/1.0");

    //  ///* fill header content-type */
    //   pstSipMsgEn->fillHdrContentType(EN_SIP_MTYPE_DISC_APP, "", EN_SIP_MSUBTYPE_SDP, "");

    //  ///* fill header Content-Disposition */
    //std::string strTem("session");
    //   pstSipMsgEn->fillHdrContentDisp(strTem.c_str());

    {/* fill body sdp */
        StSdpOrigin stOrigin;
        SipMsgCommon::setArcSdpOrigin(EN_SDP_NET_IN, EN_SDP_ADDR_IPV4, 973962699, 1671092924, "117.136.239.79", "+8616000000330", stOrigin);

        /* set connection info */
        StSdpConnInfo stConnInfo;
        SipMsgCommon::setArcSdpConnInfo(EN_SDP_NET_IN, EN_SDP_ADDR_IPV4, "117.136.239.79", stConnInfo);

        /* set time */
        StSdpTimeField stTimeField;
        stTimeField._lStartTime = 0;
        stTimeField._lStopTime = 0;

        /* set media */
        StSdpMediaDesc stMediaDesc;
        SipMsgCommon::setArcSdpMediaDesc(EN_SDP_MEDIA_AUDIO, EN_SDP_PROTO_RTP_AVP, 20232, stMediaDesc);
        stMediaDesc._strFmtVec.push_back(105);
        stMediaDesc._strFmtVec.push_back(0);
        stMediaDesc._strFmtVec.push_back(8);
        stMediaDesc._strFmtVec.push_back(97);

        /* set StSdpSessDesc */
        StSdpSessDesc stSessDesc;
        SipMsgCommon::setArcSdpSessDesc(0, "", "", "", "", "", stSessDesc);
        SipMsgCommon::setArcSdpSessDescX(&stOrigin, &stConnInfo, &stTimeField, stSessDesc);
        stSessDesc._stMediaVec.push_back(stMediaDesc);

        pstSipMsgEn->fillBodySdp(stSessDesc);
    }
    return pstSipMsgEn->encode();
}

bool SipMsg::authCalcRsp(const SipMsgEncodePtr & pstSipMsgEn, const SipProxy::SipRegisterSubscribePtr & pSipReg, unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp)
{
    std::string strRealm, strUri, strNonce, strCnonce, strMethod, strNonceCount, strQop, strRegisterID, strUsername, strPassword ;
    std::string strTemp, strKd;
    ST_ZOS_SSTR stCnonce, stNonceCount, stQop;
    char acCnonce[32], acNonceCount[32];
    bool qopExist = false;
    unsigned char qopType = 0, algoType = 0;
    int	nonceCount = 0;

    std::string temA1, temA2;
    std::string temRealm, temNonce, temMd5Rsp, temCNonce, temResponse, opaque;

    strRegisterID = pSipReg->getRegisterId();
    strUsername = pSipReg->getAuthName();
    strPassword =  pSipReg->getPassWord();
    qopExist = pSipReg->getDigestQopExist();
    qopType = pSipReg->getDigestQopType();
    nonceCount = pSipReg->getDigestNonceCount();

    temRealm = pSipReg->getDigestRealm();
    temNonce = pSipReg->getdigestNonce();
    algoType = pSipReg->getDigestAlgo();
    temCNonce = pSipReg->getDigestCNonce();
    temResponse = pSipReg->getDigestResponse();
    opaque = pSipReg->getDigestOpaque();

    ST_ZOS_SSTR stNonce;
    SipMsgCommon::str2ZosSstr(temNonce, stNonce);

    /* get qop from pstDigest */
    strQop = "auth";

    //TEST
    bool result = SipMsgCommon::calA1(strUsername, strPassword, temRealm, temA1);
    result = SipMsgCommon::calA2(ucMethod, pstReqUri,temA2);

    TLOGDEBUG("SipMsg", "authCalcRsp() called. strUsername:" + strUsername + ",strPassword:" + strPassword
        + ",temRealm:" + temRealm + ",nonce:" + temNonce + ",Cnonce:" + temCNonce + ",temA1:" + temA1 + ",temA2:" + temA2 + ",opaque:" + opaque);

    std::string acKDMd5Str;
    if (qopExist && (qopType == EN_SIP_QOP_VAL_AUTH || qopType == EN_SIP_QOP_VAL_AUTH_INT))
    {
        if (temCNonce.empty())
        {
            /* prepare cnonce, we use the var address as the random cnonce value */
            Zos_SNPrintf(acCnonce, 17, "%08x%08x", Zrandom_N32(), Zrandom_N32());
            ZOS_SSTR_SETS(&stCnonce, acCnonce);

            temCNonce = acCnonce;
        }
        else
        {
            ZOS_SSTR_SETS(&stCnonce, (char*)temCNonce.c_str());
        }

        /* prepare nonce count, only generate one cnonce for every nonce */
        Zos_SNPrintf(acNonceCount, 9, "%08x", nonceCount);
        ZOS_SSTR_SETS(&stNonceCount, acNonceCount);
        /* prepare qop */
        ZOS_SSTR_SETS(&stQop, (char*)"auth");

        result	= SipMsgCommon::calKD((char*)temA1.c_str(), (char*)temA2.c_str(), &stNonce, &stCnonce, &stNonceCount,&stQop,acKDMd5Str);
    }
    else
    {
        result	= SipMsgCommon::calKD((char*)temA1.c_str(), (char*)temA2.c_str(), &stNonce, 0, 0, 0, acKDMd5Str);
    }
    TLOGDEBUG("SipMsg", "authCalcRsp() called. qopExist:" + std::to_string(qopExist) + ",Cnonce:" + temCNonce + ",acKDMd5Str:" + acKDMd5Str);

    std::string tem1 = acCnonce;
    pSipReg->setDigestCNonce(temCNonce);
    pSipReg->setDigestResponse(acKDMd5Str);

    //fill response
    pstSipMsgEn->authDRspFillRealm(pstRsp, temRealm);
    pstSipMsgEn->authDRspFillNonce(pstRsp, temNonce);
    pstSipMsgEn->authDRspFillAlgo(pstRsp, algoType);
    if (qopExist)
    {
        pstSipMsgEn->authDRspFillQop(pstRsp,  qopType);
    }
    if (opaque.empty() == false)
    {
        pstSipMsgEn->authDRspFillQpaque(pstRsp, opaque);
    }

    /* fill param nonce to header Authorization */
    pstSipMsgEn->authDRspFillUsername(pstRsp, strUsername);
    pstSipMsgEn->authDRspFillUri(pstRsp, pstReqUri);

    if (strPassword.empty())
    {
        /* fill param response to header Authorization */
        pstSipMsgEn->authDRspFillRsp(pstRsp, "");
    }
    else
    {
        /* fill param response to header Authorization */
        pstSipMsgEn->authDRspFillRsp(pstRsp, acKDMd5Str);
    }

    if (qopExist && (qopType == EN_SIP_QOP_VAL_AUTH || qopType == EN_SIP_QOP_VAL_AUTH_INT))
    {
        pstSipMsgEn->authDRspFillCnonce(pstRsp, &stCnonce);
        pstSipMsgEn->authDRspFillNonceCount(pstRsp, &stNonceCount);
    }

    return true;
}

bool SipMsg::testCalcResponse(const std::string & strUsername, const std::string & strPassword, const std::string & strNonce,
                              const std::string & strRealm,const std::string & cnonce, ST_SIP_REQ_URI *pstReqUri)
{
    std::string  strUri, strMethod, strQop, strRegisterID ;
    std::string strTemp, strA1, strA2, strKd;
    ST_ZOS_SSTR   stNonce, stCnonce, stNonceCount, stQop;
    //char acCnonce[32], acNonceCount[32];
    bool qopExist = false;

    int	nonceCount = 0;
    std::string temA1, temA2;

    /* get qop from pstDigest */
    strQop = "auth";

    bool result = SipMsgCommon::calA1(strUsername, strPassword, strRealm, temA1);

    /* calculate A2 */
    // calcA2(ucMethod, pstReqUri, strA2);
    result = SipMsgCommon::calA2(EN_SIP_METHOD_REGISTER, pstReqUri,temA2);
    std::string acKDMd5Str;

    /* prepare cnonce, we use the var address as the random cnonce value */
    //Zos_SNPrintf(acCnonce, 17, "%08x%08x", Zrandom_N32(), Zrandom_N32());
    //ZOS_SSTR_SETS(&stCnonce, acCnonce);

    std::string strNonceCount = "00000001";
    SipMsgCommon::str2ZosSstr(strNonce, stNonce);       //nonce
    SipMsgCommon::str2ZosSstr(cnonce, stCnonce);
    SipMsgCommon::str2ZosSstr(strNonceCount, stNonceCount);

    /* prepare nonce count, only generate one cnonce for every nonce */
    //Zos_SNPrintf(acNonceCount, 9, "%08x", nonceCount);
    //ZOS_SSTR_SETS(&stNonceCount, acNonceCount);

    /* prepare qop */
    ZOS_SSTR_SETS(&stQop, (char*)"auth");
    result	= SipMsgCommon::calKD((char*)temA1.c_str(), (char*)temA2.c_str(),&stNonce, &stCnonce, &stNonceCount,&stQop,acKDMd5Str);

    /* calculate A1 */
    calcA1(strUsername, strPassword, strRealm, strA1);

    /* calculate A2 */
    calcA2(EN_SIP_METHOD_REGISTER, pstReqUri, strA2);

    /* calculate KD */
    calcKd(strA1, strA2, strNonce, cnonce, "00000001", strQop, strKd);

    return true;
}

bool SipMsg::authCalcRsp(const SipMsgEncodePtr & pstSipMsgEn, const std::string &strUsername, const std::string &strPassword,  unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp)
{
    std::string strRealm, strUri, strNonce, strCnonce, strMethod, strNonceCount, strQop;
    std::string strTemp, strA1, strA2, strKd;
    ST_ZOS_SSTR *pstRealm, *pstNonce;

    /* create a rand string of cnonce */
    strCnonce = tars::TC_UUIDGenerator::getInstance()->genID();

    /* get realm from pstDigest */
    if(Sip_ParmPickDigestRealm(pstDigest, &pstRealm) == ZOK)
    {
        SipMsgCommon::zosSstr2Str(*pstRealm, strRealm);
    }

    /* get uri from pstDigest */
    if(Sip_ParmPickDigestNonce(pstDigest, &pstNonce) == ZOK)
    {
        SipMsgCommon::zosSstr2Str(*pstNonce, strNonce);
    }

    /* get qop from pstDigest */
    strQop = "auth";

    /* calculate A1 */
    calcA1(strUsername, strPassword, strRealm, strA1);

    /* calculate A2 */
    calcA2(ucMethod, pstReqUri, strA2);

    /* calculate KD */
    calcKd(strA1, strA2, strNonce, strCnonce, strNonceCount, strQop, strKd);

    /* fill param nonce to header Authorization */
    pstSipMsgEn->authDRspFillNonce(pstRsp, strNonce);

    /* fill param response to header Authorization */
    pstSipMsgEn->authDRspFillRsp(pstRsp, strKd);

    /*  */
    pstSipMsgEn->authDRspFillUri(pstRsp, pstReqUri);

    /*  */
    pstSipMsgEn->authDRspFillUsername(pstRsp, strUsername);

    return true;
}

/* calculate A1 */
bool SipMsg::calcA1(const std::string &strUsername, const std::string &strPassword,
                    const std::string &strRealm, std::string &strA1)
{
    std::string strTemp = strUsername + ":" + strRealm + ":" + strPassword;
    strA1 = tars::TC_MD5::md5str(strTemp);
    return true;
}

/* calculate A2 */
bool SipMsg::calcA2(unsigned char ucMethod, ST_SIP_REQ_URI *pstReqUri,
                    std::string &strA2)
{
    ZDBUF zMsgBuf,  zNewBuf;
    ST_ZOS_SSTR stUri;
    std::string strUri, strMethod, strTemp;

    /* check the uri */
    if(!pstReqUri)
    {
        return false;
    }

    /* create new message buffer */
    SIP_CREATE_MSGBUFX(zMsgBuf, 256);

    /* encode request-uri */
    if(Sip_AnyElemEncodeX(pstReqUri, (PFN_SIPANYENCODE)Sip_EncodeReqUri,
        zMsgBuf) != ZOK)
    {
        SIP_DBUF_DELETE(zMsgBuf);
        //SipMsgCommon::speak("encode request-uri failed");
        return false;
    }

    zNewBuf = Zos_DbufFlat(zMsgBuf);
    Zos_DbufO2D(zNewBuf, 0, &stUri.pcStr);
    SipMsgCommon::zosSstr2Str(stUri, strUri);
    strMethod = SipMsgCommon::sipMsgType2String(ucMethod);
    strTemp = strMethod + ":" + strUri;
    strA2 = tars::TC_MD5::md5str(strTemp);
    return true;
}

/* calculate KD */
bool SipMsg::calcKd(const std::string &strA1, const std::string &strA2,
                    const std::string &strNonce, const std::string &strCnonce,
                    const std::string strNonceCount, const std::string &strQop,
                    std::string &strKd)
{
    std::string strTemp;

    /* add A1 */
    strTemp += strA2;
    strTemp += ":";

    /* add nonce */
    strTemp += strNonce;
    strTemp += ":";

    /* qop exist */
    if(!strQop.empty())
    {
        /* add nonce count */
        strTemp += strNonceCount;
        strTemp += ":";

        /* add cnonce */
        strTemp += strCnonce;
        strTemp += ":";

        /* add qop */
        strTemp += strQop;
        strTemp += ":";
    }

    /* add A2 */
    strTemp += strA2;

    /* calculate md5 */
    strKd = tars::TC_MD5::md5str(strTemp);

    return true;
}

/* copy header contact to response */
bool SipMsg::cpyHdrContact(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn)
{
    unsigned char ucMethod;
    ST_SIP_HDR_CONTACT *pstContactDe, *pstContactEn;
    ST_SIP_CONTACT_PARM *pstParmDe;

    ucMethod = EN_SIP_HDR_CONTACT;

    /* get decode contact */
    pstContactDe = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(pstSipMsgDe->_pstMsg, ucMethod);
    if (!pstContactDe)
        return false;

    pstParmDe = (ST_SIP_CONTACT_PARM *)ABNF_LIST_HEAD_DATA(&pstContactDe->stParmLst);
    if (!pstParmDe)
        return false;

    /* create encode contact */
    pstContactEn = (ST_SIP_HDR_CONTACT *)Sip_CreateMsgHdr(pstSipMsgEn->_pstMsg, ucMethod);
    Sip_ParmContactLstAdd(pstSipMsgEn->_pstMsg->zMemBuf, &pstContactEn->stParmLst, &pstParmDe);

    return true;
}

bool SipMsg::printDecodeMsg(const SipMsgDecodePtr &pstSipMsgDe)
{
    if(!pstSipMsgDe) /* check pstSipMsgDe */
    {
        return false;
    }
    /* print method */
    unsigned char ucMethod;
    if(pstSipMsgDe->getMethod(ucMethod))
    {
        SipMsgCommon::speak("method: " + SipMsgCommon::sipMethod2String(ucMethod));
    }

    if(pstSipMsgDe->isRequest()) /* print request line */
    {
        std::string strUserInfo, strHostname, strSpeak;
        int iPort, iReqUriType;
        pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
        strSpeak = "userInfo: " + strUserInfo + ", hostname: " + strHostname + " : " + std::to_string(iPort);
        SipMsgCommon::speak(strSpeak);
    }
    else /* print response line */
    {
        unsigned int ulRspCode = 0;
        std::string strRspReason, strSpeak;
        pstSipMsgDe->getRspStatusCode(ulRspCode);
        pstSipMsgDe->getRspReason(strRspReason);
        strSpeak = "rspCode: " + std::to_string(ulRspCode) + ", rspReason: " + strRspReason;
        SipMsgCommon::speak(strSpeak);
    }

    //print from user-name
    std::string strDispName, strUserName, strHost;
    int  iPort = 0;
    unsigned char ucUriType;

    //if(pstSipMsgDe->getFromUri(strDispName, strUserName, strHost, iPort, ucUriType))
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucUriType))
    {
        TLOGDEBUG("SipMsg", "From: displayName :" + strDispName + ", username:" + strUserName + "@" + strHost + (iPort != 0 ? " port: " + std::to_string(iPort) : ""));
    }

    /* print from tag */
    std::string strFromTag;
    if(pstSipMsgDe->getHdrFromToTag(true, strFromTag))
    {
        SipMsgCommon::speak("from tag: " + strFromTag);
    }

    strDispName = "";
    strUserName = "";
    strHost = "";
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucUriType))
    {
        TLOGDEBUG("SipMsg", "To: displayName :" + strDispName + ", username:" + strUserName + "@" + strHost + (iPort != 0 ? " port: " + std::to_string(iPort) : ""));
    }

    //print contact
    std::string  strHostName;
    ZCHAR   zcTpt;
    strUserName = "";
    iPort = 0;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort,zcTpt))
    {
        TLOGDEBUG("SipMsg", "Contact:" + (!strUserName.empty() ? " username : " + strUserName : "") + " " + strHostName + (iPort != 0 ? ":" + std::to_string(iPort) : ""));
    }

    /* print call-id */
    std::string strCallIdNumber, strCallIdHost;
    if(pstSipMsgDe->getCallId(strCallIdNumber, strCallIdHost))
    {
        if(strCallIdHost.empty())
        {
            SipMsgCommon::speak("call-id: " + strCallIdNumber);
        }
        else
        {
            SipMsgCommon::speak("call-id: " + strCallIdNumber + "@" + strCallIdHost);
        }
    }

    /* print cseq */
    int iCseq;
    if(pstSipMsgDe->getHdrCseq(iCseq))
    {
        SipMsgCommon::speak("Cseq: " + std::to_string(iCseq));
    }

    /* print max-forwards */
    int iMaxForward;
    if(pstSipMsgDe->getMaxForward(iMaxForward))
    {
        SipMsgCommon::speak("Max-Forwards: " + std::to_string(iMaxForward));
    }

    /* print user-agent */
    std::string strUAName, strUAVer, strUAComm;
    if(pstSipMsgDe->getUserAgent(strUAName, strUAVer, strUAComm))
    {
        SipMsgCommon::speak("User-Agent: " + strUAName + " " + strUAVer + " " + strUAComm);
    }

    /* print Via Uri , branch */
    std::string strViaBranch;
    iPort = 0;
    if (pstSipMsgDe->getHdrViaUri(strHostName,iPort) )
    {
        TLOGDEBUG("SipMsg", "Via: " + strHostName + " :" + std::to_string(iPort));
    }

    if(pstSipMsgDe->getViaBranch(strViaBranch))
    {
        SipMsgCommon::speak("Via: branch = " + strViaBranch);
    }

    /* print Content-Length */
    int iContentLen;
    if(pstSipMsgDe->getContentLength(iContentLen))
    {
        SipMsgCommon::speak("Content-Length: " + std::to_string(iContentLen));
    }

    /* print number for YTL */
    std::string strRspNumber;
    if(pstSipMsgDe->getNumber(strRspNumber))
    {
        SipMsgCommon::speak("number: " + strRspNumber);
    }

    //print  Subscription-State
    ZUCHAR  ucStaType;
    ZUINT  iExpires;
    std::string  strTypeName;
    //get Subscription-State
    if (pstSipMsgDe->getHdrSubsSta( &ucStaType,strTypeName,	&iExpires))
    {
        TLOGDEBUG("SipMsg", "Subscription-State :" + strTypeName + " expires: " + std::to_string(iExpires));
    }

    //print event
    ZUCHAR ucPkgType, ucTempType;
    std::string strPkgTypeName, strTmpTypeName;
    if (pstSipMsgDe->getHdrEvent(ucPkgType,strPkgTypeName,ucTempType,strTmpTypeName))
    {
        SipMsgCommon::speak("event : " + strPkgTypeName);
    }

    // printf Record-Route Header
    strUserName = "";
    strHostName = "";
    if (pstSipMsgDe->getHdrRecordRouteUri(true, strUserName, strHostName, iPort))
    {
        TLOGDEBUG("SipMsg", "Record-Route:" + strUserName + " @ " + strHostName + " : " + std::to_string(iPort));
    }

    //print  Supported Header
    vector<int> vectSupted;
    vector<std::string > vectName;
    if (pstSipMsgDe->getHdrSupported(vectSupted, vectName))
    {
        std::string supported = "Supprted :";
        for (unsigned int i = 0; i < vectName.size(); i++)
        {
            supported += vectName.at(i) + " ";
        }
        TLOGDEBUG("SipMsg", supported);
    }

    //print P-Called-Party-ID
    strUserName = "";
    strHostName = "";
    if (pstSipMsgDe->getHdrPCalledPartyID(strUserName, strHostName, iPort))
    {
        TLOGDEBUG("SipMsg", "P-Called-Party-ID: " + strUserName + (strHostName.size() != 0 ? "@" + strHostName : ""));
    }

    //print Contribution-ID
    std::string strContrId;
    if (pstSipMsgDe->getHdrContributionID(strContrId))
    {
        TLOGDEBUG("SipMsg", "Contribution-ID: " + strContrId);
    }

    //print Conversation-ID
    std::string strConvId;
    if (pstSipMsgDe->getHdrConversationID(strConvId))
    {
        TLOGDEBUG("SipMsg", "Conversation-ID: " + strConvId);
    }

    SipMsgCommon::speak("\r\n");
    return true;
}

bool SipMsg::printEncodeMsg(const SipMsgEncodePtr *pstSipMsgEn)
{
    return true;
}


////xjp add test
bool SipMsg::createFirstRegister_xjp(SipMsgEncodePtr &pstEnSipMsg)
{
    if (!pstEnSipMsg )
        pstEnSipMsg = std::make_shared<SipMsgEncode>();

    std::string strBranch, strUserInfo, strHostname, strLocalIp;
    int iLocalPort, iRemotePort, iRport, iPort;
    unsigned char ucMethod, ucTptType;
    vector<int> viSuptFlag;
    vector<int> viAllowFlag;

    ST_SIP_SIP_URI stSipUri;
    ST_ZOS_SSTR stUserInfo;
    ST_SIP_HOST_PORT stHostPort;
    ST_ZOS_SSTR stTag;
    ST_ZOS_SSTR stHostName;
    std::string strTag = tars::TC_UUIDGenerator::getInstance()->genID();

    if (!pstEnSipMsg->createReq())
    {
        return false;
    }

    strUserInfo = "13957854053";
    strHostname = "gd.ims.mnc000.mcc460.3gppnetwork.org";
    iLocalPort = 53890;
    strLocalIp = "192.168.8.100";
    iRemotePort = 5060;
    iPort = 0;
    ucTptType = EN_SIP_TPT_PARM_TCP;
    ucMethod = EN_SIP_METHOD_REGISTER;

    //fill req line
    pstEnSipMsg->fillReqLineBySip(ucMethod, strUserInfo, strHostname, iRemotePort);

    //fill from heard
    SipMsgCommon::str2ZosSstr(strUserInfo,stUserInfo);        //strUser to srUser
    SipMsgCommon::str2ZosSstr(strHostname,stHostName);
    SipMsgCommon::enHostPort(strHostname,iRemotePort,stHostPort);
    SipMsgCommon::str2ZosSstr(strTag,stTag);
    Sip_ParmFillSipUri(pstEnSipMsg->_pstMsg->zMemBuf, &stSipUri, &stUserInfo,&stHostPort);		//generate SipUri obj
    pstEnSipMsg->fillHdrFromOrToBySipUri(true,&stUserInfo,&stSipUri,&stTag);

    //fill to heard
    pstEnSipMsg->fillHdrFromOrToBySipUri(false,&stUserInfo,&stSipUri,NULL);

    //fill contact heard
    ST_SIP_ADDR_SPEC stAddrSpec;
    //ST_SIP_SIP_URI *pstSipUri
    if (Sip_ParmFillAddrSpecBySipUri(pstEnSipMsg->_pstMsg->zMemBuf, &stAddrSpec, &stSipUri) != ZOK)
        return 0;

    //pstEnSipMsg->fillHdrContactByAddrSpec(&stAddrSpec,strLocalIp, iLocalPort);
    pstEnSipMsg->fillHdrContact(strUserInfo,strLocalIp,iLocalPort,EN_SIP_TPT_PARM_TCP);

    //add contact expires
    pstEnSipMsg->ParmFillContactExpires( 5800);
    pstEnSipMsg->parmFillContact(CT_CAP_IM | CT_CAP_VIDEOCALL | CT_CAP_VOICECALL);

    // fill Via header
    ucTptType = EN_SIP_TPT_UDP;
    strHostname = "192.168.2.97";
    iLocalPort = SIP_PROXY_DEFAULT_SIP_LOCAL_PORT;
    iRport = 0;
    strBranch = SipMsgCommon::createBranch();
    pstEnSipMsg->fillHdrVia(ucTptType, strHostname, iLocalPort, true,iRport, strBranch);


    //fill allow header
    viAllowFlag.push_back(SIP_ALLOW_HDR_ADD_ACK);
    viAllowFlag.push_back(SIP_ALLOW_HDR_ADD_INFO);
    pstEnSipMsg->fillHdrAllow(viAllowFlag);

    //fill supported header
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_ARC);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_EVNTLST);
    pstEnSipMsg->fillHdrSupted(viSuptFlag);

    //fill author info
    StSipAuthor stAuth;
    stAuth._ucDigst = 1;
    stAuth._strUsername = "460007894556039";
    stAuth._strRealm = "gd.ims.mnc000.mcc460.3gppnetwork.org";
    stAuth._strNonce = "";
    stAuth._strResponse = "";
    stAuth._ucAlgo = EN_SIP_ALGO_VAL_MD5;
    stAuth._strNc = "120245";

    SipMsgCommon::str2ZosSstr(strUserInfo, stUserInfo);
    SipMsgCommon::enHostPort(strHostname, iRport, stHostPort);
    //Sip_ParmFillSipUri(pstEnSipMsg->_pstMsg->zMemBuf, &stSipUri, &stUserInfo, &stHostPort); /* fill stSipUri */
    stSipUri.ucUserInfoPres = 0;
    //EN_SIP_REQ_URI_SIP
    stAuth._stReqUri.ucPres = 1;
    stAuth._stReqUri.ucType = EN_SIP_REQ_URI_SIP;
    stAuth._stReqUri.u.stSipUri = stSipUri;

    pstEnSipMsg->fillHdrAuthor(stAuth);

    //fill call-id header
    std::string strCallNum = tars::TC_UUIDGenerator::getInstance()->genID();
    pstEnSipMsg->fillHdrCallId(strCallNum, strLocalIp);


    //fill Cseq header
    pstEnSipMsg->fillHdrCseq(EN_SIP_METHOD_REGISTER,0);

    //* fill Expires header */
    int dwExpires = 3600;
    pstEnSipMsg->fillHdrExpires(dwExpires);

    //* fill Allow header */
    viAllowFlag.push_back(SIP_ALLOW_HDR_ADD_ACK);
    viAllowFlag.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    pstEnSipMsg->fillHdrAllow(viAllowFlag);

    // fill supported tag
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstEnSipMsg->fillHdrSupted(viSuptFlag);

    /* fill User-Agent header */
    std::string strName = "Yes";
    std::string strVersion = "Life/1.2.86";
    std::string strComm = "Android 4.4.2";
    pstEnSipMsg->fillHdrUserAgent(strName, strVersion, strComm);

    return pstEnSipMsg->encode();
}

/* create Notify sip msg by 200 OK of  Subscribe response */
bool SipMsg::createNotifyBySub200(const SipMsgDecodePtr &pstDeSipMsg, SipMsgEncodePtr &pstEnSipMsg)
{
    int iResStatus = 0, iLocalPort = 0, iSeq = 0, imaxForward = 0, iSubStaExpire = 1832;
    unsigned char ucMethod;
    ST_SIP_SIP_URI stFromSipUri,*pstSipUri = NULL, stContactSipUri;
    std::string strFromTag, strDispName, strHostName, strLocalIp, strViaUri,strUserName,strDeContactUri,strViaBranch,  strToTag;
    ST_ZOS_SSTR stDispName, stUserInfo;
    ST_SIP_ADDR_SPEC stAddrSpec;
    ST_SIP_NA_SPEC  stNaSpec;
    vector<int> vecAllow;
    vector<int> vecSupport;
    ST_SIP_CALLID stCallId;
    int lDowrt = 0;

    strLocalIp = "192.168.102.111";
    iLocalPort = 5050;
    //input parameters check
    if (! pstDeSipMsg->getRspStatusCode2(iResStatus))
        return false;
    if ( !pstDeSipMsg->getMethod(ucMethod))
        return false;

    if (iResStatus != 200 || ucMethod != EN_SIP_METHOD_SUBS || pstDeSipMsg->isRequest())
    {
        TLOGWARN("SipMsg", "dwCode != 200, can not create notify");
        return false;
    }

    ucMethod = EN_SIP_METHOD_NOTIFY;

    if (!pstEnSipMsg)
        pstEnSipMsg = std::make_shared<SipMsgEncode>();
    pstEnSipMsg->createReq();

    //fill req line
    pstDeSipMsg->getHdrViaUri(strViaUri, lDowrt);

    pstDeSipMsg->getFromToAddrSpec(true,stAddrSpec);
    SipMsgCommon::getFromToNaSpec(pstDeSipMsg->_pstMsg,true, stNaSpec);
    stFromSipUri = stAddrSpec.u.stSipUri;
    pstDeSipMsg->getHdrFromToUserInfo(true, stUserInfo);


    stDispName =  stNaSpec.u.stNameAddr.stDispName.u.stName;
    SipMsgCommon::zosSstr2Str(stDispName,strDispName);
    stAddrSpec.u.stSipUri.ucUserInfoPres = 0;
    pstEnSipMsg->fillReqLineByIp(EN_SIP_METHOD_NOTIFY,&stUserInfo,strViaUri,lDowrt);

    SipMsgCommon::getReqSipsUri(pstEnSipMsg->_pstMsg,&pstSipUri);
    pstEnSipMsg->parmFillSipUriTpt(pstEnSipMsg->_pstMsg->zMemBuf, pstSipUri,EN_SIP_TPT_PARM_TCP);

    // fill via Header
    pstDeSipMsg->getHdrContactUri(strDeContactUri,lDowrt);
    strViaBranch = SipMsgCommon::createBranch();
    pstEnSipMsg->fillHdrVia(EN_SIP_TPT_TCP,strDeContactUri,lDowrt,true, 0,strViaBranch);

    //fill From Header
    pstDeSipMsg->getHdrToSipUri(strDispName,strUserName,strHostName,lDowrt);

    pstDeSipMsg->getHdrFromToTag(true,strFromTag);
    pstDeSipMsg->getHdrFromToTag(false,strToTag);
    pstEnSipMsg->fillHdrFromToByName(true, strDispName,strUserName,strHostName,0,strToTag);

    //fill To Header
    pstEnSipMsg->fillHdrFromToByName(false, strDispName,strUserName,strHostName,0,strFromTag);

    //fill Call-Id Header
    pstDeSipMsg->getHdrCallId(stCallId);
    pstEnSipMsg->fillHdrCallId(&stCallId);

    //fill Cseq Header
    pstEnSipMsg->fillHdrCseq(ucMethod,iSeq + 1);

    //fill Event Header
    //ST_SIP_HDR_EVNT
    pstEnSipMsg->fillHdrEvnt(EN_SIP_EVNT_PKG_REG, EN_SIP_EVNT_TEMP_OTHER);

    // fill Subscription-State Header
    pstEnSipMsg->fillHdrSubsSta(EN_SIP_SUBSTA_VAL_ACTIVE);
    pstEnSipMsg->HdrSubsStaAddExpires( iSubStaExpire);

    // fill Max-forwards Header
    pstEnSipMsg->fillHdrForwards(69);

    //fill Contact Header
    std::string   strNumber;
    EN_SIP_ADDR_SPEC  stAddrType = EN_SIP_ADDR_SPEC_SIP;
    if (pstDeSipMsg->getHdrContactSipUri(stContactSipUri,strNumber, stAddrType))
    {
        if (  stAddrType == EN_SIP_ADDR_SPEC_SIPS || stAddrType == EN_SIP_ADDR_SPEC_SIP)
        {
            pstEnSipMsg->fillHdrContactBySipUri(stContactSipUri);
        }
        else if(stAddrType == EN_SIP_ADDR_SPEC_TEL)
        {
            pstEnSipMsg->fillHdrContactByTel("",strNumber);
        }
    }

    //fill Content-Disposition Header
    std::string strTem("notification");
    pstEnSipMsg->fillHdrContentDisp(strTem);

    //fill Content-type Header
    pstEnSipMsg->fillHdrContentType(EN_SIP_MTYPE_DISC_APP,"",EN_SIP_MSUBTYPE_REGINFO_XML,"");

    //暂时不支持xml消息体，linux下的libAvatar.a 应该是缺少xml部分

    ////fill body   application/reginfo+xml
    //ST_DOM_DOC *pstDoc;
    //ZDBUF zMsgBuf;
    //char chXmlStr[8096];
    //int iXmlLen;
    ////* create the xml document, using 4K buffer */
    //int result = pstEnSipMsg->fillReginfoXmlBody(&pstDoc,  &zMsgBuf, chXmlStr, iXmlLen);     //generate xml data

    ////* fill body data */
    //if ( !pstEnSipMsg->fillBodyData(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_REGINFO_XML,zMsgBuf))
    //{
    //
    //	ZOS_DBUF_DELETE(zMsgBuf);
    //	Dom_DocDelete(pstDoc);
    //	return ZFAILED;
    //}

    ////* delete the doc */
    //Dom_DocDelete(pstDoc);

    ////* delete the buffer */
    //ZOS_DBUF_DELETE(zMsgBuf);

    return pstEnSipMsg->encode();
}

/* create the ACK of invite resp 200 */
bool  SipMsg::createAck(const SipMsgDecodePtr &pstDeSipMsg, SipMsgEncodePtr &pstEnSipMsg)
{
    int iResStatus = 0, iSeqNum = 0;
    ZUCHAR ucMethod;
    ST_SIP_SIP_URI stContactSipUri;
    std::string strViaIp,strUserName;
    int dwPort;
    ST_ZOS_SSTR stUserInfo;

    //input parameters check
    if (!pstDeSipMsg->getRspStatusCode2(iResStatus))
        return false;
    if ( !pstDeSipMsg->getMethod(ucMethod))
        return false;

    if (iResStatus != 200 || ucMethod != EN_SIP_METHOD_INVITE || pstDeSipMsg->isRequest())
    {
        TLOGWARN("SipMsg", "dwCode != 200, can not create ACK");
        return false;
    }
    ucMethod = EN_SIP_METHOD_ACK;

    if (!pstEnSipMsg)
        pstEnSipMsg = std::make_shared<SipMsgEncode>();
    pstEnSipMsg->createReq();

    //get contact sipuri
    std::string   strNumber;
    EN_SIP_ADDR_SPEC  stAddrType = EN_SIP_ADDR_SPEC_SIP;
    if (pstDeSipMsg->getHdrContactSipUri(stContactSipUri,strNumber, stAddrType))
    {
        if (  stAddrType == EN_SIP_ADDR_SPEC_SIPS || stAddrType == EN_SIP_ADDR_SPEC_SIP)
        {
            //fill Req line
            pstEnSipMsg->fillReqLineBySipUri(ucMethod, &stContactSipUri);
        }
        else if(stAddrType == EN_SIP_ADDR_SPEC_TEL)
        {
            //fill Req lin
            pstEnSipMsg->fillReqLineByTel(ucMethod, strNumber);
        }
    }

    //copy from header
    pstEnSipMsg->CpyHdrFromMsg(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg,EN_SIP_HDR_FROM);

    //copy to heaeder
    pstEnSipMsg->CpyHdrFromMsg(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg,EN_SIP_HDR_TO);

    //Fill Contact Heaeder
    pstDeSipMsg->getHdrViaUri(strViaIp, dwPort);
    pstDeSipMsg->getHdrFromToUserInfo(true,stUserInfo);
    SipMsgCommon::zosSstr2Str(stUserInfo,strUserName);
    pstEnSipMsg->fillHdrContact(strUserName,strViaIp,dwPort, EN_SIP_URI_PARM_OTHER + 1);
    //add +sip.instance
    ST_ZOS_SSTR sipInstance;
    std::string str("urn:uuid:95CAE616-2C2E-466B-BE73-87BD2BFC3C64");
    SipMsgCommon::str2ZosSstr(str, sipInstance);
    pstEnSipMsg->ParmFillContactSipInstance(&sipInstance);

    //fill header   RecorRoute  To  Route
    pstEnSipMsg->CopyRecorRouteToRoute(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg);

    //copy Call-Id heaeder
    pstEnSipMsg->CpyHdrFromMsg(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg,EN_SIP_HDR_CALL_ID);

    //fill CSeq Header
    pstDeSipMsg->getHdrCseq(iSeqNum);
    pstEnSipMsg->fillHdrCseq(ucMethod,iSeqNum);

    //fill Max-forwards
    pstEnSipMsg->fillHdrMaxForwards(70);

    //fill User-Agent
    pstEnSipMsg->fillHdrUserAgent("CPM-client", "OMA1.0", "");
    pstEnSipMsg->fillHdrUserAgent("MSP", "1.0", "");

    //fill Via Header
    std::string strViaBrach = SipMsgCommon::createBranch();
    pstEnSipMsg->fillHdrVia(EN_SIP_TPT_TCP,strViaIp,dwPort,true, 0,strViaBrach);

    return pstEnSipMsg->encode();
}

/* decode the Notify */
bool SipMsg::decodeNotify(const SipMsgDecodePtr &pstDeSipMsg)
{
    ZUCHAR  ucStaType, ucPkgType, ucTempType;
    ZUINT  iExpires;
    std::string  strTypeName, strPkgTypeName, strTmpTypeName;
    //get Subscription-State
    pstDeSipMsg->getHdrSubsSta( &ucStaType,strTypeName,	&iExpires);

    pstDeSipMsg->getHdrEvent(ucPkgType,strPkgTypeName,ucTempType,strTmpTypeName);
    return true;
}

/**
*
*
* @param [out] pstEnSipMsg.
*/
bool SipMsg::createMessage(SipMsgEncodePtr &pstEnSipMsg)
{
    std::string strToUserName, strFromUserName, strHostName, strViaIp;
    int iPort = 0, iResult = 0;
    ZUCHAR ucMethod = EN_SIP_METHOD_MSG;
    std::string strTag, strBranch, strSc, strVal ;
    ST_SIP_NA_SPEC stNaSpec;
    ST_SIP_SIP_URI *pstSipUri;

    if (!pstEnSipMsg)
        pstEnSipMsg = std::make_shared<SipMsgEncode>();
    pstEnSipMsg->createReq();

    strTag = tars::TC_UUIDGenerator::getInstance()->genID();
    //fill req line
    strToUserName = "+8613957854053";
    strFromUserName = "+8615888196659";
    strHostName = "zj.ims.mnc000.mcc460.3gppnetwork.org";
    pstEnSipMsg->fillReqLineBySip(ucMethod, strToUserName, strHostName, 0);

    //fill Via Header
    strViaIp = "117.136.239.32";
    iPort = 5560;
    strBranch = SipMsgCommon::createBranch();
    pstEnSipMsg->fillHdrVia(EN_SIP_TPT_TCP, strViaIp, iPort,false,0,strBranch);

    // fill from Header
    pstEnSipMsg->fillHdrFromToByName(true,strFromUserName,strFromUserName,strHostName,0,strTag);
    //fill to Header
    pstEnSipMsg->fillHdrFromToByName(false,"",strToUserName,strHostName,0,"");

    //fill P-Preferred-Identity Header
    //pstEnSipMsg->fillHdrPPreferredIdentity(strFromUserName);
    pstEnSipMsg->fillHdrPPreferredIdentity("","+8613911826426", "ebupt.com", 0);

    SipMsgCommon::getReqSipsUri(pstEnSipMsg->_pstMsg,&pstSipUri);
    if (pstSipUri != 0)
    {
        pstEnSipMsg->parmFillSipUriUSer(pstEnSipMsg->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
    }
    //fill P-Called-Party-ID Header
    strSc = "sc";
    strVal = "497";
    pstEnSipMsg->fillHdrPCalledPartyIdByTel(strToUserName, strSc, strVal);

    //fill P-Asserted-Identity Header

    SipMsgCommon::getFromToNaSpec(pstEnSipMsg->_pstMsg,true,stNaSpec);
    pstEnSipMsg->fillHdrPAssertedIdentity(stNaSpec);

    //fill P-Asserted-Service Header
    std::string strTem = "urn-7:3gpp-service.ims.icsi.oma.cpm.msg";
    pstEnSipMsg->fillHdrPAssertedService(strTem);

    // fill Contribution-ID Header
    std::string strContbtId = "HPdcbW81u48Qavgc";
    pstEnSipMsg->fillHdrContributionID(strContbtId);

    //fill Conversation-ID Header
    //EN_SIP_HDR_CONVERSATION_ID
    //ST_SIP_MSG
    std::string strConversId = "GOdcbW81u48Qavgc";
    pstEnSipMsg->fillHdrConversationID(strConversId);

    //fill Date Header
    pstEnSipMsg->fillHdrDate();

    /*fill Accept-Contact Header */
    vector<int> paramFlags;
    map<std::string ,std::string> geneParas;
    geneParas.insert(make_pair("+g.3gpp.icsi-ref", "urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"));
    pstEnSipMsg->fillHdrAcceptContact(paramFlags, geneParas);

    //fill Content-Type Header
    pstEnSipMsg->fillHdrContentType(EN_SIP_MTYPE_COMP_MSG, "", EN_SIP_MSUBTYPE_CPIM,"");
    char * pcBoundry = (char*)"SIP_BOUNDARY";
    int iBoundLen = strlen(pcBoundry);
    //pstEnSipMsg->fillHdrContentTypeX(EN_SIP_MTYPE_COMP_MSG, "", EN_SIP_MSUBTYPE_CPIM,"",pcBoundry ,iBoundLen );

    //fill Body ...暂时缺着，难搞
    std::string strTemMsgBody,strFromDisp,strFromUri,strToDisp,strToUri;

    //SipMsgCommon::str2ZosSstr(strBody, stBodyStr);
    pstEnSipMsg->fillBodySDPDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, CONST_SDP_STRING);      //有问题！！！！！

    //ZCHAR *pcData = NULL, ucMtype, ucMSubtype, *pcContType = NULL;
    //ST_ZOS_LSSTR stMsg;
    //ZUINT iDataLen;
    //strTemMsgBody = "今晚打老虎，试试好不好？";
    ///* fill cpim/imdn msg */
    //ST_MSF_URI   stLclUri ,stPartpUri;
    //strFromDisp = "8613957854053";
    //strFromUri =  "sip:+8615888196659@zj.ims.mnc000.mcc460.3gppnetwork.org";
    //strToDisp = "";
    //strToUri = "sip:+8613957854053@zj.ims.mnc000.mcc460.3gppnetwork.org";
    //SipMsgCommon::enMsfUri(strFromDisp, strFromUri, stLclUri);
    //SipMsgCommon::enMsfUri(strToDisp,strToUri  , stPartpUri);
    //int strMsgLen = strTemMsgBody.size();

    //pcContType = "text/plain";
    //iResult =  SipMsgCommon::SessFillImdnData(pstEnSipMsg->_pstMsg, &stLclUri, &stPartpUri, (char*)strTemMsgBody.c_str(), strMsgLen,
    //	pcContType, &pcData, &iDataLen);

    //暂时注释掉xml 消息部分,libAvatar.a 好像不支持

    //pcContType = "message/imdn+xml";
    ////fill body   application/reginfo+xml
    //ST_DOM_DOC *pstDoc;
    //ZDBUF zMsgBuf;
    //char chXmlStr[8096];
    //int iXmlLen;
    ////* create the xml document, using 4K buffer */
    //int result = pstEnSipMsg->fillReginfoXmlBody(&pstDoc,  &zMsgBuf, chXmlStr, iXmlLen);     //generate xml data

    //iResult =  SipMsgCommon::SessFillImdnData(pstEnSipMsg->_pstMsg, &stLclUri, &stPartpUri, chXmlStr, iXmlLen, pcContType, &pcData, &iDataLen);

    //Dom_DocDelete(pstDoc);						//* delete the doc */
    //ZOS_DBUF_DELETE(zMsgBuf);					//* delete the buffer */

    //
    ///* set the cpim msg */
    //ucMtype = EN_SIP_MTYPE_COMP_MSG;
    //ucMSubtype = EN_SIP_MSUBTYPE_CPIM;
    ////ZOS_SSTR_SET(&stMsg, pcData, iDataLen);
    //ZOS_LSSTR_SET(&stMsg, pcData, iDataLen);

    ///* set the body message */
    //ST_ZOS_LSSTR stBodyStr;
    //std::string strBody(pcData);
    //SipMsgCommon::str2ZosLSstr(strBody,stBodyStr);
    //iResult = pstEnSipMsg->fillBodyDataStr(ucMtype, ucMSubtype,&stBodyStr);

    //pstEnSipMsg->fillHdrRoute()

    return pstEnSipMsg->encode();
}

bool SipMsg::createBye(const SipMsgDecodePtr &pstDeSipMsg, SipMsgEncodePtr &pstEnSipMsg)
{
    int iSeqNum = 0;
    ZUCHAR ucMethod;
    ST_SIP_SIP_URI stContactSipUri;
    std::string strViaIp,strUserName;
    ZULONG dwPort = 0;

    //input parameters check
    if ( !pstDeSipMsg->getMethod(ucMethod))
        return false;

    if (ucMethod != EN_SIP_METHOD_ACK )
    {
        TLOGWARN("SipMsg", "dwCode != 200, can not create BYE");
        return false;
    }

    ucMethod = EN_SIP_METHOD_BYE;

    if (!pstEnSipMsg)
        pstEnSipMsg = std::make_shared<SipMsgEncode>();
    pstEnSipMsg->createReq();

    //get contact sipuri
    std::string   strNumber;
    EN_SIP_ADDR_SPEC  stAddrType = EN_SIP_ADDR_SPEC_SIP;
    if (pstDeSipMsg->getHdrContactSipUri(stContactSipUri,strNumber, stAddrType))
    {
        if (  stAddrType == EN_SIP_ADDR_SPEC_SIPS || stAddrType == EN_SIP_ADDR_SPEC_SIP)
        {
            //fill Req line
            pstEnSipMsg->fillReqLineBySipUri(ucMethod, &stContactSipUri);
        }
        else if(stAddrType == EN_SIP_ADDR_SPEC_TEL)
        {
            //fill Req lin
            pstEnSipMsg->fillReqLineByTel(ucMethod, strNumber);
        }
    }

    //copy from header
    pstEnSipMsg->CpyHdrFromMsg(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg,EN_SIP_HDR_FROM);

    //copy to heaeder
    pstEnSipMsg->CpyHdrFromMsg(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg,EN_SIP_HDR_TO);

    //fill header   RecorRoute  To  Route
    pstEnSipMsg->CopyRecorRouteToRoute(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg);

    //copy Call-Id heaeder
    pstEnSipMsg->CpyHdrFromMsg(pstEnSipMsg->_pstMsg, pstDeSipMsg->_pstMsg,EN_SIP_HDR_CALL_ID);

    //fill CSeq Header
    pstDeSipMsg->getHdrCseq(iSeqNum);
    pstEnSipMsg->fillHdrCseq(ucMethod,iSeqNum + 1);

    //fill Max-forwards
    pstEnSipMsg->fillHdrMaxForwards(70);

    //fill User-Agent
    pstEnSipMsg->fillHdrUserAgent("CPM-client", "OMA1.0", "");
    pstEnSipMsg->fillHdrUserAgent("MSP", "1.0", "");

    //fill Via Header
    std::string strViaBrach = SipMsgCommon::createBranch();
    pstEnSipMsg->fillHdrVia(EN_SIP_TPT_TCP,strViaIp,dwPort,true, 0,strViaBrach);

    return pstEnSipMsg->encode();
}

bool SipMsg::createResponce(const SipMsgDecodePtr &pstSipMsgDe, SipMsgEncodePtr &pstSipMsgEn,int responceCode)
{
    std::string strBranch, strHost, strRecvHost;
    std::string strDispName, strNumber, strUserName;
    std::string strFromTag, strToTag;
    unsigned char ucTptType, ucUriType, ucMethod ;
    int iPort, iRport, iCseq;
    void *zMemBuf;

    /* check the pointer */
    if(!pstSipMsgDe) return false;

    /* init pstSipMsgEn */
    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    pstSipMsgEn->createRsp();
    zMemBuf = pstSipMsgEn->_pstMsg->zMemBuf;

    /* fill status line */
    pstSipMsgEn->fillStatusLine(responceCode);

    /* fill header via */
    pstSipMsgDe->getViaBranch(strBranch);
    pstSipMsgDe->getHdrViaOther(ucTptType, strHost, iPort, strRecvHost, iRport);
    pstSipMsgEn->fillHdrVia(ucTptType, strHost, iPort,true, iRport, strBranch);

    /* fill header to */
    pstSipMsgDe->getHdrToTelUri(strDispName, strNumber);
    strToTag = SipMsgCommon::createToTag();
    if (strNumber.empty())
        strNumber = "1111111000000";

    pstSipMsgEn->fillHdrFromToTelUri(false, strDispName, strNumber,strToTag);

    /* fill header from */
    pstSipMsgDe->getHdrFromToTag(true, strFromTag);
    pstSipMsgDe->getFromUri(strDispName, strUserName, strHost, iPort, ucUriType);
    pstSipMsgEn->fillHdrFromToByName(true, strDispName, strUserName, strHost, iPort, strFromTag, ucUriType);


    /* fill header call-id */
    pstSipMsgDe->getCallId(strNumber, strHost);
    pstSipMsgEn->fillHdrCallId(strNumber, strHost);

    /* fill header cseq */
    pstSipMsgDe->getMethod(ucMethod);
    pstSipMsgDe->getHdrCseq(iCseq);
    pstSipMsgEn->fillHdrCseq(ucMethod, iCseq);

    /* fill header contact */
    pstSipMsgEn->fillHdrContact("Anonymous", "117.136.239.32", 5460, EN_SIP_TPT_PARM_TCP);

    /* fill header Record-Route */
    pstSipMsgEn->fillHdrRecordRoute("DIAG_13_0_0d00002c", "117.136.239.32", 5460, EN_SIP_TPT_PARM_TCP);

    /* fill header supported */
    vector<int> iSuptFlagVec;
    iSuptFlagVec.push_back(SIP_TAG_OPT_ADD_100REL);
    iSuptFlagVec.push_back(SIP_TAG_OPT_ADD_REPLACES);
    pstSipMsgEn->fillHdrSupted(iSuptFlagVec);

    /* fill header Session-Expires */
    pstSipMsgEn->fillHdrSessExpires(true, true, 1800);

    /* Privacy */
    pstSipMsgEn->fillHdrPrivacy(EN_SIP_PRIV_VAL_ID);

    /* fill header allow */
    iSuptFlagVec.clear();
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_REGISTER);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_ACK);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_BYE);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_REFER);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_INFO);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_MSG);
    iSuptFlagVec.push_back(SIP_ALLOW_HDR_ADD_SUBS);
    pstSipMsgEn->fillHdrAllow(iSuptFlagVec);

    ///* fill header server */
    pstSipMsgEn->fillHdrServer("CPM-client", "OMA1.0", "MSP/1.0");

    ///* fill header content-type */
    pstSipMsgEn->fillHdrContentType(EN_SIP_MTYPE_DISC_APP, "", EN_SIP_MSUBTYPE_SDP, "");

    return pstSipMsgEn->encode();
}

bool SipMsg::decodeInviteResponce(const std::string & strBuffer)
{
    SipMsgDecodePtr pstSipMsgDe = 0;
    SipGateway::SipDialogPtr pstSipDialog = std::make_shared<SipGateway::SipDialog>(0);
    unsigned int ulStatusCode = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strDispName, strUserName, strHost, strSDPBody;
    SipMsgDecodedPart * pstDecodedPart = 0;
    SipProxy::SipRegisterSubscribePtr pRegisterSub = 0;
    ST_SIP_DIGEST stDigest;
    unsigned int ulRseqNum = 0;
    bool existSDP = false, refresherPre = false,  uasRefresh = false;
    int iPort = 0, seconds = 0;
    unsigned char ucMethod = EN_SIP_METHOD_INVITE, ucTptType = 0, ucAddrType = 0;
    ST_SIP_SIP_URI  stSipUri;
    int dwPort = 0;
    std::string  strHostName;
    ZCHAR  zcTpt;

    pstSipMsgDe = std::make_shared<SipMsgDecode>();
    ucTptType = EN_SIP_TPT_UDP;

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", "dealInviteResponce() call. Sip msg is not valid or complete. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //resp header
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    std::string strRspReason, strSpeak;
    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    pstSipMsgDe->getRspReason(strRspReason);

    pstDecodedPart->_bReqPres = false;
    pstDecodedPart->_statusLinePart._iCode = ulStatusCode;
    pstDecodedPart->_statusLinePart._strReason = strRspReason;

    //digest challenge
    if (407 == ulStatusCode || 401 == ulStatusCode)
    {
        pRegisterSub = pstSipDialog->_sipRegisterSub;
        if (pRegisterSub != 0)
        {
            if (pstSipMsgDe->getDigest(stDigest, ulStatusCode))
            {
                pRegisterSub->setDigest(stDigest);
                pstSipMsgDe->decodeChallengeDigest(stDigest, pRegisterSub);
            }
        }
    }
    else if (180 == ulStatusCode || 200 == ulStatusCode || 183 == ulStatusCode)
    {
        //RPC呼叫SIP，收到的407等回复To tag与180不一致，而BYE应该取180/200 的to tag
        //get To Header
        strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
        if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
        {
            pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName, strHost, iPort, strRemoteTag, ucTptType);
        }

        //get Contact Header
        if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt ))
        {
            //收到 180 或者200 响应，填写被叫 To 的Contact
            pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);
        }
    }
    else
    {
    }

    //decode RSeq Header
    if (pstSipMsgDe->getRseq(ulRseqNum))
    {
        pstDecodedPart->_rseqPart._respNum = ulRseqNum;
    }

    //get SDP body
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod,  false,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strSDPBody);
        }
        else
        {
            TLOGDEBUG("SipMsg", "dealInviteResponce() called. checkSipMsgWithSDP() return EMPTY body! SipDialogId "
                + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        }
    }

    //get Session-Expires Header
    if (pstSipMsgDe->getHdrSessionExpires(seconds, refresherPre, uasRefresh))
    {
        int64_t curTime = TNOWMS;
        pstSipDialog->_sipMsgDecodePart._sessionExpiresPart.setSessionExpiresPart(seconds,curTime,refresherPre, uasRefresh);

        if (ulStatusCode == 200)
        {
            if (refresherPre && !uasRefresh)
            {
                pstSipDialog->setIsFreshDialog(true);
            }
        }
    }
    bool resur2 = pstSipMsgDe->getHdrRecordRouteSipUri(true, stSipUri);

    //get Record-Route
    EN_SIP_URI_PARM  uriParamType = (EN_SIP_URI_PARM)-1;
    unsigned char value1 = 0;
    unsigned int  value2 = 0;
    std::string  value3;

    resur2 = pstSipMsgDe->getHdrRecordRouteUri(true, strUserName, strHostName, dwPort, uriParamType, value1, value2, value3);

    return true;
}

bool SipMsg::decodeSipMsg(const std::string & strBuffer, SipGateway::SipDialogPtr & pstSipDialog)
{
    SipMsgDecodePtr pstSipMsgDe = 0;
    SipMsgEncodePtr pstSipMsgEn = 0;
    if (0 == pstSipDialog)
    {
        pstSipDialog = std::make_shared<SipGateway::SipDialog>(0);
    }

    SipGateway::SipDialogIdPtr sptSipDialogId = 0;
    int   iSceq = 0;
    unsigned int ulStatusCode = 0;
    unsigned char ucMethod, ucTptType;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true;           //Request present flag;
    SipMsgDecodedPart * pstDecodedPart = 0;
    SipProxy::SipRegisterSubscribePtr pRegisterSub = 0;
    ST_SIP_DIGEST stDigest;
    pstSipMsgDe = std::make_shared<SipMsgDecode>();
    unsigned int ulRseqNum = 0;
    bool bSipToRpc = false;

    ucTptType = EN_SIP_TPT_UDP;
    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;

    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", "onReiceivedSipMsg() call getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    bReqPres = pstSipMsgDe->isRequest() ? true : false;

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //check Request or responce
    if(bReqPres)
    {
        pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
        //Set ReqLine DecodePart
        std::string strUserInfo, strHostname, strSpeak;
        int iPort, iReqUriType;
        pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
        pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);
        pstDecodedPart->_bReqPres = true;

    }
    else /* print response line */
    {
        pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
        std::string strRspReason, strSpeak;
        pstSipMsgDe->getRspStatusCode(ulStatusCode);
        pstSipMsgDe->getRspReason(strRspReason);

        pstDecodedPart->_bReqPres = false;
        pstDecodedPart->_statusLinePart._iCode = ulStatusCode;
        pstDecodedPart->_statusLinePart._strReason = strRspReason;
    }

    //get From Header ====
    std::string strDispName, strUserName, strHost;
    int  iPort = 0;
    unsigned char ucAddrType;
    {
        //如果 从RPC向WinSip发起INVITE呼叫，由WinSip结束，发BYE，则WinSip在组建BYE时，此时，与最初的INVITE的相比，From和To会因为 主被叫各自所记录的对话状态，即 local tag ,remote tag 等消息的相反，导致To和From相反
        //因此，暂时，接受BYE时，不修改本端状态
        if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
        {
            pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
        }

        {
            //RPC呼叫SIP，收到的407等回复To tag与180不一致，而BYE应该取180/200 的to tag
            //get To Header
            pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
            strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
            if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
            {
                pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
            }
        }
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        if ( bReqPres || ( !bReqPres && ucMethod == EN_SIP_METHOD_INVITE))
        {
            if (iCseq == 0)
            {
                iCseq = SipMsgCommon::CreateRandInt();
            }
            pstDecodedPart->_cseqPart._iCseqVal = iCseq;
            pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
        }
    }

    {
        //RPC呼叫SIP，收到的407等回复To tag与180不一致，而BYE应该取180/200 的to tag
        //get Via Header
        /*std::string strViaUri, strViaBrach, strRecvHost;
        int iRport = 0;
        pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri,iPort, strRecvHost,iRport);
        if (pstSipMsgDe->getViaBranch(strViaBrach))
        {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
        }
        pstDecodedPart->_viaPart.SetViaPart(strViaUri, iPort,  ucTptType, false,  iRport,strViaBrach);*/

    }

    //get Via Header
    vector<ViaPart> viaHeaders;
    bool existAllowUpdate = pstSipMsgDe->getAllowOfMethod(EN_SIP_METHOD_UPDATE);

    //get Max-Forward Header
    int iMaxForwards = 0;
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
        pstDecodedPart->_MaxForwards = iMaxForwards;

    //get Supported Header
    vector<int> vectSuptFlags;
    vector<std::string> vectTypeName;
    if (pstSipMsgDe->getHdrSupported(vectSuptFlags, vectTypeName))
    {
        pstDecodedPart->_vectSuptFlag = vectSuptFlags;
    }

    //get Allow Header
    vector<int> vectAllow;
    vectTypeName.clear();
    if (pstSipMsgDe->getHdrAllow(vectAllow, vectTypeName))
    {
        pstDecodedPart->_vectAllowFlag = vectAllow;
    }

    //get Contact Header
    std::string    strHostName;
    ZCHAR  zcTpt;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt ))
    {
        if(bReqPres && ucMethod == EN_SIP_METHOD_INVITE)
        {
            pstDecodedPart->_fromPart.setFromToContact(strHostName, iPort);             //set From Cantact
        }
        else if (!bReqPres && (ulStatusCode == 200 || ulStatusCode == 180))
        {
            //收到 180 或者200 响应，填写被叫 To 的Contact
            //填写主叫的Contact 地址
            pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);               //set To Cantact
        }
        else if (bReqPres && ucMethod == EN_SIP_METHOD_BYE)
        {
            pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);               //set To Cantact
        }
    }

    //decode 401/407 challenge
    if (!bReqPres)
    {
        pRegisterSub = pstSipDialog->_sipRegisterSub;
        if (pRegisterSub != 0)
        {
            if (ulStatusCode == 401 || ulStatusCode == 407)
            {
                if (pstSipMsgDe->getDigest(stDigest, ulStatusCode))
                {
                    pRegisterSub->setDigest(stDigest);
                    pstSipMsgDe->decodeChallengeDigest(stDigest, pRegisterSub);         //decode 401/407 challenge Digest
                }
            }
        }
    }

    //decode RSeq Header
    if (pstSipMsgDe->getRseq(ulRseqNum))
    {
        pstDecodedPart->_rseqPart._respNum = ulRseqNum;
    }


    //get SDP body
    bool existSDP = false;
    std::string	strSDPBody;
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod,bReqPres,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strSDPBody);		//set 200 ok SDP as SDP body
        }
        else
        {
            TLOGDEBUG("SipMsg", "onReiceivedSipMsg() called. checkSipMsgWithSDP() return EMPTY body! SipDialogId "
                + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        }

        if (ucMethod == EN_SIP_METHOD_ACK)
        {
            pstSipDialog->setSipToRpcACKWithSDP(true);
        }
    }

    //get Session-Expires Header
    if (ucMethod == EN_SIP_METHOD_INVITE)
    {
        int seconds = 0;
        bool refresherPre = false,  uasRefresh = false;
        if (pstSipMsgDe->getHdrSessionExpires(seconds, refresherPre, uasRefresh))
        {
            int64_t curTime = TNOWMS;
            pstSipDialog->_sipMsgDecodePart._sessionExpiresPart.setSessionExpiresPart(seconds,curTime,refresherPre, uasRefresh);

            if (!bReqPres &&  ulStatusCode == 200)
            {
                if (refresherPre && !uasRefresh)
                {
                    pstSipDialog->setIsFreshDialog(true);               //uac需要定时发起刷新
                }
            }
        }
    }
    return true;
}

bool SipMsg::encodeSipMsgResponce(const SipGateway::SipDialogPtr & dlg, const SipGateway::SipResponseCodeType rspCode, const SipGateway::SipRequestMethodType reqMethod, std::string & msg)
{
    SipMsgEncodePtr pstSipMsgEn;
    SipMsgDecodedPart * pstDecodedPart = 0;
    EN_SIP_METHOD ucMethod ;
    int iStatus;
    std::string strSipGatewayId, strSipProxyid;
    SipGateway::SDPNegotiateStatus sdpStatus = SipGateway::SDPInit;
    std::string strBriefSipProxyId;


    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createRsp())
    {
        //encode responce
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. call createRsp failed .sipDialog id:" + dlg->id());
        return false;
    }

    strSipGatewayId = dlg->getSipGatewayId();
    strSipProxyid = dlg->getSipProxyId();
    SipMsg::getBriefSipProxyId(strSipProxyid, strBriefSipProxyId);

    pstDecodedPart = &dlg->_sipMsgDecodePart;

    //method converter
    ucMethod = SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);
    iStatus =  SipGateway::SipDialog::SipDialogResponseStatusToSipStatus(rspCode);

    //fill status line
    pstSipMsgEn->fillStatusLine(iStatus);

    //fill From Header
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort,
        pFromPart->_strTag, pFromPart->_ucAddrType);

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    if (pToPart->_strTag.size() == 0 && ucMethod == EN_SIP_METHOD_INVITE)
    {
        pToPart->_strTag = strSipGatewayId + "+" + strBriefSipProxyId + "+" + SipMsgCommon::createToTag();  //在TO头域的tag中标示本SipGateway服务以及SipProxy 如“SIpGateway5-1+SipProxy10-4+xcdf34d34ff2w23f”；
    }
    pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
        pToPart->_strTag, pToPart->_ucAddrType);

    ////fill Via Header

    //fill Call-Id Header
    pstSipMsgEn->fillHdrCallId(pstDecodedPart->_callIdPart._strNumber, pstDecodedPart->_callIdPart._strHost);

    //fill CSeq Header
    pstSipMsgEn->fillHdrCseq(ucMethod, pstDecodedPart->_cseqPart._iCseqVal);

    //fill Max-Forwards
    int iMaxForwards = 0;
    pstSipMsgEn->fillHdrMaxForwards(pstDecodedPart->_MaxForwards);

    //fill Contact,如何构建？ 用To直接构建TEL类型？
    //pstSipMsgEn->fillHdrContactByTel(pstDecodedPart->_toPart._strDispName, pstDecodedPart->_toPart._strUserName);
    //pstSipMsgEn->fillHdrContact(pstDecodedPart->_fromPart._strUserName,"192.168.2.125", 5060, EN_SIP_TPT_UDP);

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    pstSipMsgEn->fillHdrAllow(vectAllow);


    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);


    //fill Contact 180 或者200响应返回 Contact
    if (ucMethod == EN_SIP_METHOD_INVITE && (iStatus == 200 || iStatus == 180))
    {
        std::string strRegisterIp, strProxyIp;
        int iRegisterPort = 0, iProxyPort = 0;
        std::string sendIp;
        dlg->_sipRegisterSub->getIpsAndPorts(strProxyIp, iProxyPort, strRegisterIp, iRegisterPort, sendIp);
        if (strProxyIp.empty() || strRegisterIp.empty())
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getIpsAndPorts() reurn : strProxyIp:"
                + strProxyIp + " ,strRegisterIp:" + strRegisterIp + " ,sipDialog id:" + dlg->id());
            return false;
        }
        pstSipMsgEn->fillHdrContact(pstDecodedPart->_toPart._strUserName, strProxyIp, iProxyPort, EN_SIP_TPT_UDP);
        pstDecodedPart->_toPart.setFromToContact(strProxyIp, iProxyPort);
    }

    //fill SDP body
    if (ucMethod == EN_SIP_METHOD_INVITE && iStatus == 200)
    {
        if (dlg->getIsFreshDialog())
        {
            ST_ZOS_SSTR stBodyStr;
            std::string strOriginalSDP = dlg->_sipMsgDecodePart._bodyPart._strBody;
            SipMsgCommon::str2ZosSstr(strOriginalSDP ,stBodyStr);

            pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
        }
        else
        {
            //beacuse SIP calls RPC, so 200 OK of INVITE must contain SDP information
            if (sdpStatus != SipGateway::SDPNegotiated)
            {
                ///* set the body message */
                if (pstDecodedPart->_bodyPart._ucPres)
                {
                    ST_ZOS_SSTR stBodyStr;
                    std::string strBody(pstDecodedPart->_bodyPart._strBody);
                    SipMsgCommon::str2ZosSstr(strBody,stBodyStr);
                    pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
                }
            }
            else
            {
                TLOGWARN("SipGateway", std::string(__FUNCTION__) + "() called.SDP negotiate status wrong! sidDialog id :" + dlg->id());
            }
        }
    }

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called.encode() return FALSE! sidDialog id :" + dlg->id());
    }

    msg = pstSipMsgEn->_strMsg;
    return true;
}

bool SipMsg::getMsgCallIdHost(const std::string & sipMsg, std::string & callIdHost, bool & bReq, bool & bInvite)
{
    SipMsgDecodePtr pstSipMsgDe = 0;
    unsigned char ucMethod;
    std::string  strCallId, strCallHost;
    pstSipMsgDe = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstSipMsgDe->decode(sipMsg, sipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "()call. Sip msg is not valid or complete. SipMsg:\n" + sipMsg);
        return false;
    }
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg",  std::string(__FUNCTION__) + "()call. getMethod failed.SipMsg:\n" + sipMsg);
        return false;
    }

    bReq = pstSipMsgDe->isRequest() ? true : false;
    callIdHost = strCallId + "@" + strCallHost;
    bInvite = ucMethod == EN_SIP_METHOD_INVITE ? true : false;

    return true;
}

bool SipMsg::sipMessageValidCheck(const std::string & msg)
{
    vector<std::string> keys;
    keys.push_back("To:");
    keys.push_back("From:");
    keys.push_back("Call-ID:");
    keys.push_back("CSeq:");
    keys.push_back("Via:");
    keys.push_back("Max-Forwards:");
    keys.push_back("Contact:");
    keys.push_back("Allow:");
    keys.push_back("P-Early-Media:");
    keys.push_back("Supported:");
    keys.push_back("Content-Type:");
    keys.push_back("P-Preferred-Identity:");
    keys.push_back("Content-Length:");
    keys.push_back("Require:");

    keys.push_back("v=");
    keys.push_back("o=");
    keys.push_back("s=");
    keys.push_back("c=");
    keys.push_back("t=");
    keys.push_back("m=");
    keys.push_back("a=");

    for (unsigned int i =0; i < keys.size(); i++)
    {
        if (msg.find(keys.at(i)) > 0)
            return true;
    }
    return false;
}

bool SipMsg::__sendSipMessageResponseSipMsg(int rspCode, const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn;
    SipMsgDecodedPart * pstDecodedPart = 0;
    EN_SIP_METHOD ucMethod ;
    std::string strSipGatewayId, strSipProxyid;
    SipGateway::SDPNegotiateStatus sdpStatus = SipGateway::SDPInit;
    std::string strBriefSipProxyId;
    unsigned char tptType = EN_SIP_TPT_UDP;
    ST_SIP_SIP_URI * pstSipUri = 0;

    if (dlg == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createRsp())
    {
        //encode responce
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. call createRsp failed .sipDialog id:" + dlg->id());
        return false;
    }

    strSipGatewayId = dlg->getSipGatewayId();
    strSipProxyid = dlg->getSipProxyId();
    SipMsg::getBriefSipProxyId(strSipProxyid, strBriefSipProxyId);

    pstDecodedPart = &dlg->_sipMsgDecodePart;
    tptType = dlg->getProtocolType();

    //method converter
    ucMethod = SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);

    //fill status line
    pstSipMsgEn->fillStatusLine(rspCode);

    //fill From Header
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort,
        pFromPart->_strTag, pFromPart->_ucAddrType);

    pstSipUri = 0;
    SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,true, &pstSipUri);
    if (pstSipUri != 0)
    {
        pstSipMsgEn->fillSipUriParams(pstSipUri, pFromPart->_sipUriPrams);
    }

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    if (pToPart->_strTag.size() == 0 && ucMethod == EN_SIP_METHOD_INVITE)
    {
        pToPart->_strTag = strSipGatewayId + "+" + strBriefSipProxyId + "+" + SipMsgCommon::createToTag();  //在TO头域的tag中标示本SipGateway服务以及SipProxy 如“SIpGateway5-1+SipProxy10-4+xcdf34d34ff2w23f”；
    }
    pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
        pToPart->_strTag, pToPart->_ucAddrType);

    pstSipUri = 0;
    SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,false, &pstSipUri);
    if (pstSipUri != 0)
    {
        pstSipMsgEn->fillSipUriParams(pstSipUri, pToPart->_sipUriPrams);
    }

    //fill Via Header
    pstSipMsgEn->fillHdrVias(pstDecodedPart->_vias);

    //fill Call-Id Header
    pstSipMsgEn->fillHdrCallId(pstDecodedPart->_callIdPart._strNumber, pstDecodedPart->_callIdPart._strHost);

    //fill CSeq Header
    pstSipMsgEn->fillHdrCseq(ucMethod, pstDecodedPart->_cseqPart._iCseqVal);

    //fill Max-Forwards
    int iMaxForwards = 0;
    pstSipMsgEn->fillHdrMaxForwards(pstDecodedPart->_MaxForwards);

    //fill Contact,如何构建？ 用To直接构建TEL类型？
    //pstSipMsgEn->fillHdrContactByTel(pstDecodedPart->_toPart._strDispName, pstDecodedPart->_toPart._strUserName);
    //pstSipMsgEn->fillHdrContact(pstDecodedPart->_fromPart._strUserName,"192.168.2.125", 5060, EN_SIP_TPT_UDP);

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INFO);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_SUBS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_MSG);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_REFER);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_COMET);

    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    //fill Contact 180 或者200响应返回 Contact
    std::string strRegisterIp, strProxyIp;
    int iRegisterPort = 0, iProxyPort = 0;
    std::string sendIp;

    if (ucMethod == EN_SIP_METHOD_INVITE && (rspCode == 200 || rspCode == 180))
    {
        dlg->_sipRegisterSub->getIpsAndPorts(strProxyIp, iProxyPort, strRegisterIp, iRegisterPort, sendIp);
        if (strProxyIp.empty() || strRegisterIp.empty())
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:"+ strProxyIp + " ,strRegisterIp:" + strRegisterIp + " ,sipDialog id:" + dlg->id());
            return false;
        }
        pstSipMsgEn->fillHdrContact(pstDecodedPart->_toPart._strUserName, strProxyIp, iProxyPort, tptType);
        pstDecodedPart->_toPart.setFromToContact(strProxyIp, iProxyPort);
    }

    //fill Session -expires
    if (ucMethod == EN_SIP_METHOD_UPDATE && rspCode == 200)
    {
        dlg->_sipRegisterSub->getIpsAndPorts(strProxyIp, iProxyPort, strRegisterIp, iRegisterPort, sendIp);
        if (strProxyIp.empty() || strRegisterIp.empty())
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:"+ strProxyIp + " ,strRegisterIp:" + strRegisterIp + " ,sipDialog id:" + dlg->id());
            return false;
        }

        if (pstDecodedPart->_sessionExpiresPart._iSeconds > 0)
            pstSipMsgEn->fillHdrSessExpires(pstDecodedPart->_sessionExpiresPart._bRefreshPre, pstDecodedPart->_sessionExpiresPart._bUAS_refresh, pstDecodedPart->_sessionExpiresPart._iSeconds);

        bool ret = pstSipMsgEn->fillHdrContact(pstDecodedPart->_toPart._strUserName, strProxyIp, iProxyPort, tptType);
        if (ret && pFromPart->_contactPart._vectUriParms.size() > 0)
        {
            pstSipUri = 0;
            SipMsgCommon::getContactSipUri(pstSipMsgEn->_pstMsg, &pstSipUri);
            pstSipMsgEn->fillSipUriParams(pstSipUri, pFromPart->_contactPart._vectUriParms);
        }

        if (pstDecodedPart->_sessionExpiresPart._bRefreshPre && pstDecodedPart->_sessionExpiresPart._bUAS_refresh == false)
        {
            ZUINT iSuptedFlag = 0;
            ZOS_ZERO(iSuptedFlag);
            ZOS_SET(SIP_TAG_OPT_ADD_TMR, iSuptedFlag);
            pstSipMsgEn->fillHdrRequire(iSuptedFlag);
        }

        if (pstDecodedPart->_minSe > 0)
            pstSipMsgEn->fillHdrMinSe(pstDecodedPart->_minSe);
    }

    //fill Record-Route Header
    for (unsigned int i = 0; i < pstDecodedPart->_recordRoutes._sipUris.size(); i++)
    {
        ST_SIP_SIP_URI sipUri = pstDecodedPart->_recordRoutes._sipUris.at(i);
        pstSipMsgEn->fillHdrRecordRouteBySipUri(sipUri);
    }

    //fill SDP body
    if (ucMethod == EN_SIP_METHOD_INVITE && rspCode == 200)
    {
        if (dlg->getIsFreshDialog())
        {
            ST_ZOS_SSTR stBodyStr;
            std::string strOriginalSDP = dlg->_sipMsgDecodePart._bodyPart._strBody;
            SipMsgCommon::str2ZosSstr(strOriginalSDP ,stBodyStr);
            pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
        }
        else
        {
            //beacuse SIP calls RPC, so 200 OK of INVITE must contain SDP information
            if (sdpStatus != SipGateway::SDPNegotiated)
            {
                ///* set the body message */
                if (pstDecodedPart->_bodyPart._ucPres)
                {
                    ST_ZOS_SSTR stBodyStr;
                    std::string strBody(pstDecodedPart->_bodyPart._strBody);
                    SipMsgCommon::str2ZosSstr(strBody,stBodyStr);
                    pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
                }
            }
            else
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. SDP negotiate status wrong! sidDialog id :" + dlg->id());
            }
        }
    }

    std::string reason = dlg->getBlacklistType();
    if (!reason.empty())
    {
        pstSipMsgEn->fillHdrReason(reason);
    }

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. encode() return FALSE! sidDialog id :" + dlg->id());
    }

    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}

//===========================================分隔线=========================================//


/*  注意事项：

1.构建请求时，填充Via头域时，应该注意，CANCEL 方法中的 Via branch 参数必须与被取消的INVITE中的一样，但是，对于非2xx的回应的ACK branch是与INVITE一样，而回应2xx的ACK则不一样。
2.ACK 只在响应INVITE的响应时发送
3.BYE ，UA禁止在对话外发送BYE，请求方UA可以在已经建立好的对话或者早期对话（已收到临时响应）中发起BYE请求， 被叫方只能在建立好的对话中发起BYE请求，不能在早期发起

*/
bool SipMsg::sendSipMessageRequestSipMsg(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg ,const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    ZUCHAR ucMethod = 0;
    //check input parameter
    if (dlg == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }

    SipGateway::AuthType authType = dlg->getAuthType();
    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    if (pstDecodedPart->_fromPart._strUserName.size() == 0 || pstDecodedPart->_toPart._strUserName.size() == 0 )
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. Input caller or callee number,which is presented by userName is NULL. SipDialog id:" + dlg->id());
        return false;
    }

    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);
    switch(ucMethod)
    {
    case EN_SIP_METHOD_INVITE:
        if (authType == SipGateway::AuthNone)
        {
            return SendRequestINVITE(reqMethod,dlg , pSipGatwway);
        }
        else
        {
            return SendRequestINVITE_WithAuth(reqMethod, dlg, dlg->getAuthType(), pSipGatwway);
        }
        break;

    case EN_SIP_METHOD_ACK:
        return SendRequestACK(reqMethod,dlg, pSipGatwway);
        break;

    case EN_SIP_METHOD_BYE:
        return SendRequestBYE(reqMethod,dlg, pSipGatwway);
        break;

    case EN_SIP_METHOD_CANCEL:
        return SendRequestCANCEL(reqMethod,dlg, pSipGatwway);
        break;

    case EN_SIP_METHOD_PRACK:
        return SendRequestPACK(reqMethod,dlg, pSipGatwway);
        break;

    case EN_SIP_METHOD_UPDATE:
        return SendRequestUPDATE(reqMethod,dlg, pSipGatwway);
        break;

    default:
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.a wrong method:" + std::to_string(int(ucMethod)));
    }
    return true;
}

bool SipMsg::sendSipMessageResponseSipMsg(const SipGateway::SipResponseCodeType rspCode, const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg ,const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    int iCode = SipGateway::SipDialog::SipDialogResponseStatusToSipStatus(rspCode);
    return __sendSipMessageResponseSipMsg(iCode, reqMethod, dlg, pSipGatwway);
}

bool SipMsg::sendSipMessageResponseSipMsg2(int rspCode, const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    return __sendSipMessageResponseSipMsg(rspCode, reqMethod, dlg, pSipGatwway);
}

//send ..................
bool SipMsg::SendRequestINVITE(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg ,const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strCallerNum, strCalleeNum, strFromTag, strNumber,  strCallHost, strSipDialogIs, strBrach, strProxyIp, strRegisterIp, strFromUrl, strToUrl;
    ZUCHAR ucMethod, ucTptType;
    int iStartCseqNum = 0, iProxyPort = 0, iRegisterPort = 0, iFromPort = 0, iToPort = 0, iReSendTimes = 0;
    ZULONG dwPort = 0;
    bool bResult = true, bISBCAccessed = false;
    ST_SIP_REQ_URI  stReqUri;
    ST_SIP_SIP_URI * pstSipUri = 0;
    ST_SIP_ADDR_SPEC * pAddSpec = 0;
    std::string strCoreNetId;
    unsigned char reqUriType = EN_SIP_REQ_URI_SIP;
    bool bRefreshDialog = false;
    strCoreNetId = dlg->getCoreNetId();
    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    //check input parameter
    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    strSipDialogIs = dlg->id();
    int64_t rpcCallId = dlg->getRpcCallId();

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. call createReq() failed . ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);
    strCallerNum = pstDecodedPart->_fromPart._strUserName;
    strCalleeNum = pstDecodedPart->_toPart._strUserName;
    reqUriType = pstDecodedPart->_reqLinePart._reqUriType;

    ucTptType = dlg->getProtocolType();
    iReSendTimes = dlg->getReSendTimes();
    std::string sendIp;
    dlg->_sipRegisterSub->getIpsAndPorts(strProxyIp, iProxyPort, strRegisterIp, iRegisterPort, sendIp);
    if (strProxyIp.empty() || strRegisterIp.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:" + strProxyIp
            + " ,strRegisterIp:" + strRegisterIp + " ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }
    bISBCAccessed = dlg->getISBCAccessed();

    //fill ReqLine
    pstDecodedPart->_reqLinePart.setReqLinePartEx(ucMethod,strCalleeNum, strRegisterIp, iRegisterPort);
    bResult = pstSipMsgEn->fillReqLine(reqUriType, ucMethod, strCalleeNum, strRegisterIp, iRegisterPort);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillReqLine() failed,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId) + ",uriType:" + std::to_string(reqUriType));
        return false;
    }
    stReqUri = pstSipMsgEn->_pstMsg->u.stReqLine.stReqUri;
    if (bISBCAccessed)
    {
        SipMsgCommon::getReqSipsUri(pstSipMsgEn->_pstMsg,&pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    //fill From Header
    strFromTag = pstDecodedPart->_fromPart._strTag;
    if (strFromTag.empty())
    {
        strFromTag = SipMsgCommon::createFromTag();
    }

    strFromUrl = bISBCAccessed ? strProxyIp : strRegisterIp;
    iFromPort = bISBCAccessed ? 0 : iRegisterPort;

    std::string realm = dlg->getDomainRealm();
    if (realm.empty())
        realm = strFromUrl;

    //From头域的 uri应该填入注册服务器地址(A-SBC)，在与Glory I-SBC对接时填SipProxy所在服务器地址
    pstDecodedPart->_fromPart.setFromToPartEx(strCallerNum, strCallerNum, realm, iFromPort, strFromTag, ucTptType);
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, realm, iFromPort,pFromPart->_strTag, pFromPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header  ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    if (bISBCAccessed)
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,true, &pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    if (pFromPart->_sipUriOtherParams.size())
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,true, &pstSipUri);
        pstSipMsgEn->parmFillSipUriOthersEx(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, pFromPart->_sipUriOtherParams);
    }
    pstSipMsgEn->fillFromToPrams(pFromPart->_fromToParamVals);

    SipGateway::SipDialogState  dialogStatus = dlg->getSipDialogStatus();
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    std::string strToTag;
    if (SipGateway::kSipDialogStateACKConfirmed == dialogStatus)
    {
        bRefreshDialog = true;
        strToTag = pToPart->_strTag;
    }

    //fill To Header           To 头域中的uri应该填入注册服务器地址(A-SBC)，与Glory I-SBC对接时，需要填写remote 地址代替
    pstDecodedPart->_toPart.setFromToPartEx(strCalleeNum,strCalleeNum, strRegisterIp, iRegisterPort, strToTag, ucTptType);
    bResult = pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, strRegisterIp, iRegisterPort,pToPart->_strTag, pToPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill To Header  ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    if (bISBCAccessed)
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg, false, &pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    //TEST....
    pstSipUri = 0;
    SipMsgCommon::getReqSipsUri(pstSipMsgEn->_pstMsg, &pstSipUri);
    if (pstSipUri != 0)
    {
        pstSipMsgEn->parmFillSipUriTpt(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, ucTptType);
    }

    //TEST...
    pstSipUri = 0;
    SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg, false, &pstSipUri);
    if (pstSipUri != 0)
    {
        pstSipMsgEn->parmFillSipUriTpt(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, ucTptType);
    }

    //fill Call-Id Header
    SipMsgCommon::GetCallIdNumberHost(strSipDialogIs, strNumber, strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strNumber, strCallHost);
    bResult = pstSipMsgEn->fillHdrCallId(strNumber, strCallHost);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.. call fillHdrCallId() failed ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    //fill CSeq Header
    if (bRefreshDialog)
        iStartCseqNum = pstDecodedPart->_cseqPart._iCseqVal + 1;
    else
        iStartCseqNum = iReSendTimes == 0 ?  SipMsgCommon::CreateRandInt() : pstDecodedPart->_cseqPart._iCseqVal;

    pstDecodedPart->_cseqPart.setCseqPart(iStartCseqNum, ucMethod);
    pstSipMsgEn->fillHdrCseq(ucMethod,pstDecodedPart->_cseqPart._iCseqVal);

    //填充Via头域时，应该注意，CANCEL 方法中的 Via branch 参数必须与被取消的INVITE中的一样，但是，对于非2xx的回应的ACK branch是与INVITE一样，而回应2xx的ACK则不一样。
    //fill Via Header       BYE方法与INVITE 不是一个事务，所以Branch参数不一样
    strBrach = iReSendTimes == 0 ? SipMsgCommon::createBranch() : pstDecodedPart->_viaPart._strBranch;
    std::string viaIp = sendIp.empty() ? strProxyIp : sendIp;
    pstDecodedPart->_viaPart.setViaPart(viaIp, iProxyPort,ucTptType,false,0, strBrach);
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, viaIp, iProxyPort, false,0, strBrach);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed  ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    //fill Max-Forwards Header
    pstDecodedPart->_MaxForwards = 70;
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Contact Header
    bResult = pstSipMsgEn->fillHdrContact(strCallerNum, strProxyIp, iProxyPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed ,SipDialog id:"
            + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId) + ",strProxyIp:" + strProxyIp + ",iProxyPort:" + std::to_string(int(iProxyPort)));
    }

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INFO);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_SUBS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_MSG);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_REFER);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_COMET);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PUBLISH);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_TMR);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_REPLACES);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    /* fill P-Early-Media Header  EN_SIP_P_EARLY_MEDIA_PARM*/
    pstSipMsgEn->fillHdrPEarlyMedia(EN_SIP_P_EARLY_MEDIA_PARM_SUPPORTED);

    /* fill header Session-Expires */
    //pstSipMsgEn->fillHdrSessExpires(true, false, 1800);

    /* fill content-type header */
    pstSipMsgEn->fillHdrContentType(EN_SIP_MTYPE_DISC_APP, "", EN_SIP_MSUBTYPE_SDP, "");

    //fill header content-dispsition
    //std::string strTem = "session";
    //pstSipMsgEn->fillHdrContentDisposition(strTem);

    ////fill header P-Preferred-Identity
    if (strCoreNetId != SipGateway::SIP_GW_CORENET_ID_GLROY)
    {
        pstSipMsgEn->fillHdrPPreferredIdentity("", strCallerNum,  strFromUrl, iFromPort);
    }

    if (pstDecodedPart->_extHeaders.size())
    {
        pstSipMsgEn->fillHdrExts(pstDecodedPart->_extHeaders);
    }

    ///* set the body message */
    if (pstDecodedPart->_bodyPart._ucPres)
    {
        ST_ZOS_SSTR stBodyStr;
        std::string strBody = dlg->getSipSdpOffer();
        SipMsgCommon::str2ZosSstr(strBody,stBodyStr);
        pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
    }
    else
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled.no sdp,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    //fill Header UUI
    if (false == dlg->getUuiData().empty())
        pstSipMsgEn->fillHdrExt(SIP_GW_UUI_HEAER_NAME, dlg->getUuiData());

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. encode(), return FALSE! ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}

bool SipMsg::SendRequestINVITE_WithAuth(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, SipGateway::AuthType authType, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strCallerNum, strCalleeNum, strFromTag, strNumber,  strCallHost, strSipDialogIs, strBrach, strProxyIp, strRegisterHost, strFromUrl;
    ZUCHAR ucMethod, ucTptType;
    int iStartCseqNum = 0, iProxyPort = 0, iRegisterPort = 0, iFromPort = 0, iReSendTimes = 0;
    ZULONG dwPort = 0;
    bool bResult = true, bISBCAccessed = false;;
    std::string strRemoteConHost, strLocalConHost;       //Gateway Connection
    int iRemoteConPort = 0, iLocalConPort = 0;
    ST_SIP_REQ_URI  stReqUri;
    ST_SIP_DIGEST_RSP stRsp;
    int stateCode = 0;
    ST_SIP_SIP_URI * pstSipUri = 0;
    ST_SIP_ADDR_SPEC * pAddSpec = 0;
    bool bRefreshDialog = false;
    std::string strCoreNetId;
    unsigned char reqUriType = EN_SIP_REQ_URI_SIP;

    strCoreNetId = dlg->getCoreNetId();
    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    //check input parameter
    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    strSipDialogIs = dlg->id();
    int64_t rpcCallId = dlg->getRpcCallId();

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. createReq failed .SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);
    strCallerNum = pstDecodedPart->_fromPart._strUserName;
    strCalleeNum = pstDecodedPart->_toPart._strUserName;
    ucTptType = dlg->getProtocolType();
    iReSendTimes = dlg->getReSendTimes();
    reqUriType = pstDecodedPart->_reqLinePart._reqUriType;
    std::string sendIp;
    dlg->_sipRegisterSub->getIpsAndPorts(strProxyIp,iProxyPort,strRegisterHost, iRegisterPort, sendIp);
    if (strProxyIp.empty() || strRegisterHost.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:"
            + strProxyIp + " ,strRegisterIp:" + strRegisterHost + " .SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    bISBCAccessed = dlg->getISBCAccessed();
    //fill ReqLine
    pstDecodedPart->_reqLinePart.setReqLinePartEx(ucMethod,strCalleeNum, strRegisterHost, iRegisterPort);
    bResult = pstSipMsgEn->fillReqLine(reqUriType, ucMethod, strCalleeNum, strRegisterHost, iRegisterPort);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillReqLine() failed . SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId) + ",uriType:" + std::to_string(reqUriType));
        return false;
    }
    if (bISBCAccessed)
    {
        SipMsgCommon::getReqSipsUri(pstSipMsgEn->_pstMsg,&pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    //TEST....
    pstSipUri = 0;
    SipMsgCommon::getReqSipsUri(pstSipMsgEn->_pstMsg,  &pstSipUri);
    if (pstSipUri != 0)
    {
        pstSipMsgEn->parmFillSipUriTpt(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, ucTptType);
    }

    stReqUri = pstSipMsgEn->_pstMsg->u.stReqLine.stReqUri;
    //fill From Header
    strFromTag = pstDecodedPart->_fromPart._strTag;
    if (strFromTag.empty())
    {
        strFromTag = SipMsgCommon::createFromTag();
    }

    strFromUrl = bISBCAccessed ? strProxyIp : strRegisterHost;
    iFromPort = bISBCAccessed ? 0 : iRegisterPort;

    std::string realm = dlg->getDomainRealm();
    if (realm.empty())
        realm = strFromUrl;

    //From头域的 uri应该填入注册服务器地址（A-SBC对接），与Glory I-SBC对接时填写 SipProxy服务器ip
    pstDecodedPart->_fromPart.setFromToPartEx(strCallerNum, strCallerNum, realm, iFromPort, strFromTag, ucTptType);
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, realm, iFromPort, pFromPart->_strTag, pFromPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header. SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
    }

    if (bISBCAccessed)
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,true, &pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    if (pFromPart->_sipUriOtherParams.size())
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,true, &pstSipUri);
        pstSipMsgEn->parmFillSipUriOthersEx(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, pFromPart->_sipUriOtherParams);
    }
    pstSipMsgEn->fillFromToPrams(pFromPart->_fromToParamVals);

    //fill To Header
    //To 头域中的uri应该填入注册服务器地址(A-SBC)，与Glory I-SBC 对接时，需要填写 remote 地址
    //pstDecodedPart->_toPart.SetFromToPart(EN_SIP_ADDR_SPEC_SIP, strCalleeNum,strCalleeNum, strRegisterHost, iRegisterPort, "", ucTptType);
    SipGateway::SipDialogState  dialogStatus = dlg->getSipDialogStatus();
    std::string strToTag ;
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    if (SipGateway::kSipDialogStateACKConfirmed == dialogStatus)
    {
        bRefreshDialog = true;
        strToTag = pToPart->_strTag;
    }

    bResult = pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, strRegisterHost, iRegisterPort, strToTag, pToPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill To Header . .SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
    }

    if (bISBCAccessed)
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,true, &pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    //TEST...
    pstSipUri = 0;
    SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg, false, &pstSipUri);
    if (pstSipUri != 0)
    {
        pstSipMsgEn->parmFillSipUriTpt(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, ucTptType);
    }

    //fill Call-Id Header
    SipMsgCommon::GetCallIdNumberHost(strSipDialogIs,  strNumber,  strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strNumber,  strCallHost);
    bResult = pstSipMsgEn->fillHdrCallId(strNumber,  strCallHost);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrCallId() failed . .SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
    }

    //fill CSeq Header
    if (iReSendTimes == 0)
    {
        pstDecodedPart->_cseqPart._iCseqVal++ ;
    }
    pstSipMsgEn->fillHdrCseq(ucMethod,  pstDecodedPart->_cseqPart._iCseqVal);

    //rpc呼叫sip，振铃之后，再收到407，此时再发带鉴权的INVITE时生成新的via，则会出问题，比如，振铃时无法挂断，因为via 头branch参数已变
    SipGateway::SipDialogState curSipStatus = dlg->getSipDialogStatus();
    //fill Via Header
    strBrach = iReSendTimes == 0 ? SipMsgCommon::createBranch() : pstDecodedPart->_viaPart._strBranch;
    std::string viaIp = sendIp.empty() ? strProxyIp : sendIp;

    pstDecodedPart->_viaPart.setViaPart(viaIp, iProxyPort,ucTptType,false,0, strBrach);
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, viaIp, iProxyPort, false,0,strBrach);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed . .SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
    }

    //fill Max-Forwards Header
    pstDecodedPart->_MaxForwards = 70;
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Contact Header
    bResult = pstSipMsgEn->fillHdrContact(strCallerNum, strProxyIp, iProxyPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed  .SipDialog id:"
            + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId) + ",strProxyIp:" + strProxyIp + ",iProxyPort:" + std::to_string(int(iProxyPort)));
    }

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_REGISTER);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INFO);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_SUBS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_MSG);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_REFER);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_COMET);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PUBLISH);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_TMR);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_REPLACES);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    /* fill P-Early-Media Header  EN_SIP_P_EARLY_MEDIA_PARM*/
    pstSipMsgEn->fillHdrPEarlyMedia(EN_SIP_P_EARLY_MEDIA_PARM_SUPPORTED);

    /* fill header Session-Expires */
    //pstSipMsgEn->fillHdrSessExpires(true, false, 1800);

    //fill Authorization
    /* create digest response list */
    ABNF_LIST_CREATE(&stRsp.stDigRspLst);
    /* calculate md5 */
    authCalcRsp(pstSipMsgEn, dlg->_sipRegisterSub, ucMethod, 0, &stReqUri, &stRsp);
    stateCode = authType == SipGateway::Auth401 ? 401 : 407;
    /* fill authorization header */
    pstSipMsgEn->fillHdrAuthor(stRsp, stateCode);

    //nonce count + 1
    dlg->_sipRegisterSub->setDigestNonceCount( dlg->_sipRegisterSub->getDigestNonceCount() + 1);

    /* fill content-type header */
    pstSipMsgEn->fillHdrContentType(EN_SIP_MTYPE_DISC_APP, "", EN_SIP_MSUBTYPE_SDP, "");

    //fill header content-dispsition
    //pstSipMsgEn->fillHdrContentDisposition("session");

    ////fill header P-Preferred-Identity
    if (strCoreNetId != SipGateway::SIP_GW_CORENET_ID_GLROY)
    {
        pstSipMsgEn->fillHdrPPreferredIdentity("", strCallerNum,  strFromUrl, iFromPort);
    }

    if (pstDecodedPart->_extHeaders.size())
    {
        pstSipMsgEn->fillHdrExts(pstDecodedPart->_extHeaders);
    }

    ///* set the body message */
    if (pstDecodedPart->_bodyPart._ucPres)
    {
        ST_ZOS_SSTR stBodyStr;
        std::string strBody  = dlg->getSipSdpOffer();
        SipMsgCommon::str2ZosSstr(strBody,stBodyStr);
        pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
    }
    else
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. no sdp, SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    //fill Header UUI
    if (false == dlg->getUuiData().empty())
        pstSipMsgEn->fillHdrExt(SIP_GW_UUI_HEAER_NAME, dlg->getUuiData());

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. encode(), return FALSE!. SipDialog id:" + strSipDialogIs +" ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }
    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}


bool SipMsg::SendRequestACK(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg,const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strCallerNum, strCalleeNum, strFromTag, strNumber,  strCallHost, strSipDialogIs, strBrach, strProxyHost, strRegisterHost, strReqUri;
    ZUCHAR ucMethod, ucTptType;
    int iStartCseqNum = 0, iProxyPort = 0, iRegisterPort = 0, iReqPort = 0;
    ZULONG dwPort = 0;
    bool bResult = true;
    SipGateway::SDPNegotiateStatus sdpNegotiaStatus = SipGateway::SDPInit;
    std::string reason;
    unsigned char reqUriType = EN_SIP_REQ_URI_SIP;

    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    std::string strRemoteConHost, strLocalConHost;
    int iRemoteConPort = 0, iLocalConPort = 0;

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createReq())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call createReq failed .SipDialog id :" + dlg->id());
        return false;
    }
    strSipDialogIs = dlg->id();

    strCallerNum = pstDecodedPart->_fromPart._strUserName;
    strCalleeNum = pstDecodedPart->_toPart._strUserName;
    ucTptType = dlg->getProtocolType();
    reqUriType = pstDecodedPart->_reqLinePart._reqUriType;
    std::string sendIp;
    dlg->_sipRegisterSub->getIpsAndPorts(strProxyHost,iProxyPort,strRegisterHost, iRegisterPort, sendIp);
    if (strProxyHost.empty() || strRegisterHost.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:"
            + strProxyHost + " ,strRegisterIp:" + strRegisterHost + " ,sipDialog id:" + dlg->id());
        return false;
    }

    //fill ReqLine
    //发送ACK至 SIP 端，说明是由RPC发起的INVITE,本来应该填写contact中的uri，如果为空，用To头域中的代替？
    strReqUri =  pstDecodedPart->_toPart._contactPart._strHostName.empty() ? pstDecodedPart->_toPart._strHostName :  pstDecodedPart->_toPart._contactPart._strHostName;
    iReqPort = pstDecodedPart->_toPart._contactPart._strHostName.empty() ? pstDecodedPart->_toPart._dwPort :  pstDecodedPart->_toPart._contactPart._iPort;

    bResult = pstSipMsgEn->fillReqLine(reqUriType, ucMethod, pstDecodedPart->_toPart._strUserName, strReqUri, iReqPort);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillReqLine() failed .sipDialog id:" + dlg->id() + ",uriType:" + std::to_string(reqUriType));
        return false;
    }

    //fill From Header
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort, pFromPart->_strTag, pFromPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header .sipDialog id:" + dlg->id());
        return false;
    }

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,pToPart->_strTag, pToPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill To Header .,sipDialog id:" + dlg->id());
        return false;
    }

    //fill Call-Id Header
    SipMsgCommon::GetCallIdNumberHost(strSipDialogIs,  strNumber,  strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strNumber,  strCallHost);
    bResult = pstSipMsgEn->fillHdrCallId(strNumber,  strCallHost);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrCallId() failed .,sipDialog id:" + dlg->id());
        return false;
    }

    //fill CSeq Header
    pstSipMsgEn->fillHdrCseq(ucMethod,pstDecodedPart->_cseqPart._iCseqVal);

    //填充Via头域时，应该注意，CANCEL 方法中的 Via branch 参数必须与被取消的INVITE中的一样，但是，对于非2xx的回应的ACK branch是与INVITE一样，而回应2xx的ACK则不一样。
    //fill Via Header  BYE方法与INVITE 不是一个事务，所以Branch参数不一样
    SipGateway::SipDialogState dialogStatus = dlg->getSipDialogStatus();
    int resCode = pstDecodedPart->_statusLinePart._iCode;
    if (ucMethod == EN_SIP_METHOD_ACK && dialogStatus == SipGateway::kSipDialogState200Ok && resCode == 200)
    {
        strBrach = SipMsgCommon::createBranch();            //给INVITE请求的200 OK响应的ACK是另一个事务
    }
    else
    {
        strBrach = pstDecodedPart->_viaPart._strBranch;
    }

    bResult = pstSipMsgEn->fillHdrVia(ucTptType, strProxyHost, iProxyPort, false,0,strBrach);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed .,sipDialog id:"
            + dlg->id() + " ,strBrach:" + strBrach + " ,status:" + std::to_string(int(dialogStatus)));
        return false;
    }

    //fill Max-Forwards Header
    pstDecodedPart->_MaxForwards = 70;
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Contact Header
    bResult = pstSipMsgEn->fillHdrContact(strCallerNum, strProxyHost, iProxyPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed .sipDialog id:" + dlg->id() + ",strProxyHost:" + strProxyHost +
            ",iProxyPort:" + std::to_string(int(iProxyPort)));
        return false;
    }

    //fill User-Agent
    pstSipMsgEn->fillHdrUserAgent("SipGateway-client", "1.0", "");

    //fill Route Header
    if (pstDecodedPart->_recordRoutes._sipUris.size() > 0)
    {
        bool sipCallRpc = dlg->getSipCallRpcProp();
        pstSipMsgEn->fillHdrRouteBySipUri(pstDecodedPart->_recordRoutes._sipUris, sipCallRpc);
    }

    if (sdpNegotiaStatus == SipGateway::SDPNegotiating)
    {
        //如果SDP处于正在协商中，则说明只收到或者发送了offer； 说明Rpc呼叫SIP端， 只在200 OK响应中收到了offer，因此需要在ACK中发送answer
        /* fill content-type header */
        pstSipMsgEn->fillHdrContentType(EN_SIP_MTYPE_DISC_APP, "", EN_SIP_MSUBTYPE_SDP, "");

        ///* set the body message */
        if (pstDecodedPart->_bodyPart._ucPres)
        {
            ST_ZOS_SSTR stBodyStr;
            std::string strBody(pstDecodedPart->_bodyPart._strBody);
            SipMsgCommon::str2ZosSstr(strBody,stBodyStr);
            pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
        }
    }

    if (!pstSipMsgEn->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. encode(), return FALSE! sipDialog id:" + dlg->id());
        return false;
    }

    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}

bool SipMsg::SendRequestBYE(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strCallerNum, strCalleeNum, strFromTag, strNumber,  strCallHost, strSipDialogIs, strBrach, strProxylHost, strRegisterHost, strFromHost, strToHost;
    ZUCHAR ucMethod, ucTptType;
    int iStartCseqNum = 0, iProxyPort = 0, iRegisterPort = 0, iReSendTimes = 0, iFromPort = 0, iToPort = 0;
    ZULONG dwPort = 0;
    bool bResult = true;
    bool bSipToRpc = false;
    std::string reason;
    bool bISBC = false;
    unsigned char reqUriType = EN_SIP_REQ_URI_SIP;

    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    std::string strRemoteConHost, strLocalConHost;
    int iRemoteConPort = 0, iLocalConPort = 0;

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. createReq failed .sipDialog id:" + dlg->id());
        return false;
    }
    strSipDialogIs = dlg->id();

    strCallerNum = pstDecodedPart->_fromPart._strUserName;
    strCalleeNum = pstDecodedPart->_toPart._strUserName;
    ucTptType = dlg->getProtocolType();
    iReSendTimes = dlg->getReSendTimes();
    reqUriType = pstDecodedPart->_reqLinePart._reqUriType;
    std::string sendIp;
    dlg->_sipRegisterSub->getIpsAndPorts( strProxylHost, iProxyPort,strRegisterHost,iRegisterPort, sendIp);
    if (strProxylHost.empty() || strRegisterHost.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:" + strProxylHost + " ,strRegisterIp:" + strRegisterHost + " ,SipDialog id:" + dlg->id());
        return false;
    }

    bool bSipCallRpc = dlg->getSipCallRpcProp();
    FromToPart fromToPart = bSipCallRpc ? pstDecodedPart->_fromPart  : pstDecodedPart->_toPart;

    FromToPart * pFromPart  = 0;
    FromToPart * pToPart = 0;
    bSipToRpc = dlg->getSipCallRpcProp();
    if (bSipToRpc)
    {
        pFromPart =  &pstDecodedPart->_toPart;
        pToPart = &pstDecodedPart->_fromPart;
    }
    else
    {
        pFromPart =  &pstDecodedPart->_fromPart;
        pToPart =  &pstDecodedPart->_toPart;
    }
    bISBC = dlg->getISBCAccessed();
    strFromHost = pFromPart->_strHostName;
    iFromPort = pFromPart->_dwPort;
    strToHost = pToPart->_strHostName;
    iToPort = pToPart->_dwPort;
    if (false == bISBC && bSipCallRpc)
    {
        if (strRegisterHost != strFromHost)
        {
            strFromHost = strRegisterHost;
            iFromPort = iRegisterPort;
            strToHost = strRegisterHost;
            iToPort = iRegisterPort;
        }
    }

    //fill req line
    if (pstDecodedPart->_peerSipUri._pres)
    {
        pstSipMsgEn->fillReqLineBySipUri(EN_SIP_METHOD_BYE, &pstDecodedPart->_peerSipUri._sipUri);
    }
    else
    {
        bResult = pstSipMsgEn->fillReqLine(reqUriType, ucMethod, fromToPart._strUserName, fromToPart._contactPart._strHostName, fromToPart._contactPart._iPort);
        if (!bResult)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.call fillReqLine() failed .sipDialog id:" + dlg->id() + ",uriType:" + std::to_string(reqUriType));
            return false;
        }

        if (bSipToRpc && pToPart->_contactPart._mapOthersPara.size() > 0)
        {
            bResult = pstSipMsgEn->fillReqLineSipUriParasCxtId(pToPart->_contactPart._mapOthersPara, reason);
            if (!bResult)
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillReqLineSipUriParasCxtId() 2 failed, fill Contact SipUri Params .sipDialog id:"
                    + dlg->id() + ",param map size:" + std::to_string(int(pToPart->_contactPart._mapOthersPara.size())) + ",reason:" + reason);
            }
        }
    }

    //fill From Header
    bResult = pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, strFromHost, iFromPort,
        pFromPart->_strTag, pFromPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header .sipDialog id:" + dlg->id());
    }

    //fill To Header
    bResult = pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, strToHost, iToPort,
        pToPart->_strTag, pToPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill To Header .sipDialog id:" + dlg->id());
    }

    //fill Call-Id Header
    SipMsgCommon::GetCallIdNumberHost(strSipDialogIs,  strNumber,  strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strNumber,  strCallHost);
    bResult = pstSipMsgEn->fillHdrCallId(strNumber,  strCallHost);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrCallId() failed .sipDialog id:" + dlg->id());
    }

    //fill CSeq Header
    //在对话中，BYE方法的CSeq数值需要加1，ACK 和CANCEL除外，协议RFC3261 p84页
    int temCSeq = dlg->getSipDialogStatus() == SipGateway::kSipDialogStateACKConfirmed ? pstDecodedPart->_cseqPart._iCseqVal + 1 : pstDecodedPart->_cseqPart._iCseqVal;
    if (iReSendTimes > 0)
    {
        temCSeq = pstDecodedPart->_cseqPart._iCseqVal;
    }
    pstDecodedPart->_cseqPart.setCseqPart(temCSeq, ucMethod);
    pstSipMsgEn->fillHdrCseq(ucMethod,pstDecodedPart->_cseqPart._iCseqVal);

    //fill Via Header		填充Via头域时，应该注意，CANCEL 方法中的 Via branch 参数必须与被取消的INVITE中的一样，但是，对于非2xx的回应的ACK branch是与INVITE一样，而回应2xx的ACK则不一样。
    //BYE方法与INVITE 不是一个事务，所以Branch参数不一样
    SipGateway::SipDialogState dialogStatus = dlg->getSipDialogStatus();
    strBrach = iReSendTimes == 0 ? SipMsgCommon::createBranch() : pstDecodedPart->_viaPart._strBranch;
    pstDecodedPart->_viaPart.setViaPart(strProxylHost, iProxyPort,ucTptType,false,0, strBrach);

    bResult = pstSipMsgEn->fillHdrVia(ucTptType, strProxylHost, iProxyPort, false,0,strBrach);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed .sipDialog id:" + dlg->id());
    }

    //fill Max-Forwards Header
    pstDecodedPart->_MaxForwards = 70;
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Route Header
    if (pstDecodedPart->_recordRoutes._sipUris.size() > 0)
    {
        bool sipCallRpc = dlg->getSipCallRpcProp();
        pstSipMsgEn->fillHdrRouteBySipUri(pstDecodedPart->_recordRoutes._sipUris, sipCallRpc);
    }

    //fill User-Agent
    pstSipMsgEn->fillHdrUserAgent("SipGateway-client", "1.0", "");

    if (!pstSipMsgEn->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. encode(), return FALSE! sipDialog id:" + dlg->id());
        return false;
    }

    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}

bool SipMsg::SendRequestCANCEL(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg,const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn = 0;
    std::string strCallerNum, strCalleeNum, strFromTag, strNumber,  strCallHost, strSipDialogIs, strBrach, strProxyHost, strRegisterHost, strReqUri;
    ZUCHAR ucMethod = 0, ucTptType = 0;
    int iStartCseqNum = 0, iProxyPort = 0, iRegisterPort = 0, iReqPort = 0, iReSendTimes = 0;
    ZULONG dwPort = 0;
    bool bResult = true;
    unsigned char reqUriType = EN_SIP_REQ_URI_SIP;

    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    std::string strRemoteConHost, strLocalConHost;
    int iRemoteConPort = 0, iLocalConPort = 0;

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);
    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. createReq failed .SipDialog id:" + dlg->id());
        return false;
    }
    strSipDialogIs = dlg->id();
    iReSendTimes = dlg->getReSendTimes();
    strCallerNum = pstDecodedPart->_fromPart._strUserName;
    strCalleeNum = pstDecodedPart->_toPart._strUserName;
    ucTptType = dlg->getProtocolType();
    reqUriType = pstDecodedPart->_reqLinePart._reqUriType;
    std::string sendIp;
    dlg->_sipRegisterSub->getIpsAndPorts(strProxyHost,iProxyPort,strRegisterHost, iRegisterPort, sendIp);
    if (strProxyHost.empty() || strRegisterHost.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:" + strProxyHost + " ,strRegisterIp:" + strRegisterHost + " ,sipDialog id:" + dlg->id());
        return false;
    }

    //fill ReqLine
    //发送ACK至 SIP 端，说明是由RPC发起的INVITE,本来应该填写contact中的uri，如果为空，用To头域中的代替？
    strReqUri =  pstDecodedPart->_toPart._contactPart._strHostName.empty() ? pstDecodedPart->_toPart._strHostName :  pstDecodedPart->_toPart._contactPart._strHostName;
    iReqPort = pstDecodedPart->_toPart._contactPart._strHostName.empty() ? pstDecodedPart->_toPart._dwPort :  pstDecodedPart->_toPart._contactPart._iPort;

    bResult = pstSipMsgEn->fillReqLine(reqUriType, ucMethod, pstDecodedPart->_toPart._strUserName, strRegisterHost, iRegisterPort);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.call fillReqLine() failed . SipDialog id:" + dlg->id() + ",uriType:" + std::to_string(reqUriType));
        return false;
    }

    //fill From Header
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort,
        pFromPart->_strTag, pFromPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header . SipDialog id:" + dlg->id());
    }

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
        pToPart->_strTag, pToPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill To Header .. SipDialog id:" + dlg->id());
    }

    //fill Call-Id Header
    SipMsgCommon::GetCallIdNumberHost(strSipDialogIs,  strNumber,  strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strNumber,  strCallHost);
    bResult = pstSipMsgEn->fillHdrCallId(strNumber,  strCallHost);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled..call fillHdrCallId() failed . SipDialog id:" + dlg->id());
    }

    //fill CSeq Header
    pstSipMsgEn->fillHdrCseq(ucMethod,pstDecodedPart->_cseqPart._iCseqVal);

    //填充Via头域时，应该注意，CANCEL 方法中的 Via branch 参数必须与被取消的INVITE中的一样，但是，对于非2xx的回应的ACK branch是与INVITE一样，而回应2xx的ACK则不一样。
    //fill Via Header       BYE方法与INVITE 不是一个事务，所以Branch参数不一样
    SipGateway::SipDialogState dialogStatus = dlg->getSipDialogStatus();
    strBrach = pstDecodedPart->_viaPart._strBranch;     //CANCEL的 branch与 INVITE一致
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, strProxyHost, iProxyPort, false,0,strBrach);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed .SipDialog id:" + dlg->id());
    }

    //fill Max-Forwards Header
    pstDecodedPart->_MaxForwards = 70;
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Route Header
    if (pstDecodedPart->_recordRoutes._sipUris.size() > 0)
    {
        bool sipCallRpc = dlg->getSipCallRpcProp();
        pstSipMsgEn->fillHdrRouteBySipUri(pstDecodedPart->_recordRoutes._sipUris, sipCallRpc);
    }

    if (!pstSipMsgEn->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. encode().return FALSE! SipDialog id:" + dlg->id());
        return false;
    }
    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}


bool SipMsg::SendRequestPACK(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strCallerNum, strCalleeNum, strFromTag, strNumber,  strCallHost, strSipDialogIs, strBrach, strProxyHost, strRegisterHost, strReqUri;
    ZUCHAR ucMethod, ucTptType;
    int iStartCseqNum = 0, iProxyPort = 0, iRegisterPort = 0, iReqPort = 0;
    ZULONG dwPort = 0;
    bool bResult = true;
    unsigned char reqUriType = EN_SIP_REQ_URI_SIP;

    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    std::string strRemoteConHost, strLocalConHost;
    int iRemoteConPort = 0, iLocalConPort = 0;
    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createReq())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. createReq failed . sipDialog id:" + dlg->id());
        return false;
    }
    strSipDialogIs = dlg->id();

    strCallerNum = pstDecodedPart->_fromPart._strUserName;
    strCalleeNum = pstDecodedPart->_toPart._strUserName;
    ucTptType = dlg->getProtocolType();
    reqUriType = pstDecodedPart->_reqLinePart._reqUriType;
    std::string sendIp;
    dlg->_sipRegisterSub->getIpsAndPorts(strProxyHost,iProxyPort,strRegisterHost, iRegisterPort, sendIp);
    if (strProxyHost.empty() || strRegisterHost.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getIpsAndPorts() reurn : strProxyIp:" + strProxyHost + " ,strRegisterIp:" + strRegisterHost + " ,SipDialog id:" + dlg->id());
        return false;
    }

    //fill ReqLine
    //发送ACK至 SIP 端，说明是由RPC发起的INVITE,本来应该填写contact中的uri，如果为空，用To头域中的代替？
    strReqUri =  pstDecodedPart->_toPart._contactPart._strHostName.empty() ? pstDecodedPart->_toPart._strHostName :  pstDecodedPart->_toPart._contactPart._strHostName;
    iReqPort = pstDecodedPart->_toPart._contactPart._strHostName.empty() ? pstDecodedPart->_toPart._dwPort :  pstDecodedPart->_toPart._contactPart._iPort;

    bResult = pstSipMsgEn->fillReqLine(reqUriType, ucMethod, pstDecodedPart->_toPart._strUserName, strReqUri, iReqPort);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillReqLine() failed .SipDialog id:" + dlg->id() + ",uriType:" + std::to_string(reqUriType));
        return false;
    }

    //fill From Header
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort, pFromPart->_strTag, pFromPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.. call fillHdrFromToByName() failed, fill From Header .SipDialog id:" + dlg->id());
    }

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,pToPart->_strTag, pToPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled..call fillHdrFromToByName() failed, fill To Header .SipDialog id:" + dlg->id());
    }

    //fill Call-Id Header
    SipMsgCommon::GetCallIdNumberHost(strSipDialogIs,  strNumber,  strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strNumber,  strCallHost);
    bResult = pstSipMsgEn->fillHdrCallId(strNumber,  strCallHost);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrCallId() failed .SipDialog id:" + dlg->id());
    }

    //fill CSeq Header
    int rseq = dlg->getRseqUpdate();
    int iReqNum = (int)pstDecodedPart->_cseqPart._iCseqVal + rseq;
    pstSipMsgEn->fillHdrCseq(ucMethod, iReqNum);

    //fill Via Header
    strBrach = SipMsgCommon::createBranch();            //给INVITE请求的200 OK响应的ACK是另一个事务
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, strProxyHost, iProxyPort, false,0,strBrach);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed .SipDialog id:" + dlg->id());
    }

    //fill Max-Forwards Header
    pstDecodedPart->_MaxForwards = 70;
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill User-Agent
    pstSipMsgEn->fillHdrUserAgent("SipGateway-client", "1.0", "");

    //fill PACK Header
    pstSipMsgEn->fillHdrRAck(pstDecodedPart->_cseqPart._iCseqVal, pstDecodedPart->_rseqPart._respNum, EN_SIP_METHOD_INVITE);
    //fill Route Header
    if (pstDecodedPart->_recordRoutes._sipUris.size() > 0)
    {
        bool sipCallRpc = dlg->getSipCallRpcProp();
        pstSipMsgEn->fillHdrRouteBySipUri(pstDecodedPart->_recordRoutes._sipUris, sipCallRpc);
    }

    if (!pstSipMsgEn->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. encode(), return FALSE! SipDialog id:" + dlg->id());
        return false;
    }
    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}

bool SipMsg::SendRequestUPDATE(const SipGateway::SipRequestMethodType reqMethod, SipGateway::SipDialogPtr & dlg, const SipGateway::SipMsgSenderInterfacePtr & pSipGatwway)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strCallerNum, strCalleeNum, strFromTag, strNumber,  strCallHost, strSipDialogIs, strBrach, strProxyIp, strRegisterIp, strFromUrl, strToUrl;
    ZUCHAR ucMethod, ucTptType;
    int iStartCseqNum = 0, iProxyPort = 0, iRegisterPort = 0, iFromPort = 0, iToPort = 0;
    ZULONG dwPort = 0;
    bool bResult = true, bISBCAccessed = false;
    ST_SIP_REQ_URI  stReqUri;
    ST_SIP_SIP_URI * pstSipUri = 0;
    ST_SIP_ADDR_SPEC * pAddSpec = 0;
    std::string strCoreNetId;
    unsigned char reqUriType = EN_SIP_REQ_URI_SIP;

    strCoreNetId = dlg->getCoreNetId();
    if (pSipGatwway == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. pSipGatwway == NULL! ERROR!");
        return false;
    }

    //check input parameter
    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    strSipDialogIs = dlg->id();
    int64_t rpcCallId = dlg->getRpcCallId();

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. call createReq() failed . ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    ucMethod = (ZUCHAR)SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);
    strCallerNum = pstDecodedPart->_fromPart._strUserName;
    strCalleeNum = pstDecodedPart->_toPart._strUserName;
    ucTptType = dlg->getProtocolType();
    reqUriType = pstDecodedPart->_reqLinePart._reqUriType;
    std::string sendIp;
    dlg->_sipRegisterSub->getIpsAndPorts(strProxyIp, iProxyPort, strRegisterIp, iRegisterPort, sendIp);
    if (strProxyIp.empty() || strRegisterIp.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.getIpsAndPorts() reurn : strProxyIp:" + strProxyIp
            + " ,strRegisterIp:" + strRegisterIp + " ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }
    bISBCAccessed = dlg->getISBCAccessed();

    //fill req line
    if (pstDecodedPart->_peerSipUri._pres)
    {
        pstSipMsgEn->fillReqLineBySipUri(EN_SIP_METHOD_UPDATE, &pstDecodedPart->_peerSipUri._sipUri);
    }
    else
    {
        //fill ReqLine
        bResult = pstSipMsgEn->fillReqLine(reqUriType, ucMethod, strCalleeNum, strRegisterIp, iRegisterPort);
        if (!bResult)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillReqLine() failed . ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId) + ",uriType:" + std::to_string(reqUriType));
            return false;
        }
        stReqUri = pstSipMsgEn->_pstMsg->u.stReqLine.stReqUri;
        if (bISBCAccessed)
        {
            SipMsgCommon::getReqSipsUri(pstSipMsgEn->_pstMsg,&pstSipUri);
            if (pstSipUri != 0)
            {
                pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
            }
        }
    }

    //fill From Header
    strFromTag = pstDecodedPart->_fromPart._strTag;
    if (strFromTag.empty())
    {
        strFromTag = SipMsgCommon::createFromTag();
    }

    strFromUrl = bISBCAccessed ? strProxyIp : strRegisterIp;
    iFromPort = bISBCAccessed ? 0 : iRegisterPort;

    std::string realm = dlg->getDomainRealm();
    if (realm.empty())
        realm = strFromUrl;

    //From头域的 uri应该填入注册服务器地址(A-SBC)，在与Glory I-SBC对接时填SipProxy所在服务器地址
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, realm, iFromPort,pFromPart->_strTag, pFromPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header  ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    if (bISBCAccessed)
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg,true, &pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    //fill To Header
    //To 头域中的uri应该填入注册服务器地址(A-SBC)，与Glory I-SBC对接时，需要填写remote 地址代替
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    bResult = pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, strRegisterIp, iRegisterPort,pToPart->_strTag, pToPart->_ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill To Header  ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    if (bISBCAccessed)
    {
        pstSipUri = 0;
        SipMsgCommon::getFromToSipUri(pstSipMsgEn->_pstMsg, false, &pstSipUri);
        if (pstSipUri != 0)
        {
            pstSipMsgEn->parmFillSipUriUSer(pstSipMsgEn->_pstMsg->zMemBuf, pstSipUri, EN_SIP_USER_PARM_PHONE);
        }
    }

    //fill Call-Id Header
    SipMsgCommon::GetCallIdNumberHost(strSipDialogIs,  strNumber,  strCallHost);
    bResult = pstSipMsgEn->fillHdrCallId(strNumber,  strCallHost);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.call fillHdrCallId() failed ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    //fill CSeq Header
    iStartCseqNum = pstDecodedPart->_cseqPart._iCseqVal + 1;
    pstDecodedPart->_cseqPart.setCseqPart(iStartCseqNum, ucMethod);
    pstSipMsgEn->fillHdrCseq(ucMethod,pstDecodedPart->_cseqPart._iCseqVal);

    //填充Via头域时，应该注意，CANCEL 方法中的 Via branch 参数必须与被取消的INVITE中的一样，但是，对于非2xx的回应的ACK branch是与INVITE一样，而回应2xx的ACK则不一样。
    //fill Via Header BYE方法与INVITE 不是一个事务，所以Branch参数不一样
    SipGateway::SipDialogState dialogStatus = dlg->getSipDialogStatus();
    strBrach = SipMsgCommon::createBranch();

    bResult = pstSipMsgEn->fillHdrVia(ucTptType, strProxyIp, iProxyPort, false,0, strBrach);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.call fillHdrVia() failed  ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
    }

    //fill Max-Forwards Header
    pstDecodedPart->_MaxForwards = 70;
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Contact Header
    bResult = pstSipMsgEn->fillHdrContact(strCallerNum, strProxyIp, iProxyPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed ,SipDialog id:"
            + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId) + ",strProxyIp:" + strProxyIp + ",iProxyPort:" + std::to_string(int(iProxyPort)));
    }

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INFO);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_SUBS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_MSG);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_REFER);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_COMET);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PUBLISH);

    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();

    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_TMR);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);

    viSuptFlag.push_back(SIP_TAG_OPT_ADD_REPLACES);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_EARLY_SESS);

    /* fill P-Early-Media Header  EN_SIP_P_EARLY_MEDIA_PARM*/
    pstSipMsgEn->fillHdrPEarlyMedia(EN_SIP_P_EARLY_MEDIA_PARM_SUPPORTED);

    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    //fill Session-expires header
    if (pstDecodedPart->_sessionExpiresPart._iSeconds > 0)
    {
        pstSipMsgEn->fillHdrSessExpires(true, false, pstDecodedPart->_sessionExpiresPart._iSeconds);
    }
    //fill Route Header
    if (pstDecodedPart->_recordRoutes._sipUris.size() > 0)
    {
        bool sipCallRpc = dlg->getSipCallRpcProp();
        pstSipMsgEn->fillHdrRouteBySipUri(pstDecodedPart->_recordRoutes._sipUris, sipCallRpc);
    }

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() callled. encode(), return FALSE! ,SipDialog id:" + strSipDialogIs + " ,rpc call-id:" + std::to_string(rpcCallId));
        return false;
    }

    return pSipGatwway->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg);
}

bool SipMsg::getSipGWServiceNameFromSipMsg(const std::string & sipMsg, const std::string & currServiceName,std::string & serviceName)
{
    //Sip呼叫RPC时，将会在组建的响应消息中的TO头域前部加入标识SIpGateway服务和SipProxy服务的前缀，格式为：tag=SG4-3+SP4-0@192.168.125.126:5060@ddfb9acb0bdef479 或者 tag=SG+SP4-0@192.168.125.126:5060@ddfb9acb0bdef479
    //RPC呼叫SIP时，将会在组建INVITE的FROM头域中加入上述标识字符串
    //判断是否为INVITE方法； 在FROM 或者TO 头域的Tag中查询是否具有标识SIpGateway服务的
    SipMsgDecodePtr pstSipMsgDe = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag;
    bool bReqPres = true;
    bool result = false;
    SipMsgDecodedPart * pstDecodedPart = 0;

    pstSipMsgDe = std::make_shared<SipMsgDecode>();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(sipMsg, sipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. Sip msg is not valid or complete. sipMsg:\n" + sipMsg);
        return false;
    }

    // get From, To tag
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. getMethod() return FALSE.sipMsg:\n" + sipMsg);
        return false;
    }
    bReqPres = pstSipMsgDe->isRequest() ? true : false;
    if (ucMethod == EN_SIP_METHOD_INVITE && bReqPres)
    {
        //收到INVITE， sip发起呼叫 或者 刷新对话请求
        int pos1 = strLocalTag.find("SipG");
        int pos2 = strRemoteTag.find("SipP");
        if (pos1 < 0 && pos2 < 0)
        {
            serviceName = currServiceName;
        }
        else
        {
            //rpc call sip, sip refresh invite
            int pos3 = strRemoteTag.find("SipG");
            int pos4 = strRemoteTag.find("+");
            if (pos3 < 0)
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled.strRemoteTag:" + strRemoteTag +" ,received INVITE & Request. pos3 < 0 ! sipMsg:\n" + sipMsg);
                return false;
            }

            serviceName = strRemoteTag.substr(pos3, pos4 - pos3);
            return true;
        }
        return true;
    }
    else
    {
        //“SIpGateway5-2+SipProxy8-3+d4df4gs234xxd”
        int pos1 = strLocalTag.find("SipG");
        int pos2 = strLocalTag.find("+");
        if (pos1 < 0)
        {
            //From 头Tag中未找到SipGateway标识，则在TO中查找
            int pos3 = strRemoteTag.find("SipG");
            int pos4 = strRemoteTag.find("+");
            if (pos3 < 0)
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() callled. strRemoteTag: " + strRemoteTag +" ,received != INVITE or != Request, pos3 < 0 ! sipMsg:\n" + sipMsg);
                return false;
            }
            serviceName = strRemoteTag.substr(pos3, pos4 - pos3);
            return true;
        }
        else
        {
            serviceName = strLocalTag.substr(pos1, pos2 - pos1);
            return true;
        }
    }
    return true;
}

SipGateway::SipMsgDecodeResultType SipMsg::onReceiveSipMsgFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const std::string & remoteIp, int remotePort)
{
    if (0 == pSipProxy)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. pSipProxy == NULL! ERROR!");
        return SipGateway::kSipMsgDecodeFailed;
    }

    if (0 == pSipConnection)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. pSipConnection == NULL! ERROR!");
        return SipGateway::kSipMsgDecodeFailed;
    }

    if (strBuffer == "hello")
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return SipGateway::kSipMsgDecodeSuccessed;
    }

    unsigned char ucMethod;
    SipMsgDecodePtr pstSipMsgDe = std::make_shared<SipMsgDecode>();
    bool  bReqPres = false;
    SipGateway::ConnectionUsedModulType useModule;
    bool result = true;

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Sip msg is not valid! Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return SipGateway::kSipMsgDecodeFailed;
    }
    else if (ucResult == EnSipMsgType_Incomplete)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Sip msg is not complete! Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        if (false == sipMessageValidCheck(strBuffer))
            return SipGateway::kSipMsgDecodeFailed;
        return SipGateway::kSipMsgDecodeToSave;
    }
    else if (ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        bool bPing = ucResult == EnSipMsgType_PingRFC5626 ? true : false;
        result = pSipProxy->transferPingPongMsg(strBuffer, strBuffer.size(), bPing, pSipConnection);
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
    }

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getMethod failed. Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);         //deal with exceptional Connection
        return SipGateway::kSipMsgDecodeFailed;
    }
    bReqPres = pstSipMsgDe->isRequest() ? true : false;

    //2. 分类对待， REGISTER，SUBSCRIBE 则继续解析，填充；  其余，直接交由SipProxy服务进行转发至SIpGateway服务进行处理
    switch(ucMethod)
    {
    case EN_SIP_METHOD_REGISTER:
        if (!bReqPres)
        {
            //解析、处理 注册消息
            result = dealRegisterResponceFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
            return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
        }
        else
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Not surpported the request of  REGISTER. Msg:\n" + strBuffer);
            pSipProxy->saveExceptionConnection(pSipConnection);         //Request of REGISTER, SUBSCRIBE not supported
            return SipGateway::kSipMsgDecodeFailed;
        }
        break;

    case EN_SIP_METHOD_SUBS:
        if (!bReqPres)
        {
            //解析、处理 订阅消息
            result = dealSubscribeResponceFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
            return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
        }
        else
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Not surpported the request of  SUBSCRIBE. Msg:\n" + strBuffer);
            pSipProxy->saveExceptionConnection(pSipConnection);         //deal with exceptional Connection
            return SipGateway::kSipMsgDecodeFailed;
        }
        break;

    case EN_SIP_METHOD_NOTIFY:
        result = dealNotifyRequestFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
        break;

    case EN_SIP_METHOD_OPTIONS:
        result = dealOPTIONSRequestFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;

    // case EN_SIP_METHOD_MSG:
    //     if (!bReqPres)
    //     {
    //         result = dealMessageResponceFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
    //         return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
    //     }
    //     else
    //     {
    //         result = dealMessageRequestFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
    //         return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
    //     }
    //     break;

    default:

        SipGateway::GatewayConnection* rawPtr = static_cast<SipGateway::GatewayConnection*>(pSipConnection);
        if (0 == rawPtr)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. pSipConnection cast failed. Msg:\n" + strBuffer);
            return SipGateway::kSipMsgDecodeFailed;
        }
        SipGateway::GatewayConnectionPtr ptr(rawPtr, [](SipGateway::GatewayConnection*){});

        useModule = ptr->getUsedModule();
        if (useModule == SipGateway::SipProxyModul)
        {
            result = dealOthersSipMessgaeFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe, remoteIp, remotePort);
        }
        else
        {
            result = dealOthersSipMessgaeFromSBC_Ex(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
        }
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
        break;
    }
    return SipGateway::kSipMsgDecodeSuccessed;
}

SipGateway::SipMsgDecodeResultType SipMsg::onReceiveSipMsgFromMGCF(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const std::string & remoteIp, int remotePort)
{
    if (0 == pSipConnection)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. pSipConnection == NULL! ERROR!");
        return SipGateway::kSipMsgDecodeFailed;
    }

    if (strBuffer == "hello")
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return SipGateway::kSipMsgDecodeSuccessed;
    }

    unsigned char ucMethod;
    SipMsgDecodePtr pstSipMsgDe = std::make_shared<SipMsgDecode>();
    bool  bReqPres = false;
    bool result = true;

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Sip msg is not valid! Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return SipGateway::kSipMsgDecodeFailed;
    }
    else if (ucResult == EnSipMsgType_Incomplete)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Sip msg is not complete! Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        if (false == sipMessageValidCheck(strBuffer))
            return SipGateway::kSipMsgDecodeFailed;
        return SipGateway::kSipMsgDecodeToSave;
    }
    else if (ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        bool bPing = ucResult == EnSipMsgType_PingRFC5626 ? true : false;
        result = pSipProxy->transferPingPongMsg(strBuffer, strBuffer.size(), bPing, pSipConnection);
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
    }

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getMethod failed. Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);         //deal with exceptional Connection
        return SipGateway::kSipMsgDecodeFailed;
    }
    bReqPres = pstSipMsgDe->isRequest() ? true : false;

    SipGateway::GatewayConnection* rawPtr = static_cast<SipGateway::GatewayConnection*>(pSipConnection);
    if (0 == rawPtr)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. pSipConnection cast failed. Msg:\n" + strBuffer);
        return SipGateway::kSipMsgDecodeFailed;
    }
    SipGateway::GatewayConnectionPtr ptr(rawPtr, [](SipGateway::GatewayConnection*){});

    result = dealOthersSipMessgaeFromMgcf(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe, remoteIp, remotePort);

    return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
}

bool  SipMsg::SendRequestREGISTER_UnAoth(const  SipProxy::SipRegisterSubscribePtr & pSipReg, const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy)
{
    SipMsgEncodePtr pstSipMsgEn = std::make_shared<SipMsgEncode>();
    std::string strBranch,strFromTag, strCallIdNum,strCallHost, strRegisterHost, strClientIp, strRegisterId ;
    int   iStartCseqNum, iRegisterPort, iClientPort;
    unsigned char ucMethod, ucTptType;
    SipGateway::SipRequestMethodType sipDialogMethod;
    ST_SIP_REQ_URI stReqUri;
    int dwExpires = 3600;

    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. createReq failed .Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipMsgDecodedPart *pstDecodedPart = &pSipReg->m_sipMsgDecodePart;
    std::string sendIp;
    //get Client and Registrar ip and port
    pSipReg->getIpsAndPorts(strClientIp, iClientPort, strRegisterHost, iRegisterPort, sendIp);
    strRegisterId = pSipReg->getRegisterId();

    ucTptType = pSipReg->m_sipMsgDecodePart._viaPart._ucTptType;
    ucMethod = EN_SIP_METHOD_REGISTER;
    sipDialogMethod = SipGateway::SipDialog::SipMethodToSipDialogMethod(EN_SIP_METHOD_REGISTER);

    //fill req line
    pstDecodedPart->_reqLinePart.setReqLinePart(EN_SIP_METHOD_REGISTER,strRegisterId,strRegisterHost,iRegisterPort, EN_SIP_REQ_URI_SIP);
    pstSipMsgEn->fillReqLineBySip(ucMethod, "", strRegisterHost, iRegisterPort);

    //fill From Header
    strFromTag = pstDecodedPart->_fromPart._strTag;
    pstDecodedPart->_fromPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, strRegisterId, strRegisterId, strRegisterHost,iRegisterPort, strFromTag, ucTptType);
    bool bResult = pstSipMsgEn->fillHdrFromToByName(true,strRegisterId, strRegisterId, strRegisterHost,iRegisterPort,  strFromTag, pstDecodedPart->_fromPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrFromToByName() failed, fill From Header .Register id:" + pSipReg->getRegisterId());
    }

    //fill To Header
    pstDecodedPart->_toPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, strRegisterId,strRegisterId, strRegisterHost, iRegisterPort, "", ucTptType);
    bResult = pstSipMsgEn->fillHdrFromToByName(false, strRegisterId, strRegisterId, strRegisterHost,iRegisterPort,  "", pstDecodedPart->_toPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.call fillHdrFromToByName() failed, fill To Header .Register id:" + pSipReg->getRegisterId());
    }

    //fill Call-Id Header
    std::string strCallNum = pstDecodedPart->_callIdPart._strNumber;
    if (strCallNum.empty())
    {
        strCallNum = tars::TC_UUIDGenerator::getInstance()->genID() + "_" + std::to_string(TNOWMS);
        pstDecodedPart->_callIdPart.setCallIdPart(strCallNum, strClientIp);
    }
    bResult = pstSipMsgEn->fillHdrCallId(strCallNum, strClientIp);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.. call fillHdrCallId() failed ..Register id:" + pSipReg->getRegisterId());
    }

    stReqUri = pstSipMsgEn->_pstMsg->u.stReqLine.stReqUri;

    //fill CSeq Header
    if (pstDecodedPart->_cseqPart._iCseqVal == 0)
    {
        iStartCseqNum = 1;
        pstDecodedPart->_cseqPart.setCseqPart(iStartCseqNum, ucMethod);
    }
    iStartCseqNum = pstDecodedPart->_cseqPart._iCseqVal;
    pstSipMsgEn->fillHdrCseq(ucMethod,iStartCseqNum);

    //fill Via Header
    strBranch = SipMsgCommon::createBranch();
    std::string viaIp = sendIp.empty() ? strClientIp : sendIp;
    pstDecodedPart->_viaPart.setViaPart(viaIp, iClientPort, ucTptType, false, 0, strBranch);
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, viaIp, iClientPort, false,0,strBranch);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrVia() failed. Register id:" + pSipReg->getRegisterId());
    }

    //fill Max-Forwards Header
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Contact Header
    bResult = pstSipMsgEn->fillHdrContact(strRegisterId, strClientIp, iClientPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrContact() failed .sipUri:" + pSipReg->getSipUri()
            + ",strClientIp:" + strClientIp + ",iClientPort:" + std::to_string(int(iClientPort)));
    }

    //fill Contact Expire Param
    dwExpires = pSipReg->getExpire();
    pstSipMsgEn->parmFillContactExpires(dwExpires);

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    //fill author info
    std::string authName = pSipReg->getAuthName();
    StSipAuthor stAuth;
    stAuth._ucDigst = 1;
    stAuth._strUsername = authName;
    stAuth._strRealm =  strRegisterHost;
    stAuth._strNonce = "";
    stAuth._strResponse = "";
    stAuth._ucAlgo = EN_SIP_ALGO_VAL_MD5;
    stAuth._ucQop = EN_SIP_QOP_VAL_AUTH;
    stAuth._stReqUri = stReqUri;
    stAuth._ucReqUri = 1;

    pstSipMsgEn->fillHdrAuthor(stAuth);

    //* fill Expires header */
    pstSipMsgEn->fillHdrExpires(dwExpires);

    /* fill User-Agent header */
    std::string strName = "SipProxy";
    std::string strVersion = "1";
    std::string strComm = "0";
    pstSipMsgEn->fillHdrUserAgent(strName, strVersion, strComm);

    pstSipMsgEn->fillHdrDate();

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. call encode(), return FALSE! Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipProxy::CoreNetInfo corrNetInfo = pSipReg->getCoreNetInfo();
    std::string strConnectionId;
    SipMsgCommon::generateToSBCConnectionId(corrNetInfo.m_strSBCIp, corrNetInfo.m_port, strConnectionId);
    std::string reason;

    return pSipProxy->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), strConnectionId, true, reason);
}


bool SipMsg::SendRequestREGISTER_WithAoth(const SipProxy::SipRegisterSubscribePtr & pSipReg, const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy, const SipGateway::SipResponseCodeType & rspCode, bool refreshRegister)
{
    SipMsgEncodePtr pstSipMsgEn = std::make_shared<SipMsgEncode>();
    std::string strBranch,strFromTag, strCallIdNum,strCallHost, strRegisterHost, strClientIp, strRegisterId ;
    int   iStartCseqNum, iRegisterPort, iClientPort;
    unsigned char ucMethod, ucTptType;
    SipGateway::SipRequestMethodType sipDialogMethod;

    ST_SIP_DIGEST stDigest;
    ST_SIP_DIGEST_RSP stRsp;
    ST_SIP_REQ_URI stReqUri;
    int stateCode = 0;
    int dwExpires = 3600;

    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. createReq failed .Reigister id:" + pSipReg->getRegisterId());
        return false;
    }

    stateCode = SipGateway::SipDialog::SipDialogResponseStatusToSipStatus(rspCode);
    SipMsgDecodedPart *pstDecodedPart = &pSipReg->m_sipMsgDecodePart;
    std::string sendIp;
    //get Client and Registrar ip and port
    pSipReg->getIpsAndPorts(strClientIp, iClientPort, strRegisterHost, iRegisterPort, sendIp);
    strRegisterId = pSipReg->getRegisterId();

    ucTptType = ucTptType = pSipReg->m_sipMsgDecodePart._viaPart._ucTptType;
    ucMethod = EN_SIP_METHOD_REGISTER;
    sipDialogMethod =   SipGateway::SipDialog::SipMethodToSipDialogMethod(EN_SIP_METHOD_REGISTER);

    //fill req line
    pstSipMsgEn->fillReqLineBySip(ucMethod, "", strRegisterHost, iRegisterPort);
    stReqUri = pstSipMsgEn->_pstMsg->u.stReqLine.stReqUri;

    //fill From Header
    strFromTag = pstDecodedPart->_fromPart._strTag;
    bool bResult = pstSipMsgEn->fillHdrFromToByName(true,strRegisterId, strRegisterId, strRegisterHost,iRegisterPort,  strFromTag, pstDecodedPart->_fromPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrFromToByName() failed, fill From Header . .Reigister id:" + pSipReg->getRegisterId());
    }

    //fill To Header
    bResult = pstSipMsgEn->fillHdrFromToByName(false, strRegisterId, strRegisterId, strRegisterHost,iRegisterPort,  "", pstDecodedPart->_toPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrFromToByName() failed, fill To Header . .Reigister id:" + pSipReg->getRegisterId());
    }

    //fill Call-Id Header
    std::string strCallNum = pstDecodedPart->_callIdPart._strNumber;
    bResult = pstSipMsgEn->fillHdrCallId(strCallNum,  strClientIp);
    if (!bResult)
    {
        TLOGWARN("SipMsg",std::string(__FUNCTION__) + "() called.. call fillHdrCallId() failed ..Reigister id:" + pSipReg->getRegisterId());
    }

    //fill CSeq Header
    if (pstDecodedPart->_cseqPart._iCseqVal == 0)
    {
        iStartCseqNum = 1;
        pstDecodedPart->_cseqPart.setCseqPart(iStartCseqNum, ucMethod);
    }
    iStartCseqNum = pstDecodedPart->_cseqPart._iCseqVal;
    pstSipMsgEn->fillHdrCseq(ucMethod, iStartCseqNum);

    //fill Via Header
    strBranch = SipMsgCommon::createBranch();
    std::string viaIp = sendIp.empty() ? strClientIp : sendIp;
    pstDecodedPart->_viaPart.setViaPart(viaIp, iClientPort, ucTptType, false, 0, strBranch);
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, viaIp, iClientPort, false,0,strBranch);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrVia() failed ..Reigister id:" + pSipReg->getRegisterId());
    }

    //fill Max-Forwards Header
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Contact Header
    bResult = pstSipMsgEn->fillHdrContact(strRegisterId, strClientIp, iClientPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrContact() failed .sipUri:" + pSipReg->getSipUri()
            + ",strClientIp:" + strClientIp + ",iClientPort:"  + std::to_string(int(iClientPort)));
    }

    dwExpires = pSipReg->getExpire();
    pstSipMsgEn->parmFillContactExpires(dwExpires);

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    ////fill Authorization
    /* create digest response list */
    ABNF_LIST_CREATE(&stRsp.stDigRspLst);

    /* copy response from challenge */
    stDigest = pSipReg->getDigest();

    /* calculate md5 */
    authCalcRsp(pstSipMsgEn, pSipReg, EN_SIP_METHOD_REGISTER, &stDigest, &stReqUri, &stRsp);

    /* fill authorization header */
    pstSipMsgEn->fillHdrAuthor(stRsp, stateCode);

    //* fill Expires header */
    pstSipMsgEn->fillHdrExpires(dwExpires);

    /* fill User-Agent header */
    std::string strName = "SipProxy";
    std::string strVersion = "1";
    std::string strComm = "0";
    pstSipMsgEn->fillHdrUserAgent(strName, strVersion, strComm);

    pstSipMsgEn->fillHdrDate();

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. call encode(), return FALSE!.Reigister id:" + pSipReg->getRegisterId());
        return false;
    }

    SipProxy::CoreNetInfo corrNetInfo = pSipReg->getCoreNetInfo();
    std::string strConnectionId ;
    SipMsgCommon::generateToSBCConnectionId(corrNetInfo.m_strSBCIp, corrNetInfo.m_port, strConnectionId);
    std::string reason;

    return pSipProxy->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), strConnectionId, true, reason);
}

bool SipMsg::SendRequestSUBSCRIBE(const SipProxy::SipRegisterSubscribePtr & pSipReg, EN_SIP_EVNT_PKG eventType,  const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy, bool refreshSubs)
{
    SipMsgEncodePtr pstSipMsgEn = std::make_shared<SipMsgEncode>();
    std::string strBranch,strFromTag, strToTag, strCallIdNum,strCallHost, strRegisterHost, strClientIp, strRegisterId,strEvntId ;
    int   iStartCseqNum = 0, iRegisterPort, iClientPort;
    unsigned char ucMethod, ucTptType;
    SipGateway::SipRequestMethodType sipDialogMethod;
    std::string strReqUserInfo, strReqHost;
    int iReqPort;

    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. createReq failed .Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipMsgDecodedPart *pstDecodedPart = &pSipReg->m_sipMsgDecodePart;
    std::string sendIp;
    //get Client and Registrar ip and port
    pSipReg->getIpsAndPorts(strClientIp, iClientPort, strRegisterHost, iRegisterPort, sendIp);
    strRegisterId = pSipReg->getRegisterId();

    ucTptType = ucTptType = pSipReg->m_sipMsgDecodePart._viaPart._ucTptType;
    ucMethod = EN_SIP_METHOD_SUBS;
    sipDialogMethod =   SipGateway::SipDialog::SipMethodToSipDialogMethod(EN_SIP_METHOD_SUBS);

    //fill req line
    strReqUserInfo = pstDecodedPart->_reqLinePart._strUserName;
    strReqHost  = pstDecodedPart->_reqLinePart._strHostName;
    iReqPort  = pstDecodedPart->_reqLinePart._dwPort;

    if (refreshSubs)
    {
        pstSipMsgEn->fillReqLineByUri(EN_SIP_METHOD_SUBS, pstDecodedPart->_reqLinePart._reqUri);
    }
    else
    {
        pstSipMsgEn->fillReqLineBySip(ucMethod, strReqUserInfo, strReqHost, iReqPort);
    }

    //fill From Header
    if (pstDecodedPart->_fromPart._strTag.empty())
    {
        strFromTag = tars::TC_UUIDGenerator::getInstance()->genID();    //
    }
    else
    {
        strFromTag = pstDecodedPart->_fromPart._strTag;
    }

    pstDecodedPart->_fromPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, strRegisterId, strRegisterId, strRegisterHost,iRegisterPort, strFromTag, ucTptType);
    strFromTag = pstDecodedPart->_fromPart._strTag;

    bool bResult = pstSipMsgEn->fillHdrFromToByName(true,strRegisterId, strRegisterId, strRegisterHost,iRegisterPort,  strFromTag, pstDecodedPart->_fromPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrFromToByName() failed, fill From Header .Register id:" + pSipReg->getRegisterId());
    }

    //fill To Header
    strToTag = pstDecodedPart->_toPart._strTag;
    pstDecodedPart->_toPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, strRegisterId,strRegisterId, strRegisterHost, iRegisterPort, strToTag, ucTptType);
    bResult = pstSipMsgEn->fillHdrFromToByName(false, strRegisterId, strRegisterId, strRegisterHost,iRegisterPort, strToTag, pstDecodedPart->_toPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrFromToByName() failed, fill To Header. Register id:" + pSipReg->getRegisterId());
    }

    //fill Call-Id Header
    pstDecodedPart->_callIdPart.setCallIdPart(strRegisterId,  strClientIp);
    bResult = pstSipMsgEn->fillHdrCallId(strRegisterId,  strClientIp);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.call fillHdrCallId() failed .Register id:" + pSipReg->getRegisterId());
    }

    //fill CSeq Header
    if (pstDecodedPart->_cseqPart._iCseqVal == 0)
    {
        iStartCseqNum = 1;
        pstDecodedPart->_cseqPart.setCseqPart(iStartCseqNum, ucMethod);
    }
    else
    {
        iStartCseqNum = pstDecodedPart->_cseqPart._iCseqVal;
    }
    pstDecodedPart->_cseqPart._ucMethodType = EN_SIP_METHOD_SUBS;
    ++ pstDecodedPart->_cseqPart._iCseqVal;
    pstSipMsgEn->fillHdrCseq(ucMethod,pstDecodedPart->_cseqPart._iCseqVal );


    //fill Via Header
    strBranch = SipMsgCommon::createBranch();
    pstDecodedPart->_viaPart.setViaPart(strClientIp, iClientPort, ucTptType, false, 0, strBranch);

    pstDecodedPart->_viaPart.setViaPart(strClientIp, iClientPort, ucTptType, false, 0, strBranch);
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, strClientIp, iClientPort, false,0,strBranch);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrVia() failed .Register id:" + pSipReg->getRegisterId());
    }

    //fill Max-Forwards Header
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Contact Header
    bResult = pstSipMsgEn->fillHdrContact(strRegisterId, strClientIp, iClientPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrContact() failed .Register id:" + pSipReg->getRegisterId());
    }

    //fill Contact instance
    ST_ZOS_SSTR sipInstance;
    std::string str("urn:uuid:95CAE616-2C2E-466B-BE73-87BD2BFC3C64");
    SipMsgCommon::str2ZosSstr(str, sipInstance);
    pstSipMsgEn->ParmFillContactSipInstance(&sipInstance);


    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    pstSipMsgEn->fillHdrAllow(vectAllow);


    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    //fill Event Header
    if (pstDecodedPart->_eventPart._id.empty())
    {
        pstDecodedPart->_eventPart._id = SipMsgCommon::createBranch();
    }
    pstDecodedPart->_eventPart.setEvent(eventType, EN_SIP_EVNT_TEMP_OTHER, pstDecodedPart->_eventPart._id);
    pstSipMsgEn->fillHdrEvnt(eventType, EN_SIP_EVNT_TEMP_OTHER);
    //pstSipMsgEn->fillHdrEvntId(pstDecodedPart->_eventPart._id);


    /* fill header Accept */
    pstSipMsgEn->fillHdrAcpt(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_REGINFO_XML);

    //* fill Expires header */
    int dwExpires = 3600;
    pstSipMsgEn->fillHdrExpires(dwExpires);

    /* fill User-Agent header */
    std::string strName = "Yes";
    std::string strVersion = "Life/1.2.86";
    std::string strComm = "Android 4.4.2";
    pstSipMsgEn->fillHdrUserAgent(strName, strVersion, strComm);


    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called.  encode(), return FALSE! Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipProxy::CoreNetInfo corrNetInfo = pSipReg->getCoreNetInfo();
    std::string strConnectionId;
    SipMsgCommon::generateToSBCConnectionId(corrNetInfo.m_strSBCIp, corrNetInfo.m_port, strConnectionId);
    std::string reason;

    return pSipProxy->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(),strConnectionId,true, reason);
}


/*
* @be SIpProxy代替Client 发送注册以及订阅请求至SBC，并接收响应； SBC不会将REGISTER或者SUBSCRIBE请求发送给SIpProxy
* param [in] reqMethod,
* param [in] dlg,
* @param [out] pstEnSipMsg.
*/
bool  SipMsg::dealRegisterResponceFromSBC(const std::string &strSipMsg,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
{
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool  bReqPres = false;
    unsigned int ulStatusCode = 0;
    ST_SIP_DIGEST stDigest;
    SipProxy::SipRegisterSubscribePtr pSipReg = 0;
    SipGateway::SipResponseCodeType sipDialogStatus;
    std::string sipUri;
    bool InvalidMsg = false;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. inner error, SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);

    //get From Header ====
    std::string strDispName, strUserName, strHost;
    int  iPort = 0;
    unsigned char ucAddrType = 0;

    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType) == false)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "getHdrFromToUri() return FALSE,. SipMsg:\n" + strSipMsg);
    }

    sipUri = SipProxy::SipRegisterSubscribe::generateSipUri(strUserName, strHost);
    pSipReg = pSipProxy->getSipRegisterSubscribe(sipUri, strCallId, SipGateway::kSipRequestMethodREGISTER);
    if (pSipReg == 0)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getSipRegisterSubscribe() called. return 0! sipUri:" + sipUri + ",SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        if (iCseq < (int)pSipReg->m_sipMsgDecodePart._cseqPart._iCseqVal)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.TIME_OUT response for sipUri:" + sipUri + ",SipMsg:\n" + strSipMsg);
            return false;
        }
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    sipDialogStatus = SipGateway::SipDialog::SipStatusToSipDialogResponseStatus(ulStatusCode);

    if (ulStatusCode == 401 || ulStatusCode == 407)
    {
        if (pstSipMsgDe->getDigest(stDigest, ulStatusCode))
        {
            pSipReg->setDigest(stDigest);
            pstSipMsgDe->decodeChallengeDigest(stDigest, pSipReg);
        }
        else
        {
            InvalidMsg = true;
        }
    }
    else if (ulStatusCode == 200)
    {
        //获取Expire， 注册过期时间
        int expires = 0;
        int iContactPort  = 0;
        std::string strNextNonce, contactIp;
        if (!pstSipMsgDe->getExpires(expires))
        {
            if (!pstSipMsgDe->getHdrContactExpires(expires))
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Can not get the Expires! SipMsg:\n" + strSipMsg);
            }
        }
        pSipReg->setExpire(expires);

        // get From, To tag
        pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
        pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

        pSipReg->m_sipMsgDecodePart._toPart._strTag = strRemoteTag;
        if (pstSipMsgDe->getAuthenticationInfoNextNonce(strNextNonce))
        {
            pstSipMsgDe->getHdrContactUri(contactIp, iContactPort);
        }
    }
    return pSipProxy->onSipMessageResponse(SipGateway::kSipRequestMethodREGISTER, sipDialogStatus, pSipReg, pSipConnection, InvalidMsg) ;
}

/*
* @be SIpProxy代替Client 发送注册以及订阅请求至SBC，并接收响应； SBC不会将REGISTER或者SUBSCRIBE请求发送给SIpProxy
* param [in] reqMethod,
* param [in] dlg,
* @param [out] pstEnSipMsg.
*/
bool  SipMsg::dealSubscribeResponceFromSBC(const std::string &strSipMsg,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
{
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strUserInfo;
    bool  bReqPres = false;
    unsigned int ulStatusCode = 0;
    SipProxy::SipRegisterSubscribePtr pSipReg = 0;
    SipGateway::SipResponseCodeType sipDialogStatus;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg",  std::string(__FUNCTION__) + "() called. inner erro.SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);

    //get From Header
    std::string strDispName, strUserName, strHost;
    int  iPort = 0;
    unsigned char ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType) == false)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "getHdrFromToUri() return FALSE,. SipMsg:\n" + strSipMsg);
    }

    std::string sipUri = SipProxy::SipRegisterSubscribe::generateSipUri(strUserName, strHost);
    pSipReg = pSipProxy->getSipRegisterSubscribe(sipUri, strCallId, SipGateway::kSipRequestMethodSUBSCRIBE);
    if (pSipReg == 0)
    {
        TLOGWARN("SipMsg",  std::string(__FUNCTION__) + "() called. getSipRegisterSubscribe()  return 0! sipUri:" + sipUri + " ,strSipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    sipDialogStatus = SipGateway::SipDialog::SipStatusToSipDialogResponseStatus(ulStatusCode);
    if (ulStatusCode == 200)
    {
        //获取Expire， 订阅过期时间
        int expires = 0;
        if (!pstSipMsgDe->getExpires(expires))
        {
            if (!pstSipMsgDe->getHdrContactExpires(expires))
            {
                TLOGWARN("SipMsg",  std::string(__FUNCTION__) + "() called.  Can not get the Expires! strSipMsg:\n" + strSipMsg);
            }
        }

        // get From, To tag
        pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
        pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
        pSipReg->m_sipMsgDecodePart._toPart._strTag = strRemoteTag;

        pSipReg->setExpire(expires);
        pSipReg->setLastRegisterTime(TNOWMS);

        std::string strUri;
        if(!pstSipMsgDe->getContactUri(strUri))
        {
            TLOGWARN("SipMsg",  std::string(__FUNCTION__) + "() called.pstSipMsgDe->getContactUri()  return FALSE! strSipMsg:\n" + strSipMsg);
        }
        pSipReg->m_sipMsgDecodePart._reqLinePart.setReqUri(strUri);
    }

    return pSipProxy->onSipMessageResponse(SipGateway::kSipRequestMethodSUBSCRIBE, sipDialogStatus, pSipReg, pSipConnection) ;
}

/*
* @be 为防止 Connection 的异常，需要将接收到的除上述 REGISTER 和SUBSCRIBE响应之外的SIP消息进一步处理，然后转发至SIpGateway服务进程
* param [in] reqMethod,
* param [in] dlg,
* @param [out] pstEnSipMsg.
*/
bool SipMsg::dealOthersSipMessgaeFromSBC(const std::string &strSipMsg,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort)
{
    int   iSceq = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strSipDialogId, strSipDialogId2, strToSBCConectionId;
    std::string strCallIdHost;
    bool bReqPres = true, bSipDialogIdExis = false, bSipDialogIdExis2 = false, bRefresh = false;
    SipMsgDecodedPart * pstDecodedPart = 0;
    bool bExceptionConnecntion = false;
    bool bRpcToSip = false;
    bool bInitInvite = false;
    SipProxy::SipDialogRecord sipDialogRecord;
    std::map<std::string, std::string> others;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. inner error.SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        bExceptionConnecntion = true;
        return false;
    }
    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    strCallIdHost = strCallId + "@" + strCallHost;

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getMethod failed.SipMsg:\n" + strSipMsg);
        bExceptionConnecntion = true;
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    bReqPres = pstSipMsgDe->isRequest() ? true : false;
    bRpcToSip = strCallHost == SipGateway::SIP_GW_SERVICE_NAME ? true : false;

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //check Request or responce
    if(bReqPres)
    {
        if (ucMethod == EN_SIP_METHOD_INVITE)
        {
            if (bRpcToSip)
            {
                bRefresh = pSipProxy->checkCallIdHostExist(strCallIdHost, sipDialogRecord);
                strSipDialogId = sipDialogRecord.m_sipDialogId;
                if (bRefresh)
                {
                    TLOGINFO("SipMsg", std::string(__FUNCTION__) + "() called. Recv INVITE msg, A Refresh INVITE! checkCallIdHostExist() return  TRUE! .strSipDialogId:" + sipDialogRecord.m_sipDialogId);
                }
                else
                {
                    TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.. Recv INVITE msg, RPC->SIP, checkCallIdHostExist() return FALSE ! .strCallIdHost:" + strCallIdHost  + ",msg:\n" + strSipMsg);
                    bExceptionConnecntion = true;
                    pSipProxy->saveExceptionConnection(pSipConnection);
                    return false;
                }
            }
            else
            {
                strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
                bSipDialogIdExis = pSipProxy->checkCallIdHostExist(strCallIdHost, sipDialogRecord);
                bInitInvite = bSipDialogIdExis ? false : true;

                TLOGINFO("SipMsg", std::string(__FUNCTION__) + "() called. Recv INVITE Request msg, SIP->RPC, checkCallIdHostExist() return: "
                    + std::to_string(bSipDialogIdExis) + ".strSipDialogId:" + strSipDialogId);

                //get From Header
                std::string strDispName, strUserName, strHost;
                int iPort = 0;
                unsigned char ucAddrType = 0;
                if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType) == false)
                {
                    TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Recv INVITE msg, SIP->RPC, getHdrFromToUri() get To Header failed! sipMsg:\n"+strSipMsg);
                    pSipProxy->saveExceptionConnection(pSipConnection);
                    return false;
                }

                pSipProxy->getConnectionIdToSBC(strUserName, strHost, strToSBCConectionId);
                if(strToSBCConectionId.empty())
                    strToSBCConectionId = remoteIp + ":" + std::to_string(remotePort);

                if (bInitInvite)
                {
                    //P-Called-Party-ID
                    std::string strDispInfo;
                    int dwPort = 0;
                    if (pstSipMsgDe->getHdrPCalledPartyID(strDispName, strHost, iPort))
                    {
                        others.insert(make_pair(SipProxy::SIP_PROXY_PARAMS_SIP_TO, strDispName));
                        others.insert(make_pair(SipProxy::SIP_PROXY_PARAMS_SIP_HOST, strHost));
                    }
                    else
                    {
                        int iReqUriType = 0;
                        pstSipMsgDe->getReqUri(strDispName, strHost, iPort, iReqUriType);
                        if (!strDispName.empty() && !strHost.empty())
                        {
                            others.insert(make_pair(SipProxy::SIP_PROXY_PARAMS_SIP_TO, strDispName));
                            others.insert(make_pair(SipProxy::SIP_PROXY_PARAMS_SIP_HOST, strHost));
                        }
                    }

                    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
                        others.insert(make_pair(SipProxy::SIP_PROXY_PARAMS_SIP_FROM, strUserName));
                    others.insert(make_pair(SipProxy::SIP_PROXY_PARAMS_SIP_CALLID, strCallIdHost));
                }
            }
        }
        else
        {
            bSipDialogIdExis = pSipProxy->checkCallIdHostExist(strCallIdHost, sipDialogRecord);
            strSipDialogId = sipDialogRecord.m_sipDialogId;
            if (!bSipDialogIdExis)
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Cann't find A SipDialog with callId : " + strCallIdHost + " ,sipMsg:\n"+strSipMsg);
                bExceptionConnecntion = true;
                pSipProxy->saveExceptionConnection(pSipConnection);
            }
        }
    }
    else /* print response line */
    {
        bSipDialogIdExis = pSipProxy->checkCallIdHostExist(strCallIdHost, sipDialogRecord);
        strSipDialogId = sipDialogRecord.m_sipDialogId;
        if (bSipDialogIdExis == false)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.  333 call checkCallIdHostExist() failed to get the SipDialogPtr when received a Responce, with SipDialogId : "
                + strSipDialogId + ",strCallIdHost:" + strCallIdHost + " ,SipMsg:\n" + strSipMsg);
            bExceptionConnecntion = true;
            pSipProxy->saveExceptionConnection(pSipConnection);
            return false;
        }
    }

    pSipProxy->onOthersSipMessageTransfer(strSipMsg, strSipMsg.size(), strSipDialogId, strCallIdHost, bInitInvite, pSipConnection,bExceptionConnecntion, strToSBCConectionId, bReqPres, ucMethod, others);
    return true;
}

bool SipMsg::dealOthersSipMessgaeFromSBC_Ex(const std::string &strSipMsg,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
{
    std::string strCallIdHost;
    bool bReqPres = true;
    unsigned char ucMethod;
    unsigned int ulStatusCode = 0;
    std::string fromUserName, toUserName, remoteHost, toSBCConnetioId;
    int iPort = 0;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.inner error.SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }
    // get Call-Id Header
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    strCallIdHost = strCallId + "@" + strCallHost;

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg",  std::string(__FUNCTION__) + "() called. getMethod failed.SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    bReqPres = pstSipMsgDe->isRequest() ? true : false;
    if(false == bReqPres)
    {
        pstSipMsgDe->getRspStatusCode(ulStatusCode);
    }

    //get From Header
    std::string fromDispName, toDispName, strHost;
    unsigned char ucAddrType = 0;
    pstSipMsgDe->getHdrFromToUri(true, fromDispName, fromUserName, strHost, iPort, ucAddrType);
    //pstSipMsgDe->getHdrFromToUri(false, toDispName, toUserName, strHost, iPort, ucAddrType);

    //P-Called-Party-ID >> Request
    if(pstSipMsgDe->getHdrPCalledPartyID(toUserName, strHost, iPort))
    {
        remoteHost = strHost;
    }
    if (remoteHost.empty())
    {
        int iReqUriType = 0;
        pstSipMsgDe->getReqUri(toUserName, strHost, iPort, iReqUriType);
    }

    //get Via Header
    unsigned char ucTptType = 0;
    std::string strViaUri, strRecvHost;
    int iRport = 0;
    if (pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri, iPort, strRecvHost, iRport))
    {
        if (remoteHost.empty())
        {
            remoteHost = strViaUri;
        }
        SipMsgCommon::generateToSBCConnectionId(strViaUri, iPort, toSBCConnetioId);
    }

    if(toSBCConnetioId.empty())
    {
        //get Contact Header
        std::string strHostName;
        ZCHAR zcTpt = 0;
        if (pstSipMsgDe->getHdrContactSipUri(fromUserName, strHostName, iPort, zcTpt) && iPort > 0)
        {
            SipMsgCommon::generateToSBCConnectionId(strHostName, iPort, toSBCConnetioId);
        }
    }

    pSipProxy->onOthersSipMessageTransferEx(strSipMsg, strSipMsg.size(), strCallIdHost, bReqPres, ucMethod, ulStatusCode, fromUserName, toUserName, remoteHost, pSipConnection, toSBCConnetioId);
    return true;
}

bool SipMsg::dealOthersSipMessgaeFromMgcf(const std::string &strSipMsg,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp,  int remotePort)
{
    int   iSceq = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strSipDialogId, strSipDialogId2, strToSBCConectionId;
    std::string strCallIdHost;
    bool bReqPres = true, bSipDialogIdExis = false, bSipDialogIdExis2 = false, bRefresh = false;
    SipMsgDecodedPart * pstDecodedPart = 0;
    bool bExceptionConnecntion = false;
    bool bRpcToSip = false;
    bool bInitInvite = false;
    SipProxy::SipDialogRecord sipDialogRecord;
    unsigned int ulStatusCode = 0;
	std::string strRspReason;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. inner error.SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        bExceptionConnecntion = true;
        return false;
    }
    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    strCallIdHost = strCallId + "@" + strCallHost;

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getMethod failed.SipMsg:\n" + strSipMsg);
        bExceptionConnecntion = true;
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    bReqPres = pstSipMsgDe->isRequest() ? true : false;

    std::map<std::string, std::string> params;
    params["Remote.Ip"] = remoteIp;
    params["Remote.Port"] = std::to_string(remotePort);

    if (ucMethod == EN_SIP_METHOD_OPTIONS)
    {
        pSipProxy->onOthersSipMessageTransfer(strSipMsg, strSipMsg.size(), strSipDialogId, strCallIdHost, bInitInvite,
            pSipConnection,bExceptionConnecntion, remoteIp, bReqPres, ucMethod, params);
        return true;
    }

    if(bReqPres)
    {
        if (ucMethod == EN_SIP_METHOD_INVITE)
        {
            strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
            bSipDialogIdExis = pSipProxy->checkCallIdHostExist(strCallIdHost, sipDialogRecord);
            bInitInvite = bSipDialogIdExis ? false : true;

            //get To Header
            std::string strDispName, strUserName, strHost;
            int iPort = 0;
            unsigned char ucAddrType = 0;
            if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType) == false)
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Recv INVITE msg, SIP->RPC, getHdrFromToUri() get To Header failed! sipMsg:\n"+strSipMsg);
                pSipProxy->saveExceptionConnection(pSipConnection);
                return false;
            }
            pSipProxy->getConnectionIdToSBC(strUserName, strHost, strToSBCConectionId);
            TLOGINFO("SipMsg", std::string(__FUNCTION__) + "() called. Recv INVITE Request msg, checkCallIdHostExist() return: "
                + std::to_string(bSipDialogIdExis) + ".strSipDialogId:" + strSipDialogId);
        }
        else
        {
            bSipDialogIdExis = pSipProxy->checkCallIdHostExist(strCallIdHost, sipDialogRecord);
            strSipDialogId = sipDialogRecord.m_sipDialogId;
            if (!bSipDialogIdExis)
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Cann't find A SipDialog with callId : " + strCallIdHost + " ,sipMsg:\n"+strSipMsg);
                bExceptionConnecntion = true;
                pSipProxy->saveExceptionConnection(pSipConnection);
            }
        }
    }
    else /* print response line */
    {
        bSipDialogIdExis = pSipProxy->checkCallIdHostExist(strCallIdHost, sipDialogRecord);
        strSipDialogId = sipDialogRecord.m_sipDialogId;
        if (bSipDialogIdExis == false)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.  333 call checkCallIdHostExist() failed to get the SipDialogPtr when received a Responce, with SipDialogId : "
                + strSipDialogId + ",strCallIdHost:" + strCallIdHost + ",SipMsg:\n" + strSipMsg);
            bExceptionConnecntion = true;
            pSipProxy->saveExceptionConnection(pSipConnection);
            return false;
        }
    }

    pSipProxy->onOthersSipMessageTransfer(strSipMsg, strSipMsg.size(), strSipDialogId, strCallIdHost, bInitInvite,
        pSipConnection,bExceptionConnecntion, strToSBCConectionId, bReqPres, ucMethod, params);

    return true;
}

bool SipMsg::dealNotifyRequestFromSBC(const std::string &strBuffer,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
{
    int   iSceq = 0;
    unsigned char ucMethod = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strRegisterId, strPassWord, strSipProxyId;
    bool bReqPres = true;
    SipMsgDecodedPart * pstDecodedPart = 0;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Sip msg is not valid or complete.SipMsg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }
    strSipProxyId = pSipProxy->getSipProxyId();
    //get From Header
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    std::string strDispName, strUserName, strHost;
    int  iPort = 0;
    unsigned char ucUriType;
    if(!pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucUriType))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.  call getMethod failed.SipMsg:\n" + strBuffer);
    }

    std::string sipUri =  SipProxy::SipRegisterSubscribe::generateSipUri(strUserName, strHost);
    SipProxy::SipRegisterSubscribePtr sipRegPtr = SipProxy::SipRegisterSubscribe::create(sipUri, strUserName, "", "", strSipProxyId, SipGateway::kSipRequestMethodNOTIFY, 300);
    if (0 == sipRegPtr)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. create SipRegisterSubscribe failed.SipMsg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return 0;
    }
    pstDecodedPart = &sipRegPtr->m_sipMsgDecodePart;
    pstDecodedPart->_fromPart.setFromToPart(ucUriType,strDispName,strUserName, strHost, iPort,strLocalTag, EN_SIP_TPT_UDP);

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucUriType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucUriType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucUriType,strDispName,strUserName, strHost, iPort,strRemoteTag, EN_SIP_TPT_UDP);
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strCallId, strCallHost);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getMethod failed.SipMsg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    bReqPres = pstSipMsgDe->isRequest() ? true : false;
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart.setCseqPart(iCseq, ucMethod);
    }

    //get Via Header
    std::string strViaUri, strViaBrach, strRecvHost;
    ZUCHAR ucTptType;
    int iRport = 0;
    if (pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri,iPort, strRecvHost,iRport))
    {
        pstDecodedPart->_viaPart._iPort = iPort;
        pstDecodedPart->_viaPart._strHostName = strViaUri;
        pstDecodedPart->_viaPart._ucTptType = ucTptType;
        pstDecodedPart->_viaPart._iRport = iRport;
    }
    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }

    //get Max-Forward Header
    int iMaxForwards = 0;
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
    {
        pstDecodedPart->_MaxForwards = iMaxForwards;
    }

    //get  Event Header
    ZUCHAR ucPkgType, ucTempType;
    std::string strPkgTypeName, strTmpTypeName;
    if (pstSipMsgDe->getHdrEvent(ucPkgType,strPkgTypeName,ucTempType,strTmpTypeName))
    {
        std::string eventId;
        pstSipMsgDe->getEventId(eventId);
        pstDecodedPart->_eventPart.setEvent(ucPkgType, ucTempType, eventId);
    }

    //get Subscription-State Header
    ZUCHAR  ucStaType;
    ZUINT  iExpires;
    ZUCHAR  ucReason = 0;
    std::string  strTypeName;
    if (pstSipMsgDe->getHdrSubsSta( &ucStaType,strTypeName,	&iExpires))
    {
        pstDecodedPart->_subscriptionState.setSubscriptionState((SubscriprionStateType)ucStaType,strTypeName, iExpires)	;
    }
    if (pstSipMsgDe->getSubsReason(ucReason))
    {
        pstDecodedPart->_subscriptionState.setSubscriptionTerminatReason(ucReason);
    }

    std::string	strXmlBody;
    XmlReginfo xmlReginfo;
    //get Xml Body
    pstSipMsgDe->getXmlBodyString(strXmlBody);

    //tinyXml2 decode Xml
    if (!pstSipMsgDe->decodeNotifyXmlBodyTinyXml(strXmlBody,xmlReginfo))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. decodeNotifyXmlBodyTinyXml() call.return FALSE! SipMsg:\n" + strBuffer);
    }
    sipRegPtr->m_sipMsgDecodePart._xmlReginfo = xmlReginfo;
    return pSipProxy->onSipMessageRequest(enMethodType, sipRegPtr, pSipConnection);
}

bool SipMsg::dealOPTIONSRequestFromSBC(const std::string &strBuffer,int iBufLen,  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
{
    int   iSceq = 0;
    unsigned char ucMethod = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strRegisterId, strPassWord, strSipProxyId;
    bool bReqPres = true;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string strDispName, strUserName, strHost;
    int  iPort = 0;
    unsigned char ucUriType;
    SipGateway::SipRequestMethodType enMethodType;
    std::string    strHostName;
    ZCHAR  zcTpt;
    vector<ST_SIP_VIA_PARMS> params;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. inner error.SipMsg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }
    strSipProxyId = pSipProxy->getSipProxyId();
    //get From Header
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    if(!pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucUriType))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.  call getMethod failed.SipMsg:\n" + strBuffer);
    }

    std::string sipUri =  SipProxy::SipRegisterSubscribe::generateSipUri(strUserName, strHost);
    SipProxy::SipRegisterSubscribePtr sipRegPtr = SipProxy::SipRegisterSubscribe::create(sipUri, strUserName, "", "", strSipProxyId, SipGateway::kSipRequestMethodOPTIONS, 300);
    if (0 == sipRegPtr)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. create SipRegisterSubscribe failed.SipMsg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return 0;
    }
    pstDecodedPart = &sipRegPtr->m_sipMsgDecodePart;
    pstDecodedPart->_fromPart.setFromToPart(ucUriType,strDispName,strUserName, strHost, iPort,strLocalTag, EN_SIP_TPT_UDP);

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucUriType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucUriType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucUriType,strDispName,strUserName, strHost, iPort,strRemoteTag, EN_SIP_TPT_UDP);
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstDecodedPart->_callIdPart.setCallIdPart(strCallId, strCallHost);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getMethod failed.SipMsg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }
    bReqPres = pstSipMsgDe->isRequest() ? true : false;

    //method converter
    enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart.setCseqPart(iCseq, ucMethod);
    }

    //get Via Header
    std::string strViaUri, strViaBrach, strRecvHost;
    ZUCHAR ucTptType;
    int iRport = 0;
    if (pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri, iPort, strRecvHost, iRport))
    {
        pstDecodedPart->_viaPart.setViaPart(strViaUri, iPort, ucTptType, false, iRport, "");
    }
    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }
    pstSipMsgDe->getHdrViaParams(params);
    pstDecodedPart->_viaPart.setViaParams(params);

    //get Max-Forward Header
    int iMaxForwards = 0;
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
    {
        pstDecodedPart->_MaxForwards = iMaxForwards;
    }

    //get Contact Header
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt))
    {
        pstDecodedPart->_fromPart.setFromToContact(strHostName, iPort);
    }

    return pSipProxy->onSipMessageRequest(enMethodType, sipRegPtr, pSipConnection);
}

bool SipMsg::dealOPTIONSResponseFromSBC(const std::string &strSipMsg,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
{
    std::string strCallId, strCallHost;
    unsigned int ulStatusCode = 0;
    SipProxy::SipRegisterSubscribePtr pSipReg = 0;
    SipGateway::SipResponseCodeType sipDialogStatus;
    bool InvalidMsg = true;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. inner error, SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);

    //get From Header ====
    std::string strDispName, strUserName, strHost;
    int  iPort = 0;
    unsigned char ucAddrType = 0;

    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType) == false)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "getHdrFromToUri() return FALSE,. SipMsg:\n" + strSipMsg);
    }

    std::string sipUri = SipProxy::SipRegisterSubscribe::generateSipUri(strUserName, strHost);
    pSipReg = pSipProxy->getSipRegisterSubscribe(sipUri, strCallId, SipGateway::kSipRequestMethodOPTIONS);
    if (pSipReg == 0)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getSipRegisterSubscribe() called. return 0! sipUri:" + sipUri + ",SipMsg:\n" + strSipMsg);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return false;
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    sipDialogStatus = SipGateway::SipDialog::SipStatusToSipDialogResponseStatus(ulStatusCode);
    return pSipProxy->onSipMessageResponse(SipGateway::kSipRequestMethodOPTIONS, sipDialogStatus, pSipReg, pSipConnection, InvalidMsg) ;
}


bool SipMsg::sendRegisterSubscribeResponse(const SipGateway::SipResponseCodeType rspCode, const SipGateway::SipRequestMethodType reqMethod, const SipProxy::SipRegisterSubscribePtr & pSipReg,  const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy)
{
    SipMsgEncodePtr pstSipMsgEn;
    SipMsgDecodedPart * pstDecodedPart = 0;
    EN_SIP_METHOD ucMethod ;
    int iStatus;

    if (pSipReg == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called., Input parameter pSipReg is NULL.");
        return false;
    }

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createRsp())
    {
        //encode responce
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. createRsp failed .Register id:" + pSipReg->getRegisterId());
        return false;
    }

    pstDecodedPart = &pSipReg->m_sipMsgDecodePart;

    //method converter
    ucMethod = SipGateway::SipDialog::SipDialogMethodToSipMethod(reqMethod);
    iStatus =  SipGateway::SipDialog::SipDialogResponseStatusToSipStatus(rspCode);

    //fill status line
    pstSipMsgEn->fillStatusLine(iStatus);

    //fill From Header
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort,pFromPart->_strTag, pFromPart->_ucAddrType);

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    pToPart->_strTag = SipMsgCommon::createToTag();
    pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
        pToPart->_strTag, pToPart->_ucAddrType);

    //fill Via Header
    pstSipMsgEn->fillHdrVia(pstDecodedPart->_viaPart._ucTptType,pstDecodedPart->_viaPart._strHostName,pstDecodedPart->_viaPart._iPort,
        pstDecodedPart->_viaPart._bRport,pstDecodedPart->_viaPart._iRport,pstDecodedPart->_viaPart._strBranch);

    //fill Call-Id Header
    pstSipMsgEn->fillHdrCallId(pstDecodedPart->_callIdPart._strNumber, pstDecodedPart->_callIdPart._strHost);

    //fill CSeq Header
    pstSipMsgEn->fillHdrCseq(ucMethod, pstDecodedPart->_cseqPart._iCseqVal);

    //fill Max-Forwards
    int iMaxForwards = 0;
    pstSipMsgEn->fillHdrMaxForwards(pstDecodedPart->_MaxForwards);

    //fill Contact,如何构建？ 用To直接构建TEL类型？
    //pstSipMsgEn->fillHdrContactByTel(pstDecodedPart->_toPart._strDispName, pstDecodedPart->_toPart._strUserName);
    //pstSipMsgEn->fillHdrContact(pstDecodedPart->_fromPart._strUserName,"192.168.2.125", 5060, EN_SIP_TPT_UDP);

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.clear();
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. encode() return FALSE! Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipProxy::CoreNetInfo corrNetInfo = pSipReg->getCoreNetInfo();
    std::string strConnectionId;
    SipMsgCommon::generateToSBCConnectionId(corrNetInfo.m_strSBCIp, corrNetInfo.m_port, strConnectionId);
    std::string reason;

    return pSipProxy->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), strConnectionId, false, reason);
}

bool SipMsg::sendOPTIONSResponse(const SipProxy::SipRegisterSubscribePtr & pSipReg,
                                 const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy)
{
    SipMsgEncodePtr pstSipMsgEn;
    SipMsgDecodedPart * pstDecodedPart = 0;
    EN_SIP_METHOD ucMethod ;
    vector<int> vectAllow;
    vector<int> viSuptFlag;
    vector<std::string> vectAcptEncodings;
    vector<std::string> vectLangs;

    if (pSipReg == 0)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called, Input parameter pSipReg is NULL.");
        return false;
    }

    pstSipMsgEn = std::make_shared<SipMsgEncode>();
    if (!pstSipMsgEn->createRsp())
    {
        //encode responce
        TLOGERROR("SipMsg",  std::string(__FUNCTION__)+"() called, call createRsp failed .Register id:" + pSipReg->getRegisterId());
        return false;
    }
    pstDecodedPart = &pSipReg->m_sipMsgDecodePart;

    //method converter
    ucMethod = EN_SIP_METHOD_OPTIONS;

    //fill status line
    pstSipMsgEn->fillStatusLine(200);

    //fill From Header
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort,pFromPart->_strTag, pFromPart->_ucAddrType);

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    pToPart->_strTag = SipMsgCommon::createToTag();
    pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
        pToPart->_strTag, pToPart->_ucAddrType);

    //fill Via Header
    pstSipMsgEn->fillHdrVia(pstDecodedPart->_viaPart._ucTptType,pstDecodedPart->_viaPart._strHostName,pstDecodedPart->_viaPart._iPort,
        pstDecodedPart->_viaPart._bRport,pstDecodedPart->_viaPart._iRport,pstDecodedPart->_viaPart._strBranch);
    pstSipMsgEn->fillHdrViaExtParams(pstDecodedPart->_viaPart._paramsExt);

    //fill Call-Id Header
    pstSipMsgEn->fillHdrCallId(pstDecodedPart->_callIdPart._strNumber, pstDecodedPart->_callIdPart._strHost);

    //fill CSeq Header
    pstSipMsgEn->fillHdrCseq(ucMethod, pstDecodedPart->_cseqPart._iCseqVal);

    //fill Max-Forwards
    int iMaxForwards = 0;
    pstSipMsgEn->fillHdrMaxForwards(pstDecodedPart->_MaxForwards);

    //fill Contact,如何构建？ 用To直接构建TEL类型？
    //pstSipMsgEn->fillHdrContactByTel(pstDecodedPart->_toPart._strDispName, pstDecodedPart->_toPart._strUserName);
    //pstSipMsgEn->fillHdrContact(pstDecodedPart->_fromPart._strUserName,"192.168.2.125", 5060, EN_SIP_TPT_UDP);

    //fill Allow
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_REGISTER);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INFO);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_TMR);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_REPLACES);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_EARLY_SESS);
    pstSipMsgEn->fillHdrPEarlyMedia(EN_SIP_P_EARLY_MEDIA_PARM_SUPPORTED);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    /* fill header accept */
    pstSipMsgEn->fillHdrAcpt(EN_SIP_MTYPE_DISC_APP,EN_SIP_MSUBTYPE_SDP);

    //fill header accept-encoding
    vectAcptEncodings.push_back("gzip");
    pstSipMsgEn->fillHdrAcptEncoding(vectAcptEncodings);

    //fill header accept-languages
    vectLangs.push_back("en");
    pstSipMsgEn->fillHdrAcptLang(vectLangs);

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg",  std::string(__FUNCTION__)+"() called, encode() return FALSE! Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipProxy::CoreNetInfo corrNetInfo = pSipReg->getCoreNetInfo();
    std::string strConnectionId;
    SipMsgCommon::generateToSBCConnectionId(corrNetInfo.m_strSBCIp, corrNetInfo.m_port, strConnectionId);
    std::string reason;

    return pSipProxy->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), strConnectionId, false, reason);
}

bool SipMsg::SendRequestOPTIONS(const  SipProxy::SipRegisterSubscribePtr & pSipReg, const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy)
{
    SipMsgEncodePtr pstSipMsgEn = std::make_shared<SipMsgEncode>();
    std::string strBranch,strFromTag, strCallIdNum,strCallHost, strRegisterHost, strClientIp, strRegisterId ;
    int iStartCseqNum = 0, iRegisterPort = 0, iClientPort = 0;
    unsigned char ucMethod = 0, ucTptType = 0;
    int dwExpires = 3600;

    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. createReq failed .Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipMsgDecodedPart *pstDecodedPart = &pSipReg->m_sipMsgDecodePart;
    std::string sendIp;
    //get Client and Registrar ip and port
    pSipReg->getIpsAndPorts(strClientIp, iClientPort, strRegisterHost, iRegisterPort, sendIp);
    strRegisterId = pSipReg->getRegisterId();

    ucTptType = pSipReg->m_sipMsgDecodePart._viaPart._ucTptType;
    ucMethod = EN_SIP_METHOD_OPTIONS;

    //fill req line
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod,strRegisterId,strRegisterHost,iRegisterPort, EN_SIP_REQ_URI_SIP);
    pstSipMsgEn->fillReqLineBySip(ucMethod, "", strRegisterHost, iRegisterPort);

    //fill From Header
    strFromTag = tars::TC_UUIDGenerator::getInstance()->genID();

    pstDecodedPart->_fromPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, strRegisterId, strRegisterId, strRegisterHost,iRegisterPort, strFromTag, ucTptType);
    bool bResult = pstSipMsgEn->fillHdrFromToByName(true,strRegisterId, strRegisterId, strRegisterHost,iRegisterPort,  strFromTag, pstDecodedPart->_fromPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrFromToByName() failed, fill From Header .Register id:" + pSipReg->getRegisterId());
    }

    //fill To Header
    pstDecodedPart->_toPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, strRegisterId,strRegisterId, strRegisterHost, iRegisterPort, "", ucTptType);
    bResult = pstSipMsgEn->fillHdrFromToByName(false, strRegisterId, strRegisterId, strRegisterHost,iRegisterPort,  "", pstDecodedPart->_toPart._ucAddrType);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.call fillHdrFromToByName() failed, fill To Header .Register id:" + pSipReg->getRegisterId());
    }

    //fill Call-Id Header
    std::string strRandomCallNumber = tars::TC_UUIDGenerator::getInstance()->genID() + "_" +  std::to_string(TNOWMS);
    pstDecodedPart->_callIdPart.setCallIdPart(strRandomCallNumber,  strClientIp);
    bResult = pstSipMsgEn->fillHdrCallId(strRandomCallNumber,  strClientIp);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.. call fillHdrCallId() failed ..Register id:" + pSipReg->getRegisterId());
    }

    //fill CSeq Header
    if (pstDecodedPart->_cseqPart._iCseqVal == 0)
    {
        iStartCseqNum = 1;
        pstDecodedPart->_cseqPart.setCseqPart(iStartCseqNum, ucMethod);
    }
    else
    {
        iStartCseqNum = pstDecodedPart->_cseqPart._iCseqVal + 1;
        pstDecodedPart->_cseqPart._iCseqVal = iStartCseqNum;
    }
    pstSipMsgEn->fillHdrCseq(ucMethod,iStartCseqNum);

    //fill Via Header
    strBranch = SipMsgCommon::createBranch();
    std::string viaIp = sendIp.empty() ? strClientIp : sendIp;
    pstDecodedPart->_viaPart.setViaPart(viaIp, iClientPort, ucTptType, false, 0, strBranch);
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, viaIp, iClientPort, false,0,strBranch);
    if (!bResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrVia() failed. Register id:" + pSipReg->getRegisterId());
    }

    //fill Max-Forwards Header
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill Allow
    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INFO);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_NOTIFY);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_MSG);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_REFER);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    //fill Supported
    vector<int> viSuptFlag;
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_GRUU);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_TMR);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_REPLACES);
    viSuptFlag.push_back(SIP_TAG_OPT_ADD_EARLY_SESS);
    pstSipMsgEn->fillHdrSupted(viSuptFlag);

    /* fill P-Early-Media Header  EN_SIP_P_EARLY_MEDIA_PARM*/
    pstSipMsgEn->fillHdrPEarlyMedia(EN_SIP_P_EARLY_MEDIA_PARM_SUPPORTED);

    //fill Date Header
    pstSipMsgEn->fillHdrDate();

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. call encode(), return FALSE! Register id:" + pSipReg->getRegisterId());
        return false;
    }

    SipProxy::CoreNetInfo corrNetInfo = pSipReg->getCoreNetInfo();
    std::string strConnectionId;
    SipMsgCommon::generateToSBCConnectionId(corrNetInfo.m_strSBCIp, corrNetInfo.m_port, strConnectionId);
    std::string reason;

    return pSipProxy->sendEncodedMessage(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), strConnectionId, true, reason);
}

bool SipMsg::getSipMsgSipDialogId(const std::string & strSipMsg, std::string & sipDialogId,  std::string & callId, unsigned char & ucMethod, bool & req,unsigned int & ulStatusCode)
{
    SipMsgDecodePtr pstSipMsgDe = 0;
    int   iSceq = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;

    pstSipMsgDe = std::make_shared<SipMsgDecode>();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip msg is not valid or complete. strSipMsg :\n" + strSipMsg);
        return false;
    }
    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, getMethod failed.strSipMsg :\n" + strSipMsg);
        return false;
    }
    req = pstSipMsgDe->isRequest() ? true : false;
    if (false == req)
    {
        pstSipMsgDe->getRspStatusCode(ulStatusCode);
    }

    sipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    callId = strCallId + "@" + strCallHost;
    return true;
}

bool SipMsg::getMediaIpAndPortsOfSDPinSipMsg(const std::string & strSipMsgWithSDP,  vector<SDPMediaInfo> & vectSdpMediaInfo)
{
    SipMsgDecodePtr pstSipMsgDe = std::make_shared<SipMsgDecode>();
    return pstSipMsgDe->getSBCMediaIpAndPort(strSipMsgWithSDP, vectSdpMediaInfo);
}

bool SipMsg::CreateSipMsgFromSDP(const std::string & strBody, std::string & strSipMsg)
{
    if (strBody.empty())
        return false;

    unsigned ucMethod;
    std::string strHost, strCallHost;
    std::string strFromDispname, strToDispName, strContactHost;
    std::string strFromTag, strBranch;
    std::string strUAName, strUAVersion, strUAComm;
    int iPort;

    SipMsgEncodePtr pstSipMsgEn = std::make_shared<SipMsgEncode>();
    ucMethod = EN_SIP_METHOD_INVITE;
    strFromDispname = "13957854053";
    strFromTag = tars::TC_UUIDGenerator::getInstance()->genID();
    strToDispName = "8616000000330";
    strHost = "gd.ims.mnc000.mcc460.3gppnetwork.org";
    strContactHost = "192.168.8.100";
    iPort = 53890;
    strCallHost = "192.168.8.100";
    strUAName = "CPM-client";
    strUAVersion = "OMA1.0";

    bool result = pstSipMsgEn->createReq();
    if (false == result)
    {
        return false;
    }

    /* fill request line */
    pstSipMsgEn->fillReqLineBySip(ucMethod,strFromDispname,strHost,0);
    /* fill from header */
    pstSipMsgEn->fillHdrFromToByName(true,strFromDispname, strFromDispname, strHost, 0, strFromTag);
    /* fill to header */
    pstSipMsgEn->fillHdrFromToByName(false, strToDispName, strToDispName, strHost, 0, "");

    ST_ZOS_SSTR stTemBody;
    pstSipMsgEn->fillBodySDPDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strBody);         //有问题！！！！！

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called, encode(), return FALSE! strBody:\n" + strBody);
        return false;
    }

    strSipMsg = pstSipMsgEn->_strMsg;
    SipMsgDecodePtr pstOffer = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstOffer->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip msg is not valid or complete.strSipMsg:\n" + strSipMsg);
        return false;
    }

    stTemBody = pstOffer->_pstMsg->stBody.stBodyStr;
    return true;
}


// strSipMsgOffer ： 带SDP offer的sip消息
// strSipMsgLocalCapacity ：本端 支持音视频 能力 SDP 信息 sip 信息
bool SipMsg::SDPNegotiation(const std::string & strSipMsgOffer, const std::string & strSipMsgLocalCapacity, std::string & negotiatedSDP)
{
    SipMsgDecodePtr pstOffer, pstLocal;
    SipMsgEncodePtr pstAnswer ;
    pstAnswer = std::make_shared<SipMsgEncode>();
    pstOffer = std::make_shared<SipMsgDecode>();
    pstLocal = std::make_shared<SipMsgDecode>();
    unsigned long ulRseqNum = 0;

    //check decode procedure valid
    unsigned char ucResult = pstOffer->decode(strSipMsgOffer, strSipMsgOffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,OFFER Sip msg is not valid or complete.strSipMsgOffer:\n" + strSipMsgOffer);
        return false;
    }

    ucResult = pstLocal->decode(strSipMsgLocalCapacity, strSipMsgLocalCapacity.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, LOCAL Sip msg is not valid or complete.strSipMsgLocalCapacity:\n" + strSipMsgLocalCapacity);
        return false;
    }

    if (!pstAnswer->createReq())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called,createReq failed .");
        return false;
    }

    /* fill request line */
    pstAnswer->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);

    ST_SDP_SESS_DESC    stAnswerSessDes, stOfferSessDes, stLocalSessDes;
    ST_SDP_SESS_DESC_LST stOfferSessDesLst = pstOffer->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    ST_SDP_SESS_DESC_LST stLocalSessDesLst = pstLocal->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;

    Zos_ZeroMem(&stAnswerSessDes, sizeof(stAnswerSessDes));

    stOfferSessDes = *stOfferSessDesLst.pstHead->pData;
    stLocalSessDes = *stLocalSessDesLst.pstHead->pData;
    stAnswerSessDes.ucPres = 1;
    //fill v field
    stAnswerSessDes.stVer = stLocalSessDes.stVer;
    //fill o field
    stAnswerSessDes.stOrig = stLocalSessDes.stOrig;
    //fill s field
    stAnswerSessDes.stSessName = stLocalSessDes.stSessName;
    //fill i field
    stAnswerSessDes.stInfo = stLocalSessDes.stInfo;
    //fill c field
    stAnswerSessDes.stConn = stLocalSessDes.stConn;
    //fill t field
    stAnswerSessDes.stSessTime = stLocalSessDes.stSessTime;

    stAnswerSessDes = stLocalSessDes;
    //negotiate media desc
    ABNF_LIST_CREATE(&stAnswerSessDes.stAttrLst);
    ABNF_LIST_CREATE(&stAnswerSessDes.stMdescLst);
    ABNF_LIST_CREATE(&stAnswerSessDes.stEmailLst);
    ABNF_LIST_CREATE(&stAnswerSessDes.stPhoneLst);
    ABNF_LIST_CREATE(&stAnswerSessDes.stBwLst);

    NegotiateMedialDesc(stOfferSessDes, stLocalSessDes,stAnswerSessDes, pstAnswer);

    ////* add sdp to sip */
    if(Sip_MsgFillBodySdp(pstAnswer->_pstMsg, &stAnswerSessDes) == ZFAILED)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.");
        return false;
    }

    if (!pstAnswer->encode())
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called, encode(), return FALSE!");
        return false;
    }

    //printf("Sip include Answer :%s",  pstAnswer->_strMsg.c_str() );

    std::string answerMsg = pstAnswer->_strMsg;
    int pos =  answerMsg.find("v=");
    std::string  temSDP =  answerMsg.substr(pos,  answerMsg.size() - pos);
    negotiatedSDP = temSDP;

    return true;
}

bool SipMsg::NegotiateMedialDesc(const ST_SDP_SESS_DESC & offerSessDesc, const ST_SDP_SESS_DESC & localSessDesc, ST_SDP_SESS_DESC & answerSessDesc, SipMsgEncodePtr & pEncoder)
{
    if (offerSessDesc.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstOfferMNode;
        ST_SDP_MDESC *pstOfferMdesc;
        ZVOID * pstOfferMdescVoidPtr;
        int index = 0;
        //offer media desc 循环, audio video
        FOR_ALL_DATA_IN_ABNF_LIST(&offerSessDesc.stMdescLst, pstOfferMNode, pstOfferMdescVoidPtr)
        {
            pstOfferMdesc = (ST_SDP_MDESC *)pstOfferMdescVoidPtr;
            if(pstOfferMdesc == NULL)
            {
                break;
            }

            ST_SDP_MDESC stAnswerMdesc;
            Zos_ZeroMem(&stAnswerMdesc, sizeof(stAnswerMdesc));

            ABNF_LIST_CREATE(&stAnswerMdesc.stAttrLst);
            ABNF_LIST_CREATE(&stAnswerMdesc.stMedia.stFmtLst);

            ST_ABNF_LIST_NODE *pstLocalMNode;
            ST_SDP_MDESC *pstLocalMdesc;
            ZVOID * pstLocalMdescVoidPtr;
            int index = 0;
            bool bMediaFmtExist = false;         //media fmt contain
            vector<std::string>      vecMathedFmt; //matched media fmt
            vector<int>                 vecMatchedFmtInt;

            //local media desc  循环 audio video
            FOR_ALL_DATA_IN_ABNF_LIST(&localSessDesc.stMdescLst, pstLocalMNode, pstLocalMdescVoidPtr)
            {
                pstLocalMdesc = (ST_SDP_MDESC *)pstLocalMdescVoidPtr;
                if(pstLocalMdesc == NULL)
                {
                    break;
                }
                if (pstOfferMdesc->stMedia.ucPres)
                {
                    if (pstOfferMdesc->stMedia.ucMediaType != pstLocalMdesc->stMedia.ucMediaType)
                    {
                        continue;
                    }

                    bMediaFmtExist = true;
                    stAnswerMdesc.stMedia = pstLocalMdesc->stMedia;          //m 行赋值，fmt再筛选
                    ABNF_LIST_DELETE(&stAnswerMdesc.stMedia.stFmtLst);         //fmt clear
                    ABNF_LIST_CREATE(&stAnswerMdesc.stMedia.stFmtLst);

                    //fmt circulation
                    ST_ABNF_LIST_NODE *pstOfferFormatNode;
                    ST_ZOS_SSTR *pstOfferMFormat;
                    ZVOID * pstOfferMFormatVoidPtr;
                    //offer media formats
                    FOR_ALL_DATA_IN_ABNF_LIST(&pstOfferMdesc->stMedia.stFmtLst, pstOfferFormatNode, pstOfferMFormatVoidPtr)
                    {
                        pstOfferMFormat = (ST_ZOS_SSTR *)pstOfferMFormatVoidPtr;
                        if(pstOfferMFormat == NULL)
                        {
                            break;
                        }

                        ST_ABNF_LIST_NODE *pstLocalFormatNode;
                        ST_ZOS_SSTR *pstLocalMFormat;
                        ZVOID * pstLocalMFormatVoidPtr;
                        FOR_ALL_DATA_IN_ABNF_LIST(&pstLocalMdesc->stMedia.stFmtLst, pstLocalFormatNode, pstLocalMFormatVoidPtr)
                        {
                            ST_ZOS_SSTR * pTem;

                            pstLocalMFormat = (ST_ZOS_SSTR *)pstLocalMFormatVoidPtr;
                            if(pstLocalMFormat == NULL)
                            {
                                break;
                            }

                            std::string strLocalFmt, strOfferFmt;
                            SipMsgCommon::zosSstr2Str(*pstOfferMFormat,strOfferFmt);
                            SipMsgCommon::zosSstr2Str(*pstLocalMFormat,strLocalFmt);
                            if (strOfferFmt == strLocalFmt)
                            {
                                //    SIP_CREATE_MSGBUF(zMsgBuf);
                                Abnf_AnyLstAddParm(pEncoder->_pstMsg->zMemBuf, &stAnswerMdesc.stMedia.stFmtLst, sizeof(ST_ZOS_SSTR), (ZVOID **)&pTem);
                                Zos_UbufCpyXSStr(pEncoder->_pstMsg->zMemBuf, pstLocalMFormat,pTem);
                                vecMathedFmt.push_back(strOfferFmt);
                            }
                        }
                    }//FOR_ALL_DATA_IN_ABNF_LIST(&pstOfferMdesc->stMedia.stFmtLst, pstOfferFormatNode, pstOfferMFormatVoidPtr)
                }

            }//FOR_ALL_DATA_IN_ABNF_LIST(&localSessDesc.stMdescLst, pstLocalMNode, pstLocalMdescVoidPtr)

            if (vecMathedFmt.empty())
            {
                stAnswerMdesc.stMedia = pstOfferMdesc->stMedia;
                //cann't found any format matching the formats in this media desc in offer, set port=0, means reject this media stream
                stAnswerMdesc.stMedia.wPort = 0;
                stAnswerMdesc.stMedia.wPortNum = 0;

                ST_SDP_MDESC * pMDesc;
                Abnf_AnyLstAddParm(pEncoder->_pstMsg->zMemBuf, &answerSessDesc.stMdescLst, sizeof(ST_SDP_MDESC), (ZVOID **)&pMDesc);
                *pMDesc = stAnswerMdesc;
                continue;
            }

            //除了rtpmap 和 fmtp 对应匹配的媒体格式外，其余的属性从offer中直接复制？
            //attribute circulation
            ST_ABNF_LIST_NODE *pstOfferAttriNode;
            ST_SDP_AF *pstOfferAttribute;
            ZVOID * pstOfferAttributeVoidPtr;
            ZUINT iSize = 0;

            //offer media formats
            FOR_ALL_DATA_IN_ABNF_LIST(&pstOfferMdesc->stAttrLst, pstOfferAttriNode, pstOfferAttributeVoidPtr)
            {
                pstOfferAttribute = (ST_SDP_AF *)pstOfferAttributeVoidPtr;
                if(pstOfferAttribute == NULL)
                {
                    break;
                }
                bool bAddAttri = false;
                ST_SDP_AF * pTemAttri;
                int payLoad = 0;
                for (unsigned int i = 0; i < vecMathedFmt.size(); i++)
                {
                    try {
                        payLoad = tars::TC_Common::strto<int>(vecMathedFmt.at(i));
                    } catch (...) {
                        payLoad = 0;
                    }         //payload converter from fmt

                    if (pstOfferAttribute->ucType == EN_SDP_ATTR_FMTP)
                    {
                        if (payLoad == pstOfferAttribute->u.stFmtp.ucPayload)
                        {
                            bAddAttri = true;
                        }
                    }
                    else if (pstOfferAttribute->ucType == EN_SDP_ATTR_RTPMAP)
                    {
                        if (payLoad == pstOfferAttribute->u.stRtpmap.ucPayload)
                        {
                            bAddAttri = true;
                        }
                    }
                    else
                    {
                        bAddAttri = true;
                    }
                }

                if (bAddAttri)
                {
                    Abnf_AnyLstAddParm(pEncoder->_pstMsg->zMemBuf, &stAnswerMdesc.stAttrLst, sizeof(ST_SDP_AF), (ZVOID **)&pTemAttri);
                    *pTemAttri = * pstOfferAttribute;
                }
            }

            ST_SDP_MDESC * pMDesc;
            Abnf_AnyLstAddParm(pEncoder->_pstMsg->zMemBuf, &answerSessDesc.stMdescLst, sizeof(ST_SDP_MDESC), (ZVOID **)&pMDesc);
            *pMDesc = stAnswerMdesc;
        }
    }
    return true;
}

/* get codec encoding according to rfc3551 */
ZUCHAR SipMsg::sdpGetStaticEncoding(int ucPayload)
{
    ///* set encoding type */
    ZUCHAR ucEncoding = ZMAXUCHAR;
    switch (ucPayload)
    {
    case EN_RTP_PAYLOAD_PCMU:
        ucEncoding = EN_SDP_ENCODING_PCMU;
        break;
    case EN_RTP_PAYLOAD_GSM:
        ucEncoding = EN_SDP_ENCODING_GSM;
        break;
    case EN_RTP_PAYLOAD_G723:
        ucEncoding = EN_SDP_ENCODING_G723;
        break;
    case EN_RTP_PAYLOAD_DVI4_8K:
        ucEncoding = EN_SDP_ENCODING_DIV4;
        break;
    case EN_RTP_PAYLOAD_DVI4_16K:
        ucEncoding = EN_SDP_ENCODING_DIV4;
        break;
    case EN_RTP_PAYLOAD_LPC:
        ucEncoding = EN_SDP_ENCODING_LPC;
        break;
    case EN_RTP_PAYLOAD_PCMA:
        ucEncoding = EN_SDP_ENCODING_PCMA;
        break;
    case EN_RTP_PAYLOAD_G722:
        ucEncoding = EN_SDP_ENCODING_G722;
        break;
    case EN_RTP_PAYLOAD_L16_STEREO:
    case EN_RTP_PAYLOAD_L16_MONO:
        ucEncoding = EN_SDP_ENCODING_L16;
        break;
    case EN_RTP_PAYLOAD_QCELP:
        ucEncoding = EN_SDP_ENCODING_QCELP;
        break;
    case EN_RTP_PAYLOAD_CN:
        ucEncoding = EN_SDP_ENCODING_CN;
        break;
    case EN_RTP_PAYLOAD_MPA:
        ucEncoding = EN_SDP_ENCODING_MPA;
        break;
    case EN_RTP_PAYLOAD_G728:
        ucEncoding = EN_SDP_ENCODING_G728;
        break;
    case EN_RTP_PAYLOAD_DVI4_11K:
    case EN_RTP_PAYLOAD_DVI4_22K:
        ucEncoding = EN_SDP_ENCODING_DIV4;
        break;
    case EN_RTP_PAYLOAD_G729:
        ucEncoding = EN_SDP_ENCODING_G729;
        break;
    case EN_RTP_PAYLOAD_CELB:
        ucEncoding = EN_SDP_ENCODING_CELB;
        break;
    case EN_RTP_PAYLOAD_JPEG:
        ucEncoding = EN_SDP_ENCODING_JPEG;
        break;
    case EN_RTP_PAYLOAD_NV:
        ucEncoding = EN_SDP_ENCODING_NV;
        break;
    case EN_RTP_PAYLOAD_H261:
        ucEncoding = EN_SDP_ENCODING_H261;
        break;
    case EN_RTP_PAYLOAD_MPV:
        ucEncoding = EN_SDP_ENCODING_MPV;
        break;
    case EN_RTP_PAYLOAD_MP2T:
        ucEncoding = EN_SDP_ENCODING_MP2T;
        break;
    case EN_RTP_PAYLOAD_H263:
        ucEncoding = EN_SDP_ENCODING_H263;
        break;
    default:
        break;
    }

    return ucEncoding;
}

bool SipMsg::getPayloadName(int payload, std::string & plName)
{
    int size = ZOS_GET_TABLE_SIZE(RtpGateway::astSdpCodecTbl);

    for (int i = 0; i < size; i++)
    {
        if (payload == RtpGateway::astSdpCodecTbl[i].iPayload)
        {
            plName = RtpGateway::astSdpCodecTbl[i].pcName;
            return true;
        }
    }

    return false;
}

bool SipMsg::readSdpParams(const std::string & strSdp, StSdpSessDesc & stSdp)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC stSessDes;

    if (!CreateSipMsgFromSDP(strSdp, strSipMsg))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,CreateSipMsgFromSDP() return FALSE.  strOldSDP :\n." + strSdp);
        return false;
    }
    pstDecode = std::make_shared<SipMsgDecode>();
    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete 
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, decode() return FALSE. strOldSDP :\n." + strSdp);
        return false;
    }

    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1. strOldSDP :\n." + strSdp);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;

    //decode V field
    ST_SDP_VF stVf = stSessDes.stVer;
    stSdp._iVer = stVf.wVer;

    //decode O field
    ST_SDP_OF *pstOrig = &stSessDes.stOrig;
    //StSdpOrigin stSdpOrigin;
    SipMsgCommon::deSdpOrigin(*pstOrig, stSdp._stOrigin);

    //decode C field
    ST_SDP_CF *pstConn = &stSessDes.stConn;
    //StSdpConnInfo stSdpConnInfo;
    SipMsgCommon::deSdpConnInfo(*pstConn, stSdp._stConnInfo);

    ST_ABNF_LIST_NODE *pstrMNode;
    ZVOID * pstMdescVoidPtr;
    bool result = false;
    //offer media desc 循环, audio video
    FOR_ALL_DATA_IN_ABNF_LIST(&stSessDes.stMdescLst, pstrMNode, pstMdescVoidPtr)
    {
        ST_SDP_MDESC *pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
        if(pstMdesc == NULL)
        {
            break;
        }

        StSdpMediaDesc mediaDesc;
        //ST_SDP_MF stMedia
        mediaDesc._ucMediaType = pstMdesc->stMedia.ucMediaType;
        mediaDesc._ucProtoType = pstMdesc->stMedia.ucProtoType;
        mediaDesc._iPort = pstMdesc->stMedia.wPort;
        mediaDesc._iPortNum = pstMdesc->stMedia.wPortNum;
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,mediaDesc._iPort: " + std::to_string(mediaDesc._iPort) + ",strSdp:" + strSdp);

        if (pstMdesc->stMedia.ucMediaType == EN_SDP_MEDIA_AUDIO)
        {
            sdpAcReadSdp(pstMdesc, stSdp, mediaDesc);
            sdpRedReadSdp(pstMdesc, mediaDesc);
            sdpDtmfReadSdp(pstMdesc, mediaDesc);
            stSdp._stMediaVec.push_back(mediaDesc);
        }
        else
        {
            //todo
            //sdpVcReadSdp(pstMdesc, stSdp, mediaDesc);
        }

        //get a =
        ST_SDP_AF *pstAf;
        ZVOID *pAf;
        ST_ABNF_LIST_NODE *pstNode;
        /* process reset attribute */
        FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stAttrLst, pstNode, pAf)
        {
            pstAf = (ST_SDP_AF *)pAf;

            if (pstAf->ucType == EN_SDP_ATTR_PTIME)
                stSdp.ptime = (ZUSHORT)pstAf->u.iPTime;
            else if (pstAf->ucType == EN_SDP_ATTR_MAXPTIME)
                stSdp.maxPtime = (ZUSHORT)pstAf->u.iMaxPTime;
            else if (pstAf->ucType == EN_SDP_ATTR_REC_PREF)
                stSdp.ucRecPref = pstAf->u.ucRecPref;
            else if (pstAf->ucType == EN_SDP_ATTR_RTCP_MUX)
                stSdp.ucRtcpMux = true;
            else if (pstAf->ucType == EN_SDP_ATTR_FRAMERATE)
            {
                ST_ZOS_SSTR *pstFrameRate = &pstAf->u.stFramrate;
                ZUINT iFrameRate;
                ZOS_SSTR_TO_UI(pstFrameRate, &iFrameRate);
                stSdp.ucFrameRate = iFrameRate > 30 ? 30 : iFrameRate;
            }
        }

        Sdp_MsgGetAfDirect(pstMdesc, &stSdp.ucDirect);
    }

    return true;
}

ZUCHAR SipMsg::sdpGetAfRtpmap(ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, int *piClockRate)
{
    ST_ABNF_LIST_NODE *pstNode;
    ST_SDP_RTPMAP *pstRtpmap;
    ST_SDP_AF *pstAf;
    ZVOID *pAf;
    /* set encoding type and rtpmap information for each payload */
    FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stAttrLst, pstNode, pAf)
    {
        pstAf = (ST_SDP_AF *)pAf;
        /* check is the rtpmap */
        if (pstAf->ucType != EN_SDP_ATTR_RTPMAP)
            continue;

        /* get the rtpmap */
        pstRtpmap = &pstAf->u.stRtpmap;
        if (ucPayload == pstRtpmap->ucPayload)
        {
            *piClockRate = pstRtpmap->iClockRate;
            return pstRtpmap->stEncoding.ucType;
        }
    }

    return ZMAXUCHAR;
}


bool SipMsg::sdpGetEncoding(ZBOOL bIsAudio, ZUCHAR ucPayload, ZUCHAR *pucEncoding, int *piClockRate)
{
    /* check exist the clock rate if not dynamic payload */
    if (!*piClockRate)
    {
        /* set default clock rate */
        *piClockRate = bIsAudio ? 8000 : 90000;
    }

    /* check need set the encoding type */
    if (*pucEncoding < EN_SDP_ENCODING_UNKNOWN)
        return true;

    /* set encoding type */
    *pucEncoding = sdpGetStaticEncoding(ucPayload);
    if (*pucEncoding == ZMAXUCHAR)
    {
        return true;
    }

    switch (ucPayload)
    {
    case EN_RTP_PAYLOAD_DVI4_16K:
        *piClockRate = 16000;
        break;
    case EN_RTP_PAYLOAD_L16_STEREO:
        return false;
    case EN_RTP_PAYLOAD_L16_MONO:
        *piClockRate = 44100;
        break;
    case EN_RTP_PAYLOAD_MPA:
        *piClockRate = 90000;
        break;
    case EN_RTP_PAYLOAD_DVI4_11K:
        *piClockRate = 11025;
        break;
    case EN_RTP_PAYLOAD_DVI4_22K:
        *piClockRate = 22050;
        break;
    }

    return true;
}


/*get Ac fmtp of codec */
bool SipMsg::sdpAcGetAfFmtp(ST_SDP_MDESC *pstMdesc, StSdpCodec &pstCodec)
{
    ST_SDP_FMTP *pstFmtp = ZNULL;
    ST_ABNF_LIST_NODE *pstNode;
    ST_ZOS_SSTR *pstFmt;
    ST_SDP_AF *pstAf;
    ZUCHAR ucPayload;
    ZVOID *pAf;

    /* get attribute of each media stream */
    FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stAttrLst, pstNode, pAf)
    {
        pstAf = (ST_SDP_AF *)pAf;

        /* check is fmtp attribute */
        if (pstAf->ucType != EN_SDP_ATTR_FMTP)
            continue;

        /* get the format string */
        pstFmtp = &pstAf->u.stFmtp;        
        if (pstFmtp->ucType == EN_SDP_FMTP_UNKNOWN)
        {
            /* get the unknown payload */
            pstFmt = &pstFmtp->stUnknown.stFormat;
            Zos_StrToUc(pstFmt->pcStr, pstFmt->iLen, &ucPayload);
        }
        else
        {
            /* get the known payload */
            ucPayload = pstAf->u.stFmtp.ucPayload;
        }

        if (pstCodec.iPayload == ucPayload)
            break;
    }

    /* check reach the right audio codec */
    if (!pstFmtp || !pstAf) return ZOK;

    if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_3267)
    {
    }
    else if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_3952)
    {
        if (pstCodec.ucEncoding != EN_SDP_ENCODING_ILBC)
            return false;

        /* get iLBC options */
        pstCodec.u.iIlbcMode = pstAf->u.stFmtp.u.st3952.ucMode;
    }
    else if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_UNKNOWN)
    {
        /* check for g729 fmtp */
        if (pstCodec.ucEncoding == EN_SDP_ENCODING_G729)
        {
            ST_ZOS_SSTR *pstParm = &pstAf->u.stFmtp.stUnknown.stParms;
            ZCHAR acStr[256], *pcstr;

            if (pstParm->iLen == 0 || pstParm->iLen >= 256)
                return false;

            Zos_NStrXCpy(acStr, 256, pstParm);
            acStr[pstParm->iLen] = '\0';
            pcstr = acStr;

            if (Zos_StrNCmp(pcstr, "annexb", 6) != 0)
                return false;

            pcstr += 6;

            /* ignore the whitespace */
            while (*pcstr == ' ') pcstr++;

            /* check is the equal */
            if (*pcstr++ != '=') return ZFAILED;

            /* ignore the whitespace */
            while (*pcstr == ' ') pcstr++;

            if (Zos_StrCmp(pcstr, "yes") == 0)
                pstCodec.u.bG729Annexb = ZTRUE;
            else if (Zos_StrCmp(pcstr, "no") == 0)
                pstCodec.u.bG729Annexb = ZFALSE;
        }
    }

    return true;
}

/*get vc fmtp of codec */
bool SipMsg::sdpVcGetAfFmtp(ST_SDP_MDESC *pstMdesc, StSdpCodec &pstCodec)
{
    ST_SDP_FMTP *pstFmtp = ZNULL;
    ST_ABNF_LIST_NODE *pstNode;
    ST_ZOS_SSTR *pstFmt;
    ST_SDP_AF *pstAf;
    ZUCHAR ucPayload;
    ZVOID *pAf;

    /* get attribute of each media stream */
    FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stAttrLst, pstNode, pAf)
    {
        pstAf = (ST_SDP_AF *)pAf;

        /* check is fmtp attribute */
        if (pstAf->ucType != EN_SDP_ATTR_FMTP)
            continue;

        /* get the format string */
        pstFmtp = &pstAf->u.stFmtp;        
        if (pstFmtp->ucType == EN_SDP_FMTP_UNKNOWN)
        {
            /* get the unknown payload */
            pstFmt = &pstFmtp->stUnknown.stFormat;
            Zos_StrToUc(pstFmt->pcStr, pstFmt->iLen, &ucPayload);
        }
        else
        {
            /* get the known payload */
            ucPayload = pstAf->u.stFmtp.ucPayload;
        }

        if (pstCodec.iPayload == ucPayload)
            break;
    }

    /* check reach the right audio codec */
    if (!pstFmtp || !pstAf) return ZOK;

    //todo get vc params
    //if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_H26X)
    //{
    //    if (pstAf->u.stFmtp.u.stH26X.ucH261Pres)
    //    {
    //        if (pstCodec->ucEncoding != EN_SDP_ENCODING_H261)
    //            return ZFAILED;

    //        /* get h261 options */
    //        if (Mnf_VcGetFmtpH261(&pstAf->u.stFmtp.u.stH26X, pstCodec) != ZOK)
    //            return ZFAILED;
    //    }
    //    else
    //    {
    //        if (pstCodec->ucEncoding != EN_SDP_ENCODING_H263)
    //            return ZFAILED;

    //        /* get h263 options */
    //        if (Mnf_VcGetFmtpH263(&pstAf->u.stFmtp.u.stH26X, pstCodec) != ZOK)
    //            return ZFAILED;
    //    }
    //}
    //else if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_4629)
    //{
    //    if (pstCodec->ucEncoding == EN_SDP_ENCODING_H263_1998)
    //    {
    //        if (Mnf_VcGetFmtpH263x(&pstAf->u.stFmtp.u.st4629, pstCodec) != ZOK)
    //            return ZFAILED;
    //    }
    //    else if (pstCodec->ucEncoding != EN_SDP_ENCODING_H263_2000)
    //    {
    //        if (Mnf_VcGetFmtpH263xx(&pstAf->u.stFmtp.u.st4629, pstCodec) != ZOK)
    //            return ZFAILED;
    //    }
    //    else
    //    {
    //        return ZFAILED;
    //    }
    //}
    //else if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_3984)
    //{
    //    if (pstCodec->ucEncoding != EN_SDP_ENCODING_H264
    //        && pstCodec->ucEncoding != EN_SDP_ENCODING_H264_SVC)
    //        return ZFAILED;

    //    /* get h264 options */
    //    if (Mnf_VcGetFmtpH264(&pstAf->u.stFmtp.u.st3984, pstCodec) != ZOK)
    //        return ZFAILED;
    //}
    //else if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_3640)
    //{
    //    if (pstCodec->ucEncoding != EN_SDP_ENCODING_MP4V_ES)
    //        return ZFAILED;

    //    /* get mpeg4 options */
    //    if (Mnf_VcGetFmtpMpv4(&pstAf->u.stFmtp.u.st3640, pstCodec) != ZOK)
    //        return ZFAILED;
    //}
    //else if (pstAf->u.stFmtp.ucType == EN_SDP_FMTP_UNKNOWN)
    //{
    //    /* check for H263 */
    //    if (pstCodec->ucEncoding == EN_SDP_ENCODING_H263
    //        || pstCodec->ucEncoding == EN_SDP_ENCODING_H263_1998)
    //    {
    //        if (Mnf_VcChkFmtpH263(&pstAf->u.stFmtp.stUnknown.stParms,
    //            &pstCodec->u.stH263) != ZOK)
    //            return ZFAILED;
    //    }

    //    /* check for H264 */
    //    if (pstCodec->ucEncoding == EN_SDP_ENCODING_H264
    //        || pstCodec->ucEncoding == EN_SDP_ENCODING_H264_SVC)
    //        Mnf_VcChkFmtpH264(&pstAf->u.stFmtp.stUnknown.stParms, pstCodec);
    //}

    return true;
}
/* function of media process unit read sdp */
bool SipMsg::sdpAcReadSdp(ST_SDP_MDESC *pstMdesc, StSdpSessDesc & desc, StSdpMediaDesc & mediaDesc)
{
    ST_ABNF_LIST_NODE *pstNode;
    ST_SDP_FMT *pstFmt;
    ZUCHAR ucPayload;
    ZUINT i, iPtime;
    ZVOID *pfmt;

    FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stMedia.stFmtLst, pstNode, pfmt)
    {
        pstFmt = (ST_SDP_FMT *)pfmt;
        /* get payload value */
        if (Zos_StrToUc(pstFmt->pcStr, pstFmt->iLen, &ucPayload) != ZOK)
        {
            continue;
        }

        StSdpCodec codec;
        /* set codec payload */
        codec.iPayload = ucPayload;
        codec.iBitRate = 0;
        codec.ucEncoding = sdpGetAfRtpmap(pstMdesc, ucPayload, &codec.iBitRate);

        if (!sdpGetEncoding(ZTRUE, ucPayload, &codec.ucEncoding, &codec.iBitRate))
        {
            TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called,sdpGetEncoding ac payload: " + std::to_string(ucPayload));
            continue;
        }
        else if (codec.ucEncoding == EN_SDP_ENCODING_PARITYFEC || codec.ucEncoding == EN_SDP_ENCODING_ULPFEC
            || codec.ucEncoding == EN_SDP_ENCODING_RED || codec.ucEncoding == EN_SDP_ENCODING_TEL_EVNT)
        {
            TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called, ucEncoding continue ac payload: " + std::to_string(ucPayload));
            continue;
        }

        ZCHAR *pName = Sdp_EncodingToName(codec.ucEncoding);
        if (pName == ZNULL)
            continue;

        codec.pcName = pName;

        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, ac payload: " + std::to_string(ucPayload) + ", codec.pcName:" + codec.pcName + ", codec.iBitRate:" + std::to_string(codec.iBitRate));

        /* read parameter */
        sdpAcGetAfFmtp(pstMdesc, codec);

        mediaDesc._vecCodec.push_back(codec);
    }

    /* read ptime */
    if (Sdp_MsgGetAfPtime(pstMdesc, &iPtime) == ZOK)
        desc.ptime = (ZUSHORT)iPtime;
    if (Sdp_MsgGetAfMaxPTime(pstMdesc, &iPtime) == ZOK)
        desc.maxPtime = (ZUSHORT)iPtime;

    return true;
}

/* function of media process unit read sdp */
bool SipMsg::sdpVcReadSdp(ST_SDP_MDESC *pstMdesc, StSdpSessDesc & desc, StSdpMediaDesc & mediaDesc)
{
    ST_ABNF_LIST_NODE *pstNode;
    ST_SDP_FMT *pstFmt;
    ZUCHAR ucPayload;
    ZUINT i, iPtime;
    ZVOID *pfmt;

    FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stMedia.stFmtLst, pstNode, pfmt)
    {
        pstFmt = (ST_SDP_FMT *)pfmt;
        /* get payload value */
        if (Zos_StrToUc(pstFmt->pcStr, pstFmt->iLen, &ucPayload) != ZOK)
        {
            continue;
        }

        StSdpCodec codec;
        /* set codec payload */
        codec.iPayload = ucPayload;
        codec.iBitRate = 0;
        codec.ucEncoding = sdpGetAfRtpmap(pstMdesc, ucPayload, &codec.iBitRate);

        if (sdpGetEncoding(ZTRUE, ucPayload, &codec.ucEncoding, &codec.iBitRate) != ZOK)
        {
            continue;
        }
        else if (codec.ucEncoding == EN_SDP_ENCODING_PARITYFEC || codec.ucEncoding == EN_SDP_ENCODING_ULPFEC
            || codec.ucEncoding == EN_SDP_ENCODING_RED || codec.ucEncoding == EN_SDP_ENCODING_TEL_EVNT)
        {
            continue;
        }

        codec.pcName = Sdp_EncodingToName(codec.ucEncoding);
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, ac payload: " + std::to_string(ucPayload) + ", codec.pcName:" + codec.pcName + ", codec.iBitRate:" + std::to_string(codec.iBitRate));

        /* read parameter */
        // todo 
        //sdpVcGetAfFmtp(pstMdesc, codec);
        //sdpVcGetAfFramesize(pstMdesc, codec);
        //sdpVcGetAfImgAttr(pstMdesc, codec);

        mediaDesc._vecCodec.push_back(codec);
    }

    return true;
}

/* find rtpmap */
ZUCHAR SipMsg::sdpFindAfRtpmap(ST_SDP_MDESC *pstMdesc, ZUCHAR ucEncoding)
{
    ST_ABNF_LIST_NODE *pstNode;
    ST_SDP_RTPMAP *pstRtpmap;
    ST_SDP_AF *pstAf;
    ZVOID *pAf;

    /* set encoding type and rtpmap information for each payload */
    FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stAttrLst, pstNode, pAf)
    {
        pstAf = (ST_SDP_AF *)pAf;
        /* check is the rtpmap */
        if (pstAf->ucType != EN_SDP_ATTR_RTPMAP)
            continue;

        /* get the rtpmap */
        pstRtpmap = &pstAf->u.stRtpmap;
        if (pstRtpmap->stEncoding.ucType == ucEncoding)
            return pstRtpmap->ucPayload;
    }

    return ZMAXUCHAR;
}

/* function of media process unit read sdp */
bool SipMsg::sdpRedReadSdp(ST_SDP_MDESC *pstMdesc, StSdpMediaDesc & desc)
{
    ST_ABNF_LIST_NODE *pstNode;
    ST_SDP_FMTP *pstFmtp;
    ST_SDP_AF *pstAf;
    ZVOID *pAf;
    ZUCHAR ucPayload;

    desc._redCodec.iBitRate = 8000;
    desc._redCodec.redPres = false;
    desc._redCodec.ucEncoding = EN_SDP_ENCODING_RED;

    /* get fec payload */
    ucPayload = sdpFindAfRtpmap(pstMdesc, EN_SDP_ENCODING_ULPFEC);
    if (ucPayload == ZMAXUCHAR)
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,RedReadSdp no fec");
    else
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,RedReadSdp fec payload: " + std::to_string(ucPayload));

    desc._redCodec.stRed.ucFecPt = ucPayload;

    /* get red payload */
    ucPayload = sdpFindAfRtpmap(pstMdesc, EN_SDP_ENCODING_RED);
    if (ucPayload == ZMAXUCHAR)
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,RedReadSdp no red");
    else
    {
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,RedReadSdp red payload: " + std::to_string(ucPayload));
        desc._redCodec.redPres = true;
    }

    desc._redCodec.stRed.ucRedPt = ucPayload;
    desc._redCodec.pcName = Sdp_EncodingToName(EN_SDP_ENCODING_RED);

    /* get attribute of red */
    FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stAttrLst, pstNode, pAf)
    {
        pstAf = (ST_SDP_AF *)pAf;
        /* check is fmtp attribute */
        if (pstAf->ucType == EN_SDP_ATTR_FMTP
            && pstAf->u.stFmtp.ucType == EN_SDP_FMTP_2733
            && pstAf->u.stFmtp.u.st2733.ucRed)
        {
            pstFmtp = &pstAf->u.stFmtp;
            if (pstFmtp->u.st2733.u.stRed.ucPayloadCnt > SIP_SDP_RED_PT_MAX)
                desc._redCodec.stRed.ucPtCnt = SIP_SDP_RED_PT_MAX;
            else
                desc._redCodec.stRed.ucPtCnt = pstFmtp->u.st2733.u.stRed.ucPayloadCnt;
            Zos_MemCpy(desc._redCodec.stRed.aucPtLst, pstFmtp->u.st2733.u.stRed.aucPayload, desc._redCodec.stRed.ucPtCnt * sizeof(ZUCHAR));
        }
    }

    return ucPayload == ZMAXUCHAR ? false : true;
}

/* function of media process unit read sdp */
bool SipMsg::sdpDtmfReadSdp(ST_SDP_MDESC *pstMdesc, StSdpMediaDesc & desc)
{
    ZUCHAR ucPayload;
    desc._dtmfCodec.redPres = false;
    /* get dtmf payload */
    ucPayload = sdpFindAfRtpmap(pstMdesc, EN_SDP_ENCODING_TEL_EVNT);
    if (ucPayload == ZMAXUCHAR)
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,DtmfReadSdp no dtmf");
    else
    {
        desc._dtmfCodec.redPres = true;
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,DtmfReadSdp payload: " + std::to_string(ucPayload));
    }

    desc._dtmfCodec.ucEncoding = EN_SDP_ENCODING_TEL_EVNT;
    desc._dtmfCodec.iPayload = ucPayload;
    desc._dtmfCodec.pcName = Sdp_EncodingToName(EN_SDP_ENCODING_TEL_EVNT);

    return true;
}

/* function of media process unit fill sdp */
bool SipMsg::sdpRedFillSdp(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec)
{
    ST_SDP_AF *pstAf;

    /* add fec fmt */
    if (codec.stRed.ucFecPt != ZMAXUCHAR)
    {
        if (Sdp_MsgSetMfCodec(zMemBuf, &pstMdesc->stMedia, codec.stRed.ucFecPt) != ZOK)
        {
            TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called,Sdp_MsgSetMfCodec error, red payload: " + std::to_string(codec.stRed.ucFecPt));
            return false;
        }

        /* add rtpmap */
        if (Sdp_MsgSetAfRtpmap(zMemBuf, pstMdesc, codec.stRed.ucFecPt, 8000, EN_SDP_ENCODING_ULPFEC) != ZOK)
        {
            TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called,RedFillSdp, fec rtpmap: " + std::to_string(codec.stRed.ucFecPt));
            return false;
        }
    }

    /* add red fmt */
    if (codec.stRed.ucRedPt != ZMAXUCHAR && codec.stRed.ucPtCnt)
    {
        ST_SDP_FMT *pstFmt;
        ST_SDP_FMTP *pstFmtp;

        /* allocate sdp fromat memory */
        ABNF_LIST_ALLOC_DATA(zMemBuf, ST_SDP_FMT, pstFmt);
        if (!pstFmt)
        {
            TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called,RedFillSdp red fmt: " + std::to_string(codec.stRed.ucRedPt));
            return false;
        }

        /* copy data into string */
        Zos_UbufCpyFStr(zMemBuf, &pstFmt->pcStr, "%d", codec.stRed.ucRedPt);
        pstFmt->iLen = Zos_StrLen(pstFmt->pcStr);

        /* add node into the list */
        ABNF_LIST_INSERT_NODE(&pstMdesc->stMedia.stFmtLst, pstFmt);

        /* add rtpmap */
        if (Sdp_MsgSetAfRtpmap(zMemBuf, pstMdesc, codec.stRed.ucRedPt, 8000, EN_SDP_ENCODING_RED) != ZOK)
        {
            TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called,RedFillSdp red rtpmap: " + std::to_string(codec.stRed.ucRedPt));
            return false;
        }

        /* create media-descriptions attribute field */
        if (Sdp_MsgCreateAf(zMemBuf, pstMdesc, &pstAf) != ZOK)
        {
            TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called,RedFillSdp create attr for red.");
            return false;
        }

        /* set fmtp of media */
        pstAf->ucType = EN_SDP_ATTR_FMTP;
        pstFmtp = &pstAf->u.stFmtp;

        /* set fmtp type and payload */
        pstFmtp->ucType = EN_SDP_FMTP_2733;
        pstFmtp->ucPayload = codec.stRed.ucRedPt;

        pstFmtp->u.st2733.ucRed = ZTRUE;
        pstFmtp->u.st2733.u.stRed.ucPayloadCnt = codec.stRed.ucPtCnt;
        Zos_MemCpy(&pstFmtp->u.st2733.u.stRed.aucPayload, &codec.stRed.aucPtLst, sizeof(codec.stRed.aucPtLst));
    }

    return true;
}

/* function of media process unit fill sdp */
bool SipMsg::sdpDtmfFillSdp(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec)
{
    /* add fmt */
    if (Sdp_MsgSetMfCodec(zMemBuf, &pstMdesc->stMedia, codec.iPayload) != ZOK)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called,Sdp_MsgSetMfCodec error, red payload: " + std::to_string(codec.iPayload));
        return false;
    }

    /* add rtpmap */
    if (Sdp_MsgSetAfRtpmap(zMemBuf, pstMdesc, codec.iPayload, 8000, EN_SDP_ENCODING_TEL_EVNT) != ZOK)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called, fill dtmf rtpmap error: " + std::to_string(codec.iPayload));
        return false;
    }

    /* set parameter */
    sdpDtmfSetAfFmtp(zMemBuf, pstMdesc, codec);

    return true;
}

/* set fmtp from dtmf */
bool SipMsg::sdpDtmfSetAfFmtp(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec)
{
    ST_SDP_FMTP_2833 *pst2833;
    ST_SDP_2833_VAL *pstVal;
    ST_SDP_FMTP *pstFmtp;
    ST_SDP_AF *pstAf;

    /* create media-descriptions attribute field */
    if (Sdp_MsgCreateAf(zMemBuf, pstMdesc, &pstAf) != ZOK)
    {
        return false;
    }

    /* set fmtp of media */
    pstAf->ucType = EN_SDP_ATTR_FMTP;
    pstFmtp = &pstAf->u.stFmtp;

    /* set fmtp type and payload */
    pstFmtp->ucType = EN_SDP_FMTP_2833;
    pstFmtp->ucPayload = codec.iPayload;

    /* create dtmf value list */
    pst2833 = &pstFmtp->u.st2833;
    ABNF_LIST_CREATE(&pst2833->stValLst);

    /* set value list */
    ABNF_LIST_ALLOC_DATA(zMemBuf, ST_SDP_2833_VAL, pstVal);
    pstVal->wValA = 0;
    pstVal->wValB = 15;
    ABNF_LIST_APPEND_NODE(&pst2833->stValLst, pstVal);

    return true;
}


/* function of media process unit fill sdp */
bool SipMsg::sdpAcFillSdp(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, StSdpCodec & codec)
{
    ZUCHAR ucEncoding;
    ZUINT i, iCount;

    /* add fmt */
    if (Sdp_MsgSetMfCodec(zMemBuf, &pstMdesc->stMedia, codec.iPayload) != ZOK)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetMfCodec error: " + std::to_string(codec.iPayload));
        return false;
    }

    /* set rtpmap */
    if (Sdp_MsgSetAfRtpmap(zMemBuf, pstMdesc, codec.iPayload, codec.iBitRate, codec.ucEncoding) != ZOK)
    {
        TLOGERROR("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetAfRtpmap error: " + std::to_string(codec.iPayload));
        return false;
    }

    TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called success payload: " + std::to_string(codec.iPayload));

    /* set parameter */
    sdpAcSetAfFmtp(zMemBuf, pstMdesc, codec);

    return true;
}

/* set fmtp from codec */
bool SipMsg::sdpAcSetAfFmtp(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc,  StSdpCodec & codec)
{
    ST_SDP_FMTP *pstFmtp;
    ST_SDP_AF *pstAf;

    /* check if need add fmtp */
    if (codec.ucEncoding == EN_SDP_ENCODING_AMR || codec.ucEncoding == EN_SDP_ENCODING_AMR_WB)
        return true;

    if (codec.ucEncoding != EN_SDP_ENCODING_ILBC && codec.ucEncoding != EN_SDP_ENCODING_AMR
        && codec.ucEncoding != EN_SDP_ENCODING_AMR_WB && codec.ucEncoding != EN_SDP_ENCODING_G729)
        return true;

    /* create media-descriptions attribute field */
    if (Sdp_MsgCreateAf(zMemBuf, pstMdesc, &pstAf) != ZOK)
    {
        return false;
    }

    /* set fmtp of media */
    pstAf->ucType = EN_SDP_ATTR_FMTP;
    pstFmtp = &pstAf->u.stFmtp;

    if (codec.ucEncoding == EN_SDP_ENCODING_ILBC)
    {
        /* set fmtp type and payload */
        pstFmtp->ucType = EN_SDP_FMTP_3952;
        pstFmtp->ucPayload = codec.iPayload;

        /* set iLBC mode */
        pstFmtp->u.st3952.ucMode = (ZUCHAR)codec.u.iIlbcMode;
    }
    else if (codec.ucEncoding == EN_SDP_ENCODING_AMR
        || codec.ucEncoding == EN_SDP_ENCODING_AMR_WB)
    {
        ZCHAR acBuf[32] = {0}, acVal[8] = {0}, *pcStr = ZNULL;
        ST_SDP_FMTP_3267 *pst3267;
        ST_SDP_3267_PARM *pstParm;
        ZUINT i = 0, iMaxCnt;
        ZUSHORT wLen;

        iMaxCnt = (codec.ucEncoding == EN_SDP_ENCODING_AMR) ? 7 : 8;

        /* set fmtp type and payload */
        pstFmtp->ucType = EN_SDP_FMTP_3267;
        pstFmtp->ucPayload = codec.iPayload;

        /* set amr options */
        pst3267 = &pstFmtp->u.st3267;
        ABNF_LIST_CREATE(&pst3267->stParmLst);

        /* set mode-set when not default value */
        //todo
    }
    else if (codec.ucEncoding == EN_SDP_ENCODING_G729)
    {
        ZCHAR *pcStr;

        /* set fmtp type and payload */
        pstFmtp->ucType = EN_SDP_FMTP_UNKNOWN;

        /* set G729 annexb */
        Zos_UbufCpyFStr(zMemBuf, &pcStr, "%d", codec.iPayload);
        ZOS_SSTR_SETS(&pstFmtp->stUnknown.stFormat, pcStr);
        Zos_UbufCpyFStr(zMemBuf, &pcStr, "annexb=%s", codec.u.bG729Annexb ? "yes" : "no");
        ZOS_SSTR_SETS(&pstFmtp->stUnknown.stParms, pcStr);
    }

    return true;
}


void SipMsg::negoSdp(StSdpSessDesc & stSdpDesc, StSdpSessDesc & stNegoSdp, vector<StSdpCodec> & codec)
{
    stNegoSdp = stSdpDesc;
    vector<StSdpCodec> vectCodec;

    for (int i = 0; i < stSdpDesc._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdpDesc._stMediaVec.at(i);
        StSdpMediaDesc stNegoMediaDesc = stNegoSdp._stMediaVec.at(i);

        if (stMediaDesc._ucMediaType != EN_SDP_MEDIA_AUDIO)
            continue;

        stNegoSdp._stMediaVec.at(i)._vecCodec.clear();
        stNegoSdp._stMediaVec.at(i)._redCodec.stRed.ucPtCnt = 0;
        memset(stNegoSdp._stMediaVec.at(i)._redCodec.stRed.aucPtLst, 0, SIP_SDP_RED_PT_MAX);

        negoAcSdp(stMediaDesc, stNegoSdp._stMediaVec.at(i), codec);
        if(stNegoSdp._stMediaVec.at(i)._vecCodec.size() > 0)
        {
            StSdpCodec code = stNegoSdp._stMediaVec.at(i)._vecCodec.at(0);
            TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, ac code:" + code.pcName);

            unsigned char payload = code.iPayload;
            negoRedSdp(stMediaDesc,stNegoSdp._stMediaVec.at(i), payload);
        }
    }
}

void SipMsg::negoSdp(StSdpSessDesc & stSdpDesc, StSdpSessDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp, StSdpCodec & codec)
{
    stNegoSdp = stSdpDesc;

    for (int i = 0; i < stSdpDesc._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdpDesc._stMediaVec.at(i);
        StSdpMediaDesc &stNegoMediaDesc = stNegoSdp._stMediaVec.at(i);

        if (stMediaDesc._ucMediaType != EN_SDP_MEDIA_AUDIO)
            continue;

        stNegoMediaDesc._vecCodec.clear();
        stNegoMediaDesc._redCodec.stRed.ucPtCnt = 0;
        memset(stNegoMediaDesc._redCodec.stRed.aucPtLst, 0, SIP_SDP_RED_PT_MAX);

        negoAcSdp(stMediaDesc, stNegoMediaDesc, stCfgSdp);
        if(stNegoMediaDesc._vecCodec.size() > 0)
        {
            codec = stNegoMediaDesc._vecCodec.at(0);
            TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, ac codec:" + codec.pcName);

            unsigned char payload = codec.iPayload;
            negoRedSdp(stMediaDesc,stNegoMediaDesc, payload);
        }
        //negoDtmfSdp(stMediaDesc,negoSdpCodec);
    }
}

void SipMsg::negoMrfpSdp(StSdpSessDesc & stSdpDesc, StSdpSessDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp, StSdpCodec & codec)
{
    stNegoSdp = stSdpDesc;

    for (int i = 0; i < stSdpDesc._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdpDesc._stMediaVec.at(i);
        StSdpMediaDesc &stNegoMediaDesc = stNegoSdp._stMediaVec.at(i);

        if (stMediaDesc._ucMediaType != EN_SDP_MEDIA_AUDIO)
            continue;

        stNegoMediaDesc._vecCodec.clear();
        stNegoMediaDesc._redCodec.stRed.ucPtCnt = 0;
        memset(stNegoMediaDesc._redCodec.stRed.aucPtLst, 0, SIP_SDP_RED_PT_MAX);

        negoMrfpAcSdp(stMediaDesc, stNegoMediaDesc, stCfgSdp);
        if(stNegoMediaDesc._vecCodec.size() > 0)
        {
            codec = stNegoMediaDesc._vecCodec.at(0);
            TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, ac codec:" + codec.pcName);
        }
    }
}

void SipMsg::negoMrfpAcSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp)
{
    vector<StSdpCodec> vectCodec;
    for (int i = 0; i < stMediaDesc._vecCodec.size(); i++)
    {
        StSdpCodec codec = stMediaDesc._vecCodec.at(i);

        for (int j = 0; j < stCfgSdp._vecCodec.size(); j++)
        {
            StSdpCodec codec2 = stCfgSdp._vecCodec.at(j);

            if (codec.pcName == codec2.pcName)
            {
                vectCodec.push_back(codec);
            }
        }
    }

    for (int i = 0; i < vectCodec.size(); i++)
    {
        StSdpCodec code = vectCodec.at(i);
        if (code.iPayload == 0 || code.iPayload == 8)
        {
            StSdpCodec tmp = vectCodec.at(0);
            stNegoSdp._vecCodec.push_back(code);
            return;
        }
    }

    if (vectCodec.size() > 0)
    {
        StSdpCodec tmp = vectCodec.at(0);
        stNegoSdp._vecCodec.push_back(vectCodec.at(0));
    }
}

void SipMsg::negoAcSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp, vector<StSdpCodec> & stCfgSdp)
{
    vector<StSdpCodec> vectCodec;
    for (int i = 0; i < stMediaDesc._vecCodec.size(); i++)
    {
        StSdpCodec codec = stMediaDesc._vecCodec.at(i);

        for (int j = 0; j < stCfgSdp.size(); j++)
        {
            StSdpCodec codec2 = stCfgSdp.at(j);

            if (codec.pcName == codec2.pcName)
            {
                TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, negoAcSdp codec.pcName:" + codec.pcName + ",iPayload:" + std::to_string(codec.iPayload));
                vectCodec.push_back(codec);
            }
        }
    }

    if (vectCodec.size() > 0)
    {
        StSdpCodec tmp = vectCodec.at(0);
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, tmp.pcName:" + tmp.pcName + ",iPayload:" + std::to_string(tmp.iPayload));

        stNegoSdp._vecCodec.push_back(vectCodec.at(0));
    }
}

void SipMsg::negoAcSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp, StSdpMediaDesc & stCfgSdp)
{
    vector<StSdpCodec> vectCodec;
    for (int i = 0; i < stMediaDesc._vecCodec.size(); i++)
    {
        StSdpCodec codec = stMediaDesc._vecCodec.at(i);

        for (int j = 0; j < stCfgSdp._vecCodec.size(); j++)
        {
            StSdpCodec codec2 = stCfgSdp._vecCodec.at(j);

            if (codec.pcName == codec2.pcName)
            {
                TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, negoAcSdp codec.pcName:" + codec.pcName + ",iPayload:" + std::to_string(codec.iPayload));
                vectCodec.push_back(codec);
            }
        }
    }

    if (vectCodec.size() > 0)
    {
        StSdpCodec tmp = vectCodec.at(0);
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, tmp.pcName:" + tmp.pcName + ",iPayload:" + std::to_string(tmp.iPayload));

        stNegoSdp._vecCodec.push_back(vectCodec.at(0));
    }
}

void SipMsg::negoRedSdp(StSdpMediaDesc & stMediaDesc, StSdpMediaDesc & stNegoSdp,int payload)
{
    for (int i = 0; i < stMediaDesc._redCodec.stRed.ucPtCnt; i++)
    {
        StSdpRed red = stMediaDesc._redCodec.stRed;
        ZUCHAR ucPayload = red.aucPtLst[i];
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,sdpTmp.pcName:" + stMediaDesc._redCodec.pcName + ",iPayload:" + std::to_string(ucPayload)+ ", count:" + std::to_string(stMediaDesc._redCodec.stRed.ucPtCnt));

        if (ucPayload == payload && stNegoSdp._redCodec.stRed.ucRedPt != ZMAXUCHAR)
        {
            stNegoSdp._redCodec.stRed.aucPtLst[stNegoSdp._redCodec.stRed.ucPtCnt++] = ucPayload;
        }
    }
}

int SipMsg::getDtmfPayload(StSdpSessDesc & stSdpDesc, StSdpCodec & codec)
{
    for (int i = 0; i < stSdpDesc._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdpDesc._stMediaVec.at(i);
        if (stMediaDesc._ucMediaType != EN_SDP_MEDIA_AUDIO)
            continue;

        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, dtmf iPayload:" + std::to_string(stMediaDesc._dtmfCodec.iPayload));

        codec = stMediaDesc._dtmfCodec;
        if (stMediaDesc._dtmfCodec.redPres)
            return stMediaDesc._dtmfCodec.iPayload;
    }

    return 0;
}

void SipMsg::getAcPayload(StSdpSessDesc & stSdpDesc, vector<StSdpCodec> & codec)
{
    for (int i = 0; i < stSdpDesc._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdpDesc._stMediaVec.at(i);
        if (stMediaDesc._ucMediaType != EN_SDP_MEDIA_AUDIO)
            continue;

        for (int j = 0; j < stMediaDesc._vecCodec.size(); j++)
        {
            StSdpCodec code = stMediaDesc._vecCodec.at(j);
            if (code.iPayload == ZMAXUCHAR)
                continue;

            codec.push_back(code);
            return;
        }
    }
}

int SipMsg::getRedPayload(StSdpSessDesc & stSdpDesc)
{
    for (int i = 0; i < stSdpDesc._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdpDesc._stMediaVec.at(i);
        if (stMediaDesc._ucMediaType != EN_SDP_MEDIA_AUDIO)
            continue;

        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called, red iPayload:" + std::to_string(stMediaDesc._redCodec.iPayload));

        if (stMediaDesc._redCodec.redPres)
            return stMediaDesc._redCodec.stRed.ucRedPt;
    }

    return 0;
}

bool SipMsg::getDtmfPayload(vector<StSdpCodec> & sdpCodec, StSdpCodec & sdpCode)
{
    for (int i = 0; i < sdpCodec.size(); i++)
    {
        StSdpCodec codec = sdpCodec.at(i);
        if (codec.pcName == "telephone-event")
        {
            sdpCode = codec;
            TLOGINFO("RtpGateway", "SipMsg::getDtmfPayload() called, codec.iPayload:" + std::to_string(codec.iPayload) + "sdpCode.iPayload:" + std::to_string(sdpCode.iPayload));
            return true;
        }
    }

    return false;
}

bool SipMsg::updateMediaDesc(StSdpSessDesc & stSdpNewSess, StSdpMediaDesc & sdpCodec)
{
    StSdpMediaDesc descTmp = sdpCodec;
    for(int i = 0; i < stSdpNewSess._stMediaVec.size(); i++)
    {
        StSdpMediaDesc desc = stSdpNewSess._stMediaVec.at(i);
        if (desc._ucMediaType == EN_SDP_MEDIA_AUDIO)
        {
            int acCnt = stSdpNewSess._stMediaVec.at(i)._vecCodec.size();

            stSdpNewSess._stMediaVec.at(i)._vecCodec.swap(descTmp._vecCodec);
            stSdpNewSess._stMediaVec.at(i)._dtmfCodec = descTmp._dtmfCodec;
            stSdpNewSess._stMediaVec.at(i)._redCodec = descTmp._redCodec;

            std::string dtmf = stSdpNewSess._stMediaVec.at(i)._dtmfCodec.pcName;
            int acCnt2 = stSdpNewSess._stMediaVec.at(i)._vecCodec.size();
            int size = sdpCodec._vecCodec.size();
            TLOGINFO("RtpGateway", "SipMsg::updateMediaDesc() called dtmf:" + dtmf + ", ac size:" + std::to_string(acCnt) + ", sdpCodec size:" + std::to_string(size));

            return true;
        }
    }

    return false;
}

bool SipMsg::updateSdpCodec(StSdpMediaDesc & sdpCodec, StSdpCodec & sdpCode)
{
    if (sdpCodec._dtmfCodec.pcName == sdpCode.pcName)
    {
        TLOGINFO("RtpGateway", "SipMsg::updateSdpCodec() called dtmf pl name:" + sdpCode.pcName);

        sdpCodec._dtmfCodec.iPayload = sdpCode.iPayload;
        sdpCodec._dtmfCodec.iBitRate = sdpCode.iBitRate;
        return true;
    }

    return false;
}

bool SipMsg::createSdp(StSdpSessDesc & stSdp, std::string & ip, vector<int> & ports, std::string & strNewSDP)
{
    ST_ZOS_DLIST_NODE *pstNode;
    ST_SDP_SESS_DESC *pstSdp;
    SipMsgEncodePtr pstEncodeNewSDP = std::make_shared<SipMsgEncode>();

    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,acall createReq failed");
        return false;
    }

    /* fill request line */
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);

    /* create new sdp */
    Sdp_MsgCreate(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstSdp);

    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(ip.c_str()), &stIp);

    Sdp_MsgSetOf(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstSdp->stOrig, ZNULL, 0, ZMAXUINT, 0, &stIp);
    Sdp_MsgSetSf(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstSdp->stSessName, ZOS_NSTR("-"));
    Sdp_MsgSetTf(pstEncodeNewSDP->_pstMsg->zMemBuf, pstSdp, 0, 0);
    Sdp_MsgSetCf(&pstSdp->stConn, &stIp);

    for (int i = 0; i < stSdp._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdp._stMediaVec.at(i);
        if (stMediaDesc._ucMediaType == EN_SDP_MEDIA_AUDIO)
        {
            int port = ports.at(0);
            ST_SDP_MDESC *pstMediadesc;
            /* set the audio stream sdp */
            ZUCHAR ucType = 0;
            Sdp_MsgCreateMdesc(pstEncodeNewSDP->_pstMsg->zMemBuf, pstSdp, &pstMediadesc);
            Sdp_MsgSetMf(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstMediadesc->stMedia, EN_SDP_MEDIA_AUDIO, ZNULL, 0, EN_SDP_PROTO_RTP_AVP, ZNULL, 0, port, 0);

            for (int j = 0; j < stMediaDesc._vecCodec.size(); j++)
                sdpAcFillSdp(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stMediaDesc._vecCodec.at(j));

            sdpRedFillSdp(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stMediaDesc._redCodec);
            sdpDtmfFillSdp(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stMediaDesc._dtmfCodec);

            Sdp_MsgSetAfDirect(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stSdp.ucDirect);

            //ST_SDP_AF *pstDirect;
            //if (Sdp_MsgCreateAf(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, &pstDirect) != ZOK)
            //    return false;

            //pstDirect->ucType = EN_SDP_ATTR_SENDRECV;

            if (stSdp.maxPtime)
            {
                Sdp_MsgSetAfMaxPTime(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stSdp.maxPtime);
            }

            if (stSdp.ptime)
            {
                ST_SDP_AF *pstPtime;
                /* create media mode attribute field */
                if (Sdp_MsgCreateAf(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, &pstPtime) != ZOK)
                    return false;
                pstPtime->ucType = EN_SDP_ATTR_PTIME;
                pstPtime->u.iPTime = stSdp.ptime;
            }
        }
        else
        {
            //todo
        }
    }

    /* add sdp to sip */
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, pstSdp) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!");
        return false;
    }

    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE!");
        return false;
    }

    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos,  pstEncodeNewSDP->_strMsg.size() - pos);

    return true;
}

bool SipMsg::createSdp(StSdpSessDesc &stSdp, std::string & ip, std::string & strNewSDP)
{
    ST_ZOS_DLIST_NODE *pstNode;
    ST_SDP_SESS_DESC *pstSdp;
    SipMsgEncodePtr pstEncodeNewSDP = std::make_shared<SipMsgEncode>();

    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,acall createReq failed");
        return false;
    }

    /* fill request line */
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);

    /* create new sdp */
    Sdp_MsgCreate(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstSdp);

    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(ip.c_str()), &stIp);

    Sdp_MsgSetOf(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstSdp->stOrig, ZNULL, 0, ZMAXUINT, 0, &stIp);
    Sdp_MsgSetSf(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstSdp->stSessName, ZOS_NSTR("-"));
    Sdp_MsgSetTf(pstEncodeNewSDP->_pstMsg->zMemBuf, pstSdp, 0, 0);
    Sdp_MsgSetCf(&pstSdp->stConn, &stIp);

    for (int i = 0; i < stSdp._stMediaVec.size(); i++)
    {
        StSdpMediaDesc stMediaDesc = stSdp._stMediaVec.at(i);
        if (stMediaDesc._ucMediaType == EN_SDP_MEDIA_AUDIO)
        {
            ST_SDP_MDESC *pstMediadesc;
            /* set the audio stream sdp */
            ZUCHAR ucType = 0;
            Sdp_MsgCreateMdesc(pstEncodeNewSDP->_pstMsg->zMemBuf, pstSdp, &pstMediadesc);
            Sdp_MsgSetMf(pstEncodeNewSDP->_pstMsg->zMemBuf, &pstMediadesc->stMedia, EN_SDP_MEDIA_AUDIO, ZNULL, 0, EN_SDP_PROTO_RTP_AVP, ZNULL, 0, stMediaDesc._iPort, 0);

            for (int j = 0; j < stMediaDesc._vecCodec.size(); j++)
                sdpAcFillSdp(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stMediaDesc._vecCodec.at(j));

            sdpRedFillSdp(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stMediaDesc._redCodec);
            sdpDtmfFillSdp(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stMediaDesc._dtmfCodec);

            Sdp_MsgSetAfDirect(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stSdp.ucDirect);

            if (stSdp.maxPtime)
            {
                Sdp_MsgSetAfMaxPTime(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, stSdp.maxPtime);
            }

            if (stSdp.ptime)
            {
                ST_SDP_AF *pstPtime;
                /* create media mode attribute field */
                if (Sdp_MsgCreateAf(pstEncodeNewSDP->_pstMsg->zMemBuf, pstMediadesc, &pstPtime) != ZOK)
                    return false;
                pstPtime->ucType = EN_SDP_ATTR_PTIME;
                pstPtime->u.iPTime = stSdp.ptime;
            }
        }
        else
        {
            //todo
        }
    }

    /* add sdp to sip */
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, pstSdp) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!");
        return false;
    }

    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE!");
        return false;
    }

    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos,  pstEncodeNewSDP->_strMsg.size() - pos);

    return true;
}

bool SipMsg::CreateSDPWithNewPort(const std::string & strOldSDP,  const vector<int> vectNewPorts,const std::string & strNewIP,  std::string & strNewSDP)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC stSessDes;
    SipMsgEncodePtr pstEncodeNewSDP ;

    if (strNewIP.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,strNewIP is EMPTY! strOldSDP:\n" + strOldSDP);
        return false;
    }

    if (!CreateSipMsgFromSDP(strOldSDP, strSipMsg))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,CreateSipMsgFromSDP() return FALSE.  strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstDecode = std::make_shared<SipMsgDecode>();
    pstEncodeNewSDP = std::make_shared<SipMsgEncode>();

    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,acall createReq failed . strOldSDP :\n." + strOldSDP);
        return false;
    }

    /* fill request line */
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);

    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, decode() return FALSE. strOldSDP :\n." + strOldSDP);
        return false;

    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    if (stSessDes.stMdescLst.iCount != vectNewPorts.size())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,stSessDes.stMdescLst.dwCount != vectNewPorts.size(). iCount:\n" + std::to_string(stSessDes.stMdescLst.iCount) + ", size:" + std::to_string(vectNewPorts.size()) + ", strOldSDP:\n" + strOldSDP);
        return false;
    }
    //o 行ip 替换
    /* transform string to ST_ZOS_INET_IP */
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(strNewIP.c_str()), &stIp);
    ST_SDP_OF *pstOrig = &stSessDes.stOrig;
    StSdpOrigin stSdpOrigin;
    SipMsgCommon::deSdpOrigin(*pstOrig, stSdpOrigin);

    /* sdp message set origin-field */
    if(Sdp_MsgSetOfX(pstEncodeNewSDP->_pstMsg->zMemBuf, pstOrig, (char*)stSdpOrigin._strUsername.c_str(), (int)stSdpOrigin._lSessId, (int)stSdpOrigin._lSessVer,  &stIp) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetOfX() failed.strOldSDP:\n" + strOldSDP);
        return false;
    }

    //c 行 ip 替换
    if (Sdp_MsgSetCf(&stSessDes.stConn, &stIp) !=  ZOK)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetCf() failed,strOldSDP:\n" + strOldSDP);
        return false;
    }

    //替换端口
    if (stSessDes.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstrMNode;
        ST_SDP_MDESC *pstMdesc;
        ZVOID * pstMdescVoidPtr;
        int index = 0;
        //offer media desc 循环, audio video
        FOR_ALL_DATA_IN_ABNF_LIST(&stSessDes.stMdescLst, pstrMNode, pstMdescVoidPtr)
        {
            pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
            if(pstMdesc == NULL)
            {
                break;
            }
            int temPort = vectNewPorts.at(index);

            if (pstMdesc->stMedia.wPort > 0)
                pstMdesc->stMedia.wPort = temPort;      //update the port;

            if (pstMdesc->stConnLst.iCount > 0)
            {
                ST_ABNF_LIST_NODE *pstrCFNode = 0;
                ST_SDP_CF * pstCf = 0;
                ZVOID * pstCfVoidPtr = 0;
                FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stConnLst, pstrCFNode, pstCfVoidPtr)
                {
                    pstCf = ( ST_SDP_CF *)pstCfVoidPtr;
                    if (pstCf)
                    {
                        //c 行 ip 替换
                        Sdp_MsgSetCf(pstCf, &stIp);
                    }
                }
            }
            index++;
        }
    }

    ////* add sdp to sip */
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, &stSessDes) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.strOldSDP:\n" + strOldSDP);
        return false;
    }

    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE! strOldSDP:\n" + strOldSDP);
        return false;
    }

    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos,  pstEncodeNewSDP->_strMsg.size() - pos);
    return true;
}

bool SipMsg::CreateSDPWithNewPort2(const std::string & strOldSDP,  const vector<int> vectNewPorts,const std::string & strNewIP,  std::string & strNewSDP)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC stSessDes;
    SipMsgEncodePtr pstEncodeNewSDP ;

    if (strNewIP.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,strNewIP is EMPTY! strOldSDP:\n" + strOldSDP);
        return false;
    }

    if (!CreateSipMsgFromSDP(strOldSDP, strSipMsg))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,CreateSipMsgFromSDP() return FALSE.  strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstDecode = std::make_shared<SipMsgDecode>();
    pstEncodeNewSDP = std::make_shared<SipMsgEncode>();

    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,acall createReq failed . strOldSDP :\n." + strOldSDP);
        return false;
    }

    /* fill request line */
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);

    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, decode() return FALSE. strOldSDP :\n." + strOldSDP);
        return false;

    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    if (stSessDes.stMdescLst.iCount != vectNewPorts.size())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,stSessDes.stMdescLst.dwCount != vectNewPorts.size(). iCount:\n" + std::to_string(stSessDes.stMdescLst.iCount) + ", size:" + std::to_string(vectNewPorts.size()) + ", strOldSDP:\n" + strOldSDP);
        return false;
    }
    //o 行ip 替换
    /* transform string to ST_ZOS_INET_IP */
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(strNewIP.c_str()), &stIp);
    ST_SDP_OF *pstOrig = &stSessDes.stOrig;
    StSdpOrigin stSdpOrigin;
    SipMsgCommon::deSdpOrigin(*pstOrig, stSdpOrigin);

    /* sdp message set origin-field */
    if(Sdp_MsgSetOfX(pstEncodeNewSDP->_pstMsg->zMemBuf, pstOrig, (char*)stSdpOrigin._strUsername.c_str(), (int)stSdpOrigin._lSessId, (int)stSdpOrigin._lSessVer + 1,  &stIp) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetOfX() failed.strOldSDP:\n" + strOldSDP);
        return false;
    }

    //c 行 ip 替换
    if (Sdp_MsgSetCf(&stSessDes.stConn, &stIp) !=  ZOK)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetCf() failed,strOldSDP:\n" + strOldSDP);
        return false;
    }

    //替换端口
    if (stSessDes.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstrMNode;
        ST_SDP_MDESC *pstMdesc;
        ZVOID * pstMdescVoidPtr;
        int index = 0;
        //offer media desc 循环, audio video
        FOR_ALL_DATA_IN_ABNF_LIST(&stSessDes.stMdescLst, pstrMNode, pstMdescVoidPtr)
        {
            pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
            if(pstMdesc == NULL)
            {
                break;
            }
            int temPort = vectNewPorts.at(index);

            if (pstMdesc->stMedia.wPort > 0)
                pstMdesc->stMedia.wPort = temPort;      //update the port;

            if (pstMdesc->stConnLst.iCount > 0)
            {
                ST_ABNF_LIST_NODE *pstrCFNode = 0;
                ST_SDP_CF * pstCf = 0;
                ZVOID * pstCfVoidPtr = 0;
                FOR_ALL_DATA_IN_ABNF_LIST(&pstMdesc->stConnLst, pstrCFNode, pstCfVoidPtr)
                {
                    pstCf = ( ST_SDP_CF *)pstCfVoidPtr;
                    if (pstCf)
                    {
                        //c 行 ip 替换
                        Sdp_MsgSetCf(pstCf, &stIp);
                    }
                }
            }
            index++;
        }
    }

    ////* add sdp to sip */
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, &stSessDes) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.strOldSDP:\n" + strOldSDP);
        return false;
    }

    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE! strOldSDP:\n" + strOldSDP);
        return false;
    }

    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos,  pstEncodeNewSDP->_strMsg.size() - pos);
    return true;
}

bool SipMsg::CreateSDPWithNewPort(const std::string & strOldSDP, int newAudioPort, int newVideoPort, const std::string & strNewIP,  std::string & strNewSDP)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC    stSessDes;
    SipMsgEncodePtr pstEncodeNewSDP ;

    if (strNewIP.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,strNewIP is EMPTY! " );
        return false;
    }

    if (!CreateSipMsgFromSDP(strOldSDP, strSipMsg))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,CreateSipMsgFromSDP() return FALSE.  strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstDecode = std::make_shared<SipMsgDecode>();
    pstEncodeNewSDP = std::make_shared<SipMsgEncode>();

    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("SipMsg",std::string(__FUNCTION__)+"() called,acall createReq failed . strOldSDP :\n." + strOldSDP);
        return false;
    }

    /* fill request line */
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);

    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  decode() return FALSE. strOldSDP :\n." + strOldSDP);
        return false;

    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述

    //o 行ip 替换
    /* transform string to ST_ZOS_INET_IP */
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(strNewIP.c_str()), &stIp);
    ST_SDP_OF *pstOrig = &stSessDes.stOrig;

    /* sdp message set origin-field */
    if(Sdp_MsgSetOfX(pstEncodeNewSDP->_pstMsg->zMemBuf, pstOrig, (char*)"SDP_Sesssion", 1111, 2222,  &stIp) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetOfX() failed.");
        return false;
    }

    //c 行 ip 替换
    if (Sdp_MsgSetCf(&stSessDes.stConn, &stIp) !=  ZOK)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sdp_MsgSetCf() failed.");
        return false;
    }

    //替换端口
    if (stSessDes.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstrMNode;
        ST_SDP_MDESC *pstMdesc;
        ZVOID * pstMdescVoidPtr;

        //offer media desc 循环, audio video
        FOR_ALL_DATA_IN_ABNF_LIST(&stSessDes.stMdescLst, pstrMNode, pstMdescVoidPtr)
        {
            pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
            if(pstMdesc == NULL)
            {
                break;
            }
            if (EN_SDP_MEDIA_AUDIO == pstMdesc->stMedia.ucMediaType)
            {
                pstMdesc->stMedia.wPort = newAudioPort;         //update the port;
            }
            else if (EN_SDP_MEDIA_VIDEO == pstMdesc->stMedia.ucMediaType)
            {
                pstMdesc->stMedia.wPort = newVideoPort;         //update the port;
            }
        }
    }

    ////* add sdp to sip */
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, &stSessDes) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.");
        return false;
    }

    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE!");
        return false;
    }

    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos,  pstEncodeNewSDP->_strMsg.size() - pos);
    return true;
}


bool SipMsg::GetSDPMediaPortNum(const std::string & strSDP, int & portNum)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC    stSessDes;

    pstDecode = std::make_shared<SipMsgDecode>();

    if (!CreateSipMsgFromSDP(strSDP, strSipMsg))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, CreateSipMsgFromSDP()  return FALSE! strSDP:\n" + strSDP);
        return false;
    }
    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, decode()  return FALSE! strSDP:\n" + strSDP);
        return false;
    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {

        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1!  stSessDesLst.dwCount :" + std::to_string(stSessDesLst.iCount) + ",msg:\n" + pstDecode->_strMsg);

        if (pstDecode->_pstMsg->stBody.stBodyStr.iLen == 0)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, _pstMsg->stBody.stBodyStr.dwLen == 0" );
        }
        TLOGWARN("SipMsg",std::string(__FUNCTION__)+"() called, Mtype :" + std::to_string(pstDecode->_pstMsg->stBody.ucMtype) + " MSubType :"
            + std::to_string(pstDecode->_pstMsg->stBody.ucMSubtype) + " stSpart ucType :" +  std::to_string(pstDecode->_pstMsg->stBody.u.stSpart.ucType));

        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    portNum = stSessDes.stMdescLst.iCount;                      //会话描述的媒体描述数目，即m行个数...

    return true;
}

bool SipMsg::GetSipSDPMediaNumber(const std::string & strSipWithSDP, int & num)
{
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC    stSessDes;

    pstDecode = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstDecode->decode(strSipWithSDP, strSipWithSDP.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  decode()  return FALSE! strSipWithSDP:\n" + strSipWithSDP);
        return false;
    }

    if (!pstDecode->_pstMsg->ucPres)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  _pstMsg->ucPres FALSE!strSipWithSDP:\n" + strSipWithSDP);
    }

    if (!pstDecode->_pstMsg->stBody.ucPres)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  NO Body Exist!! strSipWithSDP:\n" + strSipWithSDP);
        //return false;
    }

    if (pstDecode->_pstMsg->stHdrLst.iCount < 1)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  stHdrLst.dwCount < 1! strSipWithSDP:\n" + strSipWithSDP);
        //return false;
    }

    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {

        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1!  stSessDesLst.dwCount :" + std::to_string((int)stSessDesLst.iCount));
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstDecode->_strMsg ::" + pstDecode->_strMsg );

        //TEST------------------s
        if (pstDecode->_pstMsg->stBody.stBodyStr.iLen == 0)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, _pstMsg->stBody.stBodyStr.dwLen == 0" );
        }
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Mtype :" + std::to_string(pstDecode->_pstMsg->stBody.ucMtype) + " MSubType :"
            + std::to_string(pstDecode->_pstMsg->stBody.ucMSubtype) + " stSpart ucType :" +  std::to_string (pstDecode->_pstMsg->stBody.u.stSpart.ucType));

        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                   //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    num = stSessDes.stMdescLst.iCount;                           //会话描述的媒体描述数目，即m行个数...

    return true;
}


bool SipMsg::ReplaceSDPWithNewPorts(const std::string & strSipWithOldPorts,const vector<int> & vectPorts, std::string & strSipWithNewPorts)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC    stSessDes;
    SipMsgEncodePtr pstEncodeNewSDP ;

    pstDecode = std::make_shared<SipMsgDecode>();
    pstEncodeNewSDP = std::make_shared<SipMsgEncode>();

    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, acall createReq failed .");
        return false;
    }
    /* fill request line */
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);

    unsigned char ucResult = pstDecode->decode(strSipWithOldPorts, strSipWithOldPorts.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstDecode->decode() return FALSE.");
        return false;

    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  stSessDesLst.dwCount < 1.");
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    if (stSessDes.stMdescLst.iCount != vectPorts.size())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, stSessDes.stMdescLst.dwCount != vectNewPorts.size().");
        return false;
    }

    if (stSessDes.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstrMNode;
        ST_SDP_MDESC *pstMdesc;
        ZVOID * pstMdescVoidPtr;
        int index = 0;
        //offer media desc 循环, audio video
        FOR_ALL_DATA_IN_ABNF_LIST(&stSessDes.stMdescLst, pstrMNode, pstMdescVoidPtr)
        {
            pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
            if(pstMdesc == NULL)
            {
                break;
            }
            int temPort = vectPorts.at(index);
            pstMdesc->stMedia.wPort = temPort;         //update the port;
            index++;
        }
    }

    ////* add sdp to sip */
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, &stSessDes) == ZFAILED)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.");
        return false;
    }

    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE!");
        return false;
    }

    strSipWithNewPorts =  pstEncodeNewSDP->_strMsg;
    return true;
}


bool SipMsg::GetSipMsgSDPBody(const std::string & sipMsg, std::string & strSDP)
{
    SipMsgDecodePtr pstDecode;
    std::string  strMsgSDP;

    pstDecode = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstDecode->decode(sipMsg, sipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  pstDecode->decode() return FALSE.sipMsg:\n" + sipMsg);
        return false;
    }

    if(!pstDecode->getSipMsgSDPBody( strMsgSDP))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  pstDecode->getSipMsgSDPBody return FALSE.sipMsg:\n" + sipMsg);
        return false;
    }
    if(strMsgSDP.empty())
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  SDP is EMPTY. sipMsg:\n" + sipMsg);
        return false;
    }
    strSDP = strMsgSDP;
    return  true;
}

bool SipMsg::getBriefSipProxyId(const std::string & strLongSipProxyId, std::string & strBriefSipProxyid)
{
    int pos1 = strLongSipProxyId.find("@SipProxy");
    int pos2 = strLongSipProxyId.find(".Main");
    if (pos1 < 0)
    {
        strBriefSipProxyid = strLongSipProxyId;
    }
    else
    {
        if (pos2 < 0)
        {
            strBriefSipProxyid = strLongSipProxyId;
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, SipProxyId format wrong, strLongSipProxyId :" + strLongSipProxyId);
            return false;
        }
        strBriefSipProxyid = strLongSipProxyId.substr(pos1 + 1, pos2 - pos1 - 1);
    }

    return true;
}

bool SipMsg::CheckSDPMediaIsVedio(const std::string & sdp, bool & bVedio)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC    stSessDes;
    bVedio = false;

    if (!CreateSipMsgFromSDP(sdp, strSipMsg))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, CreateSipMsgFromSDP() return FALSE.  sdp :\n." + sdp);
        return false;
    }
    pstDecode = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,   decode() return FALSE. sdp :\n." + sdp);
        return false;
    }

    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  stSessDesLst.dwCount < 1. sdp :\n." + sdp);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，正常来说也只有一个会话描述，因此取第一个会话描述

    if (stSessDes.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstrMNode;
        ST_SDP_MDESC *pstMdesc;
        ZVOID * pstMdescVoidPtr;

        //offer media desc 循环, audio video
        FOR_ALL_DATA_IN_ABNF_LIST(&stSessDes.stMdescLst, pstrMNode, pstMdescVoidPtr)
        {
            pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
            if(pstMdesc == NULL)
            {
                break;
            }
            if (pstMdesc->stMedia.ucMediaType == EN_SDP_MEDIA_VIDEO && pstMdesc->stMedia.wPort != 0)
            {
                bVedio = true;
            }
        }
    }

    return true;
}

bool SipMsg::GetIpFromSDPConnField(const std::string & strCsSDP, std::string & ip)
{
    vector<SDPMediaInfo>  vectSdpMediaInfo;
    std::string strSipMsgWithSDP;
    std::string reason;

    try
    {
        if (!SipMsg::CreateSipMsgFromSDP(strCsSDP, strSipMsgWithSDP))
        {
            reason = "SipMsg::CreateSipMsgFromSDP() return FALSE";
            throw false;
        }

        if (!SipMsg::getMediaIpAndPortsOfSDPinSipMsg(strSipMsgWithSDP, vectSdpMediaInfo))
        {
            reason = "SipMsg::getMediaIpAndPortsOfSDPinSipMsg() return FALSE";
            throw false;
        }

        if (vectSdpMediaInfo.size() < 1)
        {
            reason = "SipMsg::getMediaIpAndPortsOfSDPinSipMsg() return a EMPTY vectSdpMediaInfo";
            throw false;
        }
        SDPMediaInfo infos = vectSdpMediaInfo.at(0);
        ip = infos._strIp;
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  return FALSE!  reason:" + reason + ",sdp:\n" + strCsSDP);
        }
        else
        {
            TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called,  return TRUE!  ,got ip:" + ip + ",sdp:\n" + strCsSDP);
        }
        return ex;
    }
}

//.................... decode msg from SipProxy ............................. //

bool SipMsg::onReiceivedSipMsgFromSipProxy(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                                           const std::string & sipProxyId, const std::string& toSBCConnectionId, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called,  pSipGatwway == NULL!SipMsg:\n" + strBuffer);
        return false;
    }

    SipMsgDecodePtr pstSipMsgDe = 0;
    unsigned char ucMethod;
    bool bReqPres = true;
    pstSipMsgDe = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + strBuffer);
        return false;
    }

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }
    bReqPres = pstSipMsgDe->isRequest() ? true : false;
    switch(ucMethod)
    {
    case EN_SIP_METHOD_INVITE:
        if (bReqPres)
        {
            return dealInviteRequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            return dealInviteResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_ACK:
        return dealAckRequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        break;

    case EN_SIP_METHOD_BYE:
        if (bReqPres)
        {
            return dealBYERequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            return dealBYEResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_CANCEL:
        if (bReqPres)
        {
            return dealCANCELRequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            return dealCANCELResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_UPDATE:
        if (bReqPres)
        {
            return dealUPDATERequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            return dealUPDATEResponse(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_PRACK:
        if (bReqPres)
        {
            TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called, PRACK request NOT supported.SipMsg:\n" + strBuffer);
            return false;
        }
        else
        {
            return dealPRACKResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_INFO:
        if (bReqPres == false)
        {
            TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called,  INFO Response NOT supported.SipMsg:\n" + strBuffer);
            return false;
        }
        else
        {
            return dealINFORequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        break;

    default:
        {
            TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called, this method is NOT supported.SipMsg:\n" + strBuffer);
            return false;
        }
    }
    return true;
}

bool SipMsg::dealInviteResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                                const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pSipGatwway == NULL! ERROR! SipMsg:\n" + strBuffer);
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strDispName, strUserName, strHost, strSDPBody;
    SipMsgDecodedPart * pstDecodedPart = 0;
    SipProxy::SipRegisterSubscribePtr pRegisterSub = 0;
    ST_SIP_DIGEST stDigest;
    unsigned int ulRseqNum = 0;
    bool existSDP = false, refresherPre = false,  uasRefresh = false;
    int iPort = 0, seconds = 0;
    unsigned char ucMethod = EN_SIP_METHOD_INVITE, ucTptType = 0, ucAddrType = 0;
    std::string strHostName;
    ZCHAR zcTpt;
    unsigned char value1 = 0;
    unsigned int value2 = 0;
    std::string value3;
    std::string callIdHost;

    ucTptType = EN_SIP_TPT_UDP;
    //check decode procedure valid
    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    callIdHost = strCallId + "@" + strCallHost;

    std::string strRspReason, strSpeak;
    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    pstSipMsgDe->getRspReason(strRspReason);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        std::string strSipDialogIdTo = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogIdTo);
        if (pstSipDialog == 0)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  444 call getSipDialogPtr() failed to get the SipDialogPtr when received a Responce, with SipDialogId : " + strSipDialogId + " SipMsg:\n" + strBuffer);
            pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
            return false;
        }
    }

    //resp header
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    pstDecodedPart->_bReqPres = false;
    pstDecodedPart->_statusLinePart._iCode = ulStatusCode;
    pstDecodedPart->_statusLinePart._strReason = strRspReason;

    //digest challenge
    if (407 == ulStatusCode || 401 == ulStatusCode)
    {
        pRegisterSub = pstSipDialog->_sipRegisterSub;
        if (pRegisterSub != 0)
        {
            if (pstSipMsgDe->getDigest(stDigest, ulStatusCode))
            {
                pRegisterSub->setDigest(stDigest);
                pstSipMsgDe->decodeChallengeDigest(stDigest, pRegisterSub);
            }
        }
    }
    else if (180 == ulStatusCode || 200 == ulStatusCode || 183 == ulStatusCode)
    {
        //get To Header
        strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
        if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
        {
            pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName, strHost, iPort, strRemoteTag, ucTptType);
        }

        //get Contact Header
        if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt))
        {
            pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);

            std::string temAddress;
            SipMsgCommon::generateToSBCConnectionId(strHostName, iPort, temAddress);
            bool isPrivate = privateIpCheck(strHostName);
            if (temAddress != toSBCConnectionId && iPort >0 && !isPrivate)
            {
                pstSipDialog->setToSBCConnectionId(temAddress);
            }
        }

        ST_SIP_SIP_URI stSipUri;
        if (pstSipMsgDe->getHdrContactSipUri(stSipUri))
        {
            pstDecodedPart->_peerSipUri.setPeerSipUri(stSipUri);
        }

        //get Record-Route Header
        bool bSipCallRpc = pstSipDialog->getSipCallRpcProp();
        vector<ST_SIP_SIP_URI> stSipUris;
        pstSipMsgDe->getHdrRecordRouteSipUris(stSipUris);
        int sipCall = bSipCallRpc ? 1 : 0;
        int size = stSipUris.size();
        if (size > 0)
        {
            pstSipDialog->_sipMsgDecodePart._recordRoutes._sipUris = stSipUris;

            ST_SIP_SIP_URI sipUri = stSipUris.at(size - 1);
            std::string strUserInfo, strPassword;
            iPort = 0;
            SipMsgCommon::deSipUri(&sipUri, strUserInfo, strPassword, strHostName, iPort);

            if (iPort == 0)
                iPort = 5060;
            std::string temAddress;
            SipMsgCommon::generateToSBCConnectionId(strHostName, iPort, temAddress);
            pstSipDialog->setToSBCConnectionId(temAddress);
        }
    }

    pstSipDialog->pushbackDecodeObj(pstSipMsgDe);

    //decode RSeq Header
    if (pstSipMsgDe->getRseq(ulRseqNum))
    {
        pstDecodedPart->_rseqPart._respNum = ulRseqNum;
    }

    if (183 == ulStatusCode)
    {
        bool existUpdate = pstSipMsgDe->getAllowOfMethod(EN_SIP_METHOD_UPDATE);
        pstSipDialog->setSBCSupportUpdate(existUpdate);
    }

    if (ulStatusCode >= 400)
    {
        std::string text;
        unsigned int protoType = 0, cause = 0;
        pstSipMsgDe->getRspReason(text, protoType, cause);
        pstSipDialog->setErrorCauseCode(cause);

        std::string code;
        if (pstSipMsgDe->getHdrExt("ocs-term-id", code))
        {
            pstSipDialog->setOCSErrorCode(code);
            pstSipDialog->setOtherErrorCode(tars::TC_Common::strto<int>(code));
        }
    }

    //get SDP body
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod,  false,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            //pstSipDialog->setSdpAnswer(strSDPBody);
            //pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strSDPBody);
        }
        else
        {
            TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called, checkSipMsgWithSDP() return EMPTY body! SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        }
    }

    //get Session-Expires Header
    if (pstSipMsgDe->getHdrSessionExpires(seconds, refresherPre, uasRefresh))
    {
        int64_t curTime = TNOWMS;
        pstSipDialog->_sipMsgDecodePart._sessionExpiresPart.setSessionExpiresPart(seconds,curTime,refresherPre, uasRefresh);

        if (ulStatusCode == 200)
        {
            if (refresherPre && !uasRefresh)
            {
                pstSipDialog->setIsFreshDialog(true);
            }
        }
    }


    pSipGatwway->onSipMessageResponse(ulStatusCode, SipGateway::kSipRequestMethodINVITE, pstSipDialog,  sipProxyId, "", strSDPBody);
    return true;
}

bool SipMsg::dealInviteRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                               const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,pSipGatwway == NULL! SipMsg:\n" + strBuffer);
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    SipGateway::SipDialogIdPtr sptSipDialogId = 0;
    unsigned long  ulStatusCode = 0;
    unsigned char ucMethod = 0, ucTptType = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true;
    SipMsgDecodedPart * pstDecodedPart = 0;
    SipProxy::SipRegisterSubscribePtr pRegisterSub = 0;
    unsigned int ulRseqNum = 0;
    bool bSipToRpc = true;
    std::string strDispName, strUserName, strHost;
    int  iPort = 0, iReqUriType = 0;
    unsigned char ucAddrType = 0;
    int iCseq = 0;

    ucTptType = EN_SIP_TPT_UDP;
    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //create SipDialogId object
    sptSipDialogId = SipGateway::SipDialogId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,SipDialogId::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipGateway::SipDialog::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, SipDialog::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    pstSipDialog->setSipCallRpcProp(true);

    if (toSBCConnectionId.size() > 0)
    {
        pstSipDialog->setToSBCConnectionId(toSBCConnectionId);
    }

    //Set ReqLine DecodePart
    std::string strUserInfo, strHostname, strSpeak;
    pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_bReqPres = true;

    //get From Header
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    ST_SIP_SIP_URI *pstSipUri = 0;
    SipMsgCommon::getFromToSipUri(pstSipMsgDe->_pstMsg,true, &pstSipUri);
    if (pstSipUri != 0)
    {
        vector<ST_SIP_URI_PARM> params;
        pstSipMsgDe->pickSipUriParams(pstSipUri, params);
        pstDecodedPart->_fromPart.setSipUriParams(params);
    }

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    pstSipUri = 0;
    SipMsgCommon::getFromToSipUri(pstSipMsgDe->_pstMsg,false, &pstSipUri);
    if (pstSipUri != 0)
    {
        vector<ST_SIP_URI_PARM> params;
        pstSipMsgDe->pickSipUriParams(pstSipUri, params);
        pstDecodedPart->_toPart.setSipUriParams(params);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;

    //get CSeq Header
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iCseqVal = iCseq;
        pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
    }

    //get Via Header
    std::string strViaUri, strViaBrach, strRecvHost;
    int iRport = 0;
    pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri,iPort, strRecvHost,iRport);
    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }
    pstDecodedPart->_viaPart.setViaPart(strViaUri, iPort,  ucTptType, false,  iRport,strViaBrach);

    vector<ST_SIP_HDR_VIA> vias;
    if (pstSipMsgDe->getHrdVias(vias))
        pstDecodedPart->_vias = vias;

    pstSipDialog->pushbackDecodeObj(pstSipMsgDe);

    //get Max-Forward Header
    int iMaxForwards = 0;
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
    {
        pstDecodedPart->_MaxForwards = iMaxForwards;
    }

    //get Supported Header
    vector<int> vectSuptFlags;
    vector<std::string> vectTypeName;
    if (pstSipMsgDe->getHdrSupported(vectSuptFlags, vectTypeName))
    {
        pstDecodedPart->_vectSuptFlag = vectSuptFlags;
    }

    //get Allow Header
    vector<int> vectAllow;
    vectTypeName.clear();
    if (pstSipMsgDe->getHdrAllow(vectAllow, vectTypeName))
    {
        pstDecodedPart->_vectAllowFlag = vectAllow;
    }

    //get Contact Header
    std::string strHostName;
    ZCHAR  zcTpt;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt))
    {
        pstDecodedPart->_fromPart.setFromToContact(strHostName, iPort);         //set From Contact
        std::string temAddress;
        SipMsgCommon::generateToSBCConnectionId(strHostName, iPort, temAddress);
        bool isPrivate = privateIpCheck(strHostName);
        if (temAddress != toSBCConnectionId && iPort > 0 && !isPrivate)
        {
            pstSipDialog->setToSBCConnectionId(temAddress);
        }
    }
    //get Contact SipUri params
    vector<ST_SIP_URI_PARM>  sipUriParams;
    bool ucResult = pstSipMsgDe->getHdrContactSipUriParams(sipUriParams);
    if (false == ucResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, getHdrContactSipUriParams() return FALSE! SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
    }
    else
    {
        TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called, getHdrContactSipUriParams() return TRUE! _vectUriParms size:" + std::to_string(sipUriParams.size())
            + ",SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        pstDecodedPart->_fromPart.setContactSipUriParams(sipUriParams);
    }

    ST_SIP_SIP_URI stSipUri;
    if (pstSipMsgDe->getHdrContactSipUri(stSipUri))
        pstDecodedPart->_peerSipUri.setPeerSipUri(stSipUri);

    //decode RSeq Header
    if (pstSipMsgDe->getRseq(ulRseqNum))
    {
        pstDecodedPart->_rseqPart._respNum = ulRseqNum;
    }

    //P-Called-Party-ID
    std::string strDispInfo;
    int dwPort = 0;
    if (pstSipMsgDe->getHdrPCalledPartyID(strDispInfo, strHostName, dwPort))
    {
        pstDecodedPart->_pCalledPartyID.setInfo(strDispInfo, strHostName);
    }

    //get Record-Route Header
    vector<ST_SIP_SIP_URI> stSipUris;
    pstSipMsgDe->getHdrRecordRouteSipUris(stSipUris);
    if (stSipUris.size() > 0)
    {
        pstSipDialog->_sipMsgDecodePart._recordRoutes._sipUris = stSipUris;

        ST_SIP_SIP_URI sipUri = stSipUris.at(0);
        std::string strUserInfo, strPassword;
        iPort = 0;
        SipMsgCommon::deSipUri(&sipUri, strUserInfo, strPassword, strHostName, iPort);
        if (iPort == 0)
            iPort = 5060;

        std::string temAddress;
        SipMsgCommon::generateToSBCConnectionId(strHostName, iPort, temAddress);

        pstSipDialog->setToSBCConnectionId(temAddress);
    }
    //get SDP body
    bool existSDP = false;
    std::string strSDPBody;
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod, bReqPres,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            pstSipDialog->setSdpOffer(strSDPBody);
            pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strSDPBody);
        }
        else
        {
            TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called, checkSipMsgWithSDP() return EMPTY body! SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        }
    }

    //get Session-Expires Header
    int seconds = 0;
    bool refresherPre = false,  uasRefresh = false;
    if (pstSipMsgDe->getHdrSessionExpires(seconds, refresherPre, uasRefresh))
    {
        int64_t curTime = TNOWMS;
        pstSipDialog->_sipMsgDecodePart._sessionExpiresPart.setSessionExpiresPart(seconds,curTime,refresherPre, uasRefresh);
    }

    //get UUI Header
    std::string uuiData;
    if (pstSipMsgDe->getHdrExt(SIP_GW_UUI_HEAER_NAME, uuiData))
    {
        if (uuiData.empty() == false)
        {
            int pos = uuiData.find(";encoding");
            uuiData = pos > 0 ? uuiData.substr(0, pos) : uuiData;
        }
        pstSipDialog->setUuiData(uuiData);
    }

    pSipGatwway->onSipMessageRequest(enMethodType, pstSipDialog, sipProxyId, "",strSDPBody, params);

    return true;
}

bool SipMsg::dealAckRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                            const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pSipGatwway == NULL! SipMsg:\n" + strBuffer);
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    unsigned char ucMethod = 0, ucTptType = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    ucTptType = EN_SIP_TPT_UDP;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }
    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //别的方法，比如ACK 不需要重新创建对话框 SipDialog
    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        //异常流程处理，可能导致清理掉对话后，ACK才返回
        std::string strSipDialogIdTo = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\"; 
        pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogIdTo);
        if (!pstSipDialog)
        {
            //�쳣��̴��������ܵ��������Ի��ACK󣬲ŷ��
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  222 call getSipDialogPtr() failed to get the SipDialogPtr when received a Request., with SipDialogId : " + strSipDialogId + " ,SipMsg:\n" + strBuffer);
            pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
            return false;
        }
    }

    pSipGatwway->onSipMessageRequest(enMethodType, pstSipDialog, sipProxyId, "", "", params);

    return true;
}


bool SipMsg::dealBYERequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                            const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  pSipGatwway == NULL! SipMsg:\n" + strBuffer);
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    SipGateway::SipDialogIdPtr sptSipDialogId = 0;
    int   iSceq = 0;
    unsigned char ucMethod, ucTptType;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true;
    SipMsgDecodedPart * pstDecodedPart = 0;
    SipProxy::SipRegisterSubscribePtr pRegisterSub = 0;
    std::string strUserInfo, strHostname;
    int iPort = 0, iReqUriType = 0;
    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    int iCseq = 0;
    std::string strViaUri, strViaBrach, strRecvHost;
    int iRport = 0;
    std::string    strHostName;
    ZCHAR  zcTpt = 0;
    vector<int> vectSuptFlags;
    vector<std::string> vectTypeName;
    vector<int> vectAllow;
    int iMaxForwards = 0;
    ucTptType = EN_SIP_TPT_UDP;
    std::string callIdHost;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    callIdHost = strCallId + "@" + strCallHost;

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //�别的方法，比如ACK 不需要重新创建对话框 SipDialog
    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        //RFC 3261 page 82;  UAS端记录本端的localTag 等对话状态信息，与UAC 端相反，在对话内再发起请求时，此时UAS的local Tag即为 UAC的remote Tag，From的Tag会填为UAC 的To中的Tag，
        //用Winsip测试时发现，RPC 呼叫WinSip，然后WinSip结束，发BYE，tag与之前的发出的INVITE相反
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
        if (!pstSipDialog)
        {
            //异常流程处理，可能导致清理掉对话后，ACK才返回
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,   111 call getSipDialogPtr() failed to get the SipDialogPtr when received a Request(BYE)., with SipDialogId : " + strSipDialogId);

            //strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
            sptSipDialogId = SipGateway::SipDialogId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
            if (0 == sptSipDialogId)
            {
                TLOGWARN("SipMsg",std::string(__FUNCTION__)+"() called,  SipDialogId::create() function call failed! ,SipMsg:\n" + strBuffer);
                return false;
            }
            pstSipDialog = SipGateway::SipDialog::create(sptSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  when received a Request(BYE).Create a SipDilalog failed, with SipDialogId : " + strSipDialogId + " ,SipMsg:\n" + strBuffer);
                return false;
            }
            pstSipDialog->setReSendBye(true);
        }
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    //Set ReqLine DecodePart
    pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_bReqPres = true;

    //get From Header
    //如果 从RPC向WinSip发起INVITE呼叫，由WinSip结束，发BYE，则WinSip在组建BYE时，此时，与最初的INVITE的相比，From和To会因为 主被叫各自所记录的对话状态，即 local tag ,remote tag 等消息的相反，导致To和From相反
    //因此，暂时，接受BYE时，不修改本端状态
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    //get To Header    RPC呼叫SIP，收到的407等回复To tag与180不一致，而BYE应该取180/200 的to tag
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;

    //get CSeq Header
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iCseqVal = iCseq;
        pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
    }

    ////get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        if (viaHeaders.size() == 1)
        {
            ViaPart viaPart = viaHeaders.at(0);
            bool breport = viaPart._iRport > 0 ? true : false;
            pstDecodedPart->_viaPart.setViaPart(viaPart._strHostName, viaPart._iPort,  viaPart._ucTptType, breport,  viaPart._iRport, viaPart._strBranch);
        }
    }

    //get Header via
    vector<ST_SIP_HDR_VIA> vias;
    if (pstSipMsgDe->getHrdVias(vias))
        pstDecodedPart->_vias = vias;

    pstSipDialog->pushbackDecodeObj(pstSipMsgDe);

    //get Max-Forward Header
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
    {
        pstDecodedPart->_MaxForwards = iMaxForwards;
    }

    //get Supported Header
    if (pstSipMsgDe->getHdrSupported(vectSuptFlags, vectTypeName))
    {
        pstDecodedPart->_vectSuptFlag = vectSuptFlags;
    }

    //get Allow Header
    if (pstSipMsgDe->getHdrAllow(vectAllow, vectTypeName))
    {
        pstDecodedPart->_vectAllowFlag = vectAllow;
    }

    //get Contact Header
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt))
    {
        pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);         //set To Contact
    }

    std::string code;
    if (pstSipMsgDe->getHdrExt("ocs-term-id", code))
    {
        pstSipDialog->setOCSErrorCode(code);
    }

    //通知 请求
    pSipGatwway->onSipMessageRequest(enMethodType, pstSipDialog, sipProxyId, "", "", params);
    return true;
}

bool SipMsg::dealBYEResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                             const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    unsigned char ucMethod = 0, ucTptType = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = false;
    SipMsgDecodedPart * pstDecodedPart = 0;

    ucTptType = EN_SIP_TPT_UDP;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    //get SipDialog
    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
        if (pstSipDialog == 0)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  444 call getSipDialogPtr() failed to get the SipDialogPtr when received a Responce, with SipDialogId : " + strSipDialogId + " SipMsg:\n" + strBuffer);
            pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
            return false;
        }
    }

    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    std::string strRspReason, strSpeak;
    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    pstSipMsgDe->getRspReason(strRspReason);
    pstDecodedPart->_bReqPres = false;
    pstDecodedPart->_statusLinePart._iCode = ulStatusCode;
    pstDecodedPart->_statusLinePart._strReason = strRspReason;

    pSipGatwway->onSipMessageResponse(ulStatusCode, enMethodType, pstSipDialog,  sipProxyId, "", "");
    return true;
}

bool SipMsg::dealCANCELRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                               const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,. pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    unsigned char ucMethod = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }
    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        //RFC 3261 page 82;  UAS端记录本端的localTag 等对话状态信息，与UAC 端相反，在对话内再发起请求时，此时UAS的local Tag即为 UAC的remote Tag，From的Tag会填为UAC 的To中的Tag，
        //用Winsip测试时发现，RPC 呼叫WinSip，然后WinSip结束，发BYE，tag与之前的发出的INVITE相反
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,222 call getSipDialogPtr() failed to get the SipDialogPtr when received a Request., with SipDialogId : " + strSipDialogId + " ,SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
        return false;
    }

    //get Via Header
    vector<ST_SIP_HDR_VIA> vias;
    if (pstSipMsgDe->getHrdVias(vias))
        pstSipDialog->_sipMsgDecodePart._vias = vias;

    pstSipDialog->pushbackDecodeObj(pstSipMsgDe);

    pSipGatwway->onSipMessageRequest(enMethodType, pstSipDialog, sipProxyId, "", "", params);
    return true;
}

bool SipMsg::dealCANCELResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                                const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    SipGateway::SipDialogIdPtr sptSipDialogId = 0;
    unsigned int ulStatusCode = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    SipMsgDecodedPart * pstDecodedPart = 0;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg",std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
        if (pstSipDialog == 0)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, 444 call getSipDialogPtr() failed to get the SipDialogPtr when received a Responce, with SipDialogId : " + strSipDialogId + " SipMsg:\n" + strBuffer);
            pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
            return false;
        }
    }

    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    std::string strRspReason, strSpeak;
    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    pstSipMsgDe->getRspReason(strRspReason);

    pstDecodedPart->_bReqPres = false;
    pstDecodedPart->_statusLinePart._iCode = ulStatusCode;
    pstDecodedPart->_statusLinePart._strReason = strRspReason;
    pSipGatwway->onSipMessageResponse(ulStatusCode, enMethodType,pstSipDialog,  sipProxyId, "", "");

    return true;
}

bool SipMsg::dealUPDATERequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                               const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    SipGateway::SipDialogIdPtr sptSipDialogId = 0;
    unsigned char ucMethod, ucTptType;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true;         //Request present flag;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string strUserInfo, strHostname, strSpeak;
    int iPort = 0, iReqUriType = 0;
    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    std::string strViaUri, strViaBrach, strRecvHost;
    int iRport = 0;
    unsigned long  ulStatusCode = 0;
    ucTptType = EN_SIP_TPT_UDP;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (pstSipDialog && ucMethod == EN_SIP_METHOD_INVITE)
    {
        TLOGINFO("SipMsg", std::string(__FUNCTION__)+"() called,.method == INVITE or UPDATE, getSipDialogPtr() return TURE! SipMsg:\n" + strBuffer);
        //此时说明是重发INVITE，忽略掉
        return true;
    }

    //create SipDialogId object
    sptSipDialogId = SipGateway::SipDialogId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, SipDialogId::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipGateway::SipDialog::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("SipMsg",std::string(__FUNCTION__)+"() called, SipDialog::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    pstSipDialog->setSipCallRpcProp(true);         //标示是由SIP 发起的呼叫

    //Set ReqLine DecodePart
    pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_bReqPres = true;

    //get From Header
    //如果 从RPC向WinSip发起INVITE呼叫，由WinSip结束，发BYE，则WinSip在组建BYE时，此时，与最初的INVITE的相比，From和To会因为 主被叫各自所记录的对话状态，即 local tag ,remote tag 等消息的相反，导致To和From相反
    //因此，暂时，接受BYE时，不修改本端状态
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    //get To Header
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iCseqVal = iCseq;
        pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
    }

    //RPC呼叫SIP，收到的407等回复To tag与180不一致，而BYE应该取180/200 的to tag
    //get Via Header
    pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri,iPort, strRecvHost,iRport);
    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }
    pstDecodedPart->_viaPart.setViaPart(strViaUri, iPort,  ucTptType, false,  iRport,strViaBrach);

    vector<ST_SIP_HDR_VIA> vias;
    if (pstSipMsgDe->getHrdVias(vias))
        pstDecodedPart->_vias = vias;

    //get Max-Forward Header
    int iMaxForwards = 0;
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
    {
        pstDecodedPart->_MaxForwards = iMaxForwards;
    }

    //get Supported Header
    vector<int> vectSuptFlags;
    vector<std::string> vectTypeName;
    if (pstSipMsgDe->getHdrSupported(vectSuptFlags, vectTypeName))
    {
        pstDecodedPart->_vectSuptFlag = vectSuptFlags;
    }

    //get Allow Header
    vector<int> vectAllow;
    vectTypeName.clear();
    if (pstSipMsgDe->getHdrAllow(vectAllow, vectTypeName))
    {
        pstDecodedPart->_vectAllowFlag = vectAllow;
    }

    //get Contact Header
    std::string    strHostName;
    ZCHAR  zcTpt;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt ))
    {
        pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);         //set To Cantact
    }

    //get Contact SipUri params
    vector<ST_SIP_URI_PARM>  sipUriParams;
    bool ucResult = pstSipMsgDe->getHdrContactSipUriParams(sipUriParams);
    if (false == ucResult)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, getHdrContactSipUriParams() return FALSE! SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
    }
    else
    {
        TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called, getHdrContactSipUriParams() return TRUE! _vectUriParms size:" + std::to_string(sipUriParams.size())
            + ",SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        pstDecodedPart->_fromPart.setContactSipUriParams(sipUriParams);
    }

    //get Session-Expires Header
    int seconds = 0;
    bool refresherPre = false,  uasRefresh = false;
    if (pstSipMsgDe->getHdrSessionExpires(seconds, refresherPre, uasRefresh))
    {
        int64_t curTime = TNOWMS;
        pstSipDialog->_sipMsgDecodePart._sessionExpiresPart.setSessionExpiresPart(seconds,curTime,refresherPre, uasRefresh);
    }

    ZUINT second = 0;
    if (pstSipMsgDe->getHdrMinSe(second))
        pstSipDialog->_sipMsgDecodePart._minSe = second;

    //get SDP body
    bool existSDP = false;
    std::string	strSDPBody;
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod, bReqPres,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            pstSipDialog->setSdpOffer(strSDPBody);
            pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strSDPBody);         //set 200 ok SDP as SDP body
        }
        else
        {
            TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called, checkSipMsgWithSDP() return EMPTY body! SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        }
    }

    //通知 请求
    pSipGatwway->onSipMessageRequest(enMethodType, pstSipDialog, sipProxyId, "", strSDPBody, params);
    return true;
}

bool SipMsg::dealUPDATEResponse(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                                const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, pSipGatwway == NULL! ERROR! SipMsg:\n" + strBuffer);
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strSDPBody;
    bool existSDP = false;
    unsigned char ucMethod = EN_SIP_METHOD_UPDATE, ucTptType = 0, ucAddrType = 0;

    ucTptType = EN_SIP_TPT_UDP;
    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        std::string strSipDialogIdTo = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogIdTo);
        if (pstSipDialog == 0)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, 444 call getSipDialogPtr() failed to get the SipDialogPtr when received a Responce, with SipDialogId : " + strSipDialogId + " SipMsg:\n" + strBuffer);
            pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
            return false;
        }
    }

    //resp header
    std::string strRspReason;
    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    pstSipMsgDe->getRspReason(strRspReason);

    //get SDP body
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod,  false,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            //pstSipDialog->setSdpAnswer(strSDPBody);
            //pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, strSDPBody);
        }
        else
        {
            TLOGDEBUG("SipMsg", std::string(__FUNCTION__)+"() called, checkSipMsgWithSDP() return EMPTY body! SipDialogId " + pstSipDialog->id() + " ,SipMsg:\n" +strBuffer);
        }
    }

    //get Session-Expires Header
    int seconds = 0;
    bool refresherPre = false, uasRefresh = false;
    if (pstSipMsgDe->getHdrSessionExpires(seconds, refresherPre, uasRefresh))
    {
        int64_t curTime = TNOWMS;
        pstSipDialog->_sipMsgDecodePart._sessionExpiresPart.setSessionExpiresPart(seconds,curTime,refresherPre, uasRefresh);

        if (ulStatusCode == 200)
        {
            if (refresherPre && !uasRefresh)
            {
                pstSipDialog->setIsFreshDialog(true);
            }
        }
    }

    pSipGatwway->onSipMessageResponse(ulStatusCode, SipGateway::kSipRequestMethodUPDATE, pstSipDialog,  sipProxyId, "", strSDPBody);
    return true;
}

bool SipMsg::dealPRACKRequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                              const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called, dealPRACKRequest pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    SipGateway::SipDialogIdPtr sptSipDialogId = 0;
    unsigned char ucMethod, ucTptType;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string strUserInfo, strHostname, strSpeak;
    int iPort = 0, iReqUriType = 0;
    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    std::string strViaUri, strViaBrach, strRecvHost;
    int iRport = 0;
    std::string connectionId;

    ucTptType = EN_SIP_TPT_UDP;
    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. inner error,  SipMsg:\n" + strBuffer);
        return false;
    }
    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }
    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);
    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call.  222 call getSipDialogPtr() failed to get the SipDialogPtr when received a Request., with SipDialogId : " + strSipDialogId + " ,SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
        return false;
    }
    connectionId = pstSipDialog->getToSBCConnectionId();

    //create SipDialogId object
    sptSipDialogId = SipGateway::SipDialogId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. SipDialogId::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipGateway::SipDialog::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. SipDialog::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    pstSipDialog->setSipCallRpcProp(true);         //标示是由SIP 发起的呼叫
    pstSipDialog->setToSBCConnectionId(connectionId);

    //Set ReqLine DecodePart
    pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_bReqPres = true;

    //get From Header
    //如果 从RPC向WinSip发起INVITE呼叫，由WinSip结束，发BYE，则WinSip在组建BYE时，此时，与最初的INVITE的相比，From和To会因为 主被叫各自所记录的对话状态，即 local tag ,remote tag 等消息的相反，导致To和From相反
    //因此，暂时，接受BYE时，不修改本端状态
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    //get To Header
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iCseqVal = iCseq;
        pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
    }

    //RPC呼叫SIP，收到的407等回复To tag与180不一致，而BYE应该取180/200 的to tag
    //get Via Header
    pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri,iPort, strRecvHost,iRport);
    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }
    pstDecodedPart->_viaPart.setViaPart(strViaUri, iPort, ucTptType, false, iRport,strViaBrach);

    vector<ST_SIP_HDR_VIA> vias;
    if (pstSipMsgDe->getHrdVias(vias))
        pstDecodedPart->_vias = vias;
    pstSipDialog->pushbackDecodeObj(pstSipMsgDe);

    //get Max-Forward Header
    int iMaxForwards = 0;
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
    {
        pstDecodedPart->_MaxForwards = iMaxForwards;
    }

    //get Contact Header
    std::string strHostName;
    ZCHAR  zcTpt;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt ))
    {
        pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);
    }

    pSipGatwway->onSipMessageRequest(enMethodType, pstSipDialog, sipProxyId, "", "", params);
    return true;
}

bool SipMsg::dealPRACKResponce(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                               const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    SipMsgDecodedPart * pstDecodedPart = 0;

    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
        if (pstSipDialog == 0)
        {
            TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call.  444 call getSipDialogPtr() failed to get the SipDialogPtr when received a Responce, with SipDialogId : " + strSipDialogId + " SipMsg:\n" + strBuffer);
            pSipGatwway->ereaseSipProxyExceptionConnection(strSipDialogId, sipProxyId);
            return false;
        }
    }

    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    std::string strRspReason, strSpeak;
    pstSipMsgDe->getRspStatusCode(ulStatusCode);
    pstSipMsgDe->getRspReason(strRspReason);

    pstDecodedPart->_bReqPres = false;
    pstDecodedPart->_statusLinePart._iCode = ulStatusCode;
    pstDecodedPart->_statusLinePart._strReason = strRspReason;

    int iCseq = 0;
    if(pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iPrackCseqVal = iCseq;
    }
    pSipGatwway->onSipMessageResponse(ulStatusCode, enMethodType,pstSipDialog,  sipProxyId, "", "");
    return true;
}

/**
* @decode the INFO Request msg from SipProxy
* param [in] strBuffer, the received sip msg.
* param [in] iBufLen, the length of sipmsg.
* @param [out] pstEnSipMsg.
*/
bool SipMsg::dealINFORequest(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                             const std::string & sipProxyId, const std::string& toSBCConnectionId, const SipMsgDecodePtr & pstSipMsgDe, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. dealINFORequest pSipGatwway == NULL! ERROR!");
        return false;
    }

    SipGateway::SipDialogPtr pstSipDialog = 0;
    SipGateway::SipDialogIdPtr sptSipDialogId = 0;
    unsigned char ucMethod, ucTptType;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string strUserInfo, strHostname, strSpeak;
    int iPort = 0, iReqUriType = 0;
    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    std::string strViaUri, strViaBrach, strRecvHost;
    int iRport = 0;

    ucTptType = EN_SIP_TPT_UDP;
    if (!pstSipMsgDe)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //method converter
    SipGateway::SipRequestMethodType enMethodType = SipGateway::SipDialog::SipMethodToSipDialogMethod((EN_SIP_METHOD)ucMethod);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\";         //初始存储的时候没有 To头域的Tag，所以获取的时候也不要
    pstSipDialog = pSipGatwway->getSipDialogPtr(strSipDialogId);
    if (pstSipDialog && ucMethod == EN_SIP_METHOD_INVITE)
    {
        TLOGINFO("SipMsg", std::string(__FUNCTION__) + "() call.method == INVITE or UPDATE, getSipDialogPtr() return TURE! SipMsg:\n" + strBuffer);
        //此时说明是重发INVITE，忽略掉
        return true;
    }

    //create SipDialogId object
    sptSipDialogId = SipGateway::SipDialogId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("SipMsg",std::string(__FUNCTION__) + "() call. SipDialogId::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipGateway::SipDialog::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() call. SipDialog::create() function call failed! SipMsg:\n" + strBuffer);
        pSipGatwway->ereaseSipProxyExceptionConnection(sptSipDialogId->string(), sipProxyId);
        return false;
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    pstSipDialog->setSipCallRpcProp(true);         //标示是由SIP 发起的呼叫

    //Set ReqLine DecodePart
    pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_bReqPres = true;

    //get From Header
    //如果 从RPC向WinSip发起INVITE呼叫，由WinSip结束，发BYE，则WinSip在组建BYE时，此时，与最初的INVITE的相比，From和To会因为 主被叫各自所记录的对话状态，即 local tag ,remote tag 等消息的相反，导致To和From相反
    //因此，暂时，接受BYE时，不修改本端状态
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    //get To Header
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iCseqVal = iCseq;
        pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
    }

    //RPC呼叫SIP，收到的407等回复To tag与180不一致，而BYE应该取180/200 的to tag
    //get Via Header
    pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri,iPort, strRecvHost,iRport);
    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }
    pstDecodedPart->_viaPart.setViaPart(strViaUri, iPort,  ucTptType, false,  iRport,strViaBrach);

    vector<ST_SIP_HDR_VIA> vias;
    if (pstSipMsgDe->getHrdVias(vias))
        pstDecodedPart->_vias = vias;

    pstSipDialog->pushbackDecodeObj(pstSipMsgDe);

    //get Max-Forward Header
    int iMaxForwards = 0;
    if(pstSipMsgDe->getMaxForward(iMaxForwards))
    {
        pstDecodedPart->_MaxForwards = iMaxForwards;
    }

    //get Supported Header
    vector<int> vectSuptFlags;
    vector<std::string> vectTypeName;
    if (pstSipMsgDe->getHdrSupported(vectSuptFlags, vectTypeName))
    {
        pstDecodedPart->_vectSuptFlag = vectSuptFlags;
    }

    //get Allow Header
    vector<int> vectAllow;
    vectTypeName.clear();
    if (pstSipMsgDe->getHdrAllow(vectAllow, vectTypeName))
    {
        pstDecodedPart->_vectAllowFlag = vectAllow;
    }

    //get Contact Header
    std::string    strHostName;
    ZCHAR  zcTpt;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt ))
    {
        pstDecodedPart->_toPart.setFromToContact(strHostName, iPort);         //set To Cantact
    }

    //get Session-Expires Header
    int seconds = 0;
    bool refresherPre = false,  uasRefresh = false;
    if (pstSipMsgDe->getHdrSessionExpires(seconds, refresherPre, uasRefresh))
    {
        int64_t curTime = TNOWMS;
        pstSipDialog->_sipMsgDecodePart._sessionExpiresPart.setSessionExpiresPart(seconds,curTime,refresherPre, uasRefresh);
    }

    pSipGatwway->onSipMessageRequest(enMethodType, pstSipDialog, sipProxyId, "", "", params);
    return true;
}

// bool SipMsg::SendRequestMESSAGE(const SipMessageGateway::SipMessagePtr & pMessage,const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy)
// {
//     SipMsgEncodePtr pstSipMsgEn = std::make_shared<SipMsgEncode>();
//     std::string fromNumber, toNumber, coreNetId, registerHost, sbcIp, listenIp;
//     int registerPort = 0, sbcPort = 0, listenPort = 0, iStartCseqNum = 0;
//     ZUCHAR ucMethod = 0, ucTptType = 0;
//     std::string messageId, strFromTag, callIdHost, callIdNumber,localHost,strBranch;
//     std::string contentDisp;
//     ZUCHAR mtype = 0, msubType = 0;
//     std::string mtypeExt, msubtypeExt;
//     std::string msgPrefix;

//     if (!pMessage)
//     {
//         TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. pMessage is numm.");
//         return false;
//     }
//     SipMsgDecodedPart *pstDecodedPart = &pMessage->_sipMsgDecodePart;
//     //get Client and Registrar ip and port
//     pMessage->getCoreNetInfo(coreNetId, sbcIp, sbcPort, registerHost, registerPort);
//     pMessage->getUacFromTo(fromNumber, toNumber);
//     messageId = pMessage->getMessageId();
//     ucTptType = ucTptType = pstDecodedPart->_viaPart._ucTptType;
//     ucMethod = EN_SIP_METHOD_MSG;
//     pMessage->getListenIpPort(listenIp, listenPort);
//     callIdHost = pMessage->getCallIdHost();
//     contentDisp = pMessage->_sipMsgDecodePart._contentDisp;
//     pstDecodedPart->_contentType.getContentType(mtype, mtypeExt, msubType, msubtypeExt);
//     msgPrefix = pMessage->getReqPrefix();

//     if (!pstSipMsgEn->createReq())
//     {
//         TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. createReq failed .Message id:" + messageId);
//         return false;
//     }

//     //fill req line
//     pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod,toNumber,registerHost,registerPort, EN_SIP_REQ_URI_SIP);
//     pstSipMsgEn->fillReqLineBySip(ucMethod, toNumber, registerHost, registerPort);

//     //fill From Header
//     strFromTag = tars::TC_UUIDGenerator::getInstance()->genID();
//     pstDecodedPart->_fromPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, fromNumber, fromNumber, registerHost,registerPort, strFromTag, ucTptType);
//     bool bResult = pstSipMsgEn->fillHdrFromToByName(true,fromNumber, fromNumber, registerHost,registerPort,  strFromTag, pstDecodedPart->_fromPart._ucAddrType);
//     if (!bResult)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. call fillHdrFromToByName() failed, fill From Header .Message id:" + messageId);
//         return false;
//     }

//     //fill To Header
//     pstDecodedPart->_toPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, toNumber,toNumber, registerHost, registerPort, "", ucTptType);
//     bResult = pstSipMsgEn->fillHdrFromToByName(false, toNumber, toNumber, registerHost,registerPort,  "", pstDecodedPart->_toPart._ucAddrType);
//     if (!bResult)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.call fillHdrFromToByName() failed, fill To Header .Message id:" + messageId);
//         return false;
//     }

//     //fill Via Header
//     strBranch = SipMsgCommon::createBranch();
//     pstDecodedPart->_viaPart.setViaPart(listenIp, listenPort, ucTptType, false, 0, strBranch);
//     bResult = pstSipMsgEn->fillHdrVia(ucTptType, listenIp, listenPort, false,0,strBranch);
//     if (!bResult)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrVia() failed. Message id:" + messageId);
//         return false;
//     }

//     //fill Call-Id Header
//     SipMsgCommon::GetCallIdNumberHost2(callIdHost, callIdNumber,localHost);
//     pstDecodedPart->_callIdPart.setCallIdPart(callIdNumber, localHost);
//     bResult = pstSipMsgEn->fillHdrCallId(callIdNumber, localHost);
//     if (!bResult)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called.. call fillHdrCallId() failed Message id:" + messageId);
//         return false;
//     }

//     //fill CSeq Header
//     if (pstDecodedPart->_cseqPart._iCseqVal == 0)
//     {
//         iStartCseqNum = Common::getRand(1000);
//         pstDecodedPart->_cseqPart.setCseqPart(iStartCseqNum, ucMethod);
//     }
//     else
//     {
//         iStartCseqNum = pstDecodedPart->_cseqPart._iCseqVal + 1;
//         pstDecodedPart->_cseqPart._iCseqVal = iStartCseqNum;
//     }
//     pstSipMsgEn->fillHdrCseq(ucMethod,iStartCseqNum);

//     //fill Max-Forwards Header
//     pstSipMsgEn->fillHdrMaxForwards(70);

//     //fill Contact Header
//     bResult = pstSipMsgEn->fillHdrContact(fromNumber, listenIp, listenPort, ucTptType);
//     if (!bResult)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrContact() failed Message id:" + messageId);
//         return false;
//     }

//     //fill Header Content-Disposition
//     if (contentDisp.empty())
//         contentDisp = tars::TC_UUIDGenerator::getInstance()->genID() + tars::TC_UUIDGenerator::getInstance()->genID();
//     bResult = pstSipMsgEn->fillHdrContentDisp(contentDisp);
//     if (!bResult)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrContentDisp() failed Message id:" + messageId);
//         return false;
//     }

//     //fill Content-Type Header
//     bResult = pstSipMsgEn->fillHdrContentType(mtype, mtypeExt, msubType, msubtypeExt);
//     if (!bResult)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrContentType() failed Message id:" + messageId);
//         return false;
//     }

//     //fill Body
//     if (pstDecodedPart->_bodyPart._strBody.empty() == false)
//     {
//         bResult = pstSipMsgEn->fillBodySDPDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, pstDecodedPart->_bodyPart._strBody);
//         if (!bResult)
//         {
//             TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..call fillHdrContentType() failed Message id:" + messageId);
//             return false;
//         }
//     }

//     //* fill Expires header */
//     pstSipMsgEn->fillHdrExpires(70);

//     if (!pstSipMsgEn->encode())
//     {
//         TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called. call encode(), return FALSE! Message id:" + messageId);
//         return false;
//     }

//     std::string strConnectionId;
//     SipMsgCommon::generateToSBCConnectionId(sbcIp, sbcPort, strConnectionId);
//     std::string reason;
//     std::string msgSent = msgPrefix + pstSipMsgEn->_strMsg;

//     return pSipProxy->sendEncodedMessage(msgSent, msgSent.size(), strConnectionId, true, reason);
// }

// bool SipMsg::dealMessageResponceFromSBC(const std::string &strSipMsg,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
// {
//     std::string strCallId, strCallHost;
//     unsigned int ulStatusCode = 0;
//     SipMessageGateway::SipMessagePtr pMessage = 0;
//     std::string dstUri, callIdHost;
//     bool InvalidMsg = false;

//     if (!pstSipMsgDe)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. inner error, SipMsg:\n" + strSipMsg);
//         pSipProxy->saveExceptionConnection(pSipConnection);
//         return false;
//     }

//     //get To Header ====
//     std::string fromDisp, toDisp, fromUserName,toUserName, strHost;
//     int  iPort = 0;
//     unsigned char ucAddrType;
//     if(pstSipMsgDe->getHdrFromToUri(false, toDisp, toUserName, strHost, iPort, ucAddrType) == false)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "get to Header info return FALSE,. SipMsg:\n" + strSipMsg);
//         pSipProxy->saveExceptionConnection(pSipConnection);
//         return false;
//     }

//     //get From Header ====
//     if(pstSipMsgDe->getHdrFromToUri(true, fromDisp, fromUserName, strHost, iPort, ucAddrType) == false)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "get from Header info return FALSE,. SipMsg:\n" + strSipMsg);
//         pSipProxy->saveExceptionConnection(pSipConnection);
//         return false;
//     }
//     dstUri = SipMessageGateway::SipMessage::generateDstUri(fromUserName, toUserName, strHost);

//     //get Call-Id Header
//     pstSipMsgDe->getCallId(strCallId, strCallHost);
//     callIdHost = strCallId + "@" + strCallHost;

//     pMessage = pSipProxy->getSipMessage(dstUri, callIdHost);
//     if (pMessage == 0)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getSipMessage() return 0! dstUri:"+ dstUri +",callIdHost:" + callIdHost + ",SipMsg:\n" + strSipMsg);
//         pSipProxy->saveExceptionConnection(pSipConnection);
//         return false;
//     }

//     //get resp code
//     pstSipMsgDe->getRspStatusCode(ulStatusCode);

//     return pSipProxy->onSipMessageResponse2(strSipMsg, SipGateway::kSipRequestMethodMESSAGE, ulStatusCode, pMessage, pSipConnection, InvalidMsg) ;
// }

// bool SipMsg::dealMessageRequestFromSBC(const std::string &strBuffer,int iBufLen, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy, void* pSipConnection, const SipMsgDecodePtr & pstSipMsgDe)
// {
//     SipMessageGateway::SipMessagePtr pMessage = 0;
//     std::string strLocalTag,strRemoteTag;
//     bool InvalidMsg = false;
//     int iCseq = 0;
//     unsigned char ucMethod = EN_SIP_METHOD_MSG, ucTptType = 0;
//     std::string toSBCConnectionId;
//     int  iPort = 0;
//     unsigned char ucAddrType = 0;
//     int iReqUriType = 0;
//     std::string dstUri, callIdHost;

//     if (!pstSipMsgDe)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. inner error, SipMsg:\n" + strBuffer);
//         pSipProxy->saveExceptionConnection(pSipConnection);
//         return false;
//     }

//     // get Call-Id Header
//     std::string strCallId, strCallHost;
//     pstSipMsgDe->getCallId(strCallId, strCallHost);
//     pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
//     pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
//     CallIdPart callIdPart;
//     callIdPart.setCallIdPart(strCallId, strCallHost);
//     callIdHost = strCallId + "@" + strCallHost;

//     //Set ReqLine DecodePart
//     ReqLinePart reqPart;
//     std::string strUserInfo, strHostname;
//     pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
//     reqPart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);

//     //get From Header
//     FromToPart fromPart;
//     std::string strDispName, strUserName, fromUserName, toUserName, strHost;
//     if(pstSipMsgDe->getHdrFromToUri(true, strDispName, fromUserName, strHost, iPort, ucAddrType))
//     {
//         fromPart.setFromToPart(ucAddrType, strDispName,fromUserName,strHost,iPort,strLocalTag,ucTptType);
//     }

//     //get To Header
//     FromToPart toPart;
//     strDispName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
//     if(pstSipMsgDe->getHdrFromToUri(false, strDispName, toUserName, strHost, iPort, ucAddrType))
//     {
//         toPart.setFromToPart(ucAddrType, strDispName,toUserName,strHost,iPort,strRemoteTag,ucTptType);
//     }

//     dstUri = SipMessageGateway::SipMessage::generateDstUri(fromUserName, toUserName, strHost);
//     pMessage = pSipProxy->getSipMessage(dstUri, callIdHost);
//     if (pMessage)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. repeated MESSAGE,dstUri:"+ dstUri +",callIdHost:" + callIdHost + ",SipMsg:\n" + strBuffer);
//         return false;
//     }

//     //get CSeq Header
//     CSeqPart cseqPart;
//     if (pstSipMsgDe->getHdrCseq(iCseq))
//     {
//         cseqPart._iCseqVal = iCseq;
//         cseqPart._ucMethodType = ucMethod;
//     }

//     //get Contact Header
//     std::string strHostName;
//     ZCHAR zcTpt;
//     if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName,iPort, zcTpt ))
//     {
//         fromPart.setFromToContact(strHostName, iPort);
//         std::string temAddress;
//         if (iPort != 0)
//             SipMsgCommon::generateToSBCConnectionId(strHostName, iPort, toSBCConnectionId);
//     }

//     //get Via Header
//     ViaPart viaPart;
//     std::string strViaUri, strViaBrach, strRecvHost, toSBCConnetioId2;
//     int iRport = 0;
//     if (pstSipMsgDe->getHdrViaOther(ucTptType, strViaUri,iPort, strRecvHost,iRport))
//     {
//         if (iPort != 0)
//             SipMsgCommon::generateToSBCConnectionId(strViaUri, iPort, toSBCConnetioId2);
//     }

//     if (pstSipMsgDe->getViaBranch(strViaBrach))
//     {
//         viaPart._strBranch = strViaBrach;
//     }
//     viaPart.setViaPart(strViaUri, iPort, ucTptType, false, iRport,strViaBrach);
//     if (toSBCConnectionId != toSBCConnetioId2 && toSBCConnetioId2.empty() == false)
//     {
//         toSBCConnectionId = toSBCConnetioId2;
//     }

//     //P-Called-Party-ID
//     std::string strDispInfo;
//     int dwPort = 0;
//     PCalledPartyID pCalled;
//     if (pstSipMsgDe->getHdrPCalledPartyID(strDispInfo, strHostName, dwPort))
//     {
//         pCalled.setInfo(strDispInfo, strHostName);
//     }

//     //get SDP body
//     bool existSDP = false;
//     std::string strSDPBody;
//     bool bReqPres = true;
//     unsigned long ulStatusCode = 0;
//     pstSipMsgDe->checkSipMsgWithSDP(ucMethod, bReqPres, ulStatusCode, existSDP, strSDPBody);

//     pMessage = SipMessageGateway::SipMessage::create(fromPart._strUserName, toPart._strUserName, dstUri, strCallId + "@" + strCallHost);
//     pMessage->_sipMsgDecodePart._fromPart = fromPart;
//     pMessage->_sipMsgDecodePart._toPart = toPart;
//     pMessage->_sipMsgDecodePart._viaPart = viaPart;
//     pMessage->_sipMsgDecodePart._bodyPart._strBody = strSDPBody;
//     pMessage->_sipMsgDecodePart._callIdPart = callIdPart;
//     pMessage->_sipMsgDecodePart._cseqPart = cseqPart;
//     pMessage->_sipMsgDecodePart._pCalledPartyID = pCalled;
//     pMessage->_sipMsgDecodePart._reqLinePart = reqPart;
//     pMessage->setToSBCConnectionId(toSBCConnectionId);
//     pMessage->setDirection(false);
//     pMessage->setMsgBody(strSDPBody);

//     //get Via Header
//     vector<ST_SIP_HDR_VIA> vias;
//     if (pstSipMsgDe->getHrdVias(vias))
//         pMessage->_sipMsgDecodePart._vias = vias;

//     pMessage->setDecodeObj(pstSipMsgDe);

//     //get Content-Disp
//     std::string disp;
//     if (pstSipMsgDe->getHdrContentDispEx(disp))
//         pMessage->_sipMsgDecodePart._contentDisp = disp;

//     return pSipProxy->onSipMessageRequest2(pMessage,pSipConnection);
// }

// bool SipMsg::sendMessageResponseToSBC(unsigned int rspCode, const SipMessageGateway::SipMessagePtr & pMessage,
//                                       const SipProxy::SipProxyMsgSenderInterafecePtr & pSipProxy)
// {
//     SipMsgEncodePtr pstSipMsgEn;
//     SipMsgDecodedPart * pstDecodedPart = 0;
//     EN_SIP_METHOD ucMethod = EN_SIP_METHOD_MSG;
//     std::string msgRespPrefix;

//     if (pMessage == 0)
//     {
//         TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called,Input parameter pMessage is NULL.");
//         return false;
//     }

//     pstSipMsgEn = std::make_shared<SipMsgEncode>();
//     if (!pstSipMsgEn->createRsp())
//     {
//         TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called,createRsp failed .Message callId:" + pMessage->getCallIdHost());
//         return false;
//     }

//     msgRespPrefix = pMessage->getRespPrefix();
//     pstDecodedPart = &pMessage->_sipMsgDecodePart;

//     //fill status line
//     pstSipMsgEn->fillStatusLine(rspCode);

//     //fill From Header
//     FromToPart * pFromPart = &pstDecodedPart->_fromPart;
//     pstSipMsgEn->fillHdrFromToByName(true,pFromPart->_strDispName, pFromPart->_strUserName, pFromPart->_strHostName,pFromPart->_dwPort,pFromPart->_strTag, pFromPart->_ucAddrType);

//     //fill To Header
//     FromToPart * pToPart = &pstDecodedPart->_toPart;
//     pToPart->_strTag = SipMsgCommon::createToTag();
//     pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
//         pToPart->_strTag, pToPart->_ucAddrType);

//     //fill Via Header
//      pstSipMsgEn->fillHdrVias(pstDecodedPart->_vias);

//     //fill Call-Id Header
//     pstSipMsgEn->fillHdrCallId(pstDecodedPart->_callIdPart._strNumber, pstDecodedPart->_callIdPart._strHost);

//     //fill CSeq Header
//     pstSipMsgEn->fillHdrCseq(ucMethod, pstDecodedPart->_cseqPart._iCseqVal);

//     bool bodyBack = pMessage->getBodyBack();
//     //fill body
//     if (pstDecodedPart->_bodyPart._strBody.empty() == false && rspCode > 200 && bodyBack)
//     {
//         bool bResult = pstSipMsgEn->fillBodySDPDataStr(EN_SIP_MTYPE_DISC_TEXT, EN_SIP_MSUBTYPE_PLAIN, pstDecodedPart->_bodyPart._strBody);
//         if (!bResult)
//             TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called..");
//     }

//     if (!pstSipMsgEn->encode())
//     {
//         TLOGERROR("SipMsg", std::string(__FUNCTION__) + "() called, encode() return FALSE! Message callId:" + pMessage->getCallIdHost());
//         return false;
//     }

//     std::string reason;
//     std::string msgSent = msgRespPrefix + pstSipMsgEn->_strMsg;
//     void * respCon = pMessage->getRespConnection();
//     if (respCon)
//     {
//         return pSipProxy->sendEncodedMessage2(msgSent, msgSent.size(), respCon,reason);
//     }
//     else
//     {
//         std::string strConnectionId = pMessage->getToSBCConnectionId();
//         return pSipProxy->sendEncodedMessage(msgSent, msgSent.size(), strConnectionId, false, reason);
//     }
// }

SipGateway::SipMsgDecodeResultType SipMsg::onReceiveSipMsgFromSBC2(const std::string &strBuffer,int iBufLen, const SipGateway::SipMsgCodecInterfacePtr & pSipGatwway,
                                                                  const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy,  void* pSipConnection)
{
    if (0 == pSipProxy || 0 == pSipGatwway)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. pSipProxy == NULL! ERROR!");
        return SipGateway::kSipMsgDecodeFailed;
    }

    if (0 == pSipConnection)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. pSipConnection == NULL! ERROR!");
        return SipGateway::kSipMsgDecodeFailed;
    }

    if (strBuffer == "hello")
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return SipGateway::kSipMsgDecodeSuccessed;
    }

    unsigned char ucMethod;
    SipMsgDecodePtr pstSipMsgDe = std::make_shared<SipMsgDecode>();
    bool bReqPres = false;
    bool result = true;

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Sip msg is not valid! Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return SipGateway::kSipMsgDecodeFailed;
    }
    else if (ucResult == EnSipMsgType_Incomplete)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Sip msg is not complete! Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        if (false == sipMessageValidCheck(strBuffer))
            return SipGateway::kSipMsgDecodeFailed;
        return SipGateway::kSipMsgDecodeToSave;
    }
    else if (ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        bool bPing = ucResult == EnSipMsgType_PingRFC5626 ? true : false;
        result = pSipProxy->transferPingPongMsg(strBuffer, strBuffer.size(), bPing, pSipConnection);
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
    }

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. getMethod failed. Msg:\n" + strBuffer);
        pSipProxy->saveExceptionConnection(pSipConnection);
        return SipGateway::kSipMsgDecodeFailed;
    }
    bReqPres = pstSipMsgDe->isRequest() ? true : false;

    std::map<std::string, std::string> params;
    std::string sipProxyId;
    std::string toSBCConnectionId;

    switch(ucMethod)
    {
    case EN_SIP_METHOD_OPTIONS:
        if (bReqPres)
            result = dealOPTIONSRequestFromSBC(strBuffer, strBuffer.size(), pSipProxy, pSipConnection, pstSipMsgDe);
        else
            result = dealOPTIONSResponseFromSBC(strBuffer, strBuffer.size(), pSipProxy, pSipConnection, pstSipMsgDe);
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;

    //case EN_SIP_METHOD_INFO:
    //    if (bReqPres)
    //        dealINFORequest(strBuffer, strBuffer.size(), pSipGatwway, "", "", pstSipMsgDe, std::map<std::string, std::string>());
    //    else
    //        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Not surpported Msg:\n" + strBuffer);
    //    break;

    case EN_SIP_METHOD_INVITE:
        if (bReqPres)
        {
            result = dealInviteRequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            result = dealInviteResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_ACK:
            result = dealAckRequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        break;

    case EN_SIP_METHOD_BYE:
        if (bReqPres)
        {
            result = dealBYERequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            result = dealBYEResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_CANCEL:
        if (bReqPres)
        {
            result = dealCANCELRequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            result = dealCANCELResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_UPDATE:
        if (bReqPres)
        {
            result = dealUPDATERequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        else
        {
            result = dealUPDATEResponse(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_PRACK:
        if (bReqPres)
        {
            TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called, PRACK request NOT supported.SipMsg:\n" + strBuffer);
            return SipGateway::kSipMsgDecodeFailed;
        }
        else
        {
            result = dealPRACKResponce(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_INFO:
        if (bReqPres == false)
        {
            TLOGWARN("SipMsg",  std::string(__FUNCTION__)+"() called,  INFO Response NOT supported.SipMsg:\n" + strBuffer);
            return SipGateway::kSipMsgDecodeFailed;
        }
        else
        {
            result = dealINFORequest(strBuffer, iBufLen, pSipGatwway, sipProxyId, toSBCConnectionId, pstSipMsgDe, params);
        }
        break;


    default:

        TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. Not surpported Msg:\n" + strBuffer);

        //SipGateway::GatewayConnectionPtr ptr = SipGateway::GatewayConnectionPtr::dynamicCast((SipGateway::GatewayConnection *)pSipConnection);
        //if (0 == ptr)
        //{
        //    TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. GatewayConnectionPtr::dynamicCast() return FALSE. Msg:\n" + strBuffer);
        //    return SipGateway::kSipMsgDecodeFailed;
        //}

        //useModule = ptr->getUsedModule();
        //if (useModule == SipGateway::SipProxyModul)
        //{
        //    result = dealOthersSipMessgaeFromSBC(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
        //}
        //else
        //{
        //    result = dealOthersSipMessgaeFromSBC_Ex(strBuffer, strBuffer.size(),  pSipProxy, pSipConnection, pstSipMsgDe);
        //}
        //return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
        break;
    }
    return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;

}

bool SipMsg::getCallerAndCalleeSipUri(std::string & caller, std::string & callee, const std::string & sipMsg)
{
    SipMsgDecodePtr pstDecode;

    pstDecode = std::make_shared<SipMsgDecode>();

    unsigned char ucResult = pstDecode->decode(sipMsg, sipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsg", std::string(__FUNCTION__)+"() called,  pstDecode->decode() return FALSE.sipMsg:\n" + sipMsg);
        return false;
    }

    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
	int iPort = 0;

    if(pstDecode->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
		switch(ucAddrType)
		{
		case EN_SIP_ADDR_SPEC_SIP:
		  	caller = "sip:" + strUserName + strHost;
			break;
		case EN_SIP_ADDR_SPEC_SIPS:
		  	caller = "sips:" + strUserName + strHost;
			break;
		case EN_SIP_ADDR_SPEC_TEL:
		   	caller = "tel:" + strUserName;
			break;
		default:  //其余的类型暂时空缺
			break;
		}
    }

	//get To Header
	strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
	if(pstDecode->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
	{
		switch(ucAddrType)
		{
		case EN_SIP_ADDR_SPEC_SIP:
		  	callee = "sip:" + strUserName + strHost;
			break;
		case EN_SIP_ADDR_SPEC_SIPS:
		  	callee = "sips:" + strUserName + strHost;
			break;
		case EN_SIP_ADDR_SPEC_TEL:
		   	callee = "tel:" + strUserName;
			break;
		default:  //其余的类型暂时空缺
			break;
		}
	}
    return true;
}

bool SipMsg::sipUriValidCheck(const std::string & sipUri, std::string& userName, std::string& registarIp, int& port)
{
    if(sipUri.empty())
        return false;

    int  pos = sipUri.find("@");
    int  pos2 = sipUri.find(":");
    if (pos < 0)
    {
        userName = sipUri;
        return false;
    }
    else
    {
        userName = sipUri.substr(0, pos);

        if(pos2 > 0)
        {
            registarIp = sipUri.substr(pos + 1, pos2 - pos - 1);
            std::string strPort = sipUri.substr(pos2 + 1, sipUri.size() - pos2);
            port = atoi(strPort.c_str());
        }
        else
            registarIp = sipUri.substr(pos + 1, sipUri.size() - pos);

        if (userName.empty() || registarIp.empty())
            return false;
        return true;
    }
}

// SipMessageGateway::SipMessagePtr SipMsg::createSipMessage(const std::string caller, const std::string callee, const std::string msgBody, const std::string & strIp, int port, const SipProxy::SipProxyMsgCoderInterfacePtr & pSipProxy)
// {
//     SipMessageGateway::SipMessagePtr pMessage = 0;
//     std::string strLocalTag, strRemoteTag, dstUri, callIdHost;
//     int iCseq = 0, iPort = 0, iReqUriType = 0;
//     unsigned char ucMethod = EN_SIP_METHOD_MSG, ucTptType = EN_SIP_TPT_UDP,  ucAddrType = EN_SIP_ADDR_SPEC_SIP;

//     //Set ReqLine DecodePart
//     ReqLinePart reqPart;
//     std::string toUserName, strHostname;

//     sipUriValidCheck(callee, toUserName, strHostname, iPort);
//     reqPart.setReqLinePart(ucMethod, toUserName, strHostname, iPort, iReqUriType);

//     //get To Header
//     FromToPart toPart;
//     toPart.setFromToPart(EN_SIP_ADDR_SPEC_SIP, toUserName, toUserName,strHostname,iPort,strRemoteTag, ucTptType);

//     //get From Header
//     FromToPart fromPart;
//     std::string fromUserName = "Default", strHost = "127.0.0.1";
//     iPort = 0;
//     sipUriValidCheck(caller, fromUserName, strHost, iPort);

//     strLocalTag = SipMsgCommon::createFromTag();
//     fromPart.setFromToPart(EN_SIP_ADDR_SPEC_TEL, fromUserName, fromUserName, strHost,iPort,strLocalTag, ucTptType);

//     //P-Called-Party-ID
//     PCalledPartyID pCalled;
//     pCalled.setInfo(fromUserName, strHost);

//     CallIdPart callIdPart;
//     std::string strCallId = tars::TC_UUIDGenerator::getInstance()->genID();
//     callIdPart.setCallIdPart(strCallId, strHostname);
//     callIdHost = strCallId + strHostname;

//     dstUri = SipMessageGateway::SipMessage::generateDstUri(fromUserName, toUserName, strHostname);
//     pMessage = pSipProxy->getSipMessage(dstUri, callIdHost);
//     if (pMessage)
//     {
//         TLOGWARN("SipMsg", std::string(__FUNCTION__) + "() called. repeated MESSAGE,dstUri:"+ dstUri +",callIdHost:" + callIdHost);
//         return false;
//     }

//     //get CSeq Header
//     CSeqPart cseqPart;
//     cseqPart._iCseqVal = Common::getRand(10000);
//     cseqPart._ucMethodType = ucMethod;

//     //get Contact Header
//     //fromPart.setFromToContact(strHostName, iPort);

//     //get Via Header
//     ViaPart viaPart;
//     std::string strViaUri, strViaBrach, strRecvHost;
//     int iRport = 0;
//     strViaBrach = SipMsgCommon::createBranch();
//     viaPart.setViaPart(strIp, port, ucTptType, false, iRport,strViaBrach);

//     pMessage = SipMessageGateway::SipMessage::create(fromPart._strUserName, toPart._strUserName, dstUri, callIdHost);
//     pMessage->_sipMsgDecodePart._fromPart = fromPart;
//     pMessage->_sipMsgDecodePart._toPart = toPart;
//     pMessage->_sipMsgDecodePart._viaPart = viaPart;
//     pMessage->_sipMsgDecodePart._bodyPart._strBody = msgBody;
//     pMessage->_sipMsgDecodePart._callIdPart = callIdPart;
//     pMessage->_sipMsgDecodePart._cseqPart = cseqPart;
//     pMessage->_sipMsgDecodePart._pCalledPartyID = pCalled;
//     pMessage->_sipMsgDecodePart._reqLinePart = reqPart;

//     //pMessage->_sipMsgDecodePart._contentType.setContentType(mtype, mtypeExt, msubType, msubtypeExt);
//     pMessage->setDirection(false);
//     pMessage->setMsgBody(msgBody);

//     return pMessage;
// }

bool SipMsg::privateIpCheck(const std::string & remoteIp)
{
    if (remoteIp.empty())
        return false;
    int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
    sscanf(remoteIp.c_str(), "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);
    if((ip1 == 10) || (ip1 == 172 && ip2 >= 16 && ip2 <= 31) || (ip1 == 192 && ip2 == 168))
        return true;

    return false;
}
