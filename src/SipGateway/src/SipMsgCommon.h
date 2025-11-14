#ifndef __SipMsgCommon_h
#define __SipMsgCommon_h

#include <time.h>
#include <stdlib.h>
#include <string>
#include <map>

#include "servant/Application.h"

#include "avatar/avatar.h"
#include "util/zrandom.h"
#include "protocol/sip/sip_msg_cpy.h"

#include "SipMsgType.h"
#include "ConstSipMsgString.h"
#include "json/json.h"


#define SIPPROXY_LOG_DBG(log) TLOGDEBUG("[SipProxy] " << log << endl)
#define SIPPROXY_LOG_WRN(log) TLOGWARN("[SipProxy] " << log << endl)
#define SIPPROXY_LOG_IFO(log) TLOGINFO("[SipProxy] " << log << endl)
#define SIPPROXY_LOG_ERR(log) TLOGERROR("[SipProxy] " << log << endl)

#define SIPPROXY_LOGFMT_DBG(fmt,...) TLOGDEBUG("[SipProxy] " << fmt << endl)
#define SIPPROXY_LOGFMT_WRN(fmt,...) TLOGWARN("[SipProxy] " << fmt << endl)
#define SIPPROXY_LOGFMT_IFO(fmt,...) TLOGINFO("[SipProxy] " << fmt << endl)
#define SIPPROXY_LOGFMT_ERR(fmt,...) TLOGERROR("[SipProxy] " << fmt << endl)

// Const
static const int SIP_PROXY_SINGLE_MESSAGE_MAX_LENGTH = 2 * 1024 * 1024;

// Http
static const std::string SIP_PROXY_NAME_HTTP_PROTOCOL = "HttpTransport";
static const std::string SIP_PROXY_DEFAULT_HTTP_PROTOCOL = "tcp";

static const std::string SIP_PROXY_NAME_HTTP_PORT = "HttpPort";
static const int SIP_PROXY_DEFAULT_HTTP_PORT = 5000;

static const std::string SIP_PROXY_NAME_HTTP_MAX_CONNECT = "HttpMaxConnect";
static const int SIP_PROXY_DEFAULT_HTTP_MAX_CONNECT = 50000;

// Sip
static const std::string SIP_PROXY_NAME_SIP_PROTOCOL = "SipTransport";
static const std::string SIP_PROXY_DEFAULT_SIP_PROTOCOL = "udp";

static const std::string SIP_PROXY_NAME_SIP_LOCAL_PORT = "SipLocalPort";
static const int SIP_PROXY_DEFAULT_SIP_LOCAL_PORT = 5060;

static const std::string SIP_PROXY_NAME_SIP_REMOTE_HOST = "SipRemoteHost";
static const std::string SIP_PROXY_DEFAULT_SIP_REMOTE_HOST = "120.197.90.65";

static const std::string SIP_PROXY_NAME_SIP_REMOTE_PORT = "SipRemotePort";
static const int SIP_PROXY_DEFAULT_SIP_REMOTE_PORT = 5060;

static const std::string SIP_PROXY_NAME_SIP_MAX_CONNECT = "SipMaxConnect";
static const int SIP_PROXY_DEFAULT_SIP_MAX_CONNECT = 100000;

static const std::string SIP_PROXY_NAME_YES_DOMAIN = "YesDomain";
static const std::string SIP_PROXY_DEFAULT_YES_DOMAIN = "yes.my";

static const std::string SIP_PROXY_NAME_YES_REALM = "Realm";
static const std::string SIP_PROXY_DEFAULT_YES_REALM = "ytlcomms.net";

static const  std::string SIP_GW_UUI_HEAER_NAME = "User-to-User";

static const  std::string SIP_GW_OCS_CONFERENCE_ID = "ocs-conf-id";
static const  std::string SIP_GW_CALL_FORWARDING_DIVERSION = "Diversion";

static const  std::string SIP_GW_ORIGIN_CALL_USER = "origin_call_id";
static const  std::string SIP_GW_DST_CALL_USER = "dst_call_user";

