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
	 * @param sPwdPath: 程序运行的当前路径
	 * @param sRollLogPath: 滚动日志路径(stdout会重定向到滚动日志), 为空则不重定向
	 * @param vOptions: 参数
	 * @return 子进程id: ==0: 子进程中, >0: 父进程中(子进程pid), 其他抛出异常 TC_Port_Exception
	 */
	static int64_t forkExec(const string& sExe, const string& sPwdPath, const string& sRollLogPath, const vector<string>& vOptions);

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
