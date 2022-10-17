/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */
#include <sstream>
#include "util/tc_option.h"
#include "util/tc_common.h"
#include "servant/KeepAliveNodeF.h"
#include "servant/Application.h"
#include "servant/AppProtocol.h"
#include "servant/AdminServant.h"
#include "servant/ServantHandle.h"
#include "servant/BaseF.h"
#include "servant/AppCache.h"
#include "servant/NotifyObserver.h"
#include "servant/AuthLogic.h"
#include "servant/CommunicatorFactory.h"

#include <signal.h>
#if TARGET_PLATFORM_LINUX
#include <sys/resource.h>
#endif

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <fcntl.h>
#endif


#if TARS_SSL
#include "util/tc_openssl.h"
#endif

static TC_RollLogger __out__;

#define NOTIFY_AND_WAIT(msg) { \
RemoteNotify::getInstance()->report(msg); \
std::this_thread::sleep_for(std::chrono::milliseconds(20)); \
}

namespace tars
{

std::string ServerConfig::TarsPath;         //服务路径
std::string ServerConfig::Application;      //应用名称
std::string ServerConfig::ServerName;       //服务名称,一个服务名称含一个或多个服务标识
std::string ServerConfig::LocalIp;          //本机IP
std::string ServerConfig::BasePath;         //应用程序路径，用于保存远程系统配置的本地目录
std::string ServerConfig::DataPath;         //应用程序路径，用于本地数据
std::string ServerConfig::Local;            //本地套接字
std::string ServerConfig::Node;             //本机node地址
std::string ServerConfig::Log;              //日志中心地址
std::string ServerConfig::Config;           //配置中心地址
std::string ServerConfig::Notify;           //信息通知中心
std::string ServerConfig::LogPath;          //logpath
int         ServerConfig::LogSize;          //log大小(字节)
int         ServerConfig::LogNum;           //log个数()
std::string ServerConfig::LogLevel;         //log日志级别
std::string ServerConfig::ConfigFile;       //框架配置文件路径
int         ServerConfig::ReportFlow = 1;       //是否服务端上报所有接口stat流量 0不上报 1上报 (用于非tars协议服务流量统计)
int         ServerConfig::IsCheckSet = 1;       //是否对按照set规则调用进行合法性检查 0,不检查，1检查
int        ServerConfig::OpenCoroutine = 0;    //是否启用协程处理方式
size_t      ServerConfig::CoroutineMemSize; //协程占用内存空间的最大大小
uint32_t    ServerConfig::CoroutineStackSize;   //每个协程的栈大小(默认128k)
bool        ServerConfig::ManualListen = false;     //手工启动监听端口
int         ServerConfig::NetThread = 1;               //servernet thread
bool        ServerConfig::CloseCout = true;
int         ServerConfig::BackPacketLimit = 0;
int         ServerConfig::BackPacketMin = 1024;
bool         ServerConfig::CheckBindAdapter = true;

#if TARS_SSL
std::string ServerConfig::CA;
std::string ServerConfig::Cert;
std::string ServerConfig::Key;
bool ServerConfig::VerifyClient = false;
std::string ServerConfig::Ciphers;
#endif

map<string, string> ServerConfig::Context;

#define SYNC_SERVER_CONFIG(x, y) \
if(_serverConfig.x != serverConfig.x) \
{	\
ServerConfig::y = _serverConfig.x; 	\
}	\
if(ServerConfig::y != serverConfig.x) \
{	\
_serverConfig.x = ServerConfig::y;	\
}

///////////////////////////////////////////////////////////////////////////////////////////
CommunicatorPtr                 Application::_communicator = NULL;

PropertyReportPtr g_pReportRspQueue;

/**上报服务端发送队列大小的间隔时间**/
#define REPORT_SEND_QUEUE_INTERVAL 10

///////////////////////////////////////////////////////////////////////////////////////////
Application::Application()
{
	_servantHelper = std::make_shared<ServantHelperManager>();

	_notifyObserver = std::make_shared<NotifyObserver>();

	setNotifyObserver(_notifyObserver);

#if TARGET_PLATFORM_WINDOWS    
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif
}

Application::~Application()
{
	if (_epollServer)
	{
		_epollServer->terminate();
		_epollServer = NULL;
	}
#if TARGET_PLATFORM_WINDOWS
	WSACleanup();
#endif
}

void Application::ApplicationConfig::copyToStatic()
{
	ServerConfig::TarsPath = tarsPath;
	ServerConfig::Application  = application;
	ServerConfig::ServerName = serverName;
	ServerConfig::LocalIp = localIp;
	ServerConfig::BasePath = basePath;
	ServerConfig::DataPath = dataPath;
	ServerConfig::Local = local;
	ServerConfig::Node = node;
	ServerConfig::Log = log;
	ServerConfig::Config = config;
	ServerConfig::Notify = notify;
	ServerConfig::LogPath = logPath;
	ServerConfig::LogSize = logSize;
	ServerConfig::LogNum = logNum;
	ServerConfig::LogLevel = logLevel;
	ServerConfig::ConfigFile = configFile;
	ServerConfig::ReportFlow = reportFlow;
	ServerConfig::IsCheckSet = isCheckSet;
	ServerConfig::OpenCoroutine = openCoroutine;
	ServerConfig::CoroutineMemSize = coroutineMemSize;
	ServerConfig::CoroutineStackSize = coroutineStackSize;
	ServerConfig::ManualListen = manualListen;
	ServerConfig::NetThread = netThread;
	ServerConfig::CloseCout = closeCout;
	ServerConfig::BackPacketLimit = backPacketLimit;
	ServerConfig::BackPacketMin = backPacketMin;
	ServerConfig::CheckBindAdapter = checkBindAdapter;
#if TARS_SSL
	std::string ServerConfig::CA = ca;
	std::string ServerConfig::Cert = cert;
	std::string ServerConfig::Key = key;
	bool ServerConfig::VerifyClient = verifyClient;
	std::string ServerConfig::Ciphers = ciphers;
#endif
}

string Application::getTarsVersion()
{
    return TARS_VERSION;
}

CommunicatorPtr& Application::getCommunicator()
{
    return _communicator;
}

CommunicatorPtr Application::getThisCommunicator()
{
	return _thisCommunicator;
}

void reportRspQueue(TC_EpollServer *epollServer)
{
    if (!g_pReportRspQueue)
        return;

    static time_t iLastCheckTime = TNOW;

    time_t iNow = TNOW;

    if (iNow - iLastCheckTime > REPORT_SEND_QUEUE_INTERVAL)
    {
        iLastCheckTime = iNow;

        const vector<TC_EpollServer::BindAdapterPtr> &adapters = epollServer->getBindAdapters();

        size_t n = 0;
        for (size_t i = 0; i < adapters.size(); ++i)
        {
            n = n + adapters[i]->getSendBufferSize();
        }

        g_pReportRspQueue->report((int)n);
    }
}

void heartBeatFunc(const string& adapterName)
{
    TARS_KEEPALIVE(adapterName);
}

void Application::manualListen()
{
    vector<TC_EpollServer::BindAdapterPtr> v = getEpollServer()->getBindAdapters();

    for(auto &b : v)
    {
        b->manualListen();
    }
}

void Application::waitForShutdown()
{
	assert(_epollServer);

    _epollServer->setCallbackFunctor(reportRspQueue);

    _epollServer->setHeartBeatFunctor(heartBeatFunc);

	_epollServer->setDestroyAppFunctor([&](TC_EpollServer *epollServer){

		this->destroyApp();

		NOTIFY_AND_WAIT("stop");
	});

	_epollServer->waitForShutdown();

	TC_Port::unregisterCtrlC(_ctrlCId);
	TC_Port::unregisterTerm(_termId);

	_epollServer = NULL;
}

void Application::waitForReady()
{
	if(_epollServer)
	{
		_epollServer->waitForReady();
	}
}

void Application::terminate()
{
    if (_epollServer && !_epollServer->isTerminate())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //稍微休息一下, 让当前处理包能够回复

        _epollServer->terminate();
    }
}

