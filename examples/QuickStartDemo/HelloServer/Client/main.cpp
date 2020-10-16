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
#include "servant/Communicator.h"
#include "Hello.h"
#include "util/tc_option.h"

using namespace std;
using namespace tars;
using namespace TestApp;

Communicator* _comm;

static string helloObj = "TestApp.HelloServer.HelloObj@tcp -h 127.0.0.1 -p 8999";
//static string helloObj = "TestApp.HelloServer.HelloObj@tcp -h 127.0.0.1 -p 8199:tcp -h 127.0.0.1 -p 8299 -t 10000";

struct Param
{
	int count;
	string call;
	int thread;
	int buffersize;
	int netthread;
	bool hash = false;

	HelloPrx pPrx;
	ServantPrx servantPrx;
};

Param param;
std::atomic<int> callback_count(0);

struct HelloCallback : public HelloPrxCallback
{
    HelloCallback(int64_t t, int i, int c) : start(t), cur(i), count(c)
    {

    }

    //call back
    virtual void callback_testHello(int ret, const string &r)
    {
        assert(ret == 0);
	    callback_count++;

        if(cur == count-1)
        {
            int64_t cost = TC_Common::now2us() - start;
            cout << "callback_testHello count:" << count << ", " << cost << " us, avg:" << 1.*cost/count << "us" << endl;
        }
    }

    virtual void callback_testHello_exception(tars::Int32 ret)
    {
        cout << "callback exception:" << ret << endl;
    }

    int64_t start;
    int     cur;
    int     count;
};


