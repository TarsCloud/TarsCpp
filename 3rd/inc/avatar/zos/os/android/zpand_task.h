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
  File name     : zpand_task.h
  Module        : android task library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Data structure and function definitions required by task
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPAND_TASK_H__
#define _ZPAND_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* android module post drive notification */
ZFUNC ZINT Zpand_ModPostDrv(ZFUNC_VOID);

/* android task spawn */
ZFUNC ZINT Zpand_TaskSpawn(ZCHAR *pcName, ZINT iPriority, ZUINT iStackSize, 
                PFN_ZTASKENTRY pfnEntry, ZVOID *pEntryParm, 
                ZTHREAD_ID *pzRealTaskId, ZTHREAD_ID *pzThreadId);

/* android task delete */
ZFUNC ZINT Zpand_TaskDelete(ZTHREAD_ID zThreadId, ZBOOL bForce);

/* android task delay */
ZFUNC ZVOID Zpand_TaskDelay(ZUINT iMilliseconds);

/* android task exit */
ZFUNC ZVOID Zpand_TaskExit(ZTHREAD_ID zThreadId, ZCOOKIE zCookie);

/* if android task is on the current thread */
ZFUNC ZBOOL Zpand_TaskIsCurrent(ZTHREAD_ID zRealThreadId);

/* check if is ui thread */
ZFUNC ZBOOL Zpand_TaskIsMainThread(ZFUNC_VOID);

/* get current task id */
ZFUNC ZTHREAD_ID Zpand_TaskGetCurrent();

/* set current task thread name  */
ZFUNC ZVOID Zpand_TaskSetThreadName(ZCONST ZCHAR *pcName);

/* get current task thread name  */
ZFUNC ZVOID Zpand_TaskGetThreadName(ZCHAR *pcName);

/* compare thread id  */
ZFUNC ZBOOL Zpand_TaskEqual(ZTHREAD_ID zRealThreadIdA, ZTHREAD_ID zRealThreadIdB);

/* init crash report */
ZFUNC ZINT Zpand_TaskInitCrashReport(ZCHAR* pcDmpPath);

#ifdef __cplusplus
}
#endif

#endif /* _ZPAND_TASK_H__ */

