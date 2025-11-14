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
  File name     : zpand_sys.h
  Module        : android system library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-01-01    
  Description   :
   Function implement required by the task interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPAND_SYS_H__
#define _ZPAND_SYS_H__

#include <android/looper.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Android process evnt init */
ZVOID Zpand_UiEvntInit();

/* Android process event on ui thread */
ZINT Zpand_UiProcEvnt(ZEVNT zEvntId, PFN_ZUIMSGPROC pfnProc);

/* Android get ui thread id */
ALooper * Zpand_UiThreadId();

#ifdef __cplusplus
}
#endif

#endif /* _ZPAND_SYS_H__ */

