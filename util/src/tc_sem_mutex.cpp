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

#include <errno.h>
#include <string.h>
#include "util/tc_common.h"
#include "util/tc_sem_mutex.h"

namespace tars
{
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS

TC_SemMutex::TC_SemMutex():_semID(-1),_semKey(-1)
{

}

TC_SemMutex::TC_SemMutex(key_t iKey)
{
    init(iKey);
}

TC_SemMutex::~TC_SemMutex()
{
}
void TC_SemMutex::init(key_t iKey)
{
    #if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
    /* union semun is defined by including <sys/sem.h> */
    #else
    /* according to X/OPEN we have to define it ourselves */
    union semun
    {
         int val;                  /* value for SETVAL */
         struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
         unsigned short *array;    /* array for GETALL, SETALL */
                                   /* Linux specific part: */
         struct seminfo *__buf;    /* buffer for IPC_INFO */
    };
    #endif

    int  iSemID;
    union semun arg;
    u_short array[2] = { 0, 0 };

    //生成信号量集, 包含两个信号量
    if ( (iSemID = semget( iKey, 2, IPC_CREAT | IPC_EXCL | 0666)) != -1 )
    {
        arg.array = &array[0];

        //将所有信号量的值设置为0
        if ( semctl( iSemID, 0, SETALL, arg ) == -1 )
        {
            THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::init] semctl error");
            // throw TC_SemMutex_Exception("[TC_SemMutex::init] semctl error", TC_Exception::getSystemCode());
        }
    }
    else
    {
        //信号量已经存在
        if ( errno != EEXIST )
        {
            THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::init] sem has exist error");
            // throw TC_SemMutex_Exception("[TC_SemMutex::init] sem has exist error", TC_Exception::getSystemCode());
        }

        //连接信号量
        if ( (iSemID = semget( iKey, 2, 0666 )) == -1 )
        {
            THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::init] connect sem error");
            // throw TC_SemMutex_Exception("[TC_SemMutex::init] connect sem error", TC_Exception::getSystemCode());
        }
    }

    _semKey = iKey;
    _semID  = iSemID;
}

void TC_SemMutex::rlock() const
{
    //进入共享锁, 第二个信号量的值表示当前使用信号量的进程个数
    //等待第一个信号量变为0(排他锁没有使用)
    //占用第二个信号量(第二个信号量值+1, 表示被共享锁使用)
    struct sembuf sops[2] = { {0, 0, SEM_UNDO}, {1, 1, SEM_UNDO} };
    size_t nsops = 2;
    int ret = -1;

    do
    {
        ret=semop(_semID,&sops[0],nsops);

    } while ((ret == -1) &&(errno==EINTR));

    // return ret;

    //return semop( _semID, &sops[0], nsops);
}

void TC_SemMutex::unrlock( ) const
{
    //解除共享锁, 有进程使用过第二个信号量
    //等到第二个信号量可以使用(第二个信号量的值>=1)
    struct sembuf sops[1] = { {1, -1, SEM_UNDO} };
    size_t nsops = 1;

    int ret = -1;

    do
    {
        ret=semop(_semID,&sops[0],nsops);

    } while ((ret == -1) &&(errno==EINTR));

    // return ret;

    //return semop( _semID, &sops[0], nsops);
}

bool TC_SemMutex::tryrlock() const
{
    struct sembuf sops[2] = { {0, 0, SEM_UNDO|IPC_NOWAIT}, {1, 1, SEM_UNDO|IPC_NOWAIT}};
    size_t nsops = 2;

    int iRet = semop( _semID, &sops[0], nsops );
    if(iRet == -1)
    {
        if(errno == EAGAIN)
        {
            //无法获得锁
            return false;
        }
        else
        {
            THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::tryrlock] semop error");
            // throw TC_SemMutex_Exception("[TC_SemMutex::tryrlock] semop error", TC_Exception::getSystemCode());
        }
    }
    return true;
}

