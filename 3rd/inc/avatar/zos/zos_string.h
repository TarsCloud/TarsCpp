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
  File name     : zos_string.h
  Module        : zos string library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the string interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_STRING_H__
#define _ZOS_STRING_H__

/** 
 * @file zos_string.h
 * @brief Zos string interfaces functions.
 *  
 * In this file, it defined the operation function
 * of string, including string compare, string copy, string convert
 * and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* zos string default max length of string */
#define ZSTRUL_MAXLEN 32
#define ZSTRUS_MAXLEN 16
#define ZSTRUC_MAXLEN 8

/** 
 * @brief Set the first zSize bytes in a memory block from a location 
 *        denoted by pMem to iContent. The memory block starts from 
 *        a location denoted by pMem.
 *
 * @param [in] pMem The address from which the memory block starts. 
 *                  If the address is ZNULL, then Zos_MemSetwill return ZNULL.
 * @param [in] iContent The first zSize bytes will be set to iContent.
 * @param [in] zSize Indicates how many bytes will be set to iContent.
 *
 * @return The pointer to the memory block pMem on success, 
 *         or ZNULL on failure.
 */ 
ZFUNC ZVOID * Zos_MemSet(ZVOID *pMem, ZUINT iContent, ZSIZE_T zSize);

/** 
 * @brief Copies the first zSize bytes in memory block pSrc (the source memory) 
 *        to another memory block pDest (the destination memory block). 
 *        These two blocks can not overlap over each other. If they do,
 *        please use function Zos_MemMove.
 *
 * @param [in] pDest The pointer to the memory address where the destination 
 *                   memory block starts. If the address is ZNULL
 *                   function Zos_MemCpy will return ZNULL.
 * @param [in] pSrc The pointer to the memory address where the source memory 
 *                  block starts. If the address is ZNULL function 
 *                  Zos_MemCpy will return ZNULL.
 * @param [in] zSize The number of bytes which are to be copied 
 *                   to memory block pDest.
 *
 * @return The pointer to memory block pDest on success, or ZNULL on failure. 
 *
 * @see Zos_MemMove
 */
ZFUNC ZVOID * Zos_MemCpy(ZVOID *pDest, ZCONST ZVOID *pSrc, 
                ZSIZE_T zSize);

/** 
 * @brief Copies the first zSize bytes in memory block pSrc (the source memory) 
 *        to another memory block pDest (the destination memory block). 
 *        These two blocks can overlap over each other.
 *
 * @param [in] pDest The pointer to the memory address where the destination 
 *                   memory block starts. If the address is ZNULL
 *                   function Zos_MemCpy will return ZNULL.
 * @param [in] pSrc The pointer to the memory address where the source memory 
 *                  block starts. If the address is ZNULL function Zos_MemCpy 
 *                  will return ZNULL.
 * @param [in] zSize The number of bytes which are to be copied to 
 *                   memory block pDest.
 *
 * @return The pointer to memory block pDest on success, or ZNULL on failure.
 *
 * @see Zos_MemSet
 */
ZFUNC ZVOID * Zos_MemMove(ZVOID *pDest, ZCONST ZVOID *pSrc, 
                ZSIZE_T zSize);

/** 
 * @brief Compares the first zSize bytes in memory block pMem1 with the first 
 *        zSize bytes in memory block pMem2.When the first different pair has 
 *        found, Zos_MemCmp will return the difference between the byte pMem1 
 *        in and that in pMem2. If there is no different pair, Zos_MemCmp will 
 *        return 0.
 *
 * @param [in] pMem1 The pointer to the address where the memory block pMem1 
 *                   starts. If this parameter is ZNULL, then the function will 
 *                   return ZNULL.
 * @param [in] pMem2 The pointer the the address where the memory block pMem2 
 *                   starts. If this parameter is ZNULL, then the function will
 *                   return ZNULL
 * @param [in] zSize The first zSize bytes in each the memory block 
 *                   will be compared.
 *
 * @return The comparison result, or ZNULL if the input parameter pMem1 or 
 *         pMem2 is ZNULL.
 *
 * @see @ref Zos_StrCmp
 */
ZFUNC ZINT Zos_MemCmp(ZCONST ZVOID *pMem1, ZCONST ZVOID *pMem2, 
                ZSIZE_T zSize);

