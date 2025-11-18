#ifndef __ENUM_HTTP_CLIENT_SESSION_H
#define __ENUM_HTTP_CLIENT_SESSION_H

#include "HttpClient.h"
#include "HttpServer.h"
#include "json/json.h"
#include "Chain.h"

namespace SipENUMProxy
{
    enum HttpRequestTarget
    {
        kHttpReqTypeInvalid = -1,
        kHttpReqTypeGetQueuePosition = 0,
        kHttpReqTypeGetQueueLength = 1,
        kHttpReqTypeAbnormalNotify = 2,
        kHttpReqTypeOthers = 3,
    };

    class HttpRespReceiver : public tars::TC_HandleBase
    {
    public:
        virtual void onHttpResponse(bool ret, int64_t tid, const std::string& serialNum, const std::string& resCode, const std::string& content, const std::string& reason) = 0;
    };
    typedef tars::TC_AutoPtr<HttpRespReceiver> HttpRespReceiverPtr;

    class HttpClientSession : virtual public Http::Client,
                              virtual public Ring
    {

    public:
        HttpClientSession(const ChainPtr & chain, const HttpRespReceiverPtr & callBack, const std::string& url, const Common::SSLContextPtr & sslCtx, size_t maxHeadLen, int64_t tid, const std::string& callSerialNum);

    protected:
        virtual bool onResponse(Http::Response &response);
        virtual bool onRecvContent(const unsigned char *data, size_t len);
        virtual void onError(int err);
        virtual void onError2(int err, const unsigned char *data, size_t len);

    public:
        void request(int64_t tid, const std::string& text);
        void setReqType(HttpRequestTarget type);

    private:

        int64_t           m_tid;
        std::string         m_content;
        std::string         m_remote;
        HttpRespReceiverPtr     m_callBack;
        std::string         m_callSerialNum;
        HttpRequestTarget       m_reqType;
    };
    typedef tars::TC_AutoPtr<HttpClientSession> HttpClientSessionPtr;

    class HttpClientManager : public tars::TC_HandleBase
    {
    public:

        HttpClientManager(const Common::NetDriverPtr & netDriver, unsigned int maxHeadLen, const std::string& localHost);

        bool sendRequest(const std::string& server, const Common::SSLContextPtr & ssl, const HttpRespReceiverPtr & callBack, int64_t tid, const std::string& text, const std::string& callSerialNum, HttpRequestTarget reqType);
        void checkTimeout();

    private:

        std::string         m_localHost;
        size_t                  m_maxHeadLen;
        ChainPtr                m_chain;
        Common::NetDriverPtr    m_driver;
    };
    typedef tars::TC_AutoPtr<HttpClientManager> HttpClientManagerPtr;

}

#endif
