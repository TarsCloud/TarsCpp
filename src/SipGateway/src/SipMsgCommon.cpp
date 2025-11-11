#include "sip.h"
#include "SipMsgCommon.h"
#include "util/tc_timeprovider.h"
#include "util/tc_uuid_generator.h"

#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG //重载new
#define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include <stdlib.h>
//#include <crtdbg.h>

/* create a new data buffer */
void * SipMsgCommon::dbufCreate(int iDftBlkSize)
{
    return Zos_DbufCreate(ZNULL, ZDBUF_TYPE_STRUCT, iDftBlkSize);
}

/* delete data buffer */
void SipMsgCommon::dbufDelete(void *zBuf)
{
    Zos_DbufDelete(zBuf);
}

/* print string */
bool SipMsgCommon::speak(const std::string &str)
{
    printf("%s\r\n", str.c_str());
    return true;
}

/* transform ST_ZOS_SSTR to std::string */
bool SipMsgCommon::zosSstr2Str(const ST_ZOS_SSTR &stSrc, std::string &strDst)
{
    strDst.clear();
    strDst.append(stSrc.pcStr, stSrc.iLen);
    return true;
}

bool SipMsgCommon::zosUstr2Str(const ST_ZOS_USTR & stSrc,std::string & strDst)
{
    strDst.clear();
    strDst.append((char*)stSrc.pucStr, stSrc.iLen);
    return true;
}


/* transform std::string to ST_ZOS_SSTR */
bool SipMsgCommon::str2ZosSstr(const std::string &strSrc, ST_ZOS_SSTR &stDst)
{
    stDst.iLen = strSrc.size();
    stDst.pcStr = (ZCHAR *)strSrc.c_str();
    return true;
}

/* transform sip msg type to std::string */
std::string SipMsgCommon::sipMsgType2String(unsigned char ucSipMsgType)
{
    switch(ucSipMsgType)
    {
    case EnSipMsgType_Invalid:
        return std::string("Invalid Message Type");
    case EnSipMsgType_Incomplete:
        return std::string("Incomplete Message, Wait More ...");
    case EnSipMsgType_PingRFC5626:
        return std::string("Sip Ping defined in RFC5626");
    case EnSipMsgType_PongRFC5626:
        return std::string("Sip Pong defined in RFC5626");
    case EnSipMsgType_Request:
        return std::string("Sip Request");
    case EnSipMsgType_RegRsp401:
        return std::string("Sip Response 401");
    case EnSipMsgType_RegRsp200:
        return std::string("Sip Register Response 200");
    case EnSipMsgType_RegRspOther:
        return std::string("Sip Register Response");
    case EnSipMsgType_Response:
        return std::string("Sip Response");
    default:
        return std::string("Unkown Message Type");
    }
}

bool SipMsgCommon::SipMsg_Init(int fileSize, int fileCount, bool bLicense)
{
    ZINT result = false;

    std::string curDir =  SipMsgCommon::getCurrentDir();
    Zos_CfgSetLogDir((char*)curDir.c_str());
    Zos_CfgSetLogFileSize(fileSize);
    Zos_CfgSetLogFileCount(fileCount);
    Zos_CfgSetTimerCount(OCS_TIMER_MAX_COUNT);
    TLOGINFO("[SipMsg_Init() called.] " << "log dir:" + curDir << endl);

    if(bLicense)
        result = Zos_SysInit();
    else
        result = Zos_SysInitX();

    if (result != ZOK)
    {
        return false;
    }

    Zos_CfgSetLogLevel(ZLOG_LEVEL_ALL);

    //暂时注释掉XML的部分，libAvatar.a 好像有问题
    //Xml_Init();

    result = Sdp_AbnfInit();
    if (result != ZOK)
    {
        return false;
    }

    result = Sip_AbnfInit();
    if (result != ZOK)
    {
        return false;
    }
    return true;
}

void SipMsgCommon::SipMsg_Destory()
{
    Zos_SysDestroy();
}

/* msf make endpoint uri by addr-spec */
ZINT SipMsgCommon::SipMsg_UriByAddrSpec(ZUBUF zBufId, ST_ZOS_SSTR *pstUri,
                                        ST_SIP_ADDR_SPEC *pstAddrSpec)
{
    ZDBUF zMsgBuf;
    ZUCHAR ucHdrsPres;

    if (!zBufId || !pstUri || !pstAddrSpec)
        return ZFAILED;

    /* save old header present flag */
    ucHdrsPres = pstAddrSpec->u.stSipUri.ucHdrsPres;

    /* clear header present flag to make corrent uri */
    if (ucHdrsPres) pstAddrSpec->u.stSipUri.ucHdrsPres = ZFALSE;

    /* encode addr spec */
    if (Abnf_AnyEncodeX(ZPROTOCOL_SIP, (PFN_ABNFENCODE)Sip_EncodeAddrSpec,
        pstAddrSpec, ZFALSE, &zMsgBuf) != ZOK)
        return ZFAILED;

    /* get the uri string */
    Zos_UbufCpyDSStr(zBufId, zMsgBuf, pstUri);

    /* restore header present flag */
    pstAddrSpec->u.stSipUri.ucHdrsPres = ucHdrsPres;

    /* delete the buffer */
    Zos_DbufDelete(zMsgBuf);
    return ZOK;
}

/* msf make endpoint uri by name-addr or addr-spec */
ZINT SipMsgCommon::SipMsg_UriByNaSpec(ZUBUF zBufId, ST_ZOS_SSTR *pstUri,
                                      ST_SIP_NA_SPEC *pstNaSpec)
{
    ZINT iRet;
    if (!zBufId || !pstUri || !pstNaSpec)
        return ZFAILED;

    if (pstNaSpec->ucNameAddrPres)
        iRet = SipMsg_UriByAddrSpec(zBufId, pstUri, &pstNaSpec->u.stNameAddr.stAddrSpec);
    else
        iRet = SipMsg_UriByAddrSpec(zBufId, pstUri, &pstNaSpec->u.stAddrSpec);
    return iRet;
}

std::string SipMsgCommon::sipMethod2String(unsigned char ucMethod)
{
    return std::string(Sip_GetMethodDesc(ucMethod));
}

ZINT SipMsgCommon::SipMsg_Decode(ZCHAR *pcMsg, ZUINT iMsgLen, ST_SIP_MSG **ppstMsg)
{
    ST_ABNF_ERR_INFO stErrInfo;
    ST_ABNF_MSG stAbnfMsg;
    ST_ZOS_SSTR stData;
    ST_SIP_MSG *pstMsg;
    ZDBUF zMsgBuf = ZNULL, zMemBuf;

    *ppstMsg = ZNULL;

    /* get the data length */
    stData.iLen = iMsgLen;
    stData.pcStr = pcMsg;

    /* create the memory buffer */
    SIP_CREATE_MEMBUFD(zMemBuf, sizeof(ST_SIP_MSG), pstMsg);
    if (!zMemBuf)
    {
        TLOGERROR("[Druing SipMsg_Decode] " << "Create Memory failed!" << endl);
        //SIP_DBUF_DELETE(zMsgBuf);
        return ZFAILED;
    }
    /* init the error info */
    Abnf_ErrInit(&stErrInfo);
    /* init the abnf message */
    Abnf_MsgInitL(&stAbnfMsg, ZPROTOCOL_SIP, &stData, zMemBuf, &stErrInfo, ZTRUE);

    /* decode message */
    if (Sip_DecodeMsg(&stAbnfMsg, pstMsg) != ZOK)
    {
        Abnf_ErrLogPrint(&stAbnfMsg, (char*)"SIP");
        Abnf_ErrDestroy(&stErrInfo);
        SIP_DBUF_DELETE(zMemBuf);
        //SIP_DBUF_DELETE(zMsgBuf);
        TLOGERROR("[Druing SipMsg_Decode] " << "call Sip_DecodeMsg failed!" << endl);
        return ZFAILED;
    }

    /* destroy error info */
    Abnf_ErrDestroy(&stErrInfo);

    /* set message buffer */
    //pstMsg->zMsgBuf = zMsgBuf;

    /* set the sip message */
    *ppstMsg = pstMsg;

    return ZOK;
}

ZINT SipMsgCommon::SipMsg_Encode(ST_SIP_MSG *pstMsg, ZCHAR *pcBuf, ZUINT & iBufLen)
{
    ST_ABNF_ERR_INFO stErrInfo;
    ST_ABNF_MSG stAbnfMsg;
    ZINT iRet = ZFAILED;
    ZDBUF zMsgBuf;
    ZUINT iLen;

    /* create new message buffer */
    SIP_CREATE_MSGBUF(zMsgBuf);
    if (zMsgBuf == ZNULL)
    {
        TLOGERROR("[SipGateway] " << "SipMsgCommon::SipMsg_Encode() called , zMsgBuf == ZNULL. FALSE." << endl);
        return ZFAILED;
    }
    /* initialize the error info */
    Abnf_ErrInit(&stErrInfo);

    /* init the abnf message */
    if (Abnf_MsgInit(&stAbnfMsg, ZPROTOCOL_SIP, ZNULL, zMsgBuf, &stErrInfo, ZFALSE) != ZOK)
    {
        TLOGERROR("[SipGateway] " << "SipMsgCommon::SipMsg_Encode() called , Abnf_MsgInit() return FALSE." << endl);

        SIP_DBUF_DELETE(zMsgBuf);
        Abnf_ErrDestroy(&stErrInfo);
        return ZFAILED;
    }

    /* enocde message */
    if (Sip_EncodeMsg(&stAbnfMsg, pstMsg) != ZOK)
    {
        TLOGERROR("[SipGateway] " << "SipMsgCommon::SipMsg_Encode() called , Sip_EncodeMsg() return FALSE." << endl);

        SIP_DBUF_DELETE(zMsgBuf);
        Abnf_ErrLogPrint(&stAbnfMsg, (char*)"SIP");
        Abnf_ErrDestroy(&stErrInfo);
        return ZFAILED;
    }

    /* destroy error info */
    Abnf_ErrDestroy(&stErrInfo);

    iLen = Zos_DbufLen(zMsgBuf);
    if (iLen < iBufLen)
    {
        ZCHAR *pcData;

        if (!Zos_DbufIsFlat(zMsgBuf))
        {
            ZDBUF zFlatBuf = Zos_DbufFlat(zMsgBuf);
            SIP_DBUF_DELETE(zMsgBuf);
            zMsgBuf = zFlatBuf;
        }

        Zos_DbufO2D(zMsgBuf, 0, &pcData);
        Zos_MemCpy(pcBuf, pcData, Zos_DbufLen(zMsgBuf));
        iBufLen = iLen;
        iRet = ZOK;
    }
    else
    {
        TLOGERROR("[SipGateway] " << "SipMsgCommon::SipMsg_Encode() called , iLen >= iBufLen :" + std::to_string((int)iLen) + "  > " + std::to_string((int) iBufLen) << endl);
    }

    pstMsg->zMsgBuf = ZNULL;
    SIP_DBUF_DELETE(zMsgBuf);
    if (iRet != ZOK)
    {
        TLOGERROR("[SipGateway] " << "SipMsgCommon::SipMsg_Encode() called , the last return FALSE." << endl);
    }
    return iRet;
}

