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
  File name     : zos_socket.h
  Module        : zos socket library functions
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the socket interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_SOCKET_H__
#define _ZOS_SOCKET_H__

/**
 * @file
 * @brief Zos socket interfaces.
 *
 * These interfaces provide socket operation. They depending on OS interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* utpt transport tls password length */
#define ZOS_SOCK_PASSWD_LEN  64
/* utpt transport tls server name length */
#define ZOS_SOCK_SRV_NAME_LEN  64

/* @brief ZOS tunnel create event callback */
typedef ZVOID (*PFN_TUNLCREATECB)(ZCOOKIE zCookie, ZUCHAR ucEvntType);

/* @brief zos terminal type */
typedef enum EN_SOCK_TER_TYPE
{
    EN_SOCK_TER_CLI,
    EN_SOCK_TER_SERV,
}EN_SOCK_TER_TYPE;

/* @brief socket state type */
typedef enum EN_SOCK_STAT_TYPE
{
    EN_SOCK_STAT_CONNTING,
    EN_SOCK_STAT_HANDSHAKE,
    EN_SOCK_STAT_CONNED,
    EN_SOCK_STAT_DISCED,
}EN_SOCK_STAT_TYPE;

/** @brief ZOS sock type */
typedef enum EN_ZOS_SOCK_TYPE
{
    EN_SOCK_TYPE_OS  = 0,
    EN_SOCK_TYPE_STG,
    EN_SOCK_TYPE_DTLS
}EN_ZOS_SOCK_TYPE;

/** @brief ZOS stg use type */
typedef enum EN_ZOS_STG_USETYPE
{
    EN_STG_USETYPE_CLOSE = 0,
    EN_STG_USETYPE_OPEN,
    EN_STG_USETYPE_PRIORITY_OPEN
}EN_ZOS_STG_USETYPE;

/** @brief ZOS stg tunnel type */
typedef enum EN_SOCK_TSC_TRANSPORT
{
    EN_SOCK_TSC_TRANSPORT_UDP  = 1,
    EN_SOCK_TSC_TRANSPORT_TCP  = 2,
    EN_SOCK_TSC_TRANSPORT_TLS  = 3,
    EN_SOCK_TSC_TRANSPORT_DTLS = 4
} EN_SOCK_TSC_TRANSPORT;

/** @brief ZOS event type of tunnel create result  */
typedef enum EN_ZOS_TUNNEL_STAT_TYPE
{
    EN_ZOS_TUNNEL_STAT_SUCCEED,      /* tunnel create succeed */
    EN_ZOS_TUNNEL_STAT_FAILED        /* tunnel create failed */
} EN_ZOS_TUNNEL_STAT_TYPE;

/* zos socket config */
typedef struct tagZOS_SOCK_CFG
{
    ZBOOL bTlsSock;                  /* tls type socket */
    
    /* tls config */
    ZUINT iTlsMethod;                /* tls method EN_TLS_METHOD_TYPE */
    ZUINT iTlsVeryType;              /* tls verify type EN_TLS_VERY_TYPE */
    ZCHAR *pcTlsTrustCaPath;         /* tls trusted ca path */
    ZCHAR *pcTlsTrustCaFile;         /* tls trusted ca file */
    ZCHAR *pcTlsCertFile;            /* tls certificate file */
    ZCHAR *pcTlsPrvKeyFile;          /* tls private key file */
    ZCHAR acSrvName[ZOS_SOCK_SRV_NAME_LEN];       /* tls extension server name rfc6066 */
    ZCHAR acTlsPrvKeyPasswd[ZOS_SOCK_PASSWD_LEN]; /* private key file password */
} ST_ZOS_SOCK_CFG;

/** @brief Zos network proxy server info. */
typedef struct tagZOS_SOCK_PROXY_SRV_INFO{
    ST_ZOS_INET_ADDR stProxyAddr;       /* proxy server address */
    ZCHAR *pcProxyUsername;          /* stg proxy server username */
    ZCHAR *pcProxyPwd;               /* stg proxy server password */
    ZBOOL bProxy;                    /* stg whether use http proxy */
    ZUCHAR aucSpare[3];              /* for 32 Alignment */
}ST_ZOS_SOCK_PROXY_SRV_INFO;

