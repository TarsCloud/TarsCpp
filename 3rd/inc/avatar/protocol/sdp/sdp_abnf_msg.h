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
  File name     : sdp_abnf_msg.h
  Module        : sdp abnf encode
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2005-04-14    
  Description   :
      Data function definitions required by the sdp abnf message
   
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SDP_ABNF_MSG_H__
#define _SDP_ABNF_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* sdp message sdp create session-description */
ZFUNC ZINT Sdp_MsgCreate(ZUBUF zMemBuf, ST_SDP_SESS_DESC **ppstMsg);

/* sdp message copy */
ZFUNC ZINT Sdp_MsgCpy(ZUBUF zMemBuf, ST_SDP_SESS_DESC *pstDst,
                ST_SDP_SESS_DESC *pstSrc);

/* sdp message sdp session time time field create */
ZFUNC ZINT Sdp_MsgCreateTf(ZUBUF zMemBuf, ST_SDP_SESS_DESC *pstMsg, 
                ST_SDP_TF **ppstTf);

/* sdp message sdp media-descriptions create */
ZFUNC ZINT Sdp_MsgCreateMdesc(ZUBUF zMemBuf, 
                ST_SDP_SESS_DESC *pstMsg, ST_SDP_MDESC **ppstMdesc);

/* sdp message sdp media-descriptions attribute field create */
ZFUNC ZINT Sdp_MsgCreateAf(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ST_SDP_AF **ppstAf);

/* sdp message sdp session-descriptions attribute field create */
ZFUNC ZINT Sdp_MsgCreateAfX(ZUBUF zMemBuf, ST_SDP_SESS_DESC *pstMsg, 
                ST_SDP_AF **ppstAf);

/* sdp message is exist attribute field */
ZFUNC ZBOOL Sdp_MsgExistAf(ST_SDP_SESS_DESC *pstMsg, ZUCHAR ucAttrType,
                ST_SDP_AF **ppstAf);

/* sdp message sdp media-descriptions bandwidth field create */
ZFUNC ZINT Sdp_MsgCreateBf(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ST_SDP_BF **ppstBf);

/* sdp message sdp media-descriptions connection field create */
ZFUNC ZINT Sdp_MsgCreateCf(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ST_SDP_CF **ppstCf);

/* sdp message set origin-field */
ZFUNC ZINT Sdp_MsgSetOf(ZUBUF zMemBuf, ST_SDP_OF *pstOrig, 
                ZCHAR *pcUserName, ZUSHORT wUserNameLen, ZUINT iSessId, 
                ZUINT iSessVer, ST_ZOS_INET_IP *pstIp);

/* sdp message set origin-field */
ZFUNC ZINT Sdp_MsgSetOfX(ZUBUF zMemBuf, ST_SDP_OF *pstOrig, 
                ZCHAR *pcUserName, ZUINT iSessId, ZUINT iSessVer, 
                ST_ZOS_INET_IP *pstIp);

/* sdp message set session-name--field */
ZFUNC ZINT Sdp_MsgSetSf(ZUBUF zMemBuf, ST_SDP_SF *pstSessName,
                ZCHAR *pcSessName, ZUSHORT wSessNameLen);

/* sdp message set session-name-field */
ZFUNC ZINT Sdp_MsgSetSfX(ZUBUF zMemBuf, ST_SDP_SF *pstSessName,
                ZCHAR *pcSessName);

/* sdp message get connection-field */
ZFUNC ZINT Sdp_MsgGetCf(ST_SDP_CF *pstConn, ST_ZOS_INET_IP *pstIp);

/* sdp message set connection-field */
ZFUNC ZINT Sdp_MsgSetCf(ST_SDP_CF *pstConn, ST_ZOS_INET_IP *pstIp);

/* sdp message set time-field (no repeat fields) */
ZFUNC ZINT Sdp_MsgSetTf(ZUBUF zMemBuf, ST_SDP_SESS_DESC *pstMsg, 
                ZUINT iStartTime, ZUINT iStopTime);

/* sdp message get bandwidth value */
ZFUNC ZINT Sdp_MsgGetBf(ST_SDP_MDESC *pstMdesc, ZUCHAR ucType,
                ZUINT *piValue);

