#pragma once

#include "util/tc_platform.h"
#include "util/tc_autoptr.h"
#include "util/tc_epoller.h"
#include "util/tc_timeout_queue.h"
#include "util/tc_singleton.h"
#include "util/tc_socket.h"
#include "util/tc_clientsocket.h"
#include "util/tc_network_buffer.h"
#include "util/tc_transceiver.h"
#include "util/tc_openssl.h"
#include <deque>
#include <map>
#include <mutex>
#include <unordered_set>
#include <thread>

namespace tars
{
///////////////////////////////////////////
/**
* @file tc_socket_async.h
* @brief  socket异步长连接封装类.
* 说明:
*         1 TC_SocketAsyncCore控制网络线程, 使用它来创建和释放TC_SocketAsync对象
*         2 默认情况下, 和后台服务连接断了, 队列中没有发送的数据就扔掉, 可以用TC_SocketAsync::setClearBuffer来控制
*         3 和后台服务的重连时间可以通过TC_SocketAsync::setRetryInterval来设置(全局, 不能做到每个后台服务都有单独的)
*         4 RequestCallback的回调都在唯一的网络线程处理, 需要自己考虑线程安全问题, 同时回调函数里面不要阻塞, 会影响性能
*         5 可以用TC_SocketAsync::pause/resume来暂停和恢复链接
*         6 调用TC_SocketAsyncPtr使用完以后, 最好通过TC_SocketAsyncCore::release()释放掉
*
* @author ruanshudong@qq.com
*/
/////////////////////////////////////////////////
///
/**
* @brief 线程异常
*/

class TC_SocketAsync;
class TC_SocketAsyncCore;

typedef shared_ptr<TC_SocketAsync> TC_SocketAsyncPtr;

class UTIL_DLL_API TC_SocketAsync : public std::mutex , public enable_shared_from_this<TC_SocketAsync>
{
public:
    /**
    * @brief 异步请求回调对象
    * onSucc: 成功回调
    * onFailed: 失败回调, 无论什么失败都会被调用掉, 比如onSendTimeout, 也会在onFailed被调用的
    * 注意, 一个请求
    */
    class RequestCallback 
    {
    public:
        /**
         * 析构函数
         */
        virtual ~RequestCallback() {}

        /**
        * @brief 完整的响应回来了.
        *
        * @param sBuffer 回包
        * @deprecated, 使用onSucc(const vector<char> &)代替
        */
        virtual void onSucc(const string& sBuffer) {};

        /**
         * @brief 完整的响应回来了.
         * 增加一个回调, 建议使用这个回调, 减少一次内存copy, 默认回调onSucc(string), 建立历史版本
         * @param buff
         */
        virtual void onSucc(const vector<char> &buff) { onSucc(string(buff.data(), buff.size())); }

        /**
        * @brief 异常, 发生异常时, onClose也会被响应, 从而关闭网络连接.
        *
        * @param ret, 错误码
        * @param ex 异常原因
        */
        virtual void onFailed(TC_Transceiver::CloseReason ret, const string &info){};

        typedef int FAILED_CODE;
        /**
	     * @deprecated, 请使用onFailed(TC_Transceiver::CloseReason ret, const string &info)代替
         *
         */
        virtual void onFailed(FAILED_CODE ret, const string &info){};

        /**
         * @brief 连接建立后的回调
         * 可以在这个回调里面发送第一个数据
         */
        virtual void onConnected() {};

        /**
        * @brief 连接被关闭
        */
        virtual void onClose() = 0;

        /**
         * @brief 连接之前, 可以用来绑定本地地址
         */
        virtual void onBeforeConnect() {}

        /**
         * 发送之前获取数据, 可以变更数据内容
         * @param reqBuffer
         */
        virtual void onBeforeRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> &reqBuffer) {};
    };

    typedef shared_ptr<RequestCallback> RequestCallbackPtr;

    /**
    * @构造函数
    * @TC_SocketAsync
    *
    * @return
    */
    TC_SocketAsync(const shared_ptr<TC_SocketAsyncCore> &core, const TC_Endpoint &ep, const RequestCallbackPtr &callbackPtr, const TC_NetWorkBuffer::protocol_functor &pf);

    /**
    * @brief 析构函数
    */
    ~TC_SocketAsync();

    /**
    * @brief 异步发送buffer
    * @sendRequest
    * @param    string & sBuffer
    * @param bool header, 是否把数据插入到队列头部, 默认数据都在尾部的!
    */
    void sendRequest(const string &sBuffer, bool header = false);

