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
  File name     : zos_osdep_prv.h
  Module        : zos os dependency library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-08-10    
  Description   :
      included files required by os dependency
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_OSDEP_PRV_H__
#define _ZOS_OSDEP_PRV_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @brief ZOS os timer active function for callback */
typedef ZVOID (*PFN_ZOSTIMERACTIVE)(ZBOOL bPostEvnt);

/** @brief ZOS os memory and time functions */
typedef ZVOID * (*PFN_ZMALLOC)(ZSIZE_T zSize);
typedef ZVOID * (*PFN_ZREALLOC)(ZVOID *pMem, ZSIZE_T zSize);
typedef ZVOID (*PFN_ZFREE)(ZVOID *pMem);
typedef ZINT (*PFN_ZPRINT)(ZCHAR *pcStr);
typedef ZINT (*PFN_ZTIMEINIT)(ZFUNC_VOID);
typedef ZVOID (*PFN_ZTIMEDESTROY)(ZFUNC_VOID);
typedef ZINT (*PFN_ZGETEPOCHTIME)(ST_ZOS_TIMESPEC *pstTime);
typedef ZINT (*PFN_ZGETHIRESTIME)(ST_ZOS_TIMESPEC *pstTime);
typedef ZHRTIME_T (*PFN_ZGETHRTIME)(ZFUNC_VOID);
typedef ZTIME_T (*PFN_ZTIME)(ZTIME_T *pzTime);
typedef ST_ZOS_TM * (*PFN_ZLOCALTIME)(ZTIME_T *pzTime);
typedef ZTIME_T (*PFN_ZMKTIME)(ST_ZOS_TM *pstTime);
typedef ZINT (*PFN_ZTIMEZONE)(ZFUNC_VOID);

/** @brief ZOS os file functions */
typedef ZINT (*PFN_ZFILEOPEN)(ZCONST ZCHAR *pcFileName, ZUINT iFlag, ZFILE *pzFile);
typedef ZINT (*PFN_ZFILECLOSE)(ZFILE zFile);
typedef ZINT (*PFN_ZFILEREMOVE)(ZCONST ZCHAR *pcFileName);
typedef ZINT (*PFN_ZFILEREMOVEX)(ZCONST ZCHAR *pcFileName);
typedef ZINT (*PFN_ZFILERENAME)(ZCONST ZCHAR *pcOldName, ZCONST ZCHAR *pcNewName);
typedef ZBOOL (*PFN_ZFILEEOF)(ZFILE zFile);
typedef ZINT (*PFN_ZFILEREAD)(ZFILE zFile, ZCHAR *pcBuf, ZSIZE_T *pzSize);
typedef ZINT (*PFN_ZFILEWRITE)(ZFILE zFile, ZCHAR *pcBuf, 
                    ZSIZE_T *pzSize);
typedef ZINT (*PFN_ZFILEFLUSH)(ZFILE zFile);
typedef ZINT (*PFN_ZFILEGETS)(ZFILE zFile, ZCHAR *pcStr, ZINT iLen);
typedef ZINT (*PFN_ZFILESEEK)(ZFILE zFile, ZUINT iFlag, ZUINT iOffset);
typedef ZINT (*PFN_ZFILETELL)(ZFILE zFile, ZUINT *piOffset);
typedef ZINT (*PFN_ZFILESIZE)(ZFILE zFile, ZUINT *piSize);
typedef ZINT (*PFN_ZFILESTAT)(ZCONST ZCHAR *pcFileName, ST_ZFILE_INFO *pstInfo);
typedef ZINT (*PFN_ZDIROPEN)(ZCONST ZCHAR *pcDirName, ZDIR *pzDir);
typedef ZINT (*PFN_ZDIRCLOSE)(ZDIR zDir);
typedef ZINT (*PFN_ZDIRREAD)(ZDIR zDir, ZCHAR *pcEntryName,
                ZUINT iMaxSize, ST_ZFILE_INFO *pstInfo);
typedef ZINT (*PFN_ZDIRMAKE)(ZCONST ZCHAR *pcDirName, ZUINT iFlag);
typedef ZINT (*PFN_ZDIRREMOVE)(ZCONST ZCHAR *pcDirName);

/** @brief ZOS os mutex and semaphores functions */
typedef ZINT (*PFN_ZMUTEXCREATE)(ZMUTEX *pzMutex);
typedef ZVOID (*PFN_ZMUTEXDELETE)(ZMUTEX *pzMutex);
typedef ZINT (*PFN_ZMUTEXLOCK)(ZMUTEX *pzMutex);
typedef ZINT (*PFN_ZMUTEXUNLOCK)(ZMUTEX *pzMutex);
typedef ZINT (*PFN_ZSHAREXCREATE)(ZSHAREX *pzSharex);
typedef ZVOID (*PFN_ZSHAREXDELETE)(ZSHAREX *pzSharex);
typedef ZINT (*PFN_ZSHAREXLOCK)(ZSHAREX *pzSharex);
typedef ZINT (*PFN_ZSHAREXUNLOCK)(ZSHAREX *pzSharex);
typedef ZINT (*PFN_ZSHAREXLOCKEX)(ZSHAREX *pzSharex);
typedef ZINT (*PFN_ZSHAREXUNLOCKEX)(ZSHAREX *pzSharex);
typedef ZINT (*PFN_ZSEMCREATE)(ZSEM *pzSem, ZUINT iValue);
typedef ZINT (*PFN_ZSEMCREATEONE)(ZSEM *pzSem);
typedef ZVOID (*PFN_ZSEMDELETE)(ZSEM *pzSem);
typedef ZINT (*PFN_ZSEMWAIT)(ZSEM *pzSem, ZUINT iMilliSec);
typedef ZINT (*PFN_ZSEMPOST)(ZSEM *pzSem);

