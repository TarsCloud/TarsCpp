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
  File name     : sip_abnf_decode.h
  Module        : sip abnf decode
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-05-16
  Description   :
    Marcos and structure definitions required by the sip abnf decode.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_ABNF_DECODE_H__
#define _SIP_ABNF_DECODE_H__

/**
 * @file
 * @brief SIP ABNF decode interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief The result of the sip decode is failed. */
#define SIP_DECODE_RETFAIL(_msg, _info)   do { \
    ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
    return ZFAILED; \
} while (0)    

/** @brief The result of the sip decode checking. */
#define SIP_DECODE_CHKRET(_msg, _ret, _info) do { \
    if (_ret != ZOK) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/** @brief The sip decode checks memory. */
#define SIP_DECODE_CHKMEM(_msg, _mem, _info) do { \
    if (_mem == ZNULL) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/** @brief The sip decode checks token result. */
#define SIP_DECODE_CHKTKNID(_msg, _tknid, _info) do { \
    if (_tknid == ABNF_TKNID_UNKNOWN) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/*lint -save -e* */

/** @brief The sip sets use LWSP as valid character in the string. */
#define SIP_SET_USELWSP(_msg) ABNF_SET_USELWSP(_msg)

/** @brief The sip clears use LWSP as valid character in the string. */
#define SIP_CLR_USELWSP(_msg) ABNF_CLR_USELWSP(_msg)

/** @brief The sip sets use WSP as valid character in the string. */
#define SIP_SET_USEWSP(_msg) ABNF_SET_USEWSP(_msg)

/** @brief The sip clears use WSP as valid character in the string. */
#define SIP_CLR_USEWSP(_msg) ABNF_CLR_USEWSP(_msg)

/** @brief The sip gets one character. */
#define SIP_GET_CHR(_msg, _pchr, _ret) \
    _ret = Abnf_GetChr(_msg, _pchr)

/** @brief Get a string(length can be zero). */
#define SIP_GET_SSTR_CHRSET(_msg, _class, _pststr, _ret) \
    _ret = Abnf_GetSStrChrset(_msg, Sip_ChrsetGetId(), _class, _pststr)

/* get a string(length can be zero) */
#define SIP_GET_SSTR_ESCAPE(_msg, _class, _echr, _eclass, _pststr, _ret) \
    _ret = Abnf_GetSStrEscape(_msg, Sip_ChrsetGetId(), _class, _echr, _eclass, \
                              _pststr)

/* get a string(length can be zero) */
#define SIP_GET_SSTR_SEPAS(_msg, _chr, _pststr, _ret) \
    _ret = Abnf_GetSStrSepas(_msg, _chr, 0, 0, 0, _pststr)

/* get a string(length can't be zero) */
#define SIP_GET_NSSTR_CHRSET(_msg, _class, _pststr, _ret) \
    _ret = Abnf_GetNSStrChrset(_msg, Sip_ChrsetGetId(), _class, \
                               1, ABNF_MAX_LEN_NA, _pststr)

/* get a string(length can't be zero) */
#define SIP_GET_NSSTR_ESCAPE(_msg, _class, _echr, _eclass, _pststr, _ret) \
    _ret = Abnf_GetNSStrEscape(_msg, Sip_ChrsetGetId(), _class, _echr, _eclass, \
                               1, ABNF_MAX_LEN_NA, _pststr)

/* get a token string */
#define SIP_GET_TKNSSTR(_msg, _pststr, _ret) \
    _ret = Abnf_GetSStrChrset(_msg, Sip_ChrsetGetId(), SIP_TKN, _pststr)

/** @brief The sip gets the scanned string. */
#define SIP_GET_SCANNED_STR(_msg, _pstr) ABNF_GET_SCANNED_STR(_msg, _pstr) 

/** @brief The sip gets the scanning(the left message to be scanned) string. */
#define SIP_GET_SCANNING_STR(_msg, _pstr) ABNF_GET_SCANNING_STR(_msg, _pstr) 

/** @brief The sip gets the scanning(the left message to be scanned) string. */
#define SIP_GET_SCANNING_STRL(_msg, _pstr) Abnf_GetScanningStrL(_msg, _pstr) 

/** @brief Get unsigned long decimal data. */
#define SIP_GET_UIDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUiDigit(_msg, _pdata)

/** @brief Get unsigned short decimal data. */
#define SIP_GET_USDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUsDigit(_msg, _pdata)

/** @brief Get unsigned char decimal data. */
#define SIP_GET_UCDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUcDigit(_msg, _pdata)

/** @brief The sip expects to match one character. */
#define SIP_EXPECT_CHR(_msg, _chr, _case, _ret) \
    _ret = Abnf_ExpectChr(_msg, _chr, _case)

/** @brief The sip trys to expect to match one character. */
#define SIP_TRY_EXPECT_CHR(_msg, _chr, _case, _ret) \
    _ret = Abnf_TryExpectChr(_msg, _chr, _case)

/** @brief The sip decode expects eol(end of line). */
#define SIP_EXPECT_EOL(_msg, _ret) _ret = Abnf_ExpectCRLF(_msg)

/** @brief The sip decode trys to expect eol(end of line). */
#define SIP_TRY_EXPECT_EOL(_msg, _ret) _ret = Abnf_TryExpectCRLF(_msg)

/** @brief The sip expects LWS. */
#define SIP_EXPECT_LWS(_msg, _ret) _ret = Abnf_ExpectLWS(_msg)

/** @brief The sip trys to expect LWS. */
#define SIP_TRY_EXPECT_LWS(_msg, _ret) _ret = Abnf_TryExpectLWS(_msg)

/** @brief The sip decode ignores WSP = SP / HTAB. */
#define SIP_IGNORE_WSP(_msg, _ret) _ret = Abnf_IgnWS(_msg)

/** @brief The sip decode ignores LWS = [*WSP CRLF] 1*WSP. */
#define SIP_IGNORE_LWS(_msg, _ret) _ret = Abnf_IgnLWS(_msg)

/*lint -restore */

/** @brief The sip decode expects SWS = [LWS]. */
#define SIP_EXPECT_SWS(_msg, _ret) SIP_IGNORE_LWS(_msg, _ret)

/** @brief The sip decode expects SP. */
#define SIP_EXPECT_SP(_msg, _ret) SIP_EXPECT_CHR(_msg, ' ', ZTRUE, _ret)

/** @brief The sip decode expects HCOLON  =  *( SP / HTAB ) ":" SWS. */
#define SIP_EXPECT_HCOLON(_msg, _ret) \
    _ret = Sip_DecodeSepaHColon(_msg)

/** @brief The sip decode expects STAR = SWS "*" SWS ; asterisk. */
#define SIP_EXPECT_STAR(_msg, _ret) \
    _ret = Sip_DecodeSepaStar(_msg, ZFALSE)

/** @brief The sip decode expects SLASH = SWS "/" SWS ; slash. */
#define SIP_EXPECT_SLASH(_msg, _ret) \
    _ret = Sip_DecodeSepaSlash(_msg, ZFALSE)

/** @brief The sip decode expects EQUAL = SWS "=" SWS ; equal. */
#define SIP_EXPECT_EQUAL(_msg, _ret) \
    _ret = Sip_DecodeSepaEqual(_msg, ZFALSE)

/** @brief The sip decode expects LPAREN = SWS "(" SWS ; left parenthesis. */
#define SIP_EXPECT_LPAREN(_msg, _ret) \
    _ret = Sip_DecodeSepaLparen(_msg, ZFALSE)

/** @brief The sip decode expects RPAREN = SWS ")" SWS ; right parenthesis. */
#define SIP_EXPECT_RPAREN(_msg, _ret) \
    _ret = Sip_DecodeSepaRparen(_msg, ZFALSE)

/** @brief The sip decode expects RAQUOT = ">" SWS ; right angle quote. */
#define SIP_EXPECT_RAQUOT(_msg, _ret) \
    _ret = Sip_DecodeSepaRaquot(_msg, ZFALSE)

/* sip decode RAQUOT = ">" SWS ; right angle quote */
#define SIP_TRY_EXPECT_RAQUOT(_msg, _ret) \
    _ret = Sip_DecodeSepaRaquot(_msg, ZTRUE)

/** @brief The sip decode expects LAQUOT = SWS "<" ; left angle quote. */
#define SIP_EXPECT_LAQUOT(_msg, _ret) \
    _ret = Sip_DecodeSepaLaquot(_msg, ZFALSE)

/* sip try decode LAQUOT = SWS "<" ; left angle quote */
#define SIP_TRY_EXPECT_LAQUOT(_msg, _ret) \
    _ret = Sip_DecodeSepaLaquot(_msg, ZTRUE)

/** @brief The sip decode expects COMMA = SWS "," SWS ; comma. */
#define SIP_EXPECT_COMMA(_msg, _ret) \
    _ret = Sip_DecodeSepaComma(_msg, ZFALSE)

/** @brief The sip decode expects SEMI = SWS ";" SWS ; semicolon. */
#define SIP_EXPECT_SEMI(_msg, _ret) \
    _ret = Sip_DecodeSepaSemi(_msg, ZFALSE)

/** @brief The sip decode expects COLON = SWS ":" SWS ; colon. */
#define SIP_EXPECT_COLON(_msg, _ret) \
    _ret = Sip_DecodeSepaColon(_msg, ZFALSE)

/** @brief The sip decode expects LDQUOT = SWS DQUOTE; open double quotation
 *         mark.
 */
#define SIP_EXPECT_LDQUOT(_msg, _ret) \
    _ret = Sip_DecodeSepaLdquot(_msg, ZFALSE)

/** @brief The sip decode expects RDQUOT = DQUOTE SWS ; close double quotation
 *         mark.
 */
#define SIP_EXPECT_RDQUOT(_msg, _ret) \
    _ret = Sip_DecodeSepaRdquot(_msg, ZFALSE)

/** @brief The sip decode get an ipv4 in the buffer. */
#define SIP_GET_IPV4(_msg, _ip, _ret) _ret = Abnf_GetIpV4(_msg, _ip)

/** @brief The sip decode get an ipv6 in the buffer. */
#define SIP_GET_IPV6(_msg, _ipv6, _ret) _ret = Abnf_GetIpV6(_msg, _ipv6)

/** 
 * @brief SIP decodes message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSipMsg SIP message is the result of a decoded ABNF message.
 *
 * @return ZOK SIP decodes message successfully.
 * @return ZOK SIP decodes message failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeMsg(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_MSG *pstSipMsg);

/**
 * @brief SIP decodes request line.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstReqLine SIP request line structure.
 *
 * @return ZOK SIP decodes request line successfully.
 * @return ZOK SIP decodes request line failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeReqLine(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REQ_LINE *pstReqLine);

/**
 * @brief SIP decodes status line.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstStatusLine SIP status line structure.
 *
 * @return ZOK SIP decodes status line successfully.
 * @return ZOK SIP decodes status line failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeStatusLine(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_STATUS_LINE *pstStatusLine);

/**
 * @brief SIP decodes version.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVer SIP version structure.
 *
 * @return ZOK SIP decodes version successfully.
 * @return ZOK SIP decodes version failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeVer(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_VER *pstVer);

/**
 * @brief SIP decodes body.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdrLst SIP massage header list structure.
 * @param [out] pstBody SIP body structure.
 *
 * @return ZOK SIP decodes body successfully.
 * @return ZOK SIP decodes body failed.
 *
 * @see Sip_DecodeBodyX, Sip_DecodeBodyMpartLst, Sip_DecodeBodyMpartLstX,
 *      Sip_DecodeBodyMpart, Sip_DecodeBodySpart, Sip_DecodeBodySpartX,
 *      Sip_DecodeBodySfrag
 */
ZFUNC ZINT Sip_DecodeBody(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ST_SIP_BODY *pstBody);

/**
 * @brief SIP decodes body.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] ucMtype MIME type, @ref EN_SIP_MTYPE.
 * @param [in] ucMSubtype MIME subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstBoundary Boundary string.
 * @param [out] pstBody SIP body structure.
 *
 * @return ZOK SIP decodes body successfully.
 * @return ZOK SIP decodes body failed.
 *
 * @see Sip_DecodeBody, Sip_DecodeBodyMpartLst, Sip_DecodeBodyMpartLstX,
 *      Sip_DecodeBodyMpart, Sip_DecodeBodySpart, Sip_DecodeBodySpartX,
 *      Sip_DecodeBodySfrag
 */
ZFUNC ZINT Sip_DecodeBodyX(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_BODY *pstBody,
                ZUCHAR ucMtype, ZUCHAR ucMSubtype, ST_ZOS_SSTR *pstBoundary);

/**
 * @brief SIP decodes body mutlipart list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstContentType SIP header content type structure.
 * @param [out] pstMpartLst SIP body with mutlipart list.
 *
 * @return ZOK SIP decodes body mutlipart list successfully.
 * @return ZOK SIP decodes body mutlipart list failed.
 *
 * @see Sip_DecodeBody, Sip_DecodeBodyX, Sip_DecodeBodyMpartLstX,
 *      Sip_DecodeBodyMpart, Sip_DecodeBodySpart, Sip_DecodeBodySpartX,
 *      Sip_DecodeBodySfrag
 */
ZFUNC ZINT Sip_DecodeBodyMpartLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_MPART_LST *pstMpartLst, 
                ST_SIP_HDR_CONTENT_TYPE *pstContentType);

/**
 * @brief SIP decodes body with mutlipart list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstBoundary Boundary string.
 * @param [out] pstMpartLst SIP body with mutlipart list.
 *
 * @return ZOK SIP decodes body mutlipart list successfully.
 * @return ZOK SIP decodes body mutlipart list failed.
 *
 * @see Sip_DecodeBody, Sip_DecodeBodyX, Sip_DecodeBodyMpartLst,
 *      Sip_DecodeBodyMpart, Sip_DecodeBodySpart, Sip_DecodeBodySpartX,
 *      Sip_DecodeBodySfrag
 */
ZFUNC ZINT Sip_DecodeBodyMpartLstX(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_MPART_LST *pstMpartLst, 
                ST_ZOS_SSTR *pstBoundary);

