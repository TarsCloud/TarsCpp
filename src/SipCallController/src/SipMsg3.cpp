#include "SipMsg3.h"
#include "SipGW3Common.h"
#include "util/tc_uuid_generator.h"

bool SipMessage::dealInviteRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                   const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,pSipGateway == NULL! SipMsg:\n" + strBuffer);
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned long  ulStatusCode = 0;
    unsigned char ucTptType, ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true, isPrivate = false;
    SipMsgDecodedPart * pstDecodedPart = 0;
    unsigned int ulRseqNum = 0;
    std::string strDispName, strUserName, strHost, callForwarding;
    int iPort = 0, iReqUriType = 0;
    unsigned char ucAddrType = 0;
    int iCseq = 0;

    ucTptType = EN_SIP_TPT_UDP;
    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag  + "\\" + strRemoteTag;

    //create SipDialogId object
    sptSipDialogId = SipCloudGateway::SipDlgId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,SipDlgId::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipCloudGateway::SipDlg::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, SipDlg::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }

    pstSipDialog->setSipMsg(strBuffer);
    pstSipDialog->setFromTag(strLocalTag);

    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    //Set ReqLine DecodePart
    std::string strUserInfo, strHostname;
    pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);

    //get From Header
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag, ucTptType);
        pstSipDialog->setCallerNumber(strUserName);
        pstSipDialog->setCallerParams(true, ucAddrType, strDispName, strUserName, strHost, iPort);
    }

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
        pstSipDialog->setCalleeNumber(strUserName);
        pstSipDialog->setCallerParams(false, ucAddrType, strDispName, strUserName, strHost, iPort);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;
    std::string callIdHost = strCallId + "@" + strCallHost;
    pstSipDialog->setCallIdHost(callIdHost);

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);
    pstSipDialog->setContactIp(remoteIp);

    std::string routeIp;
    int routePort = 0;
    vector<ST_SIP_ROUTE_PARM> stRouteParams;
    pstSipMsgDe->getHdrRecordRouteParams(stRouteParams);
    if (stRouteParams.size() > 0)
    {
        pstSipDialog->setRecordRoute(true);
        pstSipDialog->_sipMsgDecodePart._recordRoutes._stRouteParams = stRouteParams;

        ST_SIP_ROUTE_PARM params = stRouteParams.at(0);
        ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
        ST_SIP_SIP_URI uri;
        if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
            uri = addr.u.stSipUri;
        else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
            uri = addr.u.stSipsUri;

        std::string userInfo, pwd;
        SipMsgCommon::deSipUri(&uri, userInfo, pwd, routeIp, routePort);
    }
    else
    {
        pstSipMsgDe->getHdrRouteParams(stRouteParams);
        if (stRouteParams.size() > 0)
        {
            pstSipDialog->setRecordRoute(false);
            ST_SIP_ROUTE_PARM params = stRouteParams.at(0);
            ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
            ST_SIP_SIP_URI uri;
            if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                uri = addr.u.stSipUri;
            else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                uri = addr.u.stSipsUri;

            std::string userInfo, pwd, routeAddr;
            SipMsgCommon::deSipUri(&uri, userInfo, pwd, routeAddr, routePort);

            if (pSipGateway->isLocalConn(routeAddr))
            {
                if (stRouteParams.size() > 1)
                {
                    ST_SIP_ROUTE_PARM params = stRouteParams.at(1);
                    ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
                    ST_SIP_SIP_URI uri;
                    if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                        uri = addr.u.stSipUri;
                    else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                        uri = addr.u.stSipsUri;

                    std::string userInfo, pwd;
                    SipMsgCommon::deSipUri(&uri, userInfo, pwd, routeIp, routePort);

                    for (unsigned int i = 1; i < stRouteParams.size(); i++)
                    {
                        pstSipDialog->_sipMsgDecodePart._recordRoutes._stRouteParams.push_back(stRouteParams.at(i));
                    }
                }
            }
            else
            {
                routeIp = routeAddr;
                pstSipDialog->_sipMsgDecodePart._recordRoutes._stRouteParams = stRouteParams;
            }
        }
    }

    //get Contact Header
    std::string strHostName;
    ZCHAR  zcTpt;
    unsigned char addrType;
    iPort = 0;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName, iPort, zcTpt, addrType))
    {
        pstDecodedPart->_toPart.setFromToContact(strUserName, strHostName, iPort, addrType);
    }

    pstSipDialog->setCallerRouteConnectId(getConnId(remoteIp, remotePort, routeIp, routePort, strHostName, iPort));

    //get Via Headers
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
        pstDecodedPart->_viaCalleeHdrList = viaHeaders;
    }

    //get CSeq Header
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iCseqVal = iCseq;
        pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
    }
    pstDecodedPart->_cseqPart._iCalleeCseqVal = 0;

    std::string diversion;
    if (pstSipMsgDe->getHdrExt(SIP_GW_CALL_FORWARDING_DIVERSION, diversion))
    {
        std::string sipuri;
        getDiversionInfo(diversion, sipuri);
        pstSipDialog->setCallForwarding(diversion);
    }

    //get conf id
    std::string confId;
    if (pstSipMsgDe->getHdrExt(SIP_GW_OCS_CONFERENCE_ID, confId))
    {
        pstSipDialog->setMeetingId(confId);
    }

    std::string orinCallId;
    if (pstSipMsgDe->getHdrExt(SIP_GW_ORIGIN_CALL_USER, orinCallId))
    {
        pstSipDialog->setOriginalCallId(orinCallId);
    }

    //get SDP body
    bool existSDP = false;
    std::string strSDPBody, sdpNew, sdpIp;
    vector<std::string> mediaCoding;
    int sdpPort = 0;
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod, bReqPres, ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            GetIpFromSdp(pstSipMsgDe, sdpIp, sdpPort, mediaCoding);
            if(!updateSdpWithIp(strSDPBody, remoteIp, sdpIp, sdpNew))
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, updateSdpWithIp failed! callId:" + strCallId);
                sdpNew = strSDPBody;
            }

            pstSipDialog->setSdpOffer(sdpNew);
            pstSipDialog->setSdpIp(sdpIp);
            pstSipDialog->setMediaCoding(mediaCoding);
            pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, sdpNew);
        }
    }

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_INVITE, pstSipDialog, sipGatewayId, "",sdpNew, params, strBuffer, connectionId);

    return true;
}

bool SipMessage::getDiversionInfo(const std::string & diversion, std::string & sipuri)
{
    int pos1 = diversion.find("<");
    int pos2 = diversion.find(">");
    if (pos1 < 0 || pos2 < 0)
    {
        sipuri = diversion;
        return true;
    }

    std::string uri= diversion.substr(pos1 + 1, pos2 - pos1 - 1);
    int pos3 = uri.find(";");
    if (pos3 > 0)
    {
        sipuri = uri.substr(0, pos3);
    }
    else
    {
        sipuri = uri;
    }

    return true;
}

bool SipMessage::GetIpFromSdp(const SipMsgDecodePtr & pstSipMsgDe, std::string & sdpIp, int &sdpPort, vector<std::string> & vectCoding)
{
    ZUCHAR ucMtype;                  /**< @brief m-type type EN_SIP_MTYPE */
    ZUCHAR ucMSubtype;               /**< @brief m-subtype type EN_SIP_MSUBTYPE */
    ST_SIP_HDR_CONTENT_TYPE *pstContentType = 0;
    ST_SDP_SESS_DESC_LST stSDPSessDesLst;

    ST_ABNF_LIST_NODE *pstSessDescMNode;
    ST_SDP_SESS_DESC *pstSessDesc;
    ZVOID * pstSessDescVoidPtr;
    vector<SDPMediaInfo>  vectSdpMediaInfo;

    //ST_SIP_BODY
    if (!pstSipMsgDe->_pstMsg->stBody.ucPres)
    {
        TLOGWARN("[SipMsg] " << "getSBCMediaIpAndPort（） called. Sip msg Body not exist!");
        return false;
    }

    /* find the content type header */
    pstContentType = (ST_SIP_HDR_CONTENT_TYPE *)Sip_HdrLstFindHdr(&pstSipMsgDe->_pstMsg->stHdrLst, EN_SIP_HDR_CONTENT_TYPE);

    /* if have no content type, so we can't distinguish body type */
    if (!pstContentType)
    {
        TLOGWARN("[SipMsg] " << "getSBCMediaIpAndPort() called! Can't found the Content-type header!");
        return false;
    }

    if (pstContentType != 0)
    {
        ucMtype = pstContentType->stMediaType.ucMtype;
        ucMSubtype = pstContentType->stMediaType.ucMSubtype;
        if (ucMSubtype != EN_SIP_MSUBTYPE_SDP)
        {
            TLOGWARN("[SipMsg] " << "getSBCMediaIpAndPort（） called. Content Sub-Media type is not SDP");
            return false;
        }
    }

    stSDPSessDesLst = pstSipMsgDe->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;

    //sess desc lst circulation
    FOR_ALL_DATA_IN_ABNF_LIST(&stSDPSessDesLst, pstSessDescMNode, pstSessDescVoidPtr)
    {
        pstSessDesc = (ST_SDP_SESS_DESC *)pstSessDescVoidPtr;
        if (pstSessDesc == 0)
        {
            TLOGWARN("[SipMsg] " << "getSBCMediaIpAndPort（） called. sess desc lst circulation converter pointer failed!");
            break;
        }
        SDPMediaInfo sdpMediaInfo;  //new a SDPMediaInfo obj

        //decode C field
        StSdpConnInfo  stSdpConnInfo;
        SipMsgCommon::deSdpConnInfo(pstSessDesc->stConn, stSdpConnInfo);
        sdpMediaInfo._strIp = stSdpConnInfo._strAddr;

        //decode m field
        ST_ABNF_LIST_NODE *pstMDescMNode;
        ST_SDP_MDESC *pstMDesc;
        ZVOID * pstMVoidPtr;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstSessDesc->stMdescLst, pstMDescMNode, pstMVoidPtr)
        {
            pstMDesc = (ST_SDP_MDESC *)pstMVoidPtr;
            if (0 == pstMDesc)
            {
                TLOGWARN("[SipMsg] " << "getSBCMediaIpAndPort（） called. M desc lst circulation converter pointer failed!");
                break;
            }
            SDPMediaPort mediaPortInfo;
            //ST_SDP_MF stMedia
            if (pstMDesc->stMedia.ucMediaType == EN_SDP_MEDIA_AUDIO || pstMDesc->stMedia.ucMediaType == EN_SDP_MEDIA_VIDEO)
            {
                if (sdpMediaInfo._strIp.empty() && pstMDesc->stConnLst.iCount > 0)
                {
                    StSdpConnInfo sdpConnInfo;
                    SipMsgCommon::deSdpConnInfo(*((ST_SDP_CF*)pstMDesc->stConnLst.pstHead->pData), sdpConnInfo);
                    sdpMediaInfo._strIp = sdpConnInfo._strAddr;
                }
                mediaPortInfo._bIsAudio = pstMDesc->stMedia.ucMediaType == EN_SDP_MEDIA_AUDIO ? true : false;
                mediaPortInfo._port = pstMDesc->stMedia.wPort;
                mediaPortInfo._bMediaForbit = mediaPortInfo._port > 0 ? false : true;
            }

            sdpMediaInfo._vecMediaPort.push_back(mediaPortInfo);
            if (pstMDesc->stMedia.ucMediaType == EN_SDP_MEDIA_AUDIO)
            {
                ST_ABNF_LIST_NODE *pstMfNode;
                ST_SDP_FMT *pstFmt;
                ZVOID * pstMfListPtr;
                FOR_ALL_DATA_IN_ABNF_LIST(&pstMDesc->stMedia.stFmtLst, pstMfNode, pstMfListPtr)
                {
                    pstFmt = (ST_SDP_FMT *)pstMfListPtr;
                    if (0 == pstFmt)
                    {
                        TLOGWARN("[SipMsg] " << "getSBCMediaIpAndPort（） called. converter pointer failed! ");
                        break;
                    }
                    std::string sdpCoding;
                    SipMsgCommon::zosSstr2Str(*pstFmt, sdpCoding);
                    vectCoding.push_back(sdpCoding);
                }
            }
        }
        vectSdpMediaInfo.push_back(sdpMediaInfo);
    }

    if (vectSdpMediaInfo.empty())
        return false;

    SDPMediaInfo infos = vectSdpMediaInfo.at(0);
    sdpIp = infos._strIp;
    return true;
}

