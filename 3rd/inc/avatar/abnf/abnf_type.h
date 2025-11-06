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
  File name     : abnf_type.h
  Module        : abnf typedefs
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-12-29
  Description   :
      Data structure and function definitions required by the abnf interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ABNF_TYPE_H__
#define _ABNF_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif
 
#define ABNF_MAX_SEPA 3             /* decode spearater maximum */
#define ABNF_MAX_EXTNPARM 3         /* decode spearater maximum */

#define ABNF_TKNID_INVALID ZMAXUINT /* token id invalid */
#define ABNF_TKNID_UNKNOWN (ZMAXUINT - 1) /* token id unknown */

/* abnf character set */
#define ABNF_CHRCLASS_NA   0        /* character set class not applicable */
#define ABNF_CHRSETID_NA  ZNULL     /* character set id not applicable */

/* abnf length */
#define ABNF_MIN_LEN_NA 0           /* minimum length not applicable */
#define ABNF_MAX_LEN_NA 0           /* maximum length not applicable */

/* abnf value range */
#define ABNF_MIN_VAL_NA ZMAXUINT   /* minimum length not applicable */
#define ABNF_MAX_VAL_NA ZMAXUINT   /* maximum length not applicable */

/* abnf characters */
typedef enum EN_ABNF_CHR
{
    EN_CHR_EOS = 0x00,               /* end '\0' */

    EN_CHR_HT = 0x09,                /* horizontal tab */
    EN_CHR_LF = 0x0A,                /* \n */
    EN_CHR_VT = 0x0B,                /* vertical tab */
    EN_CHR_CR = 0x0D,                /* \r */
    EN_CHR_WSP = 0x20,               /* white space */
    EN_CHR_DQUOTE = 0x22,            /* " */
    EN_CHR_HASH = 0x23,              /* # */
    EN_CHR_DOLLAR = 0x24,            /* $ */
    EN_CHR_LPRTS = 0x28,             /* ( */
    EN_CHR_RPRTS = 0x29,             /* ) */
    EN_CHR_STAR = 0x2A,              /* * */
    EN_CHR_COMMA = 0x2C,             /* , */
    EN_CHR_DASH = 0x2D,              /* - */
    EN_CHR_DOT = 0x2E,               /* . */
    EN_CHR_SLASH = 0x2F,             /* / */
    EN_CHR_COLON = 0x3A,             /* : */
    EN_CHR_SEMI = 0x3B,              /* ; */
    EN_CHR_LESS = 0x3C,              /* < */
    EN_CHR_EQUAL = 0x3D,             /* = */
    EN_CHR_GREATER = 0x3E,           /* > */
    EN_CHR_LSQBRKT = 0x5B,           /* [ */
    EN_CHR_RSQBRKT = 0x5D,           /* ] */
    EN_CHR_LBRKT = 0x7B,             /* { */ 
    EN_CHR_RBRKT = 0x7D,             /* } */

    EN_CHR_GENRALSTR = 0x100         /* general token */
} EN_ABNF_CHR;

/* abnf error info */
typedef struct tagABNF_ERR_INFO
{
    ZCHAR *pcFirstErrStr;            /* the first error string */
    ZDUMP zDumpId;                   /* abnf dump stack */
    ZUSHORT wAbnfCode;               /* abnf error code */
    ZUSHORT wErrNum;                 /* abnf user error number */
    ZOS_PADX64
} ST_ABNF_ERR_INFO;

/* abnf buffer info */
typedef struct tagABNF_BUF_INFO
{
    ZCHAR *pcOrg;                    /* origin message pointer */
    ZCHAR *pcCur;                    /* current message pointer */
    ZCHAR *pcScan;                   /* scanned message pointer */
    ZUINT iOrgLen;                   /* origin message length */
    ZUINT iCurLen;                   /* current message length */
    ZUINT iScanLen;                  /* scanned message length */
    ZOS_PADX64
} ST_ABNF_BUF_INFO;

/* abnf character info */
typedef struct tagABNF_CHR_INFO
{
    ZUCHAR ucIsUseLwsp;              /* is LWSP valid in the string */
    ZUCHAR ucIsUseWsp;               /* is WSP valid in the string */
    ZUCHAR ucEscapeChr;              /* escape character */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUINT iEscapeClass;              /* escape character property */
    ZCHRSET zChrset;                 /* character set */
    ZUINT iClass;                    /* character property */
    ZUINT iMinLen;                   /* minimum length */
    ZUINT iMaxLen;                   /* maximum length */
    ZUINT iMinVal;                   /* minimum value */
    ZUINT iMaxVal;                   /* maximum value */
    ZUINT iTknType;                  /* protocol token type */    
    ZTKNMGR zTknMgr;                 /* token manager */
} ST_ABNF_CHR_INFO;

/* abnf decode separator info */
typedef struct tagABNF_SEPA_INFO
{ 
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucLastSepa;               /* last separator character */
    ZUCHAR ucEolSepaSize;            /* last separator chr size with eol */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUCHAR aucSepa[4];               /* only support 4 separator characters */
} ST_ABNF_SEPA_INFO;

/* abnf extension info */
typedef struct tagABNF_EXTN_INFO
{
    ZUINT aiParm[2];                 /* only support 2 extend parameters */
} ST_ABNF_EXTN_INFO;

/* abnf message */
typedef struct tagABNF_MSG
{
    ZUINT iProtocol;                 /* protocol type */
    ZOS_PADX64
    ZDBUF zMemBuf;                   /* data memory buffer */
    ST_ABNF_ERR_INFO *pstErr;        /* error info */
    ST_ABNF_BUF_INFO stBuf;          /* data buffer */
    ST_ABNF_BUF_INFO stSavedBuf;     /* saved data buffer */    
    ST_ABNF_CHR_INFO stChr;          /* character property */
    ST_ABNF_SEPA_INFO stSepa;        /* separator character info */
    ST_ABNF_EXTN_INFO stExtn;        /* extend info */
} ST_ABNF_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _ABNF_TYPE_H__ */

