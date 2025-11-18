#ifndef __ENUM_HTTP_SERVER_SESSION_H
#define __ENUM_HTTP_SERVER_SESSION_H

#include "HttpClient.h"
#include "HttpServer.h"
#include "json/json.h"
#include "Chain.h"

namespace SipENUMProxy
{
    class HttpServerSession;
    typedef tars::TC_AutoPtr<HttpServerSession> HttpServerSessionPtr;

    class HttpRequestReceiver : public tars::TC_HandleBase
    {
    public:
        virtual bool onHttpRequest(const std::string & content, const Json::Value & request, const HttpServerSessionPtr & httpServer) = 0;
    };
    typedef tars::TC_AutoPtr<HttpRequestReceiver> HttpRequestReceiverPtr;

    enum HttpReqType
    {
        kHttpReqInvalid = -1,
        kHttpReqQueueInfo = 0,
        kHttpReqAnswerReq = 1,
        kHttpReqStartPlay = 2,
        kHttpReqStopPlay = 3,
        kHttpReqInService = 4,
    };

    class HttpServerSession : virtual public Http::ServerSession,
                              virtual public Ring
    {
    public:

        HttpServerSession(const ChainPtr & chain, const Common::NetSenderPtr & sender, size_t maxHeadLen, const HttpRequestReceiverPtr & receiver);
    private:
        bool authCheck(const std::string & user, const std::string & passwd, const std::string & auth);

    protected:
        virtual bool onRequest(Http::Request &request);
        virtual bool onRecvContent(const unsigned char *data, size_t len);
        virtual void onError(bool closed);

    public:
        void onSendReply(bool ret, int errCode);
        void onSendReply(bool ret, int errCode, const Json::Value text);

    private:

        std::string          m_content;
        HttpRequestReceiverPtr  m_receiver;

        Json::Value             m_request;
        std::string             m_callSeq;
    };


    class HttpServerManager : virtual public Common::NetReceiver
    {
    public:

        HttpServerManager(const Common::NetDriverPtr & driver, const Common::SSLContextPtr & sslCtx, size_t maxHeadLen, const HttpRequestReceiverPtr & receiver);
        Common::NetSenderPtr listen(const std::string & localHost, int localPort);

    public:

        virtual Common::NetReceiverPtr recvConnection(const Common::NetSenderPtr& sender);
        void checkTimeout();

    private:

        std::map<int64_t, HttpServerSessionPtr>     m_sessions;
        tars::TC_ThreadMutex                        m_mutexSessions;

        size_t                                      m_maxHeadLen;
        HttpRequestReceiverPtr                      m_receiver;
        Common::NetSenderPtr                        m_sender;
        ChainPtr                                    m_chain;
        Common::NetDriverPtr                        m_driver;
    };
    typedef tars::TC_AutoPtr<HttpServerManager> HttpServerManagerPtr;


}

#endif