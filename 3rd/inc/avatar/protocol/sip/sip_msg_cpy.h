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
  File name     : sip_msg_cpy.h
  Module        : sip protocol service message copy
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_MSG_CPY_H__
#define _SIP_MSG_CPY_H__

/** 
 * @file
 * @brief Interfaces for SIP message copy with data buffer.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Copy ST_ZOS_SSTR string. */
#define SIP_CPY_SSTR(_buf, _dstsstr, _srcsstr) do { \
    if (Zos_UbufCpyXSStr(_buf, _srcsstr, _dstsstr) != ZOK) \
        return ZFAILED; \
} while (0)

/** @brief Copy string to ST_ZOS_SSTR string. */
#define SIP_CPY_NSTR(_buf, _dstsstr, _srcstr, _srclen) do { \
    if (Zos_UbufCpyNSStr(_buf, _srcstr, _srclen, _dstsstr) != ZOK) \
        return ZFAILED; \
} while (0)

/** @brief Copy transport address. */
#define SIP_CPY_TPT_ADDR(_dst, _src) do { \
    if (_dst && _src) Zos_MemCpy(_dst, _src, sizeof(ST_SIP_TPT_ADDR)); \
} while (0)

/** @brief Copy function type defination. */
typedef ZINT (*PFN_SIPANYCPY)(ZUBUF zBufId, ZVOID *pstDst, 
                ZVOID *pstSrc);

/**
 * @brief Copy any type element of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 * @param [in] pfnEncode Encode function.
 * @param [in] pfnDecode Decode function.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAnyElem(ZUBUF zBufId, ZVOID *pstDst, 
                ZVOID *pstSrc, PFN_SIPANYENCODE pfnEncode,
                PFN_SIPANYDECODE pfnDecode);

/**
 * @brief Copy any type element of SIP message with specific copy method.
 *
 * @param [in] zBufId Buffer Id..
 * @param [in] pSrc Source data structure.
 * @param [in] pfnCpy Copy function.
 * @param [in] iSize Source data size in bytes.
 * @param [out] ppDst Destination data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAnyElemX(ZUBUF zBufId, ZVOID *pSrc,
                PFN_SIPANYCPY pfnCpy, ZINT iSize, ZVOID **ppDst);

/**
 * @brief Copy any message header of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [in] pstDstHdr Destination message header.
 * @param [in] pstSrcHdr Source message header.
 *
 * @retval ZOK Copy header successfully.
 * @retval ZFAILED Otherwise.
 */
ZINT Sip_CpyAnyHdr(ZUBUF zBufId, ST_SIP_MSG_HDR *pstDstHdr, 
                ST_SIP_MSG_HDR *pstSrcHdr);

