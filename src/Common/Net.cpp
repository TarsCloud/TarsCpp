#include "Common/Net.h"
#include "util/tc_epoller.h"
#include "util/tc_transceiver.h"
#include "util/tc_socket.h"
#include "util/tc_clientsocket.h"
#include "util/tc_common.h"
#include "util/tc_logger.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <map>
#include <memory>
#include <chrono>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <queue>
#include <condition_variable>
#include <vector>

namespace Common
{

// 静态变量
static std::atomic<bool> g_netSuspend(false);

// TarsNetSender 实现
class TarsNetSender : public NetSender
{
public:
    TarsNetSender(std::shared_ptr<tars::TC_Transceiver> transceiver, 
                  const NetReceiverPtr& receiver,
                  const std::string& protocol)
        : _transceiver(transceiver)
        , _receiver(receiver)
        , _protocol(protocol)
        , _closed(false)
    {
    }

    virtual ~TarsNetSender()
    {
        close();
    }

    virtual int send(const unsigned char *data, int dataLen) override
    {
        if (_closed || !_transceiver || !_transceiver->isValid())
        {
            return -1;
        }
        
        if (dataLen <= 0 || !data)
        {
            return -1;
        }

        try
        {
            auto buffer = std::make_shared<tars::TC_NetWorkBuffer::Buffer>();
            buffer->addBuffer((const char*)data, dataLen);
            
            auto ret = _transceiver->sendRequest(buffer);
            if (ret == tars::TC_Transceiver::eRetOk || ret == tars::TC_Transceiver::eRetFull)
            {
                return dataLen;
            }
            return -1;
        }
        catch (...)
        {
            return -1;
        }
    }

    virtual int send(int did, int level, const unsigned char *data, int dataLen) override
    {
        // 对于 TarsCpp，did 和 level 暂时忽略，直接发送
        return send(data, dataLen);
    }

    virtual void close() override
    {
        if (!_closed)
        {
            _closed = true;
            if (_transceiver)
            {
                try
                {
                    _transceiver->close();
                }
                catch (...)
                {
                }
            }
            if (_receiver)
            {
                _receiver->onConnClose();
            }
        }
    }

    virtual bool isSendReady() override
    {
        return !_closed && _transceiver && _transceiver->isValid();
    }

    virtual bool isClosed() override
    {
        return _closed || !_transceiver || !_transceiver->isValid();
    }

    virtual void setBufSize(int sendBytes, int recvBytes) override
    {
        // TarsCpp 的 buffer 大小在创建时设置，这里可以忽略或记录
    }

    virtual void setParams(const std::map<std::string, std::string>& params) override
    {
        _params = params;
    }

    virtual std::string getParam(const std::string& name) override
    {
        auto it = _params.find(name);
        if (it != _params.end())
        {
            return it->second;
        }
        return std::string();
    }

    virtual std::string protocol() override
    {
        return _protocol;
    }

    virtual int getSocket() override
    {
        if (_transceiver && _transceiver->isValid())
        {
            return _transceiver->fd();
        }
        return -1;
    }

    virtual bool getLocal(std::string& host, int &port) override
    {
        if (!_transceiver || !_transceiver->isValid())
        {
            return false;
        }

        try
        {
            // 从 transceiver 获取本地地址信息
            const auto& serverAddr = _transceiver->getServerAddr();
            if (serverAddr.first)
            {
                char addrStr[INET6_ADDRSTRLEN] = {0};
                if (serverAddr.first->sa_family == AF_INET)
                {
                    struct sockaddr_in* sin = (struct sockaddr_in*)serverAddr.first.get();
                    inet_ntop(AF_INET, &sin->sin_addr, addrStr, sizeof(addrStr));
                    host = addrStr;
                    port = ntohs(sin->sin_port);
                    return true;
                }
                else if (serverAddr.first->sa_family == AF_INET6)
                {
                    struct sockaddr_in6* sin6 = (struct sockaddr_in6*)serverAddr.first.get();
                    inet_ntop(AF_INET6, &sin6->sin6_addr, addrStr, sizeof(addrStr));
                    host = addrStr;
                    port = ntohs(sin6->sin6_port);
                    return true;
                }
            }
            return false;
        }
        catch (...)
        {
            return false;
        }
    }

