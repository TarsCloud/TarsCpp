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
// #include "servant/TC_Transceiver.h"
#include "servant/AdapterProxy.h"
#include "servant/RemoteLogger.h"
#include "tup/Tars.h"
#include <iostream>

#if TARS_HTTP2
#include "util/tc_http2.h"
#endif

namespace tars
{

class TC_Transceiver;

shared_ptr<TC_NetWorkBuffer::Buffer> ProxyProtocol::toBuffer(TarsOutputStream<BufferWriter> &os)
{
	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	buff->replaceBuffer(os.getBuffer(), os.getLength());
	os._buf = NULL;
	os._buf_len = 0;
	os._len = 0;

	return buff;
}

shared_ptr<TC_NetWorkBuffer::Buffer> ProxyProtocol::tarsRequest(RequestPacket& request, TC_Transceiver *)
{
	TarsOutputStream<BufferWriter> os;

	int iHeaderLen = 0;

//	先预留4个字节长度
	os.writeBuf((const char *)&iHeaderLen, sizeof(iHeaderLen));

	request.writeTo(os);

	assert(os.getLength() >= 4);

	iHeaderLen = htonl((int)(os.getLength()));

	memcpy((void*)os.getBuffer(), (const char *)&iHeaderLen, sizeof(iHeaderLen));

	return toBuffer(os);
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::tupResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp, uint32_t iMinLength, uint32_t iMaxLength)
{
	uint32_t len = (uint32_t)in.getBufferLength();

	//收到的字节数太少, 还需要继续接收
	if(len < sizeof(uint32_t))
		return TC_NetWorkBuffer::PACKET_LESS;

	//获取包总体长度
	uint32_t iHeaderLen = in.getValueOf4();

	//做一下保护,长度大于10M
	if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
	{
		throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
	}

	//包没有接收全
	if (len < iHeaderLen)
	{
		//看看包头是否正确
		static const uint32_t head = 20;

		if (len >= head)
		{
			string buffer;
			in.getHeader(head, buffer);

			TarsInputStream<BufferReader> is;

			is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

			//tup回来是requestpackage
			RequestPacket rsp;

			is.read(rsp.iVersion, 1, true);

			if (rsp.iVersion != TUPVERSION)
			{
				throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
			}

			is.read(rsp.cPacketType, 2, true);

			if (rsp.cPacketType != TARSNORMAL)
			{
				throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
			}

			is.read(rsp.iMessageType, 3, true);
			is.read(rsp.iRequestId, 4, true);
		}
		return TC_NetWorkBuffer::PACKET_LESS;
	}
	else
	{
		//buffer包括4个字节长度
		vector<char> buffer;
		buffer.resize(iHeaderLen);

		in.getHeader(iHeaderLen, buffer);
		TarsInputStream<BufferReader> is;

		is.setBuffer(buffer.data() + sizeof(tars::Int32), buffer.size() - sizeof(tars::Int32));

		//TUP的响应包其实也是返回包
		RequestPacket req;
		req.readFrom(is);

		if (req.iVersion != TUPVERSION )
		{
			throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(req.iVersion));
		}

		if (req.cPacketType != TARSNORMAL)
		{
			throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)req.cPacketType));
		}

		rsp.cPacketType     = req.cPacketType;
		rsp.iMessageType    = req.iMessageType;
		rsp.iRequestId      = req.iRequestId;
		rsp.iVersion        = req.iVersion;
		rsp.context         = req.context;
		//tup的响应包直接放入到sBuffer里面
		rsp.sBuffer         = buffer;

		in.moveHeader(iHeaderLen);
	}

	return TC_NetWorkBuffer::PACKET_FULL;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::jsonResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp, uint32_t iMinLength, uint32_t iMaxLength)
{
	uint32_t len = (uint32_t)in.getBufferLength();

	//收到的字节数太少, 还需要继续接收
	if (len < sizeof(uint32_t))
		return TC_NetWorkBuffer::PACKET_LESS;

	//获取包总体长度
	uint32_t iHeaderLen = in.getValueOf4();

	//做一下保护,长度大于10M
	if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
	{
		throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
	}

	//包没有接收全
	if (len < iHeaderLen)
	{
		//看看包头是否正确
		static const uint32_t head = 20;

		if (len >= head)
		{
			string buffer;
			in.getHeader(head, buffer);

			TarsInputStream<BufferReader> is;
			is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

			is.read(rsp.iVersion, 1, false);

			if (rsp.iVersion != JSONVERSION)
			{
				throw TarsDecodeException("json version not correct, version:" + TC_Common::tostr(rsp.iVersion));
			}

			is.read(rsp.cPacketType, 2, false);

			if (rsp.cPacketType != TARSNORMAL)
			{
				throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
			}

			is.read(rsp.iMessageType, 3, false);
			is.read(rsp.iRequestId, 4, false);
			is.read(rsp.iRet, 5, false);

			if (rsp.iRet < TARSSERVERUNKNOWNERR)
			{
				throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
			}
		}

		return TC_NetWorkBuffer::PACKET_LESS;
	}
	else
	{
		vector<char> buffer;
		bool ret = in.parseBufferOf4(buffer, iMinLength, iMaxLength);
		if (!ret)
		{
			throw TarsDecodeException("parse buffer exception");
		}

		TarsInputStream<BufferReader> is;
		is.setBuffer(buffer.data(), buffer.size());

		rsp.readFrom(is);

		if (rsp.iVersion != JSONVERSION)
		{
			throw TarsDecodeException("json version not correct, version:" + TC_Common::tostr(rsp.iVersion));
		}

		if (rsp.cPacketType != TARSNORMAL)
		{
			throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
		}

		if (rsp.iRet < TARSSERVERUNKNOWNERR)
		{
			throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
		}

	}

	return TC_NetWorkBuffer::PACKET_FULL;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::tarsResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp, uint32_t iMinLength, uint32_t iMaxLength)
{
	uint32_t len = (uint32_t)in.getBufferLength();

	//收到的字节数太少, 还需要继续接收
	if(len < sizeof(uint32_t))
		return TC_NetWorkBuffer::PACKET_LESS;

	//获取包总体长度
	uint32_t iHeaderLen = in.getValueOf4();

	//做一下保护,长度大于10M
	if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
	{
		throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
	}

	//包没有接收全
	if (len < iHeaderLen)
	{
		//看看包头是否正确
		static const uint32_t head = 20;

		if (len >= head)
		{
			string buffer;
			in.getHeader(head, buffer);

			TarsInputStream<BufferReader> is;
			is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

			// ResponsePacket rsp;
			is.read(rsp.iVersion, 1, false);

			if (rsp.iVersion != TARSVERSION)
			{
				throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
			}

			is.read(rsp.cPacketType, 2, false);

			if (rsp.cPacketType != TARSNORMAL)
			{
				throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
			}

			is.read(rsp.iRequestId, 3, false);
			is.read(rsp.iMessageType, 4, false);
			is.read(rsp.iRet, 5, false);

			if (rsp.iRet < TARSSERVERUNKNOWNERR)
			{
				throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
			}
		}

		return TC_NetWorkBuffer::PACKET_LESS;
	}
	else
	{
		vector<char> buffer;
		auto ret = in.parseBufferOf4(buffer, iMinLength, iMaxLength);
		if (ret == TC_NetWorkBuffer::PACKET_LESS)
		{
			throw TarsDecodeException("parse buffer exception");
		}

		TarsInputStream<BufferReader> is;
		is.setBuffer(buffer.data(), buffer.size());

		rsp.readFrom(is);

		if (rsp.iVersion != TARSVERSION)
		{
			throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
		}

		if (rsp.cPacketType != TARSNORMAL)
		{
			throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
		}

		if (rsp.iRet < TARSSERVERUNKNOWNERR)
		{
			throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
		}
	}

	return TC_NetWorkBuffer::PACKET_FULL;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::totalResponseLen(TC_NetWorkBuffer &in, ResponsePacket &rsp, uint32_t iMinLength, uint32_t iMaxLength)
{
	uint32_t len = (uint32_t)in.getBufferLength();

	//收到的字节数太少, 还需要继续接收
	if (len < sizeof(uint32_t))
		return TC_NetWorkBuffer::PACKET_LESS;

	//获取包总体长度
	uint32_t iHeaderLen = in.getValueOf4();

	//做一下保护,长度大于10M
	if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
	{
		throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
	}

	//包没有接收全
	if (len < iHeaderLen)
	{
		//看看包头是否正确
		static const uint32_t head = 18;

		if (len >= head)
		{
			string buffer;
			in.getHeader(head, buffer);

			TarsInputStream<BufferReader> is;
			is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

			// ResponsePacket rsp;
			is.read(rsp.iVersion, 1, false);

			if (rsp.iVersion != TARSVERSION && rsp.iVersion != TUPVERSION && rsp.iVersion != JSONVERSION)
			{
				throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
			}

			is.read(rsp.cPacketType, 2, false);

			if (rsp.cPacketType != TARSNORMAL)
			{
				throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
			}

			is.read(rsp.iRequestId, 3, false);
			is.read(rsp.iMessageType, 4, false);
			is.read(rsp.iRet, 5, false);

			if (rsp.iRet < TARSSERVERUNKNOWNERR)
			{
				throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
			}
		}

		return TC_NetWorkBuffer::PACKET_LESS;
	}
	else
	{
		//看看包头是否正确
		string buffer;
		in.getHeader(iHeaderLen, buffer);

		TarsInputStream<BufferReader> is;
		is.setBuffer(buffer.c_str() + sizeof(tars::Int32), iHeaderLen-sizeof(tars::Int32));

		is.read(rsp.iVersion, 1, false);

		if (rsp.iVersion == TUPVERSION)
		{
			//buffer包括4个字节长度
			vector<char> buffer;
			buffer.resize(iHeaderLen);

			in.getHeader(iHeaderLen, buffer);
			TarsInputStream<BufferReader> is;

			is.setBuffer(buffer.data() + sizeof(tars::Int32), buffer.size() - sizeof(tars::Int32));

			//TUP的响应包其实也是返回包
			RequestPacket req;
			req.readFrom(is);

			if (req.cPacketType != TARSNORMAL)
			{
				throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)req.cPacketType));
			}

			rsp.cPacketType = req.cPacketType;
			rsp.iMessageType = req.iMessageType;
			rsp.iRequestId = req.iRequestId;
			rsp.iVersion = req.iVersion;
			rsp.context = req.context;
			//tup的响应包直接放入到sBuffer里面
			rsp.sBuffer = buffer;

			in.moveHeader(iHeaderLen);
		}
		else if (rsp.iVersion == TARSVERSION || rsp.iVersion == JSONVERSION)
		{
			vector<char> buffer;
			bool ret = in.parseBufferOf4(buffer, iMinLength, iMaxLength);
			if (!ret)
			{
				throw TarsDecodeException("parse buffer exception");
			}

			TarsInputStream<BufferReader> is;
			is.setBuffer(buffer.data(), buffer.size());

			rsp.readFrom(is);

			if (rsp.cPacketType != TARSNORMAL)
			{
				throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
			}

			if (rsp.iRet < TARSSERVERUNKNOWNERR)
			{
				throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
			}
		}
		else
		{
			throw TarsDecodeException("===>version not correct, version:" + TC_Common::tostr(rsp.iVersion));
		}
	}

	return TC_NetWorkBuffer::PACKET_FULL;
}

