/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include <iostream>
#include "util/tc_http.h"
#include "util/tc_option.h"
#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "util/tc_thread_pool.h"
#include "util/tc_timeprovider.h"
#include "servant/Application.h"
using namespace std;
using namespace tars;

Communicator* _comm;

static string http2Obj = "TestApp.Http2Server.http2Obj@tcp -h 127.0.0.1 -p 8082";

struct Param
{
	int count;
	string call;
	int thread;

	ServantPrx servant2Prx;
};

Param param;
std::atomic<int> callback_count(0);

void syncRpc2(int c)
{
	int64_t t = TC_Common::now2us();

    //发起远程调用
    for (int i = 0; i < c; ++i)
    {
	    shared_ptr<TC_HttpResponse> rsp;
	    shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();
	    req->setPostRequest("http://domain.com/hello", string("helloworld-") + TC_Common::tostr(i), true);

	    try
	    {
		    param.servant2Prx->http_call("hello", req, rsp);
	    }
	    catch (exception & e)
	    {
		    cout << "exception:" << e.what() << endl;
	    }

	    assert(req->getContent() == rsp->getContent());
	    assert(req.use_count() == 1);
	    assert(rsp.use_count() == 1);

        ++callback_count;
    }

    int64_t cost = TC_Common::now2us() - t;
    cout << "syncRpc2 total:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
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

void asyncRpc2(int c)
{
	int64_t t = TC_Common::now2us();

	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();

		string buff = string("helloworld-") + TC_Common::tostr(i);
		req->setPostRequest("http://domain.com/hello", buff, true);

		HttpCallbackPtr p = new TestHttpCallback(buff);

		try
		{
			param.servant2Prx->http_call_async("hello", req, p);
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
	        cout << "Usage:" << argv[0] << "--count=1000 --call=[sync|async] --thread=1" << endl;

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

//         TarsRollLogger::getInstance()->logger()->setLogLevel(6);

        _comm->setProperty("sendqueuelimit", "1000000");
        _comm->setProperty("asyncqueuecap", "1000000");

        param.servant2Prx = _comm->stringToProxy<ServantPrx>(http2Obj);

	    param.servant2Prx->tars_connect_timeout(5000);
        param.servant2Prx->tars_async_timeout(60*1000);

        param.servant2Prx->tars_set_protocol(ServantProxy::PROTOCOL_HTTP2);

        int64_t start = TC_Common::now2us();

        std::function<void(int)> func;

        if (param.call == "sync")
        {
            func = syncRpc2;
        }
        else if(param.call == "async")
        {
        	func = asyncRpc2;
        }
        else
        {
        	cout << "no func, exits" << endl;
        	exit(0);
        }

	    vector<std::thread*> vt;
        for(int i = 0 ; i< param.thread; i++)
        {
            vt.push_back(new std::thread(func, param.count));
        }

        std::thread print([&]{while(callback_count != param.count * param.thread) {
	        cout << "Http2:" << param.call << ": ----------finish count:" << callback_count << endl;
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
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
    cout << "main return." << endl;

    return 0;
}