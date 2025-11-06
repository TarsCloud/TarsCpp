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
  File name     : zos_mutex.h
  Module        : zos mutex library functions
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Function implement required by the mutex interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_MUTEX_H__
#define _ZOS_MUTEX_H__

/**
 * @file
 * @brief Zos mutex lock and semaphore.
 *
 * This file provides mutex lock and semaphore interfaces. They are depending
 * on OS.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* semaphore wait timeout macros */
#define ZSEMA_WAIT_FOREVER ZMAXUINT  /**< @brief semaphore wait forever */
#define ZSEMA_NO_WAIT 0              /**< @brief semaphore no wait */

/**
 * @brief Create new a mutex lock.
 *
 * @param [out] pzMutex New mutex lock.
 *
 * @retval ZOK Mutex lock create successfully.
 * @retval ZFAILED Mutex lock create failed.
 *
 * @see Zos_MutexDelete
 */
ZFUNC ZINT Zos_MutexCreate(ZMUTEX *pzMutex);

/**
 * @brief Delete a mutex lock.
 *
 * @param [in] pzMutex Mutex lock address.
 *
 * @return [none].
 *
 * @see Zos_MutexCreate
 */
ZFUNC ZVOID Zos_MutexDelete(ZMUTEX *pzMutex);

/**
 * @brief Lock the mutex lock.
 *
 * @param [in] pzMutex New mutex lock.
 *
 * @retval ZOK Mutex lock successfully.
 * @retval ZFAILED Mutex lock failed.
 *
 * @see Zos_MutexUnlock
 */
ZFUNC ZINT Zos_MutexLock(ZMUTEX *pzMutex);

/**
 * @brief Unlock the mutex lock.
 *
 * @param [in] pzMutex New mutex lock.
 *
 * @retval ZOK Mutex unlock successfully.
 * @retval ZFAILED Mutex unlock failed.
 *
 * @see Zos_MutexLock
 */
ZFUNC ZINT Zos_MutexUnlock(ZMUTEX *pzMutex);

/**
 * @brief Create new a shared-exclusive lock.
 *
 * @param [out] pzSharex New shared-exclusive lock.
 *
 * @retval ZOK Sharex lock create successfully.
 * @retval ZFAILED Sharex lock create failed.
 *
 * @see Zos_SharexDelete
 */
ZFUNC ZINT Zos_SharexCreate(ZSHAREX *pzSharex);

/**
 * @brief Delete a shared-exclusive lock.
 *
 * @param [in] pzSharex Sharex lock address.
 *
 * @return [none].
 *
 * @see Zos_SharexCreate
 */
ZFUNC ZVOID Zos_SharexDelete(ZSHAREX *pzSharex);

/**
 * @brief Lock the shared-exclusive lock.
 *
 * @param [in] pzSharex New shared-exclusive lock.
 *
 * @retval ZOK Sharex lock successfully.
 * @retval ZFAILED Sharex lock failed.
 *
 * @see Zos_SharexUnlock
 */
ZFUNC ZINT Zos_SharexLock(ZSHAREX *pzSharex);

/**
 * @brief Unlock the shared-exclusive lock.
 *
 * @param [in] pzSharex New shared-exclusive lock.
 *
 * @retval ZOK Sharex unlock successfully.
 * @retval ZFAILED Sharex unlock failed.
 *
 * @see Zos_SharexLock
 */
ZFUNC ZINT Zos_SharexUnlock(ZSHAREX *pzSharex);

/**
 * @brief Lock the shared-exclusive lock.
 *
 * @param [in] pzSharex New shared-exclusive lock.
 *
 * @retval ZOK Sharex lock successfully.
 * @retval ZFAILED Sharex lock failed.
 *
 * @see Zos_SharexUnlock
 */
ZFUNC ZINT Zos_SharexLockEx(ZSHAREX *pzSharex);

/**
 * @brief Unlock the shared-exclusive lock.
 *
 * @param [in] pzSharex New shared-exclusive lock.
 *
 * @retval ZOK Sharex unlock successfully.
 * @retval ZFAILED Sharex unlock failed.
 *
 * @see Zos_SharexLock
 */
ZFUNC ZINT Zos_SharexUnlockEx(ZSHAREX *pzSharex);

/**
 * @brief Create new a semaphore.
 *
 * @param [out] pzSem New semaphore.
 * @param [in] iValue Semaphore option.
 *
 * @retval ZOK Semaphore create successfully.
 * @retval ZFAILED Semaphore create failed.
 *
 * @see Zos_SemDelete
 */
ZFUNC ZINT Zos_SemCreate(ZSEM *pzSem, ZUINT iValue);

/**
 * @brief Create new a binary semaphore.
 *
 * @param [out] pzSem New semaphore.
 *
 * @retval ZOK Semaphore create successfully.
 * @retval ZFAILED Semaphore create failed.
 *
 * @see Zos_SemCreate
 */
ZFUNC ZINT Zos_SemCreateOne(ZSEM *pzSem);

/**
 * @brief Delete a semaphore.
 *
 * @param [in] pzSem Semaphore address.
 *
 * @return [none].
 *
 * @see Zos_SemCreate
 */
ZFUNC ZVOID Zos_SemDelete(ZSEM *pzSem);

/**
 * @brief Wait for a semaphore.
 *
 * @param [in] pzSem Semaphore address.
 * @param [in] iMilliSeconds Wait in milliseconds.
 *
 * @retval ZOK Semaphore wait successfully.
 * @retval ZFAILED Invalid semaphore.
 *
 * @see Zos_SemPost
 */
ZFUNC ZINT Zos_SemWait(ZSEM *pzSem, ZUINT iMilliSeconds);

/**
 * @brief Post a semaphore.
 *
 * @param [in] pzSem Semaphore address.
 *
 * @retval ZOK Semaphore post successfully.
 * @retval ZFAILED Invalid semaphore.
 *
 * @see Zos_SemWait
 */
ZFUNC ZINT Zos_SemPost(ZSEM *pzSem);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_MUTEX_H__ */

