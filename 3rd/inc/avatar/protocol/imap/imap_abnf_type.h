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
  File name     : imap_abnf_type.h
  Module        : imap protocol
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Marcos and structure definitions required by the imap protocol.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_ABNF_TYPE_H__
#define _IMAP_ABNF_TYPE_H__
    
/** 
 * @file imap_abnf_type.h
 * @brief IMAP ABNF typedefs.
 */
#ifdef __cplusplus
extern "C" {
#endif

#define IMAP_REQ_TAG_LEN 16

/** @brief IMAP command type. */
typedef enum EN_IMAP_CMD_TYPE
{
    /* rfc3501 */
    EN_IMAP_CMD_CAPA,                /* CAPABILITY command */
    EN_IMAP_CMD_NOOP,                /* NOOP command */
    EN_IMAP_CMD_LOGOUT,              /* LOGOUT command */
    EN_IMAP_CMD_STARTTLS,            /* STARTTLS command */
    EN_IMAP_CMD_AUTH,                /* AUTHENTICATE command */
    EN_IMAP_CMD_LOGIN,               /* LOGIN command */
    EN_IMAP_CMD_SELECT,              /* SELECT command */
    EN_IMAP_CMD_EXAMINE,             /* EXAMINE command */
    EN_IMAP_CMD_CREATE,              /* CREATE command */
    EN_IMAP_CMD_DELETE,              /* DELETE command */  
    EN_IMAP_CMD_RENAME,              /* RENAME command */ 
    EN_IMAP_CMD_SUBSCRIBE,           /* SUBSCRIBE command */
    EN_IMAP_CMD_UNSUBSCRIBE,         /* UNSUBSCRIBE command */
    EN_IMAP_CMD_LIST,                /* LIST command */
    EN_IMAP_CMD_LSUB,                /* LSUB command */
    EN_IMAP_CMD_STATUS,              /* STATUS command */
    EN_IMAP_CMD_APPEND,              /* APPEND command */
    EN_IMAP_CMD_CHECK,               /* CHECK command */
    EN_IMAP_CMD_CLOSE,               /* CLOSE command */
    EN_IMAP_CMD_EXPUNGE,             /* EXPUNGE command */
    EN_IMAP_CMD_SEARCH,              /* SEARCH command */
    EN_IMAP_CMD_FETCH,               /* FETCH command */
    EN_IMAP_CMD_STORE,               /* STORE command */
    EN_IMAP_CMD_COPY,                /* COPY command */
    EN_IMAP_CMD_UID,                 /* UID command */

    /* rfc4314 */
    EN_IMAP_CMD_SETACL,              /* SETACL command */
    EN_IMAP_CMD_DELETEACL,           /* DELETEACL command */
    EN_IMAP_CMD_GETACL,             /* GETACL command */
    EN_IMAP_CMD_LISTRIGHTS,          /* LISTRIGHTS command */
    EN_IMAP_CMD_MYRIGHTS,           /* MYRIGHTS command */

    /* rfc4467 */
    EN_IMAP_CMD_RESETKEY,            /* RESETKEY command */
    EN_IMAP_CMD_GENURLAUTH,          /* GENURLAUTH command */
    EN_IMAP_CMD_URLFETCH,            /* URLFETCH command */

    /* rfc5161 */
    EN_IMAP_CMD_ENABLE,              /* ENABLE command */
    
    EN_IMAP_CMD_CTM,      
} EN_IMAP_CMD_TYPE;

/* imap response result type */
typedef enum EN_IMAP_RSP_TYPE
{
    /* rfc3501 */
    EN_IMAP_RSP_TYPE_OK,             /* OK response */
    EN_IMAP_RSP_TYPE_NO,             /* NO response */
    EN_IMAP_RSP_TYPE_BAD,            /* BAD response */
    EN_IMAP_RSP_TYPE_PREAUTH,        /* PREAUTH response */
    EN_IMAP_RSP_TYPE_BYE,            /* BYE response */
    EN_IMAP_RSP_TYPE_CAP,            /* CAPABILITY response */
    EN_IMAP_RSP_TYPE_LIST,           /* LIST response */
    EN_IMAP_RSP_TYPE_LSUB,           /* LSUB response */
    EN_IMAP_RSP_TYPE_STATUS,         /* STATUS response */
    EN_IMAP_RSP_TYPE_SEARCH,         /* SEARCH response */
    EN_IMAP_RSP_TYPE_FLAGS,          /* FLAGS response */
    EN_IMAP_RSP_TYPE_EXISTS,         /* EXISTS response */
    EN_IMAP_RSP_TYPE_RECENT,         /* RECENT response */
    EN_IMAP_RSP_TYPE_EXPUNGE,        /* EXPUNGE response */
    EN_IMAP_RSP_TYPE_FETCH,          /* FETCH response */

    /* rfc4314 */
    EN_IMAP_RSP_TYPE_ACL,            /* ACL response */
    EN_IMAP_RSP_TYPE_LISTRIGHTS,     /* LISTRIGHTS response */
    EN_IMAP_RSP_TYPE_MYRIGHTS,       /* MYRIGHTS response */

    /* rfc4467 */
    EN_IMAP_RSP_TYPE_GENURLAUTH,     /* GENURLAUTH response */
    EN_IMAP_RSP_TYPE_URLFETCH,       /* URLFETCH response */

    /* rfc5161 */
    EN_IMAP_RSP_TYPE_ENABLED,        /* ENABLED response */

    /* rfc5162 */
    EN_IMAP_RSP_TYPE_VANISHED,       /* VANISHED response */
    
    EN_IMAP_RSP_TYPE_CONT,           /* command continuation request response */
} EN_IMAP_RSP_TYPE;

/* imap flag type */
typedef enum EN_IMAP_FLAG_TYPE
{
    EN_IMAP_FLAG_ANSWERED,           /* flag of \Answered */
    EN_IMAP_FLAG_FLAGGED,            /* flag of \Flagged */
    EN_IMAP_FLAG_DELETED,            /* flag of \Deleted */
    EN_IMAP_FLAG_SEEN,               /* flag of \Seen */
    EN_IMAP_FLAG_DRAFT,              /* flag of \Draft */
    EN_IMAP_FLAG_RECENT,             /* flag of \Recent */
    EN_IMAP_FLAG_KEY_WORD,           /* flag-keyword */
    EN_IMAP_FLAG_EXTENSION,          /* flag-extension */
} EN_IMAP_FLAG_TYPE;

/* imap access right, rfc4314 */
typedef enum EN_IMAP_ACL_RIGHT
{
    EN_IMAP_ACL_RIGHT_L = 0x00000001, 
    EN_IMAP_ACL_RIGHT_R = 0x00000002, 
    EN_IMAP_ACL_RIGHT_S = 0x00000004, 
    EN_IMAP_ACL_RIGHT_W = 0x00000008, 
    EN_IMAP_ACL_RIGHT_I = 0x00000010, 
    EN_IMAP_ACL_RIGHT_P = 0x00000020, 
    EN_IMAP_ACL_RIGHT_K = 0x00000040, 
    EN_IMAP_ACL_RIGHT_X = 0x00000080, 
    EN_IMAP_ACL_RIGHT_T = 0x00000100, 
    EN_IMAP_ACL_RIGHT_E = 0x00000200, 
    EN_IMAP_ACL_RIGHT_A = 0x00000400, 
    EN_IMAP_ACL_RIGHT_0 = 0x00000800, 
    EN_IMAP_ACL_RIGHT_1 = 0x00001000, 
    EN_IMAP_ACL_RIGHT_2 = 0x00002000, 
    EN_IMAP_ACL_RIGHT_3 = 0x00004000, 
    EN_IMAP_ACL_RIGHT_4 = 0x00008000, 
    EN_IMAP_ACL_RIGHT_5 = 0x00010000, 
    EN_IMAP_ACL_RIGHT_6 = 0x00020000, 
    EN_IMAP_ACL_RIGHT_7 = 0x00040000, 
    EN_IMAP_ACL_RIGHT_8 = 0x00080000, 
    EN_IMAP_ACL_RIGHT_9 = 0x00100000, 
} EN_IMAP_ACL_RIGHT;

/* imap mailbox flag type */
typedef enum EN_IMAP_MBX_FLAG_TYPE
{
    EN_IMAP_MBX_FLAG_NOSELECT,       /* flag of \Noselect */
    EN_IMAP_MBX_FLAG_MARKED,         /* flag of \Marked */
    EN_IMAP_MBX_FLAG_UNMARKED,       /* flag of \Unmarked */
    EN_IMAP_MBX_FLAG_NOINFERIORS,    /* flag of \Noinferiors */
    EN_IMAP_MBX_FLAG_EXTENSION,      /* flag-extension */
} EN_IMAP_MBX_FLAG_TYPE;

/* imap response result type */
typedef enum EN_IMAP_RSP_RESULT_TYPE
{
    EN_IMAP_RSP_RESULT_NONE,         /* no completion presence */
    EN_IMAP_RSP_RESULT_OK,           /* successful completion of the associated command */
    EN_IMAP_RSP_RESULT_NO,           /* unsuccessful completion of the associated command */
    EN_IMAP_RSP_RESULT_BAD,          /* protocol-level error in the client's command */
} EN_IMAP_RSP_RESULT_TYPE;

/* imap response fetch item type */
typedef enum EN_IMAP_FETCH_ITEM_TYPE
{
    /* rfc3501 */
    EN_IMAP_FETCH_ITEM_BODY,         /* fetch item of BODY */
    EN_IMAP_FETCH_ITEM_BODY_STRUCT,  /* fetch item of BODYSTRUCTURE */
    EN_IMAP_FETCH_ITEM_ENVELOPE,     /* fetch item of ENVELOPE */
    EN_IMAP_FETCH_ITEM_FLAGS,        /* fetch item of FLAGS */
    EN_IMAP_FETCH_ITEM_INTER_DATE,   /* fetch item of INTERNALDATE */
    EN_IMAP_FETCH_ITEM_822,          /* fetch item of RFC822 */
    EN_IMAP_FETCH_ITEM_822_HDR,      /* fetch item of RFC822.HEADER */
    EN_IMAP_FETCH_ITEM_822_SIZE,     /* fetch item of RFC822.SIZE */
    EN_IMAP_FETCH_ITEM_822_TXT,      /* fetch item of RFC822.TEXT */
    EN_IMAP_FETCH_ITEM_UID,          /* fetch item of UID */

    /* rfc4551 */
    EN_IMAP_FETCH_ITEM_MODSEQ,       /* fetch item of MODSEQ */
    
    EN_IMAP_FETCH_ITEM_FULL,         /* fetch item of FULL */
    
    EN_IMAP_FETCH_ITEM_BODY_SEC,     /* fetch item of BODY[section]<origin> */
} EN_IMAP_FETCH_ITEM_TYPE;

/* imap response fetch body section type */
typedef enum EN_IMAP_FETCH_SEC_TYPE
{
    EN_IMAP_FETCH_SEC_HDR,       /* section of HEADER */
    EN_IMAP_FETCH_SEC_HDR_FLD,   /* section of HEADER.FIELDS */
    EN_IMAP_FETCH_SEC_HDR_FLD_NOT, /* section of HEADER.FIELDS.NOT */
    EN_IMAP_FETCH_SEC_TXT,       /* section of TEXT */
    EN_IMAP_FETCH_SEC_PART,      /* section of part */
    EN_IMAP_FETCH_SEC_ALL,       /* section of ALL */
} EN_IMAP_FETCH_SEC_TYPE;

/* imap response code type */
typedef enum EN_IMAP_RSP_CODE_TYPE
{
    /* rfc3501 */
    EN_IMAP_RSP_CODE_ALERT,          /* response code of ALERT */
    EN_IMAP_RSP_CODE_BAD_CHR,        /* response code of BADCHARSET */
    EN_IMAP_RSP_CODE_CAP,            /* response code of capability */
    EN_IMAP_RSP_CODE_PARSE,          /* response code of PARSE */
    EN_IMAP_RSP_CODE_PRE_FLAGS,      /* response code of PERMANENTFLAGS */
    EN_IMAP_RSP_CODE_RD,             /* response code of READ-ONLY */
    EN_IMAP_RSP_CODE_RW,             /* response code of READ-WRITE */
    EN_IMAP_RSP_CODE_TRY_CREATE,     /* response code of TRYCREATE */
    EN_IMAP_RSP_CODE_UID_NEXT,       /* response code of UIDNEXT */
    EN_IMAP_RSP_CODE_UID_VAD,        /* response code of UIDVALIDITY */
    EN_IMAP_RSP_CODE_UNSEEN,         /* response code of UNSEEN */

    /* rfc4467 */
    EN_IMAP_RSP_CODE_URLMECH,        /* response code of URLMECH */

    /* rfc4551 */
    EN_IMAP_RSP_CODE_HIGHESTMODSEQ,  /* response code of HIGHESTMODSEQ */
    EN_IMAP_RSP_CODE_NOMODSEQ,       /* response code of NOMODSEQ */
    EN_IMAP_RSP_CODE_MODIFIED,       /* response code of MODIFIED */

    /* rfc5162 */
    EN_IMAP_RSP_CODE_CLOSED,         /* response code of CLOSED */
    
    EN_IMAP_RSP_CODE_CTM,            /* response code of custom */
} EN_IMAP_RSP_CODE_TYPE;

/* imap request store type */
typedef enum EN_IMAP_REQ_STORE_TYPE
{
    EN_IMAP_REQ_STORE_SET,           /* replace the flags for the message (other than \Recent) with the argument */
    EN_IMAP_REQ_STORE_ADD,           /* add the argument to the flags for the message */
    EN_IMAP_REQ_STORE_REMOVE,        /* remove the argument from the flags for the message */
} EN_IMAP_REQ_STORE_TYPE;

/* imap request status command type */
typedef enum EN_IMAP_REQ_STAT_TYPE
{
    EN_IMAP_REQ_STAT_MSG      = 1,   /* MESSAGES */
    EN_IMAP_REQ_STAT_RECENT   = 2,   /* RECENT */
    EN_IMAP_REQ_STAT_UID_NEXT = 4,   /* UIDNEXT */
    EN_IMAP_REQ_STAT_UID_VAD  = 8,   /* UIDVALIDITY */
    EN_IMAP_REQ_STAT_UNSEEN   = 16,  /* UNSEEN */
} EN_IMAP_REQ_STAT_TYPE;

/* imap request status command type */
typedef enum EN_IMAP_REQ_STATUS_TKN_TYPE
{
    EN_IMAP_REQ_STATUS_TKN_MSG,      /* MESSAGES */
    EN_IMAP_REQ_STATUS_TKN_RECENT,   /* RECENT */
    EN_IMAP_REQ_STATUS_TKN_UID_NEXT, /* UIDNEXT */
    EN_IMAP_REQ_STATUS_TKN_UID_VAD,  /* UIDVALIDITY */
    EN_IMAP_REQ_STATUS_TKN_UNSEEN,   /* UNSEEN */
} EN_IMAP_REQ_STATUS_TKN_TYPE;

/* imap response parameter pair */
typedef struct tagIMAP_PARM_PAIR
{
    ST_ZOS_SSTR stParmName;          /* parameter name */
    ST_ZOS_SSTR stParmVal;           /* parameter value */
} ST_IMAP_PARM_PAIR;

/* imap header */
typedef struct tagIMAP_HDR
{
    ST_ZOS_SSTR stHdrName;           /* header name */
    ST_ZOS_SSTR stHdrValue;          /* header value */
} ST_IMAP_HDR;

/* imap media type */
typedef struct tagIMAP_MEDIA_TYPE
{
    ST_ZOS_SSTR stMType;             /* main mime type */
    ST_ZOS_SSTR stSType;             /* sub mime type */
    ST_ZOS_DLIST stParmLst;          /* parameter list of Content-type header, ST_IMAP_PARM_PAIR */
} ST_IMAP_MEDIA_TYPE;

/* imap sequence set */
typedef struct tagIMAP_SEQ_RANGE
{
    ZUINT iSeqBegin;                 /* begine of sequence */
    ZUINT iSeqEnd;                   /* end of sequence */
} ST_IMAP_SEQ_RANGE;

/* imap sequence set */
typedef struct tagIMAP_SEQ_SET
{
    ST_ZOS_DLIST stSeqLst;           /* sequence list */
    ST_ZOS_DLIST stSeqRangeLst;      /* sequence range list */
} ST_IMAP_SEQ_SET;

/* imap flag item in response */
typedef struct tagIMAP_RSP_FLAG
{
    ZUINT iFlagType;                 /* type of flag item, EN_IMAP_FLAG_TYPE */
    ST_ZOS_SSTR stFlagName;          /* flag name */
} ST_IMAP_RSP_FLAG;

/* imap response fetch address of envelope */
typedef struct tagIMAP_RSP_FETCH_ENV_ADDR
{
    ST_ZOS_SSTR stName;              /* personal name */
    ST_ZOS_SSTR stDomainLst;         /* at-domain-list */
    ST_ZOS_SSTR stMbxName;           /* mailbox name */
    ST_ZOS_SSTR stHostName;          /* host name */
} ST_IMAP_RSP_FETCH_ENV_ADDR;

/* imap response fetch item of ENVELOPE */
typedef struct tagIMAP_RSP_FETCH_ENV
{
    ST_ZOS_SSTR stDate;              /* date string of envelope */
    ST_ZOS_SSTR stSubject;           /* subject string of envelope */
    ST_ZOS_SSTR stInReplyTo;         /* in-reply-to string of envelope */
    ST_ZOS_SSTR stMsgId;             /* message-id string of envelope */
    ST_ZOS_DLIST stFromLst;          /* list of from address, ST_IMAP_RSP_ADDR */
    ST_ZOS_DLIST stSenderLst;        /* list of sender address, ST_IMAP_RSP_ADDR */
    ST_ZOS_DLIST stReplyToLst;       /* list of reply-to address, ST_IMAP_RSP_ADDR */
    ST_ZOS_DLIST stToLst;            /* list of to address, ST_IMAP_RSP_ADDR */
    ST_ZOS_DLIST stCcLst;            /* list of cc address, ST_IMAP_RSP_ADDR */
    ST_ZOS_DLIST stBccLst;           /* list of bcc address, ST_IMAP_RSP_ADDR */
} ST_IMAP_RSP_FETCH_ENV;

typedef struct tagIMAP_RSP_FETCH_BDY_PART
{
    ST_ZOS_SSTR stFileName;          /* saved file name */
    ST_ZOS_SSTR stBody;             /* body */
    ST_ZOS_DLIST stHdrLst;           /* header list, ST_IMAP_HDR */
    ST_IMAP_MEDIA_TYPE stMType;      /* media type */
    ZFILE zFile;                     /* file handle */
    ZBOOL bBase64;                   /* base64 flag */
} ST_IMAP_RSP_FETCH_BDY_PART;

/* imap response fetch item of BODY[section]<origin> */
typedef struct tagIMAP_RSP_FETCH_SEC
{
    ZUINT iOffset;                   /* offset of section data */
    ZUINT iSecType;                  /* section type, EN_IMAP_FETCH_SEC_TYPE */
    ZUINT iSecSize;                  /* size of section */
    ST_ZOS_SSTR stSecValue;         /* value of section */
    ST_ZOS_DLIST stHdrFldLst;        /* header fields list */
    ST_ZOS_DLIST stHdrLst;           /* header list, ST_IMAP_HDR */
    ST_ZOS_DLIST stPartLst;          /* part list of body, ST_IMAP_RSP_FETCH_BDY_PART */
    ST_IMAP_MEDIA_TYPE stCType;      /* content type */
} ST_IMAP_RSP_FETCH_SEC;

/* imap response fetch of body disposition */
typedef struct tagIMAP_RSP_FETCH_STRUCT_DSP
{
    ST_ZOS_SSTR stType;              /* body MD5 value */
    ST_ZOS_DLIST stParmLst;          /* list of attribute/value pairs, ST_IMAP_PARM_PAIR */
} ST_IMAP_RSP_FETCH_STRUCT_DSP;

/* imap response extension of single part */
typedef struct tagIMAP_RSP_DBY_EXT_ONE
{
    ST_ZOS_SSTR stMd5;               /* body MD5 value */
    ST_ZOS_SSTR stLoc;               /* body content URI */
    ST_ZOS_DLIST stLangLst;          /* list of body language value, ST_ZOS_SSTR */
    ST_IMAP_RSP_FETCH_STRUCT_DSP stDsp; /* disposition */
} ST_IMAP_RSP_DBY_EXT_ONE;

/* imap response extension of multi part */
typedef struct tagIMAP_RSP_DBY_EXT_MULTI
{
    ST_ZOS_SSTR stLoc;               /* body content URI */
    ST_ZOS_DLIST stParmLst;          /* list of attribute/value pairs, ST_IMAP_PARM_PAIR */
    ST_ZOS_DLIST stLangLst;          /* list of body language value, ST_ZOS_SSTR */
    ST_IMAP_RSP_FETCH_STRUCT_DSP stDsp; /* disposition */
} ST_IMAP_RSP_DBY_EXT_MULTI;

struct tagIMAP_RSP_BDY;

/* imap response multi part body structure */
typedef struct tagIMAP_RSP_BDY_MULTI
{
    ST_ZOS_SSTR stType;              /* main type */
    ST_ZOS_SSTR stSType;             /* subtype */
    ST_ZOS_DLIST stBdyLst;           /* body part list */
    ST_IMAP_RSP_DBY_EXT_MULTI stExt; /* extension */
} ST_IMAP_RSP_BDY_MULTI;

/* imap response single part body structure */
typedef struct tagIMAP_RSP_BDY_ONE
{
    ZUINT iSize;                     /* size of body in octets */
    ZUINT iLineCnt;                  /* line count in TEXT or MESSAGE/RFC822 */
    ST_ZOS_SSTR stType;              /* content media type name */
    ST_ZOS_SSTR stSubType;           /* content subtype name */
    ST_ZOS_SSTR stMsgId;             /* message id */
    ST_ZOS_SSTR stDesc;              /* content description  */
    ST_ZOS_SSTR stEncoding;          /* content transfer encoding  */
    ST_ZOS_DLIST stParmLst;          /* list of attribute/value pairs, ST_IMAP_PARM_PAIR */
    ST_IMAP_RSP_FETCH_ENV stEnv;     /* envelope structure in MESSAGE/RFC822 */
    struct tagIMAP_RSP_BDY *pstBody; /* body structure in MESSAGE/RFC822 */
    ST_IMAP_RSP_DBY_EXT_ONE stExt;   /* extension */
} ST_IMAP_RSP_BDY_ONE;

/* imap response body structure */
typedef struct tagIMAP_RSP_BDY
{
    ZBOOL bMulti;                    /* multi part flag */
    ST_ZOS_SSTR stPath;              /* body path */
    union
    {
        ST_IMAP_RSP_BDY_MULTI stMulti; /* multi part body */
        ST_IMAP_RSP_BDY_ONE stOne;   /* single part body */
    } u;
} ST_IMAP_RSP_BDY;

/* imap response fetch item */
typedef struct tagIMAP_RSP_FETCH_ITEM
{
    ZUINT iItemType;                 /* response fetch item type, EN_IMAP_FETCH_ITEM_TYPE */
    union
    {
        ZUINT i822Size;              /* fetch item of RFC822.SIZE */
        ZUINT iUid;                  /* fetch item of UID */
        ZULLONG qwModSeq;            /* fetch item of MODSEQ, rfc4551 */
        ST_ZOS_SSTR stInterDate;     /* fetch item of INTERNALDATE, string of internal date */
        ST_ZOS_SSTR stRfc822;       /* fetch item of RFC822 */
        ST_ZOS_SSTR stRfc822Hdr;    /* fetch item of RFC822.HEADER */
        ST_ZOS_SSTR stRfc822Txt;    /* fetch item of RFC822.TEXT */
        ST_ZOS_DLIST stFlagLst;      /* fetch item of FLAGS, flags list, ST_IMAP_RSP_FLAG */
        ST_IMAP_RSP_BDY stStruct;    /* fetch item of BODYSTRUCTURE */
        ST_IMAP_RSP_BDY stBody;      /* fetch item of BODY */
        ST_IMAP_RSP_FETCH_ENV stEnvelope; /* fetch item of ENVELOPE */
        ST_IMAP_RSP_FETCH_SEC stBodySec; /* fetch item of BODY[section]<origin.octet> */
    } u;
} ST_IMAP_RSP_FETCH_ITEM; 

/* imap custom response code */
typedef struct tagIMAP_RSP_CODE_CTM
{
    ST_ZOS_SSTR stCodeName;         /* custom name of response code */
    ST_ZOS_SSTR stCodeVal;          /* custom value of response code */
} ST_IMAP_RSP_CODE_CTM;

/* imap URLMECH response code custom mechanism */
typedef struct tagIMAP_RSP_CODE_URLMECH_CTM
{
    ST_ZOS_SSTR stMechName;         /* mechanism name */
    ST_ZOS_SSTR stMechData;         /* mechanism specific data, in base64 */
} ST_IMAP_RSP_CODE_URLMECH_CTM;

/* imap response code */
typedef struct tagIMAP_RSP_CODE
{
    ZUINT iCodeType;                 /* response code type. EN_IMAP_RSP_CODE_TYPE */
    union
    {
        ZUINT iUidNext;              /* response code of UIDNEXT */
        ZUINT iUidVad;               /* response code of UIDVALIDITY */
        ZUINT iUnseen;               /* response code of UNSEEN */
        ZULLONG qwHModSeq;           /* response code of HIGHESTMODSEQ, rfc4551 */
        ST_ZOS_DLIST stBadChrLst;    /* response code of BADCHARSET */
        ST_ZOS_DLIST stCapLst;       /* response code of capability */
        ST_ZOS_DLIST stPerFlagsLst;  /* response code of PERMANENTFLAGS, ST_IMAP_RSP_FLAG */
        ST_ZOS_DLIST stCtmMechLst;   /* custom mechanism list, ST_IMAP_RSP_CODE_URLMECH_CTM */
        ST_IMAP_RSP_CODE_CTM stCtm;  /* response code of custom */
        ST_IMAP_SEQ_SET stModified;  /* response code of MODIFIED */
    } u;
} ST_IMAP_RSP_CODE;

/* imap response acl list item */
typedef struct tagIMAP_RSP_ACL_ITEM
{
    ZUINT iRightSet;                 /* rights set of specific identifier */
    ST_ZOS_SSTR stUserId;            /* user identifier */
} ST_IMAP_RSP_ACL_ITEM;

/* imap OK response */
typedef struct tagIMAP_RSP_OK
{
    ZBOOL bCodePres;                 /* flag indecate whether response code presence */
    ST_ZOS_SSTR stRspText;           /* response text */
    ST_IMAP_RSP_CODE stCode;         /* response code */
} ST_IMAP_RSP_OK;

/* imap NO response */
typedef struct tagIMAP_RSP_NO
{
    ZBOOL bCodePres;                 /* flag indecate whether response code presence */
    ST_ZOS_SSTR stRspText;           /* response text */
    ST_IMAP_RSP_CODE stCode;         /* response code */
} ST_IMAP_RSP_NO;

/* imap BAD response */
typedef struct tagIMAP_RSP_BAD
{
    ZBOOL bCodePres;                 /* flag indecate whether response code presence */
    ST_ZOS_SSTR stRspText;           /* response text */
    ST_IMAP_RSP_CODE stCode;         /* response code */
} ST_IMAP_RSP_BAD;

/* imap PREAUTH response */
typedef struct tagIMAP_RSP_PREAUTH
{
    ZBOOL bCodePres;                 /* flag indecate whether response code presence */
    ST_ZOS_SSTR stRspText;           /* response text */
    ST_IMAP_RSP_CODE stCode;         /* response code */
} ST_IMAP_RSP_PREAUTH;

/* imap BYE response */
typedef struct tagIMAP_RSP_BYE
{
    ZBOOL bCodePres;                 /* flag indecate whether response code presence */
    ST_ZOS_SSTR stRspText;           /* response text */
    ST_IMAP_RSP_CODE stCode;         /* response code */
} ST_IMAP_RSP_BYE;

/* imap FETCH response */
typedef struct tagIMAP_RSP_FETCH
{
    ZUINT iSeqNum;                   /* sequence number of message */
    ST_ZOS_DLIST stItemLst;          /* fetch item list for FETCH response, ST_IMAP_RSP_FETCH_ITEM */
} ST_IMAP_RSP_FETCH;

/* imap response mailbox for LIST/LSUB command */
typedef struct tagIMAP_RSP_MAILBOX
{
    ZCHAR cHieChar;                  /* hierarchy delimiter char */
    ST_ZOS_SSTR stMbxName;           /* name of mailbox */
    ST_ZOS_DLIST stFlagLst;          /* flags list of mailbox */
} ST_IMAP_RSP_MAILBOX;

/* imap STATUS response */
typedef struct tagIMAP_RSP_STATUS
{
    ZUINT iMsgCnt;                   /* number of messages in the mailbox */
    ZUINT iRecent;                   /* number of messages with \Recent flag */
    ZUINT iUidNext;                  /* next unique identifier value of the mailbox */
    ZUINT iUidVad;                   /* unique identifier validity value of the mailbox */
    ZUINT iUnseen;                   /* number of messages which do not have \Seen flag  */
    ST_ZOS_SSTR stMbxName;           /* name of the mailbox */
} ST_IMAP_RSP_STATUS;

/* imap ACL response */
typedef struct tagIMAP_RSP_ACL
{
    ST_ZOS_SSTR stMbxName;           /* mailbox name */
    ST_ZOS_DLIST stAclLst;           /* list of identifier rights pairs, ST_IMAP_RSP_ACL_ITEM */
} ST_IMAP_RSP_ACL;

/* imap LISTRIGHTS response */
typedef struct tagIMAP_RSP_LST_RIGHT
{
    ZUINT iRightSet;                 /* rights set */
    ST_ZOS_SSTR stMbxName;           /* mailbox name */
    ST_ZOS_SSTR stId;                /* identifier */
} ST_IMAP_RSP_LST_RIGHT;

/* imap LISTRIGHTS response */
typedef struct tagIMAP_RSP_MY_RIGHT
{
    ZUINT iRightSet;                 /* rights set */
    ST_ZOS_SSTR stMbxName;           /* mailbox name */
} ST_IMAP_RSP_MY_RIGHT;

/* imap URLFETCH response URL/nstring pairs */
typedef struct tagIMAP_RSP_URL_NSTR_PAIR
{
    ST_ZOS_SSTR stUrl;               /* imap url */
    ST_ZOS_SSTR stMsgTxt;           /* message text data */
} ST_IMAP_RSP_URL_NSTR_PAIR;

/* imap SEARCH response*/
typedef struct tagIMAP_RSP_SEARCH
{
    ZUINT iModSeq;                   /* mod-sequence, rfc4551 */
    ST_ZOS_DLIST stNumLst;           /* sequence/uid list */
} ST_IMAP_RSP_SEARCH;

/* imap VANISHED response */
typedef struct tagIMAP_RSP_VANISHED
{
    ZBOOL bEarlier;                  /* flag of 'EARLIER' presence */
    ST_IMAP_SEQ_SET stSeqSet;        /* sequence set */
} ST_IMAP_RSP_VANISHED;

/* imap response */
typedef struct tagIMAP_RSP_UNTAGGED
{
    ZUINT iRspType;                  /* response type, EN_IMAP_RSP_TYPE */
    union 
    {
        ZUINT iExists;               /* EXISTS response */
        ZUINT iRecent;               /* RECENT response */
        ZUINT iExpunge;              /* EXPUNGE response */
        ST_ZOS_SSTR stContTxt;       /* text of continuation response */
        ST_IMAP_RSP_OK stOk;         /* OK response */
        ST_IMAP_RSP_NO stNo;         /* NO response */
        ST_IMAP_RSP_BAD stBad;       /* BAD response */
        ST_IMAP_RSP_PREAUTH stPreAuth; /* PREAUTH response */
        ST_IMAP_RSP_BYE stBye;       /* BYE response */
        ST_IMAP_RSP_FETCH stFetch;   /* FETCH response */
        ST_IMAP_RSP_MAILBOX stListMbx; /* mailbox info for LIST command */
        ST_IMAP_RSP_MAILBOX stLsubMbx; /* mailbox info for LSUB command */
        ST_IMAP_RSP_STATUS stStatus; /* STATUS response */ 
        ST_IMAP_RSP_ACL stAcl;       /* ACL response, rfc4314 */
        ST_IMAP_RSP_LST_RIGHT stLstRight; /* LISTRIGHTS response, rfc4314 */
        ST_IMAP_RSP_MY_RIGHT stMyRight; /* MYRIGHTS response, rfc4314 */
        ST_IMAP_RSP_SEARCH stSearch; /* SEARCH response */
        ST_IMAP_RSP_VANISHED stVanished; /* VANISHED response */
        ST_ZOS_DLIST stCapLst;       /* capability list, ST_ZOS_SSTR */
        ST_ZOS_DLIST stSearchNumLst; /* number list, sequence numbers for SEARCH command; unique identifiers for UID SEARCH, ZUINT */
        ST_ZOS_DLIST stFlagLst;      /* flag list for FLAGS response, ST_IMAP_RSP_FLAG */
        ST_ZOS_DLIST stGenUrlLst;    /* flag list for GENURLAUTH response, ST_ZOS_SSTR, rfc4467 */
        ST_ZOS_DLIST stUrlFetchLst;  /* flag list for URLFETCH response, ST_IMAP_RSP_URL_NSTR_PAIR, rfc4467 */
        ST_ZOS_DLIST stEnabledLst;   /* capability list for ENABLED response, ST_ZOS_SSTR, rfc5161 */ 
    } u;
} ST_IMAP_RSP_UNTAGGED;

/* imap one complete response */
typedef struct tagIMAP_RSP_COMPLETE
{
    ZUINT iRetType;                  /* response result type, EN_IMAP_RSP_RESULT_TYPE */
    ZBOOL bCodePres;                 /* flag indecate whether response code presence */
    ZDBUF zMsgBuf;                 /* msg buf */
    ST_ZOS_SSTR stTag;               /* tag of queset */
    ST_ZOS_SSTR stRspText;           /* response text */
    ST_ZOS_DLIST stRspLst;           /* untagged response list. ST_IMAP_RSP_UNTAGGED */
    ST_IMAP_RSP_CODE stCode;         /* response code */
} ST_IMAP_RSP_COMPLETE;

/** @brief IMAP response. */
typedef struct tagIMAP_RSP_MSG
{
    ZDBUF zMsgBuf;                 /* msg buf */
    ST_ZOS_SSTR stMsgStr;           /* message string */
    ST_ZOS_DLIST stRspLst;           /* response list. ST_IMAP_RSP_COMPLETE */
} ST_IMAP_RSP_MSG;

/* imap parameter of FETCH command BODY[section]<> */
typedef struct tagIMAP_REQ_FETCH_SEC
{
    ZUINT iSecType;                  /* section type */
    ZBOOL bPeek;                     /* peek flag */
    ZBOOL bMime;                     /* mime type */
    ZUINT iOffset;                   /* fetch offset */
    ZUINT iLength;                   /* fetch length */
    ZCHAR acPath[32];                /* part path */
    ST_ZOS_DLIST stFldLst;           /* field list */
} ST_IMAP_REQ_FETCH_SEC;

/* imap parameter of FETCH item  */
typedef struct tagIMAP_REQ_FETCH_ITEM
{
    ZUINT iFetchType;                /* fetch type */
    ST_IMAP_REQ_FETCH_SEC stSec;     /* fetch section info */
} ST_IMAP_REQ_FETCH_ITEM;

/* imap parameter of LOGIN command */
typedef struct tagIMAP_REQ_LOGIN
{
    ST_ZOS_SSTR stUserName;          /* user name */
    ST_ZOS_SSTR stPassword;          /* password */
    ST_ZOS_SSTR stMsisdn;            /* msisdn for native*/
} ST_IMAP_REQ_LOGIN;

/* imap parameter of RENAME command */
typedef struct tagIMAP_REQ_RENAME
{
    ST_ZOS_SSTR stOriName;           /* existing mailbox name */
    ST_ZOS_SSTR stNewName;           /* new mailbox name */
} ST_IMAP_REQ_RENAME;

/* imap parameter of LIST command */
typedef struct tagIMAP_REQ_LIST
{
    ST_ZOS_SSTR stRefName;           /* reference name */
    ST_ZOS_SSTR stMbxName;           /* mailbox name with possible wildcards */
} ST_IMAP_REQ_LIST;

/* imap parameter of LSUB command */
typedef struct tagIMAP_REQ_LSUB
{
    ST_ZOS_SSTR stRefName;           /* reference name */
    ST_ZOS_SSTR stMbxName;           /* mailbox name with possible wildcards */
} ST_IMAP_REQ_LSUB;

/* imap parameter of STATUS command */
typedef struct tagIMAP_REQ_STATUS
{
    ZUINT iItemType;                 /* status data item type, EN_IMAP_REQ_STAT_TYPE */
    ST_ZOS_SSTR stMbxName;           /* mailbox name */
} ST_IMAP_REQ_STATUS;

/* append message body */
typedef struct tagIMAP_REQ_APP_MSG_BDY
{
    ZFILE zFile;                     /* message file pointer */
    ZDBUF zMsgBuf;                   /* message buffer */
    ZUINT iTotalSize;                /* message total size */
    ZUINT iSentSize;                 /* message sent size */
    ZUINT iBufLeft;                  /* message buffer left size */
    ZUINT iFBufLeft;                 /* message file buffer left size */
    ZUINT iFBufOffset;               /* message file buffer offset */
    ZUINT iFileLeft;                 /* message file size without read */
    ZCHAR *pcFBuf;                   /* message file buffer */
    ST_ZOS_SSTR stData;             /* message body data */
    ST_ZOS_SSTR stFileName;          /* message body file name */
    ST_ZOS_DLIST stHdrLst;           /* message header list */
} ST_IMAP_REQ_APP_MSG_BDY;

/* append message */
typedef struct tagIMAP_REQ_APPEND_MSG
{
    ZBOOL bMultiBody;                /* muti body flag */
    ZDBUF zMsgBuf;                   /* message buffer */
    ZDBUF zMemBuf;                   /* memory buffer */
    ZUINT iTotalSize;                /* total size */
    ZUINT iSentSize;                 /* sent size */
    ZUINT iBufLeft;                  /* buffer left size */
    ST_ZOS_SSTR stBoundary;          /* boundary string */
    ST_ZOS_DLIST stHdrLst;           /* header list */
    ST_ZOS_DLIST stBdyPartLst;       /* body part list */
} ST_IMAP_REQ_APPEND_MSG;

/* imap parameter of APPEND command */
typedef struct tagIMAP_REQ_APPEND
{
    ST_ZOS_SSTR stMbxName;           /* mailbox name */
    ST_ZOS_SSTR stDate;              /* date/time string */
    ST_ZOS_SSTR stMsg;              /* message literal */
    ST_ZOS_DLIST stFlagLst;          /* flag list */
    ST_ZOS_DLIST stSegLst;           /* segment list */
    ST_IMAP_REQ_APPEND_MSG *pstAppMsg;
} ST_IMAP_REQ_APPEND;

/* imap parameter of SEARCH command */
typedef struct tagIMAP_REQ_SEARCH
{
    ZUINT iSearchType;
} ST_IMAP_REQ_SEARCH;

/* imap parameter of FETCH command */
typedef struct tagIMAP_REQ_FETCH
{
    ST_IMAP_SEQ_SET stSeqSet;        /* sequence set */
    ST_ZOS_DLIST stItemLst;          /* fetch item list, ST_IMAP_REQ_FETCH_ITEM */
} ST_IMAP_REQ_FETCH;

/* imap parameter of STORE command */
typedef struct tagIMAP_REQ_STORE
{
    ZUINT iStoreType;
    ZBOOL bSilent;
    ST_ZOS_DLIST stFlagLst;
    ST_IMAP_SEQ_SET stSeqSet;        /* sequence set */
} ST_IMAP_REQ_STORE;

/* imap parameter of COPY command */
typedef struct tagIMAP_REQ_COPY
{
    ST_ZOS_SSTR stMbxName;           /* name of target mailbox */
    ST_IMAP_SEQ_SET stSeqSet;        /* sequence set */
} ST_IMAP_REQ_COPY;

/* imap parameter of UID command */
typedef struct tagIMAP_REQ_UID
{
    ZUINT iUidType;
} ST_IMAP_REQ_UID;

/* imap parameter of SETACL command */
typedef struct tagIMAP_REQ_SET_ACL
{
    ZUINT iRightSet;                /* rights set */
    ST_ZOS_SSTR stMbxName;          /* mailbox name */
    ST_ZOS_SSTR stId;               /* identifier */
} ST_IMAP_REQ_SET_ACL;

/* imap parameter of DELETEACL command */
typedef struct tagIMAP_REQ_DEL_ACL
{
    ST_ZOS_SSTR stMbxName;          /* mailbox name */
    ST_ZOS_SSTR stId;               /* identifier */
} ST_IMAP_REQ_DEL_ACL;

/* imap parameter of LISTRIGHTS command */
typedef struct tagIMAP_REQ_LST_RIGHT
{
    ST_ZOS_SSTR stMbxName;          /* mailbox name */
    ST_ZOS_SSTR stId;               /* identifier */
} ST_IMAP_REQ_LST_RIGHT;

/* imap request message */
typedef struct tagIMAP_REQ_MSG
{
    ZUINT iCmdType;                  /* EN_IMAP_CMD_TYPE */
    ZBOOL bIsCtm;                    /* flag for custom request */
    ZDBUF zMsgBuf;                 /* message buffer id */
    ZDBUF zMemBuf;                 /* memory buffer id */
    ZCHAR acTag[IMAP_REQ_TAG_LEN];   /* tag of request command */
    union
    {
        ST_ZOS_SSTR stAuthMech;      /* authentication mechanism name */
        ST_ZOS_SSTR stSelMbxName;    /* mailbox name of SELECT command */
        ST_ZOS_SSTR stExaMbxName;    /* mailbox name of EXAMINE command */
        ST_ZOS_SSTR stCreMbxName;    /* mailbox name of CREATE command */
        ST_ZOS_SSTR stDelMbxName;    /* mailbox name of DELETE command */
        ST_ZOS_SSTR stSubMbxName;    /* mailbox name of SUBSCRIBE command */
        ST_ZOS_SSTR stUnSubMbxName;  /* mailbox name of UNSUBSCRIBE command */
        ST_ZOS_SSTR stGetAclMbx;     /* mailbox name of GETACL command */
        ST_ZOS_SSTR stMyRightMbx;    /* mailbox name of MYRIGHTS command */
        ST_ZOS_SSTR stCtmReq;        /* custom request string */
        ST_IMAP_REQ_LOGIN stLogin;   /* parameters of LOGIN command */
        ST_IMAP_REQ_RENAME stRename; /* parameters of RENAME command */
        ST_IMAP_REQ_LIST stList;     /* parameters of LIST command */
        ST_IMAP_REQ_LSUB stLsub;     /* parameters of LSUB command */
        ST_IMAP_REQ_STATUS stStatus; /* parameters of STATUS command */
        ST_IMAP_REQ_APPEND stAppend; /* parameters of APPEND command */
        ST_IMAP_REQ_SEARCH stSearch; /* parameters of SEARCH command */
        ST_IMAP_REQ_FETCH stFetch;   /* parameters of FETCH command */
        ST_IMAP_REQ_STORE stStore;   /* parameters of STORE command */
        ST_IMAP_REQ_COPY stCopy;     /* parameters of COPY command */
        ST_IMAP_REQ_UID stUid;       /* parameters of UID command */
        ST_IMAP_REQ_SET_ACL stSetAcl; /* parameters of SETACL command, rfc4314 */
        ST_IMAP_REQ_DEL_ACL stDelAcl; /* parameters of DELETEACL command, rfc4314 */
        ST_IMAP_REQ_LST_RIGHT stLstRight; /* parameters of LISTRIGHTS command, rfc4314 */
    } u;
} ST_IMAP_REQ_MSG;

/* imap message with internet message format(RFC 822) */
typedef struct tagIMAP_IMF_MSG
{
    ZDBUF zMsgBuf;                 /* message buffer id */
    ZDBUF zMemBuf;                 /* memory buffer id */
    ST_ZOS_DLIST stHdrLst;           /* header list */
    ST_ZOS_SSTR stText;             /* text data */
} ST_IMAP_IMF_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _IMAP_ABNF_TYPE_H__ */

