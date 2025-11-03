#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#include "Common/inc/NetDriver.h"
#include "util/tc_common.h"
#include "util/tc_network_buffer.h"

using namespace tars;

static std::string makeFrame(const std::string& payload) {
	uint32_t be = ::htonl(4u + (uint32_t)payload.size()); // 总长=4+负载
	std::string out;
	out.resize(4 + payload.size());
	memcpy(&out[0], &be, 4);
	memcpy(&out[4], payload.data(), payload.size());
	return out;
}

static bool readExact(int fd, void* buf, size_t len) {
	char* p = static_cast<char*>(buf);
	size_t n = 0;
	while (n < len) {
		ssize_t r = ::read(fd, p + n, len - n);
		if (r <= 0) return false;
		n += (size_t)r;
	}
	return true;
}

class MyReceiver : public NetReceiver {
public:
	explicit MyReceiver(NetDriver* drv) : _drv(drv) {
		std::cout << ">>>>>>> [DEBUG] MyReceiver created" << std::endl;
	}

	void recvConnection(const ConnectionInfoPtr &conn) override {
		std::cout << "[recvConnection] remote=" << conn->ip << ":" << conn->port
		          << " proto=" << (conn->_isUdp ? "UDP" : "TCP")
		          << " uid=" << conn->uid
		          << " adapter=" << conn->adapterName
		          << std::endl;
	}

	void recv(const ConnectionInfoPtr &conn, const std::vector<char> &buffer) override {
		std::string payload(buffer.begin(), buffer.end());
		std::cout << "[recv] adapter=" << conn->adapterName
		          << " uid=" << conn->uid
		          << " proto=" << (conn->_isUdp ? "UDP" : "TCP")
		          << " bytes=" << buffer.size()
		          << " payload=\"" << payload << "\""
		          << std::endl;

		// 回显：echo 模式直接发送 payload，不添加长度头
		auto sender = _drv->makeSender(*conn);
		int ret = sender.send(payload.data(), payload.size(),
		                      conn->_isUdp ? conn->ip : std::string(),
		                      conn->_isUdp ? conn->port : 0);
		std::cout << "[echo] send ret=" << ret << std::endl;
	}

	void onConnClose(const ConnectionInfoPtr &conn, int closeType) override {
		std::cout << "[onConnClose] uid=" << conn->uid
		          << " proto=" << (conn->_isUdp ? "UDP" : "TCP")
		          << " closeType=" << closeType
		          << std::endl;
	}

private:
	NetDriver* _drv;
};

static void runTcpClient(uint16_t port) {
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) { perror("socket"); return; }
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	if (::connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("connect"); ::close(fd); return; }

	//uto f1 = makeFrame("tcp-hello-1");
	//auto f2 = makeFrame("tcp-hello-2");
    const char* p1 = "tcp-hello-1";
    const char* p2 = "tcp-hello-2";
    ::write(fd, p1, strlen(p1));
    ::write(fd, p2, strlen(p2));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    char buf[65536];
    ssize_t n = ::read(fd, buf, sizeof(buf));
    if (n > 0) {
        std::string s(buf, buf + n);
        std::cout << "[tcp-client] recv echo bytes=" << n << " data=\"" << s << "\"" << std::endl;
    }

    ::close(fd);
}

static void runUdpClient(uint16_t port) {
	int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) { perror("socket"); return; }
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	// auto sendFrame = [&](const std::string& p) {
	// 	std::string f = makeFrame(p);
	// 	ssize_t n = ::sendto(fd, f.data(), f.size(), 0, (sockaddr*)&addr, sizeof(addr));
	// 	std::cout << "[udp-client] send ret=" << n << " payload=" << p << std::endl;
	// };

	// sendFrame("udp-hello-1");
	// sendFrame("udp-hello-2");

    auto sendRaw = [&](const std::string& p) {
        ssize_t n = ::sendto(fd, p.data(), p.size(), 0, (sockaddr*)&addr, sizeof(addr));
        std::cout << "[udp-client] send ret=" << n << " payload=\"" << p << "\"" << std::endl;
    };

    sendRaw("udp-hello-1");
    sendRaw("udp-hello-2");
    // 接收两报文（每个 datagram 就是一批字节）
    for (int i = 0; i < 2; ++i) {
        char buf[65536];
        sockaddr_in from{};
        socklen_t flen = sizeof(from);
        ssize_t n = ::recvfrom(fd, buf, sizeof(buf), 0, (sockaddr*)&from, &flen);
        if (n > 0) {
            std::string s(buf, buf + n);
            std::cout << "[udp-client] recv echo bytes=" << n << " data=\"" << s << "\"" << std::endl;
        }
    }
	// // 接收两帧
	// for (int i = 0; i < 2; ++i) {
	// 	char buf[65536];
	// 	sockaddr_in from{};
	// 	socklen_t flen = sizeof(from);
	// 	ssize_t n = ::recvfrom(fd, buf, sizeof(buf), 0, (sockaddr*)&from, &flen);
	// 	if (n < 4) continue;
	// 	uint32_t beLen = 0;
	// 	memcpy(&beLen, buf, 4);
	// 	uint32_t len = ntohl(beLen);
	// 	if ((uint32_t)n < len || len < 4) continue;
	// 	std::string payload(buf + 4, buf + len);
	// 	std::cout << "[udp-client] recv payload=\"" << payload << "\"" << std::endl;
	// }

	::close(fd);
}

