
#include "hello_test.h"
#include "servant/CommunicatorEpoll.h"


TEST_F(HelloTest, testNotifyCommunicatorEpoll)
{
	shared_ptr<Communicator> c = getCommunicator();

	int netThread = TC_Common::strto<int>(c->getProperty("netthread"));

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

	auto v1 = c->getAllCommunicatorEpoll();

	ASSERT_TRUE(v1.size() == 0);

	HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");

	std::thread th1([&]()
					{ checkSyncOnce(prx); });

	TC_Common::msleep(100);

	auto v2 = c->getAllCommunicatorEpoll();
	//两个公有通信器
	ASSERT_TRUE(v2.size() == (size_t)netThread);

	std::thread th2([&]()
					{ checkSyncOnce(prx); });
	TC_Common::msleep(100);

	auto v3 = c->getAllCommunicatorEpoll();
	//通信器个数不变
	ASSERT_TRUE(v3.size() == (size_t)netThread);

	funcInCoroutine([&]()
					{
		checkSyncOnce(prx);
		auto v4 = c->getAllCommunicatorEpoll();
		//为设置协程的情况下, 仍然是两个通信器
		ASSERT_TRUE(v4.size() == (size_t)netThread);
					},
					false);

	funcInCoroutine([&]()
					{
		checkSyncOnce(prx);
		auto v5 = c->getAllCommunicatorEpoll();
		//设置了协程, 会增加一个私有协程网络通信器
		ASSERT_TRUE(v5.size() == (size_t)(netThread + 1));
					},
					true);

	auto v6 = c->getAllCommunicatorEpoll();
	//之前的业务线程释放了, 会减少一个私有协程网络通信器
	ASSERT_TRUE(v6.size() == (size_t)netThread);

	funcInCoroutine([&]()
					{
		checkASyncOnce(prx);
		//设置协程, 会增加一个私有网路通信器, 同时异步回调在私有网络通信器中
		auto v7 = c->getAllCommunicatorEpoll();
		ASSERT_TRUE(v7.size() == (size_t)(netThread + 1));
					});

	th1.join();
	th2.join();

	stopServer(server);

}

int getNotify(shared_ptr<Communicator> c)
{
	if(c->getCommunicatorEpollNum() <= 0)
	{
		return 0;
	}
	auto n = c->getCommunicatorEpoll(0)->getNotify();

	int sum = 0;
	for(size_t i = 0; i< MAX_CLIENT_NOTIFYEVENT_NUM; i++)
	{
		if(n[i] != NULL)
		{
			++sum;
		}
	}
	return sum;
}


//测试notify句柄不泄露!
TEST_F(HelloTest, testNotifyBussThreadQuitOnce)
{
	shared_ptr<Communicator> c = getCommunicator();

	HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

	//先把服务器端的强制都创建好
	for (int i = 0; i < 20; i++)
	{
		checkSyncOnce(prx);
	}

	for (int i = 0; i < 3; i++)
	{
		size_t sp_count = ServantProxyThreadData::g_immortal->getList().size();
		size_t notify_count = getNotify(c);

		{
			std::thread th1([&]()
							{ checkSyncOnce(prx); });

			th1.join();
		}
		TC_Common::msleep(10);

		//私有线程数据释放了, 通知句柄数不增加
		ASSERT_TRUE(ServantProxyThreadData::g_immortal->getList().size() == sp_count);
		ASSERT_TRUE(getNotify(c) == (int)notify_count);
	}

	stopServer(server);

}

//测试notify句柄不泄露!
TEST_F(HelloTest, testNotifyBussThreadQuit)
{
	shared_ptr<Communicator> c = getCommunicator();
	HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

//	for(int i = 0; i < 5000; i++)
	for(int i = 0; i < 1000; i++)
	{
		{
			std::thread th1([&]()
							{
								checkSyncOnce(prx);
							});

			th1.join();
		}
		TC_Common::msleep(10);

		int num = getNotify(c);
		ASSERT_TRUE(num < 5);
	}

	stopServer(server);

}
//
//TEST_F(HelloTest, testNotifyCommunicatorQuit)
//{
//	HelloServer server;
//	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);
//
//	for(int i = 0; i < 1000; i++)
//	{
//		shared_ptr<Communicator> c = getCommunicator();
//		HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");
//
//		{
//
//			std::thread th1([&]()
//			{
//				try
//				{
//					checkSyncOnce(prx);
//				}
//				catch(exception &ex)
//				{
//					LOG_CONSOLE_DEBUG << i << ", " << ex.what() << endl;
//				}
//
//				c.reset();
//			});
//
//			th1.join();
//		}
//
//	}
//
//	stopServer(server);
//
//}

TEST_F(HelloTest, testNotifyProxyQuit)
{
	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE)0);

	shared_ptr<Communicator> c = getCommunicator();
	HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");

	int count = 10;
	while (count-- > 0)
	{
		vector<std::thread*> vt;

		for (int i = 0; i < 1; i++)
		{
			vt.push_back(new std::thread([&]()
			{
				checkSyncOnce(prx);
			}));
		}

		for (auto v: vt)
		{
			v->join();
			delete v;
		}

//		TC_Common::sleep(3);
	}

	
	stopServer(server);
}


TEST_F(HelloTest, testCommunicatorGetResourcesInfo)
{
	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE)0);

	shared_ptr<Communicator> c = getCommunicator();
	HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");

	int count = 10;
	while (count-- > 0)
	{
		vector<std::thread*> vt;

		for (int i = 0; i < 1; i++)
		{
			vt.push_back(new std::thread([&]()
			{
				checkSyncOnce(prx);
			}));
		}

		for (auto v: vt)
		{
			v->join();
			delete v;
		}

	}

	string buf = c->getResourcesInfo();
	LOG_CONSOLE_DEBUG << buf << endl;


	stopServer(server);
}
#if 0

TEST_F(HelloTest, testNotifyCtrlC)
{
	std::thread th([](){
		TC_Common::sleep(2);
		kill(getpid(), SIGINT);
	});

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

	HelloPrx prx = getObj<HelloPrx>(server.getCommunicator(), "HelloAdapter");

	for(int i = 0; i < 10000; i++)
	{
		checkSyncOnce(prx);
	}

	stopServer(server);
}

TEST_F(HelloTest, testNotifyCtrlCGlobalCommunicator)
{
	std::thread th([](){
		TC_Common::sleep(2);
		kill(getpid(), SIGINT);
	});

	shared_ptr<Communicator> c = std::make_shared<Communicator>();

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);
	for(int i = 0; i < 10000; i++)
	{
		checkSync(c.get());
	}

	stopServer(server);
}

#endif
