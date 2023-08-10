#include "util/tc_socket_async.h"
#include "util/tc_timeprovider.h"
#include <cstring>

namespace tars
{

#define H64(x) (((uint64_t)x) << 32)

void TC_SocketAsync::start()
{
    TC_SocketAsyncCore::getInstance()->start();
}

void TC_SocketAsync::terminate()
{
    TC_SocketAsyncCore::getInstance()->terminate();
}

void TC_SocketAsync::setRetryInterval(int64_t millsecond)
{
    TC_SocketAsyncCore::getInstance()->setRetryInterval(millsecond);
}

void TC_SocketAsync::release(TC_SocketAsyncPtr &ptr)
{
    TC_SocketAsyncCore::getInstance()->release(ptr);
}

////////////////////////////////////////////////////////////////////////////////////////

TC_SocketAsyncPtr TC_SocketAsync::createSocketAsync(const TC_Endpoint &ep, const RequestCallbackPtr &callbackPtr, const TC_NetWorkBuffer::protocol_functor &pf)
{
    TC_SocketAsyncPtr ptr = new TC_SocketAsync(ep, callbackPtr, pf);

    TC_SocketAsyncCore::getInstance()->addNotify(ptr);

    return ptr;
}

TC_SocketAsync::TC_SocketAsync(const TC_Endpoint &ep, const RequestCallbackPtr &callbackPtr, const TC_NetWorkBuffer::protocol_functor &pf) 
: _ep(ep)
, _callbackPtr(callbackPtr)
, _pf(pf)
, _bClear(true)
, _iMaxQueueSize(3000000)
, _uniqId(0)
, _status(EM_NORMAL)
{
    TC_SocketAsyncCore::getInstance()->start();

    _core = TC_SocketAsyncCore::getInstance();

    resetTrans(ep);
}

TC_SocketAsync::~TC_SocketAsync()
{
}

void TC_SocketAsync::resetTrans(const TC_Endpoint & ep)
{
    _ep = ep;
#if TAF_SSL
    if (_ep.isSSL())
    {
        _trans.reset(new TC_SSLTransceiver(_core->getEpoller(), _ep));
    }
    else if (_ep.isTcp())
    {
        _trans.reset(new TC_TCPTransceiver(_core->getEpoller(), _ep));
    }
    else
    {
        _trans.reset(new TC_UDPTransceiver(_core->getEpoller(), _ep));
    }
#else
    if (ep.isUdp())
    {
        _trans.reset(new TC_UDPTransceiver(_core->getEpoller(), _ep));
    }
    else
    {
        _trans.reset(new TC_TCPTransceiver(_core->getEpoller(), _ep));
    }
#endif
    
    _trans->setConnTimeout(_core->getConnectTimeout());
    
    _trans->initializeClient(std::bind(&TC_SocketAsync::onCreateCallback, this, std::placeholders::_1),
                             std::bind(&TC_SocketAsync::onCloseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                             std::bind(&TC_SocketAsync::onConnectCallback, this, std::placeholders::_1),
                             std::bind(&TC_SocketAsync::onRequestCallback, this, std::placeholders::_1),
                             std::bind(&TC_SocketAsync::onParserCallback, this, std::placeholders::_1, std::placeholders::_2),
                             std::bind(&TC_SocketAsync::onOpensslCallback, this, std::placeholders::_1));
}

shared_ptr<TC_ProxyInfo> TC_SocketAsync::onCreateCallback(TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG << endl;

	shared_ptr<TC_Epoller::EpollInfo> epollInfo = trans->getEpollInfo();

	map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

	callbacks[EPOLLIN] = std::bind(&TC_SocketAsync::handleInputImp, this, std::placeholders::_1);
	callbacks[EPOLLOUT] = std::bind(&TC_SocketAsync::handleOutputImp, this, std::placeholders::_1);
	callbacks[EPOLLERR] = std::bind(&TC_SocketAsync::handleCloseImp, this, std::placeholders::_1);

	epollInfo->registerCallback(callbacks, EPOLLIN|EPOLLOUT);

    return NULL;
}

std::shared_ptr<TC_OpenSSL> TC_SocketAsync::onOpensslCallback(TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG << endl;
#if TAF_SSL

	if(trans->isSSL()) {
		if (!_ctx) {
			_ctx = TC_OpenSSL::newCtx("", "", "", false, "");
		}
		return TC_OpenSSL::newSSL(_ctx);
	}
	return NULL;
#else
	return NULL;
#endif
}

void TC_SocketAsync::onCloseCallback(TC_Transceiver* trans, TC_Transceiver::CloseReason reason, const string &err)
{
//	LOG_CONSOLE_DEBUG << endl;

    if (status() != EM_NORMAL)
    {
        //暂停状态, 清除数据, 并且不再重连
        clearSendReqBuffer();

        _core->delRetry(getUniqId());
    }
    else
    {
        TC_SocketAsyncPtr ptr(this);

        _core->addRetry(ptr);
        //连接关闭, 清除掉buffer
        if (_bClear)
        {
            clearSendReqBuffer();
        }
    }

    if (_callbackPtr)
    {
        try {_callbackPtr->onClose(); } catch(...) {}
        try {_callbackPtr->onFailed(reason, err); } catch(...) {}
    }
}

void TC_SocketAsync::onConnectCallback(TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG << endl;
    TC_SocketAsyncPtr ptr(this);

    _core->addConnection(ptr);

    if (_callbackPtr)
    {
        try {_callbackPtr->onConnected(); } catch(...) {}
    }
}

void TC_SocketAsync::onRequestCallback(TC_Transceiver* trans)
{
    for(;;)
    {
        decltype(_messages)::iterator it;

        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_messages.empty())
            {
                return;
            }

            it = _messages.begin();
        }

        if (_callbackPtr && !it->second)
        {
            _callbackPtr->onBeforeRequest(it->first);
            it->second = true;
        }

//	    LOG_CONSOLE_DEBUG << it->first->length() << endl;

	    TC_Transceiver::ReturnStatus iRet = _trans->sendRequest(it->first);

        if (iRet == TC_Transceiver::eRetError)
        {
            return;
        }

        if(iRet != TC_Transceiver::eRetNotSend)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            _messages.erase(it);
        }

