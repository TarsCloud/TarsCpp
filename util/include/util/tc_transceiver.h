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

#ifndef TC_CPP_TRANSCEIVER_H
#define TC_CPP_TRANSCEIVER_H

#include <list>
#include "util/tc_platform.h"
#include "util/tc_network_buffer.h"
#include "util/tc_clientsocket.h"
#include "util/tc_epoller.h"
#include "util/tc_proxy_info.h"

namespace tars
{

class TC_OpenSSL;

/**
* @brief 连接异常类
*/
struct TC_Transceiver_Exception : public TC_Exception
{
    TC_Transceiver_Exception(const string &sBuffer) : TC_Exception(sBuffer){};
    TC_Transceiver_Exception(const string &sBuffer, int err) : TC_Exception(sBuffer, err){};
    ~TC_Transceiver_Exception() throw(){};    
};


/**
 * 网络传输基类
 * 使用方式说明:
 * 1 通过暴露注册回调的机制来和使用方交互, 配和epoller类完成网路层的收发都统一, epoller对象需要使用者自己创建和管理, 并传递给TC_Transceiver
 * 2 即可支持客户端的网络, 也可以支持服务端的网络, 但是两者初始化过程是有区别的(initializeClient & initializeServer)
 *   客户端网络: 发送connect时, 会创建句柄, 有oncreate_callback调用到业务层, 通过参数将TC_Epoller::EpollInfo*给业务层, 业务层在回调中根据实际情况完成epoll事件函数的注册
 *   服务端网络: 调用者完成accept, 然后通过bindFd的形式, 将句柄传递进来, bindFd会创建TC_Epoller::EpollInfo*, 返回给调用方
 * 3 无论是客户端网络还是服务端网络, 使用者都不用管理TC_Epoller::EpollInfo*的生命周期, 它会被TC_Transceiver管理, 连接关闭时释放
 * 4 作为客户端, 支持sock4/sock5/http代理(通过oncreate_callback回调, 业务层将代理信息TC_ProxyInfo返回给TC_Transceiver), 也可以通过setBindAddr指定客户端发包时的绑定地址
 * 5 在数据包的管理上, TC_Transceiver内只保存最后一个发送的数据包(可能发送了一半), 因此调用者需要维持发送的数据包队列, 几个回调函数的说明:
 *   oncreate_callback: 只存在于客户端模式, 当TC_Transceiver创建完客户端fd时回调oncreate_callback, 使用者在回调事件中监听fd事件, 如果有代理, 则返回代理信息, 否则返回null(注意, 如果有, 则没法都要new一个新的, 因为代理是具备状态信息的)
 *   onconnect_callback: 建立连接以后的回调, 注意对于ssl, 是指已经完成ssl握手后的回调
 *   onsend_callback: 当TC_Transceiver内的数据都发送完毕, 则回调onsend_callback, 使得调用者可以发送下一个数据
 *   onrequest_callback: 该回调表示网络层的连接建立完毕(如果SSL则表示SSL握手完成), 可以发送业务层队列的数据了(通过sendRequest来发送)
 *   onopenssl_callback: 需要常见openssl的对象回调, 业务层完成openssl对象的创建(因为涉及证书管理, 因此丢该业务层来创建)
 *   onparser_callback: 协议解析的回调 
 *   onclose_callback: 关闭socket的回调(在close句柄之前回调的)
 *   这种连接中只有最后一个数据包的模式, 保证当连接异常时,在调用者队列中还没有发送的数据能够通过其他连接来发送
 * 6 发送数据使用sendRequest函数, 注意返回值
 *   eRetError: 表示发送失败, 当前的数据包没有发送出去, 需要检查连接有有效性, 如果有效: isValid, 则数据需要进入使用自己的队列, onsend_callback回调的时候再发送
 *   eRetOk: 发送成功
 *   eRetFull: 发送了一半, 系统网络buff满了, 即当前fd不能再发送数据了, 等EPOLLOUT事件来的时候, 业务层再发送
 *   eRetNotSend: 数据没有发送(比如连接还没有建立, ssl没有握手, 没有鉴权等), 业务层可以缓存此数据, 等可以发送时onrequest_callback会被回调
 * 7 注册事件, 客户端在oncreate_callback回调中注册相关事件, 服务端在accept句柄并bindFd之后, 来注册事件, 通常需要注册三个事件
 *
 * 8 注意: 主要接口都以异常的形式对外抛出错误, 因此外部调用时注意捕获异常(一般是在注册的事件中)
 * 9 连接直接鉴权逻辑, 即可客户端发业务包前, 会发送一个鉴权包到服务器端, 服务器收到同样解包, 鉴权通过后, 才能继续发送业务包
 * 10 具体客户端使用方式可以参考: CommunicatorEpoll类, 服务端参考: tc_epoll_server
 * 
 */
class TC_Transceiver
{
public:
    /**
     * 连接状态
     */
    enum ConnectStatus
    {
        eUnconnected,
        eConnecting,
        eConnected,
    };

