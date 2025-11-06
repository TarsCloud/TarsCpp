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
  File name     : zos_sysindep.h
  Module        : zos system independent
  Author        : Leo Lv   
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Marcos definitions required by the zos system independent.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_SYSINDEP_H__
#define _ZOS_SYSINDEP_H__

/** 
 * @file zos_sysindep.h
 *
 * @brief Marcos definitions required by the zos independent 
 *        from operating system.
 *
 * Parameter description used:
 * -# length: String length.
 * -# maxsize: Limit of destionation string buffer.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */    
/** @brief Get the maximum between a and b. */
#define ZOS_MAX(a, b) ((a) < (b) ? (b) : (a)) 

/** @brief Get the minimum between a and b. */
#define ZOS_MIN(a, b) ((a) < (b) ? (a) : (b)) 

/** @brief Mask n bit of ZUINT. */
#define ZOS_MASK(n)     ((ZUINT)1 << ((n) % (sizeof(ZUINT) * 8))) 

/** @brief Turn on bit for p.  */
#define ZOS_SET(n, p)   (p |= ZOS_MASK(n))   

/** @brief Turn off bit for p. */
#define ZOS_CLR(n, p)   (p &= ~ ZOS_MASK(n)) 

/** @brief Check bit is turn on. */
#define ZOS_ISSET(n, p) (p & ZOS_MASK(n))    

/** @brief Turn off. */
#define ZOS_ZERO(p)     (p = 0)              
/** @} */

/** @{ */ 
/** @brief Get the largest multiple of y less than x. */
#define ZOS_ROUNDDOWN(x, y) (((x) / (y)) * (y))

/** @brief Get the least multiple of y larger than x. */
#define ZOS_ROUNDUP(x, y)   ((((x) + ((y) - 1)) / (y)) * (y))

/** @brief Get the least multiple of y larger than x, y is power of 2. */
#define ZOS_ROUNDUP2(x, y)  (((x) + ((y) - 1)) & (~((y) - 1)))

/** @brief Check if x is power of 2. */
#define ZOS_ISPOWEROF2(x)   ((((x) - 1) & (x)) == 0)
/** @} */

/* zos size alignment */
#define ZOS_ALIGN(_size) ZOS_ROUNDUP2(_size, ZALIGN_SIZE)

/** @{ */ 
/** @brief Get table size. */
#define ZOS_GET_TABLE_SIZE(_table) (sizeof(_table) / sizeof(_table[0]))

/** @brief Format a talbe size and table pair. */
#define ZOS_GET_TABLE_INFO(_table) ZOS_GET_TABLE_SIZE(_table), _table

/* zos get table info */
#define ZOS_GET_TABLE_INFOX(_table) _table, ZOS_GET_TABLE_SIZE(_table)

/** @brief Shortcut of ? operator. */
#define ZOS_PLURAL(_cond, _val1, _val2) ((_cond) ? _val1 : _val2)
/** @} */

/* zos get the string empty status */
#define ZOS_ISEMPTY_STR(_str) (!_str || *_str == '\0')

/* zos get the string empty status */
#define ZOS_ISEMPTY_NSTR(_str, _len) (!_str || !_len)

/* zos get the structure string empty status */
#define ZOS_ISEMPTY_SSTR(_sstr) (!_sstr || !(_sstr)->pcStr || !(_sstr)->iLen)

/* zos get the unsigned structure string empty status */
#define ZOS_ISEMPTY_USTR(_ustr) (!_ustr || !(_ustr)->pucStr || !(_ustr)->iLen)

/* zos loop count from minimum count to maximum count(not include maximum) */
#define FOR_ALL_COUNT_INC_RANGE(_count, _mincount, _maxcount) \
  for (_count = _mincount; _count < _maxcount; _count++)

/* zos loop count from maximum count to minimum count(include minimum) */
#define FOR_ALL_COUNT_DEC_RANGE(_count, _mincount, _maxcount) \
  for (_count = _maxcount; _count >= _mincount; _count--)

/** @{ */
/** @brief Set ST_ZOS_SSTR type string using ZCHAR type string. */
#define ZOS_SSTR_SET(_dstsstr, _str, _len) do { \
    (_dstsstr)->pcStr = _str; \
    (_dstsstr)->iLen = _len; \
} while (0)

/** @brief Init ST_ZOS_SSTR type string. */
#define ZOS_SSTR_SET0(_dstsstr) do { \
    (_dstsstr)->pcStr = ZNULL; \
    (_dstsstr)->iLen = 0; \
} while (0)

