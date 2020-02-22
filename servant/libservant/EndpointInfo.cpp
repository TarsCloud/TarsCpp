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

#include "servant/EndpointInfo.h"
#include "servant/TarsLogger.h"
#include "servant/NetworkUtil.h"
#include "util/tc_socket.h"

namespace tars
{
EndpointInfo::EndpointInfo()
{
    _setDivision.clear();
//    memset(&_addr,0,sizeof(struct sockaddr_in));
	memset(&_addr,0,sizeof(_addr));
    //7暂时写死 修改默认值一定要修改这个7
//    memcpy(_host,"0.0.0.0",7);
	//snprintf(_host, sizeof(_host), "%s", "0.0.0.0");
}


EndpointInfo::EndpointInfo(const string& host, uint16_t port, TC_Endpoint::EType type, int32_t grid, const string & setDivision, int qos, int weight, unsigned int weighttype, int authType)
: _ep(host, port, 60000, type, grid, qos, weight, weighttype, authType)
, _setDivision(setDivision)
, _addressSucc(false)
{


    _cmpDesc = createCompareDesc();

    _desc = createDesc();
}

void EndpointInfo::parseAddress()
{
    // try
    // {
        if (isIPv6())
        {
            TC_Socket::parseAddrWithPort(_ep.getHost(), _ep.getPort(), _addr.in6);
        }
        else
        {
            TC_Socket::parseAddrWithPort(_ep.getHost(), _ep.getPort(), _addr.in);
        }
    // }
    // catch (...)
    // {
    //     TLOGERROR("EndpointInfo::parseAddress fail:" << _host << ":" << _port << "]" << endl);
    // }
}

string EndpointInfo::createCompareDesc()
{
	return _ep.toString();

}

string EndpointInfo::createDesc() const
{
	return _ep.toString();
}

bool EndpointInfo::operator == (const EndpointInfo& r) const
{
    return (_cmpDesc == r._cmpDesc);
}

bool EndpointInfo::operator < (const EndpointInfo& r) const
{
    return (_cmpDesc < r._cmpDesc);
}

const string &EndpointInfo::host() const
{
    return _ep.getHost();
}

int32_t EndpointInfo::grid() const
{
    return _ep.getGrid();
}


uint16_t EndpointInfo::port() const
{
    return _ep.getPort();
}

bool EndpointInfo::isTcp() const
{
	return _ep.isTcp();
}

const struct sockaddr_in& EndpointInfo::addr() const
{
    return _addr.in;
}

const struct sockaddr * EndpointInfo::addrPtr() const
{
	return _ep.isIPv6() ? (struct sockaddr *)&_addr.in6 : (struct sockaddr *)&_addr.in;
}
//
//EndpointInfo::EType EndpointInfo::type() const
//{
//    return _type;
//}

const string& EndpointInfo::setDivision() const
{
    return _setDivision;
}
///////////////////////////////////////////////////////////
}
