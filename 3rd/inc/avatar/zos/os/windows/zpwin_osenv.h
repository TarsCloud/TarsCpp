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
  File name     : zpwin_osenv.h
  Module        : windows os environment
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      included files required by os environment
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPWIN_OSENV_H__
#define _ZPWIN_OSENV_H__

/** 
 * @file zpwin_osenv.h
 * @brief Included and typedefs files required by windows.
 *
 */
/*lint -save -e* */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
/*lint -restore */

/** @{ */
/* zos support ipv6 */
#define ZOS_SUPT_IPV6 ZTRUE
/** @} */

/* windows system */
#if ZPLATFORM == ZPLATFORM_WIN32

/*lint -save -e* */
/* is support ipv6 */
#ifdef ZOS_SUPT_IPV6
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <winsock.h>
#endif
#if _MSC_VER > 1200
#include <iphlpapi.h>
#endif
#include <windows.h>

#include <fcntl.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
/*lint -restore */
#ifdef _WIN64
#define ZOS_SUPT_64BIT
#else
#undef ZOS_SUPT_64BIT
#endif

#endif

/** @{ */
#define ZFUNC_EXPORT _declspec(dllexport)
#define ZFUNC_IMPORT _declspec(dllimport)
#define ZFUNC_NORMAL
/** @} */

/** @{ */
/* zos function type */
#ifdef ZOS_SUPT_DLL_EXPORT /* export dll */
#define ZFUNC ZFUNC_EXPORT
#elif defined(ZOS_SUPT_DLL_IMPORT) /* import dll */
#define ZFUNC ZFUNC_IMPORT
#else /* static library */
#define ZFUNC ZFUNC_NORMAL
#endif
/** @} */

/** @{ */
/* windows data type */
typedef double ZDOUBLE;              /**< @brief Type of double. */
typedef float ZFLOAT;                /**< @brief Type of float. */
typedef __int64 ZLLONG;              /**< @brief Type of longlong. */
typedef unsigned __int64 ZULLONG;    /**< @brief Type of unsigned long long. */
typedef __int64 ZINT64;              /**< @brief Type of longlong. */
typedef unsigned __int64 ZUINT64;    /**< @brief Type of unsigned long long. */
#ifdef _WIN64
typedef __int64 ZLONG;               /**< @brief Type of long. */
#else
typedef long ZLONG;                  /**< @brief Type of long. */
#endif
typedef int ZINT;                    /**< @brief Type of int. */
typedef short ZSHORT;                /**< @brief Type of short. */
typedef char ZCHAR;                  /**< @brief Type of char. */
#ifdef _WIN64
typedef unsigned __int64 ZULONG;     /**< @brief Type of unsigned long. */
#else
typedef unsigned int ZULONG;         /**< @brief Type of unsigned long. */
#endif
typedef unsigned int ZUINT;          /**< @brief Type of unsigned int. */
typedef size_t ZSIZE_T;              /**< @brief Type of size_t. */
typedef unsigned short ZUSHORT;      /**< @brief Type of unsinged short. */
typedef unsigned char ZUCHAR;        /**< @brief Type of unsigned char. */
typedef int ZBOOL;                   /**< @brief Type of int. */
typedef void ZVOID;                  /**< @brief Type of void. */
typedef void * ZHTTPOBJ;             /**< @biref Type of http object. */
/** @} */

/** @{ */
typedef time_t ZTIME_T;              /**< @brief Type of time_t. */
typedef LONGLONG ZMTIME_T;           /**< @brief Type of milliseconds time. */
typedef LONGLONG ZHRTIME_T;          /**< @brief Type of high resolution time. */
/** @} */

/** @{ */
typedef struct tagZOS_TIMESPEC
{
    ZTIME_T tv_sec;                  /* seconds */
    ZLONG tv_nsec;                   /* nanoseconds (0 -1,000,000,000) */
} ST_ZOS_TIMESPEC;
/** @} */

/** @{ */
typedef struct tm ST_ZOS_TM;         /**< @brief Type of tm. */
/** @} */

/** @{ */
typedef HANDLE ZTHREAD_ID;            /**< @brief Type of thread id. */
/** @} */

/** @{ */
typedef CRITICAL_SECTION ZMUTEX;     /**< @brief Type of mutex. */
typedef SRWLOCK ZSHAREX;             /**< @brief Type of shared-exclusive lock. */
typedef HANDLE ZSEM;                 /**< @brief Type of semaphore. */
/** @} */

/** @{ */
typedef ZVOID * ZSOCKET;              /**< @brief Type of socket. */
#define ZSOCKET_INVALID 0              /**< @brief Invalid socket. */
/** @} */

/** @{ */
typedef va_list ZVA_LIST;            /**< @brief Variable argument list. */
/** @} */
 
/** @{ */
/* variable argument list macro definitions */
#define ZOS_VA_START(_ap, _last) va_start(_ap, _last)
#define ZOS_VA_END(_ap)          va_end(_ap)
#define ZOS_VA_ARG(_ap, _type)   va_arg(_ap, _type)
/** @} */

#endif /* _ZPWIN_OSENV_H__ */

