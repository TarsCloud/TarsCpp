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
  File name     : ea_reg_info.h
  Module        : eax reg-info
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax reg-info.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_REG_INFO_H__
#define _EA_REG_INFO_H__

#ifdef __cplusplus
extern "C"{
#endif

/* reginfo state type */
typedef enum EN_EA_REG_INFO_STA_TYPE
{
    EN_EA_REG_INFO_STA_FULL,        
    EN_EA_REG_INFO_STA_PARTIAL
} EN_EA_REG_INFO_STA_TYPE;

/* reginfo registration state type */
typedef enum EN_EA_REG_INFO_REG_STA_TYPE
{
    EN_EA_REG_INFO_REG_STA_INIT,        
    EN_EA_REG_INFO_REG_STA_ACTIVE,
    EN_EA_REG_INFO_REG_STA_TERM
} EN_EA_REG_INFO_REG_STA_TYPE;

/* reginfo contact state type */
typedef enum EN_EA_REG_INFO_CONTACT_STA_TYPE
{
    EN_EA_REG_INFO_CONTACT_STA_ACTIVE,
    EN_EA_REG_INFO_CONTACT_STA_TERM
} EN_EA_REG_INFO_CONTACT_STA_TYPE;

/* reginfo contact event type */
typedef enum EN_EA_REG_INFO_EVNT_TYPE
{
    EN_EA_REG_INFO_EVNT_REGED,
    EN_EA_REG_INFO_EVNT_CREATED,
    EN_EA_REG_INFO_EVNT_REFRESHED,
    EN_EA_REG_INFO_EVNT_SHORTENED,    
    EN_EA_REG_INFO_EVNT_EXPIRED,    
    EN_EA_REG_INFO_EVNT_DEACTED,    
    EN_EA_REG_INFO_EVNT_PROBATION,    
    EN_EA_REG_INFO_EVNT_UNREGED,    
    EN_EA_REG_INFO_EVNT_REJED 
} EN_EA_REG_INFO_EVNT_TYPE;

/* reginfo get the reginfo */
ZFUNC ZINT EaReg_InfoGetInfo(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstInfo);

/* reginfo get the first reginfo registration */
ZFUNC ZINT EaReg_InfoInfoGetFirstReg(ST_XML_ELEM *pstInfo, 
                ST_XML_ELEM **ppstReg);

/* reginfo get the next reginfo registration */
ZFUNC ZINT EaReg_InfoInfoGetNextReg(ST_XML_ELEM *pstReg, 
                ST_XML_ELEM **ppstReg);

/* reginfo get the reginfo version */
ZFUNC ZINT EaReg_InfoInfoGetVer(ST_XML_ELEM *pstInfo, ZUINT *piVer);

/* reginfo get the reginfo state EN_EA_REG_INFO_STA_TYPE */
ZFUNC ZINT EaReg_InfoInfoGetSta(ST_XML_ELEM *pstInfo, ZUCHAR *pucSta);

/* reginfo get the first registration contact */
ZFUNC ZINT EaReg_InfoRegGetFirstContact(ST_XML_ELEM *pstReg, 
                ST_XML_ELEM **ppstContact);

/* reginfo get the next registration contact */
ZFUNC ZINT EaReg_InfoRegGetNextContact(ST_XML_ELEM *pstContact, 
                ST_XML_ELEM **ppstContact);

/* reginfo get the registration aor */
ZFUNC ZINT EaReg_InfoRegGetAor(ST_XML_ELEM *pstReg, ST_ZOS_USTR **ppstAor);

/* reginfo get the registration id */
ZFUNC ZINT EaReg_InfoRegGetId(ST_XML_ELEM *pstReg, ST_ZOS_USTR **ppstId);

/* reginfo get the registration state EN_EA_REG_INFO_REG_STA_TYPE */
ZFUNC ZINT EaReg_InfoRegGetSta(ST_XML_ELEM *pstReg, ZUCHAR *pucSta);

/* reginfo get the contact uri */
ZFUNC ZINT EaReg_InfoContactGetUri(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR **ppstUri);

/* reginfo get the contact display-name */
ZFUNC ZINT EaReg_InfoContactGetDispName(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR **ppstDispName);

/* reginfo get the contact state EN_EA_REG_INFO_CONTACT_STA_TYPE */
ZFUNC ZINT EaReg_InfoContactGetSta(ST_XML_ELEM *pstContact, ZUCHAR *pucSta);

/* reginfo get the contact event EN_EA_REG_INFO_EVNT_TYPE */
ZFUNC ZINT EaReg_InfoContactGetEvnt(ST_XML_ELEM *pstContact, 
                ZUCHAR *pucEvnt);

/* reginfo get the contact duration-registered */
ZFUNC ZINT EaReg_InfoContactGetDuration(ST_XML_ELEM *pstContact, 
                ZUINT *piDuration);

/* reginfo get the contact expires */
ZFUNC ZINT EaReg_InfoContactGetExpires(ST_XML_ELEM *pstContact, 
                ZUINT *piExpires);

/* reginfo get the contact retry-after */
ZFUNC ZINT EaReg_InfoContactGetRetryAfter(ST_XML_ELEM *pstContact, 
                ZUINT *piRetryAfter);

/* reginfo get the contact id */
ZFUNC ZINT EaReg_InfoContactGetId(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR **ppstId);

/* reginfo get the contact q */
ZFUNC ZINT EaReg_InfoContactGetQ(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR **ppstQ);

/* reginfo get the contact callid */
ZFUNC ZINT EaReg_InfoContactGetCallId(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR **ppstCallId);

/* reginfo get the contact cseq */
ZFUNC ZINT EaReg_InfoContactGetCseq(ST_XML_ELEM *pstContact, 
                ZUINT *piCseq);

/* reginfo set the reginfo */
ZFUNC ZINT EaReg_InfoSetInfo(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstInfo);

/* reginfo set the reginfo registration */
ZFUNC ZINT EaReg_InfoInfoSetReg(ST_XML_ELEM *pstInfo, 
                ST_XML_ELEM **ppstReg);

/* reginfo set the reginfo version */
ZFUNC ZINT EaReg_InfoInfoSetVer(ST_XML_ELEM *pstInfo, ZUINT iVer);

/* reginfo set the reginfo state EN_EA_REG_INFO_STA_TYPE */
ZFUNC ZINT EaReg_InfoInfoSetSta(ST_XML_ELEM *pstInfo, ZUCHAR ucSta);

/* reginfo set the registration contact */
ZFUNC ZINT EaReg_InfoRegSetContact(ST_XML_ELEM *pstReg, 
                ST_XML_ELEM **ppstContact);

/* reginfo set the registration aor */
ZFUNC ZINT EaReg_InfoRegSetAor(ST_XML_ELEM *pstReg, ST_ZOS_USTR *pstAor);

/* reginfo set the registration id */
ZFUNC ZINT EaReg_InfoRegSetId(ST_XML_ELEM *pstReg, ST_ZOS_USTR *pstId);

/* reginfo set the registration state EN_EA_REG_INFO_REG_STA_TYPE */
ZFUNC ZINT EaReg_InfoRegSetSta(ST_XML_ELEM *pstReg, ZUCHAR ucSta);

/* reginfo set the contact uri */
ZFUNC ZINT EaReg_InfoContactSetUri(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR *pstUri);

/* reginfo set the contact display-name */
ZFUNC ZINT EaReg_InfoContactSetDispName(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR *pstDispName);

/* reginfo set the contact state EN_EA_REG_INFO_CONTACT_STA_TYPE */
ZFUNC ZINT EaReg_InfoContactSetSta(ST_XML_ELEM *pstContact, ZUCHAR ucSta);

/* reginfo set the contact event EN_EA_REG_INFO_EVNT_TYPE */
ZFUNC ZINT EaReg_InfoContactSetEvnt(ST_XML_ELEM *pstContact, ZUCHAR ucEvnt);

/* reginfo set the contact duration-registered */
ZFUNC ZINT EaReg_InfoContactSetDuration(ST_XML_ELEM *pstContact, 
                ZUINT iDuration);

/* reginfo set the contact expires */
ZFUNC ZINT EaReg_InfoContactSetExpires(ST_XML_ELEM *pstContact, 
                ZUINT iExpires);

/* reginfo set the contact retry-after */
ZFUNC ZINT EaReg_InfoContactSetRetryAfter(ST_XML_ELEM *pstContact, 
                ZUINT iRetryAfter);

/* reginfo set the contact id */
ZFUNC ZINT EaReg_InfoContactSetId(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR *pstId);

/* reginfo set the contact q */
ZFUNC ZINT EaReg_InfoContactSetQ(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR *pstQ);

/* reginfo set the contact callid */
ZFUNC ZINT EaReg_InfoContactSetCallId(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR *pstCallId);

/* reginfo set the contact cseq */
ZFUNC ZINT EaReg_InfoContactSetCseq(ST_XML_ELEM *pstContact, 
                ZUINT iCseq);

/* reginfo get the first contact unknown-param */
ZFUNC ZINT EaReg_InfoRegGetFirstUnkwnParam(ST_XML_ELEM *pstContact, 
                ST_XML_ELEM **ppstUnkwnParam);

/* reginfo get the next contact unknown-param */
ZFUNC ZINT EaReg_InfoRegGetNextUnkwnParam(ST_XML_ELEM *pstUnkwnParam, 
                ST_XML_ELEM **ppstUnkwnParam);

/* reginfo get the unknown-param sip-instance */
ZFUNC ZINT EaReg_InfoUnkwnParamGetSipInst(ST_XML_ELEM *pstUnkwnParam, 
                ST_ZOS_USTR **ppstSipInst);

/* reginfo get the unknown-param reg-id */
ZFUNC ZINT EaReg_InfoUnkwnParamGetRegId(ST_XML_ELEM *pstUnkwnParam, 
                ZUINT *piRegId);

#ifdef __cplusplus
}
#endif

#endif /* _EA_REG_INFO_H__ */