bool SipMessage::dealInviteResponce(const std::string &strBuffer, int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                    const std::string & sipGatewayId, const std::string & remoteIp, int remotePort, const SipMsgDecodePtr & pstSipMsgDe)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pSipGateway == NULL! ERROR! SipMsg:\n" + strBuffer);
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strDispName, strUserName, strHost, strSDPBody, callIdHost;
    SipMsgDecodedPart * pstDecodedPart = 0;
    SipProxy::SipRegisterSubscribePtr pRegisterSub = 0;
    // ST_SIP_DIGEST stDigest;
    unsigned int ulRseqNum = 0;
    bool existSDP = false, refresherPre = false,  uasRefresh = false;
    int iPort = 0, seconds = 0;
    unsigned char ucTptType = 0, ucAddrType = 0;
    unsigned char ucMethod = EN_SIP_METHOD_INVITE;

    ucTptType = EN_SIP_TPT_UDP;

    //check decode procedure valid
    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);
    callIdHost = strCallId + "@" + strCallHost;

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(callIdHost);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed callIdHost:" + callIdHost + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    ZCHAR zcTpt;
    unsigned char addrType = 0;
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHost, iPort, zcTpt, addrType))
    {
        if (pstSipDialog->getIsFreshDialog())
            pstDecodedPart->_fromPart.setFromToContact(strUserName, strHost, iPort, addrType);
        else
            pstDecodedPart->_toPart.setCalleeFromToContact(strUserName, strHost, iPort, addrType);
    }

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    std::string connectionId1 = pstSipDialog->getSipGatewayConnectId1();
    std::string connectionId3 = pstSipDialog->getCallerRouteConnectId();
    pstSipDialog->setConnectionId(connectionId);

    bool iscaller = (connectionId == connectionId1 || connectionId == connectionId3) ? true : false;
    if (!iscaller)
        pstSipDialog->setCalleeToTag(strRemoteTag);

    if (pstSipDialog->getIsFreshDialog())
    {
        int cseq = 0;
        pstSipMsgDe->getHdrCseq(cseq);
        int refreshCseq = pstSipDialog->getRefreshCseq(iscaller);
        if (cseq < refreshCseq)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, cseq error callIdHost:" + callIdHost + ", curr cseq:" + std::string(cseq) + ", refreshCseq:" + std::string(refreshCseq)+ ", iscaller:" + std::string(iscaller));
            return false;
        }
    }

    //get From Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag, ucTptType);
    }

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName, strUserName, strHost, iPort, strRemoteTag, ucTptType);
    }

    vector<ST_SIP_ROUTE_PARM> stRouteParams;
    pstSipMsgDe->getHdrRecordRouteParams(stRouteParams);
    if (!iscaller && stRouteParams.size() > 0)
    {
        pstSipDialog->_sipMsgDecodePart._calleeRecordRoutes._stRouteParams.clear();
        std::string gw3Ip = pstSipDialog->getGw3Ip();
        std::string gw2Ip = pstSipDialog->getGw2Ip();
        std::string gw2Ip2 = pstSipDialog->getGw2Ip2();
        for (unsigned int i = 0; i < stRouteParams.size(); i++)
        {
            ST_SIP_ROUTE_PARM params = stRouteParams.at(i);
            ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
            ST_SIP_SIP_URI uri;
            if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                uri = addr.u.stSipUri;
            else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                uri = addr.u.stSipsUri;

            std::string userInfo, pwd, host;
            int port = 0;
            SipMsgCommon::deSipUri(&uri, userInfo, pwd, host, port);

            if (host == gw3Ip || host == gw2Ip || host == gw2Ip2)
                break;

            pstSipDialog->_sipMsgDecodePart._calleeRecordRoutes._stRouteParams.push_back(params);
        }
    }

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
    }

    //digest challenge
    if (407 == ulStatusCode || 401 == ulStatusCode)
    {
        pRegisterSub = pstSipDialog->_sipRegisterSub;
        if (pRegisterSub != 0)
        {
            ST_SIP_DIGEST stDigest;
            if (pstSipMsgDe->getDigest(stDigest, ulStatusCode))
            {
                pRegisterSub->setDigest(stDigest);
                pstSipMsgDe->decodeChallengeDigest(stDigest, pRegisterSub);
            }
        }
    }

    if (ulStatusCode == 200)
    {
        std::string cmicovs;
        if (pstSipMsgDe->getHdrExt(SIP_GW_SERVICE_CMICOVS_INFO, cmicovs))
            pstSipDialog->setCmiCovsInfo(cmicovs);
    }

    //get SDP body
    std::string sdpNew;
    if (pstSipMsgDe->checkSipMsgWithSDP(EN_SIP_METHOD_INVITE,  false,  ulStatusCode, existSDP, strSDPBody))
    {
        if (existSDP)
        {
            if (ulStatusCode == 183 || ulStatusCode == 180 || ulStatusCode == 200)
            {
                std::string sdpIp;
                vector<std::string> mediaCoding;
                int sdpPort = 0;
                GetIpFromSdp(pstSipMsgDe, sdpIp, sdpPort, mediaCoding);
                pstSipDialog->setMediaCoding(mediaCoding);
                if(mediaCoding.size() > 0)
                {
                    std::string coding = mediaCoding.at(0);
                    pstSipDialog->setMrfpMediaCoding(coding.toInt(0));
                }

                if(!updateSdpWithIp(strSDPBody, remoteIp, sdpIp, sdpNew))
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, updateSdpWithIp failed! callId:" + strCallId);
                    sdpNew = strSDPBody;
                }
                pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, sdpNew);
            }
        }
    }

    if (ulStatusCode > 400)
    {
        std::string text;
        unsigned int protoType = 0, cause = 0;
        if(pstSipMsgDe->getRspReason(text, protoType, cause))
        {
            if(text == SipCloudGateway::SIP_ACCOUNT_CHARGE_KEY_BLACKLIST_TYPE_GLOBAL)
                pstSipDialog->setReleaseType(SipCloudGateway::EN_CALL_RELEASE_BY_BLACK_LIST_GLOBAL);
            else if (text == SipCloudGateway::SIP_ACCOUNT_CHARGE_KEY_BLACKLIST_TYPE_USER)
                pstSipDialog->setReleaseType(SipCloudGateway::EN_CALL_RELEASE_BY_BLACK_LIST_USER);
        }
    }

    pSipGateway->onSipMessageResponse(ulStatusCode, EN_SIP_METHOD_INVITE, pstSipDialog, sipGatewayId, sdpNew, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealAckRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    int iPort = 0;

    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pSipGateway == NULL! SipMsg:\n" + strBuffer);
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    ucTptType = EN_SIP_TPT_UDP;

    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //别的方法，比如ACK 不需要重新创建对话框 SipDialog
    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        //异常流程处理，可能导致清理掉对话后，ACK才返回
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed strSipDialogId:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    SipMsgDecodedPart * pstDecodedPart = 0;
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    //get From Header
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
        pstDecodedPart->_viaCalleeHdrList = viaHeaders;
    }

    bool sdpPres = false;
    std::string sdp, sdpNew, sdpIp;
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod, true, 0, sdpPres, sdp))
    {
        if (!sdp.empty())
        {
            vector<std::string> mediaCoding;
            int sdpPort = 0;
            GetIpFromSdp(pstSipMsgDe, sdpIp, sdpPort, mediaCoding);
            if(!updateSdpWithIp(sdp, remoteIp, sdpIp, sdpNew))
                sdpNew = sdp;
        }
    }
    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_ACK, pstSipDialog, sipGatewayId, "", sdpNew, params, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealBYERequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  pSipGateway == NULL! SipMsg:\n" + strBuffer);
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    SipMsgDecodedPart * pstDecodedPart = 0;
    int iPort = 0, iReqUriType = 0;
    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    std::string strViaUri, strViaBrach, strRecvHost;
    std::string strHostName;
    ucTptType = EN_SIP_TPT_UDP;
    std::string callIdHost;

    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  inner error. SipMsg:\n" + strBuffer);
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
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //别的方法，比如ACK 不需要重新创建对话框 SipDialog
    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        //RFC 3261 page 82;  UAS端记录本端的localTag 等对话状态信息，与UAC 端相反，在对话内再发起请求时，此时UAS的local Tag即为 UAC的remote Tag，From的Tag会填为UAC 的To中的Tag，
        //用Winsip测试时发现，RPC 呼叫WinSip，然后WinSip结束，发BYE，tag与之前的发出的INVITE相反
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(callIdHost);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed callIdHost:" + callIdHost + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    //get From Header
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
    }

    TLOGINFO("[SipMsg] " << std::string(__FUNCTION__) + "() callled. origin caller cseq:" + std::string((int)pstDecodedPart->_cseqPart._iCseqVal) + ",callee cseq:" +  std::string((int)pstDecodedPart->_cseqPart._iCalleeCseqVal));

    //get CSeq Header
    int iCseq = 0;
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        std::string auth = pstSipDialog->_sipRegisterSub->getAuthName();
        if (!auth.empty())
        {
            std::string connectionId1 = pstSipDialog->getSipGatewayConnectId1();
            std::string connectionId3 = pstSipDialog->getCallerRouteConnectId();
            connectionId3 = connectionId3.empty() ? connectionId1 : connectionId3;
            bool isCaller = (connectionId == connectionId1 || connectionId == connectionId3) ? true : false;
            if (isCaller)
            {
                pstDecodedPart->_cseqPart._iCseqVal = iCseq;
            }
            else
            {
                pstDecodedPart->_cseqPart._iCalleeCseqVal = iCseq;
            }
        }
    }

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_BYE, pstSipDialog, sipGatewayId, "", "", params, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealBYEResponce(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                 const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    SipMsgDecodedPart * pstDecodedPart = 0;

    ucTptType = EN_SIP_TPT_UDP;

    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //get SipDialog
    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed strSipDialogId:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    std::string strDispName, strUserName, strHost;
    unsigned char ucAddrType = 0;
    int iPort = 0;
    //get From Header
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
    }

    pSipGateway->onSipMessageResponse(ulStatusCode, EN_SIP_METHOD_BYE, pstSipDialog,  sipGatewayId, "", strBuffer, connectionId);

    return true;
}

bool SipMessage::dealCANCELRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                   const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,. pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true;

    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        //RFC 3261 page 82;  UAS端记录本端的localTag 等对话状态信息，与UAC 端相反，在对话内再发起请求时，此时UAS的local Tag即为 UAC的remote Tag，From的Tag会填为UAC 的To中的Tag，
        //用Winsip测试时发现，RPC 呼叫WinSip，然后WinSip结束，发BYE，tag与之前的发出的INVITE相反
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed strSipDialogId:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    //get Via Headers
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstSipDialog->_sipMsgDecodePart._viaHdrList = viaHeaders;
    }

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_CANCEL, pstSipDialog, sipGatewayId, "", "", params, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealCANCELResponce(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                    const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned int ulStatusCode = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;

    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error. SipMsg:\n" + strBuffer);
        return false;
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " <<std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed strSipDialogId:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstSipDialog->_sipMsgDecodePart._viaHdrList = viaHeaders;
    }

    pSipGateway->onSipMessageResponse(ulStatusCode, EN_SIP_METHOD_CANCEL, pstSipDialog, sipGatewayId, "", strBuffer, connectionId);

    return true;
}

bool SipMessage::dealUPDATERequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                   const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;
    bool bReqPres = true;
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
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //create SipDialogId object
    sptSipDialogId = SipCloudGateway::SipDlgId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, SipDlgId::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipCloudGateway::SipDlg::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("[SipMsg] " <<std::string(__FUNCTION__)+"() called, SipDlg::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    //Set ReqLine DecodePart
    pstSipMsgDe->getReqUri(strUserInfo, strHostname, iPort, iReqUriType);
    pstDecodedPart->_reqLinePart.setReqLinePart(ucMethod, strUserInfo, strHostname, iPort, iReqUriType);

    //get From Header
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName, strUserName, strHost, iPort, strLocalTag, ucTptType);
    }

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName, strUserName, strHost,iPort, strRemoteTag, ucTptType);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;
    std::string callIdHost = strCallId + "@" + strCallHost;
    pstSipDialog->setCallIdHost(callIdHost);

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
    }

    ZCHAR  zcTpt;
    unsigned char addrType;
    iPort = 0;
    std::string strHostName;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName, iPort, zcTpt, addrType))
    {
        pstDecodedPart->_toPart.setFromToContact(strUserName, strHostName, iPort, addrType);
    }

    int iCseq = 0;
    //get CSeq Header
    if (pstSipMsgDe->getHdrCseq(iCseq))
    {
        pstDecodedPart->_cseqPart._iCseqVal = iCseq;
        pstDecodedPart->_cseqPart._ucMethodType = ucMethod;
    }
    pstDecodedPart->_cseqPart._iCalleeCseqVal = 0;

    //get SDP body
    bool existSDP = false;
    std::string strSDPBody, sdpNew;
    vector<std::string> mediaCoding;
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod, bReqPres,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            sdpNew = strSDPBody;
            std::string sdpIp;
            int sdpPort = 0;
            GetIpFromSdp(pstSipMsgDe, sdpIp, sdpPort, mediaCoding);
            if(!updateSdpWithIp(strSDPBody, remoteIp, sdpIp, sdpNew))
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, updateSdpWithIp failed! callId:" + strCallId);
                sdpNew = strSDPBody;
            }

            pstSipDialog->setSdpOffer(sdpNew);
            pstSipDialog->setSdpIp(sdpIp);
            pstSipDialog->setMediaCoding(mediaCoding);
            pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, sdpNew);
        }
    }

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_UPDATE, pstSipDialog, sipGatewayId, "", sdpNew, params, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealUPDATEResponse(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                    const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pSipGateway == NULL! ERROR! SipMsg:\n" + strBuffer);
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost, strSDPBody;
    bool existSDP = false;
    unsigned char ucTptType = 0, ucAddrType = 0;
    unsigned char ucMethod = EN_SIP_METHOD_UPDATE;

    ucTptType = EN_SIP_TPT_UDP;
    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag+ "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        std::string strSipDialogIdTo = strCallId + "@" + strCallHost + "\\" + strRemoteTag + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogIdTo);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed strSipDialogId:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    std::string strDispName, strUserName, strHost;
    int iPort = 0;

    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstSipDialog->_sipMsgDecodePart._fromPart.setFromToPart(ucAddrType, strDispName, strUserName, strHost, iPort, strLocalTag, ucTptType);
    }

    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstSipDialog->_sipMsgDecodePart._toPart.setFromToPart(ucAddrType, strDispName, strUserName, strHost,iPort, strRemoteTag, ucTptType);
    }

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstSipDialog->_sipMsgDecodePart._viaHdrList = viaHeaders;
    }

    //get SDP body
    std::string sdpNew;
    if (pstSipMsgDe->checkSipMsgWithSDP(ucMethod,  false,  ulStatusCode, existSDP, strSDPBody))
    {
        if (!strSDPBody.empty())
        {
            std::string strSDPBody, sdpIp;
            vector<std::string> mediaCoding;
            int sdpPort = 0;
            GetIpFromSdp(pstSipMsgDe, sdpIp, sdpPort, mediaCoding);
            if(!updateSdpWithIp(strSDPBody, remoteIp, sdpIp, sdpNew))
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, updateSdpWithIp failed! callId:" + strCallId);
                sdpNew = strSDPBody;
            }
            pstSipDialog->_sipMsgDecodePart._bodyPart.setBodyPart(true, 0, EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, sdpNew);
        }
    }

    pSipGateway->onSipMessageResponse(ulStatusCode, EN_SIP_METHOD_UPDATE, pstSipDialog, sipGatewayId, sdpNew, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealPRACKRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                  const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, dealPRACKRequest pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;

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
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. inner error,  SipMsg:\n" + strBuffer);
        return false;
    }
    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed strSipDialogId:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    //get From Header
    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    // get Call-Id Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
    }

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_PRACK, pstSipDialog, sipGatewayId, "", "", params, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealPRACKResponce(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                   const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    unsigned int ulStatusCode = 0;
    unsigned char ucMethod;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;

    if (!pstSipMsgDe)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    pstSipMsgDe->getRspStatusCode(ulStatusCode);

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed callIdHost:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstSipDialog->_sipMsgDecodePart._viaHdrList = viaHeaders;
    }

    pSipGateway->onSipMessageResponse(ulStatusCode, EN_SIP_METHOD_PRACK,pstSipDialog, sipGatewayId, "", strBuffer, connectionId);

    return true;
}

bool SipMessage::dealINFORequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                 const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. dealINFORequest pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;
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
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    std::string strSipDialogId = strCallId + "@" + strCallHost + "\\" + strLocalTag + "\\";//初始存储的时候没有 To头域的Tag，所以获取的时候也不要
    pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
    if (!pstSipDialog)
    {
        strSipDialogId = strCallId + "@" + strCallHost + "\\" + strRemoteTag  + "\\";
        pstSipDialog = pSipGateway->getSipDlgPtr2(strSipDialogId);
        if (!pstSipDialog)
        {
            strSipDialogId = strCallId + "@" + strCallHost;
            pstSipDialog = pSipGateway->getSipDlgPtrByCallId(strSipDialogId);
            if (!pstSipDialog)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getSipDlgPtr2() failed strSipDialogId:" + strSipDialogId + ", strLocalTag:" + strLocalTag + ", strRemoteTag:" + strRemoteTag + " SipMsg:\n" + strBuffer);
                return false;
            }
        }
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);

    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    //get Via Header
    vector<ViaPart> viaHeaders;
    if (pstSipMsgDe->getHdrVia(viaHeaders))
    {
        pstDecodedPart->_viaHdrList = viaHeaders;
    }

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_INFO, pstSipDialog, sipGatewayId, "", "", params, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealRegisterRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                 const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. dealINFORequest pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;
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
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //create SipDialogId object
    sptSipDialogId = SipCloudGateway::SipDlgId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("[SipMsg] " <<std::string(__FUNCTION__) + "() call. SipDlgId::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipCloudGateway::SipDlg::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. SipDlg::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;
    pstSipDialog->setSipMsg(strBuffer);

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);
    pstSipDialog->setSipGatewayConnectId1(connectionId);

    //get Contact Header
    std::string strHostName;
    ZCHAR  zcTpt;
    unsigned char addrType;
    iPort = 0;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName, iPort, zcTpt, addrType))
    {
        pstDecodedPart->_toPart.setFromToContact(strUserName, strHostName, iPort, addrType);
        bool isPrivate = pSipGateway->privateIpCheck(strHostName);
        if (!isPrivate)
        {
            iPort = iPort ? iPort : 5060;
            connectionId = strHostName + ":" + std::string(iPort);
        }
    }
    pstSipDialog->setCallerRouteConnectId(connectionId);

    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;
    std::string callIdHost = strCallId + "@" + strCallHost;
    pstSipDialog->setCallIdHost(callIdHost);

    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }
    pstDecodedPart->_viaPart.setViaPart(strViaUri, iPort,  ucTptType, false, iRport,strViaBrach);

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_REGISTER, pstSipDialog, sipGatewayId, "", "", params, strBuffer, connectionId);

    return true;
}

