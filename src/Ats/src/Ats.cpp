#include "Ats.h"
#include "SipChargeImp.h"

using namespace std;

Ats g_app;

/////////////////////////////////////////////////////////////////
void
Ats::initialize()
{
    //initialize application here:
    //...

    addServant<SipChargeImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipChargeObj");
}
/////////////////////////////////////////////////////////////////
void
Ats::destroyApp()
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
