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
#ifndef __TC_MALLOC_CHUNK_H
#define __TC_MALLOC_CHUNK_H

#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

namespace tars
{
    /////////////////////////////////////////////////
    /**  
    * @file tc_mem_chunk.h
    * @brief 内存分配器 
    * @brief Memory allocator
    *          
    */ 
    static const size_t kPageShift  = 15;
    static const size_t kNumClasses = 78;
    static const size_t kPageSize   = 1 << kPageShift;
    static const size_t kMaxSize    = 256 * 1024;
    static const size_t kAlignment  = 8;
    static const size_t kMaxPages    = 1 << (20 - kPageShift);
    static const size_t kPageAlignment = 64;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
    * 请求的内存大小与请求实际得到的内存大小的映射
    * The mapping of the requested memory size to the actual obtained memory size of the request
    * 实际得到的内存大小与内存大小所属的大小类别的映射
    * The mapping of the actual memory size to the size category to which the memory size belongs
    * 内存大小所属的大小类别与该内存大小类别需要的内存页的映射
    * The mapping of the size category to which the memory size belongs to and the memory pages required by the memory size category
    */
    class SizeMap
    {
    public:
        SizeMap() { Init(); }    

        /*
        *size所属的尺寸类别
        *Size Category to which size belongs
        */
        inline int SizeClass(int size)
        {
            return class_array_[ClassIndex(size)];
        }

        /*
        *尺寸类别cl对应的尺寸大小
        *Dimension size corresponding to dimension category cl
        */
        inline size_t ByteSizeForClass(size_t cl)
        {
            return class_to_size_[cl];
        }

        /*
        *尺寸类别cl对应的页数
        *Number of pages corresponding to dimension category cl
        */
        inline size_t class_to_pages(size_t cl)
        {
            return class_to_pages_[cl];
        }

    private:
        static inline size_t ClassIndex(int s)
        {
            const bool big = (s > kMaxSmallSize);
            const size_t  add_amount= big ? (127 + (120<<7)) : 7;
            const size_t  shift_amount = big ? 7 : 3;
            return (s + add_amount) >> shift_amount;
        }

        static inline int LgFloor(size_t n)
        {
            int log = 0;
            for (int i = 4; i >= 0; --i)
            {
                int shift = (1 << i);
                size_t x = n >> shift;
                if (x != 0)
                {
                    n = x;
                    log += shift;
                }
            }

            return log;
        }

        /*
        *初始化
        *Initialization
        */
        void Init();

        size_t AlignmentForSize(size_t size);

