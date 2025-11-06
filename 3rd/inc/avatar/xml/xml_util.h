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
  File name     : xml_util.h
  Module        : xml utility
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Data structure and function definitions required by the xml utility
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XML_UTIL_H__
#define _XML_UTIL_H__
/** 
 * @file xml_util.h
 * @brief XML utility interfaces.
 *
 * @see 
 */
#ifdef __cplusplus
extern "C"{
#endif

/** @brief XML alloc memory of specific size. */
#define XML_ALLOC(_sbufid, _size) \
    Zos_UbufAlloc(_sbufid, _size)

/** @brief XML alloc memory with zero of specific size. */
#define XML_ALLOC_CLRD(_sbufid, _size) \
    Zos_UbufAllocClrd(_sbufid, _size)

/** @brief XML alloc memory of specific type. */
#define XML_ALLOC_ELEM(_sbufid, _type) \
    XML_ALLOC_CLRD(_sbufid, sizeof(_type))

/** @brief XML list macros. */
#define XML_LIST_SIZE(_list)  \
    ZOS_TYPEDEF_DLIST_SIZE(_list)
    
/** @brief XML list is full. */   
#define XML_LIST_ISFULL(_list)  \
    ZOS_TYPEDEF_DLIST_ISFULL(_list)
    

/** @brief XML list is empty. */   
#define XML_LIST_ISEMPTY(_list) \
    ZOS_TYPEDEF_DLIST_ISEMPTY(_list)
    
/** @brief XML list is not empty. */ 
#define XML_LIST_ISNOEMPTY(_list) \
    ZOS_TYPEDEF_DLIST_ISNOEMPTY(_list)

/** @brief XML list head node. */ 
#define XML_LIST_HEAD_NODE(_list) \
    ZOS_TYPEDEF_DLIST_HEAD_NODE(_list)

/** @brief XML list tail node. */     
#define XML_LIST_TAIL_NODE(_list) \
    ZOS_TYPEDEF_DLIST_TAIL_NODE(_list)

/** @brief XML list privious node. */ 
#define XML_LIST_PREV_NODE(_node) \
    ZOS_TYPEDEF_DLIST_PREV_NODE(_node)

/** @brief XML list next node. */
#define XML_LIST_NEXT_NODE(_node) \
    ZOS_TYPEDEF_DLIST_NEXT_NODE(_node)

/** @brief XML list data node. */    
#define XML_LIST_GET_DATA(_node) \
    ZOS_TYPEDEF_DLIST_GET_DATA(_node)

/** @brief XML list pData.*/
#define XML_LIST_HEAD_DATA(_list) \
    ZOS_TYPEDEF_DLIST_HEAD_DATA(_list)

/** @brief XML list node.*/
#define XML_LIST_DATA_NODE(_data) \
    ((ST_XML_LIST_NODE *)(_data) - 1)

/** @brief Enumerate all node in list.*/
#define FOR_ALL_NODE_IN_XML_LIST(_list, _node) \
    FOR_ALL_NODE_IN_TYPEDEF_DLIST(_list, _node)

/** @brief Enumerate all next node in list.*/
#define FOR_ALL_NODE_IN_XML_LIST_NEXT(_list, _node, _nextnode) \
    FOR_ALL_NODE_IN_TYPEDEF_DLIST_NEXT(_list, _node, _nextnode)

/** @brief Enumerate all previous node in list.*/
#define FOR_ALL_NODE_IN_XML_LIST_PREV(_list, _node, _prevnode) \
    FOR_ALL_NODE_IN_TYPEDEF_DLIST_PREV(_list, _node, _prevnode)

/** @brief Enumerate all data node in list.*/
#define FOR_ALL_DATA_IN_XML_LIST(_list, _node, _data) \
    FOR_ALL_DATA_IN_TYPEDEF_DLIST(_list, _node, _data)

/** @brief Enumerate all pData next node in list.*/
#define FOR_ALL_DATA_IN_XML_LIST_NEXT(_list, _node, _nextnode, _data) \
    FOR_ALL_DATA_IN_TYPEDEF_DLIST_NEXT(_list, _node, _nextnode, _data)

/** @brief Enumerate all pData previous node in list.*/
#define FOR_ALL_DATA_IN_XML_LIST_PREV(_list, _node, _prevnode, _data) \
    FOR_ALL_DATA_IN_TYPEDEF_DLIST_PREV(_list, _node, _prevnode, _data)

/** @brief XML list create. */
#define XML_LIST_CREATE(_list) \
    ZOS_TYPEDEF_DLIST_CREATEX(_list)

/** @brief XML list delete. */
#define XML_LIST_DELETE(_list) \
    ZOS_TYPEDEF_DLIST_DELETE(_list)

/** @brief XML list node init */
#define XML_LIST_NODE_INIT(_node, _data) \
    ZOS_TYPEDEF_DLIST_NODE_INIT(_node, _data)

/** @brief ZOS insert node before head in xml list */
#define XML_LIST_ADD2HEAD(_list, _node) \
    ZOS_TYPEDEF_DLIST_ADD2HEAD(_list, _node)

/** @brief ZOS insert node after tail in xml list */
#define XML_LIST_ADD2TAIL(_list, _node) \
    ZOS_TYPEDEF_DLIST_ADD2TAIL(_list, _node)

/** @brief ZOS insert node after previou node in xml list */
#define XML_LIST_INSERT(_list, _prevnode, _node) \
    ZOS_TYPEDEF_DLIST_INSERT(_list, _prevnode, _node)

/** @brief ZOS dequeue node from the first node in xml list */
#define XML_LIST_DEQUEUE(_list, _node) \
   ZOS_TYPEDEF_DLIST_DEQUEUE(_list, _node)

/** @brief ZOS remove one node */
#define XML_LIST_REMOVE(_list, _node) \
    ZOS_TYPEDEF_DLIST_REMOVE(_list, _node)

/** @brief XML list node data size */
#define XML_LIST_DATA_SIZE(_type) \
    (sizeof(_type) + sizeof(ST_XML_LIST_NODE))

/** @brief ZOS alloc memory of one xml list node */
#define XML_LIST_ALLOC_DATA(_sbufid, _type, _data) do { \
    _data = (_type *)XML_ALLOC_CLRD(_sbufid, XML_LIST_DATA_SIZE(_type)); \
    if (_data) \
    { \
        XML_LIST_NODE_INIT(_data, (ST_XML_LIST_NODE *)(_data) + 1); \
        _data = (_type *)((ST_XML_LIST_NODE *)(_data) + 1); \
    } \
} while (0)

