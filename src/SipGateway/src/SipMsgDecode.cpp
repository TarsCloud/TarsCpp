#include <mutex>

#include "SipMsgDecode.h"
#include "servant/Application.h"

extern std::mutex g_mutex;
extern map<void*, std::string> g_mem;

SipMsgDecode::SipMsgDecode()
{
    _pstMsg = NULL;
}

SipMsgDecode::~SipMsgDecode()
{
    if(_pstMsg)
    {
        Sip_MsgDelete(_pstMsg);
        _pstMsg = 0;
        _strMsg = "";
    }
}

/* decode the buffer to _pstMsg */
unsigned char SipMsgDecode::decode(const std::string &strBuffer,int iBufLen)
{
    int iParsedLen;

    /* check strBuffer */
    if(strBuffer.empty() || iBufLen == 0)
    {
        return EnSipMsgType_Incomplete;
    }

    /* 首先判断是否是 Ping  */
    std::string DOUBLE_CRLF = "\r\n\r\n";
    std::string DOUBLE_CRLF_Linux = "\n\n";
    if(strBuffer.size() >= DOUBLE_CRLF.size() && strBuffer.substr(0, DOUBLE_CRLF.size()) == DOUBLE_CRLF)
    {
        iParsedLen = DOUBLE_CRLF.size();
        TLOGDEBUG("[SipMsgDecode] Decode sipmsg failed: PingRFC5626" << endl);
        return EnSipMsgType_PingRFC5626;
    }

    /* 首先判断是否是 Pong */
    std::string CRLF = "\r\n";
    if(strBuffer.size() >= CRLF.size() && strBuffer.substr(0, CRLF.size()) == CRLF)
    {
        iParsedLen = CRLF.size();
        TLOGDEBUG("[SipMsgDecode] Decode sipmsg failed: PongRFC5626" << endl);
        return EnSipMsgType_PongRFC5626;
    }

    /* 查找分界符 "\r\n\r\n"，它作为 Header 与 Body 的分界 */
    int pos = strBuffer.find(DOUBLE_CRLF);
    int pos2 = strBuffer.find(DOUBLE_CRLF_Linux);
    if(pos < 0 && pos2 < 0)
    {
        iParsedLen = 0;
        TLOGERROR("[SipMsgDecode] Decode sipmsg failed: Msg is Incomplete, can't find the separator between hearders and body." << endl);
        return EnSipMsgType_Incomplete;
    }

    iParsedLen = pos + DOUBLE_CRLF.size();

    /* 检查 Header 是否存在 Content-Length 及值 */
    ST_ZOS_SSTR stMsgHdr;
    stMsgHdr.pcStr = (ZCHAR *)strBuffer.c_str();
    stMsgHdr.iLen = iParsedLen;
    ZUINT dwContentLen = 0;
    if (Sip_GetContentLen2(&stMsgHdr, &dwContentLen) != ZOK)
    {
        /* 若不存在 Content-Length 及值 */   //发CANCEL 给winsip,返回的487无Content-length，因此注释掉此处的返回，将Content-length当做0
        //TLOGWARN("[SipMsgDecode] Decode sipmsg failed: Msg is Invalid,get Content-length failed, msg:\n" << strBuffer << endl);
        dwContentLen = 0;
        //return EnSipMsgType_Invalid;
    }

    if((iBufLen - stMsgHdr.iLen) < dwContentLen)
    {
        /* Body部分不够长度了 */
        iParsedLen = 0;
        TLOGERROR("[SipMsgDecode] Decode sipmsg failed: Msg is Incomplete, body length is wrong." << endl);
        return EnSipMsgType_Incomplete;
    }

    iParsedLen = stMsgHdr.iLen + dwContentLen;

    /* Header 与 Body 内容都齐全了，开始解码 */
    if (_pstMsg != NULL)
    {
        Sip_MsgDelete(_pstMsg);
        _pstMsg = NULL;
    }

    if (SipMsgCommon::SipMsg_Decode((ZCHAR *)strBuffer.c_str(), iParsedLen, &_pstMsg) != ZOK)
    {
        TLOGERROR("[SipMsgDecode] Decode sipmsg failed: Call SipMsg_Decode failed! Sip msg:\n" << strBuffer << endl);
        return EnSipMsgType_Invalid;
    }

    if(_pstMsg == NULL)
    {
        TLOGERROR("[SipMsgDecode] Decode sipmsg failed: _pstMsg is NULL!" << endl);
        return EnSipMsgType_Invalid;
    }
    _strMsg = strBuffer.substr(0,iParsedLen);
    _iMsgLen = iParsedLen;

    EnSipMsgType type = EnSipMsgType_Invalid;
    if (isRequest())
    {
        type = EnSipMsgType_Request;
        unsigned char ucMethod;
        if(getMethod(ucMethod))
        {
            switch(ucMethod)
            {
            case EN_SIP_METHOD_REGISTER:                //register
                type = (EnSipMsgType)EnSipReqMethedType_Register;
                break;

            case EN_SIP_METHOD_SUBS:                    //subscribe
                type = (EnSipMsgType)EnSipReqMethedType_Subscribe;
                break;

            case EN_SIP_METHOD_OPTIONS:                 //options
                type = (EnSipMsgType)EnSipReqMethedType_Options;
                break;

            case EN_SIP_METHOD_NOTIFY:                  //notify
                type = (EnSipMsgType)EnSipReqMethedType_Notify;
                break;

            case EN_SIP_METHOD_INVITE:                  //invite
                type = (EnSipMsgType)EnSipReqMethedType_Invite;
                break;

            case EN_SIP_METHOD_MSG:                     //message
                type = (EnSipMsgType)EnSipReqMethedType_Message;
                break;

            case EN_SIP_METHOD_ACK:                     //ACK
                type = (EnSipMsgType)EnSipReqMethedType_ACK;
                break;

            case EN_SIP_METHOD_CANCEL:                  //CANCEL
                type = (EnSipMsgType)EnSipReqMethedType_CANCEL;
                break;
            }
        }
    }
    else
    {
        type = EnSipMsgType_Response;
        unsigned int iRspCode = 0;
        if(getRspStatusCode(iRspCode))
        {
            switch(iRspCode)
            {
            case 100:                      //Trying
                type = EnSipMsgType_RegRsp100;
                break;

            case 180:                      //Ringing
                type = EnSipMsgType_RegRsp180;
                break;

            case 200:                      //OK
                type = EnSipMsgType_RegRsp200;
                break;

            case 202:                      //ACCEPTED
                type = EnSipMsgType_RegRsp202;
                break;

            case 401:                      //Not Authorized
                type = EnSipMsgType_RegRsp401;
                break;

            case 480:                      //Not Authorized
                type = EnSipMsgType_RegRsp401;
                break;

            case 486:                      //Not Authorized
                type = EnSipMsgType_RegRsp401;
                break;

            case 487:                      //Not Authorized
                type = EnSipMsgType_RegRsp401;
                break;
            }
        }
    }

    return type;
}

bool SipMsgDecode::encode()
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
/* get sip version of _pstMsg */
int SipMsgDecode::getVersion()
{
    return 2;
}

/* get sip version */
bool SipMsgDecode::isRequest()
{
    return _pstMsg->ucReqPres == 1 ? true : false;
}

/* get max forwards */
bool SipMsgDecode::getMaxForward(int &iMaxForward)
{
    ZUINT dwMaxForward = 0;
    /* check _pstMsg */
    if(!_pstMsg)
    {
        return false;
    }

    if(Sip_MsgGetMaxForward(_pstMsg, &dwMaxForward) == ZOK)
    {
        iMaxForward = dwMaxForward;
        return true;
    }
    return false;
}

/* get sip method */
bool SipMsgDecode::getMethod(unsigned char &ucMethod)
{
    if(_pstMsg->ucReqPres)
    {
        ucMethod = _pstMsg->u.stReqLine.stMethod.ucType;
    }
    else
    {
        ZUINT dwCseq = 0;
        if(Sip_MsgGetCseq(_pstMsg, &dwCseq, &ucMethod) == ZFAILED)
            return false;
    }
    return true;
}

bool SipMsgDecode::getMinExpires(int &dwExpires)
{
    ZUINT *pdwExpires = NULL;
    if(Sip_MsgGetMinExpires(_pstMsg, pdwExpires) == ZOK)
    {
        dwExpires = *pdwExpires;
        return true;
    }
    return false;
}

/* get uri of _pstMsg */
unsigned char SipMsgDecode::getReqUri(std::string &strUserInfo, std::string &strHostname, int &dwPort, int & reqUriType)
{
    ST_SIP_REQ_URI * pstReqUri;
    if(_pstMsg->ucReqPres)
    {
        if (Sip_MsgGetReqUri(_pstMsg, &pstReqUri) != ZOK)
            return false;

        std::string strTel;
        ST_SIP_TEL_URI stTelUri;

        reqUriType = (int)pstReqUri->ucType;
        switch(reqUriType)
        {
        case EN_SIP_REQ_URI_SIP:
        case EN_SIP_REQ_URI_SIPS:
            strUserInfo = getUserInfoFromUri(*pstReqUri);
            SipMsgCommon::deHostPort(&pstReqUri->u.stSipUri.stHostPort, strHostname, dwPort);
            break;

        case EN_SIP_REQ_URI_IM:
            break;

        case EN_SIP_REQ_URI_PRES:
            break;

        case EN_SIP_REQ_URI_TEL:
            stTelUri = pstReqUri->u.stTelUri;
            if (stTelUri.ucGlobalNumberPres)
            {
                SipMsgCommon::zosSstr2Str(stTelUri.u.stGlobal.stNumberDigits,strTel);
            }
            else
            {
                SipMsgCommon::zosSstr2Str(stTelUri.u.stLocal.stNumberDigits, strTel);
            }
            strUserInfo = strTel;
            break;

        case EN_SIP_REQ_URI_ABSO:
            break;

        default:
            break;
        }
    }
    return reqUriType;
}

/* get request uri */
bool SipMsgDecode::getReqUriX(ST_SIP_REQ_URI **ppstReqUri)
{
    /* check _pstMsg */
    if(!_pstMsg)
    {
        return false;
    }

    /* check is request */
    if(!isRequest())
    {
        return false;
    }

    if(ppstReqUri)
    {
        *ppstReqUri = &(_pstMsg->u.stReqLine.stReqUri);
        return true;
    }
    return false;
}

// get response status code of _pstMsg
bool SipMsgDecode::getRspStatusCode(unsigned int &iRspCode)
{
    if(_pstMsg->ucReqPres)
    {
        printf("this is a request sip, did not have status code\r\n");
        return false;
    }
    else
    {
        ZUINT temCode = 0;
        if (Sip_MsgGetStatCode(_pstMsg, &temCode) == ZOK)
        {
            iRspCode = temCode;
        }
        return true;
    }
}

/* get response code */
bool SipMsgDecode::getRspStatusCode2(int &iRspCode)
{
    int result = 0;
    unsigned int statusCode = 0;
    result = Sip_MsgGetStatCode(_pstMsg, &statusCode);
    iRspCode = statusCode;
    return result == ZOK ? true : false;
}


/* get response status reason */
bool SipMsgDecode::getRspReason(std::string &strRspReason)
{
    if(!_pstMsg) /* check _pstMsg */
    {
        return false;
    }
    if(!_pstMsg->ucReqPres)
    {
        SipMsgCommon::zosSstr2Str(_pstMsg->u.stStatusLine.stReason, strRspReason);
        return true;
    }
    return false;
}

bool SipMsgDecode::getRspReason(std::string &text, unsigned int &protoType, unsigned int &cause)
{
    ST_ZOS_SSTR *pstReasonText;

    if (Sip_MsgGetReason(_pstMsg, &protoType, &cause, &pstReasonText) == ZOK)
    {
        if (ZOS_ISEMPTY_SSTR(pstReasonText))
            return true;

        SipMsgCommon::zosSstr2Str(*pstReasonText, text);
        return true;
    }
    return false;

}
bool SipMsgDecode::getHdrFromToTag(bool bFrom,std::string &strFromTag)
{
    ST_ZOS_SSTR *pstTag;
    if(Sip_MsgGetFromToTag(_pstMsg, bFrom, &pstTag) == ZOK)
    {
        SipMsgCommon::zosSstr2Str(*pstTag, strFromTag);
        return true;
    }
    return false;
}

/* get uri of header From */
bool SipMsgDecode::getFromUri(std::string &strDispname, std::string &strUsername,
                              std::string &strHostname, int &dwPort, unsigned char &ucReqUri)
{
    ST_SIP_NA_SPEC *pstNaSpec;
    ST_SIP_NAME_ADDR stNameAddr;
    ST_SIP_ADDR_SPEC stAddrSpec;
    ST_SIP_SIP_URI stSipUri;
    ST_SIP_TEL_URI stTelUri;

    if(Sip_MsgGetFromToNaSpec(_pstMsg, ZTRUE, &pstNaSpec) == ZOK)
    {
        if(pstNaSpec->ucNameAddrPres)
        {
            stNameAddr = pstNaSpec->u.stNameAddr;
            stAddrSpec = stNameAddr.stAddrSpec;
            SipMsgCommon::zosSstr2Str(stNameAddr.stDispName.u.stName, strDispname); /* setDispname */
        }
        else
        {
            stAddrSpec = pstNaSpec->u.stAddrSpec;
        }
        ucReqUri = stAddrSpec.ucType; /* set ucReqUri */
        switch(ucReqUri)
        {
        case EN_SIP_ADDR_SPEC_SIP:
        case EN_SIP_ADDR_SPEC_SIPS:
            {
                stSipUri = ucReqUri == EN_SIP_ADDR_SPEC_SIP ? stAddrSpec.u.stSipUri : stAddrSpec.u.stSipsUri;
                SipMsgCommon::zosSstr2Str(stSipUri.stUserInfo.stUser, strUsername); /* set strUsername */
                SipMsgCommon::deSipHost(&stSipUri.stHostPort.stHost , strHostname); /* set strHostname */
                dwPort = stSipUri.stHostPort.iPort; /* set dwPort */
                return true;
            }
            break;

        case EN_SIP_ADDR_SPEC_TEL:
            stTelUri = stAddrSpec.u.stTelUri;
            if (stTelUri.ucGlobalNumberPres)
            {
                SipMsgCommon::zosSstr2Str(stTelUri.u.stGlobal.stNumberDigits, strUsername);
                if (strUsername.find("+") < 0)
                    strUsername = "+" + strUsername;
            }
            else
            {
                SipMsgCommon::zosSstr2Str(stTelUri.u.stLocal.stNumberDigits, strUsername);
            }
            strHostname = "";
            dwPort = 0;
            break;

        default:
            break;
        }
    }
    return true;
}

/* get uri of header From */
bool SipMsgDecode::getFromUriX(unsigned char &ucUriType, void **ppstUri)
{
    /* 这个方法有些问题 */

    ST_SIP_NA_SPEC *pstNaSpec;
    ST_SIP_NAME_ADDR stNameAddr;
    ST_SIP_ADDR_SPEC stAddrSpec;
    if(Sip_MsgGetFromToNaSpec(_pstMsg, ZTRUE, &pstNaSpec) == ZOK)
    {
        if(pstNaSpec->ucNameAddrPres)
        {
            stNameAddr = pstNaSpec->u.stNameAddr;
            stAddrSpec = stNameAddr.stAddrSpec;
        }
        else
        {
            stAddrSpec = pstNaSpec->u.stAddrSpec;
        }
        ucUriType = stAddrSpec.ucType; /* set ucUriType */
        switch(ucUriType)
        {
        case EN_SIP_ADDR_SPEC_SIP:
            {
                *ppstUri = &(stAddrSpec.u.stSipUri);    /* set ppstUri */
                return true;
            }
        case EN_SIP_ADDR_SPEC_TEL:
            {
                *ppstUri = &(stAddrSpec.u.stTelUri);
                return true;
            }
        }
    }
    return true;
}

/* get sip uri or sips uri from To header */
bool SipMsgDecode::getHdrToSipUri(std::string &strDispName, std::string &strUserName,
                                  std::string &strHostName, int iPort)
{
    ST_SIP_NA_SPEC *pstNaSpec;
    ST_SIP_NAME_ADDR stNameAddr;
    ST_SIP_ADDR_SPEC stAddrSpec;
    ST_SIP_SIP_URI stSipUri;

    /* get name-spec and addr-spec */
    if(Sip_MsgGetFromToNaSpec(_pstMsg, ZFALSE, &pstNaSpec) == ZFAILED)
        return false;
    if(pstNaSpec->ucNameAddrPres)
    {
        stNameAddr = pstNaSpec->u.stNameAddr;
        stAddrSpec = stNameAddr.stAddrSpec;
        /* set dispname */
        if(stNameAddr.ucDispNamePres)
        {
            if(stNameAddr.stDispName.ucNamePres)
            {
                SipMsgCommon::zosSstr2Str(stNameAddr.stDispName.u.stName, strDispName);
            }
        }
    }
    else
    {
        stAddrSpec = pstNaSpec->u.stAddrSpec;
    }

    /* get sip uri or sips uri */
    switch (stAddrSpec.ucType)
    {
    case EN_SIP_ADDR_SPEC_SIP:
        stSipUri = stAddrSpec.u.stSipUri;
        break;
    case EN_SIP_ADDR_SPEC_SIPS:
        stSipUri = stAddrSpec.u.stSipsUri;
    }

    /* set username */
    if(stSipUri.ucUserInfoPres)
        SipMsgCommon::zosSstr2Str(stSipUri.stUserInfo.stUser, strUserName);

    /* set host and port */
    SipMsgCommon::deHostPort(&stSipUri.stHostPort, strHostName, iPort);
    return true;
}

