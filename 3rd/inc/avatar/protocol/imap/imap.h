/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : imap.h
  Module        : imap protocol 
  Author        : tanek.ye
  Version       : 0.1
  Created on    : 2013-03-26
  Description   :
    Marcos and structure definitions required by the imap protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_H__
#define _IMAP_H__

/** 
 * @file imap.h
 * @brief IMAP Protocol Stack header files 
 * 
 * To include the definitions of IMAP, use the following directive:
 * @code
 * #include "imap.h"
 * @endcode
 *
 * Detail description of files in module:
 * -# @ref imap_abnf.h               "IMAP ABNF Interfaces"
 * -# @ref imap_cfg.h                "IMAP Configuration Interfaces"
 * -# @ref imap_ui.h                 "IMAP User Interfaces"
 * -# @ref imap_task.h               "IMAP task Interfaces"
 * -# @ref imap_evnt.h               "IMAP Event Interfaces"
 */

#include "utpt.h"                    /* utpt typedefs and interface */

#include "imap_abnf.h"               /* imap abnf typedefs and interfaces */
#include "imap_cfg.h"                /* imap configuration interfaces */
#include "imap_evnt.h"               /* imap event interfaces */
#include "imap_ui.h"                 /* imap upper layer interface */
#include "imap_wrap.h"               /* imap wrapt upper layer interface */
#include "imap_task.h"               /* imap task interfaces */

#endif /* _IMAP_H__ */