    virtual bool getRemote(std::string& host, int &port) override
    {
        if (!_transceiver || !_transceiver->isValid())
        {
            return false;
        }

        try
        {
            // 从 transceiver 获取客户端地址信息
            const auto& clientAddr = _transceiver->getClientAddr();
            if (clientAddr.first)
            {
                char addrStr[INET6_ADDRSTRLEN] = {0};
                if (clientAddr.first->sa_family == AF_INET)
                {
                    struct sockaddr_in* sin = (struct sockaddr_in*)clientAddr.first.get();
                    inet_ntop(AF_INET, &sin->sin_addr, addrStr, sizeof(addrStr));
                    host = addrStr;
                    port = ntohs(sin->sin_port);
                    return true;
                }
                else if (clientAddr.first->sa_family == AF_INET6)
                {
                    struct sockaddr_in6* sin6 = (struct sockaddr_in6*)clientAddr.first.get();
                    inet_ntop(AF_INET6, &sin6->sin6_addr, addrStr, sizeof(addrStr));
                    host = addrStr;
                    port = ntohs(sin6->sin6_port);
                    return true;
                }
            }
            // 如果没有客户端地址，尝试从 endpoint 获取
            const auto& ep = _transceiver->getEndpoint();
            host = ep.getHost();
            port = ep.getPort();
            return !host.empty();
        }
        catch (...)
        {
            return false;
        }
    }

    std::shared_ptr<tars::TC_Transceiver> getTransceiver() const
    {
        return _transceiver;
    }

private:
    std::shared_ptr<tars::TC_Transceiver> _transceiver;
    NetReceiverPtr _receiver;
    std::string _protocol;
    std::map<std::string, std::string> _params;
    std::atomic<bool> _closed;
};

// UDP 连接信息
struct UdpConnectionInfo
{
    NetSenderPtr sender;
    NetReceiverPtr receiver;
    std::chrono::steady_clock::time_point lastActiveTime;
    
    UdpConnectionInfo() = default;
    
    UdpConnectionInfo(NetSenderPtr s, NetReceiverPtr r)
        : sender(s), receiver(r), lastActiveTime(std::chrono::steady_clock::now())
    {
    }
};

// 待处理的连接任务（用于异步连接建立）
struct PendingConnection
{
    int clientFd;
    tars::TC_Endpoint ep;
    std::string protocol;
    int domain;
    NetReceiverPtr receiver;
    std::shared_ptr<tars::TC_Epoller> epoller;
    
