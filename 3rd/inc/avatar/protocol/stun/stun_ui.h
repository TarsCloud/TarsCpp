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
  File name     : stun_ui.h
  Module        : stun
  Author        : anthouny.zhou
  Version       : 0.1
  Created on    : 2012-09-03
  Description:
    Marcos and structure definitions required by stun user interface

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _STUN_UI_H__
#define _STUN_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* stun query result callback */
typedef ZVOID (*PFN_STUNCALLBACK)(ST_ZOS_INET_ADDR *pstLocalAddr,
                ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstMapAddr, ZINT iStatType);

/* stun query allocate callback */
typedef ZVOID (*PFN_STUNALLOCCB)(ST_ZOS_INET_ADDR *pstLocalAddr,
                ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstMapAddr,
                ST_ZOS_INET_ADDR *pstRlyAddr, ZINT iStatType);

/* stun query channelbind callback */
typedef ZVOID (*PFN_STUNCBIND)(ST_ZOS_INET_ADDR *pstLocalAddr,
                ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstRmtAddr,
                ZUSHORT wChnlNum, ZINT iStatType);

/* stun query createpermission callback */
typedef ZVOID (*PFN_STUNCPMCB)(ST_ZOS_INET_ADDR *pstLocalAddr,
                ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstRmtAddr,
                ZINT iStatType);

/* stun query receive data callback */
typedef ZVOID (*PFN_STUNRECVDATACB)(ST_ZOS_INET_ADDR *pstLocalAddr,
                ZCOOKIE zCookie, ST_ZOS_INET_ADDR *pstRmtAddr,
                ZUCHAR *pucData, ZUSHORT wDataLen);

/* stun query mapped address */
ZFUNC ZINT Stun_QueryMapAddr(ST_ZOS_INET_ADDR *pstLocalAddr, ZCOOKIE zCookie,
                PFN_STUNCALLBACK pfnCallback);

/* stun query mapped address */
ZFUNC ZINT Stun_QueryMapAddrX(ST_ZOS_INET_ADDR *pstLocalAddr, 
                ZCHAR *pcSrvAddr, ZUSHORT wSrvPort, ZCOOKIE zCookie, 
                PFN_STUNCALLBACK pfnCallback);

/* stun query create */
ZFUNC ZINT Stun_QueryCreate(ST_ZOS_INET_ADDR *pstLocalAddr, ZCOOKIE zCookie,
                ZUINT *piQryId);

/* stun query delete */
ZFUNC ZINT Stun_QueryDelete(ZUINT iQryId);

/* stun query mapped address and relay address */
ZFUNC ZINT Stun_QueryMapRelayAddr(ZUINT iQryId, 
                PFN_STUNALLOCCB pfnAllocCb);

/* stun query create permission */
ZFUNC ZINT Stun_QueryCpm(ZUINT iQryId, ST_ZOS_INET_ADDR *pstRmtAddr, 
                PFN_STUNCPMCB pfnCmpCb);

/* stun query data send indication */
ZFUNC ZINT Stun_QueryIndSend(ZUINT iQryId, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucData, ZUSHORT wDataLen);

/* stun query channel bind */
ZFUNC ZINT Stun_QueryCBind(ZUINT iQryId, ST_ZOS_INET_ADDR *pstRmtAddr, 
                PFN_STUNCBIND pfnCBindCb);

/* stun query channel send data */
ZFUNC ZINT Stun_QueryIndData(ZUINT iQryId, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucData, ZUSHORT wDataLen);

/* stun query nat type */
ZFUNC ZINT Stun_QueryNatType(ZUINT iPort, ST_ZOS_INET_ADDR *pstBindAddr);

#ifdef __cplusplus
}
#endif

#endif /* _STUN_UI_H__ */

