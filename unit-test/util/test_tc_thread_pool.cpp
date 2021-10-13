#include "util/tc_timer.h"
#include "util/tc_logger.h"
#include "util/tc_common.h"
#include "util/tc_thread_pool.h"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilThreadPollTest : public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{
	}
	static void TearDownTestCase()
	{
	}
	virtual void SetUp()   //TEST跑之前会执行SetUp
	{
	}
	virtual void TearDown() //TEST跑完之后会执行TearDown
	{
	}
};

class TestClass
{
public:
	~TestClass()
	{
		// LOG_CONSOLE_DEBUG << endl;
	}
	void test(int i)
	{
	    _data.push_back(i);
    }

    vector<int> _data;
};

TEST_F(UtilThreadPollTest, testDecontructor)
{
	TC_ThreadPool tpool;
	tpool.init(5);
    shared_ptr<TestClass> tPtr = std::make_shared<TestClass>();

	for(int i = 0; i < 5; i++)
	{
		tpool.exec(std::bind(&TestClass::test, tPtr.get(), std::placeholders::_1), i);
	}

	tpool.stop();
}