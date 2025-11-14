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
  File name     : zos_type.h
  Module        : zos type definitions
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Type definitions required by the zos system.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#include "zos_osenv.h"               /* type definition by OS */

#ifndef _ZOS_TYPE_H__
#define _ZOS_TYPE_H__

/** 
 * @file zos_type.h
 * @brief Basic data type used by Zos.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
/* zos basic value */
#define ZFALSE         0             /**< @brief false */
#define ZTRUE          1             /**< @brief true */
#define ZOK            0             /**< @brief ok */
#define ZFAILED        1             /**< @brief failed */
#define ZNULL          0             /**< @brief null */
/** @} */

/** @{ */
/* zos basic type minimum value */
#define ZMINCHAR       (-128)        /**< @brief Minimum signed char value. */
#define ZMINSHORT      (-32768)      /**< @brief Minimum signed short value. */
#define ZMININT        0x80000000    /**< @brief Minimum signed int value. */
#define ZMINLONG       0x80000000L   /**< @brief Minimum signed long value. */
#define ZMININT64      0x8000000000000000LL /**< @brief Minimum signed int64 value. */
/** @} */

/** @{ */
/* zos basic type maximum value */
#define ZMAXCHAR       0x7F          /**< @brief Maximum signed char value. */
#define ZMAXUCHAR      0xFF          /**< @brief Maximum unsigned char value. */
#define ZMAXSHORT      0x7FFF        /**< @brief Maximum signed short value. */
#define ZMAXUSHORT     0xFFFF        /**< @brief Maximum unsigned short value. */
#define ZMAXINT        0x7FFFFFFF    /**< @brief Maximum signed int value. */
#define ZMAXUINT       0xFFFFFFFF    /**< @brief Maximum unsigned int value. */
#define ZMAXSIZET      ZMAXUINT      /**< @brief Maximum size_t. */
#define ZMAXLONG       0x7FFFFFFFL   /**< @brief Maximum signed long value. */
#define ZMAXULONG      0xFFFFFFFFUL  /**< @brief Maximum unsigned long value. */
#define ZMAXINT64      0x7FFFFFFFFFFFFFFFLL /**< @brief Maximum signed int64 value. */
#define ZMAXUINT64     0xFFFFFFFFFFFFFFFFUL  /**< @brief Maximum unsigned int64 value. */
/** @} */

/** @{ */
/* zos specific type maximum value */
#define ZMAXTASKID     ZMAXUINT      /**< @brief Maximum task id value. */
#define ZMAXTIMERID    ZMAXUINT      /**< @brief Maximum timer id value. */
/** @} */

/** @{ */
/* zos specific type maximum value */
#define ZINVID         ZMAXUINT      /**< @brief Zos object invalid id. */
#define ZINVALIDID     ZMAXUINT      /**< @brief Zos object invalid id. */
/** @} */

/** @{ */
/* zos basic function type */
typedef ZVOID * (*ZFUNCPTR)(ZVOID *);
typedef ZVOID (*ZVOIDFUNCPTR)(ZVOID *);
/** @} */

/** @{ */
typedef ZUINT ZTASKID;               /**< @brief Zos task id. */
typedef ZUINT ZTIMERID;              /**< @brief Zos timer id. */
typedef ZUINT ZTKNID;                /**< @brief Zos token id. */
typedef ZINT ZHTTPID;                /**< @brief Zos http id. */
typedef ZVOID * ZTLSID;              /**< @brief Zos tls context id. */
typedef ZVOID * ZDTLSID;             /**< @brief Zos dtls context id. */
typedef ZVOID * ZIMSID;              /**< @brief Zos ims id. */

/** @} */

/** @{ */
typedef ZVOID * ZLOG;                /**< @brief Zos log id. */
typedef ZVOID * ZEVNT;               /**< @brief Zos event or major event id. */
typedef ZVOID * ZMSG;                /**< @brief Zos message id. */
typedef ZVOID * ZPOOL;               /**< @brief Zos memory pool id. */

typedef ZVOID * ZFILE;               /**< @brief Zos file. */

typedef ZVOID * ZCBUF;               /**< @brief Zos convergency buffer id. */
typedef ZVOID * ZDBUF;               /**< @brief Zos data buffer id */
typedef ZVOID * ZEBUF;               /**< @brief Zos encode buffer id */
typedef ZVOID * ZPBUF;               /**< @brief Zos pipe buffer id */
typedef ZVOID * ZSBUF;               /**< @brief Zos structure buffer id */
typedef ZVOID * ZUBUF;               /**< @brief Zos unify buffer id. */

