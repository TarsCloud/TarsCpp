#ifndef __TC_PORT_H
#define __TC_PORT_H

#include "util/tc_platform.h"
#include "util/tc_ex.h"
#include "util/tc_singleton.h"
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

/**
 * 跨平台相关函数封装
 */
class TC_Port
{
public:

    /**
     * @brief 在s1的长度n中搜索s2
     * @return 搜索到的指针, 找不到返回NULL
     */ 
    static const char* strnstr(const char* s1, const char* s2, int pos1);

    /**
     * 字符串比较
     * @param s1
     * @param s2
     * @return
     */
	static int strcmp(const char *s1, const char *s2);

    /**
     * 字符串比较
     * @param s1
     * @param s2
     * @param n
     * @return
     */
	static int strncmp(const char *s1, const char *s2, size_t n);

    /**
     * 忽略大小写比较
     * @param s1
     * @param s2
     * @return
     */
	static int strcasecmp(const char *s1, const char *s2);

    /**
     * 忽略大小写比较
     * @param s1
     * @param s2
     * @param n
     * @return
     */
    static int strncasecmp(const char *s1, const char *s2, size_t n);    

    /**
     * 从clock得到当前(到时区的)时间
     * @param clock
     * @param result
     */
    static void localtime_r(const time_t *clock, struct tm *result);

    /**
     * 从clock得到当前时间GMT(不考虑时区, 夏令时等)
     * @param clock
     * @param result
     */
    static void gmtime_r(const time_t *clock, struct tm *result);

    /**
     * 转换成时间戳(GMT时间)
     * @param timeptr
     * @return
     */
    static time_t timegm(struct tm *timeptr);

    /**
     * 当前时间
     * @param tv
     * @return
     */
    static int gettimeofday(struct timeval &tv);

    /**
     * 改变文件属性
     * @param path
     * @param mode
     * @return
     */
    static int chmod(const char *path, mode_t mode);

#if TARGET_PLATFORM_WINDOWS
    typedef struct _stat stat_t;
#else
    typedef struct stat stat_t;
#endif

    /**
     * 查看文件属性
     * @param path
     * @param buf
     * @return
     */
    static int lstat(const char * path, stat_t * buf);

    /**
     * 创建目录(可以使用TC_File下的函数makeDir/makeDirRecursive)
     * @param path
     * @return
     */
    static int mkdir(const char *path);

    /**
     * 删除路径 可以使用TC_File::removeFile
     * @param path
     * @return
     */
    static int rmdir(const char *path);

    /**
     * 打开一个文件
     * @param path
     * @param mode
     * @return
     */
    static FILE * fopen(const char * path, const char * mode);

    /**
     * src重定向到file
     * @param src
     * @param mode
     * @param dst
     * @return
     */
    static FILE	*freopen(const char * dst,  const char * mode, FILE * src);

    /**
     * 关闭网络句柄
     * @param fd
     * @return
     */
    static int closeSocket(int fd);

    /**
     * 获取进程pid
     * @return
     */
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
	 * @return 程序的标准输出
	 */
    static std::string exec(const char* cmd);

	/**
	 * 运行一个脚本(程序+命令行)
	 * @param cmd
	 * @param err
	 * @return: 程序的标准输出
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
     * 返回完整命令行参数, 如果pid不存在, 则返回为空
     * @param pid
     * @return, 命令行参数, argv[0], argv[1] ....
     */
    static std::vector<std::string> getCommandLine(int64_t pid);

    /**
     * 根据程序执行的命令行, 获取对应的进程pid
     * @param cmdLine: 程序的启动完整命令行
     * @param accurateMatch: 匹配方式, true: 精确匹配, false: 模糊匹配(启动命令行能find到cmdLine)
     * @return
     */
    static vector<int64_t> getPidsByCmdline(const string &cmdLine, bool accurateMatch = false);

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
     * 获取cpu负载
     * @param queryTime, windows下有效, 会阻塞的时间, 毫秒
     *
     * @return, 如果失败, 返回-1, 否在返回cpu负载的值
     */
    static double getCpuLoad(uint32_t queryTime = 500);

	/**
	 * 获取指定进程占用物理内存大小, 返回内存大小(K, M, G)
	 * @param pid: 目标进程id
	 * @param unit: 指定单位
	 * return -1 表示获取失败
	 *
	 */
	static int64_t getPidMemUsed(int64_t pid, const char unit = 'M');

	/**
	 * 获取进程启动时间(秒)
	 * @param pid
	 * @return
	 */
	static time_t getPidStartTime(int64_t pid);

	/**
	 * 服务器启动的绝对时间(秒)
	 * @return
	 */
	static time_t getUPTime();

	/**
	 * 获取系统内存信息
	 * @param totalSize, 总内存大小
	 * @param availableSize , 剩余可用内存大小
	 * @param usedPercent , 已使用内存百分比
	 * @param unit, 内存单位(B, K, G, M)
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
	 * @param availableSize 剩余磁盘空间（字节数）
	 * @param path, 为该路径所在目标磁盘
	 */

	static bool getDiskInfo(int64_t &totalSize, int64_t& availableSize, float& usedPercent, const string& path);

protected:

	static size_t registerSig(int sig, std::function<void()> callback);
	static void unregisterSig(int sig, size_t id);
	static void registerSig(int sig);

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	static void sighandler( int sig_no );
#else
	static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
#endif

	struct SigInfo : public TC_Singleton<SigInfo, CreateUsingNew, NoDestroyLifetime>
	{
		std::mutex   _mutex;

		unordered_map<int, unordered_map<size_t, std::function<void()>>> _callbacks;

		std::atomic<size_t> _callbackId{0};
	};

//	static SigInfo *_sigInfo;
};

}

#endif
