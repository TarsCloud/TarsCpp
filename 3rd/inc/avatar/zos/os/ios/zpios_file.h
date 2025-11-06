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
  File name     : zpios_file.h
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
#ifndef _ZPIOS_FILE_H__
#define _ZPIOS_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ios file open */
ZFUNC ZINT Zpios_FileOpen(ZCONST ZCHAR *pcFileName, ZUINT iFlag, 
                ZFILE *pzFile);

/* ios file close */
ZFUNC ZINT Zpios_FileClose(ZFILE zFile);

/* ios file remove */
ZFUNC ZINT Zpios_FileRemove(ZCONST ZCHAR *pcFileName);

/* ios file remove force */
ZFUNC ZINT Zpios_FileRemoveX(ZCONST ZCHAR *pcFileName);

/* ios file rename */
ZFUNC ZINT Zpios_FileRename(ZCONST ZCHAR *pcOldName, 
                ZCONST ZCHAR *pcNewName);

/* ios file is end of file */
ZFUNC ZBOOL Zpios_FileEof(ZFILE zFile);

/* ios file read data */
ZFUNC ZINT Zpios_FileRead(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* ios file write data */
ZFUNC ZINT Zpios_FileWrite(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* ios file flush data */
ZFUNC ZINT Zpios_FileFlush(ZFILE zFile);

/* ios file get string */
ZFUNC ZINT Zpios_FileGets(ZFILE zFile, ZCHAR *pcStr, ZINT iLen);

/* ios file seek position */
ZFUNC ZINT Zpios_FileSeek(ZFILE zFile, ZUINT iFlag, ZUINT iOffset);

/* ios file tell position */
ZFUNC ZINT Zpios_FileTell(ZFILE zFile, ZUINT *piOffset);

/* ios file get file size */
ZFUNC ZINT Zpios_FileSize(ZFILE zFile, ZUINT *piSize);

/* ios file get status */
ZFUNC ZINT Zpios_FileStat(ZCONST ZCHAR *pcFileName, ST_ZFILE_INFO *pstInfo);

/* ios file open directory */
ZFUNC ZINT Zpios_DirOpen(ZCONST ZCHAR *pcDirName, ZDIR *pzDir);

/* ios file close directory */
ZFUNC ZINT Zpios_DirClose(ZDIR zDir);

/* ios file read directory status */
ZFUNC ZINT Zpios_DirRead(ZDIR zDir, ZCHAR *pcEntryName, ZUINT iMaxSize, 
                ST_ZFILE_INFO *pstInfo);

/* ios file make directory, flag is ZFILE_URD... */
ZFUNC ZINT Zpios_DirMake(ZCONST ZCHAR *pcDirName, ZUINT iFlag);

/* ios file remove directory(no file and sub directory) */
ZFUNC ZINT Zpios_DirRemove(ZCONST ZCHAR *pcDirName);

#ifdef __cplusplus
}
#endif

#endif /* _ZPIOS_FILE_H__ */

