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
#ifndef _TARS2PHP_H
#define _TARS2PHP_H

#include "parse.h"

#include <cassert>
#include <string>

using namespace tars;

/**
 */
class Tars2Php
{
public:
	Tars2Php();

    /**
     *
     * @param file
     */
    void createFile(const string &file, const vector<string> &vsCoder);


    /**
    *
    *
    */
    void setBaseDir(const std::string & sPath) { m_sBaseDir = sPath; }

	/**
	 *
	 */
	void setCheckDefault(bool bCheck) { m_bCheckDefault = bCheck; }

protected:

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string writeTo(const TypeIdPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string readFrom(const TypeIdPtr &pPtr, bool bIsRequire = true) const;

protected:
    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string tostr(const TypePtr &pPtr) const;

    /**
     * @param pPtr
     *
     * @return string
     */
    string tostrBuiltin(const BuiltinPtr &pPtr) const;
    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string tostrVector(const VectorPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string tostrMap(const MapPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string tostrStruct(const StructPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string tostrEnum(const EnumPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */

    string toStrSuffix(const TypeIdPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return int
     */
    int getSuffix(const TypeIdPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string decode(const TypeIdPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string encode(const TypeIdPtr &pPtr) const;

protected:
    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string generatePHP(const StructPtr &pPtr, const string& namespaceId) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string generatePHP(const NamespacePtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    void generatePHP(const ContextPtr &pPtr) const;

    void generatePHP_Pdu(const ContextPtr &pPtr) const;

private:
    std::string m_sBaseDir;

	bool m_bCheckDefault;
};

#endif


