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
#include "servant/RemoteLogger.h"
#include "util/tc_port.h"

#include <sstream>
#include <assert.h>

using namespace std;
using namespace tars;

int NetworkUtil::createSocket(bool udp, bool isLocal, bool isIpv6)
{
#if TARGET_PLATFORM_WINDOWS
    int domain = (isIpv6 ? PF_INET6 : PF_INET);
#else
    int domain = isLocal ? PF_LOCAL : (isIpv6 ? PF_INET6 : PF_INET);
#endif

	int type = udp ? SOCK_DGRAM : SOCK_STREAM;

    TC_Socket s;
    s.createSocket(type, domain);

    if(!udp)
    {
        s.setTcpNoDelay();
	    s.setKeepAlive();
	    s.setNoCloseWait();
    }
    else
    {
    	s.setRecvBufferSize(512*1024);
    }

    s.setOwner(false);
    s.setblock(false);
	return s.getfd();
}

void NetworkUtil::closeSocketNoThrow(int fd)
{
    TC_Port::closeSocket(fd);
}

bool NetworkUtil::doConnect(int fd, const struct sockaddr *addr, socklen_t len)
{
	bool bConnected = false;

	int iRet = ::connect(fd, addr, len);

	if (iRet == 0)
	{
		bConnected  = true;
	}
	else if (!TC_Socket::isInProgress())
	{
        closeSocketNoThrow(fd);
        THROW_EXCEPTION_SYSCODE(TarsNetConnectException, "NetworkUtil::doConnect error");
	}

    return bConnected;
}

// string NetworkUtil::errorToString(int error)
// {
//     return strerror(error);
// }



