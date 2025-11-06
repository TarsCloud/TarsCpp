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
  File name     : zos_fsm_dump.h
  Module        : zos finite state machine (fsm) dump function
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-12-07    
  Description   :
      Data structure and function definitions required by the fsm dump
    
  Modify History:   
  1. Date:        Author:         Modification:
     
*************************************************/
#ifndef _ZOS_FSM_DUMP_H__
#define _ZOS_FSM_DUMP_H__

/**
 * @file
 * @brief Zos dump of finite state machine (fsm).
 *
 * Fsm dump provides a method to log information for debug.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Defualt dump stack size. */
#define ZFSM_DUMP_STACK_SIZE 10

/**
 * @brief Create a fsm dump.
 *
 * @param [in] pcName Dump name.
 * @param [in] wDeepSize Dump stack size.
 *
 * @return New created dump ID if successfully, or return ZNULL.
 *
 * @see 
 */
ZFUNC ZDUMP Zos_FsmDumpCreate(ZCHAR *pcName, ZUSHORT wDeepSize);

/**
 * @brief Delete a fsm dump.
 *
 * @param [in] zDumpId Dump ID.
 *
 * @return [none].
 *
 * @see 
 */
ZFUNC ZVOID Zos_FsmDumpDelete(ZDUMP zDumpId);

/**
 * @brief Add a information record to dump.
 *
 * @param [in] zDumpId Dump ID.
 * @param [in] pcFromState Description of state before process event.
 * @param [in] pcToState Description of state after process event.
 * @param [in] pcEvnt Event description.
 *
 * @retval ZOK Add record successfully.
 * @retval ZFAILED Add record failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_FsmDumpStack(ZDUMP zDumpId, ZCONST ZCHAR *pcFromState, 
                ZCONST ZCHAR *pcToState, ZCONST ZCHAR *pcEvnt);

/**
 * @brief Clear all record in dump.
 *
 * @param [in] zDumpId Dump ID.
 *
 * @retval ZOK Clear record successfully.
 * @retval ZFAILED Clear record failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_FsmDumpClear(ZDUMP zDumpId);

/**
 * @brief Print records in dump.
 *
 * @param [in] zDumpId Dump ID.
 *
 * @retval ZOK Print record successfully.
 * @retval ZFAILED Print record failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_FsmDumpPrint(ZDUMP zDumpId);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_FSM_DUMP_H__ */

