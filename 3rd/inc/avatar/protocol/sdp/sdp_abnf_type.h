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
  File name     : sdp_abnf_type.h
  Module        : sdp protocol typedef
  Author        : Zocol  
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Marcos and structure definitions required by the sdp protocol
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SDP_ABNF_TYPE_H__
#define _SDP_ABNF_TYPE_H__

#ifdef __cplusplus
extern "C"{
#endif 

/* sdp net type */
typedef enum EN_SDP_NET_TYPE
{
    EN_SDP_NET_IN,
    EN_SDP_NET_ANY,
    EN_SDP_NET_ATM,
    EN_SDP_NET_LCL,
    EN_SDP_NET_TN                    /* telephone network (rfc2848) */
} EN_SDP_NET_TYPE;

/* sdp addrtype */
typedef enum EN_SDP_ADDR_TYPE
{
    EN_SDP_ADDR_IPV4,
    EN_SDP_ADDR_IPV6,
    EN_SDP_ADDR_2543                 /* address types for telephone network */
} EN_SDP_ADDR_TYPE;

/* sdp unicast-address type */
typedef enum EN_SDP_UCAST_ADDR_TYPE
{
    EN_SDP_UCAST_ADDR_IPV4,
    EN_SDP_UCAST_ADDR_IPV6,
    EN_SDP_UCAST_ADDR_FQDN,
    EN_SDP_UCAST_ADDR_EXTN
} EN_SDP_UCAST_ADDR_TYPE;

/* sdp connection-address type */
typedef enum EN_SDP_CONN_ADDR_TYPE
{
    EN_SDP_CONN_ADDR_IPV4,
    EN_SDP_CONN_ADDR_MIPV4,
    EN_SDP_CONN_ADDR_IPV6,
    EN_SDP_CONN_ADDR_MIPV6,
    EN_SDP_CONN_ADDR_FQDN,
    EN_SDP_CONN_ADDR_TN,
    EN_SDP_CONN_ADDR_EXTN
} EN_SDP_CONN_ADDR_TYPE;

/* sdp bwtype type */
typedef enum EN_SDP_BWTYPE_TYPE
{
    EN_SDP_BWTYPE_CT,                /* rfc4566 */
    EN_SDP_BWTYPE_AS,                /* rfc4566 */
    EN_SDP_BWTYPE_RS,                /* rfc3556 */
    EN_SDP_BWTYPE_RR,                /* rfc3556 */
    EN_SDP_BWTYPE_TIAS,              /* rfc3890 */
    EN_SDP_BWTYPE_UNKNOWN
} EN_SDP_BWTYPE_TYPE;

/* sdp key-type type */
typedef enum EN_SDP_KEY_TYPE
{
    EN_SDP_KEY_PROMPT,
    EN_SDP_KEY_CLEAR,
    EN_SDP_KEY_BASE64,
    EN_SDP_KEY_URI
} EN_SDP_KEY_TYPE;

/* sdp media type */
typedef enum EN_SDP_MEDIA_TYPE
{
    EN_SDP_MEDIA_AUDIO,
    EN_SDP_MEDIA_VIDEO,
    EN_SDP_MEDIA_APP,
    EN_SDP_MEDIA_DATA,
    EN_SDP_MEDIA_CTRL,
    EN_SDP_MEDIA_IMAGE,
    EN_SDP_MEDIA_TXT,
    EN_SDP_MEDIA_MSG,
    EN_SDP_MEDIA_UNKNOWN
} EN_SDP_MEDIA_TYPE;

/* sdp proto type */
typedef enum EN_SDP_PROTO_TYPE
{
    EN_SDP_PROTO_UDP, 
    EN_SDP_PROTO_TCP,                /* rfc4145 */
    EN_SDP_PROTO_RTP,                /* historic */
    EN_SDP_PROTO_RTP_AVP,            /* rfc4566 */
    EN_SDP_PROTO_RTP_AVPF,           /* rfc4585 */
    EN_SDP_PROTO_RTP_SAVP,           /* rfc3711 */
    EN_SDP_PROTO_RTP_SAVPF,          /* rfc5124 */
    EN_SDP_PROTO_TCP_RTP_AVP,        /* rfc4571 */
    EN_SDP_PROTO_UDP_BFCP,           /* rfc4583 */
    EN_SDP_PROTO_TCP_BFCP,           /* rfc4583 */
    EN_SDP_PROTO_TCP_TLS_BFCP,       /* rfc4583 */
    EN_SDP_PROTO_TCP_TLS,            /* rfc4572 */
    EN_SDP_PROTO_VAT,                /* historic */
    EN_SDP_PROTO_UDPT1,              /* ITU-T T.38 */
    EN_SDP_PROTO_LOCAL, 
    EN_SDP_PROTO_ATM, 
    EN_SDP_PROTO_AAL1_ATMF, 
    EN_SDP_PROTO_AAL1_ITU, 
    EN_SDP_PROTO_AAL1_CUSTOM, 
    EN_SDP_PROTO_AAL2_ATMF, 
    EN_SDP_PROTO_AAL2_ITU, 
    EN_SDP_PROTO_AAL2_CUSTOM, 
    EN_SDP_PROTO_AAL5_ATMF, 
    EN_SDP_PROTO_AAL5_ITU,         
    EN_SDP_PROTO_AAL5_CUSTOM, 
    EN_SDP_PROTO_H323C, 
    EN_SDP_PROTO_VOICE,              /* rfc2848 */
    EN_SDP_PROTO_FAX,                /* rfc2848 */
    EN_SDP_PROTO_PAPER,              /* rfc2848 */
    EN_SDP_PROTO_TCP_MSRP,           /* rfc4975 */
    EN_SDP_PROTO_TCP_TLS_MSRP,       /* rfc4975 */
    EN_SDP_PROTO_UNKNOWN        
} EN_SDP_PROTO_TYPE;

/* sdp fixed-len-time-unit type */
typedef enum EN_SDP_TIME_UNIT_TYPE
{
    EN_SDP_TIME_UNIT_DAY,            
    EN_SDP_TIME_UNIT_HR,             
    EN_SDP_TIME_UNIT_MIN,            
    EN_SDP_TIME_UNIT_SEC          
} EN_SDP_TIME_UNIT_TYPE;

/* sdp rfc3640 parameter type */
typedef enum EN_SDP_3640_PARM_TYPE
{
    EN_SDP_3640_PARM_STREAM_TYPE,            
    EN_SDP_3640_PARM_PROF_LEVEL_ID,     
    EN_SDP_3640_PARM_CFG,     
    EN_SDP_3640_PARM_MODE,        
    EN_SDP_3640_PARM_OBJ_TYPE,       
    EN_SDP_3640_PARM_CNST_SIZE,
    EN_SDP_3640_PARM_CNST_DURA,
    EN_SDP_3640_PARM_MAX_DISP,
    EN_SDP_3640_PARM_DEINTER_BUFSIZE,
    EN_SDP_3640_PARM_SIZE_LEN,
    EN_SDP_3640_PARM_IDX_LEN,
    EN_SDP_3640_PARM_IDX_DELTA_LEN,
    EN_SDP_3640_PARM_CTS_DELTA_LEN,
    EN_SDP_3640_PARM_DTS_DELTA_LEN,
    EN_SDP_3640_PARM_RAND_ACS_IND,
    EN_SDP_3640_PARM_STREAM_STA_IND,
    EN_SDP_3640_PARM_AUXI_DATA_SIZE_LEN
} EN_SDP_3640_PARM_TYPE;

/* sdp h263 size type */
typedef enum EN_SDP_H263_SIZE_TYPE
{
    EN_SDP_H263_SIZE_SQCIF,            
    EN_SDP_H263_SIZE_QCIF,     
    EN_SDP_H263_SIZE_CIF,     
    EN_SDP_H263_SIZE_CIF4,        
    EN_SDP_H263_SIZE_CIF16,       
    EN_SDP_H263_SIZE_XYMPI  
} EN_SDP_H263_SIZE_TYPE;

/* sdp h263 Annex type */
typedef enum EN_SDP_H263_ANX_TYPE
{
    EN_SDP_H263_ANX_D,            
    EN_SDP_H263_ANX_E,     
    EN_SDP_H263_ANX_F,     
    EN_SDP_H263_ANX_G,        
    EN_SDP_H263_ANX_I,       
    EN_SDP_H263_ANX_J,
    EN_SDP_H263_ANX_K, 
    EN_SDP_H263_ANX_L,
    EN_SDP_H263_ANX_M,
    EN_SDP_H263_ANX_N,
    EN_SDP_H263_ANX_O,
    EN_SDP_H263_ANX_P,
    EN_SDP_H263_ANX_Q,
    EN_SDP_H263_ANX_R,
    EN_SDP_H263_ANX_S,
    EN_SDP_H263_ANX_T
} EN_SDP_H263_ANX_TYPE;

/* sdp h263 Params type */
typedef enum EN_SDP_H263_PARM_TYPE
{
    EN_SDP_H263_PARM_PAR,            
    EN_SDP_H263_PARM_CPCF,     
    EN_SDP_H263_PARM_MAXBR,     
    EN_SDP_H263_PARM_BPP,        
    EN_SDP_H263_PARM_HRD,       
    EN_SDP_H263_PARM_INTERLACE,
    EN_SDP_H263_PARM_EXT 
} EN_SDP_H263_PARM_TYPE;

/* sdp h263 option type */
typedef enum EN_SDP_H263_OPT_TYPE
{
    EN_SDP_H263_OPT_SIZE,            
    EN_SDP_H263_OPT_ANX,     
    EN_SDP_H263_OPT_PARM        
} EN_SDP_H263_OPT_TYPE;

/* sdp rfc4587 parameter type */
typedef enum EN_SDP_4587_PARM_TYPE
{
    EN_SDP_4587_PARM_CIF,
    EN_SDP_4587_PARM_QCIF,
    EN_SDP_4587_PARM_D
} EN_SDP_4587_PARM_TYPE;

/* rfc4629 parameter type */
typedef enum EN_SDP_4629_PARM_TYPE
{
    EN_SDP_4629_PARM_SQCIF,
    EN_SDP_4629_PARM_QCIF,
    EN_SDP_4629_PARM_CIF,
    EN_SDP_4629_PARM_CIF4,
    EN_SDP_4629_PARM_CIF16,
    EN_SDP_4629_PARM_CUSTOM,
    EN_SDP_4629_PARM_F,
    EN_SDP_4629_PARM_I,
    EN_SDP_4629_PARM_J,
    EN_SDP_4629_PARM_T,
    EN_SDP_4629_PARM_K,
    EN_SDP_4629_PARM_N,
    EN_SDP_4629_PARM_P,
    EN_SDP_4629_PARM_PAR,
    EN_SDP_4629_PARM_CPCF,
    EN_SDP_4629_PARM_BPP,
    EN_SDP_4629_PARM_HRD,
    EN_SDP_4629_PARM_PROF,
    EN_SDP_4629_PARM_LEVEL,
    EN_SDP_4629_PARM_INTERLACE
} EN_SDP_4629_PARM_TYPE;

/* rfc3984 parameter type */
typedef enum EN_SDP_3984_PARM_TYPE
{
    EN_SDP_3984_PARM_PROF_LEVEL_ID,
    EN_SDP_3984_PARM_MAX_MBPS,
    EN_SDP_3984_PARM_MAX_FS,
    EN_SDP_3984_PARM_MAX_CPB,
    EN_SDP_3984_PARM_MAX_DPB,
    EN_SDP_3984_PARM_MAX_BR,
    EN_SDP_3984_PARM_REDUND_PIC_CAP,
    EN_SDP_3984_PARM_SPROP_PARM_SETS,
    EN_SDP_3984_PARM_PARM_ADD,
    EN_SDP_3984_PARM_PACKET_MODE,
    EN_SDP_3984_PARM_SPROP_INTERLEAV_DEPTH,
    EN_SDP_3984_PARM_DEINT_BUF_CAP,
    EN_SDP_3984_PARM_SPROP_DEINT_BUF_REQ,
    EN_SDP_3984_PARM_SPROP_INIT_BUF_TIME,
    EN_SDP_3984_PARM_SPROP_MAX_DON_DIFF,
    EN_SDP_3984_PARM_MAX_RCMD_NALU_SIZE,
    EN_SDP_3984_PARM_LEVEL_ASYM_ALLOWED,
} EN_SDP_3984_PARM_TYPE;

/* rfc3267 parameter type */
typedef enum EN_SDP_3267_PARM_TYPE
{
    EN_SDP_3267_PARM_OCTET_ALIGN,
    EN_SDP_3267_PARM_MODE_SET,
    EN_SDP_3267_PARM_MODE_CHANGE_PERIOD,
    EN_SDP_3267_PARM_MODE_CHANGE_CAPABILITY,
    EN_SDP_3267_PARM_MODE_CHANGE_NEIGHBOR,
    EN_SDP_3267_PARM_CRC,
    EN_SDP_3267_PARM_ROBUST_SORTING,
    EN_SDP_3267_PARM_INTERLEAVING,
    EN_SDP_3267_PARM_CHANL,
    EN_SDP_3267_PARM_MAX_RED
} EN_SDP_3267_PARM_TYPE;

/* oma-poc TBCP parameter type */
typedef enum EN_SDP_TBCP_PARM_TYPE
{
    EN_SDP_TBCP_PARM_QUEUING,
    EN_SDP_TBCP_PARM_TB_PRIORITY,
    EN_SDP_TBCP_PARM_TIMESTAMP,
    EN_SDP_TBCP_PARM_TB_GRANTED,
    EN_SDP_TBCP_PARM_POC_SESS_PRIORITY,
    EN_SDP_TBCP_PARM_POC_LOCK
} EN_SDP_TBCP_PARM_TYPE;

/* fmtp type */
typedef enum EN_SDP_FMTP_TYPE
{
    EN_SDP_FMTP_2733,
    EN_SDP_FMTP_2833,
    EN_SDP_FMTP_3640,
    EN_SDP_FMTP_3952,
    EN_SDP_FMTP_H26X,
    EN_SDP_FMTP_4587,
    EN_SDP_FMTP_4629,
    EN_SDP_FMTP_3984,
    EN_SDP_FMTP_3267,
    EN_SDP_FMTP_TBCP,
    EN_SDP_FMTP_4588,
    EN_SDP_FMTP_UNKNOWN
} EN_SDP_FMTP_TYPE;

/* encoding type */
typedef enum EN_SDP_ENCODING_TYPE
{
    EN_SDP_ENCODING_PCMU = 0,
    EN_SDP_ENCODING_GSM,
    EN_SDP_ENCODING_G723,
    EN_SDP_ENCODING_PCMA,
    EN_SDP_ENCODING_G722,
    EN_SDP_ENCODING_G7221 = 5,
    EN_SDP_ENCODING_G726_16,
    EN_SDP_ENCODING_G726_24,
    EN_SDP_ENCODING_G726_32,
    EN_SDP_ENCODING_G726_40,
    EN_SDP_ENCODING_G728 = 10,
    EN_SDP_ENCODING_G729,
    EN_SDP_ENCODING_G729D,
    EN_SDP_ENCODING_G729E,
    EN_SDP_ENCODING_JPEG,
    EN_SDP_ENCODING_H261 = 15,
    EN_SDP_ENCODING_H263,
    EN_SDP_ENCODING_H263_1998,
    EN_SDP_ENCODING_H263_2000,
    EN_SDP_ENCODING_H264,
    EN_SDP_ENCODING_MPEG4 = 20,
    EN_SDP_ENCODING_MPEG4_GEN,
    EN_SDP_ENCODING_ILBC,
    EN_SDP_ENCODING_TEL_EVNT,
    EN_SDP_ENCODING_TONE,
    EN_SDP_ENCODING_DIV4 = 25,
    EN_SDP_ENCODING_AMR,
    EN_SDP_ENCODING_AMR_WB,
    EN_SDP_ENCODING_EVRC,
    EN_SDP_ENCODING_EVRC0,
    EN_SDP_ENCODING_EVRC1 = 30,
    EN_SDP_ENCODING_EVRCB,
    EN_SDP_ENCODING_EVRCB0,
    EN_SDP_ENCODING_EVRCB1,
    EN_SDP_ENCODING_MP1S,
    EN_SDP_ENCODING_MP2P = 35,
    EN_SDP_ENCODING_MP4V_ES,
    EN_SDP_ENCODING_LPC,
    EN_SDP_ENCODING_L16,
    EN_SDP_ENCODING_QCELP,
    EN_SDP_ENCODING_CN = 40,
    EN_SDP_ENCODING_MPA,
    EN_SDP_ENCODING_CELB,
    EN_SDP_ENCODING_NV,
    EN_SDP_ENCODING_MPV,
    EN_SDP_ENCODING_MP2T = 45,
    EN_SDP_ENCODING_PARITYFEC,
    EN_SDP_ENCODING_ULPFEC,
    EN_SDP_ENCODING_VP8,
    EN_SDP_ENCODING_ISAC,
    EN_SDP_ENCODING_RED = 50,
    EN_SDP_ENCODING_OPUS,
    EN_SDP_ENCODING_RTX,
    EN_SDP_ENCODING_H264_SVC,
    EN_SDP_ENCODING_H264_BP,
    EN_SDP_ENCODING_UNKNOWN
} EN_SDP_ENCODING_TYPE;

/* precondition-type type */
typedef enum EN_SDP_PRECOND_TYPE_TYPE
{
    EN_SDP_PRECOND_TYPE_QOS,
    EN_SDP_PRECOND_TYPE_UNKNOWN
} EN_SDP_PRECOND_TYPE_TYPE;

/* strength-tag type */
typedef enum EN_SDP_STREN_TYPE
{
    EN_SDP_STREN_MAND,
    EN_SDP_STREN_OPT,
    EN_SDP_STREN_NONE,
    EN_SDP_STREN_FAIL,
    EN_SDP_STREN_UNKNWON
} EN_SDP_STREN_TYPE;

/* status-type type */
typedef enum EN_SDP_STA_TYPE_TYPE
{
    EN_SDP_STA_TYPE_E2E,
    EN_SDP_STA_TYPE_LOCAL,
    EN_SDP_STA_TYPE_RMT
} EN_SDP_STA_TYPE_TYPE;

/* direction-tag type */
typedef enum EN_SDP_DIRECT_TYPE_TYPE
{
    EN_SDP_DIRECT_TYPE_NONE,
    EN_SDP_DIRECT_TYPE_SEND,
    EN_SDP_DIRECT_TYPE_RECV,
    EN_SDP_DIRECT_TYPE_SENDRECV
} EN_SDP_DIRECT_TYPE_TYPE;

/* iLBC mode type */
typedef enum EN_SDP_ILBC_TYPE
{
    EN_SDP_ILBC_20,
    EN_SDP_ILBC_30
} EN_SDP_ILBC_TYPE;

/* group semantics type */
typedef enum EN_SDP_GRP_SEMAN_TYPE
{
    EN_SDP_GRP_SEMAN_LS,             /* rfc3388 */
    EN_SDP_GRP_SEMAN_FID,            /* rfc3388 */
    EN_SDP_GRP_SEMAN_SRF,            /* rfc3524 */
    EN_SDP_GRP_SEMAN_FEC,            /* rfc4756 */
    EN_SDP_GRP_SEMAN_FEC_FR          /* rfc5956 */
} EN_SDP_GRP_SEMAN_TYPE;

/* ssrc attributes type */
typedef enum EN_SDP_SSRC_ATTR_TYPE
{
    EN_SDP_SSRC_ATTR_CNAME,          /* cname */
    EN_SDP_SSRC_ATTR_PREV_SSRC,      /* previous-ssrc */
    EN_SDP_SSRC_ATTR_FMTP,           /* fmtp */
    EN_SDP_SSRC_ATTR_LABEL,          /* label */
    EN_SDP_SSRC_ATTR_MSLABEL,        /* mslabel */
    EN_SDP_SSRC_ATTR_UNKNOWN
} EN_SDP_SSRC_ATTR_TYPE;

/* mediacnt attributes type */
typedef enum EN_SDP_MCNT_ATTR_TYPE
{
    EN_SDP_MCNT_ATTR_SLIDES,          /* slides */
    EN_SDP_MCNT_ATTR_SPEAKER,         /* speaker */
    EN_SDP_MCNT_ATTR_S1,              /* s1 */
    EN_SDP_MCNT_ATTR_MAIN,            /* main*/
    EN_SDP_MCNT_ATTR_ALT,             /* alt*/
    EN_SDP_MCNT_ATTR_UNKNOWN
} EN_SDP_MCNT_ATTR_TYPE;

/* attribute type */
typedef enum EN_SDP_ATTR_TYPE
{
    EN_SDP_ATTR_CAT = 0,            
    EN_SDP_ATTR_KEYWDS,         
    EN_SDP_ATTR_TOOL,           
    EN_SDP_ATTR_PTIME,          
    EN_SDP_ATTR_MAXPTIME,            /* rfc4566 */
    EN_SDP_ATTR_RECVONLY = 5,      
    EN_SDP_ATTR_SENDRECV,       
    EN_SDP_ATTR_SENDONLY,       
    EN_SDP_ATTR_INACTIVE,            /* rfc4566 */   
    EN_SDP_ATTR_ORIENT,         
    EN_SDP_ATTR_CONF = 10,           
    EN_SDP_ATTR_CHRSET,      
    EN_SDP_ATTR_SDPLANG,    
    EN_SDP_ATTR_LANG,         
    EN_SDP_ATTR_FRAMERATE,      
    EN_SDP_ATTR_QUALITY = 15,       
    EN_SDP_ATTR_FMTP,   
    EN_SDP_ATTR_RTPMAP,
    EN_SDP_ATTR_CUR_STA,             /* rfc3312 */
    EN_SDP_ATTR_DES_STA,             /* rfc3312 */
    EN_SDP_ATTR_CNF_STA = 20,        /* rfc3312 */
    EN_SDP_ATTR_RTCP,                /* rfc3605 */
    EN_SDP_ATTR_MID,                 /* rfc5888 */
    EN_SDP_ATTR_GROUP,               /* rfc5888 */
    EN_SDP_ATTR_PHONE_CONTEXT,       /* rfc2848 */
    EN_SDP_ATTR_CLIR = 25,           /* rfc2848 */
    EN_SDP_ATTR_Q763_NATURE,         /* rfc2848 */
    EN_SDP_ATTR_Q763_PLAN,           /* rfc2848 */
    EN_SDP_ATTR_Q763_INN,            /* rfc2848 */
    EN_SDP_ATTR_REQUIRE,             /* rfc2848 */
    EN_SDP_ATTR_CONTROL = 30,        /* rfc2326 */
    EN_SDP_ATTR_SSRC,                /* rfc5576 */
    EN_SDP_ATTR_SSRC_GROUP,          /* rfc5576 */
    EN_SDP_ATTR_CONTENT,             /* rfc4796 */
    EN_SDP_ATTR_CSUP,                /* rfc5939 */
    EN_SDP_ATTR_CREQ = 35,           /* rfc5939 */
    EN_SDP_ATTR_ACAP,                /* rfc5939 */
    EN_SDP_ATTR_TCAP,                /* rfc5939 */
    EN_SDP_ATTR_PCFG,                /* rfc5939 */
    EN_SDP_ATTR_ACFG,                /* rfc5939 */
    EN_SDP_ATTR_PATH = 40,           /* rfc4975 */
    EN_SDP_ATTR_ACPT_TYPES,          /* rfc4975 */
    EN_SDP_ATTR_ACPT_WPD_TYPES,      /* rfc4975 */
    EN_SDP_ATTR_MAX_SIZE,            /* rfc4975 */
    EN_SDP_ATTR_FILE_SLT,            /* rfc5547 */
    EN_SDP_ATTR_FILE_DISP = 45,      /* rfc5547 */
    EN_SDP_ATTR_FILE_TR_ID,          /* rfc5547 */
    EN_SDP_ATTR_FILE_DATE,           /* rfc5547 */
    EN_SDP_ATTR_FILE_ICON,           /* rfc5547 */
    EN_SDP_ATTR_FILE_RANGE,          /* rfc5547 */
    EN_SDP_ATTR_CONNECTION = 50,     /* rfc4145 */
    EN_SDP_ATTR_SETUP,               /* rfc4145 */
    EN_SDP_ATTR_FRAMESIZE,           /* rfc6064 */
    EN_SDP_ATTR_RTCP_MUX,            /* rfc5761 */
    EN_SDP_ATTR_IMG_ATTR,            /* rfc6236 */
    EN_SDP_ATTR_RTCP_FB = 55,        /* rfc4585 */
    EN_SDP_ATTR_CAND,                /* rfc5245 */
    EN_SDP_ATTR_RMT_CAND,            /* rfc5245 */
    EN_SDP_ATTR_ICE_LITE,            /* rfc5245 */
    EN_SDP_ATTR_ICE_MISMATCH,        /* rfc5245 */
    EN_SDP_ATTR_ICE_UFRAG = 60,      /* rfc5245 */
    EN_SDP_ATTR_ICE_PWD,             /* rfc5245 */
    EN_SDP_ATTR_ICE_OPTS,            /* rfc5245 */
    EN_SDP_ATTR_REC_PREF,            /* tools.ietf.org/id/draft-ietf-siprec-protocol-02.txt */
    EN_SDP_ATTR_FINGERPRINT,         /* rfc4572 */
    EN_SDP_ATTR_CRYPTO = 65,         /* rfc4568 */
    EN_SDP_ATTR_EXTMAP,              /* rfc5285 */
    EN_SDP_ATTR_MSRP_CEMA,           /* rfc6714 */
    EN_SDP_ATTR_UNKNOWN         
} EN_SDP_ATTR_TYPE;

/* attribute orient */
typedef enum EN_SDP_ATTR_ORIENT_TYPE
{
    EN_SDP_ATTR_ORIENT_PORTRAIT,
    EN_SDP_ATTR_ORIENT_LANDSCAPE,
    EN_SDP_ATTR_ORIENT_SEASCAPE
} EN_SDP_ATTR_ORIENT_TYPE;

/* attribute record prefrence */
typedef enum EN_SDP_ATTR_REC_PREF_TYPE
{
    EN_SDP_ATTR_REC_PREF_ON,
    EN_SDP_ATTR_REC_PREF_OFF,
    EN_SDP_ATTR_REC_PREF_PAUSE,
    EN_SDP_ATTR_REC_PREF_NO_PREF
} EN_SDP_ATTR_REC_PREF_TYPE;

/* conference type */
typedef enum EN_SDP_ATTR_CONF_TYPE
{
    EN_SDP_ATTR_CONF_BROADCAST,
    EN_SDP_ATTR_CONF_MEETING,
    EN_SDP_ATTR_CONF_MODERATED,
    EN_SDP_ATTR_CONF_TEST,
    EN_SDP_ATTR_CONF_H332,
    EN_SDP_ATTR_CONF_UNKNOWN
} EN_SDP_ATTR_CONF_TYPE;

/* attribute mediacnt */
typedef enum EN_SDP_ATTR_MCNT_TYPE
{
    EN_SDP_ATTR_MCNT_SLIDES,
    EN_SDP_ATTR_MCNT_SPEAKER,
    EN_SDP_ATTR_MCNT_S1,
    EN_SDP_ATTR_MCNT_MAIN,
    EN_SDP_ATTR_MCNT_ALT
} EN_SDP_ATTR_MCNT_TYPE;

/* delete-attributes */
typedef enum EN_SDP_DEL_ATTR_TYPE
{
    EN_SDP_DEL_ATTR_M,
    EN_SDP_DEL_ATTR_S,
    EN_SDP_DEL_ATTR_MS
} EN_SDP_DEL_ATTR_TYPE;

/* pcfg pot-config */
typedef enum EN_SDP_POT_CFG_TYPE
{
    EN_SDP_POT_CFG_A,
    EN_SDP_POT_CFG_T,
    EN_SDP_POT_CFG_EXT
} EN_SDP_POT_CFG_TYPE;

/* host type */
typedef enum EN_SDP_HOST_TYPE
{
    EN_SDP_HOST_REGNAME,
    EN_SDP_HOST_IPV4,
    EN_SDP_HOST_IPV6,
    EN_SDP_HOST_IPVFUTURE
} EN_SDP_HOST_TYPE;

/* transport type */
typedef enum EN_SDP_TRANS_TYPE
{
    EN_SDP_TRANS_TCP,
    EN_SDP_TRANS_OTHER   
} EN_SDP_TRANS_TYPE;

/* MSRP-URI type */
typedef enum EN_SDP_MSRP_URI_TYPE
{
    EN_SDP_MSRP_URI_MSRP,
    EN_SDP_MSRP_URI_MSRPS    
} EN_SDP_MSRP_URI_TYPE;

/* sdp type type */
typedef enum EN_SDP_TYPE_TYPE
{
    EN_SDP_TYPE_TEXT, 
    EN_SDP_TYPE_IMAGE,
    EN_SDP_TYPE_AUDIO,
    EN_SDP_TYPE_VIDEO,
    EN_SDP_TYPE_APP,  
    EN_SDP_TYPE_MSG,  
    EN_SDP_TYPE_MULTI,
    EN_SDP_TYPE_EXT        
} EN_SDP_TYPE_TYPE;

/* sdp subtype. */
typedef enum EN_SDP_SUBTYPE_TYPE
{
    EN_SDP_SUBTYPE_ALL,              /* all */
    EN_SDP_SUBTYPE_PLAIN,            /* text plain */
    EN_SDP_SUBTYPE_RICHTEXT,         /* text richtext */
    EN_SDP_SUBTYPE_CPIM,             /* message cpim */
    EN_SDP_SUBTYPE_HTML,             /* text html */
    EN_SDP_SUBTYPE_OCTET_STRM,       /* application arbitrary binary data(bin, dll, exe) */
    EN_SDP_SUBTYPE_MSWORLD,          /* application doc */
    EN_SDP_SUBTYPE_XML,              /* application xml */
    EN_SDP_SUBTYPE_XML_DTD,          /* application dtd */
    EN_SDP_SUBTYPE_PDF,              /* application pdf */
    EN_SDP_SUBTYPE_ZIP,              /* application zip */
    EN_SDP_SUBTYPE_X_GZIP,           /* application gizp */
    EN_SDP_SUBTYPE_X_RAR,            /* application rar */
    EN_SDP_SUBTYPE_X_TAR,            /* application tarball */
    EN_SDP_SUBTYPE_BMP,              /* image bmp */
    EN_SDP_SUBTYPE_GIF,              /* image gif */
    EN_SDP_SUBTYPE_JPEG,             /* image jpeg */
    EN_SDP_SUBTYPE_PNG,              /* image png */
    EN_SDP_SUBTYPE_TIFF,             /* image tiff */
    EN_SDP_SUBTYPE_H261,             /* video h261 */
    EN_SDP_SUBTYPE_H263,             /* video h263 */
    EN_SDP_SUBTYPE_H263_1998,        /* video h263-1998 */
    EN_SDP_SUBTYPE_H263_2000,        /* video h263-2000 */
    EN_SDP_SUBTYPE_H264,             /* video h264 */
    EN_SDP_SUBTYPE_MPEG,             /* video mpeg */
    EN_SDP_SUBTYPE_MIXED,            /* mixed */
    EN_SDP_SUBTYPE_IMDN_XML,         /* imdn+xml CTC */
    EN_SDP_SUBTYPE_FTHTTP_XML,       /* application fthttp CTC */
    EN_SDP_SUBTYPE_IM_ISCOMPOSING_XML, /* im-iscomposing+xml CTC */
    EN_SDP_SUBTYPE_CONFERENCE_INFO_XML, /* conference-info+xml CTC */
    EN_SDP_SUBTYPE_3GPP,             /* 3GPP */
    EN_SDP_SUBTYPE_MP4,              /* MP4 */
    EN_SDP_SUBTYPE_AAC,              /* AAC */
    EN_SDP_SUBTYPE_AMR,              /* AMR */
    EN_SDP_SUBTYPE_RPL_XML,          /* rcspushlocation+xml */
    EN_SDP_SUBTYPE_VND_GSMA_RPL_XML, /* vnd.gsma.rcspushlocation+xml */
    EN_SDP_SUBTYPE_VCARD,            /* vcard */
    EN_SDP_SUBTYPE_ENCALL_XML,    /* vnd.gsma.encall+xml */
    EN_SDP_SUBTYPE_SHAREDMAP_XML,    /* vnd.gsma.sharedmap+xml */
    EN_SDP_SUBTYPE_SHAREDSKETCH_XML,    /* vnd.gsma.sharedsketch+xml */
    EN_SDP_SUBTYPE_BOTMSG_JSON,    /* vnd.gsma.botmessage.v1.0+json */
    EN_SDP_SUBTYPE_BOTSUG_JSON,    /* vnd.gsma.botsuggestion.v1.0+json */
    EN_SDP_SUBTYPE_BOTRPS_JSON,    /* vnd.gsma.botsuggestion.response.v1.0+json */
    EN_SDP_SUBTYPE_BOTSHARED_JSON,    /* vnd.gsma.botsharedclientdata.v1.0+json */
    EN_SDP_SUBTYPE_EXT      
} EN_SDP_SUBTYPE_TYPE;
    
/* sdp selector type. */
typedef enum EN_SDP_SELECTOR_TYPE
{
    EN_SDP_SELECTOR_FNAME,       
    EN_SDP_SELECTOR_FSIZE,        
    EN_SDP_SELECTOR_FTYPE,
    EN_SDP_SELECTOR_FHASH,
    EN_SDP_SELECTOR_UNKNOWN
} EN_SDP_SELECTOR_TYPE;

/* sdp hash-type. */
typedef enum EN_SDP_HASH_TYPE
{
    EN_SDP_HASH_SHA1,       
    EN_SDP_HASH_EXT    
} EN_SDP_HASH_TYPE;

/* sdp date-parm. */
typedef enum EN_SDP_DATE_PARM_TYPE
{
    EN_SDP_DATE_PARM_CREAT,       
    EN_SDP_DATE_PARM_MDF,
    EN_SDP_DATE_PARM_READ,
    EN_SDP_DATE_PARM_UNKNOWN
} EN_SDP_DATE_PARM_TYPE;

/* sdp URL schemes. */
typedef enum EN_SDP_URL_TYPE
{
    EN_SDP_URL_CID,       
    EN_SDP_URL_MID,
    EN_SDP_URL_UNKNOWN
} EN_SDP_URL_TYPE;

/* sdp wkday type. */
typedef enum EN_SDP_WKDAY_TYPE
{
    EN_SDP_WKDAY_MON,                
    EN_SDP_WKDAY_TUE,                
    EN_SDP_WKDAY_WEB,                
    EN_SDP_WKDAY_THU,                
    EN_SDP_WKDAY_FRI,                
    EN_SDP_WKDAY_SAT,                
    EN_SDP_WKDAY_SUN                 
} EN_SDP_WKDAY_TYPE;

/* sdp month type. */
typedef enum EN_SDP_MONTH_TYPE
{
    EN_SDP_MONTH_JAN,                
    EN_SDP_MONTH_FEB,                
    EN_SDP_MONTH_MAR,                
    EN_SDP_MONTH_APR,                
    EN_SDP_MONTH_MAY,                
    EN_SDP_MONTH_JUN,                
    EN_SDP_MONTH_JUL,                
    EN_SDP_MONTH_AUG,                
    EN_SDP_MONTH_SEP,                
    EN_SDP_MONTH_OCT,                
    EN_SDP_MONTH_NOV,                
    EN_SDP_MONTH_DEC                 
} EN_SDP_MONTH_TYPE;

/* sdp zone type. */
typedef enum EN_SDP_ZONE_TYPE
{    
    EN_SDP_ZONE_UT,                  
    EN_SDP_ZONE_GMT,                 
    EN_SDP_ZONE_EST,                 
    EN_SDP_ZONE_EDT,                 
    EN_SDP_ZONE_CST,                 
    EN_SDP_ZONE_CDT,    
    EN_SDP_ZONE_MST,    
    EN_SDP_ZONE_MDT,    
    EN_SDP_ZONE_PST,    
    EN_SDP_ZONE_PDT,    
    EN_SDP_ZONE_EXT    
} EN_SDP_ZONE_TYPE;

/* sdp connection type. */
typedef enum EN_SDP_CONN_TYPE
{
    EN_SDP_CONN_NEW,       
    EN_SDP_CONN_EXIST,
    EN_SDP_CONN_UNKNOWN
} EN_SDP_CONN_TYPE;

/* sdp setup type. */
typedef enum EN_SDP_SETUP_TYPE
{
    EN_SDP_SETUP_ACTIVE,       
    EN_SDP_SETUP_PASSIVE,
    EN_SDP_SETUP_ACTPASS,       
    EN_SDP_SETUP_HOLDCONN,
    EN_SDP_SETUP_UNKNOWN
} EN_SDP_SETUP_TYPE;

/* sdp rtcp-fb type */
typedef enum EN_SDP_RTCP_FB_TYPE
{
    EN_SDP_RTCP_FB_ACK,
    EN_SDP_RTCP_FB_NACK,
    EN_SDP_RTCP_FB_TRR_INT,
    EN_SDP_RTCP_FB_CCM,              /* rfc5104 */
    EN_SDP_RTCP_FB_UNKNOWN
} EN_SDP_RTCP_FB_TYPE;

/* sdp rtcp-fb parameter type */
typedef enum EN_SDP_RTCP_FB_PARM_TYPE
{
    EN_SDP_RTCP_FB_PARM_APP,
    EN_SDP_RTCP_FB_PARM_RPSI,
    EN_SDP_RTCP_FB_PARM_PLI,
    EN_SDP_RTCP_FB_PARM_SLI,
    EN_SDP_RTCP_FB_PARM_FIR,         /* rfc5104 */
    EN_SDP_RTCP_FB_PARM_TMMBR,       /* rfc5104 */
    EN_SDP_RTCP_FB_PARM_TSTR,        /* rfc5104 */
    EN_SDP_RTCP_FB_PARM_VBCM,        /* rfc5104 */
    EN_SDP_RTCP_FB_PARM_UNKNOWN
} EN_SDP_RTCP_FB_PARM_TYPE;

/* sdp candidate transport type */
typedef enum EN_SDP_CAND_TPT_TYPE
{
    EN_SDP_CAND_TPT_UDP,
    EN_SDP_CAND_TPT_EXT
} EN_SDP_CAND_TPT_TYPE;

/* sdp candidate type */
typedef enum EN_SDP_CAND_TYPE
{
    EN_SDP_CAND_HOST,
    EN_SDP_CAND_SRV_RFLX,
    EN_SDP_CAND_PEER_RFLX,
    EN_SDP_CAND_RELAY,
    EN_SDP_CAND_EXT
} EN_SDP_CAND_TYPE;

/* sdp fingerprint hash functions type */
typedef enum EN_SDP_FGRPRNT_HASH_TYPE
{
    EN_SDP_FGRPRNT_HASH_SHA1,
    EN_SDP_FGRPRNT_HASH_SHA224,
    EN_SDP_FGRPRNT_HASH_SHA256,
    EN_SDP_FGRPRNT_HASH_SHA384,
    EN_SDP_FGRPRNT_HASH_SHA512,
    EN_SDP_FGRPRNT_HASH_MD5,
    EN_SDP_FGRPRNT_HASH_MD2,
    EN_SDP_FGRPRNT_HASH_EXT,         /* only come from updates to rfc3279 */
} EN_SDP_FGRPRNT_HASH_TYPE;

/* sdp crypto-suite type, rfc4568 */
typedef enum EN_SDP_CRYPTO_SUITE_TYPE
{
    EN_SDP_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_80,
    EN_SDP_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_32,
    EN_SDP_CRYPTO_SUITE_F8_128_HMAC_SHA1_80,
#ifdef ZOS_SUPT_CNPC
    EN_SDP_CRYPTO_SUITE_SRTP_ZUC_EEA3,
#endif
    EN_SDP_CRYPTO_SUITE_EXT,
} EN_SDP_CRYPTO_SUITE_TYPE;

/* sdp crypto-key-method type, rfc4568 */
typedef enum EN_SDP_CRYPTO_KEY_METHOD_TYPE
{
    EN_SDP_CRYPTO_KEY_METHOD_INLINE,
    EN_SDP_CRYPTO_KEY_METHOD_EXT,
} EN_SDP_CRYPTO_KEY_METHOD_TYPE;

/* sdp crypto-session-param type, rfc4568 */
typedef enum EN_SDP_CRYPTO_SESS_TYPE
{
    EN_SDP_CRYPTO_SESS_KDR,
    EN_SDP_CRYPTO_SESS_UNENCRYPTED_SRTP,
    EN_SDP_CRYPTO_SESS_UNENCRYPTED_SRTCP,
    EN_SDP_CRYPTO_SESS_UNAUTHENTICATED_SRTP,
    EN_SDP_CRYPTO_SESS_FEC_ORDER,
    EN_SDP_CRYPTO_SESS_FEC_KEY,
    EN_SDP_CRYPTO_SESS_FEC_WSH,
    EN_SDP_CRYPTO_SESS_EXT,
} EN_SDP_CRYPTO_SESS_TYPE;

/* sdp crypto-session-fec-order type, rfc4568 */
typedef enum EN_SDP_CRYPTO_SESS_FEC_ORDER_TYPE
{
    EN_SDP_CRYPTO_SESS_FEC_ORDER_FEC_SRTP,
    EN_SDP_CRYPTO_SESS_FEC_ORDER_SRTP_FEC,
} EN_SDP_CRYPTO_SESS_FEC_ORDER_TYPE;

/***************************** rfc2327 typedefs *************************/

/* sdp fmt type */
typedef ST_ZOS_SSTR ST_SDP_FMT;

/* sdp list of fmt */
ABNF_TYPEDEF_LIST(SDP_FMT); 

/* sdp unicast-address(rfc4566) */
typedef struct tagSDP_UCAST_ADDR
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucType;                   /* addr type EN_SDP_UCAST_ADDR_TYPE */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    union
    {
        ZUINT iIpv4;                 /* IP4-address */
        ZUCHAR aucIpv6[ZINET_IPV6_ADDR_SIZE]; /* IP6-address */
        ST_ZOS_SSTR stFqdn;          /* FQDN */
        ST_ZOS_SSTR stExtn;          /* extn-addr */
    } u;
} ST_SDP_UCAST_ADDR;

