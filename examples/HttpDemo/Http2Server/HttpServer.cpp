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

#include "HttpServer.h"
#include "Http2Imp.h"
#include "util/tc_http2.h"

using namespace std;

HttpServer g_app;

TC_NetWorkBuffer::PACKET_TYPE parseHttp2(TC_NetWorkBuffer&in, vector<char> &out)
{
//	cout << "parseHttp2:" << in.getBufferLength() << endl;
    TC_Http2Server*sessionPtr = (TC_Http2Server*)(in.getContextData());

    if(sessionPtr == NULL)
    {
    	shared_ptr<TC_Http2Server> session(new TC_Http2Server());
	    in.setContextData(session.get());

	    session->settings(3000);

        TC_EpollServer::Connection *connection = (TC_EpollServer::Connection *)in.getConnection();
        Http2Imp::addHttp2(connection->getId(), session);

	    sessionPtr = session.get();
    }

	return sessionPtr->parse(in, out);
}


void
HttpServer::initialize()
{
    //initialize application here:
    //...

    addServant<Http2Imp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".Http2Obj");
    addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".Http2Obj", &parseHttp2);
}
/////////////////////////////////////////////////////////////////
void
HttpServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
