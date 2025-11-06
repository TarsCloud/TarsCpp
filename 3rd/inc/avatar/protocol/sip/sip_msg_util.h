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
  File name     : sip_msg_util.h
  Module        : sip protocol message utility
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip message utility.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_MSG_UTIL_H__
#define _SIP_MSG_UTIL_H__

/** 
 * @file
 * @brief Interfaces for SIP message operating.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @{ */
/* sip Allow header add method */
#define SIP_ALLOW_HDR_ADD_NULL       0 /**< @brief Allow flag for clear. */
#define SIP_ALLOW_HDR_ADD_INVITE     1 /**< @brief Allow flag for INVITE method. */
#define SIP_ALLOW_HDR_ADD_ACK        2 /**< @brief Allow flag for ACK method. */
#define SIP_ALLOW_HDR_ADD_OPTIONS    3 /**< @brief Allow flag for OPTIONS method. */
#define SIP_ALLOW_HDR_ADD_BYE        4 /**< @brief Allow flag for BYE method. */
#define SIP_ALLOW_HDR_ADD_CANCEL     5 /**< @brief Allow flag for CANCEL method. */
#define SIP_ALLOW_HDR_ADD_UPDATE     6 /**< @brief Allow flag for UPDATE method. */
#define SIP_ALLOW_HDR_ADD_REGISTER   7 /**< @brief Allow flag for REGISTER method. */
#define SIP_ALLOW_HDR_ADD_INFO       8 /**< @brief Allow flag for INFO method. */
#define SIP_ALLOW_HDR_ADD_PRACK      9 /**< @brief Allow flag for PRACK method. */
#define SIP_ALLOW_HDR_ADD_SUBS       10 /**< @brief Allow flag for SUBS method. */
#define SIP_ALLOW_HDR_ADD_NOTIFY     11 /**< @brief Allow flag for NOTIFY method. */
#define SIP_ALLOW_HDR_ADD_MSG        12 /**< @brief Allow flag for MSG method. */
#define SIP_ALLOW_HDR_ADD_REFER      13 /**< @brief Allow flag for REFER method. */
#define SIP_ALLOW_HDR_ADD_COMET      14 /**< @brief Allow flag for COMET method. */
#define SIP_ALLOW_HDR_ADD_PUBLISH    15 /**< @brief Allow flag for PUBLISH method. */
#define SIP_ALLOW_HDR_ADD_ALL        ZMAXUINT /**< @brief Allow flag for ALL methods. */
/** @} */

/** @{ */
/* sip Supported/Require header tag add option */
#define SIP_TAG_OPT_ADD_NULL         0 /**< @brief Option flag for clear. */
#define SIP_TAG_OPT_ADD_100REL       1 /**< @brief Option flag for 100rel. */
#define SIP_TAG_OPT_ADD_EARLY_SESS   2 /**< @brief Option flag for early-session. */
#define SIP_TAG_OPT_ADD_EVNTLST      3 /**< @brief Option flag for eventlist. */
#define SIP_TAG_OPT_ADD_FROM_CHANGE  4 /**< @brief Option flag for from-change. */
#define SIP_TAG_OPT_ADD_HISTINFO     5 /**< @brief Option flag for histinfo. */
#define SIP_TAG_OPT_ADD_JOIN         6 /**< @brief Option flag for join. */
#define SIP_TAG_OPT_ADD_NOREFERSUB   7 /**< @brief Option flag for norefersub. */
#define SIP_TAG_OPT_ADD_PATH         8 /**< @brief Option flag for path. */
#define SIP_TAG_OPT_ADD_PRECONDITION 9 /**< @brief Option flag for precondition. */
#define SIP_TAG_OPT_ADD_PREF         10 /**< @brief Option flag for pref. */
#define SIP_TAG_OPT_ADD_PRIVACY      11 /**< @brief Option flag for privacy. */
#define SIP_TAG_OPT_ADD_REPLACES     12 /**< @brief Option flag for replaces. */
#define SIP_TAG_OPT_ADD_RES_PRIORITY 13 /**< @brief Option flag for resource-priority. */
#define SIP_TAG_OPT_ADD_SDP_ANAT     14 /**< @brief Option flag for sdp-anat. */
#define SIP_TAG_OPT_ADD_SEC_AGREE    15 /**< @brief Option flag for sec-agree. */
#define SIP_TAG_OPT_ADD_TDLG         16 /**< @brief Option flag for tdialog. */
#define SIP_TAG_OPT_ADD_TMR          17 /**< @brief Option flag for timer. */
#define SIP_TAG_OPT_ADD_MULTI_REFER  18 /**< @brief Option flag for multiple-refer. */
#define SIP_TAG_OPT_ADD_ANSWERMODE   19 /**< @brief Option flag for answermode. */
#define SIP_TAG_OPT_ADD_GRUU         20 /**< @brief Option flag for gruu. */
#define SIP_TAG_OPT_ADD_OUTBOUND     21 /**< @brief Option flag for outbound. */
#define SIP_TAG_OPT_ADD_RCP_LST_MSG  22 /**< @brief Option flag for recipient-list-message. */
#define SIP_TAG_OPT_ADD_RCP_LST_IVT  23 /**< @brief Option flag for recipient-list-invite. */
#define SIP_TAG_OPT_ADD_RCP_LST_SUB  24 /**< @brief Option flag for recipient-list-subscribe. */
#define SIP_TAG_OPT_ADD_ARC          30 /**< @brief option flag for ARC(Juphoon) */
#define SIP_TAG_OPT_ADD_MPT          31 /**< @brief Option flag for MPT(Juphoon). */
#define SIP_TAG_OPT_ADD_EPT          32 /**< @brief Option flag for EPT(Juphoon). */
#define SIP_TAG_OPT_ADD_ALL          ZMAXUINT /**< @brief Option flag for ALL. */
/** @} */

