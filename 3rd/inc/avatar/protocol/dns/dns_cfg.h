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
  File name     : dns_cfg.h
  Module        : dns library configure functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2006-03-24    
  Description   :
      Data structure and function definitions required 
      by the dns configure interface

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DNS_CFG_H__
#define _DNS_CFG_H__
/** 
 * @file dns_cfg.h
 * @brief DNS configuration interfaces.
 *
 * @see 
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DNS config get task priority.
 *
 * @retval The task priority, default is ZTASK_PRIORITY_NORMAL.
 */
ZFUNC ZINT Dns_CfgGetTaskPriority(ZFUNC_VOID);

/**
 * @brief DNS config get log level.
 *
 * @retval The log level, default is ZLOG_LEVEL_ALL.
 */
ZFUNC ZUINT Dns_CfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief DNS config get is ipv6 transport.
 *
 * @retval The type of tansport is ipv6.
 */
ZFUNC ZBOOL Dns_CfgGetIsIpv6Tpt(ZFUNC_VOID);

/**
 * @brief DNS config get is ipv6 primary server.
 *
 * @retval The type of primary server is ipv6.
 */
ZFUNC ZBOOL Dns_CfgGetIsIpv6PriServ(ZFUNC_VOID);

/**
 * @brief DNS config get is ipv6 secondary server.
 *
 * @retval The type of secondary server is ipv6.
 */
ZFUNC ZBOOL Dns_CfgGetIsIpv62ndServ(ZFUNC_VOID);

/**
 * @brief DNS config get local ipv4 address.
 *
 * @param [in] piIpv4 The localipv4 address.
 * @param [in] pwPort The local port.
 *
 * @retval ZOK Get the local ipv4 address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgGetLocalIpv4(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief DNS config get local ipv6 address.
 *
 * @param [out] ppucIpv6 The local ipv6 address.
 * @param [in] pwPort The local port.
 *
 * @retval ZOK Get the local ipv6 address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgGetLocalIpv6(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/**
 * @brief DNS config get local address.
 *
 * @retval The local ip address.
 */
ZFUNC ST_ZOS_INET_ADDR * Dns_CfgGetLocalAddr(ZFUNC_VOID);

/**
 * @brief DNS config get primary server ipv4 address.
 *
 * @param [in] piIpv4 The primary server ipv4 address.
 * @param [in] pwPort The primary service port.
 *
 * @retval ZOK Get the address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgGetPriServIpv4(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief DNS config get primary server ipv6 address.
 *
 * @param [out] ppucIpv6 The primary server Ipv6 address.
 * @param [in] pwPort The primary service port.
 *
 * @retval ZOK Get the address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgGetPriServIpv6(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/**
 * @brief DNS config get secondary server ipv4 address.
 *
 * @param [in] piIpv4 The secondary server ipv4 address.
 * @param [in] pwPort The secondary server port.
 *
 * @retval ZOK Get the address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgGet2ndServIpv4(ZUINT *piIpv4, ZUSHORT *pwPort);

/**
 * @brief DNS config get secondary server ipv6 address.
 *
 * @param [out] ppucIpv6 The secondary server ipv6 address.
 * @param [in] pwPort The secondary server port.
 *
 * @retval ZOK Get the address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgGet2ndServIpv6(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/**
 * @brief DNS config get query wait time length.
 *
 * @retval The query time length.
 */
ZFUNC ZUINT Dns_CfgGetQryTimeLen(ZFUNC_VOID);

/* dns config get query resend wait time length */
ZFUNC ZUINT Dns_CfgGetQryReTimeLen(ZFUNC_VOID);

/**
 * @brief DNS config get query number.
 *
 * @retval The query max number, default is 10.
 */
ZFUNC ZUINT Dns_CfgGetQryNum(ZFUNC_VOID);

/**
 * @brief DNS config get support option of using os gethostbyname.
 *
 * @retval Using OS gethostbyname option, default is ZFALSE.
 */
ZFUNC ZBOOL Dns_CfgGetSuptOsGetHost(ZFUNC_VOID);

/**
 * @brief DNS config get no cache look up.
 *
 * @retval No cache option after look up successful.
 */
ZFUNC ZBOOL Dns_CfgGetNoCache(ZFUNC_VOID);

/**
 * @brief DNS config set task priority.
 *
 * @param [in] iPriority The Task priority.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetTaskPriority(ZINT iPriority);

/**
 * @brief DNS config set log level.
 *
 * @param [in] iLevel The log level.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetLogLevel(ZUINT iLevel);

/**
 * @brief DNS config set local ipv4 address.
 *
 * @param [in] iIpv4 The ipv4 address.
 * @param [in] wPort The service port.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetLocalIpv4(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief DNS config set local ipv6 address.
 *
 * @param [in] pucIpv6 The ipv6 address.
 * @param [in] wPort The service port.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetLocalIpv6(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief DNS config set local address.
 *
 * @param [in] pstAddr The local address.
 *
 * @retval ZOK Set the local address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetLocalAddr(ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief DNS config set primary server ipv4 address.
 *
 * @param [in] iIpv4 The primary server ipv4 address.
 * @param [in] wPort The primary service port.
 *
 * @retval ZOK Set the address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetPriServIpv4(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief DNS config Set primary server ipv6 address.
 *
 * @param [in] pucIpv6 The primary server ipv6 address.
 * @param [in] wPort The primary service port.
 *
 * @retval ZOK Set the address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetPriServIpv6(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief DNS config set secondary server ipv4 address.
 *
 * @param [in] iIpv4 The secondary server ipv4 address.
 * @param [in] wPort The secondary service port.
 *
 * @retval ZOK Set the address successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSet2ndServIpv4(ZUINT iIpv4, ZUSHORT wPort);

/**
 * @brief DNS config set secondary server ipv6 address.
 *
 * @param [in] pucIpv6 The secondary server ipv6 address.
 * @param [in] wPort The secondary service port.
 *
 * @retval ZOK Set the address successful.
 * @retval ZFAILED Otherwise.
 */

ZFUNC ZINT Dns_CfgSet2ndServIpv6(ZUCHAR *pucIpv6, ZUSHORT wPort);

/**
 * @brief DNS config set query wait time length.
 *
 * @param [in] iTimeLen The query time length.
 *
 * @retval ZOK Set the length successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetQryTimeLen(ZUINT iTimeLen);

/* dns config set query resend wait time length */
ZFUNC ZINT Dns_CfgSetQryReTimeLen(ZUINT iTimeLen);

/**
 * @brief DNS config set query number.
 *
 * @param [in] iNum The query max number.
 *
 * @retval ZOK Set the number successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetQryNum(ZUINT iNum);

/**
 * @brief DNS config set using OS gethostbyname option.
 *
 * @param [in] bSupt The using OS gethostbyname option.
 *
 * @retval ZOK Set the option successful, otherwise ZFAILED.
 */
ZFUNC ZINT Dns_CfgSetSuptOsGetHost(ZBOOL bSupt);

/**
 * @brief DNS config set no cache look up.
 *
 * @param [in] bNoCache The value of NoCache(True or False).
 *
 * @retval ZOK Set the no cache successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_CfgSetNoCache(ZBOOL bNoCache);

#ifdef __cplusplus
}
#endif

#endif /* _DNS_CFG_H__ */

