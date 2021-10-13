
#include "hello_test.h"
#include "server/FrameworkServer.h"

int getStatCount(const vector<map<tars::StatMicMsgHead, tars::StatMicMsgBody>> &data)
{
	LOG_CONSOLE_DEBUG << "client stat:" << data.size() << endl;

	int sum = 0;

	for_each(data.begin(), data.end(), [&](const map<tars::StatMicMsgHead, tars::StatMicMsgBody> &r){
		for(auto e : r)
		{
//			LOG_CONSOLE_DEBUG << e.first.writeToJsonString() << ", " << e.second.writeToJsonString() << endl;
			sum += e.second.count ;
		}
	});

	return sum;
}

TEST_F(HelloTest, statReport)
{
	FrameworkServer fs;
	startServer(fs, FRAMEWORK_CONFIG());

	HelloServer hs;
	startServer(hs, CONFIG());

	_clientStatData.clear();

	shared_ptr<Communicator> c = getCommunicator();

	int totalReport = 0;
	int totalRealReport = 0;
	int count = 3;
	while(count-->0)
	{
		int report = rand() % 100;
		if(report == 0)
		{
			continue;
		}

		checkStat(c.get(), report);

		TC_Common::sleep(1);

		totalReport += report;

		totalRealReport = getStatCount(_clientStatData);

		LOG_CONSOLE_DEBUG << "report:" << report << ", totalReport:" << totalReport << ", totalRealReport:" << totalRealReport << ", " << _clientStatData.size()<< endl;

		ASSERT_TRUE(totalReport >= totalRealReport);

		ASSERT_TRUE(totalReport - totalRealReport <= 20);

	}

//	LOG_CONSOLE_DEBUG << "client stat:" << _clientStatData.size() << endl;
//	LOG_CONSOLE_DEBUG << "server stat:" << _serverStatData.size() << endl;

	stopServer(hs);
	stopServer(fs);
}


TEST_F(HelloTest, statReportInCoroutine)
{
	FrameworkServer fs;
	startServer(fs, FRAMEWORK_CONFIG());

	HelloServer hs;
	startServer(hs, CONFIG());

	_clientStatData.clear();

	shared_ptr<Communicator> c = getCommunicator();

	funcInCoroutine([=]()
	{
		int totalReport = 0;
		int totalRealReport = 0;
		int count = 3;
		while (count-- > 0)
		{
			int report = rand() % 100;
			if (report == 0)
			{
				continue;
			}

			checkStat(c.get(), report);

			TC_Common::sleep(1);

			totalReport += report;

			totalRealReport = getStatCount(_clientStatData);

			LOG_CONSOLE_DEBUG << "report:" << report << ", totalReport:" << totalReport << ", totalRealReport:"
							  << totalRealReport << ", data size:" << _clientStatData.size() << endl;

			ASSERT_TRUE(totalReport >= totalRealReport);

			ASSERT_TRUE(totalReport - totalRealReport <= 20);

		}
	}, true);

	stopServer(hs);
	stopServer(fs);
}