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
  File name     : xc_rls_srvs.h
  Module        : eax rls-services
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax rls-services.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_RLST_SRV_H__
#define _EA_RLST_SRV_H__

#ifdef __cplusplus
extern "C"{
#endif

/********** urn:ietf:params:xml:ns:rls-services **********/
typedef enum EN_EA_RLS_SRV_TYPE
{
    EN_EA_RLS_SRV_RLS_SRVS,       /* rls-services */
    EN_EA_RLS_SRV_SRV,            /* service */
    EN_EA_RLS_SRV_RES_LST,        /* resource-list */
    EN_EA_RLS_SRV_LST,            /* list */
    EN_EA_RLS_SRV_PKGS,           /* packages */
    EN_EA_RLS_SRV_URI,            /* uri */
    EN_EA_RLS_SRV_PKG             /* package */
} EN_EA_RLS_SRV_TYPE;

/* rls-services get the rls-services */
ZFUNC ZINT EaRls_SrvsGetSrvs(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstSrvs);

/* rls-services rls-services get the service */
ZFUNC ZINT EaRls_SrvsSrvsGetSrv(ST_XML_ELEM *pstSrvs, 
                ST_ZOS_USTR *pstUri, ST_XML_ELEM **ppstSrv);

/* rls-services rls-services get the first service */
ZFUNC ZINT EaRls_SrvsSrvsGetFirstSrv(ST_XML_ELEM *pstSrvs, 
                ST_XML_ELEM **ppstSrv);

/* rls-services rls-services get the next service */
ZFUNC ZINT EaRls_SrvsSrvsGetNextSrv(ST_XML_ELEM *pstSrv, 
                ST_XML_ELEM **ppstSrv);

/* rls-services get the service resource-list */
ZFUNC ZINT EaRls_SrvsSrvGetResLst(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR **ppstResLst);

/* rls-services get the service list */
ZFUNC ZINT EaRls_SrvsSrvGetLst(ST_XML_ELEM *pstSrv, 
                ST_XML_ELEM **ppstLst);

/* rls-services get the service packages */
ZFUNC ZINT EaRls_SrvsSrvGetPkgs(ST_XML_ELEM *pstSrv, 
                ST_XML_ELEM **ppstPkgs);

/* rls-services get the service uri */
ZFUNC ZINT EaRls_SrvsSrvGetUri(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR **ppstUri);

/* rls-services get the packages package */
ZFUNC ZINT EaRls_SrvsPkgsGetFirstPkg(ST_XML_ELEM *pstPkgs, 
                ST_XML_ELEM **ppstPkg);

/* rls-services get the packages package */
ZFUNC ZINT EaRls_SrvsPkgsGetNextPkg(ST_XML_ELEM *pstPkg, 
                ST_XML_ELEM **ppstPkg);

/* rls-services set the rls-services */
ZFUNC ZINT EaRls_SrvsSetSrvs(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstSrvs);

/* rls-services rls-services set the service */
ZFUNC ZINT EaRls_SrvsSrvsSetSrv(ST_XML_ELEM *pstSrvs, 
                ST_XML_ELEM **ppstSrv);

/* rls-services set the service resource-list */
ZFUNC ZINT EaRls_SrvsSrvSetResLst(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR *pstResLst);

/* rls-services set the service list */
ZFUNC ZINT EaRls_SrvsSrvSetLst(ST_XML_ELEM *pstSrv, 
                ST_XML_ELEM **ppstLst);

/* rls-services set the service packages */
ZFUNC ZINT EaRls_SrvsSrvSetPkgs(ST_XML_ELEM *pstSrv, 
                ST_XML_ELEM **ppstPkgs);

/* rls-services set the service uri */
ZFUNC ZINT EaRls_SrvsSrvSetUri(ST_XML_ELEM *pstSrv, 
                ST_ZOS_USTR *pstUri);

/* rls-services set the packages package */
ZFUNC ZINT EaRls_SrvsPkgsSetPkg(ST_XML_ELEM *pstPkgs, 
                ST_XML_ELEM **ppstPkg);

#ifdef __cplusplus
}
#endif

#endif /* _EA_RLST_SRV_H__ */

