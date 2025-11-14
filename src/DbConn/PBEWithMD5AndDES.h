#ifndef _PBEWITHMD5ANDDES_H_
#define _PBEWITHMD5ANDDES_H_

#ifdef _MSC_VER
#define OPENSSL_CONF_FILE_WIN_X86_64
#else
#define OPENSSL_CONF_FILE_LINUX_X86_64
#endif

#include <stdio.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/des.h>
#include <openssl/md5.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

/**
 * Encrypt for PBEWithMD5AndDES
 * <br><br>
 * <b>Note: </b>Remember to free "out"
 *
 * @param *msg message for encrypt
 * @param msg_len lenght of msg
 * @param *passwd password for encrypt
 * @param passwd_len lenght of password
 * @param **out pointer to encrypt buffer, malloc() during encryption
 *
 * @return *out, which is encrypt Base64 string or NULL if failed
 */
char * PBEWithMD5AndDES_encrypt(unsigned char *msg, size_t msg_len, unsigned char *passwd, size_t passwd_len, char **out);

/**
 * Decrypt for PBEWithMD5AndDES
 * <br><br>
 * <b>Note: </b>Remember to free "out"
 *
 * @param *str Encrypt Base64 string, terminated with '\0'
 * @param *passwd password for decrypt
 * @param passwd_len lenght of password
 * @param **out pointer to decrypt buffer, terminated with '\0' (*out[*len], won't affect content) malloc() during decryption
 * @param *len length of out, set to 0 if failed
 *
 * @return *out, which points to buffer or NULL if failed
 */
unsigned char * PBEWithMD5AndDES_decrypt(char *str, unsigned char *passwd, size_t passwd_len, unsigned char **out, size_t *len);


#endif

