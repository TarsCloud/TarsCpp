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
  File name     : msrp.h
  Module        : msrp protocol 
  Author        : cathy.chen
  Version       : 0.1
  Created on    : 2010-10-19
  Description   :
    Marcos and structure definitions required by the msrp protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_H__
#define _MSRP_H__

/** 
 * @file msrp.h
 * @brief MSRP Protocol Stack header files 
 * 
 * To include the definitions of MSRP, use the following directive:
 * @code
 * #include "msrp.h"
 * @endcode
 *
 * Detail description of files in module:
 * -# @ref msrp_abnf.h               "MSRP ABNF Interfaces"
 * -# @ref msrp_cfg.h                "MSRP Configuration Interfaces"
 * -# @ref msrp_ui.h                 "MSRP User Interfaces"
 * -# @ref msrp_task.h               "MSRP task Interfaces"
 * -# @ref msrp_evnt.h               "MSRP Event Interfaces"
 */

#include "utpt.h"                    /* utpt typedefs and interface */

#include "msrp_abnf.h"               /* msrp abnf typedefs and interfaces */
#include "msrp_cfg.h"                /* msrp configuration interfaces */
#include "msrp_ui.h"                 /* msrp upper layer interface */
#include "msrp_task.h"               /* msrp task interfaces */
#include "msrp_evnt.h"               /* msrp event interfaces */
#include "msrp_msg.h"                /* msrp message interfaces */

#endif /* _MSRP_H__ */

