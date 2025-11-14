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
  File name     : sip_abnf_type.h
  Module        : sip protocol abnf typedef
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-05-01
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_ABNF_TYPE_H__
#define _SIP_ABNF_TYPE_H__

/**
 * @file sip_abnf_type.h
 * @brief Type and structure used by SIP decode and encode.
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************** macros or enum values definition ******************/

/** @brief Type of SIP SIP body part. */
typedef enum EN_SIP_BODY_PART
{
    EN_SIP_BODY_PART_STR,            /**< @brief General string. */
    EN_SIP_BODY_PART_SDP,            /**< @brief SDP message. */
    EN_SIP_BODY_PART_SFRAG           /**< @brief SIP frag. */
} EN_SIP_BODY_PART;

/** @brief Type of SIP headers. */
typedef enum EN_SIP_HDR
{
    /* rfc3261 headers */
    EN_SIP_HDR_ACPT = 0,             /**< @brief Accept header. */
    EN_SIP_HDR_ACPT_ENCODING,        /**< @brief Accept-Encoding header. */
    EN_SIP_HDR_ACPT_LANG,            /**< @brief Accept-Language header. */
    EN_SIP_HDR_ALERT_INFO,           /**< @brief Alert-Info header. */
    EN_SIP_HDR_ALLOW,                /**< @brief Allow header. */         
    EN_SIP_HDR_AUTHEN_INFO,          /**< @brief Authentication-Info header. *//* 5 */
    EN_SIP_HDR_AUTHOR,               /**< @brief Authorization header. */
    EN_SIP_HDR_CALL_ID,              /**< @brief Call-ID header. */     
    EN_SIP_HDR_CALL_INFO,            /**< @brief Call-Info header. */         
    EN_SIP_HDR_CONTACT,              /**< @brief Contact header. */
    EN_SIP_HDR_CONTENT_DISP,         /**< @brief Content-Disposition header. *//* 10 */
    EN_SIP_HDR_CONTENT_ENCODING,     /**< @brief Content-Encoding header. */     
    EN_SIP_HDR_CONTENT_LANG,         /**< @brief Content-Language header. */         
    EN_SIP_HDR_CONTENT_LEN,          /**< @brief Content-Length header. */
    EN_SIP_HDR_CONTENT_TYPE,         /**< @brief Content-Type header. */
    EN_SIP_HDR_CSEQ,                 /**< @brief CSeq header. */     /* 15 */
    EN_SIP_HDR_DATE,                 /**< @brief Date header. */         
    EN_SIP_HDR_ERROR_INFO,           /**< @brief Error-Info header. */
    EN_SIP_HDR_EXPIRES,              /**< @brief Expires header. */
    EN_SIP_HDR_FROM,                 /**< @brief From header. */     
    EN_SIP_HDR_IN_REPLY_TO,          /**< @brief In-Reply-To header. */         /* 20 */
    EN_SIP_HDR_MAX_FORWARDS,         /**< @brief Max-Forwards header. */
    EN_SIP_HDR_MIME_VER,             /**< @brief Min-Expires header. */
    EN_SIP_HDR_MIN_EXPIRES,          /**< @brief MIME-Version header. */     
    EN_SIP_HDR_ORGANIZATION,         /**< @brief Organization header. */         
    EN_SIP_HDR_PRIORITY,             /**< @brief Priority header. *//* 25 */
    EN_SIP_HDR_PROXY_AUTHEN,         /**< @brief Proxy-Authenticate header. */
    EN_SIP_HDR_PROXY_AUTHOR,         /**< @brief Proxy-Authorization header. */     
    EN_SIP_HDR_PROXY_REQUIRE,        /**< @brief Proxy-Require header. */         
    EN_SIP_HDR_RECORD_ROUTE,         /**< @brief Record-Route header. */
    EN_SIP_HDR_REPLY_TO,             /**< @brief Reply-To header. *//* 30 */
    EN_SIP_HDR_REQUIRE,              /**< @brief Require header. */     
    EN_SIP_HDR_RETRY_AFTER,          /**< @brief Retry-After header. */         
    EN_SIP_HDR_ROUTE,                /**< @brief Route header. */
    EN_SIP_HDR_SERVER,               /**< @brief Server header. */
    EN_SIP_HDR_SUBJECT,              /**< @brief Subject header. */     /* 35 */
    EN_SIP_HDR_SUPTED,               /**< @brief Supported header. */         
    EN_SIP_HDR_TIMESTAMP,            /**< @brief Timestamp header. */
    EN_SIP_HDR_TO,                   /**< @brief To header. */
    EN_SIP_HDR_UNSUPTED,             /**< @brief Unsupported header. */     
    EN_SIP_HDR_USER_AGENT,           /**< @brief User-Agent header. */         /* 40 */
    EN_SIP_HDR_VIA,                  /**< @brief Via header. */
    EN_SIP_HDR_WARN,                 /**< @brief Warning header. */
    EN_SIP_HDR_WWW_AUTHEN,           /**< @brief WWW-Authenticate header. */     
    /* rfc3262 headers */
    EN_SIP_HDR_RACK,                 /**< @brief RAck header. */         
    EN_SIP_HDR_RSEQ,                 /**< @brief RSeq header. *//* 45 */
    /* rfc3265 headers */
    EN_SIP_HDR_EVNT,                 /**< @brief Event header. */     
    EN_SIP_HDR_ALLOW_EVNT,           /**< @brief Allow-Events header. */         
    EN_SIP_HDR_SUBS_STA,             /**< @brief Subscription-State header. */
    /* rfc3515 headers */
    EN_SIP_HDR_REFER_TO,             /**< @brief Refer-To header. */     
    /* rfc3892 */
    EN_SIP_HDR_REFERRED_BY,          /**< @brief Referred-By header. */         /* 50 */
    /* rfc3891 */
    EN_SIP_HDR_REPLACES,             /**< @brief Replaces header. */
    /* rfc4028 */
    EN_SIP_HDR_SESS_EXPIRES,         /**< @brief Session-Expires header. */         
    EN_SIP_HDR_MIN_SE,               /**< @brief Min-SE header. */
    /* rfc3841 */
    EN_SIP_HDR_REQ_DISP,             /**< @brief Request-Disposition header. */     
    EN_SIP_HDR_ACPT_CONTACT,         /**< @brief Accept-Contact header. */         /* 55 */
    EN_SIP_HDR_REJ_CONTACT,          /**< @brief Reject-Contact header. */
    /* draft-ietf-sip-privacy */
    EN_SIP_HDR_ANONY,                /**< @brief Anonymity header. */     
    EN_SIP_HDR_RPID_PRIVACY,         /**< @brief RPID-Privacy header. */         
    EN_SIP_HDR_RMT_PARTY_ID,         /**< @brief Remote-Party-ID header. */
    /* rfc3903 headers */
    EN_SIP_HDR_SIP_ETAG,             /**< @brief SIP-ETag header. */     /* 60 */
    EN_SIP_HDR_SIP_IF_MATCH,         /**< @brief SIP-If-Match header. */         
    /* rfc3911 headers */
    EN_SIP_HDR_JOIN,                 /**< @brief Join header. */
    /* rfc3455 for 3gpp */
    EN_SIP_HDR_P_ASO_URI,            /**< @brief P-Associated-URI header. */         
    EN_SIP_HDR_P_CALLED_PTY_ID,      /**< @brief P-Called-Party-ID header. */
    EN_SIP_HDR_P_VNET_ID,            /**< @brief P-Visited-Network-ID Language header. *//* 65 */
    EN_SIP_HDR_P_ACCESS_NET_INFO,    /**< @brief P-Access-Network-Info header. */     
    EN_SIP_HDR_P_CHARG_FUNC_ADDR,    /**< @brief P-Charging-Function-Addresses header. */         
    EN_SIP_HDR_P_CHARG_VECTOR,       /**< @brief P-Charging-Vector header. */
    /* rfc3327 for 3gpp */
    EN_SIP_HDR_PATH,                 /**< @brief Path header. */     
    /* rfc3608 for 3gpp */
    EN_SIP_HDR_SERV_ROUTE,           /**< @brief Service-Route header. */         /* 70 */
    /* rfc3325 */
    EN_SIP_HDR_P_ASSERTED_ID,        /**< @brief P-Asserted-Identity header. */
    EN_SIP_HDR_P_PREFERRED_ID,       /**< @brief P-Preferred-Identity header. */     
    /* rfc3313 */
    EN_SIP_HDR_P_MEDIA_AUTHOR,       /**< @brief P-Media-Authorization header. */         
    /* rfc3323 */
    EN_SIP_HDR_PRIVACY,              /**< @brief Privacy header. */
    /* rfc3326 */
    EN_SIP_HDR_REASON,               /**< @brief Reason header. */         /* 75 */
    /* rfc3329 */
    EN_SIP_HDR_SEC_CLI,              /**< @brief Security-Client header. */
    EN_SIP_HDR_SEC_SERV,             /**< @brief Security-Server header. */     
    EN_SIP_HDR_SEC_VERIFY,           /**< @brief Security-Verify header. */         
    /* rfc3603 */
    EN_SIP_HDR_P_DCS_TRACE_PTY_ID,   /**< @brief P-DCS-Trace-Party-ID header. */
    EN_SIP_HDR_P_DCS_OSPS,           /**< @brief P-DCS-OSPS header. */     /* 80 */
    EN_SIP_HDR_P_DCS_BILL_INFO,      /**< @brief P-DCS-Billing-Info header. */         
    EN_SIP_HDR_P_DCS_LAES,           /**< @brief P-DCS-LAES header. */
    EN_SIP_HDR_P_DCS_REDIR,          /**< @brief P-DCS-Redirect header. */
    /* rfc4244 */
    EN_SIP_HDR_HI_INFO,              /**< @brief History-Info header. */         
    /* draft poc-p-headers */
    EN_SIP_HDR_P_ALERT_MODE,         /**< @brief P-Alerting-Mode header. *//* 85 */
    /* rfc4964 */
    EN_SIP_HDR_P_ANSWER_STATE,       /**< @brief P-Answer-State header. */     
    /* rfc4457 */
    EN_SIP_HDR_P_USER_DB,            /**< @brief P-User-Database header. */         
    /* rfc4488 */
    EN_SIP_HDR_REFER_SUB,            /**< @brief Refer-Sub header. */
    /* rfc2392 */
    EN_SIP_HDR_MSG_ID,               /**< @brief Message-ID header. */     
    EN_SIP_HDR_CONTENT_ID,           /**< @brief Content-ID header. */         /* 90 */
    /* rfc5373 */
    EN_SIP_HDR_ANSWER_MODE,          /**< @brief Answer-Mode header. */
    EN_SIP_HDR_PRIV_ANSWER_MODE,     /**< @brief Priv-Answer-Mode header. */     
    /* rfc6050 */
    EN_SIP_HDR_P_ASSERTED_SRV,       /**< @brief P-Asserted-Service header. */         
    EN_SIP_HDR_P_PREFERRED_SRV,      /**< @brief P-Preferred-Service header. */
    /* rfc 5009 */
    EN_SIP_HDR_P_EARLY_MEDIA,        /**< @brief P-Early-Media header */ /* 95 */
    /* rfc 5626 */
    EN_SIP_HDR_FLOW_TIMER,           /**< @brief Flow-Timer header */
    /* rfc 4538 */
    EN_SIP_HDR_TARGET_DLG,           /**< @brief Target-Dialog header */
    /* rfc 5318 */
    EN_SIP_HDR_P_REFUSED_URI_LIST,   /**< @brief P-Refused-URI-List header */
    /* OMA-TS-SIMPLE-IM-V1_0-20100322-C */
    EN_SIP_HDR_SESS_REPLACES,        /**< @brief Session-Replaces header */
    EN_SIP_HDR_CONTRIBUTION_ID,      /**< @brief Contribution-ID header */ /* 100 */
    EN_SIP_HDR_CONT_DESCRIPTION,     /**< @brief Content-Description header */
    /* OMA-TS-CPM-V1_0-20120813-C */
    EN_SIP_HDR_CONVERSATION_ID,      /**< @brief Conversation-ID header */
    EN_SIP_HDR_IRT_CONT_ID,          /**< @brief InReplyTo-Contribution-ID header */

    EN_SIP_HDR_X_ZTE_CAUSE,          /**< @brief X-ZTE-Cause header */

    EN_SIP_HDR_SERV_INFO,            /**< @brief ServInfo header */
    EN_SIP_HDR_CONF_INFO,            /**< @brief Conference-Info header */
    EN_SIP_HDR_TERMINAL_TYPE,        /**< @brief TerminalSeatType header */
    EN_SIP_HDR_STS_CALL_ID,           /**< @brief STS-CaLL-ID header(for statistics) */
    
    /* extension-header */
    EN_SIP_HDR_EXT                   /**< @brief Extension header. */     
} EN_SIP_HDR;

/** @brief Type of SIP method. */
typedef enum EN_SIP_METHOD
{
    /* rfc3261 methods */
    EN_SIP_METHOD_INVITE = 0,        /**< @brief INVITE method. */
    EN_SIP_METHOD_ACK = 1,           /**< @brief ACK method. */
    EN_SIP_METHOD_OPTIONS = 2,       /**< @brief OPTIONS method. */
    EN_SIP_METHOD_BYE = 3,           /**< @brief BYE method. */
    EN_SIP_METHOD_CANCEL = 4,        /**< @brief CANCEL method. */
    EN_SIP_METHOD_REGISTER = 5,      /**< @brief REGISTER method. */
    /* rfc3262 methods */
    EN_SIP_METHOD_PRACK = 6,         /**< @brief PRACK method. */
    /* rfc3265 methods */
    EN_SIP_METHOD_SUBS = 7,          /**< @brief SUBSCRIBE method. */
    EN_SIP_METHOD_NOTIFY = 8,        /**< @brief NOTIFY method. */
    /* rfc2976 methods */
    EN_SIP_METHOD_INFO = 9,          /**< @brief INFO method. */
    /* rfc3311 methods */
    EN_SIP_METHOD_UPDATE = 10,       /**< @brief UPDATE method. */
    /* rfc3428 methods */
    EN_SIP_METHOD_MSG = 11,          /**< @brief MESSAGE method. */
    /* rfc3515 methods */
    EN_SIP_METHOD_REFER = 12,        /**< @brief REFER method. */
    /* Packetcable QoS methods */
    EN_SIP_METHOD_COMET = 13,        /**< @brief COMET method. */
    /* rfc3903 methods */
    EN_SIP_METHOD_PUBLISH = 14,      /**< @brief PUBLISH method. */
    /* extension methods */
    EN_SIP_METHOD_EXT                /**< @brief Extension method. */
} EN_SIP_METHOD;

/** @brief Type of SIP disp-type. */
typedef enum EN_SIP_DISP_TYPE
{
    EN_SIP_DISP_TYPE_RENDER,         /**< @brief Type for "render". */
    EN_SIP_DISP_TYPE_SESS,           /**< @brief Type for "session". */
    EN_SIP_DISP_TYPE_EARLY_SESS,     /**< @brief Type for "early-session". */
    EN_SIP_DISP_TYPE_ICON,           /**< @brief Type for "icon". */
    EN_SIP_DISP_TYPE_ALERT,          /**< @brief Type for "alert". */
    EN_SIP_DISP_TYPE_EXT
} EN_SIP_DISP_TYPE;

/** @brief Type of SIP host. */
typedef enum EN_SIP_HOST
{
    EN_SIP_HOST_NAME,                /**< @brief Host in name format. */
    EN_SIP_HOST_IPV4,                /**< @brief Host in IPv4 format. */
    EN_SIP_HOST_IPV6                 /**< @brief Host in IPv6 format. */
} EN_SIP_HOST;

/** @brief Type of SIP uri-parameter. */
typedef enum EN_SIP_URI_PARM
{
    EN_SIP_URI_PARM_TPT,             /**< @brief Type for "transport". */
    EN_SIP_URI_PARM_USER,            /**< @brief Type for "user". */
    EN_SIP_URI_PARM_METHOD,          /**< @brief Type for "method". */
    EN_SIP_URI_PARM_TTL,             /**< @brief Type for "ttl". */
    EN_SIP_URI_PARM_MADDR,           /**< @brief Type for "maddr". */
    EN_SIP_URI_PARM_LR,              /**< @brief Type for "lr". */
    EN_SIP_URI_PARM_SESS,            /**< @brief Type for "session". */
    EN_SIP_URI_PARM_TARGET,          /**< @brief Type for "target". */
    EN_SIP_URI_PARM_CAUSE,           /**< @brief Type for "cause". */
    EN_SIP_URI_PARM_OB,
    EN_SIP_URI_PARM_GR,
    EN_SIP_URI_PARM_AUID,            /**< @brief Type for "auid". */
    EN_SIP_URI_PARM_ISFS,            /**< @brief Type for "isFocus". */
    EN_SIP_URI_PARM_OTHER            /**< @brief Type for otherwise. */
} EN_SIP_URI_PARM;

/** @brief Type of SIP par. */
typedef enum EN_SIP_PAR
{
    EN_SIP_PAR_ISUB,                 /**< @brief Type for "isub". */
    EN_SIP_PAR_EXT,                  /**< @brief Type for "ext". */
    EN_SIP_PAR_PHONE_CONTEXT,        /**< @brief Type for "phone-context". */
    EN_SIP_PAR_PARM                  /**< @brief Type for otherwise. */
} EN_SIP_PAR;

/** @brief Type of SIP transport-param. */
typedef enum EN_SIP_TPT_PARM
{
    EN_SIP_TPT_PARM_UDP,             /**< @brief Type for "udp". */
    EN_SIP_TPT_PARM_TCP,             /**< @brief Type for "tcp". */
    EN_SIP_TPT_PARM_SCTP,            /**< @brief Type for "sctp". */
    EN_SIP_TPT_PARM_TLS,             /**< @brief Type for "tls". */
    EN_SIP_TPT_PARM_OTHER            /**< @brief Type for otherwise. */
} EN_SIP_TPT_PARM;

/** @brief Type of SIP user-param. */
typedef enum EN_SIP_USER_PARM
{
    EN_SIP_USER_PARM_PHONE,          /**< @brief Type for "phone". */
    EN_SIP_USER_PARM_IP,             /**< @brief Type for "ip". */
    EN_SIP_USER_PARM_OTHER           /**< @brief Type for otherwise. */
} EN_SIP_USER_PARM;

/** @brief Type of SIP session param. */
typedef enum EN_SIP_SESS_PARM
{
    EN_SIP_SESS_PARM_1_1,            /**< @brief Type for "1-1". */
    EN_SIP_SESS_PARM_ADHOC,          /**< @brief Type for "adhoc". */
    EN_SIP_SESS_PARM_PREARRANGED,    /**< @brief Type for "prearranged". */
    EN_SIP_SESS_PARM_CHAT            /**< @brief Type for "chat". */
} EN_SIP_SESS_PARM;

/** @brief Type of SIP Request-URI. */
typedef enum EN_SIP_REQ_URI
{
    EN_SIP_REQ_URI_SIP,              /**< @brief SIP URI. */
    EN_SIP_REQ_URI_SIPS,             /**< @brief SIPS URI. */
    EN_SIP_REQ_URI_IM,               /**< @brief IM URI. */
    EN_SIP_REQ_URI_PRES,             /**< @brief Presence URI. */
    EN_SIP_REQ_URI_TEL,              /**< @brief Tel URI. */
    EN_SIP_REQ_URI_ABSO = 10         /**< @brief Absolute URI. */
} EN_SIP_REQ_URI;

/** @brief Type of SIP addr-spec. */
typedef enum EN_SIP_ADDR_SPEC
{
    EN_SIP_ADDR_SPEC_SIP = EN_SIP_REQ_URI_SIP, /**< @brief Type for "sip". */
    EN_SIP_ADDR_SPEC_SIPS = EN_SIP_REQ_URI_SIPS, /**< @brief Type for "sips". */
    EN_SIP_ADDR_SPEC_IM = EN_SIP_REQ_URI_IM, /**< @brief Type for "im". */
    EN_SIP_ADDR_SPEC_PRES = EN_SIP_REQ_URI_PRES, /**< @brief Type for "pres". */
    EN_SIP_ADDR_SPEC_TEL = EN_SIP_REQ_URI_TEL, /**< @brief Type for "tel". */
    EN_SIP_ADDR_SPEC_MAILTO,         /**< @brief Type for "mailto". */
    EN_SIP_ADDR_SPEC_MID,            /**< @brief Type for "mid". */
    EN_SIP_ADDR_SPEC_CID,            /**< @brief Type for "cid". */
    EN_SIP_ADDR_SPEC_ABSO = EN_SIP_REQ_URI_ABSO /**< @brief Absolute URI. */
} EN_SIP_ADDR_SPEC;

/** @brief Type of SIP m-type. */
typedef enum EN_SIP_MTYPE
{
    EN_SIP_MTYPE_ALL,                /**< @brief Type for "*". */
    EN_SIP_MTYPE_DISC_TEXT,          /**< @brief Type for "text". */
    EN_SIP_MTYPE_DISC_IMAGE,         /**< @brief Type for "image". */
    EN_SIP_MTYPE_DISC_AUDIO,         /**< @brief Type for "audio". */
    EN_SIP_MTYPE_DISC_VIDEO,         /**< @brief Type for "video". */
    EN_SIP_MTYPE_DISC_APP,           /**< @brief Type for "application". */
    EN_SIP_MTYPE_COMP_MSG,           /**< @brief Type for "message". */
    EN_SIP_MTYPE_COMP_MULTI,         /**< @brief Type for "multipart". */
    EN_SIP_MTYPE_EXT                 /**< @brief Type for otherwise. */
} EN_SIP_MTYPE;

