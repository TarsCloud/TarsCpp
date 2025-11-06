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
  File name     : imap_abnf.h
  Module        : imap protocol abnf
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Function implement required by the imap protocol abnf

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_ABNF_H__
#define _IMAP_ABNF_H__

/**
 * @file imap_abnf.h
 * @brief IMAP ABNF Interface Functions
 *
 * To include the interfaces of IMAP ABNF decode and encode, 
 * use the following directive:
 * @code
 * #include "imap_abnf.h"
 * @endcode
 */

/*lint -save -e* */
#include "zos.h"                     /* zos system services interface */
#include "abnf.h"                    /* abnf typedefs and interfaces */
/*lint -restore */

#include "imap_abnf_cfg.h"           /* imap abnf config interface */
#include "imap_abnf_type.h"          /* imap abnf typedef and interface */
#include "imap_abnf_chrset.h"        /* imap abnf chrset interfaces */

#ifdef __cplusplus
extern "C"{
#endif

/* imap abnf init */
/**
 * @brief Initialize the IMAP ABNF resource.
 *
 * @retval ZOK Initialization successfully.
 * @retval ZFAILED Initialization failed.
 *
 * @see @ref Imap_AbnfDestroy
 */
ZFUNC ZINT Imap_AbnfInit(ZFUNC_VOID);

/**
 * @brief Destroy the IMAP ABNF resource.
 *
 * @return [none].
 *
 * @see @ref Imap_AbnfInit
 */
ZFUNC ZVOID Imap_AbnfDestroy(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _IMAP_ABNF_H__ */

