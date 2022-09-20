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

#include <iostream>
#include "servant/Communicator.h"
#include "util/tc_option.h"
#include "AdminReg.h"

using namespace std;
using namespace tars;


/**
 * 测试ip list更新以及tarsnode缓存的逻辑, 需要有一套完整的Tars环境, 部署主从的环境以便测试
 * 为了方便起见, 使用tarsproperty服务来测试
 * 主要逻辑如下:
 * - 直连主控
 * - 获取tarsproperty的ip list, 并访问
 * - 调用tarsAdminRegistry接口, 关闭其中一个服务
 * - 收到ip list回调通知, 且活跃节点减少1个
 * - 调用tarsAdminRegistry接口, 启动其中一个服务
 * - 收到ip list回调通知, 且活跃节点增加1个
 * - 连接tarsnode, 得到的逻辑相同
 */
Communicator* _comm;

void output(const string &s, const vector<EndpointInfo> &endPoints)
{
	LOG_CONSOLE_DEBUG << s << " size:" << endPoints.size() << endl;
	for(auto &ep : endPoints)
	{
		LOG_CONSOLE_DEBUG << ep.desc() << endl;
	}
}
int main(int argc, char *argv[])
{
    try
    {
		TC_Option option;
		option.decode(argc, argv);

		string locator = option.getValue("locator");
		if(locator.empty())
		{
			locator = "tars.tarsregistry.QueryObj@tcp -h 10.211.55.12 -p 17890";
		}
		_comm = new Communicator();
		_comm->setProperty("locator", locator);

		LocalRollLogger::getInstance()->logger()->setLogLevel("TARS");

		ServantPrx prx = _comm->stringToProxy<ServantPrx>("tars.tarsqueryproperty.QueryObj");
		prx->tars_ping();

		vector<EndpointInfo> activeEndPoint;
		vector<EndpointInfo> inactiveEndPoint;
		prx->tars_endpoints(activeEndPoint, inactiveEndPoint);

		if(activeEndPoint.empty())
		{
			LOG_CONSOLE_DEBUG << "no active endpoints" << endl;
			return -1;
		}

		output("active", activeEndPoint);
		output("inactive", inactiveEndPoint);

//		return 0;

//		string result;
//		AdminRegPrx adminPrx = _comm->stringToProxy<AdminRegPrx>("tars.tarsAdminRegistry.AdminRegObj");
//		adminPrx->stopServer("tars", "tarsqueryproperty", activeEndPoint[0].host(), result);
//
//		LOG_CONSOLE_DEBUG << result << endl;

		while(true)
		{
			TC_Common::sleep(1);

			activeEndPoint.clear();
			inactiveEndPoint.clear();

			prx->tars_endpoints(activeEndPoint, inactiveEndPoint);
			output("active", activeEndPoint);
			output("inactive", inactiveEndPoint);

			TC_Common::sleep(1);

			activeEndPoint.clear();
			inactiveEndPoint.clear();

			prx->tars_endpoints(activeEndPoint, inactiveEndPoint);
			output("active", activeEndPoint);
			output("inactive", inactiveEndPoint);

		}
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
    cout << "main return." << endl;

    return 0;
}
