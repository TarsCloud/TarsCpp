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
  File name     : sip_type.h
  Module        : sip protocol typedefs
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_TYPE_H__
#define _SIP_TYPE_H__

/** 
 * @file
 * @brief SIP type definations.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Base number of error. */
#define SIP_ERR_BASE 0x1000

/** @brief Error number in SIP. */
typedef enum EN_SIP_ERR_TYPE
{
    EN_SIP_SESS_ERR_NO = SIP_ERR_BASE,
    EN_SIP_SESS_ERR_TIMEOUT,
    EN_SIP_SESS_ERR_ENCODE,
    EN_SIP_SESS_ERR_DECODE,
    EN_SIP_SESS_ERR_RES,
    EN_SIP_SESS_ERR_INV_REQ,
    EN_SIP_SESS_ERR_MISS_FROMTO,
    EN_SIP_SESS_ERR_CPY_ADDR,
    EN_SIP_SESS_ERR_SELECT_TPT,
    EN_SIP_SESS_ERR_DNS,
    EN_SIP_SESS_ERR_ADD_HDR,
    EN_SIP_SESS_ERR_SESS_CREATE,
    EN_SIP_SESS_ERR_SESS_NOTFOUND,
    EN_SIP_SESS_ERR_SESS_EXIST,
    EN_SIP_SESS_ERR_SESS_TERM,
    EN_SIP_SESS_ERR_DLG_CREATE,
    EN_SIP_SESS_ERR_DLG_NOTFOUND,
    EN_SIP_SESS_ERR_DLG_EXIST,
    EN_SIP_SESS_ERR_DLG_STATE,
    EN_SIP_SESS_ERR_DLG_FORK,
    EN_SIP_SESS_ERR_DLG_MATCH,
    EN_SIP_SESS_ERR_DLG_INV_EVNT,
    EN_SIP_SESS_ERR_DLG_NTFY_EVNT,
    EN_SIP_SESS_ERR_TRANS_CREATE,
    EN_SIP_SESS_ERR_TRANS_NOTFOUND,
    EN_SIP_SESS_ERR_TRANS_EXIST,
    EN_SIP_SESS_ERR_TRANS_FAIL,
    EN_SIP_SESS_ERR_TRANS_MATCH,
    EN_SIP_SESS_ERR_TRANS_MATCHRSP,
    EN_SIP_SESS_ERR_TRANS_INV_EVNT,
    EN_SIP_SESS_ERR_TRANS_INV_MSG,
    EN_SIP_SESS_ERR_TRANS_SENDRSP,
    EN_SIP_SESS_ERR_BUILD_REQ,
    EN_SIP_SESS_ERR_BUILD_RSP,
    EN_SIP_SESS_ERR_COMP_MSG,
    EN_SIP_SESS_ERR_INV_USRID,
    EN_SIP_SESS_ERR_INV_SESSID,
    EN_SIP_SESS_ERR_INV_DLGID,
    EN_SIP_SESS_ERR_INV_TRANSID,
    EN_SIP_SESS_ERR_INV_TPTSRV,
    EN_SIP_SESS_ERR_INV_STATUSCODE,
    EN_SIP_SESS_ERR_SET_METHOD,
    EN_SIP_SESS_ERR_SET_REQLINE,
    EN_SIP_SESS_ERR_SET_STATUSLINE,
    EN_SIP_SESS_ERR_SET_FROM,
    EN_SIP_SESS_ERR_SET_TO,
    EN_SIP_SESS_ERR_SET_FROMTAG,
    EN_SIP_SESS_ERR_SET_TOTAG,
    EN_SIP_SESS_ERR_SET_CALLID,
    EN_SIP_SESS_ERR_SET_CSEQ,
    EN_SIP_SESS_ERR_SET_MAXFORWARDS,
    EN_SIP_SESS_ERR_SET_ALLOW,
    EN_SIP_SESS_ERR_SET_SUPT,
    EN_SIP_SESS_ERR_SET_VIA,
    EN_SIP_SESS_ERR_SET_RECROUTE,
    EN_SIP_SESS_ERR_SET_ROUTE,
    EN_SIP_SESS_ERR_GEN_MSG,
    EN_SIP_SESS_ERR_STORE_RECROUTE,
    EN_SIP_SESS_ERR_STORE_ROUTESET,
    EN_SIP_SESS_ERR_STORE_PEERURI,
    EN_SIP_SESS_ERR_INV_ACK,
    EN_SIP_SESS_ERR_INV_RSP,
    EN_SIP_SESS_ERR_INV_PARM,
    EN_SIP_SESS_ERR_INV_METHOD,
    EN_SIP_SESS_ERR_INV_HDR,
    EN_SIP_SESS_ERR_INV_CONTENT,
    EN_SIP_SESS_ERR_INV_APPEXTN,
    EN_SIP_SESS_ERR_INV_BRANCH,
    EN_SIP_SESS_ERR_STH_NULL,
    EN_SIP_SESS_ERR_SEVNT_UNKNOWN,
    EN_SIP_SESS_ERR_START_TMR,
    EN_SIP_SESS_ERR_SUBS_TMR,
    EN_SIP_SESS_ERR_TPT_SEND,
    EN_SIP_SESS_ERR_SET_EVNT,
    EN_SIP_SESS_ERR_INV_CSEQ,
    EN_SIP_SESS_ERR_INV_SESS_EVNT,
    EN_SIP_SESS_ERR_PRE_SESS_EVNT,
    EN_SIP_SESS_ERR_VAL_MSG,
    EN_SIP_SESS_ERR_TCP_DISC,
    
    EN_SIP_ERR_UNKNOWN
} EN_SIP_ERR_TYPE;

