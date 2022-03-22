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

#include "servant/CommunicatorEpoll.h"
#include "servant/Communicator.h"
#include "servant/Application.h"
#include "servant/RemoteLogger.h"
#include "servant/ObjectProxy.h"
#include "servant/EndpointManager.h"

using namespace std;

namespace tars
{

#define MAX_STAT_QUEUE_SIZE 100000 //上报队列缓存大小

CommunicatorEpoll::CommunicatorEpoll(Communicator * pCommunicator,size_t netThreadSeq, bool isFirst)
: _communicator(pCommunicator)
, _isFirst(isFirst)
, _netThreadSeq(netThreadSeq)
, _noSendQueueLimit(1000)
, _timeoutCheckInterval(100)
, _statQueue(MAX_STAT_QUEUE_SIZE)

{
//	LOG_CONSOLE_DEBUG << endl;

    //节点队列未发送请求的大小限制
    _noSendQueueLimit = TC_Common::strto<size_t>(pCommunicator->getProperty("sendqueuelimit", pCommunicator->getProperty("nosendqueuelimit", "100000")));
    if(_noSendQueueLimit < 1000)
    {
        _noSendQueueLimit = 1000;
    }

    //检查超时请求的时间间隔，单位:ms
    _timeoutCheckInterval = TC_Common::strto<int64_t>(pCommunicator->getProperty("timeoutcheckinterval", "1000"));
    if(_timeoutCheckInterval < 1)
    {
        _timeoutCheckInterval = 5;
    }

	for(size_t i = 0;i < MAX_CLIENT_NOTIFYEVENT_NUM;++i)
	{
		_notify[i] = NULL;
	}
}

CommunicatorEpoll::~CommunicatorEpoll()
{
//     LOG_CONSOLE_DEBUG << endl;
}

void CommunicatorEpoll::handleServantThreadQuit(uint16_t iSeq)
{
	assert(_threadId == this_thread::get_id());

	//在网络线程中处理的!
    if(_notify[iSeq])
	{
        _notify[iSeq]->autoDestroy = true;

        //通知网络线程, 网络线程好析构notify对象
		notify(iSeq);

        _notify[iSeq] = NULL;
	}
}

void CommunicatorEpoll::notifyServantThreadQuit(uint16_t iSeq)
{

	if(_scheduler)
	{
		//CommunicatorEpoll还没有退出!
		if (_threadId == this_thread::get_id())
		{
			//同一个线程里面结束, 直接释放相关资源即可
			CommunicatorEpoll::handleServantThreadQuit(iSeq);
		}
		else
		{
            //等待数据都发送出去, 避免业务线程退出以后, 还有数据没有发送出去, 这里处理不够优雅!
            {
                std::lock_guard<std::mutex> lock(_mutex);

                //再做一次判断
                if (_scheduler)
                {
                    //通知网络线程去释放资源!
                    _epoller->asyncCallback(std::bind(&CommunicatorEpoll::handleServantThreadQuit, this, iSeq));
                }
            }
		}
	}
}

void CommunicatorEpoll::notifyTerminate()
{
    if (_scheduler)
    {
        if(_threadId == this_thread::get_id())
        {
            //同一个线程里面结束, 直接释放相关资源即可
            CommunicatorEpoll::handleTerminate();
        }
        else
        {
            std::lock_guard<std::mutex> lock(_mutex);

            if (_scheduler)
            {
                //通知网络线程去释放资源!
                _epoller->syncCallback(std::bind(&CommunicatorEpoll::handleTerminate, this), 1000);

                // if (_scheduler)
                // {
                //     _epoller->syncCallback(std::bind(&CommunicatorEpoll::handleTerminate, this), 1000);
                //     LOG_CONSOLE_DEBUG << _scheduler.get() << endl;
                // }
            }
        }
    }
}

void CommunicatorEpoll::handleTerminate()
{
	assert(_threadId == this_thread::get_id());

    if (_scheduler)
    {
        for (size_t i = 0; i < MAX_CLIENT_NOTIFYEVENT_NUM; ++i)
        {
            if (_notify[i])
            {
                delete _notify[i];
            }
            _notify[i] = NULL;
        }

        for (size_t i = 0; i < _vObjectProxys.size(); i++)
        {
            if (_vObjectProxys[i])
            {
                delete _vObjectProxys[i];
                _vObjectProxys[i] = NULL;
            }
        }

        _vObjectProxys.clear();

        //这里是否还是有临界情况!!??
        if(_epoller)
        {
            //定时任务都删除掉
            for_each(_timerIds.begin(), _timerIds.end(), [&](int64_t id)
                    { _epoller->erase(id); });
        }

        _timerIds.clear();

        //独立启动的才需要释放协程调度器, 复用其他协程是不能停止调度器的!
        if(!this->isSchedCommunicatorEpoll())
        {
            _scheduler->terminate();

            assert(_pSptd);

            _pSptd->_sched.reset();

            ServantProxyThreadData::g_sp.reset();
        }

        _scheduler.reset();

        StatReport::MapStatMicMsg *pmStatMicMsg = NULL;

        while (_statQueue.pop_front(pmStatMicMsg))
        {
            assert(pmStatMicMsg != NULL);
            delete pmStatMicMsg;
            pmStatMicMsg = NULL;
        }
    }
}

void CommunicatorEpoll::terminate()
{
    //通知网络线程退出, 不再执行任何操作
	notifyTerminate();
}

void CommunicatorEpoll::notifyUpdateEndpoints(ServantProxy *servantProxy, const set<EndpointInfo> & active,const set<EndpointInfo> & inactive)
{
    CommunicatorEpoll *ce = this;

    _epoller->asyncCallback([=]()
                            { servantProxy->onNotifyEndpoints(ce, active, inactive); });
}

int CommunicatorEpoll::loadObjectLocator()
{
	assert(_threadId == this_thread::get_id());

    for (size_t i = 0; i < _objNum; i++)
    {
        _vObjectProxys[i]->loadLocator();
    }
    return 0;
}

ObjectProxy* CommunicatorEpoll::servantToObjectProxy(ServantProxy *servantProxy)
{
	TC_ThreadRLock lock(_servantMutex);

	auto it = _servantObjectProxy.find(servantProxy);
    if( it != _servantObjectProxy.end())
    {
        //当前线程(CommunicatorEpoll)还没有创建出对应的ObjectProxy
        return it->second;
    }

//    assert(false);

    return NULL;
}

ObjectProxy * CommunicatorEpoll::hasObjectProxy(const string & sObjectProxyName,const string& setName)
{
	TC_LockT<TC_ThreadRecMutex> lock(_objectMutex);

	string tmpObjName = sObjectProxyName + "!" + setName;
	auto it = _objectProxys.find(tmpObjName);
	if(it != _objectProxys.end())
	{
		return it->second;
	}

	return NULL;
}

ObjectProxy * CommunicatorEpoll::createObjectProxy(ServantProxy *servantProxy, const string & sObjectProxyName, const string& setName)
{
	ObjectProxy * pObjectProxy;
	string tmpObjName = sObjectProxyName + "!" + setName;
	{
		TC_LockT<TC_ThreadRecMutex> lock(_objectMutex);

		auto it = _objectProxys.find(tmpObjName);
		if (it != _objectProxys.end())
		{
			return it->second;
		}

	    pObjectProxy = new ObjectProxy(this, servantProxy, sObjectProxyName, setName);

		_objectProxys[tmpObjName] = pObjectProxy;

	}

	{
		TC_ThreadWLock lock(_vObjectMutex);

		_vObjectProxys.push_back(pObjectProxy);

		_objNum++;
	}

	{
		TC_ThreadWLock lock(_servantMutex);

		_servantObjectProxy[servantProxy] = pObjectProxy;
	}

	return pObjectProxy;
}

void CommunicatorEpoll::addFd(AdapterProxy* adapterProxy)
{
    shared_ptr<TC_Epoller::EpollInfo> epollInfo = adapterProxy->trans()->getEpollInfo();

    epollInfo->cookie(adapterProxy);

	map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

	callbacks[EPOLLIN] = std::bind(&CommunicatorEpoll::handleInputImp, this, std::placeholders::_1);
	callbacks[EPOLLOUT] = std::bind(&CommunicatorEpoll::handleOutputImp, this, std::placeholders::_1);
	callbacks[EPOLLERR] = std::bind(&CommunicatorEpoll::handleCloseImp, this, std::placeholders::_1);

	epollInfo->registerCallback(callbacks, EPOLLIN|EPOLLOUT);
}

void CommunicatorEpoll::notify(size_t iSeq)
{
	assert(_notify[iSeq] != NULL);

    // LOG_CONSOLE_DEBUG << "iSeq:" << iSeq << ", epollInfo:" << _notify[iSeq]->notify.getEpollInfo() << ", ce:" << this << endl;

    _notify[iSeq]->notify.getEpollInfo()->mod(EPOLLOUT);
}

void CommunicatorEpoll::initNotify(size_t iSeq, const shared_ptr<ReqInfoQueue> &msgQueue)
{
	// if(_notify[iSeq] != NULL)
	// {
    //     LOG_CONSOLE_DEBUG << "iSeq:" << iSeq << ", " << msgQueue.get() << ", " << _notify[iSeq]->msgQueue.get() << endl;
    // }
    // assert(_notify[iSeq] == NULL);

    if (_notify[iSeq] == NULL)
    {
        _notify[iSeq] = new FDInfo();
        _notify[iSeq]->msgQueue = msgQueue;
        _notify[iSeq]->iSeq     = iSeq;

        _notify[iSeq]->notify.init(_epoller);

        _notify[iSeq]->notify.getEpollInfo()->cookie((void*)_notify[iSeq]);

        map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

        callbacks[EPOLLOUT] = std::bind(&CommunicatorEpoll::handleNotify, this, std::placeholders::_1);

        _notify[iSeq]->notify.getEpollInfo()->registerCallback(callbacks, EPOLLIN | EPOLLOUT);
    }
    else
    {
        _notify[iSeq]->msgQueue = msgQueue;
    }
}

bool CommunicatorEpoll::handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
	assert(_threadId == this_thread::get_id());

