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

#include "BServant.h"
#include "servant/Communicator.h"
#include "util/tc_thread_pool.h"
#include <iostream>

using namespace std;
using namespace Test;
using namespace tars;

class Test1
{
public:
    Test1();

    ~Test1();

    void  queryResult(int iFlag, int iExecuteNum);

private:
    Communicator    _comm;
    BServantPrx     _prx;
};

Test1::Test1()
{
    // _comm.setProperty("locator", "tars.tarsregistry.QueryObj@tcp -h 10.208.139.242 -p 17890 -t 10000");
    // _comm.setProperty("stat", "tars.tarsstat.StatObj");
    _prx = _comm.stringToProxy<BServantPrx>("TestApp.BServer.BServantObj@tcp -h 127.0.0.1 -p 9100");
}

Test1::~Test1()
{
    
}

void Test1::queryResult(int iFlag, int iExecuteNum)
{
    string sIn(10,'a');
    string sOut("");

    time_t t = TC_Common::now2us();

    for(int i=0; i<iExecuteNum; i++) 
    {
        // sOut = "";
        try
        {
            int ret = -1;
            if(iFlag == 0)
            {
                ret = _prx->testCoroSerial(sIn, sOut);
            }
            else
            {
                ret = _prx->testCoroParallel(sIn, sOut);
            }

            assert(sIn == sOut);
            // cout << ret << ", " << sIn << ", " << sOut << endl;
        }
        catch(TC_Exception &e)
        {
            cout << "pthread id: " << std::this_thread::get_id() << "id: " << i << "exception: " << e.what() << endl;
        }
        catch(...)
        {
            cout << "pthread id: " << std::this_thread::get_id() << "id: " << i << "unknown exception." << endl;
        }
    }

    int64_t cost = TC_Common::now2us() - t;
    cout << "syncCall total:" << cost << "us, avg:" << 1.*cost/iExecuteNum << "us" << endl;
}

int main(int argc,char ** argv)
{
    if(argc != 4)
    {
        cout << "usage: " << argv[0] << " ThreadNum CallTimes  CallMode" << endl;
        return -1;
    }

    Test1 test1;

    try
    {
        tars::Int32 threads = TC_Common::strto<tars::Int32>(string(argv[1]));

        TC_ThreadPool tp;
        tp.init(threads);
        tp.start();

        tars::Int32 times = TC_Common::strto<tars::Int32>(string(argv[2]));
        tars::Int32 callMode = TC_Common::strto<tars::Int32>(string(argv[3]));
        
        for(int i = 0; i<threads; i++) 
        {
            auto fw = std::bind(&Test1::queryResult, &test1, callMode, times);
            tp.exec(fw);
        }

        tp.waitForAllDone(); 
    }
    catch(exception &e)
    {
        cout<<e.what()<<endl;
    }
    catch(...)
    {
        
    }
    
    return 0;
}
