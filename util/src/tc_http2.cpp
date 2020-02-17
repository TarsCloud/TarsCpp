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
#include <algorithm>
#include <iostream>
#include "util/tc_http2.h"
#include "nghttp2/nghttp2.h"

namespace tars
{

TC_Http2::TC_Http2() : _session(NULL)
{

}

TC_Http2::~TC_Http2()
{
	if(_session)
	{
		nghttp2_session_del(_session);
		_session = NULL;
	}
}

int TC_Http2::settings(unsigned int maxCurrentStreams)
{
	nghttp2_settings_entry iv[2] = {
		{NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, maxCurrentStreams},
		{NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE, 100 * 1024 * 1024},
	};

	/* 24 bytes magic string also will be sent*/
	nghttp2_submit_settings(_session,
	                        NGHTTP2_FLAG_NONE,
	                        iv,
	                        sizeof(iv)/sizeof(iv[0]));
	int rv = nghttp2_session_send(_session);
	return rv;
}

///////////////////////////////////////////////////////////////////////////////////
namespace server
{

static ssize_t str_read_callback(nghttp2_session *session, int32_t stream_id,
                                  uint8_t *buf, size_t length,
                                  uint32_t *data_flags,
                                  nghttp2_data_source *source,
                                  void *user_data) 
{
    TC_Http2Server::DataPack *dataPack = (TC_Http2Server::DataPack*)(source->ptr);
	if(dataPack->readPos == dataPack->dataBuf.size())
	{
		*data_flags |= NGHTTP2_DATA_FLAG_EOF;
		return 0;
	}
    size_t size = std::min(dataPack->dataBuf.size() - dataPack->readPos, length);

    memcpy(buf, dataPack->dataBuf.c_str() + dataPack->readPos, size);

    dataPack->readPos += size;
	
    if(dataPack->readPos == dataPack->dataBuf.size())
    {
        *data_flags |= NGHTTP2_DATA_FLAG_EOF;
    }
  
    return size;
}

static ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
                             size_t length, int flags, void *user_data) 
{
    TC_Http2Server *ptr = (TC_Http2Server*)user_data;
    ptr->appendResponseBuf((const char*)data, length);

    return (ssize_t)length;
}

static int on_header_callback(nghttp2_session *session,
                              const nghttp2_frame *frame, const uint8_t *name,
                              size_t namelen, const uint8_t *value,
                              size_t valuelen, uint8_t flags, void *user_data) 
{
    TC_Http2Server *ptr = (TC_Http2Server*)user_data;
    ptr->onHeaderCallback(frame->hd.stream_id, string((char*)name, namelen), string((char*)value, valuelen));
    return 0;
}

static int on_begin_headers_callback(nghttp2_session *session,
                                     const nghttp2_frame *frame,
                                     void *user_data) 
{
    TC_Http2Server *ptr = (TC_Http2Server*)user_data;

    if (frame->hd.type != NGHTTP2_HEADERS || frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
        return 0;
    }

    ptr->onHeaderCallback(frame->hd.stream_id);
    return 0;
}

static int on_frame_recv_callback(nghttp2_session *session, const nghttp2_frame *frame, void *user_data) 
{
    TC_Http2Server *ptr = (TC_Http2Server*)user_data;

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
    default:
        break;
    }
    return 0;
}

static int on_data_chunk_recv_callback(nghttp2_session *session, uint8_t flags,
                                       int32_t stream_id, const uint8_t *data,
                                       size_t len, void *user_data) 
{
    TC_Http2Server *ptr = (TC_Http2Server*)user_data;

    ptr->onDataChunkRecvCallback(stream_id, (const char*)data, len);

    return 0;
}

static int on_stream_close_callback(nghttp2_session *session, int32_t stream_id, uint32_t error_code, void *user_data) 
{
    TC_Http2Server *ptr = (TC_Http2Server*)user_data;
    ptr->onStreamCloseCallback(stream_id);

    return 0;
}

}

