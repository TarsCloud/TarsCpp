
#include "hello_test.h"

TEST_F(HelloTest, rpcASyncGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());
}



TEST_F(HelloTest, rpcASyncGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());
}


TEST_F(HelloTest, rpcASyncServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkASync(comm);
	});
}

TEST_F(HelloTest, rpcASyncServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkASync(comm);
	});
}

TEST_F(HelloTest, rpcASyncAllocCommunicator)
{
	transAllocCommunicator([&](Communicator *comm){
		checkASync(comm);
	});
}

TEST_F(HelloTest, rpcASyncAllocCommunicatorInCoroutine)
{
	transInCoroutineAllocCommunicator([&](Communicator *comm){
		checkASync(comm);
	});
}

TEST_F(HelloTest, rpcASyncComplexCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transComplexCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());

	transComplexCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());
}


TEST_F(HelloTest, rpcASyncComplexCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineComplexCommunicator([&](Communicator *comm)
										{
											checkASync(comm);
										},
										c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());

	transInCoroutineComplexCommunicator([&](Communicator *comm){
		checkASync(comm);
	}, c.get());

}

TEST_F(HelloTest, rpcASyncThreadFinish)
{
	shared_ptr<Communicator> c = getCommunicator();
	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

	atomic<int> callback_count{0};

	int count = 10000;
	{
		std::thread cor_call([&]()
		{
			HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");

			//发起远程调用
			for (int j = 0; j < count; ++j)
			{
				HelloPrxCallbackPtr p = new ClientHelloCallback(TC_Common::now2us(), j, count, _buffer, callback_count);

				prx->async_testHello(p, j, _buffer);
			}

		});
		cor_call.join();
	}

	waitForFinish(callback_count, count);

	// TC_Common::sleep(10);

	// LOG_CONSOLE_DEBUG << _count << ", " << callback_count << endl;
	ASSERT_TRUE(count == callback_count);

	stopServer(server);

}