/**
 * @brief Copy list of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pDstLst Destination data list.
 * @param [in] pSrcLst Source data list.
 * @param [in] iSize Source data size in bytes.
 * @param [in] pfnCpy Copy function.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyList(ZUBUF zBufId, ZVOID *pDstLst, 
                ZVOID *pSrcLst, ZUINT iSize, PFN_SIPANYCPY pfnCpy);

/**
 * @brief Copy list data of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pDstLst Destination data list.
 * @param [in] pSrc Source data.
 * @param [in] iSize Source data size in bytes.
 * @param [in] pfnCpy Copy function.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyListData(ZUBUF zBufId, ZVOID *pDstLst, 
                ZVOID *pSrc, ZUINT iSize, PFN_SIPANYCPY pfnCpy);

/**
 * @brief Copy header list from SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstHdrLst Destination header list.
 * @param [in] pstMsg Source SIP message.
 * @param [in] ucType Header type to copy, @ref EN_SIP_HDR.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrFromMsg(ZUBUF zBufId,
                ST_SIP_MSG_HDR_LST *pstHdrLst, ST_SIP_MSG *pstMsg,
                ZUCHAR ucType);

/**
 * @brief Copy header list of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrLst(ZUBUF zBufId, ST_SIP_MSG_HDR_LST *pstDst,
                ST_SIP_MSG_HDR_LST *pstSrc);

/**
 * @brief Copy header list of SIP message by header type.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 * @param [in] ucHdrType The header type.
 *
 * @retval ZOK Copy head data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrLstType(ZUBUF zBufId, ST_SIP_MSG_HDR_LST *pstDst, 
                ST_SIP_MSG_HDR_LST *pstSrc, ZUCHAR ucHdrType);

/**
 * @brief Copy header "From" or "To" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrFromTo(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstDst,
                ST_SIP_HDR_FROM_TO *pstSrc);

/**
 * @brief Copy header "CSeq" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrCseq(ZUBUF zBufId, 
                ST_SIP_HDR_CSEQ *pstDst,
                ST_SIP_HDR_CSEQ *pstSrc);

/**
 * @brief Copy header "Call-ID" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrCallId(ZUBUF zBufId, 
                ST_SIP_HDR_CALL_ID *pstDst,
                ST_SIP_HDR_CALL_ID *pstSrc);

/**
 * @brief Copy header "Contact" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrContact(ZUBUF zBufId, 
                ST_SIP_HDR_CONTACT *pstDst,
                ST_SIP_HDR_CONTACT *pstSrc);

/**
 * @brief Copy header "Route" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrRoute(ZUBUF zBufId, 
                ST_SIP_HDR_ROUTE *pstDst,
                ST_SIP_HDR_ROUTE *pstSrc);

/**
 * @brief Copy header "Via" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrVia(ZUBUF zBufId, 
                ST_SIP_HDR_VIA *pstDst,
                ST_SIP_HDR_VIA *pstSrc);

/**
 * @brief Copy header "Content-Type" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrContentType(ZUBUF zBufId, 
                ST_SIP_HDR_CONTENT_TYPE *pstDst,
                ST_SIP_HDR_CONTENT_TYPE *pstSrc);

/**
 * @brief Copy header "Accept" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrAcpt(ZUBUF zBufId, 
                ST_SIP_HDR_ACPT *pstDst,
                ST_SIP_HDR_ACPT *pstSrc);

/**
 * @brief Copy header "Supported" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrSupted(ZUBUF zBufId, 
                ST_SIP_HDR_SUPTED *pstDst,
                ST_SIP_HDR_SUPTED *pstSrc);

/**
 * @brief Copy header "Require" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrRequire(ZUBUF zBufId, 
                ST_SIP_HDR_REQUIRE *pstDst,
                ST_SIP_HDR_REQUIRE *pstSrc);

/**
 * @brief Copy header "Allow" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrAllow(ZUBUF zBufId, 
                ST_SIP_HDR_ALLOW *pstDst,
                ST_SIP_HDR_ALLOW *pstSrc);

/**
 * @brief Copy header "Privacy" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrPrivacy(ZUBUF zBufId, 
                ST_SIP_HDR_PRIVACY *pstDst,
                ST_SIP_HDR_PRIVACY *pstSrc);

/**
 * @brief Copy header "Authorization" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrAuthor(ZUBUF zBufId, 
                ST_SIP_HDR_AUTHOR *pstDst,
                ST_SIP_HDR_AUTHOR *pstSrc);

/**
 * @brief Copy header "Proxy-Authorization" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrProxyAuthor(ZUBUF zBufId, 
                ST_SIP_HDR_PROXY_AUTHOR *pstDst,
                ST_SIP_HDR_PROXY_AUTHOR *pstSrc);

/**
 * @brief Copy header "Alert-Info" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrAlertInfo(ZUBUF zBufId, 
                ST_SIP_HDR_ALERT_INFO *pstDst,
                ST_SIP_HDR_ALERT_INFO *pstSrc);

/**
 * @brief Copy header "Session-Expires" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrSessExpires(ZUBUF zBufId, 
                ST_SIP_HDR_SESS_EXPIRES *pstDst,
                ST_SIP_HDR_SESS_EXPIRES *pstSrc);

/**
 * @brief Copy header "Min-SE" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrMinSessExpires(ZUBUF zBufId, 
                ST_SIP_HDR_MIN_SE *pstDst,
                ST_SIP_HDR_MIN_SE *pstSrc);

/**
 * @brief Copy header "Refer-To" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrReferTo(ZUBUF zBufId, 
                ST_SIP_HDR_REFER_TO *pstDst,
                ST_SIP_HDR_REFER_TO *pstSrc);

/**
 * @brief Copy header "Referred-By" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrReferredBy(ZUBUF zBufId, 
                ST_SIP_HDR_REFERRED_BY *pstDst,
                ST_SIP_HDR_REFERRED_BY *pstSrc);

/**
 * @brief Copy header "Replaces" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrReplaces(ZUBUF zBufId, 
                ST_SIP_HDR_REPLACES *pstDst,
                ST_SIP_HDR_REPLACES *pstSrc);

/**
 * @brief Copy header "P-Preferred-Identity" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrPPreferredId(ZUBUF zBufId, 
                ST_SIP_HDR_P_PREFERRED_ID *pstDst,
                ST_SIP_HDR_P_PREFERRED_ID *pstSrc);
/**
 * @brief Copy header "P-Asserted-Identity" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrPAssertedId(ZUBUF zBufId, 
                ST_SIP_HDR_P_ASSERTED_ID *pstDst,
                ST_SIP_HDR_P_ASSERTED_ID *pstSrc);

/**
 * @brief Copy header "P-Answer-State" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrPAnswerState(ZUBUF zBufId, 
                ST_SIP_HDR_P_ANSWER_STATE *pstDst,
                ST_SIP_HDR_P_ANSWER_STATE *pstSrc);

/**
 * @brief Copy header "P-Early-Media" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrPEarlyMedia(ZUBUF zBufId, 
                ST_SIP_HDR_P_EARLY_MEDIA *pstDst,
                ST_SIP_HDR_P_EARLY_MEDIA *pstSrc);

/**
 * @brief Copy header "Subs-State" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrSubsSta(ZUBUF zBufId, 
                ST_SIP_HDR_SUBS_STA *pstDst,
                ST_SIP_HDR_SUBS_STA *pstSrc);

/**
 * @brief Copy header "Event" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrEvnt(ZUBUF zBufId, 
                ST_SIP_HDR_EVNT *pstDst,
                ST_SIP_HDR_EVNT *pstSrc);

/**
 * @brief Copy header "Reason" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrReason(ZUBUF zBufId, 
                ST_SIP_HDR_REASON *pstDst,
                ST_SIP_HDR_REASON *pstSrc);

/**
 * @brief Copy header "Warning" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrWarn(ZUBUF zBufId, 
                ST_SIP_HDR_WARN *pstDst,
                ST_SIP_HDR_WARN *pstSrc);

/**
 * @brief Copy header "extend-header" of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrExtHdr(ZUBUF zBufId, 
                ST_SIP_HDR_EXT_HDR *pstDst,
                ST_SIP_HDR_EXT_HDR *pstSrc);

/**
 * @brief Copy SIP message.
 *
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyMsg(ST_SIP_MSG *pstDst, ST_SIP_MSG *pstSrc);

/**
 * @brief Copy body of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyBody(ZUBUF zBufId, 
                ST_SIP_BODY *pstDst,
                ST_SIP_BODY *pstSrc);

/**
 * @brief Copy multi part body of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyMpart(ZUBUF zBufId, 
                ST_SIP_BODY_MPART *pstDst,
                ST_SIP_BODY_MPART *pstSrc);

/**
 * @brief Copy single part body of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpySpart(ZUBUF zBufId, 
                ST_SIP_BODY_SPART *pstDst,
                ST_SIP_BODY_SPART *pstSrc);

/**
 * @brief Copy sfrag body of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpySfrag(ZUBUF zBufId, 
                ST_SIP_BODY_SFRAG *pstDst,
                ST_SIP_BODY_SFRAG *pstSrc);

/**
 * @brief Copy userinfo of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyUserInfo(ZUBUF zBufId,
                ST_SIP_USER_INFO *pstDst,
                ST_SIP_USER_INFO *pstSrc);

/**
 * @brief Copy host of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHost(ZUBUF zBufId,
                ST_SIP_HOST *pstDst,
                ST_SIP_HOST *pstSrc);

/**
 * @brief Copy hostport of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHostPort(ZUBUF zBufId,
                ST_SIP_HOST_PORT *pstDst,
                ST_SIP_HOST_PORT *pstSrc);

/**
 * @brief Copy header of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdr(ZUBUF zBufId,
                ST_SIP_HDR *pstDst,
                ST_SIP_HDR *pstSrc);

/**
 * @brief Copy headers of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrs(ZUBUF zBufId,
                ST_SIP_HDRS *pstDst,
                ST_SIP_HDRS *pstSrc);

/**
 * @brief Copy SIP-URI & SIPS-URI of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpySipUri(ZUBUF zBufId,
                ST_SIP_SIP_URI *pstDst,
                ST_SIP_SIP_URI *pstSrc);

/**
 * @brief Copy global-number of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyGlobalNumber(ZUBUF zBufId,
                ST_SIP_GLOBAL_NUMBER *pstDst,
                ST_SIP_GLOBAL_NUMBER *pstSrc);

/**
 * @brief Copy local-number of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyLocalNumber(ZUBUF zBufId,
                ST_SIP_LOCAL_NUMBER *pstDst,
                ST_SIP_LOCAL_NUMBER *pstSrc);

/**
 * @brief Copy TEL-URI of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyTelUri(ZUBUF zBufId,
                ST_SIP_TEL_URI *pstDst,
                ST_SIP_TEL_URI *pstSrc);

/**
 * @brief Copy absoluteURI of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAbsoUri(ZUBUF zBufId,
                ST_SIP_ABSO_URI *pstDst,
                ST_SIP_ABSO_URI *pstSrc);

/**
 * @brief Copy addr-spec of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAddrSpec(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstDst,
                ST_SIP_ADDR_SPEC *pstSrc);

/**
 * @brief Copy addr-spec of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAddrSpecX(ZUBUF zBufId,
                ST_SIP_NA_SPEC *pstDst,
                ST_SIP_NA_SPEC *pstSrc);

/**
 * @brief Copy display-name of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDispName(ZUBUF zBufId,
                ST_SIP_DISP_NAME *pstDst,
                ST_SIP_DISP_NAME *pstSrc);

/**
 * @brief Copy name-addr of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyNameAddr(ZUBUF zBufId,
                ST_SIP_NAME_ADDR *pstDst,
                ST_SIP_NAME_ADDR *pstSrc);

/**
 * @brief Copy name-addr of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyNameAddrX(ZUBUF zBufId,
                ST_SIP_NA_SPEC *pstDst,
                ST_SIP_NA_SPEC *pstSrc);

/**
 * @brief Copy name-addr or addr-spec of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyNaSpec(ZUBUF zBufId,
                ST_SIP_NA_SPEC *pstDst,
                ST_SIP_NA_SPEC *pstSrc);

/**
 * @brief Copy request-uri of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyReqUri(ZUBUF zBufId,
                ST_SIP_REQ_URI *pstDst,
                ST_SIP_REQ_URI *pstSrc);

/**
 * @brief Copy IM-URI of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyImUri(ZUBUF zBufId,
                ST_SIP_IM_URI *pstDst,
                ST_SIP_IM_URI *pstSrc);

/**
 * @brief Copy Message-ID & Content-ID uri of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyMcUri(ZUBUF zBufId,
                ST_SIP_MC_URI *pstDst,
                ST_SIP_MC_URI *pstSrc);

/**
 * @brief Copy domain of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDomain(ZUBUF zBufId,
                ST_SIP_DOMAIN *pstDst,
                ST_SIP_DOMAIN *pstSrc);

/**
 * @brief Copy URI of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyUri(ZUBUF zBufId,
                ST_SIP_URI *pstDst,
                ST_SIP_URI *pstSrc);

/**
 * @brief Copy segment of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpySegment(ZUBUF zBufId,
                ST_SIP_SEGMENT *pstDst,
                ST_SIP_SEGMENT *pstSrc);

/**
 * @brief Copy Method of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyMethod(ZUBUF zBufId,
                ST_SIP_METHOD *pstDst,
                ST_SIP_METHOD *pstSrc);

/**
 * @brief Copy call-id of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyCallId(ZUBUF zBufId,
                ST_SIP_CALLID *pstDst,
                ST_SIP_CALLID *pstSrc);

/**
 * @brief Copy privacy value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyPrivVal(ZUBUF zBufId,
                ST_SIP_PRIV_VAL *pstDst,
                ST_SIP_PRIV_VAL *pstSrc);

/**
 * @brief Copy profile value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyProfVal(ZUBUF zBufId,
                ST_SIP_PROF_VAL *pstDst,
                ST_SIP_PROF_VAL *pstSrc);

/**
 * @brief Copy diff-processing of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDiffProc(ZUBUF zBufId,
                ST_SIP_DIFF_PROC *pstDst,
                ST_SIP_DIFF_PROC *pstSrc);

/**
 * @brief Copy event type of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyEvntType(ZUBUF zBufId,
                ST_SIP_EVNT_TYPE *pstDst,
                ST_SIP_EVNT_TYPE *pstSrc);
/**
 * @brief Copy route parameter of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyRouteParm(ZUBUF zBufId,
                ST_SIP_ROUTE_PARM *pstDst,
                ST_SIP_ROUTE_PARM *pstSrc);

/**
 * @brief Copy via-parms of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyViaParms(ZUBUF zBufId,
                ST_SIP_VIA_PARMS *pstDst,
                ST_SIP_VIA_PARMS *pstSrc);

/**
 * @brief Copy via-parm of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyViaParm(ZUBUF zBufId,
                ST_SIP_VIA_PARM *pstDst,
                ST_SIP_VIA_PARM *pstSrc);

/**
 * @brief Copy sent-protocol parameter of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyViaSentProtocol(ZUBUF zBufId,
                ST_SIP_SENT_PROTOCOL *pstDst,
                ST_SIP_SENT_PROTOCOL *pstSrc);

/**
 * @brief Copy sent-by parameter of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */ 
