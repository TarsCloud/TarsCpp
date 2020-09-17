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


#include "servant/ProxyInfo.h"
#include "servant/RemoteLogger.h"
#include "util/tc_base64.h"

namespace tars
{

void ProxyBase::onDisconnect()
{
	setProxyStage(eProxy_Stage_DisConn);
}

void ProxyBase::onConnSuccess()
{
	setProxyStage(eProxy_Stage_Connected);
}

void ProxyBase::setProxyStage(ProxyBase::EMProxyStageType proxyStage)
{
	if (_stage == proxyStage) {
		return;
	}

	_stage = proxyStage;
}

////////////////////////////////////////////////////////////////////////
bool ProxySock4::sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst)
{
	//first handshake
	buff.push_back(kProxy_Sock4_Req1_VN);
	buff.push_back(kProxy_Sock4_Req1_CD);

	unsigned short nPort = htons(dst.getPort());

	buff.insert(buff.end(), (const char *)&nPort, (const char *)&nPort + sizeof(nPort));

	struct in_addr addr;

	TC_Socket::parseAddr(dst.getHost(), addr);

	int32_t tmpLong = addr.s_addr;

	buff.insert(buff.end(), (const char *)&tmpLong, (const char *)&tmpLong + sizeof(tmpLong));

	buff.push_back('a');
	buff.push_back(0);

	return true;
}

