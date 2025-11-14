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
  File name     : sip_abnf_encode.h
  Module        : sip abnf encode
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-05-16
  Description   :
    Marcos and structure definitions required by the sip abnf encode.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_ABNF_ENCODE_H__
#define _SIP_ABNF_ENCODE_H__

/**
 * @file
 * @brief SIP ABNF encode interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Log error message and return ZFAILED. */
#define SIP_ENCODE_RETFAIL(_msg, _info)  do { \
    ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
    return ZFAILED; \
} while (0)

/** @brief Check return value. If ZFAILED, log error message and return ZFAILED. */
#define SIP_ENCODE_CHKRET(_msg, _ret, _info)  do { \
    if (_ret != ZOK) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/** @brief Chec presence flag. If ZFALSE, log error message and return ZFAILED. */
#define SIP_ENCODE_CHKPRES(_msg, _pres, _info) do { \
    if (_pres != ZTRUE) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/*lint -save -e* */

/** @brief Add a character. */
#define SIP_ADD_CHR(_msg, _chr, _ret) \
    _ret = Abnf_AddPstChr(_msg, _chr)

/** @brief Add a token. */
#define SIP_ADD_TKN(_msg, _tkntype, _tknid, _ret) \
    _ret = Sip_TknEncode(_msg, _tkntype, _tknid)

/** @brief Add a token */
#define SIP_ADD_TKNX(_msg, _tkntab, _tknid, _ret) \
    _ret = Abnf_AddTkn(_msg, ZOS_GET_TABLE_INFOX(_tkntab), _tknid)

/** @brief Add a string. */
#define SIP_ADD_SSTR(_msg, _ststr, _ret) \
    _ret = Abnf_AddPstSStr(_msg, _ststr)

/** @brief Add a long string. */
#define SIP_ADD_LSSTR(_msg, _ststr, _ret) \
    _ret = Abnf_AddPstLSStr(_msg, _ststr)

/** @brief Add a quoted string. */
#define SIP_ADD_QSTR(_msg, _ststr, _ret) \
    _ret = Abnf_AddPairSStr(_msg, '"', _ststr)

/** @brief Add a string with length. */
#define SIP_ADD_STRN(_msg, _str, _len, _ret) \
    _ret = Abnf_AddPstStrN(_msg, _str, _len)

/** @brief Add unsigned int decimal data. */
#define SIP_ADD_UIDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUiDigit(_msg, _data)

/** @brief Add unsigned short decimal data. */
#define SIP_ADD_USDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUsDigit(_msg, _data)

/** @brief Add unsigned char decimal data. */
#define SIP_ADD_UCDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUcDigit(_msg, _data)

/*lint -restore */

/** @brief Add SP. */
#define SIP_ADD_SP(_msg, _ret) SIP_ADD_CHR(_msg, ' ', _ret)

/** @brief Add WSP = SP / HTAB. */
#define SIP_ADD_WSP SIP_ADD_SP

/** @brief Add LWS = [*WSP CRLF] 1*WSP. */
#define SIP_ADD_LWS SIP_ADD_WSP

/** @brief Add SWS = [LWS]. */
#define SIP_ADD_SWS 

/** @brief Add HCOLON  =  *( SP / HTAB ) ":" SWS. */
#define SIP_ADD_HCOLON(_msg, _ret) SIP_ADD_CHR(_msg, ':', _ret)

/** @brief Add STAR = SWS "*" SWS ; asterisk. */
#define SIP_ADD_STAR(_msg, _ret) SIP_ADD_CHR(_msg, '*', _ret)

/** @brief Add SLASH = SWS "/" SWS ; slash. */
#define SIP_ADD_SLASH(_msg, _ret) SIP_ADD_CHR(_msg, '/', _ret)

/** @brief Add EQUAL = SWS "=" SWS ; equal. */
#define SIP_ADD_EQUAL(_msg, _ret) SIP_ADD_CHR(_msg, '=', _ret)

/** @brief Add LPAREN = SWS "(" SWS ; left parenthesis. */
#define SIP_ADD_LPAREN(_msg, _ret) SIP_ADD_CHR(_msg, '(', _ret)

/** @brief Add RPAREN = SWS ")" SWS ; right parenthesis. */
#define SIP_ADD_RPAREN(_msg, _ret) SIP_ADD_CHR(_msg, ')', _ret)

/** @brief Add RAQUOT = ">" SWS ; right angle quote. */
#define SIP_ADD_RAQUOT(_msg, _ret) SIP_ADD_CHR(_msg, '>', _ret)

/** @brief Add LAQUOT = SWS "<" ; left angle quote. */
#define SIP_ADD_LAQUOT(_msg, _ret) SIP_ADD_CHR(_msg, '<', _ret)

/** @brief Add COMMA = SWS "," SWS ; comma. */
#define SIP_ADD_COMMA(_msg, _ret) SIP_ADD_CHR(_msg, ',', _ret)

/** @brief Add SEMI = SWS ";" SWS ; semicolon. */
#define SIP_ADD_SEMI(_msg, _ret) SIP_ADD_CHR(_msg, ';', _ret)

/** @brief Add COLON = SWS ":" SWS ; colon. */
#define SIP_ADD_COLON(_msg, _ret) SIP_ADD_CHR(_msg, ':', _ret)

/** @brief Add LDQUOT = SWS DQUOTE; open double quotation mark. */
#define SIP_ADD_LDQUOT(_msg, _ret) SIP_ADD_CHR(_msg, '"', _ret)

/** @brief Add RDQUOT = DQUOTE SWS ; close double quotation mark. */
#define SIP_ADD_RDQUOT(_msg, _ret) SIP_ADD_CHR(_msg, '"', _ret)

/** @brief Add CRLF. */
#define SIP_ADD_CRLF(_msg, _ret) SIP_ADD_STRN(_msg, "\r\n", 2, _ret)

/** @brief Add IP in the buffer. */
#define SIP_ADD_IPV4(_msg, _ip, _ret) _ret = Abnf_AddIpV4(_msg, _ip)

/** @brief Add IPv6 in the buffer. */
#define SIP_ADD_IPV6(_msg, _ipv6, _ret) _ret = Abnf_AddIpV6(_msg, _ipv6)

/** 
 * @brief SIP encodes message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSipMsg SIP message is the result of a decoded ABNF message.
 *
 * @return ZOK SIP encodes message successfully.
 * @return ZOK SIP encodes message failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeMsg(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_MSG *pstSipMsg);

/**
 * @brief SIP encodes request line.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstReqLine SIP request line structure.
 *
 * @return ZOK SIP encodes request line successfully.
 * @return ZOK SIP encodes request line failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeReqLine(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REQ_LINE *pstReqLine);

/**
 * @brief SIP encodes status line.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstStatusLine SIP status line structure.
 *
 * @return ZOK SIP encodes status line successfully.
 * @return ZOK SIP encodes status line failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeStatusLine(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_STATUS_LINE *pstStatusLine);

/**
 * @brief SIP encodes version.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVer SIP version structure.
 *
 * @return ZOK SIP encodes version successfully.
 * @return ZOK SIP encodes version failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeVer(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_VER *pstVer);

/**
 * @brief SIP encodes body.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdrLst SIP massage header list structure.
 * @param [in] pstBody SIP body structure.
 * @param [out] pzNewBuf New created buffer contains SIP message.
 *
 * @return ZOK SIP encodes body successfully.
 * @return ZOK SIP encodes body failed.
 *
 * @see Sip_EncodeBodyX, Sip_EncodeBodyMpartLst, Sip_EncodeBodyMpartLstX,
 *      Sip_EncodeBodyMpart, Sip_EncodeBodySpart, Sip_EncodeBodySpartX,
 *      Sip_EncodeBodySfrag
 */
ZFUNC ZINT Sip_EncodeBody(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR_LST *pstHdrLst, ST_SIP_BODY *pstBody, 
                ZDBUF *pzNewBuf);

/**
 * @brief SIP encodes body.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstBody SIP body structure.
 * @param [in] pstBoundary Boundary string.
 * @param [out] pzNewBuf New created buffer contains SIP message.
 *
 * @return ZOK SIP encodes body successfully.
 * @return ZOK SIP encodes body failed.
 *
 * @see Sip_EncodeBody, Sip_EncodeBodyMpartLst, Sip_EncodeBodyMpartLstX,
 *      Sip_EncodeBodyMpart, Sip_EncodeBodySpart, Sip_EncodeBodySpartX,
 *      Sip_EncodeBodySfrag
 */
ZFUNC ZINT Sip_EncodeBodyX(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_BODY *pstBody, 
                ST_ZOS_SSTR *pstBoundary, ZDBUF *pzNewBuf);

/**
 * @brief SIP encodes body mutlipart list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstMpartLst SIP body with mutlipart list.
 * @param [in] pstBoundary Boundary string.
 *
 * @return ZOK SIP encodes body mutlipart list successfully.
 * @return ZOK SIP encodes body mutlipart list failed.
 *
 * @see Sip_EncodeBody, Sip_EncodeBodyX, Sip_EncodeBodyMpartLstX,
 *      Sip_EncodeBodyMpart, Sip_EncodeBodySpart, Sip_EncodeBodySpartX,
 *      Sip_EncodeBodySfrag
 */
ZFUNC ZINT Sip_EncodeBodyMpartLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_MPART_LST *pstMpartLst, ST_ZOS_SSTR *pstBoundary);

/**
 * @brief SIP encodes body with mutlipart.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstMpart SIP body with mutlipart stucture.
 * @param [in] pstBoundary Boundary string.
 *
 * @return ZOK SIP encodes body mutlipart successfully.
 * @return ZOK SIP encodes body mutlipart failed.
 *
 * @see Sip_EncodeBody, Sip_EncodeBodyX, Sip_EncodeBodyMpartLst,
 *      Sip_EncodeBodyMpartLstX, Sip_EncodeBodySpart, Sip_EncodeBodySpartX,
 *      Sip_EncodeBodySfrag
 */
ZFUNC ZINT Sip_EncodeBodyMpart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_MPART *pstMpart, ST_ZOS_SSTR *pstBoundary);

/**
 * @brief SIP encodes body single part.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSpart SIP body with single part stucture.
 *
 * @return ZOK SIP encodes body single part successfully.
 * @return ZOK SIP encodes body single part failed.
 *
 * @see Sip_EncodeBody, Sip_EncodeBodyX, Sip_EncodeBodyMpartLst,
 *      Sip_EncodeBodyMpartLstX, Sip_EncodeBodyMpart, Sip_EncodeBodySpartX,
 *      Sip_EncodeBodySfrag
 */
ZFUNC ZINT Sip_EncodeBodySpart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_SPART *pstSpart);

/**
 * @brief SIP encodes body sipfrag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSfrag SIP body sipfrag. Detail infomation refers rfc3420.
 *
 * @return ZOK SIP encodes body sipfrag successfully.
 * @return ZOK SIP encodes body sipfrag failed.
 *
 * @see Sip_EncodeBody, Sip_EncodeBodyX, Sip_EncodeBodyMpartLst,
 *      Sip_EncodeBodyMpartLstX, Sip_EncodeBodyMpart, Sip_EncodeBodySpart,
 *      Sip_EncodeBodySpartX
 */
ZFUNC ZINT Sip_EncodeBodySfrag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BODY_SFRAG *pstSfrag);

/**
 * @brief SIP encodes message header list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdrLst SIP message header list.
 *
 * @return ZOK SIP encodes message header list successfully.
 * @return ZOK SIP encodes message header list failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeMsgHdrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR_LST *pstHdrLst);

/**
 * @brief SIP encodes Accept header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Accept header successfully.
 * @return ZOK SIP encodes Accept header failed.
 *
 * @see Sip_EncodeHdrAcptEncoding, Sip_EncodeHdrAcptLang
 */
ZFUNC ZINT Sip_EncodeHdrAcpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Accept-Encoding header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Accept-Encoding header successfully.
 * @return ZOK SIP encodes Accept-Encoding header failed.
 *
 * @see Sip_EncodeHdrAcpt, Sip_EncodeHdrAcptLang
 */
ZFUNC ZINT Sip_EncodeHdrAcptEncoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Accept-Language header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Accept-Language header successfully.
 * @return ZOK SIP encodes Accept-Language header failed.
 *
 * @see Sip_EncodeHdrAcpt, Sip_EncodeHdrAcptEncoding
 */