/* sdp message set bandwidth field */
ZFUNC ZINT Sdp_MsgSetBf(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucType, ZUINT iValue);

/* sdp message get media-field (no fmt)*/
ZFUNC ZINT Sdp_MsgGetMf(ST_SDP_MF *pstMedia, ZUCHAR *pucMediaType, 
                ST_ZOS_SSTR **ppstUnknownMedia, ZUCHAR *pucProtoType, 
                ST_ZOS_SSTR **ppstUnknownProto, ZUSHORT *pwPort, 
                ZUSHORT *pwPortNum);

/* sdp message set media-field (no fmt)*/
ZFUNC ZINT Sdp_MsgSetMf(ZUBUF zMemBuf, ST_SDP_MF *pstMedia, 
                ZUCHAR ucMediaType, ZCHAR *pcUnknownMedia, ZUSHORT wMediaLen,
                ZUCHAR ucProtoType, ZCHAR *pcUnknownProto, ZUSHORT wProtoLen,
                ZUSHORT wPort, ZUSHORT wPortNum);

/* sdp message set media-field (no fmt)*/
ZFUNC ZINT Sdp_MsgSetMfX(ZUBUF zMemBuf, ST_SDP_MF *pstMedia, 
                ZUCHAR ucMediaType, ZCHAR *pcUnknownMedia, ZUCHAR ucProtoType, 
                ZCHAR *pcUnknownProto, ZUSHORT wPort, ZUSHORT wPortNum);

/* sdp message set media-field codec*/
ZFUNC ZINT Sdp_MsgSetMfCodec(ZUBUF zMemBuf, ST_SDP_MF *pstMedia,
                ZUCHAR ucPayload);

/* sdp message get information-field */
ZFUNC ZINT Sdp_MsgGetIf(ST_SDP_IF *pstInfo, ST_ZOS_SSTR **ppstText);

/* sdp message set information-field */
ZFUNC ZINT Sdp_MsgSetIf(ZUBUF zMemBuf, ST_SDP_IF *pstInfo,
                ZCHAR *pcText, ZUSHORT wTextLen);

/* sdp message set attribute-field of amr fmtp */
ZFUNC ZINT Sdp_MsgSetAfFmtpAmr(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ZUCHAR ucPayload, ZUCHAR ucModeSet);

/* sdp message set attribute-field of tbcp fmtp, ucType: EN_SDP_TBCP_PARM_TYPE */
ZFUNC ZINT Sdp_MsgSetAfFmtpTbcpParm(ZUBUF zMemBuf,
                ST_SDP_MDESC *pstMdesc, ZUCHAR ucType, ZUCHAR ucVal);

/* sdp message set attribute-field (media mode, recvonly...) */
ZFUNC ZINT Sdp_MsgSetAfRtpmap(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucPayload, ZUINT iClockRate, ZUCHAR ucEncodingType);

/* sdp message get attribute-field of rtcp */
ZFUNC ZINT Sdp_MsgGetAfRtcp(ST_SDP_MDESC *pstMdesc, ZUSHORT *pwPort);

/* sdp message set attribute-field of rtcp */
ZFUNC ZINT Sdp_MsgSetAfRtcp(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUSHORT wPort);

/* sdp message get attribute-field of rtcp-mux */
ZFUNC ZINT Sdp_MsgGetAfRtcpMux(ST_SDP_MDESC *pstMdesc, ZBOOL *pbRtcpMux);

/* sdp message set attribute-field of rtcp-mux */
ZFUNC ZINT Sdp_MsgSetAfRtcpMux(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZBOOL bRtcpMux);

/* sdp message set attribute-field of rtcp-fb */
ZFUNC ZINT Sdp_MsgGetAfRtcpFb(ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucIdType, ZUCHAR *pucPayload, ZUCHAR *pucParm);

/* sdp message set attribute-field of rtcp-fb */
ZFUNC ZINT Sdp_MsgSetAfRtcpFb(ZDBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucPayload, ZUCHAR ucIdType, ZUCHAR ucParm);

