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
  File name     : zos_trace.h
  Module        : zos trace functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2011-09-23    
  Description   :
      Data structure and function definitions required by the trace interface
      
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_TRACE_H__
#define _ZOS_TRACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Log a trace information.
 *
 * @param [in] pcDesc The trace description.
 * @param [in] pcFileName The file name.
 * @param [in] iFileLine The file line.
 */
ZFUNC ZVOID Zos_TraceLog(ZCHAR *pcDesc, ZCHAR *pcFileName, 
                ZUINT iFileLine);

/**
 * @brief Add event information.
 *
 * @param [in] pcName The event name.
 * @param [in] pcLabel The event label.
 * @param [in] iValue The event value.
 */
ZFUNC ZVOID Zos_TraceEvnt(ZCONST ZCHAR *pcName, ZCONST ZCHAR *pcLabel,
                ZINT iValue);

/**
 * @brief Add event information.
 *
 * @param [in] pcName The event name.
 * @param [in] pcLabel The event label.
 * @param [in] iValue The event value.
 * @param [in] pcAttrs The event attributes in JSON format.
 */
ZFUNC ZVOID Zos_TraceEvntX(ZCONST ZCHAR *pcName, ZCONST ZCHAR *pcLabel,
                ZINT iValue, ZCONST ZCHAR *pcAttrs);

/**
 * @brief Add a memory into trace.
 *
 * @param [in] pMem The object memory.
 * @param [in] pcMemDesc The object ID.
 * @param [in] pcFileName The file name.
 * @param [in] iFileLine The file line.
 *
 * @retval ZOK Add operation successfully.
 * @retval Push Add operation failed.
 *
 * @see @ref Zos_TraceMemRmv
 */
ZFUNC ZINT Zos_TraceMemAdd(ZVOID *pMem, ZCHAR *pcMemDesc, 
                ZCHAR *pcFileName, ZUINT iFileLine);

/**
 * @brief Remove a memory from trace.
 *
 * @param [in] pMem The object memory.
 *
 * @retval ZOK Remove operation successfully.
 * @retval Push Remove operation failed.
 *
 * @see @ref Zos_TraceMemAdd
 */
ZFUNC ZINT Zos_TraceMemRmv(ZVOID *pMem);

/**
 * @brief Cleaer all memoriy trace information.
 *
 * @retval ZOK Clear operation successfully.
 * @retval ZFAILED Clear operation failed.
 *
 * @see @ref Zos_TraceStack
 */
ZFUNC ZINT Zos_TraceMemClr(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_TRACE_H__ */

