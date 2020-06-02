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

#ifndef __TC_FIFO_H
#define __TC_FIFO_H

#include "util/tc_platform.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <string>

namespace tars
{

/////////////////////////////////////////////////
/** 
 *@file   tc_fifo.h
 *@brief  FIFO封装类. 
 *@brief  FIFO encapsulation class
 */
/////////////////////////////////////////////////

/** 
 *@brief  管道操作类. 
 *@brief  Pipeline operation.
 */
class TC_Fifo
{
public:

   /**
    * @brief 管道操作的枚举类型.  
    * 定义了对管道的操作类型，EM_WRITE：写管道，EM_READ ：读管道
    * @brief Enumeration types for pipeline operations
    * Defines the type of operation on the pipeline, EM_ Write: write pipe, EM_ Read: read pipe
    */
    enum ENUM_RW_SET
    {
        EM_WRITE = 1,
        EM_READ  = 2
    };

public:
   /**
    * @brief 构造函数. 
    * @brief Constructor
    *  
    * @param bOwener : 是否拥有管道，默认为ture
    * @param bOwener : Whether the pipeline is owned, the default is ture
    */
    TC_Fifo(bool bOwener = true);

   /**
    * @brief 析构函数. 
    * @brief Constructor
    */
    ~TC_Fifo();

public:
    /**
     * @brief 打开FIFO. 
     * @brief Open FIFO
     *  
     * @param sPath 要打开的FIFO文件的路径 
     * @param sPath the file path of the FIFO to open
     * @param enRW  管道操作类型
     * @param enRN  Pipe opration type
     * @param mode  该FIFO文件的权限 ，默认为可读可写
     * @param mode  The permissions of this FIFO file are read-write by default
     * @return      0-成功,-1-失败 
     * @return      0 - successfull, -1 - failed
     */
    int open(const std::string & sPath, ENUM_RW_SET enRW, mode_t mode = 0777);

    /**
     * @brief 关闭fifo 
     * @brief Close FIFO
     */
    void close();

    /**
     * @brief 获取FIFO的文件描述符. 
     * @brief Get the file descriptor of FIFO
     * 
     * @return  FIFO的文件描述符
     * @return  the file descriptor of FIFO
     */
    int fd() const { return _fd; }

    /**
     * @brief 读数据, 当读取成功时，返回实际读取的字节数，如果返回的值是0，代表已经读到文件的结束；小于0表示出现了错误
     * @brief read data, when read successfully, return the actual number of bytes read, if the value returned is 0, the end of the file has been read; less than 0 indicates an error 
     * @param buffer     读取的内容
     * @param buffer     the content to be read
     * @param max_size   读取数据的大小 
     * @param max_size   size of the data to be read
     * @return           读到的数据长度 ,小于0则表示失败
     * @return           the length of the read data , if it is less then 0, read failed
     */
    int read(char * szBuff, const size_t sizeMax);

    /**
     * @brief 向管道写数据. 
     * @brief Write data to the pipeline
     * 
     * @param szBuff       要写入的数据
     * @param szBuff       data to be wrote
     * @param sizeBuffLen  数据的大小
     * @param sizeBuffLen  size of data
     * @return             大于0：表示写了部分或者全部数据 
     *                        小于0：表示出现错误
     * @return             geater than 0: Some or all of the data are shown and written 
     *                     less than 0: Error
     */
    int write(const char * szBuff, const size_t sizeBuffLen);

private:
   /**
    * FIFO文件的路径
    * file path of FIFO
    */
    std::string        _sPathName;

   /**
    * 是否拥有FIFO
    * whether it have FIFO
    */
    bool            _bOwner;

   /**
    * FIFO的文件的操作类型
    * file opration type of FIFO
    */
    ENUM_RW_SET        _enRW;

   /**
    * FIFO的文件描述符
    * file descriptor of FIFO
    */
    int             _fd;
};

}

#endif

#endif

