/************************************************************************

        Copyright (c) 2005-2011 by Juphoon System Software, Inc.
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
  File name     : ea_im_his.h
  Module        : eax im history-list
  Author        : zocol
  Version       : 0.1
  Created on    : 2011-03-07
  Description   :
    Marcos and structure definitions required by the eax im history-list.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_IM_HIS_H__
#define _EA_IM_HIS_H__

#ifdef __cplusplus
extern "C"{
#endif

/********** urn:oma:xml:im:history-list *********/
typedef enum EN_EA_IM_HIS_TYPE
{
    EN_EA_IM_HIS_HISTORY_LST,        /* history-list */
    EN_EA_IM_HIS_HISTORY,            /* history */
    EN_EA_IM_HIS_SIZE,               /* size */
    EN_EA_IM_HIS_EXPIRY,             /* expiry */
    EN_EA_IM_HIS_SUBJECT,            /* subject */
    EN_EA_IM_HIS_PAGER,              /* pager */
    EN_EA_IM_HIS_CONF,               /* conference */
    EN_EA_IM_HIS_DATE,               /* date */
    EN_EA_IM_HIS_HISTORY_REFERENCE,  /* history-reference */
    EN_EA_IM_HIS_TIME_STAMP,         /* time-stamp */
    EN_EA_IM_HIS_FROM,               /* from */
    EN_EA_IM_HIS_TO,                 /* to */
    EN_EA_IM_HIS_RECIPIENT_LST,      /* recipient-list */
    EN_EA_IM_HIS_AUTH_ID,            /* auth-id */
    EN_EA_IM_HIS_TIME_START,         /* time-start */
    EN_EA_IM_HIS_TIME_END,           /* time-end */
    EN_EA_IM_HIS_RECORDING_NAME,     /* recording-name */
    EN_EA_IM_HIS_CONF_LST,           /* conf-list */
    EN_EA_IM_HIS_ENTRY,              /* entry */
    EN_EA_IM_HIS_GRP_URI,            /* group-uri */
    EN_EA_IM_HIS_GRP_DISP_NAME,      /* group-display-name */
    EN_EA_IM_HIS_DISP_NAME,          /* display-name */
    EN_EA_IM_HIS_URI                 /* uri */
} EN_EA_IM_HIS_TYPE;

/* im history-list get the history-list */
ZFUNC ZINT EaIm_HisGetHisLst(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstHisLst);

/* im history-list get the history */
ZFUNC ZINT EaIm_HisHisLstGetHis(ST_XML_ELEM *pstHisLst, 
                ST_ZOS_USTR *pstHisRef, ST_XML_ELEM **ppstHis);

/* history-list get the first history */
ZFUNC ZINT EaIm_HisHisLstGetFirstHis(ST_XML_ELEM *pstHisLst,
                ST_XML_ELEM **ppstHis);

/* history-list get the next history */
ZFUNC ZINT EaIm_HisHisLstGetNextHis(ST_XML_ELEM *pstHis,
                ST_XML_ELEM **ppstHis);

/* history get the size */
ZFUNC ZINT EaIm_HisHisGetSize(ST_XML_ELEM *pstHis, ZUINT *piSize);

/* history get the expiry */
ZFUNC ZINT EaIm_HisHisGetExpiry(ST_XML_ELEM *pstHis, 
                ST_XML_ELEM **ppstExpiry);

/* history get the Subject */
ZFUNC ZINT EaIm_HisHisGetSubject(ST_XML_ELEM *pstHis, 
                ST_ZOS_USTR **ppstSubject);

/* history get the pager */
ZFUNC ZINT EaIm_HisHisGetPager(ST_XML_ELEM *pstHis,
                ST_XML_ELEM **ppstPager);

/* history get the conference */
ZFUNC ZINT EaIm_HisHisGetConf(ST_XML_ELEM *pstHis,
                ST_XML_ELEM **ppstConf);

/* history get the date */
ZFUNC ZINT EaIm_HisHisGetDate(ST_XML_ELEM *pstHis,
                ST_ZOS_USTR **ppstDate);

/* history get the history-reference */
ZFUNC ZINT EaIm_HisHisGetHisRef(ST_XML_ELEM *pstHis,
                ST_ZOS_USTR **ppstHisRef);

/* pager get the time-stamp */
ZFUNC ZINT EaIm_HisPagerGetTimeStamp(ST_XML_ELEM *pstPager,
                ST_XML_ELEM **ppstTimeStamp);

