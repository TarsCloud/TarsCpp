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

#include "Http2Imp.h"
#include "servant/Application.h"

using namespace std;

TC_SpinLock Http2Imp::_mutex;

unordered_map<int32_t, TC_Http2Server*> Http2Imp::_http2;

//////////////////////////////////////////////////////
void Http2Imp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void Http2Imp::destroy()
{
    //destroy servant here:
    //...
}

void doRequestFunc(const TC_Http2Server::Req_Type reqtype, const string &requri, const TC_Http::http_header_type &reqHeader, const string &reqBody, TC_Http2Server::Http2Response &rsp)
{
    rsp.status = 200;
    rsp.about  = "OK";
    rsp.body   = "response helloworld 2";
}

int Http2Imp::doRequest(TarsCurrentPtr current, vector<char> &buffer)
{
    TC_Http2Server* session = getHttp2(current->getUId());

cout << "doRequest" << endl;
    static bool flag = true;
    if(flag)
    {
        //method 1:
        vector<int32_t> vtReqid;
        TC_Http2Server::doRequest(current->getRequestBuffer(), vtReqid);
        
        TC_Http2Server::Http2Response rsp;
        rsp.status = 200;
        rsp.about  = "OK";
        rsp.body   = "response helloworld 1";

        for(size_t i = 0; i < vtReqid.size(); i++)
        {
            session->doResponse(vtReqid[i], rsp, buffer);
        }
    }
    else
    {
        //method 2:
        session->doRequest(current->getRequestBuffer(), doRequestFunc, buffer);
    }

    flag = !flag;

    return 0;
}

int Http2Imp::doClose(TarsCurrentPtr current)
{
    delHttp2(current->getUId());

    return 0;
}