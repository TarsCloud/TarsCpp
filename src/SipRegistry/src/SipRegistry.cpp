#include "SipRegistry.h"
#include "SipRegImp.h"

using namespace std;

SipRegistry g_app;

/////////////////////////////////////////////////////////////////
void
SipRegistry::initialize()
{
    //initialize application here:
    //...

    addServant<SipRegImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipRegObj");
}
/////////////////////////////////////////////////////////////////
void
SipRegistry::destroyApp()
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
