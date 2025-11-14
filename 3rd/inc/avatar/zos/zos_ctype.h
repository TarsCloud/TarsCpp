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
  File name     : zos_ctype.h
  Module        : zos byte/character library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the 
      byte/character interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_CTYPE_H__
#define _ZOS_CTYPE_H__

/** 
 * @file zos_ctype.h
 *
 * @brief Data structure and function definitions required by the 
 *        byte/character interface.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
/** @brief Take low 8-bit of a type as a ZUCHAR. */
#define ZOS_GET_LOW_BYTE(x) ((ZUCHAR)((x) & 0xFFL))  

/** @brief Take high 8-bit of a type as a ZUCHAR. */
#define ZOS_GET_HIGH_BYTE(x) ((ZUCHAR)((ZUSHORT)(x) >> 8))   

/** @brief Take low 16-bit of a word as a ZUSHORT. */
#define ZOS_GET_LOW_WORD(xx) ((ZUSHORT)((xx) & 0xFFFFL)) 

/** @brief Take high 16-bit of a word as a ZUSHORT. */
#define ZOS_GET_HIGH_WORD(xx) ((ZUSHORT)((ZUINT)(xx) >> 16)) 

/** @brief Set low byte of a word. */
#define ZOS_PUT_LOW_BYTE(x, c) \
    ((ZUSHORT)(((c) & 0xFFL) | ((x) & 0xFF00L)))

/** @brief Set high byte of a word. */
#define ZOS_PUT_HIGH_BYTE(x, c) \
    ((ZUSHORT)(((ZUSHORT)(c) << 8) | ((x) & 0x00FFL)))

/** @brief Set low 16-bit of word using low 16-bit of cc. */
#define ZOS_PUT_LOW_WORD(xx, cc) \
    ((ZUINT)(((cc) & 0xFFFFL) | ((xx) & 0xFFFF0000L)))    

/** @brief Set high 16-bit of word using high 16-bit of cc. */
#define ZOS_PUT_HIGH_WORD(xx, cc) \
    ((ZUINT)(((ZUINT)(cc) << 16) | ((xx) & 0x0000FFFFL))) 

/** @brief Take a and b composed of ZUSHORT, a is the high 8-bit,
 *         b is the low 8-bit. */
#define ZOS_MAKE_WORD(a, b) \
    ((ZUSHORT)(((ZUCHAR)(b)) | ((ZUSHORT)((ZUCHAR)(a))) << 8))

/** @brief Take a and b composed of ZUINT, a is the high 16-bit,
 *         and b is the low 16-bit. */
#define ZOS_MAKE_DWORD(a, b) \
    ((ZUINT)(((ZUSHORT)(b)) | ((ZUINT)((ZUSHORT)(a))) << 16))
/** @} */

#define ZU 0x00000001L               /* upper case character */
#define ZL 0x00000002L               /* lower case character */
#define ZD 0x00000004L               /* numeric digit character */ 
#define ZS 0x00000008L               /* whitespace character */
#define ZP 0x00000010L               /* punctuation character */
#define ZC 0x00000020L               /* control character */
#define ZB 0x00000040L               /* blank character */
#define ZX 0x00000080L               /* hexadecimal digit character */

/** @brief Function implement required by the byte/character interface. */
//extern ZCONST ZUCHAR g_aucZosCtype[260];

/** @{ */
/** @brief Check for upper or lower case letter. */
#define ZOS_ISALPHA(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & (ZU | ZL))

/** @brief Check for an uppercase letter. */
#define ZOS_ISUPPER(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZU)

/** @brief Check for lowercase letter.  */
#define ZOS_ISLOWER(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZL)

/** @brief Check for a digit. */
#define ZOS_ISDIGIT(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZD)

/** @brief Check for a hexadecimal digit. */
#define ZOS_ISXDIGIT(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZX)

/** @brief Check for white-space characters. */
#define ZOS_ISSPACE(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZS)

/** @brief Check for punctuation character. */
#define ZOS_ISPUNCT(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZP)

/** @brief Check for upper or lower case letter or digit. */
#define ZOS_ISALNUM(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & (ZU | ZL | ZD))

/** 
 * @brief Check for punctuation character or upper case letter  or 
 *        lower case letter or digit or blank character. 
 */
#define ZOS_ISPRINT(c) \
    ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & (ZP | ZU | ZL | ZD | ZB))

/** 
  *@brief Check for punctuation character or upper case letter  or 
  *          lower case letter or digit. 
  */
#define ZOS_ISGRAPH(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & (ZP | ZU | ZL | ZD))

/** @brief Check for control character. */
#define ZOS_ISCNTRL(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZC)

/** @brief Check for ASCII. */
#define ZOS_ISASCII(c) ((ZUCHAR)(c) <= 0x7F)

/** @brief Check for space or tab character. */
#define ZOS_ISBLANK(c) ((Zos_GetZosCType() + 1)[(ZUCHAR)(c)] & ZB) 

/** @brief Check for whitespace. */
#define ZOS_ISWS(chr) ((chr) == ' ' || (chr) == '\t')

/** @brief Check for linear whitespace. */
#define ZOS_ISLWS(chr) (ZOS_ISWS(chr) || (chr) == '\r' || (chr) == '\n')
/** @} */

/** @brief Convert the letter c to upper case. */
#define ZOS_TOUPPER(c) ((c) - 'a' + 'A')

/** @brief Convert the letter c to lower case. */
#define ZOS_TOLOWER(c) ((c) - 'A' + 'a')

/** @brief Convert the letter c to ASCII. */
#define ZOS_TOASCII(c) ((c) & 0x7F)

/** @brief Convert the letter c to digit. */
#define ZOS_TODIGIT(c) ((c) - '0')

/** @brief Check for odd. */
#define ZOS_ISODD(c) ((((c) % 2) == 1) ? ZTRUE : ZFALSE)

/** @brief Check for even. */
#define ZOS_ISEVEN(c) ((((c) % 2) == 0) ? ZTRUE : ZFALSE)

/** @brief Convert the letter c to ZCHAR type. */
#define ZOS_TOZCHAR(c) ((ZCHAR)((c) & 0x7F))

/** @brief Convert the letter c to ZUCHAR type. */
#define ZOS_TOBYTE(c) ((ZUCHAR)((c) & 0xFF))

/** @brief Convert the letter c to ZUSHORT type. */
#define ZOS_TOWORD(c) ((ZUSHORT)((c) & 0xFFFFL))

/** @brief Convert x to it's absolute value. */
#define ZOS_TOABS(x) ((x) < 0 ? -(x) : x)

/** @brief Get member offset from structure. */
#define ZOS_OFFSETOF(_type, _part) ((ZSIZE_T)&(((_type *)0)->_part))

/* get zos ctype table */
ZFUNC ZCONST ZUCHAR * Zos_GetZosCType(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_CTYPE_H__ */

