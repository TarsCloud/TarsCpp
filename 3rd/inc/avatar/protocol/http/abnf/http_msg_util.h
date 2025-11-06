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
  File name     : http_msg_util.h
  Module        : http protocol
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-03-30
  Description   :
    Marcos and structure definitions required by the message.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTP_MSG_UTIL_H__
#define _HTTP_MSG_UTIL_H__

#ifdef __cplusplus
extern "C"{
#endif

/* http MD5 string len, 32 and a '\0' */
#define HTTP_MD5_HEX_STR_LEN 33

/* http convert MD5 to hex string */
#define HTTP_MD5_TO_HEX_STR(_hexstr, _md5) do { \
    ZCHAR *pcTmpStr = (ZCHAR *)(_hexstr); \
    ZINT i; \
    for (i = 0; i < ZMD5_RESULT_LEN; i++) \
    { \
        Zos_SPrintf(pcTmpStr, "%02x", _md5[i]); \
        pcTmpStr += 2; \
    } \
} while (0)

/* http message create */
ZFUNC ZINT Http_MsgCreate(ST_HTTP_MSG **ppstMsg);

/* http message delete */
ZFUNC ZINT Http_MsgDelete(ST_HTTP_MSG *pstMsg);

/* http message clone */
ZFUNC ZINT Http_MsgClone(ST_HTTP_MSG *pstSrcMsg, ST_HTTP_MSG **ppstDstMsg);

/* http message decode */
ZFUNC ZINT Http_MsgDecode(ZDBUF zMsgBuf, ST_HTTP_MSG **ppstMsg);

/* http message event encode */
ZFUNC ZINT Http_MsgEncode(ST_HTTP_MSG *pstMsg, ZDBUF *pzMsgBuf);

/* http message get method */
ZFUNC ZINT Http_MsgGetMethod(ST_HTTP_MSG *pstMsg, ZUCHAR *pucMethod);

/* http message add request line */
ZFUNC ZINT Http_MsgAddReqLine(ST_HTTP_MSG *pstMsg, ZUCHAR ucMethod,
                ST_ZOS_SSTR *pstUri);

/* http message add status line */
ZFUNC ZINT Http_MsgAddStatLine(ST_HTTP_MSG *pstMsg, ZUINT iStatCode);

/* http message add accept */
ZFUNC ZINT Http_MsgAddAcpt(ST_HTTP_MSG *pstMsg, ZCHAR *pcMediaTypess);

/* http message add accept language */
ZFUNC ZINT Http_MsgAddAcptLang(ST_HTTP_MSG *pstMsg, ZCHAR *pcLangs);

/* http message add accept encoding */
ZFUNC ZINT Http_MsgAddAcptEncoding(ST_HTTP_MSG *pstMsg, ZCHAR *pcCodings);

/* http message add accept range */
ZFUNC ZINT Http_MsgAddAcptRange(ST_HTTP_MSG *pstMsg, ZUCHAR ucRange);

/* http message add allow */
ZFUNC ZINT Http_MsgAddAllow(ST_HTTP_MSG *pstMsg, ZCHAR *pcMethods);

/* http message add cache control */
ZFUNC ZINT Http_MsgAddCacheCtrl(ST_HTTP_MSG *pstMsg, ZUCHAR ucDirect);

/* http message add connection */
ZFUNC ZINT Http_MsgAddConn(ST_HTTP_MSG *pstMsg, ZCHAR *pcOpt);

/* http message add content encoding */
ZFUNC ZINT Http_MsgAddContentEncoding(ST_HTTP_MSG *pstMsg, 
                ZCHAR *pcCodings);

/* http message add Content-type */
ZFUNC ZINT Http_MsgAddContentType(ST_HTTP_MSG *pstMsg, ZUCHAR ucMtype, 
                ZUCHAR ucMSubtype);

/* http message add Content-type */
ZFUNC ZINT Http_MsgAddContentTypeX(ST_HTTP_MSG *pstMsg, 
                ZCHAR *pcMediaType);

/* http message add content disposition */
ZFUNC ZINT Http_MsgAddContentDsp(ST_HTTP_MSG *pstMsg, 
                ZUCHAR ucDspType);

/* http message add content disposition */
ZFUNC ZINT Http_MsgAddContentDspX(ST_HTTP_MSG *pstMsg, 
                ZCHAR *pcTypeExt);

/* http message add content disposition parameter */
ZFUNC ZINT Http_MsgAddContentDspParm(ST_HTTP_MSG *pstMsg, 
                ZCHAR *pcAttr, ZCHAR *pcVal);

/* http message add date */
ZFUNC ZINT Http_MsgAddDate(ST_HTTP_MSG *pstMsg, ST_ZOS_SYS_TIME *pstDate);

/* http message add Etag */
ZFUNC ZINT Http_MsgAddEtag(ST_HTTP_MSG *pstMsg, ST_ZOS_SSTR *pstTag);