bool Application::cmdViewStatus(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdViewStatus:" << command << " " << params << endl);

    ostringstream os;

    os << OUT_LINE_LONG << endl;

    os << TC_Common::outfill("[proxy config]:") << endl;

    outClient(os);

    os << OUT_LINE << "\n" << TC_Common::outfill("[server config]:") << endl;

    outServer(os);

    os << OUT_LINE << endl;

    outAllAdapter(os);

    result = os.str();

    return true;
}

bool Application::cmdCloseCoreDump(const string& command, const string& params, string& result)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
    struct rlimit tlimit;
    int ret = 0;
    ostringstream os;

    ret = getrlimit(RLIMIT_CORE, &tlimit);
    if (ret != 0)
    {
        TLOGERROR("error: "<<strerror(errno)<<endl);
        return false;
    }

    TLOGDEBUG("before :cur:" << tlimit.rlim_cur << ";max: " << tlimit.rlim_max << endl);

    os << (_serverConfig.application + "." + _serverConfig.serverName);

    os << "|before set:cur:" << tlimit.rlim_cur << ";max: " << tlimit.rlim_max;

    string param = TC_Common::lower(TC_Common::trim(params));

    bool bClose = (param == "yes") ? true : false;

    if (bClose)
    {
        tlimit.rlim_cur = 0;
    }
    else
    {
        tlimit.rlim_cur = tlimit.rlim_max;
    }

    ret = setrlimit(RLIMIT_CORE, &tlimit);
    if (ret != 0)
    {
       TLOGERROR("error: "<<strerror(errno)<<endl);
       return false;
    }

    ret = getrlimit(RLIMIT_CORE, &tlimit);
    if (ret != 0)
    {
        TLOGERROR("error: "<<strerror(errno)<<endl);
        return false;
    }

    TLOGDEBUG("after cur:" << tlimit.rlim_cur << ";max: " << tlimit.rlim_max << endl);
    os << "|after set cur:" << tlimit.rlim_cur << ";max: " << tlimit.rlim_max << endl;

    result = os.str();
#else
	TLOGDEBUG("windows not support!");
#endif
    return true;
}

bool Application::cmdSetLogLevel(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdSetLogLevel:" << command << " " << params << endl);

    string level = TC_Common::trim(params);

    int ret = LocalRollLogger::getInstance()->logger()->setLogLevel(level);

    if (ret == 0)
    {
		_serverConfig.logLevel = TC_Common::upper(level);

		ServerConfig::LogLevel = _serverConfig.logLevel;

        result = "set log level [" + level + "] ok";

		_thisCommunicator->getAppCache()->set("logLevel", level);
    }
    else
    {
        result = "set log level [" + level + "] error";
    }

    return true;
}

bool Application::cmdEnableDayLog(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdEnableDayLog:" << command << " " << params << endl);

    vector<string> vParams = TC_Common::sepstr<string>(TC_Common::trim(params), "|");

    size_t nNum = vParams.size();

    if (!(nNum == 2 || nNum == 3))
    {
        result = "usage: tars.enabledaylog {remote|local}|[logname]|{true|false}";
        return false;
    }

    if((vParams[0] != "local" && vParams[0] != "remote"))
    {
        result = "usage: tars.enabledaylog {remote|local}|[logname]|{true|false}";
        return false;
    }

    if(nNum == 2 && (vParams[1] != "true" && vParams[1] != "false"))
    {
        result = "usage: tars.enabledaylog {remote|local}|[logname]|{true|false}";
        return false;
    }

    if(nNum == 3 && (vParams[2] != "true" && vParams[2] != "false"))
    {
        result = "usage: tars.enabledaylog {remote|local}|[logname]|{true|false}";
        return false;
    }

    bool bEnable = true;
    string sFile;


    if (nNum == 2)
    {
        bEnable = (vParams[1] == "true") ? true : false;
        sFile = "";
        result = "set " + vParams[0] + " " + vParams[1] + " ok";
    }
    else if (nNum == 3)
    {
        bEnable = (vParams[2] == "true") ? true : false;
        sFile = vParams[1];
        result = "set " + vParams[0] + " " + vParams[1] + " " + vParams[2] + " ok";
    }

    if (vParams[0] == "local")
    {
        RemoteTimeLogger::getInstance()->enableLocal(sFile, bEnable);
        return true;
    }

    if (vParams[0] == "remote")
    {
        RemoteTimeLogger::getInstance()->enableRemote(sFile, bEnable);
        return true;
    }

    result = "usage: tars.enabledaylog {remote|local}|[logname]|{true|false}";
    return false;

}

bool Application::cmdLoadConfig(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdLoadConfig:" << command << " " << params << endl);

    string filename = TC_Common::trim(params);

    if (RemoteConfig::getInstance()->addConfig(filename, result, false))
    {
        RemoteNotify::getInstance()->report(result);

        return true;
    }

    RemoteNotify::getInstance()->report(result);

    return true;
}

bool Application::cmdConnections(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdConnections:" << command << " " << params << endl);

    ostringstream os;

    os << OUT_LINE_LONG << endl;

    auto m = _epollServer->getListenSocketInfo();

    for (auto it = m.begin(); it != m.end(); ++it)
    {
        vector<TC_EpollServer::ConnStatus> v = it->second->getConnStatus();

        os << OUT_LINE << "\n" << TC_Common::outfill("[adapter:" + it->second->getName() + "] [connections:" + TC_Common::tostr(v.size()) + "]") << endl;

        os  << TC_Common::outfill("conn-uid", ' ', 15)
            << TC_Common::outfill("ip:port", ' ', 25)
            << TC_Common::outfill("last-time", ' ', 25)
            << TC_Common::outfill("timeout", ' ', 10)
	        << TC_Common::outfill("recvBufferSize", ' ', 30)
	        << TC_Common::outfill("sendBufferSize", ' ', 30)
	        << endl;

        for (size_t i = 0; i < v.size(); i++)
        {
            os  << TC_Common::outfill(TC_Common::tostr<uint32_t>(v[i].uid), ' ', 15)
                << TC_Common::outfill(v[i].ip + ":" + TC_Common::tostr(v[i].port), ' ', 25)
                << TC_Common::outfill(TC_Common::tm2str(v[i].iLastRefreshTime, "%Y-%m-%d %H:%M:%S"), ' ', 25)
                << TC_Common::outfill(TC_Common::tostr(v[i].timeout), ' ', 10)
	            << TC_Common::outfill(TC_Common::tostr(v[i].recvBufferSize), ' ', 30)
	            << TC_Common::outfill(TC_Common::tostr(v[i].sendBufferSize), ' ', 30)
	            << endl;
        }
    }
    os << OUT_LINE_LONG << endl;

    result = os.str();

    return true;
}