/**
 * @brief SIP decodes body with mutlipart.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMpart SIP body with mutlipart stucture.
 *
 * @return ZOK SIP decodes body mutlipart successfully.
 * @return ZOK SIP decodes body mutlipart failed.
 *
 * @see Sip_DecodeBody, Sip_DecodeBodyX, Sip_DecodeBodyMpartLst,
 *      Sip_DecodeBodyMpartLstX, Sip_DecodeBodySpart, Sip_DecodeBodySpartX,
 *      Sip_DecodeBodySfrag
 */
ZFUNC ZINT Sip_DecodeBodyMpart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_MPART *pstMpart);

/**
 * @brief SIP decodes body single part.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstContentType SIP header content type.
 * @param [out] pstSpart SIP body with single part stucture.
 *
 * @return ZOK SIP decodes body single part successfully.
 * @return ZOK SIP decodes body single part failed.
 *
 * @see Sip_DecodeBody, Sip_DecodeBodyX, Sip_DecodeBodyMpartLst,
 *      Sip_DecodeBodyMpartLstX, Sip_DecodeBodyMpart, Sip_DecodeBodySpartX,
 *      Sip_DecodeBodySfrag
 */
ZFUNC ZINT Sip_DecodeBodySpart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_SPART *pstSpart,
                ST_SIP_HDR_CONTENT_TYPE *pstContentType);

/**
 * @brief SIP decodes body single part.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] ucMSubtype MIME subtype, @ref EN_SIP_MSUBTYPE.
 * @param [out] pstSpart SIP body with single part stucture.
 *
 * @return ZOK SIP decodes body single part successfully.
 * @return ZOK SIP decodes body single part failed.
 *
 * @see Sip_DecodeBody, Sip_DecodeBodyX, Sip_DecodeBodyMpartLst,
 *      Sip_DecodeBodyMpartLstX, Sip_DecodeBodyMpart, Sip_DecodeBodySpart,
 *      Sip_DecodeBodySfrag
 */
ZFUNC ZINT Sip_DecodeBodySpartX(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_SPART *pstSpart, ZUCHAR ucMSubtype);

/**
 * @brief SIP decodes body sipfrag.
 *
 * @param [in] pstDataStr Signed structure data string.
 * @param [in] zMemBuf Zos data buffer.
 * @param [in] pstErrInfo ABNF error infomation.
 * @param [out] pstSfrag SIP body sipfrag. Detail infomation refers rfc3420.
 *
 * @return ZOK SIP decodes body sipfrag successfully.
 * @return ZOK SIP decodes body sipfrag failed.
 *
 * @see Sip_DecodeBody, Sip_DecodeBodyX, Sip_DecodeBodyMpartLst,
 *      Sip_DecodeBodyMpartLstX, Sip_DecodeBodyMpart, Sip_DecodeBodySpart,
 *      Sip_DecodeBodySpartX
 */
ZFUNC ZINT Sip_DecodeBodySfrag(ST_ZOS_SSTR *pstDataStr, 
                ZDBUF zMemBuf, ST_ABNF_ERR_INFO *pstErrInfo,
                ST_SIP_BODY_SFRAG *pstSfrag);

/**
 * @brief SIP decodes message header list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdrLst SIP message header list.
 *
 * @return ZOK SIP decodes message header list successfully.
 * @return ZOK SIP decodes message header list failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeMsgHdrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR_LST *pstHdrLst);

/**
 * @brief SIP decodes Accept header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Accept header successfully.
 * @return ZOK SIP decodes Accept header failed.
 *
 * @see Sip_DecodeHdrAcptEncoding, Sip_DecodeHdrAcptLang
 */
ZFUNC ZINT Sip_DecodeHdrAcpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Accept-Encoding header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Accept-Encoding header successfully.
 * @return ZOK SIP decodes Accept-Encoding header failed.
 *
 * @see Sip_DecodeHdrAcpt, Sip_DecodeHdrAcptLang
 */
ZFUNC ZINT Sip_DecodeHdrAcptEncoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Accept-Language header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Accept-Language header successfully.
 * @return ZOK SIP decodes Accept-Language header failed.
 *
 * @see Sip_DecodeHdrAcpt, Sip_DecodeHdrAcptEncoding
 */
