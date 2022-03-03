#include "util/tc_common.h"
#include "util/tc_cas_queue.h"
#include "util/tc_thread.h"
#include "util/tc_autoptr.h"
#include "util/tc_spin_lock.h"
#include "util/tc_thread_queue.h"
#include "gtest/gtest.h"
#include "servant/RemoteLogger.h"
#include <mutex>
#include <iostream>

using namespace tars;

class UtilLogTest : public testing::Test
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

TEST_F(UtilLogTest, fdLog)
{
	RemoteTimeLogger::getInstance()->initFormat("./test.log", "%Y%m%d");

	string s= "[{buy_unit:1000,down_price:0,exchange_type:1,high_amount:16000,issue_date:20220224,last_price:42.14,low_amount:1000,money_type:0,par_value:1,position_str:0001732102,price_step:10,relative_code:603102,stkcode_ctrlstr:000000000000\n"
					"    000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,stkcode_\n"
					"    status:0,stock_code:732102,stock_name:?ٺ??깺,stock_name_long:,stock_type:4,store_unit:1,sub_stock_type:41,up_price:0}";

	int i = 10000000;
	while(i > 0)
	{
		FDLOG("abc") << s << endl;

		i--;
	}

}