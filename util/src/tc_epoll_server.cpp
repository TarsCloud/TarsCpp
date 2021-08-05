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
#include "util/tc_port.h"
#include "util/tc_common.h"
#include "util/tc_network_buffer.h"
#include "util/tc_timeprovider.h"
#include <cassert>
#include <iostream>


//#define FILE_FUNC_LINE          "[" << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << "]" 
//#define LOG_CONSOLE cout << this_thread::get_id() <<"|"<< TC_Common::now2str()<< FILE_FUNC_LINE << "|"

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

#if TARS_SSL
#include "util/tc_openssl.h"
#endif


namespace tars
{

static const int BUFFER_SIZE = 8 * 1024;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// handle的实现
TC_EpollServer::Handle::Handle() 
: _pEpollServer(NULL)
, _iWaitTime(100)
{
}

TC_EpollServer::Handle::~Handle()
{
}

void TC_EpollServer::Handle::handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
}

void TC_EpollServer::Handle::sendResponse(const shared_ptr<TC_EpollServer::SendContext> &data)//uint32_t uid, const vector<char> &sSendBuffer, const string &ip, int port, int fd)
{
    _pEpollServer->send(data);
}

void TC_EpollServer::Handle::close(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    _pEpollServer->close(data);
}

void TC_EpollServer::Handle::handleTimeout(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    _pEpollServer->error("[Handle::handleTimeout] queue timeout, close [" + data->ip() + ":" + TC_Common::tostr(data->port()) + "].");

    close(data);
}

void TC_EpollServer::Handle::handleOverload(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    _pEpollServer->error("[Handle::handleOverload] adapter '" + data->adapter()->getName() + "',over load:" + TC_Common::tostr(data->adapter()->getRecvBufferSize()) + ">" + TC_Common::tostr(data->adapter()->getQueueCapacity()) + ".");

    close(data);
}

void TC_EpollServer::Handle::run()
{
    initialize();

    handleImp();
}

bool TC_EpollServer::Handle::allAdapterIsEmpty()
{
    if (_bindAdapter->getRecvBufferSize() > 0)
    {
        return false;
    }

    return true;
}

bool TC_EpollServer::Handle::allFilterIsEmpty()
{
    return true;
}

void TC_EpollServer::Handle::setEpollServer(TC_EpollServer *pEpollServer)
{
    _pEpollServer = pEpollServer;
}

void TC_EpollServer::Handle::setBindAdapter(BindAdapter*  bindAdapter)
{
    _bindAdapter = bindAdapter;
}

void TC_EpollServer::Handle::setHandleIndex(uint32_t index)
{
    _handleIndex = index;
}

void TC_EpollServer::Handle::setNetThread(NetThread *netThread)
{
    _netThread = netThread;
}

void TC_EpollServer::Handle::notifyFilter()
{
    _bindAdapter->notifyHandle(_handleIndex);
}

void TC_EpollServer::Handle::setWaitTime(uint32_t iWaitTime)
{
    _iWaitTime = iWaitTime;
}

