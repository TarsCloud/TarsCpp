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
  File name     : xcapc_util.h
  Module        : xcap client
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-04-22
  Description   :
    Marcos and structure definitions required by the xcapc utility.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XCAPC_UTIL_H__
#define _XCAPC_UTIL_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xcapc request message create */
ZFUNC ZINT Xcapc_ReqMsgCreate(ST_XCAPC_REQ_MSG **ppstReqMsg);

/* xcapc request message delete */
ZFUNC ZINT Xcapc_ReqMsgDelete(ST_XCAPC_REQ_MSG *pstReqMsg);

/* xcapc response message delete */
ZFUNC ZINT Xcapc_RspMsgDelete(ST_XCAPC_RSP_MSG *pstRspMsg);

/* xcapc get the attribute string from xml message */
ZFUNC ZINT Xcapc_XmlMsgGetAttr(ST_XML_MSG *pstXmlMsg, ST_ZOS_SSTR *pstAttr);

/* xcapc set the attribute string in xml message */
ZFUNC ZINT Xcapc_XmlMsgSetAttr(ST_XML_MSG *pstXmlMsg, ST_ZOS_SSTR *pstAttr);

/* xcapc get status description by status type */
ZFUNC ZCHAR * Xcapc_GetStatDesc(ZUCHAR ucStatType);

#ifdef __cplusplus
}
#endif

#endif /* _XCAPC_UTIL_H__ */

