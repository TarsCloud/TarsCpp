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
#include "helloworld.pb.h"
#include "util/tc_grpc.h"

using namespace std;
using namespace tars;

Communicator* _comm;

string grpcObj = "TestApp.GrpcServer.GrpcObj@";

struct Param
{
	int count;
	string call;
	int thread;
	string domain;

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
		helloworld::HelloRequest request;
		helloworld::HelloReply reply;
		request.set_name(string("world-") + TC_Common::tostr(i));
		string message = request.SerializeAsString();
		addGrpcPrefix(message, false);
		std::string url = "http://" + param.domain;
		req->setPostRequest(url, message, true);
		req->setHeader("content-type", "application/grpc");
		req->setHeader("te", "trailers");
		req->setHeader(":path", "/helloworld.Greeter/SayHello");
		std::string content;
		bool compressed;

	    try
	    {
		    param.servant2Prx->http_call("SayHello", req, rsp);
			content =  rsp->getContent();
			
			RemoveGrpcPrefix(content, &compressed);
			reply.ParseFromString(content);
			cout << "rsp content: " << reply.message() << endl;
	    }
	    catch (exception & e)
	    {
		    cout << "exception:" << e.what() << endl;
	    }

		std::string rspMessage = "Hello " + request.name();
	    assert(rspMessage == reply.message());
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
		helloworld::HelloRequest request;
		helloworld::HelloReply reply;
		std::string content;
		bool compressed;
		content =  rsp->getContent();

		RemoveGrpcPrefix(content, &compressed);
		RemoveGrpcPrefix(_buff, &compressed);
		request.ParseFromString(_buff);
		reply.ParseFromString(content);

		std::string rspMessage = "Hello " + request.name();
	    assert(rspMessage == reply.message());

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
		shared_ptr<TC_HttpResponse> rsp;
	    shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();
		helloworld::HelloRequest request;
		helloworld::HelloReply reply;
		request.set_name(string("world-") + TC_Common::tostr(i));
		string message = request.SerializeAsString();
		addGrpcPrefix(message, false);
		std::string url = "http://" + param.domain;
		req->setPostRequest(url, message, true);
		req->setHeader("content-type", "application/grpc");
		req->setHeader(":path", "/helloworld.Greeter/SayHello");
		req->setHeader("te", "trailers");


		HttpCallbackPtr p = new TestHttpCallback(message);

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
	        cout << "Usage:" << argv[0] << " --domain=127.0.0.1:50051 --count=1000 --call=[sync|async] --thread=1" << endl;

	        return 0;
        }

	    TC_Option option;
        option.decode(argc, argv);

		param.domain = option.getValue("domain");
		if(param.domain.empty()) param.domain = "127.0.0.1:50051";
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

		string ip = param.domain.substr(0, param.domain.find(":"));
		string port = param.domain.substr(param.domain.find(":") + 1);
		grpcObj += "tcp -h " + ip + " -p " + port;
		std::cout << "grpcObj: " << grpcObj << std::endl;
        param.servant2Prx = _comm->stringToProxy<ServantPrx>(grpcObj);

	    param.servant2Prx->tars_connect_timeout(5000);
        param.servant2Prx->tars_async_timeout(60*1000);

        param.servant2Prx->tars_set_protocol(ServantProxy::PROTOCOL_GRPC);

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
	        cout << "Grpc:" << param.call << ": ----------finish count:" << callback_count << endl;
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