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

#ifndef __TC_MEM_CHUNK_H__
#define __TC_MEM_CHUNK_H__

#include <string>
#include <vector>

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/**  
* @file tc_mem_chunk.h
* @brief 内存分配器 
* @brief Memory Allocator
*          
*/ 
/////////////////////////////////////////////////
/**
* @brief 原始内存块, 由TC_MemChunkAllocator来负责分配和维护
* @brief Raw memory block, by TC_MemChunkAllocator is responsible for allocation and maintenance
*
* 将连续的内存分成大小相同的块,形成链表,并能够分配和释放这些大小相同的快
* Divides contiguous memory into blocks of the same size, forms a chain table, and is able to allocate and release these blocks of the same size quickly
*/
class TC_MemChunk
{
public:

    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_MemChunk();

    /**
     * @brief 计算Chunk需要的内存块大小
     * @brief Calculate the memory block size Chunk needs
     * @param iBlockSize
     * @param iBlockCount
     *
     * @return size_t
     */
    static size_t calcMemSize(size_t iBlockSize, size_t iBlockCount);

    /**
     * @brief 计算block个数
     * @brief Count blocks
     * @param iMemSize
     * @param iBlockSize
     *
     * @return size_t
     */
    static size_t calcBlockCount(size_t iMemSize, size_t iBlockSize);

     /**
     * @brief tagChunkHead的大小
     * @brief size of the tagChunkHead
     *
     * @return size_t
     */
    static size_t getHeadSize() { return sizeof(tagChunkHead); }

    /**
     * @brief 初始化, 要保证p指向的内存指针=getMemSize大小
     * @brief Initialize to ensure that the memory pointer P points to = getMemSize size
     * @param pAddr        地址, 换到应用程序的绝对地址
     * @param pAddr        Address, to the absolute address of the application
     * @param iBlockSize   block大小
     * @param iBlockSize   size of the block
     * @param iBlockCount  block个数
     * @param iBlockCount  count of the block
     */
    void create(void *pAddr, size_t iBlockSize, size_t iBlockCount);

    /**
     * @brief 连接上
     * @brief Connect
     * @param pAddr 地址, 换到应用程序的绝对地址
     * @param pAddr Address, to the absolute address of the application
     */
    void connect(void *pAddr);

    /**
     * @brief 获取block的大小 
     * @brief Get the block size
     * @return block的大小
     * @return block size
     */
    size_t getBlockSize() const { return _pHead->_iBlockSize; }

    /**
     * @brief 获取所有的内存大小
     * @brief Get all memory size
     *
     * @return 所有的内存大小
     * @return all memory size
     */
    size_t getMemSize() const { return _pHead->_iBlockSize * _pHead->_iBlockCount + sizeof(tagChunkHead); }

    /**
     * @brief 获取可以存放数据的总容量
     * @brief Get the total capacity to store data
     *
     * @return 总容量
     * @return Total capacity
     */
    size_t getCapacity() const { return _pHead->_iBlockSize * _pHead->_iBlockCount; }

    /**
     * @brief 获取block的个数
     * @brief Number of Get Blocks
     *
     * @return block的个数
     * @return Number of blocks
     */
    size_t getBlockCount() const { return _pHead->_iBlockCount; }

    /**
     * @brief 是否还有可用block 
     * @brief Is there still a block available
     * @return 可用返回true，否则返回false
     * @return If it is available, return true, otherwise return false
     */
    bool isBlockAvailable() const { return _pHead->_blockAvailable > 0; }

    /**
     * @brief  获取可以利用的block的个数 
     * @brief  Get the number of blocks available
     * @return 可用的block的个数 
     * @return Number of blocks available
     */
    size_t getBlockAvailableCount() const { return _pHead->_blockAvailable; }

    /**
     * @brief 分配一个区块
     * @brief Allocate a block
     *
     * @return 指向分配的区块的指针
     * @return Pointer to allocated block
     */
    void* allocate();

    /**
    * @brief 分配一个区块. 
    * @brief Allocate a block.
    * 返回以1为基数的区块索引，没有可分配空间时返回 0 , 
    * Returns a block index based on 1, returning 0 if there is no allocatable space,
    * 通查索引都是比较小(即使在64位操作系统上), 4个字节以内 
    * All lookup indexes are small (even on 64-bit operating systems), within 4 bytes
    * 便于节省内存 
    * Easy to save memory
    */
    void* allocate2(size_t &iIndex);

