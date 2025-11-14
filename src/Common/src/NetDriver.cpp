#include <memory>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Common/inc/NetDriver.h"

NetSender::NetSender(TC_EpollServer* ep, uint32_t uid, int threadIndex,
                     const TC_EpollServer::BindAdapterPtr &adapter)
    : _ep(ep), _uid(uid), _threadIndex(threadIndex)
{
    if (adapter) _adapter = adapter;
    // 尝试推断是否为 UDP（如果 adapter 可用）
    if (adapter) {
        _isUdp = adapter->getEndpoint().isUdp();
    } else if (_ep) {
        // 退化尝试：若能通过 netThread/connection 推断则设置（尽量避免直接调用 protected）
        // 这里不调用 getConnectionPtr()，保持安全；_isUdp 默认为 false
    }
}

int NetSender::send(const char* data, size_t len, const std::string &ip, uint16_t port)
{
    std::cout << "==== send data: " << data << " len: " << len << " ip: " << ip << " port: " << port << std::endl;
    if (!_ep) return -1;

    // 优先使用 adapter（如果存在）
    auto adapterPtr = _adapter.lock();

    if (_isUdp) {
        // UDP发送路径：使用 adapter 的 socket 执行 sendto
        if (!adapterPtr) return -1;

        std::string targetIp = ip.empty() ? _lastIp : ip;
        uint16_t targetPort = port == 0 ? _lastPort : port;

        if (targetIp.empty() || targetPort == 0) return -1;

        _lastIp = targetIp;
        _lastPort = targetPort;

        try {
            return adapterPtr->getSocket().sendto(data, len, targetIp, targetPort);
        } catch (...) {
            return -1;
        }
    } else {
        // TCP发送路径：构造一个临时 RecvContext -> SendContext 并通过 NetThread::send 发送
        const auto &netThreads = _ep->getNetThread();
        if (_threadIndex < 0 || (size_t)_threadIndex >= netThreads.size()) return -1;

        TC_EpollServer::NetThread* nt = netThreads[_threadIndex];
        if (!nt) return -1;

        // 获取 adapter（优先使用保存的 adapter，否则尝试从 _epoll 通过 name 查找：注意 makeSender 应该提供 adapter）
        if (!adapterPtr) {
            // 不能获取 adapter，则无法用安全方式构造 RecvContext
            return -1;
        }

        // 构造临时 RecvContext：addr/fd 可以是空/0，因为 NetThread::send 内部通过 uid() 找到 Connection
        TC_Socket::addr_type emptyAddr; // default constructed
        auto recv = std::make_shared<TC_EpollServer::RecvContext>(_threadIndex, _uid, emptyAddr, 0, adapterPtr);

        // 创建 SendContext
        auto sendCtx = recv->createSendContext();

        // 创建并赋值发送 buffer
        auto buff = std::make_shared<TC_NetWorkBuffer::Buffer>();
        buff->setBuffer(data, len); // 使用 Buffer::setBuffer 或 addBuffer（见实现）
        sendCtx->setBuffer(buff);

        // 将发送请求派发到对应的网络线程（NetThread::send 是公开的）
        try {
            nt->send(sendCtx);
        } catch (...) {
            return -1;
        }
        return (int)len;
    }
}

void NetSender::close()
{
    // 关闭 TCP 连接：通过构造 RecvContext->createCloseContext 并入队列
    if (!_ep) return;
    auto adapterPtr = _adapter.lock();
    if (_isUdp) {
        // UDP 没有连接，清理状态
        _lastIp.clear();
        _lastPort = 0;
        return;
    }

    const auto &netThreads = _ep->getNetThread();
    if (_threadIndex < 0 || (size_t)_threadIndex >= netThreads.size()) return;
    TC_EpollServer::NetThread* nt = netThreads[_threadIndex];
    if (!nt) return;

    if (!adapterPtr) return;

    TC_Socket::addr_type emptyAddr;
    auto recv = std::make_shared<TC_EpollServer::RecvContext>(_threadIndex, _uid, emptyAddr, 0, adapterPtr);
    auto closeCtx = recv->createCloseContext();
    try {
        nt->send(closeCtx);
    } catch (...) {
        // ignore
    }
}

