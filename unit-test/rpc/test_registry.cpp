
#include "hello_test.h"
#include "servant/CommunicatorEpoll.h"
#include "servant/ObjectProxy.h"
#include "server/framework/DbHandle.h"
#include "server/FrameworkServer.h"
#include "QueryF.h"

TEST_F(HelloTest, registryQuery)
{
	shared_ptr<Communicator> c = getCommunicator();

	TC_Config conf = CLIENT_CONFIG();

	c->setProperty(conf);

	TC_Config fconf = FRAMEWORK_CONFIG();

	CDbHandle::cleanEndPoint();
	CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9990, 1);
	CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9991, 1);

	FrameworkServer fs;
	startServer(fs, FRAMEWORK_CONFIG());

	string obj = getObj(fconf, "RegistryAdapter");

	QueryFPrx qPrx = c->stringToProxy<QueryFPrx>(obj);

	{
		std::vector<EndpointF> endpoints = qPrx->findObjectById("TestApp.RpcServer.HelloObj");

		ASSERT_TRUE(endpoints.size() == 2);
		ASSERT_TRUE(endpoints[0].port == 9990);
		ASSERT_TRUE(endpoints[1].port == 9991);
	}

	{
		CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9992, 1);

		TC_Common::sleep(6);

		qPrx->getEndpoint();

		TC_Common::msleep(100);

		{
			std::vector<EndpointF> endpoints = qPrx->findObjectById("TestApp.RpcServer.HelloObj");

			ASSERT_TRUE(endpoints.size() == 3);
			ASSERT_TRUE(endpoints[0].port == 9990);
			ASSERT_TRUE(endpoints[1].port == 9991);
			ASSERT_TRUE(endpoints[2].port == 9992);
		}
	}

	{
		CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9993, 1);

		TC_Common::sleep(6);

		qPrx->getEndpoint();

		TC_Common::msleep(100);

		{
			std::vector<EndpointF> endpoints = qPrx->findObjectById("TestApp.RpcServer.HelloObj");

			ASSERT_TRUE(endpoints.size() == 4);
			ASSERT_TRUE(endpoints[0].port == 9990);
			ASSERT_TRUE(endpoints[1].port == 9991);
			ASSERT_TRUE(endpoints[2].port == 9992);
			ASSERT_TRUE(endpoints[3].port == 9993);
		}
	}

	stopServer(fs);

}

#define START_FRAMEWORK_SERVER_1_2 \
CDbHandle::cleanEndPoint(); \
CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9990, 1); \
CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9991, 1); \
FrameworkServer fs; \
startServer(fs, FRAMEWORK_CONFIG()); \
RpcServer rpc1Server; \
startServer(rpc1Server, RPC1_CONFIG()); \
RpcServer rpc2Server; \
startServer(rpc2Server, RPC2_CONFIG());


#define START_FRAMEWORK_HTTP_SERVER_1_2 \
CDbHandle::cleanEndPoint(); \
CDbHandle::addActiveEndPoint("TestApp.RpcServer.HttpObj", 8180, 1); \
CDbHandle::addActiveEndPoint("TestApp.RpcServer.HttpObj", 8181, 1); \
FrameworkServer fs; \
startServer(fs, FRAMEWORK_CONFIG()); \
RpcServer rpc1Server; \
startServer(rpc1Server, RPC1_CONFIG()); \
RpcServer rpc2Server; \
startServer(rpc2Server, RPC2_CONFIG());

#define STOP_FRAMEWORK_SERVER stopServer(fs);

#define HELLO_CALL {\
HelloPrx qPrx = comm->stringToProxy<HelloPrx>("TestApp.RpcServer.HelloObj"); \
int ret; \
string out; \
ret = qPrx->testHello(0, _buffer, out); \
ASSERT_TRUE(ret == 0); \
ASSERT_TRUE(out == _buffer); }

#define CHECK_REGISTRY_UPDATE {\
stopServer(rpc1Server); \
CDbHandle::cleanEndPoint(); \
CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9992, 1); \
RpcServer rpc3Server; \
startServer(rpc3Server, RPC3_CONFIG()); \
wait(6000);                    \
HelloPrx qPrx = comm->stringToProxy<HelloPrx>("TestApp.RpcServer.HelloObj"); \
string out = ""; \
int ret = qPrx->testHello(0, _buffer, out); \
ASSERT_TRUE(ret == 0); \
ASSERT_TRUE(out == _buffer); \
stopServer(rpc2Server); \
wait(100); \
out = ""; \
ret = qPrx->testHello(0, _buffer, out); \
ASSERT_TRUE(ret == 0); \
ASSERT_TRUE(out == _buffer); \
stopServer(rpc3Server); }

