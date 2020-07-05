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

#ifndef __TC_MEM_QUEUE_H__
#define __TC_MEM_QUEUE_H__

#include <string>

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file   tc_mem_queue.h 
 * @brief  共享内存循环队列类. 
 * @brief  Shared Memory Loop Queue Class
 *  
 */
             
/////////////////////////////////////////////////
/**
* @brief 内存循环队列, 同时读写必须加锁 
* @brief Memory loop queue, read and write must be locked at the same time
*  
*  内存循环队列，建议不要直接使用该类，通过jmem组件来使用
*  Memory Loop Queue, recommended not to use this class directly, using jmem components
*  
* 做了保护，即使被kill掉，队列不会坏掉，最多错误一个数据
* Protected so that even if killed, the queue will not break, at most one data error
*/
class TC_MemQueue
{
public:

    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_MemQueue();

    /**
    * @brief 析构
    * @brief Destructor
    */
    ~TC_MemQueue(){}

    /**
    * @brief 初始化
    * @brief Initialization
    * @param pAddr 指令队列空间的指针
    * @param pAddr Pointer to instruction queue space
    * @param iSize 空间的指针
    * @param iSize Pointer to space
    */
    void create(void *pAddr, size_t iSize);

    /**
    * @brief 连接上队列
    * @brief Connect Queue
    * @param pAddr 指令队列空间的指针
    * @param pAddr Pointer to instruction queue space
    */
    void connect(void *pAddr, size_t iSize);
    
    /**
     * @author goodenpei
     * 2016-09-12
     * @brief 取第一个数据
     * @brief Take the first data
     * @param sOut   输出的数据快
     * @param sOut   Fast data output
     * @return       bool,true:正确, false: 错误,无数据输出,队列空
     * @return       Bool, true: correct, false: error, no data output, empty queue
     */
    bool front(string &sOut);

    /**
    * @brief 弹出数据
    * @brief Pop-up data
    * @param sOut   输出的数据块
    * @param sOut   Output data block
    * @return       bool,true:正确, false: 错误,无数据输出,队列空 
    * @return       Bool, true: correct, false: error, no data output, empty queue
    */
    bool pop_front(string &sOut);

    /**
    * @brief 进入数据
    * @brief Enter data
    * @param  sIn 输入的数据块
    * @param  sIn Input data block
    * @return     bool,true:正确, false: 错误, 队列满
    * @return     Bool, true: correct, false: error, the queue is full
    */
    bool push_back(const string &sIn);

    /**
    * @brief 进入数据
    * @brief Enter data
    * @param pvIn   输入的数据块
    * @param pvIn   Input data block
    * @param iSize  输入数据块长度
    * @param iSize  Input data block length
    * @return       bool,true:正确, false: 错误, 队列满
    * @return       Bool, true: correct, false: error, the queue is full
    */
    bool push_back(const char *pvIn, size_t iSize);

    /**
    * @brief 队列是否满
    * @brief Is the queue full
    * @param iSize 输入数据块长度
    * @param iSize Input data block length
    * @return      bool,true:满, false: 非满
    * @return      Bool, true:full, false:not full
    */
    bool isFull(size_t iSize);


    /**
     * 获取空闲的空间大小
     * Get free space size
     */
    size_t getFreeSize();

    /**
    * @brief 队列是否空
    * @brief Is the queue empty
    * @return  bool,true: 满, false: 非满
    * @return  Bool, true:full, false:not full
    */
    bool isEmpty();

    /**
    * @brief 队列中元素个数, 不加锁的情况下不保证一定正确
    * @brief The number of elements in the queue is not guaranteed to be correct without locking
    * @return size_t, 元素个数
    * @return size_t, Number of elements
    */
    size_t elementCount();

    /**
    * @brief 队列长度(字节), 
    *        小于总存储区长度(总存储区长度包含了控制快)
    * @brief Queue length (bytes), 
    * less than total storage length (total storage length includes fast control)
    * @return size_t，队列长度
    * @return size_t，queue length
    */
    size_t queueSize();

    /**
    * @brief 共享内存长度
    * @brief Shared memory length
    * @return size_t 共享内存长度
    * @return size_t Shared memory length
    */
    size_t memSize() const {return _size;};

protected:
    /**
     * @brief 修改具体的值
     * @brief Modify specific values
     * @param iModifyAddr   需要被修改的值
     * @param iModifyAddr   Values that need to be modified
     * @param iModifyValue  替换的值
     * @param iModifyValue  Replaced Values
     */
    void update(void* iModifyAddr, size_t iModifyValue);

    /**
     * @brief 修改具体的值
     * @brief Modify specific values
     * @param iModifyAddr   需要被修改的值
     * @param iModifyAddr   Values that need to be modified
     * @param iModifyValue  替换的值
     * @param iModifyValue  Replaced Values
     */
    void update(void* iModifyAddr, bool bModifyValue);

    /**
     * @brief 修改更新到内存中
     * @brief Modify Update to Memory
     */
    void doUpdate(bool bUpdate = false);

protected:

    /**
    *  @brief 队列控制结构
    *  @brief Queue Control Structure
    */
#pragma pack(1) 
    struct CONTROL_BLOCK
    {
        /**Memory size*/
        size_t iMemSize;            /**内存大小*/
        /**Top Element Index, content ending address*/
        size_t iTopIndex;           /**顶部元素索引,内容结束地址*/
        /**Bottom element index, content start address*/
        size_t iBotIndex;           /**底部元素索引,内容开始地址*/
        /**Number of entry elements in the queue*/
        size_t iPushCount;          /**队列中进入元素的个数*/
        /**Number of pop-up elements in the queue*/
        size_t iPopCount;           /**队列中弹出元素的个数*/
    };

    /**
     * @brief 需要修改的地址
     * @brief Addresses that need to be modified
     */
    struct tagModifyData
    {
        /**Modified Address*/
        size_t  _iModifyAddr;       /**修改的地址*/
        /**Bytes*/
        char    _cBytes;            /**字节数*/
        /**Value*/
        size_t  _iModifyValue;      /**值*/
    };

    /**
     * @brief 修改数据块头部
     * @brief Modify Data Block Header
     */
    struct tagModifyHead
    {
        /**Modification Status: 0: No one is modifying at present, 1: Start preparing to modify, 2: Finish modifying, no copy in memory*/
        char            _cModifyStatus;         /**修改状态: 0:目前没有人修改, 1: 开始准备修改, 2:修改完毕, 没有copy到内存中*/
        /**Update to current index, cannot operate on 10*/
        size_t          _iNowIndex;             /**更新到目前的索引, 不能操作10个*/
        /**Up to 5 changes at a time*/
        tagModifyData   _stModifyData[5];       /**一次最多5次修改*/
    };
#pragma pack() 

    /**
    * 队列控制快(内存的起点)
    * Quick Queue Control (Start of Memory)
    */
    CONTROL_BLOCK   *_pctrlBlock;

    /**
    * 内存数据地址
    * Memory data address
    */
    void            *_paddr;

    /**
    * 共享内存
    * Shared memory
    */
    size_t            _size;

    /**
     * 修改数据块
     * Modify Data Block
     */
    tagModifyHead   *_pstModifyHead;


};

}

#endif