/** @brief XML list append one node(alloced) with data */
#define XML_LIST_APPEND_NODE(_list, _data) \
     XML_LIST_ADD2TAIL(_list, XML_LIST_DATA_NODE(_data))

/** @brief ZOS remove one node(known by data) */
#define XML_LIST_REMOVE_DATA(_list, _data) \
     XML_LIST_REMOVE(_list, XML_LIST_DATA_NODE(_data))

/** @brief XML list find node data on index location */
#define XML_LIST_FIND_BY_INDEX(_list, _index, _node, _data) do { \
    ZOS_TYPEDEF_DLIST_FIND_BY_INDEX(_list, _index, _node); \
    _data = (_node) ? (_node)->pData : ZNULL; \
} while (0)

/** @brief XML qname set prefix */
#define XML_QNAME_PREFIX_SET(_qname, _prefix, _prefixlen) do { \
    (_qname)->pucPrefix = (ZUCHAR *)_prefix; \
    (_qname)->iPrefixLen = _prefixlen; \
} while (0)

/** @brief XML qname set prefix */
#define XML_QNAME_PREFIX_SETS(_qname, _prefix) do { \
    (_qname)->pucPrefix = (ZUCHAR *)_prefix; \
    (_qname)->iPrefixLen = ZOS_STR_LEN(_prefix); \
} while (0)

/** @brief XML qname set prefix */
#define XML_QNAME_PREFIX_SETU(_qname, _prefix, _prefixlen) do { \
    (_qname)->pucPrefix = _prefix; \
    (_qname)->iPrefixLen = _prefixlen; \
} while (0)

/** @brief XML qname set prefix */
#define XML_QNAME_PREFIX_SETUX(_qname, _srcstr) do { \
    (_qname)->pucPrefix = (_srcstr)->pucStr; \
    (_qname)->iPrefixLen = (_srcstr)->iLen; \
} while (0)

/** @brief XML qname set local */
#define XML_QNAME_LOCAL_SET(_qname, _local, _locallen) do { \
    (_qname)->pucLocal = (ZUCHAR *)_local; \
    (_qname)->iLocalLen = _locallen; \
} while (0)

/** @brief XML qname set local */
#define XML_QNAME_LOCAL_SETS(_qname, _local) do { \
    (_qname)->pucLocal = (ZUCHAR *)_local; \
    (_qname)->iLocalLen = ZOS_STR_LEN(_local); \
} while (0)