bool SipMessage::dealOptionsRequest(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                     const std::string & sipGatewayId, const SipMsgDecodePtr & pstSipMsgDe, const std::string & remoteIp, int remotePort, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. dealINFORequest pSipGateway == NULL! ERROR!");
        return false;
    }

    SipCloudGateway::SipDlgPtr pstSipDialog = 0;
    SipCloudGateway::SipDlgIdPtr sptSipDialogId = 0;
    unsigned char ucTptType;
    unsigned char ucMethod;
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
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. inner error, SipMsg:\n" + strBuffer);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }

    //create SipDialogId object
    sptSipDialogId = SipCloudGateway::SipDlgId::create(strCallId + "@" + strCallHost , strLocalTag, strRemoteTag);
    if (0 == sptSipDialogId)
    {
        TLOGWARN("[SipMsg] " <<std::string(__FUNCTION__) + "() call. SipDlgId::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }

    //create SipDialog object
    pstSipDialog = SipCloudGateway::SipDlg::create(sptSipDialogId);
    if (0 == pstSipDialog)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() call. SipDlg::create() function call failed! SipMsg:\n" + strBuffer);
        return false;
    }
    pstDecodedPart = &pstSipDialog->_sipMsgDecodePart;

    std::string connectionId = remoteIp + ":" + std::string(remotePort);
    pstSipDialog->setConnectionId(connectionId);
    pstSipDialog->setSipGatewayConnectId1(connectionId);

    //get Contact Header
    std::string strHostName;
    ZCHAR  zcTpt;
    unsigned char addrType;
    iPort = 0;
    if (pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName, iPort, zcTpt, addrType))
    {
        pstDecodedPart->_toPart.setFromToContact(strUserName, strHostName, iPort, addrType);
        bool isPrivate = pSipGateway->privateIpCheck(strHostName);
        if (!isPrivate)
        {
            iPort = iPort ? iPort : 5060;
            connectionId = strHostName + ":" + std::string(iPort);
        }
    }

    pstSipDialog->setCallerRouteConnectId(connectionId);

    if(pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_fromPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strLocalTag,ucTptType);
    }

    //get To Header
    strDispName = ""; strUserName = ""; strHost = ""; iPort = 0; ucAddrType = 0;
    if(pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucAddrType))
    {
        pstDecodedPart->_toPart.setFromToPart(ucAddrType, strDispName,strUserName,strHost,iPort,strRemoteTag,ucTptType);
    }

    // get Call-Id Header
    pstDecodedPart->_callIdPart._strNumber = strCallId;
    pstDecodedPart->_callIdPart._strHost = strCallHost;
    std::string callIdHost = strCallId + "@" + strCallHost;
    pstSipDialog->setCallIdHost(callIdHost);

    if (pstSipMsgDe->getViaBranch(strViaBrach))
    {
        pstDecodedPart->_viaPart._strBranch = strViaBrach;
    }
    pstDecodedPart->_viaPart.setViaPart(strViaUri, iPort, ucTptType, false, iRport,strViaBrach);

    pSipGateway->onSipMessageRequest(EN_SIP_METHOD_OPTIONS, pstSipDialog, sipGatewayId, "", "", params, strBuffer, connectionId);

    return true;
}

SipGateway::SipMsgDecodeResultType SipMessage::onReceiveSipMsgFromCloud(const std::string & strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr &pCloudSipGateway, void* pSipConnection, const std::string & remoteIp, int remotePort)
{
    std::map<std::string, std::string> params;
    bool bReqPres = true;
    bool bInitInvite = false;
    bool result = true;
    unsigned char ucMethod;

    if (0 == pCloudSipGateway)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() called. pCloudSipGateway == NULL! ERROR!");
        return SipGateway::kSipMsgDecodeFailed;
    }

    if (0 == pSipConnection)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() called. pSipConnection == NULL! ERROR!");
        return SipGateway::kSipMsgDecodeFailed;
    }

    if (strBuffer == "hello")
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() called. Msg:\n" + strBuffer);
        return SipGateway::kSipMsgDecodeSuccessed;
    }

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() called. Sip msg is not valid! Msg:\n" + strBuffer);
        return SipGateway::kSipMsgDecodeFailed;
    }
    else if (ucResult == EnSipMsgType_Incomplete)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() called. Sip msg is not complete! Msg:\n" + strBuffer);
        if (false == sipMessageValidCheck(strBuffer))
            return SipGateway::kSipMsgDecodeFailed;
        return SipGateway::kSipMsgDecodeToSave;
    }
    else if (ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        bool bPing = ucResult == EnSipMsgType_PingRFC5626 ? true : false;
        return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
    }

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() called. getMethod failed. Msg:\n" + strBuffer);
        return SipGateway::kSipMsgDecodeFailed;
    }

    bReqPres = pstSipMsgDe->isRequest() ? true : false;
    switch(ucMethod)
    {
    case EN_SIP_METHOD_INVITE:
        if (!bReqPres)
        {
            result = dealInviteResponce(strBuffer, strBuffer.size(),  pCloudSipGateway, "", remoteIp, remotePort, pstSipMsgDe);
        }
        else
        {
            result = dealInviteRequest(strBuffer, strBuffer.size(),  pCloudSipGateway,  "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        break;
    case EN_SIP_METHOD_ACK:
        result = dealAckRequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        break;

    case EN_SIP_METHOD_BYE:
        if (bReqPres)
        {
            result = dealBYERequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            result = dealBYEResponce(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort);
        }
        break;

    case EN_SIP_METHOD_CANCEL:
        if (bReqPres)
        {
            result = dealCANCELRequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            result = dealCANCELResponce(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort);
        }
        break;

    case EN_SIP_METHOD_UPDATE:
        if (bReqPres)
        {
            result = dealUPDATERequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            result = dealUPDATEResponse(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort);
        }

        break;

    case EN_SIP_METHOD_PRACK:
        if (bReqPres)
        {
            result = dealPRACKRequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            result = dealPRACKResponce(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort);
        }
        break;

    case EN_SIP_METHOD_INFO:
        if (bReqPres)
        {
            result = dealINFORequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  INFO Response NOT supported.SipMsg:\n" + strBuffer);
            result = SipGateway::kSipMsgDecodeFailed;
        }
        break;

    case EN_SIP_METHOD_REGISTER:
        if (bReqPres)
        {
            result = dealRegisterRequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  INFO Response NOT supported.SipMsg:\n" + strBuffer);
            result = SipGateway::kSipMsgDecodeFailed;
        }
        break;

    case EN_SIP_METHOD_OPTIONS:
        if (bReqPres)
        {
            result = dealOptionsRequest(strBuffer, strBuffer.size(), pCloudSipGateway, "", pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  INFO Response NOT supported.SipMsg:\n" + strBuffer);
            result = SipGateway::kSipMsgDecodeFailed;
        }
        break;

    default:
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, this method is NOT supported.SipMsg:\n" + strBuffer);
            result = SipGateway::kSipMsgDecodeFailed;
        }
    }

    return result ? SipGateway::kSipMsgDecodeSuccessed : SipGateway::kSipMsgDecodeFailed;
}

//send ..................
bool SipMessage::SendRequestINVITE(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg ,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;
    std::string gwIp;
    unsigned char ucTptType = 0;
    int iPort = 0, gwPort = 0;
    bool bResult = false, isCaller;

    getSipGatewayIpPort(dlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    //check input parameter
    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;

    std::string userName = updateSipMsgHead(isCaller, dlg, pstSipMsgDe, reqMethod, true, true);

    ucTptType = pstDecodedPart->_viaPart._ucTptType;

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_CONTACT))
    {
        bResult = pstSipMsgDe->fillHdrContact2(userName, gwIp, gwPort, ucTptType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed, gwIp:" + gwIp + ", port:" + std::string(gwPort));
            return false;
        }
    }

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_VIA))
    {
        std::string strBranch = dlg->getBranch();
        bResult = pstSipMsgDe->fillHdrVia2(ucTptType, gwIp, gwPort, false, 0, strBranch, "", 0);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
            return false;
        }
        for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
        {
            ViaPart via = pstDecodedPart->_viaHdrList.at(i);
            bResult = pstSipMsgDe->fillHdrVia3(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
        }
    }

    if (dlg->getIsFreshDialog())
    {
        std::string gw3Ip = dlg->getGw3Ip();
        std::string gw2Ip = dlg->getGw2Ip();
        std::string gw2Ip2 = dlg->getGw2Ip2();
        pstSipMsgDe->delHdrRoute(gw3Ip, gw2Ip, gw2Ip2);
    }
    else
    {
        vector<ST_SIP_ROUTE_PARM> stRecordRouteParams;
        pstSipMsgDe->getHdrRecordRouteParams(stRecordRouteParams);
        if (stRecordRouteParams.size() > 0)
        {
            if(dlg->getRecordRoute())
            {
                if(pstSipMsgDe->delHdrById(EN_SIP_HDR_RECORD_ROUTE))
                {
                    bResult = pstSipMsgDe->fillHdrRecordRoute("", gwIp, gwPort, ucTptType);
                    if (!bResult)
                    {
                        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrRecordRoute() failed");
                        return false;
                    }

                    for (unsigned int i = 0; i < stRecordRouteParams.size(); i++)
                    {
                        pstSipMsgDe->fillHdrRecordRoute(stRecordRouteParams.at(i));
                    }
                }
            }
            else
            {
                std::string gw3Ip = dlg->getGw3Ip();
                std::string gw2Ip = dlg->getGw2Ip();
                std::string gw2Ip2 = dlg->getGw2Ip2();
                pstSipMsgDe->delHdrRoute(gw3Ip, gw2Ip, gw2Ip2);
            }
        }
        else
        {
            vector<ST_SIP_ROUTE_PARM> stRouteParams;
            pstSipMsgDe->getHdrRouteParams(stRouteParams);
            if (stRouteParams.size() > 0)
            {
                ST_SIP_ROUTE_PARM params = stRouteParams.at(0);
                ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
                ST_SIP_SIP_URI uri;
                if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                    uri = addr.u.stSipUri;
                else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                    uri = addr.u.stSipsUri;

                std::string userInfo, pwd, routeAddr;
                int routePort = 0;
                SipMsgCommon::deSipUri(&uri, userInfo, pwd, routeAddr, routePort);
                if (pSipGateway->isLocalConn(routeAddr))
                {
                    pstSipMsgDe->delHdrRoute(routeAddr, routeAddr, routeAddr);
                }
            }

            bResult = pstSipMsgDe->fillHdrRecordRoute("", gwIp, gwPort, ucTptType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrRecordRoute() failed");
                return false;
            }
        }
    }

    pstSipMsgDe->delHdrById(EN_SIP_HDR_PRIVACY);
    pstSipMsgDe->delHdrById(EN_SIP_HDR_P_ASSERTED_ID);
    pstSipMsgDe->delHdrById(EN_SIP_HDR_P_PREFERRED_ID);

    userName = dlg->getPAIName();
    if(!userName.empty() && !dlg->getIsFreshDialog())
    {
        pstSipMsgDe->fillHdrExt("P-Asserted-Identity", userName);
    }

    std::string dstUser = dlg->getMiddleCalleeNumber();
    if (!dstUser.empty())
    {
        pstSipMsgDe->fillHdrExt(SIP_GW_DST_CALL_USER, dstUser);
    }
    if(!pstSipMsgDe->getHdrById(EN_SIP_HDR_SUPTED))
    {
        pstSipMsgDe->fillHdrExt("Supported", "100rel,join,norefersub");
    }

    //* set the body message */
    if (pstSipMsgDe->_pstMsg->stBody.ucPres)
    {
        ST_ZOS_SSTR stBodyStr;
        std::string strBody = isCaller ? dlg->getSdpOffer() : dlg->getSdpAnswer();
        SipMsgCommon::str2ZosSstr(strBody, stBodyStr);
        pstSipMsgDe->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
    }

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }
    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg, pstSipMsgDe->_strMsg.size(), true, reqMethod, dlg, recvConnId);
}

bool SipMessage::SendRequestINVITEWithAuth(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg, SipGateway::AuthType authType, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    if (dlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;
    unsigned char ucTptType = 0;
    int iPort = 0, gwPort = 0;
    bool bResult = false;

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    std::string origMsg = dlg->getSipMsg();
    unsigned char ucResult = pstSipMsgDe->decode(origMsg, origMsg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    std::string gwIp = dlg->getGw2Ip2();
    gwPort = dlg->getGw2Port2();

    unsigned char dstType;

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;

    int fromType = 0, fromPort = 0, jegoFromType = 0, jegoFromPort = 0;
    int toType = 0, toPort = 0, jegoToType = 0, jegoToPort = 0;

    std::string fromDispalyName, fromUserName, fromHost;
    std::string toDispalyName, toUserName, toHost;

    std::string jegoFromDispalyName, jegoFromUserName, jegoFromHost;
    std::string jegoToDispalyName, jegoToUserName, jegoToHost;

    dlg->getCallerParams(true, fromType, fromDispalyName, fromUserName, fromHost, fromPort);
    dlg->getCallerParams(false, toType, toDispalyName, toUserName, toHost, toPort);
    dlg->getCallerParamsByJego(true, jegoFromType, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort);
    dlg->getCallerParamsByJego(false, jegoToType, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort);

    SipCloudGateway::EnterpriseType enterType = dlg->getCalleeEnterpriseType();
    dstType = jegoToType;

    pstSipMsgDe->updateReqLine(dstType, reqMethod, jegoToUserName, jegoToHost, jegoToPort);

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
    {
        bResult = pstSipMsgDe->fillHdrFromToByName(false, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort, "", dstType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            return false;
        }
    }

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
    {
        dstType = jegoFromType;

        bResult = pstSipMsgDe->fillHdrFromToByName(true, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort, pFromPart->_strTag, dstType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            return false;
        }
    }

    ucTptType = pstDecodedPart->_viaPart._ucTptType;
    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_CONTACT))
    {
        bResult = pstSipMsgDe->fillHdrContact2(jegoFromUserName, gwIp, gwPort, ucTptType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed, gwIp:" + gwIp + ", port:" + std::string(gwPort));
            return false;
        }
    }

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_VIA))
    {
        for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
        {
            ViaPart via = pstDecodedPart->_viaHdrList.at(i);
            bResult = pstSipMsgDe->fillHdrVia3(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
        }
    }

    pstSipMsgDe->delHdrById(EN_SIP_HDR_PRIVACY);
    pstSipMsgDe->delHdrById(EN_SIP_HDR_P_ASSERTED_ID);
    pstSipMsgDe->delHdrById(EN_SIP_HDR_P_PREFERRED_ID);

    pstDecodedPart->_cseqPart._iCalleeCseqVal++;

    pstSipMsgDe->delHdrById(EN_SIP_HDR_CSEQ);
    pstSipMsgDe->fillHdrCseq(reqMethod, pstDecodedPart->_cseqPart._iCalleeCseqVal);

    std::string userName = dlg->getPAIName();
    if(!userName.empty())
    {
        pstSipMsgDe->fillHdrExt("P-Asserted-Identity", userName);
    }

    vector<ST_SIP_ROUTE_PARM> stRouteParams;
    pstSipMsgDe->getHdrRecordRouteParams(stRouteParams);
    if (stRouteParams.size() > 0)
    {
        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_RECORD_ROUTE))
        {
            bResult = pstSipMsgDe->fillHdrRecordRoute("", gwIp, gwPort, ucTptType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrRecordRoute() failed");
                return false;
            }

            for (unsigned int i = 0; i < stRouteParams.size(); i++)
            {
                ST_SIP_ROUTE_PARM params = stRouteParams.at(i);
                pstSipMsgDe->fillHdrRecordRoute(params);
            }
        }
    }
    else
    {
        vector<ST_SIP_ROUTE_PARM> stRouteParams;
        pstSipMsgDe->getHdrRouteParams(stRouteParams);
        if (stRouteParams.size() > 0)
        {
            ST_SIP_ROUTE_PARM params = stRouteParams.at(0);
            ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
            ST_SIP_SIP_URI uri;
            if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                uri = addr.u.stSipUri;
            else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                uri = addr.u.stSipsUri;

            std::string userInfo, pwd, routeAddr;
            int routePort = 0;
            SipMsgCommon::deSipUri(&uri, userInfo, pwd, routeAddr, routePort);
            if (pSipGateway->isLocalConn(routeAddr))
            {
                pstSipMsgDe->delHdrRoute(routeAddr, routeAddr, routeAddr);
            }
        }

        bResult = pstSipMsgDe->fillHdrRecordRoute("", gwIp, gwPort, ucTptType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrRecordRoute() failed");
            return false;
        }
    }

    ST_SIP_REQ_URI stReqUri = pstSipMsgDe->_pstMsg->u.stReqLine.stReqUri;
    ST_SIP_DIGEST_RSP stRsp;
    ABNF_LIST_CREATE(&stRsp.stDigRspLst);
    /* calculate md5 */
    authCalcRsp(pstSipMsgDe, dlg->_sipRegisterSub, reqMethod, 0, &stReqUri, &stRsp);
    int stateCode = authType == SipGateway::Auth401 ? 401 : 407;
    /* fill authorization header */
    pstSipMsgDe->fillHdrAuthor(stRsp, stateCode);

    //nonce count + 1
    dlg->_sipRegisterSub->setDigestNonceCount(dlg->_sipRegisterSub->getDigestNonceCount() + 1);

    if (pstSipMsgDe->_pstMsg->stBody.ucPres)
    {
        ST_ZOS_SSTR stBodyStr;
        std::string strBody = dlg->getSdpOffer();
        SipMsgCommon::str2ZosSstr(strBody, stBodyStr);
        pstSipMsgDe->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
    }

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg, pstSipMsgDe->_strMsg.size(), true, reqMethod, dlg, recvConnId, true);
}