bool Application::cmdViewVersion(const string& command, const string& params, string& result)
{
    result = "$" + string(TARS_VERSION) + "$";
    return true;
}

bool Application::cmdViewBuildID(const string& command, const string& params, string& result)
{
    #define YEARSUF ((__DATE__ [9] - '0') * 10 + (__DATE__ [10] - '0'))

    #define MONTH (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 0 : 5)  \
        : __DATE__ [2] == 'b' ? 1 \
        : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 2 : 3) \
        : __DATE__ [2] == 'y' ? 4 \
        : __DATE__ [2] == 'l' ? 6 \
        : __DATE__ [2] == 'g' ? 7 \
        : __DATE__ [2] == 'p' ? 8 \
        : __DATE__ [2] == 't' ? 9 \
        : __DATE__ [2] == 'v' ? 10 : 11)

    #define DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
        + (__DATE__ [5] - '0'))

	#define TIMEINT ((((((__TIME__[0] - '0') * 10 + (__TIME__[1] - '0')) * 10 \
        + (__TIME__[3] - '0')) * 10 + (__TIME__[4] - '0')) * 10 \
        + (__TIME__[6] - '0')) * 10 + (__TIME__[7] - '0'))

    char buildTime[50] = {0};
    sprintf(buildTime, "%d.%02d%02d.%06d", YEARSUF, MONTH + 1, DAY, TIMEINT);
    result = "$" + _serverConfig.application + "." + _serverConfig.serverName + "-" + string(buildTime) + "$";
    return true;
}

bool Application::cmdLoadProperty(const string& command, const string& params, string& result)
{
    try
    {
        TLOGTARS("Application::cmdLoadProperty:" << command << " " << params << endl);

        //重新解析配置文件
        _conf.parseFile(_serverConfig.configFile);

        string sResult = "";

        //加载通讯器属性
        _thisCommunicator->setProperty(_conf);

        _thisCommunicator->reloadProperty(sResult);

        //加载远程对象
		_serverConfig.log = _conf.get("/tars/application/server<log>");
		ServerConfig::Log = _serverConfig.log;

        RemoteTimeLogger::getInstance()->setLogInfo(_thisCommunicator, _serverConfig.log, _serverConfig.application, _serverConfig.serverName, _serverConfig.logPath,setDivision());

        _serverConfig.config = _conf.get("/tars/application/server<config>");
		ServerConfig::Config = _serverConfig.config;

        RemoteConfig::getInstance()->setConfigInfo(_thisCommunicator, _serverConfig.config, _serverConfig.application, _serverConfig.serverName, _serverConfig.basePath,setDivision(), 5);

        _serverConfig.notify = _conf.get("/tars/application/server<notify>");
		ServerConfig::Notify = _serverConfig.notify;

        RemoteNotify::getInstance()->setNotifyInfo(_thisCommunicator, _serverConfig.notify, _serverConfig.application, _serverConfig.serverName, setDivision(), _serverConfig.localIp);

        result = "loaded config items:\r\n" + sResult +
                 "log=" + _serverConfig.log + "\r\n" +
                 "config=" + _serverConfig.config + "\r\n" +
                 "notify=" + _serverConfig.notify + "\r\n";
    }
    catch (TC_Config_Exception & ex)
    {
        result = "load config " + _serverConfig.configFile + " error:" + ex.what();
    }
    catch (exception &ex)
    {
        result = ex.what();
    }
    return true;
}

bool Application::cmdViewAdminCommands(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdViewAdminCommands:" << command << " " << params << endl);

    result = result +  _notifyObserver->viewRegisterCommand();

    return true;
}

bool Application::cmdSetDyeing(const string& command, const string& params, string& result)
{
    vector<string> vDyeingParams = TC_Common::sepstr<string>(params, " ");

    if (vDyeingParams.size() == 2 || vDyeingParams.size() == 3)
    {
	    _servantHelper->setDyeing(vDyeingParams[0], vDyeingParams[1], vDyeingParams.size() == 3 ? vDyeingParams[2] : "");

        result = "DyeingKey="       + vDyeingParams[0] + "\r\n" +
                 "DyeingServant="   + vDyeingParams[1] + "\r\n" +
                 "DyeingInterface=" + (vDyeingParams.size() == 3 ? vDyeingParams[2] : "") + "\r\n";
    }
    else
    {
        result = "Invalid parameters.Should be: dyeingKey dyeingServant [dyeingInterface]";
    }
    return true;
}

bool Application::cmdCloseCout(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdCloseCout:" << command << " " << params << endl);

    string s = TC_Common::lower(TC_Common::trim(params));

    if (s == "yes")
    {
        _thisCommunicator->getAppCache()->set("closeCout", "1");
    }
    else
    {
		_thisCommunicator->getAppCache()->set("closeCout", "0");
    }

    result = "set closeCout  [" + s + "] ok";

    return true;
}

bool Application::cmdReloadLocator(const string& command, const string& params, string& result)
{
    TLOGDEBUG("Application::cmdReloadLocator:" << command << " " << params << endl);

    string sPara = TC_Common::lower(TC_Common::trim(params));

    bool bSucc(true);
    if (sPara == "reload")
    {
    	TLOGDEBUG(__FUNCTION__ << "|" << __LINE__ << "|conf file:" << _serverConfig.configFile << endl);

        TC_Config reloadConf;

        reloadConf.parseFile(_serverConfig.configFile);
        string sLocator = reloadConf.get("/tars/application/client/<locator>", "");

        TLOGDEBUG(__FUNCTION__ << "|" << __LINE__ << "|conf file:" << _serverConfig.configFile << "\n"
            << "|sLocator:" << sLocator << endl);

        if (sLocator.empty())
        {
            bSucc = false;
            result = "locator info is null.";
        }
        else
        {
            _thisCommunicator->setProperty("locator", sLocator);
            _thisCommunicator->reloadLocator();
            result = sLocator + " set succ.";
        }

    }
    else
    {
        result = "please input right paras.";
        bSucc = false;
    }

    return bSucc;
}