void SipMsgCommon::printSipMsg(ST_SIP_MSG *pstMsg)
{
    if(pstMsg == NULL)
        return;

    std::string info;
    // get type
    {
        if (pstMsg->ucReqPres)
        {
            if (pstMsg->u.stReqLine.stMethod.ucType == EN_SIP_METHOD_INVITE)
                //type = SipMsg_RequestInvite;
                info += "===> Sip Mssage Type : INVITE\n";
            else if (pstMsg->u.stReqLine.stMethod.ucType == EN_SIP_METHOD_CANCEL)
                //type = SipMsg_RequestCancel;
                info += "===> Sip Mssage Type : CANCEL\n";
            else
                //type =SipMsg_Request;
                info += "===> Sip Mssage Type : Request\n";
        }
        else
        {
            //type = SipMsg_Response;
            info += "==> Sip Mssage Type : Response\n";
        }
    }

    // get uri
    {
        ST_SIP_NA_SPEC *pstNaSpec;
        if (Sip_MsgGetFromToNaSpec(pstMsg, pstMsg->ucReqPres, &pstNaSpec) == ZOK)
        {
            ST_ZOS_SSTR stUri;
            SipMsg_UriByNaSpec(pstMsg->zMemBuf, &stUri, pstNaSpec);
            //Zos_NStrNCpy(outLocal, outLocalBufSize, stUri.pcStr, stUri.wLen);

            std::string uri;
            uri.assign(stUri.pcStr, stUri.iLen);

            info += "====> Uri : " + uri + "\n";
        }
    }

    // get device
    {
        ST_SIP_HDR_EXT_HDR *pstHdr;
        if (Sip_FindExtHdr(pstMsg, (char*)"X-device", &pstHdr) == ZOK)
        {
            //Zos_NStrNCpy(outSession, outSessionBufSize, pstHdr->stVal.pcStr, pstHdr->stVal.wLen);

            std::string device;
            device.assign(pstHdr->stVal.pcStr, pstHdr->stVal.iLen);

            info += "====> Device : " + device + "\n";
        }
    }

    // get callid
    {
        ST_ZOS_SSTR *pstVal;
        if (Sip_FindMsgHdrVal(pstMsg, EN_SIP_HDR_CALL_ID, &pstVal) == ZOK)
        {
            //Zos_NStrNCpy(outCallid, outCallidBufSize, pstVal->pcStr, pstVal->wLen);
            std::string callid;
            callid.assign(pstVal->pcStr, pstVal->iLen);

            info += "====> Call-ID : " + callid + "\n";
        }
    }

    // get media type
    {
        ST_SDP_SESS_DESC *pstSdp;
        if (Sip_MsgGetBodySdp(pstMsg, &pstSdp) == ZOK)
        {
            ST_ABNF_LIST_NODE *pstMNode;
            ST_SDP_MDESC *pstMdesc;
            ZVOID * pstMdescVoidPtr;

            int index = 0;
            FOR_ALL_DATA_IN_ABNF_LIST(&pstSdp->stMdescLst, pstMNode, pstMdescVoidPtr)
            {
                pstMdesc = (ST_SDP_MDESC *)pstMdescVoidPtr;
                if(pstMdesc == NULL)
                {
                    break;
                }

                if (pstMdesc->stMedia.ucMediaType == EN_SDP_MEDIA_AUDIO)
                {
                    info += "====> Media #" + std::to_string(index) + ": Audio, Port=" + std::to_string(pstMdesc->stMedia.wPort) + "\n";
                }
                else if (pstMdesc->stMedia.ucMediaType == EN_SDP_MEDIA_VIDEO)
                {
                    info += "====> Media #" + std::to_string(index) + ": Video, Port=" + std::to_string(pstMdesc->stMedia.wPort) + "\n";
                }

                index++;
            }
        }
    }

    TLOGINFO("[SipGateway] " << info << endl);
    speak(info);
}

/* print ST_SDP_SESS_DESC */
void SipMsgCommon::printSipSdp(ST_SDP_SESS_DESC *pstSdp)
{
    speak("this method is empty");
}

/* print ST_SDP_SESS_DESC */
void SipMsgCommon::printSipSdp(StSdpSessDesc stSdpDesc)
{
    /* struct of sdp session description */
    typedef struct StSdpSessDesc
    {
        vector<StSdpMediaDesc> _stMediaVec;
        vector<StSdpAttr> _stAttrVec;
    } StSdpSessDesc;

    speak("version: " + std::to_string(stSdpDesc._iVer));
    speak("session name: " + stSdpDesc._strSessName);
    speak("info: " + stSdpDesc._strInfo);
    speak("uri: " + stSdpDesc._strUri);
    /* print origin */
    speak("origin nettype: " + std::to_string(stSdpDesc._stOrigin._ucNetType));
    speak("origin addrtype: " + std::to_string(stSdpDesc._stOrigin._ucAddrType));
    speak("origin username: " + std::string(stSdpDesc._stOrigin._strUsername));
    speak("origin session id: " + std::to_string(stSdpDesc._stOrigin._lSessId));
    speak("origin session version: " + std::to_string(stSdpDesc._stOrigin._lSessVer));
    speak("origin addr: " + std::string(stSdpDesc._stOrigin._strAddr));
    /* print connection */
    speak("connection nettype: " + std::to_string(stSdpDesc._stConnInfo._ucNetType));
    speak("connection addrtype: " + std::to_string(stSdpDesc._stConnInfo._ucAddrType));
    speak("connection addr: " + std::string(stSdpDesc._stConnInfo._strAddr));
    /* print time */
    speak("start time: " + std::to_string(stSdpDesc._stTimeDesc._lStartTime));
    speak("stop time: " + std::to_string(stSdpDesc._stTimeDesc._lStopTime));
    /* print media */
    vector<StSdpMediaDesc>::iterator it0;
    for(it0 = stSdpDesc._stMediaVec.begin();
        it0 != stSdpDesc._stMediaVec.end();
        it0++)
    {
        printSdpMediaDesc(*it0);
    }
    /* print attribution */
    vector<StSdpAttr>::iterator it1;
    for(it1 = stSdpDesc._stAttrVec.begin();
        it1 != stSdpDesc._stAttrVec.end();
        it1++)
    {
        printSdpAttr(*it1);
    }
}

/* print StSdpMediaDesc */
void SipMsgCommon::printSdpMediaDesc(StSdpMediaDesc stMediaDesc)
{
    speak("media type: " + std::to_string(stMediaDesc._ucMediaType));
    speak("media proto type: " + std::to_string(stMediaDesc._ucProtoType));
    speak("media port: " + std::to_string(stMediaDesc._iPort));
}

/* print StSdpAttr */
void SipMsgCommon::printSdpAttr(StSdpAttr stAttr)
{
    speak("thie method is empty");
}

bool SipMsgCommon::printSdpSessDesc(ST_SDP_SESS_DESC *pstMsg)
{
    if(!pstMsg)
    {
        speak("print ST_SDP_SESS_DESC failed, pstMsg is null");
        return false;
    }
    speak("ucPres: " + std::to_string(pstMsg->ucPres));

    speak("version: " + std::to_string((int)(pstMsg->stVer.wVer)));

    speak("origin usPres: " + std::to_string(pstMsg->stOrig.ucPres));
    speak("origin net type: " + std::to_string(pstMsg->stOrig.ucNetType));
    speak("origin addr type: " + std::to_string(pstMsg->stOrig.ucAddrType));
    speak("origin addr: " + std::to_string((int64_t)(pstMsg->stOrig.stAddr.u.iIpv4)));
    std::string strUsername;
    zosSstr2Str(pstMsg->stOrig.stUserName, strUsername);
    speak("origin username: " + strUsername);

    ST_SDP_SF stSf = pstMsg->stSessName;
    std::string strSessname;
    zosSstr2Str(stSf.stSessName, strSessname);
    speak("session name: " + strSessname);

    ST_SDP_TF *pstTf = (ST_SDP_TF *)ABNF_LIST_HEAD_DATA(&(pstMsg->stSessTime.stTimeLst));
    if(pstTf)
    {
        speak("startTime: " + std::to_string((int)(pstTf->iStartTime)));
        speak("stopTime: " + std::to_string((int)(pstTf->iStopTime)));
    }

    ST_SDP_MDESC *pstMdesc = (ST_SDP_MDESC *)ABNF_LIST_HEAD_DATA(&pstMsg->stMdescLst);
    if(pstMdesc)
    {
        speak("media type: " + std::to_string(pstMdesc->stMedia.ucMediaType));
        speak("media proto type: " + std::to_string(pstMdesc->stMedia.ucProtoType));
        speak("media port: " + std::to_string(pstMdesc->stMedia.wPort));
        speak("media format: ");
        ST_SDP_FMT *pstFmt;
        ZVOID *pstVoid;
        ST_ABNF_LIST_NODE *pstNode;
        FOR_ALL_DATA_IN_ABNF_LIST(&(pstMdesc->stMedia.stFmtLst), pstNode, pstVoid)
        {
            pstFmt = (ST_SDP_FMT *)pstVoid;
            speak(std::string(pstFmt->pcStr));
        }
    }

    return true;
}

bool SipMsgCommon::enUserInfo(const std::string &strUserInfo, const std::string &strPassword,
                              ST_SIP_USER_INFO &stUserInfo)
{
    if(strUserInfo.empty())
    {
        return false;
    }
    stUserInfo.ucPres = 1;
    str2ZosSstr(strUserInfo, stUserInfo.stUser);
    if(!strPassword.empty())
    {
        stUserInfo.ucPasswdPres = 1;
        str2ZosSstr(strPassword, stUserInfo.stPasswd);
    }
    return true;
}

