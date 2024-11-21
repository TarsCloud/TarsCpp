//
// Created by jarod on 2023/8/22.
//

#pragma once

#include "util/tc_platform.h"

#include <string>
#include <iostream>
#include <memory>
#include <fcntl.h>
#include "util/tc_ex.h"
#include "util/tc_epoller.h"
#include "util/tc_network_buffer.h"
#include <mutex>


#if TARGET_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace tars
{

/**
 * 类说明
 * - 异步串口读写类, 方便的异步读写串口
 * - TC_SerialPortGroup: 管理一组串口, 背后有一个线程来完成串口的读写
 * - TC_SerialPort: 异步串口读写, 由TC_SerialPortGroup负责管理它的生命周期
 * 使用说明
 * TC_SerialPortGroup serialPortGroup;
 * serialPortGroup.initialize();
 *
 * TC_SerialPortGroup::Options options;
 * options.portName = ....
 - 定义回调

class SerialPortCallback : public TC_SerialPort::RequestCallback
{
public:
	void onSucc(const vector<char> &data)
	{
        cout << "data: " << TC_Common::bin2str(data.data(), data.size()) << endl;
	}

    void onOpen()
    {
        cout << "onOpen" << endl;
    }

	void onFailed(const string &info)
	{
		cout << "info: " << info << endl;
	}

	void onClose()
	{
		cout << "onClose" << endl;
	}
};

auto callbackPtr = std::make_shared<SerialPortCallback>();
- 定义一个协议解析器
TC_NetWorkBuffer::PACKET_TYPE parser(TC_NetWorkBuffer&buff, vector<char> &out)
{
    if(buffer.empty())
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }

    data = buffer.getBuffers();
    if(data[data.size() - 1] != 0x0d)
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }
	buffer.moveHeader(data.size());
	return TC_NetWorkBuffer::PACKET_FULL;
}

 * shared_ptr<TC_SerialPort> serialPort = serialPortGroup.create(options, onparser, callbackPtr);
- 发送数据
 * serialPort->sendRequest(...);
 */

/**
 * 串口异常
 */
class TC_SerialPortException : public TC_Exception
{
public:
	TC_SerialPortException(const string & buffer)
		: TC_Exception(buffer)
	{
	}
};

class TC_SerialPort;
class TC_SerialPortGroup;

/**
* 异步串口通信类
*/
class UTIL_DLL_API TC_SerialPort
{
public:
   /**
    * @brief 异步请求回调对象, 除了onOpen可能在业务线程中回调外, 其他都在串口通信线程中回调
    * onSucc: 成功回调
    * onFailed: 失败回调, 无论什么失败都会被调用掉
    */
    class RequestCallback 
    {
    public:
		virtual ~RequestCallback() = default;
        /**
         * @brief 完整的响应回来了.
         * 增加一个回调, 建议使用这个回调, 减少一次内存copy, 默认回调onSucc(string), 建立历史版本
         * @param buff
         */
        virtual void onSucc(const vector<char> &buff) {  }

        /**
        * @brief 异常, 发生异常时, onClose也会被响应, 从而关闭网络连接.
        *
        * @param ex 异常原因
        */
        virtual void onFailed(const string &info){};

        /**
         * @brief 连接建立后的回调
         * 可以在这个回调里面发送第一个数据
         */
        virtual void onOpen() {};

        /**
        * @brief 连接被关闭
        */
        virtual void onClose() = 0;
    };

    typedef shared_ptr<RequestCallback> RequestCallbackPtr;


	//协议解析器
	using onparser_callback = std::function<TC_NetWorkBuffer::PACKET_TYPE(TC_NetWorkBuffer &, vector<char> &)>;


    struct Options
    {
        std::string portName;           //串口地址
        int baudRate;            		//输入输出比特率
    	uint8_t byteSize = 8;        	/* Number of bits/byte, 5-8        */
    	uint8_t parity = 0;          	/* 0-4=None,Odd,Even,Mark,Space    */
    	uint8_t stopBits = 1;        	/* 1,2              */		
    };

	/**
	 * 构造
	 */
	TC_SerialPort(const Options & options, TC_SerialPortGroup *serialPortGroup, const TC_SerialPort::onparser_callback & onparser, const RequestCallbackPtr & callbackPtr);

	/**
	 * 析构
	 */
	~TC_SerialPort();

	/**
	 * 获取options
	 * @return
	 */
	const Options & options()
	{
		return _options;
	}

	/**
	* @brief 异步发送buffer, 如果无法打开串口, 则抛出异常TC_SerialPortException
	* @param    string & sBuffer
	* @param bool header, 是否把数据插入到队列头部, 默认数据都在尾部的!
	*/
	void sendRequest(const std::string & sBuffer, bool header = false);

	/**
	* @brief 异步发送buffer(尽量使用这个函数, 减少一次内存copy), 如果无法打开串口, 则抛出异常TC_SerialPortException
	* @sendRequest
	* @param    string & sBu}	* @param bool header, 是否把数据插入到队列头部, 默认数据都在尾部的!
	*/
	void sendRequest(const std::shared_ptr<TC_NetWorkBuffer::Buffer> & buff, bool header = false);

	/**
	 * 设置数据cookie
	 * @param cookie
	 */
	void setCookie(void *cookie)
	{
		_cookie = cookie;
	}

	/**
	 * 获取数据cookie
	 * @return
	 */
	void * getCookie()
	{
		return _cookie;
	}


protected:

    friend class TC_SerialPortGroup;

	/**
	 * 初始化串口
	 */
	void initialize();

	/**
	 * sendRequest返回值
	 */
	enum ReturnStatus
	{
		eRetError = -1,
		eRetOk = 0,
		eRetFull = 1,
		eRetNotSend = 2,
	};

	/**
	 * 添加一个发送buffer
	 * @param reqBuffer
	 * @param header, 是否添加到头部
	 *
	 */
	void addSendReqBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer> & reqBuffer, bool header = false);

#if !TARGET_PLATFORM_WINDOWS
	bool handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> & data);

	bool handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> & data);

	bool handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> & data);
