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

	g_HelloServerObj = ServerConfig::Application + "." + ServerConfig::ServerName +".HelloObj@" + getEpollServer()->getBindAdapter("HelloAdapter")->getEndpoint().toString();
	g_TransDstServerObj = ServerConfig::Application + "." + ServerConfig::ServerName +".TransDstObj@" + getEpollServer()->getBindAdapter("TransDstAdapter")->getEndpoint().toString();

    addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".HelloObj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".TransObj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".HelloTimeoutObj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".HelloNoTimeoutObj");

	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".Ipv6Obj");

	addServant<HttpImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".HttpObj");
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".HttpObj", &TC_NetWorkBuffer::parseHttp);
	addServant<HttpImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".HttpsObj");
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".HttpsObj", &TC_NetWorkBuffer::parseHttp);

	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".TransDstObj");

	addServant<TranImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".TransWupObj");
	//设置服务的协议解析器, 对服务器端而言, 解析器的目的就是识别包长度
	//parseStream,表示第0个字节开始,类型是uint32_t, 字节序, 这个字段表示包的长度
	TC_NetWorkBuffer::protocol_functor func = AppProtocol::parseStream<0, uint32_t, true>;
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName +".TransWupObj", func);

	addServant<CustomImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".CustomObj");
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".CustomObj", parse);

	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SSLObj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SSL1Obj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SSL2Obj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SSL3Obj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".AuthObj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".UdpObj");
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".UdpIpv6Obj");

	addServant<PushImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".PushObj");
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".PushObj", parse);

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