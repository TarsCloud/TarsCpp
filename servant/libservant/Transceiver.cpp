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

#include "servant/Transceiver.h"
#include "servant/AdapterProxy.h"
#include "servant/Application.h"
#include "servant/TarsLogger.h"
#include "servant/AuthLogic.h"

#if TARS_SSL
#include "util/tc_openssl.h"
#endif

namespace tars
{

static const int BUFFER_SIZE = 16 * 1024;

///////////////////////////////////////////////////////////////////////
Transceiver::Transceiver(AdapterProxy * pAdapterProxy,const EndpointInfo &ep)
: _adapterProxy(pAdapterProxy)
, _ep(ep)
, _fd(-1)
, _connStatus(eUnconnected)
, _conTimeoutTime(0)
, _authState(AUTH_INIT)
, _sendBuffer(this)
, _recvBuffer(this)
{
    _fdInfo.iType = FDInfo::ET_C_NET;
    _fdInfo.p     = (void *)this;
    _fdInfo.fd    = -1;
}

Transceiver::~Transceiver()
{
    close();
}

void Transceiver::checkTimeout()
{
    if(eConnecting == _connStatus && TNOWMS > _conTimeoutTime)
    {
        //链接超时
        TLOGERROR("[TARS][Transceiver::checkTimeout ep:"<<_adapterProxy->endpoint().desc()<<" , connect timeout]"<<endl);
        _adapterProxy->setConTimeout(true);
        close();
    }
}

bool Transceiver::isSSL() const 
{ 
    return _adapterProxy->endpoint().type() == EndpointInfo::SSL; 
}

void Transceiver::reconnect()
{
    close();

    connect();
}

void Transceiver::connect()
{
    if(isValid())
    {
        return;
    }

    if(_connStatus == eConnecting || _connStatus == eConnected)
    {
        return;
    }

    int fd = -1;

    //每次连接前都重新解析一下地址, 避免dns变了!
    _ep.parseAddress();

    if (_ep.type() == EndpointInfo::UDP)
    {
        fd = NetworkUtil::createSocket(true, false, _ep.isIPv6());
        _connStatus = eConnected;

        _adapterProxy->getObjProxy()->getCommunicatorEpoll()->addFd(fd, &_fdInfo, EPOLLIN | EPOLLOUT);
    }
    else
    {
        fd = NetworkUtil::createSocket(false, false, _ep.isIPv6());

        _adapterProxy->getObjProxy()->getCommunicatorEpoll()->addFd(fd, &_fdInfo, EPOLLIN | EPOLLOUT);

        socklen_t len = _ep.isIPv6() ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
        bool bConnected = NetworkUtil::doConnect(fd, _ep.addrPtr(), len);
        if(bConnected)
        {
            setConnected();
        }
        else
        {
            _connStatus     = Transceiver::eConnecting;
            _conTimeoutTime = TNOWMS + _adapterProxy->getConTimeout();
        }
    }

    _fd = fd;

    TLOGTARS("[TARS][Transceiver::connect obj:" << _adapterProxy->getObjProxy()->name()
        << ",connect:" << _ep.desc() << ",fd:" << _fd << "]" << endl);

    // //设置网络qos的dscp标志
    // if(0 != _ep.qos())
    // {
    //     int iQos=_ep.qos();
    //     ::setsockopt(fd,SOL_IP,IP_TOS,&iQos,sizeof(iQos));
    // }

    //设置套接口选项
    vector<SocketOpt> &socketOpts = _adapterProxy->getObjProxy()->getSocketOpt();
    for(size_t i=0; i<socketOpts.size(); ++i)
    {
        if(setsockopt(_fd,socketOpts[i].level,socketOpts[i].optname, (const char*)socketOpts[i].optval,socketOpts[i].optlen) == -1)
        {
            TLOGERROR("[TARS][setsockopt error:" << NetworkUtil::errorToString(errno) 
                << ",objname:" << _adapterProxy->getObjProxy()->name() 
                << ",desc:" << _ep.desc()
                << ",fd:" << _fd
                << ",level:" <<  socketOpts[i].level
                << ",optname:" << socketOpts[i].optname
                << ",optval:" << socketOpts[i].optval
                <<"    ]"<< endl);
        }
    }
}

void Transceiver::setConnected()
{
    _connStatus = eConnected;
    _adapterProxy->setConTimeout(false);
    _adapterProxy->addConnExc(false);

	TLOGTARS("[TARS][tcp setConnected, " << _adapterProxy->getObjProxy()->name() << ",fd:" << _fd << "]" << endl);

	onConnect();
}

void Transceiver::onConnect()
{
#if TARS_SSL
    if (isSSL())
    {
	    _openssl = _adapterProxy->getObjProxy()->getCommunicatorEpoll()->getCommunicator()->newClientSSL(_adapterProxy->getObjProxy()->getServantProxy()->tars_name());
        if (!_openssl)
        {
            ObjectProxy* obj = _adapterProxy->getObjProxy();
            TLOGERROR("[TARS][onConnect:" << obj->name() << " can't find client SSL_CTX " << endl);
            this->close();
            return;
        }

	    _openssl->init(false);

	    _openssl->setReadBufferSize(1024 * 8);
	    _openssl->setWriteBufferSize(1024 * 8);

        int ret = _openssl->doHandshake(_sendBuffer);
        if (ret != 0)
        {
            TLOGERROR("[TARS] SSL_connect failed " << endl);
            this->close();
            return;
        }

        // send the encrypt data from write buffer
        if (!_sendBuffer.empty())
        {
	        TLOGTARS("[TARS][Transceiver::onConnect  handshake:" << _openssl->isHandshaked() << ", send handshake len:" << _sendBuffer.getBufferLength() << endl);

	        doRequest();
        }
        return;
    }
#endif

	doAuthReq();
}

void Transceiver::doAuthReq()
{
    ObjectProxy* obj = _adapterProxy->getObjProxy();

    TLOGTARS("[TARS][Transceiver::doAuthReq obj:" << obj->name() << ", auth type:" << etos(_adapterProxy->endpoint().authType()) << endl);

    if (_adapterProxy->endpoint().authType() == AUTH_TYPENONE)
    {
        _authState = AUTH_SUCC;
        _adapterProxy->doInvoke();
    }
    else
    {
        BasicAuthInfo basic;
        basic.sObjName      = obj->name();
        basic.sAccessKey    = obj->getCommunicatorEpoll()->getCommunicator()->getServantProperty(obj->name(), "accesskey");
        basic.sSecretKey    = obj->getCommunicatorEpoll()->getCommunicator()->getServantProperty(obj->name(), "secretkey");

        this->sendAuthData(basic);
    }
}

void Transceiver::finishInvoke(shared_ptr<ResponsePacket> &rsp)
{
	if (_authState != AUTH_SUCC)
	{
		std::string ret(rsp->sBuffer.begin(), rsp->sBuffer.end());
		tars::AUTH_STATE tmp = AUTH_SUCC;
		tars::stoe(ret, tmp);
		tars::AUTH_STATE newstate = tmp;

		TLOGTARS("[TARS]Transceiver::finishInvoke state: " << etos(_authState) << " -> " << etos(newstate) << endl);
		setAuthState(newstate);

		if (newstate == AUTH_SUCC)
		{
			// flush old buffered msg when auth is not complete
			_adapterProxy->doInvoke();
		}
		else
		{
			TLOGERROR("[TARS]Transceiver::finishInvoke newstate: " << etos(newstate) << ", error close!\n");
			close();
		}

		return;
	}
	_adapterProxy->finishInvoke(rsp);
}

bool Transceiver::sendAuthData(const BasicAuthInfo& info)
{
    assert (_authState != AUTH_SUCC);

    ObjectProxy* objPrx = _adapterProxy->getObjProxy();

    // 走框架的AK/SK认证
    std::string out = tars::defaultCreateAuthReq(info);

    const int kAuthType = 0x40;
    RequestPacket request;
    request.sFuncName       = "tarsInnerAuthServer";
    request.sServantName    = "authServant";
    request.iVersion        = TARSVERSION;
    request.iRequestId      = 1;
    request.cPacketType     = TARSNORMAL;
    request.iMessageType    = kAuthType;
    request.sBuffer.assign(out.begin(), out.end());

#if TARS_SSL
    if(this->isSSL()) {
	    vector<char> buff = objPrx->getProxyProtocol().requestFunc(request, this);

	    int ret = _openssl->write(buff.data(), (uint32_t) buff.size(), _sendBuffer);
	    if(ret != 0)
	    {
		    TLOGERROR("[TARS][Transceiver::sendAuthData ssl write failed, obj:" << _adapterProxy->getObjProxy()->name() << ", error:" << _openssl->getErrMsg() << endl);
		    return false;
	    }
    }
    else {
	    _sendBuffer.addBuffer(objPrx->getProxyProtocol().requestFunc(request, this));
    }

#else
	_sendBuffer.addBuffer(objPrx->getProxyProtocol().requestFunc(request, this));

#endif

	TLOGTARS("[TARS][sendAuthData:" << objPrx->name() << " len: " << _sendBuffer.getBufferLength() << endl);

	int ret = doRequest();
	if (ret != 0)
    {
        TLOGERROR("[TARS][Transceiver::setConnected failed sendRequest for Auth\n");
        close();
        return false;
    }

    return true;
}

void Transceiver::close()
{
    if(!isValid()) return;

#if TARS_SSL
    if (_openssl)
    {
        _openssl->release();
        _openssl.reset();
    }
#endif

    _adapterProxy->getObjProxy()->getCommunicatorEpoll()->delFd(_fd,&_fdInfo,EPOLLIN|EPOLLOUT);

    NetworkUtil::closeSocketNoThrow(_fd);

    _connStatus = eUnconnected;

    _fd = -1;

	_sendBuffer.clearBuffers();

	_recvBuffer.clearBuffers();

    _authState = AUTH_INIT;

    TLOGTARS("[TARS][trans close:"<< _adapterProxy->getObjProxy()->name()<< "," << _ep.desc() << "]" << endl);
}

int Transceiver::doRequest()
{
    if(!isValid()) return -1;

	//buf不为空,先发送buffer的内容
    while(!_sendBuffer.empty())
    {
    	auto data = _sendBuffer.getBufferPointer();
    	assert(data.first != NULL && data.second != 0);

        int iRet = this->send(data.first, (uint32_t) data.second, 0);

        if (iRet < 0)
        {
            return -1;
        }

	    _sendBuffer.moveHeader(iRet);
    }

	//取adapter里面积攒的数据
    if(_sendBuffer.empty()) {
        _adapterProxy->doInvoke();
    }

	//object里面应该是空的
    // assert(_adapterProxy->getObjProxy()->timeQEmpty());

    return 0;
}

int Transceiver::sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> &buff)
{
    //空数据 直接返回成功
    if(buff->empty()) {
	    return eRetOk;
    }

    if(eConnected != _connStatus)
    {
        return eRetError;
    }

    if (_authState != AUTH_SUCC)
    {
#if TARS_SSL
        if (isSSL() && !_openssl)
            return eRetError;
#endif
        TLOGTARS("[TARS][Transceiver::sendRequest failed, obj:" << _adapterProxy->getObjProxy()->name() << ", need auth." << endl);
        return eRetError; // 需要鉴权但还没通过，不能发送非认证消息
    }

    //buf不为空, 表示之前的数据还没发送完, 直接返回失败
    //等buffer可写了,epoll会通知写事件
    if(!_sendBuffer.empty()) {
	    return eRetError;
    }

#if TARS_SSL
	// 握手数据已加密,直接发送，会话数据需加密
	if (isSSL())
	{
		if(!_openssl->isHandshaked()) {
			TLOGTARS("[TARS][Transceiver::sendRequest failed, obj:" << _adapterProxy->getObjProxy()->name() << ", ssl need handshake." << endl);
			return eRetError;
		}

		int ret = _openssl->write(buff->buffer(), (uint32_t) buff->length(), _sendBuffer);
		if(ret != 0)
		{
			TLOGERROR("[TARS][Transceiver::sendRequest ssl write failed, obj:" << _adapterProxy->getObjProxy()->name() << ", error:" << _openssl->getErrMsg() << endl);
			return eRetError;
		}

		size_t length = _sendBuffer.getBufferLength();

		do
		{
			auto data = _sendBuffer.getBufferPointer();

			int iRet = this->send(data.first, (uint32_t) data.second, 0);

			//失败, 且任何数据都没有发送出去, socket写满了!
			if (iRet < 0 && _sendBuffer.getBufferLength() == length) {
				//clear data, data will send again!
				_sendBuffer.clearBuffers();
				return eRetError;
			}
			else if(iRet < 0)
			{
				//之前发送了部分数据, socket写满了
				return eRetFull;
			}

			_sendBuffer.moveHeader(iRet);

		}
		while (!_sendBuffer.empty());

		return eRetOk;
	}
#endif

//	TLOGTARS("[TARS][Transceiver::sendRequest handshake:" << _openssl->isHandshaked() << ", origin length:" << buff->length() << endl);

	int iRet = this->send(buff->buffer(), (uint32_t)buff->length(), 0);

    //失败，直接返回
    if(iRet<0) {
	    return eRetError;
    }

	static std::atomic<int> totalSend{0};
	totalSend += iRet;

//	cout << "totalSend:" << totalSend << endl;

    //没有全部发送完,写buffer 返回成功
    if(iRet < (int)buff->length())
    {
    	buff->add(iRet);
	    _sendBuffer.addBuffer(buff);
        return eRetFull;
    }

    return eRetOk;

// #if TARS_SSL
//     // 握手数据已加密,直接发送，会话数据需加密
//     std::string out;
//     if (isSSL() && _openssl->isHandshaked())
//     {
//         out = _openssl->Write(pData, iSize);
//         pData = out.data();
//         iSize = out.size();
//     }
// #endif

    // int iRet = this->send(pData,iSize,0);

    // //失败，直接返回
    // if(iRet < 0)
    // {
    //     return eRetError;
    // }

    // //没有全部发送完,写buffer 返回成功
    // if(iRet < (int)iSize)
    // {
    //     _sendBuffer.PushData(pData+iRet,iSize-iRet);
    //     return eRetFull;
    // }

    // return eRetOk;
}

