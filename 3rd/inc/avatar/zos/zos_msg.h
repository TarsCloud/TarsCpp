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
  File name     : zos_msg.h
  Module        : zos message library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the message interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_MSG_H__
#define _ZOS_MSG_H__

/**
 * @file zos_msg.h
 * @brief Zos task message.
 *
 * Task message provide a mechanism to exchage data between tasks. Each task
 * has its own message queue. All message send to task will add to this queue.
 *
 * These interfaces are thread safe.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new message memory with event id.
 *
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 *
 * @return New task message id.
 *
 * @see @ref Zos_MsgDelete
 */
ZFUNC ZMSG Zos_MsgCreate(ZTASKID zSendTaskId, ZTASKID zRecvTaskId,
                ZUINT iMsgType);

/**
 * @brief Create a new message memory with event id.
 *
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] pObject The message object.
 * @param [in] pData The message data.
 * @param [in] iDataLen The data length.
 *
 * @return New task message id.
 *
 * @see @ref Zos_MsgDelete
 */
ZFUNC ZMSG Zos_MsgCreateX(ZTASKID zSendTaskId, ZTASKID zRecvTaskId,
                ZUINT iMsgType, ZVOID *pObject, ZVOID *pData, ZUINT iDataLen);

/**
 * @brief Create a new message memory with event id.
 *
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] pObject The object address.
 * @param [in] iDataLen The data length.
 * @param [out] ppData The data address to be stored.
 *
 * @return New task message id.
 *
 * @see @ref Zos_MsgDelete
 */
ZFUNC ZMSG Zos_MsgCreateXD(ZTASKID zSendTaskId, ZTASKID zRecvTaskId,
                ZUINT iMsgType, ZVOID *pObject, ZUINT iDataLen, ZVOID **ppData);

/**
 * @brief Create a new message memory with event id.
 *
 * @param [in] zEvnt Event ID which message will share memory.
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 *
 * @return New task message id.
 *
 * @see @ref Zos_MsgDelete
 */
ZFUNC ZMSG Zos_MsgCreateE(ZEVNT zEvnt, ZTASKID zSendTaskId, 
                ZTASKID zRecvTaskId, ZUINT iMsgType);

/**
 * @brief Create a new message memory with event id.
 *
 * @param [in] zEvnt Event ID which message will share memory.
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] pObject The message object.
 * @param [in] pData The message data.
 * @param [in] iDataLen The data length.
 *
 * @return New task message id.
 *
 * @see @ref Zos_MsgDelete
 */
ZFUNC ZMSG Zos_MsgCreateED(ZEVNT zEvnt, ZTASKID zSendTaskId, 
                ZTASKID zRecvTaskId, ZUINT iMsgType, ZVOID *pObject, 
                ZVOID *pData, ZUINT iDataLen);

/**
 * @brief Delete a system message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return [none].
 *
 * @see @ref Zos_MsgCreate
 */
ZFUNC ZVOID Zos_MsgDelete(ZMSG zMsg);

/**
 * @brief Send the message to destination task.
 *
 * @param [in] zMsg Task message id.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSendX
 */
ZFUNC ZINT Zos_MsgSend(ZMSG zMsg);

/**
 * @brief Send the message to destination task, include message priority.
 *
 * @param [in] zMsg Task message id.
 * @param [in] iPriority Task message priority.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSendXP
 */
ZFUNC ZINT Zos_MsgSendP(ZMSG zMsg, ZINT iPriority);

/**
 * @brief Send message with task id and event id.
 *
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] pObject The message object.
 * @param [in] pData Message data.
 * @param [in] iDataLen Message data length.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSend
 */
ZFUNC ZINT Zos_MsgSendX(ZTASKID zSendTaskId, ZTASKID zRecvTaskId, 
                ZUINT iMsgType, ZVOID *pObject, ZVOID *pData, ZUINT iDataLen);

/**
 * @brief Send message with task id and event id, include message priority.
 *
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] pObject The message object.
 * @param [in] pData Message data.
 * @param [in] iDataLen Message data length.
 * @param [in] iPriority Task message priority.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSendP
 */
 ZFUNC ZINT Zos_MsgSendXP(ZTASKID zSendTaskId, ZTASKID zRecvTaskId, 
                ZUINT iMsgType, ZVOID *pObject, ZVOID *pData, 
                ZUINT iDataLen, ZINT iPriority);

