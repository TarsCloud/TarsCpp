#include "util/tc_common.h"
#include "util/tc_serialport.h"
#include "util/tc_network_buffer.h"
#include "util/tc_common.h"
#include "util/tc_serialport.h"
// #include "Serial.h"
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

class SerialPortCallback : public TC_SerialPort::RequestCallback
{
public:
	void onSucc(const vector<char> &data)
	{
        std::unique_lock<std::mutex> lock(mtx);
        cout << "data: " << TC_Common::bin2str(data.data(), data.size()) << endl;
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
};

TC_NetWorkBuffer::PACKET_TYPE onParser(TC_NetWorkBuffer &buffer, vector<char> &data)
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

TEST_F(UtilSerialPortTest, list)
{
	TC_SerialPortGroup serialPortGroup;
	serialPortGroup.initialize();

	auto comPorts = serialPortGroup.getComPorts();

	cout << TC_Common::tostr(comPorts.begin(), comPorts.end(), ", ") << endl;
}

TEST_F(UtilSerialPortTest, test)
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

        shared_ptr<TC_SerialPort> serialPort = serialPortGroup.create(options, onParser, make_shared<SerialPortCallback>());
        string msg_send = { 0x7e, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, (char)0xab, (char)0xcd };

    // 7e000801000201abcd
        while(true)
        {
            try
            {
                std::unique_lock<std::mutex> lock(mtx);
                serialPort->sendRequest(msg_send);
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
