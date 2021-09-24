#include <iostream>
#include "servant/Application.h"
#include "util/tc_option.h"

using namespace tars;

Communicator *_comm;
string matchObj = "TestApp.HelloServer.HelloObj@tcp -h 127.0.0.1 -p 8999";

struct Param
{
	int count;
	int thread;

	ServantPrx pPrx;
};

Param param;
std::atomic<int> callback(0);

void ping(int c)
{
	for(int i = 0 ; i < c; i++)
	{
		param.pPrx->tars_ping();
		++callback;
	}
}

int main(int argc, char *argv[])
{
	try
	{
		if (argc < 2)
		{
			cout << "Usage:" << argv[0] << " --count=1000 --thread=1" << endl;

			return 0;
		}

		TC_Option option;
		option.decode(argc, argv);

		param.count = TC_Common::strto<int>(option.getValue("count"));
		if(param.count <= 0) param.count = 1000;
		param.thread = TC_Common::strto<int>(option.getValue("thread"));
		if(param.thread <= 0) param.thread = 1;

		_comm = new Communicator();

//        LocalRollLogger::getInstance()->logger()->setLogLevel(6);

		_comm->setProperty("sendqueuelimit", "1000000");
		_comm->setProperty("asyncqueuecap", "1000000");

		param.pPrx = _comm->stringToProxy<ServantPrx>(matchObj);

		param.pPrx->tars_connect_timeout(50000);
		param.pPrx->tars_set_timeout(60 * 1000);
		param.pPrx->tars_async_timeout(60*1000);

		int64_t start = TC_Common::now2us();

		std::function<void(int)> func;

		func = ping;

		vector<std::thread*> vt;
		for(int i = 0 ; i< param.thread; i++)
		{
			vt.push_back(new std::thread(func, param.count));
		}

		std::thread print([&]{while(callback != param.count * param.thread) {
			cout << "Call:" << " : ----------finish count:" << callback << endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		};});

		for(size_t i = 0 ; i< vt.size(); i++)
		{
			vt[i]->join();
			delete vt[i];
		}

		cout << "(pid:" << std::this_thread::get_id() << ")"
		     << "(count:" << param.count << ")"
		     << "(use ms:" << (TC_Common::now2us() - start)/1000 << ")"
		     << endl;


		while(callback != param.count * param.thread) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		print.join();
		cout << " ----------finish count:" << callback << endl;
	}
	catch(exception &ex)
	{
		cout << ex.what() << endl;
	}
	cout << "main return." << endl;

	return 0;
}
