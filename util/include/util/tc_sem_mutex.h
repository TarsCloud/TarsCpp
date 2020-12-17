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

#ifndef __TC_SEM_MUTEX_H
#define __TC_SEM_MUTEX_H

#include "util/tc_platform.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#endif

#include "util/tc_lock.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
* @file tc_sem_mutex.h 
* @brief 进程间的读写锁
* linux下采用信号量来实现
* windows下采用Mutex来实现(key相同即可实现不同进程间的同步)
*  
* @author ruanshudong@qq.com 
*/              
/////////////////////////////////////////////////

/**
* @brief 信号量锁异常类
*/
struct TC_SemMutex_Exception : public TC_Exception
{
    TC_SemMutex_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_SemMutex_Exception() throw() {};
};

//锁名称统一用数字类型
#if TARGET_PLATFORM_WINDOWS
typedef int key_t;
#endif
/**
* @brief 进程间锁, 提供两种锁机制:共享锁和排斥锁. 
*  
* 1 对于相同的key, 不同进程初始化时连接到相同的sem上
* 2 采用IPC的信号量实现
* 3 信号量采用了SEM_UNDO参数, 当进程结束时会自动调整信号量
*/
class TC_SemMutex
{
public:
    /**
     * @brief 构造函数
     */
    TC_SemMutex();

    /**
     * @brief 析构函数
     */
    ~TC_SemMutex();

    /**
	* @brief 构造函数. 
	*  
    * @param iKey, key
    * @throws TC_SemMutex_Exception
    */
    TC_SemMutex(key_t iKey);

    /**
	* @brief 初始化. 
	*  
    * @param iKey, key
    * @throws TC_SemMutex_Exception
    * @return 无
     */
    void init(key_t iKey);

    /**
	* @brief 获取共享内存Key. 
	*  
    * @return key_t ,共享内存key
    */
    key_t getkey() const {return _semKey;}

    /**
    * @brief 获取共享内存ID. 
    *  
    * @return int ,共享内存Id
    */
    int getid() const   {return _semID;}

    /**
    * @brief 加读锁.
    * 
    *@return int
    */
    void rlock() const;

    /**
	* @brief 解读锁. 
	*  
    * @return int
    */
    void unrlock() const;

    /**
	* @brief 尝试读锁. 
	*  
    * @return bool : 加锁成功则返回false, 否则返回false
    */
    bool tryrlock() const;

    /**
	* @brief 加写锁. 
	*  
    * @return int
    */
    void wlock() const;

    /**
    * @brief 解写锁
    */
    void unwlock() const;

    /**
	* @brief 尝试写锁. 
	*  
    * @throws TC_SemMutex_Exception
    * @return bool : 加锁成功则返回false, 否则返回false
    */
    bool trywlock() const;

    /**
	* @brief 写锁. 
	*  
    * @return int, 0 正确
    */
    void lock() const        {wlock();};

    /**
    * @brief 解写锁
    */
    void unlock() const      {unwlock();};

    /**
	* @brief  尝试解锁. 
	*  
    * @throws TC_SemMutex_Exception
    * @return int, 0 正确
    */
    bool trylock() const    {return trywlock();};

#if TARGET_PLATFORM_WINDOWS
protected:
    void addWriter() const;
    void removeWriter() const;
    void unlockImp() const;
    DWORD tryReadLockOnce() const ;
protected:
 
    mutable HANDLE   _mutex;  
    mutable HANDLE   _readEvent;  
    mutable HANDLE   _writeEvent;  
    mutable unsigned _readers;  
    mutable unsigned _writersWaiting;  
    mutable unsigned _writers; 
#endif 

   /**
     * 信号量ID
     */
    int _semID;

    /**
     * 信号量key
     */
    key_t _semKey;

};

}

#endif

//#endif
