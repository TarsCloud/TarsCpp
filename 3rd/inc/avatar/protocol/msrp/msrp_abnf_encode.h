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
  File name     : msrp_abnf_encode.h
  Module        : msrp abnf encode
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Marcos and structure definitions required by the msrp abnf encode.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_ABNF_ENCODE_H__
#define _MSRP_ABNF_ENCODE_H__
    
/** 
 * @file msrp_abnf_encode.h
 * @brief MSRP ABNF encode interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* msrp encode return error */
#define MSRP_ENCODE_RETFAIL(_msg, _info)  do { \
    ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
    return ZFAILED; \
} while (0)

/* msrp encode check result */
#define MSRP_ENCODE_CHKRET(_msg, _ret, _info)  do { \
    if (_ret != ZOK) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* msrp check present flag */
#define MSRP_ENCODE_CHKPRES(_msg, _pres, _info) do { \
    if (_pres != ZTRUE) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/*lint -save -e* */

/* msrp encode a character */
#define MSRP_ADD_CHR(_msg, _chr, _ret) \
    _ret = Abnf_AddPstChr(_msg, _chr)

/* msrp encode a token */
#define MSRP_ADD_TKN(_msg, _tkntype, _tknid, _ret) \
    _ret = Msrp_TknEncode(_msg, _tkntype, _tknid)

/* msrp encode a token */
#define MSRP_ADD_TKNX(_msg, _tkntab, _tknid, _ret) \
    _ret = Abnf_AddTkn(_msg, ZOS_GET_TABLE_INFOX(_tkntab), _tknid)

/* msrp encode a string */
#define MSRP_ADD_SSTR(_msg, _ststr, _ret) \
    _ret = Abnf_AddPstSStr(_msg, _ststr)

/* msrp encode a quoted string */
#define MSRP_ADD_QSTR(_msg, _ststr, _ret) \
    _ret = Abnf_AddPairSStr(_msg, '"', _ststr)

/* msrp encode a string with length */
#define MSRP_ADD_STRN(_msg, _str, _len, _ret) \
    _ret = Abnf_AddPstStrN(_msg, _str, _len)

/* msrp encode unsigned long decimal data */
#define MSRP_ADD_UIDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUiDigit(_msg, _data)

/* msrp encode unsigned short decimal data */
#define MSRP_ADD_USDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUsDigit(_msg, _data)

/* msrp encode unsigned chr decimal data */
#define MSRP_ADD_UCDIGIT(_msg, _data, _ret) \
    _ret = Abnf_AddUcDigit(_msg, _data)

/*lint -restore */

/* msrp encode SP */
#define MSRP_ADD_SP(_msg, _ret) MSRP_ADD_CHR(_msg, ' ', _ret)

/* msrp encode WSP = SP / HTAB*/
#define MSRP_ADD_WSP MSRP_ADD_SP

/* msrp encode LWS = [*WSP CRLF] 1*WSP */
#define MSRP_ADD_LWS MSRP_ADD_WSP

/* msrp encode SWS = [LWS] */
#define MSRP_ADD_SWS 

/* msrp encode HCOLON  =  *( SP / HTAB ) ":" SWS */
#define MSRP_ADD_HCOLON(_msg, _ret) MSRP_ADD_CHR(_msg, ':', _ret)

/* msrp encode STAR = SWS "*" SWS ; asterisk */
#define MSRP_ADD_STAR(_msg, _ret) MSRP_ADD_CHR(_msg, '*', _ret)

/* msrp encode SLASH = SWS "/" SWS ; slash */
#define MSRP_ADD_SLASH(_msg, _ret) MSRP_ADD_CHR(_msg, '/', _ret)

/* msrp encode EQUAL = SWS "=" SWS ; equal */
#define MSRP_ADD_EQUAL(_msg, _ret) MSRP_ADD_CHR(_msg, '=', _ret)

/* msrp encode LPAREN = SWS "(" SWS ; left parenthesis */
#define MSRP_ADD_LPAREN(_msg, _ret) MSRP_ADD_CHR(_msg, '(', _ret)

/* msrp encode RPAREN = SWS ")" SWS ; right parenthesis */
#define MSRP_ADD_RPAREN(_msg, _ret) MSRP_ADD_CHR(_msg, ')', _ret)

/* msrp encode RAQUOT = ">" SWS ; right angle quote */
#define MSRP_ADD_RAQUOT(_msg, _ret) MSRP_ADD_CHR(_msg, '>', _ret)

/* msrp encode LAQUOT = SWS "<" ; left angle quote */
#define MSRP_ADD_LAQUOT(_msg, _ret) MSRP_ADD_CHR(_msg, '<', _ret)

/* msrp encode COMMA = SWS "," SWS ; comma */
#define MSRP_ADD_COMMA(_msg, _ret) MSRP_ADD_CHR(_msg, ',', _ret)

