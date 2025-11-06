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
  File name     : zos_cfg.h
  Module        : ZOS configuration
  Author        : leo.lv   
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Marcos definitions required by the zos configuration.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_CFG_H__
#define _ZOS_CFG_H__

/** 
 * @file zos_cfg.h
 * @brief Zos configruation functions.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* zos log file name length */
#define ZOS_FILE_NAME_LEN 512

/* zos aes key length */
#define ZOS_AES_KEY_LEN 16

/** @brief Zos print display function. */
typedef ZINT (*PFN_ZPRINTDISP)(ZCHAR *pcStr);

/** @brief Zos statistics function. */
typedef ZINT (*PFN_ZTRACEEVNT)(ZCONST ZCHAR *pcEvnt, ZCONST ZCHAR *pcLabel,
                ZINT iValue, ZCONST ZCHAR *pcAttrs);

/**
 * @brief Get license file name.
 *
 * @retval The license file name, default is "license.sign".
 *
 * @see Zos_CfgSetLcsFileName
 */
ZFUNC ZCONST ZCHAR * Zos_CfgGetLcsFileName(ZFUNC_VOID);

/**
 * @brief Get aes key.
 *
 * @retval The aes key, default is "0123456789abcdef".
 *
 * @see Zos_CfgSetAesKey
 */
ZFUNC ZCHAR * Zos_CfgGetAesKey(ZFUNC_VOID);

/**
 * @brief Get license context.
 *
 * @retval The license context, default is ZNULL.
 *
 * @see Zos_CfgSetAppCtx
 */
ZFUNC ZVOID * Zos_CfgGetAppCtx(ZFUNC_VOID);

/**
 * @brief Get print display callback.
 *
 * @retval Print display callback, default is ZNULL.
 *
 * @see Zos_CfgSetPrintDisp
 */
ZFUNC PFN_ZPRINTDISP Zos_CfgGetPrintDisp(ZFUNC_VOID);

/**
 * @brief Get trace statistics callback.
 *
 * @retval Trace statistics callback, default is ZNULL.
 *
 * @see Zos_CfgSetTraceEvnt
 */
ZFUNC PFN_ZTRACEEVNT Zos_CfgGetTraceEvnt(ZFUNC_VOID);

/**
 * @brief Get log print display callback.
 *
 * @retval Log print display callback, default is ZNULL.
 *
 * @see Zos_CfgSetLogDisp
 */
ZFUNC PFN_ZPRINTDISP Zos_CfgGetLogDisp(ZFUNC_VOID);

/**
 * @brief Get the log file name.
 *
 * @retval The log file name, default is "zos.log".
 *
 * @see Zos_CfgSetLogFile
 */
ZFUNC ZCHAR * Zos_CfgGetLogFile(ZFUNC_VOID);

/**
 * @brief Get log buffer size.
 *
 * @retval Log buffer size, default is 4k.
 *
 * @see Zos_CfgSetLogBufSize
 */
ZFUNC ZUINT Zos_CfgGetLogBufSize(ZFUNC_VOID);

/**
 * @brief Get log buffer alert size.
 *
 * @retval Log buffer alert size, default is 4k.
 *
 * @see Zos_CfgSetLogAlertSize
 */
ZFUNC ZUINT Zos_CfgGetLogAlertSize(ZFUNC_VOID);

/**
 * @brief Get log file size.
 *
 * @retval Log file size, default is 100k.
 *
 * @see Zos_CfgSetLogFileSize
 */
ZFUNC ZUINT Zos_CfgGetLogFileSize(ZFUNC_VOID);

/**
 * @brief Get log file count.
 *
 * @retval Log file count, default is 10.
 *
 * @see Zos_CfgSetLogFileCount
 */
ZFUNC ZUINT Zos_CfgGetLogFileCount(ZFUNC_VOID);

/**
 * @brief Get log level.
 *
 * @retval Log level, default is FATAL | ERROR | INFO.
 *
 * @see Zos_CfgSetLogLevel
 */
ZFUNC ZUINT Zos_CfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief Get print log level.
 *
 * @retval Log level, default is FATAL | ERROR | INFO.
 *
 * @see Zos_CfgGetPrintLogLevel
 */
