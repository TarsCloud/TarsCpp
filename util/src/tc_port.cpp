/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include "util/tc_port.h"
#include "util/tc_common.h"
#include "util/tc_logger.h"
#include "util/tc_file.h"
#include "util/tc_platform.h"
#include <thread>
#include <string.h>

#if TARGET_PLATFORM_LINUX 
#include <sys/vfs.h>
#include <sys/sysinfo.h>
#include <linux/param.h>
#endif

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <signal.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/wait.h>
#else

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "pdh.lib")

#include <windows.h>
#include <time.h>
#include <sys/timeb.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <shellapi.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <tchar.h>

#include "util/tc_strptime.h"
#endif

#if TARGET_PLATFORM_IOS
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <libproc.h>
#include <sys/proc_info.h>
#endif

namespace tars
{

int TC_Port::strcmp(const char *s1, const char *s2)
{
#if TARGET_PLATFORM_WINDOWS
	return ::strcmp(s1, s2);
#else
	return ::strcmp(s1, s2);
#endif
}

int TC_Port::strncmp(const char *s1, const char *s2, size_t n)
{
#if TARGET_PLATFORM_WINDOWS
	return ::strncmp(s1, s2, n);
#else
	return ::strncmp(s1, s2, n);
#endif
}

const char* TC_Port::strnstr(const char* s1, const char* s2, int pos1)
{
	int l1 = 0;
	int l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;

	const char *p = s1;
	while(l1 < pos1 && *p++ != '\0')
	{
		++l1;
	}	
	// l1 = strlen(s1);

	// pos1 = (pos1 > l1)?l1:pos1;

	while (pos1 >= l2) {
		pos1--;
		if (!memcmp(s1, s2, l2))
			return s1;
		s1++;
	}
	return NULL;
}

int TC_Port::strcasecmp(const char *s1, const char *s2)
{
#if TARGET_PLATFORM_WINDOWS
    return ::_stricmp(s1, s2);
#else
    return ::strcasecmp(s1, s2);
#endif
}

int TC_Port::strncasecmp(const char *s1, const char *s2, size_t n)
{
#if TARGET_PLATFORM_WINDOWS
    return ::_strnicmp(s1, s2, n);
#else
    return ::strncasecmp(s1, s2, n);
#endif
} 

void TC_Port::localtime_r(const time_t *clock, struct tm *result)
{
	//带时区时间
#if TARGET_PLATFORM_WINDOWS
	::localtime_s(result, clock);
#else
	::localtime_r(clock, result);
#endif
}

void TC_Port::gmtime_r(const time_t *clock, struct tm *result)
{
#if TARGET_PLATFORM_WINDOWS
	::gmtime_s(result, clock);
#else
	::gmtime_r(clock, result);
#endif

}

time_t TC_Port::timegm(struct tm *timeptr)
{
#if TARGET_PLATFORM_WINDOWS
	return ::_mkgmtime(timeptr);
#else
	return ::timegm(timeptr);
#endif
}

int TC_Port::gettimeofday(struct timeval &tv)
{
#if TARGET_PLATFORM_WINDOWS
	static const DWORDLONG FILETIME_to_timeval_skew = 116444736000000000;
	FILETIME tfile;
	::GetSystemTimeAsFileTime(&tfile);

	ULARGE_INTEGER tmp;
	tmp.LowPart = tfile.dwLowDateTime;
	tmp.HighPart = tfile.dwHighDateTime;
	tmp.QuadPart -= FILETIME_to_timeval_skew;

	ULARGE_INTEGER largeInt;
	largeInt.QuadPart = tmp.QuadPart / (10000 * 1000);
	tv.tv_sec = (long)(tmp.QuadPart / (10000 * 1000));
	tv.tv_usec = (long)((tmp.QuadPart % (10000 * 1000)) / 10);
	return 0;
#else
	return ::gettimeofday(&tv, 0);
#endif
}

int TC_Port::chmod(const char *path, mode_t mode)
{
	//带时区时间
#if TARGET_PLATFORM_WINDOWS
	return ::_chmod(path, mode);
#else
	return ::chmod(path, mode);
#endif	
}

FILE * TC_Port::fopen(const char * path, const char *  mode)
{
#if TARGET_PLATFORM_WINDOWS
	FILE *fp;	
	if (fopen_s(&fp, path, mode) != 0)
	{
		return NULL;
	}

	return fp;
#else
	return ::fopen(path, mode);
#endif
}

int TC_Port::stat(const char * path, stat_t * buf)
{
#if TARGET_PLATFORM_WINDOWS
    return ::_stat(path, buf);
#else
    return ::stat(path, buf);
#endif
}

int TC_Port::lstat(const char * path, TC_Port::stat_t * buf)
{
#if TARGET_PLATFORM_WINDOWS
	return ::_stat(path, buf);
#else
	return ::lstat(path, buf);
#endif
}

int TC_Port::mkdir(const char *path)
{
#if TARGET_PLATFORM_WINDOWS
	int iRetCode = ::_mkdir(path);
#else
	int iRetCode = ::mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif
	return iRetCode;
}

int TC_Port::rmdir(const char *path)
{
#if TARGET_PLATFORM_WINDOWS
	return ::_rmdir(path);
#else
	return ::rmdir(path);
#endif
}

int TC_Port::closeSocket(int fd)
{
#if TARGET_PLATFORM_WINDOWS
	return ::closesocket(fd);
#else
    return ::close(fd);
#endif	
}

int64_t TC_Port::getpid()
{
#if TARGET_PLATFORM_WINDOWS
	int64_t pid = ::GetCurrentProcessId();
#else
    int64_t pid = ::getpid();
#endif
	return pid;
}

string TC_Port::getEnv(const string &name)
{
	char* p = getenv(name.c_str());
    string str = p ? string(p) : "";

	return str;
}

void TC_Port::setEnv(const string &name, const string &value)
{
#if TARGET_PLATFORM_WINDOWS
	SetEnvironmentVariable(name.c_str(), value.c_str());
#else
	setenv(name.c_str(), value.c_str(), true);
#endif
}

string TC_Port::getCwd()
{
    char currentDirectory[FILENAME_MAX] = {0x00};
#if TARGET_PLATFORM_WINDOWS
    _getcwd(currentDirectory, sizeof(currentDirectory));
#else
    getcwd(currentDirectory, sizeof(currentDirectory));
#endif
    return currentDirectory;
}

int TC_Port::kill(int64_t pid)
{
#if TARGET_PLATFORM_WINDOWS
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL)
    {
        return -1;
    }

