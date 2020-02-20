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

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
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

int TC_Port::mkdir(const char *path, mode_t mode)
{
#if TARGET_PLATFORM_WINDOWS
	int iRetCode = ::_mkdir(path);
#else
	int iRetCode = ::mkdir(path, mode);
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

}