/**
 * @brief SIP message set IP address from host.
 *
 * @param [out] pstAddr Destination IP address.
 * @param [in] pstHost Source host.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_IpAddrByHost(ST_ZOS_INET_ADDR *pstAddr, 
                ST_SIP_HOST *pstHost);

/**
 * @brief SIP set IP address from host port.
 *
 * @param [out] pstAddr Destination IP address.
 * @param [in] pstHostPort Source host port.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_IpAddrByHostPort(ST_ZOS_INET_ADDR *pstAddr, 
                ST_SIP_HOST_PORT *pstHostPort);

/**
 * @brief SIP message set host by IP address.
 *
 * @param [out] pstHost Destination host.
 * @param [in] pstAddr Source IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostByIpAddr(ST_SIP_HOST *pstHost, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP message set host port by IP address.
 *
 * @param [out] pstHostPort Destination host port.
 * @param [in] pstAddr Source IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostPortByIpAddr(ST_SIP_HOST_PORT *pstHostPort,
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP message set host by IPv4 or IPv6 address.
 *
 * @param [out] pstHost Destination host.
 * @param [in] bIpv4 If ZTRUE, use IPv4, else use IPv6.
 * @param [in] iIpv4 IPv4 value.
 * @param [in] pucIpv6 IPv6 value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostByIpVal(ST_SIP_HOST *pstHost, ZBOOL bIpv4, ZUINT iIpv4, 
                ZUCHAR *pucIpv6);

/**
 * @brief SIP message set host port by IPv4 or IPv6.
 *
 * @param [out] pstHostPort Destination host port.
 * @param [in] bIpv4 If ZTRUE, use IPv4, else use IPv6.
 * @param [in] iIpv4 IPv4 value.
 * @param [in] pucIpv6 IPv6 value.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostPortByIpVal(ST_SIP_HOST_PORT *pstHostPort, 
                ZBOOL bIpv4, ZUINT iIpv4, ZUCHAR *pucIpv6, ZUINT iPort);

/**
 * @brief SIP message set host by name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [out] pstHost Destination host.
 * @param [in] pstName Host name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostByName(ZUBUF zBufId, ST_SIP_HOST *pstHost, 
                ST_ZOS_SSTR *pstName);

/**
 * @brief SIP message set host by name local.
 *
 * @param [out] pstHost Destination host.
 * @param [in] pcName Host name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostByNameL(ST_SIP_HOST *pstHost, ZCHAR *pcName);

/**
 * @brief SIP message set host by name local.
 *
 * @param [out] pstHost Destination host.
 * @param [in] pstName Host name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostByNameX(ST_SIP_HOST *pstHost, ST_ZOS_SSTR *pstName);

/**
 * @brief SIP message set host port by name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [out] pstHostPort Destination host.
 * @param [in] pstName Host name string.
 * @param [in] wPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostPortByName(ZUBUF zBufId, 
                ST_SIP_HOST_PORT *pstHostPort, ST_ZOS_SSTR *pstName, 
                ZUSHORT wPort);

/**
 * @brief SIP message set host port by name local.
 *
 * @param [out] pstHostPort Destination host.
 * @param [in] pcName Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostPortByNameL(ST_SIP_HOST_PORT *pstHostPort, 
                ZCHAR *pcName, ZUINT iPort);

/**
 * @brief SIP message set host port by name local.
 *
 * @param [out] pstHostPort Destination host.
 * @param [in] pstName Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HostPortByNameX(ST_SIP_HOST_PORT *pstHostPort, 
                ST_ZOS_SSTR *pstName, ZUINT iPort);

/**
 * @brief SIP message create.
 *
 * @param [out] ppstMsg SIP message created.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCreate(ST_SIP_MSG **ppstMsg);

/**
 * @brief SIP message create with memory buffer.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [out] ppstMsg SIP message created.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCreateX(ZUBUF zBufId, ST_SIP_MSG **ppstMsg);

/**
 * @brief SIP message create request message.
 *
 * @param [out] ppstMsg SIP message created.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCreateReq(ST_SIP_MSG **ppstMsg);

/**
 * @brief SIP message create response message.
 *
 * @param [out] ppstMsg SIP message created.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCreateRsp(ST_SIP_MSG **ppstMsg);

/**
 * @brief SIP message delete.
 *
 * @param [in] pstMsg SIP message to delete.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgDelete(ST_SIP_MSG *pstMsg);

/**
 * @brief SIP message delete in safe mode, 
 *        it is useful if user delete SIP message which is from SIP stack.
 *
 * @param [in] pstMsg SIP message.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgDeleteS(ST_SIP_MSG *pstMsg);

/**
 * @brief SIP message copy the header, type see EN_SIP_HDR.
 *
 * @param [in] pstDstMsg Destination SIP message.
 * @param [in] pstSrcMsg Source SIP message.
 * @param [in] pfnCpyHdr Copy header function.
 * @param [in] ucType Header type.
 * @param [out] ppDstHdr New created header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCpyHdr(ST_SIP_MSG *pstDstMsg, ST_SIP_MSG *pstSrcMsg,
                PFN_SIPANYCPY pfnCpyHdr, ZUCHAR ucType, ZVOID **ppDstHdr);

/**
 * @brief SIP message copy the header, type see EN_SIP_HDR.
 *
 * @param [in] pstDstMsg Destination SIP message.
 * @param [in] pSrcHdr Source header of SIP message.
 * @param [in] pfnCpyHdr Copy header function.
 * @param [in] ucType Header type, @ref EN_SIP_HDR.
 * @param [out] ppDstHdr New created header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCpyHdrX(ST_SIP_MSG *pstDstMsg, ZVOID *pSrcHdr,
                PFN_SIPANYCPY pfnCpyHdr, ZUCHAR ucType, ZVOID **ppDstHdr);

/**
 * @brief SIP message copy the From/To header.
 *
 * @param [in] pstDstMsg Destination SIP message.
 * @param [in] pstSrcMsg Source header of SIP message.
 * @param [in] bFromHdr ZTURE for From header, otherwise is To header.
 * @param [out] bSameType It it is ZFALSE, From copy to To, To copy to From.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCpyHdrFromTo(ST_SIP_MSG *pstDstMsg, 
                ST_SIP_MSG *pstSrcMsg, ZBOOL bFromHdr, ZBOOL bSameType);

/**
 * @brief SIP message copy the Route/Record-Route header.
 *
 * @param [in] pstDstMsg Destination SIP message.
 * @param [in] pstSrcMsg Source header of SIP message.
 * @param [in] bRouteHdr ZTURE for Route header, otherwise is Record-Route header.
 * @param [out] bSameType It it is ZFALSE, Route copy to Record-Route.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgCpyHdrRoute(ST_SIP_MSG *pstDstMsg, 
                ST_SIP_MSG *pstSrcMsg, ZBOOL bRouteHdr, ZBOOL bSameType);

/**
 * @brief SIP fill request line by IP address, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineByIp(ST_SIP_MSG *pstMsg, ZUCHAR ucMethod, 
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP fill request line by IP address, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pcUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineByIpX(ST_SIP_MSG *pstMsg, ZUCHAR ucMethod, 
                ZCHAR *pcUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP fill request line by IP address.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineBySipsIp(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMethod, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP fill request line by hostname, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineByName(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMethod, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_SSTR *pstHostname, ZUINT iPort);

/**
 * @brief SIP fill request line by hostname, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pcUserInfo User information string.
 * @param [in] pcHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineByNameX(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMethod, ZCHAR *pcUserInfo, 
                ZCHAR *pcHostname, ZUINT iPort);

/**
 * @brief SIP fill request line by SIPs hostname.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineBySipsName(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMethod, ST_ZOS_SSTR *pstUserInfo,
                ST_ZOS_SSTR *pstHostname, ZUINT iPort);

/**
 * @brief SIP fill request line by uri, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstUri URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineByUri(ST_SIP_MSG *pstMsg, ZUCHAR ucMethod, 
                ST_ZOS_SSTR *pstUri);

/**
 * @brief SIP fill request line by SIP uri, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstSipUri SIP URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineBySipUri(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMethod, ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP fill request line by SIPs uri, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstSipUri SIP URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineBySipsUri(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMethod, ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP fill request line by tel uri, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] pstTelUri Tel URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillReqLineByTelUri(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMethod, ST_SIP_TEL_URI *pstTelUri);

/**
 * @brief SIP fill status line.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iStatusCode Status code of response.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillStatusLine(ST_SIP_MSG *pstMsg, 
                ZUINT iStatusCode);

/**
 * @brief SIP message fill Call-ID header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstCallId Call-ID data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrCallId(ST_SIP_MSG *pstMsg, 
                ST_SIP_CALLID *pstCallId);

/**
 * @brief SIP message fill Content-Type header, 
 *        media type see EN_SIP_MTYPE, sub type see EN_SIP_MSUBTYPE.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] pstMtypeExt Extension m-type.
 * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstMSubtypeExt Extension sub-type.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrContentType(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMtype, ST_ZOS_SSTR *pstMtypeExt, ZUCHAR ucMSubtype, 
                ST_ZOS_SSTR *pstMSubtypeExt);

/**
 * @brief SIP message fill Content-Type header, 
 *        media type see EN_SIP_MTYPE, sub type see EN_SIP_MSUBTYPE.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] pstMtypeExt Extension m-type.
 * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstMSubtypeExt Extension sub-type.
 * @param [in] pcBoundary Boundary string.
 * @param [in] wLen Boundary string length.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrContentTypeX(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMtype, ST_ZOS_SSTR *pstMtypeExt, ZUCHAR ucMSubtype, 
                ST_ZOS_SSTR *pstMSubtypeExt, ZCHAR *pcBoundary, ZUSHORT wLen);

/**
 * @brief SIP message fill Content-Disposition header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcDisp The content disposition string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrContentDisp(ST_SIP_MSG *pstMsg, 
                ZCHAR *pcDisp);

/**
 * @brief SIP message fill CSeq header, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 * @param [in] iSeq CSeq value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrCSeq(ST_SIP_MSG *pstMsg, ZUCHAR ucMethod, 
                ZUINT iSeq);

/**
 * @brief SIP message fill Contact header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstAddrSpec Specific address.
 * @param [in] pstAddr Network address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrContact(ST_SIP_MSG *pstMsg, 
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP message fill Contact header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstUserInfo User information.
 * @param [in] pstAddr Network address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrContactX(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP message fill Contact header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcUserInfo User information.
 * @param [in] pstAddr Network address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrContactY(ST_SIP_MSG *pstMsg, ZCHAR *pcUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP message fill Supported header, flag see SIP_TAG_OPT_ADD_100REL...
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iSuptFlag Support option flag, @ref SIP_TAG_OPT_ADD_100REL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrSupted(ST_SIP_MSG *pstMsg, ZUINT iSuptFlag);

/**
 * @brief SIP message fill Allow header, flag see SIP_ALLOW_HDR_ADD_INVITE...
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iSuptFlag Allow method flag, @ref SIP_ALLOW_HDR_ADD_INVITE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrAllow(ST_SIP_MSG *pstMsg, ZUINT iSuptFlag);

/**
 * @brief SIP message fill Authorization header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstCredents SIP credentials data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrAuthor(ST_SIP_MSG *pstMsg, 
                ST_SIP_CREDENTS *pstCredents);

/**
 * @brief SIP fill from/to header by IP address.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToByIp(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName, 
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr, 
                ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP fill from/to header by IP address, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pcDispName Display name string.
 * @param [in] pcUserInfo User information string.
 * @param [in] pstAddr IP address.
 * @param [in] pcTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToByIpX(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ZCHAR *pcDispName,ZCHAR *pcUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr, ZCHAR *pcTag);

/**
 * @brief SIP fill from/to header by IP address.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToBySipsIp(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr, 
                ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP fill from/to header by hostname.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToByName(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP fill from/to header by hostname, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pcDispName Display name string.
 * @param [in] pcUserInfo User information string.
 * @param [in] pcHostname Host name string.
 * @param [in] iPort Port value.
 * @param [in] pcTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToByNameX(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ZCHAR *pcDispName, ZCHAR *pcUserInfo, 
                ZCHAR *pcHostname, ZUINT iPort, ZCHAR *pcTag);

/**
 * @brief SIP fill from/to header by SIPs hostname.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToBySipsName(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP fill from/to header by URI.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUri URI string.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToByUri(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP fill from/to header by URI add isfoucs-parm.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUri URI string.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToByUriIsfs(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUri, ST_ZOS_SSTR *pstTag);
/**
 * @brief SIP fill from/to header by SIP URI.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstSipUri SIP URI.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToBySipUri(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_SIP_SIP_URI *pstSipUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP fill from/to header by SIPs uri, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstSipsUri SIP URI data structure.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToBySipsUri(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_SIP_SIP_URI *pstSipsUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP fill from/to header by tel URI.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [in] pstDispName Display name string.
 * @param [in] pstTelUri Tel URI.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrFromToByTelUri(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_ZOS_SSTR *pstDispName,
                ST_SIP_TEL_URI *pstTelUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP message fill require header, flag see SIP_TAG_OPT_ADD_100REL... 
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iSuptFlag Support option flag, @ref SIP_TAG_OPT_ADD_100REL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrRequire(ST_SIP_MSG *pstMsg, ZUINT iSuptFlag);

/**
 * @brief SIP message fill Proxy-Require header, flag see SIP_TAG_OPT_ADD_100REL... 
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iSuptFlag Support option flag, @ref SIP_TAG_OPT_ADD_100REL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrProxyRequire(ST_SIP_MSG *pstMsg, ZUINT iSuptFlag);

/**
 * @brief SIP message fill Session-Expires header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bIsRefresher If ZTRUE, set refersher flag. Or clear that flag.
 * @param [in] bIsUasRefresher If ZTRUE, set UAS as refresher flag.
 *             Or clear that flag.
 * @param [in] iExpire Expires value..
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrSessExpire(ST_SIP_MSG *pstMsg, 
                ZBOOL bIsRefresher, ZBOOL bIsUasRefresher, ZUINT iExpire);

/**
 * @brief SIP message fill Min-SE header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iExpire Expires value..
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrMinSe(ST_SIP_MSG *pstMsg, ZUINT iExpire);

/**
 * @brief SIP message fill Event header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucPkgType Event pacakge type, @ref EN_SIP_EVNT_PKG.
 * @param [in] ucTempType Event template type, @ref EN_SIP_EVNT_TEMP.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrEvnt(ST_SIP_MSG *pstMsg, ZUCHAR ucPkgType, 
                ZUCHAR ucTempType);

/**
 * @brief SIP message fill Event header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcEvntPkg Event pacakge type.
 * @param [in] pcEvntTemp Event template type.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrEvntX(ST_SIP_MSG *pstMsg, ZCHAR *pcEvntPkg, 
                ZCHAR *pcEvntTemp);

/**
 * @brief SIP message fill Expire header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iExpire Expires value..
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrExpire(ST_SIP_MSG *pstMsg, ZUINT iExpire);

/**
 * @brief SIP message fill Subscription-State header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucState Subscription status type, @ref EN_SIP_SUBSTA_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrSubsSta(ST_SIP_MSG *pstMsg, ZUCHAR ucState);

/**
 * @brief SIP message fill Subscription-State header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucState Subscription status type, @ref EN_SIP_SUBSTA_VAL.
 * @param [in] ucReason Subscription reason type, @ref EN_SIP_EVNT_REASON_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrSubsStaX(ST_SIP_MSG *pstMsg, ZUCHAR ucState, 
                ZUCHAR ucReason);

/**
 * @brief SIP message fill Subscription-State header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucState Subscription status type, @ref EN_SIP_SUBSTA_VAL.
 * @param [in] ucReason Subscription reason type, @ref EN_SIP_EVNT_REASON_VAL.
 * @param [in] iExpires Subscription expire value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrSubsStaY(ST_SIP_MSG *pstMsg, ZUCHAR ucState, 
                ZUCHAR ucReason, ZUINT iExpires);

/**
 * @brief SIP message fill RAck header, method see EN_SIP_METHOD.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iCseqNum CSeq value.
 * @param [in] iRspNum RSeq value.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrRAck(ST_SIP_MSG *pstMsg, ZUINT iCseqNum,
                ZUINT iRspNum, ZUCHAR ucMethod);

/**
 * @brief SIP message fill RAck header from RSeq header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstCseq CSeq header.
 * @param [in] pstRseq RSeq header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrRAckX(ST_SIP_MSG *pstMsg, 
                ST_SIP_HDR_CSEQ *pstCseq, ST_SIP_HDR_RSEQ *pstRseq);

/**
 * @brief SIP message fill RSeq header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iRspNum RSeq value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrRSeq(ST_SIP_MSG *pstMsg, ZUINT iRspNum);

/**
 * @brief SIP message fill Privacy header, value see EN_SIP_PRIV_VAL.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucPrivVal Privacy type, @ref EN_SIP_PRIV_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrPrivacy(ST_SIP_MSG *pstMsg, ZUCHAR ucPrivVal);

/**
 * @brief SIP message fill Accept Header, mtype see EN_SIP_MTYPE, 
 *        stype see EN_SIP_MSUBTYPE.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMType Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] ucSType Value of sub-type, @ref EN_SIP_MSUBTYPE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrAcpt(ST_SIP_MSG *pstMsg, ZUCHAR ucMType, 
                ZUCHAR ucSType);

/**
 * @brief SIP message fill Refer-Sub header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bValue Refer-Sub value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrReferSub(ST_SIP_MSG *pstMsg, ZBOOL bValue);

/**
 * @brief SIP message fill Proxy-Authenticate header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstChallenge SIP challenge data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrProxyAuthen(ST_SIP_MSG *pstMsg, 
                ST_SIP_CHALLENGE *pstChallenge);

/**
 * @brief SIP message fill Proxy-Authorization header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstCredents SIP credentials data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrProxyAuthor(ST_SIP_MSG *pstMsg, 
                ST_SIP_CREDENTS *pstCredents);

/**
 * @brief SIP message fill Content-ID header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstUri URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrContentId(ST_SIP_MSG *pstMsg, 
                ST_SIP_ADDR_SPEC *pstAddrSpec);

/**
 * @brief SIP message fill SIP-If-Match header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstTag Tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrSipIfMatch(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP message fill P-Asserted-Service header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstSrvId Server ID string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrPAssertedSrv(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstSrvId);

/**
 * @brief SIP message fill P-Preferred-Service header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstSrvId Server ID string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrPPreferredSrv(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstSrvId);

/**
 * @brief SIP message fill P-Early-Media header header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iParmType Early media type, @ref EN_SIP_P_EARLY_MEDIA_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrPEarlyMedia(ST_SIP_MSG *pstMsg, 
                ZUINT iParmType);

/**
 * @brief SIP message fill P-Access-Network-Info header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucAccType Access type, @ref EN_SIP_ACCESS_TYPE.
 * @param [in] ucInfoType Access info type, @ref EN_SIP_ACCESS_INFO.
 * @param [in] pstInfoVal Access info value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrPAccNetInfo(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucAccType, ZUCHAR ucInfoType, ST_ZOS_SSTR *pstInfoVal);

/**
 * @brief SIP message fill User-Agent header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstName Name string.
 * @param [in] pstVer Version string.
 * @param [in] pstComm Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrUserAgent(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstName, ST_ZOS_SSTR *pstVer, 
                ST_ZOS_SSTR *pstComm);

/**
 * @brief SIP message fill User-Agent header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Name string.
 * @param [in] pcVer Version string.
 * @param [in] pcComm Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrUserAgentX(ST_SIP_MSG *pstMsg, ZCHAR *pcName,
                ZCHAR *pcVer, ZCHAR *pcComm);

/**
 * @brief SIP message fill Via header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucTptType The transport type, @ref EN_SIP_TPT.
 * @param [in] pstAddr The network address.
 * @param [in] pstBranch The Via branch string.
 * @param [in] bRport The rport exist flag.
 * @param [in] iRport The rport value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrVia(ST_SIP_MSG *pstMsg, ZUCHAR ucTptType, 
                ST_ZOS_INET_ADDR *pstAddr, ST_ZOS_SSTR *pstBranch, 
                ZBOOL bRport, ZUINT iRport);

/**
 * @brief SIP message fill Server header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstName Name string.
 * @param [in] pstVer Version string.
 * @param [in] pstComm Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrServer(ST_SIP_MSG *pstMsg, ST_ZOS_SSTR *pstName,
                ST_ZOS_SSTR *pstVer, ST_ZOS_SSTR *pstComm);

/**
 * @brief SIP message fill Server header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Name string.
 * @param [in] pcVer Version string.
 * @param [in] pcComm Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrServerX(ST_SIP_MSG *pstMsg, ZCHAR *pcName,
                ZCHAR *pcVer, ZCHAR *pcComm);

/**
 * @brief SIP message fill Www-Authenticate header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstChallenge SIP challenge data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrWwwAuthen(ST_SIP_MSG *pstMsg, 
                ST_SIP_CHALLENGE *pstChallenge);

/**
 * @brief SIP message fill Alert-Info header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcScheme Scheme in absoluteURI.
 * @param [in] pcDesc Description in absoluteURI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrAlertInfo(ST_SIP_MSG *pstMsg, ZCHAR *pcScheme,
                ZCHAR *pcDesc);

/**
 * @brief SIP message fill Allow-Events header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucPkgType Event pacakge type, @ref EN_SIP_EVNT_PKG.
 * @param [in] ucTempType Event template type, @ref EN_SIP_EVNT_TEMP.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrAllowEvnts(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucPkgType, ZUCHAR ucTempType);

/**
 * @brief SIP message fill Reason header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iCode Protocol type, see EN_SIP_PROTOCOL.
 * @param [in] iCause Reason cause code.
 * @param [in] pcText Reason text.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrReason(ST_SIP_MSG *pstMsg, ZUCHAR ucProtoType,
                ZUINT iCause, ZCHAR *pcText);

/**
 * @brief SIP message fill Replaces header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcText Replaces text.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrReplaces(ST_SIP_MSG *pstMsg, ZCHAR *pcText);

/**
 * @brief SIP message fill In-Reply-To header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstCallId Call-ID.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrInReplyTo(ST_SIP_MSG *pstMsg, 
                ST_SIP_CALLID *pstCallId);

/**
 * @brief SIP message fill Warning header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iCode Warning code.
 * @param [in] pcAgent Warning agent.
 * @param [in] pcText Warning text.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrWarn(ST_SIP_MSG *pstMsg, ZUINT iCode, 
                ZCHAR *pcAgent, ZCHAR *pcText);

/**
 * @brief SIP message fill Date header.
 * @param  pstMsg  SIP message.
 * @param  pstTime Date value in ST_ZOS_SYS_TIME format.
 * 
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrDate(ST_SIP_MSG *pstMsg, ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief SIP message fill ServInfo header.
 * @param  pstMsg  SIP message.
 * @param  ucServInfoType  ServInfo type @ref EN_SIP_SERVINFO_PARMS.
 * @param pstInfo ServInfo(P2P or Excom) value;
 * 
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillHdrServInfo(ST_SIP_MSG *pstMsg, ZUCHAR ucServInfoType,
                ST_ZOS_SSTR *pstInfo);

/**
 * @brief SIP message fill SDP body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstSdp SDP Body.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodySdp(ST_SIP_MSG *pstMsg, 
                ST_SDP_SESS_DESC *pstSdp);

/**
 * @brief SIP message fill data body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] zData The Body data.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodyData(ST_SIP_MSG *pstMsg, ZUCHAR ucMtype,
                ZUCHAR ucMSubtype, ZDBUF zData);

/**
 * @brief SIP message fill string body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstStr The Body string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodyStr(ST_SIP_MSG *pstMsg, ZUCHAR ucMtype,
                ZUCHAR ucMSubtype, ST_ZOS_SSTR *pstStr);

/**
 * @brief SIP message fill sipfrag body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iStatCode The status code.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodySfrag(ST_SIP_MSG *pstMsg, ZUINT iStatCode);

/**
 * @brief SIP message fill SDP body in multipart body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pstSdp SDP Body.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodyMPartSdp(ST_SIP_MSG *pstMsg, 
                ST_SDP_SESS_DESC *pstSdp);

/**
 * @brief SIP message fill data body in multipart body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] zData The Body data.
 * @param [in] ppstMPart The Multipart body, use can add header in it.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodyMPartData(ST_SIP_MSG *pstMsg, 
                ZUCHAR ucMtype, ZUCHAR ucMSubType, ZDBUF zData, 
                ST_SIP_BODY_MPART **ppstMPart);

/**
 * @brief SIP message fill data body in multipart body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstStr The Body string.
 * @param [in] ppstMPart The Multipart body, use can add header in it.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodyMPartStr(ST_SIP_MSG *pstMsg, ZUCHAR ucMtype,
                ZUCHAR ucMSubType, ST_ZOS_SSTR *pstStr, 
                ST_SIP_BODY_MPART **ppstMPart);

/**
 * @brief SIP message fill data body in multipart body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] pcMSubType Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstStr The Body string.
 * @param [in] ppstMPart The Multipart body, use can add header in it.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodyMPartStrX(ST_SIP_MSG *pstMsg, ZCHAR *pcMtype,
                ZCHAR *pcMSubType, ST_ZOS_SSTR *pstStr, 
                ST_SIP_BODY_MPART **ppstMPart);

/**
 * @brief SIP message fill sipfrag body in multipart body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iStatCode The status code.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgFillBodyMPartSfrag(ST_SIP_MSG *pstMsg, 
                ZUINT iStatCode);

/**
 * @brief SIP message get request uri in request line.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstUri The request uri.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReqUri(ST_SIP_MSG *pstMsg, 
                ST_SIP_REQ_URI **ppstUri);

/**
 * @brief SIP message get request SIP uri in request line.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstUri The request SIP uri.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReqSipUri(ST_SIP_MSG *pstMsg, 
                ST_SIP_SIP_URI **ppstUri);

/**
 * @brief SIP message get request SIPS uri in request line.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstUri The request SIPS uri.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReqSipsUri(ST_SIP_MSG *pstMsg, 
                ST_SIP_SIP_URI **ppstUri);

/**
 * @brief SIP message get request TEL uri in request line.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstUri The request TEL uri.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReqTelUri(ST_SIP_MSG *pstMsg, 
                ST_SIP_TEL_URI **ppstUri);

/**
 * @brief SIP message get request method in request line.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucMethod The request method, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReqMethod(ST_SIP_MSG *pstMsg, ZUCHAR *pucMethod);

/**
 * @brief SIP message get status code.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piStatCode The status code.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
 ZFUNC ZINT Sip_MsgGetStatCode(ST_SIP_MSG *pstMsg, ZUINT *piStatCode);

/**
 * @brief SIP message get status reason.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pstReason The status reason.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetStatReason(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstReason);

/**
 * @brief SIP message get allow method.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstMethodLst Allow method list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetAllow(ST_SIP_MSG *pstMsg,
                ST_SIP_METHOD_LST **ppstMethodLst);

/**
 * @brief SIP message get allow flag SIP_ALLOW_HDR_ADD_INVITE.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piAllowFlag Allow method flag, @ref SIP_ALLOW_HDR_ADD_INVITE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetAllowFlag(ST_SIP_MSG *pstMsg, 
                ZUINT *piAllowFlag);

/**
 * @brief SIP message get allow event SIP_ALLOW_HDR_ADD_INVITE.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piAllowEvnt Allow event, @ref EN_SIP_EVNT_PKG.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetAllowEvnt(ST_SIP_MSG *pstMsg, 
                ZUINT *piAllowEvnt);

/**
 * @brief SIP message get Authorization.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstCredents SIP credents.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetAuthor(ST_SIP_MSG *pstMsg, 
                ST_SIP_CREDENTS **ppstCredents);

/**
 * @brief SIP message get call-id.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstCallId Call-ID data strcture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetCallId(ST_SIP_MSG *pstMsg, 
                ST_SIP_CALLID **ppstCallId);


/**
 * @brief SIP message get subject.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstSubject Subject description.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSubject(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstSubject);

/**
 * @brief SIP message get contact parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParm Parameter in contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetContactParm(ST_SIP_MSG *pstMsg,
                ST_SIP_CONTACT_PARM **ppstParm);

/**
 * @brief SIP message get contact addr spec.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstAddrSpec Addr-spec data strcture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetContactAddrSpec(ST_SIP_MSG *pstMsg,
                ST_SIP_ADDR_SPEC **ppstAddrSpec);

/**
 * @brief SIP message get accept contact value.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstVal Contact value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetAcptContactVal(ST_SIP_MSG *pstMsg,
                ST_SIP_AC_VAL **ppstVal);

/**
 * @brief SIP message get accept contact value.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] iIndex Index value in same type headers.
 * @param [out] ppstVal Contact value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetAcptContactValX(ST_SIP_MSG *pstMsg, ZINT iIndex,
                ST_SIP_AC_VAL **ppstVal);

/**
 * @brief SIP message find contact parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucParmType Parameter value, see EN_SIP_AC_PARMS.
 *
 * @retval ZTRUE if parameter exist, otherwise return ZFALSE.
 */