void TC_Http2Server::appendResponseBuf(const char *buff, size_t length)
{
    TC_LockT<TC_SpinLock> lock(_responseBufLock);
    _responseBuf.insert(_responseBuf.end(), buff, buff + length);
    
}

void TC_Http2Server::onHeaderCallback(int streamId)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    _mReq[streamId].streamId = streamId;
}

void TC_Http2Server::onHeaderCallback(int32_t streamId, const string &skey, const string &svalue)
{
    TC_LockT<TC_SpinLock>lock(reqLock_);
    auto it = _mReq.find(streamId);
    if (it != _mReq.end()) 
    {
        it->second.header.insert(std::pair<string, string>(skey, svalue));

        if (TC_Port::strcasecmp(skey.c_str(), ":method") == 0)
        {
            // string sMethod = TC_Common::upper(TC_Common::trim(svalue));
            if (TC_Port::strcasecmp(svalue.c_str(), "GET") == 0)
                it->second.method = REQUEST_GET;
            else if (TC_Port::strcasecmp(svalue.c_str(),  "POST") == 0)
                it->second.method = REQUEST_POST;
            else if (TC_Port::strcasecmp(svalue.c_str(), "OPTIONS") == 0)
                it->second.method = REQUEST_OPTIONS;
            else if (TC_Port::strcasecmp(svalue.c_str(), "HEAD") == 0) 
                it->second.method = REQUEST_HEAD;
            else if (TC_Port::strcasecmp(svalue.c_str(), "PUT") == 0)
                it->second.method = REQUEST_PUT;
            else if (TC_Port::strcasecmp(svalue.c_str(), "DELETE") == 0) 
                it->second.method = REQUEST_DELETE;
        }
        else if (TC_Port::strcasecmp(skey.c_str(), ":path") == 0)
        {
            it->second.uri = svalue;
        }
    }
}

void TC_Http2Server::onFrameRecvCallback(int32_t streamId) 
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    auto it = _mReq.find(streamId);
    if (it != _mReq.end()) 
    {
        it->second.bFinish = true;

        if(it->second.header.find(":method") != it->second.header.end() ||
            it->second.header.find(":path") != it->second.header.end() ||
            it->second.header.find(":scheme") != it->second.header.end())
        {
            char *tmpptr = (char*)&(it->second);
            _reqout.insert(_reqout.end(), (char*)&tmpptr, (char*)&tmpptr + sizeof(TC_Http2Server::RequestPack *));
        }
    }
}

void TC_Http2Server::onDataChunkRecvCallback(int32_t streamId, const char *data, size_t len) 
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    auto it = _mReq.find(streamId);
    if (it != _mReq.end()) 
    {
        it->second.body.append(data, len);
    }
}

void TC_Http2Server::onStreamCloseCallback(int32_t streamId) 
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    auto it = _mReq.find(streamId);
    if (it != _mReq.end()) 
    {
        if (it->second.bFinish != true)
        {
            _mReq.erase(streamId);
        }
    }
}


TC_Http2Server::TC_Http2Server(): _session(NULL), _bNewCon(true)
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

    // *(int32_t*)((char*)_session + 2380) = 100000000;

    //TLOGDEBUG("window size:" << nghttp2_session_get_remote_window_size(_session) << endl);

    nghttp2_session_callbacks_del(callbacks);
}

TC_Http2Server::~TC_Http2Server()
{
    nghttp2_session_del(_session);
}

TC_NetWorkBuffer::PACKET_TYPE TC_Http2Server::parse(TC_NetWorkBuffer&in, vector<char> &out)
{
    if(_bNewCon)
    {
        _bNewCon = false;

        nghttp2_settings_entry iv[2] = {{NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 3000},
                                        {NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE, 100*1024*1024}};
        nghttp2_submit_settings(_session, NGHTTP2_FLAG_NONE, iv, sizeof(iv)/sizeof(nghttp2_settings_entry));

	    nghttp2_session_send(_session);
    }

    in.mergeBuffers();

    auto buff = in.getBufferPointer();

	int readlen;

	{
		TC_LockT<TC_SpinLock> lock2(_nghttpLock);

		readlen = nghttp2_session_mem_recv(_session, (uint8_t *) buff.first, buff.second);
	}

    if(readlen < 0)
    {
        return TC_NetWorkBuffer::PACKET_ERR;
    }
    else
    {
        in.moveHeader(readlen);

	    TC_LockT<TC_SpinLock> lock1(reqLock_);

	    if (_reqout.empty())
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }

        out.swap(_reqout);
        _reqout.clear();
    }

    return TC_NetWorkBuffer::PACKET_FULL;
}

