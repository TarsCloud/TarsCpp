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
  File name     : zos_module.h
  Module        : zos module management 
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2005-04-16    
  Description   :
      Marcos definitions required by the zos system management.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_MODULE_H__
#define _ZOS_MODULE_H__

/**
 * @file
 * @brief Zos module interfaces.
 *
 * Zos module is derive @ref zos_task.h "Zos task". Zos module take over
 * the entry function of task. User can focus on task @ref zos_msg.h "message"
 * process.
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Callback function of module task init. */
typedef ZINT (*PFN_ZMODINIT)(ZCONST ZCHAR *pcName, ZTASKID zTaskId);

/** @brief Callback function of module task destroy. */
typedef ZVOID (*PFN_ZMODDESTROY)(ZCONST ZCHAR *pcName, ZTASKID zTaskId);

/** @brief Callback function of module task entry. */
typedef ZINT (*PFN_ZMODENTRY)(ZMSG zMsgId);

/** @brief Callback function of wait module event. */
typedef ZINT (*PFN_ZMODWAITER)(ZEVNT zEvntId, ZVOID *pData, ZUINT iLen);

/** @brief Callback function of module task perform. */
typedef ZVOID (* PFN_ZMODPERFORM)();

/** @brief Start a module task */
#define ZOS_MOD_START(_taskid, _name, _pri, _qsize, _ssize, _init, _destroy, _entry) \
    Zos_ModTaskStart(_name, _pri, _qsize, _ssize, ZTRUE, (PFN_ZMODINIT)_init, \
                     (PFN_ZMODDESTROY)_destroy, (PFN_ZMODENTRY)_entry, &_taskid)

/** @brief Start a module task which specific stack size */
#define ZOS_MOD_STARTX(_taskid, _name, _pri, _qsize, _ssize, _selfdrv, _init, _destroy, _entry) \
    Zos_ModTaskStart(_name, _pri, _qsize, _ssize, _selfdrv, (PFN_ZMODINIT)_init, \
                     (PFN_ZMODDESTROY)_destroy, (PFN_ZMODENTRY)_entry, &_taskid)

/** @brief Stop a module task */
#define ZOS_MOD_STOP(_taskid) Zos_ModTaskStop(_taskid)

/** @brief check one module task is running */
#define ZOS_MOD_ISRUN(_name) Zos_ModTaskIsRun(_name)

/**
 * @brief Drive message queue in module manager.
 *
 * @retval ZOK Drive module manager successfully.
 * @retval ZFAILED Drive module manager failed.
 *
 * @note It is only valid if Zos_CfgGetModDrvMode is ZFALSE,
 * otherwise message queue is driven by module manager thread.
 *
 * @see Zos_ModIsDriven
 */
ZFUNC ZINT Zos_ModDriveMsg(ZFUNC_VOID);

/**
 * @brief Let module manager keep silence.
 *
 * @retval ZOK Stop module task successfully.
 * @retval ZFAILED Stop module task failed.
 *
 * @note When Zos_CfgGetModDrvMode is ZFALSE, all module tasks were
 * driven by user applicaiton. Mostly when user send a event to a module task,
 * module manager will drive message queue right now.
 * Zos_ModSilence just tell module manager don't drive even if exist event in
 * message queue. User will call Zos_ModDriveMsg after event action.
 * 
 * @see Zos_ModDriveMsg
 */
ZFUNC ZINT Zos_ModSilence(ZFUNC_VOID);

/**
 * @brief Register a new module with dynamic assigned task ID.
 *
 * @param [in] pcName Task name.
 * @param [in] iPriority Task priority.
 * @param [in] iQueueSize The max count of message queue.
 * @param [in] iStackSize Stack size in bytes.
 * @param [in] bSelfDrv Task self-driven, otherwise it depend on outer driven.
 * @param [in] pfnInit Callback function when task initialized.
 * @param [in] pfnDestroy Callback function when task destroied.
 * @param [in] pfnEntry Callback function task entry.
 * @param [out] pzTaskId New task ID assigned.
 *
 * @retval ZOK Module task start successfully.
 * @retval ZFAILED Module task start failed.
 *
 * @note It will not create a new thread if Zos_CfgGetModDrvMode is ZFALSE
 * The module task will drived by Zos_ModDriveMsg by user own.
 * If iQueueSize is 0, task will share stack and queue of manager thread
 *
 * @see Zos_ModTaskStop
 */
ZFUNC ZINT Zos_ModTaskStart(ZCONST ZCHAR *pcName, ZINT iPriority, ZINT iQueueSize,
                ZINT iStackSize, ZBOOL bSelfDrv, PFN_ZMODINIT pfnInit,
                PFN_ZMODDESTROY pfnDestroy, PFN_ZMODENTRY pfnEntry,
                ZTASKID *pzTaskId);

/**
 * @brief Stop a module task.
 *
 * @param [in] zTaskId Task ID.
 *
 * @retval ZOK Stop module task successfully.
 * @retval ZFAILED Stop module task failed.
 *
 * @see Zos_ModTaskStart
 */
ZFUNC ZINT Zos_ModTaskStop(ZTASKID zTaskId);

/**
 * @brief Cease a module task.
 *
 * @param [in] zTaskId Task ID.
 *
 * @retval ZOK Stop module task successfully.
 * @retval ZFAILED Stop module task failed.
 *
 * @note it just notify task wait delete state if task in running. 
 *
 * @see Zos_ModTaskStop
 */
ZFUNC ZINT Zos_ModTaskCease(ZTASKID zTaskId);

/**
 * @brief Pause or resume a module task.
 *
 * @param [in] zTaskId Task ID.
 * @param [in] bPause ZTRUE to pause task process, ZFALSE to resume task process.
 *
 * @retval ZOK Pause or resume module task successfully.
 * @retval ZFAILED Pause or resume module task failed.
 *
 * @see
 */
