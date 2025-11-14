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
  File name     : sip_abnf_util.h
  Module        : sip protocol
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-05-91
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_ABNF_UTIL_H__
#define _SIP_ABNF_UTIL_H__

/**
 * @file
 * @brief SIP ABNF utilities interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief sip data buffer size */
#define SIP_MSGBUF_SIZE 1020         /**< @brief Message buffer size in bytes. */
#define SIP_MEMBUF_SIZE 1020         /**< @brief Structure buffer size in bytes. */

/** @brief Create SIP structure buffer. */
#define SIP_CREATE_MEMBUF(_membuf) \
    ZOS_MEMBUF_CREATE(_membuf, SIP_MEMBUF_SIZE, 16, "sip membuf")

/** @brief Create SIP structure buffer and allocate data. */
#define SIP_CREATE_MEMBUFD(_membuf, _size, _data) \
    ZOS_MEMBUF_CREATECLRD(_membuf, SIP_MEMBUF_SIZE, 16, \
                          "sip membuf", _size, _data)

/** @brief Create SIP message buffer. */
#define SIP_CREATE_MSGBUF(_msgbuf) \
    SIP_CREATE_MSGBUFX(_msgbuf, SIP_MSGBUF_SIZE)

/** @brief Create SIP message buffer with specific default size. */
#define SIP_CREATE_MSGBUFX(_msgbuf, _dftsize) \
    ZOS_MSGBUF_CREATE(_msgbuf, _dftsize, 16, "sip msgbuf")

/** @brief Delete SIP buffer. */
#define SIP_DBUF_DELETE(_dbuf) \
    ZOS_DBUF_DELETE(_dbuf)

/** @brief Allocate data from SIP buffer. */
#define SIP_DBUF_ALLOC(_dbuf, _size) \
    Zos_DbufAlloc(_dbuf, _size)

/** @brief Allocate data from SIP buffer, clear by 0. */
#define SIP_DBUF_ALLOC_CLRD(_dbuf, _size) \
    Zos_DbufAllocClrd(_dbuf, _size)

/** @brief Clone SIP buffer. */
#define SIP_DBUF_CLONE1(_dbuf) \
    ZOS_DBUF_CLONE(_dbuf)

/** @brief Clone SIP buffer. */
#define SIP_DBUF_CLONE2(_dbuf, _newbuf) \
    ZOS_DBUF_CLONEX(_dbuf, _newbuf)

/** @brief Typedef of SIP message decode function. */
typedef ZINT (*PFN_SIPANYDECODE)(ST_ABNF_MSG *pstAbnfMsg, ZVOID *pAnyElem); 

/** @brief Typedef of SIP message encode function. */
typedef ZINT (*PFN_SIPANYENCODE)(ST_ABNF_MSG *pstAbnfMsg, ZVOID *pAnyElem); 

/**
 * @brief Create a SIP message header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 *
 * @return New created header address.
 */
