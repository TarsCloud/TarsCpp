#include "util/tc_timer.h"
#include "util/tc_common.h"
#include "util/tc_cron.h"
#include "util/tc_epoller.h"
#include <cmath>
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilTimerTest : public testing::Test
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
    void test1()
    {
	    _data.push_back(std::make_pair(TNOWMS, 0));
    }

    void test2(int64_t i)
    {
	    _data.push_back(std::make_pair(TNOWMS, i));
    }

    void test3(int i)
    {
	    _data.push_back(std::make_pair(TNOWMS, i));
    }

    void testCron()
    {
	    _data.push_back(std::make_pair(TNOWMS, 0));
    }

    void testCron2()
    {
	    _data.push_back(std::make_pair(TNOWMS, 0));
    }

    vector<pair<int64_t, int64_t>> _data;
};


TEST_F(UtilTimerTest, testDelayTask1)
{
    TC_Timer timer;
    timer.startTimer(1);
    shared_ptr<TestClass> tPtr = std::make_shared<TestClass>();

    timer.postDelayed(100, std::bind(&TestClass::test2, tPtr.get(), std::placeholders::_1), 111);
    TC_Common::msleep(106);
	ASSERT_TRUE(tPtr->_data.size() == 1);

    timer.stopTimer();
}

TEST_F(UtilTimerTest, testDelayTask2)
{
	TC_Timer timer;
	timer.startTimer(1);
	shared_ptr<TestClass> tPtr = std::make_shared<TestClass>();

	uint64_t id2 = timer.postDelayed(100, std::bind(&TestClass::test2, tPtr.get(), std::placeholders::_1), 111);
	TC_Common::msleep(5);

	ASSERT_TRUE(tPtr->_data.size() == 0);

	timer.erase(id2);

	timer.stopTimer();
}

TEST_F(UtilTimerTest, testRepeatTask)
{
    TC_Timer timer;
    shared_ptr<TestClass> tPtr = std::make_shared<TestClass>();
    timer.startTimer(1);
	uint64_t id2 = timer.postRepeated(50, false, std::bind(&TestClass::test1, tPtr.get()));

    TC_Common::msleep(1080);

	//由于精度原因, 有一定误差
	cout << tPtr->_data.size() << endl;
	ASSERT_TRUE(tPtr->_data.size() >= 20);
	ASSERT_TRUE(tPtr->_data.size() <= 21);

	for (int i = 0; i < 9; i++)
	{
		int diff = fabs(tPtr->_data[i + 1].first - tPtr->_data[i].first);
		ASSERT_TRUE(diff <= 56);
	}

	timer.erase(id2);

	TC_Common::msleep(100);

	ASSERT_TRUE(tPtr->_data.size() >= 20);
	ASSERT_TRUE(tPtr->_data.size() <= 21);

	timer.stopTimer();

}

struct AAA
{
    AAA() {  };
    ~AAA() { };
    void test() {}
    int xxx = 100;
};

void fff(const std::shared_ptr<AAA>& ppp)
{
	ppp->xxx++;
//	cout << TC_Common::now2ms() << ", " << ppp->xxx << endl;
}

TEST_F(UtilTimerTest, testTimerMem)
{
    TC_Timer timer;
    timer.startTimer(1);

    std::shared_ptr<AAA> pa = std::make_shared<AAA>();

    //right
	{
		auto func = [pa]
		{
			fff(pa);
		};
		int xxx = pa->xxx;
		timer.postDelayed(100, func);
		TC_Common::msleep(120);
		ASSERT_TRUE(pa->xxx - xxx == 1);
	}

    //right
    {
        auto funcb = std::bind(fff, pa);
	    int xxx = pa->xxx;
        timer.postDelayed(100, funcb);
	    TC_Common::msleep(120);
	    ASSERT_TRUE(pa->xxx - xxx == 1);
    }

    //right
    {
	    int xxx = pa->xxx;
        timer.postDelayed(100, fff, pa);
	    TC_Common::msleep(120);
	    ASSERT_TRUE(pa->xxx - xxx == 1);
    }

    //right
    {
	    int xxx = pa->xxx;
        timer.postDelayed(100, std::bind(fff, std::ref(pa)));
	    TC_Common::msleep(120);
	    ASSERT_TRUE(pa->xxx - xxx == 1);
    }
    //wrong, unkown reason
//    if (1)
    {
	    int xxx = pa->xxx;
        timer.postDelayed(100, std::bind(fff, pa));
	    TC_Common::msleep(120);
	    ASSERT_TRUE(pa->xxx - xxx == 1);
    }

	timer.stopTimer();
}

TEST_F(UtilTimerTest, testTimerRepeatMem)
{
    TC_Timer timer;
    timer.startTimer(1);

    std::shared_ptr<AAA> pa = std::make_shared<AAA>();

    int xxx = pa->xxx;

    auto func = [pa] {
        fff(pa);
    };
    int64_t taskId = timer.postRepeated(50, false, func);

    TC_Common::msleep(1020);

    //注意timer有精度的问题, 精度只能到5ms, 所以这里有一定的误差!
	int diff = pa->xxx - xxx;
	cout << diff << endl;
	ASSERT_TRUE(diff >= 19);
	ASSERT_TRUE(diff <= 20);

	timer.erase(taskId);

    TC_Common::msleep(1000);

	diff = pa->xxx - xxx;
	ASSERT_TRUE(diff >= 19);
	ASSERT_TRUE(diff <= 20);

	timer.stopTimer();
}

TEST_F(UtilTimerTest, testRepeatUseCount)
{
	TC_Timer timer;
	timer.startTimer(1);

	std::shared_ptr<AAA> pa = std::make_shared<AAA>();

	ASSERT_TRUE(pa.use_count() == 1);
	int64_t taskId = timer.postRepeated(50, false, std::bind(&AAA::test, pa));
	ASSERT_TRUE(pa.use_count() == 2);

	timer.erase(taskId);

	ASSERT_TRUE(pa.use_count() == 1);

	timer.postRepeated(50, false, std::bind(&AAA::test, pa));
	ASSERT_TRUE(pa.use_count() == 2);

	timer.clear();
	ASSERT_TRUE(pa.use_count() == 1);

	timer.stopTimer();
}