/* sdp IP4-multicast */
typedef struct tagSDP_MCAST_IP4
{
    ZUCHAR ucNumAddrPres;            /* number of address present flag */
    ZUCHAR ucTtl;                    /* ttl */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUINT iNumAddr;                  /* integer, number of address */
    ZUINT iIpv4;                     /* IPv4 address */     
} ST_SDP_MCAST_IP4;

/* sdp IP6-multicast */
typedef struct tagSDP_MCAST_IP6
{
    ZUCHAR ucNumAddrPres;            /* number of address present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iNumAddr;                  /* integer, number of address */
    ZUCHAR aucIpv6[ZINET_IPV6_ADDR_SIZE]; /* IPv6 address */
} ST_SDP_MCAST_IP6;

/* sdp connection-address */
typedef struct tagSDP_CONN_ADDR
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucType;                   /* addr type EN_SDP_CONN_ADDR_TYPE */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    union
    {
        ZUINT iIpv4;                 /* IP4-address */
        ST_SDP_MCAST_IP4 stMIpv4;    /* IP4-multicast */
        ZUCHAR aucIpv6[ZINET_IPV6_ADDR_SIZE]; /* IP6-address */
        ST_SDP_MCAST_IP6 stMIpv6;    /* IP6-multicast */
        ST_ZOS_SSTR stFqdn;          /* FQDN */
        ST_ZOS_SSTR stTnAddr;        /* telephone network address (RFC2848)*/
        ST_ZOS_SSTR stExtn;          /* extn-addr */
    } u;
} ST_SDP_CONN_ADDR;

