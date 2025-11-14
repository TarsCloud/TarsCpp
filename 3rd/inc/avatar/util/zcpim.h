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
  File name     : zcpim.h
  Module        : cpim abnf
  Author        : leo.lv 
  Version       : 0.1   
  Created on    : 2011-05-25    
  Description   :
      Data structure and function definitions required by the cpim abnf message.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZCPIM_H__
#define _ZCPIM_H__

#ifdef __cplusplus
extern "C"{
#endif

/* cpim message id (rfc3862) */
typedef ZVOID * ZCPIM;

/* cpim date time (rfc3339) */
typedef struct tagZCPIM_DATE_TIME
{
    ZUSHORT wYear;                   /* date1 year */
    ZUCHAR ucMonth;                  /* date1 month EN_SIP_MONTH */
    ZUCHAR ucDay;                    /* date1 day */
    ZUCHAR ucHour;                   /* time hour */
    ZUCHAR ucMinute;                 /* time minute */
    ZUCHAR ucSecond;                 /* second */
    ZUCHAR ucNegOff;                 /* negative offset */
    ZUCHAR ucOffHour;                /* time offset about hour */
    ZUCHAR ucOffMinute;              /* time offset about  minute */
    ZUCHAR aucSpare[2];              /* for 32 bit alignment */
    ZUINT iSecFrag;                  /* second fragment */
} ST_ZCPIM_DATE_TIME;

/* cpim message create */
ZFUNC ZCPIM Zcpim_Create(ZFUNC_VOID);

/* cpim message delete */
ZFUNC ZVOID Zcpim_Delete(ZCPIM zCpim);

/* cpim message load */
ZFUNC ZINT Zcpim_Load(ZCHAR *pcData, ZUINT iLen, ZCPIM *pzCpim);

/* cpim message save */
ZFUNC ZINT Zcpim_Save(ZCPIM zCpim, ZDBUF *pzData);

/* cpim message pick up header */
ZFUNC ZINT Zcpim_PickHdr(ZCPIM zCpim, ZCHAR *pcName, 
                ST_ZOS_SSTR *pstVal);

/* cpim message pick up header */
ZFUNC ZINT Zcpim_PickHdrX(ZCPIM zCpim, ZCHAR *pcName, 
                ST_ZOS_SSTR *pstVal, ZINT iIndex);

/* cpim message pick up namespace header */
ZFUNC ZINT Zcpim_PickNsHdr(ZCPIM zCpim, ZCHAR *pcNsUri, 
                ZCHAR *pcName, ST_ZOS_SSTR *pstVal);

/* cpim message pick up namespace header */
ZFUNC ZINT Zcpim_PickNsHdrX(ZCPIM zCpim, ZCHAR *pcPrefix, 
                ZUINT iPrefixLen, ZCHAR *pcName, ST_ZOS_SSTR *pstVal);

/* cpim message pick up From header */
ZFUNC ZINT Zcpim_PickHdrFrom(ZCPIM zCpim, ST_ZOS_SSTR *pstName, 
                ST_ZOS_SSTR *pstUri);

/* cpim message pick up To header */
ZFUNC ZINT Zcpim_PickHdrTo(ZCPIM zCpim, ST_ZOS_SSTR *pstName, 
                ST_ZOS_SSTR *pstUri);

/* cpim message pick up cc header */
ZFUNC ZINT Zcpim_PickHdrCc(ZCPIM zCpim, ST_ZOS_SSTR *pstName, 
                ST_ZOS_SSTR *pstUri);

/* cpim message pick up DateTime header */
ZFUNC ZINT Zcpim_PickHdrDateTime(ZCPIM zCpim, 
                ST_ZCPIM_DATE_TIME *pstDateTime);

/* cpim message pick up DateTime header */
ZFUNC ZINT Zcpim_PickHdrDateTimeX(ZCPIM zCpim, 
                ST_ZOS_SYS_TIME *pstDateTime);

/* cpim message pick up Subject header */
ZFUNC ZINT Zcpim_PickHdrSubject(ZCPIM zCpim, ST_ZOS_SSTR *pstLang, 
                ST_ZOS_SSTR *pstSubject);

/* cpim message pick up NS header */
ZFUNC ZINT Zcpim_PickHdrNs(ZCPIM zCpim, ST_ZOS_SSTR *pstPrefix, 
                ST_ZOS_SSTR *pstUri);

/* cpim message pick up Require header */
ZFUNC ZINT Zcpim_PickHdrRequire(ZCPIM zCpim, 
                ST_ZOS_SSTR *pstHdrNames);

/* cpim message pick up content header */
ZFUNC ZINT Zcpim_PickCHdr(ZCPIM zCpim, ZCHAR *pcName, 
                ST_ZOS_SSTR *pstVal);

/* cpim message pick up content header Content-Type type */
ZFUNC ZINT Zcpim_PickCHdrCType(ZCPIM zCpim, 
                ST_ZOS_SSTR *pstMType);

/* cpim message pick up Content-Length header */
ZFUNC ZINT Zcpim_PickCHdrCLen(ZCPIM zCpim, ZUINT *piLength);

/* cpim message pick up content header Content-Type parameter */
ZFUNC ZINT Zcpim_PickCHdrCTypeParm(ZCPIM zCpim, ZCHAR *pcName,
                ST_ZOS_SSTR *pstVal);

/* cpim message pick up content */
ZFUNC ZINT Zcpim_PickContent(ZCPIM zCpim, ST_ZOS_SSTR *pstContent);

/* cpim message pick up content body */
ZFUNC ZINT Zcpim_PickContentBody(ZCPIM zCpim, ST_ZOS_SSTR *pstBody);

/* cpim message fill header */
ZFUNC ZINT Zcpim_FillHdr(ZCPIM zCpim, ZCHAR *pcName, ZCHAR *pcVal);

/* cpim message fill From header */
ZFUNC ZINT Zcpim_FillHdrFrom(ZCPIM zCpim, ZCHAR *pcName, 
                ZCHAR *pcUri);

/* cpim message fill To header */
ZFUNC ZINT Zcpim_FillHdrTo(ZCPIM zCpim, ZCHAR *pcName, 
                ZCHAR *pcUri);

/* cpim message fill cc header */
ZFUNC ZINT Zcpim_FillHdrCc(ZCPIM zCpim, ZCHAR *pcName, 
                ZCHAR *pcUri);

/* cpim message fill DateTime header */
ZFUNC ZINT Zcpim_FillHdrDateTime(ZCPIM zCpim, 
                ST_ZCPIM_DATE_TIME *pstDateTime);

/* cpim message fill DateTime header */
ZFUNC ZINT Zcpim_FillHdrDateTimeX(ZCPIM zCpim, 
                ST_ZOS_SYS_TIME *pstDateTime);

/* cpim message fill Subject header */
ZFUNC ZINT Zcpim_FillHdrSubject(ZCPIM zCpim, ZCHAR *pcLang, 
                ZCHAR *pcSubject);

/* cpim message fill NS header */
ZFUNC ZINT Zcpim_FillHdrNs(ZCPIM zCpim, ZCHAR *pcPrefix, 
                ZCHAR *pcUri);

/* cpim message fill Require header(one or more than one header name) */
ZFUNC ZINT Zcpim_FillHdrRequire(ZCPIM zCpim, ZCHAR *pcHdrNames);

/* cpim message fill header in content */
ZFUNC ZINT Zcpim_FillCHdr(ZCPIM zCpim, ZCHAR *pcName, 
                ZCHAR *pcVal);

/* cpim message fill Content-Type header in content */
ZFUNC ZINT Zcpim_FillCHdrCType(ZCPIM zCpim, ZCHAR *pcVal);

/* cpim message fill At function header in content */
ZFUNC ZINT Zcpim_FillAtMember(ZCPIM zCpim, ZCHAR *pcAtMemUri);

/* cpim message fill Content-Length header in content */
ZFUNC ZINT Zcpim_FillCHdrCLen(ZCPIM zCpim, ZUINT iLen);

/* cpim message fill content */
ZFUNC ZINT Zcpim_FillContent(ZCPIM zCpim, ZCHAR *pcContent);

/* cpim message fill content */
ZFUNC ZINT Zcpim_FillContentD(ZCPIM zCpim, ZDBUF zContent);

/* cpim message fill content body */
ZFUNC ZINT Zcpim_FillContentBody(ZCPIM zCpim, ZCHAR *pcBody);

/* cpim message fill content body */
ZFUNC ZINT Zcpim_FillContentBodyB(ZCPIM zCpim, ZCHAR *pcBody, 
                ZUINT iLen);

/* cpim message fill content body */
ZFUNC ZINT Zcpim_FillContentBodyD(ZCPIM zCpim, ZDBUF zBody);

#ifdef __cplusplus
}
#endif

#endif /* _ZCPIM_H__ */