/** @brief Set ST_ZOS_SSTR type string using ST_ZOS_SSTR type string. */
#define ZOS_SSTR_SETX(_dstsstr, _srcsstr) do { \
    (_dstsstr)->pcStr = (_srcsstr)->pcStr; \
    (_dstsstr)->iLen = (_srcsstr)->iLen; \
} while (0)

/** @brief Set ST_ZOS_SSTR type string using ZCHAR type string. */
#define ZOS_SSTR_SETS(_dstsstr, _str) do { \
    (_dstsstr)->pcStr = _str; \
    (_dstsstr)->iLen = ZOS_STR_LEN(_str); \
} while (0)

/** @brief Set ST_ZOS_SSTR type string using ZCHAR type string(not null). */
#define ZOS_SSTR_SETS_S(_dstsstr, _str) do { \
    (_dstsstr)->pcStr = _str; \
    (_dstsstr)->iLen = (ZUINT)Zos_StrLen(_str); \
} while (0)

/** @brief Set ST_ZOS_SSTR type string using ZUCHAR type string. */
#define ZOS_SSTR_SETU(_dstsstr, _ustr, _len) do { \
    (_dstsstr)->pcStr = (ZCHAR *)_ustr; \
    (_dstsstr)->iLen = _len; \
} while (0)

/** @brief Set ST_ZOS_SSTR type string using ST_ZOS_USTR string. */
#define ZOS_SSTR_SETUX(_dstsstr, _usstr) do { \
    (_dstsstr)->pcStr = (ZCHAR *)(_usstr)->pucStr; \
    (_dstsstr)->iLen = (_usstr)->iLen; \
} while (0)

/** @brief Set ST_ZOS_SSTR type string using ZUCHAR type string. */
#define ZOS_SSTR_SETUS(_dstsstr, _ustr) do { \
    (_dstsstr)->pcStr = (ZCHAR *)_ustr; \
    (_dstsstr)->iLen = ZOS_USTR_USLEN(_ustr); \
} while (0)

/** @brief Set ST_ZOS_SSTR type string using ZUCHAR type string(not null). */
#define ZOS_SSTR_SETUS_S(_dstsstr, _ustr) do { \
    (_dstsstr)->pcStr = (ZCHAR *)_ustr; \
    (_dstsstr)->iLen = (ZUINT)Zos_StrLen((ZCHAR *)_ustr); \
} while (0)

/** @brief Set the ST_ZOS_USTR type string using ZCHAR type string. */
#define ZOS_USSTR_SET(_dstusstr, _str, _len) do { \
    (_dstusstr)->pucStr = (ZUCHAR *)_str; \
    (_dstusstr)->iLen = _len; \
} while (0)

/** @brief Init ST_ZOS_USTR type string. */
#define ZOS_USSTR_SET0(_dstusstr) do { \
    (_dstusstr)->pucStr = ZNULL; \
    (_dstusstr)->iLen = 0; \
} while (0)

/** @brief Set the ST_ZOS_USTR type string using ST_ZOS_SSTR type string. */
#define ZOS_USSTR_SETX(_dstusstr, _srcsstr) do { \
    (_dstusstr)->pucStr = (ZUCHAR *)(_srcsstr)->pcStr; \
    (_dstusstr)->iLen = (_srcsstr)->iLen; \
} while (0)

/** @brief Set the ST_ZOS_USTR type string using ZCHAR type string. */
#define ZOS_USSTR_SETS(_dstusstr, _str) do { \
    (_dstusstr)->pucStr = (ZUCHAR *)_str; \
    (_dstusstr)->iLen = ZOS_STR_LEN(_str); \
} while (0)

/** @brief Set the ST_ZOS_USTR type string using ZCHAR type string(not null). */
#define ZOS_USSTR_SETS_S(_dstusstr, _str) do { \
    (_dstusstr)->pucStr = (ZUCHAR *)_str; \
    (_dstusstr)->iLen = (ZUINT)Zos_StrLen(_str); \
} while (0)

/** @brief Set the ST_ZOS_SSTR type string using ZUCHAR type string. */
#define ZOS_USSTR_SETU(_dststr, _ustr, _len) do { \
    (_dststr)->pucStr = _ustr; \
    (_dststr)->iLen = _len; \
} while (0)