/** 
 * @brief Caculates the length of character string pcStr, not including the 
 *        ending character.
 *
 * @param [in] pcStr The pointer to the character string. If it is ZNULL
 *                   Zos_StrLen will return ZNULL.
 *
 * @return The length of the character string, or ZNULL if the input parameter 
 *         pcStr is ZNULL.
 *
 * @see @ref Zos_StrCpy
 */
ZFUNC ZINT Zos_StrLen(ZCONST ZCHAR *pcStr);

/** 
 * @brief Copies the charater string (including the ending character) denoted 
 *        by pcSrc to the character string denoted pcDst. If the two strings 
 *        overlap, the result of copying is uncertain. 
 * @param [in] pcDst The pointer to the string pcDst. If it is ZNULL, the 
 *                   function will return ZNULL. 
 * @param [in] pcSrc The pointer to the string pcSrc. If it is ZNULL, the 
 *                    function will return ZNULL.
 *
 * @return The pointer to the string pcDst on success, or 
 *         ZNULL if the input parameter pcDst or pcSrc is ZNULL.
 *
 * @see @ref Zos_MemCpy
 */
ZFUNC ZCHAR * Zos_StrCpy(ZCHAR *pcDst, ZCONST ZCHAR *pcSrc);

/** 
 * @brief Copis the first iCount character in string pcSrc to string pcDst.
 *
 * @param [in] pcDst The pointer to the character string pcDst to which the 
 *                   characters will be copied. If the pointer is ZNULL, 
 *                   the function will return ZNULL. 
 * @param [in] pcSrc The pointer to the character string pcSrc whose characters 
 *                   will be copied to another string pcDst. If the pointer is 
 *                   ZNULL, the function will return ZNULL.
 * @param [in] iCount Indicates iCount characters will be copied.
 *
 * @return The string pointer pcDst on success, or ZNULL if the input 
 *         parameter pcDstor pcSrc is ZNULL.
 *
 * @see @ref Zos_StrCpy
 */
ZFUNC ZCHAR * Zos_StrNCpy(ZCHAR *pcDst, ZCONST ZCHAR *pcSrc, ZINT iCount);

/** 
 * @brief Copies string pcAppend to the end of string, overlaying the ending 
 *         character of pcDst. 
 * @param [in] pcDst The pointer to string pcDst. If this pointer is ZNULL,
 *                   function Zos_StrCat will return ZNULL. 
 * @param [in] pcAppend The pointer to string pcAppend. If this pointer is 
 *                      ZNULL, function Zos_StrCat will return ZNULL
 *
 * @return The pointer pcDst on success, or ZNULL
 *  if the pcDst or pcAppend is ZNULL, or not enough memory.
 *
 * @see @ref Zos_StrCpy
 */
ZFUNC ZCHAR * Zos_StrCat(ZCHAR *pcDst, ZCONST ZCHAR *pcAppend);

/** 
 * @brief Get the fisrt string occurrence of the character iChr in pcStr
 * @param [in] pcStr The pointer to string. 
 * @param [in] iChr The character for match. 
 *
 * @return The pointer first string position on success, 
 * or ZNULL if no character in string.
 *
 * @see @ref Zos_StrStr
 */
ZFUNC ZCHAR * Zos_StrChr(ZCONST ZCHAR *pcStr, ZINT iChr);

/** 
 * @brief Get the fisrt string occurrence of the character iChr in pcStr before pcBoundary.
 * @param [in] pcStr The pointer to string. 
 * @param [in] pcBoundary The pointer to boundary string. 
 * @param [in] iChr The character for match. 
 *
 * @return The pointer first string position on success, 
 * or ZNULL if no character in string.
 *
 * @see @ref Zos_StrChr
 */
ZFUNC ZCHAR * Zos_StrChrB(ZCONST ZCHAR *pcStr, ZCONST ZCHAR *pcBoundary,
                ZINT iChr);

/** 
 * @brief Get the fisrt string occurrence of the character iChr in pcStr in iLen range.
 * @param [in] pcStr The pointer to string. 
 * @param [in] iLen The boundary string length. 
 * @param [in] iChr The character for match. 
 *
 * @return The pointer first string position on success, 
 * or ZNULL if no character in string.
 *
 * @see @ref Zos_StrChr
 */
ZFUNC ZCHAR * Zos_StrChrN(ZCONST ZCHAR *pcStr, ZUINT iLen, ZINT iChr);

