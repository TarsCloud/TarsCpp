/************************************************************************

        Copyright (c) 2005-2011 by Juphoon System Software, Inc.
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
  File name     : zos_evnt.h
  Module        : zos event
  Author        : leo.lv   
  Version       : 0.1   
  Created on    : 2011-11-02    
  Description   :
      Data structure and function definitions required by the zos event.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_EVNT_H__
#define _ZOS_EVNT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* mnf evnt type */
#define ZEVNT_END       0
#define ZEVNT_BOOL      1
#define ZEVNT_UINT      2
#define ZEVNT_ULONG     3
#define ZEVNT_USHORT    4
#define ZEVNT_UCHAR     5
#define ZEVNT_STR       6
#define ZEVNT_POINTER   7
#define ZEVNT_MEM       8
#define ZEVNT_ADDR      9
#define ZEVNT_COOKIE    10

/** @brief Make a paramenter ID. High word is environment ID, low word is name ID. */
#define ZPARMID_MAKE(_envid, _nameid) ZOS_MAKE_DWORD((_envid), (_nameid))

/**
 * @brief Create a new event.
 *
 * @return New event ID, or return ZNULL.
 *
 * @see @ref Zos_EvntDelete
 */
ZFUNC ZEVNT Zos_EvntCreate(ZFUNC_VOID);

/**
 * @brief Create a new event.
 *
 * @return New event ID, or return ZNULL.
 *
 * @see @ref Zos_EvntDelete
 */
ZFUNC ZEVNT Zos_EvntCreateX(ZCBUF zBuf);

/**
 * @brief Create a new event with format.
 *
 * @param pucFmt Array of format type @ref ZEVNT_BOOL ..., 
                 which end with @ref ZEVNT_END. The type of arguments after the
                 first must satisfy by following rules:
                 - ZEVNT_BOOL       ZBOOL
                 - ZEVNT_UINT       ZUINT
                 - ZEVNT_ULONG      ZULONG
                 - ZEVNT_USHORT     ZUSHORT
                 - ZEVNT_STR        ZCHAR *
                 - ZEVNT_POINTER    ZVOID *
                 - ZEVNT_MEM        ZVOID * and ZUINT
                 - ZEVNT_ADDR       ST_ZOS_INET_ADDR
                 - ZEVNT_COOKIE     ZCOOKIE
 *
 * @return New event ID, or return ZNULL.
 *
 * @see @ref Zos_EvntDelete
 */
ZFUNC ZEVNT Zos_EvntCreateFmt(ZUCHAR *pucFmt, ...);

/**
 * @brief Create a new event with format.
 *
 * @param pucFmt Array of format type @ref ZEVNT_BOOL ..., 
                 which end with @ref ZEVNT_END. The type of arguments after the
                 first must satisfy by following rules:
                 - ZEVNT_BOOL       ZBOOL
                 - ZEVNT_UINT       ZUINT
                 - ZEVNT_ULONG      ZULONG
                 - ZEVNT_USHORT     ZUSHORT
                 - ZEVNT_STR        ZCHAR *
                 - ZEVNT_POINTER    ZVOID *
                 - ZEVNT_MEM        ZVOID * and ZUINT
                 - ZEVNT_ADDR       ST_ZOS_INET_ADDR
                 - ZEVNT_COOKIE     ZCOOKIE
 * @param zAp Parameter list.
 *
 * @return New event ID, or return ZNULL.
 *
 * @see @ref Zos_EvntCreateFmt, Zos_EvntDelete
 */
ZFUNC ZEVNT Zos_EvntCreateFmtX(ZUCHAR *pucFmt, ZVA_LIST zAp);