/** @brief Set the ST_ZOS_USTR type string using ST_ZOS_USTR type string. */
#define ZOS_USSTR_SETUX(_dstusstr, _srcusstr) do { \
    (_dstusstr)->pucStr = (_srcusstr)->pucStr; \
    (_dstusstr)->iLen = (_srcusstr)->iLen; \
} while (0)

/** @brief Set the ST_ZOS_USTR type string using ZUCHAR type string. */
#define ZOS_USSTR_SETUS(_dstusstr, _ustr) do { \
    (_dstusstr)->pucStr = _ustr; \
    (_dstusstr)->iLen = ZOS_USTR_USLEN(_ustr); \
} while (0)

/** @brief Set the ST_ZOS_USTR type string using ZUCHAR typestring(not null). */
#define ZOS_USSTR_SETUS_S(_dstusstr, _ustr) do { \
    (_dstusstr)->pucStr = _ustr; \
    (_dstusstr)->iLen = (ZUINT)Zos_StrLen((ZCHAR *)_ustr); \
} while (0)

/** @} */

/** @{ */
/** @brief Get ZCHAR type string length. */
#define ZOS_STR_LEN(_str) \
    ((_str) ? Zos_StrLen(_str) : 0)

/** @brief Get ZCHAR type string ZUSHORT length. */
#define ZOS_STR_USLEN(_str) \
    (ZUSHORT)((_str) ? Zos_StrLen(_str) : 0)

/** @brief Get safe string value. */
#define ZOS_STR_SAFE(_str) \
    ((_str) ? (_str) : "")

/** @brief Get ZUCHAR type string length. */
#define ZOS_USTR_LEN(_ustr) \
    ((_ustr) ? Zos_StrLen((ZCHAR *)_ustr) : 0)

/** @brief Get ZUCHAR type string ZUSHORT length. */
#define ZOS_USTR_USLEN(_ustr) \
    (ZUSHORT)((_ustr) ? Zos_StrLen((ZCHAR *)_ustr) : 0)

/** @brief ZCHAR type string transform to "ZCHAR, ZUINT length". */
#define ZOS_NSTR(_str) \
    _str, Zos_StrLen(_str)

/** @brief ZCHAR type string transform to "ZCHAR, ZUSHORT length". */
#define ZOS_NSTR_S(_str) \
    _str, (ZUSHORT)Zos_StrLen(_str)

/** @brief ZCHAR type string transform to "ZUCHAR, ZUINT length". */
#define ZOS_NSTRX(_str) \
    (ZUCHAR *)_str, Zos_StrLen(_str)

/** @brief ZCHAR type string transform to "ZCHAR, ZUSHORT length". */
#define ZOS_NSTRX_S(_str) \
    (ZUCHAR *)_str, (ZUSHORT)Zos_StrLen(_str)

/** @brief ZUCHAR type string transform to "ZUCHAR, ZUINT length". */
#define ZOS_NUSTR(_ustr) \
    _ustr, ZOS_USTR_LEN(_ustr)

/** @brief ZUCHAR type string transform to "ZUCHAR, ZUSHORT length". */
#define ZOS_NUSTR_S(_ustr) \
    _ustr, (ZUSHORT)Zos_StrLen((ZCHAR *)_ustr)

/** @brief ZUCHAR type string transform to "ZCHAR, ZUINT length". */
#define ZOS_NUSTRX(_ustr) \
    (ZCHAR *)_ustr, ZOS_USTR_LEN(_ustr)

/** @brief ZUCHAR type string transform to "ZCHAR, ZUSHORT length". */
#define ZOS_NUSTRX_S(_ustr) \
    (ZCHAR *)_ustr, (ZUSHORT)Zos_StrLen((ZCHAR *)_ustr)

/** @brief ST_ZOS_SSTR type string transform to "ZCHAR, ZUINT length". */
#define ZOS_NSSTR(_sstr) \
    (_sstr)->pcStr, (_sstr)->iLen

/** @brief ST_ZOS_SSTR type string transform to "ZUCHAR, ZUINT length". */
#define ZOS_NSSTRX(_sstr) \
    (ZUCHAR *)(_sstr)->pcStr, (_sstr)->iLen

/** @brief ST_ZOS_USTR type string transform to "ZUCHAR, ZUINT length". */
#define ZOS_NUSSTR(_usstr) \
    (_usstr)->pucStr, (_usstr)->iLen

/** @brief ST_ZOS_USTR type string transform to "ZCHAR, ZUINT length". */
#define ZOS_NUSSTRX(_usstr) \
    (ZCHAR *)(_usstr)->pucStr, (_usstr)->iLen

