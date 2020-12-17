//
// Created by jarod on 2020/2/20.
//

#include "util/tc_encoder.h"
#include "gtest/gtest.h"
#include "util/tc_common.h"

using namespace tars;

class EncodeTest : public testing::Test
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

TEST_F(EncodeTest, encode)
{
	string utf8="我们的祖国;";
	string gbk;

	gbk = TC_Encoder::utf82gbk(utf8);//, TC_Encoder::ICONV_NORMAL);

	string tmpUtf8 = TC_Encoder::gbk2utf8(gbk);//, TC_Encoder::ICONV_NORMAL);

	ASSERT_TRUE(utf8 == tmpUtf8);
}