bool SipMsgCommon::deUserInfo(ST_SIP_USER_INFO *pstUserInfo, std::string &strUserInfo,
                              std::string &strPassword)
{
    if(!pstUserInfo)
    {
        return false;
    }
    if(pstUserInfo->ucPres)
    {
        zosSstr2Str(pstUserInfo->stUser, strUserInfo);
        if(pstUserInfo->ucPasswdPres)
        {
            zosSstr2Str(pstUserInfo->stPasswd, strPassword);
        }
        return true;
    }
    return false;
}

bool SipMsgCommon::enHostPort(const std::string &strHostname, const int iPort,
                              ST_SIP_HOST_PORT &stHostPort)
{
    /* check strHostname */
    if(strHostname.empty()) return false;

    if(Sip_HostPortByNameL(&stHostPort, (ZCHAR *)strHostname.c_str(), iPort) == ZFAILED)
        return false;
    return true;
}


/**
* @encode ST_SIP_HOST_PORT.
*
* @param [in] strHostname.
* @param [in] iPort, if iPort == 0, there is no iPort.
*
* @param [out] stHostPort.
*/
bool SipMsgCommon::enHostPort(const std::string &strHostname, const int iPort,
                              ST_SIP_HOST_PORT *pstHostPort)
{
    if (strHostname.empty())
        return false;

    pstHostPort->ucPres = 1;

    /* set hostname */
    pstHostPort->stHost.ucType = EN_SIP_HOST_NAME;
    SipMsgCommon::str2ZosSstr(strHostname, pstHostPort->stHost.u.stName);

    /* set iPort */
    if(iPort > 0)
    {
        pstHostPort->ucPortPres = 1;
        pstHostPort->iPort = iPort;
    }
    return true;
}


/* decode ST_SIP_HOST_PORT */
bool SipMsgCommon::deHostPort(ST_SIP_HOST_PORT *pstHostPort, std::string &strHostname,
                              int &iPort)
{
    if(!pstHostPort)
    {
        return false;
    }
    if(pstHostPort->ucPres)
    {
        deSipHost(&pstHostPort->stHost,  strHostname);
        //    switch (pstHostPort->stHost.ucType)
        //    {
        //    case EN_SIP_HOST_NAME:
        //        {
        //            zosSstr2Str(pstHostPort->stHost.u.stName, strHostname);
        //            break;
        //        }
        //    case EN_SIP_HOST_IPV4:
        //        {
        ////iIpv4Addr
        //ZCHAR * pStrIpv4 = NULL;
        //
        //Zos_InetNtoa(pstHostPort->stHost.u.iIpv4Addr, &pStrIpv4);
        //std::string strTem(pStrIpv4);
        //strHostname = strTem;
        //            break;
        //        }
        //    case EN_SIP_HOST_IPV6:
        //        {
        ////ipv6
        //ZCHAR TEMiPV6[ZINET_IPV6_ADDR_SIZE + 1];
        //Zos_InetNtop(ZINET_IPV6, pstHostPort->stHost.u.aucIpv6Addr, TEMiPV6, ZINET_IPV6_ADDR_SIZE + 1);
        //strHostname = TEMiPV6;
        //            break;
        //        }
        //    }
        iPort = pstHostPort->iPort;
        return true;
    }
    return false;
}

bool SipMsgCommon::deSipHost(ST_SIP_HOST *pstSipHost, std::string &strHostname)
{
    if(!pstSipHost)
    {
        return false;
    }
    ZCHAR acRmtAddr[ZINET_IPV4_STR_SIZE] = {0};
    switch (pstSipHost->ucType)
    {
    case EN_SIP_HOST_NAME:
        {
            zosSstr2Str(pstSipHost->u.stName, strHostname);
            break;
        }
    case EN_SIP_HOST_IPV4:
        {
            //iIpv4Addr
            ZCHAR * pStrIpv4 = NULL;
            Zos_InetNtop(ZINET_IPV4, &pstSipHost->u.iIpv4Addr, acRmtAddr, ZINET_IPV4_STR_SIZE);
            //Zos_InetNtoa(pstSipHost->u.iIpv4Addr, &pStrIpv4);
            strHostname = acRmtAddr;
            //std::string strTem(pStrIpv4);
            //strHostname = strTem;
            break;
        }
    case EN_SIP_HOST_IPV6:
        {
            //ipv6
            ZCHAR TEMiPV6[ZINET_IPV6_ADDR_SIZE + 1];
            Zos_InetNtop(ZINET_IPV6, pstSipHost->u.aucIpv6Addr, TEMiPV6, ZINET_IPV6_ADDR_SIZE + 1);
            strHostname = TEMiPV6;
            break;
        }
    }
    return true;
}


bool SipMsgCommon::enSipUri(const std::string &strUserInfo, const std::string &strPassword,
                            const std::string &strHostname, const int iPort,ST_SIP_USER_INFO * pstUserInfo, ST_SIP_HOST_PORT * pstHostPort,  ST_SIP_SIP_URI &stSipUri)
{
    ABNF_LIST_CREATE(&stSipUri.stUriParmLst);

    /* set ST_SIP_USER_INFO */
    if(!strUserInfo.empty())
    {
        enUserInfo(strUserInfo, strPassword, *pstUserInfo);
        stSipUri.ucUserInfoPres = 1;
        stSipUri.stUserInfo = *pstUserInfo;
    }
    else
    {
        stSipUri.ucUserInfoPres = 0;
    }

    /* set ucHdrsPres */
    stSipUri.ucHdrsPres = 0;

    /* set ST_SIP_HOST_PORT */
    enHostPort(strHostname, iPort, pstHostPort);
    stSipUri.stHostPort = *pstHostPort;

#if 0

    void *zMemBuf;
    ST_SIP_SIP_URI *pstSipUri;

    zMemBuf = Zos_DbufCreate(ZNULL, ZDBUF_TYPE_STRUCT, 128);

    ABNF_LIST_ALLOC_DATA(zMemBuf, ST_SIP_SIP_URI, pstSipUri);

    /* set ST_SIP_USER_INFO */
    if(!strUserInfo.empty())
    {
        enUserInfo(strUserInfo, strPassword, pstUserInfo);
        pstSipUri->ucUserInfoPres = 1;
        pstSipUri->stUserInfo = *pstUserInfo;
    }
    else
    {
        pstSipUri->ucUserInfoPres = 0;
    }

    /* set ucHdrsPres */
    pstSipUri->ucHdrsPres = 0;

    /* set ST_SIP_HOST_PORT */
    enHostPort(strHostname, iPort, pstHostPort);
    pstSipUri->stHostPort = *pstHostPort;


    //Zos_MemSet(stSipUri.aucSpare,0,2);


    *ppstSipUri = pstSipUri;
#endif
    return true;
}


bool SipMsgCommon::enSipUri(ZUBUF zBufId, const std::string &strUserInfo, const std::string &strPassword,
                            const std::string &strHostname, const int iPort, ST_SIP_SIP_URI &stSipUri)
{
    ST_SIP_HOST_PORT stHostPort;
    ST_ZOS_SSTR stUserInfo;

    SipMsgCommon::str2ZosSstr(strUserInfo, stUserInfo);				/* set stUserInfo */
    SipMsgCommon::enHostPort(strHostname, iPort, stHostPort);		 /* set stHostPort */

    return Sip_ParmFillSipUri(zBufId, &stSipUri, &stUserInfo, &stHostPort) == ZOK ? true : false;
}


bool SipMsgCommon::deSipUri(ST_SIP_SIP_URI *pstSipUri, std::string &strUserInfo,
                            std::string &strPassword, std::string &strHostname, int &iPort)
{
    if(!pstSipUri)
    {
        return false;
    }

    deHostPort(&(pstSipUri->stHostPort), strHostname, iPort);

    if(pstSipUri->ucUserInfoPres)
    {
        deUserInfo(&(pstSipUri->stUserInfo), strUserInfo, strPassword);
    }
    return true;
}

bool SipMsgCommon::enCallId(const std::string &strCallNumer, const std::string &strCallHost,
                            ST_SIP_CALLID &stCallId)
{
    stCallId.ucPres = 1;
    stCallId.ucHostPres = 0;
    str2ZosSstr(strCallNumer, stCallId.stNumber);
    if(!strCallHost.empty())
    {
        stCallId.ucHostPres = 1;

        str2ZosSstr(strCallHost, stCallId.stHost);
    }
    return true;
}

bool SipMsgCommon::enIpv4Addr(const std::string &strHost, int iPort,
                              ST_ZOS_INET_ADDR &stInetAddr)
{
    if (strHost.empty())
        return false;

    ZOS_IPV4_ADDR_SET_STRX(&stInetAddr, strHost.c_str(), iPort);
    return true;
}

/* decode ST_SDP_OF to.StSdpOrigin */
bool SipMsgCommon::deSdpOrigin(ST_SDP_OF stSdpOf, StSdpOrigin &stSdpOrigin)
{
    unsigned char ucCastAddrType;

    stSdpOrigin._ucNetType = stSdpOf.ucNetType;
    stSdpOrigin._ucAddrType = stSdpOf.ucAddrType;
    zosSstr2Str(stSdpOf.stUserName, stSdpOrigin._strUsername);
    /* set session id */
    if(stSdpOf.ucBigSessIdPres)
    {
        std::string strTemp;
        zosSstr2Str(stSdpOf.stSessId, strTemp);
        try {
            stSdpOrigin._lSessId = tars::TC_Common::strto<int64_t>(strTemp);
        } catch (...) {
            stSdpOrigin._lSessId = 0;
        }
    }
    else
    {
        stSdpOrigin._lSessId = stSdpOf.iSessId;
    }
    /* set session version */
    if(stSdpOf.ucBigSessVerPres)
    {
        std::string strTemp;
        zosSstr2Str(stSdpOf.stSessVer, strTemp);
        try {
            stSdpOrigin._lSessVer = tars::TC_Common::strto<int64_t>(strTemp);
        } catch (...) {
            stSdpOrigin._lSessVer = 0;
        }
    }
    else
    {
        stSdpOrigin._lSessVer = stSdpOf.iSessVer;
    }
    /* set addr */
    if (stSdpOf.stAddr.ucPres)
    {
        //ZCHAR * pStrIpv4;
        ZCHAR TEMiPV6[ZINET_IPV6_ADDR_SIZE + 1];
        std::string strTem;
        ZCHAR acRmtAddr[ZINET_IPV4_STR_SIZE] = {0};
        ucCastAddrType = stSdpOf.stAddr.ucType;
        switch(ucCastAddrType)
        {
        case EN_SDP_UCAST_ADDR_IPV4:
            Zos_InetNtop(ZINET_IPV4, &stSdpOf.stAddr.u.iIpv4, acRmtAddr, ZINET_IPV4_STR_SIZE);
            //Zos_InetNtoa(stSdpOf.stAddr.u.iIpv4, &pStrIpv4);
            stSdpOrigin._strAddr = acRmtAddr;
            break;

        case EN_SDP_UCAST_ADDR_IPV6:

            Zos_InetNtop(ZINET_IPV6, stSdpOf.stAddr.u.aucIpv6, TEMiPV6, ZINET_IPV6_ADDR_SIZE + 1);
            stSdpOrigin._strAddr = TEMiPV6;
            break;

        case EN_SDP_UCAST_ADDR_FQDN:
            SipMsgCommon::zosSstr2Str(stSdpOf.stAddr.u.stFqdn, strTem);
            stSdpOrigin._strAddr = strTem;
            break;

        case EN_SDP_UCAST_ADDR_EXTN:

            SipMsgCommon::zosSstr2Str(stSdpOf.stAddr.u.stExtn, strTem);
            stSdpOrigin._strAddr = strTem;
            break;

        default:

            break;
        }
    }
    return true;
}