//////////////////////////////////////////////////////////
TcpTransceiver::TcpTransceiver(AdapterProxy * pAdapterProxy, const EndpointInfo &ep)
: Transceiver(pAdapterProxy, ep)
{
}


int TcpTransceiver::doResponse()
{
    if(!isValid()) return -1;

	int iRet = 0;

    int recvCount = 0;

	static std::atomic<int> totalRecv{0};

	do
    {
	    char buff[BUFFER_SIZE] = {0x00};

	    if ((iRet = this->recv(buff, BUFFER_SIZE, 0)) > 0)
	    {
		    totalRecv += iRet;

//		    cout << "totalRecv:" << totalRecv << endl;

		    TC_NetWorkBuffer *rbuf = &_recvBuffer;
#if TARS_SSL
		    if (isSSL())
		    {
			    const bool preHandshake = _openssl->isHandshaked();
			    int ret = _openssl->read(buff, iRet, _sendBuffer);
		        if (ret != 0)
			    {
				    TLOGERROR("[TARS][Transceiver::doResponse SSL_read handshake failed: " << _adapterProxy->getObjProxy()->name() << ", info:" << _openssl->getErrMsg() << endl);
				    close();
				    return -1;
			    }
			    else if(!_sendBuffer.empty())
			    {
				    TLOGTARS("[TARS][Transceiver::doResponse SSL_read prehandshake:" << preHandshake << ", handshake:" << _openssl->isHandshaked() << ", send handshake:" << _sendBuffer.getBufferLength() << endl);

				    doRequest();
			    }

			    if (!_openssl->isHandshaked())
				    return 0;

			    if (!preHandshake) {
				    doAuthReq();
			    }

			    rbuf = _openssl->recvBuffer();
		    }
		    else
		    {
			    rbuf->addBuffer(buff, iRet);
		    }
#else
		    rbuf->addBuffer(buff, iRet);
#endif
		    ++recvCount;

		    try
		    {
			    TC_NetWorkBuffer::PACKET_TYPE ret;

			    do
		        {
			        shared_ptr<ResponsePacket> rsp = std::make_shared<ResponsePacket>();

			        ret = _adapterProxy->getObjProxy()->getProxyProtocol().responseFunc(*rbuf, *rsp.get());

				    if (ret == TC_NetWorkBuffer::PACKET_ERR) {
					    TLOGERROR( "[TARS][tcp doResponse," << _adapterProxy->getObjProxy()->name() << ",fd:" << _fd << "," << _ep.desc() << ",tcp recv decode error" << endl);
					    close();
					    break;
				    }
				    else if (ret == TC_NetWorkBuffer::PACKET_FULL) {
	                    finishInvoke(rsp);
				    }
				    else {
					    break;
				    }
			    }while(true);

			    //接收的数据小于buffer大小, 内核会再次通知你
			    if(iRet < BUFFER_SIZE)
			    {
				    break;
			    }

			    //收包太多了, 中断一下, 释放线程给send等
			    if (recvCount >= 100) {
				    _adapterProxy->getObjProxy()->getCommunicatorEpoll()->modFd(_fd, &_fdInfo, EPOLLIN | EPOLLOUT);
				    break;
			    }
		    }
		    catch (exception & ex) {
			    TLOGERROR("[TARS][tcp doResponse," << _adapterProxy->getObjProxy()->name() << ",fd:" << _fd << ","
			                                      << _ep.desc() << ",tcp recv decode error:" << ex.what() << endl);

			    close();
		    }
		    catch (...) {
			    TLOGERROR("[TARS][tcp doResponse," << _adapterProxy->getObjProxy()->name() << ",fd:" << _fd << ","
			                                      << _ep.desc() << ",tcp recv decode error." << endl);

			    close();
		    }
	    }
    }
    while (iRet>0);

    TLOGTARS("[TARS][tcp doResponse, " << _adapterProxy->getObjProxy()->name() << ",fd:" << _fd << ", all recvbuf:" << _recvBuffer.getBufferLength() << "]" << endl);

	return 0;
}

