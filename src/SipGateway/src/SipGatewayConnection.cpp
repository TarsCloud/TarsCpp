#include <random>

#include "SipGatewayConnection.h"
#include "SipMsg.h"
#include "util/tc_timeprovider.h"
#include "util/tc_uuid_generator.h"
#include "servant/Application.h"

namespace SipGateway
{
    // 匿名命名空间，辅助函数仅在本文件可见
    namespace {
        // 去除字符串前导字符
        inline void trimLeft(std::string& s, const char* chars = " 	")
        {
            size_t start = s.find_first_not_of(chars);
            if (start == std::string::npos) {
                s.clear();
            } else {
                s = s.substr(start);
            }
        }

        // 去除字符串尾部字符
        inline void trimRight(std::string& s, const char* chars = " 	")
        {
            size_t end = s.find_last_not_of(chars);
            if (end != std::string::npos) {
                s = s.substr(0, end + 1);
            } else {
                s.clear();
            }
        }

        // 去除字符串两端字符
        inline void trim(std::string& s, const char* chars = " 	")
        {
            trimLeft(s, chars);
            trimRight(s, chars);
        }

        // 删除前面 n 个字符
        inline void cutHead(std::string& s, size_t n)
        {
            if (n >= s.size()) {
                s.clear();
            } else {
                s.replace(0, n, "");
            }
        }

        // 删除后面 n 个字符
        inline void cutTail(std::string& s, size_t n)
        {
            if (n >= s.size()) {
                s.clear();
            } else {
                s.replace(s.size() - n, n, "");
            }
        }

        // 字符串转整数，带默认值
        inline int toInt(const std::string& s, int defaultValue = -1)
        {
            try {
                return std::stoi(s);
            } catch (...) {
                return defaultValue;
            }
        }