static const  std::string SIP_GW_SERVICE_CMICOVS_INFO = "Service-Cmicovs-Info";

static const int OCS_TIMER_MAX_COUNT = 20000;

class SipMsgCommon
{
public:

    /**
    * @brief create a new data buffer.
    */
    static void *dbufCreate(int iDftBlkSize = 64);

    /**
    * @brief delete data buffer.
    *
    * @param [in] zBuf Data buffer.
    */
    static void dbufDelete(void *zBuf);

    /* speak */
    static bool speak(const std::string &str);

    /* transform ST_ZOS_SSTR to std::string */
    static bool zosSstr2Str(const ST_ZOS_SSTR &stSrc, std::string &strDst);

    //transform ST_ZOS_USTR to  std::string
    static bool zosUstr2Str(const ST_ZOS_USTR & stSrc,std::string & strDst);

    /* transform std::string to ST_ZOS_SSTR */
    static bool str2ZosSstr(const std::string &strSrc, ST_ZOS_SSTR &stDst);

    /* transform EnSipMsgType to std::string */
    static std::string sipMsgType2String(unsigned char ucSipMsgType);

    /* print ST_SIP_MSG */
    static void printSipMsg(ST_SIP_MSG *pstMsg);

    /**
    * @brief print ST_SDP_SESS_DESC.
    *
    * @param [in] pstSdp.
    */
    static void printSipSdp(ST_SDP_SESS_DESC *pstSdp);

    /**
    * @brief print StSdpSessDesc.
    *
    * @param [in] pstSdp.
    */
    static void printSipSdp(StSdpSessDesc stSdpDesc);

    /**
    * @brief print StSdpMediaDesc.
    *
    * @param [in] pstSdp.
    */
    static void printSdpMediaDesc(StSdpMediaDesc stMediaDesc);

    /**
    * @brief print StSdpAttr.
    *
    * @param [in] pstSdp.
    */
    static void printSdpAttr(StSdpAttr stAttr);

    /**
    * @brief print ST_SDP_SESS_DESC.
    *
    * @param [in] pstMsg.
    */
    static bool printSdpSessDesc(ST_SDP_SESS_DESC *pstMsg);

    /*
    decode (ZCHAR *) to ST_SIP_MSG
    param[in] content of sip msg
    param[in] length of sip msg
    param[out] struct of sip msg
    */
    static ZINT SipMsg_Decode(ZCHAR *pcMsg, ZUINT iMsgLen, ST_SIP_MSG **ppstMsg);

    static void SipMsg_Destory();

    /*
    encode ST_SIP_MSG to (ZCHAR *)
    param[in] struct of sip msg
    param[out] content of sip msg
    param[out] length of sip msg
    */
    static ZINT SipMsg_Encode(ST_SIP_MSG *pstMsg, ZCHAR *pcBuf, ZUINT & iBufLen);

    static bool SipMsg_Init(int fileSize = 5 * 1024 * 1024, int fileCount = 10, bool bLicense = false);

    static ZINT SipMsg_UriByAddrSpec(ZUBUF zBufId, ST_ZOS_SSTR *pstUri,
        ST_SIP_ADDR_SPEC *pstAddrSpec);

    static ZINT SipMsg_UriByNaSpec(ZUBUF zBufId, ST_ZOS_SSTR *pstUri,
        ST_SIP_NA_SPEC *pstNaSpec);

    /**
    * @brief Get description string of specific method type.
    *
    * @param [in] ucMethod SIP method type, @ref EN_SIP_METHOD.
    *
    * @return Description string of method type.
    */
    static std::string sipMethod2String(unsigned char ucMethod);

    /**
    * @encode ST_SIP_USER_INFO.
    *
    * @param [in] strUserInfo.
    * @param [in] strPassword.
    *
    */
    static bool enUserInfo(const std::string &strUserInfo, const std::string &strPassword,
        ST_SIP_USER_INFO &stUserInfo);

