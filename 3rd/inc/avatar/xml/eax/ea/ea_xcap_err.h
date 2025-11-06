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
  File name     : ea_xcap_err.h
  Module        : eax xcap-error
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax xcap-error.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XCAP_ERR_H__
#define _EA_XCAP_ERR_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xcap-error error-element type */
typedef enum EN_EA_XCAP_ERR_ELEM_TYPE
{
    EN_EA_XCAP_ERR_ELEM_EXTN,        
    EN_EA_XCAP_ERR_ELEM_SCHEMA_VAL_ERR,
    EN_EA_XCAP_ERR_ELEM_NOT_XML_FRAG,
    EN_EA_XCAP_ERR_ELEM_NO_PARENT,
    EN_EA_XCAP_ERR_ELEM_CANNOT_INSERT,
    EN_EA_XCAP_ERR_ELEM_NOT_XML_ATT_VAL,
    EN_EA_XCAP_ERR_ELEM_UNI_FAIL,
    EN_EA_XCAP_ERR_ELEM_NOT_WELL_FORM,
    EN_EA_XCAP_ERR_ELEM_CONSTRAINT_FAIL,
    EN_EA_XCAP_ERR_ELEM_CANNOT_DEL,
    EN_EA_XCAP_ERR_ELEM_NOT_UTF8        
} EN_EA_XCAP_ERR_ELEM_TYPE;

/* xcap-error get the xcap-error */
ZFUNC ZINT EaXcap_ErrGetErr(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstErr);

/* xcap-error get the xcap-error element */
ZFUNC ZINT EaXcap_ErrErrGetElem(ST_XML_ELEM *pstErr, 
                ZUCHAR ucElemType, ST_XML_ELEM **ppstElem);

/* xcap-error get the error element phrase */
ZFUNC ZINT EaXcap_ErrElemGetPhrase(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstPhrase);

/* xcap-error get the no-parent ancestor */
ZFUNC ZINT EaXcap_ErrNoParentGetAncestor(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstAncestor);

/* xcap-error element get the exists */
ZFUNC ZINT EaXcap_ErrElemGetExists(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstField, ST_XML_ELEM **ppstExists);

/* xcap-error element get the first exists */
ZFUNC ZINT EaXcap_ErrElemGetFirstExists(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstExists);

/* xcap-error element get the next exists */
ZFUNC ZINT EaXcap_ErrElemGetNextExists(ST_XML_ELEM *pstExists, 
                ST_XML_ELEM **ppstExists);

/* xcap-error element exists get the field */
ZFUNC ZINT EaXcap_ErrExistsGetFiled(ST_XML_ELEM *pstExists, 
                ST_ZOS_USTR **ppstField);

/* xcap-error element get the exists */
ZFUNC ZINT EaXcap_ErrElemGetExistsX(ST_XML_ELEM *pstElem, 
                ZCHAR *pcPrefix, ZCHAR *pcPostfix, ST_XML_ELEM **ppstExists);

/* xcap-error element exists get the first alt-value */
ZFUNC ZINT EaXcap_ErrExistsGetFirstAltVal(ST_XML_ELEM *pstExists, 
                ST_XML_ELEM **ppstAltVal);

/* xcap-error element exists get the next alt-value */
ZFUNC ZINT EaXcap_ErrExistsGetNextAltVal(ST_XML_ELEM *pstAltVal, 
                ST_XML_ELEM **ppstAltVal);

/* xcap-error alt-value get the value */
ZFUNC ZINT EaXcap_ErrAltValGetVal(ST_XML_ELEM *pstAltVal, 
                ST_ZOS_USTR **ppstVal);

/* xcap-error set the xcap-error */
ZFUNC ZINT EaXcap_ErrSetErr(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstErr);

/* xcap-error set the xcap-error element */
ZFUNC ZINT EaXcap_ErrErrSetElem(ST_XML_ELEM *pstErr, 
                ZUCHAR ucElemType, ST_XML_ELEM **ppstElem);

/* xcap-error set the error element phrase */
ZFUNC ZINT EaXcap_ErrElemSetPhrase(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstPhrase);

/* xcap-error set the no-parent ancestor */
ZFUNC ZINT EaXcap_ErrNoParentSetAncestor(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstAncestor);

/* xcap-error element set the exists */
ZFUNC ZINT EaXcap_ErrElemSetExists(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstExists);

/* xcap-error element exists set the field */
ZFUNC ZINT EaXcap_ErrExistsSetFiled(ST_XML_ELEM *pstExists, 
                ST_ZOS_USTR *pstField);

/* xcap-error element exists get the alt-value */
ZFUNC ZINT EaXcap_ErrExistsSetAltVal(ST_XML_ELEM *pstExists, 
                ST_XML_ELEM **ppstAltVal);

/* xcap-error alt-value set the value */
ZFUNC ZINT EaXcap_ErrAltValSetVal(ST_XML_ELEM *pstAltVal, 
                ST_ZOS_USTR *pstVal);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XCAP_ERR_H__ */