bool Application::cmdViewResource(const string& command, const string& params, string& result)
{
	TLOGDEBUG("Application::cmdViewResource:" << command << " " << params << endl);

	ostringstream os;

	os << _thisCommunicator->getResourcesInfo() << endl;

	os << OUT_LINE << endl;

	vector<TC_EpollServer::BindAdapterPtr> adapters = _epollServer->getBindAdapters();
	for(auto adapter : adapters)
	{
		outAdapter(os, _servantHelper->getAdapterServant(adapter->getName()), adapter);
		os << TC_Common::outfill("recv-buffer-count") << adapter->getRecvBufferSize() << endl;
		os << TC_Common::outfill("send-buffer-count") << adapter->getSendBufferSize() << endl;
	}

	result += os.str();

	TLOGDEBUG("Application::cmdViewResource result:" << result << endl);

	return true;
}

void Application::outAllAdapter(ostream &os)
{
    auto m = _epollServer->getListenSocketInfo();

    for (auto it = m.begin(); it != m.end(); ++it)
    {
        outAdapter(os, _servantHelper->getAdapterServant(it->second->getName()), it->second);

        os << OUT_LINE << endl;
    }
}

bool Application::addConfig(const string &filename)
{
    string result;

    if (RemoteConfig::getInstance()->addConfig(filename, result, false))
    {
        RemoteNotify::getInstance()->report(result);

        return true;
    }
    RemoteNotify::getInstance()->report(result);

    return true;
}

bool Application::addAppConfig(const string &filename)
{
    string result = "";

    // true-只获取应用级别配置
    if (RemoteConfig::getInstance()->addConfig(filename, result, true))

    {
        RemoteNotify::getInstance()->report(result);

        return true;
    }

    RemoteNotify::getInstance()->report(result);

    return true;
}

void Application::main(int argc, char *argv[])
{
    TC_Option op;
    op.decode(argc, argv);

    main(op);
}

void Application::main(const TC_Option &option)
{
    __out__.modFlag(0xfffff, false);
    //直接输出编译的TAF版本
    if (option.hasParam("version"))
	{
		__out__.debug() << "TARS:" << TARS_VERSION << endl;
		exit(0);
	}

	//加载配置文件
	_serverConfig.configFile = option.getValue("config");

	if (_serverConfig.configFile == "")
	{
		cerr << "start server with config, for example: exe --config=config.conf" << endl;

		exit(-1);
	}

	string config = TC_File::load2str(_serverConfig.configFile);

	__out__.debug() << "config:" << _serverConfig.configFile << endl;
	__out__.debug() << "config:" << config << endl;

	main(config);
}

void Application::main(const string &config)
{

    try
    {
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
        TC_Common::ignorePipe();
#endif
    	__out__.modFlag(0xFFFF, false);

        //解析配置文件
        parseConfig(config);

        //初始化Proxy部分
        initializeClient();

        //初始化Server部分
        initializeServer();

        vector <TC_EpollServer::BindAdapterPtr> adapters;

        //绑定对象和端口
        bindAdapter(adapters);

        stringstream os;

        //输出所有adapter
        outAllAdapter(os);

	    __out__.info() << os.str();

	    __out__.info() << "\n" << TC_Common::outfill("[initialize server] ", '.')  << " [Done]" << endl;

	    __out__.info() << OUT_LINE_LONG << endl;

        {
            bool initing = true;
            std::mutex mtx;
            std::condition_variable cond;

            std::thread keepActiving([&]
            {
                do
                {
                    //发送心跳给node, 表示正在启动
                    TARS_KEEPACTIVING;

                    //等待initialize初始化完毕
                    std::unique_lock<std::mutex> lock(mtx);
                    cond.wait_for(lock, std::chrono::seconds(5), [&](){
                    	return !initing;
                    });

				}while(initing);

            });

			try
            {
                //业务应用的初始化
                initialize();

                {
                    std::unique_lock<std::mutex> lock(mtx);
					initing = false;
                    cond.notify_all();
                }

                keepActiving.join();
			}
            catch (exception & ex)
            {
                keepActiving.detach();

				NOTIFY_AND_WAIT("exit: " + string(ex.what()));

	            __out__.error()  << "[init exception]:" << ex.what() << endl;
                exit(-1);
            }
        }

        //动态加载配置文件
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_LOAD_CONFIG, Application::cmdLoadConfig);

        //动态设置滚动日志等级
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_SET_LOG_LEVEL, Application::cmdSetLogLevel);

        //动态设置按天日志等级
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_SET_DAYLOG_LEVEL, Application::cmdEnableDayLog);

        //查看服务状态
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_VIEW_STATUS, Application::cmdViewStatus);

        //查看当前链接状态
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_CONNECTIONS, Application::cmdConnections);

        //查看编译的TARS版本
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_VIEW_VERSION, Application::cmdViewVersion);

        //查看服务buildid(编译时间）
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_VIEW_BID, Application::cmdViewBuildID);

        //加载配置文件中的属性信息
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_LOAD_PROPERTY, Application::cmdLoadProperty);

        //查看服务支持的管理命令
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_VIEW_ADMIN_COMMANDS, Application::cmdViewAdminCommands);

        //设置染色信息
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_SET_DYEING, Application::cmdSetDyeing);

        //设置服务的core limit
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_CLOSE_CORE, Application::cmdCloseCoreDump);

        //设置是否标准输出
        TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_CLOSE_COUT, Application::cmdCloseCout);

	    //设置是否标准输出
	    TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_RELOAD_LOCATOR, Application::cmdReloadLocator);

	    //设置是否标准输出
	    TARS_ADD_ADMIN_CMD_PREFIX(TARS_CMD_RESOURCE, Application::cmdViewResource);

        //上报版本
        TARS_REPORTVERSION(TARS_VERSION);

        //发送心跳给node, 表示启动了
        TARS_KEEPALIVE("");

        //发送给notify表示服务启动了
        RemoteNotify::getInstance()->report("restart");

        //ctrl + c能够完美结束服务
		_ctrlCId = TC_Port::registerCtrlC([=]{
			this->terminate();
#if TARGET_PLATFORM_WINDOWS
			ExitProcess(0);
#endif
		});
	    _termId = TC_Port::registerTerm([=]{
		    this->terminate();
#if TARGET_PLATFORM_WINDOWS
		    ExitProcess(0);
#endif
	    });

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
        if(_conf.get("/tars/application/server<closecout>", _thisCommunicator->getAppCache()->get("closeCout")) != "0")
        {
            // 重定向stdin、stdout、stderr
            int fd = open("/dev/null", O_RDWR );
            if (fd != -1)
            {
                dup2(fd, 0);
                dup2(fd, 1);
                dup2(fd, 2);
            }
            else
            {
                close(0);
                close(1);
                close(2);
            }
        }
#endif        
    }
    catch (exception &ex)
    {
	    __out__.error()  << "[Application]:" << ex.what() << endl;

	    terminate();

		NOTIFY_AND_WAIT("exit: " + string(ex.what()));

        exit(-1);
    }

	//初始化完毕后, 日志再修改为异步
    LocalRollLogger::getInstance()->sync(false);
}

