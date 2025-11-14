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
  File name     : zpwin_osdep.h
  Module        : windows os dependency
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPWIN_OSDEP_H__
#define _ZPWIN_OSDEP_H__

/** 
 * @file zpwin_osdep.h
 * @brief Included and typedefs files required by Windows.
 * @note These functions would called by ZOS, user needn't call explictly.
 * But user also can use Zpwin_Malloc, Zpwin_Free for heap memory operation.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* *
* when is VS2015, the __iob_func is converted to __acrt_iob_func.
* so add convertion for __acrt_iob_func to __iob_func
*/  
#if _MSC_VER>=1900  
    FILE* __cdecl __iob_func(unsigned i); 
#endif /* _MSC_VER>=1900 */  

/**
 * @brief Allocate windows memory.
 *
 * @param [in] zSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zpwin_Free
 */
ZFUNC ZVOID * Zpwin_Malloc(ZSIZE_T zSize);

/* windows reallocate a memory */
ZFUNC ZVOID * Zpwin_Realloc(ZVOID *pMem, ZSIZE_T zSize);

/**
 * @brief Free memory allocated.
 *
 * @param [in] pMem Memory address.
 *
 * @see Zpwin_Malloc
 */
ZFUNC ZVOID Zpwin_Free(ZVOID *pMem);

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
ZFUNC ZINT Zpwin_Trace(ZCHAR *pcDecs, ZCHAR *pcFile, ZINT iLineNo);

/* windows com init */
ZFUNC ZINT Zpwin_ComInit(ZFUNC_VOID);

/* windows com destroy */
ZFUNC ZINT Zpwin_ComDestroy(ZFUNC_VOID);

/** @brief windows config get socket on tcp connect ok function */
ZFUNC ZFUNCPTR Zpwin_CfgGetSocketOnConnOk(ZFUNC_VOID);

/** @brief windows config set socket on tcp connect ok function */
ZFUNC ZVOID Zpwin_CfgSetSocketOnConnOk(ZFUNCPTR pfnAction);

/** @brief windows config get socket on tcp disconnected function */
ZFUNC ZFUNCPTR Zpwin_CfgGetSocketOnDisced(ZFUNC_VOID);

/** @brief windows config set socket on tcp disconnected function */
ZFUNC ZVOID Zpwin_CfgSetSocketOnDisced(ZFUNCPTR pfnAction);

/** @brief windows config get socket on tcp accepted */
ZFUNC ZFUNCPTR Zpwin_CfgGetSocketOnAccepted(ZFUNC_VOID);

/** @brief windows config set socket on tcp accept */
ZFUNC ZVOID Zpwin_CfgSetSocketOnAccepted(ZFUNCPTR pfnAction);

/** @brief windows config get socket on readable function */
ZFUNC ZFUNCPTR Zpwin_CfgGetSocketOnReadable(ZFUNC_VOID);

/** @brief windows config set socket on readable function */
ZFUNC ZVOID Zpwin_CfgSetSocketOnReadable(ZFUNCPTR pfnAction);

/** @brief windows config get socket on receive data function */
ZFUNC ZFUNCPTR Zpwin_CfgGetSocketOnWritable(ZFUNC_VOID);

/** @brief windows config set socket on receive data function */
ZFUNC ZVOID Zpwin_CfgSetSocketOnWritable(ZFUNCPTR pfnAction);

/** @brief windows config get socket on receive data function */
ZFUNC ZFUNCPTR Zpwin_CfgGetSocketOnRecvData(ZFUNC_VOID);

/** @brief windows config set socket on receive data function */
ZFUNC ZVOID Zpwin_CfgSetSocketOnRecvData(ZFUNCPTR pfnAction);

#ifdef __cplusplus
}
#endif

#endif /* _ZPWIN_OSDEP_H__ */

