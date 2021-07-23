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
#include "util/tc_grpc.h"
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

	int iHeaderLen = 0;

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
		in.setContextData(context, [](TC_NetWorkBuffer*nb){ shared_ptr<TC_HttpResponse> *p = (shared_ptr<TC_HttpResponse>*)(nb->getContextData()); if(!p) { nb->setContextData(NULL); delete p; }});
    }

    if((*context)->incrementDecode(in))
    {
	    rsp.sBuffer.resize(sizeof(shared_ptr<TC_HttpResponse>));

	    shared_ptr<TC_HttpResponse> &data = *(shared_ptr<TC_HttpResponse>*)rsp.sBuffer.data();

	    data = *context;

		auto ret = TC_NetWorkBuffer::PACKET_FULL;
	    if(!data->checkHeader("Connection", "keep-alive"))
	    {
			// 如果非keep-alive，返回PACKET_FULL_CLOSE，外层结束连接
			ret = TC_NetWorkBuffer::PACKET_FULL_CLOSE;
	    }

		(*context) = NULL;
		delete context;
		in.setContextData(NULL);

	    return ret;

    }

    return TC_NetWorkBuffer::PACKET_LESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

#if TARS_HTTP2

// ENCODE function, called by network thread
vector<char> ProxyProtocol::http2Request(RequestPacket& request, Transceiver *trans)
{
    TC_Http2Client* session = (TC_Http2Client*)trans->getSendBuffer()->getContextData();
	if(session == NULL)
	{
		session = new TC_Http2Client();

		trans->getSendBuffer()->setContextData(session, [=](TC_NetWorkBuffer*nb){ delete session; });

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

//	cout << "http2Request id:" << request.iRequestId << endl;

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


// ENCODE function, called by network thread
vector<char> ProxyProtocol::grpcRequest(RequestPacket& request, Transceiver *trans)
{
    TC_GrpcClient* session = (TC_GrpcClient*)trans->getSendBuffer()->getContextData();
	if(session == NULL)
	{
		session = new TC_GrpcClient();

		trans->getSendBuffer()->setContextData(session, [=](TC_NetWorkBuffer*nb){ delete session; });

		session->settings(3000);
	}

    if (session->buffer().size() != 0) {
        //直接发送裸得应答数据，业务层一般不直接使用，仅仅tcp支持
		trans->getSendBuffer()->addBuffer(session->buffer());
		auto data = trans->getSendBuffer()->getBufferPointer();
		int iRet = trans->send(data.first, (uint32_t) data.second, 0);
		trans->getSendBuffer()->moveHeader(iRet);
        session->buffer().clear();
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

//	cout << "http2Request id:" << request.iRequestId << endl;

	vector<char> out;
	session->swap(out);

	return out;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::grpcResponse(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	TC_GrpcClient* session = (TC_GrpcClient*)((Transceiver*)(in.getConnection()))->getSendBuffer()->getContextData();

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

