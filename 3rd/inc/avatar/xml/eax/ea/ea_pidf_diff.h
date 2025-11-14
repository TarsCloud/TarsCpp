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
  File name     : ea_pidf_diff.h
  Module        : eax pidf-diff
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pidf-diff.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_DIFF_H__
#define _EA_PIDF_DIFF_H__

#ifdef __cplusplus
extern "C"{
#endif

/* pidf-diff get the pidf-diff */
ZFUNC ZINT EaPidf_DiffGetDiff(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstDiff);

/* pidf-diff get the pidf-full */
ZFUNC ZINT EaPidf_DiffGetFull(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstFull);

/* xcap-diff get the pidf-diff first add */
ZFUNC ZINT EaPidf_DiffDiffGetFirstAdd(ST_XML_ELEM *pstDiff, 
                ST_XML_ELEM **ppstAdd);

/* xcap-diff get the pidf-diff next add */
ZFUNC ZINT EaPidf_DiffDiffGetNextAdd(ST_XML_ELEM *pstAdd, 
                ST_XML_ELEM **ppstAdd);

/* xcap-diff get the pidf-diff first remove */
ZFUNC ZINT EaPidf_DiffDiffGetFirstRmv(ST_XML_ELEM *pstDiff, 
                ST_XML_ELEM **ppstRmv);

/* xcap-diff get the pidf-diff next remove */
ZFUNC ZINT EaPidf_DiffDiffGetNextRmv(ST_XML_ELEM *pstRmv, 
                ST_XML_ELEM **ppstRmv);

/* xcap-diff get the pidf-diff first replace */
ZFUNC ZINT EaPidf_DiffDiffGetFirstRpl(ST_XML_ELEM *pstDiff, 
                ST_XML_ELEM **ppstRpl);

/* xcap-diff get the pidf-diff next replace */
ZFUNC ZINT EaPidf_DiffDiffGetNextRpl(ST_XML_ELEM *pstRpl, 
                ST_XML_ELEM **ppstRpl);

/* pidf-diff get the pidf-diff version */
ZFUNC ZINT EaPidf_DiffDiffGetVer(ST_XML_ELEM *pstDiff, 
                ZUINT *piVer);

/* pidf-diff get the pidf-diff entity */
ZFUNC ZINT EaPidf_DiffDiffGetEnt(ST_XML_ELEM *pstDiff, 
                ST_ZOS_USTR **ppstEnt);

/* pidf-diff get the pidf-full presence */
ZFUNC ZINT EaPidf_DiffFullGetPres(ST_XML_ELEM *pstFull, 
                ST_XML_ELEM **ppstPres);

/* pidf-diff get the pidf-full version */
ZFUNC ZINT EaPidf_DiffFullGetVer(ST_XML_ELEM *pstFull, 
                ZUINT *piVer);

/* pidf-diff set the pidf-diff */
ZFUNC ZINT EaPidf_DiffSetDiff(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstDiff);

/* pidf-diff set the pidf-full */
ZFUNC ZINT EaPidf_DiffSetFull(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstFull);

/* xcap-diff set the pidf-diff add */
ZFUNC ZINT EaPidf_DiffDiffSetAdd(ST_XML_ELEM *pstDiff, 
                ST_XML_ELEM **ppstAdd);

/* xcap-diff set the pidf-diff remove */
ZFUNC ZINT EaPidf_DiffDiffSetRmv(ST_XML_ELEM *pstDiff, 
                ST_XML_ELEM **ppstRmv);

/* xcap-diff set the pidf-diff replace */
ZFUNC ZINT EaPidf_DiffDiffSetRpl(ST_XML_ELEM *pstDiff, 
                ST_XML_ELEM **ppstRpl);

/* pidf-diff set the pidf-diff version */
ZFUNC ZINT EaPidf_DiffDiffSetVer(ST_XML_ELEM *pstDiff, ZUINT iVer);

/* pidf-diff set the pidf-diff entity */
ZFUNC ZINT EaPidf_DiffDiffSetEnt(ST_XML_ELEM *pstDiff, 
                ST_ZOS_USTR *pstEnt);

/* pidf-diff set the pidf-full presence */
ZFUNC ZINT EaPidf_DiffFullSetPres(ST_XML_ELEM *pstFull, 
                ST_XML_ELEM **ppstPres);

/* pidf-diff set the pidf-full version */
ZFUNC ZINT EaPidf_DiffFullSetVer(ST_XML_ELEM *pstFull, ZUINT iVer);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_DIFF_H__ */

