#pragma once

#include <string>

#include "util/tc_clientsocket.h"

struct WebSocketDemoConfig
{
    std::string clientPageFile;
    std::string websocketPath;
    tars::TC_Endpoint websocketEndpoint;
};

WebSocketDemoConfig loadWebSocketDemoConfig();
