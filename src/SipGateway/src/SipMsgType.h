#ifndef __SipMsgType_h
#define __SipMsgType_h

#include <string>
#include <vector>

#define MAX_UNSIGNED_INT 65535  /* max unsigned int */

#define SIPMSG_RSP_100_TRYING                100 /**< @brief Response code of "100 trying". */
#define SIPMSG_RSP_180_RINGING               180 /**< @brief Response code of "180 ringing". */
#define SIPMSG_RSP_181_FORWARDED             181 /**< @brief Response code of "181 forwarded". */
#define SIPMSG_RSP_182_QUEUED                182 /**< @brief Response code of "182 queued". */
#define SIPMSG_RSP_183_SESS_PROGRESS         183 /**< @brief Response code of "183 session progress". */

#define SIPMSG_RSP_200_OK                    200 /**< @brief Response code of "200 OK". */
#define SIPMSG_RSP_202_ACPTED                202 /**< @brief Response code of "202 accepted". */
#define SIPMSG_RSP_300_MULT_CHOICES          300 /**< @brief Response code of "300 mult choices". */
#define SIPMSG_RSP_301_MOVED_PERM            301 /**< @brief Response code of "301 moved permanent". */
#define SIPMSG_RSP_302_MOVED_TEMP            302 /**< @brief Response code of "302 moved temporality". */
#define SIPMSG_RSP_305_USE_PROXY             305 /**< @brief Response code of "305 use proxy". */
#define SIPMSG_RSP_380_ALT_SERVICE           380 /**< @brief Response code of "380 alt service". */