bool SipMsgDecode::getHdrToTelUri(std::string &strDispName, std::string &strNumber)
{
    ST_SIP_NA_SPEC *pstNaSpec;
    ST_SIP_NAME_ADDR stNameAddr;
    ST_SIP_ADDR_SPEC stAddrSpec;
    ST_SIP_TEL_URI stTelUri;

    /* get na-spe*/
    if(Sip_MsgGetFromToNaSpec(_pstMsg, ZFALSE, &pstNaSpec) == ZFAILED)
        return false;

    /* get addr-spec */
    if(pstNaSpec->ucNameAddrPres)
    {
        stNameAddr = pstNaSpec->u.stNameAddr;
        stAddrSpec = stNameAddr.stAddrSpec;

        /* set strDispName */
        if(stNameAddr.ucDispNamePres)
        {
            if(stNameAddr.stDispName.ucNamePres)
            {
                SipMsgCommon::zosSstr2Str(stNameAddr.stDispName.u.stName, strDispName);
            }
            else
            {
                SipMsgCommon::zosSstr2Str(stNameAddr.stDispName.u.stQStr, strDispName);
            }
        }
    }
    else
    {
        stAddrSpec = pstNaSpec->u.stAddrSpec;
    }

    /* check the uri type */
    if(stAddrSpec.ucType != EN_SIP_ADDR_SPEC_TEL)
        return false;

    /* set strNumber */
    stTelUri = stAddrSpec.u.stTelUri;
    if(stTelUri.ucGlobalNumberPres)
    {
        SipMsgCommon::zosSstr2Str(stTelUri.u.stGlobal.stNumberDigits, strNumber);
    }
    else
    {
        SipMsgCommon::zosSstr2Str(stTelUri.u.stLocal.stNumberDigits, strNumber);
    }

    return true;
}

bool SipMsgDecode::getHdrFromToUri(bool bFrom,std::string &strDispname, std::string &strUsername,
                                   std::string &strHostname, int &dwPort, unsigned char &ucReqUri)
{
    ST_SIP_NA_SPEC *pstNaSpec;
    ST_SIP_NAME_ADDR stNameAddr;
    ST_SIP_ADDR_SPEC stAddrSpec;
    ST_SIP_SIP_URI stSipUri;
    ST_SIP_TEL_URI stTelUri;

    if(Sip_MsgGetFromToNaSpec(_pstMsg, bFrom, &pstNaSpec) == ZOK)
    {
        //set disp name
        if(pstNaSpec->ucNameAddrPres)
        {
            stNameAddr = pstNaSpec->u.stNameAddr;
            stAddrSpec = stNameAddr.stAddrSpec;
            if (stNameAddr.ucDispNamePres == ZTRUE)
            {
                SipMsgCommon::zosSstr2Str(stNameAddr.stDispName.u.stName, strDispname); /* setDispname */
            }
        }
        else
        {
            stAddrSpec = pstNaSpec->u.stAddrSpec;
        }
        // set ucReqUri
        ucReqUri = stAddrSpec.ucType;
        switch(ucReqUri)
        {
        case EN_SIP_ADDR_SPEC_SIP:
        case EN_SIP_ADDR_SPEC_SIPS:
            {
                stSipUri = ucReqUri == EN_SIP_ADDR_SPEC_SIP ? stAddrSpec.u.stSipUri : stAddrSpec.u.stSipsUri;
                if (stSipUri.ucUserInfoPres == ZTRUE)
                {
                    SipMsgCommon::zosSstr2Str(stSipUri.stUserInfo.stUser, strUsername); /* set strUsername */
                }
                SipMsgCommon::deSipHost(&stSipUri.stHostPort.stHost , strHostname); /* set strHostname */
                dwPort = stSipUri.stHostPort.iPort; /* set dwPort */
                return true;
            }

        case EN_SIP_ADDR_SPEC_TEL:
            stTelUri = stAddrSpec.u.stTelUri;
            if(stTelUri.ucGlobalNumberPres)
            {
                SipMsgCommon::zosSstr2Str(stTelUri.u.stGlobal.stNumberDigits, strUsername);
                if (strUsername.find("+") < 0)
                    strUsername = "+" + strUsername;
            }
            else
            {
                SipMsgCommon::zosSstr2Str(stTelUri.u.stLocal.stNumberDigits, strUsername);
            }
            break;

        default:  //其余的类型暂时空缺

            break;
        }
    }
    return true;
}

bool SipMsgDecode::getHdrCseq(int &dwCseq )
{
    ZUINT pdwCseq = 0;
    ZUCHAR pucMethod = 0;

    if(Sip_MsgGetCseq(_pstMsg, &pdwCseq, &pucMethod) == ZOK)
    {
        dwCseq = pdwCseq;
        return true;
    }
    else
    {
        return false;
    }
}

bool SipMsgDecode::getCallId(std::string &strNumber, std::string &strHost)
{
    ST_SIP_CALLID *pstSipCallId;
    if(Sip_MsgGetCallId(_pstMsg, &pstSipCallId) == ZOK)
    {
        SipMsgCommon::zosSstr2Str(pstSipCallId->stNumber, strNumber);
        SipMsgCommon::zosSstr2Str(pstSipCallId->stHost, strHost);
        return true;
    }
    else
    {
        return false;
    }
}

bool SipMsgDecode::getContactNaSpec(ST_SIP_NA_SPEC &stNaSpec)
{
    ST_SIP_HDR_CONTACT *pstHdrContact;
    ST_SIP_CONTACT_PARM *pstContactParm;
    unsigned char ucHeader;

    ucHeader = EN_SIP_HDR_CONTACT;
    pstHdrContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(_pstMsg, ucHeader);
    if(!pstHdrContact) return false;
    pstContactParm = (ST_SIP_CONTACT_PARM *)ABNF_LIST_HEAD_DATA(&pstHdrContact->stParmLst);
    if(!pstContactParm) return false;
    stNaSpec = pstContactParm->stNaSpec;
    return true;
}

/* get header contact addr-spect */
bool SipMsgDecode::getContactAddrSpec(ST_SIP_ADDR_SPEC &stAddrSpec)
{
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    if(Sip_MsgGetContactAddrSpec(_pstMsg, &pstAddrSpec) == ZOK)
    {
        stAddrSpec = *pstAddrSpec;
        return true;
    }
    return false;
}

/* get header contact name-addr */
bool SipMsgDecode::getContactNameAddr(ST_SIP_NAME_ADDR &stNameAddr)
{
    ST_SIP_NA_SPEC stNaSpec;
    if(!getContactNaSpec(stNaSpec)) return false;
    if(stNaSpec.ucNameAddrPres)
    {
        stNameAddr = stNaSpec.u.stNameAddr;
        return true;
    }
    return false;
}

bool SipMsgDecode::getHdrCallId(ST_SIP_CALLID & stCallId)
{
    ST_SIP_CALLID *pstSipCallId;
    Sip_MsgGetCallId(_pstMsg, &pstSipCallId);
    if(pstSipCallId)
    {
        stCallId =  *pstSipCallId;
        return true;
    }

    return false;
}

bool SipMsgDecode::getContentLength(int &iContentLen)
{
    ZUINT dwContentLen = 0;
    if(Sip_GetContentLen(_pstMsg, &dwContentLen) == ZOK)
    {
        iContentLen = dwContentLen;
        return true;
    }
    return false;
}

/**< @brief algorithm type EN_SIP_ALGO_VAL */
bool SipMsgDecode::getWwwAuthen(unsigned char &ucAlgo, std::string &strRealm,
                                std::string &strNonce, std::string &strOpaque, unsigned char &ucQop,
                                std::string &strDomain)
{
    ST_SIP_CHALLENGE *pstChallenge = 0;
    ST_ABNF_LIST_NODE *pstNode = 0;
    ST_SIP_DIGEST *pstDigest = 0;
    ST_SIP_DIGEST_CLN *pstDigestCln = 0;

    if(Sip_MsgGetWwwAuthen(_pstMsg, &pstChallenge))
    {
        pstDigest = &(pstChallenge->u.stDigest);

        /* get param algorithm */
        if(Sip_ParmDigestFind(pstDigest, EN_SIP_DIGEST_CLN_ALGO, &pstDigestCln) == ZOK)
        {
            ucAlgo = pstDigestCln->u.stAlgo.u.stAlgoVal.ucType;
        }

        /* get param realm */
        if(Sip_ParmDigestFind(pstDigest, EN_SIP_DIGEST_CLN_REALM, &pstDigestCln) == ZOK)
        {
            SipMsgCommon::zosSstr2Str(pstDigestCln->u.stRealm, strRealm);
        }

        /* get param nonce */
        if(Sip_ParmDigestFind(pstDigest, EN_SIP_DIGEST_CLN_NONCE, &pstDigestCln) == ZOK)
        {
            SipMsgCommon::zosSstr2Str(pstDigestCln->u.stNonce, strNonce);
        }

        /* get param opaque */
        if(Sip_ParmDigestFind(pstDigest, EN_SIP_DIGEST_CLN_OPAQUE, &pstDigestCln) == ZOK)
        {
            SipMsgCommon::zosSstr2Str(pstDigestCln->u.stOpaque, strOpaque);
        }

        /* get param qop */
        if(Sip_ParmDigestFind(pstDigest, EN_SIP_DIGEST_CLN_QOPTS, &pstDigestCln) == ZOK)
        {
            ucQop = pstDigestCln->u.stQopts.stQval.ucType;
        }

        /* get param domain */
        if(Sip_ParmDigestFind(pstDigest, EN_SIP_DIGEST_CLN_DOMAIN, &pstDigestCln) == ZOK)
        {
            /* TODO */
            printf("TODO get param domain\r\n");
        }
        return true;
    }
    else
    {
        return false;
    }
}

/* get header Expires */
bool SipMsgDecode::getExpires(int &dwExpires)
{
    ZUINT expires = 0;
    if(Sip_MsgGetExpires(_pstMsg, &expires) == ZOK)
    {
        dwExpires = expires;
        return true;
    }
    return false;
}

bool SipMsgDecode::getWwwDigest(ST_SIP_DIGEST &stDigest)
{
    ST_SIP_CHALLENGE *pstChallenge = 0;

    if(Sip_MsgGetWwwAuthen(_pstMsg, &pstChallenge) == ZOK)
    {
        stDigest = pstChallenge->u.stDigest;
        return true;
    }
    return false;
}

bool  SipMsgDecode::getProxyDigest(ST_SIP_DIGEST &stDigest)
{
    ST_SIP_CHALLENGE *pstChallenge = 0;

    if(Sip_MsgGetProxyAuthen(_pstMsg, &pstChallenge) == ZOK)
    {
        stDigest = pstChallenge->u.stDigest;
        return true;
    }
    return false;
}


bool  SipMsgDecode::getDigest(ST_SIP_DIGEST &stDigest, unsigned long respCode)
{
    switch(respCode)
    {
    case 401:
        return getWwwDigest(stDigest);
        break;

    case 407:
        return getProxyDigest(stDigest);
        break;

    default:
        return false;
    }

    return true;
}

bool  SipMsgDecode::getDigestQopX(ST_SIP_DIGEST *pstDigest, ZUCHAR *pucQopType)
{
    if (Sip_ParmPickDigestQopX(pstDigest, pucQopType) == ZOK)
        return true;
    return false;
}

//get Digest realm
bool SipMsgDecode::getDigestRealm(  ST_SIP_DIGEST *pstDigest, std::string & realm)
{
    ST_ZOS_SSTR * pstRealm = 0;
    if (Sip_ParmPickDigestRealm(pstDigest, &pstRealm) != ZOK)
    {
        return false;
    }

    SipMsgCommon::zosSstr2Str(*pstRealm, realm);
    return true;
}


bool SipMsgDecode::getDigestNonce(  ST_SIP_DIGEST *pstDigest, std::string & nonce)
{
    ST_ZOS_SSTR * pstNonce = 0;
    if (Sip_ParmPickDigestNonce(pstDigest, &pstNonce) != ZOK)
    {
        return false;
    }
    SipMsgCommon::zosSstr2Str(*pstNonce, nonce);
    return true;
}

bool SipMsgDecode::getDigestOpaque(  ST_SIP_DIGEST *pstDigest, std::string & opaque)
{
    ST_ZOS_SSTR * pstOpaque = 0;
    if (Sip_ParmPickDigestOpaque(pstDigest, &pstOpaque) != ZOK)
    {
        return false;
    }
    SipMsgCommon::zosSstr2Str(*pstOpaque, opaque);
    return true;
}

//get Digest Algo
bool SipMsgDecode::getDigestAlgo(  ST_SIP_DIGEST *pstDigest, unsigned char & algoType)
{
    unsigned char algo;
    if (Sip_ParmPickDigestAlgoX(pstDigest, 	&algo) != ZOK)
    {
        return false;
    }
    algoType = algo;
    return true;
}

bool SipMsgDecode::getNumber(std::string &strNumber)
{
    ST_ABNF_LIST_NODE *pstNode;
    ST_SIP_MSG_HDR *pstHdr;
    void *pstV;
    std::string strVal;
    int index0, index1;

    /* check thi _pstMsg */
    if (!_pstMsg || !_pstMsg->ucPres)
        return false;

    /* look for the message header matched */
    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstNode, pstV)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstV;
        if (pstHdr->ucType == EN_SIP_HDR_P_ASO_URI)
        {
            std::string strTemp;
            SipMsgCommon::zosSstr2Str(pstHdr->stHdrVal, strTemp);
            strVal += strTemp;
        }
    }
    index0 = strVal.find("tel:");
    if(index0 != -1)
    {
        index0 += 4;
    }
    else
    {
        return false;
    }
    index1 = strVal.find(">", index0);
    if(index1 != -1)
    {
        strNumber = strVal.substr(index0, index1 - index0);
        return true;
    }
    return false;
}


bool SipMsgDecode::getUserAgent(std::string &strName, std::string &strVer,
                                std::string &strComm)
{
    ST_ZOS_SSTR *pstName, *pstVer, *pstComm;
    if(Sip_MsgGetUserAgent(_pstMsg, &pstName, &pstVer, &pstComm) == ZOK)
    {
        if (pstName != NULL)
            SipMsgCommon::zosSstr2Str(*pstName, strName);
        if (pstVer != NULL)
            SipMsgCommon::zosSstr2Str(*pstVer, strVer);
        if (pstComm != NULL)
            SipMsgCommon::zosSstr2Str(*pstComm, strComm);
        return true;
    }
    return false;
}

/* get header Via branch */
bool SipMsgDecode::getViaBranch(std::string &strBranch)
{
    ST_ZOS_SSTR *pstBranch;
    if(Sip_MsgGetViaBranch(_pstMsg, &pstBranch) == ZOK)
    {
        if(pstBranch)
        {
            SipMsgCommon::zosSstr2Str(*pstBranch, strBranch);
            return true;
        }
    }
    return false;
}

/* get header Via other value except branch */
bool SipMsgDecode::getHdrViaOther(unsigned char &ucTptType, std::string &strHost, int &iPort,std::string &strRecvHost, int &iRport)
{
    ST_SIP_HDR_VIA *pstHdrVia = 0;
    ST_SIP_VIA_PARM *pstViaParm = 0;
    ST_ZOS_INET_ADDR stAddr;
    char acRmtAddr[ZINET_IPV4_PORT_STR_SIZE];
    ZUINT pwRport = 0;

    pstHdrVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (!pstHdrVia)
        return false;

    pstViaParm = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstHdrVia->stParmLst);
    if (!pstViaParm)
        return false;

    /* get ucTptType */
    Sip_ParmPickViaSentProtocol(pstViaParm, &ucTptType);

    /* get strHost, iPort */
    if(Sip_ParmPickViaSentBy(pstViaParm, &stAddr) == ZOK)
    {
        ZOS_INET_ADDR_TO_STR(&stAddr, acRmtAddr);
        std::string strTemp = std::string(acRmtAddr);
        int iTemp = strTemp.find(":");
        if(iTemp != -1)
        {
            strHost = strTemp.substr(0, iTemp);
            try {
                iPort = tars::TC_Common::strto<int>(strTemp.substr(iTemp + 1));
            } catch (...) {
                iPort = 0;
            }
        }
        else
        {
            strHost = strTemp;
        }
    }

    /* get strRecvHost */
    if(Sip_ParmPickViaRecv(pstViaParm, &stAddr) == ZOK)
    {
        ZOS_INET_ADDR_TO_STR(&stAddr, acRmtAddr);
        strRecvHost = std::string(acRmtAddr);
    }

    /* get iRport */
    if(Sip_ParmPickViaRport(pstViaParm, &pwRport) == ZOK)
    {
        iRport = pwRport;
    }
    else
    {
        iRport = 0;
    }

    return true;
}

