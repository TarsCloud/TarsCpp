//
// Created by jarod on 2023/8/22.
//

#pragma once

#include "util/tc_platform.h"

#include <string>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include "util/tc_ex.h"
#include "util/tc_epoller.h"
#include "util/tc_network_buffer.h"

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
 *
 * shared_ptr<TC_SerialPort> serialPort = serialPortGroup.create(options);
 *
 * //定义一个协议解析器
TC_NetWorkBuffer::PACKET_TYPE parser(TC_NetWorkBuffer&buff, TC_SerialPort*)
{
    const char *p = buff.mergeBuffers();

    const char *pos = TC_Port::strnstr(p, "\r\n", buff.getBufferLength());

    if(pos == NULL)
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }

    string out(p, pos);

    //得到一个完整的串口输出类

    return TC_NetWorkBuffer::PACKET_FULL;
}

 * //设置读取串口后, 数据的解析器
 * serialPort->setParserCallback(std::bind(parser, std::placeholders::_1, std::placeholders::_2));
 * //发送数据
 * serialPort->sendRequest(...);
 * //在协议解析器中获取读取到的数据
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
	//协议解析器
	using onparser_callback = std::function<TC_NetWorkBuffer::PACKET_TYPE(TC_NetWorkBuffer &, TC_SerialPort *)>;

    struct Options
    {
        string portName;           		//串口地址
        int baudRate;            		//输入输出比特率
    	uint8_t byteSize = 8;        	/* Number of bits/byte, 5-8        */
    	uint8_t parity = 0;          	/* 0-4=None,Odd,Even,Mark,Space    */
    	uint8_t stopBits = 0;        	/* 0,1,2 = 1, 1.5, 2               */		
    };

    friend class TC_SerialPortGroup;

	/**
	 * 构造
	 */
	TC_SerialPort(const Options & options, TC_SerialPortGroup *serialPortGroup);

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
	* @brief 异步发送buffer
	* @sendRequest
	* @param    string & sBuffer
	* @param bool header, 是否把数据插入到队列头部, 默认数据都在尾部的!
	*/
	void sendRequest(const string & sBuffer, bool header = false);

	/**
	* @brief 异步发送buffer(尽量使用这个函数, 减少一次内存copy)
	* @sendRequest
	* @param    string & sBuffer
	* @param bool header, 是否把数据插入到队列头部, 默认数据都在尾部的!
	*/
	void sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> & buff, bool header = false);

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

	bool handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> & data);

	bool handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> & data);

	bool handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> & data);

	/**
	 * 实际写串口数据
	 * @param buff
	 * @return
	 */
	ReturnStatus writeBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer> & buff);

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

	/**
	 * 读取数据
	 * @param buf
	 * @param len
	 * @param flag
	 * @return
	 */
	int recv(void *buf, uint32_t len);

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

	/**
	 * 初始化串口
	 */
	void initialize(const TC_SerialPort::onparser_callback & onparser);

	/**
	 * 关闭串口句柄
	 */
	void close();

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
#else
	/**
	 * 串口句柄
	 */
	int _serialFd = -1;
#endif

	/**
	 * 数据队列, 注意, 不要用deque, 因为后面要删除迭代器, 可能失效
	 */
	list<pair<shared_ptr<TC_NetWorkBuffer::Buffer>, bool>> _messages;

	/**
	 * epollInfo
	 */
	shared_ptr<TC_Epoller::EpollInfo> _epollInfo;

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
};


class UTIL_DLL_API TC_SerialPortGroup
{
public:
    /**
     * 初始化
     */
    void initialize();

    /**
     * 创建某个串口
     * @param options
     * @return
     */
    shared_ptr<TC_SerialPort> create(const TC_SerialPort::Options & options, const TC_SerialPort::onparser_callback & onparser);

    /**
     * 删除串口
     * @param sp
     */
    void erase(const shared_ptr<TC_SerialPort> & sp);

    /**
     * 获取epoller
     * @return
     */
    TC_Epoller & getEpoller()
    {
        return _epoller;
    }

    /**
     * 结束
     */
    void terminate();


protected:
    void run();

protected:
    /**
     * epoller对象
     */
    TC_Epoller _epoller;

    std::mutex _mutex;

    map<string, shared_ptr<TC_SerialPort>> _serialPorts;

    std::thread *_th = NULL;
};

}

