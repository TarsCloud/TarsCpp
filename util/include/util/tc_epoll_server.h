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

#pragma once

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
#include "util/tc_thread_pool.h"
#include "util/tc_common.h"
#include "util/tc_network_buffer.h"
#include "util/tc_transceiver.h"
#include "util/tc_cas_queue.h"
#include "util/tc_coroutine.h"
#include "util/tc_openssl.h"

using namespace std;

/**
 * 服务模型说明:
 * - 服务模型全面支持协程化, 一共有四种模式, 可以通过setOpenCoroutine来设置, 这四种模式是:
 * 1 NET_THREAD_QUEUE_HANDLES_THREAD: 独立网路线程 + 独立handle线程: 网络线程负责收发包, 通过队列唤醒handle线程中处理
 * 2 NET_THREAD_QUEUE_HANDLES_CO: 独立网路线程组 + 独立handle线程: 网络线程负责收发包, 通过队列唤醒handle线程中处理, handle线程中启动协程处理
 * 3 NET_THREAD_MERGE_HANDLES_THREAD: 合并网路线程 + handle线程(线程个数以处理线程配置为准, 网络线程配置无效): 连接分配到不同线程中处理(如果是UDP, 则网络线程竞争接收包), 这种模式下延时最小, 相当于每个包的收发以及业务处理都在一个线程中
 * 4 NET_THREAD_MERGE_HANDLES_CO: 合并网路线程 + handle线程(线程个数以处理线程配置为准, 网络线程配置无效): 连接分配到不同线程中处理(如果是UDP, 则网络线程竞争接收包), 每个包会启动协程来处理
 *
 * 设计说明:
 * - 设计上支持协程化, 这样所有服务端处理请求的线程, 其实本质上都在协程中, 协程的调度背后使用的epoller, 参考tc_coroutine的说明
 * - 结合到客户端rpc调用, 服务端处理过程中如果发起对另外一个服务的rpc, 这时rpc作为客户端能复用服务处理线程中的epoller, 从而使得客户端和服务端在同一个epoller中使用协程调度
 * - 这样设计的优势是无论是rpc同步调用还是异步调用, 都在一个线程中执行, 减少了线程的切换, 同时大幅度减少了请求的延时!
 *
 * 队列说明:
 * - 网络线程和业务逻辑线程之间, 存在队列DataQueue
 * - 这些DataQueue都封装在DataBuffer类中
 * - DataBuffer对象被BindAdapter以及处理类(Handle)所持有, 即每个BindAdapter会初始化一个DataBuffer并设置给相关的Handle
 * - 不同服务模型下以及设置了队列模式下, 队列的使用是不同的
 * 缺省, 非队列模式(enableQueueMode: false)
 * 1 NET_THREAD_QUEUE_HANDLES_THREAD
 * - DataBuffer中实际使用的DataQueue就一个, 多个Handle线程竞争该DataQueue, 以获取数据
 * - 多个handle线程都会等待在队列上
 * - 网络线程接收到数据后, 会唤醒等待在队列上handle线程
 * 2 NET_THREAD_QUEUE_HANDLES_CO
 * - DataBuffer中实际使用的DataQueue就一个, 多个Handle协程竞争该DataQueue, 以获取数据
 * - 注意: handle线程(协程)都并不会等待在队列上, 而是通过协程调度, 如果没有数据, 协程调度阻塞
 * - 网络线程接收数据后, 会通过协程调度器唤醒协程
 * 3 NET_THREAD_MERGE_HANDLES_THREAD
 * - 网络和handle都在同一个线程中, 但是都以协程模式在运行, 注意: handle协程并不是一个请求一个协程
 * - 此时DataBuffer中实际使用的DataQueue和Handle是一对一的
 * - Handle本质运行在协程中, 但是不会每个请求都创建一个协程来执行, 而是连续执行一定的请求后, 会yield释放协程, 让网络线程继续
 * - Handle协程不会阻塞在队列上, 而是通过协程调度器来唤醒
 * - 网络线程接收数据后, 会通过协程调度器唤醒协程
 * 4 NET_THREAD_MERGE_HANDLES_CO
 * - 网络和handle都在同一个线程中, 但是都以协程模式在运行, 注意: handle协程一个请求创建一个协程
 * - 此时DataBuffer中实际使用的DataQueue和Handle是一对一的
 * - Handle协程不会阻塞在队列上, 而是通过协程调度器来唤醒
 * - 网络线程接收数据后, 会通过协程调度器唤醒协程
 *
 * 在队列模式下, 一个连接上来的请求都在同一个handle线程中处理!
 */
namespace tars
{
class PropertyReport;

namespace detail
{
    /**
    * log接口
    */
    class LogInterface
    {
    public:
        virtual ~LogInterface() {}

        /**
         * 记录日志
         * @param s
         */
        virtual void debug(const string &s) const = 0;

        /**
        * INFO日志
        * @param s
        */
        virtual void info(const string &s) const = 0;

        /**
         * tars日志
         * @param s
         */
        virtual void tars(const string &s) const = 0;

        /**
         * 记录错误日志
         * @param s
         */
        virtual void error(const string &s) const = 0;

    };
}

struct TC_EpollServer_Exception : public TC_Exception
{
   TC_EpollServer_Exception(const string &buffer) : TC_Exception(buffer) {};
   ~TC_EpollServer_Exception() {};
};

class TC_EpollServer : public TC_HandleBase, public detail::LogInterface
{
public:

    enum EM_CLOSE_T
    {
        EM_CLIENT_CLOSE = 0,         //客户端主动关闭
        EM_SERVER_CLOSE = 1,        //服务端业务主动调用close关闭连接,或者框架因某种异常主动关闭连接
        EM_SERVER_TIMEOUT_CLOSE = 2  //连接超时了，服务端主动关闭
    };

    enum
    {
        MIN_EMPTY_CONN_TIMEOUT  = 5 * 1000,      /*空链接超时时间(ms)*/
        DEFAULT_RECV_BUFFERSIZE = 64 * 1024     /*缺省数据接收buffer的大小*/
    };

    //定义加入到网络线程的fd类别
    enum CONN_TYPE
    {
        TCP_CONNECTION = 0,
        UDP_CONNECTION = 1,
    };

    class RecvContext;
    class SendContext;
    class NetThread;
    class ConnectionList;
    class Connection;
    class BindAdapter;
    class Handle;

    typedef shared_ptr<BindAdapter> BindAdapterPtr;
    typedef shared_ptr<Handle> HandlePtr;

    // typedef TC_AutoPtr<BindAdapter> BindAdapterPtr;
    // typedef TC_AutoPtr<Handle> HandlePtr;

//    using close_functor = std::function<void(void *, EM_CLOSE_T)>;
    using header_filter_functor = std::function<TC_NetWorkBuffer::PACKET_TYPE(TC_NetWorkBuffer::PACKET_TYPE, vector<char> &)> ;

