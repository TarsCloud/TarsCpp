#include "util/tc_epoller.h"
#include "util/tc_timeprovider.h"
#include "util/tc_logger.h"
#include <algorithm>

#if TARGET_PLATFORM_WINDOWS
#include "sys/epoll.h"
#else
#include <unistd.h>
#endif


namespace tars
{

TC_Epoller::NotifyInfo::~NotifyInfo()
{
	if(_epollInfo && _epoller)
	{
		// LOG_CONSOLE_DEBUG << this << ", fd:" << notifyFd() << endl;

		_epoller->releaseEpollInfo(_epollInfo);

		_notify.close();

		_epollInfo = NULL;
		_epoller    = NULL;
	}
}

void TC_Epoller::NotifyInfo::init(TC_Epoller *epoller)
{
    _epoller = epoller;

	//用udp句柄, 方便唤醒, tcp句柄还得构建连接后才能唤醒
	_notify.createSocket(SOCK_DGRAM, AF_INET);

    _epollInfo = _epoller->createEpollInfo(notifyFd());

	// LOG_CONSOLE_DEBUG << this << ", fd:" << notifyFd() << endl;
}

/////////////////////////////////////////////////////////////////////

TC_Epoller::EpollInfo::~EpollInfo()
{
//	 LOG_CONSOLE_DEBUG << this << endl;

	clearCallback();

	if(_deconstructor)
	{
		_deconstructor(_cookie);
		_cookie = NULL;
	}
}

void TC_Epoller::EpollInfo::clearCallback()
{
	_callbacks[0] = EVENT_CALLBACK();
	_callbacks[1] = EVENT_CALLBACK();
	_callbacks[2] = EVENT_CALLBACK();
}

void TC_Epoller::EpollInfo::registerCallback(const map<uint32_t, EVENT_CALLBACK> & callbacks, uint32_t events)
{
	for(auto it : callbacks)
	{
		switch(it.first)
		{
			case EPOLLIN:
				_callbacks[0]	= it.second;
				break;
			case EPOLLOUT:
				_callbacks[1]	= it.second;
				break;
			case EPOLLERR:
				_callbacks[2]	= it.second;
				break;
		}
	}

    if(events != 0)
    {
    	add(events);
    }
}

bool TC_Epoller::EpollInfo::fireEvent(uint32_t event)
{
    try
    {
        auto data = shared_from_this();

        if((event & EPOLLERR) && _callbacks[2])
        {
            _callbacks[2](data);

            return false;
        }

        if((event & EPOLLIN) && _callbacks[0])
        {
            if (!_callbacks[0](data))
                return false;
        }

        if((event & EPOLLOUT) && _callbacks[1])
        {
            if (!_callbacks[1](data))
                return false;
        }
    }
    catch(exception &ex)
    {
        cerr << "TC_Epoller::EpollInfo::fireEvent event:" << event << ", error: " << ex.what() << endl;
        return false;
    }
    catch(...)
    {
        cerr << "TC_Epoller::EpollInfo::fireEvent event:" << event << ", error." << endl;
        return false;
    }

    return true;
}

void TC_Epoller::EpollInfo::release()
{
	if(this->valid())
	{
		assert(_epoller);

		//epoll不再关注该事件
		del(0);

		_fd = INVALID_SOCKET;
	}
}

void TC_Epoller::EpollInfo::add(uint32_t events)
{
	if(valid())
	{
		_epoller->add(_fd, data(), events);
	}
}

void TC_Epoller::EpollInfo::mod(uint32_t events)
{
	if(valid())
	{
		_epoller->mod(_fd, data(), events);
	}
}

void TC_Epoller::EpollInfo::del(uint32_t events)
{
	if(valid())
	{
		_epoller->del(_fd, 0, events);
	}
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
	if(_notify != nullptr)
	{
        delete _notify;
		_notify = nullptr;
	}

	if(_pevs != nullptr)
	{
		delete[] _pevs;
		_pevs = nullptr;
	}

	clear();

	_idleCallbacks.clear();

#if TARGET_PLATFORM_WINDOWS
	if (_iEpollfd != NULL)
	{
		epoll_close(_iEpollfd);
		_iEpollfd = NULL;
	}
#else
	if (_iEpollfd >= 0)
	{
		::close(_iEpollfd);
		_iEpollfd = -1;
	}
#endif

}

#if TARGET_PLATFORM_IOS

int TC_Epoller::ctrl(SOCKET_TYPE fd, uint64_t data, uint32_t events, int op)
{
    if(fd < 0) return -1;

    int n = 0;
    struct kevent64_s ev[2];
    
    if(_enableET)
    {
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
		close();
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
    if(_enableET)
    {
        ev.events = events | EPOLLET;
    }
    else
    {
        ev.events = events;
    }
#endif

	return epoll_ctl(_iEpollfd, op, fd, &ev);
}
#endif

void TC_Epoller::create(int size, bool createNotify)
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

    _max_connections = 128;

    _pevs = new epoll_event[_max_connections];

    if(createNotify)
    {
        if (_notify != NULL)
        {
            delete _notify;
            _notify = NULL;
        }

        _notify = new NotifyInfo();
        _notify->init(this);
        _notify->getEpollInfo()->add(EPOLLIN);
    }
}

void TC_Epoller::close()
{
	if(_notify != nullptr)
	{
        delete _notify;
		_notify = nullptr;
	}

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
    ::close(_iEpollfd);
	_iEpollfd = -1;

#else
    epoll_close(_iEpollfd);
    _iEpollfd = NULL;
#endif
}

shared_ptr<TC_Epoller::EpollInfo> TC_Epoller::createEpollInfo(SOCKET_TYPE fd)
{
	return std::make_shared<TC_Epoller::EpollInfo>(this, fd);
}

void TC_Epoller::releaseEpollInfo(const shared_ptr<TC_Epoller::EpollInfo> &epollInfo)
{
	if(epollInfo)
	{
		epollInfo->clearCallback();
        epollInfo->release();
	}
}

void TC_Epoller::add(SOCKET_TYPE fd, uint64_t data, uint32_t events)
{
#if TARGET_PLATFORM_IOS
    ctrl(fd, data, events, EV_ADD|EV_ENABLE);
#else
    ctrl(fd, data, events, EPOLL_CTL_ADD);
#endif
}

void TC_Epoller::mod(SOCKET_TYPE fd, uint64_t data, uint32_t events)
{
#if TARGET_PLATFORM_IOS
    ctrl(fd, data, events, EV_ADD|EV_ENABLE);
#else
    ctrl(fd, data, events, EPOLL_CTL_MOD);
#endif
}

void TC_Epoller::del(SOCKET_TYPE fd, uint64_t data, uint32_t events)
{
#if TARGET_PLATFORM_IOS
    ctrl(fd, data, events, EV_DELETE);
#else
    ctrl(fd, data, events, EPOLL_CTL_DEL);
#endif
}

epoll_event& TC_Epoller::get(int i) 
{ 
	assert(_pevs != 0); 
	return _pevs[i]; 
}

int TC_Epoller::wait(int millsecond)
{
//#if !TARGET_PLATFORM_WINDOWS
//retry:
//#endif

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
	    return 0;
//		goto retry;
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

void TC_Epoller::terminate()
{
	//清空定时任务
	clear();

	_terminate = true;

    notify();
}

void TC_Epoller::reset()
{
	clear();

	_terminate = false;
}

void TC_Epoller::syncCallback(const std::function<void()>& func, int64_t millseconds)
{
	TC_Epoller::NotifyInfo syncNotify;
	std::mutex	syncMutex;
	std::condition_variable syncCond;

	syncNotify.init(this);

    map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;
    callbacks[EPOLLOUT] = [&](const shared_ptr<TC_Epoller::EpollInfo> &data)
    {
        try
        {
            func();
        }
        catch (...)
        {
        }

        std::unique_lock<std::mutex> lock(syncMutex);
        syncCond.notify_one();

        return false;
    };

    std::unique_lock<std::mutex> lock(syncMutex);

    syncNotify.getEpollInfo()->registerCallback(callbacks, EPOLLOUT);

    if (millseconds >= 0)
    {
        syncCond.wait_for(lock, std::chrono::milliseconds(millseconds));
    }
    else
    {
        syncCond.wait(lock);
    }
}

void TC_Epoller::asyncCallback(const std::function<void()>& func)
{
    TC_Epoller::NotifyInfo *syncNotify = new TC_Epoller::NotifyInfo();
    syncNotify->init(this);

    syncNotify->getEpollInfo()->cookie(syncNotify, [](void *p)
                                       {
                                           TC_Epoller::NotifyInfo *ni = (TC_Epoller::NotifyInfo *)p;
                                           delete ni;
                                       });
    
    map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;
    callbacks[EPOLLOUT] = [=](const shared_ptr<TC_Epoller::EpollInfo> &data)
    {
		try { func(); } catch(...) {}

		//释放到自己的owner, 这样才回保证EpollInfo被自动释放
		syncNotify->getEpollInfo().reset();
        return false;
    };

    syncNotify->getEpollInfo()->registerCallback(callbacks, EPOLLOUT);
}

void TC_Epoller::notify()
{
	if(_notify)
	{
		_notify->getEpollInfo()->mod(EPOLLOUT);
	}
}

void TC_Epoller::onAddTimer()
{
    notify();
}

void TC_Epoller::onFireEvent(std::function<void()> func)
{
    try {func();} catch(...){}
}

void TC_Epoller::done(uint64_t ms)
{
//	LOG_CONSOLE_DEBUG << "fireEvents: " << ms << endl;

    //触发定时事件
	int64_t nextTimer = fireEvents(ms);

//	LOG_CONSOLE_DEBUG << "wait: " << ms << ", " << ms - TNOWMS << endl;

	int num = wait(nextTimer);

	list<shared_ptr<EpollInfo>> delEpollInfo;

    //先处理epoll的网络事件
    for (int i = 0; i < num; ++i)
    {
		if(_terminate)
			return;

        const epoll_event& ev = get(i);

        EpollInfo *info = (EpollInfo*)TC_Epoller::getU64(ev);

        if(info == NULL || !info->valid())
        {
            continue;
        }

        if(info->_epoller != this)
        {
            //not current epoller, not process(should not be here!!!!)
            continue;
        }
        // assert(info->_epoller == this);

        //返回成智能指针, 保证EpollInfo fireEvent的过程中, 不会被释放掉
        auto data = info->shared_from_this();

        if(data->_callback)
        {
            try {data->_callback(data); } catch(exception &ex) {}
        }

        uint32_t events = 0;

        if (TC_Epoller::errorEvent(ev)) 
        {
            events = EPOLLERR;
        }
        else
        {
            if (TC_Epoller::writeEvent(ev)) {
                events |= EPOLLOUT; 
            }

            if (TC_Epoller::readEvent(ev))
            {
                events |= EPOLLIN;  
            }
        }

        if(!data->fireEvent(events))
		{
			delEpollInfo.push_back(data);

			data->release();
		}

    }

	std::for_each(_idleCallbacks.begin(), _idleCallbacks.end(), [](const std::function<void()> &f){
		try {f();} catch(...){}
	});
}

void TC_Epoller::loop(uint64_t ms)
{
	while(!_terminate)
	{
		this->done(ms);
	}
}
}

