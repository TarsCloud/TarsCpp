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
  File name     : zos_osdep.h
  Module        : zos os dependence library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-08-10    
  Description   :
      included files required by os dependence
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_OSDEP_H__
#define _ZOS_OSDEP_H__

/** 
 * @file zos_osdep.h
 * @brief Included files required by Zos depending on operating system.
 */
 
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Zos ios sock aciont */
typedef ZINT (*PFN_SOCKETONCLOSE)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONCONNOK)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONCONNFAIL)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONDISCED)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONACCEPTABLE)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONACCEPTED)(ZSOCKET zSock, 
                 ZSOCKET zNewSock, ST_ZOS_INET_ADDR *pstRmtAddr);
typedef ZINT (*PFN_SOCKETONSENDOK)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONREADABLE)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONWRITABLE)(ZSOCKET zSock);
typedef ZINT (*PFN_SOCKETONRECVDATA)(ZSOCKET zSock, 
                ZUCHAR *pucData, ZUINT iLen, ST_ZOS_INET_ADDR *pstRmtAddr);

/** @brief ZOS os trace code */
#define ZOS_TRACE_CODE(_desc) Zos_OsdepTrace(_desc, __FILE__, __LINE__)

/**
 * @brief Trace description in file with line no using OS trace function.
 *
 * @param [in] pcDecs Trace description.
 * @param [in] pcFile The file name.
 * @param [in] iLineNo The file no.
 *
 * @return ZOK Trace description successfully, otherwise ZFAILED.
 */
ZINT Zos_OsdepTrace(ZCHAR *pcDecs, ZCHAR *pcFile, ZINT iLineNo);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_OSDEP_H__ */

