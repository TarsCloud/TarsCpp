
#include "hello_test.h"


/*
 响应包解码函数，根据特定格式解码从服务端收到的数据，解析为BasePacket
*/
static TC_NetWorkBuffer::PACKET_TYPE pushResponse(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	size_t len = sizeof(tars::Int32);

	if (in.getBufferLength() < len)
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	string header;
	in.getHeader(len, header);

	assert(header.size() == len);

	tars::Int32 iHeaderLen = 0;

	::memcpy(&iHeaderLen, header.c_str(), sizeof(tars::Int32));

	iHeaderLen = ntohl(iHeaderLen);

	//做一下保护,长度大于M
	if (iHeaderLen > 100000 || iHeaderLen < (int)sizeof(unsigned int))
	{
		throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
	}

	//包没有接收全
	if (in.getBufferLength() < (uint32_t)iHeaderLen)
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	in.moveHeader(sizeof(iHeaderLen));

	tars::Int32 iRequestId = 0;
	string sRequestId;
	in.getHeader(sizeof(iRequestId), sRequestId);
	in.moveHeader(sizeof(iRequestId));

	rsp.iRequestId = ntohl(*((unsigned int *)(sRequestId.c_str())));
	len =  iHeaderLen - sizeof(iHeaderLen) - sizeof(iRequestId);
	in.getHeader(len, rsp.sBuffer);
	in.moveHeader(len);

	return TC_NetWorkBuffer::PACKET_FULL;
}
/*
   请求包编码函数，本函数的打包格式为
   整个包长度（4字节）+iRequestId（4字节）+包内容
*/
static shared_ptr<TC_NetWorkBuffer::Buffer> pushRequest(const RequestPacket& request, TC_Transceiver *)
{
	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	unsigned int net_bufflength = htonl(request.sBuffer.size()+8);
	unsigned char * bufflengthptr = (unsigned char*)(&net_bufflength);

	vector<char> buffer;
	buffer.resize(request.sBuffer.size()+8);

	memcpy(buffer.data(), bufflengthptr, sizeof(unsigned int));

	unsigned int netrequestId = htonl(request.iRequestId);
	unsigned char * netrequestIdptr = (unsigned char*)(&netrequestId);

	memcpy(buffer.data() + sizeof(unsigned int), netrequestIdptr, sizeof(unsigned int));
	memcpy(buffer.data() + sizeof(unsigned int) * 2, request.sBuffer.data(), request.sBuffer.size());

	buff->addBuffer(buffer);

	return buff;
}


//The response packet decoding function decodes the data received from the server according to the specific format and resolves it to theBasePacket
static TC_NetWorkBuffer::PACKET_TYPE customResponse(TC_NetWorkBuffer &in, ResponsePacket& rsp)
{
	size_t len = sizeof(tars::Int32);

	if (in.getBufferLength() < len)
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	string header;
	in.getHeader(len, header);

	assert(header.size() == len);

	tars::Int32 iHeaderLen = 0;

	::memcpy(&iHeaderLen, header.c_str(), sizeof(tars::Int32));

	iHeaderLen = ntohl(iHeaderLen);

	if (iHeaderLen > 100000 || iHeaderLen < (int)sizeof(unsigned int))
	{
		throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
	}

	if (in.getBufferLength() < (uint32_t)iHeaderLen)
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	in.moveHeader(sizeof(iHeaderLen));

	tars::Int32 iRequestId = 0;
	string sRequestId;
	in.getHeader(sizeof(iRequestId), sRequestId);
	in.moveHeader(sizeof(iRequestId));

	rsp.iRequestId = ntohl(*((unsigned int *)(sRequestId.c_str())));
	len =  iHeaderLen - sizeof(iHeaderLen) - sizeof(iRequestId);
	in.getHeader(len, rsp.sBuffer);
	in.moveHeader(len);

	return TC_NetWorkBuffer::PACKET_FULL;
}