/** @brief Type of SIP m-subtype. */
typedef enum EN_SIP_MSUBTYPE
{
    EN_SIP_MSUBTYPE_ALL,             /**< @brief Type for "*". */
    EN_SIP_MSUBTYPE_MIXED,           /**< @brief Type for "mixed". */
    EN_SIP_MSUBTYPE_SDP,             /**< @brief Type for "sdp". */
    EN_SIP_MSUBTYPE_ISUP,            /**< @brief Type for "isup". */
    EN_SIP_MSUBTYPE_QSIG,            /**< @brief Type for "qsig". */
    EN_SIP_MSUBTYPE_PLAIN,           /**< @brief Type for "plain". */        
    EN_SIP_MSUBTYPE_SIPFRAG,         /**< @brief Type for "sipfrag". */
    EN_SIP_MSUBTYPE_SMS,             /**< @brief Type for "simple-message-summary". */
    EN_SIP_MSUBTYPE_CPIM,            /**< @brief Type for "cpim". */
    EN_SIP_MSUBTYPE_REGINFO_XML,     /**< @brief Type for "reginfo+xml". */
    EN_SIP_MSUBTYPE_WATCHERINFO_XML, /**< @brief Type for "watcherinfo+xml". */
    EN_SIP_MSUBTYPE_CONFINFO_XML,    /**< @brief Type for "conference-info+xml". */
    EN_SIP_MSUBTYPE_PIDF_XML,        /**< @brief Type for "pidf+xml". */
    EN_SIP_MSUBTYPE_RPID_XML,        /**< @brief Type for "rpid+xml". */
    EN_SIP_MSUBTYPE_RLMI_XML,        /**< @brief Type for "rlmi+xml". */
    EN_SIP_MSUBTYPE_SIMPLE_FILTER_XML, /**< @brief Type for "simple-filter+xml". */
    EN_SIP_MSUBTYPE_RES_LSTS_XML,    /**< @brief Type for "resource-lists+xml". */
    EN_SIP_MSUBTYPE_POC_SETTINGS_XML, /**< @brief Type for "poc-settings+xml". */
    EN_SIP_MSUBTYPE_DLG_INFO_XML,    /**< @brief Type for "dialog-info+xml". */
    EN_SIP_MSUBTYPE_VND_3GPP_CW_XML, /**< @brief Type for "vnd.3gpp.cw+xml". */
    EN_SIP_MSUBTYPE_DTMF_RELAY,      /**< @brief Type for "dtmf-relay". */
    EN_SIP_MSUBTYPE_DTMF,            /**< @brief Type for "dtmf". */
    EN_SIP_MSUBTYPE_RELATED,         /**< @brief Type for "related". */
    EN_SIP_MSUBTYPE_GAD_XML,         /**< @brief Type for "vnd.poc.group-advertisement+xml". */
    EN_SIP_MSUBTYPE_XCAP_DIFF,       /**< @brief Type for "xcap-diff+xml". */
    EN_SIP_MSUBTYPE_EXTERNAL_BODY,   /**< @brief Type for "external-body". */
    EN_SIP_MSUBTYPE_3GPP_IMS_XML,    /**< @brief Type for "3gpp-ims+xml". */
    EN_SIP_MSUBTYPE_TIFF_FX,         /**< @brief Type for "tiff-fx" */
    EN_SIP_MSUBTYPE_SSCC,            /**< @brief Type for "sscc" */
    EN_SIP_MSUBTYPE_SMS_INFO,        /**< @brief Type for "sms-info+xml" */
    EN_SIP_MSUBTYPE_IMDN_XML,        /**< @brief Type for "imdn+xml" */
    EN_SIP_MSUBTYPE_MEDIA_CTRL_XML,  /**< @brief Type for "media_control+xml" */
    EN_SIP_MSUBTYPE_EUCREQ_XML,      /**< @brief Type for "end-user-confirmation-request+xml" */
    EN_SIP_MSUBTYPE_EUCRSP_XML,      /**< @brief Type for "end-user-confirmation-response+xml" */
    EN_SIP_MSUBTYPE_EUCACK_XML,      /**< @brief Type for "end-user-confirmation-ack+xml" */
    EN_SIP_MSUBTYPE_EUNREQ_XML,      /**< @brief Type for "end-user-notification-request+xml" */
    EN_SIP_MSUBTYPE_SYSREQ_XML,      /**< @brief Type for "system-request+xml" */
    EN_SIP_MSUBTYPE_MSGINFO_XML,     /**< @brief Type for "msginfo+xml" */
    EN_SIP_MSUBTYPE_VEM,             /**< @brief Type for "vemoticon+xml" */
    EN_SIP_MSUBTYPE_CLOUD_FILE_XML,  /**< @brief Type for "cloudfile+xml" */
    EN_SIP_MSUBTYPE_XML,             /**< @brief Type for "xml" */
    EN_SIP_MSUBTYPE_GRPLST_VER_XML,  /**< @brief Type for "grouplist-ver+xml" */
    EN_SIP_MSUBTYPE_VND_3GPP_SMS,    /**< @brief Type for "vnd.3gpp.sms" */
    EN_SIP_MSUBTYPE_GRP_JOIN,        /**< @brief Type for "group-join" */
    EN_SIP_MSUBTYPE_GRP_JOIN_NTFY,   /**< @brief Type for "group-join-notify" */
    EN_SIP_MSUBTYPE_VND_OMA_PUSH,    /**< @brief Type for "vnd.oma.push" */  
    EN_SIP_MSUBTYPE_RELAY_SERVER_XML,  /**< @brief Type for "relayserver+xml" */
    EN_SIP_MSUBTYPE_UD_SIGNAL,         /**< @brief Type for "ud-signal" */
    EN_SIP_MSUBTYPE_MEMB_CTL_XML,      /**< @brief Type for "memb-ctl+xml" */
    EN_SIP_MSUBTYPE_MEMB_STATE_XML,    /**< @brief Type for "memb-state+xml" */
    EN_SIP_MSUBTYPE_PTT_XML,           /**< @brief Type for "ptt+xml" */
    EN_SIP_MSUBTYPE_PTT_ACK_XML,       /**< @brief Type for "ptt-ack+xml" */
    EN_SIP_MSUBTYPE_LINK_STATE_XML,    /**< @brief Type for "link-state+xml" */
    EN_SIP_MSUBTYPE_PARA_INQ_XML,      /**< @brief Type for "para-inq+xml" */
    EN_SIP_MSUBTYPE_PARA_INQ_ACK_XML,  /**< @brief Type for "para-inq-ack+xml" */
    EN_SIP_MSUBTYPE_PARA_SET_XML,      /**< @brief Type for "para-set+xml" */
    EN_SIP_MSUBTYPE_PARA_SET_ACK_XML,  /**< @brief Type for "para-set-ack+xml" */
    EN_SIP_MSUBTYPE_VOICE_SOURCE_XML,  /**< @brief Type for "voice-source+xml" */
    EN_SIP_MSUBTYPE_GRPDATA_XML,       /**< @brief Type for "vnd.oma.cpm-groupdata+xml". */
    EN_SIP_MSUBTYPE_REVOKE_XML,        /**< @brief Type for "vnd.gsma.rcsrevoke+xml" */
    EN_SIP_MSUBTYPE_USER_QUERY_XML,    /**< @brief Type for "user-query+xml". */
    EN_SIP_MSUBTYPE_RCSALIAS_XML,      /**< @brief Type for "vnd.gsma.rcsalias-mgmt+xml" */
    EN_SIP_MSUBTYPE_SPAMBOT_XML,       /**< @brief Type for "vnd.gsma.rcsspam-report+xml." */
    EN_SIP_MSUBTYPE_PRE_CALL_XML,      /**< @brief Type for "vnd.gsma.encall+xml." */
    EN_SIP_MSUBTYPE_EXT                /**< @brief Type for otherwise. */
} EN_SIP_MSUBTYPE;

/** @brief Type of SIP gen-value. */
typedef enum EN_SIP_GEN_VAL
{
    EN_SIP_GEN_VAL_VAL,              /**< @brief Type for value. */
    EN_SIP_GEN_VAL_HOST,             /**< @brief Type for host. */
    EN_SIP_GEN_VAL_QSTR              /**< @brief Type for qouted string. */
} EN_SIP_GEN_VAL;

/** @brief Type of SIP contact-params. */
typedef enum EN_SIP_CONTACT_PARMS
{
    EN_SIP_CONTACT_PARMS_QVAL,       /**< @brief Type for "q". */
    EN_SIP_CONTACT_PARMS_EXPIRES,    /**< @brief Type for "expires". */
    EN_SIP_CONTACT_PARMS_ISFOCUS,    /**< @brief Type for "isfocus". */
    EN_SIP_CONTACT_PARMS_G_POC_TALKBURST, /**< @brief Type for "+g.poc.talkburst". */
    EN_SIP_CONTACT_PARMS_G_POC_GROUPAD, /**< @brief Type for "+g.poc.groupad". */
    EN_SIP_CONTACT_PARMS_G_3GPP_CS_VOICE, /**< @brief Type for "+g.3gpp.cs-voice". */
    EN_SIP_CONTACT_PARMS_G_3GPP_APP_REF,  /**< @brief Type for "+g.3gpp.app_ref". */
    EN_SIP_CONTACT_PARMS_G_OMA_SIP_IM, /**< @brief Type for "+g.oma.sip-im". */
    EN_SIP_CONTACT_PARMS_G_OMA_SIP_IM_L_MSG, /**< @brief Type for "+g.oma.sip-im.large-message". */
    EN_SIP_CONTACT_PARMS_G_OMA_CPM_MSG, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.msg". */
    EN_SIP_CONTACT_PARMS_G_OMA_CPM_L_MSG, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.largemsg". */
    EN_SIP_CONTACT_PARMS_G_OMA_CPM_DEFER, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.deferred". */
    EN_SIP_CONTACT_PARMS_G_OMA_CPM_SESS, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.session". */
    EN_SIP_CONTACT_PARMS_C_P_REG,    /**< @brief Type "reg-id" */
    EN_SIP_CONTACT_PARMS_C_P_INSTANCE, /**< @brief Type "+sip.instance" */
    EN_SIP_CONTACT_PARMS_TEMP_GRUU,  /**< @brief Type "temp-gruu" */
    EN_SIP_CONTACT_PARMS_PUB_GRUU,   /**< @brief Type "pub-gruu" */
    EN_SIP_CONTACT_PARMS_CONF_EXIST,   /**< @brief Type "Conf-Exist" */
    EN_SIP_CONTACT_PARMS_G_3GPP_SMS_IP, /**< @brief Type for "+g.3gpp.smsip". */
    EN_SIP_CONTACT_PARMS_ISZTEFOCUS,    /**< @brief Type for "isZTEfocus". */
    EN_SIP_CONTACT_PARMS_EXT,        /**< @brief Type for otherwise. */
    EN_SIP_CONTACT_PARMS_CFEATURE    /**< @brief Type feature-param rfc3840. */
    
} EN_SIP_CONTACT_PARMS;

/** @brief Type of SIP ServInfo-params. */
typedef enum EN_SIP_SERVINFO_PARMS
{
    EN_SIP_SERVINFO_PARMS_SERV_TYPE,        /**< @brief Type for "ST". */
    EN_SIP_SERVINFO_PARMS_SUB_SERV_TYPE,    /**< @brief Type for "SubST". */
    EN_SIP_SERVINFO_PARMS_EXT               /**< @brief Type for otherwise. */
}EN_SIP_SERVINFO_PARMS;

/** @brief Type of SIP ServInfo ST param. */
typedef enum EN_SIP_ST_TYPE
{
    EN_SIP_ST_P2P,                  /**< @brief Type for "P2P". */    
    EN_SIP_ST_EXCOM,                /**< @brief Type for "Excom". */
    EN_SIP_ST_OTHER                 /**< @brief Type for "Other". */
} EN_SIP_ST_TYPE;

/** @brief Type of SIP ServInfo Sub-ST param. */
typedef enum EN_SIP_SUB_ST_TYPE
{
    EN_SIP_SUB_ST_SFCONF,           /**< @brief Type for "Sfconf". */   
    EN_SIP_SUB_ST_SHCONF,           /**< @brief Type for "Shconf". */
    EN_SIP_SUB_ST_PFCONF,           /**< @brief Type for "Pfconf". */
    EN_SIP_SUB_ST_PHCONF,           /**< @brief Type for "Phconf". */
    EN_SIP_SUB_ST_ALARM,            /**< @brief Type for "Alarm". */
    EN_SIP_SUB_ST_BROADCAST,        /**< @brief Type for "Broadcast". */
    EN_SIP_SUB_ST_MULTICAST,        /**< @brief Type for "Multicast". */
    EN_SIP_SUB_ST_COMMAND,          /**< @brief Type for "Command". */ 
    EN_SIP_SUB_ST_OTHER             /**< @brief Type for "Other". */
} EN_SIP_SUB_ST_TYPE;

/** @brief Type of SIP algorithm-value. */
typedef enum EN_SIP_ALGO_VAL
{
    EN_SIP_ALGO_VAL_MD5,             /**< @brief Type for "MD5". */
    EN_SIP_ALGO_VAL_MD5_SESS,        /**< @brief Type for "MD5-sess". */
    EN_SIP_ALGO_VAL_OTHER            /**< @brief Type for otherwise. */
} EN_SIP_ALGO_VAL;

/** @brief Type of SIP dig-resp. */
typedef enum EN_SIP_DIG_RSP
{
    EN_SIP_DIG_RSP_USERNAME,         /**< @brief Type for "username". */
    EN_SIP_DIG_RSP_REALM,            /**< @brief Type for "realm". */
    EN_SIP_DIG_RSP_NONCE,            /**< @brief Type for "nonce". */
    EN_SIP_DIG_RSP_DIGEST_URI,       /**< @brief Type for "uri". */
    EN_SIP_DIG_RSP_DRESP,            /**< @brief Type for "response". */
    EN_SIP_DIG_RSP_ALGO,             /**< @brief Type for "algorithm". */
    EN_SIP_DIG_RSP_CNONCE,           /**< @brief Type for "cnonce". */
    EN_SIP_DIG_RSP_OPAQUE,           /**< @brief Type for "opaque". */
    EN_SIP_DIG_RSP_MSG_QOP,          /**< @brief Type for "qop". */
    EN_SIP_DIG_RSP_NONCE_COUNT,      /**< @brief Type for "nc". */
    EN_SIP_DIG_RSP_AUTS,             /**< @brief Type for "auts". */
    EN_SIP_DIG_RSP_AUTH_PARM         /**< @brief Type for otherwise. */
} EN_SIP_DIG_RSP;

/** @brief Type of SIP ainfo. */
typedef enum EN_SIP_AINFO
{
    EN_SIP_AINFO_NEXTNONCE,          /**< @brief Type for "nextnonce". */
    EN_SIP_AINFO_MSG_QOP,            /**< @brief Type for "qop". */
    EN_SIP_AINFO_RSP_AUTH,           /**< @brief Type for "rspauth". */
    EN_SIP_AINFO_CNONCE,             /**< @brief Type for "cnonce". */
    EN_SIP_AINFO_NONCE_COUNT         /**< @brief Type for "nc". */
} EN_SIP_AINFO;

/** @brief Type of SIP purpose. */
typedef enum EN_SIP_PURPS
{
    EN_SIP_PURPS_ICON,               /**< @brief Type for "icon". */
    EN_SIP_PURPS_INFO,               /**< @brief Type for "info". */
    EN_SIP_PURPS_CARD,               /**< @brief Type for "card". */
    EN_SIP_PURPS_OTHER               /**< @brief Type for otherwise. */
} EN_SIP_PURPS;

/** @brief Type of SIP wkday. */
typedef enum EN_SIP_WKDAY
{
    EN_SIP_WKDAY_SUN = 0,            /**< @brief Type for "Sun". */
    EN_SIP_WKDAY_MON,                /**< @brief Type for "Mon". */
    EN_SIP_WKDAY_TUE,                /**< @brief Type for "Tue". */
    EN_SIP_WKDAY_WEB,                /**< @brief Type for "Wed". */
    EN_SIP_WKDAY_THU,                /**< @brief Type for "Thu". */
    EN_SIP_WKDAY_FRI,                /**< @brief Type for "Fri". */
    EN_SIP_WKDAY_SAT,                /**< @brief Type for "Sat". */
    EN_SIP_WKDAY_OTHER               /**< @brief Type for otherwise. */
} EN_SIP_WKDAY;

/** @brief Type of SIP month. */
typedef enum EN_SIP_MONTH
{
    EN_SIP_MONTH_JAN,                /**< @brief Type for "Jan". */
    EN_SIP_MONTH_FEB,                /**< @brief Type for "Feb". */
    EN_SIP_MONTH_MAR,                /**< @brief Type for "Mar". */
    EN_SIP_MONTH_APR,                /**< @brief Type for "Apr". */
    EN_SIP_MONTH_MAY,                /**< @brief Type for "May". */
    EN_SIP_MONTH_JUN,                /**< @brief Type for "Jun". */
    EN_SIP_MONTH_JUL,                /**< @brief Type for "Jul". */
    EN_SIP_MONTH_AUG,                /**< @brief Type for "Aug". */
    EN_SIP_MONTH_SEP,                /**< @brief Type for "Sep". */
    EN_SIP_MONTH_OCT,                /**< @brief Type for "Oct". */
    EN_SIP_MONTH_NOV,                /**< @brief Type for "Nov". */
    EN_SIP_MONTH_DEC,                /**< @brief Type for "Dec". */
    EN_SIP_MONTH_OTHER               /**< @brief Type for otherwise. */
} EN_SIP_MONTH;

/** @brief Type of SIP handling. */
typedef enum EN_SIP_HANDLING
{
    EN_SIP_HANDLING_OPT,             /**< @brief Type for "optional". */
    EN_SIP_HANDLING_REQ,             /**< @brief Type for "required". */
    EN_SIP_HANDLING_OTHER            /**< @brief Type for otherwise. */
} EN_SIP_HANDLING;

/** @brief Type of SIP qop-value. */
typedef enum EN_SIP_QOP_VAL
{
    EN_SIP_QOP_VAL_AUTH,             /**< @brief Type for "auth". */
    EN_SIP_QOP_VAL_AUTH_INT,         /**< @brief Type for "auth-int". */
    EN_SIP_QOP_VAL_OTHER             /**< @brief Type for otherwise. */
} EN_SIP_QOP_VAL;

/** @brief Type of SIP digest-cln. */
typedef enum EN_SIP_DIGEST_CLN
{
    EN_SIP_DIGEST_CLN_REALM,         /**< @brief Type for "realm". */
    EN_SIP_DIGEST_CLN_DOMAIN,        /**< @brief Type for "domain". */
    EN_SIP_DIGEST_CLN_NONCE,         /**< @brief Type for "nonce". */
    EN_SIP_DIGEST_CLN_OPAQUE,        /**< @brief Type for "opaque". */
    EN_SIP_DIGEST_CLN_STALE,         /**< @brief Type for "stale". */
    EN_SIP_DIGEST_CLN_ALGO,          /**< @brief Type for "algorithm". */
    EN_SIP_DIGEST_CLN_QOPTS,         /**< @brief Type for "qop". */
    EN_SIP_DIGEST_CLN_AUTH_PARM      /**< @brief Type for otherwise. */
} EN_SIP_DIGEST_CLN;

/** @brief Type of SIP option-tag. */
typedef enum EN_SIP_OPT_TAG
{
    EN_SIP_OPT_TAG_100REL,           /**< @brief Type for "100rel" in rfc3262. */
    EN_SIP_OPT_TAG_EARLY_SESS,       /**< @brief Type for "early-session" in rfc3959. */
    EN_SIP_OPT_TAG_EVNTLST,          /**< @brief Type for "eventlist" in rfc4662. */
    EN_SIP_OPT_TAG_FROM_CHANGE,      /**< @brief Type for "from-change" in rfc4916. */
    EN_SIP_OPT_TAG_HISTINFO,         /**< @brief Type for "histinfo" in rfc4244. */
    EN_SIP_OPT_TAG_JOIN,             /**< @brief Type for "join" in rfc3911. */
    EN_SIP_OPT_TAG_NOREFERSUB,       /**< @brief Type for "norefersub" in rfc4488. */
    EN_SIP_OPT_TAG_PATH,             /**< @brief Type for "path" in rfc3327. */
    EN_SIP_OPT_TAG_PRECONDITION,     /**< @brief Type for "precondition" in rfc3312. */
    EN_SIP_OPT_TAG_PREF,             /**< @brief Type for "pref" in rfc3840. */
    EN_SIP_OPT_TAG_PRIVACY,          /**< @brief Type for "privacy" in rfc3323. */
    EN_SIP_OPT_TAG_REPLACES,         /**< @brief Type for "replaces" in rfc3981. */
    EN_SIP_OPT_TAG_RES_PRIORITY,     /**< @brief Type for "resource-priority" in rfc4412. */
    EN_SIP_OPT_TAG_SDP_ANAT,         /**< @brief Type for "sdp-anat" in rfc4092. */
    EN_SIP_OPT_TAG_SEC_AGREE,        /**< @brief Type for "sec-agree" in rfc3329. */
    EN_SIP_OPT_TAG_TDLG,             /**< @brief Type for "tdialog" in rfc4538. */
    EN_SIP_OPT_TAG_TMR,              /**< @brief Type for "timer" in rfc4028. */
    EN_SIP_OPT_TAG_MULTI_REFER,      /**< @brief Type for "multiple-refer" in rfc5368. */
    EN_SIP_OPT_TAG_ANSWERMODE,       /**< @brief Type for "answermode" in rfc5373. */
    EN_SIP_OPT_TAG_GRUU,             /**< @brief Type for "gruu" in rfc5627 */  
    EN_SIP_OPT_TAG_ICE,              /**< @brief Type for "ice" in rfc5768 */
    EN_SIP_OPT_TAG_OUTBOUND,         /**< @brief Type for "outbound" in rfc5626 */
    EN_SIP_OPT_TAG_MSG,              /**< @brief Type for "recipient-list-message" in rfc5365 */
    EN_SIP_OPT_TAG_RCP_LST_IVT,      /**< @brief Type for "recipient-list-invite" in rfc5366 */
    EN_SIP_OPT_TAG_SUBSCRIBE,        /**< @brief Type for "recipient-list-subscribe" in rfc5367 */
    EN_SIP_OPT_TAG_OTHER             /**< @brief Type for otherwise. */
} EN_SIP_OPT_TAG;

/** @brief Type of SIP stale. */
typedef enum EN_SIP_STALE
{
    EN_SIP_STALE_TRUE,               /**< @brief Type for "true". */
    EN_SIP_STALE_FALSE               /**< @brief Type for "false". */
} EN_SIP_STALE;

/** @brief Type of SIP transport. */
typedef enum EN_SIP_TPT
{
    EN_SIP_TPT_UDP,                  /**< @brief Type for "UDP". */
    EN_SIP_TPT_TCP,                  /**< @brief Type for "TCP". */
    EN_SIP_TPT_TLS,                  /**< @brief Type for "TLS". */ 
    EN_SIP_TPT_SCTP,                 /**< @brief Type for "SCTP". */
    EN_SIP_TPT_OTHER                 /**< @brief Type for otherwise. */
} EN_SIP_TPT;

/** @brief Type of SIP via-params. */
typedef enum EN_SIP_VIA_PARMS
{
    EN_SIP_VIA_PARMS_TTL,            /**< @brief Type for "ttl". */ 
    EN_SIP_VIA_PARMS_MADDR,          /**< @brief Type for "maddr". */
    EN_SIP_VIA_PARMS_RECV,           /**< @brief Type for "received". */
    EN_SIP_VIA_PARMS_BRANCH,         /**< @brief Type for "branch". */
    EN_SIP_VIA_PARMS_RPORT,          /**< @brief Type for "rport". */
    EN_SIP_VIA_PARMS_ALIAS,          /**< @brief Type for "alias". rfc5923 */
    EN_SIP_VIA_PARMS_EXT             /**< @brief Type for otherwise. */
} EN_SIP_VIA_PARMS;

/** @brief Type of SIP event-package. */
typedef enum EN_SIP_EVNT_PKG
{
    EN_SIP_EVNT_PKG_PRESENCE,        /**< @brief Type for "presence" in rfc3856. */ 
    EN_SIP_EVNT_PKG_REFER,           /**< @brief Type for "refer" in rfc3515. */
    EN_SIP_EVNT_PKG_POC_SET,         /**< @brief Type for "poc-settings" in rfc4354. */
    EN_SIP_EVNT_PKG_CONF,            /**< @brief Type for "conference" in rfc4575. */
    EN_SIP_EVNT_PKG_DLG,             /**< @brief Type for "dialog" in rfc4235. */
    EN_SIP_EVNT_PKG_KPML,            /**< @brief Type for "kpml" in rfc4730. */
    EN_SIP_EVNT_PKG_MSG_SUMMARY,     /**< @brief Type for "message-summary" in rfc3842. */
    EN_SIP_EVNT_PKG_REG,             /**< @brief Type for "reg" in rfc3680. */
    EN_SIP_EVNT_PKG_UA_PROF,         /**< @brief Type for "ua-profile" in draft-ietf-SIP-xcap-config. */
    EN_SIP_EVNT_PKG_XCAP_DIFF,       /**< @brief Type for "xcap-diff" in rfc5875. */
    EN_SIP_EVNT_PKG_DMSG,            /**< @brief Type for "deferred-messages" in OMA-TS-CPM_Conv_Fnct-V1_0-20101012-C. */
    EN_SIP_EVNT_PKG_GRP_LST,         /**< @brief Type for "grouplist". */
    EN_SIP_EVNT_PKG_GRP_INFO,        /**< @brief Type for "groupinfo". */
    EN_SIP_EVNT_PKG_USER_QUERY,      /**< @brief Type for "user-query". */
    EN_SIP_EVNT_PKG_WG67_KEY_IN,     /**< @brief Type for "WG67KEY-IN" in ED137 radio. */ 
    EN_SIP_EVNT_PKG_OTHER            /**< @brief Type for otherwise. */
} EN_SIP_EVNT_PKG;

/** @brief Type of SIP event-template. */
typedef enum EN_SIP_EVNT_TEMP
{
    EN_SIP_EVNT_TEMP_WINFO,          /**< @brief Type for "winfo". */
    EN_SIP_EVNT_TEMP_OTHER           /**< @brief Type for otherwise. */
} EN_SIP_EVNT_TEMP;

/** @brief Type of SIP event-param. */
typedef enum EN_SIP_EVNT_PARM
{
    EN_SIP_EVNT_PARM_ID,             /**< @brief Type for "id". */
    EN_SIP_EVNT_PARM_PROF_TYPE,      /**< @brief Type for "profile-type". */
    EN_SIP_EVNT_PARM_VENDOR,         /**< @brief Type for "vendor". */
    EN_SIP_EVNT_PARM_MODEL,          /**< @brief Type for "model". */
    EN_SIP_EVNT_PARM_VER,            /**< @brief Type for "version". */
    EN_SIP_EVNT_PARM_EFFECT_BY,      /**< @brief Type for "effective-by". */
    EN_SIP_EVNT_PARM_DEV_ID,         /**< @brief Type for "device-id". */
    EN_SIP_EVNT_PARM_NET_USER,       /**< @brief Type for "network-user". */
    EN_SIP_EVNT_PARM_DOC,            /**< @brief Type for "document". */
    EN_SIP_EVNT_PARM_AUID,           /**< @brief Type for "auid". */
    EN_SIP_EVNT_PARM_DIFF_PROC,      /**< @brief Type for "diff-processing". */
    EN_SIP_EVNT_PARM_GEN             /**< @brief Type for otherwise. */
} EN_SIP_EVNT_PARM;

