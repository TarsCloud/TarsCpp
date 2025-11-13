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

TEST_F(UtilThreadPollTest, testThreadCallbacks)
{
	std::atomic<int> initCount(0);
	std::atomic<int> destroyCount(0);
	
	TC_ThreadPool pool;
	pool.init(4);
	
	pool.setThreadInitCallback([&initCount](){
		initCount++;
	});
	
	pool.setThreadDestroyCallback([&destroyCount](){
		destroyCount++;
	});
	
	pool.start();
	
	// 执行一些任务
	for (int i = 0; i < 10; ++i)
	{
		pool.exec([](){ 
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		});
	}
	
	pool.waitForAllDone();
	pool.stop();
	
	// 验证回调被调用了正确的次数
	EXPECT_EQ(initCount.load(), 4);
	EXPECT_EQ(destroyCount.load(), 4);
}

TEST_F(UtilThreadPollTest, testThreadLocalStorage)
{
	std::mutex mtx;
	std::set<int> values;
	
	TC_ThreadPool pool;
	pool.init(4);
	
	pool.setThreadInitCallback([](){
		static std::atomic<int> counter(1000);
		thread_local int g_value = 0;
		g_value = counter.fetch_add(1);
	});
	
	pool.start();
	
	for (int i = 0; i < 100; ++i)
	{
		pool.exec([&mtx, &values](){
			static thread_local int g_value = 0;
			std::lock_guard<std::mutex> lock(mtx);
			if (g_value != 0) {
				values.insert(g_value);
			}
		});
	}
	
	pool.waitForAllDone();
	pool.stop();
}

TEST_F(UtilThreadPollTest, testCallbackException)
{
	std::atomic<int> taskCount(0);
	
	TC_ThreadPool pool;
	pool.init(2);
	
	// 初始化回调抛出异常
	pool.setThreadInitCallback([](){
		throw std::runtime_error("Init error");
	});
	
	pool.start();
	
	// 任务应该仍然能够执行
	for (int i = 0; i < 10; ++i)
	{
		pool.exec([&taskCount](){ 
			taskCount++;
		});
	}
	
	pool.waitForAllDone();
	pool.stop();
	
	// 验证任务都被执行了
	EXPECT_EQ(taskCount.load(), 10);
}

TEST_F(UtilThreadPollTest, testSetCallbackAfterStart)
{
	TC_ThreadPool pool;
	pool.init(2);
	pool.start();
	
	// 应该抛出异常
	EXPECT_THROW(
		pool.setThreadInitCallback([](){}),
		TC_ThreadPool_Exception
	);
	
	EXPECT_THROW(
		pool.setThreadDestroyCallback([](){}),
		TC_ThreadPool_Exception
	);
	
	pool.stop();
}