void Application::parseConfig(const string &config)
{
    _conf.parseString(config);

	__out__.setLogLevel(_conf.get("/tars/application/server<start_output>", "DEBUG"));

    onParseConfig(_conf);
}

TC_EpollServer::BindAdapter::EOrder Application::parseOrder(const string &s)
{
    vector<string> vtOrder = TC_Common::sepstr<string>(s, ";, \t", false);

    if (vtOrder.size() != 2)
    {
        cerr << "invalid order '" << TC_Common::tostr(vtOrder) << "'." << endl;

        exit(0);
    }
    if ((TC_Common::lower(vtOrder[0]) == "allow") && (TC_Common::lower(vtOrder[1]) == "deny"))
    {
        return TC_EpollServer::BindAdapter::ALLOW_DENY;
    }
    if ((TC_Common::lower(vtOrder[0]) == "deny") && (TC_Common::lower(vtOrder[1]) == "allow"))
    {
        return TC_EpollServer::BindAdapter::DENY_ALLOW;
    }

    cerr << "invalid order '" << TC_Common::tostr(vtOrder) << "'." << endl;

    exit(0);
}

void Application::initializeClient()
{
	__out__.info()  << "\n" << OUT_LINE_LONG << endl;

	if(CommunicatorFactory::getInstance()->hasCommunicator())
	{
		//已经存在缺省通信器了, 创建新的通信器
		_thisCommunicator = CommunicatorFactory::getInstance()->getCommunicator(_conf, "application-" + TC_Common::tostr(this));
	}
	else
	{
		//初始化缺省通信器
		_communicator = CommunicatorFactory::getInstance()->getCommunicator(_conf);

		_thisCommunicator = _communicator;
	}

	__out__.info()  << TC_Common::outfill("[proxy config]:") << endl;

    //输出
    stringstream os;
    outClient(os);

	__out__.info() << os.str();
}

void Application::outClient(ostream &os)
{
	os << OUT_LINE << "\n" << TC_Common::outfill("[load client]:") << endl;

    os << TC_Common::outfill("locator")                     << _thisCommunicator->getProperty("locator") << endl;
    os << TC_Common::outfill("sync-invoke-timeout")         << _thisCommunicator->getProperty("sync-invoke-timeout") << endl;
    os << TC_Common::outfill("async-invoke-timeout")        << _thisCommunicator->getProperty("async-invoke-timeout") << endl;
    os << TC_Common::outfill("refresh-endpoint-interval")   << _thisCommunicator->getProperty("refresh-endpoint-interval") << endl;
    os << TC_Common::outfill("stat")                        << _thisCommunicator->getProperty("stat") << endl;
    os << TC_Common::outfill("property")                    << _thisCommunicator->getProperty("property") << endl;
    os << TC_Common::outfill("report-interval")             << _thisCommunicator->getProperty("report-interval") << endl;
    os << TC_Common::outfill("keep-alive-interval")         << _thisCommunicator->getProperty("keep-alive-interval") << endl;
    os << TC_Common::outfill("netthread")                  	<< _thisCommunicator->getProperty("netthread") << endl;
    os << TC_Common::outfill("asyncthread")                 << _thisCommunicator->getProperty("asyncthread") << endl;
    os << TC_Common::outfill("modulename")                  << _thisCommunicator->getProperty("modulename") << endl;
    os << TC_Common::outfill("enableset")                   << _thisCommunicator->getProperty("enableset") << endl;
    os << TC_Common::outfill("setdivision")                 << _thisCommunicator->getProperty("setdivision") << endl;
}

string Application::toDefault(const string &s, const string &sDefault)
{
    if (s.empty())
    {
        return sDefault;
    }
    return s;
}

string Application::setDivision()
{
    bool bEnableSet = TC_Common::lower(_conf.get("/tars/application<enableset>", "n"))=="y"?true:false;;

    string sSetDevision = bEnableSet?_conf.get("/tars/application<setdivision>", ""):"";
    return sSetDevision;
}

void Application::addServantProtocol(const string& servant, const TC_NetWorkBuffer::protocol_functor& protocol)
{
    string adapterName = _servantHelper->getServantAdapter(servant);

    if (adapterName == "")
    {
        throw runtime_error("addServantProtocol fail, no found adapter for servant:" + servant);
    }
    getEpollServer()->getBindAdapter(adapterName)->setProtocol(protocol);
}

void Application::addAcceptCallback(const TC_EpollServer::accept_callback_functor& cb)
{
    _acceptFuncs.push_back(cb);
}

void Application::onAccept(TC_EpollServer::Connection* cPtr)
{
    for (size_t i = 0; i < _acceptFuncs.size(); ++i)
    {
        _acceptFuncs[i](cPtr);
    }
}

void Application::outServer(ostream &os)
{
	os << TC_Common::outfill("Application(app)")            << _serverConfig.application << endl;
	os << TC_Common::outfill("ServerName(server)")          << _serverConfig.serverName << endl;
	os << TC_Common::outfill("BasePath(basepath)")          << _serverConfig.basePath << endl;
	os << TC_Common::outfill("DataPath(datapath)")          << _serverConfig.dataPath << endl;
	os << TC_Common::outfill("LocalIp(localip)")            << _serverConfig.localIp << endl;
	os << TC_Common::outfill("Local(local)")                << _serverConfig.local << endl;
	os << TC_Common::outfill("LogPath(logpath)")            << _serverConfig.logPath << endl;
	os << TC_Common::outfill("LogSize(logsize)")            << _serverConfig.logSize << endl;
	os << TC_Common::outfill("LogNum(lognum)")              << _serverConfig.logNum << endl;
	os << TC_Common::outfill("LogLevel(loglevel)")          << _serverConfig.logLevel << endl;
	os << TC_Common::outfill("Log(log)")                    << _serverConfig.log << endl;
	os << TC_Common::outfill("Node(node)")                  << _serverConfig.node << endl;
	os << TC_Common::outfill("Config(config)")              << _serverConfig.config << endl;
	os << TC_Common::outfill("Notify(notify)")              << _serverConfig.notify << endl;
	os << TC_Common::outfill("OpenCoroutine(opencoroutine)")      << _serverConfig.openCoroutine << endl;
	os << TC_Common::outfill("CoroutineMemSize(coroutinememsize)")   << _serverConfig.coroutineMemSize << endl;
	os << TC_Common::outfill("CoroutineStackSize(coroutinestack)") << _serverConfig.coroutineStackSize << endl;
	os << TC_Common::outfill("CloseCout(closecout)")          << _serverConfig.closeCout << endl;
	os << TC_Common::outfill("NetThread(netthread)")          << _serverConfig.netThread << endl;
	os << TC_Common::outfill("ManualListen(manuallisten)")       << _serverConfig.manualListen << endl;
	os << TC_Common::outfill("ReportFlow(reportflow)")                  << _serverConfig.reportFlow<< endl;
	os << TC_Common::outfill("BackPacketLimit(backpacketlimit)")  << _serverConfig.backPacketLimit<< endl;
	os << TC_Common::outfill("BackPacketMin(backpacketmin)")  << _serverConfig.backPacketMin<< endl;

#if TARS_SSL
	os << TC_Common::outfill("Ca(ca)")                    << _serverConfig.ca << endl;
	os << TC_Common::outfill("Cert(cert)")              << _serverConfig.cert << endl;
	os << TC_Common::outfill("Key(key)")                  << _serverConfig.key << endl;
	os << TC_Common::outfill("VerifyClient(verifyclient)")      << _serverConfig.verifyClient << endl;
	os << TC_Common::outfill("Ciphers(ciphers)")               << _serverConfig.ciphers << endl;
#endif

}

