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

#include "servant/NetworkUtil.h"
#include "servant/Global.h"
#include "util/tc_epoller.h"

#include <sys/epoll.h>
#include <sstream>
#include <assert.h>

using namespace std;
using namespace tars;

int NetworkUtil::createSocket(bool udp, bool isLocal/* = false*/, bool isIpv6/* = false*/)
{
    int domain = isLocal ? PF_LOCAL : (isIpv6 ? PF_INET6 : PF_INET);
    int type = udp ? SOCK_DGRAM : SOCK_STREAM;
    int protocol = udp ? IPPROTO_UDP : IPPROTO_TCP;
    int fd = socket(domain, type, protocol);

    if (fd == INVALID_SOCKET)
    {
        ostringstream os;
        os << "createSocket ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TarsNetSocketException(os.str());
    }

    if(!udp)
    {
        setTcpNoDelay(fd);

        setKeepAlive(fd);
    }

    return fd;
}

void NetworkUtil::closeSocketNoThrow(int fd)
{
    int error = errno;
    close(fd);
    errno = error;
}

void NetworkUtil::setBlock(int fd, bool block)
{
    if (block)
    {
        int flags = fcntl(fd, F_GETFL);
        flags &= ~O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == SOCKET_ERROR)
        {
            closeSocketNoThrow(fd);
            ostringstream os;
            os << "setBlock ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TarsNetSocketException(os.str());
        }
    }
    else
    {
        int flags = fcntl(fd, F_GETFL);
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == SOCKET_ERROR)
        {
            closeSocketNoThrow(fd);
            ostringstream os;
            os << "setBlock ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
            throw TarsNetSocketException(os.str());
        }
    }
}

void NetworkUtil::setTcpNoDelay(int fd)
{
    int flag = 1;
    if(setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "setTcpNoDelay ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TarsNetSocketException(os.str());
    }
}

void NetworkUtil::setKeepAlive(int fd)
{
    int flag = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "setKeepAlive ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TarsNetSocketException(os.str());
    }
}

void NetworkUtil::doBind(int fd, struct sockaddr_in& addr)
{
    if(bind(fd, reinterpret_cast<struct sockaddr*>(&addr), int(sizeof(addr))) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "doBind ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TarsNetSocketException(os.str());
    }
    socklen_t len = static_cast<socklen_t>(sizeof(addr));
    getsockname(fd, reinterpret_cast<struct sockaddr*>(&addr), &len);
}

void NetworkUtil::doBind(int fd, const struct sockaddr *addr, socklen_t len)
{
    if (bind(fd, addr, len) == SOCKET_ERROR)
    {
        closeSocketNoThrow(fd);
        ostringstream os;
        os << "doBind ex:(" << errorToString(errno) << ")" << __FILE__ << ":" << __LINE__;
        throw TarsNetSocketException(os.str());
    }
}

bool NetworkUtil::doConnect(int fd, const struct sockaddr_in& addr)
{
    bool bConnected = false;

    int iRet = ::connect(fd, (struct sockaddr*)(&addr), int(sizeof(addr)));

    if (iRet == 0)
    {
        bConnected  = true;
    }
    else if (iRet == -1 && errno != EINPROGRESS)
    {
        ::close(fd);
        throw TarsNetConnectException(strerror(errno));
    }

    return bConnected;
}

bool NetworkUtil::doConnect(int fd, const struct sockaddr *addr, socklen_t len)
{
    int iRet = ::connect(fd, addr, len);

    if (iRet == -1 && errno != EINPROGRESS)
    {
        ::close(fd);
        throw TarsNetConnectException(strerror(errno));
    }

    return iRet == 0 ? true : false;
}

void NetworkUtil::getAddress(const string& host, int port, struct sockaddr_in& addr)
{
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());

    if(addr.sin_addr.s_addr == INADDR_NONE)
    {
        struct addrinfo* info = 0;
        int retry = 5;

        struct addrinfo hints = { 0 };
        hints.ai_family = PF_INET;

        int rs = 0;
        do
        {
            rs = getaddrinfo(host.c_str(), 0, &hints, &info);
        }
        while(info == 0 && rs == EAI_AGAIN && --retry >= 0);

        if(rs != 0)
        {
            ostringstream os;
            os << "DNSException ex:(" << errorToString(errno) << ")" << rs << ":" << host << ":" << __FILE__ << ":" << __LINE__;
            if(info != NULL)
            {
                freeaddrinfo(info);
            }
            throw TarsNetSocketException(os.str());
        }

        assert(info != NULL);
        assert(info->ai_family == PF_INET);
        struct sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(info->ai_addr);

        addr.sin_addr.s_addr = sin->sin_addr.s_addr;
        freeaddrinfo(info);
    }
}


void NetworkUtil::getAddress(const string& host, int port, struct sockaddr_in6& addr)
{
#define RETRY_TIMES   (5)
    struct addrinfo *res = NULL;
    struct addrinfo hints;

    memset(&addr, 0, sizeof(addr));
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET6;

    ostringstream os;
    os << port;

    int ret = -1;
    int retry = RETRY_TIMES;
    do
    {
        ret = getaddrinfo(host.c_str(), os.str().c_str(), &hints, &res);
    }
    while (NULL == res && ret == EAI_AGAIN && --retry >= 0);

    if (ret)
    {
        ostringstream ex;
        ex << "DNSException ex:(" << errorToString(errno) << ":" << gai_strerror(ret) << ":" << ret << ")" <<
            "|" << host << ":" << port << "| at " << __FUNCTION__ << ":" << __LINE__;
        if (res)
        {
            freeaddrinfo(res);
        }
        throw TarsNetSocketException(ex.str());
    }
    assert(res);
    assert(res->ai_family == PF_INET6);
    memcpy(&addr, res->ai_addr, sizeof(addr));
    freeaddrinfo(res);
#undef RETRY_TIMES
}

string NetworkUtil::errorToString(int error)
{
    return strerror(error);
}