/** @brief ST_ZOS_SSTR type string safety transform to "ZCHAR, ZUINT length". */
#define ZOS_NSSTR_SAFE(_sstr) \
    (_sstr) ? (_sstr)->pcStr : ZNULL, \
    (_sstr ? (_sstr)->iLen : 0)

/** @brief ST_ZOS_SSTR type string safety transform to "ZUCHAR, ZUINT length". */
#define ZOS_NSSTRX_SAFE(_sstr) \
    (_sstr) ? (ZUCHAR *)(_sstr)->pcStr : ZNULL, \
    (_sstr ? (_sstr)->iLen : 0)

/** @brief ST_ZOS_USTR type string safety transform to "ZUCHAR, ZUINT length". */
#define ZOS_NUSSTR_SAFE(_usstr) \
    (_usstr) ? (_usstr)->pucStr : ZNULL, \
    (_usstr ? (_usstr)->iLen : 0)

/** @brief ST_ZOS_USTR type string safety transform to "ZCHAR, ZUINT length". */
#define ZOS_NUSSTRX_SAFE(_usstr) \
    (_usstr) ? (ZCHAR *)(_usstr)->pucStr : ZNULL, \
    (_usstr ? (_usstr)->iLen : 0)
/** @} */

/** @{ */    
/** @brief Compare ZCHAR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_CMP(_str1, _str2) \
    Zos_NStrCmp(ZOS_NSTR(_str1), ZOS_NSTR(_str2))

/** @brief Compare ZCHAR type string and ZUCHAR type string , 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_CMPU(_str1, _ustr2) \
    Zos_NStrCmp(ZOS_NSTR(_str1), ZOS_NUSTRX(_ustr2))

/** @brief Compare ZCHAR type string and ST_ZOS_SSTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_CMPX(_str1, _sstr2) \
    Zos_NStrCmp(ZOS_NSTR(_str1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZCHAR type string and ST_ZOS_USTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_CMPUX(_str1, _usstr2) \
    Zos_NStrCmp(ZOS_NSTR(_str1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare of ZCHAR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_ICMP(_str1, _str2) \
    Zos_NStrICmp(ZOS_NSTR(_str1), ZOS_NSTR(_str2))

/** @brief Compare ZCHAR type string and ZUCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_ICMPU(_str1, _ustr2) \
    Zos_NStrICmp(ZOS_NSTR(_str1), ZOS_NUSTRX(_ustr2))

/** @brief Compare ZCHAR type  string and ST_ZOS_SSTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_ICMPX(_str1, _sstr2) \
    Zos_NStrICmp(ZOS_NSTR(_str1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZCHAR type string and ST_ZOS_USTR type string,  
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_ICMPUX(_str1, _usstr2) \
    Zos_NStrICmp(ZOS_NSTR(_str1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ZCHAR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_NCMP(_str1, _len1, _str2, _len2) \
    Zos_NStrCmp(_str1, _len1, _str2, _len2)

/** @brief Compare ZCHAR type string and ZUCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_NCMPU(_str1, _len1, _ustr2, _len2) \
    Zos_NStrCmp(_str1, _len1, (ZCHAR *)_ustr2, _len2)

/** @brief Compare ZCHAR type string and ST_ZOS_SSTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_NCMPX(_str1, _len1, _sstr2) \
    Zos_NStrCmp(_str1, _len1, ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZCHAR type string and ST_ZOS_USTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_STR_NCMPUX(_str1, _len1, _usstr2) \
    Zos_NStrCmp(_str1, _len1, ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ZCHAR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_NICMP(_str1, _len1, _str2, _len2) \
    Zos_NStrICmp(_str1, _len1, _str2, _len2)

/** @brief Compare ZCHAR type string and ZUCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_NICMPU(_str1, _len1, _ustr2, _len2) \
    Zos_NStrICmp(_str1, _len1, (ZCHAR *)_ustr2, _len2)

/** @brief Compare ZCHAR type string and ST_ZOS_SSTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_NICMPX(_str1, _len1, _sstr2) \
    Zos_NStrICmp(_str1, _len1, ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZCHAR type string and ST_ZOS_USTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_STR_NICMPUX(_str1, _len1, _usstr2) \
    Zos_NStrICmp(_str1, _len1, ZOS_NUSSTRX_SAFE(_usstr2))
/** @} */  

/** @{ */
/** @brief Copy from ZCHAR type string to ZUCHAR type string n btyes. */
#define ZOS_STR_CPYU(_dststr, _srcustr, _srclen) \
    Zos_StrNCpy(_dststr, (ZCHAR *)_srcustr, _srclen)