ZFUNC ZUINT Zos_CfgGetPrintLogLevel(ZFUNC_VOID);

/**
 * @brief Get log print status.
 *
 * @retval Log print status, default is ZTRUE.
 *
 * @see Zos_CfgSetLogPrint
 */
ZFUNC ZBOOL Zos_CfgGetLogPrint(ZFUNC_VOID);

/**
 * @brief Get log date(year, month, day) option support status.
 *
 * @retval Log date option in log string header, default is ZFALSE.
 *
 * @see Zos_CfgSetLogDispDateSupt
 */
ZFUNC ZBOOL Zos_CfgGetLogDispDateSupt(ZFUNC_VOID);

/**
 * @brief Get log time(hour, min, sec) option support status.
 *
 * @retval Log time option in log string header, default is ZTRUE.
 *
 * @see Zos_CfgSetLogDispTimeSupt
 */
ZFUNC ZBOOL Zos_CfgGetLogDispTimeSupt(ZFUNC_VOID);

/**
 * @brief Get log time in milliseconds flag.
 *
 * @retval Log time in milliseconds option, default is ZFALSE.
 *
 * @see Zos_CfgSetLogDispTimeImMs
 */
ZFUNC ZBOOL Zos_CfgGetLogDispTimeInMs(ZFUNC_VOID);

/**
 * @brief Get log thread id display support flag.
 *
 * @retval Log thread id display option, default is ZFALSE.
 *
 * @see Zos_CfgSetLogDispTheadId
 */
ZFUNC ZBOOL Zos_CfgGetLogDispTheadId(ZFUNC_VOID);

/**
 * @brief Get log task support.
 *
 * @retval Log task support status, default is ZFALSE.
 *
 * @see Zos_CfgSetLogTaskSupt
 */
ZFUNC ZBOOL Zos_CfgGetLogTaskSupt(ZFUNC_VOID);

/**
 * @brief Get task priority.
 *
 * @retval Task priority, default is ZTASK_PRIORITY_MIN.
 *
 * @see Zos_CfgSetLogTaskPriority
 */
ZFUNC ZINT Zos_CfgGetLogTaskPriority(ZFUNC_VOID);

/**
 * @brief Get log task timer cycel time length.
 *
 * @retval Log task timer cycel time length, 
 * default is 60000 milliseconds(1 minute).
 *
 * @see Zos_CfgSetLogTaskCycleTimeLen
 */
ZFUNC ZUINT Zos_CfgGetLogTaskCycleTimeLen(ZFUNC_VOID);

/**
 * @brief Get log task timer alert time length.
 *
 * @retval Log task timer alert time length, 
 * default is 500 milliseconds(1 minute).
 *
 * @see Zos_CfgSetLogTaskAlertTimeLen
 */
ZFUNC ZUINT Zos_CfgGetLogTaskAlertTimeLen(ZFUNC_VOID);

/**
 * @brief Get module driver mode.
 *
 * @retval ZTRUE if module task is a thread, otherwise return ZFALSE.
 * default is ZTRUE.
 *
 * @note It it is ZFALSE, User should call Zos_ModDriveMsg explictly.
 *
 * @see Zos_CfgSetModDrvMode
 */
ZFUNC ZBOOL Zos_CfgGetModDrvMode(ZFUNC_VOID);

/**
 * @brief Get task count.
 *
 * @retval Task count, default is 20.
 *
 * @see Zos_CfgSetTaskCount
 */
ZFUNC ZUINT Zos_CfgGetTaskCount(ZFUNC_VOID);

/**
 * @brief Get task queue size.
 *
 * @retval Task queue size, default is 100.
 *
 * @see Zos_CfgSetQueueSize
 */
ZFUNC ZUINT Zos_CfgGetQueueSize(ZFUNC_VOID);

/**
 * @brief Get timer number.
 *
 * @retval Timer number, default is 500.
 *
 * @see Zos_CfgSetTimerCount
 */
ZFUNC ZUINT Zos_CfgGetTimerCount(ZFUNC_VOID);

