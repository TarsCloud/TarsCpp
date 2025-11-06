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
  File name     : zos_sysevnt.h
  Module        : ZOS system event
  Author        : leo.lv   
  Version       : 0.1   
  Created on    : 2011-11-01    
  Description   :
      Marcos definitions required by the zos system event.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_SYSEVNT_H__
#define _ZOS_SYSEVNT_H__

/** 
 * @file zos_sysevnt.h
 * @brief Zos system event functions.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Attach a system event.
 *
 * @param [in] iEnvId The event id.
 * @param [in] iSize The event memory size.
 * @param [in] pfnInit The event initialization function.
 * @param [in] pfnDestroy The event destroy function.
 * @param [out] ppCfg A pointer to event memory.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see @ref Zos_SysEvntDetach
 */
ZFUNC ZINT Zos_SysEvntAttach(ZUINT iEnvId, ZUINT iParmSize);

/**
 * @brief Detach a system event.
 *
 * @param [in] iEnvId The event id.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see @ref Zos_SysEvntAttach
 */
ZFUNC ZVOID Zos_SysEvntDetach(ZUINT iEnvId);

/**
 * @brief Locate a system event.
 *
 * @param [in] iEnvId The event id.
 * @param [out] ppMem A pointer to event memory.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see @ref Zos_SysEvntAttach
 */
ZFUNC ZINT Zos_SysParmAttach(ZUINT iParmId, ZCHAR *pcName, 
                ZCHAR *pcDftVal);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_SYSEVNT_H__ */

