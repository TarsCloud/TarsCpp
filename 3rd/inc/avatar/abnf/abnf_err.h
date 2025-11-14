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
  File name     : abnf_err.h
  Module        : abnf error library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-12-29
  Description   :
      Data structure and function definitions required 
      by the abnf error interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ABNF_ERR_H__
#define _ABNF_ERR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* abnf error code constants */
#define ABNF_ERR_NOMEM         1    /* no memory */
#define ABNF_ERR_DECINVMSG     2    /* decode invalid message */
#define ABNF_ERR_DECINVSTR     3    /* decode invalid string */
#define ABNF_ERR_DECINVDBUF    4    /* decode invalid dbuf */
#define ABNF_ERR_DECINVTKNMGR  5    /* decode invalid token manager */
#define ABNF_ERR_DECINVTKNID   6    /* decode invalid token id */
#define ABNF_ERR_DECINVSTRMEM  7    /* decode invalid string memory */
#define ABNF_ERR_DECINVCHRMEM  8    /* decode invalid character memory */
#define ABNF_ERR_DECINVTKNMEM  9    /* decode invalid token memory */
#define ABNF_ERR_DECINVDATAMEM 10   /* decode invalid data memory */
#define ABNF_ERR_DECINVADDRMEM 11   /* decode invalid ip address memory */
#define ABNF_ERR_DECINVMINLEN  12   /* decode invalid minimum length */
#define ABNF_ERR_DECINVMAXLEN  13   /* decode invalid maximum length */
#define ABNF_ERR_DECINVMINVAL  14   /* decode invalid minimum length */
#define ABNF_ERR_DECINVMAXVAL  15   /* decode invalid maximum length */
#define ABNF_ERR_DECINVDATA    16   /* decode invalid data */
#define ABNF_ERR_DECINVIPADDR  17   /* decode invalid ip addres */
#define ABNF_ERR_DECCHRILG     18   /* decode occur illegal character */
#define ABNF_ERR_DECMSGEOS     19   /* decode message not corrected finish */
#define ABNF_ERR_DECINVEOL     20   /* decode occur invalid eol */
#define ABNF_ERR_ENCINVMSG     20   /* encode invalid message */
#define ABNF_ERR_ENCINVSTR     21   /* encode invalid string */
#define ABNF_ERR_ENCINVDBUF    22   /* encode invalid dbuf */
#define ABNF_ERR_ENCBUFADD     23   /* encode buffer add data */
#define ABNF_ERR_ENCBUFREM     24   /* encode buffer remove data */
#define ABNF_ERR_ENCINVIPADDR  25   /* encode invalid ip adress */

/* abnf error stack size */
#define ABNF_ERR_STACK_SIZE 5

#if 0

/* abnf add error info */
#define ABNF_LOG_USER_ERR(_msg, _desc, _idnum)

#else /* support log */

/* abnf add error info */
#define ABNF_LOG_USER_ERR(_msg, _desc, _idnum) \
    Abnf_ErrLog(_msg, ZNULL, 0, _desc, _idnum)

#endif

/* abnf clear error info */
#define ABNF_CLR_USER_ERR(_msg) Abnf_ErrClear(_msg)

/* abnf log error */
#define ABNF_RETFAIL(_msg, _code) do { \
    if ((_msg)->pstErr) (_msg)->pstErr->wAbnfCode = _code; \
    return ZFAILED; \
} while (0)

/* abnf error init */
ZFUNC ZINT Abnf_ErrInit(ST_ABNF_ERR_INFO *pstErrInfo);

/* abnf error destroy */
ZFUNC ZVOID Abnf_ErrDestroy(ST_ABNF_ERR_INFO *pstErrInfo);

/* clear abnf error log */
ZFUNC ZINT Abnf_ErrLog(ST_ABNF_MSG *pstAbnfMsg, ZCHAR *pcFileName, 
                ZUINT iLineNo, ZCHAR *pcDesc, ZUINT iErrId);

/* clear abnf error info */
ZFUNC ZINT Abnf_ErrClear(ST_ABNF_MSG *pstAbnfMsg);

/* log print abnf error info */
ZFUNC ZINT Abnf_ErrLogPrint(ST_ABNF_MSG *pstAbnfMsg, ZCHAR *pcLogName);

#ifdef __cplusplus
}
#endif

#endif /* _ABNF_ERR_H__ */

