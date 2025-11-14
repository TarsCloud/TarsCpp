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
  File name     : ea_rcs_geo.h
  Module        : eax rcs geo
  Author        : zocol
  Version       : 0.1
  Created on    : 2013-01-17
  Description   :
    Marcos and structure definitions required by the eax rcs geo.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_RCS_GEO_H__
#define _EA_RCS_GEO_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:gsma:params:xml:ns:rcs:rcs:geolocation **************/
typedef enum EN_EA_RCS_GEO_TYPE
{
    EN_EA_RCS_GEO_ID,                /* id */
    EN_EA_RCS_GEO_LABEL,             /* label */
    EN_EA_RCS_GEO_ENTITY,            /* entity */
    EN_EA_RCS_GEO_TIMESTAMP,         /* timestamp */
    EN_EA_RCS_GEO_RCSENV,            /* rcsenvelope */
    EN_EA_RCS_GEO_RCSPUSHLCTN        /* rcspushlocation */
} EN_EA_RCS_GEO_TYPE;

/* rcs geolocation get the rcsenvelope */
ZFUNC ZINT EaRcsGeo_GetEnv(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstEnv);

/* rcs geolocation get the rcsenvelope entity */
ZFUNC ZINT EaRcsGeo_EnvGetEntity(ST_XML_ELEM *pstEnv, 
                ST_ZOS_USTR **ppstEntity);

/* rcs geolocation get the rcspushlocation */
ZFUNC ZINT EaRcsGeo_EnvGetPushLctn(ST_XML_ELEM *pstEnv, 
                ST_XML_ELEM **ppstPushLctn);

/* rcs geolocation get the rcspushlocation id */
ZFUNC ZINT EaRcsGeo_PushLctnGetId(ST_XML_ELEM *pstPushLctn, 
                ST_ZOS_USTR **ppstId);

/* rcs geolocation get the rcspushlocation label */
ZFUNC ZINT EaRcsGeo_PushLctnGetLabel(ST_XML_ELEM *pstPushLctn, 
                ST_ZOS_USTR **ppstLabel);

/* rcs geolocation get the rcspushlocation timestamp */
ZFUNC ZINT EaRcsGeo_PushLctnGetTimestamp(ST_XML_ELEM *pstPushLctn, 
                ST_ZOS_USTR **ppstTimestamp);

/* rcs geolocation set the rcsenvelope */
ZFUNC ZINT EaRcsGeo_SetEnv(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstEnv);

/* rcs geolocation set the rcsenvelope entity */
ZFUNC ZINT EaRcsGeo_EnvSetEntity(ST_XML_ELEM *pstEnv, 
                ST_ZOS_USTR *pstEntity);

/* rcs geolocation set the rcspushlocation */
ZFUNC ZINT EaRcsGeo_EnvSetPushLctn(ST_XML_ELEM *pstEnv, 
                ST_XML_ELEM **ppstPushLctn);

/* rcs geolocation set the rcspushlocation id */
ZFUNC ZINT EaRcsGeo_PushLctnSetId(ST_XML_ELEM *pstPushLctn, 
                ST_ZOS_USTR *pstId);

/* rcs geolocation set the rcspushlocation label */
ZFUNC ZINT EaRcsGeo_PushLctnSetLabel(ST_XML_ELEM *pstPushLctn, 
                ST_ZOS_USTR *pstLabel);

/* rcs geolocation get the rcspushlocation timestamp */
ZFUNC ZINT EaRcsGeo_PushLctnSetTimestamp(ST_XML_ELEM *pstPushLctn, 
                ST_ZOS_USTR *pstTimestamp);

#ifdef __cplusplus
}
#endif

#endif /* _EA_RCS_GEO_H__ */

