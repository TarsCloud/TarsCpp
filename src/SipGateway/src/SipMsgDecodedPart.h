#ifndef _SIP_MSG_DECODE_PART_H_
#define  _SIP_MSG_DECODE_PART_H_

#include <string>
#include <map>
#include <vector>
#include "avatar.h"

//reqline part
struct ReqLinePart
{
    unsigned char        _ucMethodType;       // Method type, refer EN_SIP_METHOD
    std::string       _strUserName;       //username, for example,_ucAddrType is EN_SIP_ADDR_SPEC_TEL, this parameter disappear
    std::string       _strHostName;       //hostname
    unsigned long        _dwPort;             //port, equal to 0 when the hostname is EN_SIP_HOST_IPV4 or  EN_SIP_HOST_IPV6
    unsigned char        _reqUriType;         // req uri type, refer EN_SIP_REQ_URI
    std::string       _reqUri;
    ReqLinePart();
    ~ReqLinePart();

    void setReqLinePart(unsigned char ucMethod, std::string strUserName, std::string  strHostName, unsigned long dwPort, unsigned char ucUriType);
    void setReqLinePartEx(unsigned char ucMethod, std::string strUserName, std::string  strHostName, unsigned long dwPort);
    void setReqUri(const std::string & strUri);
    std::string getReqUri();
};

//status line
struct StatusLinePart
{
    unsigned long         _iCode;                 /**< @brief Status-Code */
    std::string        _strReason;            /**< @brief Reason-Phrase */

    StatusLinePart();
    ~StatusLinePart();

    void setStatusLinePart(unsigned int code, std::string strReasom);
};

//Contact , 为了同时记忆From和To的Contact 地址，因此，放入From 和 To中
struct ContactPart
{
    std::string                       _strUserName;
    std::string                       _strHostName;
    unsigned int                         _iPort;
    unsigned char                        _type;
    std::vector<ST_SIP_URI_PARM>              _vectUriParms;
    std::map<std::string, std::string>  _mapOthersPara;

    ContactPart();
    ~ContactPart();
};

//From or To Header Part
struct FromToPart
{
    unsigned char                _ucAddrType;          // refer EN_SIP_ADDR_SPEC
    std::string               _strDispName;         //dispname
    std::string               _strUserName;          //username, for example,_ucAddrType is EN_SIP_ADDR_SPEC_TEL, this parameter is number
    std::string               _strHostName;          //hostname  for example,_ucAddrType is EN_SIP_ADDR_SPEC_TEL, this parameter disappear
    unsigned long                _dwPort;               //port, equal to 0 when the hostname is EN_SIP_HOST_IPV4 or  EN_SIP_HOST_IPV6
    std::string               _strTag;               //tag
    unsigned char                _tptType;               //transport type

    ContactPart                  _contactPart;           //contact
    ContactPart                  _calleeContactPart;           //contact
    std::map<std::string, std::string>     _sipUriOtherParams;     //
    std::map<std::string, std::string>     _fromToParamVals;       //key-value
    std::vector<ST_SIP_URI_PARM>     _sipUriPrams;

    FromToPart();
    ~FromToPart();

    void setFromToPart(unsigned char ucAddrType, std::string strDispName, std::string strUserName, std::string strHost, unsigned long dwPort, std::string strTag, unsigned char ucTptType);
    void setFromToPartEx(std::string strDispName, std::string strUserName, std::string strHost, unsigned long dwPort, std::string strTag, unsigned char ucTptType);
    void setFromToContact(std::string strContactIp, unsigned int iPort);
    void setFromToContact(const std::string & strUserName, const std::string & strContactIp, unsigned int iPort, unsigned char type);
    void setCalleeFromToContact(const std::string & strUserName, const std::string & strContactIp, unsigned int iPort, unsigned char type);
    void setContactSipUriParams(const std::vector<ST_SIP_URI_PARM> & params);
    void setSipUriOtherParams(const std::map<std::string, std::string> & params);
    void setFromToParamVals(const std::map<std::string, std::string> & params);
    void setSipUriParams(const std::vector<ST_SIP_URI_PARM> & sipUriPrams);

};

//Via part
struct ViaPart
{
    std::string              _strHostName;           //host name
    int                         _iPort;                 //port, equal to 0 when the host name is EN_SIP_HOST_IPV4 or  EN_SIP_HOST_IPV6
    unsigned char               _ucTptType;             //transport type
    unsigned int                _ttl;
    int                         _bRport;                //
    int                         _iRport;
    std::string              _strBranch;             //branch
    std::string              _received;
    int                         _recvPort;
    std::vector<ST_SIP_VIA_PARMS>    _paramsExt;

    ViaPart();
    ~ViaPart();

    void setViaPart(std::string strHost, int iPort, unsigned char ucTptType, int bPort, int _iRport, std::string strBrach);
    void setViaPart(std::string strHost, int iPort, unsigned char ucTptType, int bPort, int _iRport, std::string strBrach, int ttl, std::string received, int recvPort);
    void setViaParams(const std::vector<ST_SIP_VIA_PARMS> & paramsExt);
    void setTptType(unsigned char ucTptType);
    unsigned char getTptType();
};