/* msrp encode SEMI = SWS ";" SWS ; semicolon */
#define MSRP_ADD_SEMI(_msg, _ret) MSRP_ADD_CHR(_msg, ';', _ret)

/* msrp encode COLON = SWS ":" SWS ; colon */
#define MSRP_ADD_COLON(_msg, _ret) MSRP_ADD_CHR(_msg, ':', _ret)

/* msrp encode LDQUOT = SWS DQUOTE; open double quotation mark */
#define MSRP_ADD_LDQUOT(_msg, _ret) MSRP_ADD_CHR(_msg, '"', _ret)

/* msrp encode RDQUOT = DQUOTE SWS ; close double quotation mark */
#define MSRP_ADD_RDQUOT(_msg, _ret) MSRP_ADD_CHR(_msg, '"', _ret)

/* msrp encode CRLF */
#define MSRP_ADD_CRLF(_msg, _ret) MSRP_ADD_STRN(_msg, "\r\n", 2, _ret)

/* msrp encode an ip in the buffer */
#define MSRP_ADD_IPV4(_msg, _ip, _ret) _ret = Abnf_AddIpV4(_msg, _ip)

/* msrp encode an ipv6 in the buffer */
#define MSRP_ADD_IPV6(_msg, _ipv6, _ret) _ret = Abnf_AddIpV6(_msg, _ipv6)

/** @defgroup group_func_encode MSRP Message Encode Functions
 * @{
 */

/**
 * @brief Encode MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMsrpMsg MSRP message structure to be encoded.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeMsg
 */
ZFUNC ZINT Msrp_EncodeMsg(ST_ABNF_MSG *pstAbnfMsg, 
            ST_MSRP_MSG *pstMsrpMsg);

/**
 * @brief Encode Request-start in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstReq MSRP Request-start structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeReqStart
 */
ZFUNC ZINT Msrp_EncodeReqStart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_REQ_START  *pstReq);

/**
 * @brief Encode Response-start in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstRsp MSRP Response-start structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeRspStart
 */
ZFUNC ZINT Msrp_EncodeRspStart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_RSP_START *pstRsp);

/**
 * @brief Encode From-Path header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrFromPath
 */
ZFUNC ZINT Msrp_EncodeHdrFromPath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode To-Path header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrToPath
 */
ZFUNC ZINT Msrp_EncodeHdrToPath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode all headers in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstMsrpMsg MSRP message structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrLst
 */
ZFUNC ZINT Msrp_EncodeHdrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MSG *pstMsrpMsg);

/**
 * @brief Encode Message-ID header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrMsgId
 */
ZFUNC ZINT Msrp_EncodeHdrMsgId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Success-Report header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrSuccRpt
 */
ZFUNC ZINT Msrp_EncodeHdrSuccRpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Failure-Report header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrFailRpt
 */
ZFUNC ZINT Msrp_EncodeHdrFailRpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Byte-Range header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrByteRange
 */
ZFUNC ZINT Msrp_EncodeHdrByteRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Status header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrStatus
 */
ZFUNC ZINT Msrp_EncodeHdrStatus(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Content-Type header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrContType
 */
ZFUNC ZINT Msrp_EncodeHdrContType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Content-ID header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrContId
 */
ZFUNC ZINT Msrp_EncodeHdrContId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Content-Description header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrContDesc
 */
ZFUNC ZINT Msrp_EncodeHdrContDesc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode Content-Disposition header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrContDisp
 */
ZFUNC ZINT Msrp_EncodeHdrContDisp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode extension header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHdrExtHdr
 */
ZFUNC ZINT Msrp_EncodeHdrExtHdr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Encode method in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstMethod MSRP method structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeMethod
 */
ZFUNC ZINT Msrp_EncodeMethod(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_METHOD *pstMethod);

/**
 * @brief Encode MSRP URI list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstMsrpUriLst MSRP URI list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeMsrpUriLst
 */
ZFUNC ZINT Msrp_EncodeMsrpUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MSRP_URI_LST *pstMsrpUriLst);

/**
 * @brief Encode MSRP URI in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstMsrpUri MSRP URI list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeMsrpUri
 */
ZFUNC ZINT Msrp_EncodeMsrpUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MSRP_URI *pstMsrpUri);

/**
 * @brief Encode authority in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstAuthority MSRP authority structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeAuthority
 */
ZFUNC ZINT Msrp_EncodeAuthority(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_AUTHORITY *pstAuthority);

/**
 * @brief Encode user info in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstUserInfo MSRP user info.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeUserInfo
 */
ZFUNC ZINT Msrp_EncodeUserInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstUserInfo);

/**
 * @brief Encode host in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstHost MSRP host structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeHost
 */
ZFUNC ZINT Msrp_EncodeHost(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HOST *pstHost);

/**
 * @brief Encode IPv6 in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pucIpv6Addr IPv6 value.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeIpv6
 */
