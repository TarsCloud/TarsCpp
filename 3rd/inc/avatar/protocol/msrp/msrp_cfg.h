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
  File name     : msrp_cfg.h
  Module        : msrp protocol config
  Author        : cathy.chen
  Version       : 0.1
  Created on    : 2010-10-18
  Description   :
    Marcos and structure definitions required by the msrp protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_CFG_H__
#define _MSRP_CFG_H__

/** 
 * @file msrp_cfg.h
 * @brief MSRP configuration interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* MSRP keep alive type */
typedef enum EN_MSRP_KEEP_ALIVE_TYPE
{
    EN_MSRP_KEEP_ALIVE_DISABLE,      /* disable keep alive */
    EN_MSRP_KEEP_ALIVE_CRLN,         /* use "\r\n\r\n" */
    EN_MSRP_KEEP_ALIVE_MSG,          /* use bodiless message */
} EN_MSRP_KEEP_ALIVE_TYPE;

/* MSRP register server type */
typedef enum EN_MSRP_REG_SRV_TYPE
{
    EN_MSRP_REG_SRV_VOIP,             /* voip service */
    EN_MSRP_REG_SRV_MMTEL,            /* 3GPP IMS service */
    EN_MSRP_REG_SRV_VOLTE,            /* 3GPP VoLTE service */
    EN_MSRP_REG_SRV_RCS5,             /* GSMA RCS5 service */
    EN_MSRP_REG_SRV_JOYN_HF,          /* GSMA joyn hotfixes service */
    EN_MSRP_REG_SRV_JOYN_BB,          /* GSMA joyn blackbird service */
    EN_MSRP_REG_SRV_CMCC_RCS,         /* CMCC RCS service */
} EN_MSRP_REG_SRV_TYPE;

/**
 * @brief Get log level of MSRP stack.
 *
 * @retval Log level of MSRP stack, default is ZLOG_LEVEL_ALL.
 *
 * @see @ref Msrp_CfgSetLogLevel
 */
ZFUNC ZUINT Msrp_CfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief Get log information print flag of MSRP stack.
 *
 * @retval Log information print flag of MSRP stack, default is ZFALSE.
 *
 * @see @ref Msrp_CfgSetLogInfoPrint
 */
ZFUNC ZBOOL Msrp_CfgGetLogInfoPrint(ZFUNC_VOID);

/**
 * @brief Get file log information print flag of MSRP stack.
 *
 * @retval file log information print flag of MSRP stack, default is ZFALSE.
 *
 * @see @ref Msrp_CfgSetLogFileInfoPrint
 */
ZFUNC ZBOOL Msrp_CfgGetLogFileInfoPrint(ZFUNC_VOID);

/**
 * @brief Get log message(packet) print flag of MSRP stack.
 *
 * @retval Log message(packet) print flag of MSRP stack, default is ZTRUE.
 *
 * @see @ref Msrp_CfgSetLogMsgPrint
 */
ZFUNC ZBOOL Msrp_CfgGetLogMsgPrint(ZFUNC_VOID);

/**
 * @brief Get Msrp socket type.
 *
 * @retval Msrp socket type.
 *
 * @see @ref Msrp_CfgSetSockType
 */
ZFUNC ZINT Msrp_CfgGetSockType(ZFUNC_VOID);

/**
 * @brief Get task priority of MSRP stack.
 *
 * @retval Task priority of MSRP stack, default is ZTASK_PRIORITY_NORMAL.
 *
 * @see @ref Msrp_CfgSetTaskPriority
 */
ZFUNC ZINT Msrp_CfgGetTaskPriority(ZFUNC_VOID);

/**
 * @brief Get keep alive type.
 *
 * @retval The type of keep alive EN_MSRP_KEEP_ALIVE_MSG.
 *
 * @see @ref Msrp_CfgSetKeepAliveType
 */
ZFUNC ZUINT Msrp_CfgGetKeepAliveType(ZFUNC_VOID);

/**
 * @brief Get keep alive interval time length in seconds.
 *
 * @retval The time in seconds of keep alive interval.
 *
 * @see @ref Msrp_CfgSetKeepAliveInterval
 */