/**
 * @brief Locate the last occurrence of character in string.
 * @param  pcStr The pointer to string terminated with null-character 
 * @param  iChr  Character to be located.
 * @return       A pointer to the last occurrence of character in pcStr.
 *               If not found, the function return ZNULL.
 */
ZFUNC ZCHAR * Zos_StrRChr(ZCONST ZCHAR *pcStr, ZINT iChr);

/**
 * @brief Locate the last occurrence of character in string.
 * @param  pcStr The pointer to string.
 * @param  iLen  The string length in bytes.
 * @param  iChr  Character to be located.
 * @return       A pointer to the last occurrence of character in pcStr.
 *               If not found, the function return ZNULL.
 */
ZFUNC ZCHAR * Zos_StrRChrN(ZCONST ZCHAR *pcStr, ZUINT iLen, ZINT iChr);

/** 
 * @brief Get the fisrt string occurrence of the sub-string pcSubStr in pcStr
 * @param [in] pcStr The pointer to string. 
 * @param [in] pcSubStr The pointer to sub-string. 
 *
 * @return The pointer first string position on success, or ZNULL if no sub-string.
 *
 * @see @ref Zos_StrChr 
 */
ZFUNC ZCHAR * Zos_StrStr(ZCONST ZCHAR *pcStr, ZCONST ZCHAR *pcSubStr);

/** 
 * @brief Get the fisrt string occurrence of the sub-string pcSubStr in pcStr.
 *        This comparison is not case-sensitive.
 * @param [in] pcStr The pointer to string. 
 * @param [in] pcSubStr The pointer to sub-string. 
 *
 * @return The pointer first string position on success, or ZNULL if no sub-string.
 *
 * @see @ref Zos_StrStr 
 */
ZFUNC ZCHAR * Zos_StrIStr(ZCONST ZCHAR *pcStr, ZCONST ZCHAR *pcSubStr);

/** 
 * @brief Get the fisrt string occurrence of the sub-string pcSubStr in pcStr
 * @param [in] pcStr The pointer to string. 
 * @param [in] pcBoundary The pointer to boundary string. 
 * @param [in] pcSubStr The pointer to sub-string. 
 *
 * @return The pointer first string position on success, or ZNULL if no sub-string.
 *
 * @see @ref Zos_StrStr 
 */
ZFUNC ZCHAR * Zos_StrStrB(ZCHAR *pcStr, ZCHAR *pcBoundary, ZCHAR *pcSubStr);

/** 
 * @brief Get the fisrt string occurrence of the sub-string pcSubStr in pcStr
 * @param [in] pcStr The pointer to string. 
 * @param [in] iLen The boundary string length. 
 * @param [in] pcSubStr The pointer to sub-string. 
 *
 * @return The pointer first string position on success, or ZNULL if no sub-string.
 *
 * @see @ref Zos_StrStr 
 */
ZFUNC ZCHAR * Zos_StrStrN(ZCHAR *pcStr, ZUINT iLen, ZCHAR *pcSubStr);

/** 
 * @brief Get the fisrt string occurrence of the sub-string pcSubStr in pcStr
 *        This comparison is not case-sensitive.
 * @param [in] pcStr The pointer to string. 
 * @param [in] iLen The boundary string length. 
 * @param [in] pcSubStr The pointer to sub-string. 
 *
 * @return The pointer first string position on success, or ZNULL if no sub-string.
 *
 * @see @ref Zos_StrStr 
 */
 ZFUNC ZCHAR * Zos_StrIStrN(ZCHAR *pcStr, ZUINT iLen, ZCHAR *pcSubStr);

/** 
 * @brief Compares character string pcStr1 with character string pcStr2 and 
 *         returns a negative number if string pcStr1 is smaller than string 
 *         pcStr2, or 0 if they are equal, or a positive number if string pcStr1
 *         is greater than string pcStr2. This comparison is case sensitive. 
 * @param [in] pcStr1 The pointer to string pcStr1. 
 * @param [in] pcStr2 The pointer to string pcStr2.
 *
 * @return A negative number if string pcStr1 is smaller than string pcStr2, 
 *         or 0 if they are equal, or a positive number if string pcStr1 is 
 *         greater than string pcStr2.
 *
 * @see @ref Zos_StrNCmp
 */
ZFUNC ZINT Zos_StrCmp(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2);

