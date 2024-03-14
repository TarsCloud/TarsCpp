
#include "hello_test.h"
//#include "util/tc_socket_async.h"

TEST_F(HelloTest, proxyInvokeTimeout)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkSyncTimeout(comm);
	}, c.get());

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkSyncTimeout(comm);
	}, c.get());

	transServerCommunicator([&](Communicator *comm){
		checkSyncTimeout(comm);
	});

	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkSyncTimeout(comm);
	});

	transAllocCommunicator([&](Communicator *comm){
		checkSyncTimeout(comm);
	});

	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkSyncTimeout(comm);
	});

}

TEST_F(HelloTest, proxyAsyncInvokeTimeout)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkASyncTimeout(comm);
	}, c.get());

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkASyncTimeout(comm);
	}, c.get());

	transServerCommunicator([&](Communicator *comm){
		checkASyncTimeout(comm);
	});

	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkASyncTimeout(comm);
	});

	transAllocCommunicator([&](Communicator *comm){
		checkASyncTimeout(comm);
	});

	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkASyncTimeout(comm);
	});

}

TEST_F(HelloTest, proxyConnectionTimeout)
{
	shared_ptr<Communicator> c = getCommunicator();

	HelloPrx prx =  c->stringToProxy<HelloPrx>("TestApp.HelloServer.HelloObj@tcp -h 111.111.111.11 -p 4444");

	prx->tars_connect_timeout(5000);
	prx->tars_set_timeout(20000);
	prx->tars_async_timeout(30000);

	int64_t t = TNOWMS;
	bool connectError = false;
	try
	{
		string out;
		prx->testHello(0, "test", out);
	}
	catch(exception &ex)
	{
		connectError = true;
	}

	ASSERT_TRUE(connectError);
	LOG_CONSOLE_DEBUG << TNOWMS - t << endl;
	ASSERT_TRUE(TNOWMS - t >= 5000 && TNOWMS - t < 5050);
}