ZFUNC ZINT Sip_EncodeHdrAcptLang(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Alert-Info header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Alert-Info header successfully.
 * @return ZOK SIP encodes Alert-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrAlertInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Allow header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Allow header successfully.
 * @return ZOK SIP encodes Allow header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrAllow(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Authentication-Info header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Authentication-Info header successfully.
 * @return ZOK SIP encodes Authentication-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrAuthenInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Authorization header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Authorization header successfully.
 * @return ZOK SIP encodes Authorization header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrAuthor(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Call-ID header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Call-ID header successfully.
 * @return ZOK SIP encodes Call-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrCallId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Call-Info header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Call-Info header successfully.
 * @return ZOK SIP encodes Call-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrCallInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Contact header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Contact header successfully.
 * @return ZOK SIP encodes Contact header failed.
 *
 * @see Sip_EncodeHdrContentDisp, Sip_EncodeHdrContentEncoding,
 *      Sip_EncodeHdrContentLang, Sip_EncodeHdrContentLen,
 *      Sip_EncodeHdrContentType
 */
ZFUNC ZINT Sip_EncodeHdrContact(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Content-Disposition header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Content-Disposition header successfully.
 * @return ZOK SIP encodes Content-Disposition header failed.
 *
 * @see Sip_EncodeHdrContact, Sip_EncodeHdrContentEncoding,
 *      Sip_EncodeHdrContentLang, Sip_EncodeHdrContentLen,
 *      Sip_EncodeHdrContentType
 */
ZFUNC ZINT Sip_EncodeHdrContentDisp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Content-Encoding header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Content-Encoding header successfully.
 * @return ZOK SIP encodes Content-Encoding header failed.
 *
 * @see Sip_EncodeHdrContact, Sip_EncodeHdrContentDisp,
 *      Sip_EncodeHdrContentLang, Sip_EncodeHdrContentLen,
 *      Sip_EncodeHdrContentType
 */
ZFUNC ZINT Sip_EncodeHdrContentEncoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Content-Lang header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Content-Lang header successfully.
 * @return ZOK SIP encodes Content-Lang header failed.
 *
 * @see Sip_EncodeHdrContact, Sip_EncodeHdrContentDisp,
 *      Sip_EncodeHdrContentEncoding, Sip_EncodeHdrContentLen,
 *      Sip_EncodeHdrContentType
 */
ZFUNC ZINT Sip_EncodeHdrContentLang(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Content-Length header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Content-Length header successfully.
 * @return ZOK SIP encodes Content-Length header failed.
 *
 * @see Sip_EncodeHdrContact, Sip_EncodeHdrContentDisp,
 *      Sip_EncodeHdrContentEncoding, Sip_EncodeHdrContentLang,
 *      Sip_EncodeHdrContentType
 */
ZFUNC ZINT Sip_EncodeHdrContentLen(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Content-Type header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Content-Type header successfully.
 * @return ZOK SIP encodes Content-Type header failed.
 *
 * @see Sip_EncodeHdrContact, Sip_EncodeHdrContentDisp,
 *      Sip_EncodeHdrContentEncoding, Sip_EncodeHdrContentLang,
 *      Sip_EncodeHdrContentLen
 */
ZFUNC ZINT Sip_EncodeHdrContentType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes CSeq header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes CSeq header successfully.
 * @return ZOK SIP encodes CSeq header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrCseq(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Date header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Date header successfully.
 * @return ZOK SIP encodes Date header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrDate(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Error-Info header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Error-Info header successfully.
 * @return ZOK SIP encodes Error-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrErrorInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Expires header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Expires header successfully.
 * @return ZOK SIP encodes Expires header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrExpires(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes From or To header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes From or To header successfully.
 * @return ZOK SIP encodes From or To header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrFromTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes In-Reply-To header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes In-Reply-To header successfully.
 * @return ZOK SIP encodes In-Reply-To header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrInReplyTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Max-Forwards header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Max-Forwards header successfully.
 * @return ZOK SIP encodes Max-Forwards header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrMaxForwards(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes MIME-Version header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes MIME-Version header successfully.
 * @return ZOK SIP encodes MIME-Version header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrMimeVer(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Min-Expires header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes MIME-Expires header successfully.
 * @return ZOK SIP encodes MIME-Expires header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrMinExpires(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Organization header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Organization header successfully.
 * @return ZOK SIP encodes Organization header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrOrganization(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Priority header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Priority header successfully.
 * @return ZOK SIP encodes Priority header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPriority(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Proxy-Authenticate header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Proxy-Authenticate header successfully.
 * @return ZOK SIP encodes Proxy-Authenticate header failed.
 *
 * @see Sip_EncodeHdrProxyAuthor, Sip_EncodeHdrProxyRequire
 */
ZFUNC ZINT Sip_EncodeHdrProxyAuthen(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Proxy-Authorization  header. Detail information refers
 *        rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Proxy-Authorization  header successfully.
 * @return ZOK SIP encodes Proxy-Authorization  header failed.
 *
 * @see Sip_EncodeHdrProxyAuthen, Sip_EncodeHdrProxyRequire
 */
ZFUNC ZINT Sip_EncodeHdrProxyAuthor(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Proxy-Require  header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Proxy-Require  header successfully.
 * @return ZOK SIP encodes Proxy-Require  header failed.
 *
 * @see Sip_EncodeHdrProxyAuthen, Sip_EncodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_EncodeHdrProxyRequire(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Proxy-Require  header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Proxy-Require  header successfully.
 * @return ZOK SIP encodes Proxy-Require  header failed.
 *
 * @see Sip_EncodeHdrProxyAuthen, Sip_EncodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_EncodeHdrReplyTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);
/**
 * @brief SIP encodes Proxy-Require  header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Proxy-Require  header successfully.
 * @return ZOK SIP encodes Proxy-Require  header failed.
 *
 * @see Sip_EncodeHdrProxyAuthen, Sip_EncodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_EncodeHdrRequire(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Retry-After header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Retry-After header successfully.
 * @return ZOK SIP encodes Retry-After header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrRetryAfter(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Route header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Route header successfully.
 * @return ZOK SIP encodes Route header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrRoute(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Server header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Server header successfully.
 * @return ZOK SIP encodes Server header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrServer(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Subject header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Subject header successfully.
 * @return ZOK SIP encodes Subject header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrSubject(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Supported header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Supported header successfully.
 * @return ZOK SIP encodes Supported header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrSupted(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Timestamp header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Timestamp header successfully.
 * @return ZOK SIP encodes Timestamp header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrTimestamp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Unsupported header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Unsupported header successfully.
 * @return ZOK SIP encodes Unsupported header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrUnsupted(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes User-Agent header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes User-Agent header successfully.
 * @return ZOK SIP encodes User-Agent header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrUserAgent(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Via header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Via header successfully.
 * @return ZOK SIP encodes Via header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrVia(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Warning header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Warning header successfully.
 * @return ZOK SIP encodes Warning header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrWarn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes WWW-Authenticate header. Detail information refers rfc3261.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes WWW-Authenticate header successfully.
 * @return ZOK SIP encodes WWW-Authenticate header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrWwwAuthen(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes RAck header. Detail information refers rfc3262.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes RAck header successfully.
 * @return ZOK SIP encodes RAck header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrRack(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes RSeq header. Detail information refers rfc3262.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes RSeq header successfully.
 * @return ZOK SIP encodes RSeq header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrRseq(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Event header. Detail information refers rfc3265.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Event header successfully.
 * @return ZOK SIP encodes Event header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrEvnt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Allow-Events header. Detail information refers rfc3265.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Allow-Events header successfully.
 * @return ZOK SIP encodes Allow-Events header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrAllowEvnts(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Subscription-State header. Detail information refers
 *        rfc3265.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Subscription-State header successfully.
 * @return ZOK SIP encodes Subscription-State header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrSubsSta(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Refer-To header. Detail information refers
 *        rfc3515.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Refer-To header successfully.
 * @return ZOK SIP encodes Refer-To header failed.
 *
 * @see Sip_EncodeHdrReferredBy
 */
ZFUNC ZINT Sip_EncodeHdrReferTo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Referred-By header. Detail information refers
 *        rfc3892.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Referred-By header successfully.
 * @return ZOK SIP encodes Referred-By header failed.
 *
 * @see Sip_EncodeHdrReferTo
 */
ZFUNC ZINT Sip_EncodeHdrReferredBy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Replaces header. Detail information refers
 *        rfc3891.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Replaces header successfully.
 * @return ZOK SIP encodes Replaces header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrReplaces(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Session-Expires header. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Session-Expires header successfully.
 * @return ZOK SIP encodes Session-Expires header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrSessExpires(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Min-SE header. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Min-SE header successfully.
 * @return ZOK SIP encodes Min-SE header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrMinSe(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Request-Disposition header. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Request-Disposition header successfully.
 * @return ZOK SIP encodes Request-Disposition header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrReqDisp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Accept-Contact eader. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Accept-Contact header successfully.
 * @return ZOK SIP encodes Accept-Contact header failed.
 *
 * @see Sip_EncodeHdrRejContact
 */
ZFUNC ZINT Sip_EncodeHdrAcptContact(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Reject-Contact eader. Detail information refers
 *        rfc4028.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Reject-Contact header successfully.
 * @return ZOK SIP encodes Reject-Contact header failed.
 *
 * @see Sip_EncodeHdrAcptContact
 */
ZFUNC ZINT Sip_EncodeHdrRejContact(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Anonymity eader. Detail information refers
 *        draft-ietf-sip-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Anonymity header successfully.
 * @return ZOK SIP encodes Anonymity header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrAnony(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes RPID-Privacy eader. Detail information refers
 *        draft-ietf-sip-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes RPID-Privacy header successfully.
 * @return ZOK SIP encodes RPID-Privacy header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrRpidPrivacy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Remote-Party-ID eader. Detail information refers
 *        draft-ietf-sip-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Remote-Party-ID header successfully.
 * @return ZOK SIP encodes Remote-Party-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrRmtPartyId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes SIP-ETag header. Detail information refers rfc3911.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes SIP-ETag header successfully.
 * @return ZOK SIP encodes SIP-ETag header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrSipEtag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes SIP-If-Match header. Detail information refers rfc3911.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes SIP-If-Match header successfully.
 * @return ZOK SIP encodes SIP-If-Match header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrSipIfMatch(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Join header. Detail information refers rfc3911.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Join header successfully.
 * @return ZOK SIP encodes Join header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrJoin(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Associated-URI header. Detail information refers rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Associated-URI header successfully.
 * @return ZOK SIP encodes P-Associated-URI header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPAsoUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Called-Party-ID header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Called-Party-ID header successfully.
 * @return ZOK SIP encodes P-Called-Party-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPCalledPtyIdSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Visited-Network-ID header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Visited-Network-ID header successfully.
 * @return ZOK SIP encodes P-Visited-Network-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPVnetId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Access-Network-Info header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Access-Network-Info header successfully.
 * @return ZOK SIP encodes P-Access-Network-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPAnetInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Charging-Addr header. Detail information refers rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Charging-Addr header successfully.
 * @return ZOK SIP encodes P-Charging-Addr header failed.
 *
 * @see Sip_EncodeHdrPChargVec
 */
ZFUNC ZINT Sip_EncodeHdrPChargAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Charging-Vector header. Detail information refers
 *        rfc3455.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Charging-Vector header successfully.
 * @return ZOK SIP encodes P-Charging-Vector header failed.
 *
 * @see Sip_EncodeHdrPChargAddr
 */
ZFUNC ZINT Sip_EncodeHdrPChargVec (ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Asserted-Identity header. Detail information refers
 *        rfc3325.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Asserted-Identity header successfully.
 * @return ZOK SIP encodes P-Asserted-Identity header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPAssertedId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Preferred-Identity header. Detail information refers
 *        rfc3325.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Preferred-Identity header successfully.
 * @return ZOK SIP encodes P-Preferred-Identity header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPPreferredId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Media-Authorization header. Detail information refers
 *        rfc3313.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Media-Authorization header successfully.
 * @return ZOK SIP encodes P-Media-Authorization header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPMediaAuthor(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Privacy header. Detail information refers rfc3323.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Privacy header successfully.
 * @return ZOK SIP encodes Privacy header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPrivacy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Reason header. Detail information refers rfc3326.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Reason header successfully.
 * @return ZOK SIP encodes Reason header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrReason(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes security-client header. Detail information refers rfc3329.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes security-client header successfully.
 * @return ZOK SIP encodes security-client header failed.
 *
 * @see Sip_EncodeHdrSecServ, Sip_EncodeHdrSecVerify
 */
ZFUNC ZINT Sip_EncodeHdrSecCli(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes security-server header. Detail information refers rfc3329.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes security-server header successfully.
 * @return ZOK SIP encodes security-server header failed.
 *
 * @see Sip_EncodeHdrSecCli, Sip_EncodeHdrSecVerify
 */
ZFUNC ZINT Sip_EncodeHdrSecServ(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes security-verify header. Detail information refers rfc3329.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes security-verify header successfully.
 * @return ZOK SIP encodes security-verify header failed.
 *
 * @see Sip_EncodeHdrSecCli, Sip_EncodeHdrSecServ
 */
ZFUNC ZINT Sip_EncodeHdrSecVerify(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-DCS-Trace-Party-ID header. Detail information refers
 *        rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-DCS-Trace-Party-ID header successfully.
 * @return ZOK SIP encodes P-DCS-Trace-Party-ID header failed.
 *
 * @see Sip_EncodeHdrPDcsOsps, Sip_EncodeHdrPDcsBillInfo, Sip_EncodeHdrPDcsLaes
 *      Sip_EncodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_EncodeHdrPDcsTracePtyId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-DCS-OSPS header. Detail information refers rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-DCS-OSPS header successfully.
 * @return ZOK SIP encodes P-DCS-OSPS header failed.
 *
 * @see Sip_EncodeHdrPDcsTracePtyId, Sip_EncodeHdrPDcsBillInfo,
 *      Sip_EncodeHdrPDcsLaes, Sip_EncodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_EncodeHdrPDcsOsps(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-DCS-Billing-Info header. Detail information refers
 *        rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-DCS-Billing-Info header successfully.
 * @return ZOK SIP encodes P-DCS-Billing-Info header failed.
 *
 * @see Sip_EncodeHdrPDcsTracePtyId, Sip_EncodeHdrPDcsOsps,
 *      Sip_EncodeHdrPDcsLaes, Sip_EncodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_EncodeHdrPDcsBillInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-DCS-LAES header. Detail information refers rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-DCS-LAES header successfully.
 * @return ZOK SIP encodes P-DCS-LAES header failed.
 *
 * @see Sip_EncodeHdrPDcsTracePtyId, Sip_EncodeHdrPDcsOsps,
 *      Sip_EncodeHdrPDcsBillInfo, Sip_EncodeHdrPDcsRedir
 */
ZFUNC ZINT Sip_EncodeHdrPDcsLaes(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-DCS-Redirect header. Detail information refers rfc3603.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-DCS-Redirect header successfully.
 * @return ZOK SIP encodes P-DCS-Redirect header failed.
 *
 * @see Sip_EncodeHdrPDcsTracePtyId, Sip_EncodeHdrPDcsOsps,
 *      Sip_EncodeHdrPDcsBillInfo, Sip_EncodeHdrPDcsLaes
 */
ZFUNC ZINT Sip_EncodeHdrPDcsRedir(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes History-Info header. Detail information refers rfc4244.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes History-Info header successfully.
 * @return ZOK SIP encodes History-Info header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrHiInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Alerting-Mode header. Detail information refers
 *       draft poc-p-headers.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Alerting-Mode header successfully.
 * @return ZOK SIP encodes P-Alerting-Mode header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPAlertMode(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Answer-State header. Detail information refers
 *       draft-poc-p-headers.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Answer-State header successfully.
 * @return ZOK SIP encodes P-Answer-State header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPAnswerState(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-User-Database header. Detail information refers rfc4457.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-User-Database header successfully.
 * @return ZOK SIP encodes P-User-Database header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPUserDb(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Refer-Sub header. Detail information refers rfc4488.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Refer-Sub header successfully.
 * @return ZOK SIP encodes Refer-Sub header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrReferSub(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Message-ID header. Detail information refers rfc2392.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Message-ID header successfully.
 * @return ZOK SIP encodes Message-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrMsgId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Content-ID header. Detail information refers rfc2392.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Content-ID header successfully.
 * @return ZOK SIP encodes Content-ID header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrContentId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Answer-Mode header. Detail information refers
 *        rfc5373.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Answer-Mode header successfully.
 * @return ZOK SIP encodes Answer-Mode header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrAnswerMode(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Priv-Answer-Mode. Detail information refers
 *        rfc5373.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Priv-Answer-Mode header successfully.
 * @return ZOK SIP encodes Priv-Answer-Mode header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPrivAnswerMode(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Asserted-Service. Detail information refers
 *        draft-drage-sipping-service-identification.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Asserted-Service header successfully.
 * @return ZOK SIP encodes P-Asserted-Service header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPAssertedSrv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Preferred-Service. Detail information refers
 *        draft-drage-sipping-service-identification.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Preferred-Service header successfully.
 * @return ZOK SIP encodes P-Preferred-Service header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrPPreferredSrv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Early-Media header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Early-Media header successfully.
 * @return ZOK SIP encodes P-Early-Media header failed.
 */
ZFUNC ZINT Sip_EncodeHdrPEarlyMedia(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Flow-Timer header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Flow-Timer header successfully.
 * @return ZOK SIP encodes Flow-Timer header failed.
 */
ZFUNC ZINT Sip_EncodeHdrFlowTimer(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Target-Dialog.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Target-Dialog header successfully.
 * @return ZOK SIP encodes Target-Dialog header failed.
 */
ZFUNC ZINT Sip_EncodeHdrTargetDlg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes P-Refused-Uri-List.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes P-Refused-Uri-List header successfully.
 * @return ZOK SIP encodes P-Refused-Uri-List header failed.
 */
ZFUNC ZINT Sip_EncodeHdrPRefusedUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Session-Replaces.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Session-Replaces header successfully.
 * @return ZOK SIP encodes Session-Replaces header failed.
 */
ZFUNC ZINT Sip_EncodeHdrSessReplaces(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Contribution-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Contribution-ID header successfully.
 * @return ZOK SIP encodes Contribution-ID header failed.
 */
ZFUNC ZINT Sip_EncodeHdrContributionID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Conversation-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Conversation-ID header successfully.
 * @return ZOK SIP encodes Conversation-ID header failed.
 */
ZFUNC ZINT Sip_EncodeHdrConversationID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes X-ZTE-Cause .
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes X-ZTE-Cause header successfully.
 * @return ZFAILED SIP encodes X-ZTE-Cause header failed.
 */
ZFUNC ZINT Sip_EncodeHdrZTECause(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);
/**
 * @brief SIP encodes InReplyTo-Contribution-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes InReplyTo-Contribution-ID header successfully.
 * @return ZOK SIP encodes InReplyTo-Contribution-ID header failed.
 */
ZFUNC ZINT Sip_EncodeHdrIrtContID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes Content-Description.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes Content-Description header successfully.
 * @return ZOK SIP encodes Content-Description header failed.
 */
ZFUNC ZINT Sip_EncodeHdrContDescription(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes extension-header. Detail information refers
 *        extension-header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes extension-header header successfully.
 * @return ZOK SIP encodes extension-header header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHdrExtHdr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes request uri.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstReqUri SIP request uri structure.
 *
 * @return ZOK SIP encodes request uri successfully.
 * @return ZOK SIP encodes request uri failed.
 *
 * @see Sip_EncodeReqUriType
 */
ZFUNC ZINT Sip_EncodeReqUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REQ_URI *pstReqUri);

/**
 * @brief SIP encodes accept-tange list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRangeLst SIP accept-tange list.
 *
 * @return ZOK SIP encodes accept-tange list successfully.
 * @return ZOK SIP encodes accept-tange list failed.
 *
 * @see Sip_EncodeAcptRange
 */
ZFUNC ZINT Sip_EncodeAcptRangeLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_RANGE_LST *pstRangeLst);

/**
 * @brief SIP encodes accept-tange.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRange SIP accept-tange.
 *
 * @return ZOK SIP encodes accept-tange successfully.
 * @return ZOK SIP encodes accept-tange failed.
 *
 * @see Sip_EncodeAcptRangeLst
 */
ZFUNC ZINT Sip_EncodeAcptRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_RANGE *pstRange);

/**
 * @brief SIP encodes media-range.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMediaRange SIP media-range.
 *
 * @return ZOK SIP encodes media-range successfully.
 * @return ZOK SIP encodes media-range failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeMediaRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MEDIA_RANGE *pstMediaRange);

/**
 * @brief SIP encodes accept-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP accept-param list.
 *
 * @return ZOK SIP encodes accept-param list successfully.
 * @return ZOK SIP encodes accept-param list failed.
 *
 * @see Sip_EncodeAcptParm
 */
ZFUNC ZINT Sip_EncodeAcptParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes accept-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP Media-Range.
 *
 * @return ZOK SIP encodes accept-param successfully.
 * @return ZOK SIP encodes accept-param failed.
 *
 * @see Sip_EncodeAcptParmLst
 */
ZFUNC ZINT Sip_EncodeAcptParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACPT_PARM *pstParm);

/**
 * @brief SIP encodes qvalue.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQval SIP qvalue.
 *
 * @return ZOK SIP encodes qvalue successfully.
 * @return ZOK SIP encodes qvalue failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeQval(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_QVAL *pstQval);

/**
 * @brief SIP encodes m-parameter list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP M-Parameter list.
 *
 * @return ZOK SIP encodes m-parameter list successfully.
 * @return ZOK SIP encodes v list failed.
 *
 * @see Sip_EncodeMParm
 */
ZFUNC ZINT Sip_EncodeMParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MPARM_LST *pstParmLst);

/**
 * @brief SIP encodes m-parameter.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP m-parameter.
 *
 * @return ZOK SIP encodes m-parameter successfully.
 * @return ZOK SIP encodes m-parameter failed.
 *
 * @see Sip_EncodeMParmLst
 */
ZFUNC ZINT Sip_EncodeMParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MPARM *pstParm);

/**
 * @brief SIP encodes encoding list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEncodingLst SIP encoding list.
 *
 * @return ZOK SIP encodes encoding list successfully.
 * @return ZOK SIP encodes encoding list failed.
 *
 * @see Sip_EncodeMParmLst
 */
ZFUNC ZINT Sip_EncodeEncodingLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ENCODING_LST *pstEncodingLst);

/**
 * @brief SIP encodes encoding.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEncoding SIP Encoding.
 *
 * @return ZOK SIP encodes encoding successfully.
 * @return ZOK SIP encodes encoding failed.
 *
 * @see Sip_EncodeEncodingLst
 */
ZFUNC ZINT Sip_EncodeEncoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ENCODING *pstEncoding);

/**
 * @brief SIP encodes codings.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCodings SIP codings.
 *
 * @return ZOK SIP encodes codings successfully.
 * @return ZOK SIP encodes codings failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeCoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CODINGS *pstCodings);

/**
 * @brief SIP encodes language list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangLst SIP language list.
 *
 * @return ZOK SIP encodes language list successfully.
 * @return ZOK SIP encodes language list failed.
 *
 * @see Sip_EncodeLang
 */
ZFUNC ZINT Sip_EncodeLangLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_LST *pstLangLst);

/**
 * @brief SIP encodes language.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLang SIP language.
 *
 * @return ZOK SIP encodes language successfully.
 * @return ZOK SIP encodes language failed.
 *
 * @see Sip_EncodeLangLst
 */
ZFUNC ZINT Sip_EncodeLang(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_LANG *pstLang);

/**
 * @brief SIP encodes language-range.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangRange SIP language-range.
 *
 * @return ZOK SIP encodes language-range successfully.
 * @return ZOK SIP encodes language-range failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeLangRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_RANGE *pstLangRange);

/**
 * @brief SIP encodes language Subtag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] stSubtagLst SIP Language-Range list.
 *
 * @return ZOK SIP encodes language Subtag list successfully.
 * @return ZOK SIP encodes language Subtag list failed.
 *
 * @see Sip_EncodeLangSubtag
 */
ZFUNC ZINT Sip_EncodeLangSubtagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SSTR_LST *stSubtagLst);

/**
 * @brief SIP encodes language Subtag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] stSubtag Zos signed structure string.
 *
 * @return ZOK SIP encodes language Subtag successfully.
 * @return ZOK SIP encodes language Subtag failed.
 *
 * @see Sip_EncodeLangSubtagLst
 */
ZFUNC ZINT Sip_EncodeLangSubtag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *stSubtag);

/**
 * @brief SIP encodes Alert-Param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlertLst SIP Alert-Param list.
 *
 * @return ZOK SIP encodes Alert-Param list successfully.
 * @return ZOK SIP encodes Alert-Param list failed.
 *
 * @see Sip_EncodeAlertParm
 */
ZFUNC ZINT Sip_EncodeAlertParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALERT_PARM_LST *pstAlertLst);

/**
 * @brief SIP encodes alert-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlert SIP Alert-Param.
 *
 * @return ZOK SIP encodes alert-param successfully.
 * @return ZOK SIP encodes alert-param failed.
 *
 * @see Sip_EncodeAlertParmLst
 */
ZFUNC ZINT Sip_EncodeAlertParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALERT_PARM *pstAlert);

/**
 * @brief SIP encodes absolute URI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAbsoUri SIP absolute URI.
 *
 * @return ZOK SIP encodes absolute URI successfully.
 * @return ZOK SIP encodes absolute URI failed.
 *
 * @see
 */

ZFUNC ZINT Sip_EncodeAbsoUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ABSO_URI *pstAbsoUri);

/**
 * @brief SIP encodes generic-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP generic-param list.
 *
 * @return ZOK SIP encodes generic-param list successfully.
 * @return ZOK SIP encodes generic-param list failed.
 *
 * @see Sip_EncodeGenParm
 */
ZFUNC ZINT Sip_EncodeGenParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_GEN_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes generic-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP generic-param.
 *
 * @return ZOK SIP encodes generic-param successfully.
 * @return ZOK SIP encodes generic-param failed.
 *
 * @see Sip_EncodeGenParmLst
 */
ZFUNC ZINT Sip_EncodeGenParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_GEN_PARM *pstParm);

/**
 * @brief SIP encodes generic-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstGenVal SIP v.
 *
 * @return ZOK SIP encodes generic-value successfully.
 * @return ZOK SIP encodes generic-valuess failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeGenValue(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_GEN_VAL *pstGenVal);

/**
 * @brief SIP encodes method list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMethodLst SIP method list.
 *
 * @return ZOK SIP encodes method list successfully.
 * @return ZOK SIP encodes method list failed.
 *
 * @see Sip_EncodeMethod
 */
ZFUNC ZINT Sip_EncodeMethodLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_METHOD_LST *pstMethodLst);

/**
 * @brief SIP encodes method.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMethod SIP method.
 *
 * @return ZOK SIP encodes method successfully.
 * @return ZOK SIP encodes method failed.
 *
 * @see Sip_EncodeMethodLst
 */
ZFUNC ZINT Sip_EncodeMethod(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_METHOD *pstMethod);

/**
 * @brief SIP encodes ainfo list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAinfoLst SIP ainfo list.
 *
 * @return ZOK SIP encodes ainfo list successfully.
 * @return ZOK SIP encodes ainfo list failed.
 *
 * @see Sip_EncodeAinfo
 */
ZFUNC ZINT Sip_EncodeAinfoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AINFO_LST *pstAinfoLst);

/**
 * @brief SIP encodes ainfo.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAinfo SIP ainfo.
 *
 * @return ZOK SIP encodes ainfo successfully.
 * @return ZOK SIP encodes ainfo failed.
 *
 * @see Sip_EncodeAinfoLst
 */
ZFUNC ZINT Sip_EncodeAinfo(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_AINFO *pstAinfo);

/**
 * @brief SIP encodes info list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfoLst SIP info list.
 *
 * @return ZOK SIP encodes info list successfully.
 * @return ZOK SIP encodes info list failed.
 *
 * @see Sip_EncodeInfo
 */
ZFUNC ZINT Sip_EncodeInfoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_LST *pstInfoLst);

/**
 * @brief SIP encodes info.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfo SIP info.
 *
 * @return ZOK SIP encodes info successfully.
 * @return ZOK SIP encodes info failed.
 *
 * @see Sip_EncodeInfoLst
 */
ZFUNC ZINT Sip_EncodeInfo(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_INFO *pstInfo);

/**
 * @brief SIP encodes info-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP info-param list.
 *
 * @return ZOK SIP encodes info-param list successfully.
 * @return ZOK SIP encodes info-param list failed.
 *
 * @see Sip_EncodeInfoParm, Sip_EncodeInfoPurps
 */
ZFUNC ZINT Sip_EncodeInfoParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes info-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP info-param.
 *
 * @return ZOK SIP encodes info-param successfully.
 * @return ZOK SIP encodes info-param failed.
 *
 * @see Sip_EncodeInfoParmLst, Sip_EncodeInfoPurps
 */
ZFUNC ZINT Sip_EncodeInfoParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_PARM *pstParm);

/**
 * @brief SIP encodes info-param purpose.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPurps SIP info-param purpose.
 *
 * @return ZOK SIP encodes info-param purpose successfully.
 * @return ZOK SIP encodes info-param purpose failed.
 *
 * @see Sip_EncodeInfoParmLst, Sip_EncodeInfoParm
 */
ZFUNC ZINT Sip_EncodeInfoPurps(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_INFO_PURPS *pstPurps);

/**
 * @brief SIP encodes contact-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP contact-param list.
 *
 * @return ZOK SIP encodes contact-param list successfully.
 * @return ZOK SIP encodes contact-param list failed.
 *
 * @see Sip_EncodeContactParm, Sip_EncodeContactParmsLst,
 *      Sip_EncodeContactParms
 */
ZFUNC ZINT Sip_EncodeContactParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes contact-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP contact-param.
 *
 * @return ZOK SIP encodes contact-param successfully.
 * @return ZOK SIP encodes contact-param failed.
 *
 * @see Sip_EncodeContactParmLst, Sip_EncodeContactParmsLst,
 *      Sip_EncodeContactParms
 */
ZFUNC ZINT Sip_EncodeContactParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP encodes contact-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP contact-params list.
 *
 * @return ZOK SIP encodes contact-params list successfully.
 * @return ZOK SIP encodes contact-params list failed.
 *
 * @see Sip_EncodeContactParmLst, Sip_EncodeContactParm,
 *      Sip_EncodeContactParms
 */
ZFUNC ZINT Sip_EncodeContactParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes contact-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP contact-params.
 *
 * @return ZOK SIP encodes contact-params successfully.
 * @return ZOK SIP encodes contact-params failed.
 *
 * @see Sip_EncodeContactParm, Sip_EncodeContactParmsLst,
 *      Sip_EncodeContactParmsLst
 */
ZFUNC ZINT Sip_EncodeContactParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTACT_PARMS *pstParms);

/**
 * @brief SIP encodes contact feature parameter.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP contact feature parameter.
 *
 * @return ZOK SIP encodes contact feature parameter successfully.
 * @return ZOK SIP encodes contact feature parameter failed.
 */
ZFUNC ZINT Sip_EncodeCFeatureParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CFEATURE_PARM *pstParm);

/**
 * @brief SIP encodes tag value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP tag value list.
 *
 * @return ZOK SIP encodes tag value list successfully.
 * @return ZOK SIP encodes tag value list failed.
 */                
ZFUNC ZINT Sip_EncodeTagValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TAG_VAL_LST *pstValLst);
                
/**
 * @brief SIP encodes tag value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTagVal SIP tag value.
 *
 * @return ZOK SIP encodes tag value successfully.
 * @return ZOK SIP encodes tag value failed.
 */                
ZFUNC ZINT Sip_EncodeTagVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstTagVal);

/**
 * @brief SIP encodes string value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP string value.
 *
 * @return ZOK SIP encodes string value successfully.
 * @return ZOK SIP encodes string value failed.
 */
ZFUNC ZINT Sip_EncodeStrVal(ST_ABNF_MSG *pstAbnfMsg, ST_ZOS_SSTR *pstVal);

/**
 * @brief SIP encodes name-addr or addr-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstNaSpec SIP name-addr or addr-spec.
 *
 * @return ZOK SIP encodes name-addr or addr-spec successfully.
 * @return ZOK SIP encodes name-addr or addr-spec failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeNaSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_NA_SPEC *pstNaSpec);

/**
 * @brief SIP encodes name-addr.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstNameAddr SIP name-addr.
 *
 * @return ZOK SIP encodes name-addr successfully.
 * @return ZOK SIP encodes name-addr failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeNameAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_NAME_ADDR *pstNameAddr);

/**
 * @brief SIP encodes display-name.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDispName SIP display-name.
 *
 * @return ZOK SIP encodes display-name successfully.
 * @return ZOK SIP encodes display-name failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeDispName(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DISP_NAME *pstDispName);

/**
 * @brief SIP encodes addr-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAddrSpec SIP addr-spec.
 *
 * @return ZOK SIP encodes addr-spec successfully.
 * @return ZOK SIP encodes addr-spec failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeAddrSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ADDR_SPEC *pstAddrSpec);

/**
 * @brief SIP encodes SIP-URI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSipUri SIP SIP-URI.
 *
 * @return ZOK SIP encodes SIP-URI successfully.
 * @return ZOK SIP encodes SIP-URI failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeSipUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP encodes userinfo.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUserInfo SIP userinfo.
 *
 * @return ZOK SIP encodes userinfo successfully.
 * @return ZOK SIP encodes userinfo failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeUserInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_USER_INFO *pstUserInfo);

/**
 * @brief SIP encodes hostport.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHostPort SIP hostport.
 *
 * @return ZOK SIP encodes hostport successfully.
 * @return ZOK SIP encodes hostport failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHostPort(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HOST_PORT *pstHostPort);

/**
 * @brief SIP encodes host.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHost SIP host.
 *
 * @return ZOK SIP encodes host successfully.
 * @return ZOK SIP encodes host failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHost(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_HOST *pstHost);

/**
 * @brief SIP encodes Ipv6 address.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pucIpv6Addr Ipv6 adress.
 *
 * @return ZOK SIP encodes ipv6 address successfully.
 * @return ZOK SIP encodes ipv6 address failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeIpv6(ST_ABNF_MSG *pstAbnfMsg, ZUCHAR *pucIpv6Addr);

/**
 * @brief SIP encodes quoted string.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQStr Zos signed structure string.
 *
 * @return ZOK SIP encodes quoted string successfully.
 * @return ZOK SIP encodes quoted string failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeQStr(ST_ABNF_MSG *pstAbnfMsg, ST_ZOS_SSTR *pstQStr);

/**
 * @brief SIP encodes uri-parameter list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriParmLst SIP uri-parameter list.
 *
 * @return ZOK SIP encodes uri-parameter list successfully.
 * @return ZOK SIP encodes uri-parameter list failed.
 *
 * @see Sip_EncodeUriParm
 */
ZFUNC ZINT Sip_EncodeUriParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_PARM_LST *pstUriParmLst);

/**
 * @brief SIP encodes uri-parameter.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriParm SIP uri-parameter.
 *
 * @return ZOK SIP encodes uri-parameter successfully.
 * @return ZOK SIP encodes uri-parameter failed.
 *
 * @see Sip_EncodeUriParmLst
 */
ZFUNC ZINT Sip_EncodeUriParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_PARM *pstUriParm);

/**
 * @brief SIP encodes headers.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdrs SIP headers.
 *
 * @return ZOK SIP encodes headers successfully.
 * @return ZOK SIP encodes headers failed.
 *
 * @see Sip_EncodeHdrLst, Sip_EncodeHdr
 */
ZFUNC ZINT Sip_EncodeHdrs(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HDRS *pstHdrs);

/**
 * @brief SIP encodes header list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdrLst SIP header list.
 *
 * @return ZOK SIP encodes header list successfully.
 * @return ZOK SIP encodes header list failed.
 *
 * @see Sip_EncodeHdrs, Sip_EncodeHdr
 */
ZFUNC ZINT Sip_EncodeHdrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HDR_LST *pstHdrLst);

/**
 * @brief SIP encodes header.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP header.
 *
 * @return ZOK SIP encodes header successfully.
 * @return ZOK SIP encodes header failed.
 *
 * @see Sip_EncodeHdrs, Sip_EncodeHdrLst
 */
ZFUNC ZINT Sip_EncodeHdr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HDR *pstHdr);

/**
 * @brief SIP encodes IM-URI. Detail information refers rfc3860.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstImUri SIP IM-URI.
 *
 * @return ZOK SIP encodes IM-URI successfully.
 * @return ZOK SIP encodes IM-URI failed.
 *
 * @see Sip_EncodeMcUri, Sip_EncodeTelUri
 */
ZFUNC ZINT Sip_EncodeImUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_IM_URI *pstImUri);

/**
 * @brief SIP encodes Message-ID & Content-ID uri. Detail information refers
 *        rfc2392.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMcUri SIP Message-ID & Content-ID uri.
 *
 * @return ZOK SIP encodes Message-ID & Content-ID uri successfully.
 * @return ZOK SIP encodes Message-ID & Content-ID uri failed.
 *
 * @see Sip_EncodeImUri, Sip_EncodeTelUri
 */
ZFUNC ZINT Sip_EncodeMcUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MC_URI *pstMcUri);

/**
 * @brief SIP encodes telephone-uri. Detail information refers rfc2806.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTelUri SIP telephone-uri.
 *
 * @return ZOK SIP encodes telephone-uri successfully.
 * @return ZOK SIP encodes telephone-uri failed.
 *
 * @see Sip_EncodeImUri, Sip_EncodeMcUri
 */
ZFUNC ZINT Sip_EncodeTelUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TEL_URI *pstTelUri);

/**
 * @brief SIP encodes par list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParLst SIP par list.
 *
 * @return ZOK SIP encodes par list successfully.
 * @return ZOK SIP encodes par list failed.
 *
 * @see Sip_EncodePar
 */
ZFUNC ZINT Sip_EncodeParLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PAR_LST *pstParLst);

/**
 * @brief SIP encodes par.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPar SIP par.
 *
 * @return ZOK SIP encodes par successfully.
 * @return ZOK SIP encodes par failed.
 *
 * @see Sip_EncodeParLst
 */
ZFUNC ZINT Sip_EncodePar(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PAR *pstPar);

/**
 * @brief SIP encodes disp-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP disp-param list.
 *
 * @return ZOK SIP encodes disp-param list successfully.
 * @return ZOK SIP encodes disp-param list failed.
 *
 * @see Sip_EncodeDispParm
 */
ZFUNC ZINT Sip_EncodeDispParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DISP_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes disp-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP disp-param.
 *
 * @return ZOK SIP encodes disp-param successfully.
 * @return ZOK SIP encodes disp-param failed.
 *
 * @see Sip_EncodeDispParmLst
 */
ZFUNC ZINT Sip_EncodeDispParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DISP_PARM *pstParm);

/**
 * @brief SIP encodes handling-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP handling-param.
 *
 * @return ZOK SIP encodes handling-param successfully.
 * @return ZOK SIP encodes handling-param failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeHandParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HAND_PARM *pstParm);

/**
 * @brief SIP encodes content-coding list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCodingLst SIP content-coding list.
 *
 * @return ZOK SIP encodes content-coding list successfully.
 * @return ZOK SIP encodes content-coding list failed.
 *
 * @see Sip_EncodeContentCoding
 */
ZFUNC ZINT Sip_EncodeContentCodingLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTENT_CODING_LST *pstCodingLst);

/**
 * @brief SIP encodes content-coding.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCoding SIP content-coding.
 *
 * @return ZOK SIP encodes content-coding successfully.
 * @return ZOK SIP encodes content-coding failed.
 *
 * @see Sip_EncodeContentCodingLst
 */
ZFUNC ZINT Sip_EncodeContentCoding(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CONTENT_CODING *pstCoding);

/**
 * @brief SIP encodes language-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangTagLst SIP language-tag list.
 *
 * @return ZOK SIP encodes language-tag list successfully.
 * @return ZOK SIP encodes language-tag list failed.
 *
 * @see Sip_EncodeLangTag
 */
ZFUNC ZINT Sip_EncodeLangTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_TAG_LST *pstLangTagLst);

/**
 * @brief SIP encodes language-tag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstLangTag SIP language-tag.
 *
 * @return ZOK SIP encodes language-tag successfully.
 * @return ZOK SIP encodes language-tag failed.
 *
 * @see Sip_EncodeLangTagLst
 */
ZFUNC ZINT Sip_EncodeLangTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LANG_TAG *pstLangTag);
                
/**
 * @brief SIP encodes sub tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstStrLst SIP sub tag list.
 *
 * @return ZOK SIP encodes language-tag successfully.
 * @return ZOK SIP encodes language-tag failed.
 */
ZFUNC ZINT Sip_EncodeSubTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SSTR_LST *pstStrLst);

/**
 * @brief SIP encodes media-type.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMediaType SIP media-type.
 *
 * @return ZOK SIP encodes media-type successfully.
 * @return ZOK SIP encodes media-type failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeMediaType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MEDIA_TYPE *pstMediaType);

/**
 * @brief SIP encodes SIP-date.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDate SIP SIP-date.
 *
 * @return ZOK SIP encodes SIP-date successfully.
 * @return ZOK SIP encodes SIP-date failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeDate(ST_ABNF_MSG *pstAbnfMsg, ST_SIP_DATE *pstDate);

/**
 * @brief SIP encodes error-uri list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstErrorUriLst SIP error-uri list.
 *
 * @return ZOK SIP encodes error-uri list successfully.
 * @return ZOK SIP encodes error-uri list failed.
 *
 * @see Sip_EncodeErrorUri
 */
ZFUNC ZINT Sip_EncodeErrorUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ERROR_URI_LST *pstErrorUriLst);

/**
 * @brief SIP encodes error-uri.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstErrorUri SIP error-uri.
 *
 * @return ZOK SIP encodes error-uri successfully.
 * @return ZOK SIP encodes error-uri failed.
 *
 * @see Sip_EncodeErrorUriLst
 */
ZFUNC ZINT Sip_EncodeErrorUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ERROR_URI *pstErrorUri);

/**
 * @brief SIP encodes from-param & to-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP from-param & to-param list.
 *
 * @return ZOK SIP encodes from-param & to-param list successfully.
 * @return ZOK SIP encodes from-param & to-param list failed.
 * 
 * @see Sip_EncodeFromToParm
 */
ZFUNC ZINT Sip_EncodeFromToParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FROM_TO_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes from-param & to-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP from-param & to-param.
 *
 * @return ZOK SIP encodes from-param & to-param successfully.
 * @return ZOK SIP encodes from-param & to-param failed.
 *
 * @see Sip_EncodeFromToParmLst
 */
ZFUNC ZINT Sip_EncodeFromToParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FROM_TO_PARM *pstParm);

/**
 * @brief SIP encodes callid list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCallidLst SIP callid list.
 *
 * @return ZOK SIP encodes callid list successfully.
 * @return ZOK SIP encodes callid list failed.
 *
 * @see Sip_EncodeCallid
 */
ZFUNC ZINT Sip_EncodeCallidLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CALLID_LST *pstCallidLst);

/**
 * @brief SIP encodes callid.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCallid SIP callid.
 *
 * @return ZOK SIP encodes callid successfully.
 * @return ZOK SIP encodes callid failed.
 *
 * @see Sip_EncodeCallidLst
 */
ZFUNC ZINT Sip_EncodeCallid(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CALLID *pstCallid);

/**
 * @brief SIP encodes challenge.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstChallenge SIP challenge.
 *
 * @return ZOK SIP encodes challenge successfully.
 * @return ZOK SIP encodes challenge failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeChallenge(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHALLENGE *pstChallenge);

/**
 * @brief SIP encodes digest.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigest SIP digest.
 *
 * @return ZOK SIP encodes digest successfully.
 * @return ZOK SIP encodes digest failed.
 *
 * @see Sip_EncodeDigestClnLst, Sip_EncodeDigestCln
 */
ZFUNC ZINT Sip_EncodeDigest(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST *pstDigest);

/**
 * @brief SIP encodes digest-cln list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigestClnLst SIP digest-cln list.
 *
 * @return ZOK SIP encodes digest-cln list successfully.
 * @return ZOK SIP encodes digest-cln list failed.
 *
 * @see Sip_EncodeDigest, Sip_EncodeDigestCln
 */
ZFUNC ZINT Sip_EncodeDigestClnLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST_CLN_LST *pstDigestClnLst);

/**
 * @brief SIP encodes digest-cln.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigestCln SIP digest-cln.
 *
 * @return ZOK SIP encodes digest-cln successfully.
 * @return ZOK SIP encodes digest-cln failed.
 *
 * @see Sip_EncodeDigest, Sip_EncodeDigestClnLst
 */
ZFUNC ZINT Sip_EncodeDigestCln(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST_CLN *pstDigestCln);

/**
 * @brief SIP encodes domain.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDomain SIP domain.
 *
 * @return ZOK SIP encodes domain successfully.
 * @return ZOK SIP encodes domain failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeDomain(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DOMAIN *pstDomain);

/**
 * @brief SIP encodes algorithm.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlgo SIP algorithm.
 *
 * @return ZOK SIP encodes algorithm successfully.
 * @return ZOK SIP encodes algorithm failed.
 *
 * @see Sip_EncodeAlgoVal
 */
ZFUNC ZINT Sip_EncodeAlgo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALGO *pstAlgo);

/**
 * @brief SIP encodes aka-namespace.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAkaNs SIP aka-namespace.
 *
 * @return ZOK SIP encodes aka-namespace successfully.
 * @return ZOK SIP encodes aka-namespace failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeAkaNs(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AKA_NS *pstAkaNs);

/**
 * @brief SIP encodes algorithm-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAlgoVal SIP algorithm-value.
 *
 * @return ZOK SIP encodes algorithm-value successfully.
 * @return ZOK SIP encodes algorithm-value failed.
 *
 * @see Sip_EncodeAlgo
 */
ZFUNC ZINT Sip_EncodeAlgoVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ALGO_VAL *pstAlgoVal);

