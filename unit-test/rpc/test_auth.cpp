
#include "hello_test.h"

TEST_F(HelloTest, authSyncServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkSync(comm, "AuthObjAdapter");
	});
}

TEST_F(HelloTest, authASyncGlobalCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkASync(comm, "AuthObjAdapter");
	});
}

