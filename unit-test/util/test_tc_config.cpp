#include "util/tc_config.h"
#include "util/tc_common.h"
#include <cmath>
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilConfigTest : public testing::Test
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

#define CONFIG "<root>\r\n</root>"


TEST_F(UtilConfigTest, config_test)
{
	TC_Config conf;
	conf.parseString(CONFIG);

	conf.set("/root<abc>", "def");

	ASSERT_TRUE(conf.get("/root<abc>") == "def");

	conf.set("/root/test<abc>", "defdef");

	ASSERT_TRUE(conf.get("/root/test<abc>") == "defdef");

	conf.erase("/root/test<abc>");

	ASSERT_TRUE(conf.get("/root/test<abc>") == "");

	conf.eraseDomain("/root/test");

	auto v =conf.getDomainVector("/root");
	ASSERT_TRUE(v.empty());

}
