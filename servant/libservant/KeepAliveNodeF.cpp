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

#include "util/tc_port.h"
#include "servant/KeepAliveNodeF.h"
#include "servant/RemoteLogger.h"
#include "servant/Communicator.h"

namespace tars
{

void KeepAliveNodeFHelper::setNodeInfo(const CommunicatorPtr &comm, const string &obj, const string &app, const string &server)
{
    _comm           = comm;
    if(!obj.empty())
    {
        _nodePrx        = _comm->stringToProxy<ServerFPrx>(obj);
    }

    _si.application = app;
    _si.serverName  = server;
	_si.pid = TC_Port::getpid();
}

void KeepAliveNodeFHelper::keepAlive(const string &adapter)
{
        try
        {
            if(_nodePrx)
            {
                set<string> s;
                {
                    TC_LockT<TC_ThreadMutex> lock(*this);

                    _adapterSet.insert(adapter);

                    //admin心跳来的时候才上报(减小上报次数)
                    if(!adapter.empty() && adapter != "AdminAdapter")
                    {
                        return;
                    }

                    s.swap(_adapterSet);
                }
                ServerInfo si   = _si;
                set<string>::const_iterator it = s.begin();
                while(it != s.end())
                {
                    si.adapter      = *it;
                    _nodePrx->async_keepAlive(NULL,si);
                    ++it;
                }

            }
        }
        catch(exception &ex)
        {
                TLOGERROR("KeepAliveNodeFHelper::keepAlive error:" << ex.what() << endl);
        }
        catch(...)
        {
                TLOGERROR("KeepAliveNodeFHelper::keepAlive unknown error" << endl);
        }
}

void KeepAliveNodeFHelper::keepActiving()
{
    try
    {
        if(_nodePrx)
        {
            _nodePrx->async_keepActiving(NULL, _si);
        }
    }
    catch(exception &ex)
    {
	    TLOGERROR("[KeepAliveNodeFHelper::keepAlive error:" << ex.what() << "]" << endl);
    }
    catch(...)
    {
	    TLOGERROR("[KeepAliveNodeFHelper::keepAlive unknown error]" << endl);
    }
}

void KeepAliveNodeFHelper::reportVersion(const string &version)
{
    try
    {
        if(_nodePrx)
        {
            _nodePrx->async_reportVersion(NULL, _si.application, _si.serverName, version);
        }
    }
    catch(exception &ex)
    {
        TLOGERROR("[KeepAliveNodeFHelper::reportVersion error:" << ex.what() << "]" << endl);
    }
    catch(...)
    {
        TLOGERROR("[KeepAliveNodeFHelper::reportVersion unknown error" << "]" << endl);
    }
}

}

