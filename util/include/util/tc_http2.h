#ifndef __TC_HTTP2_H__
#define __TC_HTTP2_H__

#include "util/tc_http.h"
#include "util/tc_spin_lock.h"
#include "util/tc_network_buffer.h"

typedef struct nghttp2_session nghttp2_session;

namespace tars
{

class TC_Http2
{
public:
	TC_Http2();

	virtual ~TC_Http2();

	/**
	 * @brief setting
	 */
	int settings(unsigned int maxCurrentStreams = 2000);

	/**
	 * @brief  buffer
	 */
	vector<char>& buffer() { return _buff; }

	/**
	 * buffer
	 * @return
	 */
	const vector<char>& buffer() const { return _buff; }

	/**
	 * swap buff
	 * @param buff
	 */
	void swap(vector<char> &buff) { _buff.swap(buff); }

	/**
	 * @brief  session
	 */
	nghttp2_session* session() const { return _session; }

protected:
	/**
	 * error code
	 */
	int                 _err = 0;

	/**
	 * session
	 */
	nghttp2_session*    _session;

	/**
	 * data buff
	 */
	vector<char>        _buff;

};
class TC_Http2Server
{
public:

    enum Req_Type
    {
        REQUEST_GET,
        REQUEST_POST,
        REQUEST_OPTIONS,
        REQUEST_HEAD,
        REQUEST_PUT,
        REQUEST_DELETE
    } ;

    struct Http2Response
    {
        int status;
        string about;
        TC_Http::http_header_type header;
        string body;
    };

    typedef std::function<void(const Req_Type reqtype, const string &requri, const TC_Http::http_header_type &reqHeader, const string &reqBody, Http2Response &rsp)> RequestFunc; 

    TC_Http2Server();

    ~TC_Http2Server();

    /**
     * get all http2 request id
     * @param in
     * @param out
     * @return
     */
    static int doRequest(const vector<char> &request, vector<int32_t>& vtReqid);

    /**
     * http2
     * @param in
     * @param out
     * @return
     */
    TC_NetWorkBuffer::PACKET_TYPE parse(TC_NetWorkBuffer&in, vector<char> &out);

    int doResponse(int32_t reqid, const Http2Response &response, vector<char>& out);

    int doRequest(const vector<char> &request, RequestFunc func, vector<char>& response);

    int getMethod(int32_t reqid, Req_Type &method);

    int getUri(int32_t reqid, string &uri);

    int getHeader(int32_t reqid, TC_Http::http_header_type &header);

    int getBody(int32_t reqid, string &body);

    void appendResponseBuf(const char *buff, size_t length);
    void onHeaderCallback(int32_t streamId);
    void onHeaderCallback(int32_t streamId, const string &skey, const string &svalue);
    void onFrameRecvCallback(int32_t streamId);
    void onDataChunkRecvCallback(int32_t streamId, const char *data, size_t len);
    void onStreamCloseCallback(int32_t streamId);

    struct DataPack
    {
        DataPack(){}

        DataPack(const string &data, int pos):dataBuf(data), readPos(pos){}

        string        dataBuf;
        unsigned int  readPos;
    };

protected:
    struct RequestPack
    {
        RequestPack():streamId(0), bFinish(false){}

        Req_Type method;
        string uri;
        TC_Http::http_header_type header;
        string body;
        int32_t streamId;
        bool bFinish;
    };

    TC_SpinLock _responseBufLock;
    vector<char> _responseBuf;

    TC_SpinLock reqLock_;
    unordered_map<int32_t, RequestPack> _mReq;

    vector<char> _reqout;

    nghttp2_session *_session;

    bool _bNewCon;

    TC_SpinLock _nghttpLock;
};

/////////////////////////////////////////////////////////////////////////////////

class TC_Http2Client : public TC_Http2
{
public:

    enum ResponseState
    {
        ResponseNone,
        ResponseHeadersDone,
        ResponseBodyDone,
    };

    struct Http2Response
    {
        int streamId;
        std::map<std::string, std::string> headers;
        vector<char> body;
        ResponseState state;

        void swap(Http2Response& other);
    };

    TC_Http2Client();

    ~TC_Http2Client();
public:

//    /**
//     * @brief HTTP2握手+setting
//     */
//    int settings(unsigned int maxCurrentStreams = 2000);
//    /**
//     * @brief  当前缓冲区
//     */
//    vector<char>& sendBuffer() { return _sendBuf; }
//
//    /**
//     * @brief  session
//     */
//    nghttp2_session* session() const { return _session; }

    /** 
     * @brief response
     */
    std::unordered_map<int, Http2Response> &responses() { return _responses; }

    /** 
     * @brief response finished
     */
    std::unordered_map<int, Http2Response> &doneResponses() { return _doneResponses; }

private:
//    /**
//     * session
//     */
//    nghttp2_session* _session;
//
//    /**
//     * 发送缓存区，由send callback填充
//     */
//    vector<char> _sendBuf;

    /**
     * 收到的响应
     */
    std::unordered_map<int, Http2Response> _responses;

    /**
     * 收到的完整响应
     */
    std::unordered_map<int, Http2Response> _doneResponses;
};

}

//#endif

#endif