/*
   Whole package length (4 bytes) + irequestid (4 bytes) + package content
*/
static shared_ptr<TC_NetWorkBuffer::Buffer> customRequest(RequestPacket& request, TC_Transceiver *)
{
	shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();

	unsigned int net_bufflength = htonl(request.sBuffer.size()+8);
	unsigned char * bufflengthptr = (unsigned char*)(&net_bufflength);

	vector<char> buffer;
	buffer.resize(request.sBuffer.size()+8);

	memcpy(buffer.data(), bufflengthptr, sizeof(unsigned int));

	unsigned int netrequestId = htonl(request.iRequestId);
	unsigned char * netrequestIdptr = (unsigned char*)(&netrequestId);

	memcpy(buffer.data() + sizeof(unsigned int), netrequestIdptr, sizeof(unsigned int));
	memcpy(buffer.data() + sizeof(unsigned int) * 2, request.sBuffer.data(), request.sBuffer.size());

	buff->addBuffer(buffer);

	return buff;
}

//回调函数
void ClientHelloCallback::callback_testTrans(int ret, const string &r)
{
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(r == buffer);
	callback_count++;

	if(callback_count == count-1)
	{
//		int64_t cost = TC_Common::now2us() - start;
//		LOG_CONSOLE_DEBUG << "callback_testTrans count:" << count << ", " << cost << " us, avg:" << 1.*cost/count << "us" << endl;
	}
}

//回调函数
void ClientHelloCallback::callback_testHello(int ret, const string &r)
{
	callback_count++;

	if(!_prx)
	{
		ASSERT_TRUE(ret == 0);
		ASSERT_TRUE(r == buffer);

		if (callback_count == count - 1)
		{
//			int64_t cost = TC_Common::now2us() - start;
//			LOG_CONSOLE_DEBUG << "callback_testHello count:" << count << ", " << cost << " us, avg:" << 1. * cost / count << "us" << endl;
		}
	}
	else
	{
		string out;
		int code = _prx->testHello(0, r, out);
		ASSERT_TRUE(code == 0);
		ASSERT_TRUE(out == r);

		++callback_count;
	}
}

void ClientHelloCallback::callback_testSyncTrans(tars::Int32 ret,  const std::string& r)
{
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(r == buffer);
	callback_count++;

	if(callback_count == count-1)
	{
//		int64_t cost = TC_Common::now2us() - start;
//		LOG_CONSOLE_DEBUG << "callback_testSyncTrans count:" << count << ", " << cost << " us, avg:" << 1.*cost/count << "us" << endl;
	}
}

void HelloTest::startServer(HelloServer &server, TC_EpollServer::SERVER_OPEN_COROUTINE openCoroutine)
{
	TC_Config conf = CONFIG();

	conf.set("/tars/application/server<opencoroutine>", TC_Common::tostr(openCoroutine));

	ASSERT_TRUE(conf.get("/tars/application/server<opencoroutine>") == TC_Common::tostr(openCoroutine));

	server.main(conf.tostr());

	ASSERT_TRUE(ServerConfig::OpenCoroutine == openCoroutine);

	server.start();

	server.waitForReady();
}

TC_Endpoint HelloTest::getEndpoint(const string &adapter)
{
	return TC_Endpoint(_conf.get("/tars/application/server/" + adapter + "<endpoint>"));
}

void HelloTest::wait(int millseconds)
{
	if(TC_CoroutineScheduler::scheduler())
	{
		TC_CoroutineScheduler::scheduler()->sleep(millseconds);
	}
	else
	{
		TC_Common::msleep(millseconds);
	}
}

void HelloTest::syncCustom(Communicator *comm, const string &adapter )
{
	ServantPrx prx = getObj<ServantPrx>(comm, adapter);
	ProxyProtocol prot;
	prot.requestFunc = customRequest;
	prot.responseFunc = customResponse;

	prx->tars_set_protocol(prot);

	int64_t start = TC_Common::now2us();

	string out;
	//发起远程调用
	for (int j = 0; j < _count; ++j) {

		ResponsePacket rsp;
		prx->rpc_call(prx->tars_gen_requestid(), "doCustomFunc", _buffer.c_str(), _buffer.length(), rsp);
	}
	int64_t cost = TC_Common::now2us() - start;

	LOG_CONSOLE_DEBUG << "count:" << _count << ", " << cost << " us, avg:" << 1.*cost/_count << "us" << endl;
}


