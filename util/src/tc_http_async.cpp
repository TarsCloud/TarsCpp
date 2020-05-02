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

#include "util/tc_http_async.h"
#include "util/tc_common.h"
#include "util/tc_timeprovider.h"

namespace tars
{

TC_HttpAsync::AsyncRequest::AsyncRequest(TC_HttpRequest &stHttpRequest, TC_HttpAsync::RequestCallbackPtr &callbackPtr, bool bUseProxy)
: _pHttpAsync(NULL)
, _iUniqId(0)
, _sendBuffer(this)
, _recvBuffer(this)
, _callbackPtr(callbackPtr)
, _bUseProxy(bUseProxy)
, _isConnected(false)
{
    memset(&_bindAddr, 0, sizeof(struct sockaddr));
    _bindAddrSet = false;

    vector<char> buff;
    stHttpRequest.encode(buff);

    _sendBuffer.addBuffer(std::move(buff));

    stHttpRequest.getHostPort(_sHost, _iPort);
}

TC_HttpAsync::AsyncRequest::AsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const string &addr)
: _pHttpAsync(NULL)
, _iUniqId(0)
, _sendBuffer(this)
, _recvBuffer(this)
, _callbackPtr(callbackPtr)
, _bUseProxy(false)
, _isConnected(false)
{
    memset(&_bindAddr, 0, sizeof(struct sockaddr));

    _bindAddrSet = false;

	stHttpRequest.encode(_sendBuffer);

    vector<string> v = TC_Common::sepstr<string>(addr, ":");

    if (v.size() < 2)
    {
        stHttpRequest.getHostPort(_sHost, _iPort);    
    }
    else
    {
        _sHost = v[0];
        _iPort = TC_Common::strto<uint32_t>(v[1]);    
    }
}

TC_HttpAsync::AsyncRequest::~AsyncRequest()
{
    doClose();
}

void TC_HttpAsync::AsyncRequest::doClose()
{
    if (_pHttpAsync) _pHttpAsync->assertThreadId();

    if (_fd.isValid())
    {
        try { if (_callbackPtr) _callbackPtr->onClose(); } catch (...) {}
        if (_pHttpAsync)
        {
            _pHttpAsync->delConnection(_fd.getfd());
            _pHttpAsync->erase(_iUniqId);
        }
        _fd.close();
    }
}

void TC_HttpAsync::AsyncRequest::setBindAddr(const struct sockaddr* addr)
{
    memcpy(&_bindAddr, addr, sizeof(struct sockaddr));

    _bindAddrSet = true;
}

void TC_HttpAsync::AsyncRequest::doConnect()
{
    if (_pHttpAsync) 
        _pHttpAsync->assertThreadId();
    _fd.createSocket();
    _fd.setblock();
    _fd.setTcpNoDelay();
    _fd.setNoCloseWait();

    try
    {
        if (_bindAddrSet)
        {
            _fd.bind(&_bindAddr, sizeof(_bindAddr));
        }

        int ret = 0;

        if (_bUseProxy)
            ret = _fd.connectNoThrow(_pHttpAsync->getProxyAddr());
        else
            ret = _fd.connectNoThrow(_sHost, _iPort);

        if (ret < 0 && !TC_Socket::isInProgress())
        {
            doException(RequestCallback::Failed_Connect, getError("connect server error."));
            return;
        }

        _pHttpAsync->addConnection(_fd.getfd(), _iUniqId, EPOLLIN | EPOLLOUT);

    }
    catch (exception &ex)
    {
        doException(RequestCallback::Failed_Connect, ex.what());
    }
}


int TC_HttpAsync::AsyncRequest::recv(void* buf, uint32_t len, uint32_t flag)
{
    int ret = ::recv(_fd.getfd(), (char*)buf, len, flag);

    if (ret == 0)
    {
        return 0;
    }
    else if (ret < 0 && TC_Socket::isPending())
    {
        return -1;
    }
    else if (ret < 0)
    {
        //其他网络异常
        return -2;
    }

    //正常发送的数据
    return ret;
}

