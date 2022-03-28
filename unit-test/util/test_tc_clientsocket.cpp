#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilClientSocketTest : public testing::Test
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


TEST_F(UtilClientSocketTest, testParseEndpoint)
{
	TC_Endpoint ep;
	ep.parse("tcp -h 127.0.0.1 -p 17890 -t 3000:");

	ASSERT_TRUE(ep.isTcp());
	ASSERT_TRUE(ep.getHost() == "127.0.0.1");
	ASSERT_TRUE(ep.getPort() == 3000);
}

TEST_F(UtilClientSocketTest, testSepEndpointOK)
{
	vector<string> v = TC_Endpoint::sepEndpoint("tcp -h 127.0.0.1 -p 17890 -t 3000:tcp -h 127.0.0.1 -p 17890 -t 3001");

	LOG_CONSOLE_DEBUG << v.size() << ", " << TC_Common::tostr(v.begin(), v.end(), ", ") << endl;

	ASSERT_TRUE(v.size() == 2);
	ASSERT_TRUE(v[0] == "tcp -h 127.0.0.1 -p 17890 -t 3000");
	ASSERT_TRUE(v[1] == "tcp -h 127.0.0.1 -p 17890 -t 3001");
}


TEST_F(UtilClientSocketTest, testSepEndpoint1)
{
	vector<string> v = TC_Endpoint::sepEndpoint("tcp -h 127.0.0.1 -p 17890 -t 3000:");

//	LOG_CONSOLE_DEBUG << v.size() << ", " << TC_Common::tostr(v.begin(), v.end(), ", ") << endl;

	ASSERT_TRUE(v.size() == 1);
	ASSERT_TRUE(v[0] == "tcp -h 127.0.0.1 -p 17890 -t 3000");
}



TEST_F(UtilClientSocketTest, testSepEndpoint2)
{
	vector<string> v = TC_Endpoint::sepEndpoint("tcp -h 127.0.0.1 -p 17890 -t 3000:tcp -h 127.0.0.1 -p 17890 -t 3001:");

//	LOG_CONSOLE_DEBUG << v.size() << ", " << TC_Common::tostr(v.begin(), v.end(), ", ") << endl;

	ASSERT_TRUE(v.size() == 2);
	ASSERT_TRUE(v[0] == "tcp -h 127.0.0.1 -p 17890 -t 3000");
	ASSERT_TRUE(v[1] == "tcp -h 127.0.0.1 -p 17890 -t 3001");
}