void HelloTest::asyncCustom(Communicator *comm, const string &adapter)
{
	ServantPrx prx = getObj<ServantPrx>(comm, adapter);
	ProxyProtocol prot;
	prot.requestFunc = customRequest;
	prot.responseFunc = customResponse;

	prx->tars_set_protocol(prot);

	atomic<int> callback_count{0};

	//发起远程调用
	for (int j = 0; j < _count; ++j)
	{
		CustomCallBackPtr cb = new CustomCallBack(callback_count);
		prx->rpc_call_async(prx->tars_gen_requestid(), "doCustomFunc", _buffer.c_str(), _buffer.length(), cb);
	}

	waitForFinish(callback_count, _count);

	ASSERT_TRUE(callback_count == _count);
}

void HelloTest::testPush(Communicator *comm, const string &adapter)
{
	ServantPrx prx = getObj<ServantPrx>(comm, adapter);

	ProxyProtocol prot;
	prot.requestFunc = pushRequest;
	prot.responseFunc = pushResponse;

	prx->tars_set_protocol(prot);

	PushCallBackPtr cbPush = new PushCallBack();
	prx->tars_set_push_callback(cbPush);

	string buf("heartbeat");

	int count = 2;
	while(count-- > 0)
	{
		cbPush->_onconnect = false;
		cbPush->_onclose = false;

		PushCallBackPtr cb = new PushCallBack();
		prx->rpc_call_async(prx->tars_gen_requestid(), "printResult", buf.c_str(), buf.length(), cb);

		wait(1000);

		ASSERT_TRUE(cbPush->_onconnect);
		ASSERT_TRUE(cb->_onprintresult);

		wait(10000);

		ASSERT_TRUE(cbPush->_onclose);
	}
}

void HelloTest::testReconnect(Communicator *comm, const string &adapter )
{
	ServantPrx prx = getObj<ServantPrx>(comm, adapter);

	ProxyProtocol prot;
	prot.requestFunc = pushRequest;
	prot.responseFunc = pushResponse;

	prx->tars_set_protocol(prot);
	prx->tars_reconnect(1);

	PushCallBackPtr cbPush = new PushCallBack();
	prx->tars_set_push_callback(cbPush);

	string buf("heartbeat");

	cbPush->_onconnect = false;
	cbPush->_onclose = false;

	ResponsePacket rsp;
	prx->rpc_call(prx->tars_gen_requestid(), "printResult", buf.c_str(), buf.length(), rsp);

	TC_Common::msleep(10500);

	ASSERT_TRUE(cbPush->_onclose);

	TC_Common::msleep(2000);

	ASSERT_TRUE(cbPush->_onconnect);

}

void HelloTest::checkWup()
{
	UniPacket<> req;

	req.setRequestId(1);
	req.setServantName("TestApp.HelloServer.HelloObj");
	req.setFuncName("testHello");

	req.put<int>("index", 1);
	req.put<string>("s", _buffer);

	string buff = "";
	req.encode(buff);

//	int64_t start = TC_Common::now2us();

	TC_Endpoint ep = getEndpoint("HelloAdapter");
	//可以直接发送请求给HelloServer 或者 发送请求给TranServer
	TC_TCPClient client;
	client.init(ep.getHost(), ep.getPort(), 3000);

	for (int i = 0; i < _count; i++)
	{
		char recvBuff[1024] = { 0 };
		size_t recvLen = sizeof(recvBuff);

		int ret = client.sendRecv(buff.c_str(), buff.length(), recvBuff, recvLen);

		ASSERT_TRUE(ret == 0);

		TarsUniPacket<> rsp;

		rsp.decode(recvBuff, recvLen);

		string retStr = "";

		ret = rsp.getTarsResultCode();

		ASSERT_TRUE(ret == 0);

		rsp.get("r", retStr);

		ASSERT_TRUE(retStr == _buffer);

	}

//	int64_t cost = TC_Common::now2us() - start;

//	LOG_CONSOLE_DEBUG << "count:" << _count << ", " << cost << " us, avg:" << 1.*cost/_count << "us" << endl;

}