bool SipMsgCommon::fillSdpVersion(int iVersion, ST_SDP_SESS_DESC *pstSdpMsg)
{
    pstSdpMsg->stVer.ucPres = ZTRUE;
    pstSdpMsg->stVer.wVer = iVersion;
    return true;
}

bool SipMsgCommon::fillSdpOrigin(StSdpOrigin *pstSdpOrigin, ST_SDP_SESS_DESC *pstSdpMsg)
{
    if(!pstSdpOrigin) return false;
    ST_SDP_OF stOrig;
    ZUBUF zMemBuf;

    /* allocate memory */
    zMemBuf = Zos_CbufCreate(sizeof(ST_SDP_OF));

    /* transform string to ST_ZOS_INET_IP */
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(pstSdpOrigin->_strAddr.c_str()), &stIp);

    /* sdp message set origin-field */
    if(Sdp_MsgSetOfX(zMemBuf, &stOrig, (ZCHAR *)(pstSdpOrigin->_strUsername.c_str()),
        ZULONG(pstSdpOrigin->_lSessId), ZULONG(pstSdpOrigin->_lSessVer),
        &stIp) == ZFAILED)
    {
        speak("Sdp_MsgSetOfX failed");
        return false;
    }
    pstSdpMsg->stOrig = stOrig;
    return true;
}

bool SipMsgCommon::fillSdpSessName(const std::string &strSessname, ST_SDP_SESS_DESC *pstSdpMsg)
{
    ZUBUF zMemBuf;

    /* allocate memory */
    zMemBuf = Zos_CbufCreate(sizeof(ST_SDP_SF));

    /* sdp message set session-name--field */
    if(Sdp_MsgSetSf(zMemBuf, &pstSdpMsg->stSessName,
        (ZCHAR *)strSessname.c_str(), (ZUSHORT)strSessname.size()) == ZFAILED)
    {
        return false;
    }
    return true;
}

/* decode ST_SDP_CF to StSdpConnInfo */
bool SipMsgCommon::deSdpConnInfo(ST_SDP_CF stSdpCf, StSdpConnInfo &stSdpConnInfo)
{
    stSdpConnInfo._ucNetType = stSdpCf.ucNetType;
    stSdpConnInfo._ucAddrType = stSdpCf.ucAddrType;
    /* set addr */
    /* TODO */

    unsigned char ucCastAddrType;
    if (stSdpCf.stAddr.ucPres)
    {
        //ZCHAR * pStrIpv4;
        ZCHAR TEMiPV6[ZINET_IPV6_ADDR_SIZE + 1];
        std::string strTem;
        ZCHAR acRmtAddr[ZINET_IPV4_STR_SIZE] = {0};
        ucCastAddrType = stSdpCf.stAddr.ucType;
        switch(ucCastAddrType)
        {
        case EN_SDP_CONN_ADDR_IPV4:
            Zos_InetNtop(ZINET_IPV4, &stSdpCf.stAddr.u.iIpv4, acRmtAddr, ZINET_IPV4_STR_SIZE);
            //Zos_InetNtoa(stSdpCf.stAddr.u.iIpv4, &pStrIpv4);
            stSdpConnInfo._strAddr = acRmtAddr;
            break;

        case EN_SDP_CONN_ADDR_IPV6:

            Zos_InetNtop(ZINET_IPV6, stSdpCf.stAddr.u.aucIpv6, TEMiPV6, ZINET_IPV6_ADDR_SIZE + 1);
            stSdpConnInfo._strAddr = TEMiPV6;
            break;

        case EN_SDP_CONN_ADDR_FQDN:
            SipMsgCommon::zosSstr2Str(stSdpCf.stAddr.u.stFqdn, strTem);
            stSdpConnInfo._strAddr = strTem;
            break;

        case EN_SDP_CONN_ADDR_EXTN:

            SipMsgCommon::zosSstr2Str(stSdpCf.stAddr.u.stExtn, strTem);
            stSdpConnInfo._strAddr = strTem;
            break;

        default:

            break;
        }
    }

    return true;
}

/* encode StSdpConnInfo to ST_SDP_CF */
bool SipMsgCommon::enSdpConnInfo(StSdpConnInfo stSdpConnInfo, ST_SDP_CF &stSdpCf)
{
    stSdpCf.ucPres = 1;
    stSdpCf.ucNetType = stSdpConnInfo._ucNetType;
    stSdpCf.ucAddrType = stSdpConnInfo._ucAddrType;
    /* set addr */
    /* TODO */
    stSdpCf.stAddr.ucPres = 0;
    return true;
}

/* decode ST_SDP_SESS_TIME to StSdpTimeDesc */
bool SipMsgCommon::deSdpTimeDesc(ST_SDP_SESS_TIME stSessTime, StSdpTimeDesc &stTimeDesc)
{
    ST_SDP_TF *pstSdpTf;
    if(!stSessTime.ucPres) return false;
    pstSdpTf = (ST_SDP_TF *)ABNF_LIST_HEAD_DATA(&(stSessTime.stTimeLst));
    if(pstSdpTf)
    {
        stTimeDesc._lStartTime = pstSdpTf->iStartTime;
        stTimeDesc._lStopTime = pstSdpTf->iStopTime;
    }
    return true;
}

/* encode ST_SDP_SESS_TIME */
bool SipMsgCommon::enSdpTimeDesc(StSdpTimeField *pstTimeField, ST_SDP_SESS_TIME **ppstTime)
{
    ST_SDP_SESS_TIME *pstTime = 0;
    ZUBUF zMemBuf = 0;

    /* allocate memory */
    zMemBuf = Zos_CbufCreate(sizeof(ST_SDP_SESS_TIME));
    ABNF_LIST_ALLOC_DATA(zMemBuf, ST_SDP_SESS_TIME, pstTime);
    if(!pstTime) return false;

    pstTime->ucPres = ZTRUE;
    pstTime->ucZPres = ZFALSE;
    ABNF_LIST_CREATE(&(pstTime->stTimeLst));
    ABNF_LIST_CREATE(&(pstTime->stZoneAdjLst));

    /* set ST_SDP_TF */
    ST_SDP_TF *pstTf;
    if(enSdpTimeField(pstTimeField, &pstTf))
    {
        ABNF_LIST_APPEND_NODE(&(pstTime->stTimeLst), pstTf);
    }

    *ppstTime = pstTime;
    return true;
}

bool SipMsgCommon::fillSdpSessTime(const StSdpTimeField *pstTimeField, ST_SDP_SESS_DESC *pstSdpMsg)
{
    ZUBUF zMemBuf;

    /* allocate memory */
    zMemBuf = Zos_CbufCreate(sizeof(ST_SDP_SESS_TIME));
    if(Sdp_MsgSetTf(zMemBuf, pstSdpMsg, pstTimeField->_lStartTime, pstTimeField->_lStopTime) == ZFAILED)
    {
        return false;
    }

    return true;
}

/* encode StSdpTimeField to ST_SDP_TF */
bool SipMsgCommon::enSdpTimeField(StSdpTimeField *pstTimeField, ST_SDP_TF **ppstTf)
{
    ST_SDP_TF *pstTf = 0;
    ZUBUF zMemBuf = 0;

    /* create memory buffer */
    zMemBuf = Zos_CbufCreate(sizeof(ST_SDP_TF));

    /* allocate memory */
    ABNF_LIST_ALLOC_DATA(zMemBuf, ST_SDP_TF, pstTf);

    /* create list */
    ABNF_LIST_CREATE(&pstTf->stRepLst);

    pstTf->iStartTime = 0;
    pstTf->iStopTime = 0;
    if(pstTimeField)
    {
        pstTf->iStartTime = (ZULONG)pstTimeField->_lStartTime;
        pstTf->iStartTime = (ZULONG)pstTimeField->_lStopTime;
    }

    *ppstTf = pstTf;
    return true;
}

bool SipMsgCommon::enSdpKey(const std::string &strKey, ST_SDP_KF &stSdpKf)
{
    stSdpKf.ucPres = 1;
    if(strKey.empty())
    {
        stSdpKf.ucDataPres = 0;
    }
    else
    {
        stSdpKf.ucDataPres = 1;
        stSdpKf.ucKeyType = EN_SDP_KEY_BASE64;
        str2ZosSstr(strKey, stSdpKf.stData);
    }
    return true;
}

/* decode ST_SDP_MDESC to StSdpMediaDesc */
bool SipMsgCommon::deSdpMediaDesc(ST_SDP_MDESC stMDesc, StSdpMediaDesc &stMediaDesc)
{
    ST_SDP_MF stMedia;
    stMedia = stMDesc.stMedia;
    if(!stMedia.ucPres) return false;
    stMediaDesc._ucMediaType = stMedia.ucMediaType;
    stMediaDesc._ucProtoType = stMedia.ucProtoType;
    stMediaDesc._iPort = stMedia.wPort;

    /* TOTO */
    //stMediaDesc._fmtVec;

    return true;
}

