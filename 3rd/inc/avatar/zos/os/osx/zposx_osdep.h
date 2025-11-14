/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : zposx_osdep.h
  Module        : mac os os dependency
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPOSX_OSDEP_H__
#define _ZPOSX_OSDEP_H__

/** 
 * @file zposx_osdep.h
 * @brief Included and typedefs files required by Mac OS.
 * @note These functions would called by ZOS, user needn't call explictly.
 * But user also can use Zposx_Malloc, Zposx_Free for heap memory operation.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief osx process message which delivered by  Zpsym_OsenvDeliverMsg */
typedef ZVOID (*PFN_ZPOSXPROCMSG)(ZVOID *pData);

/** @brief osx socket action on connect ok */
typedef ZINT (*PFN_ZPOSXSOCKETONCONNOK)(ZSOCKET zSock);

/** @brief osx socket action on disconnected */
typedef ZINT (*PFN_ZPOSXSOCKETONDISCED)(ZSOCKET zSock);

/** @brief osx socket action on accept */
typedef ZINT (*PFN_ZPOSXSOCKETONACCEPTED)(ZSOCKET zSock, 
                ZSOCKET zNewSock, ST_ZOS_INET_ADDR *pstRmtAddr);

/** @brief osx socket action on readable */
typedef ZINT (*PFN_ZPOSXSOCKETONREADABLE)(ZSOCKET zSock);

/** @brief osx socket action on receive data */
typedef ZINT (*PFN_ZPOSXSOCKETONWRITABLE)(ZSOCKET zSock);

/** @brief osx socket action on receive data */
typedef ZINT (*PFN_ZPOSXSOCKETONRECVDATA)(ZSOCKET zSock, ZUCHAR *pucData,
                ZUINT iLen, ST_ZOS_INET_ADDR *pstRmtAddr);

/** @brief osx socket config */
typedef struct tagZPOSX_SOCK_CFG
{
    PFN_ZPOSXSOCKETONCONNOK pfnOnConnOk;
    PFN_ZPOSXSOCKETONDISCED pfnOnDisced;
    PFN_ZPOSXSOCKETONACCEPTED pfnOnAccepted;
    PFN_ZPOSXSOCKETONREADABLE pfnOnReadable;
    PFN_ZPOSXSOCKETONWRITABLE pfnOnWritable;
    PFN_ZPOSXSOCKETONRECVDATA pfnOnRecvData;
} ST_ZPOSX_SOCK_CFG;

/** @brief osx config set socket on tcp connect ok function */
ZFUNC ZVOID Zposx_CfgSetSocketOnConnOk(ZFUNCPTR pfnAction);

/** @brief osx config set socket on tcp disconnected function */
ZFUNC ZVOID Zposx_CfgSetSocketOnDisced(ZFUNCPTR pfnAction);

/** @brief osx config set socket on tcp accept */
ZFUNC ZVOID Zposx_CfgSetSocketOnAccepted(ZFUNCPTR pfnAction);

/** @brief osx config set socket on readable function */
ZFUNC ZVOID Zposx_CfgSetSocketOnReadable(ZFUNCPTR pfnAction);

/** @brief osx config set socket on receive data function */
ZFUNC ZVOID Zposx_CfgSetSocketOnWritable(ZFUNCPTR pfnAction);

/** @brief osx config set socket on receive data function */
ZFUNC ZVOID Zposx_CfgSetSocketOnRecvData(ZFUNCPTR pfnAction);

#ifdef __cplusplus
}
#endif

#endif /* _ZPOSX_OSDEP_H__ */