    ////////////////////////////////////////////////////////////////////////////
    /**
    * 接收包的上下文
    */
    class RecvContext : public std::enable_shared_from_this<RecvContext>
    {
    public:
        RecvContext(int threadIndex, uint32_t uid,
                    const TC_Socket::addr_type &addr,
                    int fd,
                    const BindAdapterPtr & adapter,
                    bool isClosed = false,
                    int closeType = EM_CLIENT_CLOSE)
                : _threadIndex(threadIndex)
                , _uid(uid)
                , _addr(addr)
                , _fd(fd)
                , _adapter(adapter)
                , _isClosed(isClosed)
                , _closeType(closeType)
                , _recvTimeStamp(TNOWMS)
        {}
        inline int threadIndex() const     { return _threadIndex; }
        inline uint32_t uid() const        { return _uid; }
        inline const TC_Socket::addr_type& addr() const      { return _addr; }
        inline const string & ip() const   { parseIpPort(); return _ip; }
        inline uint16_t port() const       { parseIpPort(); return _port; }
        inline vector<char> & buffer()     { return _rbuffer; }
        inline const vector<char> & buffer() const { return _rbuffer; }
        inline int64_t recvTimeStamp() const { return _recvTimeStamp; }
        inline bool isOverload() const     { return _isOverload; }
        inline void setOverload()          { _isOverload = true; }
        inline bool isClosed() const       { return _isClosed; }
        inline int fd() const { return _fd; }
	    //连接是否还存在(客户端已经关闭)
	    bool connectionExists() const
        {
            auto adapter = _adapter.lock();
            if (adapter)
            {
                Connection *cPtr = adapter->getNetThread(_threadIndex)->getConnectionPtr(_uid);
                return cPtr != NULL;
            }
            return false;
        }
        //  { Connection *cPtr = _adapter->getNetThread(_threadIndex)->getConnectionPtr(_uid); return cPtr != NULL; }
        inline BindAdapterPtr adapter()  { return _adapter.lock(); }
        inline int closeType() const       { return _closeType; }
        inline void setCloseType(int closeType) { _closeType = closeType; }
        inline shared_ptr<SendContext> createSendContext()     { return std::make_shared<SendContext>(shared_from_this(), 's'); }
        inline shared_ptr<SendContext> createCloseContext()    { return std::make_shared<SendContext>(shared_from_this(), 'c'); }
    protected:
        void parseIpPort() const;
    protected:
    	int _threadIndex;       //网络线程id
        uint32_t _uid;            /**连接标示*/
        TC_Socket::addr_type _addr;
        mutable string _ip;             /**远程连接的ip*/
        mutable uint16_t _port;           /**远程连接的端口*/
        int _fd;                /*保存产生该消息的fd，用于回包时选择网络线程*/
        weak_ptr<BindAdapter> _adapter;        /**标识哪一个adapter的消息*/
        vector<char> _rbuffer;        /**接收的内容*/
        bool _isOverload = false;     /**是否已过载 */
        bool _isClosed = false;       /**是否已关闭*/
        int _closeType;     /*如果是关闭消息包，则标识关闭类型,0:表示客户端主动关闭；1:服务端主动关闭;2:连接超时服务端主动关闭*/
        int64_t _recvTimeStamp;  /**接收到数据的时间*/
    };

    /**
    * 发送包的上下文
    * 由RecvContext创建出来
    */
    class SendContext
    {
    public:
        SendContext(const shared_ptr<RecvContext> & context, char cmd)
                : _context(context), _cmd(cmd)
        {
            _sbuffer = std::make_shared<TC_NetWorkBuffer::Buffer>();
        }

        inline const shared_ptr<RecvContext> & getRecvContext() { return _context; }
        inline void setBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer>& buff) { _sbuffer = buff; }
        inline const shared_ptr<TC_NetWorkBuffer::Buffer> & buffer() { return _sbuffer; }
        inline char cmd() const        { return _cmd; }
        inline uint32_t uid() const    { return _context->uid(); }
        inline int fd() const          { return _context->fd(); }
        inline const string & ip() const { return _context->ip(); }
        inline uint16_t port() const   { return _context->port(); }

        friend class RecvContext;

    protected:
        shared_ptr<RecvContext>              _context;
        char _cmd;                                            /**send包才有效, 命令:'c',关闭fd; 's',有数据需要发送*/
        shared_ptr<TC_NetWorkBuffer::Buffer> _sbuffer;        /**发送的内容*/
    };

//    typedef TC_CasQueue<shared_ptr<RecvContext>> recv_queue;
    typedef TC_ThreadQueue<shared_ptr<RecvContext>> recv_queue;
//    typedef TC_CasQueue<shared_ptr<SendContext>> send_queue;
    typedef TC_ThreadQueue<shared_ptr<SendContext>> send_queue;

//    typedef recv_queue::queue_type recv_queue_type;

    ////////////////////////////////////////////////////////////////////////////

    /**
     * 数据队列包装
     */
    class DataBuffer
    {
    public:
        /**
         * 数据队列
         */
        class DataQueue
        {
        public:
        	/**
        	 * 通知等待在队列上线程都醒过来
        	 */
            inline void notify() { return _rbuffer.notifyT(); }

            /**
             * push数据到队列中, 同时唤醒某个等待处理线程
             * @param recv
             */
            inline void push_back(const shared_ptr<RecvContext> &recv ) { _rbuffer.push_back(recv); }
            inline void push_back(const deque<shared_ptr<RecvContext>> &recv ) { _rbuffer.push_back(recv); }

            /**
             * 在队列上等待
             * @param millseconds
             * @return
             */
            inline bool wait(size_t millseconds) { return _rbuffer.wait(millseconds); }

            /**
             * 弹出头部数据(如果没有数据也不阻塞)
             * @param data
             * @return
             */
            inline bool pop_front(shared_ptr<RecvContext> &data) { return _rbuffer.pop_front(data, 0, false); }

        protected:
            /**
             * 接收的数据队列
             */
            recv_queue _rbuffer;
        };

        /**
         * 构造, 传入handle处理线程,
         * @param handleNum
         */
        DataBuffer(int handleNum);

        /**
         * 通知唤醒
         * @param handleIndex
         */
        void notifyBuffer(uint32_t handleIndex);

        /**
         * 插入队列
         * @param recv
         */
        void insertRecvQueue(const shared_ptr<RecvContext> &recv);
        void insertRecvQueue(const deque<shared_ptr<RecvContext>> &recv);

        /**
         * 等待在队列上
         * @param handleIndex
         * @return
         */
        bool wait(uint32_t handleIndex);

        /**
         * 弹出数据
         * @param handleIndex
         * @param data
         * @return
         */
        bool pop(uint32_t handleIndex, shared_ptr<RecvContext> &data);

        /**
         * 是否开启队列模式
         * @return
         */
        inline bool isQueueMode() const { return _queueMode; }

        /**
         * 启用队列模式
         */
        inline void enableQueueMode() { _queueMode = true; }

        /**
         * 接收buffer的大小
         * @return
         */
        inline size_t getRecvBufferSize() const { return _iRecvBufferSize; }

        /**
         * 协程注册到DataBuffer中
         * @param handleIndex
         * @param scheduler
         */
        void registerScheduler(uint32_t handleIndex, const shared_ptr<TC_CoroutineScheduler> & scheduler);

        /**
         * 协程反注册
         * @param handleIndex
         */
        void unregisterScheduler(uint32_t handleIndex);

        /**
         * 获取handle对应的协程
         * @param handleIndex
         * @return
         */
		const shared_ptr<TC_CoroutineScheduler> &getScheduler(uint32_t handleIndex);

        /**
         * 设置等待时间
         * @param iWaitTime
         */
        inline void setWaitTime(uint32_t iWaitTime) { _iWaitTime = iWaitTime; }

