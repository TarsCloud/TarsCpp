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
#if TARS_HTTP2
#include <string>
#include <iostream>
#include "util/tc_grpc.h"

#include "nghttp2/nghttp2.h"
namespace tars
{

TC_SpinLock TC_GrpcServer::_mutex;

unordered_map<int32_t, shared_ptr<TC_GrpcServer>> TC_GrpcServer::_http2;

void addGrpcPrefix(string& body, bool compressed)
{
    char buf[5];
    buf[0] = (compressed ? 1: 0);
    *(uint32_t*)(buf+1) = htonl(body.size());
    string message;
    message.append(buf, sizeof(buf));
    message.append(body);
    body.swap(message);
}

bool RemoveGrpcPrefix(string& body, bool* compressed)
{
    if (body.empty()) {
        *compressed = false;
        return true;
    }

    const size_t sz = body.size();
    if (sz <(size_t)5) {
        return false;
    }

    char buf[5];
    body.copy(buf, 5, 0);
    *compressed = buf[0];
    const size_t message_len = ntohl(*(uint32_t*)(buf+1));
    body.erase(0, 5);

    return (message_len + 5 == sz);
}

namespace server
{

static ssize_t str_read_callback(nghttp2_session *session, int32_t stream_id,
                                  uint8_t *buf, size_t length,
                                  uint32_t *data_flags,
                                  nghttp2_data_source *source,
                                  void *user_data) 
{
    TC_Http2::DataPack *dataPack = (TC_Http2::DataPack*)(source->ptr);
	if(dataPack->_readPos == dataPack->_length)
	{
		*data_flags |= NGHTTP2_DATA_FLAG_EOF;
		return 0;
	}
    size_t size = std::min(dataPack->_length - dataPack->_readPos, length);

    memcpy(buf, dataPack->_dataBuf + dataPack->_readPos, size);

    dataPack->_readPos += size;
	
    if(dataPack->_readPos == dataPack->_length)
    {
        *data_flags |= NGHTTP2_DATA_FLAG_EOF;
    }
  
    return size;
}

static ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
                             size_t length, int flags, void *user_data) 
{
    TC_GrpcServer *ptr = (TC_GrpcServer*)user_data;
	ptr->insertBuff((const char*)data, length);

    return (ssize_t)length;
}

static int on_header_callback(nghttp2_session *session,
                              const nghttp2_frame *frame, const uint8_t *name,
                              size_t namelen, const uint8_t *value,
                              size_t valuelen, uint8_t flags, void *user_data)
{
    TC_GrpcServer *ptr = (TC_GrpcServer*)user_data;
    ptr->onHeaderCallback(frame->hd.stream_id, string((char*)name, namelen), string((char*)value, valuelen));
    return 0;
}

static int on_begin_headers_callback(nghttp2_session *session,
                                     const nghttp2_frame *frame,
                                     void *user_data)
{
    TC_GrpcServer *ptr = (TC_GrpcServer*)user_data;

    if (frame->hd.type != NGHTTP2_HEADERS || frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
        return 0;
    }

    ptr->onHeaderCallback(frame->hd.stream_id);
    return 0;
}

static int on_frame_recv_callback(nghttp2_session *session, const nghttp2_frame *frame, void *user_data)
{
    TC_GrpcServer *ptr = (TC_GrpcServer*)user_data;

    switch (frame->hd.type)
    {
    case NGHTTP2_DATA:
    case NGHTTP2_HEADERS:
        /* Check that the client request has finished */
        if (frame->hd.flags & NGHTTP2_FLAG_END_STREAM)
        {
            ptr->onFrameRecvCallback(frame->hd.stream_id);
            return 0;
        }
        break;
    case NGHTTP2_WINDOW_UPDATE:
        nghttp2_session_send(session);
        break;
    case NGHTTP2_PING:
        nghttp2_session_send(session);
        break;
    default:
        break;
    }
    return 0;
}

static int on_data_chunk_recv_callback(nghttp2_session *session, uint8_t flags,
                                       int32_t stream_id, const uint8_t *data,
                                       size_t len, void *user_data)
{
    TC_GrpcServer *ptr = (TC_GrpcServer*)user_data;

    ptr->onDataChunkRecvCallback(stream_id, (const char*)data, len);

    return 0;
}

static int on_stream_close_callback(nghttp2_session *session, int32_t stream_id, uint32_t error_code, void *user_data)
{
    TC_GrpcServer *ptr = (TC_GrpcServer*)user_data;
    ptr->onStreamCloseCallback(stream_id);

    return 0;
}
}   //namespace server


