#include "SipProxy.h"
#include "SipPrxImp.h"

using namespace std;

SipProxy g_app;

/////////////////////////////////////////////////////////////////
void
SipProxy::initialize()
{
    //initialize application here:
    //...

    addServant<SipPrxImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipPrxObj");
}
/////////////////////////////////////////////////////////////////
void
SipProxy::destroyApp()
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
