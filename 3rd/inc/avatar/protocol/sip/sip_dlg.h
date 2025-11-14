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
  File name     : sip_dlg.h
  Module        : sip protocol dialog
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_DLG_H__
#define _SIP_DLG_H__

/** 
 * @file
 * @brief SIP dialog interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief SIP invite dialog state type */
typedef enum EN_SIP_IVTD_STATE_TYPE
{
    EN_SIP_IVTD_STATE_IDLE = 1,      /**< @brief Idle state */
    EN_SIP_IVTD_STATE_INVITE_OUT = 2, /**< @brief Invite outgoing state */
    EN_SIP_IVTD_STATE_INVITE_IN = 3, /**< @brief Invite incoming state */
    EN_SIP_IVTD_STATE_EARLY_UAC = 4, /**< @brief Client in early dialog state */
    EN_SIP_IVTD_STATE_EARLY_UAS = 5, /**< @brief Server in early dialog state */
    EN_SIP_IVTD_STATE_CNF = 6,       /**< @brief Confirmed state */
    EN_SIP_IVTD_STATE_MODIFY_OUT = 7, /**< @brief Modify outgoing state */
    EN_SIP_IVTD_STATE_MODIFY_IN = 8, /**< @brief Modify incoming state */
    EN_SIP_IVTD_STATE_TERMINATING = 9, /**< @brief Terminating state */    
    EN_SIP_IVTD_STATE_TERMINATED = 10 /**< @brief Terminated state */    
} EN_SIP_IVTD_STATE_TYPE;

/** @brief SIP subscription dialog state type */
typedef enum EN_SIP_SUBSD_STATE_TYPE
{
    EN_SIP_SUBSD_STATE_IDLE = 1,     /**< @brief Idle state */
    EN_SIP_SUBSD_STATE_SUBS_OUT = 2, /**< @brief Subscription outgoing state */
    EN_SIP_SUBSD_STATE_SUBS_IN = 3,  /**< @brief Subscription incoming state */
    EN_SIP_SUBSD_STATE_CNF = 4,      /**< @brief Confirm state */
    EN_SIP_SUBSD_STATE_TERMINATING = 5, /**< @brief Terminating state */
    EN_SIP_SUBSD_STATE_TERMINATED = 6 /**< @brief Terminated state */
} EN_SIP_SUBSD_STATE_TYPE;

/****************** dialog api usage *****************
  Below api only can be used in below situation:
  1. notify session event callback
  2. invite dialog state change callback
  3. notify subscription dialog event callback

  The reason is below api will using un-protected resources.
  If they were used by non-sip thread, it could case unknown exception
******************************************************/
/**
 * @brief Get INVITE dialog count.
 *
 * @param [out] piCount The INVITE dialog count.
 *
 * @retval ZOK Get count successfully.
 * @retval ZFAILED Get count failed.
 */
ZFUNC ZINT Sip_DlgsGetCount(ZUINT *piCount);

/**
 * @brief Get INVITE dialog ID.
 *
 * @param [in] iIndex The INVITE dialog index.
 * @param [out] piDlgId The INVITE dialog ID.
 *
 * @retval ZOK Get dialog id successfully.
 * @retval ZFAILED Get dialog id failed.
 */
ZFUNC ZINT Sip_DlgsDlgsGetDlg(ZUINT iIndex, ZUINT *piDlgId);

/**
 * @brief Get INVITE dialog state.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] piState The INVITE dialog state.
 *
 * @retval ZOK Get dialog state successfully.
 * @retval ZFAILED Get dialog state failed.
 */
ZFUNC ZINT Sip_DlgGetState(ZUINT iDlgId, ZUINT *piState);

/**
 * @brief Get INVITE dialog passive flag.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] pbPassive The passive flag.
 *
 * @retval ZOK Get dialog passive flag successfully.
 * @retval ZFAILED Get dialog passive flag failed.
 */
ZFUNC ZINT Sip_DlgGetPassive(ZUINT iDlgId, ZBOOL *pbPassive);

/**
 * @brief Get INVITE dialog early state.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] pbEarly The INVITE dialog in early state.
 *
 * @retval ZOK Get dialog early state successfully.
 * @retval ZFAILED Get dialog early state failed.
 */
ZFUNC ZINT Sip_DlgGetEarlyState(ZUINT iDlgId, ZBOOL *pbEarly);

/**
 * @brief Get session ID which contains INVITE dialog.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] piSessId The session ID.
 *
 * @retval ZOK Get session ID successfully.
 * @retval ZFAILED Get session ID failed.
 */