/** @brief ZOS os network functions */
typedef ZBOOL (*PFN_ZTRUSTEVALUATE)(ZUCHAR *pucTrustCert, ZUINT iCertLen);
typedef ZINT (*PFN_ZNETINIT)(ZFUNC_VOID);
typedef ZVOID (*PFN_ZNETDESTROY)(ZFUNC_VOID);
typedef ZINT (*PFN_ZINETADDR)(ZCONST ZCHAR *pcAddrStr, ZUINT *piAddr);
typedef ZUINT (*PFN_ZINETNTOHL)(ZUINT iVal);
typedef ZUINT (*PFN_ZINETHTONL)(ZUINT iVal);
typedef ZUSHORT (*PFN_ZINETNTOHS)(ZUSHORT iVal);
typedef ZUSHORT (*PFN_ZINETHTONS)(ZUSHORT iVal);
typedef ZINT (*PFN_ZINETFDCREATE)(ZFD_SET *pzFds);
typedef ZINT (*PFN_ZINETFDDELETE)(ZFD_SET zFds);
typedef ZINT (*PFN_ZINETFDCOPY)(ZFD_SET zDstFds, ZFD_SET zSrcFds);
typedef ZINT (*PFN_ZINETFDZERO)(ZFD_SET zFds);
typedef ZINT (*PFN_ZINETFDSET)(ZFD_SET zFds, ZINT iFd);
typedef ZBOOL (*PFN_ZINETFDISSET)(ZFD_SET zFds, ZINT iFd);
typedef ZINT (*PFN_ZINETFDCLR)(ZFD_SET zFds, ZINT iFd);
typedef ZINT (*PFN_ZINETGETHOSTBYNAME)(ZCHAR *pcName, 
                    ST_ZOS_INET_IPS *pstIps);
typedef ZINT (*PFN_ZINETGETLCLIP)(ST_ZOS_INET_IPS *pstIps, ZINT iIpType);
typedef ZINT (*PFN_ZINETGETACCNETTYPE)(ZINT iDefNetType);
typedef ZCHAR * (*PFN_ZINETGETDNSIPADDR)(ZINT index);
typedef ZINT (*PFN_ZINETGETDNSCNT)(ZFUNC_VOID);
typedef ZINT (*PFN_ZSOCKETOPEN)(ZINT iInetType, ZINT iProtoType, 
                ZINT iSrvType, ZBOOL bBlock, ZBOOL bReuseAddr, 
                ZINT *piSock, ZINT iSockType, ZBOOL bDrvTask,
                ZVOID *pUserInfo);
typedef ZINT (*PFN_ZSOCKETCEASE)(ZSOCKET zSock);
typedef ZINT (*PFN_ZSOCKETCLOSE)(ZSOCKET zSock);
typedef ZINT (*PFN_ZSOCKETSHUTDOWN)(ZSOCKET zSock, ZINT iHowTo);
typedef ZINT (*PFN_ZSOCKETSELECT)(ZINT iMaxFd, ZFD_SET zReadFds, 
                ZFD_SET zWriteFds, ZFD_SET zExceptFds, 
                ZINT iMsecTimeOut, ZINT *piNumFds);
typedef ZINT (*PFN_ZSOCKETBIND)(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstAddr);
typedef ZINT (*PFN_ZSOCKETLISTEN)(ZSOCKET zSock);
typedef ZINT (*PFN_ZSOCKETACCEPT)(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstAddr, 
                ZBOOL bBlock, ZINT *piNewSock);
typedef ZINT (*PFN_ZSOCKETCONNECT)(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstAddr,
                ZBOOL bConnTls, ZBOOL *pbRunErr, ZBOOL *pbConned);
typedef ZINT (*PFN_ZSOCKETSENDTO)(ZSOCKET zSock, ST_ZOS_INET_ADDR *pstAddr, 
                ZUCHAR *pucBuf, ZUINT *piLen, ZBOOL *pbClosed, ZBOOL *pbWait);
typedef ZINT (*PFN_ZSOCKETRECVFROM)(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstAddr, ZUCHAR *pucBuf, ZUINT *piLen);
typedef ZINT (*PFN_ZSOCKETSEND)(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen, 
                ZBOOL *pbClosed, ZBOOL *pbWait);
typedef ZINT (*PFN_ZSOCKETRECV)(ZSOCKET zSock, ZUCHAR *pucBuf, ZUINT *piLen,
                ZBOOL *pbClosed);
typedef ZINT (*PFN_ZSOCKETGETLCLADDR)(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstLcalAddr);
typedef ZINT (*PFN_ZSOCKETGETRMTADDR)(ZSOCKET zSock, 
                ST_ZOS_INET_ADDR *pstRmtAddr);
typedef ZINT (*PFN_ZSOCKETSETOPTBLK)(ZSOCKET zSock, ZBOOL bBlock);
typedef ZINT (*PFN_ZSOCKETSETOPTREUSEADDR)(ZSOCKET zSock, 
                ZBOOL bReuseAddr);
