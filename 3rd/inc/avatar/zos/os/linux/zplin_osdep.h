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
  File name     : zplin_osdep.h
  Module        : linux os dependency
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPLIN_OSDEP_H__
#define _ZPLIN_OSDEP_H__

/** 
 * @file zplin_osdep.h
 * @brief Included and typedefs files required by Linux.
 * @note These functions would called by ZOS, user needn't call explictly.
 * But user also can use Zplin_Malloc, Zplin_Free for heap memory operation.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Attach linux porting environment.
 *
 * @param [in] iSize Environment memory size.
 * @param [out] ppMem A pointer to environment memory.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zplin_OsenvDetach
 */
ZFUNC ZINT Zplin_OsenvAttach(ZINT iSize, ZVOID **ppMem);

/**
 * @brief Detach linux porting environment.
 *
 * @see Zplin_OsenvAttach
 *
 */
ZFUNC ZVOID Zplin_OsenvDetach(ZFUNC_VOID);

/**
 * @brief Locate linux porting environment.
 *
 * @param [out] ppMem A pointer to environment memory.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zplin_OsenvAttach
 */
ZFUNC ZINT Zplin_OsenvLocate(ZVOID **ppMem);

/**
 * @brief Attach linux porting dependency functions.
 *
 * @param [out] pstMgr A pointer to os dependency manager.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 */
ZFUNC ZINT Zplin_OsdepAttach(ST_ZOS_OSDEP_MGR *pstMgr);

/**
 * @brief Allocate linux memory.
 *
 * @param [in] zSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zplin_Free
 */
ZFUNC ZVOID * Zplin_Malloc(ZSIZE_T zSize);

/* linux reallocate a memory */
ZFUNC ZVOID * Zplin_Realloc(ZVOID *pMem, ZSIZE_T zSize);

/**
 * @brief Free memory allocated.
 *
 * @param [in] pMem Memory address.
 *
 * @see Zplin_Malloc
 */
ZFUNC ZVOID Zplin_Free(ZVOID *pMem);

/**
 * @brief Trace source code with file and line no using Windows function.
 *
 * @param [in] pcDecs Trace description.
 * @param [in] pcFile The file name.
 * @param [in] iLineNo The file no.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 */
ZFUNC ZINT Zplin_Trace(ZCHAR *pcDecs, ZCHAR *pcFile, ZINT iLineNo);

/* linux config get socket on tcp connect ok function */
ZFUNC ZFUNCPTR Zplin_CfgGetSocketOnConnOk(ZFUNC_VOID);

/* linux config set socket on tcp connect ok function */
ZFUNC ZVOID Zplin_CfgSetSocketOnConnOk(ZFUNCPTR pfnAction);

/* linux config get socket on tcp disconnected function */
ZFUNC ZFUNCPTR Zplin_CfgGetSocketOnDisced(ZFUNC_VOID);

/* linux config set socket on tcp disconnected function */
ZFUNC ZVOID Zplin_CfgSetSocketOnDisced(ZFUNCPTR pfnAction);

/* linux config get socket on tcp accepted */
ZFUNC ZFUNCPTR Zplin_CfgGetSocketOnAccepted(ZFUNC_VOID);

/* linux config set socket on tcp accepted */
ZFUNC ZVOID Zplin_CfgSetSocketOnAccepted(ZFUNCPTR pfnAction);

/* linux config get socket on readable function */
ZFUNC ZFUNCPTR Zplin_CfgGetSocketOnReadable(ZFUNC_VOID);

/* linux config set socket on readable function */
ZFUNC ZVOID Zplin_CfgSetSocketOnReadable(ZFUNCPTR pfnAction);

/* linux config get socket on receive data function */
ZFUNC ZFUNCPTR Zplin_CfgGetSocketOnWritable(ZFUNC_VOID);

/* linux config set socket on receive data function */
ZFUNC ZVOID Zplin_CfgSetSocketOnWritable(ZFUNCPTR pfnAction);

/* linux config get socket on receive data function */
ZFUNC ZFUNCPTR Zplin_CfgGetSocketOnRecvData(ZFUNC_VOID);

/* linux config set socket on receive data function */
ZFUNC ZVOID Zplin_CfgSetSocketOnRecvData(ZFUNCPTR pfnAction);

#ifdef __cplusplus
}
#endif

#endif /* _ZPLIN_OSDEP_H__ */

