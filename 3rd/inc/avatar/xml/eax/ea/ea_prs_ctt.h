/************************************************************************

        Copyright (c) 2005-2011 by Juphoon System Software, Inc.
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
  File name     : ea_pres_ctt.h
  Module        : eax pres-content
  Author        : zocol
  Version       : 0.1
  Created on    : 2011-03-01
  Description   :
    Marcos and structure definitions required by the eax pres-content.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PRES_CTT_H__
#define _EA_PRES_CTT_H__

#ifdef __cplusplus
extern "C"{
#endif

/* pres-content get the content */
ZFUNC ZINT EaPrs_CttGetCont(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstCont);

/* pres-content get the content mime-type */
ZFUNC ZINT EaPrs_CttContGetMType(ST_XML_ELEM *pstCont, 
                ST_ZOS_USTR **ppstMType);

/* pres-content get the content encoding */
ZFUNC ZINT EaPrs_CttContGetEnco(ST_XML_ELEM *pstCont, 
                ST_ZOS_USTR **ppstEnco);

/* pres-content get the first content description */
ZFUNC ZINT EaPrs_CttContGetFirstDesc(ST_XML_ELEM *pstCont, 
                ST_XML_ELEM **ppstDesc);

/* pres-content get the next content description */
ZFUNC ZINT EaPrs_CttContGetNextDesc(ST_XML_ELEM *pstDesc, 
                ST_XML_ELEM **ppstDesc);

/* pres-content get the content data */
ZFUNC ZINT EaPrs_CttContGetData(ST_XML_ELEM *pstCont, 
                ST_ZOS_USTR **ppstData);

/* pres-content description get the description value */
ZFUNC ZINT EaPrs_CttDescGetDescValue(ST_XML_ELEM *pstDesc, 
                ST_ZOS_USTR **ppstValue);

/* pres-content description get the lang */
ZFUNC ZINT EaPrs_CttDescGetLang(ST_XML_ELEM *pstDesc, 
                ST_ZOS_USTR **ppstLang);

/* pres-content set the content */
ZFUNC ZINT EaPrs_CttSetCont(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs,
                ST_XML_ELEM **ppstCont);

/* pres-content set the content mime-type */
ZFUNC ZINT EaPrs_CttContSetMType(ST_XML_ELEM *pstCont, 
                ST_ZOS_USTR *pstMType);

/* pres-content set the content encoding */
ZFUNC ZINT EaPrs_CttContSetEnco(ST_XML_ELEM *pstCont, 
                ST_ZOS_USTR *pstEnco);

/* pres-content set the content description */
ZFUNC ZINT EaPrs_CttContSetDesc(ST_XML_ELEM *pstCont, 
                ST_XML_ELEM **ppstDesc);

/* pres-content set the content data */
ZFUNC ZINT EaPrs_CttContSetData(ST_XML_ELEM *pstCont, 
                ST_ZOS_USTR *pstData);

/* pres-content set the content data(it just set data memory not copy data) */
ZFUNC ZINT EaPrs_CttContSetDataX(ST_XML_ELEM *pstCont, 
                ST_ZOS_USTR *pstData);

/* pres-content description set the description value */
ZFUNC ZINT EaPrs_CttDescSetDescValue(ST_XML_ELEM *pstDesc, 
                ST_ZOS_USTR *pstValue);

/* pres-content description set the lang */
ZFUNC ZINT EaPrs_CttDescSetLang(ST_XML_ELEM *pstDesc, 
                ST_ZOS_USTR *pstLang);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PRES_CTT_H__ */

