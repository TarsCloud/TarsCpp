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
  File name     : zposx_timer.h
  Module        : mac osx os timer library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-02-28    
  Description   :
      Data structure and function definitions required by the timer interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPOSX_TIMER_H__
#define _ZPOSX_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* mac osx timer init */
ZFUNC ZINT Zposx_TimerInit(ZFUNC_VOID);

/* mac osx timer destroy */
ZFUNC ZINT Zposx_TimerDestroy(ZFUNC_VOID);

/* osx start a timer, time length use millisecond unit */
ZFUNC ZINT Zposx_TimerStart(ZUINT iTimeLen, 
                PFN_ZOSTIMERACTIVE pfnActive);

/* mac osx stop a timer */
ZFUNC ZINT Zposx_TimerStop(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZPOSX_TIMER_H__ */

