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
  File name     : msrp_evnt.h
  Module        : msrp protocol
  Author        : cathy.chen
  Version       : 0.1
  Created on    : 2010-05-22
  Description   :
    Marcos and structure definitions required by the msrp event.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_EVNT_H__
#define _MSRP_EVNT_H__

/** 
 * @file msrp_evnt.h
 * @brief MSRP Event Interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief msrp event status type */
typedef enum EN_MSRP_EVNT_STAT_TYPE
{
    EN_MSRP_EVNT_STAT_SESS_CONNED,        /** @brief session connected */
    EN_MSRP_EVNT_STAT_SESS_CONNING,       /** @brief session connecting */
    EN_MSRP_EVNT_STAT_SESS_DISCED,        /** @brief session disconnected */
    EN_MSRP_EVNT_STAT_SESS_CONN_ERR,      /** @brief session connection error */
    EN_MSRP_EVNT_STAT_FILE_SENDING,       /** @brief session file sending */
    EN_MSRP_EVNT_STAT_FILE_SEND_LAST,     /** @brief session file send last */
    EN_MSRP_EVNT_STAT_FILE_SEND_OK,       /** @brief session file send ok */
    EN_MSRP_EVNT_STAT_FILE_SEND_FAIL,     /** @brief session file send failure */
    EN_MSRP_EVNT_STAT_FILE_RECVING,       /** @brief session file receiving */
    EN_MSRP_EVNT_STAT_FILE_RECV_DONE,     /** @brief session file receive done */
    EN_MSRP_EVNT_STAT_FILE_RECV_FAIL,     /** @brief session file receive failure */
    EN_MSRP_EVNT_STAT_MSG_SENDING,        /** @brief message sending */
    EN_MSRP_EVNT_STAT_MSG_SEND_OK,        /** @brief message send ok */
    EN_MSRP_EVNT_STAT_MSG_SEND_FAIL,      /** @brief message send failure */
    EN_MSRP_EVNT_STAT_MSG_RECVING,        /** @brief receiving */
    EN_MSRP_EVNT_STAT_MSG_RECV_DONE,   /** @brief receive done */
    EN_MSRP_EVNT_STAT_MSG_RPT_SUCC,      /** @brief report send success */
    EN_MSRP_EVNT_STAT_MSG_RPT_FAIL,       /** @brief report send failure */
    EN_MSRP_EVNT_STAT_MSG_RSP_TIMEOUT,      /** @brief wait msrp 200 ok timeout */
} EN_MSRP_EVNT_STAT_TYPE;

/** @brief  MSRP event callback */
typedef ZINT (*PFN_MSRPEVNTCB)(ZEVNT zEvnt);

/**
 * @brief Register MSRP session event callback.
 *
 * @param [in] pfnCallback The event callback.
 *
 * @see @ref Msrp_EvntUnregCallback
 */
ZFUNC ZVOID Msrp_EvntRegCallback(PFN_MSRPEVNTCB pfnCallback);

/**
 * @brief Unregister MSRP session event callback.
 *
 * @see @ref Msrp_EvntRegCallback
 */
ZFUNC ZVOID Msrp_EvntUnregCallback(ZFUNC_VOID);

/**
 * @brief Get the session id of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The session id successfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetSmsgId
 */
ZFUNC ZVOID * Msrp_EvntGetSessId(ZEVNT zEvnt);

/**
 * @brief Get the session message id of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The session message id successfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetSessId
 */
ZFUNC ZVOID * Msrp_EvntGetSmsgId(ZEVNT zEvnt);

/**
 * @brief Get the session user id of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The session user id successfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetSessId
 */
ZFUNC ZUINT Msrp_EvntGetUserId(ZEVNT zEvnt);

/**
 * @brief Get the session user cookie of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The session user cookiesuccessfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetSessId
 */
ZFUNC ZCOOKIE Msrp_EvntGetCookie(ZEVNT zEvnt);

/**
 * @brief Get the message total data length of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The message total data length successfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetRecvLen
 */
ZFUNC ZUINT Msrp_EvntGetTotalLen(ZEVNT zEvnt);

/**
 * @brief Get the message received data length of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The message received data length successfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetSendLen
 */
ZFUNC ZUINT Msrp_EvntGetRecvLen(ZEVNT zEvnt);

/**
 * @brief Get the message sent data length of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The message sent data length successfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetRecvLen
 */
ZFUNC ZUINT Msrp_EvntGetSendLen(ZEVNT zEvnt);

/**
 * @brief Get the message content type of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The message content type successfully, otherwise return ZNULL.
 *
 * @see @ref Msrp_EvntGetSessId
 */
ZFUNC ZCHAR * Msrp_EvntGetContentType(ZEVNT zEvnt);

/**
 * @brief Get the message MIME body boundary string of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The message MIME body boundary string successfully, otherwise return ZNULL.
 *
 * @see @ref Msrp_EvntGetSessId
 */
ZFUNC ZCHAR * Msrp_EvntGetBoundary(ZEVNT zEvnt);

/**
 * @brief Get the message data range start of session event.
 *
 * @param [in] zEvnt The event id.
 *
 * @retval The message data range start successfully, otherwise return 0.
 *
 * @see @ref Msrp_EvntGetSessId
 */
ZFUNC ZUINT Msrp_EvntGetRangeStart(ZEVNT zEvnt);

/**
 * @brief Get the status description of session event.
 *
 * @param [in] iType The event status type, see @ref EN_MSRP_EVNT_STAT_TYPE.
 *
 * @retval The status description.
 */
ZFUNC ZCHAR * Msrp_EvntGetStatDesc(ZUINT iType);

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_EVNT_H__ */

