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
  File name     : zposx_osdep_prv.h
  Module        : mac osx os dependence functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPOSX_OSDEP_PRV_H__
#define _ZPOSX_OSDEP_PRV_H__

#include "zposx_file.h"              /* osx file interfaces */
#include "zposx_mutex.h"             /* osx mutex interfaces */
#include "zposx_net.h"               /* osx net interfaces */
#include "zposx_task.h"              /* osx task interfaces */
#include "zposx_timer.h"             /* osx timer interfaces */
#include "zposx_sys.h"               /* osx system interfaces */
#include "zposx_http.h"

#ifdef __cplusplus
extern "C" {
#endif

/* zos attach osx porting environment. */
ZINT Zposx_OsenvAttach(ZINT iSize, ZVOID **ppMem);

/* osx detach osx porting environment. */
ZVOID Zposx_OsenvDetach(ZFUNC_VOID);

/* osx locate osx porting environment. */
ZINT Zposx_OsenvLocate(ZVOID **ppMem);

/* osx deliver message */
ZINT Zposx_OsenvDeliverMsg(ZFUNC_VOID);

/* osx attach osx porting dependency functions. */
ZINT Zposx_OsdepAttach(ST_ZOS_OSDEP_MGR *pstMgr);

/* osx allocate osx memory. */
ZVOID * Zposx_Malloc(ZSIZE_T zSize);

/* iphone reallocate a memory */
ZVOID * Zposx_Realloc(ZVOID *pMem, ZSIZE_T zSize);

/* osx free memory allocated. */
ZVOID Zposx_Free(ZVOID *pMem);

/* osx trace source code with file and line no using print. */
ZINT Zposx_Trace(ZCONST ZCHAR *pcDecs, ZCONST ZCHAR *pcFile, ZINT iLineNo);

/* osx print string */
ZINT Zposx_Print(ZCONST ZCHAR *pcStr);

/* osx time initialize */
ZINT Zposx_TimeInit();

/* osx time destroy */
ZVOID Zposx_TimeDestroy();

/* osx get the current time of the clock */
ZINT Zposx_GetEpochTime(ST_ZOS_TIMESPEC *pstTime);

/* osx get a high resolution relative relative time stamp. 
 * This value will never go backwards and will not wrap until about 68 years 
 * from power-up (seconds will wrap) */
ZINT Zposx_GetHiresTime(ST_ZOS_TIMESPEC *pstTime);

/* osx get a high resolution relative time stamp. 
 * This value will never go backwards. This value will is in 
 * nanoseconds and will never wrap (at least for about 290 years). */
ZHRTIME_T Zposx_GetHrTime(ZFUNC_VOID);

/* osx get the current time as a value of type time_t */
ZTIME_T Zposx_Time(ZTIME_T *pzTime);

/* osx convert calendar time into UTC broken-down time (ANSI) */
ST_ZOS_TM * Zposx_GmTime(ZTIME_T *pzTime);

/* osx convert calendar time into broken-down time */
ST_ZOS_TM * Zposx_LocalTime(ZTIME_T *pzTime);

/* osx convert broken-down time into calendar time */    
ZTIME_T Zposx_MkTime(ST_ZOS_TM *pstTime);

/* osx get time zone */    
ZINT Zposx_TimeZone(ZFUNC_VOID);

/* osx config get socket on tcp connect ok function */
ZFUNCPTR Zposx_CfgGetSocketOnConnOk(ZFUNC_VOID);

/* osx config get socket on tcp disconnected function */
ZFUNCPTR Zposx_CfgGetSocketOnDisced(ZFUNC_VOID);

/* osx config get socket on tcp accept */
ZFUNCPTR Zposx_CfgGetSocketOnAccepted(ZFUNC_VOID);

/* osx config get socket on readable function */
ZFUNCPTR Zposx_CfgGetSocketOnReadable(ZFUNC_VOID);

/* osx config get socket on receive data function */
ZFUNCPTR Zposx_CfgGetSocketOnWritable(ZFUNC_VOID);

/* osx config get socket on receive data function */
ZFUNCPTR Zposx_CfgGetSocketOnRecvData(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPOSX_OSDEP_PRV_H__ */