int TC_HttpAsync::AsyncRequest::send(const void* buf, uint32_t len, uint32_t flag)
{
    int ret = ::send(_fd.getfd(), (char*)buf, len, flag);

    if (ret < 0 && TC_Socket::isPending())
    {
        return -1;
    }
    else if (ret < 0)
    {
        return -2;
    }
    return ret;
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
    doClose();

    try { if (_callbackPtr) _callbackPtr->onFailed(ret, e); } catch (...) { }
}

void TC_HttpAsync::AsyncRequest::doRequest()
{
    if (_pHttpAsync) _pHttpAsync->assertThreadId();
    if (!_fd.isValid()) return;

    int ret = -1;

    setConnected(true);

    do
    {
        ret = -1;

        if (!_sendBuffer.empty())
        {
        	auto data = _sendBuffer.getBufferPointer();
            if ((ret = this->send(data.first, data.second, 0)) > 0)
            {
            	_sendBuffer.moveHeader(ret);
            }
        }
    } while (ret > 0 && !_sendBuffer.empty());

    //网络异常
    if (ret == -2)
    {
        doException(RequestCallback::Failed_Net, getError("send error."));
    }
}

void TC_HttpAsync::AsyncRequest::doReceive()
{
    if (_pHttpAsync) _pHttpAsync->assertThreadId();
    if (!_fd.isValid()) return;

    int recv = 0;

    char buff[8192] = {0};

    do
    {
        if ((recv = this->recv(buff, sizeof(buff), 0)) > 0)
        {
        	_recvBuffer.addBuffer(buff, recv);
        }
    }
    while (recv > 0);

    if (recv == -2)
    {
        doException(RequestCallback::Failed_Net, getError("recv error"));
    }
    else
    {
        //增量decode
	    bool ret    = _stHttpResp.incrementDecode(_recvBuffer);

	    //有头部数据了
        if (_callbackPtr && !_stHttpResp.getHeaders().empty())
        {
            bool bContinue = _callbackPtr->onContinue(_stHttpResp);
            if (!bContinue)
            {
                doException(RequestCallback::Failed_Interrupt, getError("receive interrupt"));
                return;
            }
        }

        //数据接收完毕
        if (ret)
        {
            //只支持短连接, 数据收取完毕, 关闭链接
            doClose();
            try { if (_callbackPtr) _callbackPtr->onSucc(_stHttpResp); } catch (...) { }
            return;
        }
        else
        {
            //服务器关闭了连接
            bool bClose = (recv == 0);

            if (bClose)
            {
                doClose();

                try { if (_callbackPtr) _callbackPtr->onSucc(_stHttpResp); } catch (...) { }
            }
        }
    }
}

void TC_HttpAsync::AsyncRequest::processNet(const epoll_event &ev)
{
    if (TC_Epoller::errorEvent(ev))
    {
        doException(RequestCallback::Failed_Net, getError("net error"));
        return;
    }

    if (TC_Epoller::readEvent(ev))
    {
        doReceive();
    }

    if (TC_Epoller::writeEvent(ev))
    {
        doRequest();
    }
}

void TC_HttpAsync::AsyncRequest::processNotify()
{
    //没有建立连接, 发起连接
    if (!isValid())
    {
        doConnect();
    }
    else
    {
        doRequest();
    }
}

///////////////////////////////////////////////////////////////////////////
#define H64(x) (((uint64_t)x) << 32)

TC_HttpAsync::TC_HttpAsync() : _terminate(false)
{
    memset(&_proxyAddr, 0, sizeof(struct sockaddr));
    memset(&_bindAddr, 0, sizeof(struct sockaddr));

    _bindAddrSet = false;

    _data = new http_queue_type(10000);

    _epoller.create(20480);

    _notify.init(&_epoller);

    uint64_t data = H64(_notify.notifyFd()) | 0;
    _notify.add(data);
}

TC_HttpAsync::~TC_HttpAsync()
{
    terminate();

    delete _data;

    _notify.release();
    _epoller.close();

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

    while (_data->size() > 0)
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
    _data->erase(uniqId);

    TC_ThreadLock::Lock lock(*this);
    notify();
}

void TC_HttpAsync::terminate()
{
    _terminate = true;

    _notify.notify();

    _tpool.waitForAllDone();
}

void TC_HttpAsync::timeout(AsyncRequestPtr& ptr)
{
    ptr->timeout();
}

