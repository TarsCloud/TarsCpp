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
  File name     : zos_ubuf.h
  Module        : zos unify buffer functions 
  Author        : zocol
  Version       : 0.1
  Created on    : 2009-12-10
  Description   :
    Data structure and function definitions required by the unify buffer.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_UBUF_H__
#define _ZOS_UBUF_H__

/** 
 * @file zos_ubuf.h
 * @brief Zos unify buffer functions.
 *
 * Unify buffer (ubuf) provides a series of interfaces to operate various type
 * of buffer, include abuf, bbuf, cbuf, dbuf, sbuf and xbuf. User can directly
 * use other buffer ID as ubuf ID.
 *
 * Unify buffer is not thread safe.
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef ZOS_SUPT_DBG

/* zos ubuf free */
#define ZOS_UBUF_FREE(_buf, _mem) do { \
    if (!_buf) \
        Zos_TraceLog("UbufFree null buf.", __FILE__, __LINE__); \
    else if (Zos_UbufFree(_buf, _mem) != ZOK) \
        Zos_TraceLog("UbufFree invalid buf.", __FILE__, __LINE__); \
} while (0)

/* zos ubuf free and reset memory address to ZNULL */
#define ZOS_UBUF_FREEX(_buf, _mem) do { \
    if (!_buf) \
        Zos_TraceLog("UbufFreeX null buf.", __FILE__, __LINE__); \
    else if (Zos_UbufFreeX(_buf, _mem) != ZOK) \
        Zos_TraceLog("UbufFreeX invalid buf.", __FILE__, __LINE__); \
} while (0)

/* zos ubuf free sstr */
#define ZOS_UBUF_FREE_SSTR(_buf, _sstr) do { \
    if (!_buf) \
        Zos_TraceLog("UbufFreeSStr null buf.", __FILE__, __LINE__); \
    else if (Zos_UbufFreeSStr(_buf, _sstr) != ZOK) \
        Zos_TraceLog("UbufFreeSStr invalid buf.", __FILE__, __LINE__); \
} while (0)

/* zos ubuf free ustr */
#define ZOS_UBUF_FREE_USTR(_buf, _ustr) do { \
    if (!_buf) \
        Zos_TraceLog("UbufFreeUStr null buf.", __FILE__, __LINE__); \
    else if (Zos_UbufFreeUStr(_buf, _ustr) != ZOK) \
        Zos_TraceLog("UbufFreeUStr invalid buf.", __FILE__, __LINE__); \
} while (0)

#else /* no debug */

/* zos ubuf free */
#define ZOS_UBUF_FREE(_buf, _mem) \
    Zos_UbufFree(_buf, _mem)

/* zos ubuf free x */
#define ZOS_UBUF_FREEX(_buf, _mem) \
    Zos_UbufFreeX(_buf, _mem)

/* zos ubuf free sstr */
#define ZOS_UBUF_FREE_SSTR(_buf, _sstr) \
    Zos_UbufFreeSStr(_buf, _sstr)

/* zos ubuf free ustr */
#define ZOS_UBUF_FREE_USTR(_buf, _ustr) \
    Zos_UbufFreeUStr(_buf, _sstr)

#endif /* ZOS_SUPT_DBG */

/**
 * @brief Create a new ubuf with same type of specific buffer.
 *
 * @param [in] pBufId Specific buffer ID.
 *
 * @return New ubuf ID if successfully, or return ZNULL.
 *
 * @see @ref Zos_UbufCreateX
 */
ZFUNC ZUBUF Zos_UbufCreate(ZVOID *pBufId);

/**
 * @brief Delete ubuf, related buffer will not be deleted.
 *
 * @param [in] zBufId Ubuf ID.
 *
 * @return [none].
 *
 * @see @ref Zos_UbufCreate
 */
ZFUNC ZVOID Zos_UbufDelete(ZUBUF zBufId);

/**
 * @brief Allocate a data block from ubuf.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] iSize Data size request.
 *
 * @return Data block address if successfully, or return ZNULL.
 *
 * @see @ref Zos_UbufAllocClrd
 */
