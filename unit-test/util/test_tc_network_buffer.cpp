//
// Created by jarod on 2020/2/20.
//

#include "util/tc_network_buffer.h"
#include "util/tc_logger.h"
#include "gtest/gtest.h"

using namespace tars;

class UtilNetworkBufferTest : public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{
//		cout<<"SetUpTestCase"<<endl;
	}
	static void TearDownTestCase()
	{
//		cout<<"TearDownTestCase"<<endl;
	}
	virtual void SetUp()   //TEST跑之前会执行SetUp
	{
//		cout<<"SetUp"<<endl;
	}
	virtual void TearDown() //TEST跑完之后会执行TearDown
	{
//		cout<<"TearDown"<<endl;
	}
};

TEST_F(UtilNetworkBufferTest, testBuffer1)   //此时使用的是TEST_F宏
{
	uint32_t a = 10;
	vector<char> buffer(a, 'a');

	uint32_t b = htonl(a);

	TC_NetWorkBuffer nb(NULL);
	nb.addBuffer((const char *) &b, sizeof(b));
	nb.addBuffer(buffer);

	uint32_t c = nb.getValueOf4();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(b)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));

	ASSERT_TRUE(buffer == resBuffer);
}

TEST_F(UtilNetworkBufferTest, testBuffer2)   //此时使用的是TEST_F宏
{
	uint32_t a = 10;
	vector<char> buffer(a, 'a');

	uint32_t b = htonl(a);

	TC_NetWorkBuffer nb(NULL);
	nb.addBuffer((const char *) &b, 1);
	nb.addBuffer((const char *) &b + 1, 3);
	nb.addBuffer(buffer);

	uint32_t c = nb.getValueOf4();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(b)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));
	ASSERT_TRUE(buffer == resBuffer);
}

TEST_F(UtilNetworkBufferTest, testBuffer3)   //此时使用的是TEST_F宏
{
	uint32_t a = 10;
	vector<char> buffer(a, 'a');

	uint32_t b = htonl(a);

	TC_NetWorkBuffer nb(NULL);
	nb.addBuffer((const char *) &b, 3);
	nb.addBuffer((const char *) &b + 3, 1);
	nb.addBuffer(buffer.data(), 5);
	nb.addBuffer(buffer.data() + 5, a - 5);

	uint32_t c = nb.getValueOf4();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(c)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));
	ASSERT_TRUE(buffer == resBuffer);
}

TEST_F(UtilNetworkBufferTest, testBuffer4)   //此时使用的是TEST_F宏
{
	uint32_t a = 10;
	vector<char> buffer(a, 'a');

	uint32_t b = htonl(a);

	TC_NetWorkBuffer nb(NULL);
	nb.addBuffer((const char*)&b, 2);
	nb.addBuffer((const char*)&b + 2, 1);
	nb.addBuffer((const char*)&b + 3, 1);
	nb.addBuffer(buffer.data(), 3);
	nb.addBuffer(buffer.data() + 3, 4);
	nb.addBuffer(buffer.data() + 3, a - 3 - 4);

	uint32_t c = nb.getValueOf4();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(c)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));
	ASSERT_TRUE(buffer == resBuffer);
}

TEST_F(UtilNetworkBufferTest, testBuffer5)   //此时使用的是TEST_F宏
{
	uint16_t a = 10;
	vector<char> buffer(a, 10);

	uint16_t b = htons(a);

	TC_NetWorkBuffer nb(NULL);
	nb.addBuffer((const char *) &b, sizeof(b));
	nb.addBuffer(buffer);

	uint16_t c = nb.getValueOf2();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(b)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));
	ASSERT_TRUE(buffer == resBuffer);

}

TEST_F(UtilNetworkBufferTest, testBuffer6)   //此时使用的是TEST_F宏
{
	uint16_t a = 10;
	vector<char> buffer(a, 10);

	uint16_t b = htons(a);

	TC_NetWorkBuffer nb(NULL);
	nb.addBuffer((const char*)&b, 1);
	nb.addBuffer((const char*)&b + 1, 1);
	nb.addBuffer(buffer);

	uint16_t c = nb.getValueOf2();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(b)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));
	ASSERT_TRUE(buffer == resBuffer);
}

TEST_F(UtilNetworkBufferTest, testBuffer7)   //此时使用的是TEST_F宏
{
	uint16_t a = 10;
	vector<char> buffer(a, 10);

	uint16_t b = htons(a);

	TC_NetWorkBuffer nb(NULL);

	nb.addBuffer((const char*)&b, 2);
	nb.addBuffer(buffer.data(), 5);
	nb.addBuffer(buffer.data() + 5, a - 5);

	uint16_t c = nb.getValueOf2();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(c)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));
	ASSERT_TRUE(buffer == resBuffer);
}