////////////////////////////////////////////////////////////////////////////////////

shared_ptr<TC_NetWorkBuffer::Buffer> ProxyProtocol::http1Request(tars::RequestPacket& request, TC_Transceiver *trans)
{
	shared_ptr<TC_HttpRequest> &data = *(shared_ptr<TC_HttpRequest>*)request.sBuffer.data();

	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	if(!data->hasHeader("Host")) {
		data->setHost(trans->getEndpoint().getHost());
	}
	data->encode(buff);

	data.reset();

	return buff;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http1Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	shared_ptr<TC_HttpResponse> *context = (shared_ptr<TC_HttpResponse>*)(in.getContextData());

    if(!context)
    {
        context = new shared_ptr<TC_HttpResponse>();
        *context = std::make_shared<TC_HttpResponse>();
		in.setContextData(context, [](TC_NetWorkBuffer*nb){ shared_ptr<TC_HttpResponse> *p = (shared_ptr<TC_HttpResponse>*)(nb->getContextData()); if(p) { nb->setContextData(NULL); delete p; }});
    }

    if((*context)->incrementDecode(in))
    {
	    rsp.sBuffer.resize(sizeof(shared_ptr<TC_HttpResponse>));

	    shared_ptr<TC_HttpResponse> &data = *(shared_ptr<TC_HttpResponse>*)rsp.sBuffer.data();

	    data = *context;

		auto ret = TC_NetWorkBuffer::PACKET_FULL;
		if (data->checkHeader("Connection", "keep-alive") || (data->getVersion() == "HTTP/1.1" && !data->hasHeader("Connection")))
		{
			ret = TC_NetWorkBuffer::PACKET_FULL;
		}
		else
		{
			ret = TC_NetWorkBuffer::PACKET_FULL_CLOSE;
		}

		//收取到完整的包, 把当前包释放掉, 下次新包来会新建context
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
shared_ptr<TC_NetWorkBuffer::Buffer> ProxyProtocol::http2Request(RequestPacket& request, TC_Transceiver *trans)
{
    TC_Http2Client* session = (TC_Http2Client*)trans->getSendBuffer().getContextData();
	if(session == NULL)
	{
		session = new TC_Http2Client();

		trans->getSendBuffer().setContextData(session, [=](TC_NetWorkBuffer*){delete session;});

		session->settings(3000);
	}

	shared_ptr<TC_HttpRequest> *data = (shared_ptr<TC_HttpRequest>*)request.sBuffer.data();

	request.iRequestId = session->submit(*(*data).get());

	//这里把智能指针释放一次
	(*data).reset();

	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	if (request.iRequestId < 0)
	{
		TLOGERROR("http2Request::Fatal submit error: " << session->getErrMsg() << endl);
		return buff;
	}

	vector<char> out;
	session->swap(out);

	buff->addBuffer(out);

	return buff;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http2Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	TC_Http2Client* session = (TC_Http2Client*)((TC_Transceiver*)(in.getConnection()))->getSendBuffer().getContextData();

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
shared_ptr<TC_NetWorkBuffer::Buffer> ProxyProtocol::grpcRequest(RequestPacket& request, TC_Transceiver *trans)
{
    TC_GrpcClient* session = (TC_GrpcClient*)trans->getSendBuffer().getContextData();
	if(session == NULL)
	{
		session = new TC_GrpcClient();

		trans->getSendBuffer().setContextData(session, [=](TC_NetWorkBuffer*){ delete session; });

		session->settings(3000);
	}

    if (session->buffer().size() != 0) {
        //直接发送裸得应答数据，业务层一般不直接使用，仅仅tcp支持
		trans->getSendBuffer().addBuffer(session->buffer());

		trans->doRequest();

//		auto data = trans->getSendBuffer().getBufferPointer();
//		int iRet = trans->send(data.first, (uint32_t) data.second, 0);
//		trans->getSendBuffer().moveHeader(iRet);
        session->buffer().clear();
    }

	shared_ptr<TC_HttpRequest> *data = (shared_ptr<TC_HttpRequest>*)request.sBuffer.data();

	request.iRequestId = session->submit(*(*data).get());

	//这里把智能指针释放一次
	(*data).reset();

	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	if (request.iRequestId < 0)
	{
		TLOGERROR("http2Request::Fatal submit error: " << session->getErrMsg() << endl);
		return buff;
	}

//	cout << "http2Request id:" << request.iRequestId << endl;

	vector<char> out;
	session->swap(out);

	buff->addBuffer(out);

	return buff;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::grpcResponse(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	TC_GrpcClient* session = (TC_GrpcClient*)((TC_Transceiver*)(in.getConnection()))->getSendBuffer().getContextData();

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