/** 
 * @brief Compares character string pcStr1 and character string pcStr2. The 
 *         difference from Zos_StrCmp is that Zos_StrNCmp compares zSize pairs 
 *         of characters at most for each invoking. If the first zSize 
 *         characters of string  pcStr1 are the same as that of string pcStr2, 
 *         Zos_StrNCmp will return 0. This comparison is case sensitive. 
 *
 * @param [in] pcStr1 The pointer to string pcStr1.
 * @param [in] pcStr2 The pointer to string pcStr2.
 * @param [in] zSize The maximum number of character pairs Zos_StrNCmp 
 *                   may compare.
 *
 * @return A negative number if string pcStr1 is smaller than string pcStr2, 
 *         or 0 if they are equal, or a positive number if string pcStr1 is 
 *         greater than string pcStr2, and ZNULL if the input parameter 
 *         pcStr1 or pcStr2.
 *
 * @see @ref Zos_StrCmp
 */
ZFUNC ZINT Zos_StrNCmp(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2,
                ZSIZE_T zSize);

/** 
 * @brief Compares character string pcStr1 with character string pcStr2. 
 *         If these two string are the same, Zos_StrICmp will return 0, or -1. 
 *         This comparison is not case sensitive. 
 *
 * @param [in] pcStr1 The pointer to character string pcStr1.
 * @param [in] pcStr2 The pointer to character string pcStr2.
 *
 *
 * @return 0 if the two strings are the same, or -1 if they are not, 
 *         or -1 if the input parameter pcStr1 or pcStr2 is ZNULL.
 *
 * @see @ref Zos_StrNICmp
 */
ZFUNC ZINT Zos_StrICmp(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2);

/** 
 * @brief Compares character string pcStr1 with character string pcStr2. 
 *         If these two string are the same, Zos_StrICmp will return 0, or -1. 
 *         This comparison is not case sensitive. 
 *
 * @param [in] pcStr1 The pointer to character string pcStr1.
 * @param [in] pcStr2 The pointer to character string pcStr2.
 * @param [in] zSize Indicates Zos_StrNICmp may compare 
 *                   zSize pairs of characters at most.
 *
 *
 * @return 0 if the two strings are the same, or -1 if they are not, 
 *         or -1 if the input parameter pcStr1 or pcStr2 is ZNULL.
 *
 * @see @ref Zos_StrICmp
 */
ZFUNC ZINT Zos_StrNICmp(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2,
                ZSIZE_T zSize);

/** 
 * @brief Searches Token in a character string. 
 *
 * @param [in] pcStr The pointer to the character string.
 * @param [in] pcDelim The pointer to a group of separator characters.
 * @param [out] ppLast The last character that has been scanned.
 *
 *
 * @return The pointer to the Token if it has found, or ZNULL if not.
 */
ZFUNC ZCHAR * Zos_StrTok(ZCHAR *pcStr, ZCONST ZCHAR *pcDelim, 
                ZCHAR **ppLast);

/** 
 * @brief Zeroes a memory block. 
 *
 * @param [in] pMem The memory block.
 * @param [in] zSize Size of the memory block which is to be zeroed.
 *
 * @return [none]
 *
 * @see @ref Zos_MemSet
 */
ZFUNC ZVOID Zos_ZeroMem(ZVOID *pMem, ZSIZE_T zSize);

/** 
 * @brief Trims string by deleting the left and right whitespace.
 *
 * @param [in, out] ppcStr The string address.The address of the changed string.
 * @param [in, out] piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_TrimX
 */