    ::TerminateProcess(hProcess, 0);

    CloseHandle(hProcess);
    return 0;
#else
    int ret = ::kill(static_cast<pid_t>(pid), SIGKILL);
    if(ret != 0 && errno != ESRCH && errno != ENOENT)
    {
        return -1;
    }
    else
    {
        waitpid(pid, NULL, WNOHANG);
    }

    return 0;
#endif
}

int TC_Port::alive(int64_t pid)
{
#if TARGET_PLATFORM_WINDOWS
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL)
    {
        return -1;
    }

    CloseHandle(hProcess);
    return 0;
#else
    int ret = ::kill(static_cast<pid_t>(pid), 0);
    if(ret != 0)
    {
        return -1;
    }

    return 0;
#endif
}

string TC_Port::exec(const char *cmd)
{
	string err;
	return exec(cmd, err);
}

std::string TC_Port::exec(const char* cmd, std::string &err)
{
    std::string result;
#if TARGET_PLATFORM_WINDOWS
    FILE* fp = _popen(cmd, "r");
#else
    FILE* fp = popen(cmd, "r");
#endif
    if(fp == NULL) {
        err = "open '" + string(cmd) + "' error";
        return "";
    }

    char buffer[1024];
    while (!feof(fp)) {
        if (fgets(buffer, 1024, fp) != NULL)
            result.append(buffer);
    }

#if TARGET_PLATFORM_WINDOWS
    _pclose(fp);
#else
    pclose(fp);
#endif

    return result;
}

#if TARGET_PLATFORM_LINUX
void TC_Port::closeAllFileDescriptors() {
    DIR *dir;
    struct dirent *entry;
    int fd;

    dir = opendir("/proc/self/fd");
    if (dir == NULL) {
        perror("Cannot open /proc/self/fd");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        fd = atoi(entry->d_name);
        if (fd > STDERR_FILENO) { // Skip stdin, stdout, and stderr
            close(fd);
        }
    }

    closedir(dir);
}

#elif TARGET_PLATFORM_IOS

void TC_Port::closeAllFileDescriptors()
{
    int max_fd;
    int fd;
    size_t len = sizeof(max_fd);

    // Get the maximum file descriptor number for the current process
    if (sysctlbyname("kern.maxfilesperproc", &max_fd, &len, NULL, 0) < 0)
    {
        return;
    }

    for (fd = STDERR_FILENO + 1; fd < max_fd; fd++)
    {
        close(fd);
    }
}

