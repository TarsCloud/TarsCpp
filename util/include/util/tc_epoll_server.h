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
#include <unordered_map>
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
#include "util/tc_openssl.h"

using namespace std;

namespace tars
{

/////////////////////////////////////////////////
/**
 * @file  tc_epoll_server.h
 * @brief  EpollServer类
 * @brief  EpollServer Class
 *
 */
/////////////////////////////////////////////////
/**
 * Server基类
 * Server Basic Class
 * 注册协议解析器
 * Register Protocol Resolver
 * 注册逻辑处理器
 * Register Logical Processor
 * 注册管理端口处理器
 * Register Management Port Processor
 */

class PropertyReport;

class TC_EpollServer : public TC_HandleBase
{
public:

    enum EM_CLOSE_T
    {
        /**Client active shutdown*/
        EM_CLIENT_CLOSE = 0,         //客户端主动关闭
        /**The service-side business proactively calls 'close' to close the connection, 
         * or the framework proactively closes the connection due to an exception.*/
        EM_SERVER_CLOSE = 1,        //服务端业务主动调用close关闭连接,或者框架因某种异常主动关闭连接
        /**Connection timed out, server actively closed*/
        EM_SERVER_TIMEOUT_CLOSE = 2  //连接超时了，服务端主动关闭
    };

    enum
    {
        /**Empty connection timeout (ms)*/
        MIN_EMPTY_CONN_TIMEOUT  = 2*1000,    /*空链接超时时间(ms)*/
        /**The size of received buffer of the default data*/
        DEFAULT_RECV_BUFFERSIZE = 64*1024    /*缺省数据接收buffer的大小*/
    };

    //定义加入到网络线程的fd类别
    //Define the FD categories added to network threads
    enum CONN_TYPE
    {
        TCP_CONNECTION = 0,
        UDP_CONNECTION = 1,
    };

    /**
     * 定义协议解析接口的操作对象
     * 注意必须是线程安全的或是可以重入的
     * Define the Operating Object of the Protocol Resolution Interface
     * Note that it must be thread safe or reentrant
     */
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
     * Context of sending packets
     * Created by RecvContext
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
        /**Send package is valid. Command: 'c', close FD; 's', data need to be sent*/
		char                        _cmd;            /**send包才有效, 命令:'c',关闭fd; 's',有数据需要发送*/
        /**Sent context*/
		shared_ptr<TC_NetWorkBuffer::Buffer> _sbuffer;        /**发送的内容*/
	};

	////////////////////////////////////////////////////////////////////////////
	/**
	 * 接收包的上下文
     * Context of receiving package
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

	protected:
        /**Connection Label*/
		uint32_t        _uid;            /**连接标示*/
		/**IP for remote connection*/
        string          _ip;             /**远程连接的ip*/
		/**Port for remote connection*/
        uint16_t        _port;           /**远程连接的端口*/
		/**Save the FD that generated the message to select the network thread when returning the package*/
        int				_fd;				/*保存产生该消息的fd，用于回包时选择网络线程*/
		/**Message identifying which adapter*/
        BindAdapterPtr  _adapter;        /**标识哪一个adapter的消息*/
		/**Received message*/
        vector<char>    _rbuffer;        /**接收的内容*/
		/**Is overloaded*/
        bool            _isOverload = false;     /**是否已过载 */
		/**Is closed*/
        bool            _isClosed   = false;       /**是否已关闭*/
		/**If the message package is closed, the type of closure is identified, 
         * 0: the client actively closes, 
         * 1: the server actively closes, 
         * 2: the connection timeout server actively closes.
         * */
        int             _closeType;     /*如果是关闭消息包，则标识关闭类型,0:表示客户端主动关闭；1:服务端主动关闭;2:连接超时服务端主动关闭*/
		/**Time to receive data*/
        int64_t         _recvTimeStamp;  /**接收到数据的时间*/
	};

//	typedef TC_CasQueue<shared_ptr<RecvContext>> recv_queue;
	typedef TC_ThreadQueue<shared_ptr<RecvContext>> recv_queue;
