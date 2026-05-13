#include "WebSocketHttpImp.h"

#include <stdexcept>

#include "WebSocketDemoConfig.h"
#include "util/tc_common.h"
#include "util/tc_file.h"
#include "util/tc_http.h"

using namespace std;
using namespace tars;

namespace
{
string renderClientPage(const string &templateContent, const WebSocketDemoConfig &config)
{
    string page = templateContent;
    page = TC_Common::replace(page, "__WS_PORT__", TC_Common::tostr(config.websocketEndpoint.getPort()));
    page = TC_Common::replace(page, "__WS_PATH__", config.websocketPath);
    return page;
}
}

void WebSocketHttpImp::initialize()
{
    const WebSocketDemoConfig config = loadWebSocketDemoConfig();
    if (!TC_File::isFileExist(config.clientPageFile))
    {
        throw runtime_error("client page not found: " + config.clientPageFile);
    }

    _clientPageContent = renderClientPage(TC_File::load2str(config.clientPageFile), config);
    if (_clientPageContent.empty())
    {
        throw runtime_error("client page is empty: " + config.clientPageFile);
    }
}

void WebSocketHttpImp::destroy()
{
}

int WebSocketHttpImp::doRequest(TarsCurrentPtr current, vector<char> &buffer)
{
    (void)buffer;

    TC_HttpRequest request;
    request.decode(current->getRequestBuffer());

    const string requestPath = request.getRequestUrl().empty() ? "/" : request.getRequestUrl();

    TC_HttpResponse response;
    if (requestPath == "/" || requestPath == "/index.html")
    {
        response.setResponse(200, "OK", _clientPageContent);
        response.setContentType("text/html; charset=utf-8");
    }
    else
    {
        response.setResponse(404, "Not Found", "WebSocketServer example page not found.");
        response.setContentType("text/plain; charset=utf-8");
    }

    response.setConnection("close");

    const string encoded = response.encode();
    current->sendResponse(encoded.c_str(), (uint32_t)encoded.size());
    return 0;
}