void Application::syncServerConfig(const ApplicationConfig &serverConfig)
{
	SYNC_SERVER_CONFIG(tarsPath, TarsPath);
	SYNC_SERVER_CONFIG(application, Application);
	SYNC_SERVER_CONFIG(serverName, ServerName);
	SYNC_SERVER_CONFIG(localIp, LocalIp);
	SYNC_SERVER_CONFIG(basePath, BasePath);
	SYNC_SERVER_CONFIG(dataPath, DataPath);
	SYNC_SERVER_CONFIG(local, Local);
	SYNC_SERVER_CONFIG(node, Node);
	SYNC_SERVER_CONFIG(log, Log);
	SYNC_SERVER_CONFIG(config, Config);
	SYNC_SERVER_CONFIG(notify, Notify);
	SYNC_SERVER_CONFIG(logPath, LogPath);
	SYNC_SERVER_CONFIG(logSize, LogSize);
	SYNC_SERVER_CONFIG(logNum, LogNum);
	SYNC_SERVER_CONFIG(logLevel, LogLevel);
	SYNC_SERVER_CONFIG(configFile, ConfigFile);
	SYNC_SERVER_CONFIG(reportFlow, ReportFlow);
	SYNC_SERVER_CONFIG(isCheckSet, IsCheckSet);
	SYNC_SERVER_CONFIG(openCoroutine, OpenCoroutine);
	SYNC_SERVER_CONFIG(coroutineMemSize, CoroutineMemSize);
	SYNC_SERVER_CONFIG(coroutineStackSize, CoroutineStackSize);
	SYNC_SERVER_CONFIG(manualListen, ManualListen);
	SYNC_SERVER_CONFIG(netThread, NetThread);
	SYNC_SERVER_CONFIG(closeCout, CloseCout);
	SYNC_SERVER_CONFIG(backPacketLimit, BackPacketLimit);
	SYNC_SERVER_CONFIG(backPacketMin, BackPacketMin);
	SYNC_SERVER_CONFIG(checkBindAdapter, CheckBindAdapter);

#if TARS_SSL
	SYNC_SERVER_CONFIG(ca, CA);
	SYNC_SERVER_CONFIG(cert, Cert);
	SYNC_SERVER_CONFIG(key, Key);
	SYNC_SERVER_CONFIG(verifyClient, VerifyClient);
	SYNC_SERVER_CONFIG(ciphers, Ciphers);
#endif
}