/** @brief Type of SIP profile-value. */
typedef enum EN_SIP_PROF_VAL
{
    EN_SIP_PROF_VAL_DEV,             /**< @brief Type for "device". */
    EN_SIP_PROF_VAL_USER,            /**< @brief Type for "user". */
    EN_SIP_PROF_VAL_LOCAL_NET,       /**< @brief Type for "local-network". */
    EN_SIP_PROF_VAL_APP,             /**< @brief Type for "application". */       
    EN_SIP_PROF_VAL_OTHER            /**< @brief Type for otherwise. */
} EN_SIP_PROF_VAL;

/** @brief Type of SIP diff-processing. */
typedef enum EN_SIP_DIFF_PROC
{
    EN_SIP_DIFF_PROC_NO_PATCH,       /**< @brief Type for "no-patching". */
    EN_SIP_DIFF_PROC_XCAP_PATCH,     /**< @brief Type for "xcap-patching". */
    EN_SIP_DIFF_PROC_AGGR,           /**< @brief Type for "aggregate". */
    EN_SIP_DIFF_PROC_OTHER           /**< @brief Type for otherwise. */
} EN_SIP_DIFF_PROC;

/** @brief Type of SIP substate-value. */
typedef enum EN_SIP_SUBSTA_VAL
{
    EN_SIP_SUBSTA_VAL_ACTIVE,        /**< @brief Type for "active". */ 
    EN_SIP_SUBSTA_VAL_PENDING,       /**< @brief Type for "pending". */
    EN_SIP_SUBSTA_VAL_TERM,          /**< @brief Type for "terminated". */
    EN_SIP_SUBSTA_VAL_EXT            /**< @brief Type for otherwise. */
} EN_SIP_SUBSTA_VAL;

/** @brief Type of SIP priority-value. */
typedef enum EN_SIP_PRIORITY_VAL
{
    EN_SIP_PRIORITY_VAL_EMERG,       /**< @brief Type for "emergency". */
    EN_SIP_PRIORITY_VAL_URGENT,      /**< @brief Type for "urgent". */
    EN_SIP_PRIORITY_VAL_NORMAL,      /**< @brief Type for "normal". */
    EN_SIP_PRIORITY_VAL_NON_URGENT,  /**< @brief Type for "non-urgent". */
    EN_SIP_PRIORITY_VAL_OTHER        /**< @brief Type for otherwise. */
} EN_SIP_PRIORITY_VAL;

/** @brief Type of SIP event-reason-value. */
typedef enum EN_SIP_EVNT_REASON_VAL
{
    EN_SIP_EVNT_REASON_DEACT,        /**< @brief Type for "deactivated". */
    EN_SIP_EVNT_REASON_PROBATION,    /**< @brief Type for "probation". */
    EN_SIP_EVNT_REASON_REJ,          /**< @brief Type for "rejected". */
    EN_SIP_EVNT_REASON_TIMEOUT,      /**< @brief Type for "timeout". */
    EN_SIP_EVNT_REASON_GIVE_UP,      /**< @brief Type for "giveup". */
    EN_SIP_EVNT_REASON_NO_RES,       /**< @brief Type for "noresource". */
    EN_SIP_EVNT_REASON_EXT           /**< @brief Type for otherwise. */
} EN_SIP_EVNT_REASON_VAL;

/** @brief Type of SIP subexp-params. */
typedef enum EN_SIP_SUBEXP_PARMS
{
    EN_SIP_SUBEXP_PARMS_REASON,      /**< @brief Type for "reason". */
    EN_SIP_SUBEXP_PARMS_EXPIRE,      /**< @brief Type for "expires". */
    EN_SIP_SUBEXP_PARMS_RETRY_AFTER, /**< @brief Type for "retry-after". */
    EN_SIP_SUBEXP_PARMS_GEN          /**< @brief Type for otherwise. */
} EN_SIP_SUBEXP_PARMS;

/** @brief Type of SIP replaces-param. */
typedef enum EN_SIP_REPLACES_PARM
{
    EN_SIP_REPLACES_PARM_TO_TAG,     /**< @brief Type for "to-tag". */
    EN_SIP_REPLACES_PARM_FROM_TAG,   /**< @brief Type for "from-tag". */
    EN_SIP_REPLACES_PARM_EARLY_TAG,  /**< @brief Type for "early-only". */
    EN_SIP_REPLACES_PARM_GEN         /**< @brief Type for otherwise. */
} EN_SIP_REPLACES_PARM;

/** @brief Type of SIP directive. */
typedef enum EN_SIP_DIRECT
{
    EN_SIP_DIRECT_PROXY,             /**< @brief Type for "proxy". */
    EN_SIP_DIRECT_PROXY_REDIRECT,    /**< @brief Type for "redirect". */
    EN_SIP_DIRECT_CANCEL,            /**< @brief Type for "cancel". */
    EN_SIP_DIRECT_CANCEL_NO,         /**< @brief Type for "no-cancel". */
    EN_SIP_DIRECT_FORK,              /**< @brief Type for "fork". */
    EN_SIP_DIRECT_FORK_NO,           /**< @brief Type for "no-fork". */
    EN_SIP_DIRECT_RECURSE,           /**< @brief Type for "recurse". */
    EN_SIP_DIRECT_RECURSE_NO,        /**< @brief Type for "no-recurse". */
    EN_SIP_DIRECT_PARALLEL,          /**< @brief Type for "parallel". */
    EN_SIP_DIRECT_PARALLEL_SEQ,      /**< @brief Type for "sequential". */
    EN_SIP_DIRECT_QUEUE,             /**< @brief Type for "queue". */
    EN_SIP_DIRECT_QUEUE_NO           /**< @brief Type for "no-queue". */
} EN_SIP_DIRECT;

/** @brief Type of SIP feature-value. */
typedef enum EN_SIP_FEATURE_VAL
{
    EN_SIP_FEATURE_VAL_VOICE,        /**< @brief Type for "voice-mail". */
    EN_SIP_FEATURE_VAL_ATTEND,       /**< @brief Type for "attendant". */
    EN_SIP_FEATURE_VAL_OTHER         /**< @brief Type for otherwise. */
} EN_SIP_FEATURE_VAL;

/** @brief Type of SIP ac-params. */
typedef enum EN_SIP_AC_PARMS
{
    EN_SIP_AC_PARMS_FEATURE,         /**< @brief Type for "feature". */
    EN_SIP_AC_PARMS_REQ,             /**< @brief Type for "require". */
    EN_SIP_AC_PARMS_EXPLICIT,        /**< @brief Type for "explicit". */
    EN_SIP_AC_PARMS_G_POC_TALKBURST, /**< @brief Type for "+g.poc.talkburst". */
    EN_SIP_AC_PARMS_G_POC_GROUPAD,   /**< @brief Type for "+g.poc.groupad". */
    EN_SIP_AC_PARMS_G_3GPP_CS_VOICE, /**< @brief Type for "+g.3gpp.cs-voice". */
    EN_SIP_AC_PARMS_G_3GPP_APP_REF,  /**< @brief Type for "+g.3gpp.app_ref". */
    EN_SIP_AC_PARMS_G_OMA_SIP_IM,    /**< @brief Type for "+g.oma.sip-im". */
    EN_SIP_AC_PARMS_G_OMA_SIP_IM_L_MSG, /**< @brief Type for "+g.oma.sip-im.large-message". */
    EN_SIP_AC_PARMS_G_OMA_SIP_IM_S_MSG, /**< @brief Type for "+g.oma.sip-im.system-message". */
    EN_SIP_AC_PARMS_SMS,             /**< @brief Type for "sms". */
    EN_SIP_AC_PARMS_G_OMA_CPM_MSG,   /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.msg". */
    EN_SIP_AC_PARMS_G_OMA_CPM_L_MSG, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.largemsg". */
    EN_SIP_AC_PARMS_G_OMA_CPM_DEFER, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.deferred". */
    EN_SIP_AC_PARMS_G_OMA_CPM_SESS, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.session". */
    EN_SIP_AC_PARMS_G_OMA_CPM_S_MSG, /**< @brief Type for "3gpp-service.ims.icsi.oma.cpm.systemmsg". */
    EN_SIP_AC_PARMS_C_P_INSTANCE,   /**< @brief Type "+sip.instance" */
    EN_SIP_AC_PARMS_G_ISBOT,        /**< @brief Type "+g.gsma.rcs.isbot" */
    EN_SIP_AC_PARMS_GEN              /**< @brief Type for otherwise. */
} EN_SIP_AC_PARMS;

/** @brief Type of SIP privacy-tag. */
typedef enum EN_SIP_PRIVACY_TAG
{
    EN_SIP_PRIVACY_TAG_IPADDR,       /**< @brief Type for "ipaddr". */
    EN_SIP_PRIVACY_TAG_OFF,          /**< @brief Type for "off". */
    EN_SIP_PRIVACY_TAG_OTHER         /**< @brief Type for otherwise. */
} EN_SIP_PRIVACY_TAG;

/** @brief Type of SIP rpi-priv-element. */
typedef enum EN_SIP_RPI_PRIV_ELEM
{
    EN_SIP_RPI_PRIV_ELEM_FULL,       /**< @brief Type for "full". */
    EN_SIP_RPI_PRIV_ELEM_NAME,       /**< @brief Type for "name". */
    EN_SIP_RPI_PRIV_ELEM_URI,        /**< @brief Type for "uri". */
    EN_SIP_RPI_PRIV_ELEM_OFF,        /**< @brief Type for "off". */
    EN_SIP_RPI_PRIV_ELEM_OTHER       /**< @brief Type for otherwise. */
} EN_SIP_RPI_PRIV_ELEM;

/** @brief Type of SIP rpi-priv-element policy. */
typedef enum EN_SIP_RPI_PRIV_ELEM_POLICY
{
    EN_SIP_RPI_PRIV_ELEM_POLICY_NETWORK, /**< @brief Type for "network". */
    EN_SIP_RPI_PRIV_ELEM_POLICY_OTHER /**< @brief Type for otherwise. */
} EN_SIP_RPI_PRIV_ELEM_POLICY;

/** @brief Type of SIP rpid-privacy-token. */
typedef enum EN_SIP_RPID_PRIV_TKN
{
    EN_SIP_RPID_PRIV_TKN_PTY,        /**< @brief Type for "party". */
    EN_SIP_RPID_PRIV_TKN_ID,         /**< @brief Type for "id-type". */
    EN_SIP_RPID_PRIV_TKN_PRIVACY     /**< @brief Type for "privacy". */
} EN_SIP_RPID_PRIV_TKN;

/** @brief Type of SIP rpi-pty-type. */
typedef enum EN_SIP_RPI_SCREEN
{
    EN_SIP_RPI_SCREEN_NO,            /**< @brief Type for "no". */
    EN_SIP_RPI_SCREEN_YES            /**< @brief Type for "yes". */
} EN_SIP_RPI_SCREEN;

/** @brief Type of SIP rpi-pty-type. */
typedef enum EN_SIP_RPI_PTY_TYPE
{
    EN_SIP_RPI_PTY_TYPE_CALLING,     /**< @brief Type for "calling". */
    EN_SIP_RPI_PTY_TYPE_CALLED,      /**< @brief Type for "called". */
    EN_SIP_RPI_PTY_TYPE_OTHER        /**< @brief Type for otherwise. */
} EN_SIP_RPI_PTY_TYPE;

/** @brief Type of SIP rpi-id-type. */
typedef enum EN_SIP_RPI_ID_TYPE
{
    EN_SIP_RPI_ID_TYPE_SUBS,         /**< @brief Type for "subscriber". */
    EN_SIP_RPI_ID_TYPE_USER,         /**< @brief Type for "user". */
    EN_SIP_RPI_ID_TYPE_TERM,         /**< @brief Type for "term". */
    EN_SIP_RPI_ID_TYPE_OTHER         /**< @brief Type for otherwise. */
} EN_SIP_RPI_ID_TYPE;

/** @brief Type of SIP rpi-token. */
typedef enum EN_SIP_RPI_TKN
{
    EN_SIP_RPI_TKN_SCREEN,           /**< @brief Type for "screen". */
    EN_SIP_RPI_TKN_PTY,              /**< @brief Type for "party". */
    EN_SIP_RPI_TKN_ID,               /**< @brief Type for "id-type". */
    EN_SIP_RPI_TKN_PRIVACY,          /**< @brief Type for "privacy". */
    EN_SIP_RPI_TKN_OTHER             /**< @brief Type for otherwise. */
} EN_SIP_RPI_TKN;

/** @brief Type of SIP join-param. */
typedef enum EN_SIP_JOIN_PARM
{
    EN_SIP_JOIN_PARM_TO_TAG,         /**< @brief Type for "to-tag". */
    EN_SIP_JOIN_PARM_FROM_TAG,       /**< @brief Type for "from-tag". */
    EN_SIP_JOIN_PARM_GEN             /**< @brief Type for otherwise. */
} EN_SIP_JOIN_PARM;

/** @brief Type of SIP access-type. */
typedef enum EN_SIP_ACCESS_TYPE
{
    EN_SIP_ACCESS_TYPE_IEEE_802_11A, /**< @brief Type for "IEEE-802.11a". */
    EN_SIP_ACCESS_TYPE_IEEE_802_11B, /**< @brief Type for "IEEE-802.11b". */
    EN_SIP_ACCESS_TYPE_3GPP_GERAN,   /**< @brief Type for "3GPP-GERAN". */
    EN_SIP_ACCESS_TYPE_3GPP_UTRAN_FDD, /**< @brief Type for "3GPP-UTRAN-FDD". */
    EN_SIP_ACCESS_TYPE_3GPP_UTRAN_TDD, /**< @brief Type for "3GPP-UTRAN-TDD". */
    EN_SIP_ACCESS_TYPE_3GPP_CDMA2000, /**< @brief Type for "3GPP-CDMA2000". */

    /* externsion in 3gpp 24.229-920 7.2A.4.2 */
    EN_SIP_ACCESS_TYPE_IEEE_802_11,      /**< @brief Type for "IEEE-802.11" */
    EN_SIP_ACCESS_TYPE_IEEE_802_11G,     /**< @brief Type for "IEEE-802.11g" */
    EN_SIP_ACCESS_TYPE_IEEE_802_11N,     /**< @brief Type for "IEEE-802.11n" */
    EN_SIP_ACCESS_TYPE_3GPP_E_UTRAN_FDD, /**< @brief Type for "3GPP-E-UTRAN-FDD" */
    EN_SIP_ACCESS_TYPE_3GPP_E_UTRAN_TDD, /**< @brief Type for "3GPP-E-UTRAN-TDD" */
    EN_SIP_ACCESS_TYPE_ADSL,             /**< @brief Type for "ADSL" */
    EN_SIP_ACCESS_TYPE_ADSL2,            /**< @brief Type for "ADSL2" */
    EN_SIP_ACCESS_TYPE_ADSL2X,           /**< @brief Type for "ADSL2+" */
    EN_SIP_ACCESS_TYPE_RADSL,            /**< @brief Type for "RADSL" */
    EN_SIP_ACCESS_TYPE_SDSL,             /**< @brief Type for "SDSL" */
    EN_SIP_ACCESS_TYPE_HDSL,             /**< @brief Type for "HDSL" */
    EN_SIP_ACCESS_TYPE_HDSL2,            /**< @brief Type for "HDSL2" */
    EN_SIP_ACCESS_TYPE_G_SHDSL,          /**< @brief Type for "G.SHDSL" */
    EN_SIP_ACCESS_TYPE_VDSL,             /**< @brief Type for "VDSL" */
    EN_SIP_ACCESS_TYPE_IDSL,             /**< @brief Type for "IDSL" */
    EN_SIP_ACCESS_TYPE_3GPP2_1X,         /**< @brief Type for "3GPP2-1X" */
    EN_SIP_ACCESS_TYPE_3GPP2_1X_FEMTO,   /**< @brief Type for "3GPP2-1X-Femto" */
    EN_SIP_ACCESS_TYPE_3GPP2_1X_HRPD,    /**< @brief Type for "3GPP2-1X-HRPD" */
    EN_SIP_ACCESS_TYPE_3GPP2_UMB,        /**< @brief Type for "3GPP2-UMB" */
    EN_SIP_ACCESS_TYPE_DOCSIS,           /**< @brief Type for "DOCSIS" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3,       /**< @brief Type for "IEEE-802.3" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3A,      /**< @brief Type for "IEEE-802.3a" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3E,      /**< @brief Type for "IEEE-802.3e" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3I,      /**< @brief Type for "IEEE-802.3i" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3J,      /**< @brief Type for "IEEE-802.3j" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3U,      /**< @brief Type for "IEEE-802.3u" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3AB,     /**< @brief Type for "IEEE-802.3ab" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3AE,     /**< @brief Type for "IEEE-802.3ae" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3AK,     /**< @brief Type for "IEEE-802.3ak" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3AQ,     /**< @brief Type for "IEEE-802.3aq" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3AN,     /**< @brief Type for "IEEE-802.3an" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3Y,      /**< @brief Type for "IEEE-802.3y" */
    EN_SIP_ACCESS_TYPE_IEEE_802_3Z,      /**< @brief Type for "IEEE-802.3z" */
    EN_SIP_ACCESS_TYPE_GPON,             /**< @brief Type for "GPON" */
    EN_SIP_ACCESS_TYPE_XGPON1,           /**< @brief Type for "XGPON1" */
    EN_SIP_ACCESS_TYPE_GSTN,             /**< @brief Type for "GSTN" */
    EN_SIP_ACCESS_TYPE_3GPP_UTRAN,       /**< @brief Type for "3GPP-UTRAN" */
    EN_SIP_ACCESS_TYPE_3GPP_E_UTRAN,     /**< @brief Type for "3GPP-E-UTRAN" */
    EN_SIP_ACCESS_TYPE_3GPP_WLAN,        /**< @brief Type for "3GPP-WLAN" */
    EN_SIP_ACCESS_TYPE_3GPP_GAN,         /**< @brief Type for "3GPP-GAN" */
    EN_SIP_ACCESS_TYPE_3GPP_HSPA,        /**< @brief Type for "3GPP-HSPA" */
    EN_SIP_ACCESS_TYPE_OTHER             /**< @brief Type for otherwise. */
} EN_SIP_ACCESS_TYPE;

/** @brief Type of SIP access-info. */
typedef enum EN_SIP_ACCESS_INFO
{
    EN_SIP_ACCESS_INFO_CGI_3GPP,     /**< @brief Type for "cgi-3gpp". */
    EN_SIP_ACCESS_INFO_UTRAN_CELL_ID_3GPP, /**< @brief Type for "utran-cell-id-3gpp". */

    /* externsion in 3gpp 24.229-920 7.2A.4.2 */
    EN_SIP_ACCESS_INFO_DSL_LOCATION, /**< @brief Type for "dsl-location". */
    EN_SIP_ACCESS_INFO_I_WLAN_NODE_ID, /**< @brief Type for "i-wlan-node-id". */
    EN_SIP_ACCESS_INFO_CI_3GPP2,     /**< @brief Type for "ci-3gpp2". */
    EN_SIP_ACCESS_INFO_CI_3GPP2_FEMTO, /**< @brief Type for "ci-3gpp2-femto". */
    EN_SIP_ACCESS_INFO_ETH_LOCATION, /**< @brief Type for "eth-location". */
    EN_SIP_ACCESS_INFO_FIBER_LOCATION, /**< @brief Type for "fiber-location". */
    EN_SIP_ACCESS_INFO_NETWORK_PROVIDED, /**< @brief Type for "network-provided". */
    EN_SIP_ACCESS_INFO_GSTN_LOCATION, /**< @brief Type for "gstn-location". */
    EN_SIP_ACCESS_INFO_COUNTRY, /**< @brief Type for "country". */
    EN_SIP_ACCESS_INFO_CITY, /**< @brief Type for "city". */

    EN_SIP_ACCESS_INFO_EXT           /**< @brief Type for otherwise. */
} EN_SIP_ACCESS_INFO;

/** @brief Type of SIP charge-addr-params. */
typedef enum EN_SIP_CHARGE_ADDR_PARMS
{
    EN_SIP_CHARGE_ADDR_PARMS_CCF,    /**< @brief Type for "ccf". */
    EN_SIP_CHARGE_ADDR_PARMS_ECF,    /**< @brief Type for "ecf". */
    EN_SIP_CHARGE_ADDR_PARMS_GEN     /**< @brief Type for otherwise. */
} EN_SIP_CHARGE_ADDR_PARMS;

/** @brief Type of SIP charge-params. */
typedef enum EN_SIP_CHARGE_PARMS
{
    EN_SIP_CHARGE_PARMS_ICID_GEN,    /**< @brief Type for "icid-generated-at". */
    EN_SIP_CHARGE_PARMS_ORIG_IOI,    /**< @brief Type for "orig-ioi". */
    EN_SIP_CHARGE_PARMS_TERM_IOI,    /**< @brief Type for "term-ioi". */
    EN_SIP_CHARGE_PARMS_GEN          /**< @brief Type for otherwise. */
} EN_SIP_CHARGE_PARMS;

/** @brief Type of SIP priv-value. */
typedef enum EN_SIP_PRIV_VAL
{
    EN_SIP_PRIV_VAL_HDR,             /**< @brief Type for "header". */
    EN_SIP_PRIV_VAL_SESS,            /**< @brief Type for "session". */
    EN_SIP_PRIV_VAL_USER,            /**< @brief Type for "user". */
    EN_SIP_PRIV_VAL_NONE,            /**< @brief Type for "none". */
    EN_SIP_PRIV_VAL_CRITICAL,        /**< @brief Type for "critical". */
    EN_SIP_PRIV_VAL_ID,              /**< @brief Type for "id". */
    EN_SIP_PRIV_VAL_TKN              /**< @brief Type for otherwise. */
} EN_SIP_PRIV_VAL;

/** @brief Type of SIP protocol. */
typedef enum EN_SIP_PROTOCOL
{
    EN_SIP_PROTOCOL_SIP,             /**< @brief Type for "SIP". */
    EN_SIP_PROTOCOL_Q850,            /**< @brief Type for "Q.850". */
    EN_SIP_PROTOCOL_TKN              /**< @brief Type for otherwise. */
} EN_SIP_PROTOCOL;

/** @brief Type of SIP reason-params. */
typedef enum EN_SIP_REASON_PARMS
{
    EN_SIP_REASON_PARMS_CAUSE,       /**< @brief Type for "cause". */
    EN_SIP_REASON_PARMS_TXT,         /**< @brief Type for "text". */
    EN_SIP_REASON_PARMS_GEN          /**< @brief Type for otherwise. */
} EN_SIP_REASON_PARMS;

/** @brief Type of SIP mechanism-name. */
typedef enum EN_SIP_MECH_NAME
{
    EN_SIP_MECH_NAME_DIGEST,         /**< @brief Type for "digest". */
    EN_SIP_MECH_NAME_TLS,            /**< @brief Type for "tls". */
    EN_SIP_MECH_NAME_IPSEC_IKE,      /**< @brief Type for "ipsec-ike". */
    EN_SIP_MECH_NAME_IPSEC_MAN,      /**< @brief Type for "ipsec-man". */
    EN_SIP_MECH_NAME_IPSEC_3GPP,     /**< @brief Type for "ipsec-3gpp". */
    EN_SIP_MECH_NAME_TKN             /**< @brief Type for otherwise. */
} EN_SIP_MECH_NAME;

