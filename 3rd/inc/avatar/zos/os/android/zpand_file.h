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
  File name     : zpand_file.h
  Module        : file utilitis
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Data structure and function definitions required by the 
      file utilitis
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPAND_FILE_H__
#define _ZPAND_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* android file open */
ZFUNC ZINT Zpand_FileOpen(ZCONST ZCHAR *pcFileName, ZUINT iFlag, ZFILE *pzFile);

/* android file close */
ZFUNC ZINT Zpand_FileClose(ZFILE zFile);

/* android file remove */
ZFUNC ZINT Zpand_FileRemove(ZCONST ZCHAR *pcFileName);

/* android file remove force */
ZFUNC ZINT Zpand_FileRemoveX(ZCONST ZCHAR *pcFileName);

/* android file rename */
ZFUNC ZINT Zpand_FileRename(ZCONST ZCHAR *pcOldName, ZCONST ZCHAR *pcNewName);

/* android file is end of file */
ZFUNC ZBOOL Zpand_FileEof(ZFILE zFile);

/* android file read data */
ZFUNC ZINT Zpand_FileRead(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* android file write data */
ZFUNC ZINT Zpand_FileWrite(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* android file flush data */
ZFUNC ZINT Zpand_FileFlush(ZFILE zFile);

/* android file get string */
ZFUNC ZINT Zpand_FileGets(ZFILE zFile, ZCHAR *pcStr, ZINT iLen);

/* android file seek position */
ZFUNC ZINT Zpand_FileSeek(ZFILE zFile, ZUINT iFlag, ZUINT iOffset);

/* android file tell position */
ZFUNC ZINT Zpand_FileTell(ZFILE zFile, ZUINT *piOffset);

/* android file get file size */
ZFUNC ZINT Zpand_FileSize(ZFILE zFile, ZUINT *piSize);

/* android file get status */
ZFUNC ZINT Zpand_FileStat(ZCONST ZCHAR *pcFileName, ST_ZFILE_INFO *pstInfo);

/* android file open directory */
ZFUNC ZINT Zpand_DirOpen(ZCONST ZCHAR *pcDirName, ZDIR *pzDir);

/* android file close directory */
ZFUNC ZINT Zpand_DirClose(ZDIR zDir);

/* android file read directory status */
ZFUNC ZINT Zpand_DirRead(ZDIR zDir, ZCHAR *pcEntryName, ZSIZE_T zMaxSize, 
                ST_ZFILE_INFO *pstInfo);

/* android file make directory, flag is ZFILE_URD... */
ZFUNC ZINT Zpand_DirMake(ZCONST ZCHAR *pcDirName, ZUINT iFlag);

/* android file remove directory(no file and sub directory) */
ZFUNC ZINT Zpand_DirRemove(ZCONST ZCHAR *pcDirName);

#ifdef __cplusplus
}
#endif

#endif /* _ZPAND_FILE_H__ */

