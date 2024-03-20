#include "util/tc_common.h"
#include "util/tc_socket.h"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilSocketTest : public testing::Test
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

TEST_F(UtilSocketTest, testGetLocalHosts)
{
    vector<string> v1 = TC_Socket::getLocalHosts(AF_INET);

    for(size_t i = 0; i < v1.size(); i++)
    {
        cout << v1[i] << endl;
    }

    vector<string> v2 = TC_Socket::getLocalHosts(AF_INET6);

    for(size_t i = 0; i < v2.size(); i++)
    {
        cout << v2[i] << endl;
    }
}