//	typedef TC_CasQueue<shared_ptr<SendContext>> send_queue;
	typedef TC_ThreadQueue<shared_ptr<SendContext>> send_queue;
	typedef recv_queue::queue_type recv_queue_type;

	////////////////////////////////////////////////////////////////////////////
	/**
	 * 链接状态
     * Connection state
	 */
	struct ConnStatus
	{
		string          ip;
		int32_t         uid;
		uint16_t        port;
		int             timeout;
		int             iLastRefreshTime;
		size_t          recvBufferSize;
		size_t          sendBufferSize;
	};

    ////////////////////////////////////////////////////////////////////////////
    /**
     * @brief 定义服务逻辑处理的接口
     * @brief Define interfaces for logical processing of services
     *
     */
    /**
     * 服务的逻辑处理代码
     * Logical Processing Code for Services
     */
    class Handle : public TC_Thread, public TC_HandleBase
    {
    public:
        /**
         * 构造, 默认没有请求, 等待10s
         * Constructor, default no request, wait 10s
         */
        Handle();

        /**
         * 析构函数
         * Destructor
         */
        virtual ~Handle();

        /**
         * 获取服务
         * Get Service
         * @return TC_EpollServer*
         */
		TC_EpollServer* getEpollServer() const { return _pEpollServer; };

		/**
		 * 获取adapter
         * Get adapter
		 * @return
		 */
		BindAdapter *getBindAdapter() const { return _bindAdapter; }

		/**
		 * 获取Handle的索引(0~handle个数-1)
         * Get the index of Handle(0~handle count-1)
		 * @return
		 */
		uint32_t getHandleIndex() const { return _handleIndex; }

		/**
		 * 设置网络线程
         * Set up network thread
		 */
		void setNetThread(NetThread *netThread);

		/**
		 * 获取网络线程
         * Get network thread
		 * @return
		 */
		NetThread *getNetThread() { return _netThread; }

		/**
		 * 处理
         * Process
		 */
		void process(shared_ptr<RecvContext> data);

        /**
         * 线程处理方法
         * Thread processing method
         */
        virtual void run();

    public:
        /**
         * 发送数据
         * Send data
         * @param stRecvData
         * @param sSendBuffer
         */
		void sendResponse(const shared_ptr<SendContext> &data);

        /**
         * 关闭链接
         * Close connection
         * @param stRecvData
         */
        void close(const shared_ptr<RecvContext> &data);

        /**
         * 设置等待时间
         * Set up waiting time
         * @param iWaitTime
         */
        void setWaitTime(uint32_t iWaitTime);

        /**
         * 对象初始化
         * Object initialization
         */
        virtual void initialize() {};

        /**
         * 唤醒handle对应的处理线程
         * Wake up the process thread corresponding to the handle
         */
        virtual void notifyFilter();

        /**
         * 心跳(每处理完一个请求或者等待请求超时都会调用一次)
         * Heartbeat(Called every time a request has been processed or the waiting request has timed out)
         */
        virtual void heartbeat() {}

    protected:
        /**
         * 具体的处理逻辑
         * Specific processing logic
         */
        virtual void handleImp();

		/**
		 * 处理函数
         * Processing Function
		 * @param stRecvData: 接收到的数据
         * @param stRecvData: received data
		 */
		virtual void handle(const shared_ptr<RecvContext> &data) = 0;

		/**
		 * 处理超时数据, 即数据在队列中的时间已经超过
		 * 默认直接关闭连接
         * Processing timeout data, i.e. data has been queued longer than
         * Close connection directly by default
		 * @param stRecvData: 接收到的数据
         * @param stRecvData: received data
		 */
		virtual void handleTimeout(const shared_ptr<TC_EpollServer::RecvContext> &data);

		/**
		 * 处理连接关闭通知，包括
         * Handle connection shutdown notifications, including:
		 * 1.close by peer
		 * 2.recv/send fail
		 * 3.close by timeout or overload
		 * @param stRecvData:
		 */
		virtual void handleClose(const shared_ptr<TC_EpollServer::RecvContext> &data);

		/**
		 * 处理overload数据 即数据队列中长度已经超过允许值
		 * 默认直接关闭连接
         * Processing overload data means that the length of the data queue has exceeded the allowable value
         * Close connection directly by default
		 * @param stRecvData: 接收到的数据
         * @param stRecvData: received data 
		 */
		virtual void handleOverload(const shared_ptr<TC_EpollServer::RecvContext> &data);

        /**
         * 处理异步回调队列
         * Handle asynchronous callback queues

         */
        virtual void handleAsyncResponse() {}

           /**
         * handleFilter拆分的第二部分，处理用户自有数据
         * 非游戏逻辑可忽略bExpectIdle参数
         * The second part of handleFilter splitting, dealing with user-owned data
         * Non-game logic ignores bExpectIdle parameter
         */
        virtual void handleCustomMessage(bool bExpectIdle = false) {}

        /**
         * 线程已经启动, 进入具体处理前调用
         * Thread has been started and called before entering specific processing
         */
        virtual void startHandle() {}

        /**
         * 线程马上要退出时调用
         * Call when the thread is about to exit
         */
        virtual void stopHandle() {}

        /**
         * 是否所有的Adpater队列都为空
         * Whether all adapter queues are empty.
         * @return bool
         */
        virtual bool allAdapterIsEmpty();

		/**
		 * 是否所有的servant都没有resp消息待处理
         * Whether all servant don't have resp message to deal.
		 * @return bool
		 */
		virtual bool allFilterIsEmpty();

		/**
		 * 设置服务
         * Set up service
		 * @param pEpollServer
		 */
		void setEpollServer(TC_EpollServer *pEpollServer);

		/**
		 * 设置Adapter
         * Set up Apdater
		 * @param pEpollServer
		 */
		void setBindAdapter(BindAdapter*  bindAdapter);

		/**
		 * 设置index
         * Set up index
		 * @param index
		 */
		void setHandleIndex(uint32_t index);

		/**
		 * 等待在队列上
         * On the waiting queue
		 */
		void wait();

		/**
		 * 从队列中获取数据
         * Receive data from the queue
		 * @param recv
		 * @return
		 */
		bool popRecvQueue(shared_ptr<RecvContext> &recv);

        /**
         * 友元类
         * Friend Class
         */
        friend class BindAdapter;
    protected:
        /**
         * 服务
         * Service
         */
        TC_EpollServer  *_pEpollServer;

		/**
		 * handle对应的网路线程(网络线程和handle线程合并的情况下有效)
         * Network threads corresponding to handle (valid when network threads and handle threads are merged)
		 */
		NetThread       *_netThread = NULL;

		/**
		 * 所属handle组
         * The handle group to which one belongs
		 */
		BindAdapter*    _bindAdapter;

		/**
		 * 等待时间
         * Waiting time
		 */
		uint32_t        _iWaitTime;

		/**
		 * Handle的索引
         * Index of the Handle
		 */
		uint32_t        _handleIndex;

    };

    using close_functor = std::function<void (void*, EM_CLOSE_T )>;
    using auth_process_wrapper_functor = std::function<bool (Connection *c, const shared_ptr<RecvContext> &recv )>;

    ////////////////////////////////////////////////////////////////////////////
    // 服务端口管理,监听socket信息
    // Service port management, listening for socket information
    class BindAdapter : public TC_HandleBase
    {
    public:
        /**
         * 缺省的一些定义
         * Defualt definitions
         */
        enum
        {
            /**Flow*/
            DEFAULT_QUEUE_CAP       = 10*1024,    /**流量*/
            /**Queue minimum timeout (ms)*/
            MIN_QUEUE_TIMEOUT       = 3*1000,     /**队列最小超时时间(ms)*/
            /**Default maximum connections*/
            DEFAULT_MAX_CONN        = 1024,       /**缺省最大连接数*/
            /**Default queue timeout (ms)*/
            DEFAULT_QUEUE_TIMEOUT   = 60*1000,    /**缺省的队列超时时间(ms)*/
        };
        /**
         * 顺序
         * Order
         */
        enum EOrder
        {
            ALLOW_DENY,
            DENY_ALLOW
        };

		/**
		 * 数据队列
         * Data Queue
		 */
		struct DataQueue
		{
			/**
			 * 接收的数据队列
             * Received data queue
			 */
			recv_queue      _rbuffer;

			/**
			 * 锁
             * Lock
			 */
			TC_ThreadLock   _monitor;
		};

        /**
         * 构造函数
         * Constructor
         */
        BindAdapter(TC_EpollServer *pEpollServer);

        /**
         * 析够函数
         * Destructor
         */
        ~BindAdapter();

		/**
         * 设置需要手工监听
         * Set requires manual listening
         */
		void enableManualListen() { _manualListen = true; }

		/**
		 * 是否手工监听端口
         * Whether to manual listen the port or not
		 * @return
		 */
		bool isManualListen() const { return _manualListen; }

		/**
         * 手工绑定端口
         * Manual port binding
         */
		void manualListen();

        /**
         * 设置adapter name
         * Set up adapter name
         * @param name
         */
        void setName(const string &name);

        /**
         * 获取adapter name
         * Get adapter name
         * @return string
         */
        string getName() const;

		/**
		 * 增加处理线程对应的接收队列
         * Add the corresponding receiving queue for processing threads
		 * @return string
		 */		
		void initThreadRecvQueue(uint32_t handeIndex);

        /**
         * 获取queue capacity
         * Get queue capacity
         * @return int
         */
        int getQueueCapacity() const;

        /**
         * 设置queue capacity
         * Set up queue capacity
         * @param n
         */
        void setQueueCapacity(int n);

        /**
         * 设置协议名称
         * Set up the protocol name
         * @param name
         */
        void setProtocolName(const string& name);

        /**
         * 获取协议名称
         * Get the protocol name
         * @return const string&
         */
        const string& getProtocolName();

        /**
         * 是否tars协议
         * Whether it is the tars protocol
         * @return bool
         */
        bool isTarsProtocol();

        /**
         * 判断是否需要过载保护
         * Determine whether it needs overload protection
         * @return bool
         */
        int isOverloadorDiscard();

        /**
         * 设置消息在队列中的超时时间, t为毫秒
         * (超时时间精度只能是s)
         * Set the timeout time of the message in the queue, t is milliseconds
         * (timeout precision can only be s)
         * 
         * @param t
         */
        void setQueueTimeout(int t);

        /**
         * 获取消息在队列中的超时时间, 毫秒
         * Get timeout of message in queue, MS
         * @return int
         */
        int getQueueTimeout() const;

        /**
         * 设置endpoint
         * Set up endpoint
         * @param str
         */
        void setEndpoint(const string &str);

        /**
         * 获取ip
         * Get ip
         * @return const string&
         */
        TC_Endpoint getEndpoint() const;

        /**
         * 监听socket
         * Listen socket
         * @return TC_Socket
         */
        TC_Socket &getSocket();

        /**
         * 设置最大连接数
         * Set the maximum connection number
         * @param iMaxConns
         */
        void setMaxConns(int iMaxConns);

        /**
         * 获取最大连接数
         * Get the maximum connection number
         * @return size_t
         */
        size_t getMaxConns() const;

        /**
         * 设置HeartBeat时间
         * Set the HeartBeat time
         * @param n
         */
        void setHeartBeatTime(time_t t);

        /**
         * 获取HeartBeat时间
         * Get the HeartBeat time
         * @return size_t
         */
        time_t getHeartBeatTime() const;

        /**
         * 设置allow deny次序
         * Set the allow deny order
         * @param eOrder
         */
        void setOrder(EOrder eOrder);

        /**
         * 设置允许ip
         * Set allowed ip
         * @param vtAllow
         */
        void setAllow(const vector<string> &vtAllow);

        /**
         * 设置禁止ip
         * Set the disabled ip
         * @param vtDeny
         */
        void setDeny(const vector<string> &vtDeny);

        /**
         * 获取允许ip
         * Get the allowed ip
         * @return vector<string>: ip列表
         * @return vector<string>: ip list
         */
        const vector<string> &getAllow() const;

         /**
         * 获取禁止ip
         * Get the disable ip
         * @return vector<string>: ip列表
         * @return vector<string>: ip list
         */
        const vector<string> &getDeny() const;

         /**
         * 获取allow deny次序
         * Get the allow deny order
         * @return EOrder
         */
        EOrder getOrder() const;

        /**
         * 是否Ip被允许
         * Whether the ip is allowed or not
         * @param ip
         * @return bool
         */
        bool isIpAllow(const string& ip) const;

        /**
         * 是否超过了最大连接数
         * Whether it exceeds the maximum connection number
         * @return bool
         */
        bool isLimitMaxConnection() const;

        /**
         * 减少当前连接数
         * Reduce current connections
         */
        void decreaseNowConnection();

		/**
		 * 增加当前连接数
         * Increase current connections
		 */
		void increaseNowConnection();

		/**
		 * 获取所有链接状态
         * Get all connection states
		 * @return ConnStatus
		 */
		vector<ConnStatus> getConnStatus();

		/**
		 * 获取当前连接数
         * Get current connections
		 * @return int
		 */
		int getNowConnection() const;

		/**
		 * 获取服务
         * Get service
		 * @return TC_EpollServer*
		 */
		TC_EpollServer* getEpollServer() const { return _pEpollServer; };

		/**
		 * 获取对应的网络线程
         * Get the corresponding network thread
		 * @param fd
		 * @return
		 */
		inline NetThread* getNetThreadOfFd(int fd) const { return _pEpollServer->getNetThreadOfFd(fd); }

		/**
		 * 注册协议解析器
         * Registration Protocol parser
		 * @param pp
		 */
		void setProtocol(const TC_NetWorkBuffer::protocol_functor& pf, int iHeaderLen = 0, const header_filter_functor& hf = echo_header_filter);

		/**
		 * 获取协议解析器
         * Get Registration Protocol parser
		 * @return protocol_functor&
		 */
		TC_NetWorkBuffer::protocol_functor &getProtocol();

		/**
		 * 解析包头处理对象
         * Resolve Package Header Processing Objects
		 * @return protocol_functor&
		 */
		header_filter_functor &getHeaderFilterFunctor();

		/**
		 * 增加数据到队列中
         * Add data to the queue
		 * @param vtRecvData
		 * @param bPushBack 后端插入
         * @param bPushBack Backend insert
		 * @param sBuffer
		 */
		void insertRecvQueue(const shared_ptr<RecvContext> &recv);//, bool bPushBack = true);

		/**
		 * 等待数据
         * Wait for data
		 * @return bool
		 */
		bool waitForRecvQueue(uint32_t handleIndex, shared_ptr<RecvContext> &recv);

		/**
		 * 接收队列的大小
         * Size of the received queue
		 * @return size_t
		 */
		size_t getRecvBufferSize() const;

		/**
		 * 发送队列的大小
         * Size of the sent queue
		 * @return size_t
		 */
		size_t getSendBufferSize() const;

		/**
		 * add send buffer size
		 */
		inline void increaseSendBufferSize() { ++_iSendBufferSize; }

		/**
		 * increase send buffer size
		 */
		inline void decreaseSendBufferSize(size_t s = 1) { _iSendBufferSize.fetch_sub(s); }

		/**
		 * 默认的协议解析类, 直接echo
         * Default protocol resolution class, direct echo
		 * @param r
		 * @param o
		 * @return int
		 */
		static TC_NetWorkBuffer::PACKET_TYPE echo_protocol(TC_NetWorkBuffer &r, vector<char> &o);

		/**
		 * 默认的包头处理
         * Default header handling
		 * @param i
		 * @param o
		 * @return int
		 */
		static TC_NetWorkBuffer::PACKET_TYPE echo_header_filter(TC_NetWorkBuffer::PACKET_TYPE i, vector<char> &o);

        /**
         * 获取需要过滤的包头长度
         * Get the header length that needs to be filtered
         */
        int getHeaderFilterLen();

        /**
         * 所属handle组的handle数(每个handle一个对象)
         * Number of handles belonging to the handle group (one object per handle)
         * @return int
         */
        int getHandleNum();

		/**
		 * 初始化处理线程,线程将会启动
         * Initialize the processing thread, which will start
		 */
		template<typename T, typename ...Args>
		void setHandle(size_t n, Args&&... args)
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
				HandlePtr handle = new T(args...);

				handle->setHandleIndex(i);

				handle->setEpollServer(this->getEpollServer());

				handle->setBindAdapter(this);

				_handles.push_back(handle);
			}
		}

		/**
		 * 获取第几个句柄
         * Get the index of the handle
		 * @param index
		 * @return
		 */
		HandlePtr getHandle(size_t index) {
			assert(index <= _iHandleNum);
			assert(getEpollServer()->isMergeHandleNetThread());
			return _handles[index];
		}

		/*
		 * 设置服务端积压缓存的大小限制(超过大小启用)
         * Set the size limit of the server's backlog cache (exceeding the size enabled)
		 */
		void setBackPacketBuffLimit(size_t iLimitSize) { _iBackPacketBuffLimit = iLimitSize; }

		/**
		 * 获取服务端回包缓存的大小限制(超过大小启用)
         * Get the size limit of the server-side packet back cache (exceeding the size enabled)
		 */
		size_t getBackPacketBuffLimit() const { return _iBackPacketBuffLimit; }

		/*
		 * 设置服务端5/s最低发送字节
         * Set the Server 5/s Minimum Sending Bytes
		 */
		void setBackPacketBuffMin(size_t iMinLimit) { _iBackPacketBuffMin = iMinLimit; }

		/**
		 * 获取服务端5/s最低发送字节
         * Get the Server 5/s Minimum Sending Bytes
		 */
		size_t getBackPacketBuffMin() const { return _iBackPacketBuffMin; }

		/**
		 * 获取服务端接收队列(如果_rnbuffer有多个, 则根据调用者的线程id来hash获取)
         * Get the server receive queue (if there's more than one _rnbuffer, get from the hash based on the caller's thread id)
         * 
		 */
		recv_queue &getRecvQueue(uint32_t handleIndex);

		/**
		 * 获取handles
         * Get handles
		 */
		const vector<HandlePtr> &getHandles() { return _handles; }

		/**
		 * 是否是队列模式(默认是False的)
         * Whether it is the queue mode (Defualt false)
		 */
		bool isQueueMode() const { return _queueMode; }

		/**
		 * 开启队列模式(同一个连接的请求, 落在同一个handle处理线程中)
         * Open queue mode (The requests from the same connecion will fall in the same handle processing thread )
		 */
		void enableQueueMode() { _queueMode = true; }

        /**
         * 等待队列数据
         * Wait for the queue data
         */
        void waitAtQueue(uint32_t handleIndex, uint32_t waitTime);

        /**
         * 通知某个具体handle醒过来
         * Notify a specific handle to wake up
         * @param handleIndex
         */
        void notifyHandle(uint32_t handleIndex);

        /**  
         * 设置close回调函数 
         * Set close callback function
         */
        void setOnClose(const close_functor& f) { _closeFunc = f; } 

        /**
         * 注册鉴权包裹函数
         * Regist Authentication Package Function
         * @param apwf
         */
        void setAuthProcessWrapper(const auth_process_wrapper_functor& apwf) { _authWrapper = apwf; }

        void setAkSk(const std::string& ak, const std::string& sk) { _accessKey = ak; _secretKey = sk; }

        bool checkAkSk(const std::string& ak, const std::string& sk) { return ak == _accessKey && sk == _secretKey; }

        std::string getSk(const std::string& ak) const { return (_accessKey == ak) ? _secretKey : ""; }

        void setSSLCtx(const shared_ptr<TC_OpenSSL::CTX>& ctx) { _ctx = ctx; }
        shared_ptr<TC_OpenSSL::CTX> getSSLCtx() { return _ctx; };

	private:
		/**
		 * 获取等待的队列锁
         * Get the waiting queue lock
		 * @return
		 */
		TC_ThreadLock &getLock(uint32_t handleIndex);

    public:

        //统计上报的对象
        //Count reporting objects
        PropertyReport * _pReportQueue      = NULL;
        PropertyReport * _pReportConRate    = NULL;
        PropertyReport * _pReportTimeoutNum = NULL;

    protected:
        friend class TC_EpollServer;

		/**
		 * 加锁
         * Add lock
		 */
		mutable std::mutex		_mutex;

        /**
         * 服务
         * Service
         */
        TC_EpollServer  *_pEpollServer = NULL;

		/**
		 * Adapter所用的HandleGroup
         * the HandleGroup used by Adapter
		 */
		vector<HandlePtr> _handles;

        /**
         * 协议解析
         * Destruct the protocol
         */
        TC_NetWorkBuffer::protocol_functor _pf;

        /**
         * 首个数据包包头过滤
         * First packet header filtering
         */
        header_filter_functor _hf;

        /**
         * adapter的名字
         * adapter name
         */
        string          _name;

        /**
         * 监听fd
         * listen fd
         */
        TC_Socket       _s;

        /**
         * 绑定的IP
         * binded ip
         */
        TC_Endpoint     _ep;

        /**
         * 最大连接数
         * the maximum number of connections
         */
        int             _iMaxConns;

        /**
         * 当前连接数
         * the current number of connections
         */
        std::atomic<int> _iCurConns;

        /**
         * Handle个数
         * the number of Handle
         */
        size_t          _iHandleNum;

        /**
         * 允许的Order
         * the Allowed Order
         */
        volatile EOrder _eOrder;

        /**
         * 允许的ip
         * the Allowed IP
         */
        vector<string>  _vtAllow;

        /**
         * 禁止的ip
         * the Disabled IP
         */
        vector<string>  _vtDeny;

		/**
		 * 每个线程都有自己的队列
		 * 0: 给共享队列模式时使用
		 * 1~handle个数: 队列模式时使用
         * Every thread has its own queue.
         * 0: Use when sharing queue mode
         * 1~handle count: Use when queue mode
		 */
		vector<shared_ptr<DataQueue>> _threadDataQueue;

		/**
		 * 接收队列数据总个数
         * the total amount of the received queue data
		 */
		atomic<size_t>  _iRecvBufferSize{0};

		/**
		 * 发送队列数据总个数
         * the total amount of the sent queue data
		 */
		atomic<size_t>  _iSendBufferSize{0};

		/**
		 * 队列最大容量
         * the maximum capacity of the queue
		 */
		int             _iQueueCapacity;

        /**
         * 消息超时时间（从入队列到出队列间隔)(毫秒）
         * Message timeout (from queue entry to queue exit interval) (milliseconds)
         */
        int             _iQueueTimeout;

        /**
         * 首个数据包包头长度
         * First packet header length
         */
        int             _iHeaderLen;

        /**
         * 上次心跳发送时间
         * Last heartbeat sent time
         */
        volatile time_t          _iHeartBeatTime;

        /**
         * 协议名称,缺省为"tars"
         * Protocol name, default is "tars"
         */
        string                  _protocolName;

		/**
		 * 回包缓存限制大小
         * Packet Back Cache Limit Size
		 */
		size_t					_iBackPacketBuffLimit = 0;

		/**
		 * 回包速度最低限制(5/s), 默认1K
         * Minimum Packet Return Speed Limit (5/s), default 1K
		 */
		size_t					_iBackPacketBuffMin = 1024;

		//队列模式
        //Queue Mode
		bool _queueMode 		= false;

		//listen模式
        //Listen Mode
		bool _manualListen		= false;

        /**
         * 包裹认证函数,不能为空
         * Package authentication function, cannot be empty
         */
        auth_process_wrapper_functor _authWrapper;

        /**
         * 该obj的AK SK
         * the AK  SK of the object
         */
        std::string              _accessKey;
        std::string              _secretKey;

        /**
         * ssl ctx
         */
	    shared_ptr<TC_OpenSSL::CTX> _ctx;

        //连接关闭的回调函数 
        //Callback function with connection closed
        close_functor           _closeFunc;

    };

    ////////////////////////////////////////////////////////////////////////////
    // 服务连接管理
    // Service Connection Management
    /**
     *  建立连接的socket信息
     *  Socket information for establishing connections
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
         * Constructor
         * @param lfd
         * @param s
         * @param ip
         * @param port
         */
        Connection(BindAdapter *pBindAdapter, int lfd, int timeout, int fd, const string& ip, uint16_t port);

        /**
         * udp连接
         * UDP connection
         * @param fd
         */
        Connection(BindAdapter *pBindAdapter, int fd);

        /**
         * 析构函数
         * Destructor
         */
        virtual ~Connection();

        /**
         * 链接所属的adapter
         * the adapter of the connection
         */
		BindAdapterPtr& getBindAdapter()       { return _pBindAdapter; }

        /**
         * 初始化
         * Initialization
         * @param id, 连接的唯一id
         * @param id, the connection unique id
         */
        void init(unsigned int uid)         { _uid = uid; }

        /**
         * 获取连接超时时间
         * Get connection timeout
         *
         * @return int
         */
        int getTimeout() const              { return _timeout; }

        /**
         * 获取线程的惟一id
         * Get thread unique id
         *
         * @return unsigned int
         */
        uint32_t getId() const              { return _uid; }

        /**
         * 获取监听fd
         * Get listening id
         *
         * @return int
         */
        int getListenfd() const             { return _lfd; }

        /**
         * 当前连接fd
         * Current connection fd
         *
         * @return int
         */
        int getfd() const                   { return _sock.getfd(); }

        /**
         * 是否有效
         * Whether it is valid
         *
         * @return bool
         */
        bool isValid() const                { return _sock.isValid();}

        /**
         * 远程IP
         * Remote IP
         *
         * @return string
         */
        string getIp() const                { return _ip; }

        /**
         * 远程端口
         * Remote Port
         *
         * @return uint16_t
         */
        uint16_t getPort() const            { return _port; }

        /**
         * 设置首个数据包包头需要过滤的字节数
         * Set the number of bytes the first packet header needs to filter
         */
        void setHeaderFilterLen(int iHeaderLen)     { _iHeaderLen = iHeaderLen; }

        /**
         * 设置关闭,发送完当前数据就关闭连接
         * Set shutdown to close connection after sending current data
         */
        bool setClose();

        /**
         * 获取连接类型
         * Get the type of the connection
         */
        EnumConnectionType getType() const          { return _enType; }

	    /**
	 	* 是否是空连接
        * Whether there's empty connection.
	 	*/
        bool isEmptyConn() const  {return _bEmptyConn;}

        /**
         * Init Auth State;
         */
        void tryInitAuthState(int initState);

		/**
		 * 接收数据buffer
         * Receive data buffer
		 */
		TC_NetWorkBuffer &getRecvBuffer() { return _recvBuffer; }

		/**
		 * 发送数据buffer
         * Send data buffer
		 */
		TC_NetWorkBuffer &getSendBuffer() { return _sendBuffer; }

		/**
		 * 发送buffer里面数据
         * Send the data in the bufer
		 * @return
		 */
		int sendBuffer();
    
        /**
         * 直接发送裸得应答数据，业务层一般不直接使用，仅仅tcp支持
         * send naked response data
         * @param buffer
         * @return int, -1:发送出错, 0:无数据, 1:发送完毕, 2:还有数据
         * @return int, -1: sending error, 0: no data, 1: send completely, 2: data retains
         * @return
         */
        int sendBufferDirect(const std::string& buff);

        /**
         * 直接发送裸得应答数据，业务层一般不直接使用，仅仅tcp支持
         * send naked response data
         * @param buffer
         * @return int, -1:发送出错, 0:无数据, 1:发送完毕, 2:还有数据
         * @return int, -1: sending error, 0: no data, 1: send completely, 2: data retains
         * @return
         */
        int sendBufferDirect(const std::vector<char>& buff);

	    /**
		 * 关闭连接
		 * Close the connection
		 * @param fd
		 */
	    void close();

	    friend class NetThread;

    protected:

		/**
		 * 添加发送buffer
         * Add sanding buffer
		 * @param buffer
		 * @return int, -1:发送出错, 0:无数据, 1:发送完毕, 2:还有数据
         * @return int, -1: sending error, 0: no data, 1: send completely, 2: data retains
		 */
		int send(const shared_ptr<SendContext> &data);

		/**
		 * 读取数据
         * Read data
		 * @param fd
		 * @return int, -1:接收出错, 0:接收不全, 1:接收到一个完整包
         * @return int, -1: received error, 0: not receive completely, 1: receive a complete package
		 */
		int recv();

		/**
		* 接收TCP
        * Receive TCP
		*/
		int recvTcp();

		/**
		* 接收Udp
        * Receive UDP
		*/
		int recvUdp();

        /**
         * 解析协议
         * Destruct protocol
         * @param o
         */
        int parseProtocol(TC_NetWorkBuffer &rbuf);

        /**
         * 增加数据到队列中
         * Add data to the queue
         * @param vtRecvData
         */
		void insertRecvQueue(const shared_ptr<RecvContext> &recv);

        /**
         * 对于udp方式的连接，分配指定大小的接收缓冲区
         * For udp-mode connections, allocate receive buffers of a specified size
         *@param nSize
            */
        bool setRecvBuffer(size_t nSize=DEFAULT_RECV_BUFFERSIZE);

		/**
		 * 是否是tcp连接
         * Whether it is TCP connection.
		 * @return
		 */
		bool isTcp() const { return _lfd != -1; }

    public:
        /**
         * 最后刷新时间
         * Last refresh time
         */
        time_t              _iLastRefreshTime;

    protected:

        /**
         * 适配器
         * Adapter
         */
		BindAdapterPtr      _pBindAdapter;

        /**
         * TC_Socket
         */
        TC_Socket           _sock;

        /**
         * 连接的唯一编号
         * the unique id of the connection
         */
        volatile uint32_t   _uid;

        /**
         * 监听的socket
         * Listening socket
         */
        int                 _lfd;

        /**
         * 超时时间
         * Timeout
         */
        int                 _timeout;

        /**
         * ip
         */
        string              _ip;

        /**
         * 端口
         * Port
         */
        uint16_t             _port;

        /**
         * 接收数据buffer
         * the buffer to receive data
         */
        TC_NetWorkBuffer     _recvBuffer;

        /**
         * 发送数据buffer
         * the buffer to send data
         */
        TC_NetWorkBuffer    _sendBuffer;

		/**
		 * 发送数据
         * Send data
		 */
		size_t              _sendBufferSize = 0;

		/**
		 * 检查时间
         * Check time
		 */
		time_t              _lastCheckTime = 0;

		/**
		 * 发送的检查<已经发送数据, 剩余buffer大小>
         * Check Sent <Data Sent, Remaining Buffer Size>
		 */
		vector<pair<size_t, size_t>> _checkSend;

		/**
		 * 需要过滤的头部字节数
         * Number of header bytes to filter
		 */
		int                 _iHeaderLen;

        /**
         * 发送完当前数据就关闭连接
         * Close connection after sending current data
         */
        bool                _bClose;

        /**
         * 连接类型
         * Connection Type
         */
        EnumConnectionType  _enType;

        bool                _bEmptyConn;

        /*
        *接收数据的临时buffer,加这个目的是对udp接收数据包大小进行设置
        *Temporary buffer to receive data, plus this is to set the UDP receive packet size
        */
        char                *_pRecvBuffer = NULL;

        size_t              _nRecvBufferSize;

    public:
        /*
        *该连接的鉴权状态
        *Authentication status of the connection
        */
        int                 _authState;
        /*
        *该连接的鉴权状态是否初始化了
        */
        bool                _authInit;

        std::shared_ptr<TC_OpenSSL> _openssl;
    };
    ////////////////////////////////////////////////////////////////////////////
    /**
     * 带有时间链表的map
     * Map with Time Chain Table
     */
    class ConnectionList
    {
    public:
        /**
         * 构造函数
         * Constructor
         */
        ConnectionList(NetThread *pEpollServer);

        /**
         * 析够函数
         * Destructor
         */
        ~ConnectionList()
        {
	        if(_vConn)
	        {
		        //服务停止时, 主动关闭一下连接, 这样客户端会检测到, 不需要等下一个发送包时, 发送失败才知道连接被关闭
		        for (auto it = _tl.begin(); it != _tl.end(); ++it) {
			        if (_vConn[it->second].first != NULL) {
				        _vConn[it->second].first->close();
			        }
		        }
		        delete[] _vConn;
	        }
        }

        /**
         * 初始化大小
         * Initial size
         * @param size
         */
        void init(uint32_t size, uint32_t iIndex = 0);

        /**
         * 获取惟一ID
         * Get the unique ID
         *
         * @return unsigned int
         */
        uint32_t getUniqId();

        /**
         * 添加连接
         * Add Connection
         * @param cPtr
         * @param iTimeOutStamp
         */
        void add(Connection *cPtr, time_t iTimeOutStamp);

        /**
         * 刷新时间链
         * Refresh the connectiom
         * @param uid
         * @param iTimeOutStamp, 超时时间点
         * @param iTimeOutStamp, Timeout Point
         */
        void refresh(uint32_t uid, time_t iTimeOutStamp);

        /**
         * 检查超时数据
         * Check Timeout
         */
        void checkTimeout(time_t iCurTime);

        /**
         * 获取某个监听端口的连接
         * Get a connection to a listening port
         * @param lfd
         * @return vector<TC_EpollServer::ConnStatus>
         */
        vector<ConnStatus> getConnStatus(int lfd);

        /**
         * 获取某一个连接
         * Get a certain connection
         * @param p
         * @return T
         */
        Connection* get(uint32_t uid);

        /**
         * 删除连接
         * Delete connection
         * @param uid
         */
        void del(uint32_t uid);

        /**
         * 大小
         * Size
         * @return size_t
         */
        size_t size();

    protected:
        typedef pair<Connection*, multimap<time_t, uint32_t>::iterator> list_data;

        /**
         * 内部删除, 不加锁
         * Internal Delete, No Lock
         * @param uid
         */
        void _del(uint32_t uid);

    protected:
		/**
		 * 无锁
         * No Lock
		 */
		TC_ThreadMutex                 _mutex;

		/**
		 * 服务
         * Service
		 */
		NetThread                      *_pEpollServer;

        /**
         * 总计连接数
         * Total connection amount
         */
        volatile uint32_t               _total;

        /**
         * 空闲链表
         * Empty link list
         */
        list<uint32_t>                  _free;

        /**
         * 空闲链元素个数
         * number of the elements in the empty link
         */
        volatile size_t                 _free_size;

        /**
         * 链接
         * Connection
         */
        list_data                       *_vConn;

        /**
         * 超时链表
         * Timeout link list
         */
        multimap<time_t, uint32_t>      _tl;

        /**
         * 上次检查超时时间
         * Last timeout time
         */
        time_t                          _lastTimeoutTime;

        /**
         * 链接ID的魔数
         * Magic Number of Link IDs
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
         * Constructor
         */
        NetThread(TC_EpollServer *epollServer, int index);

        /**
         * 析构函数
         * Destructor
         */
        virtual ~NetThread();

		/**
		 * 获取网络线程的index
         * Get the index for the network threads
		* @return
		*/
		int getIndex() const { return _threadIndex; }

        /**
         * 网络线程执行函数
         * Network thread execution function
         */
        virtual void run();

        /**
         * 停止网络线程
         * Stop the network thread
         */
        void terminate();

        /**
         * 生成epoll
         * Generate epoll
         */
        void createEpoll(uint32_t maxAllConn);

        /**
         * 初始化udp监听
         * Initialize UDP listening
         */
        void initUdp(const unordered_map<int, BindAdapterPtr> &listeners);

        /**
         * 是否服务结束了
         * Whether the service is end.
         *
         * @return bool
         */
        bool isTerminate() const    { return _bTerminate; }

        /**
         * 获取Epoller对象
         * Get the Epoller Object
         * 
         * @return TC_Epoller*
         */
        TC_Epoller* getEpoller()    { return &_epoller; }

		/**
		 * 唤醒网络线程
         * Wake up the network thread
		 */
		void notify();

		/**
		 * 关闭连接
         * Close Connection
		 * @param uid
		 */
		void close(const shared_ptr<RecvContext> &data);

		/**
		* 发送数据
        * Send data
		* @param uid
		* @param s
		*/
		void send(const shared_ptr<SendContext> &data);

		/**
		 * 获取某一监听端口的连接数
         * Get the number of connections for a listening port
		 * @param lfd
		 *
		 * @return vector<TC_EpollServer::ConnStatus>
		 */
		vector<TC_EpollServer::ConnStatus> getConnStatus(int lfd);

        /**
         * 获取连接数
         * Get the number of connections
         *
         * @return size_t
         */
        size_t getConnectionCount()     { return _list.size(); }

        /**
         * 记录日志
         * Logging
         * @param s
         */
        void debug(const string &s) const;

        /**
         * INFO日志
         * INFO LOG
         * @param s
         */
        void info(const string &s) const;

	    /**
		 * TARS日志
         * TARS LOG
		 * @param s
		 */
	    void tars(const string &s) const;

        /**
         * 记录错误日志
         * Log errors
         * @param s
         */
        void error(const string &s) const;

        /**
         * 是否启用防止空链接攻击的机制
         * Whether the mechanism to prevent null link attacks is enabled.
         * @param bEnable
         */
        void enAntiEmptyConnAttack(bool bEnable);

        /**
         *设置空连接超时时间
         *Set empty connection timeout
         */
        void setEmptyConnTimeout(int timeout);

        /**
         *设置udp的接收缓存区大小，单位是B,最小值为8192，最大值为DEFAULT_RECV_BUFFERSIZE
         *Sets the size of the receiving buffer in UDP in B, with a minimum of 8192 and a maximum of DEFAULT_RECV_BUFFERSIZE
         */
        void setUdpRecvBufferSize(size_t nSize=DEFAULT_RECV_BUFFERSIZE);


    protected:

        /**
         * 获取连接
         * Get connection
         * @param id
         *
         * @return ConnectionPtr
         */
        Connection *getConnectionPtr(uint32_t uid)      { return _list.get(uid); }

        /**
         * 添加tcp链接
         * Add TCP connection
         * @param cPtr
         * @param iIndex
         */
        void addTcpConnection(Connection *cPtr);

        /**
         * 添加udp连接
         * Add UDP connection
         * @param cPtr
         * @param index
         */
        void addUdpConnection(Connection *cPtr);

        /**
         * 删除链接
         * Delete connection
         * @param cPtr
         * @param bEraseList 是否是超时连接的删除
         * @param bEraseList Whether it is deletion of timeout connection
         * @param closeType  关闭类型,0:表示客户端主动关闭；1:服务端主动关闭;2:连接超时服务端主动关闭
         * @param closeType  Close type, 0: indicates active closure of client, 1: active closure of server, 2: active closure of connection timeout server
         */
        void delConnection(Connection *cPtr, bool bEraseList = true, EM_CLOSE_T closeType=EM_CLIENT_CLOSE);

        /**
         * 处理管道消息
         * Processing Pipeline Messages
         */
        void processPipe();

        /**
         * 处理网络请求
         * Processing Network Request
         */
        void processNet(const epoll_event &ev);

        /**
         * 空连接超时时间
         * Empty connection timeout
         */
        int getEmptyConnTimeout() const;

        /**
         *是否空连接检测
         *Empty connection detection examination
         */
        bool isEmptyConnCheck() const;

        friend class BindAdapter;
        friend class ConnectionList;
        friend class TC_EpollServer;

    private:
        /**
         * 服务
         * Service
         */
        TC_EpollServer              *_epollServer;

		/**
		 * net线程的id
         * the net thread id
		 */
		std::thread::id             _threadId;

		/**
		 * 线程索引
         * the thread index
		 */
		int                         _threadIndex;

        /**
         * epoll
         */
        TC_Epoller                  _epoller;

        /**
         * 停止
         * Stop
         */
        bool                        _bTerminate;

		/**
		 * 通知epoll
         * Notify epoll
		 */ 
		TC_Epoller::NotifyInfo 		_notify;

        /**
         * 管理的连接链表
         * Managed Link List
         */
        ConnectionList              _list;

        /**
         * 发送队列
         * Sending Queue
         */
        send_queue                  _sbuffer;

        /**
         *空连接检测机制开关
         *Switch for empty connection detection mechanism
         */
        bool                         _bEmptyConnAttackCheck;


        /**
         * 空连接超时时间,单位是毫秒,默认值2s,
         * 该时间必须小于等于adapter自身的超时时间
         * Empty connection timeout in milliseconds, default 2s,
         * The time must be less than or equal to the adapter's own timeout
         */
        int                            _iEmptyCheckTimeout;

        /**
         * udp连接时接收包缓存大小,针对所有udp接收缓存有效
         * Received packet cache size on UDP connection, valid for all UDP receive caches
         */
        size_t                         _nUdpRecvBufferSize;

		/**
		 * 通知信号
         * Notification signal
		 */
		bool                        _notifySignal = false;
    };
    ////////////////////////////////////////////////////////////////////////////
