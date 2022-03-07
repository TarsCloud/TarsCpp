#include "util/tc_common.h"
#include "util/tc_cas_queue.h"
#include "util/tc_thread.h"
#include "util/tc_autoptr.h"
#include "util/tc_spin_lock.h"
#include "util/tc_thread_queue.h"
#include "gtest/gtest.h"
#include "servant/RemoteLogger.h"
#include <mutex>
#include <iostream>

using namespace tars;

class UtilLogTest : public testing::Test
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

TEST_F(UtilLogTest, fdLog)
{
	RemoteTimeLogger::getInstance()->initFormat("./test.log", "%Y%m%d");

	string s= "test";

	int i = 10000000;
	while(i > 0)
	{
		FDLOG("abc") << s << endl;

		i--;
	}

}