/**
 * @brief Get timer driver mode.
 *
 * @retval ZTRUE if timer task is a thread, otherwise return ZFALSE.
 * default is ZTRUE.
 *
 * @note It it is ZFALSE, user should set self timer drive functions
 * in os dependency functions list(ST_ZOS_OSDEP_MGR) pfnTimerInit, 
 * pfnTimerDestroy, pfnTimerStart, pfnTimerStop.
 *
 * @see Zos_CfgSetTimerDrvMode
 */
ZFUNC ZBOOL Zos_CfgGetTimerDrvMode(ZFUNC_VOID);

/**
 * @brief Get timer task priority priority.
 *
 * @retval Task priority, default is ZTASK_PRIORITY_NORMAL.
 *
 * @see Zos_CfgSetLogTaskPriority
 */
ZFUNC ZINT Zos_CfgGetTimerTaskPriority(ZFUNC_VOID);

/**
 * @brief Get os mutex support option.
 *
 * @retval Os mutex support option, default is ZTRUE.
 *
 * @note If it is ZFALSE, Zos_MutexCreate... will do nothing.
 *
 * @see Zos_CfgSetOsMutexSupt
 */
ZFUNC ZBOOL Zos_CfgGetOsMutexSupt(ZFUNC_VOID);

/**
 * @brief Get os shared-exclusive lock support option.
 *
 * @retval Os shared-exclusive lock support option, default is ZTRUE.
 *
 * @note If it is ZFALSE, Zos_SharexCreate... will do nothing.
 *
 * @see Zos_CfgSetOsSharexSupt
 */
ZFUNC ZBOOL Zos_CfgGetOsSharexSupt(ZFUNC_VOID);

/**
 * @brief Get os semaphore support option.
 *
 * @retval Os semaphore support option, default is ZTRUE.
 *
 * @note If it is ZFALSE, Zos_SemCreate... will do nothing.
 *
 * @see Zos_CfgSetOsMutexSupt
 */
ZFUNC ZBOOL Zos_CfgGetOsSemSupt(ZFUNC_VOID);

/**
 * @brief Get os thread support option.
 *
 * @retval Os thread support option, default is ZTRUE.
 *
 * @note If it is ZFALSE, Zos_TaskSpawn... will do nothing.
 *
 * @see Zos_CfgSetOsThreadSupt
 */
ZFUNC ZBOOL Zos_CfgGetOsThreadSupt(ZFUNC_VOID);

/**
 * @brief Get os reuse address option open when socket open.
 *
 * @retval Os reuse address option open, default is ZFALSE.
 *
 * @see Zos_CfgSetOsThreadSupt
 */
ZFUNC ZBOOL Zos_CfgGetOsSocketRO(ZFUNC_VOID);

/**
 * @brief Get os socket log INFO level enable.
 *
 * @retval Os socket log INFO level enable, default is ZFALSE.
 *
 * @see Zos_CfgSetOsSocketLogInfo
 */
ZFUNC ZBOOL Zos_CfgGetOsSocketLogInfo(ZFUNC_VOID);

/**
 * @brief Get if use OS malloc and free.
 *
 * @retval ZTRUE Use OS malloc and free.
 * @retval ZFALSE Use ZOS Pool to manange memory, this is default.
 *
 * @see Zos_CfgSetUseOsMalloc
 */
ZFUNC ZBOOL Zos_CfgGetUseOsMalloc(ZFUNC_VOID);

/**
 * @brief Get if use ipv4.
 *
 * @retval ZTRUE Use ipv4, this is default.
 * @retval ZFALSE Use ipv6,
 *
 * @see Zos_CfgSetUseIpv4
 */
ZFUNC ZBOOL Zos_CfgGetUseIpv4(ZFUNC_VOID);

/**
 * @brief Get memroy garbage memory delete time.
 *
 * @retval Memory delete time, default is 60s.
 *
 * @see Zos_CfgSetGabDelTime
 */
ZFUNC ZTIME_T Zos_CfgGetGabDelTime(ZFUNC_VOID);

/**
 * @brief Get file encryption.
 *
 * @retval file encryption.
 *
 * @see Zos_CfgSetFileEncrypt
 */
ZFUNC ZBOOL Zos_CfgGetFileEncrypt(ZFUNC_VOID);

/**
 * @brief This function sets license file name.
 *
 * @param [in] pcFileName License file name.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLcsFileName
 */
ZFUNC ZINT Zos_CfgSetLcsFileName(ZCONST ZCHAR *pcFileName);

