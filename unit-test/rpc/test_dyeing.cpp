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

#include "hello_test.h"
#include "servant/AdminF.h"
#include "server/FrameworkServer.h"
#include "server/framework/DbHandle.h"

#define DYEFILEPATH (string("tars_dyeing") + FILE_SEP)

//被动染色测试，对接口调用进行染色。被动染色分为两步：
//1.通过管理接口设置染色字段的key值
//2.通过key值调用接口

#define N 1024

int getFileLine(const char *fileName)
{
	ifstream infile(fileName);
	char str[N];
	int n=0;
	while(!infile.eof())
	{
		infile.getline(str, sizeof(str));
		string s = TC_Common::trim(str);
		if(!s.empty())
		{
			n++;
		}
	}

	return n;
}

//未打开染色开关的场景
TEST_F(HelloTest, DyeingNo)
{

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

	HelloPrx prx = getObj<HelloPrx>(server.getCommunicator().get(), "HelloAdapter");

//	int64_t tBegin = TC_TimeProvider::getInstance()->getNowMs();

//	HelloPrx dyeingPrx= server.getCommunicator()->stringToProxy<HelloPrx>(DYEING_SERVANT_ENDPOINT);
	string strIn="123456";
	string strOut;
	int ret=prx->testDyeing(strIn,strOut);
	EXPECT_EQ(ret,0);

//	TLOGDEBUG("no dyeing request time cost: "<< " | " << TC_TimeProvider::getInstance()->getNowMs() - tBegin << "(ms)" << endl);

	stopServer(server);
}

//打开染色开关，但未使用染色key调用的场景
TEST_F(HelloTest, DyeingNoKey)
{
	FrameworkServer fs;
	startServer(fs, FRAMEWORK_CONFIG());

	CDbHandle::cleanEndPoint();
	CDbHandle::addActiveEndPoint("TestApp.FrameworkServer.LogObj", 11005, 1);

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

//    int64_t tBegin = TC_TimeProvider::getInstance()->getNowMs();

	AdminFPrx adminFPrx = server.getCommunicator()->stringToProxy<AdminFPrx>("AdminObj@" + getLocalEndpoint(CONFIG()).toString());

//	AdminFPrx adminFPrx = server.getCommunicator()->stringToProxy<AdminFPrx>(UNIT_TEST_ADMIN_NAME_ENDPOINT);
	string setdyeing = adminFPrx->notify("tars.setdyeing 123456 TestApp.HelloServer.HelloObj testDyeing");
	EXPECT_TRUE(setdyeing.find("DyeingKey=123456") != string::npos);

	HelloPrx prx = getObj<HelloPrx>(server.getCommunicator().get(), "HelloAdapter");

//	DyeingTestPrx dyeingPrx= server.getCommunicator()->stringToProxy<DyeingTestPrx>(DYEING_SERVANT_ENDPOINT);
    string strIn="abc";
	string strOut;
	int ret=prx->testDyeing(strIn,strOut);
	TC_Common::sleep(1);
	EXPECT_EQ(ret,0);

//	TLOGDEBUG("dyeing without key request time cost: "<< " | " << TC_TimeProvider::getInstance()->getNowMs() - tBegin << "(ms)" << endl);

	stopServer(server);
	stopServer(fs);

}
//打开染色开关，使用染色key调用的场景
TEST_F(HelloTest, DyeingKey)
{
	FrameworkServer fs;
	startServer(fs, FRAMEWORK_CONFIG());

	CDbHandle::cleanEndPoint();
	CDbHandle::addActiveEndPoint("TestApp.FrameworkServer.LogObj", 11005, 1);

	HelloServer server;
	startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

	AdminFPrx adminFPrx = server.getCommunicator()->stringToProxy<AdminFPrx>("AdminObj@" + getLocalEndpoint(CONFIG()).toString());

	string setdyeing = adminFPrx->notify("tars.setdyeing 123456 TestApp.HelloServer.HelloObj testDyeing");
	EXPECT_TRUE(setdyeing.find("DyeingKey=123456") != string::npos);

	TC_File::removeFile("./tars_dyeing", true);

	HelloPrx prx = getObj<HelloPrx>(server.getCommunicator().get(), "HelloAdapter");

    string strIn="123456";
	string strOut;
	int ret=prx->testDyeing(strIn,strOut);
	TC_Common::sleep(1);
	EXPECT_EQ(ret,0);

	string dyeDayFile=DYEFILEPATH + "dyeing_" + TC_Common::nowdate2str() + ".log";
	EXPECT_EQ(TC_File::isFileExist(dyeDayFile), true);
	EXPECT_EQ(getFileLine(dyeDayFile.c_str()), 4);

	string dyeDebugFile=DYEFILEPATH + "dyeing.log";
	EXPECT_EQ(TC_File::isFileExist(dyeDebugFile), true);
	EXPECT_EQ(getFileLine(dyeDebugFile.c_str()), 4);

	stopServer(server);
	stopServer(fs);
}



