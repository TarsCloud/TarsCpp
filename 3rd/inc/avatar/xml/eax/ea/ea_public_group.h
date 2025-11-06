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
  File name     : ea_public_group.h
  Module        : eax public group
  Author        : shuia.xu
  Version       : 0.1
  Created on    : 2016-10-19
  Description   :
    Marcos and structure definitions required by the eax public-gourp.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PUBLIC_GROUP_H__
#define _EA_PUBLIC_GROUP_H__

#ifdef __cplusplus
extern "C"{
#endif

/* public-gourp user roles type */
typedef enum EN_EA_PUBLIC_GROUP_USR_ROLES_TYPE
{
    EN_EA_PUBLIC_GROUP_USR_ROLES_CHAIRMAN,        
    EN_EA_PUBLIC_GROUP_USR_ROLES_PARTP
} EN_EA_PUBLIC_GROUP_USR_ROLES_TYPE;

/* public-gourp user status type */
typedef enum EN_EA_PUBLIC_GROUP_USR_STATUS_TYPE
{
    EN_EA_PUBLIC_GROUP_USR_ROLES_ACTIVE,        
    EN_EA_PUBLIC_GROUP_USR_ROLES_PENDING
} EN_EA_PUBLIC_GROUP_USR_STATUS_TYPE;

/* public-gourp need permit type */
typedef enum EN_EA_PUBLIC_GROUP_NEEDPERMIT_TYPE
{
    EN_EA_PUBLIC_GROUP_NEEDPERMIT_TRUE,        
    EN_EA_PUBLIC_GROUP_NEEDPERMIT_FALSE
} EN_EA_PUBLIC_GROUP_NEEDPERMIT_TYPE;

typedef enum EN_EA_PUBLIC_GROUP_TYPE
{
    EN_EA_PUBLIC_GROUP_PUBGRP,       /* public-group */
    EN_EA_PUBLIC_GROUP_LIST,         /* list */
    EN_EA_PUBLIC_GROUP_NAME,         /* name */
    EN_EA_PUBLIC_GROUP_URI,          /* uri */
    EN_EA_PUBLIC_GROUP_MAX,          /* max */
    EN_EA_PUBLIC_GROUP_ENTRY,        /* entry */
    EN_EA_PUBLIC_GROUP_STAT,         /* status */
    EN_EA_PUBLIC_GROUP_OWNER,        /* owner */
    EN_EA_PUBLIC_GROUP_DISPNAME,     /* display-name */
    EN_EA_PUBLIC_GROUP_METADATA,     /* meta-data */
    EN_EA_PUBLIC_GROUP_CREATOR,      /* creator */
    EN_EA_PUBLIC_GROUP_IDENTIFIER,   /* identifier */
    EN_EA_PUBLIC_GROUP_TIMETAMP,     /* timestamp */
    EN_EA_PUBLIC_GROUP_NEEDPERMIT,   /* need permit */
    EN_EA_PUBLIC_GROUP_ACTIVE,       /* active */
    EN_EA_PUBLIC_GROUP_PENDING,      /* pending */
    EN_EA_PUBLIC_GROUP_TRUE,         /* true */
    EN_EA_PUBLIC_GROUP_FALSE,         /* false */
    EN_EA_PUBLIC_GROUP_BULLETIN,     /* bulletin */
    EN_EA_PUBLIC_GROUP_VALIDTYPE,    /* validtype */
    EN_EA_PUBLIC_GROUP_GRP_TYPE,     /* grouptype */
    EN_EA_PUBLIC_GROUP_GROUPTYPE,    /* group-type */
    EN_EA_PUBLIC_GROUP_NEEDRESPONSE, /* need-response */
    EN_EA_PUBLIC_GROUP_CONVID,       /* conversationId */
    EN_EA_PUBLIC_GROUP_NEWAPP,       /* newapp */
    EN_EA_PUBLIC_GROUP_MSGID,        /* msgid */
    EN_EA_PUBLIC_GROUP_GROUPINFO,    /* groupinfo */
    EN_EA_PUBLIC_GROUP_STATUS,       /* groupstatus */
    EN_EA_PUBLIC_GROUP_VERSION,      /* version */
    EN_EA_PUBLIC_GROUP_ISCHANGE,     /* ischange */
    EN_EA_PUBLIC_GROUP_OPERA_TYPE,   /* group operation type */
    EN_EA_PUBLIC_GROUP_LOCATION,     /* location */
    EN_EA_PUBLIC_GROUP_LONGITUDE,    /* longitude */
    EN_EA_PUBLIC_GROUP_LATITUDE,     /* latitude */
    EN_EA_PUBLIC_GROUP_COMMAND,      /* command */
    EN_EA_PUBLIC_GROUP_VALID_TIME,   /* validate-time */
    EN_EA_PUBLIC_GROUP_COMMAND_ID,   /* command-id */
    EN_EA_PUBLIC_GROUP_GRPID,        /* group-id */  
    EN_EA_PUBLIC_GROUP_GRPNAME,      /* group-name */     
} EN_EA_PUBLIC_GROUP_TYPE;

/* public-gourp get the public-gourp */
ZFUNC ZINT EaPub_GrpGetPubGrp(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstInfo);

/* public-group set the public-group */
ZFUNC ZINT EaPub_GrpSetPubGrp(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstPubGrp);

/* public-group get the list */
ZFUNC ZINT EaPub_GrpPubGrpGetList(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstUsrs);

/* public-group set the list */
ZFUNC ZINT EaPub_GrpPubGrpSetList(ST_XML_ELEM *pstPubGrp,
                 ST_XML_ELEM **ppstList);

/* public-group get the list name */
ZFUNC ZINT EaPub_GrpPubGrpGetListName(ST_XML_ELEM *pstList, 
                ST_ZOS_USTR **ppstName);

/* public-group get the list name */
ZFUNC ZINT EaPub_GrpPubGrpSetListName(ST_XML_ELEM *pstList, 
                ST_ZOS_USTR *pstName);

/* public-group get the list uri */
ZFUNC ZINT EaPub_GrpPubGrpGetListUri(ST_XML_ELEM *pstList, 
                ST_ZOS_USTR **ppstUri);

/* public-group set the list uri */
ZFUNC ZINT EaPub_GrpPubGrpSetListUri(ST_XML_ELEM *pstList, 
                ST_ZOS_USTR *pstUri);

/* public-group get the list max */
ZFUNC ZINT EaPub_GrpPubGrpGetListMax(ST_XML_ELEM *pstList, 
                ZUINT *piMax);

/* public-group set the list max */
ZFUNC ZINT EaPub_GrpPubGrpSetListMax(ST_XML_ELEM *pstList, 
                ZUINT iMax);

ZFUNC ZINT EaPub_GrpPubGrpGetListBulletin(ST_XML_ELEM *pstList, 
                ST_ZOS_USTR **ppstBulletin);

/* public-group set the list bulletin */
ZFUNC ZINT EaPub_GrpPubGrpSetListBulletin(ST_XML_ELEM *pstList, 
                ST_ZOS_USTR *pstBulletin);

/* public-group get the list validtype */
ZFUNC ZINT EaPub_GrpPubGrpGetListValidtype(ST_XML_ELEM *pstList, 
                ZUINT *piValidtype);

/* public-group set the list validtype */
ZFUNC ZINT EaPub_GrpPubGrpSetListValidtype(ST_XML_ELEM *pstList, 
                ZUINT iValidtype);

/* public-group get the first list entry */
ZFUNC ZINT EaPub_GrpListGetFirstEntry(ST_XML_ELEM *pstList, 
                ST_XML_ELEM **ppstEntry);

/* public-group get the next list entry */
ZFUNC ZINT EaPub_GrpListGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* public-group set list entry */
ZFUNC ZINT EaPub_GrpListSetEntry(ST_XML_ELEM *pstList, 
                ST_XML_ELEM **ppstEntry);

/* public-group get the entry uri */
ZFUNC ZINT EaPub_GrpEntryGetUri(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstUri);

/* public-group set the entry uri */
ZFUNC ZINT EaPub_GrpEntrySetUri(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR *pstUri);

/* public-group set the entry version */
ZFUNC ZINT EaPub_GrpEntrySetVersion(ST_XML_ELEM *pstEntry, 
                ZUINT iVersion);

/* public-group get the entry status */
ZFUNC ZINT EaPub_GrpEntryGetStatus(ST_XML_ELEM *pstEntry, 
                ZUINT *piStatus);

/* public-group set the entry status */
ZFUNC ZINT EaPub_GrpEntrySetStatus(ST_XML_ELEM *pstEntry, 
                ZUINT iStatus);

/* public-group get the entry owner-value */
ZFUNC ZINT EaPub_GrpEntryGetOwnerValue(ST_XML_ELEM *pstEntry, 
                ZBOOL *pbOwner);

/* public-group set the entry owner-value */
ZFUNC ZINT EaPub_GrpEntrySetOwnerValue(ST_XML_ELEM *pstEntry, 
                ZBOOL bOwner);

/* public-group get the meta-data disp-name data */
ZFUNC ZINT EaPub_GrpEntryGetDispNameData(ST_XML_ELEM *pstMetaData, 
                ST_ZOS_USTR **ppstDispName);

/* public-group set the meta-data disp-name data */
ZFUNC ZINT EaPub_GrpEntrySetDispNameData(ST_XML_ELEM *pstMetaData, 
                ST_ZOS_USTR *pstDispName);

/* public-group get the meta-data */
ZFUNC ZINT EaPub_GrpPubGrpGetMetaData(ST_XML_ELEM *pstInfo,
                 ST_XML_ELEM **ppstUsrs);

/* public-group set the meta-data */
ZFUNC ZINT EaPub_GrpPubGrpSetMetaData(ST_XML_ELEM *ppstPubGrp,
                 ST_XML_ELEM **ppstMetaData);

/* public-group get the meta-data creator data */
ZFUNC ZINT EaPub_GrpPubGrpGetMetaDataCreatorData(ST_XML_ELEM *pstMetaData, 
                ST_ZOS_USTR **ppstCreator);  

/* public-group set the meta-data creator data */
ZFUNC ZINT EaPub_GrpPubGrpSetMetaDataCreatorData(ST_XML_ELEM *pstMetaData, 
                ST_ZOS_USTR *pstCreator);

/* public-group get the meta-data identifier data */
ZFUNC ZINT EaPub_GrpPubGrpGetMetaDataIdentifierData(ST_XML_ELEM *pstMetaData, 
                ST_ZOS_USTR **ppstIdentifier);

/* public-group set the meta-data identifier data */
ZFUNC ZINT EaPub_GrpPubGrpSetMetaDataIdentifierData(ST_XML_ELEM *pstMetaData, 
                ST_ZOS_USTR *pstIdentifier);

/* public-group get the timestamp data */
ZFUNC ZINT EaPub_GrpPubGrpGetTimestampData(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstTimestamp);

/* public-group set the timestamp data */
ZFUNC ZINT EaPub_GrpPubGrpSetTimestampData(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstTimestamp);

/* public-group get the meta-data need-permit data */
ZFUNC ZINT EaPub_GrpPubGrpGetMetaDataNeedPermitData(ST_XML_ELEM *pstMetaData, 
                ZBOOL *pbNeedPermit);

/* public-group set the meta-data need-permit data */
ZFUNC ZINT EaPub_GrpPubGrpSetMetaDataNeedPermitData(ST_XML_ELEM *pstMetaData, 
                ZBOOL bNeedPermit);

/* public-group get the meta-data group-type data */
ZFUNC ZINT EaPub_GrpPubGrpGetMetaDataGroupTypeData(ST_XML_ELEM *pstMetaData, 
                ZUINT *piGroupType);

/* public-group set the meta-data group-type data */
ZFUNC ZINT EaPub_GrpPubGrpSetMetaDataGroupTypeData(ST_XML_ELEM *pstMetaData, 
                ZUINT iGroupType);

/* public-group get the meta-data need-response data */
ZFUNC ZINT EaPub_GrpPubGrpGetMetaDataNeedResponseData(ST_XML_ELEM *pstMetaData, 
                ZBOOL *pbNeedResponse);

/* public-group set the meta-data need-response data */
ZFUNC ZINT EaPub_GrpPubGrpSetMetaDataNeedResponseData(ST_XML_ELEM *pstMetaData, 
                ZBOOL bNeedResponse);

/* public-group set the newapp data */
ZFUNC ZINT EaPub_GrpPubGrpSetNewappData(ST_XML_ELEM *pstPubGrp, 
                ZBOOL bNewapp);

/* public-group set the msgid data */
ZFUNC ZINT EaPub_GrpPubGrpSetMsgidData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR *pstMsgid);