    /**
     * sendRequest返回值
     */ 
    enum ReturnStatus
    {
        eRetError = -1,
        eRetOk =0,
        eRetFull=1,
	    eRetNotSend = 2,
    };

    /**
     * 连接关闭原因
     */
    enum CloseReason
    {
        CR_Type             = 0,    //连接类型错误
        CR_Connect          = 1,    //连接出错
        CR_ConnectTimeout   = 2,    //连接超时
        CR_SSL              = 3,    //ssl错误
        CR_PROXY_SEND       = 4,    //代理模式下, 发送包错误
        CR_PROXY_RECV       = 5,    //代理模式下, 接收包错误
        CR_PROTOCOL         = 6,    //协议解析错误
        CR_SEND             = 7,    //发送错误
        CR_RECV             = 8,    //接收错误
        CR_ACTIVE           = 9,    //主动调用close触发
        CR_DECONSTRUCTOR    = 10,   //析构
	    CR_SSL_HANDSHAKE    = 11,   //ssl handshake错误
		CR_PEER_CLOSE     	= 12,   //对端主动关闭连接
	};

    /**
     * 鉴权状态
     */ 
    enum AUTH_STATUS
    {
        eAuthInit = -127,
        eAuthSucc = 0,
    };

    enum
    {
        DEFAULT_RECV_BUFFERSIZE = 64*1024       /*缺省数据接收buffer的大小*/
    };

    struct SocketOpt
    {
        int level;

        int optname;

        const void *optval;

        SOCKET_LEN_TYPE optlen;
    };

    //发起连接前, 创建网络句柄后的回调(只对客户端有效)
    using oncreate_callback = std::function<shared_ptr<TC_ProxyInfo>(TC_Transceiver*)>;
    //开启openssl对象的回调
    using onopenssl_callback = std::function<std::shared_ptr<TC_OpenSSL>(TC_Transceiver*)>;
    //关闭句柄的回调
    using onclose_callback = std::function<void(TC_Transceiver*, CloseReason reason, const string &err)> ;
    //建立连接上的回调(只对客户端有效)
    using onconnect_callback = std::function<void(TC_Transceiver*)> ;
    //可以发出业务请求了(对客户端: 连接建立完成, 对SSL: 握手完成, 可以发业务数据的回调)
	using onrequest_callback = std::function<void(TC_Transceiver*)> ;
    //解析协议的回调
	using onparser_callback = std::function<TC_NetWorkBuffer::PACKET_TYPE(TC_NetWorkBuffer&, TC_Transceiver*)> ;
	//完整解析完一个包之后的回调
	using oncompletepackage_callback = std::function<void(TC_Transceiver*)> ;
    //完整一次网络解析之后回调(一般有多次解析onparser_callback 以及 oncompletepackage_callback 之后回调, 通常在业务层可以在这个函数中可以把解析的数据一次性写入到队列中)
    using oncompletenetwork_callback = std::function<void(TC_Transceiver*)> ;
	//cient侧: 发送鉴权包的回调, 业务层在回调里面组织鉴权包
	using onclientsendauth_callback = std::function<shared_ptr<TC_NetWorkBuffer::Buffer>(TC_Transceiver*)> ;
    //client侧: 收到鉴权包的的回调, 业务层解包(注意返回PACKET_FULL, 表示鉴权成功)
	using onclientverifyauth_callback = std::function<TC_NetWorkBuffer::PACKET_TYPE(TC_NetWorkBuffer &, TC_Transceiver*)> ;
    //server侧: 验证鉴权包并返回验证包的回调
	using onserververifyauth_callback = std::function<pair<TC_NetWorkBuffer::PACKET_TYPE, shared_ptr<TC_NetWorkBuffer::Buffer>>(TC_NetWorkBuffer &, TC_Transceiver*)> ;