#define CREATE_COR \
{ \
	auto scheduler = TC_CoroutineScheduler::create(); \
	ServantProxyThreadData::getData()->_sched = scheduler; \
	scheduler->go([&]() \
	{	\
		{ HELLO_CALL; wait(6500); HELLO_CALL }   \
	}); \
	scheduler->run();	\
}

#define HTTP_CALL {\
ServantPrx prx = comm->stringToProxy<ServantPrx>("TestApp.RpcServer.HttpObj"); \
prx->tars_set_protocol(ServantProxy::PROTOCOL_HTTP1, 5); \
string buff = _buffer + "-" + TC_Common::tostr(TC_Thread::CURRENT_THREADID()); \
shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>(); \
req->setPostRequest("http://tars.com/hello", buff, true); \
req->setHeader("Connection", "close"); \
shared_ptr<TC_HttpResponse> rsp;                                               \
int count = 10;     \
while(count-- > 0) { prx->http_call("hello", req, rsp); } }

#define HTTP_CREATE_COR \
{ \
	auto scheduler = TC_CoroutineScheduler::create(); \
	ServantProxyThreadData::getData()->_sched = scheduler; \
	scheduler->go([&]() \
	{	\
		{ HTTP_CALL; wait(6200); HTTP_CALL }   \
	}); \
	scheduler->run();	\
}

TEST_F(HelloTest, registryRpc)
{
//	shared_ptr<Communicator> c = getCommunicator();

	START_FRAMEWORK_SERVER_1_2

	shared_ptr<Communicator> c = getCommunicator();

	rpcFromRegistry(c.get());
	LOG_CONSOLE_DEBUG << endl;

	funcInCoroutine([&](){
		rpcFromRegistry(c.get());
	});

	funcInCoroutine([&](){
		shared_ptr<Communicator> comm = getCommunicator();

		rpcFromRegistry(comm.get());
	});

	LOG_CONSOLE_DEBUG << endl;
	stopServer(rpc1Server);
	stopServer(rpc2Server);
	stopServer(fs);
}


//一致hash测试
TEST_F(HelloTest, registryRpcConHashInvoke)
{
	START_FRAMEWORK_SERVER_1_2
        CDbHandle::addInactiveEndPoint("TestApp.RpcServer.HelloObj", 9989, 1);
	CDbHandle::addInactiveEndPoint("TestApp.RpcServer.HelloObj", 9992, 1);

	shared_ptr<Communicator> c = getCommunicator();

	rpcConHashFromRegistry(c.get());

	stopServer(rpc1Server);
	stopServer(rpc2Server);
        STOP_FRAMEWORK_SERVER;
}

TEST_F(HelloTest, registryRpcUpdateList)
{
	shared_ptr<Communicator> comm = getCommunicator();
	comm->setProperty("refresh-endpoint-interval", "5000");

	START_FRAMEWORK_SERVER_1_2

	HELLO_CALL

	CHECK_REGISTRY_UPDATE

	STOP_FRAMEWORK_SERVER
}


TEST_F(HelloTest, registryRpcUpdateListInCoroutine1)
{
	shared_ptr<Communicator> comm = getCommunicator();
	comm->setProperty("refresh-endpoint-interval", "5000");

	START_FRAMEWORK_SERVER_1_2

	funcInCoroutine([&]()
	{
		HELLO_CALL

		CHECK_REGISTRY_UPDATE
	});

	STOP_FRAMEWORK_SERVER
}

TEST_F(HelloTest, registryRpcUpdateListInCoroutine2)
{
	shared_ptr<Communicator> comm = getCommunicator();
	comm->setProperty("refresh-endpoint-interval", "5000");

	START_FRAMEWORK_SERVER_1_2

	HELLO_CALL

	funcInCoroutine([&]()
	{
		CHECK_REGISTRY_UPDATE
	});

	STOP_FRAMEWORK_SERVER
}


