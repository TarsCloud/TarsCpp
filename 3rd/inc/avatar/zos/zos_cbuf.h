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
  File name     : zos_cbuf.h
  Module        : zos convergency buffer functions 
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-12-14
  Description   :
    Data structure and function definitions required by the convergency buffer.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_CBUF_H__
#define _ZOS_CBUF_H__

/** 
 * @file zos_cbuf.h
 * @brief Zos convergency buffer functions.
 *
 * Convergency buffer (cbuf) provides ability like aggregation buffer (abuf).
 * The diffence is cbuf using bitmap to manage its memory, which can reduce
 * the cost of size for management.
 *
 * When create cbuf, you should assign a default block size. This value do not
 * limit the allocate size. But when allocate for default size data,
 * it will be more quickly.
 * 
 * Convergency buffer is not thread safe.
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef ZOS_SUPT_DBG

/* zos cbuf create */
#define ZOS_CBUF_CREATE(_buf, _bktsize) do { \
    _buf = Zos_CbufCreate(_bktsize); \
    if (_buf) Zos_TraceMemAdd(_buf, "CBUF_CREATE", __FILE__, __LINE__); \
} while (0)

/* zos cbuf create and allocate a data block */
#define ZOS_CBUF_CREATED(_buf, _bktsize, _datasize, _data) do { \
    _buf = Zos_CbufCreateD(_bktsize, _datasize, _data); \
    if (_buf) Zos_TraceMemAdd(_buf, "CBUF_CREATED", __FILE__, __LINE__); \
} while (0)

/* zos cbuf create and allocate a data block, clear data by 0 */
#define ZOS_CBUF_CREATE_CLRD(_buf, _bktsize, _datasize, _data) do { \
    _buf = Zos_CbufCreateClrd(_bktsize, _datasize, _data); \
    if (_buf) Zos_TraceMemAdd(_buf, "CBUF_CREATE_CLRD", __FILE__, __LINE__); \
} while (0)

/* zos cbuf create from a parent buffer */
#define ZOS_CBUF_CREATEX(_buf, _pbuf, _bktsize) do { \
    _buf = Zos_CbufCreateX(_pbuf, _bktsize); \
    if (_buf) Zos_TraceMemAdd(_buf, "CBUF_CREATEX", __FILE__, __LINE__); \
} while (0)

/* zos cbuf create from a parent buffer and allocate a data block */
#define ZOS_CBUF_CREATEXD(_buf, _pbuf, _bktsize, _datasize, _data) do { \
    _buf = Zos_CbufCreateXD(_pbuf, _bktsize, _datasize, _data); \
    if (_buf) Zos_TraceMemAdd(_buf, "CBUF_CREATEXD", __FILE__, __LINE__); \
} while (0)

/* zos cbuf create from a parent buffer, allocate a data block and clear by 0 */
#define ZOS_CBUF_CREATEX_CLRD(_buf, _pbuf, _bktsize, _datasize, _data) do { \
    _buf = Zos_CbufCreateXClrd(_pbuf, _bktsize, _datasize, _data); \
    if (_buf) Zos_TraceMemAdd(_buf, "CBUF_CREATEX_CLRD", __FILE__, __LINE__); \
} while (0)

/* zos cbuf delete */
#define Zos_CBUF_DELETE(_buf) do { \
    if (_buf) Zos_TraceMemRmv(_buf); \
    Zos_CbufDelete(_buf); \
} while (0)

/* zos cbuf free memory */
#define ZOS_CBUF_FREE(_buf, _mem) do { \
    if (!_buf) \
        Zos_TraceLog("CbufFree null buf.", __FILE__, __LINE__); \
    else if (Zos_CbufFree(_buf, _mem) != ZOK) \
        Zos_TraceLog("CbufFree invalid buf.", __FILE__, __LINE__); \
} while (0)

#else /* no debug */

/* zos cbuf create */
#define ZOS_CBUF_CREATE(_buf, _bktsize) \
    _buf = Zos_CbufCreate(_bktsize)

/* zos cbuf create and allocate a data block */
#define ZOS_CBUF_CREATED(_buf, _bktsize, _datasize, _data) \
    _buf = Zos_CbufCreateD(_bktsize, _datasize, _data)

/* zos cbuf create and allocate a data block, clear data by 0 */
#define ZOS_CBUF_CREATE_CLRD(_buf, _bktsize, _datasize, _data) \
    _buf = Zos_CbufCreateClrd(_bktsize, _datasize, _data)