/** @brief Type of SIP event's owner. */
typedef enum EN_SIP_EVNT_OWNER_TYPE
{
    EN_SIP_EOT_TPT,                  /**< @brief Event from transport. */
    EN_SIP_EOT_TRANS,                /**< @brief Event from transaction. */   
    EN_SIP_EOT_DLG,                  /**< @brief Event from dialog. */   
    EN_SIP_EOT_CORE,                 /**< @brief Event from core. */   
    EN_SIP_EOT_TMR,                  /**< @brief Event from timer. */   
    EN_SIP_EOT_SUA                   /**< @brief Event from SIP user agent. */   
} EN_SIP_EVNT_OWNER_TYPE;

/** @brief Type of SIP message. */
typedef enum EN_SIP_EVNT_MSG_TYPE
{
    EN_SIP_EMT_REQ,                  /**< @brief Request message. */
    EN_SIP_EMT_RSP,                  /**< @brief Response message. */
    EN_SIP_EMT_NON                   /**< @brief Not SIP message. */
} EN_SIP_EVNT_MSG_TYPE;
 
/** @brief Type of SIP session event. */
typedef enum EN_SIP_SESS_EVNT_TYPE
{
    /** @{ */
    /** @brief Indication of session error. */
    EN_SIP_SESSE_ERR_IND = 0, 
    /** @} */

    /** @{ */
    /** @brief Confirmation of session initiating message. */
    EN_SIP_SESSE_SIM_CNF = 1,
    /** @brief Indication of session initiating message. */
    EN_SIP_SESSE_SIM_IND = 2,
    /** @} */

    /** @{ */
    /** @brief Confirmation of session status message. */
    EN_SIP_SESSE_SSM_CNF = 3,
    /** @brief Indication of session status message. */
    EN_SIP_SESSE_SSM_IND = 4,
    /** @} */

    /** @{ */
    /** @brief Indication of session acknowledge message. */
    EN_SIP_SESSE_SAM_IND = 5,
    /** @} */

    /** @{ */
    /** @brief Indication of session cancel message. */
    EN_SIP_SESSE_SCM_IND = 6,
    /** @} */

    /** @{ */
    /** @brief Confirmation of session modifying message. */
    EN_SIP_SESSE_SMM_CNF = 7,
    /** @brief Indication of session modifying message. */
    EN_SIP_SESSE_SMM_IND = 8,
    /** @} */

    /** @{ */
    /** @brief Confirmation of session terminating message. */
    EN_SIP_SESSE_STM_CNF = 9,
    /** @brief Indication of session terminating message. */
    EN_SIP_SESSE_STM_IND = 10,
    /** @} */

    /** @{ */
    /** @brief Confirmation of dialog associate message. */
    EN_SIP_SESSE_DAM_CNF = 11,
    /** @brief Indication of dialog associate message. */
    EN_SIP_SESSE_DAM_IND = 12,
    /** @} */

    /** @{ */
    /** @brief Confirmation of call independent message. */
    EN_SIP_SESSE_CIM_CNF = 13,
    /** @brief Indication of call independent message. */
    EN_SIP_SESSE_CIM_IND = 14,
    /** @} */

    /* below message definition can be used by upper modules for
       exchange session event then call sip uri interfaces */
    
    /** @{ */
    /** @brief Request of session initiating message. */
    EN_SIP_SESSE_SIM_REQ = 20,
    /** @brief Response of call independent message. */
    EN_SIP_SESSE_SIM_RSP = 21,
    /** @} */

    /** @{ */
    /** @brief Request of session status message. */
    EN_SIP_SESSE_SSM_REQ = 22,
    /** @brief Response of call independent message. */
    EN_SIP_SESSE_SSM_RSP = 23,
    /** @} */

    /** @{ */
    /** @brief Request of session acknowledge message. */
    EN_SIP_SESSE_SAM_REQ = 24,
    /** @} */

    /** @{ */
    /** @brief Request of session cancel message. */
    EN_SIP_SESSE_SCM_REQ = 25,
    /** @} */

    /** @{ */
    /** @brief Request of session modifying message. */
    EN_SIP_SESSE_SMM_REQ = 26,
    /** @brief Response of call modifying message. */
    EN_SIP_SESSE_SMM_RSP = 27,
    /** @} */

    /** @{ */
    /** @brief Request of session terminating message. */
    EN_SIP_SESSE_STM_REQ = 28,
    /** @} */

    /** @{ */
    /** @brief Request of dialog associate message. */
    EN_SIP_SESSE_DAM_REQ = 29,
    /** @brief Response of dialog associate message. */
    EN_SIP_SESSE_DAM_RSP = 30,
    /** @} */

    /** @{ */
    /** @brief Request of call independent message. */
    EN_SIP_SESSE_CIM_REQ = 31,
    /** @brief Response of call independent message. */
    EN_SIP_SESSE_CIM_RSP = 32,    
    /** @} */

    /** @{ */
    /** @brief Cease the request */
    EN_SIP_SESSE_CEASE_REQ = 33,
    /** @} */

    /** @{ */
    /** @brief Release the resource */
    EN_SIP_SESSE_RELEASE_RES = 34,
    /** @} */

    /** @{ */
    /** @brief keepalive the session */
    EN_SIP_SESSE_KEEP_ALIVE = 35,
    /** @} */
    
    /** @{ */
    /** @brief Release the sip call resource for supporting dual resigter */
    EN_SIP_SESSE_RELEASE_CALL = 36,
    /** @} */

    /** @{ */
    /** @brief Release the sip subscription call resource */
    EN_SIP_SESSE_RELEASE_SUBS = 37,
    /** @} */

    EN_SIP_SESSE_UNKNOWN = 38
} EN_SIP_SESS_EVNT_TYPE;

