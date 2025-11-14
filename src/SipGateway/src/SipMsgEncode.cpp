#include "SipMsgEncode.h"
#include "servant/Application.h"


extern std::mutex g_mutex;
extern map<void*, std::string> g_mem;

SipMsgEncode::SipMsgEncode()
{
    _pstMsg = NULL;
}

SipMsgEncode::~SipMsgEncode()
{
    if(_pstMsg)
    {
        if (Sip_MsgDelete(_pstMsg) != ZOK)
        {
        }
        _pstMsg = 0;
        _strMsg = "";
    }
}

bool SipMsgEncode::createReq()
{
    if(Sip_MsgCreateReq(&_pstMsg) == ZOK)
        return true;
    return false;
}

bool SipMsgEncode::createRsp()
{
    if(Sip_MsgCreateRsp(&_pstMsg) == ZOK)
        return true;
    return false;
}

/* fill request line */
bool SipMsgEncode::fillReqLine(unsigned char ucUriType, unsigned char ucMethod,
                               const std::string &strUserInfo, const std::string &strHost,
                               int iPort)
{
    switch (ucUriType)
    {
    case EN_SIP_ADDR_SPEC_SIP:
        return fillReqLineBySip(ucMethod, strUserInfo, strHost, iPort);
    case EN_SIP_ADDR_SPEC_SIPS:
        return fillReqLineBySips(ucMethod, strUserInfo, strHost, iPort);
    case EN_SIP_ADDR_SPEC_TEL:
        return fillReqLineByTel(ucMethod, strUserInfo);
    }
    return false;
}

/* fill request line */
bool SipMsgEncode::fillReqLineBySip(unsigned char ucMethod,const std::string &strUserInfo,
                                    const std::string &strHostname,int iPort)
{
    ST_SIP_SIP_URI stSipUri;
    ST_ZOS_SSTR stUserInfo;
    ST_SIP_HOST_PORT stHostPort;
    ST_ZOS_SSTR *pstUserInfo = 0;
    if (strHostname.empty())
    {
        return false;
    }

    SipMsgCommon::str2ZosSstr(strUserInfo, stUserInfo); /* set stUserInfo */
    SipMsgCommon::enHostPort(strHostname, iPort, stHostPort); /* set stHostPort */
    pstUserInfo = strUserInfo.size() > 0 ? &stUserInfo : 0;
    if (Sip_ParmFillSipUri(_pstMsg->zMemBuf, &stSipUri, pstUserInfo, &stHostPort) != ZOK) /* fill stSipUri */
    {
        return false;
    }
    if(strUserInfo.empty())
    {
        stSipUri.ucUserInfoPres = 0;
    }

    if(Sip_MsgFillReqLineBySipUri(_pstMsg, ucMethod, &stSipUri) == ZOK)
    {
        return true;
    }

    return false;
}

bool SipMsgEncode::fillReqLineByUri( ZUCHAR ucMethod,const std::string & strUri)
{
    if (strUri.empty())
        return false;

    ST_ZOS_SSTR  stUri;
    SipMsgCommon::str2ZosSstr(strUri, stUri);

    if (Sip_MsgFillReqLineByUri(_pstMsg,  ucMethod,	 &stUri) != ZOK)
        return false;

    return true;
}

bool SipMsgEncode::fillReqLineSipUriParasCxtId(const vector<ST_SIP_URI_PARM> & params, std::string & reason)
{
    ST_SIP_SIP_URI * pstUri = 0;
    bool result = false;
    std::string strSrc = "CxtId";

    result = SipMsgCommon::getReqSipsUri(_pstMsg, &pstUri);
    if (false == result)
    {
        reason = "getReqSipsUri() failed.";
        return false;
    }

    for (unsigned int i = 0; i < params.size(); i++)
    {
        ST_SIP_URI_PARM param = params.at(i);
        if (param.ucType == EN_SIP_URI_PARM_OTHER)
        {
            std::string strName;
            SipMsgCommon::zosSstr2Str(param.u.stOther.stName, strName);
            if (tars::TC_Common::trim(strSrc) == tars::TC_Common::trim(strName))
            {
                return parmFillSipUriOthers(_pstMsg->zMemBuf, pstUri, param.u.stOther.stName, param.u.stOther.stVal);
            }
        }
    }

    reason = "not exist.";
    return false;
}

bool SipMsgEncode::fillReqLineSipUriParasCxtId(const map<std::string, std::string> params, std::string & reason)
{
    ST_SIP_SIP_URI * pstUri = 0;
    bool result = false;
    std::string strName = "CxtId";
    if (params.size() == 0)
    {
        reason = "params.size() == 0";
        return false;
    }

    result = SipMsgCommon::getReqSipsUri(_pstMsg, &pstUri);
    if (false == result)
    {
        reason = "getReqSipsUri() failed.";
        return false;
    }

    map<std::string, std::string>::const_iterator iterPara = params.begin();
    for (; iterPara != params.end(); iterPara++)
    {
        if (iterPara->first == "CxtId")
        {
            return parmFillSipUriOthers(_pstMsg->zMemBuf, pstUri, iterPara->first, iterPara->second);
        }
    }
    reason = "not exist.";
    return false;
}


/* fill request line */
bool SipMsgEncode::fillReqLineBySipX(unsigned char ucMethod, unsigned char ucUriType, void *pstUri)
{
    if(pstUri)
    {
        switch(ucUriType)
        {
        case EN_SIP_ADDR_SPEC_SIP:
            {
                if(Sip_MsgFillReqLineBySipUri(_pstMsg, ucMethod, (ST_SIP_SIP_URI *)pstUri) == ZOK)
                    return true;
                break;
            }
        case EN_SIP_ADDR_SPEC_SIPS:
            {
                if(Sip_MsgFillReqLineBySipsUri(_pstMsg, ucMethod, (ST_SIP_SIP_URI *)pstUri) == ZOK)
                    return true;
                break;
            }
        case EN_SIP_ADDR_SPEC_TEL:
            {
                if(Sip_MsgFillReqLineByTelUri(_pstMsg, ucMethod,(ST_SIP_TEL_URI *)pstUri) == ZOK)
                    return true;
                break;
            }
        case EN_SIP_ADDR_SPEC_IM:
            {
                break;
            }
        }
    }
    return false;
}

/* fill request sips line */
bool SipMsgEncode::fillReqLineBySips(unsigned char ucMethod,const std::string &strUserInfo, const std::string &strHostname,int iPort)
{
    ST_SIP_SIP_URI stSipUri;
    ST_ZOS_SSTR stUserInfo;
    ST_SIP_HOST_PORT stHostPort;
    ST_ZOS_SSTR *pstUserInfo = 0;

    SipMsgCommon::str2ZosSstr(strUserInfo, stUserInfo);
    SipMsgCommon::enHostPort(strHostname, iPort, stHostPort);
    pstUserInfo = strUserInfo.size() > 0 ? &stUserInfo : 0;
    Sip_ParmFillSipsUri(_pstMsg->zMemBuf, &stSipUri, pstUserInfo, &stHostPort);

    if(strUserInfo.empty())
    {
        stSipUri.ucUserInfoPres = 0;
    }

    if(Sip_MsgFillReqLineBySipsUri(_pstMsg, ucMethod, &stSipUri) == ZOK)
    {
        return true;
    }
    return true;
}

/* fill request tel line */
bool SipMsgEncode::fillReqLineByTel(unsigned char ucMethod, const std::string &strNumber)
{
    ST_SIP_TEL_URI stTelUri;
    ST_ZOS_SSTR stNumber;
    if (strNumber.empty())
        return false;

    SipMsgCommon::str2ZosSstr(strNumber, stNumber);
    stTelUri.ucGlobalNumberPres = 0;
    Sip_ParmFillTelUri(_pstMsg->zMemBuf, &stTelUri, ZFALSE, &stNumber);
    if(Sip_MsgFillReqLineByTelUri(_pstMsg, ucMethod, &stTelUri) == ZOK)
    {
        return true;
    }
    return false;
}

/* fill status line */
bool SipMsgEncode::fillStatusLine(int iStatusCode)
{
    if(Sip_MsgFillStatusLine(_pstMsg, iStatusCode) == ZFAILED)
        return false;
    return true;
}


bool SipMsgEncode::fillReqLineByIp( ZUCHAR ucMethod, ST_ZOS_SSTR *pstUserInfo, const std::string & strDotIpAddr, const int & iPort)
{
    ST_ZOS_INET_ADDR  inetAddr;
    /* set stAddr */
    ZOS_IPV4_ADDR_SET_STRX(&inetAddr,strDotIpAddr.c_str(), iPort);
    if (Sip_MsgFillReqLineByIp(_pstMsg,ucMethod, pstUserInfo, &inetAddr) == ZOK)
        return true;
    return  false;
}

bool SipMsgEncode::fillReqLineBySipUri(ZUCHAR ucMethod, ST_SIP_SIP_URI *pstSipUri)
{
    if (Sip_MsgFillReqLineBySipUri(_pstMsg, ucMethod, pstSipUri) == ZOK)
        return true;
    return false;
}

bool SipMsgEncode::fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int dwPort, ZBOOL bRport,
                              int iRport, const std::string &strBranch)
{
    ST_ZOS_INET_ADDR stAddr;
    ST_ZOS_SSTR stBranch;

    /* check stBranch */
    if(strBranch.find("z9hG4bK") != 0)
    {
        TLOGWARN("[SipMsgEncode] the format of strBranch is error, Branch :" << strBranch << endl);
        return false;
    }
    SipMsgCommon::str2ZosSstr(strBranch, stBranch);
    /* set stAddr */
    ZOS_IPV4_ADDR_SET_STRX(&stAddr,strHostname.c_str(),dwPort);

    if(Sip_MsgFillHdrVia(_pstMsg, ucTptType, &stAddr, &stBranch, bRport, iRport) == ZOK)
    {
        //linux下上述填充会出现ip地址为0或者其他随机ip的情况。 因此需要重置，否则无法挂断
        ST_SIP_HDR_VIA *pstHdrVia = 0;
        ST_SIP_VIA_PARM *pstViaParm = 0;
        std::string strHost;
        int iPort = 0;

        pstHdrVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
        if (!pstHdrVia)
            return false;

        pstViaParm = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstHdrVia->stParmLst);
        if (!pstViaParm)
            return false;

        /* get ucTptType */
        Sip_ParmPickViaSentProtocol(pstViaParm, &ucTptType);
        if (pstHdrVia != 0)
        {
            if(pstViaParm !=0 )
            {
                pstViaParm->stBy.stHost.u.iIpv4Addr = stAddr.u.iIp;
            }
        }
        return true;
    }
    return false;
}

bool SipMsgEncode::fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
                              int iRport, const std::string &strBranch, const std::string & received, int recvPort)
{
    ST_SIP_VIA_PARM *pstParm;
    ST_SIP_HDR_VIA *pstVia;
    ST_ZOS_INET_ADDR stAddr;
    ST_ZOS_SSTR stBranch;

    pstVia = (ST_SIP_HDR_VIA *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (!pstVia) return false;

    /* add the parameter */
    if (Sip_ParmViaLstAdd(_pstMsg->zMemBuf, &pstVia->stParmLst, &pstParm) != ZOK)
        return false;

    /* fill via sent-protocol parameter */
    if (Sip_ParmFillViaSentProtocol(_pstMsg->zMemBuf, pstParm, ucTptType) != ZOK)
    {
        Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
        return false;
    }

    /* fill via sent-by parameter */
    char * pHostname = strHostname.size() > 0 ? (char*)strHostname.c_str() : 0;
    ZOS_IPV4_ADDR_SET_STRX(&stAddr, pHostname, dwPort);
    if (Sip_ParmFillViaSentBy(pstParm, &stAddr) != ZOK)
    {
        Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
        return false;
    }

    /* fill via branch parameter */
    SipMsgCommon::str2ZosSstr(strBranch, stBranch);
    if (Sip_ParmFillViaBranch(_pstMsg->zMemBuf, pstParm, &stBranch) != ZOK)
    {
        Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
        return false;
    }

    /* fill via alias parameter */
    if (ucTptType != EN_SIP_TPT_UDP && ucTptType != EN_SIP_TPT_OTHER
        && Sip_CfgGetSuptReUseConn())
    {
        if (Sip_ParmFillViaAlias(_pstMsg->zMemBuf, pstParm) != ZOK)
        {
            Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
            return false;
        }
    }

    /* check has rport */
    if (bRport)
    {
        /* fill via rport parameter */
        if (Sip_ParmFillViaRport(_pstMsg->zMemBuf, pstParm, iRport) != ZOK)
        {
            Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
            return false;
        }
    }

    if (received.size() > 1)
    {
        // received
        ST_ZOS_INET_ADDR  stRecvAddr;
        ZOS_IPV4_ADDR_SET_STRX(&stRecvAddr, received.c_str(), recvPort);
        if (Sip_ParmFillViaRecv(_pstMsg->zMemBuf, pstParm, &stRecvAddr) != ZOK)
        {
            Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
            return false;
        }
    }

    return true;
}


bool SipMsgEncode::fillHdrViaExtParams(const vector<ST_SIP_VIA_PARMS> & prams)
{
    ST_SIP_HDR_VIA *pstHdrVia = 0;
    ST_SIP_VIA_PARM *pstViaParm = 0;
    std::string strHost;
    int iPort = 0;
    ST_SIP_VIA_PARMS *pstParm = 0;

    pstHdrVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (0 == pstHdrVia)
        return false;

    pstViaParm = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstHdrVia->stParmLst);
    if (pstViaParm == 0)
        return false;

    for (unsigned int i = 0; i <prams.size(); i++)
    {
        ST_SIP_VIA_PARMS stPrams = prams.at(i);
        if (stPrams.ucType == EN_SIP_VIA_PARMS_EXT)
        {
            if (Sip_ParmViasLstAdd(_pstMsg->zMemBuf, &pstViaParm->stParmsLst, EN_SIP_VIA_PARMS_EXT, &pstParm) != ZOK)
            {
                return false;
            }

            Zos_SStrXCpy(_pstMsg->zMemBuf, &pstParm->u.stExt.stName, &stPrams.u.stExt.stName);
            if (stPrams.u.stExt.ucValPres)
            {
                pstParm->u.stExt.ucValPres = ZTRUE;
                pstParm->u.stExt.stGenVal.ucType = stPrams.u.stExt.stGenVal.ucType;
                switch(stPrams.u.stExt.stGenVal.ucType)
                {
                case EN_SIP_GEN_VAL_VAL:            /**< @brief Type for value. */
                    Zos_SStrXCpy(_pstMsg->zMemBuf, &pstParm->u.stExt.stGenVal.u.stVal, &stPrams.u.stExt.stGenVal.u.stVal);
                    break;

                case EN_SIP_GEN_VAL_HOST:             /**< @brief Type for host. */
                    Sip_CpyHost(_pstMsg->zMemBuf, &pstParm->u.stExt.stGenVal.u.stHost, &stPrams.u.stExt.stGenVal.u.stHost);
                    break;

                case EN_SIP_GEN_VAL_QSTR:              /**< @brief Type for qouted string. */
                    Zos_SStrXCpy(_pstMsg->zMemBuf, &pstParm->u.stExt.stGenVal.u.stQStr, &stPrams.u.stExt.stGenVal.u.stQStr);
                    break;
                }
            }
        }
    }
    return true;
}