ZFUNC ZUINT Msrp_CfgGetKeepAliveIntval(ZFUNC_VOID);

/**
 * @brief Get tcp connect times in one request of MSRP stack.
 *
 * @retval Tcp connect times of MSRP stack, default is 5.
 *
 * @see @ref Msrp_CfgSetTcpConnTimes
 */
ZFUNC ZUINT Msrp_CfgGetTcpConnTimes(ZFUNC_VOID);

/**
 * @brief Get tcp smart bind(for local listen port) flag of MSRP stack.
 *   If stack bind local port failed, it will using system allocated port to bind.
 *
 * @retval Tcp smart bind flag of MSRP stack, default is ZFALSE.
 *
 * @see @ref Msrp_CfgSetTptSmartBind
 */
ZFUNC ZBOOL Msrp_CfgGetTptSmartBind(ZFUNC_VOID);

/**
 * @brief Get tcp any ip bind(for ANY local ip) flag of MSRP stack.
 *   If stack bind local ip address failed, it will using system allocated ip to bind.
 *
 * @retval Tcp any ip bind flag of MSRP stack, default is ZFALSE.
 *
 * @see @ref Msrp_CfgSetTptAnyIpBind
 */
ZFUNC ZBOOL Msrp_CfgGetTptAnyIpBind(ZFUNC_VOID);

/**
 * @brief Get send bodiless before valid data flag of MSRP stack.
 *
 * @retval Send bodiless flag of MSRP stack, default is ZTRUE.
 *
 * @see @ref Msrp_CfgSetSendBodiless
 */
ZFUNC ZBOOL Msrp_CfgGetSendBodiless(ZFUNC_VOID);

/**
 * @brief Get MSRP transport trunk size.
 *
 * @retval Tpt trunk size of MSRP stack, default is 10240 bytes.
 *
 * @see @ref Msrp_CfgSetTptTrunkSize
 */
ZFUNC ZUINT Msrp_CfgGetTptTrunkSize(ZFUNC_VOID);

/**
 * @brief Get the tcp local port of MSRP stack.
 *
 * @retval The local port of MSRP stack, default is 5000.
 *
 * @see @ref Msrp_CfgSetTcpLclIpAddr
 */
ZFUNC ZUSHORT Msrp_CfgGetTcpLclIpPort(ZFUNC_VOID);
                
/**
 * @brief Get the tcp local address of MSRP stack.
 *
 * @retval The local address of MSRP stack, default is IPv4 with ip 0 and port 5000.
 *
 * @see @ref Msrp_CfgSetTcpLclIpAddr
 */
ZFUNC ST_ZOS_INET_ADDR * Msrp_CfgGetTcpLclIpAddr(ZFUNC_VOID);

/**
 * @brief Get the tcp local IPv4 address of MSRP stack.
 *
 * @param [out] piIpv4 The local IPv4 address.
 * @param [out] pwPort The local address port.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see @ref Msrp_CfgSetTcpLclIpv4Addr
 */
