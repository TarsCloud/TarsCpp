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

#ifndef __TC_OPTION_H
#define __TC_OPTION_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_option.h 
 * @brief 命令行参数解析类.
 * @brief Command Line Parameter Resolution Class.
 * 
 */       
/////////////////////////////////////////////////
/**
 * @brief 命令解析类，通常用于解析命令行参数
 * @brief Command parsing class, usually used to parse command line parameters
 *
 * 支持以下形式的参数:  ./main.exe --name=value --with abc def 
 * The following form of parameters are supported: ./main.exe --name=value --with abc def 
 */
class TC_Option
{
public:
    /**
     * @brief 构造函数
     * @brief Constructor
     */
    TC_Option(){};

    /**
     * @brief 解码. 
     * @brief Decode
     *  
     * @param argc 参数个数
     * @param argc Number of parameters
     * @param argv 参数数组
     * @param argv Array of parameters
     *
     */
    void decode(int argc, char *argv[]);

    /**
	 * @brief 解码(和上面decode的区别是: 只有命令, 没有签名的argv[0])
     * @brief Decode (the difference from decode above is: command only, argv[0] without signature)
	 *  
     * @param command 命令
     * @param command command
     *
     */
    void decode(const char *command);

    /**
     * @brief 是否存在某个--标识的参数. 
     * @brief Whether a parameter identified by -- exists.
     *  
     * @param sName  要判断的标识
     * @param sName  Identification to be judged
     * @return bool 存在返回true，否则返回false
     * @return bool Existence returns true, otherwise false
     */
    bool hasParam(const string &sName) const;

    /**
     * @brief 获取某个--表示的参数，如果参数不存在或者参数值为空 , 
     *        都返回""
     * @brief Gets the parameter represented by an -- and returns "" if it does not exist or if the parameter value is empty
     * @param sName   标识
     * @param sName   Identification
     * @return string 标识的参数值
     * @return string Identified parameter values
     */
    string getValue(const string &sName) const;

    /**
     * @brief 获取所有--标识的参数.
     * @brief Get all--identified parameters.
     *
     * @return map<string,string> map类型的标识和参数值的对应关系
     * @return Map<string, string> map type identity and corresponding relationship to parameter values
     */
    const map<string, string>& getMulti() const;

    /**
     * @brief 获取所有普通的参数, 例子中的abc, 
     *        def，参数按照顺序在vector中
     * @brief Get all the common parameters, abc, 
     *        def in the example, in vectors in order
     * @return vector<string> 顺序存放参数的vector
     * @return Vector<string>Sequential vector holding parameters
     */
    const vector<string>& getSingle() const;

    /**
     * @brief 获取所有--标识的参数.
     * @brief Get all--identified parameters.
     *
     * @return map<string,string> map类型的标识和参数值的对应关系
     * @return Map<string, string> map type identity and corresponding relationship to parameter values
     */
    map<string, string>& getMulti();

    /**
     * @brief 获取所有普通的参数, 例子中的abc, 
     *        def，参数按照顺序在vector中
     * @brief Get all the common parameters, abc, 
     *        def in the example, in vectors in order
     * @return vector<string> 顺序存放参数的vector
     * @return vector<string> Vector that stores parameters sequentially
     */
    vector<string>& getSingle();

protected:

    /**
     * @brief 解析字符串，取出标识和其对应的参数值， 
     *        对型如--name=value 的字符串进行解析，取出name和vaue
     * @brief Parse the string, take out the identity and its corresponding parameter values, 
     *        Parse strings of type--name=value, take out name and vaue
     * @param s 要解析的字符串
     * @param s String to parse
     */
    void parse(const string &s);

protected:
    /**
     *存放标识和其对应参数的对应关系，例如：对于--name=value，存放name和value
     *Store the relationship between the identity and its corresponding parameters, for example, for --name=value, name and value
     */
    map<string, string> _mParam;

    /**
     *存放普通参数的vetor
     *Vetor holding common parameters
     */
    vector<string>      _vSingle;
};

}

#endif

