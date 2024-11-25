//
// Created by jarod on 2023/8/22.
//

#include "util/tc_serialport.h"
#include "util/tc_logger.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <termios.h>
#endif

#if defined(_MSC_VER)
#include <setupapi.h>
#include <devguid.h>
#pragma comment(lib, "setupapi.lib")
#endif

namespace tars
{

static const int BUFFER_SIZE = 1 * 1024;

static const int MAX_BUFFER_SIZE = 10 * 1024;

TC_SerialPortGroup::~TC_SerialPortGroup()
{
#if TARGET_PLATFORM_WINDOWS
	if(_ioPort != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_ioPort); 
		_ioPort = INVALID_HANDLE_VALUE;
	}
#endif
}
void TC_SerialPortGroup::initialize()
{
	if (_th)
	{
		throw TC_SerialPortException("serial port group has initialize.");
	}

#if !TARGET_PLATFORM_WINDOWS
	_epoller.create(1024);
#else
	_ioPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
#endif
	_th = new std::thread(std::bind(&TC_SerialPortGroup::run, this));
}

shared_ptr<TC_SerialPort> TC_SerialPortGroup::create(const TC_SerialPort::Options & options, const TC_SerialPort::onparser_callback & onparser, const TC_SerialPort::RequestCallbackPtr & callbackPtr)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_serialPorts.find(options.portName) != _serialPorts.end())
	{
		throw TC_SerialPortException("serial port: `" + options.portName + "` has initialize.");
	}

	shared_ptr<TC_SerialPort> sp = std::make_shared<TC_SerialPort>(options, this, onparser, callbackPtr);
	
	sp->initialize();

    _serialPorts[options.portName] = sp;

	return sp;
}

void TC_SerialPortGroup::erase(const shared_ptr<TC_SerialPort> & sp)
{
	std::lock_guard<std::mutex> lock(_mutex);

	_serialPorts.erase(sp->options().portName);
}

vector<string> TC_SerialPortGroup::getComPorts(const string &prefix)
{
	vector<string> comPorts;
#if TARGET_PLATFORM_WINDOWS
	HDEVINFO deviceInfoSet = SetupDiGetClassDevs(
        &GUID_DEVCLASS_PORTS, // 串口设备的 GUID
        NULL,               // 枚举所有设备
        NULL,               // 无上下文
        DIGCF_PRESENT);     // 仅包括当前存在的设备

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
		return comPorts;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
		HKEY hDevKey = SetupDiOpenDevRegKey(deviceInfoSet, &deviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
		if (INVALID_HANDLE_VALUE != hDevKey)
		{
			char portName[256] = {0x00};
			DWORD dwCount = 255; 
			RegQueryValueExA(hDevKey, "PortName", NULL, NULL, (BYTE*)portName, &dwCount);
			RegCloseKey(hDevKey);
			comPorts.push_back(portName);
		}
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
#else
	vector<string> files;
	TC_File::listDirectory("/dev/", files, false, true);

	for(const auto &file : files)
	{
		if(file.find(prefix) != string::npos)
		{
			comPorts.push_back(file);
		}
	}
#endif
	return comPorts;
}

void TC_SerialPortGroup::run()
{
#if !TARGET_PLATFORM_WINDOWS
	_epoller.idle([&]
	              {
		              std::lock_guard<std::mutex> lock(_mutex);
		              for (const auto &e: _serialPorts)
		              {
			              e.second->doRequest();
		              }
	              });

	_epoller.loop();

#else

	BOOL bRet;
	DWORD dwNumberOfBytesTransferred = 0;
	DWORD dwCompletionKey = 0;
	OVERLAPPED *opOverlapped = nullptr;

	while(true)
	{
		bool bFlag = GetQueuedCompletionStatus(_ioPort, &dwNumberOfBytesTransferred, (PULONG_PTR)(void*)&dwCompletionKey, &opOverlapped, 100);

		if(bFlag && dwCompletionKey == -1)
		{
			return;
		}

		std::map<std::string, std::shared_ptr<TC_SerialPort>> serialPorts;

		{	
			std::lock_guard<std::mutex> lock(_mutex);
			serialPorts = _serialPorts;	
		}

		for (const auto &e: serialPorts)
		{
			try
			{
				if(bFlag)
				{			
					if(opOverlapped == e.second->getOsRead())
					{
						e.second->recvSucc(dwNumberOfBytesTransferred);
					}
					else if(opOverlapped == e.second->getOsWrite())
					{
						if(dwNumberOfBytesTransferred == 0)
						{
							e.second->doRequest();
						}
						else
						{
							e.second->sendSucc(dwNumberOfBytesTransferred);
						}

					}
				}
				e.second->handleInputImp();
			}
			catch(const std::exception& ex)
			{
				e.second->getCallbackPtr()->onFailed(ex.what());
				e.second->close();
			}
		}
	}

#endif
}


void TC_SerialPortGroup::terminate()
{
	if (!_th)
	{
		return;
	}
#if !TARGET_PLATFORM_WINDOWS	
	_epoller.terminate();
#else
	PostQueuedCompletionStatus(_ioPort, 0, -1, NULL);

#endif
	_th->join();
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_serialPorts.clear();
	}
	delete _th;
	_th = NULL;

}

