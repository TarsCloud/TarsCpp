/************************************************************************

        Copyright (c) 2005-2020 by Juphoon System Software, Inc.
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
  File name     : zdiameter.h
  Module        : Diameter
  Author        : phil.zhou  
  Version       : 0.1   
  Created on    : 2020-03-02    
  Description   :
      Marcos and structure definitions required by the Diameter
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZDIAMETER_H__
#define _ZDIAMETER_H__

#ifdef __cplusplus
extern "C"{
#endif 

/** @brief Type define of diameter error type */
#define ZDIAMETER_ERR_NO                0x0000
#define ZDIAMETER_ERR_OTHER             0x0001
#define ZDIAMETER_ERR_TIMEOUT           0x0002
#define ZDIAMETER_ERR_NETWORK           0x0003
#define ZDIAMETER_ERR_CREDIT_LIMIT      0x0004
#define ZDIAMETER_ERR_CEA_FAIL          0x0005

/** @brief Type define of diameter callback for notify session status changed. */
typedef ZVOID (*PFN_ZDIAMETERMESESSONSTATUSCHANGED)(ZUINT iSessId, 
                ZUINT iStatusType, ZUINT iStatCode);

/** @brief Type define of diameter callback for notify session credit control status changed. */
typedef ZVOID (*PFN_ZDIAMETERMESESSONCCSTATUSCHANGED)(ZUINT iSessId, ZUINT iCCId, 
                ZUINT iCCStatusType, ZUINT iStatCode);

/** @brief Type define of diameter session status type */
typedef enum EN_ZDIAMETER_SESS_STATUS_TYPE
{
    EN_ZDIAMETER_SESS_STATUS_IDLE,
    EN_ZDIAMETER_SESS_STATUS_CONNING,
    EN_ZDIAMETER_SESS_STATUS_CONNOK,
    EN_ZDIAMETER_SESS_STATUS_SETUPING,
    EN_ZDIAMETER_SESS_STATUS_SETUPOK,
    EN_ZDIAMETER_SESS_STATUS_DISCCONNING,
    EN_ZDIAMETER_SESS_STATUS_DISCCONNED,
} EN_ZDIAMETER_SESS_STATUS_TYPE;

/** @brief Type define of diameter session credit control status type */
typedef enum EN_ZDIAMETER_SESS_CC_STATUS_TYPE
{
    EN_ZDIAMETER_SESS_CC_STATUS_CHARGING,
    EN_ZDIAMETER_SESS_CC_STATUS_CHARGEOK,
    EN_ZDIAMETER_SESS_CC_STATUS_CHARGEFAIL,
} EN_ZDIAMETER_SESS_CC_STATUS_TYPE;

/** @brief Type define of diameter session disconnect type */
typedef enum EN_ZDIAMETER_SESS_DISC_TYPE
{
    EN_ZDIAMETER_SESS_DISC_REBOOTING,
    EN_ZDIAMETER_SESS_DISC_BUSY,
} EN_ZDIAMETER_SESS_DISC_TYPE;

/** @brief Type define of diameter credit control request type */
typedef enum EN_ZDIAMETER_CC_REQ_TYPE
{
    EN_ZDIAMETER_CC_REQ_UNKNOWN,
    EN_ZDIAMETER_CC_REQ_INIT,
    EN_ZDIAMETER_CC_REQ_UPDATE,
    EN_ZDIAMETER_CC_REQ_TERM,
} EN_ZDIAMETER_CC_REQ_TYPE;

/** @brief Type define of diameter role type */
typedef enum EN_ZDIAMETER_ROLE_TYPE
{
    EN_ZDIAMETER_ROLE_CALLER,
    EN_ZDIAMETER_ROLE_CALLEE
} EN_ZDIAMETER_ROLE_TYPE;

/** @brief Type define of diameter finish type */
typedef enum EN_ZDIAMETER_FINISH_TYPE
{
    EN_ZDIAMETER_FINISH_UNKNOWN,
    EN_ZDIAMETER_FINISH_OA,
    EN_ZDIAMETER_FINISH_DA,
    EN_ZDIAMETER_FINISH_PLATFORM,
} EN_ZDIAMETER_FINISH_TYPE;

/**
 * @brief diameter session connect.
 *
 * While session connect ok or failed, it will be notified by callback which 
 * was set by @ref pfnStatusChanged.
 *
 * @param [in] pcOriginHost Origin host.
 * @param [in] pcOriginRealm Origin realm.
 * @param [in] pcDestHost Destination host.
 * @param [in] pcDestRealm Destination realm.
 * @param [out] pcHostIpAddress Host ip address.
 * @param [out] iVendorId Vendor id.
 * @param [out] pcProductName Product name.
 * @param [out] pcRemoteAddress Remote address, e.g. IP:Port.
 * @param [out] pfnStatusChanged Session status changed callback.
 *
 * @return The session id successfully or ZMAXUINT failed, session connect will be running.
 *
 * @see @ref ZDiameter_SessDisc
 */
ZFUNC ZUINT ZDiameter_SessConn(ZCHAR *pcOriginHost, ZCHAR *pcOriginRealm, 
                ZCHAR *pcDestHost, ZCHAR *pcDestRealm, 
                ZCHAR *pcHostIpAddress, ZUINT iVendorId, ZCHAR *pcProductName, 
                ZCHAR *pcRemoteAddress, 
                PFN_ZDIAMETERMESESSONSTATUSCHANGED pfnStatusChanged);

