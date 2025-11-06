#include "SipCallController.h"
#include "SipControllerImp.h"

using namespace std;

SipCallController g_app;

/////////////////////////////////////////////////////////////////
void
SipCallController::initialize()
{
    //initialize application here:
    //...

    addServant<SipControllerImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipControllerObj");
}
/////////////////////////////////////////////////////////////////
void
SipCallController::destroyApp()
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
