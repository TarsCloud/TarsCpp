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
  File name     : dns_task.h
  Module        : dns library task functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2006-03-24   
  Description   :
      Data structure and function definitions required 
      by the dns task interface

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DNS_TASK_H__
#define _DNS_TASK_H__
/** 
 * @file dns_task.h
 * @brief DNS task interfaces.
 *
 * @see 
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DNS start.
 *
 * @retval ZOK Start successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Dns_TaskStart(ZFUNC_VOID);

/**
 * @brief DNS stop.
 *
 * @retval ZOK Stop successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZVOID Dns_TaskStop(ZFUNC_VOID);

/**
 * @brief Get DNS task ID.
 *
 * @retval DNS task ID, return ZMAXTASKID if DNS task has not started.
 */
ZFUNC ZTASKID Dns_TaskGetId(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _DNS_TASK_H__ */