/**
 * @brief diameter session disconnect.
 *
 * While session disconnect ok, it will be notified by callback which 
 * was set by @ref pfnStatusChanged.
 *
 * @param [in] iSessId Session id.
 * @param [in] iDiscType Disconnect type, see EN_ZDIAMETER_SESS_DISC_TYPE.
 *
 * @retval ZOK session disconnect will be running.
 * @retval ZFAILED session disconnect failed.
 *
 * @see @ref ZDiameter_SessConn
 */
ZFUNC ZINT ZDiameter_SessDisc(ZUINT iSessId, ZUINT iDiscType);

/**
 * @brief diameter session credit control init.
 *
 * While session credit control ok or failed, it will be notified by callback which 
 * was set by @ref pfnStatusChanged.
 *
 * @param [in] iSessId Session id.
 * @param [in] iReqCCTime Request Credit control time in seconds.
 * @param [in] pcChargeSipUri Charging sip uri without "sip:", e.g. +86123@abc.com.
 * @param [in] pcCallingUri Calling uri, using sip uri or tel uri.
 * @param [in] pcCalledUri Called uri, using sip uri or tel uri.
 * @param [in] iRoleType Role type, see EN_ZDIAMETER_ROLE_TYPE.
 * @param [in] pcOrigCallingUri Oringial calling uri, using sip uri or tel uri.
 * @param [in] pcOrigCalledUri Oringial called uri, using sip uri.
 * @param [in] pcMiddleUri Middle uri, using sip uri.
 * @param [in] pcMeetingID Meeting ID.
 * @param [in] pcSessId Session ID.
 * @param [in] pcRouteId Route ID.
 * @param [in] pcAtomId Atom ID.
 * @param [in] pcPriceNum Price Number.
 * @param [out] pfnCCStatusChanged Credit control status changed callback.
 *
 * @return The cc id successfully or ZMAXUINT failed, cc init will be running.
 */
ZFUNC ZUINT ZDiameter_SessCCInit(ZUINT iSessId, ZUINT iReqCCTime, 
                ZCHAR *pcChargeSipUri, ZCHAR *pcCallingUri, 
                ZCHAR *pcCalledUri, ZUINT iRoleType, ZCHAR *pcOrigCallingUri, 
                ZCHAR *pcOrigCalledUri, ZCHAR *pcMiddleUri, ZCHAR *pcMeetingID, 
                ZCHAR *pcSessId, ZCHAR *pcRouteId, ZCHAR *pcAtomId, ZCHAR *pcPriceNum, 
                ZUINT iNumberTypeFlag, PFN_ZDIAMETERMESESSONCCSTATUSCHANGED pfnCCStatusChanged);

/**
 * @brief diameter session credit control update.
 *
 * While session credit control ok or failed, it will be notified by callback which 
 * was set by @ref pfnCCStatusChanged.
 *
 * @param [in] iSessId Session id.
 * @param [in] iCCId Credit control.
 * @param [in] iReqCCTime Request Credit control time in seconds.
 * @param [in] iUsedCCTime Used Credit control time in seconds.
 *
 * @retval ZOK session credit control update will be running.
 * @retval ZFAILED session credit control update failed.
 */
ZFUNC ZINT ZDiameter_SessCCUpdate(ZUINT iSessId, ZUINT iCCId, 
                ZUINT iReqCCTime, ZUINT iUsedCCTime);

/**
 * @brief diameter session credit control terminate.
 *
 * While session credit control ok or failed, it will be notified by callback which 
 * was set by @ref pfnCCStatusChanged.
 *
 * @param [in] iSessId Session id.
 * @param [in] iCCId Credit control.
 * @param [in] iUsedCCTime Used Credit control time in seconds.
 * @param [in] iRingTime Ring Time in seconds.
 * @param [in] iFinishType Finish type, see EN_ZDIAMETER_FINISH_TYPE.
 *
 * @retval ZOK session credit control terminate will be running.
 * @retval ZFAILED session credit control terminate failed.
 */
ZFUNC ZINT ZDiameter_SessCCTerm(ZUINT iSessId, ZUINT iCCId, 
                ZUINT iUsedCCTime, ZUINT iRingTime, ZUINT iFinishType);

/**
 * @brief diameter session credit control get granted time.
 *
 * @param [in] iSessId Session id.
 * @param [in] iCCId Credit control id.
 *
 * @return granted time.
 */
ZFUNC ZUINT ZDiameter_SessCCGetGrantedTime(ZUINT iSessId, ZUINT iCCId);

/**
 * @brief diameter session credit control get request type.
 *
 * @param [in] iSessId Session id.
 * @param [in] iCCId Credit control id.
 *
 * @return request type, see EN_ZDIAMETER_CC_REQ_TYPE.
 */
ZFUNC ZUINT ZDiameter_SessCCGetRequestType(ZUINT iSessId, ZUINT iCCId);

/**
 * @brief diameter session credit control get result code.
 *
 * @param [in] iSessId Session id.
 * @param [in] iCCId Credit control id.
 *
 * @return result code.
 */
ZFUNC ZUINT ZDiameter_SessCCGetResultCode(ZUINT iSessId, ZUINT iCCId);

/* diameter credit control get count */
ZFUNC ZUINT ZDiameter_SessCCGetCount();

#ifdef __cplusplus
}
#endif 

#endif /* _ZDIAMETER_H__ */


