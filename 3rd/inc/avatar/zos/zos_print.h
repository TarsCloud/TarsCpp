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
  File name     : zos_print.h
  Module        : zos print library functions
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the print interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_PRINT_H__
#define _ZOS_PRINT_H__

/** 
 * @file zos_print.h
 * @brief Zos print interfaces.
 *  
 * In this file, it defined the operation function
 * of zos print, including open, close, printf, printfbuf, printfsize
 * and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* zos print out buffer length */    
#define ZPRINT_OUT_LEN 512

/* these macros is safed use for the string length is limition */
/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutStart
 */
#define ZOS_PRINT_OUT_START(_outbuf, _buf, _blen)  \
    Zos_PrintOutStart(_outbuf, &_buf, (ZUINT *)&_blen)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutEnd
 */
#define ZOS_PRINT_OUT_END(_dbuf, _buf, _blen) \
    Zos_PrintOutEnd(_dbuf, _buf, _blen)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutChr
 */
#define ZOS_PRINT_PUT_CHR(_dbuf, _buf, _blen, _chr) \
    Zos_PrintOutPutChr(_dbuf, _buf, (ZUINT *)&_blen, _chr)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutStr
 */
#define ZOS_PRINT_PUT_STR(_dbuf, _buf, _blen, _str) \
    Zos_PrintOutPutStr(_dbuf, _buf, (ZUINT *)&_blen, _str)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutNStr
 */
#define ZOS_PRINT_PUT_NSTR(_dbuf, _buf, _blen, _str, _len) \
    Zos_PrintOutPutNStr(_dbuf, _buf, (ZUINT *)&_blen, _str, _len)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutSStr
 */
#define ZOS_PRINT_PUT_SSTR(_dbuf, _buf, _blen, _sstr) \
    Zos_PrintOutPutSStr(_dbuf, _buf, (ZUINT *)&_blen, _sstr)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutFmt
 */
#define ZOS_PRINT_PUT_FMT1(_dbuf, _buf, _blen, _s1, _s2) \
    Zos_PrintOutPutFmt(_dbuf, _buf, (ZUINT *)&_blen, _s1, _s2)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutFmt
 */
#define ZOS_PRINT_PUT_FMT2(_dbuf, _buf, _blen, _s1, _s2, _s3) \
    Zos_PrintOutPutFmt(_dbuf, _buf, (ZUINT *)&_blen, _s1, _s2, _s3)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutFmt
 */
#define ZOS_PRINT_PUT_FMT3(_dbuf, _buf, _blen, _s1, _s2, _s3, _s4) \
    Zos_PrintOutPutFmt(_dbuf, _buf, (ZUINT *)&_blen, _s1, _s2, _s3, _s4)

/** 
 * @brief Print a buffer in a specific format. 
 * @see Zos_PrintOutPutFmt
 */
#define ZOS_PRINT_PUT_FMT4(_dbuf, _buf, _blen, _s1, _s2, _s3, _s4, _s5) \
    Zos_PrintOutPutFmt(_dbuf, _buf, (ZUINT *)&_blen, _s1, _s2, _s3, _s4, _s5)

/**
 * @brief This function prints formated string into buffer.
 *
 * @param [in] pcBuf Print buffer.
 * @param [in] pcFormat The String format.
 * @param [in] ... Variable arguments.
 *
 * @return The length of formated string.
 *
 * @see 
 */
ZFUNC ZINT Zos_SPrintf(ZCHAR *pcBuf, const ZCHAR *pcFormat, ...) ZATTR_PRINTF(2, 3);

/**
 * @brief This function prints formated string into buffer.
 *
 * @param [in] pcBuf Print buffer.
 * @param [in] pcFormat The String format.
 * @param [in] zAp Variable arguments.
 *
 * @return The length of formated string.
 *
 * @see 
 */
ZFUNC ZINT Zos_VSPrintf(ZCHAR *pcBuf, const ZCHAR *pcFormat, ZVA_LIST zAp);

/**
 * @brief This function prints formated string into buffer.
 *
 * @param [in] pcStr Print buffer.
 * @param [in] zSize The maximum length of write.
 * @param [in] pcFormat The String format.
 * @param [in] ... Variable arguments.
 *
 * @return The length of formated string.
 *
 * @see 
 */
ZFUNC ZINT Zos_SNPrintf(ZCHAR *pcStr, ZSIZE_T zSize, const ZCHAR *pcFormat, ...) ZATTR_PRINTF(3, 4);

/**
 * @brief This function prints formated string into buffer.
 *
 * @param [in] pcStr Print buffer.
 * @param [in] zSize The maximum length of write.
 * @param [in] pcFormat The String format.
 * @param [in] zAp Variable arguments.
 *
 * @return The length of formated string.
 *
 * @see 
 */
ZFUNC ZINT Zos_VSNPrintf(ZCHAR *pcStr, ZSIZE_T zSize, 
                const ZCHAR *pcFormat, ZVA_LIST zAp);