ZFUNC ZINT Msrp_CfgGetTcpLclIpv4Addr(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief Get the tcp local ipv6 address of MSRP stack.
 *
 * @param [out] ppucIpv6 The local IPv6 address.
 * @param [out] pwPort The local address port.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see @ref Msrp_CfgSetTcpLclIpv6Addr
 */
ZFUNC ZINT Msrp_CfgGetTcpLclIpv6Addr(ZUCHAR **ppucIpv6, 
                ZUSHORT *pwPort);

/**
 * @brief Get the tls local address of MSRP stack.
 *
 * @retval The local address of MSRP stack, default is IPv4 with ip 0 and port 5001.
 *
 * @see @ref Msrp_CfgSetTlsLclIpAddr
 */
ZFUNC ST_ZOS_INET_ADDR * Msrp_CfgGetTlsLclIpAddr(ZFUNC_VOID);

/**
 * @brief Get the tls local IPv4 address of MSRP stack.
 *
 * @param [out] piIpv4 The local IPv4 address.
 * @param [out] pwPort The local address port.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see @ref Msrp_CfgSetTlsLclIpv4Addr
 */
ZFUNC ZINT Msrp_CfgGetTlsLclIpv4Addr(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief Get the tls local ipv6 address of MSRP stack.
 *
 * @param [out] ppucIpv6 The local IPv6 address.
 * @param [out] pwPort The local address port.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see @ref Msrp_CfgSetTlsLclIpv6Addr
 */
ZFUNC ZINT Msrp_CfgGetTlsLclIpv6Addr(ZUCHAR **ppucIpv6, 
                ZUSHORT *pwPort);

/**
 * @brief Get TLS client method type.
 *
 * @retval TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @see Msrp_CfgSetTlsCliMethod
 */
ZFUNC ZUINT Msrp_CfgGetTlsCliMethod(ZFUNC_VOID);

/**
 * @brief Get TLS client verify type.
 *
 * @retval TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @see Msrp_CfgSetTlsCliVeryType
 */
ZFUNC ZUINT Msrp_CfgGetTlsCliVeryType(ZFUNC_VOID);

/**
 * @brief Get TLS client trusted ca file.
 *
 * @retval Trusted ca file name.
 *
 * @see Msrp_CfgSetTlsCliTrustCertFile
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsCliTrustCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS client trusted ca path.
 *
 * @retval Trusted ca path.
 *
 * @see Msrp_CfgSetTlsCliTrustCaPath
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsCliTrustCaPath(ZFUNC_VOID);

/**
 * @brief Get TLS client certification file.
 *
 * @retval Certification file name.
 *
 * @see Msrp_CfgSetTlsCliCertFile
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsCliCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS client private key file.
 *
 * @retval Private key file name.
 *
 * @see Msrp_CfgSetTlsCliPrvKeyFile
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsCliPrvKeyFile(ZFUNC_VOID);

/**
 * @brief Get TLS client private key password.
 *
 * @retval Private key password.
 *
 * @see Msrp_CfgSetTlsCliPrvKeyPass
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsCliPrvKeyPass(ZFUNC_VOID);

/**
 * @brief Get TLS server method type.
 *
 * @retval TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @see Msrp_CfgSetTlsServMethod
 */
ZFUNC ZUINT Msrp_CfgGetTlsServMethod(ZFUNC_VOID);

/**
 * @brief Get TLS server verify type.
 *
 * @retval TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @see Msrp_CfgSetTlsServVeryType
 */
ZFUNC ZUINT Msrp_CfgGetTlsServVeryType(ZFUNC_VOID);

/**
 * @brief Get TLS server trusted ca file.
 *
 * @retval Trusted ca file name.
 *
 * @see Msrp_CfgSetTlsServTrustCertFile
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsServTrustCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS server certification file.
 *
 * @retval Certification file name.
 *
 * @see Msrp_CfgSetTlsServCertFile
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsServCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS server private key file.
 *
 * @retval Private key file name.
 *
 * @see Msrp_CfgSetTlsServPrvKeyFile
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsServPrvKeyFile(ZFUNC_VOID);

/**
 * @brief Get TLS server private key password.
 *
 * @retval Private key password.
 *
 * @see Msrp_CfgSetTlsServPrvKeyPass
 */
ZFUNC ZCHAR * Msrp_CfgGetTlsServPrvKeyPass(ZFUNC_VOID);

/**
 * @brief Get register service type.
 *
 * @return Get register service type. Service type @ref EN_MSRP_REG_SRV_TYPE. 
 *
 * @see @ref Msrp_CfgSetRegSrvType
 */
ZFUNC ZUINT Msrp_CfgGetRegSrvType(ZFUNC_VOID);

/**
 * @brief Set log level in MSRP stack.
 *
 * @param [in] dwLevel Log level.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetLogLevel
 */
ZFUNC ZINT Msrp_CfgSetLogLevel(ZUINT iLevel);

/**
 * @brief Set log information level print flag of MSRP stack.
 *
 * @param [in] bPrint The log information level flag.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetLogInfoPrint
 */
ZFUNC ZINT Msrp_CfgSetLogInfoPrint(ZBOOL bPrint);

/**
 * @brief Set file log information level print flag of MSRP stack.
 *
 * @param [in] bPrint The file log information level flag.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetLogFileInfoPrint
 */
ZFUNC ZINT Msrp_CfgSetLogFileInfoPrint(ZBOOL bPrint);

/**
 * @brief Set log message(packet) print flag of MSRP stack.
 *
 * @param [in] bEnabled The log message flag.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetLogMsgPrint
 */
ZFUNC ZINT Msrp_CfgSetLogMsgPrint(ZBOOL bEnabled);

/**
 * @brief Set Msrp socket type.
 *
 * @param [in] iSockType The Msrp socket type.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetSockType
 */
ZFUNC ZINT Msrp_CfgSetSockType(ZINT iSockType);

/**
 * @brief Set task priority of MSRP stack.
 *
 * @param [in] iPriority The task priority of SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTaskPriority
 */
ZFUNC ZINT Msrp_CfgSetTaskPriority(ZINT iPriority);

/**
 * @brief Set keep alive type.
 *
 * @param [in] iType The keep alive type @ref EN_MSRP_KEEP_ALIVE_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetKeepAliveType
 */
ZFUNC ZINT Msrp_CfgSetKeepAliveType(ZUINT iType);

/**
 * @brief Set keep alive interval time length.
 *
 * @param [in] iIntval The time length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetKeepAliveIntval
 */
ZFUNC ZINT Msrp_CfgSetKeepAliveIntval(ZUINT iIntval);

/**
 * @brief Set tcp connect times in one request of MSRP stack.
 *
 * @param [in] dwTimes The tcp connect times.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgSetTcpConnTimes
 */
ZFUNC ZINT Msrp_CfgSetTcpConnTimes(ZUINT iTimes);

/**
 * @brief Set tcp smart bind(for local listen port) flag of MSRP stack.
 *   If stack bind local port failed, it will using system allocated port to bind.
 *
 * @param [in] bEnabled The enable flag of tcp smart bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTptSmartBind
 */
ZFUNC ZINT Msrp_CfgSetTptSmartBind(ZBOOL bEnabled);

/**
 * @brief Set tcp any ip bind(for ANY local ip) flag of MSRP stack.
 *   If stack bind local ip address failed, it will using system allocated ip to bind.
 *
 * @param [in] bEnabled The enable flag of tcp any ip bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTptAnyIpBind
 */
ZFUNC ZINT Msrp_CfgSetTptAnyIpBind(ZBOOL bEnabled);

/**
 * @brief Set send bodiless before valid data flag of MSRP stack.
 *
 * @param [in] bSendBodiless The enable flag of send bodiless.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetSendBodiless
 */
ZFUNC ZINT Msrp_CfgSetSendBodiless(ZBOOL bSendBodiless);

/**
 * @brief Set transport trunk size in one request of MSRP stack.
 *
 * @param [in] iSize The tpt trunk size.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTptTrunkSize
 */
ZFUNC ZINT Msrp_CfgSetTptTrunkSize(ZUINT iSize);

/**
 * @brief Set the tcp local address of MSRP stack.
 *
 * @param [in] pstAddr The local address for tcp bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTcpLclIpAddr
 */
ZFUNC ZINT Msrp_CfgSetTcpLclIpAddr(ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Set the tcp local IPv4 address of MSRP stack.
 *
 * @param [out] iIpv4 The local IPv4 address.
 * @param [out] wPort The local address port.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTcpLclIpv4Addr
 */
ZFUNC ZINT Msrp_CfgSetTcpLclIpv4Addr(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief Set the tcp local IPv6 address of MSRP stack.
 *
 * @param [out] pucIpv6 The local IPv6 address.
 * @param [out] wPort The local address port.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTcpLclIpv6Addr
 */
ZFUNC ZINT Msrp_CfgSetTcpLclIpv6Addr(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief Set the tls local address of MSRP stack.
 *
 * @param [in] pstAddr The local address for tls bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTlsLclIpAddr
 */
ZFUNC ZINT Msrp_CfgSetTlsLclIpAddr(ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Set the tls local IPv4 address of MSRP stack.
 *
 * @param [out] iIpv4 The local IPv4 address.
 * @param [out] wPort The local address port.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTlsLclIpv4Addr
 */
ZFUNC ZINT Msrp_CfgSetTlsLclIpv4Addr(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief Set the tls local IPv6 address of MSRP stack.
 *
 * @param [out] pucIpv6 The local IPv6 address.
 * @param [out] wPort The local address port.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_CfgGetTlsLclIpv6Addr
 */
ZFUNC ZINT Msrp_CfgSetTlsLclIpv6Addr(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief Set TLS client method type.
 *
 * @param [in] dwMethod TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsCliMethod
 */
ZFUNC ZINT Msrp_CfgSetTlsCliMethod(ZUINT iMethod);

/**
 * @brief Set TLS client verify type.
 *
 * @param [in] iVeryType TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsCliVeryType
 */
ZFUNC ZINT Msrp_CfgSetTlsCliVeryType(ZUINT iVeryType);

/**
 * @brief Set TLS client trusted ca file.
 *
 * @param [in] pcFile Trusted ca file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsCliTrustCertFile
 */
ZFUNC ZINT Msrp_CfgSetTlsCliTrustCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS client trusted ca path.
 *
 * @param [in] pcFile Trusted ca path.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsCliTrustCaPath
 */
ZFUNC ZINT Msrp_CfgSetTlsCliTrustCaPath(ZCHAR *pcPath);

/**
 * @brief Set TLS client certification file.
 *
 * @param [in] pcFile Certification file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsCliCertFile
 */
ZFUNC ZINT Msrp_CfgSetTlsCliCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS client private key file.
 *
 * @param [in] pcFile Private key file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsCliPrvKeyFile
 */
ZFUNC ZINT Msrp_CfgSetTlsCliPrvKeyFile(ZCHAR *pcFile);

/**
 * @brief Set TLS client private key password.
 *
 * @param [in] pcPasswd Private key password.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsCliPrvKeyPass
 */
ZFUNC ZINT Msrp_CfgSetTlsCliPrvKeyPass(ZCHAR *pcPasswd);

/**
 * @brief Set TLS server method type.
 *
 * @param [in] dwMethod TLS method type, ref EN_TLS_METHOD_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsServMethod
 */
ZFUNC ZINT Msrp_CfgSetTlsServMethod(ZUINT iMethod);

/**
 * @brief Set TLS server verify type.
 *
 * @param [in] iVeryType TLS verify type, ref EN_TLS_VERY_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsServVeryType
 */
ZFUNC ZINT Msrp_CfgSetTlsServVeryType(ZUINT iVeryType);

/**
 * @brief Set TLS server trusted ca file.
 *
 * @param [in] pcFile Trusted ca file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsServTrustCertFile
 */
ZFUNC ZINT Msrp_CfgSetTlsServTrustCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS server certification file.
 *
 * @param [in] pcFile Certification file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsServCertFile
 */
ZFUNC ZINT Msrp_CfgSetTlsServCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS server private key file.
 *
 * @param [in] pcFile Private key file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsServPrvKeyFile
 */
ZFUNC ZINT Msrp_CfgSetTlsServPrvKeyFile(ZCHAR *pcFile);

/**
 * @brief Set TLS server private key password.
 *
 * @param [in] pcPasswd Private key password.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Msrp_CfgGetTlsServPrvKeyPass
 */
ZFUNC ZINT Msrp_CfgSetTlsServPrvKeyPass(ZCHAR *pcPasswd);

/**
 * @brief Set register service type.
 *
 * @param [in] iType The register service type. 
 *             Service type @ref EN_MSRP_REG_SRV_TYPE. 
 *
 * @retval ZOK Set register service type successfully.
 * @retval ZFAILED Set register service type failed.
 *
 * @see @ref Msrp_CfgGetRegSrvType
 */
ZFUNC ZINT Msrp_CfgSetRegSrvType(ZUINT iType);

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_CFG_H__ */