/** @brief XML qname set local */
#define XML_QNAME_LOCAL_SETU(_qname, _local, _locallen) do { \
    (_qname)->pucLocal = _local; \
    (_qname)->iLocalLen = _locallen; \
} while (0)

/** @brief XML qname set local */
#define XML_QNAME_LOCAL_SETUX(_qname, _srcstr) do { \
    (_qname)->pucLocal = (_srcstr)->pucStr; \
    (_qname)->iLocalLen = (_srcstr)->iLen; \
} while (0)

/** @brief XML qname set name */
#define XML_QNAME_SET(_qname, _prefix, _prefixlen, _local, _locallen) \
    XML_QNAME_PREFIX_SET(_qname, _prefix, _prefixlen); \
    XML_QNAME_LOCAL_SET(_qname, _local, _locallen)

/** @brief XML qname prefix and length */
#define XML_QNAME_NPREFIX(_qname) \
    (ZCHAR *)(_qname)->pucPrefix, (_qname)->iPrefixLen

/** @brief XML qname local part and length */
#define XML_QNAME_NLOCAL(_qname) \
    (ZCHAR *)(_qname)->pucLocal, (_qname)->iLocalLen

/** @brief XML qname compare prefix */
#define XML_QNAME_CMP_PREFIX(_qname1, _qname2) \
    Zos_NStrCmp(XML_QNAME_NPREFIX(_qname1), XML_QNAME_NPREFIX(_qname2)) 

/** @brief XML qname compare local */
#define XML_QNAME_CMP_LOCAL(_qname1, _qname2) \
    Zos_NStrCmp(XML_QNAME_NLOCAL(_qname1), XML_QNAME_NLOCAL(_qname2)) 

/** @brief XML qname prefix compare the string */
#define XML_QNAME_PREFIX_CMP(_qname, _str) \
    Zos_NStrCmp(XML_QNAME_NPREFIX(_qname), _str, ZOS_STR_LEN(_str))

/** @brief XML qname prefix compare the string */
#define XML_QNAME_PREFIX_NCMP(_qname, _str, _len) \
    Zos_NStrCmp(XML_QNAME_NPREFIX(_qname), _str, _len)

/** @brief XML qname prefix compare the string */
#define XML_QNAME_PREFIX_CMPUX(_qname, _usstr) \
    Zos_NStrCmp(XML_QNAME_NPREFIX(_qname), ZOS_NUSSTRX(_usstr))

/** @brief XML qname local part compare the string */
#define XML_QNAME_LOCAL_CMP(_qname, _str) \
    Zos_StrCmpN(_str, XML_QNAME_NLOCAL(_qname))

/** @brief XML qname local part compare the string */
#define XML_QNAME_LOCAL_NCMP(_qname, _str, _len) \
    Zos_NStrCmp(XML_QNAME_NLOCAL(_qname), _str, _len)

/** @brief XML qname local part compare the unsigned structure string */
#define XML_QNAME_LOCAL_CMPUX(_qname, _usstr) \
    Zos_NStrCmp(XML_QNAME_NLOCAL(_qname), ZOS_NUSSTRX(_usstr))

/** @brief XML set the unsigned structure string with prefix */
#define XML_USSTR_SET_PREFIX(_dststr, _qname) do { \
    (_dststr)->pucStr = (_qname)->pucPrefix; \
    (_dststr)->iLen = (_qname)->iPrefixLen; \
} while (0)

/** @brief XML set the unsigned structure string with prefix */
#define XML_USSTR_SET_LOCAL(_dststr, _qname) do { \
    (_dststr)->pucStr = (_qname)->pucLocal; \
    (_dststr)->iLen = (_qname)->iLocalLen; \
} while (0)

/**
 * @brief XML create a message.
 *
 * @param [out] ppstMsg  XML message structure.
 *
 * @retval ZOK Create successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreateX.
 */
ZFUNC ZINT Xml_MsgCreate(ST_XML_MSG **ppstMsg);

/**
 * @brief XML create a message.
 *
 * @param [in] iBufSize The buffer size.
 * @param [out] ppstMsg  XML message structure.
 *
 * @retval ZOK Create successful.
 * @retval ZFAILED Otherwise.
 * @see Zos_SbufCreateClrd.
 */
ZFUNC ZINT Xml_MsgCreateX(ZUINT iBufSize, ST_XML_MSG **ppstMsg);

/**
 * @brief XML delete a message.
 *
 * @param [in] pstMsg XML message structure.
 *
 * @retval ZOK Delete successful.
 * @retval ZFAILED Otherwise.
 * @see Zos_SbufDelete.
 */
