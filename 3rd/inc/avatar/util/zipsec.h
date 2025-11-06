/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : zipsec.h
  Module        : ipsec functions
  Author        : phil.zhou
  Version       : 0.1   
  Created on    : 2013-06-04
  Description   :
      Data structure and function definitions required by the 
      ipsec interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZIPSEC_H__
#define _ZIPSEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#if (ZPLATFORM == ZPLATFORM_ANDROID) \
    || (ZPLATFORM == ZPLATFORM_LINUX) \
    || (ZPLATFORM == ZPLATFORM_IOS)

/* zipsec mode type */
typedef enum EN_ZIPSEC_MODE_TYPE
{
    EN_ZIPSEC_MODE_ANY,
    EN_ZIPSEC_MODE_TRANSPORT,
    EN_ZIPSEC_MODE_TUNNEL,
    EN_ZIPSEC_MODE_BEET
} EN_ZIPSEC_MODE_TYPE;

/* zipsec direction type */
typedef enum EN_ZIPSEC_DIR_TYPE
{
    EN_ZIPSEC_DIR_ANY,
    EN_ZIPSEC_DIR_INBOUND,
    EN_ZIPSEC_DIR_OUTBOUND,
    EN_ZIPSEC_DIR_FWD,
    EN_ZIPSEC_DIR_MAX,
    EN_ZIPSEC_DIR_INVALID
} EN_ZIPSEC_DIR_TYPE;

/* zipsec policy type */
typedef enum EN_ZIPSEC_POLICY_TYPE
{
    EN_ZIPSEC_POLICY_DISCARD,
    EN_ZIPSEC_POLICY_NONE,
    EN_ZIPSEC_POLICY_IPSEC,
    EN_ZIPSEC_POLICY_ENTRUST,
    EN_ZIPSEC_POLICY_BYPASS
} EN_ZIPSEC_POLICY_TYPE;

/* zipsec level type */
typedef enum EN_ZIPSEC_LEVEL_TYPE
{
    EN_ZIPSEC_LEVEL_DEFAULT,
    EN_ZIPSEC_LEVEL_USE,
    EN_ZIPSEC_LEVEL_REQUIRE,
    EN_ZIPSEC_LEVEL_UNIQUE
} EN_ZIPSEC_LEVEL_TYPE;

/* zipsec upper layer protocol type */
typedef enum EN_ZIPSEC_ULPROTO_TYPE
{
    EN_ZIPSEC_ULPROTO_ANY,
    EN_ZIPSEC_ULPROTO_TCP,
    EN_ZIPSEC_ULPROTO_UDP,
} EN_ZIPSEC_ULPROTO_TYPE;

/* zipsec protocol type */
typedef enum EN_ZIPSEC_PROTO_TYPE
{
    EN_ZIPSEC_PROTO_AH,
    EN_ZIPSEC_PROTO_ESP,
} EN_ZIPSEC_PROTO_TYPE;

/* zipsec address family type */
typedef enum EN_ZIPSEC_AF_TYPE
{
    EN_ZIPSEC_AF_INET, 
    EN_ZIPSEC_AF_INET6, 
} EN_ZIPSEC_AF_TYPE;

/* zipsec messgae type */
typedef enum EN_ZIPSEC_MSG_TYPE
{
    EN_ZIPSEC_MSG_SA_ADD,
    EN_ZIPSEC_MSG_SA_DEL,
    EN_ZIPSEC_MSG_SA_FLUSH,
    EN_ZIPSEC_MSG_SP_ADD,
    EN_ZIPSEC_MSG_SP_DEL,
    EN_ZIPSEC_MSG_SP_FLUSH,
} EN_ZIPSEC_MSG_TYPE;

/* zipsec secure association type */
typedef enum EN_ZIPSEC_SA_TYPE
{
    EN_ZIPSEC_SA_UNSPEC,
    EN_ZIPSEC_SA_AH,
    EN_ZIPSEC_SA_ESP
} EN_ZIPSEC_SA_TYPE;

/* zipsec secure association extension type */
typedef enum EN_ZIPSEC_SA_EXT_TYPE
{
    EN_ZIPSEC_SA_EXT_ADDR_SRC,
    EN_ZIPSEC_SA_EXT_ADDR_DST,
    EN_ZIPSEC_SA_EXT_KEY_AUTH,
    EN_ZIPSEC_SA_EXT_KEY_ENC,
} EN_ZIPSEC_SA_EXT_TYPE;

