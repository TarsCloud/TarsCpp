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
  File name     : msrp_abnf_decode.h
  Module        : msrp abnf decode
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Marcos and structure definitions required by the msrp abnf decode.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_ABNF_DECODE_H__
#define _MSRP_ABNF_DECODE_H__

/** 
 * @file msrp_abnf_decode.h
 * @brief MSRP ABNF decode interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* msrp decode result fail */
#define MSRP_DECODE_RETFAIL(_msg, _info)   do { \
    ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
    return ZFAILED; \
} while (0)    

/* msrp decode check result */
#define MSRP_DECODE_CHKRET(_msg, _ret, _info) do { \
    if (_ret != ZOK) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* msrp decode check memory */
#define MSRP_DECODE_CHKMEM(_msg, _mem, _info) do { \
    if (_mem == ZNULL) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/* msrp decode check token result */
#define MSRP_DECODE_CHKTKNID(_msg, _tknid, _info) do { \
    if (_tknid == ABNF_TKNID_UNKNOWN) \
    { \
        ABNF_LOG_USER_ERR(_msg, _info, __LINE__); \
        return ZFAILED; \
    } \
} while (0)

/*lint -save -e* */

/* msrp set use LWSP as valid character in the string */
#define MSRP_SET_USELWSP(_msg) ABNF_SET_USELWSP(_msg)

/* msrp clear use LWSP as valid character in the string */
#define MSRP_CLR_USELWSP(_msg) ABNF_CLR_USELWSP(_msg)

/* msrp set use WSP as valid character in the string */
#define MSRP_SET_USEWSP(_msg) ABNF_SET_USEWSP(_msg)

/* msrp clear use WSP as valid character in the string */
#define MSRP_CLR_USEWSP(_msg) ABNF_CLR_USEWSP(_msg)

/* msrp get one character */
#define MSRP_GET_CHR(_msg, _pchr, _ret) \
    _ret = Abnf_GetChr(_msg, _pchr)

/* get a string(length can be zero) */
#define MSRP_GET_SSTR_CHRSET(_msg, _class, _pststr, _ret) \
    _ret = Abnf_GetSStrChrset(_msg, Msrp_ChrsetGetId(), _class, _pststr)

/* get a string(length can be zero) */
#define MSRP_GET_SSTR_ESCAPE(_msg, _class, _echr, _eclass, _pststr, _ret) \
    _ret = Abnf_GetSStrEscape(_msg, Msrp_ChrsetGetId(), _class, _echr, _eclass, \
                              _pststr)

/* get a string(length can be zero) */
#define MSRP_GET_SSTR_SEPAS(_msg, _chr, _pststr, _ret) \
    _ret = Abnf_GetSStrSepas(_msg, _chr, 0, 0, 0, _pststr)

/* get a string(length can't be zero) */
#define MSRP_GET_NSSTR_CHRSET(_msg, _class, _pststr, _ret) \
    _ret = Abnf_GetNSStrChrset(_msg, Msrp_ChrsetGetId(), _class, \
                               1, ABNF_MAX_LEN_NA, _pststr)

/* get a string(length can't be zero) */
#define MSRP_GET_NSSTR_ESCAPE(_msg, _class, _echr, _eclass, _pststr, _ret) \
    _ret = Abnf_GetNSStrEscape(_msg, Msrp_ChrsetGetId(), _class, _echr, _eclass, \
                               1, ABNF_MAX_LEN_NA, _pststr)

/* get a token string */
#define MSRP_GET_TKNSSTR(_msg, _pststr, _ret) \
    _ret = Abnf_GetSStrChrset(_msg, Msrp_ChrsetGetId(), MSRP_TKN, _pststr)

/* msrp get the scanned string */
#define MSRP_GET_SCANNED_STR(_msg, _pstr) ABNF_GET_SCANNED_STR(_msg, _pstr) 

/* msrp get the scanning(the left message to be scanned) string */
#define MSRP_GET_SCANNING_STR(_msg, _pstr) ABNF_GET_SCANNING_STR(_msg, _pstr) 

