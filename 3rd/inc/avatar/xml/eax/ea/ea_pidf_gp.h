/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : ea_pidf_gp.h
  Module        : eax pidf gp
  Author        : zocol
  Version       : 0.1
  Created on    : 2013-01-17
  Description   :
    Marcos and structure definitions required by the eax pidf gp.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_GP_H__
#define _EA_PIDF_GP_H__

#ifdef __cplusplus
extern "C"{
#endif

/************ urn:ietf:params:xml:ns:pidf:geopriv10 **********/
typedef enum EN_EA_PIDF_GP_TYPE
{
    EN_EA_PIDF_GP_METHOD,           /* method */
    EN_EA_PIDF_GP_GEOPRIV,          /* geopriv */
    EN_EA_PIDF_GP_LCTN_INFO,        /* location-info */
    EN_EA_PIDF_GP_USAGE_RULES,      /* usage-rules */
    EN_EA_PIDF_GP_PROVIDED_BY,      /* provided-by */
    EN_EA_PIDF_GP_RETRANS_ALLOW,    /* retransmission-allowed */
    EN_EA_PIDF_GP_RETENT_EXPIRY,    /* retention-expiry */
    EN_EA_PIDF_GP_EXT_RULESET,      /* external-ruleset */
    EN_EA_PIDF_GP_NOTE_WELL         /* note-well */
} EN_EA_PIDF_GP_TYPE;

/* pidf gp get the first geopriv */
ZFUNC ZINT EaPidf_GpGetFirstGeopriv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstGeopriv);

/* pidf gp get the next geopriv */
ZFUNC ZINT EaPidf_GpGetNextGeopriv(ST_XML_ELEM *pstGeopriv, 
                ST_XML_ELEM **ppstGeopriv);

/* pidf gp get the geopriv location-info */
ZFUNC ZINT EaPidf_GpGeoprivGetLctnInfo(ST_XML_ELEM *pstGeopriv, 
                ST_XML_ELEM **ppstLctnInfo);

/* pidf gp get the geopriv usage-rules */
ZFUNC ZINT EaPidf_GpGeoprivGetUsageRules(ST_XML_ELEM *pstLctnInfo, 
                ST_XML_ELEM **ppstUsageRules);

/* pidf gp get the geopriv method */
ZFUNC ZINT EaPidf_GpGeoprivGetMethod(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR **ppstMethod);

/* pidf gp get the usage-rules retransmission-allowed */
ZFUNC ZINT EaPidf_GpUsageRulesGetRetransAllow(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR **ppstRetransAllow);

/* pidf gp get the usage-rules retention-expiry */
ZFUNC ZINT EaPidf_GpUsageRulesGetRetentExpiry(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR **ppstRetentExpiry);

/* pidf gp get the usage-rules external-ruleset */
ZFUNC ZINT EaPidf_GpUsageRulesGetExtRuleset(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR **ppstExtRuleset);

/* pidf gp get the usage-rules note-well */
ZFUNC ZINT EaPidf_GpUsageRulesGetNoteWell(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR **ppstNoteWell);

/* pidf gp set the geopriv */
ZFUNC ZINT EaPidf_GpSetGeopriv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstGeopriv);

/* pidf gp set the geopriv location-info */
ZFUNC ZINT EaPidf_GpGeoprivSetLctnInfo(ST_XML_ELEM *pstGeopriv, 
                ST_XML_ELEM **ppstLctnInfo);

/* pidf gp set the geopriv usage-rules */
ZFUNC ZINT EaPidf_GpGeoprivSetUsageRules(ST_XML_ELEM *pstLctnInfo, 
                ST_XML_ELEM **ppstUsageRules);

/* pidf gp set the geopriv method */
ZFUNC ZINT EaPidf_GpGeoprivSetMethod(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR *pstMethod);

/* pidf gp set the usage-rules retransmission-allowed */
ZFUNC ZINT EaPidf_GpUsageRulesSetRetransAllow(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR *pstRetransAllow);

/* pidf gp set the usage-rules retention-expiry */
ZFUNC ZINT EaPidf_GpUsageRulesSetRetentExpiry(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR *pstRetentExpiry);

/* pidf gp set the usage-rules external-ruleset */
ZFUNC ZINT EaPidf_GpUsageRulesSetExtRuleset(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR *pstExtRuleset);

/* pidf gp set the usage-rules note-well */
ZFUNC ZINT EaPidf_GpUsageRulesSetNoteWell(ST_XML_ELEM *pstUsageRules, 
                ST_ZOS_USTR *pstNoteWell);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_GP_H__ */

