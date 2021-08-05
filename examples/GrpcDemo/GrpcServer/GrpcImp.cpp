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

#include "GrpcImp.h"
#include "servant/Application.h"
#include "helloworld.pb.h"
#include "GrpcServer.h"

using namespace std;

//////////////////////////////////////////////////////
void GrpcImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void GrpcImp::destroy()
{
    //destroy servant here:
    //...
}

int GrpcImp::doRequest(TarsCurrentPtr current, vector<char> &buffer)
{
    shared_ptr<TC_GrpcServer> session = TC_GrpcServer::getHttp2(current->getUId());

	vector<shared_ptr<TC_GrpcServer::Http2Context>> contexts = session->decodeRequest();

	for(size_t i = 0; i< contexts.size(); ++i)
	{
		shared_ptr<TC_GrpcServer::Http2Context> context = contexts[i];

		std::string content =  context->request.getContent();
		bool compressed;
		RemoveGrpcPrefix(content, &compressed);
		string path = context->request.getHeader(":path");
		if (path == "/helloworld.Greeter/SayHello") {
			vector<char> data;
			helloworld::HelloReply reply;
			helloworld::HelloRequest request;
			request.ParseFromString(content);
			TLOGDEBUG("doRequest request: " << request.name() << endl);
			reply.set_message("Hello " + request.name());
			string message = reply.SerializeAsString();
			addGrpcPrefix(message, false);
			session->packGrpcResponse(context, 200, message);
			int ret = session->encodeResponse(context, "0", data);
			if(ret != 0)
			{
				cout << "encodeResponse error:" << session->getErrMsg() << endl;
			}

			buffer.insert(buffer.end(), data.begin(), data.end());
		}
	}

    return 0;
}


int GrpcImp::doClose(TarsCurrentPtr current)
{
    TC_GrpcServer::delHttp2(current->getUId());
    return 0;
}