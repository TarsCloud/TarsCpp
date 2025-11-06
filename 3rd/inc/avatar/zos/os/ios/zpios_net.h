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
  File name     : zpios_net.h
  Module        : ios socket library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by socket 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPIOS_NET_H__
#define _ZPIOS_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

/* trust certificate evaluate */
ZFUNC ZBOOL Zpios_TrustEvaluate(ZUCHAR *pucCertData, ZUINT iCertLen);

/* ios start tunl create task */
ZFUNC ZINT Zpios_StartTunlTask(ZFUNC_VOID);

/* ios stop tunl create task */
ZFUNC ZVOID Zpios_StopTunlTask(ZFUNC_VOID);

/* ios create tunl */
ZFUNC ZINT Zpios_CreateTunl(PFN_TUNLCREATECB pfnCb, ZCOOKIE zCookie, 
                ZINT iTunlType, ST_ZOS_INET_ADDR *pstAddr,
                ST_ZOS_SOCK_PROXY_SRV_INFO *pstStgProxyInfo,
                ST_ZOS_SOCK_CFG *pstCfg);

/* ios close tunl */
ZFUNC ZVOID Zpios_CloseTunl(ZFUNC_VOID);

/* ios inet addr */
ZFUNC ZINT Zpios_InetAddr(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);

/* ios inet ntohl */
ZFUNC ZUINT Zpios_InetNtohl(ZUINT iVal);

/* ios inet htonl */
ZFUNC ZUINT Zpios_InetHtonl(ZUINT iVal);

/* ios inet ntohs */
ZFUNC ZUSHORT Zpios_InetNtohs(ZUSHORT iVal);

/* ios inet htons */
ZFUNC ZUSHORT Zpios_InetHtons(ZUSHORT iVal);

/* ios inet fd create */
ZFUNC ZINT Zpios_InetFdCreate(ZFD_SET *pzFds);

/* ios inet fd delete */
ZFUNC ZINT Zpios_InetFdDelete(ZFD_SET zFds);

/* ios inet fd copy */
ZFUNC ZINT Zpios_InetFdCopy(ZFD_SET zDstFds, ZFD_SET zSrcFds);

/* ios inet fd zero */
ZFUNC ZINT Zpios_InetFdZero(ZFD_SET zFds);

/* ios inet fd set */
ZFUNC ZINT Zpios_InetFdSet(ZFD_SET zFds, ZINT iFd);

/* ios inet fd isset */
ZFUNC ZBOOL Zpios_InetFdIsset(ZFD_SET zFds, ZINT iFd);

/* ios inet fd clear */
ZFUNC ZINT Zpios_InetFdClr(ZFD_SET zFds, ZINT iFd);

/* ios inet get host by name */
ZFUNC ZINT Zpios_InetGetHostByName(ZCHAR *pcName, ST_ZOS_INET_IPS *pstIps);

/* ios inet get local ip address */
ZFUNC ZINT Zpios_InetGetLocalIp(ST_ZOS_INET_IPS *pstIps, ZINT iIpType);

/* ios inet get access network type */
ZFUNC ZINT Zpios_InetGetAccNetType(ZINT iDefNetType);

/* ios os create a socket, inet type see ZINET_IPV4..., 
   protocol type seee ZINET_PROTO_UDP... */
ZFUNC ZINT Zpios_SocketOpen(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZBOOL bBlock, ZBOOL bReuseAddr, 
                ZINT *piSock, ZINT iSockType, ZBOOL bDrvTask,
                ZVOID *pUserInfo);
   
/* ios cease a socket */
ZFUNC ZINT Zpios_SocketCease(ZSOCKET zSock);

/* ios close a socket */
ZFUNC ZINT Zpios_SocketClose(ZSOCKET zSock);

/* ios shutdown socket ZINET_SHTDWN_RECV... */
ZFUNC ZINT Zpios_SocketShutdown(ZSOCKET zSock, ZINT iHowTo);

