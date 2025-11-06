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
  File name     : xcapc_ui.h
  Module        : xcap client
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-04-22
  Description   :
    Marcos and structure definitions required by the xcapc user interface.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XCAPC_UI_H__
#define _XCAPC_UI_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xcapc application usage event type of access session */
typedef enum EN_XCAPC_STAT_TYPE
{
    EN_XCAPC_STAT_CONN_FAIL,          /* connect to server failed */
    EN_XCAPC_STAT_CONN_TIMEOUT,       /* connect to server timeout */
    EN_XCAPC_STAT_SERV_DISCED,        /* server disconnected */
    EN_XCAPC_STAT_RECV_RSP,           /* received response */
    EN_XCAPC_STAT_WAIT_RSP_TIMEOUT,   /* wait for response timeout */
    EN_XCAPC_STAT_SEND_FAIL,          /* send request failed */
    EN_XCAPC_STAT_IN_REQING,          /* alreay in requesting */
    EN_XCAPC_STAT_WRONG_CREDENTS      /* wrong credents */
} EN_XCAPC_STAT_TYPE;

/* xcapc auc process xcap status */
typedef ZINT (*PFN_XAUCPROCSTAT)(ZCOOKIE zCookie, ZUINT iAucId, \
                ZUCHAR ucStatType, ST_XCAPC_RSP_MSG *pstRspMsg);

/* xcapc auc open resource */
ZFUNC ZINT Xcapc_Open(ZCOOKIE zCookie, PFN_XAUCPROCSTAT pfnProcStat,
                ZUINT *piAucId);

/* xcapc auc open resource */
ZFUNC ZINT Xcapc_OpenX(ZCOOKIE zCookie, PFN_XAUCPROCSTAT pfnProcStat,
                ST_ZOS_INET_ADDR *pstServAddr, ZCHAR *pcServHost, 
                ZUINT *piAucId);

/* xcapc auc close resource */
ZFUNC ZINT Xcapc_Close(ZUINT iAucId);

/* xcapc auc put a doucment, node or attribute */
ZFUNC ZINT Xcapc_Put(ZUINT iAucId, ST_XCAPC_REQ_MSG *pstReqMsg);

/* xcapc auc get a doucment, node or attribute */
ZFUNC ZINT Xcapc_Get(ZUINT iAucId, ST_XCAPC_REQ_MSG *pstReqMsg);

/* xcapc auc delete a doucment, node or attribute */
ZFUNC ZINT Xcapc_Del(ZUINT iAucId, ST_XCAPC_REQ_MSG *pstReqMsg);

#ifdef __cplusplus
}
#endif

#endif /* _XCAPC_UI_H__ */

