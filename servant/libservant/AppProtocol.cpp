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
#include "servant/TarsLogger.h"
#include "tup/Tars.h"
#include <iostream>

#if TARS_HTTP2
#include "util/tc_nghttp2.h"
// #include "util/tc_http2clientmgr.h"
#endif

namespace tars
{

//TAFServer的协议解析器
TC_NetWorkBuffer::PACKET_TYPE AppProtocol::parseAdmin(TC_NetWorkBuffer  &in, shared_ptr<TC_NetWorkBuffer::SendBuffer> &out)
{
    return parse(in, out->getBuffer());
}

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
    if (body->empty())
    {
        *data_flags |= NGHTTP2_DATA_FLAG_EOF;
        return 0;
    }

    ssize_t len = length > body->size() ? body->size() : length;
    std::memcpy(buf, &(*body)[0], len);
        
    vector<char>::iterator end = body->begin();
    std::advance(end, len);
    body->erase(body->begin(), end);

    return len;
}

vector<char> ProxyProtocol::http1Request(tars::RequestPacket& request, Transceiver *)
{
    TC_HttpRequest httpRequest;

    httpRequest.setRequest(request.sFuncName, request.sServantName, string(request.sBuffer.data(), request.sBuffer.size()), true);

    vector<char> buffer;

    httpRequest.encode(buffer);    

    return buffer;
}

TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http1Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
    TC_NetWorkBuffer::PACKET_TYPE flag = in.checkHttp();

    if(flag == TC_NetWorkBuffer::PACKET_FULL)
    {
        TC_HttpResponse httpRsp;
        vector<char> buffer = in.getBuffers();

        httpRsp.decode(buffer.data(), buffer.size());

        // ResponsePacket rsp;
        rsp.status["status"]  = httpRsp.getResponseHeaderLine();
        for (const auto& kv : httpRsp.getHeaders())
        {
            // 响应的头部 
            rsp.status[kv.first] = kv.second; 
        } 

        rsp.sBuffer.assign(httpRsp.getContent().begin(), httpRsp.getContent().end());
    }

    return flag;

    // done.push_back(rsp);
    // return httpRsp.getHeadLength() + httpRsp.getContentLength();
}

// vector<char> encodeHttp2(RequestPacket& request, TC_NgHttp2* session)
// {
//     std::vector<nghttp2_nv> nva;

//     const std::string method(":method");
//     nghttp2_nv nv1 = MAKE_STRING_NV(method, request.sFuncName);
//     if (!request.sFuncName.empty())
//         nva.push_back(nv1);

//     const std::string path(":path");
//     nghttp2_nv nv2 = MAKE_STRING_NV(path, request.sServantName);
//     if (!request.sServantName.empty())
//         nva.push_back(nv2);

//     for (std::map<std::string, std::string>::const_iterator
//                 it(request.context.begin());
//                 it != request.context.end();
//                 ++ it)
//     {
//         nghttp2_nv nv = MAKE_STRING_NV(it->first, it->second);
//         nva.push_back(nv);
//     }

//     nghttp2_data_provider* pData = NULL;
//     nghttp2_data_provider data;
//     if (!request.sBuffer.empty())
//     {
//         pData = &data;
//         data.source.ptr = (void*)&request.sBuffer;
//         data.read_callback = reqbody_read_callback;
//     }

//     int32_t sid = nghttp2_submit_request(session->session(),
//                                          NULL,
//                                          &nva[0],
//                                          nva.size(),
//                                          pData,
//                                          NULL);
//     if (sid < 0)
//     {
//         TLOGERROR("encodeHttp2::Fatal error: nghttp2_submit_request return: " << sid << endl);
//         return vector<char>();
//     }

//     request.iRequestId = sid;
//     nghttp2_session_send(session->session());
        
//     // 交给tars发送
//     // std::string out;
//     // out.swap(session->sendBuffer());
//     // return out;

//     vector<char> out;

//     out.assign(session->sendBuffer().begin(), session->sendBuffer().end());

//     return out;
// }