ZFUNC ZVOID * Zos_UbufAlloc(ZUBUF zBufId, ZUINT iSize);

/**
 * @brief Allocate a data block from ubuf cleared by 0.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] iSize Data size request.
 *
 * @return Data block address if successfully, or return ZNULL.
 *
 * @see @ref Zos_UbufAlloc
 */
ZFUNC ZVOID * Zos_UbufAllocClrd(ZUBUF zBufId, ZUINT iSize);

/**
 * @brief Allocate a data block with extra space for ST_ZOS_SLIST_NODE 
 *        from ubuf.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] iDataSize Data size request.
 * @param [out] ppData Memory address of data.
 *
 * @return Memory add of data node if successfully, or return ZNULL.
 *
 * @see @ref Zos_UbufAllocDNode
 */
ZFUNC ZVOID * Zos_UbufAllocSNode(ZUBUF zBufId, ZUINT iDataSize,
                ZVOID **ppData);

/**
 * @brief Allocate a data block with extra space for ST_ZOS_DLIST_NODE 
 *        from ubuf.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] iDataSize Data size request.
 * @param [out] ppData Memory address of data.
 *
 * @return Memory add of data node if successfully, or return ZNULL.
 *
 * @see @ref Zos_UbufAllocSNode
 */
ZFUNC ZVOID * Zos_UbufAllocDNode(ZUBUF zBufId, ZUINT iDataSize,
                ZVOID **ppData);

/**
 * @brief Free a data allocated.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] pMem Memory address of data.
 *
 * @retval ZOK Free successfully.
 * @retval ZFAILED Free failed.
 *
 * @see @ref Zos_UbufFreeX
 */
ZFUNC ZINT Zos_UbufFree(ZUBUF zBufId, ZVOID *pMem);

/**
 * @brief Free a data and its pointer allocated.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] ppMem Memory address pointer of data.
 *
 * @retval ZOK Free successfully.
 * @retval ZFAILED Free failed.
 *
 * @see @ref Zos_UbufFree
 */
ZFUNC ZINT Zos_UbufFreeX(ZUBUF zBufId, ZVOID **ppMem);

/**
 * @brief Free ST_ZOS_SSTR string allocated.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] pstStr String memory.
 *
 * @retval ZOK Free successfully.
 * @retval ZFAILED Free failed.
 *
 * @see @ref Zos_UbufFree
 */
ZFUNC ZINT Zos_UbufFreeSStr(ZUBUF zBufId, ST_ZOS_SSTR *pstStr);

/**
 * @brief Free ST_ZOS_USTR string allocated.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] pstStr String value.
 *
 * @retval ZOK Free successfully.
 * @retval ZFAILED Free failed.
 *
 * @see @ref Zos_UbufFree
 */
ZFUNC ZINT Zos_UbufFreeUStr(ZUBUF zBufId, ST_ZOS_USTR *pstStr);

/**
 * @brief Get the size used in ubuf.
 *
 * @param [in] zBufId Ubuf ID.
 *
 * @return Size used in ubuf, not include descendants ubuf.
 */
ZFUNC ZUINT Zos_UbufSize(ZUBUF zBufId);

/**
 * @brief Check if the memory address is in this ubuf.
 *
 * @param [in] zBufId Ubuf ID.
 * @param [in] pData Memory address to check.
 *
 * @retval ZTRUE Memory address located in the memory allocated from this ubuf.
 * @retval ZFALSE Otherwise.
 */
ZFUNC ZBOOL Zos_UbufHoldD(ZUBUF zBufId, ZVOID *pData);

/**
 * @brief Copy ZCHAR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [out] ppcDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNStr 
 */
ZFUNC ZINT Zos_UbufCpyStr(ZUBUF zBufId, ZCONST ZCHAR *pcSrcStr, 
                ZCHAR **ppcDstStr); 

