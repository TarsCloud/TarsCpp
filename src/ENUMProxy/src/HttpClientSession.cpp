#include "HttpClientSession.h"
#include "Chain.h"
#include "servant/Application.h"

namespace SipENUMProxy
{

    HttpClientSession::HttpClientSession(const ChainPtr & chain, const HttpRespReceiverPtr & callBack, const std::string & url, const Common::SSLContextPtr & sslCtx, size_t maxHeadLen, int64_t tid, const std::string & callSerialNum)
            : Http::Client(url, 0, sslCtx, maxHeadLen),Ring(chain)
    {
        m_remote = url;
        m_callBack = callBack;
        m_tid = tid;
        m_callSerialNum = callSerialNum;
        m_reqType = kHttpReqTypeGetQueuePosition;
    }

    bool HttpClientSession::onResponse(Http::Response &response)
    {
        TLOGDEBUG("[HttpClientSession] " << std::string(__FUNCTION__) + "(), header:" + response.message(false).c_str() + ", tid:" + std::to_string(m_tid) << endl);
        std::string reason;
        if(response.statusCode().substr(0, 2) == "20")
        {
            if((contentLength()!=(size_t)-1) || chunked())
                return true;
            else
                reason = "invalid-notify-replay-content";
            TLOGWARN("[HttpClientSession] " << std::string(__FUNCTION__) + "(), header:" + response.message(false).c_str() + ", tid:" + std::to_string(m_tid) << endl);
        }

        if (m_callBack)
            m_callBack->onHttpResponse(false, m_tid, m_callSerialNum, std::string(response.statusCode().c_str()), "", reason);
        unlink();
        return false;
    }

    bool HttpClientSession::onRecvContent(const unsigned char *data, size_t len)
    {
        TLOGDEBUG("[HttpClientSession] " << std::string(__FUNCTION__) + "(), remote:" + m_remote + ", tid:" + std::to_string(m_tid) + "content=[" + m_content + "]" << endl );

        if(len)
            m_content.append((const char*)data, len);
        else
        {
            if (m_callBack)
                m_callBack->onHttpResponse(true, m_tid, m_callSerialNum, "200", m_content, "");
            unlink();
            return false;
        }
        return true;
    }

    void HttpClientSession::onError(int err)
    {
        std::string failStr;

        if(err == -2)
            failStr = "invalid-notify-replay-content";
        else
            failStr = "http-connection-error";

        if (m_callBack)
            m_callBack->onHttpResponse(false, m_tid, m_callSerialNum, "500", "", failStr);

        TLOGWARN("[HttpClientSession] " << std::string(__FUNCTION__) + "(),tid:" + std::to_string(m_tid) + ",failStr:" + failStr + ", err:" + std::to_string(err) << endl);
        unlink();
    }

    void HttpClientSession::onError2(int err, const unsigned char *data, size_t len)
    {
        bool exist = false;
        std::string msg((char*)data, len);
        if(err == -2)
        {
            TLOGWARN("[HttpClientSession] " << std::string(__FUNCTION__) + "(),tid:" + std::to_string(m_tid) + ",req type:" + std::to_string(m_reqType) + ",failStr:" + msg + ", err:" + std::to_string(err) << endl);
            if (m_reqType == kHttpReqTypeGetQueuePosition || m_reqType == kHttpReqTypeGetQueueLength)
            {
                int idx = 0;
                while(true)
                {
                    int i = msg.find("\r\n", idx);
                    if(i > 0)
                    {
                        std::string line = msg.substr(idx, i-idx);
                        idx = i+2;
                        if (line.find("isSucc") > 0)
                        {
                            exist = true;
                            if (m_callBack)
                                m_callBack->onHttpResponse(true, m_tid, m_callSerialNum, "200", line, "");
                            break;
                        }
                    }
                    else
                        break;
                }
            }
        }

        if (!exist && m_callBack)
        {
            m_callBack->onHttpResponse(false, m_tid, m_callSerialNum, "200", "", "");
        }

        unlink();
    }

    void HttpClientSession::request(int64_t tid, const std::string & text)
    {
        m_tid = tid;
        content() = text.c_str();
        Http::Client::request();
        TLOGDEBUG("[HttpClientSession] " << std::string(__FUNCTION__) + "(), remote:" + m_remote + ", tid:" + std::to_string(m_tid) + ",text=[" + text + "]" << endl);
    }

    void HttpClientSession::setReqType(HttpRequestTarget type)
    {
        m_reqType = type;
    }

    //------------------------------------------------------------------------------------

    HttpClientManager::HttpClientManager(const Common::NetDriverPtr & netDriver, unsigned int maxHeadLen, const std::string & localHost)
    {
        m_driver = netDriver;
        m_maxHeadLen = maxHeadLen;
        m_localHost = localHost;
        m_chain = new Chain();
    }

    bool HttpClientManager::sendRequest(const std::string & host, const Common::SSLContextPtr & ssl, const HttpRespReceiverPtr & callBack, int64_t tid, const std::string & text, const std::string & callSerialNum, HttpRequestTarget reqType)
    {
        HttpClientSessionPtr client;
        if( host.size())
        {
            client = new HttpClientSession(m_chain, callBack, host, 0, m_maxHeadLen, tid, callSerialNum);
            client->linkTail();
            client->setReqType(reqType);

            client->setMethod("POST");
            client->setHeader("Accept", "application/json, text/json");
            client->setHeader("Content-Type", "application/json");
            client->setHeader("Accept-Language", "zh-CN");
            client->setHeader("Connection", "Keep-Alive");        //close?
            client->setHeader("Cache-Control", "no-cache");

            if( client->connect(m_driver, m_localHost, 0))
            {
                client->request(tid, text);
                TLOGINFO("[HttpClientSession] " << std::string(__FUNCTION__) + "(), sent text to remote:" + host + ", tid:" + std::to_string(tid) + ",reqType:" + std::to_string(reqType) + ",callSerialNum:" + callSerialNum + ",m_localHost:" + m_localHost +",text=[" + text + "]" << endl);
                return true;
            }
            else
            {
                TLOGWARN("[HttpClientSession] " << std::string(__FUNCTION__) + "(), failed connect to remote:" + host + ", tid:" + std::to_string(tid) + ",reqType:" + std::to_string(reqType) + ",callSerialNum:" + callSerialNum + ",text=[" + text + "]" << endl);
            }
        }
        else
        {
            TLOGWARN("[HttpClientSession] " << std::string(__FUNCTION__) + "(), remote host is empty. tid:" + std::to_string(tid) + ",text=[" + text + "]" << endl);
        }
        return false;
    }

    void HttpClientManager::checkTimeout()
    {
        int64_t curTicks = TNOWMS;

        for(HttpClientSessionPtr client = this->m_chain->fetch<HttpClientSessionPtr>();
            client;
            client = this->m_chain->fetch<HttpClientSessionPtr>())
        {
            if( !client->checkTimeout( curTicks, 45000))
            {
                this->m_chain->store( client);
                break;
            }
        }
    }

}



