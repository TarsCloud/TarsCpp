/************************************************************************

        Copyright (c) 2005-2016 by Juphoon System Software, Inc.
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
  File name     : dtls_ui.h
  Module        : dtls protocol user interface
  Author        : kerwin
  Version       : 0.1
  Created on    : 2016-07-18
  Description   :
    Marcos and structure definitions required by the dtls user interface.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DTLS_UI_H__
#define _DTLS_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* zos support cnpc */
#ifdef ZOS_SUPT_CNPC

/* dtls session identifier */
typedef ZVOID * DTLS_SESSID;

typedef enum tagEN_DTLS_RECEIVE_PACKET_TYPE {
    EN_DTLS_RECEIVE_PKT_FINISHED = 0,
    EN_DTLS_RECEIVE_PKT_NO_DTLSPKT,
    EN_DTLS_RECEIVE_PKT_OK,
    EN_DTLS_RECEIVE_PKT_ERROR,
}EN_DTLS_RECEIVE_PACKET_TYPE;

typedef enum tagEN_DTLS_CERT_TYPE {
    EN_DTLS_CERT_TYPE_NONE = 0,
    EN_DTLS_CERT_TYPE_HARD,
    EN_DTLS_CERT_TYPE_SOFT,
}EN_DTLS_CERT_TYPE;

typedef enum tagEN_DTLS_PEER_CERT_VERIFY_RESULT {
    EN_DTLS_PEER_CERT_VERIFY_OK = 0,   /* verify by CA ok */
    EN_DTLS_PEER_CERT_VERIFY_FAILED,   /* verify by CA failed */
}EN_DTLS_PEER_CERT_VERIFY_RESULT;

/* dtls Context init */
ZFUNC ZINT Dtls_CtxInit(ZDTLSID *pzCtxId, ZINT *piCertType, ZVOID **pstCertOut,
                ZCHAR *pcSerialNumber);

/* dtls session init */
ZFUNC ZINT Dtls_SessInit(ZDTLSID zCtxId, DTLS_SESSID *pzSessId, 
                ZVOID * pfnDtlsSendToCb, 
                ZVOID * pfnDtlsSchedCb);

/* dtls handshake */
ZFUNC ZINT Dtls_Handshake(ZVOID *pUserInfo, DTLS_SESSID zDtlsSessId);

/* dtls recv udp data */
ZFUNC ZINT Dtls_RecvData(ZVOID *pUserInfo, DTLS_SESSID zDtlsSessId,
                ZVOID *pData, ZINT iDataLen, ZVOID **ppzPeerCert,
                ZCHAR *pcUpstreamKey, ZCHAR *pcDownstreamKey);

/* dtls session get the remote fingerprint 
The caller is responsible for freeing it by Zos_SysStrFree */
ZFUNC ZCHAR * Dtls_SessGetRmtFgrprnt(DTLS_SESSID zSessId, ZCHAR *pcAlgo);

/* dtls verify peer certificate by local CA files */
ZFUNC ZINT Dtls_VerifyPeerCertByCA(ZVOID *pzPeerCert, ZCHAR *pcCaFilePath,
                ZCHAR *pcCertHash, int *piCertHashLen);

#endif /* ZOS_SUPT_CNPC */

#ifdef __cplusplus
}
#endif

#endif /* _DTLS_UI_H__ */