    /**
    * @brief 异步发送buffer(尽量使用这个函数, 减少一次内存copy)
    * @sendRequest
    * @param    string & sBuffer
    * @param bool header, 是否把数据插入到队列头部, 默认数据都在尾部的!
    */
    void sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> &buff, bool header = false);

    /**
     * @brief 主动发起链接
     * 如果不主动发起链接, 调用sendRequest的时候也会自动建立链接
     */
    void connect();

    /**
     * @brief 如果当前已经建立连接, 则中断当前连接, 重新发起连接, 并清除队列数据
     */
    void reConnect();

    /**
     * @brief 是否自动重连
     *
     * @return void
     */
    void setAutoReconnect(bool autoReconnect){_autoReconnect = autoReconnect;}

    /**
     * 是否自动重连
     * @return
     */
	bool isAutoReconnect() { return _autoReconnect; }

	/**
     * @brief 链接异常断开时是否清除掉已存在的buffer
     * true: buffer 清除后, 后续有请求来时重连后台服务
     * false: buffer 不清除, 队列有数据则发起重连, 但是重连时间有一定的保护, 默认100ms重连一次
     * @param bClear [description]
     */
    void setClearBuffer(bool bClear) { _bClear = bClear; }

    /**
     * @brief 链接异常断开时是否清除buffer
     * @return [description]
     */
    bool isClearBuffer() const { return _bClear; }

    /**
     * 设置最大的队列(默认长度是100000)
     * @param iMaxQueueSize [description]
     */
    void setMaxQueueSize(size_t iMaxQueueSize) {_iMaxQueueSize = iMaxQueueSize;}

    /**
     * @brief 返回最大队列大小
     * @return [description]
     */
    size_t getMaxQueueSize() const { return _iMaxQueueSize; }

    /**
     * @brief 获取服务器端地址
     * @return [description]
     */
    string getHost() const;

    /**
     * @brief 获取端口
     * @return [description]
     */
    uint32_t getPort() const;

    /**
     * @brief 暂停, 关闭链接, 清空发送队列, 不再重连(相当于当前对象失效)
     * 此时, 不能再收发数据了,
     *
     */
    void pause();

    /**
     * @brief, 启用连接, 可以接续发送数据
     */
    void resume();

    /**
     * 链接状态, 只能处理三种之一
     */
    enum SocketAsyncStatus
    {
        EM_NORMAL = 0x01,       //正常状态, 可以收发数据
        EM_PAUSE  = 0x02,       //暂停状态, 不能收发数据, 但是可以通过resume恢复到正常状态
    };

    /**
     * @brief, 查询状态
     * @return SocketAsyncStatus
     */
    SocketAsyncStatus status() const { return _status; }

    /**
     * @brief 获取回调
     * @return [description]
     */
    RequestCallbackPtr getCallback() const { return _callbackPtr; }

    /**
     * @brief 是否连接上后端服务
     * @return [description]
     */
    bool hasConnected() const { return _trans->hasConnected(); }

    /**
     * @brief 正在连接中
     */
    bool isConnecting() const { return _trans->isConnecting(); }

    /**
     * @brief 获取端口
     * @return [description]
     */
    const TC_Endpoint &getEndpoint() const { return _ep; }

    /**
     * 设置新的连接地址
     * @param ep
     */
    void setEndpoint(const TC_Endpoint &ep);

    /**
     * @brief 设置连接超时时间
     * @param millsecond [description]
     */
    void setConnectTimeout(int millsecond) { _trans->setConnTimeout(millsecond); }

    /**
     * @brief 获取连接超时时间
     * @return [description]
     */
    int getConnectTimeout() const { return _trans->getConnTimeout(); }

    /**
     * 获取句柄
     * @return [description]
     */
    SOCKET_TYPE getfd() const { return _trans->fd(); }

    /**
     * ssl context
     * @param cafile
     * @param certfile
     * @param keyfile
     * @param verifyClient
     * @param ciphers
     */
	void setCtx(const std::string& cafile, const std::string& certfile, const std::string& keyfile, bool verifyClient, const string &ciphers);

    /**
     * 设置udp接收buffer大小(只对udp有效)
     */
    void setUdpRecvBuffer(size_t nSize);

    /**
     * 设置udp发送buffer大小(只对udp有效)
     */
    void setUdpSendBuffer(size_t nSize);

    /**
     * get trans
     * @return
     */
    TC_Transceiver *trans() { return _trans.get(); }
    
