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
  File name     : zplin_net.h
  Module        : linux socket library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by socket 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPLIN_NET_H__
#define _ZPLIN_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

/* linux inet addr */
ZFUNC ZINT Zplin_InetAddr(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);

/* linux inet ntohl */
ZFUNC ZUINT Zplin_InetNtohl(ZUINT iVal);

/* linux inet htonl */
ZFUNC ZUINT Zplin_InetHtonl(ZUINT iVal);

/* linux inet ntohs */
ZFUNC ZUSHORT Zplin_InetNtohs(ZUSHORT iVal);

/* linux inet htons */
ZFUNC ZUSHORT Zplin_InetHtons(ZUSHORT iVal);

/* linux inet fd create */
ZFUNC ZINT Zplin_InetFdCreate(ZFD_SET *pzFds);

/* linux inet fd delete */
ZFUNC ZINT Zplin_InetFdDelete(ZFD_SET zFds);

/* linux inet fd copy */
ZFUNC ZINT Zplin_InetFdCopy(ZFD_SET zDstFds, ZFD_SET zSrcFds);

/* linux inet fd zero */
ZFUNC ZINT Zplin_InetFdZero(ZFD_SET zFds);

/* linux inet fd set */
ZFUNC ZINT Zplin_InetFdSet(ZFD_SET zFds, ZINT iFd);

/* linux inet fd isset */
ZFUNC ZBOOL Zplin_InetFdIsset(ZFD_SET zFds, ZINT iFd);

/* linux inet fd clear */
ZFUNC ZINT Zplin_InetFdClr(ZFD_SET zFds, ZINT iFd);

/* linux inet get host by name */
ZFUNC ZINT Zplin_InetGetHostByName(ZCHAR *pcName, ST_ZOS_INET_IPS *pstIps);

/* linux inet get local ip address */
ZFUNC ZINT Zplin_InetGetLocalIp(ST_ZOS_INET_IPS *pstIps);

/* linux inet get access network type */
ZFUNC ZINT Zplin_InetGetAccNetType(ZINT iDefNetType);


/* linux os create a socket, inet type see ZINET_IPV4..., 
   protocol type seee ZINET_PROTO_UDP... */
ZFUNC ZINT Zplin_SocketOpen(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZBOOL bBlock, ZBOOL bReuseAddr, 
                ZINT *piSock, ZINT iSockType, ZBOOL bDrvTask,
                ZVOID *pUserInfo);
   
/* linux close a socket */
ZFUNC ZINT Zplin_SocketClose(ZSOCKET zSock);

/* linux shutdown socket ZINET_SHTDWN_RECV... */
ZFUNC ZINT Zplin_SocketShutdown(ZSOCKET zSock, ZINT iHowTo);

/* linux select socket */
ZFUNC ZINT Zplin_SocketSelect(ZINT iMaxFd, ZFD_SET zReadFds, ZFD_SET zWriteFds, 
                ZFD_SET zExceptFds, ZINT iMsecTimeOut, ZINT *piNumFds);

/* linux bind socket */
ZFUNC ZINT Zplin_SocketBind(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstLclAddr);

/* linux list socket */
ZFUNC ZINT Zplin_SocketListen(ZSOCKET zSock);

/* linux accept socket */
ZFUNC ZINT Zplin_SocketAccept(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZBOOL bBlock, ZINT *piNewSock);

/* linux connect socket */
ZFUNC ZINT Zplin_SocketConnect(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr,
                 ZBOOL bConnTls, ZBOOL *pbRunErr, ZBOOL *pbConned);

/* linux socket sendto */
ZFUNC ZINT Zplin_SocketSendTo(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);

/* linux socket recvfrom */
ZFUNC ZINT Zplin_SocketRecvFrom(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen);

/* linux socket send */
ZFUNC ZINT Zplin_SocketSend(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen, 
                ZBOOL *pbClosed, ZBOOL *pbWait);

/* linux socket recv */
ZFUNC ZINT Zplin_SocketRecv(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed);

/* linux socket get local address */
ZFUNC ZINT Zplin_SocketGetLocalAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/* linux socket get remote address */
ZFUNC ZINT Zplin_SocketGetRemoteAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* linux os set socket option block(like ioctl FIONBIO) */
ZFUNC ZINT Zplin_SocketSetOptBlk(ZSOCKET zSock, ZBOOL bBlock);

/* linux os set socket option reuse addr(like BSD SO_REUSEADDR) */
ZFUNC ZINT Zplin_SocketSetOptReuseAddr(ZSOCKET zSock, ZBOOL bReuseAddr);

/* linux os set socket option receive buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zplin_SocketSetOptRecvBuf(ZSOCKET zSock, ZUINT iBufSize);

/* linux os set socket option send buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zplin_SocketSetOptSendBuf(ZSOCKET zSock, ZUINT iBufSize);

/* linux os set socket option tos */
ZFUNC ZINT Zplin_SocketSetOptTos(ZSOCKET zSock, ZUINT iTosVal);

/* linux os set socket option tclass */
ZFUNC ZINT Zplin_SocketSetOptTclass(ZSOCKET zSock, ZUINT iTclassVal);

/* linux os set multicast address */
ZFUNC ZINT Zplin_SocketSetOptMAddr(ZSOCKET zSock, ST_ZOS_INET_IP *pstMAddr);

/* linux os get socket option error(like BSD SO_ERROR) */
ZFUNC ZINT Zplin_SocketGetOptErr(ZSOCKET zSock, ZINT *piErrNo);

/* linux os get socket last error */
ZFUNC ZINT Zplin_SocketGetLastErr(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPLIN_NET_H__ */