bool SipMsgCommon::fillSdpMediaDesc(const StSdpMediaDesc *pstMediaDesc, ST_SDP_SESS_DESC *pstSdpMsg)
{
    /* check the pointer */
    if(!pstMediaDesc)  return false;
    if(!pstSdpMsg)  return false;

    ZUBUF zMemBuf;
    ZUBUF zMfBuf;
    ST_SDP_MDESC *pstMdesc;

    /* allocate memory */
    zMemBuf = Zos_CbufCreate(sizeof(ST_SDP_MDESC));

    if(Sdp_MsgCreateMdesc(zMemBuf, pstSdpMsg, &pstMdesc) == ZFAILED)
    {
        Zos_CbufDelete(zMemBuf);
        speak("Sdp_MsgCreateMdesc failed");
        return false;
    }

    /* allocate zMfBuf */
    zMfBuf = Zos_CbufCreate(sizeof(ST_SDP_MF));

    if(Sdp_MsgSetMfX(zMfBuf, &pstMdesc->stMedia,
        pstMediaDesc->_ucMediaType, ZNULL, pstMediaDesc->_ucProtoType,
        ZNULL, pstMediaDesc->_iPort, 0) == ZFAILED)
    {
        Zos_CbufDelete(zMemBuf);
        Zos_CbufDelete(zMfBuf);
        speak("Sdp_MsgSetMfX failed");
        return false;
    }

    return true;
}

/* add ST_SDP_TF to ST_SDP_SESS_DESC */
bool SipMsgCommon::addSdpTimeField(StSdpTimeField *pstTimeField, ST_SDP_SESS_DESC &stSessDesc)
{
    if(!pstTimeField) return false;
    ST_SDP_TF stTf;
    stTf.iStartTime = (ZULONG)pstTimeField->_lStartTime;
    stTf.iStopTime = (ZULONG)pstTimeField->_lStopTime;
    ABNF_LIST_CREATE(&(stTf.stRepLst));

    ABNF_LIST_APPEND_NODE(&(stSessDesc.stSessTime.stTimeLst), &stTf);
    return true;
}

/* ST_SDP_AF to StSdpAttr */
bool SipMsgCommon::deSdpAttr(ST_SDP_AF stAf, StSdpAttr &stAttr)
{
    return true;
}

/* encode StSdpAttr to ST_SDP_AF */
bool SipMsgCommon::enSdpAttr(StSdpAttr stAttr, ST_SDP_AF &stAf)
{
    /* TODO */
    return true;
}

/* encode vector<StSdpAttr> to ST_SDP_AF_LST */
bool SipMsgCommon::enSdpAttrLst(vector<StSdpAttr> stAttrVec, ST_SDP_AF_LST &stAfLst)
{
    ABNF_LIST_CREATE(&stAfLst);
    vector<StSdpAttr>::iterator it0;
    for(it0 = stAttrVec.begin(); it0 != stAttrVec.end(); it0++)
    {
        /* TODO */
    }
    return true;
}

/* decode ST_SDP_SESS_DESC to StSdpSessDesc */
bool SipMsgCommon::deSdpSessDesc(ST_SDP_SESS_DESC stSessDescSrc, StSdpSessDesc &stSessDescDst)
{
    stSessDescDst._iVer = stSessDescSrc.stVer.wVer;
    deSdpOrigin(stSessDescSrc.stOrig, stSessDescDst._stOrigin);
    zosSstr2Str(stSessDescSrc.stSessName.stSessName, stSessDescDst._strSessName);
    zosSstr2Str(stSessDescSrc.stInfo.stInfo, stSessDescDst._strInfo);
    zosSstr2Str(stSessDescSrc.stUri.stUri, stSessDescDst._strUri);
    /* TODO */
    /* set email */
    /* set phone */
    deSdpConnInfo(stSessDescSrc.stConn, stSessDescDst._stConnInfo);
    deSdpTimeDesc(stSessDescSrc.stSessTime, stSessDescDst._stTimeDesc);

    /* set media */
    ST_SDP_MDESC_LST stMDescLst;
    ST_ABNF_LIST_NODE *pstNode;
    ST_SDP_MDESC *pstMDesc;
    void *pstVoid;
    stMDescLst = stSessDescSrc.stMdescLst;
    FOR_ALL_DATA_IN_ABNF_LIST(&stMDescLst, pstNode, pstVoid)
    {
        StSdpMediaDesc stMediaDesc;
        pstMDesc = (ST_SDP_MDESC *)pstVoid;
        deSdpMediaDesc(*pstMDesc, stMediaDesc);
        stSessDescDst._stMediaVec.push_back(stMediaDesc);
    }

    /* set attribute */
    ST_SDP_AF_LST stAfLst;
    ST_SDP_AF *pstAf;
    stAfLst = stSessDescSrc.stAttrLst;
    FOR_ALL_DATA_IN_ABNF_LIST(&stAfLst, pstNode, pstVoid)
    {
        StSdpAttr stAttr;
        pstAf = (ST_SDP_AF *)pstVoid;
        deSdpAttr(*pstAf, stAttr);
        stSessDescDst._stAttrVec.push_back(stAttr);
    }

    return true;
}

/* encode StSdpSessDesc to ST_SDP_SESS_DESC */
bool SipMsgCommon::enSdpSessDesc(StSdpSessDesc stSessDescSrc, ST_SDP_SESS_DESC **ppstSdpMsg)
{
    ST_SDP_SESS_DESC *pstSdpMsg = 0;
    ZUBUF zMemBuf = 0;

    /* allocate memory */
    zMemBuf = Zos_CbufCreate(sizeof(ST_SDP_SESS_DESC));
    if(!zMemBuf) return false;

    /* create ST_SDP_SESS_DESC */
    Sdp_MsgCreate(zMemBuf, &pstSdpMsg);
    if(!pstSdpMsg) return false;

    /* create list */
    ABNF_LIST_CREATE(&(pstSdpMsg->stEmailLst));
    ABNF_LIST_CREATE(&(pstSdpMsg->stBwLst));
    ABNF_LIST_CREATE(&(pstSdpMsg->stPhoneLst));
    ABNF_LIST_CREATE(&(pstSdpMsg->stAttrLst));
    ABNF_LIST_CREATE(&(pstSdpMsg->stMdescLst));

    /* set pres */
    pstSdpMsg->ucPres = ZTRUE;

    /* fill sdp version */
    fillSdpVersion(stSessDescSrc._iVer, pstSdpMsg);

    /* fill sdp origin-field */
    fillSdpOrigin(&(stSessDescSrc._stOrigin), pstSdpMsg);

    /* fill sdp session name */
    fillSdpSessName(stSessDescSrc._strSessName, pstSdpMsg);

    /* fill sdp time description */
    fillSdpSessTime(&(stSessDescSrc._stTimeField), pstSdpMsg);

    /* fill sdp media description */
    if(stSessDescSrc._stMediaVec.size() > 0)
    {
        StSdpMediaDesc stMediaDesc = stSessDescSrc._stMediaVec[0];
        fillSdpMediaDesc(&(stMediaDesc), pstSdpMsg);
    }

    *ppstSdpMsg = pstSdpMsg;
    return true;
}

/* set StSdpOrigin */
bool SipMsgCommon::setArcSdpOrigin(unsigned char ucNetType, unsigned char ucAddrType,
                                   int64_t lSessId, int64_t lSessVer,
                                   const std::string &strAddr,const std::string &strUsername,
                                   StSdpOrigin &stOrigin)
{
    stOrigin._ucNetType = ucNetType;
    stOrigin._ucAddrType = ucAddrType;
    stOrigin._lSessId = lSessId;
    stOrigin._lSessVer = lSessVer;
    stOrigin._strAddr = strAddr;
    stOrigin._strUsername = strUsername;
    return true;
}

/* set StSdpConnInfo */
bool SipMsgCommon::setArcSdpConnInfo(unsigned char ucNetType, unsigned char ucAddrType,
                                     const std::string &strAddr, StSdpConnInfo &stConnInfo)
{
    stConnInfo._ucNetType = ucNetType;
    stConnInfo._ucAddrType = ucAddrType;
    stConnInfo._strAddr = strAddr;
    return true;
}

/* set StSdpTimeDesc */
bool SipMsgCommon::setArcSdpTime(int64_t lStartTime, int64_t lStopTime,
                                 StSdpTimeDesc &stTime)
{
    stTime._lStartTime = lStartTime;
    stTime._lStopTime = lStopTime;
    return true;
}

/* set StSdpMediaDesc */
bool SipMsgCommon::setArcSdpMediaDesc(unsigned char ucMediaType, unsigned char ucProtoType,
                                      int iPort, StSdpMediaDesc &stMediaDesc)
{
    stMediaDesc._ucMediaType = ucMediaType;
    stMediaDesc._ucProtoType = ucProtoType;
    stMediaDesc._iPort = iPort;
    return true;
}

/* set StSdpSessDesc */
bool SipMsgCommon::setArcSdpSessDesc(int iVer, const std::string strSessName,
                                     const std::string &strInfo, const std::string strUri,
                                     const std::string &strEmail, const std::string &strPhone,
                                     StSdpSessDesc &stSessDesc)
{
    stSessDesc._iVer = iVer;
    stSessDesc._strSessName = strSessName;
    stSessDesc._strInfo = strInfo;
    stSessDesc._strUri = strUri;
    stSessDesc._strEmailVec.push_back(strEmail);
    stSessDesc._strPhoneVec.push_back(strPhone);
    return true;
}

/* set StSdpSessDesc */
bool SipMsgCommon::setArcSdpSessDescX(StSdpOrigin *pstOrigin, StSdpConnInfo *pstConnInfo,
                                      StSdpTimeField *pstTime,StSdpSessDesc &stSessDesc)
{
    if(pstOrigin)
        stSessDesc._stOrigin = *pstOrigin;
    if(pstConnInfo)
        stSessDesc._stConnInfo = *pstConnInfo;
    if(pstTime)
        stSessDesc._stTimeField = *pstTime;
    return true;
}

/* init ST_SDP_ATTR_LST */
bool SipMsgCommon::initSdpAttrLst(ST_SDP_AF_LST &stAfLst)
{
    ABNF_LIST_CREATE(&stAfLst);
    return true;
}

