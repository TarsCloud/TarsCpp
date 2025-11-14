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
  File name     : zos_pbuf.h
  Module        : zos pipe buffer library functions
  Author        : Leo Lv   
  Version       : 0.1   
  Created on    : 2007-01-16    
  Description   :
      Data structure and function definitions required 
      by the pipe buffer interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_PBUF_H__
#define _ZOS_PBUF_H__

/** 
 * @file zos_pbuf.h
 * @brief Zos pipe buffer functions.
 *
 * Pipe buffer provides a logically continuous buffer. It add and remove data 
 * both at the end. This feature makes it suitable for encode and decode XML.
 * For encode and decode use, this buffer cannot free the allocated all data. 
 * It just can free the tail data as a whole.
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a pipe buffer.
 *
 * @param [in] iBlkSize A block size.
 *
 * @return The pointer of the new created pipe buffer, or ZNULL on failure.
 *
 * @see Zos_AbufCreateD
 */
ZFUNC ZPBUF Zos_PbufCreate(ZUINT iBlkSize);

/**
 * @brief Create a pipe buffer and alloc data.
 *
 * @param [in] iBlkSize A block size.
 * @param [in] iSize Size of the memory to be allocated.
 * @param [out] ppData The user memory.
 *
 * @return The pointer of the new created pipe buffer, or ZNULL on failure.
 *
 * @see Zos_PbufCreate
 */
ZFUNC ZPBUF Zos_PbufCreateD(ZUINT iBlkSize, ZUINT iSize, 
                ZVOID **ppData);

/**
 * @brief Create a pipe buffer and alloc data(clear data to 0).
 *
 * @param [in] iBlkSize A block size.
 * @param [in] iSize Size of the memory to be allocated.
 * @param [out] ppData The user memory.
 *
 * @return The pointer of the new created pipe buffer, or ZNULL on failure.
 *
 * @see Zos_PbufCreate
 */
ZFUNC ZPBUF Zos_PbufCreateClrd(ZUINT iBlkSize, ZUINT iSize, 
                ZVOID **ppData);

/**
 * @brief Delete a pipe buffer and all data block.
 *
 * @param [in] zBuf A pointer to the pipe buffer which is to be deleted.
 *
 * @return [none].
 *
 * @see Zos_PbufCreateD
 */
ZFUNC ZVOID Zos_PbufDelete(ZPBUF zBuf);

/**
 * @brief Alloc data memory form pbuf.
 *
 * @param [in] zBuf A pointer to the pipe buffer 
 *                    which memory will be allocated.
 * @param [in] iSize Size of the memory to be allocated.
 *
 * @return [none]
 *
 * @see Zos_PbufFree
 */
ZFUNC ZVOID * Zos_PbufAlloc(ZPBUF zBuf, ZUINT iSize);

/**
 * @brief Alloc data memory from pbuf and clear data to 0.
 *
 * @param [in] zBuf A block size.
 * @param [in] iSize Size of the memory to be allocated.
 *
 * @return [none]
 *
 * @see Zos_PbufCreateClrd
 */
ZFUNC ZVOID * Zos_PbufAllocClrd(ZPBUF zBuf, ZUINT iSize);

/**
 * @brief Free memory from pipe buffer.
 *
 * @param [in] zBuf A pipe buffer pointer.
 * @param [in] pMem The memory will be freed.
 *
 * @return [none]
 *
 * @see Zos_PbufAlloc
 */
ZFUNC ZINT Zos_PbufFree(ZPBUF zBuf, ZVOID *pMem);

/**
 * @brief Get the using length of all data block in pipe buffer.
 *
 * @param [in] zBuf A pipe buffer pointer.
 *
 * @return The length, or return 0.
 *
 * @see Zos_PbufSize
 */
ZFUNC ZUINT Zos_PbufLen(ZPBUF zBuf);

/**
 * @brief Get the size of all data block in pipe buffer.
 *
 * @param [in] zBuf A pipe buffer pointer.
 *
 * @return The size, or return 0.
 *
 * @see Zos_PbufLen
 */
ZFUNC ZUINT Zos_PbufSize(ZPBUF zBuf);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_PBUF_H__ */

