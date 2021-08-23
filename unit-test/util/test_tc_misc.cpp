//
// Created by jarod on 2021/8/09.
//

#include "util/tc_json.h"
#include "gtest/gtest.h"

using namespace tars;

class UtilTest : public testing::Test
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

TEST_F(UtilTest, json)
{
   	int64_t point = 0x100000000000001;
//   	cout << "point=" << point << endl;
//   	double x = point;
//   	int64_t point2 = x;
//	cout << "point=" << point2 << endl;
//	cout << std::to_string(x) << endl;

	string buff = "{ \"x\": 72057594037927937}";

	auto d = JsonValueObjPtr::dynamicCast(TC_Json::getValue(buff));

	auto v = JsonValueNumPtr ::dynamicCast(d->get("x"));

//	cout << v->isInt << ", " << v->lvalue << ", " << (int64_t)v->value << endl;

	ASSERT_TRUE(v->isInt);
	ASSERT_TRUE(v->lvalue == point);

	//double精度有问题!!!
	ASSERT_TRUE((int64_t)v->value != point);
}