/** @brief Copy from ST_ZOS_SSTR type string to ZCHAR type string. */
#define ZOS_STR_CPYX(_dststr, _srcsstr) \
    Zos_NStrXCpy(_dststr, ZOS_STR_LEN(_dststr) + 1, _srcsstr)

/** @brief Copy from ST_ZOS_USTR type string to ZCHAR type string. */
#define ZOS_STR_CPYUX(_dststr, _srcusstr) \
    Zos_NStrUXCpy(_dststr, ZOS_STR_LEN(_dststr) + 1, _srcusstr)

/** @brief Copy from ZUCHAR string to ZCHAR type string n btyes with maxsize. */
#define ZOS_NSTR_CPYU(_dststr, _maxsize, _srcustr, _srclen) \
    Zos_NStrNCpy(_dststr, _maxsize, (ZCHAR *)_srcustr, _srclen)

/** @brief Copy from ST_ZOS_SSTR type string to ZCHAR type string with maxsize. */
#define ZOS_NSTR_CPYX(_dststr, _maxsize, _srcsstr) \
    Zos_NStrXCpy(_dststr, _maxsize, _srcsstr)

/** @brief Copy from ST_ZOS_USTR type string to ZCHAR type string with maxsize. */
#define ZOS_NSTR_CPYUX(_dststr, _maxsize, _srcusstr) \
    Zos_NStrUXCpy(_dststr, _maxsize, _srcusstr)
/** @} */   

/** @{ */
/** @brief Compare ST_ZOS_SSTR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_SSTR_CMP(_sstr, _str) \
    Zos_NStrCmp(ZOS_NSSTR_SAFE(_sstr), ZOS_NSTR(_str))

/** @brief Compare ST_ZOS_SSTR type string and ZUCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_SSTR_CMPU(_sstr, _ustr) \
    Zos_NStrCmp(ZOS_NSSTR_SAFE(_sstr), ZOS_NUSTRX(_ustr))

/** @brief Compare ST_ZOS_SSTR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_SSTR_ICMP(_sstr, _str) \
    Zos_NStrICmp(ZOS_NSSTR_SAFE(_sstr), ZOS_NSTR(_str))

/** @brief Compare ST_ZOS_SSTR type string and ZUCHAR type string, 
  *        by case insensitive,equal return ZOK. */
#define ZOS_SSTR_ICMPU(_sstr, _ustr) \
    Zos_NStrICmp(ZOS_NSSTR_SAFE(_sstr), ZOS_NUSTRX(_ustr))

/** @brief Compare ST_ZOS_SSTR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_SSTR_NCMP(_sstr, _str, _len) \
    Zos_NStrCmp(ZOS_NSSTR_SAFE(_sstr), _str, _len)

/** @brief Compare ST_ZOS_SSTR type string and ZUCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_SSTR_NCMPU(_sstr, _ustr, _len) \
    Zos_NStrCmp(ZOS_NSSTR_SAFE(_sstr), (ZCHAR *)_ustr, _len)

/** @brief Compare ST_ZOS_SSTR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_SSTR_NICMP(_sstr, _str, _len) \
    Zos_NStrICmp(ZOS_NSSTR_SAFE(_sstr), _str, _len)

/** @brief Compare ST_ZOS_SSTR type string and ZUCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_SSTR_NICMPU(_sstr, _ustr, _len) \
    Zos_NStrICmp(ZOS_NSSTR_SAFE(_sstr), (ZCHAR *)_ustr, _len)

/** @brief Compare ST_ZOS_SSTR type string and ST_ZOS_SSTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_SSTR_CMPX(_sstr1, _sstr2) \
    Zos_NStrCmp(ZOS_NSSTR_SAFE(_sstr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ST_ZOS_SSTR type string and ST_ZOS_USTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_SSTR_CMPUX(_sstr1, _usstr2) \
    Zos_NStrCmp(ZOS_NSSTR_SAFE(_sstr1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ST_ZOS_SSTR type string and ST_ZOS_SSTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_SSTR_ICMPX(_sstr1, _sstr2) \
    Zos_NStrICmp(ZOS_NSSTR_SAFE(_sstr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ST_ZOS_SSTR type string and ST_ZOS_USTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_SSTR_ICMPUX(_sstr1, _usstr2) \
    Zos_NStrICmp(ZOS_NSSTR_SAFE(_sstr1), ZOS_NUSSTRX_SAFE(_usstr2))
/** @} */ 

