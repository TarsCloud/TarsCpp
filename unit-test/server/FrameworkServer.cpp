#include "FrameworkServer.h"
#include "framework/ConfigImp.h"
#include "framework/QueryImp.h"
#include "framework/StatImp.h"
#include "framework/LogImp.h"

#include "HttpImp.h"
#include "HelloImp.h"

using namespace std;

vector<map<tars::StatMicMsgHead, tars::StatMicMsgBody>> _clientStatData;
vector<map<tars::StatMicMsgHead, tars::StatMicMsgBody>> _serverStatData;

FrameworkServer::~FrameworkServer()
{

}


void
FrameworkServer::initialize()
{
	addServant<ConfigImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".ConfigObj");
	addServant<QueryImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".QueryObj");
	addServant<StatImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".StatObj");
	addServant<LogImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".LogObj");

	string s;
	loadLogFormat("","",s);

	//日志路径
	g_globe._log_path = _conf["/tars/log<logpath>"];

	//启动写线程
	g_globe._group.start(TC_Common::strto<size_t>(_conf["/tars/log<logthread>"]));

	string prefix = TC_Common::lower(_conf.get("/tars/log<ipfix>","true"));
	g_globe._bIpPrefix = (prefix == "true") ? true : false;

	//增加对象
	addServant<LogImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".LogObj");

	TARS_ADD_ADMIN_CMD_NORMAL("reloadLogFormat", FrameworkServer::loadLogFormat);
}


bool FrameworkServer::loadLogFormat(const string& command, const string& params, string& result)
{
	TLOGDEBUG("FrameworkServer::loadLogFormat command:" << command << "|params:" << params << endl);

	try
	{
		TC_Config conf;

		conf.parseFile(ServerConfig::ConfigFile);

		vector<string> vHourlist;

		map<string,string> mLogType;

		try
		{
			string sHour = conf["/tars/log/format<hour>"];
            
			vHourlist = TC_Common::sepstr<string>(sHour,"|;,");

			sort(vHourlist.begin(),vHourlist.end());

			unique(vHourlist.begin(),vHourlist.end());

			result = "loadLogFormat succ:" + sHour;

			TLOGDEBUG("FrameworkServer::loadLogFormat result:" << result << endl);

			DLOG<< "FrameworkServer::loadLogFormat result:" << result << endl;

			//hour=app.server.file|app2.server2.file2
			map<string,string> mType;
			if(conf.getDomainMap("/tars/log/logtype", mType))
			{
				map<string,string>::iterator it = mType.begin();
				while(it != mType.end())
				{
					vector<string> vList = TC_Common::sepstr<string>(it->second,"|;,");
					for(size_t i = 0;i < vList.size();i++)
					{
						//app.server.file = hour
						mLogType[vList[i]] = it->first;

						TLOGDEBUG("FrameworkServer::loadLogFormat " << vList[i] << "|" << it->first << endl);

						DLOG<<"FrameworkServer::loadLogFormat " << vList[i] << "|" << it->first << endl;
					}
					it++;
				}
			}

			g_globe.update(vHourlist, mLogType);

		}
		catch(exception& e)
		{
			result +=  e.what();
			TLOGERROR("FrameworkServer::loadLogFormat command:" << command << "|params:" << params << "|result:" << result << endl);
		}

		return true;
	}
	catch(exception &e)
	{
		result +=  e.what();
		TLOGERROR("FrameworkServer::loadLogFormat command:" << command << "|params:" << params << "|result:" << result << endl);
	}

	return false;
}

void FrameworkServer::destroyApp()
{
}

void FrameworkServer::run()
{
	this->waitForShutdown();
}
/////////////////////////////////////////////////////////////////

RpcServer::~RpcServer()
{

}


void
RpcServer::initialize()
{
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".HelloObj");
	addServant<HttpImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".HttpObj");
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".HttpObj", &TC_NetWorkBuffer::parseHttp);
	
}

/////////////////////////////////////////////////////////////////
void RpcServer::destroyApp()
{
}

void RpcServer::run()
{
	this->waitForShutdown();
}