    /**
     * @brief 释放区块
     * @brief Release Block
     * @param 指向要释放区块的指针
     * @param Pointer to block to release
     */
    void deallocate(void *pAddr);

    /**
     * @brief 根据索引释放区块
     * @brief Release blocks based on index
     * @param 区块索引
     * @param Block Index
     */
    void deallocate2(size_t iIndex);

    /**
     * @brief 重建
     * @brief Rebuild 
     */
    void rebuild();

    /**
     * @brief chunk头部
     * @brief Chunk header
     */
#pragma pack(1) 
    struct tagChunkHead
    {
        /**block size*/
        size_t  _iBlockSize;            /**区块大小*/
        /**Number of blocks*/
        size_t  _iBlockCount;           /**block个数*/
        /**First available block index*/
        size_t  _firstAvailableBlock;   /**第一个可用的block索引*/
        /**Number of blocks available*/
        size_t  _blockAvailable;        /**可用block个数*/
    };
#pragma pack() 

    /**
     * @brief 获取头部信息
     * @brief Get header information
     *
     * @return 头部信息
     * @return Header Information
     */
    tagChunkHead getChunkHead() const;

    /**
    * @brief 根据索引获取绝对地址
    * @brief Obtain absolute address from index
    */
    void* getAbsolute(size_t iIndex);

    /**
     * @brief 绝对地址换成索引
     * @brief Absolute Address to Index
     * 
     * @param pAddr    绝对地址 
     * @param pAddr    Absolute Address
     * @return size_t  索引值
     * @return size_t  Index Value
     */
    size_t getRelative(void *pAddr);

protected:
    /**
     * @brief 初始化
     * @brief Initialization
     */
    void init(void *pAddr);

private:

    /**
     * @brief 区块头指针
     * @brief Block Header Pointer
     */
    tagChunkHead    *_pHead;

    /**
     * @brief 数据区指针
     * @brief Data area pointer
     */
    unsigned char   *_pData;
};

/**
* @brief 内存块分配器，提供分配和释放的功能  
* @brief Memory block allocator, which provides allocation and release capabilities
*  
* 只能分配相同大小的内存块,最下层的原始内存块分配, 
* Only memory blocks of the same size can be allocated, the lowest original memory block allocation,
*  
* 内存结构: 内存块长度, 4个字节 ;
* Memory structure: memory block length, 4 bytes;
*  
* Block大 小, 4个字节; 
* Block size, 4 bytes;
*  
* Chunk个数, 4个字节 ;
* Number of Chunks, 4 bytes;
*  
* TC_MemChunk 暂时只支持同一个Block大小的MemChunk 
* TC_MemChunk temporarily only supports MemChunk of the same block size
*/
class TC_MemChunkAllocator
{
public:

    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_MemChunkAllocator();

    /**
     * @brief 初始化
     * @brief Initialization
     * @param pAddr, 地址, 换到应用程序的绝对地址
     * @param pAddr, Address, to the absolute address of the application
     * @param iSize, 内存大小
     * @param iSize, Memory Size
     * @param iBlockSize, block的大小
     * @param iBlockSize, Block size
     */
    void create(void *pAddr, size_t iSize, size_t iBlockSize);

    /**
     * @brief 连接
     * @brief Connect
     * @param pAddr 地址, 换到应用程序的绝对地址
     * @param pAddr Address, to the absolute address of the application
     */
    void connect(void *pAddr);

    /**
     * @brief 获取头地址指针
     * @brief Get Header Address Pointer
     */
    void *getHead()    const       { return _pHead; }

    /**
     * @brief 每个block的大小
     * @brief Size of each block
     *
     * @return block的大小
     * @return Block size
     */
    size_t getBlockSize()  const { return _pHead->_iBlockSize; }

    /**
     * @brief 总计内存大小 
     * @brief Total memory size
     * @return 内存大小
     * @return Memory size
     */
    size_t getMemSize()  const { return _pHead->_iSize; }

    /**
     * @brief 可以存放数据的总容量 
     * @brief Total capacity to store data
     * @return 总容量
     * @return total capacity
     */
    size_t getCapacity() const { return _chunk.getCapacity(); }

