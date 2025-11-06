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
  File name     : sip_ui.h
  Module        : sip protocol user interface
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_UI_H__
#define _SIP_UI_H__

/**
 * @file
 * @brief SIP user interfaces.
 *
 * SIP stack will report event to user by sending message to task or callback.
 * User task(default is SUA) should process these messages
 * in the task message process function.
 * User task can identify message type by session event type.
 *
 * User can use @ref Sip_CfgSetUpperTaskId to change the user task ID.
 *
 * Following type of events will be sent from SIP stack to user:
 * @code
 * -------------------------
 * EN_SIP_SESSE_ERR_IND -- SIP stack process error message
 *
 * note: INVITE 100 Trying will not be send to upper layer task
 * EN_SIP_SESSE_SIM_IND -- SIP stack received INVITE request
 * EN_SIP_SESSE_SIM_CNF -- SIP stack received INVITE response
 * EN_SIP_SESSE_SMM_IND -- SIP stack received Re-INVITE request
 * EN_SIP_SESSE_SMM_CNF -- SIP stack received Re-INVITE response
 * EN_SIP_SESSE_SAM_IND -- SIP stack received ACK request
 * EN_SIP_SESSE_SCM_IND -- SIP stack received CANCEL request
 * EN_SIP_SESSE_STM_IND -- SIP stack received BYE request
 * EN_SIP_SESSE_STM_CNF -- SIP stack received BYE response
 *
 * EN_SIP_SESSE_SSM_IND -- SIP stack received request as below:
 *   PRACK, INFO, UPDATE, OPTION, REGISTER, MESSAGE, COMET, PUBLISH 
 * EN_SIP_SESSE_SSM_CNF -- SIP stack received response as below:
 *   PRACK, INFO, UPDATE, OPTION, REGISTER, MESSAGE, COMET, PUBLISH 
 *
 * EN_SIP_SESSE_DAM_IND -- SIP stack received request as below:
 *   SUBSCRIBE, REFER, NOTIFY
 * EN_SIP_SESSE_DAM_CNF -- SIP stack received response as below:
 *   SUBSCRIBE, REFER, NOTIFY
 * 
 * CIM messages are some messages which are not in exist dialog
 * EN_SIP_SESSE_CIM_IND -- SIP stack received request as below:
 *   OPTIONS, REGISTER, MESSAGE, COMET, PUBLISH
 * EN_SIP_SESSE_CIM_CNF -- SIP stack received response as below:
 *   OPTIONS, REGISTER, MESSAGE, COMET, PUBLISH
 * @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief SIP send session event.
 *
 * @param [in] pstEvnt SIP session event.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSessEvnt(ST_SIP_SESS_EVNT *pstEvnt);

/**
 * @brief SIP send session initiating request, like INVITE.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] pstMsg SIP message structure. It should be includes request line,
 *                    from header, to headers, and other headers user defined.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSimReq(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iDlgUserId, ZUINT iTransUserId, 
                ST_SIP_TPT_ADDR *pstTptAddr, ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send session initiating response, like 123456xx to INVITE.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] iTransId SIP transaction ID.
 * @param [in] iStatusCode Status code number, from 1xx to 6xx.
 * @param [in] pstMsg SIP message structure. It should be includes request line,
 *                    from header, to headers, and other headers user defined.
 *
 * @retval ZOK Send response to SIP task successfully.
 * @retval ZFAILED Send response to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSimRsp(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iSessId, ZUINT iDlgUserId, ZUINT iDlgId, 
                ZUINT iTransUserId, ZUINT iTransId, ZUINT iStatusCode, 
                ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send session status request, it could be PRACK, INFO, UPDATE, 
 *        OPTION, REGISTER, MESSAGE, COMET, PUBLISH.
 *
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] ucMethodType SIP method type, @ref EN_SIP_METHOD.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSsmReq(ZUINT iSessUserId, ZUINT iSessId,
                ZUINT iDlgUserId, ZUINT iDlgId, ZUINT iTransUserId, 
                ST_SIP_TPT_ADDR *pstTptAddr, ZUCHAR ucMethodType, 
                ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send session status response, like 123456xx to PRACK, INFO, 
 *        UPDATE, OPTION, REGISTER, MESSAGE, COMET, PUBLISH.
 *
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] iTransId SIP transaction ID.
 * @param [in] ucMethodType SIP method type, @ref EN_SIP_METHOD.
 * @param [in] iStatusCode Status code number, from 1xx to 6xx.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send response to SIP task successfully.
 * @retval ZFAILED Send response to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSsmRsp(ZUINT iSessUserId, ZUINT iSessId,
                ZUINT iDlgUserId, ZUINT iDlgId, ZUINT iTransUserId, 
                ZUINT iTransId, ZUCHAR ucMethodType, ZUINT iStatusCode, 
                ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send session ack request, it could be ACK.
 *
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSamReq(ZUINT iSessUserId, ZUINT iSessId,
                ZUINT iDlgUserId, ZUINT iDlgId, ZUINT iTransUserId, 
                ST_SIP_TPT_ADDR *pstTptAddr, ST_SIP_MSG *pstMsg);

/**
 * @brief SIP send session cancel request, it could be CANCEL.
 *
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendScmReq(ZUINT iSessUserId, ZUINT iSessId,
                ZUINT iDlgUserId, ZUINT iDlgId, ZUINT iTransUserId, 
                ST_SIP_TPT_ADDR *pstTptAddr, ST_SIP_MSG *pstMsg);

/**
 * @brief SIP send session modifying request, it could be Re-INVITE, UPDATE.
 *
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSmmReq(ZUINT iSessUserId, ZUINT iSessId,
                ZUINT iDlgUserId, ZUINT iDlgId, ZUINT iTransUserId, 
                ST_SIP_TPT_ADDR *pstTptAddr, ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send session modifying response, like 123456xx to Re-INVITE.
 *
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] iTransId SIP transaction ID.
 * @param [in] iStatusCode Status code number, from 1xx to 6xx.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send response to SIP task successfully.
 * @retval ZFAILED Send response to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendSmmRsp(ZUINT iSessUserId, ZUINT iSessId,
                ZUINT iDlgUserId, ZUINT iDlgId, ZUINT iTransUserId, 
                ZUINT iTransId, ZUINT iStatusCode, ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send session terminating request, it could be CANCEL, BYE.
 *
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendStmReq(ZUINT iSessUserId, ZUINT iSessId,
                ZUINT iDlgUserId, ZUINT iDlgId, ZUINT iTransUserId, 
                ST_SIP_TPT_ADDR *pstTptAddr, ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send session dialog associate message request, it could be 
 *        SUBSCRIBE, REFER, NOTIFY.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] ucMethodType SIP method type, @ref EN_SIP_METHOD.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendDamReq(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iSessId, ZUINT iDlgUserId, ZUINT iDlgId, 
                ZUINT iTransUserId, ST_SIP_TPT_ADDR *pstTptAddr, 
                ZUCHAR ucMethodType, ST_SIP_MSG *pstMsg); 

/**
 * @brief SIP send session dialog associate message request, only for REFER.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] iReferExpires Expires value of REFER.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendDamReqX(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iSessId, ZUINT iDlgUserId, ZUINT iDlgId, 
                ZUINT iTransUserId, ST_SIP_TPT_ADDR *pstTptAddr, 
                ZUINT iReferExpires, ST_SIP_MSG *pstMsg); 

/**
 * @brief SIP send associate message response, like 123456xx to SUBSCRIBE,
 *        REFER, NOTIFY.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] iTransId SIP transaction ID.
 * @param [in] ucMethodType SIP method type, @ref EN_SIP_METHOD.
 * @param [in] iStatusCode Status code number, from 1xx to 6xx.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send response to SIP task successfully.
 * @retval ZFAILED Send response to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendDamRsp(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iSessId, ZUINT iDlgUserId, ZUINT iDlgId, 
                ZUINT iTransUserId, ZUINT iTransId, ZUCHAR ucMethodType, 
                ZUINT iStatusCode, ST_SIP_MSG *pstMsg);    

/**
  * @brief SIP send associate message response only for REFER.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] iTransId SIP transaction ID.
 * @param [in] iStatusCode Status code number, from 1xx to 6xx.
 * @param [in] iReferExpires SIP method type, @ref EN_SIP_METHOD.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send response to SIP task successfully.
 * @retval ZFAILED Send response to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendDamRspX(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iSessId, ZUINT iDlgUserId, ZUINT iDlgId, 
                ZUINT iTransUserId, ZUINT iTransId, ZUINT iStatusCode, 
                ZUINT iReferExpires, ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send call independent message request, it could be 
 *        OPTIONS, REGISTER, MESSAGE, COMET, PUBLISH.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] pstTptAddr Transport address.
 * @param [in] ucMethodType SIP method type, @ref EN_SIP_METHOD.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send request to SIP task successfully.
 * @retval ZFAILED Send request to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendCimReq(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iDlgUserId, ZUINT iTransUserId, 
                ST_SIP_TPT_ADDR *pstTptAddr, ZUCHAR ucMethodType, 
                ST_SIP_MSG *pstMsg);    

/**
 * @brief SIP send call independent message response, it could be 123456xx to
 *        OPTIONS, REGISTER, MESSAGE, COMET, PUBLISH.
 *
 * @param [in] zCookie SIP service owner(component ID).
 * @param [in] iSessUserId User defined session user ID.
 * @param [in] iSessId SIP session ID.
 * @param [in] iDlgUserId User defined dialog user ID.
 * @param [in] iDlgId SIP dialog ID.
 * @param [in] iTransUserId User defined transaction user ID.
 * @param [in] iTransId SIP transaction ID.
 * @param [in] ucMethodType SIP method type, @ref EN_SIP_METHOD.
 * @param [in] iStatusCode Status code number, from 1xx to 6xx.
 * @param [in] pstMsg SIP message structure. If ZNULL, SIP stack will create it.
 *
 * @retval ZOK Send response to SIP task successfully.
 * @retval ZFAILED Send response to SIP task failed.
 *
 * @see
 */
ZFUNC ZINT Sip_SendCimRsp(ZCOOKIE zCookie, ZUINT iSessUserId, 
                ZUINT iSessId, ZUINT iDlgUserId, ZUINT iDlgId, 
                ZUINT iTransUserId, ZUINT iTransId, ZUCHAR ucMethodType, 
                ZUINT iStatusCode, ST_SIP_MSG *pstMsg);  

#ifdef __cplusplus
}
#endif

#endif /* _SIP_UI_H__ */

