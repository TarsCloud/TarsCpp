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
  File name     : zos_hash.h
  Module        : zos hash library functions
  Author        : zocol  
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the hash interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_HASH_H__
#define _ZOS_HASH_H__

/** 
 * @file zos_hash.h
 * @brief Zos hash interfaces.
 *  
 * In this file, it defined the operation function
 * of zos hash, including create, remove, insert
 * and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif
    
/** @brief zos hash key and compare funciton pointer macros */
typedef ZINT (*PFN_ZHASHKEY)(ZUINT iType, ZSIZE_T zParm1,  \
                ZSIZE_T zParm2, ZUINT *piHashKey);
    
typedef ZINT (*PFN_ZHASHCMP)(ZVOID * pEntry, ZUINT iType,  \
                ZSIZE_T zParm1, ZSIZE_T zParm2);

/** @brief zos hash node */
typedef struct tagZOS_HASH_NODE
{
    struct tagZOS_HASH_NODE *pstNext;/**< @brief next the hash entry of same hash key */
    ZVOID * pEntry;                  /**< @brief hash entry stored in a hash table */
} ST_ZOS_HASH_NODE;

/** @brief zos hash table */
typedef struct tagZOS_HASH
{
    ZCBUF zMemBuf;                 /**< @brief memory buffer */
    ST_ZOS_HASH_NODE **ppstHashTable; /**< @brief hash table */
    ST_ZOS_HASH_NODE *pstFreeHashLst; /**< @brief the free hash node list */
    PFN_ZHASHKEY pfnHashKey;         /**< @brief hash key function pointer */
    PFN_ZHASHCMP pfnHashCmp;         /**< @brief hash entry compare function pointer */
    ZUINT iHashSize;               /**< @brief size of hash table */
    ZOS_PADX64
} ST_ZOS_HASH;

/**
 * @brief Constructs a hash key from a case sensitive string.
 *
 * @param [in] iType Type of the string.
 * @param [in] zStrAddr The string address.
 * @param [in] iStrLen Length of the string.
 * @param [out] iHashKey The address of the hash key.
 *
 * @return ZOK Constructs a hash key successfully, otherwise ZFAILED.
 *
 * @see Zos_HashKeyFromStrNoCase
 */
ZFUNC ZINT Zos_HashKeyFromStr(ZUINT iType, ZSIZE_T zParm1, ZSIZE_T zParm2, 
                ZUINT *piHashKey);

/**
 * @brief Constructs a hash key from a not case sensitive string.
 *
 * @param [in] iType Type of the string.
 * @param [in] zStrAddr The string address.
 * @param [in] iStrLen Length of the string.
 * @param [out] iHashKey The address of the hash key.
 *
 * @return ZOK Constructs a hash key successfully, otherwise ZFAILED.
 *
 * @see Zos_HashKeyFromStr
 */
ZFUNC ZINT Zos_HashKeyFromStrNoCase(ZUINT iType, ZSIZE_T zParm1,
                ZSIZE_T zParm2, ZUINT *piHashKey);

/**
 * @brief Constructs a hash key from a parameter of type ZUINT.
 *
 * @param [in] iType The parameter type.
 * @param [in] iParm1 The parameter of type ZUINT.
 * @param [in] iParm2 Unused parameter.
 * @param [out] iHashKey The address of the hash key.
 *
 * @return ZOK Constructs a hash key successfully, otherwise ZFAILED.
 *
 * @see Zos_HashKeyFromStrNoCase
 */
ZFUNC ZINT Zos_HashKeyFromUint32(ZUINT iType, ZSIZE_T zParm1,
                ZSIZE_T zParm2, ZUINT *piHashKey);

/**
 * @brief Creates a ZOS hash table.
 *
 * @param [out] pstHash A pointer to the hash table which is to be created.
 * @param [in] iHashSize Size of the hash table.
 * @param [in] pfnHashKey A pointer to hash key production funtion.
 * @param [in] pfnHashCmp A pointer to hash entry compare function.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashDelete
 */
ZFUNC ZINT Zos_HashCreate(ST_ZOS_HASH *pstHash, ZUINT iHashSize,
                PFN_ZHASHKEY pfnHashKey, PFN_ZHASHCMP pfnHashCmp);