// ---------- NetDriver ----------
NetDriver::NetDriver(unsigned int netThreads)
    : _netThreads(netThreads > 0 ? netThreads : 1)
{
    std::cout << ">>>>>>> [DEBUG] NetDriver constructor _netThreads=" << _netThreads << std::endl;
}

NetDriver::~NetDriver()
{
    terminate();
}

bool NetDriver::initialize()
{
    std::cout << "==== initialize ==== " << std::endl;
        std::cout << ">>>>>>> [DEBUG] initialize starting with _netThreads=" << _netThreads << std::endl;

    if (!_epoll) {
        _epoll = std::unique_ptr<TC_EpollServer>(new TC_EpollServer());
    }

    // 设置工作线程数
    _epoll->setThreadNum(_netThreads);

    // 这里我们保持最小初始化；TC_EpollServer 默认构造后即可 createBindAdapter/bind。
    // 如需配置线程数或其他选项，可在此处扩展（依赖于 TC_EpollServer 的接口）。
      // 注册 accept 回调，以便 TCP accept 时通知 NetDriver::onAccept
    _epoll->setOnAccept(std::bind(&NetDriver::onAccept, this, std::placeholders::_1));
    
    return true;
}

void NetDriver::terminate()
{
    // 解除所有适配器引用并销毁 _epoll
    try {
        // 如果 TC_EpollServer 提供停止方法，这里可以调用它；为了兼容性只释放对象
        _adapters.clear();
        _udpEndpoints.clear();
        _epoll.reset();
    } catch (...) {
        // 忽略
    }
}

NetSender NetDriver::makeSender(const ConnectionInfo &conn)
{
    if (!_epoll) {
        return NetSender(nullptr, 0, -1, nullptr);
    }

    TC_EpollServer::BindAdapterPtr adapterPtr;
    if (!conn.adapterName.empty()) {
        adapterPtr = _epoll->getBindAdapter(conn.adapterName);
    }

    return NetSender(_epoll.get(), conn.uid, conn.threadIndex, adapterPtr);
}

void NetDriver::onAccept(TC_EpollServer::Connection* cPtr)
{
    std::cout << "==== onAccept ==== " << std::endl;
    // 保留hook：如果需要，在此处处理新连接事件（例如日志、统计或初始化）
    // 当前上层通过 ForwardHandle::handle 中 _receiver->recvConnection 被通知 TCP 的新连接。
    try {
        if (!cPtr) return;

        auto adapter = cPtr->getBindAdapter();
        if (!adapter) return;

        // 根据 adapter name 找到对应 receiver
        auto it = _adapters.find(adapter->getName());
        if (it == _adapters.end()) return;
        NetReceiverPtr receiver = it->second;
        if (!receiver) return;

        auto info = std::make_shared<ConnectionInfo>();
        info->ip = cPtr->getIp();
        info->port = cPtr->getPort();
        info->uid = cPtr->getId();
        info->threadIndex = cPtr->getNetThread() ? cPtr->getNetThread()->getIndex() : 0;
        info->adapterName = adapter->getName();
        info->_isUdp = adapter->getEndpoint().isUdp();

        // 直接回调，上层会收到 TCP 的 recvConnection
        receiver->recvConnection(info);
    } catch (...) {
        // 忽略异常，保证 accept 流程不被影响
    }
}