#define SIPMSG_RSP_400_BAD_REQ               400 /**< @brief Response code of "400 bad request". */
#define SIPMSG_RSP_401_UNAUTHOR              401 /**< @brief Response code of "401 unauthorizationorization". */
#define SIPMSG_RSP_402_PAYMENT_REQUIRED      402 /**< @brief Response code of "402 payment requestuired". */
#define SIPMSG_RSP_403_FORBIDDEN             403 /**< @brief Response code of "403 forbidden". */
#define SIPMSG_RSP_404_NOT_FOUND             404 /**< @brief Response code of "404 not found". */
#define SIPMSG_RSP_405_METHOD_NOT_ALLOWED    405 /**< @brief Response code of "405 method not allowed". */
#define SIPMSG_RSP_406_NOT_ACPTED            406 /**< @brief Response code of "406 not accepted". */
#define SIPMSG_RSP_407_PROXY_AUTH_REQD       407 /**< @brief Response code of "407 proxy authentication required". */
#define SIPMSG_RSP_408_REQ_TIMEOUT           408 /**< @brief Response code of "408 request timeout". */
#define SIPMSG_RSP_409_CONFLICT              409 /**< @brief Response code of "409 conflict". */
#define SIPMSG_RSP_410_GONE                  410 /**< @brief Response code of "410 gone". */
#define SIPMSG_RSP_411_LEN_REQ               411 /**< @brief Response code of "411 length request". */
#define SIPMSG_RSP_412_COND_REQ_FAILED       412 /**< @brief Response code of "412 condition request failed". */
#define SIPMSG_RSP_413_REQ_ENTITY_TOO_LARGE  413 /**< @brief Response code of "413 request entity too large". */
#define SIPMSG_RSP_414_REQ_URI_TOO_LONG      414 /**< @brief Response code of "414 request uri too long". */
#define SIPMSG_RSP_415_UNSUPTED_MEDIA        415 /**< @brief Response code of "415 unsupported media". */
#define SIPMSG_RSP_416_UNSUPTED_URI_SCHM     416 /**< @brief Response code of "416 unsupported uri schm". */
#define SIPMSG_RSP_417_UNKNOWN_RES_PRI       417 /**< @brief Response code of "417 unknown res pri". */
#define SIPMSG_RSP_420_BAD_EXTN              420 /**< @brief Response code of "420 bad extension". */
#define SIPMSG_RSP_421_EXTN_REQ              421 /**< @brief Response code of "421 extension request". */
#define SIPMSG_RSP_422_SESSTIMER_TOO_SMALL   422 /**< @brief Response code of "422 session timer too small". */
#define SIPMSG_RSP_423_INTERVAL_TOO_BRIEF    423 /**< @brief Response code of "423 interval too brief". */
#define SIPMSG_RSP_428_USE_ID_HDR            428 /**< @brief Response code of "428 use ID header". */
#define SIPMSG_RSP_429_PROVIDE_REFER_ID      429 /**< @brief Response code of "429 provide refer ID". */
#define SIPMSG_RSP_436_BAD_ID_INFO           436 /**< @brief Response code of "436 bad ID information". */
#define SIPMSG_RSP_437_UNSUPT_CERT           437 /**< @brief Response code of "437 unsupported certification". */
#define SIPMSG_RSP_438_INVAL_ID_HDR          438 /**< @brief Response code of "438 invalid ID header". */
#define SIPMSG_RSP_480_TEMP_UNAVAIL          480 /**< @brief Response code of "480 temporality unavailable". */
#define SIPMSG_RSP_481_CLEG_TRANS_NOT_EXIST  481 /**< @brief Response code of "481 call leg trans not exist". */
#define SIPMSG_RSP_482_LOOP_DETECTED         482 /**< @brief Response code of "482 loop detected". */
#define SIPMSG_RSP_483_TOO_MANY_HOPS         483 /**< @brief Response code of "483 too many hops". */
#define SIPMSG_RSP_484_ADDR_INCOMPLETE       484 /**< @brief Response code of "484 address incomplete". */
#define SIPMSG_RSP_485_AMBIGUOUS             485 /**< @brief Response code of "485 ambiguous". */
#define SIPMSG_RSP_486_BUSY_HERE             486 /**< @brief Response code of "486 busy here". */
#define SIPMSG_RSP_487_REQ_TERMINATED        487 /**< @brief Response code of "487 request terminated". */
#define SIPMSG_RSP_488_NOT_ACPT_HERE         488 /**< @brief Response code of "488 not accept here". */
#define SIPMSG_RSP_489_BAD_EVNT              489 /**< @brief Response code of "489 bad event". */
#define SIPMSG_RSP_491_REQ_PENDING           491 /**< @brief Response code of "491 request pending". */
#define SIPMSG_RSP_493_UNDECIPHERABLE        493 /**< @brief Response code of "493 undecipherable". */
#define SIPMSG_RSP_494_SEC_AGREE_REQ         494 /**< @brief Response code of "494 second agree request". */

#define SIPMSG_RSP_500_INTERNAL_ERR          500 /**< @brief Response code of "500 internal error". */
#define SIPMSG_RSP_501_NOT_IMP               501 /**< @brief Response code of "501 not implement". */
#define SIPMSG_RSP_502_BAD_GATEWAY           502 /**< @brief Response code of "502 bad gateway". */
#define SIPMSG_RSP_503_SERVICE_UNAVAIL       503 /**< @brief Response code of "503 service unavailable". */
#define SIPMSG_RSP_504_SERVER_TIMEOUT        504 /**< @brief Response code of "504 server timeout". */
#define SIPMSG_RSP_505_VER_UNSUPTED          505 /**< @brief Response code of "505 version unsupported". */
#define SIPMSG_RSP_513_MSG_TOO_LARGE         513 /**< @brief Response code of "513 message too large". */
#define SIPMSG_RSP_533_SERVICE_LOST          533 /**< @brief Response code of "533 service lost". */
#define SIPMSG_RSP_580_PRECON_FAIL           580 /**< @brief Response code of "580 precondition fail". */

