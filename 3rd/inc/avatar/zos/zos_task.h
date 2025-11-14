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
  File name     : zos_task.h
  Module        : zos task library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the task interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_TASK_H__
#define _ZOS_TASK_H__

/**
 * @file
 * @brief Zos task interfaces.
 *
 * Zos task provides OS thread wrapper. Additional, each task contains
 * it own @ref zos_msg.h "message queue" and @ref zos_timer.h "timer"
 * which makes it's easy to communicate between task.
 *
 * These interfaces are thread safe.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
#define ZTASK_PRIORITY_SYS    0      /**< @brief Task system priority */
#define ZTASK_PRIORITY_HIGH   1      /**< @brief Task high priority. */
#define ZTASK_PRIORITY_LOW    8      /**< @brief Task low priority. */
#define ZTASK_PRIORITY_NORMAL 4      /**< @brief Task normal priority. */
#define ZTASK_PRIORITY_ABOVE_NORMAL 3 /**< @brief Task above normal priority. */
#define ZTASK_PRIORITY_BELOW_NORMAL 6 /**< @brief Task below normal priority. */
/** @} */

/** @{ */
#define ZTASK_STACK_SIZE_DEFAULT 0    /**< @brief Use default stack size. */
#if (ZPLATFORM == ZPLATFORM_WIN32 \
     || ZPLATFORM == ZPLATFORM_IOS \
     || ZPLATFORM == ZPLATFORM_ANDROID \
     || ZPLATFORM == ZPLATFORM_OSX)
#define ZTASK_STACK_SIZE_MIN    ZTASK_STACK_SIZE_DEFAULT
#define ZTASK_STACK_SIZE_NORMAL ZTASK_STACK_SIZE_DEFAULT
#define ZTASK_STACK_SIZE_BIG    ZTASK_STACK_SIZE_DEFAULT
#else
#define ZTASK_STACK_SIZE_MIN    0x8000  /**< @brief Task minimum stack size 32k. */
#define ZTASK_STACK_SIZE_NORMAL 0x10000 /**< @brief Task normal stack size 64k. */
#define ZTASK_STACK_SIZE_BIG    0x20000 /**< @brief Task big stack size 128K. */
#endif
/** @} */
    
/** @{ */
#define ZTASK_QUEUE_SIZE_NORMAL 10
/** @} */

/** @brief Task ID of main thread. */
#define ZTASKID_MAIN 0x0F

/** @brief Task state defination. */
typedef enum EN_ZOS_TASK_STATE
{
    EN_ZOS_TASK_STATE_FREE = 0,      /**< @brief Task is in free state. */
    EN_ZOS_TASK_STATE_DELETE,        /**< @brief Task has been deleted. */
    EN_ZOS_TASK_STATE_WAIT_DELETE,   /**< @brief Task is waiting delete. */
    EN_ZOS_TASK_STATE_READY,         /**< @brief Task resource is ready. */
    EN_ZOS_TASK_STATE_PENDING,       /**< @brief Task is in pending state. */
    EN_ZOS_TASK_STATE_RUNNING        /**< @brief Task is running. */
} EN_ZOS_TASK_STATE;

/** @brief Task entry */
typedef struct tagZOS_TASK_ENTRY
{
    ZTASKID zTaskId;                 /**< @brief Task Id. */
    ZUINT iState;                    /**< @brief Task state @see EN_ZOS_TASK_STATE. */
    ZVOID *pEntryParm;               /**< task entry parameter. */
} ST_ZOS_TASK_ENTRY;

/** @{ */
/* zos task entry function type defination. */
typedef ZINT (*PFN_ZTASKENTRY)(ST_ZOS_TASK_ENTRY *pstEntry);
/** @} */

/**
 * @brief Spawn a new task with specific task ID.
 *
 * @param [in] pcName Task name.
 * @param [in] iPriority Task priority.
 * @param [in] iStackSize Stack size in bytes.
 * @param [in] pfnEntry User defined task entry interface. It should check
 *                      if the task should be quit periodically.
 * @param [in] pEntryParm User defined parameter for entry interface.
 *
 * @param [out] pzTaskId Task ID.
 *
 * @retval ZOK Task spawn successfully.
 * @retval ZFAILED Task spawn failed.
 *
 * @see Zos_TaskDelete
 */
