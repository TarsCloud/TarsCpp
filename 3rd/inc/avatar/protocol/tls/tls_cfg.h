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
  File name     : tls_cfg.h
  Module        : tls config
  Author        : zocol
  Version       : 0.1
  Created on    : 2008-12-16
  Description   :
    Marcos and structure definitions required by the tls config.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _TLS_CFG_H__
#define _TLS_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* tls library type */
typedef enum EN_TLS_LIB_TYPE
{
    EN_TLS_LIB_OPENSSL,              /* openssl library */
    EN_TLS_LIB_OTHER                 /* other library */
} EN_TLS_LIB_TYPE;

/* tls config get log level, default is ZLOG_LEVEL_ALL */
ZFUNC ZUINT Tls_CfgGetLogLevel(ZFUNC_VOID);

/* tls config get library type, default is EN_TLS_LIB_OPENSSL */
ZFUNC ZUINT Tls_CfgGetLibType(ZFUNC_VOID);

/* tls config set log level */
ZFUNC ZINT Tls_CfgSetLogLevel(ZUINT iLevel);

/* tls config set library type */
ZFUNC ZINT Tls_CfgSetLibType(ZUINT iLibType);

#ifdef __cplusplus
}
#endif

#endif /* _TLS_CFG_H__ */