bool SipMsgDecode::getHdrVia(vector<ViaPart> & viaHeaders)
{
    ST_SIP_MSG_HDR * pstHdr = 0;
    ST_ABNF_LIST_NODE * pstHdrNode = 0, *pstParmNode = 0;
    ZVOID * pstViaParmPtr = 0, *pstVidHdrPtr = 0;
    ST_SIP_HDR_VIA * pstHdrVia = 0;
    ST_SIP_VIA_PARM * pstViaParm = 0;
    ST_ZOS_INET_ADDR stAddr;
    char acRmtAddr[ZINET_IPV4_PORT_STR_SIZE];
    ZUINT pwRport = 0;
    unsigned char  ucTptType = 0;
    std::string  strHost, strRecvHost;
    int iRport = 0, iPort = 0, recvPort = 0;

    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;

        if (pstHdr->ucType != EN_SIP_HDR_VIA)
            continue;

        pstHdrVia = pstHdr->u.pstVia;
        int index = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdrVia->stParmLst, pstParmNode, pstViaParmPtr)
        {
            pstViaParm = (ST_SIP_VIA_PARM *)pstViaParmPtr;
            if(pstViaParm == NULL)
            {
                break;
            }

            /* get ucTptType */
            Sip_ParmPickViaSentProtocol(pstViaParm, &ucTptType);

            /* get strHost, iPort */
            if(Sip_ParmPickViaSentBy(pstViaParm, &stAddr) == ZOK)
            {
                ZOS_INET_ADDR_TO_STR(&stAddr, acRmtAddr);
                std::string strTemp = std::string(acRmtAddr);
                int iTemp = strTemp.find(":");
                if(iTemp != -1)
                {
                    strHost = strTemp.substr(0, iTemp);
                    try {
                        iPort = tars::TC_Common::strto<int>(strTemp.substr(iTemp + 1));
                    } catch (...) {
                        iPort = 0;
                    }
                }
                else
                {
                    strHost = strTemp;
                }
            }

            /* get strRecvHost */
            if(Sip_ParmPickViaRecv(pstViaParm, &stAddr) == ZOK)
            {
                ZOS_INET_ADDR_TO_STR(&stAddr, acRmtAddr);
                strRecvHost = std::string(acRmtAddr);

                int iTemp = strRecvHost.find(":");
                if(iTemp != -1)
                {
                    strRecvHost = strRecvHost.substr(0, iTemp);
                    try {
                        recvPort = tars::TC_Common::strto<int>(strRecvHost.substr(iTemp + 1));
                    } catch (...) {
                        recvPort = 0;
                    }
                }
                else
                {
                    strRecvHost = strRecvHost;
                }
            }


            /* get iRport */
            if(Sip_ParmPickViaRport(pstViaParm, &pwRport) == ZOK)
            {
                iRport = pwRport;
            }
            else
            {
                iRport = 0;
            }

            //get branch
            ST_ZOS_SSTR * pstBranch = 0;
            std::string strBranch;
            if (Sip_ParmPickViaBranch(pstViaParm, &pstBranch) == ZOK)
            {
                if(pstBranch)
                {
                    SipMsgCommon::zosSstr2Str(*pstBranch, strBranch);
                }
            }

            int bPort = iRport > 0 ? true: false;
            ViaPart viaPart;
            viaPart.setViaPart(strHost, iPort, ucTptType, bPort, iRport, strBranch, 0, strRecvHost, recvPort);

            ST_ABNF_LIST_NODE * pstParmsNode = 0;
            ZVOID * pstViaParmsPtr = 0;
            ST_SIP_VIA_PARMS * pstViaParms = 0;

            FOR_ALL_DATA_IN_ABNF_LIST(&pstViaParm->stParmsLst, pstParmsNode, pstViaParmsPtr)
            {
                pstViaParms = (ST_SIP_VIA_PARMS *)pstViaParmsPtr;
                if(pstViaParms == NULL)
                {
                    break;
                }

                if (pstViaParms->ucType == EN_SIP_VIA_PARMS_EXT)
                {
                    viaPart._paramsExt.push_back(*pstViaParms);
                }
            }

            viaHeaders.push_back(viaPart);
        }
    }
    return true;
}

bool SipMsgDecode::getHdrViaParams(vector<ST_SIP_VIA_PARMS> & params)
{
    ST_SIP_HDR_VIA *pstHdrVia = 0;
    ST_SIP_VIA_PARM *pstViaParm = 0;
    ST_ABNF_LIST_NODE * pstParmsNode = 0;
    ZVOID * pstViaParmsPtr = 0;
    ST_SIP_VIA_PARMS * pstViaParms = 0;

    pstHdrVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (0 == pstHdrVia)
        return false;

    pstViaParm = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstHdrVia->stParmLst);
    if (0 == pstViaParm)
        return false;

    FOR_ALL_DATA_IN_ABNF_LIST(&pstViaParm->stParmsLst, pstParmsNode, pstViaParmsPtr)
    {
        pstViaParms = (ST_SIP_VIA_PARMS *)pstViaParmsPtr;
        if(pstViaParms == NULL)
        {
            break;
        }

        if (pstViaParms->ucType == EN_SIP_VIA_PARMS_EXT)
        {
            params.push_back(*pstViaParms);
        }
    }
    return true;
}

bool SipMsgDecode::getHrdVia(ST_SIP_HDR_VIA & stVia)
{
    ST_SIP_HDR_VIA *pstHdrVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (!pstHdrVia)
        return false;

    stVia = *pstHdrVia;
    return true;
}

bool SipMsgDecode::getHrdVias(vector<ST_SIP_HDR_VIA> & vias)
{
    ST_SIP_MSG_HDR * pstHdr = 0;
    ST_ABNF_LIST_NODE * pstHdrNode = 0;
    ZVOID *pstVidHdrPtr = 0;

    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;

        if (pstHdr->ucType != EN_SIP_HDR_VIA)
            continue;

        vias.push_back(*(pstHdr->u.pstVia));
    }
    return true;
}

// get body of _pstMsg
std::string SipMsgDecode::getBody()
{
    return _strMsg;
}

/* get SDP media audio description */
bool SipMsgDecode::getSdpAudioDesc(int &iPort, unsigned char &ucProtoType, vector<int> &iFmtVec)
{
    ST_SDP_SESS_DESC *pstSdp;
    ST_SDP_MDESC *pstMdesc;
    ST_ABNF_LIST_NODE *pstMNode;
    ZVOID *pstVoid;
    if(Sip_MsgGetBodySdp(_pstMsg, &pstSdp) == ZOK)
    {
        FOR_ALL_DATA_IN_ABNF_LIST(&pstSdp->stMdescLst, pstMNode, pstVoid)
        {
            pstMdesc = (ST_SDP_MDESC *)pstVoid;
            if(pstMdesc->stMedia.ucMediaType == EN_SDP_MEDIA_AUDIO)
            {
                iPort = pstMdesc->stMedia.wPort;
                ucProtoType = pstMdesc->stMedia.ucProtoType;
                /* TODO */
                /* set iFmtVec */
                iFmtVec.clear();
                return true;
            }
        }
    }
    return false;
}

/* get SDP media video description */
bool SipMsgDecode::getSdpVideoDesc(int &iPort, unsigned char &ucProtoType, vector<int> &iFmtVec)
{
    ST_SDP_SESS_DESC *pstSdp;
    ST_SDP_MDESC  *pstMdesc;
    ST_ABNF_LIST_NODE *pstNode;
    ZVOID *pstVoid;
    if(Sip_MsgGetBodySdp(_pstMsg, &pstSdp) == ZOK)
    {
        FOR_ALL_DATA_IN_ABNF_LIST(&pstSdp->stMdescLst, pstNode, pstVoid)
        {
            pstMdesc = (ST_SDP_MDESC *)pstVoid;
            if(pstMdesc->stMedia.ucMediaType == EN_SDP_MEDIA_VIDEO)
            {
                iPort = pstMdesc->stMedia.wPort;
                ucProtoType = pstMdesc->stMedia.ucProtoType;
                /* TODO */
                /* set iFmtVec */
                iFmtVec.clear();
            }
        }
    }
    return false;
}

/* get origin description */
bool SipMsgDecode::getSdpOriginDesc(std::string &strUsername)
{
    ST_SDP_SESS_DESC *pstSdp;
    if(Sip_MsgGetBodySdp(_pstMsg, &pstSdp) == ZOK)
    {
        SipMsgCommon::zosSstr2Str(pstSdp->stOrig.stUserName, strUsername);
        return true;
    }
    return false;
}

/* get userinfo from ST_SIP_REQ_URI */
std::string SipMsgDecode::getUserInfoFromUri(ST_SIP_REQ_URI stReqUri)
{
    std::string strUsername = "", strTel = "";
    ST_SIP_TEL_URI stTelUri;

    switch(stReqUri.ucType)
    {
    case EN_SIP_REQ_URI_SIP:
        if (stReqUri.u.stSipUri.ucUserInfoPres)
        {
            SipMsgCommon::zosSstr2Str(stReqUri.u.stSipUri.stUserInfo.stUser,strUsername);
        }
        break;

    case EN_SIP_REQ_URI_SIPS:
        if (stReqUri.u.stSipsUri.ucUserInfoPres)
        {
            SipMsgCommon::zosSstr2Str(stReqUri.u.stSipsUri.stUserInfo.stUser,strUsername);
        }
        break;

    case EN_SIP_REQ_URI_IM:
        if (stReqUri.u.stImUri.ucDescPres)
        {
            SipMsgCommon::zosSstr2Str(stReqUri.u.stImUri.stDesc,strUsername);
        }
        break;

    case EN_SIP_REQ_URI_TEL:

        stTelUri = stReqUri.u.stTelUri;

        if (stTelUri.ucGlobalNumberPres)
        {
            SipMsgCommon::zosSstr2Str(stTelUri.u.stGlobal.stNumberDigits,strTel);
        }
        else
        {
            SipMsgCommon::zosSstr2Str(stTelUri.u.stLocal.stNumberDigits, strTel);
        }
        return std::string(stReqUri.u.stTelUri.u.stLocal.stNumberDigits.pcStr);

    default:
        return "";

    }
    return strUsername;
}

/* get hostname from ST_SIP_REQ_URI */
std::string SipMsgDecode::getHostnameFromUri(ST_SIP_REQ_URI stReqUri)
{
    ZCHAR acRmtAddr[ZINET_IPV4_STR_SIZE] = {0};
    switch(stReqUri.ucType)
    {
    case EN_SIP_REQ_URI_SIP:
        switch(stReqUri.u.stSipUri.stHostPort.stHost.ucType)
        {
        case 0 : //strName
            return std::string(stReqUri.u.stSipUri.stHostPort.stHost.u.stName.pcStr);
            break;

        case 1 : //ipv4
            //ZCHAR * pStrIpv4;
            Zos_InetNtop(ZINET_IPV4, &stReqUri.u.stSipUri.stHostPort.stHost.u.iIpv4Addr, acRmtAddr, ZINET_IPV4_STR_SIZE);
            //Zos_InetNtoa(stReqUri.u.stSipUri.stHostPort.stHost.u.iIpv4Addr, &pStrIpv4);
            return std::string(acRmtAddr);
            break;
        case 2://ipv6

            //ipv6
            ZCHAR TEMiPV6[ZINET_IPV6_ADDR_SIZE + 1];
            Zos_InetNtop(ZINET_IPV6, stReqUri.u.stSipUri.stHostPort.stHost.u.aucIpv6Addr, TEMiPV6, ZINET_IPV6_ADDR_SIZE + 1);
            std::string strHostname(TEMiPV6);
            return strHostname;
            break;

        }
        return " ";
    }
    return "";
}

/* get port from ST_SIP_REQ_URI */
int SipMsgDecode::getPortFromUri(ST_SIP_REQ_URI stReqUri)
{
    switch(stReqUri.ucType)
    {
    case EN_SIP_REQ_URI_SIPS:
    case EN_SIP_REQ_URI_SIP:
        return stReqUri.u.stSipUri.stHostPort.iPort;
    }
    return 0;
}


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
bool SipMsgDecode::getFromToAddrSpec(ZBOOL bFromHdr, ST_SIP_ADDR_SPEC & stAddrSpec)
{
    ZINT iResult = 0;
    ST_SIP_ADDR_SPEC * pAddSpec;
    iResult = Sip_MsgGetFromToAddrSpec(_pstMsg,bFromHdr, &pAddSpec);
    if (iResult == ZOK)
    {
        stAddrSpec = *pAddSpec;
        return true;
    }

    return false;
}

/* get uri of via header*/
bool SipMsgDecode::getHdrViaUri(std::string& strIpv4,  int & dwPort)
{
    ST_SIP_HDR_VIA *pstVia = NULL;
    ST_SIP_VIA_PARM * pPram = NULL;
    ZCHAR * pstrIpv4 = NULL;
    ZCHAR acRmtAddr[ZINET_IPV4_STR_SIZE] = {0};
    ST_SIP_HOST  stSipHost;

    /* find the via header */
    pstVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (!pstVia)
        return false;
    pPram = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstVia->stParmLst);
    if (!pPram)
        return false;

    stSipHost = pPram->stBy.stHost;
    dwPort = pPram->stBy.iPort;
    if (stSipHost.ucType == 0)
    {
        //stName
        SipMsgCommon::zosSstr2Str(stSipHost.u.stName, strIpv4);

    }
    else if (stSipHost.ucType == 1)
    {
        //iIpv4Addr
        Zos_InetNtop(ZINET_IPV4, &stSipHost.u.iIpv4Addr, acRmtAddr, ZINET_IPV4_STR_SIZE);
        //Zos_InetNtoa(stSipHost.u.iIpv4Addr, &pstrIpv4);
        strIpv4 = acRmtAddr;
    }
    else
    {
        //ipv6
        ZCHAR TEMiPV6[ZINET_IPV6_ADDR_SIZE + 1];
        Zos_InetNtop(ZINET_IPV6, stSipHost.u.aucIpv6Addr, TEMiPV6, ZINET_IPV6_ADDR_SIZE + 1);

        strIpv4 = TEMiPV6;

    }
    return true;
}

/**
* @brief SIP get the uri of via header
*/
bool SipMsgDecode::getHdrFromToUserInfo(bool bFrom,ST_ZOS_SSTR & stUserInfo)
{
    ST_SIP_ADDR_SPEC stAddrSpec;
    ST_SIP_SIP_URI stSipUri;

    if (getFromToAddrSpec(bFrom,  stAddrSpec))
    {
        stSipUri = stAddrSpec.u.stSipUri;
        if (stSipUri.ucUserInfoPres)
        {
            stUserInfo = stSipUri.stUserInfo.stUser;
        }
    }
    else
    {
        return false;
    }
    return true;

}

bool SipMsgDecode::getHdrFromToUserInfo(bool bFrom , std::string & strUserInfo)
{
    ST_ZOS_SSTR  stUserInfo;
    if (!getHdrFromToUserInfo(bFrom, stUserInfo))
    {
        return false;
    }
    SipMsgCommon::zosSstr2Str(stUserInfo, strUserInfo);
    return true;
}

/**
* @brief SIP get the uri of Contact header
*/
bool SipMsgDecode::getHdrContactUri(std::string& strIpv4,  int & iPort)
{
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ST_SIP_HOST stSipHost;
    ZCHAR * pstrIpv4 = NULL;
    ZCHAR acRmtAddr[ZINET_IPV4_STR_SIZE] = {0};

    if(Sip_MsgGetContactAddrSpec(_pstMsg, &pstAddrSpec) != ZOK)
    {
        return false;
    }
    iPort = pstAddrSpec->u.stSipUri.stHostPort.iPort;
    stSipHost = pstAddrSpec->u.stSipUri.stHostPort.stHost;
    if (stSipHost.ucType == 0)
    {
        //stName
        SipMsgCommon::zosSstr2Str(stSipHost.u.stName, strIpv4);
    }
    else if (stSipHost.ucType == 1)
    {
        //iIpv4Addr
        Zos_InetNtop(ZINET_IPV4, &stSipHost.u.iIpv4Addr, acRmtAddr, ZINET_IPV4_STR_SIZE);
        //Zos_InetNtoa(stSipHost.u.iIpv4Addr, &pstrIpv4);
        strIpv4 = acRmtAddr;
    }
    else
    {
        //ipv6
        ZCHAR TEMiPV6[ZINET_IPV6_ADDR_SIZE + 1];
        Zos_InetNtop(ZINET_IPV6, stSipHost.u.aucIpv6Addr, TEMiPV6, ZINET_IPV6_ADDR_SIZE + 1);

        strIpv4 = TEMiPV6;
    }
    return true;
}


