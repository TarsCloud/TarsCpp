/************************************************************************

        Copyright (c) 2005-2007 by Juphoon System Software, Inc.
                       All rights reserved.

     This software is confidential and proprietary to Juphoon System,
     Inc. No part of this software may be reproduced, stored, transmitted,
     disclosed or used in any form or by any means other than as expressly
     provided by the written license agreement between Juphoon and its
     licensee.

     THIS SOFTWARE IS PROVIDED BY JUPHOON "AS IS" AND ANY EXPRESS OR 
     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     ARE DISCLAIMED. IN NO EVENT SHALL JUPHOON BE LIABLE FOR ANY DIRECT, 
     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
     OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
     POSSIBILITY OF SUCH DAMAGE. 

                    Juphoon System Software, Inc.
                    Email: support@juphoon.com
                    Web: http://www.juphoon.com

************************************************************************/
/*************************************************  
  File name     : zos_slist.h
  Module        : zos single list library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-02-03    
  Description   :
      Data structure and function definitions required 
      by the single list interface
    
  Modify History:   
  1. Date:        Author:         Modification:
  2005-02-04      leo.lv          create the file 

*************************************************/
#ifndef _ZOS_SLIST_H__
#define _ZOS_SLIST_H__

/** 
 * @file zos_slist.h
 * @brief Zos single list interfaces.
 *  
 * In this file, it defined the operation function
 * of zos single list, including create, delete, insert, remove
 * and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief List maximum node size(infinite) */
#define ZSLIST_INFINITE_SIZE ZMAXUINT

/** @brief Get the actual count of list nodes. */
#define ZOS_SLIST_SIZE(_list) \
    ((_list)->iCount)

/** @brief Check the full of list. */
#define ZOS_SLIST_ISFULL(_list) \
    ((_list)->iMaxNum && (_list)->iCount >= (_list)->iMaxNum)

/** @brief Check for list is empty. */
#define ZOS_SLIST_ISEMPTY(_list) \
    ((_list)->pstHead == ZNULL)

/** @brief Check for list is not empty. */
#define ZOS_SLIST_ISNOEMPTY(_list) \
    ((_list)->pstHead != ZNULL)

/** @brief Get the head node of list. */
#define ZOS_SLIST_HEAD_NODE(_list) (_list)->pstHead

/** @brief Get the tail node tail of list. */
#define ZOS_SLIST_TAIL_NODE(_list) (_list)->pstTail

/** @brief If node is not ZNULL get next node, or get ZNULL. */
#define ZOS_SLIST_NEXT_NODE(_node) ((_node) ? (_node)->pstNext : ZNULL)

/** @brief Get node data or ZNULL. */
#define ZOS_SLIST_GET_DATA(_node)  ((_node) ? (_node)->pData : ZNULL)

/** @brief Transform node to node->pData. */
#define ZOS_SLIST_SET_DATA(_node)  (_node)->pData

/** @brief If the list head is not ZNULL get it's pDate, or get ZNULL. */
#define ZOS_SLIST_HEAD_DATA(_list) \
    (ZOS_SLIST_HEAD_NODE(_list)) ? ZOS_SLIST_HEAD_NODE(_list)->pData : ZNULL

/** @brief If the list tail is not ZNULL get it's pDate, or get ZNULL. */
#define ZOS_SLIST_TAIL_DATA(_list) \
    (ZOS_SLIST_TAIL_NODE(_list)) ? ZOS_SLIST_TAIL_NODE(_list)->pData : ZNULL

/** @brief If the next node is not ZNULL get it's pDate, or get ZNULL. */
#define ZOS_SLIST_NEXT_DATA(_node) \
    (ZOS_SLIST_NEXT_NODE(_node)) ? ZOS_SLIST_NEXT_NODE(_node)->pData : ZNULL

/** @brief Enumerate all node in list. */ 
#define FOR_ALL_NODE_IN_SLIST(_list, _node) \
  for (_node = ZOS_SLIST_HEAD_NODE(_list); \
       _node; _node = (_node)->pstNext)

