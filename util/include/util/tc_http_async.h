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

#ifndef __TC_HTTP_ASYNC_H_
#define __TC_HTTP_ASYNC_H_

#include <functional>
#include "util/tc_platform.h"
#include "util/tc_thread_pool.h"
#include "util/tc_network_buffer.h"
#include "util/tc_http.h"
#include "util/tc_autoptr.h"
#include "util/tc_socket.h"

namespace tars
{

/////////////////////////////////////////////////
/**
* @file tc_http_async.h
* @brief http异步调用类.
* @brief HTTP asynchronous call class.
*
* http同步调用使用TC_HttpRequest::doRequest就可以了
* 代码示例请参考example_http_async.cpp
* 说明:
*     1 背后会启动唯一的网络线程
*     2 目前只支持http短连接
*     3 RequestCallback回调里面, onSucc和onFailed是对应的, 每次异步请求, onSucc/onFailed其中之一会被唯一响应
* Synchronized HTTP calls using TC_HttpRequest:: doRequest is OK
* See example_for code examplesHttp_Async.cpp
* Explanation:
*     1 the only network thread will be launched behind it
*     2 Only short HTTP connections are currently supported
*     3 In the RequestCallback callback, onSucc and onFailed correspond. Each asynchronous request, one of onSucc/onFailed is uniquely responded to.
* @author ruanshudong@qq.com
*/
/////////////////////////////////////////////////

/**
 * @brief 异步线程处理类.
 * @brief Asynchronous Thread Processing Class
 */
class TC_HttpAsync : public TC_Thread, public TC_ThreadLock
{
public:
    /**
     * @brief 异步请求回调对象
     * @brief Asynchronous request callback object
     * onSucc, 收到成功回包时响应
     * onSucc, Response when a successful return is received
     * onFailed, 失败时响应
     * onFailed, Response on Failure
     * onSucc和onFailed是成对出现的, 且一次请求, 只会一个被响应, 且只响应一次
     * OnSucc and onFailed occur in pairs and only one request is responded to, and only once.
     * onFailed被调用时, 链接就会被关闭掉
     * The link is closed when onFailed is called, 
     */
    class RequestCallback : public TC_HandleBase
    {
    public:
        /**
        * 错误码
        * Error Code
        */
        enum FAILED_CODE
        {
            /*Network Error*/
            Failed_Net     = 0x01,      //网络出错
            /*Error Connecting to Server*/
            Failed_Connect = 0x02,      //连接服务器出错
            /*overtime*/
            Failed_Timeout = 0x03,      //超时
            /*Interrupt receiving data*/
            Failed_Interrupt = 0x04,    //中断接收数据
            /*The server actively closed the link*/
            Failed_Close    = 0x05,     //服务器主动关闭了链接
            /*Link Timeout*/
            Failed_ConnectTimeout = 0x06, //链接超时
        };

        /**
         * @brief 每次收到数据且http头收全了都会调用，
         * stHttpResponse的数据可能不是完全的http响应数据 ,只有部分body数据
         * @brief Called every time data is received and the HTTP header is fully received,
         * the stHttpResponse data may not be the complete HTTP response data, only part of the body data
         * @param stHttpResponse  收到的http数据
         * @param stHttpResponse  HTTP data received
         * @return                true:继续收取数据, false:不收取数据了(会触发onFailed, Failed_Interrupt)
         * @return                True: Continue collecting data, false: No data will be collected (will trigger onFailed, Failed_Interrupt)
         */
        virtual bool onContinue(TC_HttpResponse &stHttpResponse) { return true; }

        /**
         * @brief 完整的响应回来了.
         * @brief Full response back
         *
         * @param stHttpResponse  http响应包
         * @param stHttpResponse  HTTP response package
         */
        virtual void onSucc(TC_HttpResponse &stHttpResponse) = 0;

        /**
         * @brief 异常, 发生异常时, onClose也会被调用, 连接会被关闭掉
         * @brief Exception, when an exception occurs, onClose is also called and the connection is closed
         *
         * @param ret, 错误码
         * @param ret  error code
         * @param info 异常原因
         * @param info Exception Reason
         */
        virtual void onFailed(FAILED_CODE ret, const string &info) = 0;

        /**
         * @brief 连接被关闭
         * @brief Connection closed
         */
        virtual void onClose() {};
    };

    typedef TC_AutoPtr<RequestCallback> RequestCallbackPtr;

protected:
    /**
     * @brief 异步http请求(短连接)
     * @brief Asynchronous HTTP requests (short connections)
     */
    class AsyncRequest : public TC_HandleBase
    {
    public:
        /**
         * @brief 构造.
         * @brief Constructor
         *
         * @param stHttpRequest
         * @param callbackPtr
         */
        AsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, bool bUseProxy);

