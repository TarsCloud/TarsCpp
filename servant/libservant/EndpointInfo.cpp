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
	_ep = toEndpointF(ep);

	_cmpDesc = createCompareDesc();
	_desc = createDesc();
}

string EndpointInfo::createCompareDesc()
{
	stringstream ss;
	ss << _ep.getType() << ":" << _ep.getHost() << ":" << _ep.getPort();

	return ss.str();
}

TC_Endpoint EndpointInfo::toEndpointF(const EndpointF &ep)
{
    TC_Endpoint tEp;
    tEp.setHost(ep.host);
    tEp.setPort(ep.port);
    tEp.setTimeout(ep.timeout);
    tEp.setType((TC_Endpoint::EType)ep.istcp);
    tEp.setGrid(ep.grid);
    tEp.setQos(ep.qos);
    tEp.setWeight(ep.weight);
    tEp.setWeightType(ep.weightType);

    tEp.setAuthType((TC_Endpoint::AUTH_TYPE)ep.authType);

    return tEp;
}

EndpointF EndpointInfo::toEndpointF(const TC_Endpoint &tep, const string &nodeName)
{
    EndpointF ep;
    ep.host = tep.getHost();
    ep.port = tep.getPort();
    ep.istcp = tep.getType();
    ep.nodeName = nodeName;
    if(ep.nodeName.empty())
    {
        ep.nodeName = tep.getHost();
    }
    ep.timeout = tep.getTimeout();
    ep.authType = tep.getAuthType();
    ep.qos = tep.getQos();
    ep.weight = tep.getWeight();
    ep.weightType = tep.getWeightType();

    return ep;
}

vector<EndpointF> EndpointInfo::toEndpointFs(const TC_Endpoint &ep, const string &nodeName)
{
    //如果ep的host是0.0.0.0, 则使用本地ip地址
    string host = ep.getHost();
    if (host == "0.0.0.0")
    {
        vector<EndpointF> v;
        vector<string> ips = TC_Socket::getLocalHosts(AF_INET);

        for(const auto &ip : ips)
        {
            if(ip == "127.0.0.1")
            {
                continue;
            }

            TC_Endpoint theEp = ep;
            theEp.setHost(ip);

            v.push_back(EndpointInfo::toEndpointF(theEp, nodeName));
        }

        return v;
    }
    else if (host == "::1")
    {
        vector<EndpointF> v;
        vector<string> ips = TC_Socket::getLocalHosts(AF_INET6);

        for(const auto &ip : ips)
        {
            if(ip == "::1")
            {
                continue;
            }

            TC_Endpoint theEp = ep;
            theEp.setHost(ip);

            v.push_back(EndpointInfo::toEndpointF(theEp, nodeName));
        }

        return v;
    }
    else if (host == "*")
    {
        vector<EndpointF> v;
        {
            vector<string> ips = TC_Socket::getLocalHosts(AF_INET);

            for (const auto &ip: ips)
            {
                if (ip == "127.0.0.1")
                {
                    continue;
                }

                TC_Endpoint theEp = ep;
                theEp.setHost(ip);

                v.push_back(EndpointInfo::toEndpointF(theEp, nodeName));
            }
        }

        {
            vector<string> ips = TC_Socket::getLocalHosts(AF_INET6);

            for (const auto &ip: ips)
            {
                if (ip == "::1")
                {
                    continue;
                }

                TC_Endpoint theEp = ep;
                theEp.setHost(ip);

                v.push_back(EndpointInfo::toEndpointF(theEp, nodeName));
            }
        }

        return v;
    }
    else
    {
        return {EndpointInfo::toEndpointF(ep, nodeName)};
    }
}

string EndpointInfo::vectorEndpointFToStr(const vector<EndpointF> &eps, bool complete)
{
    ostringstream os;
    if(!complete)
    {
        for (size_t i = 0; i < eps.size(); i++)
        {
            os << eps[i].host << ":" << eps[i].port;
            if (i != eps.size() - 1)
            {
                os << ",";
            }
        }
    }
    else
    {
        for (size_t i = 0; i < eps.size(); i++)
        {
            os << toEndpointF(eps[i]).toString();
            if (i != eps.size() - 1)
            {
                os << ":";
            }
        }
    }
    return os.str();
}

vector<EndpointF> EndpointInfo::strToVectorEndpointF(const string &str, const string &nodeName)
{
    vector<string> eps = TC_Endpoint::sepEndpoint(str);

    //遍历转换格式
    vector<EndpointF> epsF;
    for (size_t i = 0; i < eps.size(); i++)
    {
        TC_Endpoint ep(eps[i]);

        epsF.push_back(EndpointInfo::toEndpointF(ep, nodeName));
    }
    return epsF;
}

map<string, vector<EndpointF>> EndpointInfo::toNodeEndpointF(const vector<tars::EndpointF> &eps)
{
    map<string, vector<EndpointF>> data;
    for(auto ep : eps)
    {
        if(ep.nodeName.empty())
        {
            //兼容旧框架, 没有赋值nodeName
            ep.nodeName = ep.host;
        }

        data[ep.nodeName].push_back(ep);
    }
    return data;
}

///////////////////////////////////////////////////////////
}