bool SipMsgDecode::getHdrContactSipUri(ST_SIP_SIP_URI & stContactSipUri, std::string & strNumber, EN_SIP_ADDR_SPEC & stAddrType)
{
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ZCHAR * pstrIpv4 = NULL;

    if(Sip_MsgGetContactAddrSpec(_pstMsg, &pstAddrSpec) != ZOK)
    {
        return false;
    }
    stAddrType = (EN_SIP_ADDR_SPEC)pstAddrSpec->ucType;
    if (stAddrType == EN_SIP_ADDR_SPEC_SIP || stAddrType == EN_SIP_ADDR_SPEC_SIPS)
    {
        stContactSipUri = pstAddrSpec->u.stSipUri;
    }
    else if (stAddrType == EN_SIP_ADDR_SPEC_TEL)
    {
        ST_SIP_TEL_URI stTel;
        stTel = pstAddrSpec->u.stTelUri;
        if (stTel.ucGlobalNumberPres)
        {
            SipMsgCommon::zosSstr2Str(stTel.u.stGlobal.stNumberDigits, strNumber);
            if (strNumber.find("+") < 0)
                strNumber = "+" + strNumber;
        }
        else
        {
            SipMsgCommon::zosSstr2Str(stTel.u.stLocal.stNumberDigits, strNumber);
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool SipMsgDecode::getHdrContactSipUriParams(vector<ST_SIP_URI_PARM> & params)
{
    ST_SIP_SIP_URI stContactSipUri;
    ST_SIP_URI_PARM *pstUriParm = 0;
    void *pstUriParmVoidPtr = 0;
    ST_ABNF_LIST_NODE *pstNode = 0;
    std::string strNumber;
    EN_SIP_ADDR_SPEC stAddrType = EN_SIP_ADDR_SPEC_SIP;

    if (!getHdrContactSipUri(stContactSipUri, strNumber, stAddrType))
        return false;

    FOR_ALL_DATA_IN_ABNF_LIST(&stContactSipUri.stUriParmLst, pstNode, pstUriParmVoidPtr)
    {
        pstUriParm = (ST_SIP_URI_PARM *)pstUriParmVoidPtr;
        if (pstUriParm == 0)
            return false;
        params.push_back(*pstUriParm);
    }
    return true;
}

bool SipMsgDecode::getSipUriParams(const ST_SIP_SIP_URI & stSipUri, ST_SIP_URI_PARM_LST & params)
{
    params = stSipUri.stUriParmLst;
    return true;
}


bool SipMsgDecode::getHdrContactExpires(int & iExpires)
{
    ST_SIP_CONTACT_PARM *pstParm = 0;
    ZUINT lExpires = 0;

    Sip_MsgGetContactParm(_pstMsg, &pstParm);
    Sip_ParmPickContactExpires(pstParm, &lExpires);
    iExpires = lExpires;

    return true;
}

bool SipMsgDecode::getHdrContactSipUri(std::string & strUserName, std::string & strHostName,int & iPort, ZCHAR & zcTpt)
{
    ST_SIP_SIP_URI stContactSipUri;
    int dwPort = 0;
    ZUCHAR ucTpt = 0;
    std::string  strNumber;

    EN_SIP_ADDR_SPEC  stAddrType = EN_SIP_ADDR_SPEC_SIP;
    if (!getHdrContactSipUri( stContactSipUri, strNumber, stAddrType))
        return false;
    if (stContactSipUri.ucUserInfoPres)
    {
        SipMsgCommon::zosSstr2Str(stContactSipUri.stUserInfo.stUser, strUserName);
    }

    if (!getHdrContactUri(strHostName,  dwPort))
        return false;

    iPort = dwPort;
    pickSipUriTpt(&stContactSipUri, &ucTpt);
    zcTpt = ucTpt;
    return true;
}

bool SipMsgDecode::getHdrContactSipUri(std::string & strUserName, std::string & strHostName,int & iPort, ZCHAR & zcTpt, unsigned char & type)
{
    ST_SIP_SIP_URI stContactSipUri;
    int dwPort = 0;
    ZUCHAR ucTpt = 0;
    std::string  strNumber;

    EN_SIP_ADDR_SPEC stAddrType = EN_SIP_ADDR_SPEC_SIP;
    if (!getHdrContactSipUri( stContactSipUri, strNumber, stAddrType))
        return false;
    if (stContactSipUri.ucUserInfoPres)
    {
        SipMsgCommon::zosSstr2Str(stContactSipUri.stUserInfo.stUser, strUserName);
    }

    if (!getHdrContactUri(strHostName, dwPort))
        return false;

    iPort = dwPort;
    type = EN_SIP_ADDR_SPEC_SIP;
    pickSipUriTpt(&stContactSipUri, &ucTpt);
    zcTpt = ucTpt;
    return true;
}

bool SipMsgDecode::getHdrContactSipUri(ST_SIP_SIP_URI & stSipUri)
{
    ST_SIP_NA_SPEC stNaSpec;
    ST_SIP_ADDR_SPEC addrSpec;
    if (!getContactNaSpec(stNaSpec))
        return false;

    if (stNaSpec.ucNameAddrPres)
        addrSpec = stNaSpec.u.stNameAddr.stAddrSpec;
    else
        addrSpec = stNaSpec.u.stAddrSpec;

    if (addrSpec.ucType != 0)
        return false;

    stSipUri = addrSpec.u.stSipUri;
    return true;
}

bool  SipMsgDecode::getContactUri(std::string & strUri)
{
    ST_SIP_ADDR_SPEC  stAddrSpec;
    std::string uri;
    bool result = getContactAddrSpec( stAddrSpec);
    if (!result)
        return false;

    result = getUriByAddrSpec(&stAddrSpec, uri);
    strUri = uri;
    return result;
}


bool SipMsgDecode::getHdrSubsSta(  ZUCHAR *pucStaType,std::string & strTypeName,	ZUINT *piExpires)
{
    ZUCHAR chTem = 0;
    if (Sip_MsgGetSubsSta(_pstMsg,  &chTem, piExpires) == ZOK)
    {
        *pucStaType = chTem;
        switch(chTem)
        {
        case 0:
            strTypeName = "active";
            break;
        case 1:
            strTypeName = "pending";
            break;
        case 2:
            strTypeName = "terminated";
            break;

        default:
            strTypeName = "otherwise";
        }
        return true;
    }
    return false;
}

bool SipMsgDecode::getHdrEvent(ZUCHAR & pucPkgType,std::string & strPkgTypeName,ZUCHAR & pucTempType, std::string & strTmpTypeName)
{
    if (Sip_MsgGetEvnt(_pstMsg, &pucPkgType, &pucTempType) == ZOK)
    {
        switch(pucPkgType)
        {
        case EN_SIP_EVNT_PKG_PRESENCE:
            strPkgTypeName = "presence";
            break;

        case EN_SIP_EVNT_PKG_REFER:
            strPkgTypeName = "refer";
            break;

        case EN_SIP_EVNT_PKG_POC_SET:
            strPkgTypeName = "poc-settings";
            break;

        case EN_SIP_EVNT_PKG_DLG:
            strPkgTypeName = "dialog";
            break;

        case EN_SIP_EVNT_PKG_KPML:
            strPkgTypeName = "kpml";
            break;

        case EN_SIP_EVNT_PKG_MSG_SUMMARY:
            strPkgTypeName = "message-summary";
            break;

        case EN_SIP_EVNT_PKG_REG:
            strPkgTypeName = "reg";
            break;

        case EN_SIP_EVNT_PKG_UA_PROF:
            strPkgTypeName = "ua-profile";
            break;

        case EN_SIP_EVNT_PKG_XCAP_DIFF:
            strPkgTypeName = "xcap-diff";
            break;

        case EN_SIP_EVNT_PKG_DMSG:
            strPkgTypeName = "deferred-messages";
            break;

        default:
            strPkgTypeName = "otherwise";
            break;
        }

        switch(pucTempType)
        {

        case EN_SIP_EVNT_TEMP_WINFO:
            strTmpTypeName = "winfo";
            break;
        default:
            strTmpTypeName = "otherwise";
            break;

        }
        return true;
    }
    return false;
}

bool  SipMsgDecode::getEventId(std::string & eventId)
{
    ST_SIP_HDR_EVNT *pstEvnt;
    ST_ZOS_SSTR * pstId;
    /* get event header */
    pstEvnt = (ST_SIP_HDR_EVNT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_EVNT);
    if (pstEvnt)
    {
        /* get the id */
        if (Sip_ParmEvntLstGetId(&pstEvnt->stParmLst,&pstId) == ZOK)
        {
            SipMsgCommon::zosSstr2Str(*pstId,eventId);
            return true;
        }
    }
    return false;
}


bool SipMsgDecode::pickSipUriTpt(ST_SIP_SIP_URI *pstSipUri, ZUCHAR *pucTpt)
{
    if (Sip_ParmPickSipUriTpt(pstSipUri, pucTpt) != ZOK)
        return false;
    return true;
}

bool SipMsgDecode::getHdrRecordRouteSipUri(bool bIsRecordRoute, ST_SIP_SIP_URI & stSipUri)
{
    ST_SIP_HDR_ROUTE *pstRoute = NULL;
    ST_SIP_ADDR_SPEC *pstAddrSpec = NULL;

    if (bIsRecordRoute)
    {
        pstRoute = (ST_SIP_HDR_ROUTE *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_RECORD_ROUTE);
    }
    else
    {
        pstRoute = (ST_SIP_HDR_ROUTE *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    }

    if (!pstRoute)
        return false;

    if (Sip_HdrRouteGetAddrSpec( pstRoute,&pstAddrSpec) != ZOK)
        return false;

    if (pstAddrSpec->ucType == EN_SIP_ADDR_SPEC_SIP)
    {
        stSipUri = pstAddrSpec->u.stSipUri;
        return true;
    }
    else if (pstAddrSpec->ucType == EN_SIP_ADDR_SPEC_SIPS)
    {
        stSipUri = pstAddrSpec->u.stSipsUri;
        return true;
    }

    return false;
}

bool SipMsgDecode::getHdrRecordRouteParams(vector<ST_SIP_ROUTE_PARM> & stRouteParams)
{
    ST_SIP_HDR_ROUTE *pstRoute = NULL;
    ST_SIP_ADDR_SPEC *pstAddrSpec = NULL;

    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_RECORD_ROUTE);
    if (!pstRoute)
        return false;

    ST_ABNF_LIST_NODE * pstHdrNode = 0;
    ZVOID *pstRouteParam = 0;
    ST_SIP_MSG_HDR * pstHdr = 0;
    ZVOID *pstVidHdrPtr = 0;
    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;

        if (pstHdr->ucType != EN_SIP_HDR_RECORD_ROUTE)
            continue;

        ST_ABNF_LIST_NODE * pstHdrNode2 = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdr->u.pstRecordRoute->stParmLst, pstHdrNode2, pstRouteParam)
        {
            ST_SIP_ROUTE_PARM * pstParam = (ST_SIP_ROUTE_PARM *)pstRouteParam;
            if (pstParam == 0)
                continue;

            stRouteParams.push_back(*pstParam);
        }
    }

    return true;
}

bool SipMsgDecode::getHdrRecordRouteSipUris(vector<ST_SIP_SIP_URI> & stSipUri)
{
    ST_SIP_HDR_ROUTE *pstRoute = NULL;
    ST_SIP_ADDR_SPEC *pstAddrSpec = NULL;

    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_RECORD_ROUTE);
    if (!pstRoute)
        return false;

    ST_ABNF_LIST_NODE * pstHdrNode = 0;
    ZVOID *pstRouteParam = 0;
    ST_SIP_MSG_HDR * pstHdr = 0;
    ZVOID *pstVidHdrPtr = 0;
    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;
        if (pstHdr->ucType != EN_SIP_HDR_RECORD_ROUTE)
            continue;
        ST_ABNF_LIST_NODE * pstHdrNode2 = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdr->u.pstRecordRoute->stParmLst, pstHdrNode2, pstRouteParam)
        {
            ST_SIP_ROUTE_PARM * pstParam = (ST_SIP_ROUTE_PARM *)pstRouteParam;
            if (pstParam == 0)
                continue;

            ST_SIP_ADDR_SPEC addr = pstParam->stNameAddr.stAddrSpec;
            if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                stSipUri.push_back(addr.u.stSipUri);
            else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                stSipUri.push_back(addr.u.stSipsUri);
        }
    }

    return true;
}

bool SipMsgDecode::getHdrRecordRouteUri(bool bIsRecordRoute,std::string & strUserName, std::string & strHostName, int & dwPort)
{
    ST_SIP_SIP_URI stSipUri;
    std::string strPassWd;
    int iPort = 0;
    if (!getHdrRecordRouteSipUri(bIsRecordRoute,stSipUri))
        return false;

    if (!SipMsgCommon::deSipUri(&stSipUri,strUserName, strPassWd,strHostName, iPort))
        return false;

    dwPort = iPort;
    return true;
}

bool SipMsgDecode::getHdrRecordRouteUri(bool bIsRecordRoute, std::string & strUserName, std::string & strHostName, int & dwPort, EN_SIP_URI_PARM & uriParamType,
                                        unsigned char & value1, unsigned int & value2, std::string & value3)
{
    ST_SIP_SIP_URI stSipUri;
    ST_SIP_URI_PARM *pstUriParm;
    std::string strPassWd;
    int iPort = 0;
    ST_ZOS_SSTR * pstAuid = 0;
    ZBOOL bLr = 0;
    unsigned char ucTpt = 0;
    ZUINT ttl = 0;

    if (!getHdrRecordRouteSipUri(bIsRecordRoute,stSipUri))
        return false;

    if (!SipMsgCommon::deSipUri(&stSipUri,strUserName, strPassWd,strHostName, iPort))
        return false;

    dwPort = iPort;

    //Lr type
    if (Sip_ParmUriLstFind(&stSipUri.stUriParmLst, EN_SIP_URI_PARM_LR, &pstUriParm) == ZOK)
    {
        uriParamType = EN_SIP_URI_PARM_LR;
        return true;
    }

    if (ZOK == Sip_ParmPickSipUriAuid(&stSipUri,&pstAuid))
    {
        uriParamType = EN_SIP_URI_PARM_AUID;
        SipMsgCommon::zosSstr2Str(*pstAuid, value3);
        return true;

    }
    else if (ZOK == Sip_ParmPickSipUriTpt(&stSipUri,  &ucTpt))
    {
        uriParamType = EN_SIP_URI_PARM_TPT;
        value1 = ucTpt;
        return true;
    }
    else if(ZOK == Sip_ParmPickSipUriTtl(&stSipUri, &ttl))
    {
        uriParamType = EN_SIP_URI_PARM_TTL;
        value2 = ttl;
        return true;
    }
    return false;
}

/* fill header Record-Route */
bool SipMsgDecode::fillHdrRecordRoute(const std::string &strUserName, const std::string &strHost,
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

bool SipMsgDecode::fillHdrRecordRouteBySipUri(const ST_SIP_SIP_URI & stSipUri)
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
bool SipMsgDecode::fillHdrRecordRoute(ST_SIP_ROUTE_PARM & stRouteParams)
{
    ST_SIP_HDR_ROUTE *pstRoute;
    ST_SIP_ROUTE_PARM *pstRouteParm;

    /* create header record-route */
    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_RECORD_ROUTE);
    if(!pstRoute)
    {
        TLOGWARN("[SipMsgDecode] Sip_CreateMsgHdr error" << endl);
        return false;
    }

    /* add the parameter */
    Sip_ParmRouteLstAdd(_pstMsg->zMemBuf, &pstRoute->stParmLst, &pstRouteParm);
    if(!pstRouteParm)
    {
        TLOGWARN("[SipMsgDecode] Sip_ParmRouteLstAdd error" << endl);
        return false;
    }

    if(Sip_CpyRouteParm(_pstMsg->zMemBuf, pstRouteParm, &stRouteParams)!= ZOK)
    {
        TLOGWARN("[SipMsgDecode] Sip_CpyRouteParm error" << endl);
        return false;
    }

    return true;
}


/* fill header Record-Route */
bool SipMsgDecode::fillHdrRoute(const std::string &strUserName, const std::string &strHost,
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

/* fill header Record-Route */
bool SipMsgDecode::fillHdrRoute(ST_SIP_ROUTE_PARM & stRouteParams)
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

bool SipMsgDecode::getHdrRouteParams(vector<ST_SIP_ROUTE_PARM> & stRouteParams)
{
    ST_SIP_HDR_ROUTE *pstRoute = NULL;
    ST_SIP_ADDR_SPEC *pstAddrSpec = NULL;

    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    if (!pstRoute)
        return false;

    ST_ABNF_LIST_NODE * pstHdrNode = 0;
    ZVOID *pstRouteParam = 0;
    ST_SIP_MSG_HDR * pstHdr = 0;
    ZVOID *pstVidHdrPtr = 0;
    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;

        if (pstHdr->ucType != EN_SIP_HDR_ROUTE)
            continue;

        ST_ABNF_LIST_NODE * pstHdrNode2 = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdr->u.pstRecordRoute->stParmLst, pstHdrNode2, pstRouteParam)
        {
            ST_SIP_ROUTE_PARM * pstParam = (ST_SIP_ROUTE_PARM *)pstRouteParam;
            if (pstParam == 0)
                continue;

            stRouteParams.push_back(*pstParam);
        }
    }

    return true;
}