void HelloTest::checkSyncHttp(Communicator *comm, const string &adapter, bool close)
{
	ServantPrx prx = getObj<ServantPrx>(comm, adapter);
	prx->tars_set_protocol(ServantProxy::PROTOCOL_HTTP1, 10);

	int64_t start = TC_Common::now2us();

	string out;
	//发起远程调用
	for (int j = 0; j < _count; ++j) {

		string buff = _buffer + "-" + TC_Common::tostr(j) + "-" + TC_Common::tostr(TC_Thread::CURRENT_THREADID());
		shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();
		req->setPostRequest("http://tars.com/hello", buff, true);
		if(close)
		{
			req->setHeader("Connection", "close");
		}
		else
		{
			req->setHeader("Connection", "keep-alive");
		}

		shared_ptr<TC_HttpResponse> rsp;
		prx->http_call("hello", req, rsp);

		ASSERT_TRUE(req->getContent() == rsp->getContent());

	}

	int64_t cost = TC_Common::now2us() - start;

	LOG_CONSOLE_DEBUG << "count:" << _count << ", " << cost << " us, avg:" << 1.*cost/_count << "us" << endl;

}

void HelloTest::checkASyncHttp(Communicator *comm, const string &adapter, bool close)
{
	atomic<int> callback_count{0};
	ServantPrx prx = getObj<ServantPrx>(comm, adapter);
	prx->tars_set_protocol(ServantProxy::PROTOCOL_HTTP1, 10);

	//发起远程调用
	for (int j = 0; j < _count; ++j)
	{
		string buff = _buffer + "-" + TC_Common::tostr(j) + "-" + TC_Common::tostr(TC_Thread::CURRENT_THREADID());
		shared_ptr<TC_HttpRequest> req = std::make_shared<TC_HttpRequest>();
		req->setPostRequest("http://tars.com/hello", buff, true);
		if(close)
		{
			req->setHeader("Connection", "close");
		}
		else
		{
			req->setHeader("Connection", "keep-alive");
		}

		HttpCallbackPtr p = new HelloHttpCallback(TC_Common::now2us(), j, _count, buff, callback_count);

		prx->http_call_async("hello", req, p);
	}

	waitForFinish(callback_count, _count);

	ASSERT_TRUE(callback_count == _count);

}

void HelloTest::checkSyncOnce(HelloPrx prx)
{
	// HelloPrx prx = getObj<HelloPrx>(comm, adapter);
//	int64_t start = TC_Common::now2us();

	string out;
	//发起远程调用

	prx->testHello(0, _buffer, out);

	ASSERT_TRUE(_buffer == out);
}

void HelloTest::checkASyncOnce(HelloPrx prx)
{
	// HelloPrx prx = getObj<HelloPrx>(comm, adapter);

	atomic<int> callback_count{0};

	HelloPrxCallbackPtr p = new ClientHelloCallback(prx, callback_count);

	prx->async_testHello(p, 0, _buffer);

	wait(100);

	ASSERT_TRUE(callback_count == 2);
}

void HelloTest::checkSync(Communicator *comm, const string &adapter)
{
	HelloPrx prx = getObj<HelloPrx>(comm, adapter);
	int64_t start = TC_Common::now2us();

	string out;
	//发起远程调用
	for (int j = 0; j < _count; ++j) {

		prx->testHello(j, _buffer, out);

		ASSERT_TRUE(_buffer == out);
	}

	int64_t cost = TC_Common::now2us() - start;

	LOG_CONSOLE_DEBUG << "count:" << _count << ", " << cost << " us, avg:" << 1.*cost/_count << "us" << endl;

}

void HelloTest::waitForFinish(atomic<int> &callback_count, int count)
{
	int lastCallbackCount = 0;
	while (callback_count != count)
	{
		wait(100);
		if(lastCallbackCount == callback_count)
		{
			LOG_CONSOLE_DEBUG << "no callback: " << callback_count << ", " << count << endl;
		}
		lastCallbackCount = callback_count;
	}

//	LOG_CONSOLE_DEBUG << "callback: " << callback_count << ", " << _count << endl;
}

void HelloTest::checkASync(Communicator *comm, const string &adapter)
{
	atomic<int> callback_count{0};

	HelloPrx prx = getObj<HelloPrx>(comm, adapter);

	//发起远程调用
	for (int j = 0; j < _count; ++j)
	{
		HelloPrxCallbackPtr p = new ClientHelloCallback(TC_Common::now2us(), j, _count, _buffer, callback_count);

		prx->async_testHello(p, j, _buffer);
	}

	waitForFinish(callback_count, _count);

	ASSERT_TRUE(callback_count == _count);

}