#endif

#if TARGET_PLATFORM_LINUX
std::vector<std::string> TC_Port::getCommandLine(int64_t pid)
{
    std::vector<std::string> commandLineArgs;

    std::string procPath = "/proc/" + std::to_string(pid) + "/cmdline";
    std::ifstream cmdlineFile(procPath);
    if (!cmdlineFile.is_open()) {
        return {};
    }

    std::stringstream buffer;
    buffer << cmdlineFile.rdbuf();
    std::string cmdline = buffer.str();

    size_t pos = 0;
    while (pos < cmdline.size()) {
        std::string arg;
        size_t nextPos = cmdline.find('\0', pos);
        if (nextPos == std::string::npos) {
            arg = cmdline.substr(pos);
            pos = cmdline.size();
        } else {
            arg = cmdline.substr(pos, nextPos - pos);
            pos = nextPos + 1;
        }
        commandLineArgs.push_back(arg);
    }

    return commandLineArgs;
}
#endif

#if TARGET_PLATFORM_WINDOWS

std::vector<std::string> TC_Port::getCommandLine(int64_t pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(pid));
    if (hProcess == NULL) {
        return {};
    }

    std::vector<std::string> commandLineArgs;

    LPWSTR cmdLine = GetCommandLineW();

    int argc;
    LPWSTR* argv = CommandLineToArgvW(cmdLine, &argc);

    if (argv != NULL) {
        for (int i = 0; i < argc; i++) {
            char buffer[MAX_PATH];
            WideCharToMultiByte(CP_ACP, 0, argv[i], -1, buffer, MAX_PATH, NULL, NULL);
            commandLineArgs.push_back(buffer);
        }

        LocalFree(argv);
    }

    CloseHandle(hProcess);

    return commandLineArgs;
}
#endif
#if TARGET_PLATFORM_IOS

std::vector<std::string> TC_Port::getCommandLine(int64_t pid)
{
    vector<string> args;

    int    mib[3], argmax, nargs, c = 0;
    size_t    size;
    char    *procargs, *sp, *np, *cp;
    int show_args = 1;

    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;

    size = sizeof(argmax);
    if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1) {
        goto ERROR_A;
    }

    /* Allocate space for the arguments. */
    procargs = (char *)malloc(argmax);
    if (procargs == NULL) {
        goto ERROR_A;
    }

    /*
     * Make a sysctl() call to get the raw argument space of the process.
     * The layout is documented in start.s, which is part of the Csu
     * project.  In summary, it looks like:
     *
     * /---------------\ 0x00000000
     * :               :
     * :               :
     * |---------------|
     * | argc          |
     * |---------------|
     * | arg[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | arg[argc - 1] |
     * |---------------|
     * | 0             |
     * |---------------|
     * | env[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | env[n]        |
     * |---------------|
     * | 0             |
     * |---------------| <-- Beginning of data returned by sysctl() is here.
     * | argc          |
     * |---------------|
     * | exec_path     |
     * |:::::::::::::::|
     * |               |
     * | String area.  |
     * |               |
     * |---------------| <-- Top of stack.
     * :               :
     * :               :
     * \---------------/ 0xffffffff
     */
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROCARGS2;
    mib[2] = pid;

    size = (size_t)argmax;
    if (sysctl(mib, 3, procargs, &size, NULL, 0) == -1) {
        goto ERROR_B;
    }

    memcpy(&nargs, procargs, sizeof(nargs));
    cp = procargs + sizeof(nargs);

    /* Skip the saved exec_path. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            /* End of exec_path reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        goto ERROR_B;
    }

    /* Skip trailing '\0' characters. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp != '\0') {
            /* Beginning of first argument reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        goto ERROR_B;
    }
    /* Save where the argv[0] string starts. */
    sp = cp;

    /*
     * Iterate through the '\0'-terminated strings and convert '\0' to ' '
     * until a string is found that has a '=' character in it (or there are
     * no more strings in procargs).  There is no way to deterministically
     * know where the command arguments end and the environment strings
     * start, which is why the '=' character is searched for as a heuristic.
     */
    for (np = NULL; c < nargs && cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            c++;
            if (np != NULL) {
                /* Convert previous '\0'. */
                *np = ' ';
            } else {
                /* *argv0len = cp - sp; */
            }
            /* Note location of current '\0'. */
            np = cp;

            if (!show_args) {
                /*
                 * Don't convert '\0' characters to ' '.
                 * However, we needed to know that the
                 * command name was terminated, which we
                 * now know.
                 */
                break;
            }
        }
    }

    /*
     * sp points to the beginning of the arguments/environment string, and
     * np should point to the '\0' terminator for the string.
     */
    if (np == NULL || np == sp) {
        /* Empty or unterminated string. */
        goto ERROR_B;
    }

    /* Make a copy of the string. */