/**
 * @brief This function sets aes key.
 *
 * @param [in] pcAesKey Aes key.
 *
 * @retval ZOK Set aes key successfully.
 * @retval ZFAILED Set aes key failed.
 *
 * @see Zos_CfgGetLcsFileName
 */
ZFUNC ZINT Zos_CfgSetAesKey(ZCONST ZCHAR *pcAesKey);

/**
 * @brief This function sets license context.
 *
 * @param [in] pCtx License context pointer.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetAppCtx
 */
ZFUNC ZINT Zos_CfgSetAppCtx(ZVOID *pCtx);

/**
 * @brief This function is zos system config, and sets print display.
 *
 * @param [in] pfnDisp Callback function pointer.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetPrintDisp
 */
ZFUNC ZINT Zos_CfgSetPrintDisp(PFN_ZPRINTDISP pfnDisp);

/**
 * @brief This function is zos system config, and sets trace statistics.
 *
 * @param [in] pfnTraceEvnt Callback function pointer.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetTraceEvnt
 */
ZFUNC ZINT Zos_CfgSetTraceEvnt(PFN_ZTRACEEVNT pfnTraceEvnt);

/**
 * @brief This function is zos system config, and sets log display.
 *
 * @param [in] pfnDisp Callback function.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogDisp
 */
ZFUNC ZINT Zos_CfgSetLogDisp(PFN_ZPRINTDISP pfnDisp);

/**
 * @brief This function is zos system config, and sets log file.
 *
 * @param [in] pcFileName log file name.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogFile
 */
ZFUNC ZINT Zos_CfgSetLogFile(ZCHAR *pcFileName);

/**
 * @brief This function is zos system config, and sets log file directory.
 *
 * @param [in] pcDir log file directory.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogDir
 */
ZFUNC ZINT Zos_CfgSetLogDir(ZCHAR *pcDir);

/**
 * @brief Get the log dir.
 *
 * @retval The log dir.
 *
 * @see Zos_CfgSetLogDir
 */
ZFUNC ZCONST ZCHAR * Zos_CfgGetLogDir();

/**
 * @brief This function is zos system config, and sets login log file.
 *
 * @param [in] pcFileName login log file name.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLoginLogFile
 */
ZFUNC ZINT Zos_CfgSetLoginLogFile(ZCHAR *pcFileName);

/**
 * @brief Get the login log file name.
 *
 * @retval The login log file name, default is "mtc_login.log".
 *
 * @see Zos_CfgSetLoginLogFile
 */
ZFUNC ZCONST ZCHAR * Zos_CfgGetLoginLogFile(ZFUNC_VOID);

/**
 * @brief This function is zos system config, and sets login log file directory.
 *
 * @param [in] pcDir login log file directory.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLoginLogDir
 */
ZFUNC ZINT Zos_CfgSetLoginLogDir(ZCHAR *pcDir);

/**
 * @brief Get the login log dir.
 *
 * @retval The login log dir.
 *
 * @see Zos_CfgSetLoginLogDir
 */
ZFUNC ZCONST ZCHAR * Zos_CfgGetLoginLogDir();

/**
 * @brief This function is zos system config, and sets log buffer size.
 *
 * @param [in] iSize log buffer size.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogBufSize
 */
ZFUNC ZINT Zos_CfgSetLogBufSize(ZUINT iSize);

/**
 * @brief This function is zos system config, and sets log buffer alert size.
 *
 * @param [in] iSize log buffer alert size.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogAlertSize
 */
ZFUNC ZINT Zos_CfgSetLogAlertSize(ZUINT iSize);

/**
 * @brief This function is zos system config, and sets log file size.
 *
 * @param [in] iSize log file size.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_CfgSetLogFileSize(ZUINT iSize);

/**
 * @brief This function is zos system config, and sets log file count.
 *
 * @param [in] iCount log file count.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_CfgSetLogFileCount(ZUINT iCount);

/**
 * @brief This function is zos system config, and sets log level.
 *
 * @param [in] iLevel log level.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogLevel
 */
ZFUNC ZINT Zos_CfgSetLogLevel(ZUINT iLevel);

