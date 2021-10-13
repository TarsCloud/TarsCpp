//
// Created by jarod on 2021/6/7.
//

#ifndef TARS_CPP_TEST_SERVER_H
#define TARS_CPP_TEST_SERVER_H

#include "util/tc_network_buffer.h"
#include "util/tc_epoll_server.h"
#include "util/tc_openssl.h"
#include "certs.h"

using namespace tars;

class MyTcpServer;

static TC_NetWorkBuffer::PACKET_TYPE parseLine(TC_NetWorkBuffer&in, vector<char> &out)
{
	try
	{
		if(in.empty())
		{
			return TC_NetWorkBuffer::PACKET_LESS;
		}

		const char *data = in.mergeBuffers();
		const char *p = TC_Port::strnstr(data, "\r\n", in.getBufferLength());

		if(NULL == p)
		{
			return TC_NetWorkBuffer::PACKET_LESS;
		}

		size_t length = p - data;

		out.resize(length+2);

		memcpy(&out[0], data, length+2);

		in.moveHeader(length + 2);

		return TC_NetWorkBuffer::PACKET_FULL;
	}
	catch (exception &ex)
	{
		return TC_NetWorkBuffer::PACKET_ERR;
	}

	return TC_NetWorkBuffer::PACKET_LESS;             //表示收到的包不完全
}

/**
 * 处理类, 每个处理线程一个对象
 */
class TcpHandle : public TC_EpollServer::Handle
{
public:

	/**
	 * 初始化
	 */
	virtual void initialize()
	{
		_server = (MyTcpServer*)(getEpollServer());
	}

	/**
	 *
	 */
	virtual void handle(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		try
		{
			shared_ptr<TC_EpollServer::SendContext> send = data->createSendContext();

//			 LOG_CONSOLE_DEBUG << TC_Thread::CURRENT_THREADID() << ", handle:" << string(data->buffer().data(), data->buffer().size()) << endl;

			send->buffer()->setBuffer(data->buffer());
			sendResponse(send);
		}
		catch (exception &ex)
		{
			close(data);
		}
	}

	virtual void handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		// LOG_CONSOLE_DEBUG << "use count:" << data.use_count() << endl;
	}

protected:

	MyTcpServer  *_server;
};


/**
 * 处理类, 每个处理线程一个对象
 */
class TcpQueueHandle : public TC_EpollServer::Handle
{
public:

	/**
	 * 初始化
	 */
	virtual void initialize()
	{
		_server = (MyTcpServer*)(getEpollServer());
	}

	/**
	 *
	 */
	virtual void handle(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		try
		{
			shared_ptr<TC_EpollServer::SendContext> send = data->createSendContext();

			string buff(data->buffer().data(), data->buffer().size());

			buff += "-" + TC_Common::tostr(TC_Thread::CURRENT_THREADID());

			send->buffer()->setBuffer(buff);

			sendResponse(send);
		}
		catch (exception &ex)
		{
			close(data);
		}
	}

protected:

	MyTcpServer  *_server;
};

class MyTcpServer
{
public:
	MyTcpServer()
	{
		_epollServer = new TC_EpollServer();
	};

	void initialize()
	{
	}
#if TARS_SSL
	void bindSSL(const std::string &str, int maxConnections = 10240)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<TcpHandle>("SSLAdapter", str, 5);

		//设置最大连接数
		lsPtr->setMaxConns(maxConnections);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseEcho);

		bool verifyClient = false;
		string ciphers = "";

		shared_ptr<TC_OpenSSL::CTX> ctx = TC_OpenSSL::newCtx(CA, SERVER_CERT, SERVER_KEY, verifyClient, ciphers);

		assert(ctx);

		lsPtr->setSSLCtx(ctx);

		//绑定对象
		_epollServer->bind(lsPtr);
	}