//    printf("%s\n", sp);

    args.push_back(sp);

    /* Clean up. */
    free(procargs);
    return args;

    ERROR_B:
    free(procargs);
    ERROR_A:

    return args;
}
#endif
vector<int64_t> TC_Port::getPidsByCmdline(const string &cmdLine, bool accurateMatch)
{
    vector<int64_t> pids;

#if TARGET_PLATFORM_IOS
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_ALL;
    mib[3] = 0;

    size_t size;
    if (sysctl(mib, 4, NULL, &size, NULL, 0) == -1) {
        return {};
    }

    struct kinfo_proc* proc_list = (struct kinfo_proc*)malloc(size);
    if (proc_list == NULL) {
        return {};
    }

    if (sysctl(mib, 4, proc_list, &size, NULL, 0) == -1) {
        free(proc_list);
        return {};
    }

    int num_procs = size / sizeof(struct kinfo_proc);
    for (int i = 0; i < num_procs; i++)
    {
        vector<string> args = getCommandLine(proc_list[i].kp_proc.p_pid);
        string path = TC_Common::tostr(args.begin(), args.end(), " ");

        if(accurateMatch)
        {
            if(cmdLine == path)
            {
                pids.push_back(proc_list[i].kp_proc.p_pid);
            }
        }
        else
        {
            if(std::string(path).find(cmdLine) != std::string::npos)
            {
                pids.push_back(proc_list[i].kp_proc.p_pid);
            }
        }
    }

    free(proc_list);

    return pids;
#elif TARGET_PLATFORM_LINUX
    DIR* dir = opendir("/proc");
    if (!dir) {
        return pids;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if(TC_Common::isdigit(entry->d_name))
        {
            int64_t pid = TC_Common::strto<int64_t>(entry->d_name);
            vector<string> args = getCommandLine(pid);
            string path = TC_Common::tostr(args.begin(), args.end(), " ");

            if(accurateMatch)
            {
                if(cmdLine == path)
                {
                    pids.push_back(pid);
                }
            }
            else
            {
                if(path.find(cmdLine) != std::string::npos)
                {
                    pids.push_back(pid);
                }
            }
        }
    }

    closedir(dir);
    return pids;
#elif TARGET_PLATFORM_WINDOWS

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return pids;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return pids;
    }

    // std::wstring wcmdLine(cmdLine.begin(), cmdLine.end());

    do {
        if(accurateMatch)
        {
            if(cmdLine == pe32.szExeFile)
            {
                pids.push_back(pe32.th32ProcessID);
            }
        }
        else
        {
            if(std::string(pe32.szExeFile).find(cmdLine) != std::string::npos)
            {
                pids.push_back( pe32.th32ProcessID);
            }
        }

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    return pids;
#else
    return {};
#endif
}

FILE *TC_Port::freopen(const char * dst,  const char * mode, FILE * src)
{
#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_WINDOWS
    return ::freopen(dst, mode, src);
#else
    return ::freopen64(dst, mode, src);
#endif
}