	AdapterProxy* adapterProxy = (AdapterProxy*)data->cookie();

    adapterProxy->trans()->close();

    return false;
}

bool CommunicatorEpoll::handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
	assert(_threadId == this_thread::get_id());

	AdapterProxy* adapterProxy = (AdapterProxy*)data->cookie();

    try
    {
        adapterProxy->trans()->doResponse();
    }
    catch(const std::exception& e)
    {
//		LOG_CONSOLE_DEBUG << "[CommunicatorEpoll::handleInputImp] error:" << e.what() << endl;
        TLOGTARS("[CommunicatorEpoll::handleInputImp] error:" << e.what() << endl);
        adapterProxy->addConnExc(true);
        return false;
    }
   
    
    return true;
}

bool CommunicatorEpoll::handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
	assert(_threadId == this_thread::get_id());

//	LOG_CONSOLE_DEBUG << endl;

    AdapterProxy* adapterProxy = (AdapterProxy*)data->cookie();

    try
    {
        adapterProxy->trans()->doRequest();
    }
    catch(const std::exception& e)
    {
//		LOG_CONSOLE_DEBUG << "[CommunicatorEpoll::handleOutputImp] error:" << e.what() << endl;

		TLOGTARS("[CommunicatorEpoll::handleOutputImp] error:" << e.what() << endl);
        adapterProxy->addConnExc(true);
        return false;
    }
    
    return true;
}

