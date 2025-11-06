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
  File name     : zos_dump.h
  Module        : zos crash dump library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the 
      crash dump interface

  Modify History:   
  1. Date:        Author:         Modification:
  2005-02-03      leo.lv          create the file 

*************************************************/
#ifndef _ZOS_DUMP_H__
#define _ZOS_DUMP_H__

/** 
 * @file zos_dump.h
 * @brief Stack dump interfaces.
 *  
 * In this file, it defined the operation function
 * of zos dump stack, including create, delete, clear, print and etc.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function creates stack dump.
 *
 * @param [in] pcName The dump name.
 * @param [in] wDeepSize The stack dump deep size.
 *
 * @return A pointer to the dump, or return ZNULL.
 *
 * @see @ref Zos_DumpDelete
 */
ZFUNC ZDUMP Zos_DumpCreate(ZCHAR *pcName, ZUSHORT wDeepSize);

/**
 * @brief This function delete stack dump.
 *
 * @param [in] zDump A stack dump identifier.
 *
 * @return[none].
 *
 * @see @ref Zos_DumpCreate
 */
ZFUNC ZVOID Zos_DumpDelete(ZDUMP zDump);

/**
 * @brief This function pushs user information into stack dump.
 *
 * @param [in] zDump A stack dump identifier.
 * @param [in] pcFileName The file name.
 * @param [in] iFileLine The file line.
 * @param [in] pcStackDesc The stack description.
 * @param [in] iStackId The stack ID.
 *
 * @retval ZOK Push operation successfully.
 * @retval Push Clear operation failed.
 *
 * @see @ref Zos_DumpClear
 */
ZFUNC ZINT Zos_DumpStack(ZDUMP zDump, ZCHAR *pcFileName, 
                ZUINT iFileLine, ZCHAR *pcStackDesc, ZUINT iStackId);

/**
 * @brief This function clears stack dump.
 *
 * @param [in] zDump A stack dump identifier.
 *
 * @retval ZOK Clear operation successfully.
 * @retval ZFAILED Clear operation failed.
 *
 * @see @ref Zos_DumpStack
 */
ZFUNC ZINT Zos_DumpClear(ZDUMP zDump);

/**
 * @brief This function prints stack dump information.
 *
 * @param [in] zDump A stack dump identifier.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see @ref Zos_DumpPrintBuf
 */
ZFUNC ZINT Zos_DumpPrint(ZDUMP zDump);

/**
 * @brief This function prints stack dump information into buffer.
 *
 * @param [in] zDump A stack dump identifier.
 * @param [in] zData The print buffer data.
 *
 * @retval ZOK Print into buffer operation successfully.
 * @retval ZFAILED Print into buffer operation failed.
 *
 * @see @ref Zos_DumpPrint
 */
ZFUNC ZINT Zos_DumpPrintBuf(ZDUMP zDump, ZDBUF zData);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_DUMP_H__ */

