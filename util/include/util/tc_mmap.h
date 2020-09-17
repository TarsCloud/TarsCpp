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

#ifndef __TC_MMAP_H
#define __TC_MMAP_H

#include "util/tc_platform.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <sys/mman.h>
#endif

#include <string>
#include "util/tc_ex.h"
using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/** 
* @file tc_mmap.h 
* @brief  mmap封装类. 
*         windows下采用文件映射实现(似乎不回会写文件!!??)
* @brief  MMAP Encapsulation Class
*         Windows uses file mapping (does not seem to write files back!!?)
*  
*/           
/////////////////////////////////////////////////
/**
* @brief MMap异常
* @brief MMap Exception
*/
struct TC_Mmap_Exception : public TC_Exception
{
   TC_Mmap_Exception(const string &buffer) : TC_Exception(buffer){};
   TC_Mmap_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
   ~TC_Mmap_Exception() throw() {};
};


/**
 *  @brief  mmap的操作类.
 *  @brief  Mmap's operation class.
 *  
 *  说明:
 *  Notes:
 *   1: 创建map时,文件一定需要有length的长度, 否则可能导致越界
 *   1: When creating a map, the file must have a length of length, otherwise it may lead to a break.
 *   2: 2中说的情况一般通过产生空洞文件避免,int mmap(const char *file, size_t length);
 *   2: The situations described in 2 are generally avoided by generating a hollow file, int MMAP (const char *file, size_t length);
 *      
 *   实现了类似的封装, 推荐直接使用
 *   Similar encapsulation implemented, recommended for direct use
 */
class TC_Mmap
{
public:

    /**
	 * @brief 构造函数. 
     * @brief Constructor
	 *  
     * @param bOwner, 如果拥有, 则析够的时候unmap
     * @param bOwner, If you have, unmap is sufficient
     */
    TC_Mmap(bool bOwner = true);

    /**
     * @brief 析构
     * @brief Destructor
     */
    ~TC_Mmap();

    /**
	 * @brief 映射到进程空间，采用: 
     * @brief Map to process space using:
	 *  
	 * PROT_READ|PROT_WRITE,MAP_SHARED方式 
     * PROT_READ|PROT_WRITE,MAP_SHARED Method
	 *  
	 * 注意的文件大小会比length大一个字节(初始化时生成空洞文件的原因) 
     * Note that the file size is one byte larger than length (the reason for the empty file at initialization)
     * @param file    文件名
     * @param file    file name
     * @param length  映射文件的长度
     * @param length  Length of map file
     * @throws        TC_Mmap_Exception
     * @return
     */
	void mmap(const char *file, size_t length);

    /**
	 * @brief 解除映射关系, 解除后不能在访问这段空间了. 
     * @brief You can't access this space after unmapping.
	 *  
     * @throws TC_Mmap_Exception
     * @return
     */
	void munmap();

    /**
     * @brief 把共享内存中的改变写回磁盘中.
     * @brief Write changes in shared memory back to disk.
     *
     * @param bSync true:同步写回, false:异步写回
     * @param bSync true: synchronous writeback, false: asynchronous writeback
     * @throws      TC_Mmap_Exception
     * @return
     */
	void msync(bool bSync = false);

    /**
     * @brief 获取映射的指针地址.
     * @brief Get Mapped Pointer Address.
     *
     * @return char* 映射的指针地址
     * @return char* Mapped pointer address
     */
    char *getPointer() const    { return _pAddr; }

    /**
     * @brief 获取映射的空间大小.
     * @brief Get the space size of the map.
     *
     * @return size_t 映射的空间大小
     * @return size_t Space Size of Mapping
     */
    size_t getSize() const      { return _iLength; }

    /**
	 * @brief 是否创建出来的，文件的存在可分为两种形式： 
     * @brief Whether created or not, the existence of a file can be divided into two forms:
	 *  
	 *  	  一种是创建的，一种是开始就存在的
     *        One is created, the other is present from the beginning
     * @return 如果是创建出来的就返回true，否则返回false
     * @return Return true if created, false otherwise
     */
    bool iscreate() const       { return _bCreate; }

    /**
	 * @brief 设置是否拥有. 
     * @brief Set whether to own.
	 *  
     * @param bOwner ture or fale
     */
    void setOwner(bool bOwner)  { _bOwner = bOwner; }

protected:

    /**
     * 是否拥有
     * Owned or not
     */
    bool    _bOwner;

    /**
     * 映射到进程空间的地址
     * Address mapped to process space
     */
    char    *_pAddr;

    /**
     * 映射的空间大小
     * Space Size of Mapping
     */
    size_t  _iLength;

    /**
     * 是否创建出来的
     * Is it created
     */
    bool    _bCreate;

#if TARGET_PLATFORM_WINDOWS
    HANDLE  _hFile;

    HANDLE  _hMap;
#endif    
};

}
#endif