/* get unsigned long decimal data */
#define MSRP_GET_UIDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUiDigit(_msg, _pdata)

/* get unsigned short decimal data */
#define MSRP_GET_USDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUsDigit(_msg, _pdata)

/* get unsigned chr decimal data */
#define MSRP_GET_UCDIGIT(_msg, _pdata, _ret) \
    _ret = Abnf_GetUcDigit(_msg, _pdata)

/* msrp expect to match one character */
#define MSRP_EXPECT_CHR(_msg, _chr, _case, _ret) \
    _ret = Abnf_ExpectChr(_msg, _chr, _case)

/* msrp try to expect to match one character */
#define MSRP_TRY_EXPECT_CHR(_msg, _chr, _case, _ret) \
    _ret = Abnf_TryExpectChr(_msg, _chr, _case)

/* msrp decode expect eol(end of line) */
#define MSRP_EXPECT_EOL(_msg, _ret)  _ret = Abnf_ExpectEol(_msg)

/* msrp decode try to expect eol(end of line) */
#define MSRP_TRY_EXPECT_EOL(_msg, _ret)  _ret = Abnf_TryExpectEol(_msg)

/* msrp expect LWS */
#define MSRP_EXPECT_LWS(_msg, _ret) _ret = Abnf_ExpectLWS(_msg)

/* msrp try expect LWS */
#define MSRP_TRY_EXPECT_LWS(_msg, _ret) _ret = Abnf_TryExpectLWS(_msg)

/* msrp decode WSP = SP / HTAB*/
#define MSRP_IGNORE_WSP(_msg, _ret) _ret = Abnf_IgnWS(_msg)

/* msrp decode LWS = [*WSP CRLF] 1*WSP */
#define MSRP_IGNORE_LWS(_msg, _ret) _ret = Abnf_IgnLWS(_msg)

/*lint -restore */

/* msrp decode SWS = [LWS] */
#define MSRP_EXPECT_SWS(_msg, _ret) MSRP_IGNORE_LWS(_msg, _ret)

/* msrp decode SP */
#define MSRP_EXPECT_SP(_msg, _ret) MSRP_EXPECT_CHR(_msg, ' ', ZTRUE, _ret)

/* msrp decode an ip in the buffer */
#define MSRP_GET_IPV4(_msg, _ip, _ret) _ret = Abnf_GetIpV4(_msg, _ip)

/* msrp decode an ipv6 in the buffer */
#define MSRP_GET_IPV6(_msg, _ipv6, _ret) _ret = Abnf_GetIpV6(_msg, _ipv6)

/** @defgroup group_func_decode MSRP Message Decode Functions
 * @{
 */

/**
 * @brief Decode MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [in,out] pstMsrpMsg MSRP message structure which will be filled with
                              data decoded.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeMsg
 */
ZFUNC ZINT Msrp_DecodeMsg(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MSG *pstMsrpMsg);

/**
 * @brief Decode Request-start in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstReq MSRP Request-start structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeReqStart
 */
ZFUNC ZINT Msrp_DecodeReqStart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_REQ_START  *pstReq);

/**
 * @brief Decode Response-start in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstRsp MSRP Response-start structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeRspStart
 */
ZFUNC ZINT Msrp_DecodeRspStart(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_RSP_START *pstRsp);

/**
 * @brief Decode From-Path header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrFromPath
 */
ZFUNC ZINT Msrp_DecodeHdrFromPath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode To-Path header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrToPath
 */
ZFUNC ZINT Msrp_DecodeHdrToPath(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode all headers in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMsrpMsg MSRP message structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrLst
 */
ZFUNC ZINT Msrp_DecodeHdrLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MSG *pstMsrpMsg);

/**
 * @brief Decode Message-ID header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrMsgId
 */
ZFUNC ZINT Msrp_DecodeHdrMsgId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Success-Report header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrSuccRpt
 */
