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
  File name     : zos_dbuf.h
  Module        : zos data buffer library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-12-29    
  Description   :
      Data structure and function definitions required 
      by the data buffer interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_DBUF_H__
#define _ZOS_DBUF_H__

/** 
 * @file zos_dbuf.h
 * @brief Zos data buffer functions.
 *
 * Data buffer provides a logically continuous buffer. It add and remove data
 * both at the beginning and end. This feature makes it suitable for
 * encode and decode protocol package in text, like SIP. And you can clone
 * it when exchanging data between modules without copy to promote performance.
 *
 * For encode and decode use, this buffer cannot free the allocated data 
 * individually. It just can free all the data as a whole.
 *
 * Most interface are not thread safe. Programmer must keep it thread safe.
 * Following is a recommand usage: one thread should create and then
 * operate data in buffer, then clone and pass it to another thread. After
 * that, the first thread should delete the buffer, and never use it again.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
#define ZDBUF_TYPE_NULL           0  /**< @brief Type undefined. */
#define ZDBUF_TYPE_BYTE           1  /**< @brief Data in buffer was aligned by byte */
#define ZDBUF_TYPE_STRUCT         2  /**< @brief Data in buffer was aligned by 4 bytes */
/** @} */

/* zos data buffer dump type */
#define ZDBUF_DUMP_CREATE         0  /* dump create */
#define ZDBUF_DUMP_DELETE         1  /* dump delete */
#define ZDBUF_DUMP_CLONE          2  /* dump clone */

/** @{
 * @brief Create dump in data buffer. These information is for debug. 
 * @see @ref Zos_DumpCreate
 */
#define ZOS_DBUF_DUMP_CREATE(_dbuf, _name, _deep) \
    Zos_DbufDumpCreate(_dbuf, _name, _deep, __FILE__, __LINE__)

/** 
 * @brief Delete dump in data buffer. 
 * @see @ref Zos_DumpDelete
 */
#define ZOS_DBUF_DUMP_DELETE(_dbuf) \
    Zos_DbufDumpDelete(_dbuf)

/** 
 * @brief Add information to data buffer's dump stack. 
 * @see @ref Zos_DumpStack
 */
#define ZOS_DBUF_DUMP_STACK(_dbuf, _type) \
    Zos_DbufDumpStack(_dbuf, __FILE__, __LINE__, _type)

/** 
 * @brief Create ZDBUF_TYPE_STRUCT type data buffer with dump. 
 * @see @ref Zos_DbufCreate
 */
#define ZOS_MEMBUF_CREATE(_buf, _fsize, _deep, _desc) do { \
    _buf = Zos_DbufCreate(ZNULL, ZDBUF_TYPE_STRUCT, _fsize); \
    ZOS_DBUF_DUMP_CREATE(_buf, _desc, _deep); \
} while (0)

/** 
 * @brief Create ZDBUF_TYPE_STRUCT type data buffer with dump, and allocate data. 
 * @see @ref Zos_DbufCreateD
 */
#define ZOS_MEMBUF_CREATED(_buf, _fsize, _deep, _desc, _size, _data) do { \
    _buf = Zos_DbufCreateD(ZNULL, ZDBUF_TYPE_STRUCT, _fsize, \
                              _size, (ZVOID **)&_data); \
    ZOS_DBUF_DUMP_CREATE(_buf, _desc, _deep); \
} while (0)

/** 
 * @brief Create ZDBUF_TYPE_STRUCT type data buffer with dump, 
 *        and allocate clear data. 
 * @see @ref Zos_DbufCreateClrd
 */
#define ZOS_MEMBUF_CREATECLRD(_buf, _fsize, _deep, _desc, _size, _data) do { \
    _buf = Zos_DbufCreateClrd(ZNULL, ZDBUF_TYPE_STRUCT, _fsize, \
                                 _size, (ZVOID **)&_data); \
    ZOS_DBUF_DUMP_CREATE(_buf, _desc, _deep); \
} while (0)