#endif

	void bindUdp(const std::string &str, int maxConnections = 10240)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<TcpHandle>("UdpAdapter", str, 2);

		//设置最大连接数
		lsPtr->setMaxConns(maxConnections);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseEcho);
		//绑定对象
		_epollServer->bind(lsPtr);
	}

	void bindManualTcp(const std::string &str, int maxConnections = 10240)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<TcpHandle>("TcpManualAdapter", str, 5);

		//设置最大连接数
		lsPtr->setMaxConns(maxConnections);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseEcho);
		lsPtr->enableManualListen();
		//绑定对象
		_epollServer->bind(lsPtr);
	}

	void startManualListen()
	{
		_epollServer->getBindAdapter("TcpManualAdapter")->manualListen();
	}

	void cancelManualListen()
	{
		_epollServer->getBindAdapter("TcpManualAdapter")->cancelListen();
	}

	void bindTcp(const std::string &str, int maxConnections = 10240)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<TcpHandle>("TcpAdapter", str, 5);

		//设置最大连接数
		lsPtr->setMaxConns(maxConnections);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseEcho);
		//绑定对象
		_epollServer->bind(lsPtr);
	}

	void bindTcpLine(const std::string &str, int maxConnections = 10240)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<TcpHandle>("TcpLineAdapter", str, 5);

		//设置最大连接数
		lsPtr->setMaxConns(maxConnections);
		//设置协议解析器
		lsPtr->setProtocol(parseLine);
		//绑定对象
		_epollServer->bind(lsPtr);
	}

	void bindTcpQueue(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<TcpQueueHandle>("TcpQueueAdapter", str, 5);

		lsPtr->enableQueueMode();

		//设置最大连接数
		lsPtr->setMaxConns(10240);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseEcho);
		//绑定对象
		_epollServer->bind(lsPtr);
	}

	void waitForShutdown()
	{
		_start = new std::thread([=]{
			_epollServer->waitForShutdown();
		});

		TC_Common::msleep(100);
	}

	void terminate()
	{
		_epollServer->terminate();
		_start->join();
		delete _start;
		delete _epollServer;
	}

	std::thread *_start;
	TC_EpollServer *_epollServer;
};


class MyHttpServer;

/**
 * 处理类, 每个处理线程一个对象
 */
class HttpHandle : public TC_EpollServer::Handle
{
public:

	/**
	 * 初始化
	 */
	virtual void initialize()
	{
		_server = (MyHttpServer*)(getEpollServer());
	}

	/**
	 *
	 */
	virtual void handle(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		try
		{

			TC_HttpRequest request;

			request.decode(data->buffer().data(), data->buffer().size());

			// cout << "-------------------------------------------------------" << endl;
			// cout << "request: " << string(data->buffer().data(), data->buffer().size()) << endl;
			// cout << "-------------------------------------------------------" << endl;

			TC_HttpResponse response;
			response.setConnection("close");
			response.setResponse(200, "OK", request.getContent());

			string buffer = response.encode();

			shared_ptr<TC_EpollServer::SendContext> send = data->createSendContext();
			send->buffer()->setBuffer(buffer.c_str(), buffer.size());

			sendResponse(send);

		}
		catch (exception &ex)
		{
			close(data);
		}
	}

protected:

	MyHttpServer  *_server;
};


class MyHttpServer
{
public:
	MyHttpServer()
	{
		_epollServer = new TC_EpollServer();
	};

	void initialize()
	{
	}

	void bindHttp(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<HttpHandle>("HttpAdapter", str, 5); //new TC_EpollServer::BindAdapter(_epollServer);

		//设置最大连接数
		lsPtr->setMaxConns(1024);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseHttp);
		//绑定对象
		_epollServer->bind(lsPtr);
	}
#if TARS_SSL
	void bindHttpSSL(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<HttpHandle>("HttpsAdapter", str, 5);

		//设置最大连接数
		lsPtr->setMaxConns(1024);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseHttp);

		bool verifyClient = false;
		string ciphers = "";

		shared_ptr<TC_OpenSSL::CTX> ctx = TC_OpenSSL::newCtx(CA, SERVER_CERT, SERVER_KEY, verifyClient, ciphers);

		lsPtr->setSSLCtx(ctx);

		//绑定对象
		_epollServer->bind(lsPtr);
	}

	void bindHttpSSLVerify(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<HttpHandle>("HttpsVerifyAdapter", str, 5);

		//设置最大连接数
		lsPtr->setMaxConns(1024);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseHttp);

		bool verifyClient = true;
		string ciphers = "";

		shared_ptr<TC_OpenSSL::CTX> ctx = TC_OpenSSL::newCtx(CLIENT_CERT, SERVER_CERT, SERVER_KEY, verifyClient, ciphers);

		lsPtr->setSSLCtx(ctx);

		//绑定对象
		_epollServer->bind(lsPtr);
	}
#endif

	void waitForShutdown()
	{
		_start = new std::thread([=]{
			_epollServer->waitForShutdown();
		});
	}

	void terminate()
	{
		_epollServer->terminate();
		_start->join();
		delete _start;
	}

	std::thread *_start;
	TC_EpollServer *_epollServer;
};

#endif //TARS_CPP_TEST_SERVER_H
