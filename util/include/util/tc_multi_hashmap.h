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

#ifndef __TC_MULTI_HASHMAP_H__
#define __TC_MULTI_HASHMAP_H__

#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <functional>
#include "util/tc_ex.h"
#include "util/tc_mem_vector.h"
#include "util/tc_pack.h"
#include "util/tc_mem_chunk.h"
#include "util/tc_hash_fun.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_multi_hashmap.h  
 * @brief 支持多key的hashmap类. 
 * @brief HashMap Class supporting multi key
 * 
 */
/////////////////////////////////////////////////

/**
*  @brief Multi Hash map异常类
*  @brief Multi Hash Map Exception Class
*/
struct TC_Multi_HashMap_Exception : public TC_Exception
{
    TC_Multi_HashMap_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_Multi_HashMap_Exception() throw(){};
};

////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief 基于内存的支持多key的hashmap.
 *  @brief Memory-based HashMap that supports multiple keys.
 *  
 * 所有操作需要自己加锁 
 * All operations require self-locking
 *  
 * 所有存储的地址均采用32位保存，为内存块的索引，要求总内存块数不能超过32位范围
 * All stored addresses are 32-bit, indexed by memory blocks, requiring that the total number of memory blocks do not exceed the 32-bit range
 */
class TC_Multi_HashMap
{
public:
    struct HashMapIterator;
    struct HashMapLockIterator;
    /**
    * @brief 真正存储的数据结构
    * @brief Truly stored data structure
    */
    struct BlockData
    {
        /**Data Key, union key (after union primary key is removed)*/
        string            _key;       /**数据Key，联合key(联合主键去掉主key后)*/
        /**data value*/
        string            _value;     /**数据value*/
        /**Is it dirty data*/
        bool            _dirty;     /**是否是脏数据*/
        /**Data Version, 1 for Initial Version, 0 for Retention*/
        uint8_t            _iVersion;    /**数据版本，1为初始版本，0为保留*/
        /**Sync time, not necessarily true writeback time*/
        time_t            _synct;     /**sync time, 不一定是真正的回写时间*/
        BlockData()
        : _dirty(false)
        , _iVersion(1)
        , _synct(0)
        {
        }
    };

    /**
    * @brief 完整的数据结构，作为get的返回值
    * @brief Complete data structure as return value of get
    */
    struct Value
    {
        /**Primary Key*/
        string            _mkey;        /**主key*/
        /**Other Data*/
        BlockData        _data;        /**其它数据*/
    };

    /**
    * @brief 判断某个标志位是否已经设置
    * @brief Determine if a flag bit has been set
    * @param bitset, 要判断的字节
    * @param bitset, Bytes to Judge
    * @param bw, 要判断的位
    * @param bw, Bits to Judge
    */
    static bool ISSET(uint8_t bitset, uint8_t bw) { return bool((bitset & (0x01 << bw)) >> bw); }
    
    /**
    * @brief 设置某个标志位
    * @brief Set a flag bit
    */
    static void SET(uint8_t &iBitset, uint8_t bw)
    {
        iBitset |= 0x01 << bw;
    }
    
    /**
    * @brief 清空某个标志位
    * @brief Empty a mark
    */
    static void UNSET(uint8_t &iBitset, uint8_t bw)
    {
        iBitset &= 0xFF ^ (0x01 << bw);
    }

    //////////////////////////////////////////////////////////////////////////
    /**
     * @brief 主key数据块
     * @brief Main key data block
     */
    class MainKey
    {
        /**
        * @brief 头部中bitwise位的意义
        * @brief Meaning of bitwise bit in head
        */
        enum BITWISE
        {
            /**Is there the next chunk*/
            NEXTCHUNK_BIT = 0,        /**是否有下一个chunk*/
            /**Is the data under the main key complete*/
            INTEGRITY_BIT,            /**主key下的数据是否完整*/
        };

    public:
        /** 
          * @brief 主key头
          * @brief Main key head
          */
#pragma pack(1) 

        struct tagMainKeyHead
        {
            /**Capacity Size*/
            uint32_t    _iSize;         /**容量大小*/
            /**Primary key hash index*/
            uint32_t    _iIndex;        /**主key hash索引*/
            /**Header address of data chain under primary key*/
            uint32_t    _iAddr;            /**主key下数据链首地址*/
            /**Next main key in the main key chain*/
            uint32_t    _iNext;            /**主key链的下一个主key*/
            /**Previous main key in the main key chain*/
            uint32_t    _iPrev;            /**主key链的上一个主key*/
            /**Next main key in the main key Get chain*/
            uint32_t    _iGetNext;        /**主key Get链的下一个主key*/
            /**Previous primary key in the main key Get chain*/
            uint32_t    _iGetPrev;        /**主key Get链的上一个主key*/
            /**Number of data under primary key*/
            uint32_t    _iBlockCount;    /**主key下数据个数*/
            /**8 bits, identifying different bool values. See BITWISE enumeration definition for the meaning of each bit*/
            uint8_t     _iBitset;        /** 8个bit，用于标识不同的bool值，各bit的含义见BITWISE枚举定义*/
            union
            {
                /**Next Chunk block address, valid when _bNextChunk=true*/
                uint32_t    _iNextChunk;    /** 下一个Chunk块地址, _bNextChunk=true时有效*/
                /**Length used in current data block, valid when _bNextChunk=false*/
                uint32_t    _iDataLen;      /** 当前数据块中使用了的长度, _bNextChunk=false时有效*/
            };
            /**Data Start Address*/
            char            _cData[0];      /** 数据开始地址*/
            tagMainKeyHead()
                : _iSize(0)
                , _iIndex(0)
                , _iAddr(0)
                , _iNext(0)
                , _iPrev(0)
                , _iGetNext(0)
                , _iGetPrev(0)
                , _iBlockCount(0)
                , _iBitset(0)
                , _iDataLen(0)
            {
                _cData[0] = 0;
            }
        };

         /**
         * @brief 
         *        一个chunk放不下数据，后面挂接其它chunk,非第一个chunk的chunk头部
         * @brief One chunk can't hold the data, and the other chunks are hung after it, not the first chunk's chunk header
         */
        struct tagChunkHead
        {
            /**Capacity size of current chunk*/
            uint32_t    _iSize;        /** 当前chunk的容量大小*/
            /**Is there another chunk*/
            bool        _bNextChunk;    /** 是否还有下一个chunk*/
            union
            {
                /**Next Chunk block address, valid when _bNextChunk=true*/
                uint32_t  _iNextChunk;    /** 下一个数据块地址, _bNextChunk=true时有效*/
                /**Length used in current data block, valid when _bNextChunk=false*/
                uint32_t  _iDataLen;      /** 当前数据块中使用了的长度, _bNextChunk=false时有效*/
            };
            /**Data Start Address*/
            char        _cData[0];      /** 数据开始地址*/

            tagChunkHead()
            :_iSize(0)
            ,_bNextChunk(false)
            ,_iDataLen(0)
            {
                _cData[0] = 0;
            }

        };
#pragma pack() 

        /**
         * @brief 构造函数
         * @brief Constructor
         * @param pMap
         * @param iAddr, 主key的地址
         * @param iAddr, Main key address
         */
        MainKey(TC_Multi_HashMap *pMap, uint32_t iAddr)
        : _pMap(pMap)
        , _iHead(iAddr)
        {
            // 构造时保存绝对地址，避免每次都计算
            // Save absolute address when constructing, avoid calculating every time
            _pHead = _pMap->getAbsolute(iAddr);
        }

        /**
         * @brief 拷贝构造函数
         * @brief Copy the Constructor
         * @param mk
         */
        MainKey(const MainKey &mk)
        : _pMap(mk._pMap)
        , _iHead(mk._iHead)
        , _pHead(mk._pHead)
        {
        }

        /**
         * @brief 赋值操作符
         * @brief Assignment operator
         * @param mk
         *
         * @return Block&
         */
        MainKey& operator=(const MainKey &mk)
        {
            _iHead = mk._iHead;
            _pMap  = mk._pMap;
            _pHead = mk._pHead;
            return (*this);
        }

        /**
         *
         * @param mb
         *
         * @return bool
         */
        bool operator==(const MainKey &mk) const { return _iHead == mk._iHead && _pMap == mk._pMap; }

        /**
         *
         * @param mb
         *
         * @return bool
         */
        bool operator!=(const MainKey &mk) const { return _iHead != mk._iHead || _pMap != mk._pMap; }

        /**
         * @brief 获取主key头指针
         * @brief Get the primary key header pointer
         *
         * @return tagMainKeyHead*
         */
        tagMainKeyHead* getHeadPtr() { return (tagMainKeyHead *)_pHead; }


        /**
         * @brief 根据主key头地址获取主key头指针
         * @brief Get the primary key header pointer based on the primary key header address
         * @param iAddr, 主key头地址
         * @param iAddr, Primary key header address
         * @return tagMainKeyHead*
         */
        tagMainKeyHead* getHeadPtr(uint32_t iAddr) { return ((tagMainKeyHead*)_pMap->getAbsolute(iAddr)); }

        /**
         * @brief 获取主key头的地址
         * @brief Get the address of the primary key header
         *
         * @return uint32_t
         */
        uint32_t getHead() { return _iHead; }
        
        /**
         * @brief 获取主key
         * @brief Get the master key
         * @param mk, 主key
         * @param mk, Main key
         * @return int
         *          TC_Multi_HashMap::RT_OK, 正常
         *          TC_Multi_HashMap::RT_OK, normal
         *          其他异常
         *          Other Exceptions
         */
        int get(string &mk);

