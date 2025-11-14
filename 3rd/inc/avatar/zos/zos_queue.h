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
  File name     : zos_queue.h
  Module        : zos queue library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the queue interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_QUEUE_H__
#define _ZOS_QUEUE_H__

/**
 * @file zos_queue.h
 * @brief Queue function interface.
 *
 * In this file, it defined the operation function
 * of zos queue, including create, delete, add, poll, 
 * peek, get and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif
    
/** @brief Queue id. */
typedef ZVOID * ZQUEUEID;

/**
 * @brief This function Creates a queue.
 *
 * @param [in] bNeedMutex A mutex lock flag.
 * @param [in] bNeedNtfyWait A notify wait flag.
 * @param [in] iMinSize Minimum size of the queue.
 * @param [in] iMaxSize Maximum size of the queue.
 * @param [out] pzQueueId A pointer to ZOS queue structure.
 *
 * @retval ZOK Create queue successfully.
 * @retval ZFAILED Create queue failed.
 *
 * @see Zos_QueueDelete
 */
ZFUNC ZINT Zos_QueueCreate(ZBOOL bNeedMutex, ZBOOL bNeedNtfyWait, 
                ZUINT iMinSize, ZUINT iMaxSize, ZQUEUEID *pzQueueId);

/**
 * @brief This function deletes a queue.
 *
 * @param [in] zQueueId A pointer to ZOS queue structure..
 *
 * @return [none].
 *
 * @see Zos_QueueCreate
 */
ZFUNC ZVOID Zos_QueueDelete(ZQUEUEID zQueueId);

/**
 * @brief This function adds a queue ands set the element data.
 *
 * @param [in] zQueueId A pointer to ZOS queue structure.
 * @param [in] pElem Element data.
 *
 * @retval ZOK Add operation successfully.
 * @retval ZFAILED Add operation failed.
 *
 * @see Zos_QueuePoll
 */
ZFUNC ZINT Zos_QueueAdd(ZQUEUEID zQueueId, ZVOID *pElem);

/**
 * @brief This function retrieves and removes the head of the queue.
 *
 * @param [in] zQueueId A pointer to ZOS queue structure.
 * @param [in] bWait Wait timeout value.
 * @param [out] ppElem Element data pointer.
 *
 * @retval ZOK Poll operation successfully.
 * @retval ZFAILED Poll operation failed.
 *
 * @see Zos_QueueAdd
 */
ZFUNC ZINT Zos_QueuePoll(ZQUEUEID zQueueId, ZBOOL bWait, ZVOID **ppElem);

/**
 * @brief This function retrieves(no remove) the head of the queue.
 *
 * @param [in] zQueueId A pointer to ZOS queue structure.
 * @param [in] bWait Wait timeout value.
 * @param [out] ppElem Element data pointer.
 *
 * @retval ZOK Peek operation successfully.
 * @retval ZFAILED Peek operation failed.
 *
 * @see Zos_QueueAdd
 */
ZFUNC ZINT Zos_QueuePeek(ZQUEUEID zQueueId, ZBOOL bWait, ZVOID **ppElem);

/**
 * @brief Get element count in queue.
 *
 * @param [in] zQueueId A pointer to ZOS queue structure.
 * @param [out] piFreeCount The free queue node count.
 * @param [out] piBusyCount The busy queue node count.
 * @param [out] piReqCount The requested queue node count.
 * @param [out] piRlsCount The released queue node count.
 * @param [out] piPeekCount The peek queue node count.
 *
 * @retval ZOK Get statistics count successfully.
 * @retval ZFAILED Get statistics count failed.
 *
 * @see Zos_QueueGetBusyCount
 */
ZFUNC ZINT Zos_QueueGetCount(ZQUEUEID zQueueId, ZUINT *piFreeCount,
                ZUINT *piBusyCount, ZUINT *piReqCount, 
                ZUINT *piRlsCount, ZUINT *piPeekCount);

/**
 * @brief Get busy element count in queue.
 *
 * @param [in] zQueueId A pointer to ZOS queue structure.
 *
 * @return The busy queue node, or return 0.
 *
 * @see Zos_QueueGetCount
 */
ZFUNC ZUINT Zos_QueueGetBusyCount(ZQUEUEID zQueueId);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_QUEUE_H__ */

