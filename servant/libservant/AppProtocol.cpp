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
#include "servant/RemoteLogger.h"
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
//	assert(trans->getAdapterProxy()->getObjProxy()->getServantProxy()->taf_connection_serial() > 0);

//	request.iRequestId = trans->getAdapterProxy()->getId();

	shared_ptr<TC_HttpRequest> &data = *(shared_ptr<TC_HttpRequest>*)request.sBuffer.data();

	vector<char> buffer;

	data->encode(buffer);

	data.reset();

	return buffer;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http1Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	shared_ptr<TC_HttpResponse> *context = (shared_ptr<TC_HttpResponse>*)(in.getContextData());

    if(!context)
    {
        context = new shared_ptr<TC_HttpResponse>();
        *context = std::make_shared<TC_HttpResponse>();
        in.setContextData(context, [=]{ delete context; }); 
    }

    if((*context)->incrementDecode(in))
    {
	    rsp.sBuffer.resize(sizeof(shared_ptr<TC_HttpResponse>));

	    shared_ptr<TC_HttpResponse> &data = *(shared_ptr<TC_HttpResponse>*)rsp.sBuffer.data();

	    data = *context;

	    if(!data->checkHeader("Connection", "keep-alive"))
	    {
		    Transceiver* session = (Transceiver*)(in.getConnection());

		    session->close();
	    }

		(*context) = NULL;
		delete context;
		in.setContextData(NULL);

	    return TC_NetWorkBuffer::PACKET_FULL;

    }

    return TC_NetWorkBuffer::PACKET_LESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// vector<char> ProxyProtocol::httpJceRequest(taf::BasePacket& request, Transceiver *trans)
// {
// 	TC_HttpRequest httpRequest;

// 	string uri;
// 	if(trans->isSSL())
// 		uri = "https://";
// 	else
// 		uri = "http://";

// 	uri += trans->getEndpointInfo().getEndpoint().getHost();

// 	vector<char> buff = tafRequest(request, trans);

// 	for(auto it = request.context.begin(); it != request.context.end(); ++it)
// 	{
// 		if(it->second == ":path")
// 		{
// 			uri += "/" + it->second;
// 		}
// 		else
// 		{
// 			httpRequest.setHeader(it->first, it->second);
// 		}
// 	}

// 	httpRequest.setPostRequest(uri, buff.data(), buff.size(), true);

// 	vector<char> buffer;

// 	httpRequest.encode(buffer);

// 	return buffer;
// }

// TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::httpJceResponse(TC_NetWorkBuffer &in, BasePacket& rsp)
// {
// 	TC_HttpResponse *context = (TC_HttpResponse*)(in.getContextData());

// 	if(!context)
// 	{
// 		context = new TC_HttpResponse();
// 		in.setContextData(context, [=]{ delete context; });
// 	}

// 	if(context->incrementDecode(in))
// 	{
// 		if(context->getStatus() != 200)
// 		{
// 			rsp.iRet = taf::JCESERVERUNKNOWNERR;
// 			rsp.sResultDesc = context->getContent();
// 			return TC_NetWorkBuffer::PACKET_FULL;
// 		}

// 		JceInputStream<> is;
// 		is.setBuffer(context->getContent().c_str() + 4, context->getContent().size() - 4);

// 		rsp.readFrom(is);

// 		if(!context->checkHeader("Connection", "keep-alive"))
// 		{
// 			Transceiver* session = (Transceiver*)(in.getConnection());

// 			session->close();
// 		}

// 		context = NULL;
// 		delete context;
// 		in.setContextData(NULL);

// 		return TC_NetWorkBuffer::PACKET_FULL;
// 	}

// 	return TC_NetWorkBuffer::PACKET_LESS;
// }

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

	shared_ptr<TC_HttpRequest> *data = (shared_ptr<TC_HttpRequest>*)request.sBuffer.data();

	request.iRequestId = session->submit(*(*data).get());

	//这里把智能指针释放一次
	(*data).reset();

	if (request.iRequestId < 0)
	{
		TLOGERROR("http2Request::Fatal submit error: " << session->getErrMsg() << endl);
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

		rsp.sBuffer.resize(sizeof(shared_ptr<TC_HttpResponse>));

		//这里智能指针有一次+1, 后面要自己reset掉
		*(shared_ptr<TC_HttpResponse>*)rsp.sBuffer.data() = out.second;
	}

	return flag;
}

#endif
}

