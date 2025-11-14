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
  File name     : ea_rcs_fthttp.h
  Module        : eax rcs fthttp
  Author        : gloria.yang
  Version       : 1.0
  Created on    : 2018-10-15
  Description   :
    Marcos and structure definitions required by the eax rcs fthttp.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_RCS_FTHTTP_H__
#define _EA_RCS_FTHTTP_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:gsma:params:xml:ns:rcs:rcs:fthttp **************/
typedef enum EN_EA_RCS_FTHTTP_TYPE
{
    EN_EA_RCS_FTHTTP_FILE,              /* file */
    EN_EA_RCS_FTHTTP_FILE_INFO,         /* file-info */
    EN_EA_RCS_FTHTTP_FILE_INFO_TYPE,    /* type */
    EN_EA_RCS_FTHTTP_FILE_SIZE,         /* file-size */
    EN_EA_RCS_FTHTTP_CONTENT_TYPE,      /* content-type */
    EN_EA_RCS_FTHTTP_FILE_TYPE,         /* file-type */    
    EN_EA_RCS_FTHTTP_FILE_DATA,         /* data */
    EN_EA_RCS_FTHTTP_FILE_DATA_URL,     /* url */
    EN_EA_RCS_FTHTTP_FILE_DATA_UNTIL,   /* until */
    EN_EA_RCS_FTHTTP_HASH_ALGORITHM,    /* file-hash-algorithm */
    EN_EA_RCS_FTHTTP_HASH_VALUE,        /* file-hash-value */
    EN_EA_RCS_FTHTTP_FILE_NAME,         /* file-name */
    EN_EA_RCS_FTHTTP_FILE_DURATION,     /* am:playing-length */
    EN_EA_RCS_FTHTTP_FILE_EXIST,        /* file-exist */
} EN_EA_RCS_FTHTTP_TYPE;

/* eax rcs fthttp get the file */
ZFUNC ZINT EaRcsFtHttp_GetFile(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstFile);

/* eax rcs fthttp set the file */
ZFUNC ZINT EaRcsFtHttp_SetFile(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstFile);

/* eax rcs fthttp get the first file-info */
ZFUNC ZINT EaRcsFtHttp_FileGetFirstFileInfo(ST_XML_ELEM *pstFile, 
                ST_XML_ELEM **ppstFileInfo);

/* eax rcs fthttp get the next file-info */
ZFUNC ZINT EaRcsFtHttp_FileGetNextFileInfo(ST_XML_ELEM *pstFileInfo, 
                ST_XML_ELEM **ppstFileInfo);

/* eax rcs fthttp set the file-info */
ZFUNC ZINT EaRcsFtHttp_FileSetFileInfo(ST_XML_ELEM *pstFile,
                 ST_XML_ELEM **ppstFileInfo);

/* eax rcs fthttp get the file-info type */
ZFUNC ZINT EaRcsFtHttp_FileGetFileInfoType(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR **ppstType);

/* eax rcs fthttp set the file-info type */
ZFUNC ZINT EaRcsFtHttp_FileSetFileInfoType(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR *pstType);

/* eax rcs fthttp get the file-size data */
ZFUNC ZINT EaRcsFtHttp_FileGetFileSizeData(ST_XML_ELEM *pstFileInfo, 
                ZUINT *piFileSize);

/* eax rcs fthttp set the file-size data */
ZFUNC ZINT EaRcsFtHttp_FileSetFileSizeData(ST_XML_ELEM *pstFileInfo, 
                ZUINT iFileSize);

/* eax rcs fthttp get the file name data */
ZFUNC ZINT EaRcsFtHttp_FileGetFileNameData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR **ppstFileName);

/* eax rcs fthttp Set the file name data */
ZFUNC ZINT EaRcsFtHttp_FileSetFileNameData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR *pstFileName);

/* eax rcs fthttp get the file type data */
ZFUNC ZINT EaRcsFtHttp_FileGetFileTypeData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR **ppstFileType);

/* eax rcs fthttp Set the file type data */
ZFUNC ZINT EaRcsFtHttp_FileSetFileTypeData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR *pstFileType);

/* eax rcs fthttp get the content type data */
ZFUNC ZINT EaRcsFtHttp_FileGetContentTypeData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR **ppstContentType);

/* eax rcs fthttp Set the content type data */
ZFUNC ZINT EaRcsFtHttp_FileSetContentTypeData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR *pstContentType);

/* eax rcs fthttp get the file data */
ZFUNC ZINT EaRcsFtHttp_FileGetFileData(ST_XML_ELEM *pstFileInfo,
                 ST_XML_ELEM **ppstFileData);

/* eax rcs fthttp set the file data */
ZFUNC ZINT EaRcsFtHttp_FileSetFileData(ST_XML_ELEM *pstFileInfo,
                 ST_XML_ELEM **ppstFileData);

/* eax rcs fthttp get the file data url */
ZFUNC ZINT EaRcsFtHttp_FileGetFileDataUrl(ST_XML_ELEM *pstFileData, 
                ST_ZOS_USTR **ppstUrl);

/* eax rcs fthttp set the file data url */
ZFUNC ZINT EaRcsFtHttp_FileSetFileDataUrl(ST_XML_ELEM *pstFileData, 
                ST_ZOS_USTR *pstUrl);

/* eax rcs fthttp get the file data until */
ZFUNC ZINT EaRcsFtHttp_FileGetFileDataUntil(ST_XML_ELEM *pstFileData, 
                ST_ZOS_USTR **ppstUntil);

/* eax rcs fthttp set the file data until */
ZFUNC ZINT EaRcsFtHttp_FileSetFileDataUntil(ST_XML_ELEM *pstFileData, 
                ST_ZOS_USTR *pstUntil);

/* eax rcs fthttp set the hash algorithm data */
ZFUNC ZINT EaRcsFtHttp_FileSetHashAlgorithmData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR *pstAlgorithm);

/* eax rcs fthttp set the hash value data */
ZFUNC ZINT EaRcsFtHttp_FileSetHashValueData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR *pstValue);

/* eax rcs fthttp get the hash value data */
ZFUNC ZINT EaRcsFtHttp_FileGetHashValueData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR **ppstValue);

/* eax rcs fthttp set the file-disposition data */
ZFUNC ZINT EaRcsFtHttp_FileSetFileDurationData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR *pstValue);

/* eax rcs fthttp get the file-disposition data */
ZFUNC ZINT EaRcsFtHttp_FileGetFileDurationData(ST_XML_ELEM *pstFileInfo, 
                ST_ZOS_USTR **ppstValue);

ZFUNC ZINT EaRcsFtHttp_FileGetExistData(ST_XML_ELEM *pstFileInfo, 
                ZBOOL *pbFileExist);

#ifdef __cplusplus
}
#endif

#endif /* _EA_RCS_FTHTTP_H__ */

