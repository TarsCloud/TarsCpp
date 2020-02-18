#include "CustomServer.h"
#include "CustomServantImp.h"

using namespace std;

CustomServer g_app;

/////////////////////////////////////////////////////////////////

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
CustomServer::initialize()
{
    //initialize application here:
    //...

    addServant<CustomServantImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".CustomServantObj");

    addServantProtocol("TestApp.CustomServer.CustomServantObj", parse);
}

/////////////////////////////////////////////////////////////////
void
CustomServer::destroyApp()
{
    //destroy application here:
    //...

}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