/* public-group get the first list groupinfo */
ZFUNC ZINT EaPub_GrpListGetFirstGroupinfo(ST_XML_ELEM *pstList, 
                ST_XML_ELEM **ppstGroupinfo);

/* public-group get the next list groupinfo */
ZFUNC ZINT EaPub_GrpListGetNextGroupinfo(ST_XML_ELEM *pstGroupinfo, 
                ST_XML_ELEM **ppstGroupinfo);

/* public-group get the groupinfo name */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupinfoName(ST_XML_ELEM *pstGroupinfo, 
                ST_ZOS_USTR **ppstName);

/* public-group get the groupinfo uri */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupinfoUri(ST_XML_ELEM *pstGroupinfo, 
                ST_ZOS_USTR **ppstUri);

/* public-group get the groupinfo max */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupinfoMax(ST_XML_ELEM *pstGroupinfo, 
                ZUINT *piMax);

/* public-group get the groupinfo groupstatus */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupinfoStatus(ST_XML_ELEM *pstGroupinfo, 
                ZUINT *piStatus);

/* public-group get the groupinfo version */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupinfoVer(ST_XML_ELEM *pstGroupinfo, 
                ZUINT *piVersion);

/* public-group get the groupinfo grouptype */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupinfoType(ST_XML_ELEM *pstGroupinfo, 
                ZUINT *piType);