bool SipMsgDecode::getHdrRouteSipUris(vector<ST_SIP_SIP_URI> & stSipUri)
{
    ST_SIP_HDR_ROUTE *pstRoute = NULL;
    ST_SIP_ADDR_SPEC *pstAddrSpec = NULL;

    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    if (!pstRoute)
        return false;

    ST_ABNF_LIST_NODE * pstHdrNode = 0;
    ZVOID *pstRouteParam = 0;
    ST_SIP_MSG_HDR * pstHdr = 0;
    ZVOID *pstVidHdrPtr = 0;
    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;
        if (pstHdr->ucType != EN_SIP_HDR_ROUTE)
            continue;

        ST_ABNF_LIST_NODE * pstHdrNode2 = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdr->u.pstRecordRoute->stParmLst, pstHdrNode2, pstRouteParam)
        {
            ST_SIP_ROUTE_PARM * pstParam = (ST_SIP_ROUTE_PARM *)pstRouteParam;
            if (pstParam == 0)
                continue;

            ST_SIP_ADDR_SPEC addr = pstParam->stNameAddr.stAddrSpec;
            if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
                stSipUri.push_back(addr.u.stSipUri);
            else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
                stSipUri.push_back(addr.u.stSipsUri);
        }
    }

    return true;
}

bool SipMsgDecode::delHdrRoute(const std::string & ip, const std::string & ip2, const std::string & ip3)
{
    ST_SIP_HDR_ROUTE *pstRoute = NULL;
    ST_SIP_ADDR_SPEC *pstAddrSpec = NULL;

    pstRoute = (ST_SIP_HDR_ROUTE *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_ROUTE);
    if (!pstRoute)
        return false;

    ST_ABNF_LIST_NODE * pstHdrNode = 0;
    ZVOID *pstRouteParam = 0, *pstRouteParam2 = 0;
    ST_SIP_MSG_HDR * pstHdr = 0;
    ZVOID *pstVidHdrPtr = 0;
    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;
        if (pstHdr->ucType != EN_SIP_HDR_ROUTE)
            continue;

        int count = 0;
        ST_ABNF_LIST_NODE * pstHdrNode3 = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdr->u.pstRoute->stParmLst, pstHdrNode3, pstRouteParam2)
        {
            ST_SIP_ROUTE_PARM * pstParam = (ST_SIP_ROUTE_PARM *)pstRouteParam2;
            if (pstParam == 0)
                continue;

            count++;
        }

        ST_ABNF_LIST_NODE * pstHdrNode2 = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdr->u.pstRoute->stParmLst, pstHdrNode2, pstRouteParam)
        {
            ST_SIP_ROUTE_PARM * pstParam = (ST_SIP_ROUTE_PARM *)pstRouteParam;
            if (pstParam == 0)
                continue;

            ST_SIP_ADDR_SPEC addr = pstParam->stNameAddr.stAddrSpec;
            std::string userInfo, pwd, host;
            int port = 0;

            if (addr.ucType == EN_SIP_ADDR_SPEC_SIP)
            {
                SipMsgCommon::deSipUri(&addr.u.stSipUri, userInfo, pwd, host, port);
            }
            else if(addr.ucType == EN_SIP_ADDR_SPEC_SIPS)
            {
                SipMsgCommon::deSipUri(&addr.u.stSipsUri, userInfo, pwd, host, port);
            }

            if (host == ip || host == ip2 || host == ip3)
            {
                if (count > 1)
                    ABNF_LIST_REMOVE_DATA(&pstHdr->u.pstRoute->stParmLst, pstParam);
                else
                    ABNF_LIST_REMOVE_DATA(&_pstMsg->stHdrLst, pstHdr);

                return true;
            }
        }
    }

    return false;
}

bool SipMsgDecode::getHdrSupported(vector<int> &vectType, vector<std::string> & vectTypeName)
{
    ST_SIP_HDR_SUPTED * pstHdrSupted = NULL;
    void * pTem = NULL;
    ST_SIP_OPT_TAG * pTag = NULL;
    ST_ABNF_LIST_NODE * pNode = NULL;

    pstHdrSupted = (ST_SIP_HDR_SUPTED *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_SUPTED);
    if (!pstHdrSupted)
        return false;
    FOR_ALL_DATA_IN_ABNF_LIST(&pstHdrSupted->stOptTagLst, pNode, pTem)
    {
        pTag = (ST_SIP_OPT_TAG *)pTem;
        vectType.push_back(pTag->ucType);
        std::string  strName;
        SipMsgCommon::OptTagEnumToName(pTag->ucType, strName);
        vectTypeName.push_back(strName);
    }
    return true;
}

bool SipMsgDecode::getHdrPAssertdedID(std::string & strDispInfo, std::string & strHostName, int & dwPort)
{
    ST_SIP_HDR_P_ASSERTED_ID *pstHdrPAsseted = NULL;

    /* create P-Asserted-Identity header */
    pstHdrPAsseted = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
    if (!pstHdrPAsseted)
        return false;

    ST_SIP_PASSERTEDID_VAL *pstAssetedVal = NULL;
    ST_ABNF_LIST_NODE * pNode = NULL;
    void * pTem = NULL;
    ZUCHAR ucAddrSpecType;

    FOR_ALL_DATA_IN_ABNF_LIST(&pstHdrPAsseted->stValLst, pNode, pTem)
    {
        pstAssetedVal = (ST_SIP_PASSERTEDID_VAL *)pTem;
        if (!pstAssetedVal)
            return false;

        ucAddrSpecType = pstAssetedVal->stNaSpec.u.stAddrSpec.ucType;
        switch(ucAddrSpecType)
        {
            case EN_SIP_ADDR_SPEC_SIP:
            case EN_SIP_ADDR_SPEC_SIPS:
                getHdrPAssetedIDBySipUri(pstAssetedVal, strDispInfo, strHostName, dwPort);
                break;

            case EN_SIP_ADDR_SPEC_TEL:
                getHdrPAssertedIDByTel(pstAssetedVal, strDispInfo);
                strHostName = "";
                break;
        }
    }

    return true;
}

bool SipMsgDecode::getHdrPAssetedIDBySipUri(const ST_SIP_PASSERTEDID_VAL *pstAssetedVal, std::string & strUserInfo, std::string & strHostName, int & dwPort)
{
    ZUCHAR ucSipType;
    ST_SIP_SIP_URI stSipUri;
    int iPort = 0;

    if (!pstAssetedVal)
        return false;

    ST_SIP_ADDR_SPEC stAddrSpec;
    if(pstAssetedVal->stNaSpec.ucNameAddrPres)
    {
        stAddrSpec = pstAssetedVal->stNaSpec.u.stNameAddr.stAddrSpec;
    }
    else
    {
        stAddrSpec = pstAssetedVal->stNaSpec.u.stAddrSpec;
    }

    ucSipType = stAddrSpec.ucType;
    if (ucSipType == EN_SIP_ADDR_SPEC_SIP)
    {
        stSipUri = stAddrSpec.u.stSipUri;
    }
    else if (ucSipType == EN_SIP_ADDR_SPEC_SIPS)
    {
        stSipUri = stAddrSpec.u.stSipsUri;
    }
    else
    {
        return false;
    }

    if (stSipUri.ucUserInfoPres)
    {
        SipMsgCommon::zosSstr2Str(stSipUri.stUserInfo.stUser,strUserInfo);
    }

    SipMsgCommon::deHostPort(&stSipUri.stHostPort,strHostName,iPort);
    dwPort = iPort;
    return true;
}

bool SipMsgDecode::getHdrPAssertedIDByTel(const ST_SIP_PASSERTEDID_VAL *pstAssetedVal, std::string & strTel)
{
    ST_ZOS_SSTR stNumber;
    std::string strTem;

    if (!pstAssetedVal)
        return false;


    ST_SIP_ADDR_SPEC stAddrSpec;
    if(pstAssetedVal->stNaSpec.ucNameAddrPres)
    {
        stAddrSpec = pstAssetedVal->stNaSpec.u.stNameAddr.stAddrSpec;
    }
    else
    {
        stAddrSpec = pstAssetedVal->stNaSpec.u.stAddrSpec;
    }

    ST_SIP_TEL_URI * pstTelUri = (ST_SIP_TEL_URI *)& stAddrSpec.u.stTelUri;
    if (stAddrSpec.ucType == EN_SIP_ADDR_SPEC_TEL)
    {
        stNumber = pstTelUri->ucGlobalNumberPres ? pstTelUri->u.stGlobal.stNumberDigits : pstTelUri->u.stLocal.stNumberDigits;
    }
    SipMsgCommon::zosSstr2Str(stNumber,strTel);

    return true;
}

bool SipMsgDecode::getHdrPCalledPartyID(std::string & strDispInfo, std::string & strHostName, int & dwPort)
{
    ST_SIP_HDR_P_CALLED_PTY_ID *pstHdrPCalled;
    std::string strTem;
    ZUCHAR ucAddrSpecType;

    /* create P-Preferred-Identity header */
    pstHdrPCalled = (ST_SIP_HDR_P_CALLED_PTY_ID *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_P_CALLED_PTY_ID);
    if (!pstHdrPCalled)
        return false;

    ucAddrSpecType = pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.ucType;
    switch(ucAddrSpecType)
    {
    case EN_SIP_ADDR_SPEC_SIP:
    case EN_SIP_ADDR_SPEC_SIPS:
        getHdrPCalledPartyIDBySipUri(pstHdrPCalled, strDispInfo, strHostName, dwPort);
        break;

    case EN_SIP_ADDR_SPEC_TEL:

        getHdrPCalledPartyIDByTel(pstHdrPCalled, strDispInfo);
        strHostName = "";
        break;
    }
    return true;
}

bool SipMsgDecode::getHdrPCalledPartyIDByTel(const ST_SIP_HDR_P_CALLED_PTY_ID *pstHdrPCalled, std::string & strTel)
{
    ST_ZOS_SSTR stNumber;
    std::string strTem;

    if (!pstHdrPCalled)
        return false;

    ST_SIP_TEL_URI * pstTelUri = (ST_SIP_TEL_URI *)& pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stTelUri;
    if (pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.ucType == EN_SIP_ADDR_SPEC_TEL)
    {
        stNumber = pstTelUri->ucGlobalNumberPres ? pstTelUri->u.stGlobal.stNumberDigits : pstTelUri->u.stLocal.stNumberDigits;

    }
    SipMsgCommon::zosSstr2Str(stNumber,strTel);
    return true;
}

bool SipMsgDecode::getHdrPCalledPartyIDBySipUri(const ST_SIP_HDR_P_CALLED_PTY_ID *pstHdrPCalled, std::string & strUserInfo, std::string & strHostName, int & dwPort)
{
    ZUCHAR ucSipType;
    ST_SIP_SIP_URI stSipUri;
    int iPort = 0;

    if (!pstHdrPCalled)
        return false;

    ucSipType = pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.ucType;
    if (ucSipType == EN_SIP_ADDR_SPEC_SIP)
    {
        stSipUri = pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stSipUri;
    }
    else if (ucSipType == EN_SIP_ADDR_SPEC_SIPS)
    {
        stSipUri = pstHdrPCalled->stPtyIdSpec.stNameAddr.stAddrSpec.u.stSipsUri;
    }
    else
    {
        return false;
    }

    if (stSipUri.ucUserInfoPres)
    {
        SipMsgCommon::zosSstr2Str(stSipUri.stUserInfo.stUser,strUserInfo);
    }

    //dwPort = stSipUri.stHostPort.ucPortPres ?  stSipUri.stHostPort.dwPort : 0;
    SipMsgCommon::deHostPort(&stSipUri.stHostPort,strHostName,iPort);
    dwPort = iPort;
    return true;
}

bool SipMsgDecode::getHdrConversationID(std::string & strConverstionId)
{
    ST_ZOS_SSTR stConvId;
    if (Sip_MsgGetConvId(_pstMsg, &stConvId) != ZOK)
        return false;
    SipMsgCommon::zosSstr2Str(stConvId,strConverstionId);
    return true;
}

bool SipMsgDecode::getHdrContributionID(std::string & strContributionId)
{
    ST_ZOS_SSTR stConvId;
    if (Sip_MsgGetContId(_pstMsg, &stConvId) != ZOK)
        return false;
    SipMsgCommon::zosSstr2Str(stConvId,strContributionId);
    return true;
}

bool SipMsgDecode::getHdrAllow(vector<int> &vectType, vector<std::string> & vectTypeName)
{
    ST_SIP_HDR_ALLOW * pstHdrAllow = NULL;
    void * pTem = NULL;
    ST_SIP_METHOD * pTag = NULL;
    ST_ABNF_LIST_NODE * pNode = NULL;

    pstHdrAllow = (ST_SIP_HDR_ALLOW *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_ALLOW);
    if (!pstHdrAllow)
        return false;
    FOR_ALL_DATA_IN_ABNF_LIST(&pstHdrAllow->stMethodLst, pNode, pTem)
    {
        pTag = (ST_SIP_METHOD *)pTem;
        vectType.push_back(pTag->ucType);

        std::string  strName;
        SipMsgCommon::OptMethodEnumToName(pTag->ucType, strName);
        vectTypeName.push_back(strName);
    }

    return true;
}

bool  SipMsgDecode::decodeNotifyXmlBody( XmlReginfo & xmlReginfo)
{
    //ST_XML_MSG *pstXmlMsg;
    //ST_ZOS_LUSTR stMsgStr;

    //ST_XML_ELEM *pstRoot, *pstRegLst, *pstContact;
    //ZUCHAR ucInfoState, ucEventType;
    //ST_ZOS_USTR *pstData;
    //unsigned long iVersion = 0, expires =0;
    //std::string strTem;
    //
    ///* check body type */
    //if (_pstMsg->stBody.ucMSubtype != EN_SIP_MSUBTYPE_REGINFO_XML)
    //{
    //	return false;
    //}

    ////ZOS_USSTR_SETX(&stMsgStr, &_pstMsg->stBody.u.stSpart.u.stStr);
    //SipMsgCommon::ZOS_LUSTR_SET_LSTR(&stMsgStr,&_pstMsg->stBody.u.stSpart.u.stStr);
    ///* load data */
    //if (Eax_MsgLoadData(&stMsgStr, &pstXmlMsg) != ZOK)
    //{
    //	TLOGWARN("[SipProxy] SubsRdRegInfoBody load data.!" << endl);
    //	return false;
    //}


    ////* get root element <reginfo> */
    //if (EaReg_InfoGetInfo(pstXmlMsg, &pstRoot) != ZOK)
    //{
    //	TLOGWARN("[SipProxy] SubsRdRegInfoXml get root <reginfo>.!" << endl);
    //	return false;
    //}

    ////* get reginfo attribute state */
    //if (EaReg_InfoInfoGetSta(pstRoot, &ucInfoState) != ZOK)
    //{
    //	TLOGWARN("[SipProxy] EaReg_InfoInfoGetSta get  attribute state return FALSE!" << endl);
    //	return false;
    //}
    //
    ////* get attribute state */
    //if (EaReg_InfoInfoGetVer(pstRoot, &iVersion) != ZOK)
    //{
    //	TLOGWARN("[SipProxy] EaReg_InfoInfoGetVer get version return FALSE!" << endl);
    //	return false;
    //}



    //xmlReginfo._vectRegistration.clear();			//clear
    //xmlReginfo._state = ucInfoState;
    //xmlReginfo._version = iVersion;

    ////* get element <reg-list> */
    //for (EaReg_InfoInfoGetFirstReg(pstRoot, &pstRegLst);
    //	pstRegLst; EaReg_InfoInfoGetNextReg(pstRegLst, &pstRegLst))
    //{
    //	XmlRegistration xmlRegistration;

    //	/* get registration id */
    //	if (EaReg_InfoRegGetId(pstRegLst, &pstData) != ZOK)
    //	{
    //		TLOGWARN("[SipProxy] EaReg_InfoRegGetId get registration id  return FALSE!" << endl);
    //		return false;
    //	}
    //	SipMsgCommon::zosUstr2Str(*pstData,strTem);
    //	xmlRegistration._id = strTem;						//registration id
    //
    //	pstData = 0;
    //	/* get registration aor */
    //	if (EaReg_InfoRegGetAor(pstRegLst, &pstData) != ZOK)
    //	{
    //		TLOGWARN("[SipProxy] EaReg_InfoRegGetId get registration aor  return FALSE!" << endl);
    //		return false;
    //	}
    //	SipMsgCommon::zosUstr2Str(*pstData,strTem);
    //	xmlRegistration._aor = strTem;						//registration aor

    //	/* get registration state */
    //	if (EaReg_InfoRegGetSta(pstRegLst, &ucInfoState) != ZOK)
    //	{
    //		TLOGWARN("[SipProxy] EaReg_InfoRegGetId get registration state  return FALSE!" << endl);
    //		return false;
    //	}
    //	xmlRegistration._state = ucInfoState;				//registration state

    //

    //	/* get element <contact> */
    //	for (EaReg_InfoRegGetFirstContact(pstRegLst, &pstContact);
    //		pstContact; EaWat_InfoLstGetNextWat(pstContact, &pstContact))
    //	{
    //		XmlContact contact;
    //		/* get contact
    //		id */
    //		pstData = 0;
    //		if (EaReg_InfoContactGetId(pstContact, &pstData) != ZOK)
    //		{
    //			TLOGWARN("[SipProxy] EaReg_InfoContactGetId get  contact id.  return FALSE!" << endl);
    //			return false;
    //		}
    //		SipMsgCommon::zosUstr2Str(*pstData,strTem);
    //		contact._id = strTem;						//contact id
    //
    //		//uri
    //		pstData = 0;
    //		if (EaReg_InfoContactGetUri(pstContact, &pstData) != ZOK)
    //		{
    //			TLOGWARN("[SipProxy] EaReg_InfoContactGetUri get  contact uri.  return FALSE!" << endl);
    //			return false;
    //		}
    //		SipMsgCommon::zosUstr2Str(*pstData,strTem);
    //		contact._uri = strTem;

    //		if (EaReg_InfoContactGetExpires(pstContact, &expires) != ZOK)
    //		{
    //			TLOGWARN("SipProxy", "EaReg_InfoContactGetExpires get  contact expres.  return FALSE!");
    //			return false;
    //		}
    //		contact._expires = expires;

    //		/* get contact event */
    //		if (EaReg_InfoContactGetEvnt(pstContact, &ucEventType) == ZOK)
    //		{
    //			TLOGWARN("SipProxy", "EaReg_InfoContactGetEvnt get  contact expres.  return FALSE!");
    //			return false;
    //		}
    //		contact._event = ucEventType;

    //		xmlRegistration._vectContact.push_back(contact);
    //	}

    //	xmlReginfo._vectRegistration.push_back(xmlRegistration);

    //}

    return true;

}

