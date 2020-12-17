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

#ifndef _TC_CLIENTSOCKET_H__
#define _TC_CLIENTSOCKET_H__

#include "util/tc_socket.h"
#include <sstream>
#include "util/tc_http.h"
#include "util/tc_epoller.h"

namespace tars
{
/////////////////////////////////////////////////
/**
 * @file  tc_clientsocket.h
 * @brief 客户端发包收包类.
 * @brief Client Contracting and Receiving Class
 */
/////////////////////////////////////////////////
/**
*  @brief 解析endpoint异常类
*  @brief Parsing endpoint Exception Class
*/
struct TC_EndpointParse_Exception : public TC_Exception
{
    TC_EndpointParse_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_EndpointParse_Exception() throw() {};
};

/**
 *  @brief 表示一个网络端口,支持以下格式:
 *  @brief Represents a network port which supports the following formats:
 *
 * 1:tcp -h 127.0.0.1 -p 2345 -t 10000
 *
 * 2:tcp -h /tmp/sock.sock -p 0 -t 10000
 *
 * 3:udp -h 127.0.0.1 -p 2345 -t 10000
 *
 * -p 0:表示本地套接字
 * -p 0:Represents a local socket
 * 
 * -q 0:表示qos的dscp值
 * -q 0:Represents the DSCP value of QoS
 * 
 * 此时-h表示的文件路径
 * At this time, the file path is represented by '-h'
 */
class TC_Endpoint
{
public:
	enum EType { UDP = 0, TCP = 1, SSL = 2 };
    /**
     *
     */
    TC_Endpoint();

    /**
     * @brief 构造函数
     * @brief Constructor
     * @param host
     * @param port
     * @param timeout, 超时时间, 毫秒
     * @param timeout, timeoput, unit : ms
     * @param type, SOCK_STREAM或SOCK_DGRAM
     * @param type, SOCK_STREAM or SOCK_DGRAM
     */
    TC_Endpoint(const string& host, int port, int timeout, EType type = TCP, int grid = 0, int qos = 0, int weight = -1, unsigned int weighttype = 0, int authType = 0)
    {
        init(host, port, timeout, type, grid, qos, weight, weighttype, authType);
    }

    /**
     * @brief 用字符串描述来构造
     * @brief Use string to describe the constructor
     * @param desc
     */
    TC_Endpoint(const string& desc)
    {
        parse(desc);
    }

    /**
     * @brief 拷贝构造
     * @brief Copy Constructor
     * @param l
     */
    TC_Endpoint(const TC_Endpoint& l)
    {
        _host   = l._host;
        _port   = l._port;
        _timeout= l._timeout;
        _type   = l._type;
        _grid   = l._grid;
        _qos    = l._qos;
        _weight = l._weight;
        _weighttype = l._weighttype;
        _authType = l._authType;
        _isIPv6 = l._isIPv6;
    }

    /**
     * @brief 赋值函数
     * @brief Assignment Function
     * @param l
     *
     * @return TC_Endpoint&
     */
    TC_Endpoint& operator = (const TC_Endpoint& l)
    {
        if(this != &l)
        {
            _host   = l._host;
            _port   = l._port;
            _timeout= l._timeout;
            _type   = l._type;
            _grid   = l._grid;
            _qos    = l._qos;
            _weight = l._weight;
            _weighttype = l._weighttype;
            _authType = l._authType;
            _isIPv6 = l._isIPv6;
        }

        return *this;
    }

    /**
     * ==
     * @param l
     *
     * @return bool
     */
    bool operator == (const TC_Endpoint& l) const
    {
        return (_host == l._host && _port == l._port && _timeout == l._timeout && _type == l._type &&
            _grid == l._grid && _qos == l._qos && _weight == l._weight && _weighttype == l._weighttype &&
            _authType == l._authType && _isIPv6 == l._isIPv6);
    }

    /**
     * @brief 设置ip
     * @brief Set IP
     * @param str
     */
    void setHost(const string& host)    { _host = host; _isIPv6 = TC_Socket::addressIsIPv6(_host); }

    /**
     * @brief 获取ip
     * @brief Get IP
     *
     * @return const string&
     */
    const string &getHost() const      { return _host; }

    /**
     * @brief 设置端口
     * @brief Set Port
     * @param port
     */
    void setPort(int port)              { _port = port; }