// int TcpTransceiver::doResponse(list<ResponsePacket>& done)
// {
//     if(!isValid())
//     {
//         return -1;
//     }

//     int iRet = 0;

//     done.clear();

//     do
//     {
//         _recvBuffer.AssureSpace(8 * 1024);
//         char stackBuffer[64 * 1024];

//         struct iovec vecs[2];
//         vecs[0].iov_base = _recvBuffer.WriteAddr();
//         vecs[0].iov_len = _recvBuffer.WritableSize();
//         vecs[1].iov_base = stackBuffer;
//         vecs[1].iov_len = sizeof stackBuffer;

//         if ((iRet = this->readv(vecs, 2)) > 0)
//         {
//             if (static_cast<size_t>(iRet) <= vecs[0].iov_len)
//             {
//                 _recvBuffer.Produce(iRet);
//             }
//             else
//             {
//                 _recvBuffer.Produce(vecs[0].iov_len);
//                 size_t stackBytes = static_cast<size_t>(iRet) - vecs[0].iov_len;
//                 _recvBuffer.PushData(stackBuffer, stackBytes);
//             }
//         }
//     }
//     while (iRet>0);

//     TLOGTARS("[TARS][tcp doResponse objname:" << _adapterProxy->getObjProxy()->name() 
//         << ",fd:" << _fd << ",recvbuf:" << _recvBuffer.ReadableSize() << "]" << endl);

