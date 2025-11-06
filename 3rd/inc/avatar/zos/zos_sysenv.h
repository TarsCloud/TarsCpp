/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : zos_sysenv.h
  Module        : zos system environment
  Author        : leo.lv   
  Version       : 0.1   
  Created on    : 2010-08-05    
  Description   :
      Data structure and function definitions required by the  system environment.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_SYSENV_H__
#define _ZOS_SYSENV_H__

/** 
 * @file zos_sysenv.h
 * @brief Included files required by Zos system environment.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief ZOS protocol type */
#define ZPROTOCOL_UNKNOWN   0        /**< @brief unknown protocol */
#define ZPROTOCOL_SDP       1        /**< @brief sdp protocol */
#define ZPROTOCOL_MGCP      2        /**< @brief mgcp text protocol */
#define ZPROTOCOL_MGCO_TXT  3        /**< @brief megaco text protocol */
#define ZPROTOCOL_MGCO_BIN  4        /**< @brief megaco binary protocol */
#define ZPROTOCOL_SIP       5        /**< @brief sip protocol */
#define ZPROTOCOL_RTSP      6        /**< @brief rtsp protocol */
#define ZPROTOCOL_H323      7        /**< @brief h.323 protocol */
#define ZPROTOCOL_HTTP      8        /**< @brief http protocol */
#define ZPROTOCOL_SIGCOMP   9        /**< @brief sigcomp */
#define ZPROTOCOL_MSRP      10       /**< @brief msrp protocol */
#define ZPROTOCOL_IMAP      11       /**< @brief imap protocol */

/** @brief ZOS system environment id */
#define ZENV_COMM        0x00        /**< @brief common platform */
#define ZENV_TPTE        0x20        /**< @brief transport engine */
#define ZENV_PROTO       0x30        /**< @brief protoco stack */
#define ZENV_MME         0x50        /**< @brief multimedia engine */
#define ZENV_SRVF        0x60        /**< @brief service framework */
#define ZENV_SRVP        0x80        /**< @brief service provision */
#define ZENV_SRVE        0x90        /**< @brief service enabler */
#define ZENV_SRVU        0xB0        /**< @brief service utiltiy */
#define ZENV_SRVC        0xC0        /**< @brief service client */
#define ZENV_DCNTR       0xD0        /**< @brief data container */
#define ZENV_OTHER       0xF0        /**< @brief other application */

/** @brief ZOS system environment maximum id(252) */
#define ZENV_ID_MAX      0xFC

/** @brief ZOS system environment for common platfrom */
#define ZENV_ID_ZOS      (ZENV_COMM + 0) /**< @brief zos environment id */
#define ZENV_ID_ZSH      (ZENV_COMM + 1) /**< @brief zsh environment id */
#define ZENV_ID_ABNF     (ZENV_COMM + 2) /**< @brief abnf environment id */
#define ZENV_ID_ASN      (ZENV_COMM + 3) /**< @brief asn environment id */
#define ZENV_ID_XML      (ZENV_COMM + 4) /**< @brief xml environment id */
#define ZENV_ID_XPATH    (ZENV_COMM + 5) /**< @brief xpath environment id */
#define ZENV_ID_UTIL     (ZENV_COMM + 6) /**< @brief utility environment id */
#define ZENV_ID_RAND     (ZENV_COMM + 7) /**< @brief random environment id */
#define ZENV_ID_EAX      (ZENV_COMM + 8) /**< @brief eax environment id */

/** @brief ZOS system environment for transport */
#define ZENV_ID_UTPT     (ZENV_TPTE + 0) /**< @brief utpt environment id */
#define ZENV_ID_MPT      (ZENV_TPTE + 1) /**< @brief mpt environment id */
#define ZENV_ID_ARC      (ZENV_TPTE + 2) /**< @brief arc environment id */

/** @brief ZOS system environment for protocol */
#define ZENV_ID_DNS      (ZENV_PROTO + 0) /**< @brief dns environment id */
#define ZENV_ID_STUN     (ZENV_PROTO + 1) /**< @brief stun environment id */
#define ZENV_ID_RTP      (ZENV_PROTO + 2) /**< @brief rtp environment id */
#define ZENV_ID_TLS      (ZENV_PROTO + 3) /**< @brief tls environment id */
#define ZENV_ID_SDP      (ZENV_PROTO + 4) /**< @brief sdp environment id */
#define ZENV_ID_SIP_ABNF (ZENV_PROTO + 5) /**< @brief sip abnf environment id */
#define ZENV_ID_SIP      (ZENV_PROTO + 6) /**< @brief sip environment id */
#define ZENV_ID_HTTP_ABNF (ZENV_PROTO + 7) /**< @brief http abnf environment id */
#define ZENV_ID_HTTPC    (ZENV_PROTO + 8) /**< @brief httpc environment id */
#define ZENV_ID_XCAPC    (ZENV_PROTO + 9) /**< @brief xcapc environment id */
#define ZENV_ID_MSRP_ABNF (ZENV_PROTO + 10) /**< @brief msrp abnf environment id */
#define ZENV_ID_MSRP     (ZENV_PROTO + 11) /**< @brief msrp environment id */
#define ZENV_ID_DS       (ZENV_PROTO + 12) /**< @brief ds environment id */
#define ZENV_ID_IMAP_ABNF (ZENV_PROTO + 13) /**< @brief msrp abnf environment id */
#define ZENV_ID_IMAP     (ZENV_PROTO + 14) /**< @brief imap environment id */

/** @brief ZOS system environment for multimedia engine */
#define ZENV_ID_MME      (ZENV_MME + 0) /**< @brief mme environment id */
#define ZENV_ID_MVC      (ZENV_MME + 1) /**< @brief mvc environment id */
#define ZENV_ID_MVD      (ZENV_MME + 2) /**< @brief mvd environment id */
#define ZENV_ID_MDM      (ZENV_MME + 3) /**< @brief mdm environment id */

/* service framework environment */
#define ZENV_ID_MSF      (ZENV_SRVF + 0) /**< @brief msf environment id */
#define ZENV_ID_MRF      (ZENV_SRVF + 1) /**< @brief mrf environment id */
#define ZENV_ID_MTF      (ZENV_SRVF + 2) /**< @brief mtf environment id */
#define ZENV_ID_MPF      (ZENV_SRVF + 3) /**< @brief mpf environment id */
#define ZENV_ID_MMF      (ZENV_SRVF + 4) /**< @brief mmf environment id */
#define ZENV_ID_MXF      (ZENV_SRVF + 5) /**< @brief mxf environment id */
#define ZENV_ID_MOF      (ZENV_SRVF + 6) /**< @brief mof environment id */
#define ZENV_ID_MVF      (ZENV_SRVF + 7) /**< @brief mvf environment id */
#define ZENV_ID_MCF      (ZENV_SRVF + 8) /**< @brief mcf environment id */
#define ZENV_ID_MGF      (ZENV_SRVF + 9) /**< @brief mgf environment id */
#define ZENV_ID_MNF      (ZENV_SRVF + 10) /**< @brief mnf environment id */

/* service provision environment */
#define ZENV_ID_MSP      (ZENV_SRVP + 0) /**< @brief msf environment id */

/* service enabler environment */
#define ZENV_ID_RSD      (ZENV_SRVE + 0) /**< @brief rsd environment id */
#define ZENV_ID_RCE      (ZENV_SRVE + 1) /**< @brief rce environment id */
#define ZENV_ID_RFE      (ZENV_SRVE + 2) /**< @brief rfe environment id */
#define ZENV_ID_RGE      (ZENV_SRVE + 3) /**< @brief rge environment id */
#define ZENV_ID_RHE      (ZENV_SRVE + 4) /**< @brief rhe environment id */
#define ZENV_ID_RIE      (ZENV_SRVE + 5) /**< @brief rie environment id */
#define ZENV_ID_RME      (ZENV_SRVE + 6) /**< @brief rme environment id */
#define ZENV_ID_RNE      (ZENV_SRVE + 7) /**< @brief rne environment id */
#define ZENV_ID_RPE      (ZENV_SRVE + 8) /**< @brief rpe environment id */
#define ZENV_ID_RRE      (ZENV_SRVE + 9) /**< @brief rre environment id */
#define ZENV_ID_RSE      (ZENV_SRVE + 10) /**< @brief rse environment id */
#define ZENV_ID_ROE      (ZENV_SRVE + 11) /**< @brief roe environment id */
#define ZENV_ID_RVE      (ZENV_SRVE + 12) /**< @brief rve environment id */
#define ZENV_ID_RAE      (ZENV_SRVE + 13) /**< @brief rae environment id */
#define ZENV_ID_RLE      (ZENV_SRVE + 14) /**< @brief rle environment id */
#define ZENV_ID_RBE      (ZENV_SRVE + 15) /**< @brief rbe environment id */

/* service client environment */
#define ZENV_ID_MTC      (ZENV_SRVC + 0) /**< @brief mtc environment id */
#define ZENV_ID_RSC      (ZENV_SRVC + 1) /**< @brief rsc environment id */
#define ZENV_ID_STS      (ZENV_SRVC + 3) /**< @brief sts environment id */

/* data container environment */
#define ZENV_ID_MUC      (ZENV_DCNTR + 0) /**< @brief muc environment id */
#define ZENV_ID_MCC      (ZENV_DCNTR + 1) /**< @brief mcc environment id */
#define ZENV_ID_MLC      (ZENV_DCNTR + 2) /**< @brief mlc environment id */

/* other application environment */
#define ZENV_ID_CLOV     (ZENV_OTHER + 1) /**< @brief clover environment id */

/**
 * @brief Attach a system environment.
 *
 * @param [in] iEnvId The environment id.
 * @param [in] iSize The environment memory size.
 * @param [out] ppMem A pointer to environment memory.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see Zos_SysEnvDetach
 */
ZFUNC ZINT Zos_SysEnvAttach(ZUINT iEnvId, ZINT iSize, ZVOID **ppMem);

/**
 * @brief Detach a system environment.
 *
 * @param [in] iEnvId The environment id.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see Zos_SysEnvAttach
 */
ZFUNC ZVOID Zos_SysEnvDetach(ZUINT iEnvId);

/**
 * @brief Locate a system environment.
 *
 * @param [in] iEnvId The environment id.
 * @param [out] ppMem A pointer to environment memory.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see Zos_SysEnvAttach
 */
ZFUNC ZINT Zos_SysEnvLocate(ZUINT iEnvId, ZVOID **ppMem);

/**
 * @brief Locate a new system environment.
 *
 * @param [in] iEnvId The environment id.
 * @param [out] ppMem A pointer to environment memory.
 *
 * @retval ZOK Successfully, otherwise ZFAILED.
 *
 * @see Zos_SysEnvAttach
 */
ZFUNC ZINT Zos_SysEnvLocateNew(ZUINT iEnvId, ZVOID **ppMem);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_SYSENV_H__ */

