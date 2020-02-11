#include "util/tc_epoll_server.h"
#include "util/tc_http.h"
#include "util/tc_logger.h"
#include "util/tc_thread_pool.h"
#include "util/tc_network_buffer.h"
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;
using namespace tars;

TC_LoggerThreadGroup g_group;
TC_RollLogger        g_logger;
TC_DayLogger         g_dlogger;

class MyServer;

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
		g_logger.debug() << "HttpHandle::initialize: " << std::this_thread::get_id() << endl;
		cout << "HttpHandle::initialize: " << std::this_thread::get_id() << endl;

	}

	/**
	 *
	 */
	virtual void handle(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		try
		{

			g_logger.debug() << "HttpHandle::handle : " << data->ip() << ":" << data->port() << endl;

			TC_HttpRequest request;

			request.decode(data->buffer().data(), data->buffer().size());


			TC_HttpResponse response;
			response.setResponse(200, "OK", "HttpServer");

			string buffer = response.encode();

			shared_ptr<TC_EpollServer::SendContext> send = data->createSendContext();
			send->buffer()->assign(buffer.c_str(), buffer.size());

			sendResponse(send);

		}
		catch (exception &ex)
		{
			g_logger.error() << "HttpHandle::handle ex:" << ex.what() << endl;
			close(data);
		}
	}

	/**
	 * [handleClose description]
	 * @param data [description]
	 */
	virtual void handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		try
		{

			g_logger.debug() << "HttpHandle::handleClose : " << data->ip() << ":" << data->port() << endl;

			string current = TC_Common::tostr(data->uid()) + "_" + data->ip() + "_" + TC_Common::tostr(data->port());


		}
		catch (exception &ex)
		{
			g_logger.error() << "HttpHandle::handle ex:" << ex.what() << endl;
			close(data);
		}
	}

	/**
	 * [heartbeat description]
	 */
	virtual void heartbeat()
	{
	}

protected:

};

TC_NetWorkBuffer::PACKET_TYPE parseEcho(TC_NetWorkBuffer&in, vector<char> &out)
{
	TC_EpollServer::Connection *c = (TC_EpollServer::Connection *)in.getConnection();
	cout << c->getIp() << endl;
    try
    {
        out = in.getBuffers();
        in.clearBuffers();
        return TC_NetWorkBuffer::PACKET_FULL;
    }
    catch (exception &ex)
    {
        return TC_NetWorkBuffer::PACKET_ERR;
    }

    return TC_NetWorkBuffer::PACKET_LESS;             //表示收到的包不完全
}


class SocketHandle : public TC_EpollServer::Handle
{
public:

	/**
	 * 初始化
	 */
	virtual void initialize()
	{
		g_logger.debug() << "SocketHandle::initialize: " << std::this_thread::get_id() << endl;
		cout << "SocketHandle::initialize: " << std::this_thread::get_id() << endl;
	}

	/**
	 *
	 */
	virtual void handle(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		try
		{
			cout << "SocketHandle::handle : " << data->ip() << ":" << data->port() << endl;

			shared_ptr<TC_EpollServer::SendContext> send = data->createSendContext();
			send->buffer()->setBuffer(data->buffer());
			sendResponse(send);

		}
		catch (exception &ex)
		{
			g_logger.error() << "SocketHandle::handle ex:" << ex.what() << endl;
			close(data);
		}
	}

	/**
	 * [handleClose description]
	 * @param data [description]
	 */
	virtual void handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
	{
		try
		{

			g_logger.debug() << "SocketHandle::handleClose : " << data->ip() << ":" << data->port();
		}
		catch (exception &ex)
		{
			g_logger.error() << "SocketHandle::handle ex:" << ex.what() << endl;
			close(data);
		}
	}

	/**
	 * [heartbeat description]
	 */
	virtual void heartbeat()
	{
	}

protected:

};


class MyServer
{
public:
	MyServer()
	{
		_epollServer = new TC_EpollServer(1);
	};

	void initialize()
	{
		cout << "initialize ok" << endl;
		g_group.start(1);

		g_logger.init("./debug", 1024 * 1024, 10);
		g_logger.modFlag(TC_RollLogger::HAS_LEVEL | TC_RollLogger::HAS_PID, true);
		g_logger.setLogLevel(5);
		g_logger.setupThread(&g_group);

		_epollServer->setLocalLogger(&g_logger);
	}

	void bindHttp(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(_epollServer);

		//设置adapter名称, 唯一
		lsPtr->setName("HttpAdapter");
		//设置绑定端口
		lsPtr->setEndpoint(str);
		//设置最大连接数
		lsPtr->setMaxConns(1024);
		//设置启动线程数
		lsPtr->setHandle<HttpHandle>(5);
		//设置协议解析器
		lsPtr->setProtocol(TC_NetWorkBuffer::parseHttp);
		//设置逻辑处理器
		g_logger.debug() << "HttpAdapter::setHandle ok" << endl;

		//绑定对象
		_epollServer->bind(lsPtr);

		g_logger.debug() << "HttpAdapter::bind ok" << endl;
	}

	void bindSocket(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(_epollServer);

		//设置adapter名称, 唯一
		lsPtr->setName("SocketAdapter");
		//设置绑定端口
		lsPtr->setEndpoint(str);
		//设置最大连接数
		lsPtr->setMaxConns(10240);
		//设置启动线程数
		lsPtr->setHandle<SocketHandle>(1);
		//设置协议解析器
		lsPtr->setProtocol(parseEcho);
//		lsPtr->enableQueueMode();

		g_logger.debug() << "SocketAdapter::SocketHandle ok" << endl;

		//绑定对象
		_epollServer->bind(lsPtr);


		g_logger.debug() << "SocketAdapter::bind ok" << endl;
	}

	void waitForShutdown()
	{
		_epollServer->waitForShutdown();
	}

protected:
	TC_EpollServer *_epollServer;
};

int main(int argc, char** argv)
{
	try
	{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS		
		TC_Common::ignorePipe();
#endif
		MyServer server;

		server.initialize();

		server.bindHttp("tcp -h 0.0.0.0 -p 8083 -t 60000");
		server.bindSocket("tcp -h 0.0.0.0 -p 8084 -t 60000");

		server.waitForShutdown();

	}
	catch (exception &ex)
	{
		cerr << "HttpServer::run ex:" << ex.what() << endl;
	}

	cout << "HttpServer::run http server thread exit." << endl;
}