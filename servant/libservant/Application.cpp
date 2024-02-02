﻿/**
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
#include "servant/StatReport.h"
#include "servant/RemoteLogger.h"
#include "servant/RemoteConfig.h"
#include "servant/RemoteNotify.h"
#include "servant/QueryF.h"

#include <csignal>
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
std::string ServerConfig::NodeName;         //服务如果部署在框架上, 则表示节点名称, 从模板中获取(framework>=3.0.17才支持), 否则为LocalIp
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
int         ServerConfig::OpenCoroutine = 0;    //是否启用协程处理方式
size_t      ServerConfig::CoroutineMemSize; //协程占用内存空间的最大大小
uint32_t    ServerConfig::CoroutineStackSize;   //每个协程的栈大小(默认128k)
bool        ServerConfig::ManualListen = false;     //手工启动监听端口
int         ServerConfig::NetThread = 1;               //servernet thread
bool        ServerConfig::CloseCout = true;
int         ServerConfig::BackPacketLimit = 0;
int         ServerConfig::BackPacketMin = 1024;
int         ServerConfig::BakFlag = 0;
int         ServerConfig::BakType = 0;

std::string ServerConfig::CA;
std::string ServerConfig::Cert;
std::string ServerConfig::Key;
bool        ServerConfig::VerifyClient = false;
std::string ServerConfig::Ciphers;
map<string, string> ServerConfig::Context;

ServerBaseInfo ServerConfig::toServerBaseInfo()
{
    ServerBaseInfo serverBaseInfo;
    serverBaseInfo.TarsPath = TarsPath;
    serverBaseInfo.Application = Application;
    serverBaseInfo.ServerName = ServerName;
    serverBaseInfo.NodeName = NodeName;
    serverBaseInfo.BasePath = BasePath;
    serverBaseInfo.DataPath = DataPath;
    serverBaseInfo.LocalIp = LocalIp;
    serverBaseInfo.LogPath = LogPath;
    serverBaseInfo.LogSize = LogSize;
    serverBaseInfo.LogNum = LogNum;
    serverBaseInfo.LogLevel = LogLevel;
    serverBaseInfo.Local = Local;
    serverBaseInfo.Node = Node;
    serverBaseInfo.Log = Log;
    serverBaseInfo.Config = Config;
    serverBaseInfo.Notify = Notify;
    serverBaseInfo.ConfigFile = ConfigFile;
    serverBaseInfo.CloseCout = CloseCout;
    serverBaseInfo.ReportFlow = ReportFlow;
    serverBaseInfo.IsCheckSet = IsCheckSet;
    serverBaseInfo.OpenCoroutine = OpenCoroutine;
    serverBaseInfo.CoroutineMemSize = CoroutineMemSize;
    serverBaseInfo.CoroutineStackSize = CoroutineStackSize;
    serverBaseInfo.NetThread = NetThread;
    serverBaseInfo.ManualListen = ManualListen;
    serverBaseInfo.BackPacketLimit = BackPacketLimit;
    serverBaseInfo.BackPacketMin = BackPacketMin;
    serverBaseInfo.BakFlag = BakFlag;
    serverBaseInfo.BakType = BakType;

    serverBaseInfo.CA = CA;
    serverBaseInfo.Cert = Cert;
    serverBaseInfo.Key = Key;
    serverBaseInfo.VerifyClient = VerifyClient;
    serverBaseInfo.Ciphers = Ciphers;
    serverBaseInfo.Context = Context;

    return serverBaseInfo;
}
///////////////////////////////////////////////////////////////////////////////////////////

CommunicatorPtr                 Application::_communicator = nullptr;

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
	if(_epollServer)
	{
		_epollServer->terminate();
		_epollServer = nullptr;
	}
#if TARGET_PLATFORM_WINDOWS
    WSACleanup();
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

CommunicatorPtr& Application::getApplicationCommunicator()
{
    return _applicationCommunicator;
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
        for (const auto & adapter : adapters)
        {
            n = n + adapter->getSendBufferSize();
        }

        g_pReportRspQueue->report((int)n);
    }
}

//void heartBeatFunc(const string& adapterName)
//{
//    TARS_KEEPALIVE(adapterName);
//}
//
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

    _epollServer->setHeartBeatFunctor([&](const string &adapter){
        _keepAliveNodeFHelper->keepAlive(adapter);
    });

	_epollServer->setDestroyAppFunctor([&](TC_EpollServer *epollServer){

		this->destroyApp();

		NOTIFY_AND_WAIT("stop");
	});

	_epollServer->waitForShutdown();

	TC_Port::unregisterCtrlC(_ctrlCId);
	TC_Port::unregisterTerm(_termId);

	_epollServer = nullptr;
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
        if(_masterSlaveCheckThread && _masterSlaveCheckThread->joinable())
        {
            {
                TC_ThreadLock::Lock lock(_masterSlaveLock);
                _terminateCheckMasterSlave = true;
                _masterSlaveLock.notify();
            }

            _masterSlaveCheckThread->join();
        }
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
    struct rlimit tlimit{};
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

    if (_remoteConfig->addConfig(filename, result, false))
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
    result = "$" + _serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + "-" + string(buildTime) + "$";
    return true;
}

bool Application::cmdLoadProperty(const string& command, const string& params, string& result)
{
    try
    {
        TLOGTARS("Application::cmdLoadProperty:" << command << " " << params << endl);

        //重新解析配置文件
        _conf.parseFile(ServerConfig::ConfigFile);

        string sResult;

        //加载通讯器属性
        _applicationCommunicator->setProperty(_conf);

        _applicationCommunicator->reloadProperty(sResult);

        //加载远程对象
        _serverBaseInfo.Log = _conf.get("/tars/application/server<log>");
//        ServerConfig::Log = _serverBaseInfo.Log;

        RemoteTimeLogger::getInstance()->setLogInfo(_applicationCommunicator, _serverBaseInfo.Log, _serverBaseInfo.Application, _serverBaseInfo.ServerName, _serverBaseInfo.LogPath,setDivision());

        _serverBaseInfo.Config = _conf.get("/tars/application/server<config>");

        _remoteConfig->setConfigInfo(_applicationCommunicator, _serverBaseInfo.Config, _serverBaseInfo.Application, _serverBaseInfo.ServerName, _serverBaseInfo.BasePath, setDivision(), 5);

        _serverBaseInfo.Notify = _conf.get("/tars/application/server<notify>");

        RemoteNotify::getInstance()->setNotifyInfo(_applicationCommunicator, _serverBaseInfo.Notify, _serverBaseInfo.Application, _serverBaseInfo.ServerName, setDivision());

        result = "loaded config items:\r\n" + sResult +
                 "log=" + _serverBaseInfo.Log + "\r\n" +
                 "config=" + _serverBaseInfo.Config + "\r\n" +
                 "notify=" + _serverBaseInfo.Notify + "\r\n";
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
        AppCache::getInstance()->set("closeCout", "1");
    }
    else
    {
        AppCache::getInstance()->set("closeCout", "0");
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
    	TLOGDEBUG(__FUNCTION__ << "|" << __LINE__ << "|conf file:" << ServerConfig::ConfigFile << endl);

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
            _applicationCommunicator->setProperty("locator", sLocator);
            _applicationCommunicator->reloadLocator();
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

	os << _applicationCommunicator->getResourcesInfo() << endl;

	os << OUT_LINE << endl;

	vector<TC_EpollServer::BindAdapterPtr> adapters = _epollServer->getBindAdapters();
	for(const auto& adapter : adapters)
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

    for (auto & it : m)
    {
        outAdapter(os, _servantHelper->getAdapterServant(it.second->getName()), it.second);

        os << OUT_LINE << endl;
    }
}

bool Application::addConfig(const string &filename)
{
    string result;

    if (_remoteConfig->addConfig(filename, result, false))
    {
        RemoteNotify::getInstance()->report(result);

        return true;
    }
    RemoteNotify::getInstance()->report(result);

    return true;
}

bool Application::addAppConfig(const string &filename)
{
    string result;

    // true-只获取应用级别配置
    if (_remoteConfig->addConfig(filename, result, true))
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
	ServerConfig::ConfigFile = option.getValue("config");

	if (ServerConfig::ConfigFile.empty())
	{
		cerr << "start server with config, for example: exe --config=config.conf" << endl;

		exit(-1);
	}

	string config = TC_File::load2str(ServerConfig::ConfigFile);

	main(config);
}

void Application::main(const string &config)
{
    //增加一把全局静态锁, 避免一个进程内, 多个服务同时初始化时带来bug(一般测试中才会这样!)
    static std::mutex gmtx;
    std::lock_guard<std::mutex> glock(gmtx);

    try
    {
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
        TC_Common::ignorePipe();
#endif
    	__out__.modFlag(0xFFFF, false);

        //解析配置文件
        parseConfig(config);

        __out__.debug() << "config:" << ServerConfig::ConfigFile << endl;
        __out__.debug() << "config:" << config << endl;

        //初始化Proxy部分
        initializeClient();

        //初始化Server部分
        initializeServer();

        _serverBaseInfo = ServerConfig::toServerBaseInfo();

        initializeAdapter();

        {
            bool initing = true;
            std::mutex kmtx;
            std::condition_variable cond;

            std::thread keepActiving([&]
            {
                do
                {
                    //发送心跳给node, 表示正在启动
                    _keepAliveNodeFHelper->keepActiving();

                    //等待initialize初始化完毕
                    std::unique_lock<std::mutex> lock(kmtx);
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
                    std::unique_lock<std::mutex> lock(kmtx);
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

        //绑定对象和端口
        bindAdapters();

        if (!_applicationCommunicator->getProperty("locator").empty() && _serverBaseInfo.BakType > 0)
        {
            int timeout = TC_Common::strto<int>(_conf.get("/tars/application/server<ms-check-timeout>", "5000"));
            if (timeout < 5000)
            {
                timeout = 5000;
            }

            _masterSlaveCheckThread = new std::thread(std::bind(&Application::checkMasterSlave, this, timeout));
        }
        else
        {
            // 非主备模式时，默认响应 onMaster
            onMaster();
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
        _keepAliveNodeFHelper->reportVersion(TARS_VERSION);
//        TARS_REPORTVERSION(TARS_VERSION);

        //发送心跳给node, 表示启动了
        _keepAliveNodeFHelper->keepAlive();
//        TARS_KEEPALIVE("");

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
        if(_conf.get("/tars/application/server<closecout>",AppCache::getInstance()->get("closeCout")) != "0")
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

void Application::initializeAdapter()
{
    //创建adapter, 不绑定端口
    vector <TC_EpollServer::BindAdapterPtr> adapters = createAdapter();

    if (!_serverBaseInfo.Local.empty())
    {
        _servantHelper->addServant<AdminServant>("AdminObj", this);

        string adminAdapter = "AdminAdapter";

        _servantHelper->setAdapterServant(adminAdapter, "AdminObj");

        TC_EpollServer::BindAdapterPtr lsPtr = _epollServer->createBindAdapter<ServantHandle>(adminAdapter, _serverBaseInfo.Local, 1, this);

        setAdapter(lsPtr, adminAdapter);

        lsPtr->setMaxConns(TC_EpollServer::BindAdapter::DEFAULT_MAX_CONN);

        lsPtr->setQueueCapacity(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_CAP);

        lsPtr->setQueueTimeout(TC_EpollServer::BindAdapter::DEFAULT_QUEUE_TIMEOUT);

        lsPtr->setProtocolName("tars");

        lsPtr->setProtocol(AppProtocol::parse);

        adapters.push_back(lsPtr);
//            _epollServer->bind(lsPtr);
    }

    _epollServer->setAdapter(adapters);

    stringstream os;

    //输出所有adapter
    outAllAdapter(os);

    __out__.info() << os.str();

    __out__.info() << "\n" << TC_Common::outfill("[initialize server] ", '.')  << " [Done]" << endl;

    __out__.info() << OUT_LINE_LONG << endl;

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

    //初始化通信器
    if(!_communicator)
    {
        _communicator = CommunicatorFactory::getInstance()->getCommunicator(_conf);
        _applicationCommunicator = _communicator;
    }
    else
    {
        //一个进程内嵌多个Application时发生, 正常业务服务不会发生
        _applicationCommunicator = CommunicatorFactory::getInstance()->getCommunicator(_conf, TC_Common::tostr(this));
    }
    _applicationCommunicator->initialize();

	__out__.info()  << TC_Common::outfill("[proxy config]:") << endl;

    //输出
    stringstream os;
    outClient(os);

	__out__.info() << os.str();
}

void Application::outClient(ostream &os)
{
	os << OUT_LINE << "\n" << TC_Common::outfill("[load client]:") << endl;

    os << TC_Common::outfill("locator")                     << _applicationCommunicator->getProperty("locator") << endl;
    os << TC_Common::outfill("sync-invoke-timeout")         << _applicationCommunicator->getProperty("sync-invoke-timeout") << endl;
    os << TC_Common::outfill("async-invoke-timeout")        << _applicationCommunicator->getProperty("async-invoke-timeout") << endl;
    os << TC_Common::outfill("refresh-endpoint-interval")   << _applicationCommunicator->getProperty("refresh-endpoint-interval") << endl;
    os << TC_Common::outfill("stat")                        << _applicationCommunicator->getProperty("stat") << endl;
    os << TC_Common::outfill("property")                    << _applicationCommunicator->getProperty("property") << endl;
    os << TC_Common::outfill("report-interval")             << _applicationCommunicator->getProperty("report-interval") << endl;
    os << TC_Common::outfill("keep-alive-interval")             << _applicationCommunicator->getProperty("keep-alive-interval") << endl;
    os << TC_Common::outfill("netthread")                  << _applicationCommunicator->getProperty("netthread") << endl;
    os << TC_Common::outfill("asyncthread")                 << _applicationCommunicator->getProperty("asyncthread") << endl;
    os << TC_Common::outfill("modulename")                  << _applicationCommunicator->getProperty("modulename") << endl;
    os << TC_Common::outfill("enableset")                     << _applicationCommunicator->getProperty("enableset") << endl;
    os << TC_Common::outfill("setdivision")                 << _applicationCommunicator->getProperty("setdivision") << endl;
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

    if (adapterName.empty())
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
    for (const auto & _acceptFunc : _acceptFuncs)
    {
        _acceptFunc(cPtr);
    }
}

TC_EpollServer::BindAdapterPtr Application::getBindAdapter(const string &obj)
{
    string adapter = this->_servantHelper->getServantAdapter(obj);

    return _epollServer->getBindAdapter(adapter);
}

bool Application::isTerminate()
{
    return !_epollServer || _epollServer->isTerminate();
}

//void Application::addServantOnClose(const string& servant, const TC_EpollServer::close_functor& cf)
//{
//    string adapterName = _servantHelper->getServantAdapter(servant);
//
//    if (adapterName.empty())
//    {
//        throw runtime_error("setServantOnClose fail, no found adapter for servant:" + servant);
//    }
//
//    getEpollServer()->getBindAdapter(adapterName)->setOnClose(cf);
//}

void Application::outServer(ostream &os)
{
	os << TC_Common::outfill("Application(app)")            << ServerConfig::Application << endl;
	os << TC_Common::outfill("ServerName(server)")          << ServerConfig::ServerName << endl;
    os << TC_Common::outfill("NodeName(nodename)")          << ServerConfig::NodeName << endl;
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
	os << TC_Common::outfill("ReportFlow(reportflow)")                  << ServerConfig::ReportFlow<< endl;
	os << TC_Common::outfill("BackPacketLimit(backpacketlimit)")  << ServerConfig::BackPacketLimit<< endl;
	os << TC_Common::outfill("BackPacketMin(backpacketmin)")  << ServerConfig::BackPacketMin<< endl;
    os << TC_Common::outfill("BakType(baktype)")  << ServerConfig::BakType << endl;
    os << TC_Common::outfill("BakFlag(bakflag)")  << ServerConfig::BakFlag << endl;

#if TARS_SSL
	os << TC_Common::outfill("Ca(ca)")                    << ServerConfig::CA << endl;
	os << TC_Common::outfill("Cert(cert)")              << ServerConfig::Cert << endl;
	os << TC_Common::outfill("Key(key)")                  << ServerConfig::Key << endl;
	os << TC_Common::outfill("VerifyClient(verifyclient)")      << ServerConfig::VerifyClient << endl;
	os << TC_Common::outfill("Ciphers(ciphers)")               << ServerConfig::Ciphers << endl;
#endif

}


void Application::initializeServer()
{
	__out__.info()  << OUT_LINE << "\n" << TC_Common::outfill("[server config]:") << endl;

    ServerConfig::Application  = toDefault(_conf.get("/tars/application/server<app>"), "UNKNOWN");

    //缺省采用进程名称
    string exe;

    try
    {
        exe = TC_File::extractFileName(TC_File::getExePath());
    }
    catch (TC_File_Exception & ex)
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
    ServerConfig::BakFlag           = TC_Common::strto<int>(_conf.get("/tars/application/server<bakflag>", "0"));
    ServerConfig::BakType           = TC_Common::strto<int>(_conf.get("/tars/application/server<baktype>", "0"));

    ServerConfig::LogSize           = TC_Common::toSize(toDefault(_conf.get("/tars/application/server<logsize>"), "52428800"), 52428800);
    ServerConfig::LogNum            = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<lognum>"), "10"));
    ServerConfig::LocalIp           = _conf.get("/tars/application/server<localip>");
    ServerConfig::NodeName          = toDefault(_conf.get("/tars/application/server<nodename>"), ServerConfig::LocalIp);
    ServerConfig::Local             = _conf.get("/tars/application/server<local>");
    ServerConfig::Node              = _conf.get("/tars/application/server<node>");
    ServerConfig::Log               = _conf.get("/tars/application/server<log>");
    ServerConfig::Config            = _conf.get("/tars/application/server<config>");
    ServerConfig::Notify            = _conf.get("/tars/application/server<notify>");
    ServerConfig::ReportFlow        = _conf.get("/tars/application/server<reportflow>")=="0"?0:1;
    ServerConfig::IsCheckSet        = _conf.get("/tars/application/server<checkset>","1")=="0"?0:1;
    ServerConfig::OpenCoroutine     = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<opencoroutine>"), "0"));
    ServerConfig::CoroutineMemSize  =  TC_Common::toSize(toDefault(_conf.get("/tars/application/server<coroutinememsize>"), "1G"), 1024*1024*1024);
    ServerConfig::CoroutineStackSize= (uint32_t)TC_Common::toSize(toDefault(_conf.get("/tars/application/server<coroutinestack>"), "128K"), 1024*128);
    ServerConfig::ManualListen      = _conf.get("/tars/application/server<manuallisten>", "0") == "0" ? false : true;
	ServerConfig::NetThread         = TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<netthread>"), "1"));
	ServerConfig::CloseCout        = _conf.get("/tars/application/server<closecout>","1")=="0"?0:1;
	ServerConfig::BackPacketLimit  = TC_Common::strto<int>(_conf.get("/tars/application/server<backpacketlimit>", TC_Common::tostr(100*1024*1024)));
	ServerConfig::BackPacketMin    = TC_Common::strto<int>(_conf.get("/tars/application/server<backpacketmin>", "1024"));

#if TARS_SSL
	ServerConfig::CA                = _conf.get("/tars/application/server<ca>");
	ServerConfig::Cert              = _conf.get("/tars/application/server<cert>");
	ServerConfig::Key               = _conf.get("/tars/application/server<key>");
	ServerConfig::VerifyClient      = _conf.get("/tars/application/server<verifyclient>","0")=="0"?false:true;
	ServerConfig::Ciphers           = _conf.get("/tars/application/server<ciphers>");

	if(!ServerConfig::Cert.empty()) {
		_ctx = TC_OpenSSL::newCtx(ServerConfig::CA, ServerConfig::Cert, ServerConfig::Key, ServerConfig::VerifyClient, ServerConfig::Ciphers);

		if (!_ctx) {
			TLOGERROR("[load server ssl error, ca:" << ServerConfig::CA << endl);
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
        for(const auto & i : v)
        {
            if(i != "127.0.0.1")
            {
                ServerConfig::LocalIp = i;
                break;
            }
        }
    }

    //设置节点名称, 请求tarsregistry会在context中带过去, 方便知道从哪来的请求
    _applicationCommunicator->_clientConfig.NodeName = ServerConfig::NodeName;
    _applicationCommunicator->_clientConfig.Context["node_name"] = ServerConfig::NodeName;

    onServerConfig();

	ostringstream os;
    //输出信息
    outServer(os);

	__out__.info() << os.str();

	if (ServerConfig::NetThread < 1)
    {
        ServerConfig::NetThread = 1;
	    __out__.info()  << OUT_LINE << "\nwarning:netThreadNum < 1." << endl;
    }

    //网络线程的配置数目不能15个
    if (ServerConfig::NetThread > 15)
    {
        ServerConfig::NetThread = 15;
	    __out__.info()  << OUT_LINE << "\nwarning:netThreadNum > 15." << endl;
    }

    if(ServerConfig::CoroutineMemSize/ServerConfig::CoroutineStackSize <= 0)
	{
		__out__.error()  << OUT_LINE << "\nerror:coroutine paramter error: coroutinememsize/coroutinestack <= 0!." << endl;
		exit(-1);
	}
    _epollServer = new TC_EpollServer();

    _epollServer->setThreadNum(ServerConfig::NetThread);
    _epollServer->setOpenCoroutine((TC_EpollServer::SERVER_OPEN_COROUTINE)ServerConfig::OpenCoroutine);
	_epollServer->setCoroutineStack(ServerConfig::CoroutineMemSize/ServerConfig::CoroutineStackSize, ServerConfig::CoroutineStackSize);

    _epollServer->setOnAccept(std::bind(&Application::onAccept, this, std::placeholders::_1));

    //初始化服务是否对空链接进行超时检查
    _epollServer->setEmptyConnTimeout(TC_Common::strto<int>(toDefault(_conf.get("/tars/application/server<emptyconntimeout>"), "0")));


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地文件cache
	__out__.info()  << OUT_LINE << "\n" << TC_Common::outfill("[set file cache ]") << "OK" << endl;
    AppCache::getInstance()->setCacheInfo(ServerConfig::DataPath + ServerConfig::ServerName + ".tarsdat", 0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化本地Log
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set roll logger] ") << "OK" << endl;
    LocalRollLogger::getInstance()->setLogInfo(ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath, ServerConfig::LogSize, ServerConfig::LogNum, _applicationCommunicator, ServerConfig::Log);
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
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set time logger] ") << "OK" << endl;
    bool bLogStatReport = (_conf.get("/tars/application/server<logstatreport>", "0") == "1") ? true : false;
    RemoteTimeLogger::getInstance()->setLogInfo(_applicationCommunicator, ServerConfig::Log, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::LogPath, setDivision(), bLogStatReport);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到配置中心代理
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set remote config] ") << "OK" << endl;
    _remoteConfig = std::make_shared<RemoteConfig>();
    _remoteConfig->setConfigInfo(_applicationCommunicator, ServerConfig::Config, ServerConfig::Application, ServerConfig::ServerName, ServerConfig::BasePath, setDivision(), 5);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到信息中心代理
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set remote notify] ") << "OK" << endl;
    RemoteNotify::getInstance()->setNotifyInfo(_applicationCommunicator, ServerConfig::Notify, ServerConfig::Application, ServerConfig::ServerName, setDivision());

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化到Node的代理
    __out__.info() << OUT_LINE << "\n" << TC_Common::outfill("[set node proxy]") << "OK" << endl;
    _keepAliveNodeFHelper = std::make_shared<KeepAliveNodeFHelper>();
    _keepAliveNodeFHelper->setNodeInfo(_applicationCommunicator, ServerConfig::Node, ServerConfig::Application, ServerConfig::ServerName);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //初始化管理对象
    __out__.info()  << OUT_LINE << "\n" << TC_Common::outfill("[set admin adapter]") << "OK" << endl;

    //队列取平均值
    if(!_applicationCommunicator->getProperty("property").empty())
    {
        string sRspQueue= "sendrspqueue";

        g_pReportRspQueue = _applicationCommunicator->getStatReport()->createPropertyReport(sRspQueue, PropertyReport::avg());
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

vector<TC_EpollServer::BindAdapterPtr> Application::createAdapter()
{
    vector<TC_EpollServer::BindAdapterPtr> adapters;

    string sPrefix = _serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".";

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
                ep.setHost(_serverBaseInfo.LocalIp);
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
                __out__.error()  << "load server ssl error, no cert config!" << bindAdapter->getEndpoint().toString() << endl;
                exit(-1);
            }
#endif

            if(_serverBaseInfo.ManualListen)
            {
                //手工监听
                bindAdapter->enableManualListen();
            }

            //队列取平均值
            if(!_applicationCommunicator->getProperty("property").empty())
            {
                PropertyReportPtr p;
                p = _applicationCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".queue", PropertyReport::avg());
                bindAdapter->_pReportQueue = p.get();

                p = _applicationCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".connectRate", PropertyReport::avg());
                bindAdapter->_pReportConRate = p.get();

                p = _applicationCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".timeoutNum", PropertyReport::sum());
                bindAdapter->_pReportTimeoutNum = p.get();

                p = _applicationCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".queueWaitTime",
                                                                                    PropertyReport::avg(), PropertyReport::min(), PropertyReport::max(), PropertyReport::count());
                bindAdapter->_pReportQueueWaitTime = p.get();

                p = _applicationCommunicator->getStatReport()->createPropertyReport(bindAdapter->getName() + ".servantHandleTime",
                                                                                    PropertyReport::avg(), PropertyReport::min(), PropertyReport::max(), PropertyReport::count());
                bindAdapter->_pReportServantHandleTime = p.get();
            }

            adapters.push_back(bindAdapter);
        }
    }

    return adapters;
}

void Application::bindAdapters()
{
    for(auto adapter : _epollServer->getBindAdapters())
    {
        _epollServer->bind(adapter);
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

void Application::checkMasterSlave(int timeout)
{
    int exceptionTimes = 0;

    GetMasterSlaveLock req;
    req.application = _serverBaseInfo.Application;
    req.server_name = _serverBaseInfo.ServerName;
    req.node_name = _serverBaseInfo.NodeName;
    req.bakType = _serverBaseInfo.BakType;
    req.isMaster = false;
    req.lastErr = false;
    req.timeout = timeout;

    bool firstCheck = true;

    QueryFPrx queryFPrx = _applicationCommunicator->stringToProxy<QueryFPrx>(_applicationCommunicator->getProperty("locator"));

    do
    {
        try
        {
            int ret = queryFPrx->getLocker(req);

            if (0 == ret)
            {
                //抢到主了, 如果不是主, 则变成主, 且回调
                req.lastErr = false;
                if (!req.isMaster)
                {
                    req.isMaster = true;
                    TLOG_INFO("This service node has become the master node!" << endl);
                    TARS_NOTIFY_ERROR("Auto change to be master, This service node become the master!");
                    onMaster();
                }
                firstCheck = false;
            } else if (1 == ret)
            {
                //没抢到主, 如果不是备, 则变成备, 且回调
                req.lastErr = false;
                if (req.isMaster || firstCheck)
                {
                    req.isMaster = false;
                    TLOG_ERROR("The service node has become the slave node." << endl);
                    TARS_NOTIFY_ERROR("Auto change to be slave, This service node become the slave!");
                    onSlave();
                }
                firstCheck = false;
            } else
            {
                TLOG_ERROR("getLocker return exception, ret:" << ret << endl);
                req.lastErr = true;
            }

            exceptionTimes = 0;
        }
        catch (exception &ex)
        {
            ++exceptionTimes;
            TLOG_ERROR("getLocker call exception:" << ex.what() << endl);
//            if (req.isMaster)
//            {
//                if (exceptionTimes > 3)
//                {
//                    //持续3次都无法访问主控, 则认为自己异常了!!
//                    req.isMaster = false;
//                    TLOG_ERROR("The service node has become the slave node because call getLocker exception." << endl);
//                    TARS_NOTIFY_ERROR(
//                            "Auto change to be slave, This service node has become the slave node because call getLocker exception!");
//                    onSlave();
//                }
//            }
        }

        TC_ThreadLock::Lock lock(_masterSlaveLock);
        _masterSlaveLock.timedWait(timeout/3);
    }
    while(!_terminateCheckMasterSlave);
}

}
