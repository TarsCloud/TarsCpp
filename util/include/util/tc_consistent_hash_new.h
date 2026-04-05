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

#pragma once

#include "util/tc_platform.h"
#include "util/tc_md5.h"
#include "util/tc_autoptr.h"
#include "util/tc_hash_fun.h"
#include <memory>
#include <atomic>


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
    virtual uint32_t hash(const char *sKey, size_t length) = 0;
    virtual TC_HashAlgorithmType getHashType() = 0;
};

typedef TC_AutoPtr<TC_HashAlgorithm> TC_HashAlgorithmPtr;

/**
 *  @brief ketama hash 算法
 *  @brief Ketama Hash Algorithm
 */
class TC_KetamaHashAlg : public TC_HashAlgorithm
{
public:
    virtual uint32_t hash(const char *sKey, size_t length);
    virtual TC_HashAlgorithmType getHashType();
};

/**
 *  @brief 默认的 hash 算法
 *  @brief Default Hash Algorithm
 */
class TC_DefaultHashAlg : public TC_HashAlgorithm
{
public:
    virtual uint32_t hash(const char *sKey, size_t length);
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
class UTIL_DLL_API TC_ConsistentHashNew
{
public:

    struct node_T_new
    {
        /**
         *节点hash值
         *node hash value
        */
        uint32_t iHashCode;

        /**
         *节点名称
         *node name
         */
        string sNode;

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
     * @brief 获取某key对应到的节点node的名称.
     * @brief Gets the name of the node to which a key corresponds.
     *
     * @param key      key名称
     * @param key      key name
     * @param sNode    对应到的节点的名称
     * @param sNode    the name of the node to which corresponds.
     * @return        0:获取成功   -1:没有被添加的节点
     * @return        0:obtain successfully  -1:no nodes added
     */
    int getNodeName(const string & key, string & sNode);

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
    int getIndex(uint32_t hashcode, unsigned int & iIndex);

    /**
     * @brief 获取某hashcode对应到的节点node的名称.
     * @brief Gets the name of the node to which a certain hashcode corresponds
     *
     * @param hashcode      hashcode
     * @param sNode   对应到的节点的名称
     * @param sNode   the name of the node to which corresponds.
     * @return        0:获取成功   -1:没有被添加的节点
     * @return        0:obtain successfully  -1:no nodes added
     */
    int getNodeName(uint32_t hashcode, string & sNode);

    /**
     * @brief 获取当前hash列表的长度.
     * @brief Get the length of the current hash list
     *
     * @return        长度值
     * @return        length
     */
    size_t size() {
        auto listPtr = std::atomic_load(&_vHashListPtr);
        return listPtr ? listPtr->size() : 0;
    }

    /**
     * @brief 清空当前的hash列表.
     * @brief Empty the current hash list.
     *
     */
    void clear() {
        auto emptyList = std::make_shared<vector<node_T_new>>();
        std::atomic_store(&_vHashListPtr, emptyList);
    }

protected:
    /**
     * @brief 使用Copy-On-Write + CAS实现完全无锁
     * @brief Completely lock-free using Copy-On-Write + CAS
     *
     * 设计说明：
     * 1. 读操作(getNodeName/getIndex)：无锁，原子读取shared_ptr快照
     * 2. 写操作(addNode/clear)：Copy-On-Write + CAS，无需任何锁
     *    - 拷贝当前vector → 在副本上修改 → CAS替换指针
     *    - 如果CAS失败（被其他写操作抢先），则重试
     * 3. 协程安全：读写操作均完全无锁，无死锁风险
     * 4. 性能优秀：读操作(99%)零开销，写操作(1%)极少重试
     *
     * Design notes:
     * 1. Read ops: Lock-free, atomically load shared_ptr snapshot
     * 2. Write ops: COW + CAS, completely lock-free
     *    - Copy current vector → modify copy → CAS swap pointer
     *    - Retry on CAS failure (another writer committed first)
     * 3. Coroutine-safe: All operations are lock-free, no deadlock risk
     * 4. Performance: Read ops (99%) zero overhead, write ops (1%) rare retry
     */
    std::shared_ptr<vector<node_T_new>> _vHashListPtr;

    TC_HashAlgorithmPtr _ptrHashAlg;

};

}
