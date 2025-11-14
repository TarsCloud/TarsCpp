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
  File name     : ea_rlmi.h
  Module        : eax rlmi
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax rlmi.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_RLMI_H__
#define _EA_RLMI_H__

#ifdef __cplusplus
extern "C"{
#endif

/* rlmi state type */
typedef enum EN_EA_RLMI_STA_TYPE
{
    EN_EA_RLMI_STA_ACTIVE,        
    EN_EA_RLMI_STA_PENDING,
    EN_EA_RLMI_STA_TERM
} EN_EA_RLMI_STA_TYPE;

/* rlmi get the list */
ZFUNC ZINT EaRlmi_GetLst(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstLst);

/* rlmi get the first list name */
ZFUNC ZINT EaRlmi_LstGetFirstName(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstName);

/* rlmi get the next list name */
ZFUNC ZINT EaRlmi_LstGetNextName(ST_XML_ELEM *pstName, 
                ST_XML_ELEM **ppstName);

/* rlmi get the first list resource */
ZFUNC ZINT EaRlmi_LstGetFirstRes(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstRes);

/* rlmi get the next list resource */
ZFUNC ZINT EaRlmi_LstGetNextRes(ST_XML_ELEM *pstRes, 
                ST_XML_ELEM **ppstRes);

/* rlmi get the list uri */
ZFUNC ZINT EaRlmi_LstGetUri(ST_XML_ELEM *pstLst, ST_ZOS_USTR **ppstUri);

/* rlmi get the list version */
ZFUNC ZINT EaRlmi_LstGetVer(ST_XML_ELEM *pstLst, ZUINT *piVer);

/* rlmi get the list fullState */
ZFUNC ZINT EaRlmi_LstGetFullSat(ST_XML_ELEM *pstLst, ZBOOL *pbFullSta);

/* rlmi get the list cid */
ZFUNC ZINT EaRlmi_LstGetCid(ST_XML_ELEM *pstLst, ST_ZOS_USTR **ppstCid);

/* rlmi get the first resource name */
ZFUNC ZINT EaRlmi_ResGetFirstName(ST_XML_ELEM *pstRes, 
                ST_XML_ELEM **ppstName);

/* rlmi get the next resource name */
ZFUNC ZINT EaRlmi_ResGetNextName(ST_XML_ELEM *pstName, 
                ST_XML_ELEM **ppstName);

/* rlmi get the first resource instance */
ZFUNC ZINT EaRlmi_ResGetFirstInst(ST_XML_ELEM *pstRes, 
                ST_XML_ELEM **ppstInst);

/* rlmi get the next resource instance */
ZFUNC ZINT EaRlmi_ResGetNextInst(ST_XML_ELEM *pstInst, 
                ST_XML_ELEM **ppstInst);

/* rlmi get the resource uri */
ZFUNC ZINT EaRlmi_ResGetUri(ST_XML_ELEM *pstRes, ST_ZOS_USTR **ppstUri);

/* rlmi get the instance id */
ZFUNC ZINT EaRlmi_InstGetId(ST_XML_ELEM *pstInst, ST_ZOS_USTR **ppstId);

/* rlmi get the instance EN_EA_RLMI_STA_TYPE */
ZFUNC ZINT EaRlmi_InstGetSta(ST_XML_ELEM *pstInst, ZUCHAR *pucType);

/* rlmi get the instance reason */
ZFUNC ZINT EaRlmi_InstGetReason(ST_XML_ELEM *pstInst, 
                ST_ZOS_USTR **ppstReason);

/* rlmi get the instance cid */
ZFUNC ZINT EaRlmi_InstGetCid(ST_XML_ELEM *pstInst, ST_ZOS_USTR **ppstCid);

/* rlmi get the name lang */
ZFUNC ZINT EaRlmi_NameGetLang(ST_XML_ELEM *pstName, 
                ST_ZOS_USTR **ppstLang);

/* rlmi set the list */
ZFUNC ZINT EaRlmi_SetLst(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstLst);

/* rlmi set the list name */
ZFUNC ZINT EaRlmi_LstSetName(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstName);

/* rlmi set the list resource */
ZFUNC ZINT EaRlmi_LstSetRes(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstRes);

/* rlmi set the list uri */
ZFUNC ZINT EaRlmi_LstSetUri(ST_XML_ELEM *pstLst, ST_ZOS_USTR *pstUri);

/* rlmi set the list version */
ZFUNC ZINT EaRlmi_LstSetVer(ST_XML_ELEM *pstLst, ZUINT iVer);

/* rlmi set the list fullState */
ZFUNC ZINT EaRlmi_LstSetFullSat(ST_XML_ELEM *pstLst, ZBOOL bFullSta);

/* rlmi set the list cid */
ZFUNC ZINT EaRlmi_LstSetCid(ST_XML_ELEM *pstLst, ST_ZOS_USTR *pstCid);

/* rlmi set the resource name */
ZFUNC ZINT EaRlmi_ResSetName(ST_XML_ELEM *pstRes, 
                ST_XML_ELEM **ppstName);

/* rlmi set the resource instance */
ZFUNC ZINT EaRlmi_ResSetInst(ST_XML_ELEM *pstRes, 
                ST_XML_ELEM **ppstInst);

/* rlmi set the resource uri */
ZFUNC ZINT EaRlmi_ResSetUri(ST_XML_ELEM *pstRes, ST_ZOS_USTR *pstUri);

/* rlmi set the instance id */
ZFUNC ZINT EaRlmi_InstSetId(ST_XML_ELEM *pstInst, ST_ZOS_USTR *pstId);

/* rlmi set the instance EN_EA_RLMI_STA_TYPE */
ZFUNC ZINT EaRlmi_InstSetSta(ST_XML_ELEM *pstInst, ZUCHAR ucType);

/* rlmi set the instance reason */
ZFUNC ZINT EaRlmi_InstSetReason(ST_XML_ELEM *pstInst, 
                ST_ZOS_USTR *pstReason);

/* rlmi set the instance cid */
ZFUNC ZINT EaRlmi_InstSetCid(ST_XML_ELEM *pstInst, ST_ZOS_USTR *pstCid);

/* rlmi set the name lang */
ZFUNC ZINT EaRlmi_NameSetLang(ST_XML_ELEM *pstName, 
                ST_ZOS_USTR *pstLang);

#ifdef __cplusplus
}
#endif

#endif /* _EA_RLMI_H__ */