ZFUNC ZVOID * Sip_CreateMsgHdr(ST_SIP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Delete a SIP message header, if there are more than one, delete first.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 *
 * @retval ZOK Delete header successfully.
 * @retval ZFAILED Delete header failed.
 */
ZFUNC ZINT Sip_DeleteMsgHdr(ST_SIP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Remove a SIP message header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pHdr SIP message header to remove.
 *
 * @retval ZOK Remove header successfully.
 * @retval ZFAILED Remove header failed.
 */
ZFUNC ZINT Sip_RemoveMsgHdr(ST_SIP_MSG *pstMsg, ZVOID *pHdr);

/**
 * @brief Fill a SIP message header of specific type with string value.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 * @param [in] pstStr Header string value.
 *
 * @retval ZOK Fill header successfully.
 * @retval ZFAILED Fill header failed.
 */
ZFUNC ZINT Sip_FillMsgHdr(ST_SIP_MSG *pstMsg, ZUCHAR ucType, 
                ST_ZOS_SSTR *pstStr);

/**
 * @brief Fill a SIP message header of specific type with string value.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 * @param [in] pcStr Header string value.
 *
 * @retval ZOK Fill header successfully.
 * @retval ZFAILED Fill header failed.
 */
ZFUNC ZINT Sip_FillMsgHdrX(ST_SIP_MSG *pstMsg, ZUCHAR ucType, ZCHAR *pcStr);

/**
 * @brief Fill a SIP message header as extension header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Header name string.
 * @param [in] pcStr Header string value.
 *
 * @retval ZOK Fill header successfully.
 * @retval ZFAILED Fill header failed.
 */
ZFUNC ZINT Sip_FillExtHdr(ST_SIP_MSG *pstMsg, ZCHAR *pcName, ZCHAR *pcStr);

/**
 * @brief Get the header count of specific type.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 *
 * @return The count of specific type header.
 */
ZFUNC ZUINT Sip_GetMsgHdrSize(ST_SIP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Get the header count of specific type.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 *
 * @return The count of specific type header.
 */
ZFUNC ZUINT Sip_GetMsgHdrSizeX(ST_SIP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Find SIP message header of specific type. 
 *   If there are more than one, return the first one.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 *
 * @return SIP message header address if found.
 */
ZFUNC ZVOID * Sip_FindMsgHdr(ST_SIP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Find SIP message header of specific type.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 * @param [in] iIndex Index value in same type headers.
 *
 * @return SIP message header address if found.
 */
ZFUNC ZVOID * Sip_FindMsgHdrX(ST_SIP_MSG *pstMsg, ZUCHAR ucType, 
                ZINT iIndex);

/**
 * @brief Find SIP message header value string of specific type. 
 *   If there are more than one, return the first one.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 * @param [out] ppstVal Header value string(except extension header).
 *
 * @retval ZOK Find header value successfully.
 * @retval ZFAILED Find header value failed.
 */
ZFUNC ZINT Sip_FindMsgHdrVal(ST_SIP_MSG *pstMsg, ZUCHAR ucType, 
                ST_ZOS_SSTR **ppstVal);

/**
 * @brief Find SIP message header value string of specific type. 
 *   If there are more than one, return the first one.
 *  @note It is recommend used in get header value from an incoming SIP message.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName SIP Header name.
 * @param [out] ppstVal Header value string(also include extension header).
 *
 * @retval ZOK Find header value successfully.
 * @retval ZFAILED Find header value failed.
 */
ZFUNC ZINT Sip_FindMsgHdrValX(ST_SIP_MSG *pstMsg, ZCHAR *pcName, 
                ST_ZOS_SSTR **ppstVal);

/**
 * @brief Get the extension header count.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Header name.
 *
 * @return The count of specific type header.
 */
ZFUNC ZINT Sip_GetExtHdrSize(ST_SIP_MSG *pstMsg, ZCHAR *pcName);

/**
 * @brief Find SIP message header of extension header.
 * If there are more than one, return the first one.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Header name.
 * @param [out] ppstHdr Extension header data.
 *
 * @retval ZOK Find header successfully.
 * @retval ZFAILED Find header failed.
 */
ZFUNC ZINT Sip_FindExtHdr(ST_SIP_MSG *pstMsg, ZCHAR *pcName, 
                ST_SIP_HDR_EXT_HDR **ppstHdr);

/**
 * @brief Find SIP message header of extension header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Header name.
 * @param [in] iIndex Index value in same name headers.
 * @param [out] ppstHdr Extension header data.
 *
 * @retval ZOK Find header successfully.
 * @retval ZFAILED Find header failed.
 */
ZFUNC ZINT Sip_FindExtHdrX(ST_SIP_MSG *pstMsg, ZCHAR *pcName, ZINT iIndex, 
                ST_SIP_HDR_EXT_HDR **ppstHdr);

/**
 * @brief Get boundary string of MIME body.
 *
 * @param [in] pstContentType Content type data of SIP message.
 * @param [in] ppstBoundary Boundary string.
 *
 * @retval ZOK Get boundary successfully.
 * @retval ZFAILED Get boundary failed.
 */
ZFUNC ZINT Sip_GetMimeBoundary(ST_SIP_HDR_CONTENT_TYPE *pstContentType,
                ST_ZOS_SSTR **ppstBoundary);

/**
 * @brief Get string of MIME body.
 *
 * @param [in] pstStr SIP message body string.
 * @param [in] pstBoundary Boundary string.
 * @param [out] pstBody Body string.
 *
 * @retval ZOK Get body successfully.
 * @retval ZFAILED Get body failed.
 */
ZFUNC ZINT Sip_GetMimeBody(ST_ZOS_SSTR *pstStr, ST_ZOS_SSTR *pstBoundary, 
                ST_ZOS_SSTR *pstBody);

/**
 * @brief Get content length value of SIP message Content-Length header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piContentLen Content length.
 *
 * @retval ZOK Get content length successfully.
 * @retval ZFAILED Get content length failed.
 */
ZFUNC ZINT Sip_GetContentLen(ST_SIP_MSG *pstMsg, ZUINT *piContentLen);

/**
 * @brief Get content length value of SIP message Content-Length header.
 *
 * @param [in] pstStr SIP message string.
 * @param [out] piContentLen Content length.
 *
 * @retval ZOK Get content length successfully.
 * @retval ZFAILED Get content length failed.
 */
ZFUNC ZINT Sip_GetContentLen2(ST_ZOS_SSTR *pstStr, ZUINT *piContentLen);

/**
 * @brief Update content length value in SIP message Content-Length header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iContentLen New content length.
 *
 * @retval ZOK Update content length successfully.
 * @retval ZFAILED Update content length failed.
 */
ZFUNC ZINT Sip_UpdateContentLen(ST_SIP_MSG *pstMsg, ZUINT iContentLen);

/**
 * @brief Update content type.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] pstBody New body of SIP message.
 *
 * @retval ZOK Update content type successfully.
 * @retval ZFAILED Update content type failed.
 */
ZFUNC ZINT Sip_UpdateContentType(ZUBUF zBufId, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ST_SIP_BODY *pstBody);

/**
 * @brief Update branch string of SIP message Via header.
 *
 * @param [in] pstVia Via header of SIP message.
 *
 * @return Branch string.
 */
ZFUNC ST_ZOS_SSTR * Sip_GetViaBranch(ST_SIP_HDR_VIA *pstVia);

/**
 * @brief Create a SIP header message .
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Header type, @ref EN_SIP_HDR.
 *
 * @return New general created header address.
 */
ZFUNC ST_SIP_MSG_HDR * Sip_HdrMsgCreate(ST_SIP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Create a SIP header message .
 *
 * @param [in] pstMsg SIP message.
 *
 * @return New general created header address.
 */
ZFUNC ST_SIP_MSG_HDR * Sip_HdrMsgCreateX(ZUBUF zBufId, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ZUCHAR ucType);

/**
 * @brief Create a SIP header message .
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Header type, @ref EN_SIP_HDR.
 * @param [in] pstHdrName Header name string.
 * @param [in] pstHdrVal Header value string.
 *
 * @return New general created header address.
 */
ZFUNC ST_SIP_MSG_HDR * Sip_HdrMsgCreateXD(ZUBUF zBufId, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ZUCHAR ucType, 
                ST_ZOS_SSTR *pstHdrName, ST_ZOS_SSTR *pstHdrVal);

/**
 * @brief Find SIP message header of specific type. If there are more than one,
 *        return first.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 *
 * @return SIP message header address if found.
 */
ZFUNC ST_SIP_MSG_HDR * Sip_HdrMsgFind(ST_SIP_MSG *pstMsg, ZUCHAR ucType);

/**
 * @brief Create new header and add to header list.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Header type, @ref EN_SIP_HDR.
 *
 * @return New created header address.
 */
ZFUNC ZVOID * Sip_HdrLstCreateHdr(ZUBUF zBufId, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ZUCHAR ucType);

/**
 * @brief Create new header and add to header list.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Header type, @ref EN_SIP_HDR.
 * @param [in] pstHdrStr Header string.
 *
 * @return New created header address.
 */
ZFUNC ZINT Sip_HdrLstCreateHdrX(ZUBUF zBufId, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ZUCHAR ucType, 
                ST_ZOS_SSTR *pstHdrName, ST_ZOS_SSTR *pstHdrVal);

/**
 * @brief Delete header from header list. If there are more than one, 
 *        delete first.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Header type, @ref EN_SIP_HDR.
 *
 * @retval ZOK Delete header successfully.
 * @retval ZFAILED Delete header failed.
 */
ZFUNC ZINT Sip_HdrLstDeleteHdr(ST_SIP_MSG_HDR_LST *pstHdrLst, 
                ZUCHAR ucType);

/**
 * @brief Remove header from header list.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] pHdr Header data to remove.
 *
 * @retval ZOK Remove header successfully.
 * @retval ZFAILED Remove header failed.
 */
ZFUNC ZINT Sip_HdrLstRemoveHdr(ST_SIP_MSG_HDR_LST *pstHdrLst, 
                ZVOID *pHdr);

/**
 * @brief Get header count of specific type.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Header type, @ref EN_SIP_HDR.
 *
 * @return Header count value.
 */
ZFUNC ZINT Sip_HdrLstGetHdrSize(ST_SIP_MSG_HDR_LST *pstHdrLst, 
                ZUCHAR ucType);

/**
 * @brief Find SIP message header of specific type. If there are more than one,
 *        return first.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 *
 * @return SIP message header address if found.
 */
ZFUNC ZVOID * Sip_HdrLstFindHdr(ST_SIP_MSG_HDR_LST *pstHdrLst, 
                ZUCHAR ucType);

/**
 * @brief Find SIP message header of specific type.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 * @param [in] iIndex Index value in same type headers.
 *
 * @return SIP message header address if found.
 */
ZFUNC ZVOID * Sip_HdrLstFindHdrX(ST_SIP_MSG_HDR_LST *pstHdrLst, 
                ZUCHAR ucType, ZINT iIndex);
/**
 * @brief Find SIP message header value string of specific type. 
 *   If there are more than one, return the first one.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] ucType Type of SIP message header, @ref EN_SIP_HDR.
 * @param [out] ppstVal Header value string(except extension header).
 *
 * @retval ZOK Find header value successfully.
 * @retval ZFAILED Find header value failed.
 */
ZFUNC ZINT Sip_HdrLstFindHdrVal(ST_SIP_MSG_HDR_LST *pstHdrLst, 
                ZUCHAR ucType, ST_ZOS_SSTR **ppstVal);

/**
 * @brief Find SIP message header value string of specific type. 
 *   If there are more than one, return the first one.
 *  @note It is recommend used in get header value from an incoming SIP message.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] pcName SIP Header name.
 * @param [out] ppstVal Header value string(also include extension header).
 *
 * @retval ZOK Find header value successfully.
 * @retval ZFAILED Find header value failed.
 */
ZFUNC ZINT Sip_HdrLstFindHdrValX(ST_SIP_MSG_HDR_LST *pstHdrLst, 
                ZCHAR *pcName, ST_ZOS_SSTR **ppstVal);

/**
 * @brief Get content length value of SIP message Content-Length header.
 *
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [out] piContentLen Content length.
 *
 * @retval ZOK Get content length successfully.
 * @retval ZFAILED Get content length failed.
 */
ZFUNC ZINT Sip_HdrLstGetContentLen(ST_SIP_MSG_HDR_LST *pstHdrLst,
                ZUINT *piContentLen);

/**
 * @brief Update content length value in SIP message Content-Length header.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstHdrLst Header list of SIP message.
 * @param [in] iContentLen New content length.
 *
 * @retval ZOK Update content length successfully.
 * @retval ZFAILED Update content length failed.
 */
ZFUNC ZINT Sip_HdrLstUpdateContentLen(ZUBUF zBufId, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ZUINT iContentLen);

/**
 * @brief Create multi-part in multi-part list.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstMpartLst Multi-part list.
 * @param [out] ppstMpart New create multi-part.
 *
 * @retval ZOK Update content length successfully.
 * @retval ZFAILED Update content length failed.
 */
ZFUNC ZINT Sip_MPartLstCreateMPart(ZUBUF zBufId, 
                ST_SIP_BODY_MPART_LST *pstMpartLst, 
                ST_SIP_BODY_MPART **ppstMpart);

/**
 * @brief Decode message buffer from data buffer.
 *
 * @param [in] zData Data buffer contains SIP message string.
 * @param [in] bCopy Copy all data in message.
 * @param [out] ppstMsg New created SIP message.
 *
 * @retval ZOK Decode message successfully.
 * @retval ZFAILED Decode message failed.
 */
ZFUNC ZINT Sip_PacketDecode(ZDBUF zData, ST_SIP_MSG **ppstMsg);

/**
 * @brief Decode an element of SIP message.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstStr Message string to be decode.
 * @param [in] pfnDecode Decode function.
 * @param [out] pAnyElem Data structure of SIP message.
 *
 * @retval ZOK Decode message successfully.
 * @retval ZFAILED Decode message failed.
 */
ZFUNC ZINT Sip_AnyElemDecode(ZDBUF zBufId, ST_ZOS_SSTR *pstStr,
                PFN_SIPANYDECODE pfnDecode, ZVOID *pAnyElem);

/**
 * @brief Encode an element of SIP message.
 *
 * @param [in] pAnyElem Data structure of SIP message.
 * @param [in] pfnEncode Encode function.
 * @param [in] bFlatBuf Flat data buffer.
 * @param [out] pzMsgBuf Data buffer contains message string.
 *
 * @retval ZOK Decode message successfully.
 * @retval ZFAILED Decode message failed.
 */
ZFUNC ZINT Sip_AnyElemEncode(ZVOID *pAnyElem, PFN_SIPANYENCODE pfnEncode, 
                ZBOOL bFlatBuf, ZDBUF *pzMsgBuf);

/**
 * @brief Encode an element of SIP message.
 *
 * @param [in] pAnyElem Data structure of SIP message.
 * @param [in] pfnEncode Encode function.
 * @param [out] zMsgBuf Data buffer contains message string.
 *
 * @retval ZOK Decode message successfully.
 * @retval ZFAILED Decode message failed.
 */
ZFUNC ZINT Sip_AnyElemEncodeX(ZVOID *pAnyElem, PFN_SIPANYENCODE pfnEncode, 
                ZDBUF zMsgBuf);

/**
 * @brief SIP message find content encoding.
 *
 * @param [in] pstHdrLst SIP message head list.
 * @param [in] pcCoding encode type.
 *
 * @retval ZTRUE if has content-encoding head.
 * @retval ZFALSE if has not content-encoding head.
 */
ZFUNC ZBOOL Sip_HdrHasContentEncoding(ST_SIP_MSG_HDR_LST *pstHdrLst, 
               ZCONST ZCHAR *pcCoding);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_ABNF_UTIL_H__ */

