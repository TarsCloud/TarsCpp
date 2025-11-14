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
  File name     : xcapc_type.h
  Module        : xcap client typedefs
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-04-22
  Description   :
    Marcos and structure definitions required by the xcapc protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XCAPC_TYPE_H__
#define _XCAPC_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* typedef xcapc list node */
typedef ST_ZOS_DLIST_NODE ST_XCAPC_LIST_NODE;

/* xcapc list use the dlist */
#define XCAPC_TYPEDEF_LIST ZOS_TYPEDEF_DLIST

/* xcapc method type */
typedef enum EN_XCAPC_METHOD
{
    EN_XCAPC_METHOD_PUT = EN_HTTP_METHOD_PUT, /* create or replace resource */
    EN_XCAPC_METHOD_GET = EN_HTTP_METHOD_GET, /* fetch a resource */
    EN_XCAPC_METHOD_DEL = EN_HTTP_METHOD_DEL  /* delete a resource */
} EN_XCAPC_METHOD;

/* xcapc mime type */
typedef enum EN_XCAPC_MIME_TYPE
{
    EN_XCAPC_MIME_APP_AUTH_POLICY = 1,
    EN_XCAPC_MIME_APP_XCAP_EL,
    EN_XCAPC_MIME_APP_XCAP_ATT,
    EN_XCAPC_MIME_APP_XCAP_NS,
    EN_XCAPC_MIME_APP_XCAP_ERR,
    EN_XCAPC_MIME_APP_XCAP_CAPS,    
    EN_XCAPC_MIME_APP_XCAP_DIFF,    
    EN_XCAPC_MIME_APP_RES_LSTS,    
    EN_XCAPC_MIME_APP_RLS_SRV,
    EN_XCAPC_MIME_APP_XDM_XDIR,
    EN_XCAPC_MIME_APP_POC_GRPS,
    EN_XCAPC_MIME_APP_GRP_USG_LST,
    EN_XCAPC_MIME_APP_PRES_CTT,
    EN_XCAPC_MIME_APP_SEARCH,
    EN_XCAPC_MIME_APP_CAB_PCC,
    EN_XCAPC_MIME_APP_PCC_CTT,
    EN_XCAPC_MIME_APP_PIDF
} EN_XCAPC_MIME_TYPE;

/* xcapc element step type */
typedef enum EN_XCAPC_STEP_TYPE
{
    EN_XCAPC_STEP_BY_NAME,           /* by-name */
    EN_XCAPC_STEP_BY_POS,            /* by-pos */
    EN_XCAPC_STEP_BY_ATTR,           /* by-attr */
    EN_XCAPC_STEP_BY_POS_ATTR,       /* by-pos-attr */
    EN_XCAPC_STEP_EXTN               /* extension-selector */
} EN_XCAPC_STEP_TYPE;

/* xcapc terminal-selector type */
typedef enum EN_XCAPC_TERMS_TYPE
{
    EN_XCAPC_TERMS_ATTR,             /* attribute-selector */
    EN_XCAPC_TERMS_NS,               /* namespace-selector */
    EN_XCAPC_TERMS_EXTN              /* extension-selector */
} EN_XCAPC_TERMS_TYPE;

/* xcapc path segment */
typedef ST_ZOS_SSTR ST_XCAPC_SEG;

/* xcapc path segment list */
XCAPC_TYPEDEF_LIST(XCAPC_SEG);

/* xcapc step by-name */
typedef struct tagXCAPC_BNAME
{
    ST_ZOS_SSTR stPrefix;            /* element name prefix */
    ST_ZOS_SSTR stName;              /* element name */
} ST_XCAPC_BNAME;

/* xcapc step by-pos */
typedef struct tagXCAPC_BPOS
{
    ZUCHAR ucPosPres;                /* position present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stPrefix;            /* element name prefix */
    ST_ZOS_SSTR stName;              /* element name */
    ZUINT iPos;                      /* position */
} ST_XCAPC_BPOS;

/* xcapc step attr-test */
typedef struct tagXCAPC_ATTRT
{
    ST_ZOS_SSTR stPrefix;            /* element name prefix */
    ST_ZOS_SSTR stName;              /* att-name */
    ST_ZOS_SSTR stVal;               /* att-value */
} ST_XCAPC_ATTRT;

/* xcapc step by-attr */
typedef struct tagXCAPC_BATTR
{
    ZUCHAR ucTestPres;               /* test present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stPrefix;            /* element name prefix */
    ST_ZOS_SSTR stName;              /* element name */
    ST_XCAPC_ATTRT stTest;           /* attr-test */
} ST_XCAPC_BATTR;

