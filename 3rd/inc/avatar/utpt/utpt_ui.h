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
  File name     : utpt_ui.h
  Module        : unify transport
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the utpt user interface.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _UTPT_UI_H__
#define _UTPT_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* utpt invalid connection id */
#define UTPT_INV_ID ZMAXUINT

/* utpt return tcp client connecting */
#define UTPT_RET_CONNING 0xFE

/* utpt return tcp client wait data */
#define UTPT_RET_WAITDATA 0xEE

/* utpt return result is pending */
#define UTPT_RET_PENDING 0xED

/* utpt return invalid id */
#define UTPT_RET_INVID 0xFD

/* utpt transport tls password length */
#define UTPT_TPT_PASSWD_LEN  64
/* utpt transport tls server name length */
#define UTPT_TLS_SRV_NAME_LEN 64

/* utpt support record transport time */
#ifdef UTPT_SUPT_RECORD_TIME

/* utpt record time reset */
#define UTPT_RECORD_RESET(_buf, _len) do { \
    ZUINT *piTimeCnt = (ZUINT *)((ZUCHAR *)(_buf) + ZOS_ALIGN(_len)); \
    *piTimeCnt = 0; \
} while (0)

/* utpt record time in transport buffer */
#define UTPT_RECORD_TIME(_buf, _len) do { \
    ZUINT *piTimeCnt = (ZUINT *)((ZUCHAR *)(_buf) + ZOS_ALIGN(_len)); \
    ZHRTIME_T *pzTime = (ZHRTIME_T *)(piTimeCnt + 1) + (*piTimeCnt); \
    *pzTime = Zos_GetHrTime(); \
    *piTimeCnt += 1; \
} while (0)

/* utpt record time in transport buffer */
#define UTPT_GET_TIME_CNT(_buf, _len) \
    *((ZUINT *)((ZUCHAR *)(_buf) + ZOS_ALIGN(_len)))
    
/* utpt record time in transport buffer */
#define UTPT_GET_TIME_SIZE(_buf, _len) \
    ((*((ZUINT *)((ZUCHAR *)(_buf) + ZOS_ALIGN(_len)))) * sizeof(ZHRTIME_T) \
      + sizeof(ZUINT))
    
/* utpt record time in transport buffer */
#define UTPT_GET_TIME_PTR(_buf, _len) \
    ((ZUCHAR *)(_buf) + ZOS_ALIGN(_len))

#else

#define UTPT_RECORD_RESET(_buf, _len)
#define UTPT_RECORD_TIME(_buf, _len)
#define UTPT_GET_TIME_CNT(_buf, _len) 0
#define UTPT_GET_TIME_SIZE(_buf, _len) 0
#define UTPT_GET_TIME_PTR(_buf, _len) ZNULL

#endif

/* utpt transport process read udp data,
   note: user can use the data for more process if return ZOK,
         otherwise the stack will delete it if return ZFAILED */
typedef ZINT (*PFN_UTPTRDUDP)(ZUINT iTptId, ZCOOKIE zCookie, ZVOID *pUserInfo, 
                ZUCHAR *pucData, ZUINT iLen, ST_ZOS_INET_ADDR *pstRmtAddr);

/* utpt transport process read tcp data,
   note: user can use the data for more process if return ZOK,
         otherwise the stack will delete it if return ZFAILED */
typedef ZINT (*PFN_UTPTRDTCP)(ZUINT iTptId, ZCOOKIE zCookie,
                ZDBUF zData);

/* utpt transport process check tcp data length,
   note: user is not allowed delete data */
typedef ZINT (*PFN_UTPTCHKTCP)(ZUINT iTptId, ZCOOKIE zCookie,
                ZDBUF zData, ZUINT *piLen, ZUINT *piMaxLen, 
                ZCOOKIE *pzCookie);