/**
 * @brief Copy ZCHAR string with length using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [in] iSrcLen Source string length.
 * @param [out] ppcDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyStr 
 */
ZFUNC ZINT Zos_UbufCpyNStr(ZUBUF zBufId, ZCONST ZCHAR *pcSrcStr, 
                ZUINT iSrcLen, ZCHAR **ppcDstStr); 

/**
 * @brief Copy ZCHAR string with long length using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [in] iSrcLen Source string length.
 * @param [out] ppcDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyStr 
 */
ZFUNC ZINT Zos_UbufCpyNLStr(ZUBUF zBufId, ZCHAR *pcSrcStr, 
                ZUINT iSrcLen, ZCHAR **ppcDstStr); 

/**
 * @brief Copy ZUCHAR string with long length using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [in] iSrcLen Source string length.
 * @param [out] ppcDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyStr 
 */
ZFUNC ZINT Zos_UbufCpyNLUSStr(ZUBUF zBufId, ZUCHAR *pcSrcStr, 
                ZUINT iSrcLen, ZUCHAR **ppcDstStr);

/**
 * @brief Copy ST_ZOS_SSTR string to ZCHAR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source string read from.
 * @param [out] ppcDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyUXStr 
 */
ZFUNC ZINT Zos_UbufCpyXStr(ZUBUF zBufId, ST_ZOS_SSTR *pstSrcStr, 
                ZCHAR **ppcDstStr); 

/**
 * @brief Copy ST_ZOS_USTR string to ZCHAR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source string read from.
 * @param [out] ppcDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyXStr 
 */
ZFUNC ZINT Zos_UbufCpyUXStr(ZUBUF zBufId, ST_ZOS_USTR *pstSrcStr, 
                ZCHAR **ppcDstStr); 

/**
 * @brief Copy data buffer to ZCHAR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] zData Source data buffer read from.
 * @param [out] ppcDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyDSStr Zos_UbufCpyDUSStr
 */
ZFUNC ZINT Zos_UbufCpyDStr(ZUBUF zBufId, ZDBUF zData, 
                ZCHAR **ppcDstStr); 

/**
 * @brief Copy ZCHAR string to ST_ZOS_SSTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNSStr
 */
