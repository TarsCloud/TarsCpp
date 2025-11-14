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
  File name     : zplin_task.h
  Module        : linux task library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by task
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPLIN_TASK_H__
#define _ZPLIN_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* linux module post drive notification */
ZFUNC ZINT Zplin_ModPostDrv(ZFUNC_VOID);

/* linux task spawn */
ZFUNC ZINT Zplin_TaskSpawn(ZCONST ZCHAR *pcName, ZINT iPriority, ZUINT iStackSize, 
                PFN_ZTASKENTRY pfnEntry, ZVOID *pEntryParm, 
                ZTHREAD_ID *pzRealTaskId, ZTHREAD_ID *pzThreadId);

/* linux task delete */
ZFUNC ZINT Zplin_TaskDelete(ZTHREAD_ID zThreadId, ZBOOL bForce);

/* linux task delay */
ZFUNC ZVOID Zplin_TaskDelay(ZUINT iMilliseconds);

/* if linux task is on the current thread */
ZFUNC ZBOOL Zplin_TaskIsCurrent(ZTHREAD_ID zRealThreadId);

/* get current task id */
ZFUNC ZTHREAD_ID Zplin_TaskGetCurrent();

/* set current task thread name  */
ZFUNC ZVOID Zplin_TaskSetThreadName(ZCONST ZCHAR *pcName);

/* get current task thread name  */
ZFUNC ZVOID Zplin_TaskGetThreadName(ZCHAR *pcName);

/* if the current linux thread is the main thread */
ZFUNC ZBOOL Zplin_TaskIsMainThread();

/* compare thread id  */
ZFUNC ZBOOL Zplin_TaskEqual(ZTHREAD_ID zRealThreadIdA, ZTHREAD_ID zRealThreadIdB);

/* return the main thread Id*/
ZFUNC pthread_t Zplin_UiThreadId(ZFUNC_VOID);
#ifdef __cplusplus
}
#endif

#endif /* _ZPLIN_TASK_H__ */

