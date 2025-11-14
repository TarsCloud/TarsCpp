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
  File name     : zlcs.h
  Module        : license functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the 
      license interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZLCS_H__
#define _ZLCS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ZLCS_NO_ERR         0
#define ZLCS_ERR_INV_PARM   -10
#define ZLCS_ERR_INV_FILE   -11
#define ZLCS_ERR_INV_SIGN_FILE -12
#define ZLCS_ERR_NO_MEM     -13
#define ZLCS_ERR_NOT_FOUND  -14
#define ZLCS_ERR_INV_FORMAT -15
#define ZLCS_ERR_ENCRYPT    -16
#define ZLCS_ERR_NETWORK    -17
#define ZLCS_ERR_OTHER      -99
#define ZLCS_ERR_VERIFY     -100
#define ZLCS_ERR_EXPIRED    -101
#define ZLCS_ERR_INVALID_DEVICE -102
#define ZLCS_ERR_ACTIVATION -103
#define ZLCS_ERR_INVALID_APP -104
#define ZLCS_ERR_GET_DEVICE_FAIL -105

/** @brief zlcs get device ID type only for android devices */
typedef enum EN_ZOS_DEVICE_ID_TYPE
{
    EN_ZOS_DEVICE_ID_SERIAL = 0,          /* device serial number */
    EN_ZOS_DEVICE_ID_IMEI,            /* device imei */
    EN_ZOS_DEVICE_ID_MACADDR,         /* device mac address  */
    EN_ZOS_DEVICE_ID_ANDROID_ID,      /* device android id */
} EN_ZOS_DEVICE_ID_TYPE;

/* activate callback function */
typedef ZVOID (* PFN_MTCLCSLOADCB)(ZBOOL bActivated);

/* get device id */
ZFUNC ZCONST ZCHAR * Zlcs_GetDeviceId(ZFUNC_VOID);

/* get device id */
ZFUNC ZCONST ZCHAR * Zlcs_GetDeviceIdX(ZUINT iType);

/* check device id */
ZFUNC ZBOOL Zlcs_ChkDeviceId(ZCONST ZCHAR *pcLcsDevId);

/* get device name */
ZFUNC ZCONST ZCHAR * Zlcs_GetDeviceName(ZFUNC_VOID);

/* get device name */
ZFUNC ZCONST ZCHAR * Zlcs_GetAppName(ZFUNC_VOID);

/* get last error number */
ZFUNC ZINT Zlcs_GetLastErrNo();

/* load license data from file */
ZFUNC ZVOID * Zlcs_Load(ZCONST ZCHAR *pcFileName, ZBOOL bPrint);

/* free license data */
ZFUNC ZVOID Zlcs_Free(ZVOID *pLcs);

/* check if is expired */
ZFUNC ZBOOL Zlcs_IsValid(ZVOID *pLcs, ZBOOL bPrint);

/* get license expired date time */
ZFUNC ZTIME_T Zlcs_GetExpireTime();

/* check if the PE file signature is verified */
ZFUNC ZBOOL Zos_IsSignVerified(ZBOOL bPrint);

/* check if it is limit module */
ZFUNC ZBOOL Zlcs_IsModuleLimit(ZVOID *pLcs);

/* check if is is valid module */
ZFUNC ZBOOL Zlcs_IsValidModule(ZVOID *pLcs, ZCONST ZCHAR *pcName);

/* check if it is valid audio codec */
ZFUNC ZBOOL Zlcs_IsValidAudioCodec(ZVOID *pLcs, ZCONST ZCHAR *pcName);

/* check if it is valid video codec */
ZFUNC ZBOOL Zlcs_IsValidVideoCodec(ZVOID *pLcs, ZCONST ZCHAR *pcName);

/* check if is valid time(not expired) */
ZFUNC ZBOOL Zlcs_IsValidTime(ZVOID *pLcs, ZBOOL bPrint);

/* check if is valid device id */
ZFUNC ZBOOL Zlcs_IsValidDevId(ZVOID *pLcs, ZBOOL bPrint);

/* check if is valid app id */
ZFUNC ZBOOL Zlcs_IsValidAppId(ZVOID *pLcs, ZBOOL bPrint);

/* check if it is activated */
ZFUNC ZBOOL Zlcs_IsActivated(ZVOID *pLcs);

/* activate the license */
ZFUNC ZINT Zlcs_Activate(ZCONST ZCHAR *pcServer, ZUINT iPort, 
        ZCONST ZCHAR *pcUri,ZCONST ZCHAR *pcFileName, ZCONST ZCHAR *pcCompanyName, 
        ZCONST ZCHAR *pcProductName, PFN_MTCLCSLOADCB pfnResultProc);

/* check if license  has audio codec */
ZFUNC ZBOOL Zlcs_IsLcsHasAudioCodec();

/* zlcs get specified key size */
ZFUNC ZUINT Zlcs_GetSpecifiedKeySize(ZCHAR *pcSect, ZCHAR *pcKey);

/* zlcs get specified key value */
ZFUNC ZCHAR * Zlcs_GetSpecifiedKeyVal(ZCHAR *pcSect, ZCHAR *pcKey, ZUINT iIndex);

/* zlcs get X-Service-Type */
ZFUNC ZCHAR * Zlcs_GetSrvType();

/* zlcs get UseTelUri value */
ZFUNC ZINT Zlcs_GetUseTelUri(ZBOOL *pbUseTelUri);

/* zlcs get UseDmPwd value */
ZFUNC ZINT Zlcs_GetUseDmPwd(ZBOOL *pbUseDmPwd);

/* zlcs get os version value */
ZFUNC ZUINT Zlcs_GetOsVersion();

#ifdef __cplusplus
}
#endif

#endif /* _ZLCS_H__ */