/** 
 * @brief Create ZDBUF_TYPE_BYTE type data buffer with dump. 
 * @see @ref Zos_DbufCreate
 */
#define ZOS_MSGBUF_CREATE(_buf, _fsize, _deep, _desc) do { \
    _buf = Zos_DbufCreate(ZNULL, ZDBUF_TYPE_BYTE, _fsize); \
    ZOS_DBUF_DUMP_CREATE(_buf, _desc, _deep); \
} while (0)

/** 
 * @brief Create ZDBUF_TYPE_BYTE type data buffer with dump, and allocate data. 
 * @see @ref Zos_DbufCreateD
 */
#define ZOS_MSGBUF_CREATED(_buf, _fsize, _deep, _desc, _size, _data) do { \
    _buf = Zos_DbufCreateD(ZNULL, ZDBUF_TYPE_BYTE, _fsize, \
                           _size, (ZVOID **)&_data); \
    ZOS_DBUF_DUMP_CREATE(_buf, _desc, _deep); \
} while (0)

/** 
 * @brief Create ZDBUF_TYPE_BYTE type data buffer with dump, 
 *        and add data to buffer. 
 * @see @ref Zos_DbufCreateAddD
 */
#define ZOS_MSGBUF_CREATE_ADDD(_buf, _fsize, _deep, _desc, _data, _size) do { \
    _buf = Zos_DbufCreateAddD(ZNULL, ZDBUF_TYPE_BYTE, _fsize, _data, _size); \
    ZOS_DBUF_DUMP_CREATE(_buf, _desc, _deep); \
} while (0)

/** 
 * @brief Delete data buffer. 
 * @see @ref Zos_DbufDelete
 */
#define ZOS_DBUF_DELETE(_dbuf) do { \
    ZOS_DBUF_DUMP_STACK(_dbuf, ZDBUF_DUMP_DELETE); \
    Zos_DbufDelete(_dbuf); \
} while (0)

/** 
 * @brief Clone data buffer. 
 * @see @ref Zos_DbufClone
 */
#define ZOS_DBUF_CLONE(_dbuf) do { \
    Zos_DbufClone(_dbuf); \
    ZOS_DBUF_DUMP_STACK(_dbuf, ZDBUF_DUMP_CLONE); \
} while (0)

/** 
 * @brief Clone data buffer and assigned it to a new buffer. 
 * @see @ref Zos_DbufClone
 */
#define ZOS_DBUF_CLONEX(_dbuf, _newbuf) do { \
    _newbuf = Zos_DbufClone(_dbuf); \
    ZOS_DBUF_DUMP_STACK(_dbuf, ZDBUF_DUMP_CLONE); \
} while (0)

/**
 * @brief Create a new data buffer.
 *
 * @param [in] zPoolId The memory pool used by data buffer. If ZNULL, it will
 *                     use system default, m_zZosDbufPoolId.
 * @param [in] ucType Data buffer type, ZDBUF_TYPE_BYTE or ZDBUF_TYPE_STRUCT.
 * @param [in] iDftBlkSize Default size of data block.
 *
 * @return New data buffer when successfully, or return ZNULL.
 *
 * @see @ref Zos_DbufCreateD
 */
ZFUNC ZDBUF Zos_DbufCreate(ZPOOL zPoolId, ZUCHAR ucType, 
                ZUINT iDftBlkSize);

/**
 * @brief Create a new data buffer, and allocate a data block.
 *
 * @param [in] zPoolId The memory pool used by data buffer. If ZNULL, it will
 *                     use system default, m_zZosDbufPoolId.
 * @param [in] ucType Data buffer type, ZDBUF_TYPE_BYTE or ZDBUF_TYPE_STRUCT.
 * @param [in] iDftBlkSize Default size of data block.
 * @param [in] iSize The size of data to get.
 * @param [out] ppData The data allocated.
 *
 * @return New data buffer when successfully, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_DbufCreate create new buffer.
 *       -# Call Zos_DbufAlloc allocate data.
 *
 * @see @ref Zos_DbufCreate Zos_DbufAlloc
 */
