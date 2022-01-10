
#include "hello_test.h"
#include "../server/WinServer.h"



TEST_F(HelloTest, winServerInCoroutine)
{
	funcInCoroutine([&]()
			{
				auto comm = getCommunicator();
				WinServer ws;
				startServer(ws, WIN_CONFIG());

				string obj = getObj(ws.getConfig(), "WinAdapter");

				HelloPrx prx = comm->stringToProxy<HelloPrx>(obj);

				string out;

				prx->testHello(0, _buffer, out);

				ASSERT_TRUE(_buffer == out);

				stopServer(ws);
			},
			true);
}

TEST_F(HelloTest, winServerGlobalInCoroutine)
{
	auto comm = getCommunicator();

	WinServer ws;
	startServer(ws, WIN_CONFIG());

	string obj = getObj(ws.getConfig(), "WinAdapter");

	{
		std::thread cor_call([&]()
		{
			auto scheduler = TC_CoroutineScheduler::create();

			//设置到协程中
			ServantProxyThreadData::getData()->_sched = scheduler;

			scheduler->go([&]()
			{

				scheduler->setNoCoroutineCallback([=](TC_CoroutineScheduler* s)
				{
					s->terminate();
				});

				HelloPrx prx = comm->stringToProxy<HelloPrx>(obj);

				string out;

				prx->testHello(0, _buffer, out);

				ASSERT_TRUE(_buffer == out);

			});

			scheduler->run();
		});
		cor_call.join();
	}

	stopServer(ws);
}

TEST_F(HelloTest, winServer)
{
	shared_ptr<Communicator> c = getCommunicator();

	WinServer ws;
	startServer(ws, WIN_CONFIG(), (TC_EpollServer::SERVER_OPEN_COROUTINE)0);

	string obj = getObj(ws.getConfig(), "WinAdapter");

	HelloPrx prx = c->stringToProxy<HelloPrx>(obj);

	string out;

	prx->testHello(0, _buffer, out);

	stopServer(ws);

}

