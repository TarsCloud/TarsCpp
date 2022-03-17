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
#include "util/tc_transceiver.h"
#include "util/tc_openssl.h"

namespace tars
{

/////////////////////////////////////////////////
/**
* @file tc_http_async.h
* @brief http异步调用类.
* @brief HTTP asynchronous call class.
*
* http同步调用使用TC_HttpRequest::doRequest就可以了
* 说明:
*     1 背后会启动唯一的网络线程
*     2 目前只支持http短连接
*     3 RequestCallback回调里面, onSucc和onFailed是对应的, 每次异步请求, onSucc/onFailed其中之一会被唯一响应
*     4 支持https
* Synchronized HTTP calls using TC_HttpRequest:: doRequest is OK
* See example_for code examplesHttp_Async.cpp
* Explanation:
*     1 the only network thread will be launched behind it
*     2 Only short HTTP connections are currently supported
*     3 In the RequestCallback callback, onSucc and onFailed correspond. Each asynchronous request, one of onSucc/onFailed is uniquely responded to.
*     4 support https
* @author ruanshudong@qq.com
*/
/////////////////////////////////////////////////

/**
* @brief socket异常类
*/
struct TC_HttpAsync_Exception : public TC_Exception
{
    TC_HttpAsync_Exception(const string &buffer) : TC_Exception(buffer) {};
    ~TC_HttpAsync_Exception() throw() {};
};

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
            Failed_Request  = 0x07,      //发送出错
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
        AsyncRequest() {}

        /**
         * @brief 构造.
         * @brief Constructor
         *
         * @param stHttpRequest
         * @param callbackPtr
         */
        ~AsyncRequest();

        /**
         * 初始化
         * @param stHttpRequest
         * @param callbackPtr
         * @param ep
         */
        void initialize(TC_Epoller *epoller, const TC_Endpoint &ep, TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr);

        /**
         * @brief 获取句柄
         * @brief Get Handle
         *
         * @return int
         */
        int getfd() const { return _trans->fd(); }

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
        void setBindAddr(const TC_Socket::addr_type &bindAddr);

        /**
         * @brief 链接是否有效
         * @brief Is the link valid
         */
        bool isValid() const { return _trans->isValid(); }

        /**
         * @brief 是否链接上
         * @brief Is it linked
         * @return [description]
         */
        bool hasConnected() const { return _trans->hasConnected(); }

        /**
         *
         * @return
         */
        TC_Transceiver *trans() { return _trans.get(); }

    protected:
        shared_ptr<TC_ProxyInfo> onCreateCallback(TC_Transceiver* trans);
        std::shared_ptr<TC_OpenSSL> onOpensslCallback(TC_Transceiver* trans);
        void onCloseCallback(TC_Transceiver* trans, TC_Transceiver::CloseReason reason, const string &err);
        void onConnectCallback(TC_Transceiver* trans);
        void onRequestCallback(TC_Transceiver* trans);
        TC_NetWorkBuffer::PACKET_TYPE onParserCallback(TC_NetWorkBuffer& buff, TC_Transceiver* trans);

        friend class TC_HttpAsync;

    protected:
        TC_HttpAsync               *_pHttpAsync;
        TC_HttpResponse             _stHttpResp;
        uint32_t                    _iUniqId;
        RequestCallbackPtr          _callbackPtr;
        unique_ptr<TC_Transceiver>  _trans;
        std::shared_ptr<TC_NetWorkBuffer::Buffer> _buff;
	    shared_ptr<TC_OpenSSL::CTX> _ctx;
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
     * 发送异步请求, 发现指定的TC_Endpoint地址
     * @param stHttpRequest
     * @param callbackPtr
     * @param ep
     */
    void doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const TC_Endpoint &ep);

    /**
     * @brief 异步发起请求(可以使用代理, 否则使用URL地址去发送)
     * @brief Asynchronous Initiation of Requests
     *
     * @param stHttpRequest
     * @param httpCallbackPtr
     * @param bUseProxy,是否使用代理方式连接
     * @param bUseProxy Whether to use proxy connection
     */
    void doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, bool bUseProxy = false);

    /**
     * @brief 异步发起请求, 发送到指定addr的地址.
     * @brief Asynchronous Initiation of Requests
     *
     * @param stHttpRequest
     * @param httpCallbackPtr
     * @param addr, 请求地址, ip:port
     * @param addr  Request Address, ip；port
     */
    void doAsyncRequest(TC_HttpRequest &stHttpRequest, RequestCallbackPtr &callbackPtr, const string &addr);

	/**
	 * 设置ctx
	 * @param ctx
	 */
	void setCtx(const shared_ptr<TC_OpenSSL::CTX> &ctx) { _ctx = ctx; }

	/**
	 * get ctx
	 * @return
	 */
	shared_ptr<TC_OpenSSL::CTX> getCtx() { return _ctx; }

    /**
     * set proxy addr
     * @param ep
     */
    void setProxyAddr(const TC_Endpoint &ep);

    /**
     * @brief 设置proxy地址
     * @brief Set proxy address
     *
     */
    void setProxyAddr(const char* Host, uint16_t Port);

    /**
     * @brief 设置代理的地址.
     * @brief Set the address of the agent.
     *
     * 不通过域名解析发送,直接发送到代理服务器的ip地址
     * Send directly to the IP address of the proxy server without domain name resolution
     * @param sProxyAddr 格式 192.168.1.2:2345 或者 sslproxy.qq.com:2345
     * @param sProxyAddr format : 192.168.1.2:2345 or sslproxy.qq.com:2345
     */
    void setProxyAddr(const char* sProxyAddr);

    /**
     * @brief 设置绑定的地址.
     * @brief Set Binding Address.
     *
     * @param sProxyAddr 格式 192.168.1.2
     * @param sProxyAddr format: 192.168.1.2
     */
    void setBindAddr(const char* sBindAddr);

//    /**
//     * @brief 设置绑定的地址.
//     * @brief Set Binding Address.
//     *
//     * @param addr 直接用 addr 赋值
//     * @param addr Assigning values directly with addr
//     */
//    void setProxyAddr(const struct sockaddr* addr);
//
//    /**
//     * @brief 获取代理地址, 设置代理地址后才有效
//     * @brief Get proxy address, set proxy address before valid
//     * @return [description]
//     */
//    const struct sockaddr* getProxyAddr() const { return &_proxyAddr; }

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

    void addFd(AsyncRequest* asyncRequest);

    bool handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

    bool handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

    bool handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> &data);

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

    friend class AsyncRequest;

protected:
    std::thread::id              _threadId;

    TC_ThreadPool               _tpool;

    http_queue_type             *_data;

    TC_Epoller                  _epoller;

	std::mutex                  _mutex;

	deque<uint64_t>             _events;

    deque<uint64_t>             _erases;

    unique_ptr<TC_Endpoint>     _proxyEp;

    TC_Socket::addr_type        _bindAddr;

	shared_ptr<TC_OpenSSL::CTX> _ctx;

};

}
#endif