typedef ZVOID * ZCHRSET;             /**< @brief Zos chrset id. */
typedef ZVOID * ZTKNMGR;             /**< @brief Zos token manager id. */
typedef ZVOID * ZDUMP;               /**< @brief Zos dump id. */
typedef ZVOID * ZOMAP;               /**< @brief Zos object map id. */
typedef ZVOID * ZOITEM;              /**< @brief Zos object map item. */
typedef ZVOID * ZOVEC;               /**< @brief Zos object vector id. */
typedef ZSIZE_T ZCOOKIE;             /**< @brief Zos object cookie. */

/* json object */
typedef ZVOID * ZJSON;

/** @} */

/** @{ */
/* zos inet types */
#define ZINET_IPV4           0       /**< IPv4 protocol. */
#define ZINET_IPV6           1       /**< IPv6 protocol. */
/** @} */

/** @{ */
/* zos shutdown types of operation will no longer be allowed */
#define ZINET_SHTDWN_RECV    0
#define ZINET_SHTDWN_SEND    1
#define ZINET_SHTDWN_BOTH    2
/** @} */

/** @{ */
/* zos protocol types */
#define ZINET_PROTO_UDP      0       /**< UDP protocol. */
#define ZINET_PROTO_TCP      1       /**< TCP protocol. */
/** @} */

/** @{ */
/* zos service type */
#define ZINET_SRV_FOREG      0       /**< foreground service . */
#define ZINET_SRV_FBCKG      1       /**< foreground and backgound service. */
/** @} */

/** @{ */
#define ZINET_IPV4_STR_SIZE  16      /**< IPv4 string size in bytes. */
#define ZINET_IPV4_PORT_STR_SIZE 24  /**< IPv4+port string size in bytes,
                                          4 bytes alignment */
#define ZINET_IPV4_ADDR_SIZE 4       /**< IPv4 address size in bytes. */

#define ZINET_IPV6_STR_SIZE  44      /**< IPv6 string size in bytes. */
#define ZINET_IPV6_PORT_STR_SIZE 52  /**< IPv6+port string size in bytes,
                                          4 bytes alignment */
#define ZINET_IPV6_ADDR_SIZE 16      /**< IPv6 address size in bytes. */

#define ZINET_ADDR_MAX_NUM   16      /**< IPv4 max count in table. */

#define ZINET_MAC_STR_SIZE   18      /**< MAC string size in bytes. */
/** @} */

/** @{ */
/* zos local ip stack types */
#define ZINET_IP_STACK_NONE      0       /**< none. */
#define ZINET_IP_STACK_IPV4      1       /**< ipv4. */
#define ZINET_IP_STACK_IPV6      2       /**< ipv6. */
#define ZINET_IP_STACK_DUAL      3       /**< ipv4 and ipv6. */
/** @} */

#ifdef ZOS_SUPT_64BIT
#define ZOS_PADX64 ZUCHAR aucSpareX64[4];
#else
#define ZOS_PADX64
#endif

/**
 * @brief Unsigned structure string.
 *
 * @see ST_ZOS_SSTR
 */
typedef struct tagZOS_USTR
{
    ZUCHAR *pucStr;                  /**< @brief string address. */
    ZUINT iLen;                      /**< @brief string length. */
    ZOS_PADX64
} ST_ZOS_USTR;

/**
 * @brief Signed structure string.
 *
 * @see ST_ZOS_USTR                      
 */
typedef struct tagZOS_SSTR
{
    ZCHAR *pcStr;                    /**< @brief string address. */
    ZUINT iLen;                      /**< @brief string length. */
    ZOS_PADX64
} ST_ZOS_SSTR;

/** @brief Zos system time. */
typedef struct tagZOS_SYS_TIME
{
    ZUSHORT wYear;                   /**< @brief Year */
    ZUCHAR ucMonth;                  /**< @brief Month 1 - 12 */
    ZUCHAR ucDay;                    /**< @brief Day 1 - 31 */
    ZUCHAR ucWeekDay;                /**< @brief Week day 0 - 6, 0 for Sunday */
    ZUCHAR ucHour;                   /**< @brief Hour 0 - 23 */
    ZUCHAR ucMinute;                 /**< @brief Minute 0 - 59 */
    ZUCHAR ucSecond;                 /**< @brief Second 0 - 59 */
    ZUINT iMilliSecond;                  /**< @brief Millisecond 0 - 999 */
    ZINT iTzOffset;                  /**< @brief Tiem zone offset in seconds */
} ST_ZOS_SYS_TIME;

#ifdef __cplusplus 
}
#endif

#endif /* _ZOS_TYPE_H__ */