/**
 * @brief Create a new event with format.
 *
 * @param pucFmt String of format type @ref %hhd ..., 
                 which end with '\0'. The type of arguments after the
                 first must satisfy by following rules:
                 - %hhd             ZCHAR
                 - %hhu             ZUCHAR
                 - %hd              ZSHORT
                 - %hu              ZUSHORT
                 - %d               ZINT
                 - %u               ZUINT(ZBOOL)
                 - %ld              ZLONG
                 - %lu              ZULONG
                 - %s               ZCHAR *
                 - %p               ZVOID *
                 - %*p              ZVOID * and ZUINT
                 - %zu              ZCOOKIE
 *
 * @return New event ID, or return ZNULL.
 *
 * @see @ref Zos_EvntDelete
 */
ZFUNC ZEVNT Zos_EvntCreateFmtS(ZUINT iStartId, ZCONST ZCHAR *pucFmt, ...) ZATTR_PRINTF(2, 3);

/**
 * @brief Create a new event with format.
 *
 * @param pucFmt String of format type @ref %hhd ..., 
                 which end with '\0'. The type of arguments after the
                 first must satisfy by following rules:
                 - %hhd             ZCHAR
                 - %hhu             ZUCHAR
                 - %hd              ZSHORT
                 - %hu              ZUSHORT
                 - %d               ZINT
                 - %u               ZUINT(ZBOOL)
                 - %ld              ZLONG
                 - %lu              ZULONG
                 - %s               ZCHAR *
                 - %p               ZVOID *
                 - %*p              ZVOID * and ZUINT
                 - %zu              ZCOOKIE
 *
 * @return New event ID, or return ZNULL.
 *
 * @see @ref Zos_EvntDelete
 */
ZFUNC ZEVNT Zos_EvntCreateFmtSX(ZUINT iStartId, ZCONST ZCHAR *pcFmt, ZVA_LIST zAp);

/**
 * @brief Clone a event.
 *
 * @return Event ID when succeed, or return ZNULL.
 *
 * @see @ref Zos_EvntDelete
 */
ZFUNC ZEVNT Zos_EvntClone(ZEVNT zEvnt);

/**
 * @brief Delete an event.
 *
 * @param [in] zEvnt Event to be deleted.
 *
 * @return [none].
 *
 * @see @ref Zos_EvntCreate
 */
ZFUNC ZVOID Zos_EvntDelete(ZEVNT zEvnt);

/**
 * @brief Read data from event.
 *
 * @param zEvnt Event to read from.
 * @param pucFmt Array of format type @ref ZEVNT_BOOL ..., 
                 which end with @ref ZEVNT_END. The type of arguments after the
                 first must satisfy by following rules:
                 - ZEVNT_BOOL       ZBOOL *
                 - ZEVNT_UINT       ZUINT *
                 - ZEVNT_ULONG      ZULONG *
                 - ZEVNT_USHORT     ZUSHORT *
                 - ZEVNT_STR        ZCHAR **
                 - ZEVNT_POINTER    ZVOID **
                 - ZEVNT_MEM        ZVOID ** and ZUINT *
                 - ZEVNT_ADDR       ST_ZOS_INET_ADDR *
                 - ZEVNT_COOKIE     ZCOOKIE *
 *
 * @return ZOK for read successfully, otherwise for failed.
 */
ZFUNC ZINT Zos_EvntRead(ZEVNT zEvnt, ZUCHAR *pucFmt, ...);

/**
 * @brief Read data from event.
 *
 * @param zEvnt Event to read from.
 * @param pucFmt Array of format type @ref ZEVNT_BOOL ..., 
                 which end with @ref ZEVNT_END. The type of arguments after the
                 first must satisfy by following rules:
                 - ZEVNT_BOOL       ZBOOL *
                 - ZEVNT_UINT       ZUINT *
                 - ZEVNT_ULONG      ZULONG *
                 - ZEVNT_USHORT     ZUSHORT *
                 - ZEVNT_STR        ZCHAR **
                 - ZEVNT_POINTER    ZVOID **
                 - ZEVNT_MEM        ZVOID ** and ZUINT *
                 - ZEVNT_ADDR       ST_ZOS_INET_ADDR *
                 - ZEVNT_COOKIE     ZCOOKIE *
 * @param zAp Parameter list.
 *
 * @return ZOK for read successfully, otherwise for failed.
 */