/* sdp typed-time */
typedef struct tagSDP_TYPED_TIME
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucUnitPres;               /* unit present flag */
    ZUCHAR ucUnit;                   /* fixed-len-time-unit EN_SDP_TIME_UNIT_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUINT iVal;                      /* time value */
} ST_SDP_TYPED_TIME;

/* list of typed-time */
ABNF_TYPEDEF_LIST(SDP_TYPED_TIME);

/* attribute unknown string */
typedef struct tagSDP_ATTR_STR
{
    ZUCHAR ucValPres;                /* att-value present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stField;             /* att-field */
    ST_ZOS_SSTR stVal;               /* att-value */
} ST_SDP_ATTR_STR;

/* attribute conference type */
typedef struct tagSDP_ATTR_CONF
{
    ZUCHAR ucType;                   /* known media EN_SDP_ATTR_CONF_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknown;           /* unknown conference type */
} ST_SDP_ATTR_CONF;

/* attribute format parameters */
typedef struct tagSDP_ATTR_FMTP
{
    ZUCHAR ucType;                   /* known */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknown;           /* unknown conference type */
} ST_SDP_ATTR_FMTP;

/* sdp zone-adjustments */
typedef struct tagSDP_ZF
{
    ZUCHAR ucDashPres;               /* dahs '-' present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iTime;                     /* time */
    ST_SDP_TYPED_TIME stTypedTime;   /* typed-time */
} ST_SDP_ZF;