bool ProxySock4::recvProxyPacket(const char *buff, size_t length)
{
	switch (_stage) {
		case eProxy_Stage_Establish: {
			//send first handshake
			if (sizeof(struct sock4ans1) != length) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: Establish protocol length error]" << endl);
				onDisconnect();
				return false;
			}

			struct sock4ans1 *pSockAns1 = (struct sock4ans1 *) buff;
			if (pSockAns1->VN != kProxy_Sock4_Ans1_VN || pSockAns1->CD != kProxy_Sock4_Ans1_CD) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: Establish protocol version error: "<< (int)pSockAns1->VN << "," << (int)pSockAns1->CD << "]" << endl);
				onDisconnect();
				return false;
			}

			//success
			onConnSuccess();
			return true;
		}
		default: {
			assert(false);
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////

bool ProxySock5::sendProxyPacket(vector<char> & vBuffer, const TC_Endpoint & dst)
{
	switch (_stage) {
		case eProxy_Stage_DisConn:
		case eProxy_Stage_Establish: {
			//first handshake
			vBuffer.push_back(kProxy_Sock5_Req1_Ver);
			vBuffer.push_back(kProxy_Sock5_Req1_nMethods);
			vBuffer.push_back(kProxy_Sock5_Req1_nMethods0);
			vBuffer.push_back(kProxy_Sock5_Req1_nMethods1);

			return true;
		}
		case eProxy_Stage_ACK1: {
			//second handshake  user pwd
			char nUserLength = (char) _user.size();
			char nPwdLength = (char) _pass.size();

			vBuffer.push_back(1);
			vBuffer.push_back(nUserLength);
			vBuffer.insert(vBuffer.end(), _user.begin(), _user.end());
			vBuffer.push_back(nPwdLength);
			vBuffer.insert(vBuffer.end(), _pass.begin(), _pass.end());

			return true;
		}
		case eProxy_Stage_ACK2: {
			//third handshake
			vBuffer.push_back(kProxy_Sock5_Req3_Ver);
			vBuffer.push_back(kProxy_Sock5_Req3_Cmd);
			vBuffer.push_back(kProxy_Sock5_Req3_Rsv);

			if(dst.isIPv6())
			{
				vBuffer.push_back(kProxy_Sock5_Req3_AtypIpv6);
			}
			else
			{
				vBuffer.push_back(kProxy_Sock5_Req3_AtypIpv4);
			}

			struct in_addr addr;

			TC_Socket::parseAddr(dst.getHost(), addr);

			int32_t tmpLong = addr.s_addr;

			vBuffer.insert(vBuffer.end(), (const char *)&tmpLong, (const char *)&tmpLong + sizeof(tmpLong));

			unsigned short nPort = htons(dst.getPort());
			vBuffer.insert(vBuffer.end(), (const char *)&nPort, (const char *)&nPort + sizeof(nPort));

			return true;

		}
		default: {
			assert(false);
		}
	}
	return false;
}

bool ProxySock5::recvProxyPacket(const char *buff, size_t length)
{
	switch (_stage) {
		case eProxy_Stage_Establish: {
			//send first handshake
			if (sizeof(struct sock5ans1) != length) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: Establish protocol length error]" << endl);
				onDisconnect();
				return false;
			}

			struct sock5ans1 *pSock5Ans1 = (struct sock5ans1 *) buff;
			if (pSock5Ans1->Ver != kProxy_Sock5_Ans1_Ver || (pSock5Ans1->Method != kProxy_Sock5_Ans1_Method_Anonymous
				&& pSock5Ans1->Method != kProxy_Sock5_Ans1_Method_User)) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: Establish protocol version error]" << endl);
				onDisconnect();
				return false;
			}

			//need user
			if (pSock5Ans1->Method == kProxy_Sock5_Ans1_Method_User) {
				setProxyStage(eProxy_Stage_ACK1);
				return true;
			}
			else {
				//Anonymous
				setProxyStage(eProxy_Stage_ACK2);
				return true;
			}
		}
		case eProxy_Stage_ACK1: {
			//send second handshake
			if (sizeof(struct authans) != length) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: ACK1 protocol length error]" << endl);
				onDisconnect();
				return false;
			}
			struct authans *pSock5Anthans = (struct authans *) buff;
			if (pSock5Anthans->Ver != kProxy_Sock5_Anthans_Ver
				|| pSock5Anthans->Status != kProxy_Sock5_Anthans_Status) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: ACK1 protocol version error]" << endl);
				onDisconnect();
				return false;
			}

			setProxyStage(eProxy_Stage_ACK2);
			return true;
		}
		case eProxy_Stage_ACK2: {
			if (sizeof(struct sock5ans2) != length) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: ACK2 protocol length error]" << endl);
				onDisconnect();
				return false;
			}
			struct sock5ans2 *pSock5An2 = (struct sock5ans2 *) buff;
			if (pSock5An2->Ver != kProxy_Sock5_Ans2_Ver || pSock5An2->Rep != kProxy_Sock5_Ans2_Rep) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected: ACK2 protocol version error: " << (int)pSock5An2->Ver << "," << (int)pSock5An2->Rep << "]" << endl);
				onDisconnect();
				return false;
			}
			//success
			onConnSuccess();
			return true;
		}
		default: {
			assert(false);
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////

bool ProxyHttp::sendProxyPacket(vector<char> & buff, const TC_Endpoint & dst)
{
	switch (_stage) {
		case eProxy_Stage_Establish: {
			ostringstream oss;
			//first handshake
			std::string strRev;
			if (_user.empty()) {
				oss << "CONNECT " << dst.getHost() << ":" << dst.getPort()
				    << " HTTP/1.1\r\nUser-Agent: Mozilla/4.0\r\n\r\n";
				strRev = oss.str();
			}
			else {
				oss << "CONNECT " << dst.getHost() << ":" << dst.getPort()
				    << " HTTP/1.1\r\nUser-Agent: Mozilla/4.0\r\n";

				oss << "Proxy-Authorization:Basic " << TC_Base64::encode(_user + ":" + _pass) << "\r\n\r\n";
				strRev = oss.str();
			}

			buff.insert(buff.end(), strRev.begin(), strRev.end());
			return true;
		}
		default: {
			assert(false);
		}
	}
	return false;
}

bool ProxyHttp::recvProxyPacket(const char *buff, size_t length)
{
	switch (_stage) {
		case eProxy_Stage_Establish: {
			TC_HttpResponse rsp;
			rsp.decode(buff, length);

			//send first handshake
			if (rsp.getStatus() != 200) {
				TLOGERROR("[ProxyHttp::recvProxyPacket, proxy disconnected status:" << rsp.getStatus() << ", about:" << rsp.getAbout() << endl);
				onDisconnect();
				return false;
			}
			//success
			onConnSuccess();
			return true;
		}
		default: {
			assert(false);
		}
	}

	return false;
}

}