ZFUNC ZDBUF Zos_DbufCreateD(ZPOOL zPoolId, ZUCHAR ucType, 
                ZUINT iDftBlkSize, ZUINT iSize, ZVOID **ppData);

/**
 * @brief Create a new data buffer, and allocate a data block cleared by 0.
 *
 * @param [in] zPoolId The memory pool used by data buffer. If ZNULL, it will
 *                     use system default, m_zZosDbufPoolId.
 * @param [in] ucType Data buffer type, ZDBUF_TYPE_BYTE or ZDBUF_TYPE_STRUCT.
 * @param [in] iDftBlkSize Default size of data block.
 * @param [in] iSize The size of data to get.
 * @param [out] ppData The data allocated.
 *
 * @return New data buffer when successfully, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_DbufCreate create new buffer.
 *       -# Call Zos_DbufAllocClrd allocate data cleared by 0.
 *
 * @see @ref Zos_DbufCreateD Zos_DbufAllocClrd
 */
ZFUNC ZDBUF Zos_DbufCreateClrd(ZPOOL zPoolId, ZUCHAR ucType, 
                ZUINT iDftBlkSize, ZUINT iSize, ZVOID **ppData);

/**
 * @brief Create a new data buffer, and add a data to buffer.
 *
 * @param [in] zPoolId The memory pool used by data buffer. If ZNULL, it will
 *                     use system default, m_zZosDbufPoolId.
 * @param [in] ucType Data buffer type, ZDBUF_TYPE_BYTE or ZDBUF_TYPE_STRUCT.
 * @param [in] iDftBlkSize Default size of data block.
 * @param [in] pucData The data to added.
 * @param [in] iLen The data length.
 *
 * @return New data buffer when successfully, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_DbufCreate create new buffer.
 *       -# Call Zos_DbufAlloc allocate data.
 *       -# Call Zos_MemCpy copy data to new allocated data block.
 *
 * @see @ref Zos_DbufCreate
 */
ZFUNC ZDBUF Zos_DbufCreateAddD(ZPOOL zPoolId, ZUCHAR ucType, 
                ZUINT iDftBlkSize, ZUCHAR *pucData, ZUINT iLen);

/**
 * @brief Delete data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return [none]
 *
 * @note This function decrease the refernce count of data buffer.
 *       And free all used memory and destory the structure when refernece
 *       count is 0.
 *      
 * @see @ref Zos_DbufCreate Zos_DbufClone
 */
ZFUNC ZVOID Zos_DbufDelete(ZDBUF zBuf);

/**
 * @brief Free all data block in data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return [none]
 *
 * This function only free the data block in buffer.
 *
 * @see @ref Zos_DbufDelete
 */
ZFUNC ZVOID Zos_DbufFree(ZDBUF zBuf);

/**
 * @brief Allocate data block from data buffer.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iSize Data blcok size request.
 *
 * @return Data block if successfully, or return ZNULL.
 *
 * @see @ref Zos_DbufCreate Zos_DbufSCreate
 */
ZFUNC ZVOID * Zos_DbufAlloc(ZDBUF zBuf, ZUINT iSize);

/**
 * @brief Allocate data block from data buffer, which clear by 0.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iSize Data blcok size request.
 *
 * @return Data block if successfully, or return ZNULL.
 *
 * @see @ref Zos_DbufAlloc Zos_DbufCreate Zos_DbufSCreate
 */
ZFUNC ZVOID * Zos_DbufAllocClrd(ZDBUF zBuf, ZUINT iSize);