        /**
         * @brief 设置主key
         * @brief Set the Main Key
         * @param pData
         * @param iDatalen
         * @param vtData, 淘汰的数据
         * @param vtData, Eliminated data
         */
        int set(const void *pData, uint32_t iDataLen, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 将当前主key移动到主key链上的下一个主key
         * @brief Move the current primary key to the next primary key in the main key chain
         * @return true, 移到下一个主key了, false, 没有下一个主key
         * @return True, moved to the next primary key, false, no next primary key
         *
         */
        bool next();

        /**
         * @brief 将当前主key移动到主key链上的上一个主key
         * @brief Move the current primary key to the last primary key on the main key chain
         * @return true, 移到上一个主key了, false, 没有上一个主key
         * @return True, moved to the previous primary key, false, no previous primary key
         *
         */
        bool prev();

        /**
         * @brief 释放所有空间
         * @brief Free up all space
         */
        void deallocate();

        /**
         * @brief 新主key时调用该函数，初始化主key相关信息
         * @brief This function is called when a new primary key is created to initialize information about the primary key
         * @param iIndex, 主key hash索引
         * @param iIndex, Primary key hash index
         * @param iAllocSize, 内存大小
         * @param iAllocSize, Memory size
         */
        void makeNew(uint32_t iIndex, uint32_t iAllocSize);

        /**
         * @brief 从主key链表中删除当前主key
         * @brief Delete the current main key from the main key chain list
         * 返回被删除的主key下的所有数据
         * Returns all data under the deleted primary key
         * @return int, TC_Multi_HashMap::RT_OK成功，其它失败
         * @return Int, TC_Multi_HashMap::RT_OK Success, Other Failures
         */
        int erase(vector<Value> &vtData);

        /**
         * @brief 刷新get链表, 将当前主key放在get链表头部
         * @brief Refresh the get list, place the current primary key at the head of the get list
         */
        void refreshGetList();

    protected:

        /**
         * @brief 获取chunk头指针
         * @brief Get chunk header pointer
         * @param iAddr, chunk头地址索引
         * @param iAddr, Chunk header address index
         *
         * @return tagChunkHead*
         */
        tagChunkHead *getChunkHead(uint32_t iAddr) { return ((tagChunkHead*)_pMap->getAbsolute(iAddr)); }

        /**
         * @brief 如果数据容量不够, 则新增加chunk, 不影响原有数据
         * @brief If the data capacity is insufficient, add a new chunk without affecting the original data
         * 使新增加的总容量大于iDataLen
         * Make the total capacity added larger than iDataLen
         * 释放多余的chunk
         * Release extra chunks
         * @param iDataLen
         * @param vtData, 返回被淘汰的数据
         * @param vtData, Return eliminated data
         *
         * @return int
         */
        int allocate(uint32_t iDataLen, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 挂接chunk, 如果core则挂接失败, 保证内存块还可以用
         * @brief Hook chunk, if core fails to hook, ensuring that memory blocks are still available
         * @param pChunk, 第一个chunk指针
         * @param pChunk, First chunk pointer
         * @param chunks, 所有的chunk地址
         * @param chunks, All chunk addresses
         *
         * @return int
         */
        int joinChunk(tagChunkHead *pChunk, const vector<uint32_t>& chunks);

        /**
         * @brief 分配指定大小的内存空间，可能会有多个chunk
         * @brief Allocate memory space of a specified size, possibly with more than one chunk
         * @param fn, 分配的空间大小
         * @param fn, Allocated space size
         * @param chunks, 分配成功返回的chunks地址列表
         * @param chunks, A list of chunks addresses that were successfully assigned back
         * @param vtData, 淘汰的数据
         * @param vtData, Eliminated data
         * @return int
         */
        int allocateChunk(uint32_t fn, vector<uint32_t> &chunks, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 释放指定chunk之后的所有chunk
         * @brief Release all chunks after the specified chunk
         * @param iChunk 释放地址
         * @param iChunk Release address
         */
        void deallocate(uint32_t iChunk);

        /**
         * @brief 获取主key存储空间大小
         * @brief Get primary key storage size
         *
         * @return uint32_t
         */
        uint32_t getDataLen();

        /**
         * @brief 获取主key
         * @brief Get the master key
         * @param pData
         * @param iDatalen
         * @return int,
         *          TC_Multi_HashMap::RT_OK, 正常
         *          TC_Multi_HashMap::RT_OK, normal
         *          其他异常
         *          other exceptions
         */
        int get(void *pData, uint32_t &iDataLen);

    public:
        bool ISFULLDATA()                        { return TC_Multi_HashMap::ISSET(getHeadPtr()->_iBitset, INTEGRITY_BIT); }
    protected:
        bool HASNEXTCHUNK()                        { return TC_Multi_HashMap::ISSET(getHeadPtr()->_iBitset, NEXTCHUNK_BIT); }
        
    public:
        void SETFULLDATA(bool b)                { if(b) TC_Multi_HashMap::SET(getHeadPtr()->_iBitset, INTEGRITY_BIT); else TC_Multi_HashMap::UNSET(getHeadPtr()->_iBitset, INTEGRITY_BIT); }
    protected:
        void SETNEXTCHUNK(bool b)                { if(b) TC_Multi_HashMap::SET(getHeadPtr()->_iBitset, NEXTCHUNK_BIT); else TC_Multi_HashMap::UNSET(getHeadPtr()->_iBitset, NEXTCHUNK_BIT); }

    private:

        /**
         * Map
         */
        TC_Multi_HashMap         *_pMap;

        /**
         * 主key头首地址, 相对地址，内存块索引
         * Primary key header header address, relative address, memory block index
         */
        uint32_t                _iHead;

        /**
        * 主key头首地址，绝对地址
        * Primary key header header address, absolute address
        */
        void                    *_pHead;

    };

    ///////////////////////////////////////////////////////////////////////////////////
    /**
    * @brief 联合主key及数据块
    * @brief Union primary key and data block
    */
    class Block
    {
    public:

        /**
        * @brief block头部中bitwise位的意义
        * @brief Meaning of the bitwise bit in the head of a block
        */
        enum BITWISE
        {
            /**Is there the next chunk*/
            NEXTCHUNK_BIT = 0,        /**是否有下一个chunk*/
            /**Is it a dirty bit*/
            DIRTY_BIT,                /**是否为脏数据位*/
            /**Is it the OnlyKey*/
            ONLYKEY_BIT,            /**是否为OnlyKey*/
        };

        /**
         * @brief block数据头
         * @brief Block header
         */
#pragma pack(1) 
        struct tagBlockHead
        {
            /**Capacity Size of Block*/
            uint32_t        _iSize;         /**block的容量大小*/
            /**Index of hash*/
            uint32_t        _iIndex;        /**hash的索引*/
            /**The next block in the union primary key block chain, if none, is 0*/
            uint32_t        _iUKBlockNext;  /**联合主键block链的下一个Block, 没有则为0*/
            /**The previous block in the union primary key block chain, if none, is 0*/
            uint32_t        _iUKBlockPrev;  /**联合主键block链的上一个Block, 没有则为0*/
            /**Next block in the main key block chain, 0 if none*/
            uint32_t        _iMKBlockNext;  /**主key block链的下一个Block, 没有则为0*/
            /**The previous block in the main key block chain, if none, is 0*/
            uint32_t        _iMKBlockPrev;  /**主key block链的上一个Block, 没有则为0*/
            /**Previous Block on Set Chain, 0 if none*/
            uint32_t        _iSetNext;      /**Set链上的上一个Block, 没有则为0*/
            /**Previous Block on Set Chain, 0 if none*/
            uint32_t        _iSetPrev;      /**Set链上的上一个Block, 没有则为0*/
            /**Point to the owning primary key header*/
            uint32_t        _iMainKey;        /**指向所属主key头*/
            /**Last slow write time*/
            time_t          _iSyncTime;     /**上次缓写时间*/
            /**Data Version, 1 for Initial Version, 0 for Retention*/
            uint8_t         _iVersion;        /**数据版本，1为初始版本，0为保留*/
            /**8 bits, identifying different bool values. See BITWISE enumeration definition for the meaning of each bit*/
            uint8_t         _iBitset;        /**8个bit，用于标识不同的bool值，各bit的含义见BITWISE枚举定义*/
            union
            {
                /**Next Chunk block, NEXTCHUNK_BIT in _iBitwise is valid when 1*/
                uint32_t    _iNextChunk;    /**下一个Chunk块, _iBitwise中的NEXTCHUNK_BIT为1时有效*/
                /**Length used in current data block, valid if NEXTCHUNK_BIT in _iBitwise is 0*/
                uint32_t    _iDataLen;      /**当前数据块中使用了的长度, _iBitwise中的NEXTCHUNK_BIT为0时有效*/
            };
            /**Beginning of Data*/
            char            _cData[0];      /**数据开始部分*/

            tagBlockHead()
            :_iSize(0)
            ,_iIndex(0)
            ,_iUKBlockNext(0)
            ,_iUKBlockPrev(0)
            ,_iMKBlockNext(0)
            ,_iMKBlockPrev(0)
            ,_iSetNext(0)
            ,_iSetPrev(0)
            ,_iMainKey(0)
            ,_iSyncTime(0)
            ,_iVersion(1)
            ,_iBitset(0)
            ,_iDataLen(0)
            {
                _cData[0] = 0;
            }

        };

        /**
         * @brief 
         *        一个chunk放不下所有数据，后面挂接其它chunk,其它chunk头部
         * @brief One chunk can't hold all the data, then hook up the other chunks, the other chunk headers
         */
        struct tagChunkHead
        {
            /**Capacity size of current chunk*/
            uint32_t    _iSize;          /**当前chunk的容量大小*/
            /**Is there another chunk*/
            bool        _bNextChunk;     /**是否还有下一个chunk*/
            union
            {
                /**Next chunk, valid when _bNextChunk=true*/
                uint32_t  _iNextChunk;     /**下一个chunk, _bNextChunk=true时有效*/
                /**Length used in current chunk, valid when _bNextChunk=false*/
                uint32_t  _iDataLen;       /**当前chunk中使用了的长度, _bNextChunk=false时有效*/
            };
            /**Beginning of Data*/
            char        _cData[0];      /**数据开始部分*/

            tagChunkHead()
            :_iSize(0)
            ,_bNextChunk(false)
            ,_iDataLen(0)
            {
                _cData[0] = 0;
            }

        };
#pragma pack() 

        /**
         * @brief 构造函数
         * @brief Constructor
         * @param Map
         * @param Block的地址
         * @param pAdd
         */
        Block(TC_Multi_HashMap *pMap, uint32_t iAddr)
        : _pMap(pMap)
        , _iHead(iAddr)
        {
            // 构造时保存绝对地址，避免每次都计算
            // Save absolute address when constructing, avoid calculating every time
            _pHead = _pMap->getAbsolute(iAddr);
        }

        /**
         * @brief 拷贝构造函数
         * @brief Copy constructor
         * @param mb
         */
        Block(const Block &mb)
        : _pMap(mb._pMap)
        , _iHead(mb._iHead)
        , _pHead(mb._pHead)
        {
        }

        /**
         * @brief  赋值运算符
         * @brief  Assignment Operators
         * @param mb
         *
         * @return Block&
         */
        Block& operator=(const Block &mb)
        {
            _iHead = mb._iHead;
            _pMap  = mb._pMap;
            _pHead = mb._pHead;
            return (*this);
        }

        /**
         *
         * @param mb
         *
         * @return bool
         */
        bool operator==(const Block &mb) const { return _iHead == mb._iHead && _pMap == mb._pMap; }

        /**
         *
         * @param mb
         *
         * @return bool
         */
        bool operator!=(const Block &mb) const { return _iHead != mb._iHead || _pMap != mb._pMap; }

        /**
         * @brief 获取Block头指针
         * @brief Get Block Header Pointer
         *
         * @return tagBlockHead*
         */
        tagBlockHead *getBlockHead() {return (tagBlockHead*)_pHead; }

        /**
         * @brief 根据头地址获取Block头指针
         * @brief Get Block Header Pointer from Header Address
         * @param iAddr, block头地址
         * @param iAddr, Block header address
         * @return tagBlockHead*
         */
        tagBlockHead *getBlockHead(uint32_t iAddr) { return ((tagBlockHead*)_pMap->getAbsolute(iAddr)); }

        /**
         * @brief 获取头部地址
         * @brief Get Header Address
         *
         * @return size_t
         */
        uint32_t getHead() { return _iHead;}

        /**
         * @brief 获取当前桶链表最后一个block的头部地址
         * @brief Get the header address of the last block in the current bucket chain list
         * @param bUKList, 是联合主键链还是主key链
         * @param bUKList, Is it a union primary key chain or a primary key chain?
         * @return uint32_t
         */
        uint32_t getLastBlockHead(bool bUKList);

        /**
         * @brief 获取回写时间
         * @brief Get Writeback Time
         *
         * @return time_t
         */
        time_t getSyncTime() { return getBlockHead()->_iSyncTime; }

        /**
         * @brief 设置回写时间
         * @brief Set Writeback Time
         * @param iSyncTime
         */
        void setSyncTime(time_t iSyncTime) { getBlockHead()->_iSyncTime = iSyncTime; }

        /**
        * @brief 获取数据版本
        * @brief Get Data Version
        */
        uint8_t getVersion() { return getBlockHead()->_iVersion; }

        /**
        * @brief 设置数据版本
        * @brief Set Data Version
        */
        void setVersion(uint8_t iVersion) { getBlockHead()->_iVersion = iVersion; }

        /**
         * @brief 获取Block中的数据
         * @brief Get data from a Block
         * @param data 要获取数据的Block
         * @param data Block to get data
         * @return int
         *          TC_Multi_HashMap::RT_OK, 正常, 其他异常
         *          TC_Multi_HashMap::RT_OK, normal, other abnormalities
         *          TC_Multi_HashMap::RT_ONLY_KEY, 只有Key
         *          TC_Multi_HashMap::RT_ONLY_KEY, only Key
         *          其他异常
         *          other exceptions
         */
        int getBlockData(TC_Multi_HashMap::BlockData &data);

        /**
         * @brief 获取原始数据
         * @brief Get raw data
         * @param pData
         * @param iDatalen
         * @return int,
         *          TC_Multi_HashMap::RT_OK, 正常
         *          TC_Multi_HashMap::RT_OK, normal
         *          其他异常
         *          other exceptions
         */
        int get(void *pData, uint32_t &iDataLen);

        /**
         * @brief 获取原始数据
         * @brief Get raw data
         * @param s
         * @return int
         *          TC_Multi_HashMap::RT_OK, 正常
         *          TC_Multi_HashMap::RT_OK, normal
         *          其他异常
         *          other ecxeptions
         */
        int get(string &s);

        /**
         * @brief 设置数据
         * @brief Set data
         * @param pData
         * @param iDatalen
         * @param bOnlyKey
         * @param iVersion, 数据版本，应该根据get出的数据版本写回，为0表示不关心数据版本
         * @param iVersion, Data version, which should be written back based on the data version from get, is 0 to indicate that you don't care about the data version
         * @param vtData, 淘汰的数据
         * @param vtData, Eliminated data
         * @return int
         *                RT_OK, 设置成功
         *                RT_OK, Set successfully
         *                RT_DATA_VER_MISMATCH, 要设置的数据版本与当前版本不符，应该重新get后再set
         *                RT_DATA_VER_MISMATCH, The data version you want to set does not match the current version. You should get it again before you set it
         *                其它为失败
         *                Others are failures
         */
        int set(const void *pData, uint32_t iDataLen, bool bOnlyKey, uint8_t iVersion, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 是否是脏数据
         * @brief Is it dirty data
         *
         * @return bool
         */
        bool isDirty()      { return ISDIRTY(); }

        /**
         * @brief 设置数据
         * @brief Set data
         * @param b
         */
        void setDirty(bool b);

        /**
         * @brief 是否只有key
         * @brief Is there only key
         *
         * @return bool
         */
        bool isOnlyKey()    { return ISONLYKEY(); }

        /**
         * @brief 当前元素移动到联合主键block链的下一个block
         * @brief The current element moves to the next block in the union primary key block chain
         * @return true, 移到下一个block了, false, 没有下一个block
         * @return True, moved to the next block, false, no next block
         *
         */
        bool nextBlock();

        /**
         * @brief 当前元素移动到联合主键block链的上一个block
         * @brief The current element moves to the previous block in the union primary key block chain
         * @return true, 移到上一个block了, false, 没有上一个block
         * @return True, moved to the previous block, false, no previous block
         *
         */
        bool prevBlock();

        /**
         * @brief 释放block的所有空间
         * @brief Release all space from the block
         */
        void deallocate();

        /**
         * @brief 新block时调用该函数
         * @brief Call this function when a new block
         * 初始化新block的一些信息
         * Some information about initializing a new block
         * @param iMainKeyAddr, 所属主key地址
         * @param iMainKeyAddr, Owning Primary Key Address
         * @param uIndex, 联合主键hash索引
         * @param uIndex, Joint Primary Key hash Index
         * @param iAllocSize, 内存大小
         * @param iAllocSize, memory size
         * @param bHead, 插入到主key链上的顺序，前序或后序
         * @param bHead, The order, prefix, or postorder inserted into the main key chain
         */
        void makeNew(uint32_t iMainKeyAddr, uint32_t uIndex, uint32_t iAllocSize, bool bHead);

        /**
         * @brief 从Block链表中删除当前Block
         * @brief Delete the current block from the block list
         * @return
         */
        void erase();

        /**
         * @brief 刷新set链表, 将当前block放在Set链表头部
         * @brief Refresh the set list and place the current block in the head of the set list
         */
        void refreshSetList();

    protected:

        /**
         * @brief 根据chunk头地址获取chunk头指针
         * @brief Get the chunk header pointer from the chunk header address
         * @param iAddr
         *
         * @return tagChunkHead*
         */
        tagChunkHead *getChunkHead(uint32_t iAddr) { return ((tagChunkHead*)_pMap->getAbsolute(iAddr)); }

        /**
         * @brief 如果数据容量不够, 则新增加chunk, 不影响原有数据
         * @brief If the data capacity is insufficient, add a new chunk without affecting the original data
         * 使新增加的总容量大于iDataLen，释放多余的chunk 
         * Make the newly added total capacity larger than iDataLen, releasing extra chunks
         * @param iDataLen
         * @param vtData, 淘汰的数据
         * @param vtData, Eliminated data
         *
         * @return int,
         */
        int allocate(uint32_t iDataLen, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 挂接chunk, 如果core则挂接失败, 保证内存块还可以用
         * @brief Hook chunk, if core fails to hook, ensuring that memory blocks are still available
         * @param pChunk, 第一个chunk指针
         * @param pChunk, First chunk pointer
         * @param chunks, 所有的chunk地址
         * @param chunks, All chunk addresses
         *
         * @return int
         */
        int joinChunk(tagChunkHead *pChunk, const vector<uint32_t>& chunks);

        /**
         * @brief 分配指定大小的内存空间, 可能会有多个chunk
         * @brief Allocate memory space of a specified size, possibly with more than one chunk
         * @param fn, 分配的空间大小
         * @param fn, Allocated space size
         * @param chunks, 分配成功返回的chunks地址列表
         * @param chunks, A list of chunks addresses that were successfully assigned back
         * @param vtData, 淘汰的数据
         * @param vtData, Eliminated data
         * @return int
         */
        int allocateChunk(uint32_t fn, vector<uint32_t> &chunks, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 释放指定chunk之后的所有chunk
         * @brief Release all chunks after the specified chunk
         * @param iChunk 释放地址
         * @param iChunk Release address
         */
        void deallocate(uint32_t iChunk);

        /**
         * @brief 获取数据长度
         * @brief Get data length
         *
         * @return size_t
         */
        uint32_t getDataLen();

        bool HASNEXTCHUNK()                        { return TC_Multi_HashMap::ISSET(getBlockHead()->_iBitset, NEXTCHUNK_BIT); }
        bool ISDIRTY()                            { return TC_Multi_HashMap::ISSET(getBlockHead()->_iBitset, DIRTY_BIT); }
        bool ISONLYKEY()                        { return TC_Multi_HashMap::ISSET(getBlockHead()->_iBitset, ONLYKEY_BIT); }

        void SETNEXTCHUNK(bool b)                { if(b) TC_Multi_HashMap::SET(getBlockHead()->_iBitset, NEXTCHUNK_BIT); else TC_Multi_HashMap::UNSET(getBlockHead()->_iBitset, NEXTCHUNK_BIT); }
        void SETDIRTY(bool b)                    { if(b) TC_Multi_HashMap::SET(getBlockHead()->_iBitset, DIRTY_BIT); else TC_Multi_HashMap::UNSET(getBlockHead()->_iBitset, DIRTY_BIT); }
        void SETONLYKEY(bool b)                    { if(b) TC_Multi_HashMap::SET(getBlockHead()->_iBitset, ONLYKEY_BIT); else TC_Multi_HashMap::UNSET(getBlockHead()->_iBitset, ONLYKEY_BIT); }

    private:

        /**
         * Map
         */
        TC_Multi_HashMap         *_pMap;

        /**
         * block区块首地址, 相对地址，内存块索引
         * Block Block Header Address, Relative Address, Memory Block Index
         */
        uint32_t                _iHead;

        /**
        * Block首地址，绝对地址
        * Block header address, absolute address
        */
        void                    *_pHead;

    };

    ////////////////////////////////////////////////////////////////////////
    /**
    * @brief 内存数据块分配器，可同时为数据区和主key区分配内存
    * @brief A memory data block allocator that allocates memory for both the data area and the primary key area
    *
    */
    class BlockAllocator
    {
    public:

        /**
         * @brief 构造函数
         * @brief Constructor
         */
        BlockAllocator(TC_Multi_HashMap *pMap)
        : _pMap(pMap)
        , _pChunkAllocator(new TC_MemMultiChunkAllocator())
        {
        }

        /**
         * @brief 析够函数
         * @brief Destructor
         */
        ~BlockAllocator()
        {
            if(_pChunkAllocator != NULL)
            {
                delete _pChunkAllocator;
            }
            _pChunkAllocator = NULL;
        }


        /**
         * @brief 初始化
         * @brief Initialization
         * @param pHeadAddr, 地址, 换到应用程序的绝对地址
         * @param pHeadAddr, Address, to the absolute address of the application
         * @param iSize, 内存大小
         * @param iSize, Memory size
         * @param iMinBlockSize, 最小数据块大小
         * @param iMinBlockSize, Minimum data block size
         * @param iMaxBlockSize, 最大数据块大小
         * @param iMaxBlockSize, Maximum data block size
         * @param fFactor, 数据块增长因子
         * @param fFactor, Data Block Growth Factor
         */
        void create(void *pHeadAddr, size_t iSize, size_t iMinBlockSize, size_t iMaxBlockSize, float fFactor)
        {
            _pChunkAllocator->create(pHeadAddr, iSize, iMinBlockSize, iMaxBlockSize, fFactor);
        }

        /**
         * @brief 连接到已经结构化的内存(如共享内存)
         * @brief Connect to already structured memory (such as shared memory)
         * @param pAddr, 地址, 换到应用程序的绝对地址
         * @param pAddr, Address, to the absolute address of the application
         */
        void connect(void *pHeadAddr)
        {
            _pChunkAllocator->connect(pHeadAddr);
        }

        /**
         * @brief 扩展空间
         * @brief Expand space
         * @param pAddr
         * @param iSize
         */
        void append(void *pAddr, size_t iSize)
        {
            _pChunkAllocator->append(pAddr, iSize);
        }

        /**
         * @brief 重建内存结构
         * @brief Rebuild Memory Structure
         */
        void rebuild()
        {
            _pChunkAllocator->rebuild();
        }

        /**
         * @brief 获取每种数据块头部信息
         * @brief Get header information for each data block
         *
         * @return vector<TC_MemChunk::tagChunkHead>
         */
        vector<TC_MemChunk::tagChunkHead> getBlockDetail() const  { return _pChunkAllocator->getBlockDetail(); }

        /**
         * @brief 总内存大小
         * @brief Total memory size
         *
         * @return size_t
         */
        size_t getMemSize() const       { return _pChunkAllocator->getMemSize(); }

        /**
         * @brief 实际的数据容量
         * @brief Actual data capacity
         *
         * @return size_t
         */
        size_t getCapacity() const      { return _pChunkAllocator->getCapacity(); }

        /**
         * @brief 每种block中的chunk个数(每种block中的chunk个数相同)
         * @brief Number of chunks per block (same number of chunks per block)
         *
         * @return vector<size_t>
         */
        vector<size_t> singleBlockChunkCount() const { return _pChunkAllocator->singleBlockChunkCount(); }

        /**
         * @brief 所有block的chunk个数
         * @brief Number of chunks for all blocks
         *
         * @return size_t
         */
        size_t allBlockChunkCount() const    { return _pChunkAllocator->allBlockChunkCount(); }

        /**
         * @brief 在内存中分配一个新的Block，实际上只分配一个chunk， 
         *        并初始化Block头
         * @brief Assign a new block in memory, actually only one chunk, and initialize the Block header
         * @param iMainKeyAddr, 新block所属主key地址
         * @param iMainKeyAddr, The primary key address to which the new block belongs
         * @param index, block hash索引
         * @param index, block hash index
         * @param bHead, 新块插入到主key链上的顺序，前序或后序
         * @param bHead, Key, or the order in which the main chain is inserted
         * @param iAllocSize: in/需要分配的大小, out/分配的块大小
         * @param iAllocSize: in/Size to be allocated, out/Allocated block size
         * @param vtData, 返回淘汰的数据
         * @param vtData, Return obsolete data
         * @return size_t, 内存块地址索引, 0表示没有空间可以分配
         * @return size_t, Memory block address index, 0 means there is no space to allocate
         */
        uint32_t allocateMemBlock(uint32_t iMainKeyAddr, uint32_t index, bool bHead, uint32_t &iAllocSize, vector<TC_Multi_HashMap::Value> &vtData);

        /**
        * @brief 在内存中分配一个主key头，只需要一个chunk即可
        * @brief To allocate a main key header in memory, only one chunk is needed
        * @param index, 主key hash索引
        * @param index, Main key hash index
        * @param iAllocSize: in/需要分配的大小, out/分配的块大小
        * @param iAllocSize: in/Size to be allocated, out/Allocated block size
        * @param vtData, 返回释放的内存块数据
        * @param vtData, Returns the released memory block data
        * @return size_t, 主key头首地址,0表示没有空间可以分配
        * @return size_t, Primary key header address, 0 means there is no space to allocate
        */
        uint32_t allocateMainKeyHead(uint32_t index, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 为地址为iAddr的Block分配一个chunk         *
         * @brief Assign a chunk to the block with the address iaddr
         * @param iAddr,分配的Block的地址
         * @param iAddr,Assigned address block
         * @param iAllocSize, in/需要分配的大小, out/分配的块大小
         * @param iAllocSize, in/Size to be allocated, out/Allocated block size
         * @param vtData 返回释放的内存块数据
         * @param vtData Returns the released memory block data
         * @return size_t, 相对地址,0表示没有空间可以分配
         * @return size_t, Relative to the address, 0 means there is no space to allocate
         */
        uint32_t allocateChunk(uint32_t iAddr, uint32_t &iAllocSize, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 释放Block
         * @brief Release block
         * @param v, 需要释放的chunk的地址列表
         * @param v, Address list of chunks to be released
         */
        void deallocateMemChunk(const vector<uint32_t> &v);

        /**
         * @brief 释放Block
         * @brief Release block
         * @param iChunk, 需要释放的chunk地址
         * @param iChunk, Chunk address to be released
         */
        void deallocateMemChunk(uint32_t iChunk);

    protected:
        /**
         * @brief 不允许copy构造
         * @brief Copy construction is not allowed
         */
        BlockAllocator(const BlockAllocator &);
        /**
         * @brief 不允许赋值
         * @brief Assignment not allowed
         */
        BlockAllocator& operator=(const BlockAllocator &);
        bool operator==(const BlockAllocator &mba) const;
        bool operator!=(const BlockAllocator &mba) const;

    public:
        /**
         * map
         */
        TC_Multi_HashMap                *_pMap;

        /**
         * chunk分配器
         * Chunk distributor
         */
        TC_MemMultiChunkAllocator        *_pChunkAllocator;
    };

    ////////////////////////////////////////////////////////////////
    /** 
      * @brief 构造map的数据项
      * @brief Construct data items of map
      */
    class HashMapLockItem
    {
    public:

        /**
         *
         * @param pMap, 
         * @param iAddr, 与LockItem对应的Block首地址
         * @param iAddr, Block first address corresponding to lockitem
         */
        HashMapLockItem(TC_Multi_HashMap *pMap, uint32_t iAddr);

        /**
         *
         * @param mcmdi
         */
        HashMapLockItem(const HashMapLockItem &mcmdi);

        /**
         *
         * @param mcmdi
         *
         * @return HashMapLockItem&
         */
        HashMapLockItem &operator=(const HashMapLockItem &mcmdi);

        /**
         *
         * @param mcmdi
         *
         * @return bool
         */
        bool operator==(const HashMapLockItem &mcmdi);

        /**
         *
         * @param mcmdi
         *
         * @return bool
         */
        bool operator!=(const HashMapLockItem &mcmdi);

        /**
         * @brief 是否是脏数据
         * @brief Is it dirty data
         *
         * @return bool
         */
        bool isDirty();

        /**
         * @brief 是否只有Key
         * @brief Is there only key
         *
         * @return bool
         */
        bool isOnlyKey();

        /**
         * @brief 最后Sync时间
         * @brief Last sync time
         *
         * @return time_t
         */
        time_t getSyncTime();

        /**
         * @brief 获取键与值
         * @brief Get keys and values
         * @param v
         * @return int
         *          RT_OK:数据获取OK
         *          RT_OK:Data acquisition OK
         *          RT_ONLY_KEY: key有效, v无效为空
         *          RT_ONLY_KEY: Key is valid, V is invalid and null
         *          其他值, 异常
         *          Other values, exception
         *
         */
        int get(TC_Multi_HashMap::Value &v);

        /**
         * @brief 仅获取key
         * @brief Get key only
         * @param mk, 主key
         * @param mk, main key
         * @param uk, 联合主key(除主key外的联合主键)
         * @param uk, Joint primary key (joint primary key except primary key)
         * @return int
         *          RT_OK:数据获取OK
         *          RT_OK: data acquisition OK
         *          其他值, 异常
         *          Other values, exception
         */
        int get(string &mk, string &uk);

        /**
         * @brief 获取对应block的相对地址
         * @brief Get the relative address of the corresponding block
         *
         * @return size_t
         */
        uint32_t getAddr() const { return _iAddr; }

    protected:

        /**
         * @brief 设置数据
         * @brief Set data
         * @param mk, 主key
         * @param mk, main key
         * @param uk, 除主key外的联合主键
         * @param uk, Union primary key except primary key
         * @param v, 数据值
         * @param v, data value
         * @param iVersion, 数据版本(1-255), 0表示不关注版本
         * @param iVersion, Data version (1-255), 0 indicates that the version is not concerned
         * @param vtData, 淘汰的数据
         * @param vtData, eliminated data 
         * @return int
         */
        int set(const string &mk, const string &uk, const string& v, uint8_t iVersion, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 设置Key, 无数据(Only Key)
         * @brief Set key, no data (only key)
         * @param mk, 主key
         * @param mk, main key
         * @param uk, 除主key外的联合主键
         * @param uk, Union primary key except primary key
         * @param vtData, 淘汰的数据
         * @param vtData, eliminated data 
         *
         * @return int
         */
        int set(const string &mk, const string &uk, vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 判断当前item是否是指定key的item, 如果是还返回value
         * @brief Judge whether the current item is the item of the specified key, and if so, return value
         * @param pKey
         * @param iKeyLen
         *
         * @return bool
         */
        bool equal(const string &mk, const string &uk, TC_Multi_HashMap::Value &v, int &ret);

        /**
         * @brief 判断当前item是否是指定key的item
         * @brief Judge whether the current item is the item of the specified key
         * @param pKey
         * @param iKeyLen
         *
         * @return bool
         */
        bool equal(const string &mk, const string &uk, int &ret);

        /**
         * @brief 将当前item移动到下一个item
         * @brief Moves the current item to the next item
         *
         * @return HashMapLockItem
         */
        void nextItem(int iType);

        /**
         * @brief 将当前item移动到上一个item
         * @brief Moves the current item to the previous item
         * @param iType
         */
        void prevItem(int iType);

        friend class TC_Multi_HashMap;
        friend struct TC_Multi_HashMap::HashMapLockIterator;

    private:
        /**
         * map
         */
        TC_Multi_HashMap *_pMap;

        /**
         * 对应的block的地址
         * Address of the corresponding block
         */
        uint32_t      _iAddr;
    };

    /////////////////////////////////////////////////////////////////////////
    /** 
      * @brief  定义迭代器 
      * @brief  Defining iterators
       */ 
    struct HashMapLockIterator
    {
    public:

        /** 
          * @brief 定义遍历方式
          * @brief Define traversal mode
          */
        enum
        {
            /**Ordinary order*/
            IT_BLOCK    = 0,        /**普通的顺序*/
            /**Set time sequence*/
            IT_SET      = 1,        /**Set时间顺序*/
            /**Get time order*/
            IT_GET      = 2,        /**Get时间顺序*/
            /**Block traversal under the same primary key*/
            IT_MKEY        = 3,        /**同一主key下的block遍历*/
            /**Block traversal under the same union primary key*/
            IT_UKEY        = 4,        /**同一联合主键下的block遍历*/
        };

        /**
         * @brief 迭代器的顺序
         * @brief The order of iterators
         */
        enum
        {
            /**order*/
            IT_NEXT     = 0,        /**顺序*/
            /**reverse order*/
            IT_PREV     = 1,        /**逆序*/
        };

        /**
         *
         */
        HashMapLockIterator();

        /**
         * @brief 构造函数
         * @brief Constructor
         * @param pMap,
         * @param iAddr, 对应的block地址
         * @param iAddr, Corresponding block address
         * @param iType, 遍历类型
         * @param iType, Traversal type
         * @param iOrder, 遍历的顺序
         * @param iOrder, The order of traversal
         */
        HashMapLockIterator(TC_Multi_HashMap *pMap, uint32_t iAddr, int iType, int iOrder);

        /**
         * @brief copy
         * @param it
         */
        HashMapLockIterator(const HashMapLockIterator &it);

        /**
         * @brief 复制
         * @brief copy
         * @param it
         *
         * @return HashMapLockIterator&
         */
        HashMapLockIterator& operator=(const HashMapLockIterator &it);

        /**
         *
         * @param mcmi
         *
         * @return bool
         */
        bool operator==(const HashMapLockIterator& mcmi);

        /**
         *
         * @param mv
         *
         * @return bool
         */
        bool operator!=(const HashMapLockIterator& mcmi);

        /**
         * @brief 前置++
         * @brief Pre++
         *
         * @return HashMapLockIterator&
         */
        HashMapLockIterator& operator++();

        /**
         * @brief 后置++
         * @brief Post++
         *
         * @return HashMapLockIterator&
         */
        HashMapLockIterator operator++(int);

        /**
         *
         *
         * @return HashMapLockItem&i
         */
        HashMapLockItem& operator*() { return _iItem; }

        /**
         *
         *
         * @return HashMapLockItem*
         */
        HashMapLockItem* operator->() { return &_iItem; }

    public:
        /**
         *
         */
        TC_Multi_HashMap  *_pMap;

        /**
         *
         */
        HashMapLockItem _iItem;

        /**
         * 迭代器的方式
         * The way of iterator
         */
        int        _iType;

        /**
         * 迭代器的顺序
         * The order of iterators
         */
        int        _iOrder;

    };

    ////////////////////////////////////////////////////////////////
    /** 
     *  @brief map的HashItem项, 一个HashItem对应多个数据项
     *  @brief The hashitem item item of map. One hashitem corresponds to multiple data items
     */
    class HashMapItem
    {
    public:

        /**
         *
         * @param pMap
         * @param iIndex, Hash索引
         * @param iIndex, Hash Index
         */
        HashMapItem(TC_Multi_HashMap *pMap, uint32_t iIndex);

        /**
         *
         * @param mcmdi
         */
        HashMapItem(const HashMapItem &mcmdi);

        /**
         *
         * @param mcmdi
         *
         * @return HashMapItem&
         */
        HashMapItem &operator=(const HashMapItem &mcmdi);

        /**
         *
         * @param mcmdi
         *
         * @return bool
         */
        bool operator==(const HashMapItem &mcmdi);

        /**
         *
         * @param mcmdi
         *
         * @return bool
         */
        bool operator!=(const HashMapItem &mcmdi);

        /**
         * @brief 获取当前hash桶的所有数据, 注意只获取有key/value的数据
         * 对于只有key的数据, 不获取
         * @brief Get all the data of the current hash bucket. Note that only the data with key / value is obtained
         * For data with only key, it is not obtained
         * @param vtData
         * @return
         */
        void get(vector<TC_Multi_HashMap::Value> &vtData);

        /**
         * @brief 获取当前item的hash索引
         * @brief Gets the hash index of the current item
         * 
         * @return int
         */
        uint32_t getIndex() const { return _iIndex; }

        /**
         * @brief 将当前item移动为下一个item
         * @brief Move the current item to the next item
         *
         */
        void nextItem();

        friend class TC_Multi_HashMap;
        friend struct TC_Multi_HashMap::HashMapIterator;

    private:
        /**
         * map
         */
        TC_Multi_HashMap *_pMap;

        /**
         * 对应的数据块索引
         * Corresponding block index
         */
        uint32_t      _iIndex;
    };

    /////////////////////////////////////////////////////////////////////////
    /**
    * @brief 定义迭代器
    * @brief Defining iterators
    */
    struct HashMapIterator
    {
    public:

        /**
         * @brief 构造函数
         * @brief Constructor
         */
        HashMapIterator();

        /**
         * @brief 构造函数
         * @brief Constructor
         * @param iIndex, hash索引
         * @param iIndex, hash index
         * @param type
         */
        HashMapIterator(TC_Multi_HashMap *pMap, uint32_t iIndex);

        /**
         * @brief copy
         * @param it
         */
        HashMapIterator(const HashMapIterator &it);

        /**
         * @brief 复制
         * @brief copy
         * @param it
         *
         * @return HashMapLockIterator&
         */
        HashMapIterator& operator=(const HashMapIterator &it);

        /**
         *
         * @param mcmi
         *
         * @return bool
         */
        bool operator==(const HashMapIterator& mcmi);

        /**
         *
         * @param mv
         *
         * @return bool
         */
        bool operator!=(const HashMapIterator& mcmi);

        /**
         * @brief 前置++
         * @brief Pre++
         *
         * @return HashMapIterator&
         */
        HashMapIterator& operator++();

        /**
         * @brief 后置++
         * @brief Post++
         *
         * @return HashMapIterator&
         */
        HashMapIterator operator++(int);

        /**
         *
         *
         * @return HashMapItem&i
         */
        HashMapItem& operator*() { return _iItem; }

        /**
         *
         *
         * @return HashMapItem*
         */
        HashMapItem* operator->() { return &_iItem; }

    public:
        /**
         *
         */
        TC_Multi_HashMap  *_pMap;

        /**
         *
         */
        HashMapItem _iItem;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief map头
     * @brief map head
     */
#pragma pack(1) 

    struct tagMapHead
    {
        /**Large version*/
        char        _cMaxVersion;        /**大版本*/
        /**Small version*/
        char        _cMinVersion;        /**小版本*/
        /**Read only*/
        bool        _bReadOnly;          /**是否只读*/
        /**Can it be eliminated automatically*/
        bool        _bAutoErase;         /**是否可以自动淘汰*/
        /**Elimination method: 0x00: according to get chain, 0x01: according to set chain*/
        char        _cEraseMode;         /**淘汰方式:0x00:按照Get链淘汰, 0x01:按照Set链淘汰*/
        /**Memory size*/
        size_t      _iMemSize;           /**内存大小*/
        /**Minimum block size*/
        size_t      _iMinDataSize;       /**最小数据块大小*/
        /**Maximum block size*/
        size_t      _iMaxDataSize;       /**最大数据块大小*/
        /**Factor*/
        float       _fFactor;            /**因子*/
        /**Number of chunks / number of hash*/
        float       _fHashRatio;         /**chunks个数/hash个数*/
        /**Number of chunks / number of primary key hashes*/
        float       _fMainKeyRatio;         /**chunks个数/主key hash个数*/
        /**Total number of elements*/
        size_t      _iElementCount;      /**总元素个数*/
        /**Number of elimination each time*/
        size_t      _iEraseCount;        /**每次淘汰个数*/
        /**Number of dirty data*/
        size_t      _iDirtyCount;        /**脏数据个数*/
        /**Set time list header*/
        uint32_t    _iSetHead;           /**Set时间链表头部*/
        /**Set time chain tail*/
        uint32_t    _iSetTail;           /**Set时间链表尾部*/
        /**Get time list header*/
        uint32_t    _iGetHead;           /**Get时间链表头部*/
        /**End of get time chain list*/
        uint32_t    _iGetTail;           /**Get时间链表尾部*/
        /**Dirty data link tail*/
        uint32_t    _iDirtyTail;         /**脏数据链尾部*/
        /**Hot standby pointer*/
        uint32_t    _iBackupTail;        /**热备指针*/
        /**Write back linked list*/
        uint32_t    _iSyncTail;          /**回写链表*/
        /**Write back time*/
        time_t      _iSyncTime;          /**回写时间*/
        /**Used memory block*/
        size_t      _iUsedChunk;         /**已经使用的内存块*/
        /**Get times*/
        size_t      _iGetCount;          /**get次数*/
        /**Hit Counts */
        size_t      _iHitCount;          /**命中次数*/
        /**Number of onlykeys for primary key*/
        size_t      _iMKOnlyKeyCount;     /**主key的onlykey个数*/
        /**Number of OnlyKeys for the primary key, usually 0*/
        size_t      _iOnlyKeyCount;         /**主键的OnlyKey个数, 这个数通常为0*/
        /**Maximum number of records in the main key chain, which should be monitored and not too large, otherwise the query will be slow*/
        size_t      _iMaxBlockCount;     /**主key链下最大的记录数，这个数值要监控，不能太大，否则会导致查询变慢*/
        /**Retain*/
        size_t      _iReserve[4];        /**保留*/
    };

    /**
     * @brief 需要修改的地址
     * @brief Addresses that need to be modified
     */
    struct tagModifyData
    {
        /**Modified Address*/
        size_t        _iModifyAddr;       /**修改的地址*/
        /**Number of bytes*/
        char        _cBytes;            /**字节数*/
        /**value*/
        size_t        _iModifyValue;      /**值*/
    };

    /**
     * @brief 修改数据块头部
     * @brief Modify Data Block Header
     */
    struct tagModifyHead
    {
        /**Modification Status: 0: No one is modifying at present, 1: Start preparing to modify, 2: Finish modifying, no copy in memory*/
        char            _cModifyStatus;         /**修改状态: 0:目前没有人修改, 1: 开始准备修改, 2:修改完毕, 没有copy到内存中*/
        /**Update to current index, cannot operate 1000*/
        size_t          _iNowIndex;             /**更新到目前的索引, 不能操作1000个*/
        /**Up to 1000 modifications at a time*/
        tagModifyData   _stModifyData[1000];     /**一次最多1000次修改*/
    };

    /**
     * @brief HashItem
     */
    struct tagHashItem
    {
        /**Memory Address Index to Data Item*/
        uint32_t    _iBlockAddr;        /**指向数据项的内存地址索引*/
        /**Number of Chain Lists*/
        uint32_t    _iListCount;        /**链表个数*/
    };

    /**
    * @brief 主key HashItem
    * @brief Main key HashItem
    */
    struct tagMainKeyHashItem
    {
        /**The offset address of the primary key data item*/
        uint32_t    _iMainKeyAddr;        /**主key数据项的偏移地址*/
        /**Number of primary keys under the same primary key hash index*/
        uint32_t    _iListCount;        /**相同主key hash索引下主key个数*/
    };
#pragma pack() 

    /**64位操作系统用基数版本号, 32位操作系统用偶数版本号*/
    /**Base version number for 64-bit operating systems and even version number for 32-bit operating systems*/
#if __WORDSIZE == 64 || defined _WIN64

    /**
    * @brief 定义版本号
    * @brief Define Version Number
    */
    enum
    {
        /**Large version number of current map*/
        MAX_VERSION         = 1,        /**当前map的大版本号*/
        /**Small version number of current map*/
        MIN_VERSION         = 1,        /**当前map的小版本号*/
    };

#else
    /**
    * @brief 定义版本号
    * @brief Define Version Number
    */
    enum
    {
        /**Large version number of current map*/
        MAX_VERSION         = 1,        /**当前map的大版本号*/
        /**Small version number of current map*/
        MIN_VERSION         = 0,        /**当前map的小版本号*/
    };

#endif

    /**
    * @brief 定义淘汰方式
    * @brief Define the elimination method
    */
    enum
    {
        /**Eliminate by Get Chain List*/
        ERASEBYGET          = 0x00,        /**按照Get链表淘汰*/
        /**Eliminate by Set Chain List*/
        ERASEBYSET          = 0x01,        /**按照Set链表淘汰*/
    };

    /**
    * @brief 定义设置数据时的选项
    * @brief Define options when setting data
    */
    enum DATATYPE
    {
        /**Incomplete data*/
        PART_DATA            = 0,        /**不完整数据*/
        /**Complete data*/
        FULL_DATA            = 1,        /**完整数据*/
        /**Determine based on internal data integrity status*/
        AUTO_DATA            = 2,        /**根据内部的数据完整性状态来决定*/
    };

    /**
     * @brief  get, set等int返回值
     * @brief  Get, set, etc. int return value
     */
    enum
    {
        /**Success*/
        RT_OK                   = 0,    /**成功*/
        /**Dirty data*/
        RT_DIRTY_DATA           = 1,    /**脏数据*/
        /**No data*/
        RT_NO_DATA              = 2,    /**没有数据*/
        /**Require Writeback*/
        RT_NEED_SYNC            = 3,    /**需要回写*/
        /**No writeback is required*/
        RT_NONEED_SYNC          = 4,    /**不需要回写*/
        /**Data phase-out success*/
        RT_ERASE_OK             = 5,    /**淘汰数据成功*/
        /**Map read-only*/
        RT_READONLY             = 6,    /**map只读*/
        /**insufficient memory*/
        RT_NO_MEMORY            = 7,    /**内存不够*/
        /**Key only, no Value*/
        RT_ONLY_KEY             = 8,    /**只有Key, 没有Value*/
        /**Backup required*/
        RT_NEED_BACKUP          = 9,    /**需要备份*/
        /**No GET*/
        RT_NO_GET               = 10,   /**没有GET过*/
        /**Write data version mismatch*/
        RT_DATA_VER_MISMATCH    = 11,    /**写入数据版本不匹配*/
        /**Incomplete master key data*/
        RT_PART_DATA            = 12,    /**主key数据不完整*/
        /**Parsing error*/
        RT_DECODE_ERR           = -1,   /**解析错误*/
        /**exception*/
        RT_EXCEPTION_ERR        = -2,   /**异常*/
        /**Load Data Exception*/
        RT_LOAD_DATA_ERR        = -3,   /**加载数据异常*/
        /**Version inconsistency*/
        RT_VERSION_MISMATCH_ERR = -4,   /**版本不一致*/
        /**Dump to file failed*/
        RT_DUMP_FILE_ERR        = -5,   /**dump到文件失败*/
        /**Load file to memory failed*/
        RT_LOAD_FILE_ERR        = -6,   /**load文件到内存失败*/
        /**Not fully replicated*/
        RT_NOTALL_ERR           = -7,   /**没有复制完全*/
    };

    /**定义迭代器*/
    /**Define Iterator*/
    typedef HashMapIterator     hash_iterator;
    typedef HashMapLockIterator lock_iterator;

    /**定义hash处理器*/
    /**Define hash processor*/
    using hash_functor = std::function<uint32_t (const string& )>;

    //////////////////////////////////////////////////////////////////////////////////////////////
    //map的接口定义
    //Map's interface definition

    /**
     * @brief 构造函数
     * @brief Constructor
     */
    TC_Multi_HashMap()
    : _iMinDataSize(0)
    , _iMaxDataSize(0)
    , _fFactor(1.0)
    , _fHashRatio(2.0)
    , _fMainKeyRatio(1.0)
    , _pDataAllocator(new BlockAllocator(this))
    , _lock_end(this, 0, 0, 0)
    , _end(this, (uint32_t)(-1))
    , _hashf(magic_string_hash())
    {
    }

    /**
     * @brief 初始化数据块平均大小
     * @brief Initialization data block average size
     * 表示内存分配的时候，会分配n个最小块， n个（最小快*增长因子）, n个（最小快*增长因子*增长因子）..., 直到n个最大块
     * When memory allocation is indicated, n smallest blocks, n (minimum fast * growth factor), n (minimum fast * growth factor * growth factor)... Are allocated until n largest blocks
     * n是hashmap自己计算出来的
     * N is calculated by HashMap himself
     * 这种分配策略通常是数据块记录变长比较多的使用， 便于节约内存，如果数据记录基本不是变长的， 那最小块=最大快，增长因子=1就可以了
     * This allocation strategy is usually used when the data block records are longer, which saves memory. If the data block records are not longer at all, then the smallest block = the fastest, and the growth factor = 1 are fine.
     * @param iMinDataSize: 最小数据块大小
     * @param iMinDataSize: Minimum data block size
     * @param iMaxDataSize: 最大数据块大小
     * @param iMaxDataSize: Maximum data block size
     * @param fFactor: 增长因子
     * @param fFactor: Growth factor
     */
    void initDataBlockSize(size_t iMinDataSize, size_t iMaxDataSize, float fFactor);

    /**
     * @brief 始化chunk数据块/hash项比值, 默认是2,
     * 有需要更改必须在create之前调用
     * @brief Initialize chunk data block/hash item ratio, default is 2, 
     * changes must be called before create
     *
     * @param fRatio
     */
    void initHashRatio(float fRatio)                { _fHashRatio = fRatio;}

    /**
     * @brief 初始化chunk个数/主key hash个数, 默认是1, 
     * 含义是一个主key下面大概有多个条数据 有需要更改必须在create之前调用 
     * @brief Number of chunks initialized / number of primary key hashes, default is 1, 
     * meaning that there are probably several pieces of data under a primary key that need to be changed and must be called before creation
     * @param fRatio
     */
    void initMainKeyHashRatio(float fRatio)         { _fMainKeyRatio = fRatio;}

    /**
     * @brief 初始化, 之前需要调用:initDataAvgSize和initHashRatio
     * @brief Initialization required before: initDataAvgSize and initHashRatio
     * @param pAddr 外部分配好的存储的绝对地址
     * @param pAddr Absolute address of externally allocated storage
     * @param iSize 存储空间大小
     * @param iSize Storage size
     * @return 失败则抛出异常
     * @return Failure throws an exception
     */
    void create(void *pAddr, size_t iSize);

    /**
     * @brief 链接到已经格式化的内存块
     * @brief Link to a formatted memory block
     * @param pAddr, 内存地址
     * @param pAddr, memory address
     * @param iSize, 内存大小
     * @param iSize, memory size
     * @return 失败则抛出异常
     * @return Failure throws an exception
     */
    void connect(void *pAddr, size_t iSize);

    /**
     * @brief 原来的数据块基础上扩展内存, 注意通常只能对mmap文件生效
     * (如果iSize比本来的内存就小,则返回-1)
     * @brief Expand memory based on the original data block, note that it is usually only valid for MMAP files
     * (Returns -1 if iSize is smaller than the original memory)
     * @param pAddr, 扩展后的空间
     * @param pAddr, Expanded space
     * @param iSize
     * @return 0:成功, -1:失败
     * @return 0:Success, -1:Failure
     */
    int append(void *pAddr, size_t iSize);

    /**
     * @brief 获取每种大小内存块的头部信息
     * @brief Get header information for each memory block size
     *
     * @return vector<TC_MemChunk::tagChunkHead>: 不同大小内存块头部信息
     * @return vector<TC_MemChunk::tagChunkHead>: Header information for memory blocks of different sizes
     */
    vector<TC_MemChunk::tagChunkHead> getBlockDetail() { return _pDataAllocator->getBlockDetail(); }

    /**
     * @brief 所有block中chunk的个数
     * @brief Number of chunks in all blocks
     *
     * @return size_t
     */
    size_t allBlockChunkCount()                     { return _pDataAllocator->allBlockChunkCount(); }

    /**
     * @brief 每种block中chunk的个数(不同大小内存块的个数相同)
     * @brief Number of chunks in each block (same number of memory blocks of different sizes)
     *
     * @return vector<size_t>
     */
    vector<size_t> singleBlockChunkCount()          { return _pDataAllocator->singleBlockChunkCount(); }

    /**
     * @brief 获取数据区hash桶的个数
     * @brief Get the number of hash buckets in the data area
     *
     * @return size_t
     */
    size_t getHashCount()                           { return _hash.size(); }

    /**
    * @brief 获取主key hash桶个数
    * @brief Get the number of main key hash buckets
    */
    size_t getMainKeyHashCount()                    { return _hashMainKey.size(); }

    /**
     * @brief 获取元素的个数
     * @brief Get the number of elements
     *
     * @return size_t
     */
    size_t size()                                   { return _pHead->_iElementCount; }

    /**
     * @brief 脏数据元素个数
     * @brief Number of dirty data elements
     *
     * @return size_t
     */
    size_t dirtyCount()                             { return _pHead->_iDirtyCount;}

    /**
     * @brief 主键OnlyKey数据元素个数
     * @brief Number of primary key OnlyKey data elements
     *
     * @return size_t
     */
    size_t onlyKeyCount()                           { return _pHead->_iOnlyKeyCount;}

    /**
     * @brief 主key OnlyKey数据元素个数
     * @brief Number of primary key OnlyKey data elements
     *
     * @return size_t
     */
    size_t onlyKeyCountM()                          { return _pHead->_iMKOnlyKeyCount;}

    /**
     * @brief 设置每次淘汰数量
     * @brief Set Quantity per Elimination
     * @param n
     */
    void setEraseCount(size_t n)                    { _pHead->_iEraseCount = n; }

    /**
     * @brief 获取每次淘汰数量
     * @brief Get Quantity Eliminated at a Time
     *
     * @return size_t
     */
    size_t getEraseCount()                          { return _pHead->_iEraseCount; }

    /**
     * @brief 设置只读
     * @brief Set Read Only
     * @param bReadOnly
     */
    void setReadOnly(bool bReadOnly)                { _pHead->_bReadOnly = bReadOnly; }

    /**
     * @brief 是否只读
     * @brief readOnly
     *
     * @return bool
     */
    bool isReadOnly()                               { return _pHead->_bReadOnly; }

    /**
     * @brief 设置是否可以自动淘汰
     * @brief Set whether auto-elimination is possible
     * @param bAutoErase
     */
    void setAutoErase(bool bAutoErase)              { _pHead->_bAutoErase = bAutoErase; }

    /**
     * @brief 是否可以自动淘汰
     * @brief Is it possible to phase out automatically
     *
     * @return bool
     */
    bool isAutoErase()                              { return _pHead->_bAutoErase; }

    /**
     * @brief 设置淘汰方式
     * @brief Set up elimination method
     * TC_Multi_HashMap::ERASEBYGET
     * TC_Multi_HashMap::ERASEBYSET
     * @param cEraseMode
     */
    void setEraseMode(char cEraseMode)              { _pHead->_cEraseMode = cEraseMode; }

    /**
     * @brief 获取淘汰方式
     * @brief Get Elimination Methods
     *
     * @return bool
     */
    char getEraseMode()                             { return _pHead->_cEraseMode; }

    /**
     * @brief 设置回写时间间隔(秒)
     * @brief Set the writeback interval (seconds)
     * @param iSyncTime
     */
    void setSyncTime(time_t iSyncTime)              { _pHead->_iSyncTime = iSyncTime; }

    /**
     * @brief 获取回写时间
     * @brief Get Writeback Time
     *
     * @return time_t
     */
    time_t getSyncTime()                            { return _pHead->_iSyncTime; }

    /**
     * @brief 获取头部数据信息
     * @brief Get header data information
     * 
     * @return tagMapHead&
     */
    tagMapHead& getMapHead()                        { return *_pHead; }

    /**
     * @brief 设置联合主键hash方式
     * @brief Set the hash mode for the union primary key
     * @param hashf
     */
    void setHashFunctor(hash_functor hashf)         { _hashf = hashf; }

    /**
    * @brief 设置主key的hash方式
    * @brief Set hash mode for primary key
    * @param hashf
    */
    void setHashFunctorM(hash_functor hashf)            { _mhashf = hashf; } 

    /**
     * @brief 返回hash处理器
     * @brief Return hash processor
     * 
     * @return hash_functor&
     */
    hash_functor &getHashFunctor()                  { return _hashf; }
    hash_functor &getHashFunctorM()                    { return _mhashf; }

    /**
     * @brief 获取指定索引的hash item
     * @brief Gets the hash item of the specified index
     * @param index, hash索引
     *
     * @return tagHashItem&
     */
    tagHashItem *item(size_t iIndex)                { return &_hash[iIndex]; }

    /**
    * @brief 根据主key hash索引取主key item
    * @brief Primary key item based on primary key hash index
    * @param iIndex, 主key的hash索引
    * @param iIndex, Hash index of primary key
    */
    tagMainKeyHashItem* itemMainKey(size_t iIndex)    { return &_hashMainKey[iIndex]; }

    /**
     * @brief dump到文件
     * @brief Dump to file
     * @param sFile
     *
     * @return int
     *          RT_DUMP_FILE_ERR: dump到文件出错
     *          RT_DUMP_FILE_ERR: Dump to file error
     *          RT_OK: dump到文件成功
     *          RT_OK: Dump to file succeeded
     */
    int dump2file(const string &sFile);

    /**
     * @brief 从文件load
     * @brief Load from file
     * @param sFile
     *
     * @return int
     *          RT_LOAL_FILE_ERR: load出错
     *          RT_LOAL_FILE_ERR: Load Error
     *          RT_VERSION_MISMATCH_ERR: 版本不一致
     *          RT_VERSION_MISMATCH_ERR: Version inconsistency
     *          RT_OK: load成功
     *          RT_OK: load successfully
     */
    int load5file(const string &sFile);

    /**
     * @brief 清空hashmap
     * @brief Empty HashMap
     * 所有map的数据恢复到初始状态
     * Restore all map data to its initial state
     */
    void clear();

    /**
    * @brief 检查主key是否存在
    * @brief Check that the primary key exists
    * @param mk, 主key
    * @param mk, main key
    *
    * @return int
    *        TC_Multi_HashMap::RT_OK, 主key存在，且有数据
    *        TC_Multi_HashMap::RT_OK, Primary key exists and has data
    *        TC_Multi_HashMap::RT_ONLY_KEY, 主key存在，没有数据
    *        TC_Multi_HashMap::RT_ONLY_KEY, Primary key exists, no data
    *        TC_Multi_HashMap::RT_PART_DATA, 主key存在，里面的数据可能不完整
    *        TC_Multi_HashMap::RT_PART_DATA, Primary key exists, the data inside may be incomplete
    *        TC_Multi_HashMap::RT_NO_DATA, 主key不存在
    *        TC_Multi_HashMap::RT_NO_DATA,Primary key does not exist
    */
    int checkMainKey(const string &mk);

    /**
    * @brief 设置主key下数据的完整性
    * @brief Set data integrity under primary key
    * @param mk, 主key
    * @param mk, main key
    * @param bFull, true为完整数据，false为不完整数据
    * @param bFull, True is complete data, False is incomplete data
    *
    * @return
    *          RT_READONLY: 只读
    *          RT_READONLY: Read Only
    *          RT_NO_DATA: 没有当前数据
    *          RT_NO_DATA: No current data
    *          RT_OK: 设置成功
    *          RT_OK: Setup Successful
    *          其他返回值: 错误
    *          Other Return Values: Error
    */
    int setFullData(const string &mk, bool bFull);

    /**
     * @brief 检查数据干净状态
     * @brief Check data cleanliness
     * @param mk, 主key
     * @param mk, main key
     * @param uk, 除主key外的联合主键
     * @param uk, Joint primary key except primary key
     *
     * @return int
     *          RT_NO_DATA: 没有当前数据
     *          RT_NO_DATA: No current data
     *          RT_ONLY_KEY:只有Key
     *          RT_ONLY_KEY:Key Only
     *          RT_DIRTY_DATA: 是脏数据
     *          RT_DIRTY_DATA: Is dirty data
     *          RT_OK: 是干净数据
     *          RT_OK: Is clean data
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int checkDirty(const string &mk, const string &uk);

    /**
    * @brief 
    *        检查主key下数据的干净状态，只要主key下面有一条脏数据，则返回脏
    * @brief Check the clean state of the data under the main key and return dirty as long as there is a dirty data under the main key
    * @param mk, 主key
    * @param mk, main key
    * @return int
    *          RT_NO_DATA: 没有当前数据
    *          RT_NO_DATA: No current data
    *          RT_ONLY_KEY:只有Key
    *          RT_ONLY_KEY:Key only
    *          RT_DIRTY_DATA: 是脏数据
    *          RT_DIRTY_DATA: is dirty data
    *          RT_OK: 是干净数据
    *          RT_OK: is clean data
    *          其他返回值: 错误
    *          Other Return Values: Error
    */
    int checkDirty(const string &mk);

    /**
     * @brief 设置为脏数据, 修改SET时间链, 会导致数据回写
     * @brief Set to dirty data, modify SET time chain, cause data writeback
     * @param mk, 主key
     * @param mk, main key
     * @param uk, 除主key外的联合主键
     * @param uk, Union primary key except primary key
     *
     * @return int
     *          RT_READONLY: 只读
     *          RT_READONLY: read-only
     *          RT_NO_DATA: 没有当前数据
     *          RT_NO_DATA: no current data
     *          RT_ONLY_KEY:只有Key
     *          RT_ONLY_KEY: key only
     *          RT_OK: 设置脏数据成功
     *          RT_OK: successfully set dirty data
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int setDirty(const string &mk, const string &uk);

    /**
     * @brief 设置为干净数据, 修改SET链, 导致数据不回写
     * @brief Set to clean data, modify SET chain, result in data not being writeback
     * @param mk, 主key
     * @param mk, main key
     * @param uk, 除主key外的联合主键
     * @param uk, Joint primary key except primary key
     *
     * @return int
     *          RT_READONLY: 只读
     *          RT_READONLY: read-only
     *          RT_NO_DATA: 没有当前数据
     *          RT_NO_DATA: no current data
     *          RT_ONLY_KEY:只有Key
     *          RT_ONLY_KEY: key only
     *          RT_OK: 设置成功
     *          RT_OK: set successfully
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int setClean(const string &mk, const string &uk);

    /**
    * @brief 更新数据的回写时间
    * @brief Writeback time for updating data
    * @param mk,
    * @param uk,
    * @param iSynctime
    *
    * @return int
    *          RT_READONLY: 只读
    *          RT_READONLY: read-only
    *          RT_NO_DATA: 没有当前数据
    *          RT_NO_DATA: no current data
    *          RT_ONLY_KEY:只有Key
    *          RT_ONLY_KEY:key only
    *          RT_OK: 设置脏数据成功
    *          RT_OK: successfully set dirty data
    *          其他返回值: 错误
    *          Other Return Values: Error
    */
    int setSyncTime(const string &mk, const string &uk, time_t iSyncTime);

    /**
     * @brief 获取数据, 修改GET时间链
     * @brief Get data, modify GET time chain
     * @param mk, 主key
     * @param mk, main key
     * @param uk, 除主key外的联合主键
     * @param uk, Joint primary key except primary key
     @ @param v, 返回的数据
     * @param v, returned data
     *
     * @return int:
     *          RT_NO_DATA: 没有数据
     *          RT_NO_DATA: no data
     *          RT_ONLY_KEY:只有Key
     *          RT_ONLY_KEY:key only
     *          RT_OK:获取数据成功
     *          RT_OK:Successful data acquisition
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int get(const string &mk, const string &uk, Value &v);

    /**
     * @brief 获取主key下的所有数据, 修改GET时间链
     * @brief Get all the data under the primary key, modify the GET time chain
     * @param mk, 主key
     * @param mk, main key
     * @param vs, 返回的数据集
     * @param vs, Returned datasets
     *
     * @return int:
     *          RT_NO_DATA: 没有数据
     *          RT_NO_DATA: no data
     *          RT_ONLY_KEY: 只有Key
     *          RT_ONLY_KEY: key only
     *          RT_PART_DATA: 数据不全，只有部分数据
     *          RT_PART_DATA: Data is incomplete, only part of it
     *          RT_OK: 获取数据成功
     *          RT_OK: Successful data acquisition
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int get(const string &mk, vector<Value> &vs);

    /**
     * @brief 获取主key hash下的所有数据 
     *        , 不修改GET时间链，主要用于迁移
     * @brief Get all the data under the primary key hash without modifying the GET time chain, mainly for migration
     * @param mh, 主key hash值
     * @param mh, main key hash value
     * @param vs, 返回的数据集，以主key进行分组的map
     * @param vs, Returned dataset, map grouped by primary key
     *
     * @return int:
     *          RT_OK: 获取数据成功
     *          RT_OK: Successful data acquisition
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int get(uint32_t &mh, map<string, vector<Value> > &vs);

    /**
     * @brief 设置数据, 修改时间链, 内存不够时会自动淘汰老的数据
     * @brief Set up data, modify time chains, and automatically eliminate old data when memory is low
     * @param mk: 主key
     * @param mk: main key
     * @param uk: 除主key外的联合主键
     * @param uk: joint primary key except primary key
     * @param v: 数据值
     * @param v: data value 
     * @param iVersion: 数据版本, 应该根据get出的数据版本写回，为0表示不关心数据版本
     * @param iVersion: Data version, which should be written back based on the data version from get, is 0 to indicate that you don't care about the data version
     * @param bDirty: 是否是脏数据
     * @param bDirty: Is it dirty data
     * @param eType: set的数据类型，PART_DATA-不完事的数据，FULL_DATA-完整的数据，AUTO_DATA-根据已有数据类型决定
     * @param eType: Set's data type, PART_DATA-Incomplete Data, FULL_DATA-Complete Data, AUTO_DATA - Based on existing data types
     * @param bHead: 插入到主key链的顺序，前序或后序
     * @param bHead: The order, prefix or postorder of insertions into the main key chain
     * @param vtData: 被淘汰的记录
     * @param vtData: Obsolete records
     * @return int:
     *          RT_READONLY: map只读
     *          RT_READONLY: map read-only
     *          RT_NO_MEMORY: 没有空间(不淘汰数据情况下会出现)
     *          RT_NO_MEMORY: no space(occurs without data phasing out)
     *            RT_DATA_VER_MISMATCH, 要设置的数据版本与当前版本不符，应该重新get后再set
     *          RT_DATA_VER_MISMATCH, The data version you want to set does not match the current version. You should get it again before you set it
     *          RT_OK: 设置成功
     *          RT_OK: Setup succeeded
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int set(const string &mk, const string &uk, const string &v, uint8_t iVersion, 
        bool bDirty, DATATYPE eType, bool bHead,vector<Value> &vtData);

    /**
     * @brief 设置key, 但无数据(only key), 
     *        内存不够时会自动淘汰老的数据
     * @brief Set key, but no data (only key), and automatically eliminate old data when memory is low
     * @param mk: 主key
     * @param mk: main key
     * @param uk: 除主key外的联合主键
     * @param uk: Joint primary key except primary key
     * @param eType: set的数据类型，PART_DATA-不完事的数据，FULL_DATA-完整的数据，AUTO_DATA-根据已有数据类型决定
     * @param eType: data type of set, PART_DATA-Incomplete Data, FULL_DATA-Complete Data, AUTO_DATA - Based on existing data types
     * @param bHead: 插入到主key链的顺序，前序或后序
     * @param bHead: The order, prefix or postorder of insertions into the main key chain
     * @param vtData: 被淘汰的数据
     * @param vtData: Eliminated data
     *
     * @return int
     *          RT_READONLY: map只读
     *          RT_READONLY: map read-only
     *          RT_NO_MEMORY: 没有空间(不淘汰数据情况下会出现)
     *          RT_NO_MEMORY: No space (occurs without data phasing out)
     *          RT_OK: 设置成功
     *          RT_OK: Set Up Successfully
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int set(const string &mk, const string &uk, DATATYPE eType, bool bHead, vector<Value> &vtData);

    /**
     * @brief 仅设置主key, 无联合key及数据 
     *        , 内存不够时会自动淘汰老的数据
     * @brief Set primary key only, no union key and no data, old data will be phased out automatically when memory is insufficient
     * @param mk: 主key
     * @param mk: main key
     * @param vtData: 被淘汰的数据
     * @param vtData: Eliminated data
     *
     * @return int
     *          RT_READONLY: map只读
     *          RT_READONLY: map read-only
     *          RT_NO_MEMORY: 没有空间(不淘汰数据情况下会出现)
     *          RT_NO_MEMORY: No space (occurs without data phasing out)
     *          RT_OK: 设置成功
     *          RT_OK: set up successfully
     *          其他返回值: 错误
     *          Other Return Values: Error
     */
    int set(const string &mk, vector<Value> &vtData);

    /**
    * @brief 批量设置数据, 内存不够时会自动淘汰老的数据
    * @brief Set up data in batches and automatically eliminate old data when memory is low
    * @param vtSet: 需要设置的数据
    * @param vtSet: Data to be set
    * @param eType: set的数据类型，PART_DATA-不完事的数据，FULL_DATA-完整的数据，AUTO_DATA-根据已有数据类型决定
    * @param eType: Set's data type, PART_DATA-Incomplete Data, FULL_DATA-Complete Data, AUTO_DATA - Based on existing data types
    * @param bHead: 插入到主key链的顺序，前序或后序
    * @param bHead: The order, prefix or postorder of insertions into the main key chain
    * @param bForce, 是否强制插入数据，为false则表示如果数据已经存在则不更新
    * @param bForce, Whether to force insertion of data, false means not updating if data already exists
    * @param vtErased: 内存不足时被淘汰的数据
    * @param vtErased: Data eliminated when out of memory
    *
    * @return 
    *          RT_READONLY: map只读
    *          RT_READONLY: map read-only
    *          RT_NO_MEMORY: 没有空间(不淘汰数据情况下会出现)
    *          RT_NO_MEMORY: No space (occurs without data phasing out)
    *          RT_OK: 设置成功
    *          RT_OK： set up successfully
    *          其他返回值: 错误
    *          Other Return Values: Error
    */
    int set(const vector<Value> &vtSet, DATATYPE eType, bool bHead, bool bForce, vector<Value> &vtErased);

    /**
     * @brief 
     *        删除数据，除非强制删除某条数据，否则应该调用下面的删除主key下所有数据的函数
     * @brief 
     *        Delete data, unless a data is forcibly deleted, the following function should be called to delete all data under the main key
     * @param mk: 主key
     * @param mk: main key
     * @param uk: 除主key外的联合主键
     * @param uk: Joint primary key except primary key
     * @param data: 被删除的记录
     * @param data: Deleted Records
     * @return int:
     *          RT_READONLY: map只读
     *          RT_READONLY: map read-only
     *          RT_NO_DATA: 没有当前数据
     *          RT_NO_DATA: no current data
     *          RT_ONLY_KEY:只有Key, 删除成功
     *          RT_ONLY_KEY: key only, deleted successfully
     *          RT_OK: 删除数据成功
     *          RT_OK: Successfully deleted data
     *         其他返回值: 错误
     *          Other Return Values: Error
     */
    int del(const string &mk, const string &uk, Value &data);

    /**
     * @brief 删除主key下所有数据
     * @brief Delete all data under the primary key
     * @param mk: 主key
     * @param mk: main key
     * @param data: 被删除的记录
     * @param data: Deleted Records
     * @return int:
     *          RT_READONLY: map只读
     *          RT_READONLY: map read-only
     *          RT_NO_DATA: 没有当前数据
     *          RT_NO_DATA: No current data
     *          RT_ONLY_KEY:只有Key, 删除成功
     *          RT_ONLY_KEY:Key only, delete successful
     *          RT_OK: 删除数据成功
     *          RT_OK: Successfully deleted data
     *         其他返回值: 错误
     *          Other Return Values: Error
     */
    int del(const string &mk, vector<Value> &data);

    /**
     * @brief 淘汰数据, 每次删除一条, 根据Get时间淘汰
     * @brief Eliminate data, one at a time, according to Get time
     * 外部循环调用该接口淘汰数据
     * External loop calls this interface to phase out data
     * 直到: 元素个数/chunks * 100 < Ratio, bCheckDirty 为true时，遇到脏数据则淘汰结束
     * Until: Number of elements / chunks * 100 < Ratio, bCheckDirty is true, the phase-out ends when dirty data is encountered
     * @param ratio: 共享内存chunks使用比例 0< Ratio < 100
     * @param ratio: Shared memory chunks usage ratio 0 < Ratio < 100
     * @param data: 被删除的数据集
     * @param data: Deleted datasets
     * @param bCheckDirty: 是否检查脏数据
     * @param bCheckDirty: Check for dirty data
     * @return int:
     *          RT_READONLY: map只读
     *          RT_READONLY: map read-only
     *          RT_OK: 不用再继续淘汰了
     *          RT_OK: No need to continue phasing out
     *          RT_ONLY_KEY:只有Key, 删除成功
     *          RT_ONLY_KEY:Key only, deleted successfully
     *          RT_DIRTY_DATA:数据是脏数据，当bCheckDirty=true时会有可能产生这种返回值
     *          RT_DIRTY_DATA:The data is dirty and this return value is possible when bCheckDirty=true
     *          RT_ERASE_OK:淘汰当前数据成功, 继续淘汰
     *          RT_ERASE_OK:Successful elimination of current data and continued elimination
     *          其他返回值: 错误, 通常忽略, 继续调用erase淘汰
     *          Other return values: Error, usually ignored, continue calling erase obsolete
     */
    int erase(int ratio, vector<Value> &vtData, bool bCheckDirty = false);

    /**
     * @brief 回写, 每次返回需要回写的一条
     * @brief Write back, one at a time that needs to be written back
     * 数据回写时间与当前时间超过_pHead->_iSyncTime则需要回写
     * Data Writeback Time and Current Time Over _pHead->_iSyncTime needs writeback
     * _pHead->_iSyncTime由setSyncTime函数设定, 默认10分钟
     * _pHead->_iSyncTime is set by setSyncTime function, default 10 minutes

     * 外部循环调用该函数进行回写
     * Outer loop calls this function to rewrite
     * map只读时仍然可以回写
     * Map can still be rewritten when read-only
     * @param iNowTime: 当前时间
     *                  回写时间与当前时间相差_pHead->_iSyncTime都需要回写
     * @param iNowTime: current time
     *                  Writeback time differs from current time_pHead->_iSyncTime needs writeback
     * @param data : 返回需要回写的数据
     * @param data : Return data that needs to be written back
     * @return int:
     *          RT_OK: 到脏数据链表头部了, 可以sleep一下再尝试
     *          RT_OK: To the dirty data chain header, sleep and try again
     *          RT_ONLY_KEY:只有Key, 删除成功, 当前数据不要缓写,继续调用sync回写
     *          RT_ONLY_KEY:Only Key, delete succeeded, do not overwrite current data, continue calling sync writeback
     *          RT_NEED_SYNC:当前返回的data数据需要回写
     *          RT_NEED_SYNC:The data data currently returned needs to be written back
     *          RT_NONEED_SYNC:当前返回的data数据不需要回写
     *          RT_NONEED_SYNC:Currently returned data does not require writeback
     *          其他返回值: 错误, 通常忽略, 继续调用sync回写
     *          Other return values: Error, usually ignored, continue to call sync writeback
     */
    int sync(time_t iNowTime, Value &data);

    /**
     * @brief 开始回写, 调整回写指针
     * @brief Start Writeback, Adjust Writeback Pointer
     */
    void sync();

    /**
     * @brief 开始备份之前调用该函数
     * @brief Call this function before starting backup
     *
     * @param bForceFromBegin: 是否强制从头开始备份
     * @param bForceFromBegin: Is backup forced from scratch
     * @return void
     */
    void backup(bool bForceFromBegin = false);

    /**
     * @brief 开始备份数据 
     *        , 每次返回需要备份的数据(一个主key下的所有数据)
     * @brief Start backing up the data and return each time the data you need to back up (all data under a primary key)
     * @param data
     *
     * @return int
     *          RT_OK: 备份完毕
     *          RT_OK: Backup complete
     *          RT_NEED_BACKUP:当前返回的data数据需要备份
     *          RT_NEED_BACKUP:The data data currently returned needs to be backed up
     *          RT_ONLY_KEY:只有Key, 当前数据不要备份
     *          RT_ONLY_KEY:Key only, do not backup current data
     *          其他返回值: 错误, 通常忽略, 继续调用backup
     *          Other return values: Error, usually ignored, continue to call backup
     */
    int backup(vector<Value> &vtData);

    /////////////////////////////////////////////////////////////////////////////////////////
    // 以下是遍历map函数, 需要对map加锁
    // The following is a traversal map function that requires a lock on the map

    /**
     * @brief 结束
     * @brief End
     *
     * @return
     */
    lock_iterator end() { return _lock_end; }

    /**
     * @brief block正序
     * @brief Block Positive Order
     *
     * @return lock_iterator
     */
    lock_iterator begin();

    /**
     *@brief  block逆序
     *@brief  Block reverse order
     *
     * @return lock_iterator
     */
    lock_iterator rbegin();

    /**
     * @brief 以Set时间排序的迭代器
     * @brief Iterators sorted by Set time
     *
     * @return lock_iterator
     */
    lock_iterator beginSetTime();

    /**
     * @brief Set链逆序的迭代器
     * @brief Set Chain Inverse Iterator
     *
     * @return lock_iterator
     */
    lock_iterator rbeginSetTime();

    /**
     * @brief 以Get时间排序的迭代器
     * @brief Iterator in Get Time
     *
     * @return lock_iterator
     */
    lock_iterator beginGetTime();

    /**
     * @brief Get链逆序的迭代器
     * @brief Iterator for Get Chain Reverse Order
     *
     * @return lock_iterator
     */
    lock_iterator rbeginGetTime();

    /**
     * @brief 获取脏链表尾部迭代器(最长时间没有操作的脏数据)
     * @brief Get dirty chain list tail iterator (dirty data that has not been operated on for the longest time)
     *
     * 返回的迭代器++表示按照时间顺序==>(最短时间没有操作的脏数据)
     * The returned iterator++ indicates dirty data in chronological order==> (minimum no operation)
     *
     * @return lock_iterator
     */
    lock_iterator beginDirty();

    /////////////////////////////////////////////////////////////////////////////////////////
    // 以下是遍历map函数, 不需要对map加锁
    // The following is a traversal map function that does not require a lock on the map

    /**
     * @brief 根据hash桶遍历
     * @brief Traverse according to hash bucket
     * 
     * @return hash_iterator
     */
    hash_iterator hashBegin();

    /**
     *@brief  结束
     *@brief  End
     *
     * @return
     */
    hash_iterator hashEnd() { return _end; }

    /**
     * @brief 根据Key查找数据
     * @brief Find data based on Key
     * @param mk: 主key
     * @param mk: main key
     * @param uk: 除主key外的联合主键
     * @param uk: Joint primary key except primary key
     * @return lock_iterator
     */
    lock_iterator find(const string &mk, const string &uk);

    /**
    * @brief 获取主key链上block的数量
    * @brief Get the number of blocks on the main key chain
    * @param mk: 主key
    * @param mk: main key
    * @return size_t
    */
    size_t count(const string &mk);

    /**
    * @brief 根据主key查找第一个block位置. 
    * @brief Find the first block location based on the primary key.
    *  
    * 与count配合可以遍历某个主key下的所有数据 也可以直接使用迭代器，直到end
    * Working with count allows you to traverse all data under a primary key or use iterators directly until the end
    * @param mk: 主key
    * @param mk: main key
    * @return lock_iterator
    */
    lock_iterator find(const string &mk);

    /**
     * @brief 描述
     * @brief describe
     *
     * @return string
     */
    string desc();

    /**
    * @brief 对可能的坏block进行检查，并可进行修复
    * @brief Check for possible bad blocks and repair them
    * @param iHash, hash索引
    * @param iHash, hash index
    * @param bRepaire, 是否进行修复
    * @param bRepaire, Whether to Repair
    *
    * @return size_t, 返回坏数据个数
    * @return size_t, Number of bad data returned
    */
    size_t checkBadBlock(uint32_t iHash, bool bRepair);

protected:

    friend class Block;
    friend class BlockAllocator;
    friend struct HashMapIterator;
    friend class HashMapItem;
    friend struct HashMapLockIterator;
    friend class HashMapLockItem;

    /**
    *  @brief 禁止copy构造
    *  @brief Prohibit copy construction
    */
    TC_Multi_HashMap(const TC_Multi_HashMap &mcm);
       /**
    *  @brief 禁止复制
    *  @brief Prohibit copying
    */
    TC_Multi_HashMap &operator=(const TC_Multi_HashMap &mcm);

    /**                      
    *  @brief 用于数据更新过程失败的自动恢复，
    *  在所有可能进行关键数据更新的函数的最开始构造
    *  @brief Automatic recovery for failed data update process, at the very beginning of all functions that may update key data.
    */
    struct FailureRecover
    {
        FailureRecover(TC_Multi_HashMap *pMap) : _pMap(pMap)
        {
            // 构造时恢复可能损坏的数据
            // Restore potentially corrupt data at construction time
            _pMap->doRecover();
            assert(_iRefCount ++ == 0);
        }
        
        ~FailureRecover()
        {
            // 析构时清理已经成功更新的数据
            // Clean up data that has been successfully updated at destruct time
            _pMap->doUpdate();
            assert(_iRefCount-- == 1);
        }
        
    protected:
        TC_Multi_HashMap   *_pMap;
        // 避免嵌套调用
        // Avoid nested calls
        static int            _iRefCount;
    };


    /**
     * @brief 初始化 
     * @brief Initialization
     * @param pAddr, 外部分配好的存储地址
     * @param pAddr, Externally assigned storage address
     */
    void init(void *pAddr);

    /**
     * @brief 增加脏数据个数
     * @brief Increase the number of dirty data
     */
    void incDirtyCount()    { saveValue(&_pHead->_iDirtyCount, _pHead->_iDirtyCount+1); }

    /**
     * @brief 减少脏数据个数
     * @brief Reduce the number of dirty data
     */
    void delDirtyCount()    { saveValue(&_pHead->_iDirtyCount, _pHead->_iDirtyCount-1); }

    /**
     * @brief 增加数据个数
     * @brief Increase the number of data
     */
    void incElementCount()  { saveValue(&_pHead->_iElementCount, _pHead->_iElementCount+1); }

    /**
     * @brief 减少数据个数
     * @brief Reduce the number of data
     */
    void delElementCount()  { saveValue(&_pHead->_iElementCount, _pHead->_iElementCount-1); }

    /**
     * @brief 增加主键下OnlyKey数据个数
     * @brief Increase the number of OnlyKey data under the primary key
     */
    void incOnlyKeyCount()    { saveValue(&_pHead->_iOnlyKeyCount, _pHead->_iOnlyKeyCount+1); }

    /**
     * @brief 减少主键下OnlyKey数据个数
     * @brief Reduce the number of OnlyKey data under the primary key
     */
    void delOnlyKeyCount()    { saveValue(&_pHead->_iOnlyKeyCount, _pHead->_iOnlyKeyCount-1); }

    /**
     * @brief 增加主key下OnlyKey数据个数
     * @brief Increase the number of OnlyKey data under the primary key
     */
    void incOnlyKeyCountM()    { saveValue(&_pHead->_iMKOnlyKeyCount, _pHead->_iMKOnlyKeyCount+1); }

    /**
     * @brief 减少主key下OnlyKey数据个数
     * @brief Reduce the number of OnlyKey data under the primary key
     */
    void delOnlyKeyCountM()    { saveValue(&_pHead->_iMKOnlyKeyCount, _pHead->_iMKOnlyKeyCount-1); }

    /**
     * @brief 增加Chunk数
     * @brief Increase the number of Chunks
     */
    void incChunkCount()    { saveValue(&_pHead->_iUsedChunk, _pHead->_iUsedChunk + 1); }

    /**
     * @brief 减少Chunk数
     * @brief Reduce the number of Chunks
     */
    void delChunkCount()    { saveValue(&_pHead->_iUsedChunk, _pHead->_iUsedChunk - 1); }

    /**
     * @brief 增加hit次数
     * @brief Increase hit count
     */
    void incGetCount()      { saveValue(&_pHead->_iGetCount, _pHead->_iGetCount+1); }

    /**
     * @brief 增加命中次数
     * @brief Increase Hits
     */
    void incHitCount()      { saveValue(&_pHead->_iHitCount, _pHead->_iHitCount+1); }

    /**
     * @brief 某hash链表数据个数+1
     * @brief Number of Hash Chain List Data+1
     * @param index
     */
    void incListCount(uint32_t index) { saveValue(&item(index)->_iListCount, item(index)->_iListCount+1); }

    /**
    * @brief 某hash值主key链上主key个数+1
    * @brief Number of primary keys in a hash value main key chain+1
    */
    void incMainKeyListCount(uint32_t index) { saveValue(&itemMainKey(index)->_iListCount, itemMainKey(index)->_iListCount+1); }

    /**
     * @brief 某hash链表数据个数-1
     * @brief Number of data in a Hash list-1
     * @param index
     */
    void delListCount(uint32_t index) { saveValue(&item(index)->_iListCount, item(index)->_iListCount-1); }

    /**
    * @brief 某hash值主key链上主key个数-1
    * @brief Number of primary keys in a hash value main key chain-1
    */
    void delMainKeyListCount(uint32_t index) { saveValue(&itemMainKey(index)->_iListCount, itemMainKey(index)->_iListCount-1); }

    /**
    * @brief 某hash值主key链上blockdata个数+/-1
    * @brief Number of blockdata on a hash value main key chain+/-1
    * @param mk, 主key
    * @param mk, main key
    * @param bInc, 是增加还是减少
    * @param bInc, Increase or decrease
    */
    void incMainKeyBlockCount(const string &mk, bool bInc = true);

    /**
    * @brief 更新主key下面最大记录数信息
    * @brief Update maximum number of records information under primary key
    */
    void updateMaxMainKeyBlockCount(size_t iCount);

    /**
     * @brief 相对地址换成绝对地址
     * @brief Replace relative address with absolute address
     * @param iAddr
     *
     * @return void*
     */
    void *getAbsolute(uint32_t iAddr) { return _pDataAllocator->_pChunkAllocator->getAbsolute(iAddr); }

    /**
     * @brief 绝对地址换成相对地址
     * @brief Replace absolute address with relative address
     *
     * @return size_t
     */
    uint32_t getRelative(void *pAddr) { return (uint32_t)_pDataAllocator->_pChunkAllocator->getRelative(pAddr); }

    /**
     * @brief 淘汰iNowAddr之外的数据(根据淘汰策略淘汰)
     * @brief Eliminate data other than iNowAddr (Eliminate by Elimination Strategy)
     * @param iNowAddr, 当前主key正在分配内存，是不能被淘汰的
     * @param iNowAddr, The main key is currently allocating memory and cannot be phased out
     *                  0表示做直接根据淘汰策略淘汰
     *                  0 means to phase out directly according to the elimination strategy
     * @param vector<Value>, 被淘汰的数据
     * @param vector<Value>, Eliminated data
     * @return size_t, 淘汰的数据个数
     * @return size_t, Number of data phased out
     */
    size_t eraseExcept(uint32_t iNowAddr, vector<Value> &vtData);

    /**
     * @brief 根据Key计算hash值
     * @brief Calculate hash values from Key
     * @param mk: 主key
     * @param mk: main key
     * @param uk: 除主key外的联合主键
     * @param uk: Joint primary key except primary key
     *
     * @return uint32_t
     */
    uint32_t hashIndex(const string &mk, const string &uk);

    /**
     * @brief 根据Key计算hash值
     * @brief Calculate hash values from Key
     * @param k: key
     *
     * @return uint32_t
     */
    uint32_t hashIndex(const string &k);

    /**
    * @brief 根据主key计算主key的hash
    * @brief Calculate hash of primary key based on primary key
    * @param mk: 主key
    * @param mk: main key
    * @return uint32_t
    */
    uint32_t mhashIndex(const string &mk); 

    /**
     * @brief 根据hash索引查找指定key(mk+uk)的数据的位置, 并返回数据
     * @brief Finds the location of the data for the specified key (mk+uk) based on the hash index and returns the data
     * @param mk: 主key
     * @param mk: main key
     * @param uk: 除主key外的联合主键
     * @param uk: Joint primary key except primary key
     * @param index: 联合主键的hash索引
     * @param index: Hash index of union primary key
     * @param v: 如果存在数据，则返回数据值
     * @param v: Return data value if data exists
     * @param ret: 具体的返回值
     * @param ret: Specific return value
     * @return lock_iterator: 返回找到的数据的位置，不存在则返回end()
     * @return lock_iterator: Return the location of the data found, or end() if it does not exist
     *
     */
    lock_iterator find(const string &mk, const string &uk, uint32_t index, Value &v, int &ret);

    /**
     * @brief 根据hash索引查找指定key(mk+uk)的数据的位置
     * @brief Location of the data for the specified key (mk+uk) based on the hash index
     * @param mk: 主key
     * @param mk: main key
     * @param uk: 除主key外的联合主键
     * @param uk: Joint primary key except primary key
     * @param index: 联合主键的hash索引
     * @param index: Hash index of union primary key
     * @param ret: 具体的返回值
     * @param ret: Specific return value
     * @return lock_iterator: 返回找到的数据的位置，不存在则返回end()
     * @return lock_iterator: Returns the location of the data found, or end() if it does not exist
     *
     */
    lock_iterator find(const string &mk, const string &uk, uint32_t index, int &ret);

    /**
    * @brief 根据主key hash索引查找主key的地址，找不到返回0
    * @brief Find the address of the primary key based on the primary key hash index, return 0 not found
    * @param mk: 主key
    * @param mk: main key
    * @param index: 主key hash索引
    * @param index: hash index of main key
    * @param ret: 具体返回值
    * @param ret: Specific return value
    * @return uint32_t: 返回找到的主key的首地址，找不到返回0
    * @return uint32_t: Return the first address of the primary key found, return 0 not found
    */
    uint32_t find(const string &mk, uint32_t index, int &ret);

    /**
     * @brief 分析主键hash的数据
     * @brief Analyzing data for primary key hash
     * @param iMaxHash: 最大的block hash桶上元素个数
     * @param iMaxHash: Number of elements on maximum block hash bucket
     * @param iMinHash: 最小的block hash桶上元素个数
     * @param iMinHash: Number of elements on minimum block hash bucket
     * @param fAvgHash: 平均元素个数
     * @param fAvgHash: Average number of elements
     */
    void analyseHash(uint32_t &iMaxHash, uint32_t &iMinHash, float &fAvgHash);

    /**
     * @brief 分析主key hash的数据
     * @brief Analyzing data for the main key hash
     * @param iMaxHash: 最大的主key hash桶上元素个数
     * @param iMaxHash: Number of elements on maximum block hash bucket
     * @param iMinHash: 最小的主key hash桶上元素个数
     * @param iMinHash: Number of elements on minimum block hash bucket
     * @param fAvgHash: 平均元素个数
     * @param fAvgHash: Average number of elements
     */
    void analyseHashM(uint32_t &iMaxHash, uint32_t &iMinHash, float &fAvgHash);

    /**
    * @brief 修改具体的值
    * @brief Modify specific values
    * @param pModifyAddr
    * @param iModifyValue
    */
    template<typename T>
    void saveValue(void* pModifyAddr, T iModifyValue)
    {
        //获取原始值
        //Get the original value
        T tmp = *(T*)pModifyAddr;
        
        //保存原始值
        //Save original value
        _pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._iModifyAddr  = (char*)pModifyAddr - (char*)_pHead;
        _pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._iModifyValue = tmp;
        _pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._cBytes       = sizeof(iModifyValue);
        _pstModifyHead->_iNowIndex++;
        
        _pstModifyHead->_cModifyStatus = 1;

        //修改具体值
        //Modify specific values
        *(T*)pModifyAddr = iModifyValue;
        
        assert(_pstModifyHead->_iNowIndex < sizeof(_pstModifyHead->_stModifyData) / sizeof(tagModifyData));
    }

    /**
    * @brief 对某个值的某位进行更新
    * @brief Update a bit of a value
    * @param pModifyAddr, 待修改的(整型数)内存地址
    * @param pModifyAddr, Memory address to be modified (integer)
    * @param bit, 需要修改的整型数的位
    * @param bit, Bits of integers that need to be modified
    * @param b, 需要修改的整型数位的值
    * @param b, Value of integer digits that need to be modified
    */
    template<typename T>
    void saveValue(T *pModifyAddr, uint8_t bit, bool b)
    {
        /**Take out the original value*/
        T tmp = *pModifyAddr;    // 取出原值
        if(b)
        {
            tmp |= 0x01 << bit;
        }
        else
        {
            tmp &= T(-1) ^ (0x01 << bit);
        }
        saveValue(pModifyAddr, tmp);
    }
    
    /**
    * @brief 恢复数据
    * @brief Restore data
    */
    void doRecover();
    
    /**
    * @brief 确认处理完毕
    * @brief Confirmation Processing Completed
    */
    void doUpdate();

    /**
     * @brief 获取大于n且离n最近的素数
     * @brief Gets the nearest prime number greater than n
     * @param n
     *
     * @return size_t
     */
    size_t getMinPrimeNumber(size_t n);

protected:

    /**
     * 头部指针
     * Head Pointer
     */
    tagMapHead                  *_pHead;

    /**
     * 最小的数据块大小
     * Minimum data block size
     */
    size_t                      _iMinDataSize;

    /**
     * 最大的数据块大小
     * Maximum data block size
     */
    size_t                      _iMaxDataSize;

    /**
     * 数据块增长因子
     * Data Block Growth Factor
     */
    float                       _fFactor;

    /**
     * 设置chunk数据块/hash项比值
     * Set chunk data block/hash item ratio
     */
    float                       _fHashRatio;

    /**
    * 主key hash个数/联合hash个数
    * Number of primary key hash/joint hash
    */
    float                        _fMainKeyRatio;

    /**
     * 联合主键hash索引区
     * Joint Primary Key hash Index Area
     */
    TC_MemVector<tagHashItem>   _hash;

    /**
    * 主key hash索引区
    * Primary key hash index area
    */
    TC_MemVector<tagMainKeyHashItem>    _hashMainKey;

    /**
     * 修改数据块
     * Modify Data Block
     */
    tagModifyHead               *_pstModifyHead;

    /**
     * block分配器对象，包括为数据区和主key区分配内存
     * Block allocator object, including memory allocation for data area and primary key area
     */
    BlockAllocator              *_pDataAllocator;

    /**
     * 尾部
     * Tail
     */
    lock_iterator               _lock_end;

    /**
     * 尾部
     * Tail
     */
    hash_iterator               _end;

    /**
     * 联合主键hash值计算公式
     * Joint Primary Key Hash Value Formula
     */
    hash_functor                _hashf;

    /**
    * 主key的hash计算函数, 如果不提供，将使用上面的_hashf
    * The hash calculation function of the primary key, if not provided, will use the _hashf above
    */
    hash_functor                _mhashf;
};

}

#endif

