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
  File name     : tls.h
  Module        : tls
  Author        : zocol
  Version       : 0.1
  Created on    : 2008-12-16
  Description   :
    Marcos and structure definitions required by the tls.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _TLS_H__
#define _TLS_H__

/* tls support openssl */
#define TLS_LIB_USE_OPENSSL

#include "zos.h"                     /* zos system services interface */
#include "tls_ui.h"                  /* tls user interfaces */
#include "tls_util.h"                /* tls utility user interfaces */
#include "tls_cfg.h"                 /* tls config interfaces */

#ifdef __cplusplus
extern "C" {
#endif

/* tls init */
ZFUNC ZINT Tls_Init(ZFUNC_VOID);

/* tls destroy */
ZFUNC ZVOID Tls_Destroy(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _TLS_H__ */