bool SipMsgEncode::fillHdrVia(const ST_SIP_HDR_VIA & stVia)
{
     ST_SIP_HDR_VIA *pstVia = (ST_SIP_HDR_VIA *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (!pstVia)
        return false;

    ABNF_LIST_CREATE(&pstVia->stParmLst);
    if (Sip_CpyHdrVia(_pstMsg->zMemBuf, pstVia, (ST_SIP_HDR_VIA *)&stVia) == ZOK)
        return true;
    return false;
}

bool SipMsgEncode::fillHdrVias(const vector<ST_SIP_HDR_VIA> & stVias)
{
    for (unsigned int i = 0; i < stVias.size(); i++)
    {
        ST_SIP_HDR_VIA *pstVia = (ST_SIP_HDR_VIA *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
        if (!pstVia)
            return false;

        const ST_SIP_HDR_VIA & stVia = stVias.at(i);
        ABNF_LIST_CREATE(&pstVia->stParmLst);
        if (Sip_CpyHdrVia(_pstMsg->zMemBuf, pstVia, (ST_SIP_HDR_VIA *)&stVia) != ZOK)
            return false;
    }
    return true;
}

bool SipMsgEncode::fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
                               int iRport, const std::string &strBranch, const std::string & received, int recvPort,
                               const vector<ST_SIP_VIA_PARMS> & prams)
{
    ST_SIP_VIA_PARM *pstParm;
    ST_SIP_HDR_VIA *pstVia;
    ST_ZOS_INET_ADDR stAddr;
    ST_ZOS_SSTR stBranch;

    pstVia = (ST_SIP_HDR_VIA *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (!pstVia) return false;

    /* add the parameter */
    if (Sip_ParmViaLstAdd(_pstMsg->zMemBuf, &pstVia->stParmLst, &pstParm) != ZOK)
        return false;

    /* fill via sent-protocol parameter */
    if (Sip_ParmFillViaSentProtocol(_pstMsg->zMemBuf, pstParm, ucTptType) != ZOK)
    {
        Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
        return false;
    }

    /* fill via sent-by parameter */
    char * pHostname = strHostname.size() > 0 ? (char*)strHostname.c_str() : 0;
    ZOS_IPV4_ADDR_SET_STRX(&stAddr, pHostname, dwPort);
    if (Sip_ParmFillViaSentBy(pstParm, &stAddr) != ZOK)
    {
        Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
        return false;
    }

    /* fill via branch parameter */
    SipMsgCommon::str2ZosSstr(strBranch, stBranch);
    if (Sip_ParmFillViaBranch(_pstMsg->zMemBuf, pstParm, &stBranch) != ZOK)
    {
        Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
        return false;
    }

    /* fill via alias parameter */
    if (ucTptType != EN_SIP_TPT_UDP && ucTptType != EN_SIP_TPT_OTHER
        && Sip_CfgGetSuptReUseConn())
    {
        if (Sip_ParmFillViaAlias(_pstMsg->zMemBuf, pstParm) != ZOK)
        {
            Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
            return false;
        }
    }

    /* check has rport */
    if (bRport)
    {
        /* fill via rport parameter */
        if (Sip_ParmFillViaRport(_pstMsg->zMemBuf, pstParm, iRport) != ZOK)
        {
            Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
            return false;
        }
    }

    if (received.size() > 1)
    {
        // received
        ST_ZOS_INET_ADDR  stRecvAddr;
        ZOS_IPV4_ADDR_SET_STRX(&stRecvAddr, received.c_str(), recvPort);
        if (Sip_ParmFillViaRecv(_pstMsg->zMemBuf, pstParm, &stRecvAddr) != ZOK)
        {
            Sip_ParmViaLstRmv(&pstVia->stParmLst, pstParm);
            return false;
        }
    }

    ST_SIP_VIA_PARM *pstViaParm = 0;
    ST_SIP_VIA_PARMS *pstParms = 0;

    pstViaParm = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstVia->stParmLst);
    if (pstViaParm == 0)
        return false;

    for (unsigned int i = 0; i <prams.size(); i++)
    {
        ST_SIP_VIA_PARMS stPrams = prams.at(i);
        if (stPrams.ucType == EN_SIP_VIA_PARMS_EXT)
        {
            if (Sip_ParmViasLstAdd(_pstMsg->zMemBuf, &pstViaParm->stParmsLst, EN_SIP_VIA_PARMS_EXT, &pstParms) != ZOK)
            {
                return false;
            }

            Zos_SStrXCpy(_pstMsg->zMemBuf, &pstParms->u.stExt.stName, &stPrams.u.stExt.stName);
            if (stPrams.u.stExt.ucValPres)
            {
                pstParms->u.stExt.ucValPres = ZTRUE;
                pstParms->u.stExt.stGenVal.ucType = stPrams.u.stExt.stGenVal.ucType;
                switch(stPrams.u.stExt.stGenVal.ucType)
                {
                case EN_SIP_GEN_VAL_VAL:            /**< @brief Type for value. */
                    Zos_SStrXCpy(_pstMsg->zMemBuf, &pstParms->u.stExt.stGenVal.u.stVal, &stPrams.u.stExt.stGenVal.u.stVal);
                    break;

                case EN_SIP_GEN_VAL_HOST:             /**< @brief Type for host. */
                    Sip_CpyHost(_pstMsg->zMemBuf, &pstParms->u.stExt.stGenVal.u.stHost, &stPrams.u.stExt.stGenVal.u.stHost);
                    break;

                case EN_SIP_GEN_VAL_QSTR:              /**< @brief Type for qouted string. */
                    Zos_SStrXCpy(_pstMsg->zMemBuf, &pstParms->u.stExt.stGenVal.u.stQStr, &stPrams.u.stExt.stGenVal.u.stQStr);
                    break;
                }
            }
        }
    }
    return true;
}

/* fill header Supported */
bool SipMsgEncode::fillHdrSupted(vector<int> iSuptFlags)
{
    vector<int>::iterator it;
    ZULONG dwSuptFlag = 0;
    ZOS_ZERO(dwSuptFlag);

    for(it = iSuptFlags.begin(); it != iSuptFlags.end(); it++)
    {
        ZOS_SET(*it, dwSuptFlag);
    }

    if(Sip_MsgFillHdrSupted(_pstMsg, dwSuptFlag) == ZOK)
    {
        return true;
    }
    return false;
}

/* fill header Record-Route */
bool SipMsgEncode::fillHdrRoute(ST_SIP_ROUTE_PARM & stRouteParams)
{
    ST_SIP_HDR_ROUTE *pstRoute;
    ST_SIP_ROUTE_PARM *pstRouteParm;

    /* create header record-route */
    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    if(!pstRoute)
        return false;

    /* add the parameter */
    Sip_ParmRouteLstAdd(_pstMsg->zMemBuf, &pstRoute->stParmLst, &pstRouteParm);
    if(!pstRouteParm)
        return false;

    if(Sip_CpyRouteParm(_pstMsg->zMemBuf, pstRouteParm, &stRouteParams)!= ZOK)
        return false;

    return true;
}

/* fill header Route */
bool SipMsgEncode::fillHdrRoute(const std::string &strUserName, const std::string &strHost,
                                int iPort, unsigned char ucTptType)
{
    ST_SIP_HDR_ROUTE *pstRoute;
    ST_ZOS_INET_ADDR stInetAddr;
    ST_SIP_ROUTE_PARM *pstRouteParm;
    ST_SIP_URI_PARM *pstUriParm;
    void *zMemBuf;

    /* create header record-route */
    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    if(!pstRoute) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* set stInetAddr */
    SipMsgCommon::enIpv4Addr(strHost, iPort, stInetAddr);

    /* add the parameter */
    Sip_ParmRouteLstAdd(zMemBuf, &pstRoute->stParmLst, &pstRouteParm);
    if(!pstRouteParm) return false;

    /* set display name */
    Sip_ParmFillAddrSpecByIp(zMemBuf, &pstRouteParm->stNameAddr.stAddrSpec, ZNULL, &stInetAddr);

    /* fill the transport */
    Sip_ParmFillSipUriTpt(zMemBuf, &pstRouteParm->stNameAddr.stAddrSpec.u.stSipUri, ucTptType);

    /* fill the lr */
    Sip_ParmFillSipUriLr(zMemBuf, &pstRouteParm->stNameAddr.stAddrSpec.u.stSipUri);

    return true;
}

bool SipMsgEncode::fillHdrRoute(const std::string &strUserName, const std::string &strHost,
                                int iPort, unsigned char ucTptType, std::string & reason)
{
    ST_SIP_HDR_ROUTE *pstRoute;
    //ST_ZOS_INET_ADDR stInetAddr;
    ST_SIP_ROUTE_PARM *pstRouteParm;
    //ST_SIP_URI_PARM *pstUriParm;
    void *zMemBuf;
    ST_SIP_SIP_URI stSipUri;
    bool result = false;

    /* create header record-route */
    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    if(!pstRoute) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    //* add the parameter */
    Sip_ParmRouteLstAdd(zMemBuf, &pstRoute->stParmLst, &pstRouteParm);
    if(!pstRouteParm) return false;

    result = ParmFillSipUri(&stSipUri, strUserName, strHost, iPort);
    if (false == result)
    {
        reason = "ParmFillSipUri() failed, strHost:" + strHost + ",port:" + std::to_string(iPort);
        return false;
    }

    result =  parmFillSipUriLr(_pstMsg->zMemBuf, &stSipUri);
    //result =  parmFillSipUriTpt(_pstMsg->zMemBuf, &stSipUri, EN_SIP_URI_PARM_LR);
    if (false == result)
    {
        reason = "parmFillSipUriTpt() failed, strHost:" + strHost + ",port:" + std::to_string(iPort);
        return false;
    }

    pstRouteParm->stNameAddr.ucDispNamePres = 0;
    pstRouteParm->stNameAddr.stAddrSpec.ucType = EN_SIP_REQ_URI_SIP;
    pstRouteParm->stNameAddr.stAddrSpec.u.stSipUri = stSipUri;

    return true;
}

bool SipMsgEncode::fillHdrRouteBySipUri(const vector<ST_SIP_SIP_URI> & stSipUris, bool bSipCallRpc)
{
    ST_SIP_HDR_ROUTE *pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    if (!pstRoute)
        return false;

    ABNF_LIST_CREATE(&pstRoute->stParmLst);
    int size = stSipUris.size();
    if (bSipCallRpc)
    {
        for (unsigned int i = 0; i < stSipUris.size(); i++)
        {
            const ST_SIP_SIP_URI & sipUri = stSipUris.at(i);
            void * pTem = 0;
            if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstRoute->stParmLst, sizeof(ST_SIP_ROUTE_PARM),&pTem) == ZOK)
            {
                ST_SIP_ROUTE_PARM *pPram = (ST_SIP_ROUTE_PARM*)pTem;
                if (0 == pPram)
                    continue;
                pPram->stNameAddr.ucDispNamePres = 0;
                pPram->stNameAddr.stAddrSpec.ucType = EN_SIP_ADDR_SPEC_SIP;
                pPram->stNameAddr.stAddrSpec.u.stSipUri = sipUri;
            }
        }
    }
    else
    {
        for (unsigned int i = size; i > 0; i--)
        {
            const ST_SIP_SIP_URI & sipUri = stSipUris.at(i - 1);
            void * pTem = 0;
            if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstRoute->stParmLst, sizeof(ST_SIP_ROUTE_PARM),&pTem) == ZOK)
            {
                ST_SIP_ROUTE_PARM *pPram = (ST_SIP_ROUTE_PARM*)pTem;
                if (0 == pPram)
                    continue;
                pPram->stNameAddr.ucDispNamePres = 0;
                pPram->stNameAddr.stAddrSpec.ucType = EN_SIP_ADDR_SPEC_SIP;
                pPram->stNameAddr.stAddrSpec.u.stSipUri = sipUri;
            }
        }
    }
    return true;
}
bool SipMsgEncode::fillHdrRecordRouteBySipUri(const ST_SIP_SIP_URI & stSipUri)
{
    ST_SIP_HDR_ROUTE *pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_RECORD_ROUTE);
    if (!pstRoute)
        return false;
    ABNF_LIST_CREATE(&pstRoute->stParmLst);
    void * pTem = 0;
    if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstRoute->stParmLst, sizeof(ST_SIP_ROUTE_PARM),&pTem) == ZOK)
    {
        ST_SIP_ROUTE_PARM *pPram = (ST_SIP_ROUTE_PARM*)pTem;
        if (0 == pPram)
            return false;
        pPram->stNameAddr.ucDispNamePres = 0;
        pPram->stNameAddr.stAddrSpec.ucType = EN_SIP_ADDR_SPEC_SIP;
        pPram->stNameAddr.stAddrSpec.u.stSipUri = stSipUri;
    }
    return true;
}

/* fill header Record-Route */
bool SipMsgEncode::fillHdrRecordRoute(const std::string &strUserName, const std::string &strHost,
                                      int iPort, unsigned char ucTptType)
{
    ST_SIP_HDR_ROUTE *pstRoute;
    ST_ZOS_INET_ADDR stInetAddr;
    ST_SIP_ROUTE_PARM *pstRouteParm;
    ST_SIP_URI_PARM *pstUriParm;
    void *zMemBuf;

    /* create header record-route */
    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_RECORD_ROUTE);
    if(!pstRoute) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* set stInetAddr */
    SipMsgCommon::enIpv4Addr(strHost, iPort, stInetAddr);

    /* add the parameter */
    Sip_ParmRouteLstAdd(zMemBuf, &pstRoute->stParmLst, &pstRouteParm);
    if(!pstRouteParm) return false;

    /* set display name */
    Sip_ParmFillAddrSpecByIp(zMemBuf, &pstRouteParm->stNameAddr.stAddrSpec, ZNULL, &stInetAddr);

    /* fill the transport */
    Sip_ParmFillSipUriTpt(zMemBuf, &pstRouteParm->stNameAddr.stAddrSpec.u.stSipUri, ucTptType);

    /* fill the lr */
    Sip_ParmFillSipUriLr(zMemBuf, &pstRouteParm->stNameAddr.stAddrSpec.u.stSipUri);

    return true;
}


/* fill header Record-Route */
bool SipMsgEncode::fillHdrRecordRoute(ST_SIP_ROUTE_PARM & stRouteParams)
{
    ST_SIP_HDR_ROUTE *pstRoute;
    ST_SIP_ROUTE_PARM *pstRouteParm;

    /* create header record-route */
    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_RECORD_ROUTE);
    if(!pstRoute)
        return false;

    /* add the parameter */
    Sip_ParmRouteLstAdd(_pstMsg->zMemBuf, &pstRoute->stParmLst, &pstRouteParm);
    if(!pstRouteParm)
        return false;

    if(Sip_CpyRouteParm(_pstMsg->zMemBuf, pstRouteParm, &stRouteParams)!= ZOK)
        return false;

    return true;
}

/* fill Max-Forwards header */
bool SipMsgEncode::fillHdrForwards(int iMaxForwards)
{
    if(Sip_FillExtHdr(_pstMsg, (char*)"Max-Forwards", (ZCHAR *)std::to_string(iMaxForwards).c_str()) == ZOK)
    {
        return true;
    }
    return false;
}

/* fill header From */
bool SipMsgEncode::fillHdrFromToByName(bool bFrom,const std::string &strDispName,const std::string &strUserInfo,
                                       const std::string &strHostname, int iPort,
                                       const std::string &strTag)
{
    ZCHAR * pzcDispName = strDispName.size() > 0 ? (ZCHAR *)strDispName.c_str() : 0;
    ZCHAR * pzcUserInfo = strUserInfo.size() > 0 ? (ZCHAR *)strUserInfo.c_str() : 0;
    ZCHAR * pzcHostname = strHostname.size() > 0 ? (ZCHAR *)strHostname.c_str() : 0;
    ZCHAR * pzcTag = strTag.size() > 0 ? (ZCHAR *)strTag.c_str() : 0;

    if(Sip_MsgFillHdrFromToByNameX(_pstMsg, bFrom, pzcDispName, pzcUserInfo, pzcHostname, iPort, pzcTag) == ZOK)
    {
        return true;
    }
    return false;
}

bool  SipMsgEncode::fillHdrFromToBySipsName(bool bFromHdr, const std::string & strDispName, const std::string & strUserInfo, const std::string & strHostname,
                                            int iPort, const std::string & strTag)
{
    ST_ZOS_SSTR stDispName, stUserInfo, stHostName,stTag ;
    SipMsgCommon::str2ZosSstr(strDispName,stDispName);
    SipMsgCommon::str2ZosSstr(strUserInfo,stUserInfo);
    SipMsgCommon::str2ZosSstr(strHostname,stHostName);
    SipMsgCommon::str2ZosSstr(strTag,stTag);

    ST_ZOS_SSTR * pstDispName = strDispName.size() > 0 ? &stDispName : 0;
    ST_ZOS_SSTR * pstUserInfo = strUserInfo.size() > 0 ? &stUserInfo : 0;
    ST_ZOS_SSTR * pstHostname = strHostname.size() > 0 ? &stHostName : 0;
    ST_ZOS_SSTR * pstTag = strTag.size() > 0 ? &stTag : 0;

    if (Sip_MsgFillHdrFromToBySipsName(_pstMsg, bFromHdr, pstDispName, pstUserInfo, pstHostname, iPort, pstTag) != ZOK)
        return false;

    return true;
}

bool SipMsgEncode::fillHdrFromToTelUri(bool bFromHdr,const std::string &strDispName, const std::string &strNumber,
                                       const std::string &strTag)
{
    ST_SIP_TEL_URI stTelUri;
    ST_ZOS_SSTR stDispName, stTag;

    /* set dispname */
    SipMsgCommon::str2ZosSstr(strDispName, stDispName);

    /* set number */
    stTelUri.ucGlobalNumberPres = ZFALSE;
    ABNF_LIST_CREATE(&(stTelUri.u.stGlobal.stParLst));
    SipMsgCommon::str2ZosSstr(strNumber, stTelUri.u.stGlobal.stNumberDigits);

    /* set tag */
    SipMsgCommon::str2ZosSstr(strTag, stTag);

    if(Sip_MsgFillHdrFromToByTelUri(_pstMsg, bFromHdr, &stDispName, &stTelUri, &stTag) == ZFAILED)
        return false;

    return true;
}


bool SipMsgEncode::fillHdrFromToByName(bool bFrom, const std::string &strDispname,const std::string &strUserInfo,
                                       const std::string &strHostname, int iPort, const std::string &strTag, unsigned char &ucReqUriType)
{
    bool result = false;

    switch(ucReqUriType)
    {
    case EN_SIP_ADDR_SPEC_SIP:
        result = fillHdrFromToByName(bFrom, strDispname ,strUserInfo, strHostname,iPort, strTag);
        if (!result)
        {
            TLOGWARN("[SipMsgEncode] fillHdrFromToByName() failed.strDispname:" << strDispname << ",strUserInfo:" << strUserInfo << ",strHostname:" << strHostname << ",iPort:" << iPort << ",strTag:" << strTag << endl);
        }
        break;

    case EN_SIP_ADDR_SPEC_SIPS:
        result = fillHdrFromToBySipsName(bFrom,  strDispname,  strUserInfo, strHostname,  iPort,  strTag);
        if (!result)
        {
            TLOGWARN("[SipMsgEncode] fillHdrFromToBySipsName() failed.strDispname:" << strDispname << ",strUserInfo:" << strUserInfo << ",strHostname:" << strHostname << ",iPort:" << iPort << ",strTag:" << strTag << endl);
        }
        break;

    case EN_SIP_ADDR_SPEC_TEL:
        result = fillHdrFromToTelUri(bFrom, strDispname, strUserInfo,strTag);
        if (!result)
        {
            TLOGWARN("[SipMsgEncode] fillHdrFromToTelUri() failed.strDispname:" << strDispname << ",strUserInfo:" << strUserInfo << ",iPort:" << iPort << ",strTag:" << strTag << endl);
        }
        break;

        // others is not supported for this moment

    default:
        return false;
    }
    return result;
}


