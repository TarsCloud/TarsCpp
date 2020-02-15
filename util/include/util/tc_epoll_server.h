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

#ifndef __TARS_TC_EPOLL_SERVER_H_
#define __TARS_TC_EPOLL_SERVER_H_

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include "util/tc_epoller.h"
#include "util/tc_thread.h"
#include "util/tc_clientsocket.h"
#include "util/tc_logger.h"
#include "util/tc_common.h"
#include "util/tc_network_buffer.h"
#include "util/tc_cas_queue.h"

#if TARS_SSL
#include "util/tc_openssl.h"
#endif

using namespace std;

namespace tars
{

/////////////////////////////////////////////////
/**
 * @file  tc_epoll_server.h
 * @brief  EpollServer类
 *
 */
/////////////////////////////////////////////////
/**
 * Server基类
 * 注册协议解析器
 * 注册逻辑处理器
 * 注册管理端口处理器
 */

class PropertyReport;

class TC_EpollServer : public TC_ThreadLock, public TC_HandleBase
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /**
     * 定义协议解析的返回值
     */
    enum EM_CLOSE_T
    {
        EM_CLIENT_CLOSE = 0,         //客户端主动关闭
        EM_SERVER_CLOSE = 1,        //服务端业务主动调用close关闭连接,或者框架因某种异常主动关闭连接
        EM_SERVER_TIMEOUT_CLOSE = 2  //连接超时了，服务端主动关闭
    };

    enum
    {
        MIN_EMPTY_CONN_TIMEOUT  = 2*1000,    /*空链接超时时间(ms)*/
        DEFAULT_RECV_BUFFERSIZE = 64*1024    /*缺省数据接收buffer的大小*/
    };

    //定义加入到网络线程的fd类别
    enum CONN_TYPE
    {
        TCP_CONNECTION = 0,
        UDP_CONNECTION = 1,
    };
    /**
     * 定义协议解析接口的操作对象
     * 注意必须是线程安全的或是可以重入的
     */
    // using protocol_functor = std::function<TC_NetWorkBuffer::PACKET_TYPE(string&, string&)>;
    // using header_filter_functor = std::function<int (int, string&)>;
    // using conn_protocol_functor = std::function<TC_NetWorkBuffer::PACKET_TYPE(string&, string&, void*)>;

	typedef std::function<TC_NetWorkBuffer::PACKET_TYPE(TC_NetWorkBuffer::PACKET_TYPE, vector<char>&)> header_filter_functor;

    class NetThread;
    class Connection;
    class BindAdapter;
    typedef TC_AutoPtr<BindAdapter> BindAdapterPtr;

    class Handle;
    typedef TC_AutoPtr<Handle> HandlePtr;

	class RecvContext;
	/**
	 * 发送包的上下文
	 * 由RecvContext创建出来
	 */
	class SendContext
	{
	public:
		SendContext(const shared_ptr<RecvContext> &context, char cmd) : _context(context), _cmd(cmd)
		{
			_sbuffer = std::make_shared<TC_NetWorkBuffer::Buffer>();
		}

		const shared_ptr<RecvContext> &getRecvContext() { return _context; }
		const shared_ptr<TC_NetWorkBuffer::Buffer> & buffer()       { return _sbuffer; }
		char cmd() const        { return _cmd; }
		uint32_t uid() const    { return _context->uid(); }
		int fd() const          { return _context->fd(); }
		const string &ip() const { return _context->ip(); }
		uint16_t port() const   { return _context->port(); }

		friend class RecvContext;

	protected:
		shared_ptr<RecvContext>     _context;
		char                        _cmd;            /**send包才有效, 命令:'c',关闭fd; 's',有数据需要发送*/
		shared_ptr<TC_NetWorkBuffer::Buffer> _sbuffer;        /**发送的内容*/
	};

	////////////////////////////////////////////////////////////////////////////
	/**
	 * 接收包的上下文
	 */
	class RecvContext : public std::enable_shared_from_this<RecvContext>
	{
	public:
		RecvContext(uint32_t uid, const string &ip, int64_t port, int fd, const BindAdapterPtr &adapter, bool isClosed = false, int closeType = EM_CLIENT_CLOSE)
			: _uid(uid), _ip(ip), _port(port), _fd(fd), _adapter(adapter), _isClosed(isClosed), _closeType(closeType), _recvTimeStamp(TNOWMS)
		{
		}
		uint32_t uid()       const       { return _uid; }
		const string &ip()   const       { return _ip; }
		uint16_t port()      const       { return _port; }
		vector<char> &buffer()           { return _rbuffer; }
		const vector<char> &buffer()  const { return _rbuffer; }

		int64_t recvTimeStamp() const    { return _recvTimeStamp; }
		bool isOverload()       const    { return _isOverload; }
		void setOverload()               { _isOverload = true; }
		bool isClosed()         const    { return _isClosed; }
		int fd()             const       { return _fd; }
		BindAdapterPtr &adapter()    { return _adapter; }
		int closeType()      const       { return _closeType; }
		void setCloseType(int closeType) { _closeType = closeType;}

		shared_ptr<SendContext> createSendContext()  { return std::make_shared<SendContext>(shared_from_this(), 's'); }
		shared_ptr<SendContext> createCloseContext() { return std::make_shared<SendContext>(shared_from_this(), 'c'); }

//		int64_t         _recvUS;

	protected:
		uint32_t        _uid;            /**连接标示*/
		string          _ip;             /**远程连接的ip*/
		uint16_t        _port;           /**远程连接的端口*/
		int				_fd;				/*保存产生该消息的fd，用于回包时选择网络线程*/
		BindAdapterPtr  _adapter;        /**标识哪一个adapter的消息*/
		vector<char>    _rbuffer;        /**接收的内容*/
		bool            _isOverload = false;     /**是否已过载 */
		bool            _isClosed   = false;       /**是否已关闭*/
		int             _closeType;     /*如果是关闭消息包，则标识关闭类型,0:表示客户端主动关闭；1:服务端主动关闭;2:连接超时服务端主动关闭*/
		int64_t         _recvTimeStamp;  /**接收到数据的时间*/
	};

