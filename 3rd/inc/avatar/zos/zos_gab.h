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
  File name     : zos_gab.h
  Module        : zos memory garbage bin library functions 
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-12-9
  Description   :
    Memory garbage bin for any type resources.

    You can put any memory resource into the garbage,
    but the type of the resource would be ignored,
    and you should enter each free resource function pointer.

    Note, the function pointer must be "ZVOID (*ZVOIDFUNCPTR)(ZVOID *)",
    you can encapsulate the ZVOIDFUNCPTR, if there does not exist.
    The memory resource could be char *, void *,  ST_ZOS_DBUF, 
    or any other resources.


  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_GAB_H__
#define _ZOS_GAB_H__

/** 
 * @file
 * @brief Zos memory garbage functions.
 *
 * Memory garbage provide mechanism to collect memory allocated, and free
 * them as a whole.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* zos stack dump identifier */
typedef ZVOID * ZGABID;

/* zos garbage add memory */
#define ZOS_GAB_ADDMEM(_zGabId, _pMem, _pfnClr) do { \
    ZINT iRet; \
    iRet = Zos_GabAddMem(_zGabId, _pMem, _pfnClr); \
    if (iRet == ZOK) Zos_TraceMemAdd(_pMem, "GAB_ADDMEM", __FILE__, __LINE__); \
} while (0)

/* zos garbage remove memory */
#define ZOS_GAB_RMVMEM(_zGabId, _pMem) do { \
    Zos_TraceMemRmv(_pMem); \
    Zos_GabRmvMem(_zGabId, _pMem); \
} while (0)

/**
 * @brief Create memory garbage.
 *
 * @param [in] bMutex Mutex lock flag.
 *
 * @return Garbage ID if successfully, or return ZNULL.
 *
 * @see @ref Zos_GabDelete
 */
ZFUNC ZGABID Zos_GabCreate(ZBOOL bMutex);

/**
 * @brief Delete memory garbage, all memory in it will also be deleted.
 *
 * @param [in] zGabId Garbage ID.
 *
 * @return [none].
 *
 * @see @ref Zos_GabCreate
 */
ZFUNC ZVOID Zos_GabDelete(ZGABID zGabId);

/**
 * @brief Add a memory block allocated to garbage.
 *
 * @param [in] zGabId Garbage ID.
 * @param [in] pMem Memory address.
 * @param [in] pfnClr Memory free function address.
 *
 * @retval ZOK Add memory successfully.
 * @retval ZFAILED Add memory failed.
 *
 * @see @ref Zos_GabRmvMem
 */
ZFUNC ZINT Zos_GabAddMem(ZGABID zGabId, ZVOID *pMem, ZVOIDFUNCPTR pfnClr);

/**
 * @brief Remove a memory block in garbage.
 *
 * @param [in] zGabId Garbage ID.
 * @param [in] pMem Memory address.
 *
 * @retval ZOK Add memory successfully.
 * @retval ZFAILED Add memory failed.
 *
 * @see @ref Zos_GabAddMem
 */
ZFUNC ZINT Zos_GabRmvMem(ZGABID zGabId, ZVOID *pMem);

/**
 * @brief Cleanup memory in garbage.
 *
 * @param [in] zGabId Garbage ID.
 * @param [in] bDeleteByTime delete memory by timestamp
 *
 * @retval ZOK Cleanup memory successfully.
 * @retval ZFAILED Cleanup memory failed.
 *
 * @see @ref Zos_GabAddMem
 */
ZFUNC ZINT Zos_GabCleanUp(ZGABID zGabId, ZBOOL bDeleteByTime);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_GAB_H__ */