/* zipsec authorization algorithm type */
typedef enum EN_ZIPSEC_AALG_TYPE
{
    EN_ZIPSEC_AALG_NONE,
    EN_ZIPSEC_AALG_MD5HMAC,
    EN_ZIPSEC_AALG_SHA1HMAC,
    EN_ZIPSEC_AALG_SHA2_256HMAC,
    EN_ZIPSEC_AALG_SHA2_384HMAC,
    EN_ZIPSEC_AALG_SHA2_512HMAC,
    EN_ZIPSEC_AALG_RIPEMD160HMAC
} EN_ZIPSEC_AALG_TYPE;

/* zipsec encryption algorithm type */
typedef enum EN_ZIPSEC_EALG_TYPE
{
    EN_ZIPSEC_EALG_NONE,
    EN_ZIPSEC_EALG_DESCBC,
    EN_ZIPSEC_EALG_3DESCBC,
    EN_ZIPSEC_EALG_CASTCBC,
    EN_ZIPSEC_EALG_BLOWFISHCBC,
    EN_ZIPSEC_EALG_AESCBC
} EN_ZIPSEC_EALG_TYPE;

/**
 * @brief Open a socket.
 *
 * @return Socket fd if successfully, or return ZMAXINT.
 *
 * @see ZIpsec_PfkeyClose
 */
ZFUNC ZINT ZIpsec_PfkeyOpen(ZFUNC_VOID);
 
/**
 * @brief Close a socket.
 *
 * @param [in] iSock Socket fd.
 *
 * @return [none].
 *
 * @see ZIpsec_PfkeyOpen
 */
ZFUNC ZVOID ZIpsec_PfkeyClose(ZINT iSock);

/**
 * @brief Get supported.
 *
 * @param [in] iSock Socket fd.
 *
 * @retval ZOK Get successfully.
 * @retval ZFAILED Get failed.
 */
ZFUNC ZINT ZIpsec_PfkeyGetSupt(ZINT iSock);

/**
 * @brief Add a sa record into kernel immediately.
 *
 * @param [in] iSpi Secure parameter index.
 * @param [in] iSaType Secure association type, @see EN_ZIPSEC_SA_TYPE.
 * @param [in] iModeType Secure association type, @see EN_ZIPSEC_MODE_TYPE.
 * @param [in] iAuthType Authorization algorithm type, @see EN_ZIPSEC_AALG_TYPE.
 * @param [in] pcAuthKey Authorization key.
 * @param [in] iEncType Encryption algorithm type, @see EN_ZIPSEC_EALG_TYPE.
 * @param [in] pcEncKey Encryption key.
 * @param [in] iSrcUlProtoType Source upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iSrcAfType Source address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcSrcAddr Source address(support ip and host name).
 * @param [in] pcSrcPort Source address port, value 0 means any port.
 * @param [in] iDstUlProtoType Destination upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iDstcAfType Destination address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcDstAddr Destination address(support ip and host name).
 * @param [in] pcDstPort Destination address port, value 0 means any port.
 *
 * @retval ZOK Add successfully.
 * @retval ZFAILED Add failed.
 *
 * @see ZIpsec_PfkeySaDel
 */
ZFUNC ZINT ZIpsec_PfkeySaAdd(ZUINT iSpi, ZUINT iSaType, ZUINT iModeType, 
                    ZUINT iAuthType, ZCHAR *pcAuthKey, 
                    ZUINT iEncType, ZCHAR *pcEncKey, 
                    ZUINT iSrcUlProtoType, ZUINT iSrcAfType, 
                    ZCHAR *pcSrcAddr, ZCHAR *pcSrcPort, 
                    ZUINT iDstUlProtoType, ZUINT iDstAfType, 
                    ZCHAR *pcDstAddr, ZCHAR *pcDstPort);

/**
 * @brief Delete a sa record from kernel immediately.
 *
 * @param [in] iSpi Secure parameter index.
 * @param [in] iSaType Secure association type, @see EN_ZIPSEC_SA_TYPE.
 * @param [in] iModeType Secure association type, @see EN_ZIPSEC_MODE_TYPE.
 * @param [in] iAuthType Authorization algorithm type, @see EN_ZIPSEC_AALG_TYPE.
 * @param [in] pcAuthKey Authorization key.
 * @param [in] iEncType Encryption algorithm type, @see EN_ZIPSEC_EALG_TYPE.
 * @param [in] pcEncKey Encryption key.
 * @param [in] iSrcUlProtoType Source upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iSrcAfType Source address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcSrcAddr Source address(support ip and host name).
 * @param [in] pcSrcPort Source address port, value 0 means any port.
 * @param [in] iDstUlProtoType Destination upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iDstcAfType Destination address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcDstAddr Destination address(support ip and host name).
 * @param [in] pcDstPort Destination address port, value 0 means any port.
 *
 * @retval ZOK Delete successfully.
 * @retval ZFAILED Delete failed.
 *
 * @see ZIpsec_PfkeySaAdd
 */
