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
  File name     : zpwin_task.h
  Module        : windows task library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by task
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPWIN_TASK_H__
#define _ZPWIN_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* windows task spawn */
ZFUNC ZINT Zpwin_TaskSpawn(ZCONST ZCHAR *pcName, ZINT iPriority, ZUINT iStackSize, 
                PFN_ZTASKENTRY pfnEntry, ZVOID *pEntryParm, 
                ZTHREAD_ID *pzRealTaskId, ZTHREAD_ID *pzThreadId);

/* windows task delete */
ZFUNC ZINT Zpwin_TaskDelete(ZTHREAD_ID zThreadId, ZBOOL bForce);

/* windows task delay */
ZFUNC ZVOID Zpwin_TaskDelay(ZUINT iMilliseconds);

/* if windows task is on the current thread */
ZFUNC ZBOOL Zpwin_TaskIsCurrent(ZTHREAD_ID zRealThreadId);

/* if the current windows thread is the main thread */
ZFUNC ZBOOL Zpwin_TaskIsMainThread();

/* get current task id */
ZFUNC ZTHREAD_ID Zpwin_TaskGetCurrent();

/* set current task thread name  */
ZFUNC ZVOID Zpwin_TaskSetThreadName(ZCONST ZCHAR *pcName);

/* get current task thread name  */
ZFUNC ZVOID Zpwin_TaskGetThreadName(ZCHAR *pcName);

/* compare thread id */
ZFUNC ZBOOL Zpwin_TaskEqual(ZTHREAD_ID zRealThreadIdA, ZTHREAD_ID zRealThreadIdB);

#ifdef __cplusplus
}
#endif

#endif /* _ZPWIN_TASK_H__ */