/* list of zone-adjustment fields */
ABNF_TYPEDEF_LIST(SDP_ZF);

/* sdp repeat-fields */
typedef struct tagSDP_RF
{
    ST_SDP_TYPED_TIME stRepInterval; /* repeat interval */
    ST_SDP_TYPED_TIME stActDur;      /* active duration */
    ST_SDP_TYPED_TIME_LST stOffsetLst;/* list of offsets from start-time */
} ST_SDP_RF; 

/* list of repeat-fields */
ABNF_TYPEDEF_LIST(SDP_RF);

/* sdp media-field */
typedef struct tagSDP_MF
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucPortNumPres;            /* port integer present flag */
    ZUCHAR ucMediaType;              /* known media EN_SDP_MEDIA_TYPE */
    ZUCHAR ucProtoType;              /* known proto EN_SDP_PROTO_TYPE */
    ZUSHORT wPort;                   /* port */
    ZUSHORT wPortNum;                /* port integer */
    ZUINT iStrmId;                   /* stream id */
    ST_ZOS_SSTR stMedia;             /* unknown media */
    ST_ZOS_SSTR stProto;             /* unknown proto */
    ST_SDP_FMT_LST stFmtLst;         /* fmt list */
} ST_SDP_MF;

/* sdp proto-version field */
typedef struct tagSDP_VF
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUSHORT wVer;                    /* version no */
} ST_SDP_VF;

/* sdp origin-field */
typedef struct tagSDP_OF
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucBigSessIdPres;          /* big sess-id */
    ZUCHAR ucBigSessVerPres;         /* big sess-version */
    ZUCHAR ucNetType;                /* nettype EN_SDP_NET_TYPE */
    ZUCHAR ucAddrType;               /* addrtype EN_SDP_ADDR_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUserName;          /* username */
    ST_ZOS_SSTR stSessId;            /* sess-id for larger than 32 bits */
    ST_ZOS_SSTR stSessVer;           /* sess-version for larger than 32 bits */
    ZUINT iSessId;                   /* sess-id */
    ZUINT iSessVer;                  /* sess-version */
    ST_SDP_UCAST_ADDR stAddr;        /* unicast-address */
} ST_SDP_OF;

/* sdp session-name-field */
typedef struct tagSDP_SF
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stSessName;          /* text */
} ST_SDP_SF;

/* sdp information-field */
typedef struct tagSDP_IF
{   
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stInfo;              /* text */
} ST_SDP_IF;

/* sdp uri-field */
typedef struct tagSDP_UF
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUri;               /* uri */
} ST_SDP_UF;

/* sdp email-fields */
typedef struct tagSDP_EF
{
    ST_ZOS_SSTR stEmailAddr;         /* email address */
} ST_SDP_EF;

/* list of email-field */
ABNF_TYPEDEF_LIST(SDP_EF);

/* sdp phone-fields */
typedef struct tagSDP_PF
{
    ST_ZOS_SSTR stPhoneNumber;       /* phone-number */
} ST_SDP_PF;

/* list of phone-field */
ABNF_TYPEDEF_LIST(SDP_PF);

/* sdp connection-field */
typedef struct tagSDP_CF
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucNetType;                /* nettype EN_SDP_NET_TYPE */
    ZUCHAR ucAddrType;               /* addrtype EN_SDP_ADDR_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_SDP_CONN_ADDR stAddr;         /* connection-address */
} ST_SDP_CF;

/* list of connection-field */
ABNF_TYPEDEF_LIST(SDP_CF); 

/* sdp bwtype */
typedef struct tagSDP_BW_TYPE
{
    ZUCHAR ucType;                   /* bw type EN_SDP_BWTYPE_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknown;           /* unknown type */
} ST_SDP_BW_TYPE;

/* sdp bandwidth-fields */
typedef struct tagSDP_BF
{
    ST_SDP_BW_TYPE stBwType;         /* bwtype */
    ZUINT iBwVal;                    /* bandwidth value */
} ST_SDP_BF;

/* list of bandwidth-field */
ABNF_TYPEDEF_LIST(SDP_BF);

/* sdp time-fields */
typedef struct tagSDP_TF
{
    ZUINT iStartTime;                /* start-time */         
    ZUINT iStopTime;                 /* stop-time */
    ST_SDP_RF_LST stRepLst;          /* repeat-fields */
} ST_SDP_TF;

/* list of time-field */
ABNF_TYPEDEF_LIST(SDP_TF);

/* sdp session time */
typedef struct tagSDP_SESS_TIME
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucZPres;                  /* 'z=' present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_SDP_TF_LST stTimeLst;         /* time-field list */
    ST_SDP_ZF_LST stZoneAdjLst;      /* zone-adjustment list */
} ST_SDP_SESS_TIME;

/* sdp key-field */
typedef struct tagSDP_KF
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR ucDataPres;               /* key-data present flag */
    ZUCHAR ucKeyType;                /* key-type EN_SDP_KEY_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_ZOS_SSTR stData;              /* key-data */
} ST_SDP_KF;

/* sdp fmtp rfc2733 -- for Generic Forward Error Correction */
typedef struct tagSDP_FMTP_2733_FEC
{
    ZUCHAR ucNetType;                /* nettype EN_SDP_NET_TYPE */
    ZUCHAR ucAddrType;               /* addrtype EN_SDP_ADDR_TYPE */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUINT iPort;                     /* port */
    ST_SDP_CONN_ADDR stAddr;         /* connection-address */
} ST_SDP_FMTP_2733_FEC;

/* sdp fmtp rfc2733 -- for Generic Forward Error Correction */
typedef struct tagSDP_FMTP_2733_RED
{
    ZUCHAR ucPayloadCnt;             /* count of payload in red */
    ZUCHAR aucPayload[15];           /* payload list in red */
} ST_SDP_FMTP_2733_RED;

/* sdp fmtp rfc2733 -- for Generic Forward Error Correction */
typedef struct tagSDP_FMTP_2733
{
    ZUCHAR ucRed;                    /* red flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_SDP_FMTP_2733_FEC stFec;  /* fec */
        ST_SDP_FMTP_2733_RED stRed;  /* red */
    } u;
} ST_SDP_FMTP_2733;

/* sdp fmtp rfc2833 value */
typedef struct tagSDP_2833_VAL
{
    ZUSHORT wValA;                   /* value a */
    ZUSHORT wValB;                   /* value b */
} ST_SDP_2833_VAL;

/* list of rfc2833 value*/
ABNF_TYPEDEF_LIST(SDP_2833_VAL);

/* sdp fmtp rfc2833 -- for DTMF Digits */
typedef struct tagSDP_FMTP_2833
{
    ST_SDP_2833_VAL_LST stValLst;    /* value list */
} ST_SDP_FMTP_2833;

/* sdp fmtp rfc3640 parameter */
typedef struct tagSDP_3640_PARM
{
    ZUCHAR ucType;                   /* Size type EN_SDP_3640_PARM_TYPE */
    ZUCHAR ucIntValPres;             /* integer value present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stVal;           /* string value */
        ZUINT iVal;                  /* integer value */
    } u;
} ST_SDP_3640_PARM;

/* list of rfc3640 parameter*/
ABNF_TYPEDEF_LIST(SDP_3640_PARM);

/* sdp fmtp rfc3640 -- for MPEG-4 Elementary Streams */
typedef struct tagSDP_FMTP_3640
{
    ST_SDP_3640_PARM_LST stParmLst;  /* parameter list */
} ST_SDP_FMTP_3640;