ZFUNC ZINT Zos_ModTaskPause(ZTASKID zTaskId, ZBOOL bPause);

/**
 * @brief Wait for stop a module task unitil the limited seconds.
 *
 * @param [in] zTaskId Task ID.
 * @param [in] iSeconds Wait for module task was delete second time.
 *
 * @retval ZOK Stop module task successfully.
 * @retval ZFAILED Stop module task failed.
 *
 * @see Zos_ModTaskStart
 */
ZFUNC ZINT Zos_ModTaskWaitStop(ZTASKID zTaskId, ZUINT iSeconds);

/**
 * @brief Check a module task is runing.
 *
 * @param [in] pcName Task name.
 *
 * @retval ZTRUE if task in runing, otherwise return ZFALSE.
 *
 * @see Zos_ModTaskStart
 */
ZFUNC ZBOOL Zos_ModTaskIsRun(ZCONST ZCHAR *pcName);

/**
 * @brief Lock a module task resoruce.
 *
 * @param [in] zTaskId Task ID.
 *
 * @retval ZOK Lock module task successfully.
 * @retval ZFAILED Lock module task failed.
 *
 * @see Zos_ModTaskUnlock
 */
ZFUNC ZINT Zos_ModTaskLock(ZTASKID zTaskId);

/**
 * @brief Unlock a module task resoruce.
 *
 * @param [in] zTaskId Task ID.
 *
 * @retval ZOK Unlock module task successfully.
 * @retval ZFAILED Unlock module task failed.
 *
 * @see Zos_ModTaskLock
 */
ZFUNC ZINT Zos_ModTaskUnlock(ZTASKID zTaskId);

/**
 * @brief Leave a event to a module task waiter.
 *
 * @param [in] pfnWaiter Task event waiter callback.
 * @param [in] zEvntId The event id.
 * @param [in] pData The event data.
 * @param [in] iLen The event data length.
 *
 * @retval ZOK Leave a event successfully.
 * @retval ZFAILED Leave a event failed.
 *
 * @note Leave event is not same with send message to Zos_MsgSend.
 * In the single thread model, a module task mayb process in message queue,
 * user can using Leave event function for avoid recursive call message event,
 * it maybe not safe if module task process message too many recursive.
 * So leave event just tell module manager, the message process has not finish,
 * please process another event after task's message queue process procedure.
 *
 * @see Zos_ModTaskIsRun
 */
ZFUNC ZINT Zos_ModLeaveEvnt(ZTASKID zTaskId, PFN_ZMODWAITER pfnWaiter,
                ZEVNT zEvntId, ZVOID *pData, ZUINT iLen);

/**
 * @brief Send the function to the thread(task id) to be performed.
 *
 * @param [in] zTaskId The event id.
 * @param [in] pfnPerform The function to be performed.
 * @param [in] pcFmt The formatted string.
 *
 * @retval ZOK perform a function successfully.
 * @retval ZFAILED perform a funtion failed.
 *
 * @see Zos_ModPerformX
 */
 
ZFUNC ZINT Zos_ModPerform(ZTASKID zTaskId, PFN_ZMODPERFORM pfnPerform,
                ZCONST ZCHAR *pcFmt, ...);

/**
 * @brief Send the function to the thread(task id) to be performed.
 *
 * @param [in] zTaskId The event id.
 * @param [in] pfnPerform The function to be performed.
 * @param [in] pcFmt The formatted string.
 *
 * @retval ZOK perform a function successfully.
 * @retval ZFAILED perform a funtion failed.
 *
 * @see Zos_ModPerform
 */
ZFUNC ZINT Zos_ModPerformX(ZTASKID zTaskId, PFN_ZMODPERFORM pfnPerform, 
                ZCONST ZCHAR *pcFmt, ...);

/**
 * @brief Subscribe for event.
 * @param  pcEvntName The event name string, it must defined as "ZCONST ZCHAR * ZCONST"
 * @param  zTaskId    The task ID where the notification posted to.
 * @param  zCookie    The user cookie value.
 * @param  pfnPerform The notification process function.
 * @retval ZOK Subscribe event successfully.
 * @retval ZFAILED Subscribe event failed.
 */
ZFUNC ZINT Zos_ModSubEvnt(ZCONST ZCHAR *pcEvntName, ZTASKID zTaskId,
                ZCOOKIE zCookie, PFN_ZMODPERFORM pfnPerform);

/**
 * @brief Un-subscribe for event.
 * @param  pcEvntName The event name string, same value with @ref Zos_ModSubEvnt.
 * @param  zTaskId    The task ID where the notification posted to,
 *                    same value with @ref Zos_ModSubEvnt.
 * @param  zCookie    The user cookie value, same value with @ref Zos_ModSubEvnt.
 * @param  pfnPerform The notification process function, same value with @ref Zos_ModSubEvnt.
 * @retval ZOK Un-subscribe event successfully.
 * @retval ZFAILED Un-subscribe event failed.
 */
ZFUNC ZINT Zos_ModUnSubEvnt(ZCONST ZCHAR *pcEvntName, ZTASKID zTaskId,
                ZCOOKIE zCookie, PFN_ZMODPERFORM pfnPerform);

/**
 * @brief Publish event.
 * @param pcEvntName The event name string.
 * @param pcFmt The format string for following parameters. Use ZNULL when there is no
 *              additional parameters.
 * @retval ZOK Publish event successfully.
 * @retval ZFAILED Publish event failed.
 */
ZFUNC ZINT Zos_ModPubEvnt(ZCONST ZCHAR *pcEvntName, ZCONST ZCHAR *pcFmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_MODULE_H__ */

