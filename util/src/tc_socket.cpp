/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cassert>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "util/tc_socket.h"
#include "util/tc_common.h"

namespace tars
{

TC_Socket::TC_Socket() : _sock(INVALID_SOCKET), _bOwner(true), _iDomain(AF_INET)
{
}

TC_Socket::~TC_Socket()
{
    if(_bOwner)
    {
        close();
    }
}

void TC_Socket::init(int fd, bool bOwner, int iDomain)
{
    if(_bOwner)
    {
        close();
    }

    _sock       = fd;
    _bOwner     = bOwner;
    _iDomain    = iDomain;
}

void TC_Socket::createSocket(int iSocketType, int iDomain)
{
    assert(iSocketType == SOCK_STREAM || iSocketType == SOCK_DGRAM);
    close();

    _iDomain    = iDomain;
    _sock       = socket(iDomain, iSocketType, 0);

    if(_sock < 0)
    {
        _sock = INVALID_SOCKET;
        throw TC_Socket_Exception("[TC_Socket::createSocket] create socket error! :" + string(strerror(errno)));
    }
}

void TC_Socket::getPeerName(string &sPeerAddress, uint16_t &iPeerPort)
{
    assert(_iDomain == AF_INET || _iDomain == AF_INET6);

    char sAddr[INET6_ADDRSTRLEN] = "\0";
    struct sockaddr_in stPeer4;
    struct sockaddr_in6 stPeer6;
    struct sockaddr *stPeer = (AF_INET6 == _iDomain) ? (struct sockaddr *)&stPeer6 : (struct sockaddr *)&stPeer4;
    socklen_t iPeerLen = (AF_INET6 == _iDomain) ? sizeof(stPeer6) : sizeof(stPeer4);

    bzero(stPeer, iPeerLen);
    getPeerName(stPeer, iPeerLen);
    inet_ntop(_iDomain, (AF_INET6 == _iDomain) ? (const void*)&stPeer6.sin6_addr : (const void *)&stPeer4.sin_addr, sAddr, sizeof(sAddr));
    sPeerAddress = sAddr;
    iPeerPort = (AF_INET6 == _iDomain) ? ntohs(stPeer6.sin6_port) : ntohs(stPeer4.sin_port);
}

void TC_Socket::getPeerName(string &sPathName)
{
    assert(_iDomain == AF_LOCAL);

    struct sockaddr_un stSock;
    bzero(&stSock, sizeof(struct sockaddr_un));
    socklen_t iSockLen = sizeof(stSock);
    getPeerName((struct sockaddr *)&stSock, iSockLen);

    sPathName = stSock.sun_path;
}

void TC_Socket::getPeerName(struct sockaddr *pstPeerAddr, socklen_t &iPeerLen)
{
    if(getpeername(_sock, pstPeerAddr, &iPeerLen) < 0)
    {
        throw TC_Socket_Exception("[TC_Socket::getPeerName] getpeername error", errno);
    }
}

void TC_Socket::getSockName(string &sSockAddress, uint16_t &iSockPort)
{
    assert(_iDomain == AF_INET || _iDomain == AF_INET6);

    char sAddr[INET6_ADDRSTRLEN] = "\0";
    struct sockaddr_in6 in6;
    struct sockaddr_in in4;
    struct sockaddr *in = (AF_INET6 == _iDomain) ? (struct sockaddr *)&in6 : (struct sockaddr *)&in4;
    socklen_t len = (AF_INET6 == _iDomain) ? sizeof(in6) : sizeof(in4);

    bzero(in, len);
    getSockName(in, len);
    inet_ntop(_iDomain, (AF_INET6 == _iDomain) ? (const void *)&in6.sin6_addr : (const void *)&in4.sin_addr, sAddr, sizeof(sAddr));
    sSockAddress = sAddr;
    iSockPort = (AF_INET6 == _iDomain) ? ntohs(in6.sin6_port) : ntohs(in4.sin_port);
}

void TC_Socket::getSockName(string &sPathName)
{
    assert(_iDomain == AF_LOCAL);

    struct sockaddr_un stSock;
    bzero(&stSock, sizeof(struct sockaddr_un));
    socklen_t iSockLen = sizeof(stSock);
    getSockName((struct sockaddr *)&stSock, iSockLen);

    sPathName = stSock.sun_path;
}

void TC_Socket::getSockName(struct sockaddr *pstSockAddr, socklen_t &iSockLen)
{
    if(getsockname(_sock, pstSockAddr, &iSockLen) < 0)
    {
        throw TC_Socket_Exception("[TC_Socket::getSockName] getsockname error", errno);
    }
}

int TC_Socket::accept(TC_Socket &tcSock, struct sockaddr *pstSockAddr, socklen_t &iSockLen)
{
    assert(tcSock._sock == INVALID_SOCKET);

    int ifd;

    while ((ifd = ::accept(_sock, pstSockAddr, &iSockLen)) < 0 && errno == EINTR);

    tcSock._sock    = ifd;
    tcSock._iDomain = _iDomain;

    return tcSock._sock;
}

void TC_Socket::parseAddr(const string &sAddr, struct in_addr &stSinAddr)
{
    int iRet = inet_pton(AF_INET, sAddr.c_str(), &stSinAddr);
    if(iRet < 0)
    {
        throw TC_Socket_Exception("[TC_Socket::parseAddr] inet_pton error", errno);
    }
    else if(iRet == 0)
    {
        struct hostent stHostent;
        struct hostent *pstHostent;
        char buf[2048] = "\0";
        int iError;

        gethostbyname_r(sAddr.c_str(), &stHostent, buf, sizeof(buf), &pstHostent, &iError);

        if (pstHostent == NULL)
        {
            throw TC_Socket_Exception("[TC_Socket::parseAddr] gethostbyname_r error! :" + string(hstrerror(iError)));
        }
        else
        {
            stSinAddr = *(struct in_addr *) pstHostent->h_addr;
        }
    }
}

void TC_Socket::parseAddr(const string &sAddr, struct in6_addr &stSinAddr)
{
    int iRet = inet_pton(AF_INET6, sAddr.c_str(), &stSinAddr);
    if(iRet < 0)
    {
        throw TC_Socket_Exception("[TC_Socket::parseAddr6] inet_pton error", errno);
    }
    else if(iRet == 0)
    {
        struct hostent stHostent;
        struct hostent *pstHostent;
        char buf[2048] = "\0";
        int iError;

        gethostbyname2_r(sAddr.c_str(), AF_INET6, &stHostent, buf, sizeof(buf), &pstHostent, &iError);

        if (pstHostent == NULL)
        {
            throw TC_Socket_Exception("[TC_Socket::parseAddr6] gethostbyname2_r error! :" + string(hstrerror(iError)));
        }
        else
        {
            if (pstHostent->h_addrtype != AF_INET6)
            {
                throw TC_Socket_Exception("[TC_Socket::parseAddr6] gethostbyname2_r return addrtype is not AF_INET6");
            }
            stSinAddr = *(struct in6_addr *) pstHostent->h_addr;
        }
    }
}

void TC_Socket::bind(const string &sServerAddr, int port)
{
    assert(_iDomain == AF_INET || _iDomain == AF_INET6);

    struct sockaddr_in6 bindAddr6;
    struct sockaddr_in bindAddr4;
    struct sockaddr *bindAddr = (AF_INET6 == _iDomain) ? (struct sockaddr *)&bindAddr6 : (struct sockaddr *)&bindAddr4;
    socklen_t len = (AF_INET6 == _iDomain) ? sizeof(bindAddr6) : sizeof(bindAddr4);

    bzero(bindAddr, len);

    if (AF_INET6 == _iDomain)
    {
        bindAddr6.sin6_family   = _iDomain;
        bindAddr6.sin6_port     = htons(port);

        if (sServerAddr == "")
        {
            bindAddr6.sin6_addr = in6addr_any;
        }
        else
        {
            parseAddr(sServerAddr, bindAddr6.sin6_addr);
        }
    }
    else
    {
        bindAddr4.sin_family   = _iDomain;
        bindAddr4.sin_port     = htons(port);

        if (sServerAddr == "")
        {
            bindAddr4.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else
        {
            parseAddr(sServerAddr, bindAddr4.sin_addr);
        }
    }

    try
    {
        bind(bindAddr, len);
    }
    catch(...)
    {
        throw TC_Socket_Exception("[TC_Socket::bind] bind '" + sServerAddr + ":" + TC_Common::tostr(port) + "' error", errno);
    }
}

void TC_Socket::bind(const char *sPathName)
{
    assert(_iDomain == AF_LOCAL);

    unlink(sPathName);

    struct sockaddr_un stBindAddr;
    bzero(&stBindAddr, sizeof(struct sockaddr_un));
    stBindAddr.sun_family = _iDomain;
    strncpy(stBindAddr.sun_path, sPathName, sizeof(stBindAddr.sun_path));

    try
    {
        bind((struct sockaddr *)&stBindAddr, sizeof(stBindAddr));
    }
    catch(...)
    {
        throw TC_Socket_Exception("[TC_Socket::bind] bind '" + string(sPathName) + "' error", errno);
    }
}

void TC_Socket::bind(struct sockaddr *pstBindAddr, socklen_t iAddrLen)
{
    //如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间
    int iReuseAddr = 1;

    //设置
    setSockOpt(SO_REUSEADDR, (const void *)&iReuseAddr, sizeof(int), SOL_SOCKET);

    if(::bind(_sock, pstBindAddr, iAddrLen) < 0)
    {
        throw TC_Socket_Exception("[TC_Socket::bind] bind error", errno);
    }
}

void TC_Socket::close()
{
    if (_sock != INVALID_SOCKET)
    {
        ::close(_sock);
        _sock = INVALID_SOCKET;
    }
}

int TC_Socket::connectNoThrow(const string &sServerAddr, uint16_t port)
{
    assert(_iDomain == AF_INET || _iDomain == AF_INET6);

    if (sServerAddr == "")
    {
        throw TC_Socket_Exception("[TC_Socket::connect] server address is empty error!");
    }

    struct sockaddr_in6 serverAddr6;
    struct sockaddr_in serverAddr4;
    struct sockaddr *serverAddr = (AF_INET6 == _iDomain) ? (struct sockaddr *)&serverAddr6 : (struct sockaddr *)&serverAddr4;
    socklen_t len = (AF_INET6 == _iDomain) ? sizeof(serverAddr6) : sizeof(serverAddr4);

    bzero(serverAddr, len);

    if (AF_INET6 == _iDomain)
    {
        serverAddr6.sin6_family = _iDomain;
        parseAddr(sServerAddr, serverAddr6.sin6_addr);
        serverAddr6.sin6_port = htons(port);
    }
    else
    {
        serverAddr4.sin_family = _iDomain;
        parseAddr(sServerAddr, serverAddr4.sin_addr);
        serverAddr4.sin_port = htons(port);
    }

    return connect(serverAddr, len);
}

int TC_Socket::connectNoThrow(struct sockaddr* addr)
{
    assert(_iDomain == AF_INET || _iDomain == AF_INET6);

    return connect(addr, (AF_INET6 == _iDomain) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in));
}

void TC_Socket::connect(const string &sServerAddr, uint16_t port)
{
    int ret = connectNoThrow(sServerAddr, port);

    if(ret < 0)
    {
        throw TC_SocketConnect_Exception("[TC_Socket::connect] connect error", errno);
    }
}

void TC_Socket::connect(const char *sPathName)
{
    int ret = connectNoThrow(sPathName);
    if(ret < 0)
    {
        throw TC_SocketConnect_Exception("[TC_Socket::connect] connect error", errno);
    }
}

int TC_Socket::connectNoThrow(const char *sPathName)
{
    assert(_iDomain == AF_LOCAL);

    struct sockaddr_un stServerAddr;
    bzero(&stServerAddr, sizeof(struct sockaddr_un));
    stServerAddr.sun_family = _iDomain;
    strncpy(stServerAddr.sun_path, sPathName, sizeof(stServerAddr.sun_path));

    return connect((struct sockaddr *)&stServerAddr, sizeof(stServerAddr));
}

int TC_Socket::connect(struct sockaddr *pstServerAddr, socklen_t serverLen)
{
    return ::connect(_sock, pstServerAddr, serverLen);

}

void TC_Socket::listen(int iConnBackLog)
{
    if (::listen(_sock, iConnBackLog) < 0)
    {
        throw TC_Socket_Exception("[TC_Socket::listen] listen error", errno);
    }
}

int TC_Socket::recv(void *pvBuf, size_t iLen, int iFlag)
{
    return ::recv(_sock, pvBuf, iLen, iFlag);
}

int TC_Socket::send(const void *pvBuf, size_t iLen, int iFlag)
{
    return ::send(_sock, pvBuf, iLen, iFlag);
}

int TC_Socket::recvfrom(void *pvBuf, size_t iLen, string &sFromAddr, uint16_t &iFromPort, int iFlags)
{
    int iBytes;
    struct sockaddr_in6 stFromAddr6;
    struct sockaddr_in stFromAddr4;
    struct sockaddr *stFromAddr = (AF_INET6 == _iDomain) ? (struct sockaddr *)&stFromAddr6 : (struct sockaddr *)&stFromAddr4;
    socklen_t iFromLen = (AF_INET6 == _iDomain) ? sizeof(stFromAddr6) : sizeof(stFromAddr4);

    bzero(stFromAddr, iFromLen);
    iBytes = recvfrom(pvBuf, iLen, stFromAddr, iFromLen, iFlags);
    if (iBytes >= 0)
    {
        char sAddr[INET6_ADDRSTRLEN] = "\0";
        inet_ntop(_iDomain, (AF_INET6 == _iDomain) ? (const void *)&stFromAddr6.sin6_addr : (const void *)&stFromAddr4.sin_addr, sAddr, sizeof(sAddr));
        sFromAddr = sAddr;
        iFromPort = (AF_INET6 == _iDomain) ? ntohs(stFromAddr6.sin6_port) : ntohs(stFromAddr4.sin_port);
    }
    return iBytes;
}

int TC_Socket::recvfrom(void *pvBuf, size_t iLen, struct sockaddr *pstFromAddr, socklen_t &iFromLen, int iFlags)
{
    return ::recvfrom(_sock, pvBuf, iLen, iFlags, pstFromAddr, &iFromLen);
}

int TC_Socket::sendto(const void *pvBuf, size_t iLen, const string &sToAddr, uint16_t port, int iFlags)
{
    struct sockaddr_in6 toAddr6;
    struct sockaddr_in toAddr4;
    struct sockaddr *toAddr = (AF_INET6 == _iDomain) ? (struct sockaddr *)&toAddr6 : (struct sockaddr *)&toAddr4;
    socklen_t len = (AF_INET6 == _iDomain) ? sizeof(toAddr6) : sizeof(toAddr4);

    bzero(toAddr, len);
    if (AF_INET6 == _iDomain)
    {
        toAddr6.sin6_family = _iDomain;

        if (sToAddr == "")
        {
            //toAddr.sin6_addr = in6addr_linklocal_allrouters;
        }
        else
        {
            parseAddr(sToAddr, toAddr6.sin6_addr);
        }
        toAddr6.sin6_port = htons(port);
    }
    else
    {
        toAddr4.sin_family = _iDomain;

        if (sToAddr == "")
        {
            toAddr4.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        }
        else
        {
            parseAddr(sToAddr, toAddr4.sin_addr);
        }

        toAddr4.sin_port = htons(port);
    }

    return sendto(pvBuf, iLen, toAddr, len, iFlags);
}

int TC_Socket::sendto(const void *pvBuf, size_t iLen, struct sockaddr *pstToAddr, socklen_t iToLen, int iFlags)
{
    return ::sendto(_sock, pvBuf, iLen, iFlags, pstToAddr, iToLen);
}

void TC_Socket::shutdown(int iHow)
{
    if (::shutdown(_sock, iHow) < 0)
    {
        throw TC_Socket_Exception("[TC_Socket::shutdown] shutdown error", errno);
    }
}

void TC_Socket::setblock(bool bBlock)
{
    assert(_sock != INVALID_SOCKET);

    setblock(_sock, bBlock);
}

int TC_Socket::setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level)
{
    return setsockopt(_sock, level, opt, pvOptVal, optLen);
}

