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

#ifndef _TARS_SERVANT_PROXY_H_
#define _TARS_SERVANT_PROXY_H_

#include "util/tc_common.h"
#include "util/tc_uuid_generator.h"
#include "util/tc_monitor.h"
#include "util/tc_autoptr.h"
#include "util/tc_proxy_info.h"
#include "util/tc_singleton.h"
#include "servant/Message.h"
#include "servant/AppProtocol.h"
#include "servant/Current.h"
#include "servant/EndpointInfo.h"
#include <tuple>
#include <cmath>
#include <algorithm>
namespace tars
{

class CommunicatorEpoll;
class EndpointInfo;

/////////////////////////////////////////////////////////////////////////

//seq 管理的类
class SeqManager
{
public:
    const static uint16_t MAX_UNSIGN_SHORT = 0xffff;

    struct SeqInfo
    {
        uint16_t next;
        bool     free;
    };

    /**
     * 构造函数
     */
    SeqManager(uint16_t iNum);

    /**
     * 析构
     */ 
    ~SeqManager(); 

    /**
     * 获取seq
     */
    uint16_t get();

    /**
     * 删除seq
     */
    void del(uint16_t iSeq);

private:
    uint16_t    _num;

    uint16_t    _free;

    uint16_t    _freeTail;

    SeqInfo  *  _p;

    TC_SpinLock _mutex;
};

/////////////////////////////////////////////////////////////////////////

/*
 * 线程私有数据
 */
class ServantProxyThreadData : public std::enable_shared_from_this<ServantProxyThreadData>
{
public:
	/**
	 * 全局不死的数据, 私用指针, 且不delete
     * 业务不需要主动使用该对象!
	 */
    class Immortal
    {
	public:
        Immortal();
        ~Immortal();
        void add(ServantProxyThreadData *data);
        void erase(ServantProxyThreadData* data);
		void erase(Communicator * comm);
        unordered_set<ServantProxyThreadData *> getList();

        SeqManager *getSeqManager() { return _pSeq.get(); }

    protected:
		unordered_set<ServantProxyThreadData*> _sp_list;

		TC_ThreadMutex _mutex;

        unique_ptr<SeqManager> _pSeq;
      
    };

    static shared_ptr<Immortal> g_immortal;

public:

    static thread_local shared_ptr<ServantProxyThreadData> g_sp;
    static unsigned int _traceParamMaxLen;

    /**
     * global Immortal ptr, 避免Immortal提前被释放掉
     */
     shared_ptr<Immortal> _sp_immortal;

    /**
     * 构造函数
     */
    ServantProxyThreadData();

    /**
     * 析构函数
     */
    ~ServantProxyThreadData();

    /**
     * 获取线程数据，没有的话会自动创建
     * @return ServantProxyThreadData*
     */
    static ServantProxyThreadData * getData();

    /**
     * 析构通信器时调用
     * @param communicator
     */
    static void deconstructor(Communicator *communicator);

    /**
     * move掉
     */ 
    ThreadPrivateData move();

    /**
     * 业务发起调用的线程和网络通信器间都有一个队列
     */
    struct CommunicatorEpollReqQueueInfo
	{
		weak_ptr<ReqInfoQueue>	_reqQueue;
		weak_ptr<CommunicatorEpoll> _communicatorEpoll;
	};

	//每发起调用的线程 记录的 公有网络通信器数据
	//此时业务线程和
	struct CommunicatorEpollInfo
	{
		/*
		 * 每个线程跟客户端网络线程通信的队列
		 * <网络线程序号, 网络通信信息>
		 */
		vector<CommunicatorEpollReqQueueInfo> _info;
		size_t         _netSeq = 0;                             //轮训选择网络线程的偏移量
		Communicator   *_communicator = NULL;
	};

	/**
	 * 业务线程处于协程模式下, 记录当前网络通信器信息
	 * 此时业务线程和网络通信器是一对一的, 即用自身线程对应的私有网络通信器即可
	 */
	struct SchedCommunicatorEpollInfo
	{
		CommunicatorEpollReqQueueInfo _info;
		Communicator   *_communicator = NULL;
	};

	/**
	 * 初始化当前业务线程和网络通信器之间的关系(构建发送队列)
	 */
	shared_ptr<ServantProxyThreadData::CommunicatorEpollInfo> addCommunicatorEpoll(const shared_ptr<CommunicatorEpoll> &ce);

