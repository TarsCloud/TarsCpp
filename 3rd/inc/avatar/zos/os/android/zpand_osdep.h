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
  File name     : zpand_osdep.h
  Module        : android os dependence functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPAND_OSDEP_H__
#define _ZPAND_OSDEP_H__

/** 
 * @file zpand_osdep.h
 * @brief Included and typedefs files required by Android.
 * @note These functions would called by ZOS, user needn't call explictly.
 * But user also can use Zpand_Malloc, Zpand_Free for heap memory operation.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* android module post drive notification */
typedef ZINT (*PFN_ZPANDMODPOSTDRV)(ZFUNC_VOID);

/* android timer init, destroy, stop */
typedef ZINT (*PFN_ZPANDTIMER)(ZFUNC_VOID);

/** @brief ZOS os timer active function for callback */
typedef ZVOID (*PFN_ZPANDTIMERACTIVE)(ZBOOL bPostEvnt);

/* android start a timer, time length use millisecond unit */
typedef ZINT (*PFN_ZPANDTIMERX)(ZUINT iTimeLen, 
                PFN_ZPANDTIMERACTIVE pfnActive);

/* android module set post drive notification */
ZFUNC ZVOID Zpand_ModSetPostDrv(PFN_ZPANDMODPOSTDRV pfnAction);

/* android set timer init */
ZFUNC ZVOID Zpand_TimerSetInit(PFN_ZPANDTIMER pfnAction);

/* android set timer destroy */
ZFUNC ZVOID Zpand_TimerSetDestroy(PFN_ZPANDTIMER pfnAction);

/* android set start a timer, time length use millisecond unit */
ZFUNC ZVOID Zpand_TimerSetStart(PFN_ZPANDTIMERX pfnAction);

/* android set stop a timer */
ZFUNC ZVOID Zpand_TimerSetStop(PFN_ZPANDTIMER pfnAction);

#ifdef ZOS_SUPT_JAVA

/**
 * @brief Set JavaVM to MME.
 *
 * @param Pointer to JavaVM.
 */
ZFUNC ZVOID Zpand_JniOnLoad(ZVOID *pJavaVm);

/**
 * @brief Get Java VM.
 *
 * @return Pointer to JavaVM when succeed, otherwise ZNULL.
 */
ZFUNC ZVOID * Zpand_GetJvm(ZFUNC_VOID);

/**
 * @brief Get JNI enviroment.
 *
 * @param ppEnv Pointer to JNIEnv pointer.
 *
 * @return ZTRUE indicate it attached to current thread.
 */
ZFUNC ZBOOL Zpand_GetJniEnv(ZVOID **ppEnv);

/**
 * @brief Put JNI enviroment.
 *
 * @param ppEnv Pointer to JNIEnv pointer.
 * @param bAttached ZTRUE indicate it attached to current thread.
 *
 */
ZFUNC ZVOID Zpand_PutJniEnv(ZVOID **ppEnv, ZBOOL bAttached);

/** @brief android config set socket on tcp connect ok function */
ZFUNC ZVOID Zpand_CfgSetSocketOnConnOk(ZFUNCPTR pfnAction);

/** @brief android config set socket on tcp disconnected function */
ZFUNC ZVOID Zpand_CfgSetSocketOnDisced(ZFUNCPTR pfnAction);

/** @brief android config set socket on tcp accept */
ZFUNC ZVOID Zpand_CfgSetSocketOnAccepted(ZFUNCPTR pfnAction);

/** @brief android config set socket on readable function */
ZFUNC ZVOID Zpand_CfgSetSocketOnReadable(ZFUNCPTR pfnAction);

/** @brief android config set socket on receive data function */
ZFUNC ZVOID Zpand_CfgSetSocketOnWritable(ZFUNCPTR pfnAction);

/** @brief android config set socket on receive data function */
ZFUNC ZVOID Zpand_CfgSetSocketOnRecvData(ZFUNCPTR pfnAction);

#endif

#ifdef __cplusplus
}
#endif

#endif /* _ZPAND_OSDEP_H__ */