    /**
    * @decode ST_SIP_USER_INFO.
    *
    * @param [in] stUserInfo.
    *
    * @param [out] strUserInfo.
    * @param [out] strPassword.
    */
    static bool deUserInfo(ST_SIP_USER_INFO *pstUserInfo, std::string &strUserInfo,
        std::string &strPassword);

    /**
    * @encode ST_SIP_HOST_PORT.
    *
    * @param [in] strHostname.
    * @param [in] iPort, if iPort == 0, there is no iPort.
    *
    * @param [out] stHostPort.
    */
    static bool enHostPort(const std::string &strHostname, const int iPort,
        ST_SIP_HOST_PORT &stHostPort);

    /**
    * @encode ST_SIP_HOST_PORT.
    *
    * @param [in] strHostname.
    * @param [in] iPort, if iPort == 0, there is no iPort.
    *
    * @param [out] stHostPort.
    */
    static bool enHostPort(const std::string &strHostname, const int iPort,
        ST_SIP_HOST_PORT *pstHostPort);

    /**
    * @decode ST_SIP_HOST_PORT.
    *
    * @param [in] stHostPort.
    *
    * @param [out] strHostname.
    * @param [out] iPort.
    */
    static bool deHostPort(ST_SIP_HOST_PORT *pstHostPort, std::string &strHostname,
        int &iPort);

    /**
    * @decode ST_SIP_HOST .
    *
    * @param [in] stHostPort.
    *
    * @param [out] strHostname.
    * @param [out] iPort.
    */
    static bool deSipHost(ST_SIP_HOST *pstSipHost, std::string &strHostname);

    /**
    * @encode ST_SIP_SIP_URI.
    *
    * @param [in] strUserInfo.
    * @param [in] strPassword.
    * @param [in] strHostname.
    * @param [in] iPort.
    * @param [in] pstUserInfo.
    * @param [in] iPort.
    * @param [out] pstHostPort.
    */
    static bool enSipUri(const std::string &strUserInfo, const std::string &strPassword,
        const std::string &strHostname, const int iPort,ST_SIP_USER_INFO * pstUserInfo, ST_SIP_HOST_PORT * pstHostPort,  ST_SIP_SIP_URI &stSipUri);

    /**
    * @encode ST_SIP_SIP_URI.
    *
    * @param [in] strUserInfo.
    * @param [in] strPassword.
    * @param [in] strHostname.
    * @param [in] iPort.
    *
    * @param [out] stSipUri.
    */
    static bool enSipUri(ZUBUF zBufId, const std::string &strUserInfo, const std::string &strPassword,
        const std::string &strHostname, const int iPort, ST_SIP_SIP_URI &stSipUri);


    /**
    * @decode ST_SIP_SIP_URI.
    *
    * @param [in] pstSipUri.
    *
    * @param [out] strUserInfo.
    * @param [out] strPassword.
    * @param [out] strHostname.
    * @param [out] iPort.
    */
    static bool deSipUri(ST_SIP_SIP_URI *pstSipUri, std::string &strUserInfo,
        std::string &strPassword, std::string &strHostname, int &iPort);

    /**
    * @encode ST_SIP_CALLID.
    *
    * @param [in] strUserInfo.
    * @param [in] strHostname, if there is no hostname, set strHostname = "".
    *
    * @param [out] stCallId.
    */
    static bool enCallId(const std::string &strCallNumber, const std::string &strCallHost,
        ST_SIP_CALLID &stCallId);

    /**
    * @encode ST_ZOS_INET_ADDR by strHost and iPort.
    *
    * @param [in] strHost.
    * @param [in] iPort, if there is not port, set iPort = 0.
    *
    * @param [out] stInetAddr.
    */
    static bool enIpv4Addr(const std::string &strHost, int iPort,
        ST_ZOS_INET_ADDR &stInetAddr);