/** @brief Type of SIP mech-parameters. */
typedef enum EN_SIP_MECH_PARMS
{
    EN_SIP_MECH_PARMS_PREF,          /**< @brief Type for "q". */
    EN_SIP_MECH_PARMS_DIGEST_ALGO,   /**< @brief Type for "d-alg". */
    EN_SIP_MECH_PARMS_DIGEST_QOP,    /**< @brief Type for "d-qop". */
    EN_SIP_MECH_PARMS_DIGEST_VERIFY, /**< @brief Type for "d-ver". */
    EN_SIP_MECH_PARMS_ALGO,          /**< @brief Type for "alg". */
    EN_SIP_MECH_PARMS_PROT,          /**< @brief Type for "prot". */
    EN_SIP_MECH_PARMS_MOD,           /**< @brief Type for "mod". */
    EN_SIP_MECH_PARMS_EALG,          /**< @brief Type for "ealg". */
    EN_SIP_MECH_PARMS_SPI,           /**< @brief Type for "spi". */
    EN_SIP_MECH_PARMS_SPI_C,         /**< @brief Type for "spi-c". */
    EN_SIP_MECH_PARMS_SPI_S,         /**< @brief Type for "spi-s". */
    EN_SIP_MECH_PARMS_PORT1,         /**< @brief Type for "port1". */
    EN_SIP_MECH_PARMS_PORT2,         /**< @brief Type for "port2". */
    EN_SIP_MECH_PARMS_PORT_C,        /**< @brief Type for "port-c". */
    EN_SIP_MECH_PARMS_PORT_S,        /**< @brief Type for "port-s". */
    EN_SIP_MECH_PARMS_EXT            /**< @brief Type for otherwise. */
} EN_SIP_MECH_PARMS;

/** @brief Type of SIP OSPS-Tag. */
typedef enum EN_SIP_OSPS_TAG
{
    EN_SIP_OSPS_TAG_BLV,             /**< @brief Type for "BLV". */
    EN_SIP_OSPS_TAG_EI,              /**< @brief Type for "EI". */
    EN_SIP_OSPS_TAG_RING,            /**< @brief Type for "RING". */
    EN_SIP_OSPS_TAG_TKN              /**< @brief Type for otherwise. */
} EN_SIP_OSPS_TAG;

/** @brief Type of SIP Billing-Info-param. */
typedef enum EN_SIP_BILL_INFO_PARM
{
    EN_SIP_BILL_INFO_PARM_RKS_GRP_ID, /**< @brief Type for "rksgroup". */
    EN_SIP_BILL_INFO_PARM_CHARGE,    /**< @brief Type for "charge". */
    EN_SIP_BILL_INFO_PARM_CALLING,   /**< @brief Type for "calling". */
    EN_SIP_BILL_INFO_PARM_CALLED,    /**< @brief Type for "called". */
    EN_SIP_BILL_INFO_PARM_ROUTING,   /**< @brief Type for "routing". */
    EN_SIP_BILL_INFO_PARM_LOC_ROUTING, /**< @brief Type for "locroute". */
    EN_SIP_BILL_INFO_PARM_GEN        /**< @brief Type for otherwise. */
} EN_SIP_BILL_INFO_PARM;

/** @brief Type of SIP Laes-param. */
typedef enum EN_SIP_LAES_PARM
{
    EN_SIP_LAES_PARM_CONTENT,        /**< @brief Type for "content". */
    EN_SIP_LAES_PARM_KEY,            /**< @brief Type for "key". */
    EN_SIP_LAES_PARM_GEN             /**< @brief Type for otherwise. */
} EN_SIP_LAES_PARM;

/** @brief Type of SIP redir-params. */
typedef enum EN_SIP_REDIR_PARMS
{
    EN_SIP_REDIR_PARMS_URI,          /**< @brief Type for "redirector-uri". */
    EN_SIP_REDIR_PARMS_COUNT,        /**< @brief Type for "count". */
    EN_SIP_REDIR_PARMS_GEN           /**< @brief Type for otherwise. */
} EN_SIP_REDIR_PARMS;

/** @brief Type of SIP alerting-type. */
typedef enum EN_SIP_ALERT_TYPE
{
    EN_SIP_ALERT_TYPE_MANUAL,        /**< @brief Type for "Manual". */
    EN_SIP_ALERT_TYPE_AUTO,          /**< @brief Type for "Auto". */
    EN_SIP_ALERT_TYPE_MAO            /**< @brief Type for "MAO". */
} EN_SIP_ALERT_TYPE;

/** @brief Type of SIP diameter transport. */
typedef enum EN_SIP_DTPT
{
    EN_SIP_DTPT_TCP,                 /**< @brief Type for "tcp". */
    EN_SIP_DTPT_SCTP,                /**< @brief Type for "sctp". */
    EN_SIP_DTPT_UDP                  /**< @brief Type for "udp". */
} EN_SIP_DTPT;

/** @brief Type of SIP diameter protocol. */
typedef enum EN_SIP_DPROTO
{
    EN_SIP_DPROTO_DIAM,              /**< @brief Type for "diameter". */
    EN_SIP_DPROTO_RDUS,              /**< @brief Type for "radius". */
    EN_SIP_DPROTO_TACACS             /**< @brief Type for "tacacs+". */
} EN_SIP_DPROTO;

/** @brief Type of SIP answer-mode-value. */
typedef enum EN_SIP_ANSWER_MODE_VAL
{
    EN_SIP_ANSWER_MODE_VAL_MANUAL,   /**< @brief Type for "Manual". */
    EN_SIP_ANSWER_MODE_VAL_AUTO,     /**< @brief Type for "Auto". */
    EN_SIP_ANSWER_MODE_VAL_TKN       /**< @brief Type for otherwise. */
} EN_SIP_ANSWER_MODE_VAL;

/** @brief Type of SIP answer-mode-param. */
typedef enum EN_SIP_ANSWER_MODE_PARM
{
    EN_SIP_ANSWER_MODE_PARM_REQUIRE, /**< @brief Type for "require". */
    EN_SIP_ANSWER_MODE_PARM_GEN      /**< @brief Type for otherwise. */
} EN_SIP_ANSWER_MODE_PARM;

/** @brief Type of P-Early-Media parameter type */
typedef enum EN_SIP_P_EARLY_MEDIA_PARM
{
    EN_SIP_P_EARLY_MEDIA_PARM_SENDRECV, /**< @brief Type for "sendrecv". */
    EN_SIP_P_EARLY_MEDIA_PARM_SENDONLY, /**< @brief Type for "sendonly". */
    EN_SIP_P_EARLY_MEDIA_PARM_RECVONLY, /**< @brief Type for "recvonly". */
    EN_SIP_P_EARLY_MEDIA_PARM_INACTIVE, /**< @brief Type for "inactive". */
    EN_SIP_P_EARLY_MEDIA_PARM_GATED, /**< @brief Type for "gated". */
    EN_SIP_P_EARLY_MEDIA_PARM_SUPPORTED, /**< @brief Type for "supported". */
    EN_SIP_P_EARLY_MEDIA_PARM_TOKEN  /**< @brief Type for token. */
} EN_SIP_P_EARLY_MEDIA_PARM;

/** @brief Type of enc-feature-tag type*/
typedef enum EN_SIP_ENCF_TAG
{
    EN_SIP_ENCF_TAG_AUDIO,           /**< @brief Type for "audio". */
    EN_SIP_ENCF_TAG_AUTOMATA,        /**< @brief Type for "automata". */
    EN_SIP_ENCF_TAG_CLASS,           /**< @brief Type for "class". */
    EN_SIP_ENCF_TAG_DUPLEX,          /**< @brief Type for "duplex". */
    EN_SIP_ENCF_TAG_DATA,            /**< @brief Type for "data". */
    EN_SIP_ENCF_TAG_CONTROL,         /**< @brief Type for "control". */
    EN_SIP_ENCF_TAG_MOBILITY,        /**< @brief Type for "mobility". */
    EN_SIP_ENCF_TAG_DESCRIPTION,     /**< @brief Type for "description". */
    EN_SIP_ENCF_TAG_EVENTS,          /**< @brief Type for "events". */
    EN_SIP_ENCF_TAG_PRIORITY,        /**< @brief Type for "priority". */
    EN_SIP_ENCF_TAG_METHODS,         /**< @brief Type for "methods". */
    EN_SIP_ENCF_TAG_SCHEMES,         /**< @brief Type for "schemes". */
    EN_SIP_ENCF_TAG_APPLICATION,     /**< @brief Type for "application". */
    EN_SIP_ENCF_TAG_VIDEO,           /**< @brief Type for "video". */
    EN_SIP_ENCF_TAG_LANGUAGE,        /**< @brief Type for "language". */
    EN_SIP_ENCF_TAG_TYPE,            /**< @brief Type for "type". */
    EN_SIP_ENCF_TAG_ISFOCUS,         /**< @brief Type for "isfocus". */
    EN_SIP_ENCF_TAG_ACTOR,           /**< @brief Type for "actor". */
    EN_SIP_ENCF_TAG_TEXT,            /**< @brief Type for "text". */
    EN_SIP_ENCF_TAG_EXTNS,           /**< @brief Type for "extensions". */
    EN_SIP_ENCF_TAG_OTHER            /**< @brief Type for other. */
} EN_SIP_ENCF_TAG;

/** @brief Type of Target-Dialog parameter type */
typedef enum EN_SIP_TD_PARM
{
    EN_SIP_TD_PARM_REMOTE,           /**< @brief Type for "remote-tag". */
    EN_SIP_TD_PARM_LOCAL,            /**< @brief Type for "local-tag". */
    EN_SIP_TD_PARM_GEN               /**< @brief Type for otherwise. */
} EN_SIP_TD_PARM;

/** @brief Type of SIP Conference-Info MediaTypes. */
typedef enum EN_SIP_CONF_INFO_MEDIA
{
    EN_SIP_CONF_INFO_MEDIA_VOICE,         /**< @brief Type for ""(not defined right now). */
    EN_SIP_CONF_INFO_MEDIA_VIDEO,         /**< @brief Type for "Voice.HDVideo". */
    EN_SIP_CONF_INFO_MEDIA_OTHER          /**< @brief Type for otherwise. */
} EN_SIP_CONF_INFO_MEDIA;

/** @brief Type of SIP Conference-Info role type. */
typedef enum EN_SIP_CONF_INFO_ROLE
{
    EN_SIP_CONF_INFO_ROLE_CHAIRMAN,       /**< @brief Type for "chairman". */
    EN_SIP_CONF_INFO_ROLE_GENERAL,        /**< @brief Type for "general". */
    EN_SIP_CONF_INFO_ROLE_OTHER           /**< @brief Type for otherwise. */
} EN_SIP_CONF_INFO_ROLE;

/** @brief Type of terminal seat type. */
typedef enum EN_SIP_SEAT_TYPE
{
    EN_SIP_SEAT_UNKNOWN,                 /* for unknown type */
    EN_SIP_SEAT_CAPTAIN,                 /* for captain type */
    EN_SIP_SEATDIRECTOR,                 /* for director type */
    EN_SIP_SEAT_ORDINARY,                /* for ordinary type */
    EN_SIP_SEAT_ASSISTANT,               /* for assistant type */
} EN_SIP_TERMINAL_TYPE;

/***************************** rfc3261 typedefs *************************/

/** @brief SIP structure string. */
typedef ST_ZOS_SSTR ST_SIP_SSTR;

/** @brief SIP structure string list. */
ABNF_TYPEDEF_LIST(SIP_SSTR); 

/** @brief SIP content-coding. */
typedef ST_ZOS_SSTR ST_SIP_CONTENT_CODING;

/** @brief SIP content-coding list. */
ABNF_TYPEDEF_LIST(SIP_CONTENT_CODING); 

/** @brief Structure of SIP type value. */
typedef struct tagSIP_TYPE_VAL
{
    ZUCHAR ucType;                   /**< @brief type value */  
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /**< @brief extension or unknown type string */
} ST_SIP_TYPE_VAL;

/** @brief SIP type value list. */
ABNF_TYPEDEF_LIST(SIP_TYPE_VAL);

/** @brief Structure of SIP Method. */
typedef struct tagSIP_METHOD
{
    ZUCHAR ucType;                   /**< @brief method type EN_SIP_METHOD */  
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /**< @brief extension or unknown type string */
} ST_SIP_METHOD;

/** @brief List of SIP method */
ABNF_TYPEDEF_LIST(SIP_METHOD); 

/** @brief Structure of SIP date. */
typedef struct tagSIP_DATE
{
    ZUSHORT wYear;                   /**< @brief date1 year */
    ZUCHAR ucMonth;                  /**< @brief date1 month EN_SIP_MONTH */
    ZUCHAR ucDay;                    /**< @brief date1 day */
    ZUCHAR ucWkDay;                  /**< @brief wkday EN_SIP_WKDAY */
    ZUCHAR ucHour;                   /**< @brief time hour */
    ZUCHAR ucMinute;                 /**< @brief time minute */
    ZUCHAR ucSecond;                 /**< @brief second */
    ST_ZOS_SSTR stWkDay;             /**< @brief weak day string */
    ST_ZOS_SSTR stMonth;             /**< @brief month string */
    ST_ZOS_SSTR stZone;              /**< @brief time zone */
} ST_SIP_DATE;

/** @brief Structure of SIP other parameter. */
typedef struct tagSIP_OTHER_PARM
{
    ZUCHAR ucValPres;                /**< @brief value present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stName;              /**< @brief name */
    ST_ZOS_SSTR stVal;               /**< @brief value */
} ST_SIP_OTHER_PARM;

/** @brief Structure of SIP userinfo. */
typedef struct tagSIP_USER_INFO
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucPasswdPres;             /**< @brief password present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stUser;              /**< @brief user / telephone-subscriber */
    ST_ZOS_SSTR stPasswd;            /**< @brief password */
} ST_SIP_USER_INFO;

/** @brief Structure of SIP host */
typedef struct tagSIP_HOST
{
    ZUCHAR ucType;                   /**< @brief host type EN_SIP_HOST */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stName;          /**< @brief hostname */
        ZUINT iIpv4Addr;             /**< @brief IPv4address */
        ZUCHAR aucIpv6Addr[ZINET_IPV6_ADDR_SIZE]; /**< @brief IPv6reference */
    } u;
} ST_SIP_HOST;

/** @brief Structure of SIP hostport. */
typedef struct tagSIP_HOST_PORT
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucPortPres;               /**< @brief Port present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ZUINT iPort;                     /**< @brief port */
    ST_SIP_HOST stHost;              /**< @brief host */
} ST_SIP_HOST_PORT;

/** @brief Structure of SIP uri-parameter. */
typedef struct ST_SIP_URI_PARM
{
    ZUCHAR ucType;                   /**< @brief uri parameter type EN_SIP_URI_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_TYPE_VAL stTpt;       /**< @brief transport-param EN_SIP_TPT_PARM */
        ST_SIP_TYPE_VAL stUser;      /**< @brief user-param EN_SIP_USER_PARM */
        ST_SIP_METHOD stMethod;      /**< @brief method-param EN_SIP_METHOD */
        ZUINT iTtl;                /**< @brief ttl-param */
        ZUINT iCause;              /**< @brief cause */
        ST_SIP_HOST stMaddr;         /**< @brief maddr-param */
        ZUCHAR ucSess;               /**< @brief session param EN_SIP_SESS_PARM */
        ST_ZOS_SSTR stTarget;        /**< @brief target */
        ST_ZOS_SSTR stGr;            /**< @brief gr-param */
        ST_ZOS_SSTR stAuid;          /**< @brief auid-param */
        ST_SIP_OTHER_PARM stOther;   /**< @brief other-param */
    }u;
} ST_SIP_URI_PARM;

/** @brief SIP uri-parameter list. */
ABNF_TYPEDEF_LIST(SIP_URI_PARM); 

/** @brief Structure of SIP header. */
typedef struct tagSIP_HDR
{
    ST_ZOS_SSTR stHname;             /**< @brief hname */
    ST_ZOS_SSTR stHval;              /**< @brief hvalue */ 
} ST_SIP_HDR;

/** @brief SIP header list. */
ABNF_TYPEDEF_LIST(SIP_HDR); 

/** @brief Structure of SIP headers. */
typedef struct tagSIP_HDRS
{
    ST_SIP_HDR_LST stHdrLst;         /**< @brief header list */
} ST_SIP_HDRS;

/** @brief Structure of SIP-URI. */
typedef struct tagSIP_SIP_URI
{
    ZUCHAR ucUserInfoPres;           /**< @brief userinfo present flag */
    ZUCHAR ucHdrsPres;               /**< @brief Headers present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_SIP_USER_INFO stUserInfo;     /**< @brief userinfo */
    ST_SIP_HOST_PORT stHostPort;     /**< @brief hostport */
    ST_SIP_URI_PARM_LST stUriParmLst; /**< @brief uri-parameters */
    ST_SIP_HDRS stHdrs;              /**< @brief Headers */
} ST_SIP_SIP_URI;

/** @brief Structure of SIP parameter. */
typedef struct tagSIP_PARM
{
    ZUCHAR ucValPres;                /**< @brief pvalue present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stPname;             /**< @brief pname */
    ST_ZOS_SSTR stPval;              /**< @brief pvalue */
} ST_SIP_PARM;

/** @brief Structure of SIP par. */
typedef struct tagSIP_PAR
{
    ZUCHAR ucType;                   /**< @brief uri parameter type EN_SIP_PAR */
    ZUCHAR ucGlobalNumDigPres;       /**< @brief global-number-digits present flag 
                                        of descriptor in context*/
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stIsub;          /**< @brief isdn-subaddress */
        ST_ZOS_SSTR stExt;           /**< @brief extension */
        ST_ZOS_SSTR stContext;       /**< @brief context */
        ST_SIP_PARM stParm;          /**< @brief parameter */
    } u;
} ST_SIP_PAR;

/** @brief SIP par list. */
ABNF_TYPEDEF_LIST(SIP_PAR);

/** @brief Structure of SIP global-number. */
typedef struct tagSIP_GLOBAL_NUMBER
{
    ST_ZOS_SSTR stNumberDigits;      /**< @brief global-number-digits */
    ST_SIP_PAR_LST stParLst;         /**< @brief par list */
} ST_SIP_GLOBAL_NUMBER;

/** @brief Structure of SIP local-number. */
typedef struct tagSIP_LOCAL_NUMBER
{
    ST_ZOS_SSTR stNumberDigits;      /**< @brief local-number-digits */
    ST_SIP_PAR_LST stParLst;         /**< @brief par list */
} ST_SIP_LOCAL_NUMBER;

/** @brief Structure of SIP telephone-uri in rfc2806. */
typedef struct tagSIP_TEL_URI
{
    ZUCHAR ucGlobalNumberPres;       /**< @brief global-number present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_GLOBAL_NUMBER stGlobal; /**< @brief global-number */
        ST_SIP_LOCAL_NUMBER stLocal; /**< @brief local-number */
    } u;
} ST_SIP_TEL_URI;

/** @brief Structure of SIP absoluteURI. */
typedef struct tagSIP_ABSO_URI
{
    ST_ZOS_SSTR stScheme;            /**< @brief scheme */
    ST_ZOS_SSTR stDesc;              /**< @brief hier-part / opaque-part */
} ST_SIP_ABSO_URI;

/** @brief Structure of SIP IM-URI in rfc3860. */
typedef struct tagSIP_IM_URI
{
    ZUCHAR ucDescPres;               /**< @brief [ to ] [ headers ] present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stDesc;              /**< @brief [ to ] [ headers ] in rfc3860 */
} ST_SIP_IM_URI;

/** @brief Structure of SIP Message-ID & Content-ID uri in rfc2392. */
typedef struct tagSIP_MC_URI
{
    ST_ZOS_SSTR stLocal;             /**< @brief local-part */
    ST_SIP_HOST stHost;              /**< @brief host */
    ST_SIP_URI_PARM_LST stUriParmLst; /**< @brief uri-parameters */
} ST_SIP_MC_URI;

/** @brief Structure of SIP addr-spec. */
typedef struct tagSIP_ADDR_SPEC
{
    ZUCHAR ucType;                   /**< @brief addr-spec type EN_SIP_ADDR_SPEC */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_SIP_URI stSipUri;     /**< @brief SIP-URI */
        ST_SIP_SIP_URI stSipsUri;    /**< @brief SIPS-URI */
        ST_SIP_TEL_URI stTelUri;     /**< @brief TEL-URI */
        ST_SIP_IM_URI stImUri;       /**< @brief IM-URI rfc3860 */        
        ST_SIP_IM_URI stPresUri;     /**< @brief PRES-URI rfc3859 */        
        ST_SIP_IM_URI stMailtoUri;   /**< @brief MAILTO-URI rfc2368 */        
        ST_SIP_MC_URI stMidUri;      /**< @brief Message-ID uri */
        ST_SIP_MC_URI stCidUri;      /**< @brief Content-ID uri */
        ST_SIP_ABSO_URI stAbsoUri;   /**< @brief absoluteURI */
    } u;
} ST_SIP_ADDR_SPEC;

/** @brief Structure of SIP display-name. */
typedef struct tagSIP_DISP_NAME
{
    ZUCHAR ucNamePres;               /**< @brief name present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stName;          /**< @brief token name */
        ST_ZOS_SSTR stQStr;          /**< @brief quoted-string */
    } u;
} ST_SIP_DISP_NAME;

/** @brief Structure of SIP name-addr. */
typedef struct tagSIP_NAME_ADDR
{
    ZUCHAR ucDispNamePres;           /**< @brief display-name present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_DISP_NAME stDispName;     /**< @brief display-name */
    ST_SIP_ADDR_SPEC stAddrSpec;     /**< @brief addr-spec */
} ST_SIP_NAME_ADDR;

/** @brief Structure of SIP name-addr or addr-spec. */
typedef struct tagSIP_NA_SPEC
{
    ZUCHAR ucNameAddrPres;           /**< @brief name-addr present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_NAME_ADDR stNameAddr; /**< @brief name-addr */
        ST_SIP_ADDR_SPEC stAddrSpec; /**< @brief addr-spec */
    } u;
} ST_SIP_NA_SPEC;

/** @brief Structure of SIP Request-URI. */
typedef struct tagSIP_REQ_URI
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucType;                   /**< @brief Request-URI type EN_SIP_REQ_URI */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_SIP_URI stSipUri;     /**< @brief SIP-URI */
        ST_SIP_SIP_URI stSipsUri;    /**< @brief SIPS-URI */
        ST_SIP_IM_URI stImUri;       /**< @brief IM-URI rfc3860 */        
        ST_SIP_IM_URI stPresUri;     /**< @brief PRES-URI rfc3859 */        
        ST_SIP_ABSO_URI stAbsoUri;   /**< @brief absoluteURI */
        ST_SIP_TEL_URI stTelUri;     /**< @brief TEL-URI */
    } u;
} ST_SIP_REQ_URI;

/** @brief Structure of SIP m-parameter. */
typedef struct tagSIP_MPARM
{
    ZUCHAR ucQvalPres;               /**< @brief quoted m-value present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stMattr;             /**< @brief m-attribute */
    ST_ZOS_SSTR stMval;              /**< @brief m-value string */    
} ST_SIP_MPARM;

/** @brief List of SIP m-parameter. */
ABNF_TYPEDEF_LIST(SIP_MPARM); 

/** @brief Structure of SIP media-type. */
typedef struct tagSIP_MEDIA_TYPE
{
    ZUCHAR ucMtype;                  /**< @brief m-type type EN_SIP_MTYPE */
    ZUCHAR ucMSubtype;               /**< @brief m-subtype type EN_SIP_MSUBTYPE */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stMtypeExt;          /**< @brief m-type extension-token */
    ST_ZOS_SSTR stMSubTypeExt;       /**< @brief m-subtype extension-token */
    ST_SIP_MPARM_LST stParmLst;      /**< @brief m-parameter list */
} ST_SIP_MEDIA_TYPE;

/** @brief Structure of SIP media-range. */
typedef ST_SIP_MEDIA_TYPE ST_SIP_MEDIA_RANGE;

/** @brief Structure of SIP qvalue. */
typedef struct tagSIP_QVAL
{
    ZUCHAR ucFracPres;               /**< @brief fraction present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ZUSHORT wInt;                    /**< @brief qvalue integer */
    ZUSHORT wFrac;                   /**< @brief qvalue fraction */
} ST_SIP_QVAL;

/** @brief Structure of SIP gen-value. */
typedef struct tagSIP_GEN_VAL
{
    ZUCHAR ucType;                   /**< @brief gen-value type EN_SIP_GEN_VAL */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stVal;           /**< @brief value string */
        ST_SIP_HOST stHost;          /**< @brief host */
        ST_ZOS_SSTR stQStr;          /**< @brief quoted-string */
    } u;
} ST_SIP_GEN_VAL;

/** @brief Structure of SIP generic-param. */
typedef struct tagSIP_GEN_PARM
{
    ZUCHAR ucValPres;                /**< @brief gen-value present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stName;              /**< @brief param name */
    ST_SIP_GEN_VAL stGenVal;         /**< @brief gen-value */
} ST_SIP_GEN_PARM;

/** @brief List of SIP generic-param. */
ABNF_TYPEDEF_LIST(SIP_GEN_PARM); 

