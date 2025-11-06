/************************************************************************

        Copyright (c) 2005-2007 by Juphoon System Software, Inc.
                       All rights reserved.

     This software is confidential and proprietary to Juphoon System,
     Inc. No part of this software may be reproduced, stored, transmitted,
     disclosed or used in any form or by any means other than as expressly
     provided by the written license agreement between Juphoon and its
     licensee.

     THIS SOFTWARE IS PROVIDED BY JUPHOON "AS IS" AND ANY EXPRESS OR 
     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     ARE DISCLAIMED. IN NO EVENT SHALL JUPHOON BE LIABLE FOR ANY DIRECT, 
     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
     OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
     POSSIBILITY OF SUCH DAMAGE. 

                    Juphoon System Software, Inc.
                    Email: support@juphoon.com
                    Web: http://www.juphoon.com

************************************************************************/
/*************************************************
  File name     : http_abnf_type.h
  Module        : http protocol abnf typedef, structures and functions
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-03-30
  Description   :
    Marcos and structure definitions required by the http protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTP_ABNF_TYPE_H__
#define _HTTP_ABNF_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************** macros or enum values definition ******************/

/* headers type */
typedef enum EN_HTTP_HDR
{
    /* rfc2616 headers */
    EN_HTTP_HDR_ACPT = 0,
    EN_HTTP_HDR_ACPT_CHRSET,
    EN_HTTP_HDR_ACPT_ENCODING,
    EN_HTTP_HDR_ACPT_LANG,
    EN_HTTP_HDR_ACPT_RANGES,
    EN_HTTP_HDR_AGE,
    EN_HTTP_HDR_ALLOW,
    EN_HTTP_HDR_AUTHEN_INFO,
    EN_HTTP_HDR_AUTHOR,
    EN_HTTP_HDR_CACHE_CTRL,
    EN_HTTP_HDR_CONN,
    EN_HTTP_HDR_CONTENT_ENCODING,
    EN_HTTP_HDR_CONTENT_LANG,
    EN_HTTP_HDR_CONTENT_LEN,
    EN_HTTP_HDR_CONTENT_LOCAT,
    EN_HTTP_HDR_CONTENT_MD5,
    EN_HTTP_HDR_CONTENT_RANGE,
    EN_HTTP_HDR_CONTENT_TYPE,
    EN_HTTP_HDR_CONTENT_DSP,
    EN_HTTP_HDR_DATE,
    EN_HTTP_HDR_ETAG,
    EN_HTTP_HDR_EXPECT,
    EN_HTTP_HDR_EXPIRES,
    EN_HTTP_HDR_FROM,
    EN_HTTP_HDR_HOST,
    EN_HTTP_HDR_IF_MATCH,
    EN_HTTP_HDR_IF_MDFY_SINCE,
    EN_HTTP_HDR_IF_NO_MATCH,
    EN_HTTP_HDR_IF_RANGE,
    EN_HTTP_HDR_IF_UNMDFY_SINCE,
    EN_HTTP_HDR_LAST_MDFY,
    EN_HTTP_HDR_LOCAT,
    EN_HTTP_HDR_MAX_FORWARDS,
    EN_HTTP_HDR_PRAGMA,
    EN_HTTP_HDR_PROXY_AUTHEN,
    EN_HTTP_HDR_PROXY_AUTHOR,
    EN_HTTP_HDR_RANGE,
    EN_HTTP_HDR_REFERER,
    EN_HTTP_HDR_RETRY_AFTER,
    EN_HTTP_HDR_SERV,
    EN_HTTP_HDR_TE,
    EN_HTTP_HDR_TRAILER,
    EN_HTTP_HDR_TRSF_ENCODING,
    EN_HTTP_HDR_UPGRADE,     
    EN_HTTP_HDR_USER_AGENT,
    EN_HTTP_HDR_VARY,
    EN_HTTP_HDR_VIA,
    EN_HTTP_HDR_WARN,
    EN_HTTP_HDR_WWW_AUTHEN,
    /* OMA-TS-XDM_Core-V1_0_1 */
    EN_HTTP_HDR_X_XCAP_AID,    
    /* 3GPP TS 24.109 */
    EN_HTTP_HDR_X_3GPP_IID,    
    EN_HTTP_HDR_X_3GPP_AID,    
    EN_HTTP_HDR_X_3GPP_AFLAG,
    /* W3 SOAP */
    EN_HTTP_HDR_SOAP_ACT,
    /* extension-header */
    EN_HTTP_HDR_EXT    
} EN_HTTP_HDR;

/* method type */
typedef enum EN_HTTP_METHOD
{
    EN_HTTP_METHOD_OPTIONS = 0,
    EN_HTTP_METHOD_GET = 1,
    EN_HTTP_METHOD_HEAD = 2,
    EN_HTTP_METHOD_POST = 3,
    EN_HTTP_METHOD_PUT = 4,
    EN_HTTP_METHOD_DEL = 5,
    EN_HTTP_METHOD_TRACE = 6,
    EN_HTTP_METHOD_CONN = 7,
    /* extension methods */
    EN_HTTP_METHOD_EXT
} EN_HTTP_METHOD;

/* host type */
typedef enum EN_HTTP_HOST
{
    EN_HTTP_HOST_NAME,
    EN_HTTP_HOST_IPV4,
    EN_HTTP_HOST_IPV6        
} EN_HTTP_HOST;

/* relativeURI type */
typedef enum EN_HTTP_RELAT_URI
{
    EN_HTTP_RELAT_URI_NET,
    EN_HTTP_RELAT_URI_ABS,
    EN_HTTP_RELAT_URI_REL        
} EN_HTTP_RELAT_URI;

/* Request-URI type */
typedef enum EN_HTTP_REQ_URI
{
    EN_HTTP_REQ_URI_ALL,
    EN_HTTP_REQ_URI_ABSO,
    EN_HTTP_REQ_URI_ABS_PATH,
    EN_HTTP_REQ_URI_AUTHOR,
    EN_HTTP_REQ_URI_STR
} EN_HTTP_REQ_URI;

/* m-type type */
typedef enum EN_HTTP_MTYPE
{
    EN_HTTP_MTYPE_DISC_TEXT,
    EN_HTTP_MTYPE_DISC_IMAGE,
    EN_HTTP_MTYPE_DISC_AUDIO,
    EN_HTTP_MTYPE_DISC_VIDEO,
    EN_HTTP_MTYPE_DISC_APP,
    EN_HTTP_MTYPE_COMP_MSG,
    EN_HTTP_MTYPE_COMP_MULTI,
    EN_HTTP_MTYPE_EXT
} EN_HTTP_MTYPE;

/* m-subtype type */
typedef enum EN_HTTP_MSUBTYPE
{
    EN_HTTP_MSUBTYPE_PLAIN,
    EN_HTTP_MSUBTYPE_HTTP,
    EN_HTTP_MSUBTYPE_OCTET_STRM,
    EN_HTTP_MSUBTYPE_XML,
    EN_HTTP_MSUBTYPE_PUBGRP_XML,
    EN_HTTP_MSUBTYPE_PUBGRP_LST_XML,
    EN_HTTP_MSUBTYPE_AUTH_POLICY_XML,
    EN_HTTP_MSUBTYPE_XCAP_EL,
    EN_HTTP_MSUBTYPE_XCAP_EL_XML,
    EN_HTTP_MSUBTYPE_XCAP_ATT_XML,
    EN_HTTP_MSUBTYPE_XCAP_NS_XML,
    EN_HTTP_MSUBTYPE_XCAP_ERR_XML,
    EN_HTTP_MSUBTYPE_XCAP_CAPS_XML,    
    EN_HTTP_MSUBTYPE_XCAP_DIFF_XML,    
    EN_HTTP_MSUBTYPE_RES_LSTS_XML,    
    EN_HTTP_MSUBTYPE_RLS_SRV_XML,
    EN_HTTP_MSUBTYPE_OMA_XCAP_DIR_XML,
    EN_HTTP_MSUBTYPE_OMA_DIR_XML,
    EN_HTTP_MSUBTYPE_OMA_VND_XCAP_DIR_XML,
    EN_HTTP_MSUBTYPE_OMA_VND_GRP_USG_LST_XML,
    EN_HTTP_MSUBTYPE_OMA_VND_POC_GRPS_XML,
    EN_HTTP_MSUBTYPE_OMA_VND_PRES_CTT_XML,
    EN_HTTP_MSUBTYPE_OMA_VND_SEARCH_XML,
    EN_HTTP_MSUBTYPE_OMA_VND_CAB_PCC_XML,
    EN_HTTP_MSUBTYPE_PCC_CTT_XML,
    EN_HTTP_MSUBTYPE_PIDF_XML,
    EN_HTTP_MSUBTYPE_SOAP_XML,
    EN_HTTP_MSUBTYPE_JSON,
    EN_HTTP_MSUBTYPE_EXT
} EN_HTTP_MSUBTYPE;

