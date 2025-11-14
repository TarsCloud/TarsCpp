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
  File name     : sip_task.h
  Module        : sip protocol
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_TASK_H__
#define _SIP_TASK_H__

/** 
 * @file
 * @brief SIP task Interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start SIP task.
 *
 * @retval ZOK SIP task start successfully.
 * @retval ZFAILED SIP task start failed.
 */
ZFUNC ZINT Sip_TaskStart(ZFUNC_VOID);

/**
 * @brief Stop SIP task.
 */
ZFUNC ZVOID Sip_TaskStop(ZFUNC_VOID);

/**
 * @brief Get SIP task ID.
 *
 * @retval SIP task ID, return ZMAXTASKID if SIP task has not started.
 */
ZFUNC ZTASKID Sip_TaskGetId(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_TASK_H__ */