/* http message add expire */
ZFUNC ZINT Http_MsgAddExpire(ST_HTTP_MSG *pstMsg, ST_HTTP_DATE *pstDate);

/* http message add host header by name */
ZFUNC ZINT Http_MsgAddHostByName(ST_HTTP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstName, ZUINT iPort);

/* http message add host header by ipv4 */
ZFUNC ZINT Http_MsgAddHostByIpv4(ST_HTTP_MSG *pstMsg, ZUINT iIpv4,
                ZUINT iPort);

/* http message add host header by ipv6 */
ZFUNC ZINT Http_MsgAddHostByIpv6(ST_HTTP_MSG *pstMsg, ZUCHAR *pucIpv6,
                ZUINT iPort);

/* http message add If-Match */
ZFUNC ZINT Http_MsgAddIfMatch(ST_HTTP_MSG *pstMsg, ST_ZOS_SSTR *pstMatchTag);

/* http message add If-None-Match */
ZFUNC ZINT Http_MsgAddIfNoMatch(ST_HTTP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstNoMatchTag);

/* http message add If-Modified-Since */
ZFUNC ZINT Http_MsgAddIfMdfySince(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_DATE *pstDate);

/* http message add If-Unmodified-Since */
ZFUNC ZINT Http_MsgAddIfUnmdfySince(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_DATE *pstDate);

/* http message add last modified */
ZFUNC ZINT Http_MsgAddLastMdfy(ST_HTTP_MSG *pstMsg, ST_HTTP_DATE *pstDate);

/* http message add location */
ZFUNC ZINT Http_MsgAddLocat(ST_HTTP_MSG *pstMsg, ST_ZOS_SSTR *pstUri);

/* http message add range */
ZFUNC ZINT Http_MsgAddRange(ST_HTTP_MSG *pstMsg, ZUINT iFirstPos,
                ZUINT iLastPos);

/* http message add content range */
ZFUNC ZINT Http_MsgAddContentRange(ST_HTTP_MSG *pstMsg, ZUINT iFirstPos,
                ZUINT iLastPos, ZUINT iInstanceLen);

/* http message add refer */
ZFUNC ZINT Http_MsgAddRefer(ST_HTTP_MSG *pstMsg, ST_ZOS_SSTR *pstUri);

/* http message add user server */
ZFUNC ZINT Http_MsgAddServer(ST_HTTP_MSG *pstMsg, ZCHAR *pcProductVal);

/* http message add transfer encoding */
ZFUNC ZINT Http_MsgAddTrsfEncoding(ST_HTTP_MSG *pstMsg, ZCHAR *pcCodings);

/* http message add user agent */
ZFUNC ZINT Http_MsgAddUserAgent(ST_HTTP_MSG *pstMsg, ZCHAR *pcProductVal);

/* http message add X-3GPP-Intended-Identity */
ZFUNC ZINT Http_MsgAddX3gppIntendId(ST_HTTP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstIdent);

/* http message add body string */
ZFUNC ZINT Http_MsgAddBody(ST_HTTP_MSG *pstMsg, ST_ZOS_SSTR *pstBodyStr);

/* http message add body buffer */
ZFUNC ZINT Http_MsgAddBodyX(ST_HTTP_MSG *pstMsg, ZDBUF zBodyBuf);

/* http message add body file */
ZFUNC ZINT Http_MsgAddBodyF(ST_HTTP_MSG *pstMsg, ZCHAR *pcFileName);

/* http message add body for multi part */
ZFUNC ZINT Http_MsgAddBodyM(ST_HTTP_MSG *pstMsg, ZCHAR *pcBoundary);

/* http message add body part */
ZFUNC ST_HTTP_BODY_PART * Http_MsgAddBodyPart(ST_HTTP_MSG *pstMsg);

/* http message get content-type */
ZFUNC ZINT Http_MsgGetContentType(ST_HTTP_MSG *pstMsg, 
                ZUCHAR *pucMtype, ZUCHAR *pucMSubtype);

/* http message get ETag */
ZFUNC ZINT Http_MsgGetContentEtag(ST_HTTP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstETagStr);

/* http message content type to part */
ZFUNC ZINT Http_PartAddContentType(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZUCHAR ucMType, ZUCHAR ucSubType);

/* http message content type to part */
ZFUNC ZINT Http_PartAddContentTypeX(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZCHAR *pcMType, ZCHAR *pcSubType);

/* http message content type to part */
ZFUNC ZINT Http_PartAddContentTypeXX(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZCHAR *pcMediaType);

/* http message content disposition to part */
ZFUNC ZINT Http_PartAddContentDsp(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZUCHAR ucDspType);

/* http message content disposition to part */
ZFUNC ZINT Http_PartAddContentDspX(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZCHAR *pcDspType);

/* http message content disposition parameter to part */
ZFUNC ZINT Http_PartAddContentDspParm(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZCHAR *pcAttr, ZCHAR *pcVal);

/* http message content description to part */
ZFUNC ZINT Http_PartAddContentDes(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZCHAR *pcDes);