void Application::initializeServer()
{
	__out__.info()  << OUT_LINE << "\n" << TC_Common::outfill("[server config]:") << endl;

    _serverConfig.application  = toDefault(_conf.get("/tars/application/server<app>"), "UNKNOWN");

    //缺省采用进程名称
    string exe = "";

    try
    {
        exe = TC_File::extractFileName(TC_File::getExePath());
    }
    catch (TC_File_Exception & ex)
    {
        //取失败则使用ip代替进程名
        exe = _conf.get("/tars/application/server<localip>");
    }

    _serverConfig.serverName        = toDefault(_conf.get("/tars/application/server<server>"), exe);


#if TARGET_PLATFORM_WINDOWS    
    _serverConfig.basePath          = TC_File::simplifyDirectory(_conf.get("/tars/application/server<basepath.win>")) + FILE_SEP;
    if (_serverConfig.basePath == FILE_SEP)
    {
        _serverConfig.basePath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<basepath>"), ".")) + FILE_SEP;
    }

    _serverConfig.dataPath          = TC_File::simplifyDirectory(_conf.get("/tars/application/server<datapath.win>")) + FILE_SEP;
    if(_serverConfig.dataPath == FILE_SEP)
    {
        _serverConfig.dataPath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<datapath>"), ".")) + FILE_SEP;
    }

    _serverConfig.logPath           = TC_File::simplifyDirectory(_conf.get("/tars/application/server<logpath.win>")) + FILE_SEP;
    if(_serverConfig.logPath == FILE_SEP)
    {
        _serverConfig.logPath       = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<logpath>"),  ".")) + FILE_SEP;
    }
#else
    _serverConfig.basePath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<basepath>"), ".")) + FILE_SEP;
    _serverConfig.dataPath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<datapath>"), ".")) + FILE_SEP;
    _serverConfig.logPath           = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<logpath>"),  ".")) + FILE_SEP;

#endif
    _serverConfig.tarsPath           = TC_File::simplifyDirectory(_serverConfig.logPath + FILE_SEP + ".." + FILE_SEP) + FILE_SEP;
    _serverConfig.logSize           = TC_Common::toSize(toDefault(_conf.get("/tars/application/server<logsize>"), "52428800"), 52428800);
    _serverConfig.logNum            = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<lognum>"), "10"));
    _serverConfig.localIp           = _conf.get("/tars/application/server<localip>");
    _serverConfig.local             = _conf.get("/tars/application/server<local>");
    _serverConfig.node              = _conf.get("/tars/application/server<node>");
    _serverConfig.log               = _conf.get("/tars/application/server<log>");
    _serverConfig.config            = _conf.get("/tars/application/server<config>");
    _serverConfig.notify            = _conf.get("/tars/application/server<notify>");
    _serverConfig.reportFlow        = _conf.get("/tars/application/server<reportflow>")=="0"?0:1;
    _serverConfig.isCheckSet        = _conf.get("/tars/application/server<checkset>","1")=="0"?0:1;
    _serverConfig.openCoroutine     = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<opencoroutine>"), "0"));
    _serverConfig.coroutineMemSize  =  TC_Common::toSize(toDefault(_conf.get("/tars/application/server<coroutinememsize>"), "1G"), 1024*1024*1024);
    _serverConfig.coroutineStackSize= (uint32_t)TC_Common::toSize(toDefault(_conf.get("/tars/application/server<coroutinestack>"), "128K"), 1024*128);
    _serverConfig.manualListen      = _conf.get("/tars/application/server<manuallisten>", "0") == "0" ? false : true;
	_serverConfig.netThread         = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<netthread>"), "1"));
	_serverConfig.closeCout        	= _conf.get("/tars/application/server<closecout>","1")=="0"?0:1;
	_serverConfig.backPacketLimit  	= TC_Common::strto<int>(_conf.get("/tars/application/server<backpacketlimit>", TC_Common::tostr(100*1024*1024)));
	_serverConfig.backPacketMin    	= TC_Common::strto<int>(_conf.get("/tars/application/server<backpacketmin>", "1024"));
	_serverConfig.checkBindAdapter	= _conf.get("/tars/application/server<checkbindadapter>","1")=="0"?false:true;

#if TARS_SSL
	_serverConfig.ca                = _conf.get("/tars/application/server<ca>");
	_serverConfig.cert              = _conf.get("/tars/application/server<cert>");
	_serverConfig.key               = _conf.get("/tars/application/server<key>");
	_serverConfig.verifyClient      = _conf.get("/tars/application/server<verifyclient>","0")=="0"?false:true;
	_serverConfig.ciphers           = _conf.get("/tars/application/server<ciphers>");

	if(!_serverConfig.Cert.empty()) {
		_ctx = TC_OpenSSL::newCtx(_serverConfig.ca, _serverConfig.cert, _serverConfig.key, _serverConfig.verifyClient, _serverConfig.ciphers);

		if (!_ctx) {
			TLOGERROR("[load server ssl error, ca:" << _serverConfig.ca << endl);
			exit(-1);
		}
	}
#endif

    if (_serverConfig.localIp.empty())
    {
        vector<string> v = TC_Socket::getLocalHosts();

        _serverConfig.localIp = "127.0.0.1";
        //获取第一个非127.0.0.1的IP
        for(size_t i = 0; i < v.size(); i++)
        {
            if(v[i] != "127.0.0.1")
            {
                _serverConfig.localIp = v[i];
                break;
            }
        }
    }

	ServerConfig::Context["node_name"] = _serverConfig.localIp;

	//保存之前的
	ApplicationConfig serverConfig = _serverConfig;

	//配置文件copy到静态
	_serverConfig.copyToStatic();

	//让业务有修改配置的机会
    onServerConfig();

	//检查变更
	syncServerConfig(serverConfig);

	ostringstream os;
    //输出信息
    outServer(os);

	__out__.info() << os.str();

	if (_serverConfig.netThread < 1)
    {
        _serverConfig.netThread = 1;
	    __out__.info()  << OUT_LINE << "\nwarning:netThreadNum < 1." << endl;
    }

    //网络线程的配置数目不能15个
    if (_serverConfig.netThread > 15)
    {
        _serverConfig.netThread = 15;
	    __out__.info()  << OUT_LINE << "\nwarning:netThreadNum > 15." << endl;
    }

    if(_serverConfig.coroutineMemSize/_serverConfig.coroutineStackSize <= 0)
	{
		__out__.error()  << OUT_LINE << "\nerror:coroutine paramter error: coroutinememsize/coroutinestack <= 0!." << endl;
		exit(-1);
	}
    _epollServer = new TC_EpollServer();

    _epollServer->setThreadNum(_serverConfig.netThread);
    _epollServer->setOpenCoroutine((TC_EpollServer::SERVER_OPEN_COROUTINE)_serverConfig.openCoroutine);
	_epollServer->setCoroutineStack(_serverConfig.coroutineMemSize/_serverConfig.coroutineStackSize, _serverConfig.coroutineStackSize);

    _epollServer->setOnAccept(std::bind(&Application::onAccept, this, std::placeholders::_1));

    //初始化服务是否对空链接进行超时检查
//    bool bEnable = (_conf.get("/tars/application/server<emptyconcheck>","0")=="1")?true:false;

//    _epollServer->enAntiEmptyConnAttack(bEnable);
    _epollServer->setEmptyConnTimeout(TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<emptyconntimeout>"), "0")));

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地文件cache
	__out__.info()  << OUT_LINE << "\n" << TC_Common::outfill("[set file cache ]") << "OK" << endl;
	_thisCommunicator->getAppCache()->setCacheInfo(_serverConfig.dataPath + _serverConfig.serverName + ".tarsdat", 0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地Log
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set roll logger] ") << "OK" << endl;
    LocalRollLogger::getInstance()->setLogInfo(_serverConfig.application, _serverConfig.serverName, _serverConfig.logPath, _serverConfig.logSize, _serverConfig.logNum, _thisCommunicator, _serverConfig.log);
    _epollServer->setLocalLogger(LocalRollLogger::getInstance()->logger());

    //初始化是日志为同步
    LocalRollLogger::getInstance()->sync(true);

    //设置日志级别
    string level = _thisCommunicator->getAppCache()->get("logLevel");
    if(level.empty())
    {
        level = _conf.get("/tars/application/server<logLevel>","DEBUG");
    }

	_serverConfig.logLevel = TC_Common::upper(level);

	LocalRollLogger::getInstance()->logger()->setLogLevel(_serverConfig.logLevel);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到LogServer代理
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set time logger] ") << "OK" << endl;
    bool bLogStatReport = (_conf.get("/tars/application/server<logstatreport>", "0") == "1") ? true : false;
    RemoteTimeLogger::getInstance()->setLogInfo(_thisCommunicator, _serverConfig.log, _serverConfig.application, _serverConfig.serverName, _serverConfig.logPath, setDivision(), bLogStatReport);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到配置中心代理
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set remote config] ") << "OK" << endl;
    RemoteConfig::getInstance()->setConfigInfo(_thisCommunicator, _serverConfig.config, _serverConfig.application, _serverConfig.serverName, _serverConfig.basePath,setDivision());

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到信息中心代理
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set remote notify] ") << "OK" << endl;
    RemoteNotify::getInstance()->setNotifyInfo(_thisCommunicator, _serverConfig.notify, _serverConfig.application, _serverConfig.serverName, setDivision(), _serverConfig.localIp);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到Node的代理
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set node proxy]") << "OK" << endl;
    KeepAliveNodeFHelper::getInstance()->setNodeInfo(_thisCommunicator, _serverConfig.node, _serverConfig.application, _serverConfig.serverName);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化管理对象
    __out__.info()  << OUT_LINE << "\n" << TC_Common::outfill("[set admin adapter]") << "OK" << endl;

    if (!_serverConfig.local.empty())
    {
        _servantHelper->addServant<AdminServant>("AdminObj", this);

        string adminAdapter = "AdminAdapter";

	    _servantHelper->setAdapterServant(adminAdapter, "AdminObj");

        TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<ServantHandle>(adminAdapter, _serverConfig.local, 1, this);

	    setAdapter(lsPtr, adminAdapter);

        lsPtr->setMaxConns(TC_EpollServer::BindAdapter::DEFAULT_MAX_CONN);

        lsPtr->setQueueCapacity(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_CAP);

        lsPtr->setQueueTimeout(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_TIMEOUT);

        lsPtr->setProtocolName("tars");

        lsPtr->setProtocol(AppProtocol::parse);

        _epollServer->bind(lsPtr);

    }

    //队列取平均值
    if(!_thisCommunicator->getProperty("property").empty())
    {
        string sRspQueue("");
        sRspQueue += _serverConfig.application;
        sRspQueue += ".";
        sRspQueue += _serverConfig.serverName;
        sRspQueue += ".sendrspqueue";

        g_pReportRspQueue = _thisCommunicator->getStatReport()->createPropertyReport(sRspQueue, PropertyReport::avg());
    }

    TarsTimeLogger::getInstance()->enableLocal(TRACE_LOG_FILENAME, false);
}