/* sdp fmtp rfc3952 -- for iLBC mode */
typedef struct tagSDP_FMTP_3952
{
    ZUCHAR ucMode;                   /* iLBC mode type EN_SDP_ILBC_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
} ST_SDP_FMTP_3952;

/* sdp fmtp h261 Size */
typedef struct tagSDP_H261_SIZE
{
    ZUCHAR ucQcifPres;               /* QCIF present flag */
    ZUCHAR ucMpi;                    /* mpi */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
} ST_SDP_H261_SIZE;

/* sdp fmtp h261 option */
typedef struct tagSDP_H261_OPT
{
    ZUCHAR ucAnxPres;                /* Annex present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_H261_SIZE stSize;         
} ST_SDP_H261_OPT;

/* list of fmtp h261 option*/
ABNF_TYPEDEF_LIST(SDP_H261_OPT);

/* sdp fmtp h263 Size */
typedef struct tagSDP_H263_SIZE
{
    ZUCHAR ucType;                   /* Size type EN_SDP_H263_SIZE_TYPE */
    ZUCHAR ucMpi;                    /* mpi */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUSHORT wXMax;                   /* XMAX */
    ZUSHORT wYMax;                   /* YMAX */
} ST_SDP_H263_SIZE;

/* sdp fmtp h263 Annex */
typedef struct tagSDP_H263_ANX
{
    ZUCHAR ucType;                   /* Annex type EN_SDP_H263_ANX_TYPE */
    ZUCHAR ucValPres;                /* value present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stAnxStr;            /* annex string */
} ST_SDP_H263_ANX;

/* sdp fmtp h263 Params */
typedef struct tagSDP_H263_PARM
{
    ZUCHAR ucType;                   /* Annex type EN_SDP_H263_PARM_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUSHORT wValA;                   /* vaule a */
    ZUSHORT wValB;                   /* vaule b */
    ST_ZOS_SSTR stExtName;           /* extension name */
    ST_ZOS_SSTR stExtVal;            /* extension value */
} ST_SDP_H263_PARM;

/* sdp fmtp h263 option */
typedef struct tagSDP_H263_OPT
{
    ZUCHAR ucType;                   /* option type EN_SDP_H263_OPT_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union 
    {
        ST_SDP_H263_SIZE stSize;     /* Size */
        ST_SDP_H263_ANX stAnx;       /* Annex */
        ST_SDP_H263_PARM stParm;     /* Params */
    } u;    
} ST_SDP_H263_OPT;

/* list of fmtp h263 option*/
ABNF_TYPEDEF_LIST(SDP_H263_OPT);

/* sdp fmtp draft-even-avt-h263-h261-options-00*/
typedef struct tagSDP_FMTP_H26X
{
    ZUCHAR ucH261Pres;               /* h261 present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_SDP_H261_OPT_LST stH261Lst; /* H261_option list */
        ST_SDP_H263_OPT_LST stH263Lst; /* H263_option list */
    } u;
} ST_SDP_FMTP_H26X;

/* sdp fmtp rfc4587 parameter */
typedef struct tagSDP_4587_PARM
{
    ZUCHAR ucType;                   /* parameter type EN_SDP_4587_PARM_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iVal;                      /* parameter value */
} ST_SDP_4587_PARM;

/* list of rfc4587 parameter*/
ABNF_TYPEDEF_LIST(SDP_4587_PARM);

/* sdp fmtp rfc4587 -- for H.261 */
typedef struct tagSDP_FMTP_4587
{
    ST_SDP_4587_PARM_LST stParmLst;  /* parameter list */
} ST_SDP_FMTP_4587;

/* sdp fmtp rfc4629 custom */
typedef struct tagSDP_4629_CUSTOM
{
    ZUCHAR ucMpi;                    /* mpi */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUSHORT wXMax;                   /* XMAX */
    ZUSHORT wYMax;                   /* YMAX */
} ST_SDP_4629_CUSTOM;

/* sdp fmtp rfc4629 Annex */
typedef struct tagSDP_4629_ANX
{
    ZUCHAR ucValPres;                /* value present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stAnxStr;            /* annex string */
} ST_SDP_4629_ANX;

/* sdp fmtp rfc4629 parameter */
typedef struct tagSDP_4629_PARM
{
    ZUCHAR ucType;                   /* parameter type EN_SDP_4629_PARM_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ZUINT iMpi;                  /* mpi */
        ST_SDP_4629_CUSTOM stCustom; /* custom */
        ST_ZOS_SSTR stAnx;           /* annex value */
        ST_ZOS_SSTR stPar;           /* par value */
        ST_ZOS_SSTR stCpcf;          /* cpcf value */
        ZUINT iBpp;                  /* bpp value */
        ZUINT iHdr;                  /* hdr value */
        ZUINT iProf;                 /* profile value */
        ZUINT iLevel;                /* level value */
        ZUINT iInterlace;            /* interlace value */
    } u;
} ST_SDP_4629_PARM;

/* list of rfc4629 parameter*/
ABNF_TYPEDEF_LIST(SDP_4629_PARM);

/* sdp fmtp rfc4629 -- for H.263 */
typedef struct tagSDP_FMTP_4629
{
    ST_SDP_4629_PARM_LST stParmLst;  /* parameter list */
} ST_SDP_FMTP_4629;

/* sdp fmtp rfc3984 parameter */
typedef struct tagSDP_3984_PARM
{
    ZUCHAR ucType;                   /* Size type EN_SDP_3984_PARM_TYPE */
    ZUCHAR ucIntValPres;             /* integer value present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stVal;           /* string value */
        ZUINT iVal;                  /* integer value */
    } u;
} ST_SDP_3984_PARM;

/* list of rfc3984 parameter*/
ABNF_TYPEDEF_LIST(SDP_3984_PARM);

/* sdp fmtp rfc3984 -- for H.264 */
typedef struct tagSDP_FMTP_3984
{
    ST_SDP_3984_PARM_LST stParmLst;  /* parameter list */
} ST_SDP_FMTP_3984;

/* sdp fmtp rfc3267 parameter */
typedef struct tagSDP_3267_PARM
{
    ZUCHAR ucType;                   /* parameter type EN_SDP_3267_PARM_TYPE */
    ZUCHAR ucIntValPres;             /* integer value present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stVal;           /* string value */
        ZUINT iVal;                  /* integer value */
    } u;
} ST_SDP_3267_PARM;

/* list of rfc3267 parameter*/
ABNF_TYPEDEF_LIST(SDP_3267_PARM);

/* sdp fmtp rfc3267 -- for AMR */
typedef struct tagSDP_FMTP_3267
{
    ST_SDP_3267_PARM_LST stParmLst;  /* parameter list */
} ST_SDP_FMTP_3267;

/* sdp fmtp oma-poc -- for TBCP parameter */
typedef struct tagSDP_TBCP_PARM
{
    ZUCHAR ucType;                   /* Size type EN_SDP_TBCP_PARM_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iVal;                      /* parameter value */
} ST_SDP_TBCP_PARM;

/* list of oma-poc -- for TBCP parameter*/
ABNF_TYPEDEF_LIST(SDP_TBCP_PARM);

/* sdp fmtp oma-poc -- for TBCP */
typedef struct tagSDP_FMTP_TBCP
{
    ST_SDP_TBCP_PARM_LST stParmLst;  /* parameter list */
} ST_SDP_FMTP_TBCP;

/* sdp fmtp rfc4588 -- for rtx */
typedef struct tagSDP_FMTP_4588
{
    ZUCHAR ucAptPt;                  /* associated payload type */
    ZUCHAR aucSpare[1];              /* for 32 bits alignment */
    ZUSHORT wRtxTime;                /* retransmission buffer time length */
} ST_SDP_FMTP_4588;

/* sdp fmtp unknown */
typedef struct tagSDP_FMTP_UNKNOWN
{
    ST_ZOS_SSTR stFormat;            /* fmtp format */
    ST_ZOS_SSTR stParms;             /* fmtp format specific parameters */
} ST_SDP_FMTP_UNKNOWN;

/* sdp attribute-fields fmtp */
typedef struct tagSDP_FMTP
{
    ZUCHAR ucType;                   /* fmtp type EN_SDP_FMTP_TYPE */
    ZUCHAR ucPayload;                /* fmtp payload type */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_SDP_FMTP_UNKNOWN stUnknown;   /* unknown */
    union 
    {
        ST_SDP_FMTP_2733 st2733;     /* rfc2733 (Generic) */
        ST_SDP_FMTP_2833 st2833;     /* rfc2833 (DTMF) */
        ST_SDP_FMTP_3640 st3640;     /* rfc3640 (MPEG-4) */
        ST_SDP_FMTP_3952 st3952;     /* rfc3952 (iLBC) */
        ST_SDP_FMTP_H26X stH26X;     /* h261-h263 */
        ST_SDP_FMTP_4587 st4587;     /* rfc4587 (H.261) */
        ST_SDP_FMTP_4629 st4629;     /* rfc4629 (H.263) */
        ST_SDP_FMTP_3984 st3984;     /* rfc3984 (H.264) */
        ST_SDP_FMTP_3267 st3267;     /* rfc3267 (AMR) */
        ST_SDP_FMTP_TBCP stTbcp;     /* oma-poc TBCP */
        ST_SDP_FMTP_4588 st4588;     /* rfc4588 (rtx) */
    } u;
} ST_SDP_FMTP;

/* sdp encoding name */
typedef struct tagSDP_ENCODING_NAME
{
    ZUCHAR ucType;                   /* encoding type EN_SDP_ENCODING_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknown;           /* unknown name */
} ST_SDP_ENCODING_NAME;

/* sdp attribute-fields rtpmap */
typedef struct tagSDP_RTPMAP
{
    ZUCHAR ucPayload;                /* payload value */
    ZUCHAR ucClockRatePres;          /* clock rate present flag */
    ZUCHAR ucParmsPres;              /* encoding parameters present flag */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUINT iClockRate;                /* clock rate */
    ST_SDP_ENCODING_NAME stEncoding; /* encoding name */
    ST_ZOS_SSTR stEncodingParms;     /* encoding parameters */
} ST_SDP_RTPMAP;

/***************************** rfc3312 ***************************/

/* sdp attribute-fields current-status */
typedef struct tagSDP_CUR_STA
{
    ZUCHAR ucPrecondType;            /* precondition EN_SDP_PRECOND_TYPE_TYPE */
    ZUCHAR ucStaType;                /* status-type EN_SDP_STA_TYPE_TYPE */
    ZUCHAR ucDirectType;             /* direction-tag EN_SDP_DIRECT_TYPE_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknownPrecond;    /* unknown precondition-type */
} ST_SDP_CUR_STA;

/* sdp attribute-fields desired-status */
typedef struct tagSDP_DES_STA
{
    ZUCHAR ucPrecondType;            /* precondition EN_SDP_PRECOND_TYPE_TYPE */
    ZUCHAR ucStrenType;              /* strength-type EN_SDP_STREN_TYPE */
    ZUCHAR ucStaType;                /* status-type EN_SDP_STA_TYPE_TYPE */
    ZUCHAR ucDirectType;             /* direction-tag EN_SDP_DIRECT_TYPE_TYPE */
    ST_ZOS_SSTR stUnknownPrecond;    /* unknown precondition-type */
} ST_SDP_DES_STA;

/* sdp attribute-fields confirm-status */
typedef struct tagSDP_CNF_STA
{
    ZUCHAR ucPrecondType;            /* precondition EN_SDP_PRECOND_TYPE_TYPE */
    ZUCHAR ucStaType;                /* status-type EN_SDP_STA_TYPE_TYPE */
    ZUCHAR ucDirectType;             /* direction-tag EN_SDP_DIRECT_TYPE_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknownPrecond;    /* unknown precondition-type */
} ST_SDP_CNF_STA;

/***************************** rfc3605 ***************************/

/* sdp attribute-fields rtcp */
typedef struct tagSDP_RTCP
{
    ZUCHAR ucPortOnlyPres;           /* present flag */
    ZUCHAR ucNetType;                /* nettype EN_SDP_NET_TYPE */
    ZUCHAR ucAddrType;               /* addrtype EN_SDP_ADDR_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUINT iPort;                     /* port */
    ST_SDP_CONN_ADDR stAddr;         /* connection-address */
} ST_SDP_RTCP;

/***************************** rfc3388 ***************************/

/* identification-tag type */
typedef ST_ZOS_SSTR ST_SDP_ID_TAG;

/* list of identification-tag */
ABNF_TYPEDEF_LIST(SDP_ID_TAG); 

/* sdp attribute-fields mid */
typedef struct tagSDP_MID
{
    ST_SDP_ID_TAG stIdTag;           /* identification-tag */
} ST_SDP_MID;

/* sdp attribute-fields group */
typedef struct tagSDP_GRP
{
    ZUCHAR ucType;                   /* semantics type EN_SDP_GRP_SEMAN_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_ID_TAG_LST stTagLst;      /* identification-tag list */
} ST_SDP_GRP;

/***************************** rfc5576 ***************************/
/* sdp ssrc id */
typedef ZUINT ST_SDP_SSRC_ID;

/* list of ssrc id */
ABNF_TYPEDEF_LIST(SDP_SSRC_ID);

