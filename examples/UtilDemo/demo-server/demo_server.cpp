#include "util/tc_epoll_server.h"
#include "util/tc_http.h"
#include "util/tc_option.h"
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
			TC_HttpRequest request;

			request.decode(data->buffer().data(), data->buffer().size());

//			cout << string(data->buffer().data(), data->buffer().size()) << endl;
//			g_logger.debug() << "HttpHandle::handle : " << data->ip() << ":" << data->port() << ", " << request.getContent() << endl;
//            cout << "HttpHandle::handle : " << data->ip() << ":" << data->port() << endl;
//            cout << request.getContent() << endl;

			TC_HttpResponse response;
			response.setResponse(200, "OK", request.getContent());

			string buffer = response.encode();

			shared_ptr<TC_EpollServer::SendContext> send = data->createSendContext();
			send->buffer()->addBuffer(buffer.c_str(), buffer.size());

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


// static int g_count = 0;

TC_NetWorkBuffer::PACKET_TYPE parseEcho(TC_NetWorkBuffer&in, vector<char> &out)
{

//	DEBUG_COST("parseEcho1");

//	TC_EpollServer::Connection *c = (TC_EpollServer::Connection *)in.getConnection();
	// cout << c->getIp() << endl;
    try
    {
        out = in.getBuffers();
        in.clearBuffers();
	    // DEBUG_COST("parseEcho");

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

	int _count = 0;
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
			// DEBUG_COST("handle");

//			cout << "SocketHandle::handle : " << data->ip() << ":" << data->port() << ", " << data->buffer().data() << endl;
			shared_ptr<TC_EpollServer::SendContext> send = data->createSendContext();
			send->buffer()->setBuffer(data->buffer().data(), data->buffer().size());
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
			g_logger.debug() << "SocketHandle::handleClose : " << data->ip() << ":" << data->port() << endl;
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


class MyServer : public TC_EpollServer
{
public:
	MyServer()
	{
	}

	TC_EpollServer* getEpollServer() { return this; }

	void initialize()
	{
		g_group.start(1);

		g_logger.init("./debug", 1024 * 1024, 10);
		g_logger.modFlag(TC_RollLogger::HAS_LEVEL | TC_RollLogger::HAS_PID, true);
		g_logger.setLogLevel(5);
		g_logger.setupThread(&g_group);

		setLocalLogger(&g_logger);

		TC_EpollServer *epollServer = this;

		TC_Port::registerCtrlC([=]{
			epollServer->terminate();
		});
    	
	}

	void bindHttp(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr adapter = this->createBindAdapter<HttpHandle>("http", str, 1);
		adapter->setMaxConns(100000);
		adapter->setProtocol(TC_NetWorkBuffer::parseHttp);

		//绑定对象
		bind(adapter);

		g_logger.debug() << "HttpAdapter::setHandle ok" << endl;
	}

	void bindSocket(const std::string &str)
	{
		TC_EpollServer::BindAdapterPtr adapter = this->createBindAdapter<SocketHandle>(str, str, 5);

		adapter->setMaxConns(10240);
		adapter->setProtocol(parseEcho);

		adapter->enableQueueMode();
		adapter->setQueueCapacity(1000000);

		g_logger.debug() << "SocketAdapter::SocketHandle ok" << endl;

		//绑定对象
		bind(adapter);

		g_logger.debug() << "SocketAdapter::bind ok" << endl;
	}
};

int main(int argc, char** argv)
{
	try
	{
		if(argc < 2)
		{
			cout << argv[0] << " --pattern=[0-3]" << endl;
			cout << "pattern 0: NET_THREAD_AND_HANDLES_THREAD" << endl;
			cout << "pattern 1: NET_THREAD_AND_HANDLES_CO" << endl;
			cout << "pattern 2: NET_HANDLES_MERGE_THREAD" << endl;
			cout << "pattern 3: NET_HANDLES_MERGE_CO" << endl;
			exit(0);
		}
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS		
		TC_Common::ignorePipe();
#endif
		TC_Option option;
		option.decode(argc, argv);

		TC_EpollServer::SERVER_OPEN_COROUTINE pattern = (TC_EpollServer::SERVER_OPEN_COROUTINE)TC_Common::strto<int>(option.getValue("pattern"));

		MyServer server;
		server.setOpenCoroutine(pattern);

		server.initialize();

		string ep = option.getValue("ep");
		if(ep.empty())
		{
			ep = "tcp -h 0.0.0.0 -p 8084 -t 60000";
		}

		server.bindHttp("tcp -h 0.0.0.0 -p 8083 -t 60000");
		server.bindSocket(ep);
		 server.bindSocket("udp -h 0.0.0.0 -p 8085 -t 60000");

//		__CT__->enable(true);

		TC_Port::registerCtrlC([&]{

			server.getEpollServer()->terminate();
		});
		server.waitForShutdown();

//		__CT__->output();
//		__CT__->outputAvg();

	}
	catch (exception &ex)
	{
		cerr << "MyServer::run ex:" << ex.what() << endl;
	}

	cout << "MyServer::run server main thread exit." << endl;
}