int64_t TC_Port::forkExec(const string& sExePath, const string& sPwdPath, const string& sRollLogPath, const vector<string>& vOptions)
{
	vector<string> vEnvs;

	if (sExePath.empty())
	{
		throw TC_Port_Exception("[TC_Port::forkExec] server exe: " + sExePath + " is empty.");
	}

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	if (TC_File::isFileExistEx(sExePath) && !TC_File::canExecutable(sExePath))
	{
		TC_File::setExecutable(sExePath, true);
	}
#endif

	//
	// Current directory
	//
	const char *pwdCStr = sPwdPath.c_str();

#if TARGET_PLATFORM_WINDOWS
	vector<string> vArgs;
	
	vArgs.insert(vArgs.end(), vOptions.begin(), vOptions.end());
	
	string path;
	for (vector<string>::const_iterator p = vArgs.begin(); p != vArgs.end(); ++p)
	{
		path += " " + *p;
	}
	
	string command = sExePath + " " + path;
    // std::wstring wcommand(command.begin(), command.end());

	TCHAR p[1024];
    strncpy_s(p, sizeof(p)/sizeof(TCHAR), command.c_str(), command.size());

    // std::wstring wsPwdPath(sPwdPath.begin(), sPwdPath.end());

	STARTUPINFOA si;
	memset(&si, 0, sizeof(si));
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(pi));
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE; //TRUE表示显示创建的进程的窗口
	
	if (!CreateProcessA(
		NULL,   //  指向一个NULL结尾的、用来指定可执行模块的宽字节字符串
		p,      // 命令行字符串
		NULL,   //    指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承。
		NULL,   //    如果lpProcessAttributes参数为空（NULL），那么句柄不能被继承。<同上>
		false,  //    指示新进程是否从调用进程处继承了句柄。
		CREATE_NEW_CONSOLE|CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,  //  指定附加的、用来控制优先类和进程的创建的标
		NULL, //    指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境
		sPwdPath.c_str(), //    指定子进程的工作路径
		&si, // 决定新进程的主窗体如何显示的STARTUPINFO结构体
		&pi  // 接收新进程的识别信息的PROCESS_INFORMATION结构体
	))
	{
		throw TC_Port_Exception("[TC_Port::forkExec] CreateProcessA exception:" + TC_Exception::getSystemError());
	}
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	
	return pi.dwProcessId;

#else

    string bin = sExePath;
    if(!TC_File::isAbsolute(bin))
    {
        bin = TC_File::simplifyDirectory(TC_Port::getCwd() + FILE_SEP + bin);
    }

	vector<string> vArgs;
	vArgs.push_back(bin);
	vArgs.insert(vArgs.end(), vOptions.begin(), vOptions.end());

	int argc = static_cast<int>(vArgs.size());
	char **argv = static_cast<char **>(malloc((argc + 1) * sizeof(char *)));
	int i = 0;
	for (vector<string>::const_iterator p = vArgs.begin(); p != vArgs.end(); ++p, ++i)
	{
		assert(i < argc);
		argv[i] = strdup(p->c_str());
	}
	assert(i == argc);
	argv[argc] = 0;

	pid_t pid = fork();
	if (pid == -1)
	{
		throw TC_Port_Exception("[TC_Port::forkExec] fork error:" + TC_Exception::getSystemError());
	}

	if (pid == 0)
	{
        closeAllFileDescriptors();

		//stdout/stderr 重定向到日志文件里面
		if (!sRollLogPath.empty())
		{
			TC_File::makeDirRecursive(TC_File::extractFilePath(sRollLogPath));
            TC_Port::freopen(sRollLogPath.c_str(), "ab", stdout);
            TC_Port::freopen(sRollLogPath.c_str(), "ab", stderr);
		}

		if (strlen(pwdCStr) != 0)
		{
			chdir(pwdCStr);
		}

		execvp(argv[0], argv);

		exit(0);
	}
	else
	{
		for (i = 0; argv[i]; i++)
		{
			free(argv[i]);
		}
		free(argv);
	}
	return pid;
#endif
}

//shared_ptr<TC_Port::SigInfo> TC_Port::SigInfo::getInstance() = std::make_shared<TC_Port::SigInfo>();

size_t TC_Port::registerSig(int sig, std::function<void()> callback)
{
	std::lock_guard<std::mutex> lock(SigInfo::getInstance()->_mutex);

	auto it = SigInfo::getInstance()->_callbacks.find(sig);

	if(it == SigInfo::getInstance()->_callbacks.end())
	{
		//没有注册过, 才注册
		registerSig(sig);
	}

	size_t id = ++SigInfo::getInstance()->_callbackId;

    SigInfo::getInstance()->_callbacks[sig][id] = callback;

	return id;
}

void TC_Port::unregisterSig(int sig, size_t id)
{
	//注意_sigInfo是全局静态的, 有可能已经析构了, 需要特殊判断一下!
//	if(SigInfo::getInstance() && SigInfo::getInstance().use_count() > 0)
	{
		std::lock_guard<std::mutex> lock(SigInfo::getInstance()->_mutex);
		auto it = SigInfo::getInstance()->_callbacks.find(sig);

		if(it != SigInfo::getInstance()->_callbacks.end())
		{
			it->second.erase(id);
		}
	}
}

size_t TC_Port::registerCtrlC(std::function<void()> callback)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	return registerSig(SIGINT, callback);
#else
	return registerSig(CTRL_C_EVENT, callback);
#endif
}

