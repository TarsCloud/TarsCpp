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
  File name     : ea_pidf_caps.h
  Module        : eax pidf caps
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pidf caps.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_CAPS_H__
#define _EA_PIDF_CAPS_H__

#ifdef __cplusplus
extern "C"{
#endif

/* pidf caps get the servcaps */
ZFUNC ZINT EaPidf_CapsGetSCaps(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstSCaps);
				
/* pidf caps get the servcaps */
ZFUNC ZINT EaPidf_CapsGetSCapsX(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSCaps);

/* pidf caps get the servcaps actor */
ZFUNC ZINT EaPidf_CapsSCapsGetActor(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstActor);

/* pidf caps get the servcaps application */
ZFUNC ZINT EaPidf_CapsSCapsGetApp(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbApp);

/* pidf caps get the servcaps audio */
ZFUNC ZINT EaPidf_CapsSCapsGetAudio(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbAudio);

/* pidf caps get the servcaps automata */
ZFUNC ZINT EaPidf_CapsSCapsGetAuto(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbAuto);

/* pidf caps get the servcaps class */
ZFUNC ZINT EaPidf_CapsSCapsGetClass(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstClass);

/* pidf caps get the servcaps control */
ZFUNC ZINT EaPidf_CapsSCapsGetCtrl(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbCtrl);

/* pidf caps get the servcaps data */
ZFUNC ZINT EaPidf_CapsSCapsGetData(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbData);

/* pidf caps get the servcaps description */
ZFUNC ZINT EaPidf_CapsSCapsGetDesc(ST_XML_ELEM *pstSCaps, 
                ST_ZOS_USTR **ppstDesc);

/* pidf caps get the servcaps duplex */
ZFUNC ZINT EaPidf_CapsSCapsGetDuplex(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstDuplex);

/* pidf caps get the servcaps event-packages */
ZFUNC ZINT EaPidf_CapsSCapsGetEvntPkgs(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstEvntPkgs);

/* pidf caps get the servcaps extensions */
ZFUNC ZINT EaPidf_CapsSCapsGetExtn(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstExtn);

/* pidf caps get the servcaps isfocus */
ZFUNC ZINT EaPidf_CapsSCapsGetIsfocus(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbIsfocus);

/* pidf caps get the servcaps message */
ZFUNC ZINT EaPidf_CapsSCapsGetMsg(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbMsg);

/* pidf caps get the servcaps methods */
ZFUNC ZINT EaPidf_CapsSCapsGetMethods(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstMethods);

/* pidf caps get the servcaps languages */
ZFUNC ZINT EaPidf_CapsSCapsGetLangs(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstLangs);

/* pidf caps get the servcaps priority */
ZFUNC ZINT EaPidf_CapsSCapsGetPriority(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstPriority);

/* pidf caps get the servcaps schemes */
ZFUNC ZINT EaPidf_CapsSCapsGetSchemes(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstSchemes);

/* pidf caps get the servcaps text */
ZFUNC ZINT EaPidf_CapsSCapsGetTxt(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbTxt);

/* pidf caps get the first servcaps type */
ZFUNC ZINT EaPidf_CapsSCapsGetFirstType(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstType);

/* pidf caps get the next servcaps type */
ZFUNC ZINT EaPidf_CapsSCapsGetNextType(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstType);

/* pidf caps get the servcaps video */
ZFUNC ZINT EaPidf_CapsSCapsGetVideo(ST_XML_ELEM *pstSCaps, 
                ZBOOL *pbVideo);

/* pidf caps get the devcaps */
ZFUNC ZINT EaPidf_CapsGetDCaps(ST_XML_ELEM *pstMsg, 
                ST_XML_ELEM **ppstDCaps);

/* pidf caps get the devcaps description */
ZFUNC ZINT EaPidf_CapsDCapsGetDesc(ST_XML_ELEM *pstDCaps, 
                ST_ZOS_USTR **ppstDesc);

/* pidf caps get the devcaps mobility */
ZFUNC ZINT EaPidf_CapsDCapsGetMob(ST_XML_ELEM *pstDCaps, 
                ST_XML_ELEM **ppstMob);

/* pidf caps get the class supported */
ZFUNC ZINT EaPidf_CapsClassGetSupt(ST_XML_ELEM *pstClass, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the class notsupported */
ZFUNC ZINT EaPidf_CapsClassGetNotSupt(ST_XML_ELEM *pstClass, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the classs business */
ZFUNC ZINT EaPidf_CapsClasssGetBiz(ST_XML_ELEM *pstClasss, 
                ST_ZOS_USTR **ppstBiz);

/* pidf caps get the classs personal */
ZFUNC ZINT EaPidf_CapsClasssGetPersonal(ST_XML_ELEM *pstClasss, 
                ST_ZOS_USTR **ppstPersonal);

/* pidf caps get the duplex supported */
ZFUNC ZINT EaPidf_CapsDuplexGetSupt(ST_XML_ELEM *pstDuplex, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the duplex notsupported */
ZFUNC ZINT EaPidf_CapsDuplexGetNotSupt(ST_XML_ELEM *pstDuplex, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the duplexs full */
ZFUNC ZINT EaPidf_CapsDuplexsGetFull(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR **ppstFull);

/* pidf caps get the duplexs half */
ZFUNC ZINT EaPidf_CapsDuplexsGetHalf(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR **ppstHalf);

/* pidf caps get the duplexs receive-only */
ZFUNC ZINT EaPidf_CapsDuplexsGetRecvOnly(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR **ppstRecvOnly);

/* pidf caps get the duplexs send-only */
ZFUNC ZINT EaPidf_CapsDuplexsGetSendOnly(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR **ppstSendOnly);

/* pidf caps get the event-packages supported */
ZFUNC ZINT EaPidf_CapsEvntPkgsGetSupt(ST_XML_ELEM *pstEvntPkgs, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the event-packages notsupported */
ZFUNC ZINT EaPidf_CapsEvntPkgsGetNotSupt(ST_XML_ELEM *pstEvntPkgs, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the event message-summary */
ZFUNC ZINT EaPidf_CapsEvntGetMsgSummary(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstMsgSummary);

/* pidf caps get the event presence */
ZFUNC ZINT EaPidf_CapsEvntGetPres(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstPres);

/* pidf caps get the event refer */
ZFUNC ZINT EaPidf_CapsEvntGetRefer(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstRefer);

/* pidf caps get the event reg */
ZFUNC ZINT EaPidf_CapsEvntGetReg(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstReg);

/* pidf caps get the event Siemens-RTP-Stats */
ZFUNC ZINT EaPidf_CapsEvntGetSRtpStat(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstSRtpStat);

/* pidf caps get the event spirits-INDPs */
ZFUNC ZINT EaPidf_CapsEvntGetSIndp(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstSIndp);

/* pidf caps get the event spirits-user-prof */
ZFUNC ZINT EaPidf_CapsEvntGetSUsrProf(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstSUsrProf);

/* pidf caps get the event winfo */
ZFUNC ZINT EaPidf_CapsEvntGetWinfo(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR **ppstWinfo);

/* pidf caps get the prioritysupported */
ZFUNC ZINT EaPidf_CapsPriorityGetSupt(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the priority notsupported */
ZFUNC ZINT EaPidf_CapsPriorityGetNotSupt(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the priority equals */
ZFUNC ZINT EaPidf_CapsPriorityGetEqual(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstEqual);

/* pidf caps get the priority higherhan */
ZFUNC ZINT EaPidf_CapsPriorityGetHigher(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstHigher);

/* pidf caps get the priority lowerthan */
ZFUNC ZINT EaPidf_CapsPriorityGetLower(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstLower);

/* pidf caps get the priority range */
ZFUNC ZINT EaPidf_CapsPriorityGetRange(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstRange);

/* pidf caps get the equals value */
ZFUNC ZINT EaPidf_CapsEqualGetVal(ST_XML_ELEM *pstEqual, 
                ZINT *piVal);

/* pidf caps get the higherhan minvalue */
ZFUNC ZINT EaPidf_CapsHigherGetMinVal(ST_XML_ELEM *pstHigher, 
                ZINT *piMinVal);

/* pidf caps get the lowerthan maxvalue */
ZFUNC ZINT EaPidf_CapsLowerGetMaxVal(ST_XML_ELEM *pstLower, 
                ZINT *piMaxVal);

/* pidf caps get the range minvalue */
ZFUNC ZINT EaPidf_CapsRangeGetMinVal(ST_XML_ELEM *pstRange, 
                ZINT *piMinVal);

/* pidf caps get the range maxvalue */
ZFUNC ZINT EaPidf_CapsRangeGetMaxVal(ST_XML_ELEM *pstRange, 
                ZINT *piMaxVal);

/* pidf caps get the methods supported */
ZFUNC ZINT EaPidf_CapsMethodsGetSupt(ST_XML_ELEM *pstMethods, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the methods notsupported */
ZFUNC ZINT EaPidf_CapsMethodsGetNotSupt(ST_XML_ELEM *pstMethods, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the method ACK */
ZFUNC ZINT EaPidf_CapsMethodGetAck(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method BYE */
ZFUNC ZINT EaPidf_CapsMethodGetBye(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method CANCEL */
ZFUNC ZINT EaPidf_CapsMethodGetCancel(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method INFO */
ZFUNC ZINT EaPidf_CapsMethodGetInfo(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method INVITE */
ZFUNC ZINT EaPidf_CapsMethodGetIvt(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method MESSAGE */
ZFUNC ZINT EaPidf_CapsMethodGetMsg(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method NOTIFY */
ZFUNC ZINT EaPidf_CapsMethodGetNtfy(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method OPTIONS */
ZFUNC ZINT EaPidf_CapsMethodGetOpt(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method PRACK */
ZFUNC ZINT EaPidf_CapsMethodGetPrack(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method PUBLISH */
ZFUNC ZINT EaPidf_CapsMethodGetPub(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method REFER */
ZFUNC ZINT EaPidf_CapsMethodGetRefer(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method REGISTER */
ZFUNC ZINT EaPidf_CapsMethodGetReg(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method SUBSCRIBE */
ZFUNC ZINT EaPidf_CapsMethodGetSubs(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the method UPDATE */
ZFUNC ZINT EaPidf_CapsMethodGetUpt(ST_XML_ELEM *pstMethod, 
                ZBOOL *pbExist);

/* pidf caps get the extensions supported */
ZFUNC ZINT EaPidf_CapsExtnsGetSupt(ST_XML_ELEM *pstExtns, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the extensions notsupported */
ZFUNC ZINT EaPidf_CapsExtnsGetNotSupt(ST_XML_ELEM *pstExtns, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the extension join */
ZFUNC ZINT EaPidf_CapsExtnGetJoin(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the extension path */
ZFUNC ZINT EaPidf_CapsExtnGetPath(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the extension precondition */
ZFUNC ZINT EaPidf_CapsExtnGetPrecond(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the extension pref */
ZFUNC ZINT EaPidf_CapsExtnGetPref(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the extension privacy */
ZFUNC ZINT EaPidf_CapsExtnGetPrivacy(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the extension rel100 */
ZFUNC ZINT EaPidf_CapsExtnGetRel100(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the extension replaces */
ZFUNC ZINT EaPidf_CapsExtnGetRpl(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the extension sec-agree */
ZFUNC ZINT EaPidf_CapsExtnGetSecAgree(ST_XML_ELEM *pstExtn, 
                ZBOOL *pbExist);

/* pidf caps get the schemes supported */
ZFUNC ZINT EaPidf_CapsSchemesGetSupt(ST_XML_ELEM *pstSchemes, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the schemes notsupported */
ZFUNC ZINT EaPidf_CapsSchemesGetNotSupt(ST_XML_ELEM *pstSchemes, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the scheme s */
ZFUNC ZINT EaPidf_CapsSchemeGetS(ST_XML_ELEM *pstScheme, 
                ST_ZOS_USTR **ppstS);

/* pidf caps get the actor supported */
ZFUNC ZINT EaPidf_CapsActorGetSupt(ST_XML_ELEM *pstActor, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the actor notsupported */
ZFUNC ZINT EaPidf_CapsActorGetNotSupt(ST_XML_ELEM *pstActor, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the actorts attendant */
ZFUNC ZINT EaPidf_CapsActorsGetAttend(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR **ppstAttend);

/* pidf caps get the actorts information */
ZFUNC ZINT EaPidf_CapsActorsGetInfo(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR **ppstInfo);

/* pidf caps get the actorts msg-taker */
ZFUNC ZINT EaPidf_CapsActorsGetMsgTaker(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR **ppstMsgTaker);

/* pidf caps get the actorts principal */
ZFUNC ZINT EaPidf_CapsActorsGetPrinc(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR **ppstPrinc);

/* pidf caps get the languages supported */
ZFUNC ZINT EaPidf_CapsLangsGetSupt(ST_XML_ELEM *pstLangs, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the languages notsupported */
ZFUNC ZINT EaPidf_CapsLangsGetNotSupt(ST_XML_ELEM *pstLangs, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the language l */
ZFUNC ZINT EaPidf_CapsLangGetL(ST_XML_ELEM *pstLang, 
                ST_ZOS_USTR **ppstL);

/* pidf caps get the mobility supported */
ZFUNC ZINT EaPidf_CapsMobGetSupt(ST_XML_ELEM *pstMob, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps get the mobility notsupported */
ZFUNC ZINT EaPidf_CapsMobGetNotSupt(ST_XML_ELEM *pstMob, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps get the mobilitys fixed */
ZFUNC ZINT EaPidf_CapsMobsGetFix(ST_XML_ELEM *pstMobs, 
                ST_ZOS_USTR **ppstFix);

/* pidf caps get the mobilitys mobile */
ZFUNC ZINT EaPidf_CapsMobsGetMob(ST_XML_ELEM *pstMobs, 
                ST_ZOS_USTR **ppstMob);

/* pidf caps set the servcaps */
ZFUNC ZINT EaPidf_CapsSetSCaps(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs,
                ST_XML_ELEM **ppstSCaps);
				
/* pidf caps set the servcaps */
ZFUNC ZINT EaPidf_CapsSetSCapsX(ST_XML_ELEM *pstElem,
                ST_XML_ELEM **ppstSCaps);

/* pidf caps set the servcaps actor */
ZFUNC ZINT EaPidf_CapsSCapsSetActor(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstActor);

/* pidf caps set the servcaps application */
ZFUNC ZINT EaPidf_CapsSCapsSetApp(ST_XML_ELEM *pstSCaps, 
                ZBOOL bApp);

/* pidf caps set the servcaps audio */
ZFUNC ZINT EaPidf_CapsSCapsSetAudio(ST_XML_ELEM *pstSCaps, 
                ZBOOL bAudio);

/* pidf caps set the servcaps automata */
ZFUNC ZINT EaPidf_CapsSCapsSetAuto(ST_XML_ELEM *pstSCaps, 
                ZBOOL bAuto);

/* pidf caps set the servcaps class */
ZFUNC ZINT EaPidf_CapsSCapsSetClass(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstClass);

/* pidf caps set the servcaps control */
ZFUNC ZINT EaPidf_CapsSCapsSetCtrl(ST_XML_ELEM *pstSCaps, 
                ZBOOL bCtrl);

/* pidf caps set the servcaps data */
ZFUNC ZINT EaPidf_CapsSCapsSetData(ST_XML_ELEM *pstSCaps, 
                ZBOOL bData);

/* pidf caps set the servcaps description */
ZFUNC ZINT EaPidf_CapsSCapsSetDesc(ST_XML_ELEM *pstSCaps, 
                ST_ZOS_USTR *pstDesc);

/* pidf caps set the servcaps duplex */
ZFUNC ZINT EaPidf_CapsSCapsSetDuplex(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstDuplex);

/* pidf caps set the servcaps event-packages */
ZFUNC ZINT EaPidf_CapsSCapsSetEvntPkgs(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstEvntPkgs);

/* pidf caps set the servcaps extensions */
ZFUNC ZINT EaPidf_CapsSCapsSetExtn(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstExtn);

/* pidf caps set the servcaps isfocus */
ZFUNC ZINT EaPidf_CapsSCapsSetIsfocus(ST_XML_ELEM *pstSCaps, 
                ZBOOL bIsfocus);

/* pidf caps set the servcaps message */
ZFUNC ZINT EaPidf_CapsSCapsSetMsg(ST_XML_ELEM *pstSCaps, 
                ZBOOL bMsg);

/* pidf caps set the servcaps methods */
ZFUNC ZINT EaPidf_CapsSCapsSetMethods(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstMethods);

/* pidf caps set the servcaps languages */
ZFUNC ZINT EaPidf_CapsSCapsSetLangs(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstLangs);

/* pidf caps set the servcaps priority */
ZFUNC ZINT EaPidf_CapsSCapsSetPriority(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstPriority);

/* pidf caps set the servcaps schemes */
ZFUNC ZINT EaPidf_CapsSCapsSetSchemes(ST_XML_ELEM *pstSCaps, 
                ST_XML_ELEM **ppstSchemes);

/* pidf caps set the servcaps text */
ZFUNC ZINT EaPidf_CapsSCapsSetTxt(ST_XML_ELEM *pstSCaps, 
                ZBOOL bTxt);

/* pidf caps set the servcaps type */
ZFUNC ZINT EaPidf_CapsSCapsSetType(ST_XML_ELEM *pstSCaps, 
                ST_ZOS_USTR *pstType);

/* pidf caps set the servcaps video */
ZFUNC ZINT EaPidf_CapsSCapsSetVideo(ST_XML_ELEM *pstSCaps, 
                ZBOOL bVideo);

/* pidf caps set the devcaps */
ZFUNC ZINT EaPidf_CapsSetDCaps(ST_XML_ELEM *pstMsg, 
                ST_XML_ELEM **ppstDCaps);

/* pidf caps set the devcaps description */
ZFUNC ZINT EaPidf_CapsDCapsSetDesc(ST_XML_ELEM *pstDCaps, 
                ST_ZOS_USTR *pstDesc);

/* pidf caps set the devcaps mobility */
ZFUNC ZINT EaPidf_CapsDCapsSetMob(ST_XML_ELEM *pstDCaps, 
                ST_XML_ELEM **ppstMob);

/* pidf caps set the class supported */
ZFUNC ZINT EaPidf_CapsClassSetSupt(ST_XML_ELEM *pstClass, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the class notsupported */
ZFUNC ZINT EaPidf_CapsClassSetNotSupt(ST_XML_ELEM *pstClass, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the classs business */
ZFUNC ZINT EaPidf_CapsClasssSetBiz(ST_XML_ELEM *pstClasss, 
                ST_ZOS_USTR *pstBiz);

/* pidf caps set the classs personal */
ZFUNC ZINT EaPidf_CapsClasssSetPersonal(ST_XML_ELEM *pstClasss, 
                ST_ZOS_USTR *pstPersonal);

/* pidf caps set the duplex supported */
ZFUNC ZINT EaPidf_CapsDuplexSetSupt(ST_XML_ELEM *pstDuplex, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the duplex notsupported */
ZFUNC ZINT EaPidf_CapsDuplexSetNotSupt(ST_XML_ELEM *pstDuplex, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the duplexs full */
ZFUNC ZINT EaPidf_CapsDuplexsSetFull(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR *pstFull);

/* pidf caps set the duplexs half */
ZFUNC ZINT EaPidf_CapsDuplexsSetHalf(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR *pstHalf);

/* pidf caps set the duplexs receive-only */
ZFUNC ZINT EaPidf_CapsDuplexsSetRecvOnly(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR *pstRecvOnly);

/* pidf caps set the duplexs send-only */
ZFUNC ZINT EaPidf_CapsDuplexsSetSendOnly(ST_XML_ELEM *pstDuplexs, 
                ST_ZOS_USTR *pstSendOnly);

/* pidf caps set the event-packages supported */
ZFUNC ZINT EaPidf_CapsEvntPkgsSetSupt(ST_XML_ELEM *pstEvntPkgs, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the event-packages notsupported */
ZFUNC ZINT EaPidf_CapsEvntPkgsSetNotSupt(ST_XML_ELEM *pstEvntPkgs, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the event message-summary */
ZFUNC ZINT EaPidf_CapsEvntSetMsgSummary(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstMsgSummary);

/* pidf caps set the event presence */
ZFUNC ZINT EaPidf_CapsEvntSetPres(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstPres);

/* pidf caps set the event refer */
ZFUNC ZINT EaPidf_CapsEvntSetRefer(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstRefer);

/* pidf caps set the event reg */
ZFUNC ZINT EaPidf_CapsEvntSetReg(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstReg);

/* pidf caps set the event Siemens-RTP-Stats */
ZFUNC ZINT EaPidf_CapsEvntSetSRtpStat(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstSRtpStat);

/* pidf caps set the event spirits-INDPs */
ZFUNC ZINT EaPidf_CapsEvntSetSIndp(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstSIndp);

/* pidf caps set the event spirits-user-prof */
ZFUNC ZINT EaPidf_CapsEvntSetSUsrProf(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstSUsrProf);

/* pidf caps set the event winfo */
ZFUNC ZINT EaPidf_CapsEvntSetWinfo(ST_XML_ELEM *pstEvnt, 
                ST_ZOS_USTR *pstWinfo);

/* pidf caps set the prioritysupported */
ZFUNC ZINT EaPidf_CapsPrioritySetSupt(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the priority notsupported */
ZFUNC ZINT EaPidf_CapsPrioritySetNotSupt(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the priority equals */
ZFUNC ZINT EaPidf_CapsPrioritySetEqual(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstEqual);

/* pidf caps set the priority higherhan */
ZFUNC ZINT EaPidf_CapsPrioritySetHigher(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstHigher);

/* pidf caps set the priority lowerthan */
ZFUNC ZINT EaPidf_CapsPrioritySetLower(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstLower);

/* pidf caps set the priority range */
ZFUNC ZINT EaPidf_CapsPrioritySetRange(ST_XML_ELEM *pstPriority, 
                ST_XML_ELEM **ppstRange);

/* pidf caps set the equals value */
ZFUNC ZINT EaPidf_CapsEqualSetVal(ST_XML_ELEM *pstEqual, 
                ZUINT iVal);

/* pidf caps set the higherhan minvalue */
ZFUNC ZINT EaPidf_CapsHigherSetMinVal(ST_XML_ELEM *pstHigher, 
                ZUINT iMinVal);

/* pidf caps set the lowerthan maxvalue */
ZFUNC ZINT EaPidf_CapsLowerSetMaxVal(ST_XML_ELEM *pstLower, 
                ZUINT iMaxVal);

/* pidf caps set the range minvalue */
ZFUNC ZINT EaPidf_CapsRangeSetMinVal(ST_XML_ELEM *pstRange, 
                ZUINT iMinVal);

/* pidf caps set the range maxvalue */
ZFUNC ZINT EaPidf_CapsRangeSetMaxVal(ST_XML_ELEM *pstRange, 
                ZUINT iMaxVal);

/* pidf caps set the methods supported */
ZFUNC ZINT EaPidf_CapsMethodsSetSupt(ST_XML_ELEM *pstMethods, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the methods notsupported */
ZFUNC ZINT EaPidf_CapsMethodsSetNotSupt(ST_XML_ELEM *pstMethods, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the method ACK */
ZFUNC ZINT EaPidf_CapsMethodSetAck(ST_XML_ELEM *pstMethod);

/* pidf caps set the method BYE */
ZFUNC ZINT EaPidf_CapsMethodSetBye(ST_XML_ELEM *pstMethod);

/* pidf caps set the method CANCEL */
ZFUNC ZINT EaPidf_CapsMethodSetCancel(ST_XML_ELEM *pstMethod);

/* pidf caps set the method INFO*/
ZFUNC ZINT EaPidf_CapsMethodSetInfo(ST_XML_ELEM *pstMethod);

/* pidf caps set the method INVITE */
ZFUNC ZINT EaPidf_CapsMethodSetIvt(ST_XML_ELEM *pstMethod);

/* pidf caps set the method MESSAGE */
ZFUNC ZINT EaPidf_CapsMethodSetMsg(ST_XML_ELEM *pstMethod);

/* pidf caps set the method NOTIFY */
ZFUNC ZINT EaPidf_CapsMethodSetNtfy(ST_XML_ELEM *pstMethod);

/* pidf caps set the method OPTIONS */
ZFUNC ZINT EaPidf_CapsMethodSetOpt(ST_XML_ELEM *pstMethod);

/* pidf caps set the method PRACK */
ZFUNC ZINT EaPidf_CapsMethodSetPrack(ST_XML_ELEM *pstMethod);

/* pidf caps set the method PUBLISH */
ZFUNC ZINT EaPidf_CapsMethodSetPub(ST_XML_ELEM *pstMethod);

/* pidf caps set the method REFER */
ZFUNC ZINT EaPidf_CapsMethodSetRefer(ST_XML_ELEM *pstMethod);

/* pidf caps set the method REGISTER */
ZFUNC ZINT EaPidf_CapsMethodSetReg(ST_XML_ELEM *pstMethod);

/* pidf caps set the method SUBSCRIBE */
ZFUNC ZINT EaPidf_CapsMethodSetSubs(ST_XML_ELEM *pstMethod);

/* pidf caps set the method UPDATE */
ZFUNC ZINT EaPidf_CapsMethodSetUpt(ST_XML_ELEM *pstMethod);

/* pidf caps set the extensions supported */
ZFUNC ZINT EaPidf_CapsExtnsSetSupt(ST_XML_ELEM *pstExtns, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the extensions notsupported */
ZFUNC ZINT EaPidf_CapsExtnsSetNotSupt(ST_XML_ELEM *pstExtns, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the extension join */
ZFUNC ZINT EaPidf_CapsExtnSetJoin(ST_XML_ELEM *pstExtn);

/* pidf caps set the extension path */
ZFUNC ZINT EaPidf_CapsExtnSetPath(ST_XML_ELEM *pstExtn);

/* pidf caps set the extension precondition */
ZFUNC ZINT EaPidf_CapsExtnSetPrecond(ST_XML_ELEM *pstExtn);

/* pidf caps set the extension pref */
ZFUNC ZINT EaPidf_CapsExtnSetPref(ST_XML_ELEM *pstExtn);

/* pidf caps set the extension privacy */
ZFUNC ZINT EaPidf_CapsExtnSetPrivacy(ST_XML_ELEM *pstExtn);

/* pidf caps set the extension rel100 */
ZFUNC ZINT EaPidf_CapsExtnSetRel100(ST_XML_ELEM *pstExtn);

/* pidf caps set the extension replaces */
ZFUNC ZINT EaPidf_CapsExtnSetRpl(ST_XML_ELEM *pstExtn);

/* pidf caps set the extension sec-agree */
ZFUNC ZINT EaPidf_CapsExtnSetSecAgree(ST_XML_ELEM *pstExtn);

/* pidf caps set the schemes supported */
ZFUNC ZINT EaPidf_CapsSchemesSetSupt(ST_XML_ELEM *pstSchemes, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the schemes notsupported */
ZFUNC ZINT EaPidf_CapsSchemesSetNotSupt(ST_XML_ELEM *pstSchemes, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the scheme s */
ZFUNC ZINT EaPidf_CapsSchemeSetS(ST_XML_ELEM *pstScheme, 
                ST_ZOS_USTR *pstS);

/* pidf caps set the actor supported */
ZFUNC ZINT EaPidf_CapsActorSetSupt(ST_XML_ELEM *pstActor, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the actor notsupported */
ZFUNC ZINT EaPidf_CapsActorSetNotSupt(ST_XML_ELEM *pstActor, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the actorts attendant */
ZFUNC ZINT EaPidf_CapsActorsSetAttend(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR *pstAttend);

/* pidf caps set the actorts information */
ZFUNC ZINT EaPidf_CapsActorsSetInfo(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR *pstInfo);

/* pidf caps set the actorts msg-taker */
ZFUNC ZINT EaPidf_CapsActorsSetMsgTaker(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR *pstMsgTaker);

/* pidf caps set the actorts principal */
ZFUNC ZINT EaPidf_CapsActorsSetPrinc(ST_XML_ELEM *pstActors, 
                ST_ZOS_USTR *pstPrinc);

/* pidf caps set the languages supported */
ZFUNC ZINT EaPidf_CapsLangsSetSupt(ST_XML_ELEM *pstLangs, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the languages notsupported */
ZFUNC ZINT EaPidf_CapsLangsSetNotSupt(ST_XML_ELEM *pstLangs, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the language l */
ZFUNC ZINT EaPidf_CapsLangSetL(ST_XML_ELEM *pstLang, 
                ST_ZOS_USTR *pstL);

/* pidf caps set the mobility supported */
ZFUNC ZINT EaPidf_CapsMobSetSupt(ST_XML_ELEM *pstMob, 
                ST_XML_ELEM **ppstSupt);

/* pidf caps set the mobility notsupported */
ZFUNC ZINT EaPidf_CapsMobSetNotSupt(ST_XML_ELEM *pstMob, 
                ST_XML_ELEM **ppstNotSupt);

/* pidf caps set the mobilitys fixed */
ZFUNC ZINT EaPidf_CapsMobsSetFix(ST_XML_ELEM *pstMobs, 
                ST_ZOS_USTR *pstFix);

/* pidf caps set the mobilitys mobile */
ZFUNC ZINT EaPidf_CapsMobsSetMob(ST_XML_ELEM *pstMobs, 
                ST_ZOS_USTR *pstMob);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_CAPS_H__ */

