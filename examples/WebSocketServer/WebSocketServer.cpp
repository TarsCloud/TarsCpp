#include "WebSocketServer.h"

#include "WebSocketDemoConfig.h"
#include "WebSocketHttpImp.h"
#include "WebSocketServantImp.h"
#include "servant/WebSocketProtocol.h"

using namespace std;
using namespace tars;

WebSocketServer g_app;

void WebSocketServer::initialize()
{
    const string serverName = ServerConfig::Application + "." + ServerConfig::ServerName;

    addServant<WebSocketHttpImp>(serverName + ".HttpObj");
    addServantProtocol(serverName + ".HttpObj", &TC_NetWorkBuffer::parseHttp);

    addServant<WebSocketServantImp>(serverName + ".WebSocketObj");
    addServantProtocol(serverName + ".WebSocketObj", &WebSocketProtocol::parse);

    const WebSocketDemoConfig config = loadWebSocketDemoConfig();
    WebSocketProtocol::setValidator([path = config.websocketPath](const TC_HttpRequest &request) {
        return request.getRequestUrl() == path;
    });
}

void WebSocketServer::destroyApp()
{
    WebSocketProtocol::setValidator(WebSocketProtocol::FuncValidator());
}

int main(int argc, char *argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception &e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }

    return -1;
}
