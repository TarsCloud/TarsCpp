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
  File name     : zpwin_file.h
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
#ifndef _ZPWIN_FILE_H__
#define _ZPWIN_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* windows file open */
ZFUNC ZINT Zpwin_FileOpen(ZCONST ZCHAR *pcFileName, ZUINT iFlag, ZFILE *pzFile);

/* windows file open with unicode */
ZFUNC ZINT Zpwin_FileOpenW(ZCONST ZCHAR *pcFileName, ZUINT iFlag, ZFILE *pzFile);

/* windows file close */
ZFUNC ZINT Zpwin_FileClose(ZFILE zFile);

/* windows file remove */
ZFUNC ZINT Zpwin_FileRemove(ZCONST ZCHAR *pcFileName);

/* windows file remove force */
ZFUNC ZINT Zpwin_FileRemoveX(ZCONST ZCHAR *pcFileName);

/* windows file rename */
ZFUNC ZINT Zpwin_FileRename(ZCONST ZCHAR *pcOldName, ZCONST ZCHAR *pcNewName);

/* windows file is end of file */
ZFUNC ZBOOL Zpwin_FileEof(ZFILE zFile);

/* windows file read data */
ZFUNC ZINT Zpwin_FileRead(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* windows file write data */
ZFUNC ZINT Zpwin_FileWrite(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* windows file flush data */
ZFUNC ZINT Zpwin_FileFlush(ZFILE zFile);

/* windows file get string */
ZFUNC ZINT Zpwin_FileGets(ZFILE zFile, ZCHAR *pcStr, ZINT iLen);

/* windows file seek position */
ZFUNC ZINT Zpwin_FileSeek(ZFILE zFile, ZUINT iFlag, ZUINT iOffset);

/* windows file tell position */
ZFUNC ZINT Zpwin_FileTell(ZFILE zFile, ZUINT *piOffset);

/* windows file get file size */
ZFUNC ZINT Zpwin_FileSize(ZFILE zFile, ZUINT *piSize);

/* windows file get status */
ZFUNC ZINT Zpwin_FileStat(ZCONST ZCHAR *pcFileName, ST_ZFILE_INFO *pstInfo);

/* windows file open directory */
ZFUNC ZINT Zpwin_DirOpen(ZCONST ZCHAR *pcDirName, ZDIR *pzDir);

/* windows file close directory */
ZFUNC ZINT Zpwin_DirClose(ZDIR zDir);

/* windows file read directory status */
ZFUNC ZINT Zpwin_DirRead(ZDIR zDir, ZCHAR *pcEntryName, ZUINT iMaxSize, 
                ST_ZFILE_INFO *pstInfo);

/* windows file make directory, flag is ZFILE_URD... */
ZFUNC ZINT Zpwin_DirMake(ZCONST ZCHAR *pcDirName, ZUINT iFlag);

/* windows file remove directory(no file and sub directory) */
ZFUNC ZINT Zpwin_DirRemove(ZCONST ZCHAR *pcDirName);

#ifdef __cplusplus
}
#endif

#endif /* _ZPWIN_FILE_H__ */

