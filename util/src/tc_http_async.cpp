#include "util/tc_http_async.h"
#include "util/tc_common.h"
#include "util/tc_timeprovider.h"

namespace tars
{

	TC_HttpAsync::AsyncRequest::~AsyncRequest()
	{
	}

	void TC_HttpAsync::AsyncRequest::initialize(TC_Epoller *epoller, const TC_Endpoint &ep, TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr)
	{
		_callbackPtr = callbackPtr;

#if TARS_SSL
		if(ep.isSSL())
		{
			_trans.reset(new TC_SSLTransceiver(epoller, ep));
		}
		else {
			_trans.reset(new TC_TCPTransceiver(epoller, ep));
		}
#else
		_trans.reset(new TC_TCPTransceiver(epoller, ep));
#endif
		_buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

		stHttpRequest.encode(_buff);

		_trans->initializeClient(std::bind(&AsyncRequest::onCreateCallback, this, std::placeholders::_1),
				std::bind(&AsyncRequest::onCloseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&AsyncRequest::onConnectCallback, this, std::placeholders::_1),
				std::bind(&AsyncRequest::onRequestCallback, this, std::placeholders::_1),
				std::bind(&AsyncRequest::onParserCallback, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&AsyncRequest::onOpensslCallback, this, std::placeholders::_1)
		);
	}


	shared_ptr<TC_ProxyInfo> TC_HttpAsync::AsyncRequest::onCreateCallback(TC_Transceiver* trans)
	{
		_pHttpAsync->addFd(this);

		return NULL;
	}

	std::shared_ptr<TC_OpenSSL> TC_HttpAsync::AsyncRequest::onOpensslCallback(TC_Transceiver* trans)
	{
#if TARS_SSL
		if(trans->isSSL()) {
			if (!_pHttpAsync->getCtx()) {
				_ctx = TC_OpenSSL::newCtx("", "", "", false, "");
			}else
			{
				_ctx = _pHttpAsync->getCtx();
			}
			return TC_OpenSSL::newSSL(_ctx);
		}
		return NULL;
#else
		return NULL;
#endif
	}

	void TC_HttpAsync::AsyncRequest::onCloseCallback(TC_Transceiver* trans, TC_Transceiver::CloseReason reason, const string &err)
	{
		if(reason == TC_Transceiver::CloseReason::CR_PEER_CLOSE)
		{
			//服务器端主动关闭的, 对于http而言, 如果没有content-length的情况下, 则认为是成功
			if(_callbackPtr)
			{
				if (_stHttpResp.isHeadComplete() && !_stHttpResp.hasHeader("Content-Length") && !_stHttpResp.isChunked())
					try
					{ _callbackPtr->onSucc(_stHttpResp); }
					catch (...)
					{}
			}
		}

		try { if (_callbackPtr) _callbackPtr->onClose(); } catch (...) {}

		if (_pHttpAsync)
		{
			_pHttpAsync->erase(_iUniqId);
		}
	}

	void TC_HttpAsync::AsyncRequest::onConnectCallback(TC_Transceiver* trans)
	{
//	LOG_CONSOLE_DEBUG << endl;
	}

	void TC_HttpAsync::AsyncRequest::onRequestCallback(TC_Transceiver* trans)
	{
//	LOG_CONSOLE_DEBUG << _buff->length() << endl;

		if(!_buff->empty())
		{
			auto iRet = trans->sendRequest(_buff);

			if (iRet == TC_Transceiver::eRetError)
			{
				doException(RequestCallback::Failed_Request, getError("request error"));
			}
		}
	}

	TC_NetWorkBuffer::PACKET_TYPE TC_HttpAsync::AsyncRequest::onParserCallback(TC_NetWorkBuffer& buff, TC_Transceiver* trans)
	{
		if(buff.empty())
		{
			return TC_NetWorkBuffer::PACKET_LESS;
		}

//	LOG_CONSOLE_DEBUG << buff.getBuffer()->buffer() << endl;
		//增量decode
		bool ret    = _stHttpResp.incrementDecode(buff);

//	LOG_CONSOLE_DEBUG << _stHttpResp.getContent() << endl;

		//有头部数据了
		if (_callbackPtr && !_stHttpResp.getHeaders().empty())
		{
			bool bContinue = _callbackPtr->onContinue(_stHttpResp);
			if (!bContinue)
			{
				doException(RequestCallback::Failed_Interrupt, getError("receive interrupt"));
				return TC_NetWorkBuffer::PACKET_ERR;
			}
		}

		//数据接收完毕
		if (ret)
		{
			try { if (_callbackPtr) _callbackPtr->onSucc(_stHttpResp); } catch (...) { }

			return TC_NetWorkBuffer::PACKET_FULL_CLOSE;
		}

		return TC_NetWorkBuffer::PACKET_LESS;
	}