public:
    /**
     * 构造函数
     * Constructor
     */
    TC_EpollServer(unsigned int iNetThreadNum = 1);

    /**
     * 析构函数
     * Destructor
     */
    ~TC_EpollServer();

    /**
     * 是否启用防止空链接攻击的机制
     * Whether mechanisms to prevent empty link attacks are enabled
     * @param bEnable
     */
    void enAntiEmptyConnAttack(bool bEnable);

    /**
     *设置空连接超时时间
     *Set empty connection timeout
     */
    void setEmptyConnTimeout(int timeout);

    /**
     * 设置本地日志
     * Set local log
     * @param plocalLogger
     */
    void setLocalLogger(RollWrapperInterface *pLocalLogger)       { _pLocalLogger = pLocalLogger; }

	/**
	 * 选择网络线程
     * Select network threads
	 * @param fd
	 */
	inline NetThread* getNetThreadOfFd(int fd) { return _netThreads[fd % _netThreads.size()]; }

	/**
	 * 合并handle线程和网络线程
     * Merge handle and network threads
	 * @param merge
	 */
	void setMergeHandleNetThread(bool merge) { _mergeHandleNetThread = merge; }

	/**
	 * 是否合并handle线程网络线程
     * Whether to merge handle thread network threads
	 * @return
	 */
	inline bool isMergeHandleNetThread() const { return _mergeHandleNetThread; }

    /**
     * 绑定监听socket
     * Bind listening socket
     * @param ls
     */
    int  bind(BindAdapterPtr &lsPtr);

    /**
     * 启动业务处理线程
     * Start Business Processing Thread
     */
    void startHandle();

    /**
     * 生成epoll
     * Generate epoll
     */
    void createEpoll();

    /**
     * 运行
     * Run
     */
    void waitForShutdown();

    /**
     * 停止服务
     * Stop Service
     */
    void terminate();

    /**
     * 是否服务结束了
     * Whether the service is over
     *
     * @return bool
     */
    bool isTerminate() const    { return _bTerminate; }

    /**
     * 根据名称获取BindAdapter
     * Get BindAdapter according to the name
     * @param sName
     * @return BindAdapterPtr
     */
    BindAdapterPtr getBindAdapter(const string &sName);

	/**
	 * 获取所有adatapters
     * Get all adapters
	 * @return
	 */
	vector<BindAdapterPtr> getBindAdapters();

    /**
     * 向网络线程添加连接
     * Add remote connection to the network thread
     */
    void addConnection(Connection * cPtr, int fd, CONN_TYPE iType);

	/**
	 * 关闭连接
     * Close connection
	 * @param uid
	 */
	void close(const shared_ptr<TC_EpollServer::RecvContext> &data);

	/**
	 * 发送数据
     * Send data
	 * @param uid
	 * @param s
	 */
	void send(const shared_ptr<SendContext> &data);

    /**
     * 获取某一监听端口的连接数
     * Get the connection amount of a certain listening port
     * @param lfd
     *
     * @return vector<TC_EpollServer::ConnStatus>
     */
    vector<ConnStatus> getConnStatus(int lfd);

	/**
	 * 获取监听socket信息
     * Get the information of the listening socket
	 *
	 * @return map<int,ListenSocket>
	 */
	unordered_map<int, BindAdapterPtr> getListenSocketInfo();

    /**
     * 获取所有连接的数目
     * Get the amount of all connections
     *
     * @return size_t
     */
    size_t getConnectionCount();

    /**
     * 记录日志
     * Logging
     * @param s
     */
    void debug(const string &s) const;

    /**
     * INFO日志
     * INFO LOG
     * @param s
     */
    void info(const string &s) const;

     /**
     * 记录错误日志
     * Log errors
     * @param s
     */
    void error(const string &s) const;

	/**
	 * tars日志
     * tars log
	 * @param s
	 */
	void tars(const string &s) const;

    /**
     * 获取网络线程的数目
     * Get the amount of the network threads
     */
    unsigned int getNetThreadNum() { return _netThreadNum; }

    /**
     * 获取网络线程的指针集合
     * Get the collection of pointers for a network thread 
     */
    vector<TC_EpollServer::NetThread*> getNetThread() { return _netThreads; }

    /**
     * 停止线程
     * Stop the thread
     */
    void stopThread();

    /**
     * 获取所有业务线程的数目
     * Get the amount of all the bussiness threads
     */
    size_t getLogicThreadNum();

    // 接收新的客户端链接时的回调
    typedef std::function<void (TC_EpollServer::Connection*)> accept_callback_functor;

    /*
     * 设置接收链接的回调
     */
    void setOnAccept(const accept_callback_functor& f) { _acceptFunc = f; }

	//回调给应用服务
    //Callback to application service
	typedef std::function<void(TC_EpollServer*)> application_callback_functor;

	/**
	 * 设置waitForShutdown线程回调的心跳
     * Set the heartbeat of the thread callback of the waitForShutdown
	 * @param hf [description]
	 */
	void setCallbackFunctor(const application_callback_functor &hf) { _hf = hf; }

    //网络线程发送心跳的函数
    //Function for network threads to send heartbeats
    typedef std::function<void(const string &)> heartbeat_callback_functor;

    /**
     * 设置netthread网络线程发送心跳的函数
     * Function for setting netthreaded network threads to send heartbeats
     * @param hf [description]
     */
    void setHeartBeatFunctor(const heartbeat_callback_functor& heartFunc) { _heartFunc = heartFunc; }
    heartbeat_callback_functor& getHeartBeatFunctor() { return _heartFunc; }