/**
 * @brief SIP encodes URI list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriLst SIP URI list.
 *
 * @return ZOK SIP encodes URI list successfully.
 * @return ZOK SIP encodes URI list failed.
 *
 * @see Sip_EncodeUri
 */
ZFUNC ZINT Sip_EncodeUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_LST *pstUriLst);

/**
 * @brief SIP encodes URI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUri SIP URI.
 *
 * @return ZOK SIP encodes URI successfully.
 * @return ZOK SIP encodes URI failed.
 *
 * @see Sip_EncodeUriLst
 */
ZFUNC ZINT Sip_EncodeUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI *pstUri);

/**
 * @brief SIP encodes path-segments.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPathSegments SIP path-segments.
 *
 * @return ZOK SIP encodes path-segments successfully.
 * @return ZOK SIP encodes path-segments failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeAbsPath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PATH_SEGMENTS *pstPathSegments);

/**
 * @brief SIP encodes segment.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSeg SIP segment.
 *
 * @return ZOK SIP encodes segment successfully.
 * @return ZOK SIP encodes segment failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeSegment(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SEGMENT *pstSeg);

/**
 * @brief SIP encodes qop-options.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQopts SIP qop-options.
 *
 * @return ZOK SIP encodes qop-options successfully.
 * @return ZOK SIP encodes qop-options failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeQopts(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_QOPTS *pstQopts);

/**
 * @brief SIP encodes some type value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQopValLst SIP some type value list.
 *
 * @return ZOK SIP encodes some type value list successfully.
 * @return ZOK SIP encodes some type value list failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeQopValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TYPE_VAL_LST *pstQopValLst);

/**
 * @brief SIP encodes some type value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQopVal SIP some type value.
 *
 * @return ZOK SIP encodes some type value successfully.
 * @return ZOK SIP encodes some type value failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeQopVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TYPE_VAL *pstQopVal);

/**
 * @brief SIP encodes other-challenge.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOther SIP other-challenge.
 *
 * @return ZOK SIP encodes other-challenge successfully.
 * @return ZOK SIP encodes other-challenge failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeOtherChallenge(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OTHER_CHALLENGE *pstOther);

/**
 * @brief SIP encodes auth-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP auth-param list.
 *
 * @return ZOK SIP encodes auth-param list successfully.
 * @return ZOK SIP encodes auth-param list failed.
 *
 * @see Sip_EncodeAuthParmLst
 */
