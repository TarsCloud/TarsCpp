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
#include "Proxy.h"

using namespace std;
using namespace TestApp;
using namespace tars;

int main(int argc,char ** argv)
{
    Communicator comm;

    try
    {
        ProxyPrx prx;
        comm.stringToProxy("TestApp.ProxyServer.ProxyObj@tcp -h 127.0.0.1 -p 9200" , prx);
	    int64_t t = TC_Common::now2us();
        try
        {
            int i = 10000;
            while(i-- >= 0)
            {
                string sReq("hello");
                string sRsp("");

                int iRet = prx->testProxy(sReq, sRsp);

                assert(iRet == 0);
                assert(sReq == sRsp);
            }

            int64_t cost = TC_Common::now2us() - t;
            cout << "syncCall total:" << cost << "us, avg:" << 1.*cost/10000 << "us" << endl;

        }
        catch(exception &ex)
        {
            cerr << "ex:" << ex.what() << endl;
        }
        catch(...)
        {
            cerr << "unknown exception." << endl;
        }
    }
    catch(exception& e)
    {
        cerr << "exception:" << e.what() << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << endl;
    }

    return 0;
}
