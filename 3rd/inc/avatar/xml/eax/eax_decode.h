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
  File name     : eax_decode.h
  Module        : extension api for xml
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-05-22
  Description   :
    Marcos and structure definitions required by the eax decode.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EAX_DECODE_H__
#define _EAX_DECODE_H__

#ifdef __cplusplus
extern "C"{
#endif

/* eax get the root element from document */
ZFUNC ZINT Eax_DocGetRoot(ST_XML_MSG *pstMsg, ST_XML_ELEM **ppstRoot);

/* eax is a empty element */
ZFUNC ZBOOL Eax_ElemIsEmpty(ST_XML_ELEM *pstElem);

/* eax get the name from element */
ZFUNC ZINT Eax_ElemGetName(ST_XML_ELEM *pstElem, 
                ST_XML_QNAME **ppstQName);

/* eax get the element namespace id */
ZFUNC ZINT Eax_ElemGetNsId(ST_XML_ELEM *pstElem, ZUINT *piNsId,
                ZUINT *piExtnNsId);

/* eax get the element name id from namespace */
ZFUNC ZINT Eax_ElemNsGetNameId(ST_XML_ELEM *pstElem, ZUINT iNsId,
                ZUINT *piNameId);

/* eax find an element by name*/
ZFUNC ZINT Eax_ElemGetChild(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstName, 
                ST_XML_ELEM **ppstChild);

/* eax find an element by qname */
ZFUNC ZINT Eax_ElemGetChildX(ST_XML_ELEM *pstElem, 
                ST_XML_QNAME *pstQName, ST_XML_ELEM **ppstChild);

/* eax find an element by name */
ZFUNC ZINT Eax_ElemGetNsChild(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ELEM **ppstChild);

/* eax get the first child from current element */
ZFUNC ZINT Eax_ElemGetFirstChild(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstChild);

/* eax get the last child from current element */
ZFUNC ZINT Eax_ElemGetLastChild(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstChild);

/* eax get the next sibling from current element */
ZFUNC ZINT Eax_ElemGetNextSibling(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSibling);

/* eax get the previous sibling from current element */
ZFUNC ZINT Eax_ElemGetPrevSibling(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSibling);

/* eax get the first namespace child element from default namespace */
ZFUNC ZINT Eax_ElemGetNsFirstChild(ST_XML_ELEM *pstElem, ZUINT iNsId,
                ZUINT iNameId, ST_XML_ELEM **ppstChild);

/* eax get the last namespace child element */
ZFUNC ZINT Eax_ElemGetNsLastChild(ST_XML_ELEM *pstElem, ZUINT iNsId,
                ZUINT iNameId, ST_XML_ELEM **ppstChild);

/* eax get the next namespace sibling from current element */
ZFUNC ZINT Eax_ElemGetNsNextSibling(ST_XML_ELEM *pstElem, 
                ZUINT iNsId, ZUINT iNameId, ST_XML_ELEM **ppstSibling);

/* eax get the previous namespace sibling from current element */
ZFUNC ZINT Eax_ElemGetNsPrevSibling(ST_XML_ELEM *pstElem, 
                ZUINT iNsId, ZUINT iNameId, ST_XML_ELEM **ppstSibling);

/* eax get the first attribute from element */
ZFUNC ZINT Eax_ElemGetFirstAttr(ST_XML_ELEM *pstElem, 
                ST_XML_ATTR **ppstAttr);

/* eax get the last attribute from element */
ZFUNC ZINT Eax_ElemGetLastAttr(ST_XML_ELEM *pstElem, 
                ST_XML_ATTR **ppstAttr);

/* eax get the first namespace attribute */
ZFUNC ZINT Eax_ElemGetFirstNsAttr(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ATTR **ppstAttr);

/* eax get the last namespace attribute */
ZFUNC ZINT Eax_ElemGetLastNsAttr(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ATTR **ppstAttr);

/* eax find an attribute by name */
ZFUNC ZINT Eax_ElemGetAttr(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstName, 
                ST_XML_ATTR **ppstAttr);

/* eax find an attribute by qname */
ZFUNC ZINT Eax_ElemGetAttrX(ST_XML_ELEM *pstElem, ST_XML_QNAME *pstQName, 
                ST_XML_ATTR **ppstAttr);

/* eax find an attribute by namespace and name */
ZFUNC ZINT Eax_ElemGetNsAttr(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ATTR **ppstAttr);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetAttrVal(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstName, 
                ST_ZOS_USTR **ppstVal);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetAttrValX(ST_XML_ELEM *pstElem, 
                ST_XML_QNAME *pstQName, ST_ZOS_USTR **ppstVal);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetNsAttrIdVal(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_ZOS_USTR **ppstVal);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetNsAttrIdValId(ST_XML_ELEM *pstElem, 
                ZUINT iNsId, ZUINT iNameId, ZUINT *piValId);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetNsAttrIdBool(ST_XML_ELEM *pstElem, 
                ZUINT iNsId, ZUINT iNameId, ZBOOL *pbTrueFalse);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetNsAttrIdUiDigit(ST_XML_ELEM *pstElem, 
                ZUINT iNsId, ZUINT iNameId, ZUINT *piData);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetNsAttrIdFloat(ST_XML_ELEM *pstElem, 
                ZUINT iNsId, ZUINT iNameId, ZFLOAT *pfData);

/* eax get the attribute value from element */
ZFUNC ZINT Eax_ElemGetNsAttrIdSiDigit(ST_XML_ELEM *pstElem, 
                ZUINT iNsId, ZUINT iNameId, ZINT *piData);

/* eax get the CharData from element */
ZFUNC ZINT Eax_ElemGetData(ST_XML_ELEM *pstElem, ST_ZOS_USTR **ppstData);

/* eax get the CharData from element */
ZFUNC ZINT Eax_ElemGetAllData(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstData);

/* eax get the element CharData id */
ZFUNC ZINT Eax_ElemGetDataId(ST_XML_ELEM *pstElem, ZUINT iNsId,
                ZUINT *piDataId);

/* eax get a bool data from element */
ZFUNC ZINT Eax_ElemGetBool(ST_XML_ELEM *pstElem, ZBOOL *pbTrueFalse);

/* eax get an unsigned int data from element */
ZFUNC ZINT Eax_ElemGetUiDigit(ST_XML_ELEM *pstElem, ZUINT *piData);

/* eax get a signed int data from element */
ZFUNC ZINT Eax_ElemGetSiDigit(ST_XML_ELEM *pstElem, ZINT *piData);

/* eax get the next attribute from current attribute */
ZFUNC ZINT Eax_AttrGetNext(ST_XML_ATTR *pstAttr, ST_XML_ATTR **ppstNext);

/* eax get the previous attribute from current attribute */
ZFUNC ZINT Eax_AttrGetPrev(ST_XML_ATTR *pstAttr, ST_XML_ATTR **ppstPrev);

/* eax get the name from attribute */
ZFUNC ZINT Eax_AttrGetName(ST_XML_ATTR *pstAttr, 
                ST_XML_QNAME **ppstQName);

/* eax get the namespace id from attribute */
ZFUNC ZINT Eax_AttrGetNsId(ST_XML_ATTR *pstAttr, ZUINT *piNsId, 
                ZUINT *piExtnNsId);

/* eax get the name id from attribute */
ZFUNC ZINT Eax_AttrNsGetNameId(ST_XML_ATTR *pstAttr, ZUINT iNsId, 
                ZUINT *piNameId);

/* eax get the data from attribute */
ZFUNC ZINT Eax_AttrGetData(ST_XML_ATTR *pstAttr, ST_ZOS_USTR **ppstData);

/* eax get the data from attribute */
ZFUNC ZINT Eax_AttrGetDataId(ST_XML_ATTR *pstAttr, ZUINT iNsId, 
                ZUINT *piDataId);

/* eax get a bool data from attribute */
ZFUNC ZINT Eax_AttrGetBool(ST_XML_ATTR *pstAttr, ZBOOL *pbTrueFalse);

/* eax get an unsigned int data from attribute */
ZFUNC ZINT Eax_AttrGetUiDigit(ST_XML_ATTR *pstAttr, ZUINT *piData);

/* eax get an float data from attribute */
ZFUNC ZINT Eax_AttrGetFloat(ST_XML_ATTR *pstAttr, ZFLOAT *pfData);

/* eax get a signed int data from attribute */
ZFUNC ZINT Eax_AttrGetSiDigit(ST_XML_ATTR *pstAttr, ZINT *piData);

#ifdef __cplusplus
}
#endif

#endif /* _EAX_DECODE_H__ */