bool SipMessage::SendRequestACK(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;

    unsigned char ucTptType;
    ucTptType = EN_SIP_TPT_UDP;

    std::string gwIp;
    int gwPort = 0;
    bool isCaller;
    getSipGatewayIpPort(dlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    ////check input parameter
    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    ucTptType = pstDecodedPart->_viaPart._ucTptType;

    updateSipMsgHead(isCaller, dlg, pstSipMsgDe, reqMethod, true, false, false);

    pstSipMsgDe->delHdrById(EN_SIP_HDR_CONTACT);
    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_VIA))
    {
        std::string strBranch = dlg->getBranch();
        bool bResult = pstSipMsgDe->fillHdrVia2(ucTptType, gwIp, gwPort, false, 0, strBranch, "", 0);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
            return false;
        }
        for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
        {
            ViaPart via = pstDecodedPart->_viaHdrList.at(i);
            bResult = pstSipMsgDe->fillHdrVia3(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
        }
    }

    std::string gw3Ip = dlg->getGw3Ip();
    std::string gw2Ip = dlg->getGw2Ip();
    std::string gw2Ip2 = dlg->getGw2Ip2();

    pstSipMsgDe->delHdrRoute(gw3Ip, gw2Ip, gw2Ip2);

    //* set the body message */
    if (pstSipMsgDe->_pstMsg->stBody.ucPres)
    {
        ST_ZOS_SSTR stBodyStr;
        std::string strBody = isCaller ? dlg->getSdpOffer() : dlg->getSdpAnswer();
        SipMsgCommon::str2ZosSstr(strBody,stBodyStr);
        pstSipMsgDe->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
    }

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg, pstSipMsgDe->_strMsg.size(), true, reqMethod, dlg, recvConnId);
}

bool SipMessage::SendRequestACK2(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    SipMsgEncodePtr pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. createReq failed .sipDialog id:" + dlg->id());
        return false;
    }

    bool isCaller = false;
    std::string gwIp;
    int gwPort = 0;
    std::string connectId1 = dlg->getSipGatewayConnectId1();
    std::string connectId3 = dlg->getCallerRouteConnectId();
    connectId3 = connectId3.empty() ? connectId1 : connectId3;
    if (recvConnId == connectId1 || recvConnId == connectId3)
    {
        gwIp = dlg->getGw2Ip();
        gwPort = dlg->getGw2Port();
        isCaller = true;
    }
    else
    {
        gwIp = dlg->getGw2Ip2();
        gwPort = dlg->getGw2Port2();
    }

    //get Contact Header
    std::string strDispName, strUserName, strHostName;
    ZCHAR  zcTpt;
    unsigned char addrType;
    int iPort = 0;
    pstSipMsgDe->getHdrContactSipUri(strUserName, strHostName, iPort, zcTpt, addrType);

    if (strHostName.empty())
    {
        strHostName = "", iPort = 0;
        if(!pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHostName, iPort, addrType))
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call getHdrFromToUri() failed");
            return false;
        }

        pstSipMsgEn->fillReqLine(addrType, EN_SIP_METHOD_ACK, strUserName, strHostName, iPort);
    }
    else
        pstSipMsgEn->fillReqLine(addrType, EN_SIP_METHOD_ACK, strUserName, strHostName, iPort);

    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_FROM);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_TO);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    if(!pstSipMsgDe->getHdrFromToUri(true, strDispName, strUserName, strHostName, iPort, addrType))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call getHdrFromToUri() failed");
        return false;
    }

    bool bResult = pstSipMsgEn->fillHdrVia(pstDecodedPart->_viaPart._ucTptType, gwIp, gwPort, false, 0, dlg->getBranch());
    if (!bResult)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
        return false;
    }

    //fill route
    vector<ST_SIP_ROUTE_PARM> stRouteParams = isCaller ? dlg->_sipMsgDecodePart._recordRoutes._stRouteParams : dlg->_sipMsgDecodePart._calleeRecordRoutes._stRouteParams;
    for (unsigned int i = 0; i < stRouteParams.size(); i++)
    {
        ST_SIP_ROUTE_PARM params = stRouteParams.at(i);

        ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
        ST_SIP_SIP_URI uri;
        if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
            uri = addr.u.stSipUri;
        else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
            uri = addr.u.stSipsUri;

        std::string userInfo, pwd, host;
        int port = 0;
        SipMsgCommon::deSipUri(&uri, userInfo, pwd, host, port);
        pstSipMsgEn->fillHdrRoute(userInfo, host, port, addr.ucType);
    }

    int cseq = isCaller ? pstDecodedPart->_cseqPart._iCalleeCseqVal : pstDecodedPart->_cseqPart._iCseqVal;
    pstSipMsgEn->fillHdrCseq(reqMethod, cseq);

    //fill Max-Forwards Header
    pstSipMsgEn->fillHdrMaxForwards(70);

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), true, reqMethod, dlg, recvConnId, true);
}

bool SipMessage::SendRequestBYE(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;
    unsigned char ucTptType = EN_SIP_TPT_UDP;
    std::string gwIp;
    int gwPort = 0;
    bool isCaller;
    getSipGatewayIpPort(dlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    //check input parameter
    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    bool bResult = false;
    updateSipMsgHead(isCaller, dlg, pstSipMsgDe, reqMethod, true, false, true);

    ucTptType = pstDecodedPart->_viaPart._ucTptType;

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_VIA))
    {
        std::string strBranch = dlg->getBranch();
        bResult = pstSipMsgDe->fillHdrVia2(ucTptType, gwIp, gwPort, false, 0, strBranch, "", 0);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
            return false;
        }
        for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
        {
            ViaPart via = pstDecodedPart->_viaHdrList.at(i);
            bResult = pstSipMsgDe->fillHdrVia3(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
        }
    }

    std::string gw3Ip = dlg->getGw3Ip();
    std::string gw2Ip = dlg->getGw2Ip();
    std::string gw2Ip2 = dlg->getGw2Ip2();
    pstSipMsgDe->delHdrRoute(gw3Ip, gw2Ip, gw2Ip2);

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg, pstSipMsgDe->_strMsg.size(), true, reqMethod, dlg, recvConnId);
}

bool SipMessage::SendRequestCANCEL(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;

    unsigned char ucTptType = EN_SIP_TPT_UDP;

    std::string gwIp;
    int gwPort = 0;
    bool isCaller;
    getSipGatewayIpPort(dlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    bool bResult = false;
    std::string calleeNumber = dlg->getCalleeNumber();
    std::string calleeModifiedNumber = dlg->getModifiedCalleeNumber();

    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    unsigned char dstType;
    int jegoFromType = 0, jegoFromPort = 0;
    int jegoToType = 0, jegoToPort = 0;
    std::string jegoFromDispalyName, jegoFromUserName, jegoFromHost;
    std::string jegoToDispalyName, jegoToUserName, jegoToHost;
    dlg->getCallerParamsByJego(true, jegoFromType, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort);
    dlg->getCallerParamsByJego(false, jegoToType, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort);
    std::string calleeToTag = dlg->getCalleeToTag();
    std::string callerToTag = dlg->getCallerToTag();
    if (callerToTag.empty())
    {
        callerToTag = calleeToTag;
    }
    std::string toTag = isCaller ? calleeToTag : callerToTag;
    pstSipMsgDe->updateReqLine(jegoToType, reqMethod, jegoToUserName, jegoToHost, jegoToPort);
    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
    {
        dstType = jegoToType;
        bResult = pstSipMsgDe->fillHdrFromToByName(false, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort, toTag, dstType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            return false;
        }
    }
    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
    {
        dstType = jegoFromType;
        bResult = pstSipMsgDe->fillHdrFromToByName(true, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort, pFromPart->_strTag, dstType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            return false;
        }
    }

    pstSipMsgDe->delHdrById(EN_SIP_HDR_CONTACT);

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_VIA))
    {
        std::string strBranch = dlg->getBranch();
        bResult = pstSipMsgDe->fillHdrVia2(ucTptType, gwIp, gwPort, false, 0, strBranch, "", 0);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
            return false;
        }

        for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
        {
            ViaPart via = pstDecodedPart->_viaHdrList.at(i);
            bResult = pstSipMsgDe->fillHdrVia3(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
        }
    }

    pstSipMsgDe->delHdrById(EN_SIP_HDR_CSEQ);
    pstSipMsgDe->fillHdrCseq(reqMethod, pstDecodedPart->_cseqPart._iCalleeCseqVal);

    pstSipMsgDe->delHdrById(EN_SIP_HDR_ROUTE);
    vector<ST_SIP_ROUTE_PARM> stRouteParams = dlg->_sipMsgDecodePart._calleeRecordRoutes._stRouteParams;
    for (unsigned int i = 0; i < stRouteParams.size(); i++)
    {
        ST_SIP_ROUTE_PARM params = stRouteParams.at(i);

        ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
        ST_SIP_SIP_URI uri;
        if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
            uri = addr.u.stSipUri;
        else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
            uri = addr.u.stSipsUri;

        std::string userInfo, pwd, host;
        int port = 0;
        SipMsgCommon::deSipUri(&uri, userInfo, pwd, host, port);
        pstSipMsgDe->fillHdrRoute(userInfo, host, port, addr.ucType);
    }

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg, pstSipMsgDe->_strMsg.size(), true, reqMethod, dlg, recvConnId);
}

bool SipMessage::SendRequestPACK(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;
    unsigned char ucTptType = EN_SIP_TPT_UDP;
    std::string gwIp;
    int gwPort = 0;
    bool isCaller;
    getSipGatewayIpPort(dlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    bool bResult = false;
    ucTptType = pstDecodedPart->_viaPart._ucTptType;
    std::string userName = updateSipMsgHead(isCaller, dlg, pstSipMsgDe, reqMethod);

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_CONTACT))
    {
        bResult = pstSipMsgDe->fillHdrContact2(userName, gwIp, gwPort, ucTptType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed");
        }
    }

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_VIA))
    {
        std::string strBranch = SipMsgCommon::createBranch();
        bResult = pstSipMsgDe->fillHdrVia2(ucTptType, gwIp, gwPort, false, 0, strBranch, "", 0);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
            return false;
        }
        for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
        {
            ViaPart via = pstDecodedPart->_viaHdrList.at(i);
            bResult = pstSipMsgDe->fillHdrVia3(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
        }
    }

    std::string gw3Ip = dlg->getGw3Ip();
    std::string gw2Ip = dlg->getGw2Ip();
    std::string gw2Ip2 = dlg->getGw2Ip2();
    pstSipMsgDe->delHdrRoute(gw3Ip, gw2Ip, gw2Ip2);

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg, pstSipMsgDe->_strMsg.size(), true, reqMethod, dlg, recvConnId);
}

bool SipMessage::SendRequestUPDATE(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;
    unsigned char ucTptType, ucUriType;
    int iPort;
    ucTptType = EN_SIP_TPT_UDP;

    std::string gwIp;
    int gwPort = 0;
    bool isCaller;
    getSipGatewayIpPort(dlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    std::string fromTag;
    pstSipMsgDe->getHdrFromToTag(true, fromTag);
    pstSipMsgDe->getHdrFromToUri(false, strDispName, strUserName, strHost, iPort, ucUriType);

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    bool bResult = false;
    ucTptType = pstDecodedPart->_viaPart._ucTptType;
    std::string calleeNumber = dlg->getCalleeNumber();
    std::string calleeModifiedNumber = dlg->getModifiedCalleeNumber();

    std::string userName  = updateSipMsgHead(isCaller, dlg, pstSipMsgDe, reqMethod);

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_CONTACT))
    {
        bResult = pstSipMsgDe->fillHdrContact2(userName, gwIp, gwPort, ucTptType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed");
            return false;
        }
    }

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_VIA))
    {
        std::string strBranch = dlg->getBranch();
        bResult = pstSipMsgDe->fillHdrVia2(ucTptType, gwIp, gwPort, false, 0, strBranch, "", 0);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
            return false;
        }
        for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
        {
            ViaPart via = pstDecodedPart->_viaHdrList.at(i);
            bResult = pstSipMsgDe->fillHdrVia3(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
        }
    }

    std::string gw3Ip = dlg->getGw3Ip();
    std::string gw2Ip = dlg->getGw2Ip();
    std::string gw2Ip2 = dlg->getGw2Ip2();

    pstSipMsgDe->delHdrRoute(gw3Ip, gw2Ip, gw2Ip2);

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg, pstSipMsgDe->_strMsg.size(), true, reqMethod, dlg, recvConnId);
}

bool SipMessage::sendSipMessageRequestMsg(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, EN_SIP_METHOD reqMethod, const std::string & msg, const std::string & recvConnId)
{
    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }

    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    SipGateway::AuthType authType = sipDlg->getAuthType();

    switch(reqMethod)
    {
    case EN_SIP_METHOD_INVITE:
        if (authType == SipGateway::AuthNone)
            return SendRequestINVITE(reqMethod,sipDlg , pSipGateway, msg, recvConnId);
        else
            return SendRequestINVITEWithAuth(reqMethod, sipDlg, authType, pSipGateway, msg, recvConnId);
        break;

    case EN_SIP_METHOD_ACK:
        if (authType == SipGateway::AuthNone)
            return SendRequestACK(reqMethod, sipDlg, pSipGateway, msg, recvConnId);
        else
            return SendRequestACK2(reqMethod, sipDlg, pSipGateway, msg, recvConnId);
        break;

    case EN_SIP_METHOD_BYE:
        return SendRequestBYE(reqMethod, sipDlg, pSipGateway, msg, recvConnId);
        break;

    case EN_SIP_METHOD_CANCEL:
        return SendRequestCANCEL(reqMethod, sipDlg, pSipGateway, msg, recvConnId);
        break;

    case EN_SIP_METHOD_PRACK:
        return SendRequestPACK(reqMethod, sipDlg, pSipGateway, msg, recvConnId);
        break;

    case EN_SIP_METHOD_UPDATE:
        return SendRequestUPDATE(reqMethod,sipDlg, pSipGateway, msg, recvConnId);
        break;

    default:
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled.a wrong method:" + std::string(int(reqMethod)));
    }
    return true;
}

bool SipMessage::sendSipMessageRequestMsg2(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, EN_SIP_METHOD reqMethod, const std::string & msg, const std::string & recvConnId)
{
    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }

    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    switch(reqMethod)
    {
    case EN_SIP_METHOD_ACK:
        return SendRequestACK2(reqMethod, sipDlg, pSipGateway, msg, recvConnId);
        break;

    case EN_SIP_METHOD_CANCEL:
        return SendRequestCANCEL2(reqMethod, sipDlg, pSipGateway, msg,recvConnId);
        break;

    default:
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled.a wrong method:" + std::string(int(reqMethod)));
        break;
    }
    return true;
}

