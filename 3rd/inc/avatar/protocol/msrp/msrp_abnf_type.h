/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : msrp_abnf_type.h
  Module        : msrp protocol
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Marcos and structure definitions required by the msrp protocol.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_ABNF_TYPE_H__
#define _MSRP_ABNF_TYPE_H__
    
/** 
 * @file msrp_abnf_type.h
 * @brief MSRP ABNF typedefs.
 */
#ifdef __cplusplus
extern "C" {
#endif

/******************** macros or enum values definition ******************/
/** @brief MSRP header type. */
typedef enum EN_MSRP_HDR
{
    /* rfc4975 headers */
    EN_MSRP_HDR_TO_PATH,
    EN_MSRP_HDR_FROM_PATH,
    EN_MSRP_HDR_MSG_ID,
    EN_MSRP_HDR_SUCC_RPT,
    EN_MSRP_HDR_FAIL_RPT,
    EN_MSRP_HDR_BYTE_RANGE,
    EN_MSRP_HDR_STATUS,
    EN_MSRP_HDR_CONT_ID,
    EN_MSRP_HDR_CONT_DESC,
    EN_MSRP_HDR_CONT_DISP,
    EN_MSRP_HDR_CONT_TYPE,
    /* extension-header */
    EN_MSRP_HDR_EXT
} EN_MSRP_HDR;

/** @brief MSRP method type. */
typedef enum EN_MSRP_METHOD
{
    /* rfc4975 headers */
    EN_MSRP_METHOD_SEND,
    EN_MSRP_METHOD_REPORT,
    /* extension methods */
    EN_MSRP_METHOD_EXT
} EN_MSRP_METHOD;

/** @brief MSRP success-reprot type. */
typedef enum EN_MSRP_SUCC_RPT
{
    EN_MSRP_SUCC_RPT_YES,
    EN_MSRP_SUCC_RPT_NO
} EN_MSRP_SUCC_RPT;

/** @brief MSRP failure-reprot type. */
typedef enum EN_MSRP_FAIL_RPT
{
    EN_MSRP_FAIL_RPT_YES,
    EN_MSRP_FAIL_RPT_NO,
    EN_MSRP_FAIL_RPT_PARTIAL
} EN_MSRP_FAIL_RPT;

/** @brief MSRP MSRP-URI type. */
typedef enum EN_MSRP_MSRP_URI
{
    EN_MSRP_MSRP_URI_MSRP,
    EN_MSRP_MSRP_URI_MSRPS    
} EN_MSRP_MSRP_URI;

/** @brief MSRP transport type. */
typedef enum EN_MSRP_TPT
{
    EN_MSRP_TPT_TCP,
    EN_MSRP_TPT_TLS,
    EN_MSRP_TPT_OTHER   
} EN_MSRP_TPT;

/** @brief MSRP host type. */
typedef enum EN_MSRP_HOST
{
    EN_MSRP_HOST_IPV4,
    EN_MSRP_HOST_IPV6,
    EN_MSRP_HOST_REGNAME,
    EN_MSRP_HOST_IPVFUTURE
} EN_MSRP_HOST;

/** @brief MSRP disposition-type type. */
typedef enum EN_MSRP_DISP_TYPE
{
    EN_MSRP_DISP_TYPE_INLINE,
    EN_MSRP_DISP_TYPE_ATTACHMENT,
    EN_MSRP_DISP_TYPE_EXT
} EN_MSRP_DISP_TYPE;

/** @brief MSRP disposition-parm type. */
typedef enum EN_MSRP_DISP_PARM
{
    EN_MSRP_DISP_PARM_FILENAME,
    EN_MSRP_DISP_PARM_MDF_DATE,
    EN_MSRP_DISP_PARM_READ_DATE,
    EN_MSRP_DISP_PARM_CREAT_DATE,
    EN_MSRP_DISP_PARM_SIZE,
    EN_MSRP_DISP_PARM_PARM
} EN_MSRP_DISP_PARM;

/** @brief MSRP continuation-flag type. */
typedef enum EN_MSRP_CONT_FLAG_TYPE
{
    EN_MSRP_CONT_MSG_CONT = '+',
    EN_MSRP_CONT_MSG_END = '$',
    EN_MSRP_CONT_MSG_ABORT = '#'
} EN_MSRP_CONT_FLAG_TYPE;

/** @brief MSRP type. */
typedef enum EN_MSRP_TYPE
{
    EN_MSRP_TYPE_DISC_TEXT, 
    EN_MSRP_TYPE_DISC_IMAGE,
    EN_MSRP_TYPE_DISC_AUDIO,
    EN_MSRP_TYPE_DISC_VIDEO,
    EN_MSRP_TYPE_DISC_APP,  
    EN_MSRP_TYPE_COMP_MSG,  
    EN_MSRP_TYPE_COMP_MULTI,
    EN_MSRP_TYPE_EXT        
} EN_MSRP_TYPE;

/** @brief MSRP subtype. */
typedef enum EN_MSRP_SUBTYPE
{
    EN_MSRP_SUBTYPE_ALL,       
    EN_MSRP_SUBTYPE_PLAIN,        
    EN_MSRP_SUBTYPE_CPIM,
    EN_MSRP_SUBTYPE_HTML,
    EN_MSRP_SUBTYPE_SIP,
    EN_MSRP_SUBTYPE_SIPFRAG,
    EN_MSRP_SUBTYPE_MSRPFRAG,
    EN_MSRP_SUBTYPE_FRPT,
    EN_MSRP_SUBTYPE_MIXED,
    EN_MSRP_SUBTYPE_EXT      
} EN_MSRP_SUBTYPE;

/***************************** rfc3986 typedefs *************************/
/** @brief MSRP IPvFuture. */
typedef struct tagMSRP_IPV_FUTURE
{
    ST_ZOS_SSTR stHexdig;            /**< @brief DEXDIG */
    ST_ZOS_SSTR stIpvFuture;         /**< @brief IPvFuture */
} ST_MSRP_IPV_FUTURE;

/** @brief MSRP host. */
typedef struct tagMSRP_HOST
{
    ZUCHAR ucType;                   /**< @brief host type EN_MSRP_HOST */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stRegName;       /**< @brief reg-name */
        ZUINT iIpv4Addr;             /**< @brief IPv4address */
        ZUCHAR aucIpv6Addr[ZINET_IPV6_ADDR_SIZE]; /**< @brief IPv6address */
        ST_MSRP_IPV_FUTURE stIpvFuture; /**< @brief IPvFuture */
    } u;
} ST_MSRP_HOST;

