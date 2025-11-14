/************************************************************************

        Copyright (c) 2005-2012 by Juphoon System Software, Inc.
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
  File name     : stun_msg.h
  Module        : stun
  Author        : anthouny.zhou
  Version       : 0.1
  Created on    : 2012-09-03
  Description: 
    Marcos and structure definitions required by stun message

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _STUN_MSG_H__
#define _STUN_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* data type for stream-tramsport data */
#define STUN_DATA_UNFRAMED_REQ 0x00 /* unframed stun request */
#define STUN_DATA_UNFRAMED_RSP 0x01 /* unframed stun response */
#define STUN_DATA_FRAMED_MSG   0x02 /* framed stun message */
#define STUN_DATA_FRAMED_DATA  0x03 /* framed data */

#define STUN_NTOHLL(_long) ZINET_NTOHL(_long)
#define STUN_HTONLL(_long) ZINET_HTONL(_long)
#define STUN_NTOHL(_long) ZINET_NTOHL(_long)
#define STUN_HTONL(_long) ZINET_HTONL(_long)
#define STUN_NTOHS(_word) ZINET_NTOHS(_word)
#define STUN_HTONS(_word) ZINET_HTONS(_word)

/* stun big endian */
#if defined(ZBENDIAN)

/* stun get bitfield */
#define STUN_GET_BF(_dword, _startbit, _nbits, _dst) \
    _dst = ((_dword) >> (_startbit)) & ((1 << (_nbits)) - 1)

#else /* stun little endian */

/* stun get bitfield */
#define STUN_GET_BF(_dword, _startbit, _nbits, _dst) do { \
    ZINT iNewStartBit = 32 - (_startbit + _nbits); \
    _dst = ((_dword) >> (iNewStartBit)) & ((1 << (_nbits)) - 1); \
} while (0)

#endif

/* stun get unsigned long data with host order flag */
#define STUN_GET_UL_DATA(_bufstr, _data, _ret) do { \
    _ret = ((_bufstr)->iLen < sizeof(ZUINT)) ? ZFAILED : ZOK; \
    if ((_ret) == ZOK) \
    { \
        _data = (_bufstr)->pucStr[0]; \
        _data = (_data << 8) + (_bufstr)->pucStr[1]; \
        _data = (_data << 8) + (_bufstr)->pucStr[2]; \
        _data = (_data << 8) + (_bufstr)->pucStr[3]; \
        (_bufstr)->pucStr += 4; \
        (_bufstr)->iLen -= 4; \
    } \
} while (0)

/* stun get long long data with host order flag */
#define STUN_GET_LL_DATA(_bufstr, _data, _ret) do { \
    _ret = ((_bufstr)->iLen < sizeof(ZULLONG)) ? ZFAILED : ZOK; \
    if ((_ret) == ZOK) \
    { \
        _data = (_bufstr)->pucStr[0]; \
        _data = (_data << 8) + (_bufstr)->pucStr[1]; \
        _data = (_data << 8) + (_bufstr)->pucStr[2]; \
        _data = (_data << 8) + (_bufstr)->pucStr[3]; \
        _data = (_data << 8) + (_bufstr)->pucStr[4]; \
        _data = (_data << 8) + (_bufstr)->pucStr[5]; \
        _data = (_data << 8) + (_bufstr)->pucStr[6]; \
        _data = (_data << 8) + (_bufstr)->pucStr[7]; \
        (_bufstr)->pucStr += 8; \
        (_bufstr)->iLen -= 8; \
    } \
} while (0)

/* stun check if message is stun message */
ZFUNC ZBOOL Stun_IsValidData(ZUCHAR *pucData, ZUINT iLen);

/* stun check if message is channel data message */
ZFUNC ZBOOL Stun_IsChnlData(ZUCHAR *pucData, ZUINT iLen);

/* stun get channel number direct from data */
ZFUNC ZUSHORT Stun_FetchChnlNum(ZUCHAR *pucData, ZUINT iLen);

/* stun get message type */
ZFUNC ZUSHORT Stun_GetMsgType(ZUCHAR *pucData, ZUINT iLen);