ZFUNC ZINT Sip_DecodeHdrAcptLang(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Alert-Info header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Alert-Info header successfully.
 * @return ZOK SIP decodes Alert-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrAlertInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Allow header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Allow header successfully.
 * @return ZOK SIP decodes Allow header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrAllow(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Authentication-Info header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Authentication-Info header successfully.
 * @return ZOK SIP decodes Authentication-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrAuthenInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Authorization header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Authorization header successfully.
 * @return ZOK SIP decodes Authorization header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrAuthor(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Call-ID header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Call-ID header successfully.
 * @return ZOK SIP decodes Call-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrCallId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Call-Info header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Call-Info header successfully.
 * @return ZOK SIP decodes Call-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrCallInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Contact header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Contact header successfully.
 * @return ZOK SIP decodes Contact header failed.
 *
 * @see Sip_DecodeHdrContentDisp, Sip_DecodeHdrContentEncoding,
 *      Sip_DecodeHdrContentLang, Sip_DecodeHdrContentLen,
 *      Sip_DecodeHdrContentType
 */
ZFUNC ZINT Sip_DecodeHdrContact(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Content-Disposition header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Content-Disposition header successfully.
 * @return ZOK SIP decodes Content-Disposition header failed.
 *
 * @see Sip_DecodeHdrContact, Sip_DecodeHdrContentEncoding,
 *      Sip_DecodeHdrContentLang, Sip_DecodeHdrContentLen,
 *      Sip_DecodeHdrContentType
 */
ZFUNC ZINT Sip_DecodeHdrContentDisp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Content-Encoding header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Content-Encoding header successfully.
 * @return ZOK SIP decodes Content-Encoding header failed.
 *
 * @see Sip_DecodeHdrContact, Sip_DecodeHdrContentDisp,
 *      Sip_DecodeHdrContentLang, Sip_DecodeHdrContentLen,
 *      Sip_DecodeHdrContentType
 */
ZFUNC ZINT Sip_DecodeHdrContentEncoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Content-Lang header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Content-Lang header successfully.
 * @return ZOK SIP decodes Content-Lang header failed.
 *
 * @see Sip_DecodeHdrContact, Sip_DecodeHdrContentDisp,
 *      Sip_DecodeHdrContentEncoding, Sip_DecodeHdrContentLen,
 *      Sip_DecodeHdrContentType
 */
ZFUNC ZINT Sip_DecodeHdrContentLang(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Content-Length header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Content-Length header successfully.
 * @return ZOK SIP decodes Content-Length header failed.
 *
 * @see Sip_DecodeHdrContact, Sip_DecodeHdrContentDisp,
 *      Sip_DecodeHdrContentEncoding, Sip_DecodeHdrContentLang,
 *      Sip_DecodeHdrContentType
 */
ZFUNC ZINT Sip_DecodeHdrContentLen(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Content-Type header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Content-Type header successfully.
 * @return ZOK SIP decodes Content-Type header failed.
 *
 * @see Sip_DecodeHdrContact, Sip_DecodeHdrContentDisp,
 *      Sip_DecodeHdrContentEncoding, Sip_DecodeHdrContentLang,
 *      Sip_DecodeHdrContentLen
 */
ZFUNC ZINT Sip_DecodeHdrContentType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes CSeq header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes CSeq header successfully.
 * @return ZOK SIP decodes CSeq header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrCseq(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Date header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Date header successfully.
 * @return ZOK SIP decodes Date header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrDate(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Error-Info header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Error-Info header successfully.
 * @return ZOK SIP decodes Error-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrErrorInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Expires header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Expires header successfully.
 * @return ZOK SIP decodes Expires header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrExpires(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes From or To header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes From or To header successfully.
 * @return ZOK SIP decodes From or To header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrFromTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes In-Reply-To header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes In-Reply-To header successfully.
 * @return ZOK SIP decodes In-Reply-To header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrInReplyTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Max-Forwards header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Max-Forwards header successfully.
 * @return ZOK SIP decodes Max-Forwards header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrMaxForwards(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes MIME-Version header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes MIME-Version header successfully.
 * @return ZOK SIP decodes MIME-Version header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrMimeVer(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Min-Expires header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes MIME-Expires header successfully.
 * @return ZOK SIP decodes MIME-Expires header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrMinExpires(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Organization header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Organization header successfully.
 * @return ZOK SIP decodes Organization header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrOrganization(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Priority header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Priority header successfully.
 * @return ZOK SIP decodes Priority header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPriority(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Proxy-Authenticate header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Proxy-Authenticate header successfully.
 * @return ZOK SIP decodes Proxy-Authenticate header failed.
 *
 * @see Sip_DecodeHdrProxyAuthor, Sip_DecodeHdrProxyRequire
 */
ZFUNC ZINT Sip_DecodeHdrProxyAuthen(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Proxy-Authorization  header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Proxy-Authorization  header successfully.
 * @return ZOK SIP decodes Proxy-Authorization  header failed.
 *
 * @see Sip_DecodeHdrProxyAuthen, Sip_DecodeHdrProxyRequire
 */
ZFUNC ZINT Sip_DecodeHdrProxyAuthor(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Proxy-Require  header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Proxy-Require  header successfully.
 * @return ZOK SIP decodes Proxy-Require  header failed.
 *
 * @see Sip_DecodeHdrProxyAuthen, Sip_DecodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_DecodeHdrProxyRequire(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Proxy-Require  header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Proxy-Require  header successfully.
 * @return ZOK SIP decodes Proxy-Require  header failed.
 *
 * @see Sip_DecodeHdrProxyAuthen, Sip_DecodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_DecodeHdrReplyTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Proxy-Require  header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Proxy-Require  header successfully.
 * @return ZOK SIP decodes Proxy-Require  header failed.
 *
 * @see Sip_DecodeHdrProxyAuthen, Sip_DecodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_DecodeHdrRequire(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Retry-After header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Retry-After header successfully.
 * @return ZOK SIP decodes Retry-After header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrRetryAfter(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Route header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Route header successfully.
 * @return ZOK SIP decodes Route header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrRoute(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Server header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Server header successfully.
 * @return ZOK SIP decodes Server header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrServer(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Subject header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Subject header successfully.
 * @return ZOK SIP decodes Subject header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrSubject(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Supported header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Supported header successfully.
 * @return ZOK SIP decodes Supported header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrSupted(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Timestamp header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Timestamp header successfully.
 * @return ZOK SIP decodes Timestamp header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrTimestamp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Unsupported header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Unsupported header successfully.
 * @return ZOK SIP decodes Unsupported header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrUnsupted(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes User-Agent header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes User-Agent header successfully.
 * @return ZOK SIP decodes User-Agent header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrUserAgent(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Via header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Via header successfully.
 * @return ZOK SIP decodes Via header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrVia(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Warning header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Warning header successfully.
 * @return ZOK SIP decodes Warning header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrWarn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes WWW-Authenticate header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes WWW-Authenticate header successfully.
 * @return ZOK SIP decodes WWW-Authenticate header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrWwwAuthen(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes RAck header. Detail information refers rfc3262.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes RAck header successfully.
 * @return ZOK SIP decodes RAck header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrRack(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes RSeq header. Detail information refers rfc3262.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes RSeq header successfully.
 * @return ZOK SIP decodes RSeq header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrRseq(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Event header. Detail information refers rfc3265.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Event header successfully.
 * @return ZOK SIP decodes Event header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrEvnt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Allow-Events header. Detail information refers rfc3265.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Allow-Events header successfully.
 * @return ZOK SIP decodes Allow-Events header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrAllowEvnts(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Subscription-State header. Detail information refers
 *        rfc3265.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Subscription-State header successfully.
 * @return ZOK SIP decodes Subscription-State header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrSubsSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Refer-To header. Detail information refers
 *        rfc3515.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Refer-To header successfully.
 * @return ZOK SIP decodes Refer-To header failed.
 *
 * @see Sip_DecodeHdrReferredBy
 */
ZFUNC ZINT Sip_DecodeHdrReferTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Referred-By header. Detail information refers
 *        rfc3892.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Referred-By header successfully.
 * @return ZOK SIP decodes Referred-By header failed.
 *
 * @see Sip_DecodeHdrReferTo
 */
ZFUNC ZINT Sip_DecodeHdrReferredBy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Replaces header. Detail information refers
 *        rfc3891.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Replaces header successfully.
 * @return ZOK SIP decodes Replaces header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrReplaces(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Session-Expires header. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Session-Expires header successfully.
 * @return ZOK SIP decodes Session-Expires header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrSessExpires(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Min-SE header. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Min-SE header successfully.
 * @return ZOK SIP decodes Min-SE header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrMinSe(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Request-Disposition header. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Request-Disposition header successfully.
 * @return ZOK SIP decodes Request-Disposition header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrReqDisp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Accept-Contact eader. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Accept-Contact header successfully.
 * @return ZOK SIP decodes Accept-Contact header failed.
 *
 * @see Sip_DecodeHdrRejContact
 */
ZFUNC ZINT Sip_DecodeHdrAcptContact(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Reject-Contact eader. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Reject-Contact header successfully.
 * @return ZOK SIP decodes Reject-Contact header failed.
 *
 * @see Sip_DecodeHdrAcptContact
 */
ZFUNC ZINT Sip_DecodeHdrRejContact(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Anonymity eader. Detail information refers
 *        draft-ietf-sip-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Anonymity header successfully.
 * @return ZOK SIP decodes Anonymity header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrAnony(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes RPID-Privacy eader. Detail information refers
 *        draft-ietf-sip-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes RPID-Privacy header successfully.
 * @return ZOK SIP decodes RPID-Privacy header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrRpidPrivacy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Remote-Party-ID eader. Detail information refers
 *        draft-ietf-sip-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Remote-Party-ID header successfully.
 * @return ZOK SIP decodes Remote-Party-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrRmtPartyId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes SIP-ETag header. Detail information refers rfc3911.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes SIP-ETag header successfully.
 * @return ZOK SIP decodes SIP-ETag header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrSipEtag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes SIP-If-Match header. Detail information refers rfc3911.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes SIP-If-Match header successfully.
 * @return ZOK SIP decodes SIP-If-Match header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrSipIfMatch(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Join header. Detail information refers rfc3911.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Join header successfully.
 * @return ZOK SIP decodes Join header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrJoin(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);
				
/**
 * @brief SIP decodes P-Associated-URI header. Detail information refers rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Associated-URI header successfully.
 * @return ZOK SIP decodes P-Associated-URI header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPAsoUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Called-Party-ID header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Called-Party-ID header successfully.
 * @return ZOK SIP decodes P-Called-Party-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPCalledPtyId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Visited-Network-ID header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Visited-Network-ID header successfully.
 * @return ZOK SIP decodes P-Visited-Network-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPVnetId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Access-Network-Info header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Access-Network-Info header successfully.
 * @return ZOK SIP decodes P-Access-Network-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPAnetInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Charging-Addr header. Detail information refers rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Charging-Addr header successfully.
 * @return ZOK SIP decodes P-Charging-Addr header failed.
 *
 * @see Sip_DecodeHdrPChargVec
 */
ZFUNC ZINT Sip_DecodeHdrPChargAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Charging-Vector header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Charging-Vector header successfully.
 * @return ZOK SIP decodes P-Charging-Vector header failed.
 *
 * @see Sip_DecodeHdrPChargAddr
 */
ZFUNC ZINT Sip_DecodeHdrPChargVec (ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Asserted-Identity header. Detail information refers
 *        rfc3325.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Asserted-Identity header successfully.
 * @return ZOK SIP decodes P-Asserted-Identity header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPAssertedId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Preferred-Identity header. Detail information refers
 *        rfc3325.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Preferred-Identity header successfully.
 * @return ZOK SIP decodes P-Preferred-Identity header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPPreferredId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Media-Authorization header. Detail information refers
 *        rfc3313.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Media-Authorization header successfully.
 * @return ZOK SIP decodes P-Media-Authorization header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPMediaAuthor(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Privacy header. Detail information refers rfc3323.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Privacy header successfully.
 * @return ZOK SIP decodes Privacy header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPrivacy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Reason header. Detail information refers rfc3326.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Reason header successfully.
 * @return ZOK SIP decodes Reason header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrReason(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes security-client header. Detail information refers rfc3329.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes security-client header successfully.
 * @return ZOK SIP decodes security-client header failed.
 *
 * @see Sip_DecodeHdrSecServ, Sip_DecodeHdrSecVerify
 */
ZFUNC ZINT Sip_DecodeHdrSecCli(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes security-server header. Detail information refers rfc3329.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes security-server header successfully.
 * @return ZOK SIP decodes security-server header failed.
 *
 * @see Sip_DecodeHdrSecCli, Sip_DecodeHdrSecVerify
 */
ZFUNC ZINT Sip_DecodeHdrSecServ(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes security-verify header. Detail information refers rfc3329.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes security-verify header successfully.
 * @return ZOK SIP decodes security-verify header failed.
 *
 * @see Sip_DecodeHdrSecCli, Sip_DecodeHdrSecServ
 */
ZFUNC ZINT Sip_DecodeHdrSecVerify(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-DCS-Trace-Party-ID header. Detail information refers
 *        rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-DCS-Trace-Party-ID header successfully.
 * @return ZOK SIP decodes P-DCS-Trace-Party-ID header failed.
 *
 * @see Sip_DecodeHdrPDcsOsps, Sip_DecodeHdrPDcsBillInfo, Sip_DecodeHdrPDcsLaes
 *      Sip_DecodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_DecodeHdrPDcsTracePtyId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-DCS-OSPS header. Detail information refers rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-DCS-OSPS header successfully.
 * @return ZOK SIP decodes P-DCS-OSPS header failed.
 *
 * @see Sip_DecodeHdrPDcsTracePtyId, Sip_DecodeHdrPDcsBillInfo,
 *      Sip_DecodeHdrPDcsLaes, Sip_DecodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_DecodeHdrPDcsOsps(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-DCS-Billing-Info header. Detail information refers
 *        rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-DCS-Billing-Info header successfully.
 * @return ZOK SIP decodes P-DCS-Billing-Info header failed.
 *
 * @see Sip_DecodeHdrPDcsTracePtyId, Sip_DecodeHdrPDcsOsps,
 *      Sip_DecodeHdrPDcsLaes, Sip_DecodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_DecodeHdrPDcsBillInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-DCS-LAES header. Detail information refers rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-DCS-LAES header successfully.
 * @return ZOK SIP decodes P-DCS-LAES header failed.
 *
 * @see Sip_DecodeHdrPDcsTracePtyId, Sip_DecodeHdrPDcsOsps,
 *      Sip_DecodeHdrPDcsBillInfo, Sip_DecodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_DecodeHdrPDcsLaes(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-DCS-Redirect header. Detail information refers rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-DCS-Redirect header successfully.
 * @return ZOK SIP decodes P-DCS-Redirect header failed.
 *
 * @see Sip_DecodeHdrPDcsTracePtyId, Sip_DecodeHdrPDcsOsps,
 *      Sip_DecodeHdrPDcsBillInfo, Sip_DecodeHdrPDcsLaes
 */
ZFUNC ZINT Sip_DecodeHdrPDcsRedir(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes History-Info header. Detail information refers rfc4244.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes History-Info header successfully.
 * @return ZOK SIP decodes History-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrHiInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Alerting-Mode header. Detail information refers
 *       draft poc-p-headers.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Alerting-Mode header successfully.
 * @return ZOK SIP decodes P-Alerting-Mode header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPAlertMode(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Answer-State header. Detail information refers
 *       draft-poc-p-headers.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Answer-State header successfully.
 * @return ZOK SIP decodes P-Answer-State header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPAnswerState(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-User-Database header. Detail information refers rfc4457.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-User-Database header successfully.
 * @return ZOK SIP decodes P-User-Database header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPUserDb(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Refer-Sub header. Detail information refers rfc4488.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Refer-Sub header successfully.
 * @return ZOK SIP decodes Refer-Sub header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrReferSub(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Message-ID header. Detail information refers rfc2392.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Message-ID header successfully.
 * @return ZOK SIP decodes Message-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrMsgId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Content-ID header. Detail information refers rfc2392.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Content-ID header successfully.
 * @return ZOK SIP decodes Content-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrContentId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Answer-Mode header. Detail information refers
 *        rfc5373.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Answer-Mode header successfully.
 * @return ZOK SIP decodes Answer-Mode header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrAnswerMode(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Priv-Answer-Mode. Detail information refers
 *        rfc5373.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Priv-Answer-Mode header successfully.
 * @return ZOK SIP decodes Priv-Answer-Mode header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPrivAnswerMode(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Asserted-Service. Detail information refers
 *        draft-drage-sipping-service-identification.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Asserted-Service header successfully.
 * @return ZOK SIP decodes P-Asserted-Service header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPAssertedSrv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Preferred-Service. Detail information refers
 *        draft-drage-sipping-service-identification.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Preferred-Service header successfully.
 * @return ZOK SIP decodes P-Preferred-Service header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrPPreferredSrv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Early-Media.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Early-Media header successfully.
 * @return ZOK SIP decodes P-Early-Media header failed.
 */
ZFUNC ZINT Sip_DecodeHdrPEarlyMedia(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Flow-Timer.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Flow-Timer header successfully.
 * @return ZOK SIP decodes Flow-Timer header failed.
 */
ZFUNC ZINT Sip_DecodeHdrFlowTimer(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Target-Dialog.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Target-Dialog header successfully.
 * @return ZOK SIP decodes Target-Dialog header failed.
 */
ZFUNC ZINT Sip_DecodeHdrTargetDlg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes P-Refused-Uri-List.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes P-Refused-Uri-List header successfully.
 * @return ZOK SIP decodes P-Refused-Uri-List header failed.
 */
ZFUNC ZINT Sip_DecodeHdrPRefusedUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Session-Replaces.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Session-Replaces header successfully.
 * @return ZOK SIP decodes Session-Replaces header failed.
 */
ZFUNC ZINT Sip_DecodeHdrSessReplaces(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Contribution-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Contribution-ID header successfully.
 * @return ZOK SIP decodes Contribution-ID header failed.
 */
ZFUNC ZINT Sip_DecodeHdrContributionID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Conversation-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Conversation-ID header successfully.
 * @return ZOK SIP decodes Conversation-ID header failed.
 */
ZFUNC ZINT Sip_DecodeHdrConversationID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes InReplyTo-Contribution-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes InReplyTo-Contribution-ID header successfully.
 * @return ZOK SIP decodes InReplyTo-Contribution-ID header failed.
 */
ZFUNC ZINT Sip_DecodeIrtContID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes Content-Description.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Content-Description header successfully.
 * @return ZOK SIP decodes Content-Description header failed.
 */
ZFUNC ZINT Sip_DecodeHdrContDescription(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes extension-header. Detail information refers
 *        extension-header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes extension-header header successfully.
 * @return ZOK SIP decodes extension-header header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHdrExtHdr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes request uri.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstReqUri SIP request uri structure.
 *
 * @return ZOK SIP decodes request uri successfully.
 * @return ZOK SIP decodes request uri failed.
 *
 * @see Sip_DecodeReqUriType
 */
ZFUNC ZINT Sip_DecodeReqUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REQ_URI *pstReqUri);

/**
 * @brief SIP decodes request uri type.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pucType Request type, @ref EN_SIP_REQ_URI.
 *
 * @return ZOK SIP decodes request uri type successfully.
 * @return ZOK SIP decodes request uri type failed.
 *
 * @see Sip_DecodeReqUri
 */
ZFUNC ZINT Sip_DecodeReqUriType(ST_ABNF_MSG *pstAbnfMsg, 
                ZUCHAR *pucType);

/**
 * @brief SIP decodes accept-tange list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRangeLst SIP accept-tange list.
 *
 * @return ZOK SIP decodes accept-tange list successfully.
 * @return ZOK SIP decodes accept-tange list failed.
 *
 * @see Sip_DecodeAcptRange
 */
ZFUNC ZINT Sip_DecodeAcptRangeLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_RANGE_LST *pstRangeLst);

/**
 * @brief SIP decodes accept-tange.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRange SIP accept-tange.
 *
 * @return ZOK SIP decodes accept-tange successfully.
 * @return ZOK SIP decodes accept-tange failed.
 *
 * @see Sip_DecodeAcptRangeLst
 */
ZFUNC ZINT Sip_DecodeAcptRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_RANGE *pstRange);

/**
 * @brief SIP decodes media-range.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMediaRange SIP media-range.
 *
 * @return ZOK SIP decodes media-range successfully.
 * @return ZOK SIP decodes media-range failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeMediaRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MEDIA_RANGE *pstMediaRange);

/**
 * @brief SIP decodes accept-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP accept-param list.
 *
 * @return ZOK SIP decodes accept-param list successfully.
 * @return ZOK SIP decodes accept-param list failed.
 *
 * @see Sip_DecodeAcptParm
 */
ZFUNC ZINT Sip_DecodeAcptParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes accept-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP Media-Range.
 *
 * @return ZOK SIP decodes accept-param successfully.
 * @return ZOK SIP decodes accept-param failed.
 *
 * @see Sip_DecodeAcptParmLst
 */
ZFUNC ZINT Sip_DecodeAcptParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_PARM *pstParm);

/**
 * @brief SIP decodes qvalue.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQval SIP qvalue.
 *
 * @return ZOK SIP decodes qvalue successfully.
 * @return ZOK SIP decodes qvalue failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeQval(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_QVAL *pstQval);

/**
 * @brief SIP decodes m-parameter list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP M-Parameter list.
 *
 * @return ZOK SIP decodes m-parameter list successfully.
 * @return ZOK SIP decodes v list failed.
 *
 * @see Sip_DecodeMParm
 */
ZFUNC ZINT Sip_DecodeMParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MPARM_LST *pstParmLst);

/**
 * @brief SIP decodes m-parameter.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP m-parameter.
 *
 * @return ZOK SIP decodes m-parameter successfully.
 * @return ZOK SIP decodes m-parameter failed.
 *
 * @see Sip_DecodeMParmLst
 */
ZFUNC ZINT Sip_DecodeMParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MPARM *pstParm);

/**
 * @brief SIP decodes encoding list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEncodingLst SIP encoding list.
 *
 * @return ZOK SIP decodes encoding list successfully.
 * @return ZOK SIP decodes encoding list failed.
 *
 * @see Sip_DecodeMParmLst
 */
ZFUNC ZINT Sip_DecodeEncodingLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ENCODING_LST *pstEncodingLst);

/**
 * @brief SIP decodes encoding.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEncoding SIP Encoding.
 *
 * @return ZOK SIP decodes encoding successfully.
 * @return ZOK SIP decodes encoding failed.
 *
 * @see Sip_DecodeEncodingLst
 */
ZFUNC ZINT Sip_DecodeEncoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ENCODING *pstEncoding);

/**
 * @brief SIP decodes codings.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCodings SIP codings.
 *
 * @return ZOK SIP decodes codings successfully.
 * @return ZOK SIP decodes codings failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeCoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CODINGS *pstCodings);

/**
 * @brief SIP decodes language list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangLst SIP language list.
 *
 * @return ZOK SIP decodes language list successfully.
 * @return ZOK SIP decodes language list failed.
 *
 * @see Sip_DecodeLang
 */
ZFUNC ZINT Sip_DecodeLangLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_LST *pstLangLst);

/**
 * @brief SIP decodes language.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLang SIP language.
 *
 * @return ZOK SIP decodes language successfully.
 * @return ZOK SIP decodes language failed.
 *
 * @see Sip_DecodeLangLst
 */
ZFUNC ZINT Sip_DecodeLang(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_LANG *pstLang);

/**
 * @brief SIP decodes language-range.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRange SIP language-range.
 *
 * @return ZOK SIP decodes language-range successfully.
 * @return ZOK SIP decodes language-range failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeLangRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_RANGE *pstRange);

/**
 * @brief SIP decodes language Subtag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangSubtagLst SIP Language-Range list.
 *
 * @return ZOK SIP decodes language Subtag list successfully.
 * @return ZOK SIP decodes language Subtag list failed.
 *
 * @see Sip_DecodeLangSubtag
 */
ZFUNC ZINT Sip_DecodeLangSubtagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SSTR_LST *pstLangSubtagLst);

/**
 * @brief SIP decodes language Subtag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangSubtag Zos signed structure string.
 *
 * @return ZOK SIP decodes language Subtag successfully.
 * @return ZOK SIP decodes language Subtag failed.
 *
 * @see Sip_DecodeLangSubtagLst
 */
ZFUNC ZINT Sip_DecodeLangSubtag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstLangSubtag);

/**
 * @brief SIP decodes Alert-Param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlertLst SIP Alert-Param list.
 *
 * @return ZOK SIP decodes Alert-Param list successfully.
 * @return ZOK SIP decodes Alert-Param list failed.
 *
 * @see Sip_DecodeAlertParm
 */
ZFUNC ZINT Sip_DecodeAlertParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALERT_PARM_LST *pstAlertLst);

/**
 * @brief SIP decodes alert-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlert SIP Alert-Param.
 *
 * @return ZOK SIP decodes alert-param successfully.
 * @return ZOK SIP decodes alert-param failed.
 *
 * @see Sip_DecodeAlertParmLst
 */
ZFUNC ZINT Sip_DecodeAlertParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALERT_PARM *pstAlert);

/**
 * @brief SIP decodes absolute URI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAbsoUri SIP absolute URI.
 *
 * @return ZOK SIP decodes absolute URI successfully.
 * @return ZOK SIP decodes absolute URI failed.
 *
 * @see
 */

ZFUNC ZINT Sip_DecodeAbsoUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ABSO_URI *pstAbsoUri);

/**
 * @brief SIP decodes generic-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP generic-param list.
 *
 * @return ZOK SIP decodes generic-param list successfully.
 * @return ZOK SIP decodes generic-param list failed.
 *
 * @see Sip_DecodeGenParm
 */
ZFUNC ZINT Sip_DecodeGenParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_GEN_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes generic-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP generic-param.
 *
 * @return ZOK SIP decodes generic-param successfully.
 * @return ZOK SIP decodes generic-param failed.
 *
 * @see Sip_DecodeGenParmLst
 */
ZFUNC ZINT Sip_DecodeGenParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_GEN_PARM *pstParm);

/**
 * @brief SIP decodes generic-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstGenVal SIP v.
 *
 * @return ZOK SIP decodes generic-value successfully.
 * @return ZOK SIP decodes generic-valuess failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeGenValue(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_GEN_VAL *pstGenVal);

/**
 * @brief SIP decodes method list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMethodLst SIP method list.
 *
 * @return ZOK SIP decodes method list successfully.
 * @return ZOK SIP decodes method list failed.
 *
 * @see Sip_DecodeMethod
 */
ZFUNC ZINT Sip_DecodeMethodLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_METHOD_LST *pstMethodLst);

/**
 * @brief SIP decodes method.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMethod SIP method.
 *
 * @return ZOK SIP decodes method successfully.
 * @return ZOK SIP decodes method failed.
 *
 * @see Sip_DecodeMethodLst
 */
ZFUNC ZINT Sip_DecodeMethod(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_METHOD *pstMethod);

/**
 * @brief SIP decodes ainfo list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAinfoLst SIP ainfo list.
 *
 * @return ZOK SIP decodes ainfo list successfully.
 * @return ZOK SIP decodes ainfo list failed.
 *
 * @see Sip_DecodeAinfo
 */
ZFUNC ZINT Sip_DecodeAinfoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AINFO_LST *pstAinfoLst);

/**
 * @brief SIP decodes ainfo.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAinfo SIP ainfo.
 *
 * @return ZOK SIP decodes ainfo successfully.
 * @return ZOK SIP decodes ainfo failed.
 *
 * @see Sip_DecodeAinfoLst
 */
ZFUNC ZINT Sip_DecodeAinfo(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_AINFO *pstAinfo);

/**
 * @brief SIP decodes info list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfoLst SIP info list.
 *
 * @return ZOK SIP decodes info list successfully.
 * @return ZOK SIP decodes info list failed.
 *
 * @see Sip_DecodeInfo
 */
ZFUNC ZINT Sip_DecodeInfoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_LST *pstInfoLst);

/**
 * @brief SIP decodes info.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfo SIP info.
 *
 * @return ZOK SIP decodes info successfully.
 * @return ZOK SIP decodes info failed.
 *
 * @see Sip_DecodeInfoLst
 */
ZFUNC ZINT Sip_DecodeInfo(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_INFO *pstInfo);

/**
 * @brief SIP decodes info-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP info-param list.
 *
 * @return ZOK SIP decodes info-param list successfully.
 * @return ZOK SIP decodes info-param list failed.
 *
 * @see Sip_DecodeInfoParm, Sip_DecodeInfoPurps
 */
ZFUNC ZINT Sip_DecodeInfoParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes info-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP info-param.
 *
 * @return ZOK SIP decodes info-param successfully.
 * @return ZOK SIP decodes info-param failed.
 *
 * @see Sip_DecodeInfoParmLst, Sip_DecodeInfoPurps
 */
ZFUNC ZINT Sip_DecodeInfoParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_PARM *pstParm);

/**
 * @brief SIP decodes info-param purpose.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPurps SIP info-param purpose.
 *
 * @return ZOK SIP decodes info-param purpose successfully.
 * @return ZOK SIP decodes info-param purpose failed.
 *
 * @see Sip_DecodeInfoParmLst, Sip_DecodeInfoParm
 */
ZFUNC ZINT Sip_DecodeInfoPurps(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_PURPS *pstPurps);

/**
 * @brief SIP decodes contact-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP contact-param list.
 *
 * @return ZOK SIP decodes contact-param list successfully.
 * @return ZOK SIP decodes contact-param list failed.
 *
 * @see Sip_DecodeContactParm, Sip_DecodeContactParmsLst,
 *      Sip_DecodeContactParms
 */
ZFUNC ZINT Sip_DecodeContactParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes contact-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP contact-param.
 *
 * @return ZOK SIP decodes contact-param successfully.
 * @return ZOK SIP decodes contact-param failed.
 *
 * @see Sip_DecodeContactParmLst, Sip_DecodeContactParmsLst,
 *      Sip_DecodeContactParms
 */
ZFUNC ZINT Sip_DecodeContactParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP decodes contact-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP contact-params list.
 *
 * @return ZOK SIP decodes contact-params list successfully.
 * @return ZOK SIP decodes contact-params list failed.
 *
 * @see Sip_DecodeContactParmLst, Sip_DecodeContactParm,
 *      Sip_DecodeContactParms
 */
ZFUNC ZINT Sip_DecodeContactParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes contact-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP contact-params.
 *
 * @return ZOK SIP decodes contact-params successfully.
 * @return ZOK SIP decodes contact-params failed.
 *
 * @see Sip_DecodeContactParm, Sip_DecodeContactParmsLst,
 *      Sip_DecodeContactParmsLst
 */
ZFUNC ZINT Sip_DecodeContactParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARMS *pstParms);
ZFUNC ZINT Sip_DecodeCFeatureParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CFEATURE_PARM *pstParm);
ZFUNC ZINT Sip_DecodeTagValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TAG_VAL_LST *pstValLst);
ZFUNC ZINT Sip_DecodeTagVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstTagVal);
ZFUNC ZINT Sip_DecodeStrVal(ST_ABNF_MSG *pstAbnfMsg, ST_ZOS_SSTR *pstVal);

/**
 * @brief SIP decodes name-addr or addr-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstNaSpec SIP name-addr or addr-spec.
 *
 * @return ZOK SIP decodes name-addr or addr-spec successfully.
 * @return ZOK SIP decodes name-addr or addr-spec failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeNaSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_NA_SPEC *pstNaSpec);

/**
 * @brief SIP decodes name-addr.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstNameAddr SIP name-addr.
 *
 * @return ZOK SIP decodes name-addr successfully.
 * @return ZOK SIP decodes name-addr failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeNameAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_NAME_ADDR *pstNameAddr);

/**
 * @brief SIP decodes display-name.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDispName SIP display-name.
 *
 * @return ZOK SIP decodes display-name successfully.
 * @return ZOK SIP decodes display-name failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeDispName(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DISP_NAME *pstDispName);

/**
 * @brief SIP decodes addr-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAddrSpec SIP addr-spec.
 *
 * @return ZOK SIP decodes addr-spec successfully.
 * @return ZOK SIP decodes addr-spec failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeAddrSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ADDR_SPEC *pstAddrSpec);

/**
 * @brief SIP decodes addr-spec type.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pucType SIP addr-addr type, @ref EN_SIP_ADDR_SPEC.
 *
 * @return ZOK SIP decodes addr-spec type successfully.
 * @return ZOK SIP decodes addr-spec type failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeAddrSpecType(ST_ABNF_MSG *pstAbnfMsg, 
                ZUCHAR *pucType);

/**
 * @brief SIP decodes SIP-URI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSipUri SIP SIP-URI.
 *
 * @return ZOK SIP decodes SIP-URI successfully.
 * @return ZOK SIP decodes SIP-URI failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeSipUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP decodes userinfo.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUserInfo SIP userinfo.
 *
 * @return ZOK SIP decodes userinfo successfully.
 * @return ZOK SIP decodes userinfo failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeUserInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_USER_INFO *pstUserInfo);

/**
 * @brief SIP decodes hostport.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHostPort SIP hostport.
 *
 * @return ZOK SIP decodes hostport successfully.
 * @return ZOK SIP decodes hostport failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHostPort(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HOST_PORT *pstHostPort);

/**
 * @brief SIP decodes host.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHost SIP host.
 *
 * @return ZOK SIP decodes host successfully.
 * @return ZOK SIP decodes host failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHost(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_HOST *pstHost);

/**
 * @brief SIP decodes Ipv6 address.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pucIpv6Addr Ipv6 adress.
 *
 * @return ZOK SIP decodes ipv6 address successfully.
 * @return ZOK SIP decodes ipv6 address failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeIpv6(ST_ABNF_MSG *pstAbnfMsg, ZUCHAR *pucIpv6Addr);

/**
 * @brief SIP decodes quoted string.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQStr Zos signed structure string.
 *
 * @return ZOK SIP decodes quoted string successfully.
 * @return ZOK SIP decodes quoted string failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeQStr(ST_ABNF_MSG *pstAbnfMsg, ST_ZOS_SSTR *pstQStr);

/**
 * @brief SIP decodes uri-parameter list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriParmLst SIP uri-parameter list.
 *
 * @return ZOK SIP decodes uri-parameter list successfully.
 * @return ZOK SIP decodes uri-parameter list failed.
 *
 * @see Sip_DecodeUriParm
 */
ZFUNC ZINT Sip_DecodeUriParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_PARM_LST *pstUriParmLst);

/**
 * @brief SIP decodes uri-parameter.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriParm SIP uri-parameter.
 *
 * @return ZOK SIP decodes uri-parameter successfully.
 * @return ZOK SIP decodes uri-parameter failed.
 *
 * @see Sip_DecodeUriParmLst
 */
ZFUNC ZINT Sip_DecodeUriParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_PARM *pstUriParm);

/**
 * @brief SIP decodes headers.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdrs SIP headers.
 *
 * @return ZOK SIP decodes headers successfully.
 * @return ZOK SIP decodes headers failed.
 *
 * @see Sip_DecodeHdrLst, Sip_DecodeHdr
 */
ZFUNC ZINT Sip_DecodeHdrs(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HDRS *pstHdrs);

/**
 * @brief SIP decodes header list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdrLst SIP header list.
 *
 * @return ZOK SIP decodes header list successfully.
 * @return ZOK SIP decodes header list failed.
 *
 * @see Sip_DecodeHdrs, Sip_DecodeHdr
 */
ZFUNC ZINT Sip_DecodeHdrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HDR_LST *pstHdrLst);

/**
 * @brief SIP decodes header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP header.
 *
 * @return ZOK SIP decodes header successfully.
 * @return ZOK SIP decodes header failed.
 *
 * @see Sip_DecodeHdrs, Sip_DecodeHdrLst
 */
ZFUNC ZINT Sip_DecodeHdr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HDR *pstHdr);

/**
 * @brief SIP decodes IM-URI. Detail information refers rfc3860.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstImUri SIP IM-URI.
 *
 * @return ZOK SIP decodes IM-URI successfully.
 * @return ZOK SIP decodes IM-URI failed.
 *
 * @see Sip_DecodeMcUri, Sip_DecodeTelUri
 */
ZFUNC ZINT Sip_DecodeImUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_IM_URI *pstImUri);

/**
 * @brief SIP decodes Message-ID & Content-ID uri. Detail information refers
 *        rfc2392.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMcUri SIP Message-ID & Content-ID uri.
 *
 * @return ZOK SIP decodes Message-ID & Content-ID uri successfully.
 * @return ZOK SIP decodes Message-ID & Content-ID uri failed.
 *
 * @see Sip_DecodeImUri, Sip_DecodeTelUri
 */
ZFUNC ZINT Sip_DecodeMcUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MC_URI *pstMcUri);

/**
 * @brief SIP decodes telephone-uri. Detail information refers rfc2806.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTelUri SIP telephone-uri.
 *
 * @return ZOK SIP decodes telephone-uri successfully.
 * @return ZOK SIP decodes telephone-uri failed.
 *
 * @see Sip_DecodeImUri, Sip_DecodeMcUri
 */
ZFUNC ZINT Sip_DecodeTelUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TEL_URI *pstTelUri);

/**
 * @brief SIP decodes global-number.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstGlobal SIP global-number.
 *
 * @return ZOK SIP decodes global-number successfully.
 * @return ZOK SIP decodes global-number failed.
 *
 * @see Sip_DecodeLocalNum
 */
ZFUNC ZINT Sip_DecodeGlobalNum(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_GLOBAL_NUMBER *pstGlobal);

/**
 * @brief SIP decodes local-number.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLocal SIP local-number.
 *
 * @return ZOK SIP decodes local-number successfully.
 * @return ZOK SIP decodes local-number failed.
 *
 * @see Sip_DecodeGlobalNum
 */
ZFUNC ZINT Sip_DecodeLocalNum(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LOCAL_NUMBER *pstLocal);

/**
 * @brief SIP decodes par list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParLst SIP par list.
 *
 * @return ZOK SIP decodes par list successfully.
 * @return ZOK SIP decodes par list failed.
 *
 * @see Sip_DecodePar
 */
ZFUNC ZINT Sip_DecodeParLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PAR_LST *pstParLst);

/**
 * @brief SIP decodes par.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPar SIP par.
 *
 * @return ZOK SIP decodes par successfully.
 * @return ZOK SIP decodes par failed.
 *
 * @see Sip_DecodeParLst
 */
ZFUNC ZINT Sip_DecodePar(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PAR *pstPar);

/**
 * @brief SIP decodes disp-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP disp-param list.
 *
 * @return ZOK SIP decodes disp-param list successfully.
 * @return ZOK SIP decodes disp-param list failed.
 *
 * @see Sip_DecodeDispParm
 */
ZFUNC ZINT Sip_DecodeDispParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DISP_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes disp-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP disp-param.
 *
 * @return ZOK SIP decodes disp-param successfully.
 * @return ZOK SIP decodes disp-param failed.
 *
 * @see Sip_DecodeDispParmLst
 */
ZFUNC ZINT Sip_DecodeDispParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DISP_PARM *pstParm);

/**
 * @brief SIP decodes handling-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP handling-param.
 *
 * @return ZOK SIP decodes handling-param successfully.
 * @return ZOK SIP decodes handling-param failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeHandParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HAND_PARM *pstParm);

/**
 * @brief SIP decodes content-coding list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCodingLst SIP content-coding list.
 *
 * @return ZOK SIP decodes content-coding list successfully.
 * @return ZOK SIP decodes content-coding list failed.
 *
 * @see Sip_DecodeContentCoding
 */
ZFUNC ZINT Sip_DecodeContentCodingLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTENT_CODING_LST *pstCodingLst);

/**
 * @brief SIP decodes content-coding.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCoding SIP content-coding.
 *
 * @return ZOK SIP decodes content-coding successfully.
 * @return ZOK SIP decodes content-coding failed.
 *
 * @see Sip_DecodeContentCodingLst
 */
ZFUNC ZINT Sip_DecodeContentCoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTENT_CODING *pstCoding);

/**
 * @brief SIP decodes language-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangTagLst SIP language-tag list.
 *
 * @return ZOK SIP decodes language-tag list successfully.
 * @return ZOK SIP decodes language-tag list failed.
 *
 * @see Sip_DecodeLangTag
 */
ZFUNC ZINT Sip_DecodeLangTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_TAG_LST *pstLangTagLst);

/**
 * @brief SIP decodes language-tag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangTag SIP language-tag.
 *
 * @return ZOK SIP decodes language-tag successfully.
 * @return ZOK SIP decodes language-tag failed.
 *
 * @see Sip_DecodeLangTagLst
 */
ZFUNC ZINT Sip_DecodeLangTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_TAG *pstLangTag);

/**
 * @brief SIP decodes media-type.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMediaType SIP media-type.
 *
 * @return ZOK SIP decodes media-type successfully.
 * @return ZOK SIP decodes media-type failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeMediaType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MEDIA_TYPE *pstMediaType);

/**
 * @brief SIP decodes SIP-date.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDate SIP SIP-date.
 *
 * @return ZOK SIP decodes SIP-date successfully.
 * @return ZOK SIP decodes SIP-date failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeDate(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_DATE *pstDate);

/**
 * @brief SIP decodes error-uri list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstErrorUriLst SIP error-uri list.
 *
 * @return ZOK SIP decodes error-uri list successfully.
 * @return ZOK SIP decodes error-uri list failed.
 *
 * @see Sip_DecodeErrorUri
 */
ZFUNC ZINT Sip_DecodeErrorUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ERROR_URI_LST *pstErrorUriLst);

/**
 * @brief SIP decodes error-uri.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstErrorUri SIP error-uri.
 *
 * @return ZOK SIP decodes error-uri successfully.
 * @return ZOK SIP decodes error-uri failed.
 *
 * @see Sip_DecodeErrorUriLst
 */
ZFUNC ZINT Sip_DecodeErrorUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ERROR_URI *pstErrorUri);

/**
 * @brief SIP decodes from-param & to-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP from-param & to-param list.
 *
 * @return ZOK SIP decodes from-param & to-param list successfully.
 * @return ZOK SIP decodes from-param & to-param list failed.
 * 
 * @see Sip_DecodeFromToParm
 */
ZFUNC ZINT Sip_DecodeFromToParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FROM_TO_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes from-param & to-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP from-param & to-param.
 *
 * @return ZOK SIP decodes from-param & to-param successfully.
 * @return ZOK SIP decodes from-param & to-param failed.
 *
 * @see Sip_DecodeFromToParmLst
 */
ZFUNC ZINT Sip_DecodeFromToParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FROM_TO_PARM *pstParm);

/**
 * @brief SIP decodes callid list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCallidLst SIP callid list.
 *
 * @return ZOK SIP decodes callid list successfully.
 * @return ZOK SIP decodes callid list failed.
 *
 * @see Sip_DecodeCallid
 */
ZFUNC ZINT Sip_DecodeCallidLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CALLID_LST *pstCallidLst);

/**
 * @brief SIP decodes callid.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCallid SIP callid.
 *
 * @return ZOK SIP decodes callid successfully.
 * @return ZOK SIP decodes callid failed.
 *
 * @see Sip_DecodeCallidLst
 */
ZFUNC ZINT Sip_DecodeCallid(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CALLID *pstCallid);

/**
 * @brief SIP decodes challenge.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstChallenge SIP challenge.
 *
 * @return ZOK SIP decodes challenge successfully.
 * @return ZOK SIP decodes challenge failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeChallenge(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHALLENGE *pstChallenge);

/**
 * @brief SIP decodes digest.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigest SIP digest.
 *
 * @return ZOK SIP decodes digest successfully.
 * @return ZOK SIP decodes digest failed.
 *
 * @see Sip_DecodeDigestClnLst, Sip_DecodeDigestCln
 */
ZFUNC ZINT Sip_DecodeDigest(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST *pstDigest);

/**
 * @brief SIP decodes digest-cln list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigestClnLst SIP digest-cln list.
 *
 * @return ZOK SIP decodes digest-cln list successfully.
 * @return ZOK SIP decodes digest-cln list failed.
 *
 * @see Sip_DecodeDigest, Sip_DecodeDigestCln
 */
ZFUNC ZINT Sip_DecodeDigestClnLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST_CLN_LST *pstDigestClnLst);

/**
 * @brief SIP decodes digest-cln.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigestCln SIP digest-cln.
 *
 * @return ZOK SIP decodes digest-cln successfully.
 * @return ZOK SIP decodes digest-cln failed.
 *
 * @see Sip_DecodeDigest, Sip_DecodeDigestClnLst
 */
ZFUNC ZINT Sip_DecodeDigestCln(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST_CLN *pstDigestCln);

/**
 * @brief SIP decodes domain.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDomain SIP domain.
 *
 * @return ZOK SIP decodes domain successfully.
 * @return ZOK SIP decodes domain failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeDomain(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DOMAIN *pstDomain);

/**
 * @brief SIP decodes algorithm.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlgo SIP algorithm.
 *
 * @return ZOK SIP decodes algorithm successfully.
 * @return ZOK SIP decodes algorithm failed.
 *
 * @see Sip_DecodeAlgoVal
 */
ZFUNC ZINT Sip_DecodeAlgo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALGO *pstAlgo);

/**
 * @brief SIP decodes aka-namespace.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAkaNs SIP aka-namespace.
 *
 * @return ZOK SIP decodes aka-namespace successfully.
 * @return ZOK SIP decodes aka-namespace failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeAkaNs(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AKA_NS *pstAkaNs);

/**
 * @brief SIP decodes algorithm-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlgoVal SIP algorithm-value.
 *
 * @return ZOK SIP decodes algorithm-value successfully.
 * @return ZOK SIP decodes algorithm-value failed.
 *
 * @see Sip_DecodeAlgo
 */
ZFUNC ZINT Sip_DecodeAlgoVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALGO_VAL *pstAlgoVal);

/**
 * @brief SIP decodes URI list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriLst SIP URI list.
 *
 * @return ZOK SIP decodes URI list successfully.
 * @return ZOK SIP decodes URI list failed.
 *
 * @see Sip_DecodeUri
 */
ZFUNC ZINT Sip_DecodeUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_LST *pstUriLst);

/**
 * @brief SIP decodes URI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUri SIP URI.
 *
 * @return ZOK SIP decodes URI successfully.
 * @return ZOK SIP decodes URI failed.
 *
 * @see Sip_DecodeUriLst
 */
ZFUNC ZINT Sip_DecodeUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI *pstUri);

/**
 * @brief SIP decodes path-segments.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPathSegments SIP path-segments.
 *
 * @return ZOK SIP decodes path-segments successfully.
 * @return ZOK SIP decodes path-segments failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeAbsPath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PATH_SEGMENTS *pstPathSegments);

/**
 * @brief SIP decodes segment.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSegment SIP segment.
 *
 * @return ZOK SIP decodes segment successfully.
 * @return ZOK SIP decodes segment failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeSegment(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SEGMENT *pstSegment);

/**
 * @brief SIP decodes qop-options.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQopts SIP qop-options.
 *
 * @return ZOK SIP decodes qop-options successfully.
 * @return ZOK SIP decodes qop-options failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeQopts(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_QOPTS *pstQopts);

/**
 * @brief SIP decodes some type value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQopValLst SIP some type value list.
 *
 * @return ZOK SIP decodes some type value list successfully.
 * @return ZOK SIP decodes some type value list failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeQopValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TYPE_VAL_LST *pstQopValLst);

/**
 * @brief SIP decodes some type value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQopVal SIP some type value.
 *
 * @return ZOK SIP decodes some type value successfully.
 * @return ZOK SIP decodes some type value failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeQopVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TYPE_VAL *pstQopVal);

/**
 * @brief SIP decodes other-challenge.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOther SIP other-challenge.
 *
 * @return ZOK SIP decodes other-challenge successfully.
 * @return ZOK SIP decodes other-challenge failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeOtherChallenge(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OTHER_CHALLENGE *pstOther);

/**
 * @brief SIP decodes auth-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP auth-param list.
 *
 * @return ZOK SIP decodes auth-param list successfully.
 * @return ZOK SIP decodes auth-param list failed.
 *
 * @see Sip_DecodeAuthParmLst
 */
ZFUNC ZINT Sip_DecodeAuthParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AUTH_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes auth-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP auth-param.
 *
 * @return ZOK SIP decodes auth-param successfully.
 * @return ZOK SIP decodes auth-param failed.
 *
 * @see Sip_DecodeAuthParm
 */
ZFUNC ZINT Sip_DecodeAuthParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AUTH_PARM *pstParm);

/**
 * @brief SIP decodes credentials.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCredents SIP credentials.
 *
 * @return ZOK SIP decodes credentials successfully.
 * @return ZOK SIP decodes credentials failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeCredents(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CREDENTS *pstCredents);

/**
 * @brief SIP decodes digest-response.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigestRsp SIP digest-response.
 *
 * @return ZOK SIP decodes digest-response successfully.
 * @return ZOK SIP decodes digest-response failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeDigestRsp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST_RSP *pstDigestRsp);

/**
 * @brief SIP decodes dig-resp list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigRspLst SIP dig-resp list.
 *
 * @return ZOK SIP decodes dig-resp list successfully.
 * @return ZOK SIP decodes dig-resp list failed.
 *
 * @see Sip_DecodeDigRsp
 */
ZFUNC ZINT Sip_DecodeDigRspLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIG_RSP_LST *pstDigRspLst);

/**
 * @brief SIP decodes dig-resp.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigRsp SIP dig-resp.
 *
 * @return ZOK SIP decodes dig-resp successfully.
 * @return ZOK SIP decodes dig-resp failed.
 *
 * @see Sip_DecodeDigRspLst
 */
ZFUNC ZINT Sip_DecodeDigRsp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIG_RSP *pstDigRsp);

/**
 * @brief SIP decodes other-response.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOther SIP other-response.
 *
 * @return ZOK SIP decodes other-response successfully.
 * @return ZOK SIP decodes other-response failed.
 *
 * @see Sip_DecodeDigRspLst
 */
ZFUNC ZINT Sip_DecodeOtherRsp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OTHER_RSP *pstOther);

/**
 * @brief SIP decodes option-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOptTagLst SIP option-tag list.
 *
 * @return ZOK SIP decodes option-tag list successfully.
 * @return ZOK SIP decodes option-tag list failed.
 *
 * @see Sip_DecodeOptTag
 */
ZFUNC ZINT Sip_DecodeOptTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OPT_TAG_LST *pstOptTagLst);

/**
 * @brief SIP decodes option-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstComment Zos signed structure string.
 *
 * @return ZOK SIP decodes option-tag list successfully.
 * @return ZOK SIP decodes option-tag list failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeComment(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstComment);

/**
 * @brief SIP decodes option-tag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOptTag SIP option-tag.
 *
 * @return ZOK SIP decodes option-tag successfully.
 * @return ZOK SIP decodes option-tag failed.
 *
 * @see Sip_DecodeOptTagLst
 */
ZFUNC ZINT Sip_DecodeOptTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OPT_TAG *pstOptTag);

/**
 * @brief SIP decodes retry-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP retry-param list.
 *
 * @return ZOK SIP decodes retry-param list successfully.
 * @return ZOK SIP decodes retry-param list failed.
 *
 * @see Sip_DecodeRetryParm
 */
ZFUNC ZINT Sip_DecodeRetryParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RETRY_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes retry-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP retry-param.
 *
 * @return ZOK SIP decodes retry-param successfully.
 * @return ZOK SIP decodes retry-param failed.
 *
 * @see Sip_DecodeRetryParmLst
 */
ZFUNC ZINT Sip_DecodeRetryParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RETRY_PARM *pstParm);

/**
 * @brief SIP decodes route-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP route-param list.
 *
 * @return ZOK SIP decodes route-param list successfully.
 * @return ZOK SIP decodes route-param list failed.
 *
 * @see Sip_DecodeRouteParm
 */
ZFUNC ZINT Sip_DecodeRouteParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ROUTE_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes route-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP route-param.
 *
 * @return ZOK SIP decodes route-param successfully.
 * @return ZOK SIP decodes route-param failed.
 *
 * @see Sip_DecodeRouteParmLst
 */
ZFUNC ZINT Sip_DecodeRouteParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ROUTE_PARM *pstParm);

/**
 * @brief SIP decodes server-val list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstServerValLst SIP server-val list.
 *
 * @return ZOK SIP decodes server-val list successfully.
 * @return ZOK SIP decodes server-val list failed.
 *
 * @see Sip_DecodeServerVal
 */
ZFUNC ZINT Sip_DecodeServerValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVER_VAL_LST *pstServerValLst);

/**
 * @brief SIP decodes server-val.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstServerVal SIP server-val.
 *
 * @return ZOK SIP decodes server-val successfully.
 * @return ZOK SIP decodes server-val failed.
 *
 * @see Sip_DecodeServerValLst
 */
ZFUNC ZINT Sip_DecodeServerVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVER_VAL *pstServerVal);

/**
 * @brief SIP decodes product.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstProduct SIP product.
 *
 * @return ZOK SIP decodes product successfully.
 * @return ZOK SIP decodes product failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeProduct(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRODUCT *pstProduct);

/**
 * @brief SIP decodes via-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP via-parm list.
 *
 * @return ZOK SIP decodes via-param list successfully.
 * @return ZOK SIP decodes via-param list failed.
 *
 * @see Sip_DecodeViaParm
 */
ZFUNC ZINT Sip_DecodeViaParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes via-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP via-param list.
 *
 * @return ZOK SIP decodes via-param list successfully.
 * @return ZOK SIP decodes via-param list failed.
 *
 * @see Sip_DecodeViaParmLst
 */
ZFUNC ZINT Sip_DecodeViaParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARM *pstParm);

