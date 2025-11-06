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
  File name     : imap_cfg.h
  Module        : imap protocol config
  Author        : tanek.ye
  Version       : 0.1
  Created on    : 2013-03-26
  Description   :
    Marcos and structure definitions required by the imap protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_CFG_H__
#define _IMAP_CFG_H__

/** 
 * @file imap_cfg.h
 * @brief IMAP configuration interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get log level of IMAP stack.
 *
 * @retval Log level of IMAP stack, default is ZLOG_LEVEL_ALL.
 *
 * @see @ref Imap_CfgSetLogLevel
 */
ZFUNC ZUINT Imap_CfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief Get log information print flag of IMAP stack.
 *
 * @retval Log information print flag of IMAP stack, default is ZFALSE.
 *
 * @see @ref Imap_CfgSetLogInfoPrint
 */
ZFUNC ZBOOL Imap_CfgGetLogInfoPrint(ZFUNC_VOID);

/**
 * @brief Get log message(packet) print flag of IMAP stack.
 *
 * @retval Log message(packet) print flag of IMAP stack, default is ZTRUE.
 *
 * @see @ref Imap_CfgSetLogMsgPrint
 */
ZFUNC ZBOOL Imap_CfgGetLogMsgPrint(ZFUNC_VOID);

/**
 * @brief Get task priority of IMAP stack.
 *
 * @retval Task priority of IMAP stack, default is ZTASK_PRIORITY_NORMAL.
 *
 * @see @ref Imap_CfgSetTaskPriority
 */
ZFUNC ZINT Imap_CfgGetTaskPriority(ZFUNC_VOID);

/**
 * @brief Get tcp smart bind(for local listen port) flag of IMAP stack.
 *   If stack bind local port failed, it will using system allocated port to bind.
 *
 * @retval Tcp smart bind flag of IMAP stack, default is ZFALSE.
 *
 * @see @ref Imap_CfgSetTptSmartBind
 */
ZFUNC ZBOOL Imap_CfgGetTptSmartBind(ZFUNC_VOID);

/**
 * @brief Get tcp any ip bind(for ANY local ip) flag of IMAP stack.
 *   If stack bind local ip address failed, it will using system allocated ip to bind.
 *
 * @retval Tcp any ip bind flag of IMAP stack, default is ZFALSE.
 *
 * @see @ref Imap_CfgSetTptAnyIpBind
 */
ZFUNC ZBOOL Imap_CfgGetTptAnyIpBind(ZFUNC_VOID);
                
/**
 * @brief Get the local address of IMAP stack.
 *
 * @retval The local address of IMAP stack, default is IPv4 with ip 0 and port 5000.
 *
 * @see @ref Imap_CfgSetLclIpAddr
 */
ZFUNC ST_ZOS_INET_ADDR * Imap_CfgGetLclIpAddr(ZFUNC_VOID);

/**
 * @brief Get the local port of IMAP stack.
 *
 * @retval The local port of IMAP stack, default is 5000.
 *
 * @see @ref Imap_CfgSetLclIpAddr
 */
ZFUNC ZUSHORT Imap_CfgGetLclIpPort(ZFUNC_VOID);

/**
 * @brief Get the local IPv4 address of IMAP stack.
 *
 * @param [out] piIpv4 The local IPv4 address.
 * @param [out] pwPort The local address port.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see @ref Imap_CfgSetLclIpv4Addr
 */
