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
  File name     : zcrc.h
  Module        : crc utilitis
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2007-04-25    
  Description   :
      
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZCRC_H__
#define _ZCRC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* initial FCS value */
#define ZCRC_INIT_FCS16 0xffff

/* calculate a new fcs given the current fcs and the new data. */
ZFUNC ZUSHORT Zcrc_Fcs16(ZUSHORT wFcs, ZUCHAR *pucStr, ZINT iLen);

/* calculate crc-32 */
ZFUNC ZUINT Zcrc_Calc32(ZUCHAR *pucData, ZUINT iLen);

#ifdef __cplusplus
}
#endif

#endif /* _ZCRC_H__ */

