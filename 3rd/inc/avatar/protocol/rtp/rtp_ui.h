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
  File name     : rtp_ui.h
  Module        : rtp protocol 
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-09-30    
  Description   :
    Marcos and structure definitions required by the rtp user interface.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _RTP_UI_H__
#define _RTP_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* rtp rtp header fix length not including csrc list */
#define RTP_RTP_HDR_FIX_LEN            12

/* rtp packet information */
typedef struct tagRTP_RTP_INFO
{
    ZUCHAR ucVer;                    /* rtp version */
    ZUCHAR ucPadding;                /* padding flag */
    ZUCHAR ucExt;                    /* extension exist flag */
    ZUCHAR ucCsrcCount;              /* contributing source count */
    ZUCHAR ucMarker;                 /* marker flag */
    ZUCHAR ucPayload;                /* payload type */
    ZUSHORT wSeq;                    /* sequence number */
    ZUINT iTs;                       /* timestamp */
    ZUINT iSsrc;                     /* synchronization source */
    ZUINT aiCsrc[15];                /* contributing source list */
} ST_RTP_RTP_INFO;

/* rtcp packet infomation */
typedef struct tagRTP_RTCP_INFO
{
    ZUCHAR ucSubType;                /* subtype */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iSsrc;                     /* synchronization source */
 } ST_RTP_RTCP_INFO;

/* rtp read rtp data process */
typedef ZINT (*PFN_RTPRDRTP)(ZCOOKIE zCookie, ZUINT iRtpId, \
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucData, \
                ZUINT iDataLen, ST_RTP_RTP_INFO *pstInfo);

/* rtp read rtcp application data process */
typedef ZINT (*PFN_RTPRDRTCPAPP)(ZCOOKIE zCookie, ZUINT iRtpId, \
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucAppData, \
                ZUINT iAppDataLen, ST_RTP_RTCP_INFO *pstInfo);

/* rtp dynamic payload type */
#define RTP_DYNAMIC_PAYLOAD 96

/* rtp payload type */
typedef enum EN_RTP_PAYLOAD_TYPE
{
    EN_RTP_PAYLOAD_PCMU = 0,
    EN_RTP_PAYLOAD_GSM = 3,
    EN_RTP_PAYLOAD_G723 = 4,
    EN_RTP_PAYLOAD_DVI4_8K = 5,
    EN_RTP_PAYLOAD_DVI4_16K = 6,
    EN_RTP_PAYLOAD_LPC = 7,
    EN_RTP_PAYLOAD_PCMA = 8,
    EN_RTP_PAYLOAD_G722 = 9,
    EN_RTP_PAYLOAD_L16_STEREO = 10,
    EN_RTP_PAYLOAD_L16_MONO = 11,
    EN_RTP_PAYLOAD_QCELP = 12,
    EN_RTP_PAYLOAD_CN = 13,
    EN_RTP_PAYLOAD_MPA = 14,
    EN_RTP_PAYLOAD_G728 = 15,
    EN_RTP_PAYLOAD_DVI4_11K = 16,
    EN_RTP_PAYLOAD_DVI4_22K = 17,
    EN_RTP_PAYLOAD_G729 = 18,
    EN_RTP_PAYLOAD_CELB = 25,
    EN_RTP_PAYLOAD_JPEG = 26,
    EN_RTP_PAYLOAD_NV = 28,
    EN_RTP_PAYLOAD_H261 = 31,
    EN_RTP_PAYLOAD_MPV = 32,
    EN_RTP_PAYLOAD_MP2T = 33,
    EN_RTP_PAYLOAD_H263 = 34,
    EN_RTP_PAYLOAD_MAX = 128
} EN_RTP_PAYLOAD_TYPE;

/* rtp open rtp channel request */
ZFUNC ZINT Rtp_Open(ST_ZOS_INET_IP *pstLocalIp, ZUCHAR ucPayload, 
                ZCOOKIE zCookie, PFN_RTPRDRTP pfnRdRtp, 
                ZUSHORT *pwRtpPort, ZUINT *piRtpId);

/* rtp open rtp channel with specified port request */
ZFUNC ZINT Rtp_OpenX(ST_ZOS_INET_IP *pstLocalIp, ZUCHAR ucPayload, 
                ZCOOKIE zCookie, PFN_RTPRDRTP pfnRdRtp, 
                ZUSHORT wRtpPort, ZUINT *piRtpId);

/* rtp close rtp channel request */
ZFUNC ZINT Rtp_Close(ZUINT iRtpId);

/* rtp open rtcp channel request */
ZFUNC ZINT Rtp_RtcpOpen(ZUINT iRtpId, ZUSHORT *pwRtcpPort);

/* rtp open rtcp channel with specified port request */
ZFUNC ZINT Rtp_RtcpOpenX(ZUINT iRtpId, ZUSHORT wRtcpPort);

