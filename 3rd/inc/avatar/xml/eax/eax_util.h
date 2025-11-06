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
  File name     : eax_util.h
  Module        : extension api for xml
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-05-22
  Description   :
    Marcos and structure definitions required by the eax utility.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EAX_UTIL_H__
#define _EAX_UTIL_H__

#ifdef __cplusplus
extern "C"{
#endif

/* eax crs(Coordinate Reference Systems) type */
typedef enum EN_EAX_CRS_TYPE
{
    EN_EAX_CRS_2D,                /* two dimensional CRS */
    EN_EAX_CRS_3D                 /* three dimensional CRS */
} EN_EAX_CRS_TYPE;        

/* eax uom(Units of Measure) type */
typedef enum EN_EAX_UOM_TYPE
{
    EN_EAX_UOM_LENGTH_METRE,      /* length measure using metres */
    EN_EAX_UOM_ANGULAR_DEGREE,    /* angular measure using degrees */
    EN_EAX_UOM_ANGULAR_RADIAN     /* angular measure using radians */
} EN_EAX_UOM_TYPE;

/* eax create a xml message */
ZFUNC ZINT Eax_MsgCreate(ST_XML_MSG **ppstMsg);

/* eax create a xml message */
ZFUNC ZINT Eax_MsgCreateX(ZUINT iBufSize, ST_XML_MSG **ppstMsg);

/* eax delete a xml message */
ZFUNC ZINT Eax_MsgDelete(ST_XML_MSG *pstMsg);

/* eax load xml message from file */
ZFUNC ZINT Eax_MsgLoadFile(ZCHAR *pcFileName, ZDBUF *pzMsgBuf,
                ST_XML_MSG **ppstXmlMsg);

/* eax load xml message from string */
ZFUNC ZINT Eax_MsgLoadData(ST_ZOS_USTR *pstData, 
                ST_XML_MSG **ppstXmlMsg);

/* eax load xml message from string(dynamic copy string) */
ZFUNC ZINT Eax_MsgLoadDataD(ST_ZOS_USTR *pstData, 
                ST_XML_MSG **ppstXmlMsg);

/* eax load message form fragment xml body string */
ZFUNC ZINT Eax_MsgLoadElem(ST_ZOS_USTR *pstData, ST_XML_MSG **ppstXmlMsg);

/* eax save xml message into file */
ZFUNC ZINT Eax_MsgSaveFile(ST_XML_MSG *pstMsg, ZBOOL bCompactFmt, 
                ZCHAR *pcFileName);

/* eax save xml message into string */
ZFUNC ZINT Eax_MsgSaveData(ST_XML_MSG *pstMsg, ZBOOL bCompactFmt,
                ZDBUF *pzData);

/* eax message set namespace */
ZFUNC ZINT Eax_MsgSetNs(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs);

/* eax message get namespace */
ZFUNC ZINT Eax_MsgGetNs(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs);

/* eax message add some namespace */
ZFUNC ZINT Eax_MsgAddNs(ST_XML_MSG *pstMsg, ZCHAR *pcPrefix, ZUINT iNsId);

/* eax message remove some namespace */
ZFUNC ZINT Eax_MsgRmvNs(ST_XML_MSG *pstMsg, ZUINT iNsId);

/* eax message remove all namespaces */
ZFUNC ZINT Eax_MsgRmvAllNs(ST_XML_MSG *pstMsg);

/* eax message find some namespace */
ZFUNC ZBOOL Eax_MsgFindNs(ST_XML_MSG *pstMsg, ZUINT iNsId);

/* eax set the extension namespace into namespaces */
ZFUNC ZINT Eax_MsgSetExtnNs(ST_XML_MSG *pstMsg, ZUINT iNsId, 
                ZUINT iExtnNsId);

/* eax init the namespace with default */
ZFUNC ZINT Eax_NsInit(ST_EAX_NS *pstNs, ZUINT iDftNsId);

/* eax set the default namespace into namespaces */
ZFUNC ZINT Eax_NsSetDft(ST_EAX_NS *pstNs, ZUINT iNsId);

/* eax set the prefix namespace into namespaces */
ZFUNC ZINT Eax_NsSetPrefix(ST_EAX_NS *pstNs, ZUINT iNsId,
                ST_ZOS_USTR *pstPrefix);

/* eax set the prefix namespace into namespaces */
ZFUNC ZINT Eax_NsSetPrefixX(ST_EAX_NS *pstNs, ZUINT iNsId, ZCHAR *pcPrefix);

/* eax get the prefix string from namespace */
ZFUNC ZINT Eax_NsGetPrefix(ST_EAX_NS *pstNs, ZUINT iNsId, 
                ST_ZOS_USTR *pstPrefix);

/* eax data check normalized */
ZFUNC ZINT Eax_DataChkNormalizedStr(ZUBUF zMemBuf, 
                ST_ZOS_USTR *pstOldStr, ST_ZOS_USTR **ppstNewStr);

/* eax data check normalized length */
ZFUNC ZINT Eax_DataChkNormalizedLen(ST_ZOS_USTR *pstStr, ZUINT *piLen);

/* eax data check escaped */
ZFUNC ZINT Eax_DataChkEscapedStr(ZUBUF zMemBuf, 
                ST_ZOS_USTR *pstOldStr, ST_ZOS_USTR **ppstNewStr);

/* eax data check escaped length */
ZFUNC ZINT Eax_DataChkEscapedLen(ST_ZOS_USTR *pstStr, ZUSHORT *pwLen);

/* eax copy the QName */
ZFUNC ZINT Eax_QNameCpy(ST_XML_QNAME *pstDst, ST_XML_QNAME *pstSrc);

/* eax duplicate the QName */
ZFUNC ZINT Eax_QNameDup(ST_XML_QNAME *pstDst, ST_XML_QNAME *pstSrc);

/* eax add an element data with string */
ZFUNC ZINT Eax_AddElemStr(ST_XML_ELEM *pstElem, ZUCHAR *pucStr);

/* eax add an element data with string length */
ZFUNC ZINT Eax_AddElemNStr(ST_XML_ELEM *pstElem, ZUCHAR *pucStr, 
                ZUSHORT wLen);

/* eax add an attribute with string */
ZFUNC ZINT Eax_AddAttrStr(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUCHAR *pucStr);

/* eax add an attribute with string length */
ZFUNC ZINT Eax_AddAttrNStr(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUCHAR *pucStr, ZUSHORT wLen);

/* eax namespace add an attribute with string */
ZFUNC ZINT Eax_AddNsAttrStr(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUCHAR *pucStr);

/* eax namespace add an attribute with string length */
ZFUNC ZINT Eax_AddNsAttrNStr(ST_XML_ELEM *pstElem, ZUINT iNsId,  
                ZUINT iNameId, ZUCHAR *pucStr, ZUSHORT wLen);

/* eax check the element namespace and name id */
ZFUNC ZBOOL Eax_ChkNsElemNameId(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId);

/* eax check the attribute namespace and name id */
ZFUNC ZBOOL Eax_ChkNsAttrNameId(ST_XML_ATTR *pstAttr, ZUINT iNsId, 
                ZUINT iNameId);

/* eax get an element by name */
ZFUNC ZINT Eax_GetElemStr(ST_XML_ELEM *pstElem, ZCHAR *pcName, 
                ST_XML_ELEM **ppstChild);

/* eax get an element by name */
ZFUNC ZINT Eax_GetElemStrX(ST_XML_ELEM *pstElem, ZCHAR *pcPrefix, 
                ZCHAR *pcName, ST_XML_ELEM **ppstChild);

/* eax get the attribute value from name */
ZFUNC ZINT Eax_GetAttrStr(ST_XML_ELEM *pstElem, ZCHAR *pcName, 
                ST_ZOS_USTR **ppstVal);

/* eax get the attribute value from qname */
ZFUNC ZINT Eax_GetAttrStrX(ST_XML_ELEM *pstElem, ZCHAR *pcPrefix, 
                ZCHAR *pcName, ST_ZOS_USTR **ppstVal);

/* eax get the timestamp */
ZFUNC ZINT Eax_GetTimeStamp(ZCHAR *pcTimeStamp, ZUSHORT wLen,
                ST_ZOS_SYS_TIME *pstDateTime);

/* eax get the timestamp */
ZFUNC ZINT Eax_GetTimeStampT(ZCHAR *pcTimeStamp, ZUSHORT wLen,
                ZTIME_T *pzTime);

/* eax get the timestamp UTC time */
ZFUNC ZINT Eax_GetTimeStampU(ZCHAR *pcTimeStamp, ZUSHORT wLen,
                ZTIME_T *pzTime);

/* eax generate the timestamp,
   it should be freed by Zos_SysStrFree */
ZFUNC ZINT Eax_GenTimeStamp(ST_ZOS_SYS_TIME *pstDateTime, ZCHAR **ppcTimeStamp);

/* eax generate the current timestamp,
   it should be freed by Zos_SysStrFree */
ZFUNC ZINT Eax_GenCurTimeStamp(ZCHAR **ppcTimeStamp);

/* eax elem get uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT Eax_ElemGetUomType(ST_XML_ELEM *pstElem, ZUCHAR *pucType);

/* eax elem set uom type, type see EN_EAX_UOM_TYPE */
ZFUNC ZINT Eax_ElemSetUomType(ST_XML_ELEM *pstElem, ZUCHAR ucType);

/* eax elem get crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT Eax_ElemGetCrsType(ST_XML_ELEM *pstElem, ZUCHAR *pucType);

/* eax elem set crs type, type see EN_EAX_CRS_TYPE */
ZFUNC ZINT Eax_ElemSetCrsType(ST_XML_ELEM *pstElem, ZUCHAR ucType);

/* eax section get the section key size */
ZFUNC ZINT Eax_SecGetSize(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZINT *piSize);

/* eax section get the section key element */
ZFUNC ZINT Eax_SecGetKey(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_XML_ELEM **ppstKeyElem);

/* eax section get the key element by index */
ZFUNC ZINT Eax_SecGetKeyX(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZINT iIdx, ST_XML_ELEM **ppstKeyElem);

/* eax section get the key element by index */
ZFUNC ZINT Eax_SecGetKeyS(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_XML_ELEM_LST **ppstKeyLst);

/* 
 * eax get section key value node 
 * <section>
 *   <key>value</key>
 * </section>
 */
ZFUNC ZINT Eax_SecGetVal(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_ZOS_USTR **ppstData);
/* 
 * eax get section key value node by index 
 * <section>
 *   <key>value1</key>
 *   <key>value2</key>
 * </section>
 */
ZFUNC ZINT Eax_SecGetValX(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZINT iIdx, ST_ZOS_USTR **ppstData);

/* eax get section key value of string <section><key>value</key></section> */
ZFUNC ZINT Eax_SecGetStr(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZCHAR *pcDftStr, ZCHAR *pcDstStr, ZUSHORT wDstSize);

/* eax get section key value of string <section><key>value</key></section> */
ZFUNC ZINT Eax_SecGetStrX(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_ZOS_USTR **ppstVal);

/* eax get section key value of unsign int 
 * <section><key>value</key></section> */
ZFUNC ZINT Eax_SecGetUi(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUINT iDftVal, ZUINT *piVal);

/* eax get section key value of unsign short 
 * <section><key>value</key></section> */
ZFUNC ZINT Eax_SecGetUs(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUSHORT wDftVal, ZUSHORT *pwVal);

/* eax get section key value of unsign char 
 * <section><key>value</key></section> */
ZFUNC ZINT Eax_SecGetUc(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUCHAR ucDftVal, ZUCHAR *pucVal);

/* eax get section key value of boolean 
 * <section><key>value(0 or 1)</key></section> */
ZFUNC ZINT Eax_SecGetBool(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);

/* eax get section key value of boolean 
 * <section><key>value(true or false)</key></section> */
ZFUNC ZINT Eax_SecGetBoolX(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);

/* eax get key value node <key>value</key> */
ZFUNC ZINT Eax_KeyGetVal(ST_XML_ELEM *pstKeyElem, ST_ZOS_USTR **ppstData);

/* eax get key value of string <key>value</key> */
ZFUNC ZINT Eax_KeyGetStr(ST_XML_ELEM *pstKeyElem, ZCHAR *pcDftStr, 
                ZCHAR *pcDstStr, ZUSHORT wDstSize);

/* eax get key value of string <key>value</key> */
ZFUNC ZINT Eax_KeyGetStrX(ST_XML_ELEM *pstKeyElem, 
                ST_ZOS_USTR **ppstVal);

/* eax get key value of unsign int <key>value</key> */
ZFUNC ZINT Eax_KeyGetUi(ST_XML_ELEM *pstKeyElem, ZUINT iDftVal, 
                ZUINT *piVal);

/* eax get key value of unsign short <key>value</key> */
ZFUNC ZINT Eax_KeyGetUs(ST_XML_ELEM *pstKeyElem, ZUSHORT wDftVal, 
                ZUSHORT *pwVal);

/* eax get key value of unsign char <key>value</key> */
ZFUNC ZINT Eax_KeyGetUc(ST_XML_ELEM *pstKeyElem, ZUCHAR ucDftVal, 
                ZUCHAR *pucVal);

/* eax get key value of boolean <key>value(0 or 1)</key> */
ZFUNC ZINT Eax_KeyGetBool(ST_XML_ELEM *pstKeyElem, ZBOOL bDftTrueFalse, 
                ZBOOL *pbTrueFalse);

/* eax get key value of boolean <key>value(true or false)</key> */
ZFUNC ZINT Eax_KeyGetBoolX(ST_XML_ELEM *pstKeyElem, ZBOOL bDftTrueFalse, 
                ZBOOL *pbTrueFalse);

/* eax get key attribute check <key attr></key> exist */
ZFUNC ZINT Eax_KeyChkAttr(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL *pbExist);

/* eax get key attribute string <key attr=value></key> */
ZFUNC ZINT Eax_KeyGetAttr(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ST_ZOS_USTR **ppstVal);

/* eax get key attribute string <key attr=value></key> */
ZFUNC ZINT Eax_KeyGetAttrStr(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZCHAR *pcVal, ZUSHORT wMaxSize);

/* eax get key attribute of unsigned int */
ZFUNC ZINT Eax_KeyGetAttrUi(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUINT iDftVal, ZUINT *piVal);

/* eax get key attribute of unsigned short */
ZFUNC ZINT Eax_KeyGetAttrUs(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUSHORT wDftVal, ZUSHORT *pwVal);

/* eax get key attribute of unsigned char */
ZFUNC ZINT Eax_KeyGetAttrUc(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUCHAR ucDftVal, ZUCHAR *pucVal);

/* eax get key attribute of bool(0 or 1) */
ZFUNC ZINT Eax_KeyGetAttrBool(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);

/* eax get key attribute of bool(true or false) */
ZFUNC ZINT Eax_KeyGetAttrBoolX(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bDftTrueFalse, ZBOOL *pbTrueFalse);

/* eax put document root element */
ZFUNC ZINT Eax_DocPutRoot(ST_XML_MSG *pstMsg, ZCHAR *pcRootName,
                ST_XML_ELEM **ppstRootElem);

/* eax put section key element <section> <key/> </section> */
ZFUNC ZINT Eax_SecPutKey(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ST_XML_ELEM **ppstKeyElem);

/* eax set section key value of string
 * <section><key>value</key></section> */
ZFUNC ZINT Eax_SecSetStr(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZCHAR *pcValStr);

/* eax set section key value of unsign int 
 * <section><key>value</key></section> */
ZFUNC ZINT Eax_SecSetUi(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUINT iVal);

/* eax set section key value of unsign short
 * <section><key>value</key></section> */
ZFUNC ZINT Eax_SecSetUs(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUSHORT wVal);

/* eax set section key value of unsign char
 * <section><key>value</key></section> */
ZFUNC ZINT Eax_SecSetUc(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZUCHAR ucVal);

/* eax set section key value of boolean 
 * <section><key>value(0 or 1)</key></section> */
ZFUNC ZINT Eax_SecSetBool(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bTrueFalse);

/* eax set section key value of boolean 
 * <section><key>value(true or false)</key></section> */
ZFUNC ZINT Eax_SecSetBoolX(ST_XML_ELEM *pstSecElem, ZCHAR *pcKeyName,
                ZBOOL bTrueFalse);

/* eax put key element value <key>value</key> */
ZFUNC ZINT Eax_KeyPutVal(ST_XML_ELEM *pstKeyElem, ZCHAR *pcVal);

/* eax set key value of unsign int <key>value</key> */
ZFUNC ZINT Eax_KeySetUi(ST_XML_ELEM *pstKeyElem, ZUINT iVal);

/* eax set key value of unsign short <key>value</key> */
ZFUNC ZINT Eax_KeySetUs(ST_XML_ELEM *pstKeyElem, ZUSHORT wVal);

/* eax set key value of unsign char <key>value</key> */
ZFUNC ZINT Eax_KeySetUc(ST_XML_ELEM *pstKeyElem, ZUCHAR ucVal);

/* eax set key value of boolean <key>value(0 or 1)</key> */
ZFUNC ZINT Eax_KeySetBool(ST_XML_ELEM *pstKeyElem, ZBOOL bTrueFalse);

/* eax set key value of boolean <key>value(true or false)</key> */
ZFUNC ZINT Eax_KeySetBoolX(ST_XML_ELEM *pstKeyElem, ZBOOL bTrueFalse);

/* eax set key attribute of string */
ZFUNC ZINT Eax_KeySetAttrStr(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZCHAR *pcVal);

/* eax set key attribute of unsigned int <key attr="value"></key> */
ZFUNC ZINT Eax_KeySetAttrUi(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUINT iVal);

/* eax set key attribute of unsigned short <key attr="value"></key> */
ZFUNC ZINT Eax_KeySetAttrUs(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUSHORT wVal);

/* eax set key attribute of unsigned char <key attr="value"></key> */
ZFUNC ZINT Eax_KeySetAttrUc(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZUCHAR ucVal);

/* eax set key attribute of boolean <key attr="value(0 or 1)"></key> */
ZFUNC ZINT Eax_KeySetAttrBool(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bTrueFalse);

/* eax set key attribute of boolean <key attr="value(true or false)"></key> */
ZFUNC ZINT Eax_KeySetAttrBoolX(ST_XML_ELEM *pstKeyElem, ZCHAR *pcName, 
                ZBOOL bTrueFalse);

#ifdef __cplusplus
}
#endif

#endif /* _EAX_UTIL_H__ */

