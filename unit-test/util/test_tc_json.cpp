//
// Created by jarod on 2021/8/09.
//

#include "util/tc_json.h"
#include "gtest/gtest.h"
#include "../server/Hello.h"

using namespace tars;
using namespace Test;

class JsonTest : public testing::Test
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

    JsonMap create()
    {
    	JsonMap jMap;
    	jMap.c = '4';
    	jMap.s = -43;
    	jMap.i = -423323;
    	jMap.l = -42342344;
    	jMap.f = 42234.23f;
    	jMap.d = 42.4232f;
    	jMap.uc = '6';
    	jMap.us = 49023;
    	jMap.ui = 90239238;
    	jMap.b = true;
    	jMap.k = KEY1;
    	jMap.ss = "abc";

    	jMap.data[KEY1] = "key1";
    	jMap.data[KEY2] = "key2";

    	jMap.v.push_back(KEY1);
    	jMap.v.push_back(KEY2);

    	jMap.im[1] = "abc";
		jMap.bm[true] = "abc";
		jMap.fm[4.53f] = "abc";

		jMap.bm[true]="abc";
		jMap.iv.push_back(333);
		jMap.dv.push_back(323.323);

    	return jMap;
    }
};

TEST_F(JsonTest, json)
{
   	int64_t point = 0x100000000000001;

	string buff = "{ \"x\": 72057594037927937}";

	auto d = JsonValueObjPtr::dynamicCast(TC_Json::getValue(buff));

	auto v = JsonValueNumPtr ::dynamicCast(d->get("x"));

	ASSERT_TRUE(v->isInt);
	ASSERT_TRUE(v->lvalue == point);

	//double精度有问题!!!
	ASSERT_TRUE((int64_t)v->value != point);
}

TEST_F(JsonTest, struct)
{
	JsonMap jMap = create();

    string v = jMap.writeToJsonString();

    cout << "json1:" << v << endl;

    JsonMap jMap2;
    jMap2.readFromJsonString(v);

    cout << "json2:" << jMap2.writeToJsonString() << endl;
    ASSERT_TRUE(jMap == jMap2);
}