        int NumMoveSize(size_t size);
    private:
        static const int kMaxSmallSize        = 1024;
        static const size_t kClassArraySize = ((kMaxSize + 127 + (120 << 7)) >> 7) + 1;
        size_t            class_to_size_[kNumClasses];
        size_t            class_to_pages_[kNumClasses];
        unsigned char    class_array_[kClassArraySize];

    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    class Static
    {
    public:
        static SizeMap* sizemap()
        {
            return &_sizemap;
        }
    private:
        static SizeMap _sizemap;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
    * TC_Span内存分配器
    * TC_Span Memory Allocator
    *
    * 用于分配sizeof(TC_Span)大小的内存块
    * Used to allocate sizeof (TC_Span) size memory block
    */
    class TC_SpanAllocator
    {
    public:
        TC_SpanAllocator() : _pHead(NULL), _pData(NULL) {}

        void* getHead() const { return _pHead; }

        /**
         * 初始化
         * Initialization
         * @param pAddr, 地址, 换到应用程序的绝对地址
         * @param pAddr,Address, to the absolute address of the application
         * @param mem_size, 内存大小
         * @param mem_size, Memory size
         */
        void create(void* pAddr, size_t iSpanSize, size_t iSpanCount);

        /**
         * 连接上
         * Connect
         * @param pAddr, 地址, 换到应用程序的绝对地址
         * @param pAddr, Address, to the absolute address of the application
         */
        void connect(void* pAddr);

        /**
         * 重建
         * Rebuild
         */
        void rebuild();

        /**
         * 是否还有可用block
         * Is there still a block available
         *
         * @return bool
         */
        bool isSpanAvailable() const { return _pHead->_spanAvailable > 0; }

        /**
         * 分配一个区块
         * Allocate a block
         *
         * @return void*
         */
        void* allocate();

        /**
         * 释放区块
         * Release Block
         * @param pAddr
         */
        void deallocate(void *pAddr);

#pragma pack(1) 
        struct tagSpanAlloc
        {
            /*Span Block Size*/
            size_t  _iSpanSize;            /**span区块大小*/
            /*Number of spans*/
            size_t  _iSpanCount;           /**span个数*/
            /*First available span index*/
            size_t  _firstAvailableSpan;   /**第一个可用的span索引*/
            /*Number of available spans*/
            size_t  _spanAvailable;        /**可用span个数*/
        };
#pragma pack() 

        static size_t getHeadSize() { return sizeof(tagSpanAlloc); }

    protected:
        /**
         * 初始化
         * Initialization
         */
        void init(void *pAddr);

        //禁止copy构造
        //Prohibit copy construction
        TC_SpanAllocator(const TC_SpanAllocator &mcm);

        //禁止复制
        //Prohibit copying
        TC_SpanAllocator &operator=(const TC_SpanAllocator &mcm);

    private:
        /**
         * 区块头指针
         * Block Header Pointer
         */
        tagSpanAlloc    *_pHead;

        /**
         * 数据区指针
         * Data area pointer
         */
        unsigned char   *_pData;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
    * 内存分配器
    * allocator
    *
    * 将连续的内存按32K大小分页
    * Paging contiguous memory at 32K size
    * 并通过TC_Span和PageMap进行管理
    * And through TC_Span and PageMap are managed
    */
    class TC_Page
    {
    public:
#pragma pack(1) 

        struct tagShmFlag
        {
            /*Whether to Set up Protected Areas*/
            bool         _bShmProtectedArea;    /*是否设置保护区*/
            /*Memory space identification, 0 before and 1 after memory use*/
            int          _iShmFlag;                /*内存空间标识，内存未使用前为0，使用后为1*/
            /*Memory space size*/
            size_t       _iShmMemSize;            /*内存空间大小*/
            /*Start address, relative address of TC_Span memory area*/
            size_t       _iShmSpanAddr;            /*TC_Span内存区的起始地址，相对地址*/
            /*Start address, relative address of PageMap memory area*/
            size_t       _iShmPageMapAddr;        /*PageMap内存区的起始地址，相对地址*/
            /*Actual address, relative address of the memory area of the Data area*/
            size_t       _iShmPageAddr;            /*Data区的内存区的其实地址，相对地址*/
            /*Number of spans in TC_Span memory area*/
            size_t       _iShmSpanNum;            /*TC_Span内存区中span的个数*/
            /*Number of maps in PageMap memory area*/
            size_t       _iShmPageMapNum;        /*PageMap内存区中map的个数*/
            /*Number of pages in Data memory area*/
            size_t       _iShmPageNum;            /*Data内存区中页的个数*/
        };

        struct tagModifyData
        {
            /*Modified Address*/
            size_t       _iModifyAddr;            /*修改的地址*/
            /*Number of bytes*/
            char         _cBytes;                /*字节数*/
            /*Value*/
            size_t       _iModifyValue;            /*值*/
        };

        struct tagModifyHead
        {
            /*Modification Status: 0: No one is modifying at present, 1: Start preparing to modify, 2: Finish modifying, no copy in memory*/
            char           _cModifyStatus;        /*修改状态: 0:目前没有人修改, 1: 开始准备修改, 2:修改完毕, 没有copy到内存中*/
            /*Update to current index*/
            size_t         _iNowIndex;            /*更新到目前的索引*/
            /*Up to 32 changes at a time*/
            tagModifyData  _stModifyData[32];    /*一次最多32次修改*/
        };

        struct TC_Span
        {
            /*Starting page number of page memory managed by space*/
            size_t       start;                    /*span所管理的页内存的起始页号*/
            /*Number of pages*/
            size_t       length;                /*页的个数*/
            size_t       next;
            size_t       prev;
            /*Used to allocate memory*/
            size_t       objects;                /*用于分配内存*/
            /*Number of memory owned by span divided by sizeclass class size*/
            unsigned int refcount;                /*按照sizeclass类大小划分span拥有的内存后的个数*/
            /*Category of memory size*/
            unsigned int sizeclass;                /*内存大小的类别*/
            /*Used to identify if the space is idle or in use*/
            unsigned int location;                /*用于识别该span是空闲，还是处于使用中*/

            enum { IN_USE, ON_FREELIST };
        };    

        struct TC_CenterList
        {
            /*Category of memory size*/
            size_t       size_class;            /*内存大小的类别*/
            /*Free Chain List*/
            TC_Span      empty;                    /*空闲链表*/
            /*Non-Idle Chain List*/
            TC_Span      nonempty;                /*非空闲链表*/
        };
#pragma pack() 

    public:
        TC_Page() : _pShmFlagHead(NULL),_pModifyHead(NULL),_pCenterCache(NULL),_pLarge(NULL),_pFree(NULL),_pSpanMemHead(NULL),_pPageMap(NULL),_pData(NULL) {}

        /**
         * 初始化
         * Initialization
         * @param pAddr, 地址, 换到应用程序的绝对地址
         * @param pAddr, Address, to the absolute address of the application
         * @param iShmMemSize, 内存大小
         * @param iShmMemSize, Memory size
         * @param bProtectedArea, 是否使用保护区
         * @param bProtectedArea, Whether to use protected areas
         */
        void create(void *pAddr, size_t iShmMemSize, bool bProtectedArea);

        /**
         * 连接上
         * Connect
         * @param pAddr, 地址, 换到应用程序的绝对地址
         * @param pAddr, Address, to the absolute address of the application
         */
        void connect(void *pAddr);

        /**
         * 重建
         * Rebuild
         */
        void rebuild();

        /**
         * 分配一个区块
         * Allocate a block
         * @param iClassSize,需要分配的内存大小类别
         * @param iClassSize,Memory Size Category to Allocate
         * @param iAllocSize, 分配的数据块大小
         * @param iAllocSize, Allocated data block size
         * @param iPageId, 该内存所属的的起始页号
         * @param iPageId, TC_Span to which this memory belongsStarting page number
         * @param iIndex, 该内存所属的TC_Span的按iAllocSize大小划分后的第iIndex个
         * @param iIndex, The iIndex of the TC_Span to which this memory belongs, divided by the size of iAllocSize
         * @return void *
         */
        void* fetchFromSpansSafe(size_t iClassSize, size_t &iAllocSize, size_t &iPageId, size_t &iIndex);

        /**
         * 释放内存, 根据该内存所属TC_Span的起始页和该TC_Span按大小类别划分后的第iIndex个
         * Release memory according to the TC_to which it belongsStart page of Span and this TC_Span iIndex by size category
         * @param iPageId, 该内存所属的TC_Span的起始页号
         * @param iPageId, Starting page number of TC_Span to which this memory belongs
         * @param iIndex, 该内存所属的TC_Span的按iAllocSize大小划分后的第iIndex个
         * @param iIndex, The iIndex of the TC_Span to which this memory belongs, divided by the size of iAllocSize
         */
        void  releaseToSpans(size_t iPageId, size_t iIndex);

        /**
         * 释放内存, 绝对地址
         * Release memory, absolute address
         * @param pAddr
         */
        void  releaseToSpans(void* pObject);

        /**
         * 根据该内存所属TC_Span的起始页和该TC_Span按大小类别划分后的第iIndex个得到该内存块的起始绝对地址
         * The starting absolute address of the memory block is obtained according to the starting page of the memory TC_Span and the iIndex of the TC_Span division
         * @param iPageId, 该内存所属的TC_Span的起始页号
         * @param iPageId, Starting page number of TC_Span to which this memory belongs
         * @param iIndex, 该内存所属的TC_Span的按iAllocSize大小划分后的第iIndex个
         * @param iIndex, The iIndex of the TC_Span to which this memory belongs, divided by the size of iAllocSize
         */
        void* getAbsolute(size_t iPageId, size_t iIndex);

        /**
         * 修改更新到内存中
         * Modify Update to Memory
         */
        void doUpdate(bool bUpdate = false);

        /**
         * 获得用于存放数据的页内存的数量
         * Get the amount of page memory used to store data
         */
        inline size_t getPageNumber();

        /**
         * 获得用于存放数据的页内存的大小
         * Get the size of the page memory used to store the data
         */
        inline size_t getPageMemSize();

        /**
         * 获得TC_Page所管理内存的结束位置
         * Get the end of TC_Page managed memory
         */
        inline size_t getPageMemEnd();

        /**
         * 传给TC_Page的内存的最小大小
         * Minimum size of memory passed to TC_Page
         */
        static size_t getMinMemSize() 
        { 
            return sizeof(tagShmFlag) + sizeof(tagModifyHead) + sizeof(TC_CenterList) * kNumClasses + sizeof(TC_Span) + sizeof(TC_Span) * kMaxPages + TC_SpanAllocator::getHeadSize() + sizeof(TC_Span) + sizeof(size_t) + kPageSize; 
        }

    protected:

        //禁止copy构造
        //Prohibit copy construction
        TC_Page(const TC_Page &mcm);

        //禁止复制
        //Prohibit copying
        TC_Page &operator=(const TC_Page &mcm);

        /**
         * 初始化
         * Initialization
         */
        void init(void *pAddr);

        /**
         * 按页初始化用于存放数据的内存
         * Page-by-page initialization of memory for storing data
         */
        void initPage(void *pAddr);


        /////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * 初始化list双向链表
         * Initialize list bi-directional list
         */
        inline void DLL_Init(TC_Span* list, size_t iIndex);

        /**
         * 从双向链表中删除span指向的节点
         * Delete span-pointed nodes from a two-way chain table
         */
        inline void DLL_Remove(TC_Span* span);

        /**
         * 将span指向的节点加入到双向链表list中
         * Add the node span points to to the two-way list of chains
         */
        inline void DLL_Prepend(TC_Span* list, TC_Span* span);

        /**
         * 双向链表list是否为空
         * Is the bi-directional list empty
         */
        inline bool DLL_IsEmpty(TC_Span* list, size_t iIndex) 
        {
          return list->next == iIndex;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * 得到PageMap中第k个元素存放的指针值，其实际意义就是看Data内存区中第k块内存页属于哪个TC_Span管理
         * Getting the pointer value for the kth element in PageMap actually means to see which TC_Span management the kth memory page in the data memory area belongs to
         */
        size_t Get(size_t k) const
        {
            return _pPageMap[k];
        }

        /**
         * 设置PageMap中第k个元素存放的指针值，其实际意义就是看Data内存区中第k块内存页属于哪个TC_Span管理
         * Setting the pointer value for the kth element in PageMap actually means to see which TC_Span management the kth memory page in the data memory area belongs to
         */
        void Set(size_t k, size_t v)
        {
            _pPageMap[k] = v;
        }

        /**
         * 确保n未超过页内存的数目与k之差
         * Ensure that n does not exceed the difference between the number of page memory and k
         */
        bool Ensure(size_t k, size_t n)
        {
            return n <= _pShmFlagHead->_iShmPageMapNum -k;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * create或rebuild的时候会调用，用于初始化时将所有的内存页映射到_pLarge上
         * Called when creating or rebuild to map all memory pages to _pLarge during initialization
         */
        bool UseShmMem();

        /**
         * 得到内存页pPageId属于哪个TC_Span管理
         * Get which TC_Span management the memory page pPageId belongs to
         */
        TC_Span* GetDescriptor(size_t pPageId);

        /**
         * 在_pFree或_pLarge链表中查找n块连续的内存页，通过TC_Span返回
         * Find n contiguous memory pages in a _pFree or _pLarge chain table by TC_Span Return
         */
        TC_Span* SearchFreeAndLargeLists(size_t n);

        /**
         * 分配n块连续的内存页，通过TC_Span返回，里面会调用SearchFreeAndLargeLists函数
         * Allocate n contiguous memory pages via TC_Span returns, which calls the SearchFreeAndLargeLists function
         */
        TC_Span* New(size_t n);

        /**
         * 在_pLarge链表中分配n块连续的内存页，通过TC_Span返回
         * Allocate n contiguous memory pages in the _pLarge chain table by TC_Span Return
         */
        TC_Span* AllocLarge(size_t n);

        /**
         * 在span所管理的内存页中，分配出n块连续内存
         * Allocate n blocks of contiguous memory in a space-managed memory page
         */
        TC_Span* Carve(TC_Span* span, size_t n);

        /**
         * 根据span所管理的内存页的数目，将该span插入到_pLarge或_pFree链表中
         * Insert a span into an _pLarge or _pFree list based on the number of memory pages it manages
         */
        void PrependToFreeList(TC_Span* span);

        /**
         * 将span从它所属的链表中删除，里面调用MergeIntoFreeList
         * Delete the span from the list to which it belongs, calling MergeIntoFreeList
         */
        void Delete(TC_Span* span);

        /**
         * 将span从它所属的链表中删除，并且查看该span所管理的内存页前后连续的内存页是否空闲，若是，则进行合并操作
         * Delete a span from the list of chains to which it belongs and see if successive pages of memory managed by the span are free, and if so, merge.
         */
        void MergeIntoFreeList(TC_Span* span);

        /**
         * 设置span所管理的内存页将要进行分割的内存大小类别
         * Set the memory size category that span manages the memory pages to be split
         */
        void RegisterSizeClass(TC_Span* span, size_t sc);

        /**
         * 将span所属的内存页，映射到_pPageMap中
         * Map the memory page that span belongs to into _pPageMap
         */
        void RecordSpan(TC_Span* span);

        //////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * 分配一个区块
         * Allocate a block
         * @param iClassSize,需要分配的内存大小类别
         * @param iClassSize,Memory Size Category to Allocate
         * @param iAllocSize, 分配的数据块大小
         * @param iAllocSize, Allocated data block size
         * @param iPageId, 该内存所属的TC_Span的起始页号
         * @param iPageId, Starting page number of TC_Span to which this memory belongs
         * @param iIndex, 该内存所属的TC_Span的按iAllocSize大小划分后的第iIndex个
         * @param iIndex, The iIndex of the X to which this memory belongs, divided by the size of iAllocSize
         * @return void *
         */
        void* FetchFromSpans(size_t iClassSize, size_t &iAllocSize, size_t &iPageId, size_t &iIndex);

        /**
         * 按iClassSize类别的内存大小分割内存页
         * Split memory pages by iClassSize class memory size
         */
        int Populate(size_t iClassSize);

        /**
         * 修改具体的值
         * Modify specific values
         * @param iModifyAddr
         * @param iModifyValue
         */
        //inline void update(void* iModifyAddr, size_t iModifyValue);
        template<typename T>
        void update(void* iModifyAddr, T iModifyValue)
        {
            _pModifyHead->_cModifyStatus = 1;
            _pModifyHead->_stModifyData[_pModifyHead->_iNowIndex]._iModifyAddr  = reinterpret_cast<size_t>(iModifyAddr) - reinterpret_cast<size_t>(_pShmFlagHead);
            _pModifyHead->_stModifyData[_pModifyHead->_iNowIndex]._iModifyValue = iModifyValue;
            _pModifyHead->_stModifyData[_pModifyHead->_iNowIndex]._cBytes       = sizeof(iModifyValue);
            _pModifyHead->_iNowIndex++;

            assert(_pModifyHead->_iNowIndex < sizeof(_pModifyHead->_stModifyData) / sizeof(tagModifyData));
        }
    
    private:
        /**
         * 头部内存块头指针
         * Header Memory Block Header Pointer
         */
        tagShmFlag                *_pShmFlagHead;
        /**
         * 保护区内存块头指针
         * Protected Area Memory Block Header Pointer
         */
        tagModifyHead             *_pModifyHead;
        /**
         * 中央自由链表头指针
         * Central Free Chain Header Pointer
         */
        TC_CenterList             *_pCenterCache;
        /**
         * 大块内存页的链表指针（大于等于1MB内存块的链表）
         * Chain list pointer for large memory pages (chains greater than or equal to 1 MB of memory blocks)
         */
        TC_Span                   *_pLarge;
        /**
         * 小块内存页的链表头指针（小于1MB的内存块）
         * Chain header pointer for small memory pages (blocks less than 1 MB)
         */
        TC_Span                   *_pFree;
        /**
         * 用于分配TC_Span的内存区域的头指针
         * Header pointer for allocating the memory area of TC_Span
         */
        TC_SpanAllocator          *_pSpanMemHead;
        /**
         * 与数据区内存相映射的内存区头指针
         * Memory header pointer mapped to data area memory
         */
        //void                      **_pPageMap;
        size_t                      *_pPageMap;
        /**
         * 数据区的头指针
         * Header pointer for data area
         */
        void                      *_pData;

        /**
         * 用于分配TC_Span的内存分配器
         * memory allocator used to allocate TC_Span
         */
        TC_SpanAllocator          _spanAlloc;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    class TC_MallocChunkAllocator
    {
    public:
        TC_MallocChunkAllocator():_pHead(NULL),_pChunk(NULL),_nallocator(NULL) {}

        ~TC_MallocChunkAllocator()
        {
            clear();
        }

        void clear()
        {
            if(_nallocator)
            {
                delete _nallocator;
                _nallocator = NULL;
            }

            _pHead = NULL;
            _pChunk = NULL;
        }

        /**
         * 初始化
         * Initialization
         * @param pAddr, 地址, 换到应用程序的绝对地址
         * @param pAddr, Address, to the absolute address of the application
         * @param iSize, 内存大小
         * @param iSize, memory size
         * @param bProtectedArea, 是否使用保护区,默认使用
         * @param bProtectedArea, Whether to use protected areas, by default
         */
        void create(void *pAddr, size_t iSize, bool bProtectedArea = true);

        /**
         * 连接上
         * Connect
         * @param pAddr, 地址, 换到应用程序的绝对地址
         * @param pAddr, Address, to the absolute address of the application
         */
        void connect(void *pAddr);

        /**
         * 扩展空间
         * Expand space
         * 
         * @param pAddr, 已经是空间被扩展之后的地址
         * @param pAddr, Address already after space has been expanded
         * @param iSize
         */
        void append(void *pAddr, size_t iSize);

        void* getHead() const { return _pHead; }

        /**
         * 总计内存大小, 包括后续增加的内存块的大小
         * Total memory size, including subsequent increases in memory block size
         *
         * @return size_t
         */
        size_t getMemSize() const { return _pHead->_iTotalSize; }

        /**
         * 用于存放数据的总计内存大小, 包括后续增加的内存块的大小
         * Total memory size used to store data, including subsequent increases in memory block size
         * 
         */
        size_t getAllCapacity();

        /**
         * 每个chunk的大小, 包括后续增加的内存块的大小
         * Size of each chunk, including subsequent increases in memory block size
         *
         * vector<size_t>
         */
        void   getSingleCapacity(vector<size_t> &vec_capacity);

        /**
         * 根据该内存所属TC_Span的起始页和该TC_Span按大小类别划分后的第iIndex个，换算成绝对地址
         * Converts to an absolute address based on the start page of the TC_Span to which the memory belongs and the iIndex of TC_Span divided by size category
         * @param iPageId
         * @param iIndex
         * @return void*
         */
        void* getAbsolute(size_t iPageId, size_t iIndex);

        /**
         * 分配一个区块,绝对地址
         * Allocate a block, absolute address
         *
         * @param iNeedSize,需要分配的大小
         * @param iNeedSize,Size to be allocated
         * @param iAllocSize, 分配的数据块大小
         * @param iAllocSize, Allocated data block size
         * @return void*
         */
        void* allocate(size_t iNeedSize, size_t &iAllocSize);

        /**
         * 释放区块, 绝对地址
         * Release block, absolute address
         * @param pAddr
         */
        void  deallocate(void* pAddr);

        /**
         * 分配一个区块
         * Allocate a block
         * @param iNeedSize,需要分配的大小
         * @param iNeedSize,Size to be allocated
         * @param iAllocSize, 分配的数据块大小
         * @param iAllocSize, Allocated data block size
         * @param iPageId, 该内存所属的TC_Span的起始页号
         * @param iPageId, Starting page number of TC_Span to which this memory belongs
         * @param iIndex, 该内存所属的TC_Span的按iAllocSize大小划分后的第iIndex个
         * @param iIndex, The iIndex of the X to which this memory belongs, divided by the size of iAllocSize
         * @return void *
         */
        void* allocate(size_t iNeedSize, size_t &iAllocSize, size_t &iPageId, size_t &iIndex);

        /**
         * 释放内存, 根据该内存所属TC_Span的起始页和该TC_Span按大小类别划分后的第iIndex个
         * Release memory based on the start page of the TC_Span to which it belongs and the iIndex divided by size category
         * @param iPageId, 该内存所属的TC_Span的起始页号
         * @param iPageId, Starting page number of TC_Span to which this memory belongs
         * @param iIndex, 该内存所属的TC_Span的按iAllocSize大小划分后的第iIndex个
         * @param iIndex, The iIndex of the X to which this memory belongs, divided by the size of iAllocSize
         */
        void  deallocate(size_t iPageId, size_t iIndex);

        /**
         * 重建
         * Rebuild
         */
        void rebuild();

        /**
         * 修改更新到内存中
         * Modify Update to Memory
         */
        void doUpdate(bool bUpdate = false);

        /**
         * 头部内存块
         * Header memory block
         */
#pragma pack(1) 
        struct tagChunkAllocatorHead
        {
            bool   _bProtectedArea;
            size_t _iSize;
            size_t _iTotalSize;
            size_t _iNext;
        };
#pragma pack() 

        /**
         * 头部内存块大小
         * size of the header memory block
         */
        static size_t getHeadSize() { return sizeof(tagChunkAllocatorHead); }

        /**
         * 传递给此内存分配器的内存块大小要不小于函数的返回值
         * The size of the memory block passed to this memory allocator is no smaller than the return value of the function
         */
        static size_t getNeedMinSize() { return sizeof(tagChunkAllocatorHead) + TC_Page::getMinMemSize(); }
    protected:
        void init(void *pAddr);

        void _connect(void *pAddr);

        TC_MallocChunkAllocator *lastAlloc();

        //禁止copy构造
        //Prohibit copy construction
        TC_MallocChunkAllocator(const TC_MallocChunkAllocator &);

        //禁止复制
        //Prohibit copying 
        TC_MallocChunkAllocator& operator=(const TC_MallocChunkAllocator &);

    private:
        /**
         * 头指针
         * head pointer
         */
        tagChunkAllocatorHead   *_pHead;
        /**
         *  chunk开始的指针
         *  Pointer to chunk start
         */
        void                    *_pChunk;
        /**
         *  chunk分配类
         *  Chunk assignment class
         */
        TC_Page                 _page;
        /**
         * 后续的多块分配器
         * Subsequent multiblock allocators
         */
        TC_MallocChunkAllocator *_nallocator;
    };
    
}
#endif