	/**
	 * 通信器析构时调用
	 * @param communicator
	 */
	void erase(Communicator *communicator);

	/**
	 * 获取公有通信器对应的网络通信器等基本信息
	 * @param communicator
	 * @return
	 */
	shared_ptr<CommunicatorEpollInfo> getCommunicatorEpollInfo(Communicator *communicator);

	/**
	 * 获取私有通信器对应的网络通信器等基本信息
	 * @param communicator
	 * @return
	 */
	shared_ptr<SchedCommunicatorEpollInfo> getSchedCommunicatorEpollInfo(Communicator *communicator);

protected:
	/**
	 * communicator对应的公用网路通信器
	 */
	unordered_map<Communicator*, shared_ptr<CommunicatorEpollInfo>>    _communicatorEpollInfo;

	/**
	 * 私有的网络通信器, 每个业务线程都对应一个, 业务线程是协程模式下使用
	 */
	unordered_map<Communicator*, shared_ptr<SchedCommunicatorEpollInfo>>    _schedCommunicatorEpollInfo;

public:
	//lock
	TC_ThreadMutex _mutex;

	//业务线程的序号, 通知网络线程时, 知道用哪个notify来唤醒网路线程
    uint16_t       _reqQNo;

    /**
     * 协程调度
     */
    shared_ptr<TC_CoroutineScheduler>  _sched;

	/**
	 * 线程私有数据
	 */
	ThreadPrivateData       _data;

	/**
	 * 当前线程是否关联了网络通信器, 如果关联了, 则表示当前线程处于网络线程中!
	 */
	CommunicatorEpoll   	*_communicatorEpoll = NULL;

    ///////////////////////////////////////////////////////////////////////////////////////
    /**
     * 调用链追踪信息
     */
    struct  TraceContext
    {
        int    traceType;               // 取值范围0-15， 0 不用打参数， 其他情况按位做控制开关，从低位到高位分别控制CS、CR、SR、SS，为1则输出对应参数
        unsigned int    paramMaxLen;    // 业务接口参数最大长度，如果超过该值，那么不输出参数，用特殊串标记 {"trace_param_over_max_len":true}
        string traceID;         // traceID
        string spanID;          // spanID
        string parentSpanID;    // 父spanID

        enum E_SpanType
        {
            EST_CS = 1,
            EST_CR = 2,
            EST_SR = 4,
            EST_SS = 8,
            EST_TS,
            EST_TE,
        };

        // 是否输出参数
        enum E_NeedParam
        {
            ENP_NO = 0,
            ENP_NORMAL = 1,
            ENP_OVERMAXLEN = 2, 
        };

        // key 分两种情况，1.rpc调用； 2.异步回调
        // eg: f.2-ee824ad0eb4dacf56b29d230a229c584|030019ac000010796162bc5900000021|030019ac000010796162bc5900000021
        bool init(const string& k)
        {
            vector<string> vs = TC_Common::sepstr<string>(k, "|");
            if (vs.size() == 2)
            {
                traceID = vs[0];
                parentSpanID = vs[1];
                spanID = "";
                auto flags = initType(traceID);
                traceType = std::get<0>(flags);
                paramMaxLen = std::get<1>(flags);
                return true;
            }
            else if (vs.size() == 3)
            {
                traceID = vs[0];
                spanID = vs[1];
                parentSpanID = vs[2];
                auto flags = initType(traceID);
                traceType = std::get<0>(flags);
                paramMaxLen = std::get<1>(flags);
                return true;
            }
            else
            {
                reset();
            }
            return false;
        }

        bool open(const string& k)
        {
            if (!k.empty())
            {
                traceID = k;
                parentSpanID = "";
                spanID = "";
                auto flags = initType(traceID);
                traceType = std::get<0>(flags);
                paramMaxLen = std::get<1>(flags);
                return true;
            }
            
            return false;
        }

        static std::tuple<int, unsigned int> initType(const string& tid)
        {
            int type = 0;
            unsigned int maxLen = ServantProxyThreadData::getTraceParamMaxLen();

            string::size_type  pos = tid.find("-");
            if (pos != string::npos)
            {
                vector<string> flags = TC_Common::sepstr<string>(tid.substr(0, pos), ".");
                if (flags.size() >= 1)
                {
                    type = strtol(flags[0].c_str(), NULL, 16);
                }
                if (flags.size() >= 2)
                {
                    // TODO(greatsong): std::max Win32下编译有问题, 添加NOMAXMIN宏不起作用
                    //maxLen = std::max(maxLen, TC_Common::strto<unsigned int>(flags[1]));
                    auto f = TC_Common::strto<unsigned int>(flags[1]);
                    if (maxLen < f)
                        maxLen = f;
                }
            
                // type = strtol(tid.substr(0, pos).c_str(), NULL, 16);
            }
            if (type < 0 || type > 15)
            {
                type = 0;
            }
            return make_tuple(type, maxLen);
        }

