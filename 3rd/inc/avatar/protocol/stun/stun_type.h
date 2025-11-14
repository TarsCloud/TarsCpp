/************************************************************************

        Copyright (c) 2005-2012 by Juphoon System Software, Inc.
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
  File name     : stun_type.h
  Module        : stun
  Author        : anthouny.zhou
  Version       : 0.1
  Created on    : 2012-09-03
  Description:
    Data structure and function definitions required 
    by the stun typedefs

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _STUN_TYPE_H__
#define _STUN_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* stun error codes */
#define STUN_ERR_TIMEOUT                  -1
#define STUN_ERR_DNS_QUERY                -2
#define STUN_ERR_NO_MEM                   -3
#define STUN_ERR_SEND_REQ                 -4
#define STUN_ERR_TIMER                    -5
#define STUN_ERR_PROC_MSG                 -6
#define STUN_ERR_FSM                      -7
#define STUN_ERR_300_TRY_ALERT            300
#define STUN_ERR_400_BAD_REQ              400
#define STUN_ERR_401_UNAUTHOR             401
#define STUN_ERR_420_UNKNOWN_ATTR         420
#define STUN_ERR_437_NO_BIND              437
#define STUN_ERR_438_STALE_NONCE          438
#define STUN_ERR_439_ACT_DST_ALREADY_SET  439
#define STUN_ERR_442_UNSUPT_TPT           442
#define STUN_ERR_443_INVAL_IP_ADDR        443
#define STUN_ERR_444_INVAL_PORT           444
#define STUN_ERR_445_OPER_FOR_TCP         445
#define STUN_ERR_446_CONN_ALREADY_EXIST   446
#define STUN_ERR_486_ALLOC_QUOTA_REACH    486
#define STUN_ERR_500_SERV_ERR             500
#define STUN_ERR_507_INSUF_CAP            507

/* stun message mask */
#define STUN_MSG_REQ_MASK 0x000F

/* stun message class */
#define STUN_MSG_CLASS_REQ 0x0000   /* request class */
#define STUN_MSG_CLASS_IND 0x0010   /* indication class */
#define STUN_MSG_CLASS_RSP 0x0100   /* success response class */
#define STUN_MSG_CLASS_ERR 0x0110   /* error response class */

/* stun message type decide */
#define STUN_IS_REQ_MSG(_type) \
    ((_type & 0x0110) == STUN_MSG_CLASS_REQ)
#define STUN_IS_INDICATION(_type) \
    ((_type & 0x0110) == STUN_MSG_CLASS_IND)
#define STUN_IS_RSP_MSG(_type) \
    ((_type & 0x0110) == STUN_MSG_CLASS_RSP)
#define STUN_IS_ERR_MSG(_type) \
    ((_type & 0x0110) == STUN_MSG_CLASS_ERR)
#define STUN_GET_REQ_TYPE(_type) \
    (_type & STUN_MSG_REQ_MASK)

/* stun message type */
typedef enum EN_STUN_MSG_TYPE
{
    /* request */
    EN_STUN_MSG_REQ_BIND       = 0x0001, /* bind request */
    EN_STUN_MSG_REQ_ALLOC      = 0x0003, /* allocate request */
    EN_STUN_MSG_REQ_REFRESH    = 0x0004, /* refresh request */
    EN_STUN_MSG_REQ_CONNECT    = 0x0005, /* connect request */
    EN_STUN_MSG_REQ_CPM        = 0x0008, /* createPermission request */
    EN_STUN_MSG_REQ_CBIND      = 0x0009, /* channelbind request */

    /* indication */
    EN_STUN_MSG_IND_CSTAT      = 0x0013, /* connect status indication */
    EN_STUN_MSG_IND_SEND       = 0x0016, /* send indication */
    EN_STUN_MSG_IND_DATA       = 0x0017, /* data indication */

    /* success response */
    EN_STUN_MSG_RSP_BIND       = 0x0101, /* bind response */
    EN_STUN_MSG_RSP_ALLOC      = 0x0103, /* allocate response */
    EN_STUN_MSG_RSP_REFRESH    = 0x0104, /* refresh response */
    EN_STUN_MSG_RSP_CONNECT    = 0x0105, /* connect response */
    EN_STUN_MSG_RSP_CPM        = 0x0108, /* createPermission response */
    EN_STUN_MSG_RSP_CBIND      = 0x0109, /* channelbind response */

    /* error response */
    EN_STUN_MSG_ERR_BIND       = 0x0111, /* bind error */
    EN_STUN_MSG_ERR_ALLOC      = 0x0113, /* allocate error */
    EN_STUN_MSG_ERR_REFRESH    = 0x0114, /* refresh error */
    EN_STUN_MSG_ERR_CONNECT    = 0x0115, /* connect error */
    EN_STUN_MSG_ERR_CPM        = 0x0118, /* createPermission error */
    EN_STUN_MSG_ERR_CBIND      = 0x0119, /* channelbind error */
} EN_STUN_MSG_TYPE;

