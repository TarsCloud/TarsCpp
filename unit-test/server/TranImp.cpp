#include "TranImp.h"
#include "HelloServer.h"

using namespace tars;

TranImp::TranImp()
{
}

///////////////////////////////////////////////////////////////////////////////
void TranImp::initialize()
{
	Application::getCommunicator()->stringToProxy(g_TransDstServerObj, _servantPrx);
	ProxyProtocol proto;
	proto.requestFunc = ProxyProtocol::streamRequest;
	proto.responseFunc = ProxyProtocol::tupResponse;

	_servantPrx->tars_set_protocol(proto);
}

void TranImp::async_call(tars::CurrentPtr current, vector<char> &response)
{
    const vector<char> &request = current->getRequestBuffer();

    TarsInputStream<> is;
    is.setBuffer(request.data(), request.size());

    RequestPacket req;
    req.readFrom(is);

    ResponsePacket rsp;

    TLOGDEBUG("async_call: begin remote call, req len:" << req.sBuffer.size() << endl);
    /*a-sync-call*/
    ServantProxyCallbackPtr cb = new ServantCallback("ServantCallback", this, current);
	_servantPrx->rpc_call_async(req.iRequestId, req.sFuncName, request.data(), request.size(), cb);
    current->setResponse(false);
}

std::atomic<int> wup_trans_count(0);

int TranImp::doRequest(tars::CurrentPtr current, vector<char> &response) {

	++wup_trans_count;

    async_call(current, response);

    return 0;
}

int TranImp::doResponse(ReqMessagePtr resp) {
    ServantCallback *cb = dynamic_cast<ServantCallback *>(resp->callback.get());

    if (cb) {
        vector<char> &buff = resp->response->sBuffer;

        if (!buff.empty()) {
            TLOGDEBUG("end remote call, req len:" << buff.size() << endl);
            cb->getCurrent()->sendResponse(buff.data(), buff.size());
        }
    }

    return 0;
}

void TranImp::destroy() {
}

///////////////////////////////////////////////////////////////////////////////
