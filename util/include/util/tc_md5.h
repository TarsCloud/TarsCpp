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
#ifndef __TC_MD5_H
#define __TC_MD5_H

#include <string>
#include "util/tc_ex.h"
#include "util/tc_common.h"

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/**
 * @file tc_md5.h
 * @brief md5类(修改并完善至md5的c版本)
 * @brief MD5 class (is modified and improved to version C of MD5)
 * 
 */            
/////////////////////////////////////////////////

/**
* @brief 该类提供MD5的散列算法，通过静态函数提供 . 
* @brief This class provides MD5 hash algorithm through static function
*  
* 提供两种输出方式:字符串(32个字符)或二进制(16个字节)
* Two output modes are provided: string (32 characters) or binary (16 bytes)
*/
#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}
#endif

#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}
#endif

/**
 * @brief md5异常. 
 * @brief MD5 exception
 */
struct TC_MD5_Exception : public TC_Exception
{
    TC_MD5_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_MD5_Exception() throw(){};
};

class TC_MD5
{
    typedef unsigned char *POINTER;
    typedef unsigned short int UINT2;
    //typedef unsigned long int UINT4;
    typedef uint32_t UINT4;

    typedef struct 
    {
        /**
        * state (ABCD)
        */
        //unsigned long state[4];        
        UINT4 state[4];        

        /**
        * number of bits, modulo 2^64 (lsb first)
        */
        //unsigned long count[2];        
        UINT4 count[2];        

        /**
        * input buffer
        */
        unsigned char buffer[64];      
    }MD5_CTX;

public:
    /**
    * @brief 对字符串进行md5处理,返回16字节二进制数据. 
    * @brief MD5 string processing, return 16 bytes of binary data
    *  
    * @param sString  输入字符串
    * @param sString  Input string
    * @return string 输出,16个字节的二进制数据
    * @return string Output, 16 bytes of binary data
    */
    static vector<char> md5bin(const string &sString);
    static vector<char> md5bin(const char *buffer, size_t length);

    /**
    * @brief 对字符串进行md5处理 ，
    *        将md5的二进制数据转换成HEX的32个字节的字符串
    * @brief MD5 is used to process strings, 
    *        and the binary data of MD5 is converted into 32 byte strings of hex
    * @param sString  输入字符串
    * @param sString  Input string
    * @return string 输出,32个字符
    * @return string Output, 32 characters
    */
    static string md5str(const string &sString);
    static string md5str (const char *buffer, size_t length);

    /**
     * @brief 对文件进行md5处理. 
     * @brief MD5 processing of files.
     *  
     * @param fileName 要处理的文件名 
     * @param fileName File name to process
     * @throw TC_MD5_Exception, 文件读取错误会抛出异常 
     * @throw TC_MD5_Exception, File read error throws an exception
     * @return string  处理后的字符串
     * @return string  Processed string
     */
    static string md5file(const string& fileName);

protected:

    static string bin2str(const void *buf, size_t len, const string &sSep);

    /**
    * @brief MD5 init.
    *  
    * @param context 上下文信息
    * @param context Context information
    * @return
    */ 
    static void md5init(MD5_CTX *context);
    
    /** 
    * @brief MD5 block update operation，Continues an MD5 
    * message-digest operation, processing another message block, 
    * and updating the context 
    * @param context  上下文信息
    * @param context  Context information
    * @param input    输入
    * @param input    input
    * @param inputLen 输入长度
    * @param inputLen input length
    * @return 
    */    
    static void md5update (MD5_CTX *context, unsigned char *input,unsigned int inputLen);    
    
    /** 
    * @brief  MD5 finalization，Ends an MD5 message-digest 
    * operation, writing the message digest and zeroizing the 
    * context 
    * @param digest   摘要
    * @param digest   abstract
    * @param context 上下文信息
    * @param context context info
    */
    static void md5final (unsigned char digest[16], MD5_CTX *context);
    
    /** 
    * @brief  MD5 basic transformation，Transforms state based on 
    *         block
    * @param state 状态
    * @param state state
    * @param block : ...
    */
    static void md5_process( MD5_CTX *ctx, const unsigned char data[64]);
    
    /** 
    * @brief  Encodes input (UINT4) into output (unsigned 
    *         char)，Assumes len is a multiple of 4
    * @param output 输出
    * @param output output
    * @param input  输入
    * @param input  input
    * @param len    输入长度
    * @param len    input length
    */    
    static void encode (unsigned char *output,UINT4 *input,unsigned int len);
    
    /** 
    * @brief Decodes input (unsigned char) into output (UINT4)， 
    * Assumes len is a multiple of 4
    * @param output 输出
    * @param output output
    * @param input  输入
    * @param input  input
    * @param len    输入长度
    * @param len    input length
    */    
    static void decode (UINT4 *output,unsigned char *input,unsigned int len);
    
    /** 
    * @brief replace "for loop" with standard memcpy if possible. 
    *  
    * @param output  输出
    * @param output  output
    * @param input   输入
    * @param input   input
    * @param len     输入长度
    * @param len     input length
    */    
    static void md5_memcpy (POINTER output,POINTER input,unsigned int len);
    
    /** 
    * @brief replace "for loop" with standard memset if possible. 
    *  
    * @param output 输出
    * @param output output
    * @param value  值
    * @param value  value
    * @param len    输入长度
    * @param len    input length
    */ 
    static void md5_memset (POINTER output,int value,unsigned int len);
    
    /**
    * 填充值
    * fill value
    */
    static unsigned char PADDING[64];
};

}
#endif