/**
 * @brief SIP decodes sent-protocol.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstProtocol SIP sent-protocol.
 *
 * @return ZOK SIP decodes sent-protocol successfully.
 * @return ZOK SIP decodes sent-protocol failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeSentProtocol(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SENT_PROTOCOL *pstProtocol);

/**
 * @brief SIP decodes sent-by.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstBy SIP sent-by.
 *
 * @return ZOK SIP decodes sent-by successfully.
 * @return ZOK SIP decodes sent-by failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeSentBy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SENT_BY *pstBy);

/**
 * @brief SIP decodes via-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP via-params list.
 *
 * @return ZOK SIP decodes via-params list successfully.
 * @return ZOK SIP decodes via-params list failed.
 *
 * @see Sip_DecodeViaParms
 */
ZFUNC ZINT Sip_DecodeViaParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes via-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP via-params.
 *
 * @return ZOK SIP decodes via-params successfully.
 * @return ZOK SIP decodes via-params failed.
 *
 * @see Sip_DecodeViaParmsLst
 */
ZFUNC ZINT Sip_DecodeViaParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARMS *pstParms);

/**
 * @brief SIP decodes via-received.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRecv SIP via-received.
 *
 * @return ZOK SIP decodes via-received successfully.
 * @return ZOK SIP decodes via-received failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeViaRecv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_RECV *pstRecv);

/**
 * @brief SIP decodes warning-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP warning-value list.
 *
 * @return ZOK SIP decodes warning-value list successfully.
 * @return ZOK SIP decodes warning-value list failed.
 *
 * @see Sip_DecodeWarnVal, Sip_DecodeWarnAgent
 */