/* zos socket mutex lock */
ZINT Zos_SocketLock(ZVOID *pSock);

/* zos socket mutex unlock */
ZINT Zos_SocketUnLock(ZVOID *pSock);

/**
 * @brief Init socket struct
 *
 * @param [in] pstSock socket struct.
 * @param [in] iInetType Inet Type,
 * @param [in] iProtoType Protocol type, 
 * @param [in] iSockType socket type,
 *          EN_SOCK_TYPE_OS for os socket,
 *          EN_SOCK_TYPE_STG for stg socket,
 * @param [in] bBlock Open socket with block option.
 */
ZVOID Zos_InitSock(ZVOID *pstSock, ZINT iInetType,
                 ZINT iProtoType, ZINT iSockType, ZBOOL bBlock, ZBOOL bDrvTask);

/**
 * @brief Destroy socket struct
 *
 * @param [in] pSock socket struct.
 */
ZVOID Zos_DesotrySock(ZVOID *pSock);

/**
 * @brief Open a socket.
 *
 * @param [in] iInetType Address family, ZINET_IPV4 for IPv4, ZINET_IPV6 for IPv6.
 * @param [in] iProtoType Protocol type, use ZINET_PROTO_IP, ZINET_PROTO_ICMP,
 *         ZINET_PROTO_TCP, ZINET_PROTO_UDP, 
 *         ZINET_PROTO_RAW or ZINET_PROTO_SCTP.
 * @param [in] iSrvType Service type, 
 *         ZINET_SRV_FOREG for foreground service, 
 *         ZINET_SRV_FBCKG for foreground and backgound service.
 * @param [in] iSockType socket type,
 *          EN_SOCK_TYPE_OS for os socket,
 *          EN_SOCK_TYPE_STG for stg socket,
 * @param [in] bBlock Open socket with block option.
 * @param [in] bReuseAddr Reuse address option.
 * @param [in] bDrvTask socket start select task option.
 * @param [in] pstCfg utpt configs for socket.
 * @param [out] pzSock New created socket.
 *
 * @retval ZOK Open socket successfully.
 * @retval ZFAILED Open socket failed.
 *
 * @see Zos_SocketClose
 */
ZFUNC ZINT Zos_SocketOpen(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZINT iSockType, ZBOOL bBlock, ZBOOL bReuseAddr, 
                ZBOOL bDrvTask, ST_ZOS_SOCK_CFG *pstCfg, ZSOCKET *pzSock);

/**
 * @brief Open a socket.
 *
 * @param [in] iInetType Address family, ZINET_IPV4 for IPv4, ZINET_IPV6 for IPv6.
 * @param [in] iProtoType Protocol type, use ZINET_PROTO_IP, ZINET_PROTO_ICMP,
 *         ZINET_PROTO_TCP, ZINET_PROTO_UDP, 
 *         ZINET_PROTO_RAW or ZINET_PROTO_SCTP.
 * @param [in] iSrvType Service type, 
 *         ZINET_SRV_FOREG for foreground service, 
 *         ZINET_SRV_FBCKG for foreground and backgound service.
 * @param [in] iSockType socket type,
 *          EN_SOCK_TYPE_OS for os socket,
 *          EN_SOCK_TYPE_STG for stg socket,
 * @param [in] bBlock Open socket with block option.
 * @param [in] bReuseAddr Reuse address option.
 * @param [in] bDrvTask socket start select task option.
 * @param [in] iRecvBufSize Receive buffer size option.
 * @param [in] iSendBufSize Send buffer size option.
 * @param [in] iTosVal Tos value option.
 * @param [in] pstLclAddr Address to assign to the socket.
 * @param [in] pstCfg utpt configs for socket.
 * @param [out] pzSock New created socket.
 *
 * @retval ZOK Open socket successfully.
 * @retval ZFAILED Open socket failed.
 *
 * @see Zos_SocketClose
 */
ZFUNC ZINT Zos_SocketOpenX(ZINT iInetType, ZINT iProtoType, ZINT iSrvType,
                ZINT iSockType, ZBOOL bBlock, ZBOOL bReuseAddr, ZBOOL bDrvTask,
                ZUINT iRecvBufSize, ZUINT iSendBufSize, ZUINT iTosVal, 
                ST_ZOS_INET_ADDR *pstLclAddr, ST_ZOS_SOCK_CFG *pstCfg,
                ZSOCKET *pzSock);

