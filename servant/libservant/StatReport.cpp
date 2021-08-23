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

#include "servant/StatReport.h"
#include "util/tc_common.h"
#include "util/tc_timeprovider.h"
#include "servant/RemoteLogger.h"
#include <iostream>
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <arpa/inet.h>
#endif
#include "servant/Application.h"
#include "servant/Communicator.h"
#include "servant/CommunicatorEpoll.h"

namespace tars
{
//////////////////////////////////////////////////////////////////
//
StatReport::StatReport(Communicator* communicator)
: _communicator(communicator)
, _time(0)
, _reportInterval(60000)
, _reportTimeout(5000)
, _maxReportSize(MAX_REPORT_SIZE)
, _terminate(false)
, _sampleRate(1)
, _maxSampleCount(500)
, _retValueNumLimit(10)
{
	srand(time(NULL));
}

StatReport::~StatReport()
{
    if (isAlive())
    {
        terminate();

        getThreadControl().join();
    }
}

void StatReport::terminate()
{
    Lock lock(*this);

    _terminate = true;

    notifyAll();
}

void StatReport::setReportInfo(const StatFPrx& statPrx,
                       const PropertyFPrx& propertyPrx,
                       const string& strModuleName,
                       const string& strModuleIp,
                       const string& strSetDivision,
                       int iReportInterval,
                       int iSampleRate,
                       unsigned int iMaxSampleCount,
                       int iMaxReportSize,
                       int iReportTimeout)
{
    Lock lock(*this);

    _statPrx        = statPrx;

    _propertyPrx    = propertyPrx;

    //包头信息,trim&substr 防止超长导致udp包发送失败
    _moduleName  = trimAndLimitStr(strModuleName, MAX_MASTER_NAME_LEN);

    _ip          = trimAndLimitStr(strModuleIp, MAX_MASTER_IP_LEN);

    _time          = TNOW;

    _reportInterval    = iReportInterval < 10000 ? 10000 : iReportInterval;

    _reportTimeout = iReportTimeout < 5000 ? 5000 : iReportTimeout;

    _sampleRate        = (iSampleRate < 1)?1: iSampleRate;

    _maxSampleCount    = iMaxSampleCount>500?500:iMaxSampleCount;

    if ( iMaxReportSize < MIN_REPORT_SIZE || iMaxReportSize > MAX_REPORT_SIZE )
    {
        _maxReportSize = MAX_REPORT_SIZE;
    }
    else
    {
        _maxReportSize = iMaxReportSize;
    }
    vector<string> vtSetInfo = TC_Common::sepstr<string>(strSetDivision,".");
    if (vtSetInfo.size()!=3 ||(vtSetInfo[0]=="*"||vtSetInfo[1]=="*"))
    {
        _setArea= "";
        _setID  = "";
    }
    else
    {
        _setName         = vtSetInfo[0];
        _setArea         = vtSetInfo[1];
        _setID           = vtSetInfo[2];
    }
    //TLOGDEBUG("setReportInfo Division:" << strSetDivision << " " << _setName << " " << _setArea << " " << _setID  << endl);
    resetStatInterv();

    if (!isAlive())
    {
        start();
    }
}

void StatReport::addStatInterv(int iInterv)
{
    Lock lock(*this);

    _timePoint.push_back(iInterv);

    sort(_timePoint.begin(),_timePoint.end());

    auto it = unique(_timePoint.begin(),_timePoint.end());

	_timePoint.resize(std::distance(_timePoint.begin(),it));
}

void StatReport::getIntervCount(int time, StatMicMsgBody& body)
{
    int iTimePoint = 0;
    bool bNeedInit = false;
    bool bGetIntev = false;
    if (body.intervalCount.size() == 0)  //第一次需要将所有描点值初始化为0
    {
        bNeedInit = true;
    }
    for(int i =0;i<(int)_timePoint.size();i++)
    {
        iTimePoint = _timePoint[i];
        if(bGetIntev == false && time < iTimePoint)
        {
            bGetIntev = true;
            body.intervalCount[iTimePoint]++;
            if(bNeedInit == false)
                break;
            else
                continue;
        }
        if(bNeedInit == true)
        {
           body.intervalCount[iTimePoint] = 0;
        }
    }
    return;
}

void  StatReport::resetStatInterv()
{
    _timePoint.clear();
    _timePoint.push_back(5);
    _timePoint.push_back(10);
    _timePoint.push_back(50);
    _timePoint.push_back(100);
    _timePoint.push_back(200);
    _timePoint.push_back(500);
    _timePoint.push_back(1000);
    _timePoint.push_back(2000);
    _timePoint.push_back(3000);

    sort(_timePoint.begin(),_timePoint.end());

    unique(_timePoint.begin(),_timePoint.end());
}

string StatReport::trimAndLimitStr(const string& str, uint32_t limitlen)
{
    static const string strTime = "\r\t";

    string ret = TC_Common::trim(str, strTime);

    if (ret.length() > limitlen)
    {
        ret.resize(limitlen);
    }
    return ret;
}

bool StatReport::divison2SetInfo(const string& str, vector<string>& vtSetInfo)
{
    vtSetInfo = TC_Common::sepstr<string>(str,".");

    if (vtSetInfo.size() != 3 ||(vtSetInfo[0]=="*"||vtSetInfo[1]=="*"))
    {
        TLOGERROR(__FUNCTION__ << ":" << __LINE__ << "|bad set name [" << str << endl);
        return false;
    }
    return true;
}

/*
tars.tarsstat to tarsstat
*/
string StatReport::getServerName(string sModuleName)
{
    string::size_type pos =  sModuleName.find(".");
    if (pos != string::npos)
    {
        return sModuleName.substr(pos + 1); //+1:过滤.
    }
    return  sModuleName;
}


void StatReport::report(const string& strModuleName,
                        const string& setdivision,
                        const string& strInterfaceName,
                        const string& strModuleIp,
                        uint16_t iPort,
                        StatResult eResult,
                        int iSptime,
                        int iReturnValue,
                        bool bFromClient)
{
    //包头信息,trim&substr 防止超长导致udp包发送失败
    //masterIp为空服务端自己获取。

    StatMicMsgHead head;
    StatMicMsgBody body;
    string sMaterServerName = "";
    string sSlaveServerName = "";
    string appName = ""; // 由setdivision生成

    if (bFromClient)
    {
        if (!_setName.empty())
        {
            head.masterName = _moduleName + "." + _setName + _setArea + _setID + "@" + ClientConfig::TarsVersion;
        }
        else
        {
            head.masterName = _moduleName + "@" + ClientConfig::TarsVersion;
        }

        if (!setdivision.empty()) //被调没有启用set分组,slavename保持原样
        {
            vector <string> vtSetInfo;
            if(divison2SetInfo(setdivision, vtSetInfo))
            {
                head.slaveSetName         = vtSetInfo[0];
                head.slaveSetArea         = vtSetInfo[1];
                head.slaveSetID          = vtSetInfo[2];
            }
            head.slaveName = trimAndLimitStr(strModuleName, MAX_MASTER_NAME_LEN) + "." + head.slaveSetName + head.slaveSetArea + head.slaveSetID;
        }
        else
        {
            head.slaveName = trimAndLimitStr(strModuleName, MAX_MASTER_NAME_LEN);
        }
        
        head.masterIp       = "";
        head.slaveIp        = trimAndLimitStr(strModuleIp, MAX_MASTER_IP_LEN);

    }
    else
    {
        //被调上报,masterName没有set信息
        head.masterName     = trimAndLimitStr(strModuleName, MAX_MASTER_NAME_LEN);

        head.masterIp       = trimAndLimitStr(strModuleIp, MAX_MASTER_IP_LEN);


        if(_setName.empty()) //被调上报，slave的set信息为空
        {
            head.slaveName      = _moduleName;//服务端version不需要上报
        }
        else
        {
            head.slaveName    = _moduleName + "." + _setName + _setArea + _setID;
        }
        head.slaveIp             = "";

        head.slaveSetName        = _setName;

        head.slaveSetArea        = _setArea;

        head.slaveSetID          = _setID;
    }

    head.interfaceName  = trimAndLimitStr(strInterfaceName, MAX_MASTER_NAME_LEN);

    head.slavePort      = iPort;

    head.returnValue    = iReturnValue;

    //包体信息.
    if (eResult == STAT_SUCC)
    {
        body.count = 1;

        body.totalRspTime = body.minRspTime = body.maxRspTime = iSptime;
    }
    else if (eResult == STAT_TIMEOUT)
    {
        body.timeoutCount = 1;
    }
    else
    {
        body.execCount = 1;
    }
    submit(head, body, bFromClient);
}

void StatReport::report(const string& strMasterName,
                        const string& strMasterIp,
                        const string& strSlaveName,
                        const string& strSlaveIp,
                        uint16_t iSlavePort,
                        const string& strInterfaceName,
                        StatResult eResult,
                        int  iSptime,
                        int  iReturnValue)
{
    //包头信息,trim&substr 防止超长导致udp包发送失败
    //masterIp为空服务端自己获取。

    StatMicMsgHead head;
    StatMicMsgBody body;

    head.masterName     = trimAndLimitStr(strMasterName + "@" + ClientConfig::TarsVersion, MAX_MASTER_NAME_LEN);
    head.masterIp       = trimAndLimitStr(strMasterIp,      MAX_MASTER_IP_LEN);
    head.slaveName      = trimAndLimitStr(strSlaveName,     MAX_MASTER_NAME_LEN);
    head.slaveIp        = trimAndLimitStr(strSlaveIp,       MAX_MASTER_IP_LEN);
    head.interfaceName  = trimAndLimitStr(strInterfaceName, MAX_MASTER_NAME_LEN);
    head.slavePort      = iSlavePort;
    head.returnValue    = iReturnValue;

    //包体信息.
    if (eResult == STAT_SUCC)
    {
        body.count = 1;

        body.totalRspTime = body.minRspTime = body.maxRspTime = iSptime;
    }
    else if (eResult == STAT_TIMEOUT)
    {
        body.timeoutCount = 1;
    }
    else
    {
        body.execCount = 1;
    }

    submit(head, body, true);
}

void StatReport::submit(StatMicMsgHead& head, StatMicMsgBody& body, bool bFromClient)
{
    Lock lock(*this);

    MapStatMicMsg& msg = (bFromClient == true)?_statMicMsgClient:_statMicMsgServer;

    auto it = msg.find( head );

    if ( it != msg.end() )
    {
        StatMicMsgBody& stBody      = it->second;
        stBody.count                += body.count;
        stBody.timeoutCount         += body.timeoutCount;
        stBody.execCount            += body.execCount;
        stBody.totalRspTime         += body.totalRspTime;
        if (stBody.maxRspTime < body.maxRspTime)
        {
            stBody.maxRspTime = body.maxRspTime;
        }
        //非0最小值
        if (stBody.minRspTime == 0 || (stBody.minRspTime > body.minRspTime && body.minRspTime != 0))
        {
            stBody.minRspTime = body.minRspTime;
        }
        getIntervCount(body.maxRspTime, stBody);
    }
    else
    {
        getIntervCount(body.maxRspTime, body);
        msg[head] = body;
    }
}

int StatReport::reportMicMsg(MapStatMicMsg& msg, bool bFromClient)
{
    if (msg.empty()) return 0;
    try
    {
        int iLen = 0;
        MapStatMicMsg  mTemp;
        MapStatMicMsg  mStatMsg;
        mStatMsg.clear();
        mTemp.clear();
        {
            Lock lock(*this);
            msg.swap(mStatMsg);
        }

       TLOGTARS("[StatReport::reportMicMsg get size:" << mStatMsg.size()<<"]"<< endl);
       for(MapStatMicMsg::iterator it = mStatMsg.begin(); it != mStatMsg.end(); it++)
       {
           const StatMicMsgHead &head = it->first;
           int iTemLen = STAT_PROTOCOL_LEN +head.masterName.length() + head.slaveName.length() + head.interfaceName.length()
               + head.slaveSetName.length() + head.slaveSetArea.length() + head.slaveSetID.length();
           iLen = iLen + iTemLen;
           if(iLen > _maxReportSize) //不能超过udp 1472
           {
               if(_statPrx)
               {
                   TLOGTARS("[StatReport::reportMicMsg send size:" << mTemp.size()<<"]"<< endl);
                   _statPrx->tars_set_timeout(_reportTimeout)->async_reportMicMsg(NULL,mTemp,bFromClient, ServerConfig::Context);
               }
               iLen = iTemLen;
               mTemp.clear();
           }

           mTemp[head] = it->second;
           if(LOG->isNeedLog(LocalRollLogger::INFO_LOG))
           {
               ostringstream os;
               os.str("");
               head.displaySimple(os);
               os << "  ";
               mTemp[head].displaySimple(os);
               TLOGTARS("[StatReport::reportMicMsg display:" << os.str() << "]" << endl);
           }
       }
       if(0 != (int)mTemp.size())
       {
           if(_statPrx)
           {
               TLOGTARS("[StatReport::reportMicMsg send size:" << mTemp.size()<<"]"<< endl);
               _statPrx->tars_set_timeout(_reportTimeout)->async_reportMicMsg(NULL,mTemp,bFromClient, ServerConfig::Context);
           }
       }
       return 0;
    }
    catch (exception& e)
    {
        TLOGERROR("StatReport::report catch exception:" << e.what() << endl);
    }
    catch ( ... )
    {
        TLOGERROR("StatReport::report catch unkown exception" << endl);
    }
    return -1;
}

int StatReport::reportPropMsg()
{
    try
    {
        MapStatPropMsg mStatMsg;

       {
           Lock lock(*this);
           for(map<string, PropertyReportPtr>::iterator it = _statPropMsg.begin(); it != _statPropMsg.end(); ++it)
           {
               StatPropMsgHead head;
               StatPropMsgBody body;

               if (!it->second->getMasterName().empty())
               {
                   if (!_setName.empty())
                   {
                       head.moduleName = it->second->getMasterName() + "." + _setName + _setArea + _setID;
                   }
                   else
                   {
                       head.moduleName = it->second->getMasterName();
                   }
               }
               else
               {
                   if (!_setName.empty())
                   {
                       head.moduleName = _moduleName + "." + _setName + _setArea + _setID;
                   }
                   else
                   {
                       head.moduleName = _moduleName;
                   }
               }

               head.ip              = "";
               head.propertyName    = it->first;
               head.setName         = _setName;
               head.setArea         = _setArea;
               head.setID           = _setID;
               head.iPropertyVer    = 2;

               vector<pair<string, string> > v = it->second->get();
               for(size_t i = 0; i < v.size(); i++)
               {
                //    bool bFlag = false;
                //    if(v[i].first == "Sum")
                //    {
                //         if(v[i].second != "0")
                //             bFlag = true;
                //    }
                //    else if(v[i].first == "Avg")
                //    {
                //         if(v[i].second != "0")
                //             bFlag = true;
                //    }
                //     else if(v[i].first == "Distr")
                //    {
                //         if(v[i].second != "")
                //             bFlag = true;
                //    }
                //    else if(v[i].first == "Max")
                //    {
                //         if(v[i].second != "-9999999")
                //             bFlag = true;
                //    }
                //    else if(v[i].first == "Min")
                //    {
                //         if(v[i].second != "0")
                //             bFlag = true;
                //    }
                //    else if(v[i].first == "Count")
                //    {
                //         if(v[i].second != "0")
                //             bFlag = true;
                //    }
                //    else
                //    {
                //         bFlag = true;
                //    }

                //    if(bFlag)
                   {
                        StatPropInfo sp;
                        sp.policy = v[i].first;
                        sp.value  = v[i].second;
                        body.vInfo.push_back(sp);
                   }
               }
               mStatMsg[head]  = body;
               if(LOG->isNeedLog(LocalRollLogger::INFO_LOG))
               {
                      ostringstream os;
                   os.str("");
                      head.displaySimple(os);
                   os << "  ";
                   mStatMsg[head].displaySimple(os);
                   TLOGTARS("[StatReport::reportPropMsg display:" << os.str() << "]" << endl);
               }
           }
       }

        TLOGTARS("[StatReport::reportPropMsg get size:" << mStatMsg.size() << "]" << endl);
        int iLen = 0;
        MapStatPropMsg mTemp;
        for (MapStatPropMsg::iterator it = mStatMsg.begin(); it != mStatMsg.end(); it++)
        {
            const StatPropMsgHead& head = it->first;
            const StatPropMsgBody& body = it->second;
            int iTemLen = head.moduleName.length() + head.ip.length() + head.propertyName.length() + head.setName.length() + head.setArea.length() + head.setID.length();
            for (size_t i = 0; i < body.vInfo.size(); i++)
            {
                iTemLen += body.vInfo[i].policy.length();
                iTemLen += body.vInfo[i].value.length();
            }
            iTemLen = PROPERTY_PROTOCOL_LEN + body.vInfo.size(); //
            iLen = iLen + iTemLen;
            if (iLen > _maxReportSize) //不能超过udp 1472
            {
                if (_propertyPrx)
                {
                    TLOGTARS("[StatReport::reportPropMsg send size:" << mTemp.size() << "]" << endl);
                    _propertyPrx->tars_set_timeout(_reportTimeout)->async_reportPropMsg(NULL, mTemp);
                }
                iLen = iTemLen;
                mTemp.clear();
            }
            mTemp[it->first] = it->second;
        }
        if (0 != (int)mTemp.size())
        {
            if (_propertyPrx)
            {
                TLOGTARS("[StatReport::reportPropMsg send size:" << mTemp.size() << "]" << endl);
                _propertyPrx->tars_set_timeout(_reportTimeout)->async_reportPropMsg(NULL, mTemp);
            }
        }
        return 0;
    }
    catch ( exception& e )
    {
        TLOGERROR("StatReport::reportPropMsg catch exception:" << e.what() << endl);
    }
    catch ( ... )
    {
        TLOGERROR("StatReport::reportPropMsg catch unkown exception" << endl);
    }
    return -1;
}

void StatReport::addMicMsg(MapStatMicMsg& old, MapStatMicMsg& add)
{
    auto iter = add.begin();
    for (; iter != add.end(); ++iter)
    {
        auto iterOld = old.find(iter->first);
        if (iterOld == old.end())
        {
            //直接insert
            old.insert(make_pair(iter->first, iter->second));
        }
        else
        {
            //合并
            iterOld->second.count += iter->second.count;
            iterOld->second.timeoutCount += iter->second.timeoutCount;
            iterOld->second.execCount += iter->second.execCount;

            map<int, int>::iterator iterOldInt, iterInt;
            map<int, int>& mCount = iter->second.intervalCount;
            map<int, int>& mCountOld = iterOld->second.intervalCount;
            iterInt = mCount.begin();
            for (; iterInt != mCount.end(); ++iterInt)
            {
                iterOldInt = mCountOld.find(iterInt->first);
                if (iterOldInt == mCountOld.end())
                {
                    mCountOld.insert(make_pair(iterInt->first, iterInt->second));
                }
                else
                {
                    iterOldInt->second += iterInt->second;
                }
            }

            iterOld->second.totalRspTime += iter->second.totalRspTime;

            if(iterOld->second.maxRspTime < iter->second.maxRspTime)
                iterOld->second.maxRspTime = iter->second.maxRspTime;

            if(iterOld->second.minRspTime > iter->second.minRspTime)
                iterOld->second.minRspTime = iter->second.minRspTime;
        }
    }
}

void StatReport::run()
{
    while(!_terminate)
    {
        {
            Lock lock(*this);

            timedWait(1000);
        }

        try
        {
            time_t tNow = TNOW;

            if(tNow - _time >= _reportInterval/1000)
            {
                reportMicMsg(_statMicMsgClient, true);

                reportMicMsg(_statMicMsgServer, false);

                MapStatMicMsg mStatMsg;

                auto communicatorEpolls = _communicator->getAllCommunicatorEpoll();

                for(auto ce : communicatorEpolls)
                {
                    MapStatMicMsg * pStatMsg;
                    while(ce->popStatMsg(pStatMsg))
                    {
                        addMicMsg(mStatMsg,*pStatMsg);
                        delete pStatMsg;
                    }
                }

                reportMicMsg(mStatMsg, true);

                reportPropMsg();

//                reportSampleMsg();

                _time = tNow;
            }

        }
        catch ( exception& e )
        {
            TLOGERROR("StatReport::run catch exception:" << e.what() << endl);
        }
        catch ( ... )
        {
            TLOGERROR("StatReport::run catch unkown exception" << endl);
        }
    }

    ServantProxyThreadData::g_sp.reset();
}

////////////////////////////////////////////////////////////////
}