    /**
     * @brief 获取端口
     * @brief Get Port
     *
     * @return int
     */
    int getPort() const                 { return _port; }

    /**
     * @brief 设置超时时间
     * @brief Set Timeout
     * @param timeout
     */
    void setTimeout(int timeout)        { _timeout = timeout; }

    /**
     * @brief 获取超时时间
     * @brief Get Timeout
     *
     * @return int
     */
    int getTimeout() const              { return _timeout; }

    /**
     * @brief  是否是TCP, 否则则为UDP
     * @brief  Determine whether it uses TCP or UDP
     *
     * @return bool
     */
    int  isTcp() const                  { return _type == TCP || _type == SSL; }

    /**
     * @brief  是否是SSL
     * @brief  Whether it uses SSL
     *
     * @return int
     */
    int isSSL() const                  { return _type == SSL; }

    /**
     * @brief 设置为TCP或UDP
     * @brief Set to TCP or UDP
     * @param bTcp
     */
	int isUdp() const                  { return _type == UDP; }
//    void setTcp(bool bTcp)              { _type = bTcp; }

    /**
     * @brief 设置为TCP/UDP/SSL
     * @brief Set to TCP/UDP/SSL
     * @param type
     */
    void setType(EType type)             { _type = type; }
    /**
     * @brief 获取协议类型
     * @brief Get the protocol type
     */
    EType getType() const                { return _type; }
	
    /**
     * @brief 获取路由状态
     * @brief Get route status 
     * @param grid
     */
    int getGrid() const                 { return _grid; }

    /**
     * @brief 设置路由状态
     * @brief Set route status
     * @param grid
     */
    void setGrid(int grid)              { _grid = grid; }

    /**
     * @brief 获取路由状态
     * @brief Get route status
     * @param grid
     */
    int getQos() const                 { return _qos; }

    /**
     * @brief 设置路由状态
     * @brief Set route status
     * @param grid
     */
    void setQos(int qos)              { _qos = qos; }

    /**
     * @brief 获取节点的静态权重值
     * @brief Get static weight value of node
     */
    int getWeight() const                 { return _weight; }

    /**
     * @brief 设置节点的静态权重值
     * @brief Set static weight value of node
     * @param weight
     */
    void setWeight(int weight)              { _weight = weight; }

    /**
     * @brief 获取节点的权重使用方式
     * @brief Get the weight usage of nodes
     */
    unsigned int getWeightType() const                 { return _weighttype; }

    /**
     * @brief 设置节点的权重使用方式
     * @brief Set the weight usage of nods
     * @param weightway
     */
    void setWeightType(unsigned int weighttype)              { _weighttype = weighttype; }

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
    /**
     * @brief 是否是本地套接字
     * @brief Whether it is a local socket or not
     *
     * @return bool
     */
    bool isUnixLocal() const            { return _port == 0; }
#endif

    /**
     * @brief is ipv6 socket or not
     * @return true if is ipv6
     */
    bool isIPv6() const                 { return _isIPv6; }

	/**
     * @brief 获取认证类型
     * @brief Get authentication type
     */
    int getAuthType() const             { return _authType; }

	/**
     * @brief 设置认证类型
     * @brief Set authentication type
     */
    void setAuthType(int type)          { _authType = type; }

    /**
     * @brief 字符串描述
     * @brief the string description
     *
     * @return string
     */
    string toString() const
    {
        ostringstream os;
        if (_type == TCP)
            os << "tcp";
        else if (_type == UDP)
            os << "udp";
        else 
            os << "ssl";

        os << " -h " << _host << " -p " << _port;
        if(_timeout != 0) os << " -t " << _timeout;
        if (_grid != 0) os << " -g " << _grid;
        if (_qos != 0) os << " -q " << _qos;
        if (_weight != -1) os << " -w " << _weight;
        if (_weighttype != 0) os << " -v " << _weighttype;
		if (_authType != 0) os << " -e " << _authType;
        return os.str();
    }

