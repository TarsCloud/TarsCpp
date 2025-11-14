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
  File name     : tls_ui.h
  Module        : tls protocol user interface
  Author        : zocol
  Version       : 0.1
  Created on    : 2008-12-16
  Description   :
    Marcos and structure definitions required by the tls user interface.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _TLS_UI_H__
#define _TLS_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* tls Tls_SessRead blocked */
#define TLS_WRITE_BLOCK 2
#define TLS_READ_BLOCK 3

/* tls method type */
typedef enum EN_TLS_METHOD_TYPE
{
    EN_TLS_METHOD_TLS1,              /* TLSv1.0 method */
    EN_TLS_METHOD_TLS1_1,            /* TLSv1.1 method */
    EN_TLS_METHOD_TLS1_2,            /* TLSv1.2 method */
} EN_TLS_METHOD_TYPE;

/* tls verify type */
typedef enum EN_TLS_VERY_TYPE
{
    EN_TLS_VERY_NONE,                /* verify none */
    EN_TLS_VERY_OPTION,              /* verify option */
    EN_TLS_VERY_OPTION_NO_CA,        /* verify option with no ca */
    EN_TLS_VERY_REQUIRE              /* verify require */
} EN_TLS_VERY_TYPE;

/* tls session identifier */
typedef ZVOID * TLS_SESSID;

/* tls create a context */
ZFUNC ZINT Tls_CtxCreate(ZUCHAR ucMethodType, ZUCHAR ucVeryType, ZINT iVeryDepth,
                ZCHAR *pcCiphers, ZCHAR *pcTrustCaFile, ZCHAR *pcTrustCaPath, 
                ZCHAR *pcCertFile, ZCHAR *pcPrvKeyFile, ZCHAR *pcPrvKeyPasswd, 
                ZTLSID *pzCtxId);

/* tls delete a context */
ZFUNC ZINT Tls_CtxDelete(ZTLSID zCtxId);

/* tls create a session */
ZFUNC ZINT Tls_SessCreate(ZTLSID zCtxId, ZSOCKET zSockFd, ZBOOL bNonBlking, 
                ZBOOL bCertHost, ZCOOKIE zCookie, TLS_SESSID *pzSessId);

/* tls delete a session */
ZFUNC ZINT Tls_SessDelete(TLS_SESSID zSessId);

/* tls shutdown a session */
ZFUNC ZINT Tls_SessShutdown(TLS_SESSID zSessId);

/* tls fork a session */
ZFUNC ZINT Tls_SessFork(ZTLSID zOldCtxId, ZSOCKET zSockFd, ZBOOL bNonBlking,
                TLS_SESSID *pzNewSessId);

/* tls session client connect to server */
ZFUNC ZINT Tls_SessConnect(TLS_SESSID zSessId);

/* tls session server accept from client */
ZFUNC ZINT Tls_SessAccept(TLS_SESSID zSessId);

/* tls session read data from connection */
ZFUNC ZINT Tls_SessRead(TLS_SESSID zSessId, ZUCHAR *pucData, ZUINT *piLen, 
                ZBOOL *pbClose);

/* tls session write data to connection */
ZFUNC ZINT Tls_SessWrite(TLS_SESSID zSessId, ZUCHAR *pucData, ZUINT *piLen, 
                ZBOOL *pbClose, ZBOOL *pbWait);

/* tls session find by remmote address */
ZFUNC ZINT Tls_SessFind(ST_ZOS_INET_ADDR *pstRmtAddr, TLS_SESSID *pzSessId);

/* tls session get the context id */
ZFUNC ZINT Tls_SessGetCtxId(TLS_SESSID zSessId, ZTLSID *pzCtxId);

/* tls session get the user id */
ZFUNC ZINT Tls_SessGetCookie(TLS_SESSID zSessId, ZCOOKIE *pzCookie);

/* tls session set the user id */
ZFUNC ZINT Tls_SessSetCookie(TLS_SESSID zSessId, ZCOOKIE zCookie);

/* tls session get the non-blocking option */
ZFUNC ZINT Tls_SessGetNonBlking(TLS_SESSID zSessId, ZBOOL *pbNonBlking);

/* tls session set the non-blocking option */
ZFUNC ZINT Tls_SessSetNonBlking(TLS_SESSID zSessId, ZBOOL bNonBlking);

/* tls session get the certificate host option */
ZFUNC ZINT Tls_SessGetCertHost(TLS_SESSID zSessId, ZBOOL *pbCertHost);

/* tls session set the certificate host option */
ZFUNC ZINT Tls_SessSetCertHost(TLS_SESSID zSessId, ZBOOL bCertHost);

/* tls session get the remote address */
ZFUNC ZINT Tls_SessGetRmtAddr(TLS_SESSID zSessId, ST_ZOS_INET_ADDR *pstAddr);

/* tls session set the remote address */
ZFUNC ZINT Tls_SessSetRmtAddr(TLS_SESSID zSessId, ST_ZOS_INET_ADDR *pstAddr);

/* tls session get the host name */
ZFUNC ZINT Tls_SessGetHost(TLS_SESSID zSessId, ZCHAR **ppcHost);

/* tls session set the host name */
ZFUNC ZINT Tls_SessSetHost(TLS_SESSID zSessId, ZCHAR *pcHost);

/* tls session get the remote fingerprint 
The caller is responsible for freeing it by Zos_SysStrFree */
ZFUNC ZCHAR * Tls_SessGetRmtFgrprnt(TLS_SESSID zSessId, ZCHAR *pcAlgo);

#ifdef __cplusplus
}
#endif

#endif /* _TLS_UI_H__ */

