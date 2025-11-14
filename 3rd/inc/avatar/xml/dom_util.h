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
  File name     : dom_util.h
  Module        : dom utility
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Data structure and function definitions required by xml dom utility
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DOM_UTIL_H__
#define _DOM_UTIL_H__
/** 
 * @file dom_util.h
 * @brief XML DOM utility interfaces.
 *
 * @see 
 */

#ifdef __cplusplus
extern "C"{
#endif

#ifdef XML_SUPT_API_DOM /** @brief Is support dom */    

/**
 * @brief Section get the section key size.
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [out] piSize To return the section key size. 
 *
 * @retval ZOK Get the size successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_NodeGetChilds.
 */
ZFUNC ZINT Dom_SecGetSize(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZINT *piSize);

/**
 * @brief Section get the section key element.
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [out] ppstKeyElem Return the section key element node.
 *
 * @retval ZOK Get the key element successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_NodeGetChilds.
 */
ZFUNC ZINT Dom_SecGetKey(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_DOM_ELEM **ppstKeyElem);
/**
 * @brief Section get the section key element by index .
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [in] iIdx The index of section key.
 * @param [out] ppstKeyElem Return the section key element Node.
 *
 * @retval ZOK Get the key element successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_NodeGetChilds.
 */
ZFUNC ZINT Dom_SecGetKeyX(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZINT iIdx, ST_DOM_ELEM **ppstKeyElem);
/**
 * @brief Section get the key element by index.
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [out] ppstNodeLst Return the Element Node list.
 *
 * @retval ZOK Get the key element successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_ElemGetElemsByTagName.
 */
ZFUNC ZINT Dom_SecGetKeyS(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_DOM_NODE_LST **ppstNodeLst);


 /**
 * @brief Dom get section key value node.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [out] ppstNode Return the section key value node.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_NodeGetFirstChild
 */
ZFUNC ZINT Dom_SecGetVal(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_DOM_NODE **ppstNode);
 /**
 * @brief Dom get section key value node by index.
 * @code
 * <section>
 * <key>value1</key>
 * <key>value2</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [in] iIdx The index of section key.
 * @param [out] ppstNode Return the section key value node.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKeyX,Dom_NodeGetFirstChild.
 */
ZFUNC ZINT Dom_SecGetValX(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZINT iIdx, ST_DOM_NODE **ppstNode);
 /**
 * @brief Dom get section key value of string.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [out] pcDftStr To get the value with char type.
 * @param [out] pcDstStr To get the value with string type.
 * @param [in] wDstSize The max size.
 *s
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_KeyGetStr.
 */
ZFUNC ZINT Dom_SecGetStr(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZCHAR *pcDftStr, ZCHAR *pcDstStr, ZUSHORT wDstSize);
 
 /**
 * @brief Dom get section key value of string.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [out] ppstVal To return the value of string type.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_KeyGetStrx.
 */
ZFUNC ZINT Dom_SecGetStrX(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_ZOS_USTR **ppstVal);
/**
 * @brief Dom get section key value of unsign long.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [in] iDftVal The key value.
 * @param [out] piVal To return the value of unsign long.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_KeyGetUl.
 */
ZFUNC ZINT Dom_SecGetUint(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUINT iDftVal, ZUINT *piVal);

/**
 * @brief Dom get section key value of unsign short.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [in] wDftVal The key value.
 * @param [out] pwVal To return the value of unsign short.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_KeyGetUs.
 */
ZFUNC ZINT Dom_SecGetUs(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUSHORT wDftVal, ZUSHORT *pwVal);


/**
 * @brief Dom get section key value of unsign char.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [in] ucDftVal The key value.
 * @param [out] pucVal To return the value of char.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_KeyGetUc.
 */
ZFUNC ZINT Dom_SecGetUc(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUCHAR ucDftVal, ZUCHAR *pucVal);

 /**
 * @brief Dom get section key value of boolean.
 * @code
 * <section>
 * <key>value(0 or 1)</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [in] bDftTrueFalse The key value (0 or 1).
 * @param [out] pbTrueFalse To return the value of boolean(0 or 1).
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_KeyGetBool.
 */
ZFUNC ZINT Dom_SecGetBool(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);
 
 /**
 * @brief Dom get section key value of boolean.
 * @code
 * <section>
 * <key>value(true or false)</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The Key name of section.
 * @param [in] bDftTrueFalse The key value (TRUE or FALSE).
 * @param [out] pbTrueFalse To return the value of boolean(TRUE or FALSE).
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_SecGetKey,Dom_KeyGetBool.
 */
ZFUNC ZINT Dom_SecGetBoolX(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);

 /**
 * @brief Dom get key value node.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [out] ppstNode Return the key value node.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal.
 */
ZFUNC ZINT Dom_KeyGetVal(ST_DOM_ELEM *pstKeyElem, ST_DOM_NODE **ppstNode);
 
 /**
 * @brief Dom get key value CharacterData.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [out] ppstData Return the Character Data node.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal.
 */
ZFUNC ZINT Dom_KeyGetValX(ST_DOM_ELEM *pstKeyElem, ST_DOM_CDATA **ppstData);

 /**
 * @brief Dom key value of string.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcDftStr The value of the key.
 * @param [out] pcDstStr Get the value.
 * @param [in] wDstSize The size of value string.
 *s
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal,Dom_NodeGetValue.
 */
ZFUNC ZINT Dom_KeyGetStr(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcDftStr, 
                ZCHAR *pcDstStr, ZUSHORT wDstSize);
 /**
 * @brief Dom get key value of string.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [out] ppstVal To return the value of string.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal,Dom_NodeGetValue.
 */
ZFUNC ZINT Dom_KeyGetStrX(ST_DOM_ELEM *pstKeyElem, 
                ST_ZOS_USTR **ppstVal);

/**
 * @brief Dom get key value of unsign long.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] iDftVal The value of the key element.
 * @param [out] piVal To return the value of unsign long.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal,Dom_NodeGetValue.
 */
ZFUNC ZINT Dom_KeyGetUi(ST_DOM_ELEM *pstKeyElem, ZUINT iDftVal, 
                ZUINT *piVal);
/**
 * @brief Dom get key value of unsign short.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] wDftVal The value of the key.
 * @param [out] pwVal To return the value of unsign short.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal,Dom_NodeGetValue.
 */
ZFUNC ZINT Dom_KeyGetUs(ST_DOM_ELEM *pstKeyElem, ZUSHORT wDftVal, 
                ZUSHORT *pwVal);

/**
 * @brief Dom get key value of unsign char.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] ucDftVal The value of the key.
 * @param [out] pucVal To return the value of unsign char.
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal,Dom_NodeGetValue.
 */
ZFUNC ZINT Dom_KeyGetUc(ST_DOM_ELEM *pstKeyElem, ZUCHAR ucDftVal, 
                ZUCHAR *pucVal);

/**
 * @brief Dom get key value of boolean.
 * @code
 * <key>value(0 or 1)</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] bDftTrueFalse a value(0 or 1).
 * @param [out] pbTrueFalse To return the value(0 or 1).
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal,Dom_NodeGetValue..
 */
ZFUNC ZINT Dom_KeyGetBool(ST_DOM_ELEM *pstKeyElem, ZBOOL bDftTrueFalse, 
                ZBOOL *pbTrueFalse);

 /**
 * @brief Dom get key value of boolean.
 * @code
 * <key>value(true or false)</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] bDftTrueFalse a value(true or false).
 * @param [out] pbTrueFalse To return the value(true or false).
 *
 * @retval ZOK Get the value successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_KeyGetVal,Dom_NodeGetValue.
 */
ZFUNC ZINT Dom_KeyGetBoolX(ST_DOM_ELEM *pstKeyElem, ZBOOL bDftTrueFalse, 
                ZBOOL *pbTrueFalse);
 
/**
 * @brief Dom get key attribute check following exist.
 * @code
 * <key attr></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [out] pbExist Return a flag,if the value is true 
 *              then the attribute exist or not exist. 
 *
 * @retval ZOK Check successful.
 * @retval ZFAILED Otherwise.
 * @see Dom_ElemGetAttr.
 */

ZFUNC ZINT Dom_KeyChkAttr(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL *pbExist);

/**
 * @brief Dom get key attribute string.
 * @code
 * <key attr></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [out] ppstVal Return the value of key attribute string. 
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemGetAttr.
 */
ZFUNC ZINT Dom_KeyGetAttr(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ST_ZOS_USTR **ppstVal);
/**
 * @brief Dom get key attribute string.
 * @code
 * <key attr=value></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [out] pcVal Return the value of key attribute string. 
 * @param [in]  wMaxSize The max size.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemGetAttr.
 */
ZFUNC ZINT Dom_KeyGetAttrStr(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZCHAR *pcVal, ZUSHORT wMaxSize);

/**
 * @brief Dom get key attribute of unsigned long.
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [in] iDftVal The value of the key attribute. 
 * @param [out] piVal Return the value of Unsigned long.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemGetAttr.
 */
ZFUNC ZINT Dom_KeyGetAttrUi(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUINT iDftVal, ZUINT *piVal);

/**
 * @brief Dom get key attribute of unsigned short.
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [in] wDftVal The value of the key attribute.
 * @param [out] pwVal Return the value of Unsigned short.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemGetAttr.
 */
ZFUNC ZINT Dom_KeyGetAttrUs(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUSHORT wDftVal, ZUSHORT *pwVal);

/**
 * @brief Dom get key attribute of unsigned char.
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [in] ucDftVal The value of the key attribute. 
 * @param [out] pucVal  Return the value of Unsigned char.
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemGetAttr.
 */
ZFUNC ZINT Dom_KeyGetAttrUc(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUCHAR ucDftVal, ZUCHAR *pucVal);

/**
 * @brief Dom get key attribute of bool(0 or 1).
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [in] bDftTrueFalse a value(0 or 1). 
 * @param [out] pbTrueFalse Return athe value(0 or 1).
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemGetAttr.
 */
ZFUNC ZINT Dom_KeyGetAttrBool(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);

/**
 * @brief Dom get key attribute of bool(true or false).
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The name of the key element.
 * @param [in] bDftTrueFalse a value(true or false).
 * @param [out] pbTrueFalse Return the value(true or false).
 *
 * @retval ZOK Get successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemGetAttr.
 */
ZFUNC ZINT Dom_KeyGetAttrBoolX(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);

/**
 * @brief Dom put document root element.
 *
 * @param [in] pstDoc A document node.
 * @param [in] pcRootName The root name.
 * @param [out] ppstRootElem Return a root Element Node.
 *
 * @retval ZOK put successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_DocCreateElem.
 */
ZFUNC ZINT Dom_DocPutRoot(ST_DOM_DOC *pstDoc, ZCHAR *pcRootName,
                ST_DOM_ELEM **ppstRootElem);

/**
 * @brief Dom put section key element.
 * @code
 * <section><key/></section>
 * @endcode.
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The key name.
 * @param [out] ppstKeyElem Return a section key Element Node.
 *
 * @retval ZOK put the key element successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_DocCreateElem,Dom_NodeAppendChild.
 */
ZFUNC ZINT Dom_SecPutKey(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_DOM_ELEM **ppstKeyElem);

 /**
 * @brief Dom set section key value of string.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The key name.
 * @param [out] pcValStr Return the value of string.
 *
 * @retval ZOK Set the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_SecPutKey,Dom_KeyPutVal.
 */
ZFUNC ZINT Dom_SecSetStr(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZCHAR *pcValStr);

/**
 * @brief Dom set section key value unsign long
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The key name.
 * @param [in] iVal Return the value of Unsigned long.
 *
 * @retval ZOK Set the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_SecPutKey,Dom_keySecUSI.
 */
ZFUNC ZINT Dom_SecSetUi(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUINT iVal);

 /**
 * @brief Dom set section key value unsign short
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The key name.
 * @param [in] wVal Return the value of Unsigned short.
 *
 * @retval ZOK Set the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_SecPutKey,Dom_keySecUs.
 */
ZFUNC ZINT Dom_SecSetUs(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUSHORT wVal);

 /**
 * @brief Dom set section key value unsign char.
 * @code
 * <section>
 * <key>value</key>
 * </section>
 * @endcode
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The key name.
 * @param [in] ucVal Return the value of Unsigned Char.
 *
 * @retval ZOK Set the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_SecPutKey,Dom_keySecUc.
 */
ZFUNC ZINT Dom_SecSetUc(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUCHAR ucVal);

/**
 * @brief Dom set section key of boolean
 * @code
 * <section>
 * <key>value(0 or 1)</key>
 * </section>
 * @endcode.
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The key name.
 * @param [in] bTrueFalse A value(0 or 1).
 *
 * @retval ZOK Set the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_SecPutKey,Dom_keySecBool.
 */
ZFUNC ZINT Dom_SecSetBool(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bTrueFalse);

/**
 * @brief Dom set section key of boolean
 * @code
 * <section>
 * <key>value(true or false)</key>
 * </section>
 * @endcode.
 *
 * @param [in] pstSecElem A section element node.
 * @param [in] pcKeyName The key name.
 * @param [in] bTrueFalse A value(true or false).
 *
 * @retval ZOK Set the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_KeyGetValX,Dom_CDataReplace.
 */
ZFUNC ZINT Dom_SecSetBoolX(ST_DOM_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bTrueFalse);

/**
 * @brief Dom put key element value.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcVal A key value.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_SecPutKey,Dom_keySecBoolx,Dom_NodeRmvChild,
 *      Dom_DocCreateTxt,Dom_NodeAppendChild.
 */
ZFUNC ZINT Dom_KeyPutVal(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcVal);

/**
 * @brief Dom set key value of unsign long.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] iVal A key value of Unsigned long.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_KeyPutVal.
 */
ZFUNC ZINT Dom_KeySetUi(ST_DOM_ELEM *pstKeyElem, ZUINT iVal);

/**
 * @brief Dom set key value of unsign short .
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] wVal A key value of Unsigned short.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_KeyPutVal.
 */
ZFUNC ZINT Dom_KeySetUs(ST_DOM_ELEM *pstKeyElem, ZUSHORT wVal);

/**
 * @brief Dom set key value of unsign char.
 * @code
 * <key>value</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] ucVal A key value of Unsigned char.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_KeyPutVal.
 */
ZFUNC ZINT Dom_KeySetUc(ST_DOM_ELEM *pstKeyElem, ZUCHAR ucVal);

/**
 * @brief Dom set key value of boolean.
 * @code
 * <key>value(0 or 1)</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] bTrueFalse A key value of boolean(0 or 1).
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_KeyPutVal.
 */
ZFUNC ZINT Dom_KeySetBool(ST_DOM_ELEM *pstKeyElem, ZBOOL bTrueFalse);

/**
 * @brief Dom set key value of boolean.
 * @code
 * <key>value(true or false)</key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] bTrueFalse A key value of boolean(true or false).
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_KeyPutVal.
 */
 
ZFUNC ZINT Dom_KeySetBoolX(ST_DOM_ELEM *pstKeyElem, ZBOOL bTrueFalse);

/**
 * @brief Dom set key attribute of string.
 * @code
 * <key attr="value"></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The key name .
 * @param [in] pcVal A attribute value of string.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemSetAttr.
 */
ZFUNC ZINT Dom_KeySetAttrStr(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZCHAR *pcVal);

/**
 * @brief Dom set key attribute of unsigned long.
 * @code
 * <key attr="value"></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The key name.
 * @param [in] iVal A attribute value of Unsigned long.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemSetAttr.
 */
ZFUNC ZINT Dom_KeySetAttrUi(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUINT iVal);
/**
 * @brief Dom set key attribute of unsigned short.
 * @code
 * <key attr="value"></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The key name.
 * @param [in] wVal A attribute value of Unsigned short.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemSetAttr.
 */
ZFUNC ZINT Dom_KeySetAttrUs(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUSHORT wVal);

/**
 * @brief Dom set key attribute of unsigned char.
 * @code
 * <key attr="value"></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The key name.
 * @param [in] ucVal A attribute value of Unsigned char.
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemSetAttr.
 */
ZFUNC ZINT Dom_KeySetAttrUc(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUCHAR ucVal);
/**
 * @brief Dom set key attribute of boolean.
 * @code
 * <key attr="value(0 or 1)"></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The key name.
 * @param [in] bTrueFalse A attribute value of boolean(0 or 1).
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemSetAttr.
 */
ZFUNC ZINT Dom_KeySetAttrBool(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bTrueFalse);

/**
 * @brief Dom set key attribute of boolean.
 * @code
 * <key attr="value(true or false)"></key>
 * @endcode
 *
 * @param [in] pstKeyElem A key element node.
 * @param [in] pcName The key name.
 * @param [in] bTrueFalse A attribute value of boolean(true or false).
 *
 * @retval ZOK put the key value successful.
 * @retval ZFAILED Otherwises.
 * @see Dom_ElemSetAttr.
 */
ZFUNC ZINT Dom_KeySetAttrBoolX(ST_DOM_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bTrueFalse);

#endif
    
#ifdef __cplusplus

}
#endif

#endif /* _DOM_UTIL_H__ */