/* stun get reason phrase from status code */
ZFUNC ZINT Stun_ReasonFromCode(ZUINT iCode, ST_ZOS_SSTR *pstReason);

/* decode the frame in stream-transport data */
ZFUNC ZINT Stun_DecodeFrame(ZDBUF zMsgBuf, ZUINT *piDataType);

/* encode the frame in stream-transport data */
ZFUNC ZINT Stun_EncodeFrame(ZDBUF zMsgBuf, ZUINT iDataType);

/* stun create message */
ZFUNC ZINT Stun_CreateMsg(ST_STUN_MSG **ppstMsg);

/* stun create message with type */
ZFUNC ZINT Stun_CreateMsgX(ZUSHORT wMsgType, ST_STUN_MSG **ppstMsg);

/* stun delete message */
ZFUNC ZINT Stun_DeleteMsg(ST_STUN_MSG *pstMsg);

/* stun decode stun message */
ZFUNC ZINT Stun_DecodeMsg(ZDBUF zMsgBuf, ST_STUN_MSG **ppstMsg);

/* stun encode stun request message */
ZFUNC ZINT Stun_EncodeMsg(ST_STUN_MSG *pstMsg, ZDBUF *pzMsgBuf);

/* stun set the message */
ZFUNC ZINT Stun_SetMsgType(ST_STUN_MSG *pstMsg, ZUSHORT wMsgType,
                ST_STUN_TRANS_ID *pstTransId);

/* stun get the attribute */
ZFUNC ZINT Stun_GetAttr(ST_STUN_MSG *pstMsg, ZUSHORT wType, 
                ST_STUN_ATTR **ppstAttr);

/* stun get attribute count */
ZFUNC ZUINT Stun_GetAttrCnt(ST_STUN_MSG *pstMsg, ZUSHORT wType);

/* stun get attribute by index */
ZFUNC ZINT Stun_GetAttrX(ST_STUN_MSG *pstMsg, ZUSHORT wType, ZUINT iIndex,
                ST_STUN_ATTR **ppstAttr);

/* stun set the attribute */
ZFUNC ZINT Stun_SetAttr(ST_STUN_MSG *pstMsg, ZUSHORT wType, 
                ST_STUN_ATTR **ppstAttr);

/* stun add the attribute */
ZFUNC ZINT Stun_AddAttr(ST_STUN_MSG *pstMsg, ZUSHORT wType, 
                ST_STUN_ATTR **ppstAttr);

/* stun remove the lifetime */
ZFUNC ZINT Stun_RmvAttr(ST_STUN_MSG *pstMsg, ST_STUN_ATTR *pstAttr);

/* stun get the error code attribute */
ZFUNC ZINT Stun_GetErrCode(ST_STUN_MSG *pstMsg, 
                ST_STUN_ERR_CODE **ppstErr);

/* stun set the error code attribute */
ZFUNC ZINT Stun_SetErrCode(ST_STUN_MSG *pstMsg, ZUINT iCode);

/* stun get the mapped address */
ZFUNC ZINT Stun_GetMapAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the mapped address */
ZFUNC ZINT Stun_SetMapAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the response address */
ZFUNC ZINT Stun_GetRspAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the response address */
ZFUNC ZINT Stun_SetRspAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the changed address */
ZFUNC ZINT Stun_GetChgedAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the changed address */
ZFUNC ZINT Stun_SetChgedAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the xor mapped address */
ZFUNC ZINT Stun_GetXorMapAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the xor mapped address */
ZFUNC ZINT Stun_SetXorMapAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the lifetime */
ZFUNC ZINT Stun_GetLifetime(ST_STUN_MSG *pstMsg, ZUINT *piLifetime);

/* stun set the lifetime */
ZFUNC ZINT Stun_SetLifetime(ST_STUN_MSG *pstMsg, ZUINT iLifetime);

/* stun get the bandwidth */
ZFUNC ZINT Stun_GetBandwidth(ST_STUN_MSG *pstMsg, ZUINT *piBandwidth);

