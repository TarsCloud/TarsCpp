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

#include "servant/Communicator.h"
#include "servant/Application.h"
#include "servant/StatReport.h"
#include "servant/RemoteLogger.h"
#include "util/tc_file.h"

namespace tars
{

//////////////////////////////////////////////////////////////////////////////////////////////

string ClientConfig::LocalIp = "127.0.0.1";

string ClientConfig::ModuleName = "unknown";

set<string> ClientConfig::SetLocalIp;

bool ClientConfig::SetOpen = false;

string ClientConfig::SetDivision = "";

string ClientConfig::TarsVersion = string(TARS_VERSION);

//////////////////////////////////////////////////////////////////////////////////////////////

Communicator::Communicator()
: _initialized(false)
, _terminating(false)
, _clientThreadNum(1)
, _statReport(NULL)
, _timeoutLogFlag(true)
, _minTimeout(100)
#ifdef TARS_OPENTRACKING
, _traceManager(NULL)
#endif
{
#if TARGET_PLATFORM_WINDOWS    
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif
    memset(_communicatorEpoll,0,sizeof(_communicatorEpoll));
}

Communicator::Communicator(TC_Config& conf, const string& domain/* = CONFIG_ROOT_PATH*/)
: _initialized(false)
, _terminating(false)
, _timeoutLogFlag(true)
#ifdef TARS_OPENTRACKING
, _traceManager(NULL)
#endif
{
    setProperty(conf, domain);
}

Communicator::~Communicator()
{
    terminate();

#if TARGET_PLATFORM_WINDOWS
    WSACleanup();
#endif
}

bool Communicator::isTerminating()
{
    return _terminating;
}

map<string, string> Communicator::getServantProperty(const string &sObj)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    auto it = _objInfo.find(sObj);
    if (it != _objInfo.end())
    {
        return it->second;
    }

    return map<string, string>();
}

void Communicator::setServantProperty(const string &sObj, const string &name, const string &value)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _objInfo[sObj][name] = value;
}

string Communicator::getServantProperty(const string &sObj, const string &name)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    auto it = _objInfo.find(sObj);
    if (it != _objInfo.end())
    {
        auto vit = it->second.find(name);

        if (vit != it->second.end())
        {
            return vit->second;
        }
    }

    return "";
}

#if TARS_SSL
shared_ptr<TC_OpenSSL> Communicator::newClientSSL(const string &objName)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    auto it = _objCtx.find(objName);
    if (it != _objCtx.end())
    {
        return TC_OpenSSL::newSSL(it->second);
    }

	if(!_ctx) {
		_ctx = TC_OpenSSL::newCtx("", "", "", false, "");
	}

    return TC_OpenSSL::newSSL(_ctx);
}

#endif

void Communicator::setProperty(TC_Config& conf, const string& domain/* = CONFIG_ROOT_PATH*/)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    conf.getDomainMap(domain, _properties);

    string defaultValue = "dft";
    if ((defaultValue == getProperty("enableset", defaultValue)) || (defaultValue == getProperty("setdivision", defaultValue)))
    {
        _properties["enableset"] = conf.get("/tars/application<enableset>", "n");
        _properties["setdivision"] = conf.get("/tars/application<setdivision>", "NULL");
    }

    vector<string> auths;

	if (conf.getDomainVector("/tars/application/client", auths))
	{
		for(size_t i = 0; i < auths.size(); i++)
		{
			map<string, string> &data = _objInfo[auths[i]];
			data["accesskey"] = conf.get("/tars/application/client/" + auths[i] + "<accesskey>");
			data["secretkey"] = conf.get("/tars/application/client/" + auths[i] + "<secretkey>");
			data["ca"]        = conf.get("/tars/application/client/" + auths[i] + "<ca>");
			data["cert"]      = conf.get("/tars/application/client/" + auths[i] + "<cert>");
			data["key"]       = conf.get("/tars/application/client/" + auths[i] + "<key>");
			data["ciphers"]   = conf.get("/tars/application/client/" + auths[i] + "<ciphers>");

#if TARS_SSL

            if (!data["ca"].empty())
            {
                shared_ptr<TC_OpenSSL::CTX> ctx = TC_OpenSSL::newCtx(data["ca"], data["cert"], data["key"], false, data["ciphers"]);
                if (!ctx)
                {
                    TLOGERROR("load obj:" << auths[i] << ", ssl error, ca:" << data["ca"] << endl);
                    exit(-1);
                }

                _objCtx[auths[i]] = ctx;
            }
#endif
        }
    }

//    initClientConfig();
}