/**
 * @brief Get data buffer type.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return Buffer type(ZDBUF_TYPE_BYTE or ZDBUF_TYPE_STRUCT), 
 * or return ZDBUF_TYPE_NULL.
 *
 * @see @ref Zos_DbufAlloc Zos_DbufCreate Zos_DbufSCreate
 */
ZFUNC ZUCHAR Zos_DbufType(ZDBUF zBuf);

/**
 * @brief Get total size in bytes of data block, including used and free.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return Total size in bytes of data block.
 *
 * @note When data buffer created, it will pre-allocate a bunch of data block 
 *       from system. So call Zos_DbufAlloc it only search and allocate data block
 *       from data block it reserved, and do not request from system directly.
 *       When data buffer run out of data block, it will allocate another bunch
 *       of data block from system again.\n
 *       This function returns the total size of data block which the data buffer
 *       request from system. To get the used size of data block, using Zos_DbufLen.
 *
 * @see @ref Zos_DbufCreate Zos_DbufLen
 */
ZFUNC ZUINT Zos_DbufSize(ZDBUF zBuf);

/**
 * @brief Get size in bytes of used data block.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return Size in bytes of used data block.
 *
 * @note Following data will be count in used:
 *       -# Allocate data, as Zos_DbufAlloc, Zos_DbufAllocClrd, Zos_DbufCreateD ...
 *       -# Add data, as Zos_DbufCreateAddD Zos_DbufCat Zos_DbufPreAddD ...
 *
 * @note To get the total size of data block which the data buffer
 *       request from system, using Zos_DbufSize
 *      
 * @see @ref Zos_DbufSize
 */
ZFUNC ZUINT Zos_DbufLen(ZDBUF zBuf);

/**
 * @brief Set new buffer size in bytes of used data block.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iLen New buffer size.
 *
 * @retval ZOK Set buffer new size successfully.
 * @retval ZFAILED Set buffer new size failed.
 *
 * @see @ref Zos_DbufSize
 */
ZFUNC ZINT Zos_DbufNewLen(ZDBUF zBuf, ZUINT iLen);

/**
 * @brief Get reference count of data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return Reference count of data buffer.
 *
 * @see @ref Zos_DbufSize
 */
ZFUNC ZUINT Zos_DbufRefCnt(ZDBUF zBuf);

/**
 * @brief Get default data block size of data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return Default data block size of data buffer.
 *
 * @see @ref Zos_DbufSize
 */
ZFUNC ZUINT Zos_DbufDftBlkSize(ZDBUF zBuf);

/**
 * @brief Get count of used data block.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return Count of used data block.
 *
 * @see @ref Zos_DbufLen
 */
ZFUNC ZUINT Zos_DbufBlkSize(ZDBUF zBuf);

/**
 * @brief Get used size in bytes of specific data block.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iIndex Data block index, from 0 to Zos_DbufBlkSize - 1.
 *
 * @return Used size in bytes of specific data block.
 *
 * @see @ref Zos_DbufBlkSize
 */
ZFUNC ZUINT Zos_DbufBlkLen(ZDBUF zBuf, ZUINT iIndex);

/**
 * @brief Get the next data buffer of current data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return The next data buffer id.
 *
 * @see @ref Zos_DbufSetNext
 */
ZFUNC ZDBUF Zos_DbufGetNext(ZDBUF zBuf);

/**
 * @brief Set the next data buffer of current data buffer.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] zNextBufId The next Data buffer id.
 *
 * @retval ZOK Set the next buffer successfully.
 * @retval ZFAILED Set the next buffer failed.
 *
 * @see @ref Zos_DbufGetNext
 */
ZFUNC ZINT Zos_DbufSetNext(ZDBUF zBuf, ZDBUF zNextBufId);

/**
 * @brief Get the offset of specific data address.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] pcData Data address.
 * @param [out] piOffset Offset of specific data address.
 *
 * @retval ZOK Search for offset successfully.
 * @retval ZFAILED Search for offset failed.
 *
 * @see @ref Zos_DbufBlkD2O
 */
