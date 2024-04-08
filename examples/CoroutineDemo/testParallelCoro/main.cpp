﻿/**
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

#include "BServant.h"
#include "servant/Communicator.h"
//#include "servant/CoroutineScheduler.h"
#include <iostream>

using namespace std;
using namespace Test;
using namespace tars;

class BServantCoroCallback : public BServantCoroPrxCallback
{
public:
    virtual ~BServantCoroCallback(){}

    virtual void callback_testCoroSerial(tars::Int32 ret, const std::string &sOut) // override
    {
    		_iRet = ret;
    		_sOut = sOut;
    }
    virtual void callback_testCoroSerial_exception(tars::Int32 ret) // override
    {
    		_iException = ret;
    }

  virtual void callback_testCoroParallel(tars::Int32 ret, const std::string &sOut) // override
    {
    		_iRet = ret;
    		_sOut = sOut;
    }

  virtual void callback_testCoroParallel_exception(tars::Int32 ret) // override
  {
    		_iException = ret;
  }

public:
	int		_iException;
	int		_iRet;
	int		_iOut;
	string	_sOut;
};
// typedef tars::TC_AutoPtr<BServantCoroCallback> BServantCoroCallbackPtr;

////////////////////////////////////////////
//继承框架的协程类
class TestCoroutine : public TC_Coroutine
{
public:
	TestCoroutine(int iNum);

	~TestCoroutine() {}

	void handle();

private:
	int _num;
	Communicator _comm;
	BServantPrx _prx;
};

TestCoroutine::TestCoroutine(int iNum)
: _num(iNum)
{
	// _comm.setProperty("locator", "tars.tarsregistry.QueryObj@tcp -h 10.208.139.242 -p 17890 -t 10000");
    _prx = _comm.stringToProxy<BServantPrx>("TestApp.BServer.BServantObj@tcp -h 127.0.0.1 -p 9200");
	// _comm.stringToProxy(_sObj, _prx);
}

void TestCoroutine::handle()
{
	string sIn(32,'a');
	unsigned long sum = 0;

	for(int i = 0; i < _num; i++) 
	{
		try
		{
            CoroParallelBasePtr sharedPtr = new CoroParallelBase(2);

			BServantCoroPrxCallbackPtr cb1(new BServantCoroCallback());
			cb1->setCoroParallelBasePtr(sharedPtr);
			_prx->coro_testCoroSerial(cb1, sIn);

			BServantCoroPrxCallbackPtr cb2(new BServantCoroCallback());
			cb2->setCoroParallelBasePtr(sharedPtr);
			_prx->coro_testCoroParallel(cb2, sIn);

			coroWhenAll(sharedPtr);

			// cout << "ret1:" << cb1->_sOut << "|ret2:" << cb2->_sOut << endl;

			if(((BServantCoroCallback*)(cb1.get()))->_iRet == 0 && ((BServantCoroCallback*)(cb2.get()))->_iRet == 0 && ((BServantCoroCallback*)(cb1.get()))->_iException == 0 && ((BServantCoroCallback*)(cb2.get()))->_iException == 0)
			{
				++sum;
			}
		}
		catch(TC_Exception &e)
		{
			cout << "i: " << i << "exception: " << e.what() << endl;
		}
		catch(...)
		{
			cout << "i: " << i << "unknown exception." << endl;
		}

	}
	cout << "succ:" << sum <<endl;
}

int main(int argc,char ** argv)
{
	if(argc != 2)
	{
		cout << "usage: " << argv[0] << " CallTimes " << endl;
		return -1;
	}

	tars::Int32 iNum = TC_Common::strto<tars::Int32>(string(argv[1]));

	TestCoroutine testCoro(iNum);

	testCoro.setCoroInfo(10, 128, 128*1024);

	testCoro.start();
		
	testCoro.getThreadControl().join();
	
    return 0;
}