/* stun set the bandwidth */
ZFUNC ZINT Stun_SetBandwidth(ST_STUN_MSG *pstMsg, ZUINT iBandwidth);

/* stun get the timer value */
ZFUNC ZINT Stun_GetTmrVal(ST_STUN_MSG *pstMsg, ZUINT *piTmrVal);

/* stun set the timer value */
ZFUNC ZINT Stun_SetTmrVal(ST_STUN_MSG *pstMsg, ZUINT iTmrVal);

/* stun get the channel number */
ZFUNC ZINT Stun_GetChnlNum(ST_STUN_MSG *pstMsg, ZUSHORT *pwChnlNum);

/* stun set the channel number */
ZFUNC ZINT Stun_SetChnlNum(ST_STUN_MSG *pstMsg, ZUSHORT wChnlNum);

/* stun get the response port */
ZFUNC ZINT Stun_GetRspPort(ST_STUN_MSG *pstMsg, ZUSHORT *pdwRspPort);

/* stun set the response port */
ZFUNC ZINT Stun_SetRspPort(ST_STUN_MSG *pstMsg, ZUSHORT dwRspPort);

/* stun get cache timeout */
ZFUNC ZINT Stun_GetCacheTmout(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstCacheTmout);

/* stun set cache timeout */
ZFUNC ZINT Stun_SetCacheTmout(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR *pstCacheTmout);