ZFUNC ZVOID Zos_Trim(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Trims string by deleting the left and right whitespace.
 *
 * @param [in,out] ppcStr The string address.The address of the changed string.
 * @param [in,out] piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_Trim
 */
ZFUNC ZVOID Zos_TrimX(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Trims string by deleting the left whitespace.
 *
 * @param [in, out] ppcStr The string address.The address of the changed string.
 * @param [in, out] piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_TrimRight
 */
ZFUNC ZVOID Zos_TrimLeft(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Trims string by deleting the left whitespace.
 *
 * @param [in,out] ppcStr The string address.The address of the changed string.
 * @param [in,out] piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_TrimLeft
 */
ZFUNC ZVOID Zos_TrimLeftX(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Trims string by deleting the right whitespace.
 *
 * @param [in,out] ppcStr The string address.The address of the changed string.
 * @param [in,out] piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_TrimLeft
 */
ZFUNC ZVOID Zos_TrimRight(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Trims string by deleting the right whitespace.
 *
 * @param [in,out] ppcStr The string address.The address of the changed string.
 * @param [in,out] piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_TrimRight
 */
ZFUNC ZVOID Zos_TrimRightX(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Trims string by deleting the left and right whitespace.
 *
 * @param [in,out] **ppcStr The string address.The address of the changed string.
 * @param [in,out] *piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_TrimAllX
 */
ZFUNC ZVOID Zos_TrimAll(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Trims string by deleting the left and right whitespace.
 *
 * @param [in,out] ppcStr The string address.The address of the changed string.
 * @param [in,out] piStrLen The address of the string length.
 *                           The address of the changed string length.
 * @param [in] bLWS Trim flag.
 *
 * @return [none]
 *
 * @see @ref Zos_TrimAll
 */
ZFUNC ZVOID Zos_TrimAllX(ZCHAR **ppcStr, ZUINT *piStrLen, ZBOOL bLWS); 

/** 
 * @brief Conver string to lower case string.
 *
 * @param [in] pcStr The string.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_Str2Upper
 */
ZFUNC ZINT Zos_Str2Lower(ZCHAR *pcStr); 

/** 
 * @brief Conver string to upper case string.
 *
 * @param [in] pcStr The string.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_Str2Lower
 */
ZFUNC ZINT Zos_Str2Upper(ZCHAR *pcStr); 

/** 
 * @brief Conver string to normal printable string.
 *
 * @param [in] pcStr The string.
 * @param [in] ppcNps The normal printable string.
 * The no printable character will be converted as '.'.
 * The string should be freed by Zos_SysStrFree.
 *
 * @return ZOK on success, or ZFAILED on failure.
 */
ZFUNC ZINT Zos_Str2NPS(ZCHAR *pcStr, ZCHAR **ppcNps); 

/** 
 * @brief Conver string with length to lower case string.
 *
 * @param [in] pcStr The string.
 * @param [in] wLen The length of string.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_NStr2Upper
 */
ZFUNC ZINT Zos_NStr2Lower(ZCHAR *pcStr, ZUINT iLen); 

/** 
 * @brief Conver string with length to upper case string.
 *
 * @param [in] pcStr The string.
 * @param [in] wLen The length of string.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_NStr2Lower
 */
ZFUNC ZINT Zos_NStr2Upper(ZCHAR *pcStr, ZUINT iLen); 

/** 
 * @brief A comparison the head or tail of a string. 
 *        This comparison is case-sensitive.
 *
 * @param [in] pcStr The string.
 * @param [in] pcPart Head or tail string.
 * @param [in] bHeadCmp Head string flag.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrHTICmp
 */
ZFUNC ZINT Zos_StrHTCmp(ZCHAR *pcStr, ZCHAR *pcPart, ZBOOL bHeadCmp); 

/** 
 * @brief A comparison the head or tail of a string. 
 *        This comparison is not case-sensitive.
 *
 * @param [in] pcStr The string.
 * @param [in] pcPart Head or tail string.
 * @param [in] bHeadCmp Head string flag.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrHTCmp
 */
ZFUNC ZINT Zos_StrHTICmp(ZCONST ZCHAR *pcStr, ZCONST ZCHAR *pcPart,
                ZBOOL bHeadCmp); 

/** 
 * @brief A comparison the head or tail of a string. 
 *        This comparison is case-sensitive.
 *
 * @param [in] pcStr The string.
 * @param [in] wLen Length of string.
 * @param [in] pcPart Head or tail string.
 * @param [in] bHeadCmp Head string flag.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_NStrHTICmp
 */
ZFUNC ZINT Zos_NStrHTCmp(ZCHAR *pcStr, ZUINT iLen, 
                ZCHAR *pcPart, ZBOOL bHeadCmp); 

/** 
 * @brief A comparison the head or tail of a string. 
 *        This comparison is not case-sensitive.
 *
 * @param [in] pcStr The string.
 * @param [in] wLen Length of string.
 * @param [in] pcPart Head or tail string.
 * @param [in] bHeadCmp Head string flag.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_NStrHTCmp
 */
ZFUNC ZINT Zos_NStrHTICmp(ZCONST ZCHAR *pcStr, ZUINT iLen, 
                ZCONST ZCHAR *pcPart, ZBOOL bHeadCmp); 

/** 
 * @brief A comparison between two strings. This comparison is case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] pcStr2 String 2.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrICmpL
 */
ZFUNC ZINT Zos_StrCmpL(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2); 

/** 
 * @brief A comparison between two strings. 
 *        This comparison is not case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] pcStr2 String 2.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrCmpL
 */
ZFUNC ZINT Zos_StrICmpL(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2); 

/** 
 * @brief A comparison between two strings. This comparison is case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] pcStr2 String 2.
 * @param [in] wLen2 The length of string 2.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrICmpN
 */
ZFUNC ZINT Zos_StrCmpN(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2, ZUINT iLen2); 

/** 
 * @brief A comparison between two strings. This comparison is not 
 *        case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] pcStr2 String 2.
 * @param [in] wLen2 The length of string 2.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrCmpN
 */
ZFUNC ZINT Zos_StrICmpN(ZCONST ZCHAR *pcStr1, ZCONST ZCHAR *pcStr2, ZUINT iLen2); 

/** 
 * @brief A comparison between two strings. This comparison is case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] pstStr2 A signed structure string.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrICmpX
 */
ZFUNC ZINT Zos_StrCmpX(ZCONST ZCHAR *pcStr1, ST_ZOS_SSTR *pstStr2); 

/** 
 * @brief A comparison between two strings. 
 *        This comparison is not case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] pstStr2 A signed structure string.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_StrCmpX
 */
ZFUNC ZINT Zos_StrICmpX(ZCONST ZCHAR *pcStr1, ST_ZOS_SSTR *pstStr2); 

/** 
 * @brief A comparison between two strings. 
 *        This comparison is case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] wLen1 The length of string 1.
 * @param [in] pcStr2 String 2.
 * @param [in] wLen2 The length of string 2.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_NStrICmp
 */
ZFUNC ZINT Zos_NStrCmp(ZCONST ZCHAR *pcStr1, ZUINT iLen1, 
                ZCONST ZCHAR *pcStr2, ZUINT iLen2); 

/** 
 * @brief A comparison between two strings. 
 *        This comparison is not case-sensitive.
 *
 * @param [in] pcStr1 String 1.
 * @param [in] wLen1 The length of string 1.
 * @param [in] pcStr2 String 2.
 * @param [in] wLen2 The length of string 2.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see @ref Zos_NStrCmp
 */
ZFUNC ZINT Zos_NStrICmp(ZCONST ZCHAR *pcStr1, ZUINT iLen1, 
                ZCONST ZCHAR *pcStr2, ZUINT iLen2); 

/** 
 * @brief Copy string to a limited buffer.
 *
 * @param [in] pcDstStr The address of the destination string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * @param [in] pcSrcStr The source string.
 *
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_NStrNCpy
 */
ZFUNC ZINT Zos_NStrCpy(ZCHAR *pcDstStr, ZUINT iMaxSize,
                ZCONST ZCHAR *pcSrcStr); 

/** 
 * @brief Copy a string with length to a size limited buffer. 
 *
 * @param [in] pcDstStr The address of the destination string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * @param [in] pcSrcStr The source string.
 * @param [in] wSrcLen Length of the source string.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_NStrCpy
 */
ZFUNC ZINT Zos_NStrNCpy(ZCHAR *pcDstStr, ZUINT iMaxSize, 
                ZCONST ZCHAR *pcSrcStr, ZUINT iSrcLen); 

/** 
 * @brief Copy a signed string to a size limited buffer. 
 *
 * @param [in] pcDstStr The address of the destination string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * @param [in] pstSrcStr A signed structure string.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_NStrCpy
 */
ZFUNC ZINT Zos_NStrXCpy(ZCHAR *pcDstStr, ZUINT iMaxSize, 
                ST_ZOS_SSTR *pstSrcStr); 

/** 
 * @brief Copy a unsigned string to a size limited buffer. 
 *
 * @param [in] pcDstStr The address of the destination string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * @param [in] pstSrcStr A unsigned structure string.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_NStrCpy
 */
ZFUNC ZINT Zos_NStrUXCpy(ZCHAR *pcDstStr, ZUINT iMaxSize, 
                ST_ZOS_USTR *pstSrcStr); 

/** 
 * @brief Copy data buffer string to a limited buffer. 
 *
 * @param [in] pcDstStr The address of the destination string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * @param [in] zMsgBuf A pointer to message buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_NStrCpy
 */
ZFUNC ZINT Zos_NStrDCpy(ZCHAR *pcDstStr, ZUINT iMaxSize, 
                ZDBUF zMsgBuf); 

/** 
 * @brief Copy a source string to a destination string. 
 *
 * @param [in] ppcDstStr The address of the destination string.
 * @param [in] pwBufLen The address of the destination string buffer length.
 * @param [in] pcSrcStr The source string.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_NStrCpy
 */
ZFUNC ZINT Zos_StrFCpy(ZCHAR **ppcDstStr, ZUSHORT *pwBufLen, 
                ZCHAR *pcSrcStr); 

/** 
 * @brief Copy structure string to a limited buffer. 
 *
 * @param [in] zBufId The unify buffer id.
 * @param [in] pstDstStr The address of the destination string.
 * @param [in] pstSrcStr The address of the source string.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_NStrCpy
 */
ZFUNC ZINT Zos_SStrXCpy(ZUBUF zBufId, ST_ZOS_SSTR *pstDstStr, 
                ST_ZOS_SSTR *pstSrcStr);

/** 
 * @brief Convert boolean data to a string.
 *
 * @param [in] bTrueFalse A boolean type data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToBool
 */
ZFUNC ZINT Zos_BoolToStr(ZBOOL bTrueFalse, ZCHAR *pcStr, ZUINT iMaxSize); 

 /** 
 * @brief Convert a string to a boolean data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pbTrueFalse The decimal data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_BoolToStr
 */
ZFUNC ZINT Zos_StrToBool(ZCONST ZCHAR *pcStr, ZUINT iStrLen,
                ZBOOL *pbTrueFalse); 

/** 
 * @brief Convert float data to a string.
 *
 * @param [in] fData A float type data.                      
 * @param [in] iPrecision The number of decimal places.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToFloat
 */
ZFUNC ZINT Zos_FloatToStr(ZFLOAT fData, ZINT iPrecision, ZCHAR *pcStr,
               ZUINT iMaxSize); 

 /** 
 * @brief Convert a string to a float data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pfData The decimal data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_FloatToStr
 */
ZFUNC ZINT Zos_StrToFloat(ZCONST ZCHAR *pcStr, ZUINT iStrLen, ZFLOAT *pfData); 

/** 
 * @brief Convert double data to a string.
 *
 * @param [in] dData A double type data.                      
 * @param [in] iPrecision The number of decimal places.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToDouble
 */
ZFUNC ZINT Zos_DoubleToStr(ZDOUBLE dData, ZINT iPrecision, ZCHAR *pcStr,
               ZUINT iMaxSize); 

 /** 
 * @brief Convert a string to a double data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pdData The decimal data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_DoubleToStr
 */
ZFUNC ZINT Zos_StrToDouble(ZCONST ZCHAR *pcStr, ZUINT iStrLen,
                ZDOUBLE *pdData); 

/** 
 * @brief Convert unsigned long data to a string.
 *
 * @param [in] dwData A unsigned long data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToUl
 */
ZFUNC ZINT Zos_UlToStr(ZULONG dwData, ZCHAR *pcStr, ZUINT iMaxSize); 

/** 
 * @brief Convert a string to a unsigned long data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pdwData The unsigned long data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_UlToStr
 */
ZFUNC ZINT Zos_StrToUl(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZULONG *pdwData); 

/** 
 * @brief Convert long data to a string.
 *
 * @param [in] dwData A long data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToUl
 */
ZFUNC ZINT Zos_LongToStr(ZLONG dwData, ZCHAR *pcStr, ZUINT iMaxSize);

/** 
 * @brief Convert a string to a long data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pdwData The long data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_UlToStr
 */
ZFUNC ZINT Zos_StrToLong(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZLONG *pdwData);

/** 
 * @brief Convert decimal int data to a string.
 *
 * @param [in] iData A decimal int data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToInt
 */
ZFUNC ZINT Zos_IntToStr(ZINT iData, ZCHAR *pcStr, ZUINT iMaxSize);

/** 
 * @brief Convert a string to a decimal int data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] piData The decimal int data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_IntToStr
 */
ZFUNC ZINT Zos_StrToInt(ZCONST ZCHAR *pcStr, ZUINT iStrLen, ZINT *piData);

/** 
 * @brief Convert decimal unsigned int data to a string.
 *
 * @param [in] iData A decimal unsigned int data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToInt
 */
ZFUNC ZINT Zos_UintToStr(ZUINT iData, ZCHAR *pcStr, ZUINT iMaxSize);

/** 
 * @brief Convert a string to a decimal unsigned int data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] piData The decimal unsigned int data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_IntToStr
 */
ZFUNC ZINT Zos_StrToUint(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZUINT *piData);

/** 
 * @brief Convert unsigned short data to a string.
 *
 * @param [in] wData A unsigned short data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToUs
 */
ZFUNC ZINT Zos_UsToStr(ZUSHORT wData, ZCHAR *pcStr, ZUINT iMaxSize); 

/** 
 * @brief Convert a string to a decimal unsigned short data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pwData The unsigned short data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_UsToStr
 */
ZFUNC ZINT Zos_StrToUs(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZUSHORT *pwData); 

/** 
 * @brief Convert short data to a string.
 *
 * @param [in] wData A short data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToUs
 */
ZFUNC ZINT Zos_ShortToStr(ZSHORT wData, ZCHAR *pcStr, ZUINT iMaxSize);

/** 
 * @brief Convert a string to a decimal short data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pwData The short data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_UsToStr
 */
ZFUNC ZINT Zos_StrToShort(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZSHORT *pwData);

/** 
 * @brief Convert unsigned char data to a string.
 *
 * @param [in] ucData A unsigned char data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToUc
 */
ZFUNC ZINT Zos_UcToStr(ZUCHAR ucData, ZCHAR *pcStr, ZUINT iMaxSize); 

/** 
 * @brief Convert a string to a unsigned char short data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] pucData The unsigned char data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_UcToStr
 */
ZFUNC ZINT Zos_StrToUc(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZUCHAR *pucData); 

/** 
 * @brief Convert char data to a string.
 *
 * @param [in] cData A unsigned char data.
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_StrToUc
 */
ZFUNC ZINT Zos_CharToStr(ZCHAR cData, ZCHAR *pcStr, ZUINT iMaxSize);

/** 
 * @brief Convert a string to a char short data.
 *
 * @param [in] pcStr A source string.
 * @param [in] iStrLen The length of source string.
 * @param [out] pcData The unsigned char data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_UcToStr
 */
ZFUNC ZINT Zos_StrToChar(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZCHAR *pcData);

/** 
 * @brief Convert unsigned int data to a hexadecimal string.
 *
 * @param [in] iData A unsigned int data.                      
 * @param [out] pcStr The pointer to the character string.
 * @param [in] wMaxSize The maximum size of destination buffer.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_XStrToUint
 */
ZFUNC ZINT Zos_UintToXStr(ZUINT iData, ZCHAR *pcStr, ZUINT iMaxSize); 

/** 
 * @brief Convert a hexadecimal string to a unsigned long data.
 *
 * @param [in] pcStr A source string.                      
 * @param [in] iStrLen The length of source string.
 * @param [out] piData The unsigned int data.
 * 
 * @return ZOK on success, or ZFAILED on failure.
 *
 * @see Zos_UintToXStr
 */
ZFUNC ZINT Zos_XStrToUint(ZCONST ZCHAR *pcStr, ZUINT iStrLen, 
                ZUINT *piData); 

/** 
 * @brief Checks whether a string represents a hex number.
 *
 * @param [in] pcStr The pointer to the string.                      
 * @param [in] wLen The length of the string.
 * 
 * @return ZTRUE Represents a hex number, otherwize ZFALSE.
 *
 * @see Zos_IsDStr
 */
ZFUNC ZBOOL Zos_IsXStr(ZCONST ZCHAR *pcStr, ZUINT iLen);

/** 
 * @brief Checks whether a string represents a decimal number.
 *
 * @param [in] pcStr The pointer to the string.                      
 * @param [in] iLen The length of the string.
 * 
 * @return ZTRUE Represents a decimal number, otherwize ZFALSE.
 *
 * @see Zos_IsXStr
 */
ZFUNC ZBOOL Zos_IsDStr(ZCONST ZCHAR *pcStr, ZUINT iLen);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_STRING_H__ */