    /**
     * @brief  字符串形式的端口
     * @brief  Port in string
     * tcp:SOCK_STREAM
     *
     * udp:SOCK_DGRAM
     *
     * -h: ip
     *
     * -p: 端口
     * -p: port
     *
     * -t: 超时时间, 毫秒
     * -t: timeout, unit:ms
     *
     * -p 和 -t可以省略, -t默认10s
     * -p and -t can be omitted, -t defaults to 10s
     *
     * tcp -h 127.0.0.1 -p 2345 -t 10000
     *
     * @param desc
     */
    void parse(const string &desc);

private:
    void init(const string& host, int port, int timeout, EType type, int grid, int qos, int weight, unsigned int weighttype, int authType);

protected:
    /**
     * ip
     */
    std::string _host;

    /**
     * 端口
     * Port
     */
    int         _port = 0;

    /**
     * 超时时间
     * Timeout
     */
    int         _timeout = 3000;

    /**
     * 类型
     * Type
     */
    EType       _type;

    /**
     * 路由状态
     * Route Status
     */
    int         _grid = 0;

    /**
     *  网络QoS的dscp值
     *  DSCP value of network QoS
     */
    int         _qos = 0;

    /**
     *  节点的静态权重值
     *  the static weight value of node
     */
    int        _weight = -1;

    /**
     *  节点的权重使用方式
     *  the weight usage of nodes
     */
    unsigned int  _weighttype = 0;
    /**
     *  鉴权类型
     *  Authentication Type
     */
    int         _authType = 0;

    /**
     * _host is ipv6 or not
     */
    bool        _isIPv6;
};

/*************************************TC_ClientSocket**************************************/

/**
* @brief 客户端socket相关操作基类
* @brief Client Socket Related Operation Base Class
*/
class TC_ClientSocket
{
public:

    /**
    *  @brief 构造函数
    *  @brief Constructor
     */
    TC_ClientSocket();

    /**
     * @brief 析够函数
     * @brief Desturctor
     */
    virtual ~TC_ClientSocket();

    /**
    * @brief 构造函数
    * @brief Constructor
    * @param sIP      服务器IP
    * @param sIP      Server IP
    * @param iPort    端口, port为0时:表示本地套接字此时ip为文件路径
    * @param iPort    Port, when port==0 : represent the local socket, and the IP is the file path this momnet
    * @param iTimeout 超时时间, 毫秒
    * @param iTimeout timeout, unit: ms
    */
    TC_ClientSocket(const string &sIp, int iPort, int iTimeout) { init(sIp, iPort, iTimeout); }

    /**
    * @brief 初始化函数
    * @brief Initialize Function
    * @param sIP      服务器IP
    * @param sIP      Server IP
    * @param iPort    端口, port为0时:表示本地套接字此时ip为文件路径
    * @param iPort    Port, when port==0 : represent the local socket, and the IP is the file path this momnet
    * @param iTimeout 超时时间, 毫秒
    * @param iTimeout timeout, unit: ms
    */
    void init(const string &sIp, int iPort, int iTimeout);

    /**
    * @brief 发送到服务器
    * @brief Send to the Server
    * @param sSendBuffer 发送buffer
    * @param sSendBuffer send buffer
    * @param iSendLen    发送buffer的长度
    * @param iSendLen    send the buffer length
    * @return            int 0 成功,<0 失败
    * @return            int: 0 success, <0 fail
    */
    virtual int send(const char *sSendBuffer, size_t iSendLen) = 0;

    /**
    * @brief 从服务器返回不超过iRecvLen的字节
    * @brief Return no more than iRecvLen bytes from the server
    * @param sRecvBuffer 接收buffer
    * @param sRecvBuffer Receive buffer
    * @param iRecvLen    指定接收多少个字符才返回,输出接收数据的长度
    * @param iRecvLen    Specify how many characters to receive before returning, and output the length of received data
    * @return            int 0 成功,<0 失败
    * @return            int: 0 success, <0 fail
    */
    virtual int recv(char *sRecvBuffer, size_t &iRecvLen) = 0;

    /**
     * 关闭连接
     * Close Connection
     */
    virtual void close();

    /**
     * 获取socket
     * Get socket
     * @return
     */
	TC_Socket *getSocket() { return &_socket; }