/** @brief MSRP authority. */
typedef struct tagMSRP_AUTHORITY
{ 
    ZUCHAR ucUserInfoPres;           /**< @brief userinfo present flag */
    ZUCHAR ucPortPres;               /**< @brief port present flag */
    ZUSHORT wPort;                   /**< @brief port */    
    ST_ZOS_SSTR stUserInfo;          /**< @brief userinfo */
    ST_MSRP_HOST stHost;             /**< @brief host */
} ST_MSRP_AUTHORITY;

/***************************** rfc2183 typedefs *************************/
/** @brief MSRP value. */
typedef struct tagMSRP_VALUE
{ 
    ZUCHAR ucQStrPres;               /**< @brief quoted-string present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stValue;             /**< @brief quoted-string */
} ST_MSRP_VALUE;

/** @brief MSRP parameter. */
typedef struct tagMSRP_PARM
{ 
    ST_ZOS_SSTR stAttr;              /**< @brief attribute */
    ST_MSRP_VALUE stValue;           /**< @brief value */
} ST_MSRP_PARM;

/** @brief MSRP filename-parm. */
typedef struct tagMSRP_FILE_NAME
{ 
    ST_MSRP_VALUE stValue;           /**< @brief quoted-string */
} ST_MSRP_FILE_NAME;

