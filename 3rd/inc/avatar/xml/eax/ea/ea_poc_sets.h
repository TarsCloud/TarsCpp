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
  File name     : ea_poc_sets.h
  Module        : eax poc poc-settings
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax poc poc-settings.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_POC_SETS_H__
#define _EA_POC_SETS_H__

#ifdef __cplusplus
extern "C"{
#endif

/* poc-settings answer-mode type */
typedef enum EN_EA_POC_SETS_AM_TYPE
{
    EN_EA_POC_SETS_AM_AUTO,        
    EN_EA_POC_SETS_AM_MANUAL
} EN_EA_POC_SETS_AM_TYPE;

/* poc-settings get the poc-settings */
ZFUNC ZINT EaPoc_SetsGetSets(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstSets);

/* poc-settings get the first entity */
ZFUNC ZINT EaPoc_SetsGetFirstEnt(ST_XML_ELEM *pstSets, 
                ST_XML_ELEM **ppstEnt);

/* poc-settings get the next entity */
ZFUNC ZINT EaPoc_SetsGetNextEnt(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstEnt);

/* poc-settings get the entity isb-settings */
ZFUNC ZINT EaPoc_SetsEntGetIsb(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstIsb);

/* poc-settings get the entity am-settings */
ZFUNC ZINT EaPoc_SetsEntGetAm(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstAm);

/* poc-settings get the entity ipab-settings */
ZFUNC ZINT EaPoc_SetsEntGetIpab(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstIpab);

/* poc-settings get the entity sss-settings */
ZFUNC ZINT EaPoc_SetsEntGetSss(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstSss);

/* poc-settings get the entity id */
ZFUNC ZINT EaPoc_SetsEntGetId(ST_XML_ELEM *pstEnt, 
                ST_ZOS_USTR **ppstId);

/* poc-settings get the isb-settings incoming-session-barring */
ZFUNC ZINT EaPoc_SetsIsbGetIsb(ST_XML_ELEM *pstIsb, 
                ST_XML_ELEM **ppstIsb);

/* poc-settings get the incoming-session-barring active */
ZFUNC ZINT EaPoc_SetsIsbGetActive(ST_XML_ELEM *pstIsb, 
                ZBOOL *pbActive);

/* poc-settings get the am-settings answer-mode EN_EA_POC_SETS_AM_TYPE */
ZFUNC ZINT EaPoc_SetsAmGetAm(ST_XML_ELEM *pstAm, 
                ZUCHAR *pucAnswerMode);

/* poc-settings get the ipab-settings incoming-personal-alert-barring */
ZFUNC ZINT EaPoc_SetsIpabGetIpab(ST_XML_ELEM *pstIpab, 
                ST_XML_ELEM **ppstIpab);

/* poc-settings get the incoming-personal-alert-barring active */
ZFUNC ZINT EaPoc_SetsIpabGetActive(ST_XML_ELEM *pstIpab, 
                ZBOOL *pbActive);

/* poc-settings get the sss-settings simultaneous-sessions-support */
ZFUNC ZINT EaPoc_SetsSssGetSss(ST_XML_ELEM *pstSss, 
                ST_XML_ELEM **ppstSss);

/* poc-settings get the simultaneous-sessions-support active */
ZFUNC ZINT EaPoc_SetsSssGetActive(ST_XML_ELEM *pstSss, 
                ZBOOL *pbActive);

/* poc-settings set the poc-settings */
ZFUNC ZINT EaPoc_SetsSetSets(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstSets);

/* poc-settings set the entity */
ZFUNC ZINT EaPoc_SetsSetEnt(ST_XML_ELEM *pstSets, 
                ST_XML_ELEM **ppstEnt);

/* poc-settings set the entity isb-settings */
ZFUNC ZINT EaPoc_SetsEntSetIsb(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstIsb);

/* poc-settings set the entity am-settings */
ZFUNC ZINT EaPoc_SetsEntSetAm(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstAm);

/* poc-settings set the entity ipab-settings */
ZFUNC ZINT EaPoc_SetsEntSetIpab(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstIpab);

/* poc-settings set the entity sss-settings */
ZFUNC ZINT EaPoc_SetsEntSetSss(ST_XML_ELEM *pstEnt, 
                ST_XML_ELEM **ppstSss);

/* poc-settings set the entity id */
ZFUNC ZINT EaPoc_SetsEntSetId(ST_XML_ELEM *pstEnt, 
                ST_ZOS_USTR *pstId);

/* poc-settings set the isb-settings incoming-session-barring */
ZFUNC ZINT EaPoc_SetsIsbSetIsb(ST_XML_ELEM *pstIsb, 
                ST_XML_ELEM **ppstIsb);

/* poc-settings set the incoming-session-barring active */
ZFUNC ZINT EaPoc_SetsIsbSetActive(ST_XML_ELEM *pstIsb, 
                ZBOOL bActive);

/* poc-settings set the am-settings answer-mode EN_EA_POC_SETS_AM_TYPE */
ZFUNC ZINT EaPoc_SetsAmSetAm(ST_XML_ELEM *pstAm, 
                ZUCHAR ucAnswerMode);

/* poc-settings set the ipab-settings incoming-personal-alert-barring */
ZFUNC ZINT EaPoc_SetsIpabSetIpab(ST_XML_ELEM *pstIpab, 
                ST_XML_ELEM **ppstIpab);

/* poc-settings set the incoming-personal-alert-barring active */
ZFUNC ZINT EaPoc_SetsIpabSetActive(ST_XML_ELEM *pstIpab, 
                ZBOOL bActive);

/* poc-settings set the sss-settings simultaneous-sessions-support */
ZFUNC ZINT EaPoc_SetsSssSetSss(ST_XML_ELEM *pstSss, 
                ST_XML_ELEM **ppstSss);

/* poc-settings set the simultaneous-sessions-support active */
ZFUNC ZINT EaPoc_SetsSssSetActive(ST_XML_ELEM *ppstSss, 
                ZBOOL bActive);

#ifdef __cplusplus
}
#endif

#endif /* _EA_POC_SETS_H__ */

