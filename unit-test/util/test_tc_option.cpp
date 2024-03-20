#include "util/tc_option.h"
#include "util/tc_common.h"
#include <cmath>
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilOptionTest : public testing::Test
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

std::vector<std::string> parseString(const std::string& input)
{
    std::vector<std::string> result;
    std::string currentString;

    bool insideQuotes = false;

    for (char c : input) {
        if (c == ' ' && !insideQuotes) {
            if (!currentString.empty()) {
                result.push_back(currentString);
                currentString.clear();
            }
        } else if (c == '"' && !insideQuotes) {
            currentString += c;
            insideQuotes = true;
        } else if (c == '"' && insideQuotes) {
            currentString += c;
            insideQuotes = false;
            result.push_back(currentString);
            currentString.clear();
        } else {
            currentString += c;
        }
    }

    if (!currentString.empty()) {
        result.push_back(currentString);
    }

    return result;
}

TEST_F(UtilOptionTest, option)
{
    string command = "server update --config=test.conf --abc --ep=\"tcp -h 127.0.0.1 -p 2345\"";
    auto v = parseString(command.c_str());

    EXPECT_EQ(v.size(), 5);
}

TEST_F(UtilOptionTest, option_test)
{
    string command = "server update --config=test.conf --abc";

    {
        TC_Option option;
        option.decode(command.c_str());

        EXPECT_EQ(option.getSingle().size(), 2);
        EXPECT_EQ(option.getSingle()[0], "server");
        EXPECT_EQ(option.getSingle()[1], "update");
        EXPECT_EQ(option.getValue("config"), "test.conf");
        EXPECT_EQ(option.hasParam("abc"), true);
    }

    {
        command = "server update --config=test.conf --abc --ep=\"tcp -h 127.0.0.1 -p 2345\"";
        TC_Option option;
        option.decode(command.c_str());
        EXPECT_EQ(option.getSingle().size(), 2);
        EXPECT_EQ(option.getSingle()[0], "server");
        EXPECT_EQ(option.getSingle()[1], "update");
        EXPECT_EQ(option.getValue("config"), "test.conf");
        EXPECT_EQ(option.hasParam("abc"), true);
        EXPECT_EQ(option.getValue("ep"), "tcp -h 127.0.0.1 -p 2345");
    }

}
