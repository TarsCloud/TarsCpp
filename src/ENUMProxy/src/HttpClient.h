#ifndef __HttpClient_h
#define __HttpClient_h

#include <memory>

#include "Common/SSLConnection.h"
#include "HttpParser.h"

namespace Http
{
    class Client;
    typedef tars::TC_AutoPtr<Client> ClientPtr;

    class Client: public Http::Request
                        , public tars::TC_ThreadRecMutex
                        , public virtual tars::TC_HandleBase
    {
    public:
        Client(const std::string& url, int retry, const Common::SSLContextPtr& sslCtx=0, size_t maxHeadLen = 16*1024);
        int onTlsVerify(int err);
        void recv(const unsigned char* data, int dataLen);
        void onConnClose();
        bool request(Common::NetDriverPtr driver, const std::string& localHost="", int localPort=0);
        void request();
        bool connect(Common::NetDriverPtr driver, const std::string& localHost="", int localPort=0);
        bool checkTimeout(int64_t ticks, unsigned int period);
        void close();

    private:
        void reset();
        void retry();
        void _close(int err);
        void close(bool rec);

        int recvContent(const unsigned char* data, int dataLen);
        void initTlsDriver(const Common::NetDriverPtr& driver);
        bool tlsVerify();

    protected:
        size_t contentLength() const;
        bool chunked() const;

        virtual bool onResponse(Http::Response& response);
        virtual bool onRecvContent(const unsigned char* data, size_t len);
        virtual void onError(int err);
        virtual void onError2(int err, const unsigned char* data, size_t len);

    private:
        std::string _host;
        int _port;
        bool _ssl;
        Common::SSLContextPtr _sslCtx;

        tars::TC_ThreadRecMutex _mutextSender;
        Common::NetSenderPtr _sender;
        int _retry;

        Common::NetDriverPtr _driver;
        Http::Response _response;
        Http::ResponseParser _parser;
        Http::ChunkParser _chunk;
        size_t _recvedLen;

        bool _actived;
        bool _process;
        int _stage;
        int64_t _startTicks;
    };
}

#endif
