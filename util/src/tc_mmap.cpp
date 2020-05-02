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

#include "util/tc_mmap.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <cerrno>
#include <cassert>
#include <iostream>
#include "util/tc_file.h"

namespace tars
{

TC_Mmap::TC_Mmap(bool bOwner)
: _bOwner(bOwner)
, _pAddr(NULL)
, _iLength(0)
, _bCreate(false)
{
#if TARGET_PLATFORM_WINDOWS    
    _hFile = NULL;
    _hMap  = NULL;
#endif    
}

TC_Mmap::~TC_Mmap()
{
    if(_bOwner)
    {
        munmap();
    }
}

void TC_Mmap::mmap(const char *file, size_t length)
{
    assert(length > 0);
    if(_bOwner)
    {
        munmap();
    }
#if TARGET_PLATFORM_WINDOWS
    if(_hFile != NULL)
    {
        CloseHandle(_hFile);
    }
    if(_hMap != NULL)
    {
        CloseHandle(_hMap);
    }
    _hFile = CreateFile(file, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN|FILE_FLAG_WRITE_THROUGH|FILE_FLAG_NO_BUFFERING, NULL);
    if(_hFile == INVALID_HANDLE_VALUE)
    {
        THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::mmap] fopen file '" + string(file) + "' error");
    }

	_hMap = CreateFileMapping(_hFile, NULL, PAGE_READWRITE, 0, length, NULL);

    if (_hMap != NULL)
    {
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			_bCreate = false;
		else
			_bCreate = true;
    }
    else
    {
		THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::mmap] fopen file '" + string(file) + "' error");
    }

    _pAddr = (char*)MapViewOfFile(_hMap, FILE_MAP_ALL_ACCESS, 0, 0, length);
//	void *pAddr = MapViewOfFile(_hMap, FILE_MAP_ALL_ACCESS, 0, 0, length);

	if (_pAddr == NULL)
	{
		CloseHandle(_hFile);
		CloseHandle(_hMap);
		THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::mmap] fopen file '" + string(file) + "' error");
	}
#else
    //注意_bCreate的赋值位置:保证多线程用一个对象的时候也不会有问题
    int fd = open(file, O_CREAT|O_EXCL|O_RDWR, 0666);
    if(fd == -1)
    {
        if(errno != EEXIST)
        {
            THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::mmap] fopen file '" + string(file) + "' error");
        }
        else
        {
            fd = open(file, O_CREAT|O_RDWR, 0666);
            if(fd == -1)
            {
                THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::mmap] fopen file '" + string(file) + "' error");
            }
            _bCreate = false;
        }
    }
    else
    {
        _bCreate = true;
    }

    if(_bCreate)
    {
        //避免空洞文件
        lseek(fd, length-1, SEEK_SET);
        write(fd,"\0",1);
    }

    _pAddr      = static_cast<char*>(::mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    if(_pAddr == (void*)-1)
    {
        _pAddr = NULL;
        close(fd);
        THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::mmap] mmap file '" + string(file) + "' error");
    }
    if(fd != -1)
    {
       close(fd); 
    }
#endif    

    _iLength    = length;

    if(_bCreate)
    {
        _pAddr[length-1] = '\0';
    }

}

void TC_Mmap::munmap()
{
    if(_pAddr == NULL)
    {
        return;
    }
#if TARGET_PLATFORM_WINDOWS
	BOOL ret = FlushViewOfFile(_pAddr, 0);
    if(!ret)
    {
        THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::munmap] munmap error");
    }
    UnmapViewOfFile(_pAddr);

    CloseHandle(_hFile);
	_hFile = NULL;

    CloseHandle(_hMap);
    _hMap = NULL;
#else   
    int ret = ::munmap(_pAddr, _iLength);
    if(ret == -1)
    {
        THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::munmap] munmap error");
    }
#endif
    _pAddr      = NULL;
    _iLength    = 0;
    _bCreate   = false;
}

void TC_Mmap::msync(bool bSync)
{
    int ret = 0;
#if TARGET_PLATFORM_WINDOWS  
    FlushViewOfFile(_pAddr, _iLength);
#else  
    if(bSync)
    {
        ret = ::msync(_pAddr, _iLength, MS_SYNC | MS_INVALIDATE);
    }
    else
    {
        ret = ::msync(_pAddr, _iLength, MS_ASYNC | MS_INVALIDATE);
    }
#endif    
    if(ret != 0)
    {
        THROW_EXCEPTION_SYSCODE(TC_Mmap_Exception, "[TC_Mmap::msync] msync error");
    }
}

}
