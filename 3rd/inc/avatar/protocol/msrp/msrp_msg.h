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
  File name     : msrp_msg.h
  Module        : msrp protocol
  Author        : cathy.chen
  Version       : 0.1
  Created on    : 2010-10-18
  Description   :
    Marcos and structure definitions required by the msrp protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_MSG_H__
#define _MSRP_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Copy function type defination. */
typedef ZINT (*PFN_MSRPANYCPY)(ZUBUF zBufId, ZVOID *pstDst, 
                ZVOID *pstSrc);

/* msrp message create */
ZINT Msrp_MsgCreate(ST_MSRP_MSG **ppstMsg);

/* msrp message create with memory buffer */
ZINT Msrp_MsgCreateX(ZUBUF zBufId, ST_MSRP_MSG **ppstMsg);

/* msrp message create request message */
ZINT Msrp_MsgCreateReq(ZUCHAR ucMethod, ST_ZOS_SSTR *pstTransId,
                ST_MSRP_MSG **ppstMsg);

/* msrp message create response message */
ZINT Msrp_MsgCreateRsp(ZUINT iStatCode, ST_ZOS_SSTR *pstTransId, 
                ST_MSRP_MSG **ppstMsg);

/* msrp message Clone */
ZINT Msrp_MsgClone(ST_MSRP_MSG *pstMsg);

/* msrp message delete */
ZFUNC ZINT Msrp_MsgDelete(ST_MSRP_MSG *pstMsg);

/* msrp message decode, zMsgBuf will be stored in ppstMsg */
ZFUNC ZINT Msrp_MsgDecode(ZDBUF zMsgBuf, ST_MSRP_MSG **ppstMsg);

/* msrp message encode, pzMsgBuf will point to zMsgBuf in pstMsg */
ZINT Msrp_MsgEncode(ST_MSRP_MSG *pstMsg, ZDBUF *pzMsgBuf);

/* msrp message add To-Path or From-Path header */
ZINT Msrp_MsgAddHdrPath(ST_MSRP_MSG *pstMsg, ZCHAR *pcUri, 
                ZBOOL bFromPath);

/* msrp message add Message-ID header */
ZINT Msrp_MsgAddHdrMsgId(ST_MSRP_MSG *pstMsg, ZCHAR *pcMsgId);

/* msrp message add Byte-Range header */
ZINT Msrp_MsgAddHdrByteRange(ST_MSRP_MSG *pstMsg, ZUINT iLen, 
                ZUINT iSentLen, ZUINT iAllLen);

/* msrp message add Success-Report header */
ZINT Msrp_MsgAddHdrSuccRpt(ST_MSRP_MSG *pstMsg, ZBOOL bSuccRpt);

/* msrp message add Failure-Report header */
ZINT Msrp_MsgAddHdrFailRpt(ST_MSRP_MSG *pstMsg, ZBOOL bFailRpt);

/* msrp message add Content-Type header */
ZINT Msrp_MsgAddHdrContType(ST_MSRP_MSG *pstMsg, ZUCHAR ucMType, 
                ZCHAR *pcExtMType, ZUCHAR ucMSType, ZCHAR *pcExtMSType);

/* msrp message add Content-Type header */
ZINT Msrp_MsgAddHdrContTypeX(ST_MSRP_MSG *pstMsg, 
                ZCHAR *pcContType);

/* msrp message add Content-Disposition header */
ZINT Msrp_MsgAddHdrContDisp(ST_MSRP_MSG *pstMsg, ZUCHAR ucType, 
                ZCHAR *pcFileName);

/* msrp message add Status header */
ZINT Msrp_MsgAddHdrStatus(ST_MSRP_MSG *pstMsg, ZUINT iStatCode);

/* msrp message pick up Status header */
ZINT Msrp_MsgPickHdrStatus(ST_MSRP_MSG *pstMsg, 
                ZUINT *piStatCode);

/* msrp message copy the header */
ZINT Msrp_MsgCpyHdr(ST_MSRP_MSG *pstDstMsg, ST_MSRP_MSG *pstSrcMsg, 
                ZUCHAR ucSType, ZUCHAR ucDType, PFN_MSRPANYCPY pfnCpyHdr);

/* msrp message copy header From-Path or To-Path */
ZINT Msrp_MsgCpyHdrPath(ZUBUF zBufId, ST_MSRP_HDR_PATH *pstDst, 
                ST_MSRP_HDR_PATH *pstSrc);

/* msrp message copy header Message-Id */
ZINT Msrp_MsgCpyHdrMsgId(ZUBUF zBufId, 
                ST_MSRP_HDR_MSG_ID *pstDst, ST_MSRP_HDR_MSG_ID *pstSrc);

/* msrp message copy header Byte-Range */
ZINT Msrp_MsgCpyHdrByteRange(ZUBUF zBufId, 
                ST_MSRP_HDR_BYTE_RANGE *pstDst, ST_MSRP_HDR_BYTE_RANGE *pstSrc);

/* msrp message copy msrp-uri */
ZINT Msrp_MsgCpyMsrpUri(ZUBUF zBufId, ST_MSRP_MSRP_URI *pstDst, 
                ST_MSRP_MSRP_URI *pstSrc);

/* msrp message copy msrp-uri, user should free them by Zos_SysStrFree */
ZINT Msrp_MsgGetMsrpUri(ZCHAR *pcUri, ZCHAR **ppcHost, 
                ZUSHORT *pwPort, ZCHAR **ppcSessId, ZUCHAR *pucTptType);

/* msrp parameter any type list add */
ZINT Msrp_ParmAnyLstAdd(ZUBUF zBufId, ZVOID *pParmLst, ZINT iSize, 
                ZVOID **ppParm);

/* msrp parameter any type list remove */
ZINT Msrp_ParmAnyLstRmv(ZVOID *pParmLst, ZVOID *pParm);

/* msrp msrp-uri list add */
ZINT Msrp_MsrpUriLstAdd(ZUBUF zBufId,
                ST_MSRP_MSRP_URI_LST *pstMsrpUriLst,
                ST_MSRP_MSRP_URI **ppstMsrpUri);

/* msrp parameter msrp-uri list remove */
ZINT Msrp_MsrpUriLstRmv(ST_MSRP_MSRP_URI_LST *pstMsrpUriLst, 
                ST_MSRP_MSRP_URI *pstMsrpUri);

/* msrp disposition parm list add */
ZINT Msrp_DisParmLstAdd(ZUBUF zBufId,
                ST_MSRP_DISP_PARM_LST *pstDisParmLst,
                ST_MSRP_DISP_PARM **ppstDisParm);

/* msrp disposition parm list remove */
ZINT Msrp_DisParmLstRmv(ST_MSRP_DISP_PARM_LST *pstDisParmLst, 
                ST_MSRP_DISP_PARM *pstDisParm);

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_MSG_H__ */

