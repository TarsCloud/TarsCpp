//
// Created by jarod on 2020/4/7.
//

#ifndef TARS_CPP_PROXYINFO_H
#define TARS_CPP_PROXYINFO_H

#include "util/tc_clientsocket.h"

namespace tars
{

class ProxyBase
{
public:
	ProxyBase(const TC_Endpoint & ep, const string & user, const string & pass)
		: _stage(eProxy_Stage_Establish), _ep(ep), _user(user), _pass(pass)
	{
	}

	virtual ~ProxyBase() {}

	enum EMProxyStageType
	{
		eProxy_Stage_Establish,
		eProxy_Stage_ACK1,
		eProxy_Stage_ACK2,
		eProxy_Stage_Connected,
		eProxy_Stage_DisConn,
	};

	typedef std::function<void(EMProxyStageType)> ProxyStageChangedCallback;

	const TC_Endpoint & getEndpoint() const { return _ep; }

	virtual bool sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst) = 0;

	virtual bool recvProxyPacket(const char *buff, size_t length) = 0;

	bool isSuccess() { return _stage == eProxy_Stage_Connected; }

	void reset() { _stage = eProxy_Stage_Establish; }
protected:
	void onDisconnect();
	void onConnSuccess();
	void setProxyStage(EMProxyStageType proxyStage);

protected:
	EMProxyStageType _stage;
	TC_Endpoint _ep;
	std::string _user;
	std::string _pass;
};

class ProxySock4 : public ProxyBase
{
public:
	ProxySock4(const TC_Endpoint & ep)
		: ProxyBase(ep, "", "")
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

class ProxySock5 : public ProxyBase
{
public:
	ProxySock5(const TC_Endpoint & ep, const string & user, const string & pass)
		: ProxyBase(ep, user, pass)
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

class ProxyHttp : public ProxyBase
{
public:
	ProxyHttp(const TC_Endpoint & ep, const string & user, const string & pass)
		: ProxyBase(ep, user, pass)
	{
	}

	virtual bool sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst);

	virtual bool recvProxyPacket(const char *buff, size_t length);

};

}

#endif //TAF_CPP_PROXYINFO_H