TC_GrpcServer::TC_GrpcServer()
{
    nghttp2_session_callbacks *callbacks;

    nghttp2_session_callbacks_new(&callbacks);

    nghttp2_session_callbacks_set_send_callback(callbacks, server::send_callback);

    nghttp2_session_callbacks_set_on_frame_recv_callback(callbacks, server::on_frame_recv_callback);

    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(callbacks, server::on_data_chunk_recv_callback);

    nghttp2_session_callbacks_set_on_stream_close_callback(callbacks, server::on_stream_close_callback);

    nghttp2_session_callbacks_set_on_header_callback(callbacks, server::on_header_callback);

    nghttp2_session_callbacks_set_on_begin_headers_callback(callbacks, server::on_begin_headers_callback);

    nghttp2_session_server_new(&_session, callbacks, ((void*)this));

    nghttp2_session_callbacks_del(callbacks);
}

TC_GrpcServer::~TC_GrpcServer()
{
}


int TC_GrpcServer::encodeResponse(const shared_ptr<TC_GrpcServer::Http2Context> &context, std::string gStatus, vector<char> &out)
{
	string sstatus = TC_Common::tostr(context->response.getStatus());

	const char* strstatus = ":status";

	nghttp2_nv *hdrs    = new nghttp2_nv[context->response.getHeaders().size() + 1];
	hdrs[0].flags       = NGHTTP2_NV_FLAG_NONE;
	hdrs[0].name        = (uint8_t*)strstatus;
	hdrs[0].namelen     = 7;
	hdrs[0].value       = (uint8_t*)sstatus.c_str();
	hdrs[0].valuelen    = sstatus.size();

	TC_Http::http_header_type::const_iterator it = context->response.getHeaders().begin();
	for (int n = 1; it != context->response.getHeaders().end(); n++, it++)
	{
		hdrs[n].flags       = NGHTTP2_NV_FLAG_NONE;
		hdrs[n].name        = (uint8_t*)it->first.c_str();
		hdrs[n].namelen     = it->first.size();
		hdrs[n].value       = (uint8_t*)it->second.c_str();
		hdrs[n].valuelen    = it->second.size();
	}

	const string &data = context->response.getContent();
	DataPack dataPack(data.c_str(), data.size());

	nghttp2_data_provider data_prd;
	data_prd.source.ptr     = (void*)&dataPack;
	data_prd.read_callback  = server::str_read_callback;
	{
		TC_LockT<TC_ThreadMutex> lock(_nghttpLock);

		_err = nghttp2_submit_headers(_session, NGHTTP2_FLAG_NONE, context->reqId, nullptr,
										hdrs, context->response.getHeaders().size()+1, nullptr);
		if (_err != 0 ) {
			delete [] hdrs;
			return _err;
		}


		_err = nghttp2_submit_data(_session, NGHTTP2_FLAG_NONE, context->reqId, &data_prd);
		if (_err != 0 ) {
			delete [] hdrs;
			return _err;
		}

		nghttp2_session_send(_session);

		const char* grpcStatus = "grpc-status";
		const char* gstatus = gStatus.c_str();

		nghttp2_nv hdr[1];
		hdr[0].flags       = NGHTTP2_NV_FLAG_NONE;
		hdr[0].name        = (uint8_t*)grpcStatus;
		hdr[0].namelen     = 11;
		hdr[0].value       = (uint8_t*)gstatus;
		hdr[0].valuelen    = 1;

		_err = nghttp2_submit_headers(_session, NGHTTP2_FLAG_END_STREAM, context->reqId, nullptr,
										hdr, 1, nullptr);
		if (_err != 0 ) {
			delete [] hdrs;
			return _err;
		}
		
		
		while (nghttp2_session_want_write(_session)) {
			_err = nghttp2_session_send(_session);
			if (_err != 0) {
				delete [] hdrs;

				return _err;
			}
		}

		this->swap(out);
	}

	delete [] hdrs;

	return 0;
}

void TC_GrpcServer::packGrpcResponse(shared_ptr<TC_GrpcServer::Http2Context> &context, const int status, const string &body)
{
    context->response.setHeader("content-type", "application/grpc");
    context->response.setResponse(status, "OK", body);
}

shared_ptr<TC_GrpcServer> TC_GrpcServer::getHttp2(uint32_t uid)
{
    TC_LockT<TC_SpinLock> lock(_mutex);

    auto it = _http2.find(uid);

    if(it != _http2.end())
    {
        return it->second;
    }
    return NULL;
}