/* ios select socket */
ZFUNC ZINT Zpios_SocketSelect(ZINT iMaxFd, ZFD_SET zReadFds, ZFD_SET zWriteFds, 
                ZFD_SET zExceptFds, ZINT iMsecTimeOut, ZINT *piNumFds);

/* ios bind socket */
ZFUNC ZINT Zpios_SocketBind(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstLclAddr);

/* ios list socket */
ZFUNC ZINT Zpios_SocketListen(ZSOCKET zSock);

/* ios accept socket */
ZFUNC ZINT Zpios_SocketAccept(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZBOOL bBlock, ZINT *piNewSock);

/* ios connect socket */
ZFUNC ZINT Zpios_SocketConnect(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr,
                ZBOOL bConnTls, ZBOOL *pbRunErr, ZBOOL *pbConned);

/* ios socket sendto */
ZFUNC ZINT Zpios_SocketSendTo(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);

/* ios socket recvfrom */
ZFUNC ZINT Zpios_SocketRecvFrom(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen);

/* ios socket send */
ZFUNC ZINT Zpios_SocketSend(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen, 
                ZBOOL *pbClosed, ZBOOL *pbWait);

/* ios socket recv */
ZFUNC ZINT Zpios_SocketRecv(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed);

/* ios socket get local address */
ZFUNC ZINT Zpios_SocketGetLocalAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/* ios socket get remote address */
ZFUNC ZINT Zpios_SocketGetRemoteAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* ios os set socket option block(like ioctl FIONBIO) */
ZFUNC ZINT Zpios_SocketSetOptBlk(ZSOCKET zSock, ZBOOL bBlock);

/* ios os set socket option reuse addr(like BSD SO_REUSEADDR) */
ZFUNC ZINT Zpios_SocketSetOptReuseAddr(ZSOCKET zSock, ZBOOL bReuseAddr);

/* ios os set socket option receive buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zpios_SocketSetOptRecvBuf(ZSOCKET zSock, ZUINT iBufSize);

/* ios os set socket option send buffer size(like BSD SO_RCVBUF) */
ZFUNC ZINT Zpios_SocketSetOptSendBuf(ZSOCKET zSock, ZUINT iBufSize);

/* ios os set socket option tos */
ZFUNC ZINT Zpios_SocketSetOptTos(ZSOCKET zSock, ZUINT iTosVal);

/* ios os set socket option tclass */
ZFUNC ZINT Zpios_SocketSetOptTclass(ZSOCKET zSock, ZUINT iTclassVal);

/* ios os get socket option error(like BSD SO_ERROR) */
ZFUNC ZINT Zpios_SocketGetOptErr(ZSOCKET zSock, ZINT *piErrNo);

/* ios os get socket last error */
ZFUNC ZINT Zpios_SocketGetLastErr(ZFUNC_VOID);

/* ******* below api should called by ZPLIN user on asynchronous event *****/
/* ios socket action on closed */
ZFUNC ZINT Zpios_SocketOnClose(ZSOCKET zSock);

/* ios socket action on connect ok */
ZFUNC ZINT Zpios_SocketOnConnOk(ZSOCKET zSock);

/* ios socket action on connect failed */
ZFUNC ZINT Zpios_SocketOnConnFail(ZSOCKET zSock);

/* ios socket action on disconnect */
ZFUNC ZINT Zpios_SocketOnDisced(ZSOCKET zSock);

/* ios socket action on accept */
ZFUNC ZINT Zpios_SocketOnAccept(ZSOCKET zSock);

/* ios socket action on receive udp data event */
ZFUNC ZINT Zpios_SocketOnRecvUdp(ZSOCKET zSock);

/* ios socket action on receive tcp data event */
ZFUNC ZINT Zpios_SocketOnRecvTcp(ZSOCKET zSock);

#ifdef __cplusplus
}
#endif

#endif /* _ZPIOS_NET_H__ */

