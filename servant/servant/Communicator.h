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

#ifndef __TARS_COMMUNICATOR_H_
#define __TARS_COMMUNICATOR_H_

#include "util/tc_thread.h"
#include "util/tc_config.h"
#include "util/tc_singleton.h"
#include "servant/Global.h"
#include "servant/ServantProxy.h"
#include "servant/ServantProxyFactory.h"
//#include "servant/ObjectProxyFactory.h"
#include "servant/AsyncProcThread.h"
// #include "servant/CommunicatorEpoll.h"
#include "servant/StatReport.h"
#include "servant/RemoteLogger.h"

#include "util/tc_openssl.h"

// #ifdef TARS_OPENTRACKING
// #include "zipkin/opentracing.h"
// #include "zipkin/tracer.h"
// #include "zipkin/ip_address.h"
// #endif
// 
const static string CONFIG_ROOT_PATH = string("/tars/application/client");

/**
 * 设计核心:
 * - 设计的核心是协程化, 如果本身就处于协程状态下, rpc网络通信就复现当前的协程调度器, 从而网络收发逻辑和rpc都在一个线程中处理, 减少线程切换, 降低延时!!!
 * - 如果发起rpc的线程不是协程, 则请求丢给了实际的网络线程处理, 和之前版本保持一致
 * - 结合到服务器端的模型, 这样设计的好处是, 如果都处于协程模式, 客户端和服务器可以复用相同的协程调度器, 从而保证服务器接受请求, 发起rpc, 接收rpc响应, 到回包给客户端, 都在一个线程中处理, 无线程切换逻辑!
 * - 从而大幅度降低了系统延时!
 *
 * 基本说明:
 * - 通信器Communicator是包含了所有客户端调用的资源, 原则上在调用生命周期都必须存在
 * - 每个通信器都包含多个CommunicatorEpoll, 这个是Communicator创建是构建出来的, 此时CommunicatorEpoll的个数和客户端网络线程相同, 至少有1个
 * - 把这种初始就生成的CommunicatorEpoll作为公有的, 后续可能会动态创建私有的CommunicatorEpoll
 * - 无论公有还是私有CommunicatorEpoll, 网络层都用的协程调度器中的epoller对象, 这样方便复用(尤其是针对私有CommunicatorEpoll)
 * - 私有CommunicatorEpoll并不是完整的网路线程, 它复用了业务线程中的协程调度器(这里业务线程可能是服务器端的业务线程/协程, 自带了协程调度器), 参见后续说明
 * - 公有和私有的CommunicatorEpoll都会存在于Communicator对象中
 * - 无论是公有还是私有的CommunicatorEpoll的生命周期由通信器来管理, 通信器析构时会被释放
 * - 一旦Communicator被释放, 它包含的所有资源都被释放了, 由它创建的ServantPrx, 都不能再进行网络通信!!!
 * - ServantProxy针对每个通信器而言, 是全局唯一的, 根据stringToProxy传入的obj来唯一确定
 * - 调用Communicator::stringToProxy, 构建ServantProxy时, 会调用每个CommunicatorEpoll创建对应的ObjectProxy
 * - 即每个ObjectProxy实例唯一对应了一个CommunicatorEpoll, 即代表了网络收发处理线程
 * - ObjectProxy的生命周期被CommunicatorEpoll管理, 当CommunicatorEpoll释放时, ObjectProxy会被释放
 * - 实际的rpc调用, 虽然调用的是ServantProxy, 但是实际会选择具体发送的ObjectProxy(即选择了CommunicatorEpoll) 和 发送队列
 * - 这个发送队列即: 在业务线程 和 CommunicatorEpoll 存在一个无锁的队列(限制长度, 每个元素是ReqMessage, 代表本次请求)
 * - 这个无锁队列, 被业务线程的私有数据管理, 当第一次使用某个CommunicatorEpoll时, 创建出来, 它的析构是复杂逻辑, 参考后续逻辑!
 * - 业务线程退出时, 会导致线程私有数据析构, 析构时发消息给CommunicatorEpoll, 在网络线程中释放资源
 * - 所以框架要求, 业务线程先释放, 才能释放框架的网路线程
 *
 * CommunicatorEpoll设计说明
 * 1 当业务线程处于协程中, 只使用私有CommunicatorEpoll(不使用的公用的)
 * - 基于当前协程的调度器TC_CoroutineScheduler, 全新创建CommunicatorEpoll
 * - 使用该CommunicatorEpoll来处理网络请求, 这样复用相同的调度器(该调度器可能是服务端线程)
 * - 这样所有网络请求都在同一个线程里面处理了
 * - 该调度器, 保存在线程私有对象中(和Communicator指针关联), 同时也保存在Communicator中
 * - 当Communicator对象析构时, 主动释放该CommunicatorEpoll
 * - 创建CommunicatorEpoll时, 注意需要clone所有的ObjectProxy(从公有CommunicatorEpoll中复制), 并选择返回对应的ObjectProxy
 * - 使用ObjectProxy来发送数据
 * - 这种场景下, 数据收发其实都在业务线程中处理了!!!
 * 2 当业务线程处于普通线程中(不存在协程调度器), 只使用公有CommunicatorEpoll
 * - 轮询选择公有的CommunicatorEpoll的, 注意此时不选择私有CommunicatorEpoll来发送数据, 降低系统的复杂度
 * - 轮询的计数器保持在线程私有数据中
 *
 * 析构问题处理
 * - 通信器是管理客户端资源的对象
 * - 通信器析构 以及 业务线程(发起到rpc调用)退出时, 如何处理相关资源释放的是重点需要考虑的
 * - 这里最核心的处理是 业务线程和CommunicatorEpoll之前有发送队列, 这个发送队列由业务线程私有数据保持, 它的问题在于:
 * 1 如果业务线程先退出, 将发送队列先析构, 但是如果此时网络线程仍在使用, 则会有问题
 * 2 如果网络线程先退出, 业务线程退出时, 拿到的CommunicatorEpoll指针可能有问题!
 * - 解决方案:
 * 1 CommunicatorEpoll使用shared_ptr, 业务线程私有数据中持有CommunicatorEpoll时, 采用weak_ptr, 这样业务线程退出时能感知CommunicatorEpoll是否还存在
 * 2 如果业务线程退出时, CommunicatorEpoll不存在了, 直接delete掉发送队列即可
 * 3 如果业务线程退出时, CommunicatorEpoll仍然存在, 发送通知给CommunicatorEpoll, 通知它业务线程退出
 * 4 发送队列使用shared_ptr被线程私有数据持有, 同时它作为weak_ptr被notify通知对象持有
 * 5 网络线程收到notify以后, 获取发送队列的weak_ptr, 转成shared_ptr以后才使用, 保证有效性, 如果转换后shared_ptr为NULL, 表示业务线程已经退出了, 此时可以不需要做任何处理
 * 6 如果notify对象释放时(CommunicatorEpoll析构), 会把发送队列中的数据清空, delete msg
 * 7 如果通信器先析构, 实际上会有一定的泄露(非常少), 线程私有变量中记录通信器信息的map不会删除记录(直到业务线程退出才会释放掉), 这里其实有资源泄露, 但是极少, 可以不管, 除非代码不断在构造和析构通信器!
 *
 * ObjectProxy创建的问题
 * - ServantProxy对象, 对每个服务而言, 是全局唯一的, 它背后对应的ObjectProxy, 是每个网络线程/协程都有一个, 即CommunicatorEpoll内部每个ServantProxy都对应了一个ObjectProxy
 * - 对于公有的CommunicatorEpoll, 它内部的ObjectProxy是stringToProxy时, 自动创建出来的
 * - 对于私有CommunicatorEpoll, 它内部的ObjectProxy是ServantProxy在invoke的时候创建出来的, 这样由于调用逻辑的原因, 私有CommunicatorEpoll内部拥有的ObjectProxy是不一样的!
 * - 私有CommunicatorEpoll内部ObjectProxy不一样, 导致了后需要更新ip list的机制不同
 *
 * ObjectProxy服务地址更新的问题
 * - 由于一个进程中CommunicatorEpoll可能会有多个(公有的+私有的), 从而会有多个ObjectProxy, 带来多次更新主控的问题
 * - 为了避免这种现象, 设计上目前只有公有CommunicatorEpoll且netThreadSeq==0的(isFirstNetThread), 才回主动更新主控
 * - 当第一个公有CommunicatorEpoll更新主控, 获取到服务的ip list之后, 会遍历所有CommunicatorEpoll, 通知所有CommunicatorEpoll里面对应的ObjectProxy去更新这个ip list
 * - 注意私有CommunicatorEpoll内部, 可能不存在这个ObjectProxy, 可能就不要更新ip list了, 需要特殊判断
 * - 私有CommunicatorEpoll中的ObjectProxy不会主动更新主控
 * -
 */