protected:

    /**
     *
     * @param ep
     */
    void resetTrans(const TC_Endpoint & ep);
    
    /**
     * @brief 获取系统错误提示
     * @return
     */
    string getError(const char* sDefault) const;

    shared_ptr<TC_ProxyInfo> onCreateCallback(TC_Transceiver* trans);
    std::shared_ptr<TC_OpenSSL> onOpensslCallback(TC_Transceiver* trans);
    void onCloseCallback(TC_Transceiver* trans, TC_Transceiver::CloseReason reason, const string &err);
    void onConnectCallback(TC_Transceiver* trans);
    void onRequestCallback(TC_Transceiver* trans);
    TC_NetWorkBuffer::PACKET_TYPE onParserCallback(TC_NetWorkBuffer& buff, TC_Transceiver* trans);
    bool handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data);
    bool handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);
    bool handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

    /**
     * 处理通知(建立链接或者发送数据)
     */
    void processNotify();

    /**
     * 通知事件
     */
    void onNotify(int notifyEvent);

    /**
     * 设置ID
     * @param uniqId [description]
     */
    void setUniqId(uint32_t uniqId) { _uniqId = uniqId; }

    /**
     * 获取ID, 重连时构建的
     * @return [description]
     */
    uint32_t getUniqId() const { return _uniqId; }

    /**
     * 添加一个发送buffer
     * @param reqBuffer
     * @param header, 是否添加到头部
     *
     */
    void addSendReqBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer> &reqBuffer, bool header = false);

    /**
     * 获取队列大小
     * @return [description]
     */
    size_t getSendReqBufferSize() const;

    /**
     * 发送独立是否为空
     * @return [description]
     */
    bool emptySendReqBufferSize() const;

    /**
     * 清除掉发送队列
     */
    void clearSendReqBuffer();

    friend class TC_SocketAsyncCore;

protected:
    shared_ptr<TC_SocketAsyncCore> _core;

    mutable std::mutex			_mutex;

    shared_ptr<TC_Transceiver>  _trans;

    //注意, 不要用deque, 因为后面要删除迭代器, 可能失效
    list<pair<shared_ptr<TC_NetWorkBuffer::Buffer>, bool>>        _messages;

    TC_Endpoint                 _ep;

    RequestCallbackPtr          _callbackPtr;

    TC_NetWorkBuffer::protocol_functor _pf;
  
    bool                        _autoReconnect = true;

    bool                        _bClear;

    size_t                      _iMaxQueueSize;

    uint32_t                    _uniqId;

    SocketAsyncStatus           _status;

	shared_ptr<TC_OpenSSL::CTX> _ctx;
};

/**
 * 长连接网络类
 * 使用智能指针!
 */
class UTIL_DLL_API TC_SocketAsyncCore : public enable_shared_from_this<TC_SocketAsyncCore>
{
public:
    /**
     * 构造函数
     */
    TC_SocketAsyncCore();

    /**
     * 析构函数
     */
    ~TC_SocketAsyncCore();

public:

    /**
    * @brief 启动异步处理.
    *
    * @param num, 异步处理的线程数
    */
    void start();

    /**
     * @brief 结束线程
     */
    void terminate();

    /**
     * @brief 设置重连的时间间隔
     * @param millsecond [description]
     */
    void setRetryInterval(int64_t millsecond) { _retryInterval = millsecond; }

    /**
     * @brief 获取重连的时间间隔
     *
     * @return [description]
     */
    time_t getRetryInterval() { return _retryInterval; }

    /**
     * @brief 设置连接超时时间
     * @param millsecond [description]
     */
    void setConnectTimeout(int millsecond) { _connTimeout = millsecond; }

    /**
     * @brief 获取连接超时时间
     * @return [description]
     */
    int getConnectTimeout() const { return _connTimeout; }

    /**
     * @brief 释放连接, 如果TC_SocketAsyncPtr对象不使用了, 必须要释放, 否则会导致内存泄露
     * @return
     */
    void release(const TC_SocketAsyncPtr &ptr);

    /**
     * epoller
     * @return
     */
    TC_Epoller* getEpoller() { return &_epoller; }

    /**
     * @brief 创建异步Socket, 必须用智能指针来管理
     *
     * @param  ep          [description]
     * @param  callbackPtr [description]
     * @param  pf          [description]
     * @return             [description]
     */
    TC_SocketAsyncPtr createSocketAsync(const TC_Endpoint &ep, const TC_SocketAsync::RequestCallbackPtr &callbackPtr, const TC_NetWorkBuffer::protocol_functor &pf);

protected:
    enum NotifyEvent
    {
        NE_CONNECT   = 0x0001,    //发起连接
        NE_RECONNECT = 0x0002,    //通知重连
        NE_PAUSE     = 0x0003,    //暂停句柄
        NE_RESUME    = 0x0004,    //恢复句柄
    };