ZFUNC ZINT Zos_DbufD2O(ZDBUF zBuf, ZCHAR *pcData, ZUINT *piOffset);

/**
 * @brief Get the data address of specific offset.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iOffset Offset in bytes.
 * @param [out] ppcData Data address of specific offset.
 *
 * @retval ZOK Search for data address successfully.
 * @retval ZFAILED Search for data address failed.
 *
 * @see @ref Zos_DbufBlkO2D
 */
ZFUNC ZINT Zos_DbufO2D(ZDBUF zBuf, ZUINT iOffset, ZCHAR **ppcData);

/**
 * @brief Get the block index and offset of specific data address.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] pcData Data address.
 * @param [out] piIndex Block index.
 * @param [out] piOffset Offset in block.
 *
 * @retval ZOK Search for offset successfully.
 * @retval ZFAILED Search for offset failed.
 *
 * @see @ref Zos_DbufD2O
 */
ZFUNC ZINT Zos_DbufBlkD2O(ZDBUF zBuf, ZCHAR *pcData, 
                ZUINT *piIndex, ZUINT *piOffset);

/**
 * @brief Get the data address of specific block and offset.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iIndex Block index.
 * @param [in] iOffset Offset in bytes.
 * @param [out] ppcData Data address of specific offset.
 *
 * @retval ZOK Search for data address successfully.
 * @retval ZFAILED Search for data address failed.
 *
 * @see @ref Zos_DbufO2D
 */
ZFUNC ZINT Zos_DbufBlkO2D(ZDBUF zBuf, ZUINT iIndex, 
                ZUINT iOffset, ZCHAR **ppcData);

/**
 * @brief Trim specific length from data buffer.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iLen Adjust length. If it is positive, trim data at the end
 *                   of buffer. Or trim data from beginning.
 *
 * @retval ZOK Trim data successfully.
 * @retval ZFAILED Trim data failed.
 *
 * @see
 */
ZFUNC ZINT Zos_DbufAdjD(ZDBUF zBuf, ZINT iLen);

/**
 * @brief Create a new data buffer, and copy all data to it.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return New created data buffer if successfully. Or return ZNULL.
 *
 * @note New created data buffer and old buffer are 2 isolated buffer.
 *       To duplicate buffer without copy, using Zos_DbufClone.
 *      
 * @see @ref Zos_DbufClone
 */
ZFUNC ZDBUF Zos_DbufCopy(ZDBUF zBuf);

/**
 * @brief Create a new data buffer, and copy all data to it.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return New created data buffer if successfully. Or return ZNULL.
 *
 * @note New created data buffer and old buffer are 2 isolated buffer.
 *       To duplicate buffer without copy, using Zos_DbufClone.
 *      
 * @see @ref Zos_DbufClone
 */
ZFUNC ZDBUF Zos_DbufCopyX(ZDBUF zBuf, ZUINT iOffset, 
                ZUINT iLen);

/**
 * @brief Copy specific length of data from offset to memory address.
 *
 * @param [in] zBuf Data buffer read from.
 * @param [in] iOffset Offset in data buffer copy from.
 * @param [in] iLen Length to copy.
 * @param [out] pcData Memory address copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_DbufFlat
 */
ZFUNC ZINT Zos_DbufCopyD(ZDBUF zBuf, ZUINT iOffset, ZUINT iLen, 
                ZCHAR *pcData);

/**
 * @brief Clone data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @return Data buffer if clone successfully. Or return ZNULL.
 *
 * @note The cloned data buffer is using exact the same structure of old one's.
 *       Cloned data buffer should also delete by Zos_DbufDelete. To create a
 *       completely new buffer, using Zos_DbufCopy.
 *
 * @see @ref Zos_DbufCopy Zos_DbufDelete
 */
