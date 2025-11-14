/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : msrp_abnf.h
  Module        : msrp protocol abnf
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Function implement required by the msrp protocol abnf

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_ABNF_H__
#define _MSRP_ABNF_H__

/**
 * @file msrp_abnf.h
 * @brief MSRP ABNF Interface Functions
 *
 * To include the interfaces of MSRP ABNF decode and encode, 
 * use the following directive:
 * @code
 * #include "msrp_abnf.h"
 * @endcode
 */

/*lint -save -e* */
#include "zos.h"                     /* zos system services interface */
#include "abnf.h"                    /* abnf typedefs and interfaces */
/*lint -restore */

#include "msrp_abnf_cfg.h"           /* msrp abnf config interface */
#include "msrp_abnf_type.h"          /* msrp abnf typedef and interface */
#include "msrp_abnf_chrset.h"        /* msrp abnf chrset interfaces */
#include "msrp_abnf_decode.h"        /* msrp abnf decode interfaces */
#include "msrp_abnf_encode.h"        /* msrp abnf encode interfaces */
#include "msrp_abnf_util.h"          /* msrp abnf utility interfaces */

#ifdef __cplusplus
extern "C"{
#endif

/* msrp abnf init */
/**
 * @brief Initialize the MSRP ABNF resource.
 *
 * @retval ZOK Initialization successfully.
 * @retval ZFAILED Initialization failed.
 *
 * @see @ref Msrp_AbnfDestroy
 */
ZFUNC ZINT Msrp_AbnfInit(ZFUNC_VOID);

/**
 * @brief Destroy the MSRP ABNF resource.
 *
 * @return [none].
 *
 * @see @ref Msrp_AbnfInit
 */
ZFUNC ZVOID Msrp_AbnfDestroy(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _MSRP_ABNF_H__ */

