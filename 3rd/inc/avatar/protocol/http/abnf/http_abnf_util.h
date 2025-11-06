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
  File name     : http_abnf_util.h
  Module        : http protocol
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-05-91
  Description   :
    Marcos and structure definitions required by the http protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTP_ABNF_UTIL_H__
#define _HTTP_ABNF_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* http response codes */
/************** informational ***************/
#define HTTP_RSP_100_CONTINUE              100
#define HTTP_RSP_101_SWITCH_PROTO          101

/************** success *********************/
#define HTTP_RSP_200_OK                    200
#define HTTP_RSP_201_CREATED               201
#define HTTP_RSP_202_ACPTED                202
#define HTTP_RSP_203_NONAUTHOR_INFO        203
#define HTTP_RSP_204_NO_CONTENT            204
#define HTTP_RSP_205_RESET_CONTENT         205
#define HTTP_RSP_206_PARTIAL_CONTENT       206

/************** redirection *****************/
#define HTTP_RSP_300_MULT_CHOICES          300
#define HTTP_RSP_301_MOVED_PERM            301
#define HTTP_RSP_302_FOUND                 302
#define HTTP_RSP_303_SEE_OTHER             303
#define HTTP_RSP_304_NOT_MDFY              304
#define HTTP_RSP_305_USE_PROXY             305
#define HTTP_RSP_307_TMP_REDIRECT          307

/************** client Error ****************/
#define HTTP_RSP_400_BAD_REQ               400
#define HTTP_RSP_401_UNAUTHOR              401
#define HTTP_RSP_402_PAYMENT_REQUIRED      402
#define HTTP_RSP_403_FORBIDDEN             403
#define HTTP_RSP_404_NOT_FOUND             404
#define HTTP_RSP_405_METHOD_NOT_ALLOWED    405
#define HTTP_RSP_406_NOT_ACPTED            406
#define HTTP_RSP_407_PROXY_AUTHEN_REQUIRED 407
#define HTTP_RSP_408_REQ_TIMEOUT           408
#define HTTP_RSP_409_CONFLICT              409
#define HTTP_RSP_410_GONE                  410
#define HTTP_RSP_411_LEN_REQUIRED          411
#define HTTP_RSP_412_PRECOND_FAIL          412
#define HTTP_RSP_413_REQ_ENT_TOO_LARGE     413
#define HTTP_RSP_414_REQ_URI_TOO_LARGE     414
#define HTTP_RSP_415_UNSUPTED_MEDIA_TYPE   415
#define HTTP_RSP_416_REQ_RANGE_NOT_SATISF  416
#define HTTP_RSP_417_EXPECTATION_FAIL      417

/************** server Error ****************/
#define HTTP_RSP_500_INTERNAL_SERV_ERR     500
#define HTTP_RSP_501_NOT_IMPLEMENTED       501
#define HTTP_RSP_502_BAD_GW                502
#define HTTP_RSP_503_SRV_UNAVAIL           503
#define HTTP_RSP_504_GW_TIMEOUT            504
#define HTTP_RSP_505_VER_NOT_SUPTED        505

/* http data buffer size */
#define HTTP_MSGBUF_SIZE 1024
#define HTTP_MEMBUF_SIZE 1024

/* http create memory buffer */
#define HTTP_CREATE_MEMBUF(_membuf) \
    ZOS_MEMBUF_CREATE(_membuf, HTTP_MEMBUF_SIZE, 8, "http membuf")

/* http create memory buffer */
#define HTTP_CREATE_MEMBUFD(_membuf, _size, _data) \
    ZOS_MEMBUF_CREATECLRD(_membuf, HTTP_MEMBUF_SIZE, 8, "http membuf", \
                          _size, _data)

/* http create message buffer */
#define HTTP_CREATE_MSGBUF(_msgbuf) \
    ZOS_MSGBUF_CREATE(_msgbuf, HTTP_MSGBUF_SIZE, 8, "http msgbuf")

/* http create message buffer and alloc data */
#define HTTP_CREATE_MSGBUFD(_msgbuf, _size, _data) \
    ZOS_MSGBUF_CREATED(_msgbuf, HTTP_MSGBUF_SIZE, 8, "http msgbuf",\
                       _size, _data)

/* http create message buffer and add data */
#define HTTP_CREATE_MSGBUF_ADDD(_msgbuf, _data, _size) \
    ZOS_MSGBUF_CREATE_ADDD(_msgbuf, HTTP_MSGBUF_SIZE, 8, "http msgbuf", \
                           _data, _size)

/* http delete dbuf */
#define HTTP_DBUF_DELETE(_dbuf) \
    ZOS_DBUF_DELETE(_dbuf)

/* http clone dbuf */
#define HTTP_DBUF_CLONE(_dbuf) \
    ZOS_DBUF_CLONE(_dbuf)

/* http clone dbuf */
#define HTTP_DBUF_CLONE2(_dbuf, _newbuf) \
    ZOS_DBUF_CLONEX(_dbuf, _newbuf)

/* http alloc memory of specific size from dbuf */
#define HTTP_DBUF_ALLOC(_dbuf, _size) \
    Zos_UbufAlloc(_dbuf, _size)

/* http alloc memory(zeroed) of specific size from dbuf */
#define HTTP_DBUF_ALLOC_CLRD(_dbuf, _size) \
    Zos_UbufAllocClrd(_dbuf, _size)