typedef ZINT (*PFN_ZSOCKETSETOPTRECVBUF)(ZSOCKET zSock, ZUINT iBufSize);
typedef ZINT (*PFN_ZSOCKETSETOPTSENDBUF)(ZSOCKET zSock, ZUINT iBufSize);
typedef ZINT (*PFN_ZSOCKETSETOPTTOS)(ZSOCKET zSock, ZUINT iTosVal);
typedef ZINT (*PFN_ZSOCKETSETOPTTCLASS)(ZSOCKET zSock, ZUINT iTClassVal);
typedef ZINT (*PFN_ZSOCKETSETOPTMADDR)(ZSOCKET zSock, ST_ZOS_INET_IP *pstMAddr);
typedef ZINT (*PFN_ZSOCKETGETOPTERR)(ZSOCKET zSock, ZINT *piErrNo);
typedef ZINT (*PFN_ZSOCKETTASKENTRY)(ST_ZOS_TASK_ENTRY *pstEntry);
typedef ZINT (*PFN_ZSOCKETGETLASTERR)(ZFUNC_VOID);
typedef ZINT (*PFN_ZSOCKETCREATETUNL)(PFN_TUNLCREATECB pfnCb, ZCOOKIE zCookie,
                ZINT iTunlType, ST_ZOS_INET_ADDR *pstAddr, 
                ST_ZOS_SOCK_PROXY_SRV_INFO *pstStgProxyInfo,
                ST_ZOS_SOCK_CFG *pstCfg);
typedef ZVOID (*PFN_ZSOCKETCLOSETUNL)(ZFUNC_VOID);
typedef ZINT (*PFN_ZSOCKETSTARTTUNLTASK)(ZFUNC_VOID);
typedef ZVOID (*PFN_ZSOCKETSTOPTUNLTASK)(ZFUNC_VOID);

/** @brief ZOS os task(thread) functions */
typedef ZINT (*PFN_ZMODPOSTDRV)(ZFUNC_VOID);
typedef ZINT (*PFN_ZTASKSPAWN)(ZCONST ZCHAR *pcName, ZINT iPriority, 
                ZUINT iStackSize, PFN_ZTASKENTRY pfnEntry, ZVOID *pEntryParm, 
                ZTHREAD_ID *pzRealTaskId, ZTHREAD_ID *pzThreadId);
typedef  ZINT (*PFN_ZTASKDELETE)(ZTHREAD_ID zThreadId, ZBOOL bForce);
typedef  ZVOID (*PFN_ZTASKDELAY)(ZUINT iMilliseconds);
typedef  ZVOID (*PFN_ZTASKBORN)(ZTHREAD_ID zThreadId, ZCOOKIE *pzCookie);
typedef  ZVOID (*PFN_ZTASKEXIT)(ZTHREAD_ID zThreadId, ZCOOKIE zCookie);
typedef  ZBOOL (*PFN_ZTASKISCURRENT)(ZTHREAD_ID zRealThreadId);
typedef  ZBOOL (*PFN_ZTASKISMAINTHREAD)();
typedef  ZTHREAD_ID (*PFN_ZTASKGETCURRENT)();
typedef  ZVOID (*PFN_ZTASKSETTHREADNAME)(ZCONST ZCHAR *pcName);
typedef  ZVOID (*PFN_ZTASKGETTHREADNAME)(ZCHAR *pcName);
typedef  ZBOOL (*PFN_ZTASKEQUAL)(ZTHREAD_ID zRealThreadIdA, ZTHREAD_ID zRealThreadIdB);
typedef  ZINT (*PFN_ZTASKINITCRASHREPORT)(ZCHAR* pcDmpPath);
typedef  ZINT (*PFN_ZUIMSGPROC)();
typedef  ZINT (*PFN_ZUIPROCEVNT)(ZEVNT zEvntId, PFN_ZUIMSGPROC pfnUiMsgProc);

/** @brief ZOS os timer functions */
typedef ZINT (*PFN_ZTIMERINIT)(ZFUNC_VOID);
typedef ZINT (*PFN_ZTIMERDESTROY)(ZFUNC_VOID);
typedef ZINT (*PFN_ZTIMERSTART)(ZUINT iTimeLen, 
                PFN_ZOSTIMERACTIVE pfnActive);
typedef ZINT (*PFN_ZTIMERSTOP)(ZFUNC_VOID);

/** @brief ZOS OS HTTP functions */
typedef ZHTTPOBJ (*PFN_ZHTTPOPEN)(ZINT iId, ZCONST ZCHAR *pcUri,
    ZBOOL bSecurity, ZBOOL bPost);
typedef ZVOID (*PFN_ZHTTPCLOSE)(ZHTTPOBJ zConn);
typedef ZINT (*PFN_ZHTTPCONN)(ZHTTPOBJ zConn);
typedef ZINT (*PFN_ZHTTPDISCONN)(ZHTTPOBJ zConn);
typedef ZINT (*PFN_ZHTTPSETPROP)(ZHTTPOBJ zConn, ZCONST ZCHAR *pcName,
    ZCONST ZCHAR *pcValue);
typedef ZINT (*PFN_ZHTTPSETCFG)(ZHTTPOBJ zConn, ZCONST ZCHAR *pcName,
    ZCONST ZCHAR *pcValue);
typedef ZINT (*PFN_ZHTTPSETBODY)(ZHTTPOBJ zConn, ZCONST ZCHAR *pcHeader,
    ZCONST ZCHAR *pcContent);
