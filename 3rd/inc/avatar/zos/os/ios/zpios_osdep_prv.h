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
  File name     : zpios_osdep_prv.h
  Module        : ios os dependence functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPIOS_OSDEP_PRV_H__
#define _ZPIOS_OSDEP_PRV_H__

#include "zpios_osdep.h"             /* os dependency */
#ifdef ZOS_SUPT_NS_FILE
#include "zpios_file_ns.h"           /* ios file interfaces */
#else
#include "zpios_file.h"              /* ios file interfaces */
#endif

#include "zpios_mutex.h"             /* ios mutex interfaces */
#include "zpios_net.h"               /* ios net interfaces */
#include "zpios_net_ns.h"            /* ios nsstream interfaces */
#include "zpios_task.h"              /* ios task interfaces */
#include "zpios_timer.h"             /* ios timer interfaces */
#include "zpios_sys.h"               /* ios system interfaces */

#ifdef __cplusplus
extern "C" {
#endif

/* zos attach ios porting environment. */
ZINT Zpios_OsenvAttach(ZINT iSize, ZVOID **ppMem);

/* ios detach ios porting environment. */
ZVOID Zpios_OsenvDetach(ZFUNC_VOID);

/* ios locate ios porting environment. */
ZINT Zpios_OsenvLocate(ZVOID **ppMem);

/* ios deliver message */
ZINT Zpios_OsenvDeliverMsg(ZFUNC_VOID);

/* ios attach ios porting dependency functions. */
ZINT Zpios_OsdepAttach(ST_ZOS_OSDEP_MGR *pstMgr);

/* ios allocate ios memory. */
ZVOID * Zpios_Malloc(ZSIZE_T zSize);

/* iphone reallocate a memory */
ZVOID * Zpios_Realloc(ZVOID *pMem, ZSIZE_T zSize);

/* ios free memory allocated. */
ZVOID Zpios_Free(ZVOID *pMem);

/* ios trace source code with file and line no using print. */
ZINT Zpios_Trace(ZCHAR *pcDecs, ZCHAR *pcFile, ZINT iLineNo);

/* ios print string */
ZINT Zpios_Print(ZCHAR *pcStr);

/* ios time initialize */
ZINT Zpios_TimeInit();

/* ios time destroy */
ZVOID Zpios_TimeDestroy();

/* ios get the current time of the clock */
ZINT Zpios_GetEpochTime(ST_ZOS_TIMESPEC *pstTime);

/* ios get a high resolution relative relative time stamp. 
 * This value will never go backwards and will not wrap until about 68 years 
 * from power-up (seconds will wrap) */
ZINT Zpios_GetHiresTime(ST_ZOS_TIMESPEC *pstTime);

/* ios get a high resolution relative time stamp. 
 * This value will never go backwards. This value will is in 
 * nanoseconds and will never wrap (at least for about 290 years). */
ZHRTIME_T Zpios_GetHrTime(ZFUNC_VOID);

/* ios get the current time as a value of type time_t */
ZTIME_T Zpios_Time(ZTIME_T *pzTime);

/* ios convert calendar time into UTC broken-down time (ANSI) */
ST_ZOS_TM * Zpios_GmTime(ZTIME_T *pzTime);

/* ios convert calendar time into broken-down time */
ST_ZOS_TM * Zpios_LocalTime(ZTIME_T *pzTime);

/* ios convert broken-down time into calendar time */    
ZTIME_T Zpios_MkTime(ST_ZOS_TM *pstTime);

/* ios get time zone */    
ZINT Zpios_TimeZone(ZFUNC_VOID);

/* ios config get socket on tcp connect ok function */
ZFUNCPTR Zpios_CfgGetSocketOnConnOk(ZFUNC_VOID);

/* ios config get socket on tcp disconnected function */
ZFUNCPTR Zpios_CfgGetSocketOnDisced(ZFUNC_VOID);

/* ios config get socket on tcp accept */
ZFUNCPTR Zpios_CfgGetSocketOnAccepted(ZFUNC_VOID);

/* ios config get socket on readable function */
ZFUNCPTR Zpios_CfgGetSocketOnReadable(ZFUNC_VOID);

/* ios config get socket on receive data function */
ZFUNCPTR Zpios_CfgGetSocketOnWritable(ZFUNC_VOID);

/* ios config get socket on receive data function */
ZFUNCPTR Zpios_CfgGetSocketOnRecvData(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPIOS_OSDEP_PRV_H__ */