    protected:

        inline int index(uint32_t handleIndex) { return handleIndex % _threadDataQueue.size(); }

        const shared_ptr<DataQueue> &getDataQueue(uint32_t handleIndex);

    protected:

        /**
         * 接收队列数据总个数
         */
        atomic<size_t>                  _iRecvBufferSize {0};

        /**
         * 是否启用队列模式, 相同连接固定
         */
        bool                            _queueMode = false;

        /**
         * 每个线程都有自己的队列
         * 0: 给共享队列模式时使用
         * 1~handle个数: 队列模式时使用
         */
        vector<shared_ptr<DataQueue>>   _threadDataQueue;

        /**
         * NET_THREAD_AND_HANDLES_CO 模式下有效
         */
        vector<shared_ptr<TC_CoroutineScheduler>>  _schedulers;

        /**
         * wait time for queue
         */
        int64_t     _iWaitTime = 3000;
    };

    ////////////////////////////////////////////////////////////////////////////

    /**
    * 链接状态
    */
    struct ConnStatus
    {
        string ip;
        int32_t uid;
        uint16_t port;
        int timeout;
        int iLastRefreshTime;
        size_t recvBufferSize;
        size_t sendBufferSize;
    };


    ////////////////////////////////////////////////////////////////////////////
    // 服务连接管理
    /**
    *  建立连接的socket信息
    */
    class Connection
    {
    public:
        /**
         * 构造函数
         * @param lfd
         * @param s
         * @param ip
         * @param port
         */
        Connection(const shared_ptr<ConnectionList> &connlist, BindAdapter *pBindAdapter,
                   int fd,
                   const string & ip,
                   uint16_t port,
                   detail::LogInterface *logger);

        /**
         * udp连接
         * @param fd
         */
        Connection(const shared_ptr<ConnectionList> &connlist, BindAdapter *pBindAdapter, int fd, detail::LogInterface *logger);

        /**
         * 析构函数
         */
        virtual ~Connection();

        /**
         * 初始化
         * @param id, 连接的唯一id
         */
        void initialize(TC_Epoller *epoller, unsigned int uid, NetThread *netThread);

        /**
         * 获取关联的网络线程
         * @return
         */
        inline NetThread *getNetThread() { return _netThread; }

        /**
         * 链接所属的adapter
         */
        inline BindAdapter* getBindAdapter() { return _pBindAdapter; }

        /**
         * registry epoll event
         */ 
        void registerEvent(NetThread *netThread);

        /**
         * get epoll info
         * @return
         */
        inline const shared_ptr<TC_Epoller::EpollInfo> &getEpollInfo() { return _trans->getEpollInfo(); }

        /**
         * 获取连接超时时间
         *
         * @return int
         */
        inline int getTimeout() const { return this->_pBindAdapter->getEndpoint().getTimeout()/1000; }

        /**
         * 获取线程的惟一id
         *
         * @return unsigned int
         */
        inline uint32_t getId() const { return _uid; }

        /**
         * 当前连接fd
         *
         * @return int
         */
        inline int getfd() const { return _trans->fd(); }

        /**
         * 是否有效
         *
         * @return bool
         */
        inline bool isValid() const { return _trans->isValid(); }

        /**
         * 远程IP
         *
         * @return string
         */
        inline const string &getIp() const { return _ip; }

        /**
         * 远程端口
         *
         * @return uint16_t
         */
        inline uint16_t getPort() const { return _port; }

        /**
         * 设置首个数据包包头需要过滤的字节数
         */
        inline void setHeaderFilterLen(int iHeaderLen) { _iHeaderLen = iHeaderLen; }

        /**
         * 设置关闭,发送完当前数据就关闭连接
         */
        bool setClose();

        /**
         * 是否是空连接
         */
        inline bool isEmptyConn() const { return _bEmptyConn; }

        /**
         * 获取连接
         */ 
        inline TC_Transceiver* getTransceiver() { return _trans.get(); }

        /**
         * 接收数据buffer
         */
        inline TC_NetWorkBuffer & getRecvBuffer() { return _trans->getRecvBuffer(); }

        /**
         * 发送数据buffer
         */
        inline TC_NetWorkBuffer & getSendBuffer() { return _trans->getSendBuffer(); }

        /**
         * 发送buffer里面数据
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
        int sendBufferDirect(const char* buff, size_t length);

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
        int sendBufferDirect(const shared_ptr<TC_NetWorkBuffer::Buffer>& buff);

        /**
         * 添加发送buffer
         * @param buffer
         * @return int, -1:发送出错, 0:无数据, 1:发送完毕, 2:还有数据
         */
        int send(const shared_ptr<SendContext> & data);

        /**
         * 增加数据到队列中
         * @param vtRecvData
         */
        inline void insertRecvQueue(const shared_ptr<RecvContext> & recv) { _pBindAdapter->insertRecvQueue(recv); }

        /**
         * 对于udp方式的连接，分配指定大小的接收缓冲区
         *@param nSize
         */
        void setUdpRecvBuffer(size_t nSize);

        /**
         * 是否是tcp连接
         * @return
         */
        inline bool isTcp() const { return _trans->getEndpoint().isTcp(); }

        /**
         * 是否是udp连接
         * @return
         */
        inline bool isUdp() const { return _trans->getEndpoint().isUdp(); }

        /**
         * 关闭连接
         */ 
        void close();

    protected:
        /**
         * 关闭连接
         * @param fd
         */
        void onCloseCallback(TC_Transceiver *trans, TC_Transceiver::CloseReason reason, const string &err);

        void onRequestCallback(TC_Transceiver *trans);

        TC_NetWorkBuffer::PACKET_TYPE onParserCallback(TC_NetWorkBuffer& buff, TC_Transceiver *trans);

        std::shared_ptr<TC_OpenSSL> onOpensslCallback(TC_Transceiver* trans);
        void onCompleteNetworkCallback(TC_Transceiver* trans);