/* utpt transport process accept tcp */
typedef ZINT (*PFN_UTPTACPTTCP)(ZUINT iTptId, ZCOOKIE zCookie, 
                ZUINT iNewTptId, ZCOOKIE *pzNewCookie, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* utpt transport process connected tcp */
typedef ZINT (*PFN_UTPTCONNTCP)(ZUINT iTptId, ZCOOKIE zCookie);

/* utpt transport process disconnect tcp */
typedef ZINT (*PFN_UTPTDISCTCP)(ZUINT iTptId, ZCOOKIE zCookie);

/* utpt transport process data writable */
typedef ZINT (*PFN_UTPTWRDATA)(ZUINT iTptId, ZCOOKIE zCookie,
                ZBOOL *pbDone);

/* utpt transport process error */
typedef ZINT (*PFN_UTPTERR)(ZUINT iTptId, ZCOOKIE zCookie, ZUINT iErrType);

/* utpt transport process watch packet,
   note: user is not allowed delete data */
typedef ZINT (*PFN_UTPTWATCHPKT)(ZUINT iTptId, ZCOOKIE zWatcherCookie,
                ZUCHAR *pucData, ZUINT iLen, ST_ZOS_INET_ADDR *pstRmtAddr);

extern ZFUNC ZCONST ZCHAR * ZCONST kUtptEvntNetUnavailable;

/* utpt transport protocol type */
typedef enum EN_UTPT_TPT_TYPE
{
    EN_UTPT_TPT_UNKNOWN,             /* unknown protocol */
    EN_UTPT_TPT_UDP,                 /* udp protocol */    
    EN_UTPT_TPT_TCP_SERV,            /* tcp server protocol */
    EN_UTPT_TPT_TCP_CLI,             /* tcp client protocol */
    EN_UTPT_TPT_TLS_SERV,            /* tls(over tcp) server protocol */
    EN_UTPT_TPT_TLS_CLI,             /* tls(over tcp) client protocol */
    EN_UTPT_TPT_TYPE_MAX             /* type max value */
} EN_UTPT_TPT_TYPE;

/* utpt transport error type */
typedef enum EN_UTPT_ERR_TYPE
{
    EN_UTPT_ERR_READ,                /* error ocurr while reading */
    EN_UTPT_ERR_UNKNOWN              /* unknown error */
} EN_UTPT_ERR_TYPE;

/* utpt transport config */
typedef struct tagUTPT_TPT_CFG
{
    ZBOOL bNonBlking;                /* non-blocking transport */
    ZBOOL bShowRunLog;               /* show run log */
    ZINT iSockType;                  /* sock type EN_ZOS_SOCK_TYPE(os, stg, dtls) */
    ZUINT iTptType;                  /* transport type EN_UTPT_TPT_TYPE */
    ZUINT iSrvType;                  /* service type ZINET_SRV_FOREG... */
    ZBOOL bP2P;                      /* p2p call flag */
    ZCOOKIE zCookie;                 /* user id */
    ZVOID *pUserInfo;                /* user info */
    ZBOOL bUdpNeedConn;              /* udp connect option */
    ZBOOL bIsImsSingle;              /* is ims single login */
    
    /* tls config */
    ZUINT iTlsMethod;                /* tls method EN_TLS_METHOD_TYPE */
    ZUINT iTlsVeryType;              /* tls verify type EN_TLS_VERY_TYPE */
    ZCHAR *pcTlsTrustCaPath;         /* tls trusted ca path */
    ZCHAR *pcTlsTrustCaFile;         /* tls trusted ca file */
    ZCHAR *pcTlsCertFile;            /* tls certificate file */
    ZCHAR *pcTlsPrvKeyFile;          /* tls private key file */
    ZCHAR acSrvName[UTPT_TLS_SRV_NAME_LEN]; /* tls server name */
    ZCHAR acTlsPrvKeyPasswd[UTPT_TPT_PASSWD_LEN]; /* private key file password */

    ST_ZOS_INET_ADDR stLclAddr;      /* local address */
    ST_ZOS_INET_ADDR stRmtAddr;      /* remote address(tcp client used) */
    PFN_UTPTRDUDP pfnRdUdp;          /* read udp data process(udp serv/cli) */
    PFN_UTPTRDTCP pfnRdTcp;          /* read tcp data process(tcp client) */
    PFN_UTPTCHKTCP pfnChkTcp;        /* check tcp data process(tcp client) */
    PFN_UTPTACPTTCP pfnAcptTcp;      /* accept tcp process(tcp server) */
    PFN_UTPTCONNTCP pfnConnTcp;      /* connected tcp process(tcp client) */
    PFN_UTPTDISCTCP pfnDiscTcp;      /* disconnect tcp process(tcp client) */
    PFN_UTPTWRDATA pfnWrData;        /* data writable process */
    PFN_UTPTERR pfnErr;              /* transport error */
} ST_UTPT_TPT_CFG;

/* transport config usage
 *
 * tls transport is not supportted now
 *
 * if transport is no-blocking mode, 
 * it maybe return UTPT_RET_CONNING while in no-block mode
 *
 * udp server/client callbacks can be null, it means only can send data.
 * 
 * tcp server pfnRdTcp, pfnChkTcp, pfnAcptTcp, pfnDiscTcp callbacks 
 * must not be null.
 *
 * tcp client pfnRdTcp, pfnChkTcp, pfnDiscTcp callbacks must not be null.
 * if tcp client is no-block mode, the pfnConnTcp callback must not be null.
 *
 */

/* utpt open transport connection */
ZFUNC ZINT Utpt_Open(ST_UTPT_TPT_CFG *pstCfg, ZUINT *piTptId);

/* utpt open transport connection */
ZFUNC ZINT Utpt_OpenX(ST_UTPT_TPT_CFG *pstCfg, ZSOCKET zSock, 
                ZUINT *piTptId);

/* utpt close transport connection */
ZFUNC ZINT Utpt_Close(ZUINT iTptId);

/* utpt close send transport connection close event to UTPT thread */
ZFUNC ZINT Utpt_CloseX(ZUINT iTptId);

/* utpt send transport data buffer */
ZFUNC ZINT Utpt_Send(ZUINT iTptId, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZDBUF zData);

/* utpt send transport data buffer */
ZFUNC ZINT Utpt_SendIms(ZUINT iTptId, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZDBUF zData, ZCHAR *pcCallId, ZUCHAR ucReqPres);

/* utpt send transport data */
ZFUNC ZINT Utpt_SendX(ZUINT iTptId, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucData, ZUINT iLen);

/* utpt get transport user id */
ZFUNC ZINT Utpt_GetCookie(ZUINT iTptId, ZCOOKIE *pzcookie);

/* utpt get utptid fromm local port */
ZFUNC ZUINT Utpt_GetTptIdFromLPort(ZUSHORT wPort);

/* utpt get utptid fromm local address */
ZFUNC ZUINT Utpt_GetTptIdFromAddr(ZUINT iTptType, ST_ZOS_INET_ADDR *pstAddr);

/* utpt set transport user id */
ZFUNC ZINT Utpt_SetCookie(ZUINT iTptId, ZCOOKIE zCookie);

/* utpt get local transport address */
ZFUNC ZINT Utpt_GetTptAddr(ZUINT iTptId, ST_ZOS_INET_ADDR *pstAddr);

/* utpt set remote address */
ZFUNC ZINT Utpt_SetRmtAddr(ZUINT iTptId, ST_ZOS_INET_ADDR *pstAddr);

/* utpt get transport watcher */
ZFUNC ZINT Utpt_GetWatcher(ZUINT iTptId, ZCOOKIE *pzWaterCookie,
                PFN_UTPTWATCHPKT *ppfnWatchPkt);

/* utpt set transport watcher */
ZFUNC ZINT Utpt_SetWatcher(ZUINT iTptId, ZCOOKIE zWatcherCookie,
                PFN_UTPTWATCHPKT pfnWatchPkt);

/* utpt set socket tos option */
ZFUNC ZINT Utpt_SetSockTosOpt(ZUINT iTptId, ZUINT iTosVal);

/* utpt get remote certificate fingerprint 
The caller is responsible for freeing it by Zos_SysStrFree */
ZFUNC ZCHAR * Utpt_GetRmtFgrprnt(ZUINT iTptId, ZCHAR *pcAlgo);

/* utpt socket action on connect ok. 
     if bNtfyEvnt is ZTRUE, event should send to utpt task */
ZFUNC ZINT Utpt_SocketOnConnOk(ZSOCKET zSock);

/* utpt socket action on disconnected. 
     if bNtfyEvnt is ZTRUE, event should send to utpt task */
ZFUNC ZINT Utpt_SocketOnDisced(ZSOCKET zSock);

/* utpt socket action on user close */
ZFUNC ZINT Utpt_SocketOnUsrClose(ZSOCKET zSock);

/* utpt socket action on socket has accepted. 
     if bNtfyEvnt is ZTRUE, event should send to utpt task */
ZFUNC ZINT Utpt_SocketOnAccepted(ZSOCKET zSock,
                ZSOCKET zNewSock, ST_ZOS_INET_ADDR *pstRmtAddr);

/* utpt socket action on socket has accepted. if bNtfyEvnt is ZTRUE, 
   event should send to utpt task */
ZFUNC ZINT Utpt_SocketOnAcceptedX(ZSOCKET zSock,
                ZSOCKET zNewSock, ST_ZOS_INET_ADDR *pstRmtAddr);

/* utpt socket action on data is available to be read. 
     if bNtfyEvnt is ZTRUE, event should send to utpt task  */
ZFUNC ZINT Utpt_SocketOnReadable(ZSOCKET zSock);

/* utpt socket action on socket is writable. 
     if bNtfyEvnt is ZTRUE, event should send to utpt task  */
ZFUNC ZINT Utpt_SocketOnWritable(ZSOCKET zSock);

/* utpt socket action on have recvied data. 
     if bNtfyEvnt is ZTRUE, event should send to utpt task  */
ZFUNC ZINT Utpt_SocketOnRecvData(ZSOCKET zSock, ZUCHAR *pucData, 
                ZUINT iLen, ST_ZOS_INET_ADDR *pstRmtAddr);

/* utpt socket start tls */
ZFUNC ZINT Utpt_SocketStartTls(ZUINT iTptId);

/* utpt get statistics */
ZFUNC ZINT Utpt_GetSts(ZUINT *piBytesSent, ZUINT *piBytesRecv, ZBOOL bReset);

/* utpt get net unavailable string */
ZFUNC ZCONST ZCHAR * Utpt_GetEvntNetUnavailable(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _UTPT_UI_H__ */

