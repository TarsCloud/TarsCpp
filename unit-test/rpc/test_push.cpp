
#include "hello_test.h"
#include "../server/WinServer.h"
#include "../server/Push.h"

TEST_F(HelloTest, pushSyncGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		testPush(comm);
	}, c.get());
}

TEST_F(HelloTest, pushSyncGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		testPush(comm);
	}, c.get());
}

TEST_F(HelloTest, pushReconnectGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		testReconnect(comm);
	}, c.get());
}

class RegisterPushCallBack : public PushPrxCallback
{
public:
	virtual void callback_testPush(tars::Int32 ret,  const std::string& msg)
	{
		_msg = msg;
	}

    virtual void onClose(const TC_Endpoint& ep) {
        _close = true;
    };

    string _msg;
    bool _close = false;
};

typedef TC_AutoPtr<RegisterPushCallBack> RegisterPushCallBackPtr;

TEST_F(HelloTest, push)
{
	shared_ptr<Communicator> comm = getCommunicator();

	WinServer ws;
	startServer(ws, WIN_CONFIG());

	string obj = getObj(ws.getConfig(), "WinAdapter");

	HelloPrx prx = comm->stringToProxy<HelloPrx>(obj);

	ServantProxyCallbackPtr callback(new RegisterPushCallBack());

	prx->tars_set_push_callback(callback);

	string msg = TC_Common::now2str();

	prx->testPushRegister(msg);

	TC_Common::msleep(50);

	ASSERT_TRUE(((RegisterPushCallBack*)callback.get())->_msg == msg);

	stopServer(ws);
}


TEST_F(HelloTest, pushClose)
{
    shared_ptr<Communicator> comm = getCommunicator();

    WinServer ws;
    startServer(ws, WIN_CONFIG());

    string obj = getObj(ws.getConfig(), "WinAdapter");

    HelloPrx prx = comm->stringToProxy<HelloPrx>(obj);

    ServantProxyCallbackPtr callback(new RegisterPushCallBack);

    prx->tars_set_push_callback(callback);

    string msg = TC_Common::now2str();

    prx->testPushRegister(msg);

    TC_Common::msleep(50);

    ASSERT_TRUE(((RegisterPushCallBack*)callback.get())->_msg == msg);

    prx->tars_close();

    TC_Common::msleep(50);

    ASSERT_TRUE(((RegisterPushCallBack*)callback.get())->_close);

    stopServer(ws);
}
