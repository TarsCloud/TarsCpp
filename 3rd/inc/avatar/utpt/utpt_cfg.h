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
  File name     : utpt_cfg.h
  Module        : unify transport
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the utpt config.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _UTPT_CFG_H__
#define _UTPT_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* utpt config get log level, default is ZLOG_LEVEL_ALL */
ZFUNC ZUINT Utpt_CfgGetLogLevel(ZFUNC_VOID);

/* utpt config get drive mode, default is using self task */
ZFUNC ZBOOL Utpt_CfgGetDrvMode(ZFUNC_VOID);

/* utpt config get task lock mode, default is using self lock */
ZFUNC ZBOOL Utpt_CfgGetTaskLock(ZFUNC_VOID);

/* utpt config get task priority, default is ZTASK_PRIORITY_NORMAL */
ZFUNC ZINT Utpt_CfgGetTaskPriority(ZFUNC_VOID);

/* utpt config get task stack size, default is 8K */
ZFUNC ZUINT Utpt_CfgGetTaskStackSize(ZFUNC_VOID);

/* utpt config get connection number, default is 80 */
ZFUNC ZUINT Utpt_CfgGetConnNum(ZFUNC_VOID);

/* utpt config get receive buffer size, default is (1024 * 16) */
ZFUNC ZUINT Utpt_CfgGetRecvBufSize(ZFUNC_VOID);

/* utpt config get send buffer size, default is (1440 * 8) */
ZFUNC ZUINT Utpt_CfgGetSendBufSize(ZFUNC_VOID);

/* utpt config set log level */
ZFUNC ZINT Utpt_CfgSetLogLevel(ZUINT dwLevel);

/* utpt config set drive mode */
ZFUNC ZINT Utpt_CfgSetDrvMode(ZBOOL bSelfTask);

/* utpt config set task lock */
ZFUNC ZINT Utpt_CfgSetTaskLock(ZBOOL bSelfLock);

/* utpt config set task priority */
ZFUNC ZINT Utpt_CfgSetTaskPriority(ZINT iPriority);

/* utpt config set task stack size */
ZFUNC ZINT Utpt_CfgSetTaskStackSize(ZUINT iStackSize);

/* utpt config set connection number */
ZFUNC ZINT Utpt_CfgSetConnNum(ZUINT dwNum);

/* utpt config set receive buffer size */
ZFUNC ZINT Utpt_CfgSetRecvBufSize(ZUINT iSize);

/* utpt config set send buffer size */
ZFUNC ZINT Utpt_CfgSetSendBufSize(ZUINT iSize);

#ifdef __cplusplus
}
#endif

#endif /* _UTPT_CFG_H__ */

