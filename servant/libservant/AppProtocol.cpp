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
// #include "util/tc_http2clientmgr.h"
#endif

namespace tars
{

// //TAFServer的协议解析器
// TC_NetWorkBuffer::PACKET_TYPE AppProtocol::parseAdmin(TC_NetWorkBuffer  &in, shared_ptr<TC_NetWorkBuffer::Buffer> &out)
// {
//     return parse(in, out->getBuffer());
// }

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
#if TARS_HTTP2

#define MAKE_NV(NAME, VALUE, VALUELEN)                                         \
      {                                                                            \
              (uint8_t *)NAME, (uint8_t *)VALUE, sizeof(NAME) - 1, VALUELEN,             \
                  NGHTTP2_NV_FLAG_NONE                                                   \
            }

#define MAKE_NV2(NAME, VALUE)                                                  \
      {                                                                            \
              (uint8_t *)NAME, (uint8_t *)VALUE, sizeof(NAME) - 1, sizeof(VALUE) - 1,    \
                  NGHTTP2_NV_FLAG_NONE                                                   \
            }

#define MAKE_STRING_NV(NAME, VALUE) {(uint8_t*)(NAME.data()), (uint8_t*)(VALUE.data()), NAME.size(), VALUE.size(), NGHTTP2_NV_FLAG_NONE};

// nghttp2读取请求包体，准备发送
static ssize_t reqbody_read_callback(nghttp2_session *session, int32_t stream_id,
                                     uint8_t *buf, size_t length,
                                     uint32_t *data_flags,
                                     nghttp2_data_source *source,
                                     void *user_data)
{
    std::vector<char>* body = (std::vector<char>* )source->ptr;
    // cout << "reqbody_read_callback:" << body->size() << endl;

    if (body->empty())
    {
        *data_flags |= NGHTTP2_DATA_FLAG_EOF;
        return 0;
    }

    ssize_t len = length > body->size() ? body->size() : length;
    memcpy(buf, body->data(), len);
        
    vector<char>::iterator end = body->begin();
    std::advance(end, len);
    body->erase(body->begin(), end);

    return len;
}

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
    string buff;

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

    context->buff.append(in.getBuffersString());
    in.clearBuffers();

    if(context->httpRsp.incrementDecode(context->buff))
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

// ENCODE function, called by network thread
vector<char> ProxyProtocol::http2Request(RequestPacket& request, Transceiver *trans)
{
    TC_Http2Client* session = trans->getHttp2Client();

    std::vector<nghttp2_nv> nva;

    const std::string method(":method");
    nghttp2_nv nv1 = MAKE_STRING_NV(method, request.sFuncName);
    if (!request.sFuncName.empty())
        nva.push_back(nv1);

    const std::string path(":path");
    nghttp2_nv nv2 = MAKE_STRING_NV(path, request.sServantName);
    if (!request.sServantName.empty())
        nva.push_back(nv2);

    for (std::map<std::string, std::string>::const_iterator it(request.context.begin()); it != request.context.end(); ++ it)
    {
        nghttp2_nv nv = MAKE_STRING_NV(it->first, it->second);
        nva.push_back(nv);
    }

    nghttp2_data_provider* pData = NULL;
    nghttp2_data_provider data;
    if (!request.sBuffer.empty())
    {
        pData = &data;
        data.source.ptr = (void*)&request.sBuffer;
        data.read_callback = reqbody_read_callback;
    }

// cout << "pData:" << pData << ", " << data.read_callback << endl;
    int32_t sid = nghttp2_submit_request(session->session(),
                                         NULL,
                                         nva.data(),
                                         nva.size(),
                                         pData,
                                         NULL);
    if (sid < 0)
    {
        TLOGERROR("[TARS]http2Request::Fatal error: nghttp2_submit_request return: " << sid << endl);
        return vector<char>();
    }

    request.iRequestId = sid;

    int rv = nghttp2_session_send(session->session());
    if (rv != 0) {
        TLOGERROR("[TARS]http2Request::Fatal error: nghttp2_session_send return: " << nghttp2_strerror(rv) << endl);
        return vector<char>();
    }
    // cout << "nghttp2_session_send, id:" << request.iRequestId << ", buff size:" << session->_buffer().size() << endl;

    // if(session->_buffer().empty())
    // {
    //     exit(0);
    // }
    // get data to send
    vector<char> out;
    out.swap(session->sendBuffer());
    
    return out;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http2Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
    // cout << "http2Response" << endl;

    TC_Http2Client* session = ((Transceiver*)(in.getConnection()))->getHttp2Client();

    auto it = session->doneResponses().begin();

    if(it == session->doneResponses().end() && !in.empty())
    {
        if(in.empty())
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }
        //merge to one buffer
        in.mergeBuffers();

        pair<const char*, size_t> buffer = in.getBufferPointer();

        int readlen = nghttp2_session_mem_recv(session->session(), (const uint8_t*)buffer.first, buffer.second);

        // vector<char> buffer = in.getBuffers();

        // int readlen = nghttp2_session_mem_recv(session->session(), (const uint8_t*)buffer.data(), buffer.size());

        if (readlen < 0)
        {
            TLOGERROR("[TARS]http2Response::Fatal error: nghttp2_session_mem_recv error:" << nghttp2_strerror((int)readlen) << endl);
            return TC_NetWorkBuffer::PACKET_ERR;
        }

        in.moveHeader(readlen);
    }

    if(session->doneResponses().empty())
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }    

    it = session->doneResponses().begin();
    rsp.iRequestId = it->second.streamId;
    rsp.status = it->second.headers;
    rsp.sBuffer.swap(it->second.body);

    session->doneResponses().erase(it);

    return TC_NetWorkBuffer::PACKET_FULL;
}

#endif
}