void HelloTest::checkWupTransSync(Communicator *comm)
{
	ServantPrx prx = getObj<ServantPrx>(comm, "TransWupAdapter");

	UniPacket<> req;

	req.setRequestId(1);
	req.setServantName("TestApp.HelloServer.TransDstObj");
	req.setFuncName("testHello");

	req.put<string>("s", _buffer);

	ProxyProtocol proto;
	proto.requestFunc = ProxyProtocol::streamRequest;
	proto.responseFunc = ProxyProtocol::tupResponse;

	prx->tars_set_protocol(proto);

	int64_t start = TC_Common::now2us();
	//发起远程调用
	for (int i = 0; i < _count; ++i)
	{
		req.setRequestId(prx->tars_gen_requestid());

		req.put<int>("index", i);

		vector<char> buff;
		req.encode(buff);

		ResponsePacket done;

		prx->rpc_call(req.getRequestId(), "testHello", buff.data(), buff.size(), done);

		TarsUniPacket<> rsp;

		rsp.decode(done.sBuffer.data(), done.sBuffer.size());

		string retStr = "";

		int ret = rsp.getTarsResultCode();

		ASSERT_TRUE(ret == 0);

		rsp.get("r", retStr);

		ASSERT_TRUE(retStr == _buffer);
	}

	int64_t cost = TC_Common::now2us() - start;

	LOG_CONSOLE_DEBUG << "count:" << _count << ", " << cost << " us, avg:" << 1.*cost/_count << "us" << endl;
}

void HelloTest::checkTransSyncASync(Communicator *comm)
{
	HelloPrx prx = getObj<HelloPrx>(comm, "TransAdapter");

	int64_t start = TC_Common::now2us();

	string out;
	//发起远程调用
	for (int j = 0; j < _count; ++j) {

		prx->testTrans(j, _buffer, out);

		ASSERT_TRUE(_buffer == out);
	}


	int64_t cost = TC_Common::now2us() - start;

	LOG_CONSOLE_DEBUG << "count:" << _count << ", " << cost << " us, avg:" << 1.*cost/_count << "us" << endl;

}

void HelloTest::checkTransASyncASync(Communicator *comm)
{
	string adapter = "TransAdapter";

	atomic<int> callback_count{0};

	HelloPrx prx = getObj<HelloPrx>(comm, adapter);

	//发起远程调用
	for (int j = 0; j < _count; ++j)
	{
		HelloPrxCallbackPtr p = new ClientHelloCallback(TC_Common::now2us(), j, _count, _buffer, callback_count);

		prx->async_testTrans(p, j, _buffer);
	}

	waitForFinish(callback_count, _count);

	ASSERT_TRUE(callback_count == _count);
}

void HelloTest::checkTransSyncSync(Communicator *comm)
{
	HelloPrx prx = getObj<HelloPrx>(comm, "TransAdapter");

	int64_t start = TC_Common::now2us();

	string out;
	//发起远程调用
	for (int j = 0; j < _count; ++j) {

		prx->testSyncTrans(j, _buffer, out);

		ASSERT_TRUE(_buffer == out);
	}

	int64_t cost = TC_Common::now2us() - start;

	LOG_CONSOLE_DEBUG << "count:" << _count << ", " << cost << " us, avg:" << 1.*cost/_count << "us" << endl;

}

void HelloTest::checkTransASyncSync(Communicator *comm)
{
	string adapter = "TransAdapter";

	atomic<int> callback_count{0};

	HelloPrx prx = getObj<HelloPrx>(comm, adapter);

	//发起远程调用
	for (int j = 0; j < _count; ++j)
	{
		HelloPrxCallbackPtr p = new ClientHelloCallback(TC_Common::now2us(), j, _count, _buffer, callback_count);

		prx->async_testSyncTrans(p, j, _buffer);
	}

	waitForFinish(callback_count, _count);

	ASSERT_TRUE(callback_count == _count);
}