void TC_Port::unregisterCtrlC(size_t id)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	unregisterSig(SIGINT, id);
#else
	unregisterSig(CTRL_C_EVENT, id);
#endif
}

size_t TC_Port::registerTerm(std::function<void()> callback)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	return registerSig(SIGTERM, callback);
#else
	return registerSig(CTRL_SHUTDOWN_EVENT, callback);
#endif
}

void TC_Port::unregisterTerm(size_t id)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	unregisterSig(SIGTERM, id);
#else
	unregisterSig(CTRL_SHUTDOWN_EVENT, id);
#endif
}

void TC_Port::registerSig(int sig)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
	signal(sig, TC_Port::sighandler);
#else
	SetConsoleCtrlHandler(TC_Port::HandlerRoutine, TRUE);
#endif
}

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
void TC_Port::sighandler( int sig_no )
{
	std::thread th([=]()
				   {
					   unordered_map<size_t, std::function<void()>> data;

					   {
					   	std::lock_guard<std::mutex> lock(SigInfo::getInstance()->_mutex);

					   	auto it = SigInfo::getInstance()->_callbacks.find(sig_no);
					   	if (it != SigInfo::getInstance()->_callbacks.end())
						   {
							   data = it->second;
						   }
					   }

					   for (auto f : data)
					   {
						   try
						   {
							   f.second();
						   }
						   catch (...)
						   {
						   }
					   }
				   });
	th.detach();
}
#else
BOOL WINAPI TC_Port::HandlerRoutine(DWORD dwCtrlType)
{
	std::thread th([=]()
				   {
					   unordered_map<size_t, std::function<void()>> data;

					   {
					   	std::lock_guard<std::mutex> lock(SigInfo::getInstance()->_mutex);

						   auto it = SigInfo::getInstance()->_callbacks.find(dwCtrlType);
						   if (it != SigInfo::getInstance()->_callbacks.end())
						   {
							   data = it->second;
						   }
					   }

					   for (auto f : data)
					   {
						   try
						   {
							   f.second();
						   }
						   catch (...)
						   {
						   }
					   }
				   });
	th.detach();
	return TRUE;
}
#endif

#if TARGET_PLATFORM_LINUX

static int64_t reSize(int64_t i, const char unit)
{
	switch (unit)
	{
	case 'B':
		return i * 1024;
	case 'M':
		return i / 1024;
	case 'G':
		return i / 1048576;
	default:
		return i;
	}
	return i;    
}
#endif

double TC_Port::getCpuLoad(uint32_t queryTime)
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
    double loadAvg[3];
    if ( getloadavg( loadAvg, 3 ) != -1 )
    {
        return static_cast<float>( loadAvg[0] );
    }
    return -1;
#else
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;

    // Initialize the PDH query
    if (PdhOpenQuery(NULL, NULL, &cpuQuery) != ERROR_SUCCESS) {
        return -1;
    }

    // Add the CPU Time counter to the query
    if (PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal) != ERROR_SUCCESS) {
        return -1;
    }

    // Collect the query data
    if (PdhCollectQueryData(cpuQuery) != ERROR_SUCCESS) {
        return -1;
    }

    // Wait for some time
    TC_Common::msleep(queryTime);

    // Collect the updated query data
    if (PdhCollectQueryData(cpuQuery) != ERROR_SUCCESS) {
        return -1;
    }

    // Get the collected data
    PDH_FMT_COUNTERVALUE counterVal;

    if (PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal) != ERROR_SUCCESS) {
        return -1;
    }

    // Print the CPU usage
//    std::cout << "CPU Usage: " << counterVal.doubleValue << " %\n";

    // Close the query
    if (PdhCloseQuery(cpuQuery) != ERROR_SUCCESS) {
        return -1;
    }

    return counterVal.doubleValue;
#endif
}

// 获取指定进程占用物理内存大小
int64_t TC_Port::getPidMemUsed(int64_t pid, const char unit)
{
#if TARGET_PLATFORM_LINUX
	string filename = "/proc/" + TC_Common::tostr(pid) + "/statm";
	string stream = TC_File::load2str(filename);
	cout << stream << endl;
	if(!stream.empty())
	{
		vector<string> vtStatm = TC_Common::sepstr<string>(stream, " ");
		if (vtStatm.size() >= 2 && TC_Common::isdigit(vtStatm[1]))
		{
			return reSize(TC_Common::strto<int>(vtStatm[1]) * 4, unit);
		}
	}
	return -1;
#elif TARGET_PLATFORM_IOS
    struct proc_taskinfo procInfo;
    int ret = proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &procInfo, PROC_PIDTASKINFO_SIZE);
    if (ret <= 0) {
        return -1;
    }

    int64_t memUsed = procInfo.pti_resident_size;

    // Apply unit conversion if necessary
    switch (unit) {
        case 'K': // KB
            memUsed /= 1024;
            break;
        case 'G': // GB
            memUsed /= (1024 * 1024 * 1024);
            break;
        case 'M': // MB
            memUsed /= (1024 * 1024);
            break;
        default:
            break;
    }

    return memUsed;