// ForwardHandle::handleClose 补充实现
void NetDriver::ForwardHandle::handleClose(const std::shared_ptr<TC_EpollServer::RecvContext> &data)
{
    std::cout << "==== handleClose ==== " << std::endl;

    try {
        if (!_receiver) return;

        auto info = std::make_shared<ConnectionInfo>();
        info->ip = data->ip();
        info->port = data->port();
        info->uid = data->uid();
        info->threadIndex = data->threadIndex();
        
        auto ad = data->adapter();
        if (ad) {
            info->adapterName = ad->getName();
            info->_isUdp = ad->getEndpoint().isUdp();
        }

        // 对于 UDP，移除首次通信记录（如果存在）
        if (info->_isUdp) {
            auto it = _driver->_udpEndpoints.find(info->uid);
            if (it != _driver->_udpEndpoints.end()) {
                _driver->_udpEndpoints.erase(it);
            }
        }

        // closeType 由 RecvContext 提供（若没有可用值则传0）
        int closeType = 0;
#ifdef TARS_HAVE_CLOSE_TYPE
        closeType = data->closeType();
#endif
        _receiver->onConnClose(info, closeType);
    }
    catch (const std::exception &) {
        // 忽略，确保不会抛出
    }
}
bool NetDriver::listen(const std::string &name, 
                      const std::string &endpoint,
                      const NetReceiverPtr &receiver,
                      size_t handleNum,
                      const TC_NetWorkBuffer::protocol_functor &pf,
                      const std::map<std::string, std::string>& options)
{
    if (!_epoll || !receiver) return false;
    std::cout << "==== listen ==== " << "adapterName: " << name << " endpoint: " << endpoint << std::endl;

    std::string adapterName = name;
    if (_adapters.find(adapterName) != _adapters.end()) {
        adapterName += "_" + std::to_string(_adapters.size());
    }
    _adapters[adapterName] = receiver;

    try {
        // 创建适配器
        auto adapter = _epoll->createBindAdapter<NetDriver::ForwardHandle>(
            adapterName, endpoint, handleNum, receiver, this);

        // 解析endpoint判断协议
        TC_Endpoint ep;
        ep.parse(endpoint);
        
       if (ep.isUdp()) {
            // UDP特定设置
            size_t udpBufferSize = 64*1024;  // 默认64K
            
            // 从options中获取设置
            auto it = options.find("udpBufferSize");
            if (it != options.end()) {
                udpBufferSize = std::stoul(it->second);
            }
            
             // 使用底层 socket 设置 UDP 缓冲区
            try {
                adapter->getSocket().setRecvBufferSize((int)udpBufferSize);
            } catch (...) { /* ignore */ }
            try {
                adapter->getSocket().setSendBufferSize((int)udpBufferSize);
            } catch (...) { /* ignore */ }
        }else {
            // TCP 可选的 recv/send buffer
            auto itR = options.find("tcpRecvBufferSize");
            if (itR != options.end()) {
                 try {
                adapter->getSocket().setRecvBufferSize((int)std::stoul(itR->second));
                } catch (...) { /* ignore */ }
            }
            auto itS = options.find("tcpSendBufferSize");
            if (itS != options.end()) {
                 try {
                adapter->getSocket().setSendBufferSize((int)std::stoul(itS->second));
                } catch (...) { /* ignore */ }
                
            }
        }

        // 设置协议解析器
        if (pf) {
            adapter->setProtocol(pf);
        } else {
            std::cout << ">>>>>>> [DEBUG] using echo protocol for " << adapterName << std::endl;
            adapter->setProtocol(TC_NetWorkBuffer::parseEcho);
        }

        // 启动监听
        _epoll->bind(adapter);
        // The following will initialize/start TC_EpollServer *once* (after adapters are bound).
        // Do this only the first time we bind an adapter.
        {
            std::lock_guard<std::mutex> lg(_startMutex);
            if (!_started && !_startScheduled) {
                _startScheduled = true;
                // start in background after a short debounce so subsequent listen() calls
                // can register additional adapters before the server init runs.
                std::thread([this]() {
                    try {
                        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // debounce window
                        std::lock_guard<std::mutex> lg2(_startMutex);
                        if (_started) return;

                        std::cout << ">>>>>>> [DEBUG] initializing & starting epoll server handles/threads (deferred)" << std::endl;
                        _epoll->initHandle();
                        _epoll->createEpoll();
                        _epoll->startHandle();

                        // 重要：为所有 TCP 监听 socket 注册 epoll 回调（参考 waitForShutdown 的实现）
                        // 监听 socket 需要注册到主 epoller，而不是网络线程的 epoller
                        std::cout << ">>>>>>> [DEBUG] Registering listen sockets..." << std::endl;
                        auto bindAdapters = _epoll->getBindAdapters();
                        std::cout << ">>>>>>> [DEBUG] Found " << bindAdapters.size() << " bind adapters" << std::endl;
                        
                        for(const auto& it : bindAdapters)
                        {
                            if(it->getEndpoint().isTcp())
                            {
                                std::cout << ">>>>>>> [DEBUG] Checking TCP adapter: " << it->getName() << std::endl;
                                if(it->getSocket().isValid())
                                {
                                    std::cout << ">>>>>>> [DEBUG] Registering listen socket for adapter: " << it->getName() << " fd=" << it->getSocket().getfd() << std::endl;
                                    // socket 有效，说明已经绑定了，注册 accept 回调
                                    shared_ptr<TC_Epoller::EpollInfo> info = _epoll->getEpoller()->createEpollInfo(it->getSocket().getfd());
                                    
                                    it->setEpollInfo(info);
                                    
                                    map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;
                                    
                                    // 使用自定义回调，手动实现 accept 逻辑（因为 accept 和 acceptCallback 都是 protected）
                                    weak_ptr<TC_EpollServer::BindAdapter> weakAdapter = it;
                                    TC_EpollServer* epollPtr = _epoll.get();
                                    NetDriver* driver = this;
                                    
                                    callbacks[EPOLLIN] = [weakAdapter, epollPtr, driver](const shared_ptr<TC_Epoller::EpollInfo> &info) -> bool {
                                        auto adapter = weakAdapter.lock();
                                        if (!adapter) return false;
                                        
                                        int fd = info->fd();
                                        int domain = adapter->getEndpoint().isIPv6() ? AF_INET6 : AF_INET;
                                        
                                        // 循环 accept 直到没有更多连接（参考 acceptCallback 的实现）
                                        bool ret;
                                        bool hasAccepted = false;
                                        do {
                                            // 使用原始 socket API 实现 accept（参考 TC_EpollServer::accept 的实现）
                                            struct sockaddr_in stSockAddr4;
                                            struct sockaddr_in6 stSockAddr6;
                                            socklen_t iSockAddrSize = (AF_INET6 == domain) ? sizeof(sockaddr_in6) : sizeof(sockaddr_in);
                                            struct sockaddr *stSockAddr = (AF_INET6 == domain) ? (struct sockaddr *) &stSockAddr6 : (struct sockaddr *) &stSockAddr4;
                                            
                                            TC_Socket cs;
                                            cs.setOwner(false);
                                            TC_Socket s;
                                            s.init(fd, false, domain);
                                            
                                            int iRetCode = s.accept(cs, (struct sockaddr *) stSockAddr, iSockAddrSize);
                                            
                                            if (iRetCode > 0) {
                                                hasAccepted = true;
                                                // 解析 IP 和端口
                                                char sAddr[INET6_ADDRSTRLEN] = "\0";
                                                inet_ntop(domain,
                                                        (AF_INET6 == domain) ? (void *) &stSockAddr6.sin6_addr : (void *) &stSockAddr4.sin_addr,
                                                        sAddr, sizeof(sAddr));
                                                uint16_t port = (AF_INET6 == domain) ? ntohs(stSockAddr6.sin6_port) : ntohs(stSockAddr4.sin_port);
                                                std::string ip = sAddr;
                                                
                                                // 获取网络线程
                                                const std::vector<TC_EpollServer::NetThread *> &netThreads = adapter->getNetThreads();
                                                if (!netThreads.empty()) {
                                                    TC_EpollServer::NetThread *netThread = netThreads[cs.getfd() % netThreads.size()];
                                                    
                                                    // 创建 Connection（参考 TC_EpollServer::accept 的实现）
                                                    TC_EpollServer::Connection *cPtr = new TC_EpollServer::Connection(
                                                        netThread->getConnectionList(), 
                                                        adapter.get(), 
                                                        cs.getfd(), 
                                                        ip, 
                                                        port, 
                                                        epollPtr
                                                    );
                                                    
                                                    // 设置 socket 选项
                                                    cs.setblock(false);
                                                    try {
                                                        cs.setKeepAlive();
                                                        cs.setTcpNoDelay();
                                                        cs.setCloseWaitDefault();
                                                    } catch (...) {}
                                                    
                                                    // 过滤连接首个数据包包头
                                                    cPtr->setHeaderFilterLen((int) adapter->getHeaderFilterLen());
                                                    
                                                    // 添加到网络线程（手动实现 addTcpConnection 的逻辑）
                                                    auto connList = netThread->getConnectionList();
                                                    uint32_t uid = connList->getUniqId();
                                                    cPtr->initialize(netThread->getEpoller(), uid, netThread);
                                                    connList->add(cPtr, cPtr->getTimeout() + TNOW);
                                                    adapter->increaseNowConnection();
                                                    cPtr->registerEvent(netThread);
                                                }
                                                
                                                ret = true;
                                            } else {
                                                ret = false;
                                            }
                                        } while (ret);
                                        
                                        return hasAccepted;  // 返回是否接受了至少一个连接
                                    };
                                    
                                    info->registerCallback(callbacks, EPOLLIN);
                                    std::cout << ">>>>>>> [DEBUG] Listen socket registered for adapter: " << it->getName() << std::endl;
                                } else {
                                    std::cout << ">>>>>>> [DEBUG] TCP adapter " << it->getName() << " socket is invalid" << std::endl;
                                }
                            }
                        }
                        std::cout << ">>>>>>> [DEBUG] Listen socket registration completed" << std::endl;
                        
                        _epoll->waitForReady();
                        
                        std::cout << ">>>>>>> [DEBUG] Starting main epoller loop..." << std::endl;
                        // 关键：启动主 epoller 循环（在后台线程中运行）
                        // 这会在后台线程中运行，直到服务器关闭
                        std::thread([this]() {
                            try {
                                std::cout << ">>>>>>> [DEBUG] Main epoller loop started" << std::endl;
                                _epoll->getEpoller()->loop(10000);
                                std::cout << ">>>>>>> [DEBUG] Main epoller loop exited" << std::endl;
                            } catch (const std::exception &e) {
                                std::cout << ">>>>>>> [ERROR] Main epoller loop error: " << e.what() << std::endl;
                            } catch (...) {
                                std::cout << ">>>>>>> [ERROR] Main epoller loop unknown error" << std::endl;
                            }
                        }).detach();
                        
                        _started = true;
                    } catch (const std::exception &e) {
                        std::cout << ">>>>>>> [ERROR] failed to start epoll server (deferred): " << e.what() << std::endl;
                    } catch (...) {
                        std::cout << ">>>>>>> [ERROR] unknown failure starting epoll server (deferred)" << std::endl;
                    }
                }).detach();
            }
        }
    }
    catch (const std::exception &e) {
        std::cout << ">>>>>>> [ERROR] failed to create adapter: " << e.what() << std::endl;
        return false;
    }

    return true;
}
bool NetDriver::listen(const std::string &protocol,
                      const std::string &localHost,
                      int localPort,
                      const NetReceiverPtr &receiver,
                      bool reuseAddr)
{
    if (!_epoll || !receiver)
    {
        std::cout << ">>>>>>> [ERROR] _epoll or _receiver is null" << std::endl;
        return false;
    }

    std::cout << "==== listen ==== " << "protocol: " << protocol << " localHost: " << localHost << " localPort: " << localPort << std::endl;

    // 规范化 protocol
    std::string proto = protocol;
    for (auto &c : proto) c = (char)std::tolower((unsigned char)c);

    if (proto != "tcp" && proto != "udp")
        return false;

    std::string host = localHost.empty() ? "0.0.0.0" : localHost;
    std::string endpoint = proto + " -h " + host + " -p " + std::to_string(localPort);

    // 生成一个默认的 adapter name（确保唯一）
    std::string name = proto + "_" + host + "_" + std::to_string(localPort);
    if (_adapters.find(name) != _adapters.end()) {
        name += "_" + std::to_string(_adapters.size());
    }

    std::map<std::string, std::string> options;
    if (reuseAddr)
        options["SO_REUSEADDR"] = "1";

    // 对 UDP 可能希望默认设置较大的缓冲
    if (proto == "udp")
        options["udpBufferSize"] = std::to_string(64*1024);

    // 调用具体参数版本的 listen
    return listen(name, endpoint, receiver, 1, TC_NetWorkBuffer::protocol_functor(), options);
}