void HelloTest::rpcFromRegistry(Communicator *comm)
{
	//test get ip from registry
	HelloPrx qPrx = comm->stringToProxy<HelloPrx>("TestApp.RpcServer.HelloObj");
	string out;
	int ret = qPrx->testHello(0, _buffer, out);

	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(out == _buffer);

	//test rpc loop
	int count = 10;
	while(count-- > 0)
	{
		int ret;
		string out1, out2;
		ret = qPrx->testPid(out1);
		ASSERT_TRUE(ret == 0);
		ret = qPrx->testPid(out2);
		ASSERT_TRUE(ret == 0);

		ASSERT_TRUE(out1 != out2);
	}

	//test rpc hash
	count = 10;
	while(count-- > 0)
	{
		vector<string> vout;

		std::thread hash([&](){
			string out;
			int ret = qPrx->tars_hash(TC_Thread::CURRENT_THREADID())->testPid(out);
			vout.push_back(out);
			ASSERT_TRUE(ret == 0);
		});

		hash.join();

		for(size_t i = 0; i < vout.size()-1; i++)
		{
			ASSERT_TRUE(vout[i] == vout[i+1]);
		}
	}

}

void HelloTest::rpcConHashFromRegistry(Communicator *comm)
{
	HelloPrx prx = comm->stringToProxy<HelloPrx>("TestApp.RpcServer.HelloObj");

	//test rpc consistent hash
	int count = 10;
	while(count-- > 0)
	{
		std::thread conHash([&](){
			std::set<std::string> servInfos;
			for (int j = 0; j < 10; ++j) {
				std::string serverInfo;
				int ret = prx->tars_consistent_hash(TC_Thread::CURRENT_THREADID())->testConHash(serverInfo);
				LOG_CONSOLE_DEBUG << "hashCode:" << TC_Thread::CURRENT_THREADID() << ", serverInfo:" << serverInfo << endl;
				servInfos.emplace(serverInfo);
				ASSERT_TRUE(ret == 0);
			}

			ASSERT_TRUE(servInfos.size() == 1);
     		});

		conHash.join();
	}
}

void HelloTest::checkSyncTimeout(Communicator *comm)
{
	HelloPrx prx = getObj<HelloPrx>(comm, "HelloAdapter");

	prx->tars_set_timeout(2000);
	prx->tars_async_timeout(3000);
	prx->testTimeout(1);

	ASSERT_TRUE(true);

	try
	{
		prx->testTimeout(4);
	}
	catch (exception& ex)
	{
		ASSERT_TRUE(true);
	}

}

void HelloTest::checkASyncTimeout(Communicator *comm)
{
	HelloPrx prx = getObj<HelloPrx>(comm, "HelloAdapter");

	prx->tars_set_timeout(2000);
	prx->tars_async_timeout(3000);

	std::atomic<int> callback_count;
	ClientHelloCallback *c = new ClientHelloCallback(TC_Common::now2us(), 0, _count, _buffer, callback_count);
	HelloPrxCallbackPtr p = c;

	prx->async_testTimeout(p, 1);

	wait(1500);
	ASSERT_TRUE(c->callback);
	c->callback = false;

	prx->async_testTimeout(p, 5);

	wait(4000);

	ASSERT_FALSE(c->callback);
	ASSERT_TRUE(c->callback_exception);
}

void HelloTest::checkStat(Communicator *comm, int reportCount)
{
	HelloPrx prx = getObj<HelloPrx>(comm, "HelloAdapter");

	prx->tars_set_timeout(2000);
	prx->tars_async_timeout(3000);

	int count = reportCount;
	while(count-- > 0)
	{
		string out;
		prx->testHello(count, _buffer, out);

		TC_Common::sleep(1);
	}
}

void HelloTest::forEach(function<void()> func)
{
	int i = 2;
	for(i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		HelloServer server;
		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) i);

		func();

		stopServer(server);
	}
}


void HelloTest::forEach(function<void(Communicator *comm)> func)
{
	int i = 0;//TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO;
	for(i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		HelloServer server;
		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) i);

		func(server.getCommunicator().get());

		stopServer(server);
	}
}