#elif TARGET_PLATFORM_WINDOWS
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (processHandle == NULL) {
        return -1;
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(processHandle, &pmc, sizeof(pmc))) {
        CloseHandle(processHandle);
        return -1;
    }

    CloseHandle(processHandle);
    int64_t memUsed = pmc.WorkingSetSize;

    // Apply unit conversion if necessary
    switch (unit) {
        case 'K': // KB
            memUsed /= 1024;
            break;
        case 'G': // GB
            memUsed /= (1024 * 1024 * 1024);
            break;
        case 'M': // MB
            memUsed /= (1024 * 1024);
            break;
        default:
            break;
    }

    return memUsed;
#endif
}

//服务器启动时间
time_t TC_Port::getUPTime()
{
#if TARGET_PLATFORM_LINUX

	string path = "/proc/uptime";
	string data = TC_File::load2str(path);
	vector<string> vs = TC_Common::sepstr<string>(data, " ");
	if (vs.size() != 2)
	{
		return 0;
	}

	return TNOW - (time_t)(TC_Common::strto<double>(vs[0]));

#elif TARGET_PLATFORM_IOS
    struct timeval boottime;
    size_t size = sizeof(boottime);
    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    if (sysctl(mib, 2, &boottime, &size, NULL, 0) != -1 && boottime.tv_sec != 0) {
        time_t bsec = boottime.tv_sec;//, busec = boottime.tv_usec;
//        time_t now = time(NULL);
        return bsec;
    } else {
        return 0;
    }

#elif TARGET_PLATFORM_WINDOWS
    ULONGLONG tickCount = GetTickCount64();
    time_t upTime = tickCount / 1000;
    return upTime;
#endif
}

time_t TC_Port::getPidStartTime(int64_t pid)
{
#if TARGET_PLATFORM_LINUX

	string statPath = "/proc/" + TC_Common::tostr(pid) + "/stat";
	string statData = TC_File::load2str(statPath);
	vector<string> vs = TC_Common::sepstr<string>(statData, " ");
	if (vs.size() < 22)
	{
		return 0;
	}
	unsigned int duration = TC_Common::strto<unsigned int>(vs[21]) / HZ;
	time_t bootTime = getUPTime();
	if (bootTime == 0)
	{
		return 0;
	}
	return bootTime + duration;
#elif TARGET_PLATFORM_IOS
    struct proc_taskallinfo allinfo;
    int ret = proc_pidinfo(pid, PROC_PIDTASKALLINFO, 0, &allinfo, sizeof(allinfo));
    if (ret <= 0) {
        return -1;
    }

    // Convert the process start time from absolute time to seconds since the Epoch
    time_t startTime = allinfo.pbsd.pbi_start_tvsec + allinfo.pbsd.pbi_start_tvusec / 1e6;
    return startTime;
#elif TARGET_PLATFORM_WINDOWS
    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(h == INVALID_HANDLE_VALUE) {
        return -1;
    }

    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(!Process32First(h, &pe)) {
        CloseHandle(h);
        return -1;
    }

    do {
        if(pe.th32ProcessID == pid) {
            FILETIME ftCreate, ftExit, ftKernel, ftUser;
            if (GetProcessTimes(h, &ftCreate, &ftExit, &ftKernel, &ftUser)) {
                ULARGE_INTEGER uli;
                uli.LowPart = ftCreate.dwLowDateTime;
                uli.HighPart = ftCreate.dwHighDateTime;
                time_t startTime = (uli.QuadPart / 10000000ULL - 11644473600ULL);
                CloseHandle(h);
                return startTime;
            }
        }
    } while(Process32Next(h, &pe));

    CloseHandle(h);
    return -1;
#endif
}