NetSenderPtr NetDriver::connect(const std::string &protocol, const std::string &localHost,
                        int localPort, const std::string &remoteHost, int remotePort,
                        const NetReceiverPtr &receiver, bool reuseAddr)
{
    if (!_epoll || !receiver) {
        std::cout << ">>>>>>> [ERROR] _epoll or receiver is null" << std::endl;
        return nullptr;
    }

    // 规范化 protocol
    std::string proto = protocol;
    for (auto &c : proto) c = (char)std::tolower((unsigned char)c);

    if (proto != "tcp" && proto != "udp") {
        std::cout << ">>>>>>> [ERROR] invalid protocol: " << protocol << std::endl;
        return nullptr;
    }

    // 确保 epoll server 已启动
    if (!waitForEpollStart()) {
        std::cout << ">>>>>>> [ERROR] epoll server start timeout" << std::endl;
        return nullptr;
    }

    try {
        if (proto == "tcp") {
            return connectTcp(localHost, localPort, remoteHost, remotePort, receiver, reuseAddr);
        } else {
            return connectUdp(localHost, localPort, remoteHost, remotePort, receiver, reuseAddr);
        }
    } catch (const std::exception &e) {
        std::cout << ">>>>>>> [ERROR] connect failed: " << e.what() << std::endl;
        return nullptr;
    }
}