/* pager get the from */
ZFUNC ZINT EaIm_HisPagerGetFrom(ST_XML_ELEM *pstPager,
                ST_ZOS_USTR **ppstFrom);

/* pager get the to */
ZFUNC ZINT EaIm_HisPagerGetTo(ST_XML_ELEM *pstPager,
                ST_ZOS_USTR **ppstTo);

/* pager get the recipient-list */
ZFUNC ZINT EaIm_HisPagerGetReciLst(ST_XML_ELEM *pstPager,
                ST_XML_ELEM **ppstReciLst);

/* pager get the auth-id */
ZFUNC ZINT EaIm_HisPagerGetAuthId(ST_XML_ELEM *pstPager,
                ST_ZOS_USTR **ppstAuthId);

/* conference get the time-start */
ZFUNC ZINT EaIm_HisConfGetTimeStart(ST_XML_ELEM *pstConf,
                ST_XML_ELEM **ppstTimeStart);

/* conference get the time-end */
ZFUNC ZINT EaIm_HisConfGetTimeEnd(ST_XML_ELEM *pstConf,
                ST_XML_ELEM **ppstTimeEnd);

/* conference get the recording-name */
ZFUNC ZINT EaIm_HisConfGetRecordName(ST_XML_ELEM *pstConf,
                ST_ZOS_USTR **ppstRecordName);

/* conference get the conf-list */
ZFUNC ZINT EaIm_HisConfGetConfLst(ST_XML_ELEM *pstConf,
                ST_XML_ELEM **ppstConfLst);

/* recipient-list get the entry */
ZFUNC ZINT EaIm_HisReciLstGetEntry(ST_XML_ELEM *pstReciLst,
                ST_ZOS_USTR *pstUri, ST_XML_ELEM **ppstEntry);

/* recipient-list get the first entry */
ZFUNC ZINT EaIm_HisReciLstGetFirstEntry(ST_XML_ELEM *pstReciLst,
                ST_XML_ELEM **ppstEntry);

/* recipient-list get the next entry */
ZFUNC ZINT EaIm_HisReciLstGetNextEntry(ST_XML_ELEM *pstEntry,
                ST_XML_ELEM **ppstEntry);

/* recipient-list get the group-uri */
ZFUNC ZINT EaIm_HisReciLstGetGrpUri(ST_XML_ELEM *pstReciLst,
                ST_ZOS_USTR **ppstGrpUri);

/* recipient-list get the group-display-name */
ZFUNC ZINT EaIm_HisReciLstGetGrpDName(ST_XML_ELEM *pstReciLst,
                ST_ZOS_USTR **ppstGrpDName);

/* conf-list get the entry */
ZFUNC ZINT EaIm_HisConfLstGetEntry(ST_XML_ELEM *pstConfLst,
                ST_ZOS_USTR *pstUri, ST_XML_ELEM **ppstEntry);

/* conf-list get the first entry */
ZFUNC ZINT EaIm_HisConfLstGetFirstEntry(ST_XML_ELEM *pstConfLst,
                ST_XML_ELEM **ppstEntry);

/* conf-list get the next entry */
ZFUNC ZINT EaIm_HisConfLstGetNextEntry(ST_XML_ELEM *pstEntry,
                ST_XML_ELEM **ppstEntry);

/* conf-list get the group-uri */
ZFUNC ZINT EaIm_HisConfLstGetGrpUri(ST_XML_ELEM *pstConfLst,
                ST_ZOS_USTR **ppstGrpUri);

/* conf-list get the group-display-name */
ZFUNC ZINT EaIm_HisConfLstGetGrpDName(ST_XML_ELEM *pstConfLst,
                ST_ZOS_USTR **ppstGrpDName);

/* entry get the display-name */
ZFUNC ZINT EaIm_HisEntryGetDName(ST_XML_ELEM *pstEntry,
                ST_ZOS_USTR **ppstDName);

/* entry get the uri */
ZFUNC ZINT EaIm_HisEntryGetUri(ST_XML_ELEM *pstEntry,
                ST_ZOS_USTR **ppstUri);

/* im history-list set the history-list */
ZFUNC ZINT EaIm_HisSetHisLst(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstHisLst);

/* history-list set the history */
ZFUNC ZINT EaIm_HisHisLstSetHis(ST_XML_ELEM *pstHisLst,
                ST_XML_ELEM **ppstHis);

/* history set the size */
ZFUNC ZINT EaIm_HisHisSetSize(ST_XML_ELEM *pstHis, ZUINT iSize);

