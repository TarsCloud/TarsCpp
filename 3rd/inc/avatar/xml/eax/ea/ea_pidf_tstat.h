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
  File name     : ea_pidf_tstat.h
  Module        : eax pidf timed-status
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pidf timed-status.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_TSTAT_H__
#define _EA_PIDF_TSTAT_H__

#ifdef __cplusplus
extern "C"{
#endif

/* pidf timed-status get the timed-status */
ZFUNC ZINT EaPidf_TstatGetTstat(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstTimeStat);

/* pidf timed-status get the basic */
ZFUNC ZINT EaPidf_TstatTstatGetBasic(ST_XML_ELEM *pstTimeStat, 
                ST_XML_ELEM **ppstBasic);

/* pidf timed-status get the note */
ZFUNC ZINT EaPidf_TstatTstatGetNote(ST_XML_ELEM *pstTimeStat, 
                ST_XML_ELEM **ppstNote);

/* pidf timed-status get the from */
ZFUNC ZINT EaPidf_TstatTstatGetFrom(ST_XML_ELEM *pstTimeStat, 
                ST_ZOS_USTR **ppstFrom);

/* pidf timed-status get the until */
ZFUNC ZINT EaPidf_TstatTstatGetUntil(ST_XML_ELEM *pstTimeStat, 
                ST_ZOS_USTR **ppstUntil);

/* pidf timed-status set the timed-status */
ZFUNC ZINT EaPidf_TstatSetTstat(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstTimeStat);

/* pidf timed-status set the basic */
ZFUNC ZINT EaPidf_TstatTstatSetBasic(ST_XML_ELEM *pstTimeStat, 
                ST_XML_ELEM **ppstBasic);

/* pidf timed-status set the note */
ZFUNC ZINT EaPidf_TstatTstatSetNote(ST_XML_ELEM *pstTimeStat, 
                ST_XML_ELEM **ppstNote);

/* pidf timed-status set the from */
ZFUNC ZINT EaPidf_TstatTstatSetFrom(ST_XML_ELEM *pstTimeStat, 
                ST_ZOS_USTR *pstFrom);

/* pidf timed-status set the until */
ZFUNC ZINT EaPidf_TstatTstatSetUntil(ST_XML_ELEM *pstTimeStat, 
                ST_ZOS_USTR *pstUntil);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_TSTAT_H__ */

