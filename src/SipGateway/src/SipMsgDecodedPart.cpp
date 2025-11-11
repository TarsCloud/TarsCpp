#include "SipMsgDecodedPart.h"
#include "SipMsgCommon.h"

// ReqLinePart =============================
ReqLinePart::ReqLinePart()
{
    _ucMethodType = 0;
    _strUserName = "";
    _strHostName = "";
    _dwPort = 0;
    _reqUriType = 0;
}

void ReqLinePart::setReqLinePart(unsigned char ucMethod, std::string strUserName, std::string  strHostName, unsigned long dwPort, unsigned char ucUriType)
{
    _ucMethodType = ucMethod;
    _strUserName = strUserName;
    _strHostName = strHostName;
    _dwPort = dwPort;
    _reqUriType = ucUriType;
}

void ReqLinePart::setReqLinePartEx(unsigned char ucMethod, std::string strUserName, std::string  strHostName, unsigned long dwPort)
{
    _ucMethodType = ucMethod;
    _strUserName = strUserName;
    _strHostName = strHostName;
    _dwPort = dwPort;
}

void ReqLinePart::setReqUri(const std::string & strUri)
{
    _reqUri = strUri;
}

std::string ReqLinePart::getReqUri()
{
    return _reqUri;
}


ReqLinePart::~ReqLinePart()
{
    _strUserName = "";
    _strHostName = "";
}

//StatusLinePart =========================
StatusLinePart::StatusLinePart()
{
    _iCode = 0;
}
void StatusLinePart::setStatusLinePart(unsigned int code, std::string strReasom)
{
    _iCode  = code;
    _strReason = strReasom;
}

StatusLinePart::~StatusLinePart()
{
    _strReason = "";
}


//ContactPart
ContactPart::ContactPart()
{
    _strHostName = "";
    _iPort = 0;
    _type = 0;
}

ContactPart::~ContactPart()
{
    _strHostName = "";
}

//FromToPart ================================
FromToPart::FromToPart()
{
    _ucAddrType = 0;
    _strDispName = "";
    _strUserName = "";
    _strHostName = "";
    _dwPort = 0;
    _strTag = "";
    _tptType = 0;
}

void FromToPart::setFromToPart(unsigned char ucAddrType, std::string strDispName, std::string strUserName, std::string strHost, unsigned long dwPort, std::string strTag, unsigned char ucTptType)
{
    _ucAddrType = ucAddrType;
    _strDispName = strDispName;
    _strUserName = strUserName;
    _strHostName = strHost;
    _dwPort = dwPort;
    _strTag = strTag;
    _tptType = ucTptType;
}

void FromToPart::setFromToPartEx(std::string strDispName, std::string strUserName, std::string strHost, unsigned long dwPort, std::string strTag, unsigned char ucTptType)
{
    _strDispName = strDispName;
    _strUserName = strUserName;
    _strHostName = strHost;
    _dwPort = dwPort;
    _strTag = strTag;
    _tptType = ucTptType;
}

FromToPart::~FromToPart()
{
    _strDispName = "";
    _strUserName = "";
    _strHostName = "";
    _dwPort = 0;
    _strTag = "";
}

void FromToPart::setFromToContact(std::string strContactIp, unsigned int iPort)
{
    _contactPart._strHostName = strContactIp;
    _contactPart._iPort = iPort;
}

void FromToPart::setFromToContact(const std::string & strUserName, const std::string & strContactIp, unsigned int iPort, unsigned char type)
{
    _contactPart._strUserName = strUserName;
    _contactPart._strHostName = strContactIp;
    _contactPart._iPort = iPort;
    _contactPart._type = type;
}

void FromToPart::setCalleeFromToContact(const std::string & strUserName, const std::string & strContactIp, unsigned int iPort, unsigned char type)
{
    _calleeContactPart._strUserName = strUserName;
    _calleeContactPart._strHostName = strContactIp;
    _calleeContactPart._iPort = iPort;
    _calleeContactPart._type = type;
}

void FromToPart::setContactSipUriParams(const vector<ST_SIP_URI_PARM> & params)
{
    _contactPart._vectUriParms = params;
    for (unsigned int i = 0; i < params.size(); i++)
    {
        ST_SIP_URI_PARM param = params.at(i);
        if (param.ucType == EN_SIP_URI_PARM_OTHER)
        {
            std::string strName;
            std::string strValue;
            SipMsgCommon::zosSstr2Str(param.u.stOther.stName, strName);
            if (param.u.stOther.ucValPres)
            {
                SipMsgCommon::zosSstr2Str(param.u.stOther.stVal, strValue);
                _contactPart._mapOthersPara.insert(make_pair(strName.c_str(), strValue.c_str()));
            }
        }
    }
}

void FromToPart::setSipUriOtherParams(const map<std::string, std::string> & params)
{
    map<std::string, std::string>::const_iterator iterParam = params.begin();
    for (; iterParam != params.end(); iterParam++)
    {
        _sipUriOtherParams[iterParam->first] = iterParam->second;
    }
}

void FromToPart::setFromToParamVals(const map<std::string, std::string> & params)
{
    map<std::string, std::string>::const_iterator iterParam = params.begin();
    for (; iterParam != params.end(); iterParam++)
    {
        _fromToParamVals[iterParam->first] = iterParam->second;
    }
}

void FromToPart::setSipUriParams(const vector<ST_SIP_URI_PARM> & sipUriPrams)
{
    _sipUriPrams = sipUriPrams;
}

//ViaPart ======================================
ViaPart::ViaPart()
{
    _strHostName = "";
    _iPort = 0;
    _ucTptType = 0;
    _ttl = 0;
    _bRport = true;
    _iRport= 0;
    _strBranch = "";
    _recvPort = 0;
}

