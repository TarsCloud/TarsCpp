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
  File name     : zarchive.h
  Module        : archive
  Author        : bob.liu
  Created on    : 2013-06-18
  Description   :
    Function implement required by archive.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZARCHIVE_H_
#define _ZARCHIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum EN_ZARCHIVE_COMPRESS_TYPE
{
    EN_ZARCHIVE_COMPRESS_NONE,       /**< @brief No compress. */
    EN_ZARCHIVE_COMPRESS_GZIP        /**< @brief Compress by gzip. */
} EN_ZARCHIVE_COMPRESS_TYPE;

/* archive create */
ZFUNC ZINT Zarchive_Create(ZCONST ZCHAR *pcFileName, ZUINT iCompressType,
                ZCONST ZCHAR *pcRoot, ZCONST ZCHAR **ppcDirs);

/* archive new */
ZFUNC ZVOID * Zarchive_New(ZCONST ZCHAR *pcFileName, ZUINT iCompressType);

/* archive add path */
ZFUNC ZINT Zarchive_AddPath(ZVOID *pzArchive, ZCONST ZCHAR *pcPath,
                ZINT iEntryOffset);

/* archive end */
ZFUNC ZVOID Zarchive_End(ZVOID *pzArchive);

#ifdef __cplusplus
}
#endif

#endif /* _ZARCHIVE_H_ */

