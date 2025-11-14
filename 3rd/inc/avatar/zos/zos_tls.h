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
  File name     : zos_tls.h
  Module        : zos tls library functions
  Author        : shuai.xu  
  Version       : 0.1   
  Created on    : 2015-1-05    
  Description   :
      Data structure and function definitions required by the tls interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_TLS_H__
#define _ZOS_TLS_H__

#define ZOS_TLS_BUF_LEN 2048

#define ZOS_RET_PENDING 0xED

#ifdef __cplusplus
extern "C" {
#endif

/* zos tls process type */
typedef enum EN_SOCK_TLS_TYPE
{
    EN_SOCK_TLS_NONE,                /* not tls socket */
    EN_SOCK_TLS_DRVSELF,             /* tls socket and process tls by us */
    EN_SOCK_TLS_DRVSYS,              /* tls socket and process tls by system */
}EN_SOCK_TLS_TYPE;

/* zos tls init */
ZINT Zos_TlsInit(ZVOID** ppstSsl, ZVOID** ppstBIORead,
                        ZVOID** ppstBIOWrite, ZVOID* zTlsCtxId,
                        ZCHAR *pcSrvName);

/* zos tls start handshake */
ZINT Zos_TlsConnect(ZVOID* pstSsl, ZVOID* pstBIOWrite, ZSOCKET zSock);

/* zos tls shut down connect */
ZINT Zos_TlsShutdown(ZVOID* pstSsl);

/* zos tls process handshake recv */
ZINT Zos_TlsConnectRecv(ZVOID* pstSsl, ZVOID* pstBIORead, 
                        ZVOID* pstBIOWrite, ZUCHAR* pucData, ZUINT iLen, 
                        ZSOCKET zSock);

/* zos tls send penging data */
ZINT Zos_TlsSendPendingDataOut(ZVOID* pstSsl,ZVOID* pstBIOWrite, 
                        ZSOCKET zSock, ZBOOL *pbClosed, ZBOOL *pbWait);

/* zos tls send data */
ZINT Zos_TlsGetDataOut(ZVOID* pstSsl,ZVOID* pstBIOWrite, ZUINT iLen, 
                            ZDBUF *pzData);

/* zos tls add data to send buffer */
ZINT Zos_TlsAddSendBuff(ZSOCKET zSock, ZUCHAR* pucData, ZUINT iLen,
                        ZUINT iOffset);

/* zos tls send encrypt data */
ZINT Zos_TlsEncryptSend(ZVOID* pstSsl, ZVOID* pstBIOWrite,  ZUCHAR* pucData,
                ZUINT iLen, ZSOCKET zSock, ZBOOL *pbClosed, ZBOOL *pbWait);

/* zos tls encrypt data */
ZINT Zos_TlsEncrypt(ZVOID* pstSsl, ZVOID* pstBIOWrite, ZUCHAR* pucData, 
                       ZUINT iLen, ZDBUF *pzData);

/* zos tls encrypt data by socket */
ZINT Zos_TlsEncryptX(ZSOCKET zSock, ZUCHAR* pucData, ZUINT iLen, 
                       ZDBUF *pzData);

/* zos tls decrypt data */
ZINT Zos_TlsDecrypt(ZVOID* pstSsl, ZVOID* pstBIORead, ZVOID* pstBIOWrite,
                        ZUCHAR* pucData, ZUINT* piLen, ZSOCKET zSock);

/* zos tls accrpt */
ZINT Zos_TlsAccept(ZVOID *pstSsl);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_TLS_H__ */

