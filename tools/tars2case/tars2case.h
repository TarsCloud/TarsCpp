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

#ifndef __TARS2CASE_H_
#define __TARS2CASE_H_

#include "parse.h"
#include "util/tc_common.h"
#include "util/tc_option.h"
#include "tup/Tars.h"
#include "tup/TarsJson.h"

#include <cassert>
#include <string>


using namespace tars;
/**
 * 根据jce生成自动测试的字符串
 */
class Tars2Case
{
public:
    Tars2Case();

    /**
     * 设置代码生成的根目录
     * @param dir
     */
    void setBaseDir(const string &dir);

    /**
     * 生成JSON2TARS用例
     * @param jsonCase
     */
    void setJsonCase(bool jsonCase);

    /**
     * 生成WEB平台需要的用例
     * @param webCase
     */
    void setWebSupport(bool webCase);

    /**
     * 生成
     * @param file
     * @param isFramework 是否是框架
     */
    void createFile(const string &tarsfile, const string& outfile = "");

protected:
    /**
     * 根据命名空间获取文件路径
     * @param ns 命名空间
     *
     * @return string
     */
    string getFilePath(const string &ns) const;

    string 	_baseDir;
    bool    _jsonCase;
    bool    _webCase;

    //下面是类型描述的源码生成
protected:
    /**
     * 生成某类型的字符串描述源码
     * @param pPtr
     *
     * @return string
     */
    string toStr(const TypePtr &pPtr) const;


    /**
     * 生成某类型的字符串描述源码
     * @param pPtr
     *
     * @return string
     */
    string toDescStr(const TypePtr &pPtr) const;

    /**
     * 生成内建类型的字符串源码
     * @param pPtr
     *
     * @return string
     */
    string tostrBuiltin(const BuiltinPtr &pPtr) const;
    /**
     * 生成vector的字符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrVector(const VectorPtr &pPtr) const;

    /**
     * 生成map的字符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrMap(const MapPtr &pPtr, bool bNew = false) const;

    /**
     * 生成某种结构的符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrStruct(const StructPtr &pPtr) const;

    /**
     * 生成某种枚举的符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrEnum(const EnumPtr &pPtr) const;

protected:
    /**
     * 生成某类型的JSON描述源码
     * @param pPtr
     *
     * @return string
     */
    JsonValuePtr generateJson(const TypePtr &pPtr) const;

    JsonValuePtr toJsonBuiltin(const BuiltinPtr &pPtr) const;

    JsonValuePtr toJsonEnum(const EnumPtr &pPtr) const;

    JsonValueObjPtr toJsonMap(const MapPtr &pPtr) const;

    JsonValueObjPtr toJsonStruct(const StructPtr &pPtr) const;

    JsonValueArrayPtr toJsonVector(const VectorPtr &pPtr) const;

    JsonValuePtr generateJsonDesc(const TypeIdPtr &pPtr) const;

    JsonValuePtr generateJsonDesc(const TypePtr &pPtr) const;

    JsonValuePtr toJsonDescEnum(const EnumPtr &pPtr) const;

    JsonValuePtr toJsonDescMap(const MapPtr &pPtr) const;

    JsonValuePtr toJsonDescStruct(const StructPtr &pPtr) const;

    JsonValuePtr toJsonDescVector(const VectorPtr &pPtr) const;

protected:

	string tocaseBuiltin(const BuiltinPtr &pPtr, const string& varName) const;

    string tocaseVector(const VectorPtr &pPtr, const string& varName) const;

    string tocaseMap(const MapPtr &pPtr, const string& varName) const;

    string tocaseStruct(const StructPtr &pPtr, const string& varName) const;

    string tocaseEnum(const EnumPtr &pPtr, const string& varName) const;

	string toCase(const TypePtr &pPtr, const string& varName) const;

    //以下是h和java文件的具体生成
protected:
    /**
     * 生成参数声明的test文件内容
     * @param pPtr
     *
     * @return string
     */
    string generateTest(const ParamDeclPtr &pPtr) const;

	/**
	*
	* 生成测试用例
	**/
    string generateCase(const ParamDeclPtr &pPtr) const;

   /**
     * 生成WEB代理请求访问的JSON串
     * @param pPtr
     * @param outfile
     *
     * @return string
     */
    string generateJson(const InterfacePtr &pPtr, const string& outfile = "") const;

   /**
     * 生成WEB代理请求访问的JSON串, JSON用例
     * @param pPtr
     * @param outfile
     *
     * @return string
     */
    string generateDesc(const InterfacePtr &pPtr, const string& outfile = "") const;

    /**
     * 生成TEST接口访问的测试
     * @param pPtr
     * @param outfile
     *
     * @return string
     */
    string generateTest(const InterfacePtr &pPtr, const string& outfile = "") const;

    /**
     * 生成每个tars文件的用例文件
     * @param pPtr
     *
     * @return string
     */
    void generateFile(const ContextPtr &pPtr, const string& outfile = "") const;

	/**
	* 生成tag和require、optional信息
	*/
	string genratePrefix(const TypeIdPtr &ptr) const;
};

#endif


