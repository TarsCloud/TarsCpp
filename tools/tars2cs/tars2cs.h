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
#ifndef _TARS2CS_H
#define _TARS2CS_H

#include "parse.h"

#include <cassert>
#include <string>

/**
 * ����tars����java�ļ�
 * �����ṹ�ı�����Լ�����Proxy��Servant
 */
class Tars2Cs
{
public:
    /**
     * ���ô������ɵĸ�Ŀ¼
     * @param dir
     */
    void setBaseDir(const string &dir);

    /**
     * ���ð�ǰ׺
     * @param prefix
     */
    void setBasePackage(const string &prefix);

    /**
     * �����Ƿ���Ҫ����˴���
     */
    void setWithServant(bool bWithServant) { _bWithServant = bWithServant;}

    /**
     * ����
     * @param file
     * @param isFramework �Ƿ��ǿ��
     */
    void createFile(const string &file);

protected:
    /**
     * ���������ռ��ȡ�ļ�·��
     * @param ns �����ռ�
     *
     * @return string
     */
    string getFilePath(const string &ns) const;

    string _packagePrefix;
    string _baseDir;
    bool   _bWithServant;

    //�����Ǳ�����Դ������
protected:

    /**
     * ����ĳ���͵Ľ���Դ��
     * @param pPtr
     *
     * @return string
     */
    string writeTo(const TypeIdPtr &pPtr) const;

    /**
     * ����ĳ���͵ı���Դ��
     * @param pPtr
     *
     * @return string
     */
    string readFrom(const TypeIdPtr &pPtr) const;

    /**
     * 
     * @param pPtr
     * 
     * @return string
     */
    string display(const TypeIdPtr &pPtr) const;

    //����������������Դ������
protected:

    /*
     * ����ĳ���͵ĳ�ʼ���ַ���
     * @param pPtr
     *
     * @return string
     */
    string toTypeInit(const TypePtr &pPtr) const;

    /**
     * ����ĳ���͵Ķ�Ӧ������ַ�������Դ��
     * @param pPtr
     *
     * @return string
     */
    string toObjStr(const TypePtr &pPtr) const;

    /**
     * �ж��Ƿ��Ƕ�������
     */
    bool isObjType(const TypePtr &pPtr) const;

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
    string tostrMap(const MapPtr &pPtr, bool bNew = false) const;

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

    //������h��java�ļ��ľ�������
protected:
    /**
     * ���ɽṹ��java�ļ�����
     * @param pPtr
     *
     * @return string
     */
    string generateCs(const StructPtr &pPtr, const NamespacePtr &nPtr) const;

    /**
     * ����������javaԴ��
     * @param pPtr
     *
     * @return string
     */
    string generateCs(const ContainerPtr &pPtr) const;

    /**
     * ���ɲ���������java�ļ�����
     * @param pPtr
     *
     * @return string
     */
    string generateCs(const ParamDeclPtr &pPtr) const;

    /**
     * ����ö�ٵ�ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    string generateCs(const EnumPtr &pPtr, const NamespacePtr &nPtr) const;

    /**
     * ���ɳ���javaԴ��
     * @param pPtr
     * 
     * @return string
     */
    void generateCs(const ConstPtr &pPtr, const NamespacePtr &nPtr) const;

    void generateCs(const vector<EnumPtr> &es,const vector<ConstPtr> &cs,const NamespacePtr &nPtr) const;
    /**
     * �������ֿռ�java�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    void generateCs(const NamespacePtr &pPtr) const;

    /**
     * ����ÿ��tars�ļ���java�ļ�Դ��
     * @param pPtr
     *
     * @return string
     */
    void generateCs(const ContextPtr &pPtr) const;

};

#endif


