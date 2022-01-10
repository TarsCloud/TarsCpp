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

#ifndef __TARS__COMMUNICATOR_EPOLL_H_
#define __TARS__COMMUNICATOR_EPOLL_H_

#include "util/tc_thread.h"
#include "util/tc_thread_mutex.h"
#include "util/tc_epoller.h"
#include "util/tc_loop_queue.h"
#include "servant/Message.h"
#include "servant/EndpointInfo.h"
#include "servant/StatReport.h"
#include "servant/Communicator.h"
#include <set>

namespace tars
{

class Communicator;
class ObjectProxy;
class StatReport;
class PropertyReport;

////////////////////////////////////////////////////////////////////////
/**
 * 监听FD事件并触发注册的handle
 */
struct FDInfo 
{
    /**
     * 构造函数
     */
    FDInfo()
    {
    }

    /**
     * 析构函数
     */
    ~FDInfo()
    {
        if (msgQueue)
        {
            ReqMessage *msg;
            while (msgQueue->pop_front(msg))
            {
                delete msg;
            }
        }
    }
    size_t iSeq;
    shared_ptr<ReqInfoQueue> msgQueue;
    TC_Epoller::NotifyInfo notify;
    bool autoDestroy = false;
};

////////////////////////////////////////////////////////////////////////
/**
 * 客户端网络处理的线程类
 */
class CommunicatorEpoll : public TC_Thread, public enable_shared_from_this<CommunicatorEpoll>
{
public:

    /**
     * 构造函数
     * @param pCommunicator
     * @param netThreadSeq, 业务线程序号, 如果是公有网络通信器, 则为-1
     * @param isFirst, 是否是第一个公有网络通信器
     */
    CommunicatorEpoll(Communicator * pCommunicator, size_t netThreadSeq, bool isFirst = false);

    /**
     * 析构函数
     */
    virtual ~CommunicatorEpoll();

    /**
     * 获取 Communicator
     */
    inline Communicator * getCommunicator()
    {
        return _communicator;
    }

    /**
     * 获取 网络线程id
     */
    inline size_t getCommunicatorNetThreadSeq()
    {
        return _netThreadSeq;
    }

    /**
     * 获取 节点队列未发送请求的大小限制
     */
    inline size_t getNoSendQueueLimit()
    {
        return _noSendQueueLimit;
    }

    /*
     * 判断是否是第一个网络线程 主控写缓存的时候用到
     */
    inline bool isFirstNetThread()
    {
        return _isFirst;
    }

    /**
     * 获取epoller
     * @return
     */
    inline TC_Epoller* getEpoller() { return _epoller; }

    /**
     * 是否存在ObjectProxy了, 如果已经存在则创建
     * @param sObjectProxyName
     * @param setName
     * @return
     */
	ObjectProxy * hasObjectProxy(const string & sObjectProxyName,const string& setName="");

    /*
     * 获取本epoll的代理对象
     */
    ObjectProxy * createObjectProxy(ServantProxy *servantProxy, const string & sObjectProxyName,const string& setName="");

    /**
     * 循环监听网络事件
     */
    virtual void run();

    /**
     * 中止监听
     */
    void terminate();

    /**
     * 注册fd对应的处理handle
     * @param adapterProxy
     */
    void addFd(AdapterProxy* adapterProxy);

    /**
     * 通知事件过来
     * @param iSeq
     */
    void notify(size_t iSeq);

    /**
     * 主动更新ip list
     * @param active
     * @param inactive
     */
    void notifyUpdateEndpoints(ServantProxy *servantProxy, const set<EndpointInfo> & active,const set<EndpointInfo> & inactive);

    /**
     * 数据加入到异步线程队列里面
     * @return
     */
    inline void pushAsyncThreadQueue(ReqMessage * msg) { _communicator->pushAsyncThreadQueue(msg); }

	/**
	 * set reconnect
	 * @param time
	 */
	inline void reConnect(int64_t ms, TC_Transceiver*p) { _reconnect[ms] = p; }

	/**
	 * communicator resource desc
	 * @return
	 */
	void getResourcesInfo(ostringstream &desc);

    /**
     * 所有对象代理加载locator信息
     */
    int loadObjectLocator();

    /**
     * servant换成对应线程的objectproxy
     * @param servantProxy
     * @return
     */
    ObjectProxy* servantToObjectProxy(ServantProxy *servantProxy);

	/**
	 * 是否是协程中的私有通信器
	 * @return
	 */
	inline bool isSchedCommunicatorEpoll() const { return !_public; }

	/**
	 * 初始化notify
	 */
    void initNotify(size_t iSeq, const shared_ptr<ReqInfoQueue> &msgQueue);

    /**
     * 直接通知
     */ 
    inline void handle(uint16_t reqQNo) { handleNotify(_notify[reqQNo]->notify.getEpollInfo()); }

