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
  File name     : zos_ims.h
  Module        : zos http library functions
  Author        : Bob Liu 
  Version       : 1.0
  Created on    : 2015-07-20    
  Description   :
      Data structure and function definitions required by the socket interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_IMS_H__
#define _ZOS_IMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ZOS_IMS_CONNMGR_ONCOMMANDSTATUS "kZosImsConnMgrOnCommandStatus"
#define ZOS_IMS_CONNMGR_ONCONFIGURATIONCHANGEA "kZosImsConnMgrOnConfigurationChange"
#define ZOS_IMS_CONNMGR_ONSERVICEREADY "kZosImsConnMgrOnServiceReady"
#define ZOS_IMS_CONNMGR_ONSTATUSCHANGE "kZosImsConnMgrOnStatusChange"
#define ZOS_IMS_CONN_HANDLEINCOMINGMESSAGE "kZosImsConnHandleIncomingMessage"
#define ZOS_IMS_CONN_ONCOMMANDSTATUS "kZosImsConnOnCommandStatus"
#define ZOS_IMS_CONN_ONEVENTRECEIVED "kZosImsConnOnEventReceived"

#define ZOS_IMS_USER_CONFIG_iUEClientPort ".iUEClientPort = "
#define ZOS_IMS_USER_CONFIG_iUEServerPort ".iUEServerPort = "
#define ZOS_IMS_USER_CONFIG_pAssociatedURI ".pAssociatedURI = <"
#define ZOS_IMS_USER_CONFIG_pUEPublicIPAddress ".pUEPublicIPAddress = "
#define ZOS_IMS_USER_CONFIG_iUEPublicPort ".iUEPublicPort = "
#define ZOS_IMS_USER_CONFIG_pSipPublicUserId ".pSipPublicUserId = "
#define ZOS_IMS_USER_CONFIG_pSipPrivateUserId ".pSipPrivateUserId = "
#define ZOS_IMS_USER_CONFIG_pSipHomeDomain ".pSipHomeDomain = "
#define ZOS_IMS_USER_CONFIG_pUEPubGruu ".pUEPubGruu = "
#define ZOS_IMS_USER_CONFIG_pLocalHostIPAddress ".pLocalHostIPAddress = "
#define ZOS_IMS_USER_CONFIG_eIpType ".eIpType = "
#define ZOS_IMS_USER_CONFIG_pIMEIStr ".pIMEIStr = "
#define ZOS_IMS_USER_CONFIG_iUEOldSAClientPort ".iUEOldSAClientPort = "

#define ZOS_IMS_DEVICE_CONFIG_bUEBehindNAT ".bUEBehindNAT = "
#define ZOS_IMS_DEVICE_CONFIG_bIpSecEnabled ".bIpSecEnabled = "
#define ZOS_IMS_DEVICE_CONFIG_bCompactFormEnabled ".bCompactFormEnabled = "
#define ZOS_IMS_DEVICE_CONFIG_bKeepAliveEnableStatus ".bKeepAliveEnableStatus = "
#define ZOS_IMS_DEVICE_CONFIG_bGruuEnabled ".bGruuEnabled = "
#define ZOS_IMS_DEVICE_CONFIG_pStrSipOutBoundProxyName ".pStrSipOutBoundProxyName = "
#define ZOS_IMS_DEVICE_CONFIG_iSipOutBoundProxyPort ".iSipOutBoundProxyPort = "
#define ZOS_IMS_DEVICE_CONFIG_iPCSCFClientPort ".iPCSCFClientPort = "
#define ZOS_IMS_DEVICE_CONFIG_iPCSCFServerPort ".iPCSCFServerPort = "
#define ZOS_IMS_DEVICE_CONFIG_pArrAuthChallenge ".pArrAuthChallenge = "
#define ZOS_IMS_DEVICE_CONFIG_pArrNC ".pArrNC = "
#define ZOS_IMS_DEVICE_CONFIG_pServiceRoute ".pServiceRoute = "
#define ZOS_IMS_DEVICE_CONFIG_pSecurityVerify ".pSecurityVerify = "
#define ZOS_IMS_DEVICE_CONFIG_iPCSCFOldSAClientPort ".iPCSCFOldSAClientPort = "
#define ZOS_IMS_DEVICE_CONFIG_iTCPThresholdValue ".iTCPThresholdValue = "
#define ZOS_IMS_DEVICE_CONFIG_sPANI ".sPANI = "
#define ZOS_IMS_DEVICE_CONFIG_sPATH ".sPATH = "

#define ZOS_IMS_IP_V4 "QIMSCM_IPTYPE_V4"
#define ZOS_IMS_IP_V6 "QIMSCM_IPTYPE_V6"

/* zos ims cm status type */
typedef enum EN_ZOS_IMS_CM_STAT_TYPE
{
    EN_ZOS_IMS_CM_STAT_DEINIT,
    EN_ZOS_IMS_CM_STAT_INIT_IN_PROGRESS,
    EN_ZOS_IMS_CM_STAT_SUCCESS,
    EN_ZOS_IMS_CM_STAT_FAILURE,
    EN_ZOS_IMS_CM_STAT_SERVICE_DIED,
    EN_ZOS_IMS_CM_STAT_SERVICE_CLOSING,
    EN_ZOS_IMS_CM_STAT_SERVICE_CLOSED,
    EN_ZOS_IMS_CM_STAT_SERVICE_RESTARTED,
    EN_ZOS_IMS_CM_STAT_SERVICE_NOT_SUPPORTED,
    EN_ZOS_IMS_CM_STAT_SERVICE_UNKNOWN,
} EN_ZOS_IMS_CM_STAT_TYPE;

