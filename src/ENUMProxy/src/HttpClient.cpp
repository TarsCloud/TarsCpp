#include <cstdlib>
#include <mutex>

#include "servant/Application.h"
#include "HttpClient.h"

using namespace Http;

Client::Client(const std::string& url, int retry, const Common::SSLContextPtr& sslCtx, size_t maxHeadLen):_parser(_response, maxHeadLen)
{
    std::string root;
    std::string port;

    _ssl = Http::parseUrl(url.c_str(), _host, port, root);
    setRequestUrl(root);

    if(!_ssl)
    {
        if(port == "80")
            setHeader("Host", _host);
        else
            setHeader("Host", _host + ":" + port);
    }
    else
    {
        _sslCtx = sslCtx;
        if(port == "443")
            setHeader("Host", _host);
        else
            setHeader("Host", _host + ":" + port);
    }
    _port = atoi(port.c_str());

    _retry = retry;
    reset();
    _actived = false;
    _process = false;
    _stage = 0;
}

int Client::onTlsVerify(int err)
{
    if (tlsVerify())
        return err;
    else
        return 1;
}

void Client::recv(const unsigned char* data, int dataLen)
{
    bool reconnect = false;
    bool closed = false;
    bool error = false;
    bool closing = false;

    std::string strRemoteHost,strLocalHost;
    int iRemotePort, iLocalPort = 0;

    if (_sender != 0)
    {
        _sender->getRemote(strRemoteHost,iRemotePort);
        _sender->getLocal(strLocalHost, iLocalPort);
    }

    std::string resp((char*)data);
    int len = strlen((char*)data);
    TLOGDEBUG("[HttpClient] " << std::string(__FUNCTION__) + "(),iLocalPort:" + std::to_string(iLocalPort) + ",remote:" + strRemoteHost+ ",port:" +
                 std::to_string(iRemotePort) + ",resp:" +  resp + ",dataLen:" + std::to_string(dataLen) + ",len:" + std::to_string(len) << endl);

    {
        std::unique_lock<tars::TC_ThreadRecMutex> selfLock(*this);
        if(_actived && _stage)
        {
            if(_parser.leftLen() == (size_t)-1)
            {
                if(_parser.input((const char*)data, dataLen))
                {
                    if(_parser.leftLen() != (size_t)-1)
                    {
                        _process = true;
                        selfLock.unlock();
                        closed = !onResponse(_response);
                        selfLock.lock();
                        _process = false;

                        if(!closed)
                        {
                            if(_actived)
                            {
                                if((contentLength() != (size_t)-1) || chunked())
                                {
                                    const std::string& ct = _response.content();
                                    if(ct.length())
                                    {
                                        int r = recvContent((const unsigned char*)ct.c_str(), ct.length());

                                        if(r == 1)
                                            closed = true;
                                        else if(!r && !_actived)
                                            closing = true;
                                    }
                                }
                                else
                                {
                                    int r = recvContent(0, 0);

                                    if(r == 1)
                                        closed = true;
                                    else if(!r && !_actived)
                                        closing = true;
                                }
                            }
                            else
                                closing = true;
                        }
                    }
                }
                else
                {
                    if(_retry)
                    {
                        _retry--;
                        reconnect = true;
                    }
                    else
                        error = true;
                }
            }
            else
            {
                int r = recvContent(data, dataLen);
                if(r == 1)
                    closed = true;
                else if(!r && !_actived)
                    closing = true;
            }
        }

        if(reconnect || closed || error)
            _actived = false;
    }

    if(reconnect)
        retry();
    else if(closed)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(_mutextSender);
        Common::NetSenderPtr sender = _sender;

        _sslCtx = 0;
        if( sender)
            sender->close();
        _sender = 0;
    }
    else if(error)
    {
        onError2(-2, data, dataLen);
        _close(-2);
    }
    else if(closing)
        _close(0);
}

void Client::onConnClose()
{
    close(true);
}

bool Client::request(Common::NetDriverPtr driver, const std::string& localHost, int localPort)
{
    if(connect(driver, localHost, localPort))
    {
        request();
        return true;
    }

    return false;
}

void Client::request()
{
    {
        std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
        _stage++;
        _startTicks = TNOWMS;
    }

    std::string buf = serialize();
    tars::TC_LockT<tars::TC_ThreadRecMutex> lock(_mutextSender);
    if (!_sender)
    {
        TLOGWARN("[HttpClient] " << std::string(__FUNCTION__) + "(), host:" +  _host.c_str() + ", _port:" + std::to_string(_port) + ",buf:\n" + buf.c_str() << endl);
        return;
    }

    int i = _sender->send((const unsigned char*)buf.c_str(), buf.length());
    TLOGDEBUG("[HttpClient] " << std::string(__FUNCTION__) + "(), host:" +  _host.c_str() + ", _port:" + std::to_string(_port) + ",buf:\n" + buf.c_str() + ",i:" + std::to_string(i) << endl);
}