/* sdp message get attribute-field direct, ucDirect: EN_SDP_DIRECT_TYPE_TYPE */
ZFUNC ZINT Sdp_MsgGetAfDirect(ST_SDP_MDESC *pstMdesc, ZUCHAR *pucDirect);

/* sdp message set attribute-field direct, ucDirect: EN_SDP_DIRECT_TYPE_TYPE */
ZFUNC ZINT Sdp_MsgSetAfDirect(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucDirect);

/* sdp message get attribute-field of ptime */
ZFUNC ZINT Sdp_MsgGetAfPtime(ST_SDP_MDESC *pstMdesc, ZUINT *piTime);

/* sdp message set attribute-field of ptime */
ZFUNC ZINT Sdp_MsgSetAfPTime(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUINT iPTime);

/* sdp message get media-level attribute-field of setup */
ZFUNC ZINT Sdp_MsgGetAfSetup(ST_SDP_MDESC *pstMdesc, ZUCHAR *pucType);

/* sdp message get sessin-level attribute-field of setup */
ZFUNC ZINT Sdp_MsgGetAfSetupX(ST_SDP_SESS_DESC *pstMsg, ZUCHAR *pucType);

/* sdp message set attribute-field of setup */
ZFUNC ZINT Sdp_MsgSetAfSetup(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucType);

/* sdp message get attribute-field of framesize */
ZFUNC ZINT Sdp_MsgGetAfFramesize(ST_SDP_MDESC *pstMdesc, ZUCHAR *pucPayload,
                ZUINT *piWidth, ZUINT *piHeight);

/* sdp message set attribute-field of framesize */
ZFUNC ZINT Sdp_MsgSetAfFramesize(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucPayload, ZUINT iWidth, ZUINT iHeight);

/* sdp message get attribute-field of connection */
ZFUNC ZINT Sdp_MsgGetAfConn(ST_SDP_MDESC *pstMdesc, ZUCHAR *pucType);

/* sdp message set attribute-field of connection */
ZFUNC ZINT Sdp_MsgSetAfConn(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucType);

/* sdp message set attribute-field of max ptime */
ZFUNC ZINT Sdp_MsgGetAfMaxPTime(ST_SDP_MDESC *pstMdesc, 
                ZUINT *piMaxPTime);

/* sdp message set attribute-field of max ptime */
ZFUNC ZINT Sdp_MsgSetAfMaxPTime(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ZUINT iMaxPTime);

/* sdp message get attribute-filed of rtx fmtp */
ZFUNC ZINT Sdp_MsgGetAfFmtpRtx(ST_SDP_MDESC *pstMdesc,
                ZUCHAR ucRtxPt, ZUCHAR *pucApt, ZUSHORT *pwRtxTime);

/* sdp message set attribute-field of rtx fmtp */
ZFUNC ZINT Sdp_MsgSetAfFmtpRtx(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc,
                ZUCHAR ucRtxPt, ZUCHAR ucApt, ZUSHORT wRtxTime);

/* sdp message get attribute-field of unknown */
ZFUNC ZINT Sdp_MsgGetAfUnknown(ST_SDP_MDESC *pstMdesc, 
                ZCHAR *pcName, ST_ZOS_SSTR **ppstVal);

/* sdp message set attribute-field of unknown */
ZFUNC ZINT Sdp_MsgSetAfUnknown(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ZCHAR *pcName, ZCHAR *pcVal);

/* sdp message get attribute-field of field string(include "a=" and value) */
ZFUNC ZINT Sdp_MsgGetAfField(ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucType, ST_ZOS_SSTR **ppstField);

/* sdp message set attribute-field of field string(include "a=" and value) */
ZFUNC ZINT Sdp_MsgSetAfField(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZCHAR *pcField);

/* sdp message get attribute-field of accept-types */
ZFUNC ZINT Sdp_MsgGetAfAcptTypes(ST_SDP_MDESC *pstMdesc, 
                ST_SDP_ACPT_TYPES **ppstAcptTypes);

/* sdp message set attribute-field of accept-types */
ZFUNC ZINT Sdp_MsgSetAfAcptTypes(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ST_SDP_ACPT_TYPES **ppstAcptTypes);

