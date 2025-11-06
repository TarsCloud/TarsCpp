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
  File name     : zos_osenv.h
  Module        : zos os environment library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-08-10    
  Description   :
      included files required by os dependence
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_OSENV_H__
#define _ZOS_OSENV_H__

/** 
 * @file zos_osenv.h
 * @brief Included files required by Zos environment on operating system.
 *
 */

#ifndef ZPLATFORM
#error "Unsupported os, please define ZPLATFORM first!"
#endif

/** @brief ZOS operating system type */
#define ZPLATFORM_WIN32        1     /**< @brief windows */
#define ZPLATFORM_LINUX        2     /**< @brief linux */
#define ZPLATFORM_OSX          3     /**< @brief mac osx */
#define ZPLATFORM_ANDROID      4     /**< @brief android */
#define ZPLATFORM_IOS          5     /**< @brief iOS */

/** @brief ZOS define endian type */
#if defined(ZCPU_SPARC) || defined(ZCPU_68K)
#define ZBENDIAN /* big endian */
#else
#define ZLENDIAN /* little endian */
#endif

/** @brief ZOS support void function declare */
#if defined(ZOS_SUPT_FUNC_VOID) && !defined(__cplusplus)
#define ZFUNC_VOID ZVOID
#else
#define ZFUNC_VOID
#endif /* ZOS_SUPT_FUNC_VOID */

/** @brief ZOS support unused parameter */
#ifdef ZOS_SUPT_UNUSED
#define ZOS_UNUSED(_expr) do {(ZVOID)(_expr);} while (0)
#else
#define ZOS_UNUSED(_expr)
#endif /* ZOS_SUPT_UNUSED */

/* __GNUC__ */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)

#define ZATTR_PRINTF(format_idx, arg_idx) \
    __attribute__((__format__ (__printf__, format_idx, arg_idx)))

#define ZATTR_SCANF(format_idx, arg_idx) \
    __attribute__((__format__ (__scanf__, format_idx, arg_idx)))

#define ZATTR_FORMAT(arg_idx) __attribute__((__format_arg__ (arg_idx)))
#define ZATTR_NORETURN __attribute__((__noreturn__))
#define ZATTR_CONST __attribute__((__const__))
#define ZATTR_UNUSED __attribute__((__unused__))
#define ZATTR_NO_INSTRUMENT __attribute__((__no_instrument_function__))

#else   /* !__GNUC__ */

#define ZATTR_PRINTF(format_idx, arg_idx)
#define ZATTR_SCANF(format_idx, arg_idx)
#define ZATTR_FORMAT(arg_idx )
#define ZATTR_NORETURN
#define ZATTR_CONST
#define ZATTR_UNUSED
#define ZATTR_NO_INSTRUMENT

#endif  /* !__GNUC__ */


/** @brief ZOS support const */
#define ZOS_SUPT_CONST
#ifdef ZOS_SUPT_CONST
#define ZCONST const
#else
#define ZCONST
#endif /* ZOS_SUPT_CONST */

/* define lint suppress macros */
#define ZOS_NOT_NULL(_sym) /*lint -esym( 613, _sym ) */

#if (ZPLATFORM == ZPLATFORM_WIN32)

#include "zpwin_osenv.h"

#elif ZPLATFORM == ZPLATFORM_LINUX

#include "zplin_osenv.h"

#elif ZPLATFORM == ZPLATFORM_OSX

#include "zposx_osenv.h"

#elif ZPLATFORM == ZPLATFORM_ANDROID

#include "zpand_osenv.h"

#elif ZPLATFORM == ZPLATFORM_IOS

#include "zpios_osenv.h"

#endif /* ZPLATFORM */

/** @brief ZOS support 64 bits alignment */
#ifdef ZOS_SUPT_64BIT
#define ZALIGN_SIZE 8
#else /* 32 bits alignmetn */
#define ZALIGN_SIZE 4
#endif /* ZOS_SUPT_64BIT */

#endif /* _ZOS_OSENV_H__ */