    /**
     * @brief 分配一个区块,绝对地址 
     * @brief Allocate a block, absolute address
     */
    void* allocate();

    /**
    * @brief 分配一个区块，返回以1为基数的区块索引， 
    *        没有可分配空间时返回0
    * @brief Assign a block and return the block index based on 1.
    *        Return 0 if there is no allocatable space
    * @param 区块索引
    * @param Block Index
    */
    void* allocate2(size_t &iIndex);

    /**
     * @brief 释放区块, 绝对地址
     * @brief Release block, absolute address
     * @param pAddr 区块的绝对地址
     * @param pAddr Absolute address of block
     */
    void deallocate(void *pAddr);

    /**
     * @brief 释放区块 
     * @brief Release Block
     * @param iIndex 区块索引
     * @param iIndex Block Index
     */
    void deallocate2(size_t iIndex);

    /**
     * @brief 获取所有chunk的区块合计的block的个数 
     * @brief Get the total number of blocks for all chunks
     * @return 合计的block的个数
     * @return Total number of blocks
     */
    size_t blockCount() const           { return _chunk.getBlockCount(); }

    /**
    * @brief 根据索引获取绝对地址 
    * @brief Obtain absolute address from index
    * @param 索引
    */
    void* getAbsolute(size_t iIndex)    { return _chunk.getAbsolute(iIndex); };

    /**
     * @brief 绝对地址换成索引
     * @brief Absolute Address to Index
     * @param pAddr   绝对地址 
     * @param pAddr   Absolute Address
     * @return size_t 索引
     * @return size_t Index
     */
    size_t getRelative(void *pAddr)     { return _chunk.getRelative(pAddr); };

    /**
     * @brief 获取头部信息 
     * @brief Get header information
     * @return 头部信息
     * @return Header Information
     */
    TC_MemChunk::tagChunkHead getBlockDetail() const;

    /**
     * @brief 重建
     * @brief Rebuild 
     */
    void rebuild();

    /**
     * @brief 头部内存块
     * @brief Header memory block
     */
#pragma pack(1) 
    struct tagChunkAllocatorHead
    {
        size_t  _iSize;
        size_t  _iBlockSize;
    };
#pragma pack() 

    /**
     * @brief 取获头部大小 
     * @brief Capture Head Size
     * @return 头部大小
     * @return Head Size
     */
    static size_t getHeadSize() { return sizeof(tagChunkAllocatorHead); }

protected:

    /**
     * @brief 初始化
     * @brief Initialization
     */
    void init(void *pAddr);

    /**
     * @brief 初始化
     * @brief Initialization
     */
    void initChunk();

    /**
     * @brief 连接
     * @brief Connect
     */
    void connectChunk();

    /**
    *@brief 不允许copy构造 
    *@brief Copy construction not allowed
    */ 
    TC_MemChunkAllocator(const TC_MemChunkAllocator &);
    /**
     *@brief 不允许赋值  
     *@brief Assignment not allowed
     */ 
    TC_MemChunkAllocator& operator=(const TC_MemChunkAllocator &);
    bool operator==(const TC_MemChunkAllocator &mca) const;
    bool operator!=(const TC_MemChunkAllocator &mca) const;

private:

    /**
     * 头指针
     * Head Pointer
     */
    tagChunkAllocatorHead   *_pHead;

    /**
     *  chunk开始的指针
     *  Pointer to chunk start
     */
    void                    *_pChunk;

    /**
     * chunk链表
     * Chunk Linked List
     */
    TC_MemChunk             _chunk;
};

/**
 * @brief 多块分配器,可以分配多个不同大小的块
 * @brief Multi-block allocator that can allocate multiple blocks of different sizes
 *  
 * 内部每种块用TC_MemChunkAllocator来分配, 
 * Each block inside is allocated with TC_MemChunkAllocator,
 *  
 * 每种大小不同块的个数是相同的, 内存块分配的策略如下: 
 * The number of blocks of different sizes is the same, and the strategies for memory block allocation are as follows:
 *  
 *  确定需要分配多大内存，假设需要分配A字节的内存；
 *  Determine how much memory you need to allocate, assuming you need to allocate A bytes of memory;
 *  
 * 分配大小大于>=A的内存块，优先分配大小最接近的；
 * Memory blocks with allocation size greater than >=A have the closest priority allocation size.
 *  
 * 如果都没有合适内存块，则分配大小<A的内存块，优先分配大小最接近的；
 * If there are no suitable memory blocks, allocate a memory block of size <A, with the closest priority allocation size;
 *  
 * 如果仍然没有合适内存块，则返回NULL；
 * If there is still no suitable memory block, NULL is returned.
 *  
 * 初始化时指定:最小块大 小, 最大块大小, 块间大小比值
 * Specify at initialization: minimum block size, maximum block size, inter-block size ratio
 *  
 * 自动计算出块的个数(每种大小块的个数相同) 
 * Automatically calculate the number of blocks (the same number for each size)
 */