    /**
    * @decode ST_SDP_OF to.StSdpOrigin
    *
    * @param [in] stSdpOf.
    *
    * @param [out] stSdpOrigin.
    */
    static bool deSdpOrigin(ST_SDP_OF stSdpOf, StSdpOrigin &stSdpOrigin);

    /**
    * @fill SDP version.
    *
    * @param [in] iVersion, @ref int.
    *
    * @param [out] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    */
    static bool fillSdpVersion(int iVersion, ST_SDP_SESS_DESC *pstSdpMsg);

    /**
    * @fill SDP origin-field.
    *
    * @param [in] pstSdpOrigin, @ref StSdpOrigin.
    *
    * @param [out] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    */
    static bool fillSdpOrigin(StSdpOrigin *pstSdpOrigin, ST_SDP_SESS_DESC *pstSdpMsg);

    /**
    * @fill SDP sess-field.
    *
    * @param [in] strSessName, @ref std::string.
    *
    * @param [out] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    */
    static bool fillSdpSessName(const std::string &strSessName, ST_SDP_SESS_DESC *pstSdpMsg);

    /**
    * @decode ST_SDP_CF to StSdpConnInfo.
    *
    * @param [in] stSdpCf.
    *
    * @param [out] stSdpConnInfo.
    */
    static bool deSdpConnInfo(ST_SDP_CF stSdpCf, StSdpConnInfo &stSdpConnInfo);

    /**
    * @encode StSdpConnInfo to ST_SDP_CF.
    *
    * @param [in] stSdpCf.
    *
    * @param [out] stSdpConnInfo.
    */
    static bool enSdpConnInfo(StSdpConnInfo stSdpConnInfo, ST_SDP_CF &stSdpCf);

    /**
    * @decode ST_SDP_SESS_TIME to StSdpTimeDesc.
    *
    * @param [in] stSessTime.
    *
    * @param [out] stTimeDesc.
    */
    static bool deSdpTimeDesc(ST_SDP_SESS_TIME stSessTime, StSdpTimeDesc &stTimeDesc);

    /**
    * @encode StSdpTimeDesc to ST_SDP_SESS_TIME.
    *
    * @param [in] pstTimeField.
    *
    * @param [out] stSessTime.
    */
    static bool enSdpTimeDesc(StSdpTimeField *pstTimeField, ST_SDP_SESS_TIME **ppstTime);

    /**
    * @fill SDP time description.
    *
    * @param [in] pstTimeField, @ref StSdpTimeField.
    *
    * @param [out] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    */
    static bool fillSdpSessTime(const StSdpTimeField *pstTimeField, ST_SDP_SESS_DESC *pstSdpMsg);

    /**
    * @encode StSdpTimeField to ST_SDP_TF.
    *
    * @param [in] pstTimeField.
    *
    * @param [out] stTf.
    */
    static bool enSdpTimeField(StSdpTimeField *pstTimeField, ST_SDP_TF **ppstTf);

    /**
    * @encode std::string to ST_SDP_KF.
    *
    * @param [in] strKey.
    *
    * @param [out] stSdpKf.
    */
    static bool enSdpKey(const std::string &strKey, ST_SDP_KF &stSdpKf);

    /**
    * @decode ST_SDP_MDESC to StSdpMediaDesc.
    *
    * @param [in] stMDesc.
    *
    * @param [out] stMediaDesc.
    */
    static bool deSdpMediaDesc(ST_SDP_MDESC stMDesc, StSdpMediaDesc &stMediaDesc);

    /**
    * @fill SDP media description.
    *
    * @param [in] pstMediaDesc, @ref StSdpMediaDesc.
    *
    * @param [out] pstSdpMsg, @ref ST_SDP_SESS_DESC.
    */
    static bool fillSdpMediaDesc(const StSdpMediaDesc *pstMediaDesc, ST_SDP_SESS_DESC *pstSdpMsg);

    /**
    * @add ST_SDP_TF to ST_SDP_SESS_DESC.
    *
    * @param [in] StSdpTimeField.
    *
    * @param [out] ST_SDP_SESS_DESC.
    */
    static bool addSdpTimeField(StSdpTimeField *pstTimeField, ST_SDP_SESS_DESC &stSessDesc);