NetSenderPtr NetDriver::connectTcp(const std::string &localHost, int localPort,
                                  const std::string &remoteHost, int remotePort,
                                  const NetReceiverPtr &receiver, bool reuseAddr) 
{
    // 创建客户端适配器（如果不存在）
    if (!_clientAdapter) {
        if (!initClientAdapter()) {
            std::cout << ">>>>>>> [ERROR] Failed to init client adapter" << std::endl;
            return nullptr;
        }
    }

    TC_Socket tcpSocket;
    tcpSocket.createSocket();
    
    // 设置 socket 选项
    if (reuseAddr) {
        int flag = 1;
        ::setsockopt(tcpSocket.getfd(), SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    }

     // 绑定本地地址（如果指定）
     if (!localHost.empty() && localPort > 0) {
        try {
            tcpSocket.bind(localHost, localPort);
        } catch (const std::exception &e) {
            std::cout << ">>>>>>> [ERROR] TCP bind failed: " << localHost << ":" << localPort 
                      << ", error: " << e.what() << std::endl;
            return nullptr;
        }
    }

    // 设置非阻塞模式
    tcpSocket.setblock(false);

    // 连接远程地址
    int ret = tcpSocket.connectNoThrow(remoteHost, remotePort);
    if (ret != 0 && errno != EINPROGRESS) {
        std::cout << ">>>>>>> [ERROR] TCP connect failed to " << remoteHost << ":" << remotePort 
                  << ", errno: " << errno << std::endl;
        return nullptr;
    }

   
    // 获取网络线程（参考 tc_epoll_server 的实现）
    const std::vector<TC_EpollServer::NetThread *> &netThreads = _clientAdapter->getNetThreads();
    if (netThreads.empty()) {
        std::cout << ">>>>>>> [ERROR] No net threads available" << std::endl;
        return nullptr;
    }

    // 选择网络线程（根据 fd 选择，参考 tc_epoll_server accept 的实现）
    TC_EpollServer::NetThread *netThread = netThreads[tcpSocket.getfd() % netThreads.size()];
    if (!netThread) {
        std::cout << ">>>>>>> [ERROR] NetThread is null" << std::endl;
        return nullptr;
    }

    // 创建 Connection 对象（参考 tc_epoll_server accept 的实现）
    // 使用带 ip 和 port 的构造函数，用于 TCP 连接
    TC_EpollServer::Connection *cPtr = new TC_EpollServer::Connection(
        netThread->getConnectionList(), 
        _clientAdapter.get(), 
        tcpSocket.getfd(), 
        remoteHost, 
        remotePort, 
        _epoll.get()
    );

    // 防止 TC_Socket 析构关闭 fd（因为 epoll 现在管理这个 fd）
    tcpSocket.setOwner(false);
   
    // 手动实现 addTcpConnection 的逻辑（因为 addTcpConnection 是 protected）
    // 参考 tc_epoll_server::NetThread::addTcpConnection 的实现
    auto connList = netThread->getConnectionList();
    uint32_t uid = connList->getUniqId();
    
    // 初始化 Connection
    cPtr->initialize(netThread->getEpoller(), uid, netThread);
    
    // 添加到连接列表
    connList->add(cPtr, cPtr->getTimeout() + TNOW);
    
    // 增加适配器的当前连接数
    _clientAdapter->increaseNowConnection();
    
    // 注册 epoll 事件（必须在最后调用，参考 tc_epoll_server 的注释）
    cPtr->registerEvent(netThread);

    // 获取连接 ID（在 initialize 后，Connection 已经被分配了 uid）
    uid = cPtr->getId();

    // 保存接收器映射
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _clientConnections[uid] = receiver;
    }

    // 创建连接信息并触发连接建立回调
    auto connInfo = std::make_shared<ConnectionInfo>();
    connInfo->uid = uid;
    connInfo->threadIndex = netThread->getIndex();
    connInfo->_isUdp = false;
    connInfo->ip = remoteHost;
    connInfo->port = remotePort;
    connInfo->adapterName = "client_adapter";

    receiver->recvConnection(connInfo);

    // 创建发送器
    return std::make_shared<NetSender>(_epoll.get(), uid, connInfo->threadIndex, _clientAdapter);
}
NetSenderPtr NetDriver::connectUdp(const std::string &localHost, int localPort,
                                  const std::string &remoteHost, int remotePort,
                                  const NetReceiverPtr &receiver, bool reuseAddr) 
{
     // 创建客户端适配器（如果不存在）
     if (!_clientAdapter) {
        if (!initClientAdapter()) {
            std::cout << ">>>>>>> [ERROR] Failed to init client adapter" << std::endl;
            return nullptr;
        }
    }

    TC_Socket udpSocket;
    udpSocket.createSocket(SOCK_DGRAM);
    
    // 设置 socket 选项
    if (reuseAddr) {
        int flag = 1;
        ::setsockopt(udpSocket.getfd(), SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    }

    // 绑定本地地址（如果指定）
    if (!localHost.empty() && localPort > 0) {
        try {
            udpSocket.bind(localHost, localPort);
        } catch (const std::exception &e) {
            std::cout << ">>>>>>> [ERROR] UDP bind failed: " << localHost << ":" << localPort 
                      << ", error: " << e.what() << std::endl;
            return nullptr;
        }
    }

    // UDP "连接"：设置默认目标地址（可选，用于后续 send/recv）
    udpSocket.connect(remoteHost, remotePort);

    // 获取网络线程
    const std::vector<TC_EpollServer::NetThread *> &netThreads = _clientAdapter->getNetThreads();
    if (netThreads.empty()) {
        std::cout << ">>>>>>> [ERROR] No net threads available" << std::endl;
        return nullptr;
    }

    // 选择网络线程
    TC_EpollServer::NetThread *netThread = netThreads[udpSocket.getfd() % netThreads.size()];
    if (!netThread) {
        std::cout << ">>>>>>> [ERROR] NetThread is null" << std::endl;
        return nullptr;
    }

    // 创建 Connection 对象（UDP 使用不带 ip/port 的构造函数）
    TC_EpollServer::Connection *cPtr = new TC_EpollServer::Connection(
        netThread->getConnectionList(), 
        _clientAdapter.get(), 
        udpSocket.getfd(), 
        _epoll.get()
    );

    // 防止 TC_Socket 析构关闭 fd
    udpSocket.setOwner(false);

    // 手动实现 addTcpConnection 的逻辑（因为 addTcpConnection 是 protected）
    // 参考 tc_epoll_server::NetThread::addTcpConnection 的实现
    auto connList = netThread->getConnectionList();
    uint32_t uid = connList->getUniqId();
    
    // 初始化 Connection
    cPtr->initialize(netThread->getEpoller(), uid, netThread);
    
    // 添加到连接列表
    connList->add(cPtr, cPtr->getTimeout() + TNOW);
    
    // 增加适配器的当前连接数
    _clientAdapter->increaseNowConnection();
    
    // 注册 epoll 事件（必须在最后调用，参考 tc_epoll_server 的注释）
    cPtr->registerEvent(netThread);

    // 获取连接 ID（在 initialize 后，Connection 已经被分配了 uid）
    uid = cPtr->getId();

    // 保存接收器映射
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _clientConnections[uid] = receiver;
    }
    // 创建连接信息并触发连接建立回调
    auto connInfo = std::make_shared<ConnectionInfo>();
    connInfo->uid = uid;
    connInfo->threadIndex = netThread->getIndex();
    connInfo->_isUdp = true;
    connInfo->ip = remoteHost;
    connInfo->port = remotePort;
    connInfo->adapterName = "client_adapter";
    
    receiver->recvConnection(connInfo);

    // 创建发送器
    return std::make_shared<NetSender>(_epoll.get(), uid, connInfo->threadIndex, _clientAdapter);
}

