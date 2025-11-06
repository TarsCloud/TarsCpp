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
  File name     : zos_log.h
  Module        : zos log library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the log interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_LOG_H__
#define _ZOS_LOG_H__

/** 
 * @file zos_log.h
 * @brief Zos log interfaces.
 *  
 * In this file, it defined the operation function
 * of zos log, including create, delete, flush, open and etc.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* zos log option */
/** @{ */
#define ZLOG_OPT_NULL      0x00000000 /**< @brief no option */
#define ZLOG_OPT_MUTEX     0x00000001 /**< @brief asynchronism option */
#define ZLOG_OPT_PRINT     0x00000002 /**< @brief print option */
/** @} */

/* zos log level */
/** @{ */
#define ZLOG_LEVEL_NULL    0x00000000 /**< @brief null to be logged */
#define ZLOG_LEVEL_FATAL   0x00000001 /**< @brief fatal message to be logged */
#define ZLOG_LEVEL_ERROR   0x00000002 /**< @brief error message to be logged */
#define ZLOG_LEVEL_WARN    0x00000004 /**< @brief warning message to be logged */
#define ZLOG_LEVEL_INFO    0x00000008 /**< @brief info message to be logged */
#define ZLOG_LEVEL_DBG     0x00000010 /**< @brief debug message to be logged */
#define ZLOG_LEVEL_FUNC    0x00000100 /**< @brief function trace to be logged */
#if defined(DEBUG) || defined(_DEBUG)
#define ZLOG_LEVEL_ALL     0x000001FF /**< @brief all message to be logged */
#define ZLOG_LEVEL_DEFAULT ZLOG_LEVEL_ALL
#else
#define ZLOG_LEVEL_ALL     0x000000FF /**< @brief all message to be logged */
#define ZLOG_LEVEL_DEFAULT (ZLOG_LEVEL_FATAL | ZLOG_LEVEL_ERROR \
                            | ZLOG_LEVEL_WARN | ZLOG_LEVEL_INFO)
#endif
/** @} */

#if 0

/**< @brief zos log fatal message */
#define ZXX_LOG_FATAL(...)
        
/**< @brief zos log error message */
#define ZXX_LOG_ERROR(...)
        
/**< @brief zos log warning message */
#define ZXX_LOG_WARN(...)
        
/**< @brief zos log info message */
#define ZXX_LOG_INFO(...)

/**< @brief zos log debug message */
#define ZXX_LOG_DBG(...)

/**< @brief zos log string */
#define ZXX_LOG_STR(_logid, _level, _str)

/**< @brief zos log buffer */
#define ZXX_LOG_BUF(_logid, _level, _buf)

#else /* support log */

/** 
 * @brief Zos log fatal message. 
 * @see Zos_LogFatal
 */
#define ZXX_LOG_FATAL(...) Zos_LogFatal(__VA_ARGS__)
        
/** 
 * @brief Zos log error message. 
 * @see Zos_LogError
 */
#define ZXX_LOG_ERROR(...) Zos_LogError(__VA_ARGS__)
        
/** 
 * @brief Zos log warning message. 
 * @see Zos_LogWarn
 */
#define ZXX_LOG_WARN(...) Zos_LogWarn(__VA_ARGS__)
        
/** 
 * @brief Zos log info message. 
 * @see Zos_LogInfo
 */
#define ZXX_LOG_INFO(...) Zos_LogInfo(__VA_ARGS__)

/** 
 * @brief Zos log debug message. 
 * @see Zos_LogDbg
 */
#define ZXX_LOG_DBG(...) Zos_LogDbg(__VA_ARGS__)

/** 
 * @brief Zos log function message. 
 * @see Zos_LogFunc
 */
#define ZXX_LOG_FUNC(...) Zos_LogFunc(__VA_ARGS__)

/** 
 * @brief Zos log string. 
 * @see Zos_LogStr
 */
#define ZXX_LOG_STR(...) Zos_LogStr(__VA_ARGS__)

/** 
 * @brief Zos log buffer. 
 * @see Zos_LogBuf
 */
#define ZXX_LOG_BUF(...) Zos_LogBuf(__VA_ARGS__)
        
