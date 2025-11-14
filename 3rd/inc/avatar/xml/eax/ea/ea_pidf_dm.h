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
  File name     : ea_pidf_dm.h
  Module        : eax pidf dm
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pidf dm.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_DM_H__
#define _EA_PIDF_DM_H__

#ifdef __cplusplus
extern "C"{
#endif

/********* urn:ietf:params:xml:ns:pidf:data-model ********/
typedef enum EN_EA_PIDF_DM_TYPE
{
    EN_EA_PIDF_DM_DEVICEID,       /* deviceID */
    EN_EA_PIDF_DM_DEVICE,         /* device */
    EN_EA_PIDF_DM_NOTE,           /* note */
    EN_EA_PIDF_DM_TIMESTAMP,      /* timestamp */
    EN_EA_PIDF_DM_ID,             /* id */
    EN_EA_PIDF_DM_PERSON          /* person */
} EN_EA_PIDF_DM_TYPE;

/* pidf data-model get the deviceID */
ZFUNC ZINT EaPidf_DmGetDevId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstDevId);

/* pidf data-model get the device */
ZFUNC ZINT EaPidf_DmGetDev(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstDev);

/* pidf data-model get the device deviceID */
ZFUNC ZINT EaPidf_DmDevGetDevID(ST_XML_ELEM *pstDev, 
                ST_XML_ELEM **ppstDevId);

/* pidf get the first tuple note */
ZFUNC ZINT EaPidf_DmDevGetFirstNote(ST_XML_ELEM *pstDev, 
                ST_XML_ELEM **ppstNote);

/* pidf get the next tuple note */
ZFUNC ZINT EaPidf_DmDevGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf data-model get the device timestamp */
ZFUNC ZINT EaPidf_DmDevGetTimestamp(ST_XML_ELEM *pstDev, 
                ST_ZOS_USTR **ppstTimestamp);

/* pidf data-model get the device id */
ZFUNC ZINT EaPidf_DmDevGetId(ST_XML_ELEM *pstDev, 
                ST_ZOS_USTR **ppstId);

/* pidf data-model get the person */
ZFUNC ZINT EaPidf_DmGetPerson(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPerson);

/* pidf data-model get the first person */
ZFUNC ZINT EaPidf_DmGetFirstPerson(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPerson);

/* pidf data-model get the next person */
ZFUNC ZINT EaPidf_DmGetNextPerson(ST_XML_ELEM *pstPerson, 
                ST_XML_ELEM **ppstPerson);

/* pidf data-model get the first person note */
ZFUNC ZINT EaPidf_DmPersonGetFirstNote(ST_XML_ELEM *pstPerson, 
                ST_XML_ELEM **ppstNote);

/* pidf data-model get the next person note */
ZFUNC ZINT EaPidf_DmPersonGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf data-model get the person timestamp */
ZFUNC ZINT EaPidf_DmPersonGetTimestamp(ST_XML_ELEM *pstPerson, 
                ST_ZOS_USTR **ppstTimestamp);

/* pidf data-model get the person id */
ZFUNC ZINT EaPidf_DmPersonGetId(ST_XML_ELEM *pstPerson, 
                ST_ZOS_USTR **ppstId);

/* pidf data-model set the deviceID */
ZFUNC ZINT EaPidf_DmSetDevId(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstDevId);

/* pidf data-model set the device */
ZFUNC ZINT EaPidf_DmSetDev(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstDev);

/* pidf data-model set the device deviceID */
ZFUNC ZINT EaPidf_DmDevSetDevID(ST_XML_ELEM *pstDev, 
                ST_XML_ELEM **ppstDevId);

/* pidf set the tuple note */
ZFUNC ZINT EaPidf_DmDevSetNote(ST_XML_ELEM *pstDev, 
                ST_XML_ELEM **ppstNote);

/* pidf data-model set the device timestamp */
ZFUNC ZINT EaPidf_DmDevSetTimestamp(ST_XML_ELEM *pstDev, 
                ST_ZOS_USTR *pstTimestamp);

/* pidf data-model set the device id */
ZFUNC ZINT EaPidf_DmDevSetId(ST_XML_ELEM *pstDev, 
                ST_ZOS_USTR *pstId);

/* pidf data-model set the person */
ZFUNC ZINT EaPidf_DmSetPerson(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstPerson);

/* pidf data-model set the person note */
ZFUNC ZINT EaPidf_DmPersonSetNote(ST_XML_ELEM *pstPerson, 
                ST_XML_ELEM **ppstNote);

/* pidf data-model set the person timestamp */
ZFUNC ZINT EaPidf_DmPersonSetTimestamp(ST_XML_ELEM *pstPerson, 
                ST_ZOS_USTR *pstTimestamp);

/* pidf data-model set the person id */
ZFUNC ZINT EaPidf_DmPersonSetId(ST_XML_ELEM *pstPerson, 
                ST_ZOS_USTR *pstId);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_DM_H__ */