namespace tars
{

class CommunicatorEpoll;
class TC_OpenSSL;

////////////////////////////////////////////////////////////////////////
/**
 * 客户端配置
 */
struct ClientConfig
{
    /**
     * 客户端IP地址
     */
    static string          LocalIp;
    /**
     * 客户端模块名称
     */
    static string          ModuleName;
    /**
     * 客户端所有的IP地址
     */
    static set<string>     SetLocalIp;
   /**
   *客户端是否打开set分组
   */
   static bool             SetOpen;
   /**
   *客户端set分组
   */
   static string           SetDivision;

   /**
    * 客户端的版本号
    */
   static string           TarsVersion;
};

////////////////////////////////////////////////////////////////////////
/**
 * 通信器,用于创建和维护客户端proxy
 */
class SVT_DLL_API Communicator : public TC_HandleBase, public TC_ThreadRecMutex
{
public:

	typedef std::function<void(ReqMessagePtr)> custom_callback;

   /**
    * 构造函数
    */
   Communicator();

    /**
     * 采用配置构造
     * @param conf
     * @param path
     */
    Communicator(TC_Config& conf, const string& domain = CONFIG_ROOT_PATH);

    /**
     * 析够
     * 析够时自动接收相关线程
     */
    ~Communicator();

public:
   /**
    * 生成代理
    * @param T
    * @param objectName
    * @param setName 指定set调用的setid
    * @return T
    */
   template<class T> T stringToProxy(const string& objectName, const string& setName = "")
   {
      T prx = NULL;

      stringToProxy<T>(objectName, prx, setName);

      return prx;
   }

