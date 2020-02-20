#include "TestRecvThread.h"
#include <iostream>
//#include <arpa/inet.h>

/*
 响应包解码函数，根据特定格式解码从服务端收到的数据，解析为ResponsePacket
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
static vector<char> pushRequest(RequestPacket& request, Transceiver *)
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
	// sbuff->addBuffer(buffer);
}

static void printResult(int iRequestId, const string &sResponseStr)
{
	cout << "request id: " << iRequestId << ", response str: " << sResponseStr << endl;
}

static void printPushInfo(const string &sResponseStr)
{
	cout << "push message: " << sResponseStr << endl;
}

int TestPushCallBack::onDispatch(ReqMessagePtr msg)
{
	if(msg->request.sFuncName == "printResult")
	{
		string sRet;
		sRet.assign(msg->response->sBuffer.data(), msg->response->sBuffer.size());
		printResult(msg->request.iRequestId, sRet);
		return 0;
	}
	else if(msg->response->iRequestId == 0)
	{
		string sRet;
		sRet.assign(msg->response->sBuffer.data(), msg->response->sBuffer.size());
		printPushInfo(sRet);
		return 0;
	}
	else
	{
		cout << "no match func!" <<endl;
	}
	return -3;
}

RecvThread::RecvThread(int second):_second(second), _bTerminate(false)
{
	string sObjName = "TestApp.PushServer.TestPushServantObj@tcp -h 127.0.0.1 -t 60000 -p 9300";

    _prx = _comm.stringToProxy<ServantPrx>(sObjName);

	ProxyProtocol prot;
    prot.requestFunc = pushRequest;
    prot.responseFunc = pushResponse;

    _prx->tars_set_protocol(prot);
}

void RecvThread::run(void)
{
	TestPushCallBackPtr cbPush = new TestPushCallBack();
	_prx->tars_set_push_callback(cbPush);	

	string buf("heartbeat");

	time_t n = TNOW;

	while(!_bTerminate)
	{
		{
			try
			{
				TestPushCallBackPtr cb = new TestPushCallBack();
				_prx->rpc_call_async(_prx->tars_gen_requestid(), "printResult", buf.c_str(), buf.length(), cb);
			}
			catch(TarsException& e)
			{     
				cout << "TarsException: " << e.what() << endl;
			}
			catch(...)
			{
				cout << "unknown exception" << endl;
			}
		}

		if(TNOW - n >= _second)
		{
			_bTerminate = true;
			break;
		}

		{
            TC_ThreadLock::Lock sync(*this);
            timedWait(500);
		}
	}
}
