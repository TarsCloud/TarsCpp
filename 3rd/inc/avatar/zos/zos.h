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
  File name     : zos.h
  Module        : zos system 
  Author        : zocol 
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Marcos and function definitions required by the zos system.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_H__
#define _ZOS_H__

/**
 * @addtogroup ZOS
 * @{
 * -# @ref zos_osenv.h               "ZOS OS Environment Definition"
 * -# @ref zos_type.h                "ZOS Basic Type Definition"
 * -# @ref zos_sysindep.h            "ZOS System Independent"
 * -# @ref zos_sysenv.h              "ZOS System Environment Definition"
 * -# @ref zos_syscfg.h              "ZOS System Configuration"
 * -# @ref zos_ctype.h               "ZOS Character Type Definition"
 * -# @ref zos_dlist.h               "ZOS Double List Operation"
 * -# @ref zos_slist.h               "ZOS Single List Operation"
 * -# @ref zos_dbuf.h                "ZOS Data Buffer Interfaces "
 * -# @ref zos_ebuf.h                "ZOS Encode Buffer Interfaces"
 * -# @ref zos_sbuf.h                "ZOS Structure Buffer Interfaces"
 * -# @ref zos_pbuf.h                "ZOS Pipe Buffer Interfaces"
 * -# @ref zos_cbuf.h                "ZOS Convergency Buffer Interfaces"
 * -# @ref zos_ubuf.h                "ZOS Unify Buffer Interfaces"
 * -# @ref zos_dump.h                "ZOS Stack Dump Interfaces"
 * -# @ref zos_log.h                 "ZOS Log Interfaces"
 * -# @ref zos_print.h               "ZOS Print Interfaces"
 * -# @ref zos_hash.h                "ZOS Hash Interfaces"
 * -# @ref zos_dhash.h               "ZOS Dynamic Hash Interfaces"
 * -# @ref zos_mutex.h               "ZOS Mutex Interfaces"
 * -# @ref zos_string.h              "ZOS String Interfaces" 
 * -# @ref zos_heap.h                "ZOS Heap Memory Interfaces"
 * -# @ref zos_pool.h                "ZOS Memory Pool Interfaces"
 * -# @ref zos_bpool.h               "ZOS Bitmap Memory Pool Interfaces"
 * -# @ref zos_mem.h                 "ZOS Memory Management Interfaces"
 * -# @ref zos_pmem.h                "ZOS Power Memory Interfaces"
 * -# @ref zos_inet.h                "ZOS Internet Interfaces "
 * -# @ref zos_socket.h              "ZOS Socket Interfaces"
 * -# @ref zos_time.h                "ZOS Time Interfaces"
 * -# @ref zos_queue.h               "ZOS Queue Operation"
 * -# @ref zos_pqueue.h              "ZOS Priority Queue Operation"
 * -# @ref zos_timer.h               "ZOS Timer Interfaces"
 * -# @ref zos_msg.h                 "ZOS Message Interfaces"
 * -# @ref zos_task.h                "ZOS Task Interfaces"
 * -# @ref zos_module.h              "ZOS Module Interfaces"
 * -# @ref zos_fsm.h                 "ZOS Finite State Machine Interfaces"
 * -# @ref zos_fsm_dump.h            "ZOS Fsm Dump Interfaces"
 * -# @ref zos_brick.h               "ZOS Brick Memory Interfaces"
 * -# @ref zos_dbkt.h                "ZOS Dynamic Bucket Interfaces"
 * -# @ref zos_omap.h                "ZOS Object Map Interfaces" 
 * -# @ref zos_ovec.h                "ZOS Object Vector Interfaces" 
 * -# @ref zos_rbt.h                 "ZOS Red Black Tree Interfaces" 
 * -# @ref zos_file.h                "ZOS File Interfaces"
 * -# @ref zos_osdep.h               "ZOS Dependent Interfaces"
 * -# @ref zos_evnt.h                "ZOS Event Interfaces"
 * -# @ref zos_trace.h               "ZOS Trace Interfaces"
 * @}
 * @file
 * @brief Zos system evnironment
 *
 * To include the definitions of Zos, use the following directive:
 * @code
 * #include "zos.h"
 * @endcode
 *
 */