//////////////////////////////////////////////////////////////
TC_SerialPort::TC_SerialPort(const Options & options, TC_SerialPortGroup *serialPortGroup, const TC_SerialPort::onparser_callback & onparser, const RequestCallbackPtr & callbackPtr)
	: _serialPortGroup(serialPortGroup), _options(options), _sendBuffer(NULL), _recvBuffer(NULL), _onParserCallback(onparser), _callbackPtr(callbackPtr)
{
}

TC_SerialPort::~TC_SerialPort()
{
	close();
}

void TC_SerialPort::close()
{
#if TARGET_PLATFORM_WINDOWS

	if( _osRead.hEvent != NULL ) 
	{
		CloseHandle( _osRead.hEvent);
		_osRead.hEvent = NULL;
		
	}

	if( _osWrite.hEvent != NULL ) 
	{
		CloseHandle( _osWrite.hEvent );	
		_osWrite.hEvent = NULL;
	}

	if(_serialFd != INVALID_HANDLE_VALUE)
	{

		CloseHandle(_serialFd); 
		_serialFd = INVALID_HANDLE_VALUE;
	}
#else
	if (_serialFd >= 0)
	{

		::close(_serialFd);
		_serialFd = -1;
	}
#endif

	_callbackPtr->onClose();	
}

void TC_SerialPort::initialize()
{
#if TARGET_PLATFORM_WINDOWS
	if (_serialFd != INVALID_HANDLE_VALUE)
    {
        throw TC_SerialPortException("open serial port: " + _options.portName + " has initialized.");
    }

	_serialFd = CreateFileA(_options.portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, NULL );

	if( _serialFd == NULL ) 
	{
		 throw TC_SerialPortException("open serial port: " + _options.portName + " create failed:" + TC_Exception::getSystemError());
	}

	memset(&_osRead, 0, sizeof(OVERLAPPED));
	memset(&_osWrite, 0, sizeof(OVERLAPPED));
	_osRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	_osWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 100;
	CommTimeOuts.ReadTotalTimeoutConstant = 2000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( _serialFd, &CommTimeOuts );

	DCB dcb;
	dcb.DCBlength = sizeof( DCB );
	GetCommState( _serialFd, &dcb );

	dcb.BaudRate = _options.baudRate;
	dcb.ByteSize = _options.byteSize;
	dcb.Parity = _options.parity;
	dcb.StopBits = _options.stopBits;

	if( !SetCommState( _serialFd, &dcb ) || !SetupComm( _serialFd, 10000, 10000 ) || !PurgeComm(_serialFd,PURGE_TXCLEAR |PURGE_RXCLEAR))
	{
		CloseHandle( _serialFd );
		throw TC_SerialPortException("Failed to set serial port: " + _options.portName + ", error:" + TC_Exception::getSystemError());
	}

	if(!CreateIoCompletionPort(_serialFd, _serialPortGroup->getIoPort(), 0, 0))
	{
		CloseHandle( _serialFd );
		throw TC_SerialPortException("Failed to bind io port: " + _options.portName + ", error:" + TC_Exception::getSystemError());		
	}

	_buffRecv.clear();

#else
	if (_serialFd >= 0)
    {
        throw TC_SerialPortException("open serial port: " + _options.portName + " has initialized.");
    }

	_serialFd = open(_options.portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (_serialFd == -1)
	{
		throw TC_SerialPortException("Failed to open serial port: " + _options.portName);
	}


	struct termios serialSettings;
    bzero(&serialSettings, sizeof(serialSettings));

	int ret = tcgetattr(_serialFd, &serialSettings);

	cfsetispeed(&serialSettings, _options.baudRate);
	cfsetospeed(&serialSettings, _options.baudRate);

	serialSettings.c_cflag &= ~CSIZE;
	serialSettings.c_cflag |= CLOCAL | CREAD;

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
			serialSettings.c_cflag |= (INPCK | ISTRIP);
			break;
		case 2:
			serialSettings.c_cflag |= (INPCK | ISTRIP);
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
		case 1:
			serialSettings.c_cflag &= ~CSTOPB;
			break;
		case 2:
			serialSettings.c_cflag |= CSTOPB;
			break;
	}

	serialSettings.c_cc[VTIME] = 0;	
	serialSettings.c_cc[VMIN] = 0;	

	ret = tcsetattr(_serialFd, TCSANOW, &serialSettings);
	if (ret != 0)
	{
		close();	
		throw TC_SerialPortException("Failed to set serial port: " + _options.portName + ", error:" + TC_Exception::getSystemError());
	}
	tcflush(_serialFd, TCIOFLUSH);

	_epollInfo = _serialPortGroup->getEpoller().createEpollInfo((int)_serialFd);

	map<uint32_t, TC_Epoller::EpollInfo::EVENT_CALLBACK> callbacks;

	callbacks[EPOLLIN] = std::bind(&TC_SerialPort::handleInputImp, this, std::placeholders::_1);
	callbacks[EPOLLOUT] = std::bind(&TC_SerialPort::handleOutputImp, this, std::placeholders::_1);
	callbacks[EPOLLERR] = std::bind(&TC_SerialPort::handleCloseImp, this, std::placeholders::_1);

	_epollInfo->registerCallback(callbacks, EPOLLIN | EPOLLOUT);
#endif
	_sendBuffer.clearBuffers();
	_recvBuffer.clearBuffers();
	
	auto callback = getCallbackPtr();
	if(callback)	
	{
		callback->onOpen();
	}

}

