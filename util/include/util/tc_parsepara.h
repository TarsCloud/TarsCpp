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

#ifndef __TC_PARSEPARA_H
#define __TC_PARSEPARA_H

#include <map>
#include <string>

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/** 
* @file tc_parsepara.h 
* @brief  name=value格式解析类(非标准的http协议) 
* @brief  name=value format parsing class (non-standard http protocol)
*   
*/            
/////////////////////////////////////////////////


/** 
* @brief  提供name=value&name1=value1形式的解析函数.
* @brief  Provides an analytic function in the form name=value&name1=value1.
* 
* 可以和map互相转换,不是标准的cgi参数的解析
* Convertible with map, not parsing standard CGI parameters
* 
* （标准cgi参数解析会把空格和+互转）,非线程安全；
* (Standard CGI parameter parsing will swap spaces and +), non-thread safe;
* @param 型如name=value&name=value字符串
* @param : Types such as name=value&name=value string
*/
class TC_Parsepara
{
public:

    TC_Parsepara(){};

    /**
    * @brief  构造函数
    * @brief  Constructor
    * @param 型如name=value&name=value字符串
    * @param : Types such as name=value&name=value string
    */
    TC_Parsepara(const string &sParam);

    /**
    * @brief  copy contructor
    * @param : name=value&name=value字符串
    * @param : Types such as name=value&name=value string
    */
    TC_Parsepara(const map<string, string> &mpParam);

    /**
    * @brief  copy contructor
    */
    TC_Parsepara(const TC_Parsepara &para);

    /**
    * =
    */
    TC_Parsepara &operator=(const TC_Parsepara &para);

    /**
    * ==
    */
    bool operator==(const TC_Parsepara &para);

    /**
    *+
    */
    const TC_Parsepara operator+(const TC_Parsepara &para);

    /**
    * +=
    */
    TC_Parsepara& operator+=(const TC_Parsepara &para);

    /**
    * @brief  decontructor
    */
    ~TC_Parsepara();

    /**
    * @brief  遍历函数
    * @brief  Traversal function
    * @param 参数名称
    * @param : Parameter Name
    * @param 参数值
    * @param : parameter values
    */
    typedef int (*TC_ParseparaTraverseFunc)(string, string, void *);

    /**
    *@brief  重载[], 获取参数值
    *@brief  Overload [], get parameter values
    *@return string &参数值
    *@return string &parameter values
    */
    string &operator[](const string &sName);

    /**
    * @brief  读入字符串,并解析
    * @brief  Overload [], get parameter values
    * @param sParam:字符串函数
    * @param sParam:String function
    * @return 无
    * @return none
    */
    void load(const string &sParam);

    /**
    * @brief  读入map,并解析
    * @brief  Read in the map and parse it
    * @param mpParam:字符串函数
    * @param mpParam:String function
    * @return void
    */
    void load(const map<string, string> &mpParam);

    /**
    * @brief  转成字符串
    * @brief  Convert to String
    * @return string
    */
    string tostr() const;

    /**
    * @brief  获取参数值
    * @brief  Get parameter values
    * @param sName 参数名称
    * @param sName Parameter Name
    * @return string
    */
    string getValue(const string &sName) const;

    /**
    * @brief  设置参数值
    * @brief  Setting parameter values
    * @param sName 参数名称
    * @param sName Parameter Name
    * @param sValue 参数值
    * @param sValue Parameter Value
    * @return void
    */
    void setValue(const string &sName, const string &sValue);

    /**
    * @brief  清除当前参数值对
    * @brief  Clear current parameter value pair
    * return void
    */
    void clear();

    /**
    * @brief  引用方式获取参数map
    * @brief  Getting parameter map by reference
    * @return map<string,string>& 返回参数map
    * @return map<string,string>& return parameter map
    */
    map<string,string> &toMap();

    /**
    * @brief  引用方式获取参数map
    * @brief  Getting parameter map by reference
    * @return map<string,string>& 返回参数map
    * @return map<string,string>& return parameter map
    */
    const map<string,string> &toMap() const;

    /**
    * @brief  遍历每个参数值对
    * @brief  Traverse through each parameter value pair
    * @param func: 函数
    * @param func: function
    * @param pParam: 参数,传入func中
    * @param pParam: parameter, passed into func
    * @return void
    */
    void traverse(TC_ParseparaTraverseFunc func, void *pParam);

    /**
    * @brief  对字符串解码,%XX转成字符,类似于http协议的编码
    * @brief  For string decoding,%XX is converted to character, similar to HTTP protocol encoding
    * @param sParam 参数
    * @param sParam paarameter
    * @return string,解码后的字符串
    * @return string,Decoded string
    */
    static string decodestr(const string &sParam);

    /**
    * @brief  对字符串编码,特殊字符转成%XX, 
    *         类似于http协议的编码(少了对空格=>+的特殊处理)
    * @brief  For string encoding, special characters are converted to%XX,
    *         Encoding similar to HTTP protocol (with less special handling of space=>+)
    * @param sParam 参数
    * @param sParam parameter
    * @return string, 编码后的字符串
    * @return string, encoded string
    */
    static string encodestr(const string &sParam);

protected:

    /**
    * @brief  字符串转换成map
    * @brief  String to map
    * @param sParam 参数
    * @param sParam parameter
    * @param mpParam map
    * @return void
    */
    void decodeMap(const string &sParam, map<string, string> &mpParam) const;

    /**
    * @brief  map转换成字符串
    * @brief  Map to string
    * @param mpParam map
    * @return string, 转换后的字符串
    * @return string, Converted string
    */
    string encodeMap(const map<string, string> &mpParam) const;

    /**
    * @brief  "%xx" 转换为字符
    * @brief  "%xx" Convert to Character
    * @param sWhat: %xx字符串后面的两个字符
    * @param sWhat: Two characters after the%xx string
    * @return char 返回字符
    * @return char return characters
    */
    static char x2c(const string &sWhat);

protected:

    map<string, string> _param;
};

}
#endif /*_TC_PARSEPARA_H*/