        void reset()
        {
            traceID = "";
            spanID = "";
            parentSpanID = "";
            traceType = 0;
            paramMaxLen = 1;
        }

        TraceContext()
        {
        }
        TraceContext(const string& k)
        {
            init(k);
        }

        void newSpan()
        {
            spanID = TC_UUIDGenerator::getInstance()->genID();
            if (parentSpanID.empty())
            {
                parentSpanID = spanID;
            }
        }

        string getKey(E_SpanType es) const
        {
            switch (es)
            {
                case EST_CS:
                case EST_CR:
                case EST_TS:
                case EST_TE:
                    return traceID + "|" + spanID + "|" + parentSpanID;
                    break;
                case EST_SR:
                case EST_SS:
                    return traceID + "|" + parentSpanID + "|*";
                    break;
                default:
                    break;
            }
            return  "";
        }
        string getKey(bool full) const
        {
            return full ? (traceID + "|" + spanID + "|" + parentSpanID) : (traceID + "|" + spanID);
        }

        // return: 0 不需要参数， 1：正常打印参数， 2：参数太长返回默认串
        static int needParam(E_SpanType es, int type, size_t len, size_t maxLen)
        {
            if (es == EST_TS)
            {
                es = EST_CS;
            }
            else if (es == EST_TE)
            {
                es = EST_CR;
            }
            if (!(bool)((int)es & type))
            {
                return ServantProxyThreadData::TraceContext::ENP_NO;
            }
            else if (len > maxLen * 1024)
            {
                return ServantProxyThreadData::TraceContext::ENP_OVERMAXLEN;
            }

            return ServantProxyThreadData::TraceContext::ENP_NORMAL;
        }
    };

    bool           _traceCall = false;     //标识当前线程是否需要调用链追踪，默认不打开
    TraceContext _traceContext;    //调用链追踪信息

    string getTraceKey(TraceContext::E_SpanType es) const
    {
        return _traceContext.getKey(es);
    }
    string getTraceKey(bool full = false) const
    {
        return _traceContext.getKey(full);
    }
    void newSpan()
    {
        _traceContext.newSpan();
    }
    bool initTrace(const string& k)
    {
        return _traceContext.init(k);
    }
    int getTraceType() const
    {
        return _traceContext.traceType;
    }
    int needTraceParam(TraceContext::E_SpanType es, size_t len)
    {
        return _traceContext.needParam(es, _traceContext.traceType, len, _traceContext.paramMaxLen);
    }

    /* 业务主动打开调用链
    *  @param traceFlag: 调用链日志输出参数控制，取值范围0-15， 0 不用打参数， 其他情况按位做控制开关，从低位到高位分别控制CS、CR、SR、SS，为1则输出对应参数
    *  @param maxLen: 参数输出最大长度， 不传或者默认0， 则按服务模板默认取值
    */
    bool openTrace(int traceFlag = 0, unsigned int maxLen = 0)
    {
        string traceID = TC_UUIDGenerator::getInstance()->genID();
        stringstream ss;
        if (maxLen > 0)
        {
            ss << std::hex << traceFlag << "." << maxLen << "-" << traceID;
        }
        else
        {
            ss << std::hex << traceFlag << "-" << traceID;
        }
        _traceCall = _traceContext.open(ss.str()); 
        return _traceCall;
    }

    static int needTraceParam(TraceContext::E_SpanType es, const string& k, size_t len)
    {
        auto flags = TraceContext::initType(k);
        int type = std::get<0>(flags);
        unsigned int maxLen = std::get<1>(flags);
        return TraceContext::needParam(es, type, len, maxLen);
    }
    static void setTraceParamMaxLen(unsigned int len)
    {
        // 最最大保护，不超过10M
        if (len < 1024 * 10)
        {
            _traceParamMaxLen = len;
        }
    }
    static unsigned int getTraceParamMaxLen()
    {
        return _traceParamMaxLen;
    }
    ////////////////////////////////////////////////////////////////////////////////////调用链追踪 end/////

};

//////////////////////////////////////////////////////////////////////////
// 协程并行请求的基类
class CoroParallelBase : virtual public TC_HandleBase
{
  public:
    /**
     * 构造
     */
    CoroParallelBase(int iNum)
    : _num(iNum)
    , _count(iNum)
    , _req_count(0)
    {}