//     if(!_recvBuffer.IsEmpty())
//     {
//         try
//         {
//             const char* data = _recvBuffer.ReadAddr();
//             size_t len = _recvBuffer.ReadableSize();
// #if TARS_SSL
//             if (isSSL())
//             {
//                 const bool preNotHandshake = !_openssl->isHandshaked();
//                 std::string out;
//                 if (!_openssl->Read(_recvBuffer.ReadAddr(), _recvBuffer.ReadableSize(), out))
//                 {
//                     TLOGERROR("[TARS][SSL_connect Failed: " << _adapterProxy->getObjProxy()->name() << endl);
//                     this->close();
//                     return -1;
//                 }
//                 else
//                 {
//                     sendRequest(out.data(), out.size(), true);
//                 }

//                 _recvBuffer.Clear();

//                 if (!_openssl->isHandshaked())
//                     return 0;

//                 if (preNotHandshake) 
//                     doAuthReq();
                
//                 std::string* plainBuf = _openssl->recvBuffer();
//                 data = plainBuf->data();
//                 len  = plainBuf->size();
//             }
// #endif
//             size_t pos = 0;
//             ProxyProtocol& proto = _adapterProxy->getObjProxy()->getProxyProtocol();

//             if (proto.responseExFunc) 
//             {
//                 long id = _adapterProxy->getId();
//                 pos = proto.responseExFunc(data, len, done, (void*)id);
//             }
//             else
//             {
//                 pos = proto.responseFunc(data, len, done);
//             }

