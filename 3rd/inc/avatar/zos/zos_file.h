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
  File name     : zos_file.h
  Module        : zos file
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2007-10-15    
  Description   :
      Data structure and function definitions required by the file.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_FILE_H__
#define _ZOS_FILE_H__

/** 
 * @file zos_file.h
 * @brief Zos file functions.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief File directory name len */
#define ZFILE_NAME_LEN 256
#define ZFILE_DIR_NAME_LEN 256

/** @brief File open flag */
#define ZFILE_READ        0x00001    /**< @brief Open file for reading */
#define ZFILE_WRITE       0x00002    /**< @brief Open file for writing */
#define ZFILE_CREATE      0x00004    /**< @brief Create file if not there */
#define ZFILE_APPEND      0x00008    /**< @brief Append to the end of the file */
#define ZFILE_TRUNC       0x00010    /**< @brief Open file and truncate to 0 length */
#define ZFILE_BIN         0x00020    /**< @brief Open file in binary mode */
#define ZFILE_CIPHER      0x00040    /**< @brief Open file in cipher mode */
#define ZFILE_NO_CIPHER   0x00080    /**< @brief Open file in no cipher mode */

/** @brief File seek flag */
#define ZFILE_SEEK_BEGIN 1           /**< @brief Begin position */
#define ZFILE_SEEK_END   2           /**< @brief End position */
#define ZFILE_SEEK_CUR   3           /**< @brief Current position */ 

/** @brief File permissions type */
#define ZFILE_URD         0x0001     /**< @brief User can read */
#define ZFILE_UWR         0x0002     /**< @brief User can write */
#define ZFILE_UEXE        0x0004     /**< @brief User can execute */
#define ZFILE_GRD         0x0010     /**< @brief Group can read */
#define ZFILE_GWR         0x0020     /**< @brief Group can write */
#define ZFILE_GEXE        0x0040     /**< @brief Group can execute */
#define ZFILE_ORD         0x0100     /**< @brief Others can read */
#define ZFILE_OWR         0x0200     /**< @brief Others can write */
#define ZFILE_OEXE        0x0400     /**< @brief Others can execute */

/** @brief File type */
typedef enum EN_ZFILE_TYPE
{
    EN_ZFILE_REG,                    /**< @brief Regular file */
    EN_ZFILE_DIR,                    /**< @brief Directory */
    EN_ZFILE_CHR,                    /**< @brief Character device */
    EN_ZFILE_BLK,                    /**< @brief Block device */
    EN_ZFILE_PIPE,                   /**< @brief FIFO / pipe */
    EN_ZFILE_LINK,                   /**< @brief Symbolic link */
    EN_ZFILE_SOCK,                   /**< @brief [UNIX domain] socket */
    EN_ZFILE_UNKNOWN = ZMAXCHAR      /**< @brief Unknown file type */
} EN_ZFILE_TYPE; 

/** @brief File information structure */
typedef struct tagZFILE_INFO 
{
    ZUCHAR ucType;                   /* file type EN_ZFILE_TYPE */
    ZUCHAR aucSpare[3];              /* for 32 bit alignment */
    ZUINT iMode;                     /* protection mode */
    ZUINT64 qwSize;                  /* file size in bytes */
    ZTIME_T zLastAccTime;            /* last accessed time */
    ZTIME_T zLastMdfyTime;           /* last data modified time */
    ZTIME_T zLastChangeTime;         /* last status changed time */
} ST_ZFILE_INFO;

/** @brief Directory */
typedef ZVOID * ZDIR;

/** @brief Directory for traversal. */
typedef ZVOID * ZDIRX;

/**
 * @brief Open a file.
 *
 * @param [in] pcFileName The file name.
 * @param [in] iFlag Open file flag ZFILE_READ...
 * @param [out] pzFile The file handle.
 *
 * @return ZOK Open file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Close
 */
ZFUNC ZINT Zfile_Open(ZCONST ZCHAR *pcFileName, ZUINT iFlag, ZFILE *pzFile);