/* acceptable-ranges type */
typedef enum EN_HTTP_ARANGE
{
    EN_HTTP_ARANGE_BYTES,
    EN_HTTP_ARANGE_NONE,
    EN_HTTP_ARANGE_OTHER
} EN_HTTP_ARANGE;

/* disposition type */
typedef enum EN_HTTP_DSP_TYPE
{
    EN_HTTP_DSP_ATTACHMENT,
    EN_HTTP_DSP_EXT
} EN_HTTP_DSP_TYPE;

/* ainfo type */
typedef enum EN_HTTP_AINFO
{
    EN_HTTP_AINFO_NEXTNONCE,
    EN_HTTP_AINFO_MSG_QOP,
    EN_HTTP_AINFO_RSP_AUTH,
    EN_HTTP_AINFO_CNONCE,
    EN_HTTP_AINFO_NONCE_COUNT,
    EN_HTTP_AINFO_AUTH_PARM
} EN_HTTP_AINFO;

/* algorithm type */
typedef enum EN_HTTP_ALGO
{
    EN_HTTP_ALGO_MD5,
    EN_HTTP_ALGO_MD5_SESS,
    EN_HTTP_ALGO_AKAv1_MD5,
    EN_HTTP_ALGO_OTHER
} EN_HTTP_ALGO;

/* dig-resp type */
typedef enum EN_HTTP_DIG_RSP
{
    EN_HTTP_DIG_RSP_USERNAME,
    EN_HTTP_DIG_RSP_REALM,
    EN_HTTP_DIG_RSP_NONCE,
    EN_HTTP_DIG_RSP_DIGEST_URI,
    EN_HTTP_DIG_RSP_DRESP,
    EN_HTTP_DIG_RSP_ALGO,
    EN_HTTP_DIG_RSP_CNONCE,
    EN_HTTP_DIG_RSP_OPAQUE,
    EN_HTTP_DIG_RSP_MSG_QOP,
    EN_HTTP_DIG_RSP_NONCE_COUNT,
    EN_HTTP_DIG_RSP_AUTH_PARM
} EN_HTTP_DIG_RSP;

/* wkday type */
typedef enum EN_HTTP_WKDAY
{
    EN_HTTP_WKDAY_SUN,
    EN_HTTP_WKDAY_MON,
    EN_HTTP_WKDAY_TUE,
    EN_HTTP_WKDAY_WEB,
    EN_HTTP_WKDAY_THU,
    EN_HTTP_WKDAY_FRI,
    EN_HTTP_WKDAY_SAT
} EN_HTTP_WKDAY;

/* month type */
typedef enum EN_HTTP_MONTH
{
    EN_HTTP_MONTH_JAN,
    EN_HTTP_MONTH_FEB,
    EN_HTTP_MONTH_MAR,
    EN_HTTP_MONTH_APR,
    EN_HTTP_MONTH_MAY,
    EN_HTTP_MONTH_JUN,
    EN_HTTP_MONTH_JUL,
    EN_HTTP_MONTH_AUG,
    EN_HTTP_MONTH_SEP,
    EN_HTTP_MONTH_OCT,
    EN_HTTP_MONTH_NOV,
    EN_HTTP_MONTH_DEC
} EN_HTTP_MONTH;

/* qop-value type */
typedef enum EN_HTTP_QOP_VAL
{
    EN_HTTP_QOP_VAL_AUTH,
    EN_HTTP_QOP_VAL_AUTH_INT,
    EN_HTTP_QOP_VAL_OTHER
} EN_HTTP_QOP_VAL;

/* digest-cln type */
typedef enum EN_HTTP_DIGEST_CLN
{
    EN_HTTP_DIGEST_CLN_REALM,
    EN_HTTP_DIGEST_CLN_DOMAIN,
    EN_HTTP_DIGEST_CLN_NONCE,
    EN_HTTP_DIGEST_CLN_OPAQUE,
    EN_HTTP_DIGEST_CLN_STALE,
    EN_HTTP_DIGEST_CLN_ALGO,
    EN_HTTP_DIGEST_CLN_QOPTS,
    EN_HTTP_DIGEST_CLN_AUTH_PARM
} EN_HTTP_DIGEST_CLN;

/* stale type */
typedef enum EN_HTTP_STALE
{
    EN_HTTP_STALE_TRUE,
    EN_HTTP_STALE_FALSE
} EN_HTTP_STALE;

/* cache-directive type */
typedef enum EN_HTTP_DIRECT
{
    EN_HTTP_DIRECT_PUBLIC,
    EN_HTTP_DIRECT_PRIVATE,
    EN_HTTP_DIRECT_NO_CACHE,
    EN_HTTP_DIRECT_NO_STORE,
    EN_HTTP_DIRECT_NO_TRANSF,
    EN_HTTP_DIRECT_MAX_AGE,
    EN_HTTP_DIRECT_S_MAX_AGE,
    EN_HTTP_DIRECT_MAX_STALE,
    EN_HTTP_DIRECT_MIN_FRESH,
    EN_HTTP_DIRECT_ONLY_IF_CACHE,
    EN_HTTP_DIRECT_MUST_REVAIL,
    EN_HTTP_DIRECT_PROXY_REVAIL,
    EN_HTTP_DIRECT_POST_CHECK,
    EN_HTTP_DIRECT_PRE_CHECK,
    EN_HTTP_DIRECT_EXTN
} EN_HTTP_DIRECT;

/* transport type */
typedef enum EN_HTTP_TPT
{
    EN_HTTP_TPT_UDP,
    EN_HTTP_TPT_TCP,
    EN_HTTP_TPT_TLS,
    EN_HTTP_TPT_SCTP,
    EN_HTTP_TPT_OTHER
} EN_HTTP_TPT;

/* transport status */
typedef enum EN_HTTP_TRANS_STAT
{
    EN_HTTP_TRANS_IDLE,
    EN_HTTP_TRANS_SENDING,
    EN_HTTP_TRANS_PRE_DONE,
    EN_HTTP_TRANS_DONE,
    EN_HTTP_TRANS_ERR,
} EN_HTTP_TRANS_STAT;

/***************************** rfc2616 typedefs *************************/

/* http structure string */
typedef ST_ZOS_SSTR ST_HTTP_SSTR;

/* list of string */
ABNF_TYPEDEF_LIST(HTTP_SSTR); 

/* http some type value */
typedef struct tagHTTP_TYPE_VAL
{
    ZUCHAR ucType;                   /* type value EN_HTTP_QOP_VAL */  
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /* extension or unknown type string */
} ST_HTTP_TYPE_VAL;

/* list of type value */
ABNF_TYPEDEF_LIST(HTTP_TYPE_VAL);

/* typedef Method */
typedef ST_HTTP_TYPE_VAL ST_HTTP_METHOD; /* EN_HTTP_METHOD */

/* list of Method */
ABNF_TYPEDEF_LIST(HTTP_METHOD); 

