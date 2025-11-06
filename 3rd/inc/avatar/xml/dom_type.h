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
  File name     : xml_type.h
  Module        : dom typedefs
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Data structure required by the xml dom
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DOM_TYPE_H__
#define _DOM_TYPE_H__
/** 
 * @file dom_type.h
 * @brief XML DOM type and structure definition.
 *
 * @see 
 */


#ifdef __cplusplus
extern "C"{
#endif

#ifdef XML_SUPT_API_DOM /**< @brief Is support dom. */    

/** @brief Dom exception code. */
typedef enum EN_DOM_EXCEPT_ERR_CODE
{
    EN_DOM_EXCEPT_ERR_INDEX_SIZE = 1, 
    EN_DOM_EXCEPT_ERR_DSTR_SIZE = 2,
    EN_DOM_EXCEPT_ERR_HIERARCHY_REQ = 3,
    EN_DOM_EXCEPT_ERR_WRONG_DOC = 4,
    EN_DOM_EXCEPT_ERR_INVALID_CHR = 5,
    EN_DOM_EXCEPT_ERR_NO_DATA_ALLOWED = 6,
    EN_DOM_EXCEPT_ERR_NO_MDFY_ALLOWED= 7,
    EN_DOM_EXCEPT_ERR_NOT_FOUND = 8,
    EN_DOM_EXCEPT_ERR_NOT_SUPTED = 9,
    EN_DOM_EXCEPT_ERR_INUSE_ATTR = 10,
    EN_DOM_EXCEPT_ERR_INV_STA = 11,
    EN_DOM_EXCEPT_ERR_SYNTAX = 12,
    EN_DOM_EXCEPT_ERR_INV_MDFY = 13,
    EN_DOM_EXCEPT_ERR_NAMESPACE = 14,
    EN_DOM_EXCEPT_ERR_INV_ACCESS = 15,
    EN_DOM_EXCEPT_ERR_VALIDATION = 16,
    EN_DOM_EXCEPT_ERR_TYPE_MISMATCH = 17
} EN_DOM_EXCEPT_ERR_CODE;

/** @brief Dom Node type. */
typedef enum EN_DOM_NODE_TYPE
{
    EN_DOM_NODE_NULL = 0,
    EN_DOM_NODE_ELEM = 1,
    EN_DOM_NODE_ATTR = 2,
    EN_DOM_NODE_TXT = 3,
    EN_DOM_NODE_CDATA_SEC = 4,
    EN_DOM_NODE_ENT_REFER = 5,
    EN_DOM_NODE_ENT = 6,
    EN_DOM_NODE_PI = 7,
    EN_DOM_NODE_COMMENT = 8,
    EN_DOM_NODE_DOC = 9,
    EN_DOM_NODE_DOC_TYPE = 10,
    EN_DOM_NODE_DOC_FRAG = 11,
    EN_DOM_NODE_NOTATION = 12
} EN_DOM_NODE_TYPE;

/** @brief Dom DocumentPosition type. */
typedef enum EN_DOM_DOC_POS_TYPE
{
    EN_DOM_DOC_POS_DISC = 0x01,
    EN_DOM_DOC_POS_PRECEDING = 0x02,
    EN_DOM_DOC_POS_FOLLOWING = 0x04,
    EN_DOM_DOC_POS_CONTAINS = 0x08,
    EN_DOM_DOC_POS_CONTAINED_BY = 0x10,
    EN_DOM_DOC_POS_IMP_SPEC = 0x20        
} EN_DOM_DOC_POS_TYPE;

/** @brief Dom DerivationMethods type. */
typedef enum EN_DOM_DERV_METHOD_TYPE
{
    EN_DOM_DERV_METHOD_RESTRICT = 0x00000001,
    EN_DOM_DERV_METHOD_EXTN = 0x00000002,
    EN_DOM_DERV_METHOD_UNION = 0x00000004,
    EN_DOM_DERV_METHOD_LST = 0x00000008
} EN_DOM_DERV_METHOD_TYPE;

/** @brief Dom OperationType type. */
typedef enum EN_DOM_OPERAT_TPYE
{
    EN_DOM_OPERAT_NODE_CLONED = 1,
    EN_DOM_OPERAT_NODE_IMPORTED = 2,
    EN_DOM_OPERAT_NODE_DELETED = 3,
    EN_DOM_OPERAT_NODE_RENAMED = 4,
    EN_DOM_OPERAT_NODE_ADOPTED = 5
} EN_DOM_OPERAT_TPYE;

/** @brief Dom ErrorSeverity type. */
typedef enum EN_DOM_ERR_SVRTY_TYPE
{
    EN_DOM_ERR_SVRTY_WARN = 1,
    EN_DOM_ERR_SVRTY_ERR = 2,
    EN_DOM_ERR_SVRTY_FATAL = 3
} EN_DOM_ERR_SVRTY_TYPE;

/** @brief Dom DOMString. */
typedef ST_ZOS_USTR ST_DOM_STR;

/** @brief Typedef dom Document. */
typedef struct tagDOM_DOC ST_DOM_DOC;

/** @brief Typedef dom Node. */
typedef struct tagDOM_NODE ST_DOM_NODE;

/** @brief Dom node list. */
ZOS_TYPEDEF_DLIST(DOM_NODE); 

/** @brief Dom name introduced in DOM Level 3. */
typedef struct tagDOM_NAME
{
    ST_DOM_STR stName;              /**< @brief Name. */
    ST_DOM_NODE *pstAttrNode;       /**< @brief ATTR node. */     
} ST_DOM_NAME;

/** @brief Dom name list. */
ZOS_TYPEDEF_DLIST(DOM_NAME);

/** @brief Dom NamedNodeMap. */
typedef struct tagDOM_NNODE_MAP
{
    ST_DOM_NAME_LST stNameLst;       /**< @brief Name list. */
} ST_DOM_NNODE_MAP;

/** @brief Dom Node. */
struct tagDOM_NODE
{
    ZUCHAR ucType;                   /**< @brief Node type EN_DOM_NODE_TYPE. */
    ZUCHAR aucSpare[3];              /**< @brief For 32 bit alignment. */
    ST_DOM_STR stName;               /**< @brief Node name. */
    ST_DOM_STR stVal;                /**< @brief Node value. */
    ST_XML_LIST_NODE stTodoNode;     /**< @brief Todo list node. */
    ST_DOM_NODE_LST *pstChildNodes;  /**< @brief Children nodes. */
    struct tagDOM_NODE *pstFirstChild; /**< @brief The first child node. */
    struct tagDOM_NODE *pstLastChild; /**< @brief The last child node. */
    struct tagDOM_NODE *pstNextSibling; /**< @brief The next sibling node. */
    struct tagDOM_NODE *pstPrevSibling; /**< @brief The previous sibling node.*/
    struct tagDOM_NODE *pstParentNode; /**< @brief Parent node. */
    ST_DOM_NNODE_MAP stAttrs;        /**< @brief Attributes. */
    ST_DOM_DOC *pstOwnerDoc;         /**< @brief Document owner. */    
    ST_DOM_STR stPrefix;             /**< @brief Namespace prefix. */
};

/** @brief Dom Attr Node. */
typedef struct tagDOM_ATTR
{
    ST_DOM_NODE stNode;              /**< @brief Attr Node. */
    ST_DOM_STR stName;               /**< @brief Node name. */
    ST_DOM_STR stVal;                /**< @brief Node value. */
    ZBOOL bDQuote;                   /**< @brief Double quote. */
} ST_DOM_ATTR;

/** @brief Dom Attr Node list. */
ZOS_TYPEDEF_DLIST(DOM_ATTR);

/** @brief Dom CharacterData. */
typedef struct tagDOM_CDATA
{
    ST_DOM_NODE stNode;              /**< @brief Comment Node. */
    ST_DOM_STR stData;               /**< @brief Node data. */
} ST_DOM_CDATA;

/** @brief Dom Comment CharacterData. */
typedef ST_DOM_CDATA ST_DOM_COMMT;

/** @brief Dom Text CharacterData. */
typedef ST_DOM_CDATA ST_DOM_TXT;

/** @brief Dom CDATASection. */
typedef ST_DOM_TXT ST_DOM_CDATA_SEC;

/** @brief Dom DocumentType : Node. */
typedef struct tagDOM_DTYPE
{
    ST_DOM_NODE stNode;              /**< @brief DocumentType Node. */
    ST_DOM_NNODE_MAP stEnts;         /**< @brief Entities. */
    ST_DOM_NNODE_MAP stNotats;       /**< @brief Notations. */
} ST_DOM_DTYPE;

/** @brief Dom Notation : Node. */
typedef struct tagDOM_NOTAT
{
    ST_DOM_NODE stNode;              /**< @brief Notation Node. */
    ST_DOM_STR stPubId;              /**< @brief PublicId. */
    ST_DOM_STR stSysId;              /**< @brief SystemId. */
} ST_DOM_NOTAT;

/** @brief Dom Entity : Node. */
typedef struct tagDOM_ENT
{
    ST_DOM_NODE stNode;              /**< @brief Entity Node. */
    ST_DOM_STR stPubId;              /**< @brief PublicId. */
    ST_DOM_STR stSysId;              /**< @brief SystemId. */
    ST_DOM_STR stNotatName;          /**< @brief Notation name. */
    ST_DOM_STR stInputEncoding;      /**< @brief Input encoding. */    
    ST_DOM_STR stXmlEncoding;        /**< @brief XML encoding. */    
    ST_DOM_STR stXmlVer;             /**< @brief XML version. */    
} ST_DOM_ENT;

/** @brief Dom ProcessingInstruction : Node. */
typedef struct tagDOM_PI
{
    ST_DOM_NODE stNode;              /**< @brief ProcessingInstruction Node. */
    ST_DOM_STR stTarget;             /**< @brief Target. */
    ST_DOM_STR stData;               /**< @brief Data. */
} ST_DOM_PI;

/** @brief Typedef dom DocumentFragment. */
typedef ST_DOM_NODE ST_DOM_DFRAG;

/** @brief Dom Element Node. */
typedef struct tagDOM_ELEM
{
    ST_DOM_NODE stNode;              /**< @brief Element Node. */
    ST_DOM_STR stTagName;            /**< @brief Tag name. */
} ST_DOM_ELEM;

/** @brief Dom Document. */
struct tagDOM_DOC
{
    ZSBUF zMemBuf;                 /**< @brief Memory buffer */
    ZBOOL bXmlStandAlone;            /**< @brief XML standalone. */
    ST_DOM_STR stDocUri;             /**< @brief Document uri. */
    ST_DOM_DTYPE stDocType;          /**< @brief Document type. */
    ST_DOM_ELEM stDocElem;           /**< @brief Document element. */
    ST_DOM_STR stInputEncoding;      /**< @brief Input encoding. */
    ST_DOM_STR stXmlEncoding;        /**< @brief XML encoding. */    
    ST_DOM_STR stXmlVer;             /**< @brief XML version. */    
};

#endif
    
#ifdef __cplusplus
}
#endif

#endif /* _DOM_TYPE_H__ */

