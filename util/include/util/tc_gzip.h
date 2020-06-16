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

#ifndef __TC_GZIP_H
#define __TC_GZIP_H

#include  "util/tc_platform.h"

#include <string>
#include <vector>
#include <string.h>
#include <cassert>
#include <memory>

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/** 
* @file tc_gzip.h 
* @brief  gzip类, 封装的zlib库
* @brief  Gzip Class, Encapsulated zlib Library
*/

/////////////////////////////////////////////////

/**
* @brief 该类提供标准GZIP压缩和解压算法
* @brief This class provides standard GZIP compression and decompression algorithms
*/
class TC_GZip
{
protected:
	struct Output
	{
		virtual ~Output(){}
		virtual void operator ()(char *begin, size_t length) = 0;
	};

	template<typename T>
    struct OutputImp : public Output {
	    OutputImp(T & buffer) : _buffer(buffer)
        {
            _buffer.clear();
        }

        virtual void operator ()(char *begin, size_t length)
        {
            _buffer.insert(_buffer.end(), begin, begin + length);
        }

        T&  _buffer;
    };

public:
    /**
    * @brief  对数据进行压缩
    * @brief  Compress data
    *  
    * @param src         需要压缩的数据
    * @param src         Data that needs to be compressed
    * @param length      数据长度
    * @param length      Data length
    * @param buffer      输出buffer
    * @param buffer      output buffer 
    * @return bool       成功失败
    * @return bool       sucessfull or failed
    */
    static bool compress(const char *src, size_t length, vector<char>& buffer);

    /**
    * @brief  对数据进行压缩
    * @brief  Compress data
    *  
    * @param src         需要压缩的数据
    * @param src         data need to be compressed
    * @param length      数据长度
    * @param length      data length
    * @param buffer      输出buffer
    * @param buffer      output buffer
    * @return bool       成功失败
    * @return bool       sucessfull or failed
    */    
    static bool compress(const char *src, size_t length, string& buffer);
    
    /**
    * @brief  对数据进行解压
    * @brief  Unzip data
    *  
    * @param src         需要解压的数据
    * @param src         data need to be decompressed
    * @param length      数据长度
    * @param length      data length
    * @param buffer      输出buffer
    * @param buffer      output buffer
    * @return bool       成功失败
    * @return bool       sucessfull or failed
    */
    static bool uncompress(const char *src, size_t length, vector<char>& buffer)
    {
        std::unique_ptr<Output> output(new OutputImp<vector<char>>(buffer));

        return uncompress(src, length, output.get());
    }

    /**
    * @brief  对数据进行解压
    * @brief  Unzip data
    *  
    * @param src         需要解压的数据
    * @param src         data need to be decompressed
    * @param length      数据长度
    * @param length      data length
    * @param buffer      输出buffer
    * @param buffer      output buffer
    * @return bool       成功失败
    * @return bool       sucessfull or failed
    */
    static bool uncompress(const char *src, size_t length, string& buffer)
    {
	    std::unique_ptr<Output> output(new OutputImp<string>(buffer));

        return uncompress(src, length, output.get());
    }

    /**
    * @brief  对数据进行分片解压, 
    *         每次解压的数据调用Output输出
    * @brief  Decompress data in pieces, 
    *         Output output is invoked for each decompressed data call
    *  
    * @param src         需要解压的数据
    * @param src         data need to be decompressed
    * @param length      数据长度
    * @param length      data length
    * @param o           输出buffer的函数对象 
    * @param o           function project for output buffer
    *                    struct Output
    *                    {
    *                        void operator()(char *begin, size_t
    *                      length);
    *                      }
    * @return bool       成功失败
    * @return bool       sucessfull or failed
    */
    static bool uncompress(const char *src, size_t length, Output* o);
};

}
//#endif
#endif
