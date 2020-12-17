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

#ifndef __TARS_ENDPOINT_INFO_H_
#define __TARS_ENDPOINT_INFO_H_

//#include "servant/Global.h"
#include "util/tc_socket.h"
#include "util/tc_clientsocket.h"
#include "EndpointF.h"
//#include "AuthF.h"

#if TARGET_PLATFORM_WINDOWS
#include <WS2tcpip.h>
#endif

using namespace std;

namespace tars
{
/**
 * 地址信息IP:Port
 */
class EndpointInfo
{
public:

    /**
     * 构造函数
     */
    EndpointInfo();

    /**
     *
     * @param ep
     */
	EndpointInfo(const TC_Endpoint &ep, const string &setDivision="");

	/**
     * 构造函数
     * @param host
     * @param port
     * @param type
     */
	EndpointInfo(const EndpointF &ep);

    /**
     * get endpoint
     * @return
     */
	const TC_Endpoint &getEndpoint() const { return _ep; }

	/**
	 *
	 * @return
	 */
	TC_Endpoint &getEndpoint() { return _ep; }

	/**
	 * 设置代理地址
	 * @param ep
	 */
	void setProxyEndpoint(const TC_Endpoint &ep) { _epProxy.reset(new TC_Endpoint(ep)); }

	/**
	 * 获取代理地址, 没有则为NULL
	 */
	const TC_Endpoint *getProxyEndpoint() const { return _epProxy.get(); }

	/**
	 * 获取连接地址
	 * @return
	 */
	const TC_Endpoint *getConnectEndpoint() const { return _epProxy? _epProxy.get() : &_ep; }

	/**
	 * @brief is ipv6 socket or not
	 * @return true if is ipv6
	 */
	bool isConnectIPv6() const  { return getConnectEndpoint()->isIPv6(); }

	/**
	 * 解析域名
	 */
	void parseConnectAddress();

	/**
     * 地址的字符串描述(用于比较)
     *
     * @return string
     */
	const string & cmpDesc() const
	{
		return _cmpDesc;
	}

	/**
     * 地址的字符串描述
     *
     * @return string
     */
    const string & desc() const
    {
        return _desc;
    }

    /**
     *
     * @return
     */
	bool isTcp() const;

	/**
     * 获取主机名
     *
     * @return const string&
     */
    const string &host() const;

    /**
     * 获取端口号
     *
     * @return uint16_t
     */
    uint16_t port() const;

    /**
     * 获取路由状态
     * @return int32_t
     */
    int32_t grid() const;

    /*
     * 获取qos的descp值
     */
    int32_t qos() const {return _ep.getQos();}

    /*
     * 获取节点的静态权重值
     */
    int weight() const {return _ep.getWeight();}

    /**
     * @brief 获取节点的权重使用方式
     */
    unsigned int getWeightType() const { return _ep.getWeightType(); }

//    /**
//     * 获取主机地址
//     *
//     * @return const struct sockaddr_in&
//     */
//    const struct sockaddr_in& addr() const;

    /**
     * Get ipv4 or ipv6 struct sockaddr
     *
     * @return const struct sockaddr *
     */
    const struct sockaddr * connectAddrPtr() const;

    /**
     * 返回端口类型
     *
     * @return EndpointInfo::EType
     */
    TC_Endpoint::EType type() const { return _ep.getType(); }

    /**
    *设置set分组信息
    *
    *@return void
    */
    void setDivision(const string& sSetDivision);

    /**
    *返回set分组信息
    *
    *@return string
    */
    const string& setDivision() const;

    /*
     * 获取认证类型
     */
    int authType() const  { return _ep.getAuthType(); }

    /**
     * @brief is ipv6 socket or not
     * @return true if is ipv6
     */
    bool isIPv6() const  { return _ep.isIPv6(); }

    /**
     * 等于
     * @param r
     *
     * @return bool
     */
    bool operator == (const EndpointInfo& r) const;

    /**
     * 小于
     * @param r
     *
     * @return bool
     */
    bool operator < (const EndpointInfo& r) const;

protected:

    /**
     * 地址的字符串描述
     * @param bWithSetInfo,标识
     * @return string
     */
    string createDesc() const;

    /**
     * 详细地址字符串描述
     * 
     * @return string 
     */
    string createCompareDesc();

private:
	/**
	 * 服务地址
	 */
    TC_Endpoint _ep;

    /**
     * 代理地址
     */
	shared_ptr<TC_Endpoint> _epProxy;

	/**
	 *set分组信息
	 */
    string                 _setDivision;

    /**
     * 地址
     */
    union
    {
        struct sockaddr_in     in;
        struct sockaddr_in6    in6;
    } _addr;

    /**
     * 比较的地址字符串描述
     */
    string                 _cmpDesc;

    /**
     * 地址的字符串描述
     */
    string                 _desc;


    /**
     * 解析域名成功
     */ 
    bool                   _addressSucc;
};
/////////////////////////////////////////////////////////////////////////////
}
#endif
