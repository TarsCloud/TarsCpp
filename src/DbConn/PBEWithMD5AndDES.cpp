#include "PBEWithMD5AndDES.h"

#define MAX(a, b)      ((a) > (b) ? (a) : (b))
#define SALT_SIZE      8

#ifndef __FUNCTION__
#define __FUNCTION__ __func__
#endif
#ifdef PBEWithMD5AndDES_DEBUG
#define DEBUG(fmt, ...)                                                 \
        do {                                                            \
                fprintf(stderr, "%s:%d %s() \t" fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
                fprintf(stderr, "\n");} while(0)
#else
#define DEBUG(x,...)   do{}while(0)
#endif

/* https://github.com/exabytes18/OpenSSL-Base64/blob/master/base64.c */
static void openssl_base64_decode(char *encoded_bytes, unsigned char **decoded_bytes, size_t *decoded_length)
{
        BIO *bioMem, *b64;
        size_t buffer_length;

        bioMem = BIO_new_mem_buf((void *)encoded_bytes, -1);
        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bioMem = BIO_push(b64, bioMem);

        buffer_length = BIO_get_mem_data(bioMem, NULL);
        *decoded_bytes = (unsigned char *)malloc(buffer_length);
        if (decoded_bytes == NULL) {
                BIO_free_all(bioMem);
                *decoded_length = 0;
                return;
        }
        *decoded_length = BIO_read(bioMem, *decoded_bytes, buffer_length);
        BIO_free_all(bioMem);
}

/* Return NULL if failed, REMEMBER to free() */
static char *openssl_base64_encode(unsigned char *decoded_bytes, size_t decoded_length)
{
        int x;
        BIO *bioMem, *b64;
        BUF_MEM *bufPtr;
        char *buff = NULL;

        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bioMem = BIO_new(BIO_s_mem());
        b64 = BIO_push(b64, bioMem);

        BIO_write(b64, decoded_bytes, decoded_length);
        x = BIO_flush(b64);
        if (x < 1)
                goto END;

        BIO_get_mem_ptr(b64, &bufPtr);

        buff = (char *) malloc(bufPtr->length+1);
        if (buff == NULL)
                goto END;
        memcpy(buff, bufPtr->data, bufPtr->length);
        buff[bufPtr->length] = 0;

END:
        BIO_free_all(b64);
        return buff;
}

/* Return NULL if failed */
static unsigned char *get_derived_key(unsigned char *md5, unsigned char *pass, size_t pass_len, unsigned char *salt, size_t salt_len)
{
        unsigned char *buf = (unsigned char *)malloc(MAX(pass_len + salt_len, MD5_DIGEST_LENGTH));
        int i;

        if ((md5 == NULL ) || (pass == NULL) || (salt == NULL)) {
                DEBUG("%s", "Parameter error");
                free(buf);
                return NULL;
        }
        if (buf == NULL) {
                DEBUG("%s", "malloc() error");
                return NULL;
        }

        /* Append salt to the password */
        memcpy(buf, pass, pass_len);
        memcpy(buf + pass_len, salt, salt_len);

        /* MD5 Hash it, and hash the result, hash the result ... 1000 times */
        MD5(buf, pass_len + salt_len, md5);
        memcpy((void *)buf, (void *)md5, MD5_DIGEST_LENGTH);
        for (i = 0; i < 1000 - 1; i ++) {
                MD5(buf, MD5_DIGEST_LENGTH, md5);
                memcpy((void *)buf, (void *)md5, MD5_DIGEST_LENGTH);
        }

        free(buf);
        return md5;
}

/* Return NULL if failed, REMEMBER to free() */
char * PBEWithMD5AndDES_encrypt(unsigned char *msg, size_t msg_len, unsigned char *passwd, size_t passwd_len, char **out)
{
        unsigned char salt[SALT_SIZE];
        unsigned char md5[MD5_DIGEST_LENGTH];
        size_t enc_len = 8 - (msg_len % 8) + msg_len;
        /* msg_padded: save msg with padded characters (0x01~0x08) for DES encoding */
        unsigned char *msg_padded = (unsigned char *)malloc(enc_len);
        /* text: save salt + DES encrypt (from msg_padded) characters, we'll return base64 of text */
        unsigned char *text = (unsigned char *)malloc(enc_len + SALT_SIZE);

        *out = NULL;
        if ((msg_padded == NULL) || (text == NULL)) {
                DEBUG("%s", "malloc() error");
                goto DONE;
        }

        /* Generate a salt (random): 8 bytes */
#if 1
        if (RAND_bytes(salt, SALT_SIZE) != 1) {
                DEBUG("%s", "RAND_bytes() error");
                goto DONE;
        }
#else
        strncpy(salt, "testsalt", SALT_SIZE);
#endif

        /* derived key generation */
        if (get_derived_key(md5, passwd, passwd_len, salt, sizeof(salt)) == NULL) {
                DEBUG("%s", "get_derived_key() error");
                goto DONE;
        }

        /* Pad the input string with 1-8 bytes */
        memcpy(msg_padded, msg, msg_len);
        memset(msg_padded + msg_len, enc_len - msg_len, enc_len - msg_len);

        /* Use the key and iv to encrypt the input string, using DES with CBC mode. */
        DES_key_schedule schedule;
        /* the first 8 bytes needing to be of odd paraity */
        DES_set_odd_parity((unsigned char (*)[8])&md5);
        /* The key schedule is an expanded form of the key; it is used to speed the encryption process. */
        if (DES_set_key_checked((unsigned char (*)[8])&md5, &schedule) == -1) {
                DEBUG("%s", "DES_set_key_checked() error");
                goto DONE;
        }
        /* Encryption occurs here */
        DES_ncbc_encrypt(msg_padded, &(text[0]) + SALT_SIZE, enc_len, &schedule, (unsigned char (*)[8])&md5 + 1, DES_ENCRYPT);

        /* Prepend the encrypted value with the salt */
        memcpy(text, salt, SALT_SIZE);

        /* Base64 encode it -> this is your result */
        *out = openssl_base64_encode(text, enc_len + SALT_SIZE);

DONE:
        if (msg_padded != NULL)
                free(msg_padded);
        if (text != NULL)
                free(text);
        return *out;
}

unsigned char * PBEWithMD5AndDES_decrypt(char *str, unsigned char *passwd, size_t passwd_len, unsigned char **out, size_t *len)
{
        /* text: Base64 decoded input message, with its length is "size" */
        unsigned char *text;
        size_t size, i;
        unsigned char pad, salt[SALT_SIZE], md5[MD5_DIGEST_LENGTH];

        *out = NULL;
        *len = 0;

        openssl_base64_decode(str, &text, &size);
        if (size <= SALT_SIZE) {
                DEBUG("%s, size too small: %d.", "openssl_base64_decode() error", (int)size);
                free(text);
                return NULL;
        }

        /*  Extract the salt (first 8 bytes). The rest is the encoded text. */
        memcpy(salt, text, SALT_SIZE);

        /*  Use derived key generation as in Encrypt above to get the key and iv */
        if (get_derived_key(md5, passwd, passwd_len, salt, sizeof(salt)) == NULL) {
                DEBUG("%s", "get_derived_key() error");
                free(text);
                return NULL;
        }

        /*  Decrypt the encoded text using key and iv */
        DES_key_schedule schedule;
        /* the first 8 bytes needing to be of odd paraity */
        DES_set_odd_parity((unsigned char (*)[8])&md5);
        /* The key schedule is an expanded form of the key; it is used to speed the encryption process. */
        if (DES_set_key_checked((unsigned char (*)[8])&md5, &schedule) == -1) {
                DEBUG("%s", "DES_set_key_checked() error");
                free(text);
                return NULL;
        }
        /* Decryption occurs here */
        *out = (unsigned char *)malloc(size - SALT_SIZE);
        if (out == NULL) {
                DEBUG("%s", "malloc() error");
                free(text);
                return NULL;
        }
        DES_ncbc_encrypt(text + SALT_SIZE, *out, size - SALT_SIZE, &schedule, (unsigned char (*)[8])&md5 + 1, DES_DECRYPT);
        free(text);

        /*  Remove padding -> this is your result */
        pad = (*out)[size - SALT_SIZE - 1];
        if ((pad <= 0) || (pad > 8)) {
                DEBUG("pad = %d\n", pad);
                free(*out);
                return (*out = NULL);
        }
        for (i = 0; i < pad; i ++) {
                if ((*out)[size - SALT_SIZE -1 -i] != pad) {
                        DEBUG("i=%d\n", (int)i);
                        free(*out);
                        return (*out = NULL);
                }
        }
        *len = size - SALT_SIZE - pad;
        (*out)[*len] = '\0';

        return *out;
}