int TC_Socket::getSockOpt(int opt, void *pvOptVal, socklen_t &optLen, int level)
{
    return getsockopt(_sock, level, opt, pvOptVal, &optLen);
}

void TC_Socket::setNoCloseWait()
{
    linger stLinger;
    stLinger.l_onoff = 1;  //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
    stLinger.l_linger = 0; //容许逗留的时间为0秒

    if(setSockOpt(SO_LINGER, (const void *)&stLinger, sizeof(linger), SOL_SOCKET) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setNoCloseWait] error", errno);
    }
}

void TC_Socket::setCloseWait(int delay)
{
    linger stLinger;
    stLinger.l_onoff = 1;  //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
    stLinger.l_linger = delay; //容许逗留的时间为delay秒

    if(setSockOpt(SO_LINGER, (const void *)&stLinger, sizeof(linger), SOL_SOCKET) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setCloseWait] error", errno);
    }
}

void TC_Socket::setCloseWaitDefault()
{
    linger stLinger;
    stLinger.l_onoff  = 0;
    stLinger.l_linger = 0;

    if(setSockOpt(SO_LINGER, (const void *)&stLinger, sizeof(linger), SOL_SOCKET) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setCloseWaitDefault] error", errno);
    }
}

void TC_Socket::setTcpNoDelay()
{
    int flag = 1;

    if(setSockOpt(TCP_NODELAY, (char*)&flag, int(sizeof(int)), IPPROTO_TCP) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setTcpNoDelay] error", errno);
    }
}

