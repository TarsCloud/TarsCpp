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
  File name     : zpand_net.h
  Module        : android socket library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Data structure and function definitions required by socket 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPAND_NET_H__
#define _ZPAND_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

/* android start tunl create task */
ZFUNC ZINT Zpand_StartTunlTask(ZFUNC_VOID);

/* android stop tunl create task */
ZFUNC ZVOID Zpand_StopTunlTask(ZFUNC_VOID);

/* android create tunl */
ZFUNC ZINT Zpand_CreateTunl(PFN_TUNLCREATECB pfnCb, ZCOOKIE zCookie,
             ZINT iTunlType, ST_ZOS_INET_ADDR *pstAddr, 
             ST_ZOS_SOCK_PROXY_SRV_INFO *pstStgProxyInfo,
             ST_ZOS_SOCK_CFG *pstCfg);

/* android inet destroy */
ZFUNC ZVOID Zpand_CloseTunl(ZFUNC_VOID);

/* android inet addr */
ZFUNC ZINT Zpand_InetAddr(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);

/* android inet ntohl */
ZFUNC ZUINT Zpand_InetNtohl(ZUINT iVal);

/* android inet htonl */
ZFUNC ZUINT Zpand_InetHtonl(ZUINT iVal);

/* android inet ntohs */
ZFUNC ZUSHORT Zpand_InetNtohs(ZUSHORT iVal);

/* android inet htons */
ZFUNC ZUSHORT Zpand_InetHtons(ZUSHORT iVal);

/* android inet fd create */
ZFUNC ZINT Zpand_InetFdCreate(ZFD_SET *pzFds);

/* android inet fd delete */
ZFUNC ZINT Zpand_InetFdDelete(ZFD_SET zFds);

/* android inet fd copy */
ZFUNC ZINT Zpand_InetFdCopy(ZFD_SET zDstFds, ZFD_SET zSrcFds);

/* android inet fd zero */
ZFUNC ZINT Zpand_InetFdZero(ZFD_SET zFds);

/* android inet fd set */
ZFUNC ZINT Zpand_InetFdSet(ZFD_SET zFds, ZINT iFd);

/* android inet fd isset */
ZFUNC ZBOOL Zpand_InetFdIsset(ZFD_SET zFds, ZINT iFd);

/* android inet fd clear */
ZFUNC ZINT Zpand_InetFdClr(ZFD_SET zFds, ZINT iFd);

/* android inet get host by name */
ZFUNC ZINT Zpand_InetGetHostByName(ZCHAR *pcName, ST_ZOS_INET_IPS *pstIps);

/* android inet get local ip address */
ZFUNC ZINT Zpand_InetGetLocalIp(ST_ZOS_INET_IPS *pstIps, ZINT iIpType);

/* android inet get access network type */
ZFUNC ZINT Zpand_InetGetAccNetType(ZINT iDefNetType);

/* android inet get dns ip addr */
ZFUNC ZCHAR * Zpand_InetGetDnsIpAddr(ZINT index);

/* android inet get ip dns count */
ZFUNC ZINT Zpand_InetGetDnsCnt(ZFUNC_VOID);

/* android os create a socket, inet type see ZINET_IPV4..., 
   protocol type seee ZINET_PROTO_UDP... */
ZFUNC ZINT Zpand_SocketOpen(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZBOOL bBlock, ZBOOL bReuseAddr, 
                ZINT *piSock, ZINT iSockType, ZBOOL bDrvTask,
                ZVOID *pUserInfo);
   
/* android close a socket */
ZFUNC ZINT Zpand_SocketClose(ZSOCKET zSock);

/* android shutdown socket ZINET_SHTDWN_RECV... */
ZFUNC ZINT Zpand_SocketShutdown(ZSOCKET zSock, ZINT iHowTo);

/* android select socket */
ZFUNC ZINT Zpand_SocketSelect(ZINT iMaxFd, ZFD_SET zReadFds, 
                 ZFD_SET zWriteFds, ZFD_SET zExceptFds, ZINT iMsecTimeOut, 
                 ZINT *piNumFds);

/* android bind socket */
ZFUNC ZINT Zpand_SocketBind(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstLclAddr);

/* android list socket */
ZFUNC ZINT Zpand_SocketListen(ZSOCKET zSock);

/* android accept socket */
ZFUNC ZINT Zpand_SocketAccept(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZBOOL bBlock, ZINT *piNewSock);

/* android connect socket */
ZFUNC ZINT Zpand_SocketConnect(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr,
                ZBOOL bConnTls, ZBOOL *pbRunErr, ZBOOL *pbConned);

/* android socket sendto */
ZFUNC ZINT Zpand_SocketSendTo(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);

/* android socket recvfrom */
ZFUNC ZINT Zpand_SocketRecvFrom(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucBuf, ZUINT *piLen);

/* android socket send */
ZFUNC ZINT Zpand_SocketSend(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen, 
                ZBOOL *pbClosed, ZBOOL *pbWait);

/* android socket recv */
ZFUNC ZINT Zpand_SocketRecv(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed);

/* android socket get local address */
ZFUNC ZINT Zpand_SocketGetLocalAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/* android socket get remote address */
ZFUNC ZINT Zpand_SocketGetRemoteAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* android os set socket option block(like ioctl FIONBIO) */
ZFUNC ZINT Zpand_SocketSetOptBlk(ZSOCKET zSock, ZBOOL bBlock);

/* android os set socket option reuse addr(like BSD SO_REUSEADDR) */
ZFUNC ZINT Zpand_SocketSetOptReuseAddr(ZSOCKET zSock, ZBOOL bReuseAddr);

/* android os set socket option receive buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zpand_SocketSetOptRecvBuf(ZSOCKET zSock, ZUINT iBufSize);

/* android os set socket option send buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zpand_SocketSetOptSendBuf(ZSOCKET zSock, ZUINT iBufSize);

/* android os set socket option tos */
ZFUNC ZINT Zpand_SocketSetOptTos(ZSOCKET zSock, ZUINT iTosVal);

/* android os set socket option tclass */
ZFUNC ZINT Zpand_SocketSetOptTclass(ZSOCKET zSock, ZUINT iTclassVal);

/* android os set multicast address */
ZFUNC ZINT Zpand_SocketSetOptMAddr(ZSOCKET zSock, 
                 ST_ZOS_INET_IP *pstMAddr);

/* android os get socket option error(like BSD SO_ERROR) */
ZFUNC ZINT Zpand_SocketGetOptErr(ZSOCKET zSock, ZINT *piErrNo);

/* android os get socket last error */
ZFUNC ZINT Zpand_SocketGetLastErr(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPAND_NET_H__ */

