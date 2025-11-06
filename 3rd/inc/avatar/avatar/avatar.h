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
  File name     : avatar.h
  Module        : avatar 
  Author        : avatar   
  Version       : 0.1   
  Created on    : 2012-07-23
  Description   :
      Data structure and function definitions required by the avatar interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _AVATAR_H__
#define _AVATAR_H__

/* Define if it is controled by license. */
#undef ZOS_LIMITED_VERSION

/* Define the time length in seconds for evaluation. */
#undef ZOS_EVAL_TIME

/* Define stg */
#undef ZOS_SUPT_STG

#include "zos/zos.h"
#include "zos/os/android/zpand_osdep.h"
#include "zos/os/ios/zpios_osdep.h"
#include "zos/os/windows/zpwin_osdep.h"
#include "abnf/abnf.h"
#include "utpt/utpt.h"
#include "zsh/zsh.h"
#include "protocol/dns/dns.h"
#include "protocol/http/abnf/http_abnf.h"
#include "protocol/http/abnf/http_msg_cpy.h"
#include "protocol/http/abnf/http_msg_util.h"
#include "protocol/http/httpc/httpc.h"
#include "protocol/rtp/rtp.h"
#include "protocol/xcapc/xcapc.h"
#include "protocol/sip/sip.h"
#include "protocol/sdp/sdp_abnf.h"
#include "protocol/stun/stun.h"
#include "protocol/msrp/msrp.h"
#include "protocol/tls/tls.h"
#include "protocol/imap/imap.h"
#include "xml/xml.h"
#include "xml/eax/eax.h"
#include "xml/xpath/xpath.h"
#include "util/zuuid.h"
#include "util/json/zjson.h"
#include "util/zini.h"
#include "util/zlcs.h"
#include "util/zbase64.h"

/**
 * @file
 * @brief Avatar version interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get Avatar version string. */
ZFUNC ZCONST ZCHAR * Avatar_GetVersion(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _AVATAR_H__ */