   /**
    * 生成代理
    * @param T
    * @param objectName
    * @param setName 指定set调用的setid
    * @param proxy
    */
   	template<class T> void stringToProxy(const string& objectName, T& proxy, const string& setName = "")
   	{
        ServantProxy *pServantProxy = getServantProxy(objectName, setName, true);
        proxy = (typename T::element_type *)(pServantProxy);
    }

    /**
     * 获取公有网络线程个数
     * @return
     */
    inline size_t getCommunicatorEpollNum()
    {
		return _communicatorEpoll.size();
    }

     /*
     *获取公有网络线程的对象
     */
    inline const shared_ptr<CommunicatorEpoll> &getCommunicatorEpoll(size_t iNum)
    {
        assert(iNum < getCommunicatorEpollNum());
        return _communicatorEpoll[iNum];
    }

    /**
     * 获取所有的网络通信器(包括公有和私有的)
     * @return
     */
	vector<shared_ptr<CommunicatorEpoll>> getAllCommunicatorEpoll();

    /**
     * 获取属性
     * @param name
     * @param dft, 缺省值
     * @return string
     */
    string getProperty(const string& name, const string& dft = "");

    /**
     * 设置属性
     * @param properties
     */
    void setProperty(const map<string, string>& properties);

    /**
     * 设置某一个属性
     * @param name
     * @param value
     */
    void setProperty(const string& name, const string& value);