/**
 * @brief This function is zos system config, and sets print log level.
 *
 * @param [in] iLevel log level.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgSetPrintLogLevel
 */
ZFUNC ZINT Zos_CfgSetPrintLogLevel(ZUINT iLevel);

/**
 * @brief This function is zos system config, and sets log print.
 *
 * @param [in] bPrint log print.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogPrint
 */
ZFUNC ZINT Zos_CfgSetLogPrint(ZBOOL bPrint);

/**
 * @brief This function is zos system config, and sets login log print.
 *
 * @param [in] bPrint log print.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLoginLogPrint
 */
ZFUNC ZINT Zos_CfgSetLoginLogPrint(ZBOOL bPrint);

/**
 * @brief Get login log print status.
 *
 * @retval Log print status, default is ZTRUE.
 *
 * @see Zos_CfgSetLoginLogPrint
 */
ZFUNC ZBOOL Zos_CfgGetLoginLogPrint(ZFUNC_VOID);

/**
 * @brief This function is zos system config,
 *        and sets log date(year, month, day) support.
 *
 * @param [in] bSupt log date.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogDispDateSupt
 */
ZFUNC ZINT Zos_CfgSetLogDispDateSupt(ZBOOL bSupt);

/**
 * @brief This function is zos system config,
 *        and sets log time(hour, min, sec) support.
 *
 * @param [in] bSupt log time.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogDispTimeSupt
 */
ZFUNC ZINT Zos_CfgSetLogDispTimeSupt(ZBOOL bSupt);

/**
 * @brief This function is zos system config,
 *        and sets log time in milliseconds.
 *
 * @param [in] bSupt If ZTRUE, log time in milliseconds or in seconds.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogDispTimeInMs
 */
ZFUNC ZINT Zos_CfgSetLogDispTimeInMs(ZBOOL bSupt);

/**
 * @brief This function is zos system config,
 *        and sets thead id display support.
 *
 * @param [in] bSupt If ZTRUE, thead id display support.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogDispTheadId
 */
ZFUNC ZINT Zos_CfgSetLogDispTheadId(ZBOOL bSupt);

/**
 * @brief This function is zos system config, and sets log task support.
 *
 * @param [in] bSupt Log task support flag.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogTaskSupt
 */
ZFUNC ZINT Zos_CfgSetLogTaskSupt(ZBOOL bSupt);

/**
 * @brief Set log task priority.
 *
 * @param [in] iPriority Log task priority.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogTaskPriority
 */
ZFUNC ZINT Zos_CfgSetLogTaskPriority(ZINT iPriority);

/**
 * @brief Set log task timer cycel time length(milliseconds).
 *
 * @param [in] iTimeLen Cycel time length.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogTaskCycleTimeLen
 */
ZFUNC ZINT Zos_CfgSetLogTaskCycleTimeLen(ZUINT iTimeLen);

/**
 * @brief Set log task timer alert time length(milliseconds).
 *
 * @param [in] iTimeLen Alert time length.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetLogTaskAlertTimeLen
 */
ZFUNC ZINT Zos_CfgSetLogTaskAlertTimeLen(ZUINT iTimeLen);

/**
 * @brief Set module task driver mode.
 *
 * @param [in] bSelfTask Modulue manager using self thread.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetModDrvMode
 */
ZFUNC ZINT Zos_CfgSetModDrvMode(ZBOOL bSelfTask);

/**
 * @brief Set module task count.
 *
 * @param [in] iTaskCount Modulue task count.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetTaskCount
 */
ZFUNC ZINT Zos_CfgSetTaskCount(ZUINT iTaskCount);

/**
 * @brief Set module queue size.
 *
 * @param [in] iQueueSize Modulue queue size.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetTaskCount
 */
ZFUNC ZINT Zos_CfgSetQueueSize(ZUINT iQueueSize);

/**
 * @brief Set ZOS timer count.
 *
 * @param [in] iTimerCount ZOS timer count.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetTimerCount
 */
ZFUNC ZINT Zos_CfgSetTimerCount(ZUINT iTimerCount);

/**
 * @brief Set ZOS timer manager driver mode.
 *
 * @param [in] bSelfTask ZOS timer manager using self thread.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetTimerDrvMode
 */