ZFUNC ZINT Imap_CfgGetLclIpv4Addr(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief Get the local ipv6 address of IMAP stack.
 *
 * @param [out] ppucIpv6 The local IPv6 address.
 * @param [out] pwPort The local address port.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see @ref Imap_CfgSetLclIpv6Addr
 */
ZFUNC ZINT Imap_CfgGetLclIpv6Addr(ZUCHAR **ppucIpv6, 
                ZUSHORT *pwPort);

/**
 * @brief Get TLS client method type.
 *
 * @retval TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @see Imap_CfgSetTlsMethod
 */
ZFUNC ZUINT Imap_CfgGetTlsMethod(ZFUNC_VOID);

/**
 * @brief Get TLS client verify type.
 *
 * @retval TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @see Imap_CfgSetTlsVeryType
 */
ZFUNC ZUINT Imap_CfgGetTlsVeryType(ZFUNC_VOID);

/**
 * @brief Get TLS client trusted ca file.
 *
 * @retval Trusted ca file name.
 *
 * @see Imap_CfgSetTlsTrustCaFile
 */
ZFUNC ZCHAR * Imap_CfgGetTlsTrustCaFile(ZFUNC_VOID);

/**
 * @brief Get TLS client trusted ca path.
 *
 * @retval Trusted ca file name.
 *
 * @see Imap_CfgSetTlsTrustCaPath
 */
ZFUNC ZCHAR * Imap_CfgGetTlsTrustCaPath(ZFUNC_VOID);

/**
 * @brief Get TLS client certification file.
 *
 * @retval Certification file name.
 *
 * @see Imap_CfgSetTlsCertFile
 */
ZFUNC ZCHAR * Imap_CfgGetTlsCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS client private key file.
 *
 * @retval Private key file name.
 *
 * @see Imap_CfgSetTlsPrvKeyFile
 */
ZFUNC ZCHAR * Imap_CfgGetTlsPrvKeyFile(ZFUNC_VOID);

/**
 * @brief Get TLS client private key password.
 *
 * @retval Private key password.
 *
 * @see Imap_CfgSetTlsPrvKeyPwd
 */
ZFUNC ZCHAR * Imap_CfgGetTlsPrvKeyPwd(ZFUNC_VOID);

/**
 * @brief Get cache directory.
 *
 * @retval Cache directory.
 *
 * @see Imap_CfgSetCacheDir
 */
ZFUNC ZCHAR * Imap_CfgGetCacheDir(ZFUNC_VOID);

/**
 * @brief Set log level in IMAP stack.
 *
 * @param [in] iLevel Log level.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetLogLevel
 */
ZFUNC ZINT Imap_CfgSetLogLevel(ZUINT iLevel);

/**
 * @brief Set log information level print flag of IMAP stack.
 *
 * @param [in] bPrint The log information level flag.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetLogInfoPrint
 */
ZFUNC ZINT Imap_CfgSetLogInfoPrint(ZBOOL bPrint);

/**
 * @brief Set log message(packet) print flag of IMAP stack.
 *
 * @param [in] bEnabled The log message flag.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetLogMsgPrint
 */
ZFUNC ZINT Imap_CfgSetLogMsgPrint(ZBOOL bEnabled);

/**
 * @brief Set task priority of IMAP stack.
 *
 * @param [in] iPriority The task priority of SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetTaskPriority
 */
ZFUNC ZINT Imap_CfgSetTaskPriority(ZINT iPriority);

/**
 * @brief Set tcp smart bind(for local listen port) flag of IMAP stack.
 *   If stack bind local port failed, it will using system allocated port to bind.
 *
 * @param [in] bEnabled The enable flag of tcp smart bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetTptSmartBind
 */
ZFUNC ZINT Imap_CfgSetTptSmartBind(ZBOOL bEnabled);

/**
 * @brief Set tcp any ip bind(for ANY local ip) flag of IMAP stack.
 *   If stack bind local ip address failed, it will using system allocated ip to bind.
 *
 * @param [in] bEnabled The enable flag of tcp any ip bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetTptAnyIpBind
 */
ZFUNC ZINT Imap_CfgSetTptAnyIpBind(ZBOOL bEnabled);

/**
 * @brief Set the local address of IMAP stack.
 *
 * @param [in] pstAddr The local address for tcp bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetLclIpAddr
 */
ZFUNC ZINT Imap_CfgSetLclIpAddr(ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Set the local IPv4 address of IMAP stack.
 *
 * @param [out] iIpv4 The local IPv4 address.
 * @param [out] wPort The local address port.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetLclIpv4Addr
 */
ZFUNC ZINT Imap_CfgSetLclIpv4Addr(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief Set the local IPv6 address of IMAP stack.
 *
 * @param [out] pucIpv6 The local IPv6 address.
 * @param [out] wPort The local address port.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Imap_CfgGetLclIpv6Addr
 */
ZFUNC ZINT Imap_CfgSetLclIpv6Addr(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief Set TLS client method type.
 *
 * @param [in] iMethod TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetTlsMethod
 */
ZFUNC ZINT Imap_CfgSetTlsMethod(ZUINT iMethod);

/**
 * @brief Set TLS client verify type.
 *
 * @param [in] iVeryType TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetTlsVeryType
 */
ZFUNC ZINT Imap_CfgSetTlsVeryType(ZUINT iVeryType);

/**
 * @brief Set TLS client trusted ca file.
 *
 * @param [in] pcTrustCaFile Trusted ca file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetTlsTrustCaFile
 */
ZFUNC ZINT Imap_CfgSetTlsTrustCaFile(ZCHAR *pcTrustCaFile);

/**
 * @brief Set TLS client trusted ca path.
 *
 * @param [in] pcTrustCaFile Trusted ca path.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetTlsTrustCaPath
 */
ZFUNC ZINT Imap_CfgSetTlsTrustCaPath(ZCHAR *pcTrustCaPath);

/**
 * @brief Set TLS client certification file.
 *
 * @param [in] pcCertFile Certification file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetTlsCertFile
 */
ZFUNC ZINT Imap_CfgSetTlsCertFile(ZCHAR *pcCertFile);

/**
 * @brief Set TLS client private key file.
 *
 * @param [in] pcPrvKeyFile Private key file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetTlsPrvKeyFile
 */
ZFUNC ZINT Imap_CfgSetTlsPrvKeyFile(ZCHAR *pcPrvKeyFile);

/**
 * @brief Set TLS client private key password.
 *
 * @param [in] pcPasswd Private key password.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetTlsPrvKeyPwd
 */
ZFUNC ZINT Imap_CfgSetTlsPrvKeyPwd(ZCHAR *pcPasswd);

/**
 * @brief Set cache directory.
 *
 * @param [in] pcCacheDir Cache directory.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Imap_CfgGetCacheDir
 */
ZFUNC ZINT Imap_CfgSetCacheDir(ZCHAR *pcCacheDir);

#ifdef __cplusplus
}
#endif

#endif /* _IMAP_CFG_H__ */

