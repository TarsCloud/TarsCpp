/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
    rights reserved.

    License to copy and use this software is granted provided that it
    is identified as the "RSA Data Security, Inc. MD5 Message-Digest
    Algorithm" in all material mentioning or referencing this software
    or this function.

    License is also granted to make and use derivative works provided
    that such works are identified as "derived from the RSA Data
    Security, Inc. MD5 Message-Digest Algorithm" in all material
    mentioning or referencing the derived work.

    RSA Data Security, Inc. makes no representations concerning either
    the merchantability of this software or the suitability of this
    software for any particular purpose. It is provided "as is"
    without express or implied warranty of any kind.

    These notices must be retained in any copies of any part of this
    documentation and/or software.
 */

#ifndef _ZMD5_H__
#define _ZMD5_H__

#ifdef __cplusplus
extern "C"{
#endif   

/* zos md5 result length */
#define ZMD5_RESULT_LEN 16

/* zos md5 context. */
typedef struct tagZMD5_CTX
{
    ZUINT aiState[4];                /* state (ABCD) */
    ZUINT aiCount[2];                /* number of bits, mod 2^64 (lsb first) */
    ZUCHAR aucBuffer[64];            /* input buffer */
} ST_ZMD5_CTX;

/* zos md5 init */
ZFUNC ZVOID Zmd5_Init(ST_ZMD5_CTX *pstCtx);

/* zos md5 update input */
ZFUNC ZVOID Zmd5_Update(ST_ZMD5_CTX *pstCtx, ZCONST ZUCHAR *pucInput, 
                ZUINT iInputLen);

/* zos md5 final value */
ZFUNC ZVOID Zmd5_Final(ZUCHAR aucDigest[16], ST_ZMD5_CTX *pstCtx);

/* zos md5 a string */
ZFUNC ZVOID Zmd5_Str(ZCONST ZUCHAR *pucStr, ZUINT iStrLen, 
                ZUCHAR aucDigest[ZMD5_RESULT_LEN]);

/* zos md5 two strings */
ZFUNC ZVOID Zmd5_Str2(ZCONST ZUCHAR *pucStr1, ZUINT iLen1,
                ZCONST ZUCHAR *pucStr2, ZUINT iLen2,
                ZUCHAR aucDigest[ZMD5_RESULT_LEN]);

/* zos md5 string, output hex number string */
ZFUNC ZCHAR * Zmd5_StrX(ZCONST ZUCHAR *pucStr, ZUINT iStrLen, ZBOOL bPrefix,
                ZCONST ZCHAR *pcSepa);

/* zos md5 string reentrancable, output hex number string */
ZFUNC ZCHAR * Zmd5_StrXr(ZCONST ZUCHAR *pucStr, ZUINT iStrLen, ZBOOL bPrefix,
                ZCONST ZCHAR *pcSepa, ZCHAR *pcBuf, ZUINT iBufSize);

#ifdef __cplusplus
}
#endif

#endif /* _ZMD5_H__ */

