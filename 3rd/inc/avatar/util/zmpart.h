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
  File name     : zmultipart.h
  Module        : mime multipart abnf
  Author        : leo.lv 
  Version       : 0.1   
  Created on    : 2005-04-14    
  Description   :
      Data structure and function definitions required by the multipart abnf message.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZMPART_H__
#define _ZMPART_H__

#ifdef __cplusplus
extern "C"{
#endif

/* multipart message id (rfc2045) */
typedef ZVOID * ZMPARTID;

/* multipart message create */
ZFUNC ZMPARTID Zmpart_Create(ZFUNC_VOID);

/* multipart message delete */
ZFUNC ZVOID Zmpart_Delete(ZMPARTID zMpartId);

/* multipart message load */
ZFUNC ZINT Zmpart_Load(ZCHAR *pcData, ZUINT iLen, ZMPARTID *pzMpartId);

/* multipart message load with boundary */
ZFUNC ZINT Zmpart_LoadB(ZCHAR *pcData, ZUINT iLen, ZCHAR *pcBoundary,
                ZMPARTID *pzMpartId);

/* multipart message save */
ZFUNC ZINT Zmpart_Save(ZMPARTID zMpartId, ZDBUF *pzData);

/* multipart locate multipart from content */
ZFUNC ZINT Zmpart_Locate(ST_ZOS_SSTR *pstContent, ST_ZOS_SSTR *pstPrevMPart, 
                ST_ZOS_SSTR *pstBoundary, ST_ZOS_SSTR *pstMPart, ZBOOL *pbLast);

/* multipart message pick up header */
ZFUNC ZINT Zmpart_PickHdr(ZMPARTID zMpartId, ZCHAR *pcName, 
                ST_ZOS_SSTR *pstVal);

/* multipart message pick up Content-Type */
ZFUNC ZINT Zmpart_PickHdrCType(ZMPARTID zMpartId, 
                ST_ZOS_SSTR *pstMType);

/* multipart message pick up Content-Transfer-Encoding */
ZFUNC ZINT Zmpart_PickHdrCTrsfEncoding(ZMPARTID zMpartId, 
                ST_ZOS_SSTR *pstEncoding);

/* multipart message pick up Content-Length */
ZFUNC ZINT Zmpart_PickHdrCLen(ZMPARTID zMpartId, 
                ZUINT *piLen);

/* multipart message pick up content */
ZFUNC ZINT Zmpart_PickContent(ZMPARTID zMpartId, ST_ZOS_SSTR *pstContent);

/* multipart message fill header */
ZFUNC ZINT Zmpart_FillHdr(ZMPARTID zMpartId, ZCHAR *pcName, ZCHAR *pcVal);

/* multipart message fill Content-Type */
ZFUNC ZINT Zmpart_FillHdrCType(ZMPARTID zMpartId, ZCHAR *pcCType);

/* multipart message fill Content-Transfer-Encoding */
ZFUNC ZINT Zmpart_FillHdrCTrsfEncoding(ZMPARTID zMpartId, 
                ZCHAR *pcEncoding);

/* multipart message fill Content-Length */
ZFUNC ZINT Zmpart_FillHdrCLen(ZMPARTID zMpartId, ZUINT iLen);

/* multipart message fill content */
ZFUNC ZINT Zmpart_FillContent(ZMPARTID zMpartId, ZCHAR *pcContent);

/* multipart check boundary */
ZFUNC ZINT Zmpart_ChkBoundary(ST_ABNF_MSG *pstMsg, ZCHAR *pcBoundary);

/* multipart check boundary */
ZFUNC ZINT Zmpart_ChkBoundaryX(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR stLine, 
                ZCHAR *pcBoundary);

/* multipart check transact-id */
ZFUNC ZINT Zmpart_ChkTransId(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR stLine, 
                ST_ZOS_SSTR *pstTransId);

#ifdef __cplusplus
}
#endif

#endif /* _ZMPART_H__ */

