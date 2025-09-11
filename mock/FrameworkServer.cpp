#include "mock/FrameworkServer.h"
#include "mock/QueryImp.h"
#include "mock/ConfigImp.h"
#include "mock/QueryImp.h"
#include "mock/StatImp.h"
#include "mock/LogImp.h"
#include "mock/framework.h"

using namespace std;

vector<map<tars::StatMicMsgHead, tars::StatMicMsgBody>> _clientStatData;
vector<map<tars::StatMicMsgHead, tars::StatMicMsgBody>> _serverStatData;

FrameworkServer::~FrameworkServer()
{

}

void
FrameworkServer::initialize()
{
    CDbHandle::addActiveEndPoint("tars.tarsmock.ConfigObj", "127.0.0.1", 11003, 1);
    CDbHandle::addActiveEndPoint("tars.tarsmock.QueryObj", "127.0.0.1", 17890, 1);
    CDbHandle::addActiveEndPoint("tars.tarsmock.LogObj", "127.0.0.1", 11005, 1);
    CDbHandle::addActiveEndPoint("tars.tarsmock.StatObj", "127.0.0.1", 12004, 1);

    addServant<ConfigImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".ConfigObj");
    addServant<QueryImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".QueryObj");
    addServant<StatImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".StatObj");
    addServant<LogImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".LogObj");

    string s;
    loadLogFormat("","",s);

    //日志路径
    g_globe._log_path = _conf["/tars/log<logpath>"];

    //启动写线程
    g_globe._group.start(TC_Common::strto<size_t>(_conf["/tars/log<logthread>"]));

    string prefix = TC_Common::lower(_conf.get("/tars/log<ipfix>","true"));
    g_globe._bIpPrefix = (prefix == "true") ? true : false;

    //增加对象
    addServant<LogImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName +".LogObj");

    TARS_ADD_ADMIN_CMD_NORMAL("reloadLogFormat", FrameworkServer::loadLogFormat);
}


bool FrameworkServer::loadLogFormat(const string& command, const string& params, string& result)
{
    TLOGDEBUG("FrameworkServer::loadLogFormat command:" << command << "|params:" << params << endl);

    try
    {
        TC_Config conf;

        conf.parseString(MOCK_FRAMEWORK_CONFIG);

        vector<string> vHourlist;

        map<string,string> mLogType;

        try
        {
            string sHour = conf["/tars/log/format<hour>"];

            vHourlist = TC_Common::sepstr<string>(sHour,"|;,");

            sort(vHourlist.begin(),vHourlist.end());

            vHourlist.erase(unique(vHourlist.begin(),vHourlist.end()),vHourlist.end());

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