/* history set the expiry */
ZFUNC ZINT EaIm_HisHisSetExpiry(ST_XML_ELEM *pstHis, 
                ST_XML_ELEM **ppstExpiry);

/* history set the Subject */
ZFUNC ZINT EaIm_HisHisSetSubject(ST_XML_ELEM *pstHis, 
                ST_ZOS_USTR *pstSubject);

/* history set the pager */
ZFUNC ZINT EaIm_HisHisSetPager(ST_XML_ELEM *pstHis,
                ST_XML_ELEM **ppstPager);

/* history set the conference */
ZFUNC ZINT EaIm_HisHisSetConf(ST_XML_ELEM *pstHis,
                ST_XML_ELEM **ppstConf);

/* history set the date */
ZFUNC ZINT EaIm_HisHisSetDate(ST_XML_ELEM *pstHis,
                ST_ZOS_USTR *pstDate);

/* history set the history-reference */
ZFUNC ZINT EaIm_HisHisSetHisRef(ST_XML_ELEM *pstHis,
                ST_ZOS_USTR *pstHisRef);

/* pager set the time-stamp */
ZFUNC ZINT EaIm_HisPagerSetTimeStamp(ST_XML_ELEM *pstPager,
                ST_XML_ELEM **ppstTimeStamp);

/* pager set the from */
ZFUNC ZINT EaIm_HisPagerSetFrom(ST_XML_ELEM *pstPager,
                ST_ZOS_USTR *pstFrom);

/* pager set the to */
ZFUNC ZINT EaIm_HisPagerSetTo(ST_XML_ELEM *pstPager,
                ST_ZOS_USTR *pstTo);

/* pager set the first recipient-list */
ZFUNC ZINT EaIm_HisPagerSetReciLst(ST_XML_ELEM *pstPager,
                ST_XML_ELEM **ppstReciLst);

/* pager set the auth-id */
ZFUNC ZINT EaIm_HisPagerSetAuthId(ST_XML_ELEM *pstPager,
                ST_ZOS_USTR *pstAuthId);

/* conference set the time-start */
ZFUNC ZINT EaIm_HisConfSetTimeStart(ST_XML_ELEM *pstConf,
                ST_XML_ELEM **ppstTimeStart);

/* conference set the time-end */
ZFUNC ZINT EaIm_HisConfSetTimeEnd(ST_XML_ELEM *pstConf,
                ST_XML_ELEM **ppstTimeEnd);

/* conference set the recording-name */
ZFUNC ZINT EaIm_HisConfSetRecordName(ST_XML_ELEM *pstConf,
                ST_ZOS_USTR *pstRecordName);

/* conference set the conf-list */
ZFUNC ZINT EaIm_HisConfSetConfLst(ST_XML_ELEM *pstConf,
                ST_XML_ELEM **ppstConfLst);

/* recipient-list set the entry */
ZFUNC ZINT EaIm_HisReciLstSetEntry(ST_XML_ELEM *pstReciLst,
                ST_XML_ELEM **ppstEntry);

/* recipient-list set the group-uri */
ZFUNC ZINT EaIm_HisReciLstSetGrpUri(ST_XML_ELEM *pstReciLst,
                ST_ZOS_USTR *pstGrpUri);

/* recipient-list set the group-display-name */
ZFUNC ZINT EaIm_HisReciLstSetGrpDName(ST_XML_ELEM *pstReciLst,
                ST_ZOS_USTR *pstGrpDName);

/* conf-list set the entry */
ZFUNC ZINT EaIm_HisConfLstSetEntry(ST_XML_ELEM *pstConfLst,
                ST_XML_ELEM **ppstEntry);

/* conf-list set the group-uri */
ZFUNC ZINT EaIm_HisConfLstSetGrpUri(ST_XML_ELEM *pstConfLst,
                ST_ZOS_USTR *pstGrpUri);

/* conf-list set the group-display-name */
ZFUNC ZINT EaIm_HisConfLstSetGrpDName(ST_XML_ELEM *pstConfLst,
                ST_ZOS_USTR *pstGrpDName);

/* entry set the display-name */
ZFUNC ZINT EaIm_HisEntrySetDName(ST_XML_ELEM *pstEntry,
                ST_ZOS_USTR *pstDName);

/* entry set the uri */
ZFUNC ZINT EaIm_HisEntrySetUri(ST_XML_ELEM *pstEntry,
                ST_ZOS_USTR *pstUri);

#ifdef __cplusplus
}
#endif

#endif /* _EA_IM_HIS_H__ */

