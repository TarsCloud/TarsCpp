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
  File name     : ea_comm_ply.h
  Module        : eax common-policy
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax common-policy.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_COMM_PLY_H__
#define _EA_COMM_PLY_H__

#ifdef __cplusplus
extern "C"{
#endif

/********** urn:ietf:params:xml:ns:common-policy *********/
typedef enum EN_EA_COMM_PLY_TYPE
{
    EN_EA_COMM_PLY_RULESET,       /* ruleset */
    EN_EA_COMM_PLY_RULE,          /* rule */
    EN_EA_COMM_PLY_CONDS,         /* conditions */
    EN_EA_COMM_PLY_ACTS,          /* actions */
    EN_EA_COMM_PLY_TXNFMS,        /* transformations */
    EN_EA_COMM_PLY_ID,            /* id */
    EN_EA_COMM_PLY_IDENT,         /* identity */
    EN_EA_COMM_PLY_SPHERE,        /* sphere */
    EN_EA_COMM_PLY_VALID,         /* validity */
    EN_EA_COMM_PLY_ONE,           /* one */
    EN_EA_COMM_PLY_MANY,          /* many */
    EN_EA_COMM_PLY_EXCEPT,        /* except */
    EN_EA_COMM_PLY_DOMAIN,        /* domain */
    EN_EA_COMM_PLY_VAL,           /* value */
    EN_EA_COMM_PLY_FROM,          /* from */
    EN_EA_COMM_PLY_UNTIL          /* until */
} EN_EA_COMM_PLY_TYPE;

/* common-policy get the ruleset */
ZFUNC ZINT EaComm_PlyGetRules(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstRules);

/* common-policy get the ruleset rule */
ZFUNC ZINT EaComm_PlyRulesGetRule(ST_XML_ELEM *pstRules, 
                ST_ZOS_USTR *pstId, ST_XML_ELEM **ppstRule);

/* common-policy get the first ruleset rule */
ZFUNC ZINT EaComm_PlyRulesGetFirstRule(ST_XML_ELEM *pstRules, 
                ST_XML_ELEM **ppstRule);

/* common-policy get the next ruleset rule */
ZFUNC ZINT EaComm_PlyRulesGetNextRule(ST_XML_ELEM *pstRule, 
                ST_XML_ELEM **ppstRule);

/* common-policy get the rule conditions */
ZFUNC ZINT EaComm_PlyRuleGetCond(ST_XML_ELEM *pstRule, 
                ST_XML_ELEM **ppstCond);

/* common-policy get the rule actions */
ZFUNC ZINT EaComm_PlyRuleGetAction(ST_XML_ELEM *pstRule, 
                ST_XML_ELEM **ppstAction);

/* common-policy get the rule transformations */
ZFUNC ZINT EaComm_PlyRuleGetTrsf(ST_XML_ELEM *pstRule, 
                ST_XML_ELEM **ppstTrsf);

/* common-policy get the rule id */
ZFUNC ZINT EaComm_PlyRuleGetId(ST_XML_ELEM *pstRule, 
                ST_ZOS_USTR **ppstId);

/* common-policy get the conditions identity */
ZFUNC ZINT EaComm_PlyCondGetIdent(ST_XML_ELEM *pstCond, 
                ST_XML_ELEM **ppstIdent);

/* common-policy get the conditions sphere */
ZFUNC ZINT EaComm_PlyCondGetSphere(ST_XML_ELEM *pstCond, 
                ST_XML_ELEM **ppstSphere);

/* common-policy get the conditions validity */
ZFUNC ZINT EaComm_PlyCondGetValid(ST_XML_ELEM *pstCond,  
                ST_XML_ELEM **ppstValid);

/* common-policy get the identity one */
ZFUNC ZINT EaComm_PlyIdentGetOne(ST_XML_ELEM *pstIdent, 
                ST_ZOS_USTR *pstId, ST_XML_ELEM **ppstOne);

/* common-policy get the identity first one */
ZFUNC ZINT EaComm_PlyIdentGetFirstOne(ST_XML_ELEM *pstIdent, 
                ST_XML_ELEM **ppstOne);

/* common-policy get the identity next one */
ZFUNC ZINT EaComm_PlyIdentGetNextOne(ST_XML_ELEM *pstOne, 
                ST_XML_ELEM **ppstOne);

/* common-policy get the identity first many */
ZFUNC ZINT EaComm_PlyIdentGetFirstMany(ST_XML_ELEM *pstIdent, 
                ST_XML_ELEM **ppstMany);

/* common-policy get the identity next many */
ZFUNC ZINT EaComm_PlyIdentGetNextMany(ST_XML_ELEM *pstMany, 
                ST_XML_ELEM **ppstMany);

/* common-policy get the one id */
ZFUNC ZINT EaComm_PlyOneGetId(ST_XML_ELEM *pstOne, 
                ST_ZOS_USTR **ppstId);

/* common-policy get the many first except */
ZFUNC ZINT EaComm_PlyManyGetFirstExcept(ST_XML_ELEM *pstMany, 
                ST_XML_ELEM **ppstExcept);

/* common-policy get the many next except */
ZFUNC ZINT EaComm_PlyManyGetNextExcept(ST_XML_ELEM *pstExcept, 
                ST_XML_ELEM **ppstExcept);

/* common-policy get the many domain */
ZFUNC ZINT EaComm_PlyManyGetDomain(ST_XML_ELEM *pstOne, 
                ST_ZOS_USTR **ppstDomain);

/* common-policy get the except domain */
ZFUNC ZINT EaComm_PlyExceptGetDomain(ST_XML_ELEM *pstExcept, 
                ST_ZOS_USTR **ppstDomain);

/* common-policy get the except id */
ZFUNC ZINT EaComm_PlyExceptGetId(ST_XML_ELEM *pstExcept, 
                ST_ZOS_USTR **ppstId);

/* common-policy get the sphere value */
ZFUNC ZINT EaComm_PlySphereGetVal(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR **ppstVal);

/* common-policy get the validity first from */
ZFUNC ZINT EaComm_PlyValidGetFirstFrom(ST_XML_ELEM *pstValid, 
                ST_XML_ELEM **ppstFrom);

/* common-policy get the validity next from */
ZFUNC ZINT EaComm_PlyValidGetNextFrom(ST_XML_ELEM *pstFrom, 
                ST_XML_ELEM **ppstFrom);

/* common-policy get the validity next until */
ZFUNC ZINT EaComm_PlyValidGetUntil(ST_XML_ELEM *pstFrom, 
                ST_XML_ELEM **ppstUntil);

/* common-policy set the ruleset */
ZFUNC ZINT EaComm_PlySetRules(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstRules);

/* common-policy set the ruleset rule */
ZFUNC ZINT EaComm_PlyRulesSetRule(ST_XML_ELEM *pstRules, 
                ST_XML_ELEM **ppstRule);

/* common-policy set the rule conditions */
ZFUNC ZINT EaComm_PlyRuleSetCond(ST_XML_ELEM *pstRule, 
                ST_XML_ELEM **ppstCond);

/* common-policy set the rule actions */
ZFUNC ZINT EaComm_PlyRuleSetAction(ST_XML_ELEM *pstRule, 
                ST_XML_ELEM **ppstAction);

/* common-policy set the rule transformations */
ZFUNC ZINT EaComm_PlyRuleSetTrsf(ST_XML_ELEM *pstRule, 
                ST_XML_ELEM **ppstTrsf);

/* common-policy set the rule id */
ZFUNC ZINT EaComm_PlyRuleSetId(ST_XML_ELEM *pstRule, 
                ST_ZOS_USTR *pstId);

/* common-policy set the conditions identity */
ZFUNC ZINT EaComm_PlyCondSetIdent(ST_XML_ELEM *pstCond, 
                ST_XML_ELEM **ppstIdent);

/* common-policy set the conditions sphere */
ZFUNC ZINT EaComm_PlyCondSetSphere(ST_XML_ELEM *pstCond, 
                ST_XML_ELEM **ppstSphere);

/* common-policy set the conditions validity */
ZFUNC ZINT EaComm_PlyCondSetValid(ST_XML_ELEM *pstCond,  
                ST_XML_ELEM **ppstValid);

/* common-policy set the identity one */
ZFUNC ZINT EaComm_PlyIdentSetOne(ST_XML_ELEM *pstIdent, 
                ST_XML_ELEM **ppstOne);

/* common-policy set the identity many */
ZFUNC ZINT EaComm_PlyIdentSetMany(ST_XML_ELEM *pstIdent, 
                ST_XML_ELEM **ppstMany);

/* common-policy set the one id */
ZFUNC ZINT EaComm_PlyOneSetId(ST_XML_ELEM *pstOne, 
                ST_ZOS_USTR *pstId);

/* common-policy set the many except */
ZFUNC ZINT EaComm_PlyManySetExcept(ST_XML_ELEM *pstMany, 
                ST_XML_ELEM **ppstExcept);

/* common-policy set the many domain */
ZFUNC ZINT EaComm_PlyManySetDomain(ST_XML_ELEM *pstOne, 
                ST_ZOS_USTR *pstDomain);

/* common-policy set the except domain */
ZFUNC ZINT EaComm_PlyExceptSetDomain(ST_XML_ELEM *pstExcept, 
                ST_ZOS_USTR *pstDomain);

/* common-policy set the except id */
ZFUNC ZINT EaComm_PlyExceptSetId(ST_XML_ELEM *pstExcept, 
                ST_ZOS_USTR *pstId);

/* common-policy set the sphere value */
ZFUNC ZINT EaComm_PlySphereSetVal(ST_XML_ELEM *pstSphere, 
                ST_ZOS_USTR *pstVal);

/* common-policy set the validity from */
ZFUNC ZINT EaComm_PlyValidSetFrom(ST_XML_ELEM *pstValid, 
                ST_XML_ELEM **ppstFrom);

/* common-policy set the validity next until */
ZFUNC ZINT EaComm_PlyValidSetUntil(ST_XML_ELEM *pstValid, 
                ST_XML_ELEM **ppstUntil);

#ifdef __cplusplus
}
#endif

#endif /* _EA_COMM_PLY_H__ */