        bool handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);
        bool handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);
        bool handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

        /**
         * 检查发送流量(避免发送数据量特别大, 而网络很慢, 导致内存不断在扩大, 相当于内存泄露了!)
         * @param sendBuffer : 当前正在发送的buffer
         * @param nowLeftBufferSize, 当前剩余还没有发送的数据大小
         * @return <0, 被限制了, 0: 可以继续发送
         */
	    int checkFlow(TC_NetWorkBuffer& sendBuffer, size_t nowLeftBufferSize);
    public:

        /**
         * 最后刷新时间
         */
        time_t _iLastRefreshTime;

    protected:
        /**
         * 连接list
         */
        weak_ptr<ConnectionList> _connList;

        /**
         * log interface
         */
        detail::LogInterface *_logger = NULL;

        /**
         * 适配器
         */
        BindAdapter* _pBindAdapter = NULL;

        /*
        * 收发包处理
        */
        std::unique_ptr<TC_Transceiver>         _trans;
        
        /**
         * 连接的唯一编号
         */
        uint32_t _uid;

        /**
         * fd socket
         */
        int _fd;

        /**
         * ip
         */
        string _ip;

        /**
         * 端口
         */
        uint16_t _port;

        /**
         * 还未发送的数据队列
         */ 
        list<shared_ptr<SendContext>> _messages;

        /**
         * 接收到数据
         */ 
        deque<shared_ptr<RecvContext>> _recv;

        /**
         * message队列中消息内存大小
         */
        size_t _messageSize = 0;

        /**
         * 每5秒发送的数据
         */
        size_t _accumulateBufferSize = 0;

        /**
         * 检查时间
         */
        time_t _lastCheckTime = 0;

        /**
         * 发送的检查<已经发送数据, 剩余buffer大小>
         */
        vector<pair<size_t, size_t>> _checkSend;

        /**
         * 需要过滤的头部字节数
         */
        int _iHeaderLen;

        /**
         * 发送完当前数据就关闭连接
         */
        bool _bClose;

        /**
         * 即是否是空连接(一个完整包都没有收到过)
         */ 
        bool _bEmptyConn;

        /**
         * 线程的id
         */
        NetThread * _netThread = NULL;
    };

    ////////////////////////////////////////////////////////////////////////////
    /**
    * 带有时间链表的map
    */
    class ConnectionList
    {
    public:
        /**
         * 构造函数
         */
        ConnectionList(detail::LogInterface *logger);

        /**
         * 析够函数
         */
        ~ConnectionList()
        {
            assert(_vConn == NULL);
        }

        /**
         * 初始化大小
         * @param size
         */
        void init(uint32_t size, uint32_t iIndex = 0);

        /**
         * close所有链接
         */ 
        void close();

        /**
         * 关闭
         * @param bindFd
         */
		void closeConnections(weak_ptr<BindAdapter> bindAdapter);

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
         * 删除连接
         * @param cPtr
         * @param bEraseList
         * @param closeType
         */
        void delConnection(Connection *cPtr, bool bEraseList, EM_CLOSE_T closeType);

        /**
         * 设置empty conn timeout(ms), 注意精度只能到秒
         * @param timeout
         */
        inline void setEmptyConnTimeout(int timeout) { _emptyCheckTimeout = timeout; }

        /**
         * get empty connection timeout
         * @return
         */
        inline int getEmptyConnTimeout() { return _emptyCheckTimeout; }

        /**
         * 检查超时数据
         */
        void checkTimeout();

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
        Connection *get(uint32_t uid);

        /**
         * 删除连接
         * @param uid
         */
        void del(uint32_t uid);

        /**
         * 大小
         * @return size_t
         */
        inline size_t size() { return _total - _free_size; }

    protected:
        typedef pair<Connection *, multimap<time_t, uint32_t>::iterator> list_data;

        /**
         * 内部删除, 不加锁
         * @param uid
         */
        void delNoLock(uint32_t uid);

    protected:

        /**
         * empty 连接检查(s)
         */
        int _emptyCheckTimeout;

        /**
         * log interface
         */
        detail::LogInterface *_logger = NULL;

        /**
         * 锁
         */
        TC_ThreadMutex _mutex;

		/**
		 * 总计连接数
		 */
        uint32_t _total;

        /**
         * 空闲链表
         */
        list<uint32_t> _free;

        /**
         * 空闲链元素个数
         */
        size_t _free_size;

        /**
         * 链接
         */
        list_data *_vConn = NULL;

        /**
         * 超时链表
         */
        multimap<time_t, uint32_t> _tl;

        /**
         * 上次检查超时时间
         */
        time_t _lastTimeoutTime;

        /**
         * 链接ID的魔数
         */
        uint32_t _iConnectionMagic;

    };

    ////////////////////////////////////////////////////////////////////////////
    // 服务端口管理,监听socket信息
    class BindAdapter : public enable_shared_from_this<BindAdapter> // : public TC_HandleBase
    {
    public:
        /**
         * 缺省的一些定义
         */
        enum
        {
            DEFAULT_QUEUE_CAP = 10 * 1024,  /**流量*/
            MIN_QUEUE_TIMEOUT = 3 * 1000,   /**队列最小超时时间(ms)*/
            DEFAULT_MAX_CONN = 1024,       /**缺省最大连接数*/
            DEFAULT_QUEUE_TIMEOUT = 60 * 1000,  /**缺省的队列超时时间(ms)*/
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
         * 析够函数
         */
        ~BindAdapter();

        /**
         * 设置需要手工监听
         */
        void enableManualListen();

        /**
         * 是否手工监听端口
         * @return
         */
        inline bool isManualListen() const { return this->_manualListen; }

        /**
         * 手工绑定端口
         */
        void manualListen();

		/**
		 * 取消监听
		 */
		void cancelListen();

        /**
         *
         * @param info
         */
        inline void setEpollInfo(const shared_ptr<TC_Epoller::EpollInfo> &info) { _info = info; }

        /**
         * get epoll info
         * @return
         */
        inline shared_ptr<TC_Epoller::EpollInfo> getEpollInfo() { return _info; }

        /**
         * 获取adapter name
         * @return string
         */
        const string &getName() const { return _name; }

        /**
         * set index
         */ 
        void setNetThreads(const vector<NetThread*> &netThreads);

        /**
         * init udp
         */
        void initUdp(NetThread* netThread);

        /**
         * get index
         */ 
        inline const vector<NetThread*> & getNetThreads() const { return _netThreads;}

        /**
         * 获取queue capacity
         * @return int
         */
        inline int getQueueCapacity() const { return _iQueueCapacity; }

        /**
         * 设置queue capacity
         * @param n
         */
        inline void setQueueCapacity(int n) { _iQueueCapacity = n; }

        /**
         * 设置协议名称
         * @param name
         */
        void setProtocolName(const string & name);

        /**
         * 获取协议名称
         * @return const string&
         */
        const string & getProtocolName();

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
        inline int getQueueTimeout() const { return _iQueueTimeout; }

        /**
         * 获取ip
         * @return const string&
         */
        inline const TC_Endpoint & getEndpoint() const { return _ep; }

        /**
         * 监听socket
         * @return TC_Socket
         */
        inline TC_Socket & getSocket() { return _s; } 

        /**
         * 设置最大连接数
         * @param iMaxConns
         */
        inline void setMaxConns(int iMaxConns) { _iMaxConns = iMaxConns; }

        /**
         * 获取最大连接数
         * @return size_t
         */
        inline size_t getMaxConns() const { return _iMaxConns;}

        /**
         * 设置HeartBeat时间
         * @param n
         */
        inline void setHeartBeatTime(time_t t) { _iHeartBeatTime = t; }

        /**
         * 获取HeartBeat时间
         * @return size_t
         */
        inline time_t getHeartBeatTime() const { return _iHeartBeatTime; }

        /**
         * 设置allow deny次序
         * @param eOrder
         */
        inline void setOrder(EOrder eOrder) {  _eOrder = eOrder; }

        /**
         * 设置允许ip
         * @param vtAllow
         */
        inline void setAllow(const vector<string> & vtAllow) { _vtAllow = vtAllow; }

        /**
         * 设置禁止ip
         * @param vtDeny
         */
        inline void setDeny(const vector<string> & vtDeny) { _vtDeny = vtDeny; }

        /**
         * 获取允许ip
         * @return vector<string>: ip列表
         */
        inline const vector<string> & getAllow() const { return _vtAllow; }

        /**
        * 获取禁止ip
        * @return vector<string>: ip列表
        */
        inline const vector<string> & getDeny() const { return _vtDeny; }

        /**
        * 获取allow deny次序
        * @return EOrder
        */
        inline EOrder getOrder() const { return _eOrder; }

        /**
         * 是否Ip被允许
         * @param ip
         * @return bool
         */
        bool isIpAllow(const string & ip) const;

        /**
         * 是否超过了最大连接数
         * @return bool
         */
        inline bool isLimitMaxConnection() const { return (_iCurConns + 1 > (int)_iMaxConns) || (_iCurConns + 1 > (int)((uint32_t)1 << 22) - 1); }

        /**
         * 减少当前连接数
         */
        inline void decreaseNowConnection() { --_iCurConns; }

        /**
         * 增加当前连接数
         */
        inline void increaseNowConnection() { ++_iCurConns; }

        /**
         * 获取所有链接状态
         * @return ConnStatus
         */
        inline vector<ConnStatus> getConnStatus() { return _epollServer->getConnStatus(_s.getfd()); }

        /**
         * 获取当前连接数
         * @return int
         */
        inline int getNowConnection() const { return _iCurConns; }

        /**
         * 获取服务
         * @return TC_EpollServer*
         */
        inline TC_EpollServer *getEpollServer() const { return _epollServer; }

        /**
         * 获取对应的网络线程
         * @param fd
         * @return
         */
        inline NetThread *getNetThread(size_t threadIndex) const { assert(threadIndex < _netThreads.size()); return _netThreads[threadIndex]; }

        /**
         * 注册协议解析器
         * @param pp
         */
        void setProtocol(const TC_NetWorkBuffer::protocol_functor & pf, int iHeaderLen = 0, const header_filter_functor & hf = echo_header_filter);

        /**
         * 获取协议解析器
         * @return protocol_functor&
         */
        inline TC_NetWorkBuffer::protocol_functor & getProtocol() { return _pf; }

        /**
         * 解析包头处理对象
         * @return protocol_functor&
         */
        inline header_filter_functor & getHeaderFilterFunctor() { return _hf; }

        /**
         * 获取数据buffer
         * @return
         */
        inline const shared_ptr<DataBuffer> &getDataBuffer() const { return _dataBuffer; }

        /**
         * 增加数据到队列中
         * @param recv
         * @param force 强制必须插入(无论是否过载, 比如close事件)
         */
        void insertRecvQueue(const shared_ptr<RecvContext> & recv, bool force = false);
        void insertRecvQueue(const deque<shared_ptr<RecvContext>> & recv);

        /**
         * 接收队列的大小
         * @return size_t
         */
        inline size_t getRecvBufferSize() const { return _dataBuffer->getRecvBufferSize(); }
 
        /**
         * 发送队列的大小
         * @return size_t
         */
        inline size_t getSendBufferSize() const { return _iSendBufferSize; }

        /**
         * add send buffer size(个数)
         */
        inline void increaseSendBufferSize() { ++_iSendBufferSize; }

        /**
         * increase send buffer size
         */
        inline void decreaseSendBufferSize(size_t s = 1) { _iSendBufferSize.fetch_sub(s); }

        /**
         * 默认的协议解析类, 直接echo
         * @param r
         * @param o
         * @return int
         */
        static TC_NetWorkBuffer::PACKET_TYPE echo_protocol(TC_NetWorkBuffer & r, vector<char> & o);

        /**
         * 默认的包头处理
         * @param i
         * @param o
         * @return int
         */
        static TC_NetWorkBuffer::PACKET_TYPE echo_header_filter(TC_NetWorkBuffer::PACKET_TYPE i, vector<char> & o);

        /**
         * 获取需要过滤的包头长度
         */
        inline int getHeaderFilterLen() { return _iHeaderLen; }

        /**
         * 所属handle组的handle数(每个handle一个对象)
         * @return size_t
         */
        inline int getHandleNum() { return _iHandleNum; }

        /**
         * 获取第几个句柄
         * @param index
         * @return
         */
        HandlePtr getHandle(size_t index)
        {
            assert(index <= _iHandleNum);
    //                assert(getEpollServer()->isMergeHandleNetThread());
            return _handles[index];
        }

        /*
         * 设置服务端积压缓存的大小限制(超过大小启用)
         */
        inline void setBackPacketBuffLimit(size_t iLimitSize) { _iBackPacketBuffLimit = iLimitSize; }

        /**
         * 获取服务端回包缓存的大小限制(超过大小启用)
         */
        inline size_t getBackPacketBuffLimit() const { return _iBackPacketBuffLimit; }

        /*
         * 设置服务端每5s最低发送字节
         */
        inline void setBackPacketBuffMin(size_t iMinLimit) { _iBackPacketBuffMin = iMinLimit; }

        /**
         * 获取服务端5/s最低发送字节
         */
        inline size_t getBackPacketBuffMin() const { return _iBackPacketBuffMin; }

        /**
         * 获取handles
         */
        inline vector<HandlePtr> & getHandles() { return _handles; }

        /**
         * 是否是队列模式(默认是False的)
         */
        inline bool isQueueMode() const { return _dataBuffer->isQueueMode(); }

        /**
         * 开启队列模式(同一个连接的请求, 落在同一个handle处理线程中)
         */
        inline void enableQueueMode() { return _dataBuffer->enableQueueMode(); }

//        /**
//         * 设置close回调函数
//         */
//        inline void setOnClose(const close_functor & f) { _closeFunc = f; }

        /**
         * 设置accesskey & secretKey & callback
         * @param ak
         * @param sk
         */
        inline void setAkSkCallback(const std::string & accesskey, const std::string & secretKey, const TC_Transceiver::onserververifyauth_callback &onverify)
        {
            _accessKey = accesskey;
            _secretKey = secretKey;
            _onVerifyCallback = onverify;
        }

        /**
         * 检查是否key是否匹配
         * @param accesskey
         * @param secretKey
         * @return
         */
        inline bool checkAkSk(const std::string & accesskey, const std::string & secretKey)
        {
            return accesskey == _accessKey && secretKey == _secretKey;
        }

        /**
         * 根据accesskey 获取secretkey, 如果accesskey不等, 则返回空
         * @param ak
         * @return
         */
        inline std::string getSk(const std::string & accesskey) const
        {
            return (_accessKey == accesskey) ? _secretKey : "";
        }

        /**
         * set openssl ctx
         * @param ctx
         */
        inline void setSSLCtx(const shared_ptr<TC_OpenSSL::CTX> &ctx) { _ctx = ctx; }

        /**
         * get ssl ctx
         * @return
         */
	    shared_ptr<TC_OpenSSL::CTX> getSSLCtx() { return _ctx; };

        /**
         * 构造函数
         */
        BindAdapter(TC_EpollServer *epollServer);

        /**
         * 设置adapter name
         * @param name
         */
        void setName(const string & name) { _name = name; }

        /**
         * 设置endpoint
         * @param str
         */
        inline void setEndpoint(const string & str) { _ep.parse(str); }

        /**
         * 是否是UDP端口
         */
        inline bool isUdp() const { return _ep.isUdp(); }

        /**
         * 初始化处理线程,线程将会启动
         */
        template<typename T, typename ...Args>
        void setHandle(size_t n, Args&&... args)
        {
            if (!_handles.empty()) {
                getEpollServer()->error("[BindAdapter::setHandle] handle is not empty!");
                return;
            }

            _iHandleNum = n;

            _dataBuffer.reset(new DataBuffer(_iHandleNum));

            for (size_t i = 0; i < _iHandleNum; ++i)
            {
                HandlePtr handle = std::make_shared<T>(args...);

                handle->setHandleIndex(i);

                handle->setEpollServer(this->getEpollServer());

                handle->setBindAdapter(this);

                _handles.push_back(handle);
            }
        }

	protected:
    	/**
    	 * 绑定服务器
    	 */
		void bind();

        friend class TC_EpollServer;
    public:

        //统计上报的对象
        PropertyReport *_pReportQueue = NULL;
        PropertyReport *_pReportConRate = NULL;
        PropertyReport *_pReportTimeoutNum = NULL;

    protected:
        /**
         * epoller指针
         */
        TC_EpollServer*            _epollServer = NULL;

        /**
         * 加锁
         */
        mutable std::mutex      _mutex;

        /**
         * Adapter所用的HandleGroup
         */
        vector<HandlePtr>       _handles;

        /**
         * 协议解析
         */
        TC_NetWorkBuffer::protocol_functor _pf;

        /**
         * 首个数据包包头过滤
         */
        header_filter_functor   _hf;

        /**
         * adapter的名字
         */
        string                  _name;

        /**
         * net threads
         */ 
        vector<NetThread*>      _netThreads;

        /**
         * 监听fd
         */
        TC_Socket               _s;

        /**
         * 绑定的IP
         */
        TC_Endpoint             _ep;

        /**
         * epoll info
         */
        shared_ptr<TC_Epoller::EpollInfo>  _info;

        /**
         * 最大连接数
         */
        int                     _iMaxConns;

        /**
         * 当前连接数
         */
        std::atomic<int>        _iCurConns;

        /**
         * Handle个数
         */
        size_t                  _iHandleNum;

        /**
         * 允许的Order
         */
        EOrder         			_eOrder;

        /**
         * 允许的ip
         */
        vector<string>          _vtAllow;

        /**
         * 禁止的ip
         */
        vector<string>          _vtDeny;

        /**
         * 发送队列数据总个数
         */
        atomic<size_t>          _iSendBufferSize {0};

        /**
         * 数据buffer
         */
        shared_ptr<DataBuffer>  _dataBuffer;

        /**
         * 队列最大容量
         */
        int                     _iQueueCapacity;

        /**
         * 消息超时时间（从入队列到出队列间隔)(毫秒）
         */
        int                     _iQueueTimeout;

        /**
         * 首个数据包包头长度
         */
        int                     _iHeaderLen;

        /**
         * 上次心跳发送时间
         */
        time_t         			_iHeartBeatTime;

        /**
         * 协议名称,缺省为"tars"
         */
        string                  _protocolName;

        /**
         * 回包缓存限制大小
         */
        size_t                  _iBackPacketBuffLimit = 1024*1024;

        /**
         * 回包速度最低限制(5/s), 默认1K
         */
        size_t                  _iBackPacketBuffMin = 1024;

        /**
         * verify auth callback
         */         
        TC_Transceiver::onserververifyauth_callback _onVerifyCallback;

        /**
         * 该obj的AK SK
         */
        std::string             _accessKey;
        std::string             _secretKey;

        /**
         * 是否手工监听
         */
        bool					_manualListen = false;
//        /**
//         * 创建一个udp句柄, 用来通知事后listen
//         */
//        TC_Socket               _udpNotify;
//
//        /**
//         * 通知
//         */
//        TC_Epoller::EpollInfo   *_notifyInfo = NULL;

        //连接关闭的回调函数
//        close_functor           _closeFunc;

        /**
        * ssl ctx
        */
        shared_ptr<TC_OpenSSL::CTX> _ctx;
    };

    ////////////////////////////////////////////////////////////////////////////
    class NetThread : public TC_Thread
    {
    public:
        /**
         * 构造函数
         */
        NetThread(int index, TC_EpollServer *epollServer);

        /**
         * 析构函数
         */
        virtual ~NetThread();

        /**
         * 获取网络线程的index
        * @return
        */
        inline int getIndex() const { return _threadIndex; }

        /**
         * connection list
         * @return
         */
        inline const shared_ptr<ConnectionList> &getConnectionList() const { return _list; }

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
        void createEpoll(uint32_t maxAllConn);

        /**
         * 已经启动, 进入网络调度状态
         * @return
         */
        bool isReady() const;

        /**
         * 获取Epoller对象
         * @return TC_Epoller*
         */
        inline TC_Epoller *getEpoller() { return _epoller; }

        /**
         * 设置初始化和处理对象
         * @param initialize
         * @param handle
         */
        void setInitializeHandle(std::function<void()> initialize, std::function<void()> handle);

        /**
         * 唤醒网络线程
         */
        inline void notify() { assert(_scheduler); _scheduler->notify(); }

        /**
         * 关闭连接
         * @param uid
         */
        void close(const shared_ptr<RecvContext> & data);

        /**
        * 发送数据
        * @param uid
        * @param s
        */
        void send(const shared_ptr<SendContext> & data);

        /**
         * 获取某一监听端口的连接数
         * @param lfd
         *
         * @return vector<ConnStatus>
         */
        inline vector<ConnStatus> getConnStatus(int lfd) { return _list->getConnStatus(lfd); }

        /**
         * 获取连接数
         *
         * @return size_t
         */
        inline size_t getConnectionCount() { return _list->size(); }

        /**
         *设置空连接超时时间
         */
        inline void setEmptyConnTimeout(int timeout) { 
            int emptyCheckTimeout = (timeout >= MIN_EMPTY_CONN_TIMEOUT) ? timeout : MIN_EMPTY_CONN_TIMEOUT;

            _list->setEmptyConnTimeout(emptyCheckTimeout); 
        }

        /**
         *设置udp的接收缓存区大小，单位是B,最小值为8192，最大值为DEFAULT_RECV_BUFFERSIZE
         */
        inline void setUdpRecvBufferSize(size_t nSize = DEFAULT_RECV_BUFFERSIZE) {
            _nUdpRecvBufferSize = (nSize >= 8192 && nSize <= DEFAULT_RECV_BUFFERSIZE) ? nSize : DEFAULT_RECV_BUFFERSIZE;
        }

    protected:

        /**
         * 获取连接
         * @param id
         *
         * @return ConnectionPtr
         */
        inline Connection *getConnectionPtr(uint32_t uid) { assert(_list); return  _list->get(uid); }

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
        void delConnection(Connection *cPtr, bool bEraseList = true, EM_CLOSE_T closeType = EM_CLIENT_CLOSE);

        /**
         * 处理管道消息
         */
        void processPipe();

        /**
         * 空连接超时时间
         */
        inline int getEmptyConnTimeout() const { return _list->getEmptyConnTimeout(); }

        /**
         *是否空连接检测
         */
        inline bool isEmptyConnCheck() const { return _list->getEmptyConnTimeout() > 0; }

        /**
         * 关联adapter
         * @param adapter
         */
        inline void addAdapter(BindAdapter* adapter) { _adapters.push_back(adapter); }

        /**
         * 通知关闭连接
         * @param fd
         */
		void notifyCloseConnectionList(const shared_ptr<BindAdapter> &adapter);

        friend class BindAdapter;
        friend class TC_EpollServer;
        friend class ConnectionList;

    private:

        /**
         * scheduler
         */
		shared_ptr<TC_CoroutineScheduler> _scheduler;

        /**
         * epoller
         */ 
        TC_Epoller*            _epoller = NULL;

        /**
         * net线程的id
         */
        size_t                  _threadId;

        /**
         * 线程索引
         */
        int                     _threadIndex;

		/**
		 * 服务
		 */
		TC_EpollServer *        _epollServer = NULL;

		/**
         * 关联的adapters
         */
        vector<BindAdapter*>    _adapters;

        /**
         * 管理的连接链表
         */
        shared_ptr<ConnectionList> _list;

        /**
         * 发送队列
         */
        send_queue              _sbuffer;

        // /**
        //  * 空连接超时时间,单位是毫秒,默认值2s,
        //  * 该时间必须小于等于adapter自身的超时时间
        //  */
        // int                     _iEmptyCheckTimeout;

        /**
         * udp连接时接收包缓存大小,针对所有udp接收缓存有效
         */
        size_t                  _nUdpRecvBufferSize;

        /**
         * 初始化对象
         */
        std::function<void()>   _initialize;

        /**
         * 处理对象
         */
        std::function<void()>   _handle;
    };

    /**
    * 具体处理基类
    */
    class Handle// :  public TC_HandleBase
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
         * 获取服务
         * @return TC_EpollServer*
         */
        inline TC_EpollServer *getEpollServer() const { return _epollServer; };

        /**
         * 获取adapter
         * @return
         */
        inline BindAdapter *getBindAdapter() const { return _bindAdapter; }

        /**
         * 获取Handle的索引(0~handle个数-1)
         * @return
         */
        inline uint32_t getHandleIndex() const { return _handleIndex; }

        /**
         * 设置网络线程
         */
        inline void setNetThread(NetThread *netThread) { _netThread = netThread; }

        /**
         * 获取网络线程
         * @return
         */
        inline NetThread *getNetThread() { return _netThread; }

        /**
         * 结束
         */
        void terminate();

        /**
         * 设置数据队列
         * @param data
         */
        inline void setDataBuffer(const shared_ptr<DataBuffer> &data) { _dataBuffer =  data; }

        /**
         * 协程处理一次
         */
        void handleOnceCoroutine();

        /**
         * 线程处理一次
         */
        void handleOnceThread();

        /**
         * 协程循环处理方法
         */
        void handleLoopCoroutine();

        /**
         * 线程循环处理方法
         */
        void handleLoopThread();

        /**
         * handle是否启动准备好(协程进入调度状态)
         * @return
         */
        bool isReady() const;
    public:
        /**
         * 发送数据
         * @param stRecvData
         * @param sSendBuffer
         */
        inline void sendResponse(const shared_ptr<SendContext> & data) { _epollServer->send(data); }

        /**
         * 关闭链接(tcp连接才有效)
         * @param stRecvData
         */
        inline void close(const shared_ptr<RecvContext> & data) { _epollServer->close(data);}

        /**
         * 对象初始化
         */
        virtual void initialize() { };

        /**
         * 唤醒handle对应的处理线程
         */
        virtual void notifyFilter();

        /**
        * 心跳(每处理完一个请求或者等待请求超时都会调用一次)
        */
        virtual void heartbeat() { }

        /**
         * 协程处理
         */
        void handleCoroutine();

        /**
         * 设置等待队列的时间
         * @param iWaitTime
         */
        void setWaitTime(uint32_t iWaitTime);
    protected:
        /**
         * 处理函数
         * @param stRecvData: 接收到的数据
         */
        virtual void handle(const shared_ptr<RecvContext> & data) = 0;

        /**
         * 处理超时数据, 即数据在队列中的时间已经超过
         * 默认直接关闭连接
         * @param stRecvData: 接收到的数据
         */
        virtual void handleTimeout(const shared_ptr<RecvContext> & data);

        /**
         * 处理连接关闭通知，包括
         * 1.close by peer
         * 2.recv/send fail
         * 3.close by timeout or overload
         * @param stRecvData:
         */
        virtual void handleClose(const shared_ptr<RecvContext> & data);

        /**
         * 处理overload数据 即数据队列中长度已经超过允许值
         * 默认直接关闭连接
         * @param stRecvData: 接收到的数据
         */
        virtual void handleOverload(const shared_ptr<RecvContext> & data);

        /**
         * 处理异步回调队列
         */
        virtual void handleAsyncResponse() { }

        /**
         * handleFilter拆分的第二部分，处理用户自有数据
         * 非游戏逻辑可忽略bExpectIdle参数
         */
        virtual void handleCustomMessage(bool bExpectIdle = false) { }

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
         * 设置服务
         * @param pEpollServer
         */
        inline void setEpollServer(TC_EpollServer *pEpollServer) { _epollServer = pEpollServer; }

        /**
         * 设置Adapter
         * @param pEpollServer
         */
        inline void setBindAdapter(BindAdapter *bindAdapter) { _bindAdapter = bindAdapter; }

        /**
         * 设置index
         * @param index
         */
        inline void setHandleIndex(uint32_t index) { _handleIndex = index; }

        /**
         * 友元类
         */
        friend class BindAdapter;
        friend class TC_EpollServer;
    protected:
        /**
         * 服务
         */
        TC_EpollServer *        _epollServer;

        /**
         * handle对应的网路线程(网络线程和handle线程合并的情况下有效)
         */
        NetThread *             _netThread = NULL;

        /**
         * 所属handle组
         */
        BindAdapter *           _bindAdapter;

        /**
         * 数据队列
         */
        shared_ptr<DataBuffer>  _dataBuffer;

        /**
         * Handle的索引
         */
        uint32_t                _handleIndex;

//        /**
//         * 结束
//         */
//        bool                    _terminate =  false;

        /**
         * 协程模式下有效
         */
		shared_ptr<TC_CoroutineScheduler> _scheduler;
    };

    /**
     * 几种服务模式
     */
    enum SERVER_OPEN_COROUTINE
    {
	    //独立网路线程 + 独立handle线程: 网络线程负责收发包, 通过队列唤醒handle线程中处理
        NET_THREAD_QUEUE_HANDLES_THREAD   = 0,
	    //独立网路线程组 + 独立handle线程: 网络线程负责收发包, 通过队列唤醒handle线程中处理, handle线程中启动协程处理
        NET_THREAD_QUEUE_HANDLES_CO       = 1,
        //合并网路线程 + handle线程(线程个数以处理线程配置为准, 网络线程配置无效): 连接分配到不同线程中处理(如果是UDP, 则网络线程竞争接收包), 这种模式下延时最小, 相当于每个包的收发以及业务处理都在一个线程中
        NET_THREAD_MERGE_HANDLES_THREAD        = 2,
	    //合并网路线程 + handle线程(线程个数以处理线程配置为准, 网络线程配置无效): 连接分配到不同线程中处理(如果是UDP, 则网络线程竞争接收包), 每个包会启动协程来处理
        NET_THREAD_MERGE_HANDLES_CO            = 3
    };

    /**
     * 构造函数
     */
    TC_EpollServer(unsigned int iNetThreadNum=1);

    /**
     * 析构函数
     */
    virtual ~TC_EpollServer();

    /**
     *设置空连接超时时间(timeout>0: 启动, timeout<=0: 不做连接超时控制)
     */
    void setEmptyConnTimeout(int timeout);

    /**
     * 设置本地日志
     * @param plocalLogger
     */
    inline void setLocalLogger(RollWrapperInterface *pLocalLogger) { _pLocalLogger = pLocalLogger; }

    /**
     * 获取server type
     * @return
     */
    inline SERVER_OPEN_COROUTINE getOpenCoroutine() const { return _openCoroutine; }

    /**
     * set server type
     * @param serverType
     */
    void setOpenCoroutine(SERVER_OPEN_COROUTINE serverPatten);

    /**
     * 设置单线程中协程使用的堆栈大小
     * 注意单线程协程消耗的内存: iPoolSize * iStackSize
     * @param iPoolSize:  协程池大小(最多多少个协程) (默认服务内置的是1w个)
     * @param iStackSize: 每个协程的堆栈大小(比如64k: 64*1024)
     */
	void setCoroutineStack(uint32_t iPoolSize, size_t iStackSize);

	/**
	 * 获取协程池大小
	 * @return
	 */
	uint32_t getCoroutinePoolSize() const { return _iCoroutinePoolSize; }

	/**
	 * 获取协程堆栈对消
	 * @return
	 */
	size_t getCoroutineStackSize() const { return _iCoroutineStackSize; }

	/**
     * 设置线程数量(非merge的情况下有效)
     * @param threadNum
     */
    void setThreadNum(int threadNum)
    {
        _threadNum = threadNum;
        if (_threadNum <= 0) _threadNum = 1;
        if (_threadNum > 15) _threadNum = 15;
    }

    /**
     * 创建BindAdpater
     * @param name: adapter 名称
     * @param ep: 服务绑定地址
     * @param handleNum: handle处理线程个数
     * @param args: Handle构造函数需要的参数
     */
    template<typename T, typename ...Args>
    BindAdapterPtr createBindAdapter(const string &name, const string &ep, size_t handleNum, Args&&... args)
    {
        TC_EpollServer::BindAdapterPtr adapter = getBindAdapter(name);
        if(adapter)
        {
            throw TC_EpollServer_Exception("[TC_EpollServer::createBindAdapter] adapter '" + name +"' has exists.");
        }

		adapter = std::make_shared<TC_EpollServer::BindAdapter>(this);

		adapter->setName(name);
		adapter->setEndpoint(ep);
		adapter->setMaxConns(10240);
		adapter->setHandle<T>(handleNum, args...);

        return adapter;
    }

    /**
     * 绑定监听socket
     * @param ls
     */
    int bind(BindAdapterPtr &lsPtr);

    /**
     * 初始化handle对象
     */
    void initHandle();

    /**
     * 启动业务处理线程
     */
    void startHandle();

    /**
     * 生成epoll
     * Generate epoll
     */
    void createEpoll();

    /**
     * 等待处理线程/协程都启动成功
     */
    void waitForReady();

    /**
     * 运行
     */
    void waitForShutdown();

    /**
     * 停止服务
     */
    void terminate();

    /**
     * 终止
     */    
    inline bool isTerminate() const { return _epoller == NULL || _epoller->isTerminate(); }

    /**
     * 获取epoller地址
     * @return
     */
    inline TC_Epoller *getEpoller() { return _epoller; }

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
     * @return vector<ConnStatus>
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
    virtual void debug(const string &s) const;

    /**
     * INFO日志
     * INFO LOG
     * @param s
     */
    virtual void info(const string &s) const;

    /**
     * tars日志
     * @param s
     */
    virtual void tars(const string &s) const;

    /**
    * 记录错误日志
    * @param s
    */
    virtual void error(const string &s) const;

    /**
     * 获取网络线程的数目
     */
    inline unsigned int getNetThreadNum() const { return _netThreads.size(); }

    /**
     * 获取网络线程的指针集合
     */
    inline const vector<NetThread *> &getNetThread() const { return _netThreads; }

    /**
     * 停止线程
     */
    void stopThread();

    /**
     * 获取所有业务线程的数目
     */
    size_t getLogicThreadNum();

    //回调给应用服务
    typedef std::function<void(TC_EpollServer *)> application_callback_functor;

    //网络线程发送心跳的函数
    //Function for network threads to send heartbeats
    typedef std::function<void(const string &)> heartbeat_callback_functor;

    /**
     * 设置waitForShutdown线程回调的心跳
     * @param hf [description]
     */
    void setCallbackFunctor(const application_callback_functor &hf) { _hf = hf; }

	/**
	 * 设置退出前的回调
	 * @param hf [description]
	 */
	void setDestroyAppFunctor(const application_callback_functor &qf) { _qf = qf; }

	// 接收新的客户端链接时的回调
	typedef std::function<void (TC_EpollServer::Connection*)> accept_callback_functor;

	/*
	 * 设置接收链接的回调
	 */
	void setOnAccept(const accept_callback_functor& f) { _acceptFunc = f; }

    /**
     * 设置netthread网络线程发送心跳的函数
     * Function for setting netthreaded network threads to send heartbeats
     * @param hf [description]
     */
    void setHeartBeatFunctor(const heartbeat_callback_functor &heartFunc) { _heartFunc = heartFunc; }

    /**
     * 心跳函数
     * @return
     */
    heartbeat_callback_functor &getHeartBeatFunctor() { return _heartFunc; }

    friend class BindAdapter;