/**
 * @brief Close a file.
 *
 * @param [in] zFile The file handle.
 *
 * @return ZOK Close file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZINT Zfile_Close(ZFILE zFile);

/**
 * @brief Close a file with aes key .
 *
 * @param [in] zFile The file handle.
 * @param [in] pcAesKey The aes key. 
 *
 * @return ZOK Close file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZINT Zfile_CloseX(ZFILE zFile, ZCHAR *pcAesKey);

/**
 * @brief Remove a file.
 *
 * @param [in] pcFileName The file name.
 *
 * @return ZOK Remove file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZINT Zfile_Remove(ZCONST ZCHAR *pcFileName);

/**
 * @brief Remove a file by force.
 *
 * @param [in] pcFileName The file name.
 *
 * @return ZOK Remove file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZINT Zfile_RemoveX(ZCONST ZCHAR *pcFileName);

/**
 * @brief Rename a file name.
 *
 * @param [in] pcOldName The file old name.
 * @param [in] pcNewName The file new name.
 *
 * @return ZOK Rename file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZINT Zfile_Rename(ZCONST ZCHAR *pcOldName, ZCONST ZCHAR *pcNewName);

/**
 * @brief Is reach the file end.
 *
 * @param [in] zFile The file handle.
 *
 * @return ZTRUE Reach file end, otherwise ZFALSE.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZBOOL Zfile_Eof(ZFILE zFile);

/**
 * @brief Load a file content into a memory .
 *
 * @param [in] pcFileName The file name.
 * @param [out] ppcData The memory data.
 * @param [out] pzSize The memory data size.
 *
 * @return ZOK Load file content successfully, otherwise ZFAILED.
 *
 * @note The file content memory should be freed by Zos_Free.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZINT Zfile_Load(ZCONST ZCHAR *pcFileName, ZCHAR **ppcData, ZSIZE_T *pzSize);

/**
 * @brief Load a file content into a memory with aes key.
 *
 * @param [in] pcFileName The file name.
 * @param [in] pcAesKey The aes key.
 * @param [out] ppcData The memory data.
 * @param [out] pzSize The memory data size.
 *
 * @return ZOK Load file content successfully, otherwise ZFAILED.
 *
 * @note The file content memory should be freed by Zos_Free.
 *
 * @see @ref Zfile_Open
 */
ZFUNC ZINT Zfile_LoadX(ZCONST ZCHAR *pcFileName, ZCHAR *pcAesKey, 
                    ZCHAR **ppcData, ZSIZE_T *pzSize);

/**
 * @brief Save a file content from memory data .
 *
 * @param [in] pcFileName The file name.
 * @param [in] pcData The memory data.
 * @param [in] zSize The memory data size.
 *
 * @return ZOK Save file content successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Load
 */
ZFUNC ZINT Zfile_Save(ZCONST ZCHAR *pcFileName, ZCHAR *pcData,  ZSIZE_T zSize);

/**
 * @brief Save a file content from memory data in cipher mode .
 *
 * @param [in] pcFileName The file name.
 * @param [out] pcData The memory data.
 * @param [out] zSize The memory data size.
 *
 * @return ZOK Save file content successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Load
 */
ZFUNC ZINT Zfile_SaveX(ZCONST ZCHAR *pcFileName, ZCHAR *pcData, ZSIZE_T zSize);

/**
 * @brief Read file data into a buffer.
 *
 * @param [in] zFile The file handle.
 * @param [in, out] pcBuf The file data buffer memory address.
 * @param [in, out] pzSize The data buffer memory address size.
 *
 * @return ZOK Read file content successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Write
 */