    /**
     * 析构函数
     */
    virtual ~CoroParallelBase() {}

    /**
     * 增加调用协程接口请求的数目
     */
    int incReqCount() { return (++_req_count); }

    /**
     * 判断协程并行请求数目是否都发送了
     */
    bool checkAllReqSend() { return _num == _req_count; }

    /**
     * 判断协程并行请求是否都回来了
     */
    bool checkAllReqReturn() { return (--_count) == 0; }

    /**
     * 获取所有请求回来的响应
     */
    vector<ReqMessage*> getAllReqMessage()
    {
        vector<ReqMessage*> vRet;

        {
            TC_LockT<TC_SpinLock> lock(_mutex);
            vRet.swap(_vReqMessage);
			_vReqMessage.clear();
        }

        return vRet;
    }

    /**
     * 插入请求回来的响应
     */
    void insert(ReqMessage* msg)
    {
        TC_LockT<TC_SpinLock> lock(_mutex);
        _vReqMessage.push_back(msg);
    }

protected:
    /**
     * 并行请求的数目
     */
    int                     _num;

    /**
     * 并行请求的响应还未回来的数目
     */
    std::atomic<int>        _count;

    /**
     * 并行请求的已发送的数目
     */
    std::atomic<int>        _req_count;

    /**
     * 互斥锁
     */
    TC_SpinLock             _mutex;

    /**
     * 请求的响应的容器
     */
    vector<ReqMessage*>     _vReqMessage;
};
typedef TC_AutoPtr<CoroParallelBase> CoroParallelBasePtr;

//等待所有的请求回来
void coroWhenAll(const CoroParallelBasePtr &ptr);

//////////////////////////////////////////////////////////////////////////
// 异步回调对象的基类
class ServantProxyCallback : virtual public TC_HandleBase
{
public:
    /**
     * 构造
     */
    ServantProxyCallback();

    /**
     * 析构函数
     */
    virtual ~ServantProxyCallback() {}

    /**
     * 设置发起调用的servant
     * @param prx
     */
	void setServantPrx(const ServantPrx &prx) { _servantPrx = prx; }

    /**
     * 获取类型
     * @return const string&
     */
    virtual const string& getType() { return _type; }

    /**
     * 设置类型
     * @return const string&
     */
    virtual void setType(const string& type) { _type = type; }

    /**
     * 设置coro并行请求的共享智能指针
     */
    virtual void setCoroParallelBasePtr(CoroParallelBasePtr pPtr) { _pPtr = pPtr; }

    /**
     * 获取coro并行请求的共享智能指针
     */
    virtual const CoroParallelBasePtr& getCoroParallelBasePtr() { return _pPtr; }

    /**
     * 异步请求是否在网络线程处理
     * 内部用的到 业务不能设置这个值
     * */
    inline void setNetThreadProcess(bool bNetThreadProcess)
    {
        _bNetThreadProcess = bNetThreadProcess;
    }

    inline bool getNetThreadProcess()
    {
        return _bNetThreadProcess;
    }

public:
	/**
	 * dispatch, call onDispatch
	 * @param msg
	 * @return
	 */
	int dispatch(ReqMessagePtr msg);

protected:
    /**
     * 异步回调对象实现该方法，进行业务逻辑处理
     * @param msg
     * @return int
     */
    virtual int onDispatch(ReqMessagePtr msg) = 0;

    /**
     * 连接关闭掉了(push callback 才有效)，老版本的onClose不带ep，为了兼容并且带上ep
     */
    virtual void onClose() {};
    virtual void onClose(const TC_Endpoint& ep) {onClose();};

	/**
	 * 连接已建立(push callback 才有效)
	 */
    virtual void onConnect(const TC_Endpoint& ep) {};

	friend class AdapterProxy;
protected:

    /**
     * 同一链路多个cb的时候可以用来区分class类型
     */
    string _type;

    /**
     * 异步请求是否在网络线程处理
     * 内部用的到 业务不能设置这个值
     * */
    bool _bNetThreadProcess;