ZFUNC ZINT Msrp_DecodeHdrSuccRpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Failure-Report header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrFailRpt
 */
ZFUNC ZINT Msrp_DecodeHdrFailRpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Byte-Range header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrByteRange
 */
ZFUNC ZINT Msrp_DecodeHdrByteRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Status header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrStatus
 */
ZFUNC ZINT Msrp_DecodeHdrStatus(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Content-Type header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrContType
 */
ZFUNC ZINT Msrp_DecodeHdrContType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Content-ID header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrContId
 */
ZFUNC ZINT Msrp_DecodeHdrContId(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Content-Description header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrContDesc
 */
ZFUNC ZINT Msrp_DecodeHdrContDesc(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode Content-Disposition header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrContDisp
 */
ZFUNC ZINT Msrp_DecodeHdrContDisp(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode extension header in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHdr MSRP header structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHdrExtHdr
 */
ZFUNC ZINT Msrp_DecodeHdrExtHdr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR *pstHdr);

/**
 * @brief Decode method in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMethod MSRP method structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeMethod
 */
ZFUNC ZINT Msrp_DecodeMethod(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_METHOD *pstMethod);

/**
 * @brief Decode MSRP URI list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMsrpUriLst MSRP URI list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeMsrpUriLst
 */
ZFUNC ZINT Msrp_DecodeMsrpUriLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MSRP_URI_LST *pstMsrpUriLst);

/**
 * @brief Decode MSRP URI in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMsrpUri MSRP URI list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeMsrpUri
 */
ZFUNC ZINT Msrp_DecodeMsrpUri(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MSRP_URI *pstMsrpUri);

/**
 * @brief Decode authority in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstAuthority MSRP authority structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeAuthority
 */
ZFUNC ZINT Msrp_DecodeAuthority(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_AUTHORITY *pstAuthority);

/**
 * @brief Decode user info in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUserInfo MSRP user info.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeUserInfo
 */
ZFUNC ZINT Msrp_DecodeUserInfo(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstUserInfo);

/**
 * @brief Decode host in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstHost MSRP host structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeHost
 */
ZFUNC ZINT Msrp_DecodeHost(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HOST *pstHost);

/**
 * @brief Decode IPv6 in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pucIpv6Addr IPv6 value.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeIpv6
 */
ZFUNC ZINT Msrp_DecodeIpv6(ST_ABNF_MSG *pstAbnfMsg, 
                ZUCHAR *pucIpv6Addr);

/**
 * @brief Decode IPvFuture in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstIpvFuture MSRP IPvFuture structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeIpvFuture
 */
ZFUNC ZINT Msrp_DecodeIpvFuture(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_IPV_FUTURE *pstIpvFuture);

/**
 * @brief Decode transport in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTpt MSRP transport structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeTpt
 */
ZFUNC ZINT Msrp_DecodeTpt(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_TPT *pstTpt);

/**
 * @brief Decode URI parameter list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriParmLst MSRP URI parameter list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeUriParmLst
 */
ZFUNC ZINT Msrp_DecodeUriParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_URI_PARM_LST *pstUriParmLst);

/**
 * @brief Decode URI parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstUriParm MSRP URI parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeUriParm
 */
ZFUNC ZINT Msrp_DecodeUriParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_URI_PARM *pstUriParm);

/**
 * @brief Decode media type in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstMediaType MSRP media type structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeMediaType
 */
ZFUNC ZINT Msrp_DecodeMediaType(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_MEDIA_TYPE *pstMediaType);

/**
 * @brief Decode generic parameter list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstGenParmLst MSRP generic parameter list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeGenParmLst
 */
ZFUNC ZINT Msrp_DecodeGenParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_GEN_PARM_LST *pstGenParmLst);

/**
 * @brief Decode generic parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstGenParm MSRP generic parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeGenParm
 */
ZFUNC ZINT Msrp_DecodeGenParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_GEN_PARM *pstGenParm);

