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
#include "util/tc_epoller.h"
#if TARGET_PLATFORM_WINDOWS
#include "sys/epoll.h"
#else
#include <unistd.h>
#endif

namespace tars
{

TC_Epoller::NotifyInfo::NotifyInfo() : _ep(NULL)
{
}

TC_Epoller::NotifyInfo::~NotifyInfo()
{
    _notify.close();
}

void TC_Epoller::NotifyInfo::init(TC_Epoller *ep)
{
    _ep = ep;

	_notify.createSocket(SOCK_DGRAM, AF_INET);
}

void TC_Epoller::NotifyInfo::add(uint64_t data)
{
    _data = data;
    _ep->add(_notify.getfd(), data, EPOLLIN | EPOLLOUT);
}

void TC_Epoller::NotifyInfo::notify()
{
    _ep->mod(_notify.getfd(), _data, EPOLLIN | EPOLLOUT);
}

void TC_Epoller::NotifyInfo::release()
{
    _ep->del(_notify.getfd(), 0, EPOLLIN | EPOLLOUT);
    _notify.close();
}

int TC_Epoller::NotifyInfo::notifyFd()
{
    return _notify.getfd();
}

//////////////////////////////////////////////////////////////////////

TC_Epoller::TC_Epoller()
{
#if TARGET_PLATFORM_WINDOWS
    _iEpollfd = NULL;
#else
	_iEpollfd = -1;
#endif
	_pevs     = nullptr;
	_max_connections = 1024;
}

TC_Epoller::~TC_Epoller()
{
	if(_pevs != nullptr)
	{
		delete[] _pevs;
		_pevs = nullptr;
	}

#if TARGET_PLATFORM_WINDOWS
	if (_iEpollfd != NULL)
		epoll_close(_iEpollfd);
#else
	if (_iEpollfd > 0)
		::close(_iEpollfd);
#endif

}

#if TARGET_PLATFORM_IOS

int TC_Epoller::ctrl(SOCKET_TYPE fd, uint64_t data, uint32_t events, int op)
{
    if(fd < 0) return -1;

    int n = 0;
    struct kevent64_s ev[2];

	if(_enableET) {
		op = op | EV_CLEAR;
	}

	if (events & EPOLLIN)
    {
        EV_SET64(&ev[n++], fd, EVFILT_READ, op, 0, 0, data, 0, 0);
    }

    if (events & EPOLLOUT)
    {
        EV_SET64(&ev[n++], fd, EVFILT_WRITE, op, 0, 0, data, 0, 0);
    }

    int ret = kevent64(_iEpollfd, ev, n, nullptr, 0, 0, nullptr);

    if(ret == -1)
    {
        //一般都是析构的时候出现，有需要close就行
//        cerr << "[TC_Epoller::ctrl] error, fd:" << fd << ", errno:" << errno  << "|"<< strerror(errno) << endl;
        ::close(_iEpollfd);
        _iEpollfd = 0;
    }
    return ret;
}

#else
int TC_Epoller::ctrl(SOCKET_TYPE fd, uint64_t data, uint32_t events, int op)
{
	struct epoll_event ev;
	ev.data.u64 = data;

#if TARGET_PLATFORM_WINDOWS
	ev.events = events;
#else
    if (_enableET)
    {
        events = events | EPOLLET;
    }

    ev.events   = events;
#endif

	return epoll_ctl(_iEpollfd, op, fd, &ev);
}
#endif

void TC_Epoller::create(int size)
{
#if TARGET_PLATFORM_IOS
    _iEpollfd = kqueue();
#else
	_iEpollfd = epoll_create(size);
#endif
    if (nullptr != _pevs)
    {
        delete[] _pevs;
    }

    _max_connections = 1024;

    _pevs = new epoll_event[_max_connections];
}

void TC_Epoller::close()
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
    ::close(_iEpollfd);
#else
    epoll_close(_iEpollfd);
#endif
    _iEpollfd = 0;
}

int TC_Epoller::add(SOCKET_TYPE fd, uint64_t data, int32_t event)
{
#if TARGET_PLATFORM_IOS
    return ctrl(fd, data, event, EV_ADD|EV_ENABLE);
#else
    return ctrl(fd, data, event, EPOLL_CTL_ADD);
#endif
}

int TC_Epoller::mod(SOCKET_TYPE fd, uint64_t data, int32_t event)
{
#if TARGET_PLATFORM_IOS
    return add(fd, data, event);
#else
    return ctrl(fd, data, event, EPOLL_CTL_MOD);
#endif
}

int TC_Epoller::del(SOCKET_TYPE fd, uint64_t data, int32_t event)
{
#if TARGET_PLATFORM_IOS    
    return ctrl(fd, data, event, EV_DELETE);
#else
    return ctrl(fd, data, event, EPOLL_CTL_DEL);
#endif
}

epoll_event& TC_Epoller::get(int i) 
{ 
	assert(_pevs != 0); 
	return _pevs[i]; 
}

int TC_Epoller::wait(int millsecond)
{
#if !TARGET_PLATFORM_WINDOWS    
retry:    
#endif

	int ret;
#if TARGET_PLATFORM_IOS
    struct timespec timeout;
    timeout.tv_sec = millsecond / 1000;
    timeout.tv_nsec = (millsecond % 1000) * 1000 * 1000;
	ret = kevent64(_iEpollfd, nullptr, 0, _pevs, _max_connections, 0, &timeout);
#else
	ret = epoll_wait(_iEpollfd, _pevs, _max_connections, millsecond);
#endif

#if TARGET_PLATFORM_WINDOWS
	return ret;
#else
	if(ret < 0 && errno == EINTR)
	{
		goto retry;
	}

	return ret;
#endif
}

bool TC_Epoller::readEvent(const epoll_event &ev)
{
#if TARGET_PLATFORM_IOS
    if (ev.filter == EVFILT_READ)
#else
    if (ev.events & EPOLLIN)
#endif
    {
        return true;
    }

    return false;
}

bool TC_Epoller::writeEvent(const epoll_event &ev)
{
#if TARGET_PLATFORM_IOS
    if (ev.filter == EVFILT_WRITE)
#else
    if (ev.events & EPOLLOUT)              
#endif 
    {
        return true;
    }

    return false;
}

bool TC_Epoller::errorEvent(const epoll_event &ev)
{
#if TARGET_PLATFORM_IOS
    if (ev.filter == EVFILT_EXCEPT)
    {
        return true;
    }
#else
    if (ev.events & EPOLLERR || ev.events & EPOLLHUP)
    {
        return true;
    }
#endif
    return false;
}

uint32_t TC_Epoller::getU32(const epoll_event &ev, bool high)
{
    uint32_t u32 = 0;
    if(high)
    {
#if TARGET_PLATFORM_IOS
        u32 = ev.udata >> 32;
#else
        u32 = ev.data.u64 >> 32;
#endif        
    }
    else
    {
#if TARGET_PLATFORM_IOS
        u32 = (uint32_t)ev.udata;
#else    
        u32 = ev.data.u32;
#endif
    }

    return u32;
}

uint64_t TC_Epoller::getU64(const epoll_event &ev)
{
    uint64_t data;
#if TARGET_PLATFORM_IOS
    data = ev.udata;
#else
    data = ev.data.u64;
#endif
    return data;
}

}


