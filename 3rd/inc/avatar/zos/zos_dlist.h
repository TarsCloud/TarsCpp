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
  File name     : zos_dlist.h
  Module        : zos double list library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-04-07    
  Description   :
      Data structure and function definitions required 
      by the double list interface
    
  Modify History:   
  1. Date:        Author:         Modification:
  2005-02-04      leo.lv          create the file 

*************************************************/
#ifndef _ZOS_DLIST_H__
#define _ZOS_DLIST_H__

/** 
 * @file zos_dlist.h
 * @brief Zos double list interfaces.
 *  
 * In this file, it defined the operation function
 * of zos double list, including create, delete, insert, remove
 * and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief List maximum node size(infinite) */
#define ZDLIST_INFINITE_SIZE ZMAXUINT

/** @brief Get the actual count of list nodes. */
#define ZOS_DLIST_SIZE(_list) \
    ((_list)->iCount)

/** @brief Check the full of list. */
#define ZOS_DLIST_ISFULL(_list) \
    ((_list)->iMaxNum && (_list)->iCount >= (_list)->iMaxNum)  

/** @brief Check for list is empty. */
#define ZOS_DLIST_ISEMPTY(_list) \
    ((_list)->pstHead == ZNULL)

/** @brief Check for list is not empty. */
#define ZOS_DLIST_ISNOEMPTY(_list) \
    ((_list)->pstHead != ZNULL)
    
/** @brief Get the head node of list. */
#define ZOS_DLIST_HEAD_NODE(_list) (_list)->pstHead

/** @brief Get the tail node tail of list. */
#define ZOS_DLIST_TAIL_NODE(_list) (_list)->pstTail

/** @brief If node is not ZNULL get previou element, or get ZNULL. */
#define ZOS_DLIST_PREV_NODE(_node) ((_node) ? (_node)->pstPrev : ZNULL)

/** @brief If node is not ZNULL get next node, or get ZNULL. */
#define ZOS_DLIST_NEXT_NODE(_node) ((_node) ? (_node)->pstNext : ZNULL)

/** @brief Get node data or ZNULL. */
#define ZOS_DLIST_GET_DATA(_node)  ((_node) ? (_node)->pData : ZNULL)

/** @brief Transform node to node->pData. */
#define ZOS_DLIST_SET_DATA(_node)  (_node)->pData

/** @brief If the list head is not ZNULL get it's pDate, or get ZNULL. */
#define ZOS_DLIST_HEAD_DATA(_list) \
    ((ZOS_DLIST_HEAD_NODE(_list)) ? ZOS_DLIST_HEAD_NODE(_list)->pData : ZNULL)

/** @brief If the list tail is not ZNULL get it's pDate, or get ZNULL. */
#define ZOS_DLIST_TAIL_DATA(_list) \
    ((ZOS_DLIST_TAIL_NODE(_list)) ? ZOS_DLIST_TAIL_NODE(_list)->pData : ZNULL)

/** @brief If the next node is not ZNULL get it's pDate, or get ZNULL. */
#define ZOS_DLIST_NEXT_DATA(_node) \
    ((ZOS_DLIST_NEXT_NODE(_node)) ? ZOS_DLIST_NEXT_NODE(_node)->pData : ZNULL)

/** @brief If the previou node is not ZNULL get it's pDate, or get ZNULL. */
#define ZOS_DLIST_PREV_DATA(_node) \
    ((ZOS_DLIST_PREV_NODE(_node)) ? ZOS_DLIST_PREV_NODE(_node)->pData : ZNULL)

/** @brief Enumerate all node in list. */ 
#define FOR_ALL_NODE_IN_DLIST(_list, _node) \
  for (_node = ZOS_DLIST_HEAD_NODE(_list); \
       _node; _node = (_node)->pstNext)

/** @brief Enumerate all node in list backward. */ 
#define FOR_ALL_NODE_IN_DLISTX(_list, _node) \
  for (_node = ZOS_DLIST_TAIL_NODE(_list); \
       _node; _node = (_node)->pstPrev)

