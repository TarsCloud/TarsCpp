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
  File name     : zvcf.h
  Module        : process vcard file functions
  Author        : phil.zhou   
  Version       : 0.1   
  Created on    : 2012-05-29    
  Description   :
      Data structure and function definitions required by the 
      process vcard file interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZVCF_H__
#define _ZVCF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* zvcf file encoding */
typedef enum EN_ZVCF_ENCODING_TYPE
{
    EN_ZVCF_ENCODING_UTF8,              /* utf-8 */
    EN_ZVCF_ENCODING_UTF16,             /* utf-16 */
    EN_ZVCF_ENCODING_GBK,               /* gb-18030 */
    EN_ZVCF_ENCODING_SIZE
} EN_ZVCF_ENCODING_TYPE;

/* zvcf vcard field name type */
typedef enum EN_ZVCF_VCARD_FIELD_TYPE
{
    EN_ZVCF_VCARD_FIELD_SOURCE,         /* source */
    EN_ZVCF_VCARD_FIELD_NAME,           /* name */
    EN_ZVCF_VCARD_FIELD_PROFILE,        /* profile */
    EN_ZVCF_VCARD_FIELD_BEGIN,          /* begin */
    EN_ZVCF_VCARD_FIELD_END,            /* end */
    EN_ZVCF_VCARD_FIELD_FN,             /* fn */
    EN_ZVCF_VCARD_FIELD_N,              /* n */
    EN_ZVCF_VCARD_FIELD_NICKNAME,       /* nickname */
    EN_ZVCF_VCARD_FIELD_PHOTO,          /* photo */
    EN_ZVCF_VCARD_FIELD_BDAY,           /* birthday */
    EN_ZVCF_VCARD_FIELD_ADR,            /* address */
    EN_ZVCF_VCARD_FIELD_LABEL,          /* label */
    EN_ZVCF_VCARD_FIELD_TEL,            /* telephone */
    EN_ZVCF_VCARD_FIELD_EMAIL,          /* email */
    EN_ZVCF_VCARD_FIELD_MAILER,         /* mailer */
    EN_ZVCF_VCARD_FIELD_TZ,             /* tz */
    EN_ZVCF_VCARD_FIELD_GEO,            /* geo */
    EN_ZVCF_VCARD_FIELD_TITLE,          /* title */
    EN_ZVCF_VCARD_FIELD_ORG,            /* organization */
    EN_ZVCF_VCARD_FIELD_URL,            /* url */
    EN_ZVCF_VCARD_FIELD_UNKNOWN
} EN_ZVCF_VCARD_FIELD_TYPE;

/* zvcf vcard field value type */
typedef enum EN_ZVCF_VCARD_FIELDVAL_TYPE
{
    EN_ZVCF_VCARD_FIELDVAL_POB = 0,     /* post office box */    
    EN_ZVCF_VCARD_FIELDVAL_EA = 1,      /* extended address */   
    EN_ZVCF_VCARD_FIELDVAL_SA = 2,      /* street adress */      
    EN_ZVCF_VCARD_FIELDVAL_LOCALITY = 3,/* locality */           
    EN_ZVCF_VCARD_FIELDVAL_REGION = 4,  /* region */             
    EN_ZVCF_VCARD_FIELDVAL_PC = 5,      /* postal code */        
    EN_ZVCF_VCARD_FIELDVAL_CN = 6,      /* country name */       
    EN_ZVCF_VCARD_FIELDVAL_ORGN = 0,    /* organizational name */
    EN_ZVCF_VCARD_FIELDVAL_DEPTN = 1,   /* department name */    
    EN_ZVCF_VCARD_FIELDVAL_UNKNOWN = 9
} EN_ZVCF_VCARD_FIELDVAL_TYPE;


/* zvcf vcard parameter name type */
typedef enum EN_ZVCF_VCARD_PARAM_TYPE
{
    EN_ZVCF_VCARD_PARA_ENCODING,        /* encoding */
    EN_ZVCF_VCARD_PARA_VALUE,           /* value */
    EN_ZVCF_VCARD_PARA_CHARSET,         /* charset */
    EN_ZVCF_VCARD_PARA_LANGUAGE,        /* language */
    EN_ZVCF_VCARD_PARA_CONTEXT,         /* context */
    EN_ZVCF_VCARD_PARA_TYPE,            /* type */
    EN_ZVCF_VCARD_PARA_UNKNOWN
} EN_ZVCF_VCARD_PARAM_TYPE;

