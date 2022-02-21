#include "util/tc_common.h"
#include <thread>
#include "gtest/gtest.h"
#include "test_server.h"

using namespace tars;

static TC_Endpoint TEST_HOST_EP("tcp -h 127.0.0.1 -p 19089 -t 10000");
static TC_Endpoint LINE_HOST_EP("tcp -h 127.0.0.1 -p 19099 -t 10000");
static TC_Endpoint QUEUE_HOST_EP("tcp -h 127.0.0.1 -p 19019 -t 10000");
static TC_Endpoint UDP_HOST_EP("udp -h 127.0.0.1 -p 18085 -t 10000");

class UtilEpollServerTest : public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{
	}
	static void TearDownTestCase()
	{
	}
	virtual void SetUp()
	{
	}
	virtual void TearDown() //TEST跑完之后会执行TearDown
	{
	}

	void startServer(MyTcpServer &server, TC_EpollServer::SERVER_OPEN_COROUTINE openCoroutine)
	{
		server.initialize();

		server._epollServer->setOpenCoroutine(openCoroutine);

		server.bindUdp(UDP_HOST_EP.toString());
		server.bindTcp(TEST_HOST_EP.toString(), 10);
		server.bindTcpLine(LINE_HOST_EP.toString(), 10);
		server.bindTcpQueue(QUEUE_HOST_EP.toString());

		server.waitForShutdown();
	}

	void startManualServer(MyTcpServer &server, TC_EpollServer::SERVER_OPEN_COROUTINE openCoroutine)
	{
		server.initialize();

		server._epollServer->setOpenCoroutine(openCoroutine);

		server.bindManualTcp(TEST_HOST_EP.toString(), 10);

		server.waitForShutdown();
	}

	void stopServer(MyTcpServer &server)
	{
		server.terminate();
	}
protected:


};


TEST_F(UtilEpollServerTest, RunUdp)
{
	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		MyTcpServer server;
		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

		TC_UDPClient client(UDP_HOST_EP.getHost(), UDP_HOST_EP.getPort(), UDP_HOST_EP.getTimeout());
		int iRet = 0;
		char recvBuffer[1024];
		size_t recvLenth = 1024;

		for (int i = 0; i < 10; i++)
		{
			iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);

			ASSERT_TRUE(iRet == 0);
			ASSERT_TRUE(string(recvBuffer, recvLenth) == "abc");
		}

		stopServer(server);
	}
}

TEST_F(UtilEpollServerTest, RunTcp)
{
	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		MyTcpServer server;
		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

		TC_TCPClient client(TEST_HOST_EP.getHost(), TEST_HOST_EP.getPort(), TEST_HOST_EP.getTimeout());
		int iRet = 0;
		char recvBuffer[1024];
		size_t recvLenth = 1024;

		for (int i = 0; i < 10; i++) {
			iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);

			ASSERT_TRUE(iRet == 0);
			ASSERT_TRUE(string(recvBuffer, recvLenth) == "abc");
		}

		stopServer(server);
	}
}

TEST_F(UtilEpollServerTest, RunEnableManualListen)
{
//	int i = 0;
	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		MyTcpServer server;
		startManualServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

		TC_TCPClient client(TEST_HOST_EP.getHost(), TEST_HOST_EP.getPort(), TEST_HOST_EP.getTimeout());
		int iRet = 0;
		char recvBuffer[1024];
		size_t recvLenth = 1024;

		iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);
		ASSERT_TRUE(iRet == -6);

		server.startManualListen();
		iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);
		ASSERT_TRUE(iRet == 0);
		ASSERT_TRUE(string(recvBuffer, recvLenth) == "abc");

		server.cancelManualListen();
		iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);
		ASSERT_TRUE(iRet != TC_ClientSocket::EM_SUCCESS);

		server.startManualListen();
		iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);
		ASSERT_TRUE(iRet == 0);
		ASSERT_TRUE(string(recvBuffer, recvLenth) == "abc");

		stopServer(server);
	}
}