void HelloTest::forEachInCoroutine(function<void(Communicator *comm)> func)
{
	int i = 0;
	for(i = 0; i <= TC_EpollServer::NET_THREAD_MERGE_HANDLES_CO; i++)
	{
		HelloServer server;
		startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) i);

		{
			std::thread cor_call([&]()
			{
				auto scheduler = TC_CoroutineScheduler::create();

				//设置到协程中
				ServantProxyThreadData::getData()->_sched = scheduler;

				scheduler->go([&]()
										   {
											   scheduler->setNoCoroutineCallback([=](TC_CoroutineScheduler *s)
																				 {
																					 s->terminate();
																				 });

											   func(server.getCommunicator().get());
											   
										   });

				scheduler->run();
			});
			cor_call.join();
		}

		stopServer(server);
	}
}

void HelloTest::funcInCoroutine(function<void()> func, bool setSched)
{
	std::thread cor_call([&]()
	{
		auto scheduler = TC_CoroutineScheduler::create();

		if(setSched)
		{
			//设置到协程中
			ServantProxyThreadData::getData()->_sched = scheduler;
		}

		scheduler->go([&]()
								   {
									   scheduler->setNoCoroutineCallback([=](TC_CoroutineScheduler *s)
																		 { s->terminate(); });

									   func();
								   });

		scheduler->run();
	});
	cor_call.join();
}

void HelloTest::transGlobalCommunicator(function<void(Communicator *)> func, Communicator *comm)
{
	forEach([&](Communicator *c){
		func(comm);
	});

//	LOG_CONSOLE_DEBUG << "succ" << endl;
}

void HelloTest::transServerCommunicator(function<void(Communicator *)> func)
{
	forEach([&](Communicator *c){
		func(c);
	});
//	LOG_CONSOLE_DEBUG << "succ" << endl;
}

void HelloTest::transAllocCommunicator(function<void(Communicator *)> func)
{
	forEach([&](Communicator *c){
		shared_ptr<Communicator> comm = std::make_shared<Communicator>();

		func(comm.get());
	});
//	LOG_CONSOLE_DEBUG << "succ" << endl;
}

void HelloTest::transComplexCommunicator(function<void(Communicator *)> func, Communicator *comm)
{
	forEach([&](Communicator *c){
		shared_ptr<Communicator> allocComm = std::make_shared<Communicator>();

		std::thread thAlloc1([&](){
			func(allocComm.get());
		});

		std::thread thServer1([&](){
			func(c);
		});

		std::thread thGlobal1([&](){
			func(c);
		});

		thAlloc1.join();
		thServer1.join();
		thGlobal1.join();
	});
//	LOG_CONSOLE_DEBUG << "succ" << endl;
}

void HelloTest::transInCoroutineGlobalCommunicator(function<void(Communicator *)> func, Communicator *comm)
{
	forEachInCoroutine([&](Communicator *c){
		func(comm);
	});
//	LOG_CONSOLE_DEBUG << "succ" << endl;
}

void HelloTest::transInCoroutineServerCommunicator(function<void(Communicator *)> func)
{
	forEachInCoroutine([&](Communicator *c){
		func(c);
	});
//	LOG_CONSOLE_DEBUG << "succ" << endl;
}

void HelloTest::transInCoroutineAllocCommunicator(function<void(Communicator *)> func)
{
	forEachInCoroutine([&](Communicator *c){
		shared_ptr<Communicator> comm = std::make_shared<Communicator>();

		func(comm.get());
	});
//	LOG_CONSOLE_DEBUG << "succ" << endl;
}


void HelloTest::transInCoroutineComplexCommunicator(function<void(Communicator *)> func, Communicator *comm)
{
	forEachInCoroutine([&](Communicator *c)
					   {
						  	{
								shared_ptr<Communicator> allocComm = std::make_shared<Communicator>();

								std::thread thAlloc1([&]()
														{ func(allocComm.get()); });

								std::thread thServer1([&]()
														{ func(c); });

								std::thread thGlobal1([&]()
														{ func(c); });

								thAlloc1.join();
								thServer1.join();
								thGlobal1.join();
						  	}

					   });
	//	LOG_CONSOLE_DEBUG << "succ" << endl;
}

shared_ptr<Communicator> HelloTest::getCommunicator()
{
	shared_ptr<Communicator> c = std::make_shared<Communicator>();

	TC_Config conf = CLIENT_CONFIG();
	c->setProperty(conf);
	c->setProperty("sendqueuelimit", "1000000");

	return c;
}