ViaPart::~ViaPart()
{
    _strHostName = "";
    _strBranch = "";
}

void ViaPart::setViaPart(std::string strHost, int iPort, unsigned char ucTptType, int bPort, int iRport, std::string  strBrach)
{
    _strHostName = strHost;
    _iPort = iPort;
    _ucTptType = ucTptType;
    _bRport = bPort;
    _iRport= iRport;
    _strBranch = strBrach;
}

void ViaPart::setViaPart(std::string strHost, int iPort, unsigned char ucTptType, int bPort, int iRport, std::string strBrach, int ttl, std::string received, int recvPort)
{
    _strHostName = strHost;
    _iPort = iPort;
    _ucTptType = ucTptType;
    _bRport = bPort;
    _iRport= iRport;
    _strBranch = strBrach;
    _ttl = ttl;
    _received = received;
    _recvPort = recvPort;
}

void ViaPart::setViaParams(const vector<ST_SIP_VIA_PARMS> & paramsExt)
{
    _paramsExt = paramsExt;
}

void ViaPart::setTptType(unsigned char ucTptType)
{
    _ucTptType = ucTptType;
}

unsigned char ViaPart::getTptType()
{
    return _ucTptType;
}

//CSeqPart ======================================

CSeqPart:: CSeqPart()
{
    _iCseqVal = 0;
    _ucMethodType = 0;
}

void CSeqPart::setCseqPart(unsigned int cseqValue, unsigned char ucMethodType)
{
    _iCseqVal = cseqValue;
    _ucMethodType = ucMethodType;
}


// CallIdPart ==============================================
CallIdPart::CallIdPart()
{
    _strNumber = "";
    _strHost = "";
}

CallIdPart::~CallIdPart()
{
    _strNumber = "";
    _strHost = "";
}

void CallIdPart::setCallIdPart(std::string strNumber, std::string strHost)
{
    _strNumber = strNumber;
    _strHost = strHost;
}

std::string  CallIdPart::getCallIdString()
{
    return _strNumber +"@" + _strHost;
}

//RecordRoutePart =============================================
RecordRoutePart::RecordRoutePart()
{
    _ucAddrType = 0;
    _strDispName = "";
    _strUserName = "";
    _strHostName = "";
    _dwPort = 0;
}

RecordRoutePart::~RecordRoutePart()
{
    _strDispName = "";
    _strUserName = "";
    _strHostName = "";
}

void RecordRoutePart::setRecordRoutePart(unsigned char addrType, std::string strDispName, std::string strUserName, std::string strHostName, unsigned long dwPort)
{
    _ucAddrType = addrType;
    _strDispName = strDispName;
    _strUserName = strUserName;
    _strHostName = strHostName;
    _dwPort = dwPort;
}

void RecordRoutePart::setSipUriParam(EN_SIP_URI_PARM type,unsigned char value1, unsigned int value2, std::string value3)
{
    _sipUriParmType = type;
    switch(type)
    {
    case EN_SIP_URI_PARM_TPT:
        _u.tptType = value1;
        break;

    case EN_SIP_URI_PARM_USER:
        _u.userType = type;
        break;

    case EN_SIP_URI_PARM_METHOD:
        _u.methodType = value1;
        break;

    case EN_SIP_URI_PARM_TTL:
        _u.ttl = value2;
        break;

    case EN_SIP_URI_PARM_CAUSE:
        _u.cause = value2;
        break;

    case EN_SIP_URI_PARM_SESS:
        _u.ucSess = value1;
        break;

    case EN_SIP_URI_PARM_TARGET:
        strcpy(_u.stTarget, value3.c_str());
        break;

    case EN_SIP_URI_PARM_GR:
        strcpy(_u.stGr, value3.c_str());
        break;

    case EN_SIP_URI_PARM_AUID:
        strcpy(_u.stAuid, value3.c_str());
        break;
    }
}


//Subscription-State part ===============================================

SubscriptionState::SubscriptionState()
{
    _stateType = SubscriprionActive;
}

void SubscriptionState::setSubscriptionState(SubscriprionStateType stateType,const std::string & pkgName, unsigned long expires)
{
    _stateType = stateType;
    _expires = expires;
    _pkgTypeName = pkgName;
    _reason  = 0;
}

void SubscriptionState::setSubscriptionTerminatReason(unsigned char ucReason)
{
    _reason = ucReason;
}

//Event part ==============================================

EventPart::EventPart()
{
    _pucPkgType = 0;
    _pucTempType = 0;
}

void EventPart::setEvent(unsigned char pkgType,  unsigned char tmpType, const std::string id)
{
    _pucPkgType = pkgType;
    _pucTempType = tmpType;
    _id = id;
}


//BodyPart ==================================================
void BodyPart::setBodyPart(unsigned char pres, unsigned char ucMpartPres, unsigned char ucMtype, unsigned char MSubType, const std::string & strBody)
{
    _ucPres = pres;
    _ucMpartPres = ucMpartPres;
    _ucMtype = ucMtype;
    _ucMSubtype = MSubType;
    _strBody = strBody;
}

void BodyPart::setBodyPart(const std::string & strBody)
{
    _strBody = strBody;
}

//Session-Expires =======================================
void SessionExpiresPart::setSessionExpiresPart(int seconds, int64_t curTime, bool refreshPre, bool uasRefresh)
{
    _iSeconds = seconds;
    _currTime = curTime;
    _bRefreshPre = refreshPre;
    _bUAS_refresh = uasRefresh;
}

void SipMsgDecodedPart::setExtHeaders(const map<std::string, std::string> & headers)
{
    map<std::string, std::string>::const_iterator iterKey = headers.begin();
    for (; iterKey != headers.end(); iterKey++)
    {
        _extHeaders[iterKey->first] = iterKey->second;
    }
}

