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

#ifndef __TC_ENCODER_H_
#define __TC_ENCODER_H_

#include <vector>

#include "util/tc_ex.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
* @file tc_encoder.h 
* @brief  转码类 
* @brief  Transcoder Class
* 
* 
*/
/////////////////////////////////////////////////

/**
*  @brief  转码异常类
*  @brief  Transcoder Exception Class
*/
struct TC_Encoder_Exception : public TC_Exception
{
    TC_Encoder_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_Encoder_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_Encoder_Exception() throw(){};
};

/**
*  @brief 该类提供基本常用编码的转换.
*  @brief This class provides the transformation of basic common codes.
*  
*         Gbk到utf8之间的相互转码函数，通过静态函数提供.
*		  Transcoding functions between GBK and UTF8 are provided through static functions.
*  
*         1：GBK ==> UTF8的转换 
*         1: The transformation from GBK to UTF8
*  
*         2：UTF8 ==> GBK的转换
*         2: The transformation from UTF8 to GBK
*/
class TC_Encoder
{
public:

	/**
	 * @brief 编码转换过程中的模式,转换模式只在LINUX中生效，WINDOWS环境
	 * 下可忽略该参数
	 */
	typedef enum{
		//一般模式，当遇到未知字符时会抛出异常
		ICONV_NORMAL,
		//转换模式，当遇到未知字符时，会查找目标字符集中相似的字符来替代
		ICONV_TRANSLIT,
		//忽略模式，当遇到未知字符是，会跳过该字符，继续转换下一字符
		ICONV_IGNORE
	}ICONV_MODE;

    /**
	* @brief  gbk 转换到 utf8. 
	* @brief  The transformation from GBK to UTF8.
	*  
    * @param sIn   输入buffer*
	* @param sIn   input buffer*
    * @throws      TC_Encoder_Exception
    * @return      转换后的utf8编码
	* @return      the UTF8 encodeing after transformation
    */
    static string gbk2utf8(const string &sIn,int mode=ICONV_IGNORE);

    /**
	* @brief  gbk 转换到 utf8. 
	* @brief  The transformation from GBK to UTF8.
	*  
    * @param sIn    输入buffer
	* @param sIn    input buffer
    * @param vtStr  输出gbk的vector
	* @param vtStr  output the GBK vector
    * @throws       TC_Encoder_Exception
    * @return
    */
    static void gbk2utf8(const string &sIn, vector<string> &vtStr,int mode=ICONV_IGNORE);


    /**
	* @brief  utf8 转换到 gbk. 
	* @brief  The transformation from UTF8 to GBK.
	*  
    * @param sIn  输入buffer
	* @param sIn  input buffer
    * @throws     TC_Encoder_Exception
    * @return    转换后的gbk编码
	* @return    the GBK encodeing after transformation
    */
    static string utf82gbk(const string &sIn,int mode=ICONV_NORMAL);

	/**	
	* @brief  将string的\n替换掉,转义字符串中的某个字符 
	* @brief  Replace string '\n ' to escape a character in the string
	*  
	* 缺省:\n 转换为 \r\0; \r转换为\,
	* Default: '\n' is escaped to '\r\0'; '\r' is escaped to '\',
	*  
	* 主要用于将string记录在一行，通常用于写bin-log的地方;
	* Mainly used to record strings on one line, usually where 'bin-logs' are written;
	* @param str   待转换字符串
	* @param str   the string to be escaped
	* @param f     需要转义的字符
	* @param f     the character to be escaped
	* @param t     转义后的字符
	* @param t     the escaped character
	* @param u     借用的转义符
	* @param u     the borrowed escape character
	* @return str  转换后的字符串
	* @return str  the escaped string
	*/
	static string transTo(const string& str, char f = '\n', char t = '\r', char u = '\0');

	/**
	* @brief  从替换的数据恢复源数据,将 transTo 的字符串还原， 
	* @brief  Restore source data from the replaced data, restore the string of transTo.
	*  
	*  缺省:\r\0 还原为\n，\r\r还原为,
	*  Default: '\r\0' is escaped to '\n'; '\r\r' is escaped to,
	*  
	*  主要用于将string记录在一行，通常用于写bin-log的地方
	*  It is mainly used to record string on one line,usally used to where the bin-log written.
	* @param str  待还原的字符串(必须是transTo后得到的字符串)
	* @param str  the string to be resroed(must be the string after transTo)
	* @param f    被转义的字符
	* @param f    the escaped character
	* @param t    转义后的字符
	* @param t    the character after being escaped
	* @param u    借用的转义符
	* @param u    the borrowed escaped character
	* @return str 还原后的字符串
	* @return str the restored string
	*/
	static string transFrom(const string& str, char f = '\n', char t = '\r', char u = '\0');

protected:    

    /**
	* @brief  utf8 转换到 gbk. 
	* @brief  The transformation from UTF8 to GBK.
	*  
    * @param sOut       输出buffer
	* @param sOut       output buffer
    * @param iMaxOutLen 输出buffer最大的长度/sOut的长度
	* @param iMaxOutLen output the max buffer length/ sOut length
    * @param sIn        输入buffer
	* @param sIn        input buffer
    * @param iInLen     输入buffer长度
	* @param iInLen     input buffer length
    * @throws           TC_Encoder_Exception
    * @return
    */
    static void utf82gbk(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen,int mode);
};

}


#endif