/* init ST_SDP_MDESC */
bool SipMsgCommon::initSdpMediaDesc(ST_SDP_MDESC &stMdesc)
{
    ABNF_LIST_CREATE(&stMdesc.stConnLst);
    ABNF_LIST_CREATE(&stMdesc.stBwLst);
    ABNF_LIST_CREATE(&stMdesc.stAttrLst);
    return true;
}

/* init ST_SDP_SESS_DESC */
bool SipMsgCommon::initSdpSessDesc(ST_SDP_SESS_DESC &stSessDesc)
{
    ABNF_LIST_CREATE(&stSessDesc.stEmailLst);
    ABNF_LIST_CREATE(&stSessDesc.stBwLst);
    ABNF_LIST_CREATE(&stSessDesc.stPhoneLst);
    ABNF_LIST_CREATE(&stSessDesc.stAttrLst);
    ABNF_LIST_CREATE(&stSessDesc.stMdescLst);
    return true;
}

/* create strBranch */
std::string SipMsgCommon::createBranch()
{
    std::string strBranch;
    strBranch += "z9hG4bK";
    strBranch += TC_UUIDGenerator::getInstance()->genID();
    return strBranch;
}

/* create To tag */
std::string SipMsgCommon::createToTag()
{
    return TC_UUIDGenerator::getInstance()->genID();
}

/* create From tag */
std::string SipMsgCommon::createFromTag()
{
    return TC_UUIDGenerator::getInstance()->genID();
}

ZINT  SipMsgCommon::Sip_ParmFillAddrSpecBySipUri(ZUBUF zBufId,
                                                 ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_SIP_URI  *pstSipUri)
{
    /* check the input parameter */
    if (!zBufId || !pstAddrSpec || !pstSipUri)
        return ZFAILED;

    /* fill SIP-URI */
    if (Sip_CpySipUri(zBufId, &pstAddrSpec->u.stSipUri, pstSipUri) != ZOK)
        return ZFAILED;

    pstAddrSpec->ucType = EN_SIP_ADDR_SPEC_SIP;
    return ZOK;
}

bool SipMsgCommon::getReqSipsUri(ST_SIP_MSG *pstMsg, ST_SIP_SIP_URI **ppstUri)
{
    if (Sip_MsgGetReqSipUri(pstMsg, ppstUri) == ZOK)
        return true;
    return false;
}

bool  SipMsgCommon::getFromToAddrSpec(ST_SIP_MSG *pstMsg, ZBOOL bFromHdr, ST_SIP_ADDR_SPEC  & stAddrSpec)
{
    ZINT iResult = 0;
    ST_SIP_ADDR_SPEC * pAddSpec;
    iResult = Sip_MsgGetFromToAddrSpec(pstMsg,bFromHdr, &pAddSpec);
    if (iResult == ZOK)
    {
        stAddrSpec = *pAddSpec;
        return true;
    }
    return false;
}

bool SipMsgCommon::UnCharVectorToArray(const vector<ZUCHAR> & ucVect, char aucPtLst[20])
{
    for (unsigned int j = 0; j < ucVect.size(); j++ )
    {
        aucPtLst[j] = ucVect.at(j);

    }
    return true;
}


bool SipMsgCommon::OptTagEnumToName(const ZUCHAR & ucType, std::string & strTypeName)
{
    switch(ucType)
    {
    case EN_SIP_OPT_TAG_100REL:
        strTypeName = "100rel";
        break;

    case EN_SIP_OPT_TAG_EARLY_SESS:
        strTypeName = "early-session";
        break;
    case EN_SIP_OPT_TAG_EVNTLST:
        strTypeName = "eventlist";
        break;

    case EN_SIP_OPT_TAG_FROM_CHANGE:
        strTypeName = "from-change";
        break;

    case EN_SIP_OPT_TAG_HISTINFO:
        strTypeName = "histinfo";
        break;

    case EN_SIP_OPT_TAG_JOIN:
        strTypeName = "join";
        break;

    case EN_SIP_OPT_TAG_NOREFERSUB:
        strTypeName = "norefersub";
        break;

    case EN_SIP_OPT_TAG_PATH:
        strTypeName = "path";
        break;

    case EN_SIP_OPT_TAG_PRECONDITION:
        strTypeName = "precondition";
        break;

    case EN_SIP_OPT_TAG_PREF:
        strTypeName = "pref";
        break;

    case EN_SIP_OPT_TAG_PRIVACY:
        strTypeName = "privacy";
        break;

    case EN_SIP_OPT_TAG_REPLACES:
        strTypeName = "replaces";
        break;

    case EN_SIP_OPT_TAG_RES_PRIORITY:
        strTypeName = "resource-priority";
        break;

    case EN_SIP_OPT_TAG_SDP_ANAT:
        strTypeName = "sdp-anat";
        break;
    case EN_SIP_OPT_TAG_SEC_AGREE:
        strTypeName = "sec-agree";
        break;

    case EN_SIP_OPT_TAG_TDLG:
        strTypeName = "tdialog";
        break;

    case EN_SIP_OPT_TAG_TMR:
        strTypeName = "timer";
        break;

    case EN_SIP_OPT_TAG_MULTI_REFER:
        strTypeName = "multiple-refer";
        break;

    case EN_SIP_OPT_TAG_ANSWERMODE:
        strTypeName = "answermode";
        break;

    case EN_SIP_OPT_TAG_GRUU:
        strTypeName = "gruu";
        break;

    case EN_SIP_OPT_TAG_ICE:
        strTypeName = "ice";
        break;

    case EN_SIP_OPT_TAG_OUTBOUND:
        strTypeName = "outbound";
        break;

    case EN_SIP_OPT_TAG_MSG:
        strTypeName = "recipient-list-message";
        break;

    case EN_SIP_OPT_TAG_RCP_LST_IVT:
        strTypeName = "recipient-list-invite";
        break;

    case EN_SIP_OPT_TAG_SUBSCRIBE:
        strTypeName = "recipient-list-subscribe";
        break;

        //case EN_SIP_OPT_TAG_MPT:
        //  strTypeName = "mpt";
        //  break;

        //case EN_SIP_OPT_TAG_EPT:
        //  strTypeName = "ept";
        //  break;

        //case EN_SIP_OPT_TAG_ARC:
        //  strTypeName = "arc";
        //  break;

    case EN_SIP_OPT_TAG_OTHER:
        strTypeName = "others";
        break;
    }

    return true;
}


bool SipMsgCommon::OptMethodEnumToName(const ZUCHAR & ucType, std::string & strTypeName)
{
    switch(ucType)
    {
    case EN_SIP_METHOD_INVITE:
        strTypeName = "INVITE";
        break;

    case EN_SIP_METHOD_ACK:
        strTypeName = "ACK";
        break;

    case EN_SIP_METHOD_OPTIONS:
        strTypeName = "OPTIONS";
        break;

    case EN_SIP_METHOD_BYE:
        strTypeName = "BYE";
        break;


    case EN_SIP_METHOD_CANCEL:
        strTypeName = "CANCEL";
        break;

    case EN_SIP_METHOD_REGISTER:
        strTypeName = "REGISTER";
        break;

    case EN_SIP_METHOD_PRACK:
        strTypeName = "PRACK";
        break;

    case EN_SIP_METHOD_SUBS:
        strTypeName = "SUBSCRIBE";
        break;

    case EN_SIP_METHOD_NOTIFY:
        strTypeName = "NOTIFY";
        break;

    case EN_SIP_METHOD_INFO:
        strTypeName = "INFO";
        break;

    case EN_SIP_METHOD_UPDATE:
        strTypeName = "UPDATE";
        break;

    case EN_SIP_METHOD_MSG:
        strTypeName = "MESSAGE";
        break;

    case EN_SIP_METHOD_REFER:
        strTypeName = "REFER";
        break;

    case EN_SIP_METHOD_COMET:
        strTypeName = "COMET";
        break;

    case EN_SIP_METHOD_PUBLISH:
        strTypeName = "PUBLISH";
        break;

    default:
        strTypeName = "others";

    }
    return true;
}


/**
* @brief SIP message get name spec in From or To header.
*
* @param [in] pstMsg SIP message.
* @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
* @param [out] ppstNaSpec Name-addr or addr-spec data strcuture.
*
* @retval ZOK Operation successfully.
* @retval ZFAILED Otherwise.
*/
bool SipMsgCommon::getFromToNaSpec(ST_SIP_MSG *pstMsg, ZBOOL bFromHdr, ST_SIP_NA_SPEC & stNaSpec)
{
    ST_SIP_NA_SPEC * pstNaSpec;
    ZINT iResult = 0;
    iResult = Sip_MsgGetFromToNaSpec(pstMsg,bFromHdr, &pstNaSpec);
    if (iResult == ZOK)
    {
        stNaSpec = *pstNaSpec;
        return true;
    }
    return false;
}


bool SipMsgCommon::getFromToSipUri(ST_SIP_MSG *pstMsg, ZBOOL bFromHdr,  ST_SIP_SIP_URI ** ppstSipUri)
{
    ZINT iResult = 0;
    ST_SIP_ADDR_SPEC * pAddSpec;
    iResult = Sip_MsgGetFromToAddrSpec(pstMsg,bFromHdr, &pAddSpec);
    if (iResult != ZOK)
    {
        return false;
    }

    if (pAddSpec->ucType == EN_SIP_ADDR_SPEC_SIP )
    {
        *ppstSipUri = &pAddSpec->u.stSipUri;
        return true;
    }
    else if (pAddSpec->ucType == EN_SIP_ADDR_SPEC_SIPS)
    {
        *ppstSipUri = &pAddSpec->u.stSipsUri;
        return true;
    }
    return false;
}

bool SipMsgCommon::getContactSipUri(ST_SIP_MSG *pstMsg, ST_SIP_SIP_URI ** ppstSipUri)
{
    ST_SIP_ADDR_SPEC * pAddSpec = 0;

    if ((Sip_MsgGetContactAddrSpec(pstMsg,&pAddSpec) != ZOK))
        return false;

    if (pAddSpec->ucType == EN_SIP_ADDR_SPEC_SIP )
    {
        *ppstSipUri = &pAddSpec->u.stSipUri;
        return true;
    }
    else if (pAddSpec->ucType == EN_SIP_ADDR_SPEC_SIPS)
    {
        *ppstSipUri = &pAddSpec->u.stSipsUri;
        return true;
    }
    return false;
}

