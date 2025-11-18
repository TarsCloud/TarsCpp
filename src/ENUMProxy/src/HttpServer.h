#ifndef __HttpServer_h
#define __HttpServer_h

#include <memory>

#include "HttpParser.h"
#include "Common/SSLConnection.h"
#include "util/tc_autoptr.h"
#include "util/tc_thread_mutex.h"

namespace Http
{
    class ServerSession;
    typedef tars::TC_AutoPtr<ServerSession> ServerSessionPtr;

    class ServerSession: public Http::Response,
                            public tars::TC_ThreadRecMutex,
                             public Common::NetReceiver
    {
    public:
        ServerSession(const Common::NetSenderPtr& sender, size_t maxHeadLen = 16*1024);
        bool response(bool chunked = false);
        bool sendChunk(const unsigned char* data, size_t len);
        bool endChunk();
        void resetTime();
        bool checkTimeout(int64_t ticks, unsigned int period);
        void close();
        string header(const string& name);
        bool recvContent(const unsigned char* data, int dataLen);
        void recv(const unsigned char* data, int dataLen);
        void onConnClose();
        size_t contentLength() const;
        bool chunked() const;

        virtual bool onRequest(Http::Request& request);
        virtual bool onRecvContent(const unsigned char* data, size_t len);
        virtual void onError(bool closed);

    private:
        void reset();
        void _close(bool error);
        void close(bool error);

    protected:
        Common::NetSenderPtr _sender;

    private:
        Http::Request _request;
        Http::RequestParser _parser;
        Http::ChunkParser _chunk;
        size_t _recvedLen;
        bool _actived;
        bool _process;
        int64_t _startTicks;
    };
}

#endif
