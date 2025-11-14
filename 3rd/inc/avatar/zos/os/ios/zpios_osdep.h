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
  File name     : zpios_osdep.h
  Module        : ios os dependency
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPIOS_OSDEP_H__
#define _ZPIOS_OSDEP_H__

/** 
 * @file zpios_osdep.h
 * @brief Included and typedefs files required by iPhone.
 * @note These functions would called by ZOS, user needn't call explictly.
 * But user also can use Zpios_Malloc, Zpios_Free for heap memory operation.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief ios socket config */
typedef struct tagZPIOS_SOCK_CFG
{
    PFN_SOCKETONCONNOK pfnOnConnOk;
    PFN_SOCKETONDISCED pfnOnDisced;
    PFN_SOCKETONACCEPTED pfnOnAccepted;
    PFN_SOCKETONREADABLE pfnOnReadable;
    PFN_SOCKETONWRITABLE pfnOnWritable;
    PFN_SOCKETONRECVDATA pfnOnRecvData;
} ST_ZPIOS_SOCK_CFG;

/** @brief ios config set socket on tcp connect ok function */
ZFUNC ZVOID Zpios_CfgSetSocketOnConnOk(ZFUNCPTR pfnAction);

/** @brief ios config set socket on tcp disconnected function */
ZFUNC ZVOID Zpios_CfgSetSocketOnDisced(ZFUNCPTR pfnAction);

/** @brief ios config set socket on tcp accept */
ZFUNC ZVOID Zpios_CfgSetSocketOnAccepted(ZFUNCPTR pfnAction);

/** @brief ios config set socket on readable function */
ZFUNC ZVOID Zpios_CfgSetSocketOnReadable(ZFUNCPTR pfnAction);

/** @brief ios config set socket on receive data function */
ZFUNC ZVOID Zpios_CfgSetSocketOnWritable(ZFUNCPTR pfnAction);

/** @brief ios config set socket on receive data function */
ZFUNC ZVOID Zpios_CfgSetSocketOnRecvData(ZFUNCPTR pfnAction);

#ifdef __cplusplus
}
#endif

#endif /* _ZPIOS_OSDEP_H__ */

