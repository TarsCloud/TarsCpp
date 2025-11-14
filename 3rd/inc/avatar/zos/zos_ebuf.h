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
  File name     : zos_ebuf.h
  Module        : zos encode buffer library functions
  Author        : Leo Lv   
  Version       : 0.1   
  Created on    : 2007-01-16   
  Description   :
      Data structure and function definitions required 
      by the encode buffer interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_EBUF_H__
#define _ZOS_EBUF_H__

/** 
 * @file zos_ebuf.h
 * @brief Zos encode buffer functions.
 *
 * Encode buffer is wrapper of data buffer. It only provide interface
 * for add data.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new encode buffer.
 *
 * @param [in] iPageSize Size request per time.
 *
 * @return New data buffer when successfully, or return ZNULL.
 *
 * @note This function also create a new data buffer. To create encode buffer
 *       using exist data buffer, refer Zos_EbufCreateX.
 *      
 * @see Zos_EbufCreateX
 */
ZFUNC ZEBUF Zos_EbufCreate(ZUINT iPageSize);

/**
 * @brief Create a new encode buffer using exist data buffer.
 *
 * @param [in] zMsgBuf Exist message buffer used.
 *
 * @return New data buffer when successfully, or return ZNULL.
 *
 * @note This function do not create a new data buffer, while it use the exist
 *       data buffer instead.
 *      
 * @see Zos_EbufCreateX
 */
ZFUNC ZEBUF Zos_EbufCreateX(ZDBUF zMsgBuf);

/**
 * @brief Delete encode buffer.
 *
 * @param [in] zBuf Encode buffer to delete.
 *
 * @return [none].
 *
 * @note This function will always delete the data buffer it used. If you do not
 *       want to delete the data buffer, you should call Zos_EbufDetach to detach
 *       the using data buffer before delete.
 *
 * @see Zos_EbufDetach
 */
ZFUNC ZVOID Zos_EbufDelete(ZEBUF zBuf);

/**
 * @brief Detach the using data buffer from encode buffer.
 *
 * @param [in] zBuf Encode buffer.
 * @param [out] pzMsgBuf The using data buffer.
 *
 * @retval ZOK Detach successfully.
 * @retval ZFAILED Detach failed.
 *
 * @see Zos_EbufAttach
 */
ZFUNC ZINT Zos_EbufDetach(ZEBUF zBuf, ZDBUF *pzMsgBuf);

/**
 * @brief Attach exist data buffer to the encode buffer.
 *
 * @param [in] zBuf Encode buffer.
 * @param [in] zMsgBuf Exist data buffer.
 *
 * @retval ZOK Attach successfully.
 * @retval ZFAILED Attach failed.
 *
 * @see Zos_EbufDetach
 */
ZFUNC ZINT Zos_EbufAttach(ZEBUF zBuf, ZDBUF zMsgBuf);

/**
 * @brief Cut down the unused size.
 *
 * @param [in] zBuf Encode buffer.
 *
 * @return [none].
 *
 * @note After call this function, the value return from Zos_EbufLen will equal
 *       to the used size in encode buffer.
 *      
 * @see Zos_EbufLen
 */
ZFUNC ZVOID Zos_EbufClip(ZEBUF zBuf);

/**
 * @brief Add 1 byte data to the end.
 *
 * @param [in] zBuf Encode buffer.
 * @param [in] ucData Data value.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see Zos_EbufAddStr
 */
ZFUNC ZINT Zos_EbufAddChr(ZEBUF zBuf, ZUCHAR ucData);

/**
 * @brief Add specific length data to the end.
 *
 * @param [in] zBuf Encode buffer.
 * @param [in] pucStr Data address.
 * @param [in] iLen Data length.
 *
 * @retval ZOK Add data successfully.
 * @retval ZFAILED Add data failed.
 *
 * @see Zos_EbufAddStr
 */
ZFUNC ZINT Zos_EbufAddStr(ZEBUF zBuf, ZUCHAR *pucStr, ZUINT iLen);

/**
 * @brief Get the data size.
 *
 * @param [in] zBuf Encode buffer.
 *
 * @return The data size.
 *
 * @note You should always call Zos_EbufClip before this function to get 
 *       the actural data length.
 *      
 * @see Zos_EbufClip
 */
ZFUNC ZUINT Zos_EbufLen(ZEBUF zBuf);

/**
 * @brief Get the total data block size using in data buffer.
 *
 * @param [in] zBuf Encode buffer.
 *
 * @return The total data block size, refer Zos_DbufSize.
 *
 * @see Zos_DbufSize
 */
ZFUNC ZUINT Zos_EbufSize(ZEBUF zBuf);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_EBUF_H__ */

