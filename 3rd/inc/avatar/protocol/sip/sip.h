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
  File name     : sip.h
  Module        : sip protocol 
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-09-30
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_H__
#define _SIP_H__

/** 
 * @file
 * @brief SIP module header files.
 *
 * To include the definitions of SIP, use the following directive:
 * @code
 * #include "sip.h"
 * @endcode
 */

#include "zos.h"                     /* zos system services interface */
#include "utpt.h"                    /* utpt typedefs and interface */
#include "sdp_abnf.h"                /* sdp abnf typedefs and interfaces */
#include "sip_abnf.h"                /* sip abnf typedefs and interfaces */
#include "sip_type.h"                /* sip typedefs and interfaces */
#include "sip_cfg.h"                 /* sip configuration interfaces */
#include "sip_msg_cmp.h"             /* sip message compare interfaces */
#include "sip_msg_cpy.h"             /* sip message copy interfaces */
#include "sip_msg_util.h"            /* sip message utility interfaces */
#include "sip_util.h"                /* sip utility interfaces */
#include "sip_ui.h"                  /* sip upper layer interface */
#include "sip_task.h"                /* sip task interfaces */
#include "sip_dlg.h"                 /* sip dialog interfaces */
    
#endif /* _SIP_H__ */

