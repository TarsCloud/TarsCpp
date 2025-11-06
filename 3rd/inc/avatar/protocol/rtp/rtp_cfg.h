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
  File name     : rtp_cfg.h
  Module        : rtp protocol config
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the rtp protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _RTP_CFG_H__
#define _RTP_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* rtp config get log level, default is ZLOG_LEVEL_ERROR */
ZFUNC ZUINT Rtp_CfgGetLogLevel(ZFUNC_VOID);

/* rtp config get task lock mode, default is using self lock */
ZFUNC ZBOOL Rtp_CfgGetTaskLock(ZFUNC_VOID);

/* rtp config get ip tos value, precedence value is from 0 to 7, 
   tos type could be any combination of ZSOCK_TOS_XXX */
ZFUNC ZINT Rtp_CfgGetIpTosVal(ZUCHAR *pucPrecLevel, ZUCHAR *pucTosType);

/* rtp config get session number, default is 10 */
ZFUNC ZUINT Rtp_CfgGetSessNum(ZFUNC_VOID);

/* rtp config get participant number, default is 10 */
ZFUNC ZUINT Rtp_CfgGetPtptNum(ZFUNC_VOID);

/* rtp config get port begin, default is 3700 */
ZFUNC ZUINT Rtp_CfgGetPortBegin(ZFUNC_VOID);

/* rtp config get enable check participant, default is ZTRUE */
ZFUNC ZBOOL Rtp_CfgGetEnChkPtpt(ZFUNC_VOID);

/* rtp config get if use epoc time */
ZFUNC ZBOOL Rtp_CfgGetUseEpocTime(ZFUNC_VOID);

/* rtp config get socket type */
ZFUNC ZINT Rtp_CfgGetSockType(ZFUNC_VOID);

/* rtp config set log level */
ZFUNC ZINT Rtp_CfgSetLogLevel(ZUINT iLevel);

/* rtp config set task lock */
ZFUNC ZINT Rtp_CfgSetTaskLock(ZBOOL bSelfLock);

/* rtp config set ip tos value, precedence value is from 0 to 7, 
   tos type could be any combination of ZSOCK_TOS_XXX */
ZFUNC ZINT Rtp_CfgSetIpTosVal(ZUCHAR ucPrecLevel, ZUCHAR ucTosType);

/* rtp config set session number */
ZFUNC ZINT Rtp_CfgSetSessNum(ZUINT iNum);

/* rtp config set participant number */
ZFUNC ZINT Rtp_CfgSetPtptNum(ZUINT iNum);

/* rtp config set port begin */
ZFUNC ZINT Rtp_CfgSetPortBegin(ZUINT iPort);

/* rtp config set enable check participant */
ZFUNC ZINT Rtp_CfgSetEnChkPtpt(ZBOOL bEnable);

/* rtp config set use epoc time */
ZFUNC ZINT Rtp_CfgSetUseEpocTime(ZBOOL bEnable);

/* rtp config set socket type */
ZFUNC ZINT Rtp_CfgSetSockType(ZINT iSockType);

#ifdef __cplusplus
}
#endif

#endif /* _RTP_CFG_H__ */

