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
  File name     : ea_prs_rules.h
  Module        : eax oma prs pres-rules
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax prs pres-rules.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PRS_RULES_H__
#define _EA_PRS_RULES_H__

#ifdef __cplusplus
extern "C"{
#endif

/* prs pres-rules provide-geopriv type */
typedef enum EN_EA_PRS_RULES_PVD_GEO_TYPE
{
    EN_EA_PRS_RULES_PVD_GEO_NULL,
    EN_EA_PRS_RULES_PVD_GEO_FALSE,
    EN_EA_PRS_RULES_PVD_GEO_FULL
} EN_EA_PRS_RULES_PVD_GEO_TYPE;

/* prs pres-rules get the provide-willingness */
ZFUNC ZINT EaPrs_RulesGetPvdWill(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* prs pres-rules get the provide-network-availability */
ZFUNC ZINT EaPrs_RulesGetPvdNetAvail(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* prs pres-rules get the provide-session-participation */
ZFUNC ZINT EaPrs_RulesGetPvdSessPartp(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* prs pres-rules get the provide-registration-state */
ZFUNC ZINT EaPrs_RulesGetPvdRegSta(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* prs pres-rules get the provide-barring-state */
ZFUNC ZINT EaPrs_RulesGetPvdBarrSta(ST_XML_ELEM *pstElem, 
                ZBOOL *pbPermission);

/* prs pres-rules get the provide-geopriv EN_EA_PRS_RULES_PVD_GEO_TYPE */
ZFUNC ZINT EaPrs_RulesGetPvdGeo(ST_XML_ELEM *pstElem, 
                ZUCHAR *pucGeo);

/* prs pres-rules get the service-id */
ZFUNC ZINT EaPrs_RulesGetSrvId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstSrvId);

/* prs pres-rules set the provide-willingness */
ZFUNC ZINT EaPrs_RulesSetPvdWill(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* prs pres-rules set the provide-network-availability */
ZFUNC ZINT EaPrs_RulesSetPvdNetAvail(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* prs pres-rules set the provide-session-participation */
ZFUNC ZINT EaPrs_RulesSetPvdSessPartp(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* prs pres-rules set the provide-registration-state */
ZFUNC ZINT EaPrs_RulesSetPvdRegSta(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* prs pres-rules set the provide-barring-state */
ZFUNC ZINT EaPrs_RulesSetPvdBarrSta(ST_XML_ELEM *pstElem, 
                ZBOOL bPermission);

/* prs pres-rules set the provide-geopriv EN_EA_PRS_RULES_PVD_GEO_TYPE */
ZFUNC ZINT EaPrs_RulesSetPvdGeo(ST_XML_ELEM *pstElem, 
                ZUCHAR ucGeo);

/* prs pres-rules set the service-id */
ZFUNC ZINT EaPrs_RulesSetSrvId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstSrvId);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PRS_RULES_H__ */