/** @brief SIP FQDN length */
#define SIP_FQDN_LEN 128

/** @brief Structure of SIP transport address. */
typedef struct tagSIP_TPT_ADDR
{
    ZUCHAR ucType;                   /**< @brief Transport type, ref EN_UTPT_TPT_TYPE. */  
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUSHORT wRmtPort;                /**< @brief Remote port value. */
    ST_ZOS_INET_ADDR stLclAddr;      /**< @brief Local address. */
    ST_ZOS_INET_ADDR stRmtAddr;      /**< @brief Remote address. */
    ZCHAR acRmtHost[SIP_FQDN_LEN];   /**< @brief Remote host port string */
} ST_SIP_TPT_ADDR;

/** @brief Structure of SIP session event. */
typedef struct tagSIP_SESS_EVNT
{
    ZUCHAR ucEvntType;               /**< @brief Event type, @ref EN_SIP_SESS_EVNT_TYPE. */
    ZUCHAR ucEvntOwner;              /**< @brief Event owner, @ref EN_SIP_EVNT_OWNER_TYPE. */
    ZUCHAR ucMsgType;                /**< @brief Message type, @ref EN_SIP_EVNT_MSG_TYPE. */
    ZUCHAR ucMethodType;             /**< @brief Method type, @ref EN_SIP_METHOD. */
    ZUINT iStatusCode;             /**< @brief Status code. */
    ZCOOKIE zCookie;                 /**< @brief User info. */
    ZUINT iSessUserId;             /**< @brief Session user ID. */
    ZUINT iDlgUserId;              /**< @brief Dialog user ID. */
    ZUINT iTransUserId;            /**< @brief Transaction user ID. */
    ZUINT iSessId;                 /**< @brief Session ID. */
    ZUINT iDlgId;                  /**< @brief Dialog ID. */
    ZUINT iTransId;                /**< @brief Transaction ID. */
    ZUINT iReferExpires;           /**< @brief Expires value of REFER. */
    ST_SIP_MSG *pstMsg;              /**< @brief SIP message. */
    ST_SIP_METHOD *pstMethod;        /**< @brief SIP method. */
    ST_SIP_EVNT_PKG *pstEvntPkg;     /**< @brief Event package for subscription. */
    ST_SIP_TPT_ADDR stTptAddr;       /**< @brief Transport address. */
} ST_SIP_SESS_EVNT;

#ifdef __cplusplus
}
#endif

#endif /* _SIP_TYPE_H__ */

