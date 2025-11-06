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
  File name     : zos_inet.h
  Module        : zos inet library functions
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2004-06-04    
  Description   :
      Data structure and function definitions required by the socket interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_INET_H__
#define _ZOS_INET_H__

/**
 * @file
 * @brief Zos network interfaces.
 *
 * This file provides interfaces to operate network address.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
/**< Clear file descriptor set. */
#define ZINET_FD_ZERO(_fds)       Zos_InetFdZero(_fds)
/**< Set a file descriptor. */
#define ZINET_FD_SET(_fd, _fds)   Zos_InetFdSet(_fds, (ZINT)_fd)
/**< Check a file descriptor is setted. */
#define ZINET_FD_ISSET(_fd, _fds) Zos_InetFdIsset(_fds, (ZINT)_fd)
/**< Clear a file descriptor. */
#define ZINET_FD_CLR(_fd, _fds)   Zos_InetFdClr(_fds, (ZINT)_fd)
/** @} */

/** @{ */
/** Convert 32 bits value from host byte order to network order */
#define ZINET_NTOHL(_long) Zos_InetNtohl(_long)
/** Convert 32 bits value from network byte order to host order */
#define ZINET_HTONL(_long) Zos_InetHtonl(_long)
/** Convert 16 bits value from host byte order to network order */
#define ZINET_NTOHS(_word) Zos_InetNtohs(_word)
/** Convert 16 bits value from network byte order to host order */
#define ZINET_HTONS(_word) Zos_InetHtons(_word)
/** @} */

/** @brief Copy IPv6 address. */
#define ZOS_IPV6_ADDR_CPY(_dst, _src) \
    Zos_MemCpy((_dst), (_src), ZINET_IPV6_ADDR_SIZE)

/** @brief Compare IPv6 address. */
#define ZOS_IPV6_ADDR_CMP(_dst, _src) \
    Zos_MemCmp((_dst), (_src), ZINET_IPV6_ADDR_SIZE)

/** @brief Convert IPv6 address to string. */
#define ZOS_IPV6_ADDR2STR(_dst, _src) \
    Zos_InetNtop(ZINET_IPV6, (_src), (_dst), ZINET_IPV6_STR_SIZE)

/** @brief Convert IPv6 string to address. */
#define ZOS_IPV6_STR2ADDR(_dst, _src) \
    Zos_InetPton(ZINET_IPV6, (_src), (_dst))

/** @brief Convert IPv6 unsigned string to address. */
#define ZOS_IPV6_USTR2ADDR(_dst, _src) \
    Zos_InetPton(ZINET_IPV6, (ZCHAR *)(_src), (_dst))

/** @brief Compare ST_ZOS_INET_ADDR type address is the same. */
#define ZOS_INET_ADDR_EQUAL(_dst, _src) \
    ((_dst)->wType == (_src)->wType && (_dst)->wPort == (_src)->wPort \
     && (((_dst)->wType == ZINET_IPV4 && (_dst)->u.iIp == (_src)->u.iIp) \
    || !Zos_MemCmp((_dst)->u.aucIpv6, (_src)->u.aucIpv6, ZINET_IPV6_ADDR_SIZE)))

/** @brief Copy ST_ZOS_INET_ADDR type address. */
#define ZOS_INET_ADDR_CPY(_dst, _src) \
    Zos_MemCpy((_dst), (_src), sizeof(ST_ZOS_INET_ADDR))

/** @brief Compare ST_ZOS_INET_IP type address is the same. */
#define ZOS_INET_IP_EQUAL(_dst, _src) \
    ((_dst)->wType == (_src)->wType \
     && (((_dst)->wType == ZINET_IPV4 && (_dst)->u.iIp == (_src)->u.iIp) \
     || !Zos_MemCmp((_dst)->u.aucIpv6, (_src)->u.aucIpv6, ZINET_IPV6_ADDR_SIZE)))

/** @brief Copy ST_ZOS_INET_IP type address. */
#define ZOS_INET_IP_CPY(_dst, _src) \
    Zos_MemCpy((_dst), (_src), sizeof(ST_ZOS_INET_IP))

/** @brief Check IP is multicast */
#define ZOS_INET_IP_IS_MCAST(_ip) \
    ((_ip)->wType == ZINET_IPV4 && Zos_InetNtohl((_ip)->u.iIp) >= 0xE0000000 \
     && Zos_InetNtohl((_ip)->u.iIp <= 0xEFFFFFFF))

