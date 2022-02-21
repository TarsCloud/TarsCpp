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