	typedef TC_CasQueue<shared_ptr<RecvContext>> recv_queue;
	typedef TC_CasQueue<shared_ptr<SendContext>> send_queue;
	typedef recv_queue::queue_type recv_queue_type;

    ////////////////////////////////////////////////////////////////////////////
    /**
     * 链接状态
     */
    struct ConnStatus
    {
        string          ip;
        int32_t         uid;
        uint16_t        port;
        int             timeout;
        int             iLastRefreshTime;
    };
    ////////////////////////////////////////////////////////////////////////////
    /**
     * @brief 定义服务逻辑处理的接口
     *
     */
    /**
     * 服务的逻辑处理代码
     */
    class Handle : public TC_Thread, public TC_ThreadLock, public TC_HandleBase
    {
    public:
        /**
         * 构造, 默认没有请求, 等待10s
         */
        Handle();

        /**
         * 析构函数
         */
        virtual ~Handle();

        /**
         * 设置服务
         * @param pEpollServer
         */
        void setEpollServer(TC_EpollServer *pEpollServer);

        /**
         * 获取服务
         * @return TC_EpollServer*
         */
        TC_EpollServer* getEpollServer();

		/**
		 * 获取Handle的索引(0~handle个数-1)
		 * @return
		 */
		uint32_t getHandleIndex() const { return _handleIndex; }

		/**
		 * 设置网络线程
		 */
		void setNetThread(NetThread *netThread);

		/**
		 * 获取网络线程
		 * @return
		 */
		NetThread *getNetThread() { return _netThread; }

		/**
		 * 处理
		 */
		void process(shared_ptr<RecvContext> data);

        /**
         * 线程处理方法
         */
        virtual void run();

    public:
        /**
         * 发送数据
         * @param stRecvData
         * @param sSendBuffer
         */
		void sendResponse(const shared_ptr<SendContext> &data);

        /**
         * 关闭链接
         * @param stRecvData
         */
        void close(const shared_ptr<RecvContext> &data);

        /**
         * 设置等待时间
         * @param iWaitTime
         */
        void setWaitTime(uint32_t iWaitTime);

        /**
         * 对象初始化
         */
        virtual void initialize() {};

        /**
         * 唤醒HandleGroup中的handle线程
         */
        virtual void notifyFilter();

        /**
         * 心跳(每处理完一个请求或者等待请求超时都会调用一次)
         */
        virtual void heartbeat() {}

    protected:
        /**
         * 具体的处理逻辑
         */
        virtual void handleImp();

		/**
		 * 处理函数
		 * @param stRecvData: 接收到的数据
		 */
		virtual void handle(const shared_ptr<RecvContext> &data) = 0;

		/**
		 * 处理超时数据, 即数据在队列中的时间已经超过
		 * 默认直接关闭连接
		 * @param stRecvData: 接收到的数据
		 */
		virtual void handleTimeout(const shared_ptr<TC_EpollServer::RecvContext> &data);

		/**
		 * 处理连接关闭通知，包括
		 * 1.close by peer
		 * 2.recv/send fail
		 * 3.close by timeout or overload
		 * @param stRecvData:
		 */
		virtual void handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data);

		/**
		 * 处理overload数据 即数据队列中长度已经超过允许值
		 * 默认直接关闭连接
		 * @param stRecvData: 接收到的数据
		 */
		virtual void handleOverload(const shared_ptr<TC_EpollServer::RecvContext> &data);

        /**
         * 处理Filter的消息
         */
        //virtual void handleFilter() {};
        /**
         * 处理异步回调队列
         */
        virtual void handleAsyncResponse() {}
           /**
         * handleFilter拆分的第二部分，处理用户自有数据
         * 非游戏逻辑可忽略bExpectIdle参数
         */
        virtual void handleCustomMessage(bool bExpectIdle = false) {}

        /**
         * 线程已经启动, 进入具体处理前调用
         */
        virtual void startHandle() {}

        /**
         * 线程马上要退出时调用
         */
        virtual void stopHandle() {}

        /**
         * 是否所有的Adpater队列都为空
         * @return bool
         */
        virtual bool allAdapterIsEmpty();

        /**
         * 是否所有的servant都没有resp消息待处理
         * @return bool
         */
        virtual bool allFilterIsEmpty();

		/**
		 * 设置Adapter
		 * @param pEpollServer
		 */
		void setBindAdapter(BindAdapter*  bindAdapter);

		/**
		 * 设置index
		 * @param index
		 */
		void setHandleIndex(uint32_t index);

		/**
		 * 等待在队列上
		 */
		void wait();

		/**
		 * 从队列中获取数据
		 * @param recv
		 * @return
		 */
		bool popRecvQueue(shared_ptr<RecvContext> &recv);

        /**
         * 友元类
         */
        friend class BindAdapter;
    protected:
        /**
         * 服务
         */
        TC_EpollServer  *_pEpollServer;


		/**
		 * handle对应的网路线程(网络线程和handle线程合并的情况下有效)
		 */
		NetThread       *_netThread = NULL;

		/**
		 * 所属handle组
		 */
		BindAdapter*    _bindAdapter;

		/**
		 * 等待时间
		 */
		uint32_t        _iWaitTime;

		/**
		 * Handle的索引
		 */
		uint32_t        _handleIndex;

