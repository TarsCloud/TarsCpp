#include "util/tc_port.h"
#include "util/tc_common.h"
#include <cmath>
#include "gtest/gtest.h"
#include "util/tc_file.h"
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

    string out = TC_Port::exec(("file " + TC_File::getExePath()).c_str());

    cout << out << endl;
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

#if TARGET_PLATFORM_WINDOWS
TEST_F(UtilPortTest, unicodeUtf8EnvironmentAndDiskPath)
{
    const string directory = u8"tars_中文路径_端口测试";
    const string variable = u8"TARS_中文环境变量";
    const string value = u8"中文环境变量值";

    TC_File::removeFile(directory, true);
    ASSERT_TRUE(TC_File::makeDir(directory));

    TC_Port::setEnv(variable, value);
    ASSERT_EQ(value, TC_Port::getEnv(variable));

    int64_t totalSize = 0;
    int64_t availableSize = 0;
    float usedPercent = 0;
    ASSERT_TRUE(TC_Port::getDiskInfo(totalSize, availableSize, usedPercent, directory));
    ASSERT_GT(totalSize, 0);

    ASSERT_EQ(0, TC_File::removeFile(directory, true));
}

TEST_F(UtilPortTest, unicodeUtf8ExecutableAndWorkingDirectory)
{
    const string directory = u8"tars_中文路径_进程测试";
    const string helper = TC_File::extractFilePath(TC_File::getExePath()) + "tars-unicode-process-helper.exe";
    const string copiedHelper = directory + FILE_SEP + u8"中文进程.exe";

    TC_File::removeFile(directory, true);
    ASSERT_TRUE(TC_File::makeDir(directory));
    TC_File::copyFile(helper, copiedHelper, false);

    const int64_t pid = TC_Port::forkExec(copiedHelper, directory, "", vector<string>());
    ASSERT_GT(pid, 0);

    HANDLE process = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION, FALSE, static_cast<DWORD>(pid));
    ASSERT_NE(nullptr, process);
    ASSERT_EQ(WAIT_OBJECT_0, WaitForSingleObject(process, 10000));
    DWORD exitCode = 1;
    ASSERT_TRUE(GetExitCodeProcess(process, &exitCode));
    CloseHandle(process);
    ASSERT_EQ(0u, exitCode);

    ASSERT_EQ(0, TC_File::removeFile(directory, true));
}
#endif

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
