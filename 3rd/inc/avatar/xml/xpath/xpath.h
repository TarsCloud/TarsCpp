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
  File name     : xpath.h
  Module        : xpath
  Author        : Bob.Liu
  Version       : 0.1
  Created on    : 2007-12-11
  Description   :
    Marcos and structure definitions required by the xpath.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XPATH_H__
#define _XPATH_H__

#include "zos.h"                     /* zos system services interface */
#include "abnf.h"                    /* abnf typedefs and interfaces */
#include "xml.h"                     /* xml typedefs and interfaces */
#include "eax.h"                     /* eax interface */
#include "xpath_ui.h"                /* xpath utility interface */

#ifdef __cplusplus
extern "C"{
#endif

/* xpath init */
ZFUNC ZINT Xpath_Init(ZFUNC_VOID);

/* xpath map destroy */
ZFUNC ZVOID Xpath_Destroy(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _XPATH_H__ */