/** @brief Structure of SIP accept-param. */
typedef struct tagSIP_ACPT_PARM
{
    ZUCHAR ucQvalPres;               /**< @brief qvalue present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_QVAL stQval;          /**< @brief qvalue */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_ACPT_PARM;

/** @brief List of SIP accept-param. */
ABNF_TYPEDEF_LIST(SIP_ACPT_PARM); 

/** @brief Structure of SIP accept-range. */
typedef struct tagSIP_ACPT_RANGE
{
    ST_SIP_MEDIA_RANGE stMediaRange; /**< @brief media-range */
    ST_SIP_ACPT_PARM_LST stParmLst;  /**< @brief accept-param list */
} ST_SIP_ACPT_RANGE;

/** @brief List of SIP accept-range. */
ABNF_TYPEDEF_LIST(SIP_ACPT_RANGE); 

/** @brief Structure of SIP language-range. */
typedef struct tagSIP_LANG_RANGE
{
    ZUCHAR ucLangAllPres;            /**< @brief all(*) for languages present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stSubtag;            /**< @brief Primary-subtag */
    ST_SIP_SSTR_LST stSubtagLst;     /**< @brief Subtag list */
} ST_SIP_LANG_RANGE;

/** @brief Structure of SIP codings. */
typedef struct tagSIP_CODINGS
{
    ZUCHAR ucCodingAllPres;          /**< @brief all(*) for codings present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stContent;           /**< @brief content-coding */
} ST_SIP_CODINGS;

/** @brief Structure of SIP encoding. */
typedef struct tagSIP_ENCODING
{
    ST_SIP_CODINGS stCodings;        /**< @brief codings */
    ST_SIP_ACPT_PARM_LST stParmLst;  /**< @brief accept-param list */
} ST_SIP_ENCODING;

/** @brief List of SIP encoding. */
ABNF_TYPEDEF_LIST(SIP_ENCODING); 

/** @brief Structure of SIP language. */
typedef struct tagSIP_LANG
{
    ST_SIP_LANG_RANGE stRange;       /**< @brief language-range */
    ST_SIP_ACPT_PARM_LST stParmLst;  /**< @brief accept-param list */
} ST_SIP_LANG;

/** @brief List of SIP language. */
ABNF_TYPEDEF_LIST(SIP_LANG); 

/** @brief Structure of SIP alert-param. */
typedef struct tagSIP_ALERT_PARM
{
    ST_SIP_ABSO_URI stAbsoUri;       /**< @brief absoluteURI */
    ST_SIP_GEN_PARM_LST stParmLst;   /**< @brief generic-param list */
} ST_SIP_ALERT_PARM;

/** @brief List of SIP alert-param. */
ABNF_TYPEDEF_LIST(SIP_ALERT_PARM); 

/** @brief Structure of SIP ainfo. */
typedef struct tagSIP_AINFO
{
    ZUCHAR ucType;                   /**< @brief ainfo type EN_SIP_AINFO */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stNextNonce;     /**< @brief nextnonce */
        ST_SIP_TYPE_VAL stMsgQop;    /**< @brief message-qop:qop-value EN_SIP_QOP_VAL */        
        ST_ZOS_SSTR stRspAuth;       /**< @brief response-auth */
        ST_ZOS_SSTR stCnonce;        /**< @brief cnonce */
        ST_ZOS_SSTR stNonceCount;    /**< @brief nonce-count */
    } u;
} ST_SIP_AINFO;

/** @brief List of SIP ainfo. */
ABNF_TYPEDEF_LIST(SIP_AINFO); 

/** @brief Structure of SIP algorithm-value. */
typedef struct tagSIP_ALGO_VAL
{
    ZUCHAR ucType;                   /**< @brief algorithm type EN_SIP_ALGO_VAL */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other algorithm */
} ST_SIP_ALGO_VAL;

/** @brief Structure of SIP aka-namespace. */
typedef struct tagSIP_AKA_NS
{
    ZUINT iVer;                      /**< @brief aka-version */
    ST_SIP_ALGO_VAL stAlgoVal;       /**< @brief algorithm-value */
} ST_SIP_AKA_NS;

/** @brief Structure of SIP algorithm. */
typedef struct tagSIP_ALGO
{
    ZUCHAR ucAkaNsPres;              /**< @brief aka-namespace present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_AKA_NS stAkaNs;       /**< @brief aka-namespace */
        ST_SIP_ALGO_VAL stAlgoVal;   /**< @brief algorithm-value */
    } u;
} ST_SIP_ALGO;

/** @brief Structure of SIP auth-param. */
typedef struct tagSIP_AUTH_PARM
{
    ZUCHAR ucValQStrPres;            /**< @brief quoted-string value present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stName;              /**< @brief token name */
    ST_ZOS_SSTR stVal;               /**< @brief token / quoted-string */
} ST_SIP_AUTH_PARM;

/** @brief List of SIP auth-param. */
ABNF_TYPEDEF_LIST(SIP_AUTH_PARM); 

/** @brief Structure of SIP dig-resp. */
typedef struct tagSIP_DIG_RSP
{
    ZUCHAR ucType;                   /**< @brief dig-resp type EN_SIP_DIG_RSP */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stUserName;      /**< @brief username */
        ST_ZOS_SSTR stRealm;         /**< @brief realm */
        ST_ZOS_SSTR stNonce;         /**< @brief nonce */
        ST_SIP_REQ_URI stDigestUri;  /**< @brief digest-uri */
        ST_ZOS_SSTR stDresp;         /**< @brief dresponse */
        ST_SIP_ALGO stAlgo;          /**< @brief algorithm */
        ST_ZOS_SSTR stCnonce;        /**< @brief cnonce */
        ST_ZOS_SSTR stOpaque;        /**< @brief opaque */
        ST_SIP_TYPE_VAL stMsgQop;    /**< @brief message-qop:qop-value EN_SIP_QOP_VAL */        
        ST_ZOS_SSTR stNonceCount;    /**< @brief nonce-count */
        ST_ZOS_SSTR stAuts;          /**< @brief auts */
        ST_SIP_AUTH_PARM stAuthParm; /**< @brief auth-param */
    } u;
} ST_SIP_DIG_RSP;

/** @brief List of SIP dig-resp. */
ABNF_TYPEDEF_LIST(SIP_DIG_RSP); 

/** @brief Structure of SIP other-response. */
typedef struct tagSIP_OTHER_RSP
{
    ST_ZOS_SSTR stScheme;            /**< @brief auth-scheme */
    ST_SIP_AUTH_PARM_LST stParmLst;  /**< @brief auth-param list */
} ST_SIP_OTHER_RSP;

/** @brief Structure of SIP digest-response. */
typedef struct tagSIP_DIGEST_RSP
{
    ST_SIP_DIG_RSP_LST stDigRspLst;  /**< @brief dig-resp list */
} ST_SIP_DIGEST_RSP;
 
/** @brief Structure of SIP credentials. */
typedef struct tagSIP_CREDENTS
{
    ZUCHAR ucDigestRspPres;          /**< @brief digest-response present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_DIGEST_RSP stDigestRsp; /**< @brief digest-response */
        ST_SIP_OTHER_RSP stOther;    /**< @brief other-response */
    } u;
} ST_SIP_CREDENTS;

/** @brief Structure of SIP callid. */
typedef struct tagSIP_CALLID
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucHostPres;               /**< @brief host word present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stNumber;            /**< @brief number word */
    ST_ZOS_SSTR stHost;              /**< @brief host word */
} ST_SIP_CALLID;

/** @brief List of SIP callid. */
ABNF_TYPEDEF_LIST(SIP_CALLID); 

/** @brief Structure of SIP info-param purpose. */
typedef struct tagSIP_INFO_PURPS
{
    ZUCHAR ucType;                   /**< @brief purpose type EN_SIP_PURPS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other purpose */
} ST_SIP_INFO_PURPS;

/** @brief Structure of SIP info-param. */
typedef struct tagSIP_INFO_PARM
{
    ZUCHAR ucPurpsPres;              /**< @brief purpose present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_INFO_PURPS stPurps;   /**< @brief purpose info */
        ST_SIP_GEN_PARM stParm;     
    } u;
} ST_SIP_INFO_PARM;

/** @brief List of SIP info-param. */
ABNF_TYPEDEF_LIST(SIP_INFO_PARM); 

/** @brief Structure of SIP info. */
typedef struct tagSIP_INFO
{
    ST_SIP_ABSO_URI stAbsoUri;       /**< @brief absoluteURI */
    ST_SIP_INFO_PARM_LST stParmLst;  /**< @brief info-param list */
} ST_SIP_INFO;

/** @brief List of SIP info. */
ABNF_TYPEDEF_LIST(SIP_INFO); 

/** @brief tag-value */
typedef ST_ZOS_SSTR ST_SIP_TAG_VAL;

/* list of content-coding */
ABNF_TYPEDEF_LIST(SIP_TAG_VAL); 

/* sip feature-param */
typedef struct tagSIP_CFEATURE_PARM
{
    ZUCHAR ucType;                   /* tag type EN_SIP_ENCF_TAG */
    ZUCHAR ucValPres;                /* value present flag */
    ZUCHAR ucStrValPres;             /* string-value present flag */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /* other-tags */
    union
    {
        ST_SIP_TAG_VAL_LST stValLst; /* tag-value-list */
        ST_ZOS_SSTR stStrVal;        /* string-value */
    } u;
} ST_SIP_CFEATURE_PARM;

/** @brief Structure of SIP contact-params. */
typedef struct tagSIP_CONTACT_PARMS
{
    ZUCHAR ucType;                   /**< @brief contact-parms type 
                                        EN_SIP_CONTACT_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_GEN_PARM stParm;          /**< @brief contact-extension */
    union
    {
        ZUINT iExpires;              /**< @brief c-p-expires */
        ZUINT iRegid;                /* c-p-reg */
        ST_SIP_QVAL stQval;          /**< @brief c-p-q */
        ST_SIP_CFEATURE_PARM stFeature; /* feature-param */
        ST_ZOS_SSTR stInstance;      /* c-p-instance */
        ST_ZOS_SSTR stPubGruu;       /* pub-gruu */
        ST_ZOS_SSTR stConfExist;     /* Conf-Exist */
        ST_ZOS_SSTR stTempGruu;      /* temp-gruu */       
    } u;
} ST_SIP_CONTACT_PARMS;

/** @brief List of SIP contact-params. */
ABNF_TYPEDEF_LIST(SIP_CONTACT_PARMS); 

/** @brief Structure of SIP contact-param. */
typedef struct tagSIP_CONTACT_PARM
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
    ST_SIP_CONTACT_PARMS_LST stParmsLst; /**< @brief contact-params list */
} ST_SIP_CONTACT_PARM;

/** @brief List of SIP contact-param. */
ABNF_TYPEDEF_LIST(SIP_CONTACT_PARM); 

/** @brief Structure of SIP handling-param. */
typedef struct tagSIP_HAND_PARM
{
    ZUCHAR ucType;                   /**< @brief type value EN_SIP_HANDLING */  
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other-handling */
} ST_SIP_HAND_PARM;

/** @brief Structure of SIP disp-param. */
typedef struct tagSIP_DISP_PARM
{
    ZUCHAR ucHandlingPres;           /**< @brief handling-param present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_HAND_PARM stHand;     /**< @brief handling-param */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;    
} ST_SIP_DISP_PARM;

/** @brief List of SIP disp-param. */
ABNF_TYPEDEF_LIST(SIP_DISP_PARM); 

/** @brief Structure of SIP language-tag. */
typedef struct tagSIP_LANG_TAG
{
    ST_ZOS_SSTR stPrimaryTag;        /**< @brief primary-tag */
    ST_SIP_SSTR_LST stSubtagLst;     /**< @brief subtag list */
} ST_SIP_LANG_TAG;

/** @brief List of SIP language-tag. */
ABNF_TYPEDEF_LIST(SIP_LANG_TAG); 

/** @brief Structure of SIP error-uri. */
typedef struct tagSIP_ERROR_URI
{
    ST_SIP_ABSO_URI stAbsoUri;       /**< @brief absoluteURI */
    ST_SIP_GEN_PARM_LST stParmLst;   /**< @brief generic-param list */
} ST_SIP_ERROR_URI;

/** @brief List of SIP error-uri. */
ABNF_TYPEDEF_LIST(SIP_ERROR_URI); 

/** @brief Structure of SIP from-param & to-param. */
typedef struct tagSIP_FROM_TO_PARM
{
    ZUCHAR ucTagPres;                /**< @brief tag-param present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stTag;           /**< @brief tag-param */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_FROM_TO_PARM;

/** @brief List of SIP from-param & to-param. */
ABNF_TYPEDEF_LIST(SIP_FROM_TO_PARM); 

/** @brief Structure of SIP segment. */
typedef struct tagSIP_SEGMENT
{
    ST_ZOS_SSTR stPchar;             /**< @brief pchar */
    ST_SIP_SSTR_LST stParmLst;       /**< @brief param list */
} ST_SIP_SEGMENT;

/** @brief List of SIP segment. */
ABNF_TYPEDEF_LIST(SIP_SEGMENT);

/** @brief Structure of SIP path-segments. */
typedef struct tagSIP_PATH_SEGMENTS
{
    ST_SIP_SEGMENT_LST stSegmentLst; /**< @brief segment list */
} ST_SIP_PATH_SEGMENTS;

/** @brief Structure of SIP URI. */
typedef struct tagSIP_URI
{
    ZUCHAR ucAbsoUriPres;            /**< @brief absoluteURI present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_ABSO_URI stAbsoUri;   /**< @brief absoluteURI */
        ST_SIP_PATH_SEGMENTS stAbsPath; /**< @brief abs-path */
    } u;
} ST_SIP_URI;

/** @brief List of SIP URI. */
ABNF_TYPEDEF_LIST(SIP_URI);

/** @brief Structure of SIP domain. */
typedef struct tagSIP_DOMAIN
{
    ZUCHAR ucStarPres;               /**< @brief star presence */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_URI_LST stUriLst;         /**< @brief URI list */
} ST_SIP_DOMAIN;

/** @brief Structure of SIP qop-options. */
typedef struct tagSIP_QOPTS
{
    ST_SIP_TYPE_VAL stQval;          /**< @brief qop-value EN_SIP_QOP_VAL */
    ST_SIP_TYPE_VAL_LST stQvalLst;   /**< @brief qop-value list */
} ST_SIP_QOPTS;

/** @brief Structure of SIP digest-cln. */
typedef struct tagSIP_DIGEST_CLN
{
    ZUCHAR ucType;                   /**< @brief digest-cln type EN_SIP_DIGEST_CLN */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stRealm;         /**< @brief realm */
        ST_SIP_DOMAIN stDomain;      /**< @brief domain */
        ST_ZOS_SSTR stNonce;         /**< @brief nonce */
        ST_ZOS_SSTR stOpaque;        /**< @brief opaque */
        ZUCHAR ucStale;              /**< @brief stale EN_SIP_STALE */
        ST_SIP_ALGO stAlgo;          /**< @brief algorithm */
        ST_SIP_QOPTS stQopts;        /**< @brief qop-options */
        ST_SIP_AUTH_PARM stAuthParm; /**< @brief auth-param */
    } u;
} ST_SIP_DIGEST_CLN;

/** @brief List of SIP digest-cln. */
ABNF_TYPEDEF_LIST(SIP_DIGEST_CLN);

/** @brief Structure of SIP digest. */
typedef struct tagSIP_DIGEST
{
    ST_SIP_DIGEST_CLN_LST stDigestClnLst; /**< @brief digest-cln list */
} ST_SIP_DIGEST;

/** @brief Structure of SIP other-challenge. */
typedef struct tagSIP_OTHER_CHALLENGE
{
    ST_ZOS_SSTR stAuthSchem;         /**< @brief auth-scheme */
    ST_SIP_AUTH_PARM stAuthParm;     /**< @brief auth-param */
    ST_SIP_AUTH_PARM_LST stParmLst;  /**< @brief auth-param list */
} ST_SIP_OTHER_CHALLENGE;

/** @brief Structure of SIP challenge. */
typedef struct tagSIP_CHALLENGE
{
    ZUCHAR ucDigestPres;             /**< @brief digest present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_SIP_DIGEST stDigest;      /**< @brief digest */
        ST_SIP_OTHER_CHALLENGE stOther; /**< @brief other-challenge */
    } u;
} ST_SIP_CHALLENGE;

/** @brief Structure of SIP option-tag. */
typedef struct tagSIP_OPT_TAG
{
    ZUCHAR ucType;                   /**< @brief option-tag type EN_SIP_OPT_TAG */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other option-tag */
} ST_SIP_OPT_TAG;

/** @brief List of SIP option-tag. */
ABNF_TYPEDEF_LIST(SIP_OPT_TAG);

/** @brief Structure of SIP rplyto-param. */
typedef ST_SIP_GEN_PARM ST_SIP_RPLYTO_PARM;

/** @brief List of SIP rplyto-param. */
ABNF_TYPEDEF_LIST(SIP_RPLYTO_PARM);

/** @brief Structure of SIP retry-param. */
typedef struct tagSIP_RETRY_PARM
{
    ZUCHAR ucDurationPres;           /**< @brief duration present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ZUINT iDuration;             /**< @brief delta-seconds */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_RETRY_PARM;

/** @brief List of SIP retry-param. */
ABNF_TYPEDEF_LIST(SIP_RETRY_PARM);

/** @brief Structure of SIP route-param. */
typedef struct tagSIP_ROUTE_PARM
{
    ST_SIP_NAME_ADDR stNameAddr;     /**< @brief name-addr */
    ST_SIP_GEN_PARM_LST stRrParmLst; /**< @brief rr-param */
} ST_SIP_ROUTE_PARM;

/** @brief List of SIP route-param. */
ABNF_TYPEDEF_LIST(SIP_ROUTE_PARM);

/** @brief Structure of SIP product. */
typedef struct tagSIP_PRODUCT
{
    ZUCHAR ucVerPres;                /**< @brief product-version present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stName;              /**< @brief product name */
    ST_ZOS_SSTR stVer;               /**< @brief product-version */
} ST_SIP_PRODUCT;

/** @brief Structure of SIP server-val. */
typedef struct tagSIP_SERVER_VAL
{
    ZUCHAR ucProductPres;            /**< @brief product present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_PRODUCT stProduct;    /**< @brief product */
        ST_ZOS_SSTR stComment;       /**< @brief comment */
    } u;
} ST_SIP_SERVER_VAL;

/** @brief List of SIP server-val. */
ABNF_TYPEDEF_LIST(SIP_SERVER_VAL);

/** @brief Structure of SIP sent-protocol. */
typedef struct tagSIP_SENT_PROTOCOL
{
    ZUCHAR ucTptType;                /**< @brief transport type EN_SIP_TPT */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stName;              /**< @brief protocol-name */
    ST_ZOS_SSTR stVer;               /**< @brief protocol-version */
    ST_ZOS_SSTR stOtherTpt;          /**< @brief other-transport */
} ST_SIP_SENT_PROTOCOL;

/** @brief Structure of SIP sent-by. */
typedef struct tagSIP_SENT_BY
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucPortPres;               /**< @brief Port present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ZUINT iPort;                     /**< @brief port */    
    ST_SIP_HOST stHost;              /**< @brief host */
} ST_SIP_SENT_BY;

/** @brief Structure of SIP via-received. */
typedef struct tagSIP_VIA_RECV
{
    ZUCHAR ucIpv4Pres;               /**< @brief IPv4address present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ZUINT iIpv4Addr;             /**< @brief IPv4address */
        ZUCHAR aucIpv6Addr[ZINET_IPV6_ADDR_SIZE]; /**< @brief IPv6address */
    } u;
} ST_SIP_VIA_RECV;

/** @brief Structure of SIP via-params. */
typedef struct tagSIP_VIA_PARMS
{
    ZUCHAR ucType;                   /**< @brief via-params type EN_SIP_VIA_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ZUINT iTtl;                /**< @brief via-ttl */
        ZUINT iRport;              /**< @brief response-port in rfc3581 */
        ST_SIP_HOST stMaddr;         /**< @brief via-maddr */
        ST_SIP_VIA_RECV stRecv;      /**< @brief via-received */
        ST_ZOS_SSTR stBranch;        /**< @brief via-branch */
        ST_SIP_GEN_PARM stExt;       /**< @brief via-extension */ 
    }u;
} ST_SIP_VIA_PARMS;

/** @brief List of SIP via-params. */
ABNF_TYPEDEF_LIST(SIP_VIA_PARMS);

/** @brief Structure of SIP via-parm. */
typedef struct tagSIP_VIA_PARM
{
    ST_SIP_SENT_PROTOCOL stProtocol; /**< @brief sent-protocol */
    ST_SIP_SENT_BY stBy;             /**< @brief sent-by */
    ST_SIP_VIA_PARMS_LST stParmsLst; /**< @brief via-params list */
} ST_SIP_VIA_PARM;

/** @brief List of SIP via-parm. */
ABNF_TYPEDEF_LIST(SIP_VIA_PARM);

/** @brief Structure of SIP warn-agent. */
typedef struct tagSIP_WARN_AGENT
{
    ZUCHAR ucHostPortPres;           /**< @brief hostport present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_HOST_PORT stHostPort; /**< @brief hostport */
        ST_ZOS_SSTR stPseu;          /**< @brief pseudonym */
    }u;
} ST_SIP_WARN_AGENT;

/** @brief Structure of SIP warning-value. */
typedef struct tagSIP_WARN_VAL
{
    ZUINT iCode;                     /**< @brief warn-code */
    ST_SIP_WARN_AGENT stAgent;       /**< @brief warn-agent */
    ST_ZOS_SSTR stText;              /**< @brief warn-text */
} ST_SIP_WARN_VAL;

/** @brief List of SIP warning-value. */
ABNF_TYPEDEF_LIST(SIP_WARN_VAL);

/** @brief Structure of SIP event package. */
typedef struct tagSIP_EVNT_PKG
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucType;                   /**< @brief package type EN_SIP_EVNT_PKG */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other package */
} ST_SIP_EVNT_PKG;

/** @brief Structure of SIP event-template. */
typedef struct tagSIP_EVNT_TEMP
{
    ZUCHAR ucType;                   /**< @brief template type EN_SIP_EVNT_TEMP */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOtherTemp;         /**< @brief other template */
} ST_SIP_EVNT_TEMP;

/** @brief List of SIP event-template. */
ABNF_TYPEDEF_LIST(SIP_EVNT_TEMP);

/** @brief Structure of SIP event-type. */
typedef struct tagSIP_EVNT_TYPE
{
    ST_SIP_EVNT_PKG stEvntPkg;       /**< @brief event package */
    ST_SIP_EVNT_TEMP_LST stEvntTempLst; /**< @brief event-template */
} ST_SIP_EVNT_TYPE;

/** @brief List of SIP event-type. */
ABNF_TYPEDEF_LIST(SIP_EVNT_TYPE);

/** @brief Structure of SIP profile-value. */
typedef struct tagSIP_PROF_VAL
{
    ZUCHAR ucType;                   /**< @brief value type EN_SIP_PROF_VAL */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other value */
} ST_SIP_PROF_VAL;

/** @brief Structure of SIP diff-processing. */
typedef struct tagSIP_DIFF_PROC
{
    ZUCHAR ucType;                   /**< @brief value type EN_SIP_DIFF_PROC */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other value */
} ST_SIP_DIFF_PROC;