TEST_F(HelloTest, winServerAsync)
{
	shared_ptr<Communicator> c = getCommunicator();

	for(int i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		WinServer ws;
		startServer(ws, WIN_CONFIG(), (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

		atomic<int> callback_count{ 0 };

		string obj = getObj(ws.getConfig(), "WinAdapter");

		HelloPrx prx = c->stringToProxy<HelloPrx>(obj);

		//发起远程调用
		for (int j = 0; j < _count; ++j)
		{
			HelloPrxCallbackPtr p = new ClientHelloCallback(TC_Common::now2us(), j, _count, _buffer,
					callback_count);

			prx->async_testHello(p, j, _buffer);
		}

		waitForFinish(callback_count, _count);

		ASSERT_TRUE(callback_count == _count);

		stopServer(ws);
	}

}


TEST_F(HelloTest, winServerHandleDestroy)
{
	int i = 1;//TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO;
	for(i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		g_handleDestroy = 0;

		WinServer ws;
		startServer(ws, WIN_CONFIG(), (TC_EpollServer::SERVER_OPEN_COROUTINE) i);

		string obj = getObj(ws.getConfig(), "WinAdapter");

		HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

		string out;

		prx->testHello(0, _buffer, out);

		ASSERT_TRUE(_buffer == out);
		stopServer(ws);

		ASSERT_TRUE(g_handleDestroy == TC_Common::strto<int>(ws.getConfig().get("/tars/application/server/WinAdapter<threads>")));
	}

}

TEST_F(HelloTest, winServerFdLeak)
{
	int i = 0;
	//先跑一边, 把需要分配的句柄都分配完
	for (i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		WinServer ws;
		startServer(ws, WIN_CONFIG(), (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

		string obj = getObj(ws.getConfig(), "WinAdapter");

		HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

		string out;

		//网络通信器有多个, 就要调用多次
		prx->testHello(0, _buffer, out);
		ASSERT_TRUE(_buffer == out);

		prx->testHello(0, _buffer, out);
		ASSERT_TRUE(_buffer == out);

		stopServer(ws);
	}

	//第二遍检查句柄是否泄漏
	for (i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		int ofdCount = getFdCounts();
		{
			WinServer ws;
			startServer(ws, WIN_CONFIG(), (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

			string obj = getObj(ws.getConfig(), "WinAdapter");

			HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

			string out;

			//网络通信器有多个, 就要调用多次
			prx->testHello(0, _buffer, out);
			ASSERT_TRUE(_buffer == out);

			prx->testHello(0, _buffer, out);
			ASSERT_TRUE(_buffer == out);

			stopServer(ws);
		}

		// TC_Common::sleep(1000);
		int nfdCount = getFdCounts();

		//linux下才有效
		LOG_CONSOLE_DEBUG << "old fd count:" << ofdCount << ", new fd count:" << nfdCount << endl;

		// ASSERT_TRUE(ofdCount == nfdCount);
	}
}


TEST_F(HelloTest, winServerRestart)
{
	int i = 0;//TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO;
	for(i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		WinServer ws;

		int count = 10;
		while(count-->0)
		{
			startServer(ws, WIN_CONFIG(), (TC_EpollServer::SERVER_OPEN_COROUTINE)i);

			string obj = getObj(ws.getConfig(), "WinAdapter");

			HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

			string out;

			prx->testHello(0, _buffer, out);

			ASSERT_TRUE(_buffer == out);

			stopServer(ws);
		}
	}
}

TEST_F(HelloTest, winLog)
{
	WinServer ws;
	startServer(ws, WIN_CONFIG());

	string log1 = ServerConfig::Application + FILE_SEP + ServerConfig::ServerName + FILE_SEP + ServerConfig::Application + "." + ServerConfig::ServerName + ".test1.log";
	string log2 = ServerConfig::Application + FILE_SEP + ServerConfig::ServerName + FILE_SEP + ServerConfig::Application + "." + ServerConfig::ServerName + ".test2.log";
	TC_File::removeFile(log1, false);
	TC_File::removeFile(log2, false);

	ASSERT_TRUE(!TC_File::isFileExist(log1));
	ASSERT_TRUE(!TC_File::isFileExist(log2));

	TLOGEXDEBUG("test1", "test log"<< endl);
	TLOGEXDEBUG("test2", "test log"<< endl);

	LOG_CONSOLE_DEBUG << log1 << endl;
	
	ASSERT_TRUE(TC_File::isFileExist(log1));
	ASSERT_TRUE(TC_File::isFileExist(log2));

	stopServer(ws);
}


TEST_F(HelloTest, winServerCo)
{
	{
		WinServer ws;

		startServer(ws, WIN_CONFIG(), TC_EpollServer::NET_THREAD_MERGE_HANDLES_THREAD);

		string obj = getObj(ws.getConfig(), "WinAdapter");

		HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

		string out;

		bool co = prx->testCoro(_buffer, out);

		EXPECT_FALSE(co);

		stopServer(ws);
	}

	{
		WinServer ws;

		startServer(ws, WIN_CONFIG(), TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO);

		string obj = getObj(ws.getConfig(), "WinAdapter");

		HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

		string out;

		bool co = prx->testCoro(_buffer, out);

		EXPECT_TRUE(co);

		stopServer(ws);
	}
	{
		WinServer ws;

		startServer(ws, WIN_CONFIG(), TC_EpollServer::NET_THREAD_QUEUE_HANDLES_THREAD);

		string obj = getObj(ws.getConfig(), "WinAdapter");

		HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

		string out;

		bool co = prx->testCoro(_buffer, out);

		EXPECT_FALSE(co);

		stopServer(ws);
	}
	{
		WinServer ws;

		startServer(ws, WIN_CONFIG(), TC_EpollServer::NET_THREAD_QUEUE_HANDLES_CO);

		string obj = getObj(ws.getConfig(), "WinAdapter");

		HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

		string out;

		bool co = prx->testCoro(_buffer, out);

		EXPECT_TRUE(co);

		stopServer(ws);
	}
}


TEST_F(HelloTest, winServerHashTag)
{
	WinServer ws;

	startServer(ws, WIN_CONFIG(), TC_EpollServer::NET_THREAD_MERGE_HANDLES_THREAD);

	string obj = getObj(ws.getConfig(), "WinAdapter");

	size_t pos = obj.find_first_of("@");
	if(pos != string::npos) {
		obj = obj.substr(0, pos) + "#9999" + obj.substr(pos);
	}else{
		obj += "#9999";
	}

	HelloPrx prx = ws.getCommunicator()->stringToProxy<HelloPrx>(obj);

	string out;

	bool co = prx->testCoro(_buffer, out);

	EXPECT_FALSE(co);

	stopServer(ws);
}
