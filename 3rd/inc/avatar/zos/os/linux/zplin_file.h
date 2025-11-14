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
  File name     : zplin_file.h
  Module        : file utilitis
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by the 
      file utilitis
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPLIN_FILE_H__
#define _ZPLIN_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* linux file open */
ZFUNC ZINT Zplin_FileOpen(ZCHAR *pcFileName, ZUINT iFlag, ZFILE *pzFile);

/* linux file close */
ZFUNC ZINT Zplin_FileClose(ZFILE zFile);

/* linux file remove */
ZFUNC ZINT Zplin_FileRemove(ZCHAR *pcFileName);

/* linux file remove force */
ZFUNC ZINT Zplin_FileRemoveX(ZCHAR *pcFileName);

/* linux file rename */
ZFUNC ZINT Zplin_FileRename(ZCHAR *pcOldName, ZCHAR *pcNewName);

/* linux file is end of file */
ZFUNC ZBOOL Zplin_FileEof(ZFILE zFile);

/* linux file read data */
ZFUNC ZINT Zplin_FileRead(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* linux file write data */
ZFUNC ZINT Zplin_FileWrite(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* linux file flush data */
ZFUNC ZINT Zplin_FileFlush(ZFILE zFile);

/* linux file get string */
ZFUNC ZINT Zplin_FileGets(ZFILE zFile, ZCHAR *pcStr, ZINT iLen);

/* linux file seek position */
ZFUNC ZINT Zplin_FileSeek(ZFILE zFile, ZUINT iFlag, ZUINT iOffset);

/* linux file tell position */
ZFUNC ZINT Zplin_FileTell(ZFILE zFile, ZUINT *piOffset);

/* linux file get file size */
ZFUNC ZINT Zplin_FileSize(ZFILE zFile, ZUINT *piSize);

/* linux file get status */
ZFUNC ZINT Zplin_FileStat(ZCHAR *pcFileName, ST_ZFILE_INFO *pstInfo);

/* linux file open directory */
ZFUNC ZINT Zplin_DirOpen(ZCHAR *pcDirName, ZDIR *pzDir);

/* linux file close directory */
ZFUNC ZINT Zplin_DirClose(ZDIR zDir);

/* linux file read directory status */
ZFUNC ZINT Zplin_DirRead(ZDIR zDir, ZCHAR *pcEntryName, ZSIZE_T zMaxSize, 
                ST_ZFILE_INFO *pstInfo);

/* linux file make directory, flag is ZFILE_URD... */
ZFUNC ZINT Zplin_DirMake(ZCHAR *pcDirName, ZUINT iFlag);

/* linux file remove directory(no file and sub directory) */
ZFUNC ZINT Zplin_DirRemove(ZCHAR *pcDirName);

#ifdef __cplusplus
}
#endif

#endif /* _ZPLIN_FILE_H__ */

