/************************************************************************

        Copyright (c) 2005-2009 by Juphoon System Software, Inc.
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
  File name     : zos_bpool.h
  Module        : zos bitmap memory pool library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2009-03-21    
  Description   :
      Data structure and function definitions required by the 
      bitmap pool interface
    
  Modify History:   
  1. Date:        Author:         Modification:
  2009/03/21      leo.lv          created

*************************************************/
#ifndef _ZOS_BPOOL_H__
#define _ZOS_BPOOL_H__

/** 
 * @file zos_bpool.h
 * @brief Data structure and function definitions required by the bitmap pool 
 *        interface.
 * 
 *In this pool have 4 bitmap memory type.Small size is less than 
 *65535 * iBitByteSize, and larg size is more than or equal to 
 *65535 * iBitByteSize.
 *
 *The Attribute of pool is using bitbyte to mark the memory free or full.One bit 
 *Correspond a block memory.0 means the memory block is free.1 means the memory
 *block is full.
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief zos bitmap memory type, size is maximum size of alloc memory
   small size is less than 65535 * iBitByteSize
   large size is more than or equal to 65535 * iBitByteSize */
typedef enum EN_ZOS_BPOOL_TYPE
{
    EN_ZOS_BPOOL_SMALL_SIZE,         /**< @brief small size */
    EN_ZOS_BPOOL_SMALL_SIZE_QFREE,   /**< @brief small size with quick free memory */
    EN_ZOS_BPOOL_LARGE_SIZE,         /**< @brief large size */
    EN_ZOS_BPOOL_LARGE_SIZE_QFREE,   /**< @brief large size with quick free memory */
    EN_ZOS_BPOOL_UNKNOWN             /**< @brief unknown type */
} EN_ZOS_BPOOL_TYPE;

/** @brief zos bitmap memory pool id */
typedef ZVOID * ZBPOOL;

/**
 * @brief Create a bitmap pool by node type.
 *
 * @param [in] ucType Pool node type.
 * @param [in] iBktSize Bucket size.
 * @param [in] iBitByte 
 *
 * @return A pointer to the newly created memory pool, or ZNULL on failure.
 *
 * @see 
 */
ZFUNC ZBPOOL Zos_BpoolCreate(ZUCHAR ucType, ZUINT iBktSize, ZUINT iBitByte);

/**
 * @brief Delete a bitmap pool.
 *
 * @param [in] zPool The memory pool identifier.
 *
 * @return [none]
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_BbktDelete free all free buckets.
 *       -# Call Zos_BbktDelete free all full buckets.
 *       -# Call Zos_Free free the pool.
 *
 * @see Zos_BbktDelete, Zos_Free
 */
ZFUNC ZVOID Zos_BpoolDelete(ZBPOOL zPool);

/**
 * @brief Clean up a bitmap pool.
 *
 * @param [in] zPool The memory pool identifier.
 *
 * @return [none]
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_BbktDelete free all free buckets.
 *       -# Call Zos_BbktDelete free all full buckets.
 *       -# Call ZOS_DLIST_CREATEX reset the list.
 *
 * @see Zos_BbktDelete, ZOS_DLIST_CREATEX
 */
ZFUNC ZVOID Zos_BpoolClean(ZBPOOL zPool);

/**
 * @brief Allocate a memory.
 *
 * @param [in] zPool The memory pool identifier.
 * @param [in] iSize Size of the memory block.
 *
 * @return The address of the allocated memory block, or ZNULL on failure.
 *
 * @note This function is shortcut of:\n
 *       -# Check the size is valid .
 *       -# Check the input pointer valid.
 *       -# Call Zos_BpoolAllocX alloc the data.
 *
 * @see Zos_BpoolAllocX
 */
ZFUNC ZVOID * Zos_BpoolAlloc(ZBPOOL zPool, ZUINT iSize);

/**
 * @brief Allocate a memory block of specific size in a memory pool.
 *
 * @param [in] zPool The memory pool identifier.
 * @param [in] iBktSize Size of the bucket. 
 * @param [in] iMemSize Size of the memory block.
 *
 * @return The address of the allocated memory block, or ZNULL on failure.
 *
 * @see Zos_BbktAlloc
 */
ZFUNC ZVOID * Zos_BpoolAllocX(ZBPOOL zPool, ZUINT iBktSize, 
                ZUINT iMemSize);

/**
 * @brief Free a memory block in a memory pool.
 *
 * @param [in] zPool The memory pool identifier.
 * @param [in] pMem The address of the memory block which is to be freed.  
 *
 * @return [none]
 *
 * @see 
 */
ZFUNC ZINT Zos_BpoolFree(ZBPOOL zPool, ZVOID *pMem);

/**
 * @brief Get the size of bucket size.
 *
 * @param [in] zPool The memory pool identifier.
 * @param [out] piSize Size of the memory block.  
 *
 * @return ZOK on success, or return ZFAILED on failure.
 *
 * @see 
 */
ZFUNC ZINT Zos_BpoolGetSize(ZBPOOL zPool, ZUINT *piSize);

/**
 * @brief Check the data location in the memory pool.
 *
 * @param [in] zPool The memory pool identifier.
 * @param [in] pData The memory block address. 
 *
 * @return ZTRUE It is, or return ZFALSE It is.
 *
 * @see 
 */
ZFUNC ZBOOL Zos_BpoolHoldD(ZBPOOL zPool, ZVOID *pData);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_BPOOL_H__ */