void TC_EpollServer::Handle::process(shared_ptr<RecvContext> data)
{
    try
    {
        //上报心跳
        heartbeat();

        //为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
        handleAsyncResponse();

        //数据已超载 overload
        if (data->isOverload())
        {
            handleOverload(data);
        }
            //关闭连接的通知消息
        else if (data->isClosed())
        {
            handleClose(data);
        }
            //数据在队列中已经超时了
        else if ((TNOWMS - data->recvTimeStamp()) > (int64_t)_bindAdapter->getQueueTimeout())
        {
            handleTimeout(data);
        }
        else
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

void TC_EpollServer::Handle::wait()
{
    if (allAdapterIsEmpty() && allFilterIsEmpty())
    {
        _bindAdapter->waitAtQueue(_handleIndex, _iWaitTime);
    }
}

bool TC_EpollServer::Handle::popRecvQueue(shared_ptr<RecvContext> &recv)
{
    return _bindAdapter->waitForRecvQueue(_handleIndex, recv);
}

void TC_EpollServer::Handle::handleImp()
{
    //by goodenpei, 判断是否启用顺序模式
    _bindAdapter->initThreadRecvQueue(getHandleIndex());

    startHandle();

    while (!getEpollServer()->isTerminate())
    {
        //等待
        wait();

        //上报心跳
        heartbeat();

        //为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
        handleAsyncResponse();
        handleCustomMessage(true);

        shared_ptr<RecvContext> data;

        try
        {
            while (popRecvQueue(data))//_bindAdapter->waitForRecvQueue(_handleIndex, data))
            {
                //上报心跳
                heartbeat();

                //为了实现所有主逻辑的单线程化,在每次循环中给业务处理自有消息的机会
                handleAsyncResponse();

                //数据已超载 overload
                if (data->isOverload())
                {
                    handleOverload(data);
                }
                //关闭连接的通知消息
                else if (data->isClosed())
                {
                    handleClose(data);
                }
                //数据在队列中已经超时了
                else if ((TNOWMS - data->recvTimeStamp()) > (int64_t)_bindAdapter->getQueueTimeout())
                {
                    handleTimeout(data);
                }
                else
                {
                    handle(data);
                }
                handleCustomMessage(false);
            }
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

    stopHandle();
}

////////////////////////////BindAdapter///////////////////////////////////
TC_EpollServer::BindAdapter::BindAdapter(TC_EpollServer *pEpollServer)
    : _pReportQueue(NULL)
    , _pReportConRate(NULL)
    , _pReportTimeoutNum(NULL)
    , _pEpollServer(pEpollServer)
    ,  _pf(echo_protocol)
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
    return (_protocolName == "tars");
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

void TC_EpollServer::BindAdapter::manualListen()
{
    this->getEpollServer()->_epoller.mod(getSocket().getfd(), getSocket().getfd(), EPOLLIN|EPOLLOUT);
}

void TC_EpollServer::BindAdapter::initThreadRecvQueue(uint32_t handeIndex)
{
    if(isQueueMode()) {
        _threadDataQueue[handeIndex + 1] = std::make_shared<BindAdapter::DataQueue>();
    }
}

TC_EpollServer::recv_queue &TC_EpollServer::BindAdapter::getRecvQueue(uint32_t handleIndex)
{
    if(isQueueMode())
    {
        return _threadDataQueue[handleIndex+1]->_rbuffer;
    }

    return _threadDataQueue[0]->_rbuffer;
}

TC_ThreadLock &TC_EpollServer::BindAdapter::getLock(uint32_t handleIndex)
{
    if(isQueueMode())
    {
        return _threadDataQueue[handleIndex+1]->_monitor;
    }

    return _threadDataQueue[0]->_monitor;
}

void TC_EpollServer::BindAdapter::waitAtQueue(uint32_t handleIndex, uint32_t waitTime)
{
    TC_ThreadLock &l = getLock(handleIndex);

    TC_ThreadLock::Lock lock(l);

    l.timedWait(waitTime);
}

void TC_EpollServer::BindAdapter::notifyHandle(uint32_t handleIndex)
{
    if(this->getEpollServer()->isMergeHandleNetThread())
    {
        //业务线程和网络线程合并了, 直接通知网络线程醒过来即可
        NetThread *netThread = getHandle(handleIndex)->getNetThread();

        assert(netThread != NULL);

        netThread->notify();
    }
    else {
        //通知对应的Handle线程队列醒过来
        if (isQueueMode()) {
            handleIndex = handleIndex + 1;
            //相同连接过来的进入同一个buffer, 被Handle的同一个线程处理
            TC_ThreadLock::Lock lock(_threadDataQueue[handleIndex]->_monitor);
            _threadDataQueue[handleIndex]->_monitor.notify();
        }
        else {
            TC_ThreadLock::Lock lock(_threadDataQueue[0]->_monitor);
            _threadDataQueue[0]->_monitor.notifyAll();
        }
    }
}

void TC_EpollServer::BindAdapter::insertRecvQueue(const shared_ptr<RecvContext> &recv)
{
    _iRecvBufferSize++;

    size_t idx = 0;

    if(isQueueMode()) {
        //相同连接过来的进入同一个buffer, 被Handle的同一个线程处理
        idx = recv->fd() % _iHandleNum + 1;
    }
    _threadDataQueue[idx]->_rbuffer.push_back(recv);

    //通知对应的线程队列醒过来
    TC_ThreadLock::Lock lock(_threadDataQueue[idx]->_monitor);
    _threadDataQueue[idx]->_monitor.notify();

}

bool TC_EpollServer::BindAdapter::waitForRecvQueue(uint32_t handleIndex, shared_ptr<RecvContext> &data)
{
    bool bRet = getRecvQueue(handleIndex).pop_front(data);

    if (!bRet)
    {
        return bRet;
    }

    --_iRecvBufferSize;

    return bRet;
}

size_t TC_EpollServer::BindAdapter::getRecvBufferSize() const
{
    return _iRecvBufferSize;
}

size_t TC_EpollServer::BindAdapter::getSendBufferSize() const
{
    return _iSendBufferSize;
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

void TC_EpollServer::BindAdapter::setName(const string &name)
{
    std::lock_guard<std::mutex> lock (_mutex);
    _name = name;
}

string TC_EpollServer::BindAdapter::getName() const
{
    std::lock_guard<std::mutex> lock (_mutex);
    return _name;
}

int TC_EpollServer::BindAdapter::getHandleNum()
{
    return _iHandleNum;
}

int TC_EpollServer::BindAdapter::getQueueCapacity() const
{
    return _iQueueCapacity;
}

void TC_EpollServer::BindAdapter::setQueueCapacity(int n)
{
    _iQueueCapacity = n;
}

int TC_EpollServer::BindAdapter::isOverloadorDiscard()
{
    int iRecvBufferSize = _iRecvBufferSize;

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

int TC_EpollServer::BindAdapter::getQueueTimeout() const
{
    return _iQueueTimeout;
}

void TC_EpollServer::BindAdapter::setEndpoint(const string &str)
{
    std::lock_guard<std::mutex> lock (_mutex);

    _ep.parse(str);
}

TC_Endpoint TC_EpollServer::BindAdapter::getEndpoint() const
{
    std::lock_guard<std::mutex> lock (_mutex);
    return _ep;
}

TC_Socket &TC_EpollServer::BindAdapter::getSocket()
{
    return _s;
}

void TC_EpollServer::BindAdapter::setMaxConns(int iMaxConns)
{
    _iMaxConns = iMaxConns;
}

size_t TC_EpollServer::BindAdapter::getMaxConns() const
{
    return _iMaxConns;
}

void TC_EpollServer::BindAdapter::setHeartBeatTime(time_t t)
{
    _iHeartBeatTime = t;
}

time_t TC_EpollServer::BindAdapter::getHeartBeatTime() const
{
    return _iHeartBeatTime;
}

void TC_EpollServer::BindAdapter::setOrder(EOrder eOrder)
{
    _eOrder = eOrder;
}

void TC_EpollServer::BindAdapter::setAllow(const vector<string> &vtAllow)
{
    std::lock_guard<std::mutex> lock (_mutex);

    _vtAllow = vtAllow;
}

void TC_EpollServer::BindAdapter::setDeny(const vector<string> &vtDeny)
{
    std::lock_guard<std::mutex> lock (_mutex);

    _vtDeny = vtDeny;
}

TC_EpollServer::BindAdapter::EOrder TC_EpollServer::BindAdapter::getOrder() const
{
    return _eOrder;
}

const vector<string> &TC_EpollServer::BindAdapter::getAllow() const
{
    return _vtAllow;
}

const vector<string> &TC_EpollServer::BindAdapter::getDeny() const
{
    return _vtDeny;
}

bool TC_EpollServer::BindAdapter::isLimitMaxConnection() const
{
    return (_iCurConns + 1 > _iMaxConns) || (_iCurConns + 1 > (int)((uint32_t)1 << 22) - 1);
}

void TC_EpollServer::BindAdapter::decreaseNowConnection()
{
    --_iCurConns;
}

void TC_EpollServer::BindAdapter::increaseNowConnection()
{
    ++_iCurConns;
}

int TC_EpollServer::BindAdapter::getNowConnection() const
{
    return _iCurConns;
}

vector<TC_EpollServer::ConnStatus> TC_EpollServer::BindAdapter::getConnStatus()
{
    return _pEpollServer->getConnStatus(_s.getfd());
}

void TC_EpollServer::BindAdapter::setProtocol(const TC_NetWorkBuffer::protocol_functor &pf, int iHeaderLen, const TC_EpollServer::header_filter_functor &hf)
{
    _pf = pf;

    _hf = hf;

    _iHeaderLen = iHeaderLen;
}

TC_NetWorkBuffer::protocol_functor &TC_EpollServer::BindAdapter::getProtocol()
{
    return _pf;
}

TC_EpollServer::header_filter_functor &TC_EpollServer::BindAdapter::getHeaderFilterFunctor()
{
    return _hf;
}

int TC_EpollServer::BindAdapter::getHeaderFilterLen()
{
    return _iHeaderLen;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务连接
TC_EpollServer::Connection::Connection(TC_EpollServer::BindAdapter *pBindAdapter, int lfd, int timeout, int fd, const string& ip, uint16_t port)
: _pBindAdapter(pBindAdapter)
, _uid(0)
, _lfd(lfd)
, _timeout(timeout)
, _ip(ip)
, _port(port)
, _recvBuffer(this)
, _sendBuffer(this)
, _iHeaderLen(0)
, _bClose(false)
, _enType(EM_TCP)
, _bEmptyConn(true)
, _pRecvBuffer(NULL)
, _nRecvBufferSize(DEFAULT_RECV_BUFFERSIZE)
, _authInit(false)
{
    assert(fd != -1);

    _iLastRefreshTime = TNOW;

    _sock.init(fd, true, pBindAdapter->_ep.isIPv6() ? AF_INET6 : AF_INET);
}

TC_EpollServer::Connection::Connection(BindAdapter *pBindAdapter, int fd)
: _pBindAdapter(pBindAdapter)
, _uid(0)
, _lfd(-1)
, _timeout(2)
, _port(0)
, _recvBuffer(this)
, _sendBuffer(this)
, _iHeaderLen(0)
, _bClose(false)
, _enType(EM_UDP)
, _bEmptyConn(false) /*udp is always false*/
, _pRecvBuffer(NULL)
, _nRecvBufferSize(DEFAULT_RECV_BUFFERSIZE)

{
    _iLastRefreshTime = TNOW;

    _sock.init(fd, false, pBindAdapter->_ep.isIPv6() ? AF_INET6 : AF_INET);
}

TC_EpollServer::Connection::~Connection()
{
    if (_pRecvBuffer)
    {
        delete _pRecvBuffer;
        _pRecvBuffer = NULL;
    }

    if (isTcp())
    {
        assert(!_sock.isValid());
    }
}

void TC_EpollServer::Connection::tryInitAuthState(int initState)
{
    if (!_authInit)
    {
        _authState = initState;
        _authInit = true;
    }
}

void TC_EpollServer::Connection::close()
{
#if TARS_SSL
        if (_openssl)
        {
            _openssl->release();
            _openssl.reset();
        }
#endif

    if (isTcp() && _sock.isValid())
    {
        _pBindAdapter->decreaseSendBufferSize(_sendBuffer.size());

        _sock.close();
    }
}

void TC_EpollServer::Connection::insertRecvQueue(const shared_ptr<TC_EpollServer::RecvContext> &recv)
{
    if(_pBindAdapter->getEpollServer()->isMergeHandleNetThread())
    {
        int index = _pBindAdapter->getEpollServer()->getNetThreadOfFd(recv->fd())->getIndex();

        //直接在网络线程中调用handle的process
        _pBindAdapter->getHandle(index)->process(recv);
    }
    else
    {
        int iRet = _pBindAdapter->isOverloadorDiscard();

        if (iRet == 0) //未过载
        {
            _pBindAdapter->insertRecvQueue(recv);
        }
        else if (iRet == -1) //超过接受队列长度的一半，需要进行overload处理
        {
            recv->setOverload();

            _pBindAdapter->insertRecvQueue(recv);//, false);
        }
        else //接受队列满，需要丢弃
        {
            _pBindAdapter->getEpollServer()->error("[Connection::insertRecvQueue] overload discard package");
        }
    }
}

int TC_EpollServer::Connection::parseProtocol(TC_NetWorkBuffer &rbuf)
{
    try
    {
        while (!rbuf.empty())
        {
            //需要过滤首包包头
            if(_iHeaderLen > 0)
            {
                if(rbuf.getBufferLength() >= (unsigned)_iHeaderLen)
                {
                    vector<char> header;
                    rbuf.getHeader(_iHeaderLen, header);
                    _pBindAdapter->getHeaderFilterFunctor()(TC_NetWorkBuffer::PACKET_FULL, header);
                    rbuf.moveHeader(_iHeaderLen);
                    _iHeaderLen = 0;
                }
                else
                {
                    vector<char> header = rbuf.getBuffers();
                    _pBindAdapter->getHeaderFilterFunctor()(TC_NetWorkBuffer::PACKET_LESS, header);
                    _iHeaderLen -= (int)rbuf.getBufferLength();
                    rbuf.clearBuffers();
                    break;
                }
            }

            vector<char> ro;

            TC_NetWorkBuffer::PACKET_TYPE b = _pBindAdapter->getProtocol()(rbuf, ro);

            if(b == TC_NetWorkBuffer::PACKET_LESS)
            {
                break;
            }
            else if(b == TC_NetWorkBuffer::PACKET_FULL)
            {
                shared_ptr<RecvContext> recv = std::make_shared<RecvContext>(getId(), _ip, _port, getfd(), _pBindAdapter);

                recv->buffer().swap(ro);

                if (_pBindAdapter->getEndpoint().isTcp() && _pBindAdapter->_authWrapper && _pBindAdapter->_authWrapper(this, recv))
                    continue;

                //收到完整的包才算
                this->_bEmptyConn = false;

                //收到完整包
                insertRecvQueue(recv);
            }
            else
            {
                _pBindAdapter->getEpollServer()->error("recv [" + _ip + ":" + TC_Common::tostr(_port) + "], packet parse error.");
                return -1; //协议解析错误
            }
        }
    }
    catch(exception &ex)
    {
        _pBindAdapter->getEpollServer()->error("recv protocol error:" + string(ex.what()));
        return -1;
    }
    catch(...)
    {
        _pBindAdapter->getEpollServer()->error("recv protocol error");
        return -1;
    }

    return 0;
}

int TC_EpollServer::Connection::recvTcp()
{
    int recvCount = 0;

    TC_NetWorkBuffer *rbuf = &_recvBuffer;

    while (true)
    {
        char buffer[BUFFER_SIZE] = {0x00};

        int iBytesReceived = _sock.recv((void *)buffer, BUFFER_SIZE);

        if (iBytesReceived < 0)
        {
            if (TC_Socket::isPending())
            {
                //没有数据了
                break;
            }
            else
            {
                //客户端主动关闭
                _pBindAdapter->getEpollServer()->debug("recv [" + _ip + ":" + TC_Common::tostr(_port) + "] close connection");
                return -1;
            }
        }
        else if (iBytesReceived == 0)
        {
            //客户端主动关闭
            _pBindAdapter->getEpollServer()->debug("recv [" + _ip + ":" + TC_Common::tostr(_port) + "] close connection");
            return -1;
        }
        else
        {

#if TARS_SSL
		    if (_pBindAdapter->getEndpoint().isSSL())
		    {
			    int ret = _openssl->read(buffer, iBytesReceived, _sendBuffer);
			    if (ret != 0)
			    {
				    _pBindAdapter->getEpollServer()->error("[SSL_read failed: " + _openssl->getErrMsg());
				    return -1;
			    }
			    else
			    {
				    if (!_sendBuffer.empty())
				    {
					    sendBuffer();
				    }

				    rbuf = _openssl->recvBuffer();
			    }
		    }
		    else
		    {
			    rbuf->addBuffer(buffer, iBytesReceived);
		    }

#else
            rbuf->addBuffer(buffer, iBytesReceived);
#endif

            //字符串太长时, 强制解析协议
            if (rbuf->getBufferLength() > 8192) {
                parseProtocol(*rbuf);
            }

            //接收到数据不超过buffer,没有数据了(如果有数据,内核会再通知你)
            if ((size_t) iBytesReceived < BUFFER_SIZE) {
                break;
            }

            if(++recvCount > 100)
            {
                //太多数据要接收,避免网络线程饥饿
                _pBindAdapter->getNetThreadOfFd(_sock.getfd())->getEpoller()->mod(_sock.getfd(), getId(), EPOLLIN|EPOLLOUT);
                break;
            }
        }
    }

    return parseProtocol(*rbuf);
}

int TC_EpollServer::Connection::recvUdp()
{
    assert(_pRecvBuffer != NULL);

    int recvCount = 0;
    while (true)
    {
        int iBytesReceived = _sock.recvfrom((void *)_pRecvBuffer, _nRecvBufferSize, _ip, _port, 0);

        if (iBytesReceived < 0)
        {
            if (TC_Socket::isPending())//errno == EAGAIN)
            {
                //没有数据了
                break;
            }
            else
            {
                //客户端主动关闭
                _pBindAdapter->getEpollServer()->debug("recv [" + _ip + ":" + TC_Common::tostr(_port) + "] close connection");
                return -1;
            }
        }
        else if (iBytesReceived == 0)
        {
            //客户端主动关闭
            _pBindAdapter->getEpollServer()->debug("recv [" + _ip + ":" + TC_Common::tostr(_port) + "] close connection");
            return -1;
        }
        else
        {

            if (_pBindAdapter->isIpAllow(_ip) == true)
            {
                //保存接收到数据
                _recvBuffer.addBuffer(_pRecvBuffer, iBytesReceived);

                parseProtocol(_recvBuffer);
            }
            else
            {
                //udp ip无权限
                _pBindAdapter->getEpollServer()->debug( "accept [" + _ip + ":" + TC_Common::tostr(_port) + "] [" + TC_Common::tostr(_lfd) + "] not allowed");
            }
            _recvBuffer.clearBuffers();

            if(++recvCount > 100)
            {
                //太多数据要接收,避免网络线程饥饿
                _pBindAdapter->getNetThreadOfFd(_sock.getfd())->getEpoller()->mod(_sock.getfd(), getId(), EPOLLIN|EPOLLOUT);
                break;
            }
        }
    }

    return 0;
}

int TC_EpollServer::Connection::recv()
{
    return isTcp() ? recvTcp() : recvUdp();
}

int TC_EpollServer::Connection::sendBuffer()
{
    if(!isTcp())
    {
        return 0;
    }

	size_t nowSendBufferSize = 0;
	size_t nowLeftBufferSize = _sendBuffer.getBufferLength();

	while(!_sendBuffer.empty())
	{
		pair<const char*, size_t> data = _sendBuffer.getBufferPointer();

		int iBytesSent = _sock.send((const void *) data.first, data.second);
		// }
		// else
		// {
		// 	iBytesSent = _sock.sendto((const void *) data.first, data.second, _ip, _port, 0);
		// }

		if (iBytesSent < 0)
		{
			if (TC_Socket::isPending())
			{
#if TARGET_PLATFORM_WINDOWS
	            _pBindAdapter->getNetThreadOfFd(_sock.getfd())->getEpoller()->mod(_sock.getfd(), getId(), EPOLLIN|EPOLLOUT);
#endif
				break;
			}
			else
			{
				_pBindAdapter->getEpollServer()->debug("send [" + _ip + ":" + TC_Common::tostr(_port) + "] close connection by peer.");
				return -1;
			}
		}

		if(iBytesSent > 0)
		{
			nowSendBufferSize += iBytesSent;

			if(isTcp())
			{
				_sendBuffer.moveHeader(iBytesSent);

				if (iBytesSent == (int)data.second)
				{
					_pBindAdapter->decreaseSendBufferSize();
				}
			}
			else
			{
				_sendBuffer.moveHeader(data.second);

				_pBindAdapter->decreaseSendBufferSize();

			}
		}

		//发送的数据小于需要发送的,break, 内核会再通知你的
		if (iBytesSent < (int)data.second)
		{
			break;
		}
	}

	//需要关闭链接
	if (_bClose && _sendBuffer.empty())
	{
		_pBindAdapter->getEpollServer()->debug("send [" + _ip + ":" + TC_Common::tostr(_port) + "] close connection by user.");
		return -2;
	}

//	当出现队列积压的前提下, 且积压超过一定大小
//	每5秒检查一下积压情况, 连续12次(一分钟), 都是积压
//	且每个检查点, 积压长度都增加或者连续3次发送buffer字节小于1k, 就关闭连接, 主要避免极端情况

	size_t iBackPacketBuffLimit = _pBindAdapter->getBackPacketBuffLimit();
	if(_sendBuffer.getBufferLength() > iBackPacketBuffLimit)
	{
		if(_sendBufferSize == 0)
		{
			//开始积压
			_lastCheckTime = TNOW;
		}
		_sendBufferSize += nowSendBufferSize;

		if (TNOW - _lastCheckTime >= 5)
		{
			//如果持续有积压, 则每5秒检查一次
			_lastCheckTime = TNOW;

			_checkSend.push_back(make_pair(_sendBufferSize, nowLeftBufferSize));

			_sendBufferSize = 0;

			size_t iBackPacketBuffMin = _pBindAdapter->getBackPacketBuffMin();

			//连续3个5秒, 发送速度都极慢, 每5秒发送 < iBackPacketBuffMin, 认为连接有问题, 关闭之
			int left = 3;
			if ((int)_checkSend.size() >= left)
			{
				bool slow = true;
				for (int i = (int)_checkSend.size() - 1; i >= (int)(_checkSend.size() - left); i--)
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

					_pBindAdapter->getEpollServer()->error(os.str());
					_sendBuffer.clearBuffers();
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

					_pBindAdapter->getEpollServer()->error(os.str());
					_sendBuffer.clearBuffers();
					return -4;
				}

				_checkSend.erase(_checkSend.begin());
			}
		}
	}
	else
	{
		//无积压
		_sendBufferSize = 0;
		_lastCheckTime  = TNOW;
		_checkSend.clear();
	}

	return 0;

}


int TC_EpollServer::Connection::sendBufferDirect(const std::string& buff)
{
    _pBindAdapter->increaseSendBufferSize();
    
    if(getBindAdapter()->getEndpoint().isTcp())
    {
#if TAF_SSL
        if (getBindAdapter()->getEndpoint().isSSL())
        {
            //assert(_openssl->isHandshaked());
            
            int ret = _openssl->write(buff.c_str(), buff.length(), _sendBuffer);
            if (ret != 0)
            {
                _pBindAdapter->getEpollServer()->error("[TC_EpollServer::Connection] send direct error! " + TC_Common::tostr(ret));
                return -1; // should not happen
            }
    
        }
        else
#endif
        {
            _sendBuffer.addBuffer(buff);
        }

        return sendBuffer();
    }
    else
    {
        _pBindAdapter->getEpollServer()->error("[TC_EpollServer::Connection] send direct not support udp! ");
        return -2;
    }
}

int TC_EpollServer::Connection::sendBufferDirect(const std::vector<char>& buff)
{
    _pBindAdapter->increaseSendBufferSize();
    
    if(getBindAdapter()->getEndpoint().isTcp())
    {
#if TAF_SSL
        if (getBindAdapter()->getEndpoint().isSSL())
        {
            //assert(_openssl->isHandshaked());
            
            int ret = _openssl->write(&buff[0], buff.size(), _sendBuffer);
            if (ret != 0)
            {
                _pBindAdapter->getEpollServer()->error("[TC_EpollServer::Connection] send direct error! " + TC_Common::tostr(ret));
                return -1; // should not happen
            }
    
        }
        else
#endif
        {
            _sendBuffer.addBuffer(buff);
        }

        return sendBuffer();
    }
    else
    {
        _pBindAdapter->getEpollServer()->error("[TC_EpollServer::Connection] send direct not support udp! ");
        return -2;
    }
}

int TC_EpollServer::Connection::send(const shared_ptr<SendContext> &sc)
{
	assert(sc);

	_pBindAdapter->increaseSendBufferSize();

    if(getBindAdapter()->getEndpoint().isTcp())
    {
    #if TARS_SSL
        if (getBindAdapter()->getEndpoint().isSSL())
        {
            assert(_openssl->isHandshaked());

            int ret = _openssl->write(sc->buffer()->buffer(), sc->buffer()->length(), _sendBuffer);
            if (ret != 0) 
            {
                _pBindAdapter->getEpollServer()->error("[TC_EpollServer::Connection] send [" + _ip + ":" + TC_Common::tostr(_port) + "] error:" + _openssl->getErrMsg());

                return -1; // should not happen
            }
        }
        else
    #endif
        {
            _sendBuffer.addBuffer(sc->buffer());
        }

        return sendBuffer();
    }
    else
    {
        //注意udp, 回包时需要带上请求包的ip, port的
		int iRet = _sock.sendto((const void *) sc->buffer()->buffer(), sc->buffer()->length(), sc->ip(), sc->port(), 0);
        if (iRet < 0)
        {
            _pBindAdapter->getEpollServer()->error("[TC_EpollServer::Connection] send udp [" + _ip + ":" + TC_Common::tostr(_port) + "] error");
            return -1;
        }
	}
    return 0;
}

bool TC_EpollServer::Connection::setRecvBuffer(size_t nSize)
{
    //only udp type needs to malloc
    if (!isTcp() && !_pRecvBuffer)
    {
        _nRecvBufferSize = nSize;

        _pRecvBuffer = new char[_nRecvBufferSize];
        if (!_pRecvBuffer)
        {
            throw TC_Exception("adapter '" + _pBindAdapter->getName() + "' malloc udp receive buffer fail");
        }
    }
    return true;
}

bool TC_EpollServer::Connection::setClose()
{
    _bClose = true;
    if (_sendBuffer.empty())
        return true;
    else
        return false;
}

////////////////////////////////////////////////////////////////
//
TC_EpollServer::ConnectionList::ConnectionList(TC_EpollServer::NetThread *pEpollServer)
: _pEpollServer(pEpollServer)
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

    _iConnectionMagic   = ((((uint32_t)_lastTimeoutTime) << 26) & (0xFFFFFFFF << 26)) + ((iIndex << 22) & (0xFFFFFFFF << 22));//((uint32_t)_lastTimeoutTime) << 20;

    //free从1开始分配, 这个值为uid, 0保留为管道用, epollwait根据0判断是否是管道消息
    for(uint32_t i = 1; i <= _total; i++)
    {
        _vConn[i].first = NULL;

        _free.push_back(i);

        ++_free_size;
    }
}

uint32_t TC_EpollServer::ConnectionList::getUniqId()
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);

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
    TC_LockT<TC_ThreadMutex> lock(_mutex);

    uint32_t muid = cPtr->getId();
    uint32_t magi = muid & (0xFFFFFFFF << 22);
    uint32_t uid  = muid & (0x7FFFFFFF >> 9);

    assert(magi == _iConnectionMagic && uid > 0 && uid <= _total && !_vConn[uid].first);

    _vConn[uid] = make_pair(cPtr, _tl.insert(make_pair(iTimeOutStamp, uid)));
}

void TC_EpollServer::ConnectionList::refresh(uint32_t uid, time_t iTimeOutStamp)
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);

    uint32_t magi = uid & (0xFFFFFFFF << 22);
    uid           = uid & (0x7FFFFFFF >> 9);

    assert(magi == _iConnectionMagic && uid > 0 && uid <= _total && _vConn[uid].first);

    if(iTimeOutStamp - _vConn[uid].first->_iLastRefreshTime < 1)
    {
        return;
    }

    _vConn[uid].first->_iLastRefreshTime = iTimeOutStamp;

    //删除超时链表
    _tl.erase(_vConn[uid].second);

    _vConn[uid].second = _tl.insert(make_pair(iTimeOutStamp, uid));
}

void TC_EpollServer::ConnectionList::checkTimeout(time_t iCurTime)
{
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
        if(_vConn[uid].first->getListenfd() == -1)
        {
            continue;
        }

        //超时关闭
        _pEpollServer->delConnection(_vConn[uid].first, false, EM_SERVER_TIMEOUT_CLOSE);

        //从链表中删除
        _del(uid);
    }

    if(_pEpollServer->isEmptyConnCheck())
    {
        it = _tl.begin();
        while(it != _tl.end())
        {
            uint32_t uid = it->second;

            //遍历所有的空连接
            if(_vConn[uid].first->isEmptyConn())
            {
                //获取空连接的超时时间点
                time_t iEmptyTimeout = (it->first - _vConn[uid].first->getTimeout()) + (_pEpollServer->getEmptyConnTimeout()/1000);

                //已经检查到当前时间点了, 后续不用在检查了
                if(iEmptyTimeout > iCurTime)
                {
                    break;
                }

                //udp的监听端口, 不做处理
                if(_vConn[uid].first->getListenfd() == -1)
                {
                    ++it;
                    continue;
                }

                //超时关闭
                _pEpollServer->delConnection(_vConn[uid].first, false, EM_SERVER_TIMEOUT_CLOSE);

                //从链表中删除
                _del(uid);
            }

            ++it;
        }
    }
}

