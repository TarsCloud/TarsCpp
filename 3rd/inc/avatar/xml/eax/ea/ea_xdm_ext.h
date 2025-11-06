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
  File name     : ea_xdm_ext.h
  Module        : eax xdm extensions 
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2011-08-19   
  Description   :
   Function implement required by eax xdm extensions

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XDM_EXT_H__
#define _EA_XDM_EXT_H__

#ifdef __cplusplus
extern "C"{
#endif

/********** urn:oma:xml:xdm:extensions *********/
typedef enum EN_EA_OMA_XDM_EXT_TYPE
{
	EN_EA_OMA_XDM_EXT_AUDIO,         /* audio */
	EN_EA_OMA_XDM_EXT_FILE_TRANSFER, /* file-transfer */
	EN_EA_OMA_XDM_EXT_GRP_ADVT,      /* group-advertisement */
	EN_EA_OMA_XDM_EXT_MSG_SESS,      /* message-session */
	EN_EA_OMA_XDM_EXT_PAGER_MODE_MSG, /* pager-mode-message */
	EN_EA_OMA_XDM_EXT_SRV,           /* service */
	EN_EA_OMA_XDM_EXT_VIDEO,         /* video */
	EN_EA_OMA_XDM_EXT_POC_SPEECH,    /* poc-speech */
	EN_EA_OMA_XDM_EXT_ALLOW,         /* allow */
	EN_EA_OMA_XDM_EXT_MUST_UNDERSTAND, /* must-understand */
	EN_EA_OMA_XDM_EXT_AGE_RESTRICTIONS, /* age-restrictions */
	EN_EA_OMA_XDM_EXT_AUTOMATIC_GRP_ADVT, /* automatic-group-advertisement */
	EN_EA_OMA_XDM_EXT_QOE,           /* qoe */
	EN_EA_OMA_XDM_EXT_SEARCHABLE,    /* searchable */
	EN_EA_OMA_XDM_EXT_SESS_ACTIVE_POLICY, /* session-active-policy */
	EN_EA_OMA_XDM_EXT_SUBJECT,       /* subject */
	EN_EA_OMA_XDM_EXT_SUPPORTED_SERVICES, /* supported-services */
	EN_EA_OMA_XDM_EXT_MEDIA_LST,     /* media-list */
	EN_EA_OMA_XDM_EXT_SRV_LST,       /* service-list */
	EN_EA_OMA_XDM_EXT_ALLOW_MEDIA_HANDLING, /* allow-media-handling */
	EN_EA_OMA_XDM_EXT_ALLOW_AUTO_ANSWERMODE, /* allow-auto-answermode */
	EN_EA_OMA_XDM_EXT_ALLOW_EXPELLING, /* allow-expelling */
	EN_EA_OMA_XDM_EXT_BLK_GRP_ADVT_SENDING, /* block-group-advertisement-sending */
	EN_EA_OMA_XDM_EXT_ALLOW_MANUAL_ANSWER_OVERRIDE, /* allow-manual-answer-override */
	EN_EA_OMA_XDM_EXT_ALLOW_OFFLINE_STORAGE, /* allow-offline-storage */
	EN_EA_OMA_XDM_EXT_ALLOW_PRIVATE_MSG, /* allow-private-message */
	EN_EA_OMA_XDM_EXT_ALLOW_REJ_IVT, /* allow-reject-invite */
	EN_EA_OMA_XDM_EXT_ALLOW_SUBCONF, /* allow-subconf */
	EN_EA_OMA_XDM_EXT_REMOVE_MEDIA_HANDLING, /* remove-media-handling */
	EN_EA_OMA_XDM_EXT_COMMON_USAGE,  /* common-usage */
	EN_EA_OMA_XDM_EXT_AGE,           /* age */
	EN_EA_OMA_XDM_EXT_FROM,          /* from */
	EN_EA_OMA_XDM_EXT_UNTIL,         /* until */
	EN_EA_OMA_XDM_EXT_ID,            /* id */
	EN_EA_OMA_XDM_EXT_FULL_DUPLEX,   /* full-duplex */
	EN_EA_OMA_XDM_EXT_HALF_DUPLEX,   /* half-duplex */
	EN_EA_OMA_XDM_EXT_ALL_MEDIA_EXCEPT, /* all-media-except */
	EN_EA_OMA_XDM_EXT_BASIC,         /* basic */
	EN_EA_OMA_XDM_EXT_PREMIUM,       /* premium */
	EN_EA_OMA_XDM_EXT_PROFESSIONAL,  /* professional */
	EN_EA_OMA_XDM_EXT_GOVERNMENT,    /* government */
	EN_EA_OMA_XDM_EXT_START_TIME,    /* start-time */
	EN_EA_OMA_XDM_EXT_END_TIME,      /* end-time */
	EN_EA_OMA_XDM_EXT_ENTRY,         /* entry */
	EN_EA_OMA_XDM_EXT_EXTERNAL,      /* external */
	EN_EA_OMA_XDM_EXT_SESS_ORIGINATOR, /* session-originator */
	EN_EA_OMA_XDM_EXT_TIME_RANGE,    /* time-range */
	EN_EA_OMA_XDM_EXT_ALL_SERVICES_EXCEPT, /* all-services-except */
	EN_EA_OMA_XDM_EXT_GRP_MEDIA,     /* group-media */
	EN_EA_OMA_XDM_EXT_ENABLER,       /* enabler */
	EN_EA_OMA_XDM_EXT_MAX_DUR,       /* max-duration */
	EN_EA_OMA_XDM_EXT_REQUIRED_PARTICIPANTS_LST, /* required-participants-list */
	EN_EA_OMA_XDM_EXT_MIN_PARTP_COUNT, /* min-participant-count */
	EN_EA_OMA_XDM_EXT_SCHEDULE,      /* schedule */
	EN_EA_OMA_XDM_EXT_FTAG           /* feature tag */ 
} EN_EA_OMA_XDM_EXT_TYPE;


/* eax xdm extensions get token id from string */
ZFUNC ZINT EaXdm_ExtTknStr2Id(ST_ZOS_USTR *pstStr, ZTKNID *pzTknId);

/* eax xdm extensions get token string from id */
ZFUNC ZINT EaXdm_ExtTknId2Str(ZTKNID zTknId, ST_ZOS_USTR *pstStr);

/* eax xdm extensions get the first element iExtType: EN_EA_OMA_XDM_EXT_TYPE */
ZFUNC ZINT EaXdm_ExtGetFirstElem(ST_XML_ELEM *pstElem, ZUINT iExtType,
                ST_XML_ELEM **ppstChildElem);

/* eax xdm extensions get the next element iExtType: EN_EA_OMA_XDM_EXT_TYPE */
ZFUNC ZINT EaXdm_ExtGetNextElem(ST_XML_ELEM *pstElem, ZUINT iExtType,
                ST_XML_ELEM **ppstNextElem);

/* xdm extensions get the audio */
ZFUNC ZINT EaXdm_ExtGetAudio(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstAudio);

/* xdm extensions set the audio */
ZFUNC ZINT EaXdm_ExtSetAudio(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstAudio);

/* xdm extensions get the file-transfer */
ZFUNC ZINT EaXdm_ExtGetFileTrsf(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstFileTrsf);

/* xdm extensions set the file-transfer */
ZFUNC ZINT EaXdm_ExtSetFileTrsf(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstFileTrsf);

/* xdm extensions get the group-advertisement */
ZFUNC ZINT EaXdm_ExtGetGrpAd(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstGrpAd);

/* xdm extensions set the group-advertisement */
ZFUNC ZINT EaXdm_ExtSetGrpAd(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstGrpAd);

/* xdm extensions get the message-session */
ZFUNC ZINT EaXdm_ExtGetMsgSess(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstMsgSess);

/* xdm extensions set the message-session */
ZFUNC ZINT EaXdm_ExtSetMsgSess(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstMsgSess);

/* xdm extensions get the pager-mode-message */
ZFUNC ZINT EaXdm_ExtGetPageModeMsg(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPageModeMsg);

/* xdm extensions set the pager-mode-message */
ZFUNC ZINT EaXdm_ExtSetPageModeMsg(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPageModeMsg);

/* xdm extensions get the service */
ZFUNC ZINT EaXdm_ExtGetSrv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSrv);