TEST_F(UtilEpollServerTest, RunLine)
{
	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		MyTcpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) i);

		TC_TCPClient client(LINE_HOST_EP.getHost(), LINE_HOST_EP.getPort(), LINE_HOST_EP.getTimeout());

		int iRet = 0;

		string sendBuffer(1024 * 1024 * 2, 'a');
		string recvBuffer;

		iRet = client.sendRecvLine((sendBuffer + "\r\n").c_str(), sendBuffer.size() + 2, recvBuffer);

		ASSERT_TRUE(iRet == 0);
		ASSERT_TRUE(recvBuffer == sendBuffer + "\r\n");
		stopServer(server);
	}
}

TEST_F(UtilEpollServerTest, AcceptCallback)
{

	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		MyTcpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

		bool accept = false;
		server._epollServer->setOnAccept([&](TC_EpollServer::Connection *conn)
		                                  {
			                                  accept = true;
		                                  });

		TC_TCPClient client(TEST_HOST_EP.getHost(), TEST_HOST_EP.getPort(), TEST_HOST_EP.getTimeout());
		int iRet = 0;
		char recvBuffer[1024];
		size_t recvLenth = 1024;

		iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);

		ASSERT_TRUE(iRet == 0);
		ASSERT_TRUE(string(recvBuffer, recvLenth) == "abc");

		TC_Common::msleep(50);
		ASSERT_TRUE(accept);

		stopServer(server);
	}
}

TEST_F(UtilEpollServerTest, ConnectionMax)
{
	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		MyTcpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) i);

		vector<TC_TCPClient *> conns;

		//注意服务连接数是10个
		for (int i = 0; i < 10; i++) {
			TC_TCPClient
				*client = new TC_TCPClient(TEST_HOST_EP.getHost(), TEST_HOST_EP.getPort(), TEST_HOST_EP.getTimeout());
			int iRet = 0;
			char recvBuffer[1024];
			size_t recvLenth = 1024;

			iRet = client->sendRecv("abc", 3, recvBuffer, recvLenth);

			ASSERT_TRUE(iRet == 0);
			ASSERT_TRUE(string(recvBuffer, recvLenth) == "abc");

			conns.push_back(client);
		}
		TC_TCPClient client(TEST_HOST_EP.getHost(), TEST_HOST_EP.getPort(), TEST_HOST_EP.getTimeout());
		int iRet = 0;
		char recvBuffer[1024];
		size_t recvLenth = 1024;

		iRet = client.sendRecv("abc", 3, recvBuffer, recvLenth);

		ASSERT_TRUE(iRet != 0);

		for (size_t i = 0; i < conns.size(); i++) {
			delete conns[i];
		}
		stopServer(server);
	}
}

TEST_F(UtilEpollServerTest, QueueMode)
{
	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		MyTcpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) i);

		vector<TC_TCPClient *> conns;

		//注意服务连接数是10个
		for (int i = 0; i < 10; i++) {
			TC_TCPClient *client =
				new TC_TCPClient(QUEUE_HOST_EP.getHost(), QUEUE_HOST_EP.getPort(), QUEUE_HOST_EP.getTimeout());

			conns.push_back(client);
		}

		for (auto client : conns) {
			string lastBuff;

			for (int i = 0; i < 10; i++) {
				int iRet = 0;
				char recvBuffer[1024] = {0};
				size_t recvLenth = 1024;

				iRet = client->sendRecv("abc", 3, recvBuffer, recvLenth);

				ASSERT_TRUE(iRet == 0);

				if (!lastBuff.empty()) {
					ASSERT_TRUE(lastBuff == string(recvBuffer, recvLenth));
				}
				lastBuff = string(recvBuffer, recvLenth);
			}
		}

		for (size_t i = 0; i < conns.size(); i++) {
			delete conns[i];
		}

		stopServer(server);
	}
}