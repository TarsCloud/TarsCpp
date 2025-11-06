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
  File name     : ea_im_ss.h
  Module        : eax im service settings
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax im service settings.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_IM_SS_H__
#define _EA_IM_SS_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:oma:xml:im:service-settings **************/
typedef enum EN_EA_OMA_IM_SS_TYPE
{
    EN_EA_OMA_IM_SS_SRV_ID,            /* service-id */
    EN_EA_OMA_IM_SS_ACTIVE,            /* active */
    EN_EA_OMA_IM_SS_DEFD_SETS,         /* deferred-settings */
    EN_EA_OMA_IM_SS_OFFLN_DLIV,        /* offline-delivery */
    EN_EA_OMA_IM_SS_VIS_SETS,          /* vis-settings */
    EN_EA_OMA_IM_SS_VIS_STA,           /* vis-status */
    EN_EA_OMA_IM_SS_HIST_SETS,         /* hist-settings */
    EN_EA_OMA_IM_SS_HIST_ACTN,         /* hist-activation */
    EN_EA_OMA_IM_SS_SESS_MODE,         /* session-mode */
    EN_EA_OMA_IM_SS_PAGE_MODE          /* pager-large-mode */
} EN_EA_OMA_IM_SS_TYPE;

/* im service settings get service id */
ZFUNC ZINT EaIm_SsGetSrvId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstId);

/* im service settings get active */
ZFUNC ZINT EaIm_SsGetAvtive(ST_XML_ELEM *pstElem, 
                ZBOOL *pbActive);

/* im service settings get deferred settings */
ZFUNC ZINT EaIm_SsGetDefdSets(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSets);

/* im service settings get offline delivery */
ZFUNC ZINT EaIm_SsDefdSetsGetOfflnDliv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstOfflnDliv);

/* im service settings get vis settings */
ZFUNC ZINT EaIm_SsGetVisSets(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSets);

/* im service settings get vis status */
ZFUNC ZINT EaIm_SsVisSetsGetSta(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSta);

/* im service settings get hist settings */
ZFUNC ZINT EaIm_SsGetHistSets(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSets);

/* im service settings hist settings get hist activation */
ZFUNC ZINT EaIm_SsHistSetsGetFirstActn(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstActn);

/* im service settings hist settings get hist activation */
ZFUNC ZINT EaIm_SsHistSetsGetNextActn(ST_XML_ELEM *pstActn, 
                ST_XML_ELEM **ppstActn);

/* im service settings hist activation get type */
ZFUNC ZINT EaIm_SsHistActnGetType(ST_XML_ELEM *pstElem, 
                ZUINT *piType);

/* im service settings set service id */
ZFUNC ZINT EaIm_SsSetSrvId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstId);

/* im service settings set active */
ZFUNC ZINT EaIm_SsSetAvtive(ST_XML_ELEM *pstElem, 
                ZBOOL bActive);

/* im service settings set deferred settings */
ZFUNC ZINT EaIm_SsSetDefdSets(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSets);

/* im service settings set offline delivery */
ZFUNC ZINT EaIm_SsDefdSetsSetOfflnDliv(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstOfflnDliv);

/* im service settings set vis settings */
ZFUNC ZINT EaIm_SsSetVisSets(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSets);

/* im service settings set vis status */
ZFUNC ZINT EaIm_SsVisSetsSetSta(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSta);

/* im service settings set hist settings */
ZFUNC ZINT EaIm_SsSetHistSets(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSets);

/* im service settings hist settings set hist activation */
ZFUNC ZINT EaIm_SsHistSetsSetActn(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstActn);

/* im service settings hist activation set type */
ZFUNC ZINT EaIm_SsHistActnSetType(ST_XML_ELEM *pstElem, 
                ZUINT iType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_IM_SS_H__ */

