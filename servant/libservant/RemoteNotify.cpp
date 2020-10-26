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

#include "servant/RemoteNotify.h"
#include "servant/Communicator.h"
#include "servant/RemoteLogger.h"
#include "servant/Application.h"

namespace tars
{

int RemoteNotify::setNotifyInfo(const CommunicatorPtr &comm, const string &obj, const string & app, const string &serverName, const string &sSetName, const string &nodeName)
{
    _comm           = comm;
    if(!obj.empty())
    {
        _notifyPrx  = _comm->stringToProxy<NotifyPrx>(obj);
        _notifyPrx->tars_timeout(500);
    }

    _setName        = sSetName;
    _app            = app;
    _serverName     = serverName;
    _nodeName      =  nodeName;
    return 0;
}

void RemoteNotify::report(const string &sResult, bool bSync)
{
    try
    {
        if(_notifyPrx)
        {
            ReportInfo info;
            info.eType     = REPORT;
            info.sApp      = _app;
            info.sServer   = _serverName;
            info.sSet      = _setName;
            info.sThreadId = TC_Common::tostr(std::this_thread::get_id());
            info.sMessage  = sResult;
            info.sNodeName = _nodeName;
            if(!bSync)
            {
                _notifyPrx->async_reportNotifyInfo(NULL, info, ServerConfig::Context);
            }
            else
            {
                _notifyPrx->reportNotifyInfo(info, ServerConfig::Context);
            }
        }
    }
    catch(exception &ex)
    {
        TLOGERROR("[RemoteNotify::report error:" << ex.what() << "]" << endl);
    }
    catch(...)
    {
        TLOGERROR("[RemoteNotify::report unknown error" << "]" << endl);
    }
}

void RemoteNotify::notify(NOTIFYLEVEL level, const string &sMessage)
{
    try
    {
        if(_notifyPrx)
        {
            ReportInfo info;
            info.eType     = NOTIFY;
            info.sApp      = _app;
            info.sServer   = _serverName;
            info.sSet      = _setName;
            info.sThreadId = TC_Common::tostr(std::this_thread::get_id());
            info.sMessage  = sMessage;
            info.eLevel    = level;
            info.sNodeName = _nodeName;
            _notifyPrx->async_reportNotifyInfo(NULL, info);
        }
    }
    catch(exception &ex)
    {
        TLOGERROR("[RemoteNotify::notify error:" << ex.what() << "]" << endl);
    }
    catch(...)
    {
        TLOGERROR("[RemoteNotify::notify unknown error" << "]" << endl);
    }
}

void RemoteNotify::report(const string &sMessage, const string & app, const string &serverName, const string &sNodeName)
{
    try
    {
        if(_notifyPrx)
        {
            ReportInfo info;
            info.eType     = REPORT;
            info.sApp      = app;
            info.sServer   = serverName;
            info.sSet      = "";
            info.sMessage  = sMessage;
            info.sNodeName = sNodeName;
            _notifyPrx->async_reportNotifyInfo(NULL, info);
        }
    }
    catch(exception &ex)
    {
        TLOGERROR("[RemoteNotify::notify error:" << ex.what() << "]" << endl);
    }
    catch(...)
    {
        TLOGERROR("[RemoteNotify::notify unknown error" << "]" << endl);
    }
}

}