    /**
     * 协程并行请求的共享智能指针
     */
    CoroParallelBasePtr _pPtr;

    /**
     * servant prx
     */
	ServantPrx _servantPrx;
};
///////////////////////////////////////////////////////////////////////////////////////////////
// for http
class HttpCallback : public TC_HandleBase
{
public:
    virtual int onHttpResponse(const shared_ptr<TC_HttpResponse> &rsp) = 0;
    virtual int onHttpResponseException(int expCode) = 0;
};

typedef TC_AutoPtr<HttpCallback> HttpCallbackPtr;

class HttpServantProxyCallback : virtual public ServantProxyCallback
{
public:
    explicit HttpServantProxyCallback(const HttpCallbackPtr& cb);

   /**
     * 异步回调对象实现该方法，进行业务逻辑处理
     * @param msg
     * @return int
     */
    virtual int onDispatch(ReqMessagePtr ptr);

    /**
     * 异步回调对象实现该方法，进行业务逻辑处理
     * @param msg
     * @return void
     */
    virtual int onDispatchResponse(const RequestPacket &req, const ResponsePacket &rsp);

    /**
     * 异步回调对象实现该方法(异常)，进行业务逻辑处理
     * @param msg
     * @return void
     */
    virtual int onDispatchException(const RequestPacket &req, const ResponsePacket &rsp);


private:
    HttpCallbackPtr _httpCb;
};

//////////////////////////////////////////////////////////////////////////
/**
 * 1:远程对象的本地代理
 * 2:同名servant在一个通信器中最多只有一个实例
 * 3:防止和用户在Tars中定义的函数名冲突，接口以tars_开头
 */
class EndpointManagerThread;

class ServantProxy : public TC_HandleBase, public TC_ThreadMutex
{
public:
    /**
     * 通过status传递数据时用到的缺省字符串
     */
    static string STATUS_DYED_KEY;  //需要染色的用户ID

    static string STATUS_GRID_KEY;  //需要灰度染色的用户ID

    static string STATUS_RESULT_CODE; //处理结果码,tup使用

    static string STATUS_RESULT_DESC; //处理结果描述,tup使用

    static string STATUS_SETNAME_VALUE; //set调用

    static string STATUS_TRACE_KEY; //trace信息

///////////////////////////////////////////////////////////////////
/**
 * socket选项
 */
	struct SocketOpt
	{
		int level;

		int optname;

		const void *optval;

		SOCKET_LEN_TYPE optlen;
	};
    /**
     * 缺省的同步调用超时时间
     * 超时后不保证消息不会被服务端处理
     */
    enum { DEFAULT_SYNCTIMEOUT = 3000, DEFAULT_ASYNCTIMEOUT=5000};

    /**
     * default connection serial num
     */
    const static int DEFAULT_CONNECTION_SERIAL = 10;

    //自定义回调
    typedef std::function<void(ReqMessagePtr)> custom_callback;

    /**
	 * 内置四种协议支持
	 */
    enum SERVANT_PROTOCOL
    {
    	PROTOCOL_TARS,              //默认tars服务的协议
    	PROTOCOL_HTTP1,             //http协议
	    PROTOCOL_HTTP2,             //http2协议
        PROTOCOL_GRPC,              //grpc协议
    };

    /**
     * 代理设置
     */
    enum SERVANT_PROXY
    {
        PROXY_SOCK4, //支持sock4代理
        PROXY_SOCK5, //支持sock5代理
        PROXY_HTTP,  //支持http代理
    };

    /**
     * 构造函数
     * @param op
     */
    ServantProxy(Communicator * pCommunicator, const string& name,const string& setName);

    /**
     * 析构函数
     */
    virtual ~ServantProxy();

public:
    /**
     * 获取Object可用服务列表 如果启用set则获取本set的,如果启用分组,只返回同分组的服务端ip
     *  @return void
     */
    void tars_endpoints(vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint);

    /**
     * 获取Object可用服务列表 所有的列表
     *  @return void
     */
    void tars_endpointsAll(vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint);

    /**
     * 获取Object可用服务列表 根据set名字获取
     *  @return void
     */
    void tars_endpointsBySet(const string & sName,vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint);

    /**
     * 获取Object可用服务列表 根据地区名字获取
     *  @return void
     */
    void tars_endpointsByStation(const string & sName,vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint);

    /**
     *  获取Object可用服务列表 包括指定归属地
     *  @return vector<TC_Endpoint>
     **/
    vector<TC_Endpoint> tars_endpoints(const std::string & sStation);