ZFUNC ZDBUF Zos_DbufClone(ZDBUF zBuf);

/**
 * @brief Split data buffer at specific offset.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] iOffset Split offset.
 *
 * @return New data buffer contains data after split offset if successfully.
 *         Or return ZNULL.
 *
 * @see @ref Zos_DbufCat
 */
ZFUNC ZDBUF Zos_DbufSplit(ZDBUF zBuf, ZUINT iOffset);

/**
 * @brief Cat the data of source buffer to the end of destination buffer..
 *
 * @param [in] zDstBufId Destination buffer the data add to.
 * @param [in] zSrcBufId Source buffer the data read from.
 *
 * @retval ZOK Cat data successfully.
 * @retval ZFAILED Cat data failed.
 *
 * @see @ref Zos_DbufSplit
 */
ZFUNC ZINT Zos_DbufCat(ZDBUF zDstBufId, ZDBUF zSrcBufId);

/**
 * @brief Check if buffer is flat.
 *
 * @param [in] zBuf Data buffer to check.
 *
 * @retval ZTRUE Data buffer is flat.
 * @retval ZFALSE Data buffer is not flat.
 *
 * @see @ref Zos_DbufFlat @ref Zos_DbufFlatX
 */
ZFUNC ZBOOL Zos_DbufIsFlat(ZDBUF zBuf);

/**
 * @brief Create new data buffer, and copy all data to it. Ensure data 
 *        int new buffer is stored in a continous memory block.
 *
 * @param [in] zBuf Data buffer to adjust.
 *
 * @retval New data buffer.
 *
 * @note Usually, the data in buffer may be stored in different data block.
 *       They may locate on discontinuous memory blocks. 
 *       This function create new data buffer, and copy the data to a continuous 
 *       memory block. Then you can call Zos_DbufO2D to get the memory address 
 *       and Zos_DbufLen to get data size.
 *
 * @see @ref Zos_DbufO2D @ref Zos_DbufLen
 */
ZFUNC ZDBUF Zos_DbufFlat(ZDBUF zBuf);

/**
 * @brief Create new data buffer, and copy all data to it. Ensure data 
 *        int new buffer is stored in a continous memory block.
 *
 * @param [in] zBuf Data buffer to adjust.
 * @param [in] iOffset Offset in data buffer adjust from.
 * @param [in] iLen Length to adjust.
 *
 * @retval New data buffer.
 *
 * @note Usually, the data in buffer may be stored in different data block.
 *       They may locate on discontinuous memory blocks. 
 *       This function create new data buffer, and copy the data to a continuous 
 *       memory block. Then you can call Zos_DbufO2D to get the memory address 
 *       and Zos_DbufLen to get data size.
 *
 * @see @ref Zos_DbufO2D @ref Zos_DbufLen
 */
ZFUNC ZDBUF Zos_DbufFlatX(ZDBUF zBuf, ZUINT iOffset, ZUINT iLen);

/**
 * @brief Add 1 byte data just before the beginning.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] ucData Data value.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPreAddStr Zos_DbufPreAddMultD
 */
ZFUNC ZINT Zos_DbufPreAddD(ZDBUF zBuf, ZUCHAR ucData);

/**
 * @brief Add specific length data just before the beginning.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] pucData Data address.
 * @param [in] iLen Data Length in bytes.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPreAddD Zos_DbufPreAddStr
 */
ZFUNC ZINT Zos_DbufPreAddMultD(ZDBUF zBuf, ZUCHAR *pucData, 
                ZUINT iLen);

/**
 * @brief Add a string data just before the beginning.
 *
 * @param [in] zBufId Destination buffer the data add to.
 * @param [in] pcData String data address.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPreAddD Zos_DbufPreAddStr
 */
ZFUNC ZINT Zos_DbufPreAddStrD(ZDBUF zBufId, ZCHAR *pcData);