ZFUNC ZINT Sip_EncodeAuthParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AUTH_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes auth-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP auth-param.
 *
 * @return ZOK SIP encodes auth-param successfully.
 * @return ZOK SIP encodes auth-param failed.
 *
 * @see Sip_EncodeAuthParm
 */
ZFUNC ZINT Sip_EncodeAuthParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AUTH_PARM *pstParm);

/**
 * @brief SIP encodes credentials.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstCredents SIP credentials.
 *
 * @return ZOK SIP encodes credentials successfully.
 * @return ZOK SIP encodes credentials failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeCredents(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CREDENTS *pstCredents);

/**
 * @brief SIP encodes digest-response.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigestRsp SIP digest-response.
 *
 * @return ZOK SIP encodes digest-response successfully.
 * @return ZOK SIP encodes digest-response failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeDigestRsp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIGEST_RSP *pstDigestRsp);

/**
 * @brief SIP encodes dig-resp list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigRspLst SIP dig-resp list.
 *
 * @return ZOK SIP encodes dig-resp list successfully.
 * @return ZOK SIP encodes dig-resp list failed.
 *
 * @see Sip_EncodeDigRsp
 */
ZFUNC ZINT Sip_EncodeDigRspLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIG_RSP_LST *pstDigRspLst);

