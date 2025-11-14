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
  File name     : ea_xdm_xdir.h
  Module        : eax xdm xcap-directory
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax xdm xcap-directory.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XDM_XDIR_H__
#define _EA_XDM_XDIR_H__

#ifdef __cplusplus
extern "C"{
#endif

/************ urn:oma:xml:xdm:xcap-directory ************/
typedef enum EN_EA_OMA_XDM_XDIR_TYPE
{
    EN_EA_OMA_XDM_XDIR_XCAP_DIR,       /* xcap-directory */
    EN_EA_OMA_XDM_XDIR_FOLDER,         /* folder */
    EN_EA_OMA_XDM_XDIR_ENTRY,          /* entry */
    EN_EA_OMA_XDM_XDIR_URI,            /* uri */
    EN_EA_OMA_XDM_XDIR_ETAG,           /* etag */
    EN_EA_OMA_XDM_XDIR_LAST_MDFY,      /* last-modified */
    EN_EA_OMA_XDM_XDIR_SIZE,           /* size */
    EN_EA_OMA_XDM_XDIR_ERR_CODE,       /* error-code */
    EN_EA_OMA_XDM_XDIR_AUID            /* auid */
} EN_EA_OMA_XDM_XDIR_TYPE;

/* xdm xcap-directory get the xcap-directory */
ZFUNC ZINT EaXdm_XdirGetDir(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstDir);

/* xdm xcap-directory get the first xcap-directory folder */
ZFUNC ZINT EaXdm_XdirDirGetFirstFolder(ST_XML_ELEM *pstDir, 
                ST_XML_ELEM **ppstFolder);

/* xdm xcap-directory get the next xcap-directory folder */
ZFUNC ZINT EaXdm_XdirDirGetNextFolder(ST_XML_ELEM *pstFolder, 
                ST_XML_ELEM **ppstFolder);

/* xdm xcap-directory get the first folder entry */
ZFUNC ZINT EaXdm_XdirFolderGetFirstEntry(ST_XML_ELEM *pstFolder, 
                ST_XML_ELEM **ppstEntry);

/* xdm xcap-directory get the next folder entry */
ZFUNC ZINT EaXdm_XdirFolderGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* xdm xcap-directory get the folder error-code */
ZFUNC ZINT EaXdm_XdirFolderGetErrCode(ST_XML_ELEM *pstFolder, 
                ST_ZOS_USTR **ppstErrCode);

/* xdm xcap-directory get the folder auid */
ZFUNC ZINT EaXdm_XdirFolderGetAuid(ST_XML_ELEM *pstFolder, 
                ST_ZOS_USTR **ppstAuid);

/* xdm xcap-directory get the folder uri */
ZFUNC ZINT EaXdm_XdirEntryGetUri(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstUri);

/* xdm xcap-directory get the folder etag */
ZFUNC ZINT EaXdm_XdirEntryGetEtag(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstEtag);

/* xdm xcap-directory get the folder last-modified */
ZFUNC ZINT EaXdm_XdirEntryGetLastMdfy(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstLastMdfy);

/* xdm xcap-directory get the folder size */
ZFUNC ZINT EaXdm_XdirEntryGetSize(ST_XML_ELEM *pstEntry, ZUINT *piSize);

/* xdm xcap-directory set the xcap-directory */
ZFUNC ZINT EaXdm_XdirSetDir(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstDir);

/* xdm xcap-directory set the xcap-directory folder */
ZFUNC ZINT EaXdm_XdirDirSetFolder(ST_XML_ELEM *pstDir, 
                ST_XML_ELEM **ppstFolder);

/* xdm xcap-directory set the folder entry */
ZFUNC ZINT EaXdm_XdirFolderSetEntry(ST_XML_ELEM *pstFolder, 
                ST_XML_ELEM **ppstEntry);

/* xdm xcap-directory set the folder error-code */
ZFUNC ZINT EaXdm_XdirFolderSetErrCode(ST_XML_ELEM *pstFolder, 
                ST_ZOS_USTR *pstErrCode);

/* xdm xcap-directory set the folder auid */
ZFUNC ZINT EaXdm_XdirFolderSetAuid(ST_XML_ELEM *pstFolder, 
                ST_ZOS_USTR *pstAuid);

/* xdm xcap-directory set the folder uri */
ZFUNC ZINT EaXdm_XdirEntrySetUri(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR *pstUri);

/* xdm xcap-directory set the folder etag */
ZFUNC ZINT EaXdm_XdirEntrySetEtag(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR *pstEtag);

/* xdm xcap-directory set the folder last-modified */
ZFUNC ZINT EaXdm_XdirEntrySetLastMdfy(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR *pstLastMdfy);

/* xdm xcap-directory set the folder size */
ZFUNC ZINT EaXdm_XdirEntrySetSize(ST_XML_ELEM *pstEntry, ZUINT iSize);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XDM_XDIR_H__ */