ZFUNC ZINT Xml_MsgDelete(ST_XML_MSG *pstMsg);

/**
 * @brief XML load message from the string.
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag if the value is ture then  
 *             init the error information.
 * @param [out] ppstMsg First XML message.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreateX,XML_LOG_ERROR,Xml_MsgLoadX,Xml_MsgDelete.s
 */
ZFUNC ZINT Xml_MsgLoad(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG **ppstMsg);

/**
 * @brief XML load message from the string(dynamic copy string).
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag if the value is ture then  
 *             init the error information.
 * @param [out] ppstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreateX,XML_LOG_ERROR,Xml_MsgLoadX,Xml_MsgDelete.
 */
ZFUNC ZINT Xml_MsgLoadD(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG **ppstMsg);

/**
 * @brief XML load message from the string.
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag if the value is ture then  
 *             init the error information.
 * @param [in] pstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DecodeInit,XML_LOG_ERROR.
 */
ZFUNC ZINT Xml_MsgLoadX(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG *pstMsg);

/**
 * @brief XML load message from the string(dynamic copy string).
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag if the value is ture then 
 *             init the error information.
 * @param [in] pstMsg XML message structure.
 * @param [in] bRefAsData Flag for reference as data.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DecodeInitD,XML_LOG_ERROR,Xml_DecodeMsg.
 */
ZFUNC ZINT Xml_MsgLoadDX(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG *pstMsg, ZBOOL bRefAsData);
/**
 * @brief XML load the string by sax action.
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag if the value is ture then  
 *             init the error information.
 * @param [in] bPipeBuf A flag if the value is ture then create the pipe buffer.
 * @param [out] pSaxCtx Sax context,to return the string.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DecodeInitD,XML_LOG_ERROR,Xml_DecodeMsg.
 */
ZFUNC ZINT Xml_MsgLoadSax(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ZBOOL bPipeBuf, ZVOID *pSaxCtx);

/**
 * @brief XML load element from the string.
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag if the value is ture then  
 *             init the error information.
 * @param [out] ppstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreate.
 */
ZFUNC ZINT Xml_MsgLoadElem(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG **ppstMsg);

/**
 * @brief XML load element from the string(dynamic copy string).
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag if the value is ture then  
 *             init the error information.
 * @param [out] ppstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreate.
 */
ZFUNC ZINT Xml_MsgLoadElemD(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG **ppstMsg);

/**
 * @brief XML load element from fragment xml body string
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag ,if the value is ture then  
 *             init the error information.
 * @param [in] pstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreate.
 */
ZFUNC ZINT Xml_MsgLoadElemF(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG *pstMsg);
				
/**
 * @brief XML load element from the string.
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag ,if the value is ture then  
 *             init the error information.
 * @param [in] pstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreate.
 */
ZFUNC ZINT Xml_MsgLoadElemX(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG *pstMsg);

/**
 * @brief XML load element from the string(dynamic copy string).
 *
 * @param [in] pstStr A string structure,it decides the buffer size.
 * @param [in] bErrInfo A flag, if the value is ture then  
 *             init the error information.
 * @param [in] pstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Xml_MsgLoadElemDX(ST_ZOS_USTR *pstStr, ZBOOL bErrInfo, 
                ST_XML_MSG *pstMsg);

/**
 * @brief XML load message from the specified file.
 *
 * @param [in] pcFileName The file name,it decides that where to load the message.
 * @param [in] bErrInfo A flag, if the value is ture then  
 *             init the error information.
 * @param [out] ppstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreate,Xml_MsgLoadFileX,Xml_MsgDelete.
 */
ZFUNC ZINT Xml_MsgLoadFile(ZCONST ZCHAR *pcFileName, ZBOOL bErrInfo, 
                ST_XML_MSG **ppstMsg);

/**
 * @brief XML load message from the specified file.
 *
 * @param [in] pcFileName The file name,it decides that where to load the message.
 * @param [in] bErrInfo A flag, if the value is ture then  
 *             init the error information.
 * @param [in] pstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgLoadDX.
 */
ZFUNC ZINT Xml_MsgLoadFileX(ZCONST ZCHAR *pcFileName, ZBOOL bErrInfo, 
                ST_XML_MSG *pstMsg);

/**
 * @brief XML load message from the specified file with reference as data.
 *
 * @param [in] pcFileName The file name,it decides that where to load the message.
 * @param [in] bErrInfo A flag, if the value is ture then  
 *             init the error information.
 * @param [out] ppstMsg XML message structure.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_MsgCreate,Xml_MsgLoadFileX,Xml_MsgDelete.
 */