bool  SipMsgDecode::getXmlBodyString(std::string & strXmlBody)
{
    /* check body type */
    if (_pstMsg->stBody.ucMSubtype != EN_SIP_MSUBTYPE_REGINFO_XML)
    {
        TLOGWARN("SipProxy", "getXmlBodyString() called. MSubtype != EN_SIP_MSUBTYPE_REGINFO_XML!");
        return false;
    }

    SipMsgCommon::zosSstr2Str(_pstMsg->stBody.u.stSpart.u.stStr, strXmlBody);
    return true;
}

bool  SipMsgDecode::decodeNotifyXmlBodyTinyXml(const std::string & strXmlBody, XmlReginfo & xmlReginfo)
{
    tinyxml2::XMLElement * reginfo = 0, *registration = 0, *contact, *uri = 0;
    tinyxml2::XMLDocument doc2;

    tinyxml2::XMLError err = doc2.Parse(strXmlBody.c_str(), strXmlBody.size());
    if (err != 0 )
    {
        int ll = 0;
    }
    reginfo = doc2.RootElement();
    if (reginfo == 0)
        return false;

    registration = reginfo->FirstChildElement();
    if(registration == 0)
        return false;

    std::string strVersion = reginfo->Attribute("version");
    std::string strState = reginfo->Attribute("state");

    xmlReginfo._version = atoi(strVersion.c_str());
    xmlReginfo._state = ReginfoStateConv(strState);

    while (registration)
    {
        //registration
        XmlRegistration xmlRegistation;
        std::string strAor = registration->Attribute("aor");
        std::string strId = registration->Attribute("id");
        std::string strState = registration->Attribute("state");

        xmlRegistation._aor = strAor;
        xmlRegistation._id = strId;
        xmlRegistation._state = RegistrationStateConv(strState);

        contact = registration->FirstChildElement();
        while (contact)
        {
            XmlContact xmlContactNode;
            std::string strId = contact->Attribute("id");
            std::string strState = contact->Attribute("state");

            xmlContactNode._id = strId;
            xmlContactNode._state = ContactStateConv(strState);
            uri = contact->FirstChildElement();
            if (uri)
            {
                std::string strUri = uri->GetText();
                xmlContactNode._uri = strUri;                                           //format like "sip:+8615888196659@10.188.2.3:5662"
                int pos1 = strUri.find("@");
                int pos2 = strUri.rfind(":");

                std::string temIp = strUri.substr(pos1 + 1, pos2 - pos1 -1);         //get th ip and port
                std::string temPort = strUri.substr(pos2 + 1, strUri.size() - pos2);
                int port = atoi(temPort.c_str());

                xmlContactNode._ip = temIp;
                xmlContactNode._port = port;
            }

            xmlRegistation._vectContact.push_back(xmlContactNode);
            contact = contact->NextSiblingElement();
        }

        xmlReginfo._vectRegistration.push_back(xmlRegistation);
        registration = registration->NextSiblingElement();
    }
    return true;
}

int SipMsgDecode::ReginfoStateConv(std::string stateIn)
{
    if ( stateIn == "full")
    {
        return 0;
    }
    else if (stateIn == "partial")
    {
        return 1;
    }
    else
    {
        return 999;
    }
}

int SipMsgDecode::RegistrationStateConv(std::string stateIn)
{
    if ( stateIn == "init")
    {
        return 0;
    }
    else if (stateIn == "active")
    {
        return 1;
    }
    else if (stateIn == "terminated")
    {
        return 2;
    }
    else
    {
        return 999;
    }
}

int SipMsgDecode::ContactStateConv(std::string stateIn)
{
    if (stateIn == "active")
    {
        return 0;
    }
    else if (stateIn == "terminated")
    {
        return 1;
    }
    else
    {
        return 999;
    }
}

bool SipMsgDecode::decodeChallengeDigest( ST_SIP_DIGEST stDigest, SipProxy::SipRegisterSubscribePtr pSipReg)
{
    std::string realm,nonce, opaque;
    unsigned char qopType, algoType;

    //get Realm
    if (getDigestRealm(&stDigest, realm))
        pSipReg->setDigestRealm(realm);
    else
        TLOGDEBUG("SipMsgDecode", "getDigestRealm() called. return 0!");

    //get nonce
    if (getDigestNonce(&stDigest,nonce))
    {
        std::string strOldNonce = pSipReg->getdigestNonce();
        pSipReg->setDigestNonce(nonce);
        if (strOldNonce.empty() == false && strOldNonce != nonce)
        {
            std::string strCNonce;
            SipMsgCommon::createDigestCnonce(strCNonce);
            pSipReg->setDigestNonceCount(1);            //内存中nonce count 设置为1，后续读取内存
            pSipReg->setDigestCNonce(strCNonce);
        }
    }
    else
    {
        TLOGWARN("SipMsgDecode", "getDigestNonce() called. return 0!");
    }

    //get opaque
    if (getDigestOpaque(&stDigest,opaque))
        pSipReg->setDigestOpaque(opaque);
    else
        TLOGDEBUG("SipMsgDecode", "getDigestOpaque() called. return 0!");

    //get algo
    if (getDigestAlgo(&stDigest,algoType))
        pSipReg->setDigestAlgo(algoType);
    else
        TLOGDEBUG("SipMsgDecode", "getDigestAlgo() called. return 0!");

    //get qopType
    if (getDigestQopX(&stDigest,&qopType))
        pSipReg->setDigestQopType(true, qopType);
    else
        TLOGWARN("SipMsgDecode", "getDigestQopX() called. return 0!");
    return true;
}

bool SipMsgDecode::getAuthenticationInfoNextNonce(std::string & nextNonce)
{
    ST_SIP_HDR_AUTHEN_INFO *pstAuthInfo = 0;
    ST_SIP_AINFO *pstAinfo;
    ST_ZOS_SSTR stNextNonce;

    /* try to find the authen info header */
    pstAuthInfo = (ST_SIP_HDR_AUTHEN_INFO *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_AUTHEN_INFO);
    if (pstAuthInfo == 0)
    {
        //TLOGWARN("SipProxy", "Can not find the Authentication-Info Header!");
        return false;
    }

    /* get the next nonce */
    Sip_ParmAifnoFind(pstAuthInfo, EN_SIP_AINFO_NEXTNONCE, &pstAinfo);
    if (pstAinfo == 0)
    {
        TLOGWARN("SipMsgDecode", "Can not find the Authentication-Info NextNonce Parameter!");
        return false;
    }

    stNextNonce = pstAinfo->u.stNextNonce;
    SipMsgCommon::zosSstr2Str(stNextNonce, nextNonce);
    return true;
}


bool SipMsgDecode::getSubsReason(unsigned char & ucReasonType)
{
    if (Sip_MsgGetSubsReason(_pstMsg, &ucReasonType) != ZOK)
    {
        return false;
    }
    return true;
}


bool SipMsgDecode::getSipUriHeaders(ST_SIP_SIP_URI stSipUri, vector<ST_SIP_HDR> & vectHeaders)
{
    vectHeaders.clear();
    ST_SIP_HDR  *pstHdr;
    ST_ABNF_LIST_NODE *pstNode;
    ZVOID *pstVoid;

    FOR_ALL_DATA_IN_ABNF_LIST(&stSipUri.stHdrs.stHdrLst, pstNode, pstVoid)
    {
        pstHdr = (ST_SIP_HDR *)pstVoid;
        vectHeaders.push_back(*pstHdr);
    }
    return false;
}

bool SipMsgDecode::getUriByAddrSpec(ST_SIP_ADDR_SPEC *pstAddrSpec, std::string & strUri)
{
    void * zMsgBuf;
    ZUCHAR ucHdrsPres;
    ST_ZOS_SSTR stUserUri;
    if (!pstAddrSpec)
        return false;

    /* save old header present flag */
    ucHdrsPres = pstAddrSpec->u.stSipUri.ucHdrsPres;

    /* clear header present flag to make corrent uri */
    if (ucHdrsPres) pstAddrSpec->u.stSipUri.ucHdrsPres = ZFALSE;

    /* encode addr spec */
    if (Abnf_AnyEncodeX(ZPROTOCOL_SIP, (PFN_ABNFENCODE)Sip_EncodeAddrSpec,
        pstAddrSpec, ZFALSE, &zMsgBuf) != ZOK)
        return false;

    ///* get the uri string */
    //if (Zos_UbufCpyDSStr(_pstMsg->zMemBuf, zMsgBuf, &stUserUri) != ZOK)
    //	return false;


    /* check the valid of source data */
    if (!Zos_DbufLen(zMsgBuf)) return ZOK;

    /* copy the string */
    if (Zos_UbufCpyDStr(_pstMsg->zMemBuf, zMsgBuf, &stUserUri.pcStr) != ZOK)
    {
        TLOGWARN("SipMsgDecode", "getUriByAddrSpec() called.UbufCpyDSStr copy the data!");
        return false;
    }

    /* set the dest string length */
    stUserUri.iLen = Zos_DbufLen(zMsgBuf);

    /* restore header present flag */
    pstAddrSpec->u.stSipUri.ucHdrsPres = ucHdrsPres;
    /* delete the buffer */
    Zos_DbufDelete(zMsgBuf);

    SipMsgCommon::zosSstr2Str(stUserUri, strUri);
    return true;
}

bool SipMsgDecode::getRseq(unsigned int & resNum)
{
    unsigned int tem = 0;
    if (Sip_MsgGetRseq(_pstMsg, &tem) != ZOK)
        return false;
    resNum = tem;
    return true;
}

bool SipMsgDecode::getSBCMediaIpAndPort(const std::string & sipMsgWithSDP,  vector<SDPMediaInfo> & vectSdpMediaInfo)
{
    if (sipMsgWithSDP.empty())
        return false;

    ZUCHAR ucMtype;                  /**< @brief m-type type EN_SIP_MTYPE */
    ZUCHAR ucMSubtype;               /**< @brief m-subtype type EN_SIP_MSUBTYPE */
    ST_SIP_HDR_CONTENT_TYPE *pstContentType = 0;
    ST_SDP_SESS_DESC_LST stSDPSessDesLst;

    ST_ABNF_LIST_NODE *pstSessDescMNode;
    ST_SDP_SESS_DESC *pstSessDesc;
    ZVOID * pstSessDescVoidPtr;

    unsigned char ucResult =  decode(sipMsgWithSDP, sipMsgWithSDP.size()) ;
    if (ucResult == EnSipMsgType_Invalid ||  ucResult == EnSipMsgType_Incomplete
        || ucResult == EnSipMsgType_PingRFC5626 || ucResult == EnSipMsgType_PongRFC5626)
    {
        TLOGWARN("SipMsgDecode", "getSBCMediaIpAndPort（） called. Sip msg is not valid or complete. SipMsg: \n" + sipMsgWithSDP);
        return false;
    }

    //ST_SIP_BODY
    if (!_pstMsg->stBody.ucPres)
    {
        TLOGWARN("SipMsgDecode", "getSBCMediaIpAndPort（） called. Sip msg Body not exist! SipMsg:\n" + sipMsgWithSDP);
        return false;
    }

    /* find the content type header */
    pstContentType = (ST_SIP_HDR_CONTENT_TYPE *)Sip_HdrLstFindHdr(&_pstMsg->stHdrLst, EN_SIP_HDR_CONTENT_TYPE);

    /* if have no content type, so we can't distinguish body type */
    if (!pstContentType)
    {
        TLOGWARN("SipMsgDecode", "getSBCMediaIpAndPort() called! Can't found the Content-type header! SipMsg : \n"+sipMsgWithSDP);
        //return false;
    }

    if (pstContentType != 0)
    {
        ucMtype = pstContentType->stMediaType.ucMtype;
        ucMSubtype = pstContentType->stMediaType.ucMSubtype;
        if (ucMSubtype != EN_SIP_MSUBTYPE_SDP)
        {
            TLOGWARN("SipMsgDecode", "getSBCMediaIpAndPort（） called. Content Sub-Media type is not SDP, SipMsg : \n"+sipMsgWithSDP);
            //return false;
        }
    }

    stSDPSessDesLst = _pstMsg->stBody.u.stSpart.u.stSdpMsgLst;
    vectSdpMediaInfo.clear();   //clear

    //sess desc lst circulation
    FOR_ALL_DATA_IN_ABNF_LIST(&stSDPSessDesLst, pstSessDescMNode, pstSessDescVoidPtr)
    {
        pstSessDesc = (ST_SDP_SESS_DESC *)pstSessDescVoidPtr;
        if (pstSessDesc == 0)
        {
            TLOGWARN("SipMsgDecode", "getSBCMediaIpAndPort（） called. sess desc lst circulation converter pointer failed! SipMsg : \n"+sipMsgWithSDP);
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
                TLOGWARN("SipMsgDecode", "getSBCMediaIpAndPort（） called. M desc lst circulation converter pointer failed! SipMsg : \n"+sipMsgWithSDP);
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
        }
        vectSdpMediaInfo.push_back(sdpMediaInfo);
    }
    return true;
}


bool SipMsgDecode::checkSipMsgWithSDP(unsigned char ucMethod,bool isReq,  unsigned long statusCode, bool & bExistSDP,std::string & strSDP)
{
    if (ucMethod != EN_SIP_METHOD_INVITE && ucMethod != EN_SIP_METHOD_ACK && ucMethod != EN_SIP_METHOD_UPDATE && ucMethod != EN_SIP_METHOD_MSG)
        return false;

    if ((!isReq && statusCode != 200) && (!isReq && statusCode != 183) && (!isReq && statusCode != 180))
        return false;

    ST_ZOS_SSTR stBodyStr = _pstMsg->stBody.stBodyStr;
    if (stBodyStr.iLen == 0)
    {
        std::string tempSip = _strMsg;
        int pos = tempSip.find("v=0");
        if (pos != -1)
        {
            strSDP = tempSip.substr(pos, tempSip.size() - pos);
            bExistSDP = true;

            TLOGWARN("SipMsgDecode", "checkSipMsgWithSDP() called. Find the SDP in string,SIZE : \n" +  std::to_string(strSDP.size()) + " SDP :" + strSDP);
            return true;
        }
    }
    else
    {
        SipMsgCommon::zosSstr2Str(stBodyStr,strSDP);
        bExistSDP = true;
        return true;
    }

    bExistSDP =false;
    return true;
}

bool SipMsgDecode::getSipMsgSDPBody(  std::string & strSDP)
{
    ZUCHAR ucMtype;                  /**< @brief m-type type EN_SIP_MTYPE */
    ZUCHAR ucMSubtype;               /**< @brief m-subtype type EN_SIP_MSUBTYPE */
    ST_SIP_HDR_CONTENT_TYPE *pstContentType = 0;
    ST_ZOS_SSTR stBodyStr;

    //ST_SIP_BODY
    if (!_pstMsg->stBody.ucPres)
    {
        TLOGWARN("SipMsgDecode", std::string(__FUNCTION__) +"() called,Sip msg Body not exist!");
        return false;
    }

    /* find the content type header */
    pstContentType = (ST_SIP_HDR_CONTENT_TYPE *)Sip_HdrLstFindHdr(&_pstMsg->stHdrLst, EN_SIP_HDR_CONTENT_TYPE);

    /* if have no content type, so we can't distinguish body type */
    if (!pstContentType)
    {
        TLOGWARN("SipMsgDecode", std::string(__FUNCTION__) +"() called, Can't found the Content-type header! SipMsg : "+ _strMsg);
        return false;
    }
    ucMtype = pstContentType->stMediaType.ucMtype;
    ucMSubtype = pstContentType->stMediaType.ucMSubtype;
    if (ucMSubtype != EN_SIP_MSUBTYPE_SDP)
    {
        TLOGWARN("SipMsgDecode", std::string(__FUNCTION__) +"() called, Content Sub-Media type is not SDP");
        return false;
    }

    stBodyStr = _pstMsg->stBody.stBodyStr;
    if (stBodyStr.iLen == 0)
    {
        TLOGWARN("SipMsgDecode", std::string(__FUNCTION__) +"() called, TEST ST_BODY lenght :" + _strMsg);

        std::string tempSip = _strMsg;
        int pos = tempSip.find("v=0");
        if (pos != -1)
        {
            strSDP = tempSip.substr(pos, tempSip.size() - pos);
        }
    }
    else
    {
        SipMsgCommon::zosSstr2Str(stBodyStr,strSDP);
    }

    if(strSDP.empty())
    {
        TLOGWARN("SipMsgDecode", "getSipMsgSDPBody() called.SDP is EMPTY. SipMsg:" + std::to_string(stBodyStr.iLen));
    }
    return true;
}