	void TC_HttpAsync::AsyncRequest::setBindAddr(const TC_Socket::addr_type &bindAddr)
	{
		_trans->setBindAddr(bindAddr);
	}

	void TC_HttpAsync::AsyncRequest::timeout()
	{
		if (_pHttpAsync) _pHttpAsync->assertThreadId();

		if (hasConnected())
			doException(RequestCallback::Failed_Timeout, "timeout error.");
		else
			doException(RequestCallback::Failed_ConnectTimeout, "connect timeout error.");

	}

	string TC_HttpAsync::AsyncRequest::getError(const string &sDefault) const
	{
		int ret = TC_Exception::getSystemCode();
		if(ret!= 0)
		{
			return sDefault + ", ret:" + TC_Common::tostr(ret) + ", msg:" + TC_Exception::parseError(ret);
		}

		return sDefault + ", ret:" + TC_Common::tostr(ret);
	}

	void TC_HttpAsync::AsyncRequest::doException(RequestCallback::FAILED_CODE ret, const string &e)
	{
		try { if (_callbackPtr) _callbackPtr->onFailed(ret, e); } catch (...) { }
	}

///////////////////////////////////////////////////////////////////////////
//#define H64(x) (((uint64_t)x) << 32)

	TC_HttpAsync::TC_HttpAsync() //: _terminate(false)
	{
		_data = new http_queue_type(10000);

		_epoller.create(10240);
	}

	TC_HttpAsync::~TC_HttpAsync()
	{
		terminate();

		delete _data;
	}

	void TC_HttpAsync::start()
	{
		_tpool.init(1);
		_tpool.start();

		_tpool.exec(std::bind(&TC_HttpAsync::run, this));
	}

	void TC_HttpAsync::waitForAllDone(int millsecond)
	{
		time_t now = TNOW;

		while (!_data->empty())
		{
			if (millsecond < 0)
			{
				TC_ThreadLock::Lock lock(*this);
				timedWait(100);
				continue;
			}

			{
				//等待100ms
				TC_ThreadLock::Lock lock(*this);
				timedWait(100);
			}

			if ((TNOW - now) >= (millsecond / 1000))
				break;
		}

		terminate();
	}

	void TC_HttpAsync::erase(uint32_t uniqId)
	{
		_erases.push_back(uniqId);
	}

	void TC_HttpAsync::terminate()
	{
		_epoller.terminate();

		_tpool.waitForAllDone();
	}

	void TC_HttpAsync::timeout(AsyncRequestPtr& ptr)
	{
		if(ptr->isValid())
		{
			ptr->timeout();
		}
	}