vector<TC_EpollServer::ConnStatus> TC_EpollServer::ConnectionList::getConnStatus(int lfd)
{
    vector<TC_EpollServer::ConnStatus> v;

	TC_LockT<TC_ThreadMutex> lock(_mutex);

    for(size_t i = 1; i <= _total; i++)
    {
        //是当前监听端口的连接
        if(_vConn[i].first != NULL && _vConn[i].first->getListenfd() == lfd)
        {
            TC_EpollServer::ConnStatus cs;

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

    _del(uid);
}

void TC_EpollServer::ConnectionList::_del(uint32_t uid)
{
    assert(uid > 0 && uid <= _total && _vConn[uid].first);

    _tl.erase(_vConn[uid].second);

    delete _vConn[uid].first;

    _vConn[uid].first = NULL;

    _free.push_back(uid);

    ++_free_size;
}

size_t TC_EpollServer::ConnectionList::size()
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);

    return _total - _free_size;
}

//////////////////////////////NetThread//////////////////////////////////
TC_EpollServer::NetThread::NetThread(TC_EpollServer *epollServer, int threadIndex)
: _epollServer(epollServer)
, _threadIndex(threadIndex)
, _bTerminate(false)
, _list(this)
, _bEmptyConnAttackCheck(false)
, _iEmptyCheckTimeout(MIN_EMPTY_CONN_TIMEOUT)
, _nUdpRecvBufferSize(DEFAULT_RECV_BUFFERSIZE)
{
    _epoller.create(10240);

    _notify.init(&_epoller);
    _notify.add(_notify.notifyFd());
}