    //网络层单次时间收发包最长时间(毫秒), 为了避免网络层一直在收包, 没机会发送包, 默认就1ms, 加速范文
    static uint64_t LONG_NETWORK_TRANS_TIME;

    /**
     * 构造函数
     * @param epoller
     * @param ep, 服务端地址 
     */
    TC_Transceiver(TC_Epoller* epoller, const TC_Endpoint &ep);

    /**
     *
     *析构函数
     */
    virtual ~TC_Transceiver();

    /**
     *
     * 初始化客户端的连接
     * 句柄是connect时创建的
     */
    void initializeClient(const oncreate_callback &oncreate,
                          const onclose_callback &onclose,
                          const onconnect_callback &onconnect,
                          const onrequest_callback &onrequest,
                          const onparser_callback &onparser,
                          const onopenssl_callback &onopenssl,
                          const oncompletepackage_callback &onfinish = oncompletepackage_callback(),
                          const oncompletenetwork_callback &onfinishAll = oncompletenetwork_callback());

    /**
     *
     * 初始化服务端的连接
     * 句柄是外部accept后, 传递进去的
     */
    void initializeServer(const onclose_callback &onclose,
            const onrequest_callback &onrequest, 
            const onparser_callback &onparser, 
            const onopenssl_callback &onopenssl,
            const oncompletepackage_callback &onfinish = oncompletepackage_callback(),
            const oncompletenetwork_callback &onfinishAll = oncompletenetwork_callback());

    /**
     * 设置绑定地址(对客户端有效, 服务端本身就是绑定的)
     */
    void setBindAddr(const char *host);

    /**
     * 设置绑定地址(对客户端有效, 服务端本身就是绑定的)
     * @param bindAddr
     */
    void setBindAddr(const TC_Socket::addr_type &bindAddr);

    /**
     * 获取client地址, 如果是udp, 则表示最后一个包对应的client地址
     * @return
     */
    const TC_Socket::addr_type & getClientAddr() const { return _clientAddr; }

    /**
     * 获取server地址(如果是客户端, 并设置了代理, 则返回代理地址)
     * @return
     */
    const TC_Socket::addr_type & getServerAddr() const { return _serverAddr; }

    /**
     * 设置鉴权回调(不需要鉴权逻辑, 则不需要处理这一步)
     */ 
    void setClientAuthCallback(const onclientsendauth_callback &onsendauth, const onclientverifyauth_callback &onverify);

    /**
     * 设置鉴权回调(不需要鉴权逻辑, 则不需要处理这一步)
     */ 
    void setServerAuthCallback(const onserververifyauth_callback &onverify);

    /**
     * 绑定fd上, 一旦绑定, 就创建对fd的事件监听EPOLLIN|EPOLLOUT
     * 并且会维持句柄的持有
     * @param fd
     * @return TC_Epoller::EpollInfo*
     */ 
    shared_ptr<TC_Epoller::EpollInfo> bindFd(int fd);

    /**
     * 设置udp接收buffer大小(只对udp有效)
     */ 
    void setUdpRecvBuffer(size_t nSize);

    /**
     * 设置socket opt
     */
    void setSocketOpt(const vector<SocketOpt> &socketOpts) { _socketOpts = socketOpts; }

    /**
     * 是否ssl
     */
    bool isSSL() const ;

    /**
     * 创建连接
     */
    void connect();

    /*
     * 关闭连接
     */
    void close();

	/**
	 * send buffer
	 * @return
	 */
	inline TC_NetWorkBuffer &getSendBuffer() { return _sendBuffer; }

	/**
	 * recv buffer
	 * @return
	 */
	inline TC_NetWorkBuffer &getRecvBuffer() { return _recvBuffer; }

    /**
     * get epoll info
     */ 
    inline const shared_ptr<TC_Epoller::EpollInfo> &getEpollInfo() { return _epollInfo; }

    /**
     * 发送buffer
     * @param buff, buffer内容
     * @param addr, 发送地址(注意如果udp server, 回包一定要指向客户端的地址, 其他情况可以不传入这个地址)
     */ 
    virtual ReturnStatus sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> &buff, const TC_Socket::addr_type& addr = TC_Socket::addr_type());