TEST_F(HelloTest, registryRpcCheckUpdateList)
{
	shared_ptr<Communicator> comm = getCommunicator();
	comm->setProperty("refresh-endpoint-interval", "5000");

	START_FRAMEWORK_SERVER_1_2

	//发起网路调用
	HELLO_CALL

	HELLO_CALL

	//构建多个线程, 都完成hello的调用
	std::thread cor1([&]()
	{
		CREATE_COR
	});

	std::thread cor2([&]()
	{
		CREATE_COR
	});

	std::thread cor3([&]()
	{
		CREATE_COR
	});

	TC_Common::msleep(200);

	vector<shared_ptr<CommunicatorEpoll>> v = comm->getAllCommunicatorEpoll();

	//5个网络通信器(2个公有, 3个私有)
	ASSERT_TRUE(v.size() == 5);
	ASSERT_FALSE(v[0]->isSchedCommunicatorEpoll());
	ASSERT_FALSE(v[1]->isSchedCommunicatorEpoll());
	ASSERT_TRUE(v[2]->isSchedCommunicatorEpoll());
	ASSERT_TRUE(v[3]->isSchedCommunicatorEpoll());
	ASSERT_TRUE(v[4]->isSchedCommunicatorEpoll());

	vector<ObjectProxy*> vop;

	for (auto ce : v)
	{
		ObjectProxy* op = ce->hasObjectProxy("TestApp.RpcServer.HelloObj");

		if (op)
		{
			vop.push_back(op);
		}
	}

	//网络通信器都有对象
	ASSERT_TRUE(vop.size() == 5);

	for (auto op : vop)
	{
		vector<AdapterProxy*> adapters = op->getAdapters();
//		LOG_CONSOLE_DEBUG << adapters.size() << endl;
//		for(auto a : adapters)
//		{
//			LOG_CONSOLE_DEBUG << a->trans()->getConnectEndpoint().toString() << endl;
//		}
		ASSERT_TRUE(adapters.size() == 2);
		ASSERT_TRUE(adapters[0]->trans()->getConnectEndpoint().getPort() == 9990);
		ASSERT_TRUE(adapters[1]->trans()->getConnectEndpoint().getPort() == 9991);
	}

	stopServer(rpc1Server);

	//更新主控ip list
	CDbHandle::cleanEndPoint();
	CDbHandle::addActiveEndPoint("TestApp.RpcServer.HelloObj", 9992, 1);

	RpcServer rpc3Server;
	startServer(rpc3Server, RPC3_CONFIG());
	//等待主控更新时间
	wait(6000);

	//调用两次, 这样两个公有网路通信器都会更新ip list
	HELLO_CALL

	HELLO_CALL

	stopServer(rpc2Server);

	wait(1000);

	for (auto op : vop)
	{
		vector<AdapterProxy*> adapters = op->getAdapters();

		for(auto adapter : adapters)
		{
//			LOG_CONSOLE_DEBUG << "isSched:"<< op->getCommunicatorEpoll()->isSchedCommunicatorEpoll()
//			<< ", netThreadSeq:" << op->getCommunicatorEpoll()->getCommunicatorNetThreadSeq()
//			<< ", " << adapter->trans()->getConnectEndpoint().toString()
//			<< ", isActiveInReg:" << adapter->isActiveInReg() << endl;

			if(adapter->trans()->getConnectEndpoint().getPort() == 9992)
			{
				ASSERT_TRUE(adapter->isActiveInReg());
			}
			else
			{
				ASSERT_FALSE(adapter->isActiveInReg());
			}
		}
	}

	//第三个服务也停掉
	stopServer(rpc3Server);
	cor1.detach();
	cor2.detach();
	cor3.detach();
	STOP_FRAMEWORK_SERVER
}