void Application::setAdapter(TC_EpollServer::BindAdapterPtr& adapter, const string &name)
{
	// 设置该obj的鉴权账号密码，只要一组就够了
	{
		std::string accKey      = _conf.get("/tars/application/server/" + name + "<accesskey>");
		std::string secretKey   = _conf.get("/tars/application/server/" + name + "<secretkey>");

		//注意这里必须用weak, 否则adapter最终释放不了!
		weak_ptr<TC_EpollServer::BindAdapter> a = adapter;
		adapter->setAkSkCallback(accKey, secretKey, std::bind(&tars::serverVerifyAuthCallback, std::placeholders::_1, std::placeholders::_2, a, _servantHelper->getAdapterServant(name)));
	}

#if TARS_SSL
	string cert = _conf.get("/tars/application/server/" + name + "<cert>");

	if (!cert.empty())
	{
		string ca = _conf.get("/tars/application/server/" + name + "<ca>");
		string key = _conf.get("/tars/application/server/" + name + "<key>");
		bool verifyClient =
			_conf.get("/tars/application/server/" + name + "<verifyclient>", "0") == "0" ? false : true;
		string ciphers = _conf.get("/tars/application/server/" + name + "<ciphers>");

		shared_ptr<TC_OpenSSL::CTX> ctx = TC_OpenSSL::newCtx(ca, cert, key, verifyClient, ciphers);

		if (!ctx) {
			TLOGERROR("load server ssl error, cert:" << cert << endl);
			exit(-1);
		}

		adapter->setSSLCtx(ctx);
	}
	else
	{
		adapter->setSSLCtx(_ctx);
	}
#endif

}

void Application::bindAdapter(vector<TC_EpollServer::BindAdapterPtr>& adapters)
{
    string sPrefix = _serverConfig.application + "." + _serverConfig.serverName + ".";

    vector<string> adapterName;

    map<string, ServantHandle*> servantHandles;

    if (_conf.getDomainVector("/tars/application/server", adapterName))
    {
        for (size_t i = 0; i < adapterName.size(); i++)
        {
            string servant = _conf.get("/tars/application/server/" + adapterName[i] + "<servant>");

            checkServantNameValid(servant, sPrefix);

            _servantHelper->setAdapterServant(adapterName[i], servant);

            string sLastPath = "/tars/application/server/" + adapterName[i];
            TC_Endpoint ep;
            ep.parse(_conf[sLastPath + "<endpoint>"]);
            if (ep.getHost() == "localip")
            {
                ep.setHost(_serverConfig.localIp);
            }

            TC_EpollServer::BindAdapterPtr bindAdapter = _epollServer->createBindAdapter<ServantHandle>(adapterName[i], _conf[sLastPath + "<endpoint>"], TC_Common::strto<int>(_conf.get(sLastPath + "<threads>", "1")), this);

            //init auth & ssl
	        setAdapter(bindAdapter, adapterName[i]);

            bindAdapter->setMaxConns(TC_Common::strto<int>(_conf.get(sLastPath + "<maxconns>", "128")));

            bindAdapter->setOrder(parseOrder(_conf.get(sLastPath + "<order>", "allow,deny")));

            bindAdapter->setAllow(TC_Common::sepstr<string>(_conf[sLastPath + "<allow>"], ";,", false));

            bindAdapter->setDeny(TC_Common::sepstr<string>(_conf.get(sLastPath + "<deny>", ""), ";,", false));

            bindAdapter->setQueueCapacity(TC_Common::strto<int>(_conf.get(sLastPath + "<queuecap>", "1024")));

            bindAdapter->setQueueTimeout(TC_Common::strto<int>(_conf.get(sLastPath + "<queuetimeout>", "10000")));

            bindAdapter->setProtocolName(_conf.get(sLastPath + "<protocol>", "tars"));

	        bindAdapter->setBackPacketBuffLimit(_serverConfig.backPacketLimit);

	        bindAdapter->setBackPacketBuffMin(_serverConfig.backPacketMin);

	        if (bindAdapter->isTarsProtocol())
            {
                bindAdapter->setProtocol(AppProtocol::parse);
            }

            //校验ssl正常初始化
#if TARS_SSL
            if (bindAdapter->getEndpoint().isSSL() && (!(bindAdapter->getSSLCtx())))
            {
                __out__.error()  << "load server ssl error, no cert config!" << bindAdapter->getEndpoint().toString() << endl;
                exit(-1);
            }
#endif

            if(_serverConfig.manualListen) {
                //手工监听
                bindAdapter->enableManualListen();
            }

            _epollServer->bind(bindAdapter);

            adapters.push_back(bindAdapter);

            //队列取平均值
            if(!_thisCommunicator->getProperty("property").empty())
            {
                PropertyReportPtr p;
                p = _thisCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".queue", PropertyReport::avg());
                bindAdapter->_pReportQueue = p.get();

                p = _thisCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".connectRate", PropertyReport::avg());
                bindAdapter->_pReportConRate = p.get();

                p = _thisCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".timeoutNum", PropertyReport::sum());
                bindAdapter->_pReportTimeoutNum = p.get();
            }
        }
    }
}

void Application::checkServantNameValid(const string& servant, const string& sPrefix)
{
    if ((servant.length() <= sPrefix.length()) || (servant.substr(0, sPrefix.length()) != sPrefix))
    {
        ostringstream os;

        os << "Servant '" << servant << "' error: must be start with '" << sPrefix << "'";

		NOTIFY_AND_WAIT("exit: " + string(os.str()));
		
	    __out__.error()  << os.str() << endl;

        exit(-1);
    }
}

void Application::outAdapter(ostream &os, const string &v, TC_EpollServer::BindAdapterPtr lsPtr)
{
    os << TC_Common::outfill("name")             << lsPtr->getName() << endl;
    os << TC_Common::outfill("servant")          << v << endl;
    os << TC_Common::outfill("endpoint")         << lsPtr->getEndpoint().toString() << endl;
    os << TC_Common::outfill("maxconns")         << lsPtr->getMaxConns() << endl;
    os << TC_Common::outfill("queuecap")         << lsPtr->getQueueCapacity() << endl;
    os << TC_Common::outfill("queuetimeout")     << lsPtr->getQueueTimeout() << "ms" << endl;
    os << TC_Common::outfill("order")            << (lsPtr->getOrder() == TC_EpollServer::BindAdapter::ALLOW_DENY ? "allow,deny" : "deny,allow") << endl;
    os << TC_Common::outfill("allow")            << TC_Common::tostr(lsPtr->getAllow()) << endl;
    os << TC_Common::outfill("deny")             << TC_Common::tostr(lsPtr->getDeny()) << endl;
    os << TC_Common::outfill("connections")      << lsPtr->getNowConnection() << endl;
    os << TC_Common::outfill("protocol")         << lsPtr->getProtocolName() << endl;
    os << TC_Common::outfill("handlethread")     << lsPtr->getHandleNum() << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
}