/**
 * @brief SIP encodes dig-resp.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDigRsp SIP dig-resp.
 *
 * @return ZOK SIP encodes dig-resp successfully.
 * @return ZOK SIP encodes dig-resp failed.
 *
 * @see Sip_EncodeDigRspLst
 */
ZFUNC ZINT Sip_EncodeDigRsp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIG_RSP *pstDigRsp);

/**
 * @brief SIP encodes other-response.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOther SIP other-response.
 *
 * @return ZOK SIP encodes other-response successfully.
 * @return ZOK SIP encodes other-response failed.
 *
 * @see Sip_EncodeDigRspLst
 */
ZFUNC ZINT Sip_EncodeOtherRsp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OTHER_RSP *pstOther);

/**
 * @brief SIP encodes option-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOptTagLst SIP option-tag list.
 *
 * @return ZOK SIP encodes option-tag list successfully.
 * @return ZOK SIP encodes option-tag list failed.
 *
 * @see Sip_EncodeOptTag
 */
ZFUNC ZINT Sip_EncodeOptTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OPT_TAG_LST *pstOptTagLst);

/**
 * @brief SIP encodes option-tag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOptTag SIP option-tag.
 *
 * @return ZOK SIP encodes option-tag list successfully.
 * @return ZOK SIP encodes option-tag list failed.
 *
 * @see Sip_EncodeOptTag
 */
ZFUNC ZINT Sip_EncodeOptTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OPT_TAG *pstOptTag);

/**
 * @brief SIP encodes option-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstComment Zos signed structure string.
 *
 * @return ZOK SIP encodes option-tag list successfully.
 * @return ZOK SIP encodes option-tag list failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeComment(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstComment);

/**
 * @brief SIP encodes retry-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP retry-param list.
 *
 * @return ZOK SIP encodes retry-param list successfully.
 * @return ZOK SIP encodes retry-param list failed.
 *
 * @see Sip_EncodeRetryParm
 */
ZFUNC ZINT Sip_EncodeRetryParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RETRY_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes retry-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP retry-param.
 *
 * @return ZOK SIP encodes retry-param successfully.
 * @return ZOK SIP encodes retry-param failed.
 *
 * @see Sip_EncodeRetryParmLst
 */
ZFUNC ZINT Sip_EncodeRetryParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RETRY_PARM *pstParm);

/**
 * @brief SIP encodes route-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP route-param list.
 *
 * @return ZOK SIP encodes route-param list successfully.
 * @return ZOK SIP encodes route-param list failed.
 *
 * @see Sip_EncodeRouteParm
 */
ZFUNC ZINT Sip_EncodeRouteParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ROUTE_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes route-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP route-param.
 *
 * @return ZOK SIP encodes route-param successfully.
 * @return ZOK SIP encodes route-param failed.
 *
 * @see Sip_EncodeRouteParmLst
 */
ZFUNC ZINT Sip_EncodeRouteParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ROUTE_PARM *pstParm);

/**
 * @brief SIP encodes server-val list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstServerValLst SIP server-val list.
 *
 * @return ZOK SIP encodes server-val list successfully.
 * @return ZOK SIP encodes server-val list failed.
 *
 * @see Sip_EncodeServerVal
 */
ZFUNC ZINT Sip_EncodeServerValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVER_VAL_LST *pstServerValLst);

/**
 * @brief SIP encodes server-val.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstServerVal SIP server-val.
 *
 * @return ZOK SIP encodes server-val successfully.
 * @return ZOK SIP encodes server-val failed.
 *
 * @see Sip_EncodeServerValLst
 */
ZFUNC ZINT Sip_EncodeServerVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVER_VAL *pstServerVal);