/** @brief Enumerate all node in list, safe for remove node. */
#define FOR_ALL_NODE_IN_SLIST_NEXT(_list, _node, _nextnode) \
  for (_node = ZOS_SLIST_HEAD_NODE(_list), \
       _nextnode = ZOS_SLIST_NEXT_NODE(_node); \
       _node; _node = _nextnode, \
       _nextnode = ZOS_SLIST_NEXT_NODE(_node))

/** @brief Enumerate all node and data in list. */ 
#define FOR_ALL_DATA_IN_SLIST(_list, _node, _data) \
  for (_node = ZOS_SLIST_HEAD_NODE(_list), \
       _data = ZOS_SLIST_GET_DATA(_node); \
       _node && _data; _node = (_node)->pstNext, \
       _data = ZOS_SLIST_GET_DATA(_node))

/** @brief Enumerate all node and data in list, safe for remove node. */
#define FOR_ALL_DATA_IN_SLIST_NEXT(_list, _node, _nextnode, _data) \
  for (_node = ZOS_SLIST_HEAD_NODE(_list), \
       _data = ZOS_SLIST_GET_DATA(_node), \
       _nextnode = ZOS_SLIST_NEXT_NODE(_node); \
       _node && _data; _node = _nextnode, \
       _nextnode = ZOS_SLIST_NEXT_NODE(_node), \
       _data = ZOS_SLIST_GET_DATA(_node))
       
/** @brief Enumerate all node(as data) in list. */ 
#define FOR_ALL_NDATA_IN_SLIST(_list, _node, _data) \
  for (_node = ZOS_SLIST_HEAD_NODE(_list), \
       _data = (ZVOID *)(_node); \
       _node; _node = (_node)->pstNext, \
       _data = (ZVOID *)(_node))

/** @brief Enumerate all node(as data) in list, safe for remove node. */ 
#define FOR_ALL_NDATA_IN_SLIST_NEXT(_list, _node, _nextnode, _data) \
  for (_node = ZOS_SLIST_HEAD_NODE(_list), \
       _data = (ZVOID *)(_node), \
       _nextnode = ZOS_SLIST_NEXT_NODE(_node); \
       _node && _data; _node = _nextnode, \
       _nextnode = ZOS_SLIST_NEXT_NODE(_node), \
       _data = (ZVOID *)(_node))

/**
 * @brief Create a list.
 *
 * @see Zos_SlistCreate.
 */
#define ZOS_SLIST_CREATE(_list, _size) \
    Zos_SlistCreate(_list, _size)

/**
 * @brief Create a list.
 *
 * @see Zos_SlistCreate.
 */
#define ZOS_SLIST_CREATEX(_list) \
    Zos_SlistCreate(_list, ZSLIST_INFINITE_SIZE)

/**
 * @brief Delete a list.
 *
 * @see Zos_SlistDelete.
 */
#define ZOS_SLIST_DELETE(_list) \
    Zos_SlistDelete(_list)

/** @brief Init list node. */
#define ZOS_SLIST_NODE_INIT(_node, _data) do { \
    (_node)->pstNext = ZNULL; \
    (_node)->pData = (ZCHAR *)(_data); \
} while (0)

/**
 * @brief Insert a node.
 *
 * @see Zos_SlistInsert.
 *
 */