ZFUNC ZINT Sip_DecodeWarnValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_WARN_VAL_LST *pstParmLst);

/**
 * @brief SIP decodes warning-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP warning-value.
 *
 * @return ZOK SIP decodes warning-value successfully.
 * @return ZOK SIP decodes warning-value failed.
 *
 * @see Sip_DecodeWarnValLst, Sip_DecodeWarnAgent
 */
ZFUNC ZINT Sip_DecodeWarnVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_WARN_VAL *pstParm);

/**
 * @brief SIP decodes warn-agent.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAgent SIP warn-agent.
 *
 * @return ZOK SIP decodes warn-agent successfully.
 * @return ZOK SIP decodes warn-agent failed.
 *
 * @see Sip_DecodeWarnValLst, Sip_DecodeWarnVal
 */
ZFUNC ZINT Sip_DecodeWarnAgent(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_WARN_AGENT *pstAgent);

/**
 * @brief SIP decodes event-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP event-param list.
 *
 * @return ZOK SIP decodes event-param list successfully.
 * @return ZOK SIP decodes event-param list failed.
 *
 * @see Sip_DecodeEvntParm
 */
ZFUNC ZINT Sip_DecodeEvntParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes event-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP event-param.
 *
 * @return ZOK SIP decodes event-param successfully.
 * @return ZOK SIP decodes event-param failed.
 *
 * @see Sip_DecodeEvntParmLst
 */
