
#include "hello_test.h"
//
//TEST_F(HelloTest, sslSyncClient)
//{
//	shared_ptr<Communicator> c = std::make_shared<Communicator>();
//	TC_Config conf = CLIENT_CONFIG();
//
//	c->setProperty(conf);
//
//	try
//	{
//
//		HelloPrx prx = c->stringToProxy<HelloPrx>("TestApp.SSLServer.SSLObj@ssl -h 127.0.0.1 -p 9005 -t 10000");
//		prx->tars_ping();
//		LOG_CONSOLE_DEBUG << "prx succ" << endl;
//
//		HelloPrx prx1 = c->stringToProxy<HelloPrx>("TestApp.SSLServer.SSL1Obj@ssl -h 127.0.0.1 -p 9006 -t 10000");
//		prx1->tars_ping();
//		LOG_CONSOLE_DEBUG << "prx1 succ" << endl;
//
//		HelloPrx prx2 = c->stringToProxy<HelloPrx>("TestApp.SSLServer.SSL2Obj@ssl -h 127.0.0.1 -p 9007 -t 10000");
//		prx2->tars_ping();
//		LOG_CONSOLE_DEBUG << "prx2 succ" << endl;
//
//		HelloPrx prx3 = c->stringToProxy<HelloPrx>("TestApp.SSLServer.SSL3Obj@ssl -h 127.0.0.1 -p 9008 -t 10000 -e 1");
//		prx3->tars_ping();
//		LOG_CONSOLE_DEBUG << "prx3 succ" << endl;
//	}
//	catch(exception &ex)
//	{
//		LOG_CONSOLE_DEBUG << ex.what() << endl;
//	}
//
//}

TEST_F(HelloTest, httpSyncServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkSyncHttp(comm, "HttpAdapter", true);
		checkSyncHttp(comm, "HttpAdapter", false);
	});
}

TEST_F(HelloTest, httpSyncServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkSyncHttp(comm, "HttpAdapter", true);
		checkSyncHttp(comm, "HttpAdapter", false);
	});
}


TEST_F(HelloTest, httpASyncServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkASyncHttp(comm, "HttpAdapter", true);
		checkASyncHttp(comm, "HttpAdapter", false);
	});
}

TEST_F(HelloTest, httpASyncServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkASyncHttp(comm, "HttpAdapter", true);
		checkASyncHttp(comm, "HttpAdapter", false);
	});
}

#if TARS_SSL

TEST_F(HelloTest, httpsSyncServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkSyncHttp(comm, "HttpsAdapter", true);
		checkSyncHttp(comm, "HttpsAdapter", false);
	});
}

TEST_F(HelloTest, httpsSyncServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkSyncHttp(comm, "HttpsAdapter", true);
		checkSyncHttp(comm, "HttpsAdapter", false);
	});
}


TEST_F(HelloTest, httpsASyncServerCommunicator)
{
	transServerCommunicator([&](Communicator *comm){
		checkASyncHttp(comm, "HttpsAdapter", true);
		checkASyncHttp(comm, "HttpsAdapter", false);
	});
}

TEST_F(HelloTest, httpsASyncServerCommunicatorInCoroutine)
{
	transInCoroutineServerCommunicator([&](Communicator *comm){
		checkASyncHttp(comm, "HttpsAdapter", true);
		checkASyncHttp(comm, "HttpsAdapter", false);
	});
}

#endif