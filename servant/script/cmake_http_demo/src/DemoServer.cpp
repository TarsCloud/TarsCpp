#include "DemoServer.h"
#include "DemoServantImp.h"

using namespace std;

DemoServer g_app;

/////////////////////////////////////////////////////////////////
void
DemoServer::initialize()
{
    //initialize application here:
    //...

    addServant<DemoServantImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".DemoServantObj");
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".DemoServantObj", &TC_NetWorkBuffer::parseHttp);
}
/////////////////////////////////////////////////////////////////
void
DemoServer::destroyApp()
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