/* http userinfo */
typedef struct tagHTTP_USER_INFO
{
    ZUCHAR ucPasswdPres;             /* password present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUser;              /* user / telephone-subscriber */
    ST_ZOS_SSTR stPasswd;            /* password */
} ST_HTTP_USER_INFO;

/* http host */
typedef struct tagHTTP_HOST
{
    ZUCHAR ucType;                   /* host type EN_HTTP_HOST */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stName;          /* hostname */
        ZUINT iIpv4Addr;             /* IPv4address */
        ZUCHAR aucIpv6Addr[ZINET_IPV6_ADDR_SIZE]; /* IPv6reference */
    } u;
} ST_HTTP_HOST;

/* http hostport */
typedef struct tagHTTP_HOST_PORT
{
    ZUCHAR ucPortPres;               /* Port present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_HOST stHost;             /* host */
    ZUINT iPort;                     /* port */
} ST_HTTP_HOST_PORT;

/* http srvr */
typedef struct tagHTTP_SRVR
{
    ZUCHAR ucUserInfoPres;           /* userinfo present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_USER_INFO stUserInfo;    /* userinfo */
    ST_HTTP_HOST_PORT stHostPort;    /* hostport */
} ST_HTTP_SRVR;

/* http authority */
typedef struct tagHTTP_AUTHOR
{
    ZUCHAR ucSrvrPres;               /* srvr present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_SRVR stSrvr;         /* srvr */
        ST_HTTP_SSTR stRegName;      /* reg-name */
    } u;
} ST_HTTP_AUTHOR;

/* http segment */
typedef struct tagHTTP_SEGMENT
{
    ST_ZOS_SSTR stPchar;             /* pchar */
    ST_HTTP_SSTR_LST stParmLst;      /* param list */
} ST_HTTP_SEGMENT;

/* list of segment */
ABNF_TYPEDEF_LIST(HTTP_SEGMENT);

/* http abs-path */
typedef struct tagHTTP_ABS_PATH
{
    ST_HTTP_SEGMENT_LST stSegmentLst; /* segment list */
} ST_HTTP_ABS_PATH;

/* http net_path */
typedef struct tagHTTP_NET_PATH
{
    ZUCHAR ucAbsPathPres;            /* abs_path present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_AUTHOR stAuthor;         /* authority */
    ST_HTTP_ABS_PATH stAbsPath;      /* abs_path */
} ST_HTTP_NET_PATH;

/* http rel_path */
typedef struct tagHTTP_REL_PATH
{
    ZUCHAR ucAbsPathPres;            /* abs_path present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_SSTR stSegment;          /* rel_segment */
    ST_HTTP_ABS_PATH stAbsPath;      /* abs_path */
} ST_HTTP_REL_PATH;

/* http hier_part */
typedef struct tagHTTP_HIER_PART
{
    ZUCHAR ucAbsPathPres;            /* abs_path present flag */
    ZUCHAR ucQueryPres;              /* query present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_HTTP_SSTR stQuery;            /* query */
    union
    {
        ST_HTTP_NET_PATH stNetPath;  /* net_path */
        ST_HTTP_ABS_PATH stAbsPath;  /* abs-path */
    } u;
} ST_HTTP_HIER_PART;

/* http opaque_part */
typedef struct tagHTTP_OPAQUE_PART
{
    ST_HTTP_SSTR stStr;              /* string */
} ST_HTTP_OPAQUE_PART;

/* http absoluteURI */
typedef struct tagHTTP_ABSO_URI
{
    ZUCHAR ucHierPartPres;           /* hier_part present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stScheme;            /* scheme */
    union
    {
        ST_HTTP_HIER_PART stHierPart; /* hier_part */
        ST_HTTP_OPAQUE_PART stOpaquePart; /* opaque_part */
    } u;
} ST_HTTP_ABSO_URI;

/* http relativeURI */
typedef struct tagHTTP_RELAT_URI
{
    ZUCHAR ucType;                   /* relativeURI type EN_HTTP_RELAT_URI */
    ZUCHAR ucQueryPres;              /* query present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_HTTP_SSTR stQuery;            /* query */
    union
    {
        ST_HTTP_NET_PATH stNetPath;  /* net_path */
        ST_HTTP_ABS_PATH stAbsPath;  /* abs-path */
        ST_HTTP_REL_PATH stRelPath;  /* rel-path */
    } u;
} ST_HTTP_RELAT_URI;

/* http byte-range-set */
typedef struct tagHTTP_BYTE_RANGE
{
    ZUCHAR ucSuffixPres;             /* suffix-length present flag */
    ZUCHAR ucLastPosPres;            /* last position present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUINT iFirstBytePos;             /* first-byte-pos */
    ZUINT iLastBytePos;              /* last-byte-pos */
    ZUINT iSuffixLen;                /* suffix-length */
} ST_HTTP_BYTE_RANGE;

/* list of byte-range-set */
ABNF_TYPEDEF_LIST(HTTP_BYTE_RANGE); 

/* http Request-URI */
typedef struct tagHTTP_REQ_URI
{
    ZUCHAR ucType;                   /* Request-URI type EN_HTTP_REQ_URI */
    ZUCHAR ucStrPres;                /* string present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stStr;               /* request uri string(decode used) */
    union
    {
        ST_HTTP_ABSO_URI stAbsoUri;  /* absoluteURI */
        ST_HTTP_ABS_PATH stAbsPath;  /* abs-path */
        ST_HTTP_AUTHOR stAuthor;     /* authority */
    } u;
} ST_HTTP_REQ_URI;

/* http parameter */
typedef struct tagHTTP_PARM
{
    ZUCHAR ucQValPres;               /* quoted-string present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stAttr;              /* attribute */
    ST_ZOS_SSTR stVal;               /* value */
} ST_HTTP_PARM;

/* list of parameter */
ABNF_TYPEDEF_LIST(HTTP_PARM); 

/* http media-type type */
typedef struct tagHTTP_MEDIA_TYPE
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucType;                   /* type type EN_HTTP_MTYPE */
    ZUCHAR ucSubtype;                /* subtype type EN_HTTP_MSUBTYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_ZOS_SSTR stTypeExt;           /* type extension-token */
    ST_ZOS_SSTR stSubTypeExt;        /* subtype extension-token */
    ST_HTTP_PARM_LST stParmLst;      /* parameter list */
} ST_HTTP_MEDIA_TYPE;

/* http media-range */
typedef struct tagHTTP_MEIDA_RANGE
{
    ZUCHAR ucAllPres;                /* all(*) for type present flag */
    ZUCHAR ucSubAllPres;             /* all(*) for subtype present flag */   
    ZUCHAR ucType;                   /* type type EN_HTTP_MTYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_ZOS_SSTR stTypeExt;           /* type extension-token */
    ST_ZOS_SSTR stSubType;           /* subtype */
    ST_HTTP_PARM_LST stParmLst;      /* parameter list */
} ST_HTTP_MEDIA_RANGE;

/* http media-disposition */
typedef struct tagHTTP_MEDIA_DSP
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucType;                   /* disposition type EN_HTTP_DSP_TYPE */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stTypeExt;           /* type extension-token */
    ST_HTTP_PARM_LST stParmLst;      /* parameter list */
} ST_HTTP_MEDIA_DSP;

/* http qvalue */
typedef struct tagHTTP_QVAL
{
    ZUCHAR ucFracPres;               /* fraction present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUSHORT wInt;                    /* qvalue integer */
    ZUSHORT wFrac;                   /* qvalue fraction */
} ST_HTTP_QVAL;

/* http generic value */
typedef struct tagHTTP_GEN_VAL
{
    ZUCHAR ucValPres;                /* value string present flag */
    ZUCHAR ucQStrPres;               /* quoted-string present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stName;              /* param name */
    ST_ZOS_SSTR stValStr;            /* value string */
} ST_HTTP_GEN_VAL;

/* list of generic value */
ABNF_TYPEDEF_LIST(HTTP_GEN_VAL); 