bool TC_Port::getSystemMemInfo(int64_t &totalSize, int64_t &availableSize, float &usedPercent, const char unit)
{
#if TARGET_PLATFORM_LINUX
	int fd;
	if((fd = open("/proc/meminfo", O_RDONLY)) == -1)
	{
		return false;
	}

	int cnt = 0;
	char buf[8192];
	char line[1024];

	lseek(fd, 0, SEEK_SET);
	if((cnt = read(fd, buf, sizeof(buf)-1)) < 0)
	{
		close(fd);
		return false;
	}

	buf[cnt] = '\0';

	int pos = 0;
	char name[32];
	unsigned int value;
	unsigned int total = 0;
	unsigned int available = 0;

	for(int i = 0, count = 0;i <= cnt && count < 2;i++)
	{
		line[pos++] = buf[i];
		if(buf[i] != '\n' && buf[i] != '\0')
			continue;

		line[pos] = '\0';
		pos = 0;
		sscanf(line, "%s%unsigned int", name, &value);

		if(!strcmp(name, "MemTotal:"))
		{
			total = value;
			count++;

		}
		else if(!strcmp(name, "MemAvailable:"))
		{
			available = value;
			count++;
		}
	}

	close(fd);

	usedPercent = (total - available) * 1.0 / total * 100;
	totalSize = reSize(total, unit);
	availableSize = reSize(available, unit);
	return true;
#elif TARGET_PLATFORM_IOS
    int mib[2];
    int64_t physical_memory;
    size_t length;
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(int64_t);
    sysctl(mib, 2, &physical_memory, &length, NULL, 0);

    vm_statistics64_data_t vmstat;
    mach_msg_type_number_t count = sizeof(vmstat) / sizeof(natural_t);
    if(KERN_SUCCESS != host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&vmstat, &count)){
        return false;
    }

    totalSize = physical_memory;
    availableSize = vmstat.free_count * vm_page_size;

    if (totalSize == 0) {
        return false;
    }

    usedPercent = ((totalSize - availableSize) * 100.0) / totalSize;

    // Apply unit conversion if necessary
    switch (unit) {
        case 'K': // KB
            totalSize /= 1024;
            availableSize /= 1024;
            break;
        case 'G': // GB
            totalSize /= (1024 * 1024 * 1024);
            availableSize /= (1024 * 1024 * 1024);
            break;
        case 'M': // MB
            totalSize /= (1024 * 1024);
            availableSize /= (1024 * 1024);
            break;
        default:
            break;
    }

    return true;
#elif TARGET_PLATFORM_WINDOWS
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memInfo)) {
        return false;
    }

    totalSize = memInfo.ullTotalPhys / (1024 * 1024); // Convert to MB
    availableSize = memInfo.ullAvailPhys / (1024 * 1024); // Convert to MB

    if (totalSize == 0) {
        return false;
    }

    usedPercent = ((totalSize - availableSize) * 100.0) / totalSize;

    // Apply unit conversion if necessary
    switch (unit) {
        case 'K': // KB
            totalSize *= 1024;
            availableSize *= 1024;
            break;
        case 'G': // GB
            totalSize /= 1024;
            availableSize /= 1024;
            break;
        // case 'M': // MB - no conversion necessary
        default:
            break;
    }

    return true;
#endif
}

// 获取系统CPU核数
int TC_Port::getCPUProcessor()
{
#if TARGET_PLATFORM_WINDOWS
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif TARGET_PLATFORM_IOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW;
    nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if (count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if (count < 1) { count = 1; }
    }
    return count;
#else
    return get_nprocs();
#endif
}

bool TC_Port::getDiskInfo(int64_t &totalSize, int64_t& availableSize, float& usedPercent, const string& path)
{
#if TARGET_PLATFORM_WINDOWS
    ULARGE_INTEGER freeBytesAvailableToCaller;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

    // std::wstring wpath(path.begin(), path.end());

    if (!GetDiskFreeSpaceEx(path.c_str(), &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        return false;
    }

    totalSize = totalNumberOfBytes.QuadPart;
    ULONGLONG freeSize = totalNumberOfFreeBytes.QuadPart;

    if (totalSize == 0) {
        return false;
    }

    availableSize = freeSize;
    usedPercent = (totalSize - freeSize) * 100.0 / totalSize;

    return true;
#else
    struct statfs64 buf;

    if(statfs64(path.c_str(),&buf)==-1)
    {
        return false;
    }

    totalSize = buf.f_blocks * buf.f_bsize;
    if (totalSize == 0)
    {
        return false;
    }

    availableSize = buf.f_bavail * buf.f_bsize;
    usedPercent = (totalSize - availableSize) * 1.0 / totalSize * 100;
    return true;
#endif
}

}