        /**
         * @brief 构造.
         * @brief Constructor
         *
         * @param stHttpRequest
         * @param callbackPtr
         * @param addr
         */
        AsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const string &addr);

        /**
         * @brief 析构
         * @brief Destructor
         */
        ~AsyncRequest();

        /**
         * @brief 获取句柄
         * @brief Get Handle
         *
         * @return int
         */
        int getfd() const { return _fd.getfd(); }

        /**
         * @brief 发起建立连接.
         * @brief Initiate Connection Establishment
         *
         */
        void doConnect();

        /**
         * @brief 获取系统错误提示
         * @brief Get System Error Tips
         * @return
         */
        string getError(const string &sDefault) const;

        /**
        * @brief 发生异常
        * @brief exception occurred
        */
        void doException(RequestCallback::FAILED_CODE ret, const string &e);

        /**
         * @brief 发送请求
         * @brief Send Request
         */
        void doRequest();

        /**
         * @brief 接收响应
         * @brief Receive Response
         */
        void doReceive();

        /**
         * @brief 关闭连接
         * @brief Close Connection
         */
        void doClose();

        /**
         * @brief 超时
         * @brief overtime
         */
        void timeout();

        /**
         * @brief 设置唯一ID.
         * @brief Set Unique ID.
         *
         * @param uniqId
         */
        void setUniqId(uint32_t uniqId)    { _iUniqId = uniqId;}

        /**
         * @brief 获取唯一ID.
         * @brief Get Unique ID.
         *
         * @return uint32_t
         */
        uint32_t getUniqId() const         { return _iUniqId; }

        /**
         * @brief 设置处理请求的http异步线程.
         * @brief Set HTTP asynchronous thread for processing requests.
         *
         * @param pHttpAsync ：异步线程处理对象
         * @param pHttpAsync : Asynchronous threading processes objects 
         */
        void setHttpAsync(TC_HttpAsync *pHttpAsync) { _pHttpAsync = pHttpAsync; }

        /**
         * @brief 设置发网络请求时绑定的ip地址.
         * @brief Set the IP address to bind when sending network requests.
         *
         * @param addr
         */
        void setBindAddr(const struct sockaddr* addr);

        /**
         * @brief 链接是否有效
         * @brief Is the link valid
         */
        bool isValid() const { return _fd.isValid(); }

        /**
         * @brief 是否链接上
         * @brief Is it linked
         * @return [description]
         */
        bool hasConnected() const { return _isConnected; }

        /**
         * @brief 设置链接状态
         * @brief Set Link State
         * @param isConnected [description]
         */
        void setConnected(bool isConnected) { _isConnected = isConnected; }

        /**
         * 处理网络事件
         * Handling network events
         * @param events [description]
         */
        void processNet(const epoll_event &ev);

        /**
         * 处理通知事件
         * Handle notification events
         */
        void processNotify();

    protected:
        /**
         * @brief 接收请求.
         * @brief Receive requests.
         *
         * @param buf
         * @param len
         * @param flag
         * @return int
         */
        int recv(void* buf, uint32_t len, uint32_t flag);

        /**
         * @brief 发送请求.
         * @brief the sent request
         *
         * @param buf 发送内容
         * @param buf the sent content
         * @param len 发送长度
         * @param len the sent length
         * @param flag
         * @return int
         */
        int send(const void* buf, uint32_t len, uint32_t flag);

    protected:
        TC_HttpAsync               *_pHttpAsync;
        TC_HttpResponse             _stHttpResp;
        TC_Socket                   _fd;
        string                      _sHost;
        uint32_t                    _iPort;
        uint32_t                    _iUniqId;
        TC_NetWorkBuffer            _sendBuffer;
	    TC_NetWorkBuffer            _recvBuffer;
        RequestCallbackPtr          _callbackPtr;
        bool                        _bindAddrSet;
        struct sockaddr             _bindAddr;
        bool                        _bUseProxy;
        bool                        _isConnected;
    };

    typedef TC_AutoPtr<AsyncRequest> AsyncRequestPtr;

