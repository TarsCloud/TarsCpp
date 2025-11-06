/************************************************************************

        Copyright (c) 2005-2011 by Juphoon System Software, Inc.
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
  Created on    : 2010-02-28    
  Description   :
    Data structure and function definitions required by socket 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPIOS_NET_NS_H__
#define _ZPIOS_NET_NS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* zos ios support ns stream */
#ifdef ZOS_SUPT_IOS_NS

#define EN_IOS_TPT_BUF_LEN 16354

/* trust certificate evaluate */
ZFUNC ZBOOL Zpios_NsTrustEvaluate(ZUCHAR *pucCertData, ZUINT iCertLen);

/* ios inet addr */
ZFUNC ZINT Zpios_NsInetAddr(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);

#ifdef ZOS_SUPT_IPV6
ZFUNC ZINT Zpios_NsInetAddr6(ZCONST ZCHAR *pcAddrStr, ZUCHAR *piAddr);
#endif

/* ios inet ntohl */
ZFUNC ZUINT Zpios_NsInetNtohl(ZUINT iVal);

/* ios inet htonl */
ZFUNC ZUINT Zpios_NsInetHtonl(ZUINT iVal);

/* ios inet ntohs */
ZFUNC ZUSHORT Zpios_NsInetNtohs(ZUSHORT iVal);

/* ios inet htons */
ZFUNC ZUSHORT Zpios_NsInetHtons(ZUSHORT iVal);

/* ios inet fd create */
ZFUNC ZINT Zpios_NsInetFdCreate(ZFD_SET *pzFds);

/* ios inet fd delete */
ZFUNC ZINT Zpios_NsInetFdDelete(ZFD_SET zFds);

/* ios inet fd copy */
ZFUNC ZINT Zpios_NsInetFdCopy(ZFD_SET zDstFds, ZFD_SET zSrcFds);

/* ios inet fd zero */
ZFUNC ZINT Zpios_NsInetFdZero(ZFD_SET zFds);

/* ios inet fd set */
ZFUNC ZINT Zpios_NsInetFdSet(ZFD_SET zFds, ZINT iFd);

/* ios inet fd isset */
ZFUNC ZBOOL Zpios_NsInetFdIsset(ZFD_SET zFds, ZINT iFd);

/* ios inet fd clear */
ZFUNC ZINT Zpios_NsInetFdClr(ZFD_SET zFds, ZINT iFd);

/* ios inet get host by name */
ZFUNC ZINT Zpios_NsInetGetHostByName(ZCHAR *pcName, 
                ST_ZOS_INET_IPS *pstIps);

/* ios inet check wifi state */
ZFUNC ZBOOL Zpios_NsInetCheckIpIsLocalAndWifi(ZUINT iIp);

#ifdef ZOS_SUPT_IPV6
ZFUNC ZBOOL Zpios_NsInetCheckIpv6IsLocalAndWifi(ZUCHAR* iIpv6);
#endif

/* ios inet get local ip address */
ZFUNC ZINT Zpios_NsInetGetLocalIp(ST_ZOS_INET_IPS *pstIps);

/* ios create a socket */
ZFUNC ZINT Zpios_NsSocketNew(ST_ZPIOS_SOCK_CFG *pstCfg, ZINT iProtoType,
                ZSOCKET *pzSock);

/* ios os create a socket, inet type see ZINET_IPV4..., 
 protocol type seee ZINET_PROTO_UDP... */
ZFUNC ZINT Zpios_NsSocketOpen(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZBOOL bBlock, ZBOOL bReuseAddr, 
                ZSOCKET *pzSock, ZVOID *pUserInfo);

/* ios cease a socket */
ZFUNC ZINT Zpios_NsSocketCease(ZSOCKET zSock);

/* ios close a socket */
ZFUNC ZINT Zpios_NsSocketClose(ZSOCKET zSock);

/* ios shutdown socket ZINET_SHTDWN_RECV... */
ZFUNC ZINT Zpios_NsSocketShutdown(ZSOCKET zSock, ZINT iHowTo);