ZFUNC ZINT Sip_DlgGetSessId(ZUINT iDlgId, ZUINT *piSessId);

/**
 * @brief Get Call-ID value of INVITE dialog.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] ppstCallId The Call-ID value.
 *
 * @retval ZOK Get Call-ID successfully.
 * @retval ZFAILED Get Call-ID failed.
 */
ZFUNC ZINT Sip_DlgGetCallId(ZUINT iDlgId, ST_SIP_CALLID **ppstCallId);

/**
 * @brief Get local tag value of INVITE dialog.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] ppstTag The local tag value.
 *
 * @retval ZOK Get local tag successfully.
 * @retval ZFAILED Get local tag failed.
 */
ZFUNC ZINT Sip_DlgGetLocalTag(ZUINT iDlgId, ST_ZOS_SSTR **ppstTag);

/**
 * @brief Get remote tag value of INVITE dialog.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] ppstTag The remote tag value.
 *
 * @retval ZOK Get remote tag successfully.
 * @retval ZFAILED Get remote tag failed.
 */
ZFUNC ZINT Sip_DlgGetRemoteTag(ZUINT iDlgId, ST_ZOS_SSTR **ppstTag);

/**
 * @brief Get local uri value of INVITE dialog.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] ppstUri The local uri value.
 *
 * @retval ZOK Get local uri successfully.
 * @retval ZFAILED Get local uri failed.
 */
ZFUNC ZINT Sip_DlgGetLocalUri(ZUINT iDlgId, ST_SIP_HDR_FROM_TO **ppstUri);

/**
 * @brief Get remote uri value of INVITE dialog.
 *
 * @param [in] iDlgId The INVITE dialog ID.
 * @param [out] ppstUri The remote uri value.
 *
 * @retval ZOK Get remote uri successfully.
 * @retval ZFAILED Get remote uri failed.
 */
ZFUNC ZINT Sip_DlgGetRemoteUri(ZUINT iDlgId, ST_SIP_HDR_FROM_TO **ppstUri);

/**
 * @brief Get SUBSCRIBE dialog count.
 *
 * @param [out] piCount The SUBSCRIBE dialog count.
 *
 * @retval ZOK Get dialog count successfully.
 * @retval ZFAILED Get dialog count failed.
 */
ZFUNC ZINT Sip_SubssGetCount(ZUINT *piCount);

/**
 * @brief Get SUBSCRIBE dialog ID.
 *
 * @param [in] iIndex The SUBSCRIBE dialog index.
 * @param [out] piSubsId The SUBSCRIBE dialog ID.
 *
 * @retval ZOK Get dialog id successfully.
 * @retval ZFAILED Get dialog id failed.
 */
ZFUNC ZINT Sip_SubssGetSubs(ZUINT iIndex, ZUINT *piSubsId);

/**
 * @brief Get SUBSCRIBE dialog state.
 *
 * @param [in] iSubsId The SUBSCRIBE dialog ID.
 * @param [out] piState The SUBSCRIBE dialog state.
 *
 * @retval ZOK Get dialog state successfully.
 * @retval ZFAILED Get dialog state failed.
 */
ZFUNC ZINT Sip_SubsGetState(ZUINT iSubsId, ZUINT *piState);

/**
 * @brief Get SUBSCRIBE dialog passive flag.
 *
 * @param [in] iSubsId The SUBSCRIBE dialog ID.
 * @param [out] pbPassive The passive flag.
 *
 * @retval ZOK Get dialog passive flag successfully.
 * @retval ZFAILED Get dialog passive flag failed.
 */
ZFUNC ZINT Sip_SubsGetPassive(ZUINT iSubsId, ZBOOL *pbPassive);

/**
 * @brief Get session ID which contains SUBSCRIBE dialog.
 *
 * @param [in] iSubsId The SUBSCRIBE dialog ID.
 * @param [out] piSessId The session ID.
 *
 * @retval ZOK Get session ID successfully.
 * @retval ZFAILED Get session ID failed.
 */
ZFUNC ZINT Sip_SubsGetSessId(ZUINT iSubsId, ZUINT *piSessId);

/**
 * @brief Get SUBSCRIBE dialog event package.
 *
 * @param [in] iSubsId The SUBSCRIBE dialog ID.
 * @param [out] piEvntPkg The event type, @ref EN_SIP_EVNT_PKG.
 *
 * @retval ZOK Get event package successfully.
 * @retval ZFAILED Get event package failed.
 */
ZFUNC ZINT Sip_SubsGetEvntPkg(ZUINT iSubsId, ZUINT *piEvntPkg);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_DLG_H__ */