#define SIPMSG_RSP_600_BUSY_EVERYWHERE       600 /**< @brief Response code of "600 busy everywhere". */
#define SIPMSG_RSP_603_DECLINE               603 /**< @brief Response code of "603 decline". */
#define SIPMSG_RSP_604_NOT_EXIST_ANYWHERE    604 /**< @brief Response code of "604 not exist anywhere". */
#define SIPMSG_RSP_606_NOT_ACPT              606 /**< @brief Response code of "606 not accept". */

/* contact cap */
#define CT_CAP_IM        0x00000001     /* instant message */
#define CT_CAP_FT        0x00000002     /* file transfer */
#define CT_CAP_IS        0x00000004     /* image share */
#define CT_CAP_VS        0x00000008     /* video share */
#define CT_CAP_SP        0x00000010     /* social information via presence */
#define CT_CAP_DP        0x00000020     /* capability discovery via presence */
#define CT_CAP_STDMSG    0x00000040     /* standalone messaging */
#define CT_CAP_VSOUTVC   0x00000080     /* video share outside of a voice call */
#define CT_CAP_VOICECALL 0x00000100     /* ip voice call */
#define CT_CAP_VIDEOCALL 0x00000200     /* ip video call */
#define CT_CAP_GEOPULL   0x00000400     /* geolocation pull */
#define CT_CAP_GEOPUSH   0x00000800     /* geolocation push */
#define CT_CAP_FSTFWDGP  0x00001000     /* full store and forward for group chat */
#define CT_CAP_FTTHUMB   0x00002000     /* file transfer thumbnail */
#define CT_CAP_FTSTFWD   0x00004000     /* file transfer store and forward */
#define CT_CAP_FTHTTP    0x00008000     /* file transfer via HTTP */
#define CT_CAP_GEOPULLFT 0x00010000     /* geolocation pull using file transfer */
#define CT_CAP_BURN      0x00020000     /* brief burn after reading function */
#define CT_CAP_PUB_MSG   0x00040000     /* public message */
#define CT_CAP_CLOUD_FILE 0x00080000    /* cloud file */
#define CT_CAP_VEM       0x00100000     /* vemoticon */
#define CT_CAP_GRP_MAN   0x00200000     /* group manage */
#define CT_CAP_CPM_MSG   0x00400000     /* cpm message */
#define CT_CAP_CPM_LARGE 0x00800000     /* cpm large message */
#define CT_CAP_CPM_FT    0x01000000     /* cpm file transfer */
#define CT_CAP_CPM_SESS  0x02000000     /* cpm session */

/* sip message app ref */
#define SIP_SER_REF_STD_MSG "urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.msg;urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.largemsg"
#define SIP_APP_REF_RCSE_IM "urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.im"
#define SIP_APP_REF_RCS_FSFGP "urn%3Aurn-7%3A3gppapplication.ims.iari.rcs.fullsfgroupchat"
#define SIP_APP_REF_RCSE_FT "urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.ft"
#define SIP_APP_REF_RCS_FTTB "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.ftthumb"
#define SIP_APP_REF_RCS_FTHP "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.fthttp"
#define SIP_APP_REF_RCS_FTSF "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.ftstandfw"
#define SIP_APP_REF_GSMA_IS "urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-is"
#define SIP_APP_REF_GSMA_VS "urn:urn-7:3gpp-application.ims.iari.gsma-vs"
#define SIP_APP_REF_RCSE_SP "urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.sp"
#define SIP_APP_REF_RCSE_DP "urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.dp"
#define SIP_SER_REF_MMTEL_VOC "urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
#define SIP_APP_REF_RCS_GPL "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.geopull"
#define SIP_APP_REF_RCS_GPLFT "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.geopullft"
#define SIP_APP_REF_RCS_GPS "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.geopush"
#define SIP_APP_REF_PUB_MSG "urn%3Aurn-7%3A3gpp-application.ims.icsi.rcs.mnc000.mcc460.publicmsg"
#define SIP_APP_REF_CLOUD_FILE "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.mnc000.mcc460.cloudfile;version=1_0"
#define SIP_APP_REF_VEM "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.mnc000.mcc460.vemoticon;version=1_0"
#define SIP_APP_REF_GRP_MAN "urn%3Aurn-7%3A3gpp-application.ims.iari.rcs.mnc000.mcc460.gpmanage;vs=1"
#define SIP_SER_REF_CPM_MSG "urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.msg"
#define SIP_SER_REF_CPM_LARGE "urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.largemsg"
#define SIP_SER_REF_CPM_FT "urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.filetransfer"
#define SIP_SER_REF_SESS "urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.session"

