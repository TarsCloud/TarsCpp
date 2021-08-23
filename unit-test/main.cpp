#include <cassert>
#include <iostream>
#include <vector>
#include "util/tc_common.h"
#include "util/tc_logger.h"
#include "gtest/gtest.h"

using namespace std;
using namespace tars;

int main(int argc, char** argv)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	tars::TC_Common::ignorePipe();
#endif

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}