/**
 * @brief SIP encodes product.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstProduct SIP product.
 *
 * @return ZOK SIP encodes product successfully.
 * @return ZOK SIP encodes product failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeProduct(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRODUCT *pstProduct);

/**
 * @brief SIP encodes via-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP via-parm list.
 *
 * @return ZOK SIP encodes via-param list successfully.
 * @return ZOK SIP encodes via-param list failed.
 *
 * @see Sip_EncodeViaParm
 */
ZFUNC ZINT Sip_EncodeViaParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes via-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP via-param list.
 *
 * @return ZOK SIP encodes via-param list successfully.
 * @return ZOK SIP encodes via-param list failed.
 *
 * @see Sip_EncodeViaParmLst
 */
ZFUNC ZINT Sip_EncodeViaParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARM *pstParm);

/**
 * @brief SIP encodes sent-protocol.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstProtocol SIP sent-protocol.
 *
 * @return ZOK SIP encodes sent-protocol successfully.
 * @return ZOK SIP encodes sent-protocol failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeSentProtocol(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SENT_PROTOCOL *pstProtocol);

/**
 * @brief SIP encodes sent-by.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstBy SIP sent-by.
 *
 * @return ZOK SIP encodes sent-by successfully.
 * @return ZOK SIP encodes sent-by failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeSentBy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SENT_BY *pstBy);

/**
 * @brief SIP encodes via-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP via-params list.
 *
 * @return ZOK SIP encodes via-params list successfully.
 * @return ZOK SIP encodes via-params list failed.
 *
 * @see Sip_EncodeViaParms
 */
ZFUNC ZINT Sip_EncodeViaParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes via-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP via-params.
 *
 * @return ZOK SIP encodes via-params successfully.
 * @return ZOK SIP encodes via-params failed.
 *
 * @see Sip_EncodeViaParmsLst
 */
ZFUNC ZINT Sip_EncodeViaParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_PARMS *pstParms);

/**
 * @brief SIP encodes via-received.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRecv SIP via-received.
 *
 * @return ZOK SIP encodes via-received successfully.
 * @return ZOK SIP encodes via-received failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeViaRecv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_VIA_RECV *pstRecv);

/**
 * @brief SIP encodes warning-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP warning-value list.
 *
 * @return ZOK SIP encodes warning-value list successfully.
 * @return ZOK SIP encodes warning-value list failed.
 *
 * @see Sip_EncodeWarnVal, Sip_EncodeWarnAgent
 */
ZFUNC ZINT Sip_EncodeWarnValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_WARN_VAL_LST *pstParmLst);

/**
 * @brief SIP encodes warning-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP warning-value.
 *
 * @return ZOK SIP encodes warning-value successfully.
 * @return ZOK SIP encodes warning-value failed.
 *
 * @see Sip_EncodeWarnValLst, Sip_EncodeWarnAgent
 */
ZFUNC ZINT Sip_EncodeWarnVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_WARN_VAL *pstParm);

/**
 * @brief SIP encodes warn-agent.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAgent SIP warn-agent.
 *
 * @return ZOK SIP encodes warn-agent successfully.
 * @return ZOK SIP encodes warn-agent failed.
 *
 * @see Sip_EncodeWarnValLst, Sip_EncodeWarnVal
 */
ZFUNC ZINT Sip_EncodeWarnAgent(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_WARN_AGENT *pstAgent);

/**
 * @brief SIP encodes event-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP event-param list.
 *
 * @return ZOK SIP encodes event-param list successfully.
 * @return ZOK SIP encodes event-param list failed.
 *
 * @see Sip_EncodeEvntParm
 */
ZFUNC ZINT Sip_EncodeEvntParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes event-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP event-param.
 *
 * @return ZOK SIP encodes event-param successfully.
 * @return ZOK SIP encodes event-param failed.
 *
 * @see Sip_EncodeEvntParmLst
 */
ZFUNC ZINT Sip_EncodeEvntParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_PARM *pstParm);

/**
 * @brief SIP encodes event-type list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntTypeLst SIP event-type list.
 *
 * @return ZOK SIP encodes event-type list successfully.
 * @return ZOK SIP encodes event-type list failed.
 *
 * @see Sip_EncodeEvntType
 */
ZFUNC ZINT Sip_EncodeEvntTypeLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TYPE_LST *pstEvntTypeLst);

/**
 * @brief SIP encodes event-type.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntType SIP event-type.
 *
 * @return ZOK SIP encodes event-type successfully.
 * @return ZOK SIP encodes event-type failed.
 *
 * @see Sip_EncodeEvntTypeLst
 */
ZFUNC ZINT Sip_EncodeEvntType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TYPE *pstEvntType);

/**
 * @brief SIP encodes substate-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSubstaVal SIP substate-value.
 *
 * @return ZOK SIP encodes substate-value successfully.
 * @return ZOK SIP encodes substate-value failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeSubstaVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SUBSTA_VAL *pstSubstaVal);

/**
 * @brief SIP encodes event-template list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntTempLst SIP event-template list.
 *
 * @return ZOK SIP encodes event-template list successfully.
 * @return ZOK SIP encodes event-template list failed.
 *
 * @see Sip_EncodeEvntTemp
 */
ZFUNC ZINT Sip_EncodeEvntTempLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TEMP_LST *pstEvntTempLst);

/**
 * @brief SIP encodes event-template.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstEvntTemp SIP event-template.
 *
 * @return ZOK SIP encodes event-template successfully.
 * @return ZOK SIP encodes event-template failed.
 *
 * @see Sip_EncodeEvntTempLst
 */
ZFUNC ZINT Sip_EncodeEvntTemp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EVNT_TEMP *pstEvntTemp);

/**
 * @brief SIP encodes subexp-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP subexp-params list.
 *
 * @return ZOK SIP encodes subexp-params list successfully.
 * @return ZOK SIP encodes subexp-params list failed.
 *
 * @see Sip_EncodeSubexpParms
 */
ZFUNC ZINT Sip_EncodeSubexpParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SUBEXP_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes rsubexp-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP subexp-params.
 *
 * @return ZOK SIP encodes subexp-params successfully.
 * @return ZOK SIP encodes subexp-params failed.
 *
 * @see Sip_EncodeSubexpParmsLst
 */
ZFUNC ZINT Sip_EncodeSubexpParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SUBEXP_PARMS *pstParms);

/**
 * @brief SIP encodes replaces-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP replaces-param list.
 *
 * @return ZOK SIP encodes replaces-param list successfully.
 * @return ZOK SIP encodes replaces-param list failed.
 *
 * @see Sip_EncodeReplacesParm
 */
ZFUNC ZINT Sip_EncodeReplacesParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REPLACES_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes replaces-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP replaces-param.
 *
 * @return ZOK SIP encodes replaces-param successfully.
 * @return ZOK SIP encodes replaces-param failed.
 *
 * @see Sip_EncodeReplacesParmLst
 */
ZFUNC ZINT Sip_EncodeReplacesParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REPLACES_PARM *pstParm);

/**
 * @brief SIP encodes directive list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDirectLst SIP directive list.
 *
 * @return ZOK SIP encodes directive list successfully.
 * @return ZOK SIP encodes directive list failed.
 *
 * @see Sip_EncodeDirect
 */
ZFUNC ZINT Sip_EncodeDirectLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIRECT_LST *pstDirectLst);

/**
 * @brief SIP encodes directive.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDirect SIP directive.
 *
 * @return ZOK SIP encodes directive successfully.
 * @return ZOK SIP encodes directive failed.
 *
 * @see Sip_EncodeDirectLst
 */
ZFUNC ZINT Sip_EncodeDirect(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIRECT *pstDirect); 

/**
 * @brief SIP encodes ac-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAcValLst SIP ac-value list.
 *
 * @return ZOK SIP encodes ac-value list successfully.
 * @return ZOK SIP encodes ac-value list failed.
 *
 * @see Sip_EncodeAcVal
 */
ZFUNC ZINT Sip_EncodeAcValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_VAL_LST *pstAcValLst);

/**
 * @brief SIP encodes ac-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAcVal SIP ac-value.
 *
 * @return ZOK SIP encodes ac-value successfully.
 * @return ZOK SIP encodes ac-value failed.
 *
 * @see Sip_EncodeAcValLst
 */
ZFUNC ZINT Sip_EncodeAcVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_VAL *pstAcVal); 

/**
 * @brief SIP encodes ac-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP ac-params list.
 *
 * @return ZOK SIP encodes ac-params list successfully.
 * @return ZOK SIP encodes ac-params list failed.
 *
 * @see Sip_EncodeAcParms
 */
ZFUNC ZINT Sip_EncodeAcParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes ac-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP ac-params.
 *
 * @return ZOK SIP encodes ac-params successfully.
 * @return ZOK SIP encodes ac-params failed.
 *
 * @see Sip_EncodeAcParmsLst
 */
ZFUNC ZINT Sip_EncodeAcParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_AC_PARMS *pstParms); 

/**
 * @brief SIP encodes feature-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstFeature SIP feature-param.
 *
 * @return ZOK SIP encodes feature-param successfully.
 * @return ZOK SIP encodes feature-param failed.
 *
 * @see Sip_EncodeFeatureValLst, Sip_EncodeFeatureVal
 */
ZFUNC ZINT Sip_EncodeFeatureParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FEATURE_PARM *pstFeature);

/**
 * @brief SIP encodes feature-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstFeatureValLst SIP feature-value list.
 *
 * @return ZOK SIP encodes feature-value list successfully.
 * @return ZOK SIP encodes feature-value list failed.
 *
 * @see Sip_EncodeFeatureParm, Sip_EncodeFeatureVal
 */
ZFUNC ZINT Sip_EncodeFeatureValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FEATURE_VAL_LST *pstFeatureValLst);

/**
 * @brief SIP encodes feature-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstFeatureVal SIP feature-value.
 *
 * @return ZOK SIP encodes feature-value successfully.
 * @return ZOK SIP encodes feature-value failed.
 *
 * @see Sip_EncodeFeatureParm, Sip_EncodeFeatureValLst
 */
ZFUNC ZINT Sip_EncodeFeatureVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_FEATURE_VAL *pstFeatureVal);

/**
 * @brief SIP encodes route-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRcValLst SIP route-param list.
 *
 * @return ZOK SIP encodes route-param list successfully.
 * @return ZOK SIP encodes route-param list failed.
 *
 * @see Sip_EncodeRcVal
 */
ZFUNC ZINT Sip_EncodeRcValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_VAL_LST *pstRcValLst);

/**
 * @brief SIP encodes route-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRcVal SIP route-param list.
 *
 * @return ZOK SIP encodes route-param list successfully.
 * @return ZOK SIP encodes route-param list failed.
 *
 * @see Sip_EncodeRcValLst
 */
ZFUNC ZINT Sip_EncodeRcVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_VAL *pstRcVal);

/**
 * @brief SIP encodes rc-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP rc-params list.
 *
 * @return ZOK SIP encodes rc-params list successfully.
 * @return ZOK SIP encodes rc-params list failed.
 *
 * @see Sip_EncodeRcParms
 */
ZFUNC ZINT Sip_EncodeRcParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes rc-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP rc-params.
 *
 * @return ZOK SIP encodes rc-params successfully.
 * @return ZOK SIP encodes rc-params failed.
 *
 * @see Sip_EncodeRcParmsLst
 */
ZFUNC ZINT Sip_EncodeRcParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RC_PARMS *pstParms);

/**
 * @brief SIP encodes privacy-tag list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPrivacyTagLst SIP privacy-tag list.
 *
 * @return ZOK SIP encodes privacy-tag list successfully.
 * @return ZOK SIP encodes privacy-tag list failed.
 *
 * @see Sip_EncodePrivacyTag
 */
ZFUNC ZINT Sip_EncodePrivacyTagLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIVACY_TAG_LST *pstPrivacyTagLst);

/**
 * @brief SIP encodes privacy-tag.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPrivacyTag SIP privacy-tag.
 *
 * @return ZOK SIP encodes privacy-tag successfully.
 * @return ZOK SIP encodes privacy-tag failed.
 *
 * @see Sip_EncodePrivacyTagLst
 */
ZFUNC ZINT Sip_EncodePrivacyTag(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIVACY_TAG *pstPrivacyTag);

/**
 * @brief SIP encodes rpid-priv list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpidPrivLst SIP rpid-priv list.
 *
 * @return ZOK SIP encodes rpid-priv list successfully.
 * @return ZOK SIP encodes rpid-priv list failed.
 *
 * @see Sip_EncodeRpidPriv, Sip_EncodeRpidPrivTknLst, Sip_EncodeRpidPrivTkn
 */
ZFUNC ZINT Sip_EncodeRpidPrivLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV_LST *pstRpidPrivLst);

/**
 * @brief SIP encodes rpid-priv.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpidPriv SIP route-param.
 *
 * @return ZOK SIP encodes rpid-priv successfully.
 * @return ZOK SIP encodes rpid-priv failed.
 *
 * @see Sip_EncodeRpidPrivLst, Sip_EncodeRpidPrivTknLst, Sip_EncodeRpidPrivTkn
 */
ZFUNC ZINT Sip_EncodeRpidPriv(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV *pstRpidPriv);