ZFUNC ZBOOL Sip_MsgFindContactParm(ST_SIP_MSG *pstMsg,
                ZUCHAR ucParmType);

/**
 * @brief SIP message find contact parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Parameter name.
 * @param [in] pcVal Parameter value.
 *
 * @retval ZTRUE if parameter exist, otherwise return ZFALSE.
 */
ZFUNC ZBOOL Sip_MsgFindContactParmX(ST_SIP_MSG *pstMsg,
                ZCHAR *pcName, ZCHAR *pcVal);

/**
 * @brief SIP message find accept contact parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucParmType Parameter value, see EN_SIP_AC_PARMS.
 *
 * @retval ZTRUE if parameter exist, otherwise return ZFALSE.
 */
ZFUNC ZBOOL Sip_MsgFindAcptContactParm(ST_SIP_MSG *pstMsg,
                ZUCHAR ucParmType);

/**
 * @brief SIP message find accept contact generic parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcName Parameter name.
 * @param [in] pcVal Parameter value.
 *
 * @retval ZTRUE if parameter exist, otherwise return ZFALSE.
 */
ZFUNC ZBOOL Sip_MsgFindAcptContactParmX(ST_SIP_MSG *pstMsg,
                ZCHAR *pcName, ZCHAR *pcVal);

/**
 * @brief SIP message find content encoding.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pcCoding Content coding.
 *
 * @retval ZTRUE if encoding exist, otherwise return ZFALSE.
 */
ZFUNC ZBOOL Sip_MsgFindContentEncoding(ST_SIP_MSG *pstMsg,
                ZCHAR *pcCoding);