ZFUNC ZINT Xml_MsgLoadFileR(ZCONST ZCHAR *pcFileName, ZBOOL bErrInfo, 
                ST_XML_MSG **ppstMsg);

/**
 * @brief XML save message into message buffer.
 *
 * @param [in] pstMsg XML message structure.
 * @param [in] bErrInfo A flag, if the value is ture then  
 *             init the error information.
 * @param [in] bCompactFmt A flag,if the value is ture then  
 *             it's the compact format.
 * @param [in] bIndentFmt A flag,if the value is ture then  
 *             it's the indent format.
 * @param [out] pzMsgBuf The data message buffer, to return the message.
 *
 * @retval ZOK Save successful.
 * @retval ZFAILED Otherwise.
 * @see XML_CREATE_MSGBUF,XML_DBUF_DELETE,Xml_MsgSaveX.
 */
ZFUNC ZINT Xml_MsgSave(ST_XML_MSG *pstMsg, ZBOOL bErrInfo, ZBOOL bCompactFmt,
                ZBOOL bIndentFmt, ZDBUF *pzMsgBuf);

/**
 * @brief XML save message into message buffer.
 *
 * @param [in] pstMsg XML message structure.
 * @param [in] bErrInfo A flag, if the value is ture then  
 *             init the error information.
 * @param [in] bCompactFmt A flag,if the value is ture then  
 *             it's the compact format.
 * @param [in] bIndentFmt A flag,if the value is ture then  
 *             it's the indent format..
 * @param [in] zMsgBuf The data message buffer.
 *
 * @retval ZOK Save successful.
 * @retval ZFAILED Otherwise.
 * @see XML_CREATE_MSGBUF,XML_DBUF_DELETE,Xml_MsgSaveX.
 */
ZFUNC ZINT Xml_MsgSaveX(ST_XML_MSG *pstMsg, ZBOOL bErrInfo, ZBOOL bCompactFmt,
                ZBOOL bIndentFmt, ZDBUF zMsgBuf);

/**
 * @brief XML load message from the specified file.
 *
 * @param [in] pstMsg XML message structure.
 * @param [in] bErrInfo A flag, if the value is ture then  
 *             init the error information.
 * @param [in] bCompactFmt A flag,if the value is ture then  
 *             it's the compact format.
 * @param [in] bIndentFmt A flag,if the value is ture then  
 *             it's the indent format.
 * @param [in] pcFileName The file name.
 *
 * @retval ZOK Load successful.
 * @retval ZFAILED Otherwise.
 * @see XML_DBUF_DELETE,Xml_MsgSaveX.
 */
ZFUNC ZINT Xml_MsgSaveFile(ST_XML_MSG *pstMsg, ZBOOL bErrInfo, 
                ZBOOL bCompactFmt, ZBOOL bIndentFmt, ZCHAR *pcFileName);
/**
 * @brief XML buffer message initialize empty file if no file exist.
 *
 * @param [in] pcFileName The file name.
 * @param [in] bUtf8Fmt A flag, if the value is ture then  
 *             the type is utf-8 format.
 * @param [in] pcRootName The root name.
 *
 * @retval ZOK Initialize successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddHdr,Xml_BufMsgAddElemX.
 */
ZFUNC ZINT Xml_BufMsgInitFile(ZCHAR *pcFileName, ZBOOL bUtf8Fmt,
                ZCHAR *pcRootName);
/**
 * @brief XML buffer message add header.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] bUtf8Fmt A flag, if the value is ture then  
 *             the type is utf-8 format.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Xml_BufMsgAddHdr(ZDBUF zDbuf, ZBOOL bUtf8Fmt);

/**
 * @brief XML buffer message add header.
 *
 * @param [in] bUtf8Fmt A flag, if the value is ture then  
 *             the type is utf-8 format.
 * @param [out] pzData The Zos data buffer, to return the header.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddHdr
 */
ZFUNC ZINT Xml_BufMsgAddHdrX(ZBOOL bUtf8Fmt, ZDBUF *pzData);

/**
 * @brief XML buffer message add an element.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcName The element name.
 * @param [in] iVal The element value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddElemX
 */
ZFUNC ZINT Xml_BufMsgAddElem(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcName, ZUINT iVal);

/**
 * @brief XML buffer message add an element.
 *s
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcName The element name.
 * @param [in] pcVal The element value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddElem
 */
