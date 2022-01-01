#include "HelloImp.h"
#include "HelloServer.h"
#include "servant/RemoteLogger.h"

using namespace tars;

std::atomic<int> hello_count(0);
std::atomic<int> trans_count(0);

HelloImp::HelloImp()
{
}

///////////////////////////////////////////////////////////////////////////////
void HelloImp::initialize()
{
    Application::getCommunicator()->stringToProxy(g_HelloServerObj, _helloPrx);

    _helloPrx->tars_async_timeout(100*1000);
}

void HelloImp::destroy()
{
    ++g_handleDestroy;
}

struct HelloCallback : public HelloPrxCallback
{
    HelloCallback(tars::CurrentPtr &current, int index, const string &s) : _current(current), _index(index), _s(s)
    {
    }

    //回调函数
    virtual void callback_testHello(int ret, const string &r)
    {
//	    LOG_CONSOLE_DEBUG << ret << endl;

        assert(ret == 0);

        Hello::async_response_testTrans(_current, ret, r);
    }

    virtual void callback_testHello_exception(tars::Int32 ret)
    {
        cout << "callback exception:" << ret << endl;
    }

    tars::CurrentPtr _current;
    int _index;
    string _s;
};

int HelloImp::testTrans(int index, const string &s, string &r, CurrentPtr current)
{
//	LOG_CONSOLE_DEBUG << index << endl;

	++trans_count;

    try
    {
        current->setResponse(false);

        HelloPrxCallbackPtr p = new HelloCallback(current, index, s);

        _helloPrx->async_testHello(p, index, s);
    }
    catch (exception &e)
    {
        cout << "exception:" << e.what() << endl;
    }

    return 0;
}


int HelloImp::testSyncTrans(int index, const string &s, string &r, CurrentPtr current)
{
	try
	{
		int ret = _helloPrx->testHello(index, s, r);

		return ret;
	}
	catch (exception &e)
	{
		cout << "exception:" << e.what() << endl;
	}

	return -1;
}

int HelloImp::testHello(int index, const string& s, string& r, CurrentPtr current)
{
//
//	if(index % 10000 == 0)
//	{
		// LOG_CONSOLE_DEBUG << index << endl;
//	}
	++hello_count;

	r = s;
    return 0;
}

int HelloImp::testPid(string &r, CurrentPtr current)
{
	r = TC_Common::tostr(this->getApplication());
	return 0;
}

int HelloImp::testTimeout(int timeout, CurrentPtr current)
{
	TC_Common::sleep(timeout);
	return 0;
}

int HelloImp::testDyeing(const std::string & strIn,std::string &strOut, CurrentPtr current)
{
	LOG_CONSOLE_DEBUG << strIn << endl;

	strOut="this is a dyeing message";

	TLOG_DEBUG(strOut << "1, debug log" << endl);
	FDLOG("test") << strOut << "1, day log" << endl;

	TLOG_DEBUG(strOut << "2, debug log" << endl);
	FDLOG("test") << strOut << "2, day log" << endl;

	return _helloPrx->testDyeingTrans();
}

tars::Int32 HelloImp::testDyeingTrans(CurrentPtr current)
{
	string strOut = "this is a dyeing trans message";

	TLOG_DEBUG(strOut << "1, debug log" << endl);
	FDLOG("test") << strOut << "1, day log" << endl;

	TLOG_DEBUG(strOut << "2, debug log" << endl);
	FDLOG("test") << strOut << "2, day log" << endl;

	return 0;
}

bool HelloImp::testCoro(const std::string& sIn, std::string &sOut, CurrentPtr current)
{
	return ServantProxyThreadData::getData()->_sched != NULL;
}

int HelloImp::testConHash(std::string &sOut, CurrentPtr current)
{
	sOut = _handle->getBindAdapter()->getEndpoint().toString();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
