//
// Created by jarod on 2020/2/20.
//

#include "util/tc_http.h"
#include "util/tc_common.h"
#include "util/tc_http_async.h"
#include "util/tc_md5.h"
#include "test_server.h"
#include "gtest/gtest.h"
#include "certs.h"

using namespace tars;

class UtilHttpAsyncTest : public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{
	}
	static void TearDownTestCase()
	{
	}

	virtual void SetUp()   //TEST跑之前会执行SetUp
	{
	}

	virtual void TearDown() //TEST跑完之后会执行TearDown
	{
	}

	void startServer(MyHttpServer &server, TC_EpollServer::SERVER_OPEN_COROUTINE openCoroutine)
	{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
		TC_Common::ignorePipe();
#endif
		server.initialize();
		server._epollServer->setOpenCoroutine(openCoroutine);

		server.bindHttp("tcp -h 0.0.0.0 -p 18077 -t 60000");
#if TARS_SSL
		server.bindHttpSSL("ssl -h 0.0.0.0 -p 18080 -t 60000");
		server.bindHttpSSLVerify("ssl -h 0.0.0.0 -p 18180 -t 60000");
#endif

		server.waitForShutdown();

	}

	void stopServer(MyHttpServer &server)
	{
		server.terminate();
	}

};


class AsyncHttpCallback : public TC_HttpAsync::RequestCallback
{
public:
	AsyncHttpCallback(const string &sUrl) : _sUrl(sUrl)
	{
	}
	virtual bool onContinue(TC_HttpResponse &stHttpResponse) { return true; }
	virtual void onSucc(TC_HttpResponse &stHttpResponse)
	{
		_rsp = stHttpResponse;
	}
	virtual void onFailed(FAILED_CODE ret, const string &info)
	{
		LOG_CONSOLE_DEBUG << "onFailed, code:" << ret << ", info:" << info << "," << _sUrl << endl;
	}
	virtual void onClose()
	{
//		LOG_CONSOLE_DEBUG << "onClose:" << _sUrl << endl;
	}
public:
	string _sUrl;
	TC_HttpResponse _rsp;
};

class DownloadCallback : public TC_HttpAsync::RequestCallback
{
public:
	DownloadCallback()
		: _fp(NULL)
	{
	}

	virtual void onSucc(TC_HttpResponse &stHttpResponse)
	{
		if (_fp)
		{
			fclose(_fp);
			_fp = NULL;
		}

		_md5 = TC_MD5::md5file("winrar");
//		LOG_CONSOLE_DEBUG << "onSucc:" << TC_File::getFileSize("winrar") << endl;
	};

	virtual bool onContinue(TC_HttpResponse &stHttpResponse)
	{
//		LOG_CONSOLE_DEBUG << "recv length:" << stHttpResponse.getContent().length() << endl;
		if (_fp == NULL)
		{
			string file = "winrar";
			_fp = fopen(file.c_str(), "wb");
		}

		string content = stHttpResponse.getContent();

		fwrite(content.c_str(), 1, content.length(), _fp);

		stHttpResponse.clearContent();

		return true;
	}

	virtual void onFailed(FAILED_CODE ret, const string &info) {
//		cout << "onFailed:" << ret << ", " << info << endl;
	};

	virtual void onClose()
	{
		if (_fp)
		{
			fclose(_fp);
			_fp = NULL;
		}
	}

	string _md5;
	FILE  *_fp;
};

TEST_F(UtilHttpAsyncTest, testSync)
{
	int type = 2;
	for(type = 0; type <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; type++)
	{
		MyHttpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) type);

		int i = 100;

		while (i-- > 0)
		{
			string url = "http://127.0.0.1:18077";
			TC_HttpRequest stHttpReq;
			stHttpReq.setUserAgent("E71/SymbianOS/9.1 Series60/3.0");
			stHttpReq.setHeader("Connection", "Close");
			stHttpReq.setAcceptEncoding("gzip, deflate, br");
			stHttpReq.setPostRequest(url, "test info");

			string sSendBuffer = stHttpReq.encode();

//			LOG_CONSOLE_DEBUG << sSendBuffer << endl;

			TC_HttpResponse stHttpRsp;
			int iRet = stHttpReq.doRequest(stHttpRsp, 5000);

			ASSERT_TRUE(iRet == 0);
			ASSERT_TRUE(stHttpRsp.getContent() == stHttpReq.getContent());
		}

		stopServer(server);
	}

}

TEST_F(UtilHttpAsyncTest, testAsync)
{
	int type = 2;

	for(type = 0; type <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; type++)
	{
		MyHttpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) type);

		string url = "http://127.0.0.1:18077";
		TC_HttpRequest stHttpReq;
		stHttpReq.setUserAgent("E71/SymbianOS/9.1 Series60/3.0");
		stHttpReq.setHeader("Connection", "Close");
		stHttpReq.setAcceptEncoding("gzip, deflate, br");
		stHttpReq.setPostRequest(url, string("test info"), true);

		TC_HttpAsync ast;
		ast.setTimeout(10000);
		ast.start();

		int i = 100;
		while (i-- > 0)
		{
			AsyncHttpCallback *callback = new AsyncHttpCallback(url);

			TC_HttpAsync::RequestCallbackPtr p(callback);

			ast.doAsyncRequest(stHttpReq, p);

			TC_Common::msleep(50);

			ASSERT_TRUE(callback->_rsp.getContent() == stHttpReq.getContent());
		}

		ast.waitForAllDone();

		stopServer(server);

	}

}