//Cseq part
struct CSeqPart
{
    unsigned int                _iCseqVal;              /**< @brief CSeq value */
    unsigned int                _iCalleeCseqVal;        /**< @brief CSeq value */
    unsigned char                _ucMethodType;          /**< @brief method type EN_SIP_METHOD */
    unsigned int                _iPrackCseqVal;          /**< @brief CSeq value */

    CSeqPart();
    void setCseqPart(unsigned int cseqValue, unsigned char ucMethodType);
};


//Call id
struct CallIdPart
{
    std::string                _strNumber;            /**< @brief number word */
    std::string                _strHost;              /**< @brief host word */

    CallIdPart();
    ~CallIdPart();

    void setCallIdPart(std::string strNumber, std::string strHost);
    std::string getCallIdString();
};

//Record route part
struct RecordRoutePart
{
    unsigned char                 _ucAddrType;                  //refer EN_SIP_ADDR_SPEC
    std::string                _strDispName;                //dispname
    std::string                _strUserName;                //username, for example,_ucAddrType is EN_SIP_ADDR_SPEC_TEL, this parameter disappear
    std::string                _strHostName;                //hostname
    unsigned int                  _dwPort;                        //port, equal to 0 when the hostname is EN_SIP_HOST_IPV4 or  EN_SIP_HOST_IPV6
    unsigned char                _sipUriParmType;
    union paramValue
    {
        unsigned char              tptType;
        unsigned char              userType;
        unsigned char              methodType;
        unsigned int               ttl;
        unsigned int               cause;
        //stMaddr
        unsigned char              ucSess;
        char                       stTarget[50];
        char                       stGr[50];
        char                        stAuid[50];
    }_u;

    RecordRoutePart();
    ~RecordRoutePart();

    void setRecordRoutePart(unsigned char addrType, std::string strDispName, std::string strUserName, std::string strHostName, unsigned long dwPort);
    void setSipUriParam(EN_SIP_URI_PARM type,unsigned char value1, unsigned int value2, std::string value3);
};


//Subscription-State:
enum SubscriprionStateType
{
    SubscriprionActive,
    SubscriprionPending,
    SubscriprionTerminated,
    SubscriprionOthers
};

enum EventPackageType
{
    EventPackageType_PRESENCE,           /**< @brief Type for "presence" in rfc3856. */
    EventPackageType_REFER,              /**< @brief Type for "refer" in rfc3515. */
    EventPackageType_POC_SET,            /**< @brief Type for "poc-settings" in rfc4354. */
    EventPackageType_CONF,               /**< @brief Type for "conference" in rfc4575. */
    EventPackageType_DLG,                /**< @brief Type for "dialog" in rfc4235. */
    EventPackageType_KPML,               /**< @brief Type for "kpml" in rfc4730. */
    EventPackageType_SUMMARY,            /**< @brief Type for "message-summary" in rfc3842. */
    EventPackageType_REG,                /**< @brief Type for "reg" in rfc3680. */
    EventPackageType_PROF,               /**< @brief Type for "ua-profile" in draft-ietf-SIP-xcap-config. */
    EventPackageType_DIFF,               /**< @brief Type for "xcap-diff" in rfc5875. */
    EventPackageType_DMSG,               /**< @brief Type for "deferred-messages" in OMA-TS-CPM_Conv_Fnct-V1_0-20101012-C. */
    EventPackageType_OTHER               /**< @brief Type for otherwise. */
};

//Subscription-State part
struct SubscriptionState
{
    SubscriprionStateType         _stateType;
    unsigned long                 _expires;
    std::string                _pkgTypeName;
    unsigned char                  _reason;         //refrence EN_SIP_EVNT_REASON_VAL
    SubscriptionState();

    void setSubscriptionState(SubscriprionStateType stateType,const std::string & pkgName, unsigned long expires);
    void setSubscriptionTerminatReason(unsigned char ucReason);
};


//Event Part
struct EventPart
{
    unsigned char                _pucPkgType;
    unsigned char                _pucTempType;
    std::string                _id;
    EventPart();
    void setEvent(unsigned char pkgType,  unsigned char tmpType, const std::string id);
};


//xml Content-Type: application/reginfo+xml decode
struct XmlContact
{
    std::string               _uri;
    std::string               _dispName;
    unsigned char                _state;                //有效值   EN_EA_REG_INFO_CONTACT_STA_ACTIVE （0）,EN_EA_REG_INFO_CONTACT_STA_TERM （1）
    unsigned char                _event;
    unsigned int                 _duration;
    unsigned int                 _expires;
    unsigned int                 _retryAfter;
    std::string               _id;
    std::string               _q;
    std::string               _callId;
    unsigned int                 _cseq;
    std::string               _ip;
    int                          _port;
};


struct XmlRegistration
{
    std::string               _aor;
    std::string               _id;
    unsigned char                _state;               //有效值     EN_EA_REG_INFO_REG_STA_INIT,   EN_EA_REG_INFO_REG_STA_ACTIVE, EN_EA_REG_INFO_REG_STA_TERM
    std::vector<XmlContact>           _vectContact;
};