/* http accept-params */
typedef struct tagHTTP_ACPT_PARM
{
    ST_HTTP_QVAL stQval;             /* qvalue */
    ST_HTTP_GEN_VAL_LST stExtnLst;   /* accept-extension list */
} ST_HTTP_ACPT_PARM;

/* http accept-range */
typedef struct tagHTTP_ACPT_RANGE
{
    ZUCHAR ucParmPres;               /* param present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_MEDIA_RANGE stMediaRange; /* media-range */
    ST_HTTP_ACPT_PARM stParm;        /* accept-param */
} ST_HTTP_ACPT_RANGE;

/* list of accept-range */
ABNF_TYPEDEF_LIST(HTTP_ACPT_RANGE); 

/* http accept-charset-param */
typedef struct tagHTTP_ACPT_CPARM
{
    ZUCHAR ucAllPres;                /* all(*) present flag */
    ZUCHAR ucQvalPres;               /* qvalue present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_HTTP_SSTR stChrset;           /* charset */
    ST_HTTP_QVAL stQval;             /* qvalue */
} ST_HTTP_ACPT_CPARM;

/* list of accept-charset-param */
ABNF_TYPEDEF_LIST(HTTP_ACPT_CPARM); 

/* http codings */
typedef struct tagHTTP_CODINGS
{
    ZUCHAR ucAllPres;                /* all(*) present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stContent;           /* content-coding */
} ST_HTTP_CODINGS;

/* http encoding */
typedef struct tagHTTP_ENCODING
{
    ZUCHAR ucQvalPres;               /* qvalue present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_CODINGS stCodings;       /* codings */
    ST_HTTP_QVAL stQval;             /* qvalue */
} ST_HTTP_ENCODING;

/* list of encoding */
ABNF_TYPEDEF_LIST(HTTP_ENCODING); 

/* http language-range */
typedef struct tagHTTP_LANG_RANGE
{
    ZUCHAR ucAllPres;                /* all(*) present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stSubtag;            /* Primary-subtag */
    ST_HTTP_SSTR_LST stSubtagLst;    /* Subtag list */
} ST_HTTP_LANG_RANGE;

/* http language */
typedef struct tagHTTP_LANG
{
    ZUCHAR ucQvalPres;               /* qvalue present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_LANG_RANGE stRange;      /* language-range */
    ST_HTTP_QVAL stQval;             /* qvalue */
} ST_HTTP_LANG;

/* list of language */
ABNF_TYPEDEF_LIST(HTTP_LANG); 

/* http ainfo */
typedef struct tagHTTP_AINFO
{
    ZUCHAR ucType;                   /* ainfo type EN_HTTP_AINFO */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stNextNonce;     /* nextnonce */
        ST_HTTP_TYPE_VAL stMsgQop;   /* message-qop:qop-value EN_HTTP_QOP_VAL */        
        ST_ZOS_SSTR stRspAuth;       /* response-auth */
        ST_ZOS_SSTR stCnonce;        /* cnonce */
        ST_ZOS_SSTR stNonceCount;    /* nonce-count */
        ST_HTTP_GEN_VAL stAuthParm;  /* auth-param */
    } u;
} ST_HTTP_AINFO;

/* list of ainfo */
ABNF_TYPEDEF_LIST(HTTP_AINFO); 

/* http algorithm */
typedef struct tagHTTP_ALGO
{
    ZUCHAR ucType;                   /* algorithm type EN_HTTP_ALGO */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /* other algorithm */
} ST_HTTP_ALGO;

/* http dig-resp */
typedef struct tagHTTP_DIG_RSP
{
    ZUCHAR ucType;                   /* dig-resp type EN_HTTP_DIG_RSP */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stUserName;      /* username */
        ST_ZOS_SSTR stRealm;         /* realm */
        ST_ZOS_SSTR stNonce;         /* nonce */
        ST_HTTP_REQ_URI stDigestUri; /* digest-uri */
        ST_ZOS_SSTR stDresp;         /* dresponse */
        ST_HTTP_ALGO stAlgo;         /* algorithm */
        ST_ZOS_SSTR stCnonce;        /* cnonce */
        ST_ZOS_SSTR stOpaque;        /* opaque */
        ST_HTTP_TYPE_VAL stMsgQop;   /* message-qop:qop-value EN_HTTP_QOP_VAL */        
        ST_ZOS_SSTR stNonceCount;    /* nonce-count */
        ST_HTTP_GEN_VAL stAuthParm;  /* auth-param */
    } u;
} ST_HTTP_DIG_RSP;

/* list of dig-resp */
ABNF_TYPEDEF_LIST(HTTP_DIG_RSP); 

/* http other-response */
typedef struct tagHTTP_OTHER_RSP
{
    ST_ZOS_SSTR stScheme;            /* auth-scheme */
    ST_HTTP_GEN_VAL stParm;          /* auth-param */
    ST_HTTP_GEN_VAL_LST stParmLst;    /* auth-param list */
} ST_HTTP_OTHER_RSP;

/* http digest-response */
typedef struct tagHTTP_DIGEST_RSP
{
    ST_HTTP_DIG_RSP_LST stDigRspLst; /* dig-resp list */
} ST_HTTP_DIGEST_RSP;
 
/* http credentials */
typedef struct tagHTTP_CREDENTS
{
    ZUCHAR ucDigestRspPres;          /* digest-response present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_DIGEST_RSP stDigestRsp; /* digest-response */
        ST_HTTP_OTHER_RSP stOther;   /* other-response */
    } u;
} ST_HTTP_CREDENTS;

/* http cache-directive */
typedef struct tagHTTP_CACHE_DIRECT
{
    ZUCHAR ucType;                   /* cache-directive type EN_HTTP_DIRECT */
    ZUCHAR ucDeltaPres;              /* delta-seconds present flag */
    ZUCHAR ucFieldPres;              /* field-name present flag */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    union
    {
        ZUINT iSeconds;              /* delta-seconds */
        ST_HTTP_SSTR_LST stFieldLst; /* field-name list */
        ST_HTTP_GEN_VAL stExtn;      /* cache-extension */
    } u;
} ST_HTTP_CACHE_DIRECT;

/* list of cache-directive */
ABNF_TYPEDEF_LIST(HTTP_CACHE_DIRECT); 

/* http language-tag */
typedef struct tagHTTP_LANG_TAG
{
    ST_ZOS_SSTR stPrimaryTag;        /* primary-tag */
    ST_HTTP_SSTR_LST stSubtagLst;    /* subtag list */
} ST_HTTP_LANG_TAG;

/* list of language-tag */
ABNF_TYPEDEF_LIST(HTTP_LANG_TAG); 

/* http HTTP-date */
typedef struct tagHTTP_DATE
{
    ZUSHORT wYear;                   /* date1 year */
    ZUCHAR ucMonth;                  /* date1 month EN_HTTP_MONTH */
    ZUCHAR ucDay;                    /* date1 day */
    ZUCHAR ucWkDay;                  /* wkday EN_HTTP_WKDAY */
    ZUCHAR ucHour;                   /* time hour */
    ZUCHAR ucMinute;                 /* time minute */
    ZUCHAR ucSecond;                 /* second */
    ZUCHAR ucWkDayPres;              /* wkday present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stTimeZone;          /* timer zone */
} ST_HTTP_DATE;

/* http entity-tag */
typedef struct tagHTTP_ENT_TAG
{
    ZUCHAR ucWeakPres;               /* weak present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stOpaqueTag;         /* opaque-tag */
} ST_HTTP_ENT_TAG;

/* list of entity-tag */
ABNF_TYPEDEF_LIST(HTTP_ENT_TAG); 

/* http expectation */
typedef struct tagHTTP_EXPECT
{
    ZUCHAR ucExtnPres;               /* expectation-extension present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_GEN_VAL stParm;          /* expect-params */
    ST_HTTP_GEN_VAL_LST stParmLst;   /* expect-params list */
} ST_HTTP_EXPECT;