/** @brief Enumerate all node in list, safe for remove node. */ 
#define FOR_ALL_NODE_IN_DLIST_NEXT(_list, _node, _nextnode) \
  for (_node = ZOS_DLIST_HEAD_NODE(_list), \
       _nextnode = ZOS_DLIST_NEXT_NODE(_node); \
       _node; _node = _nextnode, \
       _nextnode = ZOS_DLIST_NEXT_NODE(_node))

/** @brief Enumerate all node in list backward, safe for remove node. */ 
#define FOR_ALL_NODE_IN_DLIST_PREV(_list, _node, _prevnode) \
  for (_node = ZOS_DLIST_TAIL_NODE(_list), \
       _prevnode = ZOS_DLIST_PREV_NODE(_node); \
       _node; _node = _prevnode, \
       _prevnode = ZOS_DLIST_PREV_NODE(_node))

/** @brief Enumerate all node and data in list. */ 
#define FOR_ALL_DATA_IN_DLIST(_list, _node, _data) \
  for (_node = ZOS_DLIST_HEAD_NODE(_list), \
       _data = ZOS_DLIST_GET_DATA(_node); \
       _node && _data; _node = (_node)->pstNext, \
       _data = ZOS_DLIST_GET_DATA(_node))
       
/** @brief Enumerate all node and data in list backward. */ 
#define FOR_ALL_DATA_IN_DLISTX(_list, _node, _data) \
  for (_node = ZOS_DLIST_TAIL_NODE(_list), \
       _data = ZOS_DLIST_GET_DATA(_node); \
       _node && _data; _node = (_node)->pstPrev, \
       _data = ZOS_DLIST_GET_DATA(_node))

/** @brief Enumerate all node and data in list, safe for remove node. */ 
#define FOR_ALL_DATA_IN_DLIST_NEXT(_list, _node, _nextnode, _data) \
  for (_node = ZOS_DLIST_HEAD_NODE(_list), \
       _data = ZOS_DLIST_GET_DATA(_node), \
       _nextnode = ZOS_DLIST_NEXT_NODE(_node); \
       _node && _data; _node = _nextnode, \
       _nextnode = ZOS_DLIST_NEXT_NODE(_node), \
       _data = ZOS_DLIST_GET_DATA(_node))

/** @brief Enumerate all node and data in list backward, safe for remove node. */ 
#define FOR_ALL_DATA_IN_DLIST_PREV(_list, _node, _prevnode, _data) \
  for (_node = ZOS_DLIST_TAIL_NODE(_list), \
       _data = ZOS_DLIST_GET_DATA(_node), \
       _prevnode = ZOS_DLIST_PREV_NODE(_node); \
       _node && _data; _node = _prevnode, \
       _prevnode = ZOS_DLIST_PREV_NODE(_node), \
       _data = ZOS_DLIST_GET_DATA(_node))

/** @brief Enumerate all node(as data) in list. */ 
#define FOR_ALL_NDATA_IN_DLIST(_list, _node, _data) \
  for (_node = ZOS_DLIST_HEAD_NODE(_list), \
       _data = (ZVOID *)(_node); \
       _node; _node = (_node)->pstNext, \
       _data = (ZVOID *)(_node))

/** @brief Enumerate all node(as data) in list, safe for remove node. */ 
#define FOR_ALL_NDATA_IN_DLIST_NEXT(_list, _node, _nextnode, _data) \
  for (_node = ZOS_DLIST_HEAD_NODE(_list), \
       _data = (ZVOID *)(_node), \
       _nextnode = ZOS_DLIST_NEXT_NODE(_node); \
       _node && _data; _node = _nextnode, \
       _nextnode = ZOS_DLIST_NEXT_NODE(_node), \
       _data = (ZVOID *)(_node))

/**
 * @brief Create a list.
 *
 * @see Zos_DlistCreate.
 */
#define ZOS_DLIST_CREATE(_list, _size) \
    Zos_DlistCreate(_list, _size)

/**
 * @brief Create a list.
 *
 * @see Zos_DlistCreate.
 */
