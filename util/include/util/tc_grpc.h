#ifndef __TC_GRPC_PROTOCOL_H__
#define __TC_GRPC_PROTOCOL_H__

#include <unordered_map>
#include "util/tc_network_buffer.h"
#include "util/tc_spin_lock.h"
#include "util/tc_http2.h"
#include "util/tc_epoll_server.h"

namespace tars
{
void addGrpcPrefix(string& body, bool compressed);
bool RemoveGrpcPrefix(string& body, bool* compressed);

class TC_GrpcServer : public TC_Http2Server
{
public:

	/**
	 * constructor
	 */
    TC_GrpcServer();

    /**
     * deconstructor
     */
    ~TC_GrpcServer();

    
	/**
	 *
	 * @param context
	 * @param out
	 * @return
	 */
	int encodeResponse(const shared_ptr<Http2Context> &context, std::string gStatus, vector<char> &out);

	/**
	 *
	 * @param context
	 * @param status
	 * @param body
	 * @return
	 */
    void packGrpcResponse(shared_ptr<TC_GrpcServer::Http2Context> &context, const int status, const string &body);

    static shared_ptr<TC_GrpcServer> getHttp2(uint32_t uid);
    static void addHttp2(uint32_t uid, const shared_ptr<TC_GrpcServer> &ptr);
    static void delHttp2(uint32_t uid);
    static TC_NetWorkBuffer::PACKET_TYPE parseGrpc(TC_NetWorkBuffer&in, vector<char> &out);

protected:

    static TC_SpinLock _mutex;
    static unordered_map<int32_t, shared_ptr<TC_GrpcServer>> _http2;
};

/////////////////////////////////////////////////////////////////////////////////

class TC_GrpcClient : public TC_Http2Client
{
public:

	/**
	 * constructor
	 */
    TC_GrpcClient();

    /**
     * deconstructor
     */
    ~TC_GrpcClient();


	/**
     * parse response
     * @param in
     */
    TC_NetWorkBuffer::PACKET_TYPE parseResponse(TC_NetWorkBuffer &in, pair<int, shared_ptr<TC_HttpResponse>> &out);

	//    int submit(const string &method, const string &path, const map<string, string> &header, const vector<char> &buff);
	int submit(const TC_HttpRequest &request);
		/**
	 * @brief response
	 */
    std::unordered_map<int, shared_ptr<TC_HttpResponse>> &responses() { return _responses; }

    /** 
     * @brief response finished
     */
    std::unordered_map<int, shared_ptr<TC_HttpResponse>> &doneResponses() { return _doneResponses; }

private:

    /**
     * 收到的响应
	 * Responses received
     */
    std::unordered_map<int, shared_ptr<TC_HttpResponse>> _responses;

    /**
     * 收到的完整响应
	 * Complete response received
     */
    std::unordered_map<int, shared_ptr<TC_HttpResponse>> _doneResponses;

};

  
}


#endif  //__TC_GRPC_PROTOCOL_H__