        //数据还不能发送 or 发送buffer已经满了 直接返回, 暂时不要再发送了!
        if (iRet == TC_Transceiver::eRetNotSend || iRet == TC_Transceiver::eRetFull)
        {
            return;
        }
    }
}

TC_NetWorkBuffer::PACKET_TYPE TC_SocketAsync::onParserCallback(TC_NetWorkBuffer& buff, TC_Transceiver* trans)
{
//	LOG_CONSOLE_DEBUG << "buff length:" << buff.getBufferLength() << endl;

    vector<char> out;

    TC_NetWorkBuffer::PACKET_TYPE ret = _pf(buff, out);

    if (ret == TC_NetWorkBuffer:: PACKET_FULL)
    {
        if (_callbackPtr)
        {
            try { _callbackPtr->onSucc(out); } catch (...) { }
        }
    }
//	LOG_CONSOLE_DEBUG << " ret:" << ret << endl;

    return ret;
}

bool TC_SocketAsync::handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
//	LOG_CONSOLE_DEBUG << data->fd() << endl;

    _trans->close();

    return false;
}

bool TC_SocketAsync::handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
//	if(!_trans) {
//		LOG_CONSOLE_DEBUG << data->fd() << endl;
//	}
    try
    {
        _trans->doResponse();
    }
    catch(const std::exception& e)
    {
//    	LOG_CONSOLE_DEBUG << e.what() << endl;
        return false;
    }

//	LOG_CONSOLE_DEBUG << data->fd() << " succ" << endl;

    return true;
}

