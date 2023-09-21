#include "util/tc_timeout_queue_map.h"
#include "util/tc_common.h"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilTimeoutQueueMapTest : public testing::Test
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

TEST_F(UtilTimeoutQueueMapTest, test)
{
    TC_TimeoutQueueMap<string, string> queueMap;
    queueMap.setTimeout(100);

    queueMap.push("abc1", "def1");
    queueMap.push("abc2", "def2");
    queueMap.push("abc", "def");

    TC_Common::msleep(110);
    queueMap.timeout();

    EXPECT_EQ(queueMap.size(), 0);
    ASSERT_FALSE(queueMap.has("abc"));

    queueMap.push("abc", "def1");
    TC_Common::msleep(50);
    queueMap.push("abc", "def2");
    TC_Common::msleep(50);
    queueMap.push("abc", "def3");
    TC_Common::msleep(50);

    queueMap.timeout();
    ASSERT_TRUE(queueMap.has("abc"));
    EXPECT_EQ(queueMap.get("abc"), "def3");
    EXPECT_EQ(queueMap.size(), 1);

}