void TC_SemMutex::wlock() const
{
    //进入排他锁, 第一个信号量和第二个信号都没有被使用过(即, 两个锁都没有被使用)
    //等待第一个信号量变为0
    //等待第二个信号量变为0
    //释放第一个信号量(第一个信号量+1, 表示有一个进程使用第一个信号量)
    struct sembuf sops[3] = { {0, 0, SEM_UNDO}, {1, 0, SEM_UNDO}, {0, 1, SEM_UNDO} };
    size_t nsops = 3;

    int ret = -1;

    do
    {
        ret=semop(_semID,&sops[0],nsops);

    } while ((ret == -1) &&(errno==EINTR));

    // return ret;
    //return semop( _semID, &sops[0], nsops);
}

void TC_SemMutex::unwlock() const
{
    //解除排他锁, 有进程使用过第一个信号量
    //等待第一个信号量(信号量值>=1)
    struct sembuf sops[1] = { {0, -1, SEM_UNDO} };
    size_t nsops = 1;

    int ret = -1;

    do
    {
        ret=semop(_semID,&sops[0],nsops);

    } while ((ret == -1) &&(errno==EINTR));

    // return ret;

    //return semop( _semID, &sops[0], nsops);

}

bool TC_SemMutex::trywlock() const
{
    struct sembuf sops[3] = { {0, 0, SEM_UNDO|IPC_NOWAIT}, {1, 0, SEM_UNDO|IPC_NOWAIT}, {0, 1, SEM_UNDO|IPC_NOWAIT} };
    size_t nsops = 3;

    int iRet = semop( _semID, &sops[0], nsops );
    if(iRet == -1)
    {
        if(errno == EAGAIN)
        {
            //无法获得锁
            return false;
        }
        else
        {
            THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::trywlock] semop error");
            // throw TC_SemMutex_Exception("[TC_SemMutex::trywlock] semop error", TC_Exception::getSystemCode());
        }
    }

    return true;
}

#else

TC_SemMutex::TC_SemMutex():_readers(0), _writersWaiting(0), _writers(0) 
{
}

TC_SemMutex::TC_SemMutex(key_t iKey)
{
    init(iKey);
}
TC_SemMutex::~TC_SemMutex()
{
    if(_mutex != NULL)
    {
        CloseHandle(_mutex);  
        _mutex = NULL;
    }
    if(_readEvent!= NULL)
    {
        CloseHandle(_readEvent);  
        _readEvent = NULL;
    }
    if(_writeEvent != NULL)
    {
        CloseHandle(_writeEvent); 
        _writeEvent = NULL;
    }
}
void TC_SemMutex::init(key_t iKey)
{
    string key = "tc-mutex-" + TC_Common::tostr(iKey);
    string rkey = "tc-readEvent-" + TC_Common::tostr(iKey);
    string wkey = "tc-writeEvent-" + TC_Common::tostr(iKey);
    _mutex = CreateMutex(NULL, FALSE, key.c_str());  
    if (_mutex == NULL)  
    {
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::init] CreateMutex error");
    }
    _readEvent = CreateEvent(NULL, TRUE, TRUE, rkey.c_str());  
    if (_readEvent == NULL)  
    {
        CloseHandle(_mutex); 
        _mutex = NULL;
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::init] CreateEvent error");
    }  
    _writeEvent = CreateEvent(NULL, TRUE, TRUE, wkey.c_str());  
    if (_writeEvent == NULL)  
    {
        CloseHandle(_mutex); 
        _mutex = NULL;
        CloseHandle(_readEvent); 
        _readEvent = NULL;
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::init] CreateEvent error");
    }   
    _semKey = iKey; 
}
void TC_SemMutex::addWriter() const
{  
    switch (WaitForSingleObject(_mutex, INFINITE))  
    {  
    case WAIT_OBJECT_0:  
        if (++_writersWaiting == 1)   
            ResetEvent(_readEvent);  
        ReleaseMutex(_mutex);  
        break;  
    default:  
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::addWriter] WaitForSingleObject error");
    }  
}  
  