/* fill header call-id */
bool SipMsgEncode::fillHdrCallId(const std::string &strCallNumber, const std::string &strCallHost)
{
    ST_SIP_CALLID stCallId;
    SipMsgCommon::enCallId(strCallNumber, strCallHost, stCallId);
    if(Sip_MsgFillHdrCallId(_pstMsg, &stCallId) == ZOK)
    {
        return true;
    }
    return false;
}

/* fill header call-id */
bool  SipMsgEncode::fillHdrCallId( ST_SIP_CALLID *pstCallId)
{
    if (Sip_MsgFillHdrCallId(_pstMsg,pstCallId) == ZOK)
        return true;
    return false;
}


/* fill header cseq */
bool SipMsgEncode::fillHdrCseq(unsigned char ucMethod, int iCseq)
{
    if(Sip_MsgFillHdrCSeq(_pstMsg, ucMethod, iCseq) == ZOK)
    {
        return true;
    }
    return false;
}

/* fill Authorization header */
bool SipMsgEncode::fillHdrAuthor(StSipAuthor stAuth)
{
    ST_SIP_CREDENTS stCredents;
    ST_SIP_DIGEST_RSP stRsp;
    ZCBUF zCredBuf;
    ST_ZOS_SSTR stOpaque;
    /* alloc credential buffer */

    zCredBuf = _pstMsg->zMemBuf;
    /* create digest response list */
    ABNF_LIST_CREATE(&stRsp.stDigRspLst);

    if(!stAuth._strUsername.empty())
        Sip_ParmFillDRspUserNameX(zCredBuf, &stRsp, (ZCHAR *)stAuth._strUsername.c_str()); /* fill username */
    if(!stAuth._strRealm.empty())
        Sip_ParmFillDRspRealmX(zCredBuf, &stRsp, (ZCHAR *)stAuth._strRealm.c_str());       /* fill realm */
    if(!stAuth._strNonce.empty())
        Sip_ParmFillDRspNonceX(zCredBuf, &stRsp, (ZCHAR *)stAuth._strNonce.c_str());       /* fill nonce */
    if(!stAuth._strCnonce.empty())
        Sip_ParmFillDRspCnonceX(zCredBuf, &stRsp, (ZCHAR *)stAuth._strCnonce.c_str());     /* fill cnonce */
    if(!stAuth._strNc.empty())
        Sip_ParmFillDRspNonceCountX(zCredBuf, &stRsp, (ZCHAR *)stAuth._strNc.c_str());     /* fill nonce count */
    if(stAuth._ucReqUri)
        Sip_ParmFillDRspUri(zCredBuf, &stRsp, &stAuth._stReqUri);                          /* fill uri */
    if(!stAuth._strResponse.empty())
        Sip_ParmFillDRspRsp(zCredBuf, &stRsp, (ZCHAR *)stAuth._strResponse.c_str());       /* fill response */
    if(!stAuth._strOpaque.empty())
    {
        SipMsgCommon::str2ZosSstr(stAuth._strOpaque, stOpaque);
        Sip_ParmFillDRspOpaque(zCredBuf, &stRsp, &stOpaque);                               /* fill opaque */
    }
    Sip_ParmFillDRspQopX(zCredBuf, &stRsp, stAuth._ucQop);                             /* fill qop */
    Sip_ParmFillDRspAlgoX(zCredBuf, &stRsp, stAuth._ucAlgo);                           /* fill algo */
    stCredents.ucDigestRspPres = stAuth._ucDigst;

    Zos_MemCpy(&stCredents.u.stDigestRsp, &stRsp, sizeof(ST_SIP_DIGEST_RSP));

    if(Sip_MsgFillHdrAuthor(_pstMsg, &stCredents) == ZOK)
    {
        return true;
    }
    return false;
}


/* fill header Authorization, after 401 message */
bool SipMsgEncode::fillHdrAuthor(const ST_SIP_DIGEST &stDigest, const std::string & strUserName,const std::string & strPassWd,
                                 const unsigned char & ucMethod, const ST_SIP_REQ_URI & stReqUri)
{
    void *zCredBuf;
    ST_SIP_DIGEST_RSP stRsp;
    ST_SIP_DIGEST stDigestTem = stDigest;
    /* allocate memory */
    zCredBuf = _pstMsg->zMemBuf;

    /* create digest response list */
    ABNF_LIST_CREATE(&stRsp.stDigRspLst);

    /* copy response from challenge */
    //if(getWwwDigest(stDigest))
    //{
    Sip_CpyDigestToRsp(zCredBuf, &stRsp, &stDigestTem);
    //}
    //Mrf_AuthCalcRsp           Mrf_SipAddAuthor
    /* calculate md5 */  //之后可以考虑将 authCalcRsp 函数都移动到SIPMsgCommon中去
    //authCalcRsp(strUserName, strPassWd, ucMethod, &stDigestTem, &stReqUri, &stRsp);

    /* fill authorization header */
    ST_SIP_CREDENTS stCred;
    Zos_MemCpy(&stCred.u.stDigestRsp, &stRsp,sizeof(ST_SIP_DIGEST_RSP));
    Sip_MsgFillHdrAuthor(_pstMsg, &stCred);

    return true;
}


bool SipMsgEncode::fillHdrAuthor(const ST_SIP_DIGEST_RSP & stDigestResp, int rspCode)
{

    ST_SIP_CREDENTS stCred;
    int result = 0;
    stCred.ucDigestRspPres = 1;
    Zos_MemCpy(&stCred.u.stDigestRsp, &stDigestResp,sizeof(ST_SIP_DIGEST_RSP));
    if (rspCode == 401)
    {
        result = Sip_MsgFillHdrAuthor(_pstMsg, &stCred);
    }
    else if (rspCode == 407)
    {
        result = Sip_MsgFillHdrProxyAuthor(_pstMsg, &stCred);
    }

    return result == ZOK ? true : false;
}


/* fill contact header */
bool SipMsgEncode::fillHdrContact(const std::string &strUserName,const std::string &strHost, int iPort, unsigned char ucTptType)
{
    ST_SIP_HDR_CONTACT *pstContact;
    ST_SIP_CONTACT_PARM *pstContactParm;
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ST_ZOS_SSTR stUserName, stHost;
    ST_ZOS_SSTR *pstUserName, *pstHost;
    ZDBUF zMemBuf;

    if (strHost.empty())
    {
        TLOGWARN("[SipMsgEncode] " << __FUNCTION__ << "() called. strHost is empty! " << endl);
        return false;
    }

    zMemBuf = _pstMsg->zMemBuf;
    pstUserName = ZNULL;
    pstHost = ZNULL;

    /* create Contact header */
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if(!pstContact) return false;

    /* add a parameter */
    Sip_ParmContactLstAdd(zMemBuf, &pstContact->stParmLst, &pstContactParm);

    /* set display name */
    Sip_ParmNaSpecSetDispName(zMemBuf, &pstContactParm->stNaSpec, ZNULL);

    /* get addr-spec */
    Sip_ParmNaSpecGetAddrSpec(&pstContactParm->stNaSpec, &pstAddrSpec);
    if(!pstAddrSpec) return false;

    /* set addr-spec */
    SipMsgCommon::str2ZosSstr(strUserName, stUserName);
    SipMsgCommon::str2ZosSstr(strHost, stHost);
    if(stUserName.iLen > 0) pstUserName = &stUserName;
    if(stHost.iLen > 0) pstHost = &stHost;
    if(Sip_ParmFillAddrSpecByName(zMemBuf, pstAddrSpec, pstUserName, pstHost, iPort) == ZFAILED)
        return false;

    ////* fill the transport */
    if (Sip_ParmFillSipUriTpt(zMemBuf, &pstAddrSpec->u.stSipUri,ucTptType) == ZFAILED)
        return false;

    return true;
}

/* fill header contact by addr-spec */
bool SipMsgEncode::fillHdrContactByAddrSpec(ST_SIP_ADDR_SPEC *pstAddrSpec, const std::string &strHost,
                                            int iPort)
{
    /* check pointer */
    if(!pstAddrSpec) return false;

    /* set ST_ZOS_INET_ADDR */
    ST_ZOS_INET_ADDR stInetAddr;
    SipMsgCommon::enIpv4Addr(strHost, iPort, stInetAddr);

    /* fill contact header */
    if(Sip_MsgFillHdrContact(_pstMsg, pstAddrSpec, &stInetAddr) == ZOK)
    {
        return true;
    }
    return false;
}

bool SipMsgEncode::fillHdrContactByTel(std::string strDispName, std::string strNumber)
{
    ST_SIP_HDR_CONTACT *pstContact;
    ST_SIP_CONTACT_PARM *pstContactParm;
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ST_ZOS_SSTR stDispName;
    ST_SIP_TEL_URI stTelUri;
    ST_ZOS_SSTR *pstDispName = 0;

    SipMsgCommon::str2ZosSstr(strDispName, stDispName);

    /* create Contact header */
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if(!pstContact) return false;

    /* add a parameter */
    Sip_ParmContactLstAdd(_pstMsg->zMemBuf, &pstContact->stParmLst, &pstContactParm);

    /* set display name */
    pstDispName = strDispName.size() > 0 ? &stDispName : 0;
    Sip_ParmNaSpecSetDispName(_pstMsg->zMemBuf, &pstContactParm->stNaSpec, pstDispName);

    /* get addr-spec */
    Sip_ParmNaSpecGetAddrSpec(&pstContactParm->stNaSpec, &pstAddrSpec);
    if(!pstAddrSpec)
        return false;

    /* set number */
    stTelUri.ucGlobalNumberPres = ZTRUE;
    ABNF_LIST_CREATE(&(stTelUri.u.stGlobal.stParLst));
    SipMsgCommon::str2ZosSstr(strNumber, stTelUri.u.stGlobal.stNumberDigits);

    pstAddrSpec->ucType = EN_SIP_ADDR_SPEC_TEL;
    pstAddrSpec->u.stTelUri = stTelUri;

    return true;
}

/* add transport parameter to addr-spec */
bool SipMsgEncode::parmFillAddrSpecTpt(void *zMemBuf, ST_SIP_ADDR_SPEC *pstAddrSpec, unsigned char ucTptType)
{
    ST_SIP_URI_PARM *pstUriParm;
    if(ucTptType == -1) return false;

    /* add transport parameter */
    Sip_ParmUriLstAdd(zMemBuf, &pstAddrSpec->u.stSipUri.stUriParmLst, EN_SIP_URI_PARM_TPT, &pstUriParm);
    if(!pstUriParm) return false;

    /* set transport parameter */
    pstUriParm->u.stTpt.ucType = ucTptType;

    return true;
}

bool SipMsgEncode::parmFillSipUriTpt(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, unsigned char ucTptType)
{
    ST_SIP_URI_PARM *pstUriParm;
    if(ucTptType == -1) return false;

    /* add transport parameter */
    Sip_ParmUriLstAdd(zMemBuf, &pstSipUri->stUriParmLst,
        EN_SIP_URI_PARM_TPT, &pstUriParm);
    if(!pstUriParm) return false;

    /* set transport parameter */
    pstUriParm->u.stTpt.ucType = ucTptType;

    return true;
}

bool SipMsgEncode::parmFillSipUriLr(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri)
{
    ST_SIP_URI_PARM *pstUriParm;

    /* add transport parameter */
    Sip_ParmUriLstAdd(zMemBuf, &pstSipUri->stUriParmLst, EN_SIP_URI_PARM_LR, &pstUriParm);
    if(!pstUriParm) return false;

    return true;
}

bool SipMsgEncode::parmFillSipUriOthers(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, const ST_ZOS_SSTR & name, const ST_ZOS_SSTR & zosValue)
{
    ST_SIP_URI_PARM *pstUriParm;

    /* check input parameter */
    if (!pstSipUri)
        return false;

    /* add the parameter */
    if (Sip_ParmUriLstAdd(zMemBuf, &pstSipUri->stUriParmLst, EN_SIP_URI_PARM_OTHER, &pstUriParm) != ZOK)
        return false;

    /* set the parameter */
    pstUriParm->u.stOther.ucValPres = ZTRUE;
    pstUriParm->u.stOther.stName = name;
    pstUriParm->u.stOther.stVal = zosValue;

    return true;
}

bool SipMsgEncode::parmFillSipUriOthers(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, const std::string & strName, const std::string & strValue)
{
    ST_SIP_URI_PARM *pstUriParm;

    /* check input parameter */
    if (!pstSipUri)
        return false;

    /* add the parameter */
    if (Sip_ParmUriLstAdd(zMemBuf, &pstSipUri->stUriParmLst, EN_SIP_URI_PARM_OTHER, &pstUriParm) != ZOK)
        return false;

    SipMsgCommon::str2ZosSstr(strName, pstUriParm->u.stOther.stName);
    if (strValue.empty() == false)
    {
        pstUriParm->u.stOther.ucValPres = ZTRUE;
        SipMsgCommon::str2ZosSstr(strValue, pstUriParm->u.stOther.stVal);
    }
    else
    {
        pstUriParm->u.stOther.ucValPres = ZFALSE;
    }
    return true;
}


bool SipMsgEncode::parmFillSipUriUSer(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, unsigned char ucUser)
{
    if (!Sip_ParmFillSipUriUser(zMemBuf, pstSipUri,ucUser) !=ZOK)
        return false;
    return true;
}

bool SipMsgEncode::ParmFillSipUri(ST_SIP_SIP_URI *pstSipUri, const std::string & userName, const std::string & strHostname, int iPort)
{
    ST_ZOS_SSTR  stUserInfo;
    ST_SIP_HOST_PORT stHostPort;
    ST_ZOS_SSTR *pstUserInfo = 0;

    SipMsgCommon::enHostPort(strHostname, iPort, stHostPort); /* set stHostPort */
    SipMsgCommon::str2ZosSstr(userName, stUserInfo);
    pstUserInfo = userName.size() > 0 ? &stUserInfo : 0;
    if (Sip_ParmFillSipUri(_pstMsg->zMemBuf, pstSipUri, pstUserInfo, &stHostPort) != ZOK)
        return false;

    return true;
}

bool SipMsgEncode::ParmFillTelUri(ST_SIP_TEL_URI *pstTelUri, const std::string & userName)
{
    ST_ZOS_SSTR  stUserInfo;
    ST_ZOS_SSTR *pstUserInfo = 0;

    SipMsgCommon::str2ZosSstr(userName, stUserInfo);
    pstUserInfo = userName.size() > 0 ? &stUserInfo : 0;
    if (Sip_ParmFillTelUri(_pstMsg->zMemBuf, pstTelUri, ZFALSE, pstUserInfo) != ZOK)
        return false;

    return true;
}

bool SipMsgEncode::parmFillSipUriOthersEx(void *zMemBuf, ST_SIP_SIP_URI *pstSipUri, const std::map<std::string, std::string> & params)
{
    /* check input parameter */
    if (!pstSipUri)
        return false;

    if (params.size() == 0)
        return false;

    std::map<std::string, std::string>::const_iterator itePara = params.begin();
    for (; itePara != params.end(); itePara++)
    {
        std::string strName = itePara->first;
        std::string strValue = itePara->second;
        ST_SIP_URI_PARM *pstUriParm  =0;
        /* add the parameter */
        if (Sip_ParmUriLstAdd(zMemBuf, &pstSipUri->stUriParmLst, EN_SIP_URI_PARM_OTHER, &pstUriParm) != ZOK)
            return false;

        SipMsgCommon::str2ZosSstr(strName, pstUriParm->u.stOther.stName);
        if (strValue.empty() == false)
        {
            pstUriParm->u.stOther.ucValPres = ZTRUE;
            SipMsgCommon::str2ZosSstr(strValue, pstUriParm->u.stOther.stVal);
        }
        else
        {
            pstUriParm->u.stOther.ucValPres = ZFALSE;
        }
    }
    return true;
}



