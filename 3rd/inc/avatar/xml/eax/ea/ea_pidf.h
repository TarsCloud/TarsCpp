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
  File name     : ea_pidf.h
  Module        : eax pidf
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pidf.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_H__
#define _EA_PIDF_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:ietf:params:xml:ns:pidf **************/
typedef enum EN_EA_PIDF_TYPE
{
    EN_EA_PIDF_PRES,              /* presence */
    EN_EA_PIDF_TUPLE,             /* tuple */
    EN_EA_PIDF_NOTE,              /* note */
    EN_EA_PIDF_ENT,               /* entity */
    EN_EA_PIDF_STAT,              /* status */
    EN_EA_PIDF_CONTACT,           /* contact */
    EN_EA_PIDF_TIMESTAMP,         /* timestamp */
    EN_EA_PIDF_ID,                /* id */
    EN_EA_PIDF_BASIC,             /* basic */
    EN_EA_PIDF_OPEN,              /* open */
    EN_EA_PIDF_CLOSED,            /* closed */
    EN_EA_PIDF_PRIORITY,          /* priority */
    EN_EA_PIDF_QVAL,              /* qvalue */
    EN_EA_PIDF_MUSTUNDERSTAND     /* mustUnderstand */
} EN_EA_PIDF_TYPE;

/* pidf answer-mode type */
typedef enum EN_EA_PIDF_BASIC_TYPE
{
    EN_EA_PIDF_BASIC_OPEN,        
    EN_EA_PIDF_BASIC_CLOSE
} EN_EA_PIDF_BASIC_TYPE;

/* pidf get the presence */
ZFUNC ZINT EaPidf_GetPres(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstPres);

/* pidf get the first presence tuple */
ZFUNC ZINT EaPidf_PresGetFirstTuple(ST_XML_ELEM *pstPres, 
                ST_XML_ELEM **ppstTuple);

/* pidf get the next presence tuple */
ZFUNC ZINT EaPidf_PresGetNextTuple(ST_XML_ELEM *pstTuple, 
                ST_XML_ELEM **ppstTuple);

/* pidf get the first presence note */
ZFUNC ZINT EaPidf_PresGetFirstNote(ST_XML_ELEM *pstPres, 
                ST_XML_ELEM **ppstNote);

/* pidf get the next presence note */
ZFUNC ZINT EaPidf_PresGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf get the presence entity */
ZFUNC ZINT EaPidf_PresGetEnt(ST_XML_ELEM *pstPres, 
                ST_ZOS_USTR **ppstEnt);

/* pidf get the tuple status */
ZFUNC ZINT EaPidf_TupleGetStat(ST_XML_ELEM *pstTuple, 
                ST_XML_ELEM **ppstStat);

/* pidf get the tuple contact */
ZFUNC ZINT EaPidf_TupleGetContact(ST_XML_ELEM *pstTuple, 
                ST_XML_ELEM **ppstContact);

/* pidf get the first tuple note */
ZFUNC ZINT EaPidf_TupleGetFirstNote(ST_XML_ELEM *pstTuple, 
                ST_XML_ELEM **ppstNote);

/* pidf get the next tuple note */
ZFUNC ZINT EaPidf_TupleGetNextNote(ST_XML_ELEM *pstNote, 
                ST_XML_ELEM **ppstNote);

/* pidf get the tuple timestamp */
ZFUNC ZINT EaPidf_TupleGetTimestamp(ST_XML_ELEM *pstTuple, 
                ST_ZOS_USTR **ppstTimestamp);

/* pidf get the tuple id */
ZFUNC ZINT EaPidf_TupleGetId(ST_XML_ELEM *pstTuple, 
                ST_ZOS_USTR **ppstId);

/* pidf get the status basic */
ZFUNC ZINT EaPidf_StatGetBasic(ST_XML_ELEM *pstStat, 
                ST_XML_ELEM **ppstBasic);

/* pidf get the basic type EN_EA_PIDF_BASIC_TYPE*/
ZFUNC ZINT EaPidf_BasicGetType(ST_XML_ELEM *pstBasic, ZUCHAR *pucType);

/* pidf get the contact priority */
ZFUNC ZINT EaPidf_ContactGetPriority(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR **ppstPriority);

/* pidf get the note lang */
ZFUNC ZINT EaPidf_NoteGetLang(ST_XML_ELEM *pstNote, 
                ST_ZOS_USTR **ppstLang);

/* pidf get the mustUnderstand */
ZFUNC ZINT EaPidf_GetMustUnderstand(ST_XML_ELEM *pstElem,  
                ZBOOL *pbExist);

/* pidf set the presence */
ZFUNC ZINT EaPidf_SetPres(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstPres);

/* pidf set the presence tuple */
ZFUNC ZINT EaPidf_PresSetTuple(ST_XML_ELEM *pstPres, 
                ST_XML_ELEM **ppstTuple);

/* pidf set the presence note */
ZFUNC ZINT EaPidf_PresSetNote(ST_XML_ELEM *pstPres, 
                ST_XML_ELEM **ppstNote);

/* pidf set the presence entity */
ZFUNC ZINT EaPidf_PresSetEnt(ST_XML_ELEM *pstPres, 
                ST_ZOS_USTR *pstEnt);

/* pidf set the tuple status */
ZFUNC ZINT EaPidf_TupleSetStat(ST_XML_ELEM *pstTuple, 
                ST_XML_ELEM **ppstStat);

/* pidf set the tuple contact */
ZFUNC ZINT EaPidf_TupleSetContact(ST_XML_ELEM *pstTuple, 
                ST_XML_ELEM **ppstContact);

/* pidf set the tuple note */
ZFUNC ZINT EaPidf_TupleSetNote(ST_XML_ELEM *pstTuple, 
                ST_XML_ELEM **ppstNote);

/* pidf set the tuple timestamp */
ZFUNC ZINT EaPidf_TupleSetTimestamp(ST_XML_ELEM *pstTuple, 
                ST_ZOS_USTR *pstTimestamp);

/* pidf set the tuple id */
ZFUNC ZINT EaPidf_TupleSetId(ST_XML_ELEM *pstTuple, 
                ST_ZOS_USTR *pstId);

/* pidf set the status basic */
ZFUNC ZINT EaPidf_StatSetBasic(ST_XML_ELEM *pstStat, 
                ST_XML_ELEM **ppstBasic);

/* pidf set the basic type EN_EA_PIDF_BASIC_TYPE*/
ZFUNC ZINT EaPidf_BasicSetType(ST_XML_ELEM *pstBasic, ZUCHAR ucType);

/* pidf set the contact priority */
ZFUNC ZINT EaPidf_ContactSetPriority(ST_XML_ELEM *pstContact, 
                ST_ZOS_USTR *pstPriority);

/* pidf set the note lang */
ZFUNC ZINT EaPidf_NoteSetLang(ST_XML_ELEM *pstNote, 
                ST_ZOS_USTR *pstLang);

/* pidf set the mustUnderstand */
ZFUNC ZINT EaPidf_SetMustUnderstand(ST_XML_ELEM *pstElem, 
                ZBOOL bExist);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_H__ */