/* sdp attribute-fields ssrc */
typedef struct tagSDP_SSRC
{
    ZUCHAR ucAttrType;               /* attribute type EN_SDP_SSRC_ATTR_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    ZUINT iSsrc;                     /* ssrc value */
    union
    {
        ST_ZOS_SSTR stCname;         /* cname */
        ST_SDP_SSRC_ID_LST stPrevSsrc; /* previous-ssrc */
        ST_SDP_FMTP stFmtp;          /* fmtp */
        ST_ZOS_SSTR stLabel;         /* label */
        ST_ZOS_SSTR stMslabel;       /* mslabel */
        ST_ZOS_SSTR stUnknownAttr;   /* unknown attribute */
    } u;
} ST_SDP_SSRC;

/* sdp attribute-fields ssrc-group */
typedef struct tagSDP_SSRC_GRP
{
    ZUCHAR ucGrpSemanType;           /* attribute type EN_SDP_GRP_SEMAN_TYPE */
    ST_SDP_SSRC_ID_LST stSsrcIdLst;
} ST_SDP_SSRC_GRP;

/* sdp attribute-fields content mediacnt */
typedef struct tagSDP_MCNT
{
    ZUCHAR ucAttrType;               /* attribute type EN_SDP_MCNT_ATTR_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    ST_ZOS_SSTR stExtnAttr;          /* extension attribute */
} ST_SDP_MCNT;                                                                                                                    

/* list of content mediacnt */
ABNF_TYPEDEF_LIST(SDP_MCNT);

/* sdp attribute-fields content */
typedef ST_SDP_MCNT_LST ST_SDP_CNT;

/* sdp attribute-fields require */
typedef struct tagSDP_ATTR
{
    ZUCHAR ucType;                   /* attribute type EN_SDP_ATTR_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknownAttr;       /* unknown attribute */
} ST_SDP_ATTR;

/* sdp attribute-fields require attribute list */
ABNF_TYPEDEF_LIST(SDP_ATTR);

/***************************** rfc5939 ***************************/
/* sdp attribute-fields csup&creq */
typedef ST_ZOS_SSTR ST_SDP_C_OPT;

/* list of csup&creq option-tag */
ABNF_TYPEDEF_LIST(SDP_C_OPT); 

/* sdp attribute-fields csup&creq */
typedef ST_SDP_C_OPT_LST ST_SDP_COPT;

/* sdp attribute-fields acap */
typedef struct tagSDP_ACAP
{
    ZUINT iAttCap;                  /* att-cap-num */
    ST_SDP_ATTR_STR stAttr;         /* attribute */
} ST_SDP_ACAP; 

/* proto type */
typedef struct tagSDP_PROTO
{
    ZUCHAR ucProtoType;             /* proto type EN_SDP_TT_PROTO_TYPE */
    ZUCHAR aucSpare[3];             /* for 32 bit alignment */
    ST_ZOS_SSTR stUnknown;          /* unknown proto */
} ST_SDP_PROTO; 

/* list of proto */
ABNF_TYPEDEF_LIST(SDP_PROTO); 

/* sdp attribute-fields tcap */
typedef struct tagSDP_TCAP
{
    ZUINT iTrprCap;                 /* trpr-cap-num */
    ST_SDP_PROTO_LST stProtoLst;    /* proto-list */
} ST_SDP_TCAP; 

/* sdp trpr-cap-num */
typedef ZUINT ST_SDP_TRANS_PRO_CFG;

/* list of transport-protocol-config */
ABNF_TYPEDEF_LIST(SDP_TRANS_PRO_CFG);

/* sdp att-cap att-cap-num */
typedef struct tagSDP_ATT_CAP
{    
    ZUINT iCapNum;                 /* att-cap-num */
} ST_SDP_ATT_CAP; 

/* list of att-cap */
ABNF_TYPEDEF_LIST(SDP_ATT_CAP);

/* sdp attribute-fields pcfg mo-att-cap-list */
typedef struct tagSDP_MA_CL
{
    ST_SDP_ATT_CAP_LST stManLst;     /* mandatory att-cap-list*/
    ST_SDP_ATT_CAP_LST stOptLst;     /* optional att-cap-list*/
} ST_SDP_MA_CL; 

/* list of mo-att-cap-list */
ABNF_TYPEDEF_LIST(SDP_MA_CL);

/* sdp attribute-fields pcfg */
typedef struct tagSDP_ATTR_CFG_LST
{
    ZUCHAR ucColonPres;              /* COLON present flag */
    ZUCHAR ucDelPres;                /* delete-attributes present flag */
    ZUCHAR ucDelType;                /* delete type EN_SDP_DEL_ATTR_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_SDP_MA_CL_LST stMaClLst;      /* mo-att-cap-list list */   
} ST_SDP_ATTR_CFG_LST; 

/* sdp attribute-fields pcfg pot-cfg */
typedef struct tagSDP_POT_CFG
{
    ZUCHAR ucPotType;                /* pot-config type EN_SDP_POT_CFG_TYPE */
    ZUCHAR ucExtnCfgPres;            /* extension-config present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stExtnName;          /* extension-config-list ext-cap-name*/
    ST_ZOS_SSTR stExtnLst;           /* extension-config-list ext-cap-list*/
    union
    {
        ST_SDP_ATTR_CFG_LST stAttrCfg; /* attribute-config-list */
        ST_SDP_TRANS_PRO_CFG_LST stTransProCfgLst; /* transport-protocol-config-list */
    }u;
} ST_SDP_POT_CFG; 

/* list of pot-cfg */
ABNF_TYPEDEF_LIST(SDP_POT_CFG);

/* sdp attribute-fields pcfg  */
typedef struct tagSDP_PCFG
{
    ZUCHAR ucPres;                   /* present flag */  
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iCfgNum;                   /* config-number */
    ST_ZOS_SSTR stWsp;               /* WSP */
    ST_SDP_POT_CFG_LST stPotCfgLst;  /* pot-cfg-list */   
} ST_SDP_PCFG; 

/* sdp attribute-fields acfg sel-cfg */
typedef struct tagSDP_SEL_CFG
{
    ZUCHAR ucSelType;                /* sel-config type */
    ZUCHAR ucDelType;                /* attribute type EN_SDP_DEL_ATTR_TYPE */
    ZUCHAR ucDelPres;                /* delete-attributes present flag */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUINT iTrprNum;                  /* trpr-cap-num */
    ST_ZOS_SSTR stSelNameCfg;        /* extension-cap-name */
    ST_ZOS_SSTR stSelCharCfg;        /* extension-config-list VCHAR */
    ST_SDP_MA_CL stMaCl;             /* mo-att-cap-list */
    union
    {
        ST_SDP_ATTR_CFG_LST stAttrCfg; /* attribute-config-list */
        ST_ZOS_SSTR stTransProCfgLst; /* transport-protocol-config-list */
    }u;
} ST_SDP_SEL_CFG; 

/* list of sel-cfg */
ABNF_TYPEDEF_LIST(SDP_SEL_CFG);

/* sdp attribute-fields acfg */
typedef struct tagSDP_ACFG
{
    ZUCHAR ucPres;                   /* present flag */  
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iCfgNum;                   /* config number */
    ST_ZOS_SSTR stWsp;               /* WSP */
    ST_SDP_SEL_CFG_LST stSelCfgLst;  /* sel-cfg-list */   
} ST_SDP_ACFG;

/***************************** rfc4975 ***************************/
/* sdp URI-paramter */
typedef struct tagSDP_URI_PARM
{
    ZUCHAR ucRTknPres;               /* token present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stLToken;            /* left token */
    ST_ZOS_SSTR stRToken;            /* right token */
} ST_SDP_URI_PARM;

/* list of URI-paramter */
ABNF_TYPEDEF_LIST(SDP_URI_PARM); 

/* IPvFuture */
typedef struct tagSDP_IPV_FUTURE
{
    ST_ZOS_SSTR stHexdig;            /* DEXDIG */
    ST_ZOS_SSTR stIpvFuture;         /* IPvFuture */
} ST_SDP_IPV_FUTURE;

/* sdp host */
typedef struct tagSDP_HOST
{
    ZUCHAR ucType;                   /* host type EN_SDP_HOST_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_ZOS_SSTR stRegName;       /* reg-name */
        ZUINT iIpv4Addr;             /* IPv4address */
        ZUCHAR aucIpv6Addr[ZINET_IPV6_ADDR_SIZE]; /* IPv6address */
        ST_SDP_IPV_FUTURE stIpvFuture; /* IPvFuture */
    } u;
} ST_SDP_HOST;

/* sdp authority */
typedef struct tagSDP_AUTHORITY
{ 
    ZUCHAR ucUserInfoPres;           /* userinfo present flag */
    ZUCHAR ucPortPres;               /* port present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUINT iPort;                   /* port */    
    ST_ZOS_SSTR stUserInfo;          /* userinfo */
    ST_SDP_HOST stHost;              /* host */
} ST_SDP_AUTHORITY; 

/* sdp transport */
typedef struct tagSDP_TRANS
{
    ZUCHAR ucType;                   /* transport type EN_SDP_TRANS_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stExt;               /* extension or unknown type string */
} ST_SDP_TRANS;

/* sdp MSRP-URI */
typedef struct tagSDP_MSRP_URI                      
{
    ZUCHAR ucType;                   /* MSRP-URI type EN_SDP_MSRP_URI_TYPE */
    ZUCHAR ucStrPres;                /* msrp-uri string present flag */
    ZUCHAR ucSessIdPres;             /* session-id present flag */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_SDP_TRANS stTrans;            /* transport */
    ST_ZOS_SSTR stSessId;            /* session-id rfc3986 */
    ST_SDP_AUTHORITY stAuthority;    /* authority rfc3986 */
    ST_SDP_URI_PARM_LST stUriParmLst; /* URI-parameter list */
    ST_ZOS_SSTR stUriStr;            /* msrp-uri string */
} ST_SDP_MSRP_URI;

/* list of MSRP-URI */
ABNF_TYPEDEF_LIST(SDP_MSRP_URI); 

/* sdp attribute-fields path */
typedef struct tagSDP_PATH
{
    ST_SDP_MSRP_URI_LST stMsrpUriLst; /* path-list */   
} ST_SDP_PATH;

/* pval */
typedef struct tagSDP_PVAL
{ 
    ZUCHAR ucQStrPres;               /* quoted-string present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stPval;              /* quoted-string */
} ST_SDP_PVAL;

/* sdp type-parm */
typedef struct tagSDP_TYPE_PARM
{
    ZUCHAR ucPvalPres;               /* pval present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stPattr;             /* parm-attribute */
    ST_SDP_PVAL stPval;              /* parm-value */
} ST_SDP_TYPE_PARM;

/* list of type-parm */
ABNF_TYPEDEF_LIST(SDP_TYPE_PARM); 

/* sdp Format-type */
typedef struct tagSDP_FORMAT_TYPE
{ 
    ZUCHAR ucType;                   /* type type EN_SDP_TYPE_TYPE */
    ZUCHAR ucSubType;                /* subtype type EN_SDP_SUBTYPE_TYPE */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stTypeStr;           /* type string */
    ST_ZOS_SSTR stTypeExt;           /* type extension-token */
    ST_ZOS_SSTR stSubTypeExt;        /* subtype extension-token */
    ST_SDP_TYPE_PARM_LST stTypeParmLst; /* type-param list */
} ST_SDP_FORMAT_TYPE;

/* sdp attribute-fields accept-types & accept-wrapped-types format-entry */
typedef struct tagSDP_FORMAT
{
    ZUCHAR ucAllPres;                /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_FORMAT_TYPE stFormatType; /* format type */
} ST_SDP_FORMAT;

/* list of format */
ABNF_TYPEDEF_LIST(SDP_FORMAT);

/* sdp attribute-fields accept-types & accept-wrapped-types */
typedef struct tagSDP_ACPT_TYPES
{
    ST_SDP_FORMAT_LST stFormatLst;   /* format-list */   
} ST_SDP_ACPT_TYPES;

/***************************** rfc5322 ***************************/
/* sdp ccontent */
typedef struct tagSDP_CCON
{
    ZUCHAR ucCConFwsPres;            /* FWS present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stCCon;              /* ccontent string */
} ST_SDP_CCON;

/* list of comment */
ABNF_TYPEDEF_LIST(SDP_CCON);

/* sdp comment */
typedef struct tagSDP_COMM
{
    ZUCHAR ucFwsPres;                /* FWS present flag */
    ZUCHAR ucCommFwsPres;            /* comment FWS present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_SDP_CCON_LST stCConLst;       /* ccontent list */
} ST_SDP_COMM;

/* list of comment */
ABNF_TYPEDEF_LIST(SDP_COMM);

/* sdp zone */
typedef struct tagSDP_ZONE
{
    ZUCHAR ucAdd;                    /* '+' present flag */
    ZUCHAR ucUnObsPres;              /* un-obs-zone present flag */
    ZUCHAR ucType;                   /* zone type EN_SDP_ZONE_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_ZOS_SSTR stZone;              /* zone */
} ST_SDP_ZONE;

/* sdp CFWS */
typedef struct tagSDP_CFWS
{
    ZUCHAR ucCommPres;               /* comment present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_COMM_LST stCommLst;       /* comment list */
} ST_SDP_CFWS;

/* sdp time-of-day */
typedef struct tagSDP_TIME_OF_DAY
{
    ZUCHAR ucLCfwsPres;              /* left CFWS present flag */
    ZUCHAR ucRCfwsPres;              /* right CFWS present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stTime;              /* time */
    ST_SDP_CFWS stLCfws;             /* CFWS */
    ST_SDP_CFWS stRCfws;             /* CFWS */
} ST_SDP_TIME_OF_DAY;