void TC_SerialPort::setParserCallback(const onparser_callback & onparser)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_onParserCallback = onparser;
}

void TC_SerialPort::setRequestCallback(const RequestCallbackPtr & callbackPtr)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_callbackPtr = callbackPtr;
}

TC_SerialPort::RequestCallbackPtr TC_SerialPort::getCallbackPtr()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _callbackPtr;
}

void TC_SerialPort::sendRequest(const string & sBuffer, bool header)
{
	if (sBuffer.empty())
		return;

	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	buff->addBuffer(sBuffer);

	if(!isValid())
	{
		initialize();
	}
	addSendReqBuffer(buff, header);
}

void TC_SerialPort::sendRequest(const shared_ptr<TC_NetWorkBuffer::Buffer> & buff, bool header)
{
	if (buff && buff->empty())
		return;

	if(!isValid())
	{
		initialize();
	}
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
#if TARGET_PLATFORM_WINDOWS
	PostQueuedCompletionStatus(_serialPortGroup->getIoPort(), 0, 0, &_osWrite);
#else
	_serialPortGroup->getEpoller().notify();
#endif

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
			vector<char> out;
			ioriginal = buff->getBufferLength();

			{
				std::lock_guard<std::mutex> lock(_mutex);
				ret = _onParserCallback(*buff, out);
			}
			isurplus = buff->getBufferLength();

			if (ret == TC_NetWorkBuffer::PACKET_FULL || ret == TC_NetWorkBuffer::PACKET_FULL_CLOSE)
			{
				++packetCount;

				auto callback = getCallbackPtr();

				if (callback)
				{
					try { callback->onSucc(out); } catch (...) { }
				}

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
#if !TARGET_PLATFORM_WINDOWS
bool TC_SerialPort::handleCloseImp(const shared_ptr<TC_Epoller::EpollInfo> & epollInfo)
#else
bool TC_SerialPort::handleCloseImp()

#endif
{
	close();
	return false;
}
#if !TARGET_PLATFORM_WINDOWS

bool TC_SerialPort::handleInputImp(const shared_ptr<TC_Epoller::EpollInfo> & epollInfo)
{
	try
	{
		//串口读取数据
		int iRet = 0;

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
				doProtocolAnalysis(&_recvBuffer);

				//接收的数据小于buffer大小, 内核会再次通知你
				if (iRet < (int) left)
				{
					break;
				}
			}

			break;
		}
		while (iRet > 0);

		if (iRet == 0)
		{
			close();
			throw TC_SerialPortException("peer close connection");
		}
	}
	catch (exception & ex)
	{
		auto callback = getCallbackPtr();

		if (callback)
		{
			callback->onFailed(ex.what());
			callback->onClose();
		}
	}

	return true;
}