// 专门的客户端连接测试函数
static void testClientConnect() {
    // 创建独立的客户端 NetDriver
    NetDriver clientDrv(1);
    if (!clientDrv.initialize()) {
        std::cerr << "client initialize failed" << std::endl;
        return;
    }

    // 创建专门的客户端接收器
    class ClientReceiver : public NetReceiver {
    public:
        explicit ClientReceiver(NetDriver* drv) : _drv(drv) {
            std::cout << ">>>>>>> [CLIENT] ClientReceiver created" << std::endl;
        }

        void recvConnection(const ConnectionInfoPtr &conn) override {
            std::cout << "[CLIENT-recvConnection] remote=" << conn->ip << ":" << conn->port
                      << " proto=" << (conn->_isUdp ? "UDP" : "TCP")
                      << " uid=" << conn->uid
                      << " adapter=" << conn->adapterName
                      << std::endl;
        }

        void recv(const ConnectionInfoPtr &conn, const std::vector<char> &buffer) override {
            std::string payload(buffer.begin(), buffer.end());
            std::cout << "[CLIENT-recv] adapter=" << conn->adapterName
                      << " uid=" << conn->uid
                      << " proto=" << (conn->_isUdp ? "UDP" : "TCP")
                      << " bytes=" << buffer.size()
                      << " payload=\"" << payload << "\""
                      << std::endl;
        }

        void onConnClose(const ConnectionInfoPtr &conn, int closeType) override {
            std::cout << "[CLIENT-onConnClose] uid=" << conn->uid
                      << " proto=" << (conn->_isUdp ? "UDP" : "TCP")
                      << " closeType=" << closeType
                      << std::endl;
        }

    private:
        NetDriver* _drv;
    };

    auto clientReceiver = std::make_shared<ClientReceiver>(&clientDrv);

    // 测试 UDP 连接
    std::cout << "[CLIENT] Testing UDP connect..." << std::endl;
    NetSenderPtr udpSender = clientDrv.connect("udp", "", 0, "127.0.0.1", 18087, clientReceiver);
    if(!udpSender) {
        std::cout << "[CLIENT] UDP connect failed" << std::endl;
    } else {
        std::cout << "[CLIENT] UDP connect success, sending data..." << std::endl;
        int ret = udpSender->send("udp-hello-from-client", 21, "127.0.0.1", 18087);
        std::cout << "[CLIENT] UDP send ret=" << ret << std::endl;
    }

    // 测试 TCP 连接
    std::cout << "[CLIENT] Testing TCP connect..." << std::endl;
    NetSenderPtr tcpSender = clientDrv.connect("tcp", "", 0, "127.0.0.1", 18086, clientReceiver);
    if(!tcpSender) {
        std::cout << "[CLIENT] TCP connect failed" << std::endl;
    } else {
        std::cout << "[CLIENT] TCP connect success, sending data..." << std::endl;
        int ret = tcpSender->send("tcp-hello-from-client", 22);
        std::cout << "[CLIENT] TCP send ret=" << ret << std::endl;
    }

    // 等待接收响应
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    clientDrv.terminate();
    std::cout << "[CLIENT] Test completed" << std::endl;
}

int main() {
	NetDriver drv(1);
	if (!drv.initialize()) {
		std::cerr << "initialize failed" << std::endl;
		return 1;
	}

	auto receiver = std::make_shared<MyReceiver>(&drv);

	// 协议函数：4字节长度头（总长），返回包体
	// TC_NetWorkBuffer::protocol_functor pf = [](TC_NetWorkBuffer &in, std::vector<char> &out) -> TC_NetWorkBuffer::PACKET_TYPE {
	// 	return TC_NetWorkBuffer::parseBinary4<4, (4 + 8 * 1024 * 1024)>(in, out);
	// };

	// TCP 监听
	bool ok1 = drv.listen("tcpServer", "tcp -h 0.0.0.0 -p 18086", receiver, 2);
	// UDP 监听
	std::map<std::string, std::string> udpOpts;
	udpOpts["udpBufferSize"] = "65536";
	bool ok2 = drv.listen("udpServer", "udp -h 0.0.0.0 -p 18087", receiver, 1, TC_NetWorkBuffer::protocol_functor(), udpOpts);
    std::cout << "listen tcp=" << ok1 << " udp=" << ok2 << std::endl;
    
    // // 等待服务端启动完成
    // if (!drv.waitForServerReady()) {
    //     std::cerr << "[MAIN] Server failed to start in time" << std::endl;
    //     return 1;
    // }
    
    //std::cout << "[MAIN] Server is ready, starting client..." << std::endl;
    
    // 额外等待，确保网络线程真正开始监听
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
	std::thread t1(runTcpClient, 18086);
	//std::thread t2(runUdpClient, 18087);
	 // 启动客户端连接测试
    //std::thread clientThread(testClientConnect);
	t1.join();
	//t2.join();
    //clientThread.join();

	// 给服务端处理回显的时间
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	drv.terminate();
	return 0;
}