/* sip presence service id */
#define SIP_PRES_SRV_STANDAL_MSG    "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcs.sm"
#define SIP_PRES_SRV_IM_SESS        "org.openmobilealliance:IM-session"
#define SIP_PRES_SRV_STANDFW_GRP_CHAT "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcs.fullsfgroupchat"
#define SIP_PRES_SRV_FILE_TRSF      "org.openmobilealliance:File-Transfer"
#define SIP_PRES_SRV_FT_THUMB       "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcs.ftthumb"
#define SIP_PRES_SRV_FT_VIA_HTTP    "org.openmobilealliance:File-Transfer-HTTP"
#define SIP_PRES_SRV_IMG_SHARE      "org.gsma.imageshare"
#define SIP_PRES_SRV_VIDEO_SHARE    "org.gsma.videoshare"
#define SIP_PRES_SRV_SPI            "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcse.sp"
#define SIP_PRES_SRV_CAP_VIA_PRES   "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcse.dp"
#define SIP_PRES_SRV_IP_CALL        "org.3gpp.urn:urn-7:3gpp-service.ims.icsi.mmtel"
#define SIP_PRES_SRV_GEO_PULL       "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcs.geopull"
#define SIP_PRES_SRV_GEO_PULL_VIA_FT "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcs.geopullft"
#define SIP_PRES_SRV_GEO_PUSH       "org.3gpp.urn:urn-7:3gpp-application.ims.iari.rcs.geopush"

#define SIP_SDP_RED_PT_MAX 13   /* max red payload list */

typedef enum EnSipMsgType 
{
    EnSipMsgType_Invalid,
    EnSipMsgType_Incomplete, //未找到 "\r\n\r\n"
    EnSipMsgType_PingRFC5626,
    EnSipMsgType_PongRFC5626,
    EnSipMsgType_Request,
    EnSipMsgType_RegRsp100,
    EnSipMsgType_RegRsp180,
    EnSipMsgType_RegRsp200,
    EnSipMsgType_RegRsp202,
    EnSipMsgType_RegRsp401,
    EnSipMsgType_RegRsp480,
    EnSipMsgType_RegRsp486,
    EnSipMsgType_RegRsp487,
    EnSipMsgType_RegRspOther,
    EnSipMsgType_Response,
    EnSipMsgType_Invite
} EnSipMsgType;


typedef enum EnSipReqMethedType
{
    EnSipReqMethedType_Register = 50,
    EnSipReqMethedType_Subscribe,
    EnSipReqMethedType_Notify,
    EnSipReqMethedType_Options,
    EnSipReqMethedType_Invite,
    EnSipReqMethedType_Message,
    EnSipReqMethedType_ACK,
    EnSipReqMethedType_CANCEL

}EnSipReqMethedType;





typedef enum EnSipMsgMediaType
{
    EnSipMsgMediaType_Unknown,
    EnSipMsgMediaType_Voice,
    EnSipMsgMediaType_Video
} EnSipMsgMediaType;

/* struct of Authorization */
typedef struct StSipAuthor
{
    unsigned char _ucDigst;
    unsigned char _ucAlgo;  /* see type EN_SIP_ALGO_VAL */
    unsigned char _ucQop;   /* see type EN_SIP_QOP_VAL */
    std::string _strUsername;
    std::string _strRealm;
    std::string _strNonce;
    std::string _strResponse;
    std::string _strOpaque;
    std::string _strUri;
    std::string _strCnonce;
    std::string _strNc;
    unsigned char _ucReqUri;
    ST_SIP_REQ_URI _stReqUri;
} StSipAuthor;

