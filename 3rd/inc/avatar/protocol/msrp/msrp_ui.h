/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : msrp_ui.h
  Author        : cathy.chen
  Version       : 0.1
  Created on    : 2010-10-18
  Description   :
    Marcos and structure definitions required by the msrp user interface.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_UI_H__
#define _MSRP_UI_H__

/** 
 * @file msrp_ui.h
 * @brief MSRP User interfaces.
 */
#ifdef __cplusplus
extern "C"{
#endif

#define MSRP_SESS_OPT_LOG_DATA 0x0001 /**< @brief Log MSRP message data. */
#define MSRP_SESS_OPT_AUTO_RPT 0x0002 /**< @brief Automate send report. */
#define MSRP_SESS_OPT_SUPT_ACM 0x0004 /**< @brief Support alternative connection model. */
#define MSRP_SESS_OPT_KEEP_ALIVE 0x0008 /**< @brief Support keep alive. */
#define MSRP_SESS_OPT_SUPT_ALL 0xFFFF /**< @brief Support all options. */

/* msrp session transport type */
typedef enum EN_MSRP_SESS_TPT_TYPE
{
    EN_MSRP_SESS_TPT_TCP,
    EN_MSRP_SESS_TPT_TLS,
} EN_MSRP_SESS_TPT_TYPE;

/* msrp session file flag type */
typedef enum EN_MSRP_FILE_FLAG_TYPE
{
    EN_MSRP_FILE_FLAG_SEND_ONLY,
    EN_MSRP_FILE_FLAG_RECV_ONLY,
    EN_MSRP_FILE_FLAG_SEND_RECV,
    EN_MSRP_FILE_FLAG_RESUME_SEND,
    EN_MSRP_FILE_FLAG_RESUME_RECV,
} EN_MSRP_FILE_FLAG_TYPE;

/* msrp media stream direction type */
typedef enum EN_MSRP_STREAM_DIRECT_TYPE
{
    EN_MSRP_MEDIA_STREAM_SEND,
    EN_MSRP_MEDIA_STREAM_RECV,
    EN_MSRP_MEDIA_STREAM_SENDRECV,
} EN_MSRP_STREAM_DIRECT_TYPE;

/**
 * @brief Open a message session.
 *
 * @param [in] iUserId The user id, MSRP stack convey it in event callback.
 * @param [in] dwSessOpt The session option, see @ref MSRP_SESS_OPT_LOG_DATA etc.
 * @param [in] bPassive The passive mode, if ZTRUE, MSRP will wait for tcp connection from remote.
 * @param [in] iTtpType The transport type, @see @ref EN_MSRP_SESS_TPT_TYPE.
 * @param [out] ppcLclUri The local uri, the format is like "msrp(s)://ip:port/trans_id;tcp".
 * @param [out] ppstLclAddr The local address.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessOpen(ZUINT iUserId, ZUINT iSessOpt, 
                ZBOOL bPassive, ZUINT iTtpType, ZCHAR **ppcLclUri, 
                ST_ZOS_INET_ADDR **ppstLclAddr, ZVOID **ppSess);

/**
 * @brief Open a message session and specify passive mode.
 * Mostly MSRP application receive MSRP invitation, then it should open local session 
 * and specify remote URI and address, then it will wait remote connection as passive.
 *
 * @param [in] iUserId The user id, MSRP stack convey it in event callback.
 * @param [in] iSessOpt The session option, see @ref MSRP_SESS_OPT_LOG_DATA etc.
 * @param [in] pcRmtUri The remote uri.
 * @param [in] pstRmtAddr The remote address.
 * @param [in] bPassive The passive mode, if ZTRUE, MSRP will wait for tcp connection from remote.
 * @param [in] iTtpType The transport type, @see @ref EN_MSRP_SESS_TPT_TYPE.
 * @param [out] ppcLclUri The local uri.
 * @param [out] ppstLclAddr The local address.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessOpenP(ZUINT iUserId, ZUINT iSessOpt,
                ZCHAR *pcRmtUri, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZBOOL bPassive, ZUINT iTtpType, ZCHAR **ppcLclUri, 
                ST_ZOS_INET_ADDR **ppstLclAddr, ZVOID **ppSess);

/**
 * @brief Open a file transfer session.
 *
 * @param [in] iUserId The user id, MSRP stack convey it in event callback.
 * @param [in] iSessOpt The session option, see @ref MSRP_SESS_OPT_LOG_DATA etc.
 * @param [in] bPassive The passive mode, if ZTRUE, MSRP will wait for tcp connection from remote.
 * @param [in] iTtpType The transport type, @see @ref EN_MSRP_SESS_TPT_TYPE.
 * @param [in] pcFileName The local file name to be transferred.
 * @param [in] pcFileType The local file type to be transferred.
 * @param [in] zLstData The gzip list data.
 * @param [in] zTailData The gzip tail data.
 * @param [out] ppcLclUri The local uri, the format is like "msrp://ip:port/trans_id;tcp".
 * @param [out] ppstLclAddr The local address.
 * @param [out] piFileSize The local file size.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessOpenF(ZUINT iUserId, ZUINT iSessOpt, 
                ZBOOL bPassive, ZUINT iTtpType, ZCHAR *pcFileName, 
                ZCHAR *pcFileType,  ZDBUF zLstData, ZDBUF zTailData,
                ZCHAR **ppcLclUri, ST_ZOS_INET_ADDR **ppstLclAddr, 
                ZUINT *piFileSize, ZVOID **ppSess);

/**
 * @brief Open a file forward session.
 *
 * @param [in] iUserId The user id, MSRP stack convey it in event callback.
 * @param [in] iSessOpt The session option, see @ref MSRP_SESS_OPT_LOG_DATA etc.
 * @param [in] bPassive The passive mode, if ZTRUE, MSRP will wait for tcp connection from remote.
 * @param [in] iTtpType The transport type, @see @ref EN_MSRP_SESS_TPT_TYPE.
 * @param [in] pcFileName The local file name to be transferred.
 * @param [in] pcFileType The local file type to be transferred.
 * @param [in] zLstData The gzip list data.
 * @param [in] zTailData The gzip tail data.
 * @param [out] ppcLclUri The local uri, the format is like "msrp://ip:port/trans_id;tcp".
 * @param [out] ppstLclAddr The local address.
 * @param [out] piFileSize The  file size.
 * @param [out] piToTalSize The local file size.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessOpenFFW(ZUINT iUserId, ZUINT iSessOpt, 
                ZBOOL bPassive, ZUINT iTtpType, ZCHAR *pcFileName, 
                ZCHAR *pcFileType, ZDBUF zLstData, ZDBUF zTailData,
                ZCHAR **ppcLclUri, ST_ZOS_INET_ADDR **ppstLclAddr, 
                ZUINT iFileSize, ZUINT *piToTalSize, ZVOID **ppSess);

/**
 * @brief Open a file transfer session.
 *
 * @param [in] iUserId The user id, MSRP stack convey it in event callback.
 * @param [in] iSessOpt The session option, see @ref MSRP_SESS_OPT_LOG_DATA etc.
 * @param [in] bPassive The passive mode, if ZTRUE, MSRP will wait for tcp connection from remote.
 * @param [in] iTtpType The transport type, @see @ref EN_MSRP_SESS_TPT_TYPE.
 * @param [in] pcFileName The local file name to be transferred.
 * @param [in] pcFileType The local file type to be transferred.
 * @param [in] iFileFlag The local file flag, @see @ref EN_MSRP_FILE_FLAG_TYPE.
 * @param [in,out] piFileSize The local file size.
 * @param [out] ppcLclUri The local uri, the format is like "msrp://ip:port/trans_id;tcp".
 * @param [out] ppstLclAddr The local address.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessOpenFX(ZUINT iUserId, ZUINT iSessOpt, 
                ZBOOL bPassive, ZUINT iTtpType, ZCHAR *pcFileName, 
                ZCHAR *pcFileType, ZUINT iFileFlag, ZUINT *piFileSize, 
                ZCHAR **ppcLclUri, ST_ZOS_INET_ADDR **ppstLclAddr, 
                ZVOID **ppSess);

/**
 * @brief Open a file transfer session and specify passive mode.
 * Mostly MSRP application receive MSRP invitation, then it should open local session 
 * and specify remote URI and address, then it will wait remote connection as passive.
 *
 * @param [in] iUserId The user id, MSRP stack convey it in event callback.
 * @param [in] iSessOpt The session option, see @ref MSRP_SESS_OPT_LOG_DATA etc.
 * @param [in] pcRmtUri The remote uri.
 * @param [in] pstRmtAddr The remote address.
 * @param [in] bPassive The passive mode, if ZTRUE, MSRP will wait for tcp connection from remote.
 * @param [in] iTtpType The transport type, @see @ref EN_MSRP_SESS_TPT_TYPE.
 * @param [in] pcFileName The local file name to be saved.
 * @param [out] ppcLclUri The local uri.
 * @param [out] ppstLclAddr The local address.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessOpenFP(ZUINT iUserId, ZUINT iSessOpt,
                ZCHAR *pcRmtUri, ST_ZOS_INET_ADDR *pstRmtAddr, ZBOOL bPassive, 
                ZUINT iTtpType, ZCHAR *pcFileName, ZUINT iFileSize, 
                ZCHAR **ppcLclUri, ST_ZOS_INET_ADDR **ppstLclAddr, 
                ZVOID **ppSess);

/**
 * @brief Open a file transfer session and specify passive mode.
 * Mostly MSRP application receive MSRP invitation, then it should open local session 
 * and specify remote URI and address, then it will wait remote connection as passive.
 *
 * @param [in] iUserId The user id, MSRP stack convey it in event callback.
 * @param [in] iSessOpt The session option, see @ref MSRP_SESS_OPT_LOG_DATA etc.
 * @param [in] pcRmtUri The remote uri.
 * @param [in] pstRmtAddr The remote address.
 * @param [in] bPassive The passive mode, if ZTRUE, MSRP will wait for tcp connection from remote.
 * @param [in] iTtpType The transport type, @see @ref EN_MSRP_SESS_TPT_TYPE.
 * @param [in] pcFileName The local file name to be saved.
 * @param [in] iFileFlag The local file flag, @see @ref EN_MSRP_FILE_FLAG_TYPE.
 * @param [in,out] piFileSize The local file size.
 * @param [out] ppcLclUri The local uri.
 * @param [out] ppstLclAddr The local address.
 * @param [out] ppSess The session id.
 *
 * @retval ZOK Open a session successfully.
 * @retval ZFAILED Open a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessOpenFPX(ZUINT iUserId, ZUINT iSessOpt,
                ZCHAR *pcRmtUri, ST_ZOS_INET_ADDR *pstRmtAddr, ZBOOL bPassive, 
                ZUINT iTtpType, ZCHAR *pcFileName, ZUINT iFileFlag, 
                ZUINT *piFileSize, ZCHAR **ppcLclUri, 
                ST_ZOS_INET_ADDR **ppstLclAddr, ZVOID **ppSess);

/**
 * @brief Cease a session.
 * Release the local file resources, It only valid for file transfer.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Cease a session successfully.
 * @retval ZFAILED Cease a session failed.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZINT Msrp_SessCease(ZVOID *pSess);

/**
 * @brief Close a session.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Close a session successfully.
 * @retval ZFAILED Close a session failed.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZINT Msrp_SessClose(ZVOID *pSess);

/**
 * @brief Relay a session.
 * It will start to establish tcp connection to remote participant.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Relay a session successfully.
 * @retval ZFAILED Relay a session failed.
 *
 * @see @ref Msrp_SessClose
 */
ZFUNC ZINT Msrp_SessRelay(ZVOID *pSess);

/**
 * @brief Passive a session.
 * Set session connection from active to passive. It only valid for file transfer.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Passive a session successfully.
 * @retval ZFAILED Passive a session failed.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZINT Msrp_SessPassive(ZVOID *pSess);

/**
 * @brief traffic control.
 * Set session connection be traffic control.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK traffic control a session successfully.
 * @retval ZFAILED traffic control a session failed.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZINT Msrp_SessTrafficControl(ZVOID *pSess);

/**
 * @brief session send response.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK session send response successfully.
 * @retval ZFAILED session send response failed.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZINT Msrp_SessMsgSendRsp(ZVOID *pSess);


/**
 * @brief Send data.
 * It will send data to remote participant.
 *
 * @param [in] pSess The session id.
 *
 * @retval ZOK Send data successfully.
 * @retval ZFAILED Send data failed.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZINT Msrp_SessSendData(ZVOID *pSess);

/**
 * @brief Get user id of a session.
 *
 * @param [in] pSess The session id.
 *
 * @retval User id successfully, otherwise return 0.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZUINT Msrp_SessGetUserId(ZVOID *pSess);

/**
 * @brief Get user cookie of a session.
 *
 * @param [in] pSess The session id.
 *
 * @retval User cookie successfully, otherwise return 0.
 *
 * @see @ref Msrp_SessSetCookie
 */
ZFUNC ZCOOKIE Msrp_SessGetCookie(ZVOID *pSess);

/**
 * @brief Set user cookie of a session.
 *
 * @param [in] pSess The session id.
 * @param [in] dwCookie The user cookie.
 *
 * @retval ZOK Set user cookie successfully.
 * @retval ZFAILED Set user cookie failed.
 *
 * @see @ref Msrp_SessGetCookie
 */
ZFUNC ZINT Msrp_SessSetCookie(ZVOID *pSess, ZCOOKIE zCookie);

/**
 * @brief Get session option of a session.
 *
 * @param [in] pSess The session id.
 *
 * @retval Session option successfully, otherwise return 0.
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZUINT Msrp_SessGetOpt(ZVOID *pSess);

/**
 * @brief Get local uri of a session.
 *
 * @param [in] pSess The session id.
 *
 * @retval Local uri.
 * The caller is responsible for freeing it by Zos_SysStrFree
 *
 * @see @ref Msrp_SessOpen
 */
ZFUNC ZCHAR * Msrp_SessGetLclUri(ZVOID *pSess);

/**
 * @brief Get remote uri of a session.
 *
 * @param [in] pSess The session id.
 *
 * @retval Remote uri.
 * The caller is responsible for freeing it by Zos_SysStrFree
 *
 * @see @ref Msrp_SessSetRmtUri
 */
ZFUNC ZCHAR * Msrp_SessGetRmtUri(ZVOID *pSess);

/**
 * @brief Set remote uri of a session.
 *
 * @param [in] pSess The session id.
 * @param [in] pcUri The remote uri.
 *
 * @retval ZOK Set remote uri successfully.
 * @retval ZFAILED Set remote uri failed.
 *
 * @see @ref Msrp_SessGetRmtUri
 */
ZFUNC ZINT Msrp_SessSetRmtUri(ZVOID *pSess, ZCHAR *pcUri);

/**
 * @brief Get local address of a session.
 *
 * @param [in] pSess The session id.
 *
 * @param [out] pstAddr Return the local address.
 *
 * @retval ZOK Get local address successfully.
 * @retval ZFAILED Get local address failed.
 *
 * @see @ref Msrp_SessGetRmtAddr
 */
ZFUNC ZINT Msrp_SessGetLclAddr(ZVOID *pSess, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Get remote address of a session.
 *
 * @param [in] pSess The session id.
 *
 * @param [out] pstAddr Return the remote address.
 *
 * @retval ZOK Get remote address successfully.
 * @retval ZFAILED Get remote address failed.
 *
 * @see @ref Msrp_SessSetRmtAddr
 */
ZFUNC ZINT Msrp_SessGetRmtAddr(ZVOID *pSess, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Set remote address of a session.
 *
 * @param [in] pSess The session id.
 * @param [in] pstAddr The remote address.
 *
 * @retval ZOK Set remote address successfully.
 * @retval ZFAILED Set remote address failed.
 *
 * @see @ref Msrp_SessGetRmtAddr
 */
ZFUNC ZINT Msrp_SessSetRmtAddr(ZVOID *pSess,
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Get remote domain of a session.
 *
 * @param [in] pSess The session id.
 * @param [in] pstAddr The remote domain.
 *
 * @retval remote domain.
 * The caller is responsible for freeing it by Zos_SysStrFree
 *
 * @see @ref Msrp_SessGetRmtDomain
 */
ZFUNC ZCHAR * Msrp_SessGetRmtDomain(ZVOID *pSess);

/**
 * @brief Set remote domain of a session.
 *
 * @param [in] pSess The session id.
 * @param [in] pstAddr The remote domain.
 *
 * @retval ZOK Set remote domain successfully.
 * @retval ZFAILED Set remote domain failed.
 *
 * @see @ref Msrp_SessSetRmtDomain
 */
ZFUNC ZINT Msrp_SessSetRmtDomain(ZVOID *pSess, 
                ZCHAR *pcDomain);

/**
 * @brief Get file name of a file transfer session.
 *
 * @param [in] pSess The session id.
 *
 * @retval File name successfully, otherwise return ZNULL.
 * The caller is responsible for freeing it by Zos_SysStrFree
 *
 * @see @ref Msrp_SessGetFileSize
 */
ZFUNC ZCHAR * Msrp_SessGetFileName(ZVOID *pSess);

/**
 * @brief Get file size of a file transfer session.
 *
 * @param [in] pSess The session id.
 *
 * @retval File size successfully, otherwise return ZNULL.
 *
 * @see @ref Msrp_SessGetFileName
 */
ZFUNC ZUINT Msrp_SessGetFileSize(ZVOID *pSess);

/**
 * @brief Set file offset of a file transfer session.
 *
 * @param [in] pSess The session id.
 * @param [in] iStartOffset The file start offset.
 * @param [in] iStopOffset The file stop offset.
 *
 * @retval ZOK Set file offset successfully.
 * @retval ZFAILED Set file offset failed.
 */
ZFUNC ZINT Msrp_SessSetFileOffset(ZVOID *pSess, ZUINT iStartOffset, 
                ZUINT iStopOffset);

/**
 * @brief Set certificate fingerprint.
 *
 * @param [in] pSess The session id.
 * @param [in] pcFgrprnt The certificate fingerprint.
 * @param [in] pcAlgo The algorithm of calculate certificate fingerprint.
 *
 * @retval ZOK Set certificate fingerprint successfully.
 * @retval ZFAILED Set certificate fingerprint failed.
 */
ZFUNC ZINT Msrp_SessSetCertFgrprnt(ZVOID *pSess, ZCHAR *pcFgrprnt, 
                ZCHAR *pcAlgo);

/**
 * @brief Set media stream direction type.
 *
 * @param [in] pSess The session id.
 * @param [in] ucStreamDirct The stream direction type, see @ref EN_MSRP_STREAM_DIRECT_TYPE.
 *
 * @retval ZOK Set media stream direction type successfully.
 * @retval ZFAILED Set media stream direction type failed.
 */
ZFUNC ZINT Msrp_SessSetStreamDirct(ZVOID *pSess, 
                ZUCHAR ucStreamDirct);


/**
 * @brief generate hash value.
 *
 * @param [in] pSess The session.
 * @param [in] zMemBuf The Hash Value memory buffer.
 * @param [out] pstHashValue The Hash Value .
 *
 * @retval ZOK Generate hash value successfully.
 * @retval ZFAILED Generate hash value failed.
 */
ZFUNC ZINT Msrp_SessGenSha1Hash(ZVOID *pSess, ZDBUF zMemBuf, 
                   ST_ZOS_SSTR *pstHashValue);

/**
 * @brief generate hash value.
 *
 * @param [in] pSess The session.
 * @param [in] zMemBuf The Hash Value memory buffer.
 * @param [out] pstHashValue The Hash Value .
 *
 * @retval ZOK Generate hash value successfully.
 * @retval ZFAILED Generate hash value failed.
 */
ZFUNC ZINT Msrp_SessGenMd5Hash(ZVOID *pSess, ZDBUF zMemBuf, 
                   ST_ZOS_SSTR *pstHashValue);

/**
 * @brief Open a message exchange of a session.
 *
 * @param [in] pSess The session id.
 * @param [in] iMsgLen The message length to be exchanged.
 * @param [in] pcContType The message content type
 * @param [out] ppSmsg The message id.
 *
 * @retval ZOK Open a message exchange successfully.
 * @retval ZFAILED Open a message exchange failed.
 *
 * @see @ref Msrp_MsgClose
 */
ZFUNC ZINT Msrp_MsgOpen(ZVOID *pSess, ZUINT iMsgLen, 
                ZCHAR *pcContType, ZVOID **ppSmsg);

/**
 * @brief Close a message exchange of a session.
 *
 * @param [in] pSmsg The message id.
 *
 * @retval ZOK Close a message exchange successfully.
 * @retval ZFAILED Close a message exchange failed.
 *
 * @see @ref Msrp_MsgOpen
 */
ZFUNC ZINT Msrp_MsgClose(ZVOID *pSmsg);

/**
 * @brief Send a message data piece in a message exchange.
 *
 * @param [in] pSmsg The message id.
 * @param [in] pucData The message data.
 * @param [in] iLen The message data length.
 *
 * @retval ZOK Send a message data successfully.
 * @retval ZFAILED Send a message data failed.
 *
 * @see @ref Msrp_MsgSendX
 */
ZFUNC ZINT Msrp_MsgSend(ZVOID *pSmsg, ZUCHAR *pucData, 
                ZUINT iLen);
/**
 * @brief Send a message data piece in a message exchange.
 *
 * @param [in] pSmsg The message id.
 * @param [in] zData The message data.
 *
 * @retval ZOK Send a message data successfully.
 * @retval ZFAILED Send a message data failed.
 *
 * @see @ref Msrp_MsgSend
 */
ZFUNC ZINT Msrp_MsgSendX(ZVOID *pSmsg, ZDBUF zData);

/**
 * @brief Set content type of a message exchange.
 *
 * @param [in] pSmsg The message id.
 * @param [in] ucMType The main-content type, see @ref EN_MSRP_TYPE.
 * @param [in] pcExtMType The extension main-content type.
 * @param [in] ucMSType The sub-content type, see @ref EN_MSRP_SUBTYPE.
 * @param [in] pcExtMSType The extension sub-content type.
 *
 * @retval ZOK Set the content type successfully.
 * @retval ZFAILED Set the content type failed.
 *
 * @see @ref Msrp_MsgSetRender
 */
ZFUNC ZINT Msrp_MsgSetCType(ZVOID *pSmsg, ZCHAR ucMType, 
                ZCHAR *pcExtMType, ZCHAR ucMSType, ZCHAR *pcExtMSType);

/**
 * @brief Set content disposition of a message exchange.
 * It will add Content-Disposition header and disposition parameter with file name.
 *
 * @param [in] pSmsg The message id.
 * @param [in] pcFileName The disposition file name.
 *
 * @retval ZOK Set the content disposition successfully.
 * @retval ZFAILED Set the content disposition failed.
 *
 * @see @ref Msrp_MsgSetCType
 */
ZFUNC ZINT Msrp_MsgSetRender(ZVOID *pSmsg, ZCHAR *pcFileName);

/**
 * @brief Set report option of a message exchange.
 *
 * @param [in] pSmsg The message id.
 * @param [in] bSuccRpt The successful report flag.
 * @param [in] bFailRpt The failure report flag.
 *
 * @retval ZOK Set the report option successfully.
 * @retval ZFAILED Set the report option failed.
 *
 * @see @ref Msrp_MsgOpen
 */
ZFUNC ZINT Msrp_MsgSetRptOpt(ZVOID *pSmsg, ZBOOL bSuccRpt, 
                ZBOOL bFailRpt);

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_UI_H__ */

