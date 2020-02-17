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
	/**
	 * constructor
	 */
	TC_Http2();

	/**
	 * deconstructor
	 */
	virtual ~TC_Http2();

	struct DataPack
	{
		DataPack(const char *data, size_t length) : _dataBuf(data), _length(length) {}

		const char* _dataBuf;
		size_t      _length;
		size_t      _readPos = 0;
	};

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
	 * insert buff
	 * @param buff
	 */
	void insertBuff(const char *buff, size_t length) { _buff.insert(_buff.end(), buff, buff + length); }

	/**
	 * @brief  session
	 */
	nghttp2_session* session() const { return _session; }

	/**
	 *
	 * @return
	 */
	const char *getErrMsg();

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


class TC_Http2Server : public TC_Http2
{
public:

    TC_Http2Server();

    ~TC_Http2Server();

    struct Http2Context
    {
//	    Http2Context(int32_t id) : reqId(id) {}

	    int32_t         reqId;
	    bool            bFinish = false;
	    TC_HttpRequest  request;
	    TC_HttpResponse response;
    };

	/**
	 * parse all request
	 * @param request
	 * @param unordered_map<int32_t, std::shared_ptr<TC_HttpRequest>>
	 * @return
	 */
	void decodeRequest(vector<Http2Context> &contexts);

	/**
	 *
	 * @param reqid
	 * @param response
	 * @param out
	 * @return
	 */
	int encodeResponse(const Http2Context &context, vector<char> &out);

    /**
     * http2
     * @param in
     * @param out
     * @return
     */
    TC_NetWorkBuffer::PACKET_TYPE parse(TC_NetWorkBuffer&in, vector<char> &out);


    void onHeaderCallback(int32_t streamId);
    void onHeaderCallback(int32_t streamId, const string &skey, const string &svalue);
    void onFrameRecvCallback(int32_t streamId);
    void onDataChunkRecvCallback(int32_t streamId, const char *data, size_t len);
    void onStreamCloseCallback(int32_t streamId);

protected:

	Http2Context &getContext(int32_t streamId);
	void deleteContext(int32_t streamId);

protected:

//	TC_SpinLock _contextLock;
//	TC_ThreadMutex _contextLock;

//    TC_SpinLock reqLock_;
//    unordered_map<int32_t, RequestPack> _mReq;

//	unordered_map<int32_t, RequestPack> _mReq;
//	unordered_map<int32_t, std::shared_ptr<Http2Context>>  _context;
	unordered_map<int32_t, Http2Context>  _context;

	vector<Http2Context>  _contextFinished;

    vector<char> _reqout;

    bool _bNewCon;

	TC_ThreadMutex _nghttpLock;
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