/**
 * @brief SIP encodes rpid-privacy-token list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTknLst SIP rpid-privacy-token list.
 *
 * @return ZOK SIP encodes rpid-privacy-token list successfully.
 * @return ZOK SIP encodes rpid-privacy-token list failed.
 *
 * @see Sip_EncodeRpidPrivLst, Sip_EncodeRpidPrivTkn, Sip_EncodeRpidPrivTkn
 */
ZFUNC ZINT Sip_EncodeRpidPrivTknLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV_TKN_LST *pstTknLst);

/**
 * @brief SIP encodes rpid-privacy-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTkn SIP rpid-privacy-token.
 *
 * @return ZOK SIP encodes rpid-privacy-token successfully.
 * @return ZOK SIP encodes rpid-privacy-token failed.
 *
 * @see Sip_EncodeRpidPrivLst, Sip_EncodeRpidPriv, Sip_EncodeRpidPrivTknLst
 */
ZFUNC ZINT Sip_EncodeRpidPrivTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_PRIV_TKN *pstTkn);

/**
 * @brief SIP encodes rpi-privacy.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPrivacy SIP rpi-privacy.
 *
 * @return ZOK SIP encodes rpi-privacy successfully.
 * @return ZOK SIP encodes rpi-privacy failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeRpiPrivacy(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_PRIVACY *pstPrivacy);

/**
 * @brief SIP encodes rpi-priv-element list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstElemLst SIP rpi-priv-element list.
 *
 * @return ZOK SIP encodes rpi-priv-element list successfully.
 * @return ZOK SIP encodes rpi-priv-element list failed.
 *
 * @see Sip_EncodeRpiPrivElem
 */
ZFUNC ZINT Sip_EncodeRpiPrivElemLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_PRIV_ELEM_LST *pstElemLst);

/**
 * @brief SIP encodes rpi-priv-element.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstElem SIP rpi-priv-element.
 *
 * @return ZOK SIP encodes rpi-priv-element successfully.
 * @return ZOK SIP encodes rpi-priv-element failed.
 *
 * @see Sip_EncodeRpiPrivElemLst
 */
ZFUNC ZINT Sip_EncodeRpiPrivElem(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_PRIV_ELEM *pstElem);

/**
 * @brief SIP encodes rpid list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpidLst SIP rpid list.
 *
 * @return ZOK SIP encodes rpid list successfully.
 * @return ZOK SIP encodes rpid list failed.
 *
 * @see Sip_EncodeRpid
 */
ZFUNC ZINT Sip_EncodeRpidLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID_LST *pstRpidLst);

/**
 * @brief SIP encodes rpid.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRpid SIP rpid.
 *
 * @return ZOK SIP encodes rpid successfully.
 * @return ZOK SIP encodes rpid failed.
 *
 * @see Sip_EncodeRpidLst
 */
ZFUNC ZINT Sip_EncodeRpid(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPID *pstRpid);

/**
 * @brief SIP encodes rpi-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTknLst SIP rpi-token.
 *
 * @return ZOK SIP encodes rpi-token successfully.
 * @return ZOK SIP encodes rpi-token failed.
 *
 * @see Sip_EncodeRpiTkn
 */
ZFUNC ZINT Sip_EncodeRpiTknLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_TKN_LST *pstTknLst);

/**
 * @brief SIP encodes rpi-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTkn SIP rpi-token.
 *
 * @return ZOK SIP encodes rpi-token successfully.
 * @return ZOK SIP encodes rpi-token failed.
 *
 * @see Sip_EncodeRpiTknLst
 */
ZFUNC ZINT Sip_EncodeRpiTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_RPI_TKN *pstTkn);

/**
 * @brief SIP encodes other-rpi-token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstOther SIP other-rpi-token.
 *
 * @return ZOK SIP encodes other-rpi-token successfully.
 * @return ZOK SIP encodes other-rpi-token failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeOtherRpiTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_OTHER_RPI_TKN *pstOther);

/**
 * @brief SIP encodes join-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP join-param list.
 *
 * @return ZOK SIP encodes join-param list successfully.
 * @return ZOK SIP encodes join-param list failed.
 *
 * @see Sip_EncodeJoinParm
 */
ZFUNC ZINT Sip_EncodeJoinParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_JOIN_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes join-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP join-param.
 *
 * @return ZOK SIP encodes join-param successfully.
 * @return ZOK SIP encodes join-param failed.
 *
 * @see Sip_EncodeJoinParmLst
 */
ZFUNC ZINT Sip_EncodeJoinParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_JOIN_PARM *pstParm);
/**
 * @brief SIP encodes p-aso-uri-spec list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriSpecLst SIP p-aso-uri-spec list.
 *
 * @return ZOK SIP encodes p-aso-uri-spec list successfully.
 * @return ZOK SIP encodes p-aso-uri-spec list failed.
 *
 * @see Sip_EncodePAsoUriSpec
 */
ZFUNC ZINT Sip_EncodePAsoUriSpecLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_ASO_URI_SPEC_LST *pstUriSpecLst);

/**
 * @brief SIP encodes p-aso-uri-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriSpec SIP p-aso-uri-spec.
 *
 * @return ZOK SIP encodes p-aso-uri-spec successfully.
 * @return ZOK SIP encodes p-aso-uri-spec failed.
 *
 * @see Sip_EncodePAsoUriSpecLst
 */
ZFUNC ZINT Sip_EncodePAsoUriSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_ASO_URI_SPEC *pstUriSpec);

/**
 * @brief SIP encodes called-pty-id-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstIdSpec SIP called-pty-id-spec.
 *
 * @return ZOK SIP encodes called-pty-id-spec successfully.
 * @return ZOK SIP encodes called-pty-id-spec failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeCalledPtyIdSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CALLED_PTY_ID_SPEC *pstIdSpec);

/**
 * @brief SIP encodes p-vnet-id-spec list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVnetSpecLst SIP p-vnet-id-spec list.
 *
 * @return ZOK SIP encodes p-vnet-id-spec list successfully.
 * @return ZOK SIP encodes p-vnet-id-spec list header failed.
 *
 * @see Sip_EncodePVnetSpec
 */

ZFUNC ZINT Sip_EncodePVnetSpecLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_VNET_ID_SPEC_LST *pstVnetSpecLst);

/**
 * @brief SIP encodes p-vnet-id-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVnetSpec SIP p-vnet-id-spec.
 *
 * @return ZOK SIP encodes p-vnet-id-spec successfully.
 * @return ZOK SIP encodes p-vnet-id-spec header failed.
 *
 * @see Sip_EncodePVnetSpecLst
 */
ZFUNC ZINT Sip_EncodePVnetSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_VNET_ID_SPEC *pstVnetSpec);

/**
 * @brief SIP encodes access-net-spec.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstNetSpec SIP access-net-spec.
 *
 * @return ZOK SIP encodes access-net-spec successfully.
 * @return ZOK SIP encodes access-net-spec header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodePAnetSpec(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_ACCESS_NET_SPEC *pstNetSpec);

/**
 * @brief SIP encodes some type value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstType SIP some type value.
 *
 * @return ZOK SIP encodes some type value successfully.
 * @return ZOK SIP encodes some type value header failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodePAccessType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TYPE_VAL *pstType);

/**
 * @brief SIP encodes access-info list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfoLst SIP access-info list.
 *
 * @return ZOK SIP encodes access-info list successfully.
 * @return ZOK SIP encodes access-info list header failed.
 *
 * @see Sip_EncodePAnetInfo
 */
ZFUNC ZINT Sip_EncodePAnetInfoLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACCESS_INFO_LST *pstInfoLst);

/**
 * @brief SIP encodes access-info.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInfo SIP access-info.
 *
 * @return ZOK SIP encodes access-info successfully.
 * @return ZOK SIP encodes access-info failed.
 *
 * @see Sip_EncodePAnetInfoLst
 */
ZFUNC ZINT Sip_EncodePAnetInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ACCESS_INFO *pstInfo);

/**
 * @brief SIP encodes charge-addr-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP charge-addr-params.
 *
 * @return ZOK SIP encodes charge-addr-params list successfully.
 * @return ZOK SIP encodes charge-addr-params list failed.
 *
 * @see Sip_EncodePChargAddr
 */
ZFUNC ZINT Sip_EncodePChargAddrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_ADDR_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes charge-addr-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP charge-addr-params.
 *
 * @return ZOK SIP encodes charge-addr-params successfully.
 * @return ZOK SIP encodes charge-addr-params failed.
 *
 * @see Sip_EncodePChargAddrLst
 */
ZFUNC ZINT Sip_EncodePChargAddr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_ADDR_PARMS *pstParms);

/**
 * @brief SIP encodes charge-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP charge-params list.
 *
 * @return ZOK SIP encodes charge-params list successfully.
 * @return ZOK SIP encodes charge-params list failed.
 *
 * @see Sip_EncodePChargParms
 */
ZFUNC ZINT Sip_EncodePChargParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes charge-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP charge-params.
 *
 * @return ZOK SIP encodes charge-params successfully.
 * @return ZOK SIP encodes charge-params failed.
 *
 * @see Sip_EncodePChargParmsLst
 */
ZFUNC ZINT Sip_EncodePChargParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_CHARGE_PARMS *pstParms);

/**
 * @brief SIP encodes PAssertedID-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP PAssertedID-value list.
 *
 * @return ZOK SIP encodes PAssertedID-value list successfully.
 * @return ZOK SIP encodes PAssertedID-value list failed.
 *
 * @see Sip_EncodePAssertedId
 */
ZFUNC ZINT Sip_EncodePAssertedIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PASSERTEDID_VAL_LST *pstValLst);

/**
 * @brief SIP encodes PAssertedID-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP PAssertedID-value.
 *
 * @return ZOK SIP encodes PAssertedID-value successfully.
 * @return ZOK SIP encodes PAssertedID-value failed.
 *
 * @see Sip_EncodePAssertedIdLst
 */
ZFUNC ZINT Sip_EncodePAssertedId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PASSERTEDID_VAL *pstVal);

/**
 * @brief SIP encodes PPreferredID-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP PPreferredID-value list.
 *
 * @return ZOK SIP encodes PPreferredID-value list successfully.
 * @return ZOK SIP encodes PPreferredID-value list failed.
 *
 * @see Sip_EncodePPreferredId
 */
ZFUNC ZINT Sip_EncodePPreferredIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PPREFERREDID_VAL_LST *pstValLst);

/**
 * @brief SIP encodes PPreferredID-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP PPreferredID-value.
 *
 * @return ZOK SIP encodes PPreferredID-value successfully.
 * @return ZOK SIP encodes PPreferredID-value failed.
 *
 * @see Sip_EncodePPreferredIdLst, Sip_EncodeHdrProxyAuthor
 */
ZFUNC ZINT Sip_EncodePPreferredId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PPREFERREDID_VAL *pstVal);

/**
 * @brief SIP encodes P-Media-Authorization-Token list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTknLst SIP P-Media-Authorization-Token list.
 *
 * @return ZOK SIP encodes P-Media-Authorization-Token list successfully.
 * @return ZOK SIP encodes P-Media-Authorization-Token list failed.
 *
 * @see Sip_EncodePMediaAuthorTkn
 */
ZFUNC ZINT Sip_EncodePMediaAuthorTknLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_MEDIA_AUTHOR_TKN_LST *pstTknLst);

/**
 * @brief SIP encodes P-Media-Authorization-Token.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTkn SIP P-Media-Authorization-Token.
 *
 * @return ZOK SIP encodes P-Media-Authorization-Token successfully.
 * @return ZOK SIP encodes P-Media-Authorization-Token failed.
 *
 * @see Sip_EncodePMediaAuthorTknLst
 */
ZFUNC ZINT Sip_EncodePMediaAuthorTkn(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_P_MEDIA_AUTHOR_TKN *pstTkn);

/**
 * @brief SIP encodes priv-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP priv-value list.
 *
 * @return ZOK SIP encodes priv-value list successfully.
 * @return ZOK SIP encodes priv-value list failed.
 *
 * @see Sip_EncodePrivVal
 */
ZFUNC ZINT Sip_EncodePrivValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIV_VAL_LST *pstValLst);

/**
 * @brief SIP encodes priv-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP priv-value.
 *
 * @return ZOK SIP encodes priv-value successfully.
 * @return ZOK SIP encodes priv-value failed.
 *
 * @see Sip_EncodePrivValLst
 */
ZFUNC ZINT Sip_EncodePrivVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_PRIV_VAL *pstVal);

/**
 * @brief SIP encodes reason-value list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValLst SIP reason-value list.
 *
 * @return ZOK SIP encodes reason-value list.
 * @return ZOK SIP encodes reason-value list.
 *
 * @see Sip_EncodeReasonVal
 */
ZFUNC ZINT Sip_EncodeReasonValLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_VAL_LST *pstValLst);

/**
 * @brief SIP encodes reason-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstVal SIP message header.
 *
 * @return ZOK SIP encodes reason-value successfully.
 * @return ZOK SIP encodes reason-value failed.
 *
 * @see Sip_EncodeReasonValLst
 */
ZFUNC ZINT Sip_EncodeReasonVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_VAL *pstVal);

