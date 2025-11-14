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
  File name     : zucs.h
  Module        : unicode utilitis
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the 
      unicode utilitis
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZUCS_H__
#define _ZUCS_H__

#define ZUCS_IS_HANZI(uni) ((uni) > 19967 && (uni) < 40870)
#define ZUCS_IS_ASCII(uni) ((uni) >= 0 && (uni) <= 127)

/* code page type */
typedef enum EN_ZUCS_CP_TYPE
{
    EN_ZUCS_CP_GBK,                  /* Chinese Simplified (GBK) */
    EN_ZUCS_CP_BIG5,                 /* Chinese Traditional (Big5) */
    EN_ZUCS_CP_SIZE,                 /* max size */
} EN_ZUCS_CP_TYPE;

#ifdef __cplusplus
extern "C" {
#endif

/* count UTF8 size */
ZFUNC ZUINT Zucs_Utf8StrLen(ZUCHAR *pucUtf8, ZUINT iUtf8Size);

/* count UTF8 size */
ZFUNC ZSIZE_T Zucs_Utf8Size(ZUSHORT *pwBmp, ZSIZE_T zBmpSize);

/* convert UTF8 to BMP */
ZFUNC ZINT Zucs_Utf8ToBmpX(ZUSHORT **ppwBmp, ZUINT *piBmpSize, 
                ZUCHAR *pucUtf8, ZUINT iUtf8Size);

/* convert UTF8 to BMP */
ZFUNC ZUINT Zucs_Utf8ToBmp(ZUSHORT *pwBmp, ZUINT iBmpSize, 
                ZUCHAR *pucUtf8, ZUINT iUtf8Size);

/* count BMP size */
ZFUNC ZUINT Zucs_BmpStrLen(ZUSHORT *pwBmp);

/* convert BMP to UTF8, it must be bigendian */
ZFUNC ZINT Zucs_BmpToUtf8X(ZUCHAR **ppucUtf8, ZSIZE_T *pzUtf8Size, 
                ZUSHORT *pwBmp, ZSIZE_T zBmpSize);

/* convert BMP to UTF8, it must be digendian */
ZFUNC ZSIZE_T Zucs_BmpToUtf8(ZUCHAR *pucUtf8, ZSIZE_T zUtf8Size, 
                ZUSHORT *pwBmp, ZSIZE_T zBmpSize);

/* convert BMP to specific code page string, it must be bigendian */
ZFUNC ZINT Zucs_BmpToCpStr(ZUCHAR *pucCpStr, ZUINT *piCpStrSize,
                ZUINT iCpType, ZUSHORT *pwBmp);

/* convert specific code page string to BMP, it must be bigendian */
ZFUNC ZINT Zucs_CpStrToBmp(ZUSHORT *pwBmp, ZSIZE_T *pzBmpLen, 
                ZUCHAR *pucCpStr, ZUINT iCpType);

/* convert unicode to pin yin */
ZFUNC ZINT Zucs_BmpToPinyin(ZUSHORT *pwBmp, ZUINT iBmpLen,
                ZCHAR *pcOutPinyin, ZUINT iOutLen, ZCHAR *pcSep);

#ifdef __cplusplus
}
#endif

#endif /* _ZUCS_H__ */

