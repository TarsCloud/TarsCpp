#include "util/tc_common.h"
#include "util/tc_serialport.h"
#include "util/tc_network_buffer.h"
#include "util/tc_common.h"
#include "util/tc_serialport.h"
#include "util/tc_logger.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilSerialPortTest : public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{
	}
	static void TearDownTestCase()
	{
	}
	virtual void SetUp()   //TEST跑之前会执行SetUp
	{
	}
	virtual void TearDown() //TEST跑完之后会执行TearDown
	{
	}
};


std::mutex mtx;
std::condition_variable cnd;

// 8cce966b8fc89763
class SerialPortCallback : public TC_SerialPort::RequestCallback
{
public:
	void onSucc(vector<char> &&data)
	{
        cout << "data: " << string(data.data(), data.size()) << endl;
        cout << "data hex: " << TC_Common::bin2str(data.data(), data.size()) << endl;
        _serialPort->notify(std::move(data));
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
    void onHeartbeat()
    {
        cout << "onHeartbeat" << endl;
    }
    std::shared_ptr<TC_SerialPort> _serialPort;
};


// 8cce966b8fc89763
class AsyncSerialPortCallback : public TC_SerialPort::RequestCallback
{
public:
	void onSucc(const vector<char> &data)
	{
        std::unique_lock<std::mutex> lock(mtx);
        cout << "data: " << string(data.data(), data.size()) << endl;
        cout << "data hex: " << TC_Common::bin2str(data.data(), data.size()) << endl;
        cnd.notify_one();
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

    void onHeartbeat()
    {
        cout << "onHeartbeat" << endl;
    }
};


vector<uint8_t> cmd_send = { 0x7e, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xab, 0xcd };
vector<uint8_t> cmd_recv = { 0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31 };

TC_NetWorkBuffer::PACKET_TYPE onParser1(TC_NetWorkBuffer &buffer, vector<char> &out)
{
    if(buffer.empty())
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }

    out = buffer.getBuffers();
    cout << "onSerialParser:" << TC_Common::bin2str(out.data(), out.size()) << endl;
    // 如果out的前面部分和cmd_recv相等, 丢弃前面的部分
    while(out.size() >= cmd_recv.size())
    {
        if(std::equal(cmd_recv.begin(), cmd_recv.end(), out.begin()))
        {
            buffer.moveHeader(cmd_recv.size());
            out = buffer.getBuffers();
        }
        else
        {
            break;
        }
    }

    if(out.empty())
    {
        return TC_NetWorkBuffer::PACKET_LESS;
    }

    if(out[out.size() - 1] != 0x0d)
    {
        cout << "onSerialParser:" << TC_Common::bin2str(out.data(), out.size()) << ", not end with 0x0d, rebuild comm!" << endl;
        return TC_NetWorkBuffer::PACKET_ERR;
    }
	buffer.moveHeader(out.size());

	return TC_NetWorkBuffer::PACKET_FULL;
}

TEST_F(UtilSerialPortTest, list)
{
	TC_SerialPortGroup serialPortGroup;
	serialPortGroup.initialize();

	auto comPorts = serialPortGroup.getComPorts();

	cout << TC_Common::tostr(comPorts.begin(), comPorts.end(), ", ") << endl;
}

TEST_F(UtilSerialPortTest, test1)
{
#if TARGET_PLATFORM_WINDOWS    
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 1), &wsadata);
#endif	
	try
	{
        TC_SerialPortGroup serialPortGroup;
        serialPortGroup.initialize();

        TC_SerialPort::Options options;
        
        // options.portName = "/dev/tty.usbmodem00000000050C1";
        options.portName = "//./COM14";

        options.baudRate = 9600;
        options.stopBits = 0;
        options.parity = 0;

        auto callback = make_shared<SerialPortCallback>();
        shared_ptr<TC_SerialPort> serialPort = serialPortGroup.create(options, onParser1, callback);
        callback->_serialPort = serialPort;
        string msg_send = { 0x7e, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, (char)0xab, (char)0xcd };

    // 7e000801000201abcd
        while(true)
        {
            vector<char> response;
            auto status = serialPort->sendRequestAndResponse(msg_send.c_str(), msg_send.size(), response, true, 1000);
            if(status == std::cv_status::timeout)
            {
                cout << "timeout" << endl;
            }
            else
            {
                cout << "response: " << TC_Common::bin2str(response.data(), response.size()) << endl;
            }
            

            TC_Common::sleep(1);
        }
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

TC_NetWorkBuffer::PACKET_TYPE onParser2(TC_NetWorkBuffer &buffer, vector<char> &out)
{
	// LOG_DEBUG << "onSerialParser:" << buffer.getBufferLength() << endl;
	if(buffer.empty() || buffer.getBufferLength() < 4)
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	out = buffer.getBuffers();

	if(out[0] == (char)0x64 || out[0] == 'v' || out[0] == 'p')
	{
		out = vector<char>(out.begin(), out.begin() + 4);
	}
	else if(out[0] == (char)0x2f || out[0] == (char)0x47)
	{
		if(out.size() < 6)
		{
			return TC_NetWorkBuffer::PACKET_LESS;
		}
		out = vector<char>(out.begin(), out.begin() + 6);
	}
	else
	{
		LOG_CONSOLE_DEBUG << "onSerialParser unknown packet size:" << out.size() << ", data:" << TC_Common::bin2str(out.data(), out.size()) << endl;
		return TC_NetWorkBuffer::PACKET_ERR;
	}

	buffer.moveHeader(out.size());
	LOG_CONSOLE_DEBUG << "onSerialParser size:" << out.size() << ", data:" << TC_Common::bin2str(out.data(), out.size()) << endl;
    return TC_NetWorkBuffer::PACKET_FULL;
}

TEST_F(UtilSerialPortTest, test2)
{
#if TARGET_PLATFORM_WINDOWS    
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 1), &wsadata);
#endif	
	try
	{
        TC_SerialPortGroup serialPortGroup;
        serialPortGroup.initialize();

        TC_SerialPort::Options options;
        
        // options.portName = "/dev/tty.usbmodem00000000050C1";
        options.portName = "//./COM11";

        options.baudRate = 115200;
        options.stopBits = 0;
        options.parity = 0;

        shared_ptr<TC_SerialPort> serialPort = serialPortGroup.create(options, onParser2, make_shared<AsyncSerialPortCallback>());
        // string msg_send = {0x2f,0x44,0x1f,0x1f,0x1f,0x23};		
        string msg_send = {0x2f,0x47,0x57,0x00,0x01,0x23};

    // 7e000801000201abcd
        while(true)
        {
            try
            {
                std::unique_lock<std::mutex> lock(mtx);
                serialPort->sendRequest(msg_send.c_str(), msg_send.size());
                cnd.wait_for(lock, std::chrono::seconds(1));
            }
            catch(const std::exception& ex)
            {
                cout << "ex: " << ex.what() << endl;
            }

            TC_Common::sleep(1);
        }
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
