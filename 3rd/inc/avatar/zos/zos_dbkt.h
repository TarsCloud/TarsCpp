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
  File name     : zos_dbkt.h
  Module        : zos dynamic bucket management library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2009-11-04    
  Description   :
      Data structure and function definitions required by the 
      dynamic bucket management interface
    
  Modify History:   
  1. Date:        Author:         Modification:
  2005/01/21      leo.lv          created

*************************************************/
#ifndef _ZOS_DBKT_H__
#define _ZOS_DBKT_H__

/**
 * @file
 * @brief Zos dynamic bucket.
 *
 * @note Bucket contains a bunch of memory block with same size. It is suitable
 *       for those application who request same size block.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Dynamic bucket ID. */
typedef ZVOID * ZDBKT;

/**
 * @brief Create a new dynamic bucket.
 *
 * @param [in] ucIsMutex If ZTRUE, it create mutex lock for thread safe.
 * @param [in] ucIsDynaFree If ZTRUE, it free unused memory block.
 * @param [in] iBktSize Memory block size in bucket.
 * @param [in] wMinCount Min count of block hold in bucket.
 * @param [in] wMaxCount Max count of block the bucket can allocate.
 * @param [in] wIncCount Increase count of block when run out of memory block.
 *
 * @return New dynamic bucket ID, or return ZNULL.
 *
 * @see Zos_DbktDelete
 */
ZFUNC ZDBKT Zos_DbktCreate(ZUCHAR ucIsMutex, ZUCHAR ucIsDynaFree,
                ZUINT iBktSize, ZUSHORT wMinCount, ZUSHORT wMaxCount, 
                ZUSHORT wIncCount);

/**
 * @brief Delete a new dynamic bucket.
 *
 * @param [in] zBkt Dynamic bucket ID.
 *
 * @see Zos_DbktCreate
 */
ZFUNC ZVOID Zos_DbktDelete(ZDBKT zBkt);

/**
 * @brief Get a memory block from bucket.
 *
 * @param [in] zBkt Dynamic bucket ID.
 *
 * @return New allocate memory address if allocate successfully. or return ZNULL.
 *
 * @see Zos_DbktPutBkt
 */
ZFUNC ZVOID * Zos_DbktGetBkt(ZDBKT zBkt);

/**
 * @brief Put a memory block back to bucket.
 *
 * @param [in] zBkt Dynamic bucket ID.
 * @param [in] pMem Allocate memory address.
 *
 * @see Zos_DbktGetBkt
 */
ZFUNC ZVOID Zos_DbktPutBkt(ZDBKT zBkt, ZVOID *pMem);


/**
 * @brief Get count in bucket.
 *
 * @param [in] zBkt Dynamic bucket ID.
 * @param [out] piFreeCount The free bucket node count.
 * @param [out] piBusyCount The busy bucket node count.
 * @param [out] piReqCount The requested bucket node count.
 * @param [out] piRlsCount The released bucket node count.
 * @param [out] piPeekCount The peek bucket node count.
 *
 * @retval ZOK Get bucket count successfully.
 * @retval ZFAILED Get bucket count failed.
 *
 * @see Zos_DbktCreate
 */
ZFUNC ZINT Zos_DbktGetCount(ZDBKT zBkt, ZUINT *piFreeCount,
                ZUINT *piBusyCount, ZUINT *piReqCount, 
                ZUINT *piRlsCount, ZUINT *piPeekCount);

/**
 * @brief Check the memory is in one of allocated block.
 *
 * @param [in] zBkt Dynamic bucket ID.
 * @param [in] pMem Memory address.
 *
 * @retval ZTRUE Memory addres is in one of allocated block.
 * @retval ZFALSE Otherwise.
 *
 * @see Zos_DbktCreate
 */
ZFUNC ZBOOL Zos_DbktValidMem(ZDBKT zBkt, ZVOID *pMem);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_DBKT_H__ */