bool SipMessage::sendSipMessageResponseMsg(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, const int rspCode, EN_SIP_METHOD reqMethod, const std::string & msg, const std::string & recvConnId)
{
    SipGateway::SDPNegotiateStatus sdpStatus = SipGateway::SDPInit;
    unsigned char tptType = EN_SIP_TPT_UDP;

    std::string strDispName, strUserName, strHostName, strHost;
    std::string strBranch, strRecvHost;
    unsigned char ucTptType = 0, ucUriType = 0;
    int iPort = 0, iRport = 0;

    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    std::string gwIp;
    int gwPort = 0;
    bool isCaller;
    getSipGatewayIpPort(sipDlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    //check input parameter
    SipMsgDecodedPart *pstDecodedPart = &sipDlg->_sipMsgDecodePart;
    bool bResult = false;

    std::string userName = updateSipMsgHead(isCaller, sipDlg, pstSipMsgDe, reqMethod, false);

    ucTptType = pstDecodedPart->_viaPart._ucTptType;

    std::string gw3Ip = sipDlg->getGw3Ip();
    std::string gw2Ip = sipDlg->getGw2Ip();
    std::string gw2Ip2 = sipDlg->getGw2Ip2();
    pstSipMsgDe->delHdrVia(gw3Ip, gw2Ip, gw2Ip2);

    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_CONTACT))
    {
        bResult = pstSipMsgDe->fillHdrContact2(userName, gwIp, gwPort, ucTptType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed, gwIp:" + gwIp + ", port:" + std::string(gwPort));
            return false;
        }
    }

    if (reqMethod == EN_SIP_METHOD_INVITE && (rspCode == 180 || rspCode == 183))
    {
        std::string strSDPBody;
        bool existSDP;
        if (pstSipMsgDe->checkSipMsgWithSDP(reqMethod, false, rspCode, existSDP, strSDPBody))
        {
            if(existSDP)
            {
                ST_ZOS_SSTR stBodyStr;
                std::string strBody = isCaller ? sipDlg->getSdpOffer() : sipDlg->getSdpAnswer();
                SipMsgCommon::str2ZosSstr(strBody, stBodyStr);
                pstSipMsgDe->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
            }
        }
    }

    if (reqMethod == EN_SIP_METHOD_INVITE && rspCode == 200)
    {
        if (pstDecodedPart->_toPart._strTag.empty())
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. to tag is empty callId:" + sipDlg->getCallIdHost());
            return false;
        }

        pstSipMsgDe->delHdrById(EN_SIP_HDR_P_ASSERTED_ID);

        ST_ZOS_SSTR stBodyStr;
        std::string strBody = isCaller ? sipDlg->getSdpOffer() : sipDlg->getSdpAnswer();
        SipMsgCommon::str2ZosSstr(strBody, stBodyStr);
        pstSipMsgDe->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP,&stBodyStr);
    }

    vector<ST_SIP_ROUTE_PARM> stRouteParams;
    pstSipMsgDe->getHdrRecordRouteParams(stRouteParams);
    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_RECORD_ROUTE))
    {
        for (unsigned int i = 0; i < stRouteParams.size(); i++)
        {
            ST_SIP_ROUTE_PARM params = stRouteParams.at(i);
            ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
            ST_SIP_SIP_URI uri;
            if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                uri = addr.u.stSipUri;
            else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                uri = addr.u.stSipsUri;

            std::string userInfo, pwd, host;
            int port = 0;
            SipMsgCommon::deSipUri(&uri, userInfo, pwd, host, port);
            if (host == gw3Ip || host == gw2Ip || host == gw2Ip2)
            {
                bResult = pstSipMsgDe->fillHdrRecordRoute("", gwIp, gwPort, ucTptType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrRecordRoute() failed");
                    return false;
                }
            }
            else
            {
                pstSipMsgDe->fillHdrRecordRoute(params);
            }
        }
    }

    if (!pstSipMsgDe->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgDe->_strMsg,  pstSipMsgDe->_strMsg.size(), false, reqMethod, sipDlg, recvConnId);
}


bool SipMessage::sendSipMessage183(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, int rspCode, EN_SIP_METHOD reqMethod, const std::string & newSdp, const std::string & recvConnId)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strBranch, strHost, strRecvHost;
    int iPort = 0, iRport = 0, gwPort = 0;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string gwIp;
    std::string strUserName, strHostName;
    unsigned char ucTptType = EN_SIP_TPT_UDP;
    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }
    std::string msg = sipDlg->getSipMsg();
    if (msg.empty())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, SipMsg is NULL");
        return false;
    }
    pstDecodedPart = &sipDlg->_sipMsgDecodePart;

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }
    pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createRsp())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call createRsp failed .sipDialog id:" + sipDlg->id());
        return false;
    }
    pstSipMsgEn->fillStatusLine(rspCode);
    bool bResult = false;

    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    int fromType = 0, fromPort = 0, jegoFromType = 0, jegoFromPort = 0;
    int toType = 0, toPort = 0, jegoToType = 0, jegoToPort = 0;
    std::string fromDispalyName, fromUserName, fromHost;
    std::string toDispalyName, toUserName, toHost;
    sipDlg->getCallerParams(true, fromType, fromDispalyName, fromUserName, fromHost, fromPort);
    sipDlg->getCallerParams(false, toType, toDispalyName, toUserName, toHost, toPort);
    std::string toTag = sipDlg->getCallerToTag();
    unsigned char dstType;
    if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
    {
        dstType = toType;
        bResult = pstSipMsgDe->fillHdrFromToByName(false, pToPart->_strDispName, toUserName, toHost, toPort, toTag, dstType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
        }
    }

    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_FROM);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_TO);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CSEQ);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_VIA);

    pstSipMsgEn->fillHdrMaxForwards(70);

    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_PRACK);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    std::string early = "sendonly";
    std::string head = "P-Early-Media";
    pstSipMsgEn->fillHdrExt(head, early);

    gwIp = sipDlg->getGw2Ip();
    gwPort = sipDlg->getGw2Port();

    vector<ST_SIP_ROUTE_PARM> stRouteParams;
    pstSipMsgDe->getHdrRecordRouteParams(stRouteParams);

    if (stRouteParams.size() > 0)
    {
        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_RECORD_ROUTE))
        {
            bResult = pstSipMsgDe->fillHdrRecordRoute("", gwIp, gwPort, ucTptType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrRecordRoute() failed");
                return false;
            }

            for (unsigned int i = 0; i < stRouteParams.size(); i++)
            {
                ST_SIP_ROUTE_PARM params = stRouteParams.at(i);
                pstSipMsgDe->fillHdrRecordRoute(params);
            }
        }
    }
    else
    {
        bResult = pstSipMsgDe->fillHdrRecordRoute("", gwIp, gwPort, ucTptType);
        if (!bResult)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrRecordRoute() failed");
            return false;
        }
    }

    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_RECORD_ROUTE);

    //fill Supported
    //vector<int> viSuptFlag;
    //viSuptFlag.push_back(SIP_TAG_OPT_ADD_100REL);
    //pstSipMsgEn->fillHdrSupted(viSuptFlag);

    //ZULONG dwSuptFlag = 0;
    //ZOS_ZERO(dwSuptFlag);
    //ZOS_SET(SIP_TAG_OPT_ADD_100REL, dwSuptFlag);
    //pstSipMsgEn->fillHdrRequire(dwSuptFlag);
    //pstSipMsgEn->fillHdrRseq(1);

    ST_ZOS_SSTR stBodyStr;
    SipMsgCommon::str2ZosSstr(newSdp, stBodyStr);
    pstSipMsgEn->fillBodyDataStr(EN_SIP_MTYPE_DISC_APP, EN_SIP_MSUBTYPE_SDP, &stBodyStr);
    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode() return FALSE! sidDialog id :" + sipDlg->id());
        return false;
    }
    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), false, reqMethod, sipDlg, recvConnId, true);
}

bool SipMessage::sendSipMessage100(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, const std::string & recvConnId)
{
    SipMsgEncodePtr pstSipMsgEn;
    SipMsgDecodedPart * pstDecodedPart = 0;
    unsigned char ucTptType = EN_SIP_TPT_UDP;
    if (sipDlg == 0 || pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }

    std::string msg = sipDlg->getSipMsg();
    if (msg.empty())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, SipMsg is NULL");
        return false;
    }
    pstDecodedPart = &sipDlg->_sipMsgDecodePart;

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createRsp())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call createRsp failed .sipDialog id:" + sipDlg->id());
        return false;
    }

    pstSipMsgEn->fillStatusLine(100);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_FROM);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_TO);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CSEQ);

    for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
    {
        ViaPart via = pstDecodedPart->_viaHdrList.at(i);
        pstSipMsgEn->fillHdrVia(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
    }

    pstSipMsgEn->fillHdrMaxForwards(70);

    vector<int> vectAllow;
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_INVITE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_ACK);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_BYE);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_OPTIONS);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_CANCEL);
    vectAllow.push_back(SIP_ALLOW_HDR_ADD_UPDATE);
    pstSipMsgEn->fillHdrAllow(vectAllow);

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode() return FALSE! sidDialog id :" + sipDlg->id());
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), false, EN_SIP_METHOD_EXT, sipDlg, recvConnId, true);
}

bool SipMessage::onReiceivedSipMsgFromSipGateway(const std::string &strBuffer,int iBufLen, const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway,
                                                 const std::string & sipGatewayId, const std::string & connectionId, const std::map<std::string, std::string> & params)
{
    if (0 == pSipGateway)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  pSipGateway == NULL!SipMsg:\n" + strBuffer);
        return false;
    }

    SipMsgDecodePtr pstSipMsgDe = 0;
    unsigned char ucMethod;
    bool bReqPres = true;
    pstSipMsgDe = new SipMsgDecode();

    unsigned char ucResult = pstSipMsgDe->decode(strBuffer, strBuffer.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + strBuffer);
        return false;
    }

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called, getMethod failed.SipMsg:\n" + strBuffer);
        return false;
    }
    bReqPres = pstSipMsgDe->isRequest() ? true : false;
    std::string remoteIp;
    int remotePort = 5060;
    std::map<std::string, std::string>::const_iterator itorKey = params.find("Remote.Ip");
    if (itorKey != params.end())
    {
        remoteIp = itorKey->second;
    }
    itorKey = params.find("Remote.Port");
    if (itorKey != params.end())
    {
        remotePort = (itorKey->second).toInt(0);
    }
    //TLOGINFO("[SipMsg] " <<  std::string(__FUNCTION__)+"() called, remoteIp:" + remoteIp + ", remotePort:" + std::string(remotePort));
    switch(ucMethod)
    {
    case EN_SIP_METHOD_INVITE:
        if (bReqPres)
        {
            return dealInviteRequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            return dealInviteResponce(strBuffer, iBufLen, pSipGateway, sipGatewayId, remoteIp, remotePort, pstSipMsgDe);
        }
        break;

    case EN_SIP_METHOD_ACK:
        return dealAckRequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        break;

    case EN_SIP_METHOD_BYE:
        if (bReqPres)
        {
            return dealBYERequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            return dealBYEResponce(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort);
        }
        break;

    case EN_SIP_METHOD_CANCEL:
        if (bReqPres)
        {
            return dealCANCELRequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            return dealCANCELResponce(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort);
        }
        break;

    case EN_SIP_METHOD_UPDATE:
        if (bReqPres)
        {
            return dealUPDATERequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            return dealUPDATEResponse(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort);
        }
        break;

    case EN_SIP_METHOD_PRACK:
        if (bReqPres)
        {
            return dealPRACKRequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            return dealPRACKResponce(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort);
        }
        break;

    case EN_SIP_METHOD_INFO:
        if (bReqPres == false)
        {
            TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  INFO Response NOT supported.SipMsg:\n" + strBuffer);
            return false;
        }
        else
        {
            return dealINFORequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        }
        break;

    case EN_SIP_METHOD_OPTIONS:
        if (bReqPres)
        {
            return dealOptionsRequest(strBuffer, iBufLen, pSipGateway, sipGatewayId, pstSipMsgDe, remoteIp, remotePort, params);
        }
        else
        {
            TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  INFO Response NOT supported.SipMsg:\n" + strBuffer);
            return false;
        }
        break;

    default:
        {
            TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called, this method is NOT supported.SipMsg:\n" + strBuffer);
            return false;
        }
    }
    return true;
}

bool SipMessage::CreateSipMsgFromSDP(const std::string & strBody, std::string & strSipMsg)
{
    if (strBody.empty())
        return false;

    unsigned ucMethod;
    std::string strHost, strCallHost;
    std::string strFromDispname, strToDispName, strContactHost;
    std::string strFromTag, strBranch;
    std::string strUAName, strUAVersion, strUAComm;
    int iPort;

    SipMsgEncodePtr pstSipMsgEn = new SipMsgEncode();
    ucMethod = EN_SIP_METHOD_INVITE;
    strFromDispname = "13957854053";
    strFromTag = TC_UUIDGenerator::getInstance()->genID();
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
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__)+"() called, encode(), return FALSE! strBody:\n" + strBody);
        return false;
    }

    strSipMsg = pstSipMsgEn->_strMsg;
    SipMsgDecodePtr pstOffer = new SipMsgDecode();

    unsigned char ucResult = pstOffer->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sip msg is not valid or complete.strSipMsg:\n" + strSipMsg);
        return false;
    }

    stTemBody = pstOffer->_pstMsg->stBody.stBodyStr;
    return true;
}

bool SipMessage::getMediaIpAndPortsFromSipMsg(const std::string & strSipMsgWithSDP,  vector<SDPMediaInfo> & vectSdpMediaInfo)
{
    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    return pstSipMsgDe->getSBCMediaIpAndPort(strSipMsgWithSDP, vectSdpMediaInfo);
}

bool SipMessage::GetIpFromSDPConnField(const std::string & strCsSDP, std::string & ip)
{
    vector<SDPMediaInfo>  vectSdpMediaInfo;
    std::string strSipMsgWithSDP;
    std::string reason;

    try
    {
        if (!SipMessage::CreateSipMsgFromSDP(strCsSDP, strSipMsgWithSDP))
        {
            reason = "SipMsg::CreateSipMsgFromSDP() return FALSE";
            throw false;
        }

        if (!SipMessage::getMediaIpAndPortsFromSipMsg(strSipMsgWithSDP, vectSdpMediaInfo))
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
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  return FALSE!  reason:" + reason + ",sdp:\n" + strCsSDP);
        }
        else
        {
            TLOGDEBUG("[SipMsg] " << std::string(__FUNCTION__)+"() called,  return TRUE!  ,got ip:" + ip + ",sdp:\n" + strCsSDP);
        }
        return ex;
    }
}

bool SipMessage::sipMessageValidCheck(const std::string & msg)
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

