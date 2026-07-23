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

#include "Stress.h"
#include "servant/Communicator.h"
#include "util/tc_thread_pool.h"

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

using namespace std;
using namespace Test;
using namespace tars;

namespace
{

const size_t ASYNC_INFLIGHT_LIMIT = 10000;
const int INVOKE_TIMEOUT_MS = 15000;

struct RunSummary
{
    uint64_t success;
    uint64_t failed;
};

class RunState
{
public:
    explicit RunState(size_t inflightLimit)
        : _inflightLimit(inflightLimit)
    {
    }

    void acquireAsyncSlot()
    {
        unique_lock<mutex> lock(_mutex);
        _slotCondition.wait(lock, [this] { return _inflight < _inflightLimit; });
        ++_inflight;
    }

    void finishAsync(bool success)
    {
        {
            lock_guard<mutex> lock(_mutex);
            --_inflight;
            finishLocked(success);
        }

        _slotCondition.notify_one();
        _finishCondition.notify_all();
    }

    void finishSync(bool success)
    {
        {
            lock_guard<mutex> lock(_mutex);
            finishLocked(success);
        }

        _finishCondition.notify_all();
    }

    void waitForFinished(uint64_t total)
    {
        unique_lock<mutex> lock(_mutex);
        _finishCondition.wait(lock, [this, total] { return _success + _failed == total; });
    }

    RunSummary summary() const
    {
        lock_guard<mutex> lock(_mutex);
        return {_success, _failed};
    }

private:
    void finishLocked(bool success)
    {
        if (success)
        {
            ++_success;
        }
        else
        {
            ++_failed;
        }
    }

private:
    const size_t _inflightLimit;
    mutable mutex _mutex;
    condition_variable _slotCondition;
    condition_variable _finishCondition;
    size_t _inflight = 0;
    uint64_t _success = 0;
    uint64_t _failed = 0;
};

class StressCallback : public StressPrxCallback
{
public:
    StressCallback(shared_ptr<RunState> state, size_t expectedSize)
        : _state(std::move(state)), _expectedSize(expectedSize)
    {
    }

    void callback_testStr(tars::Int32 ret, const string &sOut) override
    {
        _state->finishAsync(ret == 0 && sOut.size() == _expectedSize);
    }

    void callback_testStr_exception(tars::Int32) override
    {
        _state->finishAsync(false);
    }

private:
    shared_ptr<RunState> _state;
    const size_t _expectedSize;
};

class Test1
{
public:
    Test1(const string &obj, size_t bufferSize)
        : _request(bufferSize, 'a'), _state(make_shared<RunState>(ASYNC_INFLIGHT_LIMIT))
    {
        if (obj.find('@') == string::npos)
        {
            _comm.setProperty("stat", "tars.tarsstat.StatObj");
        }
        _comm.stringToProxy(obj, _prx);
        _prx->tars_async_timeout(INVOKE_TIMEOUT_MS);
    }

    void syncCall(int64_t executeNum)
    {
        for (int64_t i = 0; i < executeNum; ++i)
        {
            bool success = false;

            try
            {
                string response;
                tars::Int32 ret = _prx->tars_set_timeout(INVOKE_TIMEOUT_MS)->testStr(_request, response);
                success = ret == 0 && response.size() == _request.size();
            }
            catch (const exception &e)
            {
                cerr << "pthread id: " << this_thread::get_id() << ", id: " << i
                     << ", exception: " << e.what() << endl;
            }
            catch (...)
            {
                cerr << "pthread id: " << this_thread::get_id() << ", id: " << i
                     << ", unknown exception." << endl;
            }

            _state->finishSync(success);
        }
    }

    void asyncCall(int64_t executeNum)
    {
        for (int64_t i = 0; i < executeNum; ++i)
        {
            _state->acquireAsyncSlot();

            try
            {
                StressPrxCallbackPtr callback(new StressCallback(_state, _request.size()));
                _prx->async_testStr(callback, _request);
            }
            catch (const exception &e)
            {
                cerr << "pthread id: " << this_thread::get_id() << ", id: " << i
                     << ", exception: " << e.what() << endl;
                _state->finishAsync(false);
            }
            catch (...)
            {
                cerr << "pthread id: " << this_thread::get_id() << ", id: " << i
                     << ", unknown exception." << endl;
                _state->finishAsync(false);
            }
        }
    }

    void waitForFinished(uint64_t total)
    {
        _state->waitForFinished(total);
    }

    RunSummary summary() const
    {
        return _state->summary();
    }

private:
    Communicator _comm;
    StressPrx _prx;
    const string _request;
    shared_ptr<RunState> _state;
};

void printUsage(const char *program)
{
    cerr << "usage: " << program << " ThreadNum CallTimes sObj size [sync|async]" << endl;
}

} // namespace

int main(int argc, char **argv)
{
    if (argc != 5 && argc != 6)
    {
        printUsage(argv[0]);
        return 1;
    }

    try
    {
        const int64_t threads = TC_Common::strto<int64_t>(argv[1]);
        const int64_t times = TC_Common::strto<int64_t>(argv[2]);
        const string obj = argv[3];
        const int64_t bufferSize = TC_Common::strto<int64_t>(argv[4]);
        const string callMode = argc == 6 ? argv[5] : "sync";

        if (callMode != "sync" && callMode != "async")
        {
            cerr << "CallMode must be sync or async" << endl;
            printUsage(argv[0]);
            return 1;
        }

        if (threads <= 0 || times <= 0 || bufferSize <= 0)
        {
            cerr << "ThreadNum, CallTimes and size must be positive" << endl;
            printUsage(argv[0]);
            return 1;
        }

        Test1 client(obj, static_cast<size_t>(bufferSize));
        TC_ThreadPool threadPool;
        threadPool.init(static_cast<size_t>(threads));
        threadPool.start();

        const uint64_t total = static_cast<uint64_t>(threads) * static_cast<uint64_t>(times);
        const int64_t start = TC_Common::now2us();

        for (int64_t i = 0; i < threads; ++i)
        {
            if (callMode == "async")
            {
                threadPool.exec(std::bind(&Test1::asyncCall, &client, times));
            }
            else
            {
                threadPool.exec(std::bind(&Test1::syncCall, &client, times));
            }
        }

        threadPool.waitForAllDone();
        client.waitForFinished(total);

        const int64_t costUs = TC_Common::now2us() - start;
        const RunSummary summary = client.summary();
        const uint64_t completed = summary.success + summary.failed;
        const double qps = costUs > 0 ? completed * 1000000.0 / costUs : 0;

        cout << fixed << setprecision(2)
             << "mode:" << callMode
             << ", total:" << completed
             << ", success:" << summary.success
             << ", failed:" << summary.failed
             << ", cost_ms:" << costUs / 1000.0
             << ", qps:" << qps
             << endl;

        return summary.failed == 0 ? 0 : 1;
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << endl;
    }

    return 1;
}
