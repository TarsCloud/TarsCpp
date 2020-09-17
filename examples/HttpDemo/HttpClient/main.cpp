

#include <iostream>
#include "util/tc_http.h"
#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "util/tc_thread_pool.h"
// #include "tup/tup.h"
#include "util/tc_timeprovider.h"
#include "servant/Application.h"

using namespace std;
using namespace tars;
// using namespace wup;

Communicator* _comm;

//static string httpObj = "TestApp.HttpServer.httpObj@tcp -h 127.0.0.1 -p 8081:tcp -h 127.0.0.1 -p 8082:tcp -h 127.0.0.1 -p 8083";
static string httpObj = "TestApp.HttpServer.httpObj@tcp -h 127.0.0.1 -p 8081";
//static string httpObj = "TestApp.HttpServer.httpObj@tcp -h 134.175.105.92 -p 8081";

struct Param
{
	int count;
	string call;
	int thread;

	ServantPrx servantPrx;
};

Param param;
std::atomic<int> callback_count(0);

void httpCall(int excut_num)
{
	int64_t _iTime = TC_TimeProvider::getInstance()->getNowMs();

	//  string sServer1("http://134.175.105.92:8081/");
	string sServer1("http://127.0.0.1:8081/");

	TC_HttpRequest stHttpReq;
	stHttpReq.setCacheControl("no-cache");
//    stHttpReq.setGetRequest(sServer1);

	TC_TCPClient client ;
	//   client.init("127.0.0.1", 8081, 3000);
	client.init("127.0.0.1", 8082, 3000);

	int iRet = 0;

	for (int i = 0; i<excut_num; i++)
	{
		TC_HttpResponse stHttpRsp;

		stHttpReq.setPostRequest(sServer1, TC_Common::tostr(i), true);
		iRet = stHttpReq.doRequest(stHttpRsp, 3000);
		//    iRet = stHttpReq.doRequest(client,stHttpRsp);

		if (iRet != 0)
		{
			cout <<"pthread id: " << TC_Thread::CURRENT_THREADID() << ", iRet:" << iRet <<endl;
		}

		++callback_count;
	}
	cout <<  "httpCall, succ:" << param.count << "/" << excut_num << ", " << TC_TimeProvider::getInstance()->getNowMs() - _iTime <<"(ms)"<<endl;
}

void syncRpc(int c)
{
	int64_t t = TC_Common::now2us();

	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		string buff = string("helloworld-") + TC_Common::tostr(i);
		shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();
		req->setPostRequest("http://tars.com/hello", buff, true);
//		req->setHeader("Connection", "keep-alive");

		shared_ptr<TC_HttpResponse> rsp;

		try
		{
			param.servantPrx->tars_hash(i)->http_call("hello", req, rsp);
			assert(req->getContent() == rsp->getContent());
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}

//		sleep(1);
//		assert(req.use_count() == 1);
//		assert(rsp.use_count() == 1);
		++callback_count;
	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "syncCall total:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}

struct TestHttpCallback : public HttpCallback
{
	TestHttpCallback(const string &buff) : _buff(buff)
	{

	}

	virtual int onHttpResponse(const shared_ptr<TC_HttpResponse> &rsp)
	{
		callback_count++;

		assert(_buff == rsp->getContent());

		return 0;
	}
	virtual int onHttpResponseException(int expCode)
	{
		cout << "onHttpResponseException expCode:" << expCode  << endl;

		callback_count++;

		return 0;
	}

	string _buff;
};

void asyncRpc(int c)
{
	int64_t t = TC_Common::now2us();

	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();

		string buff = string("helloworld-") + TC_Common::tostr(i);
		req->setPostRequest("http://tars.com/hello", buff, true);
//		req->setHeader("Connection", "keep-alive");

		HttpCallbackPtr p = new TestHttpCallback(buff);

		try
		{
			param.servantPrx->http_call_async("hello", req, p);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}

		if(i % 500 == 0)
		{
			TC_Common::msleep(100);
		}
	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "asyncRpc2 send:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}


void asyncRpcCallback(int c)
{
	int64_t t = TC_Common::now2us();

	param.servantPrx->tars_set_custom_callback([](ReqMessagePtr msg) {
		msg->callback->dispatch(msg);
	});

	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();

		string buff = string("helloworld-") + TC_Common::tostr(i);
		req->setPostRequest("http://tars.com/hello", buff, true);
//		req->setHeader("Connection", "keep-alive");

		HttpCallbackPtr p = new TestHttpCallback(buff);

		try
		{
			param.servantPrx->http_call_async("hello", req, p);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}

		if(i % 500 == 0)
		{
			TC_Common::msleep(100);
		}
	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "asyncRpc2 send:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}

int main(int argc, char *argv[])
{
	try
	{
		if (argc < 4)
		{
			cout << "Usage:" << argv[0] << "--count=1000 --call=[base|sync|async|callback] --thread=1" << endl;

			return 0;
		}

		TC_Option option;
		option.decode(argc, argv);

		param.count = TC_Common::strto<int>(option.getValue("count"));
		if(param.count <= 0) param.count = 1000;
		param.call = option.getValue("call");
		if(param.call.empty()) param.call = "sync";
		param.thread = TC_Common::strto<int>(option.getValue("thread"));
		if(param.thread <= 0) param.thread = 1;

        _comm = new Communicator();

 //      LocalRollLogger::getInstance()->logger()->setLogLevel(6);

        _comm->setProperty("sendqueuelimit", "1000000");
        _comm->setProperty("asyncqueuecap", "1000000");

		param.servantPrx = _comm->stringToProxy<ServantPrx>(httpObj);

		int64_t start = TC_Common::now2us();

		std::function<void(int)> func;

		if (param.call == "base")
		{
			func = httpCall;
		}
		else if (param.call == "sync")
		{
			func = syncRpc;
		}
		else if(param.call == "async")
		{
		    func = asyncRpc;
		}
		else if(param.call == "callback")
		{
			func = asyncRpcCallback;
		}
		else
		{
			cout << "no func, exits" << endl;
			exit(0);
		}

		param.servantPrx->tars_connect_timeout(5000);
		param.servantPrx->tars_async_timeout(60*1000);

		param.servantPrx->tars_set_protocol(ServantProxy::PROTOCOL_HTTP1, 3);

		vector<std::thread*> vt;
		for(int i = 0 ; i< param.thread; i++)
		{
			vt.push_back(new std::thread(func, param.count));
		}

		std::thread print([&]{while(callback_count != param.count * param.thread) {
			cout << "Http:" << param.call << ": ----------finish count:" << callback_count << endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		};});

		for(size_t i = 0 ; i< vt.size(); i++)
		{
			vt[i]->join();
			delete vt[i];
		}

		cout << "(pid:" << std::this_thread::get_id() << ")"
		     << "(count:" << param.count << ")"
		     << "(use ms:" << (TC_Common::now2us() - start)/1000 << ")"
		     << endl;

		while(callback_count != param.count * param.thread) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		print.join();
		cout << "----------finish count:" << callback_count << endl;

		delete _comm;
		_comm = NULL;
	}
	catch(exception &ex)
	{
		cout << ex.what() << endl;
	}
	cout << "main return." << endl;

	return 0;
}