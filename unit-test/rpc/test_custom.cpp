
#include "hello_test.h"

TEST_F(HelloTest, customSyncGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		syncCustom(comm);
	}, c.get());
}

TEST_F(HelloTest, customASyncGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		asyncCustom(comm);
	}, c.get());
}
