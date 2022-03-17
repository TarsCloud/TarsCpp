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

#ifndef TC_CPP_PROXYINFO_H
#define TC_CPP_PROXYINFO_H

#include "util/tc_clientsocket.h"

namespace tars
{

/**
 * sock4 & sock5 & http代理协议支持, 注意sock5的udp模式没有测试过!
 * 注意: TC_ProxyInfo 中包含状态机, 使用时, 每个连接都应该有一个该对象, 并且连接断掉要调用reset重置
 *
 * 使用方式:
 * 1 构建对应的代理对象, 丢给指针TC_ProxyInfo*
 * 2 发送网络包之前, 检查isSuccess
 * 3 如果不成功, 则调用sendProxyPacket, 构建鉴权包,发送给代理
 * 4 接受响应包, 检查isSuccess
 * 5 如果不成功, 则调用recvProxyPacket, 解析响应包
 * 6 检查isSuccess, 如果不成功, 则跳转到3, 直到isSuccess
 * 7 期间检查sendProxyPacket & recvProxyPacket的返回值, 如果为false, 则表示鉴权不通过.
 */
class TC_ProxyInfo
{
public:
	TC_ProxyInfo(const TC_Endpoint & ep, const string & user, const string & pass)
		: _stage(eProxy_Stage_Establish), _ep(ep), _user(user), _pass(pass)
	{
	}

	virtual ~TC_ProxyInfo() {}

	/**
	 * 设置代理类型
	 */
    enum EMProxyType
    {
        eProxy_Type_Sock4,
        eProxy_Type_Sock5,
        eProxy_Type_Http,
    };

    /**
     * 设置代理阶段
     */
	enum EMProxyStageType
	{
		eProxy_Stage_Establish,
		eProxy_Stage_ACK1,
		eProxy_Stage_ACK2,
		eProxy_Stage_Connected,
		eProxy_Stage_DisConn,
	};

	/**
	 * 代理的基本信息
	 */
    struct ProxyBaseInfo
    {
        EMProxyType     type;
        TC_Endpoint     ep;
        std::string     user;
        std::string     pass;
    };

    typedef std::function<void(EMProxyStageType)> ProxyStageChangedCallback;

	/**
	 * 创建socket信息
	 * @param type
	 * @param ep
	 * @param user
	 * @param pass
	 * @return
	 */
	static shared_ptr<TC_ProxyInfo> createProxyInfo(const ProxyBaseInfo &baseInfo);

	/**
	 * 返回代理地址
	 * @return
	 */
	const TC_Endpoint & getEndpoint() const { return _ep; }

	/**
	 * 返回代理地址
	 * @return
	 */
	TC_Endpoint & getEndpoint() { return _ep; }

	/**
	 * 是否成功
	 * @return
	 */
    bool isSuccess() { return _stage == eProxy_Stage_Connected; }

    /**
     * 重置状态
     */
    void reset() { _stage = eProxy_Stage_Establish; }

    /**
     * 获取错误信息, sendProxyPacket & recvProxyPacket 返回false, 则可以通过getErrMsg获取错误信息
     * @return
     */
    const string &getErrMsg() { return _errMsg; }

    /**
	 * 构建鉴权包
	 * @param buff
	 * @param dst
	 * @return
	 */
	virtual bool sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst) = 0;

	/**
	 * 解析响应包
	 * @param buff
	 * @param length
	 * @return
	 */
	virtual bool recvProxyPacket(const char *buff, size_t length) = 0;

protected:
	void onDisconnect();
	void onConnSuccess();
	void setProxyStage(EMProxyStageType proxyStage);

protected:
	EMProxyStageType _stage;
	string		_errMsg;
	TC_Endpoint _ep;
	std::string _user;
	std::string _pass;
};

class TC_ProxySock4 : public TC_ProxyInfo
{
public:
	TC_ProxySock4(const TC_Endpoint & ep)
		: TC_ProxyInfo(ep, "", "")
	{
	}

protected:
	static const int kProxy_Sock4_Req1_VN = 4;
	static const int kProxy_Sock4_Req1_CD = 1;

	static const int kProxy_Sock4_Ans1_VN = 0;
	static const int kProxy_Sock4_Ans1_CD = 90;

	struct sock4req1
	{
		char VN;
		char CD;
		char DSTPORT[2];
		char DSTIP[4];
		char USERID[1];
	};

	struct sock4ans1
	{
		char VN;
		char CD;
		char DSTPORT[2];
		char DSTIP[4];
	};

public:
	virtual bool sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst);

	virtual bool recvProxyPacket(const char *buff, size_t length);
};

class TC_ProxySock5 : public TC_ProxyInfo
{
public:
	TC_ProxySock5(const TC_Endpoint & ep, const string & user, const string & pass)
		: TC_ProxyInfo(ep, user, pass)
	{
	}
protected:
	static const int kProxy_Sock5_Req1_Ver = 5;
	static const int kProxy_Sock5_Req1_nMethods = 2;
	static const int kProxy_Sock5_Req1_nMethods0 = 0;       //NO AUTHENTICATION REQUIRED
	static const int kProxy_Sock5_Req1_nMethods1 = 2;       //USERNAME/PASSWORD

	static const int kProxy_Sock5_Req3_Ver = 5;
	static const int kProxy_Sock5_Req3_Cmd = 1;
	static const int kProxy_Sock5_Req3_Rsv = 0;
	static const int kProxy_Sock5_Req3_AtypIpv4 = 1;
	static const int kProxy_Sock5_Req3_AtypDns  = 3;
	static const int kProxy_Sock5_Req3_AtypIpv6 = 4;

	static const int kProxy_Sock5_Ans1_Ver = 5;
	static const int kProxy_Sock5_Ans1_Method_Anonymous = 0;
	static const int kProxy_Sock5_Ans1_Method_User = 2;

	static const int kProxy_Sock5_Anthans_Ver = 1;
	static const int kProxy_Sock5_Anthans_Status = 0;

	static const int kProxy_Sock5_Ans2_Ver = 5;
	static const int kProxy_Sock5_Ans2_Rep = 0;

	static const int kProxy_IP_Length = 4;
	static const int kProxy_PORT_Length = 2;

	struct sock5req1
	{
		char Ver;
		char nMethods;
		char Methods[2];
		//char Methods[255];
	};

	struct sock5ans1
	{
		char Ver;
		char Method;
	};

	struct sock5req2
	{
		char Ver;
		char Cmd;
		char Rsv;
		char Atyp;
		//char other[1];
		char DSTADDR[4];
		char DSTPORT[2];
	};

	struct sock5ans2
	{
		char Ver;
		char Rep;
		char Rsv;
		char Atyp;
		char DSTADDR[4];
		char DSTPORT[2];
	};

	struct authans
	{
		char Ver;
		char Status;
	};

public:
	virtual bool sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst);

	virtual bool recvProxyPacket(const char *buff, size_t length);

};

class TC_ProxyHttp : public TC_ProxyInfo
{
public:
	TC_ProxyHttp(const TC_Endpoint & ep, const string & user, const string & pass)
		: TC_ProxyInfo(ep, user, pass)
	{
	}

	virtual bool sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst);

	virtual bool recvProxyPacket(const char *buff, size_t length);

};

}

#endif //TARS_CPP_PROXYINFO_H
