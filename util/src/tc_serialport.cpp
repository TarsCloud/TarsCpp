//
// Created by jarod on 2023/8/22.
//

#include "util/tc_serialport.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <termios.h>
#endif

namespace tars
{

static const int BUFFER_SIZE = 8 * 1024;

static const int MAX_BUFFER_SIZE = 64 * 1024;

void TC_SerialPortGroup::initialize()
{
	if (_th)
	{
		throw TC_SerialPortException("serial port group has initialize.");
	}

	_th = new std::thread(std::bind(&TC_SerialPortGroup::run, this));

	_epoller.create(1024);
}

shared_ptr<TC_SerialPort> TC_SerialPortGroup::create(const TC_SerialPort::Options & options, const TC_SerialPort::onparser_callback & onparser)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_serialPorts.find(options.portName) != _serialPorts.end())
	{
		throw TC_SerialPortException("serial port: `" + options.portName + "` has initialize.");
	}

	shared_ptr<TC_SerialPort> sp = std::make_shared<TC_SerialPort>(options, this);
	
	sp->initialize(onparser);

    _serialPorts[options.portName] = sp;

	return sp;
}

void TC_SerialPortGroup::erase(const shared_ptr<TC_SerialPort> & sp)
{
	std::lock_guard<std::mutex> lock(_mutex);

	_serialPorts.erase(sp->options().portName);
}

void TC_SerialPortGroup::run()
{
	_epoller.idle([&]
	              {

		              std::lock_guard<std::mutex> lock(_mutex);
		              for (auto e: _serialPorts)
		              {
			              e.second->doRequest();
		              }
	              });

	_epoller.loop();
}

void TC_SerialPortGroup::terminate()
{
	if (!_th)
	{
		return;
	}
	_epoller.terminate();
	_th->join();
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_serialPorts.clear();
	}
	delete _th;
	_th = NULL;

}

//////////////////////////////////////////////////////////////
TC_SerialPort::TC_SerialPort(const Options & options, TC_SerialPortGroup *serialPortGroup)
	: _serialPortGroup(serialPortGroup), _options(options), _sendBuffer(NULL), _recvBuffer(NULL)
{
}

TC_SerialPort::~TC_SerialPort()
{
	close();
}

void TC_SerialPort::close()
{
	if (_serialFd >= 0)
	{
#if TARGET_PLATFORM_WINDOWS
		CloseHandle(_serialFd); 
		_serialFd = INVALID_HANDLE_VALUE;
#else
		::close(_serialFd);
		_serialFd = -1;
#endif
	}
}

