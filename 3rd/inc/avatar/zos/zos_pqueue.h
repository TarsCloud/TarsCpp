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
  File name     : zos_pqueue.h
  Module        : zos priority queue
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the queue interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_PQUEUE_H__
#define _ZOS_PQUEUE_H__

/**
 * @file zos_pqueue.h
 * @brief Priority queue function interface.
 *
 * In this file, it defined the operation function
 * of zos priority queue, including create, delete, add, poll, 
 * peek, find, get and etc.
 */
 
#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
#define ZPQUEUE_PRIORITY_NONE       0 /**< @brief none priority level */
#define ZPQUEUE_PRIORITY_UNLIMIT    ZMAXINT/**< @brief unlimit priority level */

#define ZPQUEUE_PRIORITY_EMERGENCY  1 /**< @brief emergency priority level */
#define ZPQUEUE_PRIORITY_URGENT     2 /**< @brief urgent priority level */
#define ZPQUEUE_PRIORITY_NORMAL     3 /**< @brief normal priority level */
#define ZPQUEUE_PRIORITY_NON_URGENT 4 /**< @brief non-urgent priority level */
/** @} */

/** @brief Priority queue compare the element,
   pElem1's priority is higher if return ZOK, else return ZFAILED */
typedef ZBOOL (*PFN_ZPQUEUEMATCH)(ZVOID *pElem, ZVOID *pCond);

/** @brief Priority queue id */
typedef ZVOID * ZPQUEUE;

/**
 * @brief This function creates a priority queue.
 *
 * @param [in] bNeedMutex A mutex lock flag.
 * @param [in] bNeedNtfyWait A notify wait flag.
 * @param [in] iMinSize Minimum size of the queue.
 * @param [in] iMaxSize Maximum size of the queue.
 * @param [in] iMaxPriority Max priority level.
 * @param [out] pzQueue A pointer to ZOS queue structure.
 *
 * @retval ZOK Create queue successfully.
 * @retval ZFAILED Create queue failed.
 *
 * @see Zos_PQueueDelete
 */
ZFUNC ZINT Zos_PQueueCreate(ZBOOL bNeedMutex, ZBOOL bNeedNtfyWait, 
                ZUINT iMinSize, ZUINT iMaxSize, ZUINT iMaxPriority,
                ZPQUEUE *pzQueue);

/**
 * @brief This function deletes a priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 *
 * @return [none].
 *
 * @see Zos_PQueueCreate
 */
ZFUNC ZVOID Zos_PQueueDelete(ZPQUEUE zQueue);

/**
 * @brief This function inserts specific element into the priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 * @param [in] iPriority A priority level.
 * @param [in] pElem A element data.
 *
 * @retval ZOK Insert operation successfully.
 * @retval ZFAILED Insert operation failed.
 *
 * @see Zos_PQueueRmv
 */
ZFUNC ZINT Zos_PQueueAdd(ZPQUEUE zQueue, ZUINT iPriority, ZVOID *pElem);

/**
 * @brief This function retrieves and removes the head of priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 * @param [out] iPriority A priority level pointer.
 * @param [out] pElem Element data pointer.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see Zos_PQueueAdd
 */
ZFUNC ZINT Zos_PQueueRmv(ZPQUEUE zQueue, ZUINT iPriority, 
                ZVOID *pElem);

/**
 * @brief This function retrieves(no remove) the head of the priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 * @param [in] bWait Wait timeout value.
 * @param [out] piPriority A priority level pointer.
 * @param [out] ppElem Element data pointer.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see Zos_PQueueFind
 */
ZFUNC ZINT Zos_PQueuePoll(ZPQUEUE zQueue, ZBOOL bWait, 
                ZUINT *piPriority, ZVOID **ppElem);

/**
 * @brief Find element from a priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 * @param [in] iPriority Priority searched for.
 * @param [in] pElem Element data pointer.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see Zos_PQueuePoll
 */
ZFUNC ZBOOL Zos_PQueueFind(ZPQUEUE zQueue, ZUINT iPriority, 
                ZVOID *pElem);

/**
 * @brief Get free element count in priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 * @param [in] pfnMatch Callback of element mathch function.
 * @param [in] pCond Element mathch condition.
 * @param [out] piPriority Element priority.
 * @param [out] ppElem Element data pointer.
 *
 * @return Free element count, or return 0.
 *
 * @see Zos_PQueuePoll
 */
ZFUNC ZBOOL Zos_PQueueFindIf(ZPQUEUE zQueue, PFN_ZPQUEUEMATCH pfnMatch, 
                ZVOID *pCond, ZUINT *piPriority, ZVOID **ppElem);

/**
 * @brief Get busy element count in priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 *
 * @return The busy queue node, or return 0.
 *
 * @see Zos_PQueueGetCount
 */
ZFUNC ZUINT Zos_PQueueGetBusyCount(ZPQUEUE zQueue);

/**
 * @brief Get element count in priority queue.
 *
 * @param [in] zQueue A pointer to ZOS queue structure.
 * @param [out] pdwFreeCount The free queue node count.
 * @param [out] pdwBusyCount The busy queue node count.
 * @param [out] pdwReqCount The requested queue node count.
 * @param [out] pdwRlsCount The released queue node count.
 * @param [out] pdwPeekCount The peek queue node count.
 *
 * @retval ZOK Get statistics count successfully.
 * @retval ZFAILED Get statistics count failed.
 *
 * @see Zos_PQueueGetBusyCount
 */
ZFUNC ZINT Zos_PQueueGetCount(ZPQUEUE zQueue, ZUINT *piFreeCount,
                ZUINT *piBusyCount, ZUINT *piReqCount, ZUINT *piRlsCount,
                ZUINT *piPeekCount);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_PQUEUE_H__ */

