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
  File name     : zos_time.h
  Module        : zos time library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the time interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_TIME_H__
#define _ZOS_TIME_H__

/**
 * @file zos_time.h
 * @brief Zos date time interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Clear Zos System time. */
#define ZOS_SYS_TIME_ZERO(_systime) \
    Zos_ZeroMem(_systime, sizeof(ST_ZOS_SYS_TIME));

/** @brief Copy Zos System time. */
#define ZOS_SYS_TIME_CPY(_dst, _src) \
    Zos_MemCpy(_dst, _src, sizeof(ST_ZOS_SYS_TIME));

/**
 * @brief Get system run time.
 *
 * @param [in] piSec System run time from start.
 *
 * @retval ZOK Time get successfully.
 * @retval ZFAILED Time get failed.
 */
ZFUNC ZINT Zos_GetSysRunTime(ZUINT *piSec);

/**
 * @brief Get system date time.
 *
 * @param [out] pstTime System date time.
 *
 * @retval ZOK Time get successfully.
 * @retval ZFAILED Time get failed.
 *
 * @see Zos_SetSysTime
 */
ZFUNC ZINT Zos_SysTime(ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief Get UTC system date time.
 *
 * @param [out] pstTime System date time.
 *
 * @retval ZOK Time get successfully.
 * @retval ZFAILED Time get failed.
 *
 * @see Zos_SetSysTime
 */
ZFUNC ZINT Zos_SysTimeU(ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief Convert system time to ZTIME_T format.
 *
 * @param [in] pstTime System date time.
 * @param [out] pzTime ZTIME_T format time value.
 *
 * @retval ZOK Time convert successfully.
 * @retval ZFAILED Time convert failed.
 *
 * @see Zos_Time2SysTime
 */
ZFUNC ZINT Zos_SysTime2Time(ST_ZOS_SYS_TIME *pstTime, ZTIME_T *pzTime);

/**
 * @brief Convert system time to ZMTIME_T format.
 *
 * @param [in] pstTime System date time.
 * @param [out] pzTime ZMTIME_T format time value.
 *
 * @retval ZOK Time convert successfully.
 * @retval ZFAILED Time convert failed.
 *
 * @see Zos_Time2SysTime
 */
ZFUNC ZINT Zos_SysTime2MTime(ST_ZOS_SYS_TIME *pstTime, ZMTIME_T *pzMTime);

/**
 * @brief Convert system time to string.
 *
 * @param [in] pcFmt The date time format. ZNULL for "YYYY-MM-DDThh:mm:ssTZD"
                     YYYY = four-digit year
                     YY   = two-digit year
                     MM   = two-digit month (01 through 12)
                     DD   = two-digit day of month (01 through 31)
                     hh   = two digits of hour (00 through 23) (am/pm NOT allowed)
                     mm   = two digits of minute (00 through 59)
                     ss   = two digits of second (00 through 59)
                     TZD  = time zone designator (Z or +hh:mm or -hh:mm)
                     TZN  = time zone designator (0000 or +hhmm or -hhmm)
                     TZ   = time zone name
 * @param [in] pstTime The date time value. ZNULL for current local time.
 *
 * @return Formated time string.
 */
ZFUNC ZCHAR * Zos_SysTime2Str(ZCONST ZCHAR *pcFmt, ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief Convert system time(millisecond) from string.
 *
 * @param [in] pcFmt The date time format. ZNULL for "YYYY-MM-DDThh:mm:ss.sssTZD"
                     YYYY = four-digit year
                     YY   = two-digit year
                     MM   = two-digit month (01 through 12)
                     DD   = two-digit day of month (01 through 31)
                     hh   = two digits of hour (00 through 23) (am/pm NOT allowed)
                     mm   = two digits of minute (00 through 59)
                     ss   = two digits of second (00 through 59)
                     sss  = three digits of milliseconds (000 through 999)
                     TZD  = time zone designator (Z or +hh:mm or -hh:mm)
                     TZN  = time zone designator (0000 or +hhmm or -hhmm)
                     TZ   = time zone name
 * @param [in] pcStr The date time string.
 * @param [in] wLen The date time string length.
 * @param [out] pstTime The date time value.
 *
 * @retval ZOK Convert succeed.
 * @retval ZFAILED Convert failed.
 */
ZFUNC ZINT Zos_Str2SysMTime(ZCONST ZCHAR *pcFmt, ZCONST ZCHAR *pcStr, ZUSHORT wLen,
                ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief Convert system time from string.
 *
 * @param [in] pcFmt The date time format. ZNULL for "YYYY-MM-DDThh:mm:ssTZD"
                     YYYY = four-digit year
                     YY   = two-digit year
                     MM   = two-digit month (01 through 12)
                     DD   = two-digit day of month (01 through 31)
                     hh   = two digits of hour (00 through 23) (am/pm NOT allowed)
                     mm   = two digits of minute (00 through 59)
                     ss   = two digits of second (00 through 59)
                     TZD  = time zone designator (Z or +hh:mm or -hh:mm)
                     TZN  = time zone designator (0000 or +hhmm or -hhmm)
                     TZ   = time zone name
 * @param [in] pcStr The date time string.
 * @param [in] wLen The date time string length.
 * @param [out] pstTime The date time value.
 *
 * @retval ZOK Convert succeed.
 * @retval ZFAILED Convert failed.
 */
ZFUNC ZINT Zos_Str2SysTime(ZCONST ZCHAR *pcFmt, ZCONST ZCHAR *pcStr, ZUSHORT wLen,
                ST_ZOS_SYS_TIME *pstTime);


/**
 * @brief Convert ZTIME_T time as local time to system time format.
 *
 * @param [in] zTime ZTIME_T format time value.
 * @param [out] pstTime System date time.
 *
 * @retval ZOK Time convert successfully.
 * @retval ZFAILED Time convert failed.
 *
 * @see Zos_SysTime2Time
 */
ZFUNC ZINT Zos_Time2SysTime(ZTIME_T zTime, ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief Convert ZMTIME_T time as local time to system time format.
 *
 * @param [in] zMTime ZMTIME_T format time value.
 * @param [out] pstTime System date time.
 *
 * @retval ZOK Time convert successfully.
 * @retval ZFAILED Time convert failed.
 *
 * @see Zos_SysTime2MTime
 */
ZFUNC ZINT Zos_MTime2SysTime(ZMTIME_T zTime, ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief Convert ZTIME_T time as UTC time to system time format.
 *
 * @param [in] zTime ZTIME_T format time value.
 * @param [out] pstTime System date time.
 *
 * @retval ZOK Time convert successfully.
 * @retval ZFAILED Time convert failed.
 *
 * @see Zos_SysTime2Time
 */
ZFUNC ZINT Zos_Time2SysTimeU(ZTIME_T zTime, ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief Calucate day of week.
 *
 * @param [in] wYear The year value.
 * @param [in] ucMonth The month value, 1 - 12.
 * @param [in] ucDay The day value, 1 - 31.
 *
 * @return Day of week, 0 - 6, 0 for Sunday.
 */
ZFUNC ZUCHAR Zos_GetWeekDay(ZINT iYear, ZINT iMonth, ZINT iDay);

/**
 * @brief Get epoch time from 1970/1/1 00:00.
 *
 * @param [out] pstTime Time value.
 *
 * @retval ZOK Time get successfully.
 * @retval ZFAILED Time get failed.
 *
 * @see Zos_Time
 */
ZFUNC ZINT Zos_GetEpochTime(ST_ZOS_TIMESPEC *pstTime);

/**
 * @brief Get high resolution relative time stamp.
 *
 * @param [out] pstTime Time value.
 *
 * @retval ZOK Time get successfully.
 * @retval ZFAILED Time get failed.
 *
 * @note This value will never go backwards and will not wrap until 
 * from power-up (seconds will wrap) about 68 years.
 *
 * @see Zos_GetEpochTimeRes
 */
ZFUNC ZINT Zos_GetHiresTime(ST_ZOS_TIMESPEC *pstTime);

/**
 * @brief Get another high resolution relative time stamp in nanoseconds.
 *
 * @return High resolution relative time stamp.
 *
 * @note This value will never go backwards. This value will is in nanoseconds 
 * and will never wrap (at least for about 290 years).
 *
 * @see Zos_GetHiresTime
 */
ZFUNC ZHRTIME_T Zos_GetHrTime(ZFUNC_VOID);

/**
 * @brief Get current calendar time.
 *
 * @param [out] pzTime Current calendar time value.
 *
 * @return Current calendar time value.
 *
 * @see Zos_LocalTime
 */
ZFUNC ZTIME_T Zos_Time(ZTIME_T *pzTime);

/**
 * @brief Convert calendar time to broken-down time.
 *
 * @param [in] pzTime Current time value.
 *
 * @return Broken-down time value.
 *
 * @see Zos_MkTime
 */
ZFUNC ST_ZOS_TM * Zos_LocalTime(ZTIME_T *pzTime);

/**
 * @brief Convert broken-down time to calendar time.
 *
 * @param [in] pstTime Current time value.
 *
 * @return Calendar time value.
 *
 * @see Zos_LocalTime
 */
ZFUNC ZTIME_T Zos_MkTime(ST_ZOS_TM *pstTime);

/**
 * @brief Get time zone.
 * 
 * The value is difference between UTC and the latest local standard time,
 * in seconds west of UTC.
 *
 * For example, in the China Shanghai time zone, the value is -8 * 3600 seconds.
 *
 * @return Time zone.
 */
ZFUNC ZINT Zos_TzOffset(ZFUNC_VOID);

/**
 * @brief Get time zone number from name.
 * 
 * @return Time zone number.
 * For example, in the China Shanghai time zone, the value is -8 * 3600 seconds.
 */
ZFUNC ZCHAR * Zos_TzOffset2Name(ZINT iTzOffset);

/**
 * @brief Get time zone.
 * 
 * @return Time zone is difference between UTC and the latest local standard time,
 * in seconds west of UTC.
 * For example, in the China Shanghai time zone, the value is -8 * 3600 seconds.
 */
ZFUNC ZINT Zos_TzName2Offset(ZCONST ZCHAR *pcName, ZUINT iLen);

/**
 * @brief Get time zone name.
 * 
 * @return Time zone abbreviation frequently.
 */
ZFUNC ZCHAR * Zos_TzName(ZFUNC_VOID);

/**
 * @brief Convert time zone offset to zone number.
 *
 * @param [in] iTzOffset Time zone offset @ref Zos_TzOffset
 * 
 * @return Time zone number.
 */
ZFUNC ZINT Zos_TzOffset2Zone(ZINT iTzOffset);

/**
 * @brief Convert time zone number to offset.
 *
 * @param [in] iTzOffset Time zone number.
 * 
 * @return Time zone offset in seconds @ref Zos_TzOffset.
 */
ZFUNC ZINT Zos_TzZone2Offset(ZINT iTimeZone);

/**
 * @brief Build date time value from date and time string.
 *
 * @param [in] pcDate Date string in format of "MMM d yyyy".
 * @param [in] pcTime Time string in format of "HH:mm:ss".
 *
 * @return Date time value.
 *
 * @see 
 */
ZFUNC ZTIME_T Zos_BuildTime(ZCONST ZCHAR *pcDate, ZCONST ZCHAR *pcTime);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_TIME_H__ */

