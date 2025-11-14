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
  File name     : sip_util.h
  Module        : sip protocol utility
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_UTIL_H__
#define _SIP_UTIL_H__

/**
 * @file
 * @brief SIP utilities interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
/* sip log module */
#define SIP_LOG_MOD_TPT     0        /**< @brief log module of transport. */
#define SIP_LOG_MOD_CODE    1        /**< @brief log module of decode/encode. */
#define SIP_LOG_MOD_TRANS   2        /**< @brief log module of transaction. */
#define SIP_LOG_MOD_DLG     3        /**< @brief log module of dialog. */
#define SIP_LOG_MOD_CORE    4        /**< @brief log module of core. */
#define SIP_LOG_MOD_MOD     5        /**< @brief log module of other module. */
#define SIP_LOG_MOD_ALL     6        /**< @brief log module of other module. */
/** @} */

/** @{ */
/* sip log level */
#define SIP_LOG_LEVEL_NULL  ZLOG_LEVEL_NULL /**< @brief log level of null. */
#define SIP_LOG_LEVEL_ERR   ZLOG_LEVEL_ERROR /**< @brief log level of error. */
#define SIP_LOG_LEVEL_WARN  ZLOG_LEVEL_WARN /**< @brief log level of warning. */
#define SIP_LOG_LEVEL_INFO  ZLOG_LEVEL_INFO /**< @brief log level of information. */
#define SIP_LOG_LEVEL_DBG   ZLOG_LEVEL_DBG /**< @brief log level of debug. */
#define SIP_LOG_LEVEL_ALL   ZLOG_LEVEL_ALL /**< @brief log level of all. */
/** @} */

/** @{ */
/* sip response codes */
#define SIP_RSP_100_TRYING                100 /**< @brief Response code of "100 trying". */
#define SIP_RSP_180_RINGING               180 /**< @brief Response code of "180 ringing". */
#define SIP_RSP_181_FORWARDED             181 /**< @brief Response code of "181 forwarded". */
#define SIP_RSP_182_QUEUED                182 /**< @brief Response code of "182 queued". */
#define SIP_RSP_183_SESS_PROGRESS         183 /**< @brief Response code of "183 session progress". */

#define SIP_RSP_200_OK                    200 /**< @brief Response code of "200 OK". */
#define SIP_RSP_202_ACPTED                202 /**< @brief Response code of "202 accepted". */
#define SIP_RSP_300_MULT_CHOICES          300 /**< @brief Response code of "300 mult choices". */
#define SIP_RSP_301_MOVED_PERM            301 /**< @brief Response code of "301 moved permanent". */
#define SIP_RSP_302_MOVED_TEMP            302 /**< @brief Response code of "302 moved temporality". */
#define SIP_RSP_305_USE_PROXY             305 /**< @brief Response code of "305 use proxy". */
#define SIP_RSP_380_ALT_SERVICE           380 /**< @brief Response code of "380 alt service". */

