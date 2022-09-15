
#include "server/HelloServer.h"
#include "server/Hello.h"
#include "gtest/gtest.h"
#include "certs.h"

#include <mutex>
#include <iostream>

using namespace tars;
using namespace Test;

class PushCallBack : public ServantProxyCallback
{
public:

	int onDispatch(ReqMessagePtr msg)
	{
		if(msg->request.sFuncName == "printResult")
		{
			string sRet;
			sRet.assign(&(msg->response->sBuffer[0]), msg->response->sBuffer.size());
			_onprintresult = true;
//			printResult(msg->request.iRequestId, sRet);
			return 0;
		}
		else if(msg->response->iRequestId == 0)
		{
			string sRet;
			sRet.assign(&(msg->response->sBuffer[0]), msg->response->sBuffer.size());
			++_onpushinfo;
//			printPushInfo(sRet);
			return 0;
		}
		else
		{
			LOG_CONSOLE_DEBUG << "no match func!" <<endl;
		}
		return -3;
	}

	void onConnect(const TC_Endpoint &ep)
	{
		_onconnect = true;
//		LOG_CONSOLE_DEBUG << "onConnect:" << ep.toString() << ", " << _onconnect << ", " << this << endl;
	}

	void onClose()
	{
		_onclose = true;
//		LOG_CONSOLE_DEBUG << "onClose:" << _onclose << endl;
	}

	bool _onclose = false;
	bool _onconnect = false;
	bool _onprintresult = false;
	int  _onpushinfo = 0;

};

typedef tars::TC_AutoPtr<PushCallBack> PushCallBackPtr;

struct ClientHelloCallback : public HelloPrxCallback
{
	ClientHelloCallback(int64_t t, int i, int c, const string &buff, std::atomic<int> &count) : start(t), cur(i), count(c), buffer(buff), callback_count(count)
	{
	}

	ClientHelloCallback(HelloPrx prx, std::atomic<int> &count) : _prx(prx), callback_count(count)
	{
	}

	//回调函数
	virtual void callback_testTrans(int ret, const string &r);

	//回调函数
	virtual void callback_testHello(int ret, const string &r);

	virtual void callback_testSyncTrans(tars::Int32 ret,  const std::string& r);

	virtual void callback_testHello_exception(tars::Int32 ret)
	{
		LOG_CONSOLE_DEBUG << "callback exception:" << ret << endl;
		exit(-1);
	}

	virtual void callback_testTrans_exception(tars::Int32 ret)
	{
		LOG_CONSOLE_DEBUG << "callback exception:" << ret << endl;
		exit(-1);
	}

	virtual void callback_testSyncTrans_exception(tars::Int32 ret)
	{
		LOG_CONSOLE_DEBUG << "callback exception:" << ret << endl;
		exit(-1);
	}

	virtual void callback_testTimeout(int ret)
	{
		callback = true;
	}

	virtual void callback_testTimeout_exception(tars::Int32 ret)
	{
		callback_exception = true;
	}
	HelloPrx _prx;

	bool callback = false;
	bool callback_exception = false;
	int64_t start;
	int     cur;
	int     count;
	string  buffer;
	std::atomic<int> &callback_count;

};


struct HelloHttpCallback : public HttpCallback
{
	HelloHttpCallback(int64_t t, int i, int c, const string &buff, std::atomic<int> &count) : start(t), cur(i), count(c),_buff(buff), callback_count(count)
	{

	}

	virtual int onHttpResponse(const shared_ptr<TC_HttpResponse> &rsp)
	{
		[&]()
		{
			ASSERT_TRUE(rsp->getContent() == _buff);
		}();

		callback_count++;

		if(cur == count-1)
		{
			int64_t cost = TC_Common::now2us() - start;
			LOG_CONSOLE_DEBUG << "count:" << count << ", " << cost << " us, avg:" << 1.*cost/count << "us" << endl;
		}
		return 0;
	}

	virtual int onHttpResponseException(int expCode)
	{
		LOG_CONSOLE_DEBUG << "onHttpResponseException expCode:" << expCode << endl;
		return 0;
	}

	int64_t start;
	int     cur;
	int     count;
	string _buff;
	std::atomic<int> &callback_count;

};

class CustomCallBack : public ServantProxyCallback
{
public:

	CustomCallBack(std::atomic<int> &count) : callback_count(count) {}

	virtual int onDispatch(ReqMessagePtr msg)
	{
		if(msg->response->iRet != tars::TARSSERVERSUCCESS)
		{
			cout << "ret error:" << msg->response->iRet << endl;
		}
		else
		{
//			cout << "succ:" << string(msg->response->sBuffer.data(), msg->response->sBuffer.size()) << endl;
		}

		++callback_count;
		return msg->response->iRet;
	}

	virtual void onClose()
	{

	}

	std::atomic<int> &callback_count;
};

typedef tars::TC_AutoPtr<CustomCallBack> CustomCallBackPtr;

