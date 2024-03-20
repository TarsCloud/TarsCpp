
#include "hello_test.h"

TEST_F(HelloTest, transGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	}, c.get());

	transGlobalCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	}, c.get());

	transGlobalCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	}, c.get());

	transGlobalCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	}, c.get());
}

TEST_F(HelloTest, transGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	}, c.get());

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	}, c.get());

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	}, c.get());

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	}, c.get());
}

TEST_F(HelloTest, transServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	});

	transServerCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	});

	transServerCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	});

	transServerCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	});
}

TEST_F(HelloTest, transServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	});

	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	});

	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	});

	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	});
}

TEST_F(HelloTest, transAllocCommunicator)
{
	transAllocCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	});

	transAllocCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	});

	transAllocCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	});

	transAllocCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	});
}

TEST_F(HelloTest, transAllocCommunicatorInCoroutine)
{
	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	});

	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	});

	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	});

	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	});
}


TEST_F(HelloTest, transComplexCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transComplexCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	}, c.get());
}


TEST_F(HelloTest, transComplexCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkTransSyncASync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkTransASyncASync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkTransSyncSync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkTransASyncSync(comm);
	}, c.get());
}