    /**
    * @brief  定义发送的错误
    * @brief  Define errors about sending
    */
    enum EM_CIENT_SOCKET_TYPE
    {
        /**EM_SUCCESS:Send Successfully**/
        EM_SUCCESS  = 0,          /** EM_SUCCESS:发送成功*/
        /**EM_SEND:Send Error**/
        EM_SEND     = -1,        /** EM_SEND:发送错误*/
        /**EM_SELECT:SELECT Error**/
        EM_SELECT   = -2,        /** EM_SELECT:select 错误*/
        /**EM_TIMEOUT:SELECT Timeout**/
        EM_TIMEOUT  = -3,        /** EM_TIMEOUT:select超时*/
        /**EM_RECV:RECEIVE Error**/
        EM_RECV     = -4,        /** EM_RECV: 接受错误*/
        /**EM_CLOSE:Server Proactive Shutdown **/
        EM_CLOSE    = -5,        /**EM_CLOSE: 服务器主动关闭*/
        /**EM_CONNECT:Server Connection Failed**/
        EM_CONNECT  = -6,        /** EM_CONNECT : 服务器连接失败*/
        /**EM_SOCKET : SOCKET Initialize Failed**/
        EM_SOCKET   = -7        /**EM_SOCKET : SOCKET初始化失败*/
    };

protected:
	TC_Epoller*     _epoller = NULL;

    /**
     * 套接字句柄
     * Socket Handle
     */
    TC_Socket       _socket;

    /**
     * ip或文件路径
     * IP or File Path 
     */
    string          _ip;

    /**
     * 端口或-1:表示是本地套接字
     * Port or - 1: indicates a local socket
     */
    int             _port;

    /**
     * 超时时间, 毫秒
     * Timeout, unit: ms
     */
    int             _timeout;

	/**
	 * 是否是IPv6
     * Whether it is IPV6 or not
	 */
	bool        _isIPv6 = false;
};

/**
 * @brief TCP客户端Socket
 * @brief TCP Client Socket
 * 多线程使用的时候，不用多线程同时send/recv，小心串包；
 * When using multithreading, do not send/recv at the same time, and be careful of mixing packets;
 */
class TC_TCPClient : public TC_ClientSocket
{
public:
    /**
    * @brief  构造函数
    * @brief  Constructor
     */
    TC_TCPClient(){}

    /**
    * @brief  构造函数
    * @brief  Constructor
    * @param sIp       服务器Ip
    * @param sIp       Server IP
    * @param iPort     端口
    * @param iPort     Port
    * @param iTimeout  超时时间, 毫秒
    * @param iTimeout  Timeout, unit: ms
    */
    TC_TCPClient(const string &sIp, int iPort, int iTimeout) : TC_ClientSocket(sIp, iPort, iTimeout)
    {
    }

    /**
    * @brief  发送到服务器
    * @brief  Send to the Server
    * @param sSendBuffer  发送buffer
    * @param sSendBuffer  Send buffer
    * @param iSendLen     发送buffer的长度
    * @param iSendLen     Send buffer length
    * @return             int 0 成功,<0 失败
    * @return             int: 0 success, <0 fail
    */
    virtual int send(const char *sSendBuffer, size_t iSendLen);

    /**
    * @brief  从服务器返回不超过iRecvLen的字节
    * @brief  return Bytes from the server not exceeding iRecvLen
    * @param sRecvBuffer 接收buffer
    * @param sRecvBuffer receive buffer
    * @param iRecvLen    指定接收多少个字符才返回,输出接收数据的长度
    * @param iRecvLen    Specify the number of characters to receive before returning, amd output the length of the received data
    * @return            int 0 成功,<0 失败
    * @return             int: 0 success, <0 fail
    */
    virtual int recv(char *sRecvBuffer, size_t &iRecvLen);

    /**
    *  @brief 从服务器直到结束符(注意必须是服务器返回的结束符,
    *         而不是中间的符号 ) 只能是同步调用
    * @brief From the server to the ending charactor, can only be a synchronous call 
    * (note that it must be the ending charactor returned by the server instead of a middle symbol) 
    * @param sRecvBuffer 接收buffer, 包含分隔符
    * @param sRecvBuffer receive buffer, including separators
    * @param sSep        分隔符
    * @param sSep        separator
    * @return            int 0 成功,<0 失败
    * @return             int: 0 success, <0 fail
    */
    int recvBySep(string &sRecvBuffer, const string &sSep);

    /**
     * @brief 接收到服务器关闭连接为止
     * @brief Keep receiving until the server connection is closed
     * @param recvBuffer
     *
     * @return int 0 成功,<0 失败
     * @return int: 0 success, <0 fail
     */
    int recvAll(string &sRecvBuffer);

