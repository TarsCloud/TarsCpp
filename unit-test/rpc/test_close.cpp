
#include "hello_test.h"
#include "servant/CommunicatorEpoll.h"
#include "servant/ObjectProxy.h"
#include "server/HelloImp.h"
#include "server/WinServer.h"

TEST_F(HelloTest, prxClose)
{
    auto comm = getCommunicator();

    WinServer ws;
    startServer(ws, WIN_CONFIG());

    string obj = getObj(ws.getConfig(), "WinAdapter");

    HelloPrx prx = comm->stringToProxy<HelloPrx>(obj);
    prx->testClose();

    EXPECT_EQ(HelloImp::_current.size(), 1);

    prx->tars_close();

    TC_Common::msleep(10);
    EXPECT_EQ(HelloImp::_current.size(), 0);

    stopServer(ws);

}

TEST_F(HelloTest, prxCloseInCoroutine)
{
    auto comm = getCommunicator();

    WinServer ws;
    startServer(ws, WIN_CONFIG());

    string obj = getObj(ws.getConfig(), "WinAdapter");

    HelloPrx prx = comm->stringToProxy<HelloPrx>(obj);
    prx->testClose();

    EXPECT_EQ(HelloImp::_current.size(), 1);

    funcInCoroutine([&](){
        prx->testClose();
    });

    EXPECT_EQ(HelloImp::_current.size(), 2);

    prx->tars_close();

    TC_Common::msleep(100);
    EXPECT_LE(HelloImp::_current.size(), 2);

    stopServer(ws);

}