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
#include "servant/RemoteLogger.h"
//#include "servant/NetworkUtil.h"
#include "util/tc_socket.h"

namespace tars
{

EndpointInfo::EndpointInfo()
{
    _setDivision.clear();
	// memset(&_addr,0,sizeof(_addr));
}

EndpointInfo::EndpointInfo(const TC_Endpoint &ep, const string &setDivision)
: _ep(ep)
, _setDivision(setDivision)
// , _addressSucc(false)
{
	_cmpDesc = createCompareDesc();
	_desc = createDesc();
}

EndpointInfo::EndpointInfo(const EndpointF &ep)
: _setDivision(ep.setId)
{
	_ep.setHost(ep.host);
	_ep.setPort(ep.port);
	_ep.setTimeout(ep.timeout);
	_ep.setType((TC_Endpoint::EType)ep.istcp);
	_ep.setGrid(ep.grid);
	_ep.setQos(ep.qos);
	_ep.setWeight(ep.weight);
	_ep.setWeightType(ep.weightType);

	_ep.setAuthType((TC_Endpoint::AUTH_TYPE)ep.authType);

	_cmpDesc = createCompareDesc();
	_desc = createDesc();

}


string EndpointInfo::createCompareDesc()
{
	stringstream ss;
	ss << _ep.getType() << ":" << _ep.getHost() << ":" << _ep.getPort();

	return ss.str();
}

///////////////////////////////////////////////////////////
}