ZFUNC ZINT Msrp_EncodeIpv6(ST_ABNF_MSG *pstAbnfMsg, 
                ZUCHAR *pucIpv6Addr);

/**
 * @brief Encode IPvFuture in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstIpvFuture MSRP IPvFuture structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeIpvFuture
 */
ZFUNC ZINT Msrp_EncodeIpvFuture(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_IPV_FUTURE *pstIpvFuture);

/**
 * @brief Encode transport in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstTpt MSRP transport structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeTpt
 */
ZFUNC ZINT Msrp_EncodeTpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_TPT *pstTpt);

/**
 * @brief Encode URI parameter list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstUriParmLst MSRP URI parameter list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeUriParmLst
 */
ZFUNC ZINT Msrp_EncodeUriParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_URI_PARM_LST *pstUriParmLst);

/**
 * @brief Encode URI parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstUriParm MSRP URI parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeUriParm
 */
ZFUNC ZINT Msrp_EncodeUriParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_URI_PARM *pstUriParm);

/**
 * @brief Encode media type in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstMediaType MSRP media type structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeMediaType
 */
ZFUNC ZINT Msrp_EncodeMediaType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MEDIA_TYPE *pstMediaType);

/**
 * @brief Encode generic parameter list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstGenParmLst MSRP generic parameter list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeGenParmLst
 */
ZFUNC ZINT Msrp_EncodeGenParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_GEN_PARM_LST *pstGenParmLst);

/**
 * @brief Encode generic parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstGenParm MSRP generic parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeGenParm
 */
ZFUNC ZINT Msrp_EncodeGenParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_GEN_PARM *pstGenParm);

/**
 * @brief Encode parameter value in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstPval MSRP parameter value structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodePval
 */
ZFUNC ZINT Msrp_EncodePval(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_PVAL *pstPval);

/**
 * @brief Encode quoted-string value in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstQStr MSRP quoted-string value.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeQStr
 */
ZFUNC ZINT Msrp_EncodeQStr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQStr);

/**
 * @brief Encode word list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstWordLst MSRP word list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeWordLst
 */
ZFUNC ZINT Msrp_EncodeWordLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_WORD_LST *pstWordLst);

/**
 * @brief Encode word in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstWord MSRP word structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeWord
 */
ZFUNC ZINT Msrp_EncodeWord(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_WORD *pstWord);

/**
 * @brief Encode sub-domain list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstSDomainLst MSRP sub-domain list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeSDomainLst
 */
ZFUNC ZINT Msrp_EncodeSDomainLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_SDOMAIN_LST *pstSDomainLst);

/**
 * @brief Encode sub-domain in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstSDomain MSRP sub-domain structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeSDomain
 */
ZFUNC ZINT Msrp_EncodeSDomain(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_SDOMAIN *pstSDomain);

/**
 * @brief Encode domain-literal in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstDLiteral MSRP domain-literal string.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeDLiteral
 */
ZFUNC ZINT Msrp_EncodeDLiteral(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstDLiteral);

/**
 * @brief Encode disposition parameter list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstDispParmLst MSRP disposition parameter list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeDispParmLst
 */
ZFUNC ZINT Msrp_EncodeDispParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_DISP_PARM_LST *pstDispParmLst);

/**
 * @brief Encode disposition parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstDispParm MSRP disposition parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeDispParm
 */
ZFUNC ZINT Msrp_EncodeDispParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_DISP_PARM *pstDispParm);

/**
 * @brief Encode quoted datetime in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstQDTime MSRP quoted datetime string.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeQDTime
 */
ZFUNC ZINT Msrp_EncodeQDTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQDTime);

/**
 * @brief Encode file name in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstFileName MSRP file name structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeFileName
 */
ZFUNC ZINT Msrp_EncodeFileName(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_FILE_NAME *pstFileName);

/**
 * @brief Encode parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstParm MSRP parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeParm
 */
ZFUNC ZINT Msrp_EncodeParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_PARM *pstParm);

/**
 * @brief Encode paramete value in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstValue MSRP paramete value structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeDispValue
 */
ZFUNC ZINT Msrp_EncodeDispValue(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_VALUE *pstValue);

/**
 * @brief Encode data in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstData MSRP data string.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeData
 */
ZFUNC ZINT Msrp_EncodeData(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstData);

/**
 * @brief Encode end line in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in] pstTransId MSRP transaction ID string.
 * @param [in] pucContFlag MSRP continuation-flag, @ref EN_MSRP_CONT_FLAG_TYPE.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_DecodeEndLine
 */
ZFUNC ZINT Msrp_EncodeEndLine(ST_ABNF_MSG *pstAbnfMsg, 
                    ST_ZOS_SSTR *pstTransId, ZUCHAR *pucContFlag);

/** @} */

#ifdef __cplusplus
}
#endif
                                
#endif /* _MSRP_ABNF_DECODE_H__ */

