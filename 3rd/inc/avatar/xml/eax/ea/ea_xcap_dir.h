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
  File name     : ea_xcap_dir.h
  Module        : eax xcap-directory
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax xcap-directory.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XCAP_DIR_H__
#define _EA_XCAP_DIR_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xcap-directory get the xcap-directory */
ZFUNC ZINT EaXcap_DirGetDir(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstDir);

/* xcap-directory get the first xcap-directory entry */
ZFUNC ZINT EaXcap_DirDirGetFirstEntry(ST_XML_ELEM *pstDir, 
                ST_XML_ELEM **ppstEntry);

/* xcap-directory get the next xcap-directory entry */
ZFUNC ZINT EaXcap_DirDirGetNextEntry(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* xcap-directory entry get the display-name */
ZFUNC ZINT EaXcap_DirEntryGetDispName(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstDispName);

/* xcap-directory entry get the uri */
ZFUNC ZINT EaXcap_DirEntryGetUri(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstUri);

/* xcap-directory entry get the auid */
ZFUNC ZINT EaXcap_DirEntryGetAuid(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstAuid);

/* xcap-directory entry get the etag */
ZFUNC ZINT EaXcap_DirEntryGetEtag(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstEtag);

/* xcap-directory entry get the last-modified */
ZFUNC ZINT EaXcap_DirEntryGetLastMdfy(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstLastMdfy);

/* xcap-directory entry get the size */
ZFUNC ZINT EaXcap_DirEntryGetSize(ST_XML_ELEM *pstEntry, 
                ZUINT *piSize);

/* xcap-directory entry get the id */
ZFUNC ZINT EaXcap_DirEntryGetId(ST_XML_ELEM *pstEntry, 
                ST_ZOS_USTR **ppstId);

/* xcap-directory set the xcap-directory */
ZFUNC ZINT EaXcap_DirSetDir(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstDir);

/* xcap-directory set the xcap-directory entry */
ZFUNC ZINT EaXcap_DirDirSetEntry(ST_XML_ELEM *pstDir, 
                ST_XML_ELEM **ppstEntry);
                                                                   
/* xcap-directory entry set the display-name */                      
ZFUNC ZINT EaXcap_DirEntrySetDispName(ST_XML_ELEM *pstEntry,               
                ST_ZOS_USTR *pstDispName);                         
                                                                     
/* xcap-directory entry set the uri */                               
ZFUNC ZINT EaXcap_DirEntrySetUri(ST_XML_ELEM *pstEntry,                    
                ST_ZOS_USTR *pstUri);                              
                                                                     
/* xcap-directory entry set the auid */                              
ZFUNC ZINT EaXcap_DirEntrySetAuid(ST_XML_ELEM *pstEntry,                   
                ST_ZOS_USTR *pstAuid);                             
                                                                     
/* xcap-directory entry set the etag */                              
ZFUNC ZINT EaXcap_DirEntrySetEtag(ST_XML_ELEM *pstEntry,                   
                ST_ZOS_USTR *pstEtag);                             
                                                                     
/* xcap-directory entry set the last-modified */                     
ZFUNC ZINT EaXcap_DirEntrySetLastMdfy(ST_XML_ELEM *pstEntry,               
                ST_ZOS_USTR *pstLastMdfy);                         
                                                                     
/* xcap-directory entry set the size */                              
ZFUNC ZINT EaXcap_DirEntrySetSize(ST_XML_ELEM *pstEntry,                   
                ZUINT iSize);                                    
                                                                     
/* xcap-directory entry set the id */                                
ZFUNC ZINT EaXcap_DirEntrySetId(ST_XML_ELEM *pstEntry,                     
                ST_ZOS_USTR *pstId);                               

#ifdef __cplusplus
}
#endif

#endif /* _EA_XCAP_DIR_H__ */