    /**
     * 获取通知句柄(主要用于测试)
     * @return
     */
	FDInfo** getNotify() { return _notify; }

protected:

	/**
	 * 网络线程中处理CommunicatorEpoll退出的清理逻辑
	 */
	void handleTerminate();

	/**
	 * 通知CommunicatorEpoll退出
	 */
	void notifyTerminate();

	/**
	 * 网络线程中处理业务线程退出的清理逻辑
	 */
	void handleServantThreadQuit(uint16_t iSeq);

	/**
	 * 通知业务线程退出
	 */
	void notifyServantThreadQuit(uint16_t iSeq);

    /**
     * 初始化
     * 如果在其他协程中, 并不自己run, 只需要调用该函数初始化epoller即可
     */
    void initializeEpoller();

    /**
     * 上报数据
     * @param pmStatMicMsg
     */
    void report(StatReport::MapStatMicMsg *pmStatMicMsg);

    /**
     * 弹出来统计数据
     * @param mStatMsg
     * @return
     */
    bool popStatMsg(StatReport::MapStatMicMsg* &mStatMsg);

    /**
     * 输入事件
     * @param pi
     */
    bool handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

    /**
     * 输入事件
     * @param pi
     */
    bool handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

    /**
     * 输出事件
     * @param pi
     */
    bool handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

    /**
     * 处理notify
     */
    bool handleNotify(const shared_ptr<TC_Epoller::EpollInfo> & data);

    /**
     * 处理超时
     * @param pi
     */
    void doTimeout();

    /**
     * 处理tars_ping
     * @param pi
     */
    void doKeepAlive();

    /**
     * 处理stat
     * @param pi
     */
    void doStat();

    /**
     * reconnect
     */
    void doReconnect();

    /**
     * 根据序号 获取所有obj对象
     */
    inline ObjectProxy * getObjectProxy(size_t iNum)
    {
		TC_ThreadRLock lock(_vObjectMutex);

        assert(iNum < _objNum);
        return _vObjectProxys[iNum];
    }

    /**
     * 获取所有对象的个数，为了不加锁不用map
     */
    inline size_t getObjNum()
    {
        return _objNum;
    }

    /**
     * 需要上报的stat数据size
     * @return
     */
    inline size_t getReportSize() { return _statQueue.size(); }

    friend class StatReport;
    friend class AdapterProxy;
    friend class Communicator;
    friend class ServantProxy;
    friend class ServantProxyThreadData;
protected:
    /*
     * 通信器
     */
    Communicator *         _communicator;

    /**
     * 是否第一个网络线程
     */
    bool 				    _isFirst = false;

    /**
     * 是否公有的网络线程
     */
    bool 				   _public = false;

    /**
     * notify
     */
    FDInfo*                _notify[MAX_CLIENT_NOTIFYEVENT_NUM];

    /**
     * schedule
     */
    shared_ptr<TC_CoroutineScheduler>  _scheduler;

    /**
     * 独立的网络线程存在, 线程私有数据
     */ 
    ServantProxyThreadData *_pSptd = NULL;
    
    /*
     * epoll
     */
    TC_Epoller *_epoller = NULL;

    /**
     * lock
     */
    TC_ThreadRecMutex              _objectMutex;

	/**
	 * 保存已创建的objectproxy
	 */
    unordered_map<string, ObjectProxy*>    _objectProxys;

    /**
     * _vObjectProxys读写锁
     */
	TC_ThreadRWLocker              	_vObjectMutex;

    /**
     * 保存已经创建的obj 取的时候可以不用加锁
     */
    vector<ObjectProxy *>       	_vObjectProxys;

    /**
     * 读写锁
     */
	TC_ThreadRWLocker              	_servantMutex;

    /**
     * servant对应的objectProxy
     */
    unordered_map<ServantProxy*, ObjectProxy*>  _servantObjectProxy;

    /*
     *保存已经创建obj的数量
     */
    size_t                      _objNum = 0;

    /*
     * 网络线程的id号
     * 私有网络线程: ServantProxyThreadData::_reqQNo, 从0开始计数
     */
    size_t                 _netThreadSeq;

    /*
     * 节点ip队列未发送请求的大小限制
     */
    size_t                 _noSendQueueLimit;
    
    /*
     * 超时的检查时间间隔
     */
    int64_t                _timeoutCheckInterval;

    /**
     * auto reconnect TC_Transceiver
     */
	unordered_map<int64_t, TC_Transceiver*> _reconnect;

	/**
	 * 统计数据
	 */
    TC_LoopQueue<StatReport::MapStatMicMsg*>         _statQueue;

    /**
     * 网络线程ID
     */
    std::thread::id _threadId;

    /**
     * 定时器的id
     */
    vector<int64_t> _timerIds;

	/**
	 * 锁
	 */
	std::mutex             _mutex;
};

/////////////////////////////////////////////////////////////////////////////////////
}

#endif