ZFUNC ZINT Zos_TaskSpawn(ZCONST ZCHAR *pcName, ZINT iPriority, ZUINT iStackSize, 
                PFN_ZTASKENTRY pfnEntry, ZVOID *pEntryParm, ZTASKID *pzTaskId);

/**
 * @brief Cease a task, just set runing task state to wait delete state.
 *
 * @param [in] zTaskId Task ID.
 *
 * @retval ZOK Task delete successfully.
 * @retval ZFAILED Task delete failed.
 *
 * @note Task should exit its entry when this function set wait delete state.
 *
 * @see Zos_TaskSpawn
 */
ZFUNC ZINT Zos_TaskCease(ZTASKID zTaskId);

/**
 * @brief Delete a task.
 *
 * @param [in] zTaskId Task ID.
 *
 * @retval ZOK Task delete successfully.
 * @retval ZFAILED Task delete failed.
 *
 * @note Task should exit its entry in this function is called.
 *       Or it will be deleted after 6s by force. This may cause unknown state,
 *       some operating systems don't recommend delete thread by force.
 *
 * @see Zos_TaskSpawn
 */
ZFUNC ZINT Zos_TaskDelete(ZTASKID zTaskId);

/**
 * @brief Delay for a specific time.
 *
 * @param [in] iMilliSeconds Time lenght in milliseconds.
 *
 * @return [none].
 *
 * @see 
 */
ZFUNC ZVOID Zos_TaskDelay(ZUINT iMilliSeconds);

/**
 * @brief Wait for a specific task to be deleted.
 *
 * @param [in] zTaskId Task ID.
 * @param [in] iSeconds Wait task to be deletes maximum seconds.
 *
 * @retval ZOK Task be deleted successfully.
 * @retval ZFAILED Task is not exist.
 *
 * @see 
 */
ZFUNC ZINT Zos_TaskWaitDelete(ZTASKID zTaskId, ZUINT iSeconds);

/**
 * @brief Get task name.
 *
 * @param [in] zTaskId Task ID.
 *
 * @return Task name, if task is exist, or return ZNULL.
 *
 * @see 
 */
ZFUNC ZCHAR * Zos_TaskGetName(ZTASKID zTaskId);

/**
 * @brief Get task id.
 *
 * @param [in] pcName Task name.
 *
 * @return Task id, if task is exist, or return ZMAXTASKID.
 *
 * @see 
 */
ZFUNC ZTASKID Zos_TaskGetTaskId(ZCHAR *pcName);

/**
 * @brief Get thead id.
 *
 * @param [in] zTaskId Task Id.
 *
 * @return Thread id, if task is exist, or return ZNULL.
 *
 * @see 
 */
ZFUNC ZTHREAD_ID Zos_TaskGetRealThreadId(ZTASKID zTaskId);

/**
 * @brief Get Current Task Id.
 *
 * @return Task id, if task is exist, or return ZMAXTASKID.
 *
 * @see 
 */
ZFUNC ZTHREAD_ID Zos_TaskGetCurrent();

/**
 * @brief set Current Task Thread Name.
 *
 * @param  [in] pcName The task thread name.
 *
 * @return  [none].
 *
 */
ZFUNC ZVOID Zos_TaskSetThreadName(ZCONST ZCHAR *pcName);

/**
 * @brief Get Current Task Thread Name.
 *
 * @param [out] pcName The task thread name.
 *
 * @return Task Thread Name, if task is exist, or return NULL.
 *
 * @see 
 */
ZFUNC ZCHAR * Zos_TaskGetThreadName(ZCHAR *pcName);


/**
 * @brief Get Current Task Id.
 *
 * @return Task id, if task is exist, or return ZMAXTASKID.
 *
 * @see 
 */
ZFUNC ZINT Zos_TaskInitCrashReport(ZCHAR* pcDmpPath);

/**
 * @brief Get task state.
 *
 * @param [in] zTaskId Task ID.
 *
 * @return Task state EN_ZOS_TASK_STATE, if task is exist, or return -1.
 *
 * @see EN_ZOS_TASK_STATE
 */
ZFUNC ZINT Zos_TaskGetState(ZTASKID zTaskId);

/**
 * @brief Task Info Debug Show.
 *
 * @retval ZOK Task debug show successfully.
 * @retval ZFAILED Task debug show failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_TaskDbgShow(ZINT iArgc, ZCHAR *apcArgv[]);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_TASK_H__ */