void CommunicatorEpoll::report(StatReport::MapStatMicMsg *pmStatMicMsg)
{
    bool bFlag = _statQueue.push_back(pmStatMicMsg);
    if(!bFlag)
    {
        delete pmStatMicMsg;
        pmStatMicMsg = NULL;

        TLOGERROR("[StatReport::report: queue full]" << endl);
    }
}

bool CommunicatorEpoll::popStatMsg(StatReport::MapStatMicMsg* &mStatMsg)
{
    return _statQueue.pop_front(mStatMsg);
}

void CommunicatorEpoll::doTimeout()
{
	assert(_threadId == this_thread::get_id());

	for(size_t i = 0; i < getObjNum(); ++i)
    {
        getObjectProxy(i)->doTimeout();
    }
}

void CommunicatorEpoll::doKeepAlive()
{
    assert(_threadId == this_thread::get_id());

    if (_communicator->getKeepAliveInterval() == 0)
    {
        return;
    }

    for(size_t i = 0; i < getObjNum(); ++i)
    {
        getObjectProxy(i)->doKeepAlive();
    }
}

void CommunicatorEpoll::doStat()
{
	assert(_threadId == this_thread::get_id());

    {
        if(isFirstNetThread()) {
            _communicator->doStat();
        }

        StatReport::MapStatMicMsg* pmStatMicMsg = new StatReport::MapStatMicMsg();//(mStatMicMsg);

        for(size_t i = 0;i < getObjNum(); ++i)
        {
            getObjectProxy(i)->mergeStat(*pmStatMicMsg);
        }

        //有数据才上报
        if(!pmStatMicMsg->empty())
        {
            report(pmStatMicMsg);
        }
        else
        {
            delete pmStatMicMsg;
            pmStatMicMsg = NULL;
        }
    }
}