TC_EpollServer::NetThread::~NetThread()
{
}

void TC_EpollServer::NetThread::debug(const string &s) const
{
    _epollServer->debug(s);
}

void TC_EpollServer::NetThread::info(const string &s) const
{
    _epollServer->info(s);
}

void TC_EpollServer::NetThread::tars(const string &s) const
{
	_epollServer->tars(s);
}

void TC_EpollServer::NetThread::error(const string &s) const
{
    _epollServer->error(s);
}

void TC_EpollServer::NetThread::enAntiEmptyConnAttack(bool bEnable)
{
    _bEmptyConnAttackCheck = bEnable;
}

void TC_EpollServer::NetThread::setEmptyConnTimeout(int timeout)
{
    _iEmptyCheckTimeout = (timeout >= MIN_EMPTY_CONN_TIMEOUT) ? timeout : MIN_EMPTY_CONN_TIMEOUT;
}

void TC_EpollServer::NetThread::setUdpRecvBufferSize(size_t nSize)
{
    _nUdpRecvBufferSize = (nSize >= 8192 && nSize <= DEFAULT_RECV_BUFFERSIZE) ? nSize : DEFAULT_RECV_BUFFERSIZE;
}

bool TC_EpollServer::NetThread::isEmptyConnCheck() const
{
    return _bEmptyConnAttackCheck;
}