/**
 * @brief Send message with task id and event id.
 *
 * @param [in] zEvnt Event ID which message will share memory.
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSend
 */
ZFUNC ZINT Zos_MsgSendE(ZEVNT zEvnt, ZTASKID zSendTaskId, 
                ZTASKID zRecvTaskId, ZUINT iMsgType);

/**
 * @brief Send message with task id and event id, include message priority.
 *
 * @param [in] zEvnt Event ID which message will share memory.
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] iPriority Task message priority.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSendP
 */
ZFUNC ZINT Zos_MsgSendEP(ZEVNT zEvnt, ZTASKID zSendTaskId, 
                ZTASKID zRecvTaskId, ZUINT iMsgType, ZINT iPriority);

/**
 * @brief Send message with task id and event id.
 *
 * @param [in] zEvnt Event ID which message will share memory.
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] pObject The message object.
 * @param [in] pData Message data.
 * @param [in] dwLen Message data length.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSend
 */
ZFUNC ZINT Zos_MsgSendED(ZEVNT zEvnt, ZTASKID zSendTaskId, 
                ZTASKID zRecvTaskId, ZUINT iMsgType, ZVOID *pObject, 
                ZVOID *pData, ZUINT iDataLen);

/**
 * @brief Send message with task id and event id, include message priority.
 *
 * @param [in] zEvnt Event ID which message will share memory.
 * @param [in] zSendTaskId Task ID which message send from.
 * @param [in] zRecvTaskId Task ID which message send to.
 * @param [in] iMsgType The message type.
 * @param [in] pObject The message object.
 * @param [in] pData Message data.
 * @param [in] dwLen Message data length.
 * @param [in] iPriority Task message priority.
 *
 * @retval ZOK Message send successfully.
 * @retval ZFAILED Message send failed.
 *
 * @note This function returned after message was added into the destination
 *       task message queue, regardless the message has been processed by
 *       destination task.
 *
 * @see @ref Zos_MsgSend
 */
 ZFUNC ZINT Zos_MsgSendEDP(ZEVNT zEvnt, ZTASKID zSendTaskId, 
                ZTASKID zRecvTaskId, ZUINT iMsgType, ZVOID *pObject, 
                ZVOID *pData, ZUINT iDataLen, ZINT iPriority);

/**
 * @brief Get send task id in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Send task id if successfully, or return ZMAXTASKID.
 *
 * @see @ref Zos_MsgGetRecvTaskId
 */
ZFUNC ZTASKID Zos_MsgGetSendTaskId(ZMSG zMsg);

/**
 * @brief Get received task id in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Received task id if successfully, or return ZMAXTASKID.
 *
 * @see @ref Zos_MsgGetSendTaskId
 */
ZFUNC ZTASKID Zos_MsgGetRecvTaskId(ZMSG zMsg);

/**
 * @brief Get the type in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Message type if successfully, or return ZMAXEVNTID.
 *
 * @see @ref Zos_MsgGetObject
 */
ZFUNC ZUINT Zos_MsgGetType(ZMSG zMsg);

/**
 * @brief Get the object in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Object if successfully, or return ZNULL.
 *
 * @see @ref Zos_MsgGetType
 */
ZFUNC ZVOID * Zos_MsgGetObject(ZMSG zMsg);

/**
 * @brief Get the data in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Message data if successfully, or return ZNULL.
 *
 * @see @ref Zos_MsgGetDataLen
 */
ZFUNC ZVOID * Zos_MsgGetData(ZMSG zMsg);

/**
 * @brief Get the data length in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Message data if successfully, or return ZNULL.
 *
 * @see @ref Zos_MsgGetData
 */
ZFUNC ZUINT Zos_MsgGetDataLen(ZMSG zMsg);

/**
 * @brief Get the event id in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Event ID if successfully, or return ZNULL.
 *
 * @see @ref Zos_MsgSendE
 */
ZFUNC ZEVNT Zos_MsgGetEvntId(ZMSG zMsg);

/**
 * @brief Get the event id in message.
 *
 * @param [in] zMsg Task message id.
 *
 * @return Memory Buffer if successfully, or return ZNULL.
 *
 * @see @ref Zos_MsgSendE
 */
ZFUNC ZCBUF Zos_MsgGetMemBuf(ZMSG zMsg);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_MSG_H__ */