/* zos ims cm status code type */
typedef enum EN_ZOS_IMS_CM_STAT_CODE
{
    EN_ZOS_IMS_CM_SUCCESS,
    EN_ZOS_IMS_CM_FAILURE,
    EN_ZOS_IMS_CM_MEMORY_ERROR,
    EN_ZOS_IMS_CM_INVALID_LISTENER,
    EN_ZOS_IMS_CM_INVALID_PARAM,
    EN_ZOS_IMS_CM_SERVICE_NOTALLOWED,
    EN_ZOS_IMS_CM_SERVICE_UNAVAILABLE,
    EN_ZOS_IMS_CM_INVALID_FEATURE_TAG,
    EN_ZOS_IMS_CM_DNSQUERY_PENDING,
    EN_ZOS_IMS_CM_DNSQUERY_FAILURE,
    EN_ZOS_IMS_CM_SERVICE_DIED,
    EN_ZOS_IMS_CM_MESSAGE_NOTALLOWED,
} EN_ZOS_IMS_CM_STAT_CODE;

/* zos ims cm event type */
typedef enum EN_ZOS_IMS_CONN_EVENT_TYPE
{
    EN_ZOS_IMS_CONN_EVENT_SERVICE_NOTREGISTERD,
    EN_ZOS_IMS_CONN_EVENT_SERVICE_REGISTERD,
    EN_ZOS_IMS_CONN_EVENT_SERVICE_ALLOWED,
    EN_ZOS_IMS_CONN_EVENT_SERVICE_NOTALLOWED,
    EN_ZOS_IMS_CONN_EVENT_SERVICE_FORCEFUL_CLOSE,
    EN_ZOS_IMS_CONN_EVENT_SERVICE_TERMINATE_CONNECTION,
} EN_ZOS_IMS_CONN_EVENT_TYPE;

/* zos ims cm sip protocol type */
typedef enum EN_ZOS_IMS_SIP_PROTOCOL_TYPE
{
    EN_ZOS_IMS_SIP_PROTOCOL_UDP,
    EN_ZOS_IMS_SIP_PROTOCOL_TCP,
} EN_ZOS_IMS_SIP_PROTOCOL_TYPE;

/* zos ims cm configuration type */
typedef enum EN_ZOS_IMS_CONFIG_TYPE
{
    EN_ZOS_IMS_USER_CONFIG,
    EN_ZOS_IMS_DEVICE_CONFIG,
    EN_ZOS_IMS_AUTO_CONFIG,
} EN_ZOS_IMS_CONFIG_TYPE;

/* zos ims cm message type */
typedef enum EN_ZOS_IMS_MESSAGE_TYPE
{
    EN_ZOS_IMS_MESSAGE_REQUEST, 
    EN_ZOS_IMS_MESSAGE_RESPONSE, 
} EN_ZOS_IMS_MESSAGE_TYPE;

/* zos ims handle create connection manager */
ZFUNC ZIMSID Zos_ImsHandleCreateConnectionManager();

/* zos ims handle trigger registration */
ZFUNC ZINT Zos_ImsHandleTriggerRegistration(ZIMSID zIms);

/* zos ims handle trigger deregistration */
ZFUNC ZINT Zos_ImsHandleTriggerDeRegistration(ZIMSID zIms);

/* zos ims handle create im connection */
ZFUNC ZINT Zos_ImsHandleCreateIMConnection(ZIMSID zIms, ZCHAR *pcFeatureTag);

/* zos ims handle send im message */
ZFUNC ZINT Zos_ImsHandleSendIMMessage(ZIMSID zIms, ZCHAR *pcOutboundProxy, 
                    ZUSHORT wRemotePort, ZINT iProtocol, ZINT iMessageType, 
                    ZCHAR *pcCallId, ZCHAR *pcMessage, ZINT iMessageLen);

/* zos ims handle auto config */
ZFUNC ZINT Zos_ImsHandleAutoconfig(ZIMSID zIms, ZINT pcConfigType, ZINT pcUserData);

/* zos ims handle set status */
ZFUNC ZINT Zos_ImsHandleSetStatus(ZIMSID zIms, ZCHAR *pcFeatureTag);

/* zos ims config get value string */
ZFUNC ZCHAR * Zos_ImsConfigGetValueStr(ZCHAR *pcConfigList, ZCHAR *pcConfigName);

/* zos ims config get value uint */
ZFUNC ZUINT Zos_ImsConfigGetValueUint(ZCHAR *pcConfigList, ZCHAR *pcConfigName);

/* zos ims config get value bool */
ZFUNC ZBOOL Zos_ImsConfigGetValueBool(ZCHAR *pcConfigList, ZCHAR *pcConfigName);

/* zos ims config get pAssociatedURI string */
ZFUNC ZCHAR * Zos_ImsConfigGetpAssociatedURI(ZCHAR *pcConfigList, ZCHAR *pcConfigName);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_IMS_H__ */

