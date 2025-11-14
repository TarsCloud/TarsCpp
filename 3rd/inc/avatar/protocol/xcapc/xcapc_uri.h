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
  File name     : xcapc_uri.h
  Module        : xcap client
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-04-22
  Description   :
    Marcos and structure definitions required by the xcapc uri.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XCAPC_URI_H__
#define _XCAPC_URI_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xcapc uri create */
ZFUNC ZINT Xcapc_UriCreate(ST_XCAPC_URI **ppstUri);

/* xcapc uri delete */
ZFUNC ZINT Xcapc_UriDelete(ST_XCAPC_URI *pstUri);

/* xcapc uri add document segment */
ZFUNC ZINT Xcapc_UriAddDocSeg(ST_XCAPC_URI *pstUri, ST_ZOS_SSTR *pstSeg);

/* xcapc uri add document segments */
ZFUNC ZINT Xcapc_UriAddDocSegs(ST_XCAPC_URI *pstUri, ST_ZOS_SSTR *pstSegs);

/* xcapc uri add the element step by name */
ZFUNC ZINT Xcapc_UriAddStepByName(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ST_ZOS_SSTR *pstName);

/* xcapc uri add the element step by name id */
ZFUNC ZINT Xcapc_UriAddStepByNameX(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ZUINT iNsId, ZUINT iNameId);

/* xcapc uri add the element step by position */
ZFUNC ZINT Xcapc_UriAddStepByPos(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ST_ZOS_SSTR *pstName, ZUINT iPos);

/* xcapc uri add the element step by name id and position */
ZFUNC ZINT Xcapc_UriAddStepByPosX(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ZUINT iNsId, ZUINT iNameId, 
                ZUINT iPos);

/* xcapc uri add the element step by attribute */
ZFUNC ZINT Xcapc_UriAddStepByAttr(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ST_ZOS_SSTR *pstName, 
                ST_ZOS_SSTR *pstTName, ST_ZOS_SSTR *pstTVal);

/* xcapc uri add the element step by attribute */
ZFUNC ZINT Xcapc_UriAddStepByAttrX(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ZUINT iNsId, ZUINT iENameId,
                ZUINT iTNameId, ST_ZOS_SSTR *pstTVal);

/* xcapc uri add the element step by position & attribute */
ZFUNC ZINT Xcapc_UriAddStepByPAttr(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ST_ZOS_SSTR *pstName, ZUINT iPos, 
                ST_ZOS_SSTR *pstTName, ST_ZOS_SSTR *pstTVal);

/* xcapc uri add the element step by attribute */
ZFUNC ZINT Xcapc_UriAddStepByPAttrX(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstPrefix, ZUINT iNsId, ZUINT iENameId, 
                ZUINT iPos, ZUINT iTNameId, ST_ZOS_SSTR *pstTVal);

/* xcapc uri add the query by name */
ZFUNC ZINT Xcapc_UriAddQryByName(ST_XCAPC_URI *pstUri,
                ST_ZOS_SSTR *pstNsName, ST_ZOS_SSTR *pstPrefix);

/* xcapc uri add the query by name id */
ZFUNC ZINT Xcapc_UriAddQryByNameX(ST_XCAPC_URI *pstUri, ZUINT iNsId, 
                ST_ZOS_SSTR *pstPrefix);

/* xcapc uri set the terminal selector by attribute-selector */
ZFUNC ZINT Xcapc_UriSetTermsByAttr(ST_XCAPC_URI *pstUri, 
                ST_ZOS_SSTR *pstName);

/* xcapc uri set the terminal selector by attribute-selector */
ZFUNC ZINT Xcapc_UriSetTermsByAttrX(ST_XCAPC_URI *pstUri, 
                ZUINT iNsId, ZUINT iNameId);

/* xcapc uri set the terminal selector by namespace-selector */
ZFUNC ZINT Xcapc_UriSetTermsByNs(ST_XCAPC_URI *pstUri);

/* xcapc uri format into message */
ZFUNC ZINT Xcapc_UriFormatMsg(ST_XCAPC_URI *pstUri, ZDBUF zMsgBuf);

/* xcapc uri format into message */
ZFUNC ZINT Xcapc_UriFormatMsgX(ST_XCAPC_URI *pstUri, ZDBUF *pzMsgBuf);

#ifdef __cplusplus
}
#endif

#endif /* _XCAPC_URI_H__ */

