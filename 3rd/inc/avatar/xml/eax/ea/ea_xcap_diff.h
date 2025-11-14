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
  File name     : ea_xcap_diff.h
  Module        : eax xcap-diff
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax xcap-diff.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XCAP_DIFF_H__
#define _EA_XCAP_DIFF_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xcap-diff get the xcap-diff */
ZFUNC ZINT EaXcap_DiffGetDiff(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstDiff);

/* xcap-diff get the xcap-diff xcap-root attribute */
ZFUNC ZINT EaXcap_DiffDiffGetRoot(ST_XML_ELEM *pstDiff, 
                ST_ZOS_USTR **ppstRoot);

/* xcap-diff get the xcap-diff first document */
ZFUNC ZINT EaXcap_DiffDiffGetFirstDoc(ST_XML_ELEM *pstRoot, 
                ST_XML_ELEM **ppstDoc);

/* xcap-diff get the xcap-diff next document */
ZFUNC ZINT EaXcap_DiffDiffGetNextDoc(ST_XML_ELEM *pstDoc, 
                ST_XML_ELEM **ppstDoc);

/* xcap-diff get the xcap-diff first element */
ZFUNC ZINT EaXcap_DiffDiffGetFirstElem(ST_XML_ELEM *pstRoot, 
                ST_XML_ELEM **ppstElem);

