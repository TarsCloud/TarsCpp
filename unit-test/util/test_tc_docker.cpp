//
// Created by jarod on 2021/8/09.
//

#include "util/tc_docker.h"
#include "gtest/gtest.h"
#include "../server/Hello.h"

using namespace tars;
using namespace Test;

class DockerTest : public testing::Test
{
public:
    //添加日志
    static void SetUpTestCase()
    {
//        cout<<"SetUpTestCase"<<endl;
    }
    static void TearDownTestCase()
    {
//        cout<<"TearDownTestCase"<<endl;
    }
    virtual void SetUp()   //TEST跑之前会执行SetUp
    {
//        cout<<"SetUp"<<endl;
    }
    virtual void TearDown() //TEST跑完之后会执行TearDown
    {
//        cout<<"TearDown"<<endl;
    }

};

//TEST_F(DockerTest, login)
//{
//	TC_Docker docker;
//
//	bool succ = docker.login("", "", "");
//	ASSERT_TRUE(succ);
//
//	LOG_CONSOLE_DEBUG << "login:" << succ << ", " << (succ?docker.getStdoutMessage() : docker.getStderrMessage()) << endl;
//}

TEST_F(DockerTest, pull)
{
	TC_Docker docker;

//	docker.setAuthentication("ruanshudong", "", "");

	bool succ = docker.pull("docker.tarsyun.com/tarscloud/tars.nodejsbase:latest");
	ASSERT_TRUE(succ);

	LOG_CONSOLE_DEBUG << "pull:" << succ << ", " << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;
}

TEST_F(DockerTest, pullNotExists)
{
	TC_Docker docker;

	bool succ = docker.pull("docker.tarsyun.com/test:latest");
	ASSERT_FALSE(succ);

	LOG_CONSOLE_DEBUG << "pull:" << succ << ", " << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;
}

TEST_F(DockerTest, createAndStart)
{
	bool succ;

	TC_Docker docker;

//	docker.setAuthentication("ruanshudong", "", "");

	succ = docker.pull("docker.tarsyun.com/tarscloud/tars.nodejsbase:latest");
	LOG_CONSOLE_DEBUG << "pull:" << succ << ", "
					  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;
	ASSERT_TRUE(succ);
	docker.remove("test", true);

	map<string, string> mounts;
	mounts["/"] = "/data";

	map<string, pair<string, int>> ports;
	ports["80/tcp"] = make_pair("0.0.0.0", 80);
	ports["8080/tcp"] = make_pair("127.0.0.1", 8080);

	vector<string> entrypoints;
	entrypoints.push_back("sleep");
	entrypoints.push_back("100");

	succ = docker.create("test", "docker.tarsyun.com/tarscloud/tars.nodejsbase:latest", entrypoints, {},{}, mounts, ports, "", 0, "host", "none", true, false);
	ASSERT_TRUE(succ);

	LOG_CONSOLE_DEBUG << "run:" << succ << ", " << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

	if(succ)
	{
		succ = docker.start("test");

		ASSERT_TRUE(succ);

	}
}


TEST_F(DockerTest, stop)
{
	bool succ;
	{

		TC_Docker docker;

//		docker.setAuthentication("ruanshudong", "", "");

		docker.remove("test", true);

		succ = docker.pull("docker.tarsyun.com/tarscloud/tars.nodejsbase:latest");
		LOG_CONSOLE_DEBUG << "pull:" << succ << ", "
						  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;
		ASSERT_TRUE(succ);

		vector<string> entrypoints;
		entrypoints.push_back("sleep");
		entrypoints.push_back("100");

		succ = docker.create("test", "docker.tarsyun.com/tarscloud/tars.nodejsbase:latest", entrypoints, {}, {}, {}, {}, "", 0,
				"host", "none", true, false);
		LOG_CONSOLE_DEBUG << "create:" << succ << ", "
						  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

		ASSERT_TRUE(succ);

		succ = docker.start("test");
		LOG_CONSOLE_DEBUG << "start:" << succ << ", "
						  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

		ASSERT_TRUE(succ);
	}

	{
		TC_Docker docker;
		succ = docker.stop("test", 10);
		LOG_CONSOLE_DEBUG << "stop:" << succ << ", "
						  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

		ASSERT_TRUE(succ);
	}
}

TEST_F(DockerTest, inspectContainer)
{
	TC_Docker docker;

	docker.setAuthentication("ruanshudong", "", "");

	docker.remove("test", true);

	vector<string> entrypoints;
	entrypoints.push_back("sleep");
	entrypoints.push_back("100");

	bool succ = docker.create("test", "tarscloud/tars.nodejsbase:latest", entrypoints, {},{}, {}, {}, "", 0, "host", "none", true, false);
	LOG_CONSOLE_DEBUG << "inspectContainer:" << succ << ", " << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

	ASSERT_TRUE(succ);

	succ = docker.inspectContainer("test");

	LOG_CONSOLE_DEBUG << "inspectContainer:" << succ << ", " << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;
	ASSERT_TRUE(succ);

}


TEST_F(DockerTest, inspectContainerNotExists)
{
	TC_Docker docker;

	docker.remove("test", true);

	bool succ = docker.inspectContainer("test");

	LOG_CONSOLE_DEBUG << "inspectContainer:" << succ << ", " << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;
	ASSERT_FALSE(succ);

}


TEST_F(DockerTest, start)
{
	TC_Docker docker;

	docker.remove("test", true);

	vector<string> entrypoints;
	entrypoints.push_back("sleep");
	entrypoints.push_back("100");
	bool succ;

	succ = docker.pull("docker.tarsyun.com/tarscloud/tars.nodejsbase:latest");
	LOG_CONSOLE_DEBUG << "pull:" << succ << ", "
					  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

	succ = docker.create("test", "docker.tarsyun.com/tarscloud/tars.nodejsbase:latest", entrypoints, {}, {}, {}, {}, "", 0,
			"", "none", false, false);
	LOG_CONSOLE_DEBUG << "create:" << succ << ", "
					  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

	succ = docker.start("test");
	LOG_CONSOLE_DEBUG << "start:" << succ << ", "
					  << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

	ASSERT_TRUE(succ);
}

TEST_F(DockerTest, inspectImage)
{
	TC_Docker docker;

	docker.setDockerUnixLocal("/var/run/docker.sock");

	bool succ = docker.pull("docker.tarsyun.com/tarscloud/tars.nodejsbase:latest");

	if(!succ)
	{
		LOG_CONSOLE_DEBUG << docker.getErrMessage() << endl;
	}

	ASSERT_TRUE(succ);

	{
		bool succ = docker.inspectImage("docker.tarsyun.com/tarscloud/tars.nodejsbase:latest");
		ASSERT_TRUE(succ);
		LOG_CONSOLE_DEBUG << "inspectImage:" << succ << ", " << (succ ? docker.getResponseMessage() : docker.getErrMessage()) << endl;

		JsonValueObjPtr oPtr = JsonValueObjPtr::dynamicCast(TC_Json::getValue(docker.getResponseMessage()));

		string sha = JsonValueStringPtr::dynamicCast(oPtr->value["Id"])->value;

		LOG_CONSOLE_DEBUG << sha << endl;
	}

}
