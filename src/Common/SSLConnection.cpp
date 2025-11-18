#include "SSLConnection.h"

using namespace Common;

bool SSLContextI::setPrivateKeyFile(const std::string& keyFile)
{
    _privateKeyFile = keyFile;
    return true;
}

bool SSLContextI::setCertificateFile(const std::string& crtFile)
{
    _certificateFile = crtFile;
    return true;
}

bool SSLContextI::setTrustedCert(const std::string& certName, const std::string& certContent)
{
    if (!certName.empty())
        _trustedCerts[certName] = certContent;
    return true;
}

void SSLContextI::setNetworkTlsExternVerify(int open)
{
    if (open >= 1)
        _networkTlsExternVerify = 1;
    else
        _networkTlsExternVerify = 0;
}

int SSLContextI::getNetworkTlsExternVerify()
{
    return _networkTlsExternVerify;
}

std::string SSLContextI::getPrivateKeyFile()
{
    return _privateKeyFile;
}

std::string SSLContextI::getCertificateFile()
{
    return _certificateFile;
}

map<std::string, std::string> SSLContextI::getTrustedCerts()
{
    return _trustedCerts;
}

bool SSLContextI::clientTlsVerify()
{
    return _trustedCerts.empty() ? false : true;
}

SSLContextPtr SSLContext::create()
{
    SSLContextPtr ctx= new SSLContextI();
    ctx->setNetworkTlsExternVerify(1);
    return ctx;
}

TlsClient::TlsClient(const NetReceiverPtr &receiver, const SSLContextPtr& ctx)
{
    _receiver = receiver;
    _ctx = ctx;
}

void TlsClient::close()
{
    _receiver = 0;
    _ctx = 0;
    if (_sender)
    {
        _sender->close();
        _sender = 0;
    }
}

int TlsClient::send( const unsigned char *data, int dataLen)
{
    if (!_sender)
        return -1;
    return _sender->send(data, dataLen);
}

bool TlsClient::isReady()
{
    if (!_sender || _sender->isClosed())
        return false;
    return _sender->isSendReady();
}

void TlsClient::recv(const unsigned char *data, int dataLen)
{
    if(_receiver)
        _receiver->recv(data, dataLen);
}


NetSenderPtr TlsClient::connect(const NetDriverPtr& driver, const std::string& localHost,int localPort, const std::string& remoteHost, int remotePort)
{
    initTlsDriver(driver);
    _sender = driver->connect("tls", localHost, localPort, remoteHost, remotePort, this);
    if (_sender && _receiver)
        _receiver->onConnReady();
    return _sender;
}

int TlsClient::onTlsVerify(int err)
{
    if (_ctx && _ctx->clientTlsVerify())
        return err;
    else
        return 1;
}

void TlsClient::initTlsDriver(const NetDriverPtr& driver)
{
    if (_ctx)
    {
        map<std::string,std::string> trustedCerts = _ctx->getTrustedCerts();
        map<std::string,std::string>::const_iterator it1; 
        for (it1 = trustedCerts.begin();it1 != trustedCerts.end();it1++)
        {
            std::string key = it1->first;
            std::string value = it1->second;
            driver->setConfig(key, value);
        }

        int verify = _ctx->getNetworkTlsExternVerify();
        if (verify >= 1)
            verify = 1;
        else
            verify = 0;
        //driver->setConfig("Network.Tls.ExternVerify", Common::std::string(verify));
    }
    else
    {
        //driver->setConfig("Network.Tls.ExternVerify", Common::std::string(1));
    }
}

NetSenderPtr Common::connectSSL(const NetDriverPtr& driver, const std::string& localHost, int localPort, const std::string& remoteHost, int remotePort, const NetReceiverPtr& receiver)
{
    TlsClientPtr tlsClient = new TlsClient(receiver, 0);
    return tlsClient->connect(driver, localHost, localPort, remoteHost, remotePort);
}

NetSenderPtr Common::connectSSL(const NetDriverPtr& driver, const std::string& localHost, int localPort, const std::string& remoteHost, int remotePort, const SSLContextPtr& ctx, const NetReceiverPtr& receiver)
{
    TlsClientPtr tlsClient = new TlsClient(receiver, ctx);
    return tlsClient->connect(driver, localHost, localPort, remoteHost, remotePort);
}

NetSenderPtr Common::listenSSL(const NetDriverPtr& driver, const std::string& localHost, int localPort, const SSLContextPtr& ctx, const NetReceiverPtr& receiver)
{
    return driver->listen("tls", localHost, localPort, new TlsServer(receiver));
}