/**
 * @brief Creates a ZOS hash with specific node size.
 *
 * @param [out] pstHash A pointer to the hash table which is to be created.
 * @param [in] iHashSize Size of the hash table.
 * @param [in] iNodeSize Size of the node.
 * @param [in] pfnHashKey A pointer to hash key production funtion.
 * @param [in] pfnHashCmp A pointer to hash entry compare function.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashDelete
 */
ZFUNC ZINT Zos_HashCreateX(ST_ZOS_HASH *pstHash, ZUINT iHashSize,
                ZUINT iNodeSize, PFN_ZHASHKEY pfnHashKey, 
                PFN_ZHASHCMP pfnHashCmp);

/**
 * @brief Creates a ZOS hash from aggregation buffer.
 *
 * @param [in] zMemBuf Aggregation buffer id.
 * @param [in] iHashSize Size of the hash table.
 * @param [in] pfnHashKey A pointer to hash key production funtion.
 * @param [in] pfnHashCmp A pointer to hash entry compare function.
 * @param [out] ppstHash A pointer to the hash table which is to be created.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashDelete
 */
ZFUNC ZINT Zos_HashCreateA(ZCBUF zMemBuf, ZUINT iHashSize,
                PFN_ZHASHKEY pfnHashKey, PFN_ZHASHCMP pfnHashCmp,
                ST_ZOS_HASH **ppstHash);

/**
 * @brief Creates a ZOS hash from aggregation buffer with specific node size.
 *
 * @param [in] zMemBuf Aggregation buffer id.
 * @param [in] iHashSize Size of the hash table.
 * @param [in] iNodeSize Size of the node.
 * @param [in] pfnHashKey A pointer to hash key production funtion.
 * @param [in] pfnHashCmp A pointer to hash entry compare function.
 * @param [out] ppstHash A pointer to the hash table which is to be created.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashDelete
 */
ZFUNC ZINT Zos_HashCreateAX(ZCBUF zMemBuf, ZUINT iHashSize,
                ZUINT iNodeSize, PFN_ZHASHKEY pfnHashKey, 
                PFN_ZHASHCMP pfnHashCmp, ST_ZOS_HASH **ppstHash);

/**
 * @brief This function deletes a hash table.
 *
 * @param [in] pstHash A pointer to the hash table which is to be deleted.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashCreate
 */
ZFUNC ZINT Zos_HashDelete(ST_ZOS_HASH *pstHash);

/**
 * @brief Inserts a hash entry into a hash table.
 *
 * @param [in] pstHash A pointer to a hash table.
 * @param [in] pEntry The hash entry.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashRemove
 */
ZFUNC ZINT Zos_HashInsert(ST_ZOS_HASH *pstHash, ZVOID * pEntry, ZUINT iType, 
                ZSIZE_T zParm1, ZSIZE_T zParm2);

/**
 * @brief Remove a hash entry from a hash table.
 *
 * @param [in] pstHash A pointer to the hash table.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashInsert
 */
ZFUNC ZINT Zos_HashRemove(ST_ZOS_HASH *pstHash, ZUINT iType, 
                ZSIZE_T zParm1, ZSIZE_T zParm2);

/**
 * @brief Searches a hash entry in a hash table.
 *
 * @param [in] pstHash A pointer to the hash table.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 * @param [out] ppEntry The address of the hash entry.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashInsert
 */
ZFUNC ZBOOL Zos_HashFind(ST_ZOS_HASH *pstHash, ZUINT iType, ZSIZE_T zParm1,
                ZSIZE_T zParm2, ZVOID **ppEntry);

/**
 * @brief Searches a hash entry by its hash key in hash table.
 *
 * @param [in] pstHash A pointer to the hash table.
 * @param [in] iHashKey A Hash key.
 * @param [in] iParm1 Parameter 1.
 * @param [in] zParm2 Parameter 2.
 * @param [in] iParm3 Parameter 3. 
 * @param [out] ppEntry The address of the hash entry.
 *
 * @retval ZOK Successfully.
 * @retval ZFAILED Failed.
 *
 * @see Zos_HashInsert
 */
ZFUNC ZBOOL Zos_HashFindByKey(ST_ZOS_HASH *pstHash, ZUINT iHashKey, 
                ZUINT iType, ZSIZE_T zParm1, ZSIZE_T zParm2, ZVOID **ppEntry);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_HASH_H__ */

