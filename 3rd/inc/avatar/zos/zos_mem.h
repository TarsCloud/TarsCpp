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
  File name     : zos_mem.h
  Module        : zos memory library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the  
      memory management interface
      
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_MEM_H__
#define _ZOS_MEM_H__

/** 
 * @file zos_mem.h
 * @brief Zos memory functions.
 *
 * In this file, provide memory interfaces of Zos system memory pool.
 * This memory pool was created when Zos system is initialized, see Zos_SysInit.
 * and it will be deleted when Zos system is destroy, see Zos_SysDestroy.
 *
 * User can also access its configuration using interfaces defined 
 * in zos_syscfg.h. Get setting use
 * -# Zos_CfgGetMemNeedDynaFree
 * -# Zos_CfgGetMemGrpSize
 * -# Zos_CfgGetMemBktInfo
 * 
 * Set setting use
 * -# Zos_CfgSetMemNeedDynaFree
 * -# Zos_CfgSetMemBkt
 *
 * These interfaces are thread safe.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ZOS_SUPT_DBG

/* zos allocate memory */
#define ZOS_MALLOC(_mem, _size) do { \
    _mem = Zos_Malloc(_size); \
    if (_mem) Zos_TraceMemAdd(_mem, "Malloc", __FILE__, __LINE__); \
} while (0)

/* zos allocate memory and clear by 0 */
#define ZOS_MALLOC_CLRD(_mem, _size) do { \
    _mem = Zos_MallocClrd(_size); \
    if (_mem) Zos_TraceMemAdd(_mem, "MallocClrd", __FILE__, __LINE__); \
} while (0)

/* zos free memory */
#define ZOS_FREE(_mem) do { \
    if (_mem) Zos_TraceMemRmv(_mem); \
    Zos_Free(_mem); \
} while (0)

/* zos system string free memory */
#define ZOS_SYSSTR_FREE(_mem) do { \
    if (!_mem) Zos_TraceLog("SysStrFree null mem", __FILE__, __LINE__); \
    Zos_SysStrFree(_mem); \
} while (0)

#else /* no debug */

/* zos allocate memory */
#define ZOS_MALLOC(_mem, _size) \
    _mem = Zos_Malloc(_size)

/* zos allocate memory and clear by 0 */
#define ZOS_MALLOC_CLRD(_mem, _size) \
    _mem = Zos_MallocClrd(_size)

/* zos free memory */
#define ZOS_FREE(_mem) \
    Zos_Free(_mem)

/* zos system string free memory */
#define ZOS_SYSSTR_FREE(_mem) \
    Zos_SysStrFree(_mem)

#endif /* ZOS_SUPT_DBG */

/**
 * @brief Allocate memory.
 *
 * @param [in] zSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zos_MallocClrd
 */
ZFUNC ZVOID * Zos_Malloc(ZSIZE_T zSize);

/**
 * @brief Allocate memory clear by 0.
 *
 * @param [in] zSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zos_Malloc
 */
ZFUNC ZVOID * Zos_MallocClrd(ZSIZE_T zSize);

/**
 * @brief Free memory allocated.
 *
 * @param [in] pMem Memory address.
 *
 * @retval [none].
 *
 * @see Zos_Malloc
 */
ZFUNC ZVOID Zos_Free(ZVOID *pMem);

/**
 * @brief Allocates memory for an array of zNum elements of zSize bytes 
 *    each and returns a pointer to the allocated memory.
 *
 * @param [in] zNum Memory address.
 * @param [in] zSize New data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zos_Malloc
 */
ZFUNC ZVOID * Zos_Calloc(ZSIZE_T zNum, ZSIZE_T zSize);

/**
 * @brief Adjust the size of memory allocated.
 *
 * @param [in] pMem Memory address.
 * @param [in] zSize New data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zos_Malloc
 */
ZFUNC ZVOID * Zos_Realloc(ZVOID *pMem, ZSIZE_T zSize);

/**
 * @brief Duplicate a string to new memory block.
 *
 * @param [in] pcStr Source string to copy.
 *
 * @return New string address.
 *
 * @see Zos_SysStrAllocN
 */
ZFUNC ZVOID * Zos_SysStrAlloc(ZCONST ZCHAR *pcStr);

/**
 * @brief Allocate memory for specific string length.
 *
 * @param [in] iLen Request string length, 
 * allocate memory size will be iLen + 1, and add '\\0' in the end.
 *
 * @return New string address.
 *
 * @see Zos_SysStrAlloc
 */
ZFUNC ZVOID * Zos_SysStrAllocN(ZINT iLen);

/**
 * @brief Duplicate a string with specific length to new memory.
 *
 * @param [in] pcStr Source string to copy.
 * @param [in] iLen Copy length.
 *
 * @return New string address.
 *
 * @see Zos_SysStrAlloc
 */
ZFUNC ZVOID * Zos_SysStrNAlloc(ZCONST ZCHAR *pcStr, ZINT iLen);

/**
 * @brief Copy a ST_ZOS_SSTR type string to ZCHAR type string.
 *
 * @param [in] pstStr Source string to copy.
 *
 * @return New string address.
 *
 * @see Zos_SysStrAlloc
 */
ZFUNC ZVOID * Zos_SysStrXAlloc(ST_ZOS_SSTR *pstStr);

/**
 * @brief Format a new string.
 *
 * @param [in] pcFormat New string format.
 *
 * @return New string address.
 *
 * @see Zos_SysStrAlloc
 */
ZFUNC ZVOID * Zos_SysStrFAlloc(const ZCHAR *pcFormat, ...) ZATTR_PRINTF(1,2);

/**
 * @brief Duplicate a string data to new memory.
 *
 * @param [in] zData Source string data.
 *
 * @return New string address.
 *
 * @see Zos_SysStrAlloc
 */
ZFUNC ZVOID * Zos_SysStrDAlloc(ZDBUF zData);

/**
 * @brief Free the string allocated.
 *
 * @param [in] pSysStr String address.
 *
 * @return [none].
 *
 * @see Zos_SysStrAlloc
 */
ZFUNC ZVOID Zos_SysStrFree(ZVOID *pSysStr);

/**
 * @brief Get the string length.
 *
 * @param [in] pcSysStr String address.
 *
 * @return String length.
 *
 * @see 
 */
ZFUNC ZINT Zos_SysStrLen(ZCHAR *pcSysStr);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_MEM_H__ */