    /**
     * @brief  从服务器返回iRecvLen的字节
     * @brief  Return the Bytes of iRecvLen from the server
     * @param sRecvBuffer, sRecvBuffer的buffer长度必须大于等于iRecvLen
     * @param sRecvBuffer, its buffer length must be greater than or equal to iRecvLen
     * @param iRecvLen
     * @return int 0 成功,<0 失败
     * @return int: 0 success, <0 fail
     */
    int recvLength(char *sRecvBuffer, size_t iRecvLen);

    /**
    * @brief  发送到服务器, 从服务器返回不超过iRecvLen的字节
    * @brief  Send to the server, return no more than iRecvLen bytes from the server
    * @param sSendBuffer  发送buffer
    * @param sSendBuffer  send buffer
    * @param iSendLen     发送buffer的长度
    * @param iSendLen     send buffer length
    * @param sRecvBuffer  接收buffer
    * @param sRecvBuffer  receive buffer
    * @param iRecvLen     接收buffer的长度指针[in/out],
    *                     输入时表示接收buffer的大小,返回时表示接收了多少个字符
    *  @param iRecvLen    receive the length pointer[in/out] of buffer,
    *                     When inputing, it indicates the size of the received buffer.
    *                     When returning, it indicates how many characters have been received
    * @return             int 0 成功,<0 失败
    * @return             int: 0 success, <0 fail
    */
    int sendRecv(const char* sSendBuffer, size_t iSendLen, char *sRecvBuffer, size_t &iRecvLen);

    /**
    * @brief  发送到服务器, 并等待服务器直到结尾字符, 包含结尾字符
    * sSep必须是服务器返回的结束符,而不是中间的符号，只能是同步调用
    * (一次接收一定长度的buffer,如果末尾是sSep则返回,
    * 否则继续等待接收)
    * @brief  Send to the server and wait for the server until the ending charactor. 
    * sSep must be the ending charactor returned by the server, instead of a middle symbol. It can only be called synchronously .
    * (Once a certain length of buffer is received, if the end is sSep, then returns, otherwise it will continue to wait for reception).
    *
    * @param sSendBuffer  发送buffer
    * @param sSendBuffer  send buffer
    * @param iSendLen     发送buffer的长度
    * @param iSendLen     send buffer length
    * @param sRecvBuffer  接收buffer
    * @param sRecvBuffer  receive buffer
    * @param sSep         结尾字符
    * @param sSep         ending charactor
    * @return             int 0 成功,<0 失败
    * @return             int: 0 success, <0 fail
    */
    int sendRecvBySep(const char* sSendBuffer, size_t iSendLen, string &sRecvBuffer, const string &sSep);

    /**
    * @brief  发送到服务器, 并等待服务器直到结尾字符(\r\n), 包含\r\n
    * 注意必须是服务器返回的结束符,而不是中间的符号
    * 只能是同步调用
    * (一次接收一定长度的buffer,如果末尾是\r\n则返回,否则继续等待接收)
    * @brief  Send to the server, and wait for the server until ending characters(\r\n), including \r\n
    * Attention that the ending character must be returned from the server, instead of a certain symbol in the middle.
    * It can only be called synchronously.
    *
    * @param sSendBuffer  发送buffer
    * @param sSendBuffer  send buffer
    * @param iSendLen     发送buffer的长度
    * @param iSendLen     send buffer length
    * @param sRecvBuffer  接收buffer
    * @param sRecvBuffer  receive buffer
    * @param sSep         结尾字符
    * @param sSep         ending charactor
    * @return             int 0 成功,<0 失败
    * @return             int: 0 success, <0 fail
    */
    int sendRecvLine(const char* sSendBuffer, size_t iSendLen, string &sRecvBuffer);

    /**
     * @brief  发送到服务器, 接收直到服务器关闭连接为止
     * 此时服务器关闭连接不作为错误
     * @brief  send to the server, keep receiving until the server connection is closed.
     * @param sSendBuffer
     * @param iSendLen
     * @param sRecvBuffer
     *
     * @return int
     */
    int sendRecvAll(const char* sSendBuffer, size_t iSendLen, string &sRecvBuffer);

protected:
    /**
     * @brief  获取socket
     * @brief  get socket
     *
     * @return int
     */
    int checkSocket();
};

/*************************************TC_TCPClient**************************************/
 /**
  * @brief  多线程使用的时候，不用多线程同时send/recv，小心串包
  * @brief  When using multithreading, do not send/recv with multithreading at the same time. Be careful about mixing packets.

  */
class TC_UDPClient : public TC_ClientSocket
{
public:
    /**
    * @brief  构造函数
    * @brief  Constructor
     */
    TC_UDPClient(){};