typedef ZINT (*PFN_ZHTTPSETFILE)(ZHTTPOBJ zConn, ZCONST ZCHAR *pcHeader,
    ZCONST ZCHAR *pcFileName);
typedef ZCONST ZCHAR * (*PFN_ZHTTPGETBODY)(ZHTTPOBJ zConn);

/** @brief ZOS OS IMS functions */
typedef ZIMSID (*PFN_ZIMSHANDLECREATECONNECTIONMANAGER)();
typedef ZINT (*PFN_ZIMSHANDLETRIGGERREGISTRATION)(ZIMSID zObj);
typedef ZINT (*PFN_ZIMSHANDLETRIGGERDEREGISTRATION)(ZIMSID zObj);
typedef ZINT (*PFN_ZIMSHANDLECREATEIMCONNECTION)(ZIMSID zObj, ZCHAR *pcFeatureTag);
typedef ZINT (*PFN_ZIMSHANDLESENDIMMESSAGE)(ZIMSID zObj, ZCHAR *pcOutboundProxy, 
                    ZUSHORT wRemotePort, ZINT iProtocol, ZINT iMessageType, 
                    ZCHAR *pcCallId, ZCHAR *pcMessage, ZINT iMessageLen);
typedef ZINT (*PFN_ZIMSHANDLEAUTOCONFIG)(ZIMSID zObj, ZINT pcConfigType, ZINT pcUserData);
typedef ZINT (*PFN_ZIMSHANDLESETSTATUS)(ZIMSID zObj, ZCHAR *pcFeatureTag);