bool SipMessage::sendSipMessageRequestMsgBye(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg, EN_SIP_METHOD reqMethod, const std::string & recvConnId)
{
    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    std::string msg = sipDlg->getSipMsg();
    if (msg.empty())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, SipMsg is NULL");
        return false;
    }

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    SipMsgEncodePtr pstSipMsgEn;
    int iCseq = 0, iPort = 0, iReqUriType = 0, iRport = 0;
    unsigned char ucTptType = EN_SIP_TPT_UDP;

    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. createReq failed .sipDialog id:" + sipDlg->id());
        return false;
    }

    std::string gwIp;
    int gwPort = 0;
    bool isCaller;
    getSipGatewayIpPort(sipDlg, gwIp, gwPort, isCaller, recvConnId);

    SipMsgDecodedPart *pstDecodedPart = &sipDlg->_sipMsgDecodePart;
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;

    bool bResult = false;
    int fromType = 0, fromPort = 0, jegoFromType = 0, jegoFromPort = 0;
    int toType = 0, toPort = 0, jegoToType = 0, jegoToPort = 0;
    std::string fromDispalyName, fromUserName, fromHost;
    std::string toDispalyName, toUserName, toHost;
    std::string jegoFromDispalyName, jegoFromUserName, jegoFromHost;
    std::string jegoToDispalyName, jegoToUserName, jegoToHost;

    sipDlg->getCallerParams(true, fromType, fromDispalyName, fromUserName, fromHost, fromPort);
    sipDlg->getCallerParams(false, toType, toDispalyName, toUserName, toHost, toPort);
    sipDlg->getCallerParamsByJego(true, jegoFromType, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort);
    sipDlg->getCallerParamsByJego(false, jegoToType, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort);
    unsigned char dstType;
    std::string calleeToTag = sipDlg->getCalleeToTag();
    std::string callerToTag = sipDlg->getCallerToTag();
    if (callerToTag.empty())
    {
        callerToTag = calleeToTag;
    }

    SipCloudGateway::EnterpriseType enterType;
    if (isCaller)
    {
        enterType = sipDlg->getCalleeEnterpriseType();
        if (enterType == SipCloudGateway::kEnterpriseTwilio || enterType == SipCloudGateway::kEnterpriseVanage)
        {
            pstSipMsgEn->fillReqLine(jegoToType, EN_SIP_METHOD_BYE, jegoToUserName, pstDecodedPart->_toPart._calleeContactPart._strHostName, pstDecodedPart->_toPart._calleeContactPart._iPort);
        }
        else
        {
            pstSipMsgEn->fillReqLine(jegoToType, EN_SIP_METHOD_BYE, jegoToUserName, jegoToHost, jegoToPort);
        }

        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
        {
            dstType = jegoToType;
            bResult = pstSipMsgDe->fillHdrFromToByName(false, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort, calleeToTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                return false;
            }
        }

        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
        {
            dstType = jegoFromType;

            bResult = pstSipMsgDe->fillHdrFromToByName(true, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort, pFromPart->_strTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                return false;
            }
        }
    }
    else
    {
        enterType = sipDlg->getCallerEnterpriseType();
        if (enterType == SipCloudGateway::kEnterpriseTwilio || enterType == SipCloudGateway::kEnterpriseVanage)
        {
            pstSipMsgEn->fillReqLine(jegoToType, EN_SIP_METHOD_BYE, fromUserName, pstDecodedPart->_toPart._contactPart._strHostName, pstDecodedPart->_toPart._contactPart._iPort);
        }
        else
        {
            pstSipMsgEn->fillReqLine(fromType, EN_SIP_METHOD_BYE, fromUserName, fromHost, fromPort);
        }

        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
        {
            dstType = toType;

            bResult = pstSipMsgDe->fillHdrFromToByName(true, toDispalyName, toUserName, toHost, toPort, callerToTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                return false;
            }
        }

        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
        {
            dstType = fromType;

            bResult = pstSipMsgDe->fillHdrFromToByName(false, fromDispalyName, fromUserName, fromHost, fromPort, pFromPart->_strTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                return false;
            }
        }
    }

    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_FROM);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_TO);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);

    ucTptType = pstDecodedPart->_viaPart._ucTptType;
    std::string strBranch = sipDlg->getBranch();
    bResult = pstSipMsgEn->fillHdrVia(ucTptType, gwIp, gwPort, false, 0, strBranch);
    if (!bResult)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
        return false;
    }

    int cseq = isCaller ? pstDecodedPart->_cseqPart._iCalleeCseqVal : pstDecodedPart->_cseqPart._iCseqVal;
    pstSipMsgEn->fillHdrCseq(reqMethod, cseq + 1);

    //fill Max-Forwards Header
    pstSipMsgEn->fillHdrMaxForwards(70);

    //fill route
    vector<ST_SIP_ROUTE_PARM> stRouteParams = isCaller ? sipDlg->_sipMsgDecodePart._calleeRecordRoutes._stRouteParams : sipDlg->_sipMsgDecodePart._recordRoutes._stRouteParams;
    for (unsigned int i = 0; i < stRouteParams.size(); i++)
    {
        ST_SIP_ROUTE_PARM params = stRouteParams.at(i);

        ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
        ST_SIP_SIP_URI uri;
        if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
            uri = addr.u.stSipUri;
        else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
            uri = addr.u.stSipsUri;

        std::string userInfo, pwd, host;
        int port = 0;
        SipMsgCommon::deSipUri(&uri, userInfo, pwd, host, port);
        pstSipMsgEn->fillHdrRoute(userInfo, host, port, addr.ucType);
    }

    int ocsError = sipDlg->getOcsErrorCode();
    if (ocsError)
        pstSipMsgEn->fillHdrExt("ocs-term-id", std::string(ocsError));

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode() return FALSE! sidDialog id :" + sipDlg->id());
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), false, reqMethod, sipDlg, recvConnId, false);
}

bool SipMessage::getSipMsgSipDialogId(const std::string & strSipMsg, std::string & sipDialogId,  std::string & callId, unsigned char & ucMethod, bool & req,unsigned int & ulStatusCode)
{
    SipMsgDecodePtr pstSipMsgDe = 0;
    int   iSceq = 0;
    std::string strLocalTag, strRemoteTag, strCallId, strCallHost;

    pstSipMsgDe = new SipMsgDecode();

    //check decode procedure valid
    unsigned char ucResult = pstSipMsgDe->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sip msg is not valid or complete. strSipMsg :\n" + strSipMsg);
        return false;
    }

    // get Call-Id Header
    pstSipMsgDe->getCallId(strCallId, strCallHost);
    pstSipMsgDe->getHdrFromToTag(true, strLocalTag);
    pstSipMsgDe->getHdrFromToTag(false, strRemoteTag);

    //get Method
    if(!pstSipMsgDe->getMethod(ucMethod))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, getMethod failed.strSipMsg :\n" + strSipMsg);
        return false;
    }
    req = pstSipMsgDe->isRequest() ? true : false;
    if (false == req)
    {
        pstSipMsgDe->getRspStatusCode(ulStatusCode);
    }

    sipDialogId = strCallId + "@" + strCallHost;
    callId = strCallId + "@" + strCallHost;
    return true;
}

bool SipMessage::sendSipMessageExeptionMsg(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg,
            const int rspCode, EN_SIP_METHOD reqMethod, const std::string & reason, const std::string & msg, const std::string & recvConnId, bool resend)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strBranch, strHost, strRecvHost;
    int iPort = 0, iRport = 0, gwPort = 0;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string gwIp;
    std::string strUserName, strHostName;

    unsigned char ucTptType = EN_SIP_TPT_UDP;

    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    pstDecodedPart = &sipDlg->_sipMsgDecodePart;

    SipCloudGateway::CallerDirection direction = sipDlg->getDirection();
    SipCloudGateway::CallerDirection callerDirection = sipDlg->getDirection();
    getSipGatewayIpPort2(sipDlg, gwIp, gwPort, recvConnId);

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();

    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createRsp())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call createRsp failed .sipDialog id:" + sipDlg->id());
        return false;
    }

    std::string connectionId1 = sipDlg->getSipGatewayConnectId1();
    std::string connectionId3 = sipDlg->getCallerRouteConnectId();
    connectionId3 = connectionId3.empty() ? connectionId1 : connectionId3;
    bool isCaller = (recvConnId == connectionId1 || recvConnId == connectionId3) ? true : false;

    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;

    int fromType = 0, fromPort = 0, jegoFromType = 0, jegoFromPort = 0;
    int toType = 0, toPort = 0, jegoToType = 0, jegoToPort = 0;

    std::string fromDispalyName, fromUserName, fromHost;
    std::string toDispalyName, toUserName, toHost;

    std::string jegoFromDispalyName, jegoFromUserName, jegoFromHost;
    std::string jegoToDispalyName, jegoToUserName, jegoToHost;

    sipDlg->getCallerParams(true, fromType, fromDispalyName, fromUserName, fromHost, fromPort);
    sipDlg->getCallerParams(false, toType, toDispalyName, toUserName, toHost, toPort);
    sipDlg->getCallerParamsByJego(true, jegoFromType, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort);
    sipDlg->getCallerParamsByJego(false, jegoToType, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort);
    unsigned char dstType;
    std::string calleeToTag = sipDlg->getCalleeToTag();
    std::string callerToTag = sipDlg->getCallerToTag();
    if (callerToTag.empty())
    {
        callerToTag = calleeToTag.empty() ? SipMsgCommon::createToTag() : calleeToTag;
    }

    bool bResult = false;
    if (isCaller)
    {
        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
        {
            dstType = toType;

            bResult = pstSipMsgDe->fillHdrFromToByName(false, pToPart->_strDispName, toUserName, toHost, toPort, callerToTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            }
        }

        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
        {
            dstType = fromType;

            bResult = pstSipMsgDe->fillHdrFromToByName(true, fromDispalyName, fromUserName, fromHost, fromPort, pFromPart->_strTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            }
        }
    }
    else
    {
        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
        {
            dstType = jegoToType;
            bResult = pstSipMsgDe->fillHdrFromToByName(false, pToPart->_strDispName, jegoToUserName, jegoToHost, jegoToPort, callerToTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            }
        }

        if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
        {
            dstType = jegoFromType;

            bResult = pstSipMsgDe->fillHdrFromToByName(true, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort, pFromPart->_strTag, dstType);
            if (!bResult)
            {
                TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
            }
        }
    }

    //fill status line
    pstSipMsgEn->fillStatusLine(rspCode);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_FROM);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_TO);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CSEQ);

    for (int i = 0; i < pstDecodedPart->_viaHdrList.size(); i++)
    {
        ViaPart via = pstDecodedPart->_viaHdrList.at(i);

        if (i == 0)
        {
            std::string gw3Ip = sipDlg->getGw3Ip();
            std::string gw2Ip = sipDlg->getGw2Ip();
            std::string gw2Ip2 = sipDlg->getGw2Ip2();
            if (via._strHostName == gw3Ip || via._strHostName == gw2Ip || via._strHostName == gw2Ip2)
                continue;
        }

        pstSipMsgEn->fillHdrVia(via._ucTptType, via._strHostName, via._iPort, via._bRport, via._iRport, via._strBranch, via._received, via._recvPort, via._paramsExt);
    }

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

    if (!reason.empty())
        pstSipMsgEn->fillHdrReason(reason);

    int ocsError = sipDlg->getOcsErrorCode();
    if (ocsError)
        pstSipMsgEn->fillHdrExt("ocs-term-id", std::string(ocsError));

    vector<ST_SIP_ROUTE_PARM> stRouteParams;
    pstSipMsgDe->getHdrRecordRouteParams(stRouteParams);

    for (unsigned int i = 0; i < stRouteParams.size(); i++)
    {
        ST_SIP_ROUTE_PARM params = stRouteParams.at(i);
        pstSipMsgEn->fillHdrRecordRoute(params);
    }

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode() return FALSE! sidDialog id :" + sipDlg->id());
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), false, reqMethod, sipDlg, recvConnId, resend);
}

bool SipMessage::sendSipMessageResponceRegister(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg,
                                                const int rspCode, EN_SIP_METHOD reqMethod, const std::string &msg, const std::string & recvConnId)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strBranch, strHost, strRecvHost;
    int iPort = 0, iRport = 0, gwPort = 0;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string gwIp;
    std::string strUserName, strHostName;

    unsigned char ucTptType = EN_SIP_TPT_UDP;

    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    pstDecodedPart = &sipDlg->_sipMsgDecodePart;

    gwIp = sipDlg->getGw2Ip();
    gwPort = sipDlg->getGw2Port();

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createRsp())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call createRsp failed .sipDialog id:" + sipDlg->id());
        return false;
    }

    //fill status line
    pstSipMsgEn->fillStatusLine(rspCode);
    pstSipMsgEn->fillHdrContact(pstDecodedPart->_fromPart._strUserName, gwIp, gwPort, ucTptType);
    pstSipMsgEn->ParmFillContactExpires(36000);

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    pToPart->_strTag = SipMsgCommon::createToTag();
    pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
        pToPart->_strTag, pToPart->_ucAddrType);

    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_FROM);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CSEQ);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_VIA);
    pstSipMsgEn->fillHdrMaxForwards(70);

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode() return FALSE! sidDialog id :" + sipDlg->id());
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), false, reqMethod, sipDlg, recvConnId, true);
}

bool SipMessage::sendSipMessageResponceOPTIONS(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, SipCloudGateway::SipDlgPtr &sipDlg,
                                                const int rspCode, EN_SIP_METHOD reqMethod, const std::string &msg, const std::string & recvConnId)
{
    SipMsgEncodePtr pstSipMsgEn;
    std::string strBranch, strHost, strRecvHost;
    int iPort = 0, iRport = 0, gwPort = 0;
    SipMsgDecodedPart * pstDecodedPart = 0;
    std::string gwIp;
    std::string strUserName, strHostName;

    unsigned char ucTptType = EN_SIP_TPT_UDP;

    if (sipDlg == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. Error, Input parameter dlg is NULL.");
        return false;
    }
    if (pSipGateway == 0)
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. pSipGateway == NULL! ERROR!");
        return false;
    }

    pstDecodedPart = &sipDlg->_sipMsgDecodePart;

    gwIp = sipDlg->getGw2Ip();
    gwPort = sipDlg->getGw2Port();

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size());
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createRsp())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call createRsp failed .sipDialog id:" + sipDlg->id());
        return false;
    }

    //fill status line
    pstSipMsgEn->fillStatusLine(rspCode);

    //fill To Header
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    pToPart->_strTag = SipMsgCommon::createToTag();
    pstSipMsgEn->fillHdrFromToByName(false, pToPart->_strDispName, pToPart->_strUserName, pToPart->_strHostName, pToPart->_dwPort,
        pToPart->_strTag, pToPart->_ucAddrType);

    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_FROM);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CSEQ);
    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_VIA);

    pstSipMsgEn->fillHdrMaxForwards(70);

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode() return FALSE! sidDialog id :" + sipDlg->id());
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), false, reqMethod, sipDlg, recvConnId, true);
}

bool SipMessage::SendRequestCANCEL2(EN_SIP_METHOD reqMethod, SipCloudGateway::SipDlgPtr & dlg,const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & pSipGateway, const std::string & msg, const std::string & recvConnId)
{
    unsigned char ucTptType = EN_SIP_TPT_UDP;
    std::string gwIp = dlg->getGw2Ip2();
    int gwPort = dlg->getGw2Port2();
    std::string toTag = dlg->getCalleeToTag();

    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    unsigned char ucResult = pstSipMsgDe->decode(msg, msg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " <<  std::string(__FUNCTION__)+"() called,  Sip msg is not valid or complete. SipMsg:\n" + msg);
        return false;
    }

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    bool bResult = false;
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;
    unsigned char dstType;
    int jegoFromType = 0, jegoFromPort = 0;
    int jegoToType = 0, jegoToPort = 0;
    std::string jegoFromDispalyName, jegoFromUserName, jegoFromHost;
    std::string jegoToDispalyName, jegoToUserName, jegoToHost;
    dlg->getCallerParamsByJego(true, jegoFromType, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort);
    dlg->getCallerParamsByJego(false, jegoToType, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort);

    SipMsgEncodePtr pstSipMsgEn = new SipMsgEncode();
    if (!pstSipMsgEn->createReq())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. createReq failed .sipDialog id:" + dlg->id());
        return false;
    }

    pstSipMsgEn->fillReqLine(jegoToType, EN_SIP_METHOD_CANCEL, jegoToUserName, jegoToHost, jegoToPort);

    dstType = jegoFromType;
    bResult = pstSipMsgEn->fillHdrFromToByName(true, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort, pFromPart->_strTag, dstType);
    if (!bResult)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
        return false;
    }

    dstType = jegoToType;
    bResult = pstSipMsgEn->fillHdrFromToByName(false, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort, toTag, dstType);
    if (!bResult)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
        return false;
    }

    bResult = pstSipMsgEn->fillHdrContact(pstDecodedPart->_fromPart._strUserName, gwIp, gwPort, ucTptType);
    if (!bResult)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrContact() failed");
        return false;
    }

    bResult = pstSipMsgEn->fillHdrVia(ucTptType, gwIp, gwPort, false, 0, dlg->getBranch());
    if (!bResult)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrVia() failed");
        return false;
    }

    pstSipMsgEn->fillHdrCseq(reqMethod, pstDecodedPart->_cseqPart._iCalleeCseqVal);

    vector<ST_SIP_ROUTE_PARM> stRouteParams = dlg->_sipMsgDecodePart._calleeRecordRoutes._stRouteParams;
    for (unsigned int i = 0; i < stRouteParams.size(); i++)
    {
        ST_SIP_ROUTE_PARM params = stRouteParams.at(i);

        ST_SIP_ADDR_SPEC addr = params.stNameAddr.stAddrSpec;
        ST_SIP_SIP_URI uri;
        if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
            uri = addr.u.stSipUri;
        else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
            uri = addr.u.stSipsUri;

        std::string userInfo, pwd, host;
        int port = 0;
        SipMsgCommon::deSipUri(&uri, userInfo, pwd, host, port);
        pstSipMsgEn->fillHdrRoute(userInfo, host, port, addr.ucType);
    }

    pstSipMsgEn->CpyHdrFromMsg(pstSipMsgEn->_pstMsg, pstSipMsgDe->_pstMsg, EN_SIP_HDR_CALL_ID);
    pstSipMsgEn->fillHdrMaxForwards(70);

    if (!pstSipMsgEn->encode())
    {
        TLOGERROR("[SipMsg] " << std::string(__FUNCTION__) + "() callled. encode(), return FALSE!");
        return false;
    }

    return pSipGateway->sendEncodeSipMsg(pstSipMsgEn->_strMsg, pstSipMsgEn->_strMsg.size(), false, reqMethod, dlg, recvConnId);
}