#endif

/**< @brief zos log id */
#define ZOS_LOGID Zos_LogGetZosId()
#define ZOS_LOGIN_LOGID Zos_LogGetLoginLogId()

/* zos log macros */
#define ZOS_LOG_FATAL(...)   ZXX_LOG_FATAL(__VA_ARGS__)
#define ZOS_LOG_ERROR(...)   ZXX_LOG_ERROR(__VA_ARGS__)
#define ZOS_LOG_WARN(...)    ZXX_LOG_WARN(__VA_ARGS__)
#define ZOS_LOG_INFO(...)    ZXX_LOG_INFO(__VA_ARGS__)
#define ZOS_LOG_DBG(...)     ZXX_LOG_DBG(__VA_ARGS__)
#define ZOS_LOG_FUNC(...)    ZXX_LOG_FUNC(__VA_ARGS__)

#define ZLOG_ID(_id) (ZUINT)((ZSIZE_T)(_id))
#define ZLOG_FATAL(_name, _id, ...) \
    Zos_LogNameStr(_name, ZLOG_LEVEL_FATAL, ZLOG_ID(_id), __VA_ARGS__)
#define ZLOG_WARN(_name, _id, ...) \
    Zos_LogNameStr(_name, ZLOG_LEVEL_WARN, ZLOG_ID(_id), __VA_ARGS__)
#define ZLOG_ERROR(_name, _id, ...) \
    Zos_LogNameStr(_name, ZLOG_LEVEL_ERROR, ZLOG_ID(_id), __VA_ARGS__)
#define ZLOG_INFO(_name, _id, ...) \
    Zos_LogNameStr(_name, ZLOG_LEVEL_INFO, ZLOG_ID(_id), __VA_ARGS__)
#define ZLOG_DBG(_name, _id, ...) \
    Zos_LogNameStr(_name, ZLOG_LEVEL_DBG, ZLOG_ID(_id), __VA_ARGS__)
#define ZLOG_FUNC(_name, _id, ...) \
    Zos_LogNameStr(_name, ZLOG_LEVEL_FUNC, ZLOG_ID(_id), __VA_ARGS__)

/* zos log deprecated function */
#define ZOS_LOG_DEPRECATED_FUNC() \
    ZXX_LOG_WARN(ZOS_LOGID, "%s DEPRECATED.", __FUNCTION__)

/**< @brief zos log watch log string */
typedef ZVOID (*PFN_LOGWATCHLOG)(ZLOG zLogId, ZUINT iWatcherId,
                ZUINT iLevel, ZCHAR *pcLogStr);
    
/**
 * @brief This function creats log.
 *
 * @param [in] pcIdName The log ID name.
 * @param [in] iLogLevel The supported log level.
 *
 * @return A pointer to the log id, or return ZNULL.
 *
 * @see 
 */
ZFUNC ZLOG Zos_LogCreate(ZCHAR *pcIdName, ZUINT iLogLevel);

/**
 * @brief This function creats log with specific file and control option.
 *
 * @param [in] pcIdName The log ID name.
 * @param [in] pcFileName The log file name.
 * @param [in] pcLogDir The log file directory. 
 * @param [in] iLogOpt The log option.
 * @param [in] iLogLevel The supported log level. 
 * @param [in] iBufSize Size of the log buffer.
 * @param [in] iFileType Log file type, 1 for login log file, 0 for other log files.
 *
 * @return A pointer to the log id, or return ZNULL.
 *
 * @see 
 */
ZFUNC ZLOG Zos_LogCreateX(ZCHAR *pcIdName, ZCHAR *pcFileName,
                ZCONST ZCHAR *pcLogDir, ZUINT iLogOpt, ZUINT iLogLevel,
                ZUINT iBufSize, ZUINT iFileType);

/**
 * @brief This function deletes a log.
 *
 * @param [in] zLogId The log ID.
 *
 * @return[none].
 *
 * @see 
 */
ZFUNC ZVOID Zos_LogDelete(ZLOG zLogId);