    /**
     * 获取Object可用服务列表 包括指定归属地
     *  @return void
     */
    void tars_endpoints(const std::string & sStation, vector<TC_Endpoint> & vecActive, vector<TC_Endpoint> & vecInactive);

    /**
     * 获取Object可用服务列表 如果启用分组,只返回同分组的服务端ip
     * @return vector<TC_Endpoint>
     */
    vector<TC_Endpoint> getEndpoint();

    /**
     * 获取Object可用服务列表 包括所有IDC
     * @return vector<TC_Endpoint>
     */
    vector<TC_Endpoint> getEndpoint4All();

    /**
     * 获取通信器
     *
     * @return Communicator*
     */
    Communicator* tars_communicator() { return _communicator; }

    /**
     * 发送测试消息到服务器
     */
    void tars_ping();

    /**
     * 异步ping, 不等回包
     */
	void tars_async_ping();

    /**
	 * 设置代理
	 * @param type
	 * @param ep
	 * @param user
	 * @param pass
	 */
    void tars_set_proxy(SERVANT_PROXY type, const TC_Endpoint &ep, const string &user, const string &pass);

    /**
	 * 设置同步调用超时时间，对该proxy上所有方法都有效
	 * @param msecond
	 */
    void tars_timeout(int msecond);

    /**
     * 获取同步调用超时时间，对该proxy上所有方法都有效
     * @return int
     */
    int tars_timeout() const;

    /**
     * 获取连接超时时间
     * @return int
     */
    int tars_connect_timeout() const;

    /**
     * 设置连接超时时间
     * @return int
     */
    void tars_connect_timeout(int conTimeout);

	/**
	 * set auto reconnect time
	 * @return int, second
	 */
    void tars_reconnect(int second);

    /**
     * 获取所属的Object名称
     * @return string
     */
    const string &tars_name() const;

    /**
     * set name
     * @return
     */
	const string &tars_setName() const;

	/**
	 * 获取所属的Object名称#hash@address(即传入stringToProxy中的地址)
	 * @return string
	 */
	string tars_full_name() const;

    /**
     * 获取最近一次调用的IP地址和端口
     * @return string
     */
    static TC_Endpoint tars_invoke_endpoint();

	/**
	 * 设置连接为多连接, 串行模式
	 * @param connectionSerial, <=0: 连接复用模式(一个连接上同时跑多个请求, 响应包), >0: 连接串行模式(连接个数), 同一个连接上并行只能跑一个包(http协议)
	 */
	void tars_connection_serial(int connectionSerial);

	/**
	 * 获取连接并发模式
	 * @return int
	 */
	int tars_connection_serial() const;

	/**
	 * 直接设置内置支持的协议
	 */
    void tars_set_protocol(SERVANT_PROTOCOL protocol, int connectionSerial = 0);

    /**
     * 设置用户自定义协议
     * @param protocol
     */
    void tars_set_protocol(const ProxyProtocol& protocol, int connectionSerial = 0);

    /**
     * get protocol
     * @return
     */
	const ProxyProtocol &tars_get_protocol() const;

	/**
    *设置套接字选项
    */
    void tars_set_sockopt(int level, int optname, const void *optval, SOCKET_LEN_TYPE optlen);

    /**
     * 获取套接字选项
     */
    vector<SocketOpt> tars_get_sockopt() const;

    /**
     * 设置超时检查参数
     */
    void tars_set_check_timeout(const CheckTimeoutInfo& checkTimeoutInfo);

    /**
     * 获取超时检查参数
     */
    CheckTimeoutInfo tars_get_check_timeout();

    /**
     * hash方法，为保证一段时间内同一个key的消息发送
     * 到相同的服务端，由于服务列表动态变化，所以
     * 不严格保证
     * @param key
     * @return ServantProxy*
     */
    virtual ServantProxy* tars_hash(int64_t key);

    /**
     * 一致性hash方法
     */
    virtual ServantProxy* tars_consistent_hash(int64_t key);


    /**
     * 打开调用链
     * @param traceParams： 是否输出接口调用参数，默认false， 如果设置为true时 接口调用参数会转为json输出会影响性能
     * @return ServantProxy*
     */
    virtual ServantProxy* tars_open_trace(bool traceParams = false);

    /**
     * 清除当前的Hash数据
     * 空函数 为了兼容以前的
     * @param key
     * @return ServantProxy*
     */
    void tars_clear_hash();

