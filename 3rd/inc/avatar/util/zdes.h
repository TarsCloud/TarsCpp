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
  File name     : zdes.h
  Module        : des functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2010-10-05    
  Description   :
      Data structure and function definitions required by the des interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZDES_H__
#define _ZDES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* des encrypt data */
ZFUNC ZINT Zdes_EncData(ZINT aiKey[2], ZUCHAR *pucData, ZINT iSize, 
                ZUCHAR *pucCData, ZINT *piCSize);

/* des decrypt data */
ZFUNC ZINT Zdes_DecData(ZINT aiKey[2], ZUCHAR *pucCData, ZINT iCSize, 
                ZUCHAR *pucData, ZINT *piSize);

#ifdef __cplusplus
}
#endif

#endif /* _ZDES_H__ */

