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
#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "util/tc_thread_pool.h"
#include "tup/Tars.h"
#include "tup/tup.h"
#include "util/tc_timeprovider.h"
using namespace std;
using namespace tars;
using namespace tup;

int doRequest(TC_HttpRequest& stHttp,TC_TCPClient&tcpClient, TC_HttpResponse &stHttpRsp, int iTimeout)
{
    string sSendBuffer = stHttp.encode();

    int iRet = tcpClient.send(sSendBuffer.c_str(), sSendBuffer.length());
    if(iRet != TC_ClientSocket::EM_SUCCESS)
    {
        return iRet;
    }

    stHttpRsp.reset();

    string sBuffer;

    char *sTmpBuffer = new char[10240];
    size_t iRecvLen  = 10240;

    while(true)
    {
        iRecvLen = 10240;

        iRet = tcpClient.recv(sTmpBuffer, iRecvLen);

        if(iRet == TC_ClientSocket::EM_SUCCESS)
        sBuffer.append(sTmpBuffer, iRecvLen);

        switch(iRet)
        {
        case TC_ClientSocket::EM_SUCCESS:
            if(stHttpRsp.incrementDecode(sBuffer))
            {
                delete []sTmpBuffer;
                return TC_ClientSocket::EM_SUCCESS;
            }
            continue;
        case TC_ClientSocket::EM_CLOSE:
            delete []sTmpBuffer;
            stHttpRsp.incrementDecode(sBuffer);
            return TC_ClientSocket::EM_SUCCESS;
        default:
            delete []sTmpBuffer;
            return iRet;
        }
    }

    assert(true);

    return 0;
}

void th_dohandle(int excut_num)
{
    unsigned long sum = 0;
    int64_t _iTime = TC_TimeProvider::getInstance()->getNowMs();

    string sServer1("http://127.0.0.1:8080/");

    TC_HttpRequest stHttpReq;
    stHttpReq.setCacheControl("no-cache");
    stHttpReq.setGetRequest(sServer1);

    TC_TCPClient tcpClient1;
    tcpClient1.init("127.0.0.1", 8080, 3000);

    int iRet = 0;

    for (int i = 0; i<excut_num; i++)
    {
        try
        {
            TC_HttpResponse stHttpRsp;


            iRet = doRequest(stHttpReq, tcpClient1, stHttpRsp, 3000);  
            // iRet = stHttpReq.doRequest(stHttpRsp, 3000);  
            
            if (iRet != 0)
            {
                cout <<"pthread id: " << TC_Thread::CURRENT_THREADID() << ", iRet:" << iRet << "" <<endl;
            }
            else
            {
                sum++;
            }
        }
        catch(TC_Exception &e)
        {
            cout << "pthread id: " << TC_Thread::CURRENT_THREADID() << " id: " << i << " exception: " << e.what() << endl;
        }
        catch(...)
        {
             cout << "pthread id: " << TC_Thread::CURRENT_THREADID() << " id: " << i << " unknown exception." << endl;
        }
    }
    cout <<  "pthread id: " << TC_Thread::CURRENT_THREADID() << ", succ:" << sum << "/" << excut_num << ", " << TC_TimeProvider::getInstance()->getNowMs() - _iTime <<"(ms)"<<endl;
}

int main(int argc,char ** argv)
{
    if(argc != 3)
    {
        cout << "usage: " << argv[0] << " ThreadNum CallTimes" << endl;
        return -1;
    }

    try
    {
        tars::Int32 threads = TC_Common::strto<tars::Int32>(string(argv[1]));
        TC_ThreadPool tp;
        tp.init(threads);
        tp.start(); 
        cout << "init tp succ" << endl;
        tars::Int32 times = TC_Common::strto<tars::Int32>(string(argv[2]));
 
        for(int i = 0; i<threads; i++)
        {
            tp.exec(std::bind(th_dohandle, times));
        }

        tp.waitForAllDone();
    }catch(exception &e)
    {
        cout<<e.what()<<endl;
    }
    catch(...)
    {

    }

    return 0;
}
