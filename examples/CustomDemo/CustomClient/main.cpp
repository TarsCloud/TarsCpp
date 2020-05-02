#include "servant/Application.h"
#include "util/tc_network_buffer.h"
#include <iostream>

using namespace std;
using namespace tars;


Communicator* _comm;

static string sObjName = "TestApp.CustomServer.CustomServantObj@tcp -h 127.0.0.1 -t 60000 -p 9400";

struct Param
{
	int count;
	string call;
	int thread;
	int buffersize;
	int netthread;

	ServantPrx servantPrx;
};

Param param;
std::atomic<int> callback_count(0);

//The response packet decoding function decodes the data received from the server according to the specific format and resolves it to theResponsePacket
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
static vector<char> customRequest(RequestPacket& request, Transceiver *)
{
    unsigned int net_bufflength = htonl(request.sBuffer.size()+8);
    unsigned char * bufflengthptr = (unsigned char*)(&net_bufflength);

	vector<char> buffer;
	buffer.resize(request.sBuffer.size()+8);

	memcpy(buffer.data(), bufflengthptr, sizeof(unsigned int));

    unsigned int netrequestId = htonl(request.iRequestId);
    unsigned char * netrequestIdptr = (unsigned char*)(&netrequestId);

	memcpy(buffer.data() + sizeof(unsigned int), netrequestIdptr, sizeof(unsigned int));
	memcpy(buffer.data() + sizeof(unsigned int) * 2, request.sBuffer.data(), request.sBuffer.size());

	return buffer;
}

class CustomCallBack : public ServantProxyCallback
{
public:
    virtual int onDispatch(ReqMessagePtr msg)
	{
		if(msg->response->iRet != tars::TARSSERVERSUCCESS)
		{
			cout << "ret error:" << msg->response->iRet << endl;
		}
		else
		{
//			cout << "succ:" << string(msg->response->sBuffer.data(), msg->response->sBuffer.size()) << endl;
		}

		++callback_count;
		return msg->response->iRet;
	}

};

typedef tars::TC_AutoPtr<CustomCallBack> CustomCallBackPtr;


void syncCall(int c)
{
	string buffer(param.buffersize, 'a');

	int64_t t = TC_Common::now2us();
	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		string r;

		try
		{
			ResponsePacket rsp;
			param.servantPrx->rpc_call(param.servantPrx->tars_gen_requestid(), "doCustomFunc", buffer.c_str(), buffer.length(), rsp);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}
		++callback_count;
	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "syncCall total:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}


void asyncCall(int c)
{
	int64_t t = TC_Common::now2us();

	string buffer(param.buffersize, 'a');

	//发起远程调用
	for (int i = 0; i < c; ++i)
	{
		try
		{
			CustomCallBackPtr cb = new CustomCallBack();
			param.servantPrx->rpc_call_async(param.servantPrx->tars_gen_requestid(), "doCustomFunc", buffer.c_str(), buffer.length(), cb);
		}
		catch(exception& e)
		{
			cout << "exception:" << e.what() << endl;
		}
	}

	int64_t cost = TC_Common::now2us() - t;
	cout << "asyncCall send:" << cost << "us, avg:" << 1.*cost/c << "us" << endl;
}

int main(int argc, char *argv[])
{
	try
	{
		if (argc < 6)
		{
			cout << "Usage:" << argv[0] << "--count=1000 --call=[sync|async] --thread=1 --buffersize=1000 --netthread=1" << endl;

			return 0;
		}

		TC_Option option;
		option.decode(argc, argv);

		param.count = TC_Common::strto<int>(option.getValue("count"));
		if(param.count <= 0) param.count = 1000;
		param.buffersize = TC_Common::strto<int>(option.getValue("buffersize"));
		if(param.buffersize <= 0) param.buffersize = 1000;
		param.call = option.getValue("call");
		if(param.call.empty()) param.call = "sync";
		param.thread = TC_Common::strto<int>(option.getValue("thread"));
		if(param.thread <= 0) param.thread = 1;
		param.netthread = TC_Common::strto<int>(option.getValue("netthread"));
		if(param.netthread <= 0) param.netthread = 1;

		_comm = new Communicator();
		_comm->setProperty("sendqueuelimit", "1000000");
		_comm->setProperty("asyncqueuecap", "1000000");
		_comm->setProperty("netthread", TC_Common::tostr(param.netthread));

		param.servantPrx = _comm->stringToProxy<ServantPrx>(sObjName);

//        LocalRollLogger::getInstance()->logger()->setLogLevel(6);
		ProxyProtocol prot;
		prot.requestFunc = customRequest;
		prot.responseFunc = customResponse;

		param.servantPrx->tars_set_protocol(prot);

		param.servantPrx->tars_connect_timeout(5000);
		param.servantPrx->tars_async_timeout(60*1000);

		int64_t start = TC_Common::now2us();

		std::function<void(int)> func;

		if (param.call == "sync")
		{
			func = syncCall;
		}
		else if (param.call == "async")
		{
			func = asyncCall;
		}
		else
		{
			cout << "no func, exits" << endl;
			exit(0);
		}

		vector<std::thread*> vt;
		for(int i = 0 ; i< param.thread; i++)
		{
			vt.push_back(new std::thread(func, param.count));
		}

		std::thread print([&]{while(callback_count != param.count * param.thread) {
			cout << "Custom:" << param.call << ": ----------finish count:" << callback_count << endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		};});

		for(size_t i = 0 ; i< vt.size(); i++)
		{
			vt[i]->join();
			delete vt[i];
		}

		cout << "(pid:" << std::this_thread::get_id() << ")"
		     << "(count:" << param.count << ")"
		     << "(use ms:" << (TC_Common::now2us() - start)/1000 << ")"
		     << endl;

		while(callback_count != param.count * param.thread) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		print.join();
		cout << "----------finish count:" << callback_count << endl;
	}
	catch(exception &ex)
	{
		cout << ex.what() << endl;
	}
	cout << "main return." << endl;

	return 0;
}