ZFUNC ZINT Sip_CpyViaSentBy(ZUBUF zBufId,
                ST_SIP_SENT_BY *pstDst,
                ST_SIP_SENT_BY *pstSrc); 

/**
 * @brief Copy from-param & to-param of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyFromToParm(ZUBUF zBufId, 
                ST_SIP_FROM_TO_PARM *pstDst,
                ST_SIP_FROM_TO_PARM *pstSrc);

/**
 * @brief Copy P-Preferred-ID value of SIP message.
 *
 * @param [in] zBufId Buffer Id.
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZINT Sip_CpyPPreferredIdVal(ZUBUF zBufId, 
                ST_SIP_PPREFERREDID_VAL *pstDst,
                ST_SIP_PPREFERREDID_VAL *pstSrc);

/**
 * @brief Copy P-Asserted-ID value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyPAssertedIdVal(ZUBUF zBufId, 
                ST_SIP_PASSERTEDID_VAL *pstDst,
                ST_SIP_PASSERTEDID_VAL *pstSrc);

/**
 * @brief Copy reason value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyReasonVal(ZUBUF zBufId, 
                ST_SIP_REASON_VAL *pstDst,
                ST_SIP_REASON_VAL *pstSrc);

/**
 * @brief Copy event reason value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyEvntReasonVal(ZUBUF zBufId, 
                ST_SIP_EVNT_REASON_VAL *pstDst,
                ST_SIP_EVNT_REASON_VAL *pstSrc);

/**
 * @brief Copy subs status value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpySubsStaVal(ZUBUF zBufId, 
                ST_SIP_SUBSTA_VAL *pstDst,
                ST_SIP_SUBSTA_VAL *pstSrc);

/**
 * @brief Copy warning value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyWarnVal(ZUBUF zBufId, 
                ST_SIP_WARN_VAL *pstDst,
                ST_SIP_WARN_VAL *pstSrc);

/**
 * @brief Copy generic-param of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyGenParm(ZUBUF zBufId, 
                ST_SIP_GEN_PARM *pstDst, 
                ST_SIP_GEN_PARM *pstSrc);

/**
 * @brief Copy em-param of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyEmParm(ZUBUF zBufId, 
                ST_SIP_EM_PARM *pstDst, 
                ST_SIP_EM_PARM *pstSrc);

/**
 * @brief Copy event-param of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyEvntParm(ZUBUF zBufId, 
                ST_SIP_EVNT_PARM *pstDst, 
                ST_SIP_EVNT_PARM *pstSrc);

/**
 * @brief Copy event temp of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyEvntTemp(ZUBUF zBufId, 
                ST_SIP_EVNT_TEMP *pstDst, 
                ST_SIP_EVNT_TEMP *pstSrc);

/**
 * @brief Copy subs-exp parms of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpySubExpParms(ZUBUF zBufId, 
                ST_SIP_SUBEXP_PARMS *pstDst, 
                ST_SIP_SUBEXP_PARMS *pstSrc);

/**
 * @brief Copy accept range of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAcptRange(ZUBUF zBufId, 
                ST_SIP_ACPT_RANGE *pstDst, 
                ST_SIP_ACPT_RANGE *pstSrc);

/**
 * @brief Copy media type of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyMediaType(ZUBUF zBufId, 
                ST_SIP_MEDIA_TYPE *pstDst, 
                ST_SIP_MEDIA_TYPE *pstSrc);

/**
 * @brief Copy option tag of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyOptTag(ZUBUF zBufId, 
                ST_SIP_OPT_TAG *pstDst, 
                ST_SIP_OPT_TAG *pstSrc);

/**
 * @brief Copy accept parm of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAcptParm(ZUBUF zBufId, 
                ST_SIP_ACPT_PARM *pstDst, 
                ST_SIP_ACPT_PARM *pstSrc);

/**
 * @brief Copy alert parm of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAlertParm(ZUBUF zBufId, 
                ST_SIP_ALERT_PARM *pstDst, 
                ST_SIP_ALERT_PARM *pstSrc);

/**
 * @brief Copy replaces-param of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyReplacesParm(ZUBUF zBufId, 
                ST_SIP_REPLACES_PARM *pstDst, 
                ST_SIP_REPLACES_PARM *pstSrc);

/**
 * @brief Copy contact-param of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyContactParm(ZUBUF zBufId, 
                ST_SIP_CONTACT_PARM *pstDst,
                ST_SIP_CONTACT_PARM *pstSrc);

/**
 * @brief Copy contact-params of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyContactParms(ZUBUF zBufId, 
                ST_SIP_CONTACT_PARMS *pstDst,
                ST_SIP_CONTACT_PARMS *pstSrc);

/**
 * @brief Copy reason-params of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyReasonParms(ZUBUF zBufId, 
                ST_SIP_REASON_PARMS *pstDst,
                ST_SIP_REASON_PARMS *pstSrc);

/**
 * @brief Copy c-p-q of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyQval(ZUBUF zBufId, 
                ST_SIP_QVAL *pstDst,
                ST_SIP_QVAL *pstSrc);

/**
 * @brief Copy uri-parameter of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyUriParm(ZUBUF zBufId, 
                ST_SIP_URI_PARM *pstDst,
                ST_SIP_URI_PARM *pstSrc);

/**
 * @brief Copy SIP par (global-phone-number's parameters) of SIP message.
 * 
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyPar(ZUBUF zBufId, 
                ST_SIP_PAR *pstDst,
                ST_SIP_PAR *pstSrc);

/**
 * @brief Copy SIP parameter of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyParm(ZUBUF zBufId, 
                ST_SIP_PARM *pstDst,
                ST_SIP_PARM *pstSrc);

/**
 * @brief Copy contact header to request uri of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDstReqUri Description.
 * @param [in] pstSrcContact Description.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyHdrContactToReqUri(ZUBUF zBufId,
                ST_SIP_REQ_URI *pstDstReqUri,
                ST_SIP_HDR_CONTACT *pstSrcContact);

/**
 * @brief Copy credentials of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyCredents(ZUBUF zBufId,
                ST_SIP_CREDENTS *pstDst,
                ST_SIP_CREDENTS *pstSrc);

/**
 * @brief Copy challenge of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyChallenge(ZUBUF zBufId,
                ST_SIP_CHALLENGE *pstDst,
                ST_SIP_CHALLENGE *pstSrc);

/**
 * @brief Copy digest response of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDigestRsp(ZUBUF zBufId,
                ST_SIP_DIGEST_RSP *pstDst,
                ST_SIP_DIGEST_RSP *pstSrc);

/**
 * @brief Copy digest of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDigest(ZUBUF zBufId,
                ST_SIP_DIGEST *pstDst,
                ST_SIP_DIGEST *pstSrc);

/**
 * @brief Copy dig rep of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDigRsp(ZUBUF zBufId,
                ST_SIP_DIG_RSP *pstDst,
                ST_SIP_DIG_RSP *pstSrc);

/**
 * @brief Copy digest-cln of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDigCln(ZUBUF zBufId,
                ST_SIP_DIGEST_CLN *pstDst,
                ST_SIP_DIGEST_CLN *pstSrc);

/**
 * @brief Copy algorithm of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAlgo(ZUBUF zBufId,
                ST_SIP_ALGO *pstDst,
                ST_SIP_ALGO *pstSrc);

/**
 * @brief Copy aka-namespace of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAkaNs(ZUBUF zBufId,
                ST_SIP_AKA_NS *pstDst,
                ST_SIP_AKA_NS *pstSrc);

/**
 * @brief Copy algorithm-value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAlgoVal(ZUBUF zBufId,
                ST_SIP_ALGO_VAL *pstDst,
                ST_SIP_ALGO_VAL *pstSrc);

/**
 * @brief Copy type value of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyTypeVal(ZUBUF zBufId,
                ST_SIP_TYPE_VAL *pstDst,
                ST_SIP_TYPE_VAL *pstSrc);

/**
 * @brief Copy auth-param of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyAuthParm(ZUBUF zBufId,
                ST_SIP_AUTH_PARM *pstDst,
                ST_SIP_AUTH_PARM *pstSrc);

/**
 * @brief Copy other-response of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyOtherRsp(ZUBUF zBufId,
                ST_SIP_OTHER_RSP *pstDst,
                ST_SIP_OTHER_RSP *pstSrc);

/**
 * @brief Copy other-challenge of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyOtherChallenge(ZUBUF zBufId,
                ST_SIP_OTHER_CHALLENGE *pstDst,
                ST_SIP_OTHER_CHALLENGE *pstSrc);

/**
 * @brief Copy challenge digest to reponse.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstRsp Destination data structure of digest response.
 * @param [in] pstDigest Source data structure of challenge digest.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyDigestToRsp(ZUBUF zBufId,
                ST_SIP_DIGEST_RSP *pstRsp,
                ST_SIP_DIGEST *pstDigest);

/**
 * @brief Copy m-parameter of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyMParm(ZUBUF zBufId,
                ST_SIP_MPARM *pstDst,
                ST_SIP_MPARM *pstSrc);

/**
 * @brief Copy request line of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyReqLine(ZUBUF zBufId, 
                ST_SIP_REQ_LINE *pstDst, 
                ST_SIP_REQ_LINE *pstSrc);

/**
 * @brief Copy status line of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyStatusLine(ZUBUF zBufId, 
                ST_SIP_STATUS_LINE *pstDst, 
                ST_SIP_STATUS_LINE *pstSrc);

/**
 * @brief Copy event package of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyEvntPkg(ZUBUF zBufId,
                ST_SIP_EVNT_PKG *pstDst,
                ST_SIP_EVNT_PKG *pstSrc);

/**
 * @brief Copy warn agent of SIP message.
 *
 * @param [in] zBufId Buffer Id..
 * @param [out] pstDst Destination data structure.
 * @param [in] pstSrc Source data structure.
 *
 * @retval ZOK Copy data successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_CpyWarnAgent(ZUBUF zBufId,
                ST_SIP_WARN_AGENT *pstDst,
                ST_SIP_WARN_AGENT *pstSrc);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_MSG_CPY_H__ */