    /**
    * @decode ST_SDP_AF to StSdpAttr.
    *
    * @param [in] stAf.
    *
    * @param [out] stAttr.
    */
    static bool deSdpAttr(ST_SDP_AF stAf, StSdpAttr &stAttr);

    /**
    * @encode StSdpAttr to ST_SDP_AF.
    *
    * @param [in] stAttr.
    *
    * @param [out] stAf.
    */
    static bool enSdpAttr(StSdpAttr stAttr, ST_SDP_AF &stAf);

    /**
    * @encode vector<StSdpAttr> to ST_SDP_AF_LST.
    *
    * @param [in] stAttrVec.
    *
    * @param [out] stAfLst.
    */
    static bool enSdpAttrLst(vector<StSdpAttr> stAttrVec, ST_SDP_AF_LST &stAfLst);

    /**
    * @decode ST_SDP_SESS_DESC to StSdpSessDesc.
    *
    * @param [in] stSessDescSrc.
    *
    * @param [out] stSessDescDst.
    */
    static bool deSdpSessDesc(ST_SDP_SESS_DESC stSessDescSrc, StSdpSessDesc &stSessDescDst);

    /**
    * @encode StSdpSessDesc to ST_SDP_SESS_DESC.
    *
    * @param [in] stSessDescSrc.
    *
    * @param [out] stSessDescDst.
    */
    static bool enSdpSessDesc(StSdpSessDesc stSessDescSrc, ST_SDP_SESS_DESC **ppstSdpMsg);

    /**
    * @set StSdpOrigin.
    *
    * @param [in] ucNetType, @ref EN_SDP_NET_TYPE.
    * @param [in] ucAddrType, @ref EN_SDP_ADDR_TYPE.
    * @param [in] lSessId.
    * @param [in] lSessVer.
    * @param [in] strAddr.
    * @param [in] strUsername.
    *
    * @param [out] stOrigin.
    */
    static bool setArcSdpOrigin(unsigned char ucNetType, unsigned char ucAddrType,
        int64_t lSessId, int64_t lSessVer,
        const std::string &strAddr,const std::string &strUsername,
        StSdpOrigin &stOrigin);

    /**
    * @set StSdpConnInfo.
    *
    * @param [in] ucNetType, @ref EN_SDP_NET_TYPE.
    * @param [in] ucAddrType, @ref EN_SDP_ADDR_TYPE.
    * @param [in] strAddr.
    *
    * @param [out] stOrigin.
    */
    static bool setArcSdpConnInfo(unsigned char ucNetType, unsigned char ucAddrType,
        const std::string &strAddr, StSdpConnInfo &stConnInfo);

    /**
    * @init StSdpTimeDesc.
    *
    * @param [in] lStartTime.
    * @param [in] lStopTime.
    *
    * @param [out] stTime.
    */
    static bool setArcSdpTime(int64_t lStartTime, int64_t lStopTime,
        StSdpTimeDesc &stTime);

    /**
    * @init StSdpMediaDesc.
    *
    * @param [in] ucMediaType, @ref EN_SDP_MEDIA_TYPE.
    * @param [in] ucProtoType, @ref EN_SDP_PROTO_TYPE.
    * @param [in] iPort.
    * @param [in] strFmtVec.
    *
    * @param [out] stMediaDesc.
    */
    static bool setArcSdpMediaDesc(unsigned char ucMediaType, unsigned char ucProtoType,
        int iPort, StSdpMediaDesc &stMediaDesc);

    /**
    * @set StSdpSessDesc.
    *
    * @param [in] iVer.
    * @param [in] strSessName.
    * @param [in] strInfo.
    * @param [in] strUri.
    * @param [in] strEmail.
    * @param [in] strPhone.
    *
    * @param [out] stSessDesc.
    */
    static bool setArcSdpSessDesc(int iVer, const std::string strSessName,
        const std::string &strInfo, const std::string strUri,
        const std::string &strEmail, const std::string &strPhone,
        StSdpSessDesc &stSessDesc);

