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
  File name     : http_msg_cpy.h
  Module        : http protocol service message copy
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the http protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTP_MSG_CPY_H__
#define _HTTP_MSG_CPY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* http message copy structure string */
#define HTTP_CPY_SSTR(_membuf, _dstsstr, _srcsstr) do { \
    if (Zos_SStrXCpy(_membuf, _dstsstr, _srcsstr) != ZOK) \
        return ZFAILED; \
} while (0)

/* http copy function type defination. */
typedef ZINT (*PFN_HTTPANYCPY)(ZUBUF zBufId, ZVOID *pstDst, 
                ZVOID *pstSrc);

/* http message copy list */
ZFUNC ZINT Http_CpyList(ZUBUF zBufId, ZVOID *pDstLst, ZVOID *pSrcLst, 
                ZUINT iSize, PFN_HTTPANYCPY pfnCpy);

/* http message copy list data */
ZFUNC ZINT Http_CpyListData(ZUBUF zBufId, ZVOID *pDstLst, ZVOID *pSrc, 
                ZUINT iSize, PFN_HTTPANYCPY pfnCpy);

/* http message copy userinfo */
ZFUNC ZINT Http_CpyUserInfo(ZUBUF,
                ST_HTTP_USER_INFO *pstDst,
                ST_HTTP_USER_INFO *pstSrc);

/* http message copy host */
ZFUNC ZINT Http_CpyHost(ZUBUF,
                ST_HTTP_HOST *pstDst,
                ST_HTTP_HOST *pstSrc);

/* http message copy hostport */
ZFUNC ZINT Http_CpyHostPort(ZUBUF,
                ST_HTTP_HOST_PORT *pstDst,
                ST_HTTP_HOST_PORT *pstSrc);

/* http message copy hier_part */
ZFUNC ZINT Http_CpyHierPart(ZUBUF,
                ST_HTTP_HIER_PART *pstDst,
                ST_HTTP_HIER_PART *pstSrc);

/* http message copy opaque_part */
ZFUNC ZINT Http_CpyOpaquePart(ZUBUF,
                ST_HTTP_OPAQUE_PART *pstDst,
                ST_HTTP_OPAQUE_PART *pstSrc);

/* http message copy absoluteURI */
ZFUNC ZINT Http_CpyAbsoUri(ZUBUF,
                ST_HTTP_ABSO_URI *pstDst,
                ST_HTTP_ABSO_URI *pstSrc);

/* http message copy segment */
ZFUNC ZINT Http_CpySeg(ZUBUF,
                ST_HTTP_SEGMENT *pstDst,
                ST_HTTP_SEGMENT *pstSrc);

/* http message copy abs-path */
ZFUNC ZINT Http_CpyAbsPath(ZUBUF,
                ST_HTTP_ABS_PATH *pstDst,
                ST_HTTP_ABS_PATH *pstSrc);

/* http message copy net-path */
ZFUNC ZINT Http_CpyNetPath(ZUBUF,
                ST_HTTP_NET_PATH *pstDst,
                ST_HTTP_NET_PATH *pstSrc);

/* http message copy srvr */
ZFUNC ZINT Http_CpySrvr(ZUBUF,
                ST_HTTP_SRVR *pstDst,
                ST_HTTP_SRVR *pstSrc);

/* http message copy authority */
ZFUNC ZINT Http_CpyAuthor(ZUBUF,
                ST_HTTP_AUTHOR *pstDst,
                ST_HTTP_AUTHOR *pstSrc);

/* http message copy request-uri */
ZFUNC ZINT Http_CpyReqUri(ZUBUF,
                ST_HTTP_REQ_URI *pstDst,
                ST_HTTP_REQ_URI *pstSrc);

/* http message copy addr-spec */
ZFUNC ZINT Http_CpyAddrSpec(ZUBUF,
                ST_HTTP_ADDR_SPEC *pstDst,
                ST_HTTP_ADDR_SPEC *pstSrc);

/* http message copy name-addr */
ZFUNC ZINT Http_CpyNameAddr(ZUBUF,
                ST_HTTP_NAME_ADDR *pstDst,
                ST_HTTP_NAME_ADDR *pstSrc);

/* http message copy Method */
ZFUNC ZINT Http_CpyMethod(ZUBUF,
                ST_HTTP_METHOD *pstDst,
                ST_HTTP_METHOD *pstSrc);

/* http message copy c-p-q */
ZFUNC ZINT Http_CpyQval(ZUBUF, 
                ST_HTTP_QVAL *pstDst,
                ST_HTTP_QVAL *pstSrc);

/* http message copy http parameter */
ZFUNC ZINT Http_CpyParm(ZUBUF, 
                ST_HTTP_PARM *pstDst,
                ST_HTTP_PARM *pstSrc);

/* http message copy credentials */
ZFUNC ZINT Http_CpyCredents(ZUBUF,
                ST_HTTP_CREDENTS *pstDst,
                ST_HTTP_CREDENTS *pstSrc);

/* http message copy digest response */
ZFUNC ZINT Http_CpyDigestRsp(ZUBUF,
                ST_HTTP_DIGEST_RSP *pstDst,
                ST_HTTP_DIGEST_RSP *pstSrc);

/* http message copy dig rep */
ZFUNC ZINT Http_CpyDigRsp(ZUBUF,
                ST_HTTP_DIG_RSP *pstDst,
                ST_HTTP_DIG_RSP *pstSrc);

/* http message copy algorithm */
ZFUNC ZINT Http_CpyAlgo(ZUBUF,
                ST_HTTP_ALGO *pstDst,
                ST_HTTP_ALGO *pstSrc);

/* http message copy type value */
ZFUNC ZINT Http_CpyTypeVal(ZUBUF,
                ST_HTTP_TYPE_VAL *pstDst,
                ST_HTTP_TYPE_VAL *pstSrc);

/* http message copy other-response */
ZFUNC ZINT Http_CpyOtherRsp(ZUBUF,
                ST_HTTP_OTHER_RSP *pstDst,
                ST_HTTP_OTHER_RSP *pstSrc);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_MSG_CPY_H__ */

