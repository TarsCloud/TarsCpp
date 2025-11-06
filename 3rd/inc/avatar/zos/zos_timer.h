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
  File name     : zos_timer.h
  Module        : zos timer library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the timer interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_TIMER_H__
#define _ZOS_TIMER_H__

/**
 * @file
 * @brief Zos timer.
 *
 * Timer is a specialized type of clock. A timer can be used to control 
 * the sequence of an event or process. It trigger some event when it rearches
 * time interval. 
 * 
 * There are 2 types of timer:
 * -# Cycle: Timer will trigger event after every time interval.
 * -# Non-cycle: Timer will only trigger once when it rearched time interval.
 *
 * Event triggered by timer can be invoke user defined action callback.
 * If there is no callback assigned, it will send timer owner task a timer
 * evnet.
 * -# Send task ID is ZTASKID_TIMER.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
/* timer mode */
#define ZTIMER_MODE_CYCLE    0x01    /**< @brief Cycle timer. */    
#define ZTIMER_MODE_NOCYCLE  0x02    /**< @brief Non-cycle timer. */
#define ZTIMER_MODE_PURGE    0x04    /**< @brief Purge after timeout timer. */
/** @} */
    
/** @brief User defined callback function type when timer triggers. */
typedef ZVOID (*PFN_ZTIMERACTIVE)(ZTIMERID zTimerId, ZUINT iTimerType, 
                ZCOOKIE zTimerParm);

/**
 * @brief Create a new timer.
 *
 * @param [in] zTaskId Timer owner's task ID.
 * @param [in] ucMode Timer mode, ZTIMER_MODE_CYCLE or ZTIMER_MODE_NOCYCLE.
 * @param [out] pzTimerId Timer ID.
 *
 * @retval ZOK Timer create successfully.
 * @retval ZFAILED Timer create failed.
 *
 * @see Zos_TimerDelete
 */
ZFUNC ZINT Zos_TimerCreate(ZTASKID zTaskId, ZUCHAR ucMode, 
                ZTIMERID *pzTimerId);

/**
 * @brief Create a new timer.
 *
 * @param [in] zTaskId Timer owner's task ID.
 * @param [in] ucMode Timer mode, ZTIMER_MODE_CYCLE or ZTIMER_MODE_NOCYCLE.
 * @param [in] iTimerType User defined timer type.
 * @param [in] zTimerParm User defined parameter.
 * @param [in] pfnActive User defined callback function when timer triggers.
 * @param [out] pzTimerId Timer ID.
 *
 * @retval ZOK Timer create successfully.
 * @retval ZFAILED Timer create failed.
 *
 * @see Zos_TimerDelete
 */
ZFUNC ZINT Zos_TimerCreateX(ZTASKID zTaskId, ZUCHAR ucMode, 
                ZUINT iTimerType, ZCOOKIE zTimerParm, 
                PFN_ZTIMERACTIVE pfnActive, ZTIMERID *pzTimerId);

/**
 * @brief Delete a timer.
 *
 * @param [in] zTimerId Timer ID.
 *
 * @retval ZOK Timer delete successfully.
 * @retval ZFAILED Timer delete failed.
 *
 * @see Zos_TimerCreate
 */
ZFUNC ZINT Zos_TimerDelete(ZTIMERID zTimerId);

/**
 * @brief Start a timer.
 *
 * @param [in] zTimerId Timer ID.
 * @param [in] iTimerType User defined timer type.
 * @param [in] iTimeLen Time interval in milliseconds.
 * @param [in] zTimerParm User defined parameter.
 * @param [in] pfnActive User defined callback function when timer triggers.
 *
 * @retval ZOK Timer start successfully.
 * @retval ZFAILED Timer start failed.
 *
 * @note Timer must not in running state.
 *
 * @see Zos_TimerStop
 */
ZFUNC ZINT Zos_TimerStart(ZTIMERID zTimerId, ZUINT iTimerType, 
                ZUINT iTimeLen, ZCOOKIE zTimeParm, 
                PFN_ZTIMERACTIVE pfnActive);

/**
 * @brief Start a timer.
 *
 * @param [in] zTimerId Timer ID.
 * @param [in] iTimeLen Time interval in milliseconds.
 *
 * @retval ZOK Timer start successfully.
 * @retval ZFAILED Timer start failed.
 *
 * @note Timer must not in running state.
 *
 * @see Zos_TimerStop
 */
ZFUNC ZINT Zos_TimerStartX(ZTIMERID zTimerId, ZUINT iTimeLen);

/**
 * @brief Stop a timer.
 *
 * @param [in] zTimerId Timer ID.
 *
 * @retval ZOK Timer stop successfully.
 * @retval ZFAILED Timer stop failed.
 *
 * @see Zos_TimerStart
 */
ZFUNC ZINT Zos_TimerStop(ZTIMERID zTimerId);

/**
 * @brief Check if a timer is running.
 *
 * @param [in] zTimerId Timer ID.
 *
 * @retval ZTRUE Timer has been started and still in counting.
 * @retval ZFALSE Otherwise.
 *
 * @see Zos_TimerStart
 */
ZFUNC ZBOOL Zos_TimerIsRun(ZTIMERID zTimerId);

/**
 * @brief Get the timer status.
 *
 * @param [in] zTimerId Timer ID.
 * @param [out] pbIsRun If ZTRUE, timer has been started and still in counting.
 * @param [out] piLeftTime Time left before triggered in milliseconds.
 *
 * @retval ZOK Get status successfully.
 * @retval ZFAILED Get status failed.
 *
 * @see Zos_TimerStart
 */
ZFUNC ZINT Zos_TimerGetStatus(ZTIMERID zTimerId, ZBOOL *pbIsRun, 
                ZUINT *piLeftTime);

/**
 * @brief Get ZOS timer manager task ID.
 *
 * @retval ZOS timer manager task ID, return ZMAXTASKID 
 * if timer task has not started.
 */
ZFUNC ZTASKID Zos_TimerGetTaskId(ZFUNC_VOID);

/**
 * @brief Get ZOS timer id of timer event.
 *
 * @param [in] zEvntId Event ID.
 *
 * @retval Timer parameter of a valid timer event, otherwise return ZMAXTIMERID.
 */
ZFUNC ZTIMERID Zos_TimerEvntGetId(ZEVNT zEvntId);

/**
 * @brief Get ZOS timer type of timer event.
 *
 * @param [in] zEvntId Event ID.
 *
 * @retval Timer type of a valid timer event, otherwise return ZMAXUINT.
 */
ZFUNC ZUINT Zos_TimerEvntGetType(ZEVNT zEvntId);

/**
 * @brief Get ZOS timer parameter of timer event.
 *
 * @param [in] zEvntId Event ID.
 *
 * @retval Timer parameter of a valid timer event, otherwise return 0.
 */
ZFUNC ZCOOKIE Zos_TimerEvntGetParm(ZEVNT zEvntId);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_TIMER_H__ */