bool NetDriver::initClientAdapter() 
{
    if (_clientAdapter) {
        return true;
    }
    
    try {
        // 确保 epoll server 已启动（必须在创建适配器之前）
        if (!waitForEpollStart()) {
            std::cout << ">>>>>>> [ERROR] epoll server not started" << std::endl;
            return false;
        }
        // 创建专用的客户端适配器
        _clientAdapter = std::make_shared<tars::TC_EpollServer::BindAdapter>(_epoll.get());
        _clientAdapter->setName("client_adapter");
        // 设置为手动监听模式，避免调用 bind()（客户端不需要监听）
        _clientAdapter->enableManualListen();

        // setHandle<T>(数量, 构造函数参数...)
        // ForwardHandle 的构造函数是: ForwardHandle(const NetReceiverPtr &r, NetDriver* driver)
        _clientAdapter->setHandle<NetDriver::ForwardHandle>(1, nullptr, this);
        // 设置最大连接数（为客户端连接预留空间）
        _clientAdapter->setMaxConns(10240);
         // 绑定到 epoll server
         _epoll->bind(_clientAdapter);
        // 手动分配网络线程（因为 initHandle() 已经执行过了）
        const std::vector<TC_EpollServer::NetThread *> &netThreads = _epoll->getNetThread();
        if (!netThreads.empty()) {
            _clientAdapter->setNetThreads(netThreads);
        } else {
            std::cout << ">>>>>>> [ERROR] No net threads available for client adapter" << std::endl;
            return false;
        }
        
        // 重新调用 createEpoll() 来更新连接数（包括客户端适配器）
        // 这会重新计算 maxAllConn 并重新初始化所有 ConnectionList
        _epoll->createEpoll();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Init client adapter failed: " << e.what() << std::endl;
        return false;
    }
}