class HelloTest : public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{

	}
	static void TearDownTestCase()
	{
	}

	virtual void SetUp()
	{
		_buffer.assign(100, 'a');

		_conf = CONFIG();

		TC_File::removeFile("RpcServer.tarsdat", false);
	}
	virtual void TearDown()
	{
		LOG_CONSOLE_DEBUG << "fd count:" << getFdCounts() << endl;
	}

	template<typename T>
	void startServer(T &server, TC_Config conf, TC_EpollServer::SERVER_OPEN_COROUTINE openCoroutine = TC_EpollServer::NET_THREAD_MERGE_HANDLES_THREAD)
	{
		conf.set("/tars/application/server<opencoroutine>", TC_Common::tostr(openCoroutine));

		ASSERT_TRUE(conf.get("/tars/application/server<opencoroutine>") == TC_Common::tostr(openCoroutine));

		server.main(conf.tostr());

		server.start();

		server.waitForReady();
	}

	void startServer(HelloServer &server, TC_EpollServer::SERVER_OPEN_COROUTINE openCoroutine);

	template<typename T>
	void stopServer(T &server)
	{
		server.terminate();
		server.getThreadControl().join();
	}

	TC_Endpoint getEndpoint(const string &adapter);

	string getObj(TC_Config &conf, const string &adapter)
	{
		return conf.get("/tars/application/server/" + adapter + "<servant>") + "@" + conf.get("/tars/application/server/" + adapter + "<endpoint>");
	}

	TC_Endpoint getEndpoint(TC_Config &conf, const string &adapter)
	{
		return TC_Endpoint(conf.get("/tars/application/server/" + adapter + "<endpoint>"));
	}

	TC_Endpoint getLocalEndpoint(const TC_Config &conf)
	{
		return TC_Endpoint(conf.get("/tars/application/server<local>"));
	}

	template<typename T>
	T getObj(Communicator *comm, const string &adapter)
	{
		comm->setProperty("sendqueuelimit", "1000000");
//		comm->setProperty("asyncqueuecap", "1000000");

		string obj = getObj(_conf, adapter);
		T prx =  comm->stringToProxy<T>(obj);

		prx->tars_timeout(60000);
		prx->tars_async_timeout(60000);

		return prx;
	}

	int getFdCounts()
	{
		//linux下才生效, 其他平台都是返回的0
		vector<string> fds;
		TC_File::listDirectory("/proc/" + TC_Common::tostr(getpid()) + "/fd", fds, false);
		return fds.size();
	}

	int getSocketFd(int iSocketType = SOCK_DGRAM, int iDomain = AF_INET)
	{
		TC_Socket s1;
		s1.createSocket(iSocketType, AF_INET);
		s1.setOwner(false);

		return s1.getfd();
	}

	void waitForFinish(atomic<int> &callback_count, int count);

	void wait(int millseconds);

	void syncCustom(Communicator *comm, const string &adapter = "CustomAdapter");

	void asyncCustom(Communicator *comm, const string &adapter = "CustomAdapter");

	void testPush(Communicator *comm, const string &adapter = "PushAdapter");

	void testReconnect(Communicator *comm, const string &adapter = "PushAdapter");

	void checkWup();

	void checkSyncHttp(Communicator *comm, const string &adapter = "HttpAdapter", bool close = false);

	void checkASyncHttp(Communicator *comm, const string &adapter = "HelloAdapter", bool close = false);

	void checkSync(Communicator *comm, const string &adapter = "HelloAdapter");

	void checkSyncOnce(HelloPrx prx);

	void checkASyncOnce(HelloPrx prx);//Communicator *comm, const string &adapter = "HelloAdapter");

	void checkASync(Communicator *comm, const string &adapter = "HelloAdapter");

	void checkTransSyncASync(Communicator *comm);

	void checkTransASyncASync(Communicator *comm);

	void checkTransSyncSync(Communicator *comm);

	void checkTransASyncSync(Communicator *comm);

	void checkWupTransSync(Communicator *comm);

	void checkSyncTimeout(Communicator *comm);

	void checkASyncTimeout(Communicator *comm);

	void checkStat(Communicator *comm, int reportCount);

	void rpcFromRegistry(Communicator *comm);

	void rpcConHashFromRegistry(Communicator *comm);

	void forEach(function<void()> func);

	void forEach(function<void(Communicator *comm)> func);

	void transGlobalCommunicator(function<void(Communicator *)> func, Communicator *comm);

	void transServerCommunicator(function<void(Communicator *)> func);

	void transAllocCommunicator(function<void(Communicator *)> func);

	void transComplexCommunicator(function<void(Communicator *)> func, Communicator *comm);

	void funcInCoroutine(function<void()> func, bool setSched = true);

	void forEachInCoroutine(function<void(Communicator *comm)> func);

	void transInCoroutineGlobalCommunicator(function<void(Communicator *)> func, Communicator *comm);

	void transInCoroutineServerCommunicator(function<void(Communicator *)> func);

	void transInCoroutineAllocCommunicator(function<void(Communicator *)> func);

	void transInCoroutineComplexCommunicator(function<void(Communicator *)> func, Communicator *comm);

	shared_ptr<Communicator> getCommunicator();

protected:
	int    _count = 1000;
	// int    _count = 10;

	string _buffer;

	TC_Config _conf;
};