int TC_Http2Server::doRequest(const vector<char> &request, vector<int32_t>& vtReqid)
{
    vtReqid.clear();

    for (unsigned int i = 0; i < request.size(); i += sizeof(TC_Http2Server::RequestPack *)) 
    {
        RequestPack *ptr;
        memcpy(&ptr, (char*)&(request[i]), sizeof(TC_Http2Server::RequestPack *));

        vtReqid.push_back(ptr->streamId);
    }

    return 0;
}

int TC_Http2Server::doResponse(int32_t reqid, const Http2Response &response, vector<char>& out)
{
    {
        TC_LockT<TC_SpinLock> lock(reqLock_);
        auto it = _mReq.find(reqid);
        if (it == _mReq.end())
            return -1;
    }
    string sstatus = TC_Common::tostr(response.status);

    const char* strstatus = ":status";
    nghttp2_nv *hdrs = new nghttp2_nv[response.header.size() + 1];
    hdrs[0].flags = NGHTTP2_NV_FLAG_NONE;
    hdrs[0].name = (uint8_t*)strstatus;
    hdrs[0].namelen = 7;
    hdrs[0].value = (uint8_t*)sstatus.c_str();
    hdrs[0].valuelen = sstatus.size();
    TC_Http::http_header_type::const_iterator it = response.header.begin();
    for (int n = 1; it != response.header.end(); n++, it++)
    {
        hdrs[n].flags = NGHTTP2_NV_FLAG_NONE;
        hdrs[n].name = (uint8_t*)it->first.c_str();
        hdrs[n].namelen = it->first.size();
        hdrs[n].value = (uint8_t*)it->second.c_str();
        hdrs[n].valuelen = it->second.size();
    }

    DataPack dataPack;
    dataPack.readPos = 0;
    dataPack.dataBuf = response.body;

    nghttp2_data_provider data_prd;
    data_prd.source.ptr = (void*)&dataPack;
    data_prd.read_callback = server::str_read_callback;
    int ret ;
    {
        TC_LockT<TC_SpinLock> lock(_nghttpLock);

        ret = nghttp2_submit_response(_session, reqid, hdrs, response.header.size()+1, &data_prd);
        if (ret != 0) {
        	cout << "nghttp2_submit_response error" << endl;
	        return -1;
        }

        while (nghttp2_session_want_write(_session)) {
            ret = nghttp2_session_send(_session);
            if (ret != 0) {
	            cout << "nghttp2_session_send error" << endl;
	            return -1;
            }
        }
    }

    delete [] hdrs;

    {
        TC_LockT<TC_SpinLock> lock(_responseBufLock);
        out.swap(_responseBuf);
        _responseBuf.clear();
    }

    {
        TC_LockT<TC_SpinLock> lock(reqLock_);
        _mReq.erase(reqid);
    }

    return 0;
}

