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
  File name     : sip_abnf_cfg.h
  Module        : sip abnf configuration
  Author        : Aaron Zhong   
  Version       : 0.1   
  Created on    : 2005-08-12    
  Description   :
      Configurations to customize SIP.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_ABNF_CFG_H__
#define _SIP_ABNF_CFG_H__

/** 
 * @file 
 * @brief SIP ABNF configuration interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Null option value. */
#define SIP_ABNF_OPT_NULL            0 

/** @brief Option for strict decode. When setted, message decode 
 *         will return failed if any header decode failed. */
#define SIP_ABNF_OPT_STRICT_DECODE   1 

/** @brief Option for encode in compact header mode. */
#define SIP_ABNF_OPT_USE_COMPACT_HDR 2 

/** @brief Option for SDP decode. When setted, SDP body in message will be
 *         decode into structure, or remain in string format. */
#define SIP_ABNF_OPT_SDP_DECODE      3 

/** @brief Option for add content length header in SIP message. */
#define SIP_ABNF_OPT_ADD_CONTENT_LEN 4 

/** @brief All option value. */
#define SIP_ABNF_OPT_ALL             ZMAXUINT 

/**
 * @brief Get log level.
 *
 * @retval Log Level, default is ZLOG_LEVEL_ERROR.
 *
 * @see Sip_AbnfCfgSetLogLevel
 */
ZFUNC ZUINT Sip_AbnfCfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief Get option.
 *
 * @retval Config option includes @ref SIP_ABNF_OPT_NULL, 
 *                        @ref SIP_ABNF_OPT_STRICT_DECODE, 
 *                        @ref SIP_ABNF_OPT_SDP_DECODE, 
 *                        @ref SIP_ABNF_OPT_USE_COMPACT_HDR,
 *                        @ref SIP_ABNF_OPT_ADD_CONTENT_LEN.
 *
 * @see Sip_AbnfCfgSetOption
 */
ZFUNC ZUINT Sip_AbnfCfgGetOption(ZFUNC_VOID);

/**
 * @brief Set log level.
 *
 * @param [in] iLevel Log level.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_AbnfCfgGetLogLevel
 */
ZFUNC ZINT Sip_AbnfCfgSetLogLevel(ZUINT iLevel);

/**
 * @brief Set option.
 *
 * @param [in] iOption Config option includes 
 *                      SIP_ABNF_OPT_STRICT_DECODE, SIP_ABNF_OPT_SDP_DECODE, 
 *                      SIP_ABNF_OPT_ADD_CONTENT_LEN.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see Sip_AbnfCfgGetOption
 */
ZFUNC ZINT Sip_AbnfCfgSetOption(ZUINT iOption);

/**
 * @brief Clear option.
 *
 * @param [in] iOption Config option, see SIP_ABNF_OPT_STRICT_DECODE.
 *
 * @retval ZOK Clear configuration successfully.
 * @retval ZFAILED Clear configuration failed.
 *
 * @see Sip_AbnfCfgGetOption
 */
ZFUNC ZINT Sip_AbnfCfgClrOption(ZUINT iOption);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_ABNF_CFG_H__ */