ZFUNC ZINT Xml_BufMsgAddElemX(ZDBUF zData, ZUINT iDeep, 
                ZCHAR *pcName, ZCONST ZCHAR *pcVal);

/**
 * @brief XML buffer message add an element start.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcName The element start's name.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddElemStartAttr
 */
ZFUNC ZINT Xml_BufMsgAddElemStart(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcName);

/**
 * @brief XML buffer message add an element start and attribute.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name.
 * @param [in] pcAttrName The element attribute name.
 * @param [in] iAttrVal The element attribute value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddElemStart.
 */
ZFUNC ZINT Xml_BufMsgAddElemStartAttr(ZDBUF zDbuf, 
                ZUINT iDeep, ZCHAR *pcElemName, ZCHAR *pcAttrName, 
                ZUINT iAttrVal);

/**
 * @brief XML buffer message add an element start and attribute.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name..
 * @param [in] pcAttrName The element attribute name.
 * @param [in] pcAttrVal The element attribute value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddElemStartAttr.
 */
ZFUNC ZINT Xml_BufMsgAddElemStartAttrX(ZDBUF zDbuf, 
                ZUINT iDeep, ZCHAR *pcElemName, ZCHAR *pcAttrName, 
                ZCHAR *pcAttrVal);

/**
 * @brief XML buffer message add an element start and attribute list .
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name..
 * @param [in] pcFormat The type of format.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_BufMsgAddElemStartAttrX.
 */
ZFUNC ZINT Xml_BufMsgAddElemStartAttrLst(ZDBUF zDbuf, 
                ZUINT iDeep, ZCHAR *pcElemName, const ZCHAR *pcFormat, ...) ZATTR_PRINTF(4,5);

/**
 * @brief XML buffer message add an element end.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcName The name.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Xml_BufMsgAddElemEnd(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcName);

/**
 * @brief XML buffer message add an element and attribute.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name.
 * @param [in] iElemVal The element value.
 * @param [in] pcAttrName The attribute name.
 * @param [in] iAttrVal The attribute value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see xml_BufMsgAddElem.
 */
ZFUNC ZINT Xml_BufMsgAddElemAttr(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcElemName, ZUINT iElemVal, ZCHAR *pcAttrName,
                ZUINT iAttrVal);

/**
 * @brief XML buffer message add an element and attribute.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name.
 * @param [in] iElemVal The element value.
 * @param [in] pcAttrName The attribute name.
 * @param [in] pcAttrVal The attribute value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see xml_BufMsgAddElem.
 */
ZFUNC ZINT Xml_BufMsgAddElemAttrD(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcElemName, ZUINT iElemVal, ZCHAR *pcAttrName,
                ZCHAR *pcAttrVal);

/**
 * @brief XML buffer message add an element and attribute.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name.
 * @param [in] pcElemVal The element value.
 * @param [in] pcAttrName The attribute name.
 * @param [in] pcAttrVal The attribute value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see xml_BufMsgAddElemX.
 */
ZFUNC ZINT Xml_BufMsgAddElemAttrX(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcElemName, ZCHAR *pcElemVal, ZCHAR *pcAttrName, 
                ZCHAR *pcAttrVal);

/**
 * @brief XML buffer message add an element and attribute.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name.
 * @param [in] pcElemVal The element value.
 * @param [in] pcAttrName The attribute name.
 * @param [in] iAttrVal The attribute value.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see xml_BufMsgAddElemX.
 */
ZFUNC ZINT Xml_BufMsgAddElemAttrXD(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcElemName, ZCHAR *pcElemVal, ZCHAR *pcAttrName, 
                ZUINT iAttrVal);

/**
 * @brief XML buffer message add an element and attribute list.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name.
 * @param [in] iElemVal The element value.
 * @param [in] pcFormat The type of format.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see xml_BufMsgAddElem.
 */
ZFUNC ZINT Xml_BufMsgAddElemAttrLst(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcElemName, ZUINT iElemVal, 
                const ZCHAR *pcFormat, ...) ZATTR_PRINTF(5,6);

/**
 * @brief XML buffer message add an element and attribute list.
 *
 * @param [in] zDbuf The Zos data buffer.
 * @param [in] iDeep The deep,for get the indent format.
 * @param [in] pcElemName The element name.
 * @param [in] pcElemVal The element value.
 * @param [in] pcFormat The type of format.
 *
 * @retval ZOK Add successful.
 * @retval ZFAILED Otherwise.
 * @see xml_BufMsgAddElemX.
 */
