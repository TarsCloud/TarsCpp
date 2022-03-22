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
#endif

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <signal.h>
#include <limits.h>
#include <sys/time.h>
#else

#pragma comment(lib, "ws2_32.lib")
// #include <winsock.h>
#include <time.h>
#include <sys/timeb.h>
#include "util/tc_strptime.h"
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

string TC_Port::exec(const char *cmd)
{
	string err;
	return exec(cmd, err);
}

std::string TC_Port::exec(const char* cmd, std::string &err)
{
	string fileData;
#if TARGET_PLATFORM_WINDOWS
    FILE* fp = _popen(cmd, "r");
#else
    FILE* fp = popen(cmd, "r");
#endif
	if(fp == NULL) {
		err = "open '" + string(cmd) + "' error";
		return "";
	}
    static size_t buf_len = 2 * 1024 * 1024;
    char *buf = new char[buf_len];
    memset(buf, 0, buf_len);
    fread(buf, sizeof(char), buf_len - 1, fp);
#if TARGET_PLATFORM_WINDOWS
    _pclose(fp);
#else
    pclose(fp);
#endif
    fileData = string(buf);
    delete []buf;

	return fileData;
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
	
	TCHAR p[1024];
	strncpy_s(p, sizeof(p) / sizeof(TCHAR), command.c_str(), command.length());
	
	STARTUPINFO si;
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
		pwdCStr, //    指定子进程的工作路径
		&si, // 决定新进程的主窗体如何显示的STARTUPINFO结构体
		&pi  // 接收新进程的识别信息的PROCESS_INFORMATION结构体
	))
	{
		string err = TC_Exception::parseError(TC_Exception::getSystemCode());
	
		throw TC_Port_Exception("[TC_Port::forkExec] CreateProcessA exception:" + err);
	}
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	
	return pi.dwProcessId;

#else
	vector<string> vArgs;
	vArgs.push_back(sExePath);
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
		throw TC_Port_Exception("[TC_Port::forkExec] fork exception");
	}

	if (pid == 0)
	{
		int maxFd = static_cast<int>(sysconf(_SC_OPEN_MAX));
		for (int fd = 3; fd < maxFd; ++fd)
		{
			close(fd);
		}

		//server stdcout 日志在滚动日志显示
		if (!sRollLogPath.empty())
		{
			TC_File::makeDirRecursive(TC_File::extractFilePath(sRollLogPath));
#if TARGET_PLATFORM_IOS
			if ((freopen(sRollLogPath.c_str(), "ab", stdout)) != NULL && (freopen(sRollLogPath.c_str(), "ab", stderr)) != NULL)
#else
			if ((freopen64(sRollLogPath.c_str(), "ab", stdout)) != NULL && (freopen64(sRollLogPath.c_str(), "ab", stderr)) != NULL)
#endif
			{
				cout << argv[0] << " redirect stdout and stderr  to " << sRollLogPath << endl;
			}
			else
			{
				//重定向失败 直接退出
				exit(0);
			}
		}
		else
		{
			cout << argv[0] << " cannot redirect stdout and stderr  to log file sRollLogPath is empty" << endl;
		}

//		for_each(vEnvs.begin(), vEnvs.end(), EnvVal());

		if (strlen(pwdCStr) != 0)
		{
			if (chdir(pwdCStr) == -1)
			{
				cerr << argv[0] << " cannot change working directory to " << pwdCStr << "|errno=" << errno << endl;
			}
		}

		if (execvp(argv[0], argv) == -1)
		{
			cerr << "cannot execute " << argv[0] << "|errno=" << strerror(errno) << endl;
		}
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

shared_ptr<TC_Port::SigInfo> TC_Port::_sigInfo = std::make_shared<TC_Port::SigInfo>();

size_t TC_Port::registerSig(int sig, std::function<void()> callback)
{
	std::lock_guard<std::mutex> lock(_sigInfo->_mutex);

	auto it = _sigInfo->_callbacks.find(sig);

	if(it == _sigInfo->_callbacks.end())
	{
		//没有注册过, 才注册
		registerSig(sig);
	}

	size_t id = ++_sigInfo->_callbackId;

	_sigInfo->_callbacks[sig][id] = callback;

	return id;
}

void TC_Port::unregisterSig(int sig, size_t id)
{
	//注意_sigInfo是全局静态的, 有可能已经析构了, 需要特殊判断一下!
	if(_sigInfo && _sigInfo.use_count() > 0)
	{
		std::lock_guard<std::mutex> lock(_sigInfo->_mutex);
		auto it = _sigInfo->_callbacks.find(sig);

		if(it != _sigInfo->_callbacks.end())
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
					   	std::lock_guard<std::mutex> lock(_sigInfo->_mutex);

					   	auto it = TC_Port::_sigInfo->_callbacks.find(sig_no);
					   	if (it != TC_Port::_sigInfo->_callbacks.end())
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
					   	std::lock_guard<std::mutex> lock(_sigInfo->_mutex);

						   auto it = _sigInfo->_callbacks.find(dwCtrlType);
						   if (it != _sigInfo->_callbacks.end())
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


}