/** @brief MSRP disposition-parm. */
typedef struct tagMSRP_DISP_PARM
{ 
    ZUCHAR ucType;                   /**< @brief disp-parm type EN_MSRP_DISP_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */    
    union
    {
        ST_MSRP_FILE_NAME stFileName; /**< @brief filename-parm */
        ST_ZOS_SSTR stCreatData;     /**< @brief creation-data-parm */
        ST_ZOS_SSTR stMdfData;       /**< @brief modification-data-parm */
        ST_ZOS_SSTR stReadData;      /**< @brief read-data-parm */
        ZUINT iSize;                 /**< @brief size-parm */    
        ST_MSRP_PARM stParm;         /**< @brief parameter */
    }u;
} ST_MSRP_DISP_PARM;

/** @brief MSRP list of disposition-parm. */
ABNF_TYPEDEF_LIST(MSRP_DISP_PARM); 

/** @brief MSRP content-disposition. */
typedef struct tagMSRP_HDR_CONT_DISP
{ 
    ZUCHAR ucType;                   /**< @brief disp-type type EN_MSRP_DISP_TYPE */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */ 
    ST_ZOS_SSTR stExtTkn;            /**< @brief extension-token */
    ST_MSRP_DISP_PARM_LST stDisParmLst; /**< @brief disposition-parm */
} ST_MSRP_HDR_CONT_DISP;

/***************************** rfc2045 typedefs *************************/
/** @brief MSRP word. */
typedef struct tagMSRP_WORD
{ 
    ZUCHAR ucQStrPres;               /**< @brief quoted-string present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stWord;              /**< @brief quoted-string */
} ST_MSRP_WORD;

/** @brief MSRP list of word. */
ABNF_TYPEDEF_LIST(MSRP_WORD);

/** @brief MSRP sub-domain. */
typedef struct tagMSRP_SDOMAIN
{ 
    ZUCHAR ucDLiteralPres;           /**< @brief domain-literal present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stSDomain;           /**< @brief domain */
} ST_MSRP_SDOMAIN;

/** @brief MSRP list of sub-domain. */
ABNF_TYPEDEF_LIST(MSRP_SDOMAIN); 

/** @brief MSRP content-id. */
typedef struct tagMSRP_HDR_CONT_ID
{ 
    ST_MSRP_WORD_LST stWordLst;      /**< @brief local-part */
    ST_MSRP_SDOMAIN_LST stSDominLst; /**< @brief domain */
} ST_MSRP_HDR_CONT_ID;

/** @brief MSRP content-description. */
typedef struct tagMSRP_HDR_CONT_DESC
{ 
    ST_ZOS_SSTR stText;              /**< @brief text */
} ST_MSRP_HDR_CONT_DESC;

/***************************** rfc4975 typedefs *************************/
/** @brief MSRP pval. */
typedef struct tagMSRP_PVAL
{ 
    ZUCHAR ucQStrPres;               /**< @brief quoted-string present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stPval;              /**< @brief quoted-string */
} ST_MSRP_PVAL;

/** @brief MSRP gen-paramter. */
typedef struct tagMSRP_GEN_PARM
{
    ZUCHAR ucPvalPres;               /**< @brief pval present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stPname;             /**< @brief pname */
    ST_MSRP_PVAL stPval;             /**< @brief pval */
}ST_MSRP_GEN_PARM;

/** @brief MSRP list of gen-paramter. */
ABNF_TYPEDEF_LIST(MSRP_GEN_PARM); 

/** @brief MSRP URI-paramter. */
typedef struct tagMSRP_URI_PARM
{
    ZUCHAR ucRTknPres;               /**< @brief token present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stLToken;            /**< @brief left token */
    ST_ZOS_SSTR stRToken;            /**< @brief right token */
}ST_MSRP_URI_PARM;

/** @brief MSRP list of URI-paramter. */
ABNF_TYPEDEF_LIST(MSRP_URI_PARM); 

