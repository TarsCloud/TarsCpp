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
  File name     : ea_wat_info.h
  Module        : eax watcherinfo
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax watcherinfo.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_WAT_INFO_H__
#define _EA_WAT_INFO_H__

#ifdef __cplusplus
extern "C"{
#endif

/* watcherinfo state type */
typedef enum EN_EA_WAT_INFO_STA_TYPE
{
    EN_EA_WAT_INFO_STA_FULL,       
    EN_EA_WAT_INFO_STA_PARTIAL
} EN_EA_WAT_INFO_STA_TYPE;

/* watcherinfo status type */
typedef enum EN_EA_WAT_INFO_STAT_TYPE
{
    EN_EA_WAT_INFO_STAT_PENDING,       
    EN_EA_WAT_INFO_STAT_ACTIVE,
    EN_EA_WAT_INFO_STAT_WAITING,
    EN_EA_WAT_INFO_STAT_TERM
} EN_EA_WAT_INFO_STAT_TYPE;

/* watcherinfo event type */
typedef enum EN_EA_WAT_INFO_EVNT_TYPE
{
    EN_EA_WAT_INFO_EVNT_SUBS,       
    EN_EA_WAT_INFO_EVNT_APPROV,
    EN_EA_WAT_INFO_EVNT_DEACTIVE,
    EN_EA_WAT_INFO_EVNT_PROBATION,
    EN_EA_WAT_INFO_EVNT_REJECT,
    EN_EA_WAT_INFO_EVNT_TIMEOUT,
    EN_EA_WAT_INFO_EVNT_GIVEUP,
    EN_EA_WAT_INFO_EVNT_NORES    
} EN_EA_WAT_INFO_EVNT_TYPE;

/* watcherinfo get the watcherinfo */
ZFUNC ZINT EaWat_InfoGetInfo(ST_XML_MSG *pstMsg, ST_XML_ELEM **ppstInfo);

/* watcherinfo get the first watcherinfo watcher-list */
ZFUNC ZINT EaWat_InfoInfoGetFirstLst(ST_XML_ELEM *pstInfo, 
                ST_XML_ELEM **ppstLst);

/* watcherinfo get the next watcherinfo watcher-list */
ZFUNC ZINT EaWat_InfoInfoGetNextLst(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstLst);

/* watcherinfo get the watcherinfo version */
ZFUNC ZINT EaWat_InfoInfoGetVer(ST_XML_ELEM *pstInfo, ZUINT *piVer);

/* watcherinfo get the watcherinfo state EN_EA_WAT_INFO_STA_TYPE */
ZFUNC ZINT EaWat_InfoInfoGetSta(ST_XML_ELEM *pstInfo, ZUCHAR *pucSta);

/* watcherinfo get the first watcher-list watcher */
ZFUNC ZINT EaWat_InfoLstGetFirstWat(ST_XML_ELEM *pstLst, 
                ST_XML_ELEM **ppstWat);

/* watcherinfo get the next watcher-list watcher */
ZFUNC ZINT EaWat_InfoLstGetNextWat(ST_XML_ELEM *pstWat, 
                ST_XML_ELEM **ppstWat);

/* watcherinfo get the watcher-list resource */
ZFUNC ZINT EaWat_InfoLstGetRes(ST_XML_ELEM *pstLst, ST_ZOS_USTR **ppstRes);

/* watcherinfo get the watcher-list package */
ZFUNC ZINT EaWat_InfoLstGetPkg(ST_XML_ELEM *pstLst, ST_ZOS_USTR **ppstPkg);

/* watcherinfo get the watcher display-name */
ZFUNC ZINT EaWat_InfoWatGetDispName(ST_XML_ELEM *pstWat, 
                ST_ZOS_USTR **ppstDispName);

/* watcherinfo get the watcher status EN_EA_WAT_INFO_STAT_TYPE */
ZFUNC ZINT EaWat_InfoWatGetStat(ST_XML_ELEM *pstWat, ZUCHAR *pucStat);

/* watcherinfo get the watcher event */
ZFUNC ZINT EaWat_InfoWatGetEvnt(ST_XML_ELEM *pstWat, ZUCHAR *pucEvnt);

/* watcherinfo get the watcher expiration */
ZFUNC ZINT EaWat_InfoWatGetExpire(ST_XML_ELEM *pstWat, ZUINT *piExpire);

/* watcherinfo get the watcher id */
ZFUNC ZINT EaWat_InfoWatGetId(ST_XML_ELEM *pstWat, ST_ZOS_USTR **ppstId);

/* watcherinfo get the watcher duration-subscribed */
ZFUNC ZINT EaWat_InfoWatGetDurSubs(ST_XML_ELEM *pstWat, 
                ZUINT *piDurSubs);

/* watcherinfo get the watcher lang */
ZFUNC ZINT EaWat_InfoWatGetLang(ST_XML_ELEM *pstWat, 
                ST_ZOS_USTR **ppstLang);

/* watcherinfo set the watcherinfo */
ZFUNC ZINT EaWat_InfoSetInfo(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs,
                ST_XML_ELEM **ppstInfo);

/* watcherinfo set the watcherinfo watcher-list */
ZFUNC ZINT EaWat_InfoInfoSetLst(ST_XML_ELEM *pstInfo, ST_XML_ELEM **ppstLst);

/* watcherinfo set the watcherinfo version */
ZFUNC ZINT EaWat_InfoInfoSetVer(ST_XML_ELEM *pstInfo, ZUINT iVer);

/* watcherinfo set the watcherinfo state EN_EA_WAT_INFO_STA_TYPE */
ZFUNC ZINT EaWat_InfoInfoSetSta(ST_XML_ELEM *pstInfo, ZUCHAR ucSta);

/* watcherinfo set the watcher-list watcher */
ZFUNC ZINT EaWat_InfoLstSetWat(ST_XML_ELEM *pstLst, ST_XML_ELEM **ppstWat);

/* watcherinfo set the watcher-list resource */
ZFUNC ZINT EaWat_InfoLstSetRes(ST_XML_ELEM *pstLst, ST_ZOS_USTR *pstRes);

/* watcherinfo set the watcher-list package */
ZFUNC ZINT EaWat_InfoLstSetPkg(ST_XML_ELEM *pstLst, ST_ZOS_USTR *pstPkg);

/* watcherinfo set the watcher display-name */
ZFUNC ZINT EaWat_InfoWatSetDispName(ST_XML_ELEM *pstWat, 
                ST_ZOS_USTR *pstDispName);

/* watcherinfo set the watcher status EN_EA_WAT_INFO_STAT_TYPE */
ZFUNC ZINT EaWat_InfoWatSetStat(ST_XML_ELEM *pstWat, ZUCHAR ucStat);

/* watcherinfo set the watcher event */
ZFUNC ZINT EaWat_InfoWatSetEvnt(ST_XML_ELEM *pstWat, ZUCHAR ucEvnt);

/* watcherinfo set the watcher expiration */
ZFUNC ZINT EaWat_InfoWatSetExpire(ST_XML_ELEM *pstWat, ZUINT iExpire);

/* watcherinfo set the watcher id */
ZFUNC ZINT EaWat_InfoWatSetId(ST_XML_ELEM *pstWat, ST_ZOS_USTR *pstId);

/* watcherinfo set the watcher duration-subscribed */
ZFUNC ZINT EaWat_InfoWatSetDurSubs(ST_XML_ELEM *pstWat, ZUINT iDurSubs);

/* watcherinfo set the watcher lang */
ZFUNC ZINT EaWat_InfoWatSetLang(ST_XML_ELEM *pstWat, ST_ZOS_USTR *pstLang);

#ifdef __cplusplus
}
#endif

#endif /* _EA_WAT_INFO_H__ */