/* zos cbuf create from a parent buffer */
#define ZOS_CBUF_CREATEX(_buf, _pbuf, _bktsize) \
    _buf = Zos_CbufCreateX(_pbuf, _bktsize)

/* zos cbuf create from a parent buffer and allocate a data block */
#define ZOS_CBUF_CREATEXD(_buf, _pbuf, _bktsize, _datasize, _data) \
    _buf = Zos_CbufCreateXD(_pbuf, _bktsize, _datasize, _data)

/* zos cbuf create from a parent buffer, allocate a data block and clear by 0 */
#define ZOS_CBUF_CREATEX_CLRD(_buf, _pbuf, _bktsize, _datasize, _data) \
    _buf = Zos_CbufCreateXClrd(_pbuf, _bktsize, _datasize, _data)

/* zos cbuf delete */
#define Zos_CBUF_DELETE(_buf) \
    Zos_CbufDelete(_buf)

/* zos cbuf free memory */
#define ZOS_CBUF_FREE(_buf, _mem) \
    Zos_CbufFree(_buf, _mem)

#endif /* ZOS_SUPT_DBG */

/**
 * @brief Create a new cbuf.
 *
 * @param [in] iBktSize Default data block size, from 32 - 4k. If request size
 *                      is smaller than 32 bytes, using 32 for default.
 *                      Or larger than 4k bytes, using 4k.
 *
 * @return New cbuf ID, or return ZNULL.
 *
 * @see Zos_CbufCreateX
 */
ZFUNC ZCBUF Zos_CbufCreate(ZUINT iBktSize);

/**
 * @brief Create a new cbuf, and allocate a data block.
 *
 * @param [in] iBktSize Default data block size, from 32 - 4k. If request size
 *                      is smaller than 32 bytes, using 32 for default.
 *                      Or larger than 4k bytes, using 4k.
 * @param [in] zSize The size of data to get.
 * @param [out] ppData The data allocated.
 *
 * @return New cbuf ID, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_CbufCreate create new buffer.
 *       -# Call Zos_CbufAlloc allocate data.
 *
 * @see Zos_CbufCreate Zos_CbufAlloc
 */
ZFUNC ZCBUF Zos_CbufCreateD(ZUINT iBktSize, ZUINT iSize, 
                ZVOID **ppData);

/**
 * @brief Create a new cbuf, and allocate a data block cleared by 0.
 *
 * @param [in] iBktSize Default data block size, from 32 - 4k. If request size
 *                      is smaller than 32 bytes, using 32 for default.
 *                      Or larger than 4k bytes, using 4k.
 * @param [in] zSize The size of data to get.
 * @param [out] ppData The data allocated.
 *
 * @return New cbuf ID, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_CbufCreate create new buffer.
 *       -# Call Zos_CbufAllocClrd allocate data cleared by 0.
 *
 * @see Zos_CbufCreate Zos_CbufAllocClrd
 */
ZFUNC ZCBUF Zos_CbufCreateClrd(ZUINT iBktSize, ZUINT iSize, 
                ZVOID **ppData);

/**
 * @brief Create a new cbuf, and attach to another cbuf as its child.
 *
 * @param [in] zBuf Parent cbuf attach to.
 * @param [in] iBktSize Default data block size, from 32 - 4k. If request size
 *                      is smaller than 32 bytes, using 32 for default.
 *                      Or larger than 4k bytes, using 4k.
 *
 * @return New cbuf ID, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_CbufCreate create new buffer.
 *       -# Call Zos_CbufAttach attach new buffer to another.
 *
 * @see Zos_CbufCreate Zos_CbufAttach
 */
ZFUNC ZCBUF Zos_CbufCreateX(ZCBUF zBuf, ZUINT iBktSize);

/**
 * @brief Create a new cbuf, attach to another cbuf as its child,
 *        and allocate data from it.
 *
 * @param [in] zBuf Parent cbuf attach to.
 * @param [in] iBktSize Default data block size, from 32 - 4k. If request size
 *                      is smaller than 32 bytes, using 32 for default.
 *                      Or larger than 4k bytes, using 4k.
 * @param [in] iSize The size of data to get.
 * @param [out] ppData The data allocated.
 *
 * @return New cbuf ID, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_CbufCreateX create new buffer.
 *       -# Call Zos_CbufAlloc allocate data.
 *
 * @see Zos_CbufCreateX Zos_CbufAlloc
 */
ZFUNC ZCBUF Zos_CbufCreateXD(ZCBUF zBuf, ZUINT iBktSize, 
                ZUINT iSize, ZVOID **ppData);

