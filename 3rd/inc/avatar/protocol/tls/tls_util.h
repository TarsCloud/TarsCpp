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
  File name     : tls_util.h
  Module        : tls utility
  Author        : zocol
  Version       : 0.1
  Created on    : 2008-12-16
  Description   :
    Marcos and structure definitions required by the tls utility.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _TLS_UTIL_H__
#define _TLS_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* tls log macros */
#define TLS_LOG_ERROR   Tls_LogErrStr
#define TLS_LOG_INFO    Tls_LogInfoStr
#define TLS_LOG_DBG     Tls_LogDbgStr

/* tls log ERROR string */
ZINT Tls_LogErrStr(ZCHAR *pcFormat, ...) ZATTR_PRINTF(1,2);

/* tls log INFO string */
ZINT Tls_LogInfoStr(ZCHAR *pcFormat, ...) ZATTR_PRINTF(1,2);

/* tls log DEBUG string */
ZINT Tls_LogDbgStr(ZCHAR *pcFormat, ...) ZATTR_PRINTF(1,2);

/* tls calculate certificate fingerprint 
The caller is responsible for freeing it by Zos_SysStrFree */
ZFUNC ZCHAR * Tls_CalcCertFgrprnt(ZCHAR *pcCertFile, ZCHAR *pcAlgo);

/* tls message digest format to 'xx:xx:xx...' 
The caller is responsible for freeing it by Zos_SysStrFree */
ZFUNC ZCHAR * Tls_MdFmt(ZCHAR *pcMd, ZUINT iLen);

#ifdef __cplusplus
}
#endif

#endif /* _TLS_UTIL_H__ */

