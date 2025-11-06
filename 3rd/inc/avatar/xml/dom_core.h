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
  File name     : dom_core.h
  Module        : dom core interface
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Function declears required by xml dom core interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DOM_CORE_H__
#define _DOM_CORE_H__

/** 
 * @file dom_core.h
 * @brief XML DOM core interfaces.
 *
 * @see 
 */
 
#ifdef __cplusplus
extern "C"{
#endif

#ifdef XML_SUPT_API_DOM /* is support dom */    

/********************** Dom Level 3 interfaces ***********************/

/**
 * @brief Node adds a new Child to the end of the list of children.
 *
 * @param [in] pstNode A node.
 * @param [in] pstNewChild The new child node.
 *
 * @retval ZOK Add the new Child successful.
 * @retval ZFAILED the new Child is  the current Node's parentNode 
 *          or the type of newChild and the current Node are read only.
 *
 * @see XML_LIST_ISEMPTY, XML_LIST_ADD2TAIL, XML_LIST_CREAT
 */
ZFUNC ZINT Dom_NodeAppendChild(ST_DOM_NODE *pstNode,
                ST_DOM_NODE *pstNewChild);

/**
 * @brief Node make a duplicate of this node.
 *
 * @param [in] pstNode A node.
 * @param [out] ppstNewNode To return the new node after clone.
 * @param [in] bDeep A flag,if the value is true then CloneNodeChilds,
 *             else the value is False CloneNodeSelf.
 *
 * @retval ZOK CloneNodeChilds or CloneNodeSelf sucessful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_CloneNodeSelf, Dom_CloneNodeChilds.
 */
ZFUNC ZINT Dom_NodeClone(ST_DOM_NODE *pstNode, ZBOOL bDeep,
                ST_DOM_NODE **ppstNewNode);

/**
 * @brief Node make a duplicate of this node.
 *
 * @param [in] pstNode A node.
 * @param [out] ppstAttrs To return the node attribute.
 *
 * @retval ZOK Node get Attrs successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetAttrs(ST_DOM_NODE *pstNode,
                ST_DOM_NNODE_MAP **ppstAttrs);

/**
 * @brief Node get all child nodes of this node.
 *
 * @param [in] pstNode A node.
 * @param [out] ppstChildLst To return the child list.
 *
 * @retval ZOK Node return the child nodes are not null.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetChilds(ST_DOM_NODE *pstNode, 
                ST_DOM_NODE_LST **ppstChildLst);

/**
 * @brief Node get the first child node of this node.
 *
 * @param [in] pstNode A node.
 * @param [out] ppstChild To return the first child node.
 *
 * @retval ZOK Node return the first child node is not null.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetFirstChild(ST_DOM_NODE *pstNode, 
                ST_DOM_NODE **ppstChild);

/**
 * @brief Node get the last child node of this node.
 *
 * @param [in] pstNode A node.
 * @param [out] ppstChild To return the last child node.
 *
 * @retval ZOK Node return the last child node is not null.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetLastChild(ST_DOM_NODE *pstNode, 
                ST_DOM_NODE **ppstChild);

/**
 * @brief Node get the next sibling node of this node.
 *
 * @param [in] pstNode A node.
 * @param [out] ppstSibling To return the next sibling node.
 *
 * @retval ZOK Node return the next sibling node is not null.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetNextSibling(ST_DOM_NODE *pstNode, 
                ST_DOM_NODE **ppstSibling);

/**
 * @brief Node get the previous sibling node of this node.
 *
 * @param [in] pstNode First Node.
 * @param [out] ppstSibling To return the previous sibling node.
 *
 * @retval ZOK Node return the previous sibling node is not null.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetPrevSibling(ST_DOM_NODE *pstNode, 
                ST_DOM_NODE **ppstSibling);

/**
 * @brief Node get the name of this node.
 *
 * @param [in] pstNode A Node.
 * @param [out] ppstName To return the name.
 *
 * @retval ZOK Get the node's name successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetName(ST_DOM_NODE *pstNode, ST_DOM_STR **ppstName);

/**
 * @brief Node set the name of this node.
 *
 * @param [in] pstNode A Node.
 * @param [in] pstName The name of this node.
 *
 * @retval ZOK Set the node's name successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AddNodeSStr, ZOS_USSTR_SETUX.
 */
ZFUNC ZINT Dom_NodeSetName(ST_DOM_NODE *pstNode, ST_DOM_STR *pstName);

/**
 * @brief Node get the type of this node.
 *
 * @param [in] pstNode A Node.
 * @param [out] pwType To return the type.
 *
 * @retval ZOK Get the node's type successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_NodeGetType(ST_DOM_NODE *pstNode, ZUSHORT *pwType);

/**
 * @brief Node get the value of this node.
 *
 * @param [in] pstNode A Node.
 * @param [out] ppstVal To return the value.
 *
 * @retval ZOK Get the node's value successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_NodeChkNormalizedLen.
 */
ZFUNC ZINT Dom_NodeGetVal(ST_DOM_NODE *pstNode, ST_DOM_STR **ppstVal);

/**
 * @brief Node set the value of this node.
 *
 * @param [in] pstNode A Node.
 * @param [in] pstVal The value of this node.
 *
 * @retval ZOK Set the node's value successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see  Dom_AddNodeSStr.
 */
ZFUNC ZINT Dom_NodeSetVal(ST_DOM_NODE *pstNode, ST_DOM_STR *pstVal);

/**
 * @brief Node get the owner document.
 *
 * @param [in] pstNode A Node.
 * @param [out] ppstDoc To return the owner document.
 *
 * @retval ZOK Get the Owner document successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeGetOwnerDoc(ST_DOM_NODE *pstNode, 
                ST_DOM_DOC **ppstDoc);

/**
 * @brief Node get the parent node.
 *
 * @param [in] pstNode A Node.
 * @param [out] ppstParent To return the parent node.
 *
 * @retval ZOK Get the Parent node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeGetParent(ST_DOM_NODE *pstNode, ST_DOM_NODE **ppstParent);

/**
 * @brief Node get the prefix.
 *
 * @param [in] pstNode A Node.
 * @param [out] ppstPrefix To return the prefix.
 *
 * @retval ZOK Get the Prefix successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeGetPrefix(ST_DOM_NODE *pstNode, ST_DOM_STR **ppstPrefix);

/**
 * @brief Node set the prefix.
 *
 * @param [in] pstNode A Node.
 * @param [in] pstPrefix The prefix.
 *
 * @retval ZOK Set the Prefix successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeSetPrefix(ST_DOM_NODE *pstNode, ST_DOM_STR *pstPrefix);

/**
 * @brief injudge whether the node have attributes.
 *
 * @param [in] pstNode A Node.
 *
 * @retval ZOK The attributes are exist.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZBOOL Dom_NodeHasAttrs(ST_DOM_NODE *pstNode);

/**
 * @brief injudge whether the node have children.
 *
 * @param [in] pstNode A Node.
 *
 * @retval ZOK The children nodes are exist.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZBOOL Dom_NodeHasChilds(ST_DOM_NODE *pstNode);

/**
 * @brief Node insert a newChild before existing refChild. 
 *
 * @param [in] pstNewChild A new Node.
 * @param [in] pstRefNode A refnode.
 *
 * @retval ZOK Insert successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */

ZFUNC ZINT Dom_NodeInsertBefore(ST_DOM_NODE *pstNewChild, 
                ST_DOM_NODE *pstRefNode);

/**
 * @brief Node removes oldChild from the list of children. 
 *
 * @param [in] pstOldChild An old node.
 *
 * @retval ZOK Remove a child successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeRmvChild(ST_DOM_NODE *pstOldChild);

/**
 * @brief Node removes children nodes. 
 *
 * @param [in] pstNode A Node.
 *
 * @retval ZOK Remove children successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeRmvChilds(ST_DOM_NODE *pstNode);

/**
 * @brief Node replaces oldChild with newChild. 
 *
 * @param [in] pstNewChild The new node.
 * @param [in] pstOldChild The old node.
 *
 * @retval ZOK Replaces successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeReplacChild(ST_DOM_NODE *pstNewChild, 
                ST_DOM_NODE *pstOldChild);

/**
 * @brief NodeList get item length. 
 *
 * @param [in] pstNodeLst A node list.
 * @param [out] piLen To return the length.
 *
 * @retval ZOK Get item length successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeLstGetLen(ST_DOM_NODE_LST *pstNodeLst, ZUINT *piLen);

/**
 * @brief NodeList get the item by index. 
 *
 * @param [in] pstNodeLst A node list.
 * @param [in] iIndex The index.
 * @param [out] ppstNode To return the item node.
 *
 * @retval ZOK Get the item according to the index successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NodeLstGetItem(ST_DOM_NODE_LST *pstNodeLst, ZUINT iIndex, 
                ST_DOM_NODE **ppstNode);

/**
 * @brief NamedNodeMap get length. 
 *
 * @param [in] pstNNodeMap A named node map.
 * @param [out] piLen To return the length.
 *
 * @retval ZOK Get the length successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NNodeMapGetLen(ST_DOM_NNODE_MAP *pstNNodeMap,
                ZUINT *piLen);

/**
 * @brief NamedNodeMap get the named item.
 *
 * @param [in] pstNNodeMap A named node map.
 * @param [in] pstName The name of node map.
 * @param [out] ppstNode To return the node
 *
 * @retval ZOK Get the length successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NNodeMapGetNItem(ST_DOM_NNODE_MAP *pstNNodeMap,
                ST_DOM_STR *pstName, ST_DOM_NODE **ppstNode);

/**
 * @brief NamedNodeMap get the item by index. 
 *
 * @param [in] pstNNodeMap A named node map.
 * @param [in] iIndex The index,for get the item.
 * @param [out] ppstNode To return the node.
 *
 * @retval ZOK Get the item according to the index successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NNodeMapGetItem(ST_DOM_NNODE_MAP *pstNNodeMap,
                ZUINT iIndex, ST_DOM_NODE **ppstNode);
/**
 * @brief NamedNodeMap set the named item. 
 *
 * @param [in] pstNNodeMap A named node map.
 * @param [in] pstArgNode A attribute node.
 *
 * @retval ZOK Get the item according to the index successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NNodeMapSetNItem(ST_DOM_NNODE_MAP *pstNNodeMap,
                ST_DOM_NODE *pstArgNode);   

/**
 * @brief NamedNodeMap remove the named item. 
 *
 * @param [in] pstNNodeMap A named node map.
 * @param [in] pstNode The remove node.
 *
 * @retval ZOK Remove the named item successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_NNodeMapRemNItem(ST_DOM_NNODE_MAP *pstNNodeMap,
                ST_DOM_NODE *pstNode);   

/**
 * @brief Element gets attribute. 
 *
 * @param [in] pstElem A Element structure.
 * @param [in] pstName The element name.
 * @param [out] ppstVal To return the attribute's value.
 *
 * @retval ZOK Get attribute successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_ElemGetAttr(ST_DOM_ELEM *pstElem, ST_DOM_STR *pstName,
                ST_DOM_STR **ppstVal); 

/**
 * @brief Element gets attribute node. 
 *
 * @param [in] pstElem A Element structure.
 * @param [in] pstName The element name.
 * @param [out] ppstAttr To return the attribute.
 *
 * @retval ZOK Get attribute node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_ElemGetAttrNode(ST_DOM_ELEM *pstElem, ST_DOM_STR *pstName,
                ST_DOM_ATTR **ppstAttr); 

/**
 * @brief Element gets single child element by tag name. 
 *
 * @param [in] pstElem A Element structure.
 * @param [in] pstName The element name.
 * @param [out] ppstChild To return the single child element.
 *
 * @return a node if dom Element get all child elements by tag name successful.
 * @retval ZFAILED  when any paramenter is null.
 *          
 * @see Dom_ElemGetSingleElemR.
 */
ZFUNC ZINT Dom_ElemGetSingleElem(ST_DOM_ELEM *pstElem, ST_DOM_STR *pstName, 
                ST_DOM_ELEM **ppstChild); 

/**
 * @brief Element gets all child elements by tag name. 
 *
 * @param [in] pstElem A Element structure.
 * @param [in] pstName The element name.
 * @param [out] ppstNodeLst To return the child node list.
 *
 * @retval ZOK Get all child elements successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_ElemGetElemsByTagNameX.
 */
ZFUNC ZINT Dom_ElemGetElemsByTagName(ST_DOM_ELEM *pstElem, 
                ST_DOM_STR *pstName, ST_DOM_NODE_LST **ppstNodeLst); 

/**
 * @brief Element gets all child elements by tag name with prefix. 
 *
 * @param [in] pstElem A Element node.
 * @param [in] pstName The element name.
 * @param [in] pstPrefix The namespace prefix.
 * @param [out] ppstNodeLst To return all child elements node list.
 *
 * @retval ZFAILED  if exist a parameter is null.
 * @return a node if Element gets single child element by tag name successful. 
 *          
 * @see Dom_ElemGetSingleElemR.
 */
ZFUNC ZINT Dom_ElemGetElemsByTagNameX(ST_DOM_ELEM *pstElem, 
                ST_DOM_STR *pstPrefix, ST_DOM_STR *pstName, 
                ST_DOM_NODE_LST **ppstNodeLst); 

/**
 * @brief Element gets the tag name . 
 *
 * @param [in] pstElem A Element node.
 * @param [out] ppstTagName To return the tag name.
 *
 * @retval ZOK Get the tag name successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_ElemGetSingleElemR.
 */
ZFUNC ZINT Dom_ElemGetTagName(ST_DOM_ELEM *pstElem, 
                ST_DOM_STR **ppstTagName); 

/**
 * @brief Element check whether the attribute is exsit. 
 *
 * @param [in] pstElem A Element node.
 * @param [in] pstName A attribute node name.
 *
 * @retval ZOK The attribute is exist.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_NNodeMapGetNItem.
 */
ZFUNC ZBOOL Dom_ElemHasAttr(ST_DOM_ELEM *pstElem, ST_DOM_STR *pstName);

/**
 * @brief Element sets attribute. 
 *
 * @param [in] pstElem A Element node.
 * @param [in] pstName A attribute node's name.
 * @param [in] pstVal A attribute node's value.
 *
 * @retval ZOK Set the attribute successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_NodeSetVal, Dom_NNodeMapSetNItem.
 */
ZFUNC ZINT Dom_ElemSetAttr(ST_DOM_ELEM *pstElem, ST_DOM_STR *pstName,
                ST_DOM_STR *pstVal); 

/**
 * @brief Element removes attribute. 
 *
 * @param [in] pstElem A Element node.
 * @param [in] pstName A attribute node's name. 
 *s
 * @retval ZOK Rmove attribute successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_NodeSetVal, Dom_NNodeMapSetNItem.
 */
ZFUNC ZINT Dom_ElemRemAttr(ST_DOM_ELEM *pstElem, ST_DOM_STR *pstName);  

/**
 * @brief Element removes attribute node
 *
 * @param [in] pstElem A Element node.
 * @param [in] pstOldAttr A old attribute node.
 *
 * @retval ZOK Rmove attribute node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_NNodeMapSetNItem.
 */
ZFUNC ZINT Dom_ElemRemAttrNode(ST_DOM_ELEM *pstElem, 
                ST_DOM_ATTR *pstOldAttr); 

/**
 * @brief Element sets attribute node. 
 *
 * @param [in] pstElem A element node.
 * @param [in] pstNewAttr A new attribute node.
 *
 * @retval ZOK Set attribute successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_NNodeMapSetNItem.
 */
ZFUNC ZINT Dom_ElemSetAttrNode(ST_DOM_ELEM *pstElem,
                ST_DOM_ATTR *pstNewAttr); 

/**
 * @brief Attr gets the name. 
 *
 * @param [in] pstAttr A attribute node.
 * @param [out] ppstName To return the name.
 *
 * @retval ZOK Get the name successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_AttrGetName(ST_DOM_ATTR *pstAttr, ST_DOM_STR **ppstName);

/**
 * @brief Attr gets the value. 
 *
 * @param [in] pstAttr A attribute node.
 * @param [out] ppstVal To return the value.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_AttrGetVal(ST_DOM_ATTR *pstAttr, ST_DOM_STR **ppstVal);

/**
 * @brief Attr Sets the value. 
 *
 * @param [in] pstAttr A attribute node.
 * @param [in] pstVal The attribute value.
 *
 * @retval ZOK Set the value successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_NodeSetVal
 */
ZFUNC ZINT Dom_AttrSetVal(ST_DOM_ATTR *pstAttr, ST_DOM_STR *pstVal);

/**
 * @brief Attr check whether the double quote is exist. 
 *
 * @param [in] pstAttr A attribute node.
 *
 * @retval ZTRUE The double quote is exist.
 * @retval ZFALSE Otherwise.
 *          
 * @see
 */
ZFUNC ZBOOL Dom_AttrGetDQuote(ST_DOM_ATTR *pstAttr);

/**
 * @brief Attr set the double quote. 
 *
 * @param [in] pstAttr A attribute node.
 * @param [in] bDQuote A flag's value(true or false).
 *
 * @retval ZOK Set the double quote successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_AttrSetDQuote(ST_DOM_ATTR *pstAttr, ZBOOL bDQuote);

/**
 * @brief CharacterData split data. 
 *
 * @param [in] pstData A Character Data node.
 * @param [in] iOffset The length of new node.
 * @param [out] ppstNewData Return a new Character Data node.
 *
 * @retval ZOK Split data successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DocCreateCDataSec
 */

ZFUNC ZINT Dom_CDataSplit(ST_DOM_CDATA *pstData, ZUINT iOffset,
                ST_DOM_CDATA **ppstNewData);

/**
 * @brief CharacterData replaces whole text. 
 *
 * @param [in] pstData A Character Data node.
 * @param [in] pstContent The content string.
 *
 * @retval ZOK Replace successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AddNodeSStr
 */

ZFUNC ZINT Dom_CDataReplace(ST_DOM_CDATA *pstData, ST_DOM_STR *pstContent);

/**
 * @brief CharacterData appends data. 
 *
 * @param [in] pstData A Character Data node.
 * @param [in] pstAppendData The append data.
 *
 * @retval ZOK Append data successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByNodes
 */
ZFUNC ZINT Dom_CDataAppendData(ST_DOM_CDATA *pstData, 
                ST_DOM_STR *pstAppendData);

/**
 * @brief CharacterData deletes data. 
 *
 * @param [in] pstData  A Character Data node.
 * @param [in] iOffset The palce for delete.
 * @param [in] iCount The length of data.
 *
 * @retval ZOK Delete data successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Dom_CDataDelData(ST_DOM_CDATA *pstData, ZUINT iOffset, 
                ZUINT iCount);
/**
 * @brief CharacterData inserts data. 
 *
 * @param [in] pstData A Character Data node.
 * @param [in] iOffset The place for insert.
 * @param [in] pstInsertData The length of data.
 *
 * @retval ZOK Insert data successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see  Dom_AllocByNode
 */
ZFUNC ZINT Dom_CDataInsertData(ST_DOM_CDATA *pstData, ZUINT iOffset, 
                ST_DOM_STR *pstInsertData);

/**
 * @brief CharacterData sets data. 
 *
 * @param [in] pstData A Character Data node.
 * @param [in] pstSetData A data.
 *
 * @retval ZOK Set data successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see  Dom_AddNodeSStrs
 */
ZFUNC ZINT Dom_CDataSetData(ST_DOM_CDATA *pstData, ST_DOM_STR *pstSetData);

/**
 * @brief CharacterData get a substring data. 
 *
 * @param [in] pstData A Character Data node.
 * @param [in] iOffset The place for get the substring.
 * @param [in] iCount The substring's length.
 * @param [in] pstSubData To get the substring.
 *
 * @retval ZOK Get the substring successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_CDataGetSubData(ST_DOM_CDATA *pstData, ZUINT iOffset, 
                ZUINT iCount, ST_DOM_STR *pstSubData);

/**
 * @brief ProcessingInstruction gets the data. 
 *
 * @param [in] pstPi A Processing Instruction Node.
 * @param [out] ppstData To return the data.
 *
 * @retval ZOK Get the data successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see  
 */
ZFUNC ZINT Dom_PiGetData(ST_DOM_PI *pstPi, ST_DOM_STR **ppstData);

/**
 * @brief ProcessingInstruction sets the data. 
 *
 * @param [in] pstPi A Processing Instruction Node.
 * @param [in] pstData The data of Processing Instruction Node.
 *
 * @retval ZOK Set the data successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AddNodeSStrs
 */
ZFUNC ZINT Dom_PiSetData(ST_DOM_PI *pstPi, ST_DOM_STR *pstData);

/**
 * @brief ProcessingInstruction gets the target. 
 *
 * @param [in] pstPi A Processing Instruction Node.
 * @param [out] ppstTarget To return the target.
 *
 * @retval ZOK Get the target successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see  
 */
ZFUNC ZINT Dom_PiGetTarget(ST_DOM_PI *pstPi, ST_DOM_STR **ppstTarget);

/**
 * @brief Document create a ATTR node. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstName The name of the attribute node.
 * @param [out] ppstAttr To return the attributr node.
 *
 * @retval ZOK  Create the ATTR node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DocCreateAttrX,Dom_InitNode.
 */
ZFUNC ZINT Dom_DocCreateAttr(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstName,
                ST_DOM_ATTR **ppstAttr);

/**
 * @brief Document create a ATTR node with prefix. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstPrefix The prefix.
 * @param [in] pstName The name of the attribute node.
 * @param [out] ppstAttr To return the attributr node.
 *
 * @retval ZOK Create the ATTR node with prefix successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByDoc,Dom_AddNodeSStrs,Dom_InitNode.
 */
ZFUNC ZINT Dom_DocCreateAttrX(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstPrefix,
                ST_DOM_STR *pstName, ST_DOM_ATTR **ppstAttr);

/**
 * @brief Document create a CDATASection node. 
 *s
 * @param [in] pstDoc A Document node.
 * @param [in] pstData The data of CDATASection node.
 * @param [out] ppstCDataSec Return a CDATASection node.
 *
 * @retval ZOK Create the CDATASection node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByDocsa,Dom_InitNode,Dom_AddNodeSStrs.
 */
ZFUNC ZINT Dom_DocCreateCDataSec(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstData,
                ST_DOM_CDATA_SEC **ppstCDataSec);
/**
 * @brief Document create a comment node. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstData The data of Comment node.
 * @param [out] ppstCommt Return a Comment node.
 *
 * @retval ZOK Create the comment node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByDocsa,Dom_InitNode,Dom_AddNodeSStrs.
 */
ZFUNC ZINT Dom_DocCreateCommt(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstData,
                ST_DOM_COMMT **ppstCommt);

/**
 * @brief Document create a document fragment node. 
 *
 * @param [in] pstDoc A Document node.
 * @param [out] ppstDocFrag To return a document fragment node.
 *
 * @retval ZOK Create the document fragment node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByDocsa,Dom_InitNode,Dom_AddNodeSStrs.
 */
ZFUNC ZINT Dom_DocCreateDocFrag(ST_DOM_DOC *pstDoc,
                ST_DOM_DFRAG **ppstDocFrag);

/**
 * @brief Document create a element node. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstTagName The Tag name
 * @param [out] ppstElem To return a element Node.
 *
 * @retval ZOK Create the new element node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DocCreateElemX.
 */
ZFUNC ZINT Dom_DocCreateElem(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstTagName,
                ST_DOM_ELEM **ppstElem);

/**
 * @brief Document create a element node with prefix. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstPrefix The prefix.
 * @param [in] pstName The new element node's name.
 * @param [out] ppstElem To return a element Node.
 *
 * @retval ZOK Create the new element node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByDocsa,Dom_InitNode.
 */
ZFUNC ZINT Dom_DocCreateElemX(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstPrefix,
                ST_DOM_STR *pstName, ST_DOM_ELEM **ppstElem);


/**
 * @brief Document create a ProcessingInstruction node. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstTarget The terget of Processing Instruction node.
 * @param [in] pstData The data of Processing Instruction node.
 * @param [out] ppstPi Return a  Processing Instruction Node.
 *
 * @retval ZOK Create the new ProcessingInstruction node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByDocsa,Dom_InitNode,Dom_AddNodeSStrs.
 */
ZFUNC ZINT Dom_DocCreatePi(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstTarget, 
                ST_DOM_STR *pstData, ST_DOM_PI **ppstPi);
/**
 * @brief Document create a text node. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstData The text of text node.
 * @param [out] ppstTxt Return a Text node.
 *
 * @retval ZOK Create the new text node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_AllocByDocsa,Dom_InitNode,Dom_AddNodeSStrs.
 */
ZFUNC ZINT Dom_DocCreateTxt(ST_DOM_DOC *pstDoc, ST_DOM_STR *pstData,
                ST_DOM_TXT **ppstTxt);

/**
 * @brief Document get the element. 
 *
 * @param [in] pstDoc A Document node.
 * @param [out] ppstElem Return a element Node.
 *
 * @retval ZOK Create the element successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_DocGetElem(ST_DOM_DOC *pstDoc, ST_DOM_ELEM **ppstElem);

/**
 * @brief Document get the elements by the tag name. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstTagName The tag name.
 * @param [out] ppstNodeLst Return the element node list.
 *
 * @retval ZOK Get the elements successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_ElemGetElemsByTagName
 */
ZFUNC ZINT Dom_DocGetElemsByTagName(ST_DOM_DOC *pstDoc, 
                ST_DOM_STR *pstTagName, ST_DOM_NODE_LST **ppstNodeLst);

/**
 * @brief Document get the elements by the tag name with prefix. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstPrefix The prefix.
 * @param [in] pstName The tag name.
 * @param [out] ppstNodeLst Return the element node list.
 *
 * @retval ZOK Get the elements successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_ElemGetElemsByTagNameX
 */
ZFUNC ZINT Dom_DocGetElemsByTagNameX(ST_DOM_DOC *pstDoc, 
                ST_DOM_STR *pstPrefix, ST_DOM_STR *pstName, 
                ST_DOM_NODE_LST **ppstNodeLst);

/**
 * @brief Document import a node. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] bDeep A flag,if the value is true then Clone Document all Children.
 * @param [in] pstImportedNode An import node.
 *
 * @retval ZOK Import a node successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DocCreateElem,Dom_CloneDocChilds.
 */
ZFUNC ZINT Dom_DocImportNode(ST_DOM_DOC *pstDoc, 
                ST_DOM_NODE *pstImportedNode, ZBOOL bDeep);
/**
 * @brief Document load xml document from the specified location. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pcFileName The file name.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see .
 */
ZFUNC ZINT Dom_DocLoad(ST_DOM_DOC *pstDoc, ZCHAR *pcFileName);

/**
 * @brief Document load xml document using the supplied string. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstXmlStr The xml data string.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DecodeMsg.
 */
ZFUNC ZINT Dom_DocLoadXml(ST_DOM_DOC *pstDoc, ST_ZOS_USTR *pstXmlStr);

/**
 * @brief Document save xml document to the specified location. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pcFileName The file name.
 *
 * @retval ZOK Save successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DocSaveXml,Zos_DbufSaveFile.
 */
ZFUNC ZINT Dom_DocSave(ST_DOM_DOC *pstDoc, ZCHAR *pcFileName);

/**
 * @brief Document save xml document using the supplied message buffer. 
 *
 * @param [in] pstDoc A Document node.
 * @param [in] pstMsgBuf The message buffer.
 *
 * @retval ZOK Save successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_EncodeMsg.
 */
ZFUNC ZINT Dom_DocSaveXml(ST_DOM_DOC *pstDoc, ZDBUF zMsgBuf);

/**
 * @brief Document create a xml document. 
 *
 * @param [out] ppstDoc A Document node.
 * @param [in] ucEncodingType The type of xml Encoding.
 *
 * @retval ZOK Create successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DocCreateX.
 */
ZFUNC ZINT Dom_DocCreate(ZUCHAR ucEncodingType, ST_DOM_DOC **ppstDoc);

/**
 * @brief Document create a xml document. 
 *
 * @param [out] ppstDoc A Document node.
 * @param [in] ucEncodingType The type of xml Encoding.
 * @param [in] iBufSize The buffer size,it decides the size of document. 
 *
 * @retval ZOK Create successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Dom_DocCreateX.
 */
ZFUNC ZINT Dom_DocCreateX(ZUCHAR ucEncodingType, ZUINT iBufSize, 
                ST_DOM_DOC **ppstDoc);

/**
 * @brief Document delete a xml document. 
 *
 * @param [in] pstDoc A Document node.
 *
 * @retval ZOK delete successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT Dom_DocDelete(ST_DOM_DOC *pstDoc);

#endif
    
#ifdef __cplusplus
}
#endif

#endif /* _DOM_CORE_H__ */

