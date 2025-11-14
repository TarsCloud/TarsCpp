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
  File name     : sip_abnf.h
  Module        : sip protocol
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-05-01
  Description   :
    Marcos and structure definitions required by the sip abnf code.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_ABNF_H__
#define _SIP_ABNF_H__

/** 
 * @file
 * @brief SIP ABNF decode and encode header files.
 *
 * To include the interfaces of SIP ABNF decode and encode, 
 * use the following directive:
 * @code
 * #include "sip_abnf.h"
 * @endcode
 */

/*lint -save -e* */
#include "zos.h"                     /* zos system services interface */
#include "abnf.h"                    /* abnf typedefs and interfaces */

#ifndef SIP_ABNF_SUPT_NOSDP
#include "sdp_abnf.h"                /* sdp abnf typedefs and interfaces */
#endif
/*lint -restore */

#include "sip_abnf_cfg.h"            /* sip abnf config interface */
#include "sip_abnf_type.h"           /* sip abnf typedef and interface */
#include "sip_abnf_chrset.h"         /* sip abnf chrset interfaces */
#include "sip_abnf_decode.h"         /* sip abnf decode interfaces */
#include "sip_abnf_encode.h"         /* sip abnf encode interfaces */
#include "sip_abnf_util.h"           /* sip abnf utility interfaces */

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Initiate the SIP ABNF resource.
 *
 * @retval ZOK Initialization successfully.
 * @retval ZFAILED Initialization failed.
 *
 * @see Sip_AbnfDestroy
 */
ZFUNC ZINT Sip_AbnfInit(ZFUNC_VOID);

/**
 * @brief Destroy the SIP ABNF resource.
 *
 * @return [none].
 *
 * @see Sip_AbnfInit
 */
ZFUNC ZVOID Sip_AbnfDestroy(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_ABNF_H__ */