public:

    typedef TC_TimeoutQueue<AsyncRequestPtr> http_queue_type;

    /**
     * @brief 构造函数
     * @brief Constructor
     */
    TC_HttpAsync();

    /**
     * @brief 析构函数
     * @brief Destructor
     */
    ~TC_HttpAsync();

    /**
     * @brief 异步发起请求.
     * @brief Asynchronous Initiation of Requests
     *
     * @param stHttpRequest
     * @param httpCallbackPtr
     * @param bUseProxy,是否使用代理方式连接
     * @param bUseProxy Whether to use proxy connection
     */
    void doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, bool bUseProxy = false);

    /**
     * @brief 异步发起请求.
     * @brief Asynchronous Initiation of Requests
     *
     * @param stHttpRequest
     * @param httpCallbackPtr
     * @param addr, 请求地址, ip:port
     * @param addr  Request Address, ip；port
     */
    void doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const string &addr);

    /**
     * @brief 设置proxy地址
     * @brief Set proxy address
     *
     */
    int setProxyAddr(const char* Host, uint16_t Port);

    /**
     * @brief 设置代理的地址.
     * @brief Set the address of the agent.
     *
     * 不通过域名解析发送,直接发送到代理服务器的ip地址
     * Send directly to the IP address of the proxy server without domain name resolution
     * @param sProxyAddr 格式 192.168.1.2:2345 或者 sslproxy.qq.com:2345
     * @param sProxyAddr format : 192.168.1.2:2345 or sslproxy.qq.com:2345
     */
    int setProxyAddr(const char* sProxyAddr);

    /**
     * @brief 设置绑定的地址.
     * @brief Set Binding Address.
     *
     * @param sProxyAddr 格式 192.168.1.2
     * @param sProxyAddr format: 192.168.1.2
     */
    int setBindAddr(const char* sBindAddr);

    /**
     * @brief 设置绑定的地址.
     * @brief Set Binding Address.
     *
     * @param addr 直接用 addr 赋值
     * @param addr Assigning values directly with addr
     */
    void setProxyAddr(const struct sockaddr* addr);

    /**
     * @brief 获取代理地址, 设置代理地址后才有效
     * @brief Get proxy address, set proxy address before valid
     * @return [description]
     */
    const struct sockaddr* getProxyAddr() const { return &_proxyAddr; }

    /**
     * @brief 启动异步处理.
     * @brief Start asynchronous processing
     *
     * 参数已经无效(网络层有且只有一个线程)
     * Parameters are no longer valid (network layer has one and only one thread)
     * @param num, 异步处理的线程数
     * @param num  Number of threads processed asynchronously
     */
    void start();

    /**
     * @brief 设置超时(所有请求都只能用一种超时时间).
     * @brief Set timeout (all requests can only use one timeout)
     *
     * @param timeout: 毫秒, 但是具体的超时精度只能在s左右
     * @param timeout: Milliseconds, but the exact timeout precision can only be around s
     */
    void setTimeout(int millsecond) { _data->setTimeout(millsecond); }

    /**
     * @brief 等待请求全部结束(等待毫秒精度在100ms左右).
     * @brief Wait for all requests to end (millisecond precision around 100ms).
     *
     * @param millsecond, 毫秒 -1表示永远等待
     * @param millsecond Milliseconds-1 means wait forever
     */
    void waitForAllDone(int millsecond = -1);

    /**
     * @brief 结束线程
     * @brief End Thread
     */
    void terminate();

protected:

    // typedef TC_Functor<void, TL::TLMaker<AsyncRequestPtr, int>::Result> async_process_type;
    typedef std::function<void(AsyncRequestPtr, int)> async_process_type;

    /**
     * @brief 超时处理.
     * @brief Timeout handler.
     *
     * @param ptr
     */
    static void timeout(AsyncRequestPtr& ptr);

    /**
     * @brief 确保线程
     * @brief Ensure Threads
     * @param _threadId [description]
     */
    void assertThreadId() { assert(_threadId == std::this_thread::get_id()); }

    /**
     * @brief 具体的网络处理逻辑
     * @brief Specific network processing logic
     */
    void run() ;

    /**
     * @brief 删除异步请求对象
     * @brief Delete Asynchronous Request Object
     */
    void erase(uint32_t uniqId);

    /**
     * @brief 监控链接
     * @brief Monitoring Links
     * @param fd     [description]
     * @param uniqId [description]
     * @param events [description]
     */
    void addConnection(int fd, uint32_t uniqId, uint32_t events);

    /**
     * @brief 删除链接
     * @brief Delete Link
     * @param fd     [description]
     * @param events [description]
     */
    void delConnection(int fd);

    friend class AsyncRequest;

protected:
    std::thread::id              _threadId;

    TC_ThreadPool               _tpool;

    http_queue_type             *_data;

    TC_Epoller                  _epoller;

    TC_Epoller::NotifyInfo      _notify;

	std::mutex                  _mutex;

	deque<uint64_t>             _events;

    bool                        _terminate;

    struct sockaddr             _proxyAddr;

    struct sockaddr             _bindAddr;

    bool                        _bindAddrSet;
};

}
#endif

