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
  File name     : zos_fsm.h
  Module        : zos finite state machine (fsm) library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-05-19    
  Description   :
      Data structure and function definitions required by fsm
    
  Modify History:   
  1. Date:        Author:         Modification:
     
*************************************************/
#ifndef _ZOS_FSM_H__
#define _ZOS_FSM_H__

/**
 * @file
 * @brief Zos finite state machine (fsm) interfaces.
 *
 * Zos fsm is a behavior model composed of a finite number of states, 
 * transitions between those states, and actions. All these information
 * arranged in @ref ST_ZOS_FSM structure.
 *
 * @code
 fsm's structure model:
 
 fsm                  state table            action table in state
 +--------------+     +----------------+     +-----------------------+ 
 | fsm pool     | --> | state_1 bucket | --> | action_1 | next state | 
 | pool size    |     |----------------|     |-----------------------|       
 | state offset |     | state_2 bucket |     | action_2 | next state |      
 +--------------+     |----------------|     |-----------------------|
                      |      ...       |     |    ...   |   ...      |
                      |----------------|     |-----------------------|
                      | state_n bucket |     | action_n | next state |
                      +----------------+     +-----------------------+ 
 * @endcode
 *
 * @section exp_fsm A example of fsm structure
 * @code
 typedef enum EN_EXP_STATE_TYPE
 {
     EN_EXP_STATE_0 = 1,
     EN_EXP_STATE_1 = 2,
     EN_EXP_STATE_2 = 3,
     EN_EXP_STATE_3 = 4,
 } EN_EXP_STATE_TYPE;
 
 static ST_ZOS_FSM_STA m_astSta0[] =
 {
     {ZFSM_BKT_STA(EN_EVENT_MAJOR_1, EN_EVENT_MINOR_1, Exp_Sta0OnEvent1, EN_EXP_STATE_1)},    
 };
 
 static ST_ZOS_FSM_STA m_astSta1[] =
 {
     {ZFSM_BKT_STA(EN_EVENT_MAJOR_1, EN_EVENT_MINOR_1, Exp_Sta1OnEvent1, EN_EXP_STATE_2)},
     {ZFSM_BKT_STA(EN_EVENT_MAJOR_2, EN_EVENT_MINOR_2, Exp_Sta1OnEvent2, EN_EXP_STATE_3)},
 };
 
 static ST_ZOS_FSM_STA m_astSta2[] =
 {
     {ZFSM_BKT_STA(EN_EVENT_MAJOR_3, EN_EVENT_MINOR_3, Exp_Sta2OnEvent3, ZFSM_STA_NOCHANGE)},
 };
 
 static ST_ZOS_FSM_STA m_astSta3[] =
 {
     {ZFSM_BKT_STAX(ZNULL, ZFSM_STA_INVALID)}
 };
 
 ST_ZOS_FSM_BKT g_astFsmPool[] =
 {
     {ZFSM_POOL_BKTX_DESC(m_astSta0, "STATE0")},
     {ZFSM_POOL_BKTX_DESC(m_astSta1, "STATE1")},
     {ZFSM_POOL_BKTX_DESC(m_astSta2, "STATE2")},
     {ZFSM_POOL_BKTX_DESC(m_astSta3, "STATE3")},
 };
 
 ST_ZOS_FSM g_stExpFsm =
 {
     ZFSM_POOL(g_astFsmPool, "FSM DESCRIPTION", "OBJECT DESCRIPTION")
 };
 @endcode
 *
 * The current state was store in user defined variable. So the next state 
 * not only defined in ST_ZOS_FSM_STA structure, but also 
 * can be setted in action function.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Fsm action function type. */
typedef ZINT (*PFN_ZFSMACTION)(ZVOID *, ZVOID *);

/** @brief Fsm lcoate action function type. */
typedef ZINT (*PFN_ZFSMLOCATE)(ZVOID *pFsmObj, ZINT iMajorEvnt, 
                ZINT iMinorEvnt);

/** @brief Action return value which indicate fsm state should be unchanged. */
#define ZFSM_STA_NOCHANGE    0

/** @brief Invalid input event. Fsm state will be unchanged. */
#define ZFSM_STA_INVALID     -1

