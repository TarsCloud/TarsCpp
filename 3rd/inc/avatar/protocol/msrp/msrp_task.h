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
  File name     : msrp_task.h
  Module        : msrp protocol
  Author        : cathy.chen
  Version       : 0.1
  Created on    : 2010-10-18
  Description   :
    Marcos and structure definitions required by the msrp protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_TASK_H__
#define _MSRP_TASK_H__

/** 
 * @file msrp_task.h
 * @brief MSRP task Interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start MSRP task.
 *
 * @retval ZOK MSRP task start successfully.
 * @retval ZFAILED MSRP task start failed.
 */
ZFUNC ZINT Msrp_TaskStart(ZFUNC_VOID);

/**
 * @brief Stop MSRP task.
 */
ZFUNC ZVOID Msrp_TaskStop(ZFUNC_VOID);

/**
 * @brief Restart MSRP task.
 *
 * @retval ZOK MSRP task restart successfully.
 * @retval ZFAILED MSRP task restart failed.
 */
ZFUNC ZINT Msrp_TaskRestart(ZFUNC_VOID);

/**
 * @brief Get MSRP task ID.
 *
 * @retval MSRP task ID, return ZMAXTASKID if MSRP task has not started.
 */
ZFUNC ZTASKID Msrp_TaskGetId(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_TASK_H__ */