/**
 * @brief Close a socket.
 *
 * @param [in] zSock Socket descriptor.
 *
 * @retval ZOK Close successfully.
 * @retval ZFAILED Close failed.
 *
 * @see Zos_SocketOpen
 */
ZFUNC ZINT Zos_SocketClose(ZSOCKET zSock);

/**
 * @brief Disables sends or receives of socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] iHowTo Flag that describes what types of operation 
 *                    will no longer be allowed, use ZINET_SHTDWN_RECV,
 *                    ZINET_SHTDWN_SEND or ZINET_SHTDWN_BOTH.
 *
 * @retval ZOK Shutdown successfully.
 * @retval ZFAILED Shutdown failed.
 *
 * @see Zos_SocketClose
 */
ZFUNC ZINT Zos_SocketShutdown(ZSOCKET zSock, ZINT iHowTo);

/**
 * @brief Determines the status of one or more sockets.
 *
 * @param [in] iMaxFd Max numbered file descriptor in any of three sets plus 1.
 * @param [in] zReadFds Set contains file descriptor will be watched if become
 *                       available for reading.
 * @param [in] zWriteFds Set contains file descriptor will be watched if become
 *                        available for writing.
 * @param [in] zExceptFds Set contains file descriptor will be watched 
 *                         for exception.
 * @param [in] iMsecTimeOut Maximum time for select to wait in milliseconds.
 * @param [out] piNumFds Max numbered file descriptor is ready.
 *
 * @retval ZOK Select successfully.
 * @retval ZFAILED Select failed.
 *
 * @see Zos_SocketOpen
 */
ZFUNC ZINT Zos_SocketSelect(ZINT iMaxFd, ZFD_SET zReadFds, 
                ZFD_SET zWriteFds, ZFD_SET zExceptFds,
                ZINT iMsecTimeOut, ZINT *piNumFds);

/**
 * @brief Associates a local address with a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] pstLclAddr Address to assign to the socket.
 *
 * @retval ZOK Bind successfully.
 * @retval ZFAILED Bind failed.
 *
 * @see Zos_SocketOpen
 */
ZFUNC ZINT Zos_SocketBind(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstLclAddr);

/**
 * @brief Places a socket in a state in which it is listening for 
 *        an incoming connection.
 *
 * @param [in] zSock Socket descriptor.
 *
 * @retval ZOK Listen start successfully.
 * @retval ZFAILED Listen start failed.
 *
 * @see Zos_SocketOpen
 */
ZFUNC ZINT Zos_SocketListen(ZSOCKET zSock);

/**
 * @brief Permits an incoming connection attempt on a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [out] pstRmtAddr Receives the address of the connecting entity.
 * @param [in] bBlock Accpet new socket with block option.
 * @param [out] pzNewSock New socket descriptor.
 *
 * @retval ZOK Listen start successfully.
 * @retval ZFAILED Listen start failed.
 *
 * @see Zos_SocketOpen
 */
ZFUNC ZINT Zos_SocketAccept(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZBOOL bBlock, ZSOCKET *pzNewSock);

/**
 * @brief Establishes a connection to a specified socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] pstRmtAddr Address to which the connection should be established.
 * @param [in] iTlsType tls connect type.
 * @param [in] pstCfg the config for socket .
 * @param [out] pbRunErr Connect error.
 * @param [out] pbConned Tcp has connected.
 *
 * @retval ZOK Connect successfully.
 * @retval ZFAILED Connect failed.
 *
 * @see Zos_SocketOpen
 */
ZFUNC ZINT Zos_SocketConnect(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr,
                ZBOOL bConnTls, ST_ZOS_SOCK_CFG *pstCfg, ZBOOL *pbRunErr, 
                ZBOOL *pbConned);

/**
 * @brief Send data to a specific destination.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] pstRmtAddr Address of the target socket.
 * @param [in] pucBuf Buffer containing the data to be transmitted.
 * @param [in,out] piLen Length of the data in the pucBuf parameter,
 *                       return bytes transmitted.
 * @param [out] pbClosed Socket has closed.
 * @param [out] pbWait Send data need wait for send comlete.
 *
 * @retval ZOK Transmit data successfully.
 * @retval ZFAILED Transmit data failed.
 *
 * @see Zos_SocketRecvFrom
 */