void TC_SerialPort::initialize(const TC_SerialPort::onparser_callback & onparser)
{
	if (_serialFd >= 0)
    {
        throw TC_SerialPortException("open serial port: " + _options.portName + " has initialized.");
    }

#if TARGET_PLATFORM_WINDOWS

	_serialFd = CreateFileA(_options.portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if( _serialFd == NULL ) 
	{
		 throw TC_SerialPortException("open serial port: " + _options.portName + " has initialized.");
	}
	DCB dcb;

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 100;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( _serialFd, &CommTimeOuts );

	dcb.DCBlength = sizeof( DCB );
	GetCommState( _serialFd, &dcb );
	dcb.BaudRate = _options.baudIn;
	dcb.ByteSize = _options.byteSize;
	dcb.Parity = _options.parity;
	dcb.StopBits = _options.stopBits;
	if( !SetCommState( _serialFd, &dcb ) || !SetupComm( _serialFd, 10000, 10000 ) )
	{
		CloseHandle( _serialFd );
		throw TC_SerialPortException("Failed to set serial port: " + _options.portName + ", error:" + TC_Exception::getSystemError());
	}
#else

	_serialFd = open(_options.portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (_serialFd == -1)
	{
		throw TC_SerialPortException("Failed to open serial port: " + _options.portName);
	}


	struct termios serialSettings;
    bzero(&serialSettings, sizeof(serialSettings));

	tcgetattr(_serialFd, &serialSettings);

	cfsetispeed(&serialSettings, _options.baudIn);
	cfsetospeed(&serialSettings, _options.baudOut);
	switch(_options.byteSize)
	{
		case 5:
			serialSettings.c_cflag |= CS5;
			break;
		case 6:
			serialSettings.c_cflag |= CS6;
			break;
		case 7:
			serialSettings.c_cflag |= CS7;
			break;
		case 8:
			serialSettings.c_cflag |= CS8;
			break;
		default:
			serialSettings.c_cflag |= CS8;
			break;
	}

	switch(_options.parity)
	{
		case 0:
			serialSettings.c_cflag &= ~PARENB;
			break;
		case 1:
			serialSettings.c_cflag |= PARENB;
			serialSettings.c_cflag |= PARODD;
			break;
		case 2:
			serialSettings.c_cflag |= PARENB;
			serialSettings.c_cflag &= ~PARODD;
			break;
		case 3:
			serialSettings.c_cflag |= PARENB;
			serialSettings.c_cflag |= PARODD;
			break;
		case 4:
			serialSettings.c_cflag |= PARENB;
			serialSettings.c_cflag &= ~PARODD;
			break;
		default:
			serialSettings.c_cflag &= ~PARENB;
			break;
	}

	switch(_options.stopBits)
	{
		case 0:
			serialSettings.c_cflag &= ~CSTOPB;
			break;
		case 1:
			serialSettings.c_cflag &= ~CSTOPB;
			break;
		case 2:
			serialSettings.c_cflag |= CSTOPB;
			break;
		default:
			serialSettings.c_cflag &= ~CSTOPB;
			break;
	}

	int ret = tcsetattr(_serialFd, TCSANOW, &serialSettings);
	if (ret != 0)
	{
		throw TC_SerialPortException("Failed to set serial port: " + _options.portName);
	}
	tcflush(_serialFd, TCIOFLUSH);
#endif

    _onParserCallback = onparser;

	_epollInfo = _serialPortGroup->getEpoller().createEpollInfo((int)_serialFd);

	map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

	callbacks[EPOLLIN] = std::bind(&TC_SerialPort::handleInputImp, this, std::placeholders::_1);
	callbacks[EPOLLOUT] = std::bind(&TC_SerialPort::handleOutputImp, this, std::placeholders::_1);
	callbacks[EPOLLERR] = std::bind(&TC_SerialPort::handleCloseImp, this, std::placeholders::_1);

	_epollInfo->registerCallback(callbacks, EPOLLIN | EPOLLOUT);
}

void TC_SerialPort::sendRequest(const string & sBuffer, bool header)
{
	if (sBuffer.empty())
		return;

	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	buff->addBuffer(sBuffer);

	addSendReqBuffer(buff, header);
}

void TC_SerialPort::sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> & buff, bool header)
{
	if (buff && buff->empty())
		return;

	addSendReqBuffer(buff, header);
}

void TC_SerialPort::addSendReqBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer> & reqBuffer, bool header)
{
	std::lock_guard<std::mutex> lock(_mutex);

	if (header)
	{
		_messages.push_front(std::make_pair(reqBuffer, false));
	}
	else
	{
		_messages.push_back(std::make_pair(reqBuffer, false));
	}

	//发送消息, 唤醒网络线程
	_serialPortGroup->getEpoller().notify();
}

int TC_SerialPort::doProtocolAnalysis(TC_NetWorkBuffer *buff)
{
	TC_NetWorkBuffer::PACKET_TYPE ret;

	int packetCount = 0;

	int ioriginal = 0;
	int isurplus = 0;
	try
	{
		do
		{
			ioriginal = buff->getBufferLength();
			ret = _onParserCallback(*buff, this);
			isurplus = buff->getBufferLength();

			if (ret == TC_NetWorkBuffer::PACKET_FULL || ret == TC_NetWorkBuffer::PACKET_FULL_CLOSE)
			{
				++packetCount;
			}

			// 当收到完整包时，解析完包后，buffer没movehead，则报错
			if (ret == TC_NetWorkBuffer::PACKET_FULL && ioriginal == isurplus)
			{
				ret = TC_NetWorkBuffer::PACKET_ERR;
				string err = "parser buffer movehead error";
				throw TC_SerialPortException(err);
			}

		}
		while (ret == TC_NetWorkBuffer::PACKET_FULL);
	}
	catch (exception & ex)
	{
		throw TC_SerialPortException("parser decode error:" + string(ex.what()));
	}
	catch (...)
	{
		throw TC_SerialPortException("parser decode error");
	}

	if (ret == TC_NetWorkBuffer::PACKET_ERR)
	{
		string err = "parser decode error";
		throw TC_SerialPortException("parser decode error");
	}

	return packetCount;
}

bool TC_SerialPort::handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> & epollInfo)
{
	close();
	return false;
}