    /**
    * @init StSdpSessDesc.
    *
    * @param [in] stOrigin.
    * @param [in] stConnInfo.
    * @param [in] stTime.
    *
    * @param [out] stSessDesc.
    */
    static bool setArcSdpSessDescX(StSdpOrigin *pstOrigin, StSdpConnInfo *pstConnInfo,
        StSdpTimeField *pstTime, StSdpSessDesc &stSessDesc);

    /**
    * @init ST_SDP_AF_LST.
    *
    * @param [out] stAfLst.
    */
    static bool initSdpAttrLst(ST_SDP_AF_LST &stAfLst);

    /**
    * @init ST_SDP_MDESC.
    *
    * @param [out] stMdesc.
    */
    static bool initSdpMediaDesc(ST_SDP_MDESC &stMdesc);

    /**
    * @init ST_SDP_SESS_DESC.
    *
    * @param [out] stSessDesc.
    */
    static bool initSdpSessDesc(ST_SDP_SESS_DESC &stSessDesc);

    /**
    * @create via Branch.
    *
    * @return std::string.
    */
    static std::string createBranch();

    /**
    * @create To tag.
    *
    * @return std::string.
    */
    static std::string createToTag();

    /**
    * @create From tag.
    *
    * @return std::string.
    */
    static std::string createFromTag();

    /**
    * @brief SIP parameter fill addr-spec by SIP URI.
    *
    * @param [in] zBufId Buffer request memory from.
    * @param [in] pstAddrSpec Addr-spec in SIP message.
    * @param [in] pstSipUri SIP URI.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    static ZINT Sip_ParmFillAddrSpecBySipUri(ZUBUF zBufId,ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_SIP_URI *pstSipUri);

    /**
    * @brief SIP message get request SIPS uri in request line.
    *
    * @param [in] pstMsg SIP message.
    * @param [out] ppstUri The request SIPS uri.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    static bool getReqSipsUri(ST_SIP_MSG *pstMsg, ST_SIP_SIP_URI **ppstUri);

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
    static bool getFromToAddrSpec(ST_SIP_MSG *pstMsg, ZBOOL bFromHdr, ST_SIP_ADDR_SPEC  & stAddrSpec);

    static bool UnCharVectorToArray(const vector<ZUCHAR> & ucVect, char aucPtLst[20]);

    /**
    * @brief SIP converter EN_SIP_OPT_TAG enum  to Name string
    *
    * @param [in] ucType EN_SIP_OPT_TAG enum
    * @param [out] strTypeName
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    static bool OptTagEnumToName(const ZUCHAR & ucType, std::string & strTypeName);

    /**
    * @brief SIP converter EN_SIP_METHOD enum  to Name string
    *
    * @param [in] ucType EN_SIP_OPT_TAG enum
    * @param [out] strTypeName
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    static bool OptMethodEnumToName(const ZUCHAR & ucType, std::string & strTypeName);


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
    static bool getFromToNaSpec(ST_SIP_MSG *pstMsg, ZBOOL bFromHdr, ST_SIP_NA_SPEC & stNaSpec);

    /**
    * @brief SIP message get name sipuri in From or To header.
    *
    * @param [in] pstMsg SIP message.
    * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
    * @param [out] ppstNaSpec Name-addr or addr-spec data strcuture.
    *
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    static bool getFromToSipUri(ST_SIP_MSG *pstMsg, ZBOOL bFromHdr, ST_SIP_SIP_URI ** ppstSipUri);

    static bool getContactSipUri(ST_SIP_MSG *pstMsg, ST_SIP_SIP_URI ** ppstSipUri);



    static bool getPPreaferdIDHeaderSipUri(ST_SIP_MSG *pstMsg,  ST_SIP_SIP_URI ** ppstSipUri);



    //   /* mmf session fill imdn message data  Refer to Mmf_SessFillImdnData*/
    //static bool SessFillImdnData(ST_SIP_MSG *pstMsg,ST_MSF_URI *pstLclUri, ST_MSF_URI *pstPartpUri, ZCHAR *pcMsg, ZUINT iMsgSize,
    //			ZCHAR *pcContType,ZCHAR **ppcData, ZUINT *piDataLen);