/* stun attribute type */
typedef enum EN_STUN_ATTR_TYPE
{
    EN_STUN_ATTR_MAP_ADDR      = 0x0001, /* MAPPED-ADDRESS */
    EN_STUN_ATTR_RSP_ADDR      = 0x0002, /* RESPONSE-ADDRESS */
    EN_STUN_ATTR_CHG_REQ       = 0x0003, /* CHANGE-REQUEST */
    EN_STUN_ATTR_SRC_ADDR      = 0x0004, /* SOURCE-ADDRESS */
    EN_STUN_ATTR_CHGED_ADDR    = 0x0005, /* CHANGED-ADDRESS */
    EN_STUN_ATTR_USERNAME      = 0x0006, /* USERNAME */
    EN_STUN_ATTR_PASSWORD      = 0x0007, /* PASSWORD */
    EN_STUN_ATTR_MSG_INT       = 0x0008, /* MESSAGE-INTEGRITY */
    EN_STUN_ATTR_ERR_CODE      = 0x0009, /* ERROR-CODE */
    EN_STUN_ATTR_UNKNOWN       = 0x000A, /* UNKNOWN-ATTRIBUTES */
    EN_STUN_ATTR_REFED_FROM    = 0x000B, /* REFLECTED-FROM */
    EN_STUN_ATTR_CHNL_NUM      = 0x000C, /* CHANNEL-NUMBER */
    EN_STUN_ATTR_LIFETIME      = 0x000D, /* LIFETIME */
    EN_STUN_ATTR_BANDWIDTH     = 0x0010, /* BANDWIDTH */
    EN_STUN_ATTR_XOR_PEER_ADDR = 0x0012, /* XOR-PEER-ADDRESS */
    EN_STUN_ATTR_DATA          = 0x0013, /* DATA */
    EN_STUN_ATTR_REALM         = 0x0014, /* REALM */
    EN_STUN_ATTR_NONCE         = 0x0015, /* NONCE */
    EN_STUN_ATTR_XOR_RELAY_ADDR = 0x0016, /* XOR-RELAYED-ADDRESS */
    EN_STUN_ATTR_EVEN_PORT     = 0x0018, /* EVEN-PORT */
    EN_STUN_ATTR_REQ_TPT       = 0x0019, /* REQUESTED-TRANSPORT */
    EN_STUN_ATTR_DONT_FRAG     = 0x001A, /* DONT-FRAGMENT */
    EN_STUN_ATTR_XOR_MAP_ADDR  = 0x0020, /* XOR-MAPPED-ADDRESS */
    EN_STUN_ATTR_TMR_VAL       = 0x0021, /* TIMER-VAL */
    EN_STUN_ATTR_RSRV_TOKEN    = 0x0022, /* RESERVATION-TOKEN */
    EN_STUN_ATTR_CONN_STAT     = 0x0023, /* CONNECT_STAT */
    EN_STUN_ATTR_PRIORITY      = 0x0024, /* PRIORITY */
    EN_STUN_ATTR_USE_CAND      = 0x0025, /* USE-CANDIDATE */
    EN_STUN_ATTR_PADDING       = 0x0026, /* PADDING */
    EN_STUN_ATTR_RSP_PORT      = 0x0027, /* RESPONSE-PORT */

    EN_STUN_ATTR_SOFTWARE      = 0x8022, /* SOFTWARE */
    EN_STUN_ATTR_ALERT_SRV     = 0x8023, /* ALTERNATE-SERVER */
    EN_STUN_ATTR_CACHE_TO      = 0x8027, /* CACHE-TIMEOUT */
    EN_STUN_ATTR_FINGER_PRINT  = 0x8028, /* FINGERPRINT */
    EN_STUN_ATTR_ICE_CTRLED    = 0x8029, /* ICE-CONTROLLED */
    EN_STUN_ATTR_ICE_CTRLING   = 0x802A, /* ICE-CONTROLLING */
    EN_STUN_ATTR_RSP_ORIGIN    = 0x802B, /* RESPONSE-ORIGIN */
    EN_STUN_ATTR_OTHER_ADDR    = 0x802C, /* OTHER-ADDRESS */
} EN_STUN_ATTR_TYPE;

