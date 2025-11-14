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
  File name     : httpc_cfg.h
  Module        : http client
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-03-30
  Description   :
    Marcos and structure definitions required by the http config.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTPC_CFG_H__
#define _HTTPC_CFG_H__

#ifdef __cplusplus
extern "C"{
#endif

/* httpc config proxy type */
typedef enum EN_HTTPC_CFG_PROXY_TYPE
{
    EN_HTTPC_CFG_PROXY_OFF,
    EN_HTTPC_CFG_PROXY_HTTP,
    EN_HTTPC_CFG_PROXY_HTTPS,
} EN_HTTPC_CFG_PROXY_TYPE;

/* httpc config get task priority, default is ZTASK_PRIORITY_NORMAL */
ZFUNC ZINT Httpc_CfgGetTaskPriority(ZFUNC_VOID);

/* httpc config get log level, default is ZLOG_LEVEL_ALL */
ZFUNC ZUINT Httpc_CfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief Get TLS method type.
 *
 * @retval TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @see Httpc_CfgSetTlsMethod
 */
ZFUNC ZUINT Httpc_CfgGetTlsMethod(ZFUNC_VOID);

/**
 * @brief Get TLS verify type.
 *
 * @retval TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @see Httpc_CfgSetTlsVeryType
*/
ZFUNC ZUINT Httpc_CfgGetTlsVeryType(ZFUNC_VOID);

/**
 * @brief Get TLS trusted ca file.
 *
 * @retval Trusted ca file name.
 *
 * @see Httpc_CfgSetTlsTrustCertFile
 */
ZFUNC ZCHAR * Httpc_CfgGetTlsTrustCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS trusted ca path.
 *
 * @retval Trusted ca path.
 *
 * @see Httpc_CfgSetTlsTrustCaPath
 */
ZFUNC ZCHAR * Httpc_CfgGetTlsTrustCaPath(ZFUNC_VOID);

/**
 * @brief Get TLS certification file.
 *
 * @retval Certification file name.
 *
 * @see Httpc_CfgSetTlsCertFile
 */
ZFUNC ZCHAR * Httpc_CfgGetTlsCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS private key file.
 *
 * @retval Private key file name.
 *
 * @see Httpc_CfgSetTlsPrvKeyFile
 */
ZFUNC ZCHAR * Httpc_CfgGetTlsPrvKeyFile(ZFUNC_VOID);

/**
 * @brief Get TLS private key password.
 *
 * @retval Private key password.
 *
 * @see Httpc_CfgSetTlsPrvKeyPass
 */
ZFUNC ZCHAR * Httpc_CfgGetTlsPrvKeyPass(ZFUNC_VOID);

/* httpc config set task priority */
ZFUNC ZINT Httpc_CfgSetTaskPriority(ZINT iPriority);

/* httpc config set log level */
ZFUNC ZINT Httpc_CfgSetLogLevel(ZUINT iLevel);

/**
 * @brief Set TLS method type.
 *
 * @param [in] dwMethod TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Httpc_CfgGetTlsMethod
 */
ZFUNC ZINT Httpc_CfgSetTlsMethod(ZUINT iMethod);

/**
 * @brief Set TLS verify type.
 *
 * @param [in] iVeryType TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Httpc_CfgGetTlsVeryType
 */
ZFUNC ZINT Httpc_CfgSetTlsVeryType(ZUINT iVeryType);

/**
 * @brief Set TLS trusted ca file.
 *
 * @param [in] pcFile Trusted ca file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Httpc_CfgGetTlsTrustCertFile
 */
ZFUNC ZINT Httpc_CfgSetTlsTrustCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS trusted ca path.
 *
 * @param [in] pcFile Trusted ca path.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Httpc_CfgGetTlsTrustCaPath
 */
ZFUNC ZINT Httpc_CfgSetTlsTrustCaPath(ZCHAR *pcPath);

/**
 * @brief Set TLS certification file.
 *
 * @param [in] pcFile Certification file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Httpc_CfgGetTlsCertFile
 */
ZFUNC ZINT Httpc_CfgSetTlsCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS private key file.
 *
 * @param [in] pcFile Private key file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Httpc_CfgGetTlsPrvKeyFile
 */
ZFUNC ZINT Httpc_CfgSetTlsPrvKeyFile(ZCHAR *pcFile);

/**
 * @brief Set TLS private key password.
 *
 * @param [in] pcPasswd Private key password.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Httpc_CfgGetTlsPrvKeyPass
 */
ZFUNC ZINT Httpc_CfgSetTlsPrvKeyPass(ZCHAR *pcPasswd);

/* httpc set http proxy type */
ZFUNC ZINT Httpc_CfgSetHttpProxyType(ZUINT iProxyType);

/* httpc get http proxy type */
ZFUNC ZUINT Httpc_CfgGetHttpProxyType(ZFUNC_VOID);

/* httpc set http proxy server address */
ZFUNC ZINT Httpc_CfgSetHttpProxySrvAddr(ZCHAR *pcSrvAddr);

/* httpc get http proxy server address  */
ZFUNC ZCHAR * Httpc_CfgGetHttpProxySrvAddr(ZFUNC_VOID);

/* httpc set http proxy server port */
ZFUNC ZINT Httpc_CfgSetHttpProxySrvPort(ZUSHORT wProxyPort);

/* httpc get http proxy server port */
ZFUNC ZUSHORT Httpc_CfgGetHttpProxySrvPort(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _HTTPC_CFG_H__ */