/* xcapc step by-pos-attr */
typedef struct tagXCAPC_BAPOS
{
    ZUCHAR ucPosPres;                /* position present flag */
    ZUCHAR ucTestPres;               /* test present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stPrefix;            /* element name prefix */
    ST_ZOS_SSTR stName;              /* element name */
    ZUINT iPos;                      /* position */
    ST_XCAPC_ATTRT stTest;           /* attr-test */
} ST_XCAPC_BAPOS;

/* xcapc element step */
typedef struct tagXCAPC_STEP
{
    ZUCHAR ucType;                   /* step type EN_XCAPC_STEP_TYPE */
    ZUCHAR ucAllPres;                /* all present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    union
    {
        ST_XCAPC_BNAME stName;       /* by-name */
        ST_XCAPC_BPOS stPos;         /* by-pos */
        ST_XCAPC_BATTR stAttr;       /* by-attr */
        ST_XCAPC_BAPOS stAPos;       /* by-pos-attr */
        ST_ZOS_SSTR stExtn;          /* extension-selector */
    } u;
} ST_XCAPC_STEP;

/* xcapc element step list */
XCAPC_TYPEDEF_LIST(XCAPC_STEP);

/* xcapc element-selector */
typedef struct tagXCAPC_ELEMS
{
    ST_XCAPC_STEP_LST stStepLst;     /* element step list */
} ST_XCAPC_ELEMS;

/* xcapc query */
typedef struct tagXCAPC_QRY
{
    ST_ZOS_SSTR stNsName;            /* namespace name */
    ST_ZOS_SSTR stPrefix;            /* prefix */
} ST_XCAPC_QRY;

/* xcapc query list */
XCAPC_TYPEDEF_LIST(XCAPC_QRY);

/* xcapc terminal-selector */
typedef struct tagXCAPC_TERMS
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucType;                   /* terminal type EN_XCAPC_TERMS_TYPE */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stAttr;          /* attribute-selector */
        ST_ZOS_SSTR stExtn;          /* extension-selector */
    } u;
} ST_XCAPC_TERMS;

/* xcapc node selector */
typedef struct tagXCAPC_NODES
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_XCAPC_ELEMS stElems;          /* element-selector */
    ST_XCAPC_TERMS stTerms;          /* terminal-selector */
} ST_XCAPC_NODES;

/* xcapc uri */
typedef struct tagXCAPC_URI
{
    ZDBUF zMemBuf;                 /* memory buffer */
    ST_XCAPC_SEG_LST stDocSegs;      /* document segments */
    ST_XCAPC_NODES stNodes;          /* node selector */
    ST_XCAPC_QRY_LST stQryLst;       /* query list */
} ST_XCAPC_URI;

/* xcapc request message */
typedef struct tagXCAPC_REQ_MSG
{
    ZDBUF zMemBuf;                   /* memory buffer */
    ZDBUF zBodyBuf;                  /* body buffer */
    ZUINT iMimeType;                 /* mime type EN_XCAPC_MIME_TYPE */
    ST_XCAPC_URI *pstUri;            /* xcapc uri */
    ST_ZOS_SSTR stAuthName;          /* auth name */
    ST_ZOS_SSTR stAuthPasswd;        /* auth password */
    ST_ZOS_SSTR stIntendId;          /* 3gpp intended identity */
    ST_ZOS_SSTR stMatchEtag;         /* if-match entity tag */
    ST_ZOS_SSTR stNoMatchEtag;       /* if-no-match entity tag */
    ST_ZOS_SSTR stExpert;            /* expert */
    ST_ZOS_SSTR stToken;             /* token */
    ST_ZOS_SSTR stXResolution;        /* X-Resolution */
    ST_XML_MSG *pstXmlMsg;           /* xml message */
} ST_XCAPC_REQ_MSG;

/* xcapc resonse message */
typedef struct tagXCAPC_RSP_MSG
{
    ZUINT iMimeType;                 /* mime type EN_XCAPC_MIME_TYPE */
    ZUINT iStatCode;                 /* stataus code */
    ST_ZOS_SSTR stEtag;              /* entity tag */
    ST_ZOS_USTR stHttpBody;          /* http body */
    ST_XML_MSG *pstXmlMsg;           /* xml message(decode) */
    ZDBUF zHttpMsg;                  /* http message */
} ST_XCAPC_RSP_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _XCAPC_TYPE_H__ */