TEST_F(UtilNetworkBufferTest, testBuffer8)   //此时使用的是TEST_F宏
{
	uint16_t a = 10;
	vector<char> buffer(a, 10);

	uint16_t b = htons(a);

	TC_NetWorkBuffer nb(NULL);
	nb.addBuffer((const char*)&b, 1);
	nb.addBuffer((const char*)&b + 1, 1);
	nb.addBuffer(buffer.data(), 3);
	nb.addBuffer(buffer.data() + 3, 4);
	nb.addBuffer(buffer.data() + 3, a - 3 - 4);

	uint16_t c = nb.getValueOf2();

	ASSERT_TRUE(c == a);

	ASSERT_TRUE(nb.moveHeader(sizeof(c)));

	vector<char> resBuffer;
	ASSERT_TRUE(nb.getHeader(c, resBuffer));
	ASSERT_TRUE(buffer == resBuffer);
}

TEST_F(UtilNetworkBufferTest, testIterator)
{
	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer("GET www.qq.com/a/b?name=value&ccc=ddd HTTP/1.1\r\n");
	buff.addBuffer("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	buff.addBuffer("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	buff.addBuffer("Accept-Encoding: gzip\r\n");
	buff.addBuffer("Connection: close\r\n");
	buff.addBuffer("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	TC_NetWorkBuffer::buffer_iterator it = buff.begin();

//	cout << "testIterator-------------------------------------" << endl;
	ostringstream  os;
	while(it != buff.end())
	{
		os << *it;
		++it;
	}

	ASSERT_TRUE(os.str() == buff.getBuffersString());
}

TEST_F(UtilNetworkBufferTest, testIteratorRandom)
{
	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer("GET www.qq.com/a/b?name=value&ccc=ddd HTTP/1.1\r\n");
	buff.addBuffer("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	buff.addBuffer("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	buff.addBuffer("Accept-Encoding: gzip\r\n");
	buff.addBuffer("Connection: close\r\n");
	buff.addBuffer("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

//	cout << "testIteratorRandom-------------------------------------" << endl;

	TC_NetWorkBuffer::buffer_iterator it = buff.begin();

//	for(size_t i = 0; i < buff.getBufferLength(); ++i)
//	{
//		cout << *(it + i);
//	}

	ASSERT_TRUE((it + 10) - (it + 5) == 5);
	ASSERT_TRUE((it + 44) - (it + 12 ) == 32);
	ASSERT_TRUE((it + 54) - (it + 2) == 52);
	ASSERT_TRUE((it + 111111) == buff.end());
	ASSERT_TRUE((it - 111111) == buff.end());

}

TEST_F(UtilNetworkBufferTest, testFind)
{
	vector<string> sbuff;
	sbuff.push_back("GET www.qq.com/a/b?name=value&ccc=ddd HTTP/1.1\r\n");
	sbuff.push_back("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	sbuff.push_back("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	sbuff.push_back("Accept-Encoding: gzip\r\n");
	sbuff.push_back("Connection: close\r\n");
	sbuff.push_back("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	string header;
	for(auto s : sbuff)
	{
		header += s;
	}

	string s = header + "abccccddddddd";

	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer(s.c_str(), s.size());

	string headerBuf;

	headerBuf = buff.getPrefixBuffer<string>("xxx", 3);
	ASSERT_TRUE(headerBuf.empty());

	string find = "Accept-Encoding:";
	auto it = std::search(buff.begin(), buff.end(), find.c_str(), find.c_str() + find.size());
	ASSERT_TRUE(it != buff.end());
	ASSERT_TRUE(it - buff.begin() == sbuff[0].size() + sbuff[1].size() + sbuff[2].size());

	find = "accept-encoding:";
	it = std::search(buff.begin(), buff.end(), find.c_str(), find.c_str() + find.size(), [](char x, char y){return toupper(x) == toupper(y);});
	ASSERT_TRUE(it != buff.end());
	ASSERT_TRUE(it - buff.begin() == sbuff[0].size() + sbuff[1].size() + sbuff[2].size());

	string sep = "\r\n\r\n";

	headerBuf = buff.getPrefixBuffer<string>(sep.c_str(), sep.size());

	ASSERT_TRUE(header == headerBuf + sep);

	sep = "\r\n";
	headerBuf = buff.getPrefixBuffer<string>(sep.c_str(), sep.size());
	ASSERT_TRUE(sbuff[0] == headerBuf + sep);
	buff.moveHeader(sbuff[0].size());

	headerBuf = buff.getPrefixBuffer<string>(sep.c_str(), sep.size());
	ASSERT_TRUE(sbuff[1] == headerBuf + sep);
	buff.moveHeader(sbuff[1].size());

	headerBuf = buff.getPrefixBuffer<string>(sep.c_str(), sep.size());
	ASSERT_TRUE(sbuff[2] == headerBuf + sep);
	buff.moveHeader(sbuff[2].size());

	headerBuf = buff.getPrefixBuffer<string>(sep.c_str(), sep.size());
	ASSERT_TRUE(sbuff[3] == headerBuf + sep);
	buff.moveHeader(sbuff[3].size());
}

TEST_F(UtilNetworkBufferTest, testAlloc)
{
	string buffer(100, 'a' );
	TC_NetWorkBuffer::Buffer buff;
	buff.addBuffer(buffer);

	ASSERT_TRUE(buff.capacity() == buffer.size());
	ASSERT_TRUE(buff.readIdx() == 0);
	ASSERT_TRUE(buff.writeIdx() == buffer.size());
	ASSERT_TRUE(buff.length() == buffer.size());
	ASSERT_TRUE(buff.left() == 0);

	buff.addReadIdx(10);

	ASSERT_TRUE(buff.readIdx() == 10);
	ASSERT_TRUE(buff.length() == buffer.size()-10);

	buff.compact();
	ASSERT_TRUE(buff.readIdx() == 0);
	ASSERT_TRUE(buff.writeIdx() == buffer.size() - 10);
	ASSERT_TRUE(buff.length() == buffer.size()-10);

	buff.addReadIdx(15);
	buff.expansion(120);
	ASSERT_TRUE(buff.readIdx() == 0);
	ASSERT_TRUE(buff.writeIdx() == buffer.size() - 10 - 15);

	buff.addBuffer(buffer);
	ASSERT_TRUE(buff.capacity() == 175);
	ASSERT_TRUE(buff.readIdx() == 0);

}

TEST_F(UtilNetworkBufferTest, testGetOrCreateBuffer)
{
	TC_NetWorkBuffer buff(NULL);

	shared_ptr<TC_NetWorkBuffer::Buffer> data;

	{
		data = buff.getOrCreateBuffer(100, 300);

		ASSERT_TRUE(buff.listSize() == 1);
		ASSERT_TRUE(data->capacity() == 300);
		ASSERT_TRUE(data->readIdx() == 0);
		ASSERT_TRUE(data->length() == 0);

	}

	LOG_CONSOLE_DEBUG << endl;
	{
		data->addWriteIdx(50);

		data = buff.getOrCreateBuffer(100, 300);

		ASSERT_TRUE(buff.listSize() == 1);
		ASSERT_TRUE(data->capacity() == 300);
		ASSERT_TRUE(data->readIdx() == 0);
		ASSERT_TRUE(data->length() == 50);

	}

	{
		data->addWriteIdx(200);
		data->addReadIdx(200);

		data = buff.getOrCreateBuffer(100, 300);

		ASSERT_TRUE(buff.listSize() == 1);
		ASSERT_TRUE(data->capacity() == 300);
		ASSERT_TRUE(data->readIdx() == 0);
		ASSERT_TRUE(data->length() == 50);

	}

	{
		data->addWriteIdx(200);
		data = buff.getOrCreateBuffer(100, 300);

		ASSERT_TRUE(buff.listSize() == 2);
		ASSERT_TRUE(data->capacity() == 300);
		ASSERT_TRUE(data->readIdx() == 0);
		ASSERT_TRUE(data->length() == 0);

	}

	{
		buff.clearBuffers();
		ASSERT_TRUE(buff.listSize() == 0);
	}

	buff.clearBuffers();

	string a(794, 'a');
	buff.addBuffer(a.c_str(), a.size());

	string b(6307, 'b');
	buff.addBuffer(b.c_str(), b.size());
}

TEST_F(UtilNetworkBufferTest, testCompact)
{
	srand(time(NULL));

	shared_ptr<TC_NetWorkBuffer::Buffer> data = std::make_shared<TC_NetWorkBuffer::Buffer>();

	vector<char> buff;

	for (size_t i = 0; i < 50; i++)
	{
		buff.push_back('a' + rand() % 26);
	}
	data->addBuffer(buff);

	data->addReadIdx(10);

	data->compact();

	ASSERT_TRUE(TC_Port::strncasecmp(data->buffer(), &buff[10], data->length()) == 0);
}