int TC_EpollServer::NetThread::getEmptyConnTimeout() const
{
    return _iEmptyCheckTimeout;
}

void TC_EpollServer::NetThread::createEpoll(uint32_t maxAllConn)
{
    _list.init((uint32_t)maxAllConn, _threadIndex + 1);
}

void TC_EpollServer::NetThread::initUdp(const unordered_map<int, TC_EpollServer::BindAdapterPtr> &listeners)
{
    //监听socket
    auto it = listeners.begin();

    while (it != listeners.end())
    {
        if (!it->second->getEndpoint().isTcp())
        {
            Connection *cPtr = new Connection(it->second.get(), it->first);
            //udp分配接收buffer
            cPtr->setRecvBuffer(_nUdpRecvBufferSize);

            //addUdpConnection(cPtr);
            _epollServer->addConnection(cPtr, it->first, UDP_CONNECTION);
        }

        ++it;
    }
}

void TC_EpollServer::NetThread::terminate()
{
    _bTerminate = true;

    _notify.notify();
}

void TC_EpollServer::NetThread::addTcpConnection(TC_EpollServer::Connection *cPtr)
{
    uint32_t uid = _list.getUniqId();

    cPtr->init(uid);

    _list.add(cPtr, cPtr->getTimeout() + TNOW);

    cPtr->getBindAdapter()->increaseNowConnection();

#if TARS_SSL
    if (cPtr->getBindAdapter()->getEndpoint().isSSL())
    {
        cPtr->getBindAdapter()->getEpollServer()->info("[TARS][addTcpConnection ssl connection");

        // 分配ssl对象, ctxName 放在obj proxy里
        cPtr->_openssl = TC_OpenSSL::newSSL(cPtr->getBindAdapter()->_ctx);
        if (!cPtr->_openssl)
        {
            cPtr->getBindAdapter()->getEpollServer()->error("[TARS][SSL_accept not find server cert");
	        cPtr->close();
            return;
        }

        cPtr->_openssl->recvBuffer()->setConnection(cPtr);
        cPtr->_openssl->init(true);
        cPtr->_openssl->setReadBufferSize(1024 * 8);
        cPtr->_openssl->setWriteBufferSize(1024 * 8);

        int ret = cPtr->_openssl->doHandshake(cPtr->_sendBuffer);
        if (ret != 0)
        {
            cPtr->getBindAdapter()->getEpollServer()->error("[TARS][SSL_accept " + cPtr->getBindAdapter()->getEndpoint().toString() + " error: " + cPtr->_openssl->getErrMsg());
	        cPtr->close();
            return;
        }

        // send the encrypt data from write buffer
        if (!cPtr->_sendBuffer.empty())
        {
            cPtr->sendBuffer();
        }
    }
#endif
    //注意epoll add必须放在最后, 否则可能导致执行完, 才调用上面语句
    _epoller.add(cPtr->getfd(), cPtr->getId(), EPOLLIN | EPOLLOUT);
}