ZFUNC ZINT Zos_CfgSetTimerDrvMode(ZBOOL bSelfTask);

/**
 * @brief Set ZOS timer manager task priority.
 *
 * @param [in] iPriority ZOS timer manager task priority.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetTimerTaskPriority
 */
ZFUNC ZINT Zos_CfgSetTimerTaskPriority(ZINT iPriority);

/**
 * @brief Set ZOS support mutex functions.
 *
 * @param [in] bSupt ZOS support mutex functions.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetOsMutexSupt
 */
ZFUNC ZINT Zos_CfgSetOsMutexSupt(ZBOOL bSupt);

/**
 * @brief Set ZOS support shared-exclusive lock functions.
 *
 * @param [in] bSupt ZOS support shared-exclusive lock functions.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetOsSharexSupt
 */
ZFUNC ZINT Zos_CfgSetOsSharexSupt(ZBOOL bSupt);

/**
 * @brief Set ZOS support semaphore functions.
 *
 * @param [in] bSupt ZOS support semaphore functions.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetOsSemSupt
 */
ZFUNC ZINT Zos_CfgSetOsSemSupt(ZBOOL bSupt);

/**
 * @brief Set ZOS support thread functions.
 *
 * @param [in] bSupt ZOS support thread functions.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetOsThreadSupt
 */
ZFUNC ZINT Zos_CfgSetOsThreadSupt(ZBOOL bSupt);

/**
 * @brief Set ZOS reuse address option when open socket.
 *
 * @param [in] bSupt ZOS support reuse address when open socket.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetOsSocketRO
 */
ZFUNC ZINT Zos_CfgSetOsSocketRO(ZBOOL bSupt);

/**
 * @brief Set socket log INFO level enable.
 *
 * @param [in] bEnable ZOS support reuse address when open socket.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetOsSocketLogInfo
 */
ZFUNC ZINT Zos_CfgSetOsSocketLogInfo(ZBOOL bEnable);

/**
 * @brief Set if use OS malloc and free.
 *
 * @param [in] bEnable ZTRUE to use OS malloc and free, ZFALSE to use
 *                     ZOS pool management.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetOsSocketLogInfo
 */
ZFUNC ZINT Zos_CfgSetUseOsMalloc(ZBOOL bEnable);

/**
 * @brief Set if use ipv4.
 *
 * @param [in] bUseIpv4 ZTRUE to use ipv4, ZFALSE to use ipv6.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetUseIpv4
 */
ZFUNC ZINT Zos_CfgSetUseIpv4(ZBOOL bUseIpv4);

/**
 * @brief This function is zos system config, and sets file encryption.
 *
 * @param [in] bFileEncrypt file encryption.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_CfgSetFileEncrypt(ZBOOL bFileEncrypt);

/**
 * @brief Set memroy garbage memory delete time.
 *
 * @param [in] zDelTime Memory delete time.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetGabDelTime
 */
ZFUNC ZINT Zos_CfgSetGabDelTime(ZTIME_T zDelTime);

/**
  * @brief Set which license device type to check.
  *
  * @param [in] iType device ID type @ref EN_ZOS_DEVICE_ID_TYPE.
  *
  * @retval ZOK Set operation successfully.
  * @retval ZFAILED Set operation failed.
  *
  * @see Zos_CfgGetChkDevIdType
  */
ZFUNC ZINT Zos_CfgSetChkDevIdType(ZUINT iType);
     
/**
  * @brief Get which license device type to check.
  *
  * @retval device ID type @ref EN_ZOS_DEVICE_ID_TYPE.
  *
  * @see Zos_CfgSetChkDevIdType
  */
ZFUNC ZUINT Zos_CfgGetChkDevIdType(ZFUNC_VOID);

/**
  * @brief Set reg server type.
  *
  * @retval server type @ref EN_ZOS_REG_SRV_TYPE.
  *
  * @see Zos_CfgGetRegSrvType
  */
ZFUNC ZINT Zos_CfgSetRegSrvType(ZUINT iType);

/**
  * @brief Get reg server type.
  *
  * @retval reg server type @ref EN_ZOS_REG_SRV_TYPE.
  *
  * @see Zos_CfgSetRegSrvType
  */