/* stun get the response origin */
ZFUNC ZINT Stun_GetRspOrigin(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the response origin */
ZFUNC ZINT Stun_SetRspOrigin(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the other address count */
ZFUNC ZUINT Stun_GetOtherAddrCnt(ST_STUN_MSG *pstMsg);

/* stun get the other address */
ZFUNC ZINT Stun_GetOtherAddr(ST_STUN_MSG *pstMsg, ZUINT iIndex,
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the other address */
ZFUNC ZINT Stun_SetOtherAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the xor peer address */
ZFUNC ZINT Stun_GetXorPeerAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the xor peer address */
ZFUNC ZINT Stun_SetXorPeerAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the username */
ZFUNC ZINT Stun_GetUserName(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstUserName);

/* stun set the username */
ZFUNC ZINT Stun_SetUserName(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR *pstUserName);

/* stun get the padding */
ZFUNC ZINT Stun_GetPadding(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstPadding);

/* stun set the padding */
ZFUNC ZINT Stun_SetPadding(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR *pstPadding);

/* stun get the password */
ZFUNC ZINT Stun_GetPassWord(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstPassword);

/* stun set the password */
ZFUNC ZINT Stun_SetPassWord(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR *pstPassword);

/* stun get message integrity */
ZFUNC ZINT Stun_GetMsgInteg(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstmsginteg);

/* stun set message integrity */
ZFUNC ZINT Stun_SetMsgInteg(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR *pstmsginteg);

/* stun get finger print */
ZFUNC ZINT Stun_GetFingerPrint(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstfingerprint);

/* stun set finger print */
ZFUNC ZINT Stun_SetFingerPrint(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR *pstfingerprint);

/* stun get realm */
ZFUNC ZINT Stun_GetRealm(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR **ppstrealm);

/* stun set realm */
ZFUNC ZINT Stun_SetRealm(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR *pstrealm);

/* stun get nonce */
ZFUNC ZINT Stun_GetNonce(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR **ppstnonce);

/* stun set nonce */
ZFUNC ZINT Stun_SetNonce(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR *pstnonce);

/* stun get unknown attribute */
ZFUNC ZINT Stun_GetUnknown(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstunknown);

/* stun set unknown attribute */
ZFUNC ZINT Stun_SetUnknown(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR *pstunknown);

/* stun get software */
ZFUNC ZINT Stun_GetSoftware(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR **ppstsoftware);

/* stun set software */
ZFUNC ZINT Stun_SetSoftware(ST_STUN_MSG *pstMsg, 
                ST_ZOS_SSTR *pstsoftware);

/* stun get the alternate server */
ZFUNC ZINT Stun_GetAlertServ(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the alternate server */
ZFUNC ZINT Stun_SetAlertServ(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the data */
ZFUNC ZINT Stun_GetData(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR **ppstData);

/* stun set the data */
ZFUNC ZINT Stun_SetData(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR *pstData);

/* stun get the xor relay address */
ZINT Stun_GetXorRelayAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the xor relay address */
ZINT Stun_SetXorRelayAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the requested port property */
ZFUNC ZINT Stun_GetEvenPort(ST_STUN_MSG *pstMsg, ZBOOL *pbEvntPort);

/* stun set the requested port property */
ZFUNC ZINT Stun_SetEvenPort(ST_STUN_MSG *pstMsg, ZBOOL bEvntPort);

/* stun get the requested transport */
ZFUNC ZINT Stun_GetReqTpt(ST_STUN_MSG *pstMsg, ZUINT *piReqTpt);

/* stun set the requested transport */
ZFUNC ZINT Stun_SetReqTpt(ST_STUN_MSG *pstMsg, ZUINT iReqTpt);

/* stun get the reservation token */
ZFUNC ZINT Stun_GetRsrvToken(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR **ppstRsrvToken);

/* stun set the reservation token */
ZFUNC ZINT Stun_SetRsrvToken(ST_STUN_MSG *pstMsg, ST_ZOS_SSTR *pstRsrvToken);

/* stun get the connection status */
ZFUNC ZINT Stun_GetConnStat(ST_STUN_MSG *pstMsg, ZUINT *piConnStat);

/* stun set the connection status */
ZFUNC ZINT Stun_SetConnStat(ST_STUN_MSG *pstMsg, ZUINT iConnStat);

/* stun get the reflected from */
ZFUNC ZINT Stun_GetRefleFrom(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the reflected from */
ZFUNC ZINT Stun_SetRefleFrom(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the source address */
ZFUNC ZINT Stun_GetSrcAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR **ppstAddr);

/* stun set the source address */
ZFUNC ZINT Stun_SetSrcAddr(ST_STUN_MSG *pstMsg, 
                ST_ZOS_INET_ADDR *pstAddr);

/* stun get the change request */
ZFUNC ZINT Stun_GetChangReq(ST_STUN_MSG *pstMsg, 
                ST_STUN_CHANG_REQ **ppstChgReq);

/* stun set the change request */
ZFUNC ZINT Stun_SetChangReq(ST_STUN_MSG *pstMsg, 
                ST_STUN_CHANG_REQ *pstChgReq);

/* stun get the priority */
ZFUNC ZINT Stun_GetPriority(ST_STUN_MSG *pstMsg, ZUINT *piPriority);

/* stun set the priority */
ZFUNC ZINT Stun_SetPriority(ST_STUN_MSG *pstMsg, ZUINT iPriority);

/* stun get the use candidate */
ZFUNC ZINT Stun_GetUseCand(ST_STUN_MSG *pstMsg);

/* stun set the use candidate */
ZFUNC ZINT Stun_SetUseCand(ST_STUN_MSG *pstMsg);

/* stun get the dont fragment */
ZFUNC ZINT Stun_GetDontFrag(ST_STUN_MSG *pstMsg);

/* stun set the dont fragment */
ZFUNC ZINT Stun_SetDontFrag(ST_STUN_MSG *pstMsg);

/* stun get the ice controlled */
ZFUNC ZINT Stun_GetIceCtrled(ST_STUN_MSG *pstMsg, 
                ZULLONG *qwIceCtrled);

/* stun set the ice controlled */
ZFUNC ZINT Stun_SetIceCtrled(ST_STUN_MSG *pstMsg, 
                ZULLONG qwIceCtrled);

/* stun get the ice controlling */
ZFUNC ZINT Stun_GetIceCtrling(ST_STUN_MSG *pstMsg, 
                ZULLONG *qwIceCtrling);

/* stun set the ice controlling */
ZFUNC ZINT Stun_SetIceCtrling(ST_STUN_MSG *pstMsg, 
                ZULLONG qwIceCtrling);

#ifdef __cplusplus
}
#endif

#endif /* _STUN_MSG_H__ */