/* xdm extensions set the service */
ZFUNC ZINT EaXdm_ExtSetSrv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSrv);

/* xdm extensions get service enabler */
ZFUNC ZINT EaXdm_ExtSrvGetEnabler(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR **ppstEnabler);

/* xdm extensions set service enabler */
ZFUNC ZINT EaXdm_ExtSrvSetEnabler(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR *pstEnabler);

/* xdm extensions get service group-media */
ZFUNC ZINT EaXdm_ExtSrvGetGrpMedia(ST_XML_ELEM *pstSrv, 
                ST_XML_ELEM **ppstGrpMedia);

/* xdm extensions set service group-media */
ZFUNC ZINT EaXdm_ExtSrvSetGrpMedia(ST_XML_ELEM *pstSrv, 
                ST_XML_ELEM **ppstGrpMedia);

/* xdm extensions get all-media-except */
ZFUNC ZINT EaXdm_ExtGetAllMediaExcept(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set all-media-except */
ZFUNC ZINT EaXdm_ExtSetAllMediaExcept(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the service feature tag */
ZFUNC ZINT EaXdm_ExtSrvGetFTag(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR **ppstFTag);

/* xdm extensions set the service feature tag */
ZFUNC ZINT EaXdm_ExtSrvSetFTag(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR *pstFTag);

/* xdm extensions get the video */
ZFUNC ZINT EaXdm_ExtGetVideo(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstVideo);

/* xdm extensions set the video */
ZFUNC ZINT EaXdm_ExtSetVideo(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstVideo);

/* xdm extensions get full duplex type */
ZFUNC ZINT EaXdm_ExtGetFullDuplex(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstFull);

/* xdm extensions get half duplex type */
ZFUNC ZINT EaXdm_ExtGetHalfDuplex(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstHalf);

/* xdm extensions set full duplex type */
ZFUNC ZINT EaXdm_ExtSetFullDuplex(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstFull);

/* xdm extensions set half duplex type */
ZFUNC ZINT EaXdm_ExtSetHalfDuplex(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstHalf);

/* xdm extensions get the poc-speech */
ZFUNC ZINT EaXdm_ExtGetPocSpeech(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPocSpeech);

/* xdm extensions set the poc-speech */
ZFUNC ZINT EaXdm_ExtSetPocSpeech(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPocSpeech);

/* xdm extensions get the age-restrictions */
ZFUNC ZINT EaXdm_ExtGetAgeRestriction(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the age-restrictions */
ZFUNC ZINT EaXdm_ExtSetAgeRestriction(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the age */
ZFUNC ZINT EaXdm_ExtGetAge(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the age */
ZFUNC ZINT EaXdm_ExtSetAge(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the from attribute */
ZFUNC ZINT EaXdm_ExtGetFrom(ST_XML_ELEM *pstElem, ZUINT *piRetElem);

/* xdm extensions set the from attribute */
ZFUNC ZINT EaXdm_ExtSetFrom(ST_XML_ELEM *pstElem, ZUINT iRetElem);

/* xdm extensions get the until attribute */
ZFUNC ZINT EaXdm_ExtGetUntil(ST_XML_ELEM *pstElem, ZUINT *piRetElem);

/* xdm extensions set the until attribute */
ZFUNC ZINT EaXdm_ExtSetUntil(ST_XML_ELEM *pstElem, ZUINT iRetElem);

/* xdm extensions get automatic-group-advertisement */
ZFUNC ZINT EaXdm_ExtGetAutoGrpAd(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set automatic-group-advertisement */
ZFUNC ZINT EaXdm_ExtSetAutoGrpAd(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get the qoe */
ZFUNC ZINT EaXdm_ExtGetQoe(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the qoe */
ZFUNC ZINT EaXdm_ExtSetQoe(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the qoe basic */
ZFUNC ZINT EaXdm_ExtQoeGetBasic(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the qoe basic */
ZFUNC ZINT EaXdm_ExtQoeSetBasic(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the qoe premium */
ZFUNC ZINT EaXdm_ExtQoeGetPremium(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the qoe premium */
ZFUNC ZINT EaXdm_ExtQoeSetPremium(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the qoe professional */
ZFUNC ZINT EaXdm_ExtQoeGetProfess(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the qoe professional */
ZFUNC ZINT EaXdm_ExtQoeSetProfess(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the qoe government */
ZFUNC ZINT EaXdm_ExtQoeGetGovn(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the qoe government */
ZFUNC ZINT EaXdm_ExtQoeSetGovn(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get searchable */
ZFUNC ZINT EaXdm_ExtGetSearchable(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set searchable */
ZFUNC ZINT EaXdm_ExtSetSearchable(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get the session-active-policy */
ZFUNC ZINT EaXdm_ExtGetSessActivePolicy(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSessActivePolicy);

/* xdm extensions set the session-active-policy */
ZFUNC ZINT EaXdm_ExtSetSessActivePolicy(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSessActivePolicy);

/* xdm extensions get the max-duration */
ZFUNC ZINT EaXdm_ExtGetMaxDur(ST_XML_ELEM *pstElem, ZUINT *piRetElem);

/* xdm extensions set the max-duration */
ZFUNC ZINT EaXdm_ExtSetMaxDur(ST_XML_ELEM *pstElem, ZUINT iMaxDur);

/* xdm extensions get the required-participants-list */
ZFUNC ZINT EaXdm_ExtGetReqPartpLst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the required-participants-list */
ZFUNC ZINT EaXdm_ExtSetReqPartpLst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the session-originator */
ZFUNC ZINT EaXdm_ExtGetSessOrg(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the session-originator */
ZFUNC ZINT EaXdm_ExtSetSessOrg(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the min-participant-count */
ZFUNC ZINT EaXdm_ExtGetMinPartpCount(ST_XML_ELEM *pstElem, 
                ZUINT *piMinPartpCount);

/* xdm extensions set the min-participant-count */
ZFUNC ZINT EaXdm_ExtSetMinPartpCount(ST_XML_ELEM *pstElem, 
                ZUINT iMinPartpCount);

/* xdm extensions get the schedule */
ZFUNC ZINT EaXdm_ExtGetSchedule(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the schedule */
ZFUNC ZINT EaXdm_ExtSetSchedule(ST_XML_ELEM *pstElem, ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the time-range */
ZFUNC ZINT EaXdm_ExtGetTimeRange(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the time-range */
ZFUNC ZINT EaXdm_ExtSetTimeRange(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the start-time */
ZFUNC ZINT EaXdm_ExtGetStartTime(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstStartTime);

/* xdm extensions set the start-time */
ZFUNC ZINT EaXdm_ExtSetStartTime(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstStartTime);

/* xdm extensions get the end-time */
ZFUNC ZINT EaXdm_ExtGetEndTime(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstEndTime);

/* xdm extensions set the end-time */
ZFUNC ZINT EaXdm_ExtSetEndTime(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstEndTime);

/* xdm extensions get must-understand attribute*/
ZFUNC ZINT EaXdm_ExtGetMu(ST_XML_ELEM *pstElem, ZBOOL *pbMu);

/* xdm extensions set must-understand attribute*/
ZFUNC ZINT EaXdm_ExtSetMu(ST_XML_ELEM *pstElem, ZBOOL bMu);

/* xdm extensions get allow attribute*/
ZFUNC ZINT EaXdm_ExtGetAllow(ST_XML_ELEM *pstElem, ZBOOL *pbAllow);

/* xdm extensions set allow attribute*/
ZFUNC ZINT EaXdm_ExtSetAllow(ST_XML_ELEM *pstElem, ZBOOL bAllow);

/* xdm extensions get id attribute*/
ZFUNC ZINT EaXdm_ExtGetId(ST_XML_ELEM *pstElem, ST_ZOS_USTR **pstId);

/* xdm extensions set id attribute*/
ZFUNC ZINT EaXdm_ExtSetId(ST_XML_ELEM *pstElem, ST_ZOS_USTR *stId);

/* xdm extensions get the subject */
ZFUNC ZINT EaXdm_ExtGetSubject(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstSubject);

/* xdm extensions set the subject */
ZFUNC ZINT EaXdm_ExtSetSubject(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstSubject);

/* xdm extensions get the supported-services */
ZFUNC ZINT EaXdm_ExtGetSptServ(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSptServ);

/* xdm extensions set the supported-services */
ZFUNC ZINT EaXdm_ExtSetSptServ(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSptServ);

/* xdm extensions get the all-services-except */
ZFUNC ZINT EaXdm_ExtGetAllSrvExcept(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the all-services-except */
ZFUNC ZINT EaXdm_ExtSetAllSrvExcept(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the media-list */
ZFUNC ZINT EaXdm_ExtGetMediaLst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the media-list */
ZFUNC ZINT EaXdm_ExtSetMediaLst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get the service-list */
ZFUNC ZINT EaXdm_ExtGetSrvLst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the service-list */
ZFUNC ZINT EaXdm_ExtSetSrvLst(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get allow-media-handling */
ZFUNC ZINT EaXdm_ExtGetAllowMediaHand(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set allow-media-handling */
ZFUNC ZINT EaXdm_ExtSetAllowMediaHand(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get allow-auto-answermode */
ZFUNC ZINT EaXdm_ExtGetAllowAutoAnsw(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set allow-auto-answermode */
ZFUNC ZINT EaXdm_ExtSetAllowAutoAnsw(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get allow-expelling */
ZFUNC ZINT EaXdm_ExtGetAllowExpell(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set allow-expelling */
ZFUNC ZINT EaXdm_ExtSetAllowExpell(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get block-group-advertisement-sending */
ZFUNC ZINT EaXdm_ExtGetBlkGrpAdSend(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set block-group-advertisement-sending */
ZFUNC ZINT EaXdm_ExtSetBlkGrpAdSend(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get allow-manual-answer-override */
ZFUNC ZINT EaXdm_ExtGetAllowManAnsw(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set allow-manual-answer-override */
ZFUNC ZINT EaXdm_ExtSetAllowManAnsw(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get the allow-offline-storage */
ZFUNC ZINT EaXdm_ExtGetAllowOffStorge(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions set the allow-offline-storage */
ZFUNC ZINT EaXdm_ExtSetAllowOffStorge(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstRetElem);

/* xdm extensions get allow-private-message */
ZFUNC ZINT EaXdm_ExtGetAllowPrvMsg(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set allow-private-message */
ZFUNC ZINT EaXdm_ExtSetAllowPrvMsg(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get allow-reject-invite */
ZFUNC ZINT EaXdm_ExtGetAllowRejInv(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set allow-reject-invite */
ZFUNC ZINT EaXdm_ExtSetAllowRejInv(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get allow-subconf */
ZFUNC ZINT EaXdm_ExtGetAllowSubconf(ST_XML_ELEM *pstElem, ZBOOL *pbVal);

/* xdm extensions set allow-subconf */
ZFUNC ZINT EaXdm_ExtSetAllowSubconf(ST_XML_ELEM *pstElem, ZBOOL bVal);

/* xdm extensions get the remove-media-handling */
ZFUNC ZINT EaXdm_ExtGetRmvMediaHand(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstRmvMediaHand);

/* xdm extensions set the remove-media-handling */
ZFUNC ZINT EaXdm_ExtSetRmvMediaHand(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstRmvMediaHand);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XDM_EXT_H__ */
