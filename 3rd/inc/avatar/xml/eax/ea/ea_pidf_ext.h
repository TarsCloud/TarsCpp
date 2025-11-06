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
  File name     : ea_pidf_ext.h
  Module        : eax pde pidf ext
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pde pidf ext.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_EXT_H__
#define _EA_PIDF_EXT_H__

#ifdef __cplusplus
extern "C"{
#endif

/* pde pidf ext get session-answermode */
ZFUNC ZINT EaPidf_ExtGetSess(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSess);

/* pde pidf ext session-answermode get automatic */
ZFUNC ZINT EaPidf_ExtSessGetAuto(ST_XML_ELEM *pstSess, 
                ST_XML_ELEM **ppstAuto);

/* pde pidf ext session-answermode get manual */
ZFUNC ZINT EaPidf_ExtSessGetManu(ST_XML_ELEM *pstSess, 
                ST_XML_ELEM **ppstManu);

/* pde pidf ext get home */
ZFUNC ZINT EaPidf_ExtGetHome(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstHome);

/* pde pidf ext get visited */
ZFUNC ZINT EaPidf_ExtGetVisited(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstVisited);

/* pde pidf ext get content type */
ZFUNC ZINT EaPidf_ExtGetCttType(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstCtt);

/* pde pidf ext get etag */
ZFUNC ZINT EaPidf_ExtGetEtag(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstEtag);

/* pde pidf ext get fsize */
ZFUNC ZINT EaPidf_ExtGetFsize(ST_XML_ELEM *pstElem, 
                ZUINT *piFsize);

/* pde pidf ext get resolution */
ZFUNC ZINT EaPidf_ExtGetReso(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstReso);

/* pde pidf ext set session-answermode */
ZFUNC ZINT EaPidf_ExtSetSess(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstSess);

/* pde pidf ext session-answermode set automatic */
ZFUNC ZINT EaPidf_ExtSessSetAuto(ST_XML_ELEM *pstSess, 
                ST_XML_ELEM **ppstAuto);

/* pde pidf ext session-answermode set manual */
ZFUNC ZINT EaPidf_ExtSessSetManu(ST_XML_ELEM *pstSess, 
                ST_XML_ELEM **ppstManu);
                
/* pde pidf ext set home */
ZFUNC ZINT EaPidf_ExtSetHome(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstHome);

/* pde pidf ext set visited */
ZFUNC ZINT EaPidf_ExtSetVisited(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstVisited);

/* pde pidf ext set content type */
ZFUNC ZINT EaPidf_ExtSetCttType(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstCtt);

/* pde pidf ext set etag */
ZFUNC ZINT EaPidf_ExtSetEtag(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstEtag);

/* pde pidf ext set fsize */
ZFUNC ZINT EaPidf_ExtSetFsize(ST_XML_ELEM *pstElem, 
                ZUINT iFsize);

/* pde pidf ext set resolution */
ZFUNC ZINT EaPidf_ExtSetReso(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstReso);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_EXT_H__ */