#else
	bool handleCloseImp();

	bool handleInputImp();

	bool handleOutputImp();
#endif
	/**
	 * 实际写串口数据
	 * @param buff
	 * @return
	 */
	ReturnStatus writeBuffer(const std::shared_ptr<TC_NetWorkBuffer::Buffer> & buff);

	/**
	 * 发送请求
	 */
	void doRequest();

	/**
	 * 发送数据
	 * @param buf
	 * @param len
	 * @param flag
	 * @return
	 */
	int send(const void *buf, uint32_t len);
#if !TARGET_PLATFORM_WINDOWS
	/**
	 * 读取数据
	 * @param buf
	 * @param len
	 * @param flag
	 * @return
	 */
	int recv(void *buf, uint32_t len);
#else
	int recv();
#endif

	/**
	 * 协议解析器
	 * @param buff
	 * @return
	 */
	int doProtocolAnalysis(TC_NetWorkBuffer *buff);

	/**
	 *
	 */
	void onRequestCallback();

	/**
	 * 是否有效
	 * @return
	 */
	bool isValid()
	{
	#if TARGET_PLATFORM_WINDOWS
		return _serialFd != INVALID_HANDLE_VALUE;
	#else
		return _serialFd != -1;
	#endif
	}
#if TARGET_PLATFORM_WINDOWS
	HANDLE getfd()
	{
		return _serialFd;
	}
#else
	int getfd()
	{
		return _serialFd;
	}
#endif

	/**
	 * 关闭串口句柄
	 */
	void close();

	/**
	 * 获取请求回调
	 * @return
	 */
	RequestCallbackPtr getCallbackPtr();
#if TARGET_PLATFORM_WINDOWS
	OVERLAPPED *getOsRead() { return &_osRead; }

	OVERLAPPED *getOsWrite() { return &_osWrite; }

	void sendSucc(uint32_t len);

	void recvSucc(uint32_t len);

#endif
protected:

	/**
	 * 串口管理组
	 */
	TC_SerialPortGroup *_serialPortGroup = NULL;

	/**
	 * 串口参数
	 */
	Options _options;

	std::mutex _mutex;
	std::condition_variable _cond;

#if TARGET_PLATFORM_WINDOWS
	HANDLE _serialFd = INVALID_HANDLE_VALUE;

	OVERLAPPED _osRead;
	
	OVERLAPPED _osWrite;

	list<std::shared_ptr<TC_NetWorkBuffer::Buffer>> _buffRecv;

#else
	/**
	 * 串口句柄
	 */
	int _serialFd = -1;

	/**
	 * epollInfo
	 */
	shared_ptr<TC_Epoller::EpollInfo> _epollInfo;

#endif

	/**
	 * 数据队列, 注意, 不要用deque, 因为后面要删除迭代器, 可能失效
	 */
	std::list<std::pair<std::shared_ptr<TC_NetWorkBuffer::Buffer>, bool>> _messages;

	/*
	 * 发送buffer
	 */
	TC_NetWorkBuffer _sendBuffer;

	/*
	 * 接收buffer
	 */
	TC_NetWorkBuffer _recvBuffer;

	/**
	 * 协议解析器
	 */
	onparser_callback _onParserCallback;

	/**
	 * 数据cookie
	 */
	void *_cookie;

	/**
	 * 请求回调
	 */
    RequestCallbackPtr          _callbackPtr;

};


class UTIL_DLL_API TC_SerialPortGroup
{
public:

	/**
	 * 
	 */
	~TC_SerialPortGroup();

    /**
     * 初始化
     */
    void initialize();

    /**
     * 创建某个串口
     * @param options
     * @return
     */
    shared_ptr<TC_SerialPort> create(const TC_SerialPort::Options & options, const TC_SerialPort::onparser_callback & onparser, const TC_SerialPort::RequestCallbackPtr & callbackPtr);

    /**
     * 删除串口
     * @param sp
     */
    void erase(const std::shared_ptr<TC_SerialPort> & sp);
#if !TARGET_PLATFORM_WINDOWS

    /**
     * 获取epoller
     * @return
     */
    TC_Epoller & getEpoller()
    {
        return _epoller;
    }
#else
	HANDLE getIoPort() 
	{
		return _ioPort;
	}

#endif
    /**
     * 结束
     */
    void terminate();


protected:
    void run();


protected:
#if !TARGET_PLATFORM_WINDOWS

    /**
     * epoller对象
     */
    TC_Epoller _epoller;
#else
	HANDLE _ioPort = INVALID_HANDLE_VALUE;

#endif
    std::mutex _mutex;

    std::map<std::string, std::shared_ptr<TC_SerialPort>> _serialPorts;

    std::thread *_th = NULL;
};

}