/* rtp enable or disable rtcp auto sending */
ZFUNC ZINT Rtp_RtcpEnable(ZUINT iRtpId, ZBOOL bEnable);

/* rtp send rtp data */
ZFUNC ZINT Rtp_RtpSend(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen);

/* rtp send rtp data with repeat times */
ZFUNC ZINT Rtp_RtpSendX(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ZUINT iRepeatTimes);

/* rtp send rtp data with marker bit */
ZFUNC ZINT Rtp_RtpSendM(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen);

/* rtp send rtp data with timestamp increase value and repeat times */
ZFUNC ZINT Rtp_RtpSendT(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ZUINT iTsInc, ZUINT iRepeatTimes);

/* rtp send rtp data */
ZFUNC ZINT Rtp_RtpSendA(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen,
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp send rtp data with repeat times */
ZFUNC ZINT Rtp_RtpSendAX(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ZUINT iRepeatTimes, ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp send rtp data with marker bit */
ZFUNC ZINT Rtp_RtpSendAM(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen,
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp send rtp data with timestamp increase value and repeat times */
ZFUNC ZINT Rtp_RtpSendAT(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ZUINT iTsInc, ZUINT iRepeatTimes, ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp send rtp data with header area reserved */
ZFUNC ZINT Rtp_RtpSendE(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen);

/* rtp send rtp data with repeat times with header area reserved */
ZFUNC ZINT Rtp_RtpSendEX(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ZUINT iRepeatTimes);

/* rtp send rtp data with marker bit with header area reserved */
ZFUNC ZINT Rtp_RtpSendEM(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen);

/* rtp send rtp data with timestamp increase value and repeat times with header area reserved */
ZFUNC ZINT Rtp_RtpSendET(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ZUINT iTsInc, ZUINT iRepeatTimes);

/* rtp forward rtp data */
ZFUNC ZINT Rtp_RtpForward(ZUINT iRtpId, ST_RTP_RTP_INFO *pstInfo,
                ZUCHAR *pucData, ZUINT iLen, ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp send rtp heartbeat */
ZFUNC ZINT Rtp_RtpSendHeartbeat(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp send rtcp app data */
ZFUNC ZINT Rtp_RtcpAppSend(ZUINT iRtpId, ZUCHAR ucSubType,
                ZUCHAR *pucData, ZUINT iLen);

/* rtp resend rtcp app data */
ZFUNC ZINT Rtp_RtcpAppResend(ZUINT iRtpId);

/* rtp send rtcp app data */
ZFUNC ZINT Rtp_RtcpAppSendA(ZUINT iRtpId, ZUCHAR ucSubType,
                ZUCHAR *pucData, ZUINT iLen, ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp resend rtcp app data */
ZFUNC ZINT Rtp_RtcpAppResendA(ZUINT iRtpId,
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp send rtcp heartbeat */
ZFUNC ZINT Rtp_RtcpSendHeartbeat(ZUINT iRtpId, ZUCHAR *pucData, ZUINT iLen, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp set remote rtp address */
ZFUNC ZINT Rtp_SetRmtAddr(ZUINT iRtpId, ST_ZOS_INET_ADDR *pstRmtAddr);

/* rtp set payload type */
ZFUNC ZINT Rtp_SetPayload(ZUINT iRtpId, ZUCHAR ucPayload);

/* rtp set profile */
ZFUNC ZINT Rtp_SetProf(ZUINT iRtpId, ZBOOL bEnable, ZUCHAR ucPayload, 
                ZUCHAR ucMarker, ZUCHAR ucPadding, ZUCHAR ucExt, ZUINT iTsInc);

/* rtp set read rtcp app data callback */
ZFUNC ZINT Rtp_SetRdRtcpApp(ZUINT iRtpId, PFN_RTPRDRTCPAPP pfnRdRtcpApp);

/* rtp get ssrc */
ZFUNC ZUINT Rtp_GetSsrc(ZUINT iRtpId);

/* rtp get last sequence number */
ZFUNC ZUINT Rtp_GetLastSeq(ZUINT iRtpId, ZUINT *piLastSeq);

/* rtp get rtcp statistics 
    piFracLost is lost rate, 
    piTotalLost is total lost rtp count ,
    piExtMax is max rtp extended sequence number,  
    piJitter is current average jitter,
    piRtt is the latest delay time,
    piSentPkg is total sent rtp count, 
    piSentByte is total sent rtp bytes,
    piRecvPkg is total received rtp count,  
    piRecvByte is total received rtp bytes */
ZFUNC ZINT Rtp_GetRtcpSta(ZUINT iRtpId, ZUINT *piFracLost, 
                ZUINT *piTotalLost, ZUINT *piExtMax, ZUINT *piJitter, 
                ZUINT *piRtt, ZUINT *piSentByte, ZUINT *piSentPkg,
                ZUINT *piRecvByte, ZUINT *piRecvPkg);

#ifdef __cplusplus
}
#endif

#endif /* _RTP_UI_H__ */

