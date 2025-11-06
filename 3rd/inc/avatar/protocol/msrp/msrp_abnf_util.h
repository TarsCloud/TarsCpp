/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : msrp_abnf_util.h
  Module        : msrp protocol
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Marcos and structure definitions required by the msrp protocol.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_ABNF_UTIL_H__
#define _MSRP_ABNF_UTIL_H__
    
/** 
 * @file msrp_abnf_util.h
 * @brief MSRP ABNF utility interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup group_func_util MSRP Message Utilities
 * @{
 */
    
/**
 * @brief Create a MSRP message header.
 *
 * @param [in] pstMsg MSRP message.
 * @param [in] ucType Type of message header, @ref EN_MSRP_HDR.
 *
 * @return New created header address.
 */
ZFUNC ZVOID * Msrp_CreateMsgHdr(ST_MSRP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Remove a MSRP message header.
 *
 * @param [in] pstMsg MSRP message.
 * @param [in] pHdr MSRP message header.
 *
 * @retval ZOK Remove header successfully.
 * @retval ZFAILED Remove header failed.
 */
ZFUNC ZINT Msrp_RemoveMsgHdr(ST_MSRP_MSG *pstMsg, ZVOID *pHdr);

/**
 * @brief Fill a MSRP message header of specific type.
 *
 * @param [in] pstMsg MSRP message.
 * @param [in] ucType Type of MSRP message header, @ref EN_MSRP_HDR.
 *
 * @retval MSRP message header successfully, otherwise return ZNULL.
 */
ZFUNC ZVOID * Msrp_FindMsgHdr(ST_MSRP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Find MSRP message header value string of specific type. 
 *   If there are more than one, return the first one.
 *
 * @param [in] pstMsg MSRP message.
 * @param [in] ucType Type of MSRP message header, @ref EN_MSRP_HDR.
 * @param [out] ppstVal Header value string(except extension header).
 *
 * @retval ZOK Find header value successfully.
 * @retval ZFAILED Find header value failed.
 */
ZFUNC ZINT Msrp_FindMsgHdrVal(ST_MSRP_MSG *pstMsg, ZUCHAR ucType, 
                ST_ZOS_SSTR **ppstVal);

/**
 * @brief Create new header and add to header list.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstHdrLst Header list of MSRP message.
 * @param [in] ucType Header type, @ref EN_MSRP_HDR.
 *
 * @return New created header address.
 */
ZFUNC ZVOID * Msrp_HdrLstCreateHdr(ZUBUF zBufId, 
                ST_MSRP_HDR_LST *pstHdrLst, ZUCHAR ucType);

/**
 * @brief Remove header from header list.
 *
 * @param [in] pstHdrLst Header list of MSRP message.
 * @param [in] pHdr Header data to remove.
 *
 * @retval ZOK Remove header successfully.
 * @retval ZFAILED Remove header failed.
 */
ZFUNC ZINT Msrp_HdrLstRemoveHdr(ST_MSRP_HDR_LST *pstHdrLst, 
                ZVOID *pHdr);

/**
 * @brief Get data length of MSRP message data according to Byte-Range header .
 *
 * @param [in] pstData MSRP message data includes all message headers.
 * @param [out] pdwDataLen MSPR message data(body) length.
 * @param [out] pdwMaxDataLen MSPR message data(body) maximum length.
 *
 * @retval ZOK Get message data length successfully, otherwise return ZFAILED.
 */
ZFUNC ZINT Msrp_MsgHdrGetDataLen(ST_ZOS_SSTR *pstData, 
                ZUINT *piDataLen, ZUINT *piMaxDataLen);

/**
 * @brief Get data length of MSRP body data according to message boundary .
 *
 * @param [in] pstBody MSRP message data includes body data.
 * @param [out] pstTransId MSPR message transaction id.
 * @param [out] pdwDataLen MSPR message data(body) length.
 * @param [out] pdwEndlineLen MSPR message the end line length.
 *
 * @retval ZOK Get message data length successfully, otherwise return ZFAILED.
 */
ZFUNC ZINT Msrp_MsgBodyGetDataLen(ST_ZOS_SSTR *pstBody, 
                ST_ZOS_SSTR *pstTransId, ZUINT *piDataLen, 
                ZUINT *piEndlineLen);

/**
 * @brief Find the 7 minus from data string.
 *
 * @param [in] pstData An arbitrary string which maybe contain "-------" string.
 *
 * @retval ZTRUE while string has 7 minus sub-string, otherwise return ZFALSE.
 */
ZFUNC ZBOOL Msrp_MsgBodyFind7Minus(ST_ZOS_SSTR *pstData);

/**
 * @brief Get mime boundary from content type.
 *
 * @param [in] pstContType The content type header.
 * @param [in] ppstBoundary The message body MIME boundary.
 *
 * @retval ZOK Get MIME body boundary successfully, otherwise return ZFAILED.
 */
ZFUNC ZINT Msrp_GetMimeBoundary(ST_MSRP_HDR_CONT_TYPE *pstContType,
                ST_ZOS_SSTR **ppstBoundary);
/** @} */

#ifdef __cplusplus
}
#endif
    
#endif /* _MSRP_ABNF_UTIL_H__ */