bool TC_SocketAsync::handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
//	LOG_CONSOLE_DEBUG << data->fd() << endl;

    try
    {
        _trans->doRequest();
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;
}

void TC_SocketAsync::clearSendReqBuffer()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _messages.clear();
}

size_t TC_SocketAsync::getSendReqBufferSize() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _messages.size();
}

bool TC_SocketAsync::emptySendReqBufferSize() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _messages.empty();
}

void TC_SocketAsync::addSendReqBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer> &reqBuffer, bool header)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (header)
    {
        _messages.push_front(std::make_pair(reqBuffer, false));
    }
    else
    {
        _messages.push_back(std::make_pair(reqBuffer, false));
    }
}

void TC_SocketAsync::setEndpoint(const TC_Endpoint &ep)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _ep = ep;
    if (_trans){
        _trans->setEndpoint(ep);
    }
}

string TC_SocketAsync::getHost() const
{
    std::lock_guard<std::mutex> lock(_mutex);

    return _ep.getHost();
}

uint32_t TC_SocketAsync::getPort() const
{
    std::lock_guard<std::mutex> lock(_mutex);

    return _ep.getPort();
}

string TC_SocketAsync::getError(const char* sDefault) const
{
    int ret = TC_Exception::getSystemCode();
    if(ret!= 0)
    {
        return TC_Exception::parseError(ret);
    }

    return sDefault;
}

void TC_SocketAsync::connect()
{
    onNotify(TC_SocketAsyncCore::NE_CONNECT);
}

void TC_SocketAsync::reConnect()
{
    onNotify(TC_SocketAsyncCore::NE_RECONNECT);
}

void TC_SocketAsync::setCtx(const std::string& cafile, const std::string& certfile, const std::string& keyfile, bool verifyClient, const string &ciphers)
{
#if TAF_SSL
	_ctx = TC_OpenSSL::newCtx(cafile, certfile, keyfile, verifyClient, ciphers);
#endif
}

void TC_SocketAsync::setUdpRecvBuffer(size_t nSize)
{
    _trans->setUdpRecvBuffer(nSize);
}

void TC_SocketAsync::setUdpSendBuffer(size_t nSize)
{
    _trans->setUdpSendBuffer(nSize);
}

void TC_SocketAsync::sendRequest(const string &sBuffer, bool header)
{
    if (sBuffer.empty())
        return;

    if (status() != EM_NORMAL)
    {
        return;
    }

    TC_SocketAsyncPtr ptr(this);

    shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

    buff->addBuffer(sBuffer);

    addSendReqBuffer(buff, header);

    onNotify(TC_SocketAsyncCore::NE_CONNECT);
}

void TC_SocketAsync::sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> &buff, bool header)
{
    if (buff && buff->empty())
        return;

    if (status() != EM_NORMAL)
    {
        return;
    }

    TC_SocketAsyncPtr ptr(this);

    addSendReqBuffer(buff, header);

    onNotify(TC_SocketAsyncCore::NE_CONNECT);
}

void TC_SocketAsync::processNotify()
{
    if(!_trans->hasConnected() && !_trans->isConnecting())
    {
        _trans->connect();
    }
    else if(_trans->hasConnected())
    {
        _trans->doRequest();
    }
}

void TC_SocketAsync::onNotify(int notifyEvent)
{
    _core->notify(notifyEvent, _uniqId);
}

void TC_SocketAsync::pause()
{
    _status = EM_PAUSE;

    _core->notify(TC_SocketAsyncCore::NE_PAUSE, _uniqId);
}

void TC_SocketAsync::resume()
{
    _status = EM_NORMAL;

    _core->notify(TC_SocketAsyncCore::NE_RESUME, _uniqId);
}

////////////////////////////////////////////////////////////////////////////////////
///

