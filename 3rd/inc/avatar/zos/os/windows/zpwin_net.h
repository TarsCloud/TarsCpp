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
  File name     : zpwin_net.h
  Module        : windows socket library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by socket 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPWIN_NET_H__
#define _ZPWIN_NET_H__

#ifdef __cplusplus
extern "C" {
#endif
/* windows start tunl create task */
ZFUNC ZINT Zpwin_StartTunlTask(ZFUNC_VOID);

/* windows stop tunl create task */
ZFUNC ZVOID Zpwin_StopTunlTask(ZFUNC_VOID);

/* windows create tunl */
ZFUNC ZINT Zpwin_CreateTunl(PFN_TUNLCREATECB pfnCb, ZCOOKIE zCookie,
             ZINT iTunlType, ST_ZOS_INET_ADDR *pstAddr,
             ST_ZOS_SOCK_PROXY_SRV_INFO *pstStgProxyInfo,
             ST_ZOS_SOCK_CFG *pstCfg);

/* windows inet destroy */
ZFUNC ZVOID Zpwin_CloseTunl(ZFUNC_VOID);

/* windows inet init */
ZFUNC ZINT Zpwin_InetInit(ZFUNC_VOID);

/* windows inet destroy */
ZFUNC ZVOID Zpwin_InetDestroy(ZFUNC_VOID);

/* windows inet addr */
ZFUNC ZINT Zpwin_InetAddr(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);

/* windows inet ntohl */
ZFUNC ZUINT Zpwin_InetNtohl(ZUINT iVal);

/* windows inet htonl */
ZFUNC ZUINT Zpwin_InetHtonl(ZUINT iVal);

/* windows inet ntohs */
ZFUNC ZUSHORT Zpwin_InetNtohs(ZUSHORT iVal);

/* windows inet htons */
ZFUNC ZUSHORT Zpwin_InetHtons(ZUSHORT iVal);

/* windows inet fd create */
ZFUNC ZINT Zpwin_InetFdCreate(ZFD_SET *pzFds);

/* windows inet fd delete */
ZFUNC ZINT Zpwin_InetFdDelete(ZFD_SET zFds);

/* windows inet fd copy */
ZFUNC ZINT Zpwin_InetFdCopy(ZFD_SET zDstFds, ZFD_SET zSrcFds);

/* windows inet fd zero */
ZFUNC ZINT Zpwin_InetFdZero(ZFD_SET zFds);

/* windows inet fd set */
ZFUNC ZINT Zpwin_InetFdSet(ZFD_SET zFds, ZINT iFd);

/* windows inet fd isset */
ZFUNC ZBOOL Zpwin_InetFdIsset(ZFD_SET zFds, ZINT iFd);

/* windows inet fd clear */
ZFUNC ZINT Zpwin_InetFdClr(ZFD_SET zFds, ZINT iFd);

/* windows inet get host by name */
ZFUNC ZINT Zpwin_InetGetHostByName(ZCHAR *pcName, ST_ZOS_INET_IPS *pstIps);

/* windows inet get local ip address */
ZFUNC ZINT Zpwin_InetGetLocalIp(ST_ZOS_INET_IPS *pstIps, ZINT iIpType);

/* windows inet get access network type */
ZFUNC ZINT Zpwin_InetGetAccNetType(ZINT iDefNetType);

/* windows os create a socket, inet type see ZINET_IPV4..., 
   protocol type seee ZINET_PROTO_UDP... */
ZFUNC ZINT Zpwin_SocketOpen(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZBOOL bBlock, ZBOOL bReuseAddr, 
                ZINT *piSock, ZINT iSockType, ZBOOL bDrvTask,
                ZVOID *pUserInfo);
   
/* windows close a socket */
ZFUNC ZINT Zpwin_SocketClose(ZSOCKET zSock);

/* windows shutdown socket ZINET_SHTDWN_RECV... */
ZFUNC ZINT Zpwin_SocketShutdown(ZSOCKET zSock, ZINT iHowTo);

/* windows select socket */
ZFUNC ZINT Zpwin_SocketSelect(ZINT iMaxFd, ZFD_SET zReadFds, ZFD_SET zWriteFds, 
                ZFD_SET zExceptFds, ZINT iMsecTimeOut, ZINT *piNumFds);

/* windows bind socket */
ZFUNC ZINT Zpwin_SocketBind(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstLclAddr);

/* windows list socket */
ZFUNC ZINT Zpwin_SocketListen(ZSOCKET zSock);

/* windows accept socket */
ZFUNC ZINT Zpwin_SocketAccept(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZBOOL bBlock, ZINT *piNewSock);

/* windows connect socket */
ZFUNC ZINT Zpwin_SocketConnect(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr,
                ZBOOL bConnTls, ZBOOL *pbRunErr, ZBOOL *pbConned);

/* windows socket sendto */
ZFUNC ZINT Zpwin_SocketSendTo(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);

/* windows socket recvfrom */
ZFUNC ZINT Zpwin_SocketRecvFrom(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen);

/* windows socket send */
ZFUNC ZINT Zpwin_SocketSend(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen, 
                ZBOOL *pbClosed, ZBOOL *pbWait);

/* windows socket recv */
ZFUNC ZINT Zpwin_SocketRecv(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed);

/* windows socket get local address */
ZFUNC ZINT Zpwin_SocketGetLocalAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/* windows socket get remote address */
ZFUNC ZINT Zpwin_SocketGetRemoteAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* windows os set socket option block(like ioctl FIONBIO) */
ZFUNC ZINT Zpwin_SocketSetOptBlk(ZSOCKET zSock, ZBOOL bBlock);

/* windows os set socket option reuse addr(like BSD SO_REUSEADDR) */
ZFUNC ZINT Zpwin_SocketSetOptReuseAddr(ZSOCKET zSock, ZBOOL bReuseAddr);

/* windows os set socket option receive buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zpwin_SocketSetOptRecvBuf(ZSOCKET zSock, ZUINT iBufSize);

/* windows os set socket option send buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zpwin_SocketSetOptSendBuf(ZSOCKET zSock, ZUINT iBufSize);

/* windows os set socket option tos */
ZFUNC ZINT Zpwin_SocketSetOptTos(ZSOCKET zSock, ZUINT iTosVal);

/* windows os set socket option tclass */
ZFUNC ZINT Zpwin_SocketSetOptTclass(ZSOCKET zSock, ZUINT iTclassVal);

/* windows os set multicast address */
ZFUNC ZINT Zpwin_SocketSetOptMAddr(ZSOCKET zSock, ST_ZOS_INET_IP *pstMAddr);

/* windows os get socket option error(like BSD SO_ERROR) */
ZFUNC ZINT Zpwin_SocketGetOptErr(ZSOCKET zSock, ZINT *piErrNo);

/* windows os get socket last error */
ZFUNC ZINT Zpwin_SocketGetLastErr(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPWIN_NET_H__ */