void TC_SemMutex::removeWriter() const 
{  
    switch (WaitForSingleObject(_mutex, INFINITE))  
    {  
    case WAIT_OBJECT_0:  
        if (--_writersWaiting == 0 && _writers == 0)   
            SetEvent(_readEvent);  
        ReleaseMutex(_mutex);  
        break;  
    default:  
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::removeWriter] WaitForSingleObject error");
    }
}  

void TC_SemMutex::rlock() const
{
    HANDLE h[2];  
    h[0] = _mutex;  
    h[1] = _readEvent;  
    switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))  
    {  
    case WAIT_OBJECT_0:  
    case WAIT_OBJECT_0 + 1:  
        ++_readers;  
        ResetEvent(_writeEvent);  
        ReleaseMutex(_mutex);  
        assert(_writers == 0);  
        break;  
    default:  
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::rlock] WaitForSingleObject error");
    }  
}

void TC_SemMutex::unrlock( ) const
{
    unlockImp();
}

bool TC_SemMutex::tryrlock() const
{
    for (;;)  
    {  
        if (_writers != 0 || _writersWaiting != 0)  
            return false;  
  
        DWORD result = tryReadLockOnce();  
        switch (result)  
        {  
        case WAIT_OBJECT_0:  
        case WAIT_OBJECT_0 + 1:  
            return true;  
        case WAIT_TIMEOUT:  
            continue;  
        default:  
            THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::tryrlock] WaitForSingleObject error");
        }  
    } 
}

void TC_SemMutex::wlock() const
{
    addWriter();  
    HANDLE h[2];  
    h[0] = _mutex;  
    h[1] = _writeEvent;  
    switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))  
    {  
    case WAIT_OBJECT_0:  
    case WAIT_OBJECT_0 + 1:  
        --_writersWaiting;  
        ++_readers;  
        ++_writers;  
        ResetEvent(_readEvent);  
        ResetEvent(_writeEvent);  
        ReleaseMutex(_mutex);  
        assert(_writers == 1);  
        break;  
    default:  
        removeWriter();  
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::wlock] WaitForSingleObject error");
    }  
}

void TC_SemMutex::unwlock() const
{
    unlockImp();
}

bool TC_SemMutex::trywlock() const
{
    addWriter();  
    HANDLE h[2];  
    h[0] = _mutex;  
    h[1] = _writeEvent;  
    switch (WaitForMultipleObjects(2, h, TRUE, 1))  
    {  
    case WAIT_OBJECT_0:  
    case WAIT_OBJECT_0 + 1:  
        --_writersWaiting;  
        ++_readers;  
        ++_writers;  
        ResetEvent(_readEvent);  
        ResetEvent(_writeEvent);  
        ReleaseMutex(_mutex);  
        assert(_writers == 1);  
        return true;  
    case WAIT_TIMEOUT:  
    default:  
        removeWriter();  
    }  
    return false;  
}

void TC_SemMutex::unlockImp() const 
{  
    switch (WaitForSingleObject(_mutex, INFINITE))  
    {  
    case WAIT_OBJECT_0:  
        _writers = 0;  
        if (_writersWaiting == 0) SetEvent(_readEvent);  
        if (--_readers == 0) SetEvent(_writeEvent);  
        ReleaseMutex(_mutex);  
        break;  
    default:  
        THROW_EXCEPTION_SYSCODE(TC_SemMutex_Exception, "[TC_SemMutex::unlockImp] WaitForSingleObject error");
    }  
}  

DWORD TC_SemMutex::tryReadLockOnce() const 
{  
    HANDLE h[2];  
    h[0] = _mutex;  
    h[1] = _readEvent;  
    DWORD result = WaitForMultipleObjects(2, h, TRUE, 1);   
    switch (result)  
    {  
    case WAIT_OBJECT_0:  
    case WAIT_OBJECT_0 + 1:  
        ++_readers;  
        ResetEvent(_writeEvent);  
        ReleaseMutex(_mutex);  
        assert(_writers == 0);  
        return result;  
    case WAIT_TIMEOUT:
    default:  
        ;
    }  
    return result;  
}  

#endif
}


