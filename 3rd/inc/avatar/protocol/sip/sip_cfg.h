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
  File name     : sip_cfg.h
  Module        : sip protocol config
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_CFG_H__
#define _SIP_CFG_H__

/** 
 * @file
 * @brief SIP configuration interfaces.
 *
 * SIP stack user must identify following items:
 * -# Using Zos message to inform SIP stack event to its user. 
 *    If SIP stack user was built on Zos environment, we recommand using this
 *    method. Using @ref Sip_CfgSetUpperTaskId to set user's task ID.
 * -# Or, using callback function to inform user. Using 
 *    @ref Sip_CfgSetNtfySessEvnt to set callback function.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Default SIP UDP server port */
#define SIP_DFT_TPT_UDP_PORT 5060

/** @brief Default SIP TCP server port */
#define SIP_DFT_TPT_TCP_PORT 5060

/** @brief Default SIP TLS server port */
#define SIP_DFT_TPT_TLS_PORT 5061

/** @brief Type of callback function to notify SIP stack session event to user. */
typedef ZINT (*PFN_SIPNTFYSESSEVNT)(ST_SIP_SESS_EVNT *pstEvnt);

/** @brief Type of callback function to notify invite dialog state change */
typedef ZINT (*PFN_SIPNTFYDLGSTATE)(ZUINT iDlgId, ZINT iOldState, 
                    ZINT iNewState);

/** @brief Type of callback function to notify subscription dialog event, 
 * reason see EN_SIP_EVNT_REASON_VAL */
typedef ZINT (*PFN_SIPNTFYDLGEVNT)(ZUINT iSubsId, ZUCHAR ucReason);

/**
 * @brief Get task priority, default is ZTASK_PRIORITY_NORMAL.
 *
 * @retval Task priority of SIP stack.
 *
 * @see Sip_CfgSetTaskPriority
 */
ZFUNC ZINT Sip_CfgGetTaskPriority(ZFUNC_VOID);

/**
 * @brief Get task ID of SIP stack user.
 *
 * @retval Task ID of SIP stack user.
 *
 * @see Sip_CfgSetUpperTaskId
 */
ZFUNC ZTASKID Sip_CfgGetUpperTaskId(ZFUNC_VOID);

/**
 * @brief Get SIP Callback function be used when notify SIP stack
 *        session event to user.
 *
 * @retval Callback function of notify session event to user.
 *
 * @see Sip_CfgSetNtfySessEvnt
 */
ZFUNC PFN_SIPNTFYSESSEVNT Sip_CfgGetNtfySessEvnt(ZFUNC_VOID);

/**
 * @brief Get SIP Callback function be used when notify SIP 
 * invite dialog state change.
 *
 * @retval Callback function of notify invite dialog state change to user.
 *
 * @see Sip_CfgSetNtfyDlgState
 */
ZFUNC PFN_SIPNTFYDLGSTATE Sip_CfgGetNtfyDlgState(ZFUNC_VOID);

/**
 * @brief Get SIP Callback function be used when notify SIP 
 * subscription dialog event.
 *
 * @retval Callback function of notify subscription dialog event to user.
 *
 * @see Sip_CfgSetNtfyDlgEvnt
 */
ZFUNC PFN_SIPNTFYDLGEVNT Sip_CfgGetNtfyDlgEvnt(ZFUNC_VOID);

/**
 * @brief Get max forwards value in SIP stack.
 *
 * @retval Max forwards value in SIP stack, default is 70.
 *
 * @see Sip_CfgSetMaxForwards
 */
ZFUNC ZUINT Sip_CfgGetMaxForwards(ZFUNC_VOID);

/**
 * @brief Get MTU value in SIP stack.
 *
 * @retval MTU value in SIP stack, default is 1452.
 *
 * @see Sip_CfgSetMtuSize
 */
ZFUNC ZUINT Sip_CfgGetMtuSize(ZFUNC_VOID);

/**
 * @brief Get if response 100 trying automatically.
 *
 * @retval ZTRUE Response 100 trying automatically.
 * @retval ZFALSE Not to response 100 trying automatically.
 *
 * @see Sip_CfgSetAutoRsp100
 */
ZFUNC ZBOOL Sip_CfgGetAutoRsp100(ZFUNC_VOID);

/**
 * @brief Get product value string in SIP stack".
 *
 * @retval Product value string in SIP stack, 
 * default is "SIP/1.0 (Juphoon System)".
 *
 * @see Sip_CfgSetProductVal
 */
ZFUNC ZCHAR * Sip_CfgGetProductVal(ZFUNC_VOID);

/**
 * @brief Get log level in SIP stack, default is ZLOG_LEVEL_ALL.
 *
 * @retval Log level in SIP stack, default is ZLOG_LEVEL_ALL.
 *
 * @see Sip_CfgSetLogLevel
 */
ZFUNC ZUINT Sip_CfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief Get trasnport log level in SIP stack, default is ZLOG_LEVEL_ALL.
 *
 * @retval Log level in SIP stack, default is ZLOG_LEVEL_ALL.
 *
 * @see Sip_CfgSetTptLogLevel
 */
ZFUNC ZUINT Sip_CfgGetTptLogLevel(ZFUNC_VOID);

/**
 * @brief Get flag of using IPv6 in SIP stack.
 *
 * @retval Using IPv6 in SIP stack, default is ZFALSE.
 *
 * @see Sip_CfgSetIsIpv6Tpt
 */