/* public-group get the groupinfo ischange */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupinfoIsChange(ST_XML_ELEM *pstGroupinfo, 
                ZBOOL *pbIsChange);

/* public-group get the conversationId data */
ZFUNC ZINT EaPub_GrpPubGrpGetConvIdData(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstConvId);

/* public-group set the conversationId data */
ZFUNC ZINT EaPub_GrpPubGrpSetConvIdData(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstConvId);

/* public-group get the type data */
ZFUNC ZINT EaPub_GrpPubGrpGetTypeData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR **ppstType);

/* public-group set the type data */
ZFUNC ZINT EaPub_GrpPubGrpSetTypeData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR *pstType);

/* public-group get the location */
ZFUNC ZINT EaPub_GrpPubGrpGetLocation(ST_XML_ELEM *pstPubGrp,
                 ST_XML_ELEM **ppstLocation);

/* public-group set the location */
ZFUNC ZINT EaPub_GrpPubGrpSetLocation(ST_XML_ELEM *pstPubGrp,
                 ST_XML_ELEM **ppstLocation);

/* public-group get the location longitude data */
ZFUNC ZINT EaPub_GrpLocationGetLongitudeData(ST_XML_ELEM *pstLocation, 
                ST_ZOS_USTR **ppstLongitude);

/* public-group set the location longitude data */
ZFUNC ZINT EaPub_GrpLocationSetLongitudeData(ST_XML_ELEM *pstLocation, 
                ST_ZOS_USTR *pstLongitude);