ZFUNC ZINT ZIpsec_PfkeySaDel(ZUINT iSpi, ZUINT iSaType, ZUINT iModeType, 
                    ZUINT iAuthType, ZCHAR *pcAuthKey, 
                    ZUINT iEncType, ZCHAR *pcEncKey, 
                    ZUINT iSrcUlProtoType, ZUINT iSrcAfType, 
                    ZCHAR *pcSrcAddr, ZCHAR *pcSrcPort, 
                    ZUINT iDstUlProtoType, ZUINT iDstAfType, 
                    ZCHAR *pcDstAddr, ZCHAR *pcDstPort);

/**
 * @brief Send a sa message to kernel.
 *
 * @param [in] iSock Socket fd.
 * @param [in] iMsgType Message type, @see EN_ZIPSEC_MSG_TYPE.
 * @param [in] iSpi Secure parameter index.
 * @param [in] iSaType Secure association type, @see EN_ZIPSEC_SA_TYPE.
 * @param [in] iModeType Secure association type, @see EN_ZIPSEC_MODE_TYPE.
 * @param [in] iAuthType Authorization algorithm type, @see EN_ZIPSEC_AALG_TYPE.
 * @param [in] pcAuthKey Authorization key.
 * @param [in] iEncType Encryption algorithm type, @see EN_ZIPSEC_EALG_TYPE.
 * @param [in] pcEncKey Encryption key.
 * @param [in] iSrcUlProtoType Source upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iSrcAfType Source address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcSrcAddr Source address(support ip and host name).
 * @param [in] pcSrcPort Source address port, value 0 means any port.
 * @param [in] iDstUlProtoType Destination upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iDstcAfType Destination address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcDstAddr Destination address(support ip and host name).
 * @param [in] pcDstPort Destination address port, value 0 means any port.
 *
 * @retval ZOK Fill successfully.
 * @retval ZFAILED Fill failed.
 */
ZFUNC ZINT ZIpsec_PfkeySaSend(ZINT iSock, ZUINT iMsgType, ZUINT iSpi, 
                    ZUINT iSaType, ZUINT iModeType, 
                    ZUINT iAuthType, ZCHAR *pcAuthKey, 
                    ZUINT iEncType, ZCHAR *pcEncKey, 
                    ZUINT iSrcUlProtoType, ZUINT iSrcAfType, 
                    ZCHAR *pcSrcAddr, ZCHAR *pcSrcPort, 
                    ZUINT iDstUlProtoType, ZUINT iDstAfType, 
                    ZCHAR *pcDstAddr, ZCHAR *pcDstPort);

/**
 * @brief Flush all sa record in kernel.
 *
 * @retval ZOK Flush successfully.
 * @retval ZFAILED Flush failed.
 */
ZFUNC ZINT ZIpsec_PfkeySaFlush(ZFUNC_VOID);

/**
 * @brief Add a sp record into kernel immediately.
 *
 * @param [in] iSaType Secure association type, @see EN_ZIPSEC_SA_TYPE.
 * @param [in] iPolicyType Policy type, @see EN_ZIPSEC_POLICY_TYPE.
 * @param [in] iDirType Direction type, @see EN_ZIPSEC_DIR_TYPE.
 * @param [in] iProtoType Protocol type, @see EN_ZIPSEC_PROTO_TYPE.
 * @param [in] iModeType Mode type, @see EN_ZIPSEC_MODE_TYPE.
 * @param [in] iLevelType Level type, @see EN_ZIPSEC_LEVEL_TYPE.
 * @param [in] iSrcUlProtoType Source upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iSrcAfType Source address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcSrcAddr Source address(support ip and host name).
 * @param [in] pcSrcPort Source address port, value 0 means any port.
 * @param [in] iDstUlProtoType Destination upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iDstcAfType Destination address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcDstAddr Destination address(support ip and host name).
 * @param [in] pcDstPort Destination address port, value 0 means any port.
 *
 * @retval ZOK Add successfully.
 * @retval ZFAILED Add failed.
 *
 * @see ZIpsec_PfkeySpDel
 */