void TC_GrpcServer::addHttp2(uint32_t uid, const shared_ptr<TC_GrpcServer> &ptr)
{
    TC_LockT<TC_SpinLock> lock(_mutex);

    _http2[uid] = ptr;
}

void TC_GrpcServer::delHttp2(uint32_t uid)
{
    TC_LockT<TC_SpinLock> lock(_mutex);

    auto it = _http2.find(uid);

    if(it != _http2.end())
    {
        _http2.erase(it);
    }
}

TC_NetWorkBuffer::PACKET_TYPE TC_GrpcServer::parseGrpc(TC_NetWorkBuffer&in, vector<char> &out)
{
    TC_GrpcServer*sessionPtr = (TC_GrpcServer*)(in.getContextData());
    TC_EpollServer::Connection *connection = (TC_EpollServer::Connection *)in.getConnection();

    if(sessionPtr == NULL)
    {
    	shared_ptr<TC_GrpcServer> session(new TC_GrpcServer());
	    in.setContextData(session.get());
	    session->settings(3000);
        addHttp2(connection->getId(), session);

	    sessionPtr = session.get();
    }

    if (sessionPtr->buffer().size() != 0) {
        //直接发送裸得应答数据，业务层一般不直接使用，仅仅tcp支持
        connection->sendBufferDirect(sessionPtr->buffer());
        sessionPtr->buffer().clear();
    }

    std::string inStr = in.getBuffersString();

    auto ret = sessionPtr->parse(in, out);
    
    if (sessionPtr->buffer().size() != 0) {
        //直接发送裸得应答数据，业务层一般不直接使用，仅仅tcp支持
        connection->sendBufferDirect(sessionPtr->buffer());
        sessionPtr->buffer().clear();
    }

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////

namespace client
{
static ssize_t send_callback(nghttp2_session* session, const uint8_t* data, size_t length, int flags, void* user_data)
{
    TC_GrpcClient* nghttp2 = (TC_GrpcClient* )user_data;
    nghttp2->insertBuff((const char*)data, length);

    return length;
}

static int on_begin_headers_callback(nghttp2_session* session, const nghttp2_frame* frame, void* user_data)
{
    TC_GrpcClient* nghttp2 = (TC_GrpcClient* )user_data;

    if (frame->hd.type == NGHTTP2_HEADERS)
    {
        if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE)
        {
            nghttp2->responses()[frame->hd.stream_id] = std::make_shared<TC_HttpResponse>();
        }
    }

    return 0;
}

static int on_header_callback(nghttp2_session* session, const nghttp2_frame* frame,
                         const uint8_t* name, size_t namelen,
                         const uint8_t* value, size_t valuelen,
                         uint8_t flags, void* user_data)
{
    TC_GrpcClient* nghttp2 = (TC_GrpcClient* )user_data;

    int streamId = frame->hd.stream_id;
    auto it = nghttp2->responses().find(streamId);
    if (it == nghttp2->responses().end())
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

    std::string n((const char*)name, namelen);
    std::string v((const char*)value, valuelen);

    it->second->setHeader(n, v);

    return 0;
}

static int on_frame_recv_callback(nghttp2_session* session, const nghttp2_frame* frame, void* user_data)
{
    TC_GrpcClient* nghttp2 = (TC_GrpcClient* )user_data;

    int streamId = frame->hd.stream_id;
    if (streamId == 0)
        return 0;

    auto it = nghttp2->responses().find(streamId);
    if (it == nghttp2->responses().end())
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

   
    switch (frame->hd.type)
    {
        case NGHTTP2_HEADERS:
            if (frame->hd.flags & NGHTTP2_FLAG_END_HEADERS)
            {
            	;
            }
            return 0;
        case NGHTTP2_WINDOW_UPDATE:
            nghttp2_session_send(session);
            break;
        case NGHTTP2_PING:
            nghttp2_session_send(session);
            break;
        default:
            break;
    }

    return 0;
}

static int on_data_chunk_recv_callback(nghttp2_session* session, uint8_t flags, int32_t stream_id, const uint8_t* data, size_t len, void* user_data)
{
    TC_GrpcClient* nghttp2 = (TC_GrpcClient* )user_data;

    auto it = nghttp2->responses().find(stream_id);
    if (it == nghttp2->responses().end())
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

    it->second->appendContent((const char* )data, len);
    return 0;
}

static int on_stream_close_callback(nghttp2_session* session, int32_t stream_id, uint32_t error_code, void* user_data)
{
    TC_GrpcClient* nghttp2 = (TC_GrpcClient* )user_data;

    auto it = nghttp2->responses().find(stream_id);
    if (it == nghttp2->responses().end())
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

	nghttp2->doneResponses()[stream_id] = it->second;
	nghttp2->responses().erase(it);

    return 0;
}

}   //namespace client