/* stun REQUESTED-TRANSPORT type */
typedef enum EN_STUN_TPT_TYPE
{
    EN_STUN_TPT_TCP            = 0x06,   /* tcp protocol */
    EN_STUN_TPT_UDP            = 0x11,   /* udp protocol */
} EN_STUN_TPT_TYPE;

/* stun CONNECT_STAT type */
typedef enum EN_STUN_CONN_STAT_TYPE
{
    EN_STUN_CONN_STAT_LISTEN = 0,    /* LISTEN */
    EN_STUN_CONN_STAT_ESTABLISHED = 1, /* ESTABLISHED */
    EN_STUN_CONN_STAT_CLOSED = 2     /* CLOSED */
} EN_STUN_CONN_STAT_TYPE;

/* stun event type */
typedef enum EN_STUN_EVNT_TYPE
{       
    EN_STUN_EVNT_USR_REQ,          /* user request */
    EN_STUN_EVNT_USR_STOP,         /* user stop retransmission */
    EN_STUN_EVNT_TPT_DATA,         /* utpt data */
    EN_STUN_EVNT_DNS_RSP,          /* dns response */
    EN_STUN_EVNT_TPT_ERR,          /* utpt error */
    
    EN_STUN_EVNT_ALLOC_REQ,        /* allocate request */
    EN_STUN_EVNT_ALLOC_RSP,        /* allocate success response */
    EN_STUN_EVNT_ALLOC_ERR,        /* allocate error response */
    EN_STUN_EVNT_REFRESH_RSP,      /* refresh allocate success response */
    EN_STUN_EVNT_REFRESH_ERR,      /* refresh allocate error response */
    EN_STUN_EVNT_RELEASE,          /* release allocated */
    EN_STUN_EVNT_CPM_REQ,          /* create permission request */
    EN_STUN_EVNT_CPM_RSP,          /* create permission sucess response */
    EN_STUN_EVNT_CPM_ERR,          /* create permission response */
    EN_STUN_EVNT_CBIND_REQ,        /* channel bind request */
    EN_STUN_EVNT_CBIND_RSP,        /* channel bind sucess response */
    EN_STUN_EVNT_CBIND_ERR,        /* channel bind response */
    EN_STUN_EVNT_SEND_IND,         /* send indication */
    EN_STUN_EVNT_DATA_IND,         /* data indication */
    
    EN_STUN_EVNT_TMR_SEND,         /* send timeout */
    EN_STUN_EVNT_TMR_KEEP,         /* keep timeout */
    EN_STUN_EVNT_TMR_REFRESH,      /* refresh allocate */
    EN_STUN_EVNT_TMR_REBIND,       /* refresh channle bind */
    EN_STUN_EVNT_TMR_RECPM,        /* refresh create permission */
} EN_STUN_EVNT_TYPE;

/* stun event owner type */
typedef enum EN_STUN_EVNT_OWNER_TYPE
{
    EN_STUN_EOT_USER,               /* event from user */   
    EN_STUN_EOT_UTPT,               /* event from utpt */
    EN_STUN_EOT_DNS,                /* event from dns */
    EN_STUN_EOT_TMR                 /* event from timer */
} EN_STUN_EVNT_OWNER_TYPE;

/* stun timer type */
typedef enum EN_STUN_TMR_TYPE
{
    EN_STUN_TMR_SEND,              /* transaction timer for send message timerout */
    EN_STUN_TMR_KEEP,              /* transaction timer for keep tptopen timerout */       
    EN_STUN_TMR_REFRESH,           /* relay timer for refresh allocate */
    EN_STUN_TMR_REBIND,            /* relay timer for refresh channel bind */
    EN_STUN_TMR_BIND_EXPIRED,      /* relay timer for channel bind expired */
    EN_STUN_TMR_RECPM,             /* relay timer for refresh create permission */
    EN_STUN_TMR_PERM_EXPIRED,      /* relay timer for permission expired */
} EN_STUN_TMR_TYPE;

