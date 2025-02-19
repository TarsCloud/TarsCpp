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

#include "servant/AsyncProcThread.h"
#include "servant/Communicator.h"
#include "servant/StatReport.h"
#include "servant/RemoteLogger.h"
#include "servant/AdapterProxy.h"

namespace tars
{

AsyncProcThread::AsyncProcThread(size_t iQueueCap, bool merge)
: _terminate(false), _iQueueCap(iQueueCap), _merge(merge)
{
	 _msgQueue = new TC_CasQueue<ReqMessage*>();

	 if(!_merge)
	 {
	 	start();
	 }
}

AsyncProcThread::~AsyncProcThread()
{
    terminate();

	if(_msgQueue)
	{
		delete _msgQueue;
		_msgQueue = NULL;
	}
}

void AsyncProcThread::terminate()
{
	if(!_merge) {
		Lock lock(*this);

		_terminate = true;

		notifyAll();
	}
}

void AsyncProcThread::push_back(ReqMessage * msg)
{
	if(_merge) {
		//合并了, 直接回调
		callback(msg);
	}
	else {
		if(_msgQueue->size() >= _iQueueCap)
		{
			TLOGERROR("[AsyncProcThread::push_back] async_queue full:" << _msgQueue->size() << ">=" << _iQueueCap << endl);
			delete msg;
		}
		else
		{
			_msgQueue->push_back(msg);

			TC_ThreadLock::Lock lock(*this);
			notify();
		}
	}
}

void AsyncProcThread::run()
{
    while (!_terminate)
    {
        ReqMessage * msg;

        //异步请求回来的响应包处理
        if (_msgQueue->pop_front(msg))
        {
	        callback(msg);
        }
		else
		{
		    TC_ThreadLock::Lock lock(*this);
	     	timedWait(1000);
		}
    }

	ReqMessage * msg;
	while(_msgQueue->pop_front(msg))
	{
    	delete msg;
	}

	ServantProxyThreadData::reset();
	CallbackThreadData::reset();
}

void AsyncProcThread::callback(ReqMessage * msg)
{
	TLOGTARS("[AsyncProcThread::run] get one msg." << endl);

	//从回调对象把线程私有数据传递到回调线程中
	ServantProxyThreadData * pServantProxyThreadData = ServantProxyThreadData::getData();
//	assert(pServantProxyThreadData != NULL);

	//把染色的消息设置在线程私有数据里面
	pServantProxyThreadData->_data._dyeing  = msg->data._dyeing;
	pServantProxyThreadData->_data._dyeingKey = msg->data._dyeingKey;

	// 如果开启了消息染色需要主动 enableDyeing
	TarsDyeingSwitch dye;
	if (msg->data._dyeing)
	{
		dye.enableDyeing(msg->data._dyeingKey);
	}

	pServantProxyThreadData->_data._cookie = msg->data._cookie;
	pServantProxyThreadData->_traceCall = msg->bTraceCall;
	pServantProxyThreadData->initTrace(msg->sTraceKey);

	if(msg->adapter)
	{
		pServantProxyThreadData->_data._szHost = msg->adapter->endpoint().desc();
	}

	try
	{
		ReqMessagePtr msgPtr = msg;

		msg->callback->dispatch(msgPtr);
	}
	catch (exception& e)
	{
		TLOGERROR("[AsyncProcThread exception]:" << e.what() << endl);
	}
	catch (...)
	{
		TLOGERROR("[AsyncProcThread exception.]" << endl);
	}
}
/////////////////////////////////////////////////////////////////////////
}