bool SipMsgDecode::getHdrSessionExpires(int & seconds, bool & refresherPre, bool & uasRefresh)
{
    unsigned int iSeconds = 0;
    unsigned char chRefresherPre = 0, chUASrefresh = 0;
    if (Sip_MsgGetSessExpires(_pstMsg, &iSeconds) != ZOK)
    {
        return false;
    }
    if (Sip_MsgGetSessRefresher(_pstMsg, &chRefresherPre, &chUASrefresh) != ZOK)
    {
        return false;
    }

    seconds = iSeconds;
    refresherPre = chRefresherPre ? true : false;
    uasRefresh  = chUASrefresh ? true : false;
    return true;
}

bool SipMsgDecode::getAllowOfMethod(unsigned char ucMethod)
{
    vector<int> vectTypes;
    vector<std::string> vectTypeName;
    bool result = getHdrAllow(vectTypes, vectTypeName);
    if (false == result)
        return false;

    for (unsigned int i = 0; i < vectTypes.size(); i++)
    {
        int typeValue = vectTypes.at(i);
        if (typeValue == ucMethod)
        {
            return true;
        }
    }
    return false;
}

bool SipMsgDecode::getHdrContentDispEx(std::string & disp)
{
    ST_SIP_HDR_CONTENT_DISP *pContentDisp = (ST_SIP_HDR_CONTENT_DISP*)Sip_HdrLstFindHdr(&_pstMsg->stHdrLst, EN_SIP_HDR_CONTENT_DISP);
    if (!pContentDisp)
        return false;

    if (pContentDisp->ucDispType == EN_SIP_DISP_TYPE_EXT)
    {
        SipMsgCommon::zosSstr2Str(pContentDisp->stDispExt,disp);
        return true;
    }
    return false;
}

bool SipMsgDecode::pickSipUriParams(ST_SIP_SIP_URI *pstSipUri, vector<ST_SIP_URI_PARM> & params)
{
    if (!pstSipUri)
        return false;
    if (pstSipUri->stUriParmLst.iCount== 0)
        return false;

    unsigned char ucTpt = 0;
    if (parmPickSipUriTpt(pstSipUri, ucTpt))
    {
        ST_SIP_URI_PARM param;
        param.ucType = EN_SIP_URI_PARM_TPT;
        param.u.stTpt.ucType = ucTpt;
        params.push_back(param);
    }

    unsigned char ucUser = 0;
    if (parmPickSipUriUser(pstSipUri, ucUser))
    {
        ST_SIP_URI_PARM param;
        param.ucType = EN_SIP_URI_PARM_USER;
        param.u.stUser.ucType = ucUser;
        params.push_back(param);
    }

    unsigned char ucMethod = 0;
    if (parmPickSipUriMethod(pstSipUri, ucMethod))
    {
        ST_SIP_URI_PARM param;
        param.ucType = EN_SIP_URI_PARM_METHOD;
        param.u.stMethod.ucType = ucMethod;
        params.push_back(param);
    }

    unsigned char ucSess = 0;
    if (parmPickSipUriSess(pstSipUri, ucSess))
    {
        ST_SIP_URI_PARM param;
        param.ucType = EN_SIP_URI_PARM_SESS;
        param.u.ucSess = ucSess;
        params.push_back(param);
    }

    unsigned int ttl = 0;
    if (parmPickSipUriTtl(pstSipUri, ttl))
    {
        ST_SIP_URI_PARM param;
        param.ucType = EN_SIP_URI_PARM_TTL;
        param.u.iTtl = ttl;
        params.push_back(param);
    }
    return true;
}

bool SipMsgDecode::parmPickSipUriTpt(ST_SIP_SIP_URI *pstSipUri,unsigned char & ucTpt)
{
    if (!pstSipUri)
        return false;

    return Sip_ParmPickSipUriTpt(pstSipUri, &ucTpt) == ZOK ? true : false;
}

bool SipMsgDecode::parmPickSipUriUser(ST_SIP_SIP_URI *pstSipUri, unsigned char & ucUser)
{
    if (!pstSipUri)
        return false;
    return Sip_ParmPickSipUriUser(pstSipUri, &ucUser) == ZOK ? true : false;
}

bool SipMsgDecode::parmPickSipUriMethod(ST_SIP_SIP_URI *pstSipUri, unsigned char & ucMethod)
{
    if (!pstSipUri)
        return false;
    return Sip_ParmPickSipUriMethod(pstSipUri, &ucMethod) == ZOK ? true : false;
}

bool SipMsgDecode::parmPickSipUriSess(ST_SIP_SIP_URI *pstSipUri, unsigned char & ucSess)
{
    if (!pstSipUri)
        return false;
    return Sip_ParmPickSipUriSess(pstSipUri, &ucSess) == ZOK ? true : false;
}

bool SipMsgDecode::parmPickSipUriTtl(ST_SIP_SIP_URI *pstSipUri, unsigned int & ttl)
{
    if (!pstSipUri)
        return false;
    return Sip_ParmPickSipUriTtl(pstSipUri, &ttl) == ZOK ? true : false;
}

bool SipMsgDecode::copySipUri(ST_SIP_SIP_URI *pstDst, ST_SIP_SIP_URI *pstSrc)
{
    if (Sip_CpySipUri(_pstMsg->zMemBuf, pstDst, pstSrc) == ZOK)
        return true;
    return false;
}

bool SipMsgDecode::getHdrMinSe(ZUINT & second)
{
    return Sip_MsgGetMinSe(_pstMsg, &second) == ZOK ? true : false;;
}

bool SipMsgDecode::getHdrExt(const std::string & name, std::string & val)
{
    ST_SIP_HDR_EXT_HDR *pstExtHdr = 0;
    ST_ABNF_LIST_NODE *pstNode = 0;
    ST_SIP_MSG_HDR *pstHdr = 0;
    void * pHdr = 0;

    /* check input parameters */
    if (!_pstMsg || !_pstMsg->ucPres || name.empty())
        return false;

    /* look for the message header matched */
    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstNode, pHdr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pHdr;
        if (pstHdr)
            if (pstHdr->ucType == EN_SIP_HDR_EXT)
            {
                /* get the extension header */
                pstExtHdr = pstHdr->u.pstExtHdr;

                /* compare the header name */
                std::string headName;
                SipMsgCommon::zosSstr2Str(pstExtHdr->stName, headName);
                if (headName == name)
                {
                    SipMsgCommon::zosSstr2Str(pstExtHdr->stVal, val);
                    return true;
                }
            }
    }

    return false;
}

bool SipMsgDecode::getHdrHistoryInfo(std::string & name)
{
    ST_SIP_MSG_HDR *pstHdr = 0;
    ST_ABNF_LIST_NODE *pstNode = 0;
    void * pHdr = 0;
    ST_SIP_HDR_HI_INFO *pstHiInfo;

    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstNode, pHdr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pHdr;
        if (pstHdr == 0)
            return false;

        if (pstHdr->ucType != EN_SIP_HDR_HI_INFO)
            continue;

        /* get the extension header */
        pstHiInfo = pstHdr->u.pstHiInfo;

        ST_ABNF_LIST_NODE *pstEntryNode = 0;
        void * pEntryHdr = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHiInfo->stEntryLst, pstEntryNode, pEntryHdr)
        {
            ST_SIP_HI_ENTRY * pstEntry = (ST_SIP_HI_ENTRY *)pEntryHdr;
            if(pstEntry == NULL)
                break;

            std::string uriname;

            //std::string displayname, strname;
            //SipMsgCommon::zosSstr2Str(pstEntry->stNameAddr.stDispName.u.stName, displayname);
            //SipMsgCommon::zosSstr2Str(pstEntry->stNameAddr.stDispName.u.stQStr, strname);
            SipMsgCommon::zosSstr2Str(pstEntry->stNameAddr.stAddrSpec.u.stSipUri.stUserInfo.stUser, uriname);

            name = uriname;

            //TLOGINFO("SipMsgDecode", std::string(__FUNCTION__) +"() called, name :" + name + ", strname:" + strname + ", uriname:" + uriname);

            ST_ABNF_LIST_NODE *pstParamNode = 0;
            void * pParamHdr = 0;
            std::string index;

            FOR_ALL_DATA_IN_ABNF_LIST(&pstEntry->stParmLst, pstParamNode, pParamHdr)
            {
                ST_SIP_HI_PARM * param = (ST_SIP_HI_PARM *)pParamHdr;
                if(param == NULL)
                    break;

                SipMsgCommon::zosSstr2Str(param->u.stIndex, index);

                TLOGINFO("SipMsgDecode", std::string(__FUNCTION__) +"() called, index :" + index);

                if (index == "1")
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool SipMsgDecode::fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int dwPort, ZBOOL bRport,
                             int iRport, const std::string &strBranch)
{
    ST_ZOS_INET_ADDR stAddr;
    ST_ZOS_SSTR stBranch;

    ST_SIP_HDR_VIA *pstHdrVia = 0;
    ST_SIP_VIA_PARM *pstViaParm = 0;

    /* check stBranch */
    if(strBranch.find("z9hG4bK") != 0)
    {
        TLOGWARN("SipMsgEncodec", "the format of strBranch is error, Branch :" + strBranch);
        return false;
    }
    SipMsgCommon::str2ZosSstr(strBranch, stBranch);

    pstHdrVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
    if (!pstHdrVia)
        return false;

    pstViaParm = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstHdrVia->stParmLst);
    if (!pstViaParm)
        return false;

    /* fill via sent-by parameter */
    char * pHostname = strHostname.size() > 0 ? (char*)strHostname.c_str() : 0;
    ZOS_IPV4_ADDR_SET_STRX(&stAddr, pHostname, dwPort);
    if (Sip_ParmFillViaSentBy(pstViaParm, &stAddr) != ZOK)
    {
        Sip_ParmViaLstRmv(&pstHdrVia->stParmLst, pstViaParm);
        return false;
    }
    return true;
}