    PendingConnection(int fd, const tars::TC_Endpoint& endpoint, const std::string& proto, 
                     int dom, const NetReceiverPtr& recv, std::shared_ptr<tars::TC_Epoller> poller)
        : clientFd(fd), ep(endpoint), protocol(proto), domain(dom), receiver(recv), epoller(poller)
    {
    }
};

// TarsNetDriver 实现
class TarsNetDriver : public NetDriver
{
public:
    TarsNetDriver(int maxFds, int polls = 1)
        : _maxFds(maxFds)
        , _polls(polls > 0 ? polls : 1) // 至少 1 个线程
        , _defaultIpv6(false)
        , _background(false)
        , _maxConnections(maxFds * 8 / 10) // 默认使用 80% 的 maxFds 作为最大连接数
        , _currentConnections(0)
        , _workerThreadsRunning(true)
    {
        // 创建多个 epoller（多线程事件循环）
        for (int i = 0; i < _polls; ++i)
        {
            auto epoller = std::make_shared<tars::TC_Epoller>();
            epoller->create(maxFds / _polls + 1, true);
            epoller->setName("TarsNetDriver-" + std::to_string(i));
            _epollers.push_back(epoller);
        }
        
        // 主 epoller（用于监听 socket）
        _epoller = _epollers[0];

        // 启动多个事件循环线程
        _running = true;
        for (int i = 0; i < _polls; ++i)
        {
            _eventThreads.emplace_back([this, i]() {
                while (_running)
                {
                    if (!g_netSuspend.load())
                    {
                        _epollers[i]->done(10); // 10ms timeout
                        // 只在第一个线程清理 UDP 连接缓存
                        if (i == 0)
                        {
                            cleanupUdpConnections();
                        }
                    }
                    else
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                }
            });
        }
        
        // 启动工作线程池（用于异步连接建立）
        int workerThreadCount = std::min(_polls * 2, 8); // 最多 8 个工作线程
        for (int i = 0; i < workerThreadCount; ++i)
        {
            _workerThreads.emplace_back([this]() {
                while (_workerThreadsRunning)
                {
                    std::unique_lock<std::mutex> lock(_pendingConnectionsMutex);
                    _pendingConnectionsCond.wait(lock, [this]() {
                        return !_pendingConnections.empty() || !_workerThreadsRunning;
                    });
                    
                    if (!_workerThreadsRunning && _pendingConnections.empty())
                    {
                        break;
                    }
                    
                    if (!_pendingConnections.empty())
                    {
                        auto task = std::move(_pendingConnections.front());
                        _pendingConnections.pop();
                        lock.unlock();
                        
                        // 处理连接建立
                        processPendingConnection(task);
                    }
                }
            });
        }
    }

    virtual ~TarsNetDriver()
    {
        shutdown();
    }
    
    // 根据 fd 选择 epoller（哈希分配）
    std::shared_ptr<tars::TC_Epoller> selectEpoller(int fd)
    {
        return _epollers[fd % _polls];
    }
    
    // 处理待处理的连接（异步连接建立）
    void processPendingConnection(const PendingConnection& task)
    {
        try
        {
            tars::TC_Socket clientSocket;
            clientSocket.init(task.clientFd, true, task.domain);
            clientSocket.setblock(false);
            try
            {
                clientSocket.setKeepAlive();
                clientSocket.setTcpNoDelay();
            }
            catch (...)
            {
            }
            
            // 为新连接创建 Transceiver
            std::shared_ptr<tars::TC_Transceiver> clientTransceiver;
            if (task.ep.isTcp())
            {
                clientTransceiver = std::make_shared<tars::TC_TCPTransceiver>(task.epoller.get(), task.ep);
            }
            else
            {
                clientTransceiver = std::make_shared<tars::TC_UDPTransceiver>(task.epoller.get(), task.ep);
            }
            auto clientEpollInfo = clientTransceiver->bindFd(task.clientFd);
            clientSocket.setOwner(false);
            
            // 创建 NetSender
            auto sender = new TarsNetSender(clientTransceiver, task.receiver, task.protocol);
            
            // 调用 recvConnection 创建新的接收器（如 GatewayConnection）
            NetReceiverPtr newReceiver = task.receiver->recvConnection(sender);
            if (!newReceiver)
            {
                newReceiver = task.receiver;
            }
            
            // 初始化服务端连接
            clientTransceiver->initializeServer(
                [newReceiver, this](tars::TC_Transceiver* trans, tars::TC_Transceiver::CloseReason reason, const std::string& err) {
                    _currentConnections--;
                    if (newReceiver)
                    {
                        newReceiver->onConnClose();
                    }
                },
                [newReceiver](tars::TC_Transceiver* trans) {
                    if (newReceiver)
                    {
                        newReceiver->onConnReady();
                    }
                },
                [newReceiver](tars::TC_NetWorkBuffer& buff, tars::TC_Transceiver* trans) {
                    // 解析数据包
                    if (!buff.empty())
                    {
                        auto data = buff.getBufferPointer();
                        if (data.first && data.second > 0 && newReceiver)
                        {
                            newReceiver->recv((const unsigned char*)data.first, data.second);
                        }
                    }
                    return tars::TC_NetWorkBuffer::PACKET_FULL;
                },
                [](tars::TC_Transceiver* trans) -> std::shared_ptr<tars::TC_OpenSSL> {
                    return nullptr;
                }
            );
            
            // 注册客户端连接的事件
            std::map<uint32_t, tars::TC_Epoller::EpollInfo::EVENT_CALLBACK> clientCallbacks;
            clientCallbacks[EPOLLIN] = [clientTransceiver](const std::shared_ptr<tars::TC_Epoller::EpollInfo>& info) {
                try
                {
                    clientTransceiver->doResponse();
                    return true;
                }
                catch (...)
                {
                    return false;
                }
            };
            clientCallbacks[EPOLLOUT] = [clientTransceiver](const std::shared_ptr<tars::TC_Epoller::EpollInfo>& info) {
                try
                {
                    clientTransceiver->doRequest();
                    return true;
                }
                catch (...)
                {
                    return false;
                }
            };
            clientEpollInfo->registerCallback(clientCallbacks, EPOLLIN | EPOLLOUT);
        }
        catch (...)
        {
            ::close(task.clientFd);
            _currentConnections--;
        }
    }