#define ZOS_DLIST_CREATEX(_list) \
    Zos_DlistCreate(_list, ZDLIST_INFINITE_SIZE)

/**
 * @brief delete a list.
 *
 * @see Zos_DlistDelete.
 */
#define ZOS_DLIST_DELETE(_list) \
    Zos_DlistDelete(_list)

/** @brief Init a node. */
#define ZOS_DLIST_NODE_INIT(_node, _data) do { \
    (_node)->pstPrev = (_node)->pstNext = ZNULL; \
    (_node)->pData = (ZCHAR *)(_data); \
} while (0)

/**
 * @brief Insert a node.
 *
 * @see Zos_DlistInsert.
 */
#define Zos_DlistAdd2Head(_list, _node) \
    Zos_DlistInsert(_list, ZNULL, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_DlistInsert.
 */
#define Zos_DlistAdd2Tail(_list, _node) \
    Zos_DlistInsert(_list, (_list)->pstTail, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_DlistInsert.
 */
#define ZOS_DLIST_ADD2HEAD(_list, _node) \
    Zos_DlistInsert(_list, ZNULL, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_DlistInsert.
 */
#define ZOS_DLIST_ADD2TAIL(_list, _node) \
    Zos_DlistInsert(_list, (_list)->pstTail, _node)

/**
 * @brief Insert a node.
 *
 * @see Zos_DlistInsert.
 */
#define ZOS_DLIST_INSERT(_list, _prevnode, _node) \
    Zos_DlistInsert(_list, _prevnode, _node)

/**
 * @brief Dequeue a node.
 *
 * @see Zos_DlistDequeue.
 */
/*lint -save -e(773) */
#define ZOS_DLIST_DEQUEUE(_list, _node) \
    _node = Zos_DlistDequeue(_list)
/*lint -restore */

/**
 * @brief Remove a node.
 *
 * @see Zos_DlistRemove.
 */
#define ZOS_DLIST_REMOVE(_list, _node) \
    Zos_DlistRemove(_list, _node)

/**
 * @brief Finds a node by its index number.
 *
 * @see Zos_DlistFindByIndex.
 */
/*lint -save -e(773) */
#define ZOS_DLIST_FIND_BY_INDEX(_list, _index, _node) \
    _node = Zos_DlistFindByIndex(_list, _index)    
/*lint -restore */

/**
 * @brief Finds a node data by its index number.
 *
 * @see Zos_DlistFindByIndex.
 */
#define ZOS_DLIST_DATA_FIND_BY_INDEX(_list, _index, _node, _data) do { \
    _node = Zos_DlistFindByIndex(_list, _index); \
    _data = ZOS_DLIST_GET_DATA(_node); \
} while (0)
    
/** @brief Typedef dlist with specific name. */
#define ZOS_TYPEDEF_DLIST(_name) \
    /* double list node */ \
    typedef struct tag##_name##_LST_NODE \
    { \
        struct tag##_name##_LST_NODE *pstNext; /**< @brief next dlist node */ \
        struct tag##_name##_LST_NODE *pstPrev; /**< @brief previous dlist node */ \
        ST_##_name *pData;           /**< @brief dlist node data */ \
    } ST_##_name##_LST_NODE; \
    /* double list */ \
    typedef struct tag##_name##_LST \
    { \
        ZUINT iMaxNum;             /**< @brief maximum number of dlist nodes */ \
        ZUINT iCount;              /**< @brief actual count of dlist nodes */ \
        ST_##_name##_LST_NODE *pstHead; /**< @brief dlist node head */ \
        ST_##_name##_LST_NODE *pstTail; /**< @brief dlist node tail */ \
    } ST_##_name##_LST

/** @brief It gets the actual count of list nodes. */
#define ZOS_TYPEDEF_DLIST_SIZE(_list)  \
    ZOS_DLIST_SIZE((ST_ZOS_DLIST *)(_list))
    
/** @brief It checks the full of list. */    
#define ZOS_TYPEDEF_DLIST_ISFULL(_list)  \
    ZOS_DLIST_ISFULL((ST_ZOS_DLIST *)(_list))

/** @brief It checks for list is empty. */ 
#define ZOS_TYPEDEF_DLIST_ISEMPTY(_list) \
    ZOS_DLIST_ISEMPTY((ST_ZOS_DLIST *)(_list))

/** @brief It checks for list is not empty. */ 
#define ZOS_TYPEDEF_DLIST_ISNOEMPTY(_list) \
    ZOS_DLIST_ISNOEMPTY((ST_ZOS_DLIST *)(_list))

/** @brief It gets the head node of list. */ 
#define ZOS_TYPEDEF_DLIST_HEAD_NODE(_list) \
    ZOS_DLIST_HEAD_NODE((ST_ZOS_DLIST *)(_list))

/** @brief It gets the tail node of list. */ 
#define ZOS_TYPEDEF_DLIST_TAIL_NODE(_list) \
    ZOS_DLIST_TAIL_NODE((ST_ZOS_DLIST *)(_list))

/** @brief It gets the previou node of list or gets ZNULL.
 */ 
#define ZOS_TYPEDEF_DLIST_PREV_NODE(_node) \
    ZOS_DLIST_PREV_NODE(_node)

/** @brief It gets the next node or gets ZNULL. */
#define ZOS_TYPEDEF_DLIST_NEXT_NODE(_node) \
    ZOS_DLIST_NEXT_NODE(_node)
    
/** @brief It gets the node data or gets ZNULL. */
#define ZOS_TYPEDEF_DLIST_GET_DATA(_node) \
    ZOS_DLIST_GET_DATA(_node)

/** @brief It transforms node to "node->pData". */
#define ZOS_TYPEDEF_DLIST_SET_DATA(_node) \
    ZOS_DLIST_SET_DATA(_node)

/** @brief It gets pData, or get ZNULL. */
#define ZOS_TYPEDEF_DLIST_HEAD_DATA(_list) \
    ZOS_DLIST_HEAD_DATA((ST_ZOS_DLIST *)(_list))

/** @brief It gets pData, or get ZNULL. */
#define ZOS_TYPEDEF_DLIST_TAIL_DATA(_list) \
    ZOS_DLIST_TAIL_DATA((ST_ZOS_DLIST *)(_list))

/** @brief It gets previou node pData, or get ZNULL. */
#define ZOS_TYPEDEF_DLIST_PREV_DATA(_node) \
    ZOS_DLIST_PREV_DATA((ST_ZOS_DLIST_NODE *)(_node))

/** @brief It gets next node pData, or get ZNULL. */
#define ZOS_TYPEDEF_DLIST_NEXT_DATA(_node) \
    ZOS_DLIST_NEXT_DATA((ST_ZOS_DLIST_NODE *)(_node))

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_NODE_IN_DLIST.
 */
#define FOR_ALL_NODE_IN_TYPEDEF_DLIST(_list, _node) \
    FOR_ALL_NODE_IN_DLIST((ST_ZOS_DLIST *)(_list), _node)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_NODE_IN_DLISTX.
 */
#define FOR_ALL_NODE_IN_TYPEDEF_DLISTX(_list, _node) \
    FOR_ALL_NODE_IN_DLISTX((ST_ZOS_DLIST *)(_list), _node)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_NODE_IN_DLIST_NEXT.
 */
#define FOR_ALL_NODE_IN_TYPEDEF_DLIST_NEXT(_list, _node, _nextnode) \
    FOR_ALL_NODE_IN_DLIST_NEXT((ST_ZOS_DLIST *)(_list), _node, _nextnode)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_NODE_IN_DLIST_PREV.
 */
#define FOR_ALL_NODE_IN_TYPEDEF_DLIST_PREV(_list, _node, _prevnode) \
    FOR_ALL_NODE_IN_DLIST_PREV((ST_ZOS_DLIST *)(_list), _node, _prevnode)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_DATA_IN_DLIST.
 */
#define FOR_ALL_DATA_IN_TYPEDEF_DLIST(_list, _node, _data) \
    FOR_ALL_DATA_IN_DLIST((ST_ZOS_DLIST *)(_list), _node, _data)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_DATA_IN_DLISTX.
 */
#define FOR_ALL_DATA_IN_TYPEDEF_DLISTX(_list, _node, _data) \
    FOR_ALL_DATA_IN_DLISTX((ST_ZOS_DLIST *)(_list), _node, _data)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_DATA_IN_DLIST_NEXT.
 */
#define FOR_ALL_DATA_IN_TYPEDEF_DLIST_NEXT(_list, _node, _nextnode, _data) \
    FOR_ALL_DATA_IN_DLIST_NEXT((ST_ZOS_DLIST *)(_list), _node, _nextnode, _data)

/**
 * @brief Enumerate all node in list.
 *
 * @see FOR_ALL_DATA_IN_DLIST_PREV.
 */
#define FOR_ALL_DATA_IN_TYPEDEF_DLIST_PREV(_list, _node, _prevnode, _data) \
    FOR_ALL_DATA_IN_DLIST_PREV((ST_ZOS_DLIST *)(_list), _node, _prevnode, _data)

/**
 * @brief Create a list.
 *
 * @see ZOS_DLIST_CREATE.
 */
#define ZOS_TYPEDEF_DLIST_CREATE(_list, _size) \
    ZOS_DLIST_CREATE((ST_ZOS_DLIST *)(_list), _size)

/**
 * @brief Create a list.
 *
 * @see ZOS_DLIST_CREATE.
 */
#define ZOS_TYPEDEF_DLIST_CREATEX(_list) \
    ZOS_DLIST_CREATEX((ST_ZOS_DLIST *)(_list))

/**
 * @brief Delete a list.
 *
 * @see ZOS_DLIST_DELETE.
 */
#define ZOS_TYPEDEF_DLIST_DELETE(_list) \
    ZOS_DLIST_DELETE((ST_ZOS_DLIST *)(_list))

/** @brief Init node data. */
#define ZOS_TYPEDEF_DLIST_NODE_INIT(_node, _data) \
    ZOS_DLIST_NODE_INIT((ST_ZOS_DLIST_NODE *)_node, _data)

/**
 * @brief Insert a node.
 *
 * @see ZOS_DLIST_ADD2HEAD.
 *
 */
#define ZOS_TYPEDEF_DLIST_ADD2HEAD(_list, _node) \
    ZOS_DLIST_ADD2HEAD((ST_ZOS_DLIST *)(_list), _node)

/**
 * @brief Insert a node.
 *
 * @see ZOS_DLIST_ADD2TAIL.
 *
 */
#define ZOS_TYPEDEF_DLIST_ADD2TAIL(_list, _node) \
    ZOS_DLIST_ADD2TAIL((ST_ZOS_DLIST *)(_list), _node)

/**
 * @brief Insert a node.
 *
 * @see ZOS_DLIST_INSERT.
 *
 */
#define ZOS_TYPEDEF_DLIST_INSERT(_list, _prevnode, _node) \
    ZOS_DLIST_INSERT((ST_ZOS_DLIST *)(_list), _prevnode, _node)

/**
 * @brief Dequeue a node.
 *
 * @see ZOS_DLIST_DEQUEUE.
 *
 */
#define ZOS_TYPEDEF_DLIST_DEQUEUE(_list, _node) \
    ZOS_DLIST_DEQUEUE((ST_ZOS_DLIST *)(_list), _node)

/**
 * @brief Remove a node.
 *
 * @see ZOS_DLIST_REMOVE.
 *
 */
#define ZOS_TYPEDEF_DLIST_REMOVE(_list, _node) \
    ZOS_DLIST_REMOVE((ST_ZOS_DLIST *)(_list), _node)

/**
 * @brief Finds a node by its index number.
 *
 * @see ZOS_DLIST_FIND_BY_INDEX.
 *
 */
#define ZOS_TYPEDEF_DLIST_FIND_BY_INDEX(_list, _index, _node) \
    ZOS_DLIST_FIND_BY_INDEX((ST_ZOS_DLIST *)_list, _index, _node)

/**
 * @brief Finds a node data sby its index number.
 *
 * @see ZOS_DLIST_FIND_BY_INDEX.
 *
 */
#define ZOS_TYPEDEF_DLIST_DATA_FIND_BY_INDEX(_list, _index, _node, _data) do { \
    ZOS_DLIST_FIND_BY_INDEX((ST_ZOS_DLIST *)_list, _index, _node); \
    _data = ZOS_DLIST_GET_DATA(_node); \
} while (0)

/** @brief List node. */
typedef struct tagZOS_DLIST_NODE
{
    struct tagZOS_DLIST_NODE *pstNext; /**< @brief next dlist node */
    struct tagZOS_DLIST_NODE *pstPrev; /**< @brief previous dlist node */
    ZVOID *pData;                    /**< @brief dlist node data */
} ST_ZOS_DLIST_NODE;

/** @brief List node without data. */
typedef struct tagZOS_DLIST_NODEX
{
    struct tagZOS_DLIST_NODE *pstNext; /**< @brief next dlist node */
    struct tagZOS_DLIST_NODE *pstPrev; /**< @brief previous dlist node */
} ST_ZOS_DLIST_NODEX;

/** @brief List. */
typedef struct tagZOS_DLIST
{
    ZUINT iMaxNum;                   /**< @brief maximum number of dlist nodes */
    ZUINT iCount;                    /**< @brief actual count of dlist nodes */
    ST_ZOS_DLIST_NODE *pstHead;      /**< @brief dlist node head */
    ST_ZOS_DLIST_NODE *pstTail;      /**< @brief dlist node tail */
} ST_ZOS_DLIST;

/** @brief Structure string dlist node */
typedef struct tagZOS_SSTR_DLST_NODE
{
    struct tagZOS_SSTR_DLST_NODE *pstNext; /**< @brief next dlist node */
    struct tagZOS_SSTR_DLST_NODE *pstPrev; /**< @brief previous dlist node */
    ST_ZOS_SSTR *pstData;            /**< @brief dlist node data */
} ST_ZOS_SSTR_DLST_NODE;

/** @brief Structure string dlist */
typedef struct tagZOS_SSTR_DLST
{
    ZUINT iMaxNum;                   /**< @brief maximum number of dlist nodes */
    ZUINT iCount;                    /**< @brief actual count of dlist nodes */
    ST_ZOS_SSTR_DLST_NODE *pstHead;  /**< @brief dlist node head */
    ST_ZOS_SSTR_DLST_NODE *pstTail;  /**< @brief dlist node tail */
} ST_ZOS_SSTR_DLST;

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
ZFUNC ZINT Zos_DlistCreate(ST_ZOS_DLIST *pstList, ZUINT iMaxNum);

/**
 * @brief Delete a dlist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 *
 * @return [none].
 *
 */
ZFUNC ZVOID Zos_DlistDelete(ST_ZOS_DLIST *pstList);

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
ZFUNC ZINT Zos_DlistInsert(ST_ZOS_DLIST *pstList, ZVOID *pPrevNode, ZVOID *pNode);

/**
 * @brief Dequeue one node from dlist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 *
 * @return A pointer to the node, or return ZNULL on failure.
 */
ZFUNC ZVOID * Zos_DlistDequeue(ST_ZOS_DLIST *pstList);

/**
 * @brief Remove one dlist node from this dlist.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 * @param [in] pNode The node which is to be removed.
 *
 * @retval ZOK      Successfully.
 * @retval ZFAILED  Failed.
 */
ZFUNC ZINT Zos_DlistRemove(ST_ZOS_DLIST *pstList, ZVOID *pNode);

 /**
 * @brief Find node by the index number.
 *
 * @param [in] pstList A pointer to ZOS dlist structure.
 * @param [in] iIndex The index of the node.
 *
 * @return A pointer to the node, or return ZNULL on failure.
 */
ZFUNC ZVOID * Zos_DlistFindByIndex(ST_ZOS_DLIST *pstList, ZUINT iIndex);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_DLIST_H__ */