bool SipMsgDecode::fillHdrVia(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
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
/* fill header contact by addr-spec */
bool SipMsgDecode::fillHdrContactByAddrSpec(ST_SIP_ADDR_SPEC *pstAddrSpec, const std::string &strHost, int iPort)
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

/* fill contact header */
bool SipMsgDecode::fillHdrContact(const std::string &strUserName,const std::string &strHost, int iPort, unsigned char ucTptType)
{
    ST_SIP_HDR_CONTACT *pstContact = 0;
    ST_SIP_ADDR_SPEC *pstAddrSpec = 0;
    ST_ZOS_SSTR stUserName, stHost;
    ST_ZOS_SSTR *pstUserName, *pstHost;

    if (strHost.empty())
    {
        TLOGWARN("SipMsgEncode", std::string(__FUNCTION__) + "() called. strHost is empty! ");
        return false;
    }

    pstUserName = ZNULL;
    pstHost = ZNULL;
    pstContact = (ST_SIP_HDR_CONTACT *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_CONTACT);
    if (!pstContact)
        return false;

    Sip_MsgGetContactAddrSpec(_pstMsg, &pstAddrSpec);
    if (!pstAddrSpec)
        return false;

    /* set addr-spec */
    SipMsgCommon::str2ZosSstr(strUserName, stUserName);
    SipMsgCommon::str2ZosSstr(strHost, stHost);
    if(stUserName.iLen > 0) pstUserName = &stUserName;
    if(stHost.iLen > 0) pstHost = &stHost;
    if(Sip_ParmFillAddrSpecByName(_pstMsg->zMemBuf, pstAddrSpec, pstUserName, pstHost, iPort) == ZFAILED)
        return false;
    return true;
}

/* fill header From */
bool SipMsgDecode::updateHdrFromToDisplayName(bool bFrom, std::string &strDispName)
{
    ST_SIP_NA_SPEC *pstSpec;
    ST_ZOS_SSTR stDispName;

    SipMsgCommon::str2ZosSstr(strDispName, stDispName);
    if(Sip_MsgGetFromToNaSpec(_pstMsg, bFrom, &pstSpec) != ZOK)
        return false;

    return Sip_ParmNaSpecSetDispName(_pstMsg->zMemBuf, pstSpec, &stDispName) == ZOK ? true : false;
}

bool SipMsgDecode::fillBodyDataStr(ZUCHAR ucMtype, ZUCHAR ucMSubType, ST_ZOS_SSTR *pstStr)
{
    if (Sip_MsgFillBodyStr(_pstMsg,  ucMtype, ucMSubType,  pstStr) != ZOK)
        return false;

    return true;
}

bool SipMsgDecode::getHdrPPreferId(std::string & displayName, std::string & strUserInfo, std::string & strHostName, int & iPort)
//bool SipMsgDecode::getHdrPPreferId(std::string & displayName, ST_SIP_SIP_URI & sipUri)
{
    ST_SIP_HDR_P_PREFERRED_ID *pstHdrPPreferId;
    ST_SIP_PPREFERREDID_VAL *pstParam;
    ST_SIP_SIP_URI  stSipUri;
    ST_ZOS_SSTR stDispName;

    pstHdrPPreferId = (ST_SIP_HDR_P_PREFERRED_ID *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_P_PREFERRED_ID);
    if (!pstHdrPPreferId)
        return false;
    //Sip_MsgGetPPreferId(ST_SIP_MSG *pstMsg,ST_SIP_PPREFERREDID_VAL **ppstParm);
    pstParam = (ST_SIP_PPREFERREDID_VAL *)ABNF_LIST_HEAD_DATA(&pstHdrPPreferId->stValLst);

    ZUCHAR ucSipType;

    ucSipType = pstParam->stNaSpec.u.stNameAddr.stAddrSpec.ucType;
    if (ucSipType == EN_SIP_ADDR_SPEC_SIP)
    {
        stSipUri = pstParam->stNaSpec.u.stNameAddr.stAddrSpec.u.stSipUri;
    }
    else if (ucSipType == EN_SIP_ADDR_SPEC_SIPS)
    {
        stSipUri = pstParam->stNaSpec.u.stNameAddr.stAddrSpec.u.stSipsUri;
    }
    else
    {
        return false;
    }

    if(pstParam->stNaSpec.u.stNameAddr.stDispName.ucNamePres)
    {
        stDispName = pstParam->stNaSpec.u.stNameAddr.stDispName.u.stName;
        SipMsgCommon::zosSstr2Str(pstParam->stNaSpec.u.stNameAddr.stDispName.u.stName, displayName);
    }

    if (stSipUri.ucUserInfoPres)
    {
        SipMsgCommon::zosSstr2Str(stSipUri.stUserInfo.stUser, strUserInfo);
    }

    SipMsgCommon::deHostPort(&stSipUri.stHostPort, strHostName, iPort);

    return true;
}

bool SipMsgDecode::delHdrPPreferId()
{
    ST_SIP_HDR_P_PREFERRED_ID *pstHdrPPreferId;

    pstHdrPPreferId = (ST_SIP_HDR_P_PREFERRED_ID *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_P_PREFERRED_ID);
    if (!pstHdrPPreferId)
        return false;

    Sip_DeleteMsgHdr(_pstMsg, EN_SIP_HDR_P_PREFERRED_ID);

    return true;
}

bool SipMsgDecode::fillHdrPAssertedId(std::string & displayName, const std::string & userName, const std::string  & host, const int & port)
{
    ST_SIP_HDR_P_ASSERTED_ID *pstHdrPAssertedId;
    ST_SIP_PASSERTEDID_VAL *pstParm;
    ST_SIP_SIP_URI  stSipUri;
    ST_ZOS_SSTR stDispName;
    ZUBUF zMemBuf;
    ST_SIP_ADDR_SPEC *pstAddrSpec;

    SipMsgCommon::str2ZosSstr(displayName, stDispName);
    zMemBuf = _pstMsg->zMemBuf;

    pstHdrPAssertedId = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
    if (!pstHdrPAssertedId)
    {
        pstHdrPAssertedId = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
        if (!pstHdrPAssertedId)
            return false;

        ST_SIP_PASSERTEDID_VAL *pstVal;
        ST_SIP_ADDR_SPEC *pstAddrSpec;
        /* add a value */
        Sip_ParmAnyLstAdd(zMemBuf, (ZVOID *)&pstHdrPAssertedId->stValLst, sizeof(ST_SIP_PASSERTEDID_VAL), (ZVOID **)&pstVal);

        Sip_ParmNaSpecSetDispName(zMemBuf, &pstVal->stNaSpec, &stDispName);
        Sip_ParmNaSpecGetAddrSpec(&pstVal->stNaSpec, &pstAddrSpec);
        bool result = ParmFillSipUri(&stSipUri, userName, host, port);
        if (!result)
            return false;

        pstAddrSpec->u.stSipUri = stSipUri;
        return true;
    }

    pstParm = (ST_SIP_PASSERTEDID_VAL *)ABNF_LIST_HEAD_DATA(&pstHdrPAssertedId->stValLst);

    Sip_ParmNaSpecSetDispName(zMemBuf, &pstParm->stNaSpec, &stDispName);
    Sip_ParmNaSpecGetAddrSpec(&pstParm->stNaSpec, &pstAddrSpec);
    bool result = ParmFillSipUri(&stSipUri, userName, host, port);
    if (!result)
        return false;
    pstAddrSpec->u.stSipUri = stSipUri;

    return true;
}

bool SipMsgDecode::fillHdrPAssertedIdByTel(const std::string & userName)
{
    ST_SIP_HDR_P_ASSERTED_ID *pstHdrPAssertedId;
    ST_SIP_PASSERTEDID_VAL *pstParm;
    ST_SIP_TEL_URI stTelUri;
    ZUBUF zMemBuf;
    ST_SIP_ADDR_SPEC *pstAddrSpec;

    //ST_ZOS_SSTR stDispName;
    //stDispName.iLen = 0;
    //stDispName.pcStr = 0;

    zMemBuf = _pstMsg->zMemBuf;

    pstHdrPAssertedId = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
    if (!pstHdrPAssertedId)
    {
        pstHdrPAssertedId = (ST_SIP_HDR_P_ASSERTED_ID *)Sip_CreateMsgHdr(_pstMsg, EN_SIP_HDR_P_ASSERTED_ID);
        if (!pstHdrPAssertedId)
            return false;

        ST_SIP_PASSERTEDID_VAL *pstVal;
        ST_SIP_ADDR_SPEC *pstAddrSpec;
        /* add a value */
        Sip_ParmAnyLstAdd(zMemBuf, (ZVOID *)&pstHdrPAssertedId->stValLst, sizeof(ST_SIP_PASSERTEDID_VAL), (ZVOID **)&pstVal);

        //Sip_ParmNaSpecSetDispName(zMemBuf, &pstVal->stNaSpec, &stDispName);
        Sip_ParmNaSpecGetAddrSpec(&pstVal->stNaSpec, &pstAddrSpec);
        bool result = ParmFillTelUri(&stTelUri, userName);
        if (!result)
            return false;

        pstAddrSpec->ucType = EN_SIP_ADDR_SPEC_TEL;
        pstAddrSpec->u.stTelUri = stTelUri;
        return true;
    }

    pstParm = (ST_SIP_PASSERTEDID_VAL *)ABNF_LIST_HEAD_DATA(&pstHdrPAssertedId->stValLst);

    //Sip_ParmNaSpecSetDispName(zMemBuf, &pstParm->stNaSpec, &stDispName);
    Sip_ParmNaSpecGetAddrSpec(&pstParm->stNaSpec, &pstAddrSpec);
    bool result = ParmFillTelUri(&stTelUri, userName);
    if (!result)
        return false;

    pstAddrSpec->ucType = EN_SIP_ADDR_SPEC_TEL;
    pstAddrSpec->u.stTelUri = stTelUri;

    return true;
}

bool SipMsgDecode::ParmFillSipUri(ST_SIP_SIP_URI *pstSipUri, const std::string & userName, const std::string & strHostname, int iPort)
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

bool SipMsgDecode::ParmFillTelUri(ST_SIP_TEL_URI *pstTelUri, const std::string & userName)
{
    ST_ZOS_SSTR  stUserInfo;
    ST_ZOS_SSTR *pstUserInfo = 0;

    SipMsgCommon::str2ZosSstr(userName, stUserInfo);
    pstUserInfo = userName.size() > 0 ? &stUserInfo : 0;
    pstTelUri->ucGlobalNumberPres = false;
    if (Sip_ParmFillTelUri(_pstMsg->zMemBuf, pstTelUri, false, pstUserInfo) != ZOK)
        return false;

    return true;
}

/* fill contact header */
bool SipMsgDecode::fillHdrContact2(const std::string &strUserName,const std::string &strHost, int iPort, unsigned char ucTptType)
{
    ST_SIP_HDR_CONTACT *pstContact;
    ST_SIP_CONTACT_PARM *pstContactParm;
    ST_SIP_ADDR_SPEC *pstAddrSpec;
    ST_ZOS_SSTR stUserName, stHost;
    ST_ZOS_SSTR *pstUserName, *pstHost;
    ZDBUF zMemBuf;

    if (strHost.empty())
    {
        TLOGWARN("SipMsgEncode", std::string(__FUNCTION__) + "() called. strHost is empty! ");
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


bool SipMsgDecode::fillHdrVia2(unsigned char ucTptType, const std::string &strHostname, int dwPort, ZBOOL bRport,
                              int iRport, const std::string &strBranch)
{
    ST_ZOS_INET_ADDR stAddr;
    ST_ZOS_SSTR stBranch;

    /* check stBranch */
    if(strBranch.find("z9hG4bK") != 0)
    {
        TLOGWARN("SipGateway", "the format of strBranch is error, Branch :" + strBranch);
        return false;
    }
    SipMsgCommon::str2ZosSstr(strBranch, stBranch);
    /* set stAddr */
    ZOS_IPV4_ADDR_SET_STRX(&stAddr,strHostname.c_str(),dwPort);

    //TEST -------------------------------
    int tempIp = stAddr.u.iIp;
    //TLOGDEBUG("SipGateway", "fillHdrVia() called.ST_ZOS_INET_ADDR ip :" + strHostname +" stAddr, wType :" + std::string((int)stAddr.wType) + " wPort: " + std::string((int)stAddr.wPort) + " ipv4 :" + std::string(tempIp));

    if(Sip_MsgFillHdrVia(_pstMsg, ucTptType, &stAddr, &stBranch, bRport, iRport) == ZOK)
    {
        //linux下上述填充会出现ip地址为0或者其他随机ip的情况。 因此需要重置，否则无法挂断
        ST_SIP_HDR_VIA *pstHdrVia = 0;
        ST_SIP_VIA_PARM *pstViaParm = 0;
        std::string strHost;
        int iPort = 0;

        pstHdrVia = (ST_SIP_HDR_VIA *)Sip_FindMsgHdr(_pstMsg, EN_SIP_HDR_VIA);
        pstViaParm = (ST_SIP_VIA_PARM *)ABNF_LIST_HEAD_DATA(&pstHdrVia->stParmLst);

        //TLOGDEBUG("SipGateway", "fillHdrVia() called. ST_SIP_HDR_VIA  count :" + std::string((int)pstHdrVia->stParmLst.iCount) );
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

bool SipMsgDecode::fillHdrVia2(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
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

bool SipMsgDecode::fillHdrVia3(unsigned char ucTptType, const std::string &strHostname, int dwPort,ZBOOL bRport,
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

bool SipMsgDecode::fillHdrViaExtParams(const vector<ST_SIP_VIA_PARMS> & prams)
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

bool SipMsgDecode::getHdrById(int ucType)
{
    return Sip_FindMsgHdr(_pstMsg, ucType) == 0 ? false : true;
}

bool SipMsgDecode::delHdrById(int ucType)
{
    if (!Sip_FindMsgHdr(_pstMsg, ucType))
        return false;

    return Sip_DeleteMsgHdr(_pstMsg, ucType) == ZOK ? true : false;
}

bool SipMsgDecode::delHdrVia(const std::string &ip, const std::string &ip2, const std::string &ip3)
{
    ST_SIP_MSG_HDR * pstHdr = 0;
    ST_ABNF_LIST_NODE * pstHdrNode = 0, *pstParmNode = 0;
    ZVOID * pstViaParmPtr = 0, *pstVidHdrPtr = 0, *pstViaParmPtr2 = 0;
    ST_SIP_HDR_VIA * pstHdrVia = 0;
    ST_SIP_VIA_PARM * pstViaParm = 0;
    ST_ZOS_INET_ADDR stAddr;
    char acRmtAddr[ZINET_IPV4_PORT_STR_SIZE];
    ZUINT pwRport = 0;
    unsigned char  ucTptType = 0;
    std::string  strHost, strRecvHost;
    int iRport = 0, iPort = 0, recvPort = 0;

    FOR_ALL_DATA_IN_ABNF_LIST(&_pstMsg->stHdrLst, pstHdrNode, pstVidHdrPtr)
    {
        pstHdr = (ST_SIP_MSG_HDR *)pstVidHdrPtr;
        if (pstHdr == 0)
            return false;

        if (pstHdr->ucType != EN_SIP_HDR_VIA)
            continue;

        pstHdrVia = pstHdr->u.pstVia;
        int index = 0;
        ST_ABNF_LIST_NODE * pstHdrNode3 = 0;
        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdrVia->stParmLst, pstHdrNode3, pstViaParmPtr2)
        {
            ST_SIP_VIA_PARM* pstParam = (ST_SIP_VIA_PARM *)pstViaParmPtr2;
            if (pstParam == 0)
                continue;

            index++;
        }

        FOR_ALL_DATA_IN_ABNF_LIST(&pstHdrVia->stParmLst, pstParmNode, pstViaParmPtr)
        {
            pstViaParm = (ST_SIP_VIA_PARM *)pstViaParmPtr;
            if(pstViaParm == NULL)
            {
                break;
            }

            /* get strHost, iPort */
            if(Sip_ParmPickViaSentBy(pstViaParm, &stAddr) == ZOK)
            {
                ZOS_INET_ADDR_TO_STR(&stAddr, acRmtAddr);
                std::string strTemp = std::string(acRmtAddr);
                int iTemp = strTemp.find(":");
                if(iTemp != -1)
                {
                    strHost = strTemp.substr(0, iTemp);
                }
                else
                {
                    strHost = strTemp;
                }
            }

            if (strHost == ip || strHost == ip2 || strHost == ip3)
            {
                if (index > 1)
                    ABNF_LIST_REMOVE_DATA(&pstHdrVia->stParmLst, pstViaParm);
                else
                    ABNF_LIST_REMOVE_DATA(&_pstMsg->stHdrLst, pstHdr);

                return true;
            }
        }
    }
    return true;
}

/* fill header From */
bool SipMsgDecode::fillHdrFromToByName(bool bFrom,const std::string &strDispName,const std::string &strUserInfo,
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

bool SipMsgDecode::fillHdrFromToBySipsName(bool bFromHdr, const std::string & strDispName, const std::string & strUserInfo, const std::string & strHostname,
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

bool SipMsgDecode::fillHdrFromToTelUri(bool bFromHdr,const std::string &strDispName, const std::string &strNumber,
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


bool SipMsgDecode::fillHdrFromToByName(bool bFrom, const std::string &strDispname,const std::string &strUserInfo,
                                       const std::string &strHostname, int iPort, const std::string &strTag, unsigned char &ucReqUriType)
{
    bool result = false;

    switch(ucReqUriType)
    {
    case EN_SIP_ADDR_SPEC_SIP:
        result = fillHdrFromToByName(bFrom, strDispname ,strUserInfo, strHostname,iPort, strTag);
        if (!result)
        {
            TLOGWARN("SipMsgEncode", "fillHdrFromToByName() failed.strDispname:" + strDispname + ",strUserInfo:" + strUserInfo + ",strHostname:" + strHostname + ",iPort:" + std::to_string(iPort) + ",strTag:" + strTag);
        }
        break;

    case EN_SIP_ADDR_SPEC_SIPS:
        result = fillHdrFromToBySipsName(bFrom,  strDispname,  strUserInfo, strHostname,  iPort,  strTag);
        if (!result)
        {
            TLOGWARN("SipMsgEncode", "fillHdrFromToBySipsName() failed.strDispname:" + strDispname + ",strUserInfo:" + strUserInfo + ",strHostname:" + strHostname + ",iPort:" + std::to_string(iPort) + ",strTag:" + strTag);
        }
        break;

    case EN_SIP_ADDR_SPEC_TEL:
        result = fillHdrFromToTelUri(bFrom, strDispname, strUserInfo,strTag);
        if (!result)
        {
            TLOGWARN("SipMsgEncode", "fillHdrFromToTelUri() failed.strDispname:" + strDispname + ",strUserInfo:" + strUserInfo + ",iPort:" + std::to_string(iPort) + ",strTag:" + strTag);
        }
        break;

        // others is not supported for this moment

    default:
        return false;
    }
    return result;
}


/* fill request line */
bool SipMsgDecode::updateReqLine(unsigned char ucUriType, unsigned char ucMethod,
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
bool SipMsgDecode::fillReqLineBySip(unsigned char ucMethod,const std::string &strUserInfo,
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


/* fill request sips line */
bool SipMsgDecode::fillReqLineBySips(unsigned char ucMethod,const std::string &strUserInfo, const std::string &strHostname,int iPort)
{
    ST_SIP_SIP_URI stSipUri;
    ST_ZOS_SSTR stUserInfo;
    ST_SIP_HOST_PORT stHostPort;
    ST_ZOS_SSTR *pstUserInfo = 0;

    if (strHostname.empty())
        return false;

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
bool SipMsgDecode::fillReqLineByTel(unsigned char ucMethod, const std::string &strNumber)
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

bool SipMsgDecode::fillHdrExt(const std::string & name, const std::string & val)
{
    if (Sip_FillExtHdr(_pstMsg, (char*)name.c_str(), (char*)val.c_str()) != ZOK)
        return false;
    return true;
}

bool SipMsgDecode::authDRspFillNonce(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strNonce)
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

bool SipMsgDecode::authDRspFillUsername(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strUsername)
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

bool SipMsgDecode::authDRspFillUri(ST_SIP_DIGEST_RSP *pstRsp, ST_SIP_REQ_URI *pstReqUri)
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

bool SipMsgDecode::authDRspFillRsp(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strResponse)
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

bool SipMsgDecode::authDRspFillCnonce(ST_SIP_DIGEST_RSP *pstRsp, ST_ZOS_SSTR *pstCnonce)
{
    ST_SIP_DIG_RSP *pstParm;
    /* remove old parameter */
    if (Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_CNONCE, &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }

    return Sip_ParmFillDRspCnonce(_pstMsg->zMemBuf, pstRsp, pstCnonce) == ZOK ? true : false;

}
bool SipMsgDecode::authDRspFillNonceCount(ST_SIP_DIGEST_RSP *pstRsp, ST_ZOS_SSTR *pstCount)
{
    ST_SIP_DIG_RSP *pstParm;
    /* remove old parameter */
    if (Sip_ParmDRspLstFind(&pstRsp->stDigRspLst, EN_SIP_DIG_RSP_NONCE_COUNT, &pstParm) == ZOK)
    {
        Sip_ParmDRspLstRmv(&pstRsp->stDigRspLst, pstParm);
    }
    return Sip_ParmFillDRspNonceCount(_pstMsg->zMemBuf, pstRsp, pstCount) == ZOK ? true : false;
}

bool SipMsgDecode::authDRspFillRealm(ST_SIP_DIGEST_RSP *pstRsp, const std::string &strRealm)
{
    std::string strTem = strRealm;
    if (Sip_ParmFillDRspRealmX(_pstMsg->zMemBuf, pstRsp, (char*)strTem.c_str()) != ZOK)
        return false;
    return true;
}

bool SipMsgDecode::authDRspFillAlgo(ST_SIP_DIGEST_RSP *pstRsp, unsigned char algoType)
{
    if (Sip_ParmFillDRspAlgoX(_pstMsg->zMemBuf, pstRsp,algoType) != ZOK)
        return false;
    return true;
}

bool SipMsgDecode::authDRspFillQop(ST_SIP_DIGEST_RSP *pstRsp, unsigned char qopType)
{
    if (Sip_ParmFillDRspQopX(_pstMsg->zMemBuf, pstRsp,qopType) != ZOK)
        return false;
    return true;
}

bool SipMsgDecode::authDRspFillQpaque(ST_SIP_DIGEST_RSP *pstRsp, const std::string & opaque)
{
    ST_ZOS_SSTR stOpaque;
    SipMsgCommon::str2ZosSstr(opaque, stOpaque);
    if (Sip_ParmFillDRspOpaque(_pstMsg->zMemBuf, pstRsp, &stOpaque) != ZOK)
        return false;

    return true;
}

/* fill header cseq */
bool SipMsgDecode::fillHdrCseq(unsigned char ucMethod, int iCseq)
{
    if(Sip_MsgFillHdrCSeq(_pstMsg, ucMethod, iCseq) == ZOK)
    {
        return true;
    }
    return false;
}

/* fill header Authorization, after 401 message */
bool SipMsgDecode::fillHdrAuthor(const ST_SIP_DIGEST &stDigest, const std::string & strUserName,const std::string & strPassWd,
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

bool SipMsgDecode::fillHdrAuthor(const ST_SIP_DIGEST_RSP & stDigestResp, int rspCode)
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

/* SIP message fill content-type Header */
bool SipMsgDecode::fillHdrContentType(unsigned char ucMtype, const std::string &strMtypeExt,
                                      unsigned char ucMSubtype, const std::string &strMSubtypeExt)
{
    ST_ZOS_SSTR stMtypeExt, stMSubtypeExt;
    SipMsgCommon::str2ZosSstr(strMtypeExt, stMtypeExt);
    SipMsgCommon::str2ZosSstr(strMSubtypeExt, stMSubtypeExt);
    if(Sip_MsgFillHdrContentType(_pstMsg, ucMtype, &stMtypeExt, ucMSubtype, &stMSubtypeExt) == ZOK)
    {
        return true;
    }

    TLOGWARN("SipMsgEncode", std::string(__FUNCTION__) + "() called , return FALSE!.");
    return false;
}