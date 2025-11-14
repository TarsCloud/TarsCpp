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
  File name     : ea_prs_pres.h
  Module        : eax oma-pres
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax oma-pres.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PRS_PRES_H__
#define _EA_PRS_PRES_H__

#ifdef __cplusplus
extern "C"{
#endif

/* oma-pres basic type */
typedef enum EN_EA_PRS_PRES_BASIC_TYPE
{
    EN_EA_PRS_PRES_BASIC_OPEN,
    EN_EA_PRS_PRES_BASIC_CLOSED
} EN_EA_PRS_PRES_BASIC_TYPE;

/* oma-pres registration type */
typedef enum EN_EA_PRS_PRES_REG_TYPE
{
    EN_EA_PRS_PRES_REG_ACTIVE,
    EN_EA_PRS_PRES_REG_TERM
} EN_EA_PRS_PRES_REG_TYPE;

/* oma-pres barring type */
typedef enum EN_EA_PRS_PRES_BARR_TYPE
{
    EN_EA_PRS_PRES_BARR_ACTIVE,
    EN_EA_PRS_PRES_BARR_TERM
} EN_EA_PRS_PRES_BARR_TYPE;

/* oma-pres network type */
typedef enum EN_EA_PRS_PRES_NET_TYPE
{
    EN_EA_PRS_PRES_NET_ACTIVE,
    EN_EA_PRS_PRES_NET_TERM
} EN_EA_PRS_PRES_NET_TYPE;

/* oma-pres get the service-description */
ZFUNC ZINT EaPrs_PresGetSrvDesc(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSrvDesc);

/* oma-pres get the service-description service-id */
ZFUNC ZINT EaPrs_PresSrvDescGetSrvId(ST_XML_ELEM *pstSrvDesc, 
                ST_ZOS_USTR **ppstSrvId);

/* oma-pres get the service-description version */
ZFUNC ZINT EaPrs_PresSrvDescGetVer(ST_XML_ELEM *pstSrvDesc, 
                ST_ZOS_USTR **ppstVer);

/* oma-pres get the service-description description */
ZFUNC ZINT EaPrs_PresSrvDescGetDesc(ST_XML_ELEM *pstSrvDesc, 
                ST_ZOS_USTR **ppstDesc);

/* oma-pres get the willingness */
ZFUNC ZINT EaPrs_PresGetWill(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstWill);

/* oma-pres get the willingness basic */
ZFUNC ZINT EaPrs_PresWillGetBasic(ST_XML_ELEM *pstWill, 
                ST_XML_ELEM **ppstBasic);

/* oma-pres get the session-participation */
ZFUNC ZINT EaPrs_PresGetSessPartp(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSessPartp);

/* oma-pres get the session-participation basic */
ZFUNC ZINT EaPrs_PresSessPartpGetBasic(ST_XML_ELEM *pstSessPartp, 
                ST_XML_ELEM **ppstBasic);

/* oma-pres get the registration-state */
ZFUNC ZINT EaPrs_PresGetRegSta(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRegSta);

/* oma-pres get the barring-state */
ZFUNC ZINT EaPrs_PresGetBarrSta(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstBarrSta);

/* oma-pres get the overriding-willingness */
ZFUNC ZINT EaPrs_PresGetOverridWill(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstOverridWill);

/* oma-pres get the overriding-willingness basic */
ZFUNC ZINT EaPrs_PresOverridWillGetBasic(ST_XML_ELEM *OverridWill, 
                ST_XML_ELEM **ppstBasic);

/* oma-pres get the network-availability */
ZFUNC ZINT EaPrs_PresGetNetAvail(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstNetAvail);

/* xcap-directory get the first network-availability network */
ZFUNC ZINT EaPrs_PresNetAvailGetFirstNet(ST_XML_ELEM *pstNetAvail, 
                ST_XML_ELEM **ppstNet);

/* xcap-directory get the next network-availability network */
ZFUNC ZINT EaPrs_PresNetAvailGetNextNet(ST_XML_ELEM *pstNet, 
                ST_XML_ELEM **ppstNet);

/* oma-pres get the basic type EN_EA_PRS_PRES_BASIC_TYPE */
ZFUNC ZINT EaPrs_PresBasicGetType(ST_XML_ELEM *pstBasic, ZUCHAR *pucType);

/* oma-pres get the registration type EN_EA_PRS_PRES_REG_TYPE */
ZFUNC ZINT EaPrs_PresRegGetType(ST_XML_ELEM *pstReg, ZUCHAR *pucType);

/* oma-pres get the barring type EN_EA_PRS_PRES_BARR_TYPE */
ZFUNC ZINT EaPrs_PresBarrGetType(ST_XML_ELEM *pstBarr, ZUCHAR *pucType);

/* oma-pres get the network type EN_EA_PRS_PRES_NET_TYPE */
ZFUNC ZINT EaPrs_PresNetGetType(ST_XML_ELEM *pstNet, ZUCHAR *pucType);

/* oma-pres get the network id */
ZFUNC ZINT EaPrs_PresNetGetId(ST_XML_ELEM *pstNet, 
                ST_ZOS_USTR **ppstId);

/* oma-pres get the willingness or overriding willingness until */
ZFUNC ZINT EaPrs_PresWillGetUntil(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstUntil);

/* oma-pres set the service-description */
ZFUNC ZINT EaPrs_PresSetSrvDesc(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSrvDesc);

/* oma-pres set the service-description service-id */
ZFUNC ZINT EaPrs_PresSrvDescSetSrvId(ST_XML_ELEM *pstSrvDesc, 
                ST_ZOS_USTR *pstSrvId);

/* oma-pres set the service-description version */
ZFUNC ZINT EaPrs_PresSrvDescSetVer(ST_XML_ELEM *pstSrvDesc, 
                ST_ZOS_USTR *pstVer);

/* oma-pres set the service-description description */
ZFUNC ZINT EaPrs_PresSrvDescSetDesc(ST_XML_ELEM *pstSrvDesc, 
                ST_ZOS_USTR *pstDesc);

/* oma-pres set the willingness */
ZFUNC ZINT EaPrs_PresSetWill(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstWill);

/* oma-pres set the willingness basic */
ZFUNC ZINT EaPrs_PresWillSetBasic(ST_XML_ELEM *pstWill, 
                ST_XML_ELEM **ppstBasic);

/* oma-pres set the session-participation */
ZFUNC ZINT EaPrs_PresSetSessPartp(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSessPartp);

/* oma-pres set the session-participation basic */
ZFUNC ZINT EaPrs_PresSessPartpSetBasic(ST_XML_ELEM *pstSessPartp, 
                ST_XML_ELEM **ppstBasic);

/* oma-pres set the registration-state */
ZFUNC ZINT EaPrs_PresSetRegSta(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRegSta);

/* oma-pres set the barring-state */
ZFUNC ZINT EaPrs_PresSetBarrSta(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstBarrSta);

/* oma-pres set the overriding-willingness */
ZFUNC ZINT EaPrs_PresSetOverridWill(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstOverridWill);

/* oma-pres set the overriding-willingness basic */
ZFUNC ZINT EaPrs_PresOverridWillSetBasic(ST_XML_ELEM *pstOverridWill, 
                ST_XML_ELEM **ppstBasic);

/* oma-pres set the network-availability */
ZFUNC ZINT EaPrs_PresSetNetAvail(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstNetAvail);

/* xcap-directory set the network-availability network */
ZFUNC ZINT EaPrs_PresNetAvailSetNet(ST_XML_ELEM *pstNetAvail, 
                ST_XML_ELEM **ppstNet);

/* oma-pres set the basic type EN_EA_PRS_PRES_BASIC_TYPE */
ZFUNC ZINT EaPrs_PresBasicSetType(ST_XML_ELEM *pstBasic, ZUCHAR ucType);

/* oma-pres set the registration type EN_EA_PRS_PRES_REG_TYPE */
ZFUNC ZINT EaPrs_PresRegSetType(ST_XML_ELEM *pstReg, ZUCHAR ucType);

/* oma-pres set the barring type EN_EA_PRS_PRES_BARR_TYPE */
ZFUNC ZINT EaPrs_PresBarrSetType(ST_XML_ELEM *pstBarr, ZUCHAR ucType);

/* oma-pres set the network type EN_EA_PRS_PRES_NET_TYPE */
ZFUNC ZINT EaPrs_PresNetSetType(ST_XML_ELEM *pstNet, ZUCHAR ucType);

/* oma-pres set the network id */
ZFUNC ZINT EaPrs_PresNetSetId(ST_XML_ELEM *pstNet, 
                ST_ZOS_USTR *pstId);

/* oma-pres get the willingness or overriding willingness until */
ZFUNC ZINT EaPrs_PresWillSetUntil(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstUntil);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PRS_PRES_H__ */