TEST_F(UtilHttpAsyncTest, testDownload)
{
	int type = 2;

	for(type = 0; type <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; type++) {
		MyHttpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) type);

		TC_HttpAsync ast;
		ast.setTimeout(10000);
		ast.start();

		string url = "http://cdn.tarsyun.com/src/release-1.10.0.tar.gz";
		TC_HttpRequest stHttpReq;
		stHttpReq.setGetRequest(url);

		DownloadCallback *callback = new DownloadCallback();

		TC_HttpAsync::RequestCallbackPtr p(callback);

		ast.doAsyncRequest(stHttpReq, p);

		ast.waitForAllDone();

		ASSERT_TRUE(callback->_md5 == "ecd1fa65e7de707cd5c00bdac56022cd");

		stopServer(server);

	}
}

#if TARS_SSL
TEST_F(UtilHttpAsyncTest, testAsyncSSL)
{
	int type = 2;

	for(type = 0; type <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; type++)
	{
		MyHttpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) type);

		string url = "https://127.0.0.1:18080";
		TC_HttpRequest stHttpReq;
		stHttpReq.setUserAgent("E71/SymbianOS/9.1 Series60/3.0");
		stHttpReq.setHeader("Connection", "Close");
		stHttpReq.setAcceptEncoding("gzip, deflate, br");
		stHttpReq.setPostRequest(url, string("test info"), true);

		TC_HttpAsync ast;
		ast.setTimeout(10000);
		ast.start();

		int i = 100;
		while (i-- > 0)
		{
			AsyncHttpCallback *callback = new AsyncHttpCallback(url);

			TC_HttpAsync::RequestCallbackPtr p(callback);

			ast.doAsyncRequest(stHttpReq, p);

			TC_Common::msleep(50);

			ASSERT_TRUE(callback->_rsp.getContent() == stHttpReq.getContent());
		}

		ast.waitForAllDone();

		stopServer(server);
	}
}


TEST_F(UtilHttpAsyncTest, testAsyncSSLCtxVerify)
{
	string ciphers = "";

	bool verifyClient = true;

	shared_ptr<TC_OpenSSL::CTX> ctx = TC_OpenSSL::newCtx(SERVER_CERT, CLIENT_CERT, CLIENT_KEY, verifyClient, ciphers);

	int type = 2;

	for(type = 0; type <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; type++)
	{
		MyHttpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) type);

		string url = "https://127.0.0.1:18180";
		TC_HttpRequest stHttpReq;
		stHttpReq.setUserAgent("E71/SymbianOS/9.1 Series60/3.0");
		stHttpReq.setHeader("Connection", "Close");
		stHttpReq.setAcceptEncoding("gzip, deflate, br");
		stHttpReq.setPostRequest(url, string("test info"), true);

		TC_HttpAsync ast;
		ast.setCtx(ctx);
		ast.setTimeout(10000);
		ast.start();

		int i = 100;
		while (i-- > 0)
		{
			AsyncHttpCallback *callback = new AsyncHttpCallback(url);

			TC_HttpAsync::RequestCallbackPtr p(callback);

			ast.doAsyncRequest(stHttpReq, p);

			TC_Common::msleep(50);

			ASSERT_TRUE(callback->_rsp.getContent() == stHttpReq.getContent());
		}

		ast.waitForAllDone();

		stopServer(server);
	}
}

TEST_F(UtilHttpAsyncTest, testAsyncSSLCtx)
{
	bool verifyClient = false;
	string ciphers = "";

	shared_ptr<TC_OpenSSL::CTX> ctx = TC_OpenSSL::newCtx(SERVER_CERT, "", "", verifyClient, ciphers);

	int type = 2;

	for(type = 0; type <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; type++)
	{
		MyHttpServer server;

		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) type);

		string url = "https://127.0.0.1:18080";
		TC_HttpRequest stHttpReq;
		stHttpReq.setUserAgent("E71/SymbianOS/9.1 Series60/3.0");
		stHttpReq.setHeader("Connection", "Close");
		stHttpReq.setAcceptEncoding("gzip, deflate, br");
		stHttpReq.setPostRequest(url, string("test info"), true);

		TC_HttpAsync ast;
		ast.setCtx(ctx);
		ast.setTimeout(10000);
		ast.start();

		int i = 100;
		while (i-- > 0)
		{
			AsyncHttpCallback *callback = new AsyncHttpCallback(url);

			TC_HttpAsync::RequestCallbackPtr p(callback);

			ast.doAsyncRequest(stHttpReq, p);

			TC_Common::msleep(50);

			ASSERT_TRUE(callback->_rsp.getContent() == stHttpReq.getContent());
		}

		ast.waitForAllDone();

		stopServer(server);
	}
}

TEST_F(UtilHttpAsyncTest, testRemoteAsyncSSL)
{
	string url = "https://www.qq.com";
	TC_HttpRequest stHttpReq;
	stHttpReq.setUserAgent("E71/SymbianOS/9.1 Series60/3.0");
	stHttpReq.setHeader("Connection", "Close");
	stHttpReq.setAcceptEncoding("gzip, deflate, br");
	stHttpReq.setGetRequest(url);

	TC_HttpAsync ast;
	ast.setTimeout(10000);
	ast.start();

	int i = 100;
	while (i-- > 0)
	{
		AsyncHttpCallback *callback = new AsyncHttpCallback(url);

		TC_HttpAsync::RequestCallbackPtr p(callback);

		ast.doAsyncRequest(stHttpReq, p);

		TC_Common::msleep(50);

		ASSERT_TRUE(callback->_rsp.getStatus() == 200);
	}

	ast.waitForAllDone();

}

#endif