ZFUNC ZUINT Zos_CfgGetRegSrvType(ZFUNC_VOID);

/**
 * @brief Get login log file size.
 *
 * @retval Login log file size, default is 100k.
 *
 * @see Zos_CfgSetLoginLogFileSize
 */
ZFUNC ZUINT Zos_CfgGetLoginLogFileSize(ZFUNC_VOID);

/**
 * @brief This function is zos system config, and sets login log file size.
 *
 * @param [in] iSize login log file size.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_CfgSetLoginLogFileSize(ZUINT iSize);

/**
 * @brief Get login log file count.
 *
 * @retval Login log file count, default is 10.
 *
 * @see Zos_CfgSetLoginLogFileCount
 */
ZFUNC ZUINT Zos_CfgGetLoginLogFileCount(ZFUNC_VOID);

/**
 * @brief This function is zos system config, and sets login log file count.
 *
 * @param [in] iCount login log file count.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_CfgSetLoginLogFileCount(ZUINT iCount);

/**
 * @brief Get support ims single login.
 *
 * @retval ims single login.
 */
ZFUNC ZBOOL Zos_CfgGetSuptImsSingleLogin(ZFUNC_VOID);

/**
 * @brief Set support ims single login.
 *
 * @param [in] bSuptImsSingleLogin Support ims single login.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 */
ZFUNC ZINT Zos_CfgSetSuptImsSingleLogin(ZBOOL bSuptImsSingleLogin);

/**
 * @brief Get ims single prot.
 *
 * @retval ims single prot.
 */
ZFUNC ZUINT Zos_CfgGetSinglePort(ZFUNC_VOID);

/**
 * @brief Set ims single port.
 *
 * @param [in] iSinglePort ims single prot.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 */
ZFUNC ZINT Zos_CfgSetSinglePort(ZUINT iSinglePort);

/**
 * @brief Get ims single remote address
 *
 * @retval ims single remote address.
 */
ZFUNC ZCHAR * Zos_CfgGetSingleRmtAddr(ZFUNC_VOID);

/**
 * @brief Set ims single remote address.
 *
 * @param [in] iSinglePort ims single remote address.
 *
 * @retval ZOK Set configuration successfully.
 * @retval ZFAILED Set configuration failed.
 *
 */
ZFUNC ZINT Zos_CfgSetSingleRmtAddr(ZCHAR *pcSingleRmtAddr);

/**
 * @brief Get diameter timeout timer length.
 *
 * @retval diameter timeout timer length, 
 * default is 30 seconds.
 *
 * @see Zos_CfgSetDiameterTimeoutTmrLen
 */
ZFUNC ZUINT Zos_CfgGetDiameterTimeoutTmrLen(ZFUNC_VOID);

/**
 * @brief Set diameter timeout timer length(seconds).
 *
 * @param [in] iTimeLen Alert time length.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetDiameterTimeoutTmrLen
 */
ZFUNC ZINT Zos_CfgSetDiameterTimeoutTmrLen(ZUINT iTimeLen);

/**
 * @brief Get diameter repeat times.
 *
 * @retval diameter repeat times, 
 * default is 3.
 *
 * @see Zos_CfgSetDiameterRepeartTimes
 */
ZFUNC ZUINT Zos_CfgGetDiameterRepeartTimes(ZFUNC_VOID);

/**
 * @brief Set diameter repeat times.
 *
 * @param [in] iTimeLen Alert time length.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetDiameterRepeartTimes
 */
ZFUNC ZINT Zos_CfgSetDiameterRepeartTimes(ZUINT iTimeLen);

/**
 * @brief Get diameter heartbeat timer length.
 *
 * @retval diameter heartbeat timer length, 
 * default is 6 seconds.
 *
 * @see Zos_CfgSetDiameterHeartbeatTmrLen
 */
ZFUNC ZUINT Zos_CfgGetDiameterHeartbeatTmrLen(ZFUNC_VOID);

/**
 * @brief Set diameter heartbeat timer length(seconds).
 *
 * @param [in] iTimeLen Alert time length.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see Zos_CfgGetDiameterHeartbeatTmrLen
 */
ZFUNC ZINT Zos_CfgSetDiameterHeartbeatTmrLen(ZUINT iTimeLen);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_CFG_H__ */