    /**
    * @brief  构造函数
    * @brief  Constructor
    * @param sIp       服务器IP
    * @param sIp       server IP
    * @param iPort     端口
    * @param iPort     Port
    * @param iTimeout  超时时间, 毫秒
    * @param iTimeout  Timeout, unit: ms
    */
    TC_UDPClient(const string &sIp, int iPort, int iTimeout) : TC_ClientSocket(sIp, iPort, iTimeout)
    {
    }

    /**
     * @brief  发送数据
     * @brief  Send Data
     * @param sSendBuffer 发送buffer
     * @param sSendBuffer send buffer
     * @param iSendLen    发送buffer的长度
     * @param iSendLen    send buffer length
     *
     * @return            int 0 成功,<0 失败
     * @return            int: 0 success, <0 fail
     */
    int send(const char *sSendBuffer, size_t iSendLen);

    /**
     * @brief  接收数据
     * @brief  receive data
     * @param sRecvBuffer  接收buffer
     * @param sRecvBuffer  receive buffer
     * @param iRecvLen     输入/输出字段
     * @param iRecvLen     input/output field 
     * @return             int 0 成功,<0 失败
     * @return             int: 0 success, <0 fail
     */
    int recv(char *sRecvBuffer, size_t &iRecvLen);

    /**
     * @brief  接收数据, 并返回远程的端口和ip
     * @brief  receive data, and return remote port and IP
     * @param sRecvBuffer 接收buffer
     * @param sRecvBuffer receive buffer
     * @param iRecvLen    输入/输出字段
     * @param iRecvLen    input/output field 
     * @param sRemoteIp   输出字段, 远程的ip
     * @param sRemoteIp   output field, remote IP
     * @param iRemotePort 输出字段, 远程端口
     * @param iRemotePort output field, remote port
     *
     * @return int 0 成功,<0 失败
     * @return int: 0 success, <0 fail
     */
    int recv(char *sRecvBuffer, size_t &iRecvLen, string &sRemoteIp, uint16_t &iRemotePort);

    /**
     * @brief  发送并接收数据
     * @brief  Send and Receive Data
     * @param sSendBuffer 发送buffer
     * @param sSendBuffer send buffer
     * @param iSendLen    发送buffer的长度
     * @param iSendLen    send buffer length
     * @param sRecvBuffer 输入/输出字段
     * @param sRecvBuffer input/output field
     * @param iRecvLen    输入/输出字段
     * @param iRecvLen    input/output field
     *
     * @return int 0 成功,<0 失败
     * @return int: 0 success, <0 fail
     */
    int sendRecv(const char *sSendBuffer, size_t iSendLen, char *sRecvBuffer, size_t &iRecvLen);

    /**
     * @brief  发送并接收数据, 同时获取远程的ip和端口
     * @brief  Send and receive data, and get the remote port and IP at the same time.
     * @param sSendBuffer  发送buffer
     * @param sSendBuffer  send buffer
     * @param iSendLen     发送buffer的长度
     * @param iSendLen     send buffer length
     * @param sRecvBuffer  输入/输出字段
     * @param sRecvBuffer  input/output field
     * @param iRecvLen     输入/输出字段
     * @param iRecvLen     input/output field
     * @param sRemoteIp    输出字段, 远程的ip
     * @param sRemoteIp    output field, remote IP
     * @param iRemotePort  输出字段, 远程端口
     * @param iRemotePort  output field, remote port
     *
     * @return int 0 成功,<0 失败
     * @return int: 0 success, <0 fail
     */
    int sendRecv(const char *sSendBuffer, size_t iSendLen, char *sRecvBuffer, size_t &iRecvLen, string &sRemoteIp, uint16_t &iRemotePort);

protected:
    /**
     * @brief  获取socket
     * @brief  Get Socket
     *
     * @return TC_Socket&
     */
    int checkSocket();
};

}

#endif
