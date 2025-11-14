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
  File name     : xpath_ui.h
  Module        : xpath
  Author        : Bob.Liu
  Version       : 0.1
  Created on    : 2007-12-11
  Description   :
    Marcos and structure definitions required by the xpath.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XPATH_UI_H__
#define _XPATH_UI_H__

#ifdef __cplusplus
extern "C"{
#endif

/** @brief XPath node type. */
typedef enum EN_XPATH_DATA_TYPE
{
    EN_XPATH_ROOT,                   /**< @brief Root element. */
    EN_XPATH_ELEM,                   /**< @brief Element other than root. */
    EN_XPATH_ATTR,                   /**< @brief Attribute */
    EN_XPATH_NS,                     /**< @brief Namespace */
    EN_XPATH_PI,                     /**< @brief PI */
    EN_XPATH_COMM,                   /**< @brief Comment */
    EN_XPATH_TEXT                    /**< @brief Text */
} EN_XPATH_DATA_TYPE;

/** @brief XPath managment structure. */
typedef ZVOID * ZXPATH_MGR;

/** @brief XPath context node structure. */
typedef struct tagXPATH_NODE
{
    ZUCHAR ucType;                   /**< @brief Context type, @ref EN_XPATH_DATA_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bits alignment */
    union
    {
        ST_XML_DOC *pstRoot;         /**< @brief Root element */
        ST_XML_ELEM *pstElem;        /**< @brief Element other than root */
        ST_XML_ATTR *pstAttr;        /**< @brief Attribute */
        ST_XML_ATTR *pstNs;          /**< @brief Namespace */
        ST_XML_PI *pstPi;            /**< @brief Processing instruction */
        ST_XML_COMMENT *pstComment;  /**< @brief Comment */
        ST_XML_CHR_DATA *pstText;    /**< @brief Text */
    } u;
} ST_XPATH_NODE;

/** @brief XPath context node list structure */
XML_TYPEDEF_LIST(XPATH_NODE);

/** @brief XPath get text string. */
#define XPATH_GET_TEXT(_pathmgr, _buf, _path, _str) do { \
    ST_XPATH_NODE *_node = Xpath_FindNode(_pathmgr, ZOS_NSTR(_path)); \
    if (_node) Zos_UbufCpyUXStr(_buf, &_node->u.pstText->stData, &(_str)); \
} while (0)

/** @brief XPath get attribute string. */
#define XPATH_GET_ATTR(_pathmgr, _buf, _path, _str) do { \
    ST_XPATH_NODE *_node = Xpath_FindNode(_pathmgr, ZOS_NSTR(_path)); \
    if (_node) \
        Zos_UbufCpyUXStr(_buf, &_node->u.pstAttr->stVal.stValStr, &(_str)); \
} while (0)

/**
 * @brief Create managment for XPath parser.
 *
 * @param [in] pstNs Namespace structure.
 * @param [in] pstMsg XML message.
 *
 * @return XPath parser managment structure on succeed, or return ZNULL.
 *
 * @see Xpath_Delete
 */
ZFUNC ZXPATH_MGR Xpath_Create(ST_EAX_NS *pstNs, ST_XML_MSG *pstMsg);

/**
 * @brief Delete managment for XPath parser.
 *
 * @param [in] zPathMgr XPath parser managment.
 *
 * @see Xpath_Create
 */
ZFUNC ZVOID Xpath_Delete(ZXPATH_MGR zPathMgr);

/**
 * @brief Set the context element node for the relative path.
 *
 * @param [in] zPathMgr XPath parser managment.
 * @param [in] pstNode The cursor node.
 *
 * @retval ZOK On succeed.
 * @retval ZFAILED On failed.
 *
 * @see
 */
ZFUNC ZINT Xpath_SetCursor(ZXPATH_MGR zPathMgr, ST_XPATH_NODE *pstNode);

/**
 * @brief Search for the first node matched the specified path.
 *
 * @param [in] zPathMgr XPath parser managment.
 * @param [in] pcPath The path string.
 * @param [in] iLen Length of pcPath.
 *
 * @return The node matched the path, or return ZNULL on failed. If there are
           more than one node matched, the first returned.
 *
 * @see Xpath_FindNodes
 */
ZFUNC ST_XPATH_NODE * Xpath_FindNode(ZXPATH_MGR zPathMgr, ZCHAR *pcPath, 
                ZUINT iLen);