/* zos os function id */
typedef enum EN_ZOS_FUNC_ID
{
    /* zos os time dependency */
    EN_ZOS_FUNC_ID_MALLOC = 0,
    EN_ZOS_FUNC_ID_REALLOC,
    EN_ZOS_FUNC_ID_FREE,
    EN_ZOS_FUNC_ID_PRINT,
    EN_ZOS_FUNC_ID_TIMEINIT,
    EN_ZOS_FUNC_ID_TIMEDESTROY,
    EN_ZOS_FUNC_ID_GETEPOCHTIME,
    EN_ZOS_FUNC_ID_GETHIRESTIME,
    EN_ZOS_FUNC_ID_GETHRTIME,
    EN_ZOS_FUNC_ID_TIME,
    EN_ZOS_FUNC_ID_LOCALTIME,
    EN_ZOS_FUNC_ID_MKTIME,
    EN_ZOS_FUNC_ID_TIMEZONE,

    /* zos os file dependency */
    EN_ZOS_FUNC_ID_FILEOPEN,
    EN_ZOS_FUNC_ID_FILECLOSE,
    EN_ZOS_FUNC_ID_FILEREMOVE,
    EN_ZOS_FUNC_ID_FILEREMOVEX,
    EN_ZOS_FUNC_ID_FILERENAME,
    EN_ZOS_FUNC_ID_FILEEOF,
    EN_ZOS_FUNC_ID_FILEREAD,
    EN_ZOS_FUNC_ID_FILEWRITE,
    EN_ZOS_FUNC_ID_FILEFLUSH,
    EN_ZOS_FUNC_ID_FILEGETS,
    EN_ZOS_FUNC_ID_FILESEEK,
    EN_ZOS_FUNC_ID_FILETELL,
    EN_ZOS_FUNC_ID_FILESIZE,
    EN_ZOS_FUNC_ID_FILESTAT,
    EN_ZOS_FUNC_ID_DIROPEN,
    EN_ZOS_FUNC_ID_DIRCLOSE,
    EN_ZOS_FUNC_ID_DIRREAD,
    EN_ZOS_FUNC_ID_DIRMAKE,
    EN_ZOS_FUNC_ID_DIRREMOVE,

    /* zos os mutex and sempahore dependency */
    EN_ZOS_FUNC_ID_MUTEXCREATE,
    EN_ZOS_FUNC_ID_MUTEXDELETE,
    EN_ZOS_FUNC_ID_MUTEXLOCK,
    EN_ZOS_FUNC_ID_MUTEXUNLOCK,
    EN_ZOS_FUNC_ID_SHAREXCREATE,
    EN_ZOS_FUNC_ID_SHAREXDELETE,
    EN_ZOS_FUNC_ID_SHAREXLOCK,
    EN_ZOS_FUNC_ID_SHAREXUNLOCK,
    EN_ZOS_FUNC_ID_SHAREXLOCKEX,
    EN_ZOS_FUNC_ID_SHAREXUNLOCKEX,
    EN_ZOS_FUNC_ID_SEMCREATE,
    EN_ZOS_FUNC_ID_SEMCREATEONE,
    EN_ZOS_FUNC_ID_SEMDELETE,
    EN_ZOS_FUNC_ID_SEMWAIT,
    EN_ZOS_FUNC_ID_SEMPOST,

    /* zos os network dependency */
    EN_ZOS_FUNC_ID_TRUSTEVALUATE,
    EN_ZOS_FUNC_ID_NETINIT,
    EN_ZOS_FUNC_ID_NETDESTROY,
    EN_ZOS_FUNC_ID_INETADDR,
    EN_ZOS_FUNC_ID_INETNTOHL,
    EN_ZOS_FUNC_ID_INETHTONL,
    EN_ZOS_FUNC_ID_INETNTOHS,
    EN_ZOS_FUNC_ID_INETHTONS,
    EN_ZOS_FUNC_ID_INETFDCREATE,
    EN_ZOS_FUNC_ID_INETFDDELETE,
    EN_ZOS_FUNC_ID_INETFDCOPY,
    EN_ZOS_FUNC_ID_INETFDZERO,
    EN_ZOS_FUNC_ID_INETFDSET,
    EN_ZOS_FUNC_ID_INETFDISSET,
    EN_ZOS_FUNC_ID_INETFDCLR,
    EN_ZOS_FUNC_ID_INETGETHOSTBYNAME,
    EN_ZOS_FUNC_ID_INETGETLCLIP,
    EN_ZOS_FUNC_ID_INETGETACCNETTYPE,
    EN_ZOS_FUNC_ID_INETGETDNSIPADDR,
    EN_ZOS_FUNC_ID_INETGETDNSCNT,
    EN_ZOS_FUNC_ID_SOCKETOPEN,
    EN_ZOS_FUNC_ID_SOCKETCEASE,
    EN_ZOS_FUNC_ID_SOCKETCLOSE,
    EN_ZOS_FUNC_ID_SOCKETSHUTDOWN,
    EN_ZOS_FUNC_ID_SOCKETSELECT,
    EN_ZOS_FUNC_ID_SOCKETBIND,
    EN_ZOS_FUNC_ID_SOCKETLISTEN,
    EN_ZOS_FUNC_ID_SOCKETACCEPT,
    EN_ZOS_FUNC_ID_SOCKETCONNECT,
    EN_ZOS_FUNC_ID_SOCKETSENDTO,
    EN_ZOS_FUNC_ID_SOCKETRECVFROM,
    EN_ZOS_FUNC_ID_SOCKETSEND,
    EN_ZOS_FUNC_ID_SOCKETRECV,
    EN_ZOS_FUNC_ID_SOCKETGETLCLADDR,
    EN_ZOS_FUNC_ID_SOCKETGETRMTADDR,
    EN_ZOS_FUNC_ID_SOCKETSETOPTBLK,
    EN_ZOS_FUNC_ID_SOCKETSETOPTREUSEADDR,
    EN_ZOS_FUNC_ID_SOCKETSETOPTRECVBUF,
    EN_ZOS_FUNC_ID_SOCKETSETOPTSENDBUF,
    EN_ZOS_FUNC_ID_SOCKETSETOPTTOS,
    EN_ZOS_FUNC_ID_SOCKETSETOPTTCLASS,
    EN_ZOS_FUNC_ID_SOCKETSETOPTMADDR,
    EN_ZOS_FUNC_ID_SOCKETGETOPTERR,
    EN_ZOS_FUNC_ID_SOCKETGETLASTERR,
    EN_ZOS_FUNC_ID_SOCKETCREATETUNL,
    EN_ZOS_FUNC_ID_SOCKETCLOSETUNL,
    EN_ZOS_FUNC_ID_SOCKETSTARTTUNLTASK,
    EN_ZOS_FUNC_ID_SOCKETSTOPTUNLTASK,
 
    /* zos os task(thread) dependency */
    EN_ZOS_FUNC_ID_MODPOSTDRV,
    EN_ZOS_FUNC_ID_TASKSPAWN,
    EN_ZOS_FUNC_ID_TASKDELETE,
    EN_ZOS_FUNC_ID_TASKDELAY,
    EN_ZOS_FUNC_ID_TASKBORN,
    EN_ZOS_FUNC_ID_TASKEXIT,
    EN_ZOS_FUNC_ID_TASKISCURRENT,
    EN_ZOS_FUNC_ID_TASKISMAINTHREAD,
    EN_ZOS_FUNC_ID_TASKGETCURRENT,
    EN_ZOS_FUNC_ID_TASKSETTHREADNAME,
    EN_ZOS_FUNC_ID_TASKGETTHREADNAME,
    EN_ZOS_FUNC_ID_TASKEQUAL,
    EN_ZOS_FUNC_ID_TASKINITCRASHREPORT,
    EN_ZOS_FUNC_ID_UIPROCEVNT,

    /* zos os timer dependency */
    EN_ZOS_FUNC_ID_TIMERINIT,
    EN_ZOS_FUNC_ID_TIMERDESTROY,
    EN_ZOS_FUNC_ID_TIMERSTART,
    EN_ZOS_FUNC_ID_TIMERSTOP,

    /* zos os http dependency */
    EN_ZOS_FUNC_ID_HTTPOPEN,
    EN_ZOS_FUNC_ID_HTTPCLOSE,
    EN_ZOS_FUNC_ID_HTTPCONN,
    EN_ZOS_FUNC_ID_HTTPDISCONN,
    EN_ZOS_FUNC_ID_HTTPSETPROP,
    EN_ZOS_FUNC_ID_HTTPSETCFG,
    EN_ZOS_FUNC_ID_HTTPSETBODY,
    EN_ZOS_FUNC_ID_HTTPSETFILE,
    EN_ZOS_FUNC_ID_HTTPGETBODY,
    
    /* zos os ims dependency */
    EN_ZOS_FUNC_ID_IMSHANDLECREATECONNECTIONMANAGER,
    EN_ZOS_FUNC_ID_IMSHANDLECREATEIMCONNECTION,
    EN_ZOS_FUNC_ID_IMSHANDLESENDIMMESSAGE,
    EN_ZOS_FUNC_ID_IMSHANDLETRIGGERREGISTRATION,
    EN_ZOS_FUNC_ID_IMSHANDLETRIGGERDEREGISTRATION,
    EN_ZOS_FUNC_ID_IMSHANDLEAUTOCONFIG,
    EN_ZOS_FUNC_ID_IMSHANDLESETSTATUS,

    /* zos os maximum dependency */
    EN_ZOS_FUNC_ID_MAX
} EN_ZOS_FUNC_ID_TYPE;