/**
 * @brief This function flushs buffer into file.
 *
 * @param [in] zLogId The log ID.
 *
 * @retval ZOK Flush operation successfully.
 * @retval ZFAILED Flush operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogFlush(ZLOG zLogId);

/**
 * @brief This function get zos log id.
 *
 * @return Zos Log id.
 *
 */
ZFUNC ZLOG Zos_LogGetZosId(ZFUNC_VOID);

/**
 * @brief This function get zos login log id.
 *
 * @return Zos login log id.
 *
 */
ZFUNC ZLOG Zos_LogGetLoginLogId(ZFUNC_VOID);

/**
 * @brief This function gets the print opetion.
 *
 * @param [in] zLogId The log ID.
 * @param [out] pbPrint Shows printed information in the console flag,
 *                      default is ZFALSE.
 *
 * @retval ZOK Get operation successfully.
 * @retval ZFAILED Get operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogGetPrint(ZLOG zLogId, ZBOOL *pbPrint);

/**
 * @brief This function sets the print opetion.
 *
 * @param [in] zLogId The log ID.
 * @param [in] bPrint Flag of print out. If ZTRUE, print log to console.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogSetPrint(ZLOG zLogId, ZBOOL bPrint);

/**
 * @brief This function gets the log id.
 *
 * @param [in] pcName The log name.
 * @param [out] pzLogId The log ID.
 *
 * @retval ZOK Get operation successfully.
 * @retval ZFAILED Get operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogGetId(ZCONST ZCHAR *pcName, ZLOG *pzLogId);

/**
 * @brief This function gets the log level.
 *
 * @param [in] zLogId The log ID.
 * @param [out] piLevel The log level.
 *
 * @retval ZOK Get operation successfully.
 * @retval ZFAILED Get operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogGetLevel(ZLOG zLogId, ZUINT *piLevel);

/**
 * @brief This function gets the log watcher(no time, module level prefix).
 *
 * @param [in] zLogId The log ID.
 * @param [in] piWatcherId The log watcher id.
 * @param [out] ppfnWatchLog The log watcher function pointer.
 *
 * @retval ZOK Get operation successfully.
 * @retval ZFAILED Get operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogGetWatcher(ZLOG zLogId, ZUINT *piWatcherId,
                PFN_LOGWATCHLOG *ppfnWatchLog);

/**
 * @brief This function sets the log watcher(no time, module level prefix).
 *
 * @param [in] zLogId The log ID.
 * @param [in] iWatcherId The log watcher id.
 * @param [in] pfnWatchLog The log watcher function pointer.
 *
 * @retval ZOK Set operation successfully.
 * @retval ZFAILED Set operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogSetWatcher(ZLOG zLogId, ZUINT iWatcherId,
                PFN_LOGWATCHLOG pfnWatchLog);

/**
 * @brief This function opens level by log id.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 *
 * @retval ZOK Open operation successfully.
 * @retval ZFAILED Open operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogOpen(ZLOG zLogId, ZUINT iLevel);

/**
 * @brief This function close level by log id.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 *
 * @retval ZOK Close operation successfully.
 * @retval ZFAILED Close operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogClose(ZLOG zLogId, ZUINT iLevel);

/**
 * @brief This function opens level by log name.
 *
 * @param [in] pcName Name of log to open.
 * @param [out] iLevel The log level.
 *
 * @retval ZOK Open operation successfully.
 * @retval ZFAILED Open operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogOpenX(ZCHAR *pcName, ZUINT iLevel);

/**
 * @brief This function close level by log name.
 *
 * @param [in] pcName Name of log to close.
 * @param [in] iLevel The log level.
 *
 * @retval ZOK Close operation successfully.
 * @retval ZFAILED Close operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogCloseX(ZCHAR *pcName, ZUINT iLevel);

/**
 * @brief This function prints log fatal information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogFatal(ZLOG zLogId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief This function prints log error information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogError(ZLOG zLogId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief This function prints log warning information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogWarn(ZLOG zLogId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief This function prints log normal information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogInfo(ZLOG zLogId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief This function prints log debug information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogDbg(ZLOG zLogId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief This function prints log function information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogFunc(ZLOG zLogId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(2,3);

/**
 * @brief This function prints log string information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 * @param [in] pcStr The log string.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogStr(ZLOG zLogId, ZUINT iLevel, ZCHAR *pcStr);

/**
 * @brief This function prints log string information with length.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 * @param [in] pcStr The log string.
 * @param [in] iLen The log string length.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogNStr(ZLOG zLogId, ZUINT iLevel, ZCHAR *pcStr, ZUINT iLen);

/**
 * @brief This function prints log structure string information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 * @param [in] pstStr The log structure string.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see 
 */