void syncCall(int c)
{
	string buffer(param.buffersize, 'a');

	int64_t t = TC_Common::now2us();
    //发起远程调用
    for (int i = 0; i < c; ++i)
    {
        string r;

        try
        {
			if(param.hash) {
				param.pPrx->tars_hash(i)->testHello(buffer + "-" + TC_Common::tostr(i), r);
			} else {
				param.pPrx->testHello(buffer, r);
			}
        }
        catch(exception& e)
        {
            cout << "exception:" << e.what() << endl;
        }
        ++callback_count;
    }

    int64_t cost = TC_Common::now2us() - t;
    cout << "syncCall total:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}


void asyncCall(int c)
{
	int64_t t = TC_Common::now2us();

	string buffer(param.buffersize, 'a');

	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		HelloPrxCallbackPtr p = new HelloCallback(t, i, c);

		try
		{
			if(param.hash) {
				param.pPrx->tars_hash(i)->async_testHello(p, buffer + "-" + TC_Common::tostr(i));
			} else {
				param.pPrx->async_testHello(p, buffer);
			}
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}
	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "asyncCall send:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}

void syncTupCall(int c)
{
	string buffer(param.buffersize, 'a');

	TarsUniPacket<> req;

    req.setTarsVersion(TUPVERSION);
	req.setServantName("TestApp.HelloServer.HelloObj");
	req.setFuncName("testHello");

	req.put<string>("sReq", buffer);

	ProxyProtocol proto;
	proto.requestFunc = ProxyProtocol::streamRequest;
	proto.responseFunc = ProxyProtocol::tupResponse;

	param.pPrx->tars_set_protocol(proto);
	int64_t t = TC_Common::now2us();
	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		string r;

		try
		{
			req.setRequestId(param.pPrx->tars_gen_requestid());

			vector<char> buff;
			req.encode(buff);

			ResponsePacket package;

			param.pPrx->rpc_call(req.getRequestId(), "testHello", buff.data(), buff.size(), package);

            TarsUniPacket<> rsp;

            rsp.decode(package.sBuffer.data(), package.sBuffer.size());

            int ret = rsp.get<int>("");            
            string sRsp = rsp.get<string>("sRsp");

			// assert(ret == 0);
			// assert(sRsp == buffer);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}
        ++callback_count;

	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "syncWupCall total:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}


struct TupCallback : public ServantProxyCallback
{
    TupCallback(int64_t t, int i, int c) : start(t), cur(i), count(c)
    {

    }

    virtual int onDispatch(ReqMessagePtr msg)
	{
		if(msg->response->iRet != tars::TARSSERVERSUCCESS)
		{
			cout << "ret error:" << msg->response->iRet << endl;
		}
		else
		{
			callback_count++;

			TarsUniPacket<> rsp;

			rsp.decode(msg->response->sBuffer.data(), msg->response->sBuffer.size());

			int ret = rsp.get<int>("");            
			string sRsp = rsp.get<string>("sRsp");

			if(cur == count-1)
			{
				int64_t cost = TC_Common::now2us() - start;
				cout << "TupCallback count:" << count << ", " << cost << " us, avg:" << 1.*cost/count << "us" << endl;
			}        

			return 0;
		}

		return msg->response->iRet;
	}

    int64_t start;
    int     cur;
    int     count;
};

void asyncTupCall(int c)
{
	string buffer(param.buffersize, 'a');

	TarsUniPacket<> req;

    req.setTarsVersion(TUPVERSION);
	req.setServantName("TestApp.HelloServer.HelloObj");
	req.setFuncName("testHello");

	req.put<string>("sReq", buffer);

	ProxyProtocol proto;
	proto.requestFunc = ProxyProtocol::streamRequest;
	proto.responseFunc = ProxyProtocol::tupResponse;

	param.pPrx->tars_set_protocol(proto);
	int64_t t = TC_Common::now2us();
	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		string r;

		try
		{
			req.setRequestId(param.pPrx->tars_gen_requestid());

			vector<char> buff;
			req.encode(buff);

		    ServantProxyCallbackPtr p = new TupCallback(t, i, c);

			param.pPrx->rpc_call_async(req.getRequestId(), "testHello", buff.data(), buff.size(), p);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}

	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "syncWupCall send:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 6)
        {
	        cout << "Usage:" << argv[0] << "--count=1000 --call=[sync|async|synctup|asynctup] --thread=1 --buffersize=1000 --netthread=1" << endl;

	        return 0;
        }

	    TC_Option option;
        option.decode(argc, argv);

	    param.hash = TC_Common::strto<bool>(option.getValue("hash"));
		param.count = TC_Common::strto<int>(option.getValue("count"));
	    if(param.count <= 0) param.count = 1000;
	    param.buffersize = TC_Common::strto<int>(option.getValue("buffersize"));
	    if(param.buffersize <= 0) param.buffersize = 1000;
	    param.call = option.getValue("call");
	    if(param.call.empty()) param.call = "sync";
	    param.thread = TC_Common::strto<int>(option.getValue("thread"));
	    if(param.thread <= 0) param.thread = 1;
	    param.netthread = TC_Common::strto<int>(option.getValue("netthread"));
	    if(param.netthread <= 0) param.netthread = 1;

        _comm = new Communicator();

        _comm->setProperty("sendqueuelimit", "1000000");
        _comm->setProperty("asyncqueuecap", "1000000");

	    _comm->setProperty("netthread", TC_Common::tostr(param.netthread));

//        LocalRollLogger::getInstance()->logger()->setLogLevel(6);

	    param.pPrx = _comm->stringToProxy<HelloPrx>(helloObj);

	    param.pPrx->tars_connect_timeout(5000);
        param.pPrx->tars_async_timeout(60*1000);
        param.pPrx->tars_ping();

        int64_t start = TC_Common::now2us();

        std::function<void(int)> func;

        if (param.call == "sync")
        {
            func = syncCall;
        }
        else if (param.call == "async")
        {
            func = asyncCall;
        }
        else if(param.call == "synctup")
        {
        	func = syncTupCall;
        }
        else if(param.call == "asynctup")
        {
	        func = asyncTupCall;
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
	        cout << "Hello:" << param.call << ": ----------finish count:" << callback_count << endl;
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
