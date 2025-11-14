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
  File name     : ea_conf_info.h
  Module        : eax conference-info
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax conference-info.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_CONF_INFO_H__
#define _EA_CONF_INFO_H__

#ifdef __cplusplus
extern "C"{
#endif

/* conference-info state type */
typedef enum EN_EA_CONF_INFO_STA_TYPE
{
    EN_EA_CONF_INFO_STA_FULL,        
    EN_EA_CONF_INFO_STA_PARTIAL,
    EN_EA_CONF_INFO_STA_DEL
} EN_EA_CONF_INFO_STA_TYPE;

/* conference-info endpoint-status type */
typedef enum EN_EA_CONF_INFO_ENDP_STAT_TYPE
{
    EN_EA_CONF_INFO_ENDP_STAT_PENDING,        
    EN_EA_CONF_INFO_ENDP_STAT_DIALING_OUT,
    EN_EA_CONF_INFO_ENDP_STAT_DIALING_IN,
    EN_EA_CONF_INFO_ENDP_STAT_ALERTING,
    EN_EA_CONF_INFO_ENDP_STAT_ON_HOLD,
    EN_EA_CONF_INFO_ENDP_STAT_CONNED,
    EN_EA_CONF_INFO_ENDP_STAT_MUTED_VIA_FOCUS,
    EN_EA_CONF_INFO_ENDP_STAT_DISCING,
    EN_EA_CONF_INFO_ENDP_STAT_DISCED
} EN_EA_CONF_INFO_ENDP_STAT_TYPE;

/* conference-info joining type */
typedef enum EN_EA_CONF_INFO_JOINING_TYPE
{
    EN_EA_CONF_INFO_JOINING_DIALED_IN,        
    EN_EA_CONF_INFO_JOINING_DIALED_OUT,
    EN_EA_CONF_INFO_JOINING_FOCUS_OWNER
} EN_EA_CONF_INFO_JOINING_TYPE;

/* conference-info disconnection type */
typedef enum EN_EA_CONF_INFO_DISC_TYPE
{
    EN_EA_CONF_INFO_DISC_DEPARTED,        
    EN_EA_CONF_INFO_DISC_BOOTED,
    EN_EA_CONF_INFO_DISC_FAILED,
    EN_EA_CONF_INFO_DISC_BUSY
} EN_EA_CONF_INFO_DISC_TYPE;

/* conference-info media-status type */
typedef enum EN_EA_CONF_INFO_MEDIA_STAT_TYPE
{
    EN_EA_CONF_INFO_MEDIA_STAT_REVONLY,        
    EN_EA_CONF_INFO_MEDIA_STAT_SENDONLY,
    EN_EA_CONF_INFO_MEDIA_STAT_SENDRECV,
    EN_EA_CONF_INFO_MEDIA_STAT_INACTIVE
} EN_EA_CONF_INFO_MEDIA_STAT_TYPE;

/* conference-info user roles type */
typedef enum EN_EA_CONF_INFO_USR_ROLES_TYPE
{
    EN_EA_CONF_INFO_USR_ROLES_CHAIRMAN,        
    EN_EA_CONF_INFO_USR_ROLES_PARTP
} EN_EA_CONF_INFO_USR_ROLES_TYPE;

/* conference-info authorization type */
typedef enum EN_EA_CONF_INFO_AUTH_TYPE
{
    EN_EA_CONF_INFO_AUTH_OTA,        
    EN_EA_CONF_INFO_AUTH_UA,
    EN_EA_CONF_INFO_AUTH_GBA,
} EN_EA_CONF_INFO_AUTH_TYPE;

/************* urn:ietf:params:xml:ns:conference-info *********/
typedef enum EN_EA_CONF_INFO_TYPE
{
    EN_EA_CONF_INFO_CONF_INFO,    /* conference-info */
    EN_EA_CONF_INFO_CONF_DESC,    /* conference-description */
    EN_EA_CONF_INFO_HOST_INFO,    /* host-info */
    EN_EA_CONF_INFO_CONF_STA,     /* conference-state */
    EN_EA_CONF_INFO_USRS,         /* users */
    EN_EA_CONF_INFO_SIDEBARS_BY_REF, /* sidebars-by-ref */
    EN_EA_CONF_INFO_SIDEBARS_BY_VAL, /* sidebars-by-val */
    EN_EA_CONF_INFO_ENT,          /* entity */
    EN_EA_CONF_INFO_STA,          /* state */
    EN_EA_CONF_INFO_YOUROWN,      /* yourown */
    EN_EA_CONF_INFO_VER,          /* version */
    EN_EA_CONF_INFO_FULL,         /* full */
    EN_EA_CONF_INFO_PARTIAL,      /* partial */
    EN_EA_CONF_INFO_DELD,         /* deleted */
    EN_EA_CONF_INFO_DISP_TEXT,    /* display-text */
    EN_EA_CONF_INFO_SUBJECT,      /* subject */
    EN_EA_CONF_INFO_FREE_TEXT,    /* free-text */
    EN_EA_CONF_INFO_KEYWORDS,     /* keywords */
    EN_EA_CONF_INFO_CONF_URIS,    /* conf-uris */
    EN_EA_CONF_INFO_SRV_URIS,     /* service-uris */
    EN_EA_CONF_INFO_MAXIMUM_USR_COUNT, /* maximum-user-count */
    EN_EA_CONF_INFO_GROUP_TYPE,   /* grouptype */
    EN_EA_CONF_INFO_AVAILABLE_MEDIA, /* available-media */
    EN_EA_CONF_INFO_WEB_PAGE,     /* web-page */
    EN_EA_CONF_INFO_URIS,         /* uris */
    EN_EA_CONF_INFO_USR_COUNT,    /* user-count */
    EN_EA_CONF_INFO_ACTIVE,       /* active */
    EN_EA_CONF_INFO_LOCKED,       /* locked */
    EN_EA_CONF_INFO_ENTRY,        /* entry */
    EN_EA_CONF_INFO_T,            /* type */
    EN_EA_CONF_INFO_STAT,         /* status */
    EN_EA_CONF_INFO_LABEL,        /* label */
    EN_EA_CONF_INFO_URI,          /* uri */
    EN_EA_CONF_INFO_PURPOSE,      /* purpose */
    EN_EA_CONF_INFO_MDFY,         /* modified */
    EN_EA_CONF_INFO_USR,          /* user */
    EN_EA_CONF_INFO_ASSOCIATED_AORS, /* associated-aors */
    EN_EA_CONF_INFO_ROLES,        /* roles */
    EN_EA_CONF_INFO_LANGS,        /* languages */
    EN_EA_CONF_INFO_CASCADED_FOCUS, /* cascaded-focus */
    EN_EA_CONF_INFO_ENDPOINT,     /* endpoint */
    EN_EA_CONF_INFO_REFERRED,     /* referred */
    EN_EA_CONF_INFO_JOINING_METHOD, /* joining-method */
    EN_EA_CONF_INFO_JOINING_INFO, /* joining-info */
    EN_EA_CONF_INFO_DISCONNECTION_METHOD, /* disconnection-method */
    EN_EA_CONF_INFO_DISCONNECTION_INFO, /* disconnection-info */
    EN_EA_CONF_INFO_MEDIA,        /* media */
    EN_EA_CONF_INFO_CALL_INFO,    /* call-info */
    EN_EA_CONF_INFO_PENDING,      /* pending */
    EN_EA_CONF_INFO_DIALING_OUT,  /* dialing-out */
    EN_EA_CONF_INFO_DIALING_IN,   /* dialing-in */
    EN_EA_CONF_INFO_ALERTING,     /* alerting */
    EN_EA_CONF_INFO_ON_HOLD,      /* on-hold */
    EN_EA_CONF_INFO_CONNECTED,    /* connected */
    EN_EA_CONF_INFO_MUTED_VIA_FOCUS, /* muted-via-focus */
    EN_EA_CONF_INFO_DISCONNECTING, /* disconnecting */
    EN_EA_CONF_INFO_DISCONNECTED, /* disconnected */
    EN_EA_CONF_INFO_DIAL_IN,      /* dialed-in */
    EN_EA_CONF_INFO_DIAL_OUT,     /* dialed-out */
    EN_EA_CONF_INFO_FOCUS_OWNER,  /* focus-owner */
    EN_EA_CONF_INFO_DEPARTED,     /* departed */
    EN_EA_CONF_INFO_BOOTED,       /* booted */
    EN_EA_CONF_INFO_FAILED,       /* failed */
    EN_EA_CONF_INFO_BUSY,         /* busy */
    EN_EA_CONF_INFO_WHEN,         /* when */
    EN_EA_CONF_INFO_REASON,       /* reason */
    EN_EA_CONF_INFO_BY,           /* by */
    EN_EA_CONF_INFO_SIP,          /* sip */
    EN_EA_CONF_INFO_CALL_ID,      /* call-id */
    EN_EA_CONF_INFO_FROM_TAG,     /* from-tag */
    EN_EA_CONF_INFO_TO_TAG,       /* to-tag */
    EN_EA_CONF_INFO_SRC_ID,       /* src-id */
    EN_EA_CONF_INFO_ID,           /* id */
    EN_EA_CONF_INFO_RECVONLY,     /* recvonly */
    EN_EA_CONF_INFO_SENDONLY,     /* sendonly */
    EN_EA_CONF_INFO_SENDRECV,     /* sendrecv */
    EN_EA_CONF_INFO_INACTIVE,     /* inactive */    
    EN_EA_CONF_INFO_CHAIRMAN,     /* chairman */
    EN_EA_CONF_INFO_PARTP,        /* participant */
    EN_EA_CONF_INFO_AUTHTYPE,     /* authorization-type */
    EN_EA_CONF_INFO_OTA,          /* OTA */
    EN_EA_CONF_INFO_UA,           /* UA */
    EN_EA_CONF_INFO_GBA,          /* GBA */
    EN_EA_CONF_INFO_TOKENVALUE,   /* token-value */
    EN_EA_CONF_INFO_ENTERPRISE_ID, /* enterprise-id */
    EN_EA_CONF_INFO_DEPARTMENT_ID, /* department-id */
} EN_EA_CONF_INFO_TYPE;

/* conference-info get the conference-info */
ZFUNC ZINT EaConf_InfoGetInfo(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstInfo);

/* conference-info get the conference-info conference-description */
ZFUNC ZINT EaConf_InfoInfoGetConfDesc(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstConfDesc);

/* conference-info get the conference-info host-info */
ZFUNC ZINT EaConf_InfoInfoGetHostInfo(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstHostInfo);

/* conference-info get the conference-info conference-state */
ZFUNC ZINT EaConf_InfoInfoGetConfSta(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstConfSta);

/* conference-info get the conference-info users */
ZFUNC ZINT EaConf_InfoInfoGetUsrs(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstUsrs);

/* conference-info get the conference-info sidebars-by-ref */
ZFUNC ZINT EaConf_InfoInfoGetSbByRef(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstSbByRef);

/* conference-info get the conference-info sidebars-by-val */
ZFUNC ZINT EaConf_InfoInfoGetSbByVal(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstSbByVal);

/* conference-info get the conference-info entity */
ZFUNC ZINT EaConf_InfoInfoGetEnt(ST_XML_ELEM *pstInfo, 
                ST_ZOS_USTR **ppstEnt);

/* conference-info get the conference-info state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoInfoGetSta(ST_XML_ELEM *pstInfo, 
                ZUCHAR *pucSta);

/* conference-info get the conference-info version */
ZFUNC ZINT EaConf_InfoInfoGetVer(ST_XML_ELEM *pstInfo, 
                ZUINT *piVer);

/* conference-info get the conference-info authorization-type */
ZFUNC ZINT EaConf_InfoInfoGetAuthType(ST_XML_ELEM *pstEndp, 
                ZUINT *piType);

/* conference-info get the conference-info token-value */
ZFUNC ZINT EaConf_InfoInfoGetTokenValue(ST_XML_ELEM *pstInfo, 
                ST_ZOS_USTR **ppstTokenValue);

/* conference-info get the conference-description display-text */
ZFUNC ZINT EaConf_InfoConfDescGetDispTxt(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the conference-description subject */
ZFUNC ZINT EaConf_InfoConfDescGetSubject(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR **ppstSubject);

/* conference-info get the conference-description free-text */
ZFUNC ZINT EaConf_InfoConfDescGetFreeTxt(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR **ppstFreeTxt);

/* conference-info get the conference-description keywords */
ZFUNC ZINT EaConf_InfoConfDescGetKey(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR **ppstKey);

/* conference-info get the conference-description conf-uris */
ZFUNC ZINT EaConf_InfoConfDescGetConfUris(ST_XML_ELEM *pstConfDesc, 
                ST_XML_ELEM **ppstUris);

/* conference-info get the conference-description service-uris */
ZFUNC ZINT EaConf_InfoConfDescGetSrvUris(ST_XML_ELEM *pstConfDesc, 
                ST_XML_ELEM **ppstSrvUris);

/* conference-info get the conference-description maximum-user-count */
ZFUNC ZINT EaConf_InfoConfDescGetMaxUsrCount(ST_XML_ELEM *pstConfDesc, 
                ZUINT *piMaxUsrCount);

/* conference-info get the conference-description group-type */
ZFUNC ZINT EaConf_InfoConfDescGetGroupType(ST_XML_ELEM *pstConfDesc, 
                ZUINT *piGroupType);

/* conference-info get the conference-description enterprise-id */
ZFUNC ZINT EaConf_InfoConfDescGetEnterpriseId(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR **ppstEnterpriseId);

/* conference-info get the conference-description department-id */
ZFUNC ZINT EaConf_InfoConfDescGetDepartmentId(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR **ppstDepartmentId);

/* conference-info get the conference-description available-media */
ZFUNC ZINT EaConf_InfoConfDescGetAvailMedia(ST_XML_ELEM *pstConfDesc, 
                ST_XML_ELEM **ppstAvailMedia);

/* conference-info get the host-type display-text */
ZFUNC ZINT EaConf_InfoHostDescGetDispTxt(ST_XML_ELEM *pstHostInfo, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the host-type web-page */
ZFUNC ZINT EaConf_InfoHostDescGetWebPage(ST_XML_ELEM *pstHostInfo, 
                ST_ZOS_USTR **ppstWebPage);

/* conference-info get the host-type uris */
ZFUNC ZINT EaConf_InfoHostDescGetUris(ST_XML_ELEM *pstHostInfo, 
                ST_XML_ELEM **ppstUris);

/* conference-info get the conference-state user-count */
ZFUNC ZINT EaConf_InfoConfStaGetUsrCount(ST_XML_ELEM *pstConfSta, 
                ZUINT *piMaxUsrCount);

/* conference-info get the conference-state active */
ZFUNC ZINT EaConf_InfoConfStaGetActive(ST_XML_ELEM *pstConfSta, 
                ZBOOL *pbActive);

/* conference-info get the conference-state locked */
ZFUNC ZINT EaConf_InfoConfStaGetLock(ST_XML_ELEM *pstConfSta, 
                ZBOOL *pbLock);

/* conference-info get the first conference-media entry */
ZFUNC ZINT EaConf_InfoConfMediaGetFirstEntry(ST_XML_ELEM *pstConfMedia, 
                ST_XML_ELEM **ppstEntry);

/* conference-info get the next conference-media entry */
ZFUNC ZINT EaConf_InfoConfMediaGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* conference-info get the conference-medium display-text */
ZFUNC ZINT EaConf_InfoConfMediumGetDispTxt(ST_XML_ELEM *pstConfMedium, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the conference-medium type */
ZFUNC ZINT EaConf_InfoConfMediumGetType(ST_XML_ELEM *pstConfMedium, 
                ST_ZOS_USTR **ppstType);

/* conference-info get the conference-medium status */
ZFUNC ZINT EaConf_InfoConfMediumGetStat(ST_XML_ELEM *pstConfMedium, 
                ST_XML_ELEM **ppstStat);

/* conference-info get the conference-medium label */
ZFUNC ZINT EaConf_InfoConfMediumGetLabel(ST_XML_ELEM *pstConfMedium, 
                ST_ZOS_USTR **ppstLabel);

/* conference-info get the first uris entry */
ZFUNC ZINT EaConf_InfoUrisGetFirstEntry(ST_XML_ELEM *pstUris, 
                ST_XML_ELEM **ppstEntry);

/* conference-info get the next uris entry */
ZFUNC ZINT EaConf_InfoUrisGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* conference-info get the uris state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoUrisGetSta(ST_XML_ELEM *pstUris, 
                ZUCHAR *pucSta);

/* conference-info get the uri uri */
ZFUNC ZINT EaConf_InfoUriGetUri(ST_XML_ELEM *pstUri, 
                ST_ZOS_USTR **ppstUri);

/* conference-info get the uri display-text */
ZFUNC ZINT EaConf_InfoUriGetDispTxt(ST_XML_ELEM *pstUri, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the uri purpose */
ZFUNC ZINT EaConf_InfoUriGetPurp(ST_XML_ELEM *pstUri, 
                ST_ZOS_USTR **ppstPurp);

/* conference-info get the uri modified */
ZFUNC ZINT EaConf_InfoUriGetMdfy(ST_XML_ELEM *pstUri, 
                ST_XML_ELEM **ppstMdfy);

/* conference-info get the users data */
ZFUNC ZINT EaConf_InfoUsrsGetData(ST_XML_ELEM *pstUsrs, 
                ST_ZOS_USTR **ppstData);

/* conference-info get the first users user */
ZFUNC ZINT EaConf_InfoUsrsGetFirstUsr(ST_XML_ELEM *pstUsrs, 
                ST_XML_ELEM **ppstUsr);

/* conference-info get the next users user */
ZFUNC ZINT EaConf_InfoUsrsGetNextUsr(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstUsr);

/* conference-info get the users state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoUsrsGetSta(ST_XML_ELEM *pstUsrs, 
                ZUCHAR *pucSta);

/* conference-info get the user display-text */
ZFUNC ZINT EaConf_InfoUsrGetDispTxt(ST_XML_ELEM *pstUsr, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the user associated-aors */
ZFUNC ZINT EaConf_InfoUsrGetAssoAors(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstAssoAors);

/* conference-info get the user roles */
ZFUNC ZINT EaConf_InfoUsrGetRoles(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstRoles);

/* conference-info get the user languages */
ZFUNC ZINT EaConf_InfoUsrGetLangs(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstLangs);

/* conference-info get the user cascaded-focus */
ZFUNC ZINT EaConf_InfoUsrGetCasFocus(ST_XML_ELEM *pstUsr, 
                ST_ZOS_USTR **ppstCasFocus);

/* conference-info get the user endpoint */
ZFUNC ZINT EaConf_InfoUsrGetEndp(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstEndp);

/* conference-info get the user entity */
ZFUNC ZINT EaConf_InfoUsrGetEnt(ST_XML_ELEM *pstUsr, 
                ST_ZOS_USTR **ppstEnt);

/* conference-info get the user state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoUsrGetSta(ST_XML_ELEM *pstUsr, 
                ZUCHAR *pucSta);

/* conference-info get the user yourown */
ZFUNC ZINT EaConf_InfoUsrGetYourown(ST_XML_ELEM *pstUsr, 
                ZBOOL *pbYourown);

/* conference-info get the first user-roles entry */
ZFUNC ZINT EaConf_InfoUrolesGetFirstEntry(ST_XML_ELEM *pstUroles, 
                ST_XML_ELEM **ppstEntry);

/* conference-info get the next user-roles entry */
ZFUNC ZINT EaConf_InfoUrolesGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* conference-info user-roles entry get roles type EN_EA_CONF_INFO_USR_ROLES_TYPE */
ZFUNC ZINT EaConf_InfoUrolesEntryGetType(ST_XML_ELEM *pstRoles, 
                ZUCHAR *pucRolesType);

/* conference-info get the endpoint display-text */
ZFUNC ZINT EaConf_InfoEndpGetDispTxt(ST_XML_ELEM *pstEndp, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the endpoint referred */
ZFUNC ZINT EaConf_InfoEndpGetRefer(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstRefer);

/* conference-info get the endpoint status */
ZFUNC ZINT EaConf_InfoEndpGetStat(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstStat);

/* conference-info get the endpoint joining-method */
ZFUNC ZINT EaConf_InfoEndpGetJoinMethod(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstJoinMethod);

/* conference-info get the endpoint joining-info */
ZFUNC ZINT EaConf_InfoEndpGetJoinInfo(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstJoinInfo);

/* conference-info get the disconnection-method EN_EA_CONF_INFO_DISC_TYPE */
ZFUNC ZINT EaConf_InfoEndpGetDiscMethod(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstDiscMethod);

/* conference-info get the endpoint disconnection-info */
ZFUNC ZINT EaConf_InfoEndpGetDiscInfo(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstDiscInfo);

/* conference-info get the endpoint media */
ZFUNC ZINT EaConf_InfoEndpGetMedia(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstMedia);

/* conference-info get the endpoint call-info */
ZFUNC ZINT EaConf_InfoEndpGetCallInfo(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstCallInfo);

/* conference-info get the endpoint entity */
ZFUNC ZINT EaConf_InfoEndpGetEnt(ST_XML_ELEM *pstEndp, 
                ST_ZOS_USTR **ppstEnt);

/* conference-info get the endpoint state */
ZFUNC ZINT EaConf_InfoEndpGetSta(ST_XML_ELEM *pstEndp, 
                ZUCHAR *pucSta);

/* conference-info get the endpoint status EN_EA_CONF_INFO_ENDP_STAT_TYPE */
ZFUNC ZINT EaConf_InfoEndpStatGetType(ST_XML_ELEM *pstStat,
                ZUCHAR *pucType);

/* conference-info get the joining type EN_EA_CONF_INFO_JOINING_TYPE */
ZFUNC ZINT EaConf_InfoJoiningGetType(ST_XML_ELEM *pstJoining, 
                ZUCHAR *pucType);

/* conference-info get the disconnection type EN_EA_CONF_INFO_DISC_TYPE */
ZFUNC ZINT EaConf_InfoDiscGetType(ST_XML_ELEM *pstDisc, 
                ZUCHAR *pucType);

/* conference-info get the execution when */
ZFUNC ZINT EaConf_InfoExecGetWhen(ST_XML_ELEM *pstExec, 
                ST_ZOS_USTR **ppstWhen);

/* conference-info get the execution reason */
ZFUNC ZINT EaConf_InfoExecGetReason(ST_XML_ELEM *pstExec, 
                ST_ZOS_USTR **ppstReason);

/* conference-info get the execution by */
ZFUNC ZINT EaConf_InfoExecGetBy(ST_XML_ELEM *pstExec, 
                ST_ZOS_USTR **ppstBy);

/* conference-info get the call sip */
ZFUNC ZINT EaConf_InfoCallGetSip(ST_XML_ELEM *pstCall, 
                ST_XML_ELEM **ppstSip);

/* conference-info get the sip-dialog-id display-text */
ZFUNC ZINT EaConf_InfoSipDlgIdGetDispTxt(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the sip-dialog-id call-id */
ZFUNC ZINT EaConf_InfoSipDlgIdGetCallId(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR **ppstCallId);

/* conference-info get the sip-dialog-id from-tag */
ZFUNC ZINT EaConf_InfoSipDlgIdGetFromTag(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR **ppstFromTag);

/* conference-info get the sip-dialog-id to-tag */
ZFUNC ZINT EaConf_InfoSipDlgIdGetToTag(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR **ppstToTag);

/* conference-info get the media display-text */
ZFUNC ZINT EaConf_InfoMediaGetDispTxt(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR **ppstDispTxt);

/* conference-info get the media type */
ZFUNC ZINT EaConf_InfoMediaGetType(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR **ppstType);

/* conference-info get the media label */
ZFUNC ZINT EaConf_InfoMediaGetLabel(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR **ppstLabel);

/* conference-info get the media src-id */
ZFUNC ZINT EaConf_InfoMediaGetSrcId(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR **ppstSrcId);

/* conference-info get the media status */
ZFUNC ZINT EaConf_InfoMediaGetStat(ST_XML_ELEM *pstMedia, 
                ST_XML_ELEM **ppstStat);

/* conference-info get the media id */
ZFUNC ZINT EaConf_InfoMediaGetId(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR **ppstId);

/* conference-info get the media status type EN_EA_CONF_INFO_DISC_TYPE */
ZFUNC ZINT EaConf_InfoMediaStatGetType(ST_XML_ELEM *pstMediaStat, 
                ZUCHAR *pucType);

/* conference-info get the first sidebars-by-val entry */
ZFUNC ZINT EaConf_InfoSbByValGetFirstEntry(ST_XML_ELEM *pstSbByVal, 
                ST_XML_ELEM **ppstEntry);

/* conference-info get the next sidebars-by-val entry */
ZFUNC ZINT EaConf_InfoSbByValGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* conference-info get the sidebars-by-val state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoSbByValGetSta(ST_XML_ELEM *pstSbByVal, 
                ZUCHAR *pucSta);

/* conference-info set the conference-info */
ZFUNC ZINT EaConf_InfoSetInfo(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs,
                ST_XML_ELEM **ppstInfo);

/* conference-info set the conference-info conference-description */
ZFUNC ZINT EaConf_InfoInfoSetConfDesc(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstConfDesc);

/* conference-info set the conference-info host-info */
ZFUNC ZINT EaConf_InfoInfoSetHostInfo(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstHostInfo);

/* conference-info set the conference-info conference-state */
ZFUNC ZINT EaConf_InfoInfoSetConfSta(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstConfSta);

/* conference-info set the conference-info users */
ZFUNC ZINT EaConf_InfoInfoSetUsrs(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstUsrs);

/* conference-info set the conference-info sidebars-by-ref */
ZFUNC ZINT EaConf_InfoInfoSetSbByRef(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstSbByRef);

/* conference-info set the conference-info sidebars-by-val */
ZFUNC ZINT EaConf_InfoInfoSetSbByVal(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstSbByVal);

/* conference-info set the conference-info entity */
ZFUNC ZINT EaConf_InfoInfoSetEnt(ST_XML_ELEM *pstInfo, 
                ST_ZOS_USTR *pstEnt);

/* conference-info set the conference-info state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoInfoSetSta(ST_XML_ELEM *pstInfo, 
                ZUCHAR ucSta);

/* conference-info set the conference-info version */
ZFUNC ZINT EaConf_InfoInfoSetVer(ST_XML_ELEM *pstInfo, 
                ZUINT iVer);

/* conference-info set the state type EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoStaSetType(ST_XML_ELEM *pstInfo, ZUCHAR ucType);

/* conference-info set the conference-description display-text */
ZFUNC ZINT EaConf_InfoConfDescSetDispTxt(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the conference-description subject */
ZFUNC ZINT EaConf_InfoConfDescSetSubject(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR *pstSubject);

/* conference-info set the conference-description free-text */
ZFUNC ZINT EaConf_InfoConfDescSetFreeTxt(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR *pstFreeTxt);

/* conference-info set the conference-description keywords */
ZFUNC ZINT EaConf_InfoConfDescSetKey(ST_XML_ELEM *pstConfDesc, 
                ST_ZOS_USTR *pstKey);

/* conference-info set the conference-description conf-uris */
ZFUNC ZINT EaConf_InfoConfDescSetConfUris(ST_XML_ELEM *pstConfDesc, 
                ST_XML_ELEM **ppstUris);

/* conference-info set the conference-description service-uris */
ZFUNC ZINT EaConf_InfoConfDescSetSrvUris(ST_XML_ELEM *pstConfDesc, 
                ST_XML_ELEM **ppstSrvUris);

/* conference-info set the conference-description maximum-user-count */
ZFUNC ZINT EaConf_InfoConfDescSetMaxUsrCount(ST_XML_ELEM *pstConfDesc, 
                ZUINT iMaxUsrCount);

/* conference-info set the conference-description available-media */
ZFUNC ZINT EaConf_InfoConfDescSetAvailMedia(ST_XML_ELEM *pstConfDesc, 
                ST_XML_ELEM **ppstAvailMedia);

/* conference-info set the host-type display-text */
ZFUNC ZINT EaConf_InfoHostDescSetDispTxt(ST_XML_ELEM *pstHostInfo, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the host-type web-page */
ZFUNC ZINT EaConf_InfoHostDescSetWebPage(ST_XML_ELEM *pstHostInfo, 
                ST_ZOS_USTR *pstWebPage);

/* conference-info set the host-type uris */
ZFUNC ZINT EaConf_InfoHostDescSetUris(ST_XML_ELEM *pstHostInfo, 
                ST_XML_ELEM **ppstUris);

/* conference-info set the conference-state user-count */
ZFUNC ZINT EaConf_InfoConfStaSetUsrCount(ST_XML_ELEM *pstConfSta, 
                ZUINT iMaxUsrCount);

/* conference-info set the conference-state active */
ZFUNC ZINT EaConf_InfoConfStaSetActive(ST_XML_ELEM *pstConfSta, 
                ZBOOL bActive);

/* conference-info set the conference-state locked */
ZFUNC ZINT EaConf_InfoConfStaSetLock(ST_XML_ELEM *pstConfSta, 
                ZBOOL bLock);

/* conference-info set the conference-media entry */
ZFUNC ZINT EaConf_InfoConfMediaSetEntry(ST_XML_ELEM *pstConfMedia, 
                ST_XML_ELEM **ppstEntry);

/* conference-info set the conference-medium display-text */
ZFUNC ZINT EaConf_InfoConfMediumSetDispTxt(ST_XML_ELEM *pstConfMedium, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the conference-medium type */
ZFUNC ZINT EaConf_InfoConfMediumSetType(ST_XML_ELEM *pstConfMedium, 
                ST_ZOS_USTR *pstType);

/* conference-info set the conference-medium status */
ZFUNC ZINT EaConf_InfoConfMediumSetStat(ST_XML_ELEM *pstConfMedium, 
                ST_XML_ELEM **ppstStat);

/* conference-info set the conference-medium label */
ZFUNC ZINT EaConf_InfoConfMediumSetLabel(ST_XML_ELEM *pstConfMedium, 
                ST_ZOS_USTR *pstLabel);

/* conference-info set the uris entry */
ZFUNC ZINT EaConf_InfoUrisSetEntry(ST_XML_ELEM *pstUris, 
                ST_XML_ELEM **ppstEntry);

/* conference-info set the uris state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoUrisSetSta(ST_XML_ELEM *pstUris, 
                ZUCHAR ucSta);

/* conference-info set the uri uri */
ZFUNC ZINT EaConf_InfoUriSetUri(ST_XML_ELEM *pstUri, 
                ST_ZOS_USTR *pstVal);

/* conference-info set the uri display-text */
ZFUNC ZINT EaConf_InfoUriSetDispTxt(ST_XML_ELEM *pstUri, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the uri purpose */
ZFUNC ZINT EaConf_InfoUriSetPurp(ST_XML_ELEM *pstUri, 
                ST_ZOS_USTR *pstPurp);

/* conference-info set the uri modified */
ZFUNC ZINT EaConf_InfoUriSetMdfy(ST_XML_ELEM *pstUri, 
                ST_XML_ELEM **ppstMdfy);

/* conference-info set the users user */
ZFUNC ZINT EaConf_InfoUsrsSetUsr(ST_XML_ELEM *pstUsrs, 
                ST_XML_ELEM **ppstUsr);

/* conference-info set the users state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoUsrsSetSta(ST_XML_ELEM *pstUsrs, 
                ZUCHAR ucSta);

/* conference-info set the user display-text */
ZFUNC ZINT EaConf_InfoUsrSetDispTxt(ST_XML_ELEM *pstUsr, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the user associated-aors */
ZFUNC ZINT EaConf_InfoUsrSetAssoAors(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstAssoAors);

/* conference-info set the user roles */
ZFUNC ZINT EaConf_InfoUsrSetRoles(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstRoles);

/* conference-info set the user languages */
ZFUNC ZINT EaConf_InfoUsrSetLangs(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstLangs);

/* conference-info set the user cascaded-focus */
ZFUNC ZINT EaConf_InfoUsrSetCasFocus(ST_XML_ELEM *pstUsr, 
                ST_ZOS_USTR *pstCasFocus);

/* conference-info set the user endpoint */
ZFUNC ZINT EaConf_InfoUsrSetEndp(ST_XML_ELEM *pstUsr, 
                ST_XML_ELEM **ppstEndp);

/* conference-info set the user entity */
ZFUNC ZINT EaConf_InfoUsrSetEnt(ST_XML_ELEM *pstUsr, 
                ST_ZOS_USTR *pstEnt);

/* conference-info set the user state */
ZFUNC ZINT EaConf_InfoUsrSetSta(ST_XML_ELEM *pstUsr, 
                ZUCHAR ucSta);

/* conference-info set the user yourown */
ZFUNC ZINT EaConf_InfoUsrSetYourown(ST_XML_ELEM *pstUsr, 
                ZBOOL bYourown);

/* conference-info set the user-roles entry */
ZFUNC ZINT EaConf_InfoUrolesSetEntry(ST_XML_ELEM *pstUroles, 
                ST_XML_ELEM **ppstEntry);

/* conference-info set the endpoint display-text */
ZFUNC ZINT EaConf_InfoEndpSetDispTxt(ST_XML_ELEM *pstEndp, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the endpoint referred */
ZFUNC ZINT EaConf_InfoEndpSetRefer(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstRefer);

/* conference-info set the endpoint status */
ZFUNC ZINT EaConf_InfoEndpSetStat(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstStat);

/* conference-info set the endpoint joining-method */
ZFUNC ZINT EaConf_InfoEndpSetJoinMethod(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstJoinMethod);

/* conference-info set the endpoint joining-info */
ZFUNC ZINT EaConf_InfoEndpSetJoinInfo(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstJoinInfo);

/* conference-info set the endpoint disconnection-method */
ZFUNC ZINT EaConf_InfoEndpSetDiscMethod(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstDiscMethod);

/* conference-info set the endpoint disconnection-info */
ZFUNC ZINT EaConf_InfoEndpSetDiscInfo(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstDiscInfo);

/* conference-info set the endpoint media */
ZFUNC ZINT EaConf_InfoEndpSetMedia(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstMedia);

/* conference-info set the endpoint call-info */
ZFUNC ZINT EaConf_InfoEndpSetCallInfo(ST_XML_ELEM *pstEndp, 
                ST_XML_ELEM **ppstCallInfo);

/* conference-info set the endpoint entity */
ZFUNC ZINT EaConf_InfoEndpSetEnt(ST_XML_ELEM *pstEndp, 
                ST_ZOS_USTR *pstEnt);

/* conference-info set the endpoint state */
ZFUNC ZINT EaConf_InfoEndpSetSta(ST_XML_ELEM *pstEndp, 
                ZUCHAR ucSta);

/* conference-info set the endp status type EN_EA_CONF_INFO_ENDP_STAT_TYPE */
ZFUNC ZINT EaConf_InfoEndpStatSetType(ST_XML_ELEM *pstEndpStat, 
                ZUCHAR ucType);

/* conference-info set the joining type EN_EA_CONF_INFO_JOINING_TYPE */
ZFUNC ZINT EaConf_InfoJoiningSetType(ST_XML_ELEM *pstJoining, 
                ZUCHAR ucType);

/* conference-info set the disconnection type EN_EA_CONF_INFO_DISC_TYPE */
ZFUNC ZINT EaConf_InfoDiscSetType(ST_XML_ELEM *pstDisc, 
                ZUCHAR ucType);

/* conference-info set the execution when */
ZFUNC ZINT EaConf_InfoExecSetWhen(ST_XML_ELEM *pstExec, 
                ST_ZOS_USTR *pstWhen);

/* conference-info set the execution reason */
ZFUNC ZINT EaConf_InfoExecSetReason(ST_XML_ELEM *pstExec, 
                ST_ZOS_USTR *pstReason);

/* conference-info set the execution by */
ZFUNC ZINT EaConf_InfoExecSetBy(ST_XML_ELEM *pstExec, 
                ST_ZOS_USTR *pstBy);

/* conference-info set the call sip */
ZFUNC ZINT EaConf_InfoCallSetSip(ST_XML_ELEM *pstCall, 
                ST_XML_ELEM **ppstSip);

/* conference-info set the sip-dialog-id display-text */
ZFUNC ZINT EaConf_InfoSipDlgIdSetDispTxt(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the sip-dialog-id call-id */
ZFUNC ZINT EaConf_InfoSipDlgIdSetCallId(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR *pstCallId);

/* conference-info set the sip-dialog-id from-tag */
ZFUNC ZINT EaConf_InfoSipDlgIdSetFromTag(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR *pstFromTag);

/* conference-info set the sip-dialog-id to-tag */
ZFUNC ZINT EaConf_InfoSipDlgIdSetToTag(ST_XML_ELEM *pstSipDlgId, 
                ST_ZOS_USTR *pstToTag);

/* conference-info set the media display-text */
ZFUNC ZINT EaConf_InfoMediaSetDispTxt(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR *pstDispTxt);

/* conference-info set the media type */
ZFUNC ZINT EaConf_InfoMediaSetType(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR *pstType);

/* conference-info set the media label */
ZFUNC ZINT EaConf_InfoMediaSetLabel(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR *pstLabel);

/* conference-info set the media src-id */
ZFUNC ZINT EaConf_InfoMediaSetSrcId(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR *pstSrcId);

/* conference-info set the media status */
ZFUNC ZINT EaConf_InfoMediaSetStat(ST_XML_ELEM *pstMedia, 
                ST_XML_ELEM **ppstStat);

/* conference-info set the media id */
ZFUNC ZINT EaConf_InfoMediaSetId(ST_XML_ELEM *pstMedia, 
                ST_ZOS_USTR *pstId);

/* conference-info set the media status type EN_EA_CONF_INFO_DISC_TYPE */
ZFUNC ZINT EaConf_InfoMediaStatSetType(ST_XML_ELEM *pstMediaStat, 
                ZUCHAR ucType);

/* conference-info set the sidebars-by-val entry */
ZFUNC ZINT EaConf_InfoSbByValSetEntry(ST_XML_ELEM *pstSbByVal, 
                ST_XML_ELEM **ppstEntry);

/* conference-info set the sidebars-by-val state EN_EA_CONF_INFO_STA_TYPE */
ZFUNC ZINT EaConf_InfoSbByValSetSta(ST_XML_ELEM *pstSbByVal, 
                ZUCHAR ucSta);

#ifdef __cplusplus
}
#endif

#endif /* _EA_CONF_INFO_H__ */

