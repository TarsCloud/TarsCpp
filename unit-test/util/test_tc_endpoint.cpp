//
// Created by jarod on 2020/2/20.
//

#include "util/tc_clientsocket.h"
#include "gtest/gtest.h"

using namespace tars;

class UtilEndpointTest : public testing::Test
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

TEST_F(UtilEndpointTest, sep)
{
    string str = "udp -h ::1 -p 25460 -t 60000";

    vector<string> eps = TC_Endpoint::sepEndpoint(str);

    EXPECT_EQ(eps.size(), 1);

    EXPECT_EQ(eps[0], str);

    str = "tcp -h ::1 -p 25460 -t 60000";

    eps = TC_Endpoint::sepEndpoint(str);

    EXPECT_EQ(eps.size(), 1);

    EXPECT_EQ(eps[0], str);

    str = "ssl -h ::1 -p 25460 -t 60000";

    eps = TC_Endpoint::sepEndpoint(str);

    EXPECT_EQ(eps.size(), 1);

    EXPECT_EQ(eps[0], str);

    str = " tcp -h ::1 -p 25460 -t 60000 ";

    eps = TC_Endpoint::sepEndpoint(str);

    EXPECT_EQ(eps.size(), 1);

    EXPECT_EQ(eps[0], TC_Common::trim(str));

    str = " tcp -h ::1 -p 25460 -t 60000: ";

    eps = TC_Endpoint::sepEndpoint(str);

    EXPECT_EQ(eps.size(), 1);

    EXPECT_EQ(eps[0], TC_Common::trim("tcp -h ::1 -p 25460 -t 60000"));
}


TEST_F(UtilEndpointTest, seps)
{
    string str = " udp -h ::1 -p 25460 -t 60000 : tcp -h 127.0.0.1 -p 25460 -t 60000 : ssl -h ::1 -p 25460 -t 60000: ";

    vector<string> eps = TC_Endpoint::sepEndpoint(str);

    EXPECT_EQ(eps.size(), 3);

    EXPECT_EQ(eps[0], "udp -h ::1 -p 25460 -t 60000");
    EXPECT_EQ(eps[1], "tcp -h 127.0.0.1 -p 25460 -t 60000");
    EXPECT_EQ(eps[2], "ssl -h ::1 -p 25460 -t 60000");

}