#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "util/tc_epoll_server.h"
#include "util/tc_network_buffer.h"

using namespace tars;

struct ConnectionInfo {
    std::string ip;
    uint16_t port = 0;
    uint32_t uid = 0;       // connection id from TC_EpollServer
    int threadIndex = 0;    // netthread index
    std::string adapterName;
    // 内部使用,上层不需要关心
    bool _isUdp = false;
};

using ConnectionInfoPtr = std::shared_ptr<ConnectionInfo>;

// 上层接口,不区分TCP/UDP
struct NetReceiver {
    virtual ~NetReceiver() = default;
    
    // 新连接建立时回调(TCP有效)或首次收到数据(UDP)
    virtual void recvConnection(const ConnectionInfoPtr &conn) = 0;

    // 收到数据回调,buffer已经按协议封装好
    virtual void recv(const ConnectionInfoPtr &conn, const std::vector<char> &buffer) = 0;

    // 连接关闭回调(TCP有效)
    virtual void onConnClose(const ConnectionInfoPtr &conn, int closeType) = 0;
};
using NetReceiverPtr = std::shared_ptr<NetReceiver>;

// 发送器,自动处理TCP/UDP区别
class NetSender {
public:
    // 构造时确定连接类型，并传入对应的 BindAdapter（可为 nullptr）
    NetSender(TC_EpollServer* ep, uint32_t uid, int threadIndex,
              const TC_EpollServer::BindAdapterPtr &adapter = nullptr);

    // 统一的发送接口,内部区分TCP/UDP
    // 对于UDP,如果需要发给不同目标,可以传入目标ip和port
    // 对于TCP,则忽略ip和port参数
    int send(const char* data, size_t len, 
             const std::string &ip = "", uint16_t port = 0);
    int send(const std::vector<char> &buf,
             const std::string &ip = "", uint16_t port = 0) {
        return send(buf.data(), buf.size(), ip, port);
    }

    // 关闭连接
    void close();

    // 获取连接类型(内部使用)
    bool isUdp() const { return _isUdp; }

private:
    tars::TC_EpollServer* _ep = nullptr;
    uint32_t _uid = 0;
    int _threadIndex = 0;
    bool _isUdp = false;

    // 保持 adapter 的弱引用，避免延长 adapter 生命周期
    std::weak_ptr<TC_EpollServer::BindAdapter> _adapter;

    // 目标地址（用于客户端连接）
    std::string _targetIp;
    uint16_t _targetPort = 0;

    // UDP模式下记住最后的目标地址
    std::string _lastIp;
    uint16_t _lastPort = 0;
};
using NetSenderPtr = std::shared_ptr<NetSender>;

class NetDriver;
using NetDriverPtr = std::shared_ptr<NetDriver>;
// 驱动类,管理适配器和路由
class NetDriver
{
public:
    // 创建指定网络线程数的驱动
    NetDriver(unsigned int netThreads = 1);
    ~NetDriver();

    // 初始化底层TC_EpollServer
    bool initialize();

    // 统一的监听接口,内部根据endpoint判断TCP/UDP
    // name: 适配器名(唯一)
    // endpoint: 如 "tcp -h 0.0.0.0 -p 18086" 或 "udp -h 0.0.0.0 -p 18087"
    // receiver: 处理回调对象
    // handleNum: 处理线程数
    // pf: 可选的协议解析器
    // options: 可选配置参数(如缓冲区大小等)
    bool listen(const std::string &name, 
               const std::string &endpoint,
               const NetReceiverPtr &receiver,
               size_t handleNum = 1,
               const TC_NetWorkBuffer::protocol_functor &pf = TC_NetWorkBuffer::protocol_functor(),
               const std::map<std::string, std::string>& options = {});
    
    bool listen(const std::string &protocol,
                const std::string &localHost,
                int localPort,
                const NetReceiverPtr &receiver,
                bool reuseAddr = false);
    
    // 主动创建连接接口
    NetSenderPtr connect(const std::string &protocol,
                        const std::string &localHost,
                        int localPort,
                        const std::string &remoteHost,
                        int remotePort,
                        const NetReceiverPtr &receiver,
                        bool reuseAddr = false);

    void terminate();

    // 创建发送器
    NetSender makeSender(const ConnectionInfo &conn);
    // 等待服务端启动完成
    bool waitForServerReady();

private:
    // 内部转发Handle
    class ForwardHandle : public TC_EpollServer::Handle {
    public:
        ForwardHandle(const NetReceiverPtr &r, NetDriver* driver) : _receiver(r), _driver(driver) {}
        virtual void initialize() override {}
    protected:
        virtual void handle(const std::shared_ptr<TC_EpollServer::RecvContext> &data) override;
        virtual void handleClose(const std::shared_ptr<TC_EpollServer::RecvContext> &data) override;
    private:
        NetReceiverPtr _receiver;
        NetDriver* _driver;
    };

    // 适配器名到接收器的映射
    std::unordered_map<std::string, NetReceiverPtr> _adapters;
    // 连接到首次通信地址的映射(UDP)
    std::unordered_map<uint32_t, std::pair<std::string, uint16_t>> _udpEndpoints;

    std::unique_ptr<TC_EpollServer> _epoll;
    unsigned int _netThreads = 1;
 // 标志：是否已经初始化并启动了 TC_EpollServer 的 handle/thread（防止重复启动）
    bool _started = false;
    // Whether a deferred start has been scheduled
    bool _startScheduled = false;
    // mutex to protect start/schedule flags
    std::mutex _startMutex;
    void onAccept(TC_EpollServer::Connection* cPtr);

     // 客户端连接专用的适配器
    std::shared_ptr<tars::TC_EpollServer::BindAdapter> _clientAdapter;
    
    // 客户端连接映射 (uid -> receiver)
    std::unordered_map<uint32_t, NetReceiverPtr> _clientConnections;
    
    // 保护共享数据的互斥锁
    std::mutex _mutex;
    // 初始化客户端适配器
    bool initClientAdapter();
    
    // TCP连接实现
    NetSenderPtr connectTcp(const std::string &localHost, int localPort,
                           const std::string &remoteHost, int remotePort,
                           const NetReceiverPtr &receiver, bool reuseAddr);
    
    // UDP连接实现  
    NetSenderPtr connectUdp(const std::string &localHost, int localPort,
                           const std::string &remoteHost, int remotePort,
                           const NetReceiverPtr &receiver, bool reuseAddr);
    
    // 等待epoll启动
    bool waitForEpollStart();
};