/**
 * @brief Decode parameter value in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstPval MSRP parameter value structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodePval
 */
ZFUNC ZINT Msrp_DecodePval(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_PVAL *pstPval);

/**
 * @brief Decode quoted-string value in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQStr MSRP quoted-string value.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeQStr
 */
ZFUNC ZINT Msrp_DecodeQStr(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQStr);

/**
 * @brief Decode word list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstWordLst MSRP word list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeWordLst
 */
ZFUNC ZINT Msrp_DecodeWordLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_WORD_LST *pstWordLst);

/**
 * @brief Decode word in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstWord MSRP word structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeWord
 */
ZFUNC ZINT Msrp_DecodeWord(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_WORD *pstWord);

/**
 * @brief Decode sub-domain list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSDomainLst MSRP sub-domain list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeSDomainLst
 */
ZFUNC ZINT Msrp_DecodeSDomainLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_SDOMAIN_LST *pstSDomainLst);

/**
 * @brief Decode sub-domain in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstSDomain MSRP sub-domain structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeSDomain
 */
ZFUNC ZINT Msrp_DecodeSDomain(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_SDOMAIN *pstSDomain);

/**
 * @brief Decode domain-literal in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDLiteral MSRP domain-literal string.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeDLiteral
 */
ZFUNC ZINT Msrp_DecodeDLiteral(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstDLiteral);

/**
 * @brief Decode byte range in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstByteRange MSRP byte range structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see
 */
ZFUNC ZINT Msrp_DecodeByteRange(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_HDR_BYTE_RANGE *pstByteRange);

/**
 * @brief Decode disposition parameter list in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDispParmLst MSRP disposition parameter list.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeDispParmLst
 */
ZFUNC ZINT Msrp_DecodeDispParmLst(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_DISP_PARM_LST *pstDispParmLst);

/**
 * @brief Decode disposition parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstDispParm MSRP disposition parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeDispParm
 */
ZFUNC ZINT Msrp_DecodeDispParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_DISP_PARM *pstDispParm);

/**
 * @brief Decode quoted datetime in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstQDTime MSRP quoted datetime string.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeQDTime
 */
ZFUNC ZINT Msrp_DecodeQDTime(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstQDTime);

/**
 * @brief Decode file name in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstFileName MSRP file name structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeFileName
 */
ZFUNC ZINT Msrp_DecodeFileName(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_FILE_NAME *pstFileName);

/**
 * @brief Decode parameter in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstParm MSRP parameter structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeParm
 */
ZFUNC ZINT Msrp_DecodeParm(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_PARM *pstParm);

/**
 * @brief Decode paramete value in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstValue MSRP paramete value structure.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeDispValue
 */
ZFUNC ZINT Msrp_DecodeDispValue(ST_ABNF_MSG *pstAbnfMsg, 
                ST_MSRP_VALUE *pstValue);
/**
 * @brief Decode data in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTransId MSRP transaction ID string.
 * @param [out] pstData MSRP data string.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeData
 */
ZFUNC ZINT Msrp_DecodeData(ST_ABNF_MSG *pstAbnfMsg,  
                ST_ZOS_SSTR *pstTransId, ST_ZOS_SSTR *pstData);

/**
 * @brief Decode end line in MSRP message.
 *
 * @param [in] pstAbnfMsg ABNF message structure.
 * @param [out] pstTransId MSRP transaction ID string.
 * @param [out] pucContFlag MSRP continuation-flag, @ref EN_MSRP_CONT_FLAG_TYPE.
 *
 * @retval ZOK Decode OK.
 * @retval ZFAILED Decode failed.
 *
 * @see Msrp_EncodeEndLine
 */
ZFUNC ZINT Msrp_DecodeEndLine(ST_ABNF_MSG *pstAbnfMsg, 
                ST_ZOS_SSTR *pstTransId, ZUCHAR *pucContFlag);

/** @} */

#ifdef __cplusplus
}
#endif
                
#endif /* _MSRP_ABNF_DECODE_H__ */

