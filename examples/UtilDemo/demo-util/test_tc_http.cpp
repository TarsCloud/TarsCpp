//
// Created by jarod on 2020/2/20.
//

#include "util/tc_http.h"
#include "util/tc_common.h"
#include "util/tc_file.h"
#include "gtest/gtest.h"

using namespace tars;

class HttpTest : public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{
		cout<<"SetUpTestCase"<<endl;
	}
	static void TearDownTestCase()
	{
		cout<<"TearDownTestCase"<<endl;
	}
	virtual void SetUp()   //TEST跑之前会执行SetUp
	{
		cout<<"SetUp"<<endl;
	}
	virtual void TearDown() //TEST跑完之后会执行TearDown
	{
		cout<<"TearDown"<<endl;
	}
};

TEST_F(HttpTest, testCheckRequestURL)   //此时使用的是TEST_F宏
{
	string s = string("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	TC_HttpRequest req;
	ASSERT_TRUE(req.decode(s));
	ASSERT_TRUE(req.getRequestUrl() == "/a/b");
	ASSERT_TRUE(req.getURL().getDomain() == "www.qq.com");
}

TEST_F(HttpTest, testHttp)   //此时使用的是TEST_F宏
{
	TC_HttpRequest stHttpReq;
	stHttpReq.setCacheControl("no-cache");
	stHttpReq.setGetRequest("http://118.89.197.217/", true);
	TC_HttpResponse stHttpRsp;
	int ret = stHttpReq.doRequest(stHttpRsp, 3000);

	cout << ret << ":" << stHttpRsp.getContent() << endl;

	cout << "\n\n==============================\n" << stHttpRsp.getContent().size() << endl;
	cout << stHttpRsp.genHeader() << endl;
}

TEST_F(HttpTest, testEncodeString)   //此时使用的是TEST_F宏
{
	string s = string("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

//	s += string("a", 1024);

	TC_HttpRequest req;
	req.decode(s);
	int64_t t = TC_Common::now2us();

	int count = 100000;
	int i = 0;
	while(++i<count) {
		string s;
		s = req.encode();
	}

	ASSERT_TRUE(req.decode(s));
	ASSERT_TRUE(req.getRequestUrl() == "/a/b");
	ASSERT_TRUE(req.getURL().getDomain() == "www.qq.com");

	cout << "testEncodeString::cost: " << TC_Common::now2us() - t << "us, " << 1.*(TC_Common::now2us() - t)/count << "us" << endl;
}

TEST_F(HttpTest, testEncodeVector)   //此时使用的是TEST_F宏
{
	string s = string("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	s += string("a", 1024);

	TC_HttpRequest req;
	req.decode(s);
	int64_t t = TC_Common::now2us();

	int count = 100000;
	int i = 0;

	while(++i<count) {
		vector<char> buff;

		req.encode(buff);
	}

	cout << "testEncodeVector::cost: " << TC_Common::now2us() - t << "us, " << 1.*(TC_Common::now2us() - t)/count << "us" << endl;
}

TEST_F(HttpTest, testEncodeBuffString)   //此时使用的是TEST_F宏
{
	string s = string("GET /a/b?name=value&ccc=ddd HTTP/1.1\\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	int64_t t = TC_Common::now2us();

	TC_HttpRequest req;
	req.decode(s);

	TC_NetWorkBuffer buff(NULL);

	int count = 100000;
	int i = 0;
	while(++i<count) {
		req.encode(buff);
	}

	cout << "testEncodeBuffString::cost: " << TC_Common::now2us() - t << "us, " << 1.*(TC_Common::now2us() - t)/count << "us" << endl;
}

TEST_F(HttpTest, testDecodeString)   //此时使用的是TEST_F宏
{
	string s = string("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	int64_t t = TC_Common::now2us();

	int count = 100000;
	int i = 0;
	while(++i<count) {
		TC_HttpRequest req;
		req.decode(s);
	}

	cout << "testDecodeString::cost: " << TC_Common::now2us() - t << "us, " << 1.*(TC_Common::now2us() - t)/count << "us" << endl;
}

TEST_F(HttpTest, testDecodeBuffString)   //此时使用的是TEST_F宏
{
	string s = string("HTTP/1.1 200 OK\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	int count = 100000;
	int i = 0;

	vector<TC_NetWorkBuffer> vbuff;
	while(i<count) {
		TC_NetWorkBuffer buff(NULL);

		buff.addBuffer(s);

		vbuff.push_back(buff);

		++i;
	}

	int64_t t = TC_Common::now2us();
	i = 0;
	while(i<count) {
		TC_HttpResponse req;

		ASSERT_TRUE(req.incrementDecode(vbuff[i]));

		++i;
	}

	cout << "testDecodeBuffString::cost: " << TC_Common::now2us() - t << "us, " << 1.*(TC_Common::now2us() - t)/count << "us" << endl;
}

TEST_F(HttpTest, testCheckRequestString)   //此时使用的是TEST_F宏
{
	string s = string("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	int64_t t = TC_Common::now2us();

	TC_HttpRequest req;
	int count = 100000;
	int i = 0;
	while(++i<count) {
		ASSERT_TRUE(req.checkRequest(s.c_str(), s.size()));
	}

	ASSERT_TRUE(req.decode(s));
	ASSERT_TRUE(req.getRequestUrl() == "/a/b");
	cout << req.getURL().getDomain() << endl;
	ASSERT_TRUE(req.getURL().getDomain() == "www.qq.com");

	cout << "testCheckRequestString::cost: " << TC_Common::now2us() - t << "us, " << 1.*(TC_Common::now2us() - t)/count << "us" << endl;
}

TEST_F(HttpTest, testCheckRequestBuff)   //此时使用的是TEST_F宏
{
	string s = string("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n")
		+string("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n")
		+string("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n")
		+string("Accept-Encoding: gzip\r\n")
		+string("Accept-Language: zh-cn,zh;q=0.5\r\n")
		+string("Connection: close\r\n")
		+string("Host: www.qq.com\r\n")
		+string("Q-GUID: 08f0373a192a45778cc8567d1c641475\r\n")
		+string("Q-UA: SQB12_GA/120450&SMTT_3/020100&SYM3&201514&E71&V2\r\n")
		+string("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	int64_t t = TC_Common::now2us();

	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer(s.c_str(), s.size());

	TC_HttpRequest req;
	int count = 100000;
	int i = 0;
	while(++i<count) {
		ASSERT_TRUE(req.checkRequest(buff));
	}

	cout << "testCheckRequestBuff::cost: " << TC_Common::now2us() - t << "us, " << 1.*(TC_Common::now2us() - t)/count << "us" << endl;
}

TEST_F(HttpTest, testHttpFinish)   //此时使用的是TEST_F宏
{
	string body = "abdefghigk";

	vector<string> sbuff;
	sbuff.push_back("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n");
	sbuff.push_back("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	sbuff.push_back("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	sbuff.push_back("Accept-Encoding: gzip\r\n");
	sbuff.push_back("Content-Length: " + TC_Common::tostr(body.size()) + "\r\n");
	sbuff.push_back("Connection: close\r\n");
	sbuff.push_back("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	string header;
	for(auto s : sbuff)
	{
		header += s;
	}

	string s = header + body;

	TC_HttpRequest request;
	ASSERT_TRUE(request.checkRequest(s.c_str(), s.size()));

	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer(s.c_str(), s.size());

	ASSERT_TRUE(buff.checkHttp() == TC_NetWorkBuffer::PACKET_FULL);
}

TEST_F(HttpTest, testHttpFinishNoLength)   //此时使用的是TEST_F宏
{
	vector<string> sbuff;
	sbuff.push_back("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n");
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

	string s = header;

	TC_HttpRequest request;
	ASSERT_TRUE(request.checkRequest(s.c_str(), s.size()));

	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer(s.c_str(), s.size());

	ASSERT_TRUE(buff.checkHttp() == TC_NetWorkBuffer::PACKET_FULL);
}

TEST_F(HttpTest, testHttpNoFinish)   //此时使用的是TEST_F宏
{
	string body = "abdefghigk";

	vector<string> sbuff;
	sbuff.push_back("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n");
	sbuff.push_back("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	sbuff.push_back("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	sbuff.push_back("Accept-Encoding: gzip\r\n");
	sbuff.push_back("Content-Length: " + TC_Common::tostr(body.size() + 1) + "\r\n");
	sbuff.push_back("Connection: close\r\n");
	sbuff.push_back("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	string header;
	for(auto s : sbuff)
	{
		header += s;
	}

	string s = header + body;

	TC_HttpRequest request;
	ASSERT_TRUE(!request.checkRequest(s.c_str(), s.size()));

	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer(s.c_str(), s.size());

	ASSERT_TRUE(buff.checkHttp() == TC_NetWorkBuffer::PACKET_LESS);
}

TEST_F(HttpTest, testHttpRequestChunked)   //此时使用的是TEST_F宏
{
	vector<string> body;
	body.push_back("abasdfadefghiadfagk1");
	body.push_back("abdasdfadfaefghigk2");
	body.push_back("abdsaefghigk3");
	body.push_back("abdeasdfafasfasfasfasdfasffghigk4");

	vector<string> sbuff;
	sbuff.push_back("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n");
	sbuff.push_back("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	sbuff.push_back("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	sbuff.push_back("Accept-Encoding: gzip\r\n");
	sbuff.push_back("Transfer-Encoding: chunked\r\n");
	sbuff.push_back("Connection: close\r\n");
	sbuff.push_back("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	stringstream data;
	for(auto s : sbuff)
	{
		data << s;
	}

	string sbody;
	for(auto s : body)
	{
		sbody += s;
		data << hex << s.size() << "\r\n" << s << "\r\n";
	}
	data << 0 << "\r\n\r\n";

	string s = data.str();

	TC_HttpRequest request;
	ASSERT_TRUE(request.checkRequest(s.c_str(), s.size()));
	ASSERT_TRUE(request.decode(s));

	ASSERT_TRUE(request.getContent() == sbody);

	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer(s.c_str(), s.size());

	request.reset();
	ASSERT_TRUE(request.checkRequest(buff));

}

TEST_F(HttpTest, testHttpRequestChunkedNoFinish)   //此时使用的是TEST_F宏
{
	vector<string> body;
	body.push_back("abasdfadefghiadfagk1");
	body.push_back("abdasdfadfaefghigk2");
	body.push_back("abdsaefghigk3");
	body.push_back("abdeasdfafasfasfasfasdfasffghigk4");

	vector<string> sbuff;
	sbuff.push_back("GET /a/b?name=value&ccc=ddd HTTP/1.1\r\n");
	sbuff.push_back("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	sbuff.push_back("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	sbuff.push_back("Accept-Encoding: gzip\r\n");
	sbuff.push_back("Transfer-Encoding: chunked\r\n");
	sbuff.push_back("Connection: close\r\n");
	sbuff.push_back("User-Agent: E71/SymbianOS/9.1 Series60/3.0\r\n\r\n");

	stringstream data;
	for(auto s : sbuff)
	{
		data << s;
	}

	data << hex << body[0].size() << "\r\n" << body[0] << "\r\n";
	data << hex << body[1].size() << "\r\n" << body[1] << "\r\n";
	data << hex << body[2].size() ;

	string s = data.str();

	TC_HttpRequest request;
	ASSERT_TRUE(!request.checkRequest(s.c_str(), s.size()));

	TC_NetWorkBuffer buff(NULL);
	buff.addBuffer(s.c_str(), s.size());

	ASSERT_TRUE(!request.checkRequest(buff));

}

TEST_F(HttpTest, testHttpResponse)   //此时使用的是TEST_F宏
{
	string body = "abcdef";

	vector<string> sbuff;
	sbuff.push_back("HTTP/1.1 200 OK\r\n");
	sbuff.push_back("Content-Length: " + TC_Common::tostr(body.size()) + "\r\n");
	sbuff.push_back("\r\n");

	string header;
	for(auto s : sbuff)
	{
		header += s;
	}

	string s = header + body;

	{
		TC_HttpResponse response;
		ASSERT_TRUE(response.decode(s));
		ASSERT_TRUE(response.getContent() == body);

	}

	{
		TC_HttpResponse response;
		TC_NetWorkBuffer buff(NULL);
		buff.addBuffer(s.c_str(), s.size());

		ASSERT_TRUE(response.incrementDecode(buff));
		ASSERT_TRUE(response.getContent() == body);

	}
}

TEST_F(HttpTest, testHttpResponseNoFinish)   //此时使用的是TEST_F宏
{
	string body = "abcdef";

	vector<string> sbuff;
	sbuff.push_back("HTTP/1.1 200 OK\r\n");
	sbuff.push_back("Content-Length: " + TC_Common::tostr(body.size() + 1) + "\r\n");
	sbuff.push_back("\r\n");

	string header;
	for(auto s : sbuff)
	{
		header += s;
	}

	string s = header + body;

	{
		TC_HttpResponse response;
		ASSERT_TRUE(!response.decode(s));
	}

	{
		TC_HttpResponse response;
		TC_NetWorkBuffer buff(NULL);
		buff.addBuffer(s.c_str(), s.size());

		ASSERT_TRUE(!response.incrementDecode(buff));
	}
}

TEST_F(HttpTest, testHttpResponseNoLength)   //此时使用的是TEST_F宏
{
	vector<string> sbuff;
	sbuff.push_back("HTTP/1.1 200 OK\r\n");
	sbuff.push_back("Connection: close\r\n");
	sbuff.push_back("\r\n");

	string header;
	for(auto s : sbuff)
	{
		header += s;
	}

	string s = header;

	{
		TC_HttpResponse response;
		ASSERT_TRUE(response.decode(s));
	}

	{
		TC_HttpResponse response;
		TC_NetWorkBuffer buff(NULL);
		buff.addBuffer(s.c_str(), s.size());

		ASSERT_TRUE(response.incrementDecode(buff));
	}
}

TEST_F(HttpTest, testHttpResponseIncrementFinish)   //此时使用的是TEST_F宏
{
	string body = "abcdeasdfadfsff";

	vector<string> sbuff;
	sbuff.push_back("HTTP/1.1 200 OK\r\n");
	sbuff.push_back("Content-Length: " + TC_Common::tostr(body.size()) + "\r\n");
	sbuff.push_back("\r\n");

	string header;
	for(auto s : sbuff)
	{
		header += s;
	}

	string s = header;

	{
		TC_HttpResponse response;
		ASSERT_TRUE(!response.decode(s));
	}

	{
		TC_HttpResponse response;
		TC_NetWorkBuffer buff(NULL);
		buff.addBuffer(s.c_str(), s.size());

		ASSERT_TRUE(!response.incrementDecode(buff));
	}

	s = header + body;
	{
		TC_HttpResponse response;
		ASSERT_TRUE(response.decode(s));
	}

	{
		TC_HttpResponse response;
		TC_NetWorkBuffer buff(NULL);
		buff.addBuffer(s.c_str(), s.size());

		ASSERT_TRUE(response.incrementDecode(buff));
	}

}

TEST_F(HttpTest, testHttpResponseChunked)   //此时使用的是TEST_F宏
{
	vector<string> body;
	body.push_back("abdefghiasdfasdfsadfsadfsagk1");
	body.push_back("abdefghasdfaaigk2");
	body.push_back("abdefghigadsfadsfk3");
	body.push_back("abdefgsfagasasdfasfdfdfsdfsfsdfdsffsdfsdfhigk4");

	vector<string> sbuff;
	sbuff.push_back("HTTP/1.1 200 OK\r\n");
	sbuff.push_back("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	sbuff.push_back("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	sbuff.push_back("Transfer-Encoding: chunked\r\n");
	sbuff.push_back("Connection: close\r\n\r\n");

	stringstream data;
	for(auto s : sbuff)
	{
		data << s;
	}

	string sbody;
	for(auto s : body)
	{
		sbody += s;

		data << hex << s.size() << "\r\n" << s << "\r\n";
	}
	data << 0 << "\r\n\r\n";

	string s = data.str();

	{
		TC_HttpResponse response;
		ASSERT_TRUE(response.decode(s));

		ASSERT_TRUE(response.getContent() == sbody);
	}

	{
		TC_HttpResponse response;
		TC_NetWorkBuffer buff(NULL);
		buff.addBuffer(s.c_str(), s.size());

		ASSERT_TRUE(response.incrementDecode(buff));

		ASSERT_TRUE(response.getContent() == sbody);
	}
}

TEST_F(HttpTest, testHttpResponseChunkedNoFinish)   //此时使用的是TEST_F宏
{
	vector<string> body;
	body.push_back("abdefasdfasfasghigk1");
	body.push_back("asdfaabdeafghigk2");
	body.push_back("abasdfasdfasdfasdfasdfasdfasfasdefghigk3");
	body.push_back("abdefgfasdfasdfasdfasdfadfigk4");

	vector<string> sbuff;
	sbuff.push_back("HTTP/1.1 200 OK\r\n");
	sbuff.push_back("Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n");
	sbuff.push_back("Accept-Charset: utf-8,gb2321;q=0.7,*;q=0.7\r\n");
	sbuff.push_back("Transfer-Encoding: chunked\r\n");
	sbuff.push_back("Connection: close\r\n\r\n");

	stringstream data;
	for(auto s : sbuff)
	{
		data << s;
	}

	data << hex << body[0].size() << "\r\n" << body[0] << "\r\n";
	data << hex << body[1].size() << "\r\n" << body[1] << "\r\n";
	data << hex << body[2].size() << "\r\n" << "abc";

	string s = data.str();

	{
		TC_HttpResponse response;
		ASSERT_TRUE(!response.decode(s));

	}

	{
		TC_HttpResponse response;

		TC_NetWorkBuffer buff(NULL);
		buff.addBuffer(s.c_str(), s.size());

		ASSERT_TRUE(!response.incrementDecode(buff));
	}
}

TEST_F(HttpTest, testWeb)   //此时使用的是TEST_F宏
{
	string url = "www.qq.com";
	TC_HttpRequest stHttpReq;
//	stHttpReq.setCacheControl("no-cache");
//	stHttpReq.setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36");
	stHttpReq.setUserAgent("E71/SymbianOS/9.1 Series60/3.0");
	stHttpReq.setHeader("Connection", "Close");
	stHttpReq.setAcceptEncoding("gzip, deflate, br");
	stHttpReq.setGetRequest(url);

	string sSendBuffer = stHttpReq.encode();

	TC_HttpResponse stHttpRsp;
	int iRet = stHttpReq.doRequest(stHttpRsp, 5000);
	if(iRet != 0)
	{
		cout << iRet << endl;
	}

	string file = stHttpReq.getURL().getDomain() + ".html";

	auto headers = stHttpRsp.getHeaders();

	cout << "request:" << url << endl;
	cout << TC_Common::tostr(headers.begin(), headers.end(), "\r\n") << endl;
	TC_File::save2file(file, stHttpRsp.getContent());
	
//	ASSERT_TRUE(stHttpRsp.getContentLength() == TC_File::getFileSize(file));
}

void testCookie(const string &sRspURL, const string &sReqURL, const vector<string> &vsCookie)
{
	cout << sRspURL << "=>" << sReqURL << "-----------------------------------" << endl;

	TC_HttpCookie cookie;

	cookie.addCookie(sRspURL, vsCookie);

	list<TC_HttpCookie::Cookie> vCookie = cookie.getAllCookie();

	cout << "All Cookie:" << sRspURL << "-----------------------------------" << endl;

	list<TC_HttpCookie::Cookie>::iterator it = vCookie.begin();

	while(it != vCookie.end())
	{
		cout << TC_Common::tostr(it->_data.begin(), it->_data.end(), "; ") << ", " << it->_expires << ", " << it->_path << endl;

		++it;
	}

	cout << "-----------------------------------" << endl << endl;

	string sCookie;

	cookie.getCookieForURL(sReqURL, sCookie);

	cout << TC_Common::tostr(sCookie) << endl;

	cout << "-----------------------------------" << endl << endl;
}

TEST_F(HttpTest, testCookie)   //此时使用的是TEST_F宏
{
	ASSERT_TRUE(TC_HttpCookie::matchDomain("qq.com", "www.qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".qq.com", "www.qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".qq.com", "qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain("t.qq.com", "www.qq.com") == false);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".t.qq.com", "www.qq.com") == false);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".t.qq.com", "t.qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".com", "www.qq.com") == false);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".com", "qq.com") == false);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".y.qq.com", "x.y.qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".x.y.qq.com", "x.y.qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".qq.com", "x.y.qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain(".qq.com", "y.qq.com") == true);
	ASSERT_TRUE(TC_HttpCookie::matchDomain("qq.com", "y.qq.com") == true);

	cout << TC_Common::now2GMTstr() << endl;

	string gmt = TC_Common::tm2GMTstr(time(NULL) + 10);

	string s = "HTTP/1.1  200  OK\r\n";//  200  Aouut Error\r\n";
	s += "Set-Cookie: n1=1; a=1; c=d; Path=/; Domain=qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n2=2; a=0; c=d; Path=/abc/def; Domain=.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n3=3; a=5; c=d; Path=/abc/def/aaa; Domain=.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n4=4; a=6; c=d; Path=/abc; Domain=.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n5=5; a=2; c=d; Path=/; Domain=.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n6=6; c=3; Path=/; Domain=y.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n7=7; c=3; Path=/abc; Domain=.y.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n8=6; c=3; Path=/; Domain=x.y.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n9=7; c=4; Path=/; Domain=.x.y.qq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n10=7; c=4; Path=/; Domain=qqq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n11=7; c=4; Path=/; Domain=.qqq.com; Expires=" + gmt + "\r\n";
	s += "Set-Cookie: n12=8; c=4; Expires=" + gmt + "\r\n";
	s += "Accept-Ranges: bytes\r\n\r\n";

	TC_HttpResponse rsp;
	rsp.decode(s);

	cout << "-----------------------------------" << endl;

	vector<string> vsCookie = rsp.getSetCookie();

	cout << TC_Common::tostr(vsCookie.begin(), vsCookie.end(), "\r\n") << endl << endl;

	testCookie("http://www.qq.com", "http://www.qq.com", vsCookie);
	testCookie("http://www.qq.com/abc/def", "http://www.qq.com", vsCookie);
	testCookie("http://www.qq.com/abc/def", "http://www.qq.com/abc", vsCookie);

	cout << endl;
	testCookie("http://www.qq.com", "http://qq.com", vsCookie);
	testCookie("http://www.qq.com/abc/def/aaa", "http://www.qq.com/abc/def/aaa", vsCookie);
	testCookie("http://www.qq.com/abc/def/aaa", "http://www.qq.com", vsCookie);
	testCookie("http://www.qq.com", "http://www.qq.com/abc/def", vsCookie);
	testCookie("http://qq.com", "http://qq.com/abc/def", vsCookie);
	testCookie("http://qq.com", "http://t.qq.com/abc/def", vsCookie);
	testCookie("http://qq.com", "http://y.qq.com/", vsCookie);
	testCookie("http://qq.com", "http://y.qq.com/abc", vsCookie);
	testCookie("http://x.y.qq.com", "http://x.y.qq.com", vsCookie);
}
//
//void Test_TC_Http::main()
//{
//	testCheckRequestURL();
//
//	testEncodeString();
//	testEncodeVector();
//	testEncodeBuffString();
//
//	testDecodeString();
//
//	testDecodeBuffString();
//
//	testCheckRequestString();
//	testCheckRequestBuff();
//
//	testHttpFinishNoLength();
//
//	testHttpFinish();
//
//	testHttpNoFinish();
//
//	testHttpRequestChunked();
//
//	testHttpResponse();
//
//	testHttpResponseNoFinish();
//
//	testHttpResponseNoLength();
//
//	testHttpResponseIncrementFinish();
//
//	testHttpResponseChunked();
//
//	testHttpRequestChunkedNoFinish();
//
//	testHttpResponseChunkedNoFinish();
//
////	testWeb("http://www.baidu.com");
////	testWeb("http://www.httpwatch.com/httpgallery/chunked/chunkedimage.aspx");
////	testWeb("http://www.qq.com");
//
////	testCookie();
//}