int TC_Http2Server::doRequest(const vector<char> &request, TC_Http2Server::RequestFunc requestFunc, vector<char>& response)
{
    for (unsigned int i = 0; i < request.size(); i += sizeof(TC_Http2Server::RequestPack *)) 
    {
        Http2Response rsp;

        RequestPack *ptr;
        memcpy(&ptr, (char*)&(request[i]), sizeof(TC_Http2Server::RequestPack *));

        Req_Type qstatus;

        const char* sMethod = ptr->header.find(":method")->second.c_str(); //TC_Common::upper(TC_Common::trim(ptr->header.find(":method")->second));
        if (TC_Port::strcasecmp(sMethod, "GET") == 0)
            qstatus = REQUEST_GET;
        else if (TC_Port::strcasecmp(sMethod, "POST") == 0)
            qstatus = REQUEST_POST;
        else if (TC_Port::strcasecmp(sMethod, "OPTIONS") == 0) 
            qstatus = REQUEST_OPTIONS;
        else if (TC_Port::strcasecmp(sMethod, "HEAD") == 0)
            qstatus = REQUEST_HEAD;
        else if (TC_Port::strcasecmp(sMethod, "PUT") == 0)
            qstatus = REQUEST_PUT;
        else if (TC_Port::strcasecmp(sMethod, "DELETE") == 0)
            qstatus = REQUEST_DELETE;
        else
        {
            continue;
        }
        string sstatus = ptr->header.find(":path")->second;

        requestFunc(qstatus, sstatus, ptr->header, ptr->body, rsp); 

        DataPack dataPack;
        dataPack.readPos = 0;        
        dataPack.dataBuf = rsp.body;

        sstatus = TC_Common::tostr(rsp.status);

        const char* strstatus = ":status";
        nghttp2_nv *hdrs = new nghttp2_nv[rsp.header.size() + 1];
        hdrs[0].flags = NGHTTP2_NV_FLAG_NONE;
        hdrs[0].name = (uint8_t*)strstatus;
        hdrs[0].namelen = 7;
        hdrs[0].value = (uint8_t*)sstatus.c_str();
        hdrs[0].valuelen = sstatus.size();
        TC_Http::http_header_type::iterator it = rsp.header.begin();
        for (int n = 1; it != rsp.header.end(); n++, it++)
        {
            hdrs[n].flags = NGHTTP2_NV_FLAG_NONE;
            hdrs[n].name = (uint8_t*)it->first.c_str();
            hdrs[n].namelen = it->first.size();
            hdrs[n].value = (uint8_t*)it->second.c_str();
            hdrs[n].valuelen = it->second.size();
        }

        nghttp2_data_provider data_prd;
        data_prd.source.ptr = (void*)&dataPack;
        data_prd.read_callback = server::str_read_callback;
        
        {
            TC_LockT<TC_SpinLock> lock(_nghttpLock);

            int ret = nghttp2_submit_response(_session, ptr->streamId, hdrs, rsp.header.size()+1, &data_prd);
            if (ret != 0)
            {
	            cout << "nghttp2_submit_response error:" << nghttp2_strerror(ret) << endl;
            }
                ;//TLOGERROR("Fatal error: %s", nghttp2_strerror(ret));

            while (nghttp2_session_want_write(_session)) {
                ret = nghttp2_session_send(_session);
                if (ret != 0)
                {
	                cout << "nghttp2_submit_response error:" << nghttp2_strerror(ret) << endl;
                }
                    ;//TLOGERROR("Fatal error: %s", nghttp2_strerror(ret));
            }
        }

        {
            TC_LockT<TC_SpinLock> lock(_responseBufLock);
            response.insert(response.begin(), _responseBuf.begin(), _responseBuf.end());
	        _responseBuf.clear();
        }

        delete [] hdrs;
        {
            TC_LockT<TC_SpinLock> lock(reqLock_);
            _mReq.erase(ptr->streamId); 
        }

    }

    return 0;
}

int TC_Http2Server::getMethod(int32_t reqid, Req_Type &method)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    auto it = _mReq.find(reqid);
    if (it != _mReq.end()) 
        method = it->second.method;
    else
        return -1;

    return 0;
}

int TC_Http2Server::getUri(int32_t reqid, string &uri)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    auto it = _mReq.find(reqid);
    if (it != _mReq.end()) 
        uri = it->second.uri;
    else
        return -1;

    return 0;
}

int TC_Http2Server::getHeader(int32_t reqid, TC_Http::http_header_type &header)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    auto it = _mReq.find(reqid);
    if (it != _mReq.end()) 
        header = it->second.header;
    else
        return -1;

    return 0;
}

