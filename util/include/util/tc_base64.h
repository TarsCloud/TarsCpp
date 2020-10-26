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

#ifndef __TC_BASE64_H
#define __TC_BASE64_H

#include <string>

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/** 
* @file tc_base64.h
* @brief base64 Codec Class
* @brief base64编解码类. 
*/

/////////////////////////////////////////////////

/**
* @brief This class provides standard base64 encoding and decoding methods.
* @brief 该类提供标准的base64的编码解码 
*/
class TC_Base64
{
public:
    /**
    * @brief Encode the string by base64
    * @brief 对字符串进行base64编码. 
    * 
    * @param data         Data to be encoded
    * @param bChangeLine  Add line breaks in the final encoded data if necessary,
    *                     (In the RFC, it is recommended to add an 'Enter' after every 76 characters. 
    *                      No newline is added by default.)
    * @return string      the encoded data
    * 
    * @param data         需要编码的数据
    * @param bChangeLine  是否需要在最终编码数据加入换行符 ，
    *                     (RFC中建议每76个字符后加入回车换行，默认为不添加换行
    * @return string      编码后的数据
    */
    static string encode(const string &data, bool bChangeLine = false);
    
    /**
    * @brief Decode the string by base64
    * @brief 对字符串进行base64解码. 
    * 
    * @param data     Data to be decoded
    * @return string  the decoded data
    * 
    * @param data     需要解码的数据
    * @return string  解码后的数据
    */    
    static string decode(const string &data);

    /**
    * @brief  Encode the string by base64 
    * @brief  对字符串进行base64编码.   
    * 
    * @param buffer       buffer pointer
    * @param length       length
    * @param bChangeLine  Add line breaks in the final encoded data if necessary,
    *                     (In the RFC, it is recommended to add an 'Enter' after every 76 characters. 
    *                      No newline is added by default.)
    * @return string      the encoded data
    * 
    * @param buffer       buffer指针
    * @param length       长度
    * @param bChangeLine  是否需要在最终编码数据加入换行符 ，
    *                     (RFC中建议每76个字符后加入回车换行，默认为不添加换行
    * @return string      编码后的数据
    */
    static string encode(const char *buffer, size_t length, bool bChangeLine = false);

    /**
    * @brief  Encode the string by base64 
    * @brief  对字符串进行base64编码 . 
    * 
    * @param pSrc        the data to be encoded
    * @param nSrcLen     the length of the data to be encoded
    * @param pDst        the encoded data      
    * @param bChangeLine Add line breaks in the final encoded data if necessary,
    *                     (In the RFC, it is recommended to add an 'Enter' after every 76 characters. 
    *                      No newline is added by default.)
    * @return            the length of the encoded string
    *  
    * @param pSrc        需要编码的数据
    * @param nSrcLen     需要编码的数据长度
    * @param pDst        编码后的数据      
    * @param bChangeLine 是否需要在最终编码数据加入换行符， 
    *                    RFC中建议每76个字符后加入回车换行，默认为不添加换行
    * @return            编码后的字符串的长度
    */
    static int encode(const unsigned char* pSrc, size_t nSrcLen, char* pDst, bool bChangeLine = false);
    
    /**
    * @brief Decode the string by base64
    * @brief 对字符串进行base64解码
    * 
    * @param pSrc    the data to be decoded    
    * @param nSrcLe  the length of the data to be decoded
    * @param pDst    the decoded data
    * @return        the length of the decoded string
    * 
    * @param pSrc    需要解码的数据    
    * @param nSrcLe  需要解码的数据长度
    * @param pDst   解码后的数据
    * @return       解码后的字符串的长度
    */    
    static int decode(const char* pSrc, size_t nSrcLen, unsigned char* pDst);
    
protected:

    /**
    *  base64 Code Table
    *  base64编码表
    */
    static const char EnBase64Tab[];
    /**
    *  base64 Decode Table
    *  base64解码表
    */
    static const char DeBase64Tab[];
};

}
#endif