#define MAKE_STRING_NV(NAME, VALUE) {(uint8_t*)(NAME.c_str()), (uint8_t*)(VALUE.c_str()), NAME.size(), VALUE.size(), NGHTTP2_NV_FLAG_NONE};

TC_GrpcClient::TC_GrpcClient()
{
    nghttp2_session_callbacks* callbacks;
    nghttp2_session_callbacks_new(&callbacks);
    nghttp2_session_callbacks_set_send_callback(callbacks, client::send_callback);
    nghttp2_session_callbacks_set_on_begin_headers_callback(callbacks, client::on_begin_headers_callback);
    nghttp2_session_callbacks_set_on_header_callback(callbacks, client::on_header_callback);
    nghttp2_session_callbacks_set_on_frame_recv_callback(callbacks, client::on_frame_recv_callback);
    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(callbacks, client::on_data_chunk_recv_callback);
    nghttp2_session_callbacks_set_on_stream_close_callback(callbacks, client::on_stream_close_callback);

    nghttp2_session_client_new(&_session, callbacks, this);

    nghttp2_session_callbacks_del(callbacks);
}

TC_GrpcClient::~TC_GrpcClient()
{
}


int TC_GrpcClient::submit(const TC_HttpRequest &request)
{
	std::vector<nghttp2_nv> nva;

	//注意MAKE_STRING_NV不能用临时变量, 这里出过几次问题了!
	const std::string smethod(":method");
	const std::string vmethod(request.getMethod());
	nghttp2_nv nv1 = MAKE_STRING_NV(smethod, vmethod);
	nva.push_back(nv1);

	// const std::string spath(":path");
	// const std::string vpath(request.getRequest());
	// nghttp2_nv nv2 = MAKE_STRING_NV(spath, vpath);
	// nva.push_back(nv2);

	const std::string sauthority(":authority");
    string sPort = request.getURL().isDefaultPort() ? "" : ":" + request.getURL().getPort();
	const std::string vauthority(request.getURL().getDomain() + sPort);
	nghttp2_nv nv3 = MAKE_STRING_NV(sauthority, vauthority);
	nva.push_back(nv3);

	const std::string sscheme(":scheme");
	const std::string vscheme(request.getURL().getScheme());
	nghttp2_nv nv4 = MAKE_STRING_NV(sscheme, vscheme);
	nva.push_back(nv4);

	const TC_Http::http_header_type &header = request.getHeaders();
	for (auto it = header.begin(); it != header.end(); ++ it)
	{
		if(TC_Port::strcasecmp(it->first.c_str(), "Content-Length") == 0 || it->second.empty())
			continue;

		nghttp2_nv nv = MAKE_STRING_NV(it->first, it->second);
		nva.push_back(nv);
	}

	nghttp2_data_provider* pData = NULL;
	nghttp2_data_provider data;

	DataPack dataPack(request.getContent().c_str(), request.getContent().size());

	if (!request.getContent().empty())
	{
		pData = &data;
		data.source.ptr = (void*)&dataPack;
		data.read_callback = server::str_read_callback;
	}

	_err = nghttp2_submit_request(_session,
	                              NULL,
	                              nva.data(),
	                              nva.size(),
	                              pData,
	                              NULL);
	if (_err < 0)
	{
		return _err;
	}

	int sid = _err;

	_err = nghttp2_session_send(_session);
	if (_err != 0) {
		return _err;
	}

	return sid;
}


TC_NetWorkBuffer::PACKET_TYPE TC_GrpcClient::parseResponse(TC_NetWorkBuffer &in, pair<int, shared_ptr<TC_HttpResponse>> &out)
{
	if(_doneResponses.empty() && in.empty())
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	if(!in.empty())
	{
		//merge to one buffer
		in.mergeBuffers();

		pair<const char *, size_t> buffer = in.getBufferPointer();

		int readlen = nghttp2_session_mem_recv(_session, (const uint8_t *) buffer.first, buffer.second);
		if (readlen < 0) {
			return TC_NetWorkBuffer::PACKET_ERR;
		}

		in.moveHeader(readlen);
	}

	if(_doneResponses.empty())
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	auto it = _doneResponses.begin();
	out.first = it->first;
	out.second = it->second;

	_doneResponses.erase(it);
	return TC_NetWorkBuffer::PACKET_FULL;

}

}   //namespace tars

#endif // end #if TARS_HTTP2