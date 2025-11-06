/************************************************************************

        Copyright (c) 2005-2015 by Juphoon System Software, Inc.
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
  File name     : zgba.h
  Module        : GBA
  Author        : phil.zhou 
  Version       : 0.1   
  Created on    : 2015-04-15    
  Description   :
      Data structure and function definitions required by the GBA.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZGBA_H__
#define _ZGBA_H__

#ifdef __cplusplus
extern "C"{
#endif

#define ZGBA_ERR_OTHER 0x0001
#define ZGBA_ERR_AUTH_FAIL 0x0002
#define ZGBA_ERR_PICK_XML 0x0003
#define ZGBA_ERR_TIMEOUT 0x0004
#define ZGBA_ERR_NETWORK 0x0005

/** @brief Type define of GBA callback for notify authorization indication. */
typedef ZVOID (*PFN_ZGBAAUTHIND)(ZCOOKIE zCookie, ZCHAR *pcNonce);

/** @brief Type define of GBA callback for notify btid expired. */
typedef ZVOID (*PFN_ZGBABTIDEXPIRED)(ZFUNC_VOID);

/** @brief Type define of GBA callback for notify GBA_ME bootstrapping mode operation ok. */
typedef ZVOID (*PFN_ZGBAMEBOOTOK)(ZUINT iBootId, ZCOOKIE zCookie);

/** @brief Type define of GBA callback for notify GBA_ME bootstrapping mode operation failed. */
typedef ZVOID (*PFN_ZGBAMEBOOTFAILED)(ZUINT iBootId, ZCOOKIE zCookie, 
                ZUINT iStatCode);

/** @brief Type define of GBA bootstrapping type */
typedef enum EN_ZGBA_BOOT_TYPE
{
    EN_ZGBA_BOOT_UNKNWON,
    EN_ZGBA_BOOT_ME,
    EN_ZGBA_BOOT_UICC,
} EN_ZGBA_BOOT_TYPE;

/** @brief Type define of GBA Ks_NAF type */
typedef enum EN_ZGBA_KSNAF_TYPE
{
    EN_ZGBA_KSNAF_STRING,
    EN_ZGBA_KSNAF_BASE64,
} EN_ZGBA_KSNAF_TYPE;

/** @brief Type define of ua security protocol identifier type */
typedef enum EN_ZGBA_UA_SEC_PROT_TYPE
{
    EN_ZGBA_UA_SEC_PROT_HTTP,
    EN_ZGBA_UA_SEC_PROT_IMAP,
} EN_ZGBA_UA_SEC_PROT_TYPE;

/* GBA init */
ZFUNC ZINT ZGba_Init();

/* GBA destory */
ZFUNC ZVOID ZGba_Destory();

/* GBA realm get bootstrapping type */
ZFUNC ZUINT ZGba_RealmGetBootType(ZCHAR *pcRealm, ZUINT iLen);

/* GBA bootstrapping start */
ZFUNC ZUINT ZGba_BootStart(ZUINT iBootType, ZCOOKIE zCookie, 
                PFN_ZGBAMEBOOTOK pfnBootOk, PFN_ZGBAMEBOOTFAILED pfnBootFailed);

/* GBA bootstrapping stop */
ZFUNC ZVOID ZGba_BootStop(ZUINT iBootId);

/* GBA authorization get Ks_NAF, see EN_ZGBA_UA_SEC_PROT_TYPE, see EN_ZGBA_KSNAF_TYPE */
ZFUNC ZCHAR * ZGba_AuthGetKsNaf(ZCHAR *pcNafFqdn, ZUINT iUaSecProt, ZUINT iType);

/* GBA authorization get B-TID */
ZFUNC ZCHAR * ZGba_AuthGetBTid();

/* GBA authorization check is valid */
ZFUNC ZBOOL ZGba_AuthChkValid();

/* GBA authorization set local uri */
ZFUNC ZINT ZGba_AuthSetLclUri(ZCHAR *pcUri);

/* GBA authorization set realm */
ZFUNC ZINT ZGba_AuthSetRealm(ZCHAR *pcRealm);

/* GBA authorization set host name */
ZFUNC ZINT ZGba_AuthSetHostName(ZCHAR *pcName);

/* GBA authorization set host port */
ZFUNC ZINT ZGba_AuthSetHostPort(ZUSHORT wPort);

/* GBA authorization set authorization indication callback */
ZFUNC ZINT ZGba_AuthSetAuthIndCb(PFN_ZGBAAUTHIND pfnAuthInd);

/* GBA authorization set btid expired callback */
ZFUNC ZINT ZGba_AuthSetBtidExpiredCb(PFN_ZGBABTIDEXPIRED pfnBtidExpired);

/* GBA authorization set Ks_ext_NAF */
ZFUNC ZINT ZGba_AuthSetKsExtNaf(ZCHAR *pcKsExtNaf);

/* GBA authorization enter security context */
ZFUNC ZINT ZGba_AuthEnterSecCtx(ZCOOKIE zCookie, ZCHAR *pcRsp, ZCHAR *pcIk, 
                ZCHAR *pcCk, ZCHAR *pcAuts);

#ifdef __cplusplus
}
#endif

#endif /* _ZGBA_H__ */

