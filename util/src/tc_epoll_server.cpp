/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include "util/tc_epoll_server.h"
#include "util/tc_coroutine.h"
#if TARGET_PLATFORM_WINDOWS
#include <WS2tcpip.h>
#else
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#endif

namespace tars
{

void TC_EpollServer::RecvContext::parseIpPort() const
{
	if (_ip.empty())
	{
		TC_Socket::parseAddr(_addr, _ip, _port);
	}
}

TC_EpollServer::DataBuffer::DataBuffer(int handleNum)
{
	for(int i = 0; i < handleNum; i++)
	{
		_threadDataQueue.push_back(std::make_shared<DataQueue>());
	}

	_schedulers.resize(handleNum);
	for(size_t i = 0; i < _schedulers.size(); i++)
	{
		_schedulers[i] = NULL;
	}
}

const shared_ptr<TC_EpollServer::DataBuffer::DataQueue> &TC_EpollServer::DataBuffer::getDataQueue(uint32_t handleIndex)
{
	//如果是队列模式, 则返回handle线程对应的队列
	if(isQueueMode())
	{
		return _threadDataQueue[index(handleIndex)];
	}

	//否则返回第一个队列(所有人共享)
	return _threadDataQueue[0];
}

void TC_EpollServer::DataBuffer::notifyBuffer(uint32_t handleIndex)
{
	getDataQueue(handleIndex)->notify();
}

void TC_EpollServer::DataBuffer::insertRecvQueue(const shared_ptr<RecvContext> &recv)
{
	++_iRecvBufferSize;

	getDataQueue(recv->fd())->push_back(recv);

	if(_schedulers[0] != NULL)
	{
		//存在调度器, 处于协程中
		if(isQueueMode())
		{
			_schedulers[index(recv->fd())]->notify();
		}
		else
		{
			_schedulers[index(rand())]->notify();
		}
	}
}

void TC_EpollServer::DataBuffer::insertRecvQueue(const deque<shared_ptr<RecvContext>> &recv)
{
	if (recv.empty())
	{
		return;
	}

	_iRecvBufferSize += recv.size();

	getDataQueue(recv.back()->fd())->push_back(recv);

	if (_schedulers[0] != NULL)
	{
		//存在调度器, 处于协程中
		if (isQueueMode())
		{
			_schedulers[index(recv.back()->fd())]->notify();
		}
		else
		{
			_schedulers[index(rand())]->notify();
		}
	}
}

bool TC_EpollServer::DataBuffer::wait(uint32_t handleIndex)
{
	return getDataQueue(handleIndex)->wait(_iWaitTime);
}

bool TC_EpollServer::DataBuffer::pop(uint32_t handleIndex, shared_ptr<RecvContext> &data)
{
	bool bRet = getDataQueue(handleIndex)->pop_front(data);

	if (!bRet)
	{
		return bRet;
	}

	--_iRecvBufferSize;

	return bRet;
}

void TC_EpollServer::DataBuffer::registerScheduler(uint32_t handleIndex, const shared_ptr<TC_CoroutineScheduler> &scheduler)
{
	assert(handleIndex < _schedulers.size());

	// LOG_CONSOLE_DEBUG << handleIndex << ", " << _schedulers.size() << endl;

	_schedulers[handleIndex] = scheduler;
}

void TC_EpollServer::DataBuffer::unregisterScheduler(uint32_t handleIndex)
{
	// LOG_CONSOLE_DEBUG << handleIndex << ", " << _schedulers.size() << endl;
	assert(handleIndex < _schedulers.size());

	_schedulers[handleIndex] = NULL;
}

const shared_ptr<TC_CoroutineScheduler> &TC_EpollServer::DataBuffer::getScheduler(uint32_t handleIndex)
{
	assert(handleIndex < _schedulers.size());

	return _schedulers[handleIndex];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// handle的实现
TC_EpollServer::Handle::Handle()
	: _epollServer(NULL)
{
}

TC_EpollServer::Handle::~Handle()
{
}

void TC_EpollServer::Handle::setWaitTime(uint32_t iWaitTime)
{
	this->getBindAdapter()->getDataBuffer()->setWaitTime(iWaitTime);
}

void TC_EpollServer::Handle::handleClose(const shared_ptr<RecvContext> & data)
{
}

void TC_EpollServer::Handle::handleTimeout(const shared_ptr<RecvContext> & data)
{
	_epollServer->error("[Handle::handleTimeout] queue timeout, close [" + data->ip() + ":" + TC_Common::tostr(data->port()) + "].");

	close(data);
}

void TC_EpollServer::Handle::handleOverload(const shared_ptr<RecvContext> & data)
{
	auto adapter = data->adapter();

	if(adapter)
	{
		_epollServer->error("[Handle::handleOverload] adapter '" + adapter->getName() + "',over load:"
							+ TC_Common::tostr(adapter->getRecvBufferSize()) + ">"
							+ TC_Common::tostr(adapter->getQueueCapacity()) + ".");
	}

	close(data);
}

void TC_EpollServer::Handle::terminate()
{
//	_terminate = true;

	notifyFilter();
}

void TC_EpollServer::Handle::handleOnceCoroutine()
{
	const shared_ptr<TC_CoroutineScheduler> &scheduler = TC_CoroutineScheduler::scheduler();

	assert(scheduler);

	//上报心跳
	heartbeat();

	//为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
	handleAsyncResponse();
	handleCustomMessage(true);

	bool bYield = false;

	shared_ptr<RecvContext> data;
	try
	{
		int	loop = 1000;

		while ((loop--) > 0 && !_epollServer->isTerminate())
		{
			if ((scheduler->getFreeSize() > 0) && _dataBuffer->pop(_handleIndex, data))
			{
				bYield = true;

				//上报心跳
				heartbeat();

				//为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
				handleAsyncResponse();

				if (data->isOverload())
				{
					//数据已超载 overload
					handleOverload(data);
				}
				else if (data->isClosed())
				{
					//关闭连接的通知消息
					handleClose(data);
				}
				else if ((TNOWMS - data->recvTimeStamp()) > (uint64_t)_bindAdapter->getQueueTimeout())
				{
					//数据在队列中已经超时了
					handleTimeout(data);
				}
				else
				{
					uint32_t iRet = scheduler->go(std::bind(&Handle::handle, this, data));
					if (iRet == 0)
					{
//						LOG_CONSOLE_DEBUG << "handleOverload" << endl;
						handleOverload(data);
					}
				}
				handleCustomMessage(false);
			}
			else
			{
				bYield = false;
				break;
			}
		}

		//循环100次, 没有任何消息需要处理, yield一下
		if (loop == 0) {
			bYield = false;
		}
	}
	catch (exception& ex)
	{
		if (data)
		{
			close(data);
		}

		getEpollServer()->error("[TC_EpollServer::Handle::handleOnceCoroutine] error:" + string(ex.what()));
	}
	catch (...)
	{
		if (data)
		{
			close(data);
		}

		getEpollServer()->error("[TC_EpollServer::Handle::handleOnceCoroutine] unknown error");
	}

	if (!bYield)
	{
		scheduler->yield();
	}
	else if(scheduler->isMainCoroutine())
	{
		scheduler->notify();
	}
//    LOG_CONSOLE("handleOnceCoroutine ok");

}

void TC_EpollServer::Handle::handleCoroutine()
{
	for(;;)
	{
		handleOnceCoroutine();
	}
}

void TC_EpollServer::Handle::handleOnceThread()
{
	//上报心跳
	heartbeat();

	//为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
	handleAsyncResponse();
	handleCustomMessage(true);

	shared_ptr<RecvContext> data;

	int loop = 100;

	while ((loop--) > 0 && _dataBuffer->pop(_handleIndex, data))
	{
		try
		{
			//上报心跳
			heartbeat();

			//为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
			handleAsyncResponse();

			if (data->isOverload())
			{
				//数据已超载 overload
				handleOverload(data);
			} else if (data->isClosed())
			{
				//关闭连接的通知消息
				handleClose(data);
			} else if ((TNOWMS - data->recvTimeStamp()) > (uint64_t) _bindAdapter->getQueueTimeout())
			{
				//数据在队列中已经超时了
				handleTimeout(data);
			} else
			{
				handle(data);
			}
			handleCustomMessage(false);
		}
		catch (exception &ex)
		{
			if (data)
			{
				close(data);
			}

			getEpollServer()->error("[Handle::handleImp] error:" + string(ex.what()));
		}
		catch (...)
		{
			if (data)
			{
				close(data);
			}

			getEpollServer()->error("[Handle::handleImp] unknown error");
		}
	}

	if (loop <= 0)
	{
		const shared_ptr<TC_CoroutineScheduler> &scheduler = TC_CoroutineScheduler::scheduler();
		if(scheduler && scheduler->isMainCoroutine())
		{
			scheduler->notify();
		}
	}
}

bool TC_EpollServer::Handle::isReady() const
{
	if(this->_epollServer->getOpenCoroutine() == NET_THREAD_QUEUE_HANDLES_THREAD)
	{
		return true;
	}
	else if(this->_epollServer->getOpenCoroutine() == NET_THREAD_QUEUE_HANDLES_CO)
	{
		return _scheduler && _scheduler->isReady();
	}

	assert(false);

	return false;
}

void TC_EpollServer::Handle::handleLoopCoroutine()
{
	//这种模式下, 为了保证当前线程能做和通信器结合, 必须也等在epoll上
	//因此当网络层收到数据, 写对队列后, 需要唤醒某一个handle的epoll, 从而唤醒某个协程
	_scheduler = TC_CoroutineScheduler::create();
	_scheduler->setPoolStackSize(this->_epollServer->getCoroutinePoolSize(), this->_epollServer->getCoroutineStackSize());
	_scheduler->getEpoller()->setName("epoller-handle");

	_dataBuffer->registerScheduler(_handleIndex, _scheduler);

	initialize();

	_scheduler->go(std::bind(&Handle::handleCoroutine, this));

	_epollServer->notifyThreadReady();

	_scheduler->run();

	_dataBuffer->unregisterScheduler(_handleIndex);

}

void TC_EpollServer::Handle::handleLoopThread()
{
	initialize();

	_epollServer->notifyThreadReady();

	while (!_epollServer->isTerminate())
	{
		_dataBuffer->wait(_handleIndex);

		handleOnceThread();
	}
}

bool TC_EpollServer::Handle::allAdapterIsEmpty()
{
	if (_dataBuffer->getRecvBufferSize() > 0) {
		return false;
	}

	return true;
}

bool TC_EpollServer::Handle::allFilterIsEmpty()
{
	return true;
}

void TC_EpollServer::Handle::notifyFilter()
{
	shared_ptr<TC_CoroutineScheduler> scheduler = _dataBuffer->getScheduler(this->_handleIndex);

	if (scheduler)
	{
		if(this->_epollServer->isTerminate())
		{
			scheduler->terminate();
		}
		else
		{
			scheduler->notify();
		}
	}
	else if(_netThread)
	{
		_netThread->notify();
	}
	else
	{
		_dataBuffer->notifyBuffer(_handleIndex);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务连接
TC_EpollServer::Connection::Connection(const shared_ptr<ConnectionList> &connList, BindAdapter *pBindAdapter, int fd, const string& ip, uint16_t port, detail::LogInterface* logger)
	: _connList(connList)
	, _logger(logger)
	, _pBindAdapter(pBindAdapter)
	, _uid(0)
	, _fd(fd)
	, _ip(ip)
	, _port(port)
	, _iHeaderLen(0)
	, _bClose(false)
	, _bEmptyConn(true)

{
	assert(fd != -1);

	_iLastRefreshTime = TNOW;
}

TC_EpollServer::Connection::Connection(const shared_ptr<ConnectionList> &connList, BindAdapter *pBindAdapter, int fd, detail::LogInterface *logger)
	: _connList(connList)
	, _logger(logger)
	, _pBindAdapter(pBindAdapter)
	, _uid(0)
	, _fd(fd)
	, _port(0)
	, _iHeaderLen(0)
	, _bClose(false)
	, _bEmptyConn(false) /*udp is always false*/

{
	// LOG_CONSOLE_DEBUG << endl;
	_iLastRefreshTime = TNOW;
}

TC_EpollServer::Connection::~Connection()
{
	// LOG_CONSOLE_DEBUG << endl;
}

void TC_EpollServer::Connection::initialize(TC_Epoller *epoller, unsigned int uid, NetThread *netThread)
{
	_uid        = uid;

	_netThread  = netThread;

	const TC_Endpoint &ep = _pBindAdapter->getEndpoint();

#if TARS_SSL
	if (ep.isSSL())
    {
        _trans.reset(new TC_SSLTransceiver(epoller, ep));
    }
    else if (ep.isTcp())
    {
        _trans.reset(new TC_TCPTransceiver(epoller, ep));
    }
    else
    {
        _trans.reset(new TC_UDPTransceiver(epoller, ep));
    }
#else
	if (ep.isUdp())
	{
		_trans.reset(new TC_UDPTransceiver(epoller, ep));
	}
	else
	{
		_trans.reset(new TC_TCPTransceiver(epoller, ep));
	}
#endif

	_trans->initializeServer(std::bind(&Connection::onCloseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
							 std::bind(&Connection::onRequestCallback, this, std::placeholders::_1),
							 std::bind(&Connection::onParserCallback, this, std::placeholders::_1, std::placeholders::_2),
							 std::bind(&Connection::onOpensslCallback, this, std::placeholders::_1),
							 TC_Transceiver::oncompletepackage_callback(),
							 std::bind(&Connection::onCompleteNetworkCallback, this, std::placeholders::_1));

	_trans->setServerAuthCallback(_pBindAdapter->_onVerifyCallback);

	_trans->getRecvBuffer().setConnection(this);

}

bool TC_EpollServer::Connection::handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
//	LOG_CONSOLE_DEBUG << endl;
	TC_EpollServer::NetThread *netThread = (TC_EpollServer::NetThread *)data->cookie();

	int ret = 0;
	try
	{
		ret = sendBuffer();
	}
	catch(const std::exception& ex)
	{
		// LOG_CONSOLE_DEBUG << ex.what() << endl;
		ret = -1;
		_logger->error(ex.what());
	}

	if (ret < 0)
	{
		netThread->delConnection(this, true, (ret == -1) ? EM_CLIENT_CLOSE : EM_SERVER_CLOSE);

		return false;
	}

	auto cl = _connList.lock();
	if(cl)
	{
		cl->refresh(getId(), getTimeout() + TNOW);
	}

	return true;
}

bool TC_EpollServer::Connection::handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
	// LOG_CONSOLE_DEBUG << endl;
	TC_EpollServer::NetThread *netThread = (TC_EpollServer::NetThread *)data->cookie();

	try
	{
		_trans->doResponse();
	}
	catch(const std::exception& ex)
	{
		// LOG_CONSOLE_DEBUG << ex.what() << endl;
		_logger->error(ex.what());
		netThread->delConnection(this, true, EM_CLIENT_CLOSE);
		return false;
	}

	auto cl = _connList.lock();
	if(cl)
	{
		cl->refresh(getId(), getTimeout() + TNOW);
	}

	return true;
}

bool TC_EpollServer::Connection::handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
	// LOG_CONSOLE_DEBUG << endl;
	TC_EpollServer::NetThread *netThread = (TC_EpollServer::NetThread *)data->cookie();

	netThread->delConnection(this, true, EM_SERVER_CLOSE);
	return false;
}

void TC_EpollServer::Connection::registerEvent(TC_EpollServer::NetThread *netThread)
{
	shared_ptr<TC_Epoller::EpollInfo> epollInfo = _trans->bindFd(_fd);

	epollInfo->cookie(netThread);

	map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

	callbacks[EPOLLIN] = std::bind(&TC_EpollServer::Connection::handleInputImp, this, std::placeholders::_1);
	callbacks[EPOLLOUT] = std::bind(&TC_EpollServer::Connection::handleOutputImp, this, std::placeholders::_1);
	callbacks[EPOLLERR] = std::bind(&TC_EpollServer::Connection::handleCloseImp, this, std::placeholders::_1);

	// 回调
	if (this->isTcp() && this->_pBindAdapter->_epollServer->_acceptFunc != NULL)
	{
		try 
		{ 
			this->_pBindAdapter->_epollServer->_acceptFunc(this); 
		}
		catch(exception &ex)
		{
			_logger->error(string("accept callback error:") + ex.what());
		}
	}

	epollInfo->registerCallback(callbacks, EPOLLIN | EPOLLOUT);

	//注意registerCallback, 网络线程马上关注网络事件, 并执行回调, 有可能由于_trans被释放了!
}

void TC_EpollServer::Connection::close()
{
	_trans->close();
	// LOG_CONSOLE_DEBUG << _trans.use_count() << endl;
}

void TC_EpollServer::Connection::onCloseCallback(TC_Transceiver *trans, TC_Transceiver::CloseReason reason, const string &err)
{
	if (trans->getEndpoint().isTcp() && trans->isValid())
	{
		_pBindAdapter->decreaseSendBufferSize();
	}
	_pBindAdapter->decreaseSendBufferSize(_messages.size());
}

std::shared_ptr<TC_OpenSSL> TC_EpollServer::Connection::onOpensslCallback(TC_Transceiver* trans)
{
#if TARS_SSL
	if(trans->isSSL()) {
		assert(_pBindAdapter->_ctx);
		return TC_OpenSSL::newSSL(_pBindAdapter->_ctx);
	}
	return NULL;
#else
	return NULL;
#endif
}

void TC_EpollServer::Connection::onRequestCallback(TC_Transceiver *trans)
{
	while(!_messages.empty())
	{
		auto it = _messages.begin();

		TC_Transceiver::ReturnStatus iRet = _trans->sendRequest((*it)->buffer(), (*it)->getRecvContext()->addr());

		if (iRet == TC_Transceiver::eRetError)
		{
			return;
		}

		if(iRet != TC_Transceiver::eRetNotSend)
		{
			_messageSize -= (*it)->buffer()->length();

			_messages.erase(it);
		}

		//数据还不能发送 or 发送buffer已经满了 直接返回, 暂时不要再发送了!
		if (iRet == TC_Transceiver::eRetNotSend || iRet == TC_Transceiver::eRetFull)
		{
			return;
		}
	}

}

TC_NetWorkBuffer::PACKET_TYPE TC_EpollServer::Connection::onParserCallback(TC_NetWorkBuffer& rbuf, TC_Transceiver *trans)
{
	if(rbuf.empty())
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	//需要过滤首包包头
	if (_iHeaderLen > 0) {
		if (rbuf.getBufferLength() >= (unsigned) _iHeaderLen) {
			vector<char> header;
			rbuf.getHeader(_iHeaderLen, header);
			_pBindAdapter->getHeaderFilterFunctor()(TC_NetWorkBuffer::PACKET_FULL, header);
			rbuf.moveHeader(_iHeaderLen);
			_iHeaderLen = 0;
		}
		else {
			vector<char> header = rbuf.getBuffers();
			_pBindAdapter->getHeaderFilterFunctor()(TC_NetWorkBuffer::PACKET_LESS, header);
			_iHeaderLen -= (int) rbuf.getBufferLength();
			rbuf.clearBuffers();
			return TC_NetWorkBuffer::PACKET_LESS;
		}
	}

	rbuf.setConnection(this);

	vector<char> ro;

	TC_NetWorkBuffer::PACKET_TYPE ret = _pBindAdapter->getProtocol()(rbuf, ro);

	if (ret == TC_NetWorkBuffer::PACKET_FULL)
	{
		auto recv = std::make_shared<RecvContext>(_netThread->getIndex(), getId(), trans->getClientAddr(), getfd(), _pBindAdapter->shared_from_this());

		recv->buffer().swap(ro);

		//收到完整的包才算
		this->_bEmptyConn = false;

		_recv.push_back(recv);
		//收到完整包
		// insertRecvQueue(recv);
	}

	return ret;
}

void TC_EpollServer::Connection::onCompleteNetworkCallback(TC_Transceiver *trans)
{
	_pBindAdapter->insertRecvQueue(_recv);

	//收到完整包
	// insertRecvQueue(_recv);

	_recv.clear();
}

int TC_EpollServer::Connection::sendBufferDirect(const char* buff, size_t length)
{
	_pBindAdapter->increaseSendBufferSize();

	if(getBindAdapter()->getEndpoint().isTcp()) {

		return _trans->sendRequest(std::make_shared<TC_NetWorkBuffer::Buffer>(buff, length));
	}

	return 0;
}

int TC_EpollServer::Connection::sendBufferDirect(const std::string& buff)
{
	return sendBufferDirect(buff.data(), buff.length());
}

int TC_EpollServer::Connection::sendBufferDirect(const shared_ptr<TC_NetWorkBuffer::Buffer>& buff)
{
	_pBindAdapter->increaseSendBufferSize();

	if(getBindAdapter()->getEndpoint().isTcp()) {

		return _trans->sendRequest(buff);
	}

	return 0;
}

int TC_EpollServer::Connection::checkFlow(TC_NetWorkBuffer& sendBuffer, size_t lastLeftBufferSize)
{
//	当出现队列积压的前提下, 且积压超过一定大小
//	每5秒检查一下积压情况, 连续12次(一分钟), 都是积压
//	且每个检查点, 积压长度都增加或者连续3次发送buffer字节小于1k, 就关闭连接, 主要避免极端情况

	//计算本次发送的大小: 上次没法送的数据大小 - 当前没发送的数据大小
	size_t nowSendBufferSize = lastLeftBufferSize - (_messageSize + sendBuffer.getBufferLength());

//	当出现队列积压的前提下, 且积压超过一定大小
//	每5秒检查一下积压情况, 连续12次(一分钟), 都是积压
//	且每个检查点, 积压长度都增加或者连续3次发送buffer字节小于1k, 就关闭连接, 主要避免极端情况

	size_t iBackPacketBuffLimit = _pBindAdapter->getBackPacketBuffLimit();
	if((_messageSize + sendBuffer.getBufferLength()) > iBackPacketBuffLimit)
	{
		if(_accumulateBufferSize == 0)
		{
			//开始积压
			_lastCheckTime = TNOW;
		}
		//累计本次发送数据(每5秒的数据累计到一起)
		_accumulateBufferSize += nowSendBufferSize;

		if (TNOW - _lastCheckTime >= 5)
		{
			//如果持续有积压, 则每5秒检查一次
			_lastCheckTime = TNOW;

			//记录本次累计的数据<5秒内发送数据, 剩余未发送数据>
			_checkSend.push_back(make_pair(_accumulateBufferSize, lastLeftBufferSize));

			_accumulateBufferSize = 0;

			size_t iBackPacketBuffMin = _pBindAdapter->getBackPacketBuffMin();

			//连续3个5秒, 发送速度都极慢, 每5秒发送 < iBackPacketBuffMin, 认为连接有问题, 关闭之
			int left = 3;
			if ((int)_checkSend.size() >= left)
			{
				bool slow = true;
				for (int i = (int)_checkSend.size() - 1; i >= (int)_checkSend.size() - left; i--)
				{
					//发送速度
					if (_checkSend[i].first > iBackPacketBuffMin)
					{
						slow = false;
						continue;
					}
				}

				if (slow)
				{
					ostringstream os;
					os << "send [" << _ip <<  ":" << _port << "] buffer queue send to slow, send size:";

					for (int i = (int)_checkSend.size() - 1; i >= (int)(_checkSend.size() - left); i--)
					{
						os << ", " << _checkSend[i].first;
					}

					_logger->error(os.str());
					sendBuffer.clearBuffers();
					return -5;
				}
			}

			//连续12个5秒, 都有积压现象, 检查
			if (_checkSend.size() >= 12)
			{
				bool accumulate = true;
				for (size_t i = _checkSend.size() - 1; i >= 1; i--) {
					//发送buffer 持续增加
					if (_checkSend[i].second < _checkSend[i - 1].second) {
						accumulate = false;
						break;
					}
				}

				//持续积压
				if (accumulate)
				{
					ostringstream os;
					os << "send [" << _ip <<  ":" << _port << "] buffer queue continues to accumulate data, queue size:";

					for (size_t i = 0; i < _checkSend.size(); i++)
					{
						os << ", " << _checkSend[i].second;
					}

					_logger->error(os.str());
					sendBuffer.clearBuffers();
					return -4;
				}

				_checkSend.erase(_checkSend.begin());
			}
		}
	}
	else
	{
		//无积压
		_accumulateBufferSize = 0;
		_lastCheckTime  = TNOW;
		_checkSend.clear();
	}
	return 0;
}

int TC_EpollServer::Connection::sendBuffer()
{
	TC_NetWorkBuffer& sendBuffer = _trans->getSendBuffer();

	//计算还有多少数据没有发送出去
	size_t lastLeftBufferSize = _messageSize + sendBuffer.getBufferLength();

	_trans->doRequest();

	//需要关闭链接
	if (_bClose && _trans->getSendBuffer().empty())
	{
		_logger->debug("send [" + _ip + ":" + TC_Common::tostr(_port) + "] close connection by user.");
		return -2;
	}

	return checkFlow(sendBuffer, lastLeftBufferSize);
}

int TC_EpollServer::Connection::send(const shared_ptr<SendContext> &sc)
{
	assert(sc);

	_pBindAdapter->increaseSendBufferSize();

	//队列为空, 直接发送, 发送失败进队列
	//队列不为空, 直接进队列
	const shared_ptr<TC_NetWorkBuffer::Buffer>& buff = sc->buffer();

	if(_messages.empty())
	{
		_trans->sendRequest(buff, sc->getRecvContext()->addr());
	}

	//网络句柄无效了, 返回-1, 上层会关闭连接
	if(!_trans->isValid())
	{
		return -1;
	}

	//数据没有发送完
	if(!buff->empty())
	{
		_messageSize += sc->buffer()->length();

		_messages.push_back(sc);
	}
	
    auto cl = _connList.lock();
	if(cl)
	{
		cl->refresh(getId(), getTimeout() + TNOW);
	}

	return 0;
}

void TC_EpollServer::Connection::setUdpRecvBuffer(size_t nSize)
{
	_trans->setUdpRecvBuffer(nSize);
}

bool TC_EpollServer::Connection::setClose()
{
	_bClose = true;

	return _trans->getSendBuffer().empty();
}

////////////////////////////////////////////////////////////////
//
TC_EpollServer::ConnectionList::ConnectionList(detail::LogInterface* logger)
	: _emptyCheckTimeout(0)
	, _logger(logger)
	, _total(0)
	, _free_size(0)
	, _vConn(NULL)
	, _lastTimeoutTime(0)
	, _iConnectionMagic(0)
{
}

void TC_EpollServer::ConnectionList::init(uint32_t size, uint32_t iIndex)
{
	_lastTimeoutTime = TNOW;

	_total = size;

	_free_size = 0;

	//初始化链接链表
	if (_vConn) delete[] _vConn;

	//分配total+1个空间(多分配一个空间, 第一个空间其实无效)
	_vConn = new list_data[_total+1];

	_iConnectionMagic   = ((((uint32_t)TNOW) << 26) & (0xFFFFFFFF << 26)) + ((iIndex << 22) & (0xFFFFFFFF << 22));//((uint32_t)_lastTimeoutTime) << 20;

	//free从1开始分配, 这个值为uid, 0保留为管道用, epollwait根据0判断是否是管道消息
	for(uint32_t i = 1; i <= _total; i++)
	{
		_vConn[i].first = NULL;

		_free.push_back(i);

		++_free_size;
	}

//	LOG_CONSOLE_DEBUG << this << ", " << "size:" << _free.size() << endl;

}

void TC_EpollServer::ConnectionList::close()
{
	if(_vConn)
	{
		//服务停止时, 主动关闭一下连接, 这样客户端会检测到, 不需要等下一个发送包时, 发送失败才知道连接被关闭
		for (auto it = _tl.begin(); it != _tl.end(); ++it) {
			if (_vConn[it->second].first != NULL) {
				_vConn[it->second].first->close();
				delete _vConn[it->second].first;
				_vConn[it->second].first = NULL;
			}
		}
		_tl.clear();
		delete[] _vConn;
		_vConn = NULL;
	}
}

void TC_EpollServer::ConnectionList::closeConnections(weak_ptr<BindAdapter> bindAdapter)
{
	auto adapter = bindAdapter.lock();

	if(_vConn && adapter)
	{
		multimap<time_t, uint32_t> tl = _tl;

		// LOG_CONSOLE_DEBUG << "list1 size:" << tl.size() << endl;

		//服务停止时, 主动关闭一下连接, 这样客户端会检测到, 不需要等下一个发送包时, 发送失败才知道连接被关闭
		for (auto it = tl.begin(); it != tl.end(); ++it)
		{
			if (_vConn[it->second].first != NULL && _vConn[it->second].first->getBindAdapter() == adapter.get())
			{
				this->delConnection(_vConn[it->second].first, true, TC_EpollServer::EM_SERVER_CLOSE);
			}
		}

		// LOG_CONSOLE_DEBUG << "list2 size:" << _tl.size() << endl;
	}
}

uint32_t TC_EpollServer::ConnectionList::getUniqId()
{
	TC_LockT<TC_ThreadMutex> lock(_mutex);

	assert(!_free.empty());

	uint32_t uid = _free.front();

	assert(uid > 0 && uid <= _total);

	_free.pop_front();

	--_free_size;

	return _iConnectionMagic | uid;
}

TC_EpollServer::Connection* TC_EpollServer::ConnectionList::get(uint32_t uid)
{
	uint32_t magi = uid & (0xFFFFFFFF << 22);
	uid           = uid & (0x7FFFFFFF >> 9);

	if (magi != _iConnectionMagic) return NULL;

	return _vConn[uid].first;
}

void TC_EpollServer::ConnectionList::add(Connection *cPtr, time_t iTimeOutStamp)
{
//	LOG_CONSOLE_DEBUG << "add timeout:" << iTimeOutStamp << endl;

	uint32_t muid = cPtr->getId();
	uint32_t magi = muid & (0xFFFFFFFF << 22);
	uint32_t uid  = muid & (0x7FFFFFFF >> 9);

	TC_LockT<TC_ThreadMutex> lock(_mutex);

	assert(magi == _iConnectionMagic && uid > 0 && uid <= _total && !_vConn[uid].first);

	_vConn[uid] = make_pair(cPtr, _tl.insert(make_pair(iTimeOutStamp, uid)));
}

void TC_EpollServer::ConnectionList::refresh(uint32_t uid, time_t iTimeOutStamp)
{
	uint32_t magi = uid & (0xFFFFFFFF << 22);
	uid           = uid & (0x7FFFFFFF >> 9);

	TC_LockT<TC_ThreadMutex> lock(_mutex);

	assert(magi == _iConnectionMagic && uid > 0 && uid <= _total && _vConn[uid].first);

	//至少一秒才刷新一次
	if(iTimeOutStamp - _vConn[uid].first->_iLastRefreshTime < 1)
	{
		return;
	}

	_vConn[uid].first->_iLastRefreshTime = iTimeOutStamp;

	//删除超时链表
	_tl.erase(_vConn[uid].second);

	_vConn[uid].second = _tl.insert(make_pair(iTimeOutStamp, uid));
}

void TC_EpollServer::ConnectionList::delConnection(Connection *cPtr, bool bEraseList, EM_CLOSE_T closeType)
{
	assert(cPtr->isTcp());

	BindAdapter* adapter = cPtr->getBindAdapter();

	//如果是TCP的连接才真正的关闭连接
	//false的情况,是超时被主动删除
	if(!bEraseList && _logger)
	{
		_logger->debug("timeout [" + cPtr->getIp() + ":" + TC_Common::tostr(cPtr->getPort()) + "] del from list");
	}

	uint32_t uid = cPtr->getId();

	//构造一个recv，通知业务该连接的关闭事件
	shared_ptr<RecvContext> recv   = std::make_shared<RecvContext>(cPtr->getNetThread()->getIndex(), uid, cPtr->getTransceiver()->getClientAddr(), cPtr->getfd(), adapter->shared_from_this(), true, (int)closeType);

	adapter->insertRecvQueue(recv, true);

	//从epoller删除句柄放在close之前, 否则重用socket时会有问题
	cPtr->close();

	//对于超时检查, 由于锁的原因, 在这里不从链表中删除
	if(bEraseList)
	{
		del(uid);
	}

	adapter->decreaseNowConnection();

}

void TC_EpollServer::ConnectionList::checkTimeout()
{
	time_t iCurTime = TNOW;

	//至少1s才能检查一次
	if(iCurTime - _lastTimeoutTime < 1)
	{
		return;
	}

	_lastTimeoutTime = iCurTime;

	TC_LockT<TC_ThreadMutex> lock(_mutex);

	multimap<time_t, uint32_t>::iterator it = _tl.begin();

	while(it != _tl.end())
	{
		//已经检查到当前时间点了, 后续不用在检查了
		if(it->first > iCurTime)
		{
			break;
		}

		uint32_t uid = it->second;

		++it;

		//udp的监听端口, 不做处理
		if(_vConn[uid].first->isUdp())
		{
			continue;
		}

		//配置的超时间大于0, 才需要检查连接是否超时
		if(_vConn[uid].first->getTimeout() > 0)
		{
			//超时关闭
			delConnection(_vConn[uid].first, false, EM_SERVER_TIMEOUT_CLOSE);

			//从链表中删除
			delNoLock(uid);
		}
	}

	if(_emptyCheckTimeout > 0)
	{
		it = _tl.begin();
		while(it != _tl.end())
		{
			uint32_t uid = it->second;

			//遍历所有的空连接
			if(_vConn[uid].first->isTcp() && _vConn[uid].first->isEmptyConn())
			{
				//获取空连接的超时时间点
				time_t iEmptyTimeout = (it->first - _vConn[uid].first->getTimeout()) + _emptyCheckTimeout/1000;

				//已经检查到当前时间点了, 后续不用在检查了
				if(iEmptyTimeout > iCurTime)
				{
					break;
				}

				//超时关闭
				delConnection(_vConn[uid].first, false, EM_SERVER_TIMEOUT_CLOSE);

				//从链表中删除
				delNoLock(uid);
			}

			++it;
		}
	}
}

vector<TC_EpollServer::ConnStatus> TC_EpollServer::ConnectionList::getConnStatus(int lfd)
{
	vector<ConnStatus> v;

	TC_LockT<TC_ThreadMutex> lock(_mutex);

	for(size_t i = 1; i <= _total; i++)
	{
		//是当前监听端口的连接
		if(_vConn[i].first != NULL && _vConn[i].first->isTcp()) //getListenfd() == lfd)
		{
			ConnStatus cs;

			cs.iLastRefreshTime = _vConn[i].first->_iLastRefreshTime;
			cs.ip               = _vConn[i].first->getIp();
			cs.port             = _vConn[i].first->getPort();
			cs.timeout          = _vConn[i].first->getTimeout();
			cs.uid              = _vConn[i].first->getId();
			cs.recvBufferSize   = _vConn[i].first->getRecvBuffer().getBufferLength();
			cs.sendBufferSize   = _vConn[i].first->getSendBuffer().getBufferLength();


			v.push_back(cs);
		}
	}

	return v;
}

void TC_EpollServer::ConnectionList::del(uint32_t uid)
{
	TC_LockT<TC_ThreadMutex> lock(_mutex);

	uint32_t magi = uid & (0xFFFFFFFF << 22);
	uid           = uid & (0x7FFFFFFF >> 9);

	assert(magi == _iConnectionMagic && uid > 0 && uid <= _total && _vConn[uid].first);

	delNoLock(uid);
}

void TC_EpollServer::ConnectionList::delNoLock(uint32_t uid)
{
	assert(uid > 0 && uid <= _total && _vConn[uid].first);

	_tl.erase(_vConn[uid].second);

	Connection *conn = _vConn[uid].first;

	_vConn[uid].first = NULL;

	delete conn;

	_free.push_back(uid);

	++_free_size;
}

/////////////////////////////BindAdapter///////////////////////////////////
TC_EpollServer::BindAdapter::BindAdapter(TC_EpollServer *epollServer)
	: _pReportQueue(NULL)
	, _pReportConRate(NULL)
	, _pReportTimeoutNum(NULL)
	, _epollServer(epollServer)
	, _pf(echo_protocol)
	, _hf(echo_header_filter)
	, _name("")
	, _iMaxConns(DEFAULT_MAX_CONN)
	, _iCurConns(0)
	, _iHandleNum(0)
	, _eOrder(ALLOW_DENY)
	, _iQueueCapacity(DEFAULT_QUEUE_CAP)
	, _iQueueTimeout(DEFAULT_QUEUE_TIMEOUT)
	, _iHeaderLen(0)
	, _iHeartBeatTime(0)
	, _protocolName("tars")
{
}

TC_EpollServer::BindAdapter::~BindAdapter()
{

}


void TC_EpollServer::BindAdapter::bind()
{
	// try
	// {

		assert(!_s.isValid());

#if TARGET_PLATFORM_WINDOWS
		int type = _ep.isIPv6() ? AF_INET6 : AF_INET;
#else
		int type = _ep.isUnixLocal() ? AF_LOCAL : _ep.isIPv6() ? AF_INET6 : AF_INET;
#endif

		int flag = 0;
#if TARGET_PLATFORM_LINUX
		flag = SOCK_CLOEXEC;
#endif

		if (_ep.isTcp())
		{
			_s.createSocket(SOCK_STREAM | flag, type);
		}
		else
		{
			_s.createSocket(SOCK_DGRAM | flag, type);
		}

#if TARGET_PLATFORM_WINDOWS
		_s.bind(_ep.getHost(), _ep.getPort());
#else
		if (_ep.isUnixLocal())
		{
			_s.bind(_ep.getHost().c_str());
		}
		else
		{
			_s.bind(_ep.getHost(), _ep.getPort());
		}
#endif

		if (_ep.isTcp())
		{
			_s.listen(10240);
			_s.setKeepAlive();
			_s.setTcpNoDelay();

			//不要设置close wait否则http服务回包主动关闭连接会有问题
			_s.setNoCloseWait();
		}
		_s.setblock(false);
	// }
	// catch(exception &ex)
	// {
	// 	_s.close();
	// 	cerr << "bind:" << _ep.toString() << " error:" << ex.what() << endl;
	// 	throw ex;
	// }
}

void TC_EpollServer::BindAdapter::setNetThreads(const vector<NetThread*> &netThreads)
{
	_netThreads = netThreads;

	for(auto netThread : _netThreads)
	{
		netThread->addAdapter(this);
	}
}

void TC_EpollServer::BindAdapter::initUdp(NetThread* netThread)
{
	//对于udp, 网络线程监听所有监听的udp端口
	if(this->_ep.isUdp())
	{
		Connection *cPtr = new Connection(netThread->getConnectionList(), this, _s.getfd(), _epollServer);

		netThread->addUdpConnection(cPtr);

	}
}

void TC_EpollServer::BindAdapter::setProtocolName(const string &name)
{
	std::lock_guard<std::mutex> lock (_mutex);

	_protocolName = name;
}

const string &TC_EpollServer::BindAdapter::getProtocolName()
{
	std::lock_guard<std::mutex> lock (_mutex);

	return _protocolName;
}

bool TC_EpollServer::BindAdapter::isTarsProtocol()
{
	return (_protocolName == "tars" || _protocolName == "tars");
}

bool TC_EpollServer::BindAdapter::isIpAllow(const string &ip) const
{
	std::lock_guard<std::mutex> lock (_mutex);

	if (_eOrder == ALLOW_DENY)
	{
		if (TC_Common::matchPeriod(ip, _vtAllow))
		{
			return true;
		}
		if (TC_Common::matchPeriod(ip, _vtDeny))
		{
			return false;
		}
	}
	else
	{
		if (TC_Common::matchPeriod(ip, _vtDeny))
		{
			return false;
		}
		if (TC_Common::matchPeriod(ip, _vtAllow))
		{
			return true;
		}
	}
	return _vtAllow.size() == 0;
}

void TC_EpollServer::BindAdapter::enableManualListen()
{
	_manualListen = true;
}

void TC_EpollServer::BindAdapter::manualListen()
{
	if(!this->getSocket().isValid())
	{
		weak_ptr<BindAdapter> weakPtr = shared_from_this();

		auto func = std::bind(&TC_EpollServer::listenCallback, _epollServer, weakPtr);

		_epollServer->getEpoller()->syncCallback(func);
	}
}

void TC_EpollServer::BindAdapter::cancelListen()
{
	if(this->getSocket().isValid())
	{
		weak_ptr<BindAdapter> weakPtr = shared_from_this();

		auto func = std::bind(&TC_EpollServer::listenCallback, _epollServer, weakPtr);

		_epollServer->getEpoller()->syncCallback(func);
	}
}

void TC_EpollServer::BindAdapter::insertRecvQueue(const shared_ptr<RecvContext> &recv, bool force)
{
	int iRet = isOverloadorDiscard();

	if (iRet == 0 || force) //未过载
	{
		_dataBuffer->insertRecvQueue(recv);
	}
	else if (iRet == -1) //超过队列长度4/5，需要进行overload处理
	{
		recv->setOverload();

		_dataBuffer->insertRecvQueue(recv);
	}
	else //接受队列满，需要丢弃
	{
		_epollServer->error("[BindAdapter::insertRecvQueue] overload discard package");
	}
}

void TC_EpollServer::BindAdapter::insertRecvQueue(const deque<shared_ptr<RecvContext>> &recv)
{
	int iRet = isOverloadorDiscard();

	if (iRet == 0) //未过载
	{
		_dataBuffer->insertRecvQueue(recv);
	}
	else if (iRet == -1) //超过队列长度4/5，需要进行overload处理
	{
		for(auto r : recv)
		{
			r->setOverload();
		}

		_dataBuffer->insertRecvQueue(recv);
	}
	else //接受队列满，需要丢弃
	{
		_epollServer->error("[BindAdapter::insertRecvQueue] overload discard package");
	}
}

TC_NetWorkBuffer::PACKET_TYPE TC_EpollServer::BindAdapter::echo_protocol(TC_NetWorkBuffer &r, vector<char> &o)
{
	o = r.getBuffers();

	r.clearBuffers();

	return TC_NetWorkBuffer::PACKET_FULL;
}

TC_NetWorkBuffer::PACKET_TYPE TC_EpollServer::BindAdapter::echo_header_filter(TC_NetWorkBuffer::PACKET_TYPE i, vector<char> &o)
{
	return TC_NetWorkBuffer::PACKET_FULL;
}

int TC_EpollServer::BindAdapter::isOverloadorDiscard()
{
	int iRecvBufferSize = _dataBuffer->getRecvBufferSize();

	if(iRecvBufferSize > (int)(_iQueueCapacity / 5.*4) && (iRecvBufferSize < _iQueueCapacity) && (_iQueueCapacity > 0)) //overload
	{
		//超过队列4/5开始认为过载
		return -1;
	}
	else if(iRecvBufferSize > (int)(_iQueueCapacity) &&  _iQueueCapacity > 0 ) //队列满需要丢弃接受的数据包
	{
		return -2;
	}
	return 0;
}

void TC_EpollServer::BindAdapter::setQueueTimeout(int t)
{
	if (t >= MIN_QUEUE_TIMEOUT)
	{
		_iQueueTimeout = t;
	}
	else
	{
		_iQueueTimeout = MIN_QUEUE_TIMEOUT;
	}
}

void TC_EpollServer::BindAdapter::setProtocol(const TC_NetWorkBuffer::protocol_functor &pf, int iHeaderLen, const TC_EpollServer::header_filter_functor &hf)
{
	_pf = pf;

	_hf = hf;

	_iHeaderLen = iHeaderLen;
}

//////////////////////////////NetThread//////////////////////////////////
TC_EpollServer::NetThread::NetThread(int threadIndex, TC_EpollServer *epollServer)
	: _epoller(NULL)
	, _threadIndex(threadIndex)
	, _epollServer(epollServer)
	, _nUdpRecvBufferSize(DEFAULT_RECV_BUFFERSIZE)
{
	_list = std::make_shared<ConnectionList>(_epollServer);
}

TC_EpollServer::NetThread::~NetThread()
{
	_list = NULL;
}

bool TC_EpollServer::NetThread::isReady() const
{
	return _scheduler && _scheduler->isReady();
}

void TC_EpollServer::NetThread::createEpoll(uint32_t maxAllConn)
{
	_list->init((uint32_t) maxAllConn, _threadIndex + 1);
}

void TC_EpollServer::NetThread::delConnection(TC_EpollServer::Connection *cPtr, bool bEraseList, TC_EpollServer::EM_CLOSE_T closeType)
{
	if (cPtr->isTcp())
	{
		_list->delConnection(cPtr, bEraseList, closeType);
	}
}

void TC_EpollServer::NetThread::terminate()
{
	assert(_scheduler);

	_scheduler->terminate();

}

void TC_EpollServer::NetThread::notifyCloseConnectionList(const shared_ptr<BindAdapter> &adapter)
{
	weak_ptr<BindAdapter> wAdapter = adapter;

	auto func = std::bind(&TC_EpollServer::ConnectionList::closeConnections, _list, adapter);

	_epoller->syncCallback(func);
}

void TC_EpollServer::NetThread::addTcpConnection(TC_EpollServer::Connection *cPtr)
{
	uint32_t uid = _list->getUniqId();

	// LOG_CONSOLE_DEBUG << "uid:" << uid << endl;

	cPtr->initialize(_epoller, uid, this);

	_list->add(cPtr, cPtr->getTimeout() + TNOW);

	cPtr->getBindAdapter()->increaseNowConnection();

	//注意epoll add必须放在最后, 否则可能导致执行完, 才调用上面语句
	cPtr->registerEvent(this);
}

void TC_EpollServer::NetThread::addUdpConnection(TC_EpollServer::Connection *cPtr)
{
	assert(_epoller != NULL);

	uint32_t uid = _list->getUniqId();

	// LOG_CONSOLE_DEBUG << "uid:" << uid << endl;

	cPtr->initialize(_epoller, uid, this);

	//udp分配接收buffer
	cPtr->setUdpRecvBuffer(_nUdpRecvBufferSize);

	_list->add(cPtr, cPtr->getTimeout() + TNOW);

	cPtr->registerEvent(this);
}

void TC_EpollServer::NetThread::close(const shared_ptr<TC_EpollServer::RecvContext> & data)
{
	if(this->_epollServer->isTerminate())
	{
		return;
	}

	shared_ptr<SendContext> send = data->createCloseContext();

	_sbuffer.push_back(send);

//	通知epoll响应, 关闭连接
	notify();
}

void TC_EpollServer::NetThread::send(const shared_ptr<TC_EpollServer::SendContext> & data)
{
	if(this->_epollServer->isTerminate())
	{
		return;
	}

	if(!_list)
	{
		//已经析构了!
		return;
	}

	if (_threadId == TC_Thread::CURRENT_THREADID())
	{
		//发送包线程和网络线程是同一个线程,直接发送即可
		Connection *cPtr = getConnectionPtr(data->uid());

		if (cPtr)
		{
			cPtr->send(data);
		}
	}
	else
	{
		//发送包线程和网络线程不是同一个线程, 需要先放队列, 再唤醒网络线程去发送
		_sbuffer.push_back(data);

		notify();
	}
}

void TC_EpollServer::NetThread::processPipe()
{
	// LOG_CONSOLE("processPipe");

	while (!_sbuffer.empty())
	{
		shared_ptr<SendContext> sc = _sbuffer.front();

		_sbuffer.pop_front();

		Connection *cPtr = getConnectionPtr(sc->uid());

		if (!cPtr)
		{
			continue;
		}

		switch (sc->cmd())
		{
			case 'c':
			{
				if (cPtr->setClose())
				{
					delConnection(cPtr, true, EM_SERVER_CLOSE);
				}
				break;
			}
			case 's':
			{
				int ret = cPtr->send(sc);
				if (ret < 0)
				{
					delConnection(cPtr, true, (ret == -1) ? EM_CLIENT_CLOSE : EM_SERVER_CLOSE);
				}
				break;
			}
			default:
				assert(false);
		}
	}
}

void TC_EpollServer::NetThread::setInitializeHandle(std::function<void()> initialize, std::function<void()> handle)
{
	_initialize = initialize;
	_handle     = handle;
}

void TC_EpollServer::NetThread::run()
{
	try
	{
		if(_epollServer->getOpenCoroutine() == NET_THREAD_QUEUE_HANDLES_THREAD || _epollServer->getOpenCoroutine() == NET_THREAD_MERGE_HANDLES_THREAD)
		{
			//线程模型, 此时调度器是local的, 线程里面无法感知到自己处于协程中
			_scheduler = std::make_shared<TC_CoroutineScheduler>();
			_scheduler->setPoolStackSize(10, 128*1024);

		}
		else
		{
			_scheduler = TC_CoroutineScheduler::create();

			_scheduler->setPoolStackSize(_epollServer->getCoroutinePoolSize(), _epollServer->getCoroutineStackSize());
		}

		_epoller = _scheduler->getEpoller();
		_epoller->setName("net-thread");

		assert(_epoller);

		_threadId = TC_Thread::CURRENT_THREADID();

		//对于udp, 网络线程监听所有监听的udp端口, 竞争收取数据
		for(auto adapter : _adapters)
		{
			adapter->initUdp(this);
		}

		_epoller->postRepeated(2000, false, std::bind(&ConnectionList::checkTimeout, _list));

		if(_initialize)
		{
			_initialize();
		}

		if(_handle)
		{
			_epoller->idle(_handle);
		}

		_epoller->idle(std::bind(&NetThread::processPipe, this));

		_epollServer->notifyThreadReady();

		_scheduler->run();

		_list->close();

	}
	catch(const std::exception& ex)
	{
		_epollServer->error(ex.what());
	}
	catch(...)
	{
		_epollServer->error("NetThread::run error");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
TC_EpollServer::TC_EpollServer(unsigned int iNetThreadNum)
:  _threadNum(iNetThreadNum)
, _pLocalLogger(NULL)
, _acceptFunc(NULL)
{
#if TARGET_PLATFORM_WINDOWS
	WSADATA wsadata;
WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif
//
//	_epoller.create(10240);
//	_epoller.setName("epollserver-epoller");
}

TC_EpollServer::~TC_EpollServer()
{
	terminate();

#if TARGET_PLATFORM_WINDOWS
	WSACleanup();
#endif
}

void TC_EpollServer::applicationCallback(TC_EpollServer *epollServer)
{
}

bool TC_EpollServer::accept(int fd, int domain)
{
	struct sockaddr_in stSockAddr4;
	struct ::sockaddr_in6 stSockAddr6;

	socklen_t iSockAddrSize = (AF_INET6 == domain) ? sizeof(::sockaddr_in6) : sizeof(sockaddr_in);
	struct sockaddr *stSockAddr = (AF_INET6 == domain) ? (struct sockaddr *) &stSockAddr6 : (struct sockaddr *) &stSockAddr4;

	TC_Socket cs;

	cs.setOwner(false);

	//接收连接
	TC_Socket s;

	s.init(fd, false, domain);
	int iRetCode = s.accept(cs, (struct sockaddr *) stSockAddr, iSockAddrSize);

	if (iRetCode > 0)
	{
		string ip;
		uint16_t port;

		char sAddr[INET6_ADDRSTRLEN] = "\0";

		inet_ntop(domain,
		          (AF_INET6 == domain) ? (void *) &stSockAddr6.sin6_addr : (void *) &stSockAddr4.sin_addr,
		          sAddr,
		          sizeof(sAddr));

		port = (AF_INET6 == domain) ? ntohs(stSockAddr6.sin6_port) : ntohs(stSockAddr4.sin_port);
		ip = sAddr;

		debug("accept [" + ip + ":" + TC_Common::tostr(port) + "] [" + TC_Common::tostr(cs.getfd()) + "] incomming");

		const BindAdapterPtr &adapter = _listeners[fd];

		if (!adapter->isIpAllow(ip))
		{
			debug("accept [" + ip + ":" + TC_Common::tostr(port) + "] [" + TC_Common::tostr(cs.getfd()) + "] not allowed");

			cs.close();

			return true;
		}

		if (adapter->isLimitMaxConnection())
		{
			error("accept [" + ip + ":" + TC_Common::tostr(port) + "][" + TC_Common::tostr(cs.getfd())
				      + "] beyond max connection:" + TC_Common::tostr(_listeners[fd]->getMaxConns()));
			cs.close();

			return true;
		}

		// LOG_CONSOLE_DEBUG << "fd:" << fd << ", cfd:" << cs.getfd() << endl;

		cs.setblock(false);
		cs.setKeepAlive();
		cs.setTcpNoDelay();
		cs.setCloseWaitDefault();

		const std::vector<NetThread *> &netThreads = adapter->getNetThreads();

		NetThread *netThread = netThreads[cs.getfd() % netThreads.size()];

		Connection *cPtr = new Connection(netThread->getConnectionList(), adapter.get(), cs.getfd(),  ip, port, this);

		//过滤连接首个数据包包头
		cPtr->setHeaderFilterLen((int) adapter->getHeaderFilterLen());

		netThread->addTcpConnection(cPtr);

		return true;
	}
	// else
	// {
	// 	//直到发生EAGAIN才不继续accept
	// 	if (TC_Socket::isPending())
	// 	{
	// 		return false;
	// 	}
	// }
	// return true;
	return false;
}

bool TC_EpollServer::acceptCallback(const shared_ptr<TC_Epoller::EpollInfo> &info, weak_ptr<BindAdapter> adapterPtr)
{
	auto adapter = adapterPtr.lock();

	if(!adapter)
	{
		return false;
	}
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS

	bool ret;
    do
    {
        ret = accept(info->fd(), adapter->getEndpoint().isIPv6() ? AF_INET6 : AF_INET);
	} while (ret);
#else
	accept(info->fd(), adapter->_ep.isIPv6() ? AF_INET6 : AF_INET);
#endif

	return true;
}

void TC_EpollServer::listenCallback(weak_ptr<BindAdapter> adapterPtr)
{
//	LOG_CONSOLE_DEBUG << endl;

	auto adapter = adapterPtr.lock();

	if(!adapter)
	{
		return ;
	}

	if(!adapter->getSocket().isValid())
	{
		//socket还没有开启, 则表示需要绑定
		adapter->bind();

		int fd = adapter->getSocket().getfd();

		_listeners[fd] = adapter;

		// assert(!adapter->getEpollInfo());

		auto info = _epoller->createEpollInfo(fd);

		adapter->setEpollInfo(info);

		map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

		callbacks[EPOLLIN] = std::bind(&TC_EpollServer::acceptCallback, this, std::placeholders::_1, adapter);

		adapter->getEpollInfo()->registerCallback(callbacks, EPOLLIN);

		this->info("bind adatper:" + adapter->getName());

	}
	else
	{
		//已经绑定中的, 则取消绑定
		int bindFd = adapter->getSocket().getfd();

		_listeners.erase(bindFd);

		adapter->getSocket().close();

		//通知线程关闭和BindAdapter相关的连接
		for(auto nt : this->_netThreads)
		{
			nt->notifyCloseConnectionList(adapter);
		}

		_epoller->releaseEpollInfo(adapter->getEpollInfo());

		adapter->getEpollInfo().reset();

		info("un bind adatper:" + adapter->getName());
	}
}

void TC_EpollServer::waitForShutdown()
{
	_epoller = new TC_Epoller();
	_epoller->create(10240);
	_epoller->setName("epollserver-epoller");

	_readyThreadNum = 0;

	initHandle();

	createEpoll();

	startHandle();

	if(_hf)
	{
		_epoller->postRepeated(5000, false, [&](){ _hf(this);});
	}

	for(auto it : _bindAdapters)
	{
		if(it->getEndpoint().isTcp())
		{
			if(it->getSocket().isValid())
			{
				//socket有效, 说明已经绑定了, 注册accept回调
				shared_ptr<TC_Epoller::EpollInfo> info = _epoller->createEpollInfo(it->getSocket().getfd());

				it->setEpollInfo(info);

				map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

				callbacks[EPOLLIN] = std::bind(&TC_EpollServer::acceptCallback, this, std::placeholders::_1, it);

				info->registerCallback(callbacks, EPOLLIN);
			}
		}
	}

	//等待协程调度器都启动, 否则有网络连接上时会出问题!
	waitForReady();

	_epoller->loop(10000);

	_epoller->clear();

	//停掉处理线程
	stopThread();

	//关闭监听端口
	auto it = _listeners.begin();
	while(it != _listeners.end())
	{
		if(it->second->getEndpoint().isTcp())
		{
			_epoller->releaseEpollInfo(it->second->getEpollInfo());
		}

		TC_Port::closeSocket(it->first);

		++it;
	}
	_listeners.clear();

	//回调
	if(_qf)
	{
		try
		{
			_qf(this);
		}
		catch (exception& ex)
		{
		}
	}

	//删除网络线程
	for (size_t i = 0; i < _netThreads.size(); ++i)
	{
		delete _netThreads[i];
	}

	_netThreads.clear();

	delete _epoller;

	_epoller = NULL;

	std::unique_lock<std::mutex> lock(_readyMutex);

	_readyThreadNum = 0;

	_readyCond.notify_all();
}

void TC_EpollServer::terminate()
{
	if(_epoller == NULL || _epoller->isTerminate())
	{
		return;
	}

	//先停止网络线程
	_epoller->terminate();

	//等待waitForShutdown退出!
	std::unique_lock<std::mutex> lock(_readyMutex);
	_readyCond.wait(lock, [&]{
		return _readyThreadNum == 0;
	});
}

void TC_EpollServer::setEmptyConnTimeout(int timeout)
{
	for (size_t i = 0; i < _netThreads.size(); ++i) {
		_netThreads[i]->setEmptyConnTimeout(timeout);
	}
}

int TC_EpollServer::bind(BindAdapterPtr & lsPtr)
{
	auto it = _listeners.begin();

	while (it != _listeners.end())
	{
		if (it->second->getName() == lsPtr->getName())
		{
			throw TC_Exception("bind name '" + lsPtr->getName() + "' conflicts.");
		}
		++it;
	}

	if(!lsPtr->isManualListen())
	{
		lsPtr->bind();

		_listeners[lsPtr->getSocket().getfd()] = lsPtr;
	}

	_bindAdapters.push_back(lsPtr);

	return lsPtr->getSocket().getfd();
}

void TC_EpollServer::initHandle()
{
	if(_openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_THREAD || _openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_CO)
	{
		//网路线程独立, 通过队列和处理线程交互数据
		//不同的连接丢给不同的网路线程处理
		//网络线程接收数据, 丢给对应adapter的队列, 处理线程/协程竞争数据
		for (int i = 0; i < _threadNum; ++i)
		{
			NetThread *netThread = new NetThread(i, this);

			_netThreads.push_back(netThread);
		}

		for (auto & bindAdapter : _bindAdapters)
		{
			bindAdapter->setNetThreads(_netThreads);
		}

		//启动handle线程
		for (auto & bindAdapter : _bindAdapters)
		{
			const vector<HandlePtr> & hds = bindAdapter->getHandles();

			for (uint32_t i = 0; i < hds.size(); ++i)
			{
				hds[i]->setDataBuffer(bindAdapter->getDataBuffer());
			}
		}

	}
	else if(_openCoroutine == TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO || _openCoroutine == TC_EpollServer::NET_THREAD_MERGE_HANDLES_THREAD)
	{
		//网络线程和处理线程是同一个线程(一对一)
		//不同的连接丢给不同的网路线程处理
		for (auto & bindAdapter : _bindAdapters)
		{
			vector<HandlePtr> & hds = bindAdapter->getHandles();

			vector<NetThread*> adapterNetThreads;

			for (uint32_t i = 0; i < hds.size(); ++i)
			{
				//注意net thread是和adapter相关, 索引每个adapter从头计算
				NetThread *netThread = new NetThread(i, this);

				_netThreads.push_back(netThread);

				adapterNetThreads.push_back(netThread);

				hds[i]->setDataBuffer(bindAdapter->getDataBuffer());
				hds[i]->setNetThread(netThread);

				if(_openCoroutine == TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO)
				{
					netThread->setInitializeHandle(std::bind(&Handle::initialize, hds[i]), std::bind(&Handle::handleOnceCoroutine, hds[i]));
				}
				else if(_openCoroutine == TC_EpollServer::NET_THREAD_MERGE_HANDLES_THREAD)
				{
					netThread->setInitializeHandle(std::bind(&Handle::initialize, hds[i]), std::bind(&Handle::handleOnceThread, hds[i]));
				}
			}

			bindAdapter->setNetThreads(adapterNetThreads);
		}
	}
	else
	{
		error("please check server pattern, exit!");
		exit(-1);
	}
}

void TC_EpollServer::startHandle()
{
	if(_openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_THREAD || _openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_CO)
	{
		for (size_t i = 0; i < _netThreads.size(); ++i)
		{
			_netThreads[i]->start();
		}

//		LOG_CONSOLE_DEBUG <<  "start thread:" << _netThreads.size() << endl;

		int handleNum = 0;

		//启动handle线程
		for (auto & bindAdapter : _bindAdapters)
		{
			handleNum += bindAdapter->getHandleNum();

			const vector<HandlePtr> & hds = bindAdapter->getHandles();

			for (uint32_t i = 0; i < hds.size(); ++i)
			{
				if(_openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_THREAD)
				{
					_handlePool.exec(std::bind(&Handle::handleLoopThread, hds[i]));
				}
				else if(_openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_CO)
				{
					_handlePool.exec(std::bind(&Handle::handleLoopCoroutine, hds[i]));
				}
			}
		}

		_handlePool.init(handleNum);

		_handlePool.start();
	}
	else if(_openCoroutine == TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO || _openCoroutine == TC_EpollServer::NET_THREAD_MERGE_HANDLES_THREAD)
	{
		//网络线程和处理线程是同一个线程(一对一)
		//不同的连接丢给不同的网路线程处理
		for (auto & bindAdapter : _bindAdapters)
		{
			vector<HandlePtr> & hds = bindAdapter->getHandles();

			for (uint32_t i = 0; i < hds.size(); ++i)
			{
				hds[i]->getNetThread()->start();
			}
		}
	}
	else
	{
		error("please check server pattern, exit!");
		exit(-1);
	}
}

void TC_EpollServer::notifyThreadReady()
{
	std::unique_lock<std::mutex> lock(_readyMutex);

	++_readyThreadNum;

	_readyCond.notify_all();
}

void TC_EpollServer::waitForReady()
{
	int readyThreadNum = 0;

	if(_openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_THREAD || _openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_CO)
	{
		readyThreadNum = _threadNum;
	}

	for (auto & bindAdapter : _bindAdapters)
	{
		const vector<HandlePtr> & hds = bindAdapter->getHandles();

		readyThreadNum += hds.size();
	}

	{
		std::unique_lock<std::mutex> lock(_readyMutex);
		if(_readyThreadNum == readyThreadNum)
		{
			return;
		}
		_readyCond.wait(lock, [&]{
			return _readyThreadNum == readyThreadNum;
		});
	}
}

void TC_EpollServer::stopThread()
{
	if(_openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_THREAD || _openCoroutine == TC_EpollServer::NET_THREAD_QUEUE_HANDLES_CO)
	{
		//先停止网络线程(如果先停止处理线程/协程以后, 否则来了网络事件, 会导致crash)
		for (size_t i = 0; i < _netThreads.size(); ++i)
		{
			if (_netThreads[i]->joinable())
			{
				_netThreads[i]->terminate();

				_netThreads[i]->getThreadControl().join();
			}
		}

		//停止掉handle线程
		for (auto & bindAdapter : _bindAdapters)
		{
			const vector<HandlePtr> & hds = bindAdapter->getHandles();

			for (uint32_t i = 0; i < hds.size(); ++i)
			{
				hds[i]->terminate();
			}
		}

		_handlePool.stop();
	}
	else
	{
		//合并网络线程和处理线程, 直接停止网络线程即可
		for (auto & bindAdapter : _bindAdapters)
		{
			const vector<HandlePtr> & hds = bindAdapter->getHandles();

			for (uint32_t i = 0; i < hds.size(); ++i)
			{
				hds[i]->terminate();

				if(hds[i]->getNetThread()->joinable())
				{
					hds[i]->getNetThread()->terminate();
					hds[i]->getNetThread()->getThreadControl().join();
				}
			}
		}
	}

}

void TC_EpollServer::createEpoll()
{
	uint32_t maxAllConn = 0;

	//监听socket
//	auto it = _listeners.begin();

	for (auto it : _bindAdapters)
	{
		if (it->getEndpoint().isTcp())
		{
			maxAllConn += it->getMaxConns();
		}
		else {
			maxAllConn++;
		}

//		++it;
	}

	if (maxAllConn >= (1 << 22)) {
		error("createEpoll connection num: " + TC_Common::tostr(maxAllConn) + " >= " + TC_Common::tostr(1 << 22));
		maxAllConn = (1 << 22) - 1;
	}

	for (size_t i = 0; i < _netThreads.size(); ++i) {
		_netThreads[i]->createEpoll(maxAllConn);
	}
}

TC_EpollServer::BindAdapterPtr TC_EpollServer::getBindAdapter(const string & sName)
{
	auto it = _bindAdapters.begin();

	while (it != _bindAdapters.end())
	{
		if ((*it)->getName() == sName)
		{
			return (*it);
		}

		++it;
	}
	return NULL;
}

vector<TC_EpollServer::BindAdapterPtr> TC_EpollServer::getBindAdapters()
{
	return _bindAdapters;
}

void TC_EpollServer::close(const shared_ptr<RecvContext> & data)
{
	auto adapter = data->adapter();

	if(adapter && !adapter->isUdp()) {
		//非UDP模式下, close才有效
		adapter->getNetThread(data->threadIndex())->close(data);
	}
}

void TC_EpollServer::send(const shared_ptr<SendContext> & data)
{
	if(data->buffer()->empty())
		return;

	auto adapter = data->getRecvContext()->adapter();

	if(adapter)
	{
		adapter->getNetThread(data->getRecvContext()->threadIndex())->send(data);
	}
}

void TC_EpollServer::debug(const string & s) const
{
	if (_pLocalLogger) {
		_pLocalLogger->debug() << "[TARS]" << s << endl;
	}
}

void TC_EpollServer::info(const string & s) const
{
	if (_pLocalLogger) {
		_pLocalLogger->info() << "[TARS]" << s << endl;
	}
}

void TC_EpollServer::tars(const string & s) const
{
	if (_pLocalLogger) {
		_pLocalLogger->tars() << "[TARS]" << s << endl;
	}
}

void TC_EpollServer::error(const string & s) const
{
	if (_pLocalLogger) {
		_pLocalLogger->error() << "[TARS]" << s << endl;
	}
}

vector<TC_EpollServer::ConnStatus> TC_EpollServer::getConnStatus(int lfd)
{
	vector<ConnStatus> vConnStatus;
	for (size_t i = 0; i < _netThreads.size(); ++i) {
		vector<ConnStatus> tmp = _netThreads[i]->getConnStatus(lfd);
		for (size_t k = 0; k < tmp.size(); ++k) {
			vConnStatus.push_back(tmp[k]);
		}
	}
	return vConnStatus;
}

unordered_map<int, TC_EpollServer::BindAdapterPtr> TC_EpollServer::getListenSocketInfo()
{
	return _listeners;
}

size_t TC_EpollServer::getConnectionCount()
{
	size_t iConnTotal = 0;
	for (size_t i = 0; i < _netThreads.size(); ++i) {
		iConnTotal += _netThreads[i]->getConnectionCount();
	}
	return iConnTotal;
}

size_t TC_EpollServer::getLogicThreadNum()
{
	size_t iNum = 0;

	for (auto & bindAdapter : _bindAdapters) {
		iNum += bindAdapter->getHandles().size();
	}
	return iNum;
}

void TC_EpollServer::setOpenCoroutine(SERVER_OPEN_COROUTINE openCoroutine)
{
	if(openCoroutine >= NET_THREAD_QUEUE_HANDLES_THREAD && openCoroutine <= NET_THREAD_MERGE_HANDLES_CO)
	{
		_openCoroutine = openCoroutine;
	}
}

void TC_EpollServer::setCoroutineStack(uint32_t iPoolSize, size_t iStackSize)
{
	_iCoroutinePoolSize = iPoolSize;
	_iCoroutineStackSize = iStackSize;
}

}