/** @{ */                 
/** @brief Copy from ZCHAR type string to ZUCHAR type string. */
#define ZOS_USTR_CPY(_dstustr, _srcstr) \
    Zos_StrCpy((ZCHAR *)_dstustr, _srcstr)

/** @brief Copy from ZUCHAR type string to ZUCHAR type string. */
#define ZOS_USTR_CPYU(_dstustr, _srcustr) \
    Zos_StrCpy((ZCHAR *)_dstustr, (ZCHAR *)_srcustr)

/** @brief Copy from ZCHAR type string to ZUCHAR type string n btyes. */
#define ZOS_USTR_NCPY(_dstustr, _srcstr, _srclen) \
    Zos_StrNCpy((ZCHAR *)_dstustr, _srcstr, _srclen)

/** @brief Copy from ZUCHAR type string to ZUCHAR type string n btyes. */
#define ZOS_USTR_NCPYU(_dstustr, _srcustr, _srclen) \
    Zos_StrNCpy((ZCHAR *)_dstustr, (ZCHAR *)_srcustr, _srclen)

/** @brief Copy from ST_ZOS_SSTR type string to ZCHAR type string. */
#define ZOS_USTR_CPYX(_dstustr, _srcsstr) \
    Zos_NStrXCpy((ZCHAR *)_dstustr, ZOS_STR_LEN(_dstustr) + 1, _srcsstr)

/** @brief Copy from ST_ZOS_USTR type string to ZCHAR type string. */
#define ZOS_USTR_CPYUX(_dstustr, _srcusstr) \
    Zos_NStrUXCpy((ZCHAR *)_dstustr, ZOS_STR_LEN(_dstustr) + 1, _srcusstr)

/** @brief Copy from ZCHAR type string to ZUCHAR type string with maxsize. */
#define ZOS_NUSTR_CPY(_dstustr, _maxsize, _srcstr) \
    Zos_NStrCpy((ZCHAR *)_dstustr, _maxsize, _srcstr)

/** @brief Copy from ZUCHAR type string to ZUCHAR type string with maxsize. */
#define ZOS_NUSTR_CPYU(_dstustr, _maxsize, _srcustr) \
    Zos_NStrCpy((ZCHAR *)_dstustr, _maxsize, (ZCHAR *)_srcustr)

/** @brief Copy from ZCHAR type string to ZUCHAR type string with maxsize. */
#define ZOS_NUSTR_NCPY(_dstustr, _maxsize, _srcstr, _srclen) \
    Zos_NStrNCpy((ZCHAR *)_dstustr, _maxsize, _srcstr, _srclen)

/** @brief Copy from ZUCHAR type string to ZUCHAR type string with maxsize. */
#define ZOS_NUSTR_NCPYU(_dstustr, _maxsize, _srcustr, _srclen) \
    Zos_NStrNCpy((ZCHAR *)_dstustr, _maxsize, (ZCHAR *)_srcustr, _srclen)

/** @brief Copy from ST_ZOS_SSTR type string to ZUCHAR type string with maxsize. */
#define ZOS_NUSTR_CPYX(_dstustr, _maxsize, _srcsstr) \
    Zos_NStrXCpy((ZCHAR *)_dstustr, _maxsize, _srcsstr)

/** @brief Copy from ST_ZOS_USTR type string to ZCHAR type string with maxsize. */
#define ZOS_NUSTR_CPYUX(_dststr, _maxsize, _srcusstr) \
    Zos_NStrUXCpy((ZCHAR *)_dststr, _maxsize, _srcusstr)
/** @} */

