#include "util/tc_port.h"
#include "util/tc_common.h"
#include <cmath>
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace tars;

class UtilPortTest : public testing::Test
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

TEST_F(UtilPortTest, testExec)
{
	string err;
   	string result = TC_Port::exec("ls '*.txt'", err);
   	cout << result << endl;
}

TEST_F(UtilPortTest, testGetPidMemUsed)
{
    int64_t used = TC_Port::getPidMemUsed(TC_Port::getpid());
    cout << "used:" << used << endl;

    ASSERT_TRUE(used > 0);
}

TEST_F(UtilPortTest, testGetPidStartTime)
{
    int64_t t = TC_Port::getPidStartTime(TC_Port::getpid());
    cout << "testGetPidStartTime:" << t << endl;

    ASSERT_TRUE(t >= time(NULL));
}

TEST_F(UtilPortTest, testGetUPTime)
{
    time_t t = TC_Port::getUPTime();
    cout << "testGetUPTime:" << (time(NULL)-t)/60/60/24 << "day" << endl;
    ASSERT_TRUE(t > 0);
}

TEST_F(UtilPortTest, testGetSystemMemInfo)
{
    int64_t totalSize = 0;
    int64_t availableSize = 0;
    float usedPercent = 0;

    bool flag = TC_Port::getSystemMemInfo(totalSize, availableSize, usedPercent);

    cout << "totalSize: " << totalSize << endl;
    cout << "availableSize: " << availableSize << endl;
    cout << "usedPercent: " << usedPercent << endl;
    ASSERT_TRUE(flag);
    ASSERT_TRUE(totalSize > 0);
    ASSERT_TRUE(availableSize > 0);
    ASSERT_TRUE(usedPercent > 0);
}

TEST_F(UtilPortTest, testGetCPUProcessor)
{
    int n = TC_Port::getCPUProcessor();

    cout << "cpu: " << n << endl;

    ASSERT_TRUE(n > 0);
}

TEST_F(UtilPortTest, testGetDiskInfo)
{
    float usedPercent = 0;
    int64_t availableSize = 0;
    int64_t totalSize = 0;

#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_LINUX
    TC_Port::getDiskInfo(totalSize, availableSize, usedPercent, "/");
#else
    TC_Port::getDiskInfo(totalSize, availableSize, usedPercent, "c:\\");
#endif

    cout << "totalSize: " << totalSize << endl;
    cout << "usedPercent: " << usedPercent << endl;
    cout << "availableSize: " << availableSize << endl;

    ASSERT_TRUE(totalSize > 0);
    ASSERT_TRUE(usedPercent > 0);
    ASSERT_TRUE(availableSize > 0);
}

TEST_F(UtilPortTest, testGetCpuLoad)
{
    cout << "cpu load:" << TC_Port::getCpuLoad(500) << endl;
}

#if TARGET_PLATFORM_WINDOWS
#include <windows.h>

TEST_F(UtilPortTest, testGetDisk)
{
    DWORD aDrive = GetLogicalDrives();

    for (int i = 0; i < 26; i++) {
        if (aDrive & (1 << i)) {
            char drive[4];
            sprintf(drive, "%c:\\", 'A' + i);
            std::cout << drive << std::endl;
        }
    }
}

#endif
//
//#if TARGET_PLATFORM_IOS
//#include <sys/types.h>
//#include <sys/sysctl.h>
//#include <mach/mach.h>
//#include <libproc.h>
//#include <sys/proc_info.h>
//#endif
//
//
//vector<int64_t> getPidsByCmdline(const string &cmdLine, bool accurateMatch)
//{
//    vector<int64_t> pids;
//
//#if TARGET_PLATFORM_IOS
//    int mib[4];
//    mib[0] = CTL_KERN;
//    mib[1] = KERN_PROC;
//    mib[2] = KERN_PROC_ALL;
//    mib[3] = 0;
//
//    size_t size;
//    if (sysctl(mib, 4, NULL, &size, NULL, 0) == -1) {
//        return {};
//    }
//
//    struct kinfo_proc* proc_list = (struct kinfo_proc*)malloc(size);
//    if (proc_list == NULL) {
//        return {};
//    }
//
//    if (sysctl(mib, 4, proc_list, &size, NULL, 0) == -1) {
//        free(proc_list);
//        return {};
//    }
//
//    int num_procs = size / sizeof(struct kinfo_proc);
//    for (int i = 0; i < num_procs; i++)
//    {
//        vector<string> args = getArgs(proc_list[i].kp_proc.p_pid);
//        string path = TC_Common::tostr(args.begin(), args.end(), " ");
//
//        if(accurateMatch)
//        {
//            if(cmdLine == path)
//            {
//                pids.push_back(proc_list[i].kp_proc.p_pid);
//            }
//        }
//        else
//        {
//            if(std::string(path).find(cmdLine) != std::string::npos)
//            {
//                pids.push_back(proc_list[i].kp_proc.p_pid);
//            }
//        }
//    }
//
//    free(proc_list);
//    return pids;
//#elif TARGET_PLATFORM_LINUX
//    return {};
//#elif TARGET_PLATFORM_WINDOWS
//    return {};
//#else
//    return {};
//#endif
//}

TEST_F(UtilPortTest, testGetCommandline)
{
    vector<string> cmdLine = TC_Port::getCommandLine(TC_Port::getpid());

    cout << cmdLine.size() << endl;

    string cmdStr = TC_Common::tostr(cmdLine.begin(), cmdLine.end(), " ");
    cout << cmdStr << endl;
    ASSERT_TRUE(cmdStr.find("unit-test") != string::npos);
}

TEST_F(UtilPortTest, testGetPidsByCmdline)
{
    vector<int64_t> pids = TC_Port::getPidsByCmdline("unit-test", false);

    cout << TC_Common::tostr(pids.begin(), pids.end(), ", ") << endl;

    int64_t pid = TC_Port::getpid();

    ASSERT_TRUE(std::find(pids.begin(), pids.end(), pid) != pids.end());
}