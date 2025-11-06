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
  File name     : msrp_abnf_cfg.h
  Module        : msrp protocol configuration
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Configurations to customize MSRP.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_ABNF_CFG_H__
#define _MSRP_ABNF_CFG_H__

/** 
 * @file msrp_abnf_cfg.h
 * @brief MSRP ABNF configuration interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get log level of MSRP ABNF.
 *
 * @retval Log Level, default is ZLOG_LEVEL_ERROR.
 *
 * @see @ref Msrp_AbnfCfgSetLogLevel
 */
ZFUNC ZUINT Msrp_AbnfCfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief Set log level of MSRP ABNF.
 *
 * @param [in] iLevel Log level.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 * @see @ref Msrp_AbnfCfgGetLogLevel
 */
ZFUNC ZINT Msrp_AbnfCfgSetLogLevel(ZUINT iLevel);

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_ABNF_CFG_H__ */