/** @brief Error occured during process. Fsm state will be unchanged. */
#define ZFSM_STA_ERROR       -2

/** @brief Ignore input event. Fsm state will be unchanged. */
#define ZFSM_STA_IGNEVNT     -3

/* fsm unknown event */
#define ZFSM_EVNT_UNKNOWN    (ZMAXINT - 1) /**< @brief Unknown event. */
#define ZFSM_EVNT_IGNORE      ZMAXINT /**< @brief Ignored event. */

/* fsm decode function return failed */
#define ZFSM_LOCATE_FAIL     -1      /**< @brief Locate action failed. */
#define ZFSM_LOCATE_IGNEVNT  -2      /**< @brief Ignored event. */
#define ZFSM_LOCATE_UNKNOWN  -3      /**< @brief Unknown event. */

#define ZFSM_OK                  0   /**< @brief Action run ok. */
#define ZFSM_IGNORE_EVNT         1   /**< @brief Action ignore event. */

#define ZFSM_FAIL                -1  /**< @brief Action fsm run failed. */
#define ZFSM_ERR_UNKNOWN_STA     -2  /**< @brief Unknown state error. */
#define ZFSM_ERR_UNKNOWN_EVNT    -3  /**< @brief Unknown event error. */
#define ZFSM_ERR_INVALID_EVNT    -4  /**< @brief Invalid event error. */
#define ZFSM_ERR_ERROR_EVNT      -5  /**< @brief Error event format. */
#define ZFSM_ERR_UNEXPECTED_EVNT -6  /**< @brief Unexpected event error. */
#define ZFSM_ERR_DATA_ERROR      -7  /**< @brief Object or event data error. */

/** @brief Default fsm pool structure macro. */
#define ZFSM_POOL(_bkttab, _fdesc, _odesc) \
    _bkttab, ZOS_GET_TABLE_SIZE(_bkttab), ZFALSE, ZFALSE, _fdesc, _odesc

/** @brief Fsm pool structure macro. */
#define ZFSM_POOLX(_bkttab, _smjset, _smiset, _fdesc, _odesc) \
    _bkttab, ZOS_GET_TABLE_SIZE(_bkttab), _smjset, _smiset, _fdesc, _odesc

/** @brief Fsm pool bucket structure macro. */
#define ZFSM_POOL_BKT(_statab, _locate) \
    _statab, (PFN_ZFSMLOCATE)_locate, ZNULL, ZOS_GET_TABLE_SIZE(_statab)

/** @brief Fsm pool bucket structure macro with description. */
#define ZFSM_POOL_BKT_DESC(_statab, _locate, _desc) \
    _statab, (PFN_ZFSMLOCATE)_locate, _desc, ZOS_GET_TABLE_SIZE(_statab)

/** @brief Fsm pool one bucket with no locate and state description */
#define ZFSM_POOL_BKTX(_statab) \
    _statab, ZNULL, ZNULL, ZOS_GET_TABLE_SIZE(_statab)

/** @brief Fsm pool one bucket with no locate, but it has description */
#define ZFSM_POOL_BKTX_DESC(_statab, _desc) \
    _statab, ZNULL, _desc, ZOS_GET_TABLE_SIZE(_statab)

/** @brief Fsm bucket map an event-state, 
   state map would be located by event if locate is null */
#define ZFSM_BKT_STA(_mjevnt, _mievnt, _action, _nextsta) \
    (PFN_ZFSMACTION)_action, ZNULL, _mjevnt, _mievnt, _nextsta

/** @brief Fsm bucket map an event-state, 
   state map would be located by event if locate is null */
#define ZFSM_BKT_STA_DESC(_mjevnt, _mievnt, _action, _nextsta, _desc) \
    (PFN_ZFSMACTION)_action, _desc, _mjevnt, _mievnt, _nextsta

/** @brief Fsm bucket map an event-state
   state map would be located by locate, ignored if locate is null */
#define ZFSM_BKT_STAX(_action, _nextsta) \
    (PFN_ZFSMACTION)_action, ZNULL, ZFSM_EVNT_IGNORE, ZFSM_EVNT_IGNORE, _nextsta

/** @brief Fsm bucket map an event-state
   state map would be located by locate, ignored if locate is null */
