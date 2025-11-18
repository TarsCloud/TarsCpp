#ifndef _SSL_SSLConnection_h
#define _SSL_SSLConnection_h

#include <memory>

#include "Util.h"
#include "Net.h"

#include "util/tc_autoptr.h"
#include "util/tc_thread_mutex.h"

namespace Common
{
    class SSLContext;
    class SSLContextI;
    class TlsServerConn;
    class TlsServer;
    class TlsClient;
    typedef tars::TC_AutoPtr<SSLContext> SSLContextPtr;
    typedef tars::TC_AutoPtr<TlsServerConn> TlsServerConnPtr;
    typedef tars::TC_AutoPtr<TlsServer> TlsServerPtr;
    typedef tars::TC_AutoPtr<TlsClient> TlsClientPtr;

    class SSLContext : virtual public tars::TC_HandleBase
    {
    public:
        static SSLContextPtr create();

        virtual bool setPrivateKeyFile(const std::string& keyFile) = 0;   
        virtual bool setCertificateFile(const std::string& crtFile) = 0;

        virtual bool setTrustedCert(const std::string& certName, const std::string& certContent) = 0;
        virtual void setNetworkTlsExternVerify(int open) = 0;
        virtual int getNetworkTlsExternVerify() = 0;

        virtual std::string getPrivateKeyFile() = 0;
        virtual std::string getCertificateFile() = 0;
        virtual map<std::string,std::string> getTrustedCerts() = 0;
        virtual bool clientTlsVerify() = 0;
    };

    class SSLContextI: public SSLContext
    {
    public:
        bool setPrivateKeyFile(const std::string& keyFile);
        bool setCertificateFile(const std::string& crtFile);
        bool setTrustedCert(const std::string& certName, const std::string& certContent);
        void setNetworkTlsExternVerify(int open);
        int getNetworkTlsExternVerify();
        std::string getPrivateKeyFile();
        std::string getCertificateFile();
        map<std::string,std::string> getTrustedCerts();
        bool clientTlsVerify();
    private:
        std::string _privateKeyFile;
        std::string _certificateFile;
        int _networkTlsExternVerify;
        map<std::string,std::string> _trustedCerts;
    };

    class TlsServerConn : public NetReceiver
    {
    public:
        TlsServerConn(const Common::NetSenderPtr& sender,const Common::NetReceiverPtr& receiver) :_sender(sender),_receiver(receiver){}

        void recv(const unsigned char *data, int dataLen)
        {
            if (_receiver)
                _receiver->recv(data, dataLen);
        }

        void send(const unsigned char *data,int dataLen)
        {
            if (_sender)
                _sender->send(data,dataLen);
        }

        void close()
        {
            if (_sender)
            {
                _sender->close();
                _sender = 0;
            }
        }

        virtual void onConnClose()
        {
            _sender = 0;
        }

    private:
        Common::NetSenderPtr _sender;
        Common::NetReceiverPtr _receiver;
    };

    class TlsServer : public Common::NetReceiver
    {
    public:
        TlsServer(const Common::NetReceiverPtr& receiver):_receiver(receiver)
        {
        }

        Common::NetReceiverPtr recvConnection(const Common::NetSenderPtr& sender)
        {
            _receiver->recvConnection(sender);
            // NetReceiverPtr 是 std::shared_ptr<NetReceiver>，需要返回智能指针
            return new TlsServerConn(sender, _receiver);
        }

    private:
        Common::NetReceiverPtr _receiver;
    };

    class TlsClient:public NetReceiver
    {
    public:
        TlsClient(const Common::NetReceiverPtr &receiver, const SSLContextPtr &ctx);
        void close();
        int send( const unsigned char *data, int dataLen);
        bool isReady();
        void recv(const unsigned char *data, int dataLen);
        Common::NetSenderPtr connect(const Common::NetDriverPtr &driver, const std::string &localHost,int localPort, const std::string &remoteHost, int remotePort);
        int onTlsVerify(int err);

    private:
        void initTlsDriver(const Common::NetDriverPtr &driver);

    private:
        Common::NetReceiverPtr _receiver;
        SSLContextPtr _ctx;
        Common::NetSenderPtr _sender;
    };

    Common::NetSenderPtr connectSSL(const Common::NetDriverPtr& driver, const std::string& localHost, int localPort, const std::string& remoteHost, int remotePort, const Common::NetReceiverPtr& receiver);
    Common::NetSenderPtr connectSSL(const Common::NetDriverPtr& driver, const std::string& localHost, int localPort, const std::string& remoteHost, int remotePort, const SSLContextPtr& ctx, const Common::NetReceiverPtr& receiver);

    Common::NetSenderPtr listenSSL(const Common::NetDriverPtr& driver, const std::string& localHost, int localPort, const SSLContextPtr& ctx, const Common::NetReceiverPtr& receiver);
}

#endif