ZFUNC ZINT Xml_BufMsgAddElemAttrLstX(ZDBUF zDbuf, ZUINT iDeep, 
                ZCHAR *pcElemName, ZCHAR *pcElemVal, 
                const ZCHAR *pcFormat, ...) ZATTR_PRINTF(5,6);

/**
 * @brief XML get the attribute from attribute list.
 *
 * @param [in] pstAttrLst XML attribute list.
 * @param [in] pcName To get the attribute name.
 * @param [out] ppstAttr XML attribute list,to return the list.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_AttrLstGetAttrX
 */
ZFUNC ZINT Xml_AttrLstGetAttr(ST_XML_ATTR_LST *pstAttrLst, ZCHAR *pcName, 
                ST_XML_ATTR **ppstAttr);

/**
 * @brief XML get the attribute from attribute list.
 *
 * @param [in] pstAttrLst  XML attribute list.
 * @param [in] pstName To get the attribute name.
 * @param [out] ppstAttr XML attribute structure,to return the attribute .
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Xml_AttrLstGetAttrX(ST_XML_ATTR_LST *pstAttrLst, 
                ST_ZOS_USTR *pstName, ST_XML_ATTR **ppstAttr);

/**
 * @brief XML get the attribute value from attribute list.
 *
 * @param [in] pstAttrLst XML attribute list.
 * @param [in] pcName To get the attribute name.
 * @param [out] ppstVal To return the attribute value.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_AttrLstGetAttr
 */
ZFUNC ZINT Xml_AttrLstGetAttrVal(ST_XML_ATTR_LST *pstAttrLst, 
                ZCHAR *pcName, ST_ZOS_USTR **ppstVal);

/**
 * @brief XML get the attribute value from attribute list.
 *
 * @param [in] pstAttrLst  XML attribute list.
 * @param [in] pstName To get the attribute name.
 * @param [out] ppstVal To return the attribute alue.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_AttrLstGetAttrX
 */
ZFUNC ZINT Xml_AttrLstGetAttrValX(ST_XML_ATTR_LST *pstAttrLst, 
                ST_ZOS_USTR *pstName, ST_ZOS_USTR **ppstVal);

/**
 * @brief XML check message header.
 *
 * @param [out] pcStr To get the value of xml message.
 * @param [in] iLen The length of xml message.
 *
 * @retval ZOK Check successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_ChkTypeStr
 */
ZFUNC ZINT Xml_ChkMsgHdr(ZCHAR *pcStr, ZUINT iLen);

/**
 * @brief XML check PubidChar.
 *
 * @param [out] pcStr To get the value of xml ucs ascii character table.
 * @param [in] iLen The length of xml ucs ascii character table,the max is 256.
 *
 * @retval ZOK Check successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_ChkTypeStr
 */
ZFUNC ZINT Xml_ChkPubidChar(ZCHAR *pcStr, ZUINT iLen);

/**
 * @brief XML check normalized data string.
 *
 * @param [in] zMemBuf The memory buffer.
 * @param [in] pstOldStr The old data string.
 * @param [out] ppstNewStr The new normialized data string.
 *  Otherwise ppstNewStr will be allocated in zMemBuf.
 *
 * @note If old string is normialized data string, 
 *  ppstNewStr will be set by ZNULL and return ZOK;
 *
 * @retval ZOK Check successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkNormalizedStrX
 */
ZFUNC ZINT Xml_DataChkNormalizedStr(ZUBUF zMemBuf, 
                ST_ZOS_USTR *pstOldStr, ST_ZOS_USTR **ppstNewStr);

/**
 * @brief XML check normalized data string.
 *
 * @param [in] pcOldStr The old data string.
 * @param [out] ppcNewStr The new normialized data string.
 *
 * @note If old string is normialized data string, 
 *  ppucNewStr will be set by ZNULL and return ZOK.
 *  Otherwise ppucNewStr will be allocated by Zos_SysStrAlloc,
 *  user should free it by Zos_SysStrFree.
 *
 * @retval ZOK Check successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkNormalizedStr
 */
ZFUNC ZINT Xml_DataChkNormalizedStrX(ZCONST ZCHAR *pcOldStr, 
                ZCHAR **ppcNewStr);

/**
 * @brief XML check normalized data length.
 *
 * @param [in] pstStr The data string.
 * @param [out] piLen The normialized data length.
 *
 * @retval ZOK Check length successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkNormalizedLenX
 */
ZFUNC ZINT Xml_DataChkNormalizedLen(ST_ZOS_USTR *pstStr, 
                ZUINT *piLen);

