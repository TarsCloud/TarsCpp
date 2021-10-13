
#include "hello_test.h"
#include "servant/CommunicatorEpoll.h"
#include "servant/ObjectProxy.h"
#include "server/framework/DbHandle.h"
#include "server/FrameworkServer.h"
#include "QueryF.h"

TEST_F(HelloTest, prxUpdate)
{
	shared_ptr<Communicator> comm = getCommunicator();

	RpcServer rpc1Server;
	startServer(rpc1Server, RPC1_CONFIG());

	RpcServer rpc2Server;
	startServer(rpc2Server, RPC2_CONFIG());

	HelloPrx qPrx = comm->stringToProxy<HelloPrx>("TestApp.RpcServer.HelloObj@tcp -h 127.0.0.1 -p 9990:tcp -h 127.0.0.1 -p 9991");
	int ret;
	string out;
	ret = qPrx->testHello(0, _buffer, out);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(out == _buffer);

	set<EndpointInfo> active;
	active.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9992")));

	set<EndpointInfo> inactive;
	inactive.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9990")));
	inactive.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9991")));

	qPrx->tars_update_endpoints(active, inactive);

	stopServer(rpc1Server);
	stopServer(rpc2Server);

	RpcServer rpc3Server;
	startServer(rpc3Server, RPC3_CONFIG());

	ret = qPrx->testHello(0, _buffer, out);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(out == _buffer);

	stopServer(rpc3Server);
}

TEST_F(HelloTest, prxUpdateInCoroutine1)
{
	shared_ptr<Communicator> comm = getCommunicator();

	RpcServer rpc1Server;
	startServer(rpc1Server, RPC1_CONFIG());

	RpcServer rpc2Server;
	startServer(rpc2Server, RPC2_CONFIG());

	int ret;
	string out;
	HelloPrx qPrx;

	funcInCoroutine([&](){
		qPrx = comm->stringToProxy<HelloPrx>("TestApp.RpcServer.HelloObj@tcp -h 127.0.0.1 -p 9990:tcp -h 127.0.0.1 -p 9991");

		ret = qPrx->testHello(0, _buffer, out);
		ASSERT_TRUE(ret == 0);
		ASSERT_TRUE(out == _buffer);
	});

	set<EndpointInfo> active;
	active.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9992")));

	set<EndpointInfo> inactive;
	inactive.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9990")));
	inactive.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9991")));

	qPrx->tars_update_endpoints(active, inactive);

	stopServer(rpc1Server);
	stopServer(rpc2Server);

	RpcServer rpc3Server;
	startServer(rpc3Server, RPC3_CONFIG());

	ret = qPrx->testHello(0, _buffer, out);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(out == _buffer);

	stopServer(rpc3Server);
}


TEST_F(HelloTest, prxUpdateInCoroutine2)
{
	shared_ptr<Communicator> comm = getCommunicator();

	RpcServer rpc1Server;
	startServer(rpc1Server, RPC1_CONFIG());

	RpcServer rpc2Server;
	startServer(rpc2Server, RPC2_CONFIG());

	int ret;
	string out;
	HelloPrx qPrx;

	funcInCoroutine([&](){
		qPrx = comm->stringToProxy<HelloPrx>("TestApp.RpcServer.HelloObj@tcp -h 127.0.0.1 -p 9990:tcp -h 127.0.0.1 -p 9991");

		ret = qPrx->testHello(0, _buffer, out);
		ASSERT_TRUE(ret == 0);
		ASSERT_TRUE(out == _buffer);
	}, true);

	funcInCoroutine([&]()
	{
		set<EndpointInfo> active;
		active.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9992")));

		set<EndpointInfo> inactive;
		inactive.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9990")));
		inactive.insert(EndpointInfo(TC_Endpoint("tcp -h 127.0.0.1 -p 9991")));

		qPrx->tars_update_endpoints(active, inactive);

	}, true);

	stopServer(rpc1Server);
	stopServer(rpc2Server);

	RpcServer rpc3Server;
	startServer(rpc3Server, RPC3_CONFIG());

	ret = qPrx->testHello(0, _buffer, out);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(out == _buffer);

	stopServer(rpc3Server);
}