ZFUNC ZINT Zos_UbufCpySStr(ZUBUF zBufId, ZCONST ZCHAR *pcSrcStr, 
                ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy ZCHAR string with specific length to ST_ZOS_SSTR string 
 *        using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [in] iSrcLen Copy length.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpySStr
 */
ZFUNC ZINT Zos_UbufCpyNSStr(ZUBUF zBufId, ZCONST ZCHAR *pcSrcStr, 
                ZUINT iSrcLen, ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy ZCHAR string with specific length to ST_ZOS_SSTR string 
 *        using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [in] iSrcLen Copy length.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpySStr
 */
ZFUNC ZINT Zos_UbufCpyLSStr(ZUBUF zBufId, ZCHAR *pcSrcStr, 
                ZUINT iSrcLen, ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy ZUCHAR string with specific long length to ST_ZOS_USTR string 
 *        using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [in] iSrcLen Copy length.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpySStr
 */
ZFUNC ZINT Zos_UbufCpyNLUStr(ZUBUF zBufId, ZUCHAR *pcSrcStr, 
                ZUINT iSrcLen, ST_ZOS_USTR *pstDstStr);

/**
 * @brief Copy ZCHAR string with specific long length to ST_ZOS_SSTR string 
 *        using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pcSrcStr Source string read from.
 * @param [in] iSrcLen Copy length.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpySStr
 */
ZFUNC ZINT Zos_UbufCpyNLSStr(ZUBUF zBufId, ZCHAR *pcSrcStr, 
                ZUINT iSrcLen, ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy ST_ZOS_SSTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source string read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNSStr
 */
ZFUNC ZINT Zos_UbufCpyXSStr(ZUBUF zBufId, ST_ZOS_SSTR *pstSrcStr, 
                ST_ZOS_SSTR *pstDstStr); 
/**
 * @brief Copy ST_ZOS_SSTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source string read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNSStr
 */
ZFUNC ZINT Zos_UbufCpyLXSStr(ZUBUF zBufId, ST_ZOS_SSTR *pstSrcStr, 
                ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy ST_ZOS_USTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source string read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNSStr
 */
ZFUNC ZINT Zos_UbufCpyXLUStr(ZUBUF zBufId, ST_ZOS_USTR *pstSrcStr, 
                ST_ZOS_USTR *pstDstStr);

/**
 * @brief Copy ST_ZOS_SSTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source string read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNSStr
 */
ZFUNC ZINT Zos_UbufCpyXLSStr(ZUBUF zBufId, ST_ZOS_SSTR *pstSrcStr, 
                ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy ST_ZOS_USTR string to ST_ZOS_SSTR string 
 *        using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source string read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyXSStr
 */
ZFUNC ZINT Zos_UbufCpyUXSStr(ZUBUF zBufId, ST_ZOS_USTR *pstSrcStr, 
                ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy data buffer to ST_ZOS_SSTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] zData Source data buffer read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyDStr Zos_UbufCpyDUSStr
 */
ZFUNC ZINT Zos_UbufCpyDSStr(ZUBUF zBufId, ZDBUF zData, 
                ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy data buffer to ST_ZOS_SSTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] zData Source data buffer read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyDSStr
 */
ZFUNC ZINT Zos_UbufCpyDLSStr(ZUBUF zBufId, ZDBUF zData, 
                ST_ZOS_SSTR *pstDstStr); 

/**
 * @brief Copy ZUCHAR string to ST_ZOS_USTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pucSrcStr Source data buffer read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNUSStr
 */
ZFUNC ZINT Zos_UbufCpyUSStr(ZUBUF zBufId, ZUCHAR *pucSrcStr, 
                ST_ZOS_USTR *pstDstStr); 

/**
 * @brief Copy ZUCHAR string with specific length to ST_ZOS_USTR string 
 *        using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pucSrcStr Source data buffer read from.
 * @param [in] iSrcLen Copy length.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyXUSStr
 */
ZFUNC ZINT Zos_UbufCpyNUSStr(ZUBUF zBufId, ZUCHAR *pucSrcStr, 
                ZUINT iSrcLen, ST_ZOS_USTR *pstDstStr); 

/**
 * @brief Copy ST_ZOS_SSTR string to ST_ZOS_USTR string 
 *        using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source data buffer read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNUSStr
 */
ZFUNC ZINT Zos_UbufCpyXUSStr(ZUBUF zBufId, ST_ZOS_SSTR *pstSrcStr, 
                ST_ZOS_USTR *pstDstStr); 

/**
 * @brief Copy ST_ZOS_USTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] pstSrcStr Source data buffer read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyNUSStr
 */
ZFUNC ZINT Zos_UbufCpyUXUSStr(ZUBUF zBufId, ST_ZOS_USTR *pstSrcStr, 
                ST_ZOS_USTR *pstDstStr); 

/**
 * @brief Copy data buffer to ST_ZOS_USTR string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [in] zData Source data buffer read from.
 * @param [out] pstDstStr Destination string copy to.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyDStr @ref Zos_UbufCpyDSStr
 */
ZFUNC ZINT Zos_UbufCpyDUSStr(ZUBUF zBufId, ZDBUF zData, 
                ST_ZOS_USTR *pstDstStr); 

/**
 * @brief Format string using memory from ubuf.
 *
 * @param [in] zBufId Ubuf allocate memory from.
 * @param [out] ppcDstStr Destination string.
 * @param [in] pcFormat String format, refer Zos_Printf.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Copy data failed.
 *
 * @see @ref Zos_UbufCpyDUSStr
 */
ZFUNC ZINT Zos_UbufCpyFStr(ZUBUF zBufId, ZCHAR **ppcDstStr,
                const ZCHAR *pcFormat, ...) ZATTR_PRINTF(3,4);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_UBUF_H__ */

