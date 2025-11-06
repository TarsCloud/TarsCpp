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
  File name     : zpios_mutex.h
  Module        : ios mutex library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by mutex 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPIOS_MUTEX_H__
#define _ZPIOS_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ios create a mutex */
ZFUNC ZINT Zpios_MutexCreate(ZMUTEX *pzMutex);

/* ios delete a mutex */
ZFUNC ZVOID Zpios_MutexDelete(ZMUTEX *pzMutex);

/* ios lock a mutex */
ZFUNC ZINT Zpios_MutexLock(ZMUTEX *pzMutex);

/* ios unlock a mutex */
ZFUNC ZINT Zpios_MutexUnlock(ZMUTEX *pzMutex);

/* create a shared-exclusive lock */
ZFUNC ZINT Zpios_SharexCreate(ZSHAREX *pzSharex);

/* delete a shared-exclusive lock */
ZFUNC ZVOID Zpios_SharexDelete(ZSHAREX *pzSharex);

/* lock a shared-exclusive lock */
ZFUNC ZINT Zpios_SharexLock(ZSHAREX *pzSharex);

/* unlock a shared-exclusive lock */
ZFUNC ZINT Zpios_SharexUnlock(ZSHAREX *pzSharex);

/* lock a shared-exclusive lock */
ZFUNC ZINT Zpios_SharexLockEx(ZSHAREX *pzSharex);

/* unlock a shared-exclusive lock */
ZFUNC ZINT Zpios_SharexUnlockEx(ZSHAREX *pzSharex);

/* ios create some semaphores */
ZFUNC ZINT Zpios_SemCreate(ZSEM *pzSem, ZUINT iValue);

/* ios create only one semaphore */
ZFUNC ZINT Zpios_SemCreateOne(ZSEM *pzSem);

/* ios delete a semaphore */
ZFUNC ZVOID Zpios_SemDelete(ZSEM *pzSem);

/* ios delete a semaphore, count -- if count == 0 then block current task */
ZFUNC ZINT Zpios_SemWait(ZSEM *pzSem, ZUINT iMilliSeconds);

/* ios give a semaphore, count++ ,wake up current task */
ZFUNC ZINT Zpios_SemPost(ZSEM *pzSem);

#ifdef __cplusplus
}
#endif

#endif /* _ZPIOS_MUTEX_H__ */

