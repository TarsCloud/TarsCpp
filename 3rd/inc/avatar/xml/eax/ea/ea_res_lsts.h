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
  File name     : ea_res_lsts.h
  Module        : eax resource-lists
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax resource-lists.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_RES_LSTS_H__
#define _EA_RES_LSTS_H__

#ifdef __cplusplus
extern "C"{
#endif

/********* urn:ietf:params:xml:ns:resource-lists *********/
typedef enum EN_EA_RES_LST_TYPE
{
    EN_EA_RES_LST_DISP_NAME,      /* display-name */
    EN_EA_RES_LST_LST,            /* list */
    EN_EA_RES_LST_EXTERNAL,       /* external */
    EN_EA_RES_LST_ENTRY,          /* entry */
    EN_EA_RES_LST_ENTRY_REF,      /* entry-ref */
    EN_EA_RES_LST_NAME,           /* name */
    EN_EA_RES_LST_URI,            /* uri */
    EN_EA_RES_LST_REF,            /* ref */
    EN_EA_RES_LST_ANCHOR,         /* anchor */
    EN_EA_RES_LST_RES_LSTS,       /* resource-lists */
    EN_EA_RES_LST_CPM_ACTION      /* cpm_action */
} EN_EA_RES_LST_TYPE;

/* resource-lists get the resource-lists */
ZFUNC ZINT EaRes_LstsGetLsts(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstLsts);

/* resource-lists resource-lists get the list */
ZFUNC ZINT EaRes_LstsLstsGetLst(ST_XML_ELEM *pstLsts, 
                ST_ZOS_USTR *pstName, ST_XML_ELEM **ppstLst);

/* resource-lists resource-lists get the first list */
ZFUNC ZINT EaRes_LstsLstsGetFirstLst(ST_XML_ELEM *pstLsts, 
                ST_XML_ELEM **ppstLst);

/* resource-lists resource-lists get the next list */
ZFUNC ZINT EaRes_LstsLstsGetNextLst(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstLst);

/* resource-lists get the list display-name */
ZFUNC ZINT EaRes_LstsLstGetDispName(ST_XML_ELEM *pstLst,  
                ST_ZOS_USTR **ppstDispName);

/* resource-lists get the list name */
ZFUNC ZINT EaRes_LstsLstGetName(ST_XML_ELEM *pstLst,  
                ST_ZOS_USTR **ppstName);

/* resource-lists get the list list */
ZFUNC ZINT EaRes_LstsLstGetLst(ST_XML_ELEM *pstLst, 
                ST_ZOS_USTR *pstName, ST_XML_ELEM **ppstLst);

/* resource-lists get the first list list */
ZFUNC ZINT EaRes_LstsLstGetFirstLst(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstLst);

/* resource-lists get the next list list */
ZFUNC ZINT EaRes_LstsLstGetNextLst(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstLst);

/* resource-lists get the list entry by uri */
ZFUNC ZINT EaRes_LstsLstGetEntry(ST_XML_ELEM *pstLst,  
                ST_ZOS_USTR *pstUri, ST_XML_ELEM **ppstEntry);

/* resource-lists get the first list entry */
ZFUNC ZINT EaRes_LstsLstGetFirstEntry(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstEntry);

/* resource-lists get the next list entry */
ZFUNC ZINT EaRes_LstsLstGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* resource-lists get the list external by anc */
ZFUNC ZINT EaRes_LstsLstGetExternal(ST_XML_ELEM *pstLst,  
                ST_ZOS_USTR *pstAnchor, ST_XML_ELEM **ppstExternal);

/* resource-lists get the first list external */
ZFUNC ZINT EaRes_LstsLstGetFirstExternal(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstExternal);

/* resource-lists get the next list external */
ZFUNC ZINT EaRes_LstsLstGetNextExternal(ST_XML_ELEM *pstExternal, 
                ST_XML_ELEM **ppstExternal);

/* resource-lists get the first list entry-ref */
ZFUNC ZINT EaRes_LstsLstGetFirstEntryRef(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstEntryRef);

/* resource-lists get the next list entry-ref */
ZFUNC ZINT EaRes_LstsLstGetNextEntryRef(ST_XML_ELEM *pstEntryRef, 
                ST_XML_ELEM **ppstEntryRef);

/* resource-lists get the entry display-name */
ZFUNC ZINT EaRes_LstsEntryGetDispName(ST_XML_ELEM *pstEntry,  
                ST_ZOS_USTR **ppstDispName);

/* resource-lists get the entry uri */
ZFUNC ZINT EaRes_LstsEntryGetUri(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstUri);

/* resource-lists get the entry-ref display-name */
ZFUNC ZINT EaRes_LstsEntryRefGetDispName(ST_XML_ELEM *pstEntryRef,  
                ST_ZOS_USTR **ppstDispName);

/* resource-lists get the entry-ref ref */
ZFUNC ZINT EaRes_LstsEntryRefGetRef(ST_XML_ELEM *pstEntryRef, 
                ST_ZOS_USTR **ppstRef);

/* resource-lists get the external display-name */
ZFUNC ZINT EaRes_LstsExternalGetDispName(ST_XML_ELEM *pstExternal,  
                ST_ZOS_USTR **ppstDispName);

/* resource-lists get the external anchor */
ZFUNC ZINT EaRes_LstsExternalGetAnchor(ST_XML_ELEM *pstExternal, 
                ST_ZOS_USTR **ppstAnchor);

/* resource-lists set the resource-lists */
ZFUNC ZINT EaRes_LstsSetLsts(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs,
                ST_XML_ELEM **ppstLsts);

/* resource-lists resource-lists set the list */
ZFUNC ZINT EaRes_LstsLstsSetLst(ST_XML_ELEM *pstLsts, 
                ST_XML_ELEM **ppstLst);

/* resource-lists set the list display-name */
ZFUNC ZINT EaRes_LstsLstSetDispName(ST_XML_ELEM *pstLst,  
                ST_ZOS_USTR *pstDispName);

/* resource-lists set the list name */
ZFUNC ZINT EaRes_LstsLstSetName(ST_XML_ELEM *pstLst, ST_ZOS_USTR *pstName);

/* resource-lists set the list list */
ZFUNC ZINT EaRes_LstsLstSetLst(ST_XML_ELEM *pstLst, ST_XML_ELEM **ppstLst);

/* resource-lists set the list external */
ZFUNC ZINT EaRes_LstsLstSetExternal(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstExternal);

/* resource-lists set the list entry */
ZFUNC ZINT EaRes_LstsLstSetEntry(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstEntry);

/* resource-lists set the list entry-ref */
ZFUNC ZINT EaRes_LstsLstSetEntryRef(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstEntryRef);

/* resource-lists set the entry display-name */
ZFUNC ZINT EaRes_LstsEntrySetDispName(ST_XML_ELEM *pstEntry,  
                ST_ZOS_USTR *pstDispName);

/* resource-lists set the entry uri */
ZFUNC ZINT EaRes_LstsEntrySetUri(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR *pstUri);

/* resource-lists set the entry cpm action */
ZFUNC ZINT EaRes_LstsEntrySetCpmAction(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR *pstCpmAction);

/* resource-lists set the entry-ref display-name */
ZFUNC ZINT EaRes_LstsEntryRefSetDispName(ST_XML_ELEM *pstEntryRef,  
                ST_ZOS_USTR *pstDispName);

/* resource-lists set the entry-ref ref */
ZFUNC ZINT EaRes_LstsEntryRefSetRef(ST_XML_ELEM *pstEntryRef, 
                ST_ZOS_USTR *pstRef);

/* resource-lists set the external display-name */
ZFUNC ZINT EaRes_LstsExternalSetDispName(ST_XML_ELEM *pstExternal,  
                ST_ZOS_USTR *pstDispName);

/* resource-lists set the external anchor */
ZFUNC ZINT EaRes_LstsExternalSetAnchor(ST_XML_ELEM *pstExternal, 
                ST_ZOS_USTR *pstAnchor);

#ifdef __cplusplus
}
#endif

#endif /* _EA_RES_LSTS_H__ */