#define SIP_RSP_400_BAD_REQ               400 /**< @brief Response code of "400 bad request". */
#define SIP_RSP_401_UNAUTHOR              401 /**< @brief Response code of "401 unauthorizationorization". */
#define SIP_RSP_402_PAYMENT_REQUIRED      402 /**< @brief Response code of "402 payment requestuired". */
#define SIP_RSP_403_FORBIDDEN             403 /**< @brief Response code of "403 forbidden". */
#define SIP_RSP_404_NOT_FOUND             404 /**< @brief Response code of "404 not found". */
#define SIP_RSP_405_METHOD_NOT_ALLOWED    405 /**< @brief Response code of "405 method not allowed". */
#define SIP_RSP_406_NOT_ACPTED            406 /**< @brief Response code of "406 not accepted". */
#define SIP_RSP_407_PROXY_AUTH_REQD       407 /**< @brief Response code of "407 proxy authentication required". */
#define SIP_RSP_408_REQ_TIMEOUT           408 /**< @brief Response code of "408 request timeout". */
#define SIP_RSP_409_CONFLICT              409 /**< @brief Response code of "409 conflict". */
#define SIP_RSP_410_GONE                  410 /**< @brief Response code of "410 gone". */
#define SIP_RSP_411_LEN_REQ               411 /**< @brief Response code of "411 length request". */
#define SIP_RSP_412_COND_REQ_FAILED       412 /**< @brief Response code of "412 condition request failed". */
#define SIP_RSP_413_REQ_ENTITY_TOO_LARGE  413 /**< @brief Response code of "413 request entity too large". */
#define SIP_RSP_414_REQ_URI_TOO_LONG      414 /**< @brief Response code of "414 request uri too long". */
#define SIP_RSP_415_UNSUPTED_MEDIA        415 /**< @brief Response code of "415 unsupported media". */
#define SIP_RSP_416_UNSUPTED_URI_SCHM     416 /**< @brief Response code of "416 unsupported uri schm". */
#define SIP_RSP_417_UNKNOWN_RES_PRI       417 /**< @brief Response code of "417 unknown res pri". */
#define SIP_RSP_420_BAD_EXTN              420 /**< @brief Response code of "420 bad extension". */
#define SIP_RSP_421_EXTN_REQ              421 /**< @brief Response code of "421 extension request". */
#define SIP_RSP_422_SESSTIMER_TOO_SMALL   422 /**< @brief Response code of "422 session timer too small". */
#define SIP_RSP_423_INTERVAL_TOO_BRIEF    423 /**< @brief Response code of "423 interval too brief". */
#define SIP_RSP_428_USE_ID_HDR            428 /**< @brief Response code of "428 use ID header". */
#define SIP_RSP_429_PROVIDE_REFER_ID      429 /**< @brief Response code of "429 provide refer ID". */
#define SIP_RSP_436_BAD_ID_INFO           436 /**< @brief Response code of "436 bad ID information". */
#define SIP_RSP_437_UNSUPT_CERT           437 /**< @brief Response code of "437 unsupported certification". */
#define SIP_RSP_438_INVAL_ID_HDR          438 /**< @brief Response code of "438 invalid ID header". */
#define SIP_RSP_480_TEMP_UNAVAIL          480 /**< @brief Response code of "480 temporality unavailable". */
#define SIP_RSP_481_CLEG_TRANS_NOT_EXIST  481 /**< @brief Response code of "481 call leg trans not exist". */
#define SIP_RSP_482_LOOP_DETECTED         482 /**< @brief Response code of "482 loop detected". */
#define SIP_RSP_483_TOO_MANY_HOPS         483 /**< @brief Response code of "483 too many hops". */
#define SIP_RSP_484_ADDR_INCOMPLETE       484 /**< @brief Response code of "484 address incomplete". */
#define SIP_RSP_485_AMBIGUOUS             485 /**< @brief Response code of "485 ambiguous". */
#define SIP_RSP_486_BUSY_HERE             486 /**< @brief Response code of "486 busy here". */
#define SIP_RSP_487_REQ_TERMINATED        487 /**< @brief Response code of "487 request terminated". */
#define SIP_RSP_488_NOT_ACPT_HERE         488 /**< @brief Response code of "488 not accept here". */
#define SIP_RSP_489_BAD_EVNT              489 /**< @brief Response code of "489 bad event". */
#define SIP_RSP_491_REQ_PENDING           491 /**< @brief Response code of "491 request pending". */
#define SIP_RSP_493_UNDECIPHERABLE        493 /**< @brief Response code of "493 undecipherable". */
#define SIP_RSP_494_SEC_AGREE_REQ         494 /**< @brief Response code of "494 second agree request". */

