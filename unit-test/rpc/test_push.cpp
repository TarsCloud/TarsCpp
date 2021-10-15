
#include "hello_test.h"

TEST_F(HelloTest, pushSyncGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		testPush(comm);
	}, c.get());
}

TEST_F(HelloTest, pushSyncGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		testPush(comm);
	}, c.get());
}

TEST_F(HelloTest, pushReconnectGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		testReconnect(comm);
	}, c.get());
}