bool Client::connect(Common::NetDriverPtr driver, const std::string& localHost, int localPort)
{
    std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
    tars::TC_LockT<tars::TC_ThreadRecMutex> lockSender(_mutextSender);
    // if(!_ssl)
    //     _sender = driver->connect("tcp", localHost, localPort, _host.c_str(), _port, this);
    // else
    // {
    //     initTlsDriver(driver);
    //     _sender = driver->connect("tls", localHost, localPort, _host.c_str(), _port, this);
    // }

    if(_sender)
    {
        _actived = true;
        _driver = driver;
        return true;
    }

    return false;
}

bool Client::checkTimeout( int64_t ticks, unsigned int period)
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

void Client::close()
{
    close(false);
}

void Client::reset()
{
    _parser.clear();
    _chunk.clear();
    _recvedLen = 0;
}

void Client::retry()
{
    std::string localHost;
    int localPort;

    reset();
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(_mutextSender);
        _sender->getLocal(localHost, localPort);
        _sender->close();
    }

    request(_driver, localHost, localPort);
}

void Client::_close(int err)
{
    onError(err);
    _sslCtx = 0;

    tars::TC_LockT<tars::TC_ThreadRecMutex> lock(_mutextSender);
    Common::NetSenderPtr sender = _sender;
    if(sender)
        sender->close();
    _sender = 0;
}

void Client::close(bool rec)
{
    bool closed = false;
    bool reconnect = false;

    {
        std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
        if(_actived)
        {
            _actived = false;
            if(!_process)
            {
                if(_retry && rec)
                {
                    _retry--;
                    reconnect = true;
                }
                else
                    closed = true;
            }
        }
    }

    if(reconnect)
        retry();
    else if(closed)
        _close(0);
}

int Client::recvContent(const unsigned char* data, int dataLen)
{
    bool re = false;
    bool closed = false;

    std::unique_lock<tars::TC_ThreadRecMutex> lock(*this);
    _process = true;
    if(_parser.chunked())
    {
        bool f = _chunk.input((const char*)data, dataLen);
        std::string out = _chunk.output();

        if(f || out.length())
        {
            lock.unlock();
            if(out.length())
                if(!onRecvContent((const unsigned char*)out.c_str(), out.length()))
                    closed = true;

            if(!closed && f)
            {
                if(onRecvContent(0, 0))
                {
                    re = true;
                    _stage--;
                }
                else
                    closed = true;
            }
            lock.lock();
        }
    }
    else
    {
        lock.unlock();
        if(dataLen)
        {
            size_t len = _recvedLen + dataLen > contentLength() ? contentLength() - _recvedLen : dataLen;
            if(onRecvContent(data, len))
            {
                _recvedLen += len;
                if(_recvedLen == contentLength())
                {
                    if(onRecvContent(0, 0))
                    {
                        re = true;
                        _stage--;
                    }
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
            {
                re = true;
                _stage--;
            }
            else
                closed = true;
        }
        lock.lock();
    }

    _process = false;
    if(!closed && _actived && re)
        reset();

    if(re)
        return 2;
    else if(closed)
        return 1;
    else
        return 0;
}

void Client::initTlsDriver(const Common::NetDriverPtr& driver)
{
    if (_sslCtx)
    {
        map<std::string,std::string> trustedCerts = _sslCtx->getTrustedCerts();
        map<std::string,std::string>::const_iterator it1; 
        for (it1 = trustedCerts.begin();it1 != trustedCerts.end();it1++)
        {
            std::string key = it1->first;
            std::string value = it1->second;
            //driver->setConfig(key, value);
        }

        int verify = _sslCtx->getNetworkTlsExternVerify();
        if (verify >= 1)
            verify = 1;
        else
            verify = 0;
        //driver->setConfig("Network.Tls.ExternVerify", std::string(verify));
    }
    else
    {
       //driver->setConfig("Network.Tls.ExternVerify", std::string(1));
    }
}

bool Client::tlsVerify()
{
    return _sslCtx ? _sslCtx->clientTlsVerify() : false;
}

size_t Client::contentLength() const
{
    return _parser.contentLen();
}

bool Client::chunked() const
{
    return _parser.chunked();
}

bool Client::onResponse(Http::Response& response)
{
    return true;
}

bool Client::onRecvContent(const unsigned char* data, size_t len)
{
    return true;
}

void Client::onError(int err) {}

void Client::onError2(int err, const unsigned char* data, size_t len){}