    /**
     * 针对客户端调用接口级别的超时时间设置，包括同步和异步调用
     * 每次接口调用都必须设置，否则取系统默认超时时间
     *
     * @param msecond 单位毫秒
     * @return ServantProxy*
     * 示例: prxy->tars_set_timeout(3000)->sayHello();
     */
    virtual ServantProxy* tars_set_timeout(int msecond);

    /**
     * 设置异步调用超时时间，对该proxy上所有方法都有效
     * @param msecond
     */
    void tars_async_timeout(int msecond);

    /**
     * 获取异步调用超时时间，对该proxy上所有方法都有效
     * @return int
     */
    int tars_async_timeout() const;

    /**
     * 主动更新端口
     * @param active
     * @param inactive
     */
    void tars_update_endpoints(const set<EndpointInfo> &active, const set<EndpointInfo> &inactive);

    /**
	 * 设置自定义回调(注意不在异步回调线程执行, 而是在网络线程中回调, 注意不要阻塞)
     * (这种模式下callback hash无效)
	 * @param callback
	 */
    void tars_set_custom_callback(custom_callback callback);

    /**
     * callback启用hash模式, 根据到服务端连接hash, 即同一个服务端连接过来的请求落入到一个异步回调线程中
     */
    void tars_enable_callback_hash();

    /*
     * 用proxy产生一个该object上的序列号
     * @return uint32_t
     */
    virtual uint32_t tars_gen_requestid();

    /**
     * 设置PUSH类消息的响应callback
     * @param cb
     */
    virtual void tars_set_push_callback(const ServantProxyCallbackPtr& cb);

    /**
     * 获取PUSH类消息的callback对象
     */
    ServantProxyCallbackPtr tars_get_push_callback();

	/**
	 * 超时策略获取和设置
	 * @return CheckTimeoutInfo&
	 */
	inline const CheckTimeoutInfo& tars_check_timeout_info() const { return _checkTimeoutInfo; }

    /**
     * 普通协议同步远程调用
     */
    virtual void rpc_call(uint32_t requestId, const string& sFuncName,
                          const char* buff, uint32_t len, ResponsePacket &rsp);

    /**
     * 普通协议异步调用
     */
    virtual void rpc_call_async(uint32_t requestId, const string& sFuncName,
                                const char* buff, uint32_t len,
                                const ServantProxyCallbackPtr& callback,
                                bool bCoro = false);

	/**
	 * http1/2协议同步远程调用
	 * @param funcName: 调用名称, 这里只是做统计用
	 */
    void http_call(const string &funcName, shared_ptr<TC_HttpRequest> &request, shared_ptr<TC_HttpResponse> &response);

    /**
	 * http1/2协议异步远程调用
	 * @param funcName: 调用名称, 这里只是做统计用
	 */
	void http_call_async(const string &funcName, shared_ptr<TC_HttpRequest> &request, const HttpCallbackPtr &cb, bool bCoro = false);

    /**
     * TARS协议同步方法调用
     */
    shared_ptr<ResponsePacket> tars_invoke(char cPacketType,
                            const string& sFuncName,
                            tars::TarsOutputStream<tars::BufferWriterVector>& buf,
                            const map<string, string>& context,
                            const map<string, string>& status);

    /**
     * TARS协议同步方法调用
     */
    shared_ptr<ResponsePacket> tars_invoke(char cPacketType,
                            const string& sFuncName,
                            const vector<char>& buf,
                            const map<string, string>& context,
                            const map<string, string>& status);

    /**
     * TARS协议异步方法调用
     */
    void tars_invoke_async(char cPacketType,
                                  const string& sFuncName,
                                  tars::TarsOutputStream<tars::BufferWriterVector> &buf,
                                  const map<string, string>& context,
                                  const map<string, string>& status,
                                  const ServantProxyCallbackPtr& callback,
                                  bool bCoro = false);

    /**
     * TARS协议异步方法调用
     */
    void tars_invoke_async(char cPacketType,
                                  const string& sFuncName,
                                  const vector<char> &buf,
                                  const map<string, string>& context,
                                  const map<string, string>& status,
                                  const ServantProxyCallbackPtr& callback,
                                  bool bCoro = false);
	/**
	 * 获取所有objectproxy(包括子servant), 该函数主要给自动测试使用!
	 * @return
	 */
    vector<ObjectProxy*> getObjectProxys();
protected:
    /**
	 * 获得可以复用的servant
	 * @return
	 */
    ServantPrx getServantPrx(ReqMessage *msg);