#define SIP_RSP_500_INTERNAL_ERR          500 /**< @brief Response code of "500 internal error". */
#define SIP_RSP_501_NOT_IMP               501 /**< @brief Response code of "501 not implement". */
#define SIP_RSP_502_BAD_GATEWAY           502 /**< @brief Response code of "502 bad gateway". */
#define SIP_RSP_503_SERVICE_UNAVAIL       503 /**< @brief Response code of "503 service unavailable". */
#define SIP_RSP_504_SERVER_TIMEOUT        504 /**< @brief Response code of "504 server timeout". */
#define SIP_RSP_505_VER_UNSUPTED          505 /**< @brief Response code of "505 version unsupported". */
#define SIP_RSP_513_MSG_TOO_LARGE         513 /**< @brief Response code of "513 message too large". */
#define SIP_RSP_533_SERVICE_LOST          533 /**< @brief Response code of "533 service lost". */
#define SIP_RSP_580_PRECON_FAIL           580 /**< @brief Response code of "580 precondition fail". */

#define SIP_RSP_600_BUSY_EVERYWHERE       600 /**< @brief Response code of "600 busy everywhere". */
#define SIP_RSP_603_DECLINE               603 /**< @brief Response code of "603 decline". */
#define SIP_RSP_604_NOT_EXIST_ANYWHERE    604 /**< @brief Response code of "604 not exist anywhere". */
#define SIP_RSP_606_NOT_ACPT              606 /**< @brief Response code of "606 not accept". */
/** @} */

/* sip call close event */
typedef struct tagSIP_CLOSE_Call_EVNT
{
    ZCOOKIE zCookie;
    ZUINT iSessUserId;
    ZUINT iDlgUserId;
} ST_SIP_CLOSE_CALL_EVNT;

/** @{ */
/** @brief SIP MD5 string len, 32 and a ending char. */
#define SIP_MD5_HEX_STR_LEN 33
/** @} */

/**
 * @brief Open SIP log of specific level.
 *
 * @param [in] iLogMod Specific module in SIP.
 * @param [in] iLevel Log level.
 *
 * @retval ZOK Open SIP log successfully.
 * @retval ZFAILED Open failed.
 */
ZFUNC ZINT Sip_LogOpen(ZUINT iLogMod, ZUINT iLevel);

/**
 * @brief Close SIP log of specific level.
 *
 * @param [in] iLogMod Specific module in SIP.
 * @param [in] iLevel Log level.
 *
 * @retval ZOK Close SIP log successfully.
 * @retval ZFAILED Close failed.
 */
ZFUNC ZINT Sip_LogClose(ZUINT iLogMod, ZUINT iLevel);

/**
 * @brief Initialize SIP session event.
 *
 * @param [in] pstEvnt SIP session event data structure.
 *
 * @retval ZOK Initialize successfully.
 * @retval ZFAILED Initialize failed.
 */
ZFUNC ZINT Sip_SessEvntInit(ST_SIP_SESS_EVNT *pstEvnt);

/**
 * @brief Free SIP session event.
 *
 * @param [in] pstEvnt SIP session event data structure.
 *
 * @retval ZOK Free successfully.
 * @retval ZFAILED Free failed.
 */
ZFUNC ZINT Sip_SessEvntFree(ST_SIP_SESS_EVNT *pstEvnt);

/**
 * @brief Get description string of specific session event type.
 *
 * @param [in] iEvntType SIP session event type, @ref EN_SIP_SESS_EVNT_TYPE.
 *
 * @return Description string of session event type.
 */
ZFUNC ZCHAR * Sip_GetSessEvntDesc(ZUINT iEvntType);

/**
 * @brief Let SIP stack to listen on speicifc address.
 *
 * @param [in] ucTptType The transport type, @ref EN_UTPT_TPT_TYPE.
 * @param [in] pstAddr The address to listen.
 *
 * @retval ZOK Add listen address successfully.
 * @retval ZFAILED Add listen address failed.
 */