	void TC_HttpAsync::doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const TC_Endpoint &ep)
	{
		AsyncRequestPtr req = new AsyncRequest();

		req->initialize(&_epoller, ep, stHttpRequest, callbackPtr);

		if (_bindAddr.first)
		{
			req->setBindAddr(_bindAddr);
		}

		uint32_t uniqId = _data->generateId();

		req->setUniqId(uniqId);

		req->setHttpAsync(this);

		_data->push(req, uniqId);

		{
			std::lock_guard<std::mutex> lock(_mutex);
			_events.push_back(uniqId);
		}

		_epoller.notify();
	}

	void TC_HttpAsync::doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, bool bUseProxy)
	{
		TC_Endpoint ep;

		if(bUseProxy && _proxyEp)
		{
			ep = *this->_proxyEp.get();
		}
		else
		{
			string sHost;
			uint32_t iPort;
			stHttpRequest.getHostPort(sHost, iPort);
			ep.setHost(sHost);
			ep.setPort(iPort);

			if(TC_Port::strcmp(stHttpRequest.getURL().getScheme().c_str(), "https") == 0) {
				ep.setType(TC_Endpoint::SSL);
			}
		}

		doAsyncRequest(stHttpRequest, callbackPtr, ep);
	}

	void TC_HttpAsync::doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const string &addr)
	{
		vector<string> v = TC_Common::sepstr<string>(addr, ":");

		if (v.size() < 2)
		{
			throw TC_HttpAsync_Exception("[TC_HttpAsync::doAsyncRequest] addr is error:" + addr);
		}

		TC_Endpoint ep;

		ep.setHost(v[0]);
		ep.setPort(TC_Common::strto<uint16_t>(v[1]));

		if(TC_Port::strcmp(stHttpRequest.getURL().getScheme().c_str(), "https") == 0) {
			ep.setType(TC_Endpoint::SSL);
		}

		doAsyncRequest(stHttpRequest, callbackPtr, ep);
	}

	void TC_HttpAsync::setBindAddr(const char* sBindAddr)
	{
		_bindAddr = TC_Socket::createSockAddr(sBindAddr);
	}

	void TC_HttpAsync::setProxyAddr(const TC_Endpoint &ep)
	{
		_proxyEp.reset(new TC_Endpoint());

		*_proxyEp.get() = ep;
	}

	void TC_HttpAsync::setProxyAddr(const char* sProxyAddr)
	{
		vector<string> v = TC_Common::sepstr<string>(sProxyAddr, ":");

		if (v.size() < 2)
		{
			throw TC_HttpAsync_Exception("[TC_HttpAsync::setProxyAddr] addr is error:" + string(sProxyAddr));
		}

		TC_Endpoint ep;
		ep.setHost(v[0]);
		ep.setPort(TC_Common::strto<uint16_t>(v[1]));

		return setProxyAddr(ep);
	}

	void TC_HttpAsync::setProxyAddr(const char* sHost, uint16_t iPort)
	{
		TC_Endpoint ep;
		ep.setHost(sHost);
		ep.setPort(iPort);

		return setProxyAddr(ep);
	}

	bool TC_HttpAsync::handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
	{
		AsyncRequest* asyncRequest = (AsyncRequest*)data->cookie();

		asyncRequest->doException(RequestCallback::Failed_Net, asyncRequest->getError("epoller error"));

		asyncRequest->trans()->close();

		return false;
	}

	bool TC_HttpAsync::handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
	{
		AsyncRequest* asyncRequest = (AsyncRequest*)data->cookie();

		try
		{
			asyncRequest->trans()->doResponse();
		}
		catch(const std::exception& e)
		{
			asyncRequest->doException(RequestCallback::Failed_Net, e.what());
			return false;
		}

		return true;
	}

	bool TC_HttpAsync::handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
	{
		AsyncRequest* asyncRequest = (AsyncRequest*)data->cookie();

		try
		{
			asyncRequest->trans()->doRequest();
		}
		catch(const std::exception& e)
		{
			asyncRequest->doException(RequestCallback::Failed_Net, e.what());
			return false;
		}

		return true;
	}

	void TC_HttpAsync::addFd(AsyncRequest* asyncRequest)
	{
		shared_ptr<TC_Epoller::EpollInfo> epollInfo = asyncRequest->trans()->getEpollInfo();

		epollInfo->cookie(asyncRequest);

		map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

		callbacks[EPOLLIN] = std::bind(&TC_HttpAsync::handleInputImp, this, std::placeholders::_1);
		callbacks[EPOLLOUT] = std::bind(&TC_HttpAsync::handleOutputImp, this, std::placeholders::_1);
		callbacks[EPOLLERR] = std::bind(&TC_HttpAsync::handleCloseImp, this, std::placeholders::_1);

		epollInfo->registerCallback(callbacks, EPOLLIN|EPOLLOUT);
	}

	void TC_HttpAsync::run()
	{
		_threadId = std::this_thread::get_id();

		TC_TimeoutQueue<AsyncRequestPtr>::data_functor df(&TC_HttpAsync::timeout);

		_epoller.postRepeated(100, false, [&](){ _data->timeout(df); });

		_epoller.idle([&]{
			deque<uint64_t> events;

			{
				std::lock_guard<std::mutex> lock(_mutex);
				_events.swap(events);
			}

			for(auto data : events)
			{
				uint32_t uniqId = (uint32_t)data;

				AsyncRequestPtr ptr = _data->getAndRefresh(uniqId);
				if (!ptr)
				{
					continue;
				}

				try
				{
					ptr->trans()->connect();
				}
				catch(exception &ex)
				{
					ptr->doException(RequestCallback::Failed_Connect, ex.what());
				}
			}

			for(auto it : _erases)
			{
				_data->erase(it);
			}
			_erases.clear();
		});

		_epoller.loop();
	}

}