ZFUNC ZINT Zos_LogSStr(ZLOG zLogId, ZUINT iLevel, ST_ZOS_SSTR *pstStr);

/**
 * @brief This function prints log format string information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 * @param [in] pcFormat String format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogFStr
 */
ZFUNC ZINT Zos_LogFStr(ZLOG zLogId, ZUINT iLevel, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(3,4);

/**
 * @brief This function prints log format variable string information,
 * format string maybe has prefix.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 * @param [in] pcFmtPrefix Log String prefix.
 * @param [in] pcFormat String format.
 * @param [in] zAp String variable list.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogFStr
 */
ZFUNC ZINT Zos_LogVFStr(ZLOG zLogId, ZUINT iLevel, ZCONST ZCHAR *pcFmtPrefix, 
                ZCONST ZCHAR *pcFormat, ZVA_LIST zAp);

/**
 * @brief This function prints log log buffer information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] iLevel The log level.
 * @param [in] zData The log buffer.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogFStr
 */
ZFUNC ZINT Zos_LogBuf(ZLOG zLogId, ZUINT iLevel, ZDBUF zData);

/**
 * @brief Print log named format variable string information.
 *
 * @param [in] pcLogName The log name
 * @param [in] iLevel The log level.
 * @param [in] pcFormat The log format.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogNameFStr
 */
ZFUNC ZINT Zos_LogNameStr(ZCONST ZCHAR *pcLogName, ZUINT iLevel,
        ZUINT iId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(4,5);

/**
 * @brief This function prints log named format string information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcNewLogName The log name
 * @param [in] iLevel The log level.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogNameFStrX
 */
ZFUNC ZINT Zos_LogNameFStr(ZLOG zLogId, ZCONST ZCHAR *pcNewLogName, ZUINT iLevel, 
                ZUINT iId, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(5,6);

/**
 * @brief Print log named format string information.
 *
 * @param [in] pcLogName The log name.
 * @param [in] pcNewLogName The log name
 * @param [in] iLevel The log level.
 * @param [in] pcFormat The log format.
 * @param [in] ... String vars.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogNameFStr
 */
ZFUNC ZINT Zos_LogNameFStrX(ZCONST ZCHAR *pcLogName, ZCONST ZCHAR *pcNewLogName, 
                ZUINT iLevel, ZCONST ZCHAR *pcFormat, ...) ZATTR_PRINTF(4,5);

/**
 * @brief Print log named format data buffer information,
 * the log name can be changed in explicit.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcNewLogName The log name
 * @param [in] iLevel The log level.
 * @param [in] zData The data buffer id.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogNameFStr
 */
ZFUNC ZINT Zos_LogNameBuf(ZLOG zLogId, ZCONST ZCHAR *pcNewLogName, ZUINT iLevel, 
                ZDBUF zData);

/**
 * @brief Print log named format variable string information.
 *
 * @param [in] zLogId The log ID.
 * @param [in] pcNewLogName The log name
 * @param [in] iLevel The log level.
 * @param [in] pcFmtPrefix Log String prefix.
 * @param [in] pcFormat The log format.
 * @param [in] zAp String variable list.
 *
 * @retval ZOK Print operation successfully.
 * @retval ZFAILED Print operation failed.
 *
 * @see Zos_LogNameFStr
 */
ZFUNC ZINT Zos_LogNameVFStr(ZLOG zLogId, ZCONST ZCHAR *pcNewLogName, ZUINT iLevel, 
                ZCHAR *pcFmtPrefix, ZCONST ZCHAR *pcFormat, ZVA_LIST zAp);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_LOG_H__ */
  