void TC_EpollServer::NetThread::addUdpConnection(TC_EpollServer::Connection *cPtr)
{
    uint32_t uid = _list.getUniqId();

    cPtr->init(uid);

    _list.add(cPtr, cPtr->getTimeout() + TNOW);

    _epoller.add(cPtr->getfd(), cPtr->getId(), EPOLLIN | EPOLLOUT);
}

vector<TC_EpollServer::ConnStatus> TC_EpollServer::NetThread::getConnStatus(int lfd)
{
    return _list.getConnStatus(lfd);
}

void TC_EpollServer::NetThread::delConnection(TC_EpollServer::Connection *cPtr, bool bEraseList, EM_CLOSE_T closeType)
{
    //如果是TCP的连接才真正的关闭连接
    if (cPtr->getListenfd() != -1)
    {
        BindAdapterPtr adapter = cPtr->getBindAdapter();

        //false的情况,是超时被主动删除
        if(!bEraseList)
        {
            tars("timeout [" + cPtr->getIp() + ":" + TC_Common::tostr(cPtr->getPort()) + "] del from list");
        }

        uint32_t uid = cPtr->getId();

        //构造一个tagRecvData，通知业务该连接的关闭事件
        shared_ptr<RecvContext> recv   = std::make_shared<RecvContext>(uid, cPtr->getIp(), cPtr->getPort(), cPtr->getfd(), adapter, true, (int)closeType);

        //如果是merge模式，则close直接交给网络线程处理
        if (_epollServer->isMergeHandleNetThread())
        {
            cPtr->insertRecvQueue(recv);
        }
        else
        {
            cPtr->getBindAdapter()->insertRecvQueue(recv);
        }

        //从epoller删除句柄放在close之前, 否则重用socket时会有问题
        _epoller.del(cPtr->getfd(), uid, 0);

        cPtr->close();

        //对于超时检查, 由于锁的原因, 在这里不从链表中删除
        if(bEraseList)
        {
            _list.del(uid);
        }
        
        adapter->decreaseNowConnection();
    }
}

void TC_EpollServer::NetThread::notify()
{
    _notify.notify();
}

void TC_EpollServer::NetThread::close(const shared_ptr<RecvContext> &data)
{
    shared_ptr<SendContext> send = data->createCloseContext();

    _sbuffer.push_back(send);

//	通知epoll响应, 关闭连接
    _notify.notify();
}

void TC_EpollServer::NetThread::send(const shared_ptr<SendContext> &data)
{
    if(_threadId == std::this_thread::get_id()) {
        //发送包线程和网络线程是同一个线程,直接发送即可
        Connection *cPtr = getConnectionPtr(data->uid());
        if(cPtr)
        {
            cPtr->send(data);
        }
    }
    else {
        //发送包线程和网络线程不是同一个线程, 需要先放队列, 再唤醒网络线程去发送
        _sbuffer.push_back(data);

        //通知epoll响应, 有数据要发送
        if (!_notifySignal) {
            _notifySignal = true;
            _notify.notify();
        }
    }
}

void TC_EpollServer::NetThread::processPipe()
{
    _notifySignal = false;

    while(!_sbuffer.empty())
    {
        shared_ptr<SendContext> sc = _sbuffer.front();
        Connection *cPtr = getConnectionPtr(sc->uid());

        if (!cPtr)
        {
            _sbuffer.pop_front();
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
                int ret = 0;
#if TARS_SSL
                if (cPtr->getBindAdapter()->getEndpoint().isSSL()) {
                    if (!cPtr->_openssl->isHandshaked()) {
                        return;
                    }
                }
                ret = cPtr->send(sc);
#else
                ret = cPtr->send(sc);
#endif
                if (ret < 0)
                {
                    delConnection(cPtr, true, (ret == -1) ? EM_CLIENT_CLOSE : EM_SERVER_CLOSE);
                }
                else
                {
                    _list.refresh(sc->uid(), cPtr->getTimeout() + TNOW);
                }
                break;
            }
            default:
                assert(false);
        }
        _sbuffer.pop_front();
    }
}

void TC_EpollServer::NetThread::processNet(const epoll_event &ev)
{
    uint32_t uid = TC_Epoller::getU32(ev, false);

    Connection *cPtr = getConnectionPtr(uid);

    if(!cPtr)
    {
        debug("TC_EpollServer::NetThread::processNet connection[" + TC_Common::tostr(uid) + "] not exists.");
        return;
    }

    if (TC_Epoller::errorEvent(ev))
    {
        delConnection(cPtr, true, EM_SERVER_CLOSE);
        return;
    }

    if (TC_Epoller::readEvent(ev))
    {
        int ret = cPtr->recv();
        if (ret < 0)
        {
            delConnection(cPtr, true, EM_CLIENT_CLOSE);

            return;
        }
    }

    if (TC_Epoller::writeEvent(ev))
    {
        int ret = cPtr->sendBuffer();
        if (ret < 0)
        {
            delConnection(cPtr, true, (ret == -1) ? EM_CLIENT_CLOSE : EM_SERVER_CLOSE);

            return;
        }
    }

    _list.refresh(uid, cPtr->getTimeout() + TNOW);
}

