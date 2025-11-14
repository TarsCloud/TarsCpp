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
  File name     : zplin_mutex.h
  Module        : linux mutex library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by mutex 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPLIN_MUTEX_H__
#define _ZPLIN_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* create a mutex */
ZFUNC ZINT Zplin_MutexCreate(ZMUTEX *pzMutex);

/* delete a mutex */
ZFUNC ZVOID Zplin_MutexDelete(ZMUTEX *pzMutex);

/* lock a mutex */
ZFUNC ZINT Zplin_MutexLock(ZMUTEX *pzMutex);

/* unlock a mutex */
ZFUNC ZINT Zplin_MutexUnlock(ZMUTEX *pzMutex);

/* create a shared-exclusive lock */
ZFUNC ZINT Zplin_SharexCreate(ZSHAREX *pzSharex);

/* delete a shared-exclusive lock */
ZFUNC ZVOID Zplin_SharexDelete(ZSHAREX *pzSharex);

/* lock a shared-exclusive lock */
ZFUNC ZINT Zplin_SharexLock(ZSHAREX *pzSharex);

/* unlock a shared-exclusive lock */
ZFUNC ZINT Zplin_SharexUnlock(ZSHAREX *pzSharex);

/* lock a shared-exclusive lock */
ZFUNC ZINT Zplin_SharexLockEx(ZSHAREX *pzSharex);

/* unlock a shared-exclusive lock */
ZFUNC ZINT Zplin_SharexUnlockEx(ZSHAREX *pzSharex);

/* create some semaphores */
ZFUNC ZINT Zplin_SemCreate(ZSEM *pzSem, ZUINT iValue);

/* create only one semaphore */
ZFUNC ZINT Zplin_SemCreateOne(ZSEM *pzSem);

/* delete a semaphore */
ZFUNC ZVOID Zplin_SemDelete(ZSEM *pzSem);

/* delete a semaphore, count -- if count == 0 then block current task */
ZFUNC ZINT Zplin_SemWait(ZSEM *pzSem, ZUINT iMilliSeconds);

/* give a semaphore, count++ ,wake up current task */
ZFUNC ZINT Zplin_SemPost(ZSEM *pzSem);

#ifdef __cplusplus
}
#endif

#endif /* _ZPLIN_MUTEX_H__ */