/**
 * @brief Create a new cbuf, attach to another cbuf as its child,
 *        and allocate data cleared by 0.
 *
 * @param [in] zBuf Parent cbuf attach to.
 * @param [in] iBktSize Default data block size, from 32 - 4k. If request size
 *                      is smaller than 32 bytes, using 32 for default.
 *                      Or larger than 4k bytes, using 4k.
 * @param [in] iSize The size of data to get.
 * @param [out] ppData The data allocated cleared by 0.
 *
 * @return New cbuf ID, or return ZNULL.
 *
 * @note This function is shortcut of:\n
 *       -# Call Zos_CbufCreateX create new buffer.
 *       -# Call Zos_CbufAllocClrd allocate data cleared by 0.
 *
 * @see Zos_CbufCreateX Zos_CbufAllocClrd
 */
ZFUNC ZCBUF Zos_CbufCreateXClrd(ZCBUF zBuf, ZUINT iBktSize, 
                ZUINT iSize, ZVOID **ppData);

/**
 * @brief Clone a cbuf.
 *
 * @param [in] zBuf Cbuf to clone.
 *
 * @return Cbuf ID cloned, or return ZNULL.
 *
 * @see Zos_CbufCreate
 */

ZFUNC ZCBUF Zos_CbufClone(ZCBUF zBuf);

/**
 * @brief Delete a cbuf, all memory allocate from it will also be freeed,
 *        and all descendants cbuf will be deleted.
 *
 * @param [in] zBuf Cbuf to delete.
 *
 * @return [none].
 *
 * @see Zos_CbufClean
 */
ZFUNC ZVOID Zos_CbufDelete(ZCBUF zBuf);

/**
 * @brief Free all memory allocated from it.
 *
 * @param [in] zBuf Cbuf to free.
 *
 * @return [none].
 *
 * @see Zos_CbufDelete
 */
ZFUNC ZVOID Zos_CbufClean(ZCBUF zBuf);

/**
 * @brief Attach cbuf to another cbuf as its child.
 *
 * @param [in] zDstBuf Parent cbuf ID.
 * @param [in] zSrcBuf Child cbuf ID.
 *
 * @retval ZOK Attach successfully.
 * @retval ZFAILED Attach failed.
 *
 * @see Zos_CbufAttach
 */
ZFUNC ZINT Zos_CbufAttach(ZCBUF zDstBuf, ZCBUF zSrcBuf);

/**
 * @brief Detach cbuf from its parent cbuf.
 *
 * @param [in] zBuf Cbuf ID.
 *
 * @retval ZOK Detach successfully.
 * @retval ZFAILED Detach failed.
 *
 * @see Zos_CbufAttach
 */
ZFUNC ZINT Zos_CbufDetach(ZCBUF zBuf);

/**
 * @brief Allocate memory from cbuf.
 *
 * @param [in] zBuf Cbuf ID.
 * @param [in] iSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zos_CbufAllocClrd
 */
ZFUNC ZVOID * Zos_CbufAlloc(ZCBUF zBuf, ZUINT iSize);

/**
 * @brief Allocate memory cleared by 0 from cbuf.
 *
 * @param [in] zBuf Cbuf ID.
 * @param [in] iSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zos_CbufAllocClrd
 */
ZFUNC ZVOID * Zos_CbufAllocClrd(ZCBUF zBuf, ZUINT iSize);

/**
 * @brief Free memory allocated.
 *
 * @param [in] zBuf Cbuf ID.
 * @param [in] pMem Memory address.
 *
 * @retval ZOK Free successfully.
 * @retval ZFAILED Free failed.
 *
 * @see Zos_CbufAlloc
 */
ZFUNC ZINT Zos_CbufFree(ZCBUF zBuf, ZVOID *pMem);

/**
 * @brief Get the size used in cbuf.
 *
 * @param [in] zBuf Cbuf ID.
 *
 * @return Size used in cbuf, not include descendants cbuf.
 *
 * @see
 */
ZFUNC ZUINT Zos_CbufSize(ZCBUF zBuf);

/**
 * @brief Check if the memory address is in this cbuf.
 *
 * @param [in] zBuf Cbuf ID.
 * @param [in] pData Memory address to check.
 *
 * @retval ZTRUE Memory address located in the memory allocated from this cbuf.
 * @retval ZFALSE Otherwise.
 *
 * @see
 */
ZFUNC ZBOOL Zos_CbufHoldD(ZCBUF zBuf, ZVOID *pData);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_CBUF_H__ */