bool SipMessage::getSipGatewayIpPort(SipCloudGateway::SipDlgPtr & dlg, std::string & gwIp, int & gwPort, bool & isCaller, const std::string & recvConnId)
{
    std::string connectId1 = dlg->getSipGatewayConnectId1();
    std::string connectId3 = dlg->getCallerRouteConnectId();
    if (recvConnId == connectId1 || recvConnId == connectId3)
    {
        gwIp = dlg->getGw2Ip2();
        gwPort = dlg->getGw2Port2();
        isCaller = true;
    }
    else
    {
        gwIp = dlg->getGw2Ip();
        gwPort = dlg->getGw2Port();
        isCaller = false;
    }

    TLOGDEBUG("[SipMsg] " <<  std::string(__FUNCTION__)+"() called, gwIp:" + gwIp + ",gwPort:" + std::string(gwPort) + ",recvConnId:" + recvConnId + ", connectId1:" + connectId1 + ", connectId3:" + connectId3);

    return true;
}

bool SipMessage::getSipGatewayIpPort2(SipCloudGateway::SipDlgPtr & dlg, std::string & gwIp, int & gwPort, const std::string & recvConnId)
{
    std::string connectId1 = dlg->getSipGatewayConnectId1();
    std::string connectId3 = dlg->getCallerRouteConnectId();
    connectId3 = connectId3.empty() ? connectId1 : connectId3;

    if (recvConnId == connectId1 || recvConnId == connectId3)
    {
        gwIp = dlg->getGw2Ip();
        gwPort = dlg->getGw2Port();
    }
    else
    {
        gwIp = dlg->getGw2Ip2();
        gwPort = dlg->getGw2Port2();
    }

    TLOGDEBUG("[SipMsg] " <<  std::string(__FUNCTION__)+"() called, gwIp:" + gwIp + ",gwPort:" + std::string(gwPort) + ",recvConnId:" + recvConnId + ", connectId1:" + connectId1 + ", connectId3:" + connectId3);

    return true;
}

bool SipMessage::CreateSDPWithNewIp(const std::string & strOldSDP, const std::string & strNewIP, std::string & strNewSDP)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC stSessDes;
    SipMsgEncodePtr pstEncodeNewSDP;
    if (strNewIP.empty())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,strNewIP is EMPTY! strOldSDP:\n" + strOldSDP);
        return false;
    }
    if (!CreateSipMsgFromSDP(strOldSDP, strSipMsg))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,CreateSipMsgFromSDP() return FALSE.  strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstDecode = new SipMsgDecode();
    pstEncodeNewSDP = new SipMsgEncode();
    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,acall createReq failed . strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);
    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, decode() return FALSE. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(strNewIP.c_str()), &stIp);
    ST_SDP_OF *pstOrig = &stSessDes.stOrig;
    StSdpOrigin stSdpOrigin;
    SipMsgCommon::deSdpOrigin(*pstOrig, stSdpOrigin);
    if(Sdp_MsgSetOfX(pstEncodeNewSDP->_pstMsg->zMemBuf, pstOrig, (char*)stSdpOrigin._strUsername.c_str(), (int)stSdpOrigin._lSessId, (int)stSdpOrigin._lSessVer,  &stIp) == ZFAILED)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sdp_MsgSetOfX() failed.strOldSDP:\n" + strOldSDP);
        return false;
    }
    if (Sdp_MsgSetCf(&stSessDes.stConn, &stIp) !=  ZOK)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sdp_MsgSetCf() failed,strOldSDP:\n" + strOldSDP);
        return false;
    }
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, &stSessDes) == ZFAILED)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.strOldSDP:\n" + strOldSDP);
        return false;
    }
    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE! strOldSDP:\n" + strOldSDP);
        return false;
    }
    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos, pstEncodeNewSDP->_strMsg.size() - pos);
    return true;
}

bool SipMessage::sipFromCaller(SipCloudGateway::SipDlgPtr & sipDlg)
{
    std::string connectionId = sipDlg->getConnectionId();
    std::string connectionId1 = sipDlg->getSipGatewayConnectId1();
    std::string connectionId3 = sipDlg->getCallerRouteConnectId();
    return (connectionId == connectionId1 || connectionId == connectionId3) ? true : false;
}

bool SipMessage::decodeSipUri(const std::string & sipUri, int & type, std::string& userName, std::string& registarIp, int& port)
{
    if(sipUri.empty())
        return false;

    int pos = sipUri.find("@");
    int pos2 = sipUri.find(":");
    if (pos2 < 0)
        return false;

    std::string reqType = sipUri.substr(0, pos2);
    if (reqType == "sip")
        type = 0;
    else if (reqType == "sips")
        type = 1;
    else if (reqType == "tel")
        type = 4;

    if (type == 4)
    {
        userName = sipUri.substr(pos2 + 1, sipUri.size() - pos2);
        return true;
    }

    std::string sipUri2 = sipUri.substr(pos2 + 1, sipUri.size() - pos2);

    userName = sipUri2.substr(0, pos - pos2 - 1);
    if (type == 4)
        return true;

    int pos4 = sipUri2.find("@");
    int pos3 = sipUri2.find(":");
    if (pos4 > 0)
    {
        if(pos3 > 0)
        {
            registarIp = sipUri2.substr(pos4 + 1, pos3 - pos4 - 1);
            std::string strPort = sipUri2.substr(pos3 + 1, sipUri2.size() - pos3);
            port = atoi(strPort.c_str());
        }
        else
        {
            registarIp = sipUri2.substr(pos4 + 1, sipUri2.size() - pos4);
            port = 0;
        }
    }
    else
    {
        registarIp = sipUri.substr(pos2 + 1, sipUri.size() - pos2);
        port = 0;
    }

    if (userName.empty() || registarIp.empty())
        return false;

    return true;
}

std::string SipMessage::updateSipMsgHead(bool isCaller, SipCloudGateway::SipDlgPtr & dlg, SipMsgDecodePtr & pstSipMsgDe, EN_SIP_METHOD reqMethod, bool reqLine, bool pCalledParty, bool byeReq)
{
    bool bResult = false;
    unsigned char dstType;

    SipMsgDecodedPart *pstDecodedPart = &dlg->_sipMsgDecodePart;
    FromToPart * pFromPart = &pstDecodedPart->_fromPart;
    FromToPart * pToPart = &pstDecodedPart->_toPart;

    int fromType = 0, fromPort = 0, jegoFromType = 0, jegoFromPort = 0;
    int toType = 0, toPort = 0, jegoToType = 0, jegoToPort = 0;

    std::string fromDispalyName, fromUserName, fromHost;
    std::string toDispalyName, toUserName, toHost, contactUserName;

    std::string jegoFromDispalyName, jegoFromUserName, jegoFromHost;
    std::string jegoToDispalyName, jegoToUserName, jegoToHost;

    dlg->getCallerParams(true, fromType, fromDispalyName, fromUserName, fromHost, fromPort);
    dlg->getCallerParams(false, toType, toDispalyName, toUserName, toHost, toPort);
    dlg->getCallerParamsByJego(true, jegoFromType, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort);
    dlg->getCallerParamsByJego(false, jegoToType, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort);
    //TLOGDEBUG("[SipMsg] " << std::string(__FUNCTION__) + "() callled. fromUserName:" + fromUserName + ", fromDispalyName:" + fromDispalyName + ", fromHost:" + fromHost + ",fromPort:" + std::string(fromPort) + ", fromType:" +  std::string(fromType));
    //TLOGDEBUG("[SipMsg] " << std::string(__FUNCTION__) + "() callled. toUserName:" + toUserName + ", toDispalyName:" + toDispalyName + ", toHost:" + toHost + ",toPort:" + std::string(toPort) + ", fromType:" +  std::string(toType));
    //TLOGDEBUG("[SipMsg] " << std::string(__FUNCTION__) + "() callled. jegofromUserName:" + jegoFromUserName + ", jegofromDispalyName:" + jegoFromDispalyName + ", jegoFromHost:" + jegoFromHost + ", jegoFromPort:" + std::string(jegoFromPort) + ", jegoFromType:" +  std::string(jegoFromType));
    //TLOGDEBUG("[SipMsg] " << std::string(__FUNCTION__) + "() callled. jegoToUserName:" + jegoToUserName + ", jegoToDispalyName:" + jegoToDispalyName + ", jegoToHost:" + jegoToHost + ",jegoToPort:" + std::string(jegoToPort) + ", jegoToType:" +  std::string(jegoToType));

    std::string calleeToTag = dlg->getCalleeToTag();
    std::string callerToTag = dlg->getCallerToTag();//183
    std::string pCalledPartyHost, pCalledPartyName, toTag;

    if (isCaller)
    {
        if (pFromPart->_strUserName == fromUserName && pToPart->_strUserName == toUserName)
        {
            dstType = jegoToType;
            if (reqLine)
            {
                if (reqMethod == EN_SIP_METHOD_INVITE && !dlg->getIsFreshDialog())
                {
                    pstSipMsgDe->updateReqLine(dstType, reqMethod, jegoToUserName, jegoToHost, jegoToPort);
                }
                else if (reqMethod == EN_SIP_METHOD_ACK)
                {
                    if (!pstDecodedPart->_fromPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_fromPart._contactPart._type, reqMethod, pstDecodedPart->_fromPart._contactPart._strUserName, pstDecodedPart->_fromPart._contactPart._strHostName, pstDecodedPart->_fromPart._contactPart._iPort);
                    else if (!pstDecodedPart->_toPart._calleeContactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._calleeContactPart._type, reqMethod, pstDecodedPart->_toPart._calleeContactPart._strUserName, pstDecodedPart->_toPart._calleeContactPart._strHostName, pstDecodedPart->_toPart._calleeContactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(dstType, reqMethod, jegoToUserName, jegoToHost, jegoToPort);
                }
                else
                {
                    if (!pstDecodedPart->_toPart._calleeContactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._calleeContactPart._type, reqMethod, pstDecodedPart->_toPart._calleeContactPart._strUserName, pstDecodedPart->_toPart._calleeContactPart._strHostName, pstDecodedPart->_toPart._calleeContactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(dstType, reqMethod, jegoToUserName, jegoToHost, jegoToPort);
                }
            }

            pCalledPartyName = jegoToUserName;
            pCalledPartyHost = jegoToHost;

            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
            {
                toTag = calleeToTag;
                bResult = pstSipMsgDe->fillHdrFromToByName(false, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort, toTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }

            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
            {
                dstType = jegoFromType;
                contactUserName = jegoFromUserName;
                bResult = pstSipMsgDe->fillHdrFromToByName(true, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort, pFromPart->_strTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }
        }
        else
        {
            dstType = jegoFromType;
            if (reqLine)
            {
                if (reqMethod == EN_SIP_METHOD_INVITE && !dlg->getIsFreshDialog())
                {
                    pstSipMsgDe->updateReqLine(jegoFromType, reqMethod, jegoFromUserName, jegoFromHost, jegoFromPort);
                }
                else if (reqMethod == EN_SIP_METHOD_ACK)
                {
                    if (!pstDecodedPart->_fromPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_fromPart._contactPart._type, reqMethod, pstDecodedPart->_fromPart._contactPart._strUserName, pstDecodedPart->_fromPart._contactPart._strHostName, pstDecodedPart->_fromPart._contactPart._iPort);
                    else if (!pstDecodedPart->_toPart._calleeContactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._calleeContactPart._type, reqMethod, pstDecodedPart->_toPart._calleeContactPart._strUserName, pstDecodedPart->_toPart._calleeContactPart._strHostName, pstDecodedPart->_toPart._calleeContactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(jegoFromType, reqMethod, jegoFromUserName, jegoFromHost, jegoFromPort);
                }
                else
                {
                    if (!pstDecodedPart->_toPart._calleeContactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._calleeContactPart._type, reqMethod, pstDecodedPart->_toPart._calleeContactPart._strUserName, pstDecodedPart->_toPart._calleeContactPart._strHostName, pstDecodedPart->_toPart._calleeContactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(jegoFromType, reqMethod, jegoFromUserName, jegoFromHost, jegoFromPort);
                }
            }

            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
            {
                bResult = pstSipMsgDe->fillHdrFromToByName(false, jegoFromDispalyName, jegoFromUserName, jegoFromHost, jegoFromPort, pToPart->_strTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }
            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
            {
                contactUserName = jegoToUserName;

                dstType = jegoToType;
                bResult = pstSipMsgDe->fillHdrFromToByName(true, jegoToDispalyName, jegoToUserName, jegoToHost, jegoToPort, pFromPart->_strTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }
        }
    }
    else
    {
        if (pFromPart->_strUserName == jegoFromUserName && pToPart->_strUserName == jegoToUserName)
        {
            dstType = toType;
            if (reqLine)
            {
                if (reqMethod == EN_SIP_METHOD_INVITE && !dlg->getIsFreshDialog())
                {
                    pstSipMsgDe->updateReqLine(dstType, reqMethod, toUserName, toHost, toPort);
                }
                else if (reqMethod == EN_SIP_METHOD_ACK)
                {
                    if (!pstDecodedPart->_fromPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_fromPart._contactPart._type, reqMethod, pstDecodedPart->_fromPart._contactPart._strUserName, pstDecodedPart->_fromPart._contactPart._strHostName, pstDecodedPart->_fromPart._contactPart._iPort);
                    else if (!pstDecodedPart->_toPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._contactPart._type, reqMethod, pstDecodedPart->_toPart._contactPart._strUserName, pstDecodedPart->_toPart._contactPart._strHostName, pstDecodedPart->_toPart._contactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(dstType, reqMethod, toUserName, toHost, toPort);
                }
                else
                {
                    if (!pstDecodedPart->_toPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._contactPart._type, reqMethod, pstDecodedPart->_toPart._contactPart._strUserName, pstDecodedPart->_toPart._contactPart._strHostName, pstDecodedPart->_toPart._contactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(dstType, reqMethod, toUserName, toHost, toPort);
                }
            }

            pCalledPartyName = toUserName;
            pCalledPartyHost = toHost;

            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
            {
                toTag = callerToTag.empty() ? pToPart->_strTag : callerToTag;
                bResult = pstSipMsgDe->fillHdrFromToByName(false, toDispalyName, toUserName, toHost, toPort, toTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }

            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
            {
                contactUserName = fromUserName;
                dstType = fromType;

                bResult = pstSipMsgDe->fillHdrFromToByName(true, fromDispalyName, fromUserName, fromHost, fromPort, pFromPart->_strTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }
        }
        else
        {
            dstType = fromType;
            if (reqLine)
            {
                if (reqMethod == EN_SIP_METHOD_INVITE && !dlg->getIsFreshDialog())
                {
                    pstSipMsgDe->updateReqLine(fromType, reqMethod, fromUserName, fromHost, fromPort);
                }
                else if (reqMethod == EN_SIP_METHOD_ACK)
                {
                    if (!pstDecodedPart->_fromPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_fromPart._contactPart._type, reqMethod, pstDecodedPart->_fromPart._contactPart._strUserName, pstDecodedPart->_fromPart._contactPart._strHostName, pstDecodedPart->_fromPart._contactPart._iPort);
                    else if (!pstDecodedPart->_toPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._contactPart._type, reqMethod, pstDecodedPart->_toPart._contactPart._strUserName, pstDecodedPart->_toPart._contactPart._strHostName, pstDecodedPart->_toPart._contactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(fromType, reqMethod, fromUserName, fromHost, fromPort);
                }
                else
                {
                    if (!pstDecodedPart->_toPart._contactPart._strHostName.empty())
                        pstSipMsgDe->updateReqLine(pstDecodedPart->_toPart._contactPart._type, reqMethod, pstDecodedPart->_toPart._contactPart._strUserName, pstDecodedPart->_toPart._contactPart._strHostName, pstDecodedPart->_toPart._contactPart._iPort);
                    else
                        pstSipMsgDe->updateReqLine(fromType, reqMethod, fromUserName, fromHost, fromPort);
                }
            }

            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_TO))
            {
                bResult = pstSipMsgDe->fillHdrFromToByName(false, fromDispalyName, fromUserName, fromHost, fromPort, pToPart->_strTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }

            if(pstSipMsgDe->delHdrById(EN_SIP_HDR_FROM))
            {
                contactUserName = toUserName;
                dstType = toType;
                toTag = callerToTag.empty() ? pFromPart->_strTag : callerToTag;
                bResult = pstSipMsgDe->fillHdrFromToByName(true, toDispalyName, toUserName, toHost, toPort, toTag, dstType);
                if (!bResult)
                {
                    TLOGWARN("[SipMsg] " << std::string(__FUNCTION__) + "() callled. call fillHdrFromToByName() failed, fill From Header");
                }
            }
        }
    }

    if(!dlg->_sipRegisterSub->getAuthName().empty())
    {
        pstSipMsgDe->delHdrById(EN_SIP_HDR_CSEQ);
        if (isCaller)
        {
            if (reqMethod == EN_SIP_METHOD_BYE && byeReq)
            {
                pstSipMsgDe->fillHdrCseq(reqMethod, pstDecodedPart->_cseqPart._iCalleeCseqVal + 1);
            }
            else
            {
                pstSipMsgDe->fillHdrCseq(reqMethod, pstDecodedPart->_cseqPart._iCalleeCseqVal);
            }
        }
        else
        {
            if (reqMethod == EN_SIP_METHOD_BYE && byeReq)
            {
                pstSipMsgDe->fillHdrCseq(reqMethod, pstDecodedPart->_cseqPart._iCseqVal + 1);
            }
            else
            {
                pstSipMsgDe->fillHdrCseq(reqMethod, pstDecodedPart->_cseqPart._iCseqVal);
            }
        }
    }

    //P-Called-Party-ID
    if (pCalledParty)
    {
        std::string sipatewayId = dlg->getSipGatewayId1();
        if (!sipatewayId.empty() && !pCalledPartyHost.empty())
        {
            pstSipMsgDe->delHdrById(EN_SIP_HDR_P_CALLED_PTY_ID);
            std::string pCalledPartyID = "<sip:" + pCalledPartyName + "@" + pCalledPartyHost + ">";
            pstSipMsgDe->fillHdrExt("P-Called-Party-ID", pCalledPartyID);
        }
    }

    return contactUserName;
}
bool SipMessage::CreateSDPWithNewPort(const std::string & strOldSDP,  const vector<int> vectNewPorts,const std::string & strNewIP,  std::string & strNewSDP)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC stSessDes;
    SipMsgEncodePtr pstEncodeNewSDP ;
    if (strNewIP.empty())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,strNewIP is EMPTY! strOldSDP:\n" + strOldSDP);
        return false;
    }
    if (!CreateSipMsgFromSDP(strOldSDP, strSipMsg))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,CreateSipMsgFromSDP() return FALSE.  strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstDecode = new SipMsgDecode();
    pstEncodeNewSDP = new SipMsgEncode();
    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,acall createReq failed . strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);
    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, decode() return FALSE. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    if (stSessDes.stMdescLst.iCount != vectNewPorts.size())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,stSessDes.stMdescLst.dwCount != vectNewPorts.size(). strOldSDP:\n" + strOldSDP);
        return false;
    }
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(strNewIP.c_str()), &stIp);
    ST_SDP_OF *pstOrig = &stSessDes.stOrig;
    StSdpOrigin stSdpOrigin;
    SipMsgCommon::deSdpOrigin(*pstOrig, stSdpOrigin);
    if(Sdp_MsgSetOfX(pstEncodeNewSDP->_pstMsg->zMemBuf, pstOrig, (char*)stSdpOrigin._strUsername.c_str(), (int)stSdpOrigin._lSessId, (int)stSdpOrigin._lSessVer,  &stIp) == ZFAILED)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sdp_MsgSetOfX() failed.strOldSDP:\n" + strOldSDP);
        return false;
    }
    if (Sdp_MsgSetCf(&stSessDes.stConn, &stIp) !=  ZOK)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sdp_MsgSetCf() failed,strOldSDP:\n" + strOldSDP);
        return false;
    }
    if (stSessDes.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstrMNode;
        ST_SDP_MDESC *pstMdesc;
        ZVOID * pstMdescVoidPtr;
        int index = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&stSessDes.stMdescLst, pstrMNode, pstMdescVoidPtr)
        {
            pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
            if(pstMdesc == NULL)
            {
                break;
            }
            int temPort = vectNewPorts.at(index);
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
                        Sdp_MsgSetCf(pstCf, &stIp);
                    }
                }
            }
            index++;
        }
    }
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, &stSessDes) == ZFAILED)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.strOldSDP:\n" + strOldSDP);
        return false;
    }
    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE! strOldSDP:\n" + strOldSDP);
        return false;
    }
    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos,  pstEncodeNewSDP->_strMsg.size() - pos);
    return true;
}
bool SipMessage::CreateSDPWithNewPort(const std::string & strOldSDP, int newAudioPort, int newVideoPort, const std::string & strNewIP,  std::string & strNewSDP)
{
    std::string strSipMsg;
    SipMsgDecodePtr pstDecode;
    ST_SDP_SESS_DESC_LST stSessDesLst;
    ST_SDP_SESS_DESC    stSessDes;
    SipMsgEncodePtr pstEncodeNewSDP ;
    if (strNewIP.empty())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,strNewIP is EMPTY! " );
        return false;
    }
    if (!CreateSipMsgFromSDP(strOldSDP, strSipMsg))
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,CreateSipMsgFromSDP() return FALSE.  strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstDecode = new SipMsgDecode();
    pstEncodeNewSDP = new SipMsgEncode();
    if (!pstEncodeNewSDP->createReq())
    {
        TLOGWARN("[SipMsg] " <<std::string(__FUNCTION__)+"() called,acall createReq failed . strOldSDP :\n." + strOldSDP);
        return false;
    }
    pstEncodeNewSDP->fillReqLineBySip(EN_SIP_METHOD_INVITE,"13957854053","gd.ims.mnc000.mcc460.3gppnetwork.org",0);
    unsigned char ucResult = pstDecode->decode(strSipMsg, strSipMsg.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  decode() return FALSE. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDesLst = pstDecode->_pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    if (stSessDesLst.iCount < 1)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, stSessDesLst.dwCount < 1. strOldSDP :\n." + strOldSDP);
        return false;
    }
    stSessDes =  *stSessDesLst.pstHead->pData;                  //目前只支持一个音频，即使存在视频，也只有一个会话描述，因此取第一个会话描述
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(strNewIP.c_str()), &stIp);
    ST_SDP_OF *pstOrig = &stSessDes.stOrig;
    ST_ZOS_SSTR pcUserName = stSessDes.stOrig.stUserName;
    std::string userName;
    SipMsgCommon::zosSstr2Str(pcUserName ,userName);
    ZUINT iSessId = stSessDes.stOrig.iSessId;
    ZUINT iSessVer = stSessDes.stOrig.iSessVer;
    if(Sdp_MsgSetOfX(pstEncodeNewSDP->_pstMsg->zMemBuf, pstOrig, (ZCHAR *)userName.c_str(), iSessId, iSessVer, &stIp) == ZFAILED)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sdp_MsgSetOfX() failed.");
        return false;
    }
    if (Sdp_MsgSetCf(&stSessDes.stConn, &stIp) !=  ZOK)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sdp_MsgSetCf() failed.");
        return false;
    }
    if (stSessDes.stMdescLst.iCount > 0)
    {
        ST_ABNF_LIST_NODE *pstrMNode;
        ST_SDP_MDESC *pstMdesc;
        ZVOID * pstMdescVoidPtr;
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
    if(Sip_MsgFillBodySdp(pstEncodeNewSDP->_pstMsg, &stSessDes) == ZFAILED)
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, Sip_MsgFillBodySdp() return FALSE!.");
        return false;
    }
    if (!pstEncodeNewSDP->encode())
    {
        TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called, pstEncodeNewSDP->encode() return FALSE!");
        return false;
    }
    int pos =  pstEncodeNewSDP->_strMsg.find("v=");
    strNewSDP =  pstEncodeNewSDP->_strMsg.substr(pos,  pstEncodeNewSDP->_strMsg.size() - pos);
    return true;
}