ZFUNC ZINT ZIpsec_PfkeySpAdd(ZUINT iSaType, ZUINT iPolicyType, ZUINT iDirType, 
                    ZUINT iProtoType, ZUINT iModeType, ZUINT iLevelType, 
                    ZUINT iSrcUlProtoType, ZUINT iSrcAfType, 
                    ZCHAR *pcSrcAddr, ZCHAR *pcSrcPort, 
                    ZUINT iDstUlProtoType, ZUINT iDstAfType, 
                    ZCHAR *pcDstAddr, ZCHAR *pcDstPort);

/**
 * @brief Delete a sp record from kernel immediately.
 *
 * @param [in] iSaType Secure association type, @see EN_ZIPSEC_SA_TYPE.
 * @param [in] iPolicyType Policy type, @see EN_ZIPSEC_POLICY_TYPE.
 * @param [in] iDirType Direction type, @see EN_ZIPSEC_DIR_TYPE.
 * @param [in] iProtoType Protocol type, @see EN_ZIPSEC_PROTO_TYPE.
 * @param [in] iModeType Mode type, @see EN_ZIPSEC_MODE_TYPE.
 * @param [in] iLevelType Level type, @see EN_ZIPSEC_LEVEL_TYPE.
 * @param [in] iSrcUlProtoType Source upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iSrcAfType Source address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcSrcAddr Source address(support ip and host name).
 * @param [in] pcSrcPort Source address port, value 0 means any port.
 * @param [in] iDstUlProtoType Destination upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iDstcAfType Destination address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcDstAddr Destination address(support ip and host name).
 * @param [in] pcDstPort Destination address port, value 0 means any port.
 *
 * @retval ZOK Delete successfully.
 * @retval ZFAILED Delete failed.
 *
 * @see ZIpsec_PfkeySpAdd
 */
ZFUNC ZINT ZIpsec_PfkeySpDel(ZUINT iSaType, ZUINT iPolicyType, ZUINT iDirType, 
                    ZUINT iProtoType, ZUINT iModeType, ZUINT iLevelType, 
                    ZUINT iSrcUlProtoType, ZUINT iSrcAfType, 
                    ZCHAR *pcSrcAddr, ZCHAR *pcSrcPort, 
                    ZUINT iDstUlProtoType, ZUINT iDstAfType, 
                    ZCHAR *pcDstAddr, ZCHAR *pcDstPort);

/**
 * @brief Send a sp message to kernel.
 *
 * @param [in] iSock Socket fd.
 * @param [in] iSaType Secure association type, @see EN_ZIPSEC_SA_TYPE.
 * @param [in] iPolicyType Policy type, @see EN_ZIPSEC_POLICY_TYPE.
 * @param [in] iDirType Direction type, @see EN_ZIPSEC_DIR_TYPE.
 * @param [in] iProtoType Protocol type, @see EN_ZIPSEC_PROTO_TYPE.
 * @param [in] iModeType Mode type, @see EN_ZIPSEC_MODE_TYPE.
 * @param [in] iLevelType Level type, @see EN_ZIPSEC_LEVEL_TYPE.
 * @param [in] iSrcUlProtoType Source upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iSrcAfType Source address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcSrcAddr Source address(support ip and host name).
 * @param [in] pcSrcPort Source address port, value 0 means any port.
 * @param [in] iDstUlProtoType Destination upper layper protocol type, @see EN_ZIPSEC_ULPROTO_TYPE.
 * @param [in] iDstcAfType Destination address family type, @see EN_ZIPSEC_AF_TYPE.
 * @param [in] pcDstAddr Destination address(support ip and host name).
 * @param [in] pcDstPort Destination address port, value 0 means any port.
 *
 * @retval ZOK Fill successfully.
 * @retval ZFAILED Fill failed.
 */
ZFUNC ZINT ZIpsec_PfkeySpSend(ZUINT iSock, ZUINT iMsgType, ZUINT iSaType, 
                    ZUINT iPolicyType, ZUINT iDirType, ZUINT iProtoType, 
                    ZUINT iModeType, ZUINT iLevelType, 
                    ZUINT iSrcUlProtoType, ZUINT iSrcAfType, 
                    ZCHAR *pcSrcAddr, ZCHAR *pcSrcPort, 
                    ZUINT iDstUlProtoType, ZUINT iDstAfType, 
                    ZCHAR *pcDstAddr, ZCHAR *pcDstPort);

/**
 * @brief Flush all sp record in kernel.
 *
 * @retval ZOK Flush successfully.
 * @retval ZFAILED Flush failed.
 */
ZFUNC ZINT ZIpsec_PfkeySpFlush(ZFUNC_VOID);

#endif

#ifdef __cplusplus
}
#endif

#endif /* _ZIPSEC_H__ */