/** @brief MSRP Media-type. */
typedef struct tagMSRP_MEDIA_TYPE
{ 
    ZUCHAR ucType;                   /**< @brief type type EN_MSRP_TYPE */
    ZUCHAR ucSubType;                /**< @brief subtype type EN_MSRP_SUBTYPE */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stTypeStr;           /**< @brief type string */
    ST_ZOS_SSTR stTypeExt;           /**< @brief type extension-token */
    ST_ZOS_SSTR stSubTypeExt;        /**< @brief subtype extension-token */
    ST_MSRP_GEN_PARM_LST stGenParmLst; /**< @brief gen-param list */
} ST_MSRP_MEDIA_TYPE;

/** @brief MSRP transport. */
typedef struct tagMSRP_TPT
{ 
    ZUCHAR ucType;                   /**< @brief transport type EN_MSRP_TPT */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /**< @brief extension or unknown type string */
} ST_MSRP_TPT;

/** @brief MSRP MSRP-URI. */
typedef struct tagMSRP_MSRP_URI                      
{
    ZUCHAR ucType;                   /**< @brief MSRP-URI type EN_MSRP_MSRP_URI */
    ZUCHAR ucStrPres;                /**< @brief msrp-uri string present flag */
    ZUCHAR ucSessIdPres;             /**< @brief session-id present flag */
    ZUCHAR aucSpare[1];              /**< @brief for 32 bit alignment */
    ST_MSRP_TPT stTpt;               /**< @brief transport */
    ST_ZOS_SSTR stSessId;            /**< @brief session-id rfc3986 */
    ST_MSRP_AUTHORITY stAuthority;   /**< @brief authority rfc3986 */
    ST_MSRP_URI_PARM_LST stUriParmLst; /**< @brief URI-parameter list */
    ST_ZOS_SSTR stUriStr;            /**< @brief msrp-uri string */
}ST_MSRP_MSRP_URI;

/** @brief MSRP list of MSRP-URI. */
ABNF_TYPEDEF_LIST(MSRP_MSRP_URI); 

/** @brief MSRP ext-header. */
typedef struct tagMSRP_HDR_EXT_HDR
{
    ST_ZOS_SSTR stHname;             /**< @brief hname */
    ST_ZOS_SSTR stHval;              /**< @brief hval */
} ST_MSRP_HDR_EXT_HDR;

/** @brief MSRP Status. */
typedef struct tagMSRP_HDR_STATUS
{
    ZUCHAR ucCommPres;               /**< @brief comment present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ZUINT iNamespace;                /**< @brief namespace */
    ZUINT iStatCode;                 /**< @brief Status-Code */
    ST_ZOS_SSTR stComment;           /**< @brief Comment */
} ST_MSRP_HDR_STATUS;

/** @brief MSRP Byte-Range. */
typedef struct tagMSRP_HDR_BYTE_RANGE
{
    ZUCHAR ucUnknownEndPres;          /**< @brief unknow range-end present flag */
    ZUCHAR ucUnknownTotalPres;        /**< @brief unknow total present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ZUINT iRangeStart;               /**< @brief range-start */
    ZUINT iRangeEnd;                 /**< @brief range-end */
    ZUINT iTotal;                    /**< @brief total */
} ST_MSRP_HDR_BYTE_RANGE;

/** @brief MSRP Success-Report. */
typedef struct tagMSRP_HDR_SUCC_RPT
{
    ZUCHAR ucType;                   /**< @brief reprot type EN_MSRP_SUCC_RPT */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */    
} ST_MSRP_HDR_SUCC_RPT;

/** @brief MSRP Failure-Report. */
typedef struct tagMSRP_HDR_FAIL_RPT
{
    ZUCHAR ucType;                   /**< @brief reprot type EN_MSRP_FAIL_RPT */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */    
} ST_MSRP_HDR_FAIL_RPT;

/** @brief MSRP Message-ID. */
typedef struct tagMSRP_HDR_MSG_ID
{ 
    ST_ZOS_SSTR stIdent;             /**< @brief ident */
} ST_MSRP_HDR_MSG_ID;

/** @brief MSRP To-Path & From-Path. */
typedef struct tagMSRP_HDR_PATH
{  
    ST_MSRP_MSRP_URI_LST stMsrpUriLst; /**< @brief Msrp Uri */
} ST_MSRP_HDR_PATH;

