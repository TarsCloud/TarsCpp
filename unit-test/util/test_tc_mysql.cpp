#include "util/tc_common.h"
#include "util/tc_mysql.h"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilMysqlTest : public testing::Test
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

#ifdef TARS_MYSQL

TEST_F(UtilMysqlTest, test80)
{
    try
    {
        TC_Mysql mysql;

        mysql.init("127.0.0.1", "root", "123456", "", "utf8", 3307, 0);

        mysql.queryRecord("select 1");
    }
    catch(exception &ex)
    {
        cerr << ex.what() << endl;
    }

    try
    {
        TC_Mysql mysql;

        mysql.init("127.0.0.1", "root", "tars@12345", "", "utf8", 3306, 0);

        mysql.queryRecord("select 1");

        cout << "success" << endl;
    }
    catch(exception &ex)
    {
        cerr << ex.what() << endl;
    }
}

#endif