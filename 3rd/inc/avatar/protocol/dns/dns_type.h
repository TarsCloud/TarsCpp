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
  File name     : dns_type.h
  Module        : dns library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2006-03-24    
  Description   :
      Data structure and function definitions required 
      by the dns interface

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DNS_TYPE_H__
#define _DNS_TYPE_H__
/** 
 * @file dns_type.h
 * @brief DNS type and structure definition.
 *
 * @see 
 */
#ifdef __cplusplus
extern "C" {
#endif
    
/** @brief DNS system result code 0-15(rfc1035), 6-15 are reserved. */
#define DNS_OK            0          /**< @brief No error. */
#define DNS_ERR_FMT_ERR   1          /**< @brief Query format error. */
#define DNS_ERR_SVR_FAI   2          /**< @brief Server failure. */
#define DNS_ERR_NAME_ERR  4          /**< @brief Name error, domain name not exist. */
#define DNS_ERR_NOT_IMP   5          /**< @brief Name server not implemented. */
#define DNS_ERR_REFUSED   6          /**< @brief Refused. */
#define DNS_ERR_NO_ANSWER 16         /**< @brief Got no answer error. */
#define DNS_ERR_INIT_MSG  17         /**< @brief Generate message error. */
#define DNS_ERR_DECODE    18         /**< @brief Decode dns message error. */
#define DNS_ERR_TIMEOUT   19         /**< @brief DNS response timeout error. */
#define DNS_ERR_OTHER     20         /**< @brief DNS response other error. */

/** @brief DNS rfc1035 RR(resource records) type. */
typedef enum EN_DNS_RR_TYPE
{
    EN_DNS_RR_A = 1,                 /**< @brief Ipv4 address. */
    EN_DNS_RR_NS = 2,                /**< @brief Authoritative name server. */
    EN_DNS_RR_MD = 3,                /**< @brief Mail destination (Obsolete - use MX).*/
    EN_DNS_RR_MF = 4,                /**< @brief Mail forwarder (Obsolete - use MX). */
    EN_DNS_RR_CNAME = 5,             /**< @brief Canonical name. */
    EN_DNS_RR_SOA = 6,               /**< @brief Start of authority zone. */
    EN_DNS_RR_MB = 7,                /**< @brief Mailbox domain name (EXPERIMENTAL). */
    EN_DNS_RR_MG = 8,                /**< @brief Mail group member (EXPERIMENTAL). */
    EN_DNS_RR_MR = 9,                /**< @brief Mail rename name (EXPERI). */
    EN_DNS_RR_NULL = 10,             /**< @brief Null resource record (EXPERIMENTAL). */
    EN_DNS_RR_WKS = 11,              /**< @brief Well known service. */
    EN_DNS_RR_PTR = 12,              /**< @brief Domain name pointerr. */
    EN_DNS_RR_HINFO = 13,            /**< @brief Host information. */
    EN_DNS_RR_MINFO = 14,            /**< @brief Mailbox information. */
    EN_DNS_RR_MX = 15,               /**< @brief Mail exchange. */
    EN_DNS_RR_TXT = 16,              /**< @brief Text strings. */
    EN_DNS_RR_RP = 17,               /**< @brief Responsible person. */
    EN_DNS_RR_AFSDB = 18,            /**< @brief AFS cell database. */
    EN_DNS_RR_X25 = 19,              /**< @brief X_25 calling address. */
    EN_DNS_RR_ISDN = 20,             /**< @brief ISDN calling address. */
    EN_DNS_RR_RT = 21,               /**< @brief Router. */
    EN_DNS_RR_NSAP = 22,             /**< @brief NSAP address. */
    EN_DNS_RR_NSAP_PTR = 23,         /**< @brief Reverse NSAP lookup (deprecated). */
    EN_DNS_RR_SIG = 24,              /**< @brief Security signature. */
    EN_DNS_RR_KEY = 25,              /**< @brief Security key. */
    EN_DNS_RR_PX = 26,               /**< @brief X.400 mail mapping. */
    EN_DNS_RR_GPOS = 27,             /**< @brief Geographical position (withdrawn). */
    EN_DNS_RR_AAAA = 28,             /**< @brief Ipv6 Address. */
    EN_DNS_RR_LOC = 29,              /**< @brief Location Information. */
    EN_DNS_RR_NXT = 30,              /**< @brief Next domain (security). */
    EN_DNS_RR_EID = 31,              /**< @brief Endpoint identifier. */
    EN_DNS_RR_NIMLOC = 32,           /**< @brief Nimrod Locator. */
    EN_DNS_RR_SRV = 33,              /**< @brief Server Selection. */
    EN_DNS_RR_ATMA = 34,             /**< @brief ATM Address. */
    EN_DNS_RR_NAPTR = 35,            /**< @brief Naming authority pointer. */
    EN_DNS_RR_OPT = 41,              /**< @brief OPT pseudo-RR, RFC2761. */

    /** @brief Query type values which do not appear in resource records. */
    EN_DNS_RR_IXFR = 251,            /**< @brief Incremental zone transfer. */
    EN_DNS_RR_AXFR = 252,            /**< @brief Transfer zone of authority. */
    EN_DNS_RR_MAILB = 253,           /**< @brief Transfer mailbox records. */
    EN_DNS_RR_MAILA = 254,           /**< @brief Transfer mail agent records. */
    EN_DNS_RR_ANY = 255,             /**< @brief Wildcard match. */
    EN_DNS_RR_MAX = 65536
} EN_DNS_RR_TYPE;

/** @brief DNS query response of A. */
typedef struct tagDNS_RSP_A
{
    ZUINT iAddr;                     /**< @brief Address. */
} ST_DNS_RSP_A;

/** @brief DNS query response of A. */
typedef struct tagDNS_RSP_AAAA
{
    ZUCHAR aucIpv6[ZINET_IPV6_ADDR_SIZE];   /**< @brief Address. */
} ST_DNS_RSP_AAAA;

/** @brief DNS query response of SRV. */
typedef struct tagDNS_RSP_SRV
{
    ZUSHORT wPriority;               /**< @brief Target host priority. */
    ZUSHORT wWeight;                 /**< @brief Weight for entries. */
    ZUSHORT wPort;                   /**< @brief Target host service port. */
    ZUCHAR aucSpare[2];              /**< @brief For 32 bit alignment. */
    ST_ZOS_SSTR stTarget;            /**< @brief Target domain name. */
} ST_DNS_RSP_SRV;

/** @brief DNS naptr flag bit. */
#define DNS_NAPTR_FLAG_S 0x0001      /**< @brief Next lookup for SRV records. */
#define DNS_NAPTR_FLAG_A 0x0002      /**< @brief Next lookup for an A, AAAA, or A6. */
#define DNS_NAPTR_FLAG_U 0x0004      /**< @brief Output of the Regexp is absoluteURI. */
#define DNS_NAPTR_FLAG_P 0x0008      /**< @brief New rule dependent on protocol app. */
#define DNS_NAPTR_FLAG_UNKNOWN 0x8000 /**< @brief Unknown flag. */

/** @brief DNS query response of NAPTR. */
typedef struct tagDNS_RSP_NAPTR
{
    ZUSHORT wOrder;                  /**< @brief Rules order. */
    ZUSHORT wPreference;             /**< @brief Equal "order" rules. */
    ZUINT iFlags;                    /**< @brief Flags bit flag DNS_NAPTR_FLAG_S. */
    ST_ZOS_SSTR stFlags;             /**< @brief Flags. */
    ST_ZOS_SSTR stService;           /**< @brief Service. */
    ST_ZOS_SSTR stRegexp;            /**< @brief Regular expression. */
    ST_ZOS_SSTR stReplace;           /**< @brief Replacement domain name. */
} ST_DNS_RSP_NAPTR;

/** @brief DNS query response of RR. */
typedef struct tagDNS_RSP_RR
{
    ZUSHORT wType;                   /**< @brief Query response type EN_DNS_RR_TYPE. */
    ZUCHAR aucSpare[2];              /**< @brief For 32 bit alignment. */
    union
    {
        ST_DNS_RSP_A stA;            /**< @brief A response. */
        ST_DNS_RSP_SRV stSrv;        /**< @brief SRV response. */
        ST_DNS_RSP_NAPTR stNaptr;    /**< @brief NAPTER response. */
        ST_DNS_RSP_AAAA stAAAA;      /**< @brief AAAA response. */          
        ST_ZOS_SSTR stOther;         /**< @brief Other resource data. */
    } u;
} ST_DNS_RSP_RR;

/** @brief DNS query response */
typedef struct tagDNS_RSP
{
    ZUCHAR ucResult;                 /**< @brief Query result DNS_OK. */
    ZUCHAR ucRrCount;                /**< @brief RR array count. */
    ZUSHORT wRrType;                 /**< @brief Query type EN_DNS_RR_TYPE. */
    ST_DNS_RSP_RR *pstRrGrp;         /**< @brief RR response group. */
    ZDBUF zMemBuf;                   /**< @brief memory buffer. */
} ST_DNS_RSP;

/** @brief DNS query result callback.*/
typedef ZVOID (*PFN_DNSCALLBACK)(ZCOOKIE zCookie, ST_DNS_RSP *pstRsp);

/** @brief DNS query result ipv4 address callback. */
typedef ZVOID (*PFN_DNSIPV4CALLBACK)(ZCOOKIE zCookie, ZUINT iIpv4);

#ifdef __cplusplus
}
#endif

#endif /* _DNS_TYPE_H__ */

