#if TARS_HTTP2

#include "util/tc_http.h"
#include "util/tc_http2session.h"
#include "util/tc_epoll_server.h"

namespace tars
{

#define MAKE_NV(NAME, VALUE)                                                   \
{                                                                            \
(uint8_t *)NAME, (uint8_t *)VALUE, sizeof(NAME) - 1, sizeof(VALUE) - 1,    \
    NGHTTP2_NV_FLAG_NONE                                                   \
}

#define ARRLEN(x) (sizeof(x) / sizeof(x[0]))

#define MIN(x, y) ((x < y)?x:y)

static ssize_t str_read_callback(nghttp2_session *session, int32_t stream_id,
                                  uint8_t *buf, size_t length,
                                  uint32_t *data_flags,
                                  nghttp2_data_source *source,
                                  void *user_data) 
{
    TC_Http2Session::DataPack *dataPack = (TC_Http2Session::DataPack*)(source->ptr);
    size_t size = MIN(dataPack->dataBuf.size() - dataPack->readPos, length);

    memcpy(buf, dataPack->dataBuf.c_str() + dataPack->readPos, size);

    dataPack->readPos += size;
	
    if(dataPack->readPos == dataPack->dataBuf.size())
    {
        //TLOGDEBUG("[str_read_callback] finish:" << size << endl);
        *data_flags |= NGHTTP2_DATA_FLAG_EOF;
    }

    //TLOGDEBUG("[str_read_callback] size:" << size  << " length:" << length<< endl);
  
  return size;
}

static ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
                             size_t length, int flags, void *user_data) 
{
    TC_Http2Session *ptr = (TC_Http2Session*)user_data;
    {
        TC_LockT<TC_SpinLock> lock(ptr->responseBufLock_);
        ptr->responseBuf_.append((char*)data, length);
    }
    //TLOGDEBUG("[send_callback] length:" << length << endl);
    return (ssize_t)length;
}

static int on_header_callback(nghttp2_session *session,
                              const nghttp2_frame *frame, const uint8_t *name,
                              size_t namelen, const uint8_t *value,
                              size_t valuelen, uint8_t flags, void *user_data) 
{
    //TLOGDEBUG("[on_header_callback] streamid:" << frame->hd.stream_id << " name:" << name << " value:" << value << " flags:" << flags << endl);

    TC_Http2Session *ptr = (TC_Http2Session*)user_data;
    {
        TC_LockT<TC_SpinLock>lock(ptr->reqLock_);
        map<int32_t, TC_Http2Session::RequestPack>::iterator it = ptr->mReq_.find(frame->hd.stream_id);
        if (it != ptr->mReq_.end()) 
        {
            string skey((char*)name, namelen);
            string svalue((char*)value, valuelen);
            it->second.header.insert(std::pair<string, string>(skey, svalue));

            skey = TC_Common::lower(TC_Common::trim(skey));
            if (skey == ":method") 
            {
                string sMethod = TC_Common::upper(TC_Common::trim(svalue));
                if (sMethod == "GET") 
                    it->second.method = REQUEST_GET;
                else if (sMethod == "POST") 
                    it->second.method = REQUEST_POST;
                else if (sMethod == "OPTIONS") 
                    it->second.method = REQUEST_OPTIONS;
                else if (sMethod == "HEAD") 
                    it->second.method = REQUEST_HEAD;
                else if (sMethod == "PUT") 
                    it->second.method = REQUEST_PUT;
                else if (sMethod == "DELETE") 
                    it->second.method = REQUEST_DELETE;
            }
            else if (skey == ":path") 
            {
                it->second.uri = svalue;
            }
        }
    }
    return 0;
}

