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
#include "Hello.h"
#include "Push.h"
#include "util/tc_option.h"

using namespace std;
using namespace tars;
using namespace TestApp;

Communicator* _comm;

static string helloObj = "TestApp.PushCallbackServer.PushObj@tcp -h 127.0.0.1 -p 9316";

int g_count = 0;

class PushCallbackImp : public PushPrxCallback
{
public:
	virtual void callback_pushMsg(tars::Int32 ret,  const std::string& sRsp)
	{
		LOG_CONSOLE_DEBUG << ret << ", " << sRsp << endl;
		++g_count;
	}
};

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 1)
        {
	        cout << "Usage:" << argv[0] << "--config=conf " << endl;

	        return 0;
        }

	    TC_Option option;
        option.decode(argc, argv);

        _comm = new Communicator();

        TC_Config conf;
        conf.parseFile(option.getValue("config"));
	    _comm->setProperty(conf);

	    HelloPrx pPrx = _comm->stringToProxy<HelloPrx>(helloObj);
		pPrx->tars_set_push_callback(new PushCallbackImp());
		pPrx->registerPush();

		while(g_count < 10)
		{
			TC_Common::sleep(1);
		}

    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
    cout << "main return." << endl;

    return 0;
}
