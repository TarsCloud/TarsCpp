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
  File name     : zos_pool.h
  Module        : zos memory pool
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-01-21    
  Description   :
      Data structure and function definitions required by the 
      memory pool management interface
    
  Modify History:   
  1. Date:        Author:         Modification:
  2005/01/21      leo.lv          created

*************************************************/
#ifndef _ZOS_POOL_H__
#define _ZOS_POOL_H__

/** 
 * @file zos_pool.h
 * @brief Zos memory pool functions.
 *
 * Zos memory pool provides basic memory management independent from
 * operating system. As the name suggest, it is a pool of memory block.
 * In this pool, there are various size of memory block which will 
 * be pre-allocate from operating system according to configuration. 
 *
 * Same size memory block arranged under the structure called bucket.
 * When request memory from this pool, it will first search for bucket
 * which meet the size request. If found, then return the data block address in
 * this bucket when there are free data block in it. If it is run out of 
 * data block, it will allocate another bunch of data block
 * from operating system and add them to bucket, then return the data block 
 * address of bucket in it.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Zos heap memory allocate function from specific os. */
typedef ZVOID * (*PFN_ZHEAPMALLOC)(ZSIZE_T zSize);

/** @brief Zos heap memory free function from specific os. */
typedef ZVOID (*PFN_ZHEAPFREE)(ZVOID *pMem);

/** @brief Zos bucket config info. */
typedef struct tagZOS_BKT_INFO
{
    ZUINT iBktSize;                  /* bucket size */
    ZUINT iMaxCount;                 /* bucket maixmum count */
    ZUINT iIncCount;                 /* bucket inc count per time */
} ST_ZOS_BKT_INFO;

/**
 * @brief Zos memory pool configuration.
 *
 *
 * @code  
   +---------------------+
   |   *pcName           |
   |   ucIsNeedMutex     | indicate the num of bkt info
   |   ucInfoGrpSize ------------------+  
   |   aucSpare[2]       |             |
   +---------------------+             V
   |   *pstInfoGrp -------------> +------------+     
   |                     |        | bkt info 1 | 
   +---------------------+        | bkt info 2 | 
   |   pfnHeapAlloc      |        | ...        | 
   |   pfnHeapFree       |        | bkt info n |
   +---------------------+        +------------+
 * @endcode
 */   
typedef struct tagZOS_POOL_CFG
{
    ZCHAR *pcName;                   /**< @brief Pool name. */
    ZCONST ST_ZOS_BKT_INFO *pstInfoGrp; /**< @brief Bucket info group. */
    PFN_ZHEAPMALLOC pfnHeapAlloc;    /**< @brief Heap memory allocate function.
                                          Default is Zos_HeapAlloc. */
    PFN_ZHEAPFREE pfnHeapFree;       /**< @brief Heap memory free function.
                                          Default is Zos_HeapFree. */
    ZUCHAR ucIsNeedMutex;            /**< @brief Mutex flag. If ZTRUE, 
                                        use mutex lock to ensure thread safe */
    ZUCHAR ucIsNeedDynaFree;         /**< @brief Dynamic free flag.
                                        if ZFALSE, memory pool will not free
                                        memory to operating system. */
    ZUCHAR ucIsNeedLog;              /**< @brief Log Flag. */
    ZUCHAR ucInfoGrpSize;            /**< @brief Bucket info group size. */
    ZOS_PADX64
} ST_ZOS_POOL_CFG;

/**
 * @brief Create a new memory pool with specific configuration.
 *
 * @param [in] pstPoolCfg Memory pool configuration.
 *
 * @return New pool ID if successfully, or return ZNULL.
 *
 * @see ST_ZOS_POOL_CFG
 */
ZFUNC ZPOOL Zos_PoolCreate(ST_ZOS_POOL_CFG *pstPoolCfg);

/**
 * @brief Delete memory pool. All data allocate from it will also be freed.
 *
 * @param [in] zPoolId Memory pool ID.
 *
 * @return [none].
 *
 * @see 
 */
ZFUNC ZVOID Zos_PoolDelete(ZPOOL zPoolId);

/**
 * @brief Allocate memory from memory pool.
 *
 * @param [in] zPoolId Memory pool ID.
 * @param [in] iSize Request size.
 *
 * @return New memory allocated if successfully, or return ZNULL.
 *
 * @see Zos_PoolAllocClrd 
 */
ZFUNC ZVOID * Zos_PoolAlloc(ZPOOL zPoolId, ZUINT iSize);

/**
 * @brief Allocate memory cleared by 0 from memory pool.
 *
 * @param [in] zPoolId Memory pool ID.
 * @param [in] iSize Request size.
 *
 * @return New memory allocated if successfully, or return ZNULL.
 *
 * @see Zos_PoolAlloc 
 */
ZFUNC ZVOID * Zos_PoolAllocClrd(ZPOOL zPoolId, ZUINT iSize);

/**
 * @brief Free memory allocated from memory pool.
 *
 * @param [in] zPoolId Memory pool ID.
 * @param [in] pMem Memory address.
 *
 * @return [none].
 *
 * @see  
 */
ZFUNC ZVOID Zos_PoolFree(ZPOOL zPoolId, ZVOID *pMem);

/**
 * @brief Get the size of memory block which contains specific address.
 *
 * @param [in] zPoolId Memory pool ID.
 * @param [in] pMem Memory address.
 * @param [out] piSize Memory block size.
 *
 * @retval ZOK Get size successfully.
 * @retval ZFAILED Get size failed.
 *
 * @see  
 */
ZFUNC ZINT Zos_PoolGetSize(ZPOOL zPoolId, ZVOID *pMem, ZUINT *piSize);

/**
 * @brief Check if the specific memory address is in any memory block 
 *        allocated from this pool.
 *
 * @param [in] zPoolId Memory pool ID.
 * @param [in] pMem Memory address.
 *
 * @retval ZTURE Memory address is in one memory block allocated from this pool.
 * @retval ZFAILED Otherwise.
 *
 * @see  
 */
ZFUNC ZBOOL Zos_PoolIsValid(ZPOOL zPoolId, ZVOID *pMem);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_POOL_H__ */

