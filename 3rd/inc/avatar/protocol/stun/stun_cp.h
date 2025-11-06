/************************************************************************

        Copyright (c) 2005-2012 by Juphoon System Software, Inc.
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
  File name     : stun_cp.h
  Module        : stun
  Author        : anthouny.zhou
  Version       : 0.1
  Created on    : 2012-09-03
  Description:
    Marcos and structure definitions required by stun core processor

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _STUN_CP_H__
#define _STUN_CP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* stun callback of create message */
typedef ZINT (*PFN_STUNCPMSGINIT)(ZUINT iCpId, ZCOOKIE zCookie, 
                ZUSHORT wMsgType, ST_STUN_MSG *pstMsg);

/* stun callback of send data */
typedef ZINT (*PFN_STUNCPTPTSEND)(ZUINT iCpId, ZCOOKIE zCookie,
                ZDBUF zMsgBuf);

/* stun callback of receive data */
typedef ZINT (*PFN_STUNCPTPTRECV)(ZUINT iCpId, ZCOOKIE zCookie,
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucData, 
                ZUINT iDataLen);

/* stun callback for start timer */
typedef ZINT (*PFN_STUNCPTMRSTART)(ZUINT iCpId, ZUINT iTimeLen);

/* stun callback for request fail */
typedef ZINT (*PFN_STUNCPREQFAIL)(ZUINT iCpId, ZCOOKIE zCookie,
                ZINT iErrCode, ST_STUN_MSG *pstMsg);

/* stun callback for receving binding request */
typedef ZINT (*PFN_STUNCPRECVBIND)(ZUINT iCpId, ZCOOKIE zCookie,
                ST_ZOS_INET_ADDR *pstPeerAddr, ST_STUN_MSG *pstMsg);

/* stun callback for request binding ok */
typedef ZINT (*PFN_STUNCPREQBINDOK)(ZUINT iCpId, ZCOOKIE zCookie,
                ST_ZOS_INET_ADDR *pstRmtAddr, ST_STUN_MSG *pstMsg,
                ST_ZOS_INET_ADDR *pstMpdAddr);

/* stun callback for request allocate ok */
typedef ZINT (*PFN_STUNCPREQALLOCOK)(ZUINT iCpId, ZCOOKIE zCookie,
                ST_ZOS_INET_ADDR *pstMpdAddr, ST_ZOS_INET_ADDR *pstRlyAddr,
                ST_STUN_MSG *pstMsg);

/* stun callback for request channelbind ok */
typedef ZINT (* PFN_STUNCPREQCBINDOK)(ZUINT iCpId, ZCOOKIE zCookie,
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUSHORT wChnlNum);