/* zos os dependency set */
#define ZOS_OSDEP_SET(_funcs, _depmgr, _func, _type) \
    _funcs[EN_ZOS_FUNC_ID_##_type] = (ZFUNCPTR)_depmgr.pfn##_func

/* zos os dependency find */
#define ZOS_OSDEP_FIND(_func, _type) \
    _func = (PFN_Z##_type)Zos_OsdepFind(EN_ZOS_FUNC_ID_##_type)


/** @brief ZOS os dependence manager */
typedef struct tagZOS_OSDEP_MGR
{
    /* zos os time dependence */
    PFN_ZMALLOC pfnMalloc;           /**< @brief malloc */
    PFN_ZREALLOC pfnRealloc;         /**< @brief realloc */
    PFN_ZFREE pfnFree;               /**< @brief free */
    PFN_ZPRINT pfnPrint;             /**< @brief print string */
    PFN_ZTIMEINIT pfnTimeInit;       /**< @brief time init */
    PFN_ZTIMEDESTROY pfnTimeDestroy; /**< @brief time destroy */
    PFN_ZGETEPOCHTIME pfnGetEpochTime; /**< @brief get epoch time */
    PFN_ZGETHIRESTIME pfnGetHiresTime; /**< @brief get hires time */
    PFN_ZGETHRTIME pfnGetHrTime;     /**< @brief get hr time */
    PFN_ZTIME pfnTime;               /**< @brief time */
    PFN_ZLOCALTIME pfnLocalTime;     /**< @brief localtime */
    PFN_ZMKTIME pfnMkTime;           /**< @brief mktime */
    PFN_ZTIMEZONE pfnTimeZone;       /**< @brief timezone */

    /* zos os file dependence */
    PFN_ZFILEOPEN pfnFileOpen;       /**< @brief file open */
    PFN_ZFILECLOSE pfnFileClose;     /**< @brief file close */
    PFN_ZFILEREMOVE pfnFileRemove;   /**< @brief file remove */
    PFN_ZFILEREMOVEX pfnFileRemoveX; /**< @brief file remove(force) */
    PFN_ZFILERENAME pfnFileRename;   /**< @brief file rename */
    PFN_ZFILEEOF pfnFileEof;         /**< @brief file eof */
    PFN_ZFILEREAD pfnFileRead;       /**< @brief file read */
    PFN_ZFILEWRITE pfnFileWrite;     /**< @brief file write */
    PFN_ZFILEFLUSH pfnFileFlush;     /**< @brief file flush */
    PFN_ZFILEGETS pfnFileGets;       /**< @brief file gets */
    PFN_ZFILESEEK pfnFileSeek;       /**< @brief file seek */
    PFN_ZFILETELL pfnFileTell;       /**< @brief file tell */
    PFN_ZFILESIZE pfnFileSize;       /**< @brief file size */
    PFN_ZFILESTAT pfnFileStat;       /**< @brief file status */
    PFN_ZDIROPEN pfnDirOpen;         /**< @brief dir open */
    PFN_ZDIRCLOSE pfnDirClose;       /**< @brief dir close */
    PFN_ZDIRREAD pfnDirRead;         /**< @brief dir read */
    PFN_ZDIRMAKE pfnDirMake;         /**< @brief dir make */
    PFN_ZDIRREMOVE pfnDirRemove;     /**< @brief dir remove */

    /* zos os mutex and sempahore dependence */
    PFN_ZMUTEXCREATE pfnMutexCreate; /**< @brief mutex create */
    PFN_ZMUTEXDELETE pfnMutexDelete; /**< @brief mutex delete */
    PFN_ZMUTEXLOCK pfnMutexLock;     /**< @brief mutex lock*/
    PFN_ZMUTEXUNLOCK pfnMutexUnlock; /**< @brief mutex unlock */
    PFN_ZSHAREXCREATE pfnSharexCreate; /**< @brief shared-exclusive lock create */
    PFN_ZSHAREXDELETE pfnSharexDelete; /**< @brief shared-exclusive lock delete */
    PFN_ZSHAREXLOCK pfnSharexLock;    /**< @brief shared-exclusive lock lock*/
    PFN_ZSHAREXUNLOCK pfnSharexUnlock; /**< @brief shared-exclusive lock unlock */
    PFN_ZSHAREXLOCKEX pfnSharexLockEx; /**< @brief shared-exclusive lock lock*/
    PFN_ZSHAREXUNLOCKEX pfnSharexUnlockEx; /**< @brief shared-exclusive lock unlock */
    PFN_ZSEMCREATE pfnSemCreate;     /**< @brief semaphore create */
    PFN_ZSEMCREATEONE pfnSemCreateOne; /**< @brief semaphore create only one */
    PFN_ZSEMDELETE pfnSemDelete;     /**< @brief semaphore delete */
    PFN_ZSEMWAIT pfnSemWait;         /**< @brief semaphore wait */
    PFN_ZSEMPOST pfnSemPost;         /**< @brief semaphore post*/

    /* zos os network dependence */
    PFN_ZTRUSTEVALUATE pfnTrustEvaluate; /**< @brief trust certificate evaluate */
    PFN_ZNETINIT pfnNetInit;         /**< @brief net init */
    PFN_ZNETDESTROY pfnNetDestroy;   /**< @brief net destroy */
    PFN_ZINETADDR pfnInetAddr;       /**< @brief inetaddr */
    PFN_ZINETNTOHL pfnNtohl;         /**< @brief ntohl */
    PFN_ZINETHTONL pfnHtonl;         /**< @brief htonl */
    PFN_ZINETNTOHS pfnNtohs;         /**< @brief ntohs */         
    PFN_ZINETHTONS pfnHtons;         /**< @brief htons */
    PFN_ZINETFDCREATE pfnFdCreate;   /**< @brief fd_set create */
    PFN_ZINETFDDELETE pfnFdDelete;   /**< @brief fd_set delete */
    PFN_ZINETFDCOPY pfnFdCopy;       /**< @brief fd_set copy */
    PFN_ZINETFDZERO pfnFdZero;       /**< @brief fd_set FD_ZERO */
    PFN_ZINETFDSET pfnFdSet;         /**< @brief fd_set FD_SET */
    PFN_ZINETFDISSET pfnFdIsset;     /**< @brief fd_set FD_ISSET */
    PFN_ZINETFDCLR pfnFdClr;         /**< @brief fd_set FD_CLR */
    PFN_ZINETGETHOSTBYNAME pfnGetHostByName; /**< @brief get host by name */
    PFN_ZINETGETLCLIP pfnGetLocalIp; /**< @brief get local ip address */
    PFN_ZINETGETACCNETTYPE pfnGetAccNetType; /**< @brief get access network type */
	PFN_ZINETGETDNSIPADDR pfnGetDnsIpAddr;  /**< @brief get dns addr */
    PFN_ZINETGETDNSCNT pfnGetDnsCnt;  /**< @brief get dns count */
    
    /* zos os socket dependence */
    PFN_ZSOCKETOPEN pfnSocketOpen;   /**< @brief socket open */
    PFN_ZSOCKETCEASE pfnSocketCease; /**< @brief socket cease */
    PFN_ZSOCKETCLOSE pfnSocketClose; /**< @brief socket close */
    PFN_ZSOCKETSHUTDOWN pfnSocketShutdown; /**< @brief socket shutdown */
    PFN_ZSOCKETSELECT pfnSocketSelect; /**< @brief socket socket */
    PFN_ZSOCKETBIND pfnSocketBind;   /**< @brief socket bind */
    PFN_ZSOCKETLISTEN pfnSocketListen; /**< @brief socket listen */
    PFN_ZSOCKETACCEPT pfnSocketAccept; /**< @brief socket accept */
    PFN_ZSOCKETCONNECT pfnSocketConnect; /**< @brief socket connect */
    PFN_ZSOCKETSENDTO pfnSocketSendto; /**< @brief socket sendto */
    PFN_ZSOCKETRECVFROM pfnSocketRecvFrom; /**< @brief socket recvfrom */
    PFN_ZSOCKETSEND pfnSocketSend;   /**< @brief socket send */
    PFN_ZSOCKETRECV pfnSocketRecv;   /**< @brief socket recv */
    PFN_ZSOCKETGETLCLADDR pfnSocketGetLclAddr; /**< @brief socket get local addr */
    PFN_ZSOCKETGETRMTADDR pfnSocketGetRmtAddr; /**< @brief socket get remote addr */
    PFN_ZSOCKETSETOPTBLK pfnSocketSetOptBlk; /**< @brief socket set block option */
    PFN_ZSOCKETSETOPTREUSEADDR pfnSocketSetOptReuseAddr; /**< @brief set resuse addr */
    PFN_ZSOCKETSETOPTRECVBUF pfnSocketSetOptRecvBuf; /**< @brief set receive buffer size */
    PFN_ZSOCKETSETOPTSENDBUF pfnSocketSetOptSendBuf; /**< @brief set send buffer size */
    PFN_ZSOCKETSETOPTTOS pfnSocketSetOptTos; /**< @brief set ip tos */
    PFN_ZSOCKETSETOPTTCLASS pfnSocketSetOptTclass; /**< @brief set ipv6 tclass */
    PFN_ZSOCKETSETOPTMADDR pfnSocketSetOptMAddr; /**< @brief set multicast address */
    PFN_ZSOCKETGETOPTERR pfnSocketGetOptErr; /**< @brief socket get error option */
    PFN_ZSOCKETGETLASTERR pfnSocketGetLastErr; /**< @brief socket get last error */
    PFN_ZSOCKETCREATETUNL pfnSocketCreateTunl; /**< @brief socket create tunnel */
    PFN_ZSOCKETCLOSETUNL pfnSocketCloseTunl; /**< @brief socket close tunnel */
    PFN_ZSOCKETSTARTTUNLTASK pfnSocketStartTunlTask;/**< @brief socket start tunnel task */
    PFN_ZSOCKETSTOPTUNLTASK pfnSocketStopTunlTask;/**< @brief socket stop tunnel task */
 
    /* zos os task(thread) dependence */
    PFN_ZMODPOSTDRV pfnModPostDrv;   /**< @brief module post drive notification */
    PFN_ZTASKSPAWN pfnTaskSpawn;     /**< @brief task spawn */
    PFN_ZTASKDELETE pfnTaskDelete;   /**< @brief task delete */
    PFN_ZTASKDELAY pfnTaskDelay;     /**< @brief task delay */
    PFN_ZTASKBORN pfnTaskBorn;       /**< @brief task has born(voluntary) */
    PFN_ZTASKEXIT pfnTaskExit;       /**< @brief task exit(voluntary) */
    PFN_ZTASKISCURRENT pfnTaskIsCurrent;/**< @brief if task is on current thread */
    PFN_ZTASKISMAINTHREAD pfnTaskIsMainThread;/**< @brief if task is the main thread */
    PFN_ZTASKGETCURRENT pfnTaskGetCurrent;/**< @brief get current  thread id */
    PFN_ZTASKSETTHREADNAME pfnTaskSetThreadName;     /**< @brief set current task thread name */
    PFN_ZTASKGETTHREADNAME pfnTaskGetThreadName;     /**< @brief get current task thread name */
    PFN_ZTASKEQUAL pfnTaskEqual;     /**< @brief compare  thread id */
    PFN_ZTASKINITCRASHREPORT pfnTaskInitCrashReport; /**< @brief init  crash report */
    PFN_ZUIPROCEVNT pfnUiProcEvnt;   /**< @brief if task process */

    /* zos os timer dependence */
    PFN_ZTIMERINIT pfnTimerInit;     /**< @brief timer init */
    PFN_ZTIMERDESTROY pfnTimerDestroy; /**< @brief timer destroy */
    PFN_ZTIMERSTART pfnTimerStart;   /**< @brief timer start */
    PFN_ZTIMERSTOP pfnTimerStop;     /**< @brief timer stop */

    /* zos os http dependence */
    PFN_ZHTTPOPEN pfnHttpOpen;       /**< @brief http craete */
    PFN_ZHTTPCLOSE pfnHttpClose;     /**< @brief http delete */
    PFN_ZHTTPCONN pfnHttpConn;       /**< @brief http connect */
    PFN_ZHTTPDISCONN pfnHttpDisconn; /**< @brief http disconnect */
    PFN_ZHTTPSETPROP pfnHttpSetProp; /**< @brief http set property */
    PFN_ZHTTPSETCFG pfnHttpSetCfg;   /**< @brief http set config */
    PFN_ZHTTPSETBODY pfnHttpSetBody; /**< @brief http set body */
    PFN_ZHTTPSETFILE pfnHttpSetFile; /**< @brief http set file body */
    PFN_ZHTTPGETBODY pfnHttpGetBody; /**< @biref http get body */

    /* zos os ims dependence */
    PFN_ZIMSHANDLECREATECONNECTIONMANAGER pfnImsHandleCreateConnectionManager; /**< @biref ims handle create connection manager */
    PFN_ZIMSHANDLECREATEIMCONNECTION pfnImsHandleCreateIMConnection; /**< @biref ims handle create im connection */
    PFN_ZIMSHANDLESENDIMMESSAGE pfnImsHandleSendIMMessage; /**< @biref ims handle send im message */
    PFN_ZIMSHANDLETRIGGERREGISTRATION pfnImsHandleTriggerRegistration; /**< @biref ims handle trigger registration */
    PFN_ZIMSHANDLETRIGGERDEREGISTRATION pfnImsHandleTriggerDeRegistration; /**< @biref ims handle trigger registration */
    PFN_ZIMSHANDLEAUTOCONFIG pfnImsHandleAutoconfig; /**< @biref ims handle auto config */
    PFN_ZIMSHANDLESETSTATUS pfnImsHandleSetStatus; /**< @biref ims handle set status */
} ST_ZOS_OSDEP_MGR;

#if (ZPLATFORM == ZPLATFORM_WIN32)

#ifdef ZOS_SUPT_STG
    #ifndef ZOS_SUPT_WIN_STG
    #define ZOS_SUPT_WIN_STG
    #endif
#endif
#include "os/windows/zpwin_osdep_prv.h"

#elif ZPLATFORM == ZPLATFORM_LINUX

#ifdef ZOS_SUPT_STG
    #ifndef ZOS_SUPT_LINUX_STG
    #define ZOS_SUPT_LINUX_STG
    #endif
#endif
#include "os/linux/zplin_osdep_prv.h"
    
#elif ZPLATFORM == ZPLATFORM_OSX

#ifdef ZOS_SUPT_STG
    #ifndef ZOS_SUPT_OSX_STG
    #define ZOS_SUPT_OSX_STG
    #endif
#endif    
#include "os/osx/zposx_osdep_prv.h"

#elif ZPLATFORM == ZPLATFORM_ANDROID

#ifdef ZOS_SUPT_STG
    #ifndef ZOS_SUPT_ANDROID_STG
    #define ZOS_SUPT_ANDROID_STG
    #endif
#endif    
#include "os/android/zpand_osdep_prv.h"

#elif ZPLATFORM == ZPLATFORM_IOS

#ifdef ZOS_SUPT_STG
    #ifndef ZOS_SUPT_IOS_STG
    #define ZOS_SUPT_IOS_STG
    #endif
#endif   
#include "os/ios/zpios_osdep_prv.h"

#endif /* ZPLATFORM */

/* zos os dependency attach */
ZINT Zos_OsdepAttach(ZFUNC_VOID);

/* zos os dependency alloc */
ZVOID * Zos_OsdepAlloc(ZINT iSize);

/* zos os dependency free */
ZVOID Zos_OsdepFree(ZVOID *pMem);

/* zos os dependency find */
ZFUNCPTR Zos_OsdepFind(ZINT iFuncId);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_OSDEP_PRV_H__ */