    /**
     * 是否鉴权成功
     */ 
    bool isAuthSucc() const { return _authState == eAuthSucc; }

    /**
     * 鉴权成功
     */ 
    void enableAuthSucc() { _authState = eAuthSucc; }

    /**
     * 授权状态值
     */ 
    inline AUTH_STATUS getAuthStatus() const { return _authState; }

    /*
     * 发送链接中已经有数据, 如果连接数据都已经发送完毕, 则会回调onrequest_callback
     * @return int throw TC_Transceiver_Exception
     */
    void doRequest();

    /*
     * 接受网络请求
     * @return bool false: 对端关闭连接, true: 正常, throw TC_Transceiver_Exception
     */
    virtual bool doResponse() = 0;

    /*
     * 获取文件描述符
     * @return int
     */
    inline int fd() const { return _fd; }

    /*
     * 是否有效
     */
    inline bool isValid() const { return (_fd != -1); }

    /*
     * 获取端口信息(服务器地址)
     */
    inline const TC_Endpoint& getEndpoint() const { return _ep; }

    /**
     * 获取openssl
     */ 
    inline const std::shared_ptr<TC_OpenSSL>& getOpenSSL() { return _openssl; }

    /*
     * 判断是否已经连接到服务端
     */
    inline bool hasConnected() { return isValid() && (_connStatus == eConnected); }

    /*
     * 判断是否正在连接
     */
    inline bool isConnecting() { return isValid() && (_connStatus == eConnecting); }

	/**
	 * 获取连接地址
	 * @return
	 */
	inline const TC_Endpoint &getConnectEndpoint() const { return _proxyInfo? _proxyInfo->getEndpoint() : _ep; }

	/**
	 * 获取连接地址
	 * @return
	 */
	inline TC_Endpoint &getConnectEndpoint() { return _proxyInfo? _proxyInfo->getEndpoint() : _ep; }

    /**
     * 端口描述
     */ 
    inline const string& getConnectionString() const { return _desc; }

	/**
	 * @brief is ipv6 socket or not
	 * @return true if is ipv6
	 */
	inline bool isConnectIPv6() const  { return !isUnixLocal() && getConnectEndpoint().isIPv6(); }

#if !TARGET_PLATFORM_WINDOWS

	/**
	 * is unix local
	 * @return
	 */
	inline bool isUnixLocal() const  { return getConnectEndpoint().isUnixLocal(); }
#else

	/**
	 * is unix local
	 * @return
	 */
	inline bool isUnixLocal() const  { return false; }

#endif
	/**
     * 设置连接超时时间(tcp下才有效)
     */
    void setConnTimeout(int connTimeout) { _connTimeout = connTimeout; }

    /**
     * 连接超时的时间(tcp下才有效)
     */
    int getConnTimeout() const { return _connTimeout; };

    /**
     * 连接是否超时(tcp下才有效)
     */
    inline bool isConnTimeout() const { return _isConnTimeout; }

    /**
     *
     * @param connTimeout
     * @return
     */
    inline void setIsConnTimeout(bool connTimeout) { _isConnTimeout = connTimeout; }

protected:

    /**
     * create socket
     */ 
    int createSocket(bool udp, bool isLocal = false, bool isIpv6 = false);

    /**
     * connect
     */ 
	bool doConnect(int, const struct sockaddr *, socklen_t len);

    /**
     * check connect
     */ 
    void checkConnect();

    /*
     * 设置当前连接态
     */
    void setConnected();

	/*
	 * 设置当前连接态
	 */
	void onSetConnected();

	/**
	 * 检查连接超时
	 */
	void checkConnectTimeout();

    /** 
     ** 物理连接成功回调
     **/
    void onConnect();

	/**
	 ** 发送打通代理请求
	 **/
	void connectProxy();

	/**
	 * 检查是否代理创建成功
	 * @param buff
	 * @param length
	 * @return <0: 失败, 0: 成功: 1: 需要验证
	 */
	int doCheckProxy(const char *buff, size_t length);

	/**
	 * 解析域名
	 */
	void parseConnectAddress();

    /**
     * 发送鉴权代码
     */ 
    void doAuthReq();

    /**
     * 验证鉴权信息
     */ 
    void doAuthCheck(TC_NetWorkBuffer *buff);

