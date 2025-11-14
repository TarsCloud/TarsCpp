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
  File name     : zos_rbtree.h
  Module        : zos rbtree tree library functions
  Author        : zocol  
  Version       : 0.1   
  Created on    : 2014-01-02    
  Description   :
      Data structure and function definitions required by the rbtree interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_RBT_H__
#define _ZOS_RBT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* zos red-black tree id */
typedef ZVOID * ZRBTID;

/* zos red-black tree key */
typedef ZCOOKIE ZRBTKEY;

/* zos red-black tree value */
typedef ZCOOKIE ZRBTVAL;

/* zos red-black tree node */
typedef ZVOID * ZRBTNODE;

/* zos red-black tree traverse mode */
typedef enum EN_ZOS_RBT_TRAVERSE_TYPE
{
    EN_ZOS_RBT_TRAVERSE_PREORDER,      /* pre-order traverse */
    EN_ZOS_RBT_TRAVERSE_INORDER,       /* in-order traverse */
    EN_ZOS_RBT_TRAVERSE_POSTORDER      /* post-order traverse */
} EN_ZOS_RBT_TRAVERSE_TYPE;

/* zos red-balck tree traverse */
#define FOR_ALL_NODE_IN_RBT_PREODER(_rbtid, _node) \
    for (_node = Zos_RbtHead(_rbtid, EN_ZOS_RBT_TRAVERSE_PREORDER); \
        _node; _node = Zos_RbtNext(_rbtid, _node, EN_ZOS_RBT_TRAVERSE_PREORDER))
        
/* zos red-balck tree traverse */
#define FOR_ALL_NODE_IN_RBT_INODER(_rbtid, _node) \
    for (_node = Zos_RbtHead(_rbtid, EN_ZOS_RBT_TRAVERSE_INORDER); \
        _node; _node = Zos_RbtNext(_rbtid, _node, EN_ZOS_RBT_TRAVERSE_INORDER))

/* zos red-balck tree traverse */
#define FOR_ALL_NODE_IN_RBT_POSTODER(_rbtid, _node) \
    for (_node = Zos_RbtHead(_rbtid, EN_ZOS_RBT_TRAVERSE_POSTORDER); \
       _node; _node = Zos_RbtNext(_rbtid, _node, EN_ZOS_RBT_TRAVERSE_POSTORDER))

/**
 * @brief Create a red-black tree with a compare function.
 */
ZFUNC ZRBTID Zos_RbtCreate(ZCBUF zMemBuf);

/**
 * @brief Delete a red-black tree.
 */
ZFUNC ZVOID Zos_RbtDelete(ZRBTID zTreeId);

/**
 * @brief Get the size of the red-black tree.
 */
ZFUNC ZUINT Zos_RbtSize(ZRBTID zTreeId);

/**
 * @brief Get the depth of the red-black tree.
 */
ZFUNC ZUINT Zos_RbtDepth(ZRBTID zTreeId);

/**
 * @brief Find a node that contains the given value.
 */
ZFUNC ZRBTNODE Zos_RbtFind(ZRBTID zTreeId, ZRBTKEY zKey);

/**
 * @brief Insert an value to the red-black tree.
 */
ZFUNC ZINT Zos_RbtAdd(ZRBTID zTreeId, ZRBTKEY zKey, ZRBTVAL zVal);

/**
 * @brief Remove an object from the red-black tree.
 */
ZFUNC ZINT Zos_RbtRmv(ZRBTID zTreeId, ZRBTKEY zKey);

/**
 * @brief Remanp new value to the specific key.
 * @param  zTreeId The tree ID.
 * @param  zKey    The key value.
 * @param  zNewVal The new mapped value.
 * @retval ZOK Remap OK.
 * @retval ZFAILED No node was found by the key.
 */
ZFUNC ZINT Zos_RbtMap(ZRBTID zTreeId, ZRBTKEY zKey, ZRBTVAL zNewVal);

/**
 * @brief Get begin node of the red-black tree by the given traverse mode.
 */
ZFUNC ZRBTNODE Zos_RbtHead(ZRBTID zTreeId, ZINT iMode);

/**
 * @brief Get begin node of the red-black tree by the given traverse mode.
 */
ZFUNC ZRBTNODE Zos_RbtTail(ZRBTID zTreeId, ZINT iMode);

/**
 * @brief Get next node of the red-black tree by the given traverse mode.
 */
ZFUNC ZRBTNODE Zos_RbtNext(ZRBTID zTreeId, ZRBTNODE zNode, ZINT iMode);

/**
 * @brief Get previous node of the red-black tree by the given traverse mode.
 */
ZFUNC ZRBTNODE Zos_RbtPrev(ZRBTID zTreeId, ZRBTNODE zNode, ZINT iMode);

/**
 * @brief Get a handle to the red-black tree minimum.
 */
ZFUNC ZRBTNODE Zos_RbtMin(ZRBTID zTreeId);

/**
 * @brief Get a handle to the red-black tree maximum.
 */
ZFUNC ZRBTNODE Zos_RbtMax(ZRBTID zTreeId);

/**
 * @brief Get the key of a node.
 */
ZFUNC ZUINT Zos_RbtGetKey(ZRBTNODE zNode);

/**
 * @brief Get the value of a node.
 */
ZFUNC ZRBTVAL Zos_RbtGetValue(ZRBTNODE zNode);

/**
 * @brief Print the red-black tree.
 */
ZFUNC ZDBUF Zos_RbtPrint(ZRBTID zTreeId, ZDBUF zMemBuf, ZBOOL bCompact);

/* check the red-black tree */
ZFUNC ZINT Zos_RbtChk(ZRBTID zTreeId);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_RBT_H__ */

