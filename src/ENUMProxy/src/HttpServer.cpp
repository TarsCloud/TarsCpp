#include <sstream>
#include <mutex>

#include "HttpServer.h"
#include "servant/Application.h"

using namespace Common;
using namespace Http;

namespace
{
    class TmpUnlock
    {
    public:
        explicit TmpUnlock(tars::TC_ThreadRecMutex &mutex) : _mutex(mutex)
        {
            _mutex.unlock();
        }

        ~TmpUnlock()
        {
            _mutex.lock();
        }

    private:
        tars::TC_ThreadRecMutex &_mutex;
    };
}

ServerSession::ServerSession(const NetSenderPtr& sender, size_t maxHeadLen):
_parser(_request, maxHeadLen)
{
    _sender = sender;
    _actived = true;
    _process = false;
    reset();
    resetTime();
}

bool ServerSession::response(bool chunked)
{
    NetSenderPtr sender;

    {
        std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
        sender = _sender;
    }

    if(sender)
    {
        string data = serialize(chunked);

        sender->send((const unsigned char*)data.c_str(), data.length());
        return true;
    }

    return false;
}

bool ServerSession::sendChunk( const unsigned char* data, size_t len)
{
    NetSenderPtr sender;

    {
        std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
        sender = _sender;
    }

    if(sender)
    {
        if(len)
        {
            std::stringstream ss;

            ss<<std::hex<<len<<"\r\n";
            sender->send((const unsigned char*)ss.str().c_str(), ss.str().length());
            sender->send(data, len);
            sender->send((const unsigned char*)"\r\n", 2);
        }
        else
            sender->send((const unsigned char*)"0\r\n\r\n", 5);

        return true;
    }

    return false;
}

bool ServerSession::endChunk()
{
    return sendChunk(0, 0);
}

void ServerSession::resetTime()
{
    std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
    _startTicks = TNOWMS;
}

bool ServerSession::checkTimeout(int64_t ticks, unsigned int period)
{
    bool closed = false;
    {
        std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
        if(ticks - _startTicks > period)
            closed = true;
    }

    if(closed)
    {
        close(false);
        return true;
    }

    return false;
}

void ServerSession::close()
{
    close(false);
}

string ServerSession::header(const string & name)
{
    return _request.header(name);
}

bool ServerSession::recvContent(const unsigned char* data, int dataLen)
{
    bool re = false;
    bool closed = false;

    _process = true;
    {
        if(_parser.chunked())
        {
            bool f = _chunk.input((const char*)data, dataLen);
            string out = _chunk.output();

            if(f || out.length())
            {
                TmpUnlock unlock(*this);
                if(out.length())
                    if(!onRecvContent((const unsigned char*)out.c_str(), out.length()))
                        closed = true;

                if(!closed && f)
                {
                    if(onRecvContent(0, 0))
                        re = true;
                    else
                        closed = true;
                }
            }

        }
        else
        {
            TmpUnlock unlock(*this);
            if(dataLen)
            {
                size_t len = _recvedLen + dataLen > contentLength() ? contentLength() - _recvedLen : dataLen;
                if(onRecvContent(data, len))
                {
                    _recvedLen += len;
                    if(_recvedLen == contentLength())
                    {
                        if(onRecvContent(0, 0))
                            re = true;
                        else
                            closed = true;
                    }
                }
                else
                    closed = true;
            }
            else
            {
                if(onRecvContent(0, 0))
                    re = true;
                else
                    closed = true;
            }
        }
    }
    _process = false;

    if(!closed)
    {
        if(_actived)
        {
            if(re)
                reset();
        }
        else
            _close(false);
    }

    return closed;
}

void ServerSession::recv(const unsigned char* data, int dataLen)
{
    bool closed = false;
    bool error = false;

    {
        std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
        if(_actived)
        {
            size_t oLeftLen = _parser.leftLen();

            if(_parser.leftLen() == (size_t)-1)
            {
                if(_parser.input((const char*)data, dataLen))
                {
                    if(_parser.leftLen() != (size_t)-1)
                    {
                        bool r;

                        _process = true;
                        {
                            TmpUnlock unlock(*this);
                            r = onRequest(_request);
                        }
                        _process = false;

                        if(r)
                        {
                            if(_actived)
                            {
                                if(_request.header("Expect") == "100-continue")
                                {
                                    NetSenderPtr sender;
                                    {
                                        std::unique_lock<tars::TC_ThreadRecMutex> innerLock(*this);
                                        sender = _sender;
                                    }

                                    if(sender)
                                    {
                                        string data = "HTTP/1.1 100 Continue\r\n\r\n";
                                        sender->send((const unsigned char*)data.c_str(), data.length());
                                    }
                                }

                                if((contentLength() != (size_t)-1) || chunked())
                                {
                                    const string& ct = _request.content();

                                    if(ct.length())
                                        closed = recvContent((const unsigned char*)ct.c_str(), ct.length());
                                }
                                else
                                    closed = recvContent(0, 0);
                            }
                            else
                                _close(false);
                        }
                        else
                            closed = true;
                    }
                }
                else
                {
                    _actived = false;
                    closed = true;
                    error = true;
                }
            }
            else
                closed = recvContent(data, dataLen);

            if(closed)
                _actived = false;
        }
    }

    if(closed)
    {
        ServerSessionPtr _this = this;

        if(error)
            onError(false);
        _sender->close();
        _sender = 0;
    }
}

void ServerSession::onConnClose()
{
    close(true);
}

size_t ServerSession::contentLength() const
{
    return _parser.contentLen();
}

bool ServerSession::chunked() const
{
    return _parser.chunked();
}

bool ServerSession::onRequest(Http::Request& request)
{
    return true;
}

bool ServerSession::onRecvContent( const unsigned char* data, size_t len)
{
    return true;
}

void ServerSession::onError( bool closed) {}

void ServerSession::reset()
{
    _parser.clear();
    _chunk.clear();
    _recvedLen = 0;
}

void ServerSession::_close(bool error)
{
    onError(!error);
    _sender->close();
    _sender = 0;
}

void ServerSession::close(bool error)
{
    bool closed = false;

    {
        std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);

        if(_actived)
        {
            _actived = false;
            if(!_process)
                closed = true;
        }
    }

    if(closed)
        _close(error);
}