ZFUNC ZINT Zos_SocketSendTo(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);

/**
 * @brief Receive a datagram from the source address.
 *
 * @param [in] zSock Socket descriptor.
 * @param [out] pstRmtAddr Address of source address.
 * @param [out] pucBuf Buffer for the incoming data.
 * @param [in,out] piLen Length of the pucBuf parameter,
 *                       return bytes received.
 *
 * @retval ZOK Receive data successfully.
 * @retval ZFAILED Receive data failed.
 *
 * @see Zos_SocketSendTo
 */
ZFUNC ZINT Zos_SocketRecvFrom(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstRmtAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen);

/**
 * @brief Send data on a connected socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] pucBuf Buffer containing the data to be transmitted.
 * @param [in,out] piLen Length of the data in the pucBuf parameter,
 *                       return bytes transmitted.
 * @param [out] pbClosed Socket has closed.
 * @param [out] pbWait Send data need wait for send comlete.
 *
 * @retval ZOK Transmit data successfully.
 * @retval ZFAILED Transmit data failed.
 *
 * @see Zos_SocketRecv
 */
ZFUNC ZINT Zos_SocketSend(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed, ZBOOL *pbWait);

/**
 * @brief Send buffer data on a connected socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] pucBuf Buffer containing the data to be transmitted.
 * @param [in,out] piLen Length of the data in the pucBuf parameter,
 *                       return bytes transmitted.
 * @param [out] pbClosed Socket has closed.
 * @param [out] pbWait Send data need wait for send comlete.
 *
 * @retval ZOK Transmit data successfully.
 * @retval ZFAILED Transmit data failed.
 *
 * @see Zos_SocketRecv
 */
ZFUNC ZINT Zos_SocketSendX(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed, ZBOOL *pbWait);

/**
 * @brief Receive data from a connected socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [out] pucBuf Buffer for the incoming data.
 * @param [in,out] piLen Length of the pucBuf parameter,
 *                       return bytes received.
 * @param [out] pbClosed Socket has closed.
 *
 * @retval ZOK Receive data successfully.
 * @retval ZFAILED Receive data failed.
 *
 * @see Zos_SocketSend
 */
ZFUNC ZINT Zos_SocketRecv(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed);

/**
 * @brief Get the local address from a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [out] pstLclAddr The local address of a socket.
 *
 * @retval ZOK Get local address successfully.
 * @retval ZFAILED Get local address failed.
 *
 * @see Zos_SocketGetRemoteAddr
 */
ZFUNC ZINT Zos_SocketGetLocalAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLclAddr);

/**
 * @brief Get the remote address from a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [out] pstRmtAddr The remote address of a socket.
 *
 * @retval ZOK Get remote address successfully.
 * @retval ZFAILED Get remote address failed.
 *
 * @see Zos_SocketGetLocalAddr
 */
ZFUNC ZINT Zos_SocketGetRemoteAddr(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);

/**
 * @brief Set block option of a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] bBlock Block option.
 *
 * @retval ZOK Set block option successfully.
 * @retval ZFAILED Set block option failed.
 *
 * @see Zos_SocketSetOptReuseAddr
 */
ZFUNC ZINT Zos_SocketSetOptBlk(ZSOCKET zSock, ZBOOL bBlock);

/**
 * @brief Set reuse address option of a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] bReuseAddr Reuse address option.
 *
 * @retval ZOK Set reuse address option successfully.
 * @retval ZFAILED Set reuse address option failed.
 *
 * @see Zos_SocketSetOptBlk
 */
ZFUNC ZINT Zos_SocketSetOptReuseAddr(ZSOCKET zSock, ZBOOL bReuseAddr);

/**
 * @brief Set receive buffer option of a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] iBufSize Receive buffer size.
 *
 * @retval ZOK Set receive buffer option successfully.
 * @retval ZFAILED Set receive buffer option failed.
 *
 * @see Zos_SocketSetOptSendBuf
 */
ZFUNC ZINT Zos_SocketSetOptRecvBuf(ZSOCKET zSock, ZUINT iBufSize);