        /**
         * 所属handle组
         */
        // HandleGroupPtr _handleGroup;
    };

    using close_functor = std::function<void (void*, EM_CLOSE_T )>;
    using auth_process_wrapper_functor = std::function<bool (Connection *c, const shared_ptr<RecvContext> &recv )>;

    ////////////////////////////////////////////////////////////////////////////
    // 服务端口管理,监听socket信息
    class BindAdapter : public TC_ThreadLock, public TC_HandleBase
    {
    public:
        /**
         * 缺省的一些定义
         */
        enum
        {
            DEFAULT_QUEUE_CAP       = 10*1024,    /**流量*/
            MIN_QUEUE_TIMEOUT       = 3*1000,     /**队列最小超时时间(ms)*/
            DEFAULT_MAX_CONN        = 1024,       /**缺省最大连接数*/
            DEFAULT_QUEUE_TIMEOUT   = 60*1000,    /**缺省的队列超时时间(ms)*/
        };
        /**
         * 顺序
         */
        enum EOrder
        {
            ALLOW_DENY,
            DENY_ALLOW
        };

		/**
		 * 数据队列
		 */
		struct DataQueue
		{
			/**
			 * 接收的数据队列
			 */
			recv_queue      _rbuffer;

			/**
			 * 锁
			 */
			TC_ThreadLock   _monitor;
		};

        /**
         * 构造函数
         */
        BindAdapter(TC_EpollServer *pEpollServer);

        /**
         * 析够函数
         */
        ~BindAdapter();

		/**
         * 设置需要手工监听
         */
		void enableManualListen() { _manualListen = true; }

		/**
		 * 是否手工监听端口
		 * @return
		 */
		bool isManualListen() const { return _manualListen; }

		/**
         * 手工绑定端口
         */
		void manualListen();

        /**
         * 设置adapter name
         * @param name
         */
        void setName(const string &name);

        /**
         * 获取adapter name
         * @return string
         */
        string getName() const;

		/**
		 * 增加处理线程对应的接收队列
		 * @return string
		 */		
		void initThreadRecvQueue(uint32_t handeIndex);

        /**
         * 获取queue capacity
         * @return int
         */
        int getQueueCapacity() const;

        /**
         * 设置queue capacity
         * @param n
         */
        void setQueueCapacity(int n);

        /**
         * 设置协议名称
         * @param name
         */
        void setProtocolName(const string& name);

        /**
         * 获取协议名称
         * @return const string&
         */
        const string& getProtocolName();

        /**
         * 是否tars协议
         * @return bool
         */
        bool isTarsProtocol();

        /**
         * 判断是否需要过载保护
         * @return bool
         */
        int isOverloadorDiscard();

        /**
         * 设置消息在队列中的超时时间, t为毫秒
         * (超时时间精度只能是s)
         * @param t
         */
        void setQueueTimeout(int t);

        /**
         * 获取消息在队列中的超时时间, 毫秒
         * @return int
         */
        int getQueueTimeout() const;

        /**
         * 设置endpoint
         * @param str
         */
        void setEndpoint(const string &str);

        /**
         * 获取ip
         * @return const string&
         */
        TC_Endpoint getEndpoint() const;

        /**
         * 监听socket
         * @return TC_Socket
         */
        TC_Socket &getSocket();

        /**
         * 设置最大连接数
         * @param iMaxConns
         */
        void setMaxConns(int iMaxConns);

        /**
         * 获取最大连接数
         * @return size_t
         */
        size_t getMaxConns() const;

        /**
         * 设置HeartBeat时间
         * @param n
         */
        void setHeartBeatTime(time_t t);

        /**
         * 获取HeartBeat时间
         * @return size_t
         */
        time_t getHeartBeatTime() const;

        /**
         * 设置allow deny次序
         * @param eOrder
         */
        void setOrder(EOrder eOrder);

        /**
         * 设置允许ip
         * @param vtAllow
         */
        void setAllow(const vector<string> &vtAllow);

        /**
         * 设置禁止ip
         * @param vtDeny
         */
        void setDeny(const vector<string> &vtDeny);

        /**
         * 获取允许ip
         * @return vector<string>: ip列表
         */
        vector<string> getAllow() const;

         /**
         * 获取禁止ip
         * @return vector<string>: ip列表
         */
        vector<string> getDeny() const;

         /**
         * 获取allow deny次序
         * @return EOrder
         */
        EOrder getOrder() const;

        /**
         * 是否Ip被允许
         * @param ip
         * @return bool
         */
        bool isIpAllow(const string& ip) const;

        /**
         * 是否超过了最大连接数
         * @return bool
         */
        bool isLimitMaxConnection() const;

        /**
         * 减少当前连接数
         */
        void decreaseNowConnection();

        /**
         * 增加当前连接数
         */
        void increaseNowConnection();

        /**
         * 获取所有链接状态
         * @return ConnStatus
         */
        vector<ConnStatus> getConnStatus();

        /**
         * 获取当前连接数
         * @return int
         */
        int getNowConnection() const;

        /**
         * 获取EpollServer
         * @return TC_EpollServer*
         */
        TC_EpollServer* getEpollServer();

		/**
		 * 获取对应的网络线程
		 * @param fd
		 * @return
		 */
		inline NetThread* getNetThreadOfFd(int fd) const { return _pEpollServer->getNetThreadOfFd(fd); }

        /**
         * 注册协议解析器
         * @param pp
         */
        // void setProtocol(const protocol_functor& pf, int iHeaderLen = 0, const header_filter_functor& hf = echo_header_filter);
		/**
		 * 注册协议解析器
		 * @param pp
		 */
		void setProtocol(const TC_NetWorkBuffer::protocol_functor& pf, int iHeaderLen = 0, const header_filter_functor& hf = echo_header_filter);

        /**
         * 获取协议解析器
         * @return protocol_functor&
         */
        TC_NetWorkBuffer::protocol_functor &getProtocol();

        /**
         * 解析包头处理对象
         * @return protocol_functor&
         */
        header_filter_functor &getHeaderFilterFunctor();

		/**
		 * 增加数据到队列中
		 * @param vtRecvData
		 * @param bPushBack 后端插入
		 * @param sBuffer
		 */
		void insertRecvQueue(const shared_ptr<RecvContext> &recv);//, bool bPushBack = true);

		/**
		 * 等待数据
		 * @return bool
		 */
		bool waitForRecvQueue(uint32_t handleIndex, shared_ptr<RecvContext> &recv);

        /**
         * 接收队列的大小
         * @return size_t
         */
        size_t getRecvBufferSize() const;

		/**
		 * 默认的协议解析类, 直接echo
		 * @param r
		 * @param o
		 * @return int
		 */
		static TC_NetWorkBuffer::PACKET_TYPE echo_protocol(TC_NetWorkBuffer &r, vector<char> &o);

		/**
		 * 默认的包头处理
		 * @param i
		 * @param o
		 * @return int
		 */
		static TC_NetWorkBuffer::PACKET_TYPE echo_header_filter(TC_NetWorkBuffer::PACKET_TYPE i, vector<char> &o);

        /**
         * 获取需要过滤的包头长度
         */
        int getHeaderFilterLen();

        /**
         * 设置ServantHandle数目
         * @param n
         */
        void setHandleNum(int n);

        /**
         * 所属handle组的handle数(每个handle一个对象)
         * @return int
         */
        int getHandleNum();

		/**
		 * 初始化处理线程,线程将会启动
		 */
		template<typename T> void setHandle(size_t n)
		{
			if(!_handles.empty())
			{
                getEpollServer()->error("[BindAdapter::setHandle] handle is not empty!");
				return;
			}

			_iHandleNum = n;

			_threadDataQueue.resize(_iHandleNum + 1);
			_threadDataQueue[0] = std::make_shared<BindAdapter::DataQueue>();

			if(_pEpollServer->isMergeHandleNetThread())
			{
				_iHandleNum = _pEpollServer->_netThreadNum;
			}

			for (int32_t i = 0; i < _iHandleNum ; ++i)
			{
				HandlePtr handle = new T();

				handle->setHandleIndex(i);

				handle->setEpollServer(this->getEpollServer());

				handle->setBindAdapter(this);

				_handles.push_back(handle);
			}
		}

		/**
		 * 获取第几个句柄
		 * @param index
		 * @return
		 */
		HandlePtr getHandle(size_t index) {
			assert(index <= _iHandleNum);
			assert(getEpollServer()->isMergeHandleNetThread());
			return _handles[index];
		}

        /**
         * 设置服务端回包缓存的大小限制
         */
        void setBackPacketBuffLimit(size_t iLimitSize);

        /**
         * 获取服务端回包缓存的大小限制
         */
        size_t getBackPacketBuffLimit();


		/**
		 * 获取服务端接收队列(如果_rnbuffer有多个, 则根据调用者的线程id来hash获取)
		 */
		recv_queue &getRecvQueue(uint32_t handleIndex);

		/**
		 * 获取handles
		 */
		const vector<HandlePtr> &getHandles() { return _handles; }

		/**
		 * 是否是队列模式(默认是False的)
		 */
		bool isQueueMode() const { return _queueMode; }

		/**
		 * 开启队列模式(同一个连接的请求, 落在同一个handle处理线程中)
		 */
		void enableQueueMode() { _queueMode = true; }

        /**
         * 等待队列数据
         */
        void waitAtQueue(uint32_t handleIndex, uint32_t waitTime);

        /**
         * 通知某个具体handle醒过来
         * @param handleIndex
         */
        void notifyHandle(uint32_t handleIndex);

        /**  
         * 设置close回调函数 
         */
        void setOnClose(const close_functor& f) { _closeFunc = f; } 

        /**
         * 注册鉴权包裹函数
         * @param apwf
         */
        void setAuthProcessWrapper(const auth_process_wrapper_functor& apwf) { _authWrapper = apwf; }

        void setAkSk(const std::string& ak, const std::string& sk) { _accessKey = ak; _secretKey = sk; }

        bool checkAkSk(const std::string& ak, const std::string& sk) { return ak == _accessKey && sk == _secretKey; }

        std::string getSk(const std::string& ak) const { return (_accessKey == ak) ? _secretKey : ""; }

#if TARS_SSL
        void setSSLCtx(const shared_ptr<TC_OpenSSL::CTX> &ctx) { _ctx = ctx; }
#endif

	private:
		/**
		 * 获取等待的队列锁
		 * @return
		 */
		TC_ThreadLock &getLock(uint32_t handleIndex);

    public:

        //统计上报的对象
        PropertyReport * _pReportQueue = NULL;
        PropertyReport * _pReportConRate = NULL;
        PropertyReport * _pReportTimeoutNum = NULL;

    protected:
        friend class TC_EpollServer;

        /**
         * 服务
         */
        TC_EpollServer  *_pEpollServer;

		/**
		 * 加锁
		 */
		mutable std::mutex		_mutex;

		/**
		 * Adapter所用的HandleGroup
		 */
		vector<HandlePtr> _handles;

        /**
         * 协议解析
         */
        TC_NetWorkBuffer::protocol_functor _pf;

        /**
         * 首个数据包包头过滤
         */
        header_filter_functor _hf;

        /**
         * adapter的名字
         */
        string          _name;
        /**
         * 监听fd
         */
        TC_Socket       _s;

        /**
         * 绑定的IP
         */
        TC_Endpoint     _ep;

        /**
         * 最大连接数
         */
        int             _iMaxConns;

        /**
         * 当前连接数
         */
        std::atomic<int> _iCurConns;

        /**
         * Handle个数
         */
        size_t          _iHandleNum;

        /**
         * 允许的Order
         */
        volatile EOrder _eOrder;

        /**
         * 允许的ip
         */
        vector<string>  _vtAllow;

        /**
         * 禁止的ip
         */
        vector<string>  _vtDeny;

		/**
		 * 每个线程都有自己的队列
		 * 0: 给共享队列模式时使用
		 * 1~handle个数: 队列模式时使用
		 */
		vector<shared_ptr<DataQueue>> _threadDataQueue;

		/**
		 * 接收队列数据总个数
		 */
		atomic<size_t>  _iBufferSize{0};

        /**
         * 队列最大容量
         */
        int             _iQueueCapacity;

        /**
         * 消息超时时间（从入队列到出队列间隔)(毫秒）
         */
        int             _iQueueTimeout;

        /**
         * 首个数据包包头长度
         */
        int             _iHeaderLen;

        /**
         * 上次心跳发送时间
         */
        volatile time_t          _iHeartBeatTime;

        /**
         * 协议名称,缺省为"tars"
         */
        string                  _protocolName;

        //回包缓存限制大小
        size_t                    _iBackPacketBuffLimit;

		//队列模式
		bool _queueMode 		= false;

		//listen模式
		bool _manualListen		= false;

        /**
         * 包裹认证函数,不能为空
         */
        auth_process_wrapper_functor _authWrapper;

        /**
         * 该obj的AK SK
         */
        std::string              _accessKey;
        std::string              _secretKey;

#if TARS_SSL

        /**
         * ssl ctx
         */
	    shared_ptr<TC_OpenSSL::CTX> _ctx;
#endif
        //连接关闭的回调函数 
        close_functor           _closeFunc;

    };

    ////////////////////////////////////////////////////////////////////////////
    // 服务连接管理
    /**
     *  建立连接的socket信息
     */
    class Connection
    {
    public:
        enum EnumConnectionType
        {
            EM_TCP = 0,
            EM_UDP = 1,
        };

        /**
         * 构造函数
         * @param lfd
         * @param s
         * @param ip
         * @param port
         */
        Connection(BindAdapter *pBindAdapter, int lfd, int timeout, int fd, const string& ip, uint16_t port);

        /**
         * udp连接
         * @param fd
         */
        Connection(BindAdapter *pBindAdapter, int fd);

        /**
         * 析构函数
         */
        virtual ~Connection();

        /**
         * 链接所属的adapter
         */
		BindAdapterPtr& getBindAdapter()       { return _pBindAdapter; }

        /**
         * 初始化
         * @param id, 连接的唯一id
         */
        void init(unsigned int uid)         { _uid = uid; }

        /**
         * 获取连接超时时间
         *
         * @return int
         */
        int getTimeout() const              { return _timeout; }

        /**
         * 获取线程的惟一id
         *
         * @return unsigned int
         */
        uint32_t getId() const              { return _uid; }

        /**
         * 获取监听fd
         *
         * @return int
         */
        int getListenfd() const             { return _lfd; }

        /**
         * 当前连接fd
         *
         * @return int
         */
        int getfd() const                   { return _sock.getfd(); }

        /**
         * 是否有效
         *
         * @return bool
         */
        bool isValid() const                { return _sock.isValid();}

        /**
         * 远程IP
         *
         * @return string
         */
        string getIp() const                { return _ip; }

        /**
         * 远程端口
         *
         * @return uint16_t
         */
        uint16_t getPort() const            { return _port; }

        /**
         * 设置首个数据包包头需要过滤的字节数
         */
        void setHeaderFilterLen(int iHeaderLen)     { _iHeaderLen = iHeaderLen; }

        /**
         * 设置关闭,发送完当前数据就关闭连接
         */
        bool setClose();

        /**
         * 获取连接类型
         */
        EnumConnectionType getType() const          { return _enType; }

	    /**
	 	* 是否是空连接
	 	*/
        bool isEmptyConn() const  {return _bEmptyConn;}

        /**
         * Init Auth State;
         */
        void tryInitAuthState(int initState);

	    friend class NetThread;

    protected:

		/**
		 * 关闭连接
		 * @param fd
		 */
		void close();

		/**
		* 发送TCP
		*/
		int sendTcp(const shared_ptr<SendContext> &data);

		/**
		* 发送Udp
		*/
		int sendUdp(const shared_ptr<SendContext> &data);

		/**
		 * 添加发送buffer
		 * @param buffer
		 * @return int, -1:发送出错, 0:无数据, 1:发送完毕, 2:还有数据
		 */
		int send(const shared_ptr<SendContext> &data);

		/**
		 * 发送buffer里面数据
		 * @return
		 */
		int sendBuffer();

		/**
		 * 读取数据
		 * @param fd
		 * @return int, -1:接收出错, 0:接收不全, 1:接收到一个完整包
		 */
		int recv();

		/**
		* 接收TCP
		*/
		int recvTcp();

		/**
		* 接收Udp
		*/
		int recvUdp();
        /**
         * 解析协议
         * @param o
         * @return int: <0:协议错误, 0:没有一个完整的包, 1:收到至少一个包
         */
        int parseProtocol(TC_NetWorkBuffer &rbuf);

        /**
         * 增加数据到队列中
         * @param vtRecvData
         */
		void insertRecvQueue(const shared_ptr<RecvContext> &recv);

        /**
         * 对于udp方式的连接，分配指定大小的接收缓冲区
         *@param nSize
            */
        bool setRecvBuffer(size_t nSize=DEFAULT_RECV_BUFFERSIZE);

		/**
		 * 是否是tcp连接
		 * @return
		 */
		bool isTcp() const { return _lfd != -1; }

    public:
        /**
         * 最后刷新时间
         */
        time_t              _iLastRefreshTime;

    protected:

        /**
         * 适配器
         */
		BindAdapterPtr      _pBindAdapter;

        /**
         * TC_Socket
         */
        TC_Socket           _sock;

        /**
         * 连接的唯一编号
         */
        volatile uint32_t   _uid;

        /**
         * 监听的socket
         */
        int                 _lfd;

        /**
         * 超时时间
         */
        int                 _timeout;

        /**
         * ip
         */
        string              _ip;

        /**
         * 端口
         */
        uint16_t             _port;

        /**
         * 接收数据buffer
         */
        TC_NetWorkBuffer    _recvBuffer;

        /**
         * 发送数据buffer
         */
        TC_NetWorkBuffer    _sendBuffer;

        /**
         * 需要过滤的头部字节数
         */
        int                 _iHeaderLen;

        /**
         * 发送完当前数据就关闭连接
         */
        bool                _bClose;

        /**
         * 连接类型
         */
        EnumConnectionType  _enType;

        bool                _bEmptyConn;

        /*
        *接收数据的临时buffer,加这个目的是对udp接收数据包大小进行设置
        */
        char                *_pRecvBuffer = NULL;

        size_t              _nRecvBufferSize;

    public:
        /*
        *该连接的鉴权状态
        */
        int                 _authState;
        /*
        *该连接的鉴权状态是否初始化了
        */
        bool                _authInit;
#if TARS_SSL
        std::shared_ptr<TC_OpenSSL> _openssl;
#endif
    };
    ////////////////////////////////////////////////////////////////////////////
    /**
     * 带有时间链表的map
     */
    class ConnectionList : public TC_ThreadLock
    {
    public:
        /**
         * 构造函数
         */
        ConnectionList(NetThread *pEpollServer);

        /**
         * 析够函数
         */
        ~ConnectionList() { if(_vConn) { delete[] _vConn; } }

        /**
         * 初始化大小
         * @param size
         */
        void init(uint32_t size, uint32_t iIndex = 0);

        /**
         * 获取惟一ID
         *
         * @return unsigned int
         */
        uint32_t getUniqId();

        /**
         * 添加连接
         * @param cPtr
         * @param iTimeOutStamp
         */
        void add(Connection *cPtr, time_t iTimeOutStamp);

        /**
         * 刷新时间链
         * @param uid
         * @param iTimeOutStamp, 超时时间点
         */
        void refresh(uint32_t uid, time_t iTimeOutStamp);

        /**
         * 检查超时数据
         */
        void checkTimeout(time_t iCurTime);

        /**
         * 获取某个监听端口的连接
         * @param lfd
         * @return vector<TC_EpollServer::ConnStatus>
         */
        vector<ConnStatus> getConnStatus(int lfd);

        /**
         * 获取某一个连接
         * @param p
         * @return T
         */
        Connection* get(uint32_t uid);

        /**
         * 删除连接
         * @param uid
         */
        void del(uint32_t uid);

        /**
         * 大小
         * @return size_t
         */
        size_t size();

    protected:
        typedef pair<Connection*, multimap<time_t, uint32_t>::iterator> list_data;

        /**
         * 内部删除, 不加锁
         * @param uid
         */
        void _del(uint32_t uid);

    protected:
		/**
		 * 无锁
		 */
		TC_SpinLock                     _mutex;

        /**
         * 服务
         */
        NetThread                        *_pEpollServer;

        /**
         * 总计连接数
         */
        volatile uint32_t               _total;

        /**
         * 空闲链表
         */
        list<uint32_t>                  _free;

        /**
         * 空闲链元素个数
         */
        volatile size_t                 _free_size;

        /**
         * 链接
         */
        list_data                       *_vConn;

        /**
         * 超时链表
         */
        multimap<time_t, uint32_t>      _tl;

        /**
         * 上次检查超时时间
         */
        time_t                          _lastTimeoutTime;

        /**
         * 链接ID的魔数
         */
        uint32_t                        _iConnectionMagic;
    };
    ////////////////////////////////////////////////////////////////////////////
    class NetThread : public TC_Thread, public TC_HandleBase
    {
    public:
        
        ////////////////////////////////////////////////////////////////////////////
    public:
        /**
         * 构造函数
         */
        NetThread(TC_EpollServer *epollServer, int index);

        /**
         * 析构函数
         */
        virtual ~NetThread();

		/**
		 * 获取网络线程的index
		* @return
		*/
		int getIndex() const { return _threadIndex; }

        /**
         * 绑定监听socket
         * @param ls
         */
        // int bind(BindAdapterPtr &lsPtr);

        /**
         * 网络线程执行函数
         */
        virtual void run();

        /**
         * 停止网络线程
         */
        void terminate();

        /**
         * 生成epoll
         */
        void createEpoll(uint32_t iIndex = 0);

        /**
         * 初始化udp监听
         */
        void initUdp(const unordered_map<int, BindAdapterPtr> &listeners);

        /**
         * 是否服务结束了
         *
         * @return bool
         */
        bool isTerminate() const    { return _bTerminate; }

        /**
         * 获取Epoller对象
         * @return TC_Epoller*
         */
        TC_Epoller* getEpoller()    { return &_epoller; }

		/**
		 * 唤醒网络线程
		 */
		void notify();

		/**
		 * 关闭连接
		 * @param uid
		 */
		void close(const shared_ptr<RecvContext> &data);

		/**
		* 发送数据
		* @param uid
		* @param s
		*/
		void send(const shared_ptr<SendContext> &data);

		/**
		 * 获取某一监听端口的连接数
		 * @param lfd
		 *
		 * @return vector<TC_EpollServer::ConnStatus>
		 */
		vector<TC_EpollServer::ConnStatus> getConnStatus(int lfd);

        /**
         * 获取连接数
         *
         * @return size_t
         */
        size_t getConnectionCount()     { return _list.size(); }

        /**
         * 记录日志
         * @param s
         */
        void debug(const string &s);

        /**
         * INFO日志
         * @param s
         */
        void info(const string &s);

	    /**
		 * TARS日志
		 * @param s
		 */
	    void tars(const string &s);

        /**
         * 记录错误日志
         * @param s
         */
        void error(const string &s);

		/**
		 * tars日志
		 * @param s
		 */
		void tars(const string &s) const;

        /**
         * 是否启用防止空链接攻击的机制
         * @param bEnable
         */
        void enAntiEmptyConnAttack(bool bEnable);

        /**
         *设置空连接超时时间
         */
        void setEmptyConnTimeout(int timeout);

        /**
         *设置udp的接收缓存区大小，单位是B,最小值为8192，最大值为DEFAULT_RECV_BUFFERSIZE
         */
        void setUdpRecvBufferSize(size_t nSize=DEFAULT_RECV_BUFFERSIZE);

        /**
         * 发送队列的大小
         * @return size_t
         */
        size_t getSendRspSize();

    protected:

        /**
         * 获取连接
         * @param id
         *
         * @return ConnectionPtr
         */
        Connection *getConnectionPtr(uint32_t uid)      { return _list.get(uid); }

        /**
         * 添加tcp链接
         * @param cPtr
         * @param iIndex
         */
        void addTcpConnection(Connection *cPtr);

        /**
         * 添加udp连接
         * @param cPtr
         * @param index
         */
        void addUdpConnection(Connection *cPtr);

        /**
         * 删除链接
         * @param cPtr
         * @param bEraseList 是否是超时连接的删除
         * @param closeType  关闭类型,0:表示客户端主动关闭；1:服务端主动关闭;2:连接超时服务端主动关闭
         */
        void delConnection(Connection *cPtr, bool bEraseList = true,EM_CLOSE_T closeType=EM_CLIENT_CLOSE);

        /**
         * 处理管道消息
         */
        void processPipe();

        /**
         * 处理网络请求
         */
        void processNet(const epoll_event &ev);

        /**
         * 空连接超时时间
         */
        int getEmptyConnTimeout() const;

        /**
         *是否空连接检测
         */
        bool isEmptyConnCheck() const;

        friend class BindAdapter;
        friend class ConnectionList;
        friend class TC_EpollServer;

    private:
        /**
         * 服务
         */
        TC_EpollServer            *_epollServer;

		/**
		 * net线程的id
		 */
		std::thread::id             _threadId;

		/**
		 * 线程索引
		 */
		int                         _threadIndex;

        /**
         * epoll
         */
        TC_Epoller                  _epoller;

        /**
         * 停止
         */
        bool                        _bTerminate;

        /**
         * handle是否已经启动
         */
        bool                        _handleStarted;

		/**
		 * 通知epoll
		 */ 
		TC_Epoller::NotifyInfo 		_notify;

        /**
         * 管理的连接链表
         */
        ConnectionList              _list;

        /**
         * 发送队列
         */
        send_queue                  _sbuffer;

        /**
         *空连接检测机制开关
         */
        bool                         _bEmptyConnAttackCheck;

        /**
         * 空连接超时时间,单位是毫秒,默认值2s,
         * 该时间必须小于等于adapter自身的超时时间
         */
        int                            _iEmptyCheckTimeout;

        /**
         * udp连接时接收包缓存大小,针对所有udp接收缓存有效
         */
        size_t                         _nUdpRecvBufferSize;

		/**
		 * 通知信号
		 */
		bool                        _notifySignal = false;
    };
    ////////////////////////////////////////////////////////////////////////////