void Communicator::initialize()
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    if (_initialized) return;

    _initialized = true;

	ClientConfig::TarsVersion   = TARS_VERSION;

    ClientConfig::SetOpen = TC_Common::lower(getProperty("enableset", "n")) == "y" ? true : false;

    if (ClientConfig::SetOpen)
    {
        ClientConfig::SetDivision = getProperty("setdivision");

        vector<string> vtSetDivisions = TC_Common::sepstr<string>(ClientConfig::SetDivision, ".");

        string sWildCard = "*";

        if (vtSetDivisions.size() != 3 || vtSetDivisions[0] == sWildCard || vtSetDivisions[1] == sWildCard)
        {
            //set分组名不对时默认没有打开set分组
            ClientConfig::SetOpen = false;
            setProperty("enableset","n");
            TLOGERROR( "[set division name error:" << ClientConfig::SetDivision << ", client failed to open set]" << endl);
        }
    }

    ClientConfig::LocalIp = getProperty("localip", "");

    if (ClientConfig::SetLocalIp.empty())
    {
        vector<string> v = TC_Socket::getLocalHosts();
        for (size_t i = 0; i < v.size(); i++)
        {
            if (v[i] != "127.0.0.1" && ClientConfig::LocalIp.empty())
            {
                ClientConfig::LocalIp = v[i];
            }
            ClientConfig::SetLocalIp.insert(v[i]);
        }
    }

    //缺省采用进程名称
    string exe = "";

    try
    {
        exe = TC_File::extractFileName(TC_File::getExePath());
    }
    catch (TC_File_Exception &ex)
    {
        //取失败则使用ip代替进程名
        exe = ClientConfig::LocalIp;
    }

    ClientConfig::ModuleName = getProperty("modulename", exe);

#if TARS_SSL

    string ca      = getProperty("ca");
    string cert    = getProperty("cert");
    string key     = getProperty("key");
    string ciphers = getProperty("ciphers");

	if(!ca.empty()) {
		_ctx = TC_OpenSSL::newCtx(ca, cert, key, false, ciphers);

        if (!_ctx)
        {
            TLOGERROR("load client ssl error, ca:" << ca << endl);
            exit(-1);
        }
    }
#endif

    _servantProxyFactory = new ServantProxyFactory(this);

    //网络线程
    _clientThreadNum = TC_Common::strto<size_t>(getProperty("netthread", "1"));

    if (0 == _clientThreadNum)
    {
        _clientThreadNum = 1;
    }
    else if (MAX_CLIENT_THREAD_NUM < _clientThreadNum)
    {
        _clientThreadNum = MAX_CLIENT_THREAD_NUM;
    }

    //异步线程数
    _asyncThreadNum = TC_Common::strto<size_t>(getProperty("asyncthread", "3"));

    if (_asyncThreadNum == 0)
    {
        _asyncThreadNum = 3;
    }

    if (_asyncThreadNum > MAX_CLIENT_ASYNCTHREAD_NUM)
    {
        _asyncThreadNum = MAX_CLIENT_ASYNCTHREAD_NUM;
    }

    bool merge = TC_Common::strto<bool>(getProperty("mergenetasync", "0"));

    //异步队列的大小
    size_t iAsyncQueueCap = TC_Common::strto<size_t>(getProperty("asyncqueuecap", "100000"));
    if (iAsyncQueueCap < 10000)
    {
        iAsyncQueueCap = 10000;
    }

	//第一个通信器才去启动回调线程
	for (size_t i = 0; i < _asyncThreadNum; ++i) {
		_asyncThread.push_back(new AsyncProcThread(iAsyncQueueCap, merge));
	}

    //stat总是有对象, 保证getStat返回的对象总是有效
    _statReport = new StatReport(_clientThreadNum);

    for (size_t i = 0; i < _clientThreadNum; ++i)
    {
        _communicatorEpoll[i] = new CommunicatorEpoll(this, i);
        _communicatorEpoll[i]->start();
    }

    //异步队列数目上报
    _reportAsyncQueue = getStatReport()->createPropertyReport(ClientConfig::ModuleName + ".asyncqueue", PropertyReport::avg());

    //初始化统计上报接口
    string statObj = getProperty("stat", "");

    string propertyObj = getProperty("property", "");

    int iReportInterval = TC_Common::strto<int>(getProperty("report-interval", "60000"));

    int iReportTimeout = TC_Common::strto<int>(getProperty("report-timeout", "5000"));