/**
 * @brief Search for node matched the specified path.
 *
 * @param [in] zPathMgr XPath parser managment.
 * @param [in] pcPath The path string.
 * @param [in] iLen Length of pcPath.
 *
 * @return The node matched the path, or return ZNULL on failed. If there are
           more than one node matched, all nodes returned.
 *
 * @see Xpath_FindNode
 */
ZFUNC ST_XPATH_NODE_LST * Xpath_FindNodes(ZXPATH_MGR zPathMgr, 
                ZCHAR *pcPath, ZUINT iLen);

/** @brief Namespace resolver callback function used by 
           @ref Xpath_Eval @ref Xpath_EvalX. 
 *
 * @param dwCookie User specified data in @ref Xpath_Eval and @ref Xpath_EvalX. 
 * @param pstElem The current element the parser is working at.
 * @param pcPrefix The namespace string in the XPath expression. 
 * @param iPrefixLen The length of pcPrefix.
 * @param pcElemPrefix The namespace string of current working node. 
 * @param iElemPrefixLen The lenght of pcElemPrefix.
 *
 * @return ZTRUE for these namespace string are indicate the same namespace.
           ZFALSE for they are different namespace.
 */
typedef ZBOOL (*PFN_XPATHNSRESOLVER)(ZCOOKIE zCookie, ST_XML_ELEM *pstElem,
                ZCHAR *pcPrefix, ZUINT iPrefixLen, ZCHAR *pcElemPrefix,
                ZUINT iElemPrefixLen);

/** @brief Result check callback function used by @ref Xpath_Eval. 
 *
 * @param pResult The matched node. 
 * @param ucType The node type @ref EN_XPATH_DATA_TYPE.
 *
 * @return ZTRUE for stoping parse. ZFALSE for continue searching.
 */
typedef ZBOOL (*PFN_XPATHCHKRESULT)(ZCOOKIE zCookie, ZVOID *pResult,
                ZUCHAR ucType);

/**
 * @brief Evaluate the specified expression.
 *
 * @param [in] dwCookie User specified data which will be used in 
                          pfnNsResolver and pfnChkResult.
 * @param [in] pstMsg The XML message.
 * @param [in] pstCtxElem The context element where parse start.
 * @param [in] pcPath The path string.
 * @param [in] iLen Length of pcPath.
 * @param [in] pfnNsResolver The namespace resolver callback function. When 
                             there is a namespace, parser will invoke this
                             function to compare if the prefix in pcPath and
                             the element's prefix are matched. These string
                             may be different, but both indicate the same
                             namespace, so this can only be known by user.
 * @param [in] pfnChkResult The result check callback function. When there is
                            a matched node found, parser will invoke this
                            function.
 *
 * @return The node matched the path, or return ZNULL on failed. If there are
           more than one node matched, all nodes returned.
 *
 * @see Xpath_EvalX
 */
ZFUNC ZINT Xpath_Eval(ZCOOKIE zCookie, ST_XML_MSG *pstMsg,
                ST_XML_ELEM *pstCtxElem, ZCONST ZCHAR *pcPath, ZUINT iLen,
                PFN_XPATHNSRESOLVER pfnNsResolver,
                PFN_XPATHCHKRESULT pfnChkResult);

/**
 * @brief Evaluate the specified expression, and get the first matched.
 *
 * @param [in] dwCookie User specified data which will be used in 
                          pfnNsResolver and pfnChkResult.
 * @param [in] pstMsg The XML message.
 * @param [in] pstCtxElem The context element where parse start.
 * @param [in] pcPath The path string.
 * @param [in] iLen Length of pcPath.
 * @param [in] pfnNsResolver The namespace resolver callback function. When 
                             there is a namespace, parser will invoke this
                             function to compare if the prefix in pcPath and
                             the element's prefix are matched. These string
                             may be different, but both indicate the same
                             namespace, so this can only be known by user.
 * @param [out] ppResult The node matced the path. If there are more than one
                         node matched, this value will be assigned by first.
 *
 * @return The element contains the node matched the path,
           or return ZNULL on failed.
 *
 * @see Xpath_Eval
 */
ZFUNC ST_XML_ELEM * Xpath_EvalX(ZCOOKIE zCookie, ST_XML_MSG *pstMsg,
                ST_XML_ELEM *pstCtxElem, ZCONST ZCHAR *pcPath, ZUINT iLen,
                PFN_XPATHNSRESOLVER pfnNsResolver, ZVOID **ppResult);

#ifdef __cplusplus
}
#endif

#endif /* _XPATH_UI_H__ */