/* stun Transaction ID */
typedef struct tagSTUN_TRANS_ID
{
    ZUINT aiTransId[3];              /* transaction id */
} ST_STUN_TRANS_ID;

/* stun ERROR-CODE */
typedef struct tagSTUN_ERR_CODE
{
    ZUINT iCode;                     /* code */
    ST_ZOS_SSTR stReason;            /* reason phrase */
} ST_STUN_ERR_CODE;

/* stun CHANGED-ADDRESS */
typedef struct tagSTUN_CHANG_REQ
{
    ZBOOL change_ip;                 /* "change ip" flage */
    ZBOOL change_port;               /* "change port" flage */
} ST_STUN_CHANG_REQ;

/* stun attribute */
typedef struct tagSTUN_ATTR
{
    ZUSHORT wType;                   /* attribute type EN_STUN_ATTR_TYPE */
    ZUSHORT wLen;                    /* attribute value length */
    ST_ZOS_DLIST_NODE stNode;        /* list node */
    union
    {
        ST_ZOS_INET_ADDR stMapAddr;  /* mapped address */
        ST_ZOS_INET_ADDR stRspAddr;  /* response address */
        ST_STUN_CHANG_REQ stChgReq;  /* change request */
        ST_ZOS_INET_ADDR stSrcAddr;  /* source address */
        ST_ZOS_INET_ADDR stChgedAddr; /* changed address */
        ST_ZOS_SSTR stUserName;      /* user name */
        ST_ZOS_SSTR stPassword;      /* password */
        ST_ZOS_SSTR stMsgInt;        /* message integrity */
        ST_STUN_ERR_CODE stErrCode;  /* error code */
        ST_ZOS_SSTR stUnknown;       /* unknown attribute */
        ST_ZOS_INET_ADDR stRefedForm; /* reflected form */
        ZUSHORT wChnlNum;            /* channel number */
        ZUINT iLifeTime;             /* lifetime attribute */
        ZUINT iBandwidth;            /* bandwidth attribute */
        ST_ZOS_INET_ADDR stXorPeerAddr; /* xor peer address */
        ST_ZOS_SSTR stData;          /* data */
        ST_ZOS_SSTR stRealm;         /* realm */
        ST_ZOS_SSTR stNonce;         /* nonce */
        ST_ZOS_INET_ADDR stXorRelayAddr; /* xor relayed address */
        ZBOOL bEvntPort;             /* requested port property */
        ZUINT iReqTpt;               /* requested transport EN_STUN_TPT_TYPE */
        ST_ZOS_INET_ADDR stXorMapAddr; /* xor mapped address */
        ZUINT iTmrVal;               /* timer value */
        ST_ZOS_SSTR stRsrvToken;     /* reservation token */
        ZUINT iConnStat;             /* conn status EN_STUN_CONN_STAT_TYPE */
        ZUINT iPriority;             /* priority attribute */
        ST_ZOS_SSTR stPadding;      /* padding */
        ZUSHORT wRspPort;            /* response port */
        ST_ZOS_SSTR stSoftware;      /* software */
        ST_ZOS_INET_ADDR stAlertSrv; /* alternate server */
        ST_ZOS_SSTR stCacheTmout;    /* cache timeout */
        ST_ZOS_SSTR stFingerPrint;   /* finger print */
        ZULLONG qwIceCtrled;         /* ice controlled */
        ZULLONG qwIceCtrling;        /* ice controlling */
        ST_ZOS_INET_ADDR stRspOrigin; /* response origin */
        ST_ZOS_INET_ADDR stOtherAddr; /* other address */
        ST_ZOS_SSTR stOther;         /* other attribute */
    } u;
} ST_STUN_ATTR;

/* stun message */
typedef struct tagSTUN_MSG
{
    ZDBUF zMemBuf;                   /* memory buffer */
    ZDBUF zMsgBuf;                   /* message buffer */
    ZUSHORT wMsgType;                /* message type EN_STUN_MSG_TYPE */
    ZUSHORT wMsgLen;                 /* message length */
    ZUCHAR ucFingerPrint;            /* flag for fingerprint */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    ZUINT iMagicCookie;              /* magic cookie */
    ST_STUN_TRANS_ID stTransId;      /* transaction id */
    ST_ZOS_DLIST stAttrLst;          /* attribute list */
    ST_ZOS_USTR stBufStr;            /* string for decode */
} ST_STUN_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _STUN_TYPE_H__ */