#else
bool TC_SerialPort::handleInputImp()
{
	try
	{
		this->recv();
	}
	catch (exception & ex)
	{
		auto callback = getCallbackPtr();

		if (callback)
		{
			callback->onFailed(ex.what());
			callback->onClose();
		}
	}
	return true;
}
#endif
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

#if !TARGET_PLATFORM_WINDOWS
bool TC_SerialPort::handleOutputImp(const shared_ptr<TC_Epoller::EpollInfo> & data)
#else
bool TC_SerialPort::handleOutputImp()
#endif
{
	try
	{
		doRequest();
	}
	catch (exception & ex)
	{
		auto callback = getCallbackPtr();
		if (callback)
		{
			callback->onFailed(ex.what());
			callback->onClose();
		}
	}
	return true;
}

#if TARGET_PLATFORM_WINDOWS
void TC_SerialPort::sendSucc(uint32_t len)
{
	_sendBuffer.moveHeader(len);
}

void TC_SerialPort::recvSucc(uint32_t len)
{
	try
	{
		assert(!_buffRecv.empty());

		auto it = _buffRecv.begin();

		if((*it)->length() > 0)
		{
			assert((*it)->length() == len);
		}
		else
		{
			(*it)->addWriteIdx(len);
		}

		_recvBuffer.addBuffer(*it);
		_buffRecv.erase(it);

		doProtocolAnalysis(&_recvBuffer);
	}
	catch (exception & ex)
	{
		auto callback = getCallbackPtr();
		if (callback)
		{
			callback->onFailed(ex.what());
			callback->onClose();
		}
	}	

}

#endif

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
		else if(iRet == 0)
		{
			return eRetOk;
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
	if(len == 0)
	{
		return 0;
	}
	unsigned long dwBytesWritten = 0;
	bool bWriteStat = WriteFile(_serialFd, buf, len, &dwBytesWritten, &_osWrite);
	bool isPending = TC_Socket::isPending() ; 
	if (!bWriteStat && !isPending)
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "send error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		HANDLE fd = _serialFd;
		close();
		throw TC_SerialPortException("TC_SerialPort::send, fd:" + TC_Common::tostr(fd) + ", error:" + err);
	}

	return dwBytesWritten;
#else
	int iRet = ::write(_serialFd, (const char *) buf, len);

	if (iRet < 0 && !TC_Socket::isPending())
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "send error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		int fd = _serialFd;
		close();
		throw TC_SerialPortException("TC_SerialPort::send, fd:" + TC_Common::tostr(fd) + ", error:" + err);
	}
	return iRet;

#endif
}

#if TARGET_PLATFORM_WINDOWS

int TC_SerialPort::recv()
{
	DWORD  dwErrorFlags;
	COMSTAT ComStat;
	DWORD dwBytesRead;

	ClearCommError( _serialFd, &dwErrorFlags, &ComStat );

	if(ComStat.cbInQue <= 0)
	{
		return 0;
	}

	dwBytesRead = (DWORD) ComStat.cbInQue;

	std::shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();
	buff->alloc(dwBytesRead);
	_buffRecv.push_back(buff);

	bool bReadStatus = ReadFile( _serialFd, buff->buffer(), dwBytesRead, &dwBytesRead, &_osRead);
	bool isPending = TC_Socket::isPending();
	 
	if (!bReadStatus && !isPending)
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "recv error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		HANDLE fd = _serialFd;
		close();
		throw TC_SerialPortException("TC_SerialPort::recv, fd:" + TC_Common::tostr(fd) + ", error:" + err);
	}

	if(dwBytesRead > 0){
		buff->addWriteIdx(dwBytesRead);
	}

	return 0;
}

#else

int TC_SerialPort::recv(void *buf, uint32_t len)
{
	int iRet = ::read(_serialFd, (char *) buf, len);

	if ((iRet < 0 && !TC_Socket::isPending()))
	{
		int nerr = TC_Exception::getSystemCode();
		string err = "recv error, errno:" + TC_Common::tostr(nerr) + "," + TC_Exception::parseError(nerr);
		int fd = _serialFd;
		close();
		throw TC_SerialPortException("TC_SerialPort::recv, fd:" + TC_Common::tostr(fd) + ", error:" + err);
	}
	return iRet;
}
#endif

}
