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
  File name     : dns.h
  Module        : dns library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2006-03-24    
  Description   :
      Data structure and function definitions required 
      by the dns interface

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _DNS_H__
#define _DNS_H__

/** 
 * @file
 * @brief DNS stack header files.
 *
 * To include the definitions of DNS, use the following directive:
 * @code
 * #include "dns.h"
 * @endcode
 */

/* header include files */
#include "zos.h"                     /* zos system services interface */
#include "dns_type.h"                /* dns typedef and interface */
#include "dns_cfg.h"                 /* dns config typedefs and interface */
#include "dns_ui.h"                  /* dns upper layer interface */
#include "dns_util.h"                /* dns utility interface */
#include "dns_task.h"                /* dns task interface */

#endif /* _DNS_H__ */

