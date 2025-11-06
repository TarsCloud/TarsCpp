/************************************************************************

        Copyright (c) 2005-2015 by Juphoon System Software, Inc.
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
  File name     : zaes.h
  Module        : AES functions
  Author        : anthouny.zhou   
  Version       : 0.1   
  Created on    : 2015-01-20    
  Description   :
      Data structure and function definitions required by the AES interface
      
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZAES_H__
#define _ZAES_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @brief AES padding type */
typedef enum EN_ZAES_PADDING_TYPE
{
    EN_ZAES_PADDING_NO,              /* padding no mode */
    EN_ZAES_PADDING_ZERO,            /* padding zero mode */
    EN_ZAES_PADDING_PKCS5,           /* padding PKCS5 mode */ 
    EN_ZAES_PADDING_PKCS7            /* padding PKCS7 mode */
} EN_ZAES_PADDING_TYPE;

/* aes ecb encrypt */
ZINT Zaes_Encrypt(ZUCHAR *pucInput, ZINT iInputLen, ZUCHAR *pucKey, 
                ZUCHAR *pucOutput);

/* aes ecb decrypt */
ZINT Zaes_Decrypt(ZUCHAR *pucInput, ZINT iInputLen, ZUCHAR *pucKey, 
                ZUCHAR *pucOutput);

/* aes cbc encrypt */
ZINT Zaes_CbcEncrypt(ZUCHAR *pucInput, ZINT iInputLen, ZUCHAR *pucKey, 
                ZUCHAR *pucIvec, ZUCHAR *pucOutput);

/* aes cbc decrypt */
ZINT Zaes_CbcDecrypt(ZUCHAR *pucInput, ZINT iInputLen, ZUCHAR *pucKey, 
                ZUCHAR *pucIvec, ZUCHAR *pucOutput);

/* aes ecb encrypt data */
ZINT Zaes_EncData(ZUBUF zMemBuf, ZUCHAR *pucKey, ZUCHAR *pucInput, 
                ZINT iInputLen, ZUCHAR **ppucOutput, ZINT *piOutputLen, 
                ZUINT iPaddingMode);

/* aes ecb decrypt data */
ZINT Zaes_DecData(ZUBUF zMemBuf, ZUCHAR *pucKey, ZUCHAR *pucInput, 
                ZINT iInputLen, ZUCHAR **ppucOutput, ZINT *piOutputLen, 
                ZUINT iPaddingMode);

ZINT Zaes_EncFile(ZUCHAR *pucKey, ZUINT iPaddingMode, ZBOOL bAddFileHeader,
                ZCONST ZCHAR *pcSrcFileName, ZCONST ZCHAR *pcDstFileName);

/* aes ecb encrypt file */
ZINT Zaes_DecFile(ZUCHAR *pucKey, ZUINT iPaddingMode, ZBOOL bCheckHeader,
                ZCONST ZCHAR *pcSrcFileName, ZCONST ZCHAR *pcDstFileName);

/* aes cbc encrypt data */
ZINT Zaes_CbcEncData(ZUBUF zMemBuf, ZUCHAR *pucKey, ZUCHAR *pucIvec, 
                ZUCHAR *pucInput, ZINT iInputLen, ZUCHAR **ppucOutput, 
                ZINT *piOutputLen, ZUINT iPaddingMode);

/* aes cbc decrypt data */
ZINT Zaes_CbcDecData(ZUBUF zMemBuf, ZUCHAR *pucKey, ZUCHAR *pucIvec, 
                ZUCHAR *pucInput, ZINT iInputLen, ZUCHAR **ppucOutput, 
                ZINT *piOutputLen, ZUINT iPaddingMode);

#ifdef __cplusplus
}
#endif

#endif /* _ZAES_H__ */