    ///* mmf fill cpim message with imdn request */
    //static bool FillCpimMsgImdnX(ZUBUF zMemBuf, ST_MSF_URI *pstLclUri, 	ST_MSF_URI *pstPartpUri, ZCHAR *pcBody, ZUINT iBoydSize,
    //	ST_ZOS_SYS_TIME *pstTime, ZUINT iImdnType, ZCHAR *pcCType,
    //	ZCHAR **ppcMsg, ZUINT *piMsgSize, ST_ZOS_SSTR *pstIMsgId);


    /**
    * @brief stmsfUri.

    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    static bool enMsfUri(const ST_ZOS_SSTR & stDispName, const ST_ZOS_SSTR & stUserUri, ST_MSF_URI & stmsfUri);
    static bool enMsfUri(const std::string & strDispName, const std::string & strMsfUri, ST_MSF_URI & stmsfUri);

    /**
    * @brief get the  Call-Id Host, .
    * @brief [in] strSipDialogIs , SipDialogId  == call-id + local tag + remote tag
    * @retval ZOK Operation successfully.
    * @retval ZFAILED Otherwise.
    */
    static bool GetCallIdNumberHost(const std::string & strSipDialogIs, std::string & strNumber, std::string & strHost);
    static bool GetCallIdNumberHost2(const std::string & strSipDialogIs, std::string & strNumber, std::string & strHost);

    static int CreateRandInt();

    /* get response code */
    static bool getRspStatusCode(ST_SIP_MSG *pstMsg, unsigned int &iRspCode);

    static  bool getMethod(ST_SIP_MSG *pstMsg, unsigned char &ucMethod);


    //md5 A1
    static bool calA1( const std::string & userName, const  std::string & passWord,const  std::string & realm, std::string & resultA1);

    //md5 A2
    static bool calA2(ZUCHAR ucMethod, ST_SIP_REQ_URI *pstDigestUri,std::string & resultA2);

    //md5 KD
    static bool calKD(ZCHAR acA1Md5Str[SIP_MD5_HEX_STR_LEN],ZCHAR acA2Md5Str[SIP_MD5_HEX_STR_LEN], ST_ZOS_SSTR *pstNonce, ST_ZOS_SSTR *pstCnonce,
        ST_ZOS_SSTR *pstNonceCount,ST_ZOS_SSTR *pstQop, std::string & acKDMd5Str);


    //generate new CNonce
    static bool createDigestCnonce(std::string & strCNnonce);

    static bool generateToSBCConnectionId(const std::string & SBCip, const int & SBCport, std::string & connctionId);
    static bool decodeToSBCConnectionId(const std::string & connctionId,  std::string & SBCip,  int & SBCport);
    static bool deleteInternationCode(const std::string & calleeNumber, std::string & newNumber);
    static bool getDomainIdAndAppId(const std::string & domainIdAndAppId, std::string & domainId, std::string & appId);
    static bool checkIpValid(const std::string & ip);
    static bool checkVideoCall(const std::string & sipMsg);

    static bool encodeRtpHostPorts(const std::string & ip, const vector<int> & evenPorts, std::string & hostPorts);
    static bool decodeRtpHostPorts(const std::string & hostPorts, std::string & ip,vector<int> & evenPorts);

    static std::string encodeMapParasLog(const map<std::string, std::string> & params);
    static std::string getCurrentDir();
    static bool jsonToMap(const std::string& s, std::map<std::string, std::string>& m);
    static std::string mapToJson(const std::map<std::string, std::string>& m);

    static bool setLogLevel(int level);
    static ZUINT toZLogLevel(int level);
};

#endif /* __SipMsgCommon_h */
