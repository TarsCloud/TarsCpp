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

#include "util/tc_thread.h"
#include "util/tc_port.h"
#include "util/tc_coroutine.h"
#include "util/tc_common.h"
#include <sstream>
#include <cerrno>
#include <cassert>

namespace tars
{


TC_ThreadControl::TC_ThreadControl(std::thread *th) : _th(th)
{
	assert(_th != NULL);
}

void TC_ThreadControl::join()
{
    if (std::this_thread::get_id() == _th->get_id())
    {
        throw TC_ThreadThreadControl_Exception("[TC_ThreadControl::join] can't be called in the same thread");
    }

    if (_th->joinable())
    {
    	_th->join();
    }
}

void TC_ThreadControl::detach()
{
    _th->detach();
}

std::thread::id TC_ThreadControl::id() const
{
    return _th->get_id();
}

void TC_ThreadControl::sleep(int64_t millsecond)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millsecond));
}

void TC_ThreadControl::yield()
{
    std::this_thread::yield();
}

TC_Thread::TC_Thread(const string &threadName) : _threadName(threadName), _running(false), _th(NULL)
{
}

TC_Thread::~TC_Thread()
{
    if (_th != NULL)
    {
        //如果资源没有被detach或者被join，则自己释放
        if (_th->joinable())
        {
            _th->detach();
        }

        delete _th;
        _th = NULL;
    }
}

void TC_Thread::setThreadName(const string &threadName)
{
	_threadName = threadName;
}

class RunningClosure
{
public:
	RunningClosure(TC_Thread *pThread): _pThread(pThread) {
		pThread->_running = true;
	}

	~RunningClosure() {
		if(!_pThread->getScheduler())
		{
			//非协程模式
			_pThread->_running = false;
		}
	}
protected:

	TC_Thread *_pThread;
};

void TC_Thread::threadEntry(TC_Thread *pThread)
{
	RunningClosure r(pThread);

    {
        TC_ThreadLock::Lock sync(pThread->_lock);
        pThread->_lock.notifyAll();
    }

    try
    {
        pThread->run();
    }
    catch (exception &ex)
    {
        cerr << std::this_thread::get_id() << "|" << ex.what() << endl;
        throw ex;
    }
    catch (...)
    {
        throw;
    }
}

TC_ThreadControl TC_Thread::start()
{
    TC_ThreadLock::Lock sync(_lock);

    if (_running)
    {
        throw TC_ThreadThreadControl_Exception("[TC_Thread::start] thread has start");
    }

    try
    {
        _th = new std::thread(&TC_Thread::threadEntry, this);

    }
    catch(...)
    {
        throw TC_ThreadThreadControl_Exception("[TC_Thread::start] thread start error");
    }

    _lock.wait();

    return TC_ThreadControl(_th);
}

void TC_Thread::coroutineEntry(TC_Thread *pThread, uint32_t iPoolSize, size_t iStackSize, bool autoQuit)
{
	pThread->_scheduler = TC_CoroutineScheduler::create();

	pThread->_scheduler->setPoolStackSize(iPoolSize, iStackSize);

    if(autoQuit)
    {
	    pThread->_scheduler->setNoCoroutineCallback([](TC_CoroutineScheduler *scheduler){
            scheduler->terminate();
        });
    }

	pThread->_scheduler->go(std::bind(TC_Thread::threadEntry, pThread));

    {
        TC_ThreadLock::Lock sync(pThread->_lock);
        pThread->_lock.notifyAll();
    }

	pThread->_scheduler->run();

    pThread->_running = false;

    pThread->_scheduler.reset();
    TC_CoroutineScheduler::reset();
}

TC_ThreadControl TC_Thread::startCoroutine(uint32_t iPoolSize, size_t iStackSize, bool autoQuit)
{
    TC_ThreadLock::Lock sync(_lock);

    if (_running)
    {
        throw TC_ThreadThreadControl_Exception("[TC_Thread::startCoroutine] thread has start");
    }

    try
    {
        _th = new std::thread(&TC_Thread::coroutineEntry, this, iPoolSize, iStackSize, autoQuit);
    }
    catch(...)
    {
        throw TC_ThreadThreadControl_Exception("[TC_Thread::startCoroutine] thread start error");
    }

    _lock.wait();

    return TC_ThreadControl(_th);
    
}

void TC_Thread::join()
{
    if(!_th)
    {
        return;
    }
    if (std::this_thread::get_id() == _th->get_id())
    {
        throw TC_ThreadThreadControl_Exception("[TC_Thread::join] can't be called in the same thread");
    }

    if (_th->joinable())
    {
        _th->join();
    }
}

bool TC_Thread::joinable()
{
    if(!_th)
    {
        return false;
    }

    return _th->joinable();
}

void TC_Thread::detach()
{
    _th->detach();
}

TC_ThreadControl TC_Thread::getThreadControl() 
{
    return TC_ThreadControl(_th);
}

bool TC_Thread::isAlive() const
{
    return _running;
}

size_t TC_Thread::CURRENT_THREADID()
{
    static thread_local size_t threadId = 0;
    if(threadId == 0 )
    {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        threadId = strtol(ss.str().c_str(), NULL, 0);
    }
    return threadId;
}

}

