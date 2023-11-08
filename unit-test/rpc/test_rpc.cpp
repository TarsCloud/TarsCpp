
#include "hello_test.h"

TEST_F(HelloTest, rpcSyncGlobalCommunicator)
{
	{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());
	}

	TC_Common::sleep(10);
}

TEST_F(HelloTest, rpcSyncGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());
}

TEST_F(HelloTest, rpcSyncServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkSync(comm);
	});
}


TEST_F(HelloTest, rpcSyncServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkSync(comm);
	});
}

//TEST_F(HelloTest, rpcSyncAllocCommunicator)
//{
//	transAllocCommunicator([&](Communicator *comm){
//		checkSync(comm);
//	});
//}
//
//TEST_F(HelloTest, rpcSyncAllocCommunicatorInCoroutine)
//{
//	transInCoroutineAllocCommunicator([&](Communicator *comm){
//		checkSync(comm);
//	});
//}

TEST_F(HelloTest, rpcSyncComplexCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());
}

TEST_F(HelloTest, rpcSyncComplexCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkSync(comm);
	}, c.get());
}


TEST_F(HelloTest, rpcSyncIpV6GlobalCommunicator)
{
    HelloServer server;
    startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

//    TC_Common::sleep(1000);
    checkSync(server.getCommunicator().get(), "Ipv6Adapter");

    stopServer(server);
//
//	shared_ptr<Communicator> c = getCommunicator();
//
//	transGlobalCommunicator([&](Communicator *comm){
//		checkSync(comm, "Ipv6Adapter");
//	}, c.get());
}

TEST_F(HelloTest, rpcSyncIpV6GlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkSync(comm, "Ipv6Adapter");
	}, c.get());
}
