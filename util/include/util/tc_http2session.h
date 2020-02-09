#ifndef __TC_HTTP2_H__
#define __TC_HTTP2_H__

#if TARS_HTTP2

#include "util/tc_thread.h"
#include "util/tc_autoptr.h"
#include "util/tc_http.h"
#include "util/tc_network_buffer.h"
#include "util/tc_spin_lock.h"
#include "nghttp2/nghttp2.h"

namespace tars
{

typedef enum
{
    REQUEST_GET,
    REQUEST_POST,
    REQUEST_OPTIONS,
    REQUEST_HEAD,
    REQUEST_PUT,
    REQUEST_DELETE
}Req_Type;

typedef int (*ResponseFunc)(const Req_Type reqtype, 
                         const string &requri, 
                         const TC_Http::http_header_type &reqHeader, 
                         const string &reqBody,
                         int &resopnseStatus,
                         string &resopnseAbout,
                         TC_Http::http_header_type &responseHeader,
                         string &responseBody);


class TC_Http2Session: public TC_HandleBase
{
public:

    TC_Http2Session();

    ~TC_Http2Session();

    struct Http2Response
    {
        int status;
        string about;
        TC_Http::http_header_type header;
        string body;
    };

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

    int doRequest(const vector<char> &request, vector<char>& response);

    void setResponseFunc(ResponseFunc func) { responseFunc_ = func; }

    int getMethod(int32_t reqid, Req_Type &method);

    int getUri(int32_t reqid, string &uri);

    int getHeader(int32_t reqid, TC_Http::http_header_type &header);

    int getBody(int32_t reqid, string &body);

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

    struct DataPack
    {
        DataPack(){}

        DataPack(string &data, int pos):dataBuf(data), readPos(pos){}

        string dataBuf;
        unsigned int readPos;
    };


    TC_SpinLock responseBufLock_;
    string responseBuf_;

    TC_SpinLock reqLock_;
    map<int32_t, RequestPack> mReq_;

    vector<char> reqout_;

protected:


private:

    int (*responseFunc_)(const Req_Type reqtype, 
                         const string &requri, 
                         const TC_Http::http_header_type &reqHeader, 
                         const string &reqBody,
                         int &resopnseStatus,
                         string &resopnseAbout,
                         TC_Http::http_header_type &responseHeader,
                         string &responseBody);

    nghttp2_session *session_;

    bool bNewCon_;

    TC_SpinLock nghttpLock;

    // bool bOldVersion_;
    // bool bUpgrade_;
};

typedef TC_AutoPtr<TC_Http2Session> TC_Http2SessionPtr;

}

#endif

#endif