bool  SipMsgCommon::getPPreaferdIDHeaderSipUri(ST_SIP_MSG *pstMsg,   ST_SIP_SIP_URI ** ppstSipUri)
{
    ST_SIP_HDR_P_PREFERRED_ID *pstHdrPpid = 0;
    ST_SIP_PPREFERREDID_VAL *pstParm = 0;

    pstHdrPpid = (ST_SIP_HDR_P_PREFERRED_ID *)Sip_FindMsgHdr(pstMsg, EN_SIP_HDR_P_PREFERRED_ID);
    if (0 == pstHdrPpid)
        return false;

    pstParm =(ST_SIP_PPREFERREDID_VAL*) ABNF_LIST_HEAD_DATA(&pstHdrPpid->stValLst);
    if (pstParm == 0)
        return false;

    if (pstParm->stNaSpec.ucNameAddrPres)
    {
        *ppstSipUri = &pstParm->stNaSpec.u.stNameAddr.stAddrSpec.u.stSipUri;
    }
    else
    {
        *ppstSipUri = &pstParm->stNaSpec.u.stAddrSpec.u.stSipUri;
    }

    return true;
}



//
//bool SipMsgCommon::SessFillImdnData(ST_SIP_MSG *pstMsg,ST_MSF_URI *pstLclUri,ST_MSF_URI *pstPartpUri, ZCHAR *pcMsg, ZUINT iMsgSize,
//							 ZCHAR *pcContType,ZCHAR **ppcData, ZUINT *piDataLen)
//{
//	ZUINT iNtfyType;  //messaging session,只定义消息会话即messaging session， 其他 large message session ，file transfer session，image share session等暂时不考虑（参考EN_MMF_SESS_TYPE）
//	int iresult = 0;
//	ST_ZOS_SYS_TIME stTime;          /* date time */
//	ST_ZOS_SSTR stIMsgId;            /* Message-ID header for IMDN */
//	std::string strMsgId;
//
//	strMsgId = "JNdcbW81u48Qavgc";	//暂时写定
//	Zos_SysTimeU(&stTime);
//	iNtfyType = EN_MMF_MSESS_IMDN_DISP;
//	SipMsgCommon::str2ZosSstr(strMsgId,stIMsgId);
//
//	return FillCpimMsgImdnX(pstMsg->zMemBuf, pstLclUri, 	pstPartpUri, pcMsg, iMsgSize, &stTime, iNtfyType, pcContType, ppcData, piDataLen, &stIMsgId) == ZOK ? true : false;
//
//}
//
//bool SipMsgCommon::FillCpimMsgImdnX(ZUBUF zMemBuf, ST_MSF_URI *pstLclUri, ST_MSF_URI *pstPartpUri, ZCHAR *pcBody, ZUINT iBoydSize,
//							 ST_ZOS_SYS_TIME *pstTime, ZUINT iImdnType, ZCHAR *pcCType,
//							 ZCHAR **ppcMsg, ZUINT *piMsgSize, ST_ZOS_SSTR *pstIMsgId)
//{
//	ZBOOL bSuccSupt, bFailSupt;
//	ZCHAR *pcDeliRptDesc;
//	ZUBUF zCpimId;
//	ZUBUF zData;
//	ZCHAR *pcStr;
//
//	/* create the cpim abnf message */
//	zCpimId = Zcpim_Create();
//	if (!zCpimId) return ZFAILED;
//
//	/* add From header */
//	if (pstLclUri->stDispName.wLen)
//		Zcpim_FillHdrFrom(zCpimId, pstLclUri->stDispName.pcStr,
//		pstLclUri->stUserUri.pcStr);
//	else
//		Zcpim_FillHdrFrom(zCpimId, ZNULL, pstLclUri->stUserUri.pcStr);
//
//	/* add To header */
//	if (pstPartpUri->stDispName.wLen)
//		Zcpim_FillHdrTo(zCpimId, pstPartpUri->stDispName.pcStr,
//		pstPartpUri->stUserUri.pcStr);
//	else
//		Zcpim_FillHdrTo(zCpimId, ZNULL, pstPartpUri->stUserUri.pcStr);
//
//	/* add NS header */
//	Zcpim_FillHdrNs(zCpimId, "imdn", "urn:ietf:params:imdn");
//
//	/* add Message-ID header */
//	if (ZOS_ISEMPTY_SSTR(pstIMsgId))
//	{
//		pcStr = Zrandom_RandId(ZNULL, ZNULL, 16);
//		Zcpim_FillHdr(zCpimId, "imdn.Message-ID", pcStr);
//		Zos_UbufCpySStr(zMemBuf, pcStr, pstIMsgId);
//		Zos_SysStrFree(pcStr);
//	}
//	else
//		Zcpim_FillHdr(zCpimId, "imdn.Message-ID", pstIMsgId->pcStr);
//
//	/* add DateTime header */
//	Zcpim_FillHdrDateTimeX(zCpimId, pstTime);
//
//	/* get the delivery report support */
//	//bSuccSupt = Mmp_GetImdnDeliSuccRptSupt();
//	//bFailSupt = Mmp_GetImdnDeliFailRptSupt();
//
//	bSuccSupt = true;
//	bFailSupt = false;								//写定测试
//
//	if (bSuccSupt && bFailSupt)
//		pcDeliRptDesc = "positive-delivery, negative-delivery";
//	else if (bSuccSupt)
//		pcDeliRptDesc = "positive-delivery";
//	else if (bFailSupt)
//		pcDeliRptDesc = "negative-delivery";
//	else
//		pcDeliRptDesc = "positive-delivery";
//
//	/* add Disposition-Notification header */
//	  if ( iImdnType == EN_MMF_MSESS_IMDN_DELI)
//		Zcpim_FillHdr(zCpimId, "imdn.Disposition-Notification", pcDeliRptDesc);
//	else if ( iImdnType == EN_MMF_MSESS_IMDN_DISP )
//		Zcpim_FillHdr(zCpimId, "imdn.Disposition-Notification", "display");
//	else if ( iImdnType == EN_MMF_MSESS_IMDN_BOTH )
//	{
//		ZCHAR acDesc[64] = {0};
//		ZINT iLen;
//
//		iLen = Zos_StrLen(pcDeliRptDesc);
//		Zos_MemCpy(acDesc, pcDeliRptDesc, 64);
//		Zos_MemCpy(acDesc + iLen, ", display", 64 - iLen);
//		Zcpim_FillHdr(zCpimId, "imdn.Disposition-Notification", acDesc);
//	}
//
//	///* add Content-Transfer-Encoding header */
//	//if (Mmp_GetCpimBase64EncodeEnable())
//	//	Zcpim_FillCHdr(zCpimId, "Content-Transfer-Encoding", "base64");
//
//	/* add Content-type header */
//	if (pcCType)
//		Zcpim_FillCHdrCType(zCpimId, pcCType);
//	else /* according to RFC3862, message body include at least  a Content-Type header */
//		Zcpim_FillCHdrCType(zCpimId, "text/plain");
//
//	/* add Content-length header */
//	Zcpim_FillCHdrCLen(zCpimId, iBoydSize);
//
//	/* add cpim content body */
//	if (pcBody && iBoydSize) Zcpim_FillContentBodyB(zCpimId, pcBody, iBoydSize);
//
//	/* cpim message save */
//	Zcpim_Save(zCpimId, &zData);
//
//	/* copy the message */
//	Zos_UbufCpyDStr(zMemBuf, zData, ppcMsg);
//
//	/* get the message size */
//	*piMsgSize = Zos_DbufLen(zData);
//
//	/* delete the data */
//	ZOS_DBUF_DELETE(zData);
//
//	/* delete the cpim message */
//	Zcpim_Delete(zCpimId);
//
//
//
//	return true;
//}

bool SipMsgCommon::enMsfUri(const ST_ZOS_SSTR & stDispName, const ST_ZOS_SSTR & stUserUri, ST_MSF_URI & stmsfUri)
{
    stmsfUri.stDispName = stDispName;
    stmsfUri.stUserUri = stUserUri;

    return true;
}


bool SipMsgCommon::enMsfUri(const std::string & strDispName, const std::string & strMsfUri, ST_MSF_URI & stmsfUri)
{
    ST_ZOS_SSTR stDispName, stUri;
    str2ZosSstr(strDispName, stDispName);
    str2ZosSstr(strMsfUri, stUri);

    stmsfUri.stDispName = stDispName;
    stmsfUri.stUserUri = stUri;

    return true;
}


bool SipMsgCommon::GetCallIdNumberHost(const std::string & strSipDialogIs, std::string & strNumber, std::string & strHost)
{
    std::string strCallId ;
    int iFristSep = -1, iAtPos = -1;

    iFristSep = strSipDialogIs.find("\\");
    if (iFristSep < 0)
        return "";

    strCallId = strSipDialogIs.substr(0, iFristSep);
    iAtPos = strCallId.find("@");
    if (iAtPos < 0)
        return "";

    strNumber = strCallId.substr(0, iAtPos);
    strHost = strSipDialogIs.substr(iAtPos + 1, strCallId.size() - iAtPos - 1);

    return true;
}

bool SipMsgCommon::GetCallIdNumberHost2(const std::string & strSipDialogIs, std::string & strNumber, std::string & strHost)
{
    int iAtPos = strSipDialogIs.find("@");
    if (iAtPos < 0)
        return "";

    strNumber = strSipDialogIs.substr(0, iAtPos);
    strHost = strSipDialogIs.substr(iAtPos + 1);

    return true;
}


int SipMsgCommon::CreateRandInt()
{
    srand(unsigned(time(0)));
    return rand() % 10000;

}

// get response status code of _pstMsg
bool SipMsgCommon::getRspStatusCode(ST_SIP_MSG *pstMsg, unsigned int &iRspCode)
{
    if(pstMsg->ucReqPres)
    {
        printf("this is a request sip, did not have status code\r\n");
        return false;
    }
    else
    {

        //iRspCode = _pstMsg->u.stStatusLine.dwCode;
        ZUINT temCode = 0;
        if (Sip_MsgGetStatCode(pstMsg, &temCode) == ZOK)
        {
            iRspCode = temCode;
        }

        return true;
    }
}


/* get sip method */
bool SipMsgCommon::getMethod(ST_SIP_MSG *pstMsg, unsigned char &ucMethod)
{
    if(pstMsg->ucReqPres)
    {
        ucMethod = pstMsg->u.stReqLine.stMethod.ucType;
    }
    else
    {
        ZUINT dwCseq;
        if(Sip_MsgGetCseq(pstMsg, &dwCseq, &ucMethod) == ZFAILED)
            return false;
    }
    return true;
}


