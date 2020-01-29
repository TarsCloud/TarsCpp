#include "TestRecvThread.h"
#include <iostream>
#include <arpa/inet.h>

/*
 响应包解码函数，根据特定格式解码从服务端收到的数据，解析为ResponsePacket
*/
static TC_NetWorkBuffer::PACKET_TYPE pushResponse(TC_NetWorkBuffer &in, ResponsePacket& done)
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
	if (iHeaderLen > 100000 || iHeaderLen < sizeof(unsigned int))
	{
		throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
	}

	//包没有接收全
	if (in.getBufferLength() < (uint32_t)iHeaderLen)
	{
		return TC_NetWorkBuffer::PACKET_LESS;
	}

	tars::Int32 iRequestId = 0;
	string sRequestId;
	in.getHeader(sizeof(iHeaderLen), sRequestId);

	ResponsePacket rsp;
	rsp.iRequestId = ntohl(*((unsigned int *)(sRequestId.c_str())));
	in.getHeader(iHeaderLen - sizeof(iHeaderLen) - sizeof(iRequestId), rsp.sBuffer);

    return TC_NetWorkBuffer::PACKET_FULL;
}
/*
   请求包编码函数，本函数的打包格式为
   整个包长度（字节）+iRequestId（字节）+包内容
*/
static void pushRequest(const RequestPacket& request, shared_ptr<TC_NetWorkBuffer::SendBuffer>& buff)
{
    // unsigned int net_bufflength = htonl(request.sBuffer.size()+8);
    // unsigned char * bufflengthptr = (unsigned char*)(&net_bufflength);

    // buff = "";
    // for (int i = 0; i<4; ++i)
    // {
    //     buff += *bufflengthptr++;
    // }

    // unsigned int netrequestId = htonl(request.iRequestId);
    // unsigned char * netrequestIdptr = (unsigned char*)(&netrequestId);

    // for (int i = 0; i<4; ++i)
    // {
    //     buff += *netrequestIdptr++;
    // }

    // string tmp;
    // tmp.assign((const char*)(&request.sBuffer[0]), request.sBuffer.size());
    // buff+=tmp;

	TarsOutputStream<BufferWriterVector> os;

	tars::Int32 iHeaderLen = 0;
	tars::Int32 iRequestId = request.iRequestId;

//	先预留8个字节长度
	os.writeBuf((const char *)&iHeaderLen, sizeof(iHeaderLen));
	os.writeBuf((const char *)&iRequestId, sizeof(iRequestId));

	request.writeTo(os);

	buff->swap(os.getByteBuffer());

	// assert(buff->length() >= 4);

	iHeaderLen = htonl((int)(buff->length()));

	memcpy((void*)buff->buffer(), (const char *)&iHeaderLen, sizeof(iHeaderLen));
}

static void printResult(int iRequestId, const string &sResponseStr)
{
	cout << "request id: " << iRequestId << endl;
	cout << "response str: " << sResponseStr << endl;
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
		cout << "sBuffer: " << msg->response->sBuffer.size() << endl;
		sRet.assign(&(msg->response->sBuffer[0]), msg->response->sBuffer.size());
		printResult(msg->request.iRequestId, sRet);
		return 0;
	}
	else if(msg->response->iRequestId == 0)
	{
		string sRet;
		sRet.assign(&(msg->response->sBuffer[0]), msg->response->sBuffer.size());
		printPushInfo(sRet);
		return 0;
	}
	else
	{
		cout << "no match func!" <<endl;
	}
	return -3;
}

RecvThread::RecvThread():_bTerminate(false)
{
	string sObjName = "Test.TestPushServer.TestPushServantObj";
    string sObjHost = "tcp -h 10.120.129.226 -t 60000 -p 10099";

    _prx = _comm.stringToProxy<ServantPrx>(sObjName+"@"+sObjHost);

	ProxyProtocol prot;
    prot.requestFunc = pushRequest;
    prot.responseFunc = pushResponse;

    _prx->tars_set_protocol(prot);
}

void RecvThread::terminate()
{
	_bTerminate = true;
	{
	    tars::TC_ThreadLock::Lock sync(*this);
	    notifyAll();
	}
}

void RecvThread::run(void)
{
	TestPushCallBackPtr cbPush = new TestPushCallBack();
	_prx->tars_set_push_callback(cbPush);	

	string buf("heartbeat");

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

		{
            TC_ThreadLock::Lock sync(*this);
            timedWait(5000);
		}
	}
}
