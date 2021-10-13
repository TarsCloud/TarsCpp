
#include "hello_test.h"

TEST_F(HelloTest, rpcWupTransGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	}, c.get());
}

TEST_F(HelloTest, rpcWupTransGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	}, c.get());

}

TEST_F(HelloTest, rpcWupTransServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	});
}

TEST_F(HelloTest, rpcWupTransServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	});
}

TEST_F(HelloTest, rpcWupTransAllocCommunicator)
{
	transAllocCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	});
}

TEST_F(HelloTest, rpcWupTransAllocCommunicatorInCoroutine)
{
	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	});
}


TEST_F(HelloTest, rpcWupTransComplexCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transComplexCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	}, c.get());
}


TEST_F(HelloTest, rpcWupTransComplexCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkWupTransSync(comm);
	}, c.get());
}

