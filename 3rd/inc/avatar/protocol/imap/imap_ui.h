/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : imap_ui.h
  Author        : tanek.ye
  Version       : 0.1
  Created on    : 2013-03-26
  Description   :
    Marcos and structure definitions required by the imap user interface.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_UI_H__
#define _IMAP_UI_H__

/** 
 * @file imap_ui.h
 * @brief IMAP User interfaces.
 */
#ifdef __cplusplus
extern "C"{
#endif

#define IMAP_SESS_OPT_LOG_DATA 0x0001 /**< @brief Log IMAP message data. */
#define IMAP_SESS_OPT_AUTO_RPT 0x0010 /**< @brief Automate send report . */

/* imap transport event type */
typedef enum EN_IMAP_TPT_EVNT_TYPE
{
    EN_IMAP_TPT_EVNT_CONNED,
    EN_IMAP_TPT_EVNT_DISCON,
    EN_IMAP_TPT_EVNT_CONNERR,
    EN_IMAP_TPT_EVNT_SENT,
    EN_IMAP_TPT_EVNT_SEND_FAIL,
    EN_IMAP_TPT_EVNT_SENDING, 
    EN_IMAP_TPT_EVNT_RECV_ERR,
} EN_IMAP_TPT_EVNT_TYPE;

/** @brief  IMAP event callback */
typedef ZINT (*PFN_IMAPRSPCB)(ZVOID * pSess, ZVOID * pTrans);

/** @brief  IMAP transport callback */
typedef ZINT (*PFN_IMAPTPTCB)(ZVOID * pSess, ZUINT iEvntType);

/**
 * @brief Open a message session.
 *
 * @param [in] pUserInfo The user id, IMAP stack convey it in event callback.
 * @param [in] bUseTls Use tls flag.
 * @param [in] pfnRspCb The response callback.
 * @param [in] pfnTptCb The transport callback.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Imap_SessClose
 */
ZFUNC ZINT Imap_SessOpen(ZCOOKIE zCookie, ZBOOL bUseTls, 
                PFN_IMAPRSPCB pfnRspCb, PFN_IMAPTPTCB pfnTptCb, 
                ZVOID **ppSess);

/**
 * @brief Close a session.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Close a session successfully.
 * @retval ZFAILED Close a session failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_SessClose(ZVOID * pSess);

/**
 * @brief Connect the session.
 * It will start to establish tcp connection to remote participant.
 *
 * @param [in] pSess The session id.
 * @param [in] pstRmtAddr The server address.
 *
 * @retval ZOK Connect the session successfully.
 * @retval ZFAILED Connect the session failed.
 *
 * @see @ref Imap_SessClose
 */
ZFUNC ZINT Imap_SessConn(ZVOID * pSess, ST_ZOS_INET_ADDR *pstRmtAddr);

/**
 * @brief Connect the session.
 * It will start to establish tcp connection to remote participant.
 *
 * @param [in] pSess The session id.
 * @param [in] pcHost The server host name.
 * @param [in] wPort The server port.
 *
 * @retval ZOK Connect the session successfully.
 * @retval ZFAILED Connect the session failed.
 *
 * @see @ref Imap_SessClose
 */
ZFUNC ZINT Imap_SessConnX(ZVOID * pSess, ZCHAR *pcHost, ZUSHORT wPort);

/**
 * @brief Send CAPABILITY command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdCapa(ZVOID * pSess);

/**
 * @brief Send NOOP command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdNoop(ZVOID * pSess);

/**
 * @brief Send LOGOUT command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdLogout(ZVOID * pSess);

/**
 * @brief Send STARTTLS command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdStartTls(ZVOID * pSess);

/**
 * @brief Send AUTHENTICATE command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMechName Authentication mechanism name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdAuth(ZVOID * pSess, ZCHAR *pcMechName);

/**
 * @brief Send LOGIN command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcUser User id.
 * @param [in] pcPwd Password.
 * @param [in] pcMsisdn Msisdn.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdLogin(ZVOID * pSess, ZCHAR *pcUser, ZCHAR *pcPwd, 
                              ZCHAR *pcMsisdn);

/**
 * @brief Send SELECT command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdSelect(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send EXAMINE command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdExamine(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send CREATE command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName New mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdCreate(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send DELETE command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdDelete(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send RENAME command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcOriMbxName Original mailbox name.
 * @param [in] pcNewMbxName New mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdRename(ZVOID * pSess, ZCHAR *pcOriMbxName, 
                ZCHAR *pcNewMbxName);

/**
 * @brief Send SUBSCRIBE command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdSub(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send UNSUBSCRIBE command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdUnsub(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send LIST command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcRefName Reference name.
 * @param [in] pcMbxName Mailbox name with possible wildcards.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdList(ZVOID * pSess, ZCHAR *pcRefName, 
                ZCHAR *pcMbxName);

/**
 * @brief Send LSUB command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcRefName Reference name.
 * @param [in] pcMbxName Mailbox name with possible wildcards.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdLsub(ZVOID * pSess, ZCHAR *pcRefName, 
                ZCHAR *pcMbxName);

/**
 * @brief Send STATUS command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Mailbox name.
 * @param [in] iStatusType Status type set, @see EN_IMAP_REQ_STAT_TYPE.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdStatus(ZVOID * pSess, ZCHAR *pcMbxName, 
                ZUINT iStatusType);

/**
 * @brief Send APPEND command.
 *
 * @param [in] pSess The session id.
 * @param [in] dwAppMsgId Append message id, generate by Imap_ImfMsgCreate.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdAppend(ZVOID * pSess, ZVOID * pAppMsgId, 
                ZCHAR *pcMbxName);

/**
 * @brief Send APPEND command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcData Message data.
 * @param [in] iDataLen Message length.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdAppendX(ZVOID * pSess, ZCHAR *pcData, 
                ZUINT iDataLen, ZCHAR *pcMbxName);

/**
 * @brief Send APPEND command.
 *
 * @param [in] pSess The session id.
 * @param [in] dwAppMsg Message data, generate by Imap_AppMsgCreate.
 * @param [in] pcMbxName Mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdAppendXX(ZVOID * pSess, ZVOID * pAppMsg, 
                ZCHAR *pcMbxName);

/**
 * @brief Send CHECK command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdCheck(ZVOID * pSess);

/**
 * @brief Send CLOSE command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdClose(ZVOID * pSess);

/**
 * @brief Send EXPUNGE command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdExpunge(ZVOID * pSess);

/**
 * @brief Send SEARCH command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdSearch(ZVOID * pSess);

/**
 * @brief Send FETCH command.
 *
 * @param [in] pSess The session id.
 * @param [in] pSeqSetId Sequence set id, generate by Imap_SeqSetCreate.
 * @param [in] iType Fetch type ,@see EN_IMAP_FETCH_ITEM_TYPE.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdFetch(ZVOID * pSess, ZVOID * pSeqSetId, ZUINT iType);

/**
 * @brief Send STORE command.
 *
 * @param [in] pSess The session id.
 * @param [in] pSeqSetId Sequence set id, generate by Imap_SeqSetCreate.
 * @param [in] iStoreType Store operation type, @see EN_IMAP_REQ_STORE_TYPE.
 * @param [in] bSilent Flag of silent, if ZTRUE, server will never return new flag value.
 * @param [in] iFlagType Flag type, @see EN_IMAP_FLAG_TYPE.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdStore(ZVOID * pSess, ZVOID * pSeqSetId, 
                ZUINT iStoreType, ZBOOL bSilent, ZUINT iFlagType);

/**
 * @brief Send COPY command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Target mailbox name.
 * @param [in] pSeqSetId Sequence set id, generate by Imap_SeqSetCreate.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdCopy(ZVOID * pSess, ZCHAR *pcMbxName, 
                ZVOID * pSeqSetId);

/**
 * @brief Send COPY command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Target mailbox name.
 * @param [in] iSeq Sequence id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdCopyOne(ZVOID * pSess, ZCHAR *pcMbcName, 
                ZUINT iSeq);

/**
 * @brief Send COPY command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName Target mailbox name.
 * @param [in] iSeq Begin sequence id.
 * @param [in] iSeq End sequence id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdCopyRange(ZVOID * pSess, ZCHAR *pcMbcName, 
                ZUINT iSeqBegin, ZUINT iSeqEnd);

/**
 * @brief Send UID command.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdUid();

/**
 * @brief Send SETACL command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName The mailbox name.
 * @param [in] pcUserId The user id.
 * @param [in] iRightsSet The rights set, @see EN_IMAP_ACL_RIGHT.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdSetAcl(ZVOID * pSess, ZCHAR *pcMbxName, 
                ZCHAR *pcUserId, ZUINT iRightsSet);

/**
 * @brief Send DELETEACL command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName The mailbox name.
 * @param [in] pcUserId The user id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdDelAcl(ZVOID * pSess, ZCHAR *pcMbxName, 
                ZCHAR *pcUserId);

/**
 * @brief Send GETACL command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName The mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdGetAcl(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send LISTRIGHTS command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName The mailbox name.
 * @param [in] pcUserId The user id.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdListRights(ZVOID * pSess, ZCHAR *pcMbxName, 
                ZCHAR *pcUserId);

/**
 * @brief Send MYRIGHTS command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcMbxName The mailbox name.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdMyRights(ZVOID * pSess, ZCHAR *pcMbxName);

/**
 * @brief Send custom command.
 *
 * @param [in] pSess The session id.
 * @param [in] iReqType The request type.
 * @param [in] pcRequest The requset string.
 *
 * @retval ZOK Send command successfully.
 * @retval ZFAILED Send command failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_CmdCtmReq(ZVOID * pSess, ZUINT iReqType, 
                ZCHAR *pcRequest);

/**
 * @brief Create sequence set, used by COPY, FETCH, STORE command.
 *
 * @param [in] pSess The session id.
 *
 * @return Sequence set id.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZVOID * Imap_SeqSetCreate(ZVOID * pSess);

/**
 * @brief Delete sequence set.
 *
 * @param [in] pSeqSetId The sequence set id.
 *
 * @retval ZOK Delete sequence set successfully.
 * @retval ZFAILED Delete sequence set failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_SeqSetDelete(ZVOID * pSeqSetId);

/**
 * @brief Add sequence id to sequence set.
 *
 * @param [in] pSeqSetId The sequence set id.
 * @param [in] iSeq The sequence id.
 *
 * @retval ZOK Add sequence id successfully.
 * @retval ZFAILED Add sequence id failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_SeqSetAddSeq(ZVOID * pSeqSetId, ZUINT iSeq);

/**
 * @brief Add sequence range to sequence set.
 *
 * @param [in] pSeqSetId The sequence set id.
 * @param [in] iSeqBegin The begin sequence id.
 * @param [in] iSeqEnd The end sequence id.
 *
 * @retval ZOK Add sequence range successfully.
 * @retval ZFAILED Add sequence range failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_SeqSetAddRange(ZVOID * pSeqSetId, 
                ZUINT iSeqBegin, ZUINT iSeqEnd);

/**
 * @brief Create append message, used by APPEND command.
 *
 * @param [in] pSess The session id.
 *
 * @return Append message id.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZVOID * Imap_ImfMsgCreate(ZVOID * pSess);

/**
 * @brief Delete append message.
 *
 * @param [in] pMsgId The append message id.
 *
 * @retval ZOK Delete append message successfully.
 * @retval ZFAILED Delete append message failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_ImfMsgDelete(ZVOID * pMsgId);

/**
 * @brief Add header info to append message.
 *
 * @param [in] pMsgId The append message id.
 * @param [in] pcHdrName The name of header.
 * @param [in] pcHdrVal The value of header.
 *
 * @retval ZOK Add header info successfully.
 * @retval ZFAILED Add header info failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_ImfMsgAddHdr(ZVOID * pMsgId, ZCHAR *pcHdrName, 
                ZCHAR *pcHdrVal);

/**
 * @brief Add message text to append message.
 *
 * @param [in] pMsgId The append message id.
 * @param [in] pcTxt The message text.
 * @param [in] iLen The length of message text.
 *
 * @retval ZOK Add message text successfully.
 * @retval ZFAILED Add message text failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_ImfMsgAddTxt(ZVOID * pMsgId, ZCHAR *pcTxt, 
                ZUINT iLen);

/**
 * @brief Create append message, used by APPEND command.
 *
 * @param [in] pSess The session id.
 * @param [in] pcBoundary The boundary, ZNULL if single part body.
 *
 * @return Append message id.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZVOID * Imap_AppMsgCreate(ZVOID * pSess, ZCHAR *pcBoundary);

/**
 * @brief Add header info to append message.
 *
 * @param [in] dwAppMsg The append message id.
 * @param [in] pcHdrName The name of header.
 * @param [in] pcHdrValue The value of header.
 *
 * @retval ZOK Add header info successfully.
 * @retval ZFAILED Add header info failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_AppMsgAddHdr(ZVOID * pAppMsg, ZCHAR *pcHdrName, 
                ZCHAR *pcHdrValue);

/**
 * @brief Add body to append message.
 *
 * @param [in] dwAppMsg The append message id.
 *
 * @return Append message body id.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZVOID * Imap_AppMsgAddBdy(ZVOID * pAppMsg);

/**
 * @brief Add header info to append message body.
 *
 * @param [in] dwAppMsg The append message id.
 * @param [in] pAppBdy The append message body id.
 * @param [in] pcHdrName The name of header.
 * @param [in] pcHdrValue The value of header.
 *
 * @retval ZOK Add header info successfully.
 * @retval ZFAILED Add header info failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_AppBdyAddHdr(ZVOID * pAppMsg, ZVOID * pAppBdy, 
                ZCHAR *pcHdrName, ZCONST ZCHAR *pcHdrValue);

/**
 * @brief Add body data to append message body.
 *
 * @param [in] dwAppMsg The append message id.
 * @param [in] pAppBdy The append message body id.
 * @param [in] pcBdyData The body data buffer.
 * @param [in] iBdySize The body data length.
 *
 * @retval ZOK Add header info successfully.
 * @retval ZFAILED Add header info failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_AppBdyAddBdy(ZVOID * pAppMsg, ZVOID * pAppBdy, 
                ZCHAR *pcBdyData, ZUINT iBdySize);

/**
 * @brief Add body data to append message body.
 *
 * @param [in] dwAppMsg The append message id.
 * @param [in] pAppBdy The append message body id.
 * @param [in] pcFileName The file of body data.
 *
 * @retval ZOK Add header info successfully.
 * @retval ZFAILED Add header info failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_AppBdyAddBdyX(ZVOID * pAppMsg, ZVOID * pAppBdy, 
                ZCONST ZCHAR *pcFileName);

/**
 * @brief Get user id of session.
 *
 * @param [in] pSess The session id.
 *
 * @return The cookie value.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZCOOKIE Imap_SessGetCookie(ZVOID * pSess);

/**
 * @brief Get response of a transaction.
 *
 * @param [in] pTrans The transaction id.
 * @param [out] ppstComplete The response of the transaction.
 *
 * @retval ZOK Get responsesuccessfully.
 * @retval ZFAILED Get response failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_TransGetRsp(ZVOID * pTrans, 
                ST_IMAP_RSP_COMPLETE **ppstComplete);

/**
 * @brief Get response type of a transaction.
 *
 * @param [in] pTrans The transaction id.
 * @param [out] piReqType The response type of the transaction.
 *
 * @retval ZOK Get response type successfully.
 * @retval ZFAILED Get response type failed.
 *
 * @see @ref Imap_SessOpen
 */
ZFUNC ZINT Imap_TransGetReqType(ZVOID * pTrans, ZUINT *piReqType);

/**
 * @brief Get request total size of a transaction.
 *
 * @param [in] dwTransId The transaction id.
 *
 * @retval the total size of request.
 *
 */
ZFUNC ZUINT Imap_TransGetReqTotalSize(ZVOID * pTrans);

/**
 * @brief Get request sent size of a transaction.
 *
 * @param [in] dwTransId The transaction id.
 *
 * @retval the sent size of request.
 *
 */
ZFUNC ZUINT Imap_TransGetReqSentSize(ZVOID * pTrans);

ZFUNC ZINT Imap_TransDone(ZVOID * pSess, ZVOID * pTrans);

#ifdef __cplusplus
}
#endif

#endif /* _IMAP_UI_H__ */