/** @brief Structure of SIP event-param. */
typedef struct tagSIP_EVNT_PARM
{
    ZUCHAR ucType;                   /**< @brief parameter type EN_SIP_EVNT_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stId;            /**< @brief id */
        ST_SIP_PROF_VAL stProfVal;   /**< @brief profile-value */
        ST_ZOS_SSTR stVendor;        /**< @brief vendor */
        ST_ZOS_SSTR stModel;         /**< @brief model */
        ST_ZOS_SSTR stVer;           /**< @brief version */
        ZUINT iEffectBy;             /**< @brief effective-by */
        ST_SIP_ADDR_SPEC stDevId;    /**< @brief device-id */
        ST_SIP_ADDR_SPEC stNetUser;  /**< @brief network-user */
        ST_ZOS_SSTR stDoc;           /**< @brief document */
        ST_ZOS_SSTR stAuid;          /**< @brief auid */
        ST_SIP_DIFF_PROC stDiffProc; /**< @brief diff-processing */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_EVNT_PARM;

/** @brief List of SIP event-param. */
ABNF_TYPEDEF_LIST(SIP_EVNT_PARM);

/** @brief Structure of SIP substate-value. */
typedef struct tagSIP_SUBSTA_VAL
{
    ZUCHAR ucType;                   /**< @brief substate-value type EN_SIP_SUBSTA_VAL */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /**< @brief extension-substate */
} ST_SIP_SUBSTA_VAL;

/** @brief Structure of SIP event-reason-value. */
typedef struct tagSIP_EVNT_REASON_VAL
{
    ZUCHAR ucType;                   /**< @brief reason type EN_SIP_EVNT_REASON_VAL */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /**< @brief reason-extension */
} ST_SIP_EVNT_REASON_VAL;

/** @brief Structure of SIP subexp-params. */
typedef struct tagSIP_SUBEXP_PARMS
{
    ZUCHAR ucType;                   /**< @brief parameter type EN_SIP_SUBEXP_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_EVNT_REASON_VAL stReasonVal; /**< @brief event-reason-value */
        ZUINT iExpires;              /**< @brief expires delta-seconds */
        ZUINT iRetryAfter;           /**< @brief retry-after  delta-seconds */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_SUBEXP_PARMS;

/** @brief List of SIP subexp-params. */
ABNF_TYPEDEF_LIST(SIP_SUBEXP_PARMS);

/** @brief Structure of SIP replaces-param. */
typedef struct tagSIP_REPLACES_PARM
{
    ZUCHAR ucType;                   /**< @brief param type EN_SIP_REPLACES_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stToTag;         /**< @brief to-tag */
        ST_ZOS_SSTR stFromTag;       /**< @brief from-tag */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_REPLACES_PARM;

/** @brief List of SIP replaces-param. */
ABNF_TYPEDEF_LIST(SIP_REPLACES_PARM);

/** @brief Structure of SIP directive. */
typedef struct tagSIP_DIRECT
{
    ZUCHAR ucType;                   /**< @brief directive type EN_SIP_DIRECT */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
} ST_SIP_DIRECT;

/** @brief List of SIP directive. */
ABNF_TYPEDEF_LIST(SIP_DIRECT);

/** @brief Structure of SIP feature-value. */
typedef struct tagSIP_FEATURE_VAL
{
    ZUCHAR ucType;                   /**< @brief feature-param type EN_SIP_FEATURE_VAL */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other-value */
} ST_SIP_FEATURE_VAL;

/** @brief List of SIP feature-value. */
ABNF_TYPEDEF_LIST(SIP_FEATURE_VAL);

/** @brief Structure of SIP feature-param. */
typedef struct tagSIP_FEATURE_PARM
{
    ZUCHAR ucNotPres;                /**< @brief not present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_FEATURE_VAL_LST stValLst; /**< @brief feature-value list */
} ST_SIP_FEATURE_PARM;

/** @brief Structure of SIP ac-params. */
typedef struct tagSIP_AC_PARMS
{
    ZUCHAR ucType;                   /**< @brief ac-params type EN_SIP_AC_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_FEATURE_PARM stFeature;   /**< @brief feature-param */
    ST_SIP_GEN_PARM stParm;          /**< @brief generic-param */
    ST_ZOS_SSTR stInstance;          /**< @brief +sip.instance-param */
} ST_SIP_AC_PARMS;

/** @brief List of SIP ac-params. */
ABNF_TYPEDEF_LIST(SIP_AC_PARMS);

/** @brief Structure of SIP ac-value. */
typedef struct tagSIP_AC_VAL
{
    ST_SIP_AC_PARMS_LST stParmsLst;  /**< @brief ac-params list */
} ST_SIP_AC_VAL;

/** @brief List of SIP ac-value. */
ABNF_TYPEDEF_LIST(SIP_AC_VAL);

/** @brief Structure of SIP rc-params. */
typedef struct tagSIP_RC_PARMS
{
    ZUCHAR ucFeaturePres;            /**< @brief feature-param present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_FEATURE_PARM stFeature; /**< @brief feature-param */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_RC_PARMS;

/** @brief List of SIP rc-params. */
ABNF_TYPEDEF_LIST(SIP_RC_PARMS);

/** @brief Structure of SIP rc-value. */
typedef struct tagSIP_RC_VAL
{
    ST_SIP_RC_PARMS_LST stParmsLst;  /**< @brief rc-params list */    
} ST_SIP_RC_VAL;

/** @brief List of SIP rc-value. */
ABNF_TYPEDEF_LIST(SIP_RC_VAL);

/** @brief Structure of SIP privacy-tag. */
typedef struct tagSIP_PRIVACY_TAG
{
    ZUCHAR ucType;                   /**< @brief privacy-tag type EN_SIP_PRIVACY_TAG */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief token */
} ST_SIP_PRIVACY_TAG;

/** @brief List of SIP privacy-tag. */
ABNF_TYPEDEF_LIST(SIP_PRIVACY_TAG);

/** @brief Structure of SIP rpi-priv-element. */
typedef struct tagSIP_RPI_PRIV_ELEM
{
    ZUCHAR ucNetPres;                /**< @brief network present flag */
    ZUCHAR ucElemType;               /**< @brief rpi-priv-element type 
                                        EN_SIP_RPI_PRIV_ELEM */
    ZUCHAR ucPolicyType;             /**< @brief network type 
                                        EN_SIP_RPI_PRIV_ELEM_POLICY */ 
    ZUCHAR aucSpare[1];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOtherElem;         /**< @brief other element token */
    ST_ZOS_SSTR stOtherPolicy;       /**< @brief other policy token */
} ST_SIP_RPI_PRIV_ELEM;

/** @brief List of SIP rpi-priv-element. */
ABNF_TYPEDEF_LIST(SIP_RPI_PRIV_ELEM);

/** @brief Structure of SIP rpi-privacy. */
typedef struct tagSIP_RPI_PRIVACY
{
    ST_SIP_RPI_PRIV_ELEM stElm;      /**< @brief rpi-priv-element */
    ST_SIP_RPI_PRIV_ELEM_LST stElmLst; /**< @brief rpi-priv-element list */
} ST_SIP_RPI_PRIVACY;

/** @brief Structure of SIP rpid-privacy-token. */
typedef struct tagSIP_RPID_PRIV_TKN
{
    ZUCHAR ucType;                   /**< @brief rpid-privacy-token type 
                                        EN_SIP_RPID_PRIV_TKN */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_TYPE_VAL stPtyType;   /**< @brief rpi-pty-type EN_SIP_RPI_PTY_TYPE */
        ST_SIP_TYPE_VAL stIdType;    /**< @brief rpi-id-type EN_SIP_RPI_ID_TYPE */
        ST_SIP_RPI_PRIVACY stPrivacy; /**< @brief rpi-privacy */
    } u;
} ST_SIP_RPID_PRIV_TKN;

/** @brief List of SIP rpid-privacy-token. */
ABNF_TYPEDEF_LIST(SIP_RPID_PRIV_TKN);

/** @brief Structure of SIP rpid-priv. */
typedef struct tagSIP_RPID_PRIV
{
    ST_SIP_RPID_PRIV_TKN stTkn;      /**< @brief rpid-privacy-token */
    ST_SIP_RPID_PRIV_TKN_LST stTknLst; /**< @brief rpid-privacy-token list */
} ST_SIP_RPID_PRIV;

/** @brief List of SIP rpid-priv. */
ABNF_TYPEDEF_LIST(SIP_RPID_PRIV);

/** @brief Structure of SIP other-rpi-token. */
typedef struct tagSIP_OTHER_RPI_TKN
{
    ZUCHAR ucDashPres;               /**< @brief dash present flag */
    ZUCHAR ucValPres;                /**< @brief token value present flag */
    ZUCHAR ucValQPres;               /**< @brief token value quoted present flag */
    ZUCHAR aucSpare[1];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stName;              /**< @brief token name */
    ST_ZOS_SSTR stVal;               /**< @brief token value */
} ST_SIP_OTHER_RPI_TKN;

/** @brief Structure of SIP rpi-token. */
typedef struct tagSIP_RPI_TKN
{
    ZUCHAR ucType;                   /**< @brief rpid-token type EN_SIP_RPI_TKN */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ZUCHAR ucScreen;             /**< @brief rpi-screen EN_SIP_RPI_SCREEN */
        ST_SIP_TYPE_VAL stPtyType;   /**< @brief rpi-pty-type EN_SIP_RPI_PTY_TYPE */
        ST_SIP_TYPE_VAL stIdType;    /**< @brief rpi-id-type EN_SIP_RPI_ID_TYPE */
        ST_SIP_RPI_PRIVACY stPrivacy; /**< @brief rpi-privacy */
        ST_SIP_OTHER_RPI_TKN stOther; /**< @brief other-rpi-token */
    } u;
} ST_SIP_RPI_TKN;

/** @brief List of SIP rpi-token. */
ABNF_TYPEDEF_LIST(SIP_RPI_TKN);

/** @brief Structure of SIP rpid. */
typedef struct tagSIP_RPID
{
    ZUCHAR ucDispNamePres;           /**< @brief display-name present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_DISP_NAME stDispName;     /**< @brief display-name */
    ST_SIP_ADDR_SPEC stAddrSpec;     /**< @brief addr-spec */
    ST_SIP_RPI_TKN_LST stTknLst;     /**< @brief rpi-token list */
} ST_SIP_RPID;

/** @brief List of SIP rpid. */
ABNF_TYPEDEF_LIST(SIP_RPID);

/** @brief Structure of SIP join-param. */
typedef struct tagSIP_JOIN_PARM
{
    ZUCHAR ucType;                   /**< @brief join-param type EN_SIP_JOIN_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stToTag;         /**< @brief to-tag */
        ST_ZOS_SSTR stFromTag;       /**< @brief from-tag */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_JOIN_PARM;

/** @brief List of SIP join-param. */
ABNF_TYPEDEF_LIST(SIP_JOIN_PARM);

/** @brief Structure of SIP p-aso-uri-spec. */
typedef struct tagSIP_P_ASO_URI_SPEC
{
    ST_SIP_NAME_ADDR stNameAddr;     /**< @brief name-addr */
    ST_SIP_GEN_PARM_LST stAiParmLst; /**< @brief ai-param list */
} ST_SIP_P_ASO_URI_SPEC;

/** @brief List of SIP p-aso-uri-spec. */
ABNF_TYPEDEF_LIST(SIP_P_ASO_URI_SPEC);

/** @brief Structure of SIP called-pty-id-spec. */
typedef struct tagSIP_CALLED_PTY_ID_SPEC
{
    ST_SIP_NAME_ADDR stNameAddr;     /**< @brief name-addr */
    ST_SIP_GEN_PARM_LST stCpidParmLst; /**< @brief cpid-param list */
}ST_SIP_CALLED_PTY_ID_SPEC;

/** @brief Structure of SIP p-vnet-id-spec. */
typedef struct tagSIP_P_VNET_ID_SPEC
{
    ZUCHAR ucQStrPres;               /**< @brief quoted-string present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stSpecDesc;          /**< @brief token / quoted-string */    
    ST_SIP_GEN_PARM_LST stVnetParmLst; /**< @brief vnetwork-param list */
} ST_SIP_P_VNET_ID_SPEC;

/** @brief List of SIP p-vnet-id-spec. */
ABNF_TYPEDEF_LIST(SIP_P_VNET_ID_SPEC);

/** @brief Structure of SIP access-info. */
typedef struct tagSIP_ACCESS_INFO
{
    ZUCHAR ucType;                   /**< @brief info type EN_SIP_ACCESS_INFO */
    ZUCHAR ucQStrPres;               /**< @brief quoted-string present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stStr;           /**< @brief string */  
        ST_SIP_GEN_VAL stExt;        /**< @brief extension-access-info */
    } u;
} ST_SIP_ACCESS_INFO;

/** @brief List of SIP access-info. */
ABNF_TYPEDEF_LIST(SIP_ACCESS_INFO);

/** @brief Structure of SIP access-net-spec. */
typedef struct tagSIP_P_ACCESS_NET_SPEC
{
    ST_SIP_TYPE_VAL stType;          /**< @brief access-type EN_SIP_ACCESS_TYPE */        
    ST_SIP_ACCESS_INFO_LST stInfoLst; /**< @brief access-info list */
} ST_SIP_P_ACCESS_NET_SPEC;

/** @brief Structure of SIP charge-addr-params. */
typedef struct tagSIP_CHARGE_ADDR_PARMS
{
    ZUCHAR ucType;                   /**< @brief charge-addr-params type 
                                        EN_SIP_CHARGE_ADDR_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_GEN_VAL stCcf;        /**< @brief ccf */
        ST_SIP_GEN_VAL stEcf;        /**< @brief ecf */
        ST_SIP_GEN_PARM stGen;       /**< @brief generic-param */
    } u;
} ST_SIP_CHARGE_ADDR_PARMS;

/** @brief List of SIP charge-addr-params. */
ABNF_TYPEDEF_LIST(SIP_CHARGE_ADDR_PARMS);

/** @brief Structure of SIP charge-params. */
typedef struct tagSIP_CHARGE_PARMS
{
    ZUCHAR ucType;                   /**< @brief charge-params type 
                                        EN_SIP_CHARGE_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_HOST stIcidGen;       /**< @brief icid-gen-addr */
        ST_SIP_GEN_VAL stOrigIoi;    /**< @brief orig-ioi */
        ST_SIP_GEN_VAL stTermIoi;    /**< @brief term-ioi */
        ST_SIP_GEN_PARM stGen;       /**< @brief generic-param */
    } u;
} ST_SIP_CHARGE_PARMS;

/** @brief List of SIP charge-params. */
ABNF_TYPEDEF_LIST(SIP_CHARGE_PARMS);

/** @brief Structure of SIP PAssertedID-value. */
typedef struct tagSIP_PASSERTEDID_VAL
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
} ST_SIP_PASSERTEDID_VAL;

/** @brief List of SIP PAssertedID-value. */
ABNF_TYPEDEF_LIST(SIP_PASSERTEDID_VAL);

/** @brief Structure of SIP PPreferredID-value. */
typedef struct tagSIP_PPREFERREDID_VAL
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
} ST_SIP_PPREFERREDID_VAL;

/** @brief List of SIP PPreferredID-value. */
ABNF_TYPEDEF_LIST(SIP_PPREFERREDID_VAL);

/** @brief Structure of SIP P-Media-Authorization-Token. */
typedef struct tagSIP_P_MEDIA_AUTHOR_TKN
{
    ST_ZOS_SSTR stTkn;               /**< @brief token */
} ST_SIP_P_MEDIA_AUTHOR_TKN;

/** @brief List of SIP P-Media-Authorization-Token. */
ABNF_TYPEDEF_LIST(SIP_P_MEDIA_AUTHOR_TKN);

/** @brief Structure of SIP priv-value. */
typedef struct tagSIP_PRIV_VAL
{
    ZUCHAR ucType;                   /**< @brief priv-value type EN_SIP_PRIV_VAL */ 
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stTkn;               /**< @brief token */
} ST_SIP_PRIV_VAL;

/** @brief List of SIP priv-value. */
ABNF_TYPEDEF_LIST(SIP_PRIV_VAL);

/** @brief Structure of SIP reason-params. */
typedef struct tagSIP_REASON_PARMS
{
    ZUCHAR ucType;                   /**< @brief reason-params type 
                                        EN_SIP_REASON_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ZUINT iCause;                /**< @brief protocol-cause */
        ST_ZOS_SSTR stTxt;           /**< @brief reason-text */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_REASON_PARMS;

/** @brief List of SIP reason-params. */
ABNF_TYPEDEF_LIST(SIP_REASON_PARMS);

/** @brief Structure of SIP reason-value. */
typedef struct tagSIP_REASON_VAL
{
    ZUCHAR ucType;                   /**< @brief protocol type EN_SIP_PROTOCOL */ 
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stProtocol;          /**< @brief protocol token */
    ST_SIP_REASON_PARMS_LST stParmsLst;  /**< @brief reason-params list */
} ST_SIP_REASON_VAL;

/** @brief List of SIP reason-value. */
ABNF_TYPEDEF_LIST(SIP_REASON_VAL);

/** @brief Structure of SIP mech-parameters. */
typedef struct tagSIP_MECH_PARMS
{
    ZUCHAR ucType;                   /**< @brief mech-parameters type 
                                        EN_SIP_MECH_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_QVAL stPref;          /**< @brief preference */
        ST_ZOS_SSTR stAlgo;          /**< @brief digest-algorithm/algorithm/encrypt-algorithm */
        ST_ZOS_SSTR stQop;           /**< @brief digest-qop */
        ST_ZOS_SSTR stVerify;        /**< @brief digest-verify */
        ST_ZOS_SSTR stProto;         /**< @brief protocol */
        ST_ZOS_SSTR stMode;          /**< @brief mode */
        ZUINT iSpi;                /**< @brief spi/spi-c/spi-s */
        ZUINT iPort;               /**< @brief port1/port2/port-c/port-s */
        ST_SIP_GEN_PARM stParm;      /**< @brief extension = generic-param */
    } u;
} ST_SIP_MECH_PARMS;

/** @brief List of SIP mech-parameters. */
ABNF_TYPEDEF_LIST(SIP_MECH_PARMS);

/** @brief Structure of SIP sec-mechanism. */
typedef struct tagSIP_SEC_MECH
{
    ZUCHAR ucNameType;               /**< @brief mechanism-name type EN_SIP_MECH_NAME */ 
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stName;              /**< @brief mechanism-name token */
    ST_SIP_MECH_PARMS_LST stParmsLst; /**< @brief mech-parameters list */
} ST_SIP_SEC_MECH;

/** @brief List of SIP sec-mechanism. */
ABNF_TYPEDEF_LIST(SIP_SEC_MECH);

/** @brief Structure of SIP FEID. */
typedef struct tagSIP_FEID
{
    ST_ZOS_SSTR stFinId;             /**< @brief financial identifier */
    ST_SIP_HOST stHost;              /**< @brief domain name */
} ST_SIP_FEID; 

/** @brief Structure of SIP Billing-Info-param. */
typedef struct tagSIP_BILL_INFO_PARM
{
    ZUCHAR ucType;                   /**< @brief Billing-Info-param type 
                                        EN_SIP_BILL_INFO_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stRksGrpId;      /**< @brief RKS-Group-ID-param */
        ST_SIP_ADDR_SPEC stCharge;   /**< @brief Charge-param */
        ST_SIP_ADDR_SPEC stCalling;  /**< @brief Calling-param */
        ST_SIP_ADDR_SPEC stCalled;   /**< @brief Called-param */ 
        ST_SIP_ADDR_SPEC stRouting;  /**< @brief Routing-param */
        ST_SIP_ADDR_SPEC stLocRouting; /**< @brief Loc-Routing-param */
        ST_SIP_GEN_PARM stGenParm;   /**< @brief generic-param */
    } u;
} ST_SIP_BILL_INFO_PARM;

/** @brief List of SIP Billing-Info-param. */
ABNF_TYPEDEF_LIST(SIP_BILL_INFO_PARM);

/** @brief Structure of SIP Laes-param. */
typedef struct tagSIP_LAES_PARM
{
    ZUCHAR ucType;                   /**< @brief Laes-param type EN_SIP_LAES_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_HOST_PORT stContent;  /**< @brief Laes-content */
        ST_ZOS_SSTR stKey;           /**< @brief Laes-key */
        ST_SIP_GEN_PARM stGenParm;   /**< @brief generic-param */
    } u;
} ST_SIP_LAES_PARM;

/** @brief List of SIP Laes-param. */
ABNF_TYPEDEF_LIST(SIP_LAES_PARM);

/** @brief Structure of SIP redir-params. */
typedef struct tagSIP_REDIR_PARMS
{
    ZUCHAR ucType;                   /**< @brief redir-params type EN_SIP_REDIR_PARMS */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_SIP_ADDR_SPEC stUri;      /**< @brief redir-uri-param */
        ST_ZOS_SSTR stCount;         /**< @brief redir-count-param */
        ST_SIP_GEN_PARM stGenParm;   /**< @brief generic-param */
    } u;
} ST_SIP_REDIR_PARMS;

/** @brief List of SIP redir-params. */
ABNF_TYPEDEF_LIST(SIP_REDIR_PARMS);

/** @brief Structure of SIP hi-param. */
typedef struct tagSIP_HI_PARM
{
    ZUCHAR ucIndexPres;              /**< @brief hi-index present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stIndex;         /**< @brief hi-index */
        ST_SIP_GEN_PARM stExt;       /**< @brief hi-extension */
    } u;
} ST_SIP_HI_PARM;

/** @brief List of SIP hi-param. */
ABNF_TYPEDEF_LIST(SIP_HI_PARM);

/** @brief Structure of SIP hi-entry. */
typedef struct tagSIP_HI_ENTRY
{
    ST_SIP_NAME_ADDR stNameAddr;     /**< @brief hi-targeted-to-uri */
    ST_SIP_HI_PARM_LST stParmLst;    /**< @brief hi-param list */
} ST_SIP_HI_ENTRY;

/** @brief List of SIP hi-entry. */
ABNF_TYPEDEF_LIST(SIP_HI_ENTRY);

/** @brief Structure of SIP DiameterURI. */
typedef struct tagSIP_DIAM_URI
{
    ZUCHAR ucAaasPres;               /**< @brief aaas present flag */
    ZUCHAR ucPortPres;               /**< @brief port present flag */
    ZUCHAR ucTptPres;                /**< @brief transport present flag */
    ZUCHAR ucProtoPres;              /**< @brief protocol present flag */
    ZUCHAR ucTptType;                /**< @brief transport type EN_SIP_DTPT */
    ZUCHAR ucProtoType;              /**< @brief protocol type EN_SIP_DPROTO */ 
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stFqdn;              /**< @brief FQDN */
    ZUINT iPort;                   /**< @brief port */
} ST_SIP_DIAM_URI;

/** @brief Structure of SIP answer-mode-value. */
typedef struct tagSIP_ANSWER_MODE_VAL
{
    ZUCHAR ucType;                   /**< @brief value type EN_SIP_ANSWER_MODE_VAL */ 
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stTkn;               /**< @brief token */
} ST_SIP_ANSWER_MODE_VAL;

/** @brief Structure of SIP answer-mode-param. */
typedef struct tagSIP_ANSWER_MODE_PARM
{
    ZUCHAR ucType;                   /**< @brief param type EN_SIP_ANSWER_MODE_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_GEN_PARM stGenParm;       /**< @brief generic-param */
} ST_SIP_ANSWER_MODE_PARM;

/** @brief List of SIP answer-mode-param. */
ABNF_TYPEDEF_LIST(SIP_ANSWER_MODE_PARM);

/** @brief Structure of SIP Service-ID. */
typedef struct tagSIP_SRV_ID
{
    ST_ZOS_SSTR stUrnSrvId;          /**< @brief urn-service-id */
} ST_SIP_SRV_ID;

/** @brief List of SIP Service-ID. */
ABNF_TYPEDEF_LIST(SIP_SRV_ID);

/** @brief sip early-media parm */
typedef struct tagSIP_EM_PARM
{
    ZUCHAR ucType;                   /**< @brief type EN_SIP_P_EARLY_MEDIA_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bits alignment */
    ST_ZOS_SSTR stTkn;               /**< @brief other parameter */
} ST_SIP_EM_PARM;

/** @brief list of early-media parm */
ABNF_TYPEDEF_LIST(SIP_EM_PARM);

/** @brief sip target-dialog parm */
typedef struct tagSIP_TD_PARM
{
    ZUCHAR ucType;                   /**< @brief type EN_SIP_TD_PARM */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bits alignment */
    union
    {
        ST_ZOS_SSTR stRmtTkn;        /**< @brief remote-parm */
        ST_ZOS_SSTR stLclTkn;        /**< @brief local-parm */
        ST_SIP_GEN_PARM stGen;       /**< @brief generic-parm */
    } u;
} ST_SIP_TD_PARM;

/* list of early-media parm */
ABNF_TYPEDEF_LIST(SIP_TD_PARM);

/** @brief Structure of SIP refused-param. */
typedef struct tagSIP_SIP_REFUSED_PARM
{
    ZUCHAR ucMemberPres;             /**< @brief members-param present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stMember;        /**< @brief members-param */
        ST_SIP_GEN_PARM stParm;      /**< @brief generic-param */
    } u;
} ST_SIP_REFUSED_PARM;

/** @brief List of SIP refused-param. */
ABNF_TYPEDEF_LIST(SIP_REFUSED_PARM); 

/** @brief sip uri-list-entry */
typedef struct tagSIP_URI_ENTRY
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
    ST_SIP_REFUSED_PARM_LST stRefusedParmLst; /**< @brief refused-param list */
} ST_SIP_URI_ENTRY;