void TC_EpollServer::NetThread::run()
{
    _threadId = std::this_thread::get_id();

    if(_epollServer->isMergeHandleNetThread()) {

        vector<TC_EpollServer::BindAdapterPtr> adapters = _epollServer->getBindAdapters();

        for (auto adapter : adapters)
        {
            adapter->getHandle(_threadIndex)->setNetThread(this);
            adapter->getHandle(_threadIndex)->initialize();
        }
    }

    //循环监听网路连接请求
    while(!_bTerminate)
    {
        _list.checkTimeout(TNOW);

        int iEvNum = _epoller.wait(1000);
        //没有网络事件
        if (iEvNum == 0)
        {
            //在这里加上心跳逻辑，获取所有的bindAdpator,然后发心跳
            if (_epollServer->isMergeHandleNetThread()) 
            {
                vector<TC_EpollServer::BindAdapterPtr> adapters = _epollServer->getBindAdapters();
                for (auto adapter : adapters)
                {
                    adapter->getHandle(_threadIndex)->heartbeat();
                }
            }
        }

        if (_bTerminate)
            break;

        for (int i = 0; i < iEvNum; ++i)
        {
            try
            {
                const epoll_event &ev = _epoller.get(i);

                uint32_t fd = TC_Epoller::getU32(ev, false);

                if (fd == (uint32_t)_notify.notifyFd())
                {
                    //检查是否是通知消息
                    processPipe();
                }
                else
                {
                    processNet(ev);
                }
            }
            catch (exception &ex)
            {
                error("run exception:" + string(ex.what()));
            }
        }
    }
}

//size_t TC_EpollServer::NetThread::getSendRspSize()
//{
//    return _sbuffer.size();
//}
//////////////////////////////////////////////////////////////
TC_EpollServer::TC_EpollServer(unsigned int iNetThreadNum)
: _netThreadNum(iNetThreadNum)
, _bTerminate(false)
, _handleStarted(false)
, _pLocalLogger(NULL)
, _acceptFunc(NULL)
{
#if TARGET_PLATFORM_WINDOWS    
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif

    if(_netThreadNum < 1)
    {
        _netThreadNum = 1;
    }

    //网络线程的配置数目不能15个
    if(_netThreadNum > 15)
    {
        _netThreadNum = 15;
    }

    //创建epoll
    _epoller.create(10240);

    _notify.init(&_epoller);
    _notify.add(_notify.notifyFd());

    for (size_t i = 0; i < _netThreadNum; ++i)
    {
        TC_EpollServer::NetThread* netThreads = new TC_EpollServer::NetThread(this, i);
        _netThreads.push_back(netThreads);
    }
}

TC_EpollServer::~TC_EpollServer()
{
    terminate();

    for (size_t i = 0; i < _netThreadNum; ++i)
    {
        delete _netThreads[i];
    }

    _netThreads.clear();

    _netThreadNum = 0;

    auto it = _listeners.begin();

    while (it != _listeners.end())
    {
        TC_Port::closeSocket(it->first);
        ++it;
    }
    _listeners.clear();

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
    struct sockaddr* stSockAddr = (AF_INET6 == domain) ? (struct sockaddr*) & stSockAddr6 : (struct sockaddr*) & stSockAddr4;

    TC_Socket cs;

    cs.setOwner(false);

    //接收连接
    TC_Socket s;

    s.init(fd, false, domain);
    int iRetCode = s.accept(cs, (struct sockaddr *)stSockAddr, iSockAddrSize);
    if (iRetCode > 0)
    {
        string ip;
        uint16_t port;

        char sAddr[INET6_ADDRSTRLEN] = "\0";

        inet_ntop(domain, (AF_INET6 == domain) ? ( void *)&stSockAddr6.sin6_addr : ( void *)&stSockAddr4.sin_addr, sAddr, sizeof(sAddr));
        port = (AF_INET6 == domain) ? ntohs(stSockAddr6.sin6_port) : ntohs(stSockAddr4.sin_port);
        ip = sAddr;
        
        debug("accept [" + ip + ":" + TC_Common::tostr(port) + "] [" + TC_Common::tostr(cs.getfd()) + "] incomming");

        if (!_listeners[fd]->isIpAllow(ip))
        {
            debug("accept [" + ip + ":" + TC_Common::tostr(port) + "] [" + TC_Common::tostr(cs.getfd()) + "] not allowed");

            cs.close();

            return true;
        }

        if (_listeners[fd]->isLimitMaxConnection())
        {
            error("accept [" + ip + ":" + TC_Common::tostr(port) + "][" + TC_Common::tostr(cs.getfd()) + "] beyond max connection:" + TC_Common::tostr(_listeners[fd]->getMaxConns()));
            cs.close();

            return true;
        }

        cs.setblock(false);
        cs.setKeepAlive();
        cs.setTcpNoDelay();
        cs.setCloseWaitDefault();

        int timeout = _listeners[fd]->getEndpoint().getTimeout() / 1000;

        Connection *cPtr = new Connection(_listeners[fd].get(), fd, (timeout < 2 ? 2 : timeout), cs.getfd(), ip, port);

        //过滤连接首个数据包包头
        cPtr->setHeaderFilterLen((int)_listeners[fd]->getHeaderFilterLen());

        addConnection(cPtr, cs.getfd(), TCP_CONNECTION);

        return true;
    }
    else
    {
        // //直到发生EAGAIN才不继续accept
        // if (TC_Socket::isPending())
        // {
        //     return false;
        // }

        return false;
    }
    return true;
}

void TC_EpollServer::waitForShutdown()
{

    if(!isMergeHandleNetThread())
        startHandle();

    createEpoll();

    for (size_t i = 0; i < _netThreadNum; ++i)
    {
        _netThreads[i]->start();
    }

    int64_t iLastCheckTime = TNOWMS;

    while (!_bTerminate)
    {
        int iEvNum = _epoller.wait(300);

        if (_bTerminate)
            break;

        if(TNOWMS - iLastCheckTime > 1000) {
            try { _hf(this); } catch(...) {}
            iLastCheckTime = TNOWMS;
        }

        for (int i = 0; i < iEvNum; ++i)
        {
            try
            {
                const epoll_event &ev = _epoller.get(i);

                uint32_t fd = TC_Epoller::getU32(ev, false);

                auto it = _listeners.find(fd);

                if (it != _listeners.end())
                {
                    //manualListen 会进入这个场景
                    if (TC_Epoller::writeEvent(ev))
                    {
                        TC_Socket s;
                        s.init(fd, false);
                        s.listen(1024);

                        debug("run listen fd: " + TC_Common::tostr(fd));
                    }

                    //监听端口有请求
                    if (TC_Epoller::readEvent(ev))
                    {
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS

                        bool ret;
                        do
                        {

                            ret = accept(fd, it->second->_ep.isIPv6() ? AF_INET6 : AF_INET);
                        } while (ret);
#else

                        accept(fd, it->second->_ep.isIPv6() ? AF_INET6 : AF_INET);
#endif
                    }
                }
            }
            catch (exception &ex)
            {
                error("run exception:" + string(ex.what()));
            }
            catch (...)
            {
                error("TC_EpollServer::waitForShutdown unknown error");
            }
        }
    }

    for (size_t i = 0; i < _netThreads.size(); ++i)
    {
        if (_netThreads[i]->isAlive())
        {
            _netThreads[i]->terminate();

            _netThreads[i]->getThreadControl().join();
        }
    }
    
}

void TC_EpollServer::terminate()
{
    if (!_bTerminate)
    {
        _bTerminate = true;

        //停掉处理线程
        stopThread();

        //停掉主线程(waitForShutdown)
        _notify.notify();
    }
}

void TC_EpollServer::enAntiEmptyConnAttack(bool bEnable)
{
    for(size_t i = 0; i < _netThreads.size(); ++i)
    {
        _netThreads[i]->enAntiEmptyConnAttack(bEnable);
    }
}

