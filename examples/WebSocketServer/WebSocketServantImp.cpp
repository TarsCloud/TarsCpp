#include "WebSocketServantImp.h"

#include "servant/WebSocketProtocol.h"
#include "util/tc_common.h"

using namespace std;
using namespace tars;

void WebSocketServantImp::initialize()
{
}

void WebSocketServantImp::destroy()
{
}

int WebSocketServantImp::doRequest(TarsCurrentPtr current, vector<char> &response)
{
    (void)response;

    const vector<char> &requestBuffer = current->getRequestBuffer();
    const string request(requestBuffer.begin(), requestBuffer.end());

    string message;
    message += "server time: " + TC_Common::now2str("%Y-%m-%d %H:%M:%S");
    message += "\n";
    message += "client: " + current->getIp() + ":" + TC_Common::tostr(current->getPort());
    message += "\n";
    message += "payload: " + request;

    return WebSocketProtocol::encodeAndSend(message, current);
}

int WebSocketServantImp::doClose(TarsCurrentPtr current)
{
    LOG->debug() << "websocket close from " << current->getIp() << ":" << current->getPort() << endl;
    return 0;
}
