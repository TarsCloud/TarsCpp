#include "servant/Message.h"
#include "servant/ServantProxy.h"
#include "servant/Communicator.h"

namespace tars
{

void ReqMessage::init(CallType eCallType, ServantProxy *prx)
{
	eStatus        = ReqMessage::REQ_REQ;
	eType          = eCallType;
	bFromRpc       = false;

	callback       = NULL;
	proxy          = prx;
	pObjectProxy   = NULL;

	request.iRequestId = 1;
	response       = std::make_shared<ResponsePacket>();
	sReqData       = std::make_shared<TC_NetWorkBuffer::Buffer>();
	pMonitor       = NULL;

	iBeginTime     = TNOWMS;
	iEndTime       = 0;
	adapter        = NULL;
	bPush          = false;
	sched          = NULL;
	iCoroId        = 0;
}

ReqMessage::~ReqMessage()
{
	if(deconstructor)
	{
		deconstructor();
	}

	if(pMonitor != NULL)
	{
		delete pMonitor;
		pMonitor = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
void ReqMonitor::wait()
{
	if(!bMonitorFin)
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (!bMonitorFin)
		{
			cond.wait(lock, [&]
					  { return bMonitorFin || msg->proxy->tars_communicator()->isTerminating(); });
		}
	}
}

void ReqMonitor::notify()
{
	std::unique_lock<std::mutex> lock(mutex);
	bMonitorFin = true;
	cond.notify_one();
}

}
