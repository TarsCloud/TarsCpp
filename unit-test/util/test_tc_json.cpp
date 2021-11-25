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

    JsonData createJsonValue()
    {
    	JsonData jValue;
    	jValue.c = '4';
    	jValue.s = -43;
    	jValue.i = -423323;
    	jValue.l = -42342344;
    	jValue.f = 42234.23f;
    	jValue.d = 42.4232f;
    	jValue.uc = '6';
    	jValue.us = 49023;
    	jValue.ui = 90239238;
    	jValue.b = true;
    	jValue.k = KEY1;
    	jValue.ss = "abc";

    	jValue.data[KEY1] = "key1";
    	jValue.data[KEY2] = "key2";

    	jValue.v.push_back(KEY1);
    	jValue.v.push_back(KEY2);

    	jValue.im[1] = "abc";
		jValue.bm[true] = "abc";
		jValue.fm[4.53f] = "abc";

		jValue.bm[true]="abc";
		jValue.iv.push_back(333);
		jValue.dv.push_back(323.323);

    	return jValue;
    }

    JsonMap createJsonMap()
    {
    	JsonKey k;
    	k.i = 10;

    	JsonMap jm;
    	jm.json[k] = createJsonValue();

    	return jm;
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

TEST_F(JsonTest, jsonStruct)
{
	JsonData jValue = createJsonValue();

    string v = jValue.writeToJsonString();

    cout << "json1:" << v << endl;

    JsonData jMap2;
    jMap2.readFromJsonString(v);

    cout << "json2:" << jMap2.writeToJsonString() << endl;
    ASSERT_TRUE(jValue == jMap2);
}


TEST_F(JsonTest, jsonMap)
{
	JsonMap jMap = createJsonMap();

	string v = jMap.writeToJsonString();

	cout << "json1:" << v << endl;

	JsonMap jMap2;
	jMap2.readFromJsonString(v);

	cout << "json2:" << jMap2.writeToJsonString() << endl;
	ASSERT_TRUE(jMap == jMap2);
}