ZFUNC ZINT Zos_EvntReadX(ZEVNT zEvnt, ZUCHAR *pucFmt, ZVA_LIST zAp);

/**
 * @brief Read data from event.
 *
 * @param zEvnt Event to read from.
 * @param pucFmt String of format type @ref %hhd ..., 
                 which end with '\0'. The type of arguments after the
                 first must satisfy by following rules:
                 - %hhd             ZCHAR
                 - %hhu             ZUCHAR
                 - %hd              ZSHORT
                 - %hu              ZUSHORT
                 - %d               ZINT
                 - %u               ZUINT(ZBOOL)
                 - %ld              ZLONG
                 - %lu              ZULONG
                 - %s               ZCHAR *
                 - %p               ZVOID *
                 - %*p              ZVOID * and ZUINT
                 - %zu              ZCOOKIE

 *
 * @return ZOK for read successfully, otherwise for failed.
 */
ZFUNC ZINT Zos_EvntReadS(ZEVNT zEvnt, ZCONST ZCHAR *pucFmt, ...) ZATTR_PRINTF(2, 3);

/**
 * @brief Read data from event.
 *
 * @param zEvnt Event to read from.
 * @param pucFmt String of format type @ref %hhd ..., 
                 which end with '\0'. The type of arguments after the
                 first must satisfy by following rules:
                 - %hhd             ZCHAR
                 - %hhu             ZUCHAR
                 - %hd              ZSHORT
                 - %hu              ZUSHORT
                 - %d               ZINT
                 - %u               ZUINT(ZBOOL)
                 - %ld              ZLONG
                 - %lu              ZULONG
                 - %s               ZCHAR *
                 - %p               ZVOID *
                 - %*p              ZVOID * and ZUINT
                 - %zu              ZCOOKIE

 *
 * @return ZOK for read successfully, otherwise for failed.
 */
ZFUNC ZINT Zos_EvntReadSX(ZEVNT zEvnt, ZCONST ZCHAR *pucFmt, ZVA_LIST zAp);

/**
 * @brief Allocate memory from event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see @ref Zos_EvntAllocClrd
 */
ZFUNC ZVOID * Zos_EvntAlloc(ZEVNT zEvnt, ZUINT iSize);

/**
 * @brief Allocate memory cleared by 0 from event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see @ref Zos_EvntAlloc
 */
ZFUNC ZVOID * Zos_EvntAllocClrd(ZEVNT zEvnt, ZUINT iSize);

/**
 * @brief Get ZOS message id from event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @return ZOS message id if successfully, or return ZNULL.
 *
 * @see @ref Zos_EvntSetMsgId
 */
ZFUNC ZMSG Zos_EvntGetMsgId(ZEVNT zEvnt);

/**
 * @brief Set ZOS message id into event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] zMsgId Message id.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EvntGetMsgId
 */
ZFUNC ZINT Zos_EvntSetMsgId(ZEVNT zEvnt, ZMSG zMsgId);

/**
 * @brief Erase a parameter from event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EvntAlloc
 */