protected:

    /**
     * accept callback
     * @param data
     */
    bool acceptCallback(const shared_ptr<TC_Epoller::EpollInfo> &info, weak_ptr<BindAdapter> adapter);

    /**
     * listen callback
     * @param data
     */
    void listenCallback(weak_ptr<BindAdapter> adapterPtr);

    /**
     * 接收句柄
     * @param fd
     * @return
     */
    bool accept(int fd, int domain = AF_INET);

    /**
     * 通知线程ready了
     */
	void notifyThreadReady();

    static void applicationCallback(TC_EpollServer *epollServer);

private:
    /**
     * server 模式
     */
    SERVER_OPEN_COROUTINE _openCoroutine = NET_THREAD_QUEUE_HANDLES_THREAD;

//    /**
//     * 是否已经结束
//     */
//    bool                    _terminate = false;

    /**
     * 网络线程
     */
    std::vector<NetThread *> _netThreads;

    /**
     * epoll
     */
    TC_Epoller *_epoller = NULL;

    /*
     * 线程数量
     */
    int _threadNum = 1;

    /**
     * 已经准备ready的线程数
     */
    std::atomic<int> _readyThreadNum{0};

	/**
	 * 锁
	 */
	std::mutex             _readyMutex;

	/**
	 * 条件变量, 用来等待网络线程启动
	 */
	std::condition_variable _readyCond;

		/**
     * handle处理的线程池(merge模式下无效)
     */
    TC_ThreadPool _handlePool;

    /**
     * 本地循环日志
     */
    RollWrapperInterface *_pLocalLogger;

    /**
     *
     */
    vector<BindAdapterPtr> _bindAdapters;

    /**
     * 监听socket
     */
    unordered_map<int, BindAdapterPtr> _listeners;

    /**
     * 协程池大小
     */
	uint32_t _iCoroutinePoolSize = 10000;

	/**
	 * 堆栈大小
	 */
	size_t _iCoroutineStackSize = 64*1024;

    /**
     * 应用回调
     */
    application_callback_functor _hf;

    /**
     * 退出前的回调
     */
	application_callback_functor _qf;

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