void TC_EpollServer::setEmptyConnTimeout(int timeout)
{
    for(size_t i = 0; i < _netThreads.size(); ++i)
    {
        _netThreads[i]->setEmptyConnTimeout(timeout);
    }
}

void TC_EpollServer::bind(const TC_Endpoint &ep, TC_Socket &s, bool manualListen)
{
#if TARGET_PLATFORM_WINDOWS
    int type = ep.isIPv6() ? AF_INET6 : AF_INET;
#else
    int type = ep.isUnixLocal() ? AF_LOCAL : ep.isIPv6() ? AF_INET6 : AF_INET;
#endif

#if TARGET_PLATFORM_LINUX
    if (ep.isTcp())
    {
        s.createSocket(SOCK_STREAM | SOCK_CLOEXEC, type);
    }
    else
    {
        s.createSocket(SOCK_DGRAM | SOCK_CLOEXEC, type);
    }
#else
	if (ep.isTcp())
	{
		s.createSocket(SOCK_STREAM, type);
	}
	else
	{
		s.createSocket(SOCK_DGRAM, type);
	}
#endif

#if TARGET_PLATFORM_WINDOWS
    s.bind(ep.getHost(), ep.getPort());
    if (ep.isTcp())
#else
    if (ep.isUnixLocal())
    {
        s.bind(ep.getHost().c_str());
    }
    else
    {
        s.bind(ep.getHost(), ep.getPort());
    }
    if (ep.isTcp() && !ep.isUnixLocal())

#endif
    {
        if(!manualListen)
        {
            //手工监听
            s.listen(10240);
        }
        s.setKeepAlive();
        s.setTcpNoDelay();

        //不要设置close wait否则http服务回包主动关闭连接会有问题
        s.setNoCloseWait();
    }

    s.setblock(false);
}

int TC_EpollServer::bind(BindAdapterPtr &lsPtr)
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

    const TC_Endpoint &ep = lsPtr->getEndpoint();

    TC_Socket &s = lsPtr->getSocket();

    bind(ep, s, lsPtr->isManualListen());

    _listeners[s.getfd()] = lsPtr;

    _bindAdapters.push_back(lsPtr);

    return s.getfd();
}

void TC_EpollServer::addConnection(TC_EpollServer::Connection *cPtr, int fd, TC_EpollServer::CONN_TYPE iType)
{
    TC_EpollServer::NetThread* netThread = getNetThreadOfFd(fd);

    if(iType == TCP_CONNECTION)
    {
        netThread->addTcpConnection(cPtr);
    }
    else
    {
        netThread->addUdpConnection(cPtr);
    }
    // 回调
    if (_acceptFunc != NULL)
    {
        _acceptFunc(cPtr);
    }
}

void TC_EpollServer::startHandle()
{
    if(!this->isMergeHandleNetThread())
    {
        if (!_handleStarted)
        {
            _handleStarted = true;

            for (auto & bindAdapter : _bindAdapters)
            {
                const vector<TC_EpollServer::HandlePtr> & hds = bindAdapter->getHandles();

                for (uint32_t i = 0; i < hds.size(); ++i) {
                    if (!hds[i]->isAlive()) {
                        hds[i]->start();
                    }
                }
            }
        }
    }
}

void TC_EpollServer::stopThread()
{
    if(!this->isMergeHandleNetThread())
    {
        for (auto & bindAdapter : _bindAdapters)
        {
            const vector<TC_EpollServer::HandlePtr> & hds = bindAdapter->getHandles();

            //把处理线程都唤醒
            if(!bindAdapter->isQueueMode()) {
                bindAdapter->notifyHandle(0);
            }
            else {
                for (uint32_t i = 0; i < hds.size(); i++) {
                    bindAdapter->notifyHandle(i);
                }
            }

            for (uint32_t i = 0; i < hds.size(); ++i) {
                if (hds[i]->isAlive()) {
                    hds[i]->getThreadControl().join();
                }
            }
        }
    }
}

void TC_EpollServer::createEpoll()
{

    uint32_t maxAllConn = 0;

    //监听socket
    auto it = _listeners.begin();

    while (it != _listeners.end())
    {
        if (it->second->getEndpoint().isTcp())
        {
            //获取最大连接数
            maxAllConn += it->second->getMaxConns();

            _epoller.add(it->first, it->first, EPOLLIN);
        }
        else
        {
            maxAllConn++;
        }

        ++it;
    }

    if (maxAllConn >= (1 << 22))
    {
        error("createEpoll connection num: " + TC_Common::tostr(maxAllConn) + " >= " + TC_Common::tostr(1 << 22));
        maxAllConn = (1 << 22) - 1;
    }

    for (size_t i = 0; i < _netThreads.size(); ++i)
    {
        _netThreads[i]->createEpoll(maxAllConn);
    }

    //必须先等所有网络线程调用createEpoll()，初始化list后，才能调用initUdp()
    for (size_t i = 0; i < _netThreads.size(); ++i)
    {
        _netThreads[i]->initUdp(_listeners);
    }
}

TC_EpollServer::BindAdapterPtr TC_EpollServer::getBindAdapter(const string &sName)
{
    auto it = _listeners.begin();

    while (it != _listeners.end())
    {
        if (it->second->getName() == sName)
        {
            return it->second;
        }

        ++it;
    }
    return NULL;
}

vector<TC_EpollServer::BindAdapterPtr> TC_EpollServer::getBindAdapters()
{
    return this->_bindAdapters;
}

void TC_EpollServer::close(const shared_ptr<TC_EpollServer::RecvContext> &data)
{
    TC_EpollServer::NetThread *netThread = getNetThreadOfFd(data->fd());

    netThread->close(data);
}

void TC_EpollServer::send(const shared_ptr<SendContext> &data)
{
    TC_EpollServer::NetThread *netThread = getNetThreadOfFd(data->fd());

    netThread->send(data);
}

void TC_EpollServer::debug(const string &s) const
{
    if(_pLocalLogger)
    {
        _pLocalLogger->debug() << "[TARS]" << s << endl;
    }
}

void TC_EpollServer::info(const string &s) const
{
    if(_pLocalLogger)
    {
        _pLocalLogger->info() << "[TARS]" << s << endl;
    }
}

void TC_EpollServer::tars(const string &s) const
{
	if(_pLocalLogger)
	{
		_pLocalLogger->tars() << "[TARS]" << s << endl;
	}
}

void TC_EpollServer::error(const string &s) const
{
    if(_pLocalLogger)
    {
        _pLocalLogger->error() << "[TARS]" << s << endl;
    }
}

vector<TC_EpollServer::ConnStatus> TC_EpollServer::getConnStatus(int lfd)
{
    vector<TC_EpollServer::ConnStatus> vConnStatus;
    for(size_t i = 0; i < _netThreads.size(); ++i)
    {
        vector<TC_EpollServer::ConnStatus> tmp = _netThreads[i]->getConnStatus(lfd);
        for(size_t k = 0; k < tmp.size(); ++k)
        {
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
    for(size_t i = 0; i < _netThreads.size(); ++i)
    {
        iConnTotal += _netThreads[i]->getConnectionCount();
    }
    return iConnTotal;
}

size_t TC_EpollServer::getLogicThreadNum()
{
    if(this->isMergeHandleNetThread())
    {
        return this->_netThreadNum;
    }
    else
    {

        size_t iNum = 0;

        for (auto & bindAdapter : _bindAdapters) {
            iNum += bindAdapter->getHandles().size();
        }
        return iNum;
    }
}

}
