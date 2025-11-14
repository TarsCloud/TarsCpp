/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : imap_evnt.h
  Module        : imap protocol
  Author        : tanek.ye
  Version       : 0.1
  Created on    : 2013-03-26
  Description   :
    Marcos and structure definitions required by the imap event.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_EVNT_H__
#define _IMAP_EVNT_H__

/** 
 * @file imap_evnt.h
 * @brief IMAP Event Interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief imap event status type */
typedef enum EN_IMAP_EVNT_STAT_TYPE
{
    EN_IMAP_EVNT_STAT_SESS_CONNED,        /** @brief session connected */
    EN_IMAP_EVNT_STAT_SESS_CONNING,       /** @brief session connecting */
    EN_IMAP_EVNT_STAT_SESS_DISCED,        /** @brief session disconnected */
    EN_IMAP_EVNT_STAT_SESS_CONN_ERR,      /** @brief session connection error */
    EN_IMAP_EVNT_STAT_FILE_SENDING,       /** @brief session file sending */
    EN_IMAP_EVNT_STAT_FILE_SEND_LAST,     /** @brief session file send last */
    EN_IMAP_EVNT_STAT_FILE_SEND_OK,       /** @brief session file send ok */
    EN_IMAP_EVNT_STAT_FILE_SEND_FAIL,     /** @brief session file send failure */
    EN_IMAP_EVNT_STAT_FILE_RECVING,       /** @brief session file receiving */
    EN_IMAP_EVNT_STAT_FILE_RECV_DONE,     /** @brief session file receive done */
    EN_IMAP_EVNT_STAT_FILE_RECV_FAIL,     /** @brief session file receive failure */
    EN_IMAP_EVNT_STAT_MSG_SENDING,        /** @brief message sending */
    EN_IMAP_EVNT_STAT_MSG_SEND_OK,        /** @brief message send ok */
    EN_IMAP_EVNT_STAT_MSG_SEND_FAIL,      /** @brief message send failure */
    EN_IMAP_EVNT_STAT_MSG_RECVING,        /** @brief receiving */
    EN_IMAP_EVNT_STAT_MSG_RECV_DONE,      /** @brief receive done */
    EN_IMAP_EVNT_STAT_MSG_RPT_SUCC,       /** @brief report send success */
    EN_IMAP_EVNT_STAT_MSG_RPT_FAIL        /** @brief report send failure */
} EN_IMAP_EVNT_STAT_TYPE;

/** @brief  IMAP event callback */
typedef ZINT (*PFN_IMAPEVNTCB)(ZEVNT zEvntId);

/**
 * @brief Get the session id of session event.
 *
 * @param [in] zEvntId The event id.
 *
 * @retval The session id successfully, otherwise return 0.
 *
 * @see @ref Imap_EvntGetMsgId
 */
ZFUNC ZVOID * Imap_EvntGetSessId(ZEVNT zEvntId);

/* imap event get session transaction id */
ZFUNC ZVOID * Imap_EvntGetTransId(ZEVNT zEvntId);

/* imap event get session remote addr */
ZFUNC ZUINT Imap_EvntGetRmtAddr(ZEVNT zEvntId);

/* imap event get session remote addr6 */
ZFUNC ZCHAR * Imap_EvntGetRmtAddr6(ZEVNT zEvntId);

#ifdef __cplusplus
}
#endif

#endif /* _IMAP_EVNT_H__ */
