#include "util/tc_port.h"
#include "util/tc_common.h"
#include <cmath>
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilPortTest : public testing::Test
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

TEST_F(UtilPortTest, testExec)
{
	string err;
   	string result = TC_Port::exec("ls '*.txt'", err);
   	cout << result << endl;
}

TEST_F(UtilPortTest, testGetPidMemUsed)
{
    int64_t used = TC_Port::getPidMemUsed(TC_Port::getpid());
    cout << "used:" << used << endl;

    ASSERT_TRUE(used > 0);
}

TEST_F(UtilPortTest, testGetPidStartTime)
{
    int64_t t = TC_Port::getPidStartTime(TC_Port::getpid());
    cout << "testGetPidStartTime:" << t << endl;

    ASSERT_TRUE(t >= time(NULL));
}

TEST_F(UtilPortTest, testGetUPTime)
{
    time_t t = TC_Port::getUPTime();
    cout << "testGetUPTime:" << t/60/60/24 << "day" << endl;
    ASSERT_TRUE(t > 0);
}

TEST_F(UtilPortTest, testGetSystemMemInfo)
{
    int64_t totalSize = 0;
    int64_t availableSize = 0;
    float usedPercent = 0;

    bool flag = TC_Port::getSystemMemInfo(totalSize, availableSize, usedPercent);

    cout << "totalSize: " << totalSize << endl;
    cout << "availableSize: " << availableSize << endl;
    cout << "usedPercent: " << usedPercent << endl;
    ASSERT_TRUE(flag);
    ASSERT_TRUE(totalSize > 0);
    ASSERT_TRUE(availableSize > 0);
    ASSERT_TRUE(usedPercent > 0);
}

TEST_F(UtilPortTest, testGetCPUProcessor)
{
    int n = TC_Port::getCPUProcessor();

    cout << "cpu: " << n << endl;

    ASSERT_TRUE(n > 0);
}

TEST_F(UtilPortTest, testGetDiskInfo)
{
    float usedPercent = 0;
    int64_t availableSize = 0;

#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_LINUX
    TC_Port::getDiskInfo(usedPercent, availableSize, "/");
#else
    TC_Port::getDiskInfo(usedPercent, availableSize, "c:\\");
#endif

    cout << "usedPercent: " << usedPercent << endl;
    cout << "availableSize: " << availableSize << endl;

    ASSERT_TRUE(usedPercent > 0);
    ASSERT_TRUE(availableSize > 0);
}