    /**
     * 设置属性
     * @param conf
     * @param path
     */
    void setProperty(TC_Config& conf, const string& domain = CONFIG_ROOT_PATH);

    /**
     * get servant property
     * @param sObj
     * @return
     */
	map<string, string> getServantProperty(const string &sObj);

	/**
	 * set servant property
	 * @param sObj
	 * @return
	 */
	void setServantProperty(const string &sObj, const string& name, const string& value);

	/**
	 * get servant property
	 * @param sObj
	 * @return
	 */
	string getServantProperty(const string &sObj, const string& name);

    /**
     * 上报统计
     * @return StatReport*
     */
    StatReport * getStatReport();

    /**
     * 重新加载属性
     */
    int reloadProperty(string & sResult);
    
    /*
    * 重新加载locator
    */
    void reloadLocator();

    /**
     * 获取obj对应可用ip port列表  如果启用分组,只返回同分组的服务端ip
     * @param sObjName
     * @return vector<TC_Endpoint>
     */
    vector<TC_Endpoint> getEndpoint(const string & objName);

   /**
    * 获取obj对应可用ip port列表 包括所有IDC的
    * @param sObjName
    * @return vector<TC_Endpoint>
    */
   vector<TC_Endpoint> getEndpoint4All(const string& objName);

    /**
     * 结束
     */
    void terminate();

    /**
     * 超时请求的回包回来后，是否打印超时的日志信息，AdapterProxy里用到
     */
    bool getTimeoutLogFlag() { return _timeoutLogFlag; }

    /**
     * 获取最小的超时时间
     */
    int64_t getMinTimeout() { return _minTimeout; }

    /*
     * tars_ping间隔，0：不启用
     */
    int64_t getKeepAliveInterval() { return _keepAliveInterval; }

    /**
     * get resource info
     * @return
     */
	string getResourcesInfo();

	/**
	 * 是否析构中
	 * @return bool
	 */
	bool isTerminating();

protected:
    /**
     * 初始化
     */
    void initialize();

    /**
	 * 生成代理
	 * @param T
	 * @param objectName
	 * @param setName 指定set调用的setid
	 * @return T
	 */
    template<class T> T stringToProxy(const string& objectName, const string& setName, bool rootServant)
    {
    	T prx = NULL;

    	stringToProxy<T>(objectName, prx, setName, rootServant);

    	return prx;
    }

	/**
	 * 生成代理
	 * @param T
	 * @param objectName
	 * @param setName 指定set调用的setid
	 * @param proxy
	 */
	template<class T> void stringToProxy(const string& objectName, T& proxy, const string& setName, bool rootServant)
	{
		ServantProxy *pServantProxy = getServantProxy(objectName, setName, rootServant);
		proxy = (typename T::element_type *)(pServantProxy);
	}

    /**
     * 获取对象代理生成器
     * @return ServantProxyFactoryPtr
     */
    ServantProxyFactory * servantProxyFactory();

    /**
     * 获取通用对象
     * @param objectName
     * @param setName 指定set调用的setid
     * @return ServantPrx
     */
    ServantProxy * getServantProxy(const string& objectName,const string& setName, bool rootServant);

    /**
     * 数据加入到异步线程队列里面
     * @return
     */
    void pushAsyncThreadQueue(ReqMessage * msg);

    /**
     * 上报统计事件
     * @return
     */
    void doStat();

	/**
	 * get openssl of trans
	 * @param sObjName
	 * @return vector<TC_Endpoint>
	 */
	shared_ptr<TC_OpenSSL> newClientSSL(const string & objName);

    /**
     * 设置调用链控制参数
     * @param name: 参数名
     */
    void setTraceParam(const string& name = "");

    /**
     * 通信器启动
     */ 
    void  notifyCommunicatorEpollStart();

