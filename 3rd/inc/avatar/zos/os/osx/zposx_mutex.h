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
  File name     : zposx_mutex.h
  Module        : mac osx mutex library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by mutex 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPOSX_MUTEX_H__
#define _ZPOSX_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* mac osx create a mutex */
ZFUNC ZINT Zposx_MutexCreate(ZMUTEX *pzMutex);

/* mac osx delete a mutex */
ZFUNC ZVOID Zposx_MutexDelete(ZMUTEX *pzMutex);

/* mac osx lock a mutex */
ZFUNC ZINT Zposx_MutexLock(ZMUTEX *pzMutex);

/* mac osx unlock a mutex */
ZFUNC ZINT Zposx_MutexUnlock(ZMUTEX *pzMutex);

/* create a shared-exclusive lock */
ZFUNC ZINT Zposx_SharexCreate(ZSHAREX *pzSharex);

/* delete a shared-exclusive lock */
ZFUNC ZVOID Zposx_SharexDelete(ZSHAREX *pzSharex);

/* lock a shared-exclusive lock */
ZFUNC ZINT Zposx_SharexLock(ZSHAREX *pzSharex);

/* unlock a shared-exclusive lock */
ZFUNC ZINT Zposx_SharexUnlock(ZSHAREX *pzSharex);

/* lock a shared-exclusive lock */
ZFUNC ZINT Zposx_SharexLockEx(ZSHAREX *pzSharex);

/* unlock a shared-exclusive lock */
ZFUNC ZINT Zposx_SharexUnlockEx(ZSHAREX *pzSharex);

/* mac osx create some semaphores */
ZFUNC ZINT Zposx_SemCreate(ZSEM *pzSem, ZUINT iValue);

/* mac osx create only one semaphore */
ZFUNC ZINT Zposx_SemCreateOne(ZSEM *pzSem);

/* mac osx delete a semaphore */
ZFUNC ZVOID Zposx_SemDelete(ZSEM *pzSem);

/* mac osx delete a semaphore, count -- if count == 0 then block current task */
ZFUNC ZINT Zposx_SemWait(ZSEM *pzSem, ZUINT iMilliSeconds);

/* mac osx give a semaphore, count++ ,wake up current task */
ZFUNC ZINT Zposx_SemPost(ZSEM *pzSem);

#ifdef __cplusplus
}
#endif

#endif /* _ZPOSX_MUTEX_H__ */