/** @brief list of uri-list-entry */
ABNF_TYPEDEF_LIST(SIP_URI_ENTRY);

/** @brief Structure of SIP ServInfo-params. */
typedef struct tagSIP_SERVINFO_PARMS
{
    ZUCHAR ucType;                  /**< @brief ServInfo-parms type 
                                                                    EN_SIP_SERVINFO_PARMS */
    ZUCHAR aucSpare[3];             /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stServType;     /* server-type(P2P or Excom) @ref EN_SIP_ST_TYPE */
        ST_ZOS_SSTR stSubServType;  /* sub-server-type @ref EN_SIP_SUB_ST_TYPE */    
    } u;
} ST_SIP_SERVINFO_PARMS;

/** @brief List of SIP ServInfo-params. */
ABNF_TYPEDEF_LIST(SIP_SERVINFO_PARMS); 

/** @brief Structure of SIP ServInfo-param. */
typedef struct tagSIP_SERVINFO_PARM
{
    ST_SIP_SERVINFO_PARMS_LST stParmsLst; /**< @brief ServInfo-params list */
} ST_SIP_SERVINFO_PARM;

/** @brief List of SIP ServInfo-param. */
ABNF_TYPEDEF_LIST(SIP_SERVINFO_PARM); 

/***************************** rfc3261 headers *************************/
/** @brief Structure of SIP Accept. */
typedef struct tagSIP_HDR_ACPT
{
    ST_SIP_ACPT_RANGE_LST stRangeLst; /**< @brief accept range list */
} ST_SIP_HDR_ACPT;

/** @brief Structure of SIP Accept-Encoding. */
typedef struct tagSIP_HDR_ACPT_ENCODING
{
    ST_SIP_ENCODING_LST stEncodingLst; /**< @brief encoding list */
} ST_SIP_HDR_ACPT_ENCODING;

/** @brief Structure of SIP Accept-Language. */
typedef struct tagSIP_HDR_ACPT_LANG
{
    ST_SIP_LANG_LST stLangLst;       /**< @brief language list */
} ST_SIP_HDR_ACPT_LANG;

/** @brief Structure of SIP Alert-Info. */
typedef struct tagSIP_HDR_ALERT_INFO
{
    ST_SIP_ALERT_PARM_LST stAlertLst; /**< @brief alert-param list */
} ST_SIP_HDR_ALERT_INFO;

/** @brief Structure of SIP Allow. */
typedef struct tagSIP_HDR_ALLOW
{
    ST_SIP_METHOD_LST stMethodLst;   /**< @brief Method list */
} ST_SIP_HDR_ALLOW;

/** @brief Structure of SIP Authentication-Info. */
typedef struct tagSIP_HDR_AUTHEN_INFO
{
    ST_SIP_AINFO_LST stAinfoLst;     /**< @brief ainfo list */
} ST_SIP_HDR_AUTHEN_INFO;

/** @brief Structure of SIP Authorization. */
typedef struct tagSIP_HDR_AUTHOR
{
    ST_SIP_CREDENTS stCredents;      /**< @brief credentials */
} ST_SIP_HDR_AUTHOR;

/** @brief Structure of SIP Call-ID. */
typedef struct tagSIP_HDR_CALL_ID
{
    ST_SIP_CALLID stCallid;          /**< @brief callid */
} ST_SIP_HDR_CALL_ID;

/** @brief Structure of SIP Call-Info. */
typedef struct tagSIP_HDR_CALL_INFO
{
    ST_SIP_INFO_LST stInfoLst;       /**< @brief info list */
} ST_SIP_HDR_CALL_INFO;

/** @brief Structure of SIP Contact. */
typedef struct tagSIP_HDR_CONTACT
{
    ZUCHAR ucAllPres;                /**< @brief All(*) for contact param */ 
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_CONTACT_PARM_LST stParmLst; /**< @brief contact-param list */
} ST_SIP_HDR_CONTACT;

/** @brief Structure of SIP Content-Disposition. */
typedef struct tagSIP_HDR_CONTENT_DISP
{
    ZUCHAR ucDispType;               /**< @brief disp-type type EN_SIP_DISP_TYPE */ 
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stDispExt;           /**< @brief disp-extension-token */
    ST_SIP_DISP_PARM_LST stParmLst;  /**< @brief disp-param list */
} ST_SIP_HDR_CONTENT_DISP;

/** @brief Structure of SIP Content-Encoding. */
typedef struct tagSIP_HDR_CONTENT_ENCODING
{
    ST_SIP_CONTENT_CODING_LST stCodingLst; /**< @brief content-coding list */
} ST_SIP_HDR_CONTENT_ENCODING;

/** @brief Structure of SIP Content-Language. */
typedef struct tagSIP_HDR_CONTENT_LANG
{
    ST_SIP_LANG_TAG_LST stLangTagLst; /**< @brief language-tag list */
} ST_SIP_HDR_CONTENT_LANG;

/** @brief Structure of SIP Content-Type. */
typedef struct tagSIP_HDR_CONTENT_TYPE
{
    ST_SIP_MEDIA_TYPE stMediaType;   /**< @brief media-type */
} ST_SIP_HDR_CONTENT_TYPE;

/** @brief Structure of SIP CSeq. */
typedef struct tagSIP_HDR_CSEQ
{
    ZUINT iCseqVal;                /**< @brief CSeq value */
    ST_SIP_METHOD stMethod;          /**< @brief Method */
} ST_SIP_HDR_CSEQ;

/** @brief Structure of SIP Date. */
typedef struct tagSIP_HDR_DATE
{
    ST_SIP_DATE stDate;              /**< @brief SIP-date */
} ST_SIP_HDR_DATE;

/** @brief Structure of SIP Error-Info. */
typedef struct tagSIP_HDR_ERROR_INFO
{
    ST_SIP_ERROR_URI_LST stErrorUriLst; /**< @brief error-uri list */
} ST_SIP_HDR_ERROR_INFO;

/** @brief Structure of SIP In-Reply-To. */
typedef struct tagSIP_HDR_IN_REPLY_TO
{
    ST_SIP_CALLID_LST stCallidLst;   /**< @brief callid list */
} ST_SIP_HDR_IN_REPLY_TO;

/** @brief Structure of SIP From or To. */
typedef struct tagSIP_HDR_FROM_TO
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
    ST_SIP_FROM_TO_PARM_LST stParmLst; /**< @brief from-param list */
} ST_SIP_HDR_FROM_TO;

/** @brief Structure of SIP MIME-Version. */
typedef struct tagSIP_HDR_MIME_VER
{
    ZUINT iMajor;                  /**< @brief major version */
    ZUINT iMinor;                  /**< @brief minor version */
} ST_SIP_HDR_MIME_VER;

/** @brief Structure of SIP Organization. */
typedef struct tagSIP_HDR_ORGANIZATION
{
    ZUCHAR ucOrganizationPres;       /**< @brief organization resent flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOrganization;      /**< @brief organization */
} ST_SIP_HDR_ORGANIZATION;

/** @brief Structure of SIP Priority. */
typedef struct tagSIP_HDR_PRIORITY
{
    ZUCHAR ucType;                   /**< @brief priority type EN_SIP_PRIORITY_VAL */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stOther;             /**< @brief other-priority */
} ST_SIP_HDR_PRIORITY;

/** @brief Structure of SIP Proxy-Authenticate. */
typedef struct tagSIP_HDR_PROXY_AUTHEN
{
    ST_SIP_CHALLENGE stChallenge;    /**< @brief challenge */
} ST_SIP_HDR_PROXY_AUTHEN;

/** @brief Structure of SIP Proxy-Authorization. */
typedef struct tagSIP_HDR_HDR_PROXY_AUTHOR
{
    ST_SIP_CREDENTS stCredents;      /**< @brief credentials */
} ST_SIP_HDR_PROXY_AUTHOR;

/** @brief Structure of SIP Proxy-Require. */
typedef struct tagSIP_HDR_PROXY_REQUIRE
{
    ST_SIP_OPT_TAG_LST stOptTagLst;  /**< @brief option-tag list */
} ST_SIP_HDR_PROXY_REQUIRE;

/** @brief Structure of SIP Reply-To. */
typedef struct tagSIP_HDR_REPLY_TO
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
    ST_SIP_RPLYTO_PARM_LST stParmLst; /**< @brief rplyto-param list */
} ST_SIP_HDR_REPLY_TO;

/** @brief Structure of SIP Require. */
typedef struct tagSIP_HDR_REQUIRE
{
    ST_SIP_OPT_TAG_LST stOptTagLst;  /**< @brief option-tag list */
} ST_SIP_HDR_REQUIRE;

/** @brief Structure of SIP Retry-After. */
typedef struct tagSIP_HDR_RETRY_AFTER
{
    ZUCHAR ucCommentPres;            /**< @brief comment present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ZUINT iTime;                   /**< @brief delta-seconds */
    ST_ZOS_SSTR stComment;           /**< @brief comment */
    ST_SIP_RETRY_PARM_LST stParmLst; /**< @brief retry-param list */
} ST_SIP_HDR_RETRY_AFTER;

/** @brief Structure of SIP Route. */
typedef struct tagSIP_HDR_ROUTE
{
    ST_SIP_ROUTE_PARM_LST stParmLst; /**< @brief route-param list */
} ST_SIP_HDR_ROUTE;

/** @brief List of SIP Route header. */
ABNF_TYPEDEF_LIST(SIP_HDR_ROUTE); 

/** @brief Structure of SIP Server. */
typedef struct tagSIP_HDR_SERVER
{
    ST_SIP_SERVER_VAL_LST stValLst;  /**< @brief server-val list */
} ST_SIP_HDR_SERVER;

/** @brief Structure of SIP Subject. */
typedef struct tagSIP_HDR_SUBJECT
{
    ZUCHAR ucDescPres;               /**< @brief description present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stDesc;              /**< @brief subject description */
} ST_SIP_HDR_SUBJECT;

/** @brief Structure of SIP Supported. */
typedef struct tagSIP_HDR_SUPTED
{
    ST_SIP_OPT_TAG_LST stOptTagLst;  /**< @brief option-tag list */
} ST_SIP_HDR_SUPTED;

/** @brief Structure of SIP Timestamp. */
typedef struct tagSIP_HDR_TIMESTAMP
{
    ZUCHAR ucFracPres;               /**< @brief fraction present flag */
    ZUCHAR ucDelayPres;              /**< @brief delay present flag */
    ZUCHAR ucDelayFracPres;          /**< @brief delay fraction present flag */
    ZUCHAR aucSpare[1];              /**< @brief for 32 bit alignment */
    ZUINT iVal;                    /**< @brief value */
    ZUINT iFrac;                   /**< @brief fraction */
    ZUINT iDelay;                  /**< @brief delay */
    ZUINT iDelayFrac;              /**< @brief delay fraction */
} ST_SIP_HDR_TIMESTAMP;

/** @brief Structure of SIP Unsupported. */
typedef struct tagSIP_HDR_UNSUPTED
{
    ST_SIP_OPT_TAG_LST stOptTagLst;  /**< @brief option-tag list */
} ST_SIP_HDR_UNSUPTED;

/** @brief Structure of SIP User-Agent. */
typedef struct tagSIP_HDR_USER_AGENT
{
    ST_SIP_SERVER_VAL_LST stValLst;  /**< @brief server-val list */
} ST_SIP_HDR_USER_AGENT;

/** @brief Structure of SIP Via. */
typedef struct tagSIP_HDR_VIA
{
    ST_SIP_VIA_PARM_LST stParmLst;   /**< @brief via-parm list */
} ST_SIP_HDR_VIA;

/** @brief List of SIP Via header. */
ABNF_TYPEDEF_LIST(SIP_HDR_VIA); 

/** @brief Structure of SIP Warning. */
typedef struct tagSIP_HDR_WARN
{
    ST_SIP_WARN_VAL_LST stValLst;    /**< @brief warning-value list */
} ST_SIP_HDR_WARN;

/** @brief Structure of SIP WWW-Authenticate. */
typedef struct tagSIP_HDR_WWW_AUTHEN
{
    ST_SIP_CHALLENGE stChallenge;    /**< @brief challenge */
} ST_SIP_HDR_WWW_AUTHEN;

/** @brief Structure of SIP extension-header. */
typedef struct tagSIP_HDR_EXT_HDR
{
    ST_ZOS_SSTR stName;              /**< @brief header-name */
    ST_ZOS_SSTR stVal;               /**< @brief header-value */
} ST_SIP_HDR_EXT_HDR;

/***************************** rfc3262 headers *************************/
/** @brief Structure of SIP RAck. */
typedef struct tagSIP_HDR_RACK
{
    ZUINT iRspNum;                   /**< @brief response-num */
    ZUINT iCseqNum;                  /**< @brief CSeq-num */
    ST_SIP_METHOD stMethod;          /**< @brief Method */  
} ST_SIP_HDR_RACK;

/** @brief Structure of SIP RSeq. */
typedef struct tagSIP_HDR_RSEQ
{
    ZUINT iRspNum;                   /**< @brief response-num */
} ST_SIP_HDR_RSEQ;

/***************************** rfc3265 headers *************************/
/** @brief Structure of SIP Event. */
typedef struct tagSIP_HDR_EVNT
{
    ST_SIP_EVNT_TYPE stEvntType;     /**< @brief event-type */
    ST_SIP_EVNT_PARM_LST stParmLst;  /**< @brief event-param list */
} ST_SIP_HDR_EVNT;

/** @brief Structure of SIP Allow-Events. */
typedef struct tagSIP_HDR_ALLOW_EVNTS
{
    ST_SIP_EVNT_TYPE_LST stEvntTypeLst; /**< @brief event-type list */
} ST_SIP_HDR_ALLOW_EVNTS;

/** @brief Structure of SIP Subscription-State. */
typedef struct tagSIP_HDR_SUBS_STA
{
    ST_SIP_SUBSTA_VAL stSubsVal;     /**< @brief substate-value */
    ST_SIP_SUBEXP_PARMS_LST stParmLst; /**< @brief subexp-params list */
} ST_SIP_HDR_SUBS_STA;

/***************************** rfc3515 headers *************************/
/** @brief Structure of SIP Refer-To. */
typedef struct tagSIP_HDR_REFER_TO
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
    ST_SIP_GEN_PARM_LST stParmLst;   /**< @brief generic-param list */
} ST_SIP_HDR_REFER_TO;

/*********************** rfc3892 headers *****************************/
/** @brief Structure of SIP Referred-By. */
typedef struct tagSIP_HDR_REFERRED_BY
{
    ST_SIP_NA_SPEC stNaSpec;         /**< @brief name-addr or addr-spec */
    ST_SIP_GEN_PARM_LST stParmLst;   /**< @brief generic-param list */
} ST_SIP_HDR_REFERRED_BY;

/*************************** rfc3891 headers **************************/
/** @brief Structure of SIP Replaces. */
typedef struct tagSIP_HDR_REPLACES
{
    ST_SIP_CALLID stCallid;          /**< @brief callid */
    ST_SIP_REPLACES_PARM_LST stParmLst; /**< @brief replaces-param list */
} ST_SIP_HDR_REPLACES;

/*********************** rfc4028 headers *****************************/
/** @brief Structure of SIP Session-Expires. */
typedef struct tagSIP_HDR_SESS_EXPIRES
{
    ZUCHAR ucRefresherPres;          /**< @brief refresher present flag */
    ZUCHAR ucRefresherUasPres;       /**< @brief refresher uas present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ZUINT iSeconds;                  /**< @brief delta-seconds */
} ST_SIP_HDR_SESS_EXPIRES;

/** @brief Structure of SIP Min-SE. */
typedef struct tagSIP_HDR_MIN_SE
{
    ZUINT iSeconds;                  /**< @brief delta-seconds */
} ST_SIP_HDR_MIN_SE;

/************************* rfc3841 headers ***************************/
/** @brief Structure of SIP Request-Disposition. */
typedef struct tagSIP_HDR_REQ_DISP
{
    ST_SIP_DIRECT stDirect;          /**< @brief directive */
    ST_SIP_DIRECT_LST stDirectLst;   /**< @brief directive list */
} ST_SIP_HDR_REQ_DISP;

/** @brief Structure of SIP Accept-Contact. */
typedef struct tagSIP_HDR_ACPT_CONTACT
{
    ST_SIP_AC_VAL_LST stAcValLst;    /**< @brief ac-value list */
} ST_SIP_HDR_ACPT_CONTACT;

/** @brief Structure of SIP Reject-Contact. */
typedef struct tagSIP_HDR_REJ_CONTACT
{
    ST_SIP_RC_VAL_LST stRcValLst;    /**< @brief rc-value list */
} ST_SIP_HDR_REJ_CONTACT;

/******************** draft-ietf-SIP-privacy headers ********************/
/** @brief Structure of SIP Anonymity. */
typedef struct tagSIP_HDR_ANONY
{
    ST_SIP_PRIVACY_TAG_LST stTagLst; /**< @brief privacy-tag list */
} ST_SIP_HDR_ANONY;

/** @brief Structure of SIP RPID-Privacy. */
typedef struct tagSIP_HDR_RPID_PRIVACY
{
    ST_SIP_RPID_PRIV_LST stPrivLst;  /**< @brief rpid-priv list */
} ST_SIP_HDR_RPID_PRIVACY;

/** @brief Structure of SIP Remote-Party-ID. */
typedef struct tagSIP_HDR_RMT_PARTY_ID
{
    ST_SIP_RPID_LST stRpidLst;       /**< @brief rpid list */
} ST_SIP_HDR_RMT_PARTY_ID;

/******************** rfc3903 headers ********************/
/** @brief Structure of SIP SIP-ETag. */
typedef struct tagSIP_HDR_SIP_ETAG
{
    ST_ZOS_SSTR stEntTag;            /**< @brief entity-tag */
} ST_SIP_HDR_SIP_ETAG;

/** @brief Structure of SIP SIP-If-Match. */
typedef struct tagSIP_HDR_SIP_IF_MATCH
{
    ST_ZOS_SSTR stEntTag;            /**< @brief entity-tag */
} ST_SIP_HDR_SIP_IF_MATCH;

/******************** rfc3911 headers ********************/
/** @brief Structure of SIP Join. */
typedef struct tagSIP_HDR_JOIN
{
    ST_SIP_CALLID stCallid;          /**< @brief callid */
    ST_SIP_JOIN_PARM_LST stParmLst;  /**< @brief join-param list */
} ST_SIP_HDR_JOIN;

/******************** rfc3455 headers ********************/
/** @brief Structure of SIP P-Associated-URI. */
typedef struct tagSIP_HDR_P_ASO_URI
{
    ST_SIP_P_ASO_URI_SPEC_LST stSpecLst; /**< @brief p-aso-uri-spec list */
} ST_SIP_HDR_P_ASO_URI;

/** @brief Structure of SIP P-Called-Party-ID. */
typedef struct tagSIP_HDR_P_CALLED_PTY_ID
{
    ST_SIP_CALLED_PTY_ID_SPEC stPtyIdSpec; /**< @brief called-pty-id-spec */
} ST_SIP_HDR_P_CALLED_PTY_ID;

/** @brief Structure of SIP P-Visited-Network-ID. */
typedef struct tagSIP_HDR_P_VNET_ID
{
    ST_SIP_P_VNET_ID_SPEC_LST stSpecLst; /**< @brief vnetwork-spec list */
} ST_SIP_HDR_P_VNET_ID;

/** @brief Structure of SIP P-Access-Network-Info. */
typedef struct tagSIP_HDR_P_ACCESS_NET_INFO
{
    ST_SIP_P_ACCESS_NET_SPEC stNetSpec; /**< @brief access-net-spec */
} ST_SIP_HDR_P_ACCESS_NET_INFO;

/** @brief Structure of SIP P-Charging-Addr. */
typedef struct tagSIP_HDR_P_CHARG_ADDR
{
    ST_SIP_CHARGE_ADDR_PARMS_LST stParmsLst; /**< @brief charge-addr-params list */
} ST_SIP_HDR_P_CHARG_ADDR;

/** @brief Structure of SIP P-Charging-Vector. */
typedef struct tagSIP_HDR_P_CHARG_VEC
{
    ST_SIP_GEN_VAL stIcidVal;        /**< @brief icid-value */
    ST_SIP_CHARGE_PARMS_LST stParmsLst; /**< @brief charge-params list */
} ST_SIP_HDR_P_CHARG_VEC;

/******************** rfc3325 headers ********************/
/** @brief Structure of SIP PAssertedID. */
typedef struct tagSIP_HDR_P_ASSERTED_ID
{
    ST_SIP_PASSERTEDID_VAL_LST stValLst; /**< @brief PAssertedID-value list */
} ST_SIP_HDR_P_ASSERTED_ID;

/** @brief Structure of SIP PPreferredID. */
typedef struct tagSIP_HDR_P_PREFERRED_ID
{
    ST_SIP_PPREFERREDID_VAL_LST stValLst; /**< @brief PPreferredID-value list */
} ST_SIP_HDR_P_PREFERRED_ID;

/******************** rfc3313 headers ********************/
/** @brief Structure of SIP P-Media-Authorization. */
typedef struct tagSIP_HDR_P_MEDIA_AUTHOR
{
    ST_SIP_P_MEDIA_AUTHOR_TKN_LST stTknLst; 
                                     /**< @brief P-Media-Authorization-Token list */
} ST_SIP_HDR_P_MEDIA_AUTHOR;

/******************** rfc3323 headers ********************/
/** @brief Structure of SIP Privacy-hdr. */
typedef struct tagSIP_HDR_PRIVACY
{
    ST_SIP_PRIV_VAL_LST stValLst;    /**< @brief priv-value list */
} ST_SIP_HDR_PRIVACY;

/******************** rfc3326 headers ********************/
/** @brief Structure of SIP Reason. */
typedef struct tagSIP_HDR_REASON
{
    ST_SIP_REASON_VAL_LST stValLst;  /**< @brief reason-value list */
} ST_SIP_HDR_REASON;

/******************** rfc3329 headers ********************/
/** @brief Structure of SIP security-client. */
typedef struct tagSIP_HDR_SEC_CLI
{
    ST_SIP_SEC_MECH_LST stMechLst;   /**< @brief sec-mechanism list */
} ST_SIP_HDR_SEC_CLI;

/** @brief Structure of SIP security-server. */
typedef struct tagSIP_HDR_SEC_SERV
{
    ST_SIP_SEC_MECH_LST stMechLst;   /**< @brief sec-mechanism list */
} ST_SIP_HDR_SEC_SERV;

/** @brief Structure of SIP security-verify. */
typedef struct tagSIP_HDR_SEC_VERIFY
{
    ST_SIP_SEC_MECH_LST stMechLst;   /**< @brief sec-mechanism list */
} ST_SIP_HDR_SEC_VERIFY;

/******************** rfc3603 headers ********************/
/** @brief Structure of SIP P-DCS-Trace-Party-ID. */
typedef struct tagSIP_HDR_P_DCS_TRACE_PTY_ID
{
    ST_SIP_NAME_ADDR stNameAddr;     /**< @brief name-addr */
} ST_SIP_HDR_P_DCS_TRACE_PTY_ID;

/** @brief Structure of SIP P-DCS-OSPS. */
typedef struct tagSIP_HDR_P_DCS_OSPS
{
    ST_SIP_TYPE_VAL stOspsTag;       /**< @brief OSPS-Tag EN_SIP_OSPS_TAG */
} ST_SIP_HDR_P_DCS_OSPS;

/** @brief Structure of SIP P-DCS-Billing-Info. */
typedef struct tagSIP_HDR_P_DCS_BILL_INFO
{
    ST_ZOS_SSTR stBillCorId;         /**< @brief Billing-Correlation-ID */
    ST_SIP_FEID stFeId;              /**< @brief FEID */
    ST_SIP_BILL_INFO_PARM_LST stBillLst; /**< @brief Billing-Info-param list */
} ST_SIP_HDR_P_DCS_BILL_INFO;

/** @brief Structure of SIP P-DCS-LAES. */
typedef struct tagSIP_HDR_P_DCS_LAES
{
    ST_SIP_HOST_PORT stSig;          /**< @brief Laes-sig */
    ST_SIP_LAES_PARM_LST stParmLst;  /**< @brief Laes-param list */
} ST_SIP_HDR_P_DCS_LAES;

/** @brief Structure of SIP P-DCS-Redirect. */
typedef struct tagSIP_HDR_P_DCS_REDIR
{
    ST_SIP_ADDR_SPEC stCalledId;     /**< @brief Called-ID */
    ST_SIP_REDIR_PARMS_LST stParmsLst; /**< @brief redir-params list */
} ST_SIP_HDR_P_DCS_REDIR;