/* ios select socket */
ZFUNC ZINT Zpios_NsSocketSelect(ZINT iMaxFd, ZFD_SET zReadFds, 
                ZFD_SET zWriteFds, ZFD_SET zExceptFds, ZINT iMsecTimeOut, 
                ZINT *piNumFds);

/* ios bind socket */
ZFUNC ZINT Zpios_NsSocketBind(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/* ios list socket */
ZFUNC ZINT Zpios_NsSocketListen(ZSOCKET zSock);

/* ios accept socket */
ZFUNC ZINT Zpios_NsSocketAccept(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZBOOL bBlock, ZSOCKET *pzNewSock);

/* ios connect socket */
ZFUNC ZINT Zpios_NsSocketConnect(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZBOOL bConnTls, ZBOOL *pbRunErr, 
                ZBOOL *pbConned);

/* ios socket sendto */
ZFUNC ZINT Zpios_NsSocketSendTo(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucBuf, ZUINT *piLen, 
                ZBOOL *pbClosed, ZBOOL *pbWait);

/* ios socket recvfrom */
ZFUNC ZINT Zpios_NsSocketRecvFrom(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr, ZUCHAR *pucBuf, ZUINT *piLen);

/* ios socket send */
ZFUNC ZINT Zpios_NsSocketSend(ZSOCKET zSock, ZUCHAR *pucBuf, 
                ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);

/* ios socket recv */
ZFUNC ZINT Zpios_NsSocketRecv(ZSOCKET zSock, ZUCHAR *pucBuf, 
                ZUINT *piLen, ZBOOL *pbClosed);

/* ios socket get local address */
ZFUNC ZINT Zpios_NsSocketGetLocalAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/* ios socket get remote address */
ZFUNC ZINT Zpios_NsSocketGetRemoteAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/* ios os set socket option block(like ioctl FIONBIO) */
ZFUNC ZINT Zpios_NsSocketSetOptBlk(ZSOCKET zSock, ZBOOL bBlock);

/* ios os set socket option reuse addr(like BSD SO_REUSEADDR) */
ZFUNC ZINT Zpios_NsSocketSetOptReuseAddr(ZSOCKET zSock, 
                ZBOOL bReuseAddr);

/* ios os set socket option tos */
ZFUNC ZINT Zpios_NsSocketSetOptTos(ZSOCKET zSock, ZUINT iTosVal);

/* ios os set socket option tclass */
ZFUNC ZINT Zpios_NsSocketSetOptTclass(ZSOCKET zSock, ZUINT iTclassVal);

/* ios os set multicast address */
ZFUNC ZINT Zpios_NsSocketSetOptMAddr(ZSOCKET zSock, ST_ZOS_INET_IP *pstAddr);

/* ios os get socket option error(like BSD SO_ERROR) */
ZFUNC ZINT Zpios_NsSocketGetOptErr(ZSOCKET zSock, ZINT *piErrNo);

/* ios os get socket last error */
ZFUNC ZINT Zpios_NsSocketGetLastErr(ZFUNC_VOID);

/* ******* below api should called by ZPLIN user on asynchronous event *****/
/* ios socket on tls handshake ok */
ZFUNC ZINT Zpios_NsSocketTlsHandshakeOk(ZSOCKET zSock);

/* ios socket proc tls received data */
ZFUNC ZINT Zpios_NsSocketProcTlsData(ZSOCKET zSock);

/* ios socket action on connect ok */
ZFUNC ZINT Zpios_NsSocketOnConnOk(ZSOCKET zSock);
    
/* ios socket action on disconnect */
ZFUNC ZINT Zpios_NsSocketOnDisced(ZSOCKET zSock);
    
/* ios socket action on accept */
ZFUNC ZINT Zpios_NsSocketOnAccepted(ZSOCKET zSock, ZSOCKET zNewSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);
            
/* ios socket action on receive data event */
ZFUNC ZINT Zpios_NsSocketOnReadable(ZSOCKET zSock);

/* ios socket action on writable event */
ZFUNC ZINT Zpios_NsSocketOnWritable(ZSOCKET zSock);

#endif /* ZOS_SUPT_IOS_NS */
    
#ifdef __cplusplus
}
#endif

#endif /* _ZPIOS_NET_NS_H__ */