/**
 * @brief Set send buffer option of a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] iBufSize Send buffer size.
 *
 * @retval ZOK Set send buffer option successfully.
 * @retval ZFAILED Set send buffer option failed.
 *
 * @see Zos_SocketSetOptRecvBuf
 */
ZFUNC ZINT Zos_SocketSetOptSendBuf(ZSOCKET zSock, ZUINT iBufSize);

/**
 * @brief Set tos option of a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] iTosVal Tos value option.
 *
 * @retval ZOK Set tos option successfully.
 * @retval ZFAILED Set tos option failed.
 *
 * @see Zos_SocketSetOptBlk
 */
ZFUNC ZINT Zos_SocketSetOptTos(ZSOCKET zSock, ZUINT iTosVal);

/**
 * @brief Set ipv6 tclass option of a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [in] iTclassVal ipv6 tclass value option.
 *
 * @retval ZOK Set ipv6 tclass option successfully.
 * @retval ZFAILED Set ipv6 tclass option failed.
 *
 * @see Zos_SocketSetOptBlk
 */
ZFUNC ZINT Zos_SocketSetOptTclass(ZSOCKET zSock, ZUINT iTclassVal);

/**
 * @brief Get error option of a socket.
 *
 * @param [in] zSock Socket descriptor.
 * @param [out] piErrNo Socket error value.
 *
 * @retval ZOK Get error option successfully.
 * @retval ZFAILED Get error option failed.
 *
 * @see Zos_SocketGetLastErr
 */
ZFUNC ZINT Zos_SocketGetOptErr(ZSOCKET zSock, ZINT *piErrNo);

/**
 * @brief Get the last error of a socket.
 *
 * @retval The last error of a socket.
 *
 * @see Zos_SocketGetOptErr
 */
ZFUNC ZINT Zos_SocketGetLastErr(ZFUNC_VOID);

/**
 * @brief Create stg tunnel.
 *
 * @param [in] pfnCb tunnel callback.
 * @param [in] dwCookie reg cookie.
 * @param [in] iTunlType stg tunnel type.
 * @param [in] pstAddr stg tunnel address.
 * @param [in] pstStgProxyInfo stg tunnel proxy info.
 * @param [in] pstCfg tls configs for socket.
 *
 * @retval ZOK Create stg tunnel  successfully.
 * @retval ZFAILED Set Create stg tunnel failed.
 *
 * @see Zos_SocketCloseTunl
 */
ZFUNC ZINT Zos_SocketCreateTunl(PFN_TUNLCREATECB pfnCb, ZCOOKIE zCookie,
                ZINT iTunlType, ST_ZOS_INET_ADDR *pstAddr,
                ST_ZOS_SOCK_PROXY_SRV_INFO *pstStgProxyInfo,
                ST_ZOS_SOCK_CFG *pstCfg);

/**
 * @brief Close stg tunnel.
 *
 * @see Zos_SocketCreateTunl
 */
ZFUNC ZVOID Zos_SocketCloseTunl(ZFUNC_VOID);

/**
 * @brief start stg tunnel create task.
 *
 * @retval ZOK start stg tunnel  task successfully.
 * @retval ZFAILED start stg tunnel task failed.
 *
 * @see Zos_SocketStopTunlTask
 */
ZFUNC ZINT Zos_SocketStartTunlTask(ZFUNC_VOID);

/**
 * @brief stop stg tunnel task.
 *
 * @see Zos_SocketStartTunlTask
 */
ZFUNC ZVOID Zos_SocketStopTunlTask(ZFUNC_VOID);

/**
 * @brief Get real socket.
 *
 * @param [in] zSock Socket descriptor.
 *
 * @retval real socket get real socket successfully.
 * @retval ZSOCKET_INVALID get real socket failed.
 *
 */
ZFUNC ZINT Zos_SocketGetRealSock(ZSOCKET zSock);


/**
 * @brief Start tls.
 *
 * @param [in] zSock Socket descriptor.
 *
 * @retval ZOK start tls successfully.
 * @retval ZFAILED start tls failed.
 *
 */
ZFUNC ZINT Zos_SocketStartTls(ZSOCKET zSock);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_SOCKET_H__ */