TC_SocketAsyncCore::TC_SocketAsyncCore()
: _thread(NULL), _terminate(false), _retryInterval(1000)
{
    _conn = new Connection();

    _epoller.create(10240);
}

TC_SocketAsyncCore::~TC_SocketAsyncCore()
{
    terminate();
//
//    if (_isStarted)
//    {
//        _notify.release();
//    }

    _epoller.terminate();
    _epoller.close();
}

void TC_SocketAsyncCore::start()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_thread != NULL)
        return;

    _terminate = false;

    if (_thread == NULL)
    {
        _thread = new std::thread(&TC_SocketAsyncCore::run, this);
    }
}

void TC_SocketAsyncCore::terminate()
{
    if (!_terminate)
    {
        _terminate = true;

        _epoller.terminate();
    }

    if (_thread)
    {
        if (_thread->get_id() == this_thread::get_id())
        {
            _thread->detach();
        }
        else
        {
            _thread->join();
        }
        delete _thread;
        _thread = NULL;
    }
}

void TC_SocketAsyncCore::release(TC_SocketAsyncPtr &ptr)
{
    std::lock_guard<std::recursive_mutex> lock(_release_mutex);

	if (ptr)
	{
		ptr->trans()->close();

		_conn->release(ptr);
	}
}

void TC_SocketAsyncCore::addRetry(TC_SocketAsyncPtr & ptr)
{
    _conn->addRetry(ptr);
}

void TC_SocketAsyncCore::delRetry(uint32_t uniqId)
{
    _conn->delRetry(uniqId);
}

void TC_SocketAsyncCore::addConnection(TC_SocketAsyncPtr & ptr)
{
    _conn->add(ptr);
}

void TC_SocketAsyncCore::addNotify(TC_SocketAsyncPtr & ptr)
{
    uint32_t uniqId = _conn->getUniqId();

    ptr->setUniqId(uniqId);

    _conn->add(ptr);
}

//void TC_SocketAsyncCore::delNotify(TC_SocketAsyncPtr & ptr)
//{
//    if (ptr)
//    {
//    	this->_epoller.del(ptr->trans()->fd(), 0);
//    	LOG_CONSOLE_DEBUG << "fd:" << ptr->trans()->fd() << endl;
//
//        _conn->release(ptr);
//    }
//}

void TC_SocketAsyncCore::notify(int notifyEvent, uint32_t uniqId)
{
    TC_SocketAsyncPtr ptr = _conn->get(uniqId);
    if (ptr)
    {
        {
            uint64_t data = H64(notifyEvent) | uniqId;

            std::lock_guard<std::mutex> lock(_mutex);

            //最后一个元素和当前data一样, 就不需要在放进去了
            if(_events.empty() || *_events.rbegin() != data)
            {
                _events.push_back(data);
            }
        }
        //发送消息, 唤醒网络线程
        _epoller.notify();
    }
}

void TC_SocketAsyncCore::process(int notifyEvent, uint32_t uniqId)
{
    TC_SocketAsyncPtr ptr = _conn->get(uniqId);
    if (!ptr)
    {
        return;
    }

    if (notifyEvent == NE_CONNECT || notifyEvent == NE_RESUME)
    {
        ptr->processNotify();
    }
    else if (notifyEvent == NE_RECONNECT)
    {
        ptr->trans()->close();
        ptr->processNotify();
    }
    else if (notifyEvent == NE_PAUSE)
    {
        ptr->trans()->close();
    }
    else
    {
        assert(false);
    }
}

void TC_SocketAsyncCore::run()
{
    _epoller.postRepeated(_retryInterval, false, [&](){
        _conn->retryConnect();
    });

    _epoller.idle([&]{
        deque<uint64_t> events;

        {
            std::lock_guard<std::mutex> lock(_mutex);

            _events.swap(events);
        }

        for(auto data : events)
        {
            process(data >> 32, (uint32_t)data);
        }
    });

    _epoller.loop();
}

}

