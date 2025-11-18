#include "HttpServerSession.h"
#include "servant/Application.h"

namespace SipENUMProxy
{

    HttpServerSession::HttpServerSession(const ChainPtr & chain, const Common::NetSenderPtr &sender, size_t maxHeadLen, const HttpRequestReceiverPtr & receiver):
            Http::ServerSession(sender, maxHeadLen), Ring(chain)
    {
        m_receiver = receiver;
    }

    bool HttpServerSession::authCheck(const std::string &user, const std::string &passwd, const std::string &auth)
    {
        return true;
    }

    bool HttpServerSession::onRequest(Http::Request &request)
    {
        std::string senderHost; int senderPort;
        _sender->getRemote(senderHost, senderPort);
        std::string sender = senderHost + ":" + std::to_string(senderPort);

        TLOGDEBUG("[HttpServerSession] " << __FUNCTION__ << "(), text:" << request.message() << endl);
        std::string statusCode;

        if(request.method()=="POST")
        {
            if((contentLength()!=(size_t)-1) || chunked())
            {
                return true;
            }
            else
            {
                statusCode = "411";
            }
        }
        else
        {
            statusCode = "405";
        }

        setStatusCode(statusCode.c_str());
        response();
        unlink();
        return false;
    }

    bool HttpServerSession::onRecvContent(const unsigned char *data, size_t len)
    {
        TLOGINFO("[HttpServerSession] " << __FUNCTION__ << "(), len:" << len << endl);

        if(len)
           m_content.append((const char*)data, len);
        else
        {
            std::string reason;
            do
            {
                //try
                //{
                //    Json::Reader reader;
                //    if (false == reader.parse(m_content.c_str(), m_request))
                //    {
                //        reason = "decode json failed.";
                //        break;
                //    }

                //    Json::Value head;
                //    if (m_request.isMember("head"))
                //        head = m_request.get("head", head);
                //    else
                //    {
                //        reason = "head not exist";
                //        break;
                //    }

                //    Json::Value callSeq;
                //    if (head.isMember("IDNO"))
                //        callSeq = head.get("IDNO", callSeq);
                //    else
                //    {
                //        reason = "IDNO not exist";
                //        break;
                //    }

                //    m_callSeq = callSeq.asString().c_str();
                //}
                //catch (...)
                //{
                //    reason = !reason.empty() ? "json invalid;" : reason;
                //    break;
                //}

                Json::Reader reader;
                if (false == reader.parse(m_content.c_str(), m_request))
                {
                    reason = "decode json failed.";
                    break;
                }

                if (m_receiver)
                {
                    bool ret = m_receiver->onHttpRequest(m_content, m_request, this);
                    if (!ret)
                    {
                        reason = "call receiver failed.";
                        break;
                    }
                }
            } while (0);

            m_content.clear();
            if (!reason.empty())
            {
                TLOGWARN("[HttpServerSession] " << __FUNCTION__ << "(),text:" << m_content << ",reason:" << reason << endl);
                setStatusCode("501");
                response();
                unlink();
                return false;
            }
        }
        return true;
    }

    void HttpServerSession::onError(bool closed)
    {
        unlink();
    }

    void HttpServerSession::onSendReply(bool ret, int errCode)
    {
        this->setStatusCode("200");
        this->setHeader("Accept", "application/json, text/json");
        this->setHeader("Accept-Language", "zh-CN");
        this->setHeader("Connection", "keep-alive");
        this->setHeader("Content-Type", "application/json");
        this->setHeader("Cache-Control", "no-cache");

        try
        {
            Json::Value head;
            Json::Value body;
            if (m_request.isMember("head") && m_request.isMember("body"))
            {
                head = m_request.get("head", head);
                body = m_request.get("body", body);

                //response
                Json::Value response;
                Json::Value respHead, respBody(Json::objectValue);
                Json::Value::Members headMembers = head.getMemberNames();
                for (unsigned int i = 0; i < headMembers.size(); i++)
                {
                    Json::Value tmp;
                    if (strcmp("IDNO", headMembers.at(i).c_str()) != 0)
                        respHead[headMembers.at(i).c_str()] = head.get(headMembers.at(i).c_str(), tmp);
                }
                respHead["Result"] = Json::Value(errCode);

                //Json::Value::Members bodyMembers = body.getMemberNames();
                //for (unsigned int i = 0; i < bodyMembers.size(); i++)
                //{
                //    Json::Value tmp;
                //    std::string strReq = "ReqName";
                //    if (strcmp(bodyMembers.at(i).c_str(), strReq.c_str()))
                //        respBody[bodyMembers.at(i).c_str()] = body.get(bodyMembers.at(i).c_str(), tmp);
                //}

                response["head"] = respHead;
                response["body"] = respBody;
                Json::StyledWriter write;
                std::string strResp = write.write(response);

                TLOGINFO("[HttpServerSession] " << __FUNCTION__ << "(), call seq:" << m_callSeq << ",resp:" << strResp << ",errCode:" << errCode << endl);
                this->content() = strResp.c_str();
            }
        }
        catch (...)
        {
        }

        if( !this->response(false))
            TLOGWARN("[HttpServerSession] " << __FUNCTION__ << "(), response after connection closed, call seq:" << m_callSeq << endl);
        this->clear();
    }

    void HttpServerSession::onSendReply(bool ret, int errCode, Json::Value text)
    {
        TLOGINFO("[HttpServerSession] " << __FUNCTION__ << "(), called" << endl);

        this->setStatusCode("200");
        this->setHeader("Accept", "application/json, text/json");
        this->setHeader("Accept-Language", "zh-CN");
        this->setHeader("Connection", "keep-alive");
        this->setHeader("Content-Type", "application/json");
        this->setHeader("Cache-Control", "no-cache");

        Json::StyledWriter write;
        std::string strResp = write.write(text);

        this->content() = strResp.c_str();

        if( !this->response(false))
            TLOGWARN("[HttpServerSession] " << __FUNCTION__ << "(), response after connection closed, call seq:" << m_callSeq << endl);
        this->clear();
    }

    //-------------------------------------- HttpServerManager  -------------------------------------
    HttpServerManager::HttpServerManager(const Common::NetDriverPtr & driver, const Common::SSLContextPtr & sslCtx, size_t maxHeadLen, const HttpRequestReceiverPtr & receiver)
    {
        m_receiver = receiver;
        m_maxHeadLen = maxHeadLen;
        m_chain = new Chain();
        m_driver = driver;
    }

    Common::NetSenderPtr HttpServerManager::listen(const std::string & localHost, int localPort)
    {
        m_sender = m_driver->listen("tcp", localHost, localPort, this);
        return m_sender;
    }

    Common::NetReceiverPtr HttpServerManager::recvConnection(const Common::NetSenderPtr& sender)
    {
        HttpServerSessionPtr session = new HttpServerSession(m_chain, sender, m_maxHeadLen, m_receiver);

        session->linkTail();
        return session;
    }

    void HttpServerManager::checkTimeout()
    {
        int64_t curTicks = TNOWMS;

        for(HttpServerSessionPtr session= m_chain->fetch<HttpServerSessionPtr>(); session; session = m_chain->fetch<HttpServerSessionPtr>())
        {
            if(!session->checkTimeout(curTicks, 45000))
            {
                m_chain->store(session);
                break;
            }
        }
    }

}