ZFUNC ZINT Sip_AddListenAddr(ZUCHAR ucTptType, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Get reason description string of specific status code.
 *
 * @param [in] dwCode Status code.
 * @param [out] pstReason Reason description string.
 *
 * @retval ZOK Get description string successfully.
 * @retval ZFAILED Get description string failed.
 */
ZFUNC ZINT Sip_ReasonFromCode(ZUINT iCode, ST_ZOS_SSTR *pstReason);

/**
 * @brief Get description string of specific method type.
 *
 * @param [in] ucMethod SIP method type, @ref EN_SIP_METHOD.
 *
 * @return Description string of method type.
 */
ZFUNC ZCHAR * Sip_GetMethodDesc(ZUCHAR ucMethod);

/**
 * @brief Get description string of specific status code.
 *
 * @param [in] iStatCode Status code.
 *
 * @return Description string of status code.
 */
ZFUNC ZCHAR * Sip_GetStatCodeDesc(ZUINT iStatCode);

/**
 * @brief Calculate A1 value.
 *
 * @param [in] pstUsername User name string.
 * @param [in] pstPassword User password string.
 * @param [in] pstRealm Realm string.
 * @param [out] acResultStr A1 value.
 *
 * @retval ZOK Calculate successfully.
 * @retval ZFAILED Calculate failed.
 */
ZFUNC ZINT Sip_CalcA1(ST_ZOS_SSTR *pstUsername, ST_ZOS_SSTR *pstPassword,
                ST_ZOS_SSTR *pstRealm, ZCHAR acResultStr[SIP_MD5_HEX_STR_LEN]);

/**
 * @brief Calculate A2 value.
 *
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstDigestUri Digest URI data.
 * @param [out] acResultStr A2 value.
 *
 * @retval ZOK Calculate successfully.
 * @retval ZFAILED Calculate failed.
 */
ZFUNC ZINT Sip_CalcA2(ZUCHAR ucMethod, ST_SIP_REQ_URI *pstDigestUri,
                ZCHAR acResultStr[SIP_MD5_HEX_STR_LEN]);

/**
 * @brief Calculate A2 value.
 *
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstDigestUri Digest URI string.
 * @param [out] acResultStr A2 value.
 *
 * @retval ZOK Calculate successfully.
 * @retval ZFAILED Calculate failed.
 */
ZFUNC ZINT Sip_CalcA2X(ZUCHAR ucMethod, ST_ZOS_SSTR *pstDigestUri,
                ZCHAR acResultStr[SIP_MD5_HEX_STR_LEN]);

/**
 * @brief Calculate KD value.
 *
 * @param [in] acA1Md5Str A1 value.
 * @param [in] acA2Md5Str A2 value.
 * @param [in] pstNonce Nonce string.
 * @param [in] pstCnonce CNonce string.
 * @param [in] pstNonceCount Nonce count string.
 * @param [in] pstQop Qop string.
 * @param [out] acResultStr KD value.
 *
 * @retval ZOK Calculate successfully.
 * @retval ZFAILED Calculate failed.
 */
ZFUNC ZINT Sip_CalcKd(ZCHAR acA1Md5Str[SIP_MD5_HEX_STR_LEN],
                ZCHAR acA2Md5Str[SIP_MD5_HEX_STR_LEN], ST_ZOS_SSTR *pstNonce,
                ST_ZOS_SSTR *pstCnonce, ST_ZOS_SSTR *pstNonceCount,
                ST_ZOS_SSTR *pstQop, ZCHAR acResultStr[SIP_MD5_HEX_STR_LEN]);

/**
 * @brief Get re-transmission count.
 *
 * @return Re-transmission count of sip message.
 */
ZFUNC ZUINT Sip_StsGetReTransCount(ZFUNC_VOID);

/* sip force close connection */
ZFUNC ZINT Sip_ForceCloseConn(ST_SIP_TPT_ADDR *pstAddr);

/* sip force close sip call resource */
ZFUNC ZINT Sip_ForceCloseCall(ZVOID *pComp, ZUINT iSessUserId);

/* sip force close sip subscription call resource */
ZFUNC ZINT Sip_ForceCloseSubs(ZVOID *pComp, ZUINT iSessUserId, ZUINT iDlgUsrId);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_UTIL_H__ */

