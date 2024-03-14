#include "HelloServer.h"
#include "HelloImp.h"
#include "TranImp.h"
#include "HttpImp.h"
#include "CustomImp.h"
#include "PushImp.h"

#include <thread>
// #include "gperftools/profiler.h"

using namespace std;

string g_HelloServerObj;
string g_TransDstServerObj;
atomic<int> g_handleDestroy = {0};

static TC_NetWorkBuffer::PACKET_TYPE parse(TC_NetWorkBuffer &in, vector<char> &out)
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

	in.getHeader(iHeaderLen, out);
	in.moveHeader(iHeaderLen);

	return TC_NetWorkBuffer::PACKET_FULL;
}

void
HelloServer::initialize()
{
//    this->_epollServer->setUdpRecvBufferSize(10 * 1024 * 1024);
//    this->_epollServer->setUdpSendBufferSize(10 * 1024 * 1024);
	g_HelloServerObj =_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".HelloObj@" + getEpollServer()->getBindAdapter("HelloAdapter")->getEndpoint().toString();
	g_TransDstServerObj =_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".TransDstObj@" + getEpollServer()->getBindAdapter("TransDstAdapter")->getEndpoint().toString();

    addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".HelloObj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".TransObj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".HelloTimeoutObj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".HelloNoTimeoutObj");

	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".Ipv6Obj");

	addServant<HttpImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".HttpObj");
	addServantProtocol(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".HttpObj", &TC_NetWorkBuffer::parseHttp);
	addServant<HttpImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".HttpsObj");
	addServantProtocol(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".HttpsObj", &TC_NetWorkBuffer::parseHttp);

	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".TransDstObj");

	addServant<TranImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".TransWupObj");
	//设置服务的协议解析器, 对服务器端而言, 解析器的目的就是识别包长度
	//parseStream,表示第0个字节开始,类型是uint32_t, 字节序, 这个字段表示包的长度
	TC_NetWorkBuffer::protocol_functor func = AppProtocol::parseStream<0, uint32_t, true>;
	addServantProtocol(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".TransWupObj", func);

	addServant<CustomImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".CustomObj");
	addServantProtocol(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".CustomObj", parse);

	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".SSLObj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".SSL1Obj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".SSL2Obj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".SSL3Obj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".AuthObj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".UdpObj");
	addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".UdpIpv6Obj");

	addServant<PushImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".PushObj");
	addServantProtocol(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".PushObj", parse);

	pushThread = new PushInfoThread();
	pushThread->start();
}

/////////////////////////////////////////////////////////////////
void HelloServer::destroyApp()
{
	//destroy application here:
	//...
	if(pushThread)
	{
		pushThread->terminate();
		pushThread->getThreadControl().join();

		delete pushThread;
	}

//	LOG_CONSOLE_DEBUG << endl;
//    ProfilerStop();
}

void HelloServer::run()
{
	this->waitForShutdown();
}