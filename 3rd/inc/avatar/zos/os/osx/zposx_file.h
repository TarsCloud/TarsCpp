/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : zposx_file.h
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
#ifndef _ZPOSX_FILE_H__
#define _ZPOSX_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* mac osx file open */
ZFUNC ZINT Zposx_FileOpen(ZCONST ZCHAR *pcFileName, ZUINT iFlag, 
                ZFILE *pzFile);

/* mac osx file close */
ZFUNC ZINT Zposx_FileClose(ZFILE zFile);

/* mac osx file remove */
ZFUNC ZINT Zposx_FileRemove(ZCONST ZCHAR *pcFileName);

/* mac osx file remove force */
ZFUNC ZINT Zposx_FileRemoveX(ZCONST ZCHAR *pcFileName);

/* mac osx file rename */
ZFUNC ZINT Zposx_FileRename(ZCONST ZCHAR *pcOldName, 
                ZCONST ZCHAR *pcNewName);

/* mac osx file is end of file */
ZFUNC ZBOOL Zposx_FileEof(ZFILE zFile);

/* mac osx file read data */
ZFUNC ZINT Zposx_FileRead(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* mac osx file write data */
ZFUNC ZINT Zposx_FileWrite(ZFILE zFile, ZCONST ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* mac osx file flush data */
ZFUNC ZINT Zposx_FileFlush(ZFILE zFile);

/* mac osx file get string */
ZFUNC ZINT Zposx_FileGets(ZFILE zFile, ZCHAR *pcStr, ZINT iLen);

/* mac osx file seek position */
ZFUNC ZINT Zposx_FileSeek(ZFILE zFile, ZUINT iFlag, ZUINT qwOffset);

/* mac osx file tell position */
ZFUNC ZINT Zposx_FileTell(ZFILE zFile, ZUINT *pqwOffset);

/* mac osx file get file size */
ZFUNC ZINT Zposx_FileSize(ZFILE zFile, ZUINT *piSize);

/* mac osx file get status */
ZFUNC ZINT Zposx_FileStat(ZCONST ZCHAR *pcFileName, ST_ZFILE_INFO *pstInfo);

/* mac osx file open directory */
ZFUNC ZINT Zposx_DirOpen(ZCONST ZCHAR *pcDirName, ZDIR *pzDir);

/* mac osx file close directory */
ZFUNC ZINT Zposx_DirClose(ZDIR zDir);

/* mac osx file read directory status */
ZFUNC ZINT Zposx_DirRead(ZDIR zDir, ZCHAR *pcEntryName, ZUINT iMaxSize, 
                ST_ZFILE_INFO *pstInfo);

/* mac osx file make directory, flag is ZFILE_URD... */
ZFUNC ZINT Zposx_DirMake(ZCONST ZCHAR *pcDirName, ZUINT iFlag);

/* mac osx file remove directory(no file and sub directory) */
ZFUNC ZINT Zposx_DirRemove(ZCONST ZCHAR *pcDirName);

#ifdef __cplusplus
}
#endif

#endif /* _ZPOSX_FILE_H__ */