/** @brief MSRP Content-Type. */
typedef struct tagMSRP_HDR_CONT_TYPE
{ 
    ST_MSRP_MEDIA_TYPE stMediaType;  /**< @brief Media-type */
} ST_MSRP_HDR_CONT_TYPE;

/** @brief MSRP header. */
typedef struct tagMSRP_HDR                       
{
    ZUCHAR ucType;                   /**< @brief header type EN_MSRP_HDR */
    ZUCHAR ucStrPres;                /**< @brief header string present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stHdrStr;            /**< @brief header string */    
    union
    {
        /* rfc4975 headers */
        ST_MSRP_HDR_PATH *pstFromPath; /**< @brief From-Path */
        ST_MSRP_HDR_PATH *pstToPath; /**< @brief To-Path */
        ST_MSRP_HDR_MSG_ID *pstMsgId; /**< @brief Message-ID */
        ST_MSRP_HDR_SUCC_RPT *pstSuccRpt; /**< @brief Success-Report */
        ST_MSRP_HDR_FAIL_RPT *pstFailRpt; /**< @brief Failure-Report */
        ST_MSRP_HDR_BYTE_RANGE *pstByteRange; /**< @brief Byte-Range */
        ST_MSRP_HDR_STATUS *pstStatus; /**< @brief Status */
        ST_MSRP_HDR_CONT_ID *pstContId;    /**< @brief Content-ID */
        ST_MSRP_HDR_CONT_DESC *pstContDesc; /**< @brief Content-Description */
        ST_MSRP_HDR_CONT_DISP *pstContDisp; /**< @brief Content-Disposition */
        ST_MSRP_HDR_CONT_TYPE *pstContType; /**< @brief Content-Type */
        /* extension-header */
        ST_MSRP_HDR_EXT_HDR *pstExtHdr;  /**< @brief Extension-Header */
        ZVOID *pUnknownHdr;          /**< @brief unknown header */
    }u;
} ST_MSRP_HDR;

/** @brief MSRP list of header. */
ABNF_TYPEDEF_LIST(MSRP_HDR); 

/** @brief MSRP Method. */
typedef struct tagMSRP_METHOD
{
    ZUCHAR ucType;                   /**< @brief method type EN_MSRP_METHOD */  
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /**< @brief extension or unknown type string */
} ST_MSRP_METHOD;

/** @brief MSRP Request-start. */
typedef struct tagMSRP_REQ_START
{
    ST_MSRP_METHOD stMethod;         /**< @brief Method */
} ST_MSRP_REQ_START;

/** @brief MSRP Response-start. */
typedef struct tagMSRP_RSP_START
{
    ZUCHAR ucCommPres;               /**< @brief Comment flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stComment;           /**< @brief Comment */
} ST_MSRP_RSP_START;

/** @brief MSRP message. */
typedef struct tagMSRP_MSG
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucReqPres;                /**< @brief request present flag */
    ZUCHAR ucContStfPres;            /**< @brief content-stuff data present flag */
    ZUCHAR ucContFlag;               /**< @brief type of EN_MSRP_CONT_FLAG_TYPE */
    ZUCHAR ucRefCnt;                 /**< @brief reference count */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ZDBUF zMemBuf;                   /**< @brief memory buffer */
    ZDBUF zMsgBuf;                   /**< @brief message buffer */
    ZUINT iStatCode;                 /**< @brief Status-Code */
    ST_ZOS_SSTR stTransId;           /**< @brief transact-id */
    ST_ZOS_SSTR stData;              /**< @brief data */
    ST_ZOS_SSTR stMsgStr;           /**< @brief message string(decode used) */
    union
    {
        ST_MSRP_REQ_START stReq;     /**< @brief Request-start*/
        ST_MSRP_RSP_START stRsp;     /**< @brief Response-start */
    } u;
    ST_MSRP_HDR_LST stHdrLst;        /**< @brief headers */
} ST_MSRP_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_ABNF_TYPE_H__ */