bool SipMsgEncode::parmFillContact(int iCapOpt)
{
    ZBOOL bHasIari = ZFALSE, bHasIcsi = ZFALSE, bVideo = ZFALSE;
    ST_SIP_HDR_CONTACT *pstContact;
    ST_SIP_CONTACT_PARM *pstParm;
    std::string strIariVal, strIcsiVal;
    ZDBUF zMemBuf;

    zMemBuf = _pstMsg->zMemBuf;

    /* find Contact header */
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if (!pstContact) return false;

    /* get the first parameter */
    pstParm = (ST_SIP_CONTACT_PARM *)ABNF_LIST_HEAD_DATA(&pstContact->stParmLst);
    if (!pstParm) return false;

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.im" */
    if ((iCapOpt & CT_CAP_IM))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCSE_IM;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.ft" */
    if ((iCapOpt & CT_CAP_FT))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCSE_FT;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.msg;urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.largemsg" */
    if ((iCapOpt & CT_CAP_STDMSG))
    {
        if (bHasIcsi) strIcsiVal += ",";
        strIcsiVal += SIP_SER_REF_STD_MSG;
        if (!bHasIcsi) bHasIcsi = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn:urn-7:3gpp-application.ims.iari.gsma-vs" */
    if ((iCapOpt & CT_CAP_VSOUTVC))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_GSMA_VS;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.cs-voice" */
    if ((iCapOpt & CT_CAP_VS))
        Sip_ParmFillContact3gppCv(zMemBuf, pstParm);

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-is" */
    if ((iCapOpt & CT_CAP_IS))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_GSMA_IS;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.dp" */
    if ((iCapOpt & CT_CAP_DP))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCSE_DP;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.sp" */
    if ((iCapOpt & CT_CAP_SP))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCSE_SP;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel" */
    if ((iCapOpt & CT_CAP_VOICECALL) || (iCapOpt & CT_CAP_VIDEOCALL))
    {
        if (bHasIcsi) strIcsiVal += ",";
        strIcsiVal += SIP_SER_REF_MMTEL_VOC;
        if (!bHasIcsi) bHasIcsi = ZTRUE;

        /* set the flag of adding video */
        if ((iCapOpt & CT_CAP_VIDEOCALL))
            bVideo = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.geopull" */
    if ((iCapOpt & CT_CAP_GEOPULL))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCS_GPL;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.geopush" */
    if ((iCapOpt & CT_CAP_GEOPUSH))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCS_GPS;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gppapplication.ims.iari.rcs.fullsfgroupchat" */
    if ((iCapOpt & CT_CAP_FSTFWDGP))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCS_FSFGP;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.ftthumb" */
    if ((iCapOpt & CT_CAP_FTTHUMB))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCS_FTTB;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.ftstandfw" */
    if ((iCapOpt & CT_CAP_FTSTFWD))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCS_FTSF;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.fthttp" */
    if ((iCapOpt & CT_CAP_FTHTTP))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCS_FTHP;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.geopullft" */
    if ((iCapOpt & CT_CAP_GEOPULLFT))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_RCS_GPLFT;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-application.ims.icsi.rcs.mnc000.mcc460.publicmsg" */
    if ((iCapOpt & CT_CAP_PUB_MSG))
    {
        if (bHasIcsi) strIariVal += ",";
        strIcsiVal += SIP_APP_REF_PUB_MSG;
        if (!bHasIcsi) bHasIcsi = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.mnc000.mcc460.cloudfile;version=1_0" */
    if ((iCapOpt & CT_CAP_CLOUD_FILE))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_CLOUD_FILE;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.mnc000.mcc460.vemoticon;version=1_0" */
    if ((iCapOpt & CT_CAP_VEM))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_VEM;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.iari-ref="urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.mnc000.mcc460.gpmanage;vs=1" */
    if ((iCapOpt & CT_CAP_GRP_MAN))
    {
        if (bHasIari) strIariVal += ",";
        strIariVal += SIP_APP_REF_GRP_MAN;
        if (!bHasIari) bHasIari = ZTRUE;
    }

    /* add ac parm of +g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.msg" */
    if ((iCapOpt & CT_CAP_CPM_MSG))
    {
        if (bHasIcsi) strIcsiVal += ",";
        strIcsiVal += SIP_SER_REF_CPM_MSG;
        if (!bHasIcsi) bHasIcsi = ZTRUE;
    }

    /* add ac parm of +g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.largemsg" */
    if ((iCapOpt & CT_CAP_CPM_LARGE))
    {
        if (bHasIcsi) strIcsiVal += ",";
        strIcsiVal += SIP_SER_REF_CPM_LARGE;
        if (!bHasIcsi) bHasIcsi = ZTRUE;
    }

    /* add ac parm of +g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.filetransfer" */
    if ((iCapOpt & CT_CAP_CPM_FT))
    {
        if (bHasIcsi) strIcsiVal += ",";
        strIcsiVal += SIP_SER_REF_CPM_FT;
        if (!bHasIcsi) bHasIcsi = ZTRUE;
    }

    /* add ac parm of +g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.session" */
    if ((iCapOpt & CT_CAP_CPM_SESS))
    {
        if (bHasIcsi) strIcsiVal += ",";
        strIcsiVal += SIP_SER_REF_SESS;
        if (!bHasIcsi) bHasIcsi = ZTRUE;
    }

    if (bHasIari)
    {
        /* add "+g.3gpp.iari-ref" parameters */
        Sip_ParmFillContactExtnParms(zMemBuf, pstParm,
            (char*)"+g.3gpp.iari-ref", ZTRUE, (ZCHAR *)strIariVal.c_str());
    }

    if (bHasIcsi)
    {
        /* add "+g.3gpp.icsi-ref" parameters */
        Sip_ParmFillContactExtnParms(zMemBuf, pstParm,
            (char*) "+g.3gpp.icsi-ref", ZTRUE, (ZCHAR *)strIcsiVal.c_str());
    }

    /* add "video" parameters */
    if (bVideo)
        Sip_ParmFillContactExtnParms(zMemBuf, pstParm,
        (char*)"video", ZTRUE, ZNULL);

    /* add ac parm of ";barCycle" */
    if ((iCapOpt & CT_CAP_BURN))
    {
        Sip_ParmFillContactExtnParms(zMemBuf, pstParm,
            (char*)"barCycle", ZFALSE, ZNULL);
    }

    return true;
}

/* add header contact param expires */
bool SipMsgEncode::parmFillContactExpires(int iExpires)
{
    ST_SIP_MSG *pstMsg;
    ST_SIP_HDR_CONTACT *pstContact;
    ST_SIP_CONTACT_PARM *pstContactParm;
    ZDBUF zMemBuf;

    pstMsg = _pstMsg;
    zMemBuf = pstMsg->zMemBuf;

    /* find contact header */
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(pstMsg, EN_SIP_HDR_CONTACT);
    if(!pstContact) return false;

    /* get contact param */
    pstContactParm = (ST_SIP_CONTACT_PARM *)ABNF_LIST_HEAD_DATA(&pstContact->stParmLst);
    if(!pstContactParm) return false;

    /* add contact expires in register message */
    if(Sip_ParmFillContactExpires(zMemBuf, pstContactParm, iExpires) == ZFAILED)
        return false;

    return true;
}

/* fill User-Agent header */
bool SipMsgEncode::fillHdrUserAgent(const std::string &strName, const std::string strVersion, const std::string &strComm)
{
    ST_ZOS_SSTR stName;
    ST_ZOS_SSTR stVer;
    ST_ZOS_SSTR stComm;

    ST_ZOS_SSTR *pstName = ZNULL;
    ST_ZOS_SSTR *pstVer = ZNULL;
    ST_ZOS_SSTR *pstComm = ZNULL;

    SipMsgCommon::str2ZosSstr(strName, stName);
    SipMsgCommon::str2ZosSstr(strVersion, stVer);
    SipMsgCommon::str2ZosSstr(strComm, stComm);

    if(stName.iLen > 0) pstName = &stName;
    if(stVer.iLen > 0) pstVer = &stVer;
    if(stComm.iLen > 0) pstComm = &stComm;


    ST_SIP_HDR_USER_AGENT * pstUserAgent= NULL;
    pstUserAgent = (ST_SIP_HDR_USER_AGENT *)Sip_FindMsgHdr(_pstMsg,EN_SIP_HDR_USER_AGENT);
    if (NULL == pstUserAgent)
    {
        if(Sip_MsgFillHdrUserAgent(_pstMsg, pstName, pstVer, pstComm) == ZFAILED)
            return false;
    }
    else
    {
        if (Sip_HdrFillUserAgent(_pstMsg->zMemBuf, pstUserAgent, pstName, pstVer,pstComm) != ZOK)
            return false;
    }

    return true;
}

/* fill header server */
bool SipMsgEncode::fillHdrServer(const std::string &strName, const std::string &strVer, const std::string &strComm)
{
    ST_ZOS_SSTR stName;
    ST_ZOS_SSTR stVer;
    ST_ZOS_SSTR stComm;

    ST_ZOS_SSTR *pstName = ZNULL;
    ST_ZOS_SSTR *pstVer = ZNULL;
    ST_ZOS_SSTR *pstComm = ZNULL;

    SipMsgCommon::str2ZosSstr(strName, stName);
    SipMsgCommon::str2ZosSstr(strVer, stVer);
    SipMsgCommon::str2ZosSstr(strComm, stComm);

    if(stName.iLen > 0) pstName = &stName;
    if(stVer.iLen > 0) pstVer = &stVer;
    if(stComm.iLen > 0) pstComm = &stComm;

    if(Sip_MsgFillHdrServer(_pstMsg, pstName, pstVer, pstComm) == ZFAILED)
        return false;

    return true;
}

/* fill Event header */
bool SipMsgEncode::fillHdrEvnt(unsigned char ucPkgType, unsigned char ucTempType)
{
    if(Sip_MsgFillHdrEvnt(_pstMsg, ucPkgType, ucTempType) == ZOK)
    {
        return true;
    }
    return false;
}

bool SipMsgEncode::fillHdrEvntId(std::string evntId)
{
    ST_SIP_HDR_EVNT *pstHdrEvnt = 0;
    ST_ZOS_SSTR stEvntId;
    SipMsgCommon::str2ZosSstr(evntId, stEvntId);
    if (evntId.empty())
        return false;

    pstHdrEvnt = (ST_SIP_HDR_EVNT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_EVNT);
    if (!pstHdrEvnt)
    {
        pstHdrEvnt = (ST_SIP_HDR_EVNT *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_EVNT);
    }
    if (pstHdrEvnt == 0)
    {
        return false;
    }

    /* fill the event parameter */
    if (Sip_HdrEvntAddId(_pstMsg->zMemBuf, pstHdrEvnt, &stEvntId) != ZOK)
    {
        return false;
    }

    return true;
}


/* fill Expires header */
bool SipMsgEncode::fillHdrExpires(int iExpires)
{
    if(Sip_MsgFillHdrExpire(_pstMsg, iExpires) != ZOK)
        return false;
    return true;
}

/* fill header Session-Expires */
bool SipMsgEncode::fillHdrSessExpires(bool bIsRefresher, bool bIsUasRefresher, int iExpires)
{
    if(bIsRefresher)
        bIsRefresher = ZTRUE;
    else
        bIsRefresher = ZFALSE;

    if(bIsUasRefresher)
        bIsUasRefresher = ZTRUE;
    else
        bIsUasRefresher = ZFALSE;

    if(Sip_MsgFillHdrSessExpire(_pstMsg, bIsRefresher, bIsUasRefresher, iExpires) != ZOK)
        return false;
    return true;
}

/* fill header privacy */
bool SipMsgEncode::fillHdrPrivacy(unsigned char ucPrivVal)
{
    if(Sip_MsgFillHdrPrivacy(_pstMsg, ucPrivVal) != ZOK)
        return false;
    return true;
}

/* fill header Content-Disposition */
bool SipMsgEncode::fillHdrContentDisp(const std::string &strContentDisp)
{
    ST_SIP_HDR_CONTENT_DISP *pstContentDisp;

    /* check the string */
    if(strContentDisp.empty()) return false;

    /* create Content-Disposition header */
    pstContentDisp = (ST_SIP_HDR_CONTENT_DISP *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_CONTENT_DISP);
    if (!pstContentDisp) return false;

    /* fill content disposition */
    if(Sip_HdrFillContentDisp(_pstMsg->zMemBuf, pstContentDisp, (ZCHAR *)strContentDisp.c_str()) != ZOK)
        return false;

    return true;
}

/* fill header Allow */
bool SipMsgEncode::fillHdrAllow(vector<int> viSuptFlag)
{
    vector<int>::iterator it;
    ZULONG dwSuptFlag = 0;
    ZOS_ZERO(dwSuptFlag);

    for(it = viSuptFlag.begin(); it != viSuptFlag.end(); it++)
    {
        ZOS_SET(*it, dwSuptFlag);
    }

    if(Sip_MsgFillHdrAllow(_pstMsg, dwSuptFlag) == ZOK)
    {
        return true;
    }
    return false;
}

bool SipMsgEncode::fillHdrAcpt(unsigned char ucMType, unsigned char ucSType)
{
    if(Sip_MsgFillHdrAcpt(_pstMsg, ucMType, ucSType))
    {
        return true;
    }
    return false;
}

bool SipMsgEncode::fillHdrAcptEncoding(const vector<std::string> & encodings)
{
    ST_SIP_HDR_ACPT_ENCODING *pstHdrAcptEcd = 0;
    ST_SIP_ENCODING *pstEncode = 0;
    void * pTem = NULL;
    /* create Acept-Encoding header */
    pstHdrAcptEcd = (ST_SIP_HDR_ACPT_ENCODING *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ACPT_ENCODING);
    if (!pstHdrAcptEcd) return false;

    ABNF_LIST_CREATE(&pstHdrAcptEcd->stEncodingLst);
    for (unsigned int i = 0; i < encodings.size(); i++)
    {
        std::string strEncode = encodings.at(i);
        pTem = 0;
        if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstHdrAcptEcd->stEncodingLst, sizeof(ST_SIP_ENCODING),&pTem) == ZOK)
        {
            pstEncode = (ST_SIP_ENCODING * )pTem;
            if (0 == pstEncode)
                return false;

            pstEncode->stCodings.ucCodingAllPres = ZFALSE;
            SipMsgCommon::str2ZosSstr(strEncode, pstEncode->stCodings.stContent);
        }
    }

    return true;
}


bool SipMsgEncode::fillHdrAcptLang(const vector<std::string> & languages)
{
    ST_SIP_HDR_ACPT_LANG *pstHdrLang = 0;
    ST_SIP_LANG *pstLang = 0;
    void * pTem = NULL;

    pstHdrLang = (ST_SIP_HDR_ACPT_LANG *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ACPT_LANG);
    if (!pstHdrLang) return false;

    ABNF_LIST_CREATE(&pstHdrLang->stLangLst);
    for (unsigned int i = 0; i < languages.size(); i++)
    {
        std::string strLang = languages.at(i);
        pTem = 0;
        if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstHdrLang->stLangLst, sizeof(ST_SIP_LANG),&pTem) == ZOK)
        {
            pstLang = (ST_SIP_LANG * )pTem;
            if (0 == pstLang)
                return false;

            pstLang->stRange.ucLangAllPres = ZFALSE;
            SipMsgCommon::str2ZosSstr(strLang, pstLang->stRange.stSubtag);
        }
    }
    return true;
}

/* SIP message fill content-type Header */
bool SipMsgEncode::fillHdrContentType(unsigned char ucMtype, const std::string &strMtypeExt,
                                      unsigned char ucMSubtype, const std::string &strMSubtypeExt)
{
    ST_ZOS_SSTR stMtypeExt, stMSubtypeExt;
    SipMsgCommon::str2ZosSstr(strMtypeExt, stMtypeExt);
    SipMsgCommon::str2ZosSstr(strMSubtypeExt, stMSubtypeExt);
    if(Sip_MsgFillHdrContentType(_pstMsg, ucMtype, &stMtypeExt, ucMSubtype, &stMSubtypeExt) == ZOK)
    {
        return true;
    }

    TLOGWARN("[SipMsgEncode] " << __FUNCTION__ << "() called , return FALSE!." << endl);
    return false;
}

bool SipMsgEncode::fillHdrContentTypeX(ZUCHAR ucMtype, const std::string &strMtypeExt, ZUCHAR ucMSubtype, const std::string &strMSubtypeExt, ZCHAR *pcBoundary, ZUSHORT wLen)
{
    ST_ZOS_SSTR stMtypeExt, stMSubtypeExt;
    SipMsgCommon::str2ZosSstr(strMtypeExt, stMtypeExt);
    SipMsgCommon::str2ZosSstr(strMSubtypeExt, stMSubtypeExt);
    if (Sip_MsgFillHdrContentTypeX(_pstMsg, ucMtype, &stMtypeExt,  ucMSubtype, &stMSubtypeExt, pcBoundary,  wLen) != ZOK)
        return false;
    return true;
}


/* SIP message fill SDP body */
bool SipMsgEncode::fillBodySdp(StSdpSessDesc stSessDesc)
{
    ST_SDP_SESS_DESC *pstSdpMsg = 0;

    /* create sdp msg */
    if(!createSdpMsg(&pstSdpMsg))
    {
        TLOGWARN("[SipMsgEncode] " << __FUNCTION__ << "() called , createSdpMsg() return FALSE!." << endl);
        return false;
    }
    /* fill sdp origin */
    fillSdpOrigin(pstSdpMsg, &stSessDesc._stOrigin);

    ///* fill sdp sess name */
    fillSdpSessName(pstSdpMsg, stSessDesc._strSessName);                //如果linux下填入s 行，且名称为 “-”，则会报错。填多个字符，又不会显示任何。。。

    ///* fill sdp time */
    fillSdpSessTime(pstSdpMsg, &stSessDesc._stTimeField);

    ///* fill sdp connection field*/
    fillSdpSessConnection(pstSdpMsg, stSessDesc._stConnInfo);

    /* fill sdp version */
    //fillSdpVersion(pstSdpMsg, stSessDesc._iVer);

    //TEST
    //fill sdp info
    fillSdpInfoFild(pstSdpMsg, stSessDesc._strInfo);
    //pstSdpMsg->stInfo.ucPres = ZFALSE;

    //fillSdpSessTime(pstSdpMsg, &stSessDesc._stTimeField);

    /* fill sdp attribution */
    //fillSdpSessAttr(pstSdpMsg, EN_SDP_ATTR_ICE_LITE);

    /* fill sdp media */
    for (unsigned int i = 0; i < stSessDesc._stMediaVec.size(); i++)
    {
        fillSdpMediaDesc(pstSdpMsg, &stSessDesc._stMediaVec[i]);
    }

    /* add sdp to sip */
    if(Sip_MsgFillBodySdp(_pstMsg, pstSdpMsg) == ZFAILED)
    {
        TLOGWARN("[SipMsgEncode] Sip_MsgFillBodySdp() called , return FALSE!." << endl);
        return false;
    }

    return true;
}

