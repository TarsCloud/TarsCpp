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
#include "ssl.h"
#include "util/tc_option.h"

using namespace std;
using namespace tars;
using namespace TestApp;

Communicator* _comm;

static string helloObj = "TestApp.SSLServer.SSLObj@ssl -h 127.0.0.1 -p 9005";
static string hello1Obj = "TestApp.SSLServer.SSL1Obj@ssl -h 127.0.0.1 -p 9006";
static string hello2Obj = "TestApp.SSLServer.SSL2Obj@ssl -h 127.0.0.1 -p 9007";
static string hello3Obj = "TestApp.SSLServer.SSL3Obj@ssl -h 127.0.0.1 -p 9008 -e 1";

struct Param
{
	int count;
	string call;
	int thread;
	int buffersize;
	int netthread;

	HelloPrx pPrx;
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
	        param.pPrx->testHello(buffer, r);
        }
        catch(exception& e)
        {
            cout << "exception:" << e.what() << endl;
        }
        ++callback_count;

    }

    int64_t cost = TC_Common::now2us() - t;
    cout << "syncCall  total:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
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
			param.pPrx->async_testHello(p, buffer);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}

		if(i % 1000 == 0)
			TC_Common::msleep(100);
	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "asyncCall send:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}


int main(int argc, char *argv[])
{
    try
    {
        if (argc < 6)
        {
	        cout << "Usage:" << argv[0] << "--config=conf --count=1000 --call=[sync|async|sync1|async1|sync2|async2] --thread=1 --buffersize=1000 --netthread=1" << endl;

	        return 0;
        }

	    TC_Option option;
        option.decode(argc, argv);

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

        TC_Config conf;
        conf.parseFile(option.getValue("config"));
	    _comm->setProperty(conf);

//        LocalRollLogger::getInstance()->logger()->setLogLevel(6);

        _comm->setProperty("sendqueuelimit", "1000000");
        _comm->setProperty("asyncqueuecap", "1000000");

	    _comm->setProperty("netthread", TC_Common::tostr(param.netthread));

        int64_t start = TC_Common::now2us();

        std::function<void(int)> func;

        if (param.call == "sync")
        {
            func = syncCall;
	        param.pPrx = _comm->stringToProxy<HelloPrx>(helloObj);
        }
        else if (param.call == "async")
        {
            func = asyncCall;
	        param.pPrx = _comm->stringToProxy<HelloPrx>(helloObj);
        }
	    else if (param.call == "sync1")
	    {
		    func = syncCall;
		    param.pPrx = _comm->stringToProxy<HelloPrx>(hello1Obj);
	    }
	    else if (param.call == "async1")
	    {
		    func = asyncCall;
		    param.pPrx = _comm->stringToProxy<HelloPrx>(hello1Obj);
	    }
        else if (param.call == "sync2")
        {
	        func = syncCall;
	        param.pPrx = _comm->stringToProxy<HelloPrx>(hello2Obj);
        }
        else if (param.call == "async2")
        {
	        func = asyncCall;
	        param.pPrx = _comm->stringToProxy<HelloPrx>(hello2Obj);
        }
        else if (param.call == "sync3")
        {
	        func = syncCall;
	        param.pPrx = _comm->stringToProxy<HelloPrx>(hello3Obj);
        }
        else if (param.call == "async3")
        {
	        func = asyncCall;
	        param.pPrx = _comm->stringToProxy<HelloPrx>(hello3Obj);
        }
        else
        {
        	cout << "no func, exits:" << param.call << endl;
        	exit(0);
        }

	    param.pPrx->tars_connect_timeout(5000);
	    param.pPrx->tars_async_timeout(60*1000);

	    vector<std::thread*> vt;
        for(int i = 0 ; i< param.thread; i++)
        {
            vt.push_back(new std::thread(func, param.count));
        }

        std::thread print([&]{while(callback_count != param.count * param.thread) {
	        cout << "SSL:" << param.call << ": ----------finish count:" << callback_count << endl;
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