ZFUNC ZBOOL Sip_CfgGetIsIpv6Tpt(ZFUNC_VOID);

/**
 * @brief Get UDP listen address of IPv4.
 *
 * @param [out] piIpv4 IPv4 of listen address.
 * @param [out] pwPort Port of listen address.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see Sip_CfgSetUServIpv4Addr
 */
ZFUNC ZINT Sip_CfgGetUServIpv4Addr(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief Get UDP listen address of IPv6.
 *
 * @param [out] ppucIpv6 IPv6 of listen address.
 * @param [out] pwPort Port of listen address.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see Sip_CfgSetUServIpv6Addr
 */
ZFUNC ZINT Sip_CfgGetUServIpv6Addr(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/**
 * @brief Get UDP listen address.
 *
 * @retval UDP server listen address.
 *
 * @see Sip_CfgSetUServAddr
 */
ZFUNC ST_ZOS_INET_ADDR * Sip_CfgGetUServAddr(ZFUNC_VOID);

/**
 * @brief Get TCP listen address of IPv4.
 *
 * @param [out] piIpv4 IPv4 of listen address.
 * @param [out] pwPort Port of listen address.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see Sip_CfgSetTServIpv4Addr
 */
ZFUNC ZINT Sip_CfgGetTServIpv4Addr(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief Get TCP listen address of IPv6.
 *
 * @param [out] ppucIpv6 IPv6 of listen address.
 * @param [out] pwPort Port of listen address.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see Sip_CfgSetTServIpv6Addr
 */
ZFUNC ZINT Sip_CfgGetTServIpv6Addr(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/**
 * @brief Get TCP listen address.
 *
 * @retval TCP server listen address.
 *
 * @see Sip_CfgSetTServAddr
 */
ZFUNC ST_ZOS_INET_ADDR * Sip_CfgGetTServAddr(ZFUNC_VOID);

/**
 * @brief Get wait time length for TCP connect, default is 2000ms.
 *
 * @retval Wait time length for TCP connect.
 *
 * @see Sip_CfgSetTcpConnTimes
 */
ZFUNC ZUINT Sip_CfgGetTcpConnTimes(ZFUNC_VOID);

/**
 * @brief Get TLS listen address of IPv4.
 *
 * @param [out] piIpv4 IPv4 of listen address.
 * @param [out] pwPort Port of listen address.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see Sip_CfgSetTlsServIpv4Addr
 */
ZFUNC ZINT Sip_CfgGetTlsServIpv4Addr(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief Get TLS listen address of IPv6.
 *
 * @param [out] ppucIpv6 IPv6 of listen address.
 * @param [out] pwPort Port of listen address.
 *
 * @retval ZOK Get configuration successfully.
 * @retval ZFAILED Get configuration failed.
 *
 * @see Sip_CfgSetTlsServIpv6Addr
 */
ZFUNC ZINT Sip_CfgGetTlsServIpv6Addr(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/**
 * @brief Get TLS listen address.
 *
 * @retval TLS server listen address.
 *
 * @see Sip_CfgSetTlsServAddr
 */
ZFUNC ST_ZOS_INET_ADDR * Sip_CfgGetTlsServAddr(ZFUNC_VOID);

/**
 * @brief Get ToS configuration.
 *
 * @retval The Dscp Value
 *
 * @see Sip_CfgSetIpTosVal
 */
ZFUNC ZUINT Sip_CfgGetDscpVal(ZFUNC_VOID);

/**
 * @brief Get T1 timer length.
 *
 * @retval Timer length in milliseconds, default is 500ms.
 *
 * @see Sip_CfgSetT1
 */
ZFUNC ZUINT Sip_CfgGetT1(ZFUNC_VOID);

/**
 * @brief Get T2 timer length.
 *
 * @retval Timer length in milliseconds, default is 4000ms.
 *
 * @see Sip_CfgSetT2
 */
ZFUNC ZUINT Sip_CfgGetT2(ZFUNC_VOID);

/**
 * @brief Get T4 timer length.
 *
 * @retval Timer length in milliseconds, default is 5000ms.
 *
 * @see Sip_CfgSetT4
 */
ZFUNC ZUINT Sip_CfgGetT4(ZFUNC_VOID);

/**
 * @brief Get TA timer length.
 *
 * @retval Timer length in milliseconds, default is 500ms.
 *
 * @see Sip_CfgSetTA
 */
ZFUNC ZUINT Sip_CfgGetTA(ZFUNC_VOID);

/**
 * @brief Get TB timer length.
 *
 * @retval Timer length in milliseconds, default is 32000ms.
 *
 * @see Sip_CfgSetTB
 */
ZFUNC ZUINT Sip_CfgGetTB(ZFUNC_VOID);

/**
 * @brief Get TC timer length.
 *
 * @retval Timer length in milliseconds, default is 64000ms.
 *
 * @see Sip_CfgSetTC
 */
ZFUNC ZUINT Sip_CfgGetTC(ZFUNC_VOID);

/**
 * @brief Get TD timer length.
 *
 * @retval Timer length in milliseconds, default is 32000ms.
 *
 * @see Sip_CfgSetTD
 */
ZFUNC ZUINT Sip_CfgGetTD(ZFUNC_VOID);

/**
 * @brief Get TE timer length.
 *
 * @retval Timer length in milliseconds, default is 500ms.
 *
 * @see Sip_CfgSetTE
 */
ZFUNC ZUINT Sip_CfgGetTE(ZFUNC_VOID);

/**
 * @brief Get TF timer length.
 *
 * @retval Timer length in milliseconds, default is 32000ms.
 *
 * @see Sip_CfgSetTF
 */
ZFUNC ZUINT Sip_CfgGetTF(ZFUNC_VOID);

/**
 * @brief Get TG timer length.
 *
 * @retval Timer length in milliseconds, default is 500ms.
 *
 * @see Sip_CfgSetTG
 */
ZFUNC ZUINT Sip_CfgGetTG(ZFUNC_VOID);

/**
 * @brief Get TH timer length.
 *
 * @retval Timer length in milliseconds, default is 32000ms.
 *
 * @see Sip_CfgSetTH
 */
ZFUNC ZUINT Sip_CfgGetTH(ZFUNC_VOID);

/**
 * @brief Get TI timer length.
 *
 * @retval Timer length in milliseconds, default is 5000ms.
 *
 * @see Sip_CfgSetTI
 */
ZFUNC ZUINT Sip_CfgGetTI(ZFUNC_VOID);

/**
 * @brief Get TJ timer length.
 *
 * @retval Timer length in milliseconds, default is 32000ms.
 *
 * @see Sip_CfgSetTJ
 */
ZFUNC ZUINT Sip_CfgGetTJ(ZFUNC_VOID);

/**
 * @brief Get TK timer length.
 *
 * @retval Timer length in milliseconds, default is 5000ms.
 *
 * @see Sip_CfgSetTK
 */
ZFUNC ZUINT Sip_CfgGetTK(ZFUNC_VOID);

/**
 * @brief Get TL timer length.
 *
 * @retval Timer length in milliseconds, default is 500ms.
 *
 * @see Sip_CfgSetTL
 */
ZFUNC ZUINT Sip_CfgGetTL(ZFUNC_VOID);

/**
 * @brief Get wait notify timer length.
 *
 * @retval Timer length in milliseconds, default is 60000ms.
 *
 * @see Sip_CfgSetWaitNtfyTime
 */
ZFUNC ZUINT Sip_CfgGetWaitNtfyTime(ZFUNC_VOID);

/**
 * @brief Get wait tcp connect timer length.
 *
 * @retval Timer length in milliseconds, default is 500ms.
 *
 * @see Sip_CfgSetWaitConnTime
 */
ZFUNC ZUINT Sip_CfgGetWaitConnTime(ZFUNC_VOID);

/**
 * @brief Get if using UDP keep alive heart beat function.
 *
 * @retval ZTRUE Enable UDP keep alive heart beat.
 * @retval ZFALSE Otherwise.
 *
 * @see Sip_CfgGetUdpHeartbeatTime
 */
ZFUNC ZBOOL Sip_CfgGetUdpHeartbeat(ZFUNC_VOID);

/**
 * @brief Get timer length of UDP keep alive heart beat.
 *
 * @retval Timer length in seconds, default is 10s.
 *
 * @see Sip_CfgGetUdpHeartbeat
 */
ZFUNC ZUINT Sip_CfgGetUdpHeartbeatTime(ZFUNC_VOID);

/**
 * @brief Get if using TCP keep alive heart beat function.
 *
 * @retval ZTRUE Enable TCP keep alive heart beat.
 * @retval ZFALSE Otherwise.
 *
 * @see Sip_CfgGetTcpHeartbeatTime
 */
ZFUNC ZBOOL Sip_CfgGetTcpHeartbeat(ZFUNC_VOID);

/**
 * @brief Get timer length of TCP keep alive heart beat.
 *
 * @retval Timer length in seconds, default is 10s.
 *
 * @see Sip_CfgGetTcpHeartbeat
 */
ZFUNC ZUINT Sip_CfgGetTcpHeartbeatTime(ZFUNC_VOID);

/**
 * @brief Get if detecting TCP keep alive heart beat time.
 *
 * @retval ZTRUE Enable detect TCP keep alive heart beat time.
 * @retval ZFALSE Otherwise.
 *
 */
ZFUNC ZBOOL Sip_CfgGetTcpHeartbeatDetect(ZFUNC_VOID);

/**
 * @brief Get min timer length of TCP keep alive heart beat.
 *
 * @retval Min Timer length in seconds, default is 10s.
 *
 * @see Sip_CfgSetTcpMinHeartbeatTime
 */
ZFUNC ZUINT Sip_CfgGetTcpMinHeartbeatTime(ZFUNC_VOID);

/**
 * @brief Get max timer length of TCP keep alive heart beat.
 *
 * @retval Max Timer length in seconds, default is 10s.
 *
 * @see Sip_CfgSetTcpMaxHeartbeatTime
 */
ZFUNC ZUINT Sip_CfgGetTcpMaxHeartbeatTime(ZFUNC_VOID);

/**
 * @brief Get succeed timer length of TCP keep alive heart beat.
 *
 * @retval Succeed Timer length in seconds, default is 10s.
 *
 * @see Sip_CfgSetTcpSuccHeartbeatTime
 */
ZFUNC ZUINT Sip_CfgGetTcpSuccHeartbeatTime(ZFUNC_VOID);

/**
 * @brief Get current timer length of TCP keep alive heart beat.
 *
 * @retval Current Timer length in seconds, default is 10s.
 *
 * @see Sip_CfgSetTcpCurHeartbeatTime
 */
ZFUNC ZUINT Sip_CfgGetTcpCurHeartbeatTime(ZFUNC_VOID);

/**
 * @brief Get step timer length of TCP keep alive heart beat detect.
 *
 * @retval Step Timer length in seconds, default is 10s.
 *
 * @see Sip_CfgSetTcpHeartbeatStepTime
 */
ZFUNC ZUINT Sip_CfgGetTcpHeartbeatStepTime(ZFUNC_VOID);

/**
 * @brief Get tcp heartbeat fail times.
 *
 * @retval Step Tcp heartbeat fail times, default is 0.
 *
 * @see Sip_CfgSetTcpHeartbeatFailTimes
 */
ZFUNC ZUINT Sip_CfgGetTcpHeartbeatFailTimes(ZFUNC_VOID);

/**
 * @brief Get tcp last keep alive timestamp of TCP keep alive heart beat detect.
 *
 * @retval tcp last keep alive timestamp in seconds.
 *
 * @see Sip_CfgSetTcpLastKeepAliveTimestamp
 */
ZFUNC ZTIME_T Sip_CfgGetTcpLastKeepAliveTimestamp(ZFUNC_VOID);

/**
 * @brief Get if using TCP smart bind function.
 *
 * @retval ZTRUE Enable TCP smart bind.
 * @retval ZFALSE Otherwise.
 *
 * @see 
 */
ZFUNC ZBOOL Sip_CfgGetTptSmartBind(ZFUNC_VOID);

/**
 * @brief Get if bind TCP address to "0.0.0.0".
 *
 * @retval ZTRUE Bind TCP address to "0.0.0.0".
 * @retval ZFALSE Bind TCP address to the address setted by 
 *                @ref Sip_CfgSetTServAddr, @ref Sip_CfgSetTServIpv4Addr or 
 *                @ref Sip_CfgSetTServIpv6Addr.
 *
 * @see 
 */
ZFUNC ZBOOL Sip_CfgGetTptAnyIpBind(ZFUNC_VOID);

/**
 * @brief Get Sip socket type.
 *
 * @retval Sip socket type.
 *
 * @see Sip_CfgSetSockType
 */
ZFUNC ZINT Sip_CfgGetSockType(ZFUNC_VOID);

/**
 * @brief Get max count of call.
 *
 * @retval Max count of call, default is 64.
 *
 * @see Sip_CfgSetCallNum
 */
ZFUNC ZUINT Sip_CfgGetCallNum(ZFUNC_VOID);

/**
 * @brief Get max count of session.
 *
 * @retval Max count of session, default is 64.
 *
 * @see Sip_CfgSetSessNum
 */
ZFUNC ZUINT Sip_CfgGetSessNum(ZFUNC_VOID);

/**
 * @brief Get max count of dialog.
 *
 * @retval Max count of dialog, default is 64.
 *
 * @see Sip_CfgSetDlgNum
 */
ZFUNC ZUINT Sip_CfgGetDlgNum(ZFUNC_VOID);

/**
 * @brief Get max count of subscription.
 *
 * @retval Max count of subscription, default is 64.
 *
 * @see Sip_CfgSetSubsdNum
 */
ZFUNC ZUINT Sip_CfgGetSubsdNum(ZFUNC_VOID);

/**
 * @brief Get max count of transaction.
 *
 * @retval Max count of transaction, default is 64.
 *
 * @see Sip_CfgGetTransNum
 */
ZFUNC ZUINT Sip_CfgGetTransNum(ZFUNC_VOID);

/**
 * @brief Get max count of connection.
 *
 * @retval Max count of connection, default is 16.
 *
 * @see Sip_CfgSetConnNum
 */
ZFUNC ZUINT Sip_CfgGetConnNum(ZFUNC_VOID);

/**
 * @brief Get TLS client method type.
 *
 * @retval TLS client method type, ref EN_TLS_METHOD_TYPE.
 *
 * @see Sip_CfgSetTlsCliMethod
 */
ZFUNC ZUINT Sip_CfgGetTlsCliMethod(ZFUNC_VOID);

/**
 * @brief Get TLS client verify type.
 *
 * @retval TLS client verify type, ref EN_TLS_VERY_TYPE.
 *
 * @see Sip_CfgSetTlsCliVeryType
 */
ZFUNC ZUINT Sip_CfgGetTlsCliVeryType(ZFUNC_VOID);

/**
 * @brief Get TLS client trusted ca path.
 *
 * @retval Trusted ca file name.
 *
 * @see Sip_CfgSetTlsCliTrustCaPath
 */
ZFUNC ZCHAR * Sip_CfgGetTlsCliTrustCaPath(ZFUNC_VOID);

/**
 * @brief Get TLS client trusted ca file.
 *
 * @retval Trusted ca file name.
 *
 * @see Sip_CfgSetTlsCliTrustCertFile
 */
ZFUNC ZCHAR * Sip_CfgGetTlsCliTrustCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS client certification file.
 *
 * @retval Certification file name.
 *
 * @see Sip_CfgSetTlsCliCertFile
 */
ZFUNC ZCHAR * Sip_CfgGetTlsCliCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS client private key file.
 *
 * @retval Private key file name.
 *
 * @see Sip_CfgSetTlsCliPrvKeyFile
 */
ZFUNC ZCHAR * Sip_CfgGetTlsCliPrvKeyFile(ZFUNC_VOID);

/**
 * @brief Get TLS client private key password.
 *
 * @retval Private key password.
 *
 * @see Sip_CfgSetTlsCliPrvKeyPass
 */
ZFUNC ZCHAR * Sip_CfgGetTlsCliPrvKeyPass(ZFUNC_VOID);

/**
 * @brief Get TLS server method type.
 *
 * @retval TLS server method type, ref EN_TLS_METHOD_TYPE.
 *
 * @see Sip_CfgSetTlsServMethod
 */
ZFUNC ZUINT Sip_CfgGetTlsServMethod(ZFUNC_VOID);

/**
 * @brief Get TLS server verify type.
 *
 * @retval TLS server verify type, ref EN_TLS_VERY_TYPE.
 *
 * @see Sip_CfgSetTlsServVeryType
 */
ZFUNC ZUINT Sip_CfgGetTlsServVeryType(ZFUNC_VOID);

/**
 * @brief Get TLS server trusted ca file.
 *
 * @retval Trusted ca file name.
 *
 * @see Sip_CfgSetTlsServTrustCertFile
 */
ZFUNC ZCHAR * Sip_CfgGetTlsServTrustCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS server certification file.
 *
 * @retval Certification file name.
 *
 * @see Sip_CfgSetTlsServCertFile
 */
ZFUNC ZCHAR * Sip_CfgGetTlsServCertFile(ZFUNC_VOID);

/**
 * @brief Get TLS server private key file.
 *
 * @retval Private key file name.
 *
 * @see Sip_CfgSetTlsServPrvKeyFile
 */
ZFUNC ZCHAR * Sip_CfgGetTlsServPrvKeyFile(ZFUNC_VOID);

/**
 * @brief Get TLS server private key password.
 *
 * @retval Private key password.
 *
 * @see Sip_CfgSetTlsServPrvKeyPass
 */
ZFUNC ZCHAR * Sip_CfgGetTlsServPrvKeyPass(ZFUNC_VOID);

/**
 * @brief Get support reuse connection.
 *
 * @retval reuse connection.
 */
ZFUNC ZBOOL Sip_CfgGetSuptReUseConn(ZFUNC_VOID);

/**
 * @brief Get random string length.
 *
 * @retval
 *
 * @see Sip_CfgSetRndStrLen
 */
ZFUNC ZUSHORT Sip_CfgGetRndStrLen(ZFUNC_VOID);

/**
 * @brief Get support local login.
 *
 * @retval local login.
 */
ZFUNC ZBOOL Sip_CfgGetSuptLclLogin(ZFUNC_VOID);

/**
 * @brief Set task priority.
 *
 * @param [in] iPriority The task priority of SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTaskPriority
 */
ZFUNC ZINT Sip_CfgSetTaskPriority(ZINT iPriority);

/**
 * @brief Set support zsh command.
 *
 * @param [in] bSuptCmd Support zsh command.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 */
ZFUNC ZINT Sip_CfgSetSuptZshCmd(ZBOOL bSuptCmd);

/**
 * @brief Set task ID of SIP stack user.
 *
 * @param [in] zUpperTaskId The task ID of SIP stack user.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetUpperTaskId
 */
ZFUNC ZINT Sip_CfgSetUpperTaskId(ZTASKID zUpperTaskId);

/**
 * @brief Set SIP Callback function be used when notify SIP stack
 *        session event to user.
 *
 * @param [in] pfnNtfyEvnt Callback function be used when notify SIP stack
 *                         session event to user.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetNtfySessEvnt
 */
ZFUNC ZINT Sip_CfgSetNtfySessEvnt(PFN_SIPNTFYSESSEVNT pfnNtfyEvnt);

/**
 * @brief Set SIP Callback function be used when notify SIP stack
 *        invite dialog state change to user.
 *
 * @param [in] pfnNtfyState Callback of notify invite dialog state change.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetNtfyDlgState
 */
ZFUNC ZINT Sip_CfgSetNtfyDlgState(PFN_SIPNTFYDLGSTATE pfnNtfyState);

/**
 * @brief Set SIP Callback function be used when notify SIP stack
 *        subscription dialog event to user.
 *
 * @param [in] pfnNtfyEvnt Callback of notify subscription dialog event.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetNtfyDlgEvnt
 */
ZFUNC ZINT Sip_CfgSetNtfyDlgEvnt(PFN_SIPNTFYDLGEVNT pfnNtfyEvnt);

/**
 * @brief Set max forwards value in SIP stack.
 *
 * @param [in] iMaxForwards Max forwards value in SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetMaxForwards
 */
ZFUNC ZINT Sip_CfgSetMaxForwards(ZUINT iMaxForwards);

/**
 * @brief Set MTU value in SIP stack.
 *
 * @param [in] iMtuSize MTU value in SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetMtuSize
 */
ZFUNC ZINT Sip_CfgSetMtuSize(ZUINT iMtuSize);

/**
 * @brief Set product value string in SIP stack".
 *
 * @param [in] pcProductVal Product value string in SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetProductVal
 */
ZFUNC ZINT Sip_CfgSetProductVal(ZCHAR *pcProductVal);

/**
 * @brief Set log level in SIP stack.
 *
 * @param [in] iLevel Log level in SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetLogLevel
 */
ZFUNC ZINT Sip_CfgSetLogLevel(ZUINT iLevel);

/**
 * @brief Set log level transport in SIP stack.
 *
 * @param [in] iLevel Log level in SIP stack.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTptLogLevel
 */
ZFUNC ZINT Sip_CfgSetTptLogLevel(ZUINT iLevel);

/**
 * @brief Set UDP listen address of IPv4.
 *
 * @param [in] iIpv4 IPv4 of listen address.
 * @param [in] wPort Port of listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetUServIpv4Addr
 */
ZFUNC ZINT Sip_CfgSetUServIpv4Addr(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief Set UDP listen address of IPv6.
 *
 * @param [in] pucIpv6 IPv6 of listen address.
 * @param [in] wPort Port of listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetUServIpv6Addr
 */
ZFUNC ZINT Sip_CfgSetUServIpv6Addr(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief Set UDP listen address.
 *
 * @param [in] pstAddr UDP listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetUServAddr
 */
ZFUNC ZINT Sip_CfgSetUServAddr(ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Set TCP listen address of IPv4.
 *
 * @param [in] iIpv4 IPv4 of listen address.
 * @param [in] wPort Port of listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTServIpv4Addr
 */
ZFUNC ZINT Sip_CfgSetTServIpv4Addr(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief Set TCP listen address of IPv6.
 *
 * @param [in] pucIpv6 IPv6 of listen address.
 * @param [in] wPort Port of listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTServIpv6Addr
 */
ZFUNC ZINT Sip_CfgSetTServIpv6Addr(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief Set TCP listen address.
 *
 * @param [in] pstAddr TCP listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTServAddr
 */
ZFUNC ZINT Sip_CfgSetTServAddr(ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Set wait time length for TCP connect.
 *
 * @param [in] iTimes Wait time length for TCP connect.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpConnTimes
 */
ZFUNC ZINT Sip_CfgSetTcpConnTimes(ZUINT iTimes);

/**
 * @brief Set TLS listen address of IPv4.
 *
 * @param [in] iIpv4 IPv4 of listen address.
 * @param [in] wPort Port of listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServIpv4Addr
 */
ZFUNC ZINT Sip_CfgSetTlsServIpv4Addr(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief Set TLS listen address of IPv6.
 *
 * @param [in] pucIpv6 IPv6 of listen address.
 * @param [in] wPort Port of listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServIpv6Addr
 */
ZFUNC ZINT Sip_CfgSetTlsServIpv6Addr(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief Set TLS listen address.
 *
 * @param [in] pstAddr TLS listen address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServAddr
 */
ZFUNC ZINT Sip_CfgSetTlsServAddr(ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Set ToS configuration.
 *
 * @param [in] iValue Dscp Value.
 *
 * @retval ZOK Set dscp value successfully.
 * @retval ZFAILED Set dscp value failed.
 *
 * @see Sip_CfgGetDscpVal
 */
ZFUNC ZINT Sip_CfgSetDscpVal(ZUINT iValue);

/**
 * @brief Set T1 timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetT1
 */
ZFUNC ZINT Sip_CfgSetT1(ZUINT iTimeLen);

/**
 * @brief Set T2 timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetT2
 */
ZFUNC ZINT Sip_CfgSetT2(ZUINT iTimeLen);

/**
 * @brief Set T4 timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetT4
 */
ZFUNC ZINT Sip_CfgSetT4(ZUINT iTimeLen);

/**
 * @brief Set TA timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTA
 */
ZFUNC ZINT Sip_CfgSetTA(ZUINT iTimeLen);

/**
 * @brief Set TB timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTB
 */
ZFUNC ZINT Sip_CfgSetTB(ZUINT iTimeLen);

/**
 * @brief Set TC timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTC
 */
ZFUNC ZINT Sip_CfgSetTC(ZUINT iTimeLen);

/**
 * @brief Set TD timer length.
 *
 * @param [in] dwTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTD
 */
ZFUNC ZINT Sip_CfgSetTD(ZUINT iTimeLen);

/**
 * @brief Set TE timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTE
 */
ZFUNC ZINT Sip_CfgSetTE(ZUINT iTimeLen);

/**
 * @brief Set TF timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTF
 */
ZFUNC ZINT Sip_CfgSetTF(ZUINT iTimeLen);

/**
 * @brief Set TG timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTG
 */
ZFUNC ZINT Sip_CfgSetTG(ZUINT iTimeLen);

/**
 * @brief Set TH timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTH
 */
ZFUNC ZINT Sip_CfgSetTH(ZUINT iTimeLen);

/**
 * @brief Set TI timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTI
 */
ZFUNC ZINT Sip_CfgSetTI(ZUINT iTimeLen);

/**
 * @brief Set TJ timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTJ
 */
ZFUNC ZINT Sip_CfgSetTJ(ZUINT iTimeLen);

/**
 * @brief Set TK timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTK
 */
ZFUNC ZINT Sip_CfgSetTK(ZUINT iTimeLen);

/**
 * @brief Set TL timer length.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTL
 */
ZFUNC ZINT Sip_CfgSetTL(ZUINT iTimeLen);

/**
 * @brief Set length of timer for waiting NOTIFY.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetWaitNtfyTime
 */
ZFUNC ZINT Sip_CfgSetWaitNtfyTime(ZUINT iTimeLen);

/**
 * @brief Set length of timer for waiting connection establish.
 *
 * @param [in] iTimeLen Timer length in milliseconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetWaitConnTime
 */
ZFUNC ZINT Sip_CfgSetWaitConnTime(ZUINT iTimeLen);

/**
 * @brief Get register service type.
 *
 * @return Get register service type. Service type @ref EN_SIP_REG_SRV_TYPE. 
 *
 * @see @ref Sip_CfgSetRegSrvType
 */
ZFUNC ZUINT Sip_CfgGetRegSrvType(ZFUNC_VOID);

/**
 * @brief Set register service type.
 *
 * @param [in] iType The register service type. 
 *             Service type @ref EN_SIP_REG_SRV_TYPE. 
 *
 * @retval ZOK Set register service type successfully.
 * @retval ZFAILED Set register service type failed.
 *
 * @see @ref Sip_CfgGetRegSrvType
 */
ZFUNC ZINT Sip_CfgSetRegSrvType(ZUINT iType);

/**
 * @brief Set if using UDP keep alive heart beat function.
 *
 * @param [in] bEnabled If ZTRUE, enable UDP keep alive heart beat.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetUdpHeartbeat
 */
ZFUNC ZINT Sip_CfgSetUdpHeartbeat(ZBOOL bEnabled);

/**
 * @brief Set timer length in seconds of UDP keep alive heart beat function.
 *
 * @param [in] iTimeLen Timer length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetUdpHeartbeatTime
 */
ZFUNC ZINT Sip_CfgSetUdpHeartbeatTime(ZUINT iTimeLen);

/**
 * @brief Set if using TCP keep alive heart beat function.
 *
 * @param [in] bEnabled If ZTRUE, enable TCP keep alive heart beat.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpHeartbeat
 */
ZFUNC ZINT Sip_CfgSetTcpHeartbeat(ZBOOL bEnabled);

/**
 * @brief Set timer length in seconds of TCP keep alive heart beat function.
 *
 * @param [in] iTimeLen Timer length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpHeartbeatTime
 */
ZFUNC ZINT Sip_CfgSetTcpHeartbeatTime(ZUINT iTimeLen);

/**
 * @brief Set if detecting TCP keep alive heart beat time.
 *
 * @param [in] bEnabled If ZTRUE, enable detect TCP keep alive heart beat time.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpHeartbeatDetect
 */
ZFUNC ZINT Sip_CfgSetTcpHeartbeatDetect(ZBOOL bEnabled);

/**
 * @brief Set min timer length in seconds of TCP keep alive heart beat function.
 *
 * @param [in] iTimeLen Min Timer length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpMinHeartbeatTime
 */
ZFUNC ZINT Sip_CfgSetTcpMinHeartbeatTime(ZUINT iTimeLen);

/**
 * @brief Set max timer length in seconds of TCP keep alive heart beat function.
 *
 * @param [in] iTimeLen Max Timer length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpMaxHeartbeatTime
 */
ZFUNC ZINT Sip_CfgSetTcpMaxHeartbeatTime(ZUINT iTimeLen);

/**
 * @brief Set succeed timer length in seconds of TCP keep alive heart beat function.
 *
 * @param [in] iTimeLen Succeed Timer length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpSuccHeartbeatTime
 */
ZFUNC ZINT Sip_CfgSetTcpSuccHeartbeatTime(ZUINT iTimeLen);

/**
 * @brief Set current timer length in seconds of TCP keep alive heart beat function.
 *
 * @param [in] iTimeLen Current Timer length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpCurHeartbeatTime
 */
ZFUNC ZINT Sip_CfgSetTcpCurHeartbeatTime(ZUINT iTimeLen);

/**
 * @brief Set step timer length in seconds of TCP keep alive heart beat detect.
 *
 * @param [in] iTimeLen Step Timer length in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpHeartbeatStepTime
 */
ZFUNC ZINT Sip_CfgSetTcpHeartbeatStepTime(ZUINT iTimeLen);

/**
 * @brief Set tcp heartbeat fail times.
 *
 * @param [in] iTtimes Tcp hearbeat fail times.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpHeartbeatFailTimes
 */
ZFUNC ZINT Sip_CfgSetTcpHeartbeatFailTimes(ZUINT iTtimes);

/**
 * @brief Set tcp last keep alive timestamp in seconds of TCP keep alive heart beat detect.
 *
 * @param [in] zTime tcp last keep alive timestamp in seconds.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTcpLastKeepAliveTimestamp
 */
ZFUNC ZINT Sip_CfgSetTcpLastKeepAliveTimestamp(ZTIME_T zTime);

/**
 * @brief Set if using TCP smart bind function.
 *
 * @param [in] bEnabled If ZTRUE, enable TCP smart bind.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTptSmartBind
 */
ZFUNC ZINT Sip_CfgSetTptSmartBind(ZBOOL bEnabled);

/**
 * @brief Set if bind TCP address to "0.0.0.0".
 *
 * @param [in] bEnabled If ZTRUE, bind TCP address to "0.0.0.0". Otherwise,
 *                      bind TCP address to the address setted by 
 *                      @ref Sip_CfgSetTServAddr, 
 *                      @ref Sip_CfgSetTServIpv4Addr or 
 *                      @ref Sip_CfgSetTServIpv6Addr
 *
 * @see Sip_CfgGetTptAnyIpBind
 */
ZFUNC ZINT Sip_CfgSetTptAnyIpBind(ZBOOL bEnabled);

/**
 * @brief Set Sip socket type.
 *
 * @param [in] iSockType socket type.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetSockType
 */
ZFUNC ZINT Sip_CfgSetSockType(ZINT iSockType);

/**
 * @brief Set max count of call.
 *
 * @param [in] iNum Max count of call.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetCallNum
 */
ZFUNC ZINT Sip_CfgSetCallNum(ZUINT iNum);

/**
 * @brief Set max count of session.
 *
 * @param [in] iNum Max count of session.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetSessNum
 */
ZFUNC ZINT Sip_CfgSetSessNum(ZUINT iNum);

/**
 * @brief Set max count of dialog.
 *
 * @param [in] iNum Max count of dialog.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetDlgNum
 */
ZFUNC ZINT Sip_CfgSetDlgNum(ZUINT iNum);

/**
 * @brief Set max count of subscription.
 *
 * @param [in] iNum Max count of subscription.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetSubsdNum
 */
ZFUNC ZINT Sip_CfgSetSubsdNum(ZUINT iNum);

/**
 * @brief Set max count of transaction.
 *
 * @param [in] iNum Max count of transaction.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTransNum
 */
ZFUNC ZINT Sip_CfgSetTransNum(ZUINT iNum);

/**
 * @brief Set max count of connection.
 *
 * @param [in] iNum Max count of connection.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetConnNum
 */
ZFUNC ZINT Sip_CfgSetConnNum(ZUINT iNum);

/**
 * @brief Set TLS client method type.
 *
 * @param [in] iMethod TLS client method type, ref EN_TLS_METHOD_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsCliMethod
 */
ZFUNC ZINT Sip_CfgSetTlsCliMethod(ZUINT iMethod);

/**
 * @brief Set TLS client verify type.
 *
 * @param [in] iVeryType TLS client verify type, ref EN_TLS_VERY_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsCliVeryType
 */
ZFUNC ZINT Sip_CfgSetTlsCliVeryType(ZUINT iVeryType);

/**
 * @brief Set TLS client trusted ca path.
 *
 * @param [in] pcFile Trusted ca file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsCliTrustCaPath
 */
ZFUNC ZINT Sip_CfgSetTlsCliTrustCaPath(ZCHAR *pcPath);

/**
 * @brief Set TLS client trusted ca file.
 *
 * @param [in] pcFile Trusted ca file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsCliTrustCertFile
 */
ZFUNC ZINT Sip_CfgSetTlsCliTrustCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS client certification file.
 *
 * @param [in] pcFile Certification file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsCliCertFile
 */
ZFUNC ZINT Sip_CfgSetTlsCliCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS client private key file.
 *
 * @param [in] pcFile Private key file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsCliPrvKeyFile
 */
ZFUNC ZINT Sip_CfgSetTlsCliPrvKeyFile(ZCHAR *pcFile);

/**
 * @brief Set TLS client private key password.
 *
 * @param [in] pcPasswd Private key password.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsCliPrvKeyPass
 */
ZFUNC ZINT Sip_CfgSetTlsCliPrvKeyPass(ZCHAR *pcPasswd);

/**
 * @brief Set TLS server method type.
 *
 * @param [in] iMethod TLS server method type, ref EN_TLS_METHOD_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServMethod
 */
ZFUNC ZINT Sip_CfgSetTlsServMethod(ZUINT iMethod);

/**
 * @brief Set TLS server verify type.
 *
 * @param [in] iVeryType TLS server verify type, ref EN_TLS_VERY_TYPE.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServVeryType
 */
ZFUNC ZINT Sip_CfgSetTlsServVeryType(ZUINT iVeryType);

/**
 * @brief Set TLS server trusted ca file.
 *
 * @param [in] pcFile Trusted ca file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServTrustCertFile
 */
ZFUNC ZINT Sip_CfgSetTlsServTrustCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS server certification file.
 *
 * @param [in] pcFile Certification file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServCertFile
 */
ZFUNC ZINT Sip_CfgSetTlsServCertFile(ZCHAR *pcFile);

/**
 * @brief Set TLS server private key file.
 *
 * @param [in] pcFile Private key file name.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServPrvKeyFile
 */
ZFUNC ZINT Sip_CfgSetTlsServPrvKeyFile(ZCHAR *pcFile);

/**
 * @brief Set TLS server private key password.
 *
 * @param [in] pcPasswd Private key password.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetTlsServPrvKeyPass
 */
ZFUNC ZINT Sip_CfgSetTlsServPrvKeyPass(ZCHAR *pcPasswd);

/**
 * @brief Set support reuse connection.
 *
 * @param [in] bSuptAlias Support reuse connection.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 */
ZFUNC ZINT Sip_CfgSetSuptReUseConn(ZBOOL bSuptAlias);

/**
 * @brief Set random string length.
 *
 * @param [in] wLen random string length to set, Value must between 7 and 26.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetRndStrLen
 */
ZFUNC ZINT Sip_CfgSetRndStrLen(ZUSHORT wLen);

/**
 * @brief Set support local login.
 *
 * @param [in] bSuptLclLogin Support local login.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 */
ZFUNC ZINT Sip_CfgSetSuptLclLogin(ZBOOL bSuptLclLogin);

/**
 * @brief Get udp socket connect enable.
 *
 * @retval udp socket connect enable flag.
 *
 * @see Sip_CfgSetUdpConnectEnable
 *
 */
ZFUNC ZBOOL Sip_CfgGetUdpConnectEnable(ZFUNC_VOID);

/**
 * @brief Set udp socket connect enable.
 *
 * @param [in] bEnable Enable udp connect.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_CfgGetUdpConnectEnable
 *
 */
ZFUNC ZINT Sip_CfgSetUdpConnectEnable(ZBOOL bEnable);

/* sip config get register mode. */
ZFUNC ZUINT Sip_CfgGetRegMode(ZFUNC_VOID);

/* sip config set register mode. */
ZFUNC ZINT Sip_CfgSetRegMode(ZUINT iMode);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_CFG_H__ */

