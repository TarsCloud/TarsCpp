#include "RtpGateway.h"
#include "SipRtpImp.h"

using namespace std;

RtpGateway g_app;

/////////////////////////////////////////////////////////////////
void
RtpGateway::initialize()
{
    //initialize application here:
    //...

    addServant<SipRtpImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".SipRtpObj");
}
/////////////////////////////////////////////////////////////////
void
RtpGateway::destroyApp()
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