bool SipMessage::GetIpPortFromSDPConnField(const std::string & strCsSDP, std::string & ip, int & port)
{
    vector<SDPMediaInfo>  vectSdpMediaInfo;
    std::string strSipMsgWithSDP;
    std::string reason;
    try
    {
        if (!SipMessage::CreateSipMsgFromSDP(strCsSDP, strSipMsgWithSDP))
        {
            reason = "SipMsg::CreateSipMsgFromSDP() return FALSE";
            throw false;
        }
        if (!SipMessage::getMediaIpAndPortsOfSDPinSipMsg(strSipMsgWithSDP, vectSdpMediaInfo))
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
        SDPMediaPort ports = infos._vecMediaPort.at(0);
        port = ports._port;
        throw true;
    }
    catch (bool ex)
    {
        if (false == ex)
        {
            TLOGWARN("[SipMsg] " << std::string(__FUNCTION__)+"() called,  return FALSE!  reason:" + reason + ",sdp:\n" + strCsSDP);
        }
        else
        {
            TLOGDEBUG("[SipMsg] " << std::string(__FUNCTION__)+"() called,  return TRUE!  ,got ip:" + ip + ",sdp:\n" + strCsSDP);
        }
        return ex;
    }
}
bool SipMessage::getMediaIpAndPortsOfSDPinSipMsg(const std::string & strSipMsgWithSDP,  vector<SDPMediaInfo> & vectSdpMediaInfo)
{
    SipMsgDecodePtr pstSipMsgDe = new SipMsgDecode();
    return pstSipMsgDe->getSBCMediaIpAndPort(strSipMsgWithSDP, vectSdpMediaInfo);
}

bool SipMessage::authCalcRsp(const SipMsgDecodePtr & pstSipMsgDe, const SipProxy::SipRegisterSubscribePtr & pSipReg, unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp)
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

    TLOGINFO("[SipMsg] " << "authCalcRsp() called. pstSipMsgEn strUsername:" + strUsername + ",strPassword:" + strPassword
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
    TLOGINFO("[SipMsg] " << "authCalcRsp() called. qopExist:" + std::string(qopExist) + ",Cnonce:" + temCNonce + ",acKDMd5Str:" + acKDMd5Str);

    std::string tem1 = acCnonce;
    pSipReg->setDigestCNonce(temCNonce);
    pSipReg->setDigestResponse(acKDMd5Str);

    //fill response
    pstSipMsgDe->authDRspFillRealm(pstRsp, temRealm);
    pstSipMsgDe->authDRspFillNonce(pstRsp, temNonce);
    pstSipMsgDe->authDRspFillAlgo(pstRsp, algoType);
    if (qopExist)
    {
        pstSipMsgDe->authDRspFillQop(pstRsp,  qopType);
    }
    if (opaque.empty() == false)
    {
        pstSipMsgDe->authDRspFillQpaque(pstRsp, opaque);
    }

    /* fill param nonce to header Authorization */
    pstSipMsgDe->authDRspFillUsername(pstRsp, strUsername);
    pstSipMsgDe->authDRspFillUri(pstRsp, pstReqUri);

    if (strPassword.empty())
    {
        /* fill param response to header Authorization */
        pstSipMsgDe->authDRspFillRsp(pstRsp, "");
    }
    else
    {
        /* fill param response to header Authorization */
        pstSipMsgDe->authDRspFillRsp(pstRsp, acKDMd5Str);
    }

    if (qopExist && (qopType == EN_SIP_QOP_VAL_AUTH || qopType == EN_SIP_QOP_VAL_AUTH_INT))
    {
        pstSipMsgDe->authDRspFillCnonce(pstRsp, &stCnonce);
        pstSipMsgDe->authDRspFillNonceCount(pstRsp, &stNonceCount);
    }

    return true;
}

bool SipMessage::authCalcRsp(const SipMsgDecodePtr & pstSipMsgDe, const std::string &strUsername, const std::string &strPassword,  unsigned char ucMethod, ST_SIP_DIGEST *pstDigest, ST_SIP_REQ_URI *pstReqUri, ST_SIP_DIGEST_RSP *pstRsp)
{
    std::string strRealm, strUri, strNonce, strCnonce, strMethod, strNonceCount, strQop;
    std::string strTemp, strA1, strA2, strKd;
    ST_ZOS_SSTR *pstRealm, *pstNonce;

    /* create a rand string of cnonce */
    strCnonce = TC_UUIDGenerator::getInstance()->genID();

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
    pstSipMsgDe->authDRspFillNonce(pstRsp, strNonce);

    /* fill param response to header Authorization */
    pstSipMsgDe->authDRspFillRsp(pstRsp, strKd);

    /*  */
    pstSipMsgDe->authDRspFillUri(pstRsp, pstReqUri);

    /*  */
    pstSipMsgDe->authDRspFillUsername(pstRsp, strUsername);

    return true;
}

/* calculate A1 */
bool SipMessage::calcA1(const std::string &strUsername, const std::string &strPassword,
                    const std::string &strRealm, std::string &strA1)
{
    std::string strTemp = strUsername + ":" + strRealm + ":" + strPassword;
    strA1 = Common::md5(strTemp);
    return true;
}

/* calculate A2 */
bool SipMessage::calcA2(unsigned char ucMethod, ST_SIP_REQ_URI *pstReqUri,
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
    strA2 = Common::md5(strTemp);
    return true;
}

/* calculate KD */
bool SipMessage::calcKd(const std::string &strA1, const std::string &strA2,
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
    strKd = Common::md5(strTemp);

    return true;
}

bool SipMessage::updateSdpWithIp(const std::string& oldSdp, const std::string& remoteIp, const std::string& sdpIp, std::string& sdpNew)
{
    if (oldSdp.empty() || remoteIp.empty() || sdpIp.empty())
        return false;

    bool isRemoteIpPrivate = privateIpCheck(remoteIp);
    bool isSdpIpPrivate = privateIpCheck(sdpIp);
    if ((!isRemoteIpPrivate && isSdpIpPrivate) || (isRemoteIpPrivate && !isSdpIpPrivate))
        return CreateSDPWithNewIp(oldSdp, remoteIp, sdpNew);

    return false;
}

bool SipMessage::privateIpCheck(const std::string& remoteIp)
{
    if (remoteIp.empty())
        return false;

    int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
    sscanf(remoteIp.c_str(), "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);

    return (ip1 == 10) || (ip1 == 172 && ip2 >= 16 && ip2 <= 31) || (ip1 == 192 && ip2 == 168);
}

std::string SipMessage::getConnId(const std::string& remoteIp, int remotePort, const std::string& routeIp, int routePort,
                                     const std::string& contactIp, int contactPort)
{
    if (!routePort)
        routePort = 5060;

    if (!contactPort)
        contactPort = 5060;

    bool isRemotePrivate = privateIpCheck(remoteIp);
    bool isRoutePrivate = privateIpCheck(routeIp);
    bool isContactPrivate = privateIpCheck(contactIp);

    if (isRemotePrivate)
    {
        if (!routeIp.empty() && isRoutePrivate)
            return routeIp + ":" + std::string(routePort);

        if (!contactIp.empty() && isContactPrivate)
            return contactIp + ":" + std::string(contactPort);

        return remoteIp + ":" + std::string(remotePort);
    }

    if (!routeIp.empty() && !isRoutePrivate)
        return routeIp + ":" + std::string(routePort);

    if (!contactIp.empty() && !isContactPrivate)
        return contactIp + ":" + std::string(contactPort);

    return remoteIp + ":" + std::string(remotePort);
}