/* list of expectation */
ABNF_TYPEDEF_LIST(HTTP_EXPECT); 

/* http addr-spec */
typedef struct tagHTTP_ADDR_SPEC
{
    ST_ZOS_SSTR stLocalPart;         /* local-part */
    ST_ZOS_SSTR stDomain;            /* domain */
} ST_HTTP_ADDR_SPEC;

/* http name-addr */
typedef struct tagHTTP_NAME_ADDR
{
    ZUCHAR ucPhrasePres;             /* phrase present flag */
    ZUCHAR ucRoutePres;              /* route present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stPhrase;            /* phrase */
    ST_ZOS_SSTR stRoute;             /* route */
    ST_HTTP_ADDR_SPEC stAddrSpec;    /* addr-spec */
} ST_HTTP_NAME_ADDR;

/* http pragma-directive */
typedef struct tagHTTP_PRAGMA_DIRECT
{
    ZUCHAR ucExtnPres;               /* expectation-extension present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_GEN_VAL stPragma;        /* extension-pragma */
} ST_HTTP_PRAGMA_DIRECT;

/* list of pragma-directive */
ABNF_TYPEDEF_LIST(HTTP_PRAGMA_DIRECT); 

/* http transfer-extension */
typedef struct tagHTTP_TRSF_EXTN
{
    ST_HTTP_SSTR stName;             /* name */
    ST_HTTP_PARM_LST stParmLst;      /* parameter list */
} ST_HTTP_TRSF_EXTN;

/* http t-codings */
typedef struct tagHTTP_TCODING
{
    ZUCHAR ucExtnPres;               /* expectation-extension present flag */
    ZUCHAR ucParmPres;               /* params present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_HTTP_TRSF_EXTN stTrsfExtn;    /* transfer-extension */
    ST_HTTP_ACPT_PARM stAcptParm;    /* accept-params */
} ST_HTTP_TCODING;

/* list of t-codings */
ABNF_TYPEDEF_LIST(HTTP_TCODING); 

/* http transfer-coding */
typedef struct tagHTTP_TRSF_CODING
{
    ZUCHAR ucExtnPres;               /* expectation-extension present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_TRSF_EXTN stTrsfExtn;    /* transfer-extension */
} ST_HTTP_TRSF_CODING;

/* list of transfer-codings */
ABNF_TYPEDEF_LIST(HTTP_TRSF_CODING); 

/* http product */
typedef struct tagHTTP_PRODUCT
{
    ZUCHAR ucVerPres;                /* product-version present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stName;              /* product name */
    ST_ZOS_SSTR stVer;               /* product-version */
} ST_HTTP_PRODUCT;

/* list of product */
ABNF_TYPEDEF_LIST(HTTP_PRODUCT);

/* http server-val */
typedef struct tagHTTP_SERVER_VAL
{
    ZUCHAR ucProductPres;            /* product present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_PRODUCT stProduct;   /* product */
        ST_ZOS_SSTR stComment;       /* comment */
    } u;
} ST_HTTP_SERVER_VAL;

/* list of server-val */
ABNF_TYPEDEF_LIST(HTTP_SERVER_VAL);

/* http received-protocol */
typedef struct tagHTTP_RECV_PROTO
{
    ZUCHAR ucNamePres;               /* protocol-name present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stName;              /* protocol-name */
    ST_ZOS_SSTR stVer;               /* protocol-version */
} ST_HTTP_RECV_PROTO;

/* http received-by */
typedef struct tagHTTP_RECV_BY
{
    ZUCHAR ucHostPortPres;           /* hostport present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_HOST_PORT stHostPort; /* hostport */
        ST_ZOS_SSTR stPseu;          /* pseudonym */
    }u;
} ST_HTTP_RECV_BY;

/* http via-parm */
typedef struct tagHTTP_VIA_PARM
{
    ZUCHAR ucCommentPres;            /* comment present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_RECV_PROTO stProto;      /* received-protocol */
    ST_HTTP_RECV_BY stBy;            /* received-by */
    ST_ZOS_SSTR stComment;           /* comment */
} ST_HTTP_VIA_PARM;

/* list of via-parm */
ABNF_TYPEDEF_LIST(HTTP_VIA_PARM);

/* http warn-agent */
typedef struct tagHTTP_WARN_AGENT
{
    ZUCHAR ucHostPortPres;           /* hostport present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_HOST_PORT stHostPort; /* hostport */
        ST_ZOS_SSTR stPseu;          /* pseudonym */
    }u;
} ST_HTTP_WARN_AGENT;

/* http warning-value */
typedef struct tagHTTP_WARN_VAL
{
    ZUCHAR ucDatePres;               /* warn-date present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iCode;                     /* warn-code */
    ST_HTTP_WARN_AGENT stAgent;      /* warn-agent */
    ST_ZOS_SSTR stText;              /* warn-text */
    ST_HTTP_DATE stDate;             /* warn-date */
} ST_HTTP_WARN_VAL;

/* list of warning-value */
ABNF_TYPEDEF_LIST(HTTP_WARN_VAL);

/* http URI */
typedef struct tagHTTP_URI
{
    ZUCHAR ucAbsoUriPres;            /* absoluteURI present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_ABSO_URI stAbsoUri;  /* absoluteURI */
        ST_HTTP_ABS_PATH stAbsPath;  /* abs-path */
    } u;
} ST_HTTP_URI;

/* list of URI */
ABNF_TYPEDEF_LIST(HTTP_URI);

/* http domain */
typedef struct tagHTTP_DOMAIN
{
    ST_HTTP_URI stUri;               /* URI */
    ST_HTTP_URI_LST stUriLst;        /* URI list */
} ST_HTTP_DOMAIN;

/* http qop-options */
typedef struct tagHTTP_QOPTS
{
    ST_HTTP_TYPE_VAL stQval;         /* qop-value EN_HTTP_QOP_VAL */
    ST_HTTP_TYPE_VAL_LST stQvalLst;  /* qop-value list */
} ST_HTTP_QOPTS;

/* http digest-cln */
typedef struct tagHTTP_DIGEST_CLN
{
    ZUCHAR ucType;                   /* digest-cln type EN_HTTP_DIGEST_CLN */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stRealm;         /* realm */
        ST_HTTP_DOMAIN stDomain;     /* domain */
        ST_ZOS_SSTR stNonce;         /* nonce */
        ST_ZOS_SSTR stOpaque;        /* opaque */
        ZUCHAR ucStale;              /* stale EN_HTTP_STALE */
        ST_HTTP_ALGO stAlgo;         /* algorithm */
        ST_HTTP_QOPTS stQopts;       /* qop-options */
        ST_HTTP_GEN_VAL stAuthParm;  /* auth-param */
    } u;
} ST_HTTP_DIGEST_CLN;

/* list of digest-cln */
ABNF_TYPEDEF_LIST(HTTP_DIGEST_CLN);

/* http digest */
typedef struct tagHTTP_DIGEST
{
    ST_HTTP_DIGEST_CLN_LST stDigestClnLst; /* digest-cln list */
} ST_HTTP_DIGEST;

/* http other-challenge */
typedef struct tagHTTP_OTHER_CHALLENGE
{
    ST_ZOS_SSTR stAuthSchem;         /* auth-scheme */
    ST_HTTP_GEN_VAL stAuthParm;      /* auth-param */
    ST_HTTP_GEN_VAL_LST stParmLst;   /* auth-param list */
} ST_HTTP_OTHER_CHALLENGE;

/* http challenge */
typedef struct tagHTTP_CHALLENGE
{
    ZUCHAR ucDigestPres;             /* digest present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_DIGEST stDigest;      /* digest */
        ST_HTTP_OTHER_CHALLENGE stOther; /* other-challenge */
    } u;
} ST_HTTP_CHALLENGE;

/* list of challenge */
ABNF_TYPEDEF_LIST(HTTP_CHALLENGE);