ZFUNC ZINT Sip_DecodeEvntParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_PARM *pstParm);

/**
 * @brief SIP decodes event-type list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntTypeLst SIP event-type list.
 *
 * @return ZOK SIP decodes event-type list successfully.
 * @return ZOK SIP decodes event-type list failed.
 *
 * @see Sip_DecodeEvntType
 */
ZFUNC ZINT Sip_DecodeEvntTypeLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TYPE_LST *pstEvntTypeLst);

/**
 * @brief SIP decodes event-type.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntType SIP event-type.
 *
 * @return ZOK SIP decodes event-type successfully.
 * @return ZOK SIP decodes event-type failed.
 *
 * @see Sip_DecodeEvntTypeLst
 */
ZFUNC ZINT Sip_DecodeEvntType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TYPE *pstEvntType);

/**
 * @brief SIP decodes substate-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSubstaVal SIP substate-value.
 *
 * @return ZOK SIP decodes substate-value successfully.
 * @return ZOK SIP decodes substate-value failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeSubstaVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SUBSTA_VAL *pstSubstaVal);

/**
 * @brief SIP decodes event-template list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntTempLst SIP event-template list.
 *
 * @return ZOK SIP decodes event-template list successfully.
 * @return ZOK SIP decodes event-template list failed.
 *
 * @see Sip_DecodeEvntTemp
 */
ZFUNC ZINT Sip_DecodeEvntTempLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TEMP_LST *pstEvntTempLst);

/**
 * @brief SIP decodes event-template.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntTemp SIP event-template.
 *
 * @return ZOK SIP decodes event-template successfully.
 * @return ZOK SIP decodes event-template failed.
 *
 * @see Sip_DecodeEvntTempLst
 */
ZFUNC ZINT Sip_DecodeEvntTemp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TEMP *pstEvntTemp);

/**
 * @brief SIP decodes subexp-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP subexp-params list.
 *
 * @return ZOK SIP decodes subexp-params list successfully.
 * @return ZOK SIP decodes subexp-params list failed.
 *
 * @see Sip_DecodeSubexpParms
 */
ZFUNC ZINT Sip_DecodeSubexpParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SUBEXP_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes rsubexp-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP subexp-params.
 *
 * @return ZOK SIP decodes subexp-params successfully.
 * @return ZOK SIP decodes subexp-params failed.
 *
 * @see Sip_DecodeSubexpParmsLst
 */
ZFUNC ZINT Sip_DecodeSubexpParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SUBEXP_PARMS *pstParms);

/**
 * @brief SIP decodes replaces-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP replaces-param list.
 *
 * @return ZOK SIP decodes replaces-param list successfully.
 * @return ZOK SIP decodes replaces-param list failed.
 *
 * @see Sip_DecodeReplacesParm
 */
ZFUNC ZINT Sip_DecodeReplacesParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REPLACES_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes replaces-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP replaces-param.
 *
 * @return ZOK SIP decodes replaces-param successfully.
 * @return ZOK SIP decodes replaces-param failed.
 *
 * @see Sip_DecodeReplacesParmLst
 */
ZFUNC ZINT Sip_DecodeReplacesParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REPLACES_PARM *pstParm);

/**
 * @brief SIP decodes directive list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDirectLst SIP directive list.
 *
 * @return ZOK SIP decodes directive list successfully.
 * @return ZOK SIP decodes directive list failed.
 *
 * @see Sip_DecodeDirect
 */
ZFUNC ZINT Sip_DecodeDirectLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIRECT_LST *pstDirectLst);

/**
 * @brief SIP decodes directive.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDirect SIP directive.
 *
 * @return ZOK SIP decodes directive successfully.
 * @return ZOK SIP decodes directive failed.
 *
 * @see Sip_DecodeDirectLst
 */
ZFUNC ZINT Sip_DecodeDirect(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIRECT *pstDirect); 

/**
 * @brief SIP decodes ac-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAcValLst SIP ac-value list.
 *
 * @return ZOK SIP decodes ac-value list successfully.
 * @return ZOK SIP decodes ac-value list failed.
 *
 * @see Sip_DecodeAcVal
 */
ZFUNC ZINT Sip_DecodeAcValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_VAL_LST *pstAcValLst);

/**
 * @brief SIP decodes ac-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAcVal SIP ac-value.
 *
 * @return ZOK SIP decodes ac-value successfully.
 * @return ZOK SIP decodes ac-value failed.
 *
 * @see Sip_DecodeAcValLst
 */
ZFUNC ZINT Sip_DecodeAcVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_VAL *pstAcVal); 

/**
 * @brief SIP decodes ac-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP ac-params list.
 *
 * @return ZOK SIP decodes ac-params list successfully.
 * @return ZOK SIP decodes ac-params list failed.
 *
 * @see Sip_DecodeAcParms
 */
ZFUNC ZINT Sip_DecodeAcParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes ac-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP ac-params.
 *
 * @return ZOK SIP decodes ac-params successfully.
 * @return ZOK SIP decodes ac-params failed.
 *
 * @see Sip_DecodeAcParmsLst
 */
ZFUNC ZINT Sip_DecodeAcParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_PARMS *pstParms); 

/**
 * @brief SIP decodes feature-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstFeature SIP feature-param.
 *
 * @return ZOK SIP decodes feature-param successfully.
 * @return ZOK SIP decodes feature-param failed.
 *
 * @see Sip_DecodeFeatureValLst, Sip_DecodeFeatureVal
 */
ZFUNC ZINT Sip_DecodeFeatureParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FEATURE_PARM *pstFeature);

/**
 * @brief SIP decodes feature-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstFeatureValLst SIP feature-value list.
 *
 * @return ZOK SIP decodes feature-value list successfully.
 * @return ZOK SIP decodes feature-value list failed.
 *
 * @see Sip_DecodeFeatureParm, Sip_DecodeFeatureVal
 */
ZFUNC ZINT Sip_DecodeFeatureValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FEATURE_VAL_LST *pstFeatureValLst);

/**
 * @brief SIP decodes feature-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstFeatureVal SIP feature-value.
 *
 * @return ZOK SIP decodes feature-value successfully.
 * @return ZOK SIP decodes feature-value failed.
 *
 * @see Sip_DecodeFeatureParm, Sip_DecodeFeatureValLst
 */
ZFUNC ZINT Sip_DecodeFeatureVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FEATURE_VAL *pstFeatureVal);

/**
 * @brief SIP decodes route-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRcValLst SIP route-param list.
 *
 * @return ZOK SIP decodes route-param list successfully.
 * @return ZOK SIP decodes route-param list failed.
 *
 * @see Sip_DecodeRcVal
 */
ZFUNC ZINT Sip_DecodeRcValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_VAL_LST *pstRcValLst);

/**
 * @brief SIP decodes route-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRcVal SIP route-param list.
 *
 * @return ZOK SIP decodes route-param list successfully.
 * @return ZOK SIP decodes route-param list failed.
 *
 * @see Sip_DecodeRcValLst
 */
ZFUNC ZINT Sip_DecodeRcVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_VAL *pstRcVal);

/**
 * @brief SIP decodes rc-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP rc-params list.
 *
 * @return ZOK SIP decodes rc-params list successfully.
 * @return ZOK SIP decodes rc-params list failed.
 *
 * @see Sip_DecodeRcParms
 */
ZFUNC ZINT Sip_DecodeRcParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes rc-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP rc-params.
 *
 * @return ZOK SIP decodes rc-params successfully.
 * @return ZOK SIP decodes rc-params failed.
 *
 * @see Sip_DecodeRcParmsLst
 */
ZFUNC ZINT Sip_DecodeRcParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_PARMS *pstParms);

/**
 * @brief SIP decodes privacy-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPrivacyTagLst SIP privacy-tag list.
 *
 * @return ZOK SIP decodes privacy-tag list successfully.
 * @return ZOK SIP decodes privacy-tag list failed.
 *
 * @see Sip_DecodePrivacyTag
 */
ZFUNC ZINT Sip_DecodePrivacyTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIVACY_TAG_LST *pstPrivacyTagLst);

/**
 * @brief SIP decodes privacy-tag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPrivacyTag SIP privacy-tag.
 *
 * @return ZOK SIP decodes privacy-tag successfully.
 * @return ZOK SIP decodes privacy-tag failed.
 *
 * @see Sip_DecodePrivacyTagLst
 */
ZFUNC ZINT Sip_DecodePrivacyTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIVACY_TAG *pstPrivacyTag);

/**
 * @brief SIP decodes rpid-priv list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpidPrivLst SIP rpid-priv list.
 *
 * @return ZOK SIP decodes rpid-priv list successfully.
 * @return ZOK SIP decodes rpid-priv list failed.
 *
 * @see Sip_DecodeRpidPriv, Sip_DecodeRpidPrivTknLst, Sip_DecodeRpidPrivTkn
 */
ZFUNC ZINT Sip_DecodeRpidPrivLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV_LST *pstRpidPrivLst);

/**
 * @brief SIP decodes rpid-priv.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpidPriv SIP route-param.
 *
 * @return ZOK SIP decodes rpid-priv successfully.
 * @return ZOK SIP decodes rpid-priv failed.
 *
 * @see Sip_DecodeRpidPrivLst, Sip_DecodeRpidPrivTknLst, Sip_DecodeRpidPrivTkn
 */
ZFUNC ZINT Sip_DecodeRpidPriv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV *pstRpidPriv);

/**
 * @brief SIP decodes rpid-privacy-token list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTknLst SIP rpid-privacy-token list.
 *
 * @return ZOK SIP decodes rpid-privacy-token list successfully.
 * @return ZOK SIP decodes rpid-privacy-token list failed.
 *
 * @see Sip_DecodeRpidPrivLst, Sip_DecodeRpidPrivTkn, Sip_DecodeRpidPrivTkn
 */
ZFUNC ZINT Sip_DecodeRpidPrivTknLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV_TKN_LST *pstTknLst);

/**
 * @brief SIP decodes rpid-privacy-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTkn SIP rpid-privacy-token.
 *
 * @return ZOK SIP decodes rpid-privacy-token successfully.
 * @return ZOK SIP decodes rpid-privacy-token failed.
 *
 * @see Sip_DecodeRpidPrivLst, Sip_DecodeRpidPriv, Sip_DecodeRpidPrivTknLst
 */
