/*
 *  sha1.h
 *
 *  Description:
 *      This is the header file for code which implements the Secure
 *      Hashing Algorithm 1 as defined in FIPS PUB 180-1 published
 *      April 17, 1995.
 *
 *      Many of the variable names in this code, especially the
 *      single character names, were used because those were the names
 *      used in the publication.
 *
 *      Please read the file sha1.c for more information.
 *
 */

#ifndef _ZSHA1_H_
#define _ZSHA1_H_
    
#ifdef __cplusplus
extern "C" {
#endif

/* SHA-1 hash size */
#define ZSHA1_HASH_SIZE 20

/* SHA-1 status code */
typedef enum EN_ZSHA1_STAT
{
    EN_ZSHA1_OK = 0,                 /* success */
    EN_ZSHA1_NULL,                   /* null pointer parameter */
    EN_ZSHA1_INPUT_TOO_LONG,         /* input data too long */
    EN_ZSHA1_ERR                     /* called input after result */
} EN_ZSHA1_STAT;

/* SHA-1 context information */
typedef struct tagZSHA1_CTX
{
    ZUINT aiInterHash[ZSHA1_HASH_SIZE/4]; /* message digest  */
    ZUINT iLowLen;                   /* message length in bits */
    ZUINT iHighLen;                  /* message length in bits */
    ZSHORT wMsgBlkIdx;               /* index into message block array */    
    ZUCHAR aucMsgBlk[64];            /* 512-bit message blocks */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZINT iComputed;                  /* is the digest computed?         */
    ZINT iCorrupted;                 /* is the message digest corrupted? */
} ST_ZSHA1_CTX;

/* sha1 initialize the context */
ZFUNC ZINT Zsha1_Reset(ST_ZSHA1_CTX *pstCtx);

/* sha1 accepts an array of octets as the next portion of the message*/
ZFUNC ZINT Zsha1_Input(ST_ZSHA1_CTX *pstCtx, const ZUCHAR *pucMsg, ZUINT iLen);

/* sha1 get 160-bit message digest */
ZFUNC ZINT Zsha1_Result(ST_ZSHA1_CTX *pstCtx, ZUCHAR aucDigest[ZSHA1_HASH_SIZE]);

#ifdef __cplusplus
}
#endif

#endif