bool NetDriver::waitForEpollStart() 
{
    {
        std::lock_guard<std::mutex> lg(_startMutex);
        if (_started) return true;
        
        if (!_startScheduled) {
            _startScheduled = true;
            std::thread([this]() {
                try {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    std::lock_guard<std::mutex> lg2(_startMutex);
                    if (_started) return;

                    _epoll->initHandle();
                    _epoll->createEpoll();
                    _epoll->startHandle();
                    _epoll->waitForReady();
                    _started = true;
                } catch (const std::exception &e) {
                    std::cout << ">>>>>>> [ERROR] failed to start epoll server: " << e.what() << std::endl;
                }
            }).detach();
        }
    }  // 释放锁，让后台线程可以执行

    // 等待启动完成（在锁外等待，避免死锁）
    for (int i = 0; i < 50; ++i) {  // 5 秒超时
        {
            std::lock_guard<std::mutex> lg(_startMutex);
            if (_started) return true;
        }  // 每次检查后立即释放锁
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;  // 超时返回 false
}

bool NetDriver::waitForServerReady() 
{
    // 等待启动完成（最多 10 秒）
    for (int i = 0; i < 100; ++i) {
        {
            std::lock_guard<std::mutex> lg(_startMutex);
            if (_started) return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;
}

void NetDriver::ForwardHandle::handle(const std::shared_ptr<TC_EpollServer::RecvContext> &data)
{
    try {
         
        if (!_receiver) {
            std::cout << ">>>>>>> [ERROR] _receiver is null" << std::endl;
            return;
        }

        auto info = std::make_shared<ConnectionInfo>();
        info->ip = data->ip();
        info->port = data->port();
        info->uid = data->uid();
        info->threadIndex = data->threadIndex();
        
        std::cout << ">>>>>>> [DEBUG] handle got data from " << info->ip << ":" << info->port << std::endl;
        
        auto ad = data->adapter();
        if (ad) {
            info->adapterName = ad->getName();
            info->_isUdp = ad->getEndpoint().isUdp();
        } else {
            std::cout << ">>>>>>> [ERROR] adapter is null" << std::endl;
        }

        // UDP首次通信时触发connection回调
        if (info->_isUdp) {
            std::cout << ">>>>>>> [DEBUG] checking UDP first comm" << std::endl;
            if (_driver->_udpEndpoints.find(info->uid) == _driver->_udpEndpoints.end()) {
                std::cout << ">>>>>>> [DEBUG] UDP first comm, calling recvConnection" << std::endl;
                _driver->_udpEndpoints[info->uid] = std::make_pair(info->ip, info->port);
                _receiver->recvConnection(info);
            }
        }

        _receiver->recv(info, data->buffer());
    }
    catch (const std::exception &e) {
        std::cout << ">>>>>>> [ERROR] exception in handle: " << e.what() << std::endl;
        close(data);
    }
}