/** @{ */ 
/** @brief Compare ZUCHAR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_CMP(_ustr1, _str2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), ZOS_NSTR(_str2))

/** @brief Compare ZUCHAR type string and ZUCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_CMPU(_ustr1, _ustr2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), ZOS_NUSTRX(_ustr2))

/** @brief Compare ZUCHAR type string and ST_ZOS_SSTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_CMPX(_ustr1, _sstr2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZUCHAR type string and ST_ZOS_USTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_CMPUX(_ustr1, _usstr2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ZUCHAR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_ICMP(_ustr1, _str2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), ZOS_NSTR(_str2))

/** @brief Compare ZUCHAR type string and ZUCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_ICMPU(_ustr1, _ustr2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), ZOS_NUSTRX(_ustr2))

/** @brief Compare ZUCHAR type string and ST_ZOS_SSTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_ICMPX(_ustr1, _sstr2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZUCHAR type string and ST_ZOS_USTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_ICMPUX(_ustr1, _usstr2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ZUCHAR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_NCMP(_ustr1, _str2, _len2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), _str2, _len2)

/** @brief Compare ZUCHAR type string and ZUCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_NCMPU(_ustr1, _ustr2, _len2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), (ZCHAR *)_ustr2, _len2)

/** @brief Compare ZUCHAR type string and ST_ZOS_SSTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_NCMPX(_ustr1, _sstr2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZUCHAR type string and ST_ZOS_USTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USTR_NCMPUX(_ustr1, _usstr2) \
    Zos_NStrCmp(ZOS_NUSTRX(_ustr1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ZUCHAR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_NICMP(_ustr1, _str2, _len2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), _str2, _len2)

/** @brief Compare ZUCHAR type string and ZUCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_NICMPU(_ustr1, _ustr2, _len2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), (ZCHAR *)_ustr2, _len2)

/** @brief Compare ZUCHAR type string and ST_ZOS_SSTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_NICMPX(_ustr1, _sstr2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ZUCHAR type string and ST_ZOS_USTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USTR_NICMPUX(_ustr1, _usstr2) \
    Zos_NStrICmp(ZOS_NUSTRX(_ustr1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ST_ZOS_USTR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USSTR_CMP(_usstr, _str) \
    Zos_NStrCmp(ZOS_NUSSTRX_SAFE(_usstr), ZOS_NSTR(_str))

/** @brief Compare ST_ZOS_USTR type string and ZUCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USSTR_CMPU(_usstr, _ustr) \
    Zos_NStrCmp(ZOS_NUSSTRX_SAFE(_usstr), ZOS_NUSTRX(_ustr))

/** @brief Compare ST_ZOS_USTR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USSTR_ICMP(_usstr, _str) \
    Zos_NStrICmp(ZOS_NUSSTRX_SAFE(_usstr), ZOS_NSTR(_str))

/** @brief Compare ST_ZOS_USTR type string and ZUCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USSTR_ICMPU(_usstr, _ustr) \
    Zos_NStrICmp(ZOS_NUSSTRX_SAFE(_usstr), ZOS_NUSTRX(_ustr))

/** @brief Compare ST_ZOS_USTR type string and ZCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USSTR_NCMP(_usstr, _str, _len) \
    Zos_NStrCmp(ZOS_NUSSTRX_SAFE(_usstr), _str, _len)

/** @brief Compare ST_ZOS_USTR type string and ZUCHAR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USSTR_NCMPU(_usstr, _ustr, _len) \
    Zos_NStrCmp(ZOS_NUSSTRX_SAFE(_usstr), (ZCHAR *)_ustr, _len)

/** @brief Compare ST_ZOS_USTR type string and ZCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USSTR_NICMP(_usstr, _str, _len) \
    Zos_NStrICmp(ZOS_NUSSTRX_SAFE(_usstr), _str, _len)

/** @brief Compare ST_ZOS_USTR type string and ZUCHAR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USSTR_NICMPU(_usstr, _ustr, _len) \
    Zos_NStrICmp(ZOS_NUSSTRX_SAFE(_usstr), (ZCHAR *)_ustr, _len)

/** @brief Compare ST_ZOS_USTR type string and ST_ZOS_SSTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USSTR_CMPX(_usstr1, _sstr2) \
    Zos_NStrCmp(ZOS_NUSSTRX_SAFE(_usstr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ST_ZOS_USTR type string and ST_ZOS_USTR type string, 
  *        by case sensitive, equal return ZOK. */
#define ZOS_USSTR_CMPUX(_usstr1, _usstr2) \
    Zos_NStrCmp(ZOS_NUSSTRX_SAFE(_usstr1), ZOS_NUSSTRX_SAFE(_usstr2))

/** @brief Compare ST_ZOS_USTR type string and ST_ZOS_SSTR type string, 
  *            by case insensitive, equal return ZOK. */
#define ZOS_USSTR_ICMPX(_usstr1, _sstr2) \
    Zos_NStrICmp(ZOS_NUSSTRX_SAFE(_usstr1), ZOS_NSSTR_SAFE(_sstr2))

/** @brief Compare ST_ZOS_USTR type string and ST_ZOS_USTR type string, 
  *        by case insensitive, equal return ZOK. */
#define ZOS_USSTR_ICMPUX(_usstr1, _usstr2) \
    Zos_NStrICmp(ZOS_NUSSTRX_SAFE(_usstr1), ZOS_NUSSTRX_SAFE(_usstr2))
