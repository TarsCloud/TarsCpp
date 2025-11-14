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
  File name     : ea_poc_lsrv.h
  Module        : eax poc list-service
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax poc list-service.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_POC_LSRV_H__
#define _EA_POC_LSRV_H__

#ifdef __cplusplus
extern "C"{
#endif

/************* urn:oma:xml:poc:list-service **************/
typedef enum EN_EA_OMA_POC_LSRV_TYPE
{
    EN_EA_OMA_POC_LSRV_GRP,            /* group */
    EN_EA_OMA_POC_LSRV_LST_SRV,        /* list-service */
    EN_EA_OMA_POC_LSRV_DISP_NAME,      /* display-name */
    EN_EA_OMA_POC_LSRV_LST,            /* list */
    EN_EA_OMA_POC_LSRV_IVT_MEMBER,     /* invite-member */
    EN_EA_OMA_POC_LSRV_MAX_PARTP_COUNT, /* max-participant-count */
    EN_EA_OMA_POC_LSRV_URI,            /* uri */
    EN_EA_OMA_POC_LSRV_ENTRY,          /* entry */
    EN_EA_OMA_POC_LSRV_EXTERNAL,       /* external */
    EN_EA_OMA_POC_LSRV_IS_LST_MEMBER,  /* is-list-member */
    EN_EA_OMA_POC_LSRV_ALLOW_CONF_STA, /* allow-conference-state */
    EN_EA_OMA_POC_LSRV_ALLOW_IVT_USRS_DYNA, /* allow-invite-users-dynamically */
    EN_EA_OMA_POC_LSRV_JOIN_HANDLING,  /* join-handling */
    EN_EA_OMA_POC_LSRV_ALLOW_INIT_CONF, /* allow-initiate-conference */
    EN_EA_OMA_POC_LSRV_ALLOW_ANONYMITY, /* allow-anonymity */
    EN_EA_OMA_POC_LSRV_IS_KEY_PARTP    /* is-key-participant */
} EN_EA_OMA_POC_LSRV_TYPE;

/* poc list-service get the group */
ZFUNC ZINT EaPoc_LsrvGetGrp(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstGrp);

/* poc list-service get the list-service */
ZFUNC ZINT EaPoc_LsrvGetLsrv(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstLsv);

/* poc list-service get the first list-service */
ZFUNC ZINT EaPoc_LsrvGrpGetFirstLsrv(ST_XML_ELEM *pstGrp, 
                ST_XML_ELEM **ppstLsrv);

/* poc list-service get the next list-service */
ZFUNC ZINT EaPoc_LsrvGrpGetNextLsrv(ST_XML_ELEM *pstLsrv, 
                ST_XML_ELEM **ppstLsrv);

/* poc list-service get the list-service display-name */
ZFUNC ZINT EaPoc_LsrvLsrvGetDispName(ST_XML_ELEM *pstLsrv,  
                ST_ZOS_USTR **ppstDispName);

/* poc list-service get the list-service list */
ZFUNC ZINT EaPoc_LsrvLsrvGetLst(ST_XML_ELEM *pstLsrv,  
                ST_XML_ELEM **ppstLst);

/* poc list-service get the list-service invite-members */
ZFUNC ZINT EaPoc_LsrvLsrvGetIvtMembers(ST_XML_ELEM *pstLsrv,  
                ZBOOL *pbIvtMembers);

/* poc list-service get the list-service max-participant-count */
ZFUNC ZINT EaPoc_LsrvLsrvGetMaxPartpCount(ST_XML_ELEM *pstLsrv,  
                ZUINT *piMaxPartpCount);

/* poc list-service get the list-service ruleset */
ZFUNC ZINT EaPoc_LsrvLsrvGetRules(ST_XML_ELEM *pstLsrv,  
                ST_XML_ELEM **ppstRules);

/* poc list-service get the list-service uri */
ZFUNC ZINT EaPoc_LsrvLsrvGetUri(ST_XML_ELEM *pstLsrv,  
                ST_ZOS_USTR **ppstUri);

/* poc list-service get the list entry by uri */
ZFUNC ZINT EaPoc_LsrvLstGetEntry(ST_XML_ELEM *pstLst,  
                ST_ZOS_USTR *pstUri, ST_XML_ELEM **ppstEntry);

/* poc list-service get the first list entry */
ZFUNC ZINT EaPoc_LsrvLstGetFirstEntry(ST_XML_ELEM *pstLst,  
                ST_XML_ELEM **ppstEntry);

/* poc list-service get the next list entry */
ZFUNC ZINT EaPoc_LsrvLstGetNextEntry(ST_XML_ELEM *pstEntry,  
                ST_XML_ELEM **ppstEntry);

/* poc list-service get the first list external */
ZFUNC ZINT EaPoc_LsrvLstGetFirstExternal(ST_XML_ELEM *pstLst,  
                ST_XML_ELEM **ppstExternal);

/* poc list-service get the next list external */
ZFUNC ZINT EaPoc_LsrvLstGetNextExternal(ST_XML_ELEM *pstExternal,  
                ST_XML_ELEM **ppstExternal);

/* poc list-service get the is-list-member */
ZFUNC ZINT EaPoc_LsrvGetIsLstMember(ST_XML_ELEM *pstElem,  
                ZBOOL *pbExist);

/* poc list-service get the allow-conference-state */
ZFUNC ZINT EaPoc_LsrvGetAllowConfSta(ST_XML_ELEM *pstElem,  
                ZBOOL *pbAllow);

/* poc list-service get the allow-invite-users-dynamically */
ZFUNC ZINT EaPoc_LsrvGetAllowIvtUsrDyna(ST_XML_ELEM *pstElem,  
                ZBOOL *pbAllow);

/* poc list-service get the join-handling */
ZFUNC ZINT EaPoc_LsrvGetJoinHandl(ST_XML_ELEM *pstElem,  
                ZBOOL *pbJoin);

/* poc list-service get the allow-initiate-conference */
ZFUNC ZINT EaPoc_LsrvGetAllowInitConf(ST_XML_ELEM *pstElem,  
                ZBOOL *pbAllow);

/* poc list-service get the allow-anonymity */
ZFUNC ZINT EaPoc_LsrvGetAllowAnony(ST_XML_ELEM *pstElem,  
                ZBOOL *pbAllow);

/* poc list-service get the is-key-participant */
ZFUNC ZINT EaPoc_LsrvGetIsKeyPartp(ST_XML_ELEM *pstElem,  
                ZBOOL *pbIsKeyPartp);

/* poc list-service set the group */
ZFUNC ZINT EaPoc_LsrvSetGrp(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstGrp);

/* poc list-service set the list-service */
ZFUNC ZINT EaPoc_LsrvGrpSetLsrv(ST_XML_ELEM *pstGrp, 
                ST_XML_ELEM **ppstLsrv);

/* poc list-service set the list-service display-name */
ZFUNC ZINT EaPoc_LsrvLsrvSetDispName(ST_XML_ELEM *pstLsrv,  
                ST_ZOS_USTR *pstDispName);

/* poc list-service set the list-service list */
ZFUNC ZINT EaPoc_LsrvLsrvSetLst(ST_XML_ELEM *pstLsrv,  
                ST_XML_ELEM **ppstLst);

/* poc list-service set the list-service invite-members */
ZFUNC ZINT EaPoc_LsrvLsrvSetIvtMembers(ST_XML_ELEM *pstLsrv,  
                ZBOOL bIvtMembers);

/* poc list-service set the list-service max-participant-count */
ZFUNC ZINT EaPoc_LsrvLsrvSetMaxPartpCount(ST_XML_ELEM *pstLsrv,  
                ZUINT iMaxPartpCount);

/* poc list-service set the list-service ruleset */
ZFUNC ZINT EaPoc_LsrvLsrvSetRules(ST_XML_ELEM *pstLsrv,  
                ST_XML_ELEM **ppstRules);

/* poc list-service set the list-service uri */
ZFUNC ZINT EaPoc_LsrvLsrvSetUri(ST_XML_ELEM *pstLsrv,  
                ST_ZOS_USTR *pstUri);

/* poc list-service set the list entry */
ZFUNC ZINT EaPoc_LsrvLstSetEntry(ST_XML_ELEM *pstLst,  
                ST_XML_ELEM **ppstEntry);

/* poc list-service set the list external */
ZFUNC ZINT EaPoc_LsrvLstSetExternal(ST_XML_ELEM *pstLst,  
                ST_XML_ELEM **ppstExternal);

/* poc list-service set the is-list-member */
ZFUNC ZINT EaPoc_LsrvSetIsLstMember(ST_XML_ELEM *pstElem);

/* poc list-service set the allow-conference-state */
ZFUNC ZINT EaPoc_LsrvSetAllowConfSta(ST_XML_ELEM *pstElem,  
                ZBOOL bAllow);

/* poc list-service set the allow-invite-users-dynamically */
ZFUNC ZINT EaPoc_LsrvSetAllowIvtUsrDyna(ST_XML_ELEM *pstElem,  
                ZBOOL bAllow);

/* poc list-service set the join-handling */
ZFUNC ZINT EaPoc_LsrvSetJoinHandl(ST_XML_ELEM *pstElem,  
                ZBOOL bJoin);

/* poc list-service set the allow-initiate-conference */
ZFUNC ZINT EaPoc_LsrvSetAllowInitConf(ST_XML_ELEM *pstElem,  
                ZBOOL bAllow);

/* poc list-service set the allow-anonymity */
ZFUNC ZINT EaPoc_LsrvSetAllowAnony(ST_XML_ELEM *pstElem,  
                ZBOOL bAllow);

/* poc list-service set the is-key-participant */
ZFUNC ZINT EaPoc_LsrvSetIsKeyPartp(ST_XML_ELEM *pstElem,  
                ZBOOL bIsKeyPartp);

#ifdef __cplusplus
}
#endif

#endif /* _EA_POC_LSRV_H__ */

