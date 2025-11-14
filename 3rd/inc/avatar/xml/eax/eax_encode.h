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
  File name     : eax_encode.h
  Module        : extension api for xml
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-05-22
  Description   :
    Marcos and structure definitions required by the eax encode.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EAX_ENCODE_H__
#define _EAX_ENCODE_H__

#ifdef __cplusplus
extern "C"{
#endif

/* eax add a document header */
ZFUNC ZINT Eax_DocAddHdr(ST_XML_MSG *pstMsg);

/* eax add the root element into document */
ZFUNC ZINT Eax_DocAddRoot(ST_XML_MSG *pstMsg, ST_XML_QNAME *pstQName,
                ST_XML_ELEM **ppstRoot);

/* eax add the root element into document */
ZFUNC ZINT Eax_DocAddRootX(ST_XML_MSG *pstMsg, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ELEM **ppstRoot);

/* eax add the root namespace element into document */
ZFUNC ZINT Eax_DocAddNsRoot(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ZUINT iNsId, ZUINT iNameId, ST_XML_ELEM **ppstRoot);

/* eax add the root namespace element into document(add xmlns attr) */
ZFUNC ZINT Eax_DocNsAddRoot(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ZUINT iNsId, ZUINT iNameId, ST_XML_ELEM **ppstRoot);

/* eax add the root namespace element into document */
ZFUNC ZINT Eax_DocNsAddRootX(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ZUINT iNsId, ZUINT iNameId, ST_XML_ELEM **ppstRoot);

/* eax add element namespace attribute */
ZFUNC ZINT Eax_ElemAddNsAttr(ST_XML_ELEM *pstElem, ST_EAX_NS *pstNs);

/* eax add element namespace attribute */
ZFUNC ZINT Eax_ElemAddNsAttrX(ST_XML_ELEM *pstElem, ZUINT iNsId,
                ZCHAR *pcPrefix);

/* eax add element namespace attribute */
ZFUNC ZINT Eax_ElemAddNsAttrS(ST_XML_ELEM *pstElem, ZCHAR *pcPrefix,
                ZCHAR *pcNsVal);

/* eax remove element namespace attribute */
ZFUNC ZINT Eax_ElemRmvNsAttr(ST_XML_ELEM *pstElem);

/* eax add a child element */
ZFUNC ZINT Eax_ElemAddChild(ST_XML_ELEM *pstElem, ST_XML_QNAME *pstQName, 
                ST_XML_ELEM **ppstChild);

/* eax add a child element */
ZFUNC ZINT Eax_ElemAddChildX(ST_XML_ELEM *pstElem, ZCHAR *pcChild, 
                ST_XML_ELEM **ppstChild);

/* eax add a child namespace element */
ZFUNC ZINT Eax_ElemAddNsChild(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ELEM **ppstChild);

/* eax add the attribute into element */
ZFUNC ZINT Eax_ElemAddAttr(ST_XML_ELEM *pstElem, ST_XML_QNAME *pstQName, 
                ST_XML_ATTR **ppstAttr);

/* eax add element attribute */
ZFUNC ZINT Eax_ElemAddAttrS(ST_XML_ELEM *pstElem, ZCHAR *pcName,
                ZCHAR *pcVal);

/* eax add an attribute with structure string */
ZFUNC ZINT Eax_ElemAddAttrVal(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstName, 
                ST_ZOS_USTR *pstVal);

/* eax add the attribute id into element */
ZFUNC ZINT Eax_ElemAddNsAttrId(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ATTR **ppstAttr);

/* eax add an attribute with name id and value into element */
ZFUNC ZINT Eax_ElemAddNsAttrIdVal(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_ZOS_USTR *pstVal);

/* eax add an attribute with name id and value id into element */
ZFUNC ZINT Eax_ElemAddNsAttrIdValId(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUINT iValId);

/* eax add an attribute with bool data */
ZFUNC ZINT Eax_ElemAddNsAttrIdBool(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZBOOL bTrueFalse);

/* eax add an attribute with unsigned int data */
ZFUNC ZINT Eax_ElemAddNsAttrIdUiDigit(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUINT iData);

/* eax add the attribute id into element */
ZFUNC ZINT Eax_ElemAddNsAttrNsId(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_XML_ATTR **ppstAttr);

/* eax add an attribute with name id and value into element */
ZFUNC ZINT Eax_ElemAddNsAttrNsIdVal(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ST_ZOS_USTR *pstVal);

/* eax add an attribute with name id and value into element */
ZFUNC ZINT Eax_ElemAddNsAttrNsIdValId(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUINT iValId);

/* eax add an attribute with name id and bool value into element */
ZFUNC ZINT Eax_ElemAddNsAttrNsIdBool(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZBOOL bTrueFalse);

/* eax add an attribute with name id and unsigned int data into element */
ZFUNC ZINT Eax_ElemAddNsAttrNsIdUiDigit(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUINT iData);

/* eax add an attribute with name id and signed int data into element */
ZFUNC ZINT Eax_ElemAddNsAttrNsIdSiDigit(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZUINT iData);

/* eax add an attribute with name id and float data into element */
ZFUNC ZINT Eax_ElemAddNsAttrNsIdFloat(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iNameId, ZFLOAT fData);

/* eax add the CharData into element */
ZFUNC ZINT Eax_ElemAddData(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstData);

/* eax add the CharData into element(it just set data memory not copy data) */
ZFUNC ZINT Eax_ElemAddDataX(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstData);

/* eax add the CDATA into element */
ZFUNC ZINT Eax_ElemAddCData(ST_XML_ELEM *pstElem, ST_ZOS_USTR *pstData);

/* eax add the CharData id into element */
ZFUNC ZINT Eax_ElemAddDataId(ST_XML_ELEM *pstElem, ZUINT iNsId, 
                ZUINT iDataId);

/* eax add a bool data into element */
ZFUNC ZINT Eax_ElemAddBool(ST_XML_ELEM *pstElem, ZBOOL bTrueFalse);

/* eax add an unsigned int data into element */
ZFUNC ZINT Eax_ElemAddUiDigit(ST_XML_ELEM *pstElem, ZUINT iData);

/* eax add a signed int data into element */
ZFUNC ZINT Eax_ElemAddSiDigit(ST_XML_ELEM *pstElem, ZINT iData);

/* eax add the value into attribute */
ZFUNC ZINT Eax_AttrAddData(ST_XML_ATTR *pstAttr, ST_ZOS_USTR *pstData);

/* eax add a bool data into attribute */
ZFUNC ZINT Eax_AttrAddBool(ST_XML_ATTR *pstAttr, ZBOOL bTrueFalse);

/* eax add an unsigned int data into attribute */
ZFUNC ZINT Eax_AttrAddUiDigit(ST_XML_ATTR *pstAttr, ZUINT iData);

/* eax add a signed int data into attribute */
ZFUNC ZINT Eax_AttrAddSiDigit(ST_XML_ATTR *pstAttr, ZINT iData);

/* eax add an float data into attribute */
ZFUNC ZINT Eax_AttrAddFloat(ST_XML_ATTR *pstAttr, ZFLOAT fData);

#ifdef __cplusplus
}
#endif

#endif /* _EAX_ENCODE_H__ */