//             if(pos > 0)
//             {
// #if TARS_SSL
//                 if (isSSL())
//                 {
//                     std::string* plainBuf = _openssl->recvBuffer();
//                     plainBuf->erase(0, pos);
//                 }
//                 else
// #endif
//                 {
//                     _recvBuffer.Consume(pos);
//                     if (_recvBuffer.Capacity() > 8 * 1024 * 1024)
//                         _recvBuffer.Shrink();
//                 }
//             }
//         }
//         catch (exception &ex)
//         {
//             TLOGERROR("[TARS][tcp doResponse objname:" << _adapterProxy->getObjProxy()->name() 
//                 << ",fd:" << _fd << ",desc:" << _ep.desc() << ",tcp recv decode error:" << ex.what() << endl);

//             close();
//         }
//         catch (...)
//         {
//             TLOGERROR("[TARS][tcp doResponse objname:" << _adapterProxy->getObjProxy()->name() 
//                 << ",fd:" << _fd << ",desc" << _ep.desc() << ",tcp recv decode error." << endl);

//             close();
//         }
//     }

//     return done.empty()?0:1;
// }

int TcpTransceiver::send(const void* buf, uint32_t len, uint32_t flag)
{
    //只有是连接状态才能收发数据
    if(eConnected != _connStatus)
        return -1;


	int iRet = ::send(_fd, (const char*)buf, len, flag);

	if (iRet < 0 && !TC_Socket::isPending())
    {
        TLOGTARS("[TARS][tcp send," << _adapterProxy->getObjProxy()->name() << ", fd:" << _fd << "," << _ep.desc()
            << ", fail! errno:" << TC_Exception::getSystemCode() << ", "
            << TC_Exception::parseError(TC_Exception::getSystemCode()) << ", close]" << endl);

        close();

        return iRet;
    }

#if TARGET_PLATFORM_WINDOWS
    if(iRet < 0 && TC_Socket::isPending())
    {
        _adapterProxy->getObjProxy()->getCommunicatorEpoll()->modFd(_fd, &_fdInfo, EPOLLIN | EPOLLOUT);        
    }
#endif    
    TLOGTARS("[TARS][tcp send," << _adapterProxy->getObjProxy()->name() << ", fd:" << _fd << ","
        << _ep.desc() << ", len:" << iRet << "]" << endl);

    return iRet;
}