        // 生成随机数 [0, max)
        inline int getRandom(int max)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, max - 1);
            return dis(gen);
        }
    }



    GatewayConnection::GatewayConnection(const SipProxy::SipProxyMsgCoderInterfacePtr & decode,  const Common::NetSenderPtr& sender, ConnectionUsedModulType modulType,const std::string& protocol)
    {
        _sipProxyDecoder = decode;
        _sender = sender;
        constructCommonPart();

        _eUsedModul = modulType;
        _eTransType = TransTypeSip;
        _transProtocol = protocol == SipGateway::SIP_TRANS_PROTOCOL_TCP ? SipGateway::kTransTCP : SipGateway::kTransUDP;
       // _application = application;
       // _eventArrayPtr = _application->createEventArray(decode);

        std::string strRemoteHost,strLocalHost;
        int iRemotePort, iLocalPort = 0;

        if (sender != 0)
        {
            sender->getRemote(strRemoteHost,iRemotePort);
            sender->getLocal(strLocalHost, iLocalPort);
        }
        _remoteHost = strRemoteHost;
        _remotePort = iRemotePort;

        TLOGDEBUG("[GatewayConnection] " << "1 New GatewayConnection() called. Connection Id:" + _strConnectionID + ",randomId:" + _randomId + ",remote:" + _remoteHost + ",:" + std::to_string(_remotePort) << endl);
    }

    GatewayConnection::GatewayConnection(const SipProxy::SipProxyMsgCoderInterfacePtr & decode, ConnectionUsedModulType modulType)
    {
        _sipProxyDecoder = decode;
        _sender = 0;
        constructCommonPart();

        _eUsedModul = modulType;
        _eTransType = TransTypeSip;
       // _application = application;
        //_eventArrayPtr = _application->createEventArray(decode);

        TLOGDEBUG("[GatewayConnection] " << "2 New GatewayConnection() called. Connection Id:" + _strConnectionID + ",randomId:" + _randomId << endl);
    }

    GatewayConnection::GatewayConnection(const SipProxy::SipProxyMsgCoderInterfacePtr & decode,  ConnectionUsedModulType modulType,
        const std::string& protocol, const std::string& localHost, int localPort, const std::string& sbcIp, int sbcPort, const std::string & id)
    {
        _sipProxyDecoder = decode;
        _sender = 0;
        constructCommonPart();

        _eUsedModul = modulType;
        _eTransType = TransTypeSip;
        //_application = application;
        _localHost = localHost;
        _localPort = localPort;
        _remoteHost = sbcIp;
        _remotePort = sbcPort;
        _transProtocol = protocol == SipGateway::SIP_TRANS_PROTOCOL_TCP ? SipGateway::kTransTCP : SipGateway::kTransUDP;
        _strConnectionID = id.empty() ? generateNewConnectionId() : id;
        //_eventArrayPtr = _application->createEventArray(decode);

        TLOGDEBUG("[GatewayConnection] " << "3 New GatewayConnection() called. Connection Id:" + _strConnectionID + ",randomId:" + _randomId + " ,Remote Ip:"
            + _remoteHost + " ,port:" +std::to_string(_remotePort) + " ,local Ip:" + _localHost + " ,port:" + std::to_string(_localPort) << endl);
    }

	GatewayConnection::GatewayConnection(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & decode,  const Common::NetSenderPtr& sender, ConnectionUsedModulType modulType,const std::string& protocol)
    {
        _sipGwDecoder = decode;
        _sender = sender;
        constructCommonPart();

        _eUsedModul = modulType;
        _eTransType = TransTypeSip;
        _transProtocol = protocol == SipGateway::SIP_TRANS_PROTOCOL_TCP ? SipGateway::kTransTCP : SipGateway::kTransUDP;
        //_application = application;
        //_eventArrayPtr = _application->createEventArray(decode);

        std::string strRemoteHost,strLocalHost;
        int iRemotePort, iLocalPort = 0;

        if (sender != 0)
        {
            sender->getRemote(strRemoteHost,iRemotePort);
            sender->getLocal(strLocalHost, iLocalPort);
        }
        _remoteHost = strRemoteHost;
        _remotePort = iRemotePort;

        TLOGDEBUG("[GatewayConnection] " << "1 New GatewayConnection() called. Connection Id:" + _strConnectionID + ",randomId:" + _randomId + ",remote:" + _remoteHost + ",:" + std::to_string(_remotePort) << endl);
    }

    GatewayConnection::GatewayConnection(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & decode, ConnectionUsedModulType modulType)
    {
        _sipGwDecoder = decode;
        _sender = 0;
        constructCommonPart();

        _eUsedModul = modulType;
        _eTransType = TransTypeSip;
        //_application = application;
        //_eventArrayPtr = _application->createEventArray(decode);

        TLOGDEBUG("[GatewayConnection] " << "2 New GatewayConnection() called. Connection Id:" + _strConnectionID + ",randomId:" + _randomId << endl);
    }

    GatewayConnection::GatewayConnection(const SipCloudGateway::CloudSipGatewayMsgCoderInterfacePtr & decode,  ConnectionUsedModulType modulType,
        const std::string& protocol, const std::string& localHost, int localPort, const std::string& sbcIp, int sbcPort, const std::string & id)
    {
        _sipGwDecoder = decode;
        _sender = 0;
        constructCommonPart();

        _eUsedModul = modulType;
        _eTransType = TransTypeSip;
        //_application = application;
        _localHost = localHost;
        _localPort = localPort;
        _remoteHost = sbcIp;
        _remotePort = sbcPort;
        _transProtocol = protocol == SipGateway::SIP_TRANS_PROTOCOL_TCP ? SipGateway::kTransTCP : SipGateway::kTransUDP;
        _strConnectionID = id.empty() ? generateNewConnectionId() : id;
        //_eventArrayPtr = _application->createEventArray(decode);

        TLOGDEBUG("[GatewayConnection] " << "3 New GatewayConnection() called. Connection Id:" + _strConnectionID + ",randomId:" + _randomId + " ,Remote Ip:"
            + _remoteHost + " ,port:" +std::to_string(_remotePort) + " ,local Ip:" + _localHost + " ,port:" + std::to_string(_localPort) << endl);
    }

    GatewayConnection::GatewayConnection(const SipRtpGateway::RtpGatewayMsgSenderInterfacePtr & decode, const Common::NetSenderPtr& sender, ConnectionUsedModulType modulType)
    {
        _sipProxyDecoder = 0;
		_sipGwDecoder = 0;
		_rtpGatewayDecoder = decode;
        _sender = sender;
        constructCommonPart();

        _eUsedModul = RtpGatewayModul;
        _eTransType = TransTypeAudio;

        TLOGDEBUG("[GatewayConnection] " << "4 New GatewayConnection() called.  random Connection Id :" + _strConnectionID + ",randomId:" + _randomId << endl);
    }

    GatewayConnection::GatewayConnection(const SipRtpGateway::RtpGatewayMsgSenderInterfacePtr & decode, TransType transType)
    {
        _sipProxyDecoder = 0;
		_sipGwDecoder = 0;
        _rtpGatewayDecoder = decode;
        _sender = 0;
        constructCommonPart();

        _eUsedModul = RtpGatewayModul;
        _eTransType = transType;

        TLOGDEBUG("[GatewayConnection] " << "5 New GatewayConnection() called.  random Connection Id : " + _strConnectionID + ",randomId:" + _randomId << endl);
    }

    void GatewayConnection::constructCommonPart()
    {
        _createTime  = TNOWMS;
        _eraseTime =  TNOWMS;
        _strConnectionID = generateNewConnectionId();
        _randomId = TC_UUIDGenerator::getInstance()->genID();

        ++_totalNum;
        ++_newNum;

        _bToSBC = false;
        _bOnClosed = false;
        _remotePort = 0;
        _transProtocol = SipGateway::kTransUDP;
        _msgBuffer = "";
        _localPort = 0;
        _callProxying = false;
        //_application = 0;
        _lastActiveTicks = TNOWMS;
    }

    GatewayConnection::~GatewayConnection()
    {
        //_totalNum --;
        --_totalNum;
        ++_deleteNum;

        _sender = 0;
        _sipProxyDecoder = 0;
        _rtpGatewayDecoder = 0;
		_sipGwDecoder = 0;

        TLOGDEBUG("[GatewayConnection] " << "~GatewayConnection() called.random Connection Id : " + _strConnectionID + ",randomId:" + _randomId << endl);
    }

    //copy construction
    GatewayConnection::GatewayConnection(const GatewayConnection & connect)
    {
        _sender = connect._sender;
        _createTime  = TNOWMS;
        ++_totalNum;
        ++_newNum;
    }

    //assignment
    GatewayConnection & GatewayConnection::operator = (const GatewayConnection & connect)
    {
        if (this == &connect)
            return *this;

        _sender = connect._sender;
        _strConnectionID = generateNewConnectionId();
        return *this;
    }

    bool GatewayConnection::setSender(const Common::NetSenderPtr& sender)
    {
        std::lock_guard<std::mutex> temLock(_mutexSender);
        _sender = sender;
        return true;
    }

    Common::NetSenderPtr GatewayConnection::getSender()
    {
        std::lock_guard<std::mutex> temLock(_mutexSender);
        return _sender;
    }

    bool GatewayConnection::checkSendValid()
    {
        std::lock_guard<std::mutex> temLock(_mutexSender);
        return (_sender == 0) ? false : true;
    }

    bool GatewayConnection::send(const unsigned char *data,int dataLen)
    {
        std::lock_guard<std::mutex> temLock(_mutexSender);
        if (_sender != 0)
        {
            int sentTimes = 0;
            int sendLenSum = 0;
            bool result = true;

            do
            {
                int sendLen = _sender->send(data + sendLenSum, dataLen - sendLenSum);
                if (sendLen < 0)
                {
                    result = false;
                    break;
                }
                sendLenSum += sendLen;
                sentTimes++;
            } while ((sendLenSum < dataLen) && (sentTimes <= 5));

            _lastActiveTicks = TC_TimeProvider::getInstance()->getNowMs();

            std::string strRemoteHost,strLocalHost;
            int iRemotePort, iLocalPort = 0;
            _sender->getRemote(strRemoteHost,iRemotePort);
            _sender->getLocal(strLocalHost, iLocalPort);
            std::string protoc = _transProtocol == SipGateway::kTransTCP ? SipGateway::SIP_TRANS_PROTOCOL_TCP : SipGateway::SIP_TRANS_PROTOCOL_UDP;

            if (_eUsedModul == SipProxyModul || _eUsedModul == SipGatewayEntryModule ||  _eUsedModul == SipMessageGatewayModule || _eUsedModul == AcdGatewayModule || _eUsedModul == MgcfSipGatewayModul  || _eUsedModul == CloudSipGatewayModul)
            {
                TLOGINFO("[GatewayConnection] " << "======= [" + _randomId + "] DATA SENT," + std::to_string(sentTimes) + ",remote host :"+ strRemoteHost + " ,remote port:" +
                    std::to_string(iRemotePort) + " ,local ip:" + strLocalHost + " ,local port:" + std::to_string(iLocalPort) +
                    " [dataLen:" + std::to_string(dataLen) + ",sent:" + std::to_string(sendLenSum) + " Bytes " + std::string(protoc) + "] ========\n" + std::string((char*)data) + "\n-------- End -------\n" << endl);
            }
            if (!result)
                TLOGWARN("[GatewayConnection] " << "======= Send failed\n" << endl);
            return result;
        }
        else
        {
            if (_eUsedModul == SipProxyModul || _eUsedModul == SipGatewayEntryModule || _eUsedModul == SipMessageGatewayModule || _eUsedModul == CloudSipGatewayModul || _eUsedModul == MgcfSipGatewayModul)
            {
                std::string msg((char*)data, strlen((char*)data));
                TLOGERROR("[GatewayConnection] " << "GatewayConnection::send() called, _send is NULL. _strConnectionID:" + _strConnectionID + " ,msg:\n" + msg << endl);
            }
            return false;
        }
    }

    void GatewayConnection::recv(const unsigned char *data,int dataLen)
    {
        std::string dataStr = std::string((const char *)data, dataLen);
        std::string strRemoteHost,strLocalHost;
        int iRemotePort, iLocalPort = 0;
        {
            std::lock_guard<std::mutex> temLock(_mutexSender);
            if (_sender != 0)
            {
                _sender->getRemote(strRemoteHost,iRemotePort);
                _sender->getLocal(strLocalHost, iLocalPort);
            }
            else
            {
                TLOGERROR("[GatewayConnection] " << std::string(__FUNCTION__) + "(), _send is NULL,msg size:" + std::to_string(dataLen) << endl);
                return;
            }
            _lastActiveTicks = TC_TimeProvider::getInstance()->getNowMs();
        }

        if (_eUsedModul == SipGatewayModul)
        {
        }
        else if (_eUsedModul == SipProxyModul || _eUsedModul == SipGatewayEntryModule || _eUsedModul == SipMessageGatewayModule || _eUsedModul == AcdGatewayModule || _eUsedModul == MgcfSipGatewayModul|| _eUsedModul == CloudSipGatewayModul)
        {
            if (_sipProxyDecoder)
            {
                if (false == _sipProxyDecoder->checkIpInWhitelist(strRemoteHost))
                {
                    TLOGWARN("[GatewayConnection] " << std::string(__FUNCTION__) + "(), ip " + strRemoteHost + " not in whitelist! msg size:" + std::to_string(dataLen) << endl);
                    return;
                }
            }

 			if (_sipGwDecoder)
            {
                if (false == _sipGwDecoder->checkIpInWhitelist(strRemoteHost))
                {
                    TLOGWARN("[GatewayConnection] " << std::string(__FUNCTION__) + "(), ip " + strRemoteHost + " not in whitelist! msg size:" + std::to_string(dataLen) << endl);
                    return;
                }
            }

            std::string protoc = _transProtocol == SipGateway::kTransTCP ? SipGateway::SIP_TRANS_PROTOCOL_TCP : SipGateway::SIP_TRANS_PROTOCOL_UDP;
            TLOGINFO("[GatewayConnection] " << "======= [" + _randomId + "] DATA RECVed,From Remote Ip:"+ strRemoteHost +" port: "+ std::to_string(iRemotePort) +" local ip:" + strLocalHost + " ,local port:" + std::to_string(iLocalPort) +" [ " +
                std::to_string(dataLen) + " Bytes " + std::string(protoc) + "] ========\n" + dataStr + "\n-------- End -------\n" << endl);

            if (_transProtocol == SipGateway::kTransTCP)
            {
                std::lock_guard<std::mutex> lk(_mutexMsgBuffer);

                std::vector<std::string> validMsgs;
                SipGateway::GatewayConnection::unpackTcpStreamByBob(_msgBuffer, dataStr, validMsgs);
                for (unsigned int i = 0; i < validMsgs.size(); i++)
                {
                    const std::string & msg = validMsgs.at(i);
                    pushEvent(msg, strRemoteHost, iRemotePort);
                }
            }
            else
            {
                pushEvent(dataStr, strRemoteHost, iRemotePort);
            }
        }
        else if (_eUsedModul == RtpGatewayModul)
        {
            bool bAudioMedia = _eTransType == TransTypeAudio ? true : false;
            _rtpGatewayDecoder->receivedSBCRtpMsg(dataStr, dataStr.size(), bAudioMedia, this);
        }
		else
        {
            TLOGWARN("[GatewayConnection] " << std::string(__FUNCTION__) + "(), Other Moduls. dataLen:" + std::to_string(dataLen) << endl);
        }
    }

    void GatewayConnection::onConnClose()
    {
        {
            std::lock_guard<std::mutex> temLock(_mutexSender);
            if (_sender != 0)
            {
                _bOnClosed = true;
                _sender->close();
                _sender = 0;
            }
        }

        if (_bToSBC)
        {
            TLOGWARN("[GatewayConnection] " << std::string(__FUNCTION__) + "() called. To SBC Connection be Closed! local ip:" + _localHost + " ,port:"
                + std::to_string(_localPort) + " ,remote ip:" + _remoteHost + " ,port:" + std::to_string(_remotePort)
                + ",errno:" + std::to_string(errno) + ",randomId:" + _randomId + "," + std::to_string(int(getRandom(10000))) << endl);

            if (_sipProxyDecoder && _transProtocol == SipGateway::kTransTCP)
            {
                std::lock_guard<std::mutex> lock1(_mutexCallProxying);
                if (_callProxying)
                    return;

                _callProxying = true;
                _sipProxyDecoder->toSBCConnectionClosed(_strConnectionID, _localHost, _localPort, _remoteHost, _remotePort, SipGateway::SIP_TRANS_PROTOCOL_TCP);
                _callProxying = false;
            }

			if (_sipGwDecoder && _transProtocol == SipGateway::kTransTCP)
			{
				std::lock_guard<std::mutex> lock1(_mutexCallProxying);
				if (_callProxying)
					return;

				_callProxying = true;
				_sipGwDecoder->toCloudConnectionClosed(_strConnectionID, _localHost, _localPort, _remoteHost, _remotePort, SipGateway::SIP_TRANS_PROTOCOL_TCP);
				_callProxying = false;
			}
        }

        TLOGDEBUG("[GatewayConnection] " << std::string(__FUNCTION__) + "() called. Connection Id : " + _strConnectionID+ ",randomId:" + _randomId << endl);
    }

    void GatewayConnection::onConnReady()
    {
    }

    std::atomic<int> GatewayConnection::_totalNum(0);
    std::atomic<int> GatewayConnection::_newNum(0);
    std::atomic<int> GatewayConnection::_deleteNum(0);

    std::string GatewayConnection::generateNewConnectionId()
    {
        std::string strTem = std::to_string(SipMsgCommon::CreateRandInt());
        return  TC_UUIDGenerator::getInstance()->genID()  + "+Num+" + strTem + TC_UUIDGenerator::getInstance()->genID();
    }

    void GatewayConnection::setEraseTime(std::string erasePath )
    {
        _eraseTime = TNOWMS;
        _strErasePath = erasePath;
        TLOGDEBUG("[GatewayConnection] " << "setEraseTime() called.  random Connection Id : " + _strConnectionID + ",randomId:" + _randomId << endl);
    }

    int64_t GatewayConnection::getCreateTime()
    {
        return _createTime;
    }

    int64_t GatewayConnection::getEraseTime()
    {
        return _eraseTime;
    }

    std::string GatewayConnection::getErasePath()
    {
        return _strErasePath;
    }

    void GatewayConnection::setConnectionId(const std::string & id)
    {
        _strConnectionID = id;
    }

    std::string GatewayConnection::getConnectionId()
    {
        return _strConnectionID;
    }

    void GatewayConnection::setToSBCConnection(bool bToSBC)
    {
        _bToSBC = bToSBC;
    }

    bool GatewayConnection::getToSBCConnection()
    {
        return _bToSBC;
    }

    void GatewayConnection::setConnectionInfos(const std::string & localHost, int localPort, const std::string & remoteHost, int port, const std::string & transProtocol)
    {
        _localHost = localHost;
        _localPort = localPort;
        _remoteHost = remoteHost;
        _remotePort = port;
        _transProtocol = transProtocol == SipGateway::SIP_TRANS_PROTOCOL_TCP ? SipGateway::kTransTCP : SipGateway::kTransUDP;
    }

    void GatewayConnection::clearUp()
    {
        std::lock_guard<std::mutex> temLock(_mutexSender);

        if (_sender != 0)
           _sender->close();

        _sender = 0;
        _bOnClosed = true;
        TLOGDEBUG("[GatewayConnection] " << std::string(__FUNCTION__) + "() called.Connection Id : " + _strConnectionID  + ",randomId:" + _randomId << endl);
    }

    SipGateway::TransProtocolType GatewayConnection::getTransProtocol()
    {
        return _transProtocol;
    }

    std::string GatewayConnection::getSBCIp()
    {
        return _remoteHost;
    }

    std::string GatewayConnection::getRemoteHost()
    {
        std::string strRemoteHost;
        int iRemotePort = 0;
        {
            std::lock_guard<std::mutex> temLock(_mutexSender);
            if (_sender != 0)
            {
                _sender->getRemote(strRemoteHost,iRemotePort);
            }
            else
            {
                TLOGERROR("[GatewayConnection] " << std::string(__FUNCTION__) + "() called, _send is NULL." << endl);
                return "";
            }
        }
        return strRemoteHost;
    }

    int GatewayConnection::getLocalPort()
    {
        return _localPort;
    }

    void GatewayConnection::setSipDialogId(const std::string & sipDialogId)
    {
        _strSipDialogId = sipDialogId;
    }
    std::string GatewayConnection:: getSipDialogId()
    {
        return _strSipDialogId;
    }

    ConnectionUsedModulType GatewayConnection::getUsedModule()
    {
        return _eUsedModul;
    }

    int GatewayConnection::getContentLength(std::string &data)
    {
        const char *CRLF = "\r\n";
        int len = -1, poshead = 0, postail = -1;

        while ((postail = data.find(CRLF, poshead)) >= 0)
        {
            int linelen = postail - poshead;
            if (linelen == 0)
            {
                poshead = postail + 2;
                continue;
            }

            std::string line = data.substr(poshead, linelen);
            poshead = postail + 2;

            if (linelen >= 16 && ((line[0] == 'C' || line[0] == 'c') && (line[1] == 'O' || line[1] == 'o') && (line[2] == 'N' || line[2] == 'n') && (line[3] == 'T' || line[3] == 't') && (line[4] == 'E' || line[4] == 'e') && (line[5] == 'N' || line[5] == 'n') && (line[6] == 'T' || line[6] == 't') && line[7] == '-' && (line[8] == 'L' || line[8] == 'l') && (line[9] == 'E' || line[9] == 'e') && (line[10] == 'N' || line[10] == 'n') && (line[11] == 'G' || line[11] == 'g') && (line[12] == 'T' || line[12] == 't') && (line[13] == 'H' || line[13] == 'h')))
            {
                cutHead(line, 14);
            }
            else if (linelen >= 3 && (line[0] == 'L' || line[0] == 'l'))
            {
                cutHead(line, 1);
            }
            else
                continue;

            while (line.size() && (line[0] == ' ' || line[0] == '\t'))
                cutHead(line, 1);
            if (!line.size())
                continue;

            if (line.size() && line[0] == ':')
                cutHead(line, 1);
            if (!line.size())
                continue;

            while (line.size() && (line[0] == ' ' || line[0] == '\t'))
                cutHead(line, 1);
            if (!line.size())
                continue;

            while (line.size() && (line[line.size() - 1] == ' ' || line[line.size() - 1] == '\t'))
                cutTail(line, 1);
            if (!line.size())
                continue;

            len = toInt(line, -1);
            if (len == -1)
                continue;

            break;
        }

        return len;
    }

    void GatewayConnection::unpackTcpStreamByBob(std::string &remainMsg, const std::string &oriMsg, std::vector<std::string> &validMsgs)
    {
        const char *DOUBLE_CRLF = "\r\n\r\n";
        int poshead = 0, postail = -1;

        remainMsg += oriMsg;
        while ((postail = remainMsg.find(DOUBLE_CRLF, poshead)) >= 0)
        {
            if (postail == poshead)
            {
                poshead = postail + 4;
                continue;
            }

            if (remainMsg[poshead] == '\r' && remainMsg[poshead + 1] == '\n')
                poshead += 2;

            int msglen = (postail - poshead) + 4;
            std::string msg = remainMsg.substr(poshead, msglen);

            int len = getContentLength(msg);
            if (len > 0)
            {
                if (msglen + len > remainMsg.size() - poshead)
                    break;
                validMsgs.push_back(remainMsg.substr(poshead, msglen + len));
            }
            else if (len == 0)
                validMsgs.push_back(msg);
            else
                len = 0;

            poshead += msglen + len;
        }

        if (poshead == remainMsg.size())
            remainMsg.clear();
        else if (poshead > 0)
            remainMsg = remainMsg.substr(poshead);

        if (remainMsg.size() > 10 * 1024 * 1024)
            remainMsg.clear();
    }

    void GatewayConnection::pushEvent(const std::string & msg, const std::string & remoteIp, int iRemotePort)
    {
        // if (_eventArrayPtr)
        // {
        //     MsgEventPtr msgEvent = std::make_shared<MsgEvent>(msg, _strConnectionID, remoteIp, iRemotePort, this);
        //     _eventArrayPtr->pushEvent(msgEvent, false);
        // }
    }

    int64_t GatewayConnection::getActiveTicks()
    {
        return _lastActiveTicks;
    }
};