/******************** rfc4244 headers ********************/
/** @brief Structure of SIP History-Info. */
typedef struct tagSIP_HDR_HI_INFO
{
    ST_SIP_HI_ENTRY_LST stEntryLst;  /**< @brief hi-entry list */
} ST_SIP_HDR_HI_INFO;

/*************** draft poc-p-headers headers *************/
/** @brief Structure of SIP P-Alerting-Mode. */
typedef struct tagSIP_HDR_P_ALERT_MODE
{
    ZUCHAR ucType;                   /**< @brief alerting-type EN_SIP_ALERT_TYPE */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
} ST_SIP_HDR_P_ALERT_MODE;

/******************** rfc4964 headers ********************/
/** @brief Structure of SIP P-Answer-State. */
typedef struct tagSIP_HDR_P_ANSWER_STATE
{
    ZUCHAR ucCnfPres;                /**< @brief Confirmed present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_GEN_PARM_LST stParmLst;   /**< @brief generic-param list */
} ST_SIP_HDR_P_ANSWER_STATE;

/******************** rfc4457 headers ********************/
/** @brief Structure of SIP P-User-Database. */
typedef struct tagSIP_HDR_P_USER_DB
{
    ST_SIP_DIAM_URI stDiamUri;       /**< @brief DiameterURI */
    ST_SIP_GEN_PARM_LST stParmLst;   /**< @brief generic-param list */
} ST_SIP_HDR_P_USER_DB;

/******************** rfc4488 headers ********************/
/** @brief Structure of SIP Refer-Sub. */
typedef struct tagSIP_HDR_REFER_SUB
{
    ZUCHAR ucTruePres;               /**< @brief true present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_GEN_PARM_LST stParmLst;   /**< @brief generic-param list */
} ST_SIP_HDR_REFER_SUB;

/******************** rfc2392 headers ********************/
/** @brief Structure of SIP Message-ID. */
typedef struct tagSIP_HDR_MSG_ID
{
    ST_SIP_ADDR_SPEC stAddrSpec;     /**< @brief addr-spec */
} ST_SIP_HDR_MSG_ID;

/** @brief Structure of SIP Content-ID. */
typedef struct tagSIP_HDR_CONTENT_ID
{
    ST_SIP_ADDR_SPEC stAddrSpec;     /**< @brief addr-spec */
} ST_SIP_HDR_CONTENT_ID;

/*************** draft SIP answermode headers *************/
/** @brief Structure of SIP Answer-Mode. */
typedef struct tagSIP_HDR_ANSWER_MODE
{
    ST_SIP_ANSWER_MODE_VAL stVal;    /**< @brief answer-mode-value */
    ST_SIP_ANSWER_MODE_PARM_LST stParmLst; /**< @brief answer-mode-param list */
} ST_SIP_HDR_ANSWER_MODE;

/** @brief Structure of SIP Priv-Answer-Mode. */
typedef struct tagSIP_HDR_PRIV_ANSWER_MODE
{
    ST_SIP_ANSWER_MODE_VAL stVal;    /**< @brief answer-mode-value */
    ST_SIP_ANSWER_MODE_PARM_LST stParmLst; /**< @brief answer-mode-param list */
} ST_SIP_HDR_PRIV_ANSWER_MODE;

/******* draft-drage-SIPping-service-identification *******/
/** @brief Structure of SIP P-Asserted-Service. */
typedef struct tagSIP_HDR_P_ASSERTED_SRV
{
    ST_SIP_SRV_ID_LST stVal;         /**< @brief PAssertedService-value */
} ST_SIP_HDR_P_ASSERTED_SRV;

/** @brief Structure of SIP P-Preferred-Service. */
typedef struct tagSIP_HDR_P_PREFERRED_SRV
{
    ST_SIP_SRV_ID_LST stVal;         /**< @brief PPreferredService-value */
} ST_SIP_HDR_P_PREFERRED_SRV;

/************************* rfc5009 ************************/
/* sip P-Preferred-Service */
typedef struct tagSIP_HDR_P_EARLY_MEDIA
{
    ST_SIP_EM_PARM_LST stParm;       /**< @brief parameter list */
} ST_SIP_HDR_P_EARLY_MEDIA;

/************************* rfc4538 ************************/
/* sip Target-Dialog */
typedef struct tagSIP_HDR_TARGET_DLG
{
    ST_SIP_CALLID stCallid;          /**< @brief callid */
    ST_SIP_TD_PARM_LST stParm;       /**< @brief td-param list */
} ST_SIP_HDR_TARGET_DLG;

/************************* rfc5318 ************************/
/* sip P-Refused-URI-List */
typedef struct tagSIP_HDR_P_REFUSED_URI_LIST
{
    ST_SIP_URI_ENTRY_LST stUriEntryLst; /**< @brief uri-list-entry list */
} ST_SIP_HDR_P_REFUSED_URI_LIST;

/************************* im ************************/
/* sip Contribution-ID */
typedef struct tagSIP_HDR_CONTRIBUTION_ID
{
    ST_ZOS_SSTR stContId;            /**< @brief contribution-id */
} ST_SIP_HDR_CONTRIBUTION_ID;

/** @brief Structure of SIP X-ZTE-Cause. */
typedef struct tagSIP_HDR_X_ZTE_CAUSE
{
    ST_ZOS_SSTR stZTECause;          /**< @brief X-ZTE-Cause */
} ST_SIP_HDR_X_ZTE_CAUSE;

/** @brief Structure of SIP ServInfo. */
typedef struct tagSIP_HDR_SERVINFO
{
    ST_SIP_SERVINFO_PARM_LST stParmLst; /**< @brief ServInfo-param list */
} ST_SIP_HDR_SERVINFO;

/** @brief Structure of SIP Conference-Info. */
typedef struct tagSIP_HDR_CONF_INFO
{
    ZUCHAR ucMediaType;                /**< @brief MediaTypes @ref EN_SIP_CONF_INFO_MEDIA */
    ZUCHAR ucRoleType;                 /**< @brief RoleType @ref EN_SIP_CONF_INFO_ROLE */
    ST_ZOS_SSTR stSubject;             /**< @brief Subject */
} ST_SIP_HDR_CONF_INFO;

/** @brief Structure of SIP TerminalSeatType. */
typedef struct tagSIP_HDR_SEAT_TYPE
{
    ZUINT iType;                   /**< @brief MediaTypes @ref EN_SIP_SEAT_TYPE */
} ST_SIP_HDR_SEAT_TYPE;

/** @brief Structure of SIP message-header. */
typedef struct tagSIP_MSG_HDR
{
    ZUCHAR ucType;                   /**< @brief message-header type EN_SIP_HDR */ 
    ZUCHAR ucStrPres;                /**< @brief header string present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ST_ZOS_SSTR stHdrName;           /**< @brief header name string */
    ST_ZOS_SSTR stHdrVal;            /**< @brief header value string */
    union
    {
        /* rfc3261 headers */
        ST_SIP_HDR_ACPT *pstAcpt;    /**< @brief Accept header. */
        ST_SIP_HDR_ACPT_ENCODING *pstAcptEncoding; /**< @brief Accept-Encoding header. */
        ST_SIP_HDR_ACPT_LANG *pstAcptLang; /**< @brief Accept-Language header. */
        ST_SIP_HDR_ALERT_INFO *pstAlertInfo; /**< @brief Alert-Info header. */
        ST_SIP_HDR_ALLOW *pstAllow;  /**< @brief Allow header. */
        ST_SIP_HDR_AUTHEN_INFO *pstAuthenInfo; /**< @brief Authentication-Info header. */
        ST_SIP_HDR_AUTHOR *pstAuthor; /**< @brief Authorization header. */
        ST_SIP_HDR_CALL_ID *pstCallId; /**< @brief Call-ID header. */
        ST_SIP_HDR_CALL_INFO *pstCallInfo; /**< @brief Call-Info header. */
        ST_SIP_HDR_CONTACT *pstContact;/**< @brief Contact header. */
        ST_SIP_HDR_CONTENT_DISP *pstContentDisp; /**< @brief Content-Disposition header. */
        ST_SIP_HDR_CONTENT_ENCODING *pstContentEncoding; /**< @brief Content-Encoding header. */
        ST_SIP_HDR_CONTENT_LANG *pstContentLang; /**< @brief Content-Language header. */
        ZUINT iContentLen;         /**< @brief Content-Length header. */
        ST_SIP_HDR_CONTENT_TYPE *pstContentType; /**< @brief Content-Type header. */
        ST_SIP_HDR_CSEQ *pstCseq;    /**< @brief CSeq header. */
        ST_SIP_HDR_DATE *pstDate;    /**< @brief Date header. */
        ST_SIP_HDR_ERROR_INFO *pstErrorInfo; /**< @brief Error-Info header. */
        ZUINT iExpires;            /**< @brief Expires header. */
        ST_SIP_HDR_FROM_TO *pstFromTo; /**< @brief From or To header. */
        ST_SIP_HDR_IN_REPLY_TO *pstInReplyTo; /**< @brief In-Reply-To header. */
        ZUINT iMaxForwards;        /**< @brief Max-Forwards header. */
        ST_SIP_HDR_MIME_VER *pstMimeVer; /**< @brief MIME-Version header. */
        ZUINT iMinExpires;         /**< @brief Min-Expires header. */
        ST_SIP_HDR_ORGANIZATION *pstOrganization; /**< @brief Organization header. */
        ST_SIP_HDR_PRIORITY *pstPriority; /**< @brief Priority header. */
        ST_SIP_HDR_PROXY_AUTHEN *pstProxyAuthen; /**< @brief Proxy-Authenticate header. */
        ST_SIP_HDR_PROXY_AUTHOR *pstProxyAuthor; /**< @brief Proxy-Authorization header. */
        ST_SIP_HDR_PROXY_REQUIRE *pstProxyRequire; /**< @brief Proxy-Require header. */
        ST_SIP_HDR_ROUTE *pstRecordRoute; /**< @brief Record-Route header. */
        ST_SIP_HDR_REPLY_TO *pstReplyTo; /**< @brief Reply-To header. */
        ST_SIP_HDR_REQUIRE *pstRequire; /**< @brief Require header. */
        ST_SIP_HDR_RETRY_AFTER *pstRetryAfter; /**< @brief Retry-After header. */
        ST_SIP_HDR_ROUTE *pstRoute;  /**< @brief Route header. */
        ST_SIP_HDR_SERVER *pstServer; /**< @brief Server header. */
        ST_SIP_HDR_SUBJECT *pstSubject;/**< @brief Subject header. */
        ST_SIP_HDR_SUPTED *pstSupted; /**< @brief Supported header. */
        ST_SIP_HDR_TIMESTAMP *pstTimestamp; /**< @brief Timestamp header. */
        ST_SIP_HDR_UNSUPTED *pstUnsupted; /**< @brief Unsupported header. */
        ST_SIP_HDR_USER_AGENT *pstUserAgent; /**< @brief User-Agent header. */
        ST_SIP_HDR_VIA *pstVia;      /**< @brief Via header. */
        ST_SIP_HDR_WARN *pstWarn;    /**< @brief Warning header. */
        ST_SIP_HDR_WWW_AUTHEN *pstWwwAuthen; /**< @brief WWW-Authenticate header. */
        /* rfc3262 headers */
        ST_SIP_HDR_RACK *pstRack;    /**< @brief RAck header. */
        ST_SIP_HDR_RSEQ *pstRseq;    /**< @brief RSeq header. */
        /* rfc3265 headers */
        ST_SIP_HDR_EVNT *pstEvnt;    /**< @brief Event header. */
        ST_SIP_HDR_ALLOW_EVNTS *pstAllowEvnts; /**< @brief Allow-Events header. */
        ST_SIP_HDR_SUBS_STA *pstSubsSta; /**< @brief Subscription-State header. */
        /* rfc3515 headers */
        ST_SIP_HDR_REFER_TO *pstReferTo; /**< @brief Refer-To header. */        
        /* rfc3892 */
        ST_SIP_HDR_REFERRED_BY *pstReferredBy; /**< @brief Referred-By header. */
        /* rfc3891 */
        ST_SIP_HDR_REPLACES *pstReplaces; /**< @brief Replaces header. */
        /* rfc4028 */
        ST_SIP_HDR_SESS_EXPIRES *pstSessExpires; /**< @brief Session-Expires header. */
        ST_SIP_HDR_MIN_SE *pstMinSe; /**< @brief Min-SE header. */
        /* rfc3841 */
        ST_SIP_HDR_REQ_DISP *pstReqDisp; /**< @brief Request-Disposition header. */
        ST_SIP_HDR_ACPT_CONTACT *pstAcptContact; /**< @brief Accept-Contact header. */
        ST_SIP_HDR_REJ_CONTACT *pstRejContact; /**< @brief Reject-Contact header. */
        /* draft-ietf-sip-privacy */
        ST_SIP_HDR_ANONY *pstAnony;  /**< @brief Anonymity header. */
        ST_SIP_HDR_RPID_PRIVACY *pstRpidPrivacy; /**< @brief RPID-Privacy header. */
        ST_SIP_HDR_RMT_PARTY_ID *pstRmtPartyId; /**< @brief Remote-Party-ID header. */  
        /* rfc3903 */
        ST_SIP_HDR_SIP_ETAG *pstSipEtag; /**< @brief SIP-ETag header. */
        ST_SIP_HDR_SIP_IF_MATCH *pstSipIfMatch; /**< @brief SIP-If-Match header. */
        /* rfc3911 */
        ST_SIP_HDR_JOIN *pstJoin;    /**< @brief Join header. */
        /* rfc3455 for 3gpp */
        ST_SIP_HDR_P_ASO_URI *pstPAsoUri; /**< @brief P-Associated-URI header. */
        ST_SIP_HDR_P_CALLED_PTY_ID *pstPCalledPtyId; /**< @brief P-Called-Party-ID header. */
        ST_SIP_HDR_P_VNET_ID *pstPVnetId; /**< @brief P-Visited-Network-ID header. */
        ST_SIP_HDR_P_ACCESS_NET_INFO *pstPAnetInfo; /**< @brief P-Access-Network-Info header. */
        ST_SIP_HDR_P_CHARG_ADDR *pstPChargAddr; /**< @brief P-Charging-Addr header. */
        ST_SIP_HDR_P_CHARG_VEC *pstPChargVec; /**< @brief P-Charging-Vector header. */        
        /* rfc3327 for 3gpp */
        ST_SIP_HDR_ROUTE *pstPath;   /**< @brief Path header. */
        /* rfc3608 for 3gpp */
        ST_SIP_HDR_ROUTE *pstServRoute; /**< @brief Service-Route header. */
        /* rfc3325 */
        ST_SIP_HDR_P_ASSERTED_ID *pstPAssertedId; /**< @brief PAssertedID header. */
        ST_SIP_HDR_P_PREFERRED_ID *pstPPreferredId; /**< @brief PPreferredID header. */
        /* rfc3313 */
        ST_SIP_HDR_P_MEDIA_AUTHOR *pstPMediaAuthor; /**< @brief P-Media-Authorization header. */
        /* rfc3323 */
        ST_SIP_HDR_PRIVACY *pstPrivacy; /**< @brief Privacy-hdr header. */
        /* rfc3326 */
        ST_SIP_HDR_REASON *pstReason; /**< @brief Reason header. */        
        /* rfc3329 */
        ST_SIP_HDR_SEC_CLI *pstSecCli; /**< @brief security-client header. */        
        ST_SIP_HDR_SEC_SERV *pstSecServ; /**< @brief security-server header. */        
        ST_SIP_HDR_SEC_VERIFY *pstSecVerify; /**< @brief security-verify header. */
        /* rfc3603 */
        ST_SIP_HDR_P_DCS_TRACE_PTY_ID *pstPDcsTPtyId; /**< @brief P-DCS-Trace-Party-ID header. */
        ST_SIP_HDR_P_DCS_OSPS *pstPDcsOsps; /**< @brief P-DCS-OSPS header. */
        ST_SIP_HDR_P_DCS_BILL_INFO *pstPDcsBillInfo; /**< @brief P-DCS-Billing-Info header. */
        ST_SIP_HDR_P_DCS_LAES *pstPDcsLaes; /**< @brief P-DCS-LAES header. */
        ST_SIP_HDR_P_DCS_REDIR *pstPDcsRedir; /**< @brief P-DCS-Redirect header. */
        /* rfc4244 */
        ST_SIP_HDR_HI_INFO *pstHiInfo; /**< @brief History-Info header. */
        /* draft poc-p-headers */
        ST_SIP_HDR_P_ALERT_MODE *pstPAlertMode; /**< @brief P-Alerting-Mode header. */
        /* rfc4964 */
        ST_SIP_HDR_P_ANSWER_STATE *pstPAnswerState; /**< @brief P-Answer-State header. */
        /* rfc4457 */
        ST_SIP_HDR_P_USER_DB *pstPUserDb; /**< @brief P-User-Database header. */
        /* rfc4488  */
        ST_SIP_HDR_REFER_SUB *pstReferSub; /**< @brief Refer-Sub header. */
        /* rfc2392 */
        ST_SIP_HDR_MSG_ID *pstMsgId; /**< @brief Message-ID header. */
        ST_SIP_HDR_CONTENT_ID *pstContentId; /**< @brief Content-ID header. */
        /* rfc5373 */
        ST_SIP_HDR_ANSWER_MODE *pstAnswerMode; /**< @brief Answer-Mode header. */
        ST_SIP_HDR_PRIV_ANSWER_MODE *pstPrivAnswerMode; /**< @brief Priv-Answer-Mode header. */
        /* draft-drage-sipping-service-identification */
        ST_SIP_HDR_P_ASSERTED_SRV *pstPAssertedSrv; /**< @brief PAssertedService header. */
        ST_SIP_HDR_P_PREFERRED_SRV *pstPPreferredSrv; /**< @brief PPreferredService header. */
        /* rfc5009 */
        ST_SIP_HDR_P_EARLY_MEDIA *pstPEarlyMedia; /**< @brief P-Early-Media header. */
        /* rfc5626 */
        ZUINT iFlowTimer;          /**< @brief Flow-Timer header. */
        /* rfc4538 */
        ST_SIP_HDR_TARGET_DLG *pstTargetDlg; /**< @brief Target-Dialog header */
        /* rfc 5318 */
        ST_SIP_HDR_P_REFUSED_URI_LIST *pstPRefusedUriLst; /**< @brief P-Refused-URI-List header */
        /* OMA-TS-SIMPLE-IM-V1_0-20100322-C */
        ST_ZOS_SSTR stSessReplaces; /**< @brief Session-Replaces header */
        ST_SIP_HDR_CONTRIBUTION_ID *pstContId; /**< @brief Contribution-ID header */
        ST_ZOS_SSTR stContDescription; /**< @brief Content-Description header */
        /* OMA-TS-CPM-V1_0-20120813-C */
        ST_ZOS_SSTR stConversationId; /**< @brief Conversation-ID header */
        ST_ZOS_SSTR stIrtContId;     /**< @brief InReplyTo-Contribution-ID header */
        /* extension-header */
        ST_SIP_HDR_EXT_HDR *pstExtHdr; /**< @brief extension-header header. */
        ST_SIP_HDR_X_ZTE_CAUSE *pstZTECause;    /**< @brief X-ZTE-Cause header. */
        ST_SIP_HDR_SERVINFO *pstServInfo;   /**< @brief ServInfo header. */
        ST_SIP_HDR_CONF_INFO *pstConfInfo;  /**< @brief Conference-Info header.*/
        ST_SIP_HDR_SEAT_TYPE *pstSeatType; /**< @brief TerminalSeatType header.*/
        ST_ZOS_SSTR stStsCallId;     /**< @brief STS-Call-ID header */
        ZVOID *pUnknownHdr;          /* unknown header */
    } u;
} ST_SIP_MSG_HDR;

/** @brief List of SIP message-header. */
ABNF_TYPEDEF_LIST(SIP_MSG_HDR); 

/** @brief Structure of SIP body with mutlipart rfc2046. */
typedef struct tagSIP_BODY_MPART
{
    ST_SIP_MSG_HDR_LST stHdrLst;     /**< @brief message-header list */
    struct tagSIP_BODY *pstBody;     /**< @brief body in multipart body */
} ST_SIP_BODY_MPART;

/** @brief List of SIP multipart body. */
ABNF_TYPEDEF_LIST(SIP_BODY_MPART); 

/** @brief Structure of SIP body SIPfrag rfc3420. */
typedef struct tagSIP_BODY_SFRAG
{
    ZUCHAR ucSLinePres;              /**< @brief Status line present flag */
    ZUCHAR ucReqPres;                /**< @brief Request present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    union
    {
        struct tagSIP_REQ_LINE *pstReqLine; /**< @brief Request Line */
        struct tagSIP_STATUS_LINE *pstStatusLine; /**< @brief Status Line */
    } u;
    ST_SIP_MSG_HDR_LST stHdrLst;     /**< @brief message-header list */
    struct tagSIP_BODY *pstBody;     /**< @brief body in multipart body */
} ST_SIP_BODY_SFRAG;

/** @brief Single part of SIP body. */
typedef struct tagSIP_BODY_SPART
{
    ZUCHAR ucType;                   /**< @brief body part type EN_SIP_BODY_PART */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stStr;          /**< @brief not SDP message body */
        ST_SIP_BODY_SFRAG stSfrag;   /**< @brief SIP frag */
#ifndef SIP_ABNF_SUPT_NOSDP
        ST_SDP_SESS_DESC_LST stSdpMsgLst; /**< @brief SDP protocol message */
#endif
    } u;
} ST_SIP_BODY_SPART;

/** @brief Structure of SIP body. */
typedef struct tagSIP_BODY
{                                         
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucMpartPres;              /**< @brief multipart present flag */
    ZUCHAR ucMtype;                  /**< @brief m-type type EN_SIP_MTYPE */
    ZUCHAR ucMSubtype;               /**< @brief m-subtype type EN_SIP_MSUBTYPE */
    ST_ZOS_SSTR stMtypeExt;          /**< @brief m-type extension-token */
    ST_ZOS_SSTR stMSubTypeExt;       /**< @brief m-subtype extension-token */
    ST_ZOS_SSTR stBodyStr;           /**< @brief body string(decode used) */
    union
    {
        ST_SIP_BODY_MPART_LST stMpartLst; /**< @brief multipart body */
        ST_SIP_BODY_SPART stSpart;   /**< @brief single part body */
    } u;
} ST_SIP_BODY;

/** @brief Structure of SIP SIP-Version. */
typedef struct tagSIP_VER
{
    ZUINT iMajor;                  /**< @brief major version */
    ZUINT iMinor;                  /**< @brief minor version */
} ST_SIP_VER;

/** @brief Structure of SIP Request-Line. */
typedef struct tagSIP_REQ_LINE
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_METHOD stMethod;          /**< @brief Method */
    ST_SIP_REQ_URI stReqUri;         /**< @brief Request-URI */
    ST_SIP_VER stVer;                /**< @brief SIP-Version */
} ST_SIP_REQ_LINE;

/** @brief Structure of SIP Status-Line. */
typedef struct tagSIP_STATUS_LINE
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR aucSpare[3];              /**< @brief for 32 bit alignment */
    ST_SIP_VER stVer;                /**< @brief SIP-Version */
    ZUINT iCode;                     /**< @brief Status-Code */
    ST_ZOS_SSTR stReason;            /**< @brief Reason-Phrase */
} ST_SIP_STATUS_LINE;

/** @brief Structure of SIP message. */
typedef struct tagSIP_MSG
{
    ZUCHAR ucPres;                   /**< @brief present flag */
    ZUCHAR ucReqPres;                /**< @brief Request present flag */
    ZUCHAR aucSpare[2];              /**< @brief for 32 bit alignment */
    ZDBUF zMemBuf;                   /**< @brief memory buffer */
    ZDBUF zMsgBuf;                   /**< @brief message buffer */
    ST_ZOS_SSTR stMsgStr;            /**< @brief message string(decode used) */
    union
    {
        ST_SIP_REQ_LINE stReqLine;   /**< @brief Request Line */
        ST_SIP_STATUS_LINE stStatusLine; /**< @brief Status Line */
    } u;
    ST_SIP_MSG_HDR_LST stHdrLst;     /**< @brief message-header list */
    ST_SIP_BODY stBody;              /**< @brief message-body */
} ST_SIP_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _SIP_ABNF_TYPE_H__ */

