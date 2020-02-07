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
#ifndef _TARS2C_H
#define _TARS2C_H

#include "parse.h"

#include <cassert>
#include <string>

using namespace tars;

/**
 * ����tars����c�ļ�
 * �����ṹ�ı�����Լ�����Proxy��Servant
 */
class Tars2C
{
public:

    /**
     * ����
     * @param file
     */
    void createFile(const string &file);


    /**
    * ���������ļ���Ŀ¼
    *
    */
    void setBaseDir(const std::string & sPath) { m_sBaseDir = sPath; }


	/**
	 * �����Ƿ���Ĭ��ֵ
	 * 
	 * @author kevintian (2010-10-8)
	 * 
	 * @param bCheck 
	 */
	void setCheckDefault(const bool bCheck) { m_bCheckDefault = bCheck; }

    //�����Ǳ�����Դ������
protected:

    /**
     * ����ĳ���͵Ľ���Դ��
     * @param pPtr
     *
     * @return string
     */
    string writeTo(const TypeIdPtr &pPtr,const string& namespaceId = "") const;

    /**
     * ����ĳ���͵ı���Դ��
     * @param pPtr
     *
     * @return string
     */
    string readFrom(const TypeIdPtr &pPtr, bool bIsRequire = true,const string& namespaceId="") const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string display(const TypeIdPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string displaySimple(const TypeIdPtr &pPtr, bool bSep) const;

    //����������������Դ������
protected:
    /**
     * ����ĳ���͵��ַ�������Դ��
     * @param pPtr
     *
     * @return string
     */
    string tostr(const TypePtr &pPtr) const;

    /**
     * �����ڽ����͵��ַ���Դ��
     * @param pPtr
     *
     * @return string
     */
    string tostrBuiltin(const BuiltinPtr &pPtr) const;
    /**
     * ����vector���ַ�������
     * @param pPtr
     *
     * @return string
     */
    string tostrVector(const VectorPtr &pPtr) const;

    /**
     * ����map���ַ�������
     * @param pPtr
     *
     * @return string
     */
    string tostrMap(const MapPtr &pPtr) const;

    /**
     * ����ĳ�ֽṹ�ķ�������
     * @param pPtr
     *
     * @return string
     */
    string tostrStruct(const StructPtr &pPtr) const;


    /**
     * ����ĳ��ö�ٵķ�������
     * @param pPtr
     *
     * @return string
     */
    string tostrEnum(const EnumPtr &pPtr) const;

    /**
     * ���ɽӿ�����
     * @param pPtr
     *
     * @return string
     */

    string toFuncName(const TypePtr &pPtr) const;

    string toClassName(const TypePtr &pPtr) const;
    /**
     * �������ͱ����Ľ���Դ��
     * @param pPtr
     *
     * @return string
     */
    string decode(const TypeIdPtr &pPtr) const;

    /**
     * �������ͱ����ı���Դ��
     * @param pPtr
     *
     * @return string
     */
    string encode(const TypeIdPtr &pPtr) const;

    //������h��c�ļ��ľ�������
protected:
    /**
     * �ṹ��md5
     * @param pPtr
     *
     * @return string
     */
    string MD5(const StructPtr &pPtr) const;

    /**
     * ���ɽṹ��ͷ�ļ�����
     * @param pPtr
     *
     * @return string
     */
    string generateH(const StructPtr &pPtr, const string& namespaceId) const;

    /**
     * ���ɽṹ��cpp�ļ�����
     * @param pPtr
     *
     * @return string
     */
    string generateC(const StructPtr &pPtr, const string& namespaceId) const;

    /**
     * ����������ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateH(const ContainerPtr &pPtr) const;

    /**
     * ����������cԴ��
     * @param pPtr
     *
     * @return string
     */
    string generateCpp(const ContainerPtr &pPtr) const;

    /**
     * ���ɲ���������ͷ�ļ�����
     * @param pPtr
     *
     * @return string
     */
    string generateH(const ParamDeclPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string generateOutH(const ParamDeclPtr &pPtr) const;

    /**
     *
     * @param pPtr
     * @param cn
     *
     * @return string
     */
    //string generateAsyncResponseCpp(const OperationPtr &pPtr, const string &cn) const;

    /**
     * ���ɲ���������cpp�ļ�����
     * @param pPtr
     *
     * @return string
     */
    string generateCpp(const ParamDeclPtr &pPtr) const;

    /**
     * ������������proxy��cpp�ļ�����
     * @param pPtr
     * @param cn
     *
     * @return string
     */
    string generateCpp(const OperationPtr &pPtr, const string &cn) const;

    /**
     * ���ɲ�����servant��ͷ�ļ�Դ��
     * @param pPtr
     * @param bVirtual
     *
     * @return string
     */
    string generateH(const OperationPtr &pPtr, bool bVirtual, const string& interfaceId) const;

    /**
     * ���ɽӿڵ�ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateH(const InterfacePtr &pPtr) const;

    /**
     * ���Ľӿڵ�cpp�ļ���Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateCpp(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const;

    /**
     * ����ö�ٵ�ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateH(const EnumPtr &pPtr, const string& namespaceId) const;

    /**
     * ���ɳ���ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateH(const ConstPtr &pPtr) const;

    /**
     * �������ֿռ��ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateH(const NamespacePtr &pPtr) const;

    /**
     * �������ֿռ�cpp�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateC(const NamespacePtr &pPtr) const;

    /**
     * �������ֿռ��ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    void generateH(const ContextPtr &pPtr) const;

    /**
     * �������ֿռ�cpp�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    void generateC(const ContextPtr &pPtr) const;

    /**
     *  
     * �������ֲ��ҽṹ 
     * @param pPtr 
     * @param id 
     * 
     * @return StructPtr 
     */
    StructPtr findStruct(const ContextPtr &pPtr,const string &id);

    std::string m_sBaseDir;


	/**
	 * �Ƿ���Ĭ��ֵ
	 * 
	 * @author kevintian (2010-10-8)
	 */
	bool m_bCheckDefault;
};

#endif


