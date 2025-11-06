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
  File name     : ea_pres_rules.h
  Module        : eax pres-rules
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pres-rules.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PRES_RULES_H__
#define _EA_PRES_RULES_H__

#ifdef __cplusplus
extern "C"{
#endif

/* pres-rules provide-user-input type */
typedef enum EN_EA_PRES_PUI_TYPE
{
    EN_EA_PRES_PUI_NULL,
    EN_EA_PRES_PUI_FALSE,
    EN_EA_PRES_PUI_BARE,
    EN_EA_PRES_PUI_THRESHOLDS,
    EN_EA_PRES_PUI_FULL
} EN_EA_PRES_PUI_TYPE;

/* pres-rules sub-handling type */
typedef enum EN_EA_PRES_SUB_HDL_TYPE
{
    EN_EA_PRES_SUB_HDL_BLK,
    EN_EA_PRES_SUB_HDL_CNF,
    EN_EA_PRES_SUB_HDL_POL_BLK,
    EN_EA_PRES_SUB_HDL_ALLOW
} EN_EA_PRES_SUB_HDL_TYPE;

/* pres-rules get the service-uri-scheme */
ZFUNC ZINT EaPres_RulesGetSrvUriSchema(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstSchema);

/* pres-rules get the class */
ZFUNC ZINT EaPres_RulesGetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstClass);

/* pres-rules get the occurrence-id */
ZFUNC ZINT EaPres_RulesGetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstOccurId);

/* pres-rules get the service-uri */
ZFUNC ZINT EaPres_RulesGetSrvUri(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstUri);

/* pres-rules get the provideServicePermission all-services */
ZFUNC ZINT EaPres_RulesPSPGetAllSrv(ST_XML_ELEM *pstElem, 
                ZBOOL *pbExist);

/* pres-rules get the provideServicePermission service-uri */
ZFUNC ZINT EaPres_RulesPSPGetSrvUri(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstUri);

/* pres-rules get the provideServicePermission service-uri-scheme */
ZFUNC ZINT EaPres_RulesPSPGetSrvUriSchema(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstSchema);

/* pres-rules get the provideServicePermission occurrence-id */
ZFUNC ZINT EaPres_RulesPSPGetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstOccurId);

/* pres-rules get the provideServicePermission class */
ZFUNC ZINT EaPres_RulesPSPGetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstClass);

/* pres-rules get the provide-services */
ZFUNC ZINT EaPres_RulesGetPvdSrv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* pres-rules get the deviceID */
ZFUNC ZINT EaPres_RulesGetDevId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstDevId);

/* pres-rules get the provideDevicePermission all-devices */
ZFUNC ZINT EaPres_RulesPDPGetAllDevs(ST_XML_ELEM *pstElem, 
                ZBOOL *pbExist);

/* pres-rules get the provideDevicePermission deviceID */
ZFUNC ZINT EaPres_RulesPDPGetDevId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstDevId);

/* pres-rules get the provideDevicePermission occurrence-id */
ZFUNC ZINT EaPres_RulesPDPGetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstOccurId);

/* pres-rules get the provideDevicePermission class */
ZFUNC ZINT EaPres_RulesPDPGetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstClass);

/* pres-rules get the provide-devices */
ZFUNC ZINT EaPres_RulesGetPvdDev(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* pres-rules get the providePersonPermission all-persons */
ZFUNC ZINT EaPres_RulesPPPGetAllPerson(ST_XML_ELEM *pstElem, 
                ZBOOL *pbExist);

/* pres-rules get the providePersonPermission occurrence-id */
ZFUNC ZINT EaPres_RulesPPPGetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstOccurId);

/* pres-rules get the providePersonPermission class */
ZFUNC ZINT EaPres_RulesPPPGetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstClass);

/* pres-rules get the provide-persons */
ZFUNC ZINT EaPres_RulesGetPvdPerson(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPerson);

