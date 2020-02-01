#ifndef __TEST_RECV_THREAD_H
#define __TEST_RECV_THREAD_H

#include "servant/Application.h"

class TestPushCallBack : public ServantProxyCallback
{
public:
	virtual int onDispatch(ReqMessagePtr msg);
};

// class TestPushCallBack : public tars::PushCallback
// {
// public:
//     virtual int onRequestException(int iRet);
//     virtual int onRequestResponse(const tars::RequestPacket& request, const tars::ResponsePacket& response);
//     virtual int onPushResponse(const tars::ResponsePacket& response);
// };


typedef tars::TC_AutoPtr<TestPushCallBack> TestPushCallBackPtr;

class RecvThread : public TC_Thread, public TC_ThreadLock
{
public:
	RecvThread();

	virtual void run();

	void terminate();
private:
	bool _bTerminate;

	Communicator _comm;

	ServantPrx  _prx;
};
#endif