    /**
     * 解析协议 
     */ 
    int doProtocolAnalysis(TC_NetWorkBuffer *buff);

    /*
     * 内部关闭连接, udp连接不关闭
     */
    void tcpClose(bool deconstructor, CloseReason reason, const string &err);

    /**
     * udp Close
     */ 
    void udpClose();

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

    friend class CloseClourse;
protected:

    /*
     * 服务端的地址
     */
    TC_Socket::addr_type               _serverAddr;

    /**
     * 客户端地址
     */
    TC_Socket::addr_type               _clientAddr;

    /**
     * bind addr
     */
    TC_Socket::addr_type               _bindAddr;

        /*
     * epoller
     */
    TC_Epoller*             _epoller = NULL;

    /*
     * 连接的节点信息(服务端地址)
     */
    TC_Endpoint             _ep;

    /*
     * 端口描述
     */
    string                  _desc;

    /*
     * 套接字
     */
    int                     _fd;

    /**
     * true: server端, false: 客户端
     */ 
    bool                    _isServer;

    /**
     * socket选项
     */ 
    vector<SocketOpt>       _socketOpts;

    /*
     * 事件注册信息
     */
    shared_ptr<TC_Epoller::EpollInfo>   _epollInfo;
    
    /*
     * 连接状态
     */
    ConnectStatus            _connStatus;

    /*
     * openssl
     */
    std::shared_ptr<TC_OpenSSL> _openssl;

    /*
     * 发送buffer
     */
	TC_NetWorkBuffer        _sendBuffer;

	/*
     * 接收buffer
     */
    TC_NetWorkBuffer        _recvBuffer;

	/**
	 * 代理
	 */
	shared_ptr<TC_ProxyInfo> _proxyInfo;

    /* 
     * 鉴权状态 
     */
    AUTH_STATUS              _authState = eAuthInit;

    /**
     * 最后一个包的发送地址
     */
    TC_Socket::addr_type    _lastAddr;

    /*
     * 接收缓存(udp情况才有效)
     */
    shared_ptr<TC_NetWorkBuffer::Buffer> _pRecvBuffer;

    /*
     * 接收缓存大小(udp情况才有效)
     */
    size_t                  _nRecvBufferSize = DEFAULT_RECV_BUFFERSIZE;

    /**
     * 是否超时
     */
    bool                    _isConnTimeout = false;

    /**
     * 连接超时时间
     */
    int                     _connTimeout = 5000;

    int64_t                 _connTimerId = 0;

    oncreate_callback       _createSocketCallback;

    onopenssl_callback      _onOpensslCallback;

    onconnect_callback      _onConnectCallback;

    onrequest_callback      _onRequestCallback;

    onclose_callback        _onCloseCallback;

    onparser_callback       _onParserCallback;

	oncompletepackage_callback      _onCompletePackageCallback;

    oncompletenetwork_callback      _onCompleteNetworkCallback;

    onclientsendauth_callback       _onClientSendAuthCallback;

    onclientverifyauth_callback     _onClientVerifyAuthCallback;

    onserververifyauth_callback     _onServerVerifyAuthCallback;
};

//////////////////////////////////////////////////////////
/**
 * TCP 传输实现
 */
class TC_TCPTransceiver : public TC_Transceiver
{
public:
    /**
     * 构造函数
     * @param ep
     * @param fd
     */
    TC_TCPTransceiver(TC_Epoller* epoller, const TC_Endpoint &ep);

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
     * @return throw
     */
	virtual bool doResponse();
};


//////////////////////////////////////////////////////////
/**
 * SSL 传输实现
 */
class TC_SSLTransceiver : public TC_TCPTransceiver
{
public:
    /**
     * 构造函数
     * @param ep
     * @param fd
     */
    TC_SSLTransceiver(TC_Epoller* epoller, const TC_Endpoint &ep);

    /**
     * 处理返回
     * @return throw 
     */
	virtual bool doResponse();
};

//////////////////////////////////////////////////////////
/**
 * UDP 传输实现
 */
class TC_UDPTransceiver : public TC_Transceiver
{
public:

    /**
     * 构造函数
     */
    TC_UDPTransceiver(TC_Epoller* epoller, const TC_Endpoint &ep);

    /**
     * 析构函数
     */
    ~TC_UDPTransceiver();

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
     * @return throw
     */
	virtual bool doResponse();

protected:
};

}

#endif