/**
 * @brief SIP encodes reason-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP reason-params list.
 *
 * @return ZOK SIP encodes reason-params list successfully.
 * @return ZOK SIP encodes reason-params list failed.
 *
 * @see Sip_EncodeReasonParms
 */
ZFUNC ZINT Sip_EncodeReasonParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes reason-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP reason-params.
 *
 * @return ZOK SIP encodes reason-params successfully.
 * @return ZOK SIP encodes reason-params failed.
 *
 * @see Sip_EncodeReasonParmsLst
 */
ZFUNC ZINT Sip_EncodeReasonParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REASON_PARMS *pstParms);

/**
 * @brief SIP encodes sec-mechanism list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMechLst SIP sec-mechanism list.
 *
 * @return ZOK SIP encodes sec-mechanism list successfully.
 * @return ZOK SIP encodes sec-mechanism list failed.
 *
 * @see Sip_EncodeSecMech
 */

ZFUNC ZINT Sip_EncodeSecMechLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SEC_MECH_LST *pstMechLst);

/**
 * @brief SIP encodes sec-mechanism.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMech SIP sec-mechanism.
 *
 * @return ZOK SIP encodes sec-mechanism successfully.
 * @return ZOK SIP encodes sec-mechanism failed.
 *
 * @see Sip_EncodeSecMechLst
 */
ZFUNC ZINT Sip_EncodeSecMech(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SEC_MECH *pstMech);

/**
 * @brief SIP encodes mech-parameters list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP mech-parameters list.
 *
 * @return ZOK SIP encodes mech-parameters list successfully.
 * @return ZOK SIP encodes mech-parameters list failed.
 *
 * @see Sip_EncodeMechParms
 */
ZFUNC ZINT Sip_EncodeMechParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MECH_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes mech-parameters.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP mech-parameters.
 *
 * @return ZOK SIP encodes mech-parameters successfully.
 * @return ZOK SIP encodes mech-parameters failed.
 *
 * @see Sip_EncodeMechParmsLst
 */
ZFUNC ZINT Sip_EncodeMechParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MECH_PARMS *pstParms);

/**
 * @brief SIP encodes Billing-Info-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP Billing-Info-param list.
 *
 * @return ZOK SIP encodes Billing-Info-param list successfully.
 * @return ZOK SIP encodes Billing-Info-param list failed.
 *
 * @see Sip_EncodeBillInfoParm
 */
ZFUNC ZINT Sip_EncodeBillInfoParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BILL_INFO_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes Billing-Info-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP Billing-Info-param.
 *
 * @return ZOK SIP encodes Billing-Info-param successfully.
 * @return ZOK SIP encodes Billing-Info-param failed.
 *
 * @see Sip_EncodeBillInfoParmLst
 */
ZFUNC ZINT Sip_EncodeBillInfoParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_BILL_INFO_PARM *pstParm);

/**
 * @brief SIP encodes Laes-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP Laes-param list.
 *
 * @return ZOK SIP encodes Laes-param list successfully.
 * @return ZOK SIP encodes Laes-param list failed.
 *
 * @see Sip_EncodeLaesParm
 */
ZFUNC ZINT Sip_EncodeLaesParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LAES_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes Laes-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP Laes-param.
 *
 * @return ZOK SIP encodes Laes-param successfully.
 * @return ZOK SIP encodes Laes-param failed.
 *
 * @see Sip_EncodeLaesParmLst
 */
ZFUNC ZINT Sip_EncodeLaesParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_LAES_PARM *pstParm);

/**
 * @brief SIP encodes redir-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP redir-params list.
 *
 * @return ZOK SIP encodes redir-params list successfully.
 * @return ZOK SIP encodes redir-params list failed.
 *
 * @see Sip_EncodeRedirParms
 */
ZFUNC ZINT Sip_EncodeRedirParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REDIR_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes redir-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP redir-params.
 *
 * @return ZOK SIP encodes redir-params successfully.
 * @return ZOK SIP encodes redir-params failed.
 *
 * @see Sip_EncodeRedirParmsLst
 */
ZFUNC ZINT Sip_EncodeRedirParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REDIR_PARMS *pstParms);

/**
 * @brief SIP encodes hi-entry list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiEntryLst SIP hi-entry list.
 *
 * @return ZOK SIP encodes hi-entry list successfully.
 * @return ZOK SIP encodes hi-entry list failed.
 *
 * @see Sip_EncodeHiEntry
 */
ZFUNC ZINT Sip_EncodeHiEntryLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_ENTRY_LST *pstHiEntryLst);

/**
 * @brief SIP encodes hi-entry.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiEntry SIP hi-entry.
 *
 * @return ZOK SIP encodes hi-entry successfully.
 * @return ZOK SIP encodes hi-entry failed.
 *
 * @see Sip_EncodeHiEntryLst
 */
ZFUNC ZINT Sip_EncodeHiEntry(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_ENTRY *pstHiEntry);

/**
 * @brief SIP encodes hi-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiParmLst SIP hi-param list.
 *
 * @return ZOK SIP encodes hi-param list successfully.
 * @return ZOK SIP encodes hi-param list failed.
 *
 * @see Sip_EncodeHiParm
 */
ZFUNC ZINT Sip_EncodeHiParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_PARM_LST *pstHiParmLst);

/**
 * @brief SIP encodes hi-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHiParm SIP hi-param.
 *
 * @return ZOK SIP encodes hi-param successfully.
 * @return ZOK SIP encodes hi-param failed.
 *
 * @see Sip_EncodeHiParmLst
 */
ZFUNC ZINT Sip_EncodeHiParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_HI_PARM *pstHiParm);

/**
 * @brief SIP encodes DiameterURI.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDiamUri SIP DiameterURI.
 *
 * @return ZOK SIP encodes DiameterURI successfully.
 * @return ZOK SIP encodes DiameterURI failed.
 *
 * @see
 */

ZFUNC ZINT Sip_EncodeDiamUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_DIAM_URI *pstDiamUri);

/**
 * @brief SIP encodes answer-mode-value.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstModeVal SIP answer-mode-value.
 *
 * @return ZOK SIP encodes answer-mode-value successfully.
 * @return ZOK SIP encodes answer-mode-value failed.
 *
 * @see
 */
ZFUNC ZINT Sip_EncodeAnswerModeVal(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ANSWER_MODE_VAL *pstModeVal);

/**
 * @brief SIP encodes answer-mode-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP answer-mode-param list.
 *
 * @return ZOK SIP encodes answer-mode-param list successfully.
 * @return ZOK SIP encodes answer-mode-param list failed.
 *
 * @see Sip_EncodeAnswerModeParm
 */
ZFUNC ZINT Sip_EncodeAnswerModeParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ANSWER_MODE_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes answer-mode-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP answer-mode-param.
 *
 * @return ZOK SIP encodes answer-mode-param successfully.
 * @return ZOK SIP encodes answer-mode-param failed.
 *
 * @see Sip_EncodeAnswerModeParmLst
 */
ZFUNC ZINT Sip_EncodeAnswerModeParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_ANSWER_MODE_PARM *pstParm);

/**
 * @brief SIP encodes Service-ID list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstIdLst SIP Service-ID list.
 *
 * @return ZOK SIP encodes Service-ID list successfully.
 * @return ZOK SIP encodes Service-ID list failed.
 *
 * @see Sip_EncodeSrvId
 */
ZFUNC ZINT Sip_EncodeSrvIdLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SRV_ID_LST *pstIdLst);

/**
 * @brief SIP encodes Service-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstId SIP Service-ID.
 *
 * @return ZOK SIP encodes Service-ID successfully.
 * @return ZOK SIP encodes Service-ID failed.
 *
 * @see Sip_EncodeSrvIdLst
 */
ZFUNC ZINT Sip_EncodeSrvId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SRV_ID *pstId);

/**
 * @brief SIP encodes early media parameter list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP early media parameter list.
 *
 * @return ZOK SIP encodes early media parameter list successfully.
 * @return ZOK SIP encodes early media parameter list failed.
 * @see Sip_EncodeEmParm
 */
ZFUNC ZINT Sip_EncodeEmParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EM_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes early media parameter list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP early media parameter.
 *
 * @return ZOK SIP encodes early media parameter successfully.
 * @return ZOK SIP encodes early media parameter failed.
 * @see Sip_EncodeEmParmLst
 */
ZFUNC ZINT Sip_EncodeEmParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_EM_PARM *pstParm);

/**
 * @brief SIP encodes instance.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstInstance SIP instance.
 *
 * @return ZOK SIP encodes instance successfully.
 * @return ZOK SIP encodes instance failed.
 */
ZFUNC ZINT Sip_EncodeCPInstance(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstInstance);

/**
 * @brief SIP encodes target-dialog parm list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst target-dialog parm list.
 *
 * @return ZOK SIP encodes target-dialog parm list successfully.
 * @return ZOK SIP encodes target-dialog parm list failed.
 */
ZFUNC ZINT Sip_EncodeTDParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TD_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes target-dialog parm.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm target-dialog parm.
 *
 * @return ZOK SIP encodes target-dialog parm successfully.
 * @return ZOK SIP encodes target-dialog parm failed.
 */
ZFUNC ZINT Sip_EncodeTDParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_TD_PARM *pstParm);

/**
 * @brief SIP encodes Uri-list-entry list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm Uri-list-entry list.
 *
 * @return ZOK SIP encodes Uri-list-entry list successfully.
 * @return ZOK SIP encodes Uri-list-entry list failed.
 */
ZFUNC ZINT Sip_EncodeUriEntryLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_ENTRY_LST *pstUriEntryLst);

/**
 * @brief SIP encodes Uri-list-entry.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm Uri-list-entry.
 *
 * @return ZOK SIP encodes Uri-list-entry successfully.
 * @return ZOK SIP encodes Uri-list-entry failed.
 */
ZFUNC ZINT Sip_EncodeUriEntry(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_URI_ENTRY *pstUriEntry);

/**
 * @brief SIP encodes refused-parm list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm refused-parm list.
 *
 * @return ZOK SIP encodes refused-parm list successfully.
 * @return ZOK SIP encodes refused-parm list failed.
 */
ZFUNC ZINT Sip_EncodeRefusedParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REFUSED_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes refused-parm.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm refused-parm.
 *
 * @return ZOK SIP encodes refused-parm successfully.
 * @return ZOK SIP encodes refused-parm failed.
 */
ZFUNC ZINT Sip_EncodeRefusedParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_REFUSED_PARM *pstParm);
/**
 * @brief SIP encodes ServInfo header parms.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm refused-parm.
 *
 * @return ZOK SIP encodes ServInfo header parms successfully.
 * @return ZOK SIP encodes ServInfo header parms failed.
 */
ZFUNC ZINT Sip_EncodeHdrServInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes ServInfo-param list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmLst SIP ServInfo-param list.
 *
 * @return ZOK SIP encodes ServInfo-param list successfully.
 * @return ZOK SIP encodes ServInfo-param list failed.
 *
 * @see Sip_EncodeServInfoParm, Sip_EncodeServInfoParmsLst,
 *      Sip_EncodeServInfoParms
 */
ZFUNC ZINT Sip_EncodeServInfoParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARM_LST *pstParmLst);

/**
 * @brief SIP encodes ServInfo-param.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm SIP ServInfo-param.
 *
 * @return ZOK SIP encodes ServInfo-param successfully.
 * @return ZOK SIP encodes ServInfo-param failed.
 *
 * @see Sip_EncodeServInfoParmLst, Sip_EncodeServInfoParmsLst,
 *      Sip_EncodeServInfoParms
 */
ZFUNC ZINT Sip_EncodeServInfoParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARM *pstParm);

/**
 * @brief SIP encodes ServInfo-params list.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParmsLst SIP ServInfo-params list.
 *
 * @return ZOK SIP encodes ServInfo-params list successfully.
 * @return ZOK SIP encodes ServInfo-params list failed.
 *
 * @see Sip_EncodeServInfoParmLst, Sip_EncodeServInfoParm,
 *      Sip_EncodeServInfoParms
 */
ZFUNC ZINT Sip_EncodeServInfoParmsLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARMS_LST *pstParmsLst);

/**
 * @brief SIP encodes ServInfo-params.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParms SIP ServInfo-params.
 *
 * @return ZOK SIP encodes ServInfo-params successfully.
 * @return ZOK SIP encodes ServInfo-params failed.
 *
 * @see Sip_EncodeServInfoParm, Sip_EncodeServInfoParmsLst,
 *      Sip_EncodeServInfoParmsLst
 */
ZFUNC ZINT Sip_EncodeServInfoParms(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_SERVINFO_PARMS *pstParms);

/**
 * @brief SIP encodes TerminalSeatTypeheader.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes TerminalSeatType header successfully.
 * @return ZOK SIP encodes TerminalSeatType header failed.
 *
 */
ZFUNC ZINT Sip_EncodeHdrTermSeatType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

/**
 * @brief SIP encodes STS-Call-ID.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr SIP message header.
 *
 * @return ZOK SIP encodes STS-Call-ID header successfully.
 * @return ZOK SIP encodes STS-Call-ID header failed.
 *
 */
ZINT Sip_EncodeHdrStsCallID(ST_ABNF_MSG *pstAbnfMsg, 
                ST_SIP_MSG_HDR *pstHdr);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_ABNF_ENCODE_H__ */

