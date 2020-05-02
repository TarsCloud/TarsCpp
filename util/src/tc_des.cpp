#include "util/tc_des.h"
#include <iostream>
#include <string.h>
#include <algorithm>

namespace tars
{

static const unsigned short bytebit[8]    = {
    0200, 0100, 040, 020, 010, 04, 02, 01 };

static const uint32_t bigbyte[24] = {
    0x800000L,  0x400000L,  0x200000L,  0x100000L,
    0x80000L,   0x40000L,   0x20000L,   0x10000L,
    0x8000L,    0x4000L,    0x2000L,    0x1000L,
    0x800L,     0x400L,     0x200L,     0x100L,
    0x80L,      0x40L,      0x20L,      0x10L,
    0x8L,       0x4L,       0x2L,       0x1L    };

static const unsigned char pc1[56] = {
    56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
     9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3 };

static const unsigned char totrot[16] = {
    1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28 };

static const unsigned char pc2[48] = {
    13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
    22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
    40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 };

static const uint32_t SP1[64] = {
    0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
    0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
    0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
    0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
    0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
    0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
    0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
    0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
    0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
    0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
    0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
    0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
    0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
    0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
    0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
    0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };

static const uint32_t SP2[64] = {
    0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
    0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
    0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
    0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
    0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
    0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
    0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
    0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
    0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
    0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
    0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
    0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
    0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
    0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
    0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
    0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };

static const uint32_t SP3[64] = {
    0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
    0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
    0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
    0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
    0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
    0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
    0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
    0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
    0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
    0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
    0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
    0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
    0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
    0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
    0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
    0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };

static const uint32_t SP4[64] = {
    0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
    0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
    0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
    0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
    0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
    0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
    0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
    0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
    0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
    0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
    0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
    0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
    0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
    0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
    0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
    0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };

static const uint32_t SP5[64] = {
    0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
    0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
    0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
    0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
    0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
    0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
    0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
    0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
    0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
    0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
    0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
    0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
    0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
    0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
    0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
    0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };

static const uint32_t SP6[64] = {
    0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
    0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
    0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
    0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
    0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
    0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
    0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
    0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
    0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
    0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
    0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
    0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
    0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
    0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
    0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
    0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };

static const uint32_t SP7[64] = {
    0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
    0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
    0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
    0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
    0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
    0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
    0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
    0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
    0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
    0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
    0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
    0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
    0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
    0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
    0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
    0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };

static const uint32_t SP8[64] = {
    0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
    0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
    0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
    0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
    0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
    0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
    0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
    0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
    0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
    0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
    0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
    0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
    0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
    0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
    0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
    0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };

void TC_Des::scrunch(register const char *outof, register uint32_t *into)
{
    *into    = (*outof++ & 0xffL) << 24;
    *into   |= (*outof++ & 0xffL) << 16;
    *into   |= (*outof++ & 0xffL) << 8;
    *into++ |= (*outof++ & 0xffL);
    *into    = (*outof++ & 0xffL) << 24;
    *into   |= (*outof++ & 0xffL) << 16;
    *into   |= (*outof++ & 0xffL) << 8;
    *into   |= (*outof   & 0xffL);
    return;
}

void TC_Des::unscrun(register uint32_t *outof, register char *into)
{
    *into++ = (*outof >> 24) & 0xffL;
    *into++ = (*outof >> 16) & 0xffL;
    *into++ = (*outof >>  8) & 0xffL;
    *into++ =  *outof++  & 0xffL;
    *into++ = (*outof >> 24) & 0xffL;
    *into++ = (*outof >> 16) & 0xffL;
    *into++ = (*outof >>  8) & 0xffL;
    *into   =  *outof    & 0xffL;
    return;
}

void TC_Des::desfunc(register uint32_t *block, register uint32_t *keys)
{
    register uint32_t fval, work, right, leftt;
    register int round;

    leftt = block[0];
    right = block[1];
    work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
    right ^= work;
    leftt ^= (work << 4);
    work = ((leftt >> 16) ^ right) & 0x0000ffffL;
    right ^= work;
    leftt ^= (work << 16);
    work = ((right >> 2) ^ leftt) & 0x33333333L;
    leftt ^= work;
    right ^= (work << 2);
    work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
    leftt ^= work;
    right ^= (work << 8);
    right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
    work = (leftt ^ right) & 0xaaaaaaaaL;
    leftt ^= work;
    right ^= work;
    leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;

    for( round = 0; round < 8; round++ )
	{
        work  = (right << 28) | (right >> 4);
        work ^= *keys++;
        fval  = SP7[ work        & 0x3fL];
        fval |= SP5[(work >>  8) & 0x3fL];
        fval |= SP3[(work >> 16) & 0x3fL];
        fval |= SP1[(work >> 24) & 0x3fL];
        work  = right ^ *keys++;
        fval |= SP8[ work        & 0x3fL];
        fval |= SP6[(work >>  8) & 0x3fL];
        fval |= SP4[(work >> 16) & 0x3fL];
        fval |= SP2[(work >> 24) & 0x3fL];
        leftt ^= fval;
        work  = (leftt << 28) | (leftt >> 4);
        work ^= *keys++;
        fval  = SP7[ work        & 0x3fL];
        fval |= SP5[(work >>  8) & 0x3fL];
        fval |= SP3[(work >> 16) & 0x3fL];
        fval |= SP1[(work >> 24) & 0x3fL];
        work  = leftt ^ *keys++;
        fval |= SP8[ work        & 0x3fL];
        fval |= SP6[(work >>  8) & 0x3fL];
        fval |= SP4[(work >> 16) & 0x3fL];
        fval |= SP2[(work >> 24) & 0x3fL];
        right ^= fval;
    }

    right = (right << 31) | (right >> 1);
    work = (leftt ^ right) & 0xaaaaaaaaL;
    leftt ^= work;
    right ^= work;
    leftt = (leftt << 31) | (leftt >> 1);
    work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
    right ^= work;
    leftt ^= (work << 8);
    work = ((leftt >> 2) ^ right) & 0x33333333L;
    right ^= work;
    leftt ^= (work << 2);
    work = ((right >> 16) ^ leftt) & 0x0000ffffL;
    leftt ^= work;
    right ^= (work << 16);
    work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
    leftt ^= work;
    right ^= (work << 4);
    *block++ = right;
    *block = leftt;
    return;
}

void TC_Des::cookey(register uint32_t *raw1, uint32_t *k)
{
    register uint32_t *cook, *raw0;
    uint32_t dough[32];
    register int i;

    cook = dough;
    for( i = 0; i < 16; i++, raw1++ )
	{
        raw0 = raw1++;
        *cook    = (*raw0 & 0x00fc0000L) << 6;
        *cook   |= (*raw0 & 0x00000fc0L) << 10;
        *cook   |= (*raw1 & 0x00fc0000L) >> 10;
        *cook++ |= (*raw1 & 0x00000fc0L) >> 6;
        *cook    = (*raw0 & 0x0003f000L) << 12;
        *cook   |= (*raw0 & 0x0000003fL) << 16;
        *cook   |= (*raw1 & 0x0003f000L) >> 4;
        *cook++ |= (*raw1 & 0x0000003fL);
    }

	register uint32_t *to, *endp, *p;
	to = k, endp = &k[32], p = dough;
	while( to < endp )
        *to++ = *p++;
}

void TC_Des::deskey(const char *key, short edf, uint32_t *k)
{
    register int i, j, l, m, n;
    unsigned char pc1m[56], pcr[56];
	uint32_t kn[32];

    for ( j = 0; j < 56; j++ )
	{
        l = pc1[j];
        m = l & 07;
        pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1 : 0;
    }
    for( i = 0; i < 16; i++ )
	{
        if( edf == DE1 ) m = (15 - i) << 1;
        else m = i << 1;
        n = m + 1;
        kn[m] = kn[n] = 0L;
        for( j = 0; j < 28; j++ )
		{
            l = j + totrot[i];
            if( l < 28 ) pcr[j] = pc1m[l];
            else pcr[j] = pc1m[l - 28];
        }
        for( j = 28; j < 56; j++ )
		{
            l = j + totrot[i];
            if( l < 56 ) pcr[j] = pc1m[l];
            else pcr[j] = pc1m[l - 28];
        }
        for( j = 0; j < 24; j++ )
		{
            if( pcr[pc2[j]] ) kn[m] |= bigbyte[j];
            if( pcr[pc2[j+24]] ) kn[n] |= bigbyte[j];
        }
    }
    cookey(kn, k);
}

void TC_Des::des(const char *inblock, char *outblock, uint32_t *KnL)
{
    uint32_t work[2];

    scrunch(inblock, work);
    desfunc(work, KnL);
    unscrun(work, outblock);
}

void TC_Des::des3key(const char *hexkey, short mode, uint32_t *KnL, uint32_t *KnR, uint32_t *Kn3)
{
    const char *first, *third;
    short revmod;

    if( mode == EN0 )
	{
        revmod = DE1;
        first = hexkey;
        third = &hexkey[16];
    }
    else
	{
        revmod = EN0;
        first = &hexkey[16];
        third = hexkey;
    }
    deskey(&hexkey[8], revmod, KnR);
    deskey(third, mode, Kn3);
    deskey(first, mode, KnL);
}

void TC_Des::des3(const char *from, char *into, uint32_t *KnL, uint32_t *KnR, uint32_t *Kn3)
{
	uint32_t work[2];

	scrunch(from, work);
	desfunc(work, KnL);
	desfunc(work, KnR);
	desfunc(work, Kn3);
	unscrun(work, into);
}

string TC_Des::encrypt(const char *key, const char * sIn, size_t iInlen)
{
    char des_key[8]  = {0};
    char des_input[8]  = {0};
    char des_output[8] = {0};

    //准备key
    memcpy(des_key, key, sizeof(des_key));
    uint32_t KnL[32] = {0};
    deskey(des_key, EN0, KnL);

    int mod  = iInlen % sizeof(des_input);
    size_t k     = 0;
    string ret;
    for(k = 0; k < iInlen / sizeof(des_input); k++)
    {
        memcpy(des_input, (sIn + k * sizeof(des_input)), sizeof(des_input));

        des(des_input, des_output, KnL);

        for(size_t i = 0; i < sizeof(des_output); i++)
        {
            ret += des_output[i];
        }
    }

    //设置填充值
    memset(des_input, sizeof(des_input) - mod, sizeof(des_input));
    memcpy(des_input, sIn + k * sizeof(des_input), mod);
    des(des_input, des_output, KnL);

    for(unsigned int i = 0; i < sizeof(des_output); i++)
    {
        ret += des_output[i];
    }

	return ret;
}

string TC_Des::decrypt(const char *key, const char * sIn, size_t iInlen)
{
    char des_key[8]  = {0};
    char des_input[8]  = {0};
    char des_output[8] = {0};

    //准备key
    memcpy(des_key, key, sizeof(des_key));
    uint32_t KnL[32] = {0};
    deskey(des_key, DE1, KnL);

    string ret;
    size_t k = 0;
    for(k = 0; k < iInlen / sizeof(des_input); k++)
    {
        memcpy(des_input, (sIn + k * sizeof(des_input)), sizeof(des_input));

        des(des_input, des_output, KnL);
        ret.append(des_output,sizeof(des_output));

#if 0
        for(size_t i = 0; i < sizeof(des_output); i++)
        {
            ret += des_output[i];
        }
        #endif
    }

	if(ret.length() > 0)
	{
        //去掉填充字符
		int pad = (int)(*(ret.end() - 1));
        if(pad>0 && pad<=(int)sizeof(des_output)&& (size_t)pad<=ret.length())
        {
            bool bOk=true;
            size_t iLengthBuf=ret.size();
            for(size_t iBuf=0;iBuf<(size_t)(pad-1);iBuf++)
            {
                if(ret[iLengthBuf-iBuf-2] != ret[iLengthBuf-1])
                {
                    bOk=false;
                    break;
                }
            }
            if(bOk)
                ret.erase(ret.end() - pad, ret.end());
        }
	}

	return ret;
}

string TC_Des::encrypt3(const char *key, const char * sIn, size_t iInlen)
{
    char des_key[24] = {0};
    memcpy(des_key, key, std::min(sizeof(des_key), strlen(key)));

    size_t len = (iInlen + ((size_t) 7)) & (~((size_t)7));
    size_t post_len = iInlen % 8;
    if(post_len == 0)
    {
        len += 8;
    }

    char *in_data  = new char[len];
    char *out_data = new char[len];

    memcpy(in_data, sIn, iInlen);

	//in_data不是8 的整数倍的话
	if ( post_len != 0)
	{
		for(size_t i = 0; i < 8 - post_len; i++)
		{
			*(in_data + iInlen + i) = (char)(8-post_len);
		}
	}
    else
    {
		for(size_t i = 0; i < 8; i++)
		{
			*(in_data + iInlen + i) = 0x08;
		}
    }

    uint32_t KnL[32] = {0};
    uint32_t KnR[32] = {0};
    uint32_t Kn3[32] = {0};
    des3key(des_key, EN0, KnL, KnR, Kn3);

	size_t count = len/8;

	for(size_t i = 0; i < count; ++i)
    {
        des3(in_data + i * 8, out_data + i * 8, KnL, KnR, Kn3);
    }

    string ret(out_data, len);

    delete []in_data;
    delete []out_data;

	return ret;
}

string TC_Des::decrypt3(const char *key, const char * sIn, size_t iInlen)
{
    try
    {
        char des_key[24] = {0};
        memcpy(des_key, key, min(sizeof(des_key), strlen(key)));

        size_t len = iInlen;

        char *in_data  = new char[len];
        char *out_data = new char[len];

        memcpy(in_data, sIn, iInlen);

        uint32_t KnL[32] = {0};
        uint32_t KnR[32] = {0};
        uint32_t Kn3[32] = {0};
        des3key(des_key, DE1, KnL, KnR, Kn3);

        size_t count = len/8;

        for(size_t i = 0; i < count; ++i)
        {
            des3(in_data + i * 8, out_data + i * 8, KnL, KnR, Kn3);
        }

        string ret((char*)out_data, len);

        if(ret.length() > 0)
        {
            //去掉填充字符
            int pad = (int)(*(ret.end() - 1));
            if (pad<0 || pad>(int)ret.length())
                ret.clear();
            else
                ret.erase(ret.end() - pad, ret.end());
        }

        delete []in_data;
        delete []out_data;

		return ret;
    }
    catch(TC_DES_Exception &e)
    {
        string res = "TC_Des::decrypt3 error" + string(e.what());
        throw TC_DES_Exception(res);
    }
}

}