/**
 * @brief SIP message get content-type, mtype see EN_SIP_MTYPE, 
 *        subtype see EN_SIP_MSUBTYPE.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucMtype Type of m-type, @ref EN_SIP_MTYPE.
 * @param [out] pucMSubtype Type of sub-type, @ref EN_SIP_MSUBTYPE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetContentType(ST_SIP_MSG *pstMsg, ZUCHAR *pucMtype,
                ZUCHAR *pucMSubtype);

/**
 * @brief SIP message get cseq value.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piCseq CSeq value.
 * @param [out] pucMethod SIP Method EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetCseq(ST_SIP_MSG *pstMsg, ZUINT *piCseq,
                ZUCHAR *pucMethod);

/**
 * @brief SIP message get date.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstDate Date value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetDate(ST_SIP_MSG *pstMsg, ST_SIP_DATE **ppstDate);

/**
 * @brief SIP message get date.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstDate Date value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetDateSysTime(ST_SIP_MSG *pstMsg, ST_ZOS_SYS_TIME *pstTime);

/**
 * @brief SIP message get expires.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piExpires Expires value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetExpires(ST_SIP_MSG *pstMsg, ZUINT *piExpires);

/**
 * @brief SIP message get name spec in From or To header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [out] ppstNaSpec Name-addr or addr-spec data strcuture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetFromToNaSpec(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_SIP_NA_SPEC **ppstNaSpec);

/**
 * @brief SIP message get addr spec in From or To header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromHdr If ZTRUE, object is From header. Or it is To header.
 * @param [out] ppstAddrSpec Addr-spec data strcture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetFromToAddrSpec(ST_SIP_MSG *pstMsg, 
                ZBOOL bFromHdr, ST_SIP_ADDR_SPEC **ppstAddrSpec);

/**
 * @brief SIP message get from-tag or to-tag.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] bFromTag If ZTRUE, tag is for From header. Or is for To header.
 * @param [out] ppstFromToTag Tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetFromToTag(ST_SIP_MSG *pstMsg, ZBOOL bFromTag,
                ST_ZOS_SSTR **ppstFromToTag);

/**
 * @brief SIP message get name spec in Refer-To header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstNaSpec Name-addr or addr-spec data strcuture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReferToNaSpec(ST_SIP_MSG *pstMsg,
                ST_SIP_NA_SPEC **ppstNaSpec);

/**
 * @brief SIP message get addr spec in Refer-To header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstAddrSpec Addr-spec data strcture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReferToAddrSpec(ST_SIP_MSG *pstMsg,
                ST_SIP_ADDR_SPEC **ppstAddrSpec);

/**
 * @brief SIP message get name spec in Refer-By header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstNaSpec Name-addr or addr-spec data strcuture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReferByNaSpec(ST_SIP_MSG *pstMsg,
                ST_SIP_NA_SPEC **ppstNaSpec);

/**
 * @brief SIP message get addr spec in Refer-By header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstAddrSpec Addr-spec data strcture.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReferByAddrSpec(ST_SIP_MSG *pstMsg,
                ST_SIP_ADDR_SPEC **ppstAddrSpec);

/**
 * @brief SIP message get refer-sub value.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pbValue Refer-Sub value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReferSub(ST_SIP_MSG *pstMsg, ZBOOL *pbValue);

/**
 * @brief SIP message get support option tag.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstOptTagLst Option tag list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSuptOptTag(ST_SIP_MSG *pstMsg,
                ST_SIP_OPT_TAG_LST **ppstOptTagLst);

/**
 * @brief SIP message get support flag SIP_TAG_OPT_ADD_100REL...
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piSuptFlag Support tag flag value, @ref SIP_TAG_OPT_ADD_100REL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSuptFlag(ST_SIP_MSG *pstMsg, ZUINT *piSuptFlag);

/**
 * @brief SIP message get require option tag.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstOptTagLst Option tag list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRequireOptTag(ST_SIP_MSG *pstMsg,
                ST_SIP_OPT_TAG_LST **ppstOptTagLst);

/**
 * @brief SIP message get require flag SIP_TAG_OPT_ADD_100REL...
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piRequireFlag Require tag flag value, @ref SIP_TAG_OPT_ADD_100REL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRequireFlag(ST_SIP_MSG *pstMsg, 
                ZUINT *piRequireFlag);

/**
 * @brief SIP message retry after timer.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piDeltaSec Retry after delta seconds.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRetryAfter(ST_SIP_MSG *pstMsg, 
                ZUINT *piDeltaSec);

/**
 * @brief SIP message get record route.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParmLst Parameter list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRecRoute(ST_SIP_MSG *pstMsg,
                ST_SIP_ROUTE_PARM_LST **ppstParmLst);

/**
 * @brief SIP message get route.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParmLst Parameter list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRoute(ST_SIP_MSG *pstMsg,
                ST_SIP_ROUTE_PARM_LST **ppstParmLst);

/**
 * @brief SIP message get route parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRouteParm(ST_SIP_MSG *pstMsg,
                ST_SIP_ROUTE_PARM **ppstParm);

/**
 * @brief SIP message get path.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParmLst Parameter list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetPath(ST_SIP_MSG *pstMsg,
                ST_SIP_ROUTE_PARM_LST **ppstParmLst);

/**
 * @brief SIP message get privacy value EN_SIP_PRIV_VAL.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstVal Value data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetPrivacyVal(ST_SIP_MSG *pstMsg, 
                ST_SIP_PRIV_VAL **ppstVal);

/**
 * @brief SIP message get service route.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParmLst Parameter list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetServRoute(ST_SIP_MSG *pstMsg,
                ST_SIP_ROUTE_PARM_LST **ppstParmLst);

/**
 * @brief SIP message get service route.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParmLst Parameter list.
 * @param [in] iIndex Index value in same type headers.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetServRouteX(ST_SIP_MSG *pstMsg,
                ST_SIP_ROUTE_PARM_LST **ppstParmLst, ZUINT iIndex);

/**
 * @brief SIP message get WWW-Authe[in] /
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstChallenge SIP challenge data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetWwwAuthen(ST_SIP_MSG *pstMsg, 
                ST_SIP_CHALLENGE **ppstChallenge);

/**
 * @brief SIP message get Proxy-Authenticate.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstChallenge SIP challenge data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetProxyAuthen(ST_SIP_MSG *pstMsg, 
                ST_SIP_CHALLENGE **ppstChallenge);

/**
 * @brief SIP message get Proxy-Authorization.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstCredents SIP credents.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetProxyAuthor(ST_SIP_MSG *pstMsg, 
                ST_SIP_CREDENTS **ppstCredents);

/**
 * @brief SIP message get rseq value.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piRspNum RSeq value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRseq(ST_SIP_MSG *pstMsg, ZUINT *piRspNum);

/**
 * @brief SIP message get rack value.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piCseqNum CSeq value.
 * @param [out] piRspNum RSeq value.
 * @param [out] pucMethod Method type, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetRack(ST_SIP_MSG *pstMsg, ZUINT *piCseqNum, 
                ZUINT *piRspNum, ZUCHAR *pucMethod);

/**
 * @brief SIP message get event, pkgtype see EN_SIP_EVNT_PKG, 
 *        temptype see EN_SIP_EVNT_TEMP.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucPkgType Event package type, @ref EN_SIP_EVNT_PKG.
 * @param [out] pucTempType Event template type, @ref EN_SIP_EVNT_TEMP.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetEvnt(ST_SIP_MSG *pstMsg, ZUCHAR *pucPkgType,
                ZUCHAR *pucTempType);

/**
 * @brief SIP message get event package, type see EN_SIP_EVNT_PKG.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucPkgType Event package type, @ref EN_SIP_EVNT_PKG.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetEvntPkg(ST_SIP_MSG *pstMsg, ZUCHAR *pucPkgType);

/**
 * @brief SIP message get replaces.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstCallId Call-ID data strcuture.
 * @param [out] ppstParmLst Parameter list.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReplaces(ST_SIP_MSG *pstMsg, 
                ST_SIP_CALLID **ppstCallId,
                ST_SIP_REPLACES_PARM_LST **ppstParmLst);

/**
 * @brief SIP message get max forwards.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piMaxForward Max forward value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetMaxForward(ST_SIP_MSG *pstMsg, 
                ZUINT *piMaxForward);

/**
 * @brief SIP message get min expires.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piExpires Expires value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetMinExpires(ST_SIP_MSG *pstMsg, 
                ZUINT *piExpires);

/**
 * @brief SIP message get session expires.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piSecond Session expire seconds.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSessExpires(ST_SIP_MSG *pstMsg, 
                ZUINT *piSecond);

/**
 * @brief SIP message get session refresher.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucRefPres The refresher present flag.
 * @param [out] pucUasPres The UAS present flag.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSessRefresher(ST_SIP_MSG *pstMsg, 
                ZUCHAR *pucRefPres, ZUCHAR *pucUasPres);

/**
 * @brief SIP message get Min-SE.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piSecond Minumum session time.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetMinSe(ST_SIP_MSG *pstMsg, ZUINT *piSecond);

/**
 * @brief SIP message get subscription state, state see EN_SIP_SUBSTA_VAL.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] pucStaType Subscribe status type, @ref EN_SIP_SUBSTA_VAL.
 * @param [out] piExpires Expires value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSubsSta(ST_SIP_MSG *pstMsg, ZUCHAR *pucStaType,
                ZUINT *piExpires);

/**
 * @brief SIP message get subscription reason, reason see EN_SIP_EVNT_REASON_VAL.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucReason Event reason type, @ref EN_SIP_EVNT_REASON_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSubsReason(ST_SIP_MSG *pstMsg, 
                ZUCHAR *pucReason);

/**
 * @brief SIP message get SIP-etag.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstEtag Entity tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetEtag(ST_SIP_MSG *pstMsg, ST_ZOS_SSTR **ppstEtag);

/**
 * @brief SIP message get user agent.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstName Name string.
 * @param [out] ppstVer Version string.
 * @param [out] ppstComm Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetUserAgent(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstName, ST_ZOS_SSTR **ppstVer, 
                ST_ZOS_SSTR **ppstComm);

/**
 * @brief SIP message get via branch.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstBranch The via branch.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetViaBranch(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstBranch);

/**
 * @brief SIP message get server.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstName Name string.
 * @param [out] ppstVer Version string.
 * @param [out] ppstComm Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetServer(ST_SIP_MSG *pstMsg, ST_ZOS_SSTR **ppstName,
                ST_ZOS_SSTR **ppstVer, ST_ZOS_SSTR **ppstComm);

/**
 * @brief SIP message get PAssertedID-value parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetPassertId(ST_SIP_MSG *pstMsg,
                ST_SIP_PASSERTEDID_VAL **ppstParm);

/**
 * @brief SIP message get PPreferredID-value parameter.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetPPreferId(ST_SIP_MSG *pstMsg,
                ST_SIP_PPREFERREDID_VAL **ppstParm);

/**
 * @brief SIP message get Warning header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] piCode Warning code.
 * @param [in] ppstAgent Warning agent.
 * @param [in] ppstText Warning text.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetWarn(ST_SIP_MSG *pstMsg, ZUINT *piCode,
                ST_ZOS_SSTR **ppstAgent, ST_ZOS_SSTR **ppstText);

/**
 * @brief SIP message get Warning header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] piProtoType Protocol type.
 * @param [in] piCause Cause value.
 * @param [in] ppstText Warning text.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetReason(ST_SIP_MSG *pstMsg, ZUINT *piProtoType,
                ZUINT *piCause, ST_ZOS_SSTR **ppstText);

/**
 * @brief SIP message get Session-Replaces header.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ppstReplaces Session-Replaces value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetSessReplace(ST_SIP_MSG *pstMsg,
                ST_ZOS_SSTR **ppstReplaces);

/**
 * @brief SIP message get SDP body.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstSdp SDP data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetBodySdp(ST_SIP_MSG *pstMsg, 
                ST_SDP_SESS_DESC **ppstSdp);

/**
 * @brief SIP message get string body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] ucMSubtype Value of m-subtype, @ref EN_SIP_MSUBTYPE.
 * @param [out] ppstStr The Body string.
 * @param [out] ppstMpart The Multipart body, use can add header in it.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetBodyStr(ST_SIP_MSG *pstMsg, ZUCHAR ucMtype,
                ZUCHAR ucMSubType, ST_ZOS_SSTR **ppstStr,
                ST_SIP_BODY_MPART **ppstMpart);

/**
 * @brief SIP message get string body.
 *
 * @param [in] pstMsg SIP message.
 * @param [in] ucMtype Value of m-type, @ref EN_SIP_MTYPE.
 * @param [out] ppstStr The Body string.
 * @param [out] ppstMpart The Multipart body, use can add header in it.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetBodyStrX(ST_SIP_MSG *pstMsg, ZUCHAR ucMtype, 
                ST_ZOS_SSTR **ppstStr, ST_SIP_BODY_MPART **ppstMpart);

/**
 * @brief SIP message get sipfrag body.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ppstSfrag The sipfrag body.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetBodySfrag(ST_SIP_MSG *pstMsg,
                ST_SIP_BODY_SFRAG **ppstSfrag);

/**
 * @brief SIP message get sipfrag status code in body.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] piStatCode The sipfrag status code.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetBodySfragRsp(ST_SIP_MSG *pstMsg,
                ZUINT *piStatCode);

/**
 * @brief SIP message get Conversation-ID.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pstConvId The string of Conversation-ID.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetConvId(ST_SIP_MSG *pstMsg, ST_ZOS_SSTR *pstConvId);

/**
 * @brief SIP message get Contribution-ID.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pstConvId The string of Contribution-ID.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetContId(ST_SIP_MSG *pstMsg, ST_ZOS_SSTR *pstContId);

/**
 * @brief SIP message get InReplyTo-Contribution-ID.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pstConvId The string of InReplyTo-Contribution-ID.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetIrtContId(ST_SIP_MSG *pstMsg, 
                ST_ZOS_SSTR *pstIrtContId);

/**
 * @brief SIP message get Accept.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucMtype Type of m-type, @ref EN_SIP_MTYPE.
 * @param [out] pucMSubtype Type of sub-type, @ref EN_SIP_MSUBTYPE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetAcpt(ST_SIP_MSG *pstMsg, ZUCHAR *pucMtype,
                ZUCHAR *pucMSubtype);

/**
 * @brief SIP message get X-ZTE-Cause string.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pstZTECause string of X-ZTE-Cause. 
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetZTECause(ST_SIP_MSG *pstMsg, ST_ZOS_SSTR *pstZTECause);

/**
 * @brief SIP message get ST type in ServInfo header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ucStType ST type of ServInfo. 
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetServInfoStType(ST_SIP_MSG *pstMsg, ZUCHAR *ucStType);

/**
 * @brief SIP message get Sub-ST type in ServInfo header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] ucSubStType Sub-ST type of ServInfo. 
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetServInfoSubStType(ST_SIP_MSG *pstMsg, ZUCHAR *ucSubStType);

/**
 * @brief SIP message get media types in Conference-Info header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucMediaTypemedia types of Conference-Info. 
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetConfInfoMediaTypes(ST_SIP_MSG *pstMsg, ZUCHAR *pucMediaType);

/**
 * @brief SIP message get seat types inTerminalSeatType header.
 *
 * @param [in] pstMsg SIP message.
 * @param [out] pucType type of TerminalSeatType. 
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_MsgGetTermSeatType(ST_SIP_MSG *pstMsg, ZUCHAR *pucType);

/**
 * @brief SIP header fill Cseq.
 *
 * @param [in] pstCseq CSeq header.
 * @param [in] iCseqVal CSeq value.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillCseq(ST_SIP_HDR_CSEQ *pstCseq, ZUINT iCseqVal,
                ZUCHAR ucMethod);

/**
 * @brief SIP header fill Contact.
 *
 * @param [in] zBufId The buffer id.
 * @param [in] pstContact CSeq header.
 * @param [in] pstAddrSpec Specific address.
 * @param [in] pstAddr Network address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillContact(ZUBUF zBufId, 
                ST_SIP_HDR_CONTACT *pstContact, ST_SIP_ADDR_SPEC *pstAddrSpec, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP header fill Contact.
 *
 * @param [in] zBufId The buffer id.
 * @param [in] pstContact CSeq header.
 * @param [in] pstUserInfo User information.
 * @param [in] pstAddr Network address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillContactX(ZUBUF zBufId, 
                ST_SIP_HDR_CONTACT *pstContact, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP header fill From or To by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByIp(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName, 
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr, 
                ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pcDispName Display name string.
 * @param [in] pcUserInfo User information string.
 * @param [in] pstAddr IP address.
 * @param [in] pcTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByIpX(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ZCHAR *pcDispName,
                ZCHAR *pcUserInfo, ST_ZOS_INET_ADDR *pstAddr, ZCHAR *pcTag);

/**
 * @brief SIP header fill From or To by SIPs IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToBySipsIp(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr, 
                ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByName(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pcDispName Display name string.
 * @param [in] pcUserInfo User information string.
 * @param [in] pcHostname Host name string.
 * @param [in] iPort Port value.
 * @param [in] pcTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByNameX(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ZCHAR *pcDispName,
                ZCHAR *pcUserInfo, ZCHAR *pcHostname, 
                ZUINT iPort, ZCHAR *pcTag);

/**
 * @brief SIP header fill From or To by SIPs hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToBySipsName(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUri URI string.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByUri(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUri, ST_ZOS_SSTR *pstTag);


/**
 * @brief SIP header fill From or To by URI add isfocus-parm.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUri URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByUriIsfs(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by SIP URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstSipUri SIP URI.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToBySipUri(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_SIP_SIP_URI *pstSipUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by SIPs URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstSipsUri SIP URI data structure.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToBySipsUri(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_SIP_SIP_URI *pstSipsUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by tel URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstTelUri Tel URI.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByTelUri(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_SIP_TEL_URI *pstTelUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill From or To by im URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstImUri IM URI string.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillFromToByImUri(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstDispName,
                ST_SIP_IM_URI *pstImUri, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP header fill refer-to header by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferTo Refer-To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferToByIp(ZUBUF zBufId, 
                ST_SIP_HDR_REFER_TO *pstReferTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP header fill refer-to header by SIPs IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferTo Refer-To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferToBySipsIp(ZUBUF zBufId, 
                ST_SIP_HDR_REFER_TO *pstReferTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP header fill refer-to header by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferTo Refer-To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferToByName(ZUBUF zBufId, 
                ST_SIP_HDR_REFER_TO *pstReferTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort);

/**
 * @brief SIP header fill refer-to header by SIPs hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferTo Refer-To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferToBySipsName(ZUBUF zBufId, 
                ST_SIP_HDR_REFER_TO *pstReferTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort);

/**
 * @brief SIP header fill refer-to header by URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferTo Refer-To header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUri URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferToByUri(ZUBUF zBufId, 
                ST_SIP_HDR_REFER_TO *pstReferTo, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUri);

/**
 * @brief SIP header fill referred-by header by IP address
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferredBy Refer-by header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferredByByIp(ZUBUF zBufId, 
                ST_SIP_HDR_REFERRED_BY *pstReferredBy, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP header fill referred-by header by SIPs IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferredBy Refer-by header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferredByBySipsIp(ZUBUF zBufId, 
                ST_SIP_HDR_REFERRED_BY *pstReferredBy, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP header fill referred-by header by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferredBy Refer-by header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferredByByName(ZUBUF zBufId, 
                ST_SIP_HDR_REFERRED_BY *pstReferredBy, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort);

/**
 * @brief SIP header fill referred-by header by SIPs hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferredBy Refer-by header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferredByBySipsName(ZUBUF zBufId, 
                ST_SIP_HDR_REFERRED_BY *pstReferredBy, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_SSTR *pstHostname, 
                ZUINT iPort);

/**
 * @brief SIP header fill referred-by header by URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferredBy Refer-by header data structure.
 * @param [in] pstDispName Display name string.
 * @param [in] pstUri URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillReferredByByUri(ZUBUF zBufId, 
                ST_SIP_HDR_REFERRED_BY *pstReferredBy, ST_ZOS_SSTR *pstDispName,
                ST_ZOS_SSTR *pstUri);

/**
 * @brief SIP header fill Content-Disposition header by URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstContentDisp Content-Disposition header data structure.
 * @param [in] pcDisp Display Content disposition string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillContentDisp(ZUBUF zBufId, 
                ST_SIP_HDR_CONTENT_DISP *pstContentDisp, ZCHAR *pcDisp);

/**
 * @brief SIP header fill Content-Encoding header by URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstContentDisp Content-Encoding header data structure.
 * @param [in] pcDisp Display Content encoding string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillContentEncoding(ZUBUF zBufId, 
                ST_SIP_HDR_CONTENT_ENCODING *pstContentEncoding, ZCHAR *pcDisp);

/**
 * @brief SIP header fill user-agent header.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAgent User-agent header data structure.
 * @param [in] pstProductName Product name string.
 * @param [in] pstProductVer Product version string.
 * @param [in] pstComment Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillUserAgent(ZUBUF zBufId, 
                ST_SIP_HDR_USER_AGENT *pstAgent, ST_ZOS_SSTR *pstProductName, 
                ST_ZOS_SSTR *pstProductVer, ST_ZOS_SSTR *pstComment);

/**
 * @brief SIP header fill server header.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstServer Server header data structure.
 * @param [in] pstProductName Product name string.
 * @param [in] pstProductVer Product version string.
 * @param [in] pstComment Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillServer(ZUBUF zBufId, 
                ST_SIP_HDR_SERVER *pstServer, ST_ZOS_SSTR *pstProductName, 
                ST_ZOS_SSTR *pstProductVer, ST_ZOS_SSTR *pstComment);

/**
 * @brief SIP header fill Event header.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstEvnt Event header data structure.
 * @param [in] pstEvntPkg SIP event package.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillEvnt(ZUBUF zBufId, ST_SIP_HDR_EVNT *pstEvnt, 
                ST_SIP_EVNT_PKG *pstEvntPkg);

/**
 * @brief SIP header fill Subscription-State header, state see EN_SIP_SUBSTA_VAL.
 *
 * @param [in] pstSubsSta Subscription-State header data structure.
 * @param [in] ucState Subscription status value, @ref EN_SIP_SUBSTA_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillSubsSta(ST_SIP_HDR_SUBS_STA *pstSubsSta, 
                ZUCHAR ucState);

/**
 * @brief SIP header fill Subscription-State header, state see EN_SIP_SUBSTA_VAL.
 *
 * @param [in] pstSubsSta Subscription-State header data structure.
 * @param [in] ucState Subscription status value, @ref EN_SIP_SUBSTA_VAL.
 * @param [in] ucReason Event reason, @ref EN_SIP_EVNT_REASON_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillSubsStaX(ZUBUF zBufId, 
                ST_SIP_HDR_SUBS_STA *pstSubsSta, ZUCHAR ucState, 
                ZUCHAR ucReason);

/**
 * @brief SIP header fill Subscription-State header, state see EN_SIP_SUBSTA_VAL.
 *
 * @param [in] pstSubsSta Subscription-State header data structure.
 * @param [in] ucState Subscription status value, @ref EN_SIP_SUBSTA_VAL.
 * @param [in] ucReason Event reason, @ref EN_SIP_EVNT_REASON_VAL.
 * @param [in] iExpires Expires.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillSubsStaY(ZUBUF zBufId, 
                ST_SIP_HDR_SUBS_STA *pstSubsSta, ZUCHAR ucState, 
                ZUCHAR ucReason, ZUINT iExpires);

/**
 * @brief SIP header fill RAck, method see EN_SIP_METHOD.
 *
 * @param [in] pstRack RAck header data structure.
 * @param [in] iCseqNum CSeq value.
 * @param [in] iRspNum RSeq value.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillRAck(ST_SIP_HDR_RACK *pstRack, ZUINT iCseqNum,
                ZUINT iRspNum, ZUCHAR ucMethod);

/**
 * @brief SIP header fill Rseq.
 *
 * @param [in] pstRseq RSeq header data structure.
 * @param [in] iRspNum RSeq value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillRseq(ST_SIP_HDR_RSEQ *pstRseq, ZUINT iRspNum);

/**
 * @brief SIP header fill Privacy, value see EN_SIP_PRIV_VAL
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstPrivacy Privacy header data structure.
 * @param [in] ucPrivVal Privacy value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillPrivacy(ZUBUF zBufId, 
                ST_SIP_HDR_PRIVACY *pstPrivacy, ZUCHAR ucPrivVal);

/**
 * @brief SIP From or To header add tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFromToAddTag(ZUBUF zBufId, 
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP From or To header add tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pcTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFromToAddTagX(ZUBUF zBufId,
                ST_SIP_HDR_FROM_TO *pstFromTo, ZCHAR *pcTag);

/**
 * @brief SIP From or To header init with display name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstFromTo From or To header data structure.
 * @param [in] pstName Name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFromToInitDispName(ZUBUF zBufId,
                ST_SIP_HDR_FROM_TO *pstFromTo, ST_ZOS_SSTR *pstName);

/**
 * @brief SIP From or To header get addr spec.
 *
 * @param [in] pstFromTo From or To header data structure.
 * @param [out] ppstAddrSpec Addr-spec data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFromToGetAddrSpec(ST_SIP_HDR_FROM_TO *pstFromTo,
                ST_SIP_ADDR_SPEC **ppstAddrSpec);

/**
 * @brief SIP header from or to header get tag.
 *
 * @param [in] pstFromTo From or To header data structure.
 * @param [out] ppstTag Tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFromToGetTag(ST_SIP_HDR_FROM_TO *pstFromTo,
                ST_ZOS_SSTR **ppstTag);

/**
 * @brief SIP Accept header add method.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcpt Accept header data structure.
 * @param [in] ucMType Value of m-type, @ref EN_SIP_MTYPE.
 * @param [in] ucSType Value of sub-type, @ref EN_SIP_MSUBTYPE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrAcptAddRange(ZUBUF zBufId, 
                ST_SIP_HDR_ACPT *pstAcpt, ZUCHAR ucMType, ZUCHAR ucSType);

/**
 * @brief SIP Allow header add method.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAllow Allow header data structure.
 * @param [in] pstMethod SIP method data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrAllowAddMethod(ZUBUF zBufId,
                ST_SIP_HDR_ALLOW *pstAllow, ST_SIP_METHOD *pstMethod);

/**
 * @brief SIP Allow-Events header add method.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAllowEvnts Allow-Events header data structure.
 * @param [in] ucPkgType Event pacakge type, @ref EN_SIP_EVNT_PKG.
 * @param [in] ucTempType Event template type, @ref EN_SIP_EVNT_TEMP.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrAllowEvntAddEvnt(ZUBUF zBufId,
                ST_SIP_HDR_ALLOW_EVNTS *pstAllowEvnts, ZUCHAR ucPkgType, 
                ZUCHAR ucTempType);

/**
 * @brief SIP Supported header add option-tag, type see EN_SIP_OPT_TAG.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSupted Support header data structure.
 * @param [in] ucType Oprtion tag type, @ref EN_SIP_OPT_TAG.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrSuptedAddTag(ZUBUF zBufId,
                ST_SIP_HDR_SUPTED *pstSupted, ZUCHAR ucType);

/**
 * @brief SIP Supported header add option-tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSupted Support header data structure.
 * @param [in] pcTag Tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrSuptedAddTagX(ZUBUF zBufId,
                ST_SIP_HDR_SUPTED *pstSupted, ZCHAR *pcTag);

/**
 * @brief SIP Require header add option-tag, type see EN_SIP_OPT_TAG.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstRequire Require header data structure.
 * @param [in] ucType Oprtion tag type, @ref EN_SIP_OPT_TAG.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrRequireAddTag(ZUBUF zBufId,
                ST_SIP_HDR_REQUIRE *pstRequire, ZUCHAR ucType);

/**
 * @brief SIP Require header add option-tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstRequire Require header data structure.
 * @param [in] pcTag Tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrRequireAddTagX(ZUBUF zBufId,
                ST_SIP_HDR_REQUIRE *pstRequire, ZCHAR *pcTag);

/**
 * @brief SIP Refer-To header add SIP uri header.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferto Refer-To header data structure.
 * @param [in] pcName Name string.
 * @param [in] wNameLen Name string length.
 * @param [in] pcVal Value string.
 * @param [in] wValLen Value string length.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrReferToAddHdr(ZUBUF zBufId,
                ST_SIP_HDR_REFER_TO *pstReferto, ZCHAR *pcName,
                ZUSHORT wNameLen, ZCHAR *pcVal, ZUSHORT wValLen);

/**
 * @brief SIP Refer-To header add parameter.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferto Refer-To header data structure.
 * @param [in] pcName Name string.
 * @param [in] pcVal Value string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrReferToAddParm(ZUBUF zBufId,
                ST_SIP_HDR_REFER_TO *pstReferto, ZCHAR *pcName, ZCHAR *pcVal);

/**
 * @brief SIP Refer-To header add parameter.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReferto Refer-To header data structure.
 * @param [in] pcName Name string.
 * @param [in] pcVal Value string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrReferToAddParmX(ZUBUF zBufId,
                ST_SIP_HDR_REFER_TO *pstReferto, ZCHAR *pcName, 
                ZBOOL bQStr, ZCHAR *pcVal);

/**
 * @brief SIP Refer-To header get parameter value.
 *
 * @param [in] pstReferto Refer-To header data structure.
 * @param [in] pcName Name string.
 * @param [out] ppstVal Value string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrReferToGetParmVal(ST_SIP_HDR_REFER_TO *pstReferto, 
                ZCHAR *pcName, ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP Event header add event-param id.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstEvnt Event header data structure.
 * @param [in] pstId ID string of event.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrEvntAddId(ZUBUF zBufId, ST_SIP_HDR_EVNT *pstEvnt, 
                ST_ZOS_SSTR *pstId);

/**
 * @brief SIP Subscription-State header add reason value, 
 *        type see EN_SIP_EVNT_REASON_VAL.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSubsSta Subscription-State header data structure.
 * @param [in] ucType Subscription status type, @ref EN_SIP_EVNT_REASON_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrSubsStaAddReasonVal(ZUBUF zBufId,
                ST_SIP_HDR_SUBS_STA *pstSubsSta, ZUCHAR ucType);

/**
 * @brief SIP Subscription-State header add expires.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSubsSta Subscription-State header data structure.
 * @param [in] iExpires Expires value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrSubsStaAddExpires(ZUBUF zBufId,
                ST_SIP_HDR_SUBS_STA *pstSubsSta, ZUINT iExpires);

/**
 * @brief SIP Subscription-State header add retry-after.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSubsSta Subscription-State header data structure.
 * @param [in] iRetryAfter Retry after value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrSubsStaAddRetryAfter(ZUBUF zBufId,
                ST_SIP_HDR_SUBS_STA *pstSubsSta, ZUINT iRetryAfter);

/**
 * @brief SIP Replaces header add from tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReplace Replace header data structure.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrReplaceAddFromTag(ZUBUF zBufId,
                ST_SIP_HDR_REPLACES *pstReplace, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP Replaces header add to tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReplace Replace header data structure.
 * @param [in] pstTag Tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrReplaceAddToTag(ZUBUF zBufId,
                ST_SIP_HDR_REPLACES *pstReplace, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP Replaces header add early tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReplace Replace header data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrReplaceAddEarlyTag(ZUBUF zBufId,
                ST_SIP_HDR_REPLACES *pstReplace);


/**
 * @brief SIP header alert get generic-param .
 *
 * @param [in] pstAlertInfo alert info data structure.
 * @param [in] pcName Generic parameter name.
 * @param [out] ppstVal Generic parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrAlertInfoGetParm(ST_SIP_HDR_ALERT_INFO *pstAlertInfo,
                ZCHAR *pcName, ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP Contact header get addr spec.
 *
 * @param [in] pstContact Contact header data structure.
 * @param [out] ppstAddrSpec Addr-spec data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrContactGetAddrSpece(ST_SIP_HDR_CONTACT *pstContact, 
                ST_SIP_ADDR_SPEC **ppstAddrSpec);

/**
 * @brief SIP Record-Route, Route header get addr spec.
 *
 * @param [in] pstRoute Record-Route or Route header data structure.
 * @param [out] ppstAddrSpec Addr-spec data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrRouteGetAddrSpec(ST_SIP_HDR_ROUTE *pstRoute,
                ST_SIP_ADDR_SPEC **ppstAddrSpec);

/**
 * @brief SIP Record-Route, Route header get uri-parameters, type see EN_SIP_URI_PARM.
 *
 * @param [in] pstRoute Record-Route or Route header data structure.
 * @param [in] ucType URI type, @ref EN_SIP_URI_PARM.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrRouteGetUriParm(ST_SIP_HDR_ROUTE *pstRoute,
                ZUCHAR ucType, ST_SIP_URI_PARM **ppstParm);

/**
 * @brief SIP parameter fill request line.
 *
 * @param [in] pstReqLine Request line in SIP message.
 * @param [in] iMajorVer Major version.
 * @param [in] iMinorVer Minor version.
 * @param [in] ucMethodType Method type, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqLine(ST_SIP_REQ_LINE *pstReqLine, 
                ZUINT iMajorVer, ZUINT iMinorVer, ZUCHAR ucMethodType);

/**
 * @brief SIP parameter fill status line.
 *
 * @param [in] pstStatusLine Status line of SIP message.
 * @param [in] iStatusCode Status code of response.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillStatusLine(ST_SIP_STATUS_LINE *pstStatusLine, 
                ZUINT iStatusCode);

/**
 * @brief SIP parameter fill SIP-URI/SIPS-URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostPort Host port in SIP message.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUri(ZUBUF zBufId, ST_SIP_SIP_URI *pstSipUri,
                ST_ZOS_SSTR *pstUserInfo, ST_SIP_HOST_PORT *pstHostPort);

/**
 * @brief SIP parameter fill SIPS-URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostPort Host port in SIP message.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipsUri(ZUBUF zBufId, ST_SIP_SIP_URI *pstSipUri,
                ST_ZOS_SSTR *pstUserInfo, ST_SIP_HOST_PORT *pstHostPort);

/**
 * @brief SIP parameter fill TEL-URI, if it is global, number should without '+'
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstTelUri Tel URI.
 * @param [in] bIsGlobal If ZTRUE, set as global number, or as local number.
 * @param [in] pstNumber Number string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillTelUri(ZUBUF zBufId, ST_SIP_TEL_URI *pstTelUri,
                ZBOOL bIsGlobal, ST_ZOS_SSTR *pstNumber);

/**
 * @brief SIP parameter fill IM-URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstImUri IM URI string.
 * @param [in] pstDesc Description string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillImUri(ZUBUF zBufId, ST_SIP_IM_URI *pstImUri,
                ST_ZOS_SSTR *pstDesc);

/**
 * @brief SIP parameter fill Request-URI SIP uri by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReqUri Request URI in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqUriByIp(ZUBUF zBufId, 
                ST_SIP_REQ_URI *pstReqUri, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill Request-URI SIP uri by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReqUri Request URI in SIP message.
 * @param [in] pcUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqUriByIpX(ZUBUF zBufId, 
                ST_SIP_REQ_URI *pstReqUri, ZCHAR *pcUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill Request-URI by SIPs IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReqUri Request URI in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqUriBySipsIp(ZUBUF zBufId,
                ST_SIP_REQ_URI *pstReqUri, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill Request-URI SIP uri by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReqUri Request URI in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqUriByName(ZUBUF zBufId,
                ST_SIP_REQ_URI *pstReqUri, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_SSTR *pstHostname, ZUINT iPort);

/**
 * @brief SIP parameter fill Request-URI SIP uri by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReqUri Request URI in SIP message.
 * @param [in] pcUserInfo User information string.
 * @param [in] pcHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqUriByNameX(ZUBUF zBufId,
                ST_SIP_REQ_URI *pstReqUri, ZCHAR *pcUserInfo, 
                ZCHAR *pcHostname, ZUINT iPort);

/**
 * @brief SIP parameter fill Request-URI by SIPs hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReqUri Request URI in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqUriBySipsName(ZUBUF zBufId,
                ST_SIP_REQ_URI *pstReqUri, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_SSTR *pstHostname, ZUINT iPort);

/**
 * @brief SIP parameter fill Request-URI by URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstReqUri Request URI in SIP message.
 * @param [in] pstUri URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReqUriByUri(ZUBUF zBufId, 
                ST_SIP_REQ_URI *pstReqUri, ST_ZOS_SSTR *pstUri);

/**
 * @brief SIP parameter fill addr-spec SIP uri by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecByIp(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill addr-spec SIP uri by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pcUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecByIpX(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ZCHAR *pcUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill addr-spec SIPs uri by IP address.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecBySipsIp(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_ZOS_SSTR *pstUserInfo, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill addr-spec SIP uri by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecByName(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_ZOS_SSTR *pstUserInfo,
                ST_ZOS_SSTR *pstHostname, ZUINT iPort);

/**
 * @brief SIP parameter fill addr-spec SIP uri by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pcUserInfo User information string.
 * @param [in] pcHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecByNameX(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ZCHAR *pcUserInfo,
                ZCHAR *pcHostname, ZUINT iPort);

/**
 * @brief SIP parameter fill addr-spec SIPs uri by hostname.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstUserInfo User information string.
 * @param [in] pstHostname Host name string.
 * @param [in] iPort Port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecBySipsName(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_ZOS_SSTR *pstUserInfo,
                ST_ZOS_SSTR *pstHostname, ZUINT iPort);

/**
 * @brief SIP parameter fill addr-spec by URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstUri URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecByUri(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_ZOS_SSTR *pstUri);

/**
 * @brief SIP parameter fill addr-spec by SIP URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstSipUri SIP URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecBySipUri(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP parameter fill addr-spec by SIPs URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstSipsUri SIP URI data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecBySipsUri(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_SIP_URI *pstSipsUri);

/**
 * @brief SIP parameter fill addr-spec by tel URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstTelUri Tel URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecByTelUri(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_TEL_URI *pstTelUri);

/**
 * @brief SIP parameter fill addr-spec by im URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstImUri IM URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAddrSpecByImUri(ZUBUF zBufId,
                ST_SIP_ADDR_SPEC *pstAddrSpec, ST_SIP_IM_URI *pstImUri);

/**
 * @brief SIP parameter name-addr or addr-spec set display name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstNaSpec Name-addr or addr-spec of SIP message.
 * @param [in] pstName Name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmNaSpecSetDispName(ZUBUF zBufId,
                ST_SIP_NA_SPEC *pstNaSpec, ST_ZOS_SSTR *pstName);

/**
 * @brief SIP parameter name-addr or addr-spec set display name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstNaSpec Name-addr or addr-spec of SIP message.
 * @param [in] pcName Name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmNaSpecSetDispNameX(ZUBUF zBufId,
                ST_SIP_NA_SPEC *pstNaSpec, ZCHAR *pcName);

/**
 * @brief SIP parameter name-addr or addr-spec get addr-spec.
 *
 * @param [in] pstNaSpec Name-addr or addr-spec of SIP message.
 * @param [out] ppstAddrSpec Addr-spec data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmNaSpecGetAddrSpec(ST_SIP_NA_SPEC *pstNaSpec,
                ST_SIP_ADDR_SPEC **ppstAddrSpec);
/**
 * @brief SIP parameter name-addr or addr-spec set display name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstNaSpec Name-addr or addr-spec of SIP message.
 * @param [in] pstName Name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmNaAddrSpecSetDispName(ZUBUF zBufId,
                ST_SIP_NA_SPEC *pstNaSpec, ST_ZOS_SSTR *pstName);

/**
 * @brief SIP parameter addr-spec get user information.
 *
 * @param [in] pstAddrSpec The addr-spec of SIP message.
 * @param [out] pstUserInfo The user information address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAddrSpecGetUserInfo(ST_SIP_ADDR_SPEC *pstAddrSpec,
                ST_ZOS_SSTR *pstUserInfo);

/**
 * @brief SIP parameter fill hostport.
 *
 * @param [in] pstHostPort Host port in SIP message.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillHostPort(ST_SIP_HOST_PORT *pstHostPort, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill via sent-protocol EN_SIP_TPT.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] ucTptType Transport protocol type, @ref EN_SIP_TPT.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillViaSentProtocol(ZUBUF zBufId,
                ST_SIP_VIA_PARM *pstViaParm, ZUCHAR ucTptType);

/**
 * @brief SIP parameter fill via sent-by.
 *
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillViaSentBy(ST_SIP_VIA_PARM *pstViaParm, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill via-branch.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] pstBranch Branch string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillViaBranch(ZUBUF zBufId, 
                ST_SIP_VIA_PARM *pstViaParm, ST_ZOS_SSTR *pstBranch);

/**
 * @brief SIP parameter fill via-received.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillViaRecv(ZUBUF zBufId, 
                ST_SIP_VIA_PARM *pstViaParm, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill via response-port.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] iRport Response-port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillViaRport(ZUBUF zBufId, 
                ST_SIP_VIA_PARM *pstViaParm, ZUINT iRport);

/**
 * @brief SIP parameter fill via alias.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstViaParm Parameter of Via header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillViaAlias(ZUBUF zBufId, 
                ST_SIP_VIA_PARM *pstViaParm);

/**
 * @brief SIP parameter fill via keep.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] iLen keep alive frequency in seconds, if present and with a value other than zero.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillViaKeep(ZUBUF zBufId, ST_SIP_VIA_PARM *pstViaParm, 
                ZUINT iLen);

/**
 * @brief SIP parameter pick up via sent-protocol EN_SIP_TPT.
 *
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] pucTptType Transport protocol type, @ref EN_SIP_TPT.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickViaSentProtocol(ST_SIP_VIA_PARM *pstViaParm, 
                ZUCHAR *pucTptType);

/**
 * @brief SIP parameter pick up via sent-by.
 *
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickViaSentBy(ST_SIP_VIA_PARM *pstViaParm, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter pick up via-branch.
 *
 * @param [in] pstViaParm Parameter of Via header.
 * @param [out] ppstBranch Branch string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickViaBranch(ST_SIP_VIA_PARM *pstViaParm, 
                ST_ZOS_SSTR **ppstBranch);

/**
 * @brief SIP parameter pick up via maddr.
 *
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickViaMaddr(ST_SIP_VIA_PARM *pstViaParm, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter pick up via-received.
 *
 * @param [in] pstViaParm Parameter of Via header.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickViaRecv(ST_SIP_VIA_PARM *pstViaParm, 
                ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter pick up via response-port.
 *
 * @param [in] pstViaParm Parameter of Via header.
 * @param [out] piRport Response-port value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickViaRport(ST_SIP_VIA_PARM *pstViaParm, 
                ZUINT *piRport);

/**
 * @brief SIP parameter fill event-type.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstEvntType Event type data structure.
 * @param [in] pstEvntPkg SIP event package.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillEvntType(ZUBUF zBufId,
                ST_SIP_EVNT_TYPE *pstEvntType, ST_SIP_EVNT_PKG *pstEvntPkg);

/**
 * @brief SIP parameter fill event package, type see EN_SIP_EVNT_PKG.
 *
 * @param [in] pstEvntPkg SIP event package.
 * @param [in] ucType Event package type, @ref EN_SIP_EVNT_PKG.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillEvntPkg(ST_SIP_EVNT_PKG *pstEvntPkg, 
                ZUCHAR ucType);

/**
 * @brief SIP parameter fill event package.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstEvntPkg SIP event package.
 * @param [in] pcOther Description string of event.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillEvntPkgX(ZUBUF zBufId, 
                ST_SIP_EVNT_PKG *pstEvntPkg, ZCHAR *pcOther);

/**
 * @brief SIP parameter fill event template.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstEvntType SIP event package.
 * @param [in] ucType Event template type, @ref EN_SIP_EVNT_TEMP.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillEvntTemp(ZUBUF zBufId, 
                ST_SIP_EVNT_TYPE *pstEvntType, ZUCHAR ucType);

/**
 * @brief SIP parameter fill event template.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstEvntType SIP event type.
 * @param [in] pcOther Other event.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillEvntTempX(ZUBUF zBufId, 
                ST_SIP_EVNT_TYPE *pstEvntType, ZCHAR *pcOther);

/**
 * @brief SIP parameter fill display name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDispName Display name string.
 * @param [in] pstName Name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDispName(ZUBUF zBufId, 
                ST_SIP_DISP_NAME *pstDispName, ST_ZOS_SSTR *pstName);

/**
 * @brief SIP parameter format normal string to quoted string.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pucStr The normal string.
 * @param [in] iLen The normal string length.
 * @param [out] ppucQStr The quoted string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFmtNStr2QStr(ZUBUF zBufId, ZUCHAR *pucStr, 
                ZUINT iLen, ZUCHAR **ppucQStr, ZUINT *piQLen);

/**
 * @brief SIP parameter format quoted string to normal string.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pucQStr The quoted string.
 * @param [in] wLen The normal string length.
 * @param [out] ppucStr The normal string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFmtQStr2NStr(ZUBUF zBufId, ZUCHAR *pucQStr, 
                ZUINT iQLen, ZUCHAR **ppucStr, ZUINT *piLen);

/**
 * @brief SIP parameter fill digest response of user name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstUser User name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspUserName(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstUser);

/**
 * @brief SIP parameter fill digest response of user name.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pcUser User name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspUserNameX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcUser);

/**
 * @brief SIP parameter fill digest response of realm.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstRealm Realm string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspRealm(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstRealm);

/* sip parameter fill digest response of realm */
ZFUNC ZINT Sip_ParmFillDRspRealmX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcRealm);