/* zvcf vcard parameter value type */
typedef enum EN_ZVCF_VCARD_PARAMVAL_TYPE
{
    EN_ZVCF_VCARD_PARAVAL_PREF,         /* pref */
    EN_ZVCF_VCARD_PARAVAL_DOM,          /* dom */
    EN_ZVCF_VCARD_PARAVAL_INTL,         /* intl */
    EN_ZVCF_VCARD_PARAVAL_PARCEL,       /* parcel */
    EN_ZVCF_VCARD_PARAVAL_POSTAL,       /* postal */
    EN_ZVCF_VCARD_PARAVAL_HOME,         /* home */
    EN_ZVCF_VCARD_PARAVAL_HOMEFAX,      /* home fax */
    EN_ZVCF_VCARD_PARAVAL_WORK,         /* work */
    EN_ZVCF_VCARD_PARAVAL_WORKFAX,      /* work fax */
    EN_ZVCF_VCARD_PARAVAL_MSG,          /* msg */
    EN_ZVCF_VCARD_PARAVAL_VOICE,        /* voice */
    EN_ZVCF_VCARD_PARAVAL_FAX,          /* fax */
    EN_ZVCF_VCARD_PARAVAL_CELL,         /* cell */
    EN_ZVCF_VCARD_PARAVAL_VIDEO,        /* video */
    EN_ZVCF_VCARD_PARAVAL_PAGER,        /* pager */
    EN_ZVCF_VCARD_PARAVAL_BBS,          /* bbs */
    EN_ZVCF_VCARD_PARAVAL_MODEM,        /* modem */
    EN_ZVCF_VCARD_PARAVAL_CAR,          /* car */
    EN_ZVCF_VCARD_PARAVAL_ISDN,         /* isdn */
    EN_ZVCF_VCARD_PARAVAL_PCS,          /* pcs */
    EN_ZVCF_VCARD_PARAVAL_INTERNET,     /* internet */
    EN_ZVCF_VCARD_PARAVAL_PROFILE,      /* profile */
    EN_ZVCF_VCARD_PARAVAL_BLOG,         /* blog */
    EN_ZVCF_VCARD_PARAVAL_UNKNOWN
} EN_ZVCF_VCARD_PARAMVAL_TYPE;

/* zvcf id */
typedef ZVOID * ZVCF;

/* zvcf create */
ZFUNC ZINT Zvcf_Create(ZVCF *pzVcf);

/* zvcf delete */
ZFUNC ZINT Zvcf_Delete(ZVCF zVcf);

/* zvcf load from file */
ZFUNC ZINT Zvcf_LoadFile(ZCHAR *pcFileName, ZVCF *pzVcf);

/* zvcf load save file */
ZFUNC ZINT Zvcf_SaveFile(ZVCF zVcf, ZCHAR *pcFileName, ZUINT iEncoding);

/* zvcf add entry */
ZFUNC ZINT Zvcf_AddEntry(ZVCF zVcf);

/* zvcf get entry size */
ZFUNC ZINT Zvcf_GetEntrySize(ZVCF zVcf, ZUINT *piSize);

/* zvcf get entry field size by field type */
ZFUNC ZINT Zvcf_GetEntryFieldSize(ZVCF zVcf, ZUINT iEntryNo, 
                ZUINT iFieldType, ZUINT *piSize);

/* zvcf get entry field value by field type */
ZFUNC ZINT Zvcf_GetEntryFieldValByType(ZVCF zVcf, ZUINT iEntryNo, 
                ZUINT iFieldType, ST_ZOS_SSTR *pstStr);

/* zvcf get entry field value by field type, field value type and index */
ZFUNC ZINT Zvcf_GetEntryFieldValByTypeX(ZVCF zVcf, ZUINT iEntryNo, 
                ZUINT iFieldType, ZUINT iFieldValType, ZUINT iIndex, 
                ZUINT *piParaValType, ST_ZOS_SSTR *pstStr);

/* zvcf put field into entry by filed type, paramter type and paramter value type, type see 
EN_ZVCF_VCARD_FIELD_TYPE, EN_ZVCF_VCARD_PARAM_TYPE and EN_ZVCF_VCARD_PARAMVAL_TYPE */
ZFUNC ZINT Zvcf_PutField(ZVCF zVcf, ZUINT iFiledType, ZUINT iParaType, 
                ZUINT iParaValue, ZBOOL bPref);

/* zvcf put value into entry by field value type, type see EN_ZVCF_VCARD_VALUE_TYPE */
ZFUNC ZINT Zvcf_PutValue(ZVCF zVcf, ZUINT iValueType, ST_ZOS_SSTR *pstStr);

#ifdef __cplusplus
}
#endif

#endif /* _ZVCF_H__ */