/**
 * @brief Open the print flag.
 *
 * @return [none].
 *
 * @see 
 */
ZFUNC ZVOID Zos_PrintOpen(ZFUNC_VOID);

/**
 * @brief Close the print flag.
 *
 * @return [none].
 *
 * @see 
 */
ZFUNC ZVOID Zos_PrintClose(ZFUNC_VOID);

/**
 * @brief Check for print flag.
 *
 * @retval ZTRUE Print flag is open.
 * @retval ZFALSE Otherwise.
 *
 * @see 
 */
ZFUNC ZBOOL Zos_PrintIsOpen(ZFUNC_VOID);

/**
 * @brief Printf function.
 *
 * @param [in] pcFormat The variable format, followed with variables.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_Printf(const ZCHAR *pcFormat, ...) ZATTR_PRINTF(1, 2);

/**
 * @brief Print buffer with length.
 *
 * @param [in] pcBuf String buffer.
 * @param [in] iLen Length of the buffer.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintBuf(ZCHAR *pcBuf, ZINT iLen);

/**
 * @brief Print string with length.
 *
 * @param [in] pcStr String.
 * @param [in] iLen Length of the buffer.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintNStr(ZCHAR *pcStr, ZINT iLen);

/**
 * @brief Print size of formated string.
 *
 * @param [in] pcFormat The variable format.
 * @param [in] ... Variable arguments.
 *
 * @return The length of formated string.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintSize(const ZCHAR *pcFormat, ...) ZATTR_PRINTF(1, 2);

/**
 * @brief Print size of formated string with variable list.
 *
 * @param [in] pcFormat The variable format.
 * @param [in] zAp Variable arguments.
 *
 * @return The length of formated string.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintVSize(const ZCHAR *pcFormat, ZVA_LIST zAp);

/**
 * @brief Init print buffer and length.
 *
 * @param [out] pcOutBuf
 * @param [out] ppcBuf
 * @param [out] piLen
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintOutStart(ZCHAR *pcOutBuf, ZCHAR **ppcBuf, ZUINT *piLen);

/**
 * @brief ST_ZOS_DBUF is not NULL add iLen bytes in the end of a data buffer,
 *        otherwise print it.
 *
 * @param [in] zDbuf Zos data buffer.
 * @param [in] pcBuf String buffer.
 * @param [in] iLen Buffer length.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintOutEnd(ZDBUF zDbuf, ZCHAR *pcBuf, ZUINT iLen);

/**
 * @brief ST_ZOS_DBUF is not NULL Print a byte into dbuffer,
 *        otherwise print the char.
 *
 * @param [in] zDbuf Zos data buffer.
 * @param [in] pcBuf String buffer.
 * @param [in] piLen Buffer length.
 * @param [in] ucChr A char.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintOutPutChr(ZDBUF zDbuf, ZCHAR *pcBuf, ZUINT *piLen,
                ZUCHAR ucChr);

/**
 * @brief ST_ZOS_DBUF is not NULL Print string into dbuffer,
 *        otherwise print the string.
 *
 * @param [in] zDbuf Zos data buffer.
 * @param [in] pcBuf String buffer.
 * @param [in] piLen Buffer length.
 * @param [in] pcStr String.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintOutPutStr(ZDBUF zDbuf, ZCHAR *pcBuf, ZUINT *piLen,
                ZCHAR *pcStr);

/**
 * @brief ST_ZOS_DBUF is not NULL Print n byte into dbuffer,
 *        otherwise print n byte.
 *
 * @param [in] zDbuf Zos data buffer.
 * @param [in] pcBuf String buffer.
 * @param [in] piLen Buffer length.
 * @param [in] pcStr String.
 * @param [in] iStrLen String length.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintOutPutNStr(ZDBUF zDbuf, ZCHAR *pcBuf, ZUINT *piLen,
                ZCHAR *pcStr, ZUINT iStrLen);

/**
 * @brief ST_ZOS_DBUF is not NULL Print ST_ZOS_SSTR string into dbuffer,
 *        otherwise print ST_ZOS_SSTR string.
 *
 * @param [in] zDbuf Zos data buffer.
 * @param [in] pcBuf String buffer.
 * @param [in] piLen Buffer length.
 * @param [in] pstStr ST_ZOS_SSTR string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintOutPutSStr(ZDBUF zDbuf, ZCHAR *pcBuf, ZUINT *piLen,
                ST_ZOS_SSTR *pstStr);

/* zos print out put a format string */
/**
 * @brief ST_ZOS_DBUF is not NULL Print formated string into buffer,
 *        otherwise formated string.
 *
 * @param [in] zDbuf Zos data buffer.
 * @param [in] pcBuf String buffer.
 * @param [in] piLen Buffer length.
 * @param [in] pcFormat String format.
 * @param [in] ... Variable arguments.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_PrintOutPutFmt(ZDBUF zDbuf, ZCHAR *pcBuf, ZUINT *piLen,
                const ZCHAR *pcFormat, ...) ZATTR_PRINTF(4, 5);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_PRINT_H__ */