// ENCODE function, called by network thread
vector<char> ProxyProtocol::http2Request(RequestPacket& request, Transceiver *trans)
{
    cout << "http2Request" << endl;
    // TC_NgHttp2* session = Http2ClientSessionManager::getInstance()->getSession(request.iRequestId);
    TC_NgHttp2* session = trans->getHttp2Session();

    assert(session != NULL);

    cout << "http2Request:" << session << endl;

    if (session->getState() == TC_NgHttp2::None)
    {
        session->Init();
        session->settings();
    }

    assert (session->getState() == TC_NgHttp2::Http2);

    // return encodeHttp2(request, session);
    cout << "http2Request1" << endl;

    std::vector<nghttp2_nv> nva;

    const std::string method(":method");
    nghttp2_nv nv1 = MAKE_STRING_NV(method, request.sFuncName);
    if (!request.sFuncName.empty())
        nva.push_back(nv1);

    const std::string path(":path");
    nghttp2_nv nv2 = MAKE_STRING_NV(path, request.sServantName);
    if (!request.sServantName.empty())
        nva.push_back(nv2);

    for (std::map<std::string, std::string>::const_iterator
                it(request.context.begin());
                it != request.context.end();
                ++ it)
    {
        nghttp2_nv nv = MAKE_STRING_NV(it->first, it->second);
        nva.push_back(nv);
    }

    cout << "http2Request2" << endl;
    nghttp2_data_provider* pData = NULL;
    nghttp2_data_provider data;
    if (!request.sBuffer.empty())
    {
        pData = &data;
        data.source.ptr = (void*)&request.sBuffer;
        data.read_callback = reqbody_read_callback;
    }

    int32_t sid = nghttp2_submit_request(session->session(),
                                         NULL,
                                         &nva[0],
                                         nva.size(),
                                         pData,
                                         NULL);
    if (sid < 0)
    {
        TLOGERROR("[TARS]http2Request::Fatal error: nghttp2_submit_request return: " << sid << endl);
        return vector<char>();
    }

    cout << "http2Request3" << endl;
    request.iRequestId = sid;
    nghttp2_session_send(session->session());
        
    // 交给tars发送
    // std::string out;
    // out.swap(session->sendBuffer());
    // return out;

    vector<char> out;

    out.assign(session->sendBuffer().begin(), session->sendBuffer().end());

    cout << "http2Request4:" << out.data() << endl;
    return out;
}

// TC_NetWorkBuffer::PACKET_TYPE http2Response(TC_NetWorkBuffer &in, list<ResponsePacket>& done, void* userptr)
TC_NetWorkBuffer::PACKET_TYPE ProxyProtocol::http2Response(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
    TC_NgHttp2* session = ((Transceiver*)(in.getConnection()))->getHttp2Session();

    assert (session->getState() == TC_NgHttp2::Http2);

    auto it = session->doneResponses().begin();

    if(it == session->doneResponses().end())
    {
        vector<char> buffer = in.getBuffers();
        in.clearBuffers();

        // Transceiver* userptr = ((Transceiver*))in->getConnection();
        // int sessionId = userptr->getAdapterProxy()->getId();
        // TC_NgHttp2* session = Http2ClientSessionManager::getInstance()->getSession(sessionId);

        int readlen = nghttp2_session_mem_recv(session->session(), (const uint8_t*)buffer.data(), buffer.size());

        if (readlen < 0)
        {
            // throw std::runtime_error("nghttp2_session_mem_recv return error");
            return TC_NetWorkBuffer::PACKET_ERR;
        }
    }

    it = session->doneResponses().begin();
    if(it == session->doneResponses().end())
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }    

    rsp.iRequestId = it->second.streamId;
    rsp.status = it->second.headers;
    rsp.sBuffer.assign(it->second.body.begin(), it->second.body.end());

    session->doneResponses().erase(it);

    // std::map<int, Http2Response>::const_iterator it(session->_doneResponses.begin());
    // for (; it != session->_doneResponses.end(); ++ it)
    // {
    //     ResponsePacket rsp;
             
    //     rsp.iRequestId = it->second.streamId;
    //     rsp.status = it->second.headers;
    //     rsp.sBuffer.assign(it->second.body.begin(), it->second.body.end());

    //     done.push_back(rsp);
    // }

    // session->_doneResponses.clear();
    return TC_NetWorkBuffer::PACKET_FULL;
}

#endif
}