/* zos header include files */
#include "zos_osenv.h"               /* zos os environment */
#include "zos_type.h"                /* zos data type */    
#include "zos_sysindep.h"            /* zos system independent */
#include "zos_sysenv.h"              /* zos system environment interfaces */
#include "zos_syscfg.h"              /* zos system configuration interfaces */
#include "zos_sysevnt.h"             /* zos system event interfaces */
#include "zos_cfg.h"                 /* zos configuration interfaces */
#include "zos_ctype.h"               /* zos character type */
#include "zos_dlist.h"               /* zos double list interfaces */
#include "zos_slist.h"               /* zos single list interfaces */
#include "zos_dbuf.h"                /* zos data buffer interfaces */
#include "zos_ebuf.h"                /* zos encode buffer interfaces */
#include "zos_sbuf.h"                /* zos structure buffer interfaces */
#include "zos_pbuf.h"                /* zos pipe buffer interfaces */
#include "zos_cbuf.h"                /* zos convergency buffer interfaces */
#include "zos_ubuf.h"                /* zos unify buffer interfaces */
#include "zos_dump.h"                /* zos stack dump interfaces */
#include "zos_log.h"                 /* zos log interfaces */
#include "zos_print.h"               /* zos print interfaces */
#include "zos_hash.h"                /* zos hash interfaces */
#include "zos_dhash.h"               /* zos dynamic hash interfaces */
#include "zos_mutex.h"               /* zos mutex interfaces */
#include "zos_string.h"              /* zos string interfaces */
#include "zos_heap.h"                /* zos heap memory interfaces */
#include "zos_pool.h"                /* zos memory pool interfaces */
#include "zos_bpool.h"               /* zos bitmap memory pool interfaces */
#include "zos_mem.h"                 /* zos memory management interfaces */
#include "zos_pmem.h"                /* zos power memory interfaces */
#include "zos_inet.h"                /* zos internet interfaces */
#include "zos_socket.h"              /* zos socket interfaces */
#include "zos_time.h"                /* zos time interfaces */
#include "zos_queue.h"               /* zos queue interfaces */
#include "zos_pqueue.h"              /* zos priority queue interfaces */
#include "zos_timer.h"               /* zos timer interfaces */
#include "zos_msg.h"                 /* zos message interfaces */
#include "zos_task.h"                /* zos task interfaces */
#include "zos_module.h"              /* zos module interfaces */
#include "zos_fsm.h"                 /* zos finite state machine interfaces */
#include "zos_fsm_dump.h"            /* zos fsm dump interfaces */
#include "zos_brick.h"               /* zos brick memory interfaces */
#include "zos_dbkt.h"                /* zos dynamic bucket interfaces */
#include "zos_omap.h"                /* zos object map interfaces */
#include "zos_ovec.h"                /* zos object vector interfaces */
#include "zos_rbt.h"                 /* zos red black tree interfaces */
#include "zos_file.h"                /* zos file typedefs and interfaces */
#include "zos_osdep.h"               /* zos os dependent interface */
#include "zos_evnt.h"                /* zos event interface */
#include "zos_trace.h"               /* zos trace interface */
#include "zos_gab.h"                 /* zos memory garbage interface */
#include "zos_tls.h"                 /* zos tls interface */
#include "zos_http.h"                /* zos http interface */
#include "zos_ims.h"                 /* zos ims interface */

#ifdef __cplusplus
extern "C" {   
#endif

/**
 * @brief Initialize Zos system evnironment.
 *
 * @retval ZOK Zos initialized OK.
 * @retval ZFAILED Zos initialized failed.
 *
 * This function initialize Zos environment, allocate and create basic
 * data structure according to the setting. User can access these setting
 * by @ref zos_cfg.h "Zos Configuration".
 *
 * @see @ref Zos_SysDestroy
 */
ZFUNC ZINT Zos_SysInit(ZFUNC_VOID);

ZFUNC ZINT Zos_SysInitX(ZFUNC_VOID);
/**
 * @brief Destroy Zos system evnironment.
 *
 * @return [none].
 *
 * @see @ref Zos_SysInit
 */
ZFUNC ZVOID Zos_SysDestroy(ZFUNC_VOID);

/**
 * @brief init license.
 *
 * @return ZOK OR ZFAILED.
 *
 * @see @ref Zos_SysInit
 */
ZFUNC ZINT Zos_LcsInit();

/**
 * @brief Destroy license.
 *
 * @return [none].
 *
 * @see @ref Zos_SysInit
 */
ZFUNC ZVOID Zos_LcsDestroy();

/**
 * @brief reload zos license.
 *
 * @return [none].
 *
 * @see @ref Zos_LcsInit
 */
ZFUNC ZINT Zos_LcsReload();

/**
 * @brief judge license is actived.
 *
 * @return [none].
 *
 * @see @ref Zos_LcsInit
 */
ZFUNC ZBOOL Zos_LcsIsActived();

/**
 * @brief Zos sys init print.
 *
 * @return [none].
 *
 */
ZFUNC ZVOID Zos_SysInitPrint(ZCHAR *pcStr);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_H__ */