/** @brief Set IP structure by IPv4 address. */
#define ZOS_IPV4_ADDR_SET(_dst, _srcv4) do { \
    (_dst)->wType = ZINET_IPV4; \
    (_dst)->u.iIp = _srcv4; \
} while (0)

/** @brief Set IP structure by IPv4 address and port. */
#define ZOS_IPV4_ADDR_SETX(_dst, _srcv4, _port) do { \
    ZOS_IPV4_ADDR_SET(_dst, _srcv4); \
    (_dst)->wPort = (ZUSHORT)_port; \
} while (0)

/** @brief Set IP structure by IPv6 address. */
#define ZOS_IPV6_ADDR_SET(_dst, _srcv6) do { \
    (_dst)->wType = ZINET_IPV6; \
    ZOS_IPV6_ADDR_CPY((_dst)->u.aucIpv6, _srcv6); \
} while (0)

/** @brief Set IP structure by IPv6 address and port. */
#define ZOS_IPV6_ADDR_SETX(_dst, _srcv6, _port) do { \
    ZOS_IPV6_ADDR_SET(_dst, _srcv6); \
    (_dst)->wPort = (ZUSHORT)_port; \
} while (0)

/** @brief Set IP structure by IPv4 string. */
#define ZOS_IPV4_ADDR_SET_STR(_dst, _srcv4s) do { \
    (_dst)->wType = ZINET_IPV4; \
    Zos_InetAddr(_srcv4s, &(_dst)->u.iIp); \
} while (0)

/** @brief Set IP structure by IPv4 string and port. */
#define ZOS_IPV4_ADDR_SET_STRX(_dst, _srcv4s, _port) do { \
    ZOS_IPV4_ADDR_SET_STR(_dst, _srcv4s); \
    (_dst)->wPort = (ZUSHORT)_port; \
} while (0)

/** @brief Set IP structure by IPv6 string. */
#define ZOS_IPV6_ADDR_SET_STR(_dst, _srcv6s) do { \
    (_dst)->wType = ZINET_IPV6; \
    ZOS_IPV6_STR2ADDR((_dst)->u.aucIpv6, _srcv6s); \
} while (0)

/** @brief Set IP structure by IPv6 string and port. */
#define ZOS_IPV6_ADDR_SET_STRX(_dst, _srcv6s, _port) do { \
    ZOS_IPV6_ADDR_SET_STR(_dst, _srcv6s); \
    (_dst)->wPort = (ZUSHORT)_port; \
} while (0)

/** @brief Set IP structure by IP address. */
#define ZOS_INET_ADDR_SET(_dst, _isv4, _srcv4, _srcv6) do { \
    if (_isv4) \
        ZOS_IPV4_ADDR_SET(_dst, _srcv4); \
    else \
        ZOS_IPV6_ADDR_SET(_dst, _srcv6); \
} while (0)

/** @brief Set IP structure by IP address and port. */
#define ZOS_INET_ADDR_SETX(_dst, _isv4, _srcv4, _srcv6, _port) do { \
    if (_isv4) \
        ZOS_IPV4_ADDR_SETX(_dst, _srcv4, _port); \
    else \
        ZOS_IPV6_ADDR_SETX(_dst, _srcv6, _port); \
} while (0)

/** @brief Set IP structure by IP string. */
#define ZOS_INET_ADDR_SET_STR(_dst, _isv4, _srcv4s, _srcv6s) do { \
    if (_isv4) \
        ZOS_IPV4_ADDR_SET_STR(_dst, _srcv4s); \
    else \
        ZOS_IPV6_ADDR_SET_STR(_dst, _srcv6s); \
} while (0)

/** @brief Set IP structure by IP string and port. */
#define ZOS_INET_ADDR_SET_STRX(_dst, _isv4, _srcv4s, _srcv6s, _port) do { \
    if (_isv4) \
        ZOS_IPV4_ADDR_SET_STRX(_dst, _srcv4s, _port); \
    else \
        ZOS_IPV6_ADDR_SET_STRX(_dst, _srcv6s, _port); \
} while (0)

/** @brief Set IP structure by IP address structure. */
#define ZOS_IP46_ADDR_SET(_dstv4, _dstv6, _isv4, _srcv4, _srcv6) do { \
    if (_isv4) _dstv4 = _srcv4; else ZOS_IPV6_ADDR_CPY(_dstv6, _srcv6); \
} while (0)