int TC_Http2Server::getBody(int32_t reqid, string &body)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    auto it = _mReq.find(reqid);
    if (it != _mReq.end()) 
        body = it->second.body;
    else
        return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void TC_Http2Client::Http2Response::swap(Http2Response& other)
{
    if (this == &other)
        return;

    std::swap(streamId, other.streamId);
    headers.swap(other.headers);
    body.swap(other.body);
    std::swap(state, other.state);
}

///////////////////////////////////////////////////////////////////////////////////////

namespace client
{
static ssize_t send_callback(nghttp2_session* session, const uint8_t* data, size_t length, int flags, void* user_data)
{
    TC_Http2Client* nghttp2 = (TC_Http2Client* )user_data;
    nghttp2->buffer().insert(nghttp2->buffer().end(), (const char*)data, (const char*)data + length);

    return length;
}

static int on_begin_headers_callback(nghttp2_session* session, const nghttp2_frame* frame, void* user_data)
{
    TC_Http2Client* nghttp2 = (TC_Http2Client* )user_data;

    if (frame->hd.type == NGHTTP2_HEADERS)
    {
        if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE)
        {
            TC_Http2Client::Http2Response rsp;
            rsp.streamId = frame->hd.stream_id;
            rsp.state = TC_Http2Client::ResponseNone;
            nghttp2->responses()[rsp.streamId] = rsp;
        }
    }

    return 0;
}

static int on_header_callback(nghttp2_session* session, const nghttp2_frame* frame,
                         const uint8_t* name, size_t namelen,
                         const uint8_t* value, size_t valuelen,
                         uint8_t flags, void* user_data)
{
    TC_Http2Client* nghttp2 = (TC_Http2Client* )user_data;

    int streamId = frame->hd.stream_id;
    auto it = nghttp2->responses().find(streamId);
    if (it == nghttp2->responses().end())
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

    std::string n((const char*)name, namelen);
    std::string v((const char*)value, valuelen);

    it->second.headers.insert(std::make_pair(n, v));

    return 0;
}

static int on_frame_recv_callback(nghttp2_session* session, const nghttp2_frame* frame, void* user_data)
{
    TC_Http2Client* nghttp2 = (TC_Http2Client* )user_data;

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
                it->second.state = TC_Http2Client::ResponseHeadersDone;
            }
            return 0;

        default:
            break;
    }

    return 0;
}

static int on_data_chunk_recv_callback(nghttp2_session* session, uint8_t flags, int32_t stream_id, const uint8_t* data, size_t len, void* user_data)
{
    TC_Http2Client* nghttp2 = (TC_Http2Client* )user_data;

    auto it = nghttp2->responses().find(stream_id);
    if (it == nghttp2->responses().end())
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

    it->second.body.insert(it->second.body.end(), (const char* )data, (const char* )data + len);
    return 0;
}

static int on_stream_close_callback(nghttp2_session* session, int32_t stream_id, uint32_t error_code, void* user_data)
{
    TC_Http2Client* nghttp2 = (TC_Http2Client* )user_data;

    auto it = nghttp2->responses().find(stream_id);
    if (it == nghttp2->responses().end())
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

    it->second.state = TC_Http2Client::ResponseBodyDone;

    nghttp2->doneResponses()[stream_id].swap(it->second);
    nghttp2->responses().erase(it);

    return 0;
}

}

TC_Http2Client::TC_Http2Client()
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

TC_Http2Client::~TC_Http2Client()
{
//    nghttp2_session_del(_session);
}

// void TC_Http2Client::onNegotiateDone(bool succ)
// {
//     // assert (_state == Negotiating);
//     _state = succ ? Http2: Http1;
//     if (succ) 
//     {
//         int rv = nghttp2_session_upgrade(_session,
//                                          (const uint8_t*)_settings.data(),
//                                          _settings.size(),
//                                          NULL);
//         if (rv)
//             cerr << "nghttp2_session_upgrade error: " << nghttp2_strerror(rv) << endl;
//     }
// }

// string& TC_Http2Client::_buffer()
// {
//     return _sendBuf;
// }

// nghttp2_session* TC_Http2Client::session() const
// {
//     return _session;
// }

} // end namespace tars

#endif // end #if TARS_HTTP2

