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

#ifndef __TARS_TRANSCEIVER_H_
#define __TARS_TRANSCEIVER_H_

#include "servant/EndpointInfo.h"
#include "servant/NetworkUtil.h"
#include "servant/CommunicatorEpoll.h"
#include "servant/AuthLogic.h"
#include <list>
#include <util/tc_network_buffer.h>

using namespace std;

namespace tars
{

class TC_OpenSSL;

class AdapterProxy;
class ProxyBase;

/**
 * 网络传输基类，主要提供send/recv接口
 */
class Transceiver
{
public:
    /*
     * 连接状态
     */
    enum ConnectStatus
    {
        eUnconnected,
        eConnecting,
        eConnected,
    };

    /*
     * 发数据的返回状态
     */
    enum ReturnStatus
    {
        eRetError   = -1,   //发送错误
        eRetOk      = 0,    //发送成功
        eRetFull    = 1,    //数据发送一半, 队列满了, 等事件过来继续发送
        eRetNotSend = 2,    //连接还没有ok(ssl没有握手, 没有完成鉴权, 上一个数据包还没有发送完, 没有完成代理鉴权等), 数据没有发送
    };

    /**
     * 构造函数
     * @param ep
     * @param fd
     */
    Transceiver(AdapterProxy * pAdapterProxy,const EndpointInfo &ep);

    /**
     *
     *析构函数
     */
    virtual ~Transceiver();

    /**
     * 是否ssl
     */
    bool isSSL() const ;

    /*
     * 检查连接是否超时
     */
    void checkTimeout();

    /**
     * 重新创建连接
     */
    void reconnect();

    /**
     * 创建连接，初始化
     */
    void connect();

    /*
     * 关闭连接
     */
    virtual void close(bool destructor=false);

    /*
     * 设置当前连接态
     */
    void setConnected();

    /*
     * 往fd里面发送数据
     * 如果fd缓冲区已满,返回错误
     * 如果数据发送一半，缓冲区满了,返回成功
     */
	int sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> &pData);

	/**
	 * send buffer
	 * @return
	 */
	TC_NetWorkBuffer *getSendBuffer() { return &_sendBuffer; }

	/**
	 * recv buffer
	 * @return
	 */
	TC_NetWorkBuffer *getRecvBuffer() { return &_recvBuffer; }

    /*
     * 处理请求，判断Send BufferCache是否有完整的包
     * @return int
     */
    virtual int doRequest();

    /*
     * 处理返回，判断Recv BufferCache是否有完整的包
     * @param done
     * @return int
     */
    virtual int doResponse() = 0;

    /*
     * 网络发送接口
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int send(const void* buf, uint32_t len, uint32_t flag) = 0;

    /*
     * 网络接收接口
     * @param buf
     * @param len
     * @param flag
     *
     * @return int
     */
    virtual int recv(void* buf, uint32_t len, uint32_t flag) = 0;

    /*
     * 获取文件描述符
     * @return int
     */
    virtual int fd() const
    {
        return _fd;
    }

    /*
     * 是否有效
     */
    bool isValid() const
    {
        return (_fd != -1);
    }

    /*
     * 获取端口信息
     */
    const EndpointInfo& getEndpointInfo() const
    {
        return _ep;
    }

    /*
     * 获取connect所属的adapter
     */
    AdapterProxy * getAdapterProxy()
    {
        return _adapterProxy;
    }

    /*
     * 判断是否已经连接到服务端
     */
    bool hasConnected()
    { 
        return isValid() && (_connStatus == eConnected); 
    }

    /*
     * 判断是否正在连接
     */
    bool isConnecting()
    { 
        return isValid() && (_connStatus == eConnecting); 
    }

    /*
     * 设置连接失败
     */
    void setConnectFailed()
    { 
        _connStatus = eUnconnected; 
    }

    void finishInvoke(shared_ptr<ResponsePacket> &rsp);

    /**
     * 设置鉴权状态
     */
    void setAuthState(AUTH_STATE newstate) { _authState = newstate; }

    /*
     * 获取鉴权状态
     */
    int getAuthState() const { return _authState; }

    /*
     * 发送鉴权数据
     */
    bool sendAuthData(const BasicAuthInfo& );

protected:
	/*
	 * 设置当前连接态
	 */
	void onSetConnected();

    /** 
     ** 物理连接成功回调
     **/
    void                     onConnect();

	/**
	 ** 发送打通代理请求
	 **/
	void                     connectProxy();

	/**
	 * 检查是否代理创建成功
	 * @param buff
	 * @param length
	 * @return <0: 失败, 0: 成功: 1: 需要验证
	 */
	int                      doCheckProxy(const char *buff, size_t length);

    /** 
     ** 鉴权初始化请求
     **/
    void                     doAuthReq();

    /*
     * AdapterProxy
     */
    AdapterProxy *           _adapterProxy;

    /*
     * 连接的节点信息
     */
    EndpointInfo             _ep;

    /*
     * 套接字
     */
    int                      _fd;

    /*
     * 事件注册信息
     */
    FDInfo                   _fdInfo;
    
    /*
     * 连接状态
     */
    ConnectStatus            _connStatus;

    /*
     * 连接的超时时间
     */
    int64_t                  _conTimeoutTime;

    /* 
     * 鉴权状态 
     */
    AUTH_STATE              _authState;

protected:

    std::shared_ptr<TC_OpenSSL> _openssl;


    /*
     * 发送buffer
     */
	TC_NetWorkBuffer _sendBuffer;

	/*
     * 接收buffer
     */
    TC_NetWorkBuffer _recvBuffer;

	/**
	 * 代理
	 */
	ProxyBase*       _proxyPointer = NULL;
};

//////////////////////////////////////////////////////////
/**
 * TCP 传输实现
 */
class TcpTransceiver : public Transceiver
{
public:
    /**
     * 构造函数
     * @param ep
     * @param fd
     */
    TcpTransceiver(AdapterProxy * pAdapterProxy, const EndpointInfo &ep);

    /**
     * TCP 发送实现
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int send(const void* buf, uint32_t len, uint32_t flag);

    /**
     * TCP 接收实现
     * @param buf
     * @param len
     * @param flag
     *
     * @return int
     */
    virtual int recv(void* buf, uint32_t len, uint32_t flag);

    /**
     * 处理返回，判断Recv BufferCache是否有完整的包
     * @param done
     * @return int, =1,表示有数据就包
     */
	virtual int doResponse();
};
//////////////////////////////////////////////////////////
/**
 * UDP 传输实现
 */
class UdpTransceiver : public Transceiver
{
public:
    enum
    {
        DEFAULT_RECV_BUFFERSIZE = 64*1024       /*缺省数据接收buffer的大小*/
    };

    /**
     * 构造函数
     */
    UdpTransceiver(AdapterProxy * pAdapterProxy, const EndpointInfo &ep);

    /**
     * 析构函数
     */
    ~UdpTransceiver();

    /**
     * UDP 发送实现
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int send(const void* buf, uint32_t len, uint32_t flag);

    /**
     * UDP 接收实现
     * @param buf
     * @param len
     * @param flag
     * @return int
     */
    virtual int recv(void* buf, uint32_t len, uint32_t flag);

    /**
     * 处理返回，判断Recv BufferCache是否有完整的包
     * @param done
     * @return int
     */
	virtual int doResponse();

private:
    /*
     * 接收缓存
     */
    char*                       _pRecvBuffer;
};
//////////////////////////////////////////////////////////

}
#endif
