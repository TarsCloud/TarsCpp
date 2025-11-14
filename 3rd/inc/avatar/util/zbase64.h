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
  File name     : zbase64.h
  Module        : base64 codec functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the 
      base64 codec functions
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZBASE64_H__
#define _ZBASE64_H__

#ifdef __cplusplus
extern "C" {
#endif

/* base64 encode plain text to crypto text */
ZFUNC ZINT Zbase64_Encode(ZUCHAR *pucPlain, ZUINT iLen, 
                ZBOOL bSafeMode, ZUCHAR *pucCrypto, ZUINT *piLen);

/* base64 encode plain text to crypto text,
   pstCrypto will contain crypto text in zMemBuf */
ZFUNC ZINT Zbase64_EncodeX(ZUBUF zMemBuf, ST_ZOS_USTR *pstPlain, 
                ZBOOL bSafeMode, ST_ZOS_USTR *pstCrypto);

/* base64 decode crypto text to plain text */
ZFUNC ZINT Zbase64_Decode(ZUCHAR *pucCrypto, ZUINT iLen, 
                ZUCHAR *pucPlain, ZUINT *piLen);

/* base64 decode crypto text to plain text,
   pstPlain will contain plain text in zMemBuf */
ZFUNC ZINT Zbase64_DecodeX(ZUBUF zMemBuf, ST_ZOS_USTR *pstCrypto,
                ST_ZOS_USTR *pstPlain);

/* base64 decode crypto text to plain text,
   pstPlain will contain plain text in zMemBuf */
ZFUNC ZINT Zbase64_DecodeXX(ZUBUF zMemBuf, ZUCHAR *pucCrypto, 
                ZUINT iCrypLen, ZUCHAR **ppucPlain, ZUINT *piLen);

/* base32 encode plain text to crypto text */
ZFUNC ZINT Zbase32_Encode(ZUCHAR *pucPlain, ZUINT iLen, 
                ZUCHAR *pucCrypto, ZUINT *piLen);

/* base32 encode plain text to crypto text,
   pstCrypto will contain crypto text in zMemBuf */
ZFUNC ZINT Zbase32_EncodeX(ZUBUF zMemBuf, ST_ZOS_USTR *pstPlain, 
                ST_ZOS_USTR *pstCrypto);

/* base32 decode crypto text to plain text */
ZFUNC ZINT Zbase32_Decode(ZUCHAR *pucCrypto, ZUINT iLen, 
                ZUCHAR *pucPlain, ZUINT *piLen);

/* base32 decode crypto text to plain text,
   pstPlain will contain plain text in zMemBuf */
ZFUNC ZINT Zbase32_DecodeX(ZUBUF zMemBuf, ST_ZOS_USTR *pstCrypto,
                ST_ZOS_USTR *pstPlain);

/* base16 encode plain text to crypto text */
ZFUNC ZINT Zbase16_Encode(ZUCHAR *pucPlain, ZUINT iLen, 
                ZUCHAR *pucCrypto, ZUINT *piLen);

/* base16 encode plain text to crypto text,
   pstCrypto will contain crypto text in zMemBuf */
ZFUNC ZINT Zbase16_EncodeX(ZUBUF zMemBuf, ST_ZOS_USTR *pstPlain, 
                ST_ZOS_USTR *pstCrypto);

/* base16 decode crypto text to plain text */
ZFUNC ZINT Zbase16_Decode(ZUCHAR *pucCrypto, ZUINT iLen, 
                ZUCHAR *pucPlain, ZUINT *piLen);

/* base16 decode crypto text to plain text,
   pstPlain will contain plain text in zMemBuf */
ZFUNC ZINT Zbase16_DecodeX(ZUBUF zMemBuf, ST_ZOS_USTR *pstCrypto,
                ST_ZOS_USTR *pstPlain);

#ifdef __cplusplus
}
#endif

#endif /* _ZBASE64_H__ */