    /**
     * get proxy info
     */
	inline const std::shared_ptr<TC_ProxyInfo::ProxyBaseInfo>& getProxyInfo() { return _proxyBaseInfo; }

	/**
	 *
	 */
	void tars_initialize(bool rootServant);

    /**
     *
     * @param prx
     * @param f
     */
	void travelObjectProxys(ServantProxy *prx, function<void(ObjectProxy*)> f);

    friend class ServantProxyCallback;
	friend class Communicator;
    friend class ServantProxyFactory;

private:

    /**
     * 获取第一个ObjectProxy
     * @return
     */
    ObjectProxy *getObjectProxy(size_t netThreadSeq = 0);

    /**
     *
     * @param func
     */
    void forEachObject(std::function<void(ObjectProxy*)> func);

private:
    /**
     * 远程方法调用
     * @param req
     * @return int
     */
    void invoke(ReqMessage *msg, bool bCoroAsync = false);

    /**
     * 选择某个servant来发送
     * @param msg
     * @param bCoroAsync
     */
	int servant_invoke(ReqMessage *msg, bool bCoroAsync);

    /**
     * 选取一个网络线程对应的信息
     * @param pSptd
     * @return void
     */
    void selectNetThreadInfo(ServantProxyThreadData *pSptd, ObjectProxy *&pObjProxy, shared_ptr<ReqInfoQueue> &pReqQ);
    /**
     * 检查是否需要设置染色消息
     * @param  req
     */
    void checkDye(RequestPacket& req);

    /**
     * 检查是否需要设置调用链追踪
     * @param  req
     */
    void checkTrace(RequestPacket &req);

    /**
     * 更新endpoint
     * @param active
     * @param inactive
     */
	void onNotifyEndpoints(CommunicatorEpoll *communicatorEpoll, const set<EndpointInfo> & active,const set<EndpointInfo> & inactive);

	/**
	 * 端口不活跃
	 */
    void onSetInactive(const EndpointInfo &ep);
    /**
     * 检查是否需要设置cookie
     * @param  req
     */
    void checkCookie(RequestPacket &req);

private:
    friend class ObjectProxy;
    friend class AdapterProxy;
    friend class CommunicatorEpoll;

    /**
     * 通信器
     */
    Communicator *              _communicator;

    /**
     * 保存ObjectProxy对象的指针数组
     */
    ObjectProxy *               _objectProxy;  //保存ObjectProxy对象的指针数组

    /**
     * 同步调用超时(毫秒)
     */
    int                         _syncTimeout;

    /**
     * 同步调用超时(毫秒)
     */
    int                         _asyncTimeout;

    /**
     * 唯一id
     */
    std::atomic<uint32_t>      _id;

    /**
     * 获取endpoint对象
     */
    std::unique_ptr<EndpointManagerThread> _endpointInfo;
    
    /**
     * 是否在RequestPacket中的context设置主调信息
     */
    bool                    _masterFlag;

    /*
     *最小的超时时间
     */
    int64_t                    _minTimeout;

    /**
     * 最大连接串行数(默认0, 表示连接并行请求)
     */
    int                         _connectionSerial = 0;

    /**
     * 短连接使用http使用
     */
	ServantPrx                  _rootPrx;

    /**
	 *
	 */
    std::atomic<int> _servantId{0};

	/**
	 *
	 */
	std::mutex                  _servantMutex;

	/**
	 *
	 */
	vector<ServantPrx>          _servantList;

	/**
	 * 代理的基本信息
	 */
    std::shared_ptr<TC_ProxyInfo::ProxyBaseInfo>     _proxyBaseInfo;

    /**
	 * custom callback
	 */
    custom_callback _callback;

    /**
     * callback hash
     */
    bool _callbackHash = false;

    /**
     * 链接超时
     */
    int _connTimeout = DEFAULT_ASYNCTIMEOUT;

	/*
	 * 请求和响应的协议解析器
	 */
	ProxyProtocol                         _proxyProtocol;

	/*
	 * push消息 callback
	 */
	ServantProxyCallbackPtr               _pushCallback;

	/*
	 * 超时控制策略信息
	 */
	CheckTimeoutInfo                      _checkTimeoutInfo;

	/*
	 * socket选项
	 */
	vector<SocketOpt>                     _socketOpts;

};
}
#endif