ZFUNC ZINT Zos_EParmErase(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Get ZCHAR type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value
 */
ZFUNC ZCHAR Zos_EParmGetChar(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZCHAR type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] cParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetUchar
 */
ZFUNC ZINT Zos_EParmSetChar(ZEVNT zEvnt, ZUINT iParmId, 
                ZCHAR cParmVal);

/**
 * @brief Get ZUCHAR type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value
 */
ZFUNC ZUCHAR Zos_EParmGetUchar(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZUCHAR type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] ucParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetChar
 */
ZFUNC ZINT Zos_EParmSetUchar(ZEVNT zEvnt, ZUINT iParmId, 
                ZUCHAR ucParmVal);

/**
 * @brief Get ZSHORT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetUshort
 */
ZFUNC ZSHORT Zos_EParmGetShort(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZSHORT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] wParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetUshort
 */
ZFUNC ZINT Zos_EParmSetShort(ZEVNT zEvnt, ZUINT iParmId, 
                ZSHORT wParmVal);

/**
 * @brief Get ZUSHORT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetShort
 */
ZFUNC ZUSHORT Zos_EParmGetUshort(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZUSHORT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] wParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetShort
 */
ZFUNC ZINT Zos_EParmSetUshort(ZEVNT zEvnt, ZUINT iParmId, 
                ZUSHORT wParmVal);

/**
 * @brief Get ZINT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetUint
 */
ZFUNC ZINT Zos_EParmGetInt(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZINT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] iParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetUint
 */
ZFUNC ZINT Zos_EParmSetInt(ZEVNT zEvnt, ZUINT iParmId, 
                ZINT iParmVal);

/**
 * @brief Get ZUINT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetInt
 */
ZFUNC ZUINT Zos_EParmGetUint(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZUINT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] iParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetInt
 */
ZFUNC ZINT Zos_EParmSetUint(ZEVNT zEvnt, ZUINT iParmId, 
                ZUINT iParmVal);

/**
 * @brief Get ZLONG type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetUlong
 */
ZFUNC ZLONG Zos_EParmGetLong(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZLONG type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] dwParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetUlong
 */
ZFUNC ZINT Zos_EParmSetLong(ZEVNT zEvnt, ZUINT iParmId, 
                ZLONG dwParmVal);

/**
 * @brief Get ZULONG type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetLong
 */
ZFUNC ZULONG Zos_EParmGetUlong(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZULONG type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] dwVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetLong
 */
ZFUNC ZINT Zos_EParmSetUlong(ZEVNT zEvnt, ZUINT iParmId, 
                ZULONG dwParmVal);

/**
 * @brief Get ZINT64 type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetUint64
 */
ZFUNC ZINT64 Zos_EParmGetInt64(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZINT type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] qwParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetUint64
 */
ZFUNC ZINT Zos_EParmSetInt64(ZEVNT zEvnt, ZUINT iParmId, 
                ZINT64 qwParmVal);

/**
 * @brief Get ZUINT64 type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetInt64
 */
ZFUNC ZUINT64 Zos_EParmGetUint64(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZUINT64 type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] iParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetInt64
 */
ZFUNC ZINT Zos_EParmSetUint64(ZEVNT zEvnt, ZUINT iParmId, 
                ZUINT64 qwParmVal);

/**
 * @brief Get ZBOOL type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetUint
 */
ZFUNC ZBOOL Zos_EParmGetBool(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZBOOL type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] bVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetBool
 */
ZFUNC ZINT Zos_EParmSetBool(ZEVNT zEvnt, ZUINT iParmId, 
                ZBOOL bParmVal);

/**
 * @brief Get ZCHAR type string parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetUstr
 */
ZFUNC ZCHAR * Zos_EParmGetStr(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZCHAR type string of event parameter.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] pcParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetUstr
 */
ZFUNC ZINT Zos_EParmSetStr(ZEVNT zEvnt, ZUINT iParmId, 
                ZCONST ZCHAR *pcParmVal);

/**
 * @brief Get ZCHAR type string parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [out] piSize Parameter value size.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetNUStr
 */
ZFUNC ZCHAR * Zos_EParmGetNStr(ZEVNT zEvnt, ZUINT iParmId, 
                    ZUINT *piSize);

/**
 * @brief Set a ZCHAR type string of event parameter.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] pcParmVal Parameter value.
 * @param [in] iSize Parameter value size.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetNUStr
 */
ZFUNC ZINT Zos_EParmSetNStr(ZEVNT zEvnt, ZUINT iParmId, 
                    ZCHAR *pcParmVal, ZUINT iSize);

/**
 * @brief Get ZCHAR type string parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [out] dwSize Parameter value size.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetNUStr
 */
ZFUNC ZUCHAR * Zos_EParmGetNUStr(ZEVNT zEvnt, ZUINT iParmId, 
                ZUINT *piSize);

/**
 * @brief Set a ZCHAR type string of event parameter.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] pucParmVal Parameter value.
 * @param [in] dwSize Parameter value size.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetNUStr
 */
ZFUNC ZINT Zos_EParmSetNUStr(ZEVNT zEvnt, ZUINT iParmId, 
                    ZUCHAR *pucParmVal, ZUINT iSize);

/**
 * @brief Get ZUCHAR type string parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetStr
 */
ZFUNC ZUCHAR * Zos_EParmGetUstr(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a ZUCHAR type string of event parameter.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] pcParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetStr
 */
ZFUNC ZINT Zos_EParmSetUstr(ZEVNT zEvnt, ZUINT iParmId, 
                ZUCHAR *pucParmVal);

/**
 * @brief Get memory address parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [out] piMemSize Parameter memory size.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmGetUint
 */
ZFUNC ZVOID * Zos_EParmGetMem(ZEVNT zEvnt, ZUINT iParmId,
                ZUINT *piMemSize);

/**
 * @brief Set a memory address of event parameter.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] pParmVal Parameter value.
 * @param [in] iMemSize Parameter memory size.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetStr
 */
ZFUNC ZINT Zos_EParmSetMem(ZEVNT zEvnt, ZUINT iParmId, 
                ZVOID *pParmVal, ZUINT iMemSize);

/**
 * @brief Get pointer type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmSetPtr
 */
ZFUNC ZVOID * Zos_EParmGetPtr(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a pointer type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] zParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetPtr
 */
ZFUNC ZINT Zos_EParmSetPtr(ZEVNT zEvnt, ZUINT iParmId, 
                ZVOID * zParmVal);

/**
 * @brief Get pointer type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmSetCookie
 */
ZFUNC ZCOOKIE Zos_EParmGetCookie(ZEVNT zEvnt, ZUINT iParmId);

/**
 * @brief Set a pointer type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iParmId Parameter ID.
 * @param [in] zParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetCookie
 */
ZFUNC ZINT Zos_EParmSetCookie(ZEVNT zEvnt, ZUINT iParmId, 
                ZCOOKIE zParmVal);

/**
 * @brief Get send task id of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @return Send task id if successfully, or return ZMAXTASKID.
 *
 * @see @ref Zos_EParmGetRecvTaskId
 */
 
ZFUNC ZTASKID Zos_EParmGetSendTaskId(ZEVNT zEvnt);

/**
 * @brief Get received task id of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @return Received task id if successfully, or return ZMAXTASKID.
 *
 * @see @ref Zos_EParmGetSendTaskId
 */
ZFUNC ZTASKID Zos_EParmGetRecvTaskId(ZEVNT zEvnt);

/**
 * @brief Get major type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value if successfully, or return ZMAXUINT
 *
 * @see @ref Zos_EParmSetMajorType
 */
ZFUNC ZUINT Zos_EParmGetMajorType(ZEVNT zEvnt);

/**
 * @brief Set major type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iType Major type value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetMajorType
 */
ZFUNC ZINT Zos_EParmSetMajorType(ZEVNT zEvnt, ZUINT iType);

/**
 * @brief Get minor type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value if successfully, or return ZMAXUINT
 *
 * @see @ref Zos_EParmSetMinorType
 */
ZFUNC ZUINT Zos_EParmGetMinorType(ZEVNT zEvnt);

/**
 * @brief Set minor type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iType Minor type value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetMinorType
 */
ZFUNC ZINT Zos_EParmSetMinorType(ZEVNT zEvnt, ZUINT iType);

/**
 * @brief Get status type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value if successfully, or return ZMAXUINT
 *
 * @see @ref Zos_EParmSetStatType
 */
ZFUNC ZUINT Zos_EParmGetStatType(ZEVNT zEvnt);

/**
 * @brief Set status type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iType Status type value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetStatType
 */
ZFUNC ZINT Zos_EParmSetStatType(ZEVNT zEvnt, ZUINT iType);

/**
 * @brief Get status code parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value if successfully, or return 0
 *
 * @see @ref Zos_EParmSetStatCode
 */
ZFUNC ZUINT Zos_EParmGetStatCode(ZEVNT zEvnt);

/**
 * @brief Set status type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iCode Status code value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetStatCode
 */
ZFUNC ZINT Zos_EParmSetStatCode(ZEVNT zEvnt, ZUINT iCode);

/**
 * @brief Get element bool parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value if successfully, or return ZFALSE
 *
 * @see @ref Zos_EParmSetElemBool
 */
ZFUNC ZBOOL Zos_EParmGetElemBool(ZEVNT zEvnt);

/**
 * @brief Set element bool parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] bBool element bool value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmSetElemBool
 */
ZFUNC ZINT Zos_EParmSetElemBool(ZEVNT zEvnt, ZBOOL bBool);

/**
 * @brief Get element id parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value if successfully, or return 0
 *
 * @see @ref Zos_EParmSetElemId
 */
ZFUNC ZUINT Zos_EParmGetElemId(ZEVNT zEvnt);

/**
 * @brief Set element id parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] iId Element id value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetElemId
 */
ZFUNC ZINT Zos_EParmSetElemId(ZEVNT zEvnt, ZUINT iId);

/**
 * @brief Get element memory parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value
 *
 * @see @ref Zos_EParmSetElemMem
 */
ZFUNC ZVOID * Zos_EParmGetElemMem(ZEVNT zEvnt);

/**
 * @brief Set element memory parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] dwId Element id value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetElemMem
 */
ZFUNC ZINT Zos_EParmSetElemMem(ZEVNT zEvnt, ZVOID *pMem);

/**
 * @brief Get element data parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] piSize  Element data size.
 *
 * @retval Parameter value
 *
 * @see @ref Zos_EParmSetElemData
 */
ZFUNC ZVOID * Zos_EParmGetElemData(ZEVNT zEvnt, ZUINT *piSize);

/**
 * @brief Set element data parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] pData Element data value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetElemData
 */
ZFUNC ZINT Zos_EParmSetElemData(ZEVNT zEvnt, ZVOID *pData, 
                ZUINT iSize);

/**
 * @brief Set element data parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] zData Element data value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetElemData
 */
ZFUNC ZINT Zos_EParmSetElemDataX(ZEVNT zEvnt, ZDBUF zData);

/**
 * @brief Get element data parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value if successfully, or return ZNULL.
 *
 * @see @ref Zos_EParmSetElemData
 */
ZFUNC ZDBUF Zos_EParmGetElemDbuf(ZEVNT zEvnt);

/**
 * @brief Set element data parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] zBufId Element data buffer value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetElemData
 */
ZFUNC ZINT Zos_EParmSetElemDbuf(ZEVNT zEvnt, ZDBUF zBuf);

/**
 * @brief Clear element data buffer parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetElemData
 */
ZFUNC ZINT Zos_EParmClrElemDbuf(ZEVNT zEvnt);

/**
 * @brief Get pointer type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 *
 * @retval Parameter value.
 *
 * @see @ref Zos_EParmSetElemCookie
 */
ZFUNC ZCOOKIE Zos_EParmGetElemCookie(ZEVNT zEvnt);

/**
 * @brief Set a pointer type parameter value of event.
 *
 * @param [in] zEvnt Event ID.
 * @param [in] zParmVal Parameter value.
 *
 * @retval ZOK Set successfully.
 * @retval ZFAILED Set failed.
 *
 * @see @ref Zos_EParmGetElemCookie
 */
ZFUNC ZINT Zos_EParmSetElemCookie(ZEVNT zEvnt, ZCOOKIE zParmVal);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_EVNT_H__ */