//    int iSampleRate = TC_Common::strto<int>(getProperty("sample-rate", "1000"));

	// int iMaxSampleCount = TC_Common::strto<int>(getProperty("max-sample-count", "100"));

    int iMaxReportSize = TC_Common::strto<int>(getProperty("max-report-size", "1400"));

    _timeoutLogFlag = TC_Common::strto<bool>(getProperty("timeout-log-flag", "1"));

    _minTimeout = TC_Common::strto<int64_t>(getProperty("min-timeout", "100"));
    if (_minTimeout < 1)
        _minTimeout = 1;

    StatFPrx statPrx = NULL;
    if (!statObj.empty())
    {
        statPrx = stringToProxy<StatFPrx>(statObj);
    }

    //上报Property信息的代理
    PropertyFPrx propertyPrx = NULL;
    if (!propertyObj.empty())
    {
        propertyPrx = stringToProxy<PropertyFPrx>(propertyObj);
    }

	string sSetDivision = ClientConfig::SetOpen?ClientConfig::SetDivision:"";
	_statReport->setReportInfo(statPrx, propertyPrx, ClientConfig::ModuleName, ClientConfig::LocalIp, sSetDivision, iReportInterval, 0, 0, iMaxReportSize, iReportTimeout);

#if TARS_OPENTRACKING
	string collector_host = getProperty("collector_host", "");
    string collector_port = getProperty("collector_port", "");
    if(!collector_host.empty() && !collector_port.empty())
    {
        //init zipkin config
        zipkin::ZipkinOtTracerOptions options;
        options.service_name = ClientConfig::ModuleName;
        options.service_address = {zipkin::IpVersion::v4, ClientConfig::LocalIp};

        options.sample_rate = strtod(getProperty("sample_rate", "1.0").c_str(), NULL);
        options.collector_host = collector_host;
        options.collector_port = atoi(collector_port.c_str());
        _traceManager = new TraceManager(options);
        assert(_traceManager != NULL);
    }
#endif

}


void Communicator::setProperty(const map<string, string> &properties)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _properties = properties;
}

void Communicator::setProperty(const string &name, const string &value)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    _properties[name] = value;
}

string Communicator::getProperty(const string &name, const string &dft /* = ""*/)
{
    TC_LockT<TC_ThreadRecMutex> lock(*this);

    map<string, string>::iterator it = _properties.find(name);

    if (it != _properties.end())
    {
        return it->second;
    }
    return dft;
}

void Communicator::reloadLocator()
{
    for (size_t i = 0; i < _clientThreadNum; ++i)
    {
        _communicatorEpoll[i]->getObjectProxyFactory()->loadObjectLocator();
    }
}

int Communicator::reloadProperty(string &sResult)
{
    for (size_t i = 0; i < _clientThreadNum; ++i)
    {
        _communicatorEpoll[i]->getObjectProxyFactory()->loadObjectLocator();
    }

    int iReportInterval = TC_Common::strto<int>(getProperty("report-interval", "60000"));

    int iReportTimeout = TC_Common::strto<int>(getProperty("report-timeout", "5000"));

    // int iSampleRate = TC_Common::strto<int>(getProperty("sample-rate", "1000"));

    // int iMaxSampleCount = TC_Common::strto<int>(getProperty("max-sample-count", "100"));

    int iMaxReportSize = TC_Common::strto<int>(getProperty("max-report-size", "1400"));

    string statObj = getProperty("stat", "");

    string propertyObj = getProperty("property", "");

    StatFPrx statPrx = NULL;

    if (!statObj.empty())
    {
        statPrx = stringToProxy<StatFPrx>(statObj);
    }

    PropertyFPrx propertyPrx = NULL;

    if (!propertyObj.empty())
    {
        propertyPrx = stringToProxy<PropertyFPrx>(propertyObj);
    }

    string sSetDivision = ClientConfig::SetOpen ? ClientConfig::SetDivision : "";
    _statReport->setReportInfo(statPrx, propertyPrx, ClientConfig::ModuleName, ClientConfig::LocalIp, sSetDivision, iReportInterval, 0, 0, iMaxReportSize, iReportTimeout);

    sResult = "locator=" + getProperty("locator", "") + "\r\n" +
              "stat=" + statObj + "\r\n" + "property=" + propertyObj + "\r\n" +
              "SetDivision=" + sSetDivision + "\r\n" +
              "report-interval=" + TC_Common::tostr(iReportInterval) + "\r\n" +
              "report-timeout=" + TC_Common::tostr(iReportTimeout) + "\r\n";
    //        "sample-rate=" + TC_Common::tostr(iSampleRate) + "\r\n" +
    //        "max-sample-count=" + TC_Common::tostr(iMaxSampleCount) + "\r\n";

    return 0;
}

