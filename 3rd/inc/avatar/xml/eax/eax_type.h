/************************************************************************

        Copyright (c) 2005-2009 by Juphoon System Software, Inc.
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
  File name     : eax_type.h
  Module        : extension api for xml
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-05-22
  Description   :
    Marcos and structure definitions required by the extension api for xml.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EAX_TYPE_H__
#define _EAX_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* eax maximum namesacpe number */
#define EAX_NS_MAX_NUM 12

/*************************** namesapace list ****************
 ************************** w3 namespaces *****************
 * http://www.w3.org/2001/XMLSchema
 * http://www.w3.org/2001/XMLSchema-instance
 *
 ************************* ietf namesacpes ******************
 * urn:ietf:params:xml:ns:common-policy 
 * urn:ietf:params:xml:ns:pres-rules
 * urn:ietf:params:xml:ns:resource-lists
 * urn:ietf:params:xml:ns:rls-services
 * urn:ietf:params:xml:ns:watcherinfo
 * urn:ietf:params:xml:ns:im-iscomposing
 * urn:ietf:params:xml:ns:rlmi
 * urn:ietf:params:xml:ns:pidf
 * urn:ietf:params:xml:ns:pidf:caps
 * urn:ietf:params:xml:ns:pidf:cipid
 * urn:ietf:params:xml:ns:pidf-diff
 * urn:ietf:params:xml:ns:pidf:data-model
 * urn:ietf:params:xml:ns:pidf:rpid
 * urn:ietf:params:xml:ns:pidf:timed-status
 * urn:ietf:params:xml:ns:pidf:im
 * urn:ietf:params:xml:ns:xcap-diff
 * urn:ietf:params:xml:ns:xcap-directory
 * urn:ietf:params:xml:ns:xcap-error
 * urn:ietf:params:xml:ns:xcap-must-understand
 * urn:ietf:params:xml:ns:location-type
 * urn:ietf:params:xml:ns:dialog-info
 * urn:ietf:params:xml:schema:xml-patch-ops
 * urn:ietf:params:xml:ns:reginfo
 *
 ************************** oma namesacpes *****************
 * urn:oma:xml:poc:list-service
 * urn:oma:xml:poc:poc-rules
 * urn:oma:params:xml:ns:poc:poc-settings
 * urn:oma:xml:prs:pidf:oma-pres
 * urn:oma:xml:prs:pres-rules
 * urn:oma:xml:xdm:resource-list:oma-appusage
 * urn:oma:xml:xdm:common-policy
 * urn:oma:xml:xdm:extensions
 * urn:oma:xml:xdm:resource-list:oma-uriusage
 * urn:oma:xml:xdm:xcap-directory
 * urn:oma:xml:xdm:xcap-error
 * urn:oma:xml:im:service-settings
 * urn:oma:xml:rest:netapi:terminallocation:1
 *
 ************************** gsma namesacpes ****************
 * urn:gsma:params:xml:ns:rcs:rcs:geolocation
 *
 ************************** opengis namesacpes ****************
 * http://www.opengis.net/gml
 * http://www.opengis.net/pidflo/1.0
 *
************************************************************/
/* eax namespace */
typedef enum EN_EAX_NS_TYPE
{
    /* w3 namespaces */
    EN_EAX_NS_W3_2001_XSC = 0,
    EN_EAX_NS_W3_2001_XSC_INST,
    
    /* ietf namespaces */
    EN_EAX_NS_IETF_COMM_PLY,
    EN_EAX_NS_IETF_PRES_RULE,
    EN_EAX_NS_IETF_RES_LST,
    EN_EAX_NS_IETF_RLS_SRV = 5,
    EN_EAX_NS_IETF_WATINFO,
    EN_EAX_NS_IETF_IM_ISC,
    EN_EAX_NS_IETF_RLMI,
    EN_EAX_NS_IETF_PIDF,
    EN_EAX_NS_IETF_PIDF_CAPS = 10,
    EN_EAX_NS_IETF_PIDF_CIPID,
    EN_EAX_NS_IETF_PIDF_DIFF,
    EN_EAX_NS_IETF_PIDF_DM,
    EN_EAX_NS_IETF_PIDF_RPID,
    EN_EAX_NS_IETF_PIDF_TS = 15,
    EN_EAX_NS_IETF_PIDF_IM,
    EN_EAX_NS_IETF_XCAP_DIFF,
    EN_EAX_NS_IETF_XCAP_DIR,
    EN_EAX_NS_IETF_XCAP_ERR,
    EN_EAX_NS_IETF_XCAP_MU = 20,
    EN_EAX_NS_IETF_LOCTN_TP,
    EN_EAX_NS_IETF_DLG_INFO,
    EN_EAX_NS_IETF_PATCH_OPS,
    EN_EAX_NS_IETF_CONF_INFO,
    EN_EAX_NS_IETF_GEO_BASIC = 25,
    EN_EAX_NS_IETF_GEO_CIVIC,
    EN_EAX_NS_IETF_GEO_PRIV10,
    EN_EAX_NS_IETF_REG_INFO,
    EN_EAX_NS_IETF_IMDN,
    EN_EAX_NS_IETF_CPY_CTRL = 30,
    EN_EAX_NS_IETF_MSG_INFO,

    /* oma namesapces */
    EN_EAX_NS_OMA_POC_LST_SRV,
    EN_EAX_NS_OMA_POC_RULE,
    EN_EAX_NS_OMA_POC_SET,
    EN_EAX_NS_OMA_PRS_PRES = 35,
    EN_EAX_NS_OMA_PRS_RULES,
    EN_EAX_NS_OMA_XDM_AUSG,
    EN_EAX_NS_OMA_XDM_CPLY,
    EN_EAX_NS_OMA_XDM_EXT,
    EN_EAX_NS_OMA_XDM_UUSG = 40,
    EN_EAX_NS_OMA_XDM_XDIR,
    EN_EAX_NS_OMA_XDM_XERR, 
    EN_EAX_NS_OMA_IM_SRV_SET,
    EN_EAX_NS_OMA_PRS_CTT,
    EN_EAX_NS_OMA_IM_HIS_LST = 45,
    EN_EAX_NS_OMA_PIDF_EXT_1_0,
    EN_EAX_NS_OMA_PIDF_EXT_1_1,
    EN_EAX_NS_OMA_REST_TERM_1,
    EN_EAX_NS_OMA_CAB_PCC,
    EN_EAX_NS_OMA_TDC_CARD = 50,
    EN_EAX_NS_IDIAL_EXT_PRES,
            
    /* gsma namesapces */
    EN_EAX_NS_GSMA_RCS_GEO,
    EN_EAX_NS_GSMA_CALLDATA,
    EN_EAX_NS_GSMA_SHAREDDATA,
    EN_EAX_NS_GSMA_GROUPDATA,
    EN_EAX_NS_GSMA_RCS_FTHTTP,
    EN_EAX_NS_GSMA_RCS_RRAM,
    
    /* opengis namesapces */
    EN_EAX_NS_GIS_GML,
    EN_EAX_NS_GIS_PIDFLO_1_0,

    /* extern gourp namesapces */
    EN_EAX_NS_PUBLIC_GROUP,
    EN_EAX_NS_PUBLIC_GROUP_LIST,
        
    /* other namespace */
    EN_EAX_NS_UNKNOWN
} EN_EAX_NS_TYPE;

/* eax namespace id */
typedef struct tagEAX_NS_ID
{
    ZUINT iNsId;                     /* namespace id EN_EAX_NS_TYPE */
    ZUINT iExtnNsId;                 /* extension namespace id EN_EAX_NS_TYPE */
} ST_EAX_NS_ID;

/* eax namespace attribute */
typedef struct tagEAX_NS_ATTR
{
    ST_EAX_NS_ID stNsId;             /* namespace id */
    ST_ZOS_USTR stPrefix;            /* prefix name */
} ST_EAX_NS_ATTR;

/* eax namesapce */
typedef struct tagEAX_NS
{
    ST_EAX_NS_ID stDftNsId;          /* default namespace id */
    ZUINT iNsAttrNum;                /* namesacpe attributes number */
    ST_EAX_NS_ATTR astNsAttr[EAX_NS_MAX_NUM]; /* namespace attributes */
} ST_EAX_NS;

/* eax message */
typedef struct tagEAX_MSG
{
    ST_EAX_NS stXmlNs;               /* xml namespace */
    ST_XML_MSG *pstXmlMsg;           /* xml message */
} ST_EAX_MSG;

#ifdef __cplusplus
}
#endif

#endif /* _EAX_TYPE_H__ */