    // 清理过期的 UDP 连接（30 秒无活动）
    void cleanupUdpConnections()
    {
        static auto lastCleanup = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        
        // 每 5 秒清理一次
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastCleanup).count() < 5)
        {
            return;
        }
        lastCleanup = now;

        std::lock_guard<std::mutex> lock(_udpConnectionsMutex);
        const auto timeout = std::chrono::seconds(30);
        
        for (auto it = _udpConnections.begin(); it != _udpConnections.end();)
        {
            if (now - it->second.lastActiveTime > timeout)
            {
                it = _udpConnections.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // 生成 UDP 连接 key
    std::string makeUdpKey(const std::string& remoteHost, int remotePort)
    {
        return remoteHost + ":" + std::to_string(remotePort);
    }

    virtual void setBackground(bool background) override
    {
        _background = background;
    }

    virtual void shutdown() override
    {
        _running = false;
        
        // 停止工作线程池
        _workerThreadsRunning = false;
        _pendingConnectionsCond.notify_all();
        for (auto& thread : _workerThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
        
        // 停止事件循环线程
        for (auto& thread : _eventThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
        
        // 关闭所有 epoller
        for (auto& epoller : _epollers)
        {
            if (epoller)
            {
                epoller->close();
            }
        }
    }

    virtual void *watchDogCheck(int abortTimeout) override
    {
        // 简单实现，返回 nullptr 表示正常
        return nullptr;
    }

    virtual void setDefaultIpv6(bool enable) override
    {
        _defaultIpv6 = enable;
    }

    virtual bool getDefaultIpv6() override
    {
        return _defaultIpv6;
    }

    virtual void setNetworkChanged(bool reload, bool offline) override
    {
        // 网络状态变化处理
    }

    virtual NetSenderPtr listen(const std::string& protocol, 
                                const std::string& localHost, 
                                int localPort, 
                                const NetReceiverPtr& receiver, 
                                bool reuseAddr) override
    {
        try
        {
            // 创建监听 socket
            tars::TC_Socket listenSocket;
            bool isUdp = (protocol == "udp");
            int domain = _defaultIpv6 ? AF_INET6 : AF_INET;
            
            listenSocket.createSocket(isUdp ? SOCK_DGRAM : SOCK_STREAM, domain);
            
            if (reuseAddr)
            {
                int opt = 1;
                setsockopt(listenSocket.getfd(), SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
            }

            listenSocket.bind(localHost, localPort);
            
            if (!isUdp)
            {
                listenSocket.listen(1024);
                listenSocket.setblock(false);
            }

            // 创建 Endpoint
            tars::TC_Endpoint ep(localHost, localPort, 60000, isUdp ? tars::TC_Endpoint::UDP : tars::TC_Endpoint::TCP);
            
            // 创建监听 socket 的 epoll info
            auto listenEpollInfo = _epoller->createEpollInfo(listenSocket.getfd());
            listenSocket.setOwner(false); // EpollInfo 会管理 socket

            if (isUdp)
            {
                // UDP: 监听 socket 直接接收数据，使用连接复用机制
                // UDP 使用第一个 epoller
                auto udpTransceiver = std::make_shared<tars::TC_UDPTransceiver>(_epollers[0].get(), ep);
                auto udpEpollInfo = udpTransceiver->bindFd(listenSocket.getfd());
                
                udpTransceiver->initializeServer(
                    [receiver](tars::TC_Transceiver* trans, tars::TC_Transceiver::CloseReason reason, const std::string& err) {
                        // UDP 连接关闭
                    },
                    [receiver](tars::TC_Transceiver* trans) {
                        // UDP 连接就绪
                    },
                    [receiver, this, udpTransceiver](tars::TC_NetWorkBuffer& buff, tars::TC_Transceiver* trans) {
                        // UDP 数据包处理 - 使用连接复用
                        if (!buff.empty())
                        {
                            auto data = buff.getBufferPointer();
                            if (data.first && data.second > 0)
                            {
                                // 获取客户端地址
                                const auto& clientAddr = trans->getClientAddr();
                                if (clientAddr.first)
                                {
                                    std::string remoteHost;
                                    int remotePort = 0;
                                    
                                    // 解析客户端地址
                                    char addrStr[INET6_ADDRSTRLEN] = {0};
                                    if (clientAddr.first->sa_family == AF_INET)
                                    {
                                        struct sockaddr_in* sin = (struct sockaddr_in*)clientAddr.first.get();
                                        inet_ntop(AF_INET, &sin->sin_addr, addrStr, sizeof(addrStr));
                                        remoteHost = addrStr;
                                        remotePort = ntohs(sin->sin_port);
                                    }
                                    else if (clientAddr.first->sa_family == AF_INET6)
                                    {
                                        struct sockaddr_in6* sin6 = (struct sockaddr_in6*)clientAddr.first.get();
                                        inet_ntop(AF_INET6, &sin6->sin6_addr, addrStr, sizeof(addrStr));
                                        remoteHost = addrStr;
                                        remotePort = ntohs(sin6->sin6_port);
                                    }
                                    
                                    if (!remoteHost.empty())
                                    {
                                        std::string udpKey = makeUdpKey(remoteHost, remotePort);
                                        NetReceiverPtr targetReceiver = nullptr;
                                        
                                        // 查找或创建 UDP 连接
                                        {
                                            std::lock_guard<std::mutex> lock(_udpConnectionsMutex);
                                            auto it = _udpConnections.find(udpKey);
                                            if (it != _udpConnections.end())
                                            {
                                                // 复用现有连接
                                                it->second.lastActiveTime = std::chrono::steady_clock::now();
                                                targetReceiver = it->second.receiver;
                                            }
                                            else
                                            {
                                                // 创建新连接
                                                auto sender = new TarsNetSender(udpTransceiver, receiver, "udp");
                                                auto newReceiver = receiver->recvConnection(sender);
                                                if (newReceiver)
                                                {
                                                    _udpConnections[udpKey] = UdpConnectionInfo(sender, newReceiver);
                                                    targetReceiver = newReceiver;
                                                }
                                                else
                                                {
                                                    targetReceiver = receiver;
                                                }
                                            }
                                        }
                                        
                                        // 将数据传递给接收器
                                        if (targetReceiver)
                                        {
                                            targetReceiver->recv((const unsigned char*)data.first, data.second);
                                        }
                                    }
                                }
                            }
                        }
                        return tars::TC_NetWorkBuffer::PACKET_FULL;
                    },
                    [](tars::TC_Transceiver* trans) -> std::shared_ptr<tars::TC_OpenSSL> {
                        return nullptr;
                    }
                );

                std::map<uint32_t, tars::TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;
                callbacks[EPOLLIN] = [udpTransceiver](const std::shared_ptr<tars::TC_Epoller::EpollInfo>& info) {
                    try
                    {
                        udpTransceiver->doResponse();
                        return true;
                    }
                    catch (...)
                    {
                        return false;
                    }
                };
                udpEpollInfo->registerCallback(callbacks, EPOLLIN);

                return new TarsNetSender(udpTransceiver, receiver, protocol);
            }
            else
            {
                // TCP: 监听 socket 只用于 accept，每个 accept 的连接创建新的 Transceiver
                std::map<uint32_t, tars::TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;
                callbacks[EPOLLIN] = [this, receiver, ep, protocol, domain](const std::shared_ptr<tars::TC_Epoller::EpollInfo>& info) {
                    try
                    {
                        // Accept 新连接
                        tars::TC_Socket clientSocket;
                        int listenFd = info->fd();
                        
                        // 循环 accept 直到没有新连接（限制每次最多处理 100 个，避免阻塞）
                        int acceptCount = 0;
                        const int MAX_ACCEPT_PER_LOOP = 100;
                        
                        while (acceptCount < MAX_ACCEPT_PER_LOOP)
                        {
                            // 检查连接数限制
                            int current = _currentConnections.load();
                            if (current >= _maxConnections)
                            {
                                // 连接数已满，拒绝新连接
                                struct sockaddr_storage addr;
                                socklen_t addrLen = sizeof(addr);
                                int clientFd = ::accept(listenFd, (struct sockaddr*)&addr, &addrLen);
                                if (clientFd >= 0)
                                {
                                    ::close(clientFd); // 直接关闭
                                }
                                break;
                            }
                            
                            struct sockaddr_storage addr;
                            socklen_t addrLen = sizeof(addr);
                            int clientFd = ::accept(listenFd, (struct sockaddr*)&addr, &addrLen);
                            if (clientFd < 0)
                            {
                                if (errno == EAGAIN || errno == EWOULDBLOCK)
                                {
                                    break; // 没有更多连接
                                }
                                break;
                            }
                            
                            acceptCount++;
                            _currentConnections++;
                            
                            // 异步连接建立：将任务放入队列，由工作线程处理
                            auto selectedEpoller = selectEpoller(clientFd);
                            PendingConnection task(clientFd, ep, protocol, domain, receiver, selectedEpoller);
                            
                            {
                                std::lock_guard<std::mutex> lock(_pendingConnectionsMutex);
                                _pendingConnections.push(std::move(task));
                            }
                            _pendingConnectionsCond.notify_one();
                        }
                        
                        return true;
                    }
                    catch (...)
                    {
                        return false;
                    }
                };
                
                listenEpollInfo->registerCallback(callbacks, EPOLLIN);
                
                // 返回一个虚拟的 NetSender 用于监听 socket
                // 实际的数据收发通过 accept 后的连接进行
                return new TarsNetSender(nullptr, receiver, protocol);
            }
        }
        catch (...)
        {
            return nullptr;
        }
    }

    virtual NetSenderPtr connect(const std::string& protocol, 
                                 const std::string& localHost, 
                                 int localPort, 
                                 const std::string& remoteHost, 
                                 int remotePort, 
                                 const NetReceiverPtr& receiver, 
                                 bool reuseAddr) override
    {
        try
        {
            // 检查连接数限制（TCP 连接）
            bool isUdp = (protocol == "udp");
            if (!isUdp)
            {
                int current = _currentConnections.load();
                if (current >= _maxConnections)
                {
                    return nullptr; // 连接数已满
                }
                _currentConnections++;
            }
            
            // 创建 Endpoint
            tars::TC_Endpoint ep(remoteHost, remotePort, 60000, isUdp ? tars::TC_Endpoint::UDP : tars::TC_Endpoint::TCP);
            
            // 创建 Transceiver
            std::shared_ptr<tars::TC_Transceiver> transceiver;
            if (isUdp)
            {
                transceiver = std::make_shared<tars::TC_UDPTransceiver>(_epoller.get(), ep);
            }
            else
            {
                transceiver = std::make_shared<tars::TC_TCPTransceiver>(_epoller.get(), ep);
            }
            
            // 设置绑定地址
            if (!localHost.empty() && localPort > 0)
            {
                transceiver->setBindAddr(localHost.c_str());
            }

            // 初始化客户端
            bool isUdpLocal = isUdp; // 在 lambda 中捕获
            transceiver->initializeClient(
                    [](tars::TC_Transceiver* trans) -> std::shared_ptr<tars::TC_ProxyInfo> {
                    return nullptr;
                },
                [receiver, this, isUdpLocal](tars::TC_Transceiver* trans, tars::TC_Transceiver::CloseReason reason, const std::string& err) {
                    if (!isUdpLocal)
                    {
                        _currentConnections--;
                    }
                    if (receiver)
                    {
                        receiver->onConnClose();
                    }
                },
                [receiver](tars::TC_Transceiver* trans) {
                    if (receiver)
                    {
                        receiver->onConnReady();
                    }
                },
                [receiver](tars::TC_Transceiver* trans) {
                    if (receiver)
                    {
                        receiver->onSendReady();
                    }
                },
                [receiver](tars::TC_NetWorkBuffer& buff, tars::TC_Transceiver* trans) {
                    // 解析数据包 - 对于 UDP/TCP，直接返回完整包
                    if (!buff.empty())
                    {
                        auto data = buff.getBufferPointer();
                        if (data.first && data.second > 0 && receiver)
                        {
                            receiver->recv((const unsigned char*)data.first, data.second);
                        }
                    }
                    return tars::TC_NetWorkBuffer::PACKET_FULL;
                },
                [](tars::TC_Transceiver* trans) -> std::shared_ptr<tars::TC_OpenSSL> {
                    return nullptr;
                }
            );

            // 发起连接
            transceiver->connect();

            // 获取 epoll info 并注册事件
            auto epollInfo = transceiver->getEpollInfo();
            if (epollInfo)
            {
                std::map<uint32_t, tars::TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;
                callbacks[EPOLLIN] = [transceiver](const std::shared_ptr<tars::TC_Epoller::EpollInfo>& info) {
                    try
                    {
                        transceiver->doResponse();
                        return true;
                    }
                    catch (...)
                    {
                        return false;
                    }
                };
                callbacks[EPOLLOUT] = [transceiver](const std::shared_ptr<tars::TC_Epoller::EpollInfo>& info) {
                    try
                    {
                        transceiver->doRequest();
                        return true;
                    }
                    catch (...)
                    {
                        return false;
                    }
                };
                epollInfo->registerCallback(callbacks, EPOLLIN | EPOLLOUT);
            }

            return new TarsNetSender(transceiver, receiver, protocol);
        }
        catch (...)
        {
            bool isUdpLocal = (protocol == "udp");
            if (!isUdpLocal)
            {
                _currentConnections--;
            }
            return nullptr;
        }
    }

    // virtual NetStreamPtr stream(const StreamReceiverPtr& receiver, bool realtime, int64_t sendOffset, bool security) override
    // {
    //     // 暂不实现
    //     return nullptr;
    // }

    virtual NetSslPtr createNetSsl(const SslReceiverPtr& receiver, bool client, SslType type) override
    {
        // 暂不实现
        return nullptr;
    }

    virtual void setConfig(const std::string& name, const std::string& value) override
    {
        _configs[name] = value;
    }

    virtual std::string getConfig(const std::string& name) override
    {
        auto it = _configs.find(name);
        if (it != _configs.end())
        {
            return it->second;
        }
        return std::string();
    }

    virtual double sendAvgKbps(double *avgKbps, int *count) override
    {
        return 0.0;
    }

    virtual double recvAvgKbps(double *avgKbps, int *count) override
    {
        return 0.0;
    }

    virtual int sendAvgPkts(int *avgPkts, int *count) override
    {
        return 0;
    }

    virtual int recvAvgPkts(int *avgPkts, int *count) override
    {
        return 0;
    }

    virtual int64_t totalSendBytes(int64_t *totalBytes, int *count) override
    {
        return 0;
    }

    virtual int64_t totalRecvBytes(int64_t *totalBytes, int *count) override
    {
        return 0;
    }

    virtual int64_t totalSendPkts(int64_t *totalPkts, int *count) override
    {
        return 0;
    }

    virtual int64_t totalRecvPkts(int64_t *totalPkts, int *count) override
    {
        return 0;
    }

    virtual void totalReset() override
    {
    }

    virtual bool resolveHosts(const std::string& domain, 
                              std::vector<std::string>& hosts4, 
                              std::vector<std::string>& hosts6, 
                              bool& defaultIpv6, 
                              int timeout) override
    {
        defaultIpv6 = _defaultIpv6;
        // 简单的域名解析实现
        try
        {
            struct addrinfo hints, *result, *rp;
            memset(&hints, 0, sizeof(struct addrinfo));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            int ret = getaddrinfo(domain.c_str(), nullptr, &hints, &result);
            if (ret != 0)
            {
                return false;
            }

            for (rp = result; rp != nullptr; rp = rp->ai_next)
            {
                char host[NI_MAXHOST];
                if (getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST) == 0)
                {
                    if (rp->ai_family == AF_INET)
                    {
                        hosts4.push_back(host);
                    }
                    else if (rp->ai_family == AF_INET6)
                    {
                        hosts6.push_back(host);
                    }
                }
            }

            freeaddrinfo(result);
            return !hosts4.empty() || !hosts6.empty();
        }
        catch (...)
        {
            return false;
        }
    }

    virtual bool resolveHosts(const std::set<std::string> &domains, 
                              std::map<std::string, ResolveResult> &results, 
                              int timeout) override
    {
        bool allSuccess = true;
        for (const auto& domain : domains)
        {
            ResolveResult result;
            std::vector<std::string> hosts4, hosts6;
            bool defaultIpv6 = false;
            if (resolveHosts(domain, hosts4, hosts6, defaultIpv6, timeout))
            {
                result.defaultIpv6 = defaultIpv6;
                result.hosts4 = hosts4;
                result.hosts6 = hosts6;
                results[domain] = result;
            }
            else
            {
                allSuccess = false;
            }
        }
        return allSuccess;
    }

    // 获取当前连接数
    int getCurrentConnections() const
    {
        return _currentConnections.load();
    }

    // 获取最大连接数
    int getMaxConnections() const
    {
        return _maxConnections;
    }

    // 设置最大连接数
    void setMaxConnections(int maxConn)
    {
        _maxConnections = maxConn;
    }

private:
    int _maxFds;
    int _polls;
    bool _defaultIpv6;
    bool _background;
    std::shared_ptr<tars::TC_Epoller> _epoller; // 主 epoller（用于监听）
    std::vector<std::shared_ptr<tars::TC_Epoller>> _epollers; // 多个 epoller（多线程）
    std::vector<std::thread> _eventThreads; // 多个事件循环线程
    std::atomic<bool> _running;
    std::map<std::string, std::string> _configs;
    
    // 连接数限制
    int _maxConnections;
    std::atomic<int> _currentConnections;
    
    // UDP 连接复用
    std::map<std::string, UdpConnectionInfo> _udpConnections;
    std::mutex _udpConnectionsMutex;
    
    // 异步连接建立（线程池）
    std::queue<PendingConnection> _pendingConnections;
    std::mutex _pendingConnectionsMutex;
    std::condition_variable _pendingConnectionsCond;
    std::vector<std::thread> _workerThreads;
    std::atomic<bool> _workerThreadsRunning;
};

// NetDriver 静态方法实现
NetDriverPtr NetDriver::create(int maxFds, int polls)
{
    return new TarsNetDriver(maxFds, polls);
}

void NetDriver::setNetSuspend(bool suspend)
{
    g_netSuspend.store(suspend);
}

} // namespace Common

