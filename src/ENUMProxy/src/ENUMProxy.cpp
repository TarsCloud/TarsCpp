#include "ENUMProxy.h"
#include "SipRouterImp.h"

using namespace std;

ENUMProxy g_app;

/////////////////////////////////////////////////////////////////
void
ENUMProxy::initialize()
{
    //initialize application here:
    //...

    addServant<SipRouterImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipRouterObj");
}
/////////////////////////////////////////////////////////////////
void
ENUMProxy::destroyApp()
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