#define ZFSM_BKT_STAX_DESC(_action, _nextsta, _desc) \
    (PFN_ZFSMACTION)_action, _desc, ZFSM_EVNT_IGNORE, ZFSM_EVNT_IGNORE, _nextsta
    
/** @brief Fsm bucket map an event-state but ignore action,
   it would be located by event if locate is null */
#define ZFSM_BKT_STA_IGN(_mjevnt, _mievnt) \
    ZNULL, ZNULL, _mjevnt, _mievnt, ZFSM_STA_IGNEVNT

/** @brief Fsm bucket map an event-state but ignore action,
   it would be located by event if locate is null */
#define ZFSM_BKT_STA_IGN_DESC(_mjevnt, _mievnt, _desc) \
    ZNULL, _desc, _mjevnt, _mievnt, ZFSM_STA_IGNEVNT

/** @brief Fsm bucket map an event-state and state needn't change, 
   state map would be located by event if locate is null */
#define ZFSM_BKT_STA_NOCHG(_mjevnt, _mievnt, _action) \
    (PFN_ZFSMACTION)_action, ZNULL, _mjevnt, _mievnt, ZFSM_STA_NOCHANGE

/** @brief Fsm bucket map an event-state and state needn't change, 
   state map would be located by event if locate is null */
#define ZFSM_BKT_STA_NOCHG_DESC(_mjevnt, _mievnt, _action, _desc) \
    (PFN_ZFSMACTION)_action, _desc, _mjevnt, _mievnt, ZFSM_STA_NOCHANGE

/****************************************************************
  fsm's structure model:
   
     fsm                 fsm pool               fsm state table        
 +--------------+     +----------------+     +-----------------------+ 
 | fsm pool     | --> | state_1 bucket | --> | action_1 | next state | 
 | pool size    |     |----------------|     |-----------------------|       
 | state offset |     | state_2 bucket |     | action_2 | next state |      
 +--------------+     |----------------|     |-----------------------|
                      |      ...       |     |    ...  |   ...       |
                      |----------------|     |-----------------------|
                      | state_n bucket |     | action_n | next state |
                      +----------------+     +-----------------------+ 
                                                                                  
****************************************************************/

/** 
 * @brief Fsm state structure.
 * 
 * Major and minor event only valid for null locate function.
 * If major and minor events are all ZFSM_STA_IGNEVNT, step will be ignored.
 * If major or minor event is ZFSM_STA_IGNEVNT, event will be ignored
 * if next state is ZFSM_STA_IGNEVNT, step action will be ignored
 */
typedef struct tagZOS_FSM_STA
{
    PFN_ZFSMACTION pfnAction;        /**< @brief State action. */
    ZCONST ZCHAR *pcEvntDesc;        /**< @brief Event description. */
    ZINT iMajorEvnt;                 /**< @brief Major event. */
    ZINT iMinorEvnt;                 /**< @brief Minor event. */
    ZINT iNextSta;                   /**< @brief Next state in normal status. */
    ZOS_PADX64
} ST_ZOS_FSM_STA;

/** @brief Fsm state bucket, if locate is null, it will compare event value. */
typedef struct tagZOS_FSM_BKT
{
    ZCONST ST_ZOS_FSM_STA *pstStaTab; /**< @brief State table. */
    PFN_ZFSMLOCATE pfnLocate;        /**< @brief State entry locate fucntion. */
    ZCONST ZCHAR *pcStaDesc;         /**< @brief State description */
    ZINT iStaTabSize;                /**< @brief State table size. */
    ZOS_PADX64
} ST_ZOS_FSM_BKT;

/** @brief Fsm pool sturcture.
 * 
 * If bSupMajorSet or bSupMinorSet is on and state locate function is not null, 
 * fsm run will try to match event-state(besides ZMAXINT) by ZOS_ISSET,
 * event id should be defined as 2 power value, such as 1, 2, 4, 8...
 */