/**
 * @brief XML check normalized data length.
 *
 * @param [in] pcStr The data string.
 * @param [out] piLen The normialized data length.
 *
 * @retval ZOK Check length successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkNormalizedLen
 */
ZFUNC ZINT Xml_DataChkNormalizedLenX(ZCONST ZCHAR *pcStr, 
                ZUINT *piLen);

/**
 * @brief XML check escaped data string.
 *
 * @param [in] zMemBuf The memory buffer.
 * @param [in] pstOldStr The old data string.
 * @param [out] ppstNewStr The new escaped data string.
 *  Otherwise ppstNewStr will be allocated in zMemBuf.
 *
 * @note If old string is escaped data string, 
 *  ppstNewStr will be set by ZNULL and return ZOK;
 *
 * @retval ZOK Check successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkEscapedStrX
 */
ZFUNC ZINT Xml_DataChkEscapedStr(ZUBUF zMemBuf, 
                ST_ZOS_USTR *pstOldStr, ST_ZOS_USTR **ppstNewStr);

/**
 * @brief XML check escaped data string.
 *
 * @param [in] pcOldStr The old data string.
 * @param [out] ppcNewStr The new escaped data string.
 *
 * @note If old string is escaped data string, 
 *  ppucNewStr will be set by ZNULL and return ZOK.
 *  Otherwise ppucNewStr will be allocated by Zos_SysStrAlloc,
 *  user should free it by Zos_SysStrFree.
 *
 * @retval ZOK Check successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkEscapedStr
 */
ZFUNC ZINT Xml_DataChkEscapedStrX(ZCONST ZCHAR *pcOldStr, 
                ZCHAR **ppcNewStr);

/**
 * @brief XML check escaped data length.
 *
 * @param [in] pstStr The data string.
 * @param [out] pwLen The escaped data length.
 *
 * @retval ZOK Check length successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkEscapedLenX
 */
ZFUNC ZINT Xml_DataChkEscapedLen(ST_ZOS_USTR *pstStr, 
                ZUINT *piLen);

/**
 * @brief XML check escaped data length.
 *
 * @param [in] pcStr The data string.
 * @param [out] pwLen The normialized data length.
 *
 * @retval ZOK Check length successful.
 * @retval ZFAILED Otherwise.
 * @see Xml_DataChkEscapedLen
 */
ZFUNC ZINT Xml_DataChkEscapedLenX(ZCONST ZCHAR *pcStr, 
                ZUINT *piLen);

/**
 * @brief XML copy normalized data string.
 *
 * @param [in] zMemBuf The memory buffer.
 * @param [in] pstStr The data string.
 * @param [out] ppcNormStr The normialized data string.
 *
 * @retval ZOK Copy data string successful, otherwise return ZFAILED.
 * @see Xml_DataCpyEscapedStrX
 */
ZFUNC ZINT Xml_DataCpyNormalizedStr(ZUBUF zMemBuf, 
                ST_ZOS_USTR *pstStr, ZCHAR **ppcNormStr);
/**
 * @brief XML copy normalized data string.
 *
 * @param [in] pcStr The data string.
 * @param [out] ppcNormStr The normialized data string.
 *  User should free it by Zos_SysStrFree.
 *
 * @retval ZOK Copy data string successful, otherwise return ZFAILED.
 * @see Xml_DataCpyEscapedStr
 */
ZFUNC ZINT Xml_DataCpyNormalizedStrX(ZCONST ZCHAR *pcStr, 
                ZCHAR **ppcNormStr);

/**
 * @brief XML copy escaped data string.
 *
 * @param [in] zMemBuf The memory buffer.
 * @param [in] pstStr The data string.
 * @param [out] ppcEscaStr The escaped data string.
 *
 * @retval ZOK Copy data string successful, otherwise return ZFAILED.
 * @see Xml_DataCpyNormalizedStr
 */
ZFUNC ZINT Xml_DataCpyEscapedStr(ZUBUF zMemBuf, 
                ST_ZOS_USTR *pstStr, ZCHAR **ppcEscaStr);

/**
 * @brief XML copy escaped data string.
 *
 * @param [in] pcStr The data string.
 * @param [out] ppcEscaStr The escaped data string.
 *  User should free it by Zos_SysStrFree.
 *
 * @retval ZOK Copy data string successful, otherwise return ZFAILED.
 * @see Xml_DataCpyNormalizedStrX
 */
ZFUNC ZINT Xml_DataCpyEscapedStrX(ZCONST ZCHAR *pcStr, 
                ZCHAR **ppcEscaStr);

#ifdef __cplusplus
}
#endif

#endif /* _XML_UTIL_H__ */

