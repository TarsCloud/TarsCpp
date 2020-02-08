#include "servant/Application.h"
#include "util/tc_network_buffer.h"
#include <iostream>

using namespace std;
using namespace tars;


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
static void customRequest(const RequestPacket& request, shared_ptr<TC_NetWorkBuffer::SendBuffer>& sbuff)
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

	sbuff->addBuffer(buffer);
}

class CustomCallBack : public ServantProxyCallback
{
public:
    virtual int onDispatchResponse(const RequestPacket &req, const ResponsePacket &rsp)
	{
		cout << "async response:" << rsp.sBuffer.data() << endl;
	}

    virtual int onDispatchException(const RequestPacket &req, const ResponsePacket &rsp)
	{
		cout << "async exception:" << rsp.iRet << endl;
	}
};

typedef tars::TC_AutoPtr<CustomCallBack> CustomCallBackPtr;

int main(int argc,char**argv)
{
    try
    {
        Communicator comm;
	    string sObjName = "TestApp.CustomServer.CustomServantObj@tcp -h 127.0.0.1 -t 60000 -p 9400";

        ServantPrx prx = comm.stringToProxy<ServantPrx>(sObjName);

        ProxyProtocol prot;
        prot.requestFunc = customRequest;
        prot.responseFunc = customResponse;

        prx->tars_set_protocol(prot);

        string buf = "helloword";

        ResponsePacket rsp;
		prx->rpc_call(prx->tars_gen_requestid(), "doCustomFunc", buf.c_str(), buf.length(), rsp);

        cout << "sync response buffer:" << rsp.sBuffer.data() << endl;

		CustomCallBackPtr cb = new CustomCallBack();
		prx->rpc_call_async(prx->tars_gen_requestid(), "doCustomFunc", buf.c_str(), buf.length(), cb);

		TC_Common::sleep(2000);
    }
    catch(std::exception&e)
    {
        cerr<<"std::exception:"<<e.what()<<endl;
    }
    catch(...)
    {
        cerr<<"unknown exception"<<endl;
    }
    return 0;
}
