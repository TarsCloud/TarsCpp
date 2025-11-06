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
  File name     : zplin_osdep_prv.h
  Module        : linux os dependence functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by os dependency 
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPLIN_OSDEP_PRV_H__
#define _ZPLIN_OSDEP_PRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zplin_file.h"              /* linux file interfaces */
#include "zplin_mutex.h"             /* linux mutex interfaces */
#include "zplin_net.h"               /* linux net interfaces */
#include "zplin_task.h"              /* linux task interfaces */
#include "zplin_timer.h"             /* linux timer interfaces */

/* linux print string */
ZFUNC ZINT Zplin_Print(ZCHAR *pcStr);

/* linux time initialize */
ZFUNC ZINT Zplin_TimeInit();

/* linux time destroy */
ZFUNC ZVOID Zplin_TimeDestroy();

/* linux get the current time of the clock */
ZFUNC ZINT Zplin_GetEpochTime(ST_ZOS_TIMESPEC *pstTime);

/* linux get a high resolution relative relative time stamp. 
 * This value will never go backwards and will not wrap until about 68 years 
 * from power-up (seconds will wrap) */
ZFUNC ZINT Zplin_GetHiresTime(ST_ZOS_TIMESPEC *pstTime);

/* linux get a high resolution relative time stamp. 
 * This value will never go backwards. This value will is in 
 * nanoseconds and will never wrap (at least for about 290 years). */
ZFUNC ZHRTIME_T Zplin_GetHrTime(ZFUNC_VOID);

/* linux get the current time as a value of type time_t */
ZFUNC ZTIME_T Zplin_Time(ZTIME_T *pzTime);

/* linux convert calendar time into UTC broken-down time (ANSI) */
ZFUNC ST_ZOS_TM * Zplin_GmTime(ZTIME_T *pzTime);

/* linux convert calendar time into broken-down time */
ZFUNC ST_ZOS_TM * Zplin_LocalTime(ZTIME_T *pzTime);

/* linux convert broken-down time into calendar time */    
ZFUNC ZTIME_T Zplin_MkTime(ST_ZOS_TM *pstTime);

/* linux get time zone */    
ZFUNC ZINT Zplin_TimeZone(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPLIN_OSDEP_PRV_H__ */

