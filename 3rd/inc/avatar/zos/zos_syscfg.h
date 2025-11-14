/************************************************************************

        Copyright (c) 2005-2011 by Juphoon System Software, Inc.
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
  File name     : zos_syscfg.h
  Module        : ZOS system configuration
  Author        : leo.lv   
  Version       : 0.1   
  Created on    : 2011-11-01    
  Description   :
      Marcos definitions required by the zos system configuration.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_SYSCFG_H__
#define _ZOS_SYSCFG_H__

/** 
 * @file zos_syscfg.h
 * @brief Zos system configruation functions.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Zos system config initialization. */
typedef ZINT (*PFN_ZSYSCFGINIT)(ZVOID *pCfg);

/** @brief Zos system config destroy. */
typedef ZVOID (*PFN_ZSYSCFGDESTROY)(ZVOID *pCfg);

/**
 * @brief Attach a system config.
 *
 * @param [in] iEnvId The config id.
 * @param [in] iSize The config memory size.
 * @param [in] pfnInit The config initialization function.
 * @param [in] pfnDestroy The config destroy function.
 * @param [out] ppCfg A pointer to config memory.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see @ref Zos_SysCfgDetach
 */
ZFUNC ZINT Zos_SysCfgAttach(ZUINT iEnvId, ZINT iSize, 
                PFN_ZSYSCFGINIT pfnInit, PFN_ZSYSCFGDESTROY pfnDestroy,
                ZVOID **ppCfg);

/**
 * @brief Detach a system config.
 *
 * @param [in] iEnvId The config id.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see @ref Zos_SysCfgAttach
 */
ZFUNC ZVOID Zos_SysCfgDetach(ZUINT iEnvId);

/**
 * @brief Locate a system config.
 *
 * @param [in] iEnvId The config id.
 * @param [out] ppMem A pointer to config memory.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see @ref Zos_SysCfgAttach
 */
ZFUNC ZINT Zos_SysCfgLocate(ZUINT iEnvId, ZVOID **ppCfg);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_SYSCFG_H__ */

