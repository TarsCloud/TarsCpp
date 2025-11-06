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
  File name     : zpios_task.h
  Module        : ios task library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by task
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPIOS_TASK_H__
#define _ZPIOS_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ios module post drive notification */
ZFUNC ZINT Zpios_ModPostDrv(ZFUNC_VOID);

/* ios task spawn */
ZFUNC ZINT Zpios_TaskSpawn(ZCONST ZCHAR *pcName, ZINT iPriority, 
                ZUINT iStackSize, PFN_ZTASKENTRY pfnEntry, ZVOID *pEntryParm, 
                ZTHREAD_ID *pzRealTaskId, ZTHREAD_ID *pzThreadId);

/* ios task delete */
ZFUNC ZINT Zpios_TaskDelete(ZTHREAD_ID zThreadId, ZBOOL bForce);

/* ios task delay */
ZFUNC ZVOID Zpios_TaskDelay(ZUINT iMilliseconds);

/* if ios task is on the current thread */
ZFUNC ZBOOL Zpios_TaskIsCurrent(ZTHREAD_ID zRealThreadId);

/* if the current ios thread is the main thread */
ZFUNC ZBOOL Zpios_TaskIsMainThread();

/* get current task id */
ZFUNC ZTHREAD_ID Zpios_TaskGetCurrent();

/* set current task thread name  */
ZFUNC ZVOID Zpios_TaskSetThreadName(ZCONST ZCHAR *pcName);

/* get current task thread name  */
ZFUNC ZVOID Zpios_TaskGetThreadName(ZCHAR *pcName);

/* compare thread id  */
ZFUNC ZBOOL Zpios_TaskEqual(ZTHREAD_ID zRealThreadIdA, ZTHREAD_ID zRealThreadIdB);

#ifdef __cplusplus
}
#endif

#endif /* _ZPIOS_TASK_H__ */