public:
    /**
     * 构造函数
     */
    TC_EpollServer(unsigned int iNetThreadNum = 1);

    /**
     * 析构函数
     */
    ~TC_EpollServer();

    /**
     * 是否启用防止空链接攻击的机制
     * @param bEnable
     */
    void enAntiEmptyConnAttack(bool bEnable);

    /**
     *设置空连接超时时间
     */
    void setEmptyConnTimeout(int timeout);

    /**
     *设置NetThread的内存池信息
     */
    // void setNetThreadBufferPoolInfo(size_t minBlock, size_t maxBlock, size_t maxBytes);

    /**
     * 设置本地日志
     * @param plocalLogger
     */
    void setLocalLogger(RollWrapperInterface *pLocalLogger)       { _pLocalLogger = pLocalLogger; }

	/**
	 * 选择网络线程
	 * @param fd
	 */
	inline NetThread* getNetThreadOfFd(int fd) { return _netThreads[fd % _netThreads.size()]; }

	/**
	 * 合并handle线程和网络线程
	 * @param merge
	 */
	void setMergeHandleNetThread(bool merge) { _mergeHandleNetThread = merge; }

	/**
	 * 是否合并handle线程网络线程
	 * @return
	 */
	inline bool isMergeHandleNetThread() const { return _mergeHandleNetThread; }


    // /**
    //  * 绑定到一个已经有的handle组上
    //  * @param groupName
    //  * @param handleNum
    //  * @param adapter
    //  */
    // void setHandleGroup(const string& groupName, BindAdapterPtr adapter)
    // {
    //     map<string, HandleGroupPtr>::iterator it = _handleGroups.find(groupName);

    //     if (it != _handleGroups.end())
    //     {
    //         it->second->adapters[adapter->getName()] = adapter;
    //         adapter->_handleGroup = it->second;
    //     }
    // }

    // /**
    //  * 创建一个handle对象组，如果已经存在则直接返回
    //  * @param name
    //  * @return HandlePtr
    //  */
    // template<class T> void setHandleGroup(const string& groupName, int32_t handleNum, BindAdapterPtr adapter)
    // {
    //     map<string, HandleGroupPtr>::iterator it = _handleGroups.find(groupName);

    //     if (it == _handleGroups.end())
    //     {
    //         HandleGroupPtr hg = new HandleGroup();

    //         hg->name = groupName;

    //         adapter->_handleGroup = hg;

    //         for (int32_t i = 0; i < handleNum; ++i)
    //         {
    //             HandlePtr handle = new T();

    //             handle->setEpollServer(this);

    //             handle->setHandleGroup(hg);

    //             hg->handles.push_back(handle);
    //         }

    //         _handleGroups[groupName] = hg;

    //         it = _handleGroups.find(groupName);
    //     }
    //     it->second->adapters[adapter->getName()] = adapter;

    //     adapter->_handleGroup = it->second;
    // }

    // /**
    //  * 选择网络线程
    //  * @param fd
    //  */
    // NetThread* getNetThreadOfFd(int fd)
    // {
    //     return _netThreads[fd % _netThreads.size()];
    // }

    /**
     * 绑定监听socket
     * @param ls
     */
    int  bind(BindAdapterPtr &lsPtr);

    /**
     * 启动业务处理线程
     */
    void startHandle();

    /**
     * 生成epoll
     */
    void createEpoll();

    /**
     * 运行
     */
    void waitForShutdown();

    /**
     * 停止服务
     */
    void terminate();

    /**
     * 是否服务结束了
     *
     * @return bool
     */
    bool isTerminate() const    { return _bTerminate; }

    /**
     * 根据名称获取BindAdapter
     * @param sName
     * @return BindAdapterPtr
     */
    BindAdapterPtr getBindAdapter(const string &sName);

	/**
	 * 获取所有adatapters
	 * @return
	 */
	vector<BindAdapterPtr> getBindAdapters();

    /**
     * 向网络线程添加连接
     */
    void addConnection(Connection * cPtr, int fd, CONN_TYPE iType);

	/**
	 * 关闭连接
	 * @param uid
	 */
	void close(const shared_ptr<TC_EpollServer::RecvContext> &data);

	/**
	 * 发送数据
	 * @param uid
	 * @param s
	 */
	void send(const shared_ptr<SendContext> &data);

    /**
     * 获取某一监听端口的连接数
     * @param lfd
     *
     * @return vector<TC_EpollServer::ConnStatus>
     */
    vector<ConnStatus> getConnStatus(int lfd);

	/**
	 * 获取监听socket信息
	 *
	 * @return map<int,ListenSocket>
	 */
	unordered_map<int, BindAdapterPtr> getListenSocketInfo();

    /**
     * 获取所有连接的数目
     *
     * @return size_t
     */
    size_t getConnectionCount();

    /**
     * 记录日志
     * @param s
     */
    void debug(const string &s);

    /**
     * INFO日志
     * @param s
     */
    void info(const string &s);

     /**
     * 记录错误日志
     * @param s
     */
    void error(const string &s);

	/**
	 * tars日志
	 * @param s
	 */
	void tars(const string &s);

    /**
     * 获取网络线程的数目
     */
    unsigned int getNetThreadNum() { return _netThreadNum; }

    /**
     * 获取网络线程的指针集合
     */
    vector<TC_EpollServer::NetThread*> getNetThread() { return _netThreads; }

    /**
     * 停止线程
     */
    void stopThread();

    /**
     * 获取所有业务线程的数目
     */
    size_t getLogicThreadNum();

	//回调给应用服务
	typedef std::function<void(TC_EpollServer*)> application_callback_functor;

	/**
	 * 设置waitForShutdown线程回调的心跳
	 * @param hf [description]
	 */
	void setCallbackFunctor(const application_callback_functor &hf) { _hf = hf; }

    //网络线程发送心跳的函数
    typedef std::function<void(const string &)> heartbeat_callback_functor;

    /**
     * 设置netthread网络线程发送心跳的函数
     * @param hf [description]
     */
    void setHeartBeatFunctor(const heartbeat_callback_functor& heartFunc) { _heartFunc = heartFunc; }
    heartbeat_callback_functor& getHeartBeatFunctor() { return _heartFunc; }