/***************************** rfc2616 headers *************************/
/* http Accept */
typedef struct tagHTTP_HDR_ACPT
{
    ST_HTTP_ACPT_RANGE_LST stRangeLst; /* accept range list */
} ST_HTTP_HDR_ACPT;

/* http Accept-Charset */
typedef struct tagHTTP_HDR_ACPT_CHRSET
{
    ST_HTTP_ACPT_CPARM_LST stParmLst; /* accept charset  parameterlist */
} ST_HTTP_HDR_ACPT_CHRSET;

/* http Accept-Encoding */
typedef struct tagHTTP_HDR_ACPT_ENCODING
{
    ST_HTTP_ENCODING_LST stEncodingLst; /* encoding list */
} ST_HTTP_HDR_ACPT_ENCODING;

/* http Accept-Language */
typedef struct tagHTTP_HDR_ACPT_LANG
{
    ST_HTTP_LANG_LST stLangLst;      /* language list */
} ST_HTTP_HDR_ACPT_LANG;

/* http Accept-Ranges */
typedef struct tagHTTP_HDR_ACPT_RANGES
{
    ZUCHAR ucType;                   /* acceptable-ranges type EN_HTTP_ARANGE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_SSTR stOther;            /* other */
} ST_HTTP_HDR_ACPT_RANGES;

/* http Age */
typedef struct tagHTTP_HDR_AGE
{
    ZUINT iSeconds;                  /* delta-seconds */
} ST_HTTP_HDR_AGE;

/* http Allow */
typedef struct tagHTTP_HDR_ALLOW
{
    ST_HTTP_METHOD_LST stMethodLst;  /* Method list */
} ST_HTTP_HDR_ALLOW;

/* http Authentication-Info */
typedef struct tagHTTP_HDR_AUTHEN_INFO
{
    ST_HTTP_AINFO_LST stAinfoLst;   /* ainfo list */
} ST_HTTP_HDR_AUTHEN_INFO;

/* http Authorization */
typedef struct tagHTTP_HDR_AUTHOR
{
    ST_HTTP_CREDENTS stCredents;     /* credentials */
} ST_HTTP_HDR_AUTHOR;

/* http Cache-Control */
typedef struct tagHTTP_HDR_CACHE_CTRL
{
    ST_HTTP_CACHE_DIRECT_LST stDirectLst; /* cache-directive list */
} ST_HTTP_HDR_CACHE_CTRL;

/* http Connection */
typedef struct tagHTTP_HDR_CONN
{
    ST_HTTP_SSTR_LST stConnLst;      /* connection-token list */
} ST_HTTP_HDR_CONN;

/* http Content-Encoding */
typedef struct tagHTTP_HDR_CONTENT_ENCODING
{
    ST_HTTP_SSTR_LST stCodingLst;    /* content-coding list */
} ST_HTTP_HDR_CONTENT_ENCODING;

/* http Content-Language */
typedef struct tagHTTP_HDR_CONTENT_LANG
{
    ST_HTTP_LANG_TAG_LST stLangTagLst;/* language-tag list */
} ST_HTTP_HDR_CONTENT_LANG;

/* http Content-Location */
typedef struct tagHTTP_HDR_CONTENT_LOCAT
{
    ZUCHAR ucAbsoUriPres;            /* absoluteURI present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_ABSO_URI stAbsoUri;  /* absoluteURI */
        ST_HTTP_RELAT_URI stRelatUri; /* relativeURI */
    } u;
} ST_HTTP_HDR_CONTENT_LOCAT;

/* http Content-MD5 */
typedef struct tagHTTP_HDR_CONTENT_MD5
{
    ST_HTTP_SSTR stMd5Digest;        /* md5-digest */
} ST_HTTP_HDR_CONTENT_MD5;

/* http Content-Range */
typedef struct tagHTTP_HDR_CONTENT_RANGE
{
    ZUCHAR ucPosAllPres;             /* all position present flag */
    ZUCHAR ucInstanceAllPres;        /* all instance present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUINT iFirstBytePos;             /* first-byte-pos */
    ZUINT iLastBytePos;              /* last-byte-pos */
    ZUINT iInstanceLen;              /* instance-length */
} ST_HTTP_HDR_CONTENT_RANGE;

/* http Content-Type */
typedef struct tagHTTP_HDR_CONTENT_TYPE
{
    ST_HTTP_MEDIA_TYPE stMediaType;  /* media-type */
} ST_HTTP_HDR_CONTENT_TYPE;

/* http Content-Disposition */
typedef struct tagHTTP_HDR_CONTENT_DSP
{
    ST_HTTP_MEDIA_DSP stMediaDsp;    /* media-disposition */
} ST_HTTP_HDR_CONTENT_DSP;

/* http Date */
typedef struct tagHTTP_HDR_DATE
{
    ST_HTTP_DATE stDate;             /* HTTP-date */
} ST_HTTP_HDR_DATE;

/* http ETag */
typedef struct tagHTTP_HDR_ETAG
{
    ST_HTTP_ENT_TAG stEntTag;        /* entity-tag */
} ST_HTTP_HDR_ETAG;

/* http Expect */
typedef struct tagHTTP_HDR_EXPECT
{
    ST_HTTP_EXPECT_LST stExpectLst;  /* expectation list */
} ST_HTTP_HDR_EXPECT;

/* http Expires */
typedef struct tagHTTP_HDR_EXPIRES
{
    ZUCHAR ucCTCPres;                /* present flag for CTC */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iExpires;                  /* expires */
    ST_HTTP_DATE stDate;             /* HTTP-date */
} ST_HTTP_HDR_EXPIRES;

/* http From */
typedef struct tagHTTP_HDR_FROM
{
    ZUCHAR ucNameAddrPres;           /* name-addr present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_NAME_ADDR stNameAddr; /* name-addr */
        ST_HTTP_ADDR_SPEC stAddrSpec; /* addr-spec */
    } u;
} ST_HTTP_HDR_FROM;

/* http Host */
typedef struct tagHTTP_HDR_HOST
{
    ST_HTTP_HOST_PORT stHostPort;    /* host port */
} ST_HTTP_HDR_HOST;

/* http If-Match */
typedef struct tagHTTP_HDR_IF_MATCH
{
    ZUCHAR ucAllPres;                /* all present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_ENT_TAG stEntTag;        /* entity-tag */
    ST_HTTP_ENT_TAG_LST stEntTagLst; /* entity-tag list */
} ST_HTTP_HDR_IF_MATCH;

/* http If-Modified-Since */
typedef struct tagHTTP_HDR_IF_MDFY_SINCE
{
    ST_HTTP_DATE stDate;             /* HTTP-date */
} ST_HTTP_HDR_IF_MDFY_SINCE;

/* http If-None-Match */
typedef struct tagHTTP_HDR_IF_NO_MATCH
{
    ZUCHAR ucAllPres;                /* all present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_ENT_TAG stEntTag;        /* entity-tag */
    ST_HTTP_ENT_TAG_LST stEntTagLst; /* entity-tag list */
} ST_HTTP_HDR_IF_NO_MATCH;

/* http If-Range */
typedef struct tagHTTP_HDR_IF_RANGE
{
    ZUCHAR ucEntTagPres;             /* entity-tag present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_ENT_TAG stEntTag;    /* entity-tag */
        ST_HTTP_DATE stDate;         /* HTTP-date */
    } u;
} ST_HTTP_HDR_IF_RANGE;

/* http If-Unmodified-Since */
typedef struct tagHTTP_HDR_IF_UNMDFY_SINCE
{
    ST_HTTP_DATE stDate;             /* HTTP-date */
} ST_HTTP_HDR_IF_UNMDFY_SINCE;

/* http Last-Modified */
typedef struct tagHTTP_HDR_LAST_MDFY
{
    ST_HTTP_DATE stDate;             /* HTTP-date */
} ST_HTTP_HDR_LAST_MDFY;