bool TC_SerialPort::handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> & epollInfo)
{
	//串口读取数据

	int iRet = 0;
//	int64_t now = TNOWMS;

	do
	{
		size_t expansion = (std::max)((std::min)(_recvBuffer.getBufferLength(), (size_t) MAX_BUFFER_SIZE), (size_t) BUFFER_SIZE);
		auto data = _recvBuffer.getOrCreateBuffer(BUFFER_SIZE / 2, expansion);

		uint32_t left = (uint32_t) data->left();

		if ((iRet = this->recv((void *) data->free(), left)) > 0)
		{
			data->addWriteIdx(iRet);

			_recvBuffer.addLength(iRet);

			//解析协议
			_onParserCallback(_recvBuffer, this);

			//接收的数据小于buffer大小, 内核会再次通知你
			if (iRet < (int) left)
			{
				break;
			}
		}
	}
	while (iRet > 0);

	if (iRet == 0)
	{
		close();
		throw TC_SerialPortException("peer close connection");
	}

	return true;
}

void TC_SerialPort::onRequestCallback()
{
	//串口发送数据
	for (;;)
	{
		decltype(_messages)::iterator it;

		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_messages.empty())
			{
				return;
			}

			it = _messages.begin();
		}

		ReturnStatus iRet = writeBuffer(it->first);
		if (iRet == eRetError)
		{
			return;
		}

		if (iRet != eRetNotSend)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_messages.erase(it);
		}

		//数据还不能发送 or 发送buffer已经满了 直接返回, 暂时不要再发送了!
		if (iRet == eRetNotSend || iRet == eRetFull)
		{
			return;
		}

		return;
	}
}

bool TC_SerialPort::handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> & data)
{
	doRequest();
	return true;
}

TC_SerialPort::ReturnStatus TC_SerialPort::writeBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer> & buff)
{
	//空数据 直接返回成功
	if (buff->empty())
	{
		return eRetOk;
	}

	// assert(_sendBuffer.empty());
	//buf不为空, 表示之前的数据还没发送完, 直接返回失败, 等buffer可写了,epoll会通知写事件
	if (!_sendBuffer.empty())
	{
		//不应该运行到这里
		return eRetNotSend;
	}

	_sendBuffer.addBuffer(buff);

	do
	{
		auto data = _sendBuffer.getBufferPointer();

		int iRet = this->send(data.first, (uint32_t) data.second);
		if (iRet < 0)
		{
			if (!isValid())
			{
				_sendBuffer.clearBuffers();
				return eRetError;
			}
			else
			{
				return eRetFull;
			}
		}

		_sendBuffer.moveHeader(iRet);
//		assert(iRet != 0);
	}
	while (!_sendBuffer.empty());

	return eRetOk;
}

void TC_SerialPort::doRequest()
{
	//buf不为空,先发送buffer的内容
	while (!_sendBuffer.empty())
	{
		auto data = _sendBuffer.getBufferPointer();
		assert(data.first != NULL && data.second != 0);

		int iRet = this->send(data.first, (uint32_t) data.second);

		if (iRet <= 0)
		{
			return;
		}

		_sendBuffer.moveHeader(iRet);
	}

	if (_sendBuffer.empty())
	{
		onRequestCallback();
	}
}

int TC_SerialPort::send(const void *buf, uint32_t len)
{
#if TARGET_PLATFORM_WINDOWS
	unsigned long dwBytesWritten = 0;
	bool bWriteStat = WriteFile(_serialFd, buf, len, &dwBytesWritten, NULL);

	if (!bWriteStat && !TC_Socket::isPending())
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "send error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		close();
		throw TC_SerialPortException("TC_SerialPort::send, fd:" + TC_Common::tostr(_serialFd) + ", error:" + err);
	}

	return dwBytesWritten;
#else
	int iRet = ::write(_serialFd, (const char *) buf, len);

	if (iRet < 0 && !TC_Socket::isPending())
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "send error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		close();
		throw TC_SerialPortException("TC_SerialPort::send, fd:" + TC_Common::tostr(_serialFd) + ", error:" + err);
	}
	return iRet;

#endif
}

int TC_SerialPort::recv(void *buf, uint32_t len)
{
#if TARGET_PLATFORM_WINDOWS
	unsigned long dwBytesRead = 0;
	bool bReadStatus = ReadFile( _serialFd, buf, len, &dwBytesRead, NULL );

	if (!bReadStatus && !TC_Socket::isPending())
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "recv error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		close();
		throw TC_SerialPortException("TC_SerialPort::recv, fd:" + TC_Common::tostr(_serialFd) + ", error:" + err);
	}

	return dwBytesRead;
#else
	int iRet = ::read(_serialFd, (char *) buf, len);

	if ((iRet < 0 && !TC_Socket::isPending()))
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "recv error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		close();
		throw TC_SerialPortException("TC_SerialPort::recv, fd:" + TC_Common::tostr(_serialFd) + ", error:" + err);
	}
	return iRet;

#endif
}

}
