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
#include "util/tc_http2session.h"
#include "servant/Application.h"

using namespace std;

TC_SpinLock Http2Imp::_mutex;

unordered_map<int32_t, TC_Http2Session*> Http2Imp::_http2Sessions;

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

// typedef int (*ResponseFunc)(const Req_Type reqtype, 
//                          const string &requri, 
//                          const TC_Http::http_header_type &reqHeader, 
//                          const string &reqBody,
//                          int &resopnseStatus,
//                          string &resopnseAbout,
//                          TC_Http::http_header_type &responseHeader,
//                          string &responseBody);

int Http2Imp::doRequest(TarsCurrentPtr current, vector<char> &buffer)
{
    vector<int32_t> vtReqid;

    TC_Http2Session::doRequest(current->getRequestBuffer(), vtReqid);
    
    cout << "doRequest:" << TC_Common::tostr(vtReqid.begin(), vtReqid.end(), ", ") << endl;

    TC_Http2Session* session = getHttp2Session(current->getUId());

    TC_Http2Session::Http2Response rsp;
    rsp.status = 200;
    rsp.about  = "OK";
    rsp.body   = "response helloworld";

    for(size_t i = 0; i < vtReqid.size(); i++)
    {
        session->doResponse(vtReqid[i], rsp, buffer);
    }

    cout << "response buffer size:" << buffer.size() << endl;
    return 0;
}

int Http2Imp::doClose(TarsCurrentPtr current)
{
    delHttp2Session(current->getUId());

    return 0;
}