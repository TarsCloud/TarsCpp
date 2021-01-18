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


namespace tars
{

//#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
//static void sighandler( int sig_no )
//{
//	TLOGERROR("[TARS][sighandler] sig_no :" << sig_no << endl);
//
//    Application::terminate();
//}
//#else
//static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
//{
//	TLOGERROR("[TARS][sighandler] dwCtrlType :" << dwCtrlType << endl);
//	Application::terminate();
//	ExitProcess(0);
//	return TRUE;
//}
//#endif


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
int         ServerConfig::ReportFlow = 1;       //是否服务端上报所有接口stat流量 0不上报 1上报 (用于非taf协议服务流量统计)
int         ServerConfig::IsCheckSet = 1;       //是否对按照set规则调用进行合法性检查 0,不检查，1检查
bool        ServerConfig::OpenCoroutine = false;    //是否启用协程处理方式
size_t      ServerConfig::CoroutineMemSize; //协程占用内存空间的最大大小
uint32_t    ServerConfig::CoroutineStackSize;   //每个协程的栈大小(默认128k)
bool        ServerConfig::ManualListen = false;     //手工启动监听端口
bool        ServerConfig::MergeNetImp = false;     //合并网络和处理线程
int         ServerConfig::NetThread = 1;               //servernet thread
bool        ServerConfig::CloseCout = true;
int         ServerConfig::BackPacketLimit = 0;
int         ServerConfig::BackPacketMin = 1024;

#if TARS_SSL
std::string ServerConfig::CA;
std::string ServerConfig::Cert;
std::string ServerConfig::Key;
bool ServerConfig::VerifyClient = false;
std::string ServerConfig::Ciphers;
#endif

map<string, string> ServerConfig::Context;

///////////////////////////////////////////////////////////////////////////////////////////
//TC_Config                       Application::_conf;
//TC_EpollServerPtr               Application::_epollServer  = NULL;
CommunicatorPtr                 Application::_communicator = NULL;

PropertyReportPtr g_pReportRspQueue;

/**上报服务端发送队列大小的间隔时间**/
#define REPORT_SEND_QUEUE_INTERVAL 10

///////////////////////////////////////////////////////////////////////////////////////////
Application::Application()
{
#if TARGET_PLATFORM_WINDOWS    
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif
	_servantHelper = std::make_shared<ServantHelperManager>();

	_notifyObserver = std::make_shared<NotifyObserver>();

	setNotifyObserver(_notifyObserver);

}

Application::~Application()
{
    terminate();
#if TARGET_PLATFORM_WINDOWS    
    WSACleanup();
#endif
}

string Application::getTarsVersion()
{
    return TARS_VERSION;
}
//
//TC_Config& Application::getConfig()
//{
//    return _conf;
//}
//
//TC_EpollServerPtr& Application::getEpollServer()
//{
//    return _epollServer;
//}

CommunicatorPtr& Application::getCommunicator()
{
    return _communicator;
}

void reportRspQueue(TC_EpollServer *epollServer)
{
    // TLOGDEBUG("Application::reportRspQueue" << endl);

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
    _epollServer->setCallbackFunctor(reportRspQueue);

    _epollServer->setHeartBeatFunctor(heartBeatFunc);

    _epollServer->waitForShutdown();

    destroyApp();

    RemoteNotify::getInstance()->report("stop");

	std::this_thread::sleep_for(std::chrono::milliseconds(100)); //稍微休息一下, 让当前处理包能够回复

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

    os << (ServerConfig::Application + "." + ServerConfig::ServerName);

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
        ServerConfig::LogLevel = TC_Common::upper(level);

        result = "set log level [" + level + "] ok";

        AppCache::getInstance()->set("logLevel", level);
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


    if(nNum == 2)
    {
        bEnable = (vParams[1] == "true")?true:false;
        sFile = "";
        result = "set " + vParams[0] + " " + vParams[1] + " ok";
    }
    else if(nNum == 3)
    {
        bEnable = (vParams[2] == "true")?true:false;
        sFile = vParams[1];
        result = "set " + vParams[0] + " " + vParams[1] + " "+vParams[2] + " ok";
    }


    if(vParams[0] == "local")
    {
        RemoteTimeLogger::getInstance()->enableLocal(sFile,bEnable);
        return true;
    }

    if(vParams[0] == "remote")
    {
        RemoteTimeLogger::getInstance()->enableRemote(sFile,bEnable);
        return true;
    }

    result = "usage: tars.enabledaylog {remote|local}|[logname]|{true|false}";
    return false;

}

bool Application::cmdLoadConfig(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdLoadConfig:" << command << " " << params << endl);

    string filename = TC_Common::trim(params);

    if (RemoteConfig::getInstance()->addConfig(filename, result,false))
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

        os << OUT_LINE << "\n" << TC_Common::outfill("[adater:" + it->second->getName() + "] [connections:" + TC_Common::tostr(v.size()) + "]") << endl;

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
    result = "$" + ServerConfig::Application + "." + ServerConfig::ServerName + "-" + string(buildTime) + "$";
    return true;
}

bool Application::cmdLoadProperty(const string& command, const string& params, string& result)
{
    try
    {
        TLOGTARS("Application::cmdLoadProperty:" << command << " " << params << endl);

        //重新解析配置文件
        _conf.parseFile(ServerConfig::ConfigFile);

        string sResult = "";

        //加载通讯器属性
        _communicator->setProperty(_conf);

        _communicator->reloadProperty(sResult);

        //加载远程对象
        ServerConfig::Log = _conf.get("/tars/application/server<log>");

        RemoteTimeLogger::getInstance()->setLogInfo(_communicator, ServerConfig::Log, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath,setDivision());

        ServerConfig::Config = _conf.get("/tars/application/server<config>");

        RemoteConfig::getInstance()->setConfigInfo(_communicator, ServerConfig::Config, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::BasePath,setDivision(), 5);

        ServerConfig::Notify = _conf.get("/tars/application/server<notify>");

        RemoteNotify::getInstance()->setNotifyInfo(_communicator, ServerConfig::Notify, ServerConfig::Application, ServerConfig::ServerName, setDivision(), ServerConfig::LocalIp);

        result = "loaded config items:\r\n" + sResult +
                 "log=" + ServerConfig::Log + "\r\n" +
                 "config=" + ServerConfig::Config + "\r\n" +
                 "notify=" + ServerConfig::Notify + "\r\n";
    }
    catch (TC_Config_Exception & ex)
    {
        result = "load config " + ServerConfig::ConfigFile + " error:" + ex.what();
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

    result =result +  _notifyObserver->viewRegisterCommand();

    return true;
}

bool Application::cmdSetDyeing(const string& command, const string& params, string& result)
{
    vector<string> vDyeingParams = TC_Common::sepstr<string>(params, " ");

    if(vDyeingParams.size() == 2 || vDyeingParams.size() == 3)
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

    if(s == "yes")
    {
        AppCache::getInstance()->set("closeCout","1");
    }
    else
    {
        AppCache::getInstance()->set("closeCout","0");
    }

    result = "set closeCout  [" + s + "] ok";

    return true;
}

bool Application::cmdReloadLocator(const string& command, const string& params, string& result)
{
    TLOGTARS("Application::cmdReloadLocator:" << command << " " << params << endl);

    string sPara = TC_Common::lower(TC_Common::trim(params));

    bool bSucc(true);
    if (sPara == "reload")
    {
        TC_Config reloadConf;

        reloadConf.parseFile(ServerConfig::ConfigFile);
        string sLocator = reloadConf.get("/tars/application/client/<locator>", "");

        TLOGDEBUG(__FUNCTION__ << "|" << __LINE__ << "|conf file:" << ServerConfig::ConfigFile << "\n"
            << "|sLocator:" << sLocator << endl);

        if (sLocator.empty())
        {
            bSucc = false;
            result = "locator info is null.";
        }
        else
        {
            _communicator->setProperty("locator", sLocator);
            _communicator->reloadLocator();
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

	os << _communicator->getResourcesInfo() << endl;

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
	//直接输出编译的TARS版本
	if(option.hasParam("version"))
	{
		cout << "TARS:" << TARS_VERSION << endl;
		exit(0);
	}

	//加载配置文件
	ServerConfig::ConfigFile = option.getValue("config");

	if(ServerConfig::ConfigFile == "")
	{
		cerr << "start server with config, for example: exe --config=config.conf" << endl;

		exit(-1);
	}

	string config = TC_File::load2str(ServerConfig::ConfigFile);

	main(config);
}

void Application::main(const string &config)
{
    try
    {
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
        TC_Common::ignorePipe();
#endif

        //解析配置文件
        parseConfig(config);

        //初始化Proxy部分
        initializeClient();

        //初始化Server部分
        initializeServer();

        vector <TC_EpollServer::BindAdapterPtr> adapters;

        //绑定对象和端口
        bindAdapter(adapters);

        //输出所有adapter
        outAllAdapter(cout);

        cout << "\n" << TC_Common::outfill("[initialize server] ", '.')  << " [Done]" << endl;

        cout << OUT_LINE_LONG << endl;

        {
            bool initing = true;
            std::mutex mtx;
            std::condition_variable cond;

            std::thread keepActiving([&]
            {
                do {
                    //发送心跳给node, 表示正在启动
                    TARS_KEEPACTIVING;

                    //等待initialize初始化完毕
                    std::unique_lock<std::mutex> lock(mtx);
                    cond.wait_for(lock, std::chrono::seconds(2));

                }while(initing);
            });
            //捕捉初始化可能的异常
            try
            {
                TC_Common::msleep(100);
                //业务应用的初始化
                initialize();
                initing = false;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cond.notify_all();
                }
                keepActiving.join();
            }
            catch (exception & ex)
            {
                keepActiving.detach();
                RemoteNotify::getInstance()->report("exit: " + string(ex.what()));
	            std::this_thread::sleep_for(std::chrono::milliseconds(100)); //稍微休息一下, 让当前处理包能够回复

	            cout << "[init exception]:" << ex.what() << endl;
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
//#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
//		std::thread th1(signal, SIGINT, sighandler);
//		th1.detach();
//		//k8s pod完美退出
//		std::thread th2(signal, SIGTERM, sighandler);
//		th2.detach();
//#else
//		std::thread th([] {SetConsoleCtrlHandler(HandlerRoutine, TRUE); });
//		th.detach();
//#endif
//#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	    TC_Port::registerCtrlC([=]{
		    this->terminate();
#if TARGET_PLATFORM_WINDOWS
		    ExitProcess(0);
#endif
	    });
	    TC_Port::registerTerm([=]{
		    this->terminate();
#if TARGET_PLATFORM_WINDOWS
		    ExitProcess(0);
#endif
	    });

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
        if(_conf.get("/tars/application/server<closecout>",AppCache::getInstance()->get("closeCout")) != "0")
        {
            // 重定向stdin、stdout、stderr
            int fd = open("/dev/null", O_RDWR );
            if(fd != -1)
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
        cout << "[main exception]:" << ex.what() << endl;

        RemoteNotify::getInstance()->report("exit: " + string(ex.what()));

	    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //稍微休息一下, 让当前处理包能够回复

	    exit(-1);
    }

    //初始化完毕后, 日志再修改为异步
    LocalRollLogger::getInstance()->sync(false);
}

void Application::parseConfig(const string &config)
{
    _conf.parseString(config);

    onParseConfig(_conf);
}

TC_EpollServer::BindAdapter::EOrder Application::parseOrder(const string &s)
{
    vector<string> vtOrder = TC_Common::sepstr<string>(s,";, \t", false);

    if(vtOrder.size() != 2)
    {
        cerr << "invalid order '" << TC_Common::tostr(vtOrder) << "'."<< endl;

        exit(0);
    }
    if((TC_Common::lower(vtOrder[0]) == "allow")&&(TC_Common::lower(vtOrder[1]) == "deny"))
    {
        return TC_EpollServer::BindAdapter::ALLOW_DENY;
    }
    if((TC_Common::lower(vtOrder[0]) == "deny")&&(TC_Common::lower(vtOrder[1]) == "allow"))
    {
         return TC_EpollServer::BindAdapter::DENY_ALLOW;
    }

     cerr << "invalid order '" << TC_Common::tostr(vtOrder) << "'."<< endl;

     exit(0);
}

void Application::initializeClient()
{
    cout << "\n" << OUT_LINE_LONG << endl;

    //初始化通信器
    _communicator = CommunicatorFactory::getInstance()->getCommunicator(_conf);

    cout << TC_Common::outfill("[proxy config]:") << endl;

    //输出
    outClient(cout);
}

void Application::outClient(ostream &os)
{
	cout << OUT_LINE << "\n" << TC_Common::outfill("[load client]:") << endl;

    os << TC_Common::outfill("locator")                     << _communicator->getProperty("locator") << endl;
    os << TC_Common::outfill("sync-invoke-timeout")         << _communicator->getProperty("sync-invoke-timeout") << endl;
    os << TC_Common::outfill("async-invoke-timeout")        << _communicator->getProperty("async-invoke-timeout") << endl;
    os << TC_Common::outfill("refresh-endpoint-interval")   << _communicator->getProperty("refresh-endpoint-interval") << endl;
    os << TC_Common::outfill("stat")                        << _communicator->getProperty("stat") << endl;
    os << TC_Common::outfill("property")                    << _communicator->getProperty("property") << endl;
    os << TC_Common::outfill("report-interval")             << _communicator->getProperty("report-interval") << endl;
//    os << TC_Common::outfill("sample-rate")                 << _communicator->getProperty("sample-rate") << endl;
//    os << TC_Common::outfill("max-sample-count")            << _communicator->getProperty("max-sample-count") << endl;
    os << TC_Common::outfill("netthread")                  << _communicator->getProperty("netthread") << endl;
    os << TC_Common::outfill("asyncthread")                 << _communicator->getProperty("asyncthread") << endl;
    os << TC_Common::outfill("modulename")                  << _communicator->getProperty("modulename") << endl;
    os << TC_Common::outfill("enableset")                     << _communicator->getProperty("enableset") << endl;
    os << TC_Common::outfill("setdivision")                 << _communicator->getProperty("setdivision") << endl;
}

string Application::toDefault(const string &s, const string &sDefault)
{
    if(s.empty())
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

void Application::addServantOnClose(const string& servant, const TC_EpollServer::close_functor& cf)
{
    string adapterName = _servantHelper->getServantAdapter(servant);

    if (adapterName.empty())
    {
        throw runtime_error("setServantOnClose fail, no found adapter for servant:" + servant);
    }

    getEpollServer()->getBindAdapter(adapterName)->setOnClose(cf);
}

void Application::outServer(ostream &os)
{
	os << TC_Common::outfill("Application(app)")            << ServerConfig::Application << endl;
	os << TC_Common::outfill("ServerName(server)")          << ServerConfig::ServerName << endl;
	os << TC_Common::outfill("BasePath(basepath)")          << ServerConfig::BasePath << endl;
	os << TC_Common::outfill("DataPath(datapath)")          << ServerConfig::DataPath << endl;
	os << TC_Common::outfill("LocalIp(localip)")            << ServerConfig::LocalIp << endl;
	os << TC_Common::outfill("Local(local)")                << ServerConfig::Local << endl;
	os << TC_Common::outfill("LogPath(logpath)")            << ServerConfig::LogPath << endl;
	os << TC_Common::outfill("LogSize(logsize)")            << ServerConfig::LogSize << endl;
	os << TC_Common::outfill("LogNum(lognum)")              << ServerConfig::LogNum << endl;
	os << TC_Common::outfill("LogLevel(loglevel)")          << ServerConfig::LogLevel << endl;
	os << TC_Common::outfill("Log(log)")                    << ServerConfig::Log << endl;
	os << TC_Common::outfill("Node(node)")                  << ServerConfig::Node << endl;
	os << TC_Common::outfill("Config(config)")              << ServerConfig::Config << endl;
	os << TC_Common::outfill("Notify(notify)")              << ServerConfig::Notify << endl;
	os << TC_Common::outfill("OpenCoroutine(opencoroutine)")      << ServerConfig::OpenCoroutine << endl;
	os << TC_Common::outfill("CoroutineMemSize(coroutinememsize)")   << ServerConfig::CoroutineMemSize << endl;
	os << TC_Common::outfill("CoroutineStackSize(coroutinestack)") << ServerConfig::CoroutineStackSize << endl;
	os << TC_Common::outfill("CloseCout(closecout)")          << ServerConfig::CloseCout << endl;
	os << TC_Common::outfill("NetThread(netthread)")          << ServerConfig::NetThread << endl;
	os << TC_Common::outfill("ManualListen(manuallisten)")       << ServerConfig::ManualListen << endl;
	os << TC_Common::outfill("MergeNetImp(mergenetimp)")       << ServerConfig::MergeNetImp << endl;
	os << TC_Common::outfill("ReportFlow(reportflow)")                  << ServerConfig::ReportFlow<< endl;
	os << TC_Common::outfill("BackPacketLimit(backpacketlimit)")  << ServerConfig::BackPacketLimit<< endl;
	os << TC_Common::outfill("BackPacketMin(backpacketmin)")  << ServerConfig::BackPacketMin<< endl;

#if TARS_SSL
	cout << TC_Common::outfill("Ca(ca)")                    << ServerConfig::CA << endl;
	cout << TC_Common::outfill("Cert(cert)")              << ServerConfig::Cert << endl;
	cout << TC_Common::outfill("Key(key)")                  << ServerConfig::Key << endl;
	cout << TC_Common::outfill("VerifyClient(verifyclient)")      << ServerConfig::VerifyClient << endl;
	cout << TC_Common::outfill("Ciphers(ciphers)")               << ServerConfig::Ciphers << endl;
#endif

}


void Application::initializeServer()
{
    cout << OUT_LINE << "\n" << TC_Common::outfill("[server config]:") << endl;

    ServerConfig::Application  = toDefault(_conf.get("/tars/application/server<app>"), "UNKNOWN");

    //缺省采用进程名称
    string exe = "";

    try
    {
        exe = TC_File::extractFileName(TC_File::getExePath());
    }
    catch(TC_File_Exception & ex)
    {
        //取失败则使用ip代替进程名
        exe = _conf.get("/tars/application/server<localip>");
    }

    ServerConfig::ServerName        = toDefault(_conf.get("/tars/application/server<server>"), exe);


#if TARGET_PLATFORM_WINDOWS    
    ServerConfig::BasePath          = TC_File::simplifyDirectory(_conf.get("/tars/application/server<basepath.win>")) + FILE_SEP;
    if (ServerConfig::BasePath == FILE_SEP)
    {
        ServerConfig::BasePath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<basepath>"), ".")) + FILE_SEP;
    }

    ServerConfig::DataPath          = TC_File::simplifyDirectory(_conf.get("/tars/application/server<datapath.win>")) + FILE_SEP;
    if(ServerConfig::DataPath == FILE_SEP)
    {
        ServerConfig::DataPath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<datapath>"), ".")) + FILE_SEP;
    }

    ServerConfig::LogPath           = TC_File::simplifyDirectory(_conf.get("/tars/application/server<logpath.win>")) + FILE_SEP;
    if(ServerConfig::LogPath == FILE_SEP)
    {
        ServerConfig::LogPath       = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<logpath>"),  ".")) + FILE_SEP;
    }
#else
    ServerConfig::BasePath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<basepath>"), ".")) + FILE_SEP;
    ServerConfig::DataPath          = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<datapath>"), ".")) + FILE_SEP;
    ServerConfig::LogPath           = TC_File::simplifyDirectory(toDefault(_conf.get("/tars/application/server<logpath>"),  ".")) + FILE_SEP;

#endif
    ServerConfig::TarsPath           = TC_File::simplifyDirectory(ServerConfig::LogPath + FILE_SEP + ".." + FILE_SEP) + FILE_SEP;

    ServerConfig::LogSize           = TC_Common::toSize(toDefault(_conf.get("/tars/application/server<logsize>"), "52428800"), 52428800);
    ServerConfig::LogNum            = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<lognum>"), "10"));
    ServerConfig::LocalIp           = _conf.get("/tars/application/server<localip>");
    ServerConfig::Local             = _conf.get("/tars/application/server<local>");
    ServerConfig::Node              = _conf.get("/tars/application/server<node>");
    ServerConfig::Log               = _conf.get("/tars/application/server<log>");
    ServerConfig::Config            = _conf.get("/tars/application/server<config>");
    ServerConfig::Notify            = _conf.get("/tars/application/server<notify>");
    ServerConfig::ReportFlow        = _conf.get("/tars/application/server<reportflow>")=="0"?0:1;
    ServerConfig::IsCheckSet        = _conf.get("/tars/application/server<checkset>","1")=="0"?0:1;
    ServerConfig::OpenCoroutine     = TC_Common::strto<bool>(toDefault(_conf.get("/tars/application/server<opencoroutine>"), "0"));
    ServerConfig::CoroutineMemSize  =  TC_Common::toSize(toDefault(_conf.get("/tars/application/server<coroutinememsize>"), "1G"), 1024*1024*1024);
    ServerConfig::CoroutineStackSize= (uint32_t)TC_Common::toSize(toDefault(_conf.get("/tars/application/server<coroutinestack>"), "128K"), 1024*128);
    ServerConfig::ManualListen      = _conf.get("/tars/application/server<manuallisten>", "0") == "0" ? false : true;
	ServerConfig::MergeNetImp       = _conf.get("/tars/application/server<mergenetimp>", "0") == "0" ? false : true;
	ServerConfig::NetThread         = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<netthread>"), "1"));
	ServerConfig::CloseCout        = _conf.get("/tars/application/server<closecout>","1")=="0"?0:1;
	ServerConfig::BackPacketLimit  = TC_Common::strto<int>(_conf.get("/tars/application/server<backpacketlimit>", "100*1024*1024"));
	ServerConfig::BackPacketMin    = TC_Common::strto<int>(_conf.get("/tars/application/server<backpacketmin>", "1024"));

	ServerConfig::Context["node_name"] = ServerConfig::LocalIp;
#if TARS_SSL
	ServerConfig::CA                = _conf.get("/tars/application/server<ca>");
	ServerConfig::Cert              = _conf.get("/tars/application/server<cert>");
	ServerConfig::Key               = _conf.get("/tars/application/server<key>");
	ServerConfig::VerifyClient      = _conf.get("/tars/application/server<verifyclient>","0")=="0"?false:true;
	ServerConfig::Ciphers           = _conf.get("/tars/application/server<ciphers>");

	if(!ServerConfig::Cert.empty()) {
		_ctx = TC_OpenSSL::newCtx(ServerConfig::CA, ServerConfig::Cert, ServerConfig::Key, ServerConfig::VerifyClient, ServerConfig::Ciphers);

		if (!_ctx) {
			TLOGERROR("[TARS]load server ssl error, ca:" << ServerConfig::CA << endl);
			exit(-1);
		}
	}
#endif


    if (ServerConfig::LocalIp.empty())
    {
        // ServerConfig::LocalIp = "127.0.0.1";
        vector<string> v = TC_Socket::getLocalHosts();

        ServerConfig::LocalIp = "127.0.0.1";
        //获取第一个非127.0.0.1的IP
        for(size_t i = 0; i < v.size(); i++)
        {
            if(v[i] != "127.0.0.1")
            {
                ServerConfig::LocalIp = v[i];
                break;
            }
        }
    }

    onServerConfig();

    //输出信息
    outServer(cout);

    if (ServerConfig::NetThread < 1)
    {
        ServerConfig::NetThread = 1;
        cout << OUT_LINE << "\nwarning:netThreadNum < 1." << endl;
    }

    //网络线程的配置数目不能15个
    if (ServerConfig::NetThread > 15)
    {
        ServerConfig::NetThread = 15;
        cout << OUT_LINE << "\nwarning:netThreadNum > 15." << endl;
    }

    _epollServer = new TC_EpollServer(ServerConfig::NetThread);

    _epollServer->setOnAccept(std::bind(&Application::onAccept, this, std::placeholders::_1));

    //初始化服务是否对空链接进行超时检查
    bool bEnable = (_conf.get("/tars/application/server<emptyconcheck>","0")=="1")?true:false;

    _epollServer->enAntiEmptyConnAttack(bEnable);
    _epollServer->setEmptyConnTimeout(TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<emptyconntimeout>"), "3")));

    //合并线程
	_epollServer->setMergeHandleNetThread(ServerConfig::MergeNetImp);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地文件cache
    cout << OUT_LINE << "\n" << TC_Common::outfill("[set file cache ]") << "OK" << endl;
    AppCache::getInstance()->setCacheInfo(ServerConfig::DataPath+ServerConfig::ServerName+".tarsdat",0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地Log
    cout << OUT_LINE << "\n" << TC_Common::outfill("[set roll logger] ") << "OK" << endl;
    LocalRollLogger::getInstance()->setLogInfo(ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath, ServerConfig::LogSize, ServerConfig::LogNum, _communicator, ServerConfig::Log);
    _epollServer->setLocalLogger(LocalRollLogger::getInstance()->logger());

    //初始化是日志为同步
    LocalRollLogger::getInstance()->sync(true);

    //设置日志级别
    string level = AppCache::getInstance()->get("logLevel");
    if(level.empty())
    {
        level = _conf.get("/tars/application/server<logLevel>","DEBUG");
    }

	ServerConfig::LogLevel = TC_Common::upper(level);

	LocalRollLogger::getInstance()->logger()->setLogLevel(ServerConfig::LogLevel);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到LogServer代理
    cout << OUT_LINE << "\n" << TC_Common::outfill("[set time logger] ") << "OK" << endl;
    bool bLogStatReport = (_conf.get("/tars/application/server<logstatreport>", "0") == "1") ? true : false;
    RemoteTimeLogger::getInstance()->setLogInfo(_communicator, ServerConfig::Log, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath, setDivision(), bLogStatReport);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到配置中心代理
    cout << OUT_LINE << "\n" << TC_Common::outfill("[set remote config] ") << "OK" << endl;
    RemoteConfig::getInstance()->setConfigInfo(_communicator, ServerConfig::Config, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::BasePath,setDivision());

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到信息中心代理
    cout << OUT_LINE << "\n" << TC_Common::outfill("[set remote notify] ") << "OK" << endl;
    RemoteNotify::getInstance()->setNotifyInfo(_communicator, ServerConfig::Notify, ServerConfig::Application, ServerConfig::ServerName, setDivision(), ServerConfig::LocalIp);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到Node的代理
    cout << OUT_LINE << "\n" << TC_Common::outfill("[set node proxy]") << "OK" << endl;
    KeepAliveNodeFHelper::getInstance()->setNodeInfo(_communicator, ServerConfig::Node, ServerConfig::Application, ServerConfig::ServerName);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化管理对象
    cout << OUT_LINE << "\n" << TC_Common::outfill("[set admin adapter]") << "OK" << endl;

    if(!ServerConfig::Local.empty())
    {
        _servantHelper->addServant<AdminServant>("AdminObj", this);

        _servantHelper->setAdapterServant("AdminAdapter", "AdminObj");

        TC_EpollServer::BindAdapterPtr lsPtr = new TC_EpollServer::BindAdapter(_epollServer.get());

	    setAdapter(lsPtr, "AdminAdapter");

        lsPtr->setEndpoint(ServerConfig::Local);

        lsPtr->setMaxConns(TC_EpollServer::BindAdapter::DEFAULT_MAX_CONN);

        lsPtr->setQueueCapacity(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_CAP);

        lsPtr->setQueueTimeout(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_TIMEOUT);

        lsPtr->setProtocolName("tars");

        lsPtr->setProtocol(AppProtocol::parse);

        lsPtr->setHandle<ServantHandle>(1, this);

        _epollServer->bind(lsPtr);
    }

    //队列取平均值
    if(!_communicator->getProperty("property").empty())
    {
        string sRspQueue("");
        sRspQueue += ServerConfig::Application;
        sRspQueue += ".";
        sRspQueue += ServerConfig::ServerName;
        sRspQueue += ".sendrspqueue";

        g_pReportRspQueue = _communicator->getStatReport()->createPropertyReport(sRspQueue, PropertyReport::avg());
    }

}

void Application::setAdapter(TC_EpollServer::BindAdapterPtr& adapter, const string &name)
{
	adapter->setName(name);

	// 设置该obj的鉴权账号密码，只要一组就够了
	{
		std::string accKey      = _conf.get("/tars/application/server/" + name + "<accesskey>");
		std::string secretKey   = _conf.get("/tars/application/server/" + name + "<secretkey>");

		if (!accKey.empty())
			adapter->setAkSk(accKey, secretKey);

//		adapter->setAuthProcessWrapper(&tars::processAuth);
		adapter->setAuthProcessWrapper(std::bind(tars::processAuth, std::placeholders::_1, std::placeholders::_2, _servantHelper->getAdapterServant(name)));
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
    // size_t iBackPacketBuffLimit = TC_Common::strto<size_t>(toDefault(_conf.get("/tars/application/server<BackPacketBuffLimit>", "0"), "0"));

    string sPrefix = ServerConfig::Application + "." + ServerConfig::ServerName + ".";

    vector<string> adapterName;

    map<string, ServantHandle*> servantHandles;

    if (_conf.getDomainVector("/tars/application/server", adapterName))
    {
        for (size_t i = 0; i < adapterName.size(); i++)
        {
            string servant = _conf.get("/tars/application/server/" + adapterName[i] + "<servant>");

            checkServantNameValid(servant, sPrefix);

            _servantHelper->setAdapterServant(adapterName[i], servant);

            TC_EpollServer::BindAdapterPtr bindAdapter = new TC_EpollServer::BindAdapter(_epollServer.get());

            //init auth & ssl
	        setAdapter(bindAdapter, adapterName[i]);

            string sLastPath = "/tars/application/server/" + adapterName[i];
            TC_Endpoint ep;
            ep.parse(_conf[sLastPath + "<endpoint>"]);
            if (ep.getHost() == "localip")
            {
                ep.setHost(ServerConfig::LocalIp);
            }

            bindAdapter->setName(adapterName[i]);

            bindAdapter->setEndpoint(_conf[sLastPath + "<endpoint>"]);

            bindAdapter->setMaxConns(TC_Common::strto<int>(_conf.get(sLastPath + "<maxconns>", "128")));

            bindAdapter->setOrder(parseOrder(_conf.get(sLastPath + "<order>", "allow,deny")));

            bindAdapter->setAllow(TC_Common::sepstr<string>(_conf[sLastPath + "<allow>"], ";,", false));

            bindAdapter->setDeny(TC_Common::sepstr<string>(_conf.get(sLastPath + "<deny>", ""), ";,", false));

            bindAdapter->setQueueCapacity(TC_Common::strto<int>(_conf.get(sLastPath + "<queuecap>", "1024")));

            bindAdapter->setQueueTimeout(TC_Common::strto<int>(_conf.get(sLastPath + "<queuetimeout>", "10000")));

            bindAdapter->setProtocolName(_conf.get(sLastPath + "<protocol>", "tars"));

	        bindAdapter->setBackPacketBuffLimit(ServerConfig::BackPacketLimit);
	        bindAdapter->setBackPacketBuffMin(ServerConfig::BackPacketMin);

	        if (bindAdapter->isTarsProtocol())
            {
                bindAdapter->setProtocol(AppProtocol::parse);
            }

            //校验ssl正常初始化
#if TARS_SSL
            if (bindAdapter->getEndpoint().isSSL() && (!(bindAdapter->getSSLCtx())))
            {
                cout << "load server ssl error, no cert config!" << bindAdapter->getEndpoint().toString() << endl;
                exit(-1);
            }
#endif
            bindAdapter->setHandle<ServantHandle>(TC_Common::strto<int>(_conf.get(sLastPath + "<threads>", "1")), this);

            if(ServerConfig::ManualListen) {
                //手工监听
                bindAdapter->enableManualListen();
            }

            _epollServer->bind(bindAdapter);

            adapters.push_back(bindAdapter);

            //队列取平均值
            if(!_communicator->getProperty("property").empty())
            {
                PropertyReportPtr p;
                p = _communicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".queue", PropertyReport::avg());
                bindAdapter->_pReportQueue = p.get();

                p = _communicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".connectRate", PropertyReport::avg());
                bindAdapter->_pReportConRate = p.get();

                p = _communicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".timeoutNum", PropertyReport::sum());
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

        RemoteNotify::getInstance()->report("exit:" + os.str());

	    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //稍微休息一下, 让当前处理包能够回复
        cout << os.str() << endl;


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
    // os << outfill("queuesize")        << lsPtr->getRecvBufferSize() << endl;
    os << TC_Common::outfill("connections")      << lsPtr->getNowConnection() << endl;
    os << TC_Common::outfill("protocol")         << lsPtr->getProtocolName() << endl;
    os << TC_Common::outfill("handlethread")     << lsPtr->getHandleNum() << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
}