void TC_HttpAsync::doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, bool bUseProxy)
{
    AsyncRequest * req = new AsyncRequest(stHttpRequest, callbackPtr, bUseProxy);

    if (_bindAddrSet)
    {
        req->setBindAddr(&_bindAddr);
    }

    uint32_t uniqId = _data->generateId();

    req->setUniqId(uniqId);

    req->setHttpAsync(this);

    _data->push(req, uniqId);

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _events.push_back(H64(_notify.notifyFd()) | uniqId);
    }
    _notify.notify();
}

void TC_HttpAsync::doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const string &addr)
{
    AsyncRequest * req = new AsyncRequest(stHttpRequest, callbackPtr, addr);

    if (_bindAddrSet)
    {
        req->setBindAddr(&_bindAddr);
    }

    uint32_t uniqId = _data->generateId();

    req->setUniqId(uniqId);

    req->setHttpAsync(this);

    _data->push(req, uniqId);

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _events.push_back(H64(_notify.notifyFd()) | uniqId);
    }
    _notify.notify();
}

void TC_HttpAsync::addConnection(int fd, uint32_t uniqId, uint32_t events)
{
    uint64_t data = H64(fd) | uniqId;

    _epoller.add(fd, data, events);
}

void TC_HttpAsync::delConnection(int fd)
{
    _epoller.del(fd, 0, 0);
}

int TC_HttpAsync::setBindAddr(const char* sBindAddr)
{
    memset(&_bindAddr, 0x00, sizeof(_bindAddr));

    struct sockaddr_in* p = (struct sockaddr_in *)&_bindAddr;

    try
    {
        TC_Socket::parseAddr(sBindAddr, p->sin_addr);
    }
    catch (exception &ex)
    {
        return -1;
    }

    p->sin_family = AF_INET;
    p->sin_port   = htons(0);

    _bindAddrSet  = true;

    return 0;
}

int TC_HttpAsync::setProxyAddr(const char* sProxyAddr)
{
    vector<string> v = TC_Common::sepstr<string>(sProxyAddr, ":");

    if (v.size() < 2)
        return -1;

    return setProxyAddr(v[0].c_str(), TC_Common::strto<uint16_t>(v[1]));
}

int TC_HttpAsync::setProxyAddr(const char* sHost, uint16_t iPort)
{
    memset(&_proxyAddr, 0x00, sizeof(_proxyAddr));

    struct sockaddr_in *p = (struct sockaddr_in *)&_proxyAddr;

    try
    {
        TC_Socket::parseAddr(sHost, p->sin_addr);
    }
    catch (exception &ex)
    {
        return -1;
    }

    p->sin_family = AF_INET;
    p->sin_port   = htons(iPort);

    return 0;
}

void TC_HttpAsync::setProxyAddr(const struct sockaddr* addr)
{
    memcpy(&_proxyAddr, addr, sizeof(struct sockaddr));
}

void TC_HttpAsync::run()
{
    _threadId = std::this_thread::get_id();

    TC_TimeoutQueue<AsyncRequestPtr>::data_functor df(&TC_HttpAsync::timeout);

    int64_t lastDealTimeout = 0;

    while (!_terminate)
    {
        try
        {
            int64_t now = TNOWMS;
            if (lastDealTimeout + 500 < now)
            {
                lastDealTimeout = now;
                _data->timeout(df);
            }

            int num = _epoller.wait(100);

            for (int i = 0; i < num; ++i)
            {
                epoll_event ev = _epoller.get(i);

                if (_terminate)
                    break;

                uint32_t fd = TC_Epoller::getU32(ev, true);

                if ((int)fd == _notify.notifyFd())
                {
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
                            continue;

                        ptr->processNotify();
                    }                                          
                    
                }
                else
                {

                    uint32_t uniqId = TC_Epoller::getU32(ev, false);

                    AsyncRequestPtr ptr = _data->getAndRefresh(uniqId);
                    if (!ptr) 
                        continue;
 
                    ptr->processNet(ev);
                }
            }
        }
        catch (exception &ex)
        {
            cerr << "[TC_HttpAsync::run] error:" << ex.what() << endl;
        }
    }
}

}


