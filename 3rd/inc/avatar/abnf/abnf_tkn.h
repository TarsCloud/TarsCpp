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
  File name     : abnf_tkn.h
  Module        : abnf token management library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-03-09
  Description   :
      Data structure and function definitions 
      required by the abnf token management interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ABNF_TKN_H__
#define _ABNF_TKN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* abnf token item */
typedef struct tagABNF_TKN_ITEM
{
    ZCHAR *pcStr;                    /* token string */
    ZUSHORT wLen;                    /* token string length */
    ZUSHORT wTknId;                  /* token id */
    ZOS_PADX64
} ST_ABNF_TKN_ITEM;

/* abnf token table */
typedef struct tagABNF_TKN_TAB
{
    ZUSHORT wTknType;                /* token type */
    ZUSHORT wTableSize;              /* token table size */
    ZCONST ST_ABNF_TKN_ITEM *pstTable; /* token table */
} ST_ABNF_TKN_TAB;

/* abnf token manager create */    
ZFUNC ZTKNMGR Abnf_TknMgrCreate(ZUSHORT wProtocol, ZBOOL bIsNocase, 
                ZUSHORT wSize);

/* abnf token manager delete */
ZFUNC ZINT Abnf_TknMgrDelete(ZTKNMGR zTknMgr);

/* abnf token manager add subclass */
ZFUNC ZINT Abnf_TknAdd(ZTKNMGR zTknMgr, ZUSHORT wTknType, 
                ST_ZOS_SSTR *pstTkn, ZUSHORT wTknId);

/* abnf lookup the specific token */
ZFUNC ZTKNID Abnf_TknFind(ZTKNMGR zTknMgr, ZUSHORT wTknType, 
                ST_ZOS_SSTR *pstTkn);

/* abnf lookup the specific token */
ZFUNC ZTKNID Abnf_TknFindByKey(ZTKNMGR zTknMgr, ZUSHORT wTknType, 
                ZUINT iHashKey, ST_ZOS_SSTR *pstTkn);

#ifdef __cplusplus
}
#endif

#endif /* _ABNF_TKN_H__ */