class TC_MemMultiChunkAllocator
{
public:

    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_MemMultiChunkAllocator();

    /**
     * @brief 析够函数
     * @brief Destructor
     */
    ~TC_MemMultiChunkAllocator();


    /**
     * @brief 初始化
     * @brief Initialization
     * @param pAddr          地址, 换到应用程序的绝对地址
     * @param pAddr          Address, to the absolute address of the application
     * @param iSize          内存大小
     * @param iSize          Memory size
     * @param iMinBlockSize  block的大小下限
     * @param iMinBlockSize  Lower size limit for blocks
     * @param iMaxBlockSize  block的大小上限
     * @param iMaxBlockSize  Maximum Block Size
     * @param fFactor        因子
     * @param fFactor        Facotor
     */
    void create(void *pAddr, size_t iSize, size_t iMinBlockSize, size_t iMaxBlockSize, float fFactor = 1.1);

    /**
     * @brief 连接上
     * @brief Connect
     * @param pAddr 地址, 换到应用程序的绝对地址
     * @param pAddr Address, to the absolute address of the application
     */
    void connect(void *pAddr);

    /**
     * @brief 扩展空间
     * @brief Expand space
     * 
     * @param pAddr 已经是空间被扩展之后的地址
     * @param pAddr Address already after space has been expanded
     * @param iSize
     */
    void append(void *pAddr, size_t iSize);

    /**
     * @brief 获取每个block的大小, 包括后续增加的内存块的大小
     * @brief Get the size of each block, including the size of subsequently added memory blocks
     *
     * @return vector<size_t>block大小的vector
     * @return Vector<size_T>block size vector
     */
    vector<size_t> getBlockSize()  const;

    /**
     * @brief 每个block中chunk个数(都是相等的) 
     * @brief Number of chunks per block (all equal)
     * @return chunk个数 
     * @return Number of chunks
     */
    size_t getBlockCount() const { return _iBlockCount; }

    /**
     * @brief 获取每个块头部信息, 包括后续增加的内存块的大小
     * @brief Get the header information for each block, including the size of subsequently added memory blocks
     * @param i
     *
     * @return vector<TC_MemChunk::tagChunkHead>
     */
    vector<TC_MemChunk::tagChunkHead> getBlockDetail() const;

    /**
     * @brief 总计内存大小, 包括后续增加的内存块的大小
     * @brief Total memory size, including subsequent increases in memory block size
     *
     * @return size_t
     */
    size_t getMemSize()  const              { return _pHead->_iTotalSize; }

    /**
     * @brief 真正可以放数据的容量, 包括后续增加的内存块的数据容量 
     * @brief The real capacity to hold data, including subsequent increases in memory blocks
     * @return 可以放数据的容量
     * @return Capacity to hold data
     */
    size_t getCapacity() const;

    /**
     * @brief 一个chunk的block个数, 包括后续增加的内存块的 
     * @brief Number of blocks for a chunk, including subsequently added memory blocks
     * @return vector<size_t>block个数
     * @return vector<size_t>block count
     */
    vector<size_t> singleBlockChunkCount() const;

    /**
     * @brief 所有chunk的区块合计的block的个数 
     * @brief The total number of blocks for all chunks
     * @return 合计的block的个数
     * @return Total number of blocks
     */
    size_t allBlockChunkCount() const;

    /**
     * @brief 分配一个区块,绝对地址 
     * @brief Allocate a block, absolute address
     * @param iNeedSize   需要分配的大小
     * @param iNeedSize   Size to be allocated
     * @param iAllocSize  分配的数据块大小
     * @param iAllocSize  Allocated data block size
     */
    void* allocate(size_t iNeedSize, size_t &iAllocSize);

