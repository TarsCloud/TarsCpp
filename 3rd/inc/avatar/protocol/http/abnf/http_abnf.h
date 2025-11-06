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
  File name     : http_abnf.h
  Module        : http protocol
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-03-30
  Description   :
    Marcos and structure definitions required by the http abnf code.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTP_ABNF_H__
#define _HTTP_ABNF_H__

/*lint -save -e* */
#include "zos.h"                     /* zos system services interface */
#include "abnf.h"                    /* abnf interface */
/*lint -restore */

#include "http_abnf_cfg.h"           /* http abnf config */
#include "http_abnf_type.h"          /* http abnf typedef and interface */
#include "http_abnf_chrset.h"        /* http abnf character set */
#include "http_abnf_util.h"          /* http abnf utility interfaces */

#ifdef __cplusplus
extern "C"{
#endif

/* http abnf init */
ZFUNC ZINT Http_AbnfInit(ZFUNC_VOID);

/* http abnf destroy */
ZFUNC ZVOID Http_AbnfDestroy(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_ABNF_H__ */