/* http abnf log macros */
#define HTTP_LOG_ERROR Http_LogErrStr
#define HTTP_LOG_INFO  Http_LogInfoStr
#define HTTP_LOG_DBG   Http_LogDbgStr

/* http log ERROR string */
ZFUNC ZINT Http_LogErrStr(ZCHAR *pcFormat, ...) ZATTR_PRINTF(1,2);

/* http log INFO string */
ZFUNC ZINT Http_LogInfoStr(ZCHAR *pcFormat, ...) ZATTR_PRINTF(1,2);

/* http log DEBUG string */
ZFUNC ZINT Http_LogDbgStr(ZCHAR *pcFormat, ...) ZATTR_PRINTF(1,2);

/* http create message header by header type EN_HTTP_HDR
 * the return address is the union member of ST_HTTP_MSG_HDR */
ZFUNC ZVOID * Http_CreateMsgHdr(ST_HTTP_MSG *pstMsg, ZUCHAR ucType);

/* http delete the first message header by header type EN_HTTP_HDR */
ZFUNC ZINT Http_DeleteMsgHdr(ST_HTTP_MSG *pstMsg, ZUCHAR ucType);

/* http fill message header by header type EN_HTTP_HDR with string value */
ZFUNC ZINT Http_FillMsgHdr(ST_HTTP_MSG *pstMsg, ZUCHAR ucType, 
                ST_ZOS_SSTR *pstStr);

/* http fill extension message header by header name and value */
ZFUNC ZINT Http_FillExtHdr(ST_HTTP_MSG *pstMsg, ZCHAR *pcName, 
                ST_ZOS_SSTR *pstStr);

/* http fill extension message header by header name and value */
ZFUNC ZINT Http_FillExtHdrX(ST_HTTP_MSG *pstMsg, ZCHAR *pcName, 
                ST_ZOS_SSTR *pstVal);

/* http find message header by header type 
 * if the header was not decoded, the function will return ZNULL
 * the return address is the union member of  ST_HTTP_MSG_HDR */
ZFUNC ZVOID * Http_FindMsgHdr(ST_HTTP_MSG *pstMsg, ZUCHAR ucType);

/* http find message header by header type and index
 * if the header was not decoded, the function will return ZNULL
 * the return address is the union member of  ST_HTTP_MSG_HDR */
ZFUNC ZVOID * Http_FindMsgHdrX(ST_HTTP_MSG *pstMsg, ZUCHAR ucType, 
                ZINT iIndex);

/* http find extension message header by header name */
ZFUNC ZINT Http_FindExtHdr(ST_HTTP_MSG *pstMsg, ZCHAR *pcName, 
                ST_HTTP_HDR_EXT_HDR **ppstHdr);

/* http find extension message header by header name and index */
ZFUNC ZINT Http_FindExtHdrX(ST_HTTP_MSG *pstMsg, ZCHAR *pcName, ZINT iIndex, 
                ST_HTTP_HDR_EXT_HDR **ppstHdr);

/* http get message header count by header type */
ZFUNC ZINT Http_GetMsgHdrCount(ST_HTTP_MSG *pstMsg, ZUCHAR ucType);

/* http get extension message header count by header name */
ZFUNC ZINT Http_GetExtHdrCount(ST_HTTP_MSG *pstMsg, ZCHAR *pcName);

/* http get the content length from http message */
ZFUNC ZINT Http_GetContentLen(ST_HTTP_MSG *pstMsg, ZUINT *piContentLen);

/* http get the content length from string buffer */
ZFUNC ZINT Http_GetContentLen2(ST_ZOS_SSTR *pstStr, ZUINT *piContentLen);

/* http update the content length in headers */
ZFUNC ZINT Http_UpdateContentLen(ST_HTTP_MSG *pstMsg, ZUINT iContentLen);

/* http create message header in header list */
ZFUNC ZVOID * Http_HdrLstCreateHdr(ST_HTTP_MSG_HDR_LST *pstHdrLst, 
                ZUBUF zMemBuf, ZUCHAR ucHdrType);

/* http delete message header from header list */
ZFUNC ZINT Http_HdrLstDeleteHdr(ST_HTTP_MSG_HDR_LST *pstHdrLst, 
                ZUCHAR ucHdrType);

/* http find message header from header list */
ZFUNC ZVOID * Http_HdrLstFindHdr(ST_HTTP_MSG_HDR_LST *pstHdrLst, 
                ZUCHAR ucHdrType);

/* http get the content length from header list */
ZFUNC ZINT Http_HdrLstGetContentLen(ST_HTTP_MSG_HDR_LST *pstHdrLst,
                ZUINT *piContentLen);

/* http update the content length in header list */
ZFUNC ZINT Http_HdrLstUpdateContentLen(ST_HTTP_MSG_HDR_LST *pstHdrLst, 
                ZUBUF zMemBuf, ZUINT iContentLen);

/* http get status reason phrase from status code */
ZFUNC ZINT Http_ReasonFromCode(ZUINT iCode, ST_ZOS_SSTR *pstReason);

/* http convert uri from percent-encoded string to normal string */
ZFUNC ZINT Http_UriP2N(ZUBUF zMemBuf, ST_ZOS_SSTR *pstPUri,
                ST_ZOS_SSTR *pstNUri);

/* http convert uri from normal string to percent-encoded string */
ZFUNC ZINT Http_UriN2P(ZUBUF zMemBuf, ST_ZOS_SSTR *pstNUri,
                ST_ZOS_SSTR *pstPUri);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_ABNF_UTIL_H__ */

