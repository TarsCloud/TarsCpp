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
  File name     : zpand_osdep_prv.h
  Module        : android os dependence functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPAND_OSDEP_PRV_H__
#define _ZPAND_OSDEP_PRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zpand_file.h"              /* android file interfaces */
#include "zpand_mutex.h"             /* android mutex interfaces */
#include "zpand_net.h"               /* android net interfaces */
#include "zpand_task.h"              /* android task interfaces */
#include "zpand_timer.h"             /* android timer interfaces */
#include "zpand_http.h"              /* android http interfaces */
#include "zpand_osdep.h"             /* android os dependence */
#include "zpand_ims.h"               /* android ims interfaces */
#include "zpand_sys.h"

/* android module get post drive notification */
ZFUNC PFN_ZPANDMODPOSTDRV Zpand_ModGetPostDrv();

/* android get timer init */
ZFUNC PFN_ZPANDTIMER Zpand_TimerGetInit();

/* android get timer destroy */
ZFUNC PFN_ZPANDTIMER Zpand_TimerGetDestroy();

/* android get start a timer, time length use millisecond unit */
ZFUNC PFN_ZPANDTIMERX Zpand_TimerGetStart();

/* android get stop a timer */
ZFUNC PFN_ZPANDTIMER Zpand_TimerGetStop();

/* zos android attach android porting environment. */
ZFUNC ZINT Zpand_OsenvAttach(ZINT iSize, ZVOID **ppMem);

/* zos android Detach android porting environment. */
ZFUNC ZVOID Zpand_OsenvDetach(ZFUNC_VOID);

/* zos android Locate android porting environment. */
ZFUNC ZINT Zpand_OsenvLocate(ZVOID **ppMem);

/* zos android attach android porting dependency functions. */
ZFUNC ZINT Zpand_OsdepAttach(ST_ZOS_OSDEP_MGR *pstMgr);

/* zos android allocate android memory. */
ZFUNC ZVOID * Zpand_Malloc(ZSIZE_T zSize);

/* android reallocate a memory */
ZFUNC ZVOID * Zpand_Realloc(ZVOID *pMem, ZSIZE_T zSize);

/* zos android free memory allocated. */
ZFUNC ZVOID Zpand_Free(ZVOID *pMem);

/* zos android trace source code with file and line no using Windows function. */
ZFUNC ZINT Zpand_Trace(ZCHAR *pcDecs, ZCHAR *pcFile, ZINT iLineNo);

/* android print string */
ZFUNC ZINT Zpand_Print(ZCHAR *pcStr);

/* android time initialize */
ZFUNC ZINT Zpand_TimeInit();

/* android time destroy */
ZFUNC ZVOID Zpand_TimeDestroy();

/* android get the current time of the clock */
ZFUNC ZINT Zpand_GetEpochTime(ST_ZOS_TIMESPEC *pstTime);

/* android get a high resolution relative relative time stamp. 
 * This value will never go backwards and will not wrap until about 68 years 
 * from power-up (seconds will wrap) */
ZFUNC ZINT Zpand_GetHiresTime(ST_ZOS_TIMESPEC *pstTime);

/* android get a high resolution relative time stamp. 
 * This value will never go backwards. This value will is in 
 * nanoseconds and will never wrap (at least for about 290 years). */
ZFUNC ZHRTIME_T Zpand_GetHrTime(ZFUNC_VOID);

/* android get the current time as a value of type time_t */
ZFUNC ZTIME_T Zpand_Time(ZTIME_T *pzTime);

/* android convert calendar time into broken-down time */
ZFUNC ST_ZOS_TM * Zpand_LocalTime(ZTIME_T *pzTime);

/* android convert broken-down time into calendar time */    
ZFUNC ZTIME_T Zpand_MkTime(ST_ZOS_TM *pstTime);

/* android get time zone */    
ZFUNC ZINT Zpand_TimeZone(ZFUNC_VOID);

/* android config get socket on tcp connect ok function */
ZFUNC ZFUNCPTR Zpand_CfgGetSocketOnConnOk(ZFUNC_VOID);

/* android config get socket on tcp disconnected function */
ZFUNC ZFUNCPTR Zpand_CfgGetSocketOnDisced(ZFUNC_VOID);

/* android config get socket on tcp accepted */
ZFUNC ZFUNCPTR Zpand_CfgGetSocketOnAccepted(ZFUNC_VOID);

/* android config get socket on readable function */
ZFUNC ZFUNCPTR Zpand_CfgGetSocketOnReadable(ZFUNC_VOID);

/* android config get socket on receive data function */
ZFUNC ZFUNCPTR Zpand_CfgGetSocketOnWritable(ZFUNC_VOID);

/* android config get socket on receive data function */
ZFUNC ZFUNCPTR Zpand_CfgGetSocketOnRecvData(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPAND_OSDEP_PRV_H__ */