void TC_Socket::setKeepAlive()
{
    int flag = 1;
    if(setSockOpt(SO_KEEPALIVE, (char*)&flag, int(sizeof(int)), SOL_SOCKET) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setKeepAlive] error", errno);
    }
}

void TC_Socket::setSendBufferSize(int sz)
{
    if(setSockOpt(SO_SNDBUF, (char*)&sz, int(sizeof(int)), SOL_SOCKET) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setSendBufferSize] error", errno);
    }
}

int TC_Socket::getSendBufferSize()
{
    int sz;
    socklen_t len = sizeof(sz);
    if(getSockOpt(SO_SNDBUF, (void*)&sz, len, SOL_SOCKET) == -1 || len != sizeof(sz))
    {
        throw TC_Socket_Exception("[TC_Socket::getSendBufferSize] error", errno);
    }

    return sz;
}

void TC_Socket::setRecvBufferSize(int sz)
{
    if(setSockOpt(SO_RCVBUF, (char*)&sz, int(sizeof(int)), SOL_SOCKET) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setRecvBufferSize] error", errno);
    }
}

int TC_Socket::getRecvBufferSize()
{
    int sz;
    socklen_t len = sizeof(sz);
    if(getSockOpt(SO_RCVBUF, (void*)&sz, len, SOL_SOCKET) == -1 || len != sizeof(sz))
    {
        throw TC_Socket_Exception("[TC_Socket::getRecvBufferSize] error", errno);
    }

    return sz;
}

