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

#include "util/tc_thread_pool.h"
#include "util/tc_common.h"

#include <iostream>

namespace tars
{

TC_ThreadPool::TC_ThreadPool()
    :  _threadNum(1), _bTerminate(false)
{
}

TC_ThreadPool::~TC_ThreadPool()
{
    stop();
}

void TC_ThreadPool::init(size_t num)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (!_threads.empty())
    {
        throw TC_ThreadPool_Exception("[TC_ThreadPool::init] thread pool has start!");
    }

    _threadNum = num;
}

void TC_ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);

        _bTerminate = true;

        _condition.notify_all();
    }

    for (size_t i = 0; i < _threads.size(); i++)
    {
        if(_threads[i]->joinable())
        {
            _threads[i]->join();
        }
        delete _threads[i];
        _threads[i] = NULL;
    }

    std::unique_lock<std::mutex> lock(_mutex);
    _threads.clear();
}

void TC_ThreadPool::start()
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (!_threads.empty())
    {
        throw TC_ThreadPool_Exception("[TC_ThreadPool::start] thread pool has start!");
    }

    for (size_t i = 0; i < _threadNum; i++)
    {
        _threads.push_back(new thread(&TC_ThreadPool::run, this));
    }
}

bool TC_ThreadPool::get(TaskFuncPtr& task)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_tasks.empty())
    {
        _condition.wait(lock, [this] { return _bTerminate || !_tasks.empty(); });
    }

    if (_bTerminate)
        return false;

    if (!_tasks.empty())
    {
        task = std::move(_tasks.front());

        _tasks.pop();

        return true;
    }

    return false;
}

void TC_ThreadPool::run()
{
    //调用处理部分
    while (!isTerminate())
    {
        TaskFuncPtr task;
        bool ok = get(task);
        if (ok)
        {
            ++_atomic;
            try
            {
                if (task->_expireTime != 0 && task->_expireTime  < TNOWMS )
                {
                    //超时任务，是否需要处理?
                }
                else
                {
                    task->_func();
                }
            }
            catch (...)
            {
            }

            --_atomic;

            //任务都执行完毕了
            std::unique_lock<std::mutex> lock(_mutex);
            if (_atomic == 0 && _tasks.empty())
            {
                _condition.notify_all();
            }
        }
    }
}

bool TC_ThreadPool::waitForAllDone(int millsecond)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_tasks.empty())
        return true;

    if (millsecond < 0)
    {
        _condition.wait(lock, [this] { return _tasks.empty(); });
        return true;
    }
    else
    {
        return _condition.wait_for(lock, std::chrono::milliseconds(millsecond), [this] { return _tasks.empty(); });
    }
}

///////////////////////////////////////////////////////////////////////////////////////

TC_ThreadPoolHash::TC_ThreadPoolHash()
{

}

TC_ThreadPoolHash::~TC_ThreadPoolHash()
{

}

void TC_ThreadPoolHash::init(size_t num)
{
    for (size_t i = 0 ;i < num;i++)
    {
        TC_ThreadPool* p = new TC_ThreadPool();
        p->init(1);
        _pools.push_back(p);
    }
}

TC_ThreadPool* TC_ThreadPoolHash::selectThread(const string& hashkey)
{
    if (_pools.empty())
    {
        return nullptr;
    }
    std::hash<string> hash_fu;
    size_t pos = hash_fu(hashkey) % _pools.size();
    return _pools[pos];
}

void TC_ThreadPoolHash::stop()
{
    for (size_t i = 0; i < _pools.size(); i++)
    {
        _pools[i]->stop();
        delete _pools[i];
    }
    _pools.clear();
}

void TC_ThreadPoolHash::start()
{
    for (size_t i = 0; i < _pools.size() ;i ++)
    {
        _pools[i]->start();
    }
}


}