/* struct of sdp origin-field */
typedef struct StSdpOrigin
{
    unsigned char _ucNetType;       /* nettype EN_SDP_NET_TYPE */
    unsigned char _ucAddrType;      /* addrtype EN_SDP_ADDR_TYPE */
    std::string _strUsername;
    int64_t _lSessId;
    int64_t _lSessVer;
    std::string _strAddr;
} StSdpOrigin;

/* struct of sdp connection info */
typedef struct StSdpConnInfo
{
    unsigned char _ucNetType;       /* net type EN_SDP_NET_TYPE */
    unsigned char _ucAddrType;      /* addr type EN_SDP_ADDR_TYPE */
    unsigned char _aucSpare[2];     /* for 32 bit alignment */
    std::string _strAddr;        /* connection-address */
} StSdpConnInfo;

/* struct of sdp time description */
typedef struct StSdpTimeDesc
{
    int64_t _lStartTime;
    int64_t _lStopTime;
} StSdpTimeDesc;

typedef struct StSdpTimeField
{
    unsigned long _lStartTime;
    unsigned long _lStopTime;
} StSdpTimeField;

/* typedef of audio codec */
typedef struct StSdpRed
{
    StSdpRed()
    {
        ucPtCnt = 0;
        ucRedPt = ZMAXUCHAR;
        ucFecPt = ZMAXUCHAR;
    };

    unsigned char ucRedPt;                  /* red payload type */
    unsigned char ucFecPt;                  /* fec payload type */
    unsigned char ucPtCnt;                  /* red payload count */
    unsigned char aucPtLst[SIP_SDP_RED_PT_MAX]; /* red payload list */
} StSdpRed;

typedef struct StSdpCodec
{
    StSdpCodec()
    {
        iPayload = ZMAXUCHAR;
        iBitRate = 0;
        redPres = false;
    }

    ZUCHAR ucEncoding;
    std::string pcName;          /* codec name */
    int iPayload;                  /* codec payload */
    int iBitRate;                  /* codec bitrate */
    bool redPres;
    StSdpRed stRed;

    union {
        ZBOOL bG729Annexb;           /* g729 annex b option */
        ZUINT iIlbcMode;             /* ilbc mode option */
        //ST_MNF_AMR_OPT stAmrOpt;     /* amr options */
    } u;
} StSdpCodec;

/* struct of sdp media description */
typedef struct StSdpMediaDesc
{
    unsigned char _ucMediaType;         /* known media EN_SDP_MEDIA_TYPE */
    unsigned char _ucProtoType;         /* known proto EN_SDP_PROTO_TYPE */
    int _iPort;                         /* port */
    int _iPortNum;                         /* port */

    std::vector<unsigned char> _strFmtVec;  /* media format vector */

    std::string _strInfo;            /* in */
    //std::vector<StSdpConnInfo> _stConnVec; /* connection info */
    //                                  /* bandwidth info */
    std::string _strKey;             /* key */
    std::vector<std::string> _strAttrVec; /* attribute vector */
    std::vector<StSdpCodec> _vecCodec;     /* media description, must field, m= */
    StSdpCodec _redCodec;
    StSdpCodec _dtmfCodec;
} StSdpMediaDesc;

typedef struct StSdpAttr
{
    unsigned char ucType;               /* attribute type EN_SDP_ATTR_TYPE */
} StSdpAttr;