// int TcpTransceiver::readv(const struct iovec* vecs, int32_t vcnt)
// {
//     //只有是连接状态才能收发数据
//     if(eConnected != _connStatus)
//         return -1;

//     int iRet = ::readv(_fd, vecs, vcnt);

//     if (iRet == 0 || (iRet < 0 && errno != EAGAIN))
//     {
//         TLOGTARS("[TARS][tcp readv, " << _adapterProxy->getObjProxy()->name()
//                 << ",fd:" << _fd << ", " << _ep.desc() <<",ret " << iRet
//                 << ", fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl);

//         close();

//         return 0;
//     }

//     TLOGTARS("[TARS][tcp readv," << _adapterProxy->getObjProxy()->name()
//             << ",fd:" << _fd << "," << _ep.desc() << ",ret:" << iRet << "]" << endl);

//     return iRet;
// }

// int TcpTransceiver::recv(void* buf, uint32_t len, uint32_t flag)
// {
//     //只有是连接状态才能收发数据
//     if(_connStatus != eConnected)
//     {
//         return -1;
//     }

//     int iRet = ::recv(_fd, buf, len, flag);

//     if (iRet == 0 || (iRet < 0 && errno != EAGAIN))
//     {
//         TLOGTARS("[TARS][tcp recv objname:" << _adapterProxy->getObjProxy()->name()
//             << ",fd:" << _fd << ", " << _ep.desc() <<",ret " << iRet
//             << ", fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl);

//         close();

//         return 0;
//     }

//     TLOGTARS("[TARS][tcp recv objname:" << _adapterProxy->getObjProxy()->name()
//         << ",fd:" << _fd << ",desc:" << _ep.desc() << ",ret:" << iRet << "]" << endl);

//     return iRet;
// }