/* stun callback for request create permission ok */
typedef ZINT (*PFN_STUNCPREQCPMOK)(ZUINT iCpId, ZCOOKIE zCookie,
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* stun core processor callback functions */
typedef struct tagSTUN_CP_CBS
{
    /* common callbacks */
    PFN_STUNCPMSGINIT pfnMsgInit;
    PFN_STUNCPTPTSEND pfnTptSend;
    PFN_STUNCPTPTRECV pfnTptRecv;
    PFN_STUNCPTMRSTART pfnTmrStart;

    /* stun callbacks */
    PFN_STUNCPRECVBIND pfnRecvBind;
    PFN_STUNCPREQBINDOK pfnReqBindOk;
    PFN_STUNCPREQFAIL pfnReqBindFail;

    /* turn callbacks */
    PFN_STUNCPREQALLOCOK pfnReqAllocOk;
    PFN_STUNCPREQFAIL pfnReqAllocFail;
    PFN_STUNCPREQFAIL pfnRefreshFail;
    PFN_STUNCPREQCPMOK pfnReqCpmOk;
    PFN_STUNCPREQFAIL pfnReqCpmFail;
    PFN_STUNCPREQCBINDOK pfnReqCBindOk;
    PFN_STUNCPREQFAIL pfnReqCBindFail;
} ST_STUN_CP_CBS;

/* stun core processor config */
typedef struct tagSTUN_CP_CFG
{
    ZUCHAR ucFingerPrint;            /* use FINGERPRINT to distinguish data */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    ZCOOKIE zCookie;                 /* user info */
    ST_STUN_CP_CBS stCbs;            /* callback functions */
} ST_STUN_CP_CFG;

/* stun create core processor */
ZFUNC ZINT Stun_CpCreate(ST_STUN_CP_CFG *pstCfg, ZUINT *piCpId);

/* stun delete core processor */
ZFUNC ZINT Stun_CpDelete(ZUINT iCpId);

/* stun get user id of core processor */
ZFUNC ZCOOKIE Stun_CpGetCookie(ZUINT iCpId);

/* stun core processor time out */
ZFUNC ZINT Stun_CpProcTm(ZUINT iCpId);

/* stun core processor receive data */
ZFUNC ZINT Stun_CpProcStunMsg(ZUINT iCpId, ZCOOKIE zCookie,
                ST_ZOS_INET_ADDR *pstPeerAddr, ZUCHAR *pucData, ZUINT iDataLen);

/* stun core processor channel data */
ZFUNC ZINT Stun_CpProcChnlData(ZUINT iCpId, ZCOOKIE zCookie,
                ZUCHAR *pucData, ZUINT iDataLen);

/* stun core processor Binding request */
ZFUNC ZINT Stun_CpReqBind(ZUINT iCpId, ZCOOKIE zCookie);

/* stun core processor Binding request no retransmission */
ZFUNC ZINT Stun_CpReqBindX(ZUINT iCpId, ZCOOKIE zCookie);

/* stun core processor stop Binding request */
ZFUNC ZINT Stun_CpReqBindStop(ZUINT iCpId, ZCOOKIE zCookie);

/* stun core processor allocate request */
ZFUNC ZINT Stun_CpReqAlloc(ZUINT iCpId, ZCOOKIE zCookie, 
                 ZUINT iLifeTime, ST_ZOS_SSTR *pstFingerPrint);

/* stun core processor release allocate */
ZFUNC ZINT Stun_CpReqAllocRls(ZUINT iCpId, ZCOOKIE zCookie);

/* stun core processor create permission */
ZFUNC ZINT Stun_CpReqPermCreate(ZUINT iCpId, ZCOOKIE zCookie, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZBOOL bAutoRefresh);

/* stun core processor release permission */
ZFUNC ZINT Stun_CpReqPermRls(ZUINT iCpId, ZCOOKIE zCookie, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* stun core processor channelbind request */
ZFUNC ZINT Stun_CpReqChnlBind(ZUINT iCpId, ZCOOKIE zCookie, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* stun core processor release channel */
ZFUNC ZINT Stun_CpReqChnlRls(ZUINT iCpId, ZCOOKIE zCookie, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* stun core processor channel send data */
ZFUNC ZINT Stun_CpIndData(ZUINT iCpId, ZCOOKIE zCookie, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucData, 
                ZUSHORT wDataLen);

/* stun core processor send indication */
ZFUNC ZINT Stun_CpIndSend(ZUINT iCpId, ZCOOKIE zCookie, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucData, 
                ZUSHORT wDataLen);

/* stun core processor process binding request */
ZFUNC ZINT Stun_CpGenBindRsp(ZUCHAR *pucData, ZUINT iDataLen,
                ST_ZOS_INET_ADDR *pstPeerAddr, ZDBUF *pzRspMsg);

/* stun core processor process binding request */
ZFUNC ZINT Stun_CpGenBindRspX(ZUCHAR *pucData, ZUINT iDataLen,
                ST_ZOS_INET_ADDR *pstPeerAddr, ST_STUN_MSG **ppstReqMsg,
                ST_STUN_MSG **ppstRspMsg);

#ifdef __cplusplus
}
#endif

#endif /* _STUN_CP_H__ */