/* xcap-diff get the xcap-diff next element */
ZFUNC ZINT EaXcap_DiffDiffGetNextElem(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* xcap-diff get the xcap-diff first attribute */
ZFUNC ZINT EaXcap_DiffDiffGetFirstAttr(ST_XML_ELEM *pstRoot, 
                ST_XML_ELEM **ppstAttr);

/* xcap-diff get the xcap-diff next attribute */
ZFUNC ZINT EaXcap_DiffDiffGetNextAttr(ST_XML_ELEM *pstAttr, 
                ST_XML_ELEM **ppstAttr);

/* xcap-diff get the document change-log */
ZFUNC ZINT EaXcap_DiffDocGetCLog(ST_XML_ELEM *pstDoc, 
                ST_XML_ELEM **ppstCLog);

/* xcap-diff get the document body-not-changed */
ZFUNC ZINT EaXcap_DiffDocGetBodyNotChg(ST_XML_ELEM *pstDoc, 
                ZBOOL *pbExist);

/* xcap-diff get the document doc-selector */
ZFUNC ZINT EaXcap_DiffDocGetDSelector(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR **ppstDSelecotr);

/* xcap-diff get the document sel */
ZFUNC ZINT EaXcap_DiffDocGetSel(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR **ppstSel);

/* xcap-diff get the document new-etag */
ZFUNC ZINT EaXcap_DiffDocGetNEtag(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR **ppstNEtag);

/* xcap-diff get the document previous-etag */
ZFUNC ZINT EaXcap_DiffDocGetPEtag(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR **ppstPEtag);

/* xcap-diff get the document hash */
ZFUNC ZINT EaXcap_DiffDocGetHash(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR **ppstHash);

/* xcap-diff get the change-log first add */
ZFUNC ZINT EaXcap_DiffCLogGetFirstAdd(ST_XML_ELEM *pstCLog, 
                ST_XML_ELEM **ppstAdd);

/* xcap-diff get the change-log next add */
ZFUNC ZINT EaXcap_DiffCLogGetNextAdd(ST_XML_ELEM *pstAdd, 
                ST_XML_ELEM **ppstAdd);

/* xcap-diff get the change-log first remove */
ZFUNC ZINT EaXcap_DiffCLogGetFirstRmv(ST_XML_ELEM *pstCLog, 
                ST_XML_ELEM **ppstRmv);

/* xcap-diff get the change-log next remove */
ZFUNC ZINT EaXcap_DiffCLogGetNextRmv(ST_XML_ELEM *pstRmv, 
                ST_XML_ELEM **ppstRmv);

/* xcap-diff get the change-log first replace */
ZFUNC ZINT EaXcap_DiffCLogGetFirstRpl(ST_XML_ELEM *pstCLog, 
                ST_XML_ELEM **ppstRpl);

/* xcap-diff get the change-log next replace */
ZFUNC ZINT EaXcap_DiffCLogGetNextRpl(ST_XML_ELEM *pstRpl, 
                ST_XML_ELEM **ppstRpl);

/* xcap-diff get the element sel */
ZFUNC ZINT EaXcap_DiffElemGetSel(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstSel);

/* xcap-diff get the element exists */
ZFUNC ZINT EaXcap_DiffElemGetExists(ST_XML_ELEM *pstElem, 
                ZBOOL *pbExist);

/* xcap-diff get the atribute sel */
ZFUNC ZINT EaXcap_DiffAttrGetSel(ST_XML_ELEM *pstAttr, 
                ST_ZOS_USTR **ppstSel);

/* xcap-diff get the atribute exists */
ZFUNC ZINT EaXcap_DiffAttrGetExists(ST_XML_ELEM *pstAttr, 
                ZBOOL *pbExist);

/* xcap-diff set the xcap-diff */
ZFUNC ZINT EaXcap_DiffSetDiff(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstDiff);

/* xcap-diff set the xcap-diff xcap-root attribute */
ZFUNC ZINT EaXcap_DiffDiffSetRoot(ST_XML_ELEM *pstDiff, 
                ST_ZOS_USTR *pstRoot);

/* xcap-diff set the xcap-diff document */
ZFUNC ZINT EaXcap_DiffDiffSetDoc(ST_XML_ELEM *pstRoot, 
                ST_XML_ELEM **ppstDoc);

/* xcap-diff set the xcap-diff element */
ZFUNC ZINT EaXcap_DiffDiffSetElem(ST_XML_ELEM *pstRoot, 
                ST_XML_ELEM **ppstElem);

/* xcap-diff set the xcap-diff attribute */
ZFUNC ZINT EaXcap_DiffDiffSetAttr(ST_XML_ELEM *pstRoot, 
                ST_XML_ELEM **ppstAttr);

/* xcap-diff set the document change-log */
ZFUNC ZINT EaXcap_DiffDocSetCLog(ST_XML_ELEM *pstDoc, 
                ST_XML_ELEM **ppstCLog);

/* xcap-diff set the document body-not-changed */
ZFUNC ZINT EaXcap_DiffDocSetBodyNotChg(ST_XML_ELEM *pstDoc);

/* xcap-diff set the document doc-selector */
ZFUNC ZINT EaXcap_DiffDocSetDSelector(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR *pstDSelecotr);

/* xcap-diff set the document sel */
ZFUNC ZINT EaXcap_DiffDocSetSel(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR *pstSel);

/* xcap-diff set the document new-etag */
ZFUNC ZINT EaXcap_DiffDocSetNEtag(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR *pstNEtag);

/* xcap-diff set the document previous-etag */
ZFUNC ZINT EaXcap_DiffDocSetPEtag(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR *pstPEtag);

/* xcap-diff set the document hash */
ZFUNC ZINT EaXcap_DiffDocSetHash(ST_XML_ELEM *pstDoc, 
                ST_ZOS_USTR *pstHash);

/* xcap-diff set the change-log add */
ZFUNC ZINT EaXcap_DiffCLogSetAdd(ST_XML_ELEM *pstCLog, 
                ST_XML_ELEM **ppstAdd);

/* xcap-diff set the change-log remove */
ZFUNC ZINT EaXcap_DiffCLogSetRmv(ST_XML_ELEM *pstCLog, 
                ST_XML_ELEM **ppstRmv);

/* xcap-diff set the change-log replace */
ZFUNC ZINT EaXcap_DiffCLogSetRpl(ST_XML_ELEM *pstCLog, 
                ST_XML_ELEM **ppstRpl);

/* xcap-diff set the element sel */
ZFUNC ZINT EaXcap_DiffElemSetSel(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstSel);

/* xcap-diff set the element exists */
ZFUNC ZINT EaXcap_DiffElemSetExists(ST_XML_ELEM *pstElem, 
                ZBOOL bExist);

/* xcap-diff set the atribute sel */
ZFUNC ZINT EaXcap_DiffAttrSetSel(ST_XML_ELEM *pstAttr, 
                ST_ZOS_USTR *pstSel);

/* xcap-diff set the atribute exists */
ZFUNC ZINT EaXcap_DiffAttrSetExists(ST_XML_ELEM *pstAttr, 
                ZBOOL bExist);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XCAP_DIFF_H__ */

