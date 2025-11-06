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
  File name     : httpc_ui.h
  Module        : http client protocol
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-03-30
  Description   :
    Marcos and structure definitions required by the http user interface.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTPC_UI_H__
#define _HTTPC_UI_H__

#ifdef __cplusplus
extern "C"{
#endif

/* http event status of session */
typedef enum EN_HTTPC_SESS_STAT_TYPE
{
    EN_HTTPC_SESS_STAT_CONN_ERR,      /* transport connect error */
    EN_HTTPC_SESS_STAT_CONNING,       /* transport connecting */
    EN_HTTPC_SESS_STAT_CONNED,        /* transport connected */
    EN_HTTPC_SESS_STAT_DISCED,        /* transport disconnected */
    EN_HTTPC_SESS_STAT_SEND_ERR,      /* send message error */
    EN_HTTPC_SESS_STAT_SENDING,       /* sending message */
    EN_HTTPC_SESS_STAT_SEND_OK,       /* send message ok */
    EN_HTTPC_SESS_STAT_RECV_ERR,      /* receive message error */
    EN_HTTPC_SESS_STAT_RECVING        /* receiving message */
} EN_HTTPC_SESS_STAT_TYPE;

/* httpc process session status */
typedef ZINT (*PFN_HTTPCPROCSTAT)(ZVOID * pSess, ZCOOKIE zCookie,
                ZUCHAR ucStatType, ZUINT iTptDataLen);

/* httpc process session message
   note: user can use the message for more process if return ZOK,
         otherwise the stack will delete it if return ZFAILED*/
typedef ZINT (*PFN_HTTPCPROCMSG)(ZVOID * pSess, ZCOOKIE zCookie,
                ST_HTTP_MSG *pstMsg);

/* httpc process session message headers,
   note: user is not allowed delete message */
typedef ZINT (*PFN_HTTPCPROCHDRS)(ZVOID * pSess, ZCOOKIE zCookie,
                ST_HTTP_MSG *pstMsg, ZBOOL bHasBody);

/* httpc process session message body
   note: user can use the data for more process if return ZOK,
         otherwise the stack will delete it if return ZFAILED */
typedef ZINT (*PFN_HTTPCPROCBODY)(ZVOID * pSess, ZCOOKIE zCookie,
                ZDBUF zBody, ZBOOL bLastBody);

/* httpc user interface usage
 * http stack just act as a http transport channel. 
 * user should construct http message when he need send message
 * user should keep session state by himself
 *
 * 1. user agent open session to remote server
 *   1) the call sequence is below:
 *     a. user should open a session by Httpc_Open
 *     b. user should use Httpc_RegCallback to register status callback
 *     c. user should use Httpc_Conn to connect remote http server
 *     d. user can use Httpc_Send to send http message to remote server
 *   2) user can use Httpc_Disc to disconnect the connection
 *      but the session will not delete
 *      if you want to send message, it must connect to server again.
 *   3) user can set remote address after disconnect the connection
 * 2. server accept session from remote user agent
 *   1) user should register callback by Httpc_UaReg after got accept status
 *   2) user can not connect to other server otherwise session is disconnected
 *   3) user can use other function like case 1
 * 3. server task should use Httpc_CfgSetServCallback to register 
 *    the status callback
 * 4. server will notify accpeted, connecting, connected, disconnected status
 * 5. session will not be deleted by stack, user must delete it by Httpc_Close
 *
 */

/* httpc session open */
ZFUNC ZINT Httpc_Open(ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstLclAddr, 
                PFN_HTTPCPROCSTAT pfnProcStat, PFN_HTTPCPROCMSG pfnProcMsg, 
                ZVOID **ppSess);

/* httpc session open with separate the messsage headers and body */
ZFUNC ZINT Httpc_OpenX(ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstLclAddr, 
                PFN_HTTPCPROCSTAT pfnProcStat, PFN_HTTPCPROCHDRS pfnProcHdrs, 
                PFN_HTTPCPROCBODY pfnProcBody, ZVOID **ppSess);

/* http client session open for receive file */
ZFUNC ZINT Httpc_OpenF(ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstLclAddr, 
                PFN_HTTPCPROCSTAT pfnProcStat, PFN_HTTPCPROCMSG pfnProcMsg, 
                ZUINT iFileSize, ZCHAR *pcFileType, ZCHAR *pcFileName, 
                ZUINT iStartOffset, ZUINT iStopOffset, ZVOID **ppSess);

/* httpc session open with ssl */
ZFUNC ZINT Httpc_OpenS(ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstLclAddr, 
                PFN_HTTPCPROCSTAT pfnProcStat, PFN_HTTPCPROCMSG pfnProcMsg,
                ZVOID **ppSess);

/* httpc session open with separate the messsage headers and body with ssl */
ZFUNC ZINT Httpc_OpenSX(ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstLclAddr, 
                PFN_HTTPCPROCSTAT pfnProcStat, PFN_HTTPCPROCHDRS pfnProcHdrs, 
                PFN_HTTPCPROCBODY pfnProcBody, ZVOID **ppSess);

/* http client session open for receive file with tls */
ZFUNC ZINT Httpc_OpenSF(ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstLclAddr, 
                PFN_HTTPCPROCSTAT pfnProcStat, PFN_HTTPCPROCMSG pfnProcMsg, 
                ZUINT iFileSize, ZCHAR *pcFileType, ZCHAR *pcFileName, 
                ZUINT iStartOffset, ZUINT iStopOffset, ZVOID **ppSess);

/* httpc session close */
ZFUNC ZINT Httpc_Close(ZVOID * pSess);

/* httpc session connect to remote server */
ZFUNC ZINT Httpc_Conn(ZVOID * pSess, ZCONST ZCHAR *pcSrvIp, ZUSHORT wIpLen, 
                ZUSHORT wSrvPort);

/* httpc session disconnect to remote */
ZFUNC ZINT Httpc_Disc(ZVOID * pSess);

/* httpc session send message */
ZFUNC ZINT Httpc_Send(ZVOID * pSess, ST_HTTP_MSG *pstMsg);

/* httpc session send data buffer */
ZFUNC ZINT Httpc_SendData(ZVOID * pSess, ZDBUF zData);

/* httpc session send data string */
ZFUNC ZINT Httpc_SendDataX(ZVOID * pSess, ST_ZOS_SSTR *pstData);

/* httpc session get user id */
ZFUNC ZINT Httpc_GetCookie(ZVOID * pSess, ZCOOKIE *pzCookie);

/* httpc session set user id */
ZFUNC ZINT Httpc_SetCookie(ZVOID * pSess, ZCOOKIE zCookie);

/* httpc session get remote address */
ZFUNC ZINT Httpc_GetRmtAddr(ZVOID * pSess, ST_ZOS_INET_ADDR *pstAddr);

/* httpc session set remote address */
ZFUNC ZINT Httpc_SetRmtAddr(ZVOID * pSess, ST_ZOS_INET_ADDR *pstAddr);

/* http client session set content length */
ZFUNC ZINT Httpc_SetContentLen(ZVOID * pSess, ZUINT iContentLen);

#ifdef __cplusplus
}
#endif

#endif /* _HTTPC_UI_H__ */