    /**
     * @brief 分配一个区块, 返回区块索引
     * @brief Allocate a block and return the block index
     * @param iNeedSize    需要分配的大小
     * @param iNeedSize    Size to be allocated
     * @param iAllocSize   分配的数据块大小
     * @param iAllocSize   Allocated data block size
     * @param               size_t，以1为基数的索引，0表示无效
     * @param               size_t，Index based on 1, 0 is invalid
     */
    void* allocate2(size_t iNeedSize, size_t &iAllocSize, size_t &iIndex);

    /**
     * @brief 释放区块
     * @brief Release Block
     * @param p 绝对地址
     * @param p Absolute Address
     */
    void deallocate(void *pAddr);

    /**
     * @brief 释放区块
     * @brief Release Block
     * @param iIndex 区块索引
     * @param iIndex Block Index
     */
    void deallocate2(size_t iIndex);

    /**
     * @brief 重建
     * @brief Rebuild 
     */
    void rebuild();

    /**
     * @brief 相对索引换算成绝对地址
     * @brief Convert relative index to absolute address
     * @param iIndex 相对索引 
     * @param iIndex Relative Index
     * @return       绝对地址指针
     * @return       Absolute Address Pointer
     */
    void *getAbsolute(size_t iIndex);

    /**
     * @brief  绝对地址换成索引地址
     * @brief  Replace absolute address with index address
     * @param  绝对地址
     * @param  Absolute Address
     * @return 索引地址
     * @return Index Address
     */
    size_t getRelative(void *pAddr);

    /**
     * @brief 头部内存块
     * @brief Header memory block
     */
#pragma pack(1) 
    struct tagChunkAllocatorHead
    {
        /**Current block size*/
        size_t  _iSize;             /**当前块大小*/
        /**Size of subsequent allocation blocks combined*/
        size_t  _iTotalSize;        /**后续分配块合在一起的大小*/
        size_t  _iMinBlockSize;
        size_t  _iMaxBlockSize;
        float   _fFactor;
        /**Next allocator address, if not 0*/
        size_t  _iNext;             /**下一个分配器地址, 如果没有则为0*/
    };
#pragma pack() 

    /**
     * @brief 头部大小
     * @brief Head Size
     *
     * @return size_t
     */
    static size_t getHeadSize() { return sizeof(tagChunkAllocatorHead); }

protected:

    /**
     * @brief 初始化
     * @brief Initialization
     */
    void init(void *pAddr);

    /**
     * @brief 计算
     * @brief Calculation
     */
    void calc();

    /**
     * @brief 清空
     * @brief Clear up
     */
    void clear();

    /**
     * @brief 最后一个分配器
     * @brief Last allocator
     * 
     * @return TC_MemMultiChunkAllocator*
     */
    TC_MemMultiChunkAllocator *lastAlloc();

    /**
     *@brief 不允许copy构造
     *@brief Copy construction not allowed
     */
    TC_MemMultiChunkAllocator(const TC_MemMultiChunkAllocator &);
    /** 
      * @brief 不允许赋值
      * @brief Assignment not allowed
      */
    TC_MemMultiChunkAllocator& operator=(const TC_MemMultiChunkAllocator &);
    bool operator==(const TC_MemMultiChunkAllocator &mca) const;
    bool operator!=(const TC_MemMultiChunkAllocator &mca) const;

private:

    /**
     * 头指针
     * Head Pointer
     */
    tagChunkAllocatorHead   *_pHead;

    /**
     *  chunk开始的指针
     *  Pointer to chunk start
     */
    void                    *_pChunk;

    /**
     * 区块大小
     * block size
     */
    vector<size_t>          _vBlockSize;

    /**
     * 每个chunk中block的个数
     * Number of blocks per chunk
     */
    size_t                  _iBlockCount;

    /**
     * chunk链表
     * Chunk Linked List
     */
    vector<TC_MemChunkAllocator*>       _allocator;

    /**
     * 所有的索引个数
     * Number of all indexes
     */
    size_t                              _iAllIndex;

    /**
     * 后续的多块分配器
     * Subsequent multiblock allocators
     */
    TC_MemMultiChunkAllocator           *_nallocator;
};

}

#endif