/* struct of sdp session description */
typedef struct StSdpSessDesc
{
    StSdpSessDesc()
    {
        ucDirect = EN_SDP_DIRECT_TYPE_SENDRECV;
        redPres = false;
        ucRtcpMux = false;
        _iVer = 0;
        ucFrameRate = 0;
        ptime = 0;
        maxPtime = 0;
    }

    int _iVer;                              /* version, must field, v= */
    StSdpOrigin _stOrigin;                  /* origin, must field, o= */
    std::string _strSessName;            /* session name, must field, s= */
    std::string _strInfo;                /* info, i= */
    std::string _strUri;                 /* uri, u= */
    std::vector<std::string> _strEmailVec;    /* email, e= */
    std::vector<std::string> _strPhoneVec;    /* phone, p= */
    StSdpConnInfo _stConnInfo;              /* connection info, c =  */
    /* bandwidth info */
    StSdpTimeDesc _stTimeDesc;              /* time description, must field t= */ /* not use */
    StSdpTimeField _stTimeField;            /* t= */
    std::string _strKey;                 /* key-field */
    std::vector<StSdpAttr> _stAttrVec;           /* session attribute, a= */
    std::vector<StSdpMediaDesc> _stMediaVec;     /* media description, must field, m= */
    int ptime;                              /* ptime */
    int maxPtime;                           /* maxptime */ 
    ZUCHAR ucDirect;                        /* direct, EN_SDP_DIRECT_TYPE_TYPE */
    ZUCHAR ucRecPref;                       /* record prefrence EN_SDP_ATTR_REC_PREF_TYPE */
    bool ucRtcpMux;
    int ucFrameRate;
    bool redPres;
} StSdpSessDesc;

/* attribute-field of amr fmtp,mode-set  */
#define EN_SDP_ATTR_FMTP_MODE_SET_0        0x00000001
#define EN_SDP_ATTR_FMTP_MODE_SET_1        0x00000002
#define EN_SDP_ATTR_FMTP_MODE_SET_2        0x00000004
#define EN_SDP_ATTR_FMTP_MODE_SET_3        0x00000008
#define EN_SDP_ATTR_FMTP_MODE_SET_4        0x00000010
#define EN_SDP_ATTR_FMTP_MODE_SET_5        0x00000020
#define EN_SDP_ATTR_FMTP_MODE_SET_6        0x00000030

// Xml Node Attibute structure
struct XmlNodeAttribute
{
    std::string _AttrName;
    std::string _AttrValue;
    XmlNodeAttribute(std::string attrName, std::string attrValue)
    {
        _AttrName = attrName;
        _AttrValue = attrValue;
    }

    XmlNodeAttribute(  std::string attrValue)
    {
        _AttrValue = attrValue;
    }
};

// Xml Node Structure
struct XmlNode
{
    std::string _strTagName;
    std::string _strTxtValue;
    std::vector<XmlNodeAttribute*> _vectAttr;
    std::vector<XmlNode> _vectChlidNodes;
};

/* msf uri */
struct ST_MSF_URI
{
    ST_ZOS_SSTR stDispName;          /* display name */
    ST_ZOS_SSTR stUserUri;           /* user uri */
};

/**< @brief mmf messaging session message IMDN type */
typedef enum EN_MMF_MSESS_IMDN_TYPE
{
    EN_MMF_MSESS_IMDN_UNKNOWN = 100,
    /**< @brief unknown type */
    EN_MMF_MSESS_IMDN_REG,           /**< @brief regular message */
    EN_MMF_MSESS_IMDN_DELI,          /**< @brief delivery notification */
    EN_MMF_MSESS_IMDN_DISP,          /**< @brief display notification */
    EN_MMF_MSESS_IMDN_BOTH,          /**< @brief delivery & display notification */
} EN_MMF_MSESS_IMDN_TYPE;

//define struct to contain SBC media info
typedef struct SDPMediaPort
{
    bool                            _bIsAudio;          //是否音频
    bool                            _bMediaForbit;      //如果端口为0，则表示拒绝该媒体流
    int                             _port;              //端口

}SDPMediaPort;

typedef struct SDPMediaInfo
{
    std::string                  _strIp;             //ip
    std::vector<SDPMediaPort>            _vecMediaPort;      //媒体流以及对应端口

}SDPMediaInfo;

#endif /* __SipMsgType_h */