/* sdp time */
typedef struct tagSDP_TIME
{
    ZUCHAR ucSecPres;                /* second present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_TIME_OF_DAY stHour;       /* Hour */
    ST_SDP_TIME_OF_DAY stMin;        /* minute */
    ST_SDP_TIME_OF_DAY stSec;        /* second */
    ST_SDP_ZONE stZone;              /* zone */
} ST_SDP_TIME;

/* sdp day & year*/
typedef struct tagSDP_DAY_YEAR
{
    ZUCHAR ucUnObsPres;              /* day present flag */
    ZUCHAR ucFwsPres;                /* FWS present flag */
    ZUCHAR ucLCfwsPres;              /* left CFWS present flag */
    ZUCHAR ucRCfwsPres;              /* right CFWS present flag */
    ZUINT iDayYear;                  /* day or year */
    ST_SDP_CFWS stLCfws;             /* CFWS */
    ST_SDP_CFWS stRCfws;             /* CFWS */
} ST_SDP_DAY_YEAR;

/* sdp date */
typedef struct tagSDP_DATE
{
    ZUCHAR ucMonthType;              /* month EN_SDP_MONTH_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_DAY_YEAR stDay;           /* day */
    ST_SDP_DAY_YEAR stYear;          /* year */
} ST_SDP_DATE;

/* sdp day-of-week */
typedef struct tagSDP_DAY_OF_WEEK
{
    ZUCHAR ucLCfwsPres;              /* left CFWS present flag */
    ZUCHAR ucRCfwsPres;              /* right CFWS present flag */
    ZUCHAR ucDayType;                /* day name EN_SDP_WKDAY_TYPE */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ST_SDP_CFWS stLCfws;             /* CFWS */
    ST_SDP_CFWS stRCfws;             /* CFWS */
} ST_SDP_DAY_OF_WEEK;

/* sdp date-time */
typedef struct tagSDP_DATE_TIME
{
    ZUCHAR ucCfwsPres;               /* CFWS present flag */
    ZUCHAR ucDayOfWeekPres;          /* day-of-week present flag */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_SDP_DATE stDate;              /* date */
    ST_SDP_TIME stTime;              /* time */
    ST_SDP_CFWS stCfws;              /* CFWS */
    ST_SDP_DAY_OF_WEEK stDayOfWeek;  /* day-of-week */
} ST_SDP_DATE_TIME;

/***************************** rfc822 ***************************/
/* sdp word */
typedef struct tagSDP_WORD
{
    ZUCHAR ucQStrPres;               /* quoted-string present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stWord;              /* quoted-string */
} ST_SDP_WORD;

/* List of sdp word */
ABNF_TYPEDEF_LIST(SDP_WORD); 

/* sdp sub-domain */
typedef struct tagSDP_SDOMAIN
{
    ZUCHAR ucDLiteralPres;           /* domain-literal present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stSDomain;           /* domain */
} ST_SDP_SDOMAIN;

/* List of sdp sub-domain */
ABNF_TYPEDEF_LIST(SDP_SDOMAIN); 

/* sdp addr-spec */
typedef struct tagSDP_ADDR_SPEC
{
    ST_SDP_WORD_LST stWordLst;       /* word list */
    ST_SDP_SDOMAIN_LST stSDomainLst; /* sub-domain list */
} ST_SDP_ADDR_SPEC;

/***************************** rfc2392 ***************************/
/* sdp cid-url */
typedef struct tagSDP_CID_URL
{
    ZUCHAR ucType;                   /* file type EN_SDP_URL_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_ADDR_SPEC stAddrSpec;     /* addr-spec */
} ST_SDP_CID_URL;

/***************************** rfc5547 ***************************/
/* sdp filename-selector */
typedef struct tagSDP_SELECTOR_FNAME
{
    ST_ZOS_SSTR stFileName;          /* file name */
} ST_SDP_SELECTOR_FNAME;

/* sdp ft-parameter. */
typedef struct tagSDP_FTPARM
{
    ST_ZOS_SSTR stAttr;               /* attribute */
    ST_SDP_SELECTOR_FNAME stFileName; /* filename-string */
} ST_SDP_FTPARM;

/* List of sdp ft-parameter. */
ABNF_TYPEDEF_LIST(SDP_FTPARM); 

/* sdp filetype-selector */
typedef struct tagSDP_SELECTOR_FTYPE
{
    ZUCHAR ucType;                   /* file type EN_SDP_TYPE_TYPE */
    ZUCHAR ucSubType;                /* file subtype type EN_SDP_SUBTYPE */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ST_ZOS_SSTR stTypeStr;           /* type string */
    ST_ZOS_SSTR stTypeExt;           /* file type extension-token */
    ST_ZOS_SSTR stSubTypeExt;        /* file subtype extension-token */
    ST_SDP_FTPARM_LST stFtParmLst;   /* ft-parameter list */
} ST_SDP_SELECTOR_FTYPE;

/* sdp hash-value. */
typedef struct tagSDP_HASH_VALUE
{
    ST_ZOS_SSTR stHexDig;           /* HEXDIG */
} ST_SDP_HASH_VALUE;

/* List of sdp hash-value. */
ABNF_TYPEDEF_LIST(SDP_HASH_VALUE); 

/* sdp hash-selector */
typedef struct tagSDP_SELECTOR_HASH
{
    ZUCHAR ucHashAlg;                /* hash-algorithm type EN_SDP_HASH_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stHashAlgExt;        /* file hash-algorithm extension-token */
    ST_ZOS_SSTR stHashValStr;        /* file hash-value string */
    ST_SDP_HASH_VALUE_LST stHashValueLst; /* hash-value list */
} ST_SDP_SELECTOR_HASH;

/* sdp attribute-fields selector */
typedef struct tagSDP_SELECTOR
{
    ZUCHAR ucType;                   /* attribute type EN_SDP_SELECTOR_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_SDP_SELECTOR_FNAME stFileName; /* filename-selector */
        ZUINT iFileSize;           /* filesize-selector */
        ST_SDP_SELECTOR_FTYPE stFileType; /* filetype-selector */
        ST_SDP_SELECTOR_HASH stHash; /* hash-selector */
    } u;
} ST_SDP_SELECTOR;

/* list of selector */
ABNF_TYPEDEF_LIST(SDP_SELECTOR);

/* sdp attribute-fields file-selector */
typedef struct tagSDP_FILE_SLT
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_SELECTOR_LST stSelectorLst; /* Selector-list */   
} ST_SDP_FILE_SLT;

/* sdp attribute-fields date-param */
typedef struct tagSDP_DATE_PARM
{
    ZUCHAR ucType;                   /* attribute type EN_SDP_DATE_PARM_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    union
    {
        ST_SDP_DATE_TIME stCDate;    /* c-date-param */
        ST_SDP_DATE_TIME stMDate;    /* m-date-param */
        ST_SDP_DATE_TIME stRDate;    /* r-date-param */
    } u;
} ST_SDP_DATE_PARM;

/* list of date-param */
ABNF_TYPEDEF_LIST(SDP_DATE_PARM);

/* sdp attribute-fields file-date */
typedef struct tagSDP_FILE_DATE
{
    ST_SDP_DATE_PARM_LST stDateParmLst; /* date-param list */   
} ST_SDP_FILE_DATE;

/* sdp attribute-fields file-icon */
typedef struct tagSDP_FILE_ICON
{
    ST_SDP_CID_URL stCidUrl;         /* file-icon-value cid-url */   
} ST_SDP_FILE_ICON;

/* sdp attribute-fields file-range */
typedef struct tagSDP_FILE_RANGE
{
    ZUCHAR ucUnknowStop;             /* unknown stop-offset */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iStartOffset;              /* start-offset */   
    ZUINT iStopOffset;               /* stop-offset */ 
} ST_SDP_FILE_RANGE;