/** @brief Convert ST_ZOS_INET_IP structure to string. */
#define ZOS_INET_IP_TO_STR(_ip, _str) do { \
    ZUINT iLen = sizeof(_str); \
    Zos_InetIpToStr((ST_ZOS_INET_IP *)(_ip), (_str), &iLen); \
} while (0)

/** @brief Convert string to ST_ZOS_INET_IP structure. */
#define ZOS_INET_STR_TO_IP(_str, _ip) \
    Zos_InetStrToIp(ZOS_NSTR(_str), (ST_ZOS_INET_IP *)(_ip))

/** @brief Convert ST_ZOS_INET_ADDR structure to string. */
#define ZOS_INET_ADDR_TO_STR(_addr, _str) do { \
    ZUINT iLen = sizeof(_str); \
    Zos_InetAddrToStr((_addr), (_str), &iLen); \
} while (0)

/** @brief Convert string to ST_ZOS_INET_ADDR structure. */
#define ZOS_INET_STR_TO_ADDR(_str, _addr) \
    Zos_InetStrToAddr(ZOS_NSTR(_str), (_addr))

/** @{ */
typedef ZVOID * ZFD_SET;            /**< File descriptor set used by socket. */
/** @} */

/** @brief Zos IP structure. */
typedef struct tagZOS_INET_IP
{
    ZUSHORT wType;                   /* ZINET_IPV4... */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZOS_PADX64
    union
    {
        ZUINT iIp;                   /* not order[host or n/w] dependent */     
        ZUCHAR aucIp[ZINET_IPV4_ADDR_SIZE]; /* ipv4 address */
        ZUCHAR aucIpv6[ZINET_IPV6_ADDR_SIZE]; /* ipv6 address */
    } u;
} ST_ZOS_INET_IP;

/** @brief Zos IP set structure. */
typedef struct tagZOS_INET_IPS
{
    ZUINT iCount;                    /* ip count */
    ZOS_PADX64
    ST_ZOS_INET_IP astIps[ZINET_ADDR_MAX_NUM]; /* ip set */
} ST_ZOS_INET_IPS;

/** @brief Zos network address structure. */
typedef struct tagZOS_INET_ADDR
{
    ZUSHORT wType;                   /* ZINET_IPV4... */
    ZUSHORT wPort;                   /* not order[host or n/w] dependent */ 
    ZOS_PADX64
    union
    {
        ZUINT iIp;                   /* not order[host or n/w] dependent */     
        ZUCHAR aucIp[ZINET_IPV4_ADDR_SIZE]; /* ipv4 address */
        ZUCHAR aucIpv6[ZINET_IPV6_ADDR_SIZE]; /* ipv6 address */
    } u;
} ST_ZOS_INET_ADDR;

/** @brief Zos multicast address structure. */
typedef struct tagZOS_INET_MCAST
{
    ZUINT iCastAddr;                 /* multicast class D address */
    ZUINT iLocalAddr;                /* local interface address */
} ST_ZOS_INET_MCAST;

/*************************************************
  Function Name: Zos_TrustEvaluate
  Description: 
     ZOS trust certificate evaluatae.
     
  Input Parameters:  
     ZUCHAR *pucCertData X509 certificate.
     ZUINT iCertLen certificate len.

  Output Parameters:
     None.
    
  Return Values: 
     Returns ZTRUE on success, or ZFALSE on failure.    

*************************************************/
ZFUNC ZBOOL Zos_TrustEvaluate(ZUCHAR *pucCertData, ZUINT iCertLen);

/**
 * @brief Convert network address from an (Ipv4) dotted string to ZUINT.
 *
 * @param [in] pcAddrStr An (Ipv4) dotted string.
 * @param [out] piAddr ZUINT type value of Ipv4.
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see Zos_InetPton4
 */
ZFUNC ZINT Zos_InetAddr(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);

/**
 * @brief Convert network address from an (Ipv4) dotted string to ZUINT.
 *
 * @param [in] pcAddrStr An (Ipv4) dotted string.
 *
 * @retval The value of Ipv4 address.
 *
 * @see Zos_InetPton4
 */
ZFUNC ZUINT Zos_InetAddrX(ZCONST ZCHAR *pcAddrStr);

/**
 * @brief Convert ZUINT type value of Ipv6 to an (Ipv6) dotted string.
 *
 * @param [in] iAddr ZUINT type value of Ipv6.
 *
 * @return An (Ipv6) dotted string address.
 *
 * @see Zos_InetPton6
 */
