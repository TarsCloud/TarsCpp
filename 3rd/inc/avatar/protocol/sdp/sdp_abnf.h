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
  File name     : sdp_abnf.h
  Module        : sdp protocol abnf
  Author        : Zocol  
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the sdp abnf

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SDP_ABNF_H__
#define _SDP_ABNF_H__

/* header include files */
/*lint -save -e* */
#include "zos.h"                     /* zos system services interface */
#include "abnf.h"                    /* abnf typedef and interface */
/*lint -restore */

#include "sdp_abnf_cfg.h"            /* sdp abnf configuration */
#include "sdp_abnf_type.h"           /* sdp abnf typedef and interface */
#include "sdp_abnf_chrset.h"         /* sdp abnf chrset interfaces */
#include "sdp_abnf_decode.h"         /* sdp abnf decode interfaces */
#include "sdp_abnf_encode.h"         /* sdp abnf encode interfaces */
#include "sdp_abnf_msg.h"            /* sdp abnf message interfaces */
    
#ifdef __cplusplus
extern "C"{
#endif 

/* sdp abnf initialization */
ZFUNC ZINT Sdp_AbnfInit(ZFUNC_VOID);

/* sdp abnf destroy */
ZFUNC ZVOID Sdp_AbnfDestroy(ZFUNC_VOID);

/* sdp abnf decode message */
ZFUNC ZINT Sdp_DecodeMsgLst(ST_ZOS_SSTR *pstDataStr, ZDBUF zMemBuf, 
                ST_ABNF_ERR_INFO *pstErrInfo, ST_SDP_SESS_DESC_LST *pstMsgLst);

ZFUNC ZINT Sdp_DecodeMsg(ST_ZOS_SSTR *pstDataStr, ZDBUF zMemBuf, 
                ST_ABNF_ERR_INFO *pstErrInfo, ST_SDP_SESS_DESC *pstMsg);

/* sdp abnf encode message */
ZFUNC ZINT Sdp_EncodeMsgLst(ST_SDP_SESS_DESC_LST *pstMsgLst, 
                ZDBUF zMemBuf, ST_ABNF_ERR_INFO *pstErrInfo);

ZFUNC ZINT Sdp_EncodeMsg(ST_SDP_SESS_DESC *pstMsg, ZDBUF zMemBuf, 
                ST_ABNF_ERR_INFO *pstErrInfo);

/* sdp message get encoding name from type */
ZFUNC ZCHAR *Sdp_EncodingToName(ZUCHAR ucEncoding);

ZFUNC ZUCHAR Sdp_EncodingFromName(ZCONST ZCHAR *pcName);

#ifdef __cplusplus
}
#endif 

#endif /* _SDP_ABNF_H__ */