protected:

    friend class BindAdapter;

	/**
	 * 接收句柄
     * Receiving handle
	 * @param fd
	 * @return
	 */
	bool accept(int fd, int domain = AF_INET);

	/**
	 * 绑定端口
     * Bind Port
	 * @param ep
	 * @param s
	 * @param manualListen
	 */
	void bind(const TC_Endpoint &ep, TC_Socket &s, bool manualListen);

	static void applicationCallback(TC_EpollServer *epollServer);
private:
    /**
     * 网络线程
     * Network Thread
     */
    std::vector<NetThread*>     _netThreads;

    /*
     * 网络线程数目
     * Network Thread Amount
     */
    unsigned int                _netThreadNum;

	/**
 	* epoll
 	*/
	TC_Epoller                  _epoller;

	/**
	 * 通知epoll
     * Notify epoll
	 */
	TC_Epoller::NotifyInfo 		_notify;

    /*
     * 服务是否停止
     * Whether the service is stopped
     */
    bool                        _bTerminate;

    /*
     * 业务线程是否启动
     * Whether the bussiness thread is started.
     */
    bool                        _handleStarted;

	/**
	 * 合并网络和业务线程
     * Merge network and business threads
	 */
	bool                        _mergeHandleNetThread = false;

    /**
     * 本地循环日志
     * Local Loop Log
     */
    RollWrapperInterface        *_pLocalLogger;

	/**
	 *
	 */
	vector<BindAdapterPtr>       _bindAdapters;

	/**
 	* 监听socket
    * Listening socket
 	*/
	unordered_map<int, BindAdapterPtr>    _listeners;

	/**
	 * 应用回调
     * Application callback
	 */
	application_callback_functor _hf;

    /**
     * 发送心跳的函数
     * Heartbeat Sending Function
     */
    heartbeat_callback_functor _heartFunc;

    /**
     * 接收链接的回调函数
     */
    accept_callback_functor _acceptFunc;
};

typedef TC_AutoPtr<TC_EpollServer> TC_EpollServerPtr;

}

#endif