/* public-group get the location latitude data */
ZFUNC ZINT EaPub_GrpLocationGetLatitudeData(ST_XML_ELEM *pstLocation, 
                ST_ZOS_USTR **ppstLatitude);

/* public-group set the location latitude data */
ZFUNC ZINT EaPub_GrpLocationSetLatitudeData(ST_XML_ELEM *pstLocation, 
                ST_ZOS_USTR *pstLatitude);

/* public-group get the command data */
ZFUNC ZINT EaPub_GrpPubGrpGetCommandData(ST_XML_ELEM *pstPubGrp, 
                ZUINT *piCommand);

/* public-group set the command data */
ZFUNC ZINT EaPub_GrpPubGrpSetCommandData(ST_XML_ELEM *pstPubGrp, 
                ZUINT iCommand);

/* public-group get validate-time data */
ZFUNC ZINT EaPub_GrpPubGrpGetValidTimeData(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstValidTime);

/* public-group set the validate-time data */
ZFUNC ZINT EaPub_GrpPubGrpSetValidTimeData(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstValidTime);

/* public-group get the command id data */
ZFUNC ZINT EaPub_GrpPubGrpGetCommandIdData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR **ppstCommandId);

/* public-group set the command-id data */
ZFUNC ZINT EaPub_GrpPubGrpSetCommandIdData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR *pstCommandId);

/* public-group get the group-id data */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupIdData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR **ppstGroupId);

/* public-group set the group-id data */
ZFUNC ZINT EaPub_GrpPubGrpSetGroupIdData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR *pstGroupId);

/* public-group get the group-name data */
ZFUNC ZINT EaPub_GrpPubGrpGetGroupNameData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR **ppstGroupName);

/* public-group set the group-name data */
ZFUNC ZINT EaPub_GrpPubGrpSetGroupNameData(ST_XML_ELEM *pstPubGrp, 
                ST_ZOS_USTR *pstGroupName);

#ifdef __cplusplus
}
#endif

#endif /* _EA_CONF_INFO_H__ */

