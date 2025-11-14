#include "SipCallRelay.h"
#include "SipRelayImp.h"

using namespace std;

SipCallRelay g_app;

/////////////////////////////////////////////////////////////////
void
SipCallRelay::initialize()
{
    //initialize application here:
    //...

    addServant<SipRelayImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipRelayObj");
}
/////////////////////////////////////////////////////////////////
void
SipCallRelay::destroyApp()
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