/* http Location */
typedef struct tagHTTP_HDR_LOCAT
{           
    ZCHAR *pcStr;                    /* src str */
    ST_HTTP_ABSO_URI stAbsoUri;      /* absoluteURI */
} ST_HTTP_HDR_LOCAT;

/* http Pragma */
typedef struct tagHTTP_HDR_PRAGMA
{
    ST_HTTP_PRAGMA_DIRECT_LST stPragmaLst; /* pragma-directive list */
} ST_HTTP_HDR_PRAGMA;

/* http Proxy-Authenticate */
typedef struct tagHTTP_HDR_HDR_PROXY_AUTHEN
{
    ST_HTTP_CHALLENGE stChallenge;   /* challenge */
} ST_HTTP_HDR_PROXY_AUTHEN;

/* http Proxy-Authorization */
typedef struct tagHTTP_HDR_HDR_PROXY_AUTHOR
{
    ST_HTTP_CREDENTS stCredents;     /* credentials */
} ST_HTTP_HDR_PROXY_AUTHOR;

/* http Range */
typedef struct tagHTTP_HDR_RANGE
{
    ST_HTTP_BYTE_RANGE stByteRange;  /* byte-range-set */
    ST_HTTP_BYTE_RANGE_LST stByteRangeLst; /* byte-range-set list */
} ST_HTTP_HDR_RANGE;

/* http Referer */
typedef struct tagHTTP_HDR_REFERER
{
    ZUCHAR ucAbsoUriPres;            /* absoluteURI present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_ABSO_URI stAbsoUri;  /* absoluteURI */
        ST_HTTP_RELAT_URI stRelatUri; /* relativeURI */
    } u;
} ST_HTTP_HDR_REFERER;

/* http Retry-After */
typedef struct tagHTTP_HDR_RETRY_AFTER
{
    ZUCHAR ucDatePres;               /* HTTP-date present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_HTTP_DATE stDate;         /* HTTP-date */
        ZUINT iSeconds;              /* delta-seconds */
    } u;
} ST_HTTP_HDR_RETRY_AFTER;

/* http Server */
typedef struct tagHTTP_HDR_SERV
{
    ST_HTTP_SERVER_VAL stVal;        /* server-val */
    ST_HTTP_SERVER_VAL_LST stValLst; /* server-val list */
} ST_HTTP_HDR_SERV;

/* http TE */
typedef struct tagHTTP_HDR_TE
{
    ST_HTTP_TCODING_LST stTcodingLst; /* t-codings list */
} ST_HTTP_HDR_TE;

/* http Trailer */
typedef struct tagHTTP_HDR_TRAILER
{
    ST_HTTP_SSTR_LST stFieldLst;     /* field-name list */
} ST_HTTP_HDR_TRAILER;

/* http Transfer-Encoding */
typedef struct tagHTTP_HDR_TRSF_ENCODING
{
    ST_HTTP_TRSF_CODING_LST stCodingLst; /* transfer-coding list */
} ST_HTTP_HDR_TRSF_ENCODING;

/* http Upgrade */
typedef struct tagHTTP_HDR_UPGRADE
{
    ST_HTTP_PRODUCT_LST stProductLst; /* product list */
} ST_HTTP_HDR_UPGRADE;

/* http User-Agent */
typedef struct tagHTTP_HDR_USER_AGENT
{
    ST_HTTP_SERVER_VAL stVal;        /* server-val */
    ST_HTTP_SERVER_VAL_LST stValLst; /* server-val list */
} ST_HTTP_HDR_USER_AGENT;

/* http Vary */
typedef struct tagHTTP_HDR_VARY
{
    ZUCHAR ucAllPres;                /* all present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_SSTR_LST stFieldLst;     /* field-name list */
} ST_HTTP_HDR_VARY;

/* http Via */
typedef struct tagHTTP_HDR_VIA
{
    ST_HTTP_VIA_PARM_LST stParmLst;  /* via-parm list */
} ST_HTTP_HDR_VIA;

/* http Warning */
typedef struct tagHTTP_HDR_WARN
{
    ST_HTTP_WARN_VAL_LST stValLst;   /* warning-value list */
} ST_HTTP_HDR_WARN;

/* http WWW-Authenticate */
typedef struct tagHTTP_HDR_WWW_AUTHEN
{
    ST_HTTP_CHALLENGE stChallenge;   /* challenge */
} ST_HTTP_HDR_WWW_AUTHEN;

/********************* OMA-TS-XDM_Core-V1_0_1 headers ******************/
/* http X-XCAP-Asserted-Identity */
typedef struct tagHTTP_HDR_X_XCAP_AID
{
    ST_HTTP_SSTR stIdent;            /* identity */
} ST_HTTP_HDR_X_XCAP_AID;

/************************* 3GPP TS 24.109 headers **********************/
/* http X-3GPP-Intended-Identity */
typedef struct tagHTTP_HDR_X_3GPP_IID
{
    ST_HTTP_SSTR stIdent;            /* identity */
} ST_HTTP_HDR_X_3GPP_IID;

/* http X-3GPP-Asserted-Identity */
typedef struct tagHTTP_HDR_X_3GPP_AID
{
    ST_HTTP_SSTR_LST stIdentLst;     /* identity-list */
} ST_HTTP_HDR_X_3GPP_AID;

/* http X-3GPP-Authorization-Flags */
typedef struct tagHTTP_HDR_X_3GPP_AFLAG
{
    ST_HTTP_SSTR_LST stAflagLst;     /* auth-flag-list */
} ST_HTTP_HDR_X_3GPP_AFLAG;

/*************** Simple Object Access Protocol (SOAP) 1.1 **************/
/* http soapaction */
typedef struct tagHTTP_HDR_SOAP_ACT
{
    ST_HTTP_SSTR stUriRef;           /* URI-reference */
} ST_HTTP_HDR_SOAP_ACT;

/* http extension-header */
typedef struct tagHTTP_HDR_EXT_HDR
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stName;              /* header-name */
    ST_ZOS_SSTR stVal;               /* header-value */
} ST_HTTP_HDR_EXT_HDR;

