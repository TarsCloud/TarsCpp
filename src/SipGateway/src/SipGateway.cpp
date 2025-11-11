#include "SipGateway.h"
#include "SipGwImp.h"

using namespace std;

SipGateway g_app;

/////////////////////////////////////////////////////////////////
void
SipGateway::initialize()
{
    //initialize application here:
    //...

    addServant<SipGwImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipGwObj");
}
/////////////////////////////////////////////////////////////////
void
SipGateway::destroyApp()
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