ZFUNC ZINT Zos_InetNtop6(ZUCHAR *pucSrc, ZUCHAR *pucDst, ZUINT iLen);

/**
 * @brief Convert IP value of Ipv4 or Ipv6 to an string.
 *
 * @param [in] iInetType IP type, ZINET_IPV4 for Ipv4, ZINET_IPV6 for Ipv6.
 * @param [in] pSrc Source IP value.
 * @param [out] pcDst Destination buffer for output.
 * @param [in] iLen Destination buffer length in bytes.
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see Zos_InetPton
 */
ZFUNC ZINT Zos_InetNtop(ZINT iInetType, ZVOID *pSrc, ZCHAR *pcDst,
                ZUINT iLen);

/**
 * @brief Convert string of Ipv4 or Ipv6 to IP value.
 *
 * @param [in] iInetType IP type, ZINET_IPV4 for Ipv4, ZINET_IPV6 for Ipv6.
 * @param [in] pcSrc IP string.
 * @param [out] pDst IP value.
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see Zos_InetNtop
 */
ZFUNC ZINT Zos_InetPton(ZINT iInetType, ZCHAR *pcSrc, ZVOID *pDst);

/**
 * @brief Convert IP value of Ipv4 or Ipv6 to an string.
 *
 * @param [in] pstIp Internet address.
 * @param [in] pcDst Destination string buffer.It require ZINET_IPV4_STR_SIZE 
                     bytes for IPv4, ZINET_IPV6_STR_SIZE bytes for IPv6.
 * @param [out] piLen If pcDst is ZNULL, this will be set to the require
                      size in bytes of pcDst. When function return ZOK, it will
                      be set to the actual size in bytes of pcDst. 
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see Zos_InetStrToIp
 */
ZFUNC ZINT Zos_InetIpToStr(ST_ZOS_INET_IP *pstIp, ZCHAR *pcDst,
                ZUINT *piLen);

/**
 * @brief Convert string of Ipv4 or Ipv6 to IP value.
 *
 * @param [in] pcSrc IP string buffer.
 * @param [in] iLen String length of pcSrc.
 * @param [out] pstIp Internet address.
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see Zos_InetIpToStr
 */
ZFUNC ZINT Zos_InetStrToIp(ZCONST ZCHAR *pcSrc, ZUINT iLen,
                ST_ZOS_INET_IP *pstIp);

/**
 * @brief Convert IP value of Ipv4 or Ipv6 to an string.
 *
 * @param [in] pstAddr Internet address.
 * @param [in] pcDst Destination string buffer.It require 
                     ZINET_IPV4_PORT_STR_SIZE bytes for IPv4,
                     ZINET_IPV6_PORT_STR_SIZE bytes for IPv6.
 * @param [out] piLen If pcDst is ZNULL, this will be set to the require
                      size in bytes of pcDst. When function return ZOK, it will
                      be set to the actual size in bytes of pcDst
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see Zos_InetStrToAddr
 */
ZFUNC ZINT Zos_InetAddrToStr(ST_ZOS_INET_ADDR *pstAddr, ZCHAR *pcDst,
                ZUINT *piLen);

/**
 * @brief Convert string of Ipv4 or Ipv6 to IP value.
 *
 * @param [in] pcSrc IP string buffer.
 * @param [in] iLen String length of pcSrc.
 * @param [out] pstAddr Internet address.
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see Zos_InetAddrToStr
 */
