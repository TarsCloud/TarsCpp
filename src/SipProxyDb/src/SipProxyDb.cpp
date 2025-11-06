#include "SipProxyDb.h"
#include "SipDbImp.h"

using namespace std;

SipProxyDb g_app;

/////////////////////////////////////////////////////////////////
void
SipProxyDb::initialize()
{
    //initialize application here:
    //...

    addServant<SipDbImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipDbObj");
}
/////////////////////////////////////////////////////////////////
void
SipProxyDb::destroyApp()
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
