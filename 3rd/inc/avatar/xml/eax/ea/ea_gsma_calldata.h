/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : ea_gsma_calldata.h
  Module        : eax gsma calldata
  Author        : shuai.xu
  Version       : 0.1
  Created on    : 2017-04-27
  Description   :
    Marcos and structure definitions required by the ea gsma calldata.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_GSMA_CALLDATA_H__
#define _EA_GSMA_CALLDATA_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:gsma:params:xml:ns:rcs:rcs:calldata **************/
typedef enum EN_EA_GSMA_CALLDATA_TYPE
{
    EN_EA_GSMA_CALLDATA_RCSENVELOPE,         /* rcsenvelope */
    EN_EA_GSMA_CALLDATA_RCSCALLDATA,         /* rcscalldata */
    EN_EA_GSMA_CALLDATA_SUBJECT,             /* sbuject */
    EN_EA_GSMA_CALLDATA_IMPORTANCE,         /* importance */
    EN_EA_GSMA_CALLDATA_LOCATION,         /* location */
    EN_EA_GSMA_CALLDATA_COMPOSERID,    /* composerid */
    EN_EA_GSMA_CALLDATA_PIC,            /* pic */
    EN_EA_GSMA_CALLDATA_NOTE,        /* note */
    EN_EA_GSMA_CALLDATA_URL         /* url */
} EN_EA_GSMA_CALLDATA_TYPE;

/* eax gsma calldata get token id from string */
ZFUNC ZINT EaGsmaCallData_TknStr2Id(ST_ZOS_USTR *pstStr, ZTKNID *pzTknId);

/* eax gsma calldata get token string from id */
ZFUNC ZINT EaGsmaCallData_TknId2Str(ZTKNID zTknId, ST_ZOS_USTR *pstStr);

/* eax gsma calldata get the rcsenvelope */
ZFUNC ZINT EaGsmaCallData_GetEnv(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstEnv);

/* eax gsma calldata get the calldata */
ZFUNC ZINT EaGsmaCallData_EnvGetCallData(ST_XML_ELEM *pstEnv, 
                ST_XML_ELEM **ppstCallData);

/* rcs gsma calldata get the subject */
ZFUNC ZINT EaGsmaCallData_CallDataGetSubject(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR **ppstSubject);

/* rcs gsma calldata get the important */
ZFUNC ZINT EaGsmaCallData_CallDataGetImportant(ST_XML_ELEM *pstCallData, 
                ZBOOL *pbImportant);

/* rcs gsma calldata get the location */
ZFUNC ZINT EaGsmaCallData_CallDataGetLocation(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR **ppstLocation);

/* rcs gsma calldata get the picture */
ZFUNC ZINT EaGsmaCallData_CallDataGetPicture(ST_XML_ELEM *pstCallData, 
                ST_XML_ELEM **ppstPicture);

/* eax gsma calldata picture get the url */
ZFUNC ZINT EaGsmaCallData_PictureGetUrl(ST_XML_ELEM *pstPicture, 
                ST_ZOS_USTR **ppstUrl);

/* rcs gsma calldata get the composerid */
ZFUNC ZINT EaGsmaCallData_CallDataGetComposerId(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR **ppstComposerId);

/* rcs gsma calldata get the note */
ZFUNC ZINT EaGsmaCallData_CallDataGetNote(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR **ppstNote);

/* eax gsma calldata set the Env */
ZFUNC ZINT EaGsmaCallData_SetEnv(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstEnv);

/* eax gsma calldata set the rcs call data */
ZFUNC ZINT EaGsmaCallData_EnvSetCallData(ST_XML_ELEM *pstEnv, 
                ST_XML_ELEM **ppstCallData);

/* eax gsma calldata set the subject */
ZFUNC ZINT EaGsmaCallData_CallDataSetSubject(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR *pstSubject);

/* eax gsma calldata set the important */
ZFUNC ZINT EaGsmaCallData_CallDataSetImportant(ST_XML_ELEM *pstCallData, 
                ZBOOL bImportant);

/* eax gsma calldata set the location */
ZFUNC ZINT EaGsmaCallData_CallDataSetLocation(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR *pstLocation);

/* eax gsma calldata set the picture */
ZFUNC ZINT EaGsmaCallData_CallDataSetPicture(ST_XML_ELEM *pstCallData, 
                ST_XML_ELEM **ppstData);

/* eax gsma calldata picture set the url */
ZFUNC ZINT EaGsmaCallData_PictureSetUrl(ST_XML_ELEM *pstPicture, 
                ST_ZOS_USTR *pstUrl);

/* eax gsma calldata set the composerid */
ZFUNC ZINT EaGsmaCallData_CallDataSetComposerId(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR *pstComposerId);

/* eax gsma calldata set the note */
ZFUNC ZINT EaGsmaCallData_CallDataSetNote(ST_XML_ELEM *pstCallData, 
                ST_ZOS_USTR *pstNote);

#ifdef __cplusplus
}
#endif

#endif /* _EA_GSMA_CALLDATA_H__ */

