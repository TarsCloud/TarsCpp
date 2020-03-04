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

#include "servant/Global.h"
#include "util/tc_socket.h"
#include "AuthF.h"

#if TARGET_PLATFORM_WINDOWS
#include <WS2tcpip.h>
#endif

using namespace std;

namespace tars
{

//enum AUTH_STATE;
//////////////////////////////////////////////////////////////////////////////
/**
 * 地址信息IP:Port
 */
class EndpointInfo
{
public:
//    enum EType { UDP = TC_ClientSocket::, TCP = 1, SSL = 2};

    /**
     * 构造函数
     */
    EndpointInfo();

    /**
     * 构造函数
     * @param host
     * @param port
     * @param type
     */
    EndpointInfo(const string& host, uint16_t port, TC_Endpoint::EType type, int32_t grid, const string & setDivision, int qos, int weight = -1, unsigned int weighttype = 0, int authType = 0);

    /**
     * get endpoint
     * @return
     */
	const TC_Endpoint &getEndpoint() const { return _ep; }
//    /**
//     * 地址的字符串描述,不带set信息
//     *
//     * @return string
//     */
//     const string& descNoSetInfo() const;

    /**
     * 地址的字符串描述
     *
     * @return string
     */
    const string & desc() const
    {
        return _desc;
    }

//    /**
//     * 比较的地址的字符串描述
//     *
//     * @return string
//     */
//    const string & compareDesc() const
//    {
//        return _cmpDesc;
//    }

    /**
     * 详细地址字符串描述
     * 
     * @return string 
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

    /**
     * 解析域名 
     */
    void parseAddress();

    /**
     * 获取主机地址
     *
     * @return const struct sockaddr_in&
     */
    const struct sockaddr_in& addr() const;

    /**
     * Get ipv4 or ipv6 struct sockaddr
     *
     * @return const struct sockaddr *
     */
    const struct sockaddr * addrPtr() const;

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

//    /**
//    *等于,set信息不参与比较
//    *@param r
//    *
//    *@return bool
//    */
//    bool equalNoSetInfo(const EndpointInfo& r) const;

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
    TC_Endpoint _ep;

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