TEST_F(HelloTest, registryHttpRpcCheckUpdateList)
{
	shared_ptr<Communicator> comm = getCommunicator();
	comm->setProperty("refresh-endpoint-interval", "5000");

	START_FRAMEWORK_HTTP_SERVER_1_2

	//发起网路调用
	HTTP_CALL

	//构建多个线程, 都完成hello的调用
	std::thread cor1([&]()
	{
		//注意http call里面会wait一下在调用, 等待ip list更新
		HTTP_CREATE_COR
	});

	TC_Common::msleep(200);

	vector<shared_ptr<CommunicatorEpoll>> v = comm->getAllCommunicatorEpoll();

//	for_each(v.begin(), v.end(), [](const shared_ptr<CommunicatorEpoll> &c){
//		LOG_CONSOLE_DEBUG << c->getCommunicatorNetThreadSeq() << ", " << c->isSchedCommunicatorEpoll() << endl;
//	});

	//3个网络通信器(2个公有, 1个私有)
	ASSERT_TRUE(v.size() == 3);
	ASSERT_FALSE(v[0]->isSchedCommunicatorEpoll());
	ASSERT_FALSE(v[1]->isSchedCommunicatorEpoll());
	ASSERT_TRUE(v[2]->isSchedCommunicatorEpoll());

	ServantPrx prx = comm->stringToProxy<ServantPrx>("TestApp.RpcServer.HttpObj"); \

	vector<ObjectProxy*> vop = 	prx->getObjectProxys();
	//并行连接数*网络通信器个数
	ASSERT_TRUE(vop.size() == 5*v.size());

	for (auto op : vop)
	{
		vector<AdapterProxy*> adapters = op->getAdapters();
//		for_each(adapters.begin(), adapters.end(), [](AdapterProxy*ap){
//			LOG_CONSOLE_DEBUG << ap->trans()->getConnectEndpoint().toString() << endl;
//		});
//		LOG_CONSOLE_DEBUG << op << ", -------------------------------------------------" << endl;
		ASSERT_TRUE(adapters.size() == 2);
		ASSERT_TRUE(adapters[0]->trans()->getConnectEndpoint().getPort() == 8180);
		ASSERT_TRUE(adapters[1]->trans()->getConnectEndpoint().getPort() == 8181);
	}

	stopServer(rpc1Server);

	//更新主控ip list
	CDbHandle::cleanEndPoint();
	CDbHandle::addActiveEndPoint("TestApp.RpcServer.HttpObj", 8182, 1);

	RpcServer rpc3Server;
	startServer(rpc3Server, RPC3_CONFIG());
	//等待主控更新时间
	wait(6000);
	//调用, 触发ip list更新
	HTTP_CALL
	stopServer(rpc2Server);
	wait(100);
	stopServer(rpc3Server);

	for (auto op : vop)
	{
		vector<AdapterProxy*> adapters = op->getAdapters();

		for(auto adapter : adapters)
		{
//			LOG_CONSOLE_DEBUG << "isSched:"<< op->getCommunicatorEpoll()->isSchedCommunicatorEpoll()
//			<< ", netThreadSeq:" << op->getCommunicatorEpoll()->getCommunicatorNetThreadSeq()
//			<< ", " << adapter->trans()->getConnectEndpoint().toString()
//			<< ", isActiveInReg:" << adapter->isActiveInReg() << endl;

			if(adapter->trans()->getConnectEndpoint().getPort() == 8182)
			{
				ASSERT_TRUE(adapter->isActiveInReg());
			}
			else
			{
				ASSERT_FALSE(adapter->isActiveInReg());
			}
		}
	}

	cor1.detach();
	STOP_FRAMEWORK_SERVER
}

TEST_F(HelloTest, registryRpcHashTagInvoke)
{
	START_FRAMEWORK_SERVER_1_2
	CDbHandle::addInactiveEndPoint("TestApp.RpcServer.HelloObj", 9989, 1);
	CDbHandle::addInactiveEndPoint("TestApp.RpcServer.HelloObj", 9992, 1);

	shared_ptr<Communicator> c = getCommunicator();

	string obj = "TestApp.RpcServer.HelloObj#9999";

	HelloPrx prx = c->stringToProxy<HelloPrx>(obj);

	string out;

	int co = prx->testHello(0, _buffer, out);

	ASSERT_TRUE(co == 0);

	stopServer(rpc1Server);
	stopServer(rpc2Server);
	STOP_FRAMEWORK_SERVER;
}


TEST_F(HelloTest, registryRpcMultiHashTagInvoke)
{
	START_FRAMEWORK_SERVER_1_2
	CDbHandle::addInactiveEndPoint("TestApp.RpcServer.HelloObj", 9989, 1);
	CDbHandle::addInactiveEndPoint("TestApp.RpcServer.HelloObj", 9992, 1);

	shared_ptr<Communicator> c = getCommunicator();

	string obj = "TestApp.RpcServer.HelloObj";

	HelloPrx prx = c->stringToProxy<HelloPrx>(obj);

	string out;

	int co = prx->testHello(0, _buffer, out);

	ASSERT_TRUE(co == 0);

	stopServer(rpc1Server);
	stopServer(rpc2Server);
	STOP_FRAMEWORK_SERVER;
}