/**
 * @brief SIP parameter fill digest response of nonce.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstNonce Nonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspNonce(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstNonce);

/**
 * @brief SIP parameter fill digest response of nonce.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pcNonce Nonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspNonceX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcNonce);

/**
 * @brief SIP parameter fill digest response of cnonce.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstCNonce CNonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspCnonce(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstCNonce);

/**
 * @brief SIP parameter fill digest response of cnonce.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pcCNonce CNonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspCnonceX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcCNonce);

/**
 * @brief SIP parameter fill digest response of nonce-count.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstNonceCount Nonce count string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspNonceCount(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstNonceCount);

/**
 * @brief SIP parameter fill digest response of nonce-count.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pcNonceCount Nonce count string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspNonceCountX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcNonceCount);

/**
 * @brief SIP parameter fill digest response of auts.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstAuts Auts string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspAuts(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstAuts);

/**
 * @brief SIP parameter fill digest response of auts.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstAuts Auts string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspAutsX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcAuts);

/**
 * @brief SIP parameter fill digest response of digest URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstDigestUri Digest URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspUri(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_SIP_REQ_URI *pstDigestUri);

/**
 * @brief SIP parameter fill digest response of digest URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstDigestUri Digest URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspUriX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstDigestUri);

/**
 * @brief SIP parameter fill digest response of digest URI.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pcDigestUri Digest URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspUriY(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcDigestUri);

/**
 * @brief SIP parameter fill digest response of response.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pcRspStr Response URI string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspRsp(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZCHAR *pcRspStr);

/**
 * @brief SIP parameter fill digest response of opaque.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstOpaque Opaque string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspOpaque(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_ZOS_SSTR *pstOpaque);

/**
 * @brief SIP parameter fill digest response of qop.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstQop Qop data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspQop(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_SIP_TYPE_VAL *pstQop);

/**
 * @brief SIP parameter fill digest response of qop EN_SIP_QOP_VAL.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] ucQopType Qop type, @ref EN_SIP_QOP_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspQopX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZUCHAR ucQopType);

/**
 * @brief SIP parameter fill digest response of algorithm.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pstAlgo Algorithm data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspAlgo(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ST_SIP_ALGO *pstAlgo);

/**
 * @brief SIP parameter fill digest response of algorithm EN_SIP_ALGO_VAL
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] ucAlogType Algorithm type, @ref EN_SIP_ALGO_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDRspAlgoX(ZUBUF zBufId, 
                ST_SIP_DIGEST_RSP *pstDigestRsp, ZUCHAR ucAlogType);

/**
 * @brief SIP parameter pick up digest response of user name.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstUser User name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspUserName(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_ZOS_SSTR **ppstUser);

/**
 * @brief SIP parameter pick up digest response of realm.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstRealm Realm string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspRealm(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_ZOS_SSTR **ppstRealm);

/**
 * @brief SIP parameter pick up digest response of nonce.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstNonce Nonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspNonce(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_ZOS_SSTR **ppstNonce);

/**
 * @brief SIP parameter pick up digest response of cnonce.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstCNonce CNonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspCnonce(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_ZOS_SSTR **ppstCNonce);

/**
 * @brief SIP parameter pick up digest response of nonce-count.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstNonceCount Nonce count string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspNonceCount(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_ZOS_SSTR **ppstNonceCount);

/**
 * @brief SIP parameter pick up digest response of digest URI.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstDigestUri Digest URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspUri(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_SIP_REQ_URI **ppstDigestUri);

/**
 * @brief SIP parameter pick up digest response of response.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstRsp Digest response string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspRsp(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_ZOS_SSTR **ppstRsp);

/**
 * @brief SIP parameter pick up digest response of opaque.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstOpaque Opaque string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspOpaque(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_ZOS_SSTR **ppstOpaque);

/**
 * @brief SIP parameter pick up digest response of qop.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstQop Qop data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspQop(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_SIP_TYPE_VAL **ppstQop);

/**
 * @brief SIP parameter pick up digest response of qop EN_SIP_QOP_VAL.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pucQopType Qop type, @ref EN_SIP_QOP_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspQopX(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ZUCHAR *pucQopType);

/**
 * @brief SIP parameter pick up digest response of algorithm.
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [out] ppstAlgo Algorithm data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspAlgo(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ST_SIP_ALGO **ppstAlgo);

/**
 * @brief SIP parameter pick up digest response of algorithm EN_SIP_ALGO_VAL
 *
 * @param [in] pstDigestRsp Digest response data structure.
 * @param [in] pucAlogType Algorithm type, @ref EN_SIP_ALGO_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDRspAlgoX(ST_SIP_DIGEST_RSP *pstDigestRsp, 
                ZUCHAR *pucAlogType);

/**
 * @brief SIP parameter fill digest of realm.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pstRealm Realm string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestRealm(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ST_ZOS_SSTR *pstRealm);

/**
 * @brief SIP parameter fill digest of realm.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pcRealm Realm string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestRealmX(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ZCHAR *pcRealm);

/**
 * @brief SIP parameter fill digest of domain.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pstDomain Domain name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestDomain(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ST_ZOS_SSTR *pstDomain);

/**
 * @brief SIP parameter fill digest of domain.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pcDomain Domain name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestDomainX(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ZCHAR *pcDomain);

/**
 * @brief SIP parameter fill digest of nonce.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pstNonce Nonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestNonce(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ST_ZOS_SSTR *pstNonce);

/**
 * @brief SIP parameter fill digest of nonce.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pcNonce Nonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestNonceX(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ZCHAR *pcNonce);

/**
 * @brief SIP parameter fill digest of opaque.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pstOpaque Opaque string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestOpaque(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ST_ZOS_SSTR *pstOpaque);

/**
 * @brief SIP parameter fill digest of opaque.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pcOpaque Opaque string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestOpaqueX(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ZCHAR *pcOpaque);

/**
 * @brief SIP parameter fill digest of stale EN_SIP_STALE
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] ucStale Stale type, @ref EN_SIP_STALE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestStale(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ZUCHAR ucStale);

/**
 * @brief SIP parameter fill digest of algorithm.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pstAlgo Algorithm data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestAlgo(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ST_SIP_ALGO *pstAlgo);

/**
 * @brief SIP parameter fill digest of algorithm EN_SIP_ALGO_VAL
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] ucAlogType Algorithm type, @ref EN_SIP_ALGO_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestAlgoX(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ZUCHAR ucAlogType);

/**
 * @brief SIP parameter fill digest of qop-options.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pstQop Qop data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestQop(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ST_SIP_TYPE_VAL *pstQop);

/**
 * @brief SIP parameter fill digest of qop-options EN_SIP_QOP_VAL.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] ucQopType Qop type, @ref EN_SIP_QOP_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillDigestQopX(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ZUCHAR ucQopType);
                
/**
 * @brief SIP parameter pick up digest of realm.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [out] ppstRealm Realm string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestRealm(ST_SIP_DIGEST *pstDigest, 
                ST_ZOS_SSTR **ppstRealm);

/**
 * @brief SIP parameter pick up digest of domain.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstDigest Digest data structure.
 * @param [in] pstDomain Domain name string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestDomain(ZUBUF zBufId, 
                ST_SIP_DIGEST *pstDigest, ST_ZOS_SSTR *pstDomain);

/**
 * @brief SIP parameter pick up digest of nonce.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [out] ppstNonce Nonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestNonce(ST_SIP_DIGEST *pstDigest, 
                ST_ZOS_SSTR **ppstNonce);

/**
 * @brief SIP parameter pick up digest of opaque.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [out] ppstOpaque Opaque string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestOpaque(ST_SIP_DIGEST *pstDigest, 
                ST_ZOS_SSTR **ppstOpaque);

/**
 * @brief SIP parameter pick up digest of stale EN_SIP_STALE
 *
 * @param [in] pstDigest Digest data structure.
 * @param [in] pucStale Stale type, @ref EN_SIP_STALE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestStale(ST_SIP_DIGEST *pstDigest, 
                ZUCHAR *pucStale);
                
/**
 * @brief SIP parameter pick up digest of algorithm.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [out] ppstAlgo Algorithm data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestAlgo(ST_SIP_DIGEST *pstDigest, 
                ST_SIP_ALGO **ppstAlgo);

/**
 * @brief SIP parameter pick up digest of algorithm EN_SIP_ALGO_VAL
 *
 * @param [in] pstDigest Digest data structure.
 * @param [in] pucAlogType Algorithm type, @ref EN_SIP_ALGO_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestAlgoX(ST_SIP_DIGEST *pstDigest, 
                ZUCHAR *pucAlogType);
                
/**
 * @brief SIP parameter pick up digest of qop.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [out] ppstQop Qop data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestQop(ST_SIP_DIGEST *pstDigest, 
                ST_SIP_TYPE_VAL **ppstQop);

/**
 * @brief SIP parameter pick up digest of qop EN_SIP_QOP_VAL.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [in] pucQopType Qop type, @ref EN_SIP_QOP_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickDigestQopX(ST_SIP_DIGEST *pstDigest, 
                ZUCHAR *pucQopType);

/**
 * @brief SIP parameter fill contact-param.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 * @param [in] pstAddrSpec Addr-spec in SIP message.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactParm(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm,
                ST_SIP_ADDR_SPEC *pstAddrSpec, 
                ST_ZOS_INET_ADDR *pstAddr);

/* sip parameter fill contact-param, type see EN_SIP_ADDR_SPEC, only sip/sips */
ZFUNC ZINT Sip_ParmFillContactParmX(ZUBUF zBufId, 
                ST_SIP_CONTACT_PARM *pstParm, ZUCHAR ucAddrType,
                ST_ZOS_SSTR *pstUserInfo, ST_ZOS_INET_ADDR *pstAddr);

/**
 * @brief SIP parameter fill contact expires.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 * @param [in] iExpires Expires value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactExpires(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm, ZUINT iExpires);

/**
 * @brief SIP parameter fill contact Qval.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 * @param [in] wQint qvalue integer.
 * @param [in] wQfrac qvalue fraction.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactQval(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm, ZUSHORT wQint, ZUSHORT wQfrac);

/**
 * @brief SIP parameter fill contact isfocus.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactIsFocus(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact +g.poc.talkburst.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactPocTb(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact +g.3gpp.cs-voice.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContact3gppCv(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact +g.oma.SIP-im.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactOmaSipIm(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact +g.oma.SIP-im.large-message.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactOmaSipImLMsg(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact 3gpp-service.ims.icsi.oma.cpm.msg.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactOmaCpm(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact 3gpp-service.ims.icsi.oma.cpm.largemsg.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactOmaCpmLMsg(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact 3gpp-service.ims.icsi.oma.cpm.deferred.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactOmaCpmDefer(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact 3gpp-service.ims.icsi.oma.cpm.session.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactOmaCpmSess(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter fill contact +g.oma.SIP-im.large-message.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 * @param [in] pstVal sip.instance value string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactSipInstance(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm, ST_ZOS_SSTR *pstVal);

/**
 * @brief SIP parameter fill contact reg-id.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 * @param [in] iRegId reg-id value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactRegId(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm, ZUINT iRegId);

/**
 * @brief SIP parameter fill contact +g.3gpp.smsip.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContact3gppSmsIp(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm);
                
/**
 * @brief SIP parameter fill contact extension parameter.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter of contact.
 * @param [in] pcName The extension parameters name.
 * @param [in] bQStr The qouted string.
 * @param [in] pcVal The extension parameters value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillContactExtnParms(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM *pstParm, ZCHAR *pcName, 
                ZBOOL bQStr, ZCHAR *pcVal);

/**
 * @brief SIP parameter pick up contact expires.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] piExpires Expires value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactExpires(ST_SIP_CONTACT_PARM *pstParm,
                ZUINT *piExpires);

/**
 * @brief SIP parameter pick up contact isfocus.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, isfocus is exist, or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactIsFocus(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact isZTEfocus.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, isZTEfocus is exist, or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactIsZteFocus(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact +g.poc.talkburst.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, +g.poc.talkburst is exist, or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactPocTb(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact +g.3gpp.cs-voice.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, +g.3gpp.cs-voice is exist, or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContact3gppCv(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact +g.oma.SIP-im.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, +g.oma.SIP-im is exist, or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactOmaSipIm(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact +g.oma.SIP-im.large-message.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, +g.oma.SIP-im.large-message is exist, or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactOmaSipImLMsg(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact 3gpp-service.ims.icsi.oma.cpm.msg.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, 3gpp-service.ims.icsi.oma.cpm.msg is exist, 
 *                      or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactOmaCpm(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact 3gpp-service.ims.icsi.oma.cpm.largemsg.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, 3gpp-service.ims.icsi.oma.cpm.largemsg is exist, 
 *                      or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactOmaCpmLMsg(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact 3gpp-service.ims.icsi.oma.cpm.deferred.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, 3gpp-service.ims.icsi.oma.cpm.deferred is exist, 
 *                      or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactOmaCpmDefer(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact 3gpp-service.ims.icsi.oma.cpm.session.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, 3gpp-service.ims.icsi.oma.cpm.session is exist, 
 *                      or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactOmaCpmSess(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact +sip.instance.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] ppstVal the +sip.instance value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactSipInstance(ST_SIP_CONTACT_PARM *pstParm,
                ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP parameter pick up contact reg-id.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] piRegId the reg-id value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactRegId(ST_SIP_CONTACT_PARM *pstParm,
                ZUINT *piRegId);

/**
 * @brief SIP parameter pick up contact "pub-gruu".
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] ppstVal the "pub-gruu" value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactPubGruu(ST_SIP_CONTACT_PARM *pstParm,
                ST_ZOS_SSTR **ppstVal);

 /**
 * @brief SIP parameter pick up contact +g.3gpp.smsip.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [out] pbExist If ZTURE, +g.3gpp.smsip is exist, or isn't exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContact3gppSmsIp(ST_SIP_CONTACT_PARM *pstParm,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up contact extension parameters.
 *
 * @param [in] pstParm Parameter of contact.
 * @param [in] pcName The extension parameters name.
 * @param [out] pbQStr The qouted string.
 * @param [out] ppstVal The extension parameters value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContactExtnParms(ST_SIP_CONTACT_PARM *pstParm,
                ZCHAR *pcName, ZBOOL *pbQStr, ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP parameter fill ac-value +g.poc.talkburst.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of +g.poc.talkburst.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValPocTb(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value +g.3gpp.cs-voice.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of +g.3gpp.cs-voice.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcVal3gppCv(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value +g.oma.SIP-im.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of +g.oma.SIP-im.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaSipIm(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value +g.oma.SIP-im.large-message.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of +g.oma.SIP-im.large-message.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaSipImLMsg(ZUBUF zBufId,
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value +g.oma.SIP-im.system-message.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of +g.oma.SIP-im.system-message.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaSipImSMsg(ZUBUF zBufId,
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value sms.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of sms.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaSipImSms(ZUBUF zBufId,
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value 3gpp-service.ims.icsi.oma.cpm.msg.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of 3gpp-service.ims.icsi.oma.cpm.msg.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaCpmMsg(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value 3gpp-service.ims.icsi.oma.cpm.largemsg.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of 3gpp-service.ims.icsi.oma.cpm.largemsg.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaCpmLMsg(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value 3gpp-service.ims.icsi.oma.cpm.deferred.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of 3gpp-service.ims.icsi.oma.cpm.deferred.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaCpmDefer(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value 3gpp-service.ims.icsi.oma.cpm.session.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of 3gpp-service.ims.icsi.oma.cpm.session.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValOmaCpmSess(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value require.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of require.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValRequire(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value explicit.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of explicit.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValExplict(ZUBUF zBufId, 
                ST_SIP_AC_VAL *pstAcVal);

/**
 * @brief SIP parameter fill ac-value +sip.instance.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value of sip.instance.
 * @param [in] pstVal string of sip.instance.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValSipInstance(ZUBUF zBufId,
                ST_SIP_AC_VAL *pstAcVal, ST_ZOS_SSTR *pstVal);

/**
 * @brief SIP parameter fill ac-value generic parameter.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstAcVal Ac-value data structure.
 * @param [in] pcName Name string.
 * @param [in] bQStr The qouted string.
 * @param [in] pcVal Value string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillAcValGenParm(ZUBUF zBufId,
                ST_SIP_AC_VAL *pstAcVal, ZCHAR *pcName, 
                ZBOOL bQStr, ZCHAR *pcVal);

/**
 * @brief SIP parameter pick up ac-value +g.poc.talkburst.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValPocTb(ST_SIP_AC_VAL *pstAcVal, 
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value +g.3gpp.cs-voice.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcVal3gppCv(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value +g.oma.SIP-im.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaSipIm(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value +g.oma.SIP-im.large-message.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaSipImLMsg(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value +g.oma.SIP-im.system-message.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaSipImSMsg(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value sms.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaSipImSms(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value cpm page message.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaCpm(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value cpm large message.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaCpmLMsg(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value cpm deferred message.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaCpmDefer(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value cpm session.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValOmaCpmSess(ST_SIP_AC_VAL *pstAcVal,
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value require.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValRequire(ST_SIP_AC_VAL *pstAcVal, 
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value explicit.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValExplict(ST_SIP_AC_VAL *pstAcVal, 
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value +sip.instance.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] ppstVal +sip.instance Value data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValSipInstance(ST_SIP_AC_VAL *pstAcVal,
                ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP parameter pick up ac-value is chatbot.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [out] pbExist If ZTURE, specific tag is exist, or is not exist.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValBot(ST_SIP_AC_VAL *pstAcVal, 
                ZBOOL *pbExist);

/**
 * @brief SIP parameter pick up ac-value generic parameter.
 *
 * @param [in] pstAcVal Ac-value data structure.
 * @param [in] pcName Name string.
 * @param [out] pbQStr The qouted string.
 * @param [out] ppstVal Value data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickAcValGenParm(ST_SIP_AC_VAL *pstAcVal,
                ZCHAR *pcName, ZBOOL *pbQStr, ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP parameter fill SIP-URI transport-param EN_SIP_TPT_PARM.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] ucTpt Transport-param type, @ref EN_SIP_TPT_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriTpt(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri, ZUCHAR ucTpt);

/**
 * @brief SIP parameter fill SIP-URI user-param EN_SIP_USER_PARM.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] ucUser User-param type, @ref EN_SIP_USER_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriUser(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri, ZUCHAR ucUser);

/**
 * @brief SIP parameter fill SIP-URI method EN_SIP_METHOD.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] ucMethod Method type, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriMethod(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri, ZUCHAR ucMethod);

/**
 * @brief SIP parameter fill SIP-URI session param EN_SIP_SESS_PARM.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] ucSess Session param type, @ref EN_SIP_SESS_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriSess(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri, ZUCHAR ucSess);

/**
 * @brief SIP parameter fill SIP-URI ttl.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] iTtl Ttl value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriTtl(ZUBUF zBufId, ST_SIP_SIP_URI *pstSipUri, 
                ZUINT iTtl);

/**
 * @brief SIP parameter fill SIP-URI maddr-param.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] pstAddr IP address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriMaddr(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri, ST_SIP_HOST *pstAddr);

/**
 * @brief SIP parameter fill SIP-URI auid-param.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 * @param [in] pstAuid Auid string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriAuid(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri, ST_ZOS_SSTR *pstAuid);


/* sip parameter fill ServInfo-param */
ZFUNC ZINT Sip_ParmFillServInfoParmByType(ZUBUF zBufId,
                ST_SIP_SERVINFO_PARM *pstParm, ST_ZOS_SSTR *pstVal, 
                ZUCHAR ucType);

/**
 * @brief SIP parameter fill SIP-URI isFocus-param.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriISFS(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP parameter fill SIP-URI lr.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSipUri SIP URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillSipUriLr(ZUBUF zBufId, 
                ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP parameter pick up SIP-URI transport-param EN_SIP_TPT_PARM.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] pucTpt Transport-param type, @ref EN_SIP_TPT_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriTpt(ST_SIP_SIP_URI *pstSipUri, 
                ZUCHAR *pucTpt);

/**
 * @brief SIP parameter pick up SIP-URI user-param EN_SIP_USER_PARM.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] pucUser User-param type, @ref EN_SIP_USER_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriUser(ST_SIP_SIP_URI *pstSipUri, 
                ZUCHAR *pucUser);

/**
 * @brief SIP parameter pick up SIP-URI method EN_SIP_METHOD.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] pucMethod Method type, @ref EN_SIP_METHOD.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriMethod(ST_SIP_SIP_URI *pstSipUri, 
                ZUCHAR *pucMethod);

/**
 * @brief SIP parameter pick up SIP-URI session param EN_SIP_SESS_PARM.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] pucSess Session param type, @ref EN_SIP_SESS_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriSess(ST_SIP_SIP_URI *pstSipUri, 
                ZUCHAR *pucSess);

/**
 * @brief SIP parameter pick up SIP-URI ttl.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] piTtl Ttl value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriTtl(ST_SIP_SIP_URI *pstSipUri, 
                ZUINT *piTtl);

/**
 * @brief SIP parameter pick up SIP-URI maddr-param.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] ppstAddr Maddr-param value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriMaddr(ST_SIP_SIP_URI *pstSipUri, 
                ST_SIP_HOST **ppstAddr);

/**
 * @brief SIP parameter pick up Content-Type param.
 *
 * @param [in] pstName param name.
 * @param [out] ppstVal param value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickContentTypeParm(ST_SIP_HDR_CONTENT_TYPE *pstContentType, 
                ST_ZOS_SSTR *pstName, ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP parameter pick up SIP-URI auid-param.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] ppstAddr Auid-param value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriLr(ST_SIP_SIP_URI *pstSipUri, ZBOOL *pbLr);

/**
 * @brief SIP parameter pick up SIP-URI lr.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] ppstAddr Auid-param value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriAuid(ST_SIP_SIP_URI *pstSipUri, 
                ST_ZOS_SSTR **ppstAuid);

/**
 * @brief SIP parameter pick up SIP-URI IsFoucs-parm.
 *
 * @param [in] pstSipUri SIP URI.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriISFS(ST_SIP_SIP_URI *pstSipUri);

/**
 * @brief SIP parameter pick up SIP-URI gr.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] ppstGr Gr-param value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriGr(ST_SIP_SIP_URI *pstSipUri, 
                ST_ZOS_SSTR **ppstGr);

/**
 * @brief SIP parameter pick up SIP-URI cause.
 *
 * @param [in] pstSipUri SIP URI.
 * @param [out] piCause Cause-param value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPickSipUriCause(ST_SIP_SIP_URI *pstSipUri, 
                ZUINT *piCause);

/**
 * @brief SIP parameter fill reason value.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstVal The reason value.
 * @param [in] iCause The cause value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReasonCause(ZUBUF zBufId, 
                ST_SIP_REASON_VAL *pstVal, ZUINT iCause);

/**
 * @brief SIP parameter fill reason text value.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstVal The reason value.
 * @param [in] pcTxt The cause text value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillReasonTxt(ZUBUF zBufId, 
                ST_SIP_REASON_VAL *pstVal, ZCONST ZCHAR *pcTxt);

/**
 * @brief SIP parameter any type list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pParmLst Parameter list.
 * @param [in] iSize Parameter data size in bytes.
 * @param [out] ppParm Parameter address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAnyLstAdd(ZUBUF zBufId, ZVOID *pParmLst, 
                ZINT iSize, ZVOID **ppParm);

/**
 * @brief SIP parameter any type list remove.
 *
 * @param [in] pParmLst Parameter list.
 * @param [in] pParm Parameter address.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAnyLstRmv(ZVOID *pParmLst, ZVOID *pParm);

/**
 * @brief SIP parameter any type list copy.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pParmLst Parameter list.
 * @param [in] pSrcParm Source parameter copy from.
 * @param [in] pfnCpy Copy function.
 * @param [in] iSize Parameter data size in bytes.
 * @param [out] ppDstParm Destination parameter copy to.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAnyLstCpy(ZUBUF zBufId, ZVOID *pParmLst,
                ZVOID *pSrcParm, PFN_SIPANYCPY pfnCpy, ZINT iSize, 
                ZVOID **ppDstParm);

/**
 * @brief SIP parameter fill media type, mtype see EN_SIP_MTYPE, 
 *        subtype see EN_SIP_MSUBTYPE.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstMediaType Media type data structure.
 * @param [in] ucMtype Type of m-type, @ref EN_SIP_MTYPE.
 * @param [in] pstMtypeExt Extension m-type.
 * @param [in] ucMSubtype Type of sub-type, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstMSubtypeExt Extension sub-type.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillMediaType(ZUBUF zBufId,
                ST_SIP_MEDIA_TYPE *pstMediaType, ZUCHAR ucMtype, 
                ST_ZOS_SSTR *pstMtypeExt, ZUCHAR ucMSubtype, 
                ST_ZOS_SSTR *pstMSubtypeExt);

/**
 * @brief SIP parameter fill media type, mtype see EN_SIP_MTYPE, 
 *  subtype see EN_SIP_MSUBTYPE.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstMediaType Media type data structure.
 * @param [in] ucMtype Type of m-type, @ref EN_SIP_MTYPE.
 * @param [in] pstMtypeExt Extension m-type.
 * @param [in] ucMSubtype Type of sub-type, @ref EN_SIP_MSUBTYPE.
 * @param [in] pstMSubtypeExt Extension sub-type.
 * @param [in] pcBoundary Boundary string.
 * @param [in] wLen Boundary string length.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillMediaTypeX(ZUBUF zBufId,
                ST_SIP_MEDIA_TYPE *pstMediaType, ZUCHAR ucMtype, 
                ST_ZOS_SSTR *pstMtypeExt, ZUCHAR ucMSubtype, 
                ST_ZOS_SSTR *pstMSubtypeExt, ZCHAR *pcBoundary, ZUSHORT wLen);

/**
 * @brief SIP parameter fill generic-parameters.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter data structure.
 * @param [in] pcName Name string.
 * @param [in] pcVal Value string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillGenParm(ZUBUF zBufId, ST_SIP_GEN_PARM *pstParm, 
                ZCHAR *pcName, ZCHAR *pcVal);

/**
 * @brief SIP parameter fill generic-parameters.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParm Parameter data structure.
 * @param [in] pcName Name string.
 * @param [in] pcVal Value string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFillGenParmX(ZUBUF zBufId, ST_SIP_GEN_PARM *pstParm, 
                ZCHAR *pcName, ZBOOL bQStr, ZCHAR *pcVal);

/**
 * @brief SIP parameter option tag list add tag, type see EN_SIP_OPT_TAG.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstOptTagLst Option tag list.
 * @param [in] ucType Option type flag, @ref EN_SIP_OPT_TAG.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmOptTagLstAddTag(ZUBUF zBufId,
                ST_SIP_OPT_TAG_LST *pstOptTagLst, ZUCHAR ucType);

/**
 * @brief SIP parameter option tag list get tag, type see EN_SIP_OPT_TAG.
 *
 * @param [in] pstOptTagLst Option tag list.
 * @param [in] ucType Option type flag, @ref EN_SIP_OPT_TAG.
 * @param [out] ppstTag Option tag.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmOptTagLstGetTag(ST_SIP_OPT_TAG_LST *pstOptTagLst, 
                ZUCHAR ucType, ST_SIP_OPT_TAG **ppstTag);

/**
 * @brief SIP parameter From or To list add tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFromToLstAddTag(ZUBUF zBufId,
                ST_SIP_FROM_TO_PARM_LST *pstParmLst, ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP parameter From or To list add tag.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pcTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFromToLstAddTagX(ZUBUF zBufId,
                ST_SIP_FROM_TO_PARM_LST *pstParmLst, ZCHAR *pcTag);

/**
 * @brief SIP parameter from or to list remove tag.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstTag Tag string of From or To header.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFromToLstRmvTag(ST_SIP_FROM_TO_PARM_LST *pstParmLst,
                ST_ZOS_SSTR *pstTag);

/**
 * @brief SIP parameter from or to list get tag.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstTag Tag string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmFromToLstGetTag(ST_SIP_FROM_TO_PARM_LST *pstParmLst,
                ST_ZOS_SSTR **ppstTag);

/**
 * @brief SIP parameter alert info list get string.
 *
 * @param [in] pstAlertLst Parameter list.
 * @param [in] pcName Generic parameter name.
 * @param [out] ppstVal Generic parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
/* sip parameter from or to list get generic parameter */
ZFUNC ZINT Sip_ParmAlertInfoGetParm(ST_SIP_ALERT_PARM_LST *pstAlertLst,
                ZCHAR *pcName, ST_ZOS_SSTR **ppstVal);

/**
 * @brief SIP parameter ac-params list add, type see EN_SIP_AC_PARMS.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Type of ac-params, @ref EN_SIP_AC_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAcLstAdd(ZUBUF zBufId,
                ST_SIP_AC_PARMS_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_AC_PARMS **ppstParm);

/**
 * @brief SIP parameter ac-params list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAcLstRmv(ST_SIP_AC_PARMS_LST *pstParmLst, 
                ST_SIP_AC_PARMS *pstParm);

/**
 * @brief SIP parameter ac-params list find, type see EN_SIP_AC_PARMS.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Type of ac-params, @ref EN_SIP_AC_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAcLstFind(ST_SIP_AC_PARMS_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_AC_PARMS **ppstParm);

/**
 * @brief SIP parameter method list add, type see EN_SIP_METHOD.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Method type, @ref EN_SIP_METHOD.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmMethodLstAdd(ZUBUF zBufId,
                ST_SIP_METHOD_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_METHOD **ppstParm);

/**
 * @brief SIP parameter method list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmMethodLstRmv(ST_SIP_METHOD_LST *pstParmLst, 
                ST_SIP_METHOD *pstParm);

/**
 * @brief SIP parameter option-tag list add, type see EN_SIP_OPT_TAG.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Option tag type, @ref EN_SIP_OPT_TAG.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmOptTagLstAdd(ZUBUF zBufId,
                ST_SIP_OPT_TAG_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_OPT_TAG **ppstParm);

/**
 * @brief SIP parameter option-tag list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmOptTagLstRmv(ST_SIP_OPT_TAG_LST *pstParmLst, 
                ST_SIP_OPT_TAG *pstParm);

/**
 * @brief SIP parameter header list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmHdrLstAdd(ZUBUF zBufId, ST_SIP_HDR_LST *pstParmLst, 
                ST_SIP_HDR **ppstParm);

/**
 * @brief SIP parameter header list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmHdrLstRmv(ST_SIP_HDR_LST *pstParmLst, 
                ST_SIP_HDR *pstParm);

/**
 * @brief SIP parameter header list find.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pcName Name string.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmHdrLstFind(ST_SIP_HDR_LST *pstParmLst, ZCHAR *pcName,
                ST_SIP_HDR **ppstParm);

/**
 * @brief SIP parameter event list add, type see EN_SIP_EVNT_PARM.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Event parameter type, @ref EN_SIP_EVNT_PARM.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAdd(ZUBUF zBufId, 
                ST_SIP_EVNT_PARM_LST *pstParmLst, ZUCHAR ucType, 
                ST_SIP_EVNT_PARM **ppstParm);

/**
 * @brief SIP parameter event list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstRmv(ST_SIP_EVNT_PARM_LST *pstParmLst, 
                ST_SIP_EVNT_PARM *pstParm);

/**
 * @brief SIP parameter event list find, type see EN_SIP_EVNT_PARM.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Event parameter type, @ref EN_SIP_EVNT_PARM.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstFind(ST_SIP_EVNT_PARM_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_EVNT_PARM **ppstParm);

/**
 * @brief SIP parameter event list add id.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstId Parameter ID string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddId(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ST_ZOS_SSTR *pstId);

/**
 * @brief SIP parameter event list add profile-value, type see EN_SIP_EVNT_PARM.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Profile value type, @ref EN_SIP_PROF_VAL.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddProfVal(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ZUCHAR ucType);

/**
 * @brief SIP parameter event list add vendor.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstVendor Vendor string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddVendor(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ST_ZOS_SSTR *pstVendor);

/**
 * @brief SIP parameter event list add model.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstModel Model string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddModel(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ST_ZOS_SSTR *pstModel);

/**
 * @brief SIP parameter event list add versio[in] /
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstVer Version string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddVer(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ST_ZOS_SSTR *pstVer);

/**
 * @brief SIP parameter event list add effective-by.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] iEffectBy Effective-by value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddEffectBy(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ZUINT iEffectBy);

/**
 * @brief SIP parameter event list add document.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstDoc Document string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddDoc(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ST_ZOS_SSTR *pstDoc);

/**
 * @brief SIP parameter event list add auid.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstAuid Auid string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddAuid(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ST_ZOS_SSTR *pstAuid);

/**
 * @brief SIP parameter event list add diff-processing.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Diff-processing type, @ref EN_SIP_DIFF_PROC.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstAddDiffProc(ZUBUF zBufId,
                ST_SIP_EVNT_PARM_LST *pstParmLst, ZUCHAR ucType);

/**
 * @brief SIP parameter event list get id.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstId ID string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstGetId(ST_SIP_EVNT_PARM_LST *pstParmLst,
                ST_ZOS_SSTR **ppstId);

/**
 * @brief SIP parameter event list get diff-processing.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [out] pucType Diff-processing type.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntLstGetDiffProc(ST_SIP_EVNT_PARM_LST *pstParmLst,
                ZUCHAR *pucType);

/**
 * @brief SIP parameter event-template list add, type see EN_SIP_EVNT_TEMP.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Event template type, @ref EN_SIP_EVNT_TEMP.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntTempLstAdd(ZUBUF zBufId,
                ST_SIP_EVNT_TEMP_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_EVNT_TEMP **ppstParm);

/**
 * @brief SIP parameter event-template list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEvntTempLstRmv(ST_SIP_EVNT_TEMP_LST *pstParmLst, 
                ST_SIP_EVNT_TEMP *pstParm);

/**
 * @brief SIP parameter contact param list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmContactLstAdd(ZUBUF zBufId,
                ST_SIP_CONTACT_PARM_LST *pstParmLst,
                ST_SIP_CONTACT_PARM **ppstParm);

/**
 * @brief SIP parameter contact param list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmContactLstRmv(ST_SIP_CONTACT_PARM_LST *pstParmLst, 
                ST_SIP_CONTACT_PARM *pstParm);

/**
 * @brief SIP parameter contact params list add, type see EN_SIP_CONTACT_PARMS.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Contact parameter type, @ref EN_SIP_CONTACT_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmContactsLstAdd(ZUBUF zBufId,
                ST_SIP_CONTACT_PARMS_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_CONTACT_PARMS **ppstParm);

/**
 * @brief SIP parameter contact params list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmContactsLstRmv(ST_SIP_CONTACT_PARMS_LST *pstParmLst, 
                ST_SIP_CONTACT_PARMS *pstParm);

/**
 * @brief SIP parameter contact params list find, type see EN_SIP_CONTACT_PARMS.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Contact parameter type, @ref EN_SIP_CONTACT_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmContactsLstFind(ST_SIP_CONTACT_PARMS_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_CONTACT_PARMS **ppstParm);

/**
 * @brief SIP parameter contact params list get expire.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [out] piExpire Expires value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmContactsLstGetExpire(ST_SIP_CONTACT_PARMS_LST *pstParmLst,
                ZUINT *piExpire);

/**
 * @brief SIP parameter ServInfo param list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServInfoLstAdd(ZUBUF zBufId, 
                ST_SIP_SERVINFO_PARM_LST *pstParmLst,
                ST_SIP_SERVINFO_PARM **ppstParm);

/**
 * @brief SIP parameter ServInfo param list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServInfoLstRmv(ST_SIP_SERVINFO_PARM_LST *pstParmLst,
                ST_SIP_SERVINFO_PARM *pstParm);

/**
 * @brief SIP parameter ServInfo params list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameters list.
 * @param [in] ucType Parameters type.
 * @param [out] ppstParm Parameters value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServInfoParmsLstAdd(ZUBUF zBufId,
                ST_SIP_SERVINFO_PARMS_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_SERVINFO_PARMS **ppstParm);

/**
 * @brief SIP parameter ServInfo params list remove.
 *
 * @param [in] pstParmLst Parameters list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServInfoParmsLstRmv(ST_SIP_SERVINFO_PARMS_LST *pstParmLst, 
                ST_SIP_SERVINFO_PARMS *pstParm);

/**
 * @brief SIP parameter ServInfo params list find, type see EN_SIP_SERVINFO_PARMS.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType ServInfo parameter type, @ref EN_SIP_SERVINFO_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServInfoParmsLstFind(ST_SIP_SERVINFO_PARMS_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_SERVINFO_PARMS **ppstParm);

/**
 * @brief SIP parameter method list get method, type see EN_SIP_METHOD.
 *
 * @param [in] pstMethodLst Method list.
 * @param [in] ucType Method type, @ref EN_SIP_METHOD.
 * @param [out] ppstMethod SIP method data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmMethodLstGetMethod(ST_SIP_METHOD_LST *pstMethodLst,
                ZUCHAR ucType, ST_SIP_METHOD **ppstMethod);

/**
 * @brief SIP parameter media add attribute.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pcAttr Attribute string.
 * @param [in] wAttrLen Attribute string length.
 * @param [in] pcVal Value string.
 * @param [in] wValLen Value string length.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmMediaLstAddAttr(ZUBUF zBufId,
                ST_SIP_MPARM_LST *pstParmLst, ZCHAR *pcAttr, ZUSHORT wAttrLen,
                ZCHAR *pcVal, ZUSHORT wValLen);

/**
 * @brief SIP parameter digest-response list add, type see EN_SIP_DIG_RSP.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Digest-response type, @ref EN_SIP_DIG_RSP.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDRspLstAdd(ZUBUF zBufId, ST_SIP_DIG_RSP_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_DIG_RSP **ppstParm);

/**
 * @brief SIP parameter digest-response list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDRspLstRmv(ST_SIP_DIG_RSP_LST *pstParmLst, 
                ST_SIP_DIG_RSP *pstParm);

/**
 * @brief SIP parameter digest-response list find, type see EN_SIP_DIG_RSP.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Digest-response type, @ref EN_SIP_DIG_RSP.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDRspLstFind(ST_SIP_DIG_RSP_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_DIG_RSP **ppstParm);

/**
 * @brief SIP parameter digest-cln list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Digest-cln type, @ref EN_SIP_DIGEST_CLN.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDClnLstAdd(ZUBUF zBufId,
                ST_SIP_DIGEST_CLN_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_DIGEST_CLN **ppstParm);

/**
 * @brief SIP parameter digest-cln list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDClnLstRmv(ST_SIP_DIGEST_CLN_LST *pstParmLst, 
                ST_SIP_DIGEST_CLN *pstParm);

/**
 * @brief SIP parameter digest-cln list find, type see EN_SIP_DIGEST_CLN.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Digest-cln type, @ref EN_SIP_DIGEST_CLN.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDClnLstFind(ST_SIP_DIGEST_CLN_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_DIGEST_CLN **ppstParm);

/**
 * @brief SIP parameter uri-parameters list add, type see EN_SIP_URI_PARM.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType URI type, @ref EN_SIP_URI_PARM.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmUriLstAdd(ZUBUF zBufId, ST_SIP_URI_PARM_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_URI_PARM **ppstParm);

/**
 * @brief SIP parameter uri-parameters list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmUriLstRmv(ST_SIP_URI_PARM_LST *pstParmLst,
                ST_SIP_URI_PARM *pstParm);

/**
 * @brief SIP parameter uri-parameters list find, type see EN_SIP_URI_PARM.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType URI type, @ref EN_SIP_URI_PARM.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmUriLstFind(ST_SIP_URI_PARM_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_URI_PARM **ppstParm);

/**
 * @brief SIP parameter route-parameters list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmRouteLstAdd(ZUBUF zBufId,
                ST_SIP_ROUTE_PARM_LST *pstParmLst, 
                ST_SIP_ROUTE_PARM **ppstParm);

/**
 * @brief SIP parameter route-parameters list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmRouteLstRmv(ST_SIP_ROUTE_PARM_LST *pstParmLst, 
                ST_SIP_ROUTE_PARM *pstParm);
/**
 * @brief SIP parameter gen param list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmGenLstAdd(ZUBUF zBufId,
                ST_SIP_GEN_PARM_LST *pstParmLst, ST_SIP_GEN_PARM **ppstParm);

/**
 * @brief SIP parameter gen param list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmGenLstRmv(ST_SIP_GEN_PARM_LST *pstParmLst, 
                ST_SIP_GEN_PARM *pstParm);

/**
 * @brief SIP parameter generic-parameters list find.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pcName Name string.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmGenLstFind(ST_SIP_GEN_PARM_LST *pstParmLst, 
                ZCHAR *pcName, ST_SIP_GEN_PARM **ppstParm);

/**
 * @brief SIP parameter via-parm list add.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmViaLstAdd(ZUBUF zBufId, 
                ST_SIP_VIA_PARM_LST *pstParmLst, ST_SIP_VIA_PARM **ppstParm);

/**
 * @brief SIP parameter via-parm list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmViaLstRmv(ST_SIP_VIA_PARM_LST *pstParmLst,
                ST_SIP_VIA_PARM *pstParm);

/**
 * @brief SIP parameter via-parameters list add, type see EN_SIP_VIA_PARMS.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Via parameter type, @ref EN_SIP_VIA_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmViasLstAdd(ZUBUF zBufId, 
                ST_SIP_VIA_PARMS_LST *pstParmLst, ZUCHAR ucType, 
                ST_SIP_VIA_PARMS **ppstParm);

/**
 * @brief SIP parameter via-parameters list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmViasLstRmv(ST_SIP_VIA_PARMS_LST *pstParmLst,
                ST_SIP_VIA_PARMS *pstParm);

/**
 * @brief SIP parameter via-parameters list find, type see EN_SIP_VIA_PARMS.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Via parameter type, @ref EN_SIP_VIA_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmViasLstFind(ST_SIP_VIA_PARMS_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_VIA_PARMS **ppstParm);

/**
 * @brief SIP parameter digest find, type see EN_SIP_DIGEST_CLN.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [in] ucType Digest parameter type, @ref EN_SIP_DIGEST_CLN.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDigestFind(ST_SIP_DIGEST *pstDigest, ZUCHAR ucType, 
                ST_SIP_DIGEST_CLN **ppstParm);

/**
 * @brief SIP parameter digest get nonce.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [out] ppstNonce Nonce string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDigestGetNonce(ST_SIP_DIGEST *pstDigest, 
                ST_ZOS_SSTR **ppstNonce);

/**
 * @brief SIP parameter digest get stale.
 *
 * @param [in] pstDigest Digest data structure.
 * @param [out] pbStale Stale status, @ref EN_SIP_STALE.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmDigestGetStale(ST_SIP_DIGEST *pstDigest, 
                ZBOOL *pbStale);

/**
 * @brief SIP parameter server-val list add parameter.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServValLstAdd(ZUBUF zBufId,
                ST_SIP_SERVER_VAL_LST *pstParmLst,
                ST_SIP_SERVER_VAL **ppstParm);

/**
 * @brief SIP parameter server-val list remove parameter.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServValLstRmv(ST_SIP_SERVER_VAL_LST *pstParmLst, 
                ST_SIP_SERVER_VAL *pstParm);

/**
 * @brief SIP parameter server-val list add product.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstProductName Product name string.
 * @param [in] pstProductVer Product version string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServValLstAddProduct(ZUBUF zBufId,
                ST_SIP_SERVER_VAL_LST *pstParmLst, ST_ZOS_SSTR *pstProductName, 
                ST_ZOS_SSTR *pstProductVer);

/**
 * @brief SIP parameter server-val list add comment.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstComment Comment string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmServValLstAddComment(ZUBUF zBufId,
                ST_SIP_SERVER_VAL_LST *pstParmLst, ST_ZOS_SSTR *pstComment);

/**
 * @brief SIP parameter subexp-params list add, type see EN_SIP_SUBEXP_PARMS.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Type of subexp-params, @ref EN_SIP_SUBEXP_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmSubexpsLstAdd(ZUBUF zBufId,
                ST_SIP_SUBEXP_PARMS_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_SUBEXP_PARMS **ppstParm);

/**
 * @brief SIP parameter subexp-params list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmSubexpsLstRmv(ST_SIP_SUBEXP_PARMS_LST *pstParmLst, 
                ST_SIP_SUBEXP_PARMS *pstParm);

/**
 * @brief SIP parameter subexp-params list find.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Type of subexp-params, @ref EN_SIP_SUBEXP_PARMS.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmSubexpsLstFind(ST_SIP_SUBEXP_PARMS_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_SUBEXP_PARMS **ppstParm);

/**
 * @brief SIP parameter replaces-params list add, type see EN_SIP_REPLACES_PARM.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Type of replaces-params, @ref EN_SIP_REPLACES_PARM.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmReplacesLstAdd(ZUBUF zBufId,
                ST_SIP_REPLACES_PARM_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_REPLACES_PARM **ppstParm);

/**
 * @brief SIP parameter replaces-params list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmReplacesLstRmv(ST_SIP_REPLACES_PARM_LST *pstParmLst, 
                ST_SIP_REPLACES_PARM *pstParm);

/**
 * @brief SIP parameter replaces-params list remove.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Type of replaces-params, @ref EN_SIP_REPLACES_PARM.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmReplacesLstFind(ST_SIP_REPLACES_PARM_LST *pstParmLst, 
                ZUCHAR ucType, ST_SIP_REPLACES_PARM **ppstParm);

/**
 * @brief SIP parameter priv-value list add value, type see EN_SIP_PRIV_VAL.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstParmLst Parameter list.
 * @param [in] ucType Type of priv-value, @ref EN_SIP_PRIV_VAL.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPrivValLstAdd(ZUBUF zBufId,
                ST_SIP_PRIV_VAL_LST *pstParmLst, ZUCHAR ucType,
                ST_SIP_PRIV_VAL **ppstParm);

/**
 * @brief SIP parameter priv-value list remove value.
 *
 * @param [in] pstParmLst Parameter list.
 * @param [in] pstParm Parameter data structure.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmPrivValLstRmv(ST_SIP_PRIV_VAL_LST *pstParmLst, 
                ST_SIP_PRIV_VAL *pstParm);

/**
 * @brief SIP parameter ainfo find from authen info, type see EN_SIP_AINFO.
 *
 * @param [in] pstAuthInfo Authentication-Info header of SIP message.
 * @param [in] ucType Type of ainfo, @ref EN_SIP_AINFO.
 * @param [out] ppstParm Parameter value.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmAifnoFind(ST_SIP_HDR_AUTHEN_INFO *pstAuthInfo, 
                ZUCHAR ucType, ST_SIP_AINFO **ppstParm);

/**
 * @brief SIP parameter service id list add id.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstSrvIdLst Server ID list.
 * @param [in] pstId ID string.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmSrvIdLstAddId(ZUBUF zBufId,
                ST_SIP_SRV_ID_LST *pstSrvIdLst, ST_ZOS_SSTR *pstId);

/**
 * @brief SIP Early Media parameter add type.
 *
 * @param [in] zBufId Buffer request memory from.
 * @param [in] pstEmParmLst Emergency parameter list.
 * @param [in] iParmType The parameter type, @ref EN_SIP_P_EARLY_MEDIA_PARM.
 *
 * @retval ZOK Operation successfully.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ParmEmParmLstAddType(ZUBUF zBufId,
                ST_SIP_EM_PARM_LST *pstEmParmLst, ZUINT iParmType);

/**
 * @brief SIP message check SIP uri validity.
 *
 * @param [in] pcSipUri SIP URI string.
 * @param [in] wLen SIP URI string length.
 *
 * @retval ZOK It is valid SIP URI.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ValidSipUri(ZCHAR *pcSipUri, ZUSHORT wLen);

/**
 * @brief SIP message check SIP uri user part validity.
 *
 * @param [in] pcUserPart SIP URI user part string.
 * @param [in] wLen SIP URI user part string length.
 *
 * @retval ZOK It is valid SIP URI user part.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ValidSipUriUserPart(ZCHAR *pcUserPart, ZUSHORT wLen);

/**
 * @brief SIP message check SIPs uri validity.
 *
 * @param [in] pcSipUri SIP URI string.
 * @param [in] wLen SIP URI string length.
 *
 * @retval ZOK It is valid SIPs URI.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ValidSipsUri(ZCHAR *pcSipUri, ZUSHORT wLen);

/**
 * @brief SIP message check tel uri validity.
 *
 * @param [in] pcTelUri Tel URI string.
 * @param [in] wLen Tel URI string length.
 *
 * @retval ZOK It is valid Tel URI.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ValidTelUri(ZCHAR *pcTelUri, ZUSHORT wLen);

/**
 * @brief SIP message check tel uri number validity.
 *
 * @param [in] pcNumber Tel URI number string.
 * @param [in] wLen Tel URI number string length.
 *
 * @retval ZOK It is valid Tel URI number.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ValidTelUriNumber(ZCHAR *pcNumber, ZUSHORT wLen);

/**
 * @brief SIP message check token format display name validity.
 *
 * @param [in] pcName The display name string.
 * @param [in] wLen The display name string length.
 *
 * @retval ZOK It is valid token format display name.
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_ValidTknDispName(ZCHAR *pcName, ZUINT iLen);

/**
 * @brief Fill Sip Header Route.
 *
 * @param [in] zBufId The Bufer Id.
 * @param [in] pstRoute Structure of Sip Route.
 * @param [in] ucTptType Transport Type.
 * @param [in] network address.
 *
 * @retval ZOK Fill Sip Header Route Successful .
 * @retval ZFAILED Otherwise.
 */
ZFUNC ZINT Sip_HdrFillRoute(ZUBUF zBufId, 
                ST_SIP_HDR_ROUTE *pstRoute, ZUCHAR ucTptType, 
                ST_ZOS_INET_ADDR *pstAddr);
                
#ifdef __cplusplus
}
#endif

#endif /* _SIP_MSG_UTIL_H__ */

