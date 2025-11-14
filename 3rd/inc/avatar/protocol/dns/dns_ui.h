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
  File name     : dns_ui.h
  Module        : dns protocol
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2006-03-24    
  Description   :
      Data structure and function definitions required 
      by the dns user interfaces

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DNS_UI_H__
#define _DNS_UI_H__
/** 
 * @file dns_ui.h
 * @brief DNS user interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DNS user interface of asynchronous lookup domain name.
 *
 * @param [in] zCookie The user id.
 * @param [in] wRrType The resource records type EN_DNS_RR_TYPE.
 * @param [in] pstPntDN A point of dns.
 * @param [in] pfnCallback Callback the query result.
 *
 * @retval ZOK Lookup the domain name successful.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Dns_LookupX.
 */
ZFUNC ZINT Dns_Lookup(ZCOOKIE zCookie, ZUSHORT wRrType, ZCHAR *pcPntDn, 
                ZUSHORT wDnLen, PFN_DNSCALLBACK pfnCallback);

/**
 * @brief DNS user interface of synchronous lookup domain name.
 *
 * @param [in] zCookie The user id.
 * @param [in] zMemBuf Memory buffer.
 * @param [in] wRrType The resource records type EN_DNS_RR_TYPE.
 * @param [in] pstPntDn A point of domain name.
 * @param [in] pstRsp The query response.
 *
 * @retval ZOK Lookup the domain name successful.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Dns_Lookup.
 */
ZFUNC ZINT Dns_LookupX(ZCOOKIE zCookie, ZDBUF zMemBuf, ZUSHORT wRrType, 
                ST_ZOS_SSTR *pstPntDn, ST_DNS_RSP *pstRsp);

/**
 * @brief DNS user interface of asynchronous get host by name.
 *
 * @param [in] zCookie The user id.
 * @param [in] pstName The host name,Get host by this name.
 * @param [in] pfnIpv4Callback Callback the query result of ipv4 address.
 *
 * @retval ZOK Get the host successful.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Dns_GetHostByNameX.
 */
ZFUNC ZINT Dns_GetHostByName(ZCOOKIE zCookie, ST_ZOS_SSTR *pstName, 
                PFN_DNSIPV4CALLBACK pfnIpv4Callback);

/**
 * @brief DNS user interface of synchronous get host by name.
 *
 * @param [in] pstName The host name,Get host by this name.
 * @param [in] piIpAddr The ip address,to return the ip4 address.
 *
 * @retval ZOK Get the host successful.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Dns_GetHostByName.
 */
ZFUNC ZINT Dns_GetHostByNameX(ST_ZOS_SSTR *pstName, ZUINT *piIpAddr);

/**
 * @brief DNS user interface of synchronous get host by name.
 *
 * @param [in] pstName The host name,Get host by this name.
 * @param [in] piIpAddr The ip address,to return the ip4 address.
 *
 * @retval ZOK Get the host successful.
 * @retval ZFAILED Otherwise.
 *
 * @see @ref Dns_GetHostByName.
 */
ZFUNC ZINT Dns_GetHostByNameS(ZCHAR *pcName, ZUINT *piIpAddr);

#ifdef __cplusplus
}
#endif

#endif /* _DNS_UI_H__ */