ZFUNC ZINT Zfile_Read(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/* zos file read data with aes key */
/**
 * @brief Read file data into a buffer with aes key.
 *
 * @param [in] zFile The file handle.
 * @param [in] pcAesKey The aes key. 
 * @param [in, out] pcBuf The file data buffer memory address.
 * @param [in, out] pzSize The data buffer memory address size.
 *
 * @return ZOK Read file content successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Write
 */
ZFUNC ZINT Zfile_ReadX(ZFILE zFile, ZCHAR *pcAesKey, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/**
 * @brief Write file data into a buffer.
 *
 * @param [in] zFile The file handle.
 * @param [in] pcBuf The file data buffer memory address.
 * @param [in, out] pzSize The data buffer memory address size.
 *
 * @return ZOK Write buffer data successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Read
 */
ZFUNC ZINT Zfile_Write(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/**
 * @brief Flush file buffer data into disk.
 *
 * @param [in] zFile The file handle.
 *
 * @return ZOK Flush buffer data successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Write
 */
ZFUNC ZINT Zfile_Flush(ZFILE zFile);

/**
 * @brief Get a line data from file.
 *
 * @param [in] zFile The file handle.
 * @param [in] pcStr The data buffer memory address.
 * @param [in] iLen The data buffer length.
 *
 * @return ZOK Get a line data successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Write
 */
ZFUNC ZINT Zfile_Gets(ZFILE zFile, ZCHAR *pcStr, ZINT iLen);

/**
 * @brief Seek file new position.
 *
 * @param [in] zFile The file handle.
 * @param [in] iFlag The seek flag ZFILE_SEEK_BEGIN...
 * @param [in] iOffset The file position from seek start position.
 *
 * @return ZOK Seek file position successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Write
 */
ZFUNC ZINT Zfile_Seek(ZFILE zFile, ZUINT iFlag, ZUINT iOffset);

/**
 * @brief Get file current position.
 *
 * @param [in] zFile The file handle.
 * @param [out] piOffset The file position.
 *
 * @return ZOK Get file position successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Seek
 */
ZFUNC ZINT Zfile_Tell(ZFILE zFile, ZUINT *piOffset);

/**
 * @brief Get file size.
 *
 * @param [in] zFile The file handle.
 *
 * @return File size.
 *
 * @see @ref Zfile_Tell
 */
ZFUNC ZUINT Zfile_Size(ZFILE zFile);

/**
 * @brief Get file status information.
 *
 * @param [in] pcFileName The file name.
 * @param [out] pstInfo The file status information.
 *
 * @return File status information.
 *
 * @see @ref Zfile_Size
 */
ZFUNC ZINT Zfile_Stat(ZCONST ZCHAR *pcFileName, ST_ZFILE_INFO *pstInfo);

/**
 * @brief Get file size
 *
 * @param [in] pcFileName The file name.
 *
 * @return File size.
 *
 * @see @ref Zfile_Size
 */
ZFUNC ZUINT64 Zfile_StatSize(ZCONST ZCHAR *pcFileName);

/**
 * @brief Open a two stage save type file.
 *
 * @param [in] pcFileName The file name.
 * @param [in] iFlag Open file flag ZFILE_READ...
 * @param [out] pzFile The file handle.
 *
 * @return ZOK Open file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_CloseS
 */
ZFUNC ZINT Zfile_OpenS(ZCONST ZCHAR *pcFileName, ZUINT iFlag, ZFILE *pzFile);

/**
 * @brief Close a two stage save file.
 *
 * @param [in] zFile The file handle.
 *
 * @return ZOK Close file successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_OpenS
 */
ZFUNC ZINT Zfile_CloseS(ZFILE zFile);

/**
 * @brief Read two stage save file data into a buffer.
 *
 * @param [in] zFile The file handle.
 * @param [in, out] pcBuf The file data buffer memory address.
 * @param [in, out] pzSize The data buffer memory address size.
 *
 * @return ZOK Read file content successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Write
 */
ZFUNC ZINT Zfile_ReadS(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/**
 * @brief Write two stage save file data into a buffer.
 *
 * @param [in] zFile The file handle.
 * @param [in] pcBuf The file data buffer memory address.
 * @param [in, out] pzSize The data buffer memory address size.
 *
 * @return ZOK Write buffer data successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_Read
 */
ZFUNC ZINT Zfile_WriteS(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);

/**
 * @brief Get two stage save file size.
 *
 * @param [in] zFile The file handle.
 *
 * @return File size.
 *
 * @see @ref Zfile_OpenS
 */
ZFUNC ZUINT Zfile_SizeS(ZFILE zFile);

/**
 * @brief Exist file in disk
 *
 * @param [in] pcFileName The file name.
 *
 * @return Exist file in disk.
 *
 * @see @ref Zfile_IsExistDir
 */
ZFUNC ZBOOL Zfile_IsExistFile(ZCONST ZCHAR *pcFileName);

/**
 * @brief Exist directory in disk
 *
 * @param [in] pcDirName The directory name.
 *
 * @return Exist directory in disk.
 *
 * @see @ref Zfile_IsExistFile
 */
ZFUNC ZBOOL Zfile_IsExistDir(ZCONST ZCHAR *pcDirName);

/**
 * @brief Open a directory.
 *
 * @param [in] pcDirName The directory name.
 * @param [out] pzDir The directory handle.
 *
 * @return ZOK Open directory successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_DirClose
 */
ZFUNC ZINT Zfile_DirOpen(ZCONST ZCHAR *pcDirName, ZDIR *pzDir);

/**
 * @brief Close a directory.
 *
 * @param [in] zDir The directory handle.
 *
 * @return ZOK Close directory successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_DirOpen
 */
ZFUNC ZINT Zfile_DirClose(ZDIR zDir);

/**
 * @brief Read directory data into a buffer.
 *
 * @param [in] zDir The directory handle.
 * @param [in] pcEntryName The file entry name.
 * @param [in] zMaxSize The file entry name size.
 * @param [out] pstInfo The file status information.
 *
 * @return ZOK Read file content successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_DirMake
 */
ZFUNC ZINT Zfile_DirRead(ZDIR zDir, ZCHAR *pcEntryName, ZSIZE_T zMaxSize, 
                ST_ZFILE_INFO *pstInfo);

/**
 * @brief Read directory data into a buffer.
 *
 * @param [in] zDir The directory handle.
 * @param [in] ppcEntryName The file entry name.
 * @param [out] pstInfo The file status information.
 *
 * @return ZOK Read directory file entry successfully, otherwise ZFAILED.
 *
 * @note ppcEntryName should be freed by Zos_SysStrFree.
 *
 * @see @ref Zfile_DirMake
 */
ZFUNC ZINT Zfile_DirReadX(ZDIR zDir, ZCHAR **ppcEntryName, ST_ZFILE_INFO *pstInfo);

/**
 * @brief Make a new directory.
 *
 * @param [in] pcDirName The directory name.
 * @param [in] iFlag The directory flag ZFILE_URD.
 *
 * @return ZOK Make a new directory successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_DirRemove
 */
ZFUNC ZINT Zfile_DirMake(ZCONST ZCHAR *pcDirName, ZUINT iFlag);

/**
 * @brief Remove a directory(no file and sub directory).
 *
 * @param [in] pcDirName The directory name.
 *
 * @return ZOK Remove a directory successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_DirMake
 */
ZFUNC ZINT Zfile_DirRemove(ZCONST ZCHAR *pcDirName);

/**
 * @brief Remove a directory(it can exist file and sub directory).
 *
 * @param [in] pcDirName The directory name.
 *
 * @return ZOK Remove a directory successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_DirMake
 */
ZFUNC ZINT Zfile_DirRemoveX(ZCONST ZCHAR *pcDirName);

/**
 * @brief Remove a directory's child(file and sub directory).
 *
 * @param [in] pcDirName The directory name.
 *
 * @return ZOK Remove the child of directory successfully, otherwise ZFAILED.
 *
 * @see @ref Zfile_DirMake
 */
ZFUNC ZINT Zfile_DirRemoveChild(ZCONST ZCHAR *pcDirName);

/**
 * @brief Open structure porinter for directory traversal.
 *
 * @param [in] pcFullPath The full path name to traversal.
 * @param [in] iRootLen The root path name length, which used to get
 *             relative path.
 *
 * @return Structure porinter.
 *
 * @see @ref Zfile_DirXClose Zfile_DirXNext
 */
ZFUNC ZDIRX Zfile_DirXOpen(ZCONST ZCHAR *pcFullPath, ZUINT iRootLen);

/**
 * @brief Close structure porinter for directory traversal.
 *
 * @param [in] pstDirX Structure porinter create by @ref Zfile_DirXOpen.
 *
 * @see @ref Zfile_DirXOpen
 */
ZFUNC ZVOID Zfile_DirXClose(ZDIRX zDirX);

/**
 * @brief Get current file or directory information.
 *
 * @param [in] pstDirX Structure porinter create by @ref Zfile_DirXOpen.
 *
 * @return Current file or directory information.
 *
 * @see @ref Zfile_DirXOpen
 */
ZFUNC ZCONST ST_ZFILE_INFO * Zfile_DirXInfo(ZDIRX zDirX);

/**
 * @brief Get current file or directory path.
 *
 * @param [in] pstDirX Structure porinter create by @ref Zfile_DirXOpen.
 *
 * @return Current file or directory path.
 *
 * @see @ref Zfile_DirXOpen
 */
ZFUNC ZCONST ZCHAR * Zfile_DirXAccPath(ZDIRX zDirX);

/**
 * @brief Get current file or directory relative path.
 *
 * @param [in] pstDirX Structure porinter create by @ref Zfile_DirXOpen.
 *
 * @return Current file or directory relative path.
 *
 * @see @ref Zfile_DirXOpen
 */
ZFUNC ZCONST ZCHAR * Zfile_DirXRelPath(ZDIRX zDirX);

/**
 * @brief Move to next file or directory.
 *
 * @param [in] pstDirX Structure porinter create by @ref Zfile_DirXOpen.
 *
 * @retval ZOK Indicate operation suceessfully.
 * @retval ZFAILED Indicate no more file or directory.
 *
 * @see @ref Zfile_DirXOpen
 */
ZFUNC ZINT Zfile_DirXNext(ZDIRX zDirX);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_FILE_H__ */

