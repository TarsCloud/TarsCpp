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
  File name     : zos_sbuf.h
  Module        : zos structure buffer library functions
  Author        : Leo Lv   
  Version       : 0.1   
  Created on    : 2007-01-16    
  Description   :
      Data structure and function definitions required 
      by the structure buffer interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_SBUF_H__
#define _ZOS_SBUF_H__

/**
 * @file
 * @brief Zos structure buffer.
 *
 * Structure buffer(sbuf) is base on @ref zos_pmem.h "Power Memory".
 * User can locate the sbuf ID from memory address. It request memory, 
 * call page, from @ref zos_pmem.h "Power Memory", and allocate data block
 * from page.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a sbuf.
 *
 * @param [in] iPageSize Page size in bytes.
 *
 * @return Structure buffer ID on successfully, or return ZNULL.
 *
 * @see @ref Zos_SbufDelete
 */
ZFUNC ZSBUF Zos_SbufCreate(ZUINT iPageSize);

/**
 * @brief Create a sbuf and allocate a data block.
 *
 * @param [in] iPageSize Page size in bytes.
 * @param [in] iSize Data block size in bytes.
 * @param [out] ppData Data address allocated.
 *
 * @return Structure buffer ID on successfully, or return ZNULL.
 *
 * @see @ref Zos_SbufCreate
 */
ZFUNC ZSBUF Zos_SbufCreateD(ZUINT iPageSize, ZUINT iSize, 
                ZVOID **ppData);

/**
 * @brief Create a sbuf and allocate a data block cleared by 0.
 *
 * @param [in] iPageSize Page size in bytes.
 * @param [in] iSize Data block size in bytes.
 * @param [out] ppData Data address allocated.
 *
 * @return Structure buffer ID on successfully, or return ZNULL.
 *
 * @see @ref Zos_SbufCreate
 */
ZFUNC ZSBUF Zos_SbufCreateClrd(ZUINT iPageSize, ZUINT iSize, 
                ZVOID **ppData);

/**
 * @brief Delete a sbuf and free all data.
 *
 * @param [in] zBuf Structure buffer ID.
 *
 * @return [None].
 *
 * @see @ref Zos_SbufCreate
 */
ZFUNC ZVOID Zos_SbufDelete(ZSBUF zBuf);

/**
 * @brief Clean a sbuf and free all data.
 *
 * @param [in] zBuf Structure buffer ID.
 *
 * @return [None].
 *
 * @see @ref Zos_SbufDelete
 */
ZFUNC ZVOID Zos_SbufClean(ZSBUF zBuf);

/**
 * @brief Allocate a data block.
 *
 * @param [in] zBuf Structure buffer ID.
 * @param [in] iSize Data block size in bytes.
 *
 * @return Data address allocated on successfully, or return ZNULL.
 *
 * @see @ref Zos_SbufAllocClrd
 */
ZFUNC ZVOID * Zos_SbufAlloc(ZSBUF zBuf, ZUINT iSize);

/**
 * @brief Allocate a data block cleared by 0.
 *
 * @param [in] zBuf Structure buffer ID.
 * @param [in] iSize Data block size in bytes.
 *
 * @return Data address allocated on successfully, or return ZNULL.
 *
 * @see @ref Zos_SbufAlloc
 */
ZFUNC ZVOID * Zos_SbufAllocClrd(ZSBUF zBuf, ZUINT iSize);

/**
 * @brief Allocated data block length.
 *
 * @param [in] zBuf Structure buffer ID.
 *
 * @return Allocate a data block length.
 *
 * @see @ref Zos_SbufSize
 */
ZFUNC ZUINT Zos_SbufLen(ZSBUF zBuf);

/**
 * @brief Size of memory hold in sbuf.
 *
 * @param [in] zBuf Structure buffer ID.
 *
 * @return Size of memory hold in sbuf.
 *
 * @see @ref Zos_SbufPageRoom
 */
ZFUNC ZUINT Zos_SbufSize(ZSBUF zBuf);

/**
 * @brief Page room size of sbuf.
 *
 * @param [in] zBuf Structure buffer ID.
 *
 * @return Size of memory hold in sbuf.
 *
 * @see @ref Zos_SbufSize
 */
ZFUNC ZUINT Zos_SbufPageRoom(ZSBUF zBuf);

/**
 * @brief Get cookie from sbu.
 *
 * @return The cookie in the buffer.
 *
 * @see @ref Zos_SbufSetCookie
 */
ZFUNC ZCOOKIE Zos_SbufGetCookie(ZSBUF zBuf);

/**
 * @brief Set cookie into sbu.
 *
 * @return The cookie in the buffer.
 *
 * @retval ZOK Set cookie successfully.
 * @retval ZFAILED Set cookie failed.
 *
 * @see @ref Zos_SbufGetCookie
 */
ZFUNC ZINT Zos_SbufSetCookie(ZSBUF zBuf, ZCOOKIE zCookie);

/**
 * @brief Get sbuf ID from data address.
 *
 * @param [in] iPageSize Page size in bytes.
 * @param [in] pData Data address.
 *
 * @return Structure buffer ID on successfully, or return ZNULL.
 *
 * @see @ref Zos_SbufD2O
 */
ZFUNC ZSBUF Zos_SbufD2M(ZUINT iPageSize, ZVOID *pData);

/**
 * @brief Get offset of specific data address.
 *
 * @param [in] zBuf Structure buffer ID.
 * @param [in] pData Data address.
 * @param [out] pzOffset Offset value.
 *
 * @retval ZOK Data address is in sbuf.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Zos_SbufD2M
 */
ZFUNC ZINT Zos_SbufD2O(ZSBUF zBuf, ZVOID *pData, ZSIZE_T *pzOffset);

/**
 * @brief Get data address of specific offset.
 *
 * @param [in] zBuf Structure buffer ID.
 * @param [in] zOffset Offset value.
 * @param [out] ppData Data address.
 *
 * @retval ZOK Get data address successfully.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Zos_SbufD2O
 */
ZFUNC ZINT Zos_SbufO2D(ZSBUF zBuf, ZSIZE_T zOffset, ZVOID **ppData);

/**
 * @brief Check if data address is hold in sbuf.
 *
 * @param [in] zBuf Structure buffer ID.
 * @param [in] pData Data address.
 *
 * @retval ZTRUE Data address is hold in sbuf.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Zos_SbufO2D
 */
ZFUNC ZBOOL Zos_SbufHoldD(ZSBUF zBuf, ZVOID *pData);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_SBUF_H__ */

