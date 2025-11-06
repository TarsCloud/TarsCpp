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
  File name     : ea_patch_ops.h
  Module        : eax xml-patch-ops
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax xml-patch-ops.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PATCH_OPS_H__
#define _EA_PATCH_OPS_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xml-patch-ops pos type */
typedef enum EN_EA_PATCH_OPS_POS_TYPE
{
    EN_EA_PATCH_OPS_POS_BEFORE,        
    EN_EA_PATCH_OPS_POS_AFTER,
    EN_EA_PATCH_OPS_POS_PREPEND
} EN_EA_PATCH_OPS_POS_TYPE;

/* xml-patch-ops ws type */
typedef enum EN_EA_PATCH_OPS_WS_TYPE
{
    EN_EA_PATCH_OPS_WS_BEFORE,        
    EN_EA_PATCH_OPS_WS_AFTER,
    EN_EA_PATCH_OPS_WS_BOTH
} EN_EA_PATCH_OPS_WS_TYPE;

/* xml-patch-ops get the add */
ZFUNC ZINT EaPatch_OpsGetAdd(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstAdd);

/* xml-patch-ops get the replace */
ZFUNC ZINT EaPatch_OpsGetRpl(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRpl);

/* xml-patch-ops get the remove */
ZFUNC ZINT EaPatch_OpsGetRmv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRmv);

/* xml-patch-ops get the add sel */
ZFUNC ZINT EaPatch_OpsAddGetSel(ST_XML_ELEM *pstAdd,
                ST_ZOS_USTR **ppstSel);

/* xml-patch-ops get the add pos EN_EA_PATCH_OPS_POS_TYPE */
ZFUNC ZINT EaPatch_OpsAddGetPos(ST_XML_ELEM *pstAdd, ZUCHAR *pucPos);

/* xml-patch-ops get the add type */
ZFUNC ZINT EaPatch_OpsAddGetType(ST_XML_ELEM *pstAdd,  
                ST_ZOS_USTR **ppstType);

/* xml-patch-ops get the replace sel */
ZFUNC ZINT EaPatch_OpsRplGetSel(ST_XML_ELEM *pstRpl,
                ST_ZOS_USTR **ppstSel);

/* xml-patch-ops get the remove sel */
ZFUNC ZINT EaPatch_OpsRmvGetSel(ST_XML_ELEM *pstRmv,
                ST_ZOS_USTR **ppstSel);

/* xml-patch-ops get the remove ws EN_EA_PATCH_OPS_WS_TYPE */
ZFUNC ZINT EaPatch_OpsRmvGetWs(ST_XML_ELEM *pstRmv, ZUCHAR *pucWs);

/* xml-patch-ops set the add */
ZFUNC ZINT EaPatch_OpsSetAdd(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstAdd);

/* xml-patch-ops set the replace */
ZFUNC ZINT EaPatch_OpsSetRpl(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRpl);

/* xml-patch-ops set the remove */
ZFUNC ZINT EaPatch_OpsSetRmv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRmv);

/* xml-patch-ops set the add sel */
ZFUNC ZINT EaPatch_OpsAddSetSel(ST_XML_ELEM *pstAdd,
                ST_ZOS_USTR *pstSel);

/* xml-patch-ops set the add pos EN_EA_PATCH_OPS_POS_TYPE */
ZFUNC ZINT EaPatch_OpsAddSetPos(ST_XML_ELEM *pstAdd, ZUCHAR ucPos);

/* xml-patch-ops set the add type */
ZFUNC ZINT EaPatch_OpsAddSetType(ST_XML_ELEM *pstAdd,  
                ST_ZOS_USTR *pstType);

/* xml-patch-ops set the replace sel */
ZFUNC ZINT EaPatch_OpsRplSetSel(ST_XML_ELEM *pstRpl,
                ST_ZOS_USTR *pstSel);

/* xml-patch-ops set the remove sel */
ZFUNC ZINT EaPatch_OpsRmvSetSel(ST_XML_ELEM *pstRmv,
                ST_ZOS_USTR *pstSel);

/* xml-patch-ops set the remove ws EN_EA_PATCH_OPS_WS_TYPE */
ZFUNC ZINT EaPatch_OpsRmvSetWs(ST_XML_ELEM *pstRmv, ZUCHAR ucWs);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PATCH_OPS_H__ */