/**
 * @brief Add data of data buffer just before the beginning of another.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] zData Source buffer the data read from.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufCat
 */
ZFUNC ZINT Zos_DbufPreAddBufD(ZDBUF zBuf, ZDBUF zData);

/**
 * @brief Add format string just before the beginning.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] pcFormat String format, refer Zos_Printf.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPreAddStr
 */
ZFUNC ZINT Zos_DbufPreAddFmtD(ZDBUF zBuf, 
                const ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief Add 1 byte data after the end.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] ucData Data value.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPstAddStr Zos_DbufPstAddMultD
 */
ZFUNC ZINT Zos_DbufPstAddD(ZDBUF zBuf, ZUCHAR ucData);

/**
 * @brief Add specific length data after the end.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] pucData Data address.
 * @param [in] iLen Data Length in bytes.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPreAddD Zos_DbufPreAddStr
 */
ZFUNC ZINT Zos_DbufPstAddMultD(ZDBUF zBuf, ZUCHAR *pucData, 
                ZUINT iLen);

/**
 * @brief Add a string data after the end.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] pcData String data address.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPreAddD Zos_DbufPreAddStr
 */
ZFUNC ZINT Zos_DbufPstAddStrD(ZDBUF zBuf, ZCHAR *pcData);

/**
 * @brief Add data of data buffer after the end of another.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] zData Source buffer the data read from.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufCat
 */
ZFUNC ZINT Zos_DbufPstAddBufD(ZDBUF zBuf, ZDBUF zData);

/**
 * @brief Add format string after the end.
 *
 * @param [in] zBuf Destination buffer the data add to.
 * @param [in] pcFormat String format, refer Zos_Printf.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see @ref Zos_DbufPreAddStr
 */
