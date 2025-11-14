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
  File name     : ea_xdm_cply.h
  Module        : eax xdm common-policy
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax xdm common-policy.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XDM_CPLY_H__
#define _EA_XDM_CPLY_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xdm common-policy get the other-identity */
ZFUNC ZINT EaXdm_CplyGetOtherIdent(ST_XML_ELEM *pstElem, ZBOOL *pbExist);

/* xdm common-policy get the external-list */
ZFUNC ZINT EaXdm_CplyGetELst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstELst);

/* xdm common-policy get the anonymous-request */
ZFUNC ZINT EaXdm_CplyGetAnonyReq(ST_XML_ELEM *pstElem, ZBOOL *pbExist);

/* xdm common-policy get the first external-list entry */
ZFUNC ZINT EaXdm_CplyELstGetFirstEntry(ST_XML_ELEM *pstELst, 
                ST_XML_ELEM **ppstEntry);

/* xdm common-policy get the next external-list entry */
ZFUNC ZINT EaXdm_CplyELstGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* xdm common-policy get the entry anc */
ZFUNC ZINT EaXdm_CplytEntryGetAnc(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstAnc);

/* xdm common-policy set the other-identity */
ZFUNC ZINT EaXdm_CplySetOtherIdent(ST_XML_ELEM *pstElem);

/* xdm common-policy set the external-list */
ZFUNC ZINT EaXdm_CplySetELst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstELst);

/* xdm common-policy set the anonymous-request */
ZFUNC ZINT EaXdm_CplySetAnonyReq(ST_XML_ELEM *pstElem);

/* xdm common-policy set the external-list entry */
ZFUNC ZINT EaXdm_CplyELstSetEntry(ST_XML_ELEM *pstELst, 
                ST_XML_ELEM **ppstEntry);

/* xdm common-policy set the entry anc */
ZFUNC ZINT EaXdm_CplytEntrySetAnc(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR *pstAnc);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XDM_CPLY_H__ */