	/**
	 *
	 * @param func
	 */
	void forEachSchedCommunicatorEpoll(std::function<void(const shared_ptr<CommunicatorEpoll> &)> func);

	/**
	 * 创建一个协程内的网络通信器
	 * @return
	 */
	shared_ptr<CommunicatorEpoll> createSchedCommunicatorEpoll(size_t netThreadSeq,  const shared_ptr<ReqInfoQueue> &reqInfoQueue);

	/**
	 * 删除协程内网络通信器
	 * @param netThreadSeq
	 */
	void eraseSchedCommunicatorEpoll(size_t netThreadSeq);

	/**
     * 框架内部需要直接访问通信器的类
     */
    friend class AdapterProxy;

    friend class ServantProxy;

    friend class ObjectProxy;

    friend class ServantProxyFactory;

    friend class ObjectProxyFactory;

    friend class AsyncProcThread;

    friend class CommunicatorEpoll;

	friend class ServantProxyThreadData;

protected:
    /**
     * 是否初始化
     */
    bool  _initialized;

    /**
     * 停止标志
     */
    bool  _terminating;

    /**
     * 客户端的属性配置
     */
    map<string, string>    _properties;

    /**
     * obj info
     */
    map<string, map<string, string>>   _objInfo;

	/**
     * ServantProxy代码的工厂类
     */
    ServantProxyFactory* _servantProxyFactory;

    /*
     * 公有网络线程
     */
    vector<shared_ptr<CommunicatorEpoll>>    _communicatorEpoll;//[MAX_CLIENT_THREAD_NUM];

    /**
     * 私有网络线程, 会动态变化
     */
    unordered_map<size_t, shared_ptr<CommunicatorEpoll>>	_schedCommunicatorEpoll;

    /**
     * 操作通信器的锁
     */
    TC_SpinLock			_schedMutex;

    /**
     * 锁
     */ 
    std::mutex             _mutex;

    /**
     * 条件变量, 用来等待网络线程启动
     */
	std::condition_variable _cond;
    
    /**
     * 通信器启动个数
     */ 
    std::atomic<size_t>    _communicatorEpollStartNum{0};

    /*
     * 上报类
     */
    StatReport *           _statReport;

    /*
     * 超时请求的回包回来后，是否打印超时的日志信息，AdapterProxy里用到
     */
    bool                   _timeoutLogFlag;

    /*
     * 最小的超时时间
     */
    int64_t                _minTimeout;

    /*
     * tars_ping间隔，0：不启用
     */
    int64_t                _keepAliveInterval;

	/**
	 * ssl ctx
	 */
	shared_ptr<TC_OpenSSL::CTX> _ctx;

	/**
	 * ssl
	 */
	unordered_map<string, shared_ptr<TC_OpenSSL::CTX>> _objCtx;

    /*
     * 异步线程数组
     */
    //异步线程(跨通信器共享)
    vector<AsyncProcThread*> _asyncThread;

    /*
     * 异步队列的统计上报的对象
     */
    PropertyReportPtr        _reportAsyncQueue;

    /*
     * 异步线程数目
     */
    size_t                 _asyncThreadNum;
    /*
     * 分发给异步线程的索引seq
     */
    size_t                 _asyncSeq = 0;

    /**
     * 注册事件
     */
    size_t 					_sigId = -1;
	
// #ifdef TARS_OPENTRACKING
// public:
//     struct TraceManager:public TC_HandleBase{
//         zipkin::ZipkinOtTracerOptions _zipkin_options;
//         std::shared_ptr<opentracing::Tracer> _tracer;
//         TraceManager(): _tracer(nullptr){}
//         TraceManager(zipkin::ZipkinOtTracerOptions& options):_zipkin_options(options){
//             _tracer = zipkin::makeZipkinOtTracer(options);
//         }
//         ~TraceManager(){if(_tracer != nullptr){_tracer->Close();}}
//     };

//     TC_AutoPtr<TraceManager> _traceManager;
// #endif

};
////////////////////////////////////////////////////////////////////////
}
#endif