/* create a new sdp */
bool SipMsgEncode::createSdpMsg(ST_SDP_SESS_DESC **ppstSdpMsg)
{
    ST_SDP_SESS_DESC *pstSdpMsg;
    ZDBUF zMemBuf;

    zMemBuf = _pstMsg->zMemBuf;

    /* create sdp msg */
    if(Sdp_MsgCreate(zMemBuf, &pstSdpMsg) == ZFAILED)
    {
        TLOGERROR("[SipGateway] Sdp_MsgCreate() called , return FALSE...1." << endl);
        return false;
    }
    *ppstSdpMsg = pstSdpMsg;
    return true;
}

/* set version of sdp */
bool SipMsgEncode::fillSdpVersion(ST_SDP_SESS_DESC *pstSdpMsg, int iVersion)
{
    pstSdpMsg->stVer.ucPres = ZTRUE;
    pstSdpMsg->stVer.wVer = iVersion;
    return true;
}

/* set origin of sdp */
bool SipMsgEncode::fillSdpOrigin(ST_SDP_SESS_DESC *pstSdpMsg, StSdpOrigin *pstOrigin)
{
    if(!pstOrigin) return false;
    ZUBUF zMemBuf;

    /* set memory buffer */
    zMemBuf = _pstMsg->zMemBuf;

    /* transform string to ST_ZOS_INET_IP */
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP((ZCHAR *)(pstOrigin->_strAddr.c_str()), &stIp);

    /* sdp message set origin-field */
    if(Sdp_MsgSetOfX(zMemBuf, &pstSdpMsg->stOrig, (ZCHAR *)(pstOrigin->_strUsername.c_str()), ZULONG(pstOrigin->_lSessId), ZULONG(pstOrigin->_lSessVer),  &stIp) == ZFAILED)
    {
        //SipMsgCommon::speak("Sdp_MsgSetOfX failed");
        TLOGWARN("[SipMsgEncode] Sdp_MsgSetOfX() failed." << endl);
        return false;
    }

    return true;
}

/* set session name of sdp */
bool SipMsgEncode::fillSdpSessName(ST_SDP_SESS_DESC *pstSdpMsg, const std::string &strSessName)
{
    ZUBUF zMemBuf;
    if (strSessName.empty())
        return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    if (Sdp_MsgSetSf(zMemBuf, &pstSdpMsg->stSessName, (char*)strSessName.c_str(), strSessName.size()))

        //   /* sdp message set session-name--field */
        //if(Sdp_MsgSetSf(zMemBuf, &pstSdpMsg->stSessName, "-", 1) == ZFAILED)

        //if(Sdp_MsgSetSf(zMemBuf, &pstSdpMsg->stSessName,ZOS_NSTR("-")) == ZFAILED)
    {
        TLOGWARN("[SipMsgEncode] Sdp_MsgSetSf() failed." << endl);
        return false;
    }

    return true;
}

bool SipMsgEncode::fillSdpSessConnection(ST_SDP_SESS_DESC *pstSdpMsg, const StSdpConnInfo &stConnectionIngo)
{
    ST_ZOS_INET_IP stIp;
    ZOS_INET_STR_TO_IP(stConnectionIngo._strAddr.c_str(),&stIp);
    if (Sdp_MsgSetCf(&pstSdpMsg->stConn, &stIp) !=  ZOK)
    {
        TLOGWARN("[SipMsgEncode] Sdp_MsgSetCf() failed." << endl);
        return false;
    }
    return true;
}


/* add session time description */
bool SipMsgEncode::fillSdpSessTime(ST_SDP_SESS_DESC *pstSdpMsg, StSdpTimeField *pstTimeField)
{
    ZUBUF zMemBuf;

    /* allocate memory */
    zMemBuf = _pstMsg->zMemBuf;

    if(Sdp_MsgSetTf(zMemBuf, pstSdpMsg, (ZULONG)pstTimeField->_lStartTime,
        (ZULONG)pstTimeField->_lStopTime) == ZFAILED)
    {
        TLOGWARN("[SipMsgEncode] Sdp_MsgSetTf() failed." << endl);
        return false;
    }

    return true;
}

/* add session attribution */
bool SipMsgEncode::fillSdpSessAttr(ST_SDP_SESS_DESC *pstSdpMsg, unsigned char ucType)
{
    ST_SDP_AF *pstAf;
    void *zMemBuf;

    /* check pointer */
    if(!pstSdpMsg) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* create attribution for sdp */
    if(Sdp_MsgCreateAfX(zMemBuf, pstSdpMsg, &pstAf) == ZFAILED)
        return false;

    pstAf->ucType = ucType;

    return true;
}

/* add media description */
bool SipMsgEncode::fillSdpMediaDesc(ST_SDP_SESS_DESC *pstSdpMsg, StSdpMediaDesc *pstMediaDesc)
{
    /* check the pointer */
    if(!pstMediaDesc)  return false;
    if(!pstSdpMsg)  return false;

    void * zMemBuf;
    ST_SDP_MDESC *pstMdesc;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* create media description */
    if(Sdp_MsgCreateMdesc(zMemBuf, pstSdpMsg, &pstMdesc) == ZFAILED)
    {
        //SipMsgCommon::speak("Sdp_MsgCreateMdesc failed");
        return false;
    }

    /* set media */
    if(Sdp_MsgSetMfX(zMemBuf, &pstMdesc->stMedia, pstMediaDesc->_ucMediaType,
        ZNULL, pstMediaDesc->_ucProtoType, ZNULL, pstMediaDesc->_iPort, 0) == ZFAILED)
    {
        //SipMsgCommon::speak("Sdp_MsgSetMfX failed");
        return false;
    }

    /* set media fmt */
    vector<unsigned char>::iterator it0;
    for(it0 = pstMediaDesc->_strFmtVec.begin(); it0 != pstMediaDesc->_strFmtVec.end(); it0++)
    {
        parmFillSdpMediaFmt(&pstMdesc->stMedia, *it0);
    }

    /* set rtpmap */
    parmFillSdpMediaRtpmap(pstMdesc, 105, 8000, EN_SDP_ENCODING_AMR);

    unsigned char ucModeSet = EN_SDP_ATTR_FMTP_MODE_SET_0 | EN_SDP_ATTR_FMTP_MODE_SET_1 | EN_SDP_ATTR_FMTP_MODE_SET_2 | EN_SDP_ATTR_FMTP_MODE_SET_3
        | EN_SDP_ATTR_FMTP_MODE_SET_4 | EN_SDP_ATTR_FMTP_MODE_SET_5;
    //set fmtp red
    parmFillSdpMediaRtpmap(pstMdesc,114,8000,EN_SDP_ENCODING_RED);

    //set fmtp iLBC
    SdpMsgSetAfFmtpILBC(pstMdesc,105,EN_SDP_ILBC_30);

    //EN_SDP_ENCODING_AMR_WB
    SdpMsgSetAfFmtpAmrWb( pstMdesc,105,  ucModeSet,EN_SDP_ENCODING_AMR_WB  );

    //EN_SDP_ENCODING_AMR_RED
    char chArrry[20] ={""};
    SipMsgCommon::UnCharVectorToArray(pstMediaDesc->_strFmtVec,chArrry);
    SdpMsgSetAfFmtpRed(pstMdesc, 105,chArrry);

    parmFillSdpMediaRtpmap(pstMdesc, 116, 8000, EN_SDP_ENCODING_RTX);
    parmFillSdpMediaRtpmap(pstMdesc, 180, 5060, EN_SDP_ENCODING_CELB);
    /* set direct */
    parmFillSdpMediaDirect(pstMdesc,EN_SDP_DIRECT_TYPE_SEND);

    //set ptime
    SdpMsgSetAfPTime(pstMdesc,20);

    return true;
}

bool SipMsgEncode::fillSdpInfoFild(ST_SDP_SESS_DESC *pstSdpMsg, const std::string &strInfo)
{
    ZUBUF zMemBuf;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* sdp message set information--field */
    if(Sdp_MsgSetIf(zMemBuf, &pstSdpMsg->stInfo,(ZCHAR *)strInfo.c_str(), (ZUSHORT)strInfo.size()) == ZFAILED)
    {
        TLOGERROR("[SipGateway] Sdp_MsgSetIf() failed." << endl);
        return false;
    }

    return true;
}

bool SipMsgEncode::parmFillSdpMediaFmt(ST_SDP_MF *pstMf, unsigned char ucPayload)
{
    void *zMemBuf;

    /* check pointer */
    if(!pstMf) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* set format of ST_SDP_MF */
    if(Sdp_MsgSetMfCodec(zMemBuf, pstMf, ucPayload) == ZFAILED)
        return false;

    return true;
}

bool SipMsgEncode::parmFillSdpMediaRtpmap(ST_SDP_MDESC *pstMdesc, unsigned char ucPayload, int iClockRate, unsigned char ucEncodingType)
{
    void *zMemBuf;

    /* check pointer */
    if(!pstMdesc) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* set media attribute */
    if(Sdp_MsgSetAfRtpmap(zMemBuf, pstMdesc, ucPayload, iClockRate, ucEncodingType) == ZFAILED)
        return false;

    return true;
}

bool SipMsgEncode::parmFillSdpMediaRtcp(ST_SDP_MDESC *pstMdesc, int iPort)
{
    void *zMemBuf;

    /* check pointer */
    if(!pstMdesc) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* set media attribute */
    if(Sdp_MsgSetAfRtcp(zMemBuf, pstMdesc, iPort) == ZFAILED)
        return false;

    return true;
}

bool SipMsgEncode::parmFillSdpMediaRtcpMux(ST_SDP_MDESC *pstMdesc, bool bRtcpMux)
{
    void *zMemBuf;

    /*  check pointer */
    if(!pstMdesc) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* set media attribute */
    if(Sdp_MsgSetAfRtcpMux(zMemBuf, pstMdesc, bRtcpMux) == ZFAILED)
        return false;

    return true;
}

bool SipMsgEncode::parmFillSdpMediaRtcpFb(ST_SDP_MDESC *pstMdesc, unsigned char ucPayload, unsigned char ucIdType, unsigned char ucParm)
{
    void *zMemBuf;

    /* check pointer */
    if(!pstMdesc) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    if(Sdp_MsgSetAfRtcpFb(zMemBuf, pstMdesc, ucPayload, ucIdType, ucParm) == ZFAILED)
        return false;

    return true;
}

bool SipMsgEncode::parmFillSdpMediaDirect(ST_SDP_MDESC *pstMdesc, unsigned char ucDirect)
{
    void *zMemBuf;

    /* check pointer */
    if(!pstMdesc) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    if(Sdp_MsgSetAfDirect(zMemBuf, pstMdesc, ucDirect) == ZFAILED)
        return false;

    return true;
}

/* set media attribute-field ptime */
bool SipMsgEncode::parmFillSdpMediaPTime(ST_SDP_MDESC *pstMdesc, int iPTime)
{
    void *zMemBuf;

    /* check pointer */
    if(!pstMdesc) return false;

    /* set memory*/
    zMemBuf = _pstMsg->zMemBuf;

    /* set ptime */
    if(Sdp_MsgSetAfPTime(zMemBuf, pstMdesc, iPTime) == ZFAILED)
        return false;

    return true;
}

/* set media attribute-field setup */
bool SipMsgEncode::parmFillSdpMediaSetup(ST_SDP_MDESC *pstMdesc, unsigned char ucType)
{
    void *zMemBuf;

    /* check pointer */
    if(!pstMdesc) return false;

    /* set memory */
    zMemBuf = _pstMsg->zMemBuf;

    /* set setup */
    if(Sdp_MsgSetAfSetup(zMemBuf, pstMdesc, ucType) == ZFAILED)
        return false;

    return true;
}

bool SipMsgEncode::encode()
{
    char pcBuf[8192];
    unsigned int uiBufLen = 8192;
    if(SipMsgCommon::SipMsg_Encode(_pstMsg,pcBuf,uiBufLen) == ZOK)
    {
        _strMsg = std::string(pcBuf, uiBufLen);
        return true;
    }
    return false;
}

bool SipMsgEncode::authDRspFillNonce(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strNonce)
{
    ST_SIP_DIG_RSP *pstParm;
    if (strNonce.empty())
        return false;

    /* remove old parameter */
    if(Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_NONCE, &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }

    if ( Sip_ParmFillDRspNonceX(_pstMsg->zMemBuf, pstRsp, (ZCHAR *)strNonce.c_str()) != ZOK )
    {
        return false;
    }
    return true;
}

bool SipMsgEncode::authDRspFillUsername(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strUsername)
{
    void *zMemBuf;
    ST_SIP_DIG_RSP *pstParm;
    if (strUsername.empty())
        return false;

    /* remove old parameter */
    if(Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_USERNAME, &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }
    zMemBuf = _pstMsg->zMemBuf;

    if (Sip_ParmFillDRspUserNameX(zMemBuf, pstRsp, (ZCHAR *)strUsername.c_str() )!= ZOK)
    {
        return false;
    }
    return true;
}

bool SipMsgEncode::authDRspFillUri(ST_SIP_DIGEST_RSP *pstRsp, ST_SIP_REQ_URI *pstReqUri)
{
    void *zMemBuf;
    ST_SIP_DIG_RSP *pstParm;

    /* remove old parameter */
    if(Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_DIGEST_URI,
        &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }
    zMemBuf = _pstMsg->zMemBuf;

    Sip_ParmFillDRspUri(zMemBuf, pstRsp, pstReqUri);
    return true;
}

bool SipMsgEncode::authDRspFillRsp(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strResponse)
{
    ST_SIP_DIG_RSP *pstParm;

    /* remove old parameter */
    if(Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_DRESP,
        &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }

    if (Sip_ParmFillDRspRsp(_pstMsg->zMemBuf, pstRsp, (ZCHAR *)strResponse.c_str()) != ZOK)
    {
        return false;
    }

    return true;
}

bool SipMsgEncode::authDRspFillCnonce(ST_SIP_DIGEST_RSP *pstRsp, ST_ZOS_SSTR *pstCnonce)
{
    ST_SIP_DIG_RSP *pstParm;
    /* remove old parameter */
    if (Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_CNONCE, &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }

    return Sip_ParmFillDRspCnonce(_pstMsg->zMemBuf, pstRsp, pstCnonce) == ZOK ? true : false;

}
bool SipMsgEncode::authDRspFillNonceCount(ST_SIP_DIGEST_RSP *pstRsp, ST_ZOS_SSTR *pstCount)
{
    ST_SIP_DIG_RSP *pstParm;
    /* remove old parameter */
    if (Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_NONCE_COUNT, &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }
    return Sip_ParmFillDRspNonceCount(_pstMsg->zMemBuf, pstRsp, pstCount) == ZOK ? true : false;
}