typedef struct tagZOS_FSM
{
    ZCONST ST_ZOS_FSM_BKT *pstFsmPool; /**< @brief Fsm state bucket pool. */
    ZINT iMaxSta;                    /**< @brief Maximum state value. */
    ZUSHORT wSupMajorSet;            /**< @brief Major event match by ZOS_ISSET. */
    ZUSHORT wSupMinorSet;            /**< @brief Minor event match by ZOS_ISSET. */
    ZCONST ZCHAR *pcFsmDesc;         /**< @brief Fsm description. */
    ZCONST ZCHAR *pcObjDesc;         /**< @brief Object description. */
} ST_ZOS_FSM;

/**
 * @brief Fsm driver function.
 *
 * @param [in] pstFsm User defined fsm structure.
 * @param [in] pFsmObj User defined object for fsm action.
 * @param [in] zCookie User defined object ID for log information.
 * @param [in,out] piSta Fsm state value. After return it contains next state.
 *                       This value must start from 1.
 * @param [in] pParm User defined parameter for fsm action.
 * @param [in] iMajorEvnt Major event ID.
 * @param [in] iMinorEvnt Minor event ID.
 * @param [in] pcEvntDesc Event description for log information.
 *
 * @retval ZFSM_OK Action run ok.
 * @retval ZFSM_IGNORE_EVNT Action ignore event.
 * @retval ZFSM_FAIL Action fsm run failed.
 * @retval ZFSM_ERR_UNKNOWN_STA Unknown state error.
 * @retval ZFSM_ERR_UNKNOWN_EVNT Unknown event error.
 * @retval ZFSM_ERR_INVALID_EVNT Invalid event error.
 * @retval ZFSM_ERR_ERROR_EVNT Error event format.
 * @retval ZFSM_ERR_UNEXPECTED_EVNT Unexpected event error.
 * @retval ZFSM_ERR_DATA_ERROR Object or event data error.
 *
 * @see @ref Zos_FsmGetStaDesc
 */
ZFUNC ZINT Zos_FsmRun(ZCONST ST_ZOS_FSM *pstFsm, ZVOID *pFsmObj, 
                ZCOOKIE zCookie, ZINT *piSta, ZVOID *pParm, ZINT iMajorEvnt, 
                ZINT iMinorEvnt, ZCONST ZCHAR *pcEvntDesc);

/**
 * @brief Fsm driver function.
 *
 * @param [in] pstFsm User defined fsm structure.
 * @param [in] pFsmObj User defined object for fsm action.
 * @param [in] zCookie User defined object ID for log information.
 * @param [in,out] piSta Fsm state value. After return it contains next state.
 *                       This value must start from 1.
 * @param [in] pParm User defined parameter for fsm action.
 * @param [in] iMajorEvnt Major event ID.
 * @param [in] iMinorEvnt Minor event ID.
 * @param [in] pcEvntDesc Event description for log information.
 * @param [in] bShowLog ZTRUE for log information.
 *
 * @retval ZFSM_OK Action run ok.
 * @retval ZFSM_IGNORE_EVNT Action ignore event.
 * @retval ZFSM_FAIL Action fsm run failed.
 * @retval ZFSM_ERR_UNKNOWN_STA Unknown state error.
 * @retval ZFSM_ERR_UNKNOWN_EVNT Unknown event error.
 * @retval ZFSM_ERR_INVALID_EVNT Invalid event error.
 * @retval ZFSM_ERR_ERROR_EVNT Error event format.
 * @retval ZFSM_ERR_UNEXPECTED_EVNT Unexpected event error.
 * @retval ZFSM_ERR_DATA_ERROR Object or event data error.
 *
 * @see @ref Zos_FsmGetStaDesc
 */
ZFUNC ZINT Zos_FsmRunX(ZCONST ST_ZOS_FSM *pstFsm, ZVOID *pFsmObj, 
                ZCOOKIE zCookie, ZINT *piSta, ZVOID *pParm, ZINT iMajorEvnt, 
                ZINT iMinorEvnt, ZCONST ZCHAR *pcEvntDesc, ZBOOL bShowLog);

/**
 * @brief Get state description.
 *
 * @param [in] pstFsm User defined fsm structure.
 * @param [in] iSta Fsm state value.
 *
 * @return State description string.
 *
 * @see @ref Zos_FsmRun
 */
ZFUNC ZCONST ZCHAR * Zos_FsmGetStaDesc(ZCONST ST_ZOS_FSM *pstFsm, ZINT iSta);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_FSM_H__ */