    /**
     * @brief 处理
     */
    void process(int notifyEvent, uint32_t uniqId);

    /**
     * @brief 具体的网络处理逻辑
     */
    void run();

    /////////////////////////////////////////////////////////////////////////////

    /**
     * 添加需要重建的链接
     * @param ptr [description]
     */
    void addRetry(const TC_SocketAsyncPtr &ptr);

    /**
     * 删除重连
     * @param uniqId [description]
     */
    void delRetry(uint32_t uniqId);

    /////////////////////////////////////////////////////////////////////////////
    /**
     * 发起通知
     * @param uniqId [description]
     */
    void notify(int notifyEvent, uint32_t uniqId);

    /**
     * @brief 添加链接
     * @param fd          连接句柄
     */
    void addConnection(const TC_SocketAsyncPtr &ptr);

    /**
     * 添加通知句柄
     * @param ptr [description]
     */
    void addNotify(const TC_SocketAsyncPtr &ptr);

    /**
     * 连接管理
     */
    class Connection 
    {
    public:
        /**
         * 构造函数
         */
        Connection() : _uniqId(0)
        {
        }

        /**
         * 析构函数
         */
        ~Connection()
        {
        }

        /**
         * 获取唯一ID, 理论上到42亿后, 再从0开始 可能会有问题!!!!
         * @return [description]
         */
        uint32_t getUniqId()
        {
            return ++_uniqId;
        }

        /**
         * 添加到重试连接队列中, 定时检测重连
         * @param ptr [description]
         */
        void addRetry(const TC_SocketAsyncPtr &ptr)
        {
        	std::lock_guard<std::mutex> lock(_mutex);

            _retry[ptr->getUniqId()] = ptr;
        }

        /**
         * 从重连队列中删除
         * @param uniqId [description]
         */
        void delRetry(uint32_t uniqId)
        {
        	std::lock_guard<std::mutex> lock(_mutex);

            _retry.erase(uniqId);
        }

        /**
         * 发起重试重连
         */
        void retryConnect()
        {
            unordered_map<uint32_t, TC_SocketAsyncPtr> retry;

            {
                //交换, 相当于重连数据清空
            	std::lock_guard<std::mutex> lock(_mutex);
                retry.swap(_retry);
            }

            auto it = retry.begin();
            while (it != retry.end())
            {
                if (it->second && it->second->isAutoReconnect())
				{
					it->second->onNotify(NE_CONNECT);
				}
                ++it;
            }
        }

        /**
         * 添加连接
         * @param  ptr [description]
         * @return     [description]
         */
        uint32_t add(const TC_SocketAsyncPtr &ptr)
        {
        	std::lock_guard<std::mutex> lock(_mutex);

            _list[ptr->getUniqId()] = ptr;

            return ptr->getUniqId();
        }

        /**
         * 删除连接
         * @param  uniqId [description]
         * @return        [description]
         */
        void del(uint32_t uniqId)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            _retry.erase(uniqId);

            auto it = _list.find(uniqId);
            if (it != _list.end())
            {
                auto findIt = *it;
                _list.erase(it);
            }
        }

        /**
         * 获取连接
         * @param  uniqId [description]
         * @param  erase  [description]
         * @return        [description]
         */
        TC_SocketAsyncPtr get(uint32_t uniqId)
        {
            TC_SocketAsyncPtr ptr = NULL;

            std::lock_guard<std::mutex> lock(_mutex);
            auto it = _list.find(uniqId);
            if (it != _list.end())
            {
                ptr = it->second;
            }

            return ptr;
        }

        void release(const TC_SocketAsyncPtr &ptr)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if(ptr)
            {
                _list.erase(ptr->getUniqId());
                _retry.erase(ptr->getUniqId());
            }
        }

    protected:
        std::atomic<uint32_t> _uniqId;

        std::mutex _mutex;

        unordered_map<uint32_t, TC_SocketAsyncPtr> _retry;

        unordered_map<uint32_t, TC_SocketAsyncPtr> _list;
    };

    typedef shared_ptr<Connection> ConnectionPtr;

    friend class TC_SocketAsync;

protected:
    ConnectionPtr               _conn;

    TC_Epoller                  _epoller;

    std::thread 				*_thread;

    bool                        _terminate;

    int                         _connTimeout = 5000;

	int64_t                     _retryInterval;

	std::mutex                  _mutex;

	std::recursive_mutex        _release_mutex;

	deque<uint64_t>             _events;

};

}

