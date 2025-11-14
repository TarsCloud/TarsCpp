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
  File name     : imap_wrap.h
  Module        : imap protocol 
  Author        : tanek.ye
  Version       : 0.1
  Created on    : 2013-04-22
  Description   :
    Marcos and structure definitions required by the imap protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_WRAP_H__
#define _IMAP_WRAP_H__

#ifdef __cplusplus
extern "C"{
#endif

/* imap wrap capability type */
typedef enum EN_IMAP_WRAP_CAP_TYPE
{
    EN_IMAP_WRAP_CAP_IMAP41,         /* IMAP4rev1 */
    EN_IMAP_WRAP_CAP_IDLE,           /* IDLE, rfc2177 */
    EN_IMAP_WRAP_CAP_UIDPLUS,        /* UIDPLUS, rfc2359 */
    EN_IMAP_WRAP_CAP_ID,             /* ID, rfc2971 */
    EN_IMAP_WRAP_CAP_URLAUTH,        /* URLAUTH, rfc4467 */
    EN_IMAP_WRAP_CAP_CONDSTORE,      /* CONDSTORE, rfc4551 */
    EN_IMAP_WRAP_CAP_ENABLE,         /* ENABLE, rfc5161 */
    EN_IMAP_WRAP_CAP_QRESYNC,        /* QRESYNC, rfc5162 */
    EN_IMAP_WRAP_CAP_CONVERT,        /* CONVERT, rfc5259 */
    EN_IMAP_WRAP_CAP_METADATA,       /* METADATA, rfc5464 */
    EN_IMAP_WRAP_CAP_NOTIFY,         /* NOTIFY, rfc5465 */
    EN_IMAP_WRAP_CAP_SPECIAL_USE,    /* SPECIAL-USE, rfc6154 */
    EN_IMAP_WRAP_CAP_CTM,            /* custom capability */
} EN_IMAP_WRAP_CAP_TYPE;

/* imap wrap mailbox attribute type */
typedef enum EN_IMAP_WRAP_MBX_ATTR_TYPE
{
    EN_IMAP_WRAP_MBX_ATTR_NOSELECT,  /* \Noselect */
    EN_IMAP_WRAP_MBX_ATTR_MARKED,    /* \Marked */
    EN_IMAP_WRAP_MBX_ATTR_UNMARKED,  /* \Unmarked */
    EN_IMAP_WRAP_MBX_ATTR_NOINFERIORS, /* \Noinferiors */
    EN_IMAP_WRAP_MBX_ATTR_EXTENSION, /* extension attribute */
} EN_IMAP_WRAP_MBX_FALG_TYPE;

/* imap wrap flag type */
typedef enum EN_IMAP_WRAP_FLAG_TYPE
{
    EN_IMAP_WRAP_FLAG_ANSWERED = EN_IMAP_FLAG_ANSWERED, /* \Answered */
    EN_IMAP_WRAP_FLAG_FLAGGED = EN_IMAP_FLAG_FLAGGED, /* \Flagged */
    EN_IMAP_WRAP_FLAG_DELETED = EN_IMAP_FLAG_DELETED, /* \Deleted */
    EN_IMAP_WRAP_FLAG_SEEN = EN_IMAP_FLAG_SEEN, /* \Seen */
    EN_IMAP_WRAP_FLAG_DRAFT = EN_IMAP_FLAG_DRAFT, /* \Draft */
    EN_IMAP_WRAP_FLAG_RECENT = EN_IMAP_FLAG_RECENT, /* \Recent */
    EN_IMAP_WRAP_FLAG_KEYWORD = EN_IMAP_FLAG_KEY_WORD, /* flag keyword */
    EN_IMAP_WRAP_FLAG_EXT = EN_IMAP_FLAG_EXTENSION, /* extension flag */
} EN_IMAP_WRAP_FLAG_TYPE;

/** 
 * @brief IMAP open message store.
 *
 * @param [in] *pcIpAddr Message store server IP address.
 * @param [in] wServPort Message store server port.
 * @param [in] *pcUser User name.
 * @param [in] *pcPwd Password.
 * @param [in] *pcMsisdn Msisdn.
 * @param [in] bUseTls Use tls flag.
 * @param [out] ppStoreId Store Id of the message store.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreClose
 */
ZFUNC ZINT Imap_StoreOpen(ZCHAR *pcServIp, ZUSHORT wServPort, 
                ZCHAR *pcUser, ZCHAR *pcPwd, ZCHAR* pcMsisdn, ZBOOL bUseTls, 
                ZVOID **ppStoreId);

/** 
 * @brief IMAP close message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_StoreClose(ZVOID * pStoreId);

/** 
 * @brief IMAP get capability count of message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [out] piCount The count of capability of message store.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_StoreCapGetCnt(ZVOID * pStoreId, ZUINT *piCount);

/** 
 * @brief IMAP enumerate capability of message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] iIndex Index of capability.
 * @param [out] piType Capability type, @ref EN_IMAP_WRAP_CAP_TYPE.
 * @param [out] pbIsAuth Auth flag.
 * @param [out] ppcName Capability name, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_StoreCapEnum(ZVOID * pStoreId, ZUINT iIndex, 
                ZUINT *piType, ZBOOL *pbIsAuth, ZCHAR **ppcName);

/** 
 * @brief IMAP check whether specific capability is set on message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] iCapType Capability type, @ref EN_IMAP_WRAP_CAP_TYPE.
 * @param [out] pbIsSet ZTRUE if capability is set, ZFALSE if not.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_StoreCapIsSet(ZVOID * pStoreId, ZUINT iCapType, 
                ZBOOL *pbIsSet);

/** 
 * @brief IMAP check whether specific capability is set on message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] *pcCapName Capability name.
 * @param [out] pbIsSet ZTRUE if capability is set, ZFALSE if not.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_StoreCapIsSetX(ZVOID * pStoreId, ZCHAR *pcCapName, 
                ZBOOL *pbIsSet);

/** 
 * @brief IMAP get mailbox count of message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [out] piCount Count of mailbox of message store.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxGetCnt(ZVOID * pStoreId, ZUINT *piCount);

/** 
 * @brief IMAP enumerate mailbox of message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] iIndex Index of mailbox.
 * @param [out] ppMbxId Mailbox id.
 * @param [out] ppcMbxName Mailbox name, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxEnum(ZVOID * pStoreId, ZUINT iIndex, ZVOID **ppMbxId, 
                ZCHAR **ppcMbxName);

/* IMAP find mailbox by name */
/** 
 * @brief IMAP find mailbox by name.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] pcMbxName Name of mailbox.
 * @param [out] ppMbxId Mailbox id if found.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxFindByName(ZVOID * pStoreId, ZCHAR *pcMbxName, 
                ZVOID **ppMbxId);

/** 
 * @brief IMAP create mailbox on message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] pcMbxName Name of mailbox.
 * @param [out] ppMbxId Newly created mailbox id.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxCreate(ZVOID * pStoreId, ZCHAR *pcMbxName, 
                ZVOID **ppMbxId);

/** 
 * @brief IMAP delete mailbox on message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxDelete(ZVOID * pStoreId, ZVOID * pMbxId);

/** 
 * @brief IMAP renmae mailbox on message store.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] pcNewName New mailbox name.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxRename(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZCHAR *pcNewName);

/** 
 * @brief IMAP set mailbox active.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] bReadOnly Flag whether open with readonly.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxSetActive(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZBOOL bReadOnly);

/** 
 * @brief IMAP set mailbox inactive.
 *
 * @param [in] pStoreId Store Id of the message store.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxSetInactive(ZVOID * pStoreId);

/** 
 * @brief IMAP get attribute count of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [out] piCount Attribute Count of mailbox
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxAttrGetCnt(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT *piCount);

/** 
 * @brief IMAP enumerate attribute of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of attribute.
 * @param [out] piAttrType Attribute type, @ref EN_IMAP_WRAP_MBX_ATTR_TYPE.
 * @param [out] ppcAttrName Attribute name, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxAttrEnum(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZUINT *piAttrType, ZCHAR **ppcAttrName);

/** 
 * @brief IMAP check whether attribute is set on mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iAttrType Attribute type, @ref EN_IMAP_WRAP_MBX_ATTR_TYPE.
 * @param [out] pbIsSet ZTURE if attribute is set, ZFALSE if not.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxAttrIsSet(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iAttrType, ZBOOL *pbIsSet);

/** 
 * @brief IMAP check whether attribute is set on mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] pcAttName Attribute name.
 * @param [out] pbIsSet ZTURE if attribute is set, ZFALSE if not.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxAttrIsSetX(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZCHAR *pcAttName, ZBOOL *pbIsSet);

/** 
 * @brief IMAP get flag count of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [out] piCount Flag count of mailbox.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxFlagGetCnt(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT *piCount);

/** 
 * @brief IMAP enumerate flag of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Flag index.
 * @param [out] piFlagType Flag type, @ref EN_IMAP_WRAP_FLAG_TYPE.
 * @param [out] pbPrem Flag of permanent.
 * @param [out] ppcFlagName Flag name, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxFlagEnum(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZUINT *piFlagType, ZBOOL *pbPrem, 
                ZCHAR **ppcFlagName);

/** 
 * @brief IMAP check whether flag is set on mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iFlagType Flag type, @ref EN_IMAP_WRAP_FLAG_TYPE.
 * @param [out] pbIsSet ZTURE if flag is set, ZFALSE if not.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxFlagIsSet(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iFlagType, ZBOOL *pbIsSet);

/** 
 * @brief IMAP check whether flag is set on mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] pcFlagName Flag name.
 * @param [out] pbIsSet ZTURE if flag is set, ZFALSE if not.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxFlagIsSetX(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZCHAR *pcFlagName, ZBOOL *pbIsSet);

/** 
 * @brief IMAP get next unique id of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [out] piUidNext Next unique id of mailbox.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxGetUidNext(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT *piUidNext);

/** 
 * @brief IMAP get unique id validity value of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [out] piUidVad Unique id validity value of mailbox.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxGetUidVad(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT *piUidVad);

/** 
 * @brief IMAP get access control list count of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [out] piCount Access control list count of mailbox.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxGetAclCnt(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT *piCount);

/** 
 * @brief IMAP enumerate access control list of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of access control list.
 * @param [out] ppcUserId User id, user should use Zos_SysSyrFree to free it.
 * @param [out] piRightSet Rigths set of user.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxAclEnum(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZCHAR **ppcUserId, ZUINT *piRightSet);

/** 
 * @brief IMAP set access control list of mailbox for specific user.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] pcUserId User id.
 * @param [in] iRightSet Rigths set of user.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxSetAcl(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZCHAR *pcUserId, ZUINT iRightSet);

/** 
 * @brief IMAP delete access control list of mailbox for specific user.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] pcUserId User id.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxDelAcl(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZCHAR *pcUserId);

/** 
 * @brief IMAP get access control list of mailbox for specific user.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] pcUserId User id.
 * @param [out] piRightSet Rights set of specific user.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxGetRight(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZCHAR *pcUserId, ZUINT *piRightSet);

/** 
 * @brief IMAP get access control list of mailbox for current user.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [out] piRightSet Rights set of current user.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MbxMyRight(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT *piRightSet);

/** 
 * @brief IMAP get message count of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [out] piMgsCnt Count of message exisits in mailbox.
 * @param [out] piRecent Count of message with \Recent flag.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetCnt(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT *piMgsCnt, ZUINT *piRecent);

/** 
 * @brief IMAP enumerate message of mailbox.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 * @param [out] piSeq Sequence id of message.
 * @param [out] piUid Unique id of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgEnum(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZUINT *piSeq, ZUINT *piUid);

/** 
 * @brief IMAP get flag count of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 * @param [out] piCount Flag count of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgFlagGetCnt(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZUINT *piCount);

/** 
 * @brief IMAP enumerate flag of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iMsgIndex Index of message.
 * @param [in] iFlagIndex Index of flag.
 * @param [out] piFlagType Flag type, @ref EN_IMAP_WRAP_FLAG_TYPE.
 * @param [out] ppcFlagName Flag name, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgFlagEnum(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iMsgIndex, ZUINT iFlagIndex, ZUINT *piFlagType, 
                ZCHAR **ppcFlagName);

/** 
 * @brief IMAP check whether flag is set on message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 * @param [in] iFlagType Flag type, @ref EN_IMAP_WRAP_FLAG_TYPE.
 * @param [in] pbIsSet ZTRUE if flag is set, ZFALSE if not.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgFlagIsSet(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZUINT iFlagType, ZBOOL *pbIsSet);

/** 
 * @brief IMAP add message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] dwMsgId Message id, create by ImapMsgAppCreate.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgAdd(ZVOID * pStoreId, ZVOID * pMbxId, ZVOID * pMsgId);

/** 
 * @brief IMAP delete message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgDelete(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex);

/** 
 * @brief IMAP delete message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iUid Unique id of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgDeleteX(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iUid);

/** 
 * @brief IMAP copy message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iDstMbxId Destination mailbox id.
 * @param [in] iIndex Index of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgCopy(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZVOID * pDstMbxId, ZUINT iIndex);

/** 
 * @brief IMAP copy message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iDstMbxId Destination mailbox id.
 * @param [in] iUid Unique id of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgCopyX(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZVOID * pDstMbxId, ZUINT iUid);

/** 
 * @brief IMAP get modify sequence id of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 * @param [out] pqwModSeq Modify sequence id of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetModSeq(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZULLONG *pqwModSeq);

/** 
 * @brief IMAP get modify sequence id of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iUid Unique id of message.
 * @param [out] pqwModSeq Modify sequence id of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetModSeqX(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iUid, ZULLONG *pqwModSeq);

/** 
 * @brief IMAP get subject of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 * @param [out] ppcSubject Value of subject header, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetSubject(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZCHAR **ppcSubject);

/** 
 * @brief IMAP get subject of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iUid Unique id of message.
 * @param [out] ppcSubject Value of subject header, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetSubjectX(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iUid, ZCHAR **ppcSubject);

/** 
 * @brief IMAP get specific header value of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 * @param [in] pcHdrName Name of header.
 * @param [out] ppcHdrVal Value of specific header, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetHdr(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZCONST ZCHAR *pcHdrName, ZCHAR **ppcHdrVal);

/** 
 * @brief IMAP get specific header value of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iUid Unique id of message.
 * @param [in] pcHdrName Name of header.
 * @param [out] ppcHdrVal Value of specific header, user should use Zos_SysSyrFree to free it.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetHdrX(ZVOID * pStoreId, ZVOID * pMbxId, ZUINT iUid, 
                ZCONST ZCHAR *pcHdrName, ZCHAR **ppcHdrVal);

/** 
 * @brief IMAP get text of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iIndex Index of message.
 * @param [out] ppcText Text of message.
 * @param [out] pdwLen Text length of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetText(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iIndex, ZCHAR **ppcText, ZUINT *piLen);

/** 
 * @brief IMAP get text of message.
 *
 * @param [in] pStoreId Store Id of the message store.
 * @param [in] dwMbxId Mailbox id.
 * @param [in] iUid Unique id of message.
 * @param [out] ppcText Text of message.
 * @param [out] pdwLen Text length of message.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_MsgGetTextX(ZVOID * pStoreId, ZVOID * pMbxId, 
                ZUINT iUid, ZCHAR **ppcText, ZUINT *piLen);

/** 
 * @brief IMAP create temp message, used for Imap_MsgAdd.
 *
 * @param [in] pStoreId Store Id of the message store.
 *
 * @return Message id.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZVOID * Imap_WrapImfMsgCreate(ZVOID * pStoreId);

/** 
 * @brief IMAP delete temp message.
 *
 * @param [in] dwMsgId Message id, generate by Imap_WrapImfMsgCreate.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_WrapImfMsgDelete(ZVOID * pMsgId);

/** 
 * @brief IMAP add header to temp message.
 *
 * @param [in] dwMsgId Message id, generate by Imap_WrapImfMsgCreate.
 * @param [in] pcHdrName Header name.
 * @param [in] pcHdrVal Header value.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_WrapImfMsgAddHdr(ZVOID * pMsgId, ZCHAR *pcHdrName, 
                ZCHAR *pcHdrVal);

/** 
 * @brief IMAP add text to temp message.
 *
 * @param [in] dwMsgId Message id, generate by Imap_WrapImfMsgCreate.
 * @param [in] pcTxt Message text.
 * @param [in] dwLen Message text length.
 *
 * @retval ZOK on succeed.
 * @retval ZFAILED on failure.
 *
 * @see Imap_StoreOpen
 */
ZFUNC ZINT Imap_WrapImfMsgAddTxt(ZVOID * pMsgId, ZCHAR *pcTxt, 
                ZUINT iLen);

#ifdef __cplusplus
}
#endif

#endif /* _IMAP_WRAP_H__ */