static int on_begin_headers_callback(nghttp2_session *session,
                                     const nghttp2_frame *frame,
                                     void *user_data) 
{
    //TLOGDEBUG("[on_begin_headers_callback] streamid:" << frame->hd.stream_id << endl);
    TC_Http2Session *ptr = (TC_Http2Session*)user_data;

    if (frame->hd.type != NGHTTP2_HEADERS ||
            frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
        return 0;
    }

    {
        TC_LockT<TC_SpinLock> lock(ptr->reqLock_);
        ptr->mReq_[frame->hd.stream_id].streamId = frame->hd.stream_id;
    }

    return 0;
}

static int on_frame_recv_callback(nghttp2_session *session, const nghttp2_frame *frame, void *user_data) 
{
    //TLOGDEBUG("[on_frame_recv_callback] id:" << frame->hd.stream_id << " type:" << int(frame->hd.type) << endl);
    cout << "[on_frame_recv_callback] id:" << frame->hd.stream_id << " type:" << int(frame->hd.type) << endl;

    TC_Http2Session *ptr = (TC_Http2Session*)user_data;

    switch (frame->hd.type) 
    {
    case NGHTTP2_DATA:
    case NGHTTP2_HEADERS:
        /* Check that the client request has finished */
        if (frame->hd.flags & NGHTTP2_FLAG_END_STREAM) 
        {
            //TLOGDEBUG("[on_frame_recv_callback] NGHTTP2_FLAG_END_STREAM" << endl);

            {
                TC_LockT<TC_SpinLock> lock(ptr->reqLock_);
                map<int32_t, TC_Http2Session::RequestPack>::iterator it = ptr->mReq_.find(frame->hd.stream_id);
                if (it != ptr->mReq_.end()) 
                {
                    it->second.bFinish = true;

                    if(it->second.header.find(":method") != it->second.header.end() ||
                       it->second.header.find(":path") != it->second.header.end() ||
                       it->second.header.find(":scheme") != it->second.header.end())
                    {
                        cout << "insert reqout_" << endl;
                        char *tmpptr = (char*)&(it->second);
                        ptr->reqout_.insert(ptr->reqout_.end(), (char*)&tmpptr, (char*)&tmpptr + sizeof(TC_Http2Session::RequestPack *));
                    }
                }
            }
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
    //TLOGDEBUG("[on_data_chunk_recv_callback] stream_id:" << stream_id << endl);
    TC_Http2Session *ptr = (TC_Http2Session*)user_data;
    {
        TC_LockT<TC_SpinLock> lock(ptr->reqLock_);
        map<int32_t, TC_Http2Session::RequestPack>::iterator it = ptr->mReq_.find(stream_id);
        if (it != ptr->mReq_.end()) 
        {
            it->second.body.append((char*)data, len);
        }
    }
    return 0;
}

static int on_stream_close_callback(nghttp2_session *session, int32_t stream_id, uint32_t error_code, void *user_data) 
{
    //TLOGDEBUG("[on_stream_close_callback] streamid:" << stream_id << endl);

    TC_Http2Session *ptr = (TC_Http2Session*)user_data;

    {
        TC_LockT<TC_SpinLock> lock(ptr->reqLock_);
        map<int32_t, TC_Http2Session::RequestPack>::iterator it = ptr->mReq_.find(stream_id);
        if (it != ptr->mReq_.end()) 
        {
            if (it->second.bFinish != true)
            {
                ptr->mReq_.erase(stream_id);
            }
        }
    }
    return 0;
}

TC_Http2Session::TC_Http2Session():session_(NULL), bNewCon_(true)
{
    nghttp2_session_callbacks *callbacks;

    nghttp2_session_callbacks_new(&callbacks);

    nghttp2_session_callbacks_set_send_callback(callbacks, send_callback);

    nghttp2_session_callbacks_set_on_frame_recv_callback(callbacks, on_frame_recv_callback);

    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(callbacks, on_data_chunk_recv_callback);

    nghttp2_session_callbacks_set_on_stream_close_callback(callbacks, on_stream_close_callback);

    nghttp2_session_callbacks_set_on_header_callback(callbacks, on_header_callback);

    nghttp2_session_callbacks_set_on_begin_headers_callback(callbacks, on_begin_headers_callback);

    nghttp2_session_server_new(&session_, callbacks, ((void*)this));

    *(int32_t*)((char*)session_ + 2380) = 100000000;

    //TLOGDEBUG("window size:" << nghttp2_session_get_remote_window_size(session_) << endl);

    nghttp2_session_callbacks_del(callbacks);
}

TC_Http2Session::~TC_Http2Session()
{
    nghttp2_session_del(session_);
}

TC_NetWorkBuffer::PACKET_TYPE TC_Http2Session::parse(TC_NetWorkBuffer&in, vector<char> &out)
{
    if(bNewCon_)
    {
        bNewCon_ = false;

        nghttp2_settings_entry iv[2] = {{NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 100},
                                        {NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE, 100*1024*1024}};
        nghttp2_submit_settings(session_, NGHTTP2_FLAG_NONE, iv,  ARRLEN(iv));
    }

    vector<char> buff = in.getBuffers();

    int readlen = nghttp2_session_mem_recv(session_, (uint8_t *)buff.data(), buff.size()); 

    cout << "parse:" << readlen << ", reqout_ size: " << reqout_.size() << endl;

    if(readlen < 0)
    {
        return TC_NetWorkBuffer::PACKET_ERR;
    }
    else
    {
        in.moveHeader(readlen);

        if (reqout_.empty())
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }
        
        out.insert(out.end(), reqout_.begin(), reqout_.end());
        reqout_.clear();
    }

    return TC_NetWorkBuffer::PACKET_FULL;
}

int TC_Http2Session::doRequest(const vector<char> &request, vector<int32_t>& vtReqid)
{
    vtReqid.clear();

    string responseAbout;
    TC_Http::http_header_type responseHeader;
    string sstatus;
    for (unsigned int i = 0; i < request.size(); i += sizeof(TC_Http2Session::RequestPack *)) 
    {
        RequestPack *ptr;
        memcpy(&ptr, (char*)&(request[i]), sizeof(TC_Http2Session::RequestPack *));

        vtReqid.push_back(ptr->streamId);
    }

    return 0;
}

int TC_Http2Session::doResponse(int32_t reqid, const Http2Response &response, vector<char>& out)
{
    {
        TC_LockT<TC_SpinLock> lock(reqLock_);
        map<int32_t, RequestPack>::iterator it = mReq_.find(reqid);
        if (it == mReq_.end())
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
    data_prd.read_callback = str_read_callback;
    int ret ;
    {
        TC_LockT<TC_SpinLock> lock(nghttpLock);

        ret = nghttp2_submit_response(session_, reqid, hdrs, response.header.size()+1, &data_prd);
        if (ret != 0) 
            return -1;

        while (nghttp2_session_want_write(session_)) {
            ret = nghttp2_session_send(session_);
            if (ret != 0) 
                return -1;
        }
    }

    delete [] hdrs;

    {
        TC_LockT<TC_SpinLock> lock(responseBufLock_);
        out.clear();
        out.insert(out.end(), responseBuf_.begin(), responseBuf_.end());

        responseBuf_.clear();
    }

    {
        TC_LockT<TC_SpinLock> lock(reqLock_);
        mReq_.erase(reqid);
    }

    return 0;
}

int TC_Http2Session::doRequest(const vector<char> &request, vector<char>& response)
{
    int responseStatus = 0;
    string responseAbout;
    TC_Http::http_header_type responseHeader;
    string sstatus;
    for (unsigned int i = 0; i < request.size(); i += sizeof(TC_Http2Session::RequestPack *)) 
    {
        RequestPack *ptr;
        memcpy(&ptr, (char*)&(request[i]), sizeof(TC_Http2Session::RequestPack *));

        string sMethod = TC_Common::upper(TC_Common::trim(ptr->header.find(":method")->second));
        if (sMethod == "GET") 
            responseStatus = REQUEST_GET;
        else if (sMethod == "POST") 
            responseStatus = REQUEST_POST;
        else if (sMethod == "OPTIONS") 
            responseStatus = REQUEST_OPTIONS;
        else if (sMethod == "HEAD") 
            responseStatus = REQUEST_HEAD;
        else if (sMethod == "PUT") 
            responseStatus = REQUEST_PUT;
        else if (sMethod == "DELETE") 
            responseStatus = REQUEST_DELETE;
        else
        {
            continue;
        }
        sstatus = ptr->header.find(":path")->second;
        responseAbout.clear();
        responseHeader.clear();
        DataPack dataPack;
        dataPack.readPos = 0;
        responseFunc_((Req_Type)responseStatus, 
                      sstatus, 
                      ptr->header,
                      ptr->body, 
                      responseStatus,
                      responseAbout,
                      responseHeader, 
                      dataPack.dataBuf); 

        sstatus = TC_Common::tostr(responseStatus);

        const char* strstatus = ":status";
        nghttp2_nv *hdrs = new nghttp2_nv[responseHeader.size() + 1];
        hdrs[0].flags = NGHTTP2_NV_FLAG_NONE;
        hdrs[0].name = (uint8_t*)strstatus;
        hdrs[0].namelen = 7;
        hdrs[0].value = (uint8_t*)sstatus.c_str();
        hdrs[0].valuelen = sstatus.size();
        TC_Http::http_header_type::iterator it = responseHeader.begin();
        for (int n = 1; it != responseHeader.end(); n++, it++)
        {
            hdrs[n].flags = NGHTTP2_NV_FLAG_NONE;
            hdrs[n].name = (uint8_t*)it->first.c_str();
            hdrs[n].namelen = it->first.size();
            hdrs[n].value = (uint8_t*)it->second.c_str();
            hdrs[n].valuelen = it->second.size();
        }

        nghttp2_data_provider data_prd;
        data_prd.source.ptr = (void*)&dataPack;
        data_prd.read_callback = str_read_callback;
        int ret ;
        {
            TC_LockT<TC_SpinLock> lock(nghttpLock);

            ret = nghttp2_submit_response(session_, ptr->streamId, hdrs, responseHeader.size()+1, &data_prd);
            if (ret != 0) 
                ;//TLOGERROR("Fatal error: %s", nghttp2_strerror(ret));

            while (nghttp2_session_want_write(session_)) {
                ret = nghttp2_session_send(session_);
                if (ret != 0) 
                    ;//TLOGERROR("Fatal error: %s", nghttp2_strerror(ret));
            }
            response.clear();
            response.insert(response.end(), responseBuf_.begin(), responseBuf_.end());

            responseBuf_.clear();
        }

        delete [] hdrs;
        {
            TC_LockT<TC_SpinLock> lock(reqLock_);
            mReq_.erase(ptr->streamId); 
        }

    }

    return 0;
}

int TC_Http2Session::getMethod(int32_t reqid, Req_Type &method)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    map<int32_t, RequestPack>::iterator it = mReq_.find(reqid);
    if (it != mReq_.end()) 
        method = it->second.method;
    else
        return -1;

    return 0;
}

int TC_Http2Session::getUri(int32_t reqid, string &uri)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    map<int32_t, RequestPack>::iterator it = mReq_.find(reqid);
    if (it != mReq_.end()) 
        uri = it->second.uri;
    else
        return -1;

    return 0;
}

int TC_Http2Session::getHeader(int32_t reqid, TC_Http::http_header_type &header)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    map<int32_t, RequestPack>::iterator it = mReq_.find(reqid);
    if (it != mReq_.end()) 
        header = it->second.header;
    else
        return -1;

    return 0;
}

int TC_Http2Session::getBody(int32_t reqid, string &body)
{
    TC_LockT<TC_SpinLock> lock(reqLock_);
    map<int32_t, RequestPack>::iterator it = mReq_.find(reqid);
    if (it != mReq_.end()) 
        body = it->second.body;
    else
        return -1;

    return 0;
}
} 

#endif