/** @} */

/** @{ */  
/** @brief Trim left of ZUCHAR type string. */
#define ZOS_USTR_TRIM_LEFT(_ustr, _len) \
    Zos_TrimLeft((ZCHAR **)_ustr, _len, ZTRUE)

/** @brief Trim right of ZUCHAR type string. */
#define ZOS_USTR_TRIM_RIGHT(_ustr, _len) \
    Zos_TrimRight((ZCHAR **)_ustr, _len, ZTRUE)

/** @brief Trim left of ST_ZOS_SSTR type string. */
#define ZOS_SSTR_TRIM_LEFT(_sstr) \
    Zos_TrimLeft(&(_sstr)->pcStr, &(_sstr)->iLen, ZTRUE)

/** @brief Trim right of ST_ZOS_SSTR type string. */
#define ZOS_SSTR_TRIM_RIGHT(_sstr) \
    Zos_TrimRight(&(_sstr)->pcStr, &(_sstr)->iLen, ZTRUE)

/** @brief Trim left of ST_ZOS_USTR type string. */
#define ZOS_USSTR_TRIM_LEFT(_usstr) \
    Zos_TrimLeft((ZCHAR **)&(_usstr)->pucStr, &(_usstr)->iLen, ZTRUE)

/** @brief Trim right of ST_ZOS_USTR type string. */
#define ZOS_USSTR_TRIM_RIGHT(_usstr) \
    Zos_TrimRight((ZCHAR **)&(_usstr)->pucStr, &(_usstr)->iLen, ZTRUE)

/** @brief Convert ZUCHAR type string to ZULONG value. */
#define ZOS_USTR_TO_UL(_ustr, _len, _data) \
    Zos_StrToUl((ZCHAR *)_ustr, _len, _data)

/** @brief Convert ZUCHAR type string to ZUINT value. */
#define ZOS_USTR_TO_UI(_ustr, _len, _data) \
    Zos_StrToUint((ZCHAR *)_ustr, _len, _data)

/** @brief Convert ZUCHAR type string to ZUSHORT value. */
#define ZOS_USTR_TO_US(_ustr, _len, _data) \
    Zos_StrToUs((ZCHAR *)_ustr, _len, _data)

/** @brief Convert ZUCHAR type string to ZUCHAR value. */
#define ZOS_USTR_TO_UC(_ustr, _len, _data) \
    Zos_StrToUc((ZCHAR *)_ustr, _len, _data)

/** @brief Convert ST_ZOS_SSTR type string to ZULONG value. */
#define ZOS_SSTR_TO_UL(_sstr, _data) \
    Zos_StrToUl(ZOS_NSSTR_SAFE(_sstr), _data)

/** @brief Convert ST_ZOS_SSTR type string to ZUINT value. */
#define ZOS_SSTR_TO_UI(_sstr, _data) \
    Zos_StrToUint(ZOS_NSSTR_SAFE(_sstr), _data)

/** @brief Convert ST_ZOS_SSTR type string to ZUSHORT value. */
#define ZOS_SSTR_TO_US(_sstr, _data) \
    Zos_StrToUs(ZOS_NSSTR_SAFE(_sstr), _data)

/** @brief Convert ST_ZOS_SSTR type string to ZUCHAR value. */
#define ZOS_SSTR_TO_UC(_sstr, _data) \
    Zos_StrToUc(ZOS_NSSTR_SAFE(_sstr), _data)

/** @brief Convert ST_ZOS_USTR type string to ZULONG value. */
#define ZOS_USSTR_TO_UL(_usstr, _data) \
    Zos_StrToUl(ZOS_NUSSTRX_SAFE(_usstr), _data)

/** @brief Convert ST_ZOS_USTR type string to ZUINT value. */
#define ZOS_USSTR_TO_UI(_usstr, _data) \
    Zos_StrToUint(ZOS_NUSSTRX_SAFE(_usstr), _data)

/** @brief Convert ST_ZOS_USTR type string to ZUSHORT value. */
#define ZOS_USSTR_TO_US(_usstr, _data) \
    Zos_StrToUs(ZOS_NUSSTRX_SAFE(_usstr), _data)

/** @brief Convert ST_ZOS_USTR type string to ZUCHAR value. */
#define ZOS_USSTR_TO_UC(_usstr, _data) \
    Zos_StrToUc(ZOS_NUSSTRX_SAFE(_usstr), _data)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_SYSINDEP_H__ */

