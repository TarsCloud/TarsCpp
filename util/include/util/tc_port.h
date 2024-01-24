#ifndef __TC_PORT_H
#define __TC_PORT_H

#include "util/tc_platform.h"
#include "util/tc_ex.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS

#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/types.h>
#else

#include <direct.h>
#include <io.h>

typedef unsigned short mode_t;

#define S_IFREG _S_IFREG			//表示为普通文件，为了跨平台，一律使用S_IFREG
#define S_IFDIR _S_IFDIR			//表示为目录，为了跨平台，一律使用S_IFDIR

#endif

#include <stdio.h>
#include <memory>
#include <atomic>
#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <unordered_map>

using namespace std;

namespace tars
{

/**
* @brief 跨平台port异常类
*/
struct TC_Port_Exception : public TC_Exception
{
	TC_Port_Exception(const string &buffer) : TC_Exception(buffer){};
	~TC_Port_Exception() throw() {};
};

class TC_Port
{
public:

    /**
     * @brief 在s1的长度n中搜索s2
     * @return 搜索到的指针, 找不到返回NULL
     */ 
    static const char* strnstr(const char* s1, const char* s2, int pos1);

	static int strcmp(const char *s1, const char *s2);

	static int strncmp(const char *s1, const char *s2, size_t n);

	static int strcasecmp(const char *s1, const char *s2);

    static int strncasecmp(const char *s1, const char *s2, size_t n);    

    static void localtime_r(const time_t *clock, struct tm *result);

    static void gmtime_r(const time_t *clock, struct tm *result);

    static time_t timegm(struct tm *timeptr);

    static int gettimeofday(struct timeval &tv);

    static int chmod(const char *path, mode_t mode);

    static FILE * fopen(const char * path, const char * mode);

#if TARGET_PLATFORM_WINDOWS
    typedef struct _stat stat_t;
#else
    typedef struct stat stat_t;
#endif
    static int lstat(const char * path, stat_t * buf);

    static int mkdir(const char *path);

    static int rmdir(const char *path);

    static int closeSocket(int fd);

    static int64_t getpid();

	/**
	 * 获取环境变量
	 * @param name
	 * @return
	 */
    static std::string getEnv(const std::string &name);

	/**
	 * 设置环境变量
	 * @param name
	 * @param value
	 */
    static void setEnv(const std::string &name, const std::string &value);

    /**
     * 获取当前进程的目录
     * @return
     */
    static string getCwd();

    /**
     * kill某个pid的进程
     * @param pid
     */
    static void kill(int64_t pid);

	/**
	 * 运行一个脚本
	 * @param cmd
	 * @param err
	 * @return 程序的标准输出(最大2k的输出长度)
	 */
    static std::string exec(const char* cmd);

	/**
	 * 运行一个脚本(程序+命令行)
	 * @param cmd
	 * @param err
	 * @return: 程序的标准输出(最大2k的输出长度)
	 */
	static std::string exec(const char* cmd, std::string &err);

	/**
	 * fork子进程并运行程序
	 * @param sExe: 可执行程序路径
	 * @param sPwdPath:  设置程序运行的当前路径(chdir)
	 * @param sRollLogPath: 重定向输出路径(stdout/stderr会重定向到该日志文件), 为空则不重定向
	 * @param vArgs: 参数
	 * @return 子进程id: ==0: 子进程中, >0: 父进程中(子进程pid), 其他抛出异常 TC_Port_Exception
	 */
	static int64_t forkExec(const string& sExe, const string& sPwdPath, const string& sRollLogPath, const vector<string>& vArgs);

#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_LINUX
    /**
     * 关闭所有当前进程的文件句柄(除了stdin/stdout/stderr)
     */
    static void closeAllFileDescriptors();
#endif

    /**
	 * 注册ctrl+c回调事件(SIGINT/CTRL_C_EVENT)
	 * @param callback
	 * @return size_t, 注册事件的id, 取消注册时需要
	 */
	static size_t registerCtrlC(std::function<void()> callback);

	/**
	 * 取消注册ctrl+c回调事件
	 * @param callback
	 * @return
	 */
	static void unregisterCtrlC(size_t id);

	/**
	 * 注册term事件的回调(SIGTERM/CTRL_SHUTDOWN_EVENT)
	 * @param callback
	 * @return size_t, 注册事件的id, 取消注册时需要
	 */
	static size_t registerTerm(std::function<void()> callback);

	/**
	 * 取消注册
	 * @param id
	 */
	static void unregisterTerm(size_t id);

	/**
	 * 获取指定进程占用物理内存大小, 返回内存大小(K, M, G)
	 * @param pid: 目标进程id
	 * @param unit: 指定单位
	 * return -1 表示获取失败
	 *
	 */
	static int64_t getPidMemUsed(int64_t pid, const char unit = 'M');

	/**
	 * 获取进程启动时间
	 * @param pid
	 * @return
	 */
	static time_t getPidStartTime(int64_t pid);

	/**
	 * 服务器启动时间
	 * @return
	 */
	static time_t getUPTime();

	/**
	 * 获取系统内存信息
	 * @param totalSize, 总内存大小
	 * @param availableSize , 剩余可用内存大小
	 * @param usedPercent , 已使用内存百分比
	 * @param unit, 内存单位
	 * @return
	 */
	static bool getSystemMemInfo(int64_t &totalSize, int64_t &availableSize, float &usedPercent, const char unit = 'M');

	/**
	 * 获取系统逻辑CPU核数
	 * @return -1 表示获取失败
	 */
	static int  getCPUProcessor();

	/**
	 * 获取磁盘信息
	 * @param usedPercent 磁盘使用百分比
	 * @param availableSize 剩余磁盘空间（单位M）
	 * @param path, 为该路径所在目标磁盘
	 */

	static bool getDiskInfo(float& usedPercent, int64_t& availableSize, const string& path);

protected:

	static size_t registerSig(int sig, std::function<void()> callback);
	static void unregisterSig(int sig, size_t id);
	static void registerSig(int sig);

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	static void sighandler( int sig_no );
#else
	static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
#endif

	struct SigInfo
	{
		std::mutex   _mutex;

		unordered_map<int, unordered_map<size_t, std::function<void()>>> _callbacks;

		std::atomic<size_t> _callbackId{0};
	};

	static shared_ptr<SigInfo>	_sigInfo;
};

}

#endif