vector<TC_Endpoint> Communicator::getEndpoint(const string &objName)
{
    ServantProxy * pServantProxy = getServantProxy(objName);
    return pServantProxy->getEndpoint();
}

vector<TC_Endpoint> Communicator::getEndpoint4All(const string &objName)
{
    ServantProxy *pServantProxy = getServantProxy(objName);
    return pServantProxy->getEndpoint4All();
}

string Communicator::getResourcesInfo()
{
    ostringstream os;
    for (size_t i = 0; i < _clientThreadNum; ++i)
    {
        os << OUT_LINE << endl;
        os << _communicatorEpoll[i]->getResourcesInfo();
    }
    return os.str();
}

void Communicator::notifyUpdateEndpoints(const ServantPrx &prx, const set<EndpointInfo> &active, const set<EndpointInfo> &inactive)
{
    for (size_t i = 0; i < _clientThreadNum; ++i)
    {
        _communicatorEpoll[i]->notifyUpdateEndpoints(prx, active, inactive);
    }
}

void Communicator::terminate()
{
    {
        TC_LockT<TC_ThreadRecMutex> lock(*this);

        if (_terminating)
            return;

        _terminating = true;

        if (_initialized)
        {
            for (size_t i = 0; i < _clientThreadNum; ++i)
            {
            	if(_communicatorEpoll[i]) {
		            _communicatorEpoll[i]->terminate();
	            }
            }

            if (_statReport)
            {
                _statReport->terminate();
            }

        }
    }

	// 释放当前线程数据
	// ServantProxyThreadData::reset();

	//把锁释放掉, 再来等待线程停止, 避免死锁
    //因为通信器线程运行过程中, 有可能加上上面那把锁
    if (_initialized)
    {
    	//停止掉异步线程
	    for (size_t i = 0; i < _asyncThread.size(); ++i)
	    {
		    if (_asyncThread[i])
		    {
			    if (_asyncThread[i]->isAlive())
			    {
				    _asyncThread[i]->terminate();
				    _asyncThread[i]->getThreadControl().join();
			    }
		    }
	    }

	    //停止掉网络线程
        for (size_t i = 0; i < _clientThreadNum; ++i)
        {
        	if(_communicatorEpoll[i]) {
		        _communicatorEpoll[i]->getThreadControl().join();
	        }
        }

        //都停止了, 再把异步线程析构掉(因为异步线程中会调用网络线程的数据)
	    for (size_t i = 0; i < _asyncThread.size(); ++i)
	    {
		    if (_asyncThread[i])
		    {
			    delete _asyncThread[i];
			    _asyncThread[i] = NULL;
		    }
	    }
	    _asyncThread.clear();

        if (_statReport)
        {
            _statReport->getThreadControl().join();
            delete _statReport;
            _statReport = NULL;
        }

	    //delete网络线程
        for (size_t i = 0; i < _clientThreadNum; ++i)
        {
        	if(_communicatorEpoll[i]) {
		        delete _communicatorEpoll[i];
		        _communicatorEpoll[i] = NULL;
	        }
        }

        if (_servantProxyFactory)
        {
            delete _servantProxyFactory;
            _servantProxyFactory = NULL;
        }
    }
}

void Communicator::pushAsyncThreadQueue(ReqMessage *msg)
{
    if (msg->pObjectProxy->getRootServantProxy()->_callback)
    {
        ReqMessagePtr msgPtr = msg;

        msg->pObjectProxy->getRootServantProxy()->_callback(msgPtr);
    }
    else if (msg->pObjectProxy->getRootServantProxy()->_callbackHash)
    {
        //先不考虑每个线程队列数目不一致的情况
        _asyncThread[((uint32_t)msg->adapter->trans()->fd()) % _asyncThreadNum]->push_back(msg);
    }
    else
    {
        //先不考虑每个线程队列数目不一致的情况
        _asyncThread[(_asyncSeq++) % _asyncThreadNum]->push_back(msg);
    }
}

void Communicator::doStat()
{
    //队列长度上报
    if (_reportAsyncQueue)
    {
        size_t n = 0;
        for (size_t i = 0; i < _asyncThread.size(); ++i)
        {
            n = n + _asyncThread[i]->getSize();
        }
        _reportAsyncQueue->report((int)n);
    }
}

ServantProxy *Communicator::getServantProxy(const string &objectName, const string &setName)
{
    Communicator::initialize();

    return _servantProxyFactory->getServantProxy(objectName, setName);
}

StatReport *Communicator::getStatReport()
{
    Communicator::initialize();

    return _statReport;
}

ServantProxyFactory *Communicator::servantProxyFactory()
{
    return _servantProxyFactory;
}
///////////////////////////////////////////////////////////////
}
