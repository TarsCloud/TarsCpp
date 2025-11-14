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
  File name     : dns_util.h
  Module        : dns library utility functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2006-03-24    
  Description   :
      Data structure and function definitions required 
      by the dns utility interface

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DNS_UTIL_H__
#define _DNS_UTIL_H__
/** 
 * @file dns_util.h
 * @brief DNS utility functions.
 *
 * @see 
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DNS copy RR group(array).
 *
 * @param [in] zMemBuf The memory buffer.
 * @param [out] ppstDstRr To return the dest rr group address.
 * @param [in] pastSrcRr DNS query response of RR.
 * @param [in] ucRrCount The RR array count.
 *
 * @retval ZOK Copy successful.
 * @retval ZFAILED Otherwise.
 * @see Dns_CpyQRsp.
 */
ZFUNC ZINT Dns_CpyRrGrp(ZDBUF zMemBuf, ST_DNS_RSP_RR **ppstDstRr,
                ST_DNS_RSP_RR *pastSrcRr, ZUCHAR ucRrCount);

/**
 * @brief DNS copy query response.
 *
 * @param [in] zMemBuf The memory buffer.
 * @param [in] pstDstRr To copy the result from pstSrcRr.
 * @param [in] pstSrcRr DNS query response of RR.
 *
 * @retval ZOK Copy successful.
 * @retval ZFAILED Otherwise.
 * @see 
 */
ZFUNC ZINT Dns_CpyQRsp(ZDBUF zMemBuf, ST_DNS_RSP_RR *pstDstRr,
                ST_DNS_RSP_RR *pstSrcRr);

/**
 * @brief DNS copy response.
 *
 * @param [in] zMemBuf The memory buffer.
 * @param [in] pstDstRsp To copy the result from pstSrcRr.
 * @param [in] pstSrcRsp Second DNS query response.
 *
 * @retval ZOK Copy successful.
 * @retval ZFAILED Otherwise.
 * @see Dns_CpyRrGrp.
 */
ZFUNC ZINT Dns_CpyRsp(ZDBUF zMemBuf, ST_DNS_RSP *pstDstRsp,
                ST_DNS_RSP *pstSrcRsp);

/**
 * @brief DNS get A RR ipv4 address.
 *
 * @param [in] pstRsp DNS query response.
 * @param [in] piIpv4 Ip address,it's to get reversed ip sequence.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Dns_GetRrAIpv4(ST_DNS_RSP *pstRsp, ZUINT *piIpv4);

/**
 * @brief DNS get AAAA RR ipv6 address.
 *
 * @param [in] pstRsp DNS query response.
 * @param [in] pucAddr Ip address,it's to get reversed ip sequence.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Dns_GetRrAIpv6(ST_DNS_RSP *pstRsp, ZUCHAR *pucAddr);

/**
 * @brief DNS print out the response data to the specific log.
 *
 * @param [in] pstRsp DNS query response.
 * @param [in] pcLogName Log manager name.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_LogPrintRsp(ST_DNS_RSP *pstRsp, ZCHAR *pcLogName);

/**
 * @brief DNS Close.
 * 
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Dns_Close();

#ifdef __cplusplus
}
#endif

#endif /* _DNS_UTIL_H__ */