/* sdp message get attribute-field of accept-wrapped-types */
ZFUNC ZINT Sdp_MsgGetAfAcptWpdTypes(ST_SDP_MDESC *pstMdesc, 
                ST_SDP_ACPT_TYPES **ppstAcptWpdTypes);

/* sdp message set attribute-field of accept-wrapped-types */
ZFUNC ZINT Sdp_MsgSetAfAcptWpdTypes(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ST_SDP_ACPT_TYPES **ppstAcptWpdTypes);

/* sdp message set accept-types attribute-field add all type */
ZFUNC ZINT Sdp_MsgAfAcptTypesAddAll(ZUBUF zMemBuf, 
                ST_SDP_ACPT_TYPES *pstAcptTypes);

/* sdp message set accept-types attribute-field add all type */
ZFUNC ZINT Sdp_MsgAfAcptTypesAddType(ZUBUF zMemBuf, 
                ST_SDP_ACPT_TYPES *pstAcptTypes, ZUCHAR ucType, 
                ZUCHAR ucSubType);

/* sdp message set accept-types attribute-field add all type */
ZFUNC ZINT Sdp_MsgAfAcptTypesAddTypeStr(ZUBUF zMemBuf, 
                ST_SDP_ACPT_TYPES *pstAcptTypes, ZCHAR *pcTypeStr);

/* sdp message set accept-types attribute-field add all type */
ZFUNC ZINT Sdp_MsgAfAcptTypesAddExtn(ZUBUF zMemBuf, 
                ST_SDP_ACPT_TYPES *pstAcptTypes, ZCHAR *pcType, 
                ZCHAR *pcSubType);

/* sdp message get attribute-field of file-selector */
ZFUNC ZINT Sdp_MsgGetAfFileSlt(ST_SDP_MDESC *pstMdesc, 
                ST_SDP_FILE_SLT **ppstSlt);

/* sdp message set attribute-field of file-selector */
ZFUNC ZINT Sdp_MsgSetAfFileSlt(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ST_SDP_FILE_SLT **ppstSlt);

/* sdp message get file size of file-selector attribute-field */
ZFUNC ZINT Sdp_MsgAfFileSltGetFileSize(ST_SDP_MDESC *pstMdesc, 
                ZUINT *piSize);

/* sdp message get file size of file-selector attribute-field */
ZFUNC ZINT Sdp_MsgAfFileSltGetFileSizeX(ST_SDP_FILE_SLT *pstSlt, 
                ZUINT *piSize);

/* sdp message set file-selector attribute-field add file size */
ZFUNC ZINT Sdp_MsgAfFileSltAddFileSize(ZUBUF zMemBuf, 
                ST_SDP_FILE_SLT *pstSlt, ZUINT iSize);

/* sdp message get file name of file-selector attribute-field,
     It should be freed by Zos_SysStrFree. */
ZFUNC ZINT Sdp_MsgAfFileSltGetFileName(ST_SDP_MDESC *pstMdesc, 
                ZCHAR **ppcName);

ZFUNC ZINT Sdp_MsgAfFileSltGetFileNameX(ST_SDP_FILE_SLT *pstSlt, 
                ZCHAR **ppcName);

/* sdp message set file-selector attribute-field add file name */
ZFUNC ZINT Sdp_MsgAfFileSltAddFileName(ZUBUF zMemBuf, 
                ST_SDP_FILE_SLT *pstSlt, ZCHAR *pcName);

/* sdp message get file type of file-selector attribute-field,
    pucType see EN_SDP_TYPE, pucSubType see EN_SDP_SUBTYPE */
ZFUNC ZINT Sdp_MsgAfFileSltGetFileType(ST_SDP_MDESC *pstMdesc, 
                ZUCHAR *pucType, ST_ZOS_SSTR **ppstTypeExt,
                ZUCHAR *pucSubType, ST_ZOS_SSTR **ppstSubTypeExt);

/* sdp message get file type string of file-selector attribute-field */
ZFUNC ZINT Sdp_MsgAfFileSltGetFileTypeStr(ST_SDP_MDESC *pstMdesc, 
                ST_ZOS_SSTR **ppstTypeStr);