/* http message-header */
typedef struct tagHTTP_MSG_HDR                       
{
    ZUCHAR ucType;                   /* message-header type EN_HTTP_HDR */ 
    ZUCHAR ucStrPres;                /* header string present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stHdrStr;            /* header string */
    union
    {
        ST_HTTP_HDR_ACPT stAcpt;     /* Accept */
        ST_HTTP_HDR_ACPT_CHRSET stAcptChrset; /* Accept-Charset */        
        ST_HTTP_HDR_ACPT_ENCODING stAcptEncoding; /* Accept-Encoding*/
        ST_HTTP_HDR_ACPT_LANG stAcptLang; /* Accept-Language*/
        ST_HTTP_HDR_ACPT_RANGES stAcptRanges; /* Accept-Ranges */
        ST_HTTP_HDR_AGE stAge;       /* Age */
        ST_HTTP_HDR_ALLOW stAllow;   /* Allow */
        ST_HTTP_HDR_AUTHEN_INFO stAuthenInfo; /* Authentication-Info */
        ST_HTTP_HDR_AUTHOR stAuthor; /* Authorization */
        ST_HTTP_HDR_CACHE_CTRL stCacheCtrl; /* Cache-Control */
        ST_HTTP_HDR_CONN stConn; /* Connection */        
        ST_HTTP_HDR_CONTENT_ENCODING stContentEncoding; /* Content-Encoding */
        ST_HTTP_HDR_CONTENT_LANG stContentLang; /* Content-Language */
        ZUINT iContentLen;         /* Content-Length */
        ST_HTTP_HDR_CONTENT_LOCAT stContentLocat; /* Content-Location */
        ST_HTTP_HDR_CONTENT_MD5 stContentMd5; /* Content-MD5 */
        ST_HTTP_HDR_CONTENT_RANGE stContentRange; /* Content-Range */
        ST_HTTP_HDR_CONTENT_TYPE stContentType; /* Content-Type */
        ST_HTTP_HDR_CONTENT_DSP stContentDsp; /* Content-Disposition */
        ST_HTTP_HDR_DATE stDate;     /* Date */
        ST_HTTP_HDR_ETAG stEtag;     /* ETage */
        ST_HTTP_HDR_EXPECT stExpect; /* Expect */
        ST_HTTP_HDR_EXPIRES stExpires; /* Expires */
        ST_HTTP_HDR_FROM stFrom;     /* From */
        ST_HTTP_HDR_HOST stHost;     /* Host */
        ST_HTTP_HDR_IF_MATCH stIfMatch; /* If-Match */
        ST_HTTP_HDR_IF_MDFY_SINCE stIfMdfySince; /* If-Modified-Since */
        ST_HTTP_HDR_IF_NO_MATCH stIfNoMatch; /* If-None-Match */
        ST_HTTP_HDR_IF_RANGE stIfRange; /* If-Range */
        ST_HTTP_HDR_IF_UNMDFY_SINCE stUnmdfySince; /* If-Unmodified-Since */
        ST_HTTP_HDR_LAST_MDFY stLastMdfy; /* Last-Modified */
        ST_HTTP_HDR_LOCAT stLocat;   /* Location */
        ZUINT iMaxForwards;        /* Max-Forwards */
        ST_HTTP_HDR_PRAGMA stPragma; /* Pragma */
        ST_HTTP_HDR_PROXY_AUTHEN stProxyAuthen; /* Proxy-Authenticate */
        ST_HTTP_HDR_PROXY_AUTHOR stProxyAuthor; /* Proxy-Authorization */
        ST_HTTP_HDR_RANGE stRange;   /* Range */
        ST_HTTP_HDR_REFERER stReferer; /* Referer */ 
        ST_HTTP_HDR_RETRY_AFTER stRetryAfter; /* Retry-After */
        ST_HTTP_HDR_SERV stServ;     /* Server */
        ST_HTTP_HDR_TE stTe;         /* TE */
        ST_HTTP_HDR_TRAILER stTrailer; /* Trailer */
        ST_HTTP_HDR_TRSF_ENCODING stTrsfEncoding; /* Transfer-Encoding */
        ST_HTTP_HDR_UPGRADE stUpgrade; /* Upgrade */
        ST_HTTP_HDR_USER_AGENT stUserAgent; /* User-Agent */
        ST_HTTP_HDR_VARY stVary;     /* Vary */
        ST_HTTP_HDR_VIA stVia;       /* Via */
        ST_HTTP_HDR_WARN stWarn;     /* Warning */
        ST_HTTP_HDR_WWW_AUTHEN stWwwAuthen; /* WWW-Authenticate */
        /* OMA-TS-XDM_Core-V1_0_1 */
        ST_HTTP_HDR_X_XCAP_AID stXXcapAid; /* X-XCAP-Asserted-Identity */
        /* 3GPP TS 24.109 */
        ST_HTTP_HDR_X_3GPP_IID stX3gppIid; /* X-3GPP-Intended-Identity */
        ST_HTTP_HDR_X_3GPP_AID stX3gppAid; /* X-3GPP-Asserted-Identity */
        ST_HTTP_HDR_X_3GPP_AFLAG stX3gppAflag; /* X-3GPP-Authorization-Flags */
        /* W3 SOAP */
        ST_HTTP_HDR_SOAP_ACT stSoapAct; /* soapaction */
        /* extension-header */
        ST_HTTP_HDR_EXT_HDR stExtHdr; /* extension-header */
    } u;
} ST_HTTP_MSG_HDR;

/* list of message-header */
ABNF_TYPEDEF_LIST(HTTP_MSG_HDR); 

/* http file status of body part */
typedef struct tagHTTP_FILE_STAT
{
    ZUINT iFileSize;                 /* file total size */
    ZUINT iFileLeft;                 /* file size not be read */
    ZUINT iFBufLeft;                 /* file buffer size not be sent */
    ZUINT iFBufOffset;               /* file buffer offset */
    ZUINT iFStartOffset;             /* file real start offset */
    ZFILE zFile;                     /* file handler */
    ZCHAR *pcFileBuf;                /* file buffer address */
} ST_HTTP_FILE_STAT;

/* http media-disposition */
typedef struct tagHTTP_SEND_DSC
{
    ZUCHAR ucPres;                               /* present flag */
    ST_ZOS_SSTR stConEncoding;                   /* Content-Transfer-Encoding */
    ZUINT iFileStartOffset;                      /* file start offset */
    ZUINT iFileStopOffset;                       /* file stop offset */
    ZUINT iTotalSize;                            /* file size */
    ZUINT iSendSize;                             /* send size */
} ST_HTTP_SEND_DSC;

/* http body part */
typedef struct tagHTTP_BODY_PART
{
    ZUCHAR ucFilePres;               /* file present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iPartSize;                 /* body part size */
    ZDBUF zMsgBuf;                   /* encoded message buffer */
    ST_HTTP_MEDIA_TYPE stMType;      /* media type */
    ST_HTTP_MEDIA_DSP stMDsp;        /* media disposition */
    ST_ZOS_SSTR stMDes;              /* media description */
    ST_HTTP_SEND_DSC stSendDsc;    /* send operation description */
    ST_HTTP_FILE_STAT stFileStat;    /* file status of body part */
    ST_ZOS_SSTR stStr;               /* message body */
} ST_HTTP_BODY_PART;

/* list of message-header */
ABNF_TYPEDEF_LIST(HTTP_BODY_PART); 

/* http body */
typedef struct tagHTTP_BODY
{                                         
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucMultiPart;              /* multipart flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUINT iBodySize;                 /* body size */
    ST_ZOS_SSTR stStr;               /* message body */
    ST_ZOS_SSTR stBoundary;          /* boundary */
    ST_HTTP_BODY_PART_LST stPartLst; /* body pary list */
} ST_HTTP_BODY;

/* http HTTP-Version */
typedef struct tagHTTP_VER
{
    ZUINT iMajor;                    /* major version */
    ZUINT iMinor;                    /* minor version */
} ST_HTTP_VER;

/* http Request-Line */
typedef struct tagHTTP_REQ_LINE
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_METHOD stMethod;         /* Method */
    ST_HTTP_REQ_URI stReqUri;        /* Request-URI */
    ST_HTTP_VER stVer;               /* HTTP-Version */
} ST_HTTP_REQ_LINE;

/* http Status-Line */
typedef struct tagHTTP_STAT_LINE
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_HTTP_VER stVer;               /* HTTP-Version */
    ZUINT iCode;                     /* Status-Code */
    ST_ZOS_SSTR stReason;            /* Reason-Phrase */
} ST_HTTP_STAT_LINE;

/* http transport status */
typedef struct tagHTTP_TRANS_STAT
{
    ZUCHAR ucStatus;                 /* transport status, EN_HTTP_TRANS_STAT */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iPercent;                  /* sent percent */
    ZUINT iSentSize;                 /* sent size */
    ZUINT iTotalSize;                /* total size */
    ZUINT iFileSentSize;             /* file sent size */
    ZTIME_T zLastRptTime;            /* last report time */ 
} ST_HTTP_TRANS_STAT;

/* http message */
typedef struct tagHTTP_MSG
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucReqPres;                /* Request present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZDBUF zMemBuf;                   /* memory buffer */
    ZDBUF zMsgBuf;                   /* message buffer */
    ST_ZOS_SSTR stMsgStr;            /* message string(decode used) */
    union
    {
        ST_HTTP_REQ_LINE stReqLine;  /* Request Line */
        ST_HTTP_STAT_LINE stStatLine;/* Status Line */
    } u;
    ST_HTTP_MSG_HDR_LST stHdrLst;    /* message-header list */
    ST_HTTP_BODY stBody;             /* message-body */
    ST_HTTP_TRANS_STAT stStat;       /* transport status */
} ST_HTTP_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_ABNF_TYPE_H__ */

