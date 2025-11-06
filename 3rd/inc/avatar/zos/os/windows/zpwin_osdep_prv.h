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
  File name     : zpwin_osdep_prv.h
  Module        : windows os dependency functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPWIN_OSDEP_PRV_H__
#define _ZPWIN_OSDEP_PRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zpwin_file.h"              /* windows file interfaces */
#include "zpwin_mutex.h"             /* windows mutex interfaces */
#include "zpwin_net.h"               /* windows net interfaces */
#include "zpwin_task.h"              /* windows task interfaces */
#include "zpwin_timer.h"             /* windows timer interfaces */
#include "zpwin_sys.h"               /* windows system interfaces */

/* zos windows attach windows porting environment. */
ZFUNC ZINT Zpwin_OsenvAttach(ZUINT iSize, ZVOID **ppMem);

/* zos windows detach windows porting environment. */
ZFUNC ZVOID Zpwin_OsenvDetach(ZFUNC_VOID);

/* zos windows locate windows porting environment. */
ZFUNC ZINT Zpwin_OsenvLocate(ZVOID **ppMem);

/* zos windows attach windows porting dependency functions. */
ZFUNC ZINT Zpwin_OsdepAttach(ST_ZOS_OSDEP_MGR *pstMgr);

/* windows print string */
ZFUNC ZINT Zpwin_Print(ZCHAR *pcStr);

/* windows time initialize */
ZFUNC ZINT Zpwin_TimeInit(ZFUNC_VOID);

/* windows get the current time of the clock */
ZFUNC ZINT Zpwin_GetEpochTime(ST_ZOS_TIMESPEC *pstTime);

/* windows get a high resolution relative relative time stamp. 
 * This value will never go backwards and will not wrap until about 68 years 
 * from power-up (seconds will wrap) */
ZFUNC ZINT Zpwin_GetHiresTime(ST_ZOS_TIMESPEC *pstTime);

/* windows get a high resolution relative time stamp. 
 * This value will never go backwards. This value will is in 
 * nanoseconds and will never wrap (at least for about 290 years). */
ZFUNC ZHRTIME_T Zpwin_GetHrTime(ZFUNC_VOID);

/* windows get the current time as a value of type time_t */
ZFUNC ZTIME_T Zpwin_Time(ZTIME_T *pzTime);

/* windows convert calendar time into broken-down time */
ZFUNC ST_ZOS_TM * Zpwin_LocalTime(ZTIME_T *pzTime);

/* windows convert broken-down time into calendar time */    
ZFUNC ZTIME_T Zpwin_MkTime(ST_ZOS_TM *pstTime);

/* windows get time zone */    
ZFUNC ZINT Zpwin_TimeZone(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPWIN_OSDEP_PRV_H__ */

