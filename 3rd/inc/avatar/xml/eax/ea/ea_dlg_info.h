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
  File name     : ea_dlg_info.h
  Module        : eax dialog-info
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax dialog-info.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_DLG_INFO_H__
#define _EA_DLG_INFO_H__

#ifdef __cplusplus
extern "C"{
#endif

/* dialog-info state type */
typedef enum EN_EA_DLG_INFO_STA_TYPE
{
    EN_EA_DLG_INFO_STA_FULL,        
    EN_EA_DLG_INFO_STA_PARTIAL
} EN_EA_DLG_INFO_STA_TYPE;

/* dialog-info direction type */
typedef enum EN_EA_DLG_INFO_DIRECT_TYPE
{
    EN_EA_DLG_INFO_DIRECT_INITOR,        
    EN_EA_DLG_INFO_DIRECT_RECIPT
} EN_EA_DLG_INFO_DIRECT_TYPE;

/* dialog-info event type */
typedef enum EN_EA_DLG_INFO_EVNT_TYPE
{
    EN_EA_DLG_INFO_EVNT_CANCELL,        
    EN_EA_DLG_INFO_EVNT_REJECT,
    EN_EA_DLG_INFO_EVNT_REPLACE,
    EN_EA_DLG_INFO_EVNT_LOCAL_BYE,
    EN_EA_DLG_INFO_EVNT_REMOTE_BYE,
    EN_EA_DLG_INFO_EVNT_ERR,
    EN_EA_DLG_INFO_EVNT_TIMEOUT    
} EN_EA_DLG_INFO_EVNT_TYPE;

/* dialog-info get the dialog-info */
ZFUNC ZINT EaDlg_InfoGetInfo(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstInfo);

/* dialog-info get the first dialog-info dialog */
ZFUNC ZINT EaDlg_InfoInfoGetFirstDlg(ST_XML_ELEM *pstInfo, 
                ST_XML_ELEM **ppstDlg);

/* dialog-info get the next dialog-info rule */
ZFUNC ZINT EaDlg_InfoInfoGetNextDlg(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstDlg);

/* dialog-info get the dialog-info version */
ZFUNC ZINT EaDlg_InfoInfoGetVer(ST_XML_ELEM *pstInfo, ZUINT *piVer);

/* dialog-info get the dialog-info state EN_EA_DLG_INFO_STA_TYPE */
ZFUNC ZINT EaDlg_InfoInfoGetSta(ST_XML_ELEM *pstInfo, ZUCHAR *pucSta);

/* dialog-info get the dialog-info entity */
ZFUNC ZINT EaDlg_InfoInfoGetEnt(ST_XML_ELEM *pstInfo, 
                ST_ZOS_USTR **ppstEnt);

/* dialog-info get the dialog state */
ZFUNC ZINT EaDlg_InfoDlgGetSta(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstSta);

/* dialog-info get the dialog duration */
ZFUNC ZINT EaDlg_InfoDlgGetDuration(ST_XML_ELEM *pstInfo, ZUINT *piDuration);

/* dialog-info get the dialog replaces */
ZFUNC ZINT EaDlg_InfoDlgGetRpls(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstRpls);

/* dialog-info get the dialog referred-by */
ZFUNC ZINT EaDlg_InfoDlgGetReferBy(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstReferBy);

/* dialog-info get the dialog route-set */
ZFUNC ZINT EaDlg_InfoDlgGetRouteSet(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstRouteSet);

/* dialog-info get the dialog local */
ZFUNC ZINT EaDlg_InfoDlgGetLocal(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstLocal);

/* dialog-info get the dialog remote */
ZFUNC ZINT EaDlg_InfoDlgGetRmt(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstRmt);

/* dialog-info get the dialog id */
ZFUNC ZINT EaDlg_InfoDlgGetId(ST_XML_ELEM *pstDlg, ST_ZOS_USTR **ppstId);

/* dialog-info get the dialog call-id */
ZFUNC ZINT EaDlg_InfoDlgGetCallId(ST_XML_ELEM *pstDlg, 
                ST_ZOS_USTR **ppstCallId);

/* dialog-info get the dialog local-tag */
ZFUNC ZINT EaDlg_InfoDlgGetLocalTag(ST_XML_ELEM *pstDlg, 
                ST_ZOS_USTR **ppstLocalTag);

/* dialog-info get the dialog remote-tag */
ZFUNC ZINT EaDlg_InfoDlgGetRmtTag(ST_XML_ELEM *pstDlg, 
                ST_ZOS_USTR **ppstRmtTag);

/* dialog-info get the dialog direction */
ZFUNC ZINT EaDlg_InfoDlgGetDirect(ST_XML_ELEM *pstDlg, ZUCHAR *pucDirect);

/* dialog-info get the replaces call-id */
ZFUNC ZINT EaDlg_InfoRplsGetCallId(ST_XML_ELEM *pstRpls, 
                ST_ZOS_USTR **ppstCallId);

/* dialog-info get the replaces local-tag */
ZFUNC ZINT EaDlg_InfoRplsGetLocalTag(ST_XML_ELEM *pstRpls, 
                ST_ZOS_USTR **ppstLocalTag);

/* dialog-info get the replaces remote-tag */
ZFUNC ZINT EaDlg_InfoRplsGetRmtTag(ST_XML_ELEM *pstRpls, 
                ST_ZOS_USTR **ppstRmtTag);

/* dialog-info get the first route-set hop */
ZFUNC ZINT EaDlg_InfoRouteSetGetFirstHop(ST_XML_ELEM *pstRouteSet, 
                ST_XML_ELEM **ppstHop);

/* dialog-info get the next route-set hop */
ZFUNC ZINT EaDlg_InfoRouteSetGetNextHop(ST_XML_ELEM *pstHop, 
                ST_XML_ELEM **ppstHop);

/* dialog-info get the participant identity */
ZFUNC ZINT EaDlg_InfoPartpGetIdent(ST_XML_ELEM *pstPartp, 
                ST_XML_ELEM **ppstIdent);

/* dialog-info get the participant target */
ZFUNC ZINT EaDlg_InfoPartpGetTarget(ST_XML_ELEM *pstPartp, 
                ST_XML_ELEM **ppstTarget);

/* dialog-info get the participant session-description */
ZFUNC ZINT EaDlg_InfoPartpGetSessDesc(ST_XML_ELEM *pstPartp, 
                ST_XML_ELEM **ppstSessDesc);

/* dialog-info get the participant cseq */
ZFUNC ZINT EaDlg_InfoPartpGetCseq(ST_XML_ELEM *pstPartp, ZUINT *piCseq);

/* dialog-info get the nameaddr display-name */
ZFUNC ZINT EaDlg_InfoNameAddrGetDispName(ST_XML_ELEM *pstNameAddr, 
                ST_ZOS_USTR **ppstDispName);

/* dialog-info get the first target param */
ZFUNC ZINT EaDlg_InfoTargetGetFirstParm(ST_XML_ELEM *pstTarget, 
                ST_XML_ELEM **ppstParm);

/* dialog-info get the next target param */
ZFUNC ZINT EaDlg_InfoTargetGetNextParm(ST_XML_ELEM *pstParm, 
                ST_XML_ELEM **ppstParm);

/* dialog-info get the target uri */
ZFUNC ZINT EaDlg_InfoTargetGetUri(ST_XML_ELEM *pstParm, 
                ST_ZOS_USTR **ppstUri);

/* dialog-info get the param pname */
ZFUNC ZINT EaDlg_InfoParmGetPname(ST_XML_ELEM *pstParm, 
                ST_ZOS_USTR **ppstPname);

/* dialog-info get the param pval */
ZFUNC ZINT EaDlg_InfoParmGetPval(ST_XML_ELEM *pstParm, 
                ST_ZOS_USTR **ppstPval);

/* dialog-info get the sessd type */
ZFUNC ZINT EaDlg_InfoSessdGetType(ST_XML_ELEM *pstSessd, 
                ST_ZOS_USTR **ppstType);

/* dialog-info get the state event EN_EA_DLG_INFO_EVNT_TYPE */
ZFUNC ZINT EaDlg_InfoStaGetEvnt(ST_XML_ELEM *pstSta, ZUCHAR *pucEvnt);

/* dialog-info get the state code */
ZFUNC ZINT EaDlg_InfoStaGetCode(ST_XML_ELEM *pstSta, ZUINT *piCode);

/* dialog-info set the dialog-info */
ZFUNC ZINT EaDlg_InfoSetInfo(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstInfo);

/* dialog-info set the dialog-info dialog */
ZFUNC ZINT EaDlg_InfoInfoSetDlg(ST_XML_ELEM *pstInfo, 
                ST_XML_ELEM **ppstDlg);

/* dialog-info set the dialog-info version */
ZFUNC ZINT EaDlg_InfoInfoSetVer(ST_XML_ELEM *pstInfo, ZUINT iVer);

/* dialog-info set the dialog-info state EN_EA_DLG_INFO_STA_TYPE */
ZFUNC ZINT EaDlg_InfoInfoSetSta(ST_XML_ELEM *pstInfo, ZUCHAR ucSta);

/* dialog-info set the dialog-info entity */
ZFUNC ZINT EaDlg_InfoInfoSetEnt(ST_XML_ELEM *pstInfo, 
                ST_ZOS_USTR *pstEnt);

/* dialog-info set the dialog state */
ZFUNC ZINT EaDlg_InfoDlgSetSta(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstSta);

/* dialog-info set the dialog duration */
ZFUNC ZINT EaDlg_InfoDlgSetDuration(ST_XML_ELEM *pstInfo, ZUINT iDuration);

/* dialog-info set the dialog replaces */
ZFUNC ZINT EaDlg_InfoDlgSetRpls(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstRpls);

/* dialog-info set the dialog referred-by */
ZFUNC ZINT EaDlg_InfoDlgSetReferBy(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstReferBy);

/* dialog-info set the dialog route-set */
ZFUNC ZINT EaDlg_InfoDlgSetRouteSet(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstRouteSet);

/* dialog-info set the dialog local */
ZFUNC ZINT EaDlg_InfoDlgSetLocal(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstLocal);

/* dialog-info set the dialog remote */
ZFUNC ZINT EaDlg_InfoDlgSetRmt(ST_XML_ELEM *pstDlg, 
                ST_XML_ELEM **ppstRmt);

/* dialog-info set the dialog id */
ZFUNC ZINT EaDlg_InfoDlgSetId(ST_XML_ELEM *pstDlg, ST_ZOS_USTR *pstId);

/* dialog-info set the dialog id */
ZFUNC ZINT EaDlg_InfoDlgSetIdX(ST_XML_ELEM *pstDlg, ZUINT iId);

/* dialog-info set the dialog call-id */
ZFUNC ZINT EaDlg_InfoDlgSetCallId(ST_XML_ELEM *pstDlg, 
                ST_ZOS_USTR *pstCallId);

/* dialog-info set the dialog local-tag */
ZFUNC ZINT EaDlg_InfoDlgSetLocalTag(ST_XML_ELEM *pstDlg, 
                ST_ZOS_USTR *pstLocalTag);

/* dialog-info set the dialog remote-tag */
ZFUNC ZINT EaDlg_InfoDlgSetRmtTag(ST_XML_ELEM *pstDlg, 
                ST_ZOS_USTR *pstRmtTag);

/* dialog-info set the dialog direction */
ZFUNC ZINT EaDlg_InfoDlgSetDirect(ST_XML_ELEM *pstDlg, ZUCHAR ucDirect);

/* dialog-info set the replaces call-id */
ZFUNC ZINT EaDlg_InfoRplsSetCallId(ST_XML_ELEM *pstRpls, 
                ST_ZOS_USTR *pstCallId);

/* dialog-info set the replaces local-tag */
ZFUNC ZINT EaDlg_InfoRplsSetLocalTag(ST_XML_ELEM *pstRpls, 
                ST_ZOS_USTR *pstLocalTag);

/* dialog-info set the replaces remote-tag */
ZFUNC ZINT EaDlg_InfoRplsSetRmtTag(ST_XML_ELEM *pstRpls, 
                ST_ZOS_USTR *pstRmtTag);

/* dialog-info set the route-set hop */
ZFUNC ZINT EaDlg_InfoRouteSetSetHop(ST_XML_ELEM *pstRouteSet, 
                ST_XML_ELEM **ppstHop);

/* dialog-info set the participant identity */
ZFUNC ZINT EaDlg_InfoPartpSetIdent(ST_XML_ELEM *pstPartp, 
                ST_XML_ELEM **ppstIdent);

/* dialog-info set the participant target */
ZFUNC ZINT EaDlg_InfoPartpSetTarget(ST_XML_ELEM *pstPartp, 
                ST_XML_ELEM **ppstTarget);

/* dialog-info set the participant session-description */
ZFUNC ZINT EaDlg_InfoPartpSetSessDesc(ST_XML_ELEM *pstPartp, 
                ST_XML_ELEM **ppstSessDesc);

/* dialog-info set the participant cseq */
ZFUNC ZINT EaDlg_InfoPartpSetCseq(ST_XML_ELEM *pstPartp, ZUINT iCseq);

/* dialog-info set the nameaddr display-name */
ZFUNC ZINT EaDlg_InfoNameAddrSetDispName(ST_XML_ELEM *pstNameAddr, 
                ST_ZOS_USTR *pstDispName);

/* dialog-info set the tarset param */
ZFUNC ZINT EaDlg_InfoTargetSetParm(ST_XML_ELEM *pstTarget, 
                ST_XML_ELEM **ppstParm);

/* dialog-info set the tarset uri */
ZFUNC ZINT EaDlg_InfoTargetSetUri(ST_XML_ELEM *pstParm, 
                ST_ZOS_USTR *pstUri);

/* dialog-info set the param pname */
ZFUNC ZINT EaDlg_InfoParmSetPname(ST_XML_ELEM *pstParm, 
                ST_ZOS_USTR *pstPname);

/* dialog-info set the param pval */
ZFUNC ZINT EaDlg_InfoParmSetPval(ST_XML_ELEM *pstParm, 
                ST_ZOS_USTR *pstPval);

/* dialog-info set the sessd type */
ZFUNC ZINT EaDlg_InfoSessdSetType(ST_XML_ELEM *pstSessd, 
                ST_ZOS_USTR *pstType);

/* dialog-info set the state event EN_EA_DLG_INFO_EVNT_TYPE */
ZFUNC ZINT EaDlg_InfoStaSetEvnt(ST_XML_ELEM *pstSta, ZUCHAR ucEvnt);

/* dialog-info set the state code */
ZFUNC ZINT EaDlg_InfoStaSetCode(ST_XML_ELEM *pstSta, ZUINT iCode);

#ifdef __cplusplus
}
#endif

#endif /* _EA_DLG_INFO_H__ */

