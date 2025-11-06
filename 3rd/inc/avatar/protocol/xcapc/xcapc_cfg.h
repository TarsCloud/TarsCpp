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
  File name     : xcapc_cfg.h
  Module        : xcap client
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-04-22
  Description   :
    Marcos and structure definitions required by the xcapc config.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XCAPC_CFG_H__
#define _XCAPC_CFG_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xcapc hostname length */
#define XCAPC_HOST_NAME_LEN 64

/* xcapc host */
typedef struct tagXCAPC_HOST
{
    ZUCHAR ucType;                   /* host type EN_HTTP_HOST */
    ZUCHAR aucSpare[1];              /* for 32 bit alignment */
    ZUSHORT wPort;                   /* host port */
    union
    {
        ZUINT iIpv4Addr;             /* IPv4address */
        ZCHAR acName[XCAPC_HOST_NAME_LEN]; /* hostname */
        ZUCHAR aucIpv6Addr[ZINET_IPV6_ADDR_SIZE]; /* IPv6reference */
    } u;
} ST_XCAPC_HOST;

/* xcapc config get log level, default is ZLOG_LEVEL_ALL */
ZFUNC ZUINT Xcapc_CfgGetLogLevel(ZFUNC_VOID);

/* xcapc config get compact xml, default is ZFALSE */
ZFUNC ZBOOL Xcapc_CfgGetCompactXml(ZFUNC_VOID);

/* xcapc config get task priority, default is ZTASK_PRIORITY_NORMAL */
ZFUNC ZINT Xcapc_CfgGetTaskPriority(ZFUNC_VOID);

/* xcapc config get tcp connect times, default is 5 */
ZFUNC ZUINT Xcapc_CfgGetTcpConnTimes(ZFUNC_VOID);

/* xcapc config get wait connect timer length, default is 2000ms */
ZFUNC ZUINT Xcapc_CfgGetWaitConnTmrLen(ZFUNC_VOID);

/* xcapc config get wait response timer length, default is 32000ms */
ZFUNC ZUINT Xcapc_CfgGetWaitRspTmrLen(ZFUNC_VOID);

/* xcapc config get product value
   default is "XDM-client/1.0 (Juphoon System)" */
ZFUNC ZCHAR * Xcapc_CfgGetProductVal(ZFUNC_VOID);

/* xcapc config get local ipv4 address, default is "192.168.0.121" */
ZFUNC ZINT Xcapc_CfgGetLocalIpv4(ZUINT *piIpv4, ZUSHORT *pwPort);

/* xcapc config get local ipv6 address */
ZFUNC ZINT Xcapc_CfgGetLocalIpv6(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/* xcapc config get local address */
ZFUNC ZINT Xcapc_CfgGetLocalAddr(ST_ZOS_INET_ADDR *pstAddr);

/* xcapc config get server ipv4 address */
ZFUNC ZINT Xcapc_CfgGetServIpv4(ZUINT *piIpv4, ZUSHORT *pwPort);

/* xcapc config get server ipv6 address */
ZFUNC ZINT Xcapc_CfgGetServIpv6(ZUCHAR **ppucIpv6, ZUSHORT *pwPort);

/* xcapc config get server address */
ZFUNC ZINT Xcapc_CfgGetServAddr(ST_ZOS_INET_ADDR *pstAddr);

/* xcapc config get server host */
ZFUNC ST_XCAPC_HOST * Xcapc_CfgGetServHost(ZFUNC_VOID);

/* xcapc config set log level */
ZFUNC ZINT Xcapc_CfgSetLogLevel(ZUINT iLevel);

/* xcapc config set compact xml */
ZFUNC ZINT Xcapc_CfgSetCompactXml(ZBOOL bCompact);

/* xcapc config set task priority */
ZFUNC ZINT Xcapc_CfgSetTaskPriority(ZINT iPriority);

/* xcapc config set tcp connect times */
ZFUNC ZINT Xcapc_CfgSetTcpConnTimes(ZUINT iTimes);

/* xcapc config set wait connect timer length(second) */
ZFUNC ZINT Xcapc_CfgSetWaitConnTmrLen(ZUINT iLen);

/* xcapc config set wait response timer length(second) */
ZFUNC ZINT Xcapc_CfgSetWaitRspTmrLen(ZUINT iLen);

/* xcapc config set product value */
ZFUNC ZINT Xcapc_CfgSetProductVal(ZCHAR *pcProductVal);

/* xcapc config set local ipv4 address */
ZFUNC ZINT Xcapc_CfgSetLocalIpv4(ZUINT iIpv4, ZUSHORT wPort);

/* xcapc config set local ipv6 address */
ZFUNC ZINT Xcapc_CfgSetLocalIpv6(ZUCHAR *pucIpv6, ZUSHORT wPort);

/* xcapc config set server ipv4 address */
ZFUNC ZINT Xcapc_CfgSetServIpv4(ZUINT iIpv4, ZUSHORT wPort);

/* xcapc config set server ipv6 address */
ZFUNC ZINT Xcapc_CfgSetServIpv6(ZUCHAR *pucIpv6, ZUSHORT wPort);

/* xcapc config set server host */
ZFUNC ZINT Xcapc_CfgSetServHost(ST_XCAPC_HOST *pstHost);

#ifdef __cplusplus
}
#endif

#endif /* _XCAPC_CFG_H__ */