/* http message content transfer encoding head to part */
ZFUNC ZINT Http_PartAddContentTranEncoding(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZCHAR *pcCode);

/* http message content range head to part */
ZFUNC ZINT Http_PartAddContentRange(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZUINT iStartOffset, ZUINT iStopOffset, 
                ZUINT iTotalSize);

/* http message content length head to part */
ZFUNC ZINT Http_PartAddContentLength(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZUINT iSendSize);

/* http message body string to part */
ZFUNC ZINT Http_PartAddBody(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ST_ZOS_SSTR *pstBodyStr);

/* http message body buffer to part */
ZFUNC ZINT Http_PartAddBodyX(ST_HTTP_MSG *pstMsg, 
                ST_HTTP_BODY_PART *pstPart, ZDBUF zBodyBuf);

/* http message body file to part */
ZFUNC ZINT Http_PartAddBodyF(ST_HTTP_MSG *pstMsg, ST_HTTP_BODY_PART *pstPart, 
                ZCHAR *pcFileName);

/* http message find connection */
ZFUNC ZBOOL Http_MsgFindConn(ST_HTTP_MSG *pstMsg, ZCHAR *pcConn);

/* http parameter fill digest response of user name */
ZFUNC ZINT Http_ParmFillDRspUserName(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstUser);

/* http parameter fill digest response of realm */
ZFUNC ZINT Http_ParmFillDRspRealm(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstRealm);

/* http parameter fill digest response of nonce */
ZFUNC ZINT Http_ParmFillDRspNonce(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstNonce);

/* http parameter fill digest response of digest uri */
ZFUNC ZINT Http_ParmFillDRspUri(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, 
                ST_HTTP_REQ_URI *pstDigestUri);

/* http parameter fill digest response of response */
ZFUNC ZINT Http_ParmFillDRspRsp(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcRspStr);

/* http parameter fill digest response of opaque */
ZFUNC ZINT Http_ParmFillDRspOpaque(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstOpaque);

/* http parameter fill digest response of cnonce */
ZFUNC ZINT Http_ParmFillDRspCnonce(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstCnonce);

/* http parameter fill digest response of nonce-count */
ZFUNC ZINT Http_ParmFillDRspNonceCount(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstNonceCount);

/* http parameter fill digest response of qop */
ZFUNC ZINT Http_ParmFillDRspQop(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_HTTP_TYPE_VAL *pstQop);

/* http parameter fill digest response of algorithm */
ZFUNC ZINT Http_ParmFillDRspAlgo(ZDBUF zMemBuf, 
                ST_HTTP_DIGEST_RSP *pstDigestRsp, ST_HTTP_ALGO *pstAlgo);

/* http parameter digest-response list add */
ZFUNC ZINT Http_ParmDRspLstAdd(ZDBUF zMemBuf,
                ST_HTTP_DIG_RSP_LST *pstRspLst, ZUCHAR ucType,
                ST_HTTP_DIG_RSP **ppstRsp);

/* http parameter digest-response list remove */
ZFUNC ZINT Http_ParmDRspLstRmv(ST_HTTP_DIG_RSP_LST *pstRspLst, 
                ST_HTTP_DIG_RSP *pstRsp);

/* http parameter fill credentials based on challenge */
ZFUNC ZINT Http_ParmFillCredents(ZDBUF zMemBuf, 
                ST_HTTP_CREDENTS *pstCredents, ZUCHAR ucMethod, 
                ST_HTTP_CHALLENGE *pstChallenge, ST_ZOS_SSTR *pstUser, 
                ST_ZOS_SSTR *pstPass, ST_HTTP_REQ_URI *pstDigestUri);

/* http calculate A1 */
ZFUNC ZINT Http_ParmCalcA1(ST_ZOS_SSTR *pstUsername, ST_ZOS_SSTR *pstPassword,
                ST_ZOS_SSTR *pstRealm, ZCHAR acResultStr[HTTP_MD5_HEX_STR_LEN]);

/* http calculate A2 */
ZFUNC ZINT Http_ParmCalcA2(ZUCHAR ucMethod, ST_HTTP_REQ_URI *pstDigestUri,
                ZCHAR acResultStr[HTTP_MD5_HEX_STR_LEN]);

/* http calculate KD */
ZFUNC ZINT Http_ParmCalcKd(ZCHAR acA1Md5Str[HTTP_MD5_HEX_STR_LEN],
                ZCHAR acA2Md5Str[HTTP_MD5_HEX_STR_LEN], ST_ZOS_SSTR *pstNonce,
                ST_ZOS_SSTR *pstCnonce, ST_ZOS_SSTR *pstNonceCount,
                ST_ZOS_SSTR *pstQop, ZCHAR acResultStr[HTTP_MD5_HEX_STR_LEN]);

/* http get method description */
ZFUNC ZCHAR * Http_GetMethodDesc(ZUCHAR ucMethod);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_MSG_UTIL_H__ */

