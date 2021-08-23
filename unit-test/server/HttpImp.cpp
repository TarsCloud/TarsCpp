#include "HttpImp.h"
//#include "HttpServer.h"
#include "servant/RemoteLogger.h"
#include "util/tc_http.h"

using namespace tars;

HttpImp::HttpImp()
{
}

int HttpImp::doRequest(tars::CurrentPtr current, vector<char>& response)
{
    const vector<char>& request = current->getRequestBuffer();

    // LOG_CONSOLE_DEBUG << string(request.data(), request.size()) << endl;

    TC_HttpRequest req;
    req.decode(request);

    TC_HttpResponse rsp;
    rsp.setResponse(200, "OK", req.getContent());
//    rsp.setContentType("text/html;charset=utf-8");
//    rsp.setConnection("close");
    string buffer = rsp.encode();

    // LOG_CONSOLE_DEBUG << buffer << endl;

    current->sendResponse(buffer.c_str(), (uint32_t)buffer.length());
//	current->close();

    return 0;
}

void HttpImp::initialize()
{
}

void HttpImp::destroy()
{
}

///////////////////////////////////////////////////////////////////////////////