/***************************** rfc4145 ***************************/
/* attribute connection type */
typedef struct tagSDP_CONNECTION
{
    ZUCHAR ucType;                   /* known media EN_SDP_CONN_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
} ST_SDP_CONNECTION;

/* attribute setup type */
typedef struct tagSDP_SETUP
{
    ZUCHAR ucType;                   /* known media EN_SDP_SETUP_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
} ST_SDP_SETUP;

/***************************** rfc6064 ***************************/
/* attribute framesize type */
typedef struct tagSDP_FRAMESIZE
{
    ZUCHAR ucPayload;                /* payload number */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    ZUINT iWidth;                    /* width */
    ZUINT iHeight;                   /* height */
} ST_SDP_FRAMESIZE;

/***************************** rfc6236 ***************************/
/* max value size */
#define SDP_IMG_ATTR_MAX_CNT 8

/* image attribute range */
typedef struct tagSDP_IMG_ATTR_RANGE
{
    ZUCHAR ucRangePres;              /* range presence flag */
    ZUCHAR ucStepPres;               /* step presence flag */
    ZUCHAR ucValCnt;                 /* valid value count */
    ZUCHAR aucSpare[1];              /* for 32 bits alignment */
    ZUINT aiValTbl[SDP_IMG_ATTR_MAX_CNT]; /* value table */
} ST_SDP_IMG_ATTR_RANGE;

/* image attribute srange */
typedef struct tagSDP_IMG_ATTR_SRANGE
{
    ZUCHAR ucRangePres;              /* range presence flag */
    ZUCHAR ucValCnt;                 /* valid value count */
    ZUCHAR aucSpare[2];              /* for 32 bits alignment */
    ZFLOAT afValTbl[SDP_IMG_ATTR_MAX_CNT]; /* value table */
} ST_SDP_IMG_ATTR_SRANGE;

/* image attribute prange */
typedef struct tagSDP_IMG_ATTR_PRANGE
{
    ZFLOAT fMinVal;                  /* minumum value */
    ZFLOAT fMaxVal;                  /* maximum value */
} ST_SDP_IMG_ATTR_PRANGE;

/* image attribute range */
typedef struct tagSDP_IMG_ATTR_XRANGE
{
    ZUINT iMinVal;                 /* minumum value */
    ZUINT iMaxVal;                 /* maximum value */
} ST_SDP_IMG_ATTR_XRANGE;

/* image attribute set */
typedef struct tagSDP_IMG_ATTR_SET
{
    ZUCHAR ucSarPres;                /* sar presence flag */
    ZUCHAR ucParPres;                /* par presence flag */
    ZUCHAR ucQPres;                  /* q presence flag */
    ZUCHAR ucBrPres;                 /* br presence flag */
    ZUCHAR ucFrPres;                 /* fr presence flag */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    ZFLOAT fQ;                       /* q value */
    ST_SDP_IMG_ATTR_RANGE stRangeX;  /* x range */
    ST_SDP_IMG_ATTR_RANGE stRangeY;  /* y range */
    ST_SDP_IMG_ATTR_SRANGE stSar;    /* sar value */
    ST_SDP_IMG_ATTR_PRANGE stPar;    /* par value */
    ST_SDP_IMG_ATTR_XRANGE stBr;     /* br value */
    ST_SDP_IMG_ATTR_XRANGE stFr;     /* fr value */
} ST_SDP_IMG_ATTR_SET;

/* list of attribute set */
ABNF_TYPEDEF_LIST(SDP_IMG_ATTR_SET);

/* attribute imageattr type */
typedef struct tagSDP_IMG_ATTR
{
    ZUCHAR ucPayload;                /* payload number */
    ZUCHAR ucSendAll;                /* '*' for send */
    ZUCHAR ucRecvAll;                /* '*' for receive */
    ZUCHAR aucSpare[1];              /* for 32 bits alignment */
    ST_SDP_IMG_ATTR_SET_LST stSendLst; /* image attribute set list of send */
    ST_SDP_IMG_ATTR_SET_LST stRecvLst; /* image attribute set list of receive*/
} ST_SDP_IMG_ATTR;

/* rtcp-fb parameter */
typedef struct tagSDP_RTCP_FB_PARM
{
    ZUCHAR ucType;                   /* parameter type, EN_SDP_RTCP_FB_PARM_TYPE */
    ZUCHAR ucValuePres;              /* value presence flag */
    ZUCHAR aucSpare[2];              /* for 32 bits alignment */
    ST_ZOS_SSTR stType;              /* unknown type string */
    union
    {
        ZUINT aiMsgType[4];        /* subMessageType table */
        ST_ZOS_SSTR stMaxRate;       /* smaxpr MaxPacketRateValue */
        ST_ZOS_SSTR stValue;         /* unknown type value */
    } u;
} ST_SDP_RTCP_FB_PARM;

/* attribute rtcp-fb type */
typedef struct tagSDP_RTCP_FB
{
    ZUCHAR ucPayload;                /* payload type */
    ZUCHAR ucIdType;                 /* feedback type, EN_SDP_RTCP_FB_TYPE */
    ZUCHAR ucParmPres;               /* parameter presence flag */
    ZUCHAR aucSpare[1];              /* for 32 bits alignment */
    ST_ZOS_SSTR stId;                /* feedback value id */
    ST_SDP_RTCP_FB_PARM stParm;      /* feedback value parameter */
} ST_SDP_RTCP_FB;

/* extension attribute */
typedef struct tagSDP_EXT_ATTR
{
    ST_ZOS_SSTR stName;              /* attribute name */
    ST_ZOS_SSTR stValue;             /* attribute value */
} ST_SDP_EXT_ATTR;

/* extensino attribute list */
ABNF_TYPEDEF_LIST(SDP_EXT_ATTR);

/* attribute candidate type */
typedef struct tagSDP_CAND
{
    ZUCHAR ucTptType;                /* transport, EN_SDP_CAND_TPT_TYPE */
    ZUCHAR ucCandType;               /* cand-type, EN_SDP_CAND_TYPE */
    ZUSHORT wCompId;                 /* component-id from 1-256 */
    ZUSHORT wPort;                   /* port */
    ZUSHORT wRelPort;                /* rel-port */
    ZUINT iPriority;                 /* priority */
    ST_ZOS_SSTR stExtTpt;            /* transport-extension */
    ST_ZOS_SSTR stCandType;          /* token of cand-type */
    ST_ZOS_SSTR stFoundation;        /* foundation */
    ST_SDP_CONN_ADDR stConnAddr;     /* connection-address */
    ST_SDP_CONN_ADDR stRelAddr;      /* rel-addr */
    ST_SDP_EXT_ATTR_LST stExtAttrLst; /* extension attribute */
} ST_SDP_CAND;

/* attribute remote-candidate type */
typedef struct tagSDP_RMT_CAND
{
    ZUSHORT wPort;                   /* port */
    ZUSHORT wCompId;                 /* component-id from 1-256 */
    ST_SDP_CONN_ADDR stConnAddr;     /* connection-address */
} ST_SDP_RMT_CAND;

/* attribute fingerprint type */
typedef struct tagSDP_FGRPRNT
{
    ZUCHAR ucHashType;               /* hash function type, EN_SDP_FGRPRNT_HASH_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    ST_ZOS_SSTR stHashExt;           /* hash function type extension */
    ST_ZOS_SSTR stVal;               /* fingerprint value */
} ST_SDP_FGRPRNT;

/* strp key-info = key-salt ["|" lifetime] ["|" mki] */
typedef struct tagSDP_STRP_KEY_INFO
{
    ZUCHAR ucLifeTimePres;          /* lifetime presence flag */
    ZUCHAR ucLifeTimeExpForm;       /* lifetime is in exponential form */
    ZUCHAR ucMkiPres;               /* mki presence flag */
    ZUCHAR aucSpare[1];             /* for 32 bits alignment */
    ST_ZOS_SSTR stKeySalt;          /* key-salt base64 encoded */
    ZUINT iLifeTime;                /* life time */
    ZUINT iMkiValue;                /* mki value */
    ZUINT iMkiLength;               /* mki length */
} ST_SDP_STRP_KEY_INFO;

/* attribute cryptokey-param  type */
typedef struct tagSDP_CRYPTO_KEY_PARM
{
    ZUCHAR ucMethod;                /* crypto-key-method, EN_SDP_CRYPTO_KEY_METHOD_TYPE */
    ZUCHAR ucSrtpInfoPres;          /* srtp info presence */
    ZUCHAR aucSpare[2];             /* for 32 bits alignment */
    ST_ZOS_SSTR stMethodExt;        /* extension method, EN_SDP_CRYPTO_KEY_METHOD_EXT */
    union
    {
        ST_SDP_STRP_KEY_INFO stSrtpInfo;
        ST_ZOS_SSTR stKeyInfo;
    } u;
} ST_SDP_CRYPTO_KEY_PARM;

ABNF_TYPEDEF_LIST(SDP_CRYPTO_KEY_PARM); 

typedef struct tagSDP_CRYPTO_SESS_PARM
{
    ZUCHAR ucSessType;              /* crypto-sess-type, EN_SDP_CRYPTO_SESS_TYPE */
    ZUCHAR aucSpare[3];             /* for 32 bits alignment */
    union
    {
        ZUCHAR ucKdr;               /* EN_SDP_CRYPTO_SESS_KDR */
        ZUCHAR ucFecOrder;          /* EN_SDP_CRYPTO_SESS_FEC_ORDER, EN_SDP_CRYPTO_SESS_FEC_ORDER_TYPE */
        ZUINT iWsh;
        ST_SDP_CRYPTO_KEY_PARM_LST stFecKeys;/* EN_SDP_CRYPTO_SESS_FEC_KEY */
        ST_ZOS_SSTR stSessExt;      /* extension session-param, EN_SDP_CRYPTO_SESS_EXT */
    } u;
} ST_SDP_CRYPTO_SESS_PARM;

ABNF_TYPEDEF_LIST(SDP_CRYPTO_SESS_PARM); 

/* attribute crypto type */
typedef struct tagSDP_CRYPTO
{
    ZUCHAR ucSuite;                 /* crypto-suite type, EN_SDP_CRYPTO_SUITE_TYPE */
    ZUCHAR aucSpare[3];             /* for 32 bits alignment */
    ZUINT iTag;                     /* tag, 1*9DIGIT */
    ST_ZOS_SSTR stSuiteExt;         /* crypto-suite extension string */
    ST_SDP_CRYPTO_KEY_PARM_LST stKeyParmLst;
    ST_SDP_CRYPTO_SESS_PARM_LST stSessParmLst;
} ST_SDP_CRYPTO;

/* attribute extmap type */
typedef struct tagSDP_EXTMAP
{
    ZUCHAR ucDirPres;               /* direction presence */
    ZUCHAR ucExtAttrPres;           /* extension attributes presence */
    ZUCHAR ucDirection;             /* direction type EN_SDP_DIRECT_TYPE_TYPE */
    ZUCHAR aucSpare[1];             /* for 32 bits alignment */
    ZUINT iValue;                   /* rtp extension value */
    ST_ZOS_SSTR stUri;              /* URI string */
    ST_ZOS_SSTR stExtAttr;          /* extension attributes */
} ST_SDP_EXTMAP;

/* remote-candidate list */
ABNF_TYPEDEF_LIST(SDP_RMT_CAND);

/* ice-options list */
ABNF_TYPEDEF_LIST(ZOS_SSTR);

/* sdp attribute-fields */
typedef struct tagSDP_AF
{
    ZUCHAR ucType;                   /* attribute type EN_SDP_ATTR_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_ZOS_SSTR stFieldStr;          /* field string */
    union
    {
        ST_ZOS_SSTR stCat;           /* category */
        ST_ZOS_SSTR stKeywords;      /* keywords */
        ST_ZOS_SSTR stTool;          /* name and version of tool */
        ZUINT iPTime;                /* packet time */
        ZUINT iMaxPTime;             /* maximum packet time */
        ZUCHAR ucOrient;             /* orientation EN_SDP_ATTR_ORIENT_TYPE */
        ST_SDP_ATTR_CONF stConf;     /* conference type */
        ST_ZOS_SSTR stChrset;        /* character set */
        ST_ZOS_SSTR stSdplang;       /* sdp language tag */
        ST_ZOS_SSTR stLang;          /* language tag */
        ST_ZOS_SSTR stFramrate;      /* frame rate */
        ST_ZOS_SSTR stQuality;       /* quality */
        ST_SDP_FMTP stFmtp;          /* fmtp */
        ST_SDP_RTPMAP stRtpmap;      /* rtpmap */
        ST_SDP_CUR_STA stCurSta;     /* current-status for rfc3312 */
        ST_SDP_DES_STA stDesSta;     /* desired-status for rfc3312 */
        ST_SDP_CNF_STA stCnfSta;     /* confirm-status for rfc3312 */
        ST_SDP_RTCP stRtcp;          /* rtcp for rfc3605 */
        ST_SDP_MID stMid;            /* mid for rfc5888 */
        ST_SDP_GRP stGrp;            /* group for rfc5888 */
        ST_ZOS_SSTR stPhoneContext;  /* phone context ident for rfc2848 */
        ZBOOL bClir;                 /* Calling Line Identity Restriction */
        ZUCHAR ucQ763Nat;            /* Q763-nature for rfc2848 */
        ZUCHAR ucQ763Plan;           /* Q763-plan for rfc2848 */
        ZUCHAR ucQ763Inn;            /* Q763-INN for rfc2848 */
        ST_SDP_ATTR_LST stReqAttrLst; /* require attributes for rfc2848 */
        ST_ZOS_SSTR stControl;       /* control */
        ST_SDP_SSRC stSsrc;          /* ssrc for rfc5576 */
        ST_SDP_SSRC_GRP stSsrcGrp;   /* ssrc group for rfc5576 */
        ST_SDP_CNT stContent;        /* content for rfc4796*/
        ST_SDP_COPT stCsup;          /* csup for rfc5939 */
        ST_SDP_COPT stCreq;          /* creq for rfc5939 */
        ST_SDP_ACAP stAcap;          /* acap for rfc5939 */
        ST_SDP_TCAP stTcap;          /* tcap for rfc5939 */
        ST_SDP_PCFG stPcfg;          /* pcfg for rfc5939 */
        ST_SDP_ACFG stAcfg;          /* acfg for rfc5939 */
        ST_SDP_PATH stPath;          /* path for rfc4975 */
        ST_SDP_ACPT_TYPES stAcptTypes; /* accept-types for rfc4975 */
        ST_SDP_ACPT_TYPES stAcptWpdTypes; /* accept-wrapped-types for rfc4975 */
        ZUINT iMaxSize;            /* max-size for rfc4975 */
        ST_SDP_FILE_SLT stFileSlt;   /* file-selector for rfc5547 */
        ST_ZOS_SSTR stFileDisp;      /* file-disp for rfc5547 */
        ST_ZOS_SSTR stFileTrId;      /* file-tr-id for rfc5547 */
        ST_SDP_FILE_DATE stFileDate; /* file-date for rfc5547 */
        ST_SDP_FILE_ICON stFileIcon; /* file-icon for rfc5547 */
        ST_SDP_FILE_RANGE stFileRange; /* file-range for rfc5547 */
        ST_SDP_CONNECTION stConnection; /* connection for rfc4145 */
        ST_SDP_SETUP stSetup;        /* setup for rfc4145 */
        ST_SDP_FRAMESIZE stFramesize; /* framesize for rfc6064 */
        ST_SDP_IMG_ATTR stImgAttr;   /* imgattr for rfc6236 */
        ST_SDP_RTCP_FB stRtcpFb;     /* rtcp-fb for rfc4585 */
        ST_SDP_CAND stCand;          /* candidate for rfc5245 */
        ST_SDP_RMT_CAND_LST stRmtCandLst; /* remote-candidates for rfc5245 */
        ST_ZOS_SSTR stIceUfrag;      /* ice-ufrag for rfc5245 */
        ST_ZOS_SSTR stIcePwd;        /* ice-pwd for rfc5245 */
        ST_ZOS_SSTR_LST stIceOptLst; /* ice-options for rfc5245 */
        ZUCHAR ucRecPref;            /* record prefrence EN_SDP_ATTR_REC_PREF_TYPE */
        ST_SDP_FGRPRNT stFgrprnt;    /* fingerprint EN_SDP_FGRPRNT_HASH_TYPE */
        ST_SDP_CRYPTO stCrypto;      /* crypto EN_SDP_ATTR_CRYPTO */
        ST_SDP_EXTMAP stExtMap;      /* extmap EN_SDP_ATTR_EXTMAP */
        ST_SDP_ATTR_STR stUnknown;   /* unknown attribute */
    } u;
} ST_SDP_AF;

/* list of attribute-field */
ABNF_TYPEDEF_LIST(SDP_AF);

/* sdp media-descriptions */
typedef struct tagSDP_MDESC
{
    ST_SDP_MF stMedia;               /* media-field */
    ST_SDP_IF stInfo;                /* information-field */
    ST_SDP_CF_LST stConnLst;         /* connection-fields */
    ST_SDP_BF_LST stBwLst;           /* bandwidth-fields */
    ST_SDP_KF stKey;                 /* key-field */
    ST_SDP_AF_LST stAttrLst;         /* attribute-fields */
} ST_SDP_MDESC;

/* list of media-field */
ABNF_TYPEDEF_LIST(SDP_MDESC);

/* sdp session-description(rfc4566) or announcement(rfc2327) */
typedef struct tagSDP_SESS_DESC
{
    ZUCHAR ucPres;                   /* present flag */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ST_SDP_VF stVer;                 /* proto-version */
    ST_SDP_OF stOrig;                /* origin-field */
    ST_SDP_SF stSessName;            /* session-name-field */
    ST_SDP_IF stInfo;                /* information-field */
    ST_SDP_UF stUri;                 /* uri-field */
    ST_SDP_EF_LST stEmailLst;        /* email-fields list */
    ST_SDP_PF_LST stPhoneLst;        /* phone-fields list */
    ST_SDP_CF stConn;                /* connection-field */
    ST_SDP_BF_LST stBwLst;           /* bandwidth-fields */
    ST_SDP_SESS_TIME stSessTime;     /* time-fields */
    ST_SDP_KF stKey;                 /* key-field */
    ST_SDP_AF_LST stAttrLst;         /* attribute-fields */
    ST_SDP_MDESC_LST stMdescLst;     /* media-descriptions */
} ST_SDP_SESS_DESC;

/* list of session-description */
ABNF_TYPEDEF_LIST(SDP_SESS_DESC);

#ifdef __cplusplus
}
#endif 

#endif /* _SDP_ABNF_TYPE_H__ */