//md5 A1
bool SipMsgCommon::calA1(  const std::string & userName, const  std::string & passWord,const  std::string & realm, std::string & resultA1)
{
    char acA1Md5Str[SIP_MD5_HEX_STR_LEN];
    ST_ZOS_SSTR  stUsername, stPassword, stRealm;

    str2ZosSstr(userName, stUsername);
    str2ZosSstr(passWord, stPassword);
    str2ZosSstr(realm, stRealm);

    if (Sip_CalcA1(&stUsername, &stPassword, &stRealm,acA1Md5Str) != ZOK)
    {
        return false;
    }

    resultA1 = acA1Md5Str;

    return true;
}



bool  SipMsgCommon::calA2(ZUCHAR ucMethod, ST_SIP_REQ_URI *pstDigestUri,std::string & resultA2)
{
    ZCHAR acKdMd5Str[SIP_MD5_HEX_STR_LEN];
    if (Sip_CalcA2(ucMethod, pstDigestUri, acKdMd5Str) != ZOK)
    {
        return false;
    }
    resultA2 = acKdMd5Str;

    return true;
}


bool  SipMsgCommon::calKD(ZCHAR acA1Md5Str[SIP_MD5_HEX_STR_LEN],ZCHAR acA2Md5Str[SIP_MD5_HEX_STR_LEN], ST_ZOS_SSTR *pstNonce, ST_ZOS_SSTR *pstCnonce,
                          ST_ZOS_SSTR *pstNonceCount,ST_ZOS_SSTR *pstQop, std::string & acKDMd5Str)
{
    ZCHAR acResultStr[SIP_MD5_HEX_STR_LEN];
    if (Sip_CalcKd(acA1Md5Str, acA2Md5Str, pstNonce, pstCnonce, pstNonceCount, pstQop, acResultStr) != ZOK)
    {
        return false;
    }

    acKDMd5Str = acResultStr;
    return true;
}


bool SipMsgCommon::createDigestCnonce(std::string & strCNnonce)
{
    char acCnonce[32];
    /* prepare cnonce, we use the var address as the random cnonce value */
    Zos_SNPrintf(acCnonce, 17, "%08x%08x", Zrandom_N32(), Zrandom_N32());

    strCNnonce = acCnonce;

    return true;
}

bool SipMsgCommon::generateToSBCConnectionId(const std::string & SBCip, const int & SBCport, std::string & connctionId)
{
    if (SBCip.empty())
        return false;

    connctionId.clear();
    connctionId = SBCip + ":" + std::to_string(SBCport);
    return true;
}

bool SipMsgCommon::decodeToSBCConnectionId(const std::string & connctionId,  std::string & SBCip,  int & SBCport)
{
    if(connctionId.empty())
        return false;

    int pos = connctionId.find(":");
    if(pos == -1)
        return false;

    std::string strPort;
    SBCip = connctionId.substr(0, pos);
    strPort = connctionId.substr(pos + 1, connctionId.size() - pos - 1);
    try {
        SBCport = tars::TC_Common::strto<int>(strPort);
    } catch (...) {
        SBCport = 0;
    }

    return  true;
}

bool SipMsgCommon::deleteInternationCode(const std::string & calleeNumber, std::string & newNumber)
{
    if (calleeNumber.find("+") == 0)
    {
        std::string tem = calleeNumber.substr(1, calleeNumber.size() - 1);
        newNumber = tem;
    }
    else if (calleeNumber.find("00") == 0)
    {
        std::string tem = calleeNumber.substr(2, calleeNumber.size() - 2);
        newNumber = tem;
    }
    else if (calleeNumber.find("01") == 0)
    {
        std::string tem = calleeNumber.substr(2, calleeNumber.size() - 2);
        newNumber = tem;
    }
    else if (calleeNumber.find("05") == 0)
    {
        std::string tem = calleeNumber.substr(2, calleeNumber.size() - 2);
        newNumber = tem;
    }
    else if (calleeNumber.find("09") == 0)
    {
        std::string tem = calleeNumber.substr(2, calleeNumber.size() - 2);
        newNumber = tem;
    }
    else if (calleeNumber.find("001") == 0)
    {
        std::string tem =  calleeNumber.substr(3, calleeNumber.size() - 3);
        newNumber = tem;
    }
    else if (calleeNumber.find("002") == 0)
    {
        std::string tem = calleeNumber.substr(3, calleeNumber.size() - 3);
        newNumber = tem;
    }
    else if (calleeNumber.find("009") == 0)
    {
        std::string tem =  calleeNumber.substr(3, calleeNumber.size() - 3);
        newNumber = tem;
    }
    else if (calleeNumber.find("011") == 0)
    {
        std::string tem = calleeNumber.substr(3, calleeNumber.size() - 3);
        newNumber = tem;
    }
    else if (calleeNumber.find("095") == 0)
    {
        std::string tem = calleeNumber.substr(3, calleeNumber.size() - 3);
        newNumber = tem;
    }
    else if (calleeNumber.find("810") == 0)
    {
        std::string tem = calleeNumber.substr(3, calleeNumber.size() - 3);
        newNumber = tem;
    }
    else if (calleeNumber.find("0011") == 0)
    {
        std::string tem = calleeNumber.substr(4, calleeNumber.size() - 4);
        newNumber = tem;
    }
    else
    {
        newNumber = calleeNumber;       //默认国内电话
    }
    return true;
}

bool SipMsgCommon::getDomainIdAndAppId(const std::string & domainIdAndAppId, std::string & domainId, std::string & appId)
{
    //format :  9999:0
    int  pos = domainIdAndAppId.find(":");
    if (pos < 0)
    {
        return false;
    }

    domainId = domainIdAndAppId.substr(0,pos);
    appId = domainIdAndAppId.substr(pos + 1, domainIdAndAppId.size() - (pos + 1));
    return true;
}

bool SipMsgCommon::checkIpValid(const std::string & ip)
{
    if (ip.empty())
        return false;

    int a=-1,b=-1,c=-1,d=-1;
    char s[200]={0};
    sscanf(ip.c_str(),"%d.%d.%d.%d%s",&a,&b,&c,&d,s);
    if(a>255 || a<0 || b>255 || b<0 ||c>255 || c<0 ||d>255 || d<0 )
        return false;

    if(s[0]!=0) return false;

    if (ip == "127.0.0.1")
        return false;
    return true;
}

bool SipMsgCommon::checkVideoCall(const std::string & sipMsg)
{
    int pos = sipMsg.find("video");
    return pos > 0 ? true : false;
}

bool SipMsgCommon::encodeRtpHostPorts(const std::string & ip, const vector<int> & evenPorts, std::string & hostPorts)
{
    //format: ip@port1+port2
    if (ip.empty())
        return false;
    if (evenPorts.size() == 0)
        return false;

    hostPorts = ip + "@";
    unsigned int portSize = evenPorts.size();
    for (unsigned int i = 0; i < portSize; i++)
    {
        hostPorts += std::to_string(evenPorts.at(i));
        if (i != portSize - 1)
        {
            hostPorts += "+";
        }
    }
    return true;
}

bool SipMsgCommon::decodeRtpHostPorts(const std::string & hostPorts, std::string & ip,vector<int> & evenPorts)
{
    int pos1 = hostPorts.find("@");
    if (pos1 < 0)
        return false;

    evenPorts.clear();
    ip = hostPorts.substr(0, pos1);
    std::string strTmpPorts = hostPorts.substr(pos1 + 1, hostPorts.size() - pos1 - 1);
    while(strTmpPorts.find("+") > 0)
    {
        int posAdd = strTmpPorts.find("+");
        std::string port = strTmpPorts.substr(0, posAdd);
        try {
            evenPorts.push_back(tars::TC_Common::strto<int>(port));
        } catch (...) {
            evenPorts.push_back(0);
        }
        strTmpPorts = strTmpPorts.substr(posAdd + 1, strTmpPorts.size() - posAdd - 1);
    }
    int postLast = 0;
    try {
        postLast = tars::TC_Common::strto<int>(strTmpPorts);
    } catch (...) {
        postLast = 0;
    }
    evenPorts.push_back(postLast);
    return true;
}

std::string SipMsgCommon::encodeMapParasLog(const map<std::string, std::string> & params)
{
    std::string strLog;
    map<std::string, std::string>::const_iterator iterKey = params.begin();
    for (; iterKey != params.end(); iterKey++)
    {
        strLog += iterKey->first;
        strLog += ":";
        strLog += iterKey->second;
        strLog += ";";
    }
    return strLog;
}

std::string SipMsgCommon::getCurrentDir()
{
    std::string curDir;
    char *file_path_getcwd;
    file_path_getcwd=(char *)malloc(200);
    getcwd(file_path_getcwd, 200);
    curDir = file_path_getcwd;

    return curDir;
}

bool SipMsgCommon::jsonToMap(const std::string& s, std::map<std::string, std::string>& m)
{
    m.clear();
    if (s.empty())
        return true;

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(s.c_str(), root))
        return false;

    Json::ValueIterator it = root.begin();
    for (; it != root.end(); ++it)
    {
        if (!it->isString())
            return false;
        m[it.memberName()] = it->asCString();
    }
    return true;
}

std::string SipMsgCommon::mapToJson(const std::map<std::string, std::string>& m)
{
    Json::Value root;
    std::map<std::string, std::string>::const_iterator it = m.begin();
    for (; it != m.end(); ++it)
        root[it->first.c_str()] = Json::Value(it->second.c_str());
    Json::FastWriter fastWriter;
    return fastWriter.write(root).c_str();
}

bool SipMsgCommon::setLogLevel(int level)
{
    ZINT ret = Zos_CfgSetLogLevel(toZLogLevel(level));
    return ret == ZOK;
}

ZUINT SipMsgCommon::toZLogLevel(int level)
{
    static const ZUINT logLevels[] =
    {
        ZLOG_LEVEL_ALL,
        ZLOG_LEVEL_FATAL,
        ZLOG_LEVEL_ERROR,
        ZLOG_LEVEL_WARN,
        ZLOG_LEVEL_INFO,
        ZLOG_LEVEL_DBG,
        ZLOG_LEVEL_FUNC
    };

    if (level >= 0 && level < sizeof(logLevels) / sizeof(logLevels[0]))
        return logLevels[level];

    return ZLOG_LEVEL_ALL;
}