void TC_Socket::setblock(int fd, bool bBlock)
{
    int val = 0;

    if ((val = fcntl(fd, F_GETFL, 0)) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setblock] fcntl [F_GETFL] error", errno);
    }

    if(!bBlock)
    {
        val |= O_NONBLOCK;
    }
    else
    {
        val &= ~O_NONBLOCK;
    }

    if (fcntl(fd, F_SETFL, val) == -1)
    {
        throw TC_Socket_Exception("[TC_Socket::setblock] fcntl [F_SETFL] error", errno);
    }
}

void TC_Socket::createPipe(int fds[2], bool bBlock)
{
    if(::pipe(fds) != 0)
    {
        throw TC_Socket_Exception("[TC_Socket::createPipe] error", errno);
    }

    try
    {
        setblock(fds[0], bBlock);
        setblock(fds[1], bBlock);
    }
    catch(...)
    {
        ::close(fds[0]);
        ::close(fds[1]);
        throw;
    }
}

vector<string> TC_Socket::getLocalHosts(int domain)
{
    vector<string> result;

    TC_Socket ts;
    ts.createSocket(SOCK_STREAM, domain);

    int cmd = SIOCGIFCONF;

    struct ifconf ifc;

    int numaddrs = 10;

    int old_ifc_len = 0;

    while(true)
    {
        int bufsize = numaddrs * static_cast<int>(sizeof(struct ifreq));
        ifc.ifc_len = bufsize;
        ifc.ifc_buf = (char*)malloc(bufsize);
        int rs = ioctl(ts.getfd(), cmd, &ifc);

        if(rs == -1)
        {
            free(ifc.ifc_buf);
            throw TC_Socket_Exception("[TC_Socket::getLocalHosts] ioctl error", errno);
        }
        else if(ifc.ifc_len == old_ifc_len)
        {
            break;
        }
        else
        {
            old_ifc_len = ifc.ifc_len;
        }
    
        numaddrs += 10;
        free(ifc.ifc_buf);
    }

    numaddrs = ifc.ifc_len / static_cast<int>(sizeof(struct ifreq));
    struct ifreq* ifr = ifc.ifc_req;
    for(int i = 0; i < numaddrs; ++i)
    {
        if(ifr[i].ifr_addr.sa_family == AF_INET)
        {
            struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(&ifr[i].ifr_addr);
            if(addr->sin_addr.s_addr != 0)
            {
                char sAddr[INET_ADDRSTRLEN] = "\0";
                inet_ntop(AF_INET, &(*addr).sin_addr, sAddr, sizeof(sAddr));
                result.push_back(sAddr);
            }
        }
        else if (ifr[i].ifr_addr.sa_family == AF_INET6)
        {
            struct sockaddr_in6* addr = reinterpret_cast<struct sockaddr_in6*>(&ifr[i].ifr_addr);
            if(!memcmp(&addr->sin6_addr, &in6addr_any, sizeof(addr->sin6_addr)))
            {
                char sAddr[INET6_ADDRSTRLEN] = "\0";
                inet_ntop(AF_INET6, &(*addr).sin6_addr, sAddr, sizeof(sAddr));
                result.push_back(sAddr);
            }
        }
    }

    free(ifc.ifc_buf);

    return result;
}

bool addressIsIPv6(const string& addr, bool def_value)
{
#define IPv6_ADDRESS_CHAR ':'
    return (addr.find(IPv6_ADDRESS_CHAR) != string::npos) ? true : false;
#undef IPv6_ADDRESS_CHAR
}


}
