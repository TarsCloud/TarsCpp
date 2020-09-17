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

#ifndef __TC_CONSISTENT_HASH_NEW_H_
#define __TC_CONSISTENT_HASH_NEW_H_

#include "util/tc_md5.h"
#include "util/tc_autoptr.h"
#include "util/tc_hash_fun.h"


namespace tars
{

enum TC_HashAlgorithmType
{
    E_TC_CONHASH_KETAMAHASH = 0,
    E_TC_CONHASH_DEFAULTHASH = 1
};

/**
 *  @brief hash 算法虚基类
 *  @brief hash Algorithmic Virtual Base Class
 */
class TC_HashAlgorithm : public TC_HandleBase
{
public:
    virtual int32_t hash(const char *sKey, size_t length) = 0;
    virtual TC_HashAlgorithmType getHashType() = 0;

protected:
    int32_t subTo32Bit(int32_t hash) { return (hash & 0xFFFFFFFFL); }

};

typedef TC_AutoPtr<TC_HashAlgorithm> TC_HashAlgorithmPtr;

/**
 *  @brief ketama hash 算法
 *  @brief Ketama Hash Algorithm
 */
class TC_KetamaHashAlg : public TC_HashAlgorithm
{
public:
    virtual int32_t hash(const char *sKey, size_t length);
    virtual TC_HashAlgorithmType getHashType();
};

/**
 *  @brief 默认的 hash 算法
 *  @brief Default Hash Algorithm
 */
class TC_DefaultHashAlg : public TC_HashAlgorithm
{
public:
    virtual int32_t hash(const char *sKey, size_t length);
    virtual TC_HashAlgorithmType getHashType();
};

/**
 *  @brief hash alg 工厂
 *  @brief Hash Algorithm Factory
 */
class TC_HashAlgFactory
{
public:
    static TC_HashAlgorithm *getHashAlg(TC_HashAlgorithmType hashType);
};

/**
 *  @brief 一致性hash算法类
 *  @brief Consistency Hash Algorithm Class
 */
class  TC_ConsistentHashNew
{
public:

    struct node_T_new
    {
        /**
         *节点hash值
         *node hash value
        */
        int32_t iHashCode;

        /**
         *节点下标
         * node subscript
        */
        unsigned int iIndex;
    };

    /**
     *  @brief 构造函数
     *  @brief Constructor
     */
    TC_ConsistentHashNew();

    /**
     *  @brief 构造函数
     *  @brief Constructor
     */
    TC_ConsistentHashNew(TC_HashAlgorithmType hashType);

    /**
     * @brief 排序
     * @brief Sort
     *
     * @param node  节点名称
     * @param node  node name
     * @param index 节点的下标值
     * @param index node subscript value
     */
    void sortNode();

    /**
     * @brief 打印节点信息
     * @brief print node info
     *
     */
    void printNode();

    /**
     * @brief 增加节点.
     * @brief add nodes
     *
     * @param node  节点名称
     * @param node  node name
     * @param index 节点的下标值
     * @param index node subscript value
     * @param weight 节点的权重，默认为1
     * @param weight node weight, default value is 1
     * @return      是否成功
     * @return      whether it is successfull or not
     */
    int addNode(const string & node, unsigned int index, int weight = 1);

    /**
     * @brief 获取某key对应到的节点node的下标.
     * @brief Gets the subscript of the node to which a key corresponds.
     *
     * @param key      key名称
     * @param key      key name
     * @param iIndex  对应到的节点下标
     * @param iIndex  the subscript of the node to which corresponds.
     * @return        0:获取成功   -1:没有被添加的节点
     * @return        0:obtain successfully  -1:no nodes added
     */
    int getIndex(const string & key, unsigned int & iIndex);

    /**
     * @brief 获取某hashcode对应到的节点node的下标.
     * @brief Gets the subscript of the node to which a certain hashcode corresponds
     *
     * @param hashcode      hashcode
     * @param iIndex  对应到的节点下标
     * @param iIndex  the subscript of the node to which corresponds
     * @return        0:获取成功   -1:没有被添加的节点
     * @return        0:obtain successfully  -1:no nodes added
     */
    int getIndex(int32_t hashcode, unsigned int & iIndex);

    /**
     * @brief 获取当前hash列表的长度.
     * @brief Get the length of the current hash list
     *
     * @return        长度值
     * @return        length
     */
    size_t size() { return _vHashList.size(); }

    /**
     * @brief 清空当前的hash列表.
     * @brief Empty the current hash list.
     *
     */
    void clear() { _vHashList.clear(); }

protected:
    vector<node_T_new>    _vHashList;
    TC_HashAlgorithmPtr _ptrHashAlg;

};

}
#endif