/* pres-rules get the provide-activities */
ZFUNC ZINT EaPres_RulesGetPvdAct(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-class */
ZFUNC ZINT EaPres_RulesGetPvdClass(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-deviceID */
ZFUNC ZINT EaPres_RulesGetPvdDevId(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-mood */
ZFUNC ZINT EaPres_RulesGetPvdMood(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-place-is */
ZFUNC ZINT EaPres_RulesGetPvdPlaceIs(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-place-type */
ZFUNC ZINT EaPres_RulesGetPvdPlaceType(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-privacy */
ZFUNC ZINT EaPres_RulesGetPvdPrivacy(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-relationship */
ZFUNC ZINT EaPres_RulesGetPvdRelationship(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-status-icon */
ZFUNC ZINT EaPres_RulesGetPvdStatIcon(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-sphere */
ZFUNC ZINT EaPres_RulesGetPvdSphere(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-time-offset */
ZFUNC ZINT EaPres_RulesGetPvdTimeOffset(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-user-input EN_EA_PRES_PUI_TYPE */
ZFUNC ZINT EaPres_RulesGetPvdUsrInput(ST_XML_ELEM *pstElem, 
                ZUCHAR *pucType);

/* pres-rules get the provide-note */
ZFUNC ZINT EaPres_RulesGetPvdNote(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* pres-rules get the sub-handling EN_EA_PRES_SUB_HDL_TYPE */
ZFUNC ZINT EaPres_RulesGetSubHandl(ST_XML_ELEM *pstElem, 
                ZUCHAR *pucType);

/* pres-rules get the unknownBooleanPermission */
ZFUNC ZINT EaPres_RulesGetUnknownPermission(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstName, ST_ZOS_USTR **ppstNs, 
                ZBOOL *pbPermission);

/* pres-rules get the provide-unknown-attribute */
ZFUNC ZINT EaPres_RulesGetPvdUnknownAttr(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* pres-rules get the provide-all-attributes */
ZFUNC ZINT EaPres_RulesGetPvdAllAttr(ST_XML_ELEM *pstElem, 
                ZBOOL *pbExist);

/* pres-rules set the service-uri-scheme */
ZFUNC ZINT EaPres_RulesSetSrvUriSchema(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstSchema);

/* pres-rules set the class */
ZFUNC ZINT EaPres_RulesSetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstClass);

/* pres-rules set the occurrence-id */
ZFUNC ZINT EaPres_RulesSetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstOccurId);

/* pres-rules set the service-uri */
ZFUNC ZINT EaPres_RulesSetSrvUri(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *stUri);

/* pres-rules set the provideServicePermission all-services */
ZFUNC ZINT EaPres_RulesPSPSetAllSrv(ST_XML_ELEM *pstElem);

/* pres-rules set the provideServicePermission service-uri */
ZFUNC ZINT EaPres_RulesPSPSetSrvUri(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstUri);

/* pres-rules set the provideServicePermission service-uri-scheme */
ZFUNC ZINT EaPres_RulesPSPSetSrvUriSchema(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstSchema);

/* pres-rules set the provideServicePermission occurrence-id */
ZFUNC ZINT EaPres_RulesPSPSetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstOccurId);

/* pres-rules set the provideServicePermission class */
ZFUNC ZINT EaPres_RulesPSPSetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstClass);

/* pres-rules set the provide-services */
ZFUNC ZINT EaPres_RulesSetPvdSrv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* pres-rules set the deviceID */
ZFUNC ZINT EaPres_RulesSetDevId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstDevId);

/* pres-rules set the provideDevicePermission all-devices */
ZFUNC ZINT EaPres_RulesPDPSetAllDevs(ST_XML_ELEM *pstElem);

/* pres-rules set the provideDevicePermission deviceID */
ZFUNC ZINT EaPres_RulesPDPSetDevId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstDevId);

/* pres-rules set the provideDevicePermission occurrence-id */
ZFUNC ZINT EaPres_RulesPDPSetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstOccurId);

/* pres-rules set the provideDevicePermission class */
ZFUNC ZINT EaPres_RulesPDPSetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstClass);

/* pres-rules set the provide-devices */
ZFUNC ZINT EaPres_RulesSetPvdDev(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* pres-rules set the providePersonPermission all-persons */
ZFUNC ZINT EaPres_RulesPPPSetAllPerson(ST_XML_ELEM *pstElem);

/* pres-rules set the providePersonPermission occurrence-id */
ZFUNC ZINT EaPres_RulesPPPSetOccurId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstOccurId);

/* pres-rules set the providePersonPermission class */
ZFUNC ZINT EaPres_RulesPPPSetClass(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstClass);

/* pres-rules set the provide-persons */
ZFUNC ZINT EaPres_RulesSetPvdPerson(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* pres-rules set the provide-activities */
ZFUNC ZINT EaPres_RulesSetPvdAct(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-class */
ZFUNC ZINT EaPres_RulesSetPvdClass(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-deviceID */
ZFUNC ZINT EaPres_RulesSetPvdDevId(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-mood */
ZFUNC ZINT EaPres_RulesSetPvdMood(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-place-is */
ZFUNC ZINT EaPres_RulesSetPvdPlaceIs(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-place-type */
ZFUNC ZINT EaPres_RulesSetPvdPlaceType(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-privacy */
ZFUNC ZINT EaPres_RulesSetPvdPrivacy(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-relationship */
ZFUNC ZINT EaPres_RulesSetPvdRelationship(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-status-icon */
ZFUNC ZINT EaPres_RulesSetPvdStatIcon(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-sphere */
ZFUNC ZINT EaPres_RulesSetPvdSphere(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-time-offset */
ZFUNC ZINT EaPres_RulesSetPvdTimeOffset(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the provide-user-input EN_EA_PRES_PUI_TYPE */
ZFUNC ZINT EaPres_RulesSetPvdUsrInput(ST_XML_ELEM *pstElem, 
                ZUCHAR ucType);

/* pres-rules set the provide-note */
ZFUNC ZINT EaPres_RulesSetPvdNote(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* pres-rules set the sub-handling EN_EA_PRES_SUB_HDL_TYPE */
ZFUNC ZINT EaPres_RulesSetSubHandl(ST_XML_ELEM *pstElem, ZUCHAR ucType);

/* pres-rules set the unknownBooleanPermission */
ZFUNC ZINT EaPres_RulesSetUnknownPermission(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstName, ST_ZOS_USTR *pstNs, ZBOOL bPermission);

/* pres-rules set the provide-unknown-attribute */
ZFUNC ZINT EaPres_RulesSetPvdUnknownAttr(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstElem);

/* pres-rules set the provide-all-attributes */
ZFUNC ZINT EaPres_RulesSetPvdAllAttr(ST_XML_ELEM *pstElem);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PRES_RULES_H__ */

