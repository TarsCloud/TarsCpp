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
  File name     : zposx_net.h
  Module        : mac osx socket library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by socket 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPOSX_NET_H__
#define _ZPOSX_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

/* mac osx start tunl create task */
ZFUNC ZINT Zposx_StartTunlTask(ZFUNC_VOID);

/* mac osx stop tunl create task */
ZFUNC ZVOID Zposx_StopTunlTask(ZFUNC_VOID);

/* mac osx create tunl */
ZFUNC ZINT Zposx_CreateTunl(PFN_TUNLCREATECB pfnCb, ZCOOKIE zCookie,
             ZINT iTunlType, ST_ZOS_INET_ADDR *pstAddr,
             ST_ZOS_SOCK_PROXY_SRV_INFO *pstStgProxyInfo,
             ST_ZOS_SOCK_CFG *pstCfg);

/* mac osx inet destroy */
ZFUNC ZVOID Zposx_CloseTunl(ZFUNC_VOID);

/* mac osx inet addr */
ZFUNC ZINT Zposx_InetAddr(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);

/* mac osx inet ntohl */
ZFUNC ZUINT Zposx_InetNtohl(ZUINT iVal);

/* mac osx inet htonl */
ZFUNC ZUINT Zposx_InetHtonl(ZUINT iVal);

/* mac osx inet ntohs */
ZFUNC ZUSHORT Zposx_InetNtohs(ZUSHORT iVal);

/* mac osx inet htons */
ZFUNC ZUSHORT Zposx_InetHtons(ZUSHORT iVal);

/* mac osx inet fd create */
ZFUNC ZINT Zposx_InetFdCreate(ZFD_SET *pzFds);

/* mac osx inet fd delete */
ZFUNC ZINT Zposx_InetFdDelete(ZFD_SET zFds);

/* mac osx inet fd copy */
ZFUNC ZINT Zposx_InetFdCopy(ZFD_SET zDstFds, ZFD_SET zSrcFds);

/* mac osx inet fd zero */
ZFUNC ZINT Zposx_InetFdZero(ZFD_SET zFds);

/* mac osx inet fd set */
ZFUNC ZINT Zposx_InetFdSet(ZFD_SET zFds, ZINT iFd);

/* mac osx inet fd isset */
ZFUNC ZBOOL Zposx_InetFdIsset(ZFD_SET zFds, ZINT iFd);

/* mac osx inet fd clear */
ZFUNC ZINT Zposx_InetFdClr(ZFD_SET zFds, ZINT iFd);

/* mac osx inet get host by name */
ZFUNC ZINT Zposx_InetGetHostByName(ZCHAR *pcName, ST_ZOS_INET_IPS *pstIps);

/* mac osx inet get local ip address */
ZFUNC ZINT Zposx_InetGetLocalIp(ST_ZOS_INET_IPS *pstIps, ZINT iIpType);

/* mac osx inet get local ip address */
ZFUNC ZINT Zposx_InetGetLocalIpX(ST_ZOS_INET_IPS *pstIps);

/* mac osx os create a socket, inet type see ZINET_IPV4..., 
   protocol type seee ZINET_PROTO_UDP... */
ZFUNC ZINT Zposx_SocketOpen(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZBOOL bBlock, ZBOOL bReuseAddr, ZINT *pzSock, 
                ZINT iSockType, ZBOOL bDrvTask, ZVOID *pUserInfo);
   
/* mac osx close a socket */
ZFUNC ZINT Zposx_SocketClose(ZSOCKET zSock);

/* mac osx shutdown socket ZINET_SHTDWN_RECV... */
ZFUNC ZINT Zposx_SocketShutdown(ZSOCKET zSock, ZINT iHowTo);

/* mac osx select socket */
ZFUNC ZINT Zposx_SocketSelect(ZINT iMaxFd, ZFD_SET zReadFds, ZFD_SET zWriteFds, 
                ZFD_SET zExceptFds, ZINT iMsecTimeOut, ZINT *piNumFds);

/* mac osx bind socket */
ZFUNC ZINT Zposx_SocketBind(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstLclAddr);

/* mac osx list socket */
ZFUNC ZINT Zposx_SocketListen(ZSOCKET zSock);

/* mac osx accept socket */
ZFUNC ZINT Zposx_SocketAccept(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZBOOL bBlock, ZINT *piNewSock);

/* mac osx connect socket */
ZFUNC ZINT Zposx_SocketConnect(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstAddr,
                ZBOOL bConnTls, ZBOOL *pbRunErr, ZBOOL *pbConned);

/* mac osx socket sendto */
ZFUNC ZINT Zposx_SocketSendTo(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);

/* mac osx socket recvfrom */
ZFUNC ZINT Zposx_SocketRecvFrom(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen);

/* mac osx socket send */
ZFUNC ZINT Zposx_SocketSend(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen, 
                ZBOOL *pbClosed, ZBOOL *pbWait);

/* mac osx socket recv */
ZFUNC ZINT Zposx_SocketRecv(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed);

/* mac osx socket get local address */
ZFUNC ZINT Zposx_SocketGetLocalAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/* mac osx socket get remote address */
ZFUNC ZINT Zposx_SocketGetRemoteAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* mac osx os set socket option block(like ioctl FIONBIO) */
ZFUNC ZINT Zposx_SocketSetOptBlk(ZSOCKET zSock, ZBOOL bBlock);

/* mac osx os set socket option reuse addr(like BSD SO_REUSEADDR) */
ZFUNC ZINT Zposx_SocketSetOptReuseAddr(ZSOCKET zSock, ZBOOL bReuseAddr);

/* mac osx os set socket option receive buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zposx_SocketSetOptRecvBuf(ZSOCKET zSock, ZUINT iBufSize);

/* mac osx os set socket option send buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zposx_SocketSetOptSendBuf(ZSOCKET zSock, ZUINT iBufSize);

/* mac osx os set socket option tos */
ZFUNC ZINT Zposx_SocketSetOptTos(ZSOCKET zSock, ZUINT iTosVal);

/* mac osx os get socket option error(like BSD SO_ERROR) */
ZFUNC ZINT Zposx_SocketGetOptErr(ZSOCKET zSock, ZINT *piErrNo);

/* mac osx os get socket last error */
ZFUNC ZINT Zposx_SocketGetLastErr(ZFUNC_VOID);

/* ******* below api should called by user on asynchronous event *****/

/* mac osx socket action on connect ok */
ZFUNC ZINT Zposx_SocketOnConnOk(ZSOCKET zSock);

/* mac osx socket action on disconnect */
ZFUNC ZINT Zposx_SocketOnDisced(ZSOCKET zSock);

/* mac osx os get access network type */
ZFUNC ZINT Zposx_InetGetAccNetType(ZINT iDefNetType);

#ifdef __cplusplus
}
#endif

#endif /* _ZPOSX_NET_H__ */