void CommunicatorEpoll::getResourcesInfo(ostringstream &desc)
{
	assert(_threadId == this_thread::get_id());

	desc << TC_Common::outfill("index") << _netThreadSeq << endl;
	desc << TC_Common::outfill("stat size") << getReportSize() << endl;
	desc << TC_Common::outfill("obj num") << getObjNum() << endl;

	const static string TAB = "    ";
	for(size_t i = 0; i < getObjNum(); ++i)
	{
		desc << TAB << OUT_LINE_TAB(1) << endl;

		desc << TAB << TC_Common::outfill("obj name") << getObjectProxy(i)->name() << endl;
		const vector<AdapterProxy*> &adapters = getObjectProxy(i)->getAdapters();

		for(auto adapter : adapters)
		{
			desc << TAB << TAB << OUT_LINE_TAB(2) << endl;

			desc << TAB << TAB << TC_Common::outfill("adapter") << adapter->endpoint().getEndpoint().toString() << endl;
			desc << TAB << TAB << TC_Common::outfill("recv size")  << adapter->trans()->getRecvBuffer().getBufferLength() << endl;
			desc << TAB << TAB << TC_Common::outfill("send size")  << adapter->trans()->getSendBuffer().getBufferLength() << endl;
		}
	}
}

void CommunicatorEpoll::doReconnect()
{
	assert(_threadId == this_thread::get_id());

	int64_t iNow = TNOWMS;

    set<TC_Transceiver*> does;
	while(!_reconnect.empty())
	{
		auto it = _reconnect.begin();

		if(it->first > iNow)
		{
			return;
		}

        //一次循环同一个节点只尝试一次重试,以避免多次触发close，导致重连的间隔无效
        if (does.find(it->second) != does.end())
        {
            _reconnect.erase(it++);
        }
        else
        {
            does.insert(it->second);
            it->second->connect();
            _reconnect.erase(it++);
        }
	}
}

bool CommunicatorEpoll::handleNotify(const shared_ptr<TC_Epoller::EpollInfo> &data)
{
	assert(_threadId == this_thread::get_id());

	// LOG_CONSOLE_DEBUG << endl;

    //队列有消息通知过来
    FDInfo *pFDInfo = (FDInfo*)data->cookie();

    ReqMessage * msg = NULL;

    try
    {
        int64_t now = TNOWMS;

        while (pFDInfo->msgQueue->pop_front(msg))
        {
            msg->pObjectProxy->invoke(msg);

            if(TNOWMS - now >= 1)
            {
                //避免包太多的时候, 循环占用网路线程, 导致连接都建立不上, 一个包都无法发送出去
				data->mod(EPOLLOUT);

                break;
            }
        }

        if (pFDInfo->msgQueue->empty() && pFDInfo->autoDestroy)
        {
            delete pFDInfo;
            return false;
        }
    }
    catch(exception & e)
    {
        TLOGERROR("[CommunicatorEpoll::handleNotify error: " << e.what() << "]"<<endl);
    }
    catch(...)
    {
        TLOGERROR("[CommunicatorEpoll::handleNotify error]" <<endl);
    }

    return true;
}

void CommunicatorEpoll::initializeEpoller()
{
	_threadId = this_thread::get_id();

    _scheduler = TC_CoroutineScheduler::scheduler();

     assert(_scheduler);

    _epoller = _scheduler->getEpoller();

    auto id1 = _epoller->postRepeated(1000, false, std::bind(&CommunicatorEpoll::doReconnect, this));
	auto id2 = _epoller->postRepeated(1000 * 5, false, std::bind(&CommunicatorEpoll::doStat, this));
    auto id3 = _epoller->postRepeated(_timeoutCheckInterval, false, std::bind(&CommunicatorEpoll::doTimeout, this));

	_timerIds = { id1, id2, id3 };

    if (_communicator->getKeepAliveInterval() > 0) {
        auto id = _epoller->postRepeated(1000 * 2, false, std::bind(&CommunicatorEpoll::doKeepAlive, this));
        _timerIds.emplace_back(id);
    }
}

void CommunicatorEpoll::run()
{
    //注意网络通信器是通过startCoroutine启动的, 因此就在协程中!

	_public = true;

    initializeEpoller();

    _pSptd = ServantProxyThreadData::getData();
    _pSptd->_sched = _scheduler;

    _netThreadSeq = _pSptd->_reqQNo;

	_epoller->setName("communicator-epoller-public-netseq:" + TC_Common::tostr(_netThreadSeq));

    //关联公有网络通信器
    auto info = _pSptd->addCommunicatorEpoll(shared_from_this());
    info->_communicator = this->_communicator;

	//当前线程处于网络线程中!
    _pSptd->_communicatorEpoll = this;

    _communicator->notifyCommunicatorEpollStart();

}

//////////////////////////////////////////////////////////////////////////////////
}
