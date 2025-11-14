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
  File name     : zos_dhash.h
  Module        : zos dynamic hash library functions
  Author        : zocol  
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the dynamic hash
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_DHASH_H__
#define _ZOS_DHASH_H__

/** 
 * @file zos_dhash.h
 * @brief Zos dynamic hash interfaces.
 *  
 * In this file, it defined the operation function
 * of zos dynamic hash, including create, remove, insert etc.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* zos dynamic hash key and compare funciton */
typedef ZINT (*PFN_ZDHASHKEY)(ZUINT iType, ZSIZE_T zParm1,  \
                ZSIZE_T zParm2, ZUINT *piHashKey);
    
typedef ZINT (*PFN_ZDHASHCMP)(ZVOID * pEntry, ZUINT iType,  \
                ZSIZE_T zParm1, ZSIZE_T zParm2);

/* zos dynamic hash id */
typedef ZVOID * ZDHASHID;

/**
 * @brief Creates a ZOS hash table.
 *
 * @param [in] pfnKey A pointer to hash key production funtion.
 * @param [in] pfnCmp A pointer to hash entry compare function.
 * @param [out] pzHashId A pointer to the dynamic hash table 
 *                       which is to be created.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashDelete
 */
ZFUNC ZINT Zos_DhashCreate(PFN_ZDHASHKEY pfnKey, PFN_ZDHASHCMP pfnCmp, 
                ZDHASHID *pzHashId);

/**
 * @brief Creates a dynamic hash in an abuf.
 *
 * @param [in] zMemBuf Aggregation buffer.
 * @param [in] pfnKey A pointer to hash key production funtion.
 * @param [in] pfnCmp A pointer to hash entry compare function.
 * @param [in] pzHashId A pointer to the dynamic hash table 
 *                      which is to be created.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashDelete
 */
ZFUNC ZINT Zos_DhashCreateX(ZCBUF zMemBuf, PFN_ZDHASHKEY pfnKey, 
                PFN_ZDHASHCMP pfnCmp, ZDHASHID *pzHashId);

/**
 * @brief Creates a dynamic hash in an abuf.
 *
 * @param [in] zMemBuf Aggregation buffer.
 * @param [in] iBktSize The Default data block size.
 * @param [in] pfnKey A pointer to hash key production funtion.
 * @param [in] pfnCmp A pointer to hash entry compare function.
 * @param [in] pzHashId A pointer to the dynamic hash table 
 *                      which is to be created.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashDelete
 */
ZFUNC ZINT Zos_DhashCreateXD(ZCBUF zMemBuf, ZUINT iBktSize, 
                PFN_ZDHASHKEY pfnKey, PFN_ZDHASHCMP pfnCmp, ZDHASHID *pzHashId);

/**
 * @brief This function deletes a dynamic hash table.
 *
 * @param [in] zHashId A pointer to the dynamic hash id which is to be deleted.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashInsert
 */
ZFUNC ZINT Zos_DhashDelete(ZDHASHID zHashId);

/**
 * @brief Inserts a hash entry into a dynamic hash table.
 *
 * @param [in] zHashId A pointer to a hash table.
 * @param [in] pEntry The hash entry.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashRemove
 */
ZFUNC ZINT Zos_DhashInsert(ZDHASHID zHashId, ZVOID * pEntry, 
                ZUINT iType, ZSIZE_T zParm1, ZSIZE_T zParm2);

/**
 * @brief Remove a hash entry in hash table.
 *
 * @param [in] zHashId A pointer to a hash table.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashRemoveX
 */
ZFUNC ZINT Zos_DhashRemove(ZDHASHID zHashId, ZUINT iType, 
                ZSIZE_T zParm1, ZSIZE_T zParm2);

/**
 * @brief Remove an entry by the same hash key in hash table.
 *
 * @param [in] zHashId A pointer to a hash table.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 * @param [in] dwEntry The hash entry.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashRemove
 */
ZFUNC ZINT Zos_DhashRemoveX(ZDHASHID zHashId, ZUINT iType, 
                ZSIZE_T zParm1, ZSIZE_T zParm2, ZVOID * pEntry);

/**
 * @brief Searches a hash entry in a hash table.
 *
 * @param [in] zHashId A pointer to the hash table.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 * @param [out] pdwEntry The hash entry which has been found.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashFindSize
 */
ZFUNC ZBOOL Zos_DhashFind(ZDHASHID zHashId, ZUINT iType, 
                ZSIZE_T zParm1, ZSIZE_T zParm2, ZVOID **ppEntry);

/**
 * @brief Find entry size with same hash key from hash.
 *
 * @param [in] zHashId A pointer to the hash table.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 * @param [out] piSize The entry size with same hash key.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see @ref Zos_DhashFind
 */
ZFUNC ZINT Zos_DhashFindSize(ZDHASHID zHashId, ZUINT iType, 
                ZSIZE_T zParm1, ZSIZE_T zParm2, ZUINT *piSize);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_DHASH_H__ */