ZFUNC ZINT Sip_DecodeRpidPrivTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV_TKN *pstTkn);

/**
 * @brief SIP decodes rpi-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPrivacy SIP rpi-privacy.
 *
 * @return ZOK SIP decodes rpi-privacy successfully.
 * @return ZOK SIP decodes rpi-privacy failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeRpiPrivacy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_PRIVACY *pstPrivacy);

/**
 * @brief SIP decodes rpi-priv-element list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstElemLst SIP rpi-priv-element list.
 *
 * @return ZOK SIP decodes rpi-priv-element list successfully.
 * @return ZOK SIP decodes rpi-priv-element list failed.
 *
 * @see Sip_DecodeRpiPrivElem
 */
ZFUNC ZINT Sip_DecodeRpiPrivElemLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_PRIV_ELEM_LST *pstElemLst);

/**
 * @brief SIP decodes rpi-priv-element.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstElem SIP rpi-priv-element.
 *
 * @return ZOK SIP decodes rpi-priv-element successfully.
 * @return ZOK SIP decodes rpi-priv-element failed.
 *
 * @see Sip_DecodeRpiPrivElemLst
 */
ZFUNC ZINT Sip_DecodeRpiPrivElem(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_PRIV_ELEM *pstElem);

/**
 * @brief SIP decodes rpid list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpidLst SIP rpid list.
 *
 * @return ZOK SIP decodes rpid list successfully.
 * @return ZOK SIP decodes rpid list failed.
 *
 * @see Sip_DecodeRpid
 */
ZFUNC ZINT Sip_DecodeRpidLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_LST *pstRpidLst);

/**
 * @brief SIP decodes rpid.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpid SIP rpid.
 *
 * @return ZOK SIP decodes rpid successfully.
 * @return ZOK SIP decodes rpid failed.
 *
 * @see Sip_DecodeRpidLst
 */
ZFUNC ZINT Sip_DecodeRpid(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID *pstRpid);

/**
 * @brief SIP decodes rpi-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTknLst SIP rpi-token.
 *
 * @return ZOK SIP decodes rpi-token successfully.
 * @return ZOK SIP decodes rpi-token failed.
 *
 * @see Sip_DecodeRpiTkn
 */
ZFUNC ZINT Sip_DecodeRpiTknLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_TKN_LST *pstTknLst);

/**
 * @brief SIP decodes rpi-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTkn SIP rpi-token.
 *
 * @return ZOK SIP decodes rpi-token successfully.
 * @return ZOK SIP decodes rpi-token failed.
 *
 * @see Sip_DecodeRpiTknLst
 */
ZFUNC ZINT Sip_DecodeRpiTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_TKN *pstTkn);

/**
 * @brief SIP decodes other-rpi-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOther SIP other-rpi-token.
 *
 * @return ZOK SIP decodes other-rpi-token successfully.
 * @return ZOK SIP decodes other-rpi-token failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeOtherRpiTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OTHER_RPI_TKN *pstOther);

/**
 * @brief SIP decodes join-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP join-param list.
 *
 * @return ZOK SIP decodes join-param list successfully.
 * @return ZOK SIP decodes join-param list failed.
 *
 * @see Sip_DecodeJoinParm
 */
ZFUNC ZINT Sip_DecodeJoinParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_JOIN_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes join-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP join-param.
 *
 * @return ZOK SIP decodes join-param successfully.
 * @return ZOK SIP decodes join-param failed.
 *
 * @see Sip_DecodeJoinParmLst
 */
ZFUNC ZINT Sip_DecodeJoinParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_JOIN_PARM *pstParm);
/**
 * @brief SIP decodes p-aso-uri-spec list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriSpecLst SIP p-aso-uri-spec list.
 *
 * @return ZOK SIP decodes p-aso-uri-spec list successfully.
 * @return ZOK SIP decodes p-aso-uri-spec list failed.
 *
 * @see Sip_DecodePAsoUriSpec
 */
ZFUNC ZINT Sip_DecodePAsoUriSpecLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_ASO_URI_SPEC_LST *pstUriSpecLst);

/**
 * @brief SIP decodes p-aso-uri-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriSpec SIP p-aso-uri-spec.
 *
 * @return ZOK SIP decodes p-aso-uri-spec successfully.
 * @return ZOK SIP decodes p-aso-uri-spec failed.
 *
 * @see Sip_DecodePAsoUriSpecLst
 */
ZFUNC ZINT Sip_DecodePAsoUriSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_ASO_URI_SPEC *pstUriSpec);

/**
 * @brief SIP decodes called-pty-id-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstIdSpec SIP called-pty-id-spec.
 *
 * @return ZOK SIP decodes called-pty-id-spec successfully.
 * @return ZOK SIP decodes called-pty-id-spec failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeCalledPtyIdSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CALLED_PTY_ID_SPEC *pstIdSpec);

/**
 * @brief SIP decodes p-vnet-id-spec list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVnetSpecLst SIP p-vnet-id-spec list.
 *
 * @return ZOK SIP decodes p-vnet-id-spec list successfully.
 * @return ZOK SIP decodes p-vnet-id-spec list header failed.
 *
 * @see Sip_DecodePVnetSpec
 */

ZFUNC ZINT Sip_DecodePVnetSpecLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_VNET_ID_SPEC_LST *pstVnetSpecLst);

/**
 * @brief SIP decodes p-vnet-id-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVnetSpec SIP p-vnet-id-spec.
 *
 * @return ZOK SIP decodes p-vnet-id-spec successfully.
 * @return ZOK SIP decodes p-vnet-id-spec header failed.
 *
 * @see Sip_DecodePVnetSpecLst
 */
ZFUNC ZINT Sip_DecodePVnetSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_VNET_ID_SPEC *pstVnetSpec);

/**
 * @brief SIP decodes access-net-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstNetSpec SIP access-net-spec.
 *
 * @return ZOK SIP decodes access-net-spec successfully.
 * @return ZOK SIP decodes access-net-spec header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodePAnetSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_ACCESS_NET_SPEC *pstNetSpec);

/**
 * @brief SIP decodes some type value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstType SIP some type value.
 *
 * @return ZOK SIP decodes some type value successfully.
 * @return ZOK SIP decodes some type value header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodePAccessType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TYPE_VAL *pstType);

/**
 * @brief SIP decodes access-info list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfoLst SIP access-info list.
 *
 * @return ZOK SIP decodes access-info list successfully.
 * @return ZOK SIP decodes access-info list header failed.
 *
 * @see Sip_DecodePAnetInfo
 */
ZFUNC ZINT Sip_DecodePAnetInfoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACCESS_INFO_LST *pstInfoLst);

/**
 * @brief SIP decodes access-info.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfo SIP access-info.
 *
 * @return ZOK SIP decodes access-info successfully.
 * @return ZOK SIP decodes access-info failed.
 *
 * @see Sip_DecodePAnetInfoLst
 */
ZFUNC ZINT Sip_DecodePAnetInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACCESS_INFO *pstInfo);

/**
 * @brief SIP decodes charge-addr-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP charge-addr-params.
 *
 * @return ZOK SIP decodes charge-addr-params list successfully.
 * @return ZOK SIP decodes charge-addr-params list failed.
 *
 * @see Sip_DecodePChargAddr
 */
ZFUNC ZINT Sip_DecodePChargAddrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_ADDR_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes charge-addr-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP charge-addr-params.
 *
 * @return ZOK SIP decodes charge-addr-params successfully.
 * @return ZOK SIP decodes charge-addr-params failed.
 *
 * @see Sip_DecodePChargAddrLst
 */
ZFUNC ZINT Sip_DecodePChargAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_ADDR_PARMS *pstParms);

/**
 * @brief SIP decodes charge-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP charge-params list.
 *
 * @return ZOK SIP decodes charge-params list successfully.
 * @return ZOK SIP decodes charge-params list failed.
 *
 * @see Sip_DecodePChargParms
 */
ZFUNC ZINT Sip_DecodePChargParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes charge-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP charge-params.
 *
 * @return ZOK SIP decodes charge-params successfully.
 * @return ZOK SIP decodes charge-params failed.
 *
 * @see Sip_DecodePChargParmsLst
 */
ZFUNC ZINT Sip_DecodePChargParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_PARMS *pstParms);

/**
 * @brief SIP decodes PAssertedID-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP PAssertedID-value list.
 *
 * @return ZOK SIP decodes PAssertedID-value list successfully.
 * @return ZOK SIP decodes PAssertedID-value list failed.
 *
 * @see Sip_DecodePAssertedId
 */
ZFUNC ZINT Sip_DecodePAssertedIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PASSERTEDID_VAL_LST *pstValLst);

/**
 * @brief SIP decodes PAssertedID-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP PAssertedID-value.
 *
 * @return ZOK SIP decodes PAssertedID-value successfully.
 * @return ZOK SIP decodes PAssertedID-value failed.
 *
 * @see Sip_DecodePAssertedIdLst
 */
ZFUNC ZINT Sip_DecodePAssertedId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PASSERTEDID_VAL *pstVal);

/**
 * @brief SIP decodes PPreferredID-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP PPreferredID-value list.
 *
 * @return ZOK SIP decodes PPreferredID-value list successfully.
 * @return ZOK SIP decodes PPreferredID-value list failed.
 *
 * @see Sip_DecodePPreferredId
 */
ZFUNC ZINT Sip_DecodePPreferredIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PPREFERREDID_VAL_LST *pstValLst);

/**
 * @brief SIP decodes PPreferredID-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP PPreferredID-value.
 *
 * @return ZOK SIP decodes PPreferredID-value successfully.
 * @return ZOK SIP decodes PPreferredID-value failed.
 *
 * @see Sip_DecodePPreferredIdLst, Sip_DecodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_DecodePPreferredId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PPREFERREDID_VAL *pstVal);

/**
 * @brief SIP decodes P-Media-Authorization-Token list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTknLst SIP P-Media-Authorization-Token list.
 *
 * @return ZOK SIP decodes P-Media-Authorization-Token list successfully.
 * @return ZOK SIP decodes P-Media-Authorization-Token list failed.
 *
 * @see Sip_DecodePMediaAuthorTkn
 */
ZFUNC ZINT Sip_DecodePMediaAuthorTknLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_MEDIA_AUTHOR_TKN_LST *pstTknLst);

/**
 * @brief SIP decodes P-Media-Authorization-Token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTkn SIP P-Media-Authorization-Token.
 *
 * @return ZOK SIP decodes P-Media-Authorization-Token successfully.
 * @return ZOK SIP decodes P-Media-Authorization-Token failed.
 *
 * @see Sip_DecodePMediaAuthorTknLst
 */
ZFUNC ZINT Sip_DecodePMediaAuthorTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_MEDIA_AUTHOR_TKN *pstTkn);

/**
 * @brief SIP decodes priv-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP priv-value list.
 *
 * @return ZOK SIP decodes priv-value list successfully.
 * @return ZOK SIP decodes priv-value list failed.
 *
 * @see Sip_DecodePrivVal
 */
ZFUNC ZINT Sip_DecodePrivValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIV_VAL_LST *pstValLst);

/**
 * @brief SIP decodes priv-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP priv-value.
 *
 * @return ZOK SIP decodes priv-value successfully.
 * @return ZOK SIP decodes priv-value failed.
 *
 * @see Sip_DecodePrivValLst
 */
ZFUNC ZINT Sip_DecodePrivVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIV_VAL *pstVal);

/**
 * @brief SIP decodes reason-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP reason-value list.
 *
 * @return ZOK SIP decodes reason-value list.
 * @return ZOK SIP decodes reason-value list.
 *
 * @see Sip_DecodeReasonVal
 */
ZFUNC ZINT Sip_DecodeReasonValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_VAL_LST *pstValLst);

/**
 * @brief SIP decodes reason-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP message header.
 *
 * @return ZOK SIP decodes reason-value successfully.
 * @return ZOK SIP decodes reason-value failed.
 *
 * @see Sip_DecodeReasonValLst
 */
ZFUNC ZINT Sip_DecodeReasonVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_VAL *pstVal);

/**
 * @brief SIP decodes reason-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP reason-params list.
 *
 * @return ZOK SIP decodes reason-params list successfully.
 * @return ZOK SIP decodes reason-params list failed.
 *
 * @see Sip_DecodeReasonParms
 */
ZFUNC ZINT Sip_DecodeReasonParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes reason-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP reason-params.
 *
 * @return ZOK SIP decodes reason-params successfully.
 * @return ZOK SIP decodes reason-params failed.
 *
 * @see Sip_DecodeReasonParmsLst
 */
ZFUNC ZINT Sip_DecodeReasonParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_PARMS *pstParms);

/**
 * @brief SIP decodes sec-mechanism list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMechLst SIP sec-mechanism list.
 *
 * @return ZOK SIP decodes sec-mechanism list successfully.
 * @return ZOK SIP decodes sec-mechanism list failed.
 *
 * @see Sip_DecodeSecMech
 */

ZFUNC ZINT Sip_DecodeSecMechLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SEC_MECH_LST *pstMechLst);

/**
 * @brief SIP decodes sec-mechanism.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMech SIP sec-mechanism.
 *
 * @return ZOK SIP decodes sec-mechanism successfully.
 * @return ZOK SIP decodes sec-mechanism failed.
 *
 * @see Sip_DecodeSecMechLst
 */
ZFUNC ZINT Sip_DecodeSecMech(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SEC_MECH *pstMech);

/**
 * @brief SIP decodes mech-parameters list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP mech-parameters list.
 *
 * @return ZOK SIP decodes mech-parameters list successfully.
 * @return ZOK SIP decodes mech-parameters list failed.
 *
 * @see Sip_DecodeMechParms
 */
ZFUNC ZINT Sip_DecodeMechParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MECH_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes mech-parameters.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP mech-parameters.
 *
 * @return ZOK SIP decodes mech-parameters successfully.
 * @return ZOK SIP decodes mech-parameters failed.
 *
 * @see Sip_DecodeMechParmsLst
 */
ZFUNC ZINT Sip_DecodeMechParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MECH_PARMS *pstParms);

/**
 * @brief SIP decodes Billing-Info-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP Billing-Info-param list.
 *
 * @return ZOK SIP decodes Billing-Info-param list successfully.
 * @return ZOK SIP decodes Billing-Info-param list failed.
 *
 * @see Sip_DecodeBillInfoParm
 */