/* sdp message get file type of file-selector attribute-field */
ZFUNC ZINT Sdp_MsgAfFileSltGetFileTypeStrX(ST_SDP_FILE_SLT *pstSlt, 
                ST_ZOS_SSTR **ppstTypeStr);
                
/* sdp message set file-selector attribute-field add file type */
ZFUNC ZINT Sdp_MsgAfFileSltAddFileType(ZUBUF zMemBuf, 
                ST_SDP_FILE_SLT *pstSlt, ZCHAR *pcType);

/* sdp message get file hash of file-selector attribute-field */
ZFUNC ZINT Sdp_MsgAfFileSltGetFileHash(ST_SDP_MDESC *pstMdesc, 
                ZUCHAR *pucAlgType, ST_ZOS_SSTR **ppstHashVal);

/* sdp message set file-selector attribute-field add file hash,
    hash algo type see EN_SDP_HASH_TYPE. */
ZFUNC ZINT Sdp_MsgAfFileSltAddFileHash(ZUBUF zMemBuf, 
                ST_SDP_FILE_SLT *pstSlt, ZCHAR *pcAlgo, ZCHAR *pcVal);

/* sdp message get attribute-field of file-disposition */
ZFUNC ZINT Sdp_MsgGetAfFileDisp(ST_SDP_MDESC *pstMdesc, 
                ST_ZOS_SSTR **ppstDisp);

/* sdp message set attribute-field of file-disposition */
ZFUNC ZINT Sdp_MsgSetAfFileDisp(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ZCHAR *pcDisp);

/* sdp message get attribute-field of file-icon */
ZFUNC ZINT Sdp_MsgGetAfFileIcon(ST_SDP_MDESC *pstMdesc, 
                ZUCHAR *pucUrlType, ST_ZOS_SSTR **ppstWord, 
                ST_ZOS_SSTR **ppstDomain);

/* sdp message set attribute-field of file-icon */
ZFUNC ZINT Sdp_MsgSetAfFileIcon(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ZUCHAR ucUrlType, 
                ZCHAR *pcWord, ZCHAR *pcDomain);

/* sdp message get attribute-field of file-transfer-id */
ZFUNC ZINT Sdp_MsgGetAfFileTrsfId(ST_SDP_MDESC *pstMdesc, 
                ST_ZOS_SSTR **ppstTrsfId);

/* sdp message set attribute-field of file-transfer-id */
ZFUNC ZINT Sdp_MsgSetAfFileTrsfId(ZUBUF zMemBuf, 
                ST_SDP_MDESC *pstMdesc, ZCHAR *pcTrsfId);

/* sdp message get attribute-field of file-range */
ZFUNC ZINT Sdp_MsgGetAfFileRange(ST_SDP_MDESC *pstMdesc, 
                ZUINT *piStartOffset, ZUINT *piStopOffset);

/* sdp message set attribute-field of file-range */
ZFUNC ZINT Sdp_MsgSetAfFileRange(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc,
                ZUINT iStartOffset, ZUINT iStopOffset);

/* sdp message get attribute-field of path */
ZFUNC ZINT Sdp_MsgGetAfPath(ST_SDP_MDESC *pstMdesc, 
                ST_ZOS_SSTR **ppstUri);

/* sdp message enumrate attribute-field of candidate */
ZFUNC ZINT Sdp_MsgEnumAfCand(ST_SDP_MDESC *pstMdesc, ST_SDP_CAND **ppstCand);

/* sdp message Get attribute-field of candidate */
ZFUNC ZINT Sdp_MsgSetAfCand(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc,
                ZCHAR *pcFoundation, ZUSHORT wCompId, ZUCHAR ucTptType,
                ZUINT iPriority, ST_ZOS_INET_ADDR *pstAddr, ZUCHAR ucCandType,
                ST_ZOS_INET_ADDR *pstRelAddr);

/* sdp message get attribute-field of remote-candidate */
ZFUNC ZINT Sdp_MsgGetAfRmtCand(ST_SDP_MDESC *pstMdesc, ZUSHORT wCompId,
                ST_ZOS_INET_ADDR *pstAddr);

