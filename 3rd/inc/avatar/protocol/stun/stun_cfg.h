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
  File name     : stun_cfg.h
  Module        : stun
  Author        : anthouny.zhou
  Version       : 0.1
  Created on    : 2012-09-03
  Description:
    stun config

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _STUN_CFG_H__
#define _STUN_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* stun config get log level, default is ZLOG_LEVEL_ALL */
ZFUNC ZUINT Stun_CfgGetLogLevel(ZFUNC_VOID);

/* stun config get local ipv4 address, default is "192.168.0.10" */
ZFUNC ZUINT Stun_CfgGetLocalIpv4(ZFUNC_VOID);

/* stun config get server name, default is "stun.fwdnet.net" */
ZFUNC ZCHAR * Stun_CfgGetServName(ZFUNC_VOID);

/* stun config get server ipv4 address, default is "192.168.0.250" */
ZFUNC ZUINT Stun_CfgGetServIpv4(ZFUNC_VOID);

/* stun config get server udp port, default is 3478 */
ZFUNC ZUSHORT Stun_CfgGetServUPort(ZFUNC_VOID);

/* stun config Get transaiton block number, default is 80 */
ZFUNC ZUINT Stun_CfgGetQryNum(ZFUNC_VOID);

/* stun config Get try time length */
ZFUNC ZUINT Stun_CfgGetRtoMs(ZFUNC_VOID);

/* stun config Get try count */
ZFUNC ZUINT Stun_CfgGetRcCnt(ZFUNC_VOID);

/* stun config Get try count */
ZFUNC ZUINT Stun_CfgGetRmCnt(ZFUNC_VOID);

/* stun config get use finger print */
ZFUNC ZBOOL Stun_CfgGetFingerPrint(ZFUNC_VOID);

/* stun config set log level */
ZFUNC ZINT Stun_CfgSetLogLevel(ZUINT iLevel);

/* stun config set local ipv4 address */
ZFUNC ZINT Stun_CfgSetLocalIpv4(ZUINT iIpv4);

/* stun config set server name, it can ip address or domain name */
ZFUNC ZINT Stun_CfgSetServName(ZCHAR *pcName);

/* stun config set server ipv4 address */
ZFUNC ZINT Stun_CfgSetServIpv4(ZUINT iIpv4);

/* stun config set server udp port */
ZFUNC ZINT Stun_CfgSetServUPort(ZUSHORT wPort);

/* stun config set transaiton block number */
ZFUNC ZINT Stun_CfgSetQryNum(ZUINT iNum);

/* stun config set try time length */
ZFUNC ZINT Stun_CfgSetRtoMs(ZUINT iTimeMs);

/* stun config set try count */
ZFUNC ZINT Stun_CfgSetRcCnt(ZUINT iTimes);

/* stun config set try count */
ZFUNC ZINT Stun_CfgSetRmCnt(ZUINT iTimes);

/* stun config set finger print */
ZFUNC ZINT Stun_CfgSetFingerPrint(ZBOOL bFingerPrint);

#ifdef __cplusplus
}
#endif

#endif /* _STUN_CFG_H__ */