struct XmlReginfo
{
    unsigned int                _version;
    unsigned char               _state;
    std::vector<XmlRegistration>     _vectRegistration;
};

struct RSeqPart
{
    unsigned int                _respNum;
    RSeqPart() {_respNum = 0;}
};


struct BodyPart
{
    unsigned char               _ucPres;
    unsigned char               _ucMpartPres;
    unsigned char               _ucMtype;
    unsigned char               _ucMSubtype;
    std::string              _strBody;

    BodyPart()
    {
        _ucPres = 0;
        _ucMpartPres = 0;
        _ucMtype = 0;
        _ucMSubtype = 0;
    }
    void setBodyPart(unsigned char pres, unsigned char ucMpartPres, unsigned char ucMtype, unsigned char MSubType, const std::string & strBody);
    void setBodyPart(const std::string & strBody);
};

struct SessionExpiresPart
{
    int                          _iSeconds;                          //会话过期时间，秒
    int64_t                      _currTime;                          //接收到含Session-Expires 时时间， 毫秒
    bool                         _bRefreshPre;                       //refresher参数是否存在
    bool                         _bUAS_refresh;                      //UAS 刷新

    SessionExpiresPart()
    {
        _iSeconds = 0;
        _currTime = 0;
        _bRefreshPre = false;
    }

    void setSessionExpiresPart(int seconds, int64_t curTime, bool refreshPre, bool uasRefresh);
};

struct ContentType
{
    unsigned char        _ucMtype;                  /**< @brief m-type type EN_SIP_MTYPE */
    unsigned char       _ucMSubtype;               /**< @brief m-subtype type EN_SIP_MSUBTYPE */
    std::string      _stMtypeExt;
    std::string      _stMSubTypeExt;

    ContentType()
    {
        _ucMtype = 0;
        _ucMSubtype = 0;
    }

    void setContentType(unsigned char mtype, const std::string & mtypeExt, unsigned char msubType, const std::string & msubTypeExt)
    {
        _ucMtype = mtype;
        _stMtypeExt = mtypeExt;
        _ucMSubtype = msubType;
        _stMSubTypeExt = msubTypeExt;
    }

    void getContentType(unsigned char & mtype, std::string & mtypeExt, unsigned char &  msubType, std::string & msubTypeExt)
    {
        mtype = _ucMtype;
        mtypeExt = _stMtypeExt;
        msubType = _ucMSubtype;
        msubTypeExt = _stMSubTypeExt;
    }
};

struct PeerSipUri
{
    bool    _pres;
    ST_SIP_SIP_URI _sipUri;

    PeerSipUri()
    {
        _pres = false;
    }

    void setPeerSipUri(const ST_SIP_SIP_URI & sipUri)
    {
        _pres = true;
        _sipUri = sipUri;
    }
};

struct RecordRouteBySipUri
{
    std::vector<ST_SIP_SIP_URI> _sipUris;
    std::vector<ST_SIP_ROUTE_PARM> _stRouteParams;
};

struct PCalledPartyID
{
    std::string _dispInfo;
    std::string _hostName;

    PCalledPartyID()
    {
        _dispInfo = "";
        _hostName = "";
    }

    void setInfo(const std::string & disp, const std::string & hostName)
    {
        _dispInfo = disp;
        _hostName = hostName;
    }
};

// a stucture to store the decoded information of  a sip msg object
struct SipMsgDecodedPart
{
    bool                          _bReqPres;                // Request present flag
    ReqLinePart                   _reqLinePart;
    StatusLinePart                _statusLinePart;
    FromToPart                    _fromPart;
    FromToPart                    _toPart;
    ViaPart                       _viaPart;
    std::vector<ViaPart>               _viaHdrList;             //为了省事，减少改动量，暂时这么做。
    std::vector<ViaPart>               _viaCalleeHdrList;             //为了省事，减少改动量，暂时这么做。

    CSeqPart                      _cseqPart;
    CallIdPart                    _callIdPart;

    int                           _MaxForwards;
    std::vector<int>                   _vectSuptFlag;
    std::vector<int>                   _vectAllowFlag;
    EventPart                     _eventPart;
    SubscriptionState             _subscriptionState;

    XmlReginfo                   _xmlReginfo;
    RSeqPart                     _rseqPart;
    BodyPart                     _bodyPart;                 //Body
    SessionExpiresPart           _sessionExpiresPart;
    std::map<std::string, std::string> _extHeaders;

    ContentType                  _contentType;
    std::string               _contentDisp;

    std::vector<ST_SIP_HDR_VIA>          _vias;
    PeerSipUri                      _peerSipUri;
    RecordRouteBySipUri             _recordRoutes;
    RecordRouteBySipUri             _calleeRecordRoutes;
    unsigned int                    _minSe;
    PCalledPartyID              _pCalledPartyID;

    SipMsgDecodedPart()
    {
        _bReqPres = true;
        _MaxForwards = 70;
        _minSe = 0;
    }

    ~SipMsgDecodedPart()
    {
        _vectSuptFlag.clear();
        _vectAllowFlag.clear();
    }
    void setExtHeaders(const std::map<std::string, std::string> & headers);
};




#endif