#define Zos_SlistAdd2Head(_list, _node) \
    Zos_SlistInsert(_list, ZNULL, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_SlistInsert.
 *
 */
#define Zos_SlistAdd2Tail(_list, _node) \
    Zos_SlistInsert(_list, (_list)->pstTail, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_SlistInsert.
 *
 */
#define ZOS_SLIST_ADD2HEAD(_list, _node) \
    Zos_SlistInsert(_list, ZNULL, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_SlistInsert.
 *
 */
#define ZOS_SLIST_ADD2TAIL(_list, _node) \
    Zos_SlistInsert(_list, (_list)->pstTail, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_SlistInsert.
 *
 */
#define ZOS_SLIST_INSERT(_list, _prevnode, _node) \
    Zos_SlistInsert(_list, _prevnode, _node)

/**
 * @brief Dequeue a node.
 *
 * @see Zos_SlistDequeue.
 *
 */
#define ZOS_SLIST_DEQUEUE(_list, _node) \
    _node = Zos_SlistDequeue(_list)

/**
 * @brief Remove a node.
 *
 * @see Zos_SlistRemove.
 *
 */
#define ZOS_SLIST_REMOVE(_list, _node) \
    Zos_SlistRemove(_list, _node)

/**
 * @brief Find a node by index.
 *
 * @see Zos_SlistFindByIndex.
 *
 */
#define ZOS_SLIST_FIND_BY_INDEX(_list, _index, _node) \
    _node = Zos_SlistFindByIndex(_list, _index)


/** @brief Typedef slist with specific name. */
#define ZOS_TYPEDEF_SLIST(_name) \
    /* single list node */ \
    typedef struct tag##_name##_LST_NODE \
    { \
        struct tag##_name##_LST_NODE *pstNext; /**< @brief next slist node */ \
        ST_##_name *pData;           /**< @brief slist node data */ \
    } ST_##_name##_LST_NODE; \
    /* single list */ \
    typedef struct tag##_name##_LST \
    { \
        ZUINT iMaxNum;               /**< @brief maximum number of slist nodes */ \
        ZUINT iCount;                /**< @brief actual count of slist nodes */ \
        ST_##_name##_LST_NODE *pstHead; /**< @brief slist node head */ \
        ST_##_name##_LST_NODE *pstTail; /**< @brief slist node tail */ \
    } ST_##_name##_LST

/** @brief It gets the actual count of list nodes. */
#define ZOS_TYPEDEF_SLIST_SIZE(_list)  \
    ZOS_SLIST_SIZE((ST_ZOS_SLIST *)(_list))

/** @brief It checks the full of list. */
#define ZOS_TYPEDEF_SLIST_ISFULL(_list)  \
    ZOS_SLIST_ISFULL((ST_ZOS_SLIST *)(_list))

/** @brief It checks for list is empty. */
#define ZOS_TYPEDEF_SLIST_ISEMPTY(_list) \
    ZOS_SLIST_ISEMPTY((ST_ZOS_SLIST *)(_list))

/** @brief It checks for list is not empty. */
#define ZOS_TYPEDEF_SLIST_ISNOEMPTY(_list) \
    ZOS_SLIST_ISNOEMPTY((ST_ZOS_SLIST *)(_list))

/** @brief It gets the head node of list. */
#define ZOS_TYPEDEF_SLIST_HEAD_NODE(_list) \
    ZOS_SLIST_HEAD_NODE((ST_ZOS_SLIST *)(_list))

/** @brief It gets the tail node of list. */
#define ZOS_TYPEDEF_SLIST_TAIL_NODE(_list) \
    ZOS_SLIST_TAIL_NODE((ST_ZOS_SLIST *)(_list))

/** @brief It gets the next node or gets ZNULL. */
#define ZOS_TYPEDEF_SLIST_NEXT_NODE(_node) \
    ZOS_SLIST_NEXT_NODE((_node))

/** @brief It gets the node data or gets ZNULL. */
#define ZOS_TYPEDEF_SLIST_GET_DATA(_node) \
    ZOS_SLIST_GET_DATA((_node))

/** @brief It transforms node to "node->pData". */
#define ZOS_TYPEDEF_SLIST_SET_DATA(_node) \
    ZOS_SLIST_SET_DATA(_node)

#define ZOS_TYPEDEF_SLIST_HEAD_DATA(_list) \
    ZOS_SLIST_HEAD_DATA((ST_ZOS_SLIST *)(_list))

/** @brief It gets pData, or get ZNULL. */
#define ZOS_TYPEDEF_SLIST_TAIL_DATA(_list) \
    ZOS_SLIST_TAIL_DATA((ST_ZOS_SLIST *)(_list))

/** @brief It gets next node pData, or get ZNULL. */
#define ZOS_TYPEDEF_SLIST_NEXT_DATA(_node) \
    ZOS_SLIST_NEXT_DATA((ST_ZOS_SLIST_NODE *)(_node))

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_NODE_IN_SLIST.
 */
#define FOR_ALL_NODE_IN_TYPEDEF_SLIST(_list, _node) \
    FOR_ALL_NODE_IN_SLIST((ST_ZOS_SLIST *)(_list), _node)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_NODE_IN_SLIST_NEXT.
 */
#define FOR_ALL_NODE_IN_TYPEDEF_SLIST_NEXT(_list, _node, _nextnode) \
    FOR_ALL_NODE_IN_SLIST_NEXT((ST_ZOS_SLIST *)(_list), _node, _nextnode)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_DATA_IN_SLIST.
 */
#define FOR_ALL_DATA_IN_TYPEDEF_SLIST(_list, _node, _data) \
    FOR_ALL_DATA_IN_SLIST((ST_ZOS_SLIST *)(_list), _node, _data)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_DATA_IN_SLIST.
 */
#define FOR_ALL_DATA_IN_TYPEDEF_SLIST_NEXT(_list, _node, _nextnode, _data) \
    FOR_ALL_DATA_IN_SLIST_NEXT((ST_ZOS_SLIST *)(_list), _node, _nextnode, _data)

/**
 * @brief Create a list.
 *
 * @see ZOS_SLIST_CREATE.
 */
#define ZOS_TYPEDEF_SLIST_CREATE(_list, _size) \
    ZOS_SLIST_CREATE((ST_ZOS_SLIST *)(_list), _size)

/**
 * @brief Create a list.
 *
 * @see ZOS_SLIST_CREATE.
 */
#define ZOS_TYPEDEF_SLIST_CREATEX(_list) \
    ZOS_SLIST_CREATE((ST_ZOS_SLIST *)(_list), ZSLIST_INFINITE_SIZE)

/**
 * @brief Delete a list.
 *
 * @see ZOS_SLIST_DELETE.
 */
#define ZOS_TYPEDEF_SLIST_DELETE(_list) \
    ZOS_SLIST_DELETE((ST_ZOS_SLIST *)(_list))

/** @brief Init node data. */
#define ZOS_TYPEDEF_SLIST_NODE_INIT(_node, _data) \
    ZOS_SLIST_NODE_INIT((ST_ZOS_SLIST_NODE *)(_node), _data)

/**
 * @brief Insert a node.
 *
 * @see ZOS_SLIST_ADD2HEAD.
 *
 */
#define ZOS_TYPEDEF_SLIST_ADD2HEAD(_list, _node) \
    ZOS_SLIST_ADD2HEAD((ST_ZOS_SLIST *)(_list), _node)

/**
 * @brief Insert a node.
 *
 * @see ZOS_SLIST_ADD2TAIL.
 *
 */
#define ZOS_TYPEDEF_SLIST_ADD2TAIL(_list, _node) \
    ZOS_SLIST_ADD2TAIL((ST_ZOS_SLIST *)(_list), _node)

/**
 * @brief Insert a node.
 *
 * @see ZOS_SLIST_INSERT.
 *
 */
#define ZOS_TYPEDEF_SLIST_INSERT(_list, _prevnode, _node) \
    ZOS_SLIST_INSERT((ST_ZOS_SLIST *)(_list), _prevnode, _node)

/**
 * @brief Dequeue a node.
 *
 * @see ZOS_SLIST_DEQUEUE.
 *
 */
#define ZOS_TYPEDEF_SLIST_DEQUEUE(_list, _node) \
    ZOS_SLIST_DEQUEUE((ST_ZOS_SLIST *)(_list), _node)

/**
 * @brief Remove a node.
 *
 * @see Zos_SlistRemove.
 *
 */
#define ZOS_TYPEDEF_SLIST_REMOVE(_list, _node) \
    Zos_SlistRemove((ST_ZOS_SLIST *)(_list), (_node))

/**
 * @brief Finds a node by its index number.
 *
 * @see ZOS_SLIST_FIND_BY_INDEX.
 *
 */
#define ZOS_TYPEDEF_SLIST_FIND_BY_INDEX(_list, _index, _node) \
    ZOS_SLIST_FIND_BY_INDEX((ST_ZOS_SLIST *)_list, _index, _node)

/** @brief zos slist node */
typedef struct tagZOS_SLIST_NODE
{
    struct tagZOS_SLIST_NODE *pstNext; /**< @brief next slist node */
    ZVOID *pData;                    /**< @brief slist node data */
} ST_ZOS_SLIST_NODE;

/** @brief zos slist node without data */
typedef struct tagZOS_SLIST_NODEX
{
    struct tagZOS_SLIST_NODE *pstNext; /**< @brief next slist node */
} ST_ZOS_SLIST_NODEX;

/** @brief zos slist */
typedef struct tagZOS_SLIST
{
    ZUINT iMaxNum;                   /**< @brief maximum number of slist nodes */
    ZUINT iCount;                    /**< @brief actual count of slist nodes */
    ST_ZOS_SLIST_NODE *pstHead;      /**< @brief slist node head */
    ST_ZOS_SLIST_NODE *pstTail;      /**< @brief slist node tail */
} ST_ZOS_SLIST;

/**
 * @brief Creates a dlist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 * @param [in] iMaxNum The maximum number of nodes in the dlist.
 *
 * @retval ZOK      Create successfully.
 * @retval ZFAILED  Create failed.
 *
 */
ZFUNC ZINT Zos_SlistCreate(ST_ZOS_SLIST *pstList, ZUINT iMaxNum);

/**
 * @brief Delete a dlist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 *
 * @return [none].
 *
 */
ZFUNC ZVOID Zos_SlistDelete(ST_ZOS_SLIST *pstList);

/**
 * @brief Insert one dlist node to the after the previous node of this dlist 
 *        if pstPrevNode is ZNULL, then node will insert before the head as 
 *        the new head.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 * @param [in] pPrevNode The node which shall be previous to the new 
 *                       node in the dlist.
 * @param [in] pNode The new node.
 *
 * @retval ZOK      Successfully.
 * @retval ZFAILED  Failed.
 */
ZFUNC ZINT Zos_SlistInsert(ST_ZOS_SLIST *pstList, ZVOID *pPrevNode, ZVOID *pNode);

/**
 * @brief Dequeue one node from dlist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 *
 * @return A pointer to the node, or return ZNULL on failure.
 */
ZFUNC ZVOID * Zos_SlistDequeue(ST_ZOS_SLIST *pstList);

/**
 * @brief Remove one dlist node from this dlist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 * @param [in] pNode The node which is to be removed.
 *
 * @retval ZOK      Successfully.
 * @retval ZFAILED  Failed.
 */
ZFUNC ZINT Zos_SlistRemove(ST_ZOS_SLIST *pstList, ZVOID *pNode);

/**
 * @brief Removes a node following a certain node pointed 
 *        by pPrevNode from an slist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 * @param [in] pPrevNode The pointer to a node followed by the node which
 *                       will be removed.
 * @param [in] pNode The node which is to be removed.
 *
 * @retval ZOK      Successfully.
 * @retval ZFAILED  Failed.
 */
ZFUNC ZINT Zos_SlistRemoveAfter(ST_ZOS_SLIST *pstList,
                ZVOID *pPrevNode, ZVOID *pNode);

/**
 * @brief Find node by the index number.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 * @param [in] iIndex The index of the node.
 *
 * @return A pointer to the node, or return ZNULL on failure.
 */
ZFUNC ZVOID * Zos_SlistFindByIndex(ST_ZOS_SLIST *pstList, ZUINT iIndex);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_SLIST_H__ */