ZFUNC ZINT Zos_DbufPstAddFmtD(ZDBUF zBuf, 
                const ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief Delete specific length of data from the beginning.
 *
 * @param [in] zBuf Data buffer to delete.
 * @param [in] iLen Length to delete.
 * @param [out] pucData The data deleted.
 *
 * @retval ZOK Delete data successfully.
 * @retval ZFAILED Delete data failed.
 *
 * @see @ref Zos_DbufPreDelD
 */
ZFUNC ZINT Zos_DbufPreDelMultD(ZDBUF zBuf, ZUINT iLen, 
                ZUCHAR *pucData);

/**
 * @brief Delete specific length of data at the end.
 *
 * @param [in] zBuf Data buffer to delete.
 * @param [in] iLen Length to delete.
 * @param [out] pucData The data deleted.
 *
 * @retval ZOK Delete data successfully.
 * @retval ZFAILED Delete data failed.
 *
 * @see @ref Zos_DbufPstDelD
 */
ZFUNC ZINT Zos_DbufPstDelMultD(ZDBUF zBuf, ZUINT iLen, 
                ZUCHAR *pucData);

/**
 * @brief Print out the data to the specific log.
 *
 * @param [in] zBuf Data buffer to print.
 * @param [in] pcLogName Log manager name.
 * @param [in] bTxtMode If ZTRUE, format output as text, or format as binary.
 * @param [in] bPrintLoginLog If ZTRUE, print login log to a file or not.
 *
 * @retval ZOK Print data successfully.
 * @retval ZFAILED Print data failed.
 *
 * @see @ref Zos_DbufLoadFile
 */
ZFUNC ZINT Zos_DbufLogPrint(ZDBUF zBuf, ZCHAR *pcLogName, 
                ZBOOL bTxtMode, ZBOOL bPrintLoginLog);

/**
 * @brief Create new data buffer, and load data from a file.
 *
 * @param [in] pcFileName Name of the file load date from.
 * @param [out] pzBuf New create data buffer.
 *
 * @retval ZOK Create data buffer successfully.
 * @retval ZFAILED Create data buffer failed.
 *
 * @see @ref Zos_DbufLoadFileX
 */
ZFUNC ZINT Zos_DbufLoadFile(ZCONST ZCHAR *pcFileName, ZDBUF *pzBuf);

/**
 * @brief Create new data buffer, and load data from a file 
 *        with file handle.
 *
 * @param [in] zFile Handle of the file load date from.
 * @param [out] pzBuf New create data buffer.
 *
 * @retval ZOK Create data buffer successfully.
 * @retval ZFAILED Create data buffer failed.
 *
 * @see @ref Zos_DbufLoadFile
 */
ZFUNC ZINT Zos_DbufLoadFileX(ZFILE zFile, ZDBUF *pzBuf);

/**
 * @brief Save data to a file.
 *
 * @param [in] zBuf Data buffer read data from.
 * @param [in] pcFileName Name of the file save date to.
 * @param [in] bCipher The file need cipher flag.
 *
 * @retval ZOK Save successfully.
 * @retval ZFAILED Save failed.
 *
 * @see @ref Zos_DbufLoadFileX
 */
ZFUNC ZINT Zos_DbufSaveFile(ZDBUF zBuf, ZCONST ZCHAR *pcFileName, 
                ZBOOL bCipher);

/**
 * @brief Save data to a file.
 *
 * @param [in] zBuf Data buffer read data from.
 * @param [in] zFile Handle of the file save date to.
 *
 * @retval ZOK Save successfully.
 * @retval ZFAILED Save data buffer failed.
 *
 * @see @ref Zos_DbufLoadFileX
 */
ZFUNC ZINT Zos_DbufSaveFileX(ZDBUF zBuf, ZFILE zFile);

/**
 * @brief Create dump of data buffer. User can stack life time trace 
 *        information of data buffer in this dump.
 *
 * @param [in] zBuf Data buffer in which created dump stack.
 * @param [in] pcName The name of dump.
 * @param [in] wDeep The max count of information store in this dump.
 * @param [in] pcFileName The source file name invoke this interfaces.
 * @param [in] iLineNo The source file line number invoke this interfaces.
 *
 * @retval ZOK Create dump stack successfully.
 * @retval ZFAILED Create dump stack failed.
 *
 * @see @ref Zos_DbufDumpDelete
 */
ZFUNC ZINT Zos_DbufDumpCreate(ZDBUF zBuf, ZCHAR *pcName, ZUSHORT wDeep,
                ZCHAR *pcFileName, ZUINT iLineNo);

/**
 * @brief Delete dump of data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @retval ZOK Delete dump stack successfully.
 * @retval ZFAILED Delete dump stack failed.
 *
 * @see @ref Zos_DbufDumpCreate
 */
ZFUNC ZINT Zos_DbufDumpDelete(ZDBUF zBuf);

/**
 * @brief Push dump stack of data buffer.
 *
 * @param [in] zBuf Data buffer id.
 * @param [in] pcFileName The stack file name.
 * @param [in] iLineNo The file line no.
 * @param [in] ucDumpType The dump type, user self definition.
 *
 * @retval ZOK Delete dump stack successfully.
 * @retval ZFAILED Delete dump stack failed.
 *
 * @retval ZOK Push dump stack successfully.
 * @retval ZFAILED Push dump stack failed.
 *
 * @see @ref Zos_DbufDumpPrint
 */
ZFUNC ZINT Zos_DbufDumpStack(ZDBUF zBuf, ZCHAR *pcFileName, 
                ZUINT iLineNo, ZUCHAR ucDumpType);

/**
 * @brief Print dump stacks of data buffer.
 *
 * @param [in] zBuf Data buffer id.
 *
 * @retval ZOK Print dump stack successfully.
 * @retval ZFAILED Print dump stack failed.
 *
 * @see @ref Zos_DbufDumpStack
 */
ZFUNC ZINT Zos_DbufDumpPrint(ZDBUF zBuf);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_DBUF_H__ */