bool SipMsgEncode::authDRspFillRealm(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strRealm)
{
    std::string strTem = strRealm;
    if (Sip_ParmFillDRspRealmX(_pstMsg->zMemBuf, pstRsp, (char*)strTem.c_str()) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::authDRspFillAlgo(ST_SIP_DIGEST_RSP *pstRsp, unsigned char algoType)
{
    if (Sip_ParmFillDRspAlgoX(_pstMsg->zMemBuf, pstRsp,algoType) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::authDRspFillQop(ST_SIP_DIGEST_RSP *pstRsp, unsigned char qopType)
{
    if (Sip_ParmFillDRspQopX(_pstMsg->zMemBuf, pstRsp,qopType) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::authDRspFillQpaque(ST_SIP_DIGEST_RSP *pstRsp, const std::string & opaque)
{
    ST_ZOS_SSTR stOpaque;
    SipMsgCommon::str2ZosSstr(opaque, stOpaque);
    if (Sip_ParmFillDRspOpaque(_pstMsg->zMemBuf, pstRsp, &stOpaque) != ZOK)
        return false;

    return true;
}

bool SipMsgEncode::fillHdrFromToByUri(ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,	ST_ZOS_SSTR *pstUri, ST_ZOS_SSTR *pstTag)
{
    if (Sip_MsgFillHdrFromToByUri(_pstMsg, bFromHdr,pstDispName, pstUri, pstTag) == ZOK)
        return true;
    return false;
}

bool SipMsgEncode::FillHdrContact(ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_SIP_URI *pstSipUri)
{
    ST_ZOS_INET_ADDR  inetAddr;
    ST_SIP_URI_PARM *pstUriParm;
    void *zMemBuf;

    if (Sip_ParmFillAddrSpecBySipUri(_pstMsg->zMemBuf, pstAddrSpec,  pstSipUri) != ZOK)
        return false;

    zMemBuf = _pstMsg->zMemBuf;

    /* set stAddr */
    ZOS_IPV4_ADDR_SET_STRX(&inetAddr,"192.164.102.55", 8080);

    //SipMsgCommon::enIpv4Addr("www.sina.com", 0,pstAddrSpec);

    if (Sip_MsgFillHdrContact(_pstMsg,  pstAddrSpec, &inetAddr) != ZOK)
        return false;

    ST_SIP_HDR_CONTACT *pstContact = 0;
    ST_SIP_CONTACT_PARM *pstParm = 0;

    /* create contact header */
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if (!pstContact) return false;

    /* add a parameter */
    pstParm =(ST_SIP_CONTACT_PARM*) ABNF_LIST_HEAD_DATA(&pstContact->stParmLst);
    if (!pstParm)
        return false;

    /* set display name */
    //Sip_ParmNaSpecSetDispName(zMemBuf, &pstParm->stNaSpec, ZNULL);

    /* get addr-spec */
    Sip_ParmNaSpecGetAddrSpec(&pstParm->stNaSpec, &pstAddrSpec);
    if(!pstAddrSpec) return false;

    /* set addr-spec */
    //Sip_ParmFillAddrSpecByNameX(zMemBuf, pstAddrSpec,
    //	"127.0.0.1", "127.0.0.1", 5000);

    /* add transport parameter */
    Sip_ParmUriLstAdd(zMemBuf, &pstAddrSpec->u.stSipUri.stUriParmLst,
        EN_SIP_URI_PARM_TPT, &pstUriParm);

    /* set transport parameter */
    pstUriParm->u.stTpt.ucType = 0;

    /* add contact expires in register message */
    Sip_ParmFillContactExpires(_pstMsg->zMemBuf, pstParm, 8000);

    //add +sip.instance
    ST_ZOS_SSTR sipInstance;
    std::string str("urn:uuid:95CAE616-2C2E-466B-BE73-87BD2BFC3C64");
    SipMsgCommon::str2ZosSstr(str, sipInstance);

    //* create contact header */
    //pstContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    //if (!pstContact) return false;

    //* add a parameter */
    pstParm =(ST_SIP_CONTACT_PARM*) ABNF_LIST_HEAD_DATA(&pstContact->stParmLst);

    /*add +sip.instance=*/
    Sip_ParmFillContactSipInstance(_pstMsg->zMemBuf, pstParm,&sipInstance);
    Sip_ParmFillContact3gppCv(_pstMsg->zMemBuf,pstParm);

    /* add a parameter */
    pstParm =(ST_SIP_CONTACT_PARM*) ABNF_LIST_HEAD_DATA(&pstContact->stParmLst);

    return true;

}

/* fill Contact Header by SipUri */
bool SipMsgEncode::fillHdrContactBySipUri(const ST_SIP_SIP_URI & stSipUri)
{
    ST_SIP_HDR_CONTACT * pstHdrContact = NULL;
    ST_SIP_CONTACT_PARM * pstParam = NULL;
    ST_SIP_ADDR_SPEC * pstAddrSpec = NULL;

    pstHdrContact = (ST_SIP_HDR_CONTACT *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if (pstHdrContact == NULL)
    {
        return false;
    }

    //add a param
    Sip_ParmAnyLstAdd(_pstMsg->zMemBuf,&pstHdrContact->stParmLst,sizeof(ST_SIP_CONTACT_PARM), (ZVOID **)&pstParam);
    if (pstParam == NULL)
        return false;

    /* get addr-spec */
    Sip_ParmNaSpecGetAddrSpec(&pstParam->stNaSpec, &pstAddrSpec);
    if(!pstAddrSpec)
        return false;

    pstAddrSpec->u.stSipUri = stSipUri;
    return true;
}

/* fill From or To Header by SipUri */
bool SipMsgEncode::fillHdrFromOrToBySipUri( ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName, ST_SIP_SIP_URI *pstSipUri, ST_ZOS_SSTR *pstTag)
{
    if (Sip_MsgFillHdrFromToBySipUri(_pstMsg, bFromHdr,  pstDispName,pstSipUri, pstTag) == ZOK)
        return true;
    return false;
}

/* fill the expires parameter of contact */
bool  SipMsgEncode::ParmFillContactExpires( ZUINT iExpires)
{

    ST_SIP_HDR_CONTACT *pstContact;
    ST_SIP_CONTACT_PARM *pstParm;

    /* create contact header */
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if (!pstContact) return false;

    /* add a parameter */
    pstParm =(ST_SIP_CONTACT_PARM*) ABNF_LIST_HEAD_DATA(&pstContact->stParmLst);

    /* add contact expires in register message */
    return Sip_ParmFillContactExpires(_pstMsg->zMemBuf, pstParm, iExpires) == ZOK ? true : false;

}

//
///* mgf fill cpim message with imdn request */
//bool SipMsgEncode::fillCpimMsgImdnX(ST_SIP_SIP_URI *pstLclUri,  ST_SIP_SIP_URI *pstPartpUri, ZCHAR *pcBody, ZUINT iBoydSize,
//						  ST_ZOS_SYS_TIME *pstTime, ZUINT iImdnType, ZCHAR *pcCType,
//						  ZCHAR **ppcMsg, ZUINT *piMsgSize, ST_ZOS_SSTR *pstIMsgId)
//{
//	ZBOOL bSuccSupt = false, bFailSupt = false;
//	ZCHAR *pcDeliRptDesc;
//	ZUBUF zCpimId;
//	ZUBUF zData;
//	ZCHAR *pcStr;
//
//	std::string strLocalHostName;
//	std::string strRemotHostName;
//
//	SipMsgCommon::zosSstr2Str( pstLclUri->stHostPort.stHost.u.stName,strLocalHostName);
//	SipMsgCommon::zosSstr2Str( pstPartpUri->stHostPort.stHost.u.stName,strRemotHostName);
//
//	/* create the cpim abnf message */
//	zCpimId = Zcpim_Create();
//	if (!zCpimId) return ZFAILED;
//	/* add From header */
//	if (pstLclUri->ucUserInfoPres > 0)
//		Zcpim_FillHdrFrom(zCpimId, pstLclUri->stUserInfo.stUser.pcStr, (char*)strLocalHostName.c_str());
//	else
//		Zcpim_FillHdrFrom(zCpimId, ZNULL, (char*)strLocalHostName.c_str());
//
//	/* add To header */
//	if (pstPartpUri->ucUserInfoPres > 0)
//		Zcpim_FillHdrTo(zCpimId, pstPartpUri->stUserInfo.stUser.pcStr, (char*)strRemotHostName.c_str());
//	else
//		Zcpim_FillHdrTo(zCpimId, ZNULL, (char*)strRemotHostName.c_str());
//
//	/* add NS header */
//	Zcpim_FillHdrNs(zCpimId, "imdn", "urn:ietf:params:imdn");
//
//	/* add Message-ID header */
//	if (ZOS_ISEMPTY_SSTR(pstIMsgId))
//	{
//		pcStr = Zrandom_RandId(ZNULL, ZNULL, 16);
//		Zcpim_FillHdr(zCpimId, "imdn.Message-ID", pcStr);
//		Zos_UbufCpySStr(_pstMsg->zMemBuf, pcStr, pstIMsgId);
//		Zos_SysStrFree(pcStr);
//	}
//	else
//		Zcpim_FillHdr(zCpimId, "imdn.Message-ID", pstIMsgId->pcStr);
//
//	/* add DateTime header */
//	Zcpim_FillHdrDateTimeX(zCpimId, pstTime);
//
//	/* get the delivery report support */
//	//bSuccSupt = Mgp_GetImdnDeliSuccRptSupt();
//	//bFailSupt = Mgp_GetImdnDeliFailRptSupt();
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
//	//if (iImdnType == EN_MGF_SESS_REQ_DELI)
//		//Zcpim_FillHdr(zCpimId, "imdn.Disposition-Notification", pcDeliRptDesc);
//	//else if (iImdnType == EN_MGF_SESS_REQ_DISP)
//		Zcpim_FillHdr(zCpimId, "imdn.Disposition-Notification", "display");
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
//	Zos_UbufCpyDStr(_pstMsg->zMemBuf, zData, ppcMsg);
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
//	return ZOK;
//}



bool SipMsgEncode::fillHdrPPreferredIdentity(const std::string & strDispName)
{
    ST_SIP_HDR_P_PREFERRED_ID *pstHdrPpid;
    ST_SIP_PPREFERREDID_VAL *pstVal;
    ZUBUF zMemBuf;
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ST_ZOS_SSTR stDispName;
    ST_SIP_SIP_URI  stSipUri;

    SipMsgCommon::str2ZosSstr("",stDispName);

    ST_SIP_ADDR_SPEC  stFromAddrSpec;
    SipMsgCommon::getFromToAddrSpec(_pstMsg, true, stFromAddrSpec);    //get add spec from from header

    /* create P-Preferred-Identity header */
    pstHdrPpid = (ST_SIP_HDR_P_PREFERRED_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_PREFERRED_ID);
    if (!pstHdrPpid) return false;

    /* add a value */
    zMemBuf = _pstMsg->zMemBuf;
    Sip_ParmAnyLstAdd(zMemBuf, (ZVOID *)&pstHdrPpid->stValLst, sizeof(ST_SIP_PPREFERREDID_VAL), (ZVOID **)&pstVal);

    Sip_ParmNaSpecSetDispName(zMemBuf, &pstVal->stNaSpec, &stDispName);
    Sip_ParmNaSpecGetAddrSpec(&pstVal->stNaSpec, &pstAddrSpec);
    //pstAddrSpec->u.stSipUri = stFromAddrSpec.u.stSipUri;	// copy the sipuri to

    ParmFillSipUri(&stSipUri, "+8613911826426", "ebupt.com", 0);
    parmFillSipUriUSer(_pstMsg->zMemBuf, &stSipUri,EN_SIP_USER_PARM_PHONE);

    pstAddrSpec->u.stSipUri = stSipUri;

    return true;
}


bool SipMsgEncode::fillHdrPPreferredIdentity(const std::string & strDispName,const std::string & userName, const std::string  & host, const int & port)
{
    ST_SIP_HDR_P_PREFERRED_ID *pstHdrPpid;
    ST_SIP_PPREFERREDID_VAL *pstVal;
    ZUBUF zMemBuf;
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ST_ZOS_SSTR stDispName;
    ST_SIP_SIP_URI  stSipUri;

    SipMsgCommon::str2ZosSstr(strDispName,stDispName);

    ST_SIP_ADDR_SPEC  stFromAddrSpec;
    SipMsgCommon::getFromToAddrSpec(_pstMsg, true, stFromAddrSpec);    //get add spec from from header

    /* create P-Preferred-Identity header */
    pstHdrPpid = (ST_SIP_HDR_P_PREFERRED_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_PREFERRED_ID);
    if (!pstHdrPpid) return false;

    /* add a value */
    zMemBuf = _pstMsg->zMemBuf;
    Sip_ParmAnyLstAdd(zMemBuf, (ZVOID *)&pstHdrPpid->stValLst, sizeof(ST_SIP_PPREFERREDID_VAL), (ZVOID **)&pstVal);

    Sip_ParmNaSpecSetDispName(zMemBuf, &pstVal->stNaSpec, &stDispName);
    Sip_ParmNaSpecGetAddrSpec(&pstVal->stNaSpec, &pstAddrSpec);

    bool result = ParmFillSipUri(&stSipUri, userName,host, port);
    if (!result)
        return false;
    pstAddrSpec->u.stSipUri = stSipUri;
    return true;
}

bool SipMsgEncode::fillHdrPAssertedIdentity(const std::string & strDispName,const std::string & userName, const std::string  & host, const int & port)
{
    ST_SIP_HDR_P_ASSERTED_ID *pstHdrPAssd;
    ST_SIP_PASSERTEDID_VAL *pstVal;
    ZUBUF zMemBuf;
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ST_ZOS_SSTR stDispName;
    ST_SIP_SIP_URI  stSipUri;

    SipMsgCommon::str2ZosSstr(strDispName,stDispName);//ST_SIP_HDR_P_ASSERTED_ID ST_SIP_PASSERTEDID_VAL

    ST_SIP_ADDR_SPEC  stFromAddrSpec;
    SipMsgCommon::getFromToAddrSpec(_pstMsg, true, stFromAddrSpec);    //get add spec from from header

    /* create P-Asserted-Identity header */
    pstHdrPAssd = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
    if (!pstHdrPAssd) return false;

    /* add a value */
    zMemBuf = _pstMsg->zMemBuf;
    Sip_ParmAnyLstAdd(zMemBuf, (ZVOID *)&pstHdrPAssd->stValLst, sizeof(ST_SIP_PASSERTEDID_VAL), (ZVOID **)&pstVal);

    Sip_ParmNaSpecSetDispName(zMemBuf, &pstVal->stNaSpec, &stDispName);
    Sip_ParmNaSpecGetAddrSpec(&pstVal->stNaSpec, &pstAddrSpec);

    bool result = ParmFillSipUri(&stSipUri, userName,host, port);
    if (!result)
        return false;
    pstAddrSpec->u.stSipUri = stSipUri;
    return true;
}

bool SipMsgEncode::fillHdrPAssertedIdByTel(const std::string & userName)
{
    TLOGINFO("[SipMsgEncode] userName:" << userName << endl);
    ST_SIP_HDR_P_ASSERTED_ID *pstHdrPAssertedId;
    ST_SIP_PASSERTEDID_VAL *pstParm;
    ST_SIP_TEL_URI stTelUri;
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ZUBUF zMemBuf = _pstMsg->zMemBuf;

    pstHdrPAssertedId = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
    if (!pstHdrPAssertedId)
        return false;

    ST_SIP_PASSERTEDID_VAL *pstVal;
    /* add a value */
    Sip_ParmAnyLstAdd(zMemBuf, (ZVOID *)&pstHdrPAssertedId->stValLst, sizeof(ST_SIP_PASSERTEDID_VAL), (ZVOID **)&pstVal);

    Sip_ParmNaSpecGetAddrSpec(&pstVal->stNaSpec, &pstAddrSpec);
    //bool result = ParmFillTelUri(&stTelUri, userName);
    //if (!result)
    //    return false;

    /* set number */
    stTelUri.ucGlobalNumberPres = ZFALSE;
    ABNF_LIST_CREATE(&(stTelUri.u.stLocal.stParLst));
    SipMsgCommon::str2ZosSstr(userName, stTelUri.u.stLocal.stNumberDigits);

    pstAddrSpec->ucType = EN_SIP_ADDR_SPEC_TEL;
    pstAddrSpec->u.stTelUri = stTelUri;

    return true;
}

/*fill header Accept-Contact header.*/
bool SipMsgEncode::fillHdrAcceptContact(vector<int> paramFlags, map<std::string,std::string> mapGenParas)
{
    ST_SIP_HDR_ACPT_CONTACT *pstAcpt;
    ST_SIP_AC_VAL *pstAcVal;

    /* create Accept-Contact header */
    pstAcpt = (ST_SIP_HDR_ACPT_CONTACT*)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ACPT_CONTACT);
    if (!pstAcpt) return false;

    /* add a parameter */
    if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, (ZVOID *)&pstAcpt->stAcValLst, sizeof(ST_SIP_AC_VAL), (ZVOID **)&pstAcVal) != ZOK)
        return false;

    /* add ac parm of "+g.oma.sip-im" */
    //if (Sip_ParmFillAcValOmaSipIm(_pstMsg->zMemBuf, pstAcVal) != ZOK)
    //return false;

    for (unsigned int i = 0; i < paramFlags.size(); i++)
    {
        paramFillAcceptContactParam(_pstMsg->zMemBuf,  pstAcVal,paramFlags[i]);        //set params without gen value, sames to call the Sip_ParmFillAcValOmaSipIm
    }

    map<std::string,std::string> ::iterator iter;
    for (iter = mapGenParas.begin(); iter != mapGenParas.end(); iter++)
    {
        Sip_ParmFillAcValGenParm(_pstMsg->zMemBuf, pstAcVal, (char*)iter->first.c_str(),ZTRUE, (char*)iter->second.c_str());
    }

    return true;
}

/* fill accept contact parameter */
bool SipMsgEncode::paramFillAcceptContactParam(ZUBUF ZUBUF, ST_SIP_AC_VAL *pstAcVal, ZUCHAR ucType)
{
    /* check input parameter */
    if (!pstAcVal) return false;

    ST_SIP_AC_PARMS *pstParm;
    /* add the parameter */
    if (Sip_ParmAcLstAdd(ZUBUF, &pstAcVal->stParmsLst, sizeof(ST_SIP_AC_PARMS), &pstParm) != ZOK)
    {
        return false;
    }
    /* set the param type */
    pstParm->ucType = ucType;
    return true;
}

/* fill P-Early-Media header header.  */
bool  SipMsgEncode::fillHdrPEarlyMedia(  ZUINT iParmType)
{
    if (Sip_MsgFillHdrPEarlyMedia(_pstMsg,iParmType) != ZOK)
        return false;
    return true;
}

/* SIP message fill Subscription-State header.*/
bool SipMsgEncode::fillHdrSubsSta( ZUCHAR ucState)
{
    if (Sip_MsgFillHdrSubsSta(_pstMsg,  ucState) == ZOK)
        return true;
    return false;
}

/* SIP message fill Subscription-State header. */
bool SipMsgEncode::fillHdrSubsStaY( ZUCHAR ucState,  ZUCHAR ucReason, ZUINT iExpires)
{

    if (Sip_MsgFillHdrSubsStaY(_pstMsg, ucState, ucReason, iExpires) == ZOK)
        return true;
    return  false;
}

/* fill the expires parameter of   Subscription-State header*/
bool SipMsgEncode::HdrSubsStaAddExpires( ZUINT iExpires)
{
    ST_SIP_HDR_SUBS_STA * pstSubsSta = NULL;
    ST_SIP_SUBEXP_PARMS * pstParms = NULL;

    pstSubsSta = (ST_SIP_HDR_SUBS_STA *)Sip_FindMsgHdr(_pstMsg,EN_SIP_HDR_SUBS_STA);
    if (pstSubsSta != NULL)
    {
        /* add subexp-params */
        if (Sip_ParmSubexpsLstAdd(_pstMsg->zMemBuf, &pstSubsSta->stParmLst,
            EN_SIP_SUBEXP_PARMS_EXPIRE, &pstParms) != ZOK)
        {
            return false;
        }

        /* set parameter value */
        pstParms->u.iExpires = iExpires;
    }
    return true;
}

bool SipMsgEncode::fillHdrContentDisp(ZCHAR *pcDisp)
{
    if (Sip_MsgFillHdrContentDisp(_pstMsg,pcDisp) == ZOK)
        return true;
    return false;
}

bool SipMsgEncode::HdrAcptAddRange( ZUCHAR ucMType, ZUCHAR ucSType)
{
    ST_SIP_HDR_ACPT *pstAcpt = NULL;
    pstAcpt = (ST_SIP_HDR_ACPT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_ACPT);
    if (pstAcpt == NULL)
    {
        pstAcpt = (ST_SIP_HDR_ACPT *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_ACPT);
    }

    if ( Sip_HdrAcptAddRange(_pstMsg->zMemBuf, pstAcpt, ucMType, ucSType) == ZOK)
        return true;
    return false;
}

//set fmtp field,  EN_SDP_ENCODING_AMR encoding
bool SipMsgEncode::SdpMsgSetAfFmtpAmr( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, ZUCHAR ucModeSet)
{
    if (Sdp_MsgSetAfFmtpAmr( _pstMsg->zMemBuf,pstMdesc,  ucPayload,  ucModeSet) == ZOK)
        return true;
    return false;
}

bool SipMsgEncode::SdpMsgSetAfFmtpAmrWb( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, ZUCHAR ucModeSet, ZUCHAR ucEncoding)
{

    ST_SDP_FMTP *pstFmtp;
    ST_SDP_AF *pstAf;
    ZCHAR acBuf[32] = {0}, acVal[8], *pcStr;
    ST_SDP_FMTP_3267 *pst3267;
    ST_SDP_3267_PARM *pstParm;
    ZUINT i, iMaxCnt;
    ZUSHORT wLen;

    //cerate attribute
    if (Sdp_MsgCreateAf(_pstMsg->zMemBuf, pstMdesc, &pstAf) != ZOK)
    {
        return false;
    }

    pstAf->ucType = EN_SDP_ATTR_FMTP;
    pstFmtp = &pstAf->u.stFmtp;

    iMaxCnt = ucEncoding == EN_SDP_ENCODING_AMR ? 7 : 8;

    /* set fmtp type and payload */
    pstFmtp->ucType = EN_SDP_FMTP_3267;
    pstFmtp->ucPayload = ucPayload;

    /* set amr options */
    pst3267 = &pstFmtp->u.st3267;
    ABNF_LIST_CREATE(&pst3267->stParmLst);

    /* set mode-set when not default value */
    if ( ucModeSet != 0xFF)
    {
        /* allocate data */
        ABNF_LIST_ALLOC_DATA(_pstMsg->zMemBuf, ST_SDP_3267_PARM, pstParm);
        pstParm->ucType = EN_SDP_3267_PARM_MODE_SET;
        pstParm->ucIntValPres = ZFALSE;
        ABNF_LIST_APPEND_NODE(&pst3267->stParmLst, pstParm);

        /* set mode-set string */
        pcStr = acBuf;
        wLen = 32;
        for (i = 0; i <= iMaxCnt; i++)
        {
            if ( ucModeSet & (0x1 << i))
            {
                Zos_SPrintf(acVal, "%d,", i);
                Zos_StrFCpy(&pcStr, &wLen, acVal);
            }
        }

        /* check mode-set string */
        if (wLen == 32) pcStr = (char*)"1,";
        else pcStr = acBuf;
        wLen = (ZUSHORT)(Zos_StrLen(pcStr) - 1);
        pcStr[wLen] = '\0';

        /* add string to mode-set */
        Zos_UbufCpySStr(_pstMsg->zMemBuf, pcStr, &pstParm->u.stVal);
    }

    /* set octet-align parameter when not default value */

    ABNF_LIST_ALLOC_DATA(_pstMsg->zMemBuf, ST_SDP_3267_PARM, pstParm);
    pstParm->ucType = EN_SDP_3267_PARM_OCTET_ALIGN;
    pstParm->ucIntValPres = ZTRUE;
    pstParm->u.iVal = 1;
    ABNF_LIST_APPEND_NODE(&pst3267->stParmLst, pstParm);
    return true;
}

//
bool SipMsgEncode::SdpMsgSetAfFmtpILBC( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, ZUCHAR ucMode)
{
    ST_SDP_FMTP *pstFmtp;
    ST_SDP_AF *pstAf;

    //cerate attribute
    if (Sdp_MsgCreateAf(_pstMsg->zMemBuf, pstMdesc, &pstAf) != ZOK)
    {
        return false;
    }

    pstAf->ucType = EN_SDP_ATTR_FMTP;
    pstFmtp = &pstAf->u.stFmtp;

    /* set fmtp type and payload */
    pstFmtp->ucType = EN_SDP_FMTP_3952;
    pstFmtp->ucPayload = ucPayload;

    /* set iLBC mode */
    pstFmtp->u.st3952.ucMode = ucMode;

    return true;
}

bool SipMsgEncode::SdpMsgSetAfFmtpRed( ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload,  char aucPtLst[13])
{
    ST_SDP_FMTP *pstFmtp;
    ST_SDP_AF *pstAf;

    //cerate attribute
    if (Sdp_MsgCreateAf(_pstMsg->zMemBuf, pstMdesc, &pstAf) != ZOK)
    {
        return false;
    }

    /* set fmtp of media */
    pstAf->ucType = EN_SDP_ATTR_FMTP;
    pstFmtp = &pstAf->u.stFmtp;

    /* set fmtp type and payload */
    pstFmtp->ucType = EN_SDP_FMTP_2733;
    pstFmtp->ucPayload = ucPayload;

    pstFmtp->u.st2733.ucRed = ZTRUE;
    pstFmtp->u.st2733.u.stRed.ucPayloadCnt = strlen(aucPtLst);
    Zos_MemCpy(&pstFmtp->u.st2733.u.stRed.aucPayload, aucPtLst,strlen(aucPtLst));

    return true;
}


/* sdp message exist media-descriptions, type see EN_SDP_MEDIA_TYPE */
bool SipMsgEncode::SdpMsgExistMdesc(ST_SDP_SESS_DESC *pstMsg, ZUCHAR ucMediaType, ST_SDP_MDESC **ppstMdesc)
{
    if (Sdp_MsgExistMdesc(pstMsg, ucMediaType, ppstMdesc) == ZOK)
        return true;
    return false;

}

bool SipMsgEncode::SdpMsgSetAfPTime( ST_SDP_MDESC *pstMdesc, ZULONG iPTime)
{
    ST_SDP_AF *pstAf;

    if (Sdp_MsgCreateAf(_pstMsg->zMemBuf, pstMdesc, &pstAf) != ZOK)
    {
        return false;
    }

    /* set fmtp of media */
    pstAf->ucType = EN_SDP_ATTR_PTIME;
    pstAf->u.iPTime = iPTime;

    return true;
}

bool SipMsgEncode::SdpMsgSetAfMaxPTime( ST_SDP_MDESC *pstMdesc, ZULONG iMaxPTime)
{
    ST_SDP_AF *pstAf;

    if (Sdp_MsgCreateAf(_pstMsg->zMemBuf, pstMdesc, &pstAf) != ZOK)
    {
        return false;
    }

    /* set fmtp of media */
    pstAf->ucType = EN_SDP_ATTR_MAXPTIME;
    pstAf->u.iPTime = iMaxPTime;

    return true;
}

bool SipMsgEncode::fillBodyData( ZUCHAR ucMtype,ZUCHAR ucMSubtype, ZDBUF zMsgBuf)
{
    if (Sip_MsgFillBodyData(_pstMsg,  ucMtype, ucMSubtype,  zMsgBuf) ==ZOK)
        return true;
    return false;
}


bool SipMsgEncode::fillBodyDataStr(ZUCHAR ucMtype, ZUCHAR ucMSubType, ST_ZOS_SSTR *pstStr)
{

    if (Sip_MsgFillBodyStr(_pstMsg,  ucMtype, ucMSubType,  pstStr) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::fillBodySDPDataStr(ZUCHAR ucMtype, ZUCHAR ucMSubType,  std::string strTem)
{
    ST_SIP_BODY_SPART *pstSpart;

    if (!_pstMsg || strTem.empty())
        return false;

    /* init session description */
    _pstMsg->stBody.ucPres = ZTRUE;
    _pstMsg->stBody.ucMpartPres = ZFALSE;
    _pstMsg->stBody.ucMtype = ucMtype;
    _pstMsg->stBody.ucMSubtype = ucMSubType;
    pstSpart = &_pstMsg->stBody.u.stSpart;
    pstSpart->ucType = EN_SIP_BODY_PART_STR;
    Zos_UbufCpyNLSStr(_pstMsg->zMemBuf, (char*)strTem.c_str(), strTem.size(), &pstSpart->u.stStr);

    return true;
}


bool SipMsgEncode::fillReginfoXmlBody(ST_DOM_DOC **ppstDoc,   ZDBUF *zMsgBuf, char *pXmlStr, int & iStrLen)
{
    //暂时注释掉XML 部分，好像libAvatar.a 不支持XML

    //ST_DOM_STR stName;
    //ST_DOM_ELEM *pstRoot;
    //ZINT iRet, iLen;

    ////* create the xml document, using 4K buffer */
    //iRet = Dom_DocCreateX(EN_XML_ENCODING_UTF8, 0x2000, ppstDoc);
    ////generate test datas
    //XmlNode rootNodeData  = GenerateTestXmlNodes();
    //ZOS_USSTR_SETS(&stName, rootNodeData._strTagName.c_str());
    ////create root node
    //Dom_DocCreateElem(*ppstDoc, &stName, &pstRoot);

    ////set node attributes and child nodes
    // SetXmlNodeAttrAndChildNodes( *ppstDoc,pstRoot, &rootNodeData);

    //////* create msg buffer */
    //ZOS_MSGBUF_CREATE(*zMsgBuf, 2000, 4, ZNULL);

    ////* save xml */
    //if (Dom_DocSaveXml(*ppstDoc, *zMsgBuf) != ZOK)
    //{

    //	ZOS_DBUF_DELETE(*zMsgBuf);
    //	Dom_DocDelete(*ppstDoc);
    //	return false;
    //}

    //ZCHAR *pcData;
    //iLen = Zos_DbufLen(*zMsgBuf);
    //if (!Zos_DbufIsFlat(*zMsgBuf))
    //{
    //	ZDBUF zFlatBuf = Zos_DbufFlat(*zMsgBuf);
    //	SIP_DBUF_DELETE(*zMsgBuf);
    //	*zMsgBuf = zFlatBuf;
    //}

    //Zos_DbufO2D(*zMsgBuf, 0, &pcData);
    //Zos_MemCpy(pXmlStr, pcData, iLen);
    //iStrLen = iLen;

    return true;
}


bool SipMsgEncode::SetXmlNodeAttrAndChildNodes(ST_DOM_DOC *pstDoc, ST_DOM_ELEM * pParentElem, XmlNode * pParentNode)
{
    //暂时注释掉XML 部分，好像libAvatar.a 不支持XML

    //ST_DOM_STR stName, stVal;
    //ST_DOM_ELEM * pChild;
    //ST_DOM_TXT *pTxtChld;
    ////set attributes
    //if (pParentNode->_vectAttr.size() > 0)
    //{
    //	for (unsigned int i = 0; i < pParentNode->_vectAttr.size(); i ++)
    //	{
    //		XmlNodeAttribute xmlAttr = *pParentNode->_vectAttr.at(i);
    //		/* set attributes */
    //		ZOS_USSTR_SETS(&stName, xmlAttr._AttrName.c_str());
    //		ZOS_USSTR_SETS(&stVal, xmlAttr._AttrValue.c_str());
    //		Dom_ElemSetAttr(pParentElem, &stName, &stVal);
    //	}

    //}

    ////set child nodes
    //if (pParentNode->_vectChlidNodes.size() > 0)
    //{
    //	for (unsigned int num = 0; num < pParentNode->_vectChlidNodes.size(); num++)
    //	{
    //		/* create the element */
    //		XmlNode childNode = pParentNode->_vectChlidNodes.at(num);

    //		if (childNode._vectAttr.size() == 0)
    //		{
    //			ZOS_USSTR_SETS(&stName, childNode._strTagName.c_str());
    //			Dom_DocCreateElem(pstDoc, &stName, &pChild);					//创建子节点

    //			ZOS_USSTR_SETS(&stName, childNode._strTxtValue.c_str());
    //			Dom_DocCreateTxt(pstDoc, &stName,&pTxtChld);

    //			Dom_NodeAppendChild(&pChild->stNode, &pTxtChld->stNode);
    //			Dom_NodeAppendChild(&pParentElem->stNode, &pChild->stNode);			//添加子节点
    //		}
    //		else
    //		{
    //			ZOS_USSTR_SETS(&stName, childNode._strTagName.c_str());
    //			Dom_DocCreateElem(pstDoc, &stName, &pChild);					//创建子节点
    //			SetXmlNodeAttrAndChildNodes(pstDoc, pChild, &childNode);			//地柜
    //			Dom_NodeAppendChild(&pParentElem->stNode, &pChild->stNode);			//添加子节点

    //		}
    //	}
    //}

    return true;
}


XmlNode SipMsgEncode::GenerateTestXmlNodes()
{
    vector<XmlNode> vectNodes;
    XmlNode rootNode;
    rootNode._strTagName = "reginfo";
    //xmlns="urn:ietf:params:xml:ns:reginfo" xmlns:gr="urn:ietf:params:xml:ns:gruuinfo" xmlns:eri="urn:3gpp:ns:extRegInfo:1.0" version="0" state="full"

    rootNode._vectAttr.push_back(new XmlNodeAttribute("xmlns", "urn:ietf:params:xml:ns:reginfo"));
    rootNode._vectAttr.push_back(new XmlNodeAttribute("xmlns:gr", "urn:ietf:params:xml:ns:gruuinfo"));
    rootNode._vectAttr.push_back(new XmlNodeAttribute("xmlns:eri", "urn:3gpp:ns:extRegInfo:1.0"));
    rootNode._vectAttr.push_back(new XmlNodeAttribute("version", "0"));
    rootNode._vectAttr.push_back(new XmlNodeAttribute("state", "full"));


    XmlNode registration1;
    registration1._strTagName = "registration";
    //aor="sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org" id="2549103" state="active"
    registration1._vectAttr.push_back(new XmlNodeAttribute("aor", "sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org"));
    registration1._vectAttr.push_back(new XmlNodeAttribute("id", "2549103"));

    //contact
    XmlNode contact; //state="active" event="registered" duration-registered="0" expires="1800" id="630978"
    contact._strTagName = "contact";
    contact._vectAttr.push_back(new XmlNodeAttribute("state", "active"));
    contact._vectAttr.push_back(new XmlNodeAttribute("event", "registered"));
    contact._vectAttr.push_back(new XmlNodeAttribute("expires", "0"));

    //<uri>sip:+8613957854053@10.188.2.3:5960</uri>
    XmlNode uri;
    uri._strTagName = "uri";
    uri._strTxtValue = "sip:+8613957854053@10.188.2.3:5960";

    contact._vectChlidNodes.push_back(uri);
    registration1._vectChlidNodes.push_back(contact);
    rootNode._vectChlidNodes.push_back(registration1);

    return rootNode;
}

bool  SipMsgEncode::CpyHdrFromTo(  ST_SIP_HDR_FROM_TO *pstDst, ST_SIP_HDR_FROM_TO *pstSrc)
{
    if (Sip_CpyHdrFromTo(_pstMsg->zMemBuf, pstDst, pstSrc) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::CpyHdrFromMsg( ST_SIP_MSG *pstMsgDst, ST_SIP_MSG *pstMsgSrc,	ZUCHAR ucType)
{
    if (!pstMsgDst || !pstMsgSrc|| ucType >= EN_SIP_HDR_EXT)
        return false;

    ST_SIP_MSG_HDR *pstDst, *pstSrc;

    /* find source header */
    pstSrc = Sip_HdrMsgFind(pstMsgSrc, ucType);
    if (!pstSrc) return false;

    /* create destination header */
    pstDst = Sip_HdrMsgCreateX(pstMsgDst->zMemBuf, &pstMsgDst->stHdrLst, ucType);
    if (!pstDst) return false;

    /* copy header based on type */
    switch (ucType)
    {
    case EN_SIP_HDR_FROM:
    case EN_SIP_HDR_TO:
        return Sip_CpyHdrFromTo(pstMsgDst->zMemBuf, pstDst->u.pstFromTo,
            pstSrc->u.pstFromTo) == ZOK ? true : false;

    case EN_SIP_HDR_CSEQ:
        return Sip_CpyHdrCseq(pstMsgDst->zMemBuf, pstDst->u.pstCseq,
            pstSrc->u.pstCseq) == ZOK ? true : false;

    case EN_SIP_HDR_CALL_ID:
        return Sip_CpyHdrCallId(pstMsgDst->zMemBuf, pstDst->u.pstCallId,
            pstSrc->u.pstCallId) == ZOK ? true : false;

    case EN_SIP_HDR_CONTACT:
        return Sip_CpyHdrContact(pstMsgDst->zMemBuf, pstDst->u.pstContact,
            pstSrc->u.pstContact) == ZOK ? true : false;

    case EN_SIP_HDR_ROUTE:
        return Sip_CpyHdrRoute(pstMsgDst->zMemBuf, pstDst->u.pstRoute,
            pstSrc->u.pstRoute) == ZOK ? true : false;

    case EN_SIP_HDR_VIA:
        return Sip_CpyHdrVia(pstMsgDst->zMemBuf, pstDst->u.pstVia,
            pstSrc->u.pstVia) == ZOK ? true : false;

    case EN_SIP_HDR_CONTENT_TYPE:
        return Sip_CpyHdrContentType(pstMsgDst->zMemBuf, pstDst->u.pstContentType,
            pstSrc->u.pstContentType) == ZOK ? true : false;

    case EN_SIP_HDR_P_ASSERTED_ID:
        return Sip_CpyHdrPAssertedId(pstMsgDst->zMemBuf, pstDst->u.pstPAssertedId,
            pstSrc->u.pstPAssertedId) == ZOK ? true : false;

    case EN_SIP_HDR_WARN:
        return Sip_CpyHdrWarn(pstMsgDst->zMemBuf, pstDst->u.pstWarn,
            pstSrc->u.pstWarn) == ZOK ? true : false;

    case EN_SIP_HDR_P_ANSWER_STATE:
        return Sip_CpyHdrPAnswerState(pstMsgDst->zMemBuf, pstDst->u.pstPAnswerState,
            pstSrc->u.pstPAnswerState) == ZOK ? true : false;
    default:
        return Sip_CpyAnyHdr(pstMsgDst->zMemBuf, pstDst, pstSrc) == ZOK ? true : false;
    }

    return true;
}

bool SipMsgEncode::ParmFillContactSipInstance(ST_ZOS_SSTR *pstVal)
{
    ST_SIP_HDR_CONTACT *pstContact;
    ST_SIP_CONTACT_PARM *pstParm;

    /* create contact header */
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if (!pstContact) return false;
    /* add a parameter */
    pstParm =(ST_SIP_CONTACT_PARM*) ABNF_LIST_HEAD_DATA(&pstContact->stParmLst);
    /*add +sip.instance=*/
    Sip_ParmFillContactSipInstance(_pstMsg->zMemBuf, pstParm, pstVal);

    return true;
}


bool SipMsgEncode::CopyRecorRouteToRoute(ST_SIP_MSG *pstMsgDst, ST_SIP_MSG *pstMsgSrc)
{
    ST_SIP_HDR_ROUTE * pstRouteDst = NULL, *pstRouteSrc = NULL;
    int iResult = 0;

    if (!pstMsgDst || !pstMsgSrc)
        return false;

    pstRouteSrc = (ST_SIP_HDR_ROUTE * )Sip_FindMsgHdr(pstMsgSrc, EN_SIP_HDR_RECORD_ROUTE);
    if (pstRouteSrc == NULL)
        return false;

    pstRouteDst = (ST_SIP_HDR_ROUTE * )Sip_FindMsgHdr(pstMsgSrc, EN_SIP_HDR_ROUTE);
    if (NULL == pstRouteDst)
    {
        pstRouteDst = (ST_SIP_HDR_ROUTE * )Sip_HdrMsgCreate(pstMsgDst,EN_SIP_HDR_ROUTE);
    }

    if(!pstRouteDst)
        return false;

    return Sip_CpyHdrRoute(pstMsgDst->zMemBuf, pstRouteDst,pstRouteSrc) == ZOK ? true : false;

}


bool SipMsgEncode::fillHdrMaxForwards(ZULONG lMaxForwards)
{
    ZUINT *piMaxForwards;

    /* get max forwards */
    if (!Sip_CfgGetMaxForwards()) return ZOK;

    /* find max-forwards header */
    piMaxForwards = (ZUINT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_MAX_FORWARDS);
    if (piMaxForwards) return ZOK;

    /* create max-forwards header */
    piMaxForwards = (ZUINT *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_MAX_FORWARDS);
    if (!piMaxForwards)
    {
        return false;
    }

    /* set the max forwards */
    *piMaxForwards = lMaxForwards;

    return true;
}

bool SipMsgEncode::fillHdrExtHdr(ZCHAR *pcName, ZCHAR *pcStr)
{
    return Sip_FillExtHdr(_pstMsg, pcName, pcStr) == ZOK ? true : false;
}

bool SipMsgEncode::fillHdrPCalledPartyIdByTel(const std::string & strGlobalNumber,const std::string & strParmName, const std::string & strParmValue)
{
    ST_SIP_HDR_P_CALLED_PTY_ID *pstHdrPCalled;
    ST_ZOS_SSTR stGlobalNumber;
    ST_ZOS_SSTR  stParmName, stValue;
    ST_SIP_PAR * pPar = NULL;
    void * pTem = NULL;
    SipMsgCommon::str2ZosSstr(strGlobalNumber,stGlobalNumber);
    SipMsgCommon::str2ZosSstr(strParmName,stParmName);
    SipMsgCommon::str2ZosSstr(strParmValue,stValue);

    /* create P-Preferred-Identity header */
    pstHdrPCalled = (ST_SIP_HDR_P_CALLED_PTY_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_CALLED_PTY_ID);
    if (!pstHdrPCalled) return false;

    pstHdrPCalled->stPtyIdSpec.stNameAddr.ucDispNamePres = 0;
    pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.ucType = EN_SIP_ADDR_SPEC_TEL;
    pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stTelUri.ucGlobalNumberPres = true;
    pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stTelUri.u.stGlobal.stNumberDigits = stGlobalNumber;

    ABNF_LIST_CREATE(&pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stTelUri.u.stGlobal.stParLst);
    if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf,&pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stTelUri.u.stGlobal.stParLst, sizeof(ST_SIP_PAR),&pTem) == ZOK)
    {
        pPar = (ST_SIP_PAR * )pTem;
        pPar->ucType = EN_SIP_PAR_PARM;
        ST_SIP_PARM stParm;
        stParm.ucValPres = 1;
        stParm.stPname = stParmName;
        stParm.stPval = stValue;
        pPar->u.stParm = stParm;

    }
    return true;
}

bool SipMsgEncode::fillHdrPCalledPartyIdBySipUri(const ST_SIP_SIP_URI & stSipUri, bool bSip)
{
    ST_SIP_HDR_P_CALLED_PTY_ID *pstHdrPCalled;
    ZUCHAR ucSipType;

    ucSipType = bSip ? EN_SIP_ADDR_SPEC_SIP : EN_SIP_ADDR_SPEC_SIPS;

    /* create P-Preferred-Identity header */
    pstHdrPCalled = (ST_SIP_HDR_P_CALLED_PTY_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_CALLED_PTY_ID);
    if (!pstHdrPCalled) return false;

    pstHdrPCalled->stPtyIdSpec.stNameAddr.ucDispNamePres = 0;
    pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.ucType = ucSipType;

    if (bSip)
        pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stSipUri = stSipUri;
    else
        pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stSipsUri = stSipUri;

    return true;
}


bool SipMsgEncode::fillHdrPAssertedIdentity(const ST_SIP_NA_SPEC & stNaSpec)
{
    ST_SIP_HDR_P_ASSERTED_ID * pstPAsserted = NULL;
    ST_SIP_PASSERTEDID_VAL * pParm = NULL;
    void * pTem = NULL;

    pstPAsserted = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_FindMsgHdr(_pstMsg,EN_SIP_HDR_P_ASSERTED_ID);
    if (!pstPAsserted)
    {
        pstPAsserted = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
    }

    if(!pstPAsserted)
        return false;
    //ABNF_LIST_CREATE(&pstPAsserted->stValLst);

    if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstPAsserted->stValLst, sizeof(ST_SIP_PASSERTEDID_VAL), &pTem) != ZOK)
    {
        return false;
    }

    pParm = (ST_SIP_PASSERTEDID_VAL * )pTem;
    pParm->stNaSpec = stNaSpec;

    return true;
}

bool SipMsgEncode::fillHdrPAssertedService(const std::string & strSer)
{
    ST_SIP_HDR_P_ASSERTED_SRV * pstHdrSer = NULL;
    ST_SIP_SRV_ID *pSrv = NULL;
    void * pTem = NULL;
    ST_ZOS_SSTR stSrv;

    SipMsgCommon::str2ZosSstr(strSer, stSrv);
    pstHdrSer = (ST_SIP_HDR_P_ASSERTED_SRV*)Sip_FindMsgHdr(_pstMsg,EN_SIP_HDR_P_ASSERTED_SRV);
    if (!pstHdrSer)
        pstHdrSer = (ST_SIP_HDR_P_ASSERTED_SRV*)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_SRV);

    if(!pstHdrSer)
        return false;

    if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstHdrSer->stVal, sizeof(ST_SIP_SRV_ID), &pTem) != ZOK)
    {
        return false;
    }

    pSrv = (ST_SIP_SRV_ID * )pTem;
    pSrv->stUrnSrvId = stSrv;

    return true;
}

bool SipMsgEncode::fillHdrContributionID(const std::string & strContId)
{
    ST_SIP_HDR_CONTRIBUTION_ID * pstHdrCrb = NULL;
    ST_ZOS_SSTR stValue;

    SipMsgCommon::str2ZosSstr(strContId, stValue);
    pstHdrCrb = (ST_SIP_HDR_CONTRIBUTION_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_CONTRIBUTION_ID);

    if(!pstHdrCrb)
        return false;

    pstHdrCrb->stContId = stValue;
    return true;
}

bool SipMsgEncode::fillHdrConversationID(const std::string & strConvertId)
{
    ST_ZOS_SSTR *pstHdr = NULL, stConvId;
    SipMsgCommon::str2ZosSstr(strConvertId,stConvId);

    pstHdr = (ST_ZOS_SSTR *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_CONVERSATION_ID);
    if (!pstHdr)
        return false;

    *pstHdr = stConvId;
    return true;
}

bool SipMsgEncode::fillHdrDate()
{
    ST_ZOS_SYS_TIME stDate;
    /* get date */
    Zos_SysTimeU(&stDate);
    return Sip_MsgFillHdrDate(_pstMsg, &stDate) == ZOK ? true : false;
}


bool  SipMsgEncode::fillHdrRAck(ZUINT iCseqNum,ZUINT iRspNum, ZUCHAR ucMethod)
{
    if (Sip_MsgFillHdrRAck(_pstMsg,  iCseqNum, iRspNum, ucMethod) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::fillHdrContentDisposition(const std::string & strDisp)
{
    if (Sip_MsgFillHdrContentDisp(_pstMsg, (char*)strDisp.c_str()) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::fillHdrExt(const std::string & name, const std::string & val)
{
    if (Sip_FillExtHdr(_pstMsg, (char*)name.c_str(), (char*)val.c_str()) != ZOK)
        return false;
    return true;
}

bool SipMsgEncode::fillHdrExts(const std::map<std::string, std::string> & headers)
{
    if (headers.size() == 0)
        return true;

    std::map<std::string, std::string>::const_iterator iterKey = headers.begin();
    for (; iterKey != headers.end(); iterKey++)
    {
        if (!fillHdrExt(iterKey->first, iterKey->second))
            return false;
    }
    return true;
}

bool SipMsgEncode::fillFromToPram(const std::string & name, const std::string & val)
{
    ST_SIP_HDR_FROM_TO * pstHdrFrom = NULL;
    ST_SIP_FROM_TO_PARM *pParam = NULL;
    void * pTem = NULL;

    pstHdrFrom = (ST_SIP_HDR_FROM_TO*)Sip_FindMsgHdr(_pstMsg,EN_SIP_HDR_FROM);
    if (!pstHdrFrom)
        return false;

    if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, &pstHdrFrom->stParmLst, sizeof(ST_SIP_FROM_TO_PARM), &pTem) != ZOK)
    {
        return false;
    }

    pParam = (ST_SIP_FROM_TO_PARM *)pTem;
    if (!pParam)
        return false;

    pParam->ucTagPres = ZFALSE;
    SipMsgCommon::str2ZosSstr(name, pParam->u.stParm.stName);
    if (val.empty() == false)
    {
        pParam->u.stParm.ucValPres = ZTRUE;
        pParam->u.stParm.stGenVal.ucType = EN_SIP_GEN_VAL_VAL;
        SipMsgCommon::str2ZosSstr(val, pParam->u.stParm.stGenVal.u.stVal);
    }
    return true;
}

bool SipMsgEncode::fillFromToPrams(const std::map<std::string, std::string> & params)
{
    if (params.size() == 0)
        return true;

    std::map<std::string, std::string>::const_iterator iterKey = params.begin();
    for (; iterKey != params.end(); iterKey++)
    {
        if (!fillFromToPram(iterKey->first, iterKey->second))
            return false;
    }
    return true;
}

bool SipMsgEncode::fillSipUriParams(ST_SIP_SIP_URI *pstSipUri, vector<ST_SIP_URI_PARM> & params)
{
    if (!pstSipUri || params.size() == 0)
        return false;

    for(unsigned int i = 0; i < params.size(); i++)
    {
        const ST_SIP_URI_PARM & param = params.at(i);
        if (param.ucType == EN_SIP_URI_PARM_TPT)
        {
            Sip_ParmFillSipUriTpt(_pstMsg->zMemBuf, pstSipUri, param.u.stTpt.ucType);
        }
        else if(param.ucType == EN_SIP_URI_PARM_USER)
        {
            Sip_ParmFillSipUriUser(_pstMsg->zMemBuf, pstSipUri, param.u.stUser.ucType);
        }
        else if(param.ucType == EN_SIP_URI_PARM_METHOD)
        {
            Sip_ParmFillSipUriMethod(_pstMsg->zMemBuf, pstSipUri, param.u.stMethod.ucType);
        }
        else if(param.ucType == EN_SIP_URI_PARM_SESS)
        {
            Sip_ParmFillSipUriSess(_pstMsg->zMemBuf, pstSipUri, param.u.ucSess);
        }
        else if(param.ucType == EN_SIP_URI_PARM_TTL)
        {
            Sip_ParmFillSipUriTtl(_pstMsg->zMemBuf, pstSipUri, param.u.iTtl);
        }
        else if (param.ucType == EN_SIP_URI_PARM_OTHER)
        {
            parmFillSipUriOthers(_pstMsg->zMemBuf, pstSipUri, param.u.stOther.stName, param.u.stOther.stVal);
        }
    }
    return true;
}

bool SipMsgEncode::fillHdrRequire(ZUINT iSuptFlag)
{
    return Sip_MsgFillHdrRequire(_pstMsg, iSuptFlag) == ZOK ? true : false;
}

bool SipMsgEncode::fillHdrMinSe(ZUINT secounds)
{
    return Sip_MsgFillHdrMinSe(_pstMsg, secounds) == ZOK ? true : false;
}

bool SipMsgEncode::fillHdrReason(const std::string &reason)
{
    ST_SIP_HDR_REASON *pstReason;
    ST_SIP_REASON_VAL *pstVal;
    ZUCHAR ucProtoType;
    ZUINT iCause;
    //603 DECLINE
    ucProtoType = EN_SIP_PROTOCOL_Q850;
    iCause = 21;
    /* create reason header */
    pstReason = (ST_SIP_HDR_REASON *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_REASON);
    if (!pstReason) return false;
    ABNF_LIST_CREATE(&pstReason->stValLst);
    /* add a reason value */
    if (Sip_ParmAnyLstAdd(_pstMsg->zMemBuf, (ZVOID *)&pstReason->stValLst,
        sizeof(ST_SIP_REASON_VAL), (ZVOID **)&pstVal) != ZOK)
        return false;
    pstVal->ucType = ucProtoType;
    ABNF_LIST_CREATE(&pstVal->stParmsLst);
    /* add reason cause */
    if (Sip_ParmFillReasonCause(_pstMsg->zMemBuf, pstVal, iCause) != ZOK)
        return false;
    /* add reason text */
    if (Sip_ParmFillReasonTxt(_pstMsg->zMemBuf, pstVal, (ZCHAR *)reason.c_str()) != ZOK)
        return false;
    return true;
}

//
////TEST
//void* SipMsgEncode::operator new(size_t size)
//{
//	void * p = ::operator new(size);
//	{
//		Common::RecLock lock(g_mutex);
//		g_mem.insert(make_pair(p, std::string(__FILE__) + std::string(__LINE__)));
//	}
//	return p;
//}
//void SipMsgEncode::operator delete(void* p)
//{
//	{
//		Common::RecLock lock(g_mutex);
//		map<void*, std::string>::iterator it = g_mem.find(p);
//		g_mem.erase(it);
//	}
//	::operator delete(p);
//}