ZFUNC ZINT Sip_DecodeBillInfoParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BILL_INFO_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes Billing-Info-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP Billing-Info-param.
 *
 * @return ZOK SIP decodes Billing-Info-param successfully.
 * @return ZOK SIP decodes Billing-Info-param failed.
 *
 * @see Sip_DecodeBillInfoParmLst
 */
ZFUNC ZINT Sip_DecodeBillInfoParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BILL_INFO_PARM *pstParm);

/**
 * @brief SIP decodes Laes-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP Laes-param list.
 *
 * @return ZOK SIP decodes Laes-param list successfully.
 * @return ZOK SIP decodes Laes-param list failed.
 *
 * @see Sip_DecodeLaesParm
 */
ZFUNC ZINT Sip_DecodeLaesParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LAES_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes Laes-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP Laes-param.
 *
 * @return ZOK SIP decodes Laes-param successfully.
 * @return ZOK SIP decodes Laes-param failed.
 *
 * @see Sip_DecodeLaesParmLst
 */
ZFUNC ZINT Sip_DecodeLaesParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LAES_PARM *pstParm);

/**
 * @brief SIP decodes redir-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP redir-params list.
 *
 * @return ZOK SIP decodes redir-params list successfully.
 * @return ZOK SIP decodes redir-params list failed.
 *
 * @see Sip_DecodeRedirParms
 */
ZFUNC ZINT Sip_DecodeRedirParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REDIR_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes redir-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP redir-params.
 *
 * @return ZOK SIP decodes redir-params successfully.
 * @return ZOK SIP decodes redir-params failed.
 *
 * @see Sip_DecodeRedirParmsLst
 */
ZFUNC ZINT Sip_DecodeRedirParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REDIR_PARMS *pstParms);

/**
 * @brief SIP decodes hi-entry list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiEntryLst SIP hi-entry list.
 *
 * @return ZOK SIP decodes hi-entry list successfully.
 * @return ZOK SIP decodes hi-entry list failed.
 *
 * @see Sip_DecodeHiEntry
 */
ZFUNC ZINT Sip_DecodeHiEntryLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_ENTRY_LST *pstHiEntryLst);

/**
 * @brief SIP decodes hi-entry.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiEntry SIP hi-entry.
 *
 * @return ZOK SIP decodes hi-entry successfully.
 * @return ZOK SIP decodes hi-entry failed.
 *
 * @see Sip_DecodeHiEntryLst
 */
ZFUNC ZINT Sip_DecodeHiEntry(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_ENTRY *pstHiEntry);

/**
 * @brief SIP decodes hi-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiParmLst SIP hi-param list.
 *
 * @return ZOK SIP decodes hi-param list successfully.
 * @return ZOK SIP decodes hi-param list failed.
 *
 * @see Sip_DecodeHiParm
 */
ZFUNC ZINT Sip_DecodeHiParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_PARM_LST *pstHiParmLst);

/**
 * @brief SIP decodes hi-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiParm SIP hi-param.
 *
 * @return ZOK SIP decodes hi-param successfully.
 * @return ZOK SIP decodes hi-param failed.
 *
 * @see Sip_DecodeHiParmLst
 */
ZFUNC ZINT Sip_DecodeHiParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_PARM *pstHiParm);

/**
 * @brief SIP decodes DiameterURI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDiamUri SIP DiameterURI.
 *
 * @return ZOK SIP decodes DiameterURI successfully.
 * @return ZOK SIP decodes DiameterURI failed.
 *
 * @see
 */

ZFUNC ZINT Sip_DecodeDiamUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIAM_URI *pstDiamUri);

/**
 * @brief SIP decodes answer-mode-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstModeVal SIP answer-mode-value.
 *
 * @return ZOK SIP decodes answer-mode-value successfully.
 * @return ZOK SIP decodes answer-mode-value failed.
 *
 * @see
 */
ZFUNC ZINT Sip_DecodeAnswerModeVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ANSWER_MODE_VAL *pstModeVal);

/**
 * @brief SIP decodes answer-mode-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP answer-mode-param list.
 *
 * @return ZOK SIP decodes answer-mode-param list successfully.
 * @return ZOK SIP decodes answer-mode-param list failed.
 *
 * @see Sip_DecodeAnswerModeParm
 */
ZFUNC ZINT Sip_DecodeAnswerModeParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ANSWER_MODE_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes answer-mode-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP answer-mode-param.
 *
 * @return ZOK SIP decodes answer-mode-param successfully.
 * @return ZOK SIP decodes answer-mode-param failed.
 *
 * @see Sip_DecodeAnswerModeParmLst
 */
ZFUNC ZINT Sip_DecodeAnswerModeParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ANSWER_MODE_PARM *pstParm);

/**
 * @brief SIP decodes Service-ID list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstIdLst SIP Service-ID list.
 *
 * @return ZOK SIP decodes Service-ID list successfully.
 * @return ZOK SIP decodes Service-ID list failed.
 *
 * @see Sip_DecodeSrvId
 */
ZFUNC ZINT Sip_DecodeSrvIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SRV_ID_LST *pstIdLst);

/**
 * @brief SIP decodes Service-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstId SIP Service-ID.
 *
 * @return ZOK SIP decodes Service-ID successfully.
 * @return ZOK SIP decodes Service-ID failed.
 *
 * @see Sip_DecodeSrvIdLst
 */
ZFUNC ZINT Sip_DecodeSrvId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SRV_ID *pstId);

/**
 * @brief SIP decodes HCOLON.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 *
 * @return ZOK SIP decodes HCOLON successfully.
 * @return ZOK SIP decodes HCOLON failed.
 */
ZFUNC ZINT Sip_DecodeSepaHColon(ST_ABNF_MSG *pstAbnfMsg);

/**
 * @brief SIP decodes STAR.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode STAR. 
 *  If no STAR, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes STAR successfully.
 * @return ZOK SIP decodes STAR failed.
 */
ZFUNC ZINT Sip_DecodeSepaStar(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes SLASH.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode SLASH. 
 *  If no SLASH, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes SLASH successfully.
 * @return ZOK SIP decodes SLASH failed.
 */
ZFUNC ZINT Sip_DecodeSepaSlash(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes EQUAL.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode EQUAL. 
 *  If no EQUAL, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes EQUAL successfully.
 * @return ZOK SIP decodes EQUAL failed.
 */
ZFUNC ZINT Sip_DecodeSepaEqual(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes LPAREN.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode LPAREN. 
 *  If no LPAREN, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes LPAREN successfully.
 * @return ZOK SIP decodes LPAREN failed.
 */
ZFUNC ZINT Sip_DecodeSepaLparen(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes RPAREN.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode RPAREN. 
 *  If no RPAREN, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes RPAREN successfully.
 * @return ZOK SIP decodes RPAREN failed.
 */
ZFUNC ZINT Sip_DecodeSepaRparen(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes RAQUOT.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode RAQUOT. 
 *  If no RAQUOT, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes RAQUOT successfully.
 * @return ZOK SIP decodes RAQUOT failed.
 */
ZFUNC ZINT Sip_DecodeSepaRaquot(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes LAQUOT.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode LAQUOT. 
 *  If no LAQUOT, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes LAQUOT successfully.
 * @return ZOK SIP decodes LAQUOT failed.
 * 
 */
ZFUNC ZINT Sip_DecodeSepaLaquot(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes COMMA.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode COMMA. 
 *  If no COMMA, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes COMMA successfully.
 * @return ZOK SIP decodes COMMA failed.
 * 
 */
ZFUNC ZINT Sip_DecodeSepaComma(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes SEMI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode SEMI. 
 *  If no SEMI, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes SEMI successfully.
 * @return ZOK SIP decodes SEMI failed.
 * 
 */
ZFUNC ZINT Sip_DecodeSepaSemi(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes COLON.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode COLON. 
 *  If no COLON, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes COLON successfully.
 * @return ZOK SIP decodes COLON failed.
 * 
 */
ZFUNC ZINT Sip_DecodeSepaColon(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes LDQUOT.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode LDQUOT. 
 *  If no LDQUOT, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes LDQUOT successfully.
 * @return ZOK SIP decodes LDQUOT failed.
 */
ZFUNC ZINT Sip_DecodeSepaLdquot(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes RDQUOT.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] bTry Try to decode RDQUOT. 
 *  If no RDQUOT, it will return ZOK if bTry is ZTRUE.
 *
 * @return ZOK SIP decodes RDQUOT successfully.
 * @return ZOK SIP decodes RDQUOT failed.
 */
ZFUNC ZINT Sip_DecodeSepaRdquot(ST_ABNF_MSG *pstAbnfMsg, ZBOOL bTry);

/**
 * @brief SIP decodes early media parameter list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst Early media parameter list.
 *
 * @return ZOK SIP decodes early media parameter list successfully.
 * @return ZOK SIP decodes early media parameter list failed.
 */
ZFUNC ZINT Sip_DecodePEarlyMediaParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EM_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes early media parameter.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm Early media parameter.
 *
 * @return ZOK SIP decodes early media parameter successfully.
 * @return ZOK SIP decodes early media parameter failed.
 */
ZFUNC ZINT Sip_DecodePEarlyMediaParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EM_PARM *pstParm);

/**
 * @brief SIP decodes STAR.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInstance Instance parameter.
 *
 * @return ZOK SIP decodes STAR successfully.
 * @return ZOK SIP decodes STAR failed.
 */
ZFUNC ZINT Sip_DecodeCPInstance(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstInstance);

/**
 * @brief SIP decodes target-dialog parm list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst target-dialog parm list.
 *
 * @return ZOK SIP decodes target-dialog parm list successfully.
 * @return ZOK SIP decodes target-dialog parm list failed.
 */
ZFUNC ZINT Sip_DecodeTDParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TD_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes target-dialog parm.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm target-dialog parm.
 *
 * @return ZOK SIP decodes target-dialog parm successfully.
 * @return ZOK SIP decodes target-dialog parm failed.
 */
ZFUNC ZINT Sip_DecodeTDParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TD_PARM *pstParm);

/**
 * @brief SIP decodes Uri-list-entry list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm Uri-list-entry list.
 *
 * @return ZOK SIP decodes Uri-list-entry list successfully.
 * @return ZOK SIP decodes Uri-list-entry list failed.
 */
ZFUNC ZINT Sip_DecodeUriEntryLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_ENTRY_LST *pstUriEntryLst);

/**
 * @brief SIP decodes Uri-list-entry.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm Uri-list-entry.
 *
 * @return ZOK SIP decodes Uri-list-entry successfully.
 * @return ZOK SIP decodes Uri-list-entry failed.
 */
ZFUNC ZINT Sip_DecodeUriEntry(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_ENTRY *pstUriEntry);

/**
 * @brief SIP decodes refused-parm list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm refused-parm list.
 *
 * @return ZOK SIP decodes refused-parm list successfully.
 * @return ZOK SIP decodes refused-parm list failed.
 */
ZFUNC ZINT Sip_DecodeRefusedParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REFUSED_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes refused-parm.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm refused-parm.
 *
 * @return ZOK SIP decodes refused-parm successfully.
 * @return ZOK SIP decodes refused-parm failed.
 */
ZFUNC ZINT Sip_DecodeRefusedParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REFUSED_PARM *pstParm);

/**
 * @brief SIP decodes X-ZTE-Cause header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes X-ZTE-Cause header successfully.
 * @return ZFAILED SIP decodes X-ZTE-Cause header failed.
 *
 */
ZFUNC ZINT Sip_DecodeHdrZTECause(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes ServInfo header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes ServInfo header successfully.
 * @return ZFAILED SIP decodes ServInfo header failed.
 *
 */
ZFUNC ZINT Sip_DecodeHdrServInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes ServInfo-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP ServInfo-param list.
 *
 * @return ZOK SIP decodes ServInfo-param list successfully.
 * @return ZOK SIP decodes ServInfo-param list failed.
 *
 * @see Sip_DecodeServInfoParm, Sip_DecodeServInfoParmsLst,
 *      Sip_DecodeServInfoParms
 */
ZFUNC ZINT Sip_DecodeServInfoParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARM_LST *pstParmLst);

/**
 * @brief SIP decodes ServInfo-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP ServInfo-param.
 *
 * @return ZOK SIP decodes ServInfo-param successfully.
 * @return ZOK SIP decodes ServInfo-param failed.
 *
 * @see Sip_DecodeServInfoParmLst, Sip_DecodeServInfoParmsLst,
 *      Sip_DecodeServInfoParms
 */
ZFUNC ZINT Sip_DecodeServInfoParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARM *pstParm);

/**
 * @brief SIP decodes ServInfo-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP ServInfo-params list.
 *
 * @return ZOK SIP decodes ServInfo-params list successfully.
 * @return ZOK SIP decodes ServInfo-params list failed.
 *
 * @see Sip_DecodeServInfoParmLst, Sip_DecodeServInfoParm,
 *      Sip_DecodeServInfoParms
 */
ZFUNC ZINT Sip_DecodeServInfoParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARMS_LST *pstParmsLst);

/**
 * @brief SIP decodes ServInfo-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP ServInfo-params.
 *
 * @return ZOK SIP decodes ServInfo-params successfully.
 * @return ZOK SIP decodes ServInfo-params failed.
 *
 * @see Sip_DecodeServInfoParm, Sip_DecodeServInfoParmsLst,
 *      Sip_DecodeServInfoParmsLst
 */
ZFUNC ZINT Sip_DecodeServInfoParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARMS *pstParms);

/**
 * @brief SIP decodes Conference-Info  header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes Conference-Info  header successfully.
 * @return ZFAILED SIP decodes Conference-Info  header failed.
 *
 */
ZFUNC ZINT Sip_DecodeHdrConfInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes TerminalSeatType header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes TerminalSeatType header successfully.
 * @return ZFAILED SIP decodes TerminalSeatType header failed.
 *
 */
ZFUNC ZINT Sip_DecodeHdrTermSeatType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP decodes STS-Call-ID header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP decodes STS-Call-ID header successfully.
 * @return ZFAILED SIP decodes STS-Call-ID header failed.
 *
 */
ZINT Sip_DecodeHdrStsCallID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_ABNF_DECODE_H__ */