protected:

    friend class BindAdapter;

	/**
	 * 接收句柄
	 * @param fd
	 * @return
	 */
	bool accept(int fd, int domain = AF_INET);

	/**
	 * 绑定端口
	 * @param ep
	 * @param s
	 * @param manualListen
	 */
	void bind(const TC_Endpoint &ep, TC_Socket &s, bool manualListen);

	static void applicationCallback(TC_EpollServer *epollServer);
// public:

    //统计服务端相应队列大小的上报的对象
    // PropertyReport *            _pReportRspQueue;

private:
    /**
     * 网络线程
     */
    std::vector<NetThread*>        _netThreads;

    /*
     * 网络线程数目
     */
    unsigned int                _netThreadNum;

	/**
 	* epoll
 	*/
	TC_Epoller                  _epoller;

	/**
	 * 通知epoll
	 */
	TC_Epoller::NotifyInfo 		_notify;

    /*
     * 服务是否停止
     */
    bool                        _bTerminate;

    /*
     * 业务线程是否启动
     */
    bool                        _handleStarted;

	/**
	 * 合并网络和业务线程
	 */
	bool                        _mergeHandleNetThread = false;
    /**
     * 本地循环日志
     */
    RollWrapperInterface        *_pLocalLogger;


	/**
	 *
	 */
	vector<BindAdapterPtr>       _bindAdapters;

	/**
 	* 监听socket
 	*/
	unordered_map<int, BindAdapterPtr>    _listeners;

	/**
	 * 应用回调
	 */
	application_callback_functor _hf;

    /**
     * 发送心跳的函数
     */
    heartbeat_callback_functor _heartFunc;

    /**
     * 处理handle对象
     */
    // map<string, HandleGroupPtr> _handleGroups;
};
typedef TC_AutoPtr<TC_EpollServer> TC_EpollServerPtr;
}

#endif