ZFUNC ZINT Zos_InetStrToAddr(ZCONST ZCHAR *pcSrc, ZUINT iLen,
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief Convert value of MAC address to string.
 *
 * @param [in] pucMac Value of MAC address.
 * @param [out] ppcMacStr String of MAC address.
 *
 * @retval ZOK Convert successfully.
 * @retval ZFAILED Convert failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_InetMactoa(ZUCHAR *pucMac, ZCHAR **ppcMacStr);

/**
 * @brief Compare 2 IP address.
 *
 * @param [in] pstSrc First IP address.
 * @param [in] pstDst Second IP address.
 *
 * @retval ZOK Those 2 IP address are the same.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Zos_InetCmpAddr(ST_ZOS_INET_ADDR *pstSrc, ST_ZOS_INET_ADDR *pstDst);

/**
 * @brief Compare 2 IP address(not compare port).
 *
 * @param [in] pstSrc First IP address.
 * @param [in] pstDst Second IP address.
 *
 * @retval ZOK Those 2 IP address(not incluing port) are the same.
 * @retval ZFAILED Otherwise.
 *
 * @see 
 */
ZFUNC ZINT Zos_InetCmpIp(ST_ZOS_INET_ADDR *pstSrc, ST_ZOS_INET_ADDR *pstDst);

/**
 * @brief Convert network order int data to host order int data.
 *
 * @param [in] iVal network order data vale.
 *
 * @retval Host order data value.
 *
 * @see Zos_InetHtonl
 */
ZFUNC ZUINT Zos_InetNtohl(ZUINT iVal);

/**
 * @brief Convert host order int data to network order int data.
 *
 * @param [in] iVal host order data vale.
 *
 * @retval Network order data value.
 *
 * @see Zos_InetNtohl
 */
ZFUNC ZUINT Zos_InetHtonl(ZUINT iVal);

/**
 * @brief Convert network order short int data to host order short int data.
 *
 * @param [in] iVal network order data vale.
 *
 * @retval Host order data value.
 *
 * @see Zos_InetHtons
 */
ZFUNC ZUSHORT Zos_InetNtohs(ZUSHORT iVal);

/**
 * @brief Convert host order short int data to network order short int data.
 *
 * @param [in] iVal host order data vale.
 *
 * @retval Network order data value.
 *
 * @see Zos_InetNtohs
 */
ZFUNC ZUSHORT Zos_InetHtons(ZUSHORT iVal);

/**
 * @brief Create network file descriptor set.
 *
 * @param [out] pzFds Network file descriptor set.
 *
 * @retval ZOK Create network file descriptor set successfully.
 * @retval ZFAILED Create network  file descriptor set failed.
 *
 * @see Zos_InetFdDelete
 */
ZFUNC ZINT Zos_InetFdCreate(ZFD_SET *pzFds);

/**
 * @brief Delete network file descriptor set.
 *
 * @param [in] zFds Network file descriptor set.
 *
 * @retval ZOK Delete network file descriptor set successfully.
 * @retval ZFAILED Delete network file descriptor set failed.
 *
 * @see Zos_InetFdCreate
 */
ZFUNC ZINT Zos_InetFdDelete(ZFD_SET zFds);

/**
 * @brief Delete network file descriptor set.
 *
 * @param [in] zDstFds The destination network file descriptor set.
 * @param [in] zSrcFds The source network file descriptor set.
 *
 * @retval ZOK Copy network file descriptor set successfully.
 * @retval ZFAILED Copy network file descriptor set failed.
 *
 * @see Zos_InetFdCreate
 */
ZFUNC ZINT Zos_InetFdCopy(ZFD_SET zDstFds, ZFD_SET zSrcFds);

/**
 * @brief Clear network file descriptor set.
 *
 * @param [in] zFds Network file descriptor set.
 *
 * @retval ZOK Clear network file descriptor set successfully.
 * @retval ZFAILED Clear network file descriptor set failed.
 *
 * @see Zos_InetFdCopy
 */
ZFUNC ZINT Zos_InetFdZero(ZFD_SET zFds);

/**
 * @brief Add network file descriptor into set.
 *
 * @param [in] zFds Network file descriptor set.
 * @param [in] iFd Network file descriptor.
 *
 * @retval ZOK Set network file descriptor successfully.
 * @retval ZFAILED Set network file descriptor failed.
 *
 * @see Zos_InetFdCopy
 */
ZFUNC ZINT Zos_InetFdSet(ZFD_SET zFds, ZINT iFd);

/**
 * @brief Check is network file descriptor in set.
 *
 * @param [in] zFds Network file descriptor set.
 * @param [in] iFd Network file descriptor.
 *
 * @retval ZTRUE if Network file descriptor in set, otherwise return ZFALSE.
 *
 * @see Zos_InetFdCopy
 */
ZFUNC ZBOOL Zos_InetFdIsset(ZFD_SET zFds, ZINT iFd);

/**
 * @brief Clear network file descriptor from set.
 *
 * @param [in] zFds Network file descriptor set.
 * @param [in] iFd Network file descriptor.
 *
 * @retval ZOK Clear network file descriptor successfully.
 * @retval ZFAILED Clear network file descriptor failed.
 *
 * @see Zos_InetFdCopy
 */
ZFUNC ZINT Zos_InetFdClr(ZFD_SET zFds, ZINT iFd);

/**
 * @brief Get host by name, using os gethostbyname function.
 *
 * @param [in] pcName The host name.
 * @param [out] pstIps The host ip address set.
 *
 * @retval ZOK Get host address successfully.
 * @retval ZFAILED Get host address failed.
 *
 * @see Zos_InetGetLocalIp
 */
ZFUNC ZINT Zos_InetGetHostByName(ZCHAR *pcName, ST_ZOS_INET_IPS *pstIps);

/**
 * @brief Get local ip address set.
 *
 * @param [out] pstIps The host ip address set.
 *
 * @retval ZOK Get local ip address successfully.
 * @retval ZFAILED Get local ip address failed.
 *
 * @see Zos_InetGetHostByName
 */
ZFUNC ZINT Zos_InetGetLocalIp(ST_ZOS_INET_IPS *pstIps);

/**
 * @brief Get local ip address set.
 *
 * @param [in] iIpType  STG or SBC.
 * @param [out] pstIps The host ip address set.
 *
 * @retval ZOK Get local ip address successfully.
 * @retval ZFAILED Get local ip address failed.
 *
 * @see Zos_InetGetHostByName
 */
ZFUNC ZINT Zos_InetGetLocalIpX(ST_ZOS_INET_IPS *pstIps, ZINT iIpType);

/**
 * @brief Get one local ip address.
 *
 * @param [in] iIpType  STG or SBC.
 *
 * @retval Local ip address if successfully, otherwise return 0.
 *
 * @see Zos_InetGetLocalIp
 */
ZFUNC ZUINT Zos_InetGetOneLocalIp(ZINT iIpType);

/**
 * @brief Get one local ipv6 address.
 *
 * @param [in] iIpType  STG or SBC.
 * @param [out] pucIpv6 The ipv6 address.
 *
 * @retval ZOK Get local ipv6 address successfully.
 *
 * @see Zos_InetGetLocalIpv6
 */
ZFUNC ZINT Zos_InetGetOneLocalIpv6(ZINT iIpType, ZUCHAR * pucIpv6);

/**
 * @brief Get access network type.
 *
 * @retval the access network type.
 *
 * @see Zos_InetGetAccNetType
 */
ZFUNC ZINT Zos_InetGetAccNetType(ZINT iDefNetType);

/**
 * @brief Get dns ip address.
 *
 * @retval the dns ip address.
 *
 * @see Zos_InetGetDnsIpAddr
 */
ZFUNC ZCHAR * Zos_InetGetDnsIpAddr(ZINT index);

/**
 * @brief Get dns count.
 *
 * @retval dns count.
 *
 */
ZFUNC ZINT Zos_InetGetDnsCnt(ZFUNC_VOID);

/**
 * @brief Converts an ipv4 address into an ipv6 address.
 *
 * @param [in] iAddr ipv4 address.
 * @param [out] aucAddr6 The host ipv6 address.
 *
 * @retval ZOK Get ipv6 address successfully.
 * @retval ZFAILED Get ipv6 address failed.
 *
 * @see Zos_InetIpv4ToIpv6
 */
ZFUNC ZINT Zos_InetIpv4ToIpv6(ZINT iAddr, ZUCHAR* aucAddr6);

/* zos ipv6 change to ipv4 */
ZFUNC ZINT Zos_InetIpv6ToIpv4(ZUCHAR* aucAddr6);

/**
 * @brief ZOS converts string of ipv4 into string of ipv6.
 *
 * @param [in] pcStrIpv4  The string of ipv4.
 * @param [out] pcStrIpv6 The string of ipv6.
 *
 * @retval ZOK Get string of ipv6 successfully.
 * @retval ZFAILED Get string of ipv6 failed.
 *
 * @see Zos_InetStrIpv4ToIpv6
 */
ZFUNC ZINT Zos_InetStrIpv4ToIpv6(ZCHAR* pcStrIpv4, ZCHAR* pcStrIpv6);

/* zos detect local ip stack */
ZFUNC ZINT Zos_InetLocalIpStackDetect(ZFUNC_VOID); 

/**
 * @brief ZOS specify dns name to get dns ip.
 *
 * @param [in] name dns name.
 * @param [out] value dns ip.
 *
 * @return dns ip length successfully.
 */
ZFUNC ZINT Zos_InetSysPropertyGet(const char* name, char* value);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_INET_H__ */

