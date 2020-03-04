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

#include "util/tc_epoll_server.h"
#include "util/tc_http.h"
#include "servant/AppProtocol.h"
#include "servant/Transceiver.h"
#include "servant/AdapterProxy.h"
#include "servant/TarsLogger.h"
#include "tup/Tars.h"
#include <iostream>

#if TARS_HTTP2
#include "util/tc_http2.h"
#endif

namespace tars
{

class Transceiver;

vector<char> ProxyProtocol::tarsRequest(RequestPacket& request, Transceiver *)
{
	TarsOutputStream<BufferWriterVector> os;

	tars::Int32 iHeaderLen = 0;

//	先预留4个字节长度
	os.writeBuf((const char *)&iHeaderLen, sizeof(iHeaderLen));

	request.writeTo(os);

    vector<char> buff;

	buff.swap(os.getByteBuffer());

	assert(buff.size() >= 4);

	iHeaderLen = htonl((int)(buff.size()));

	memcpy((void*)buff.data(), (const char *)&iHeaderLen, sizeof(iHeaderLen));

    return buff;
}

////////////////////////////////////////////////////////////////////////////////////

vector<char> ProxyProtocol::http1Request(tars::RequestPacket& request, Transceiver *trans)
{
	request.iRequestId = trans->getAdapterProxy()->getId();

    TC_HttpRequest httpRequest;

    httpRequest.setRequest(request.sFuncName, request.sServantName, string(request.sBuffer.data(), request.sBuffer.size()), true);

    vector<char> buffer;

    httpRequest.encode(buffer);    

    return buffer;
}

struct Http1Context
{
//    string buff;

    TC_HttpResponse httpRsp;
};

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http1Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
    Http1Context *context = (Http1Context*)(in.getContextData());

    if(context == NULL)
    {
        context = new Http1Context();
        in.setContextData(context, [=]{ delete context; }); 
    }

//    context->buff.append(in.getBuffersString());
//    in.clearBuffers();

    if(context->httpRsp.incrementDecode(in))
    {
        rsp.iRequestId = ((Transceiver*)(in.getConnection()))->getAdapterProxy()->getId();

        rsp.status["status"]  = context->httpRsp.getResponseHeaderLine();
        for (const auto& kv : context->httpRsp.getHeaders())
        {
            rsp.status[kv.first] = kv.second; 
        } 

        rsp.sBuffer.assign(context->httpRsp.getContent().begin(), context->httpRsp.getContent().end());

        delete context;

        context = NULL;

        in.setContextData(NULL); 

        return TC_NetWorkBuffer::PACKET_FULL;

    }

    return TC_NetWorkBuffer::PACKET_LESS;
}

#if TARS_HTTP2

// ENCODE function, called by network thread
vector<char> ProxyProtocol::http2Request(RequestPacket& request, Transceiver *trans)
{
    TC_Http2Client* session = (TC_Http2Client*)trans->getSendBuffer()->getContextData();
	if(session == NULL)
	{
		session = new TC_Http2Client();

		trans->getSendBuffer()->setContextData(session, [=]{delete session;});

		session->settings(3000);
	}

	request.iRequestId = session->submit(request.sFuncName, request.sServantName, request.context, request.sBuffer);
	if (request.iRequestId < 0)
	{
		TLOGERROR("[TARS]http2Request::Fatal submit error: " << session->getErrMsg() << endl);
		return vector<char>();
	}

	vector<char> out;
	session->swap(out);

	return out;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http2Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	TC_Http2Client* session = (TC_Http2Client*)((Transceiver*)(in.getConnection()))->getSendBuffer()->getContextData();

	pair<int, shared_ptr<TC_HttpResponse>> out;
	TC_NetWorkBuffer::PACKET_TYPE flag = session->parseResponse(in, out);

	if(flag == TC_NetWorkBuffer::PACKET_FULL)
	{
		rsp.iRequestId  = out.first;
		out.second->getHeaders(rsp.status);
		rsp.sBuffer.assign(out.second->getContent().begin(), out.second->getContent().end());
	}

	return flag;
}

#endif
}