int TcpTransceiver::recv(void* buf, uint32_t len, uint32_t flag)
{
    //只有是连接状态才能收发数据
    if(eConnected != _connStatus)
        return -1;

    int iRet = ::recv(_fd, (char*)buf, len, flag);

	if (iRet == 0 || (iRet < 0 && !TC_Socket::isPending()))
    {
        TLOGTARS("[TARS][tcp recv, " << _adapterProxy->getObjProxy()->name()
                << ",fd:" << _fd << ", " << _ep.desc() <<", ret:" << iRet
                << ", fail! errno:" << TC_Exception::getSystemCode() << "," << TC_Exception::parseError(TC_Exception::getSystemCode()) << ",close]" << endl);

        close();

        return 0;
    }

#if TARGET_PLATFORM_WINDOWS
    if(iRet < 0 && TC_Socket::isPending())
    {
        _adapterProxy->getObjProxy()->getCommunicatorEpoll()->modFd(_fd, &_fdInfo, EPOLLIN | EPOLLOUT);        
    }
#endif   
    TLOGTARS("[TARS][tcp recv," << _adapterProxy->getObjProxy()->name()
            << ",fd:" << _fd << "," << _ep.desc() << ", len:" << iRet << "]" << endl);

    return iRet;
}

/////////////////////////////////////////////////////////////////
UdpTransceiver::UdpTransceiver(AdapterProxy * pAdapterProxy, const EndpointInfo &ep)
: Transceiver(pAdapterProxy, ep)
, _pRecvBuffer(NULL)
{
    // UDP不支持鉴权
    _authState = AUTH_SUCC;

    if(!_pRecvBuffer)
    {
        _pRecvBuffer = new char[DEFAULT_RECV_BUFFERSIZE];
        if(!_pRecvBuffer)
        {
            throw TC_Exception("objproxy '" + _adapterProxy->getObjProxy()->name() + "' malloc udp receive buffer fail");
        }
    }

}

UdpTransceiver::~UdpTransceiver()
{
    if(_pRecvBuffer)
    {
        delete _pRecvBuffer;
        _pRecvBuffer = NULL;
    }
}


int UdpTransceiver::doResponse()
{
    if(!isValid()) return -1;

    int recv = 0;

//    done.clear();
    do
    {
        if ((recv = this->recv(_pRecvBuffer, DEFAULT_RECV_BUFFERSIZE, 0)) > 0)
        {
            TLOGTARS("[TARS][udp doResponse, " << _adapterProxy->getObjProxy()->name()
                    << ",fd:" << _fd << ",recvbuf:" << recv << "]" << endl);

	        _recvBuffer.clearBuffers();
            _recvBuffer.addBuffer(_pRecvBuffer, recv);

            try
            {
	            shared_ptr<ResponsePacket> rsp = std::make_shared<ResponsePacket>();

                TC_NetWorkBuffer::PACKET_TYPE ret;

                ret = _adapterProxy->getObjProxy()->getProxyProtocol().responseFunc(_recvBuffer, *rsp.get());

                if(ret == TC_NetWorkBuffer::PACKET_ERR || ret == TC_NetWorkBuffer::PACKET_LESS)
                {
                    TLOGERROR("[TARS][udp doResponse," << _adapterProxy->getObjProxy()->name() << ",fd:" << _fd << "," << _ep.desc() << ",tcp recv decode error, ret:" << ret << endl);
                    break;
                }
                else
                {
	                finishInvoke(rsp);
                }
            }
            catch (exception &ex)
            {
                TLOGERROR("[TARS][udp doResponse, " << _adapterProxy->getObjProxy()->name()
                        << ",fd:" << _fd << "," << _ep.desc()
                        << ", udp recv decode error:" << ex.what() << endl);
            }
            catch (...)
            {
                TLOGERROR("[TARS][udp doResponse, " << _adapterProxy->getObjProxy()->name()
                        << ",fd:" << _fd << "," << _ep.desc()
                        << ", udp recv decode error." << endl);
            }
        }
    }
    while (recv > 0);

    return 0;
}

int UdpTransceiver::send(const void* buf, uint32_t len, uint32_t flag)
{
    if(!isValid()) return -1;

    int iRet=::sendto(_fd, (const char*)buf, len, flag, (struct sockaddr*) &(_ep.addr()), sizeof(sockaddr));

    if (iRet<0)
    {
        if(!TC_Socket::isPending())
        {
            TLOGERROR("[TARS][udp send " << _adapterProxy->getObjProxy()->name()
                    << ",fd:" << _fd << "," << _ep.desc() << ", fail! errno:"
                    << TC_Exception::getSystemCode() << "," 
                    << TC_Exception::parseError(TC_Exception::getSystemCode()) << ",close]" << endl);

			close();

            return iRet;
        }
        iRet=0;
    }
    else if(iRet>0 && iRet != (int)len)
    {
        TLOGERROR("[TARS][udp send, " << _adapterProxy->getObjProxy()->name()
                << ",fd:" << _fd << "," << _ep.desc() << ", send error."
                << ", len:" << len << ", sendLen:" << iRet << endl);
        //udp只发一次 发送一半也算全部发送成功
        iRet = len;
    }
    return iRet;
}

int UdpTransceiver::recv(void* buf, uint32_t len, uint32_t flag)
{
    if(!isValid()) return -1;

    int iRet = ::recvfrom(_fd, (char*)buf, len, flag, NULL, NULL); //need check from_ip & port

    if (iRet < 0  && !TC_Socket::isPending())
    {
        TLOGERROR("[TARS][udp recv " << _adapterProxy->getObjProxy()->name() << ",fd:" << _fd << ","
            << _ep.desc() << ", fail! errno:" << TC_Exception::getSystemCode() << "," 
            << TC_Exception::parseError(TC_Exception::getSystemCode()) << ",close]" << endl);

		close();

		return 0;
    }
    return iRet;
}

// int UdpTransceiver::doResponse(list<ResponsePacket>& done)
// {
//     if(!isValid())
//     {
//         return -1;
//     }

//     int recv = 0;

//     done.clear();
//     do
//     {
//         if ((recv = this->recv(_recvBuffer, DEFAULT_RECV_BUFFERSIZE, 0)) > 0)
//         {
//             TLOGTARS("[TARS][udp doResponse objname:" << _adapterProxy->getObjProxy()->name()
//                 << ",fd:" << _fd << ",recvbuf:" << recv << "]" << endl);

//             try
//             {
//                 _adapterProxy->getObjProxy()->getProxyProtocol().responseFunc(_recvBuffer, recv, done);
//             }
//             catch (exception &ex)
//             {
//                 TLOGERROR("[TARS][udp doResponse, " << _adapterProxy->getObjProxy()->name()
//                     << ",fd:" << _fd << ",desc:" << _ep.desc()
//                     << ", udp recv decode error:" << ex.what() << endl);
//             }
//             catch (...)
//             {
//                 TLOGERROR("[TARS][udp doResponse, " << _adapterProxy->getObjProxy()->name()
//                     << ",fd:" << _fd << ",desc:" << _ep.desc()
//                     << ", udp recv decode error." << endl);
//             }
//         }
//     }
//     while (recv > 0);

//     return done.empty()?0:1;
// }

// int UdpTransceiver::send(const void* buf, uint32_t len, uint32_t flag)
// {
//     if(!isValid())
//     {
//         return -1;
//     }

//     int iRet = ::sendto(_fd, buf, len, flag, (struct sockaddr*) &(_ep.addr()), sizeof(sockaddr));

//     if (iRet<0)
//     {
//         if(errno != EAGAIN)
//         {
//             TLOGERROR("[TARS][udp send objname:" << _adapterProxy->getObjProxy()->name()
//                 << ",fd:" << _fd << ",desc:" << _ep.desc() 
//                 << ", fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl);

//             close();

//             return iRet;
//         }

//         iRet = 0;
//     }
//     else if(iRet > 0 && iRet != (int)len)
//     {
//         TLOGERROR("[TARS][udp send objname:" << _adapterProxy->getObjProxy()->name()
//                 << ",fd:" << _fd << "," << _ep.desc() << ", send error."
//                 << ", len:" << len << ", sendLen:" << iRet << endl);

//         //udp只发一次 发送一半也算全部发送成功
//         iRet = len;
//     }

//     return iRet;
// }

// int UdpTransceiver::recv(void* buf, uint32_t len, uint32_t flag)
// {
//     if(!isValid())
//     {
//         return -1;
//     }

//     int iRet = ::recvfrom(_fd, buf, len, flag, NULL, NULL); //need check from_ip & port

//     if (iRet < 0  && errno != EAGAIN)
//     {
//         TLOGERROR("[TARS][udp recv objname:" << _adapterProxy->getObjProxy()->name() 
//             << ",fd:" << _fd << ",desc:" << _ep.desc() 
//             << ", fail! errno:" << errno << "," << strerror(errno) << ",close]" << endl);

//         close();

//         return 0;
//     }

//     return iRet;
// }

/////////////////////////////////////////////////////////////////
}