/* sdp message set attribute-field of remote-candidate */
ZFUNC ZINT Sdp_MsgSetAfRmtCand(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc,
                ZUSHORT wCompId, ST_ZOS_INET_ADDR *pstAddr);

/* sdp message exist media-descriptions, type see EN_SDP_MEDIA_TYPE */
ZFUNC ZBOOL Sdp_MsgExistMdesc(ST_SDP_SESS_DESC *pstMsg, 
                ZUCHAR ucMediaType, ST_SDP_MDESC **ppstMdesc);

/* sdp message exist attribute field, type see EN_SDP_ATTR_TYPE */
ZFUNC ZBOOL Sdp_MsgMdescExistAf(ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucAttrType, ST_SDP_AF **ppstAf);

/* sdp message get attribute-field of fingerprint */
ZFUNC ZINT Sdp_MsgGetAfFgrprnt(ST_SDP_MDESC *pstMdesc, ZUCHAR *pucHashType, 
                ST_ZOS_SSTR **ppstFgrprntVal);

/* sdp message add attribute-field of fingerprint */
ZFUNC ZINT Sdp_MsgAddAfFgrprnt(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUCHAR ucHashType, ZCHAR *pcFgrprntVal);

/* sdp message enumrate attribute-field of crypto */
ZFUNC ZINT Sdp_MsgEnumAfCrypto(ST_SDP_MDESC *pstMdesc, ST_SDP_CRYPTO **ppstCrypto);

/* sdp message get attribute-field of srtp key-info */
ZFUNC ZINT Sdp_MsgGetAfSrtpKeyInfo(ST_SDP_CRYPTO *pstCrypto, ZUINT *piTag,
                ZUCHAR *pucCryptoSuite, ST_ZOS_SSTR **ppstKeySalt,
                ZUINT *piLifeTime, ZUINT *piMki, ZUINT *piMkiLen);

/* sdp message set attribute-field of srtp key-info */
ZFUNC ZINT Sdp_MsgSetAfSrtpKeyInfo(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUINT iTag, ZUCHAR ucCryptoSuite, ST_ZOS_SSTR *pstKeySalt,
                ST_SDP_CRYPTO **ppstCrypto);

/* sdp message get attribute-field of srtp key-info */
ZFUNC ZINT Sdp_MsgGetAfSrtpSessParam(ST_SDP_CRYPTO *pstCrypto,
                ZUCHAR ucSessParmType, ST_SDP_CRYPTO_SESS_PARM **ppstSessParm);

/* sdp message get attribute-field of srtp key-info */
ZFUNC ZINT Sdp_MsgSetAfSrtpSessParam(ZUBUF zMemBuf,
                ST_SDP_CRYPTO *pstCrypto, ZUCHAR ucParmType,
                ST_SDP_CRYPTO_SESS_PARM **ppstSessParm);

/* sdp message get attribute-field of extmap */
ZFUNC ZINT Sdp_MsgGetAfExtMap(ST_SDP_MDESC *pstMdesc, ZUINT *piValue,
                ZUINT *piDirection, ZCONST ZCHAR *pcUri,
                ST_ZOS_SSTR **ppstExtAttr);

/* sdp message set attribute-field of extmap */
ZFUNC ZINT Sdp_MsgSetAfExtMap(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc,
                ZUINT iValue, ZUINT *piDirection, ZCHAR *pcUri,
                ZCHAR *pcExtAttr);

/* sdp message set attribute-field of max-size */
ZFUNC ZINT Sdp_MsgSetAfMaxSize(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc, 
                ZUINT iMaxSize);

/* sdp message get attribute-field content */
ZFUNC ZINT Sdp_MsgGetAfContentType(ST_SDP_MDESC *pstMdesc, ZUCHAR *pucAttrType);

/* sdp message set attribute-field of msrp-cema */
ZFUNC ZINT Sdp_MsgSetAfSetMsrpCema(ZUBUF zMemBuf, ST_SDP_MDESC *pstMdesc);

                
#ifdef __cplusplus
}
#endif

#endif /* _SDP_ABNF_MSG_H__ */

