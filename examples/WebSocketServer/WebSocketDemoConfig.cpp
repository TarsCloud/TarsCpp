#include "WebSocketDemoConfig.h"

#include "servant/Application.h"
#include "util/tc_config.h"
#include "util/tc_file.h"

using namespace std;
using namespace tars;

WebSocketDemoConfig loadWebSocketDemoConfig()
{
    TC_Config conf;
    conf.parseFile(ServerConfig::ConfigFile);

    WebSocketDemoConfig config;
    config.clientPageFile = conf.get("/tars/application/server<clientpage>", "websocket_client.html");
    if (!TC_File::isAbsolute(config.clientPageFile))
    {
        config.clientPageFile = TC_File::extractFilePath(ServerConfig::ConfigFile) + config.clientPageFile;
    }

    config.websocketPath = conf.get("/tars/application/server<websocketpath>", "/ws");
    config.websocketEndpoint = TC_Endpoint(conf.get("/tars/application/server/WebSocketAdapter<endpoint>"));

    return config;
}
