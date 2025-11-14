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
  File name     : abnf_decode.h
  Module        : abnf decode library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-12-29
  Description   :
      Data structure and function definitions required 
      by the abnf decode interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ABNF_DECODE_H__
#define _ABNF_DECODE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* abnf adjust buffer */
ZFUNC ZVOID Abnf_AdjBuf(ST_ABNF_BUF_INFO *pstBuf, ST_ABNF_SEPA_INFO *pstSepa,
                ZBOOL bIgnEol, ZBOOL *pbRealEol);

/* abnf ignore the white space, only used by MGCP an SDP */
ZFUNC ZINT Abnf_IgnWS(ST_ABNF_MSG *pstMsg);

/* abnf ignore the linear white space, only used by SIP and Megaco/H.248 */
ZFUNC ZINT Abnf_IgnLWS(ST_ABNF_MSG *pstMsg);

/* abnf decode get one line */
ZFUNC ZINT Abnf_GetLine(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstLine);

/* abnf decode get one word, if need ignore the WS or LWS, please explicit 
 * call Abnf_IgnWS or Abnf_IgnLWS before or after call this function.
 */
ZFUNC ZINT Abnf_GetStr(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstStr);

/* abnf decode get one protocol token, if need ignore the WS or LWS, 
 * please explicit call Abnf_IgnWS or Abnf_IgnLWS 
 * before or after call this function.
 */
ZFUNC ZINT Abnf_GetTkn(ST_ABNF_MSG *pstMsg, ZTKNID *pzTknId);

/* abnf decode get a character and change the length , 
 * if need ignore the WS or LWS, please explicit call Abnf_IgnWS 
 * or Abnf_IgnLWS before or after call this function.
 */
ZFUNC ZINT Abnf_GetChr(ST_ABNF_MSG *pstMsg, ZUCHAR *pucChr);

/* abnf decode expect to match a character and change the length, 
 * if need ignore the WS or LWS, please explicit call Abnf_IgnWS 
 * or Abnf_IgnLWS before or after call this function.
 */
ZFUNC ZINT Abnf_ExpectChr(ST_ABNF_MSG *pstMsg, ZUCHAR ucChr, ZBOOL bCase);

/* abnf decode try to expect match a character, change length while match, 
 * needn't use the restore decode message while not match, 
 * if need ignore the WS or LWS, please explicit call Abnf_IgnWS 
 * or Abnf_IgnLWS before or after call this function.
 */
ZFUNC ZINT Abnf_TryExpectChr(ST_ABNF_MSG *pstMsg, ZUCHAR ucChr, ZBOOL bCase);

/* abnf decode expect EOL(end of line) occur */
ZFUNC ZINT Abnf_ExpectEol(ST_ABNF_MSG *pstMsg);

/* abnf decode try to expect EOL(end of line) occur */
ZFUNC ZINT Abnf_TryExpectEol(ST_ABNF_MSG *pstMsg);

/* abnf decode expect LWSP or WSP occur */
ZFUNC ZINT Abnf_ExpectLWS(ST_ABNF_MSG *pstMsg);

/* abnf decode try to expect LWSP or WSP occur */
ZFUNC ZINT Abnf_TryExpectLWS(ST_ABNF_MSG *pstMsg);

/* abnf decode expect CRLF occur */
ZFUNC ZINT Abnf_ExpectCRLF(ST_ABNF_MSG *pstMsg);

/* abnf decode try to expect CRLF occur */
ZFUNC ZINT Abnf_TryExpectCRLF(ST_ABNF_MSG *pstMsg);

/* abnf get an unsigned int data from decimal digit string */
ZFUNC ZINT Abnf_GetUiDigit(ST_ABNF_MSG *pstMsg, ZUINT *piData);

/* abnf get an unsigned short data from decimal digit string */
ZFUNC ZINT Abnf_GetUsDigit(ST_ABNF_MSG *pstMsg, ZUSHORT *pwData);

/* abnf get an unsigned char data from decimal digit string */
ZFUNC ZINT Abnf_GetUcDigit(ST_ABNF_MSG *pstMsg, ZUCHAR *pucData);

/* abnf get an unsigned long data from hexadecimal digit string */
ZFUNC ZINT Abnf_GetXUiDigit(ST_ABNF_MSG *pstMsg, ZUINT *piData);

/* abnf get ipv4 address */
ZFUNC ZINT Abnf_GetIpV4(ST_ABNF_MSG *pstMsg, ZUINT *piAddr);

/* abnf get ipv6 address */
ZFUNC ZINT Abnf_GetIpV6(ST_ABNF_MSG *pstMsg, ZUCHAR *pucAddr);

/* abnf get scanned string */
ZFUNC ZINT Abnf_GetScannedStr(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstStr);

/* abnf get scanned long string */
ZFUNC ZINT Abnf_GetScannedStrL(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstStr);

/* abnf get scanning(the left message to be scanned) string */
ZFUNC ZINT Abnf_GetScanningStr(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstStr);

/* abnf get scanning(the left message to be scanned) long string */
ZFUNC ZINT Abnf_GetScanningStrL(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstStr);

/* abnf get the string by chrset */
ZFUNC ZINT Abnf_GetSStrChrset(ST_ABNF_MSG *pstMsg, ZCHRSET zChrset,
                ZUINT iClass, ST_ZOS_SSTR *pstStr);

/* abnf get the string by chrset */
ZFUNC ZINT Abnf_GetSStrEscape(ST_ABNF_MSG *pstMsg, ZCHRSET zChrset,
                ZUINT iClass, ZUCHAR ucEscapeChr, ZUINT iEscapeClass,
                ST_ZOS_SSTR *pstStr);

/* abnf get the string by seperators */
ZFUNC ZINT Abnf_GetSStrSepas(ST_ABNF_MSG *pstMsg, ZUCHAR ucSpea0, 
                ZUCHAR ucSpea1, ZUCHAR ucSpea2, ZUCHAR ucSpea3,
                ST_ZOS_SSTR *pstStr);

/* abnf get the string by chrset and length */
ZFUNC ZINT Abnf_GetNSStrChrset(ST_ABNF_MSG *pstMsg, ZCHRSET zChrset,
                ZUINT iClass, ZUINT iMinLen, ZUINT iMaxLen, 
                ST_ZOS_SSTR *pstStr);

/* abnf get the escape string by chrset and length */
ZFUNC ZINT Abnf_GetNSStrEscape(ST_ABNF_MSG *pstMsg, ZCHRSET zChrset,
                ZUINT iClass, ZUCHAR ucEscapeChr, ZUINT iEscapeClass, 
                ZUINT iMinLen, ZUINT iMaxLen, ST_ZOS_SSTR *pstStr);

/* abnf get the token by chrset */
ZFUNC ZINT Abnf_GetTknChrset(ST_ABNF_MSG *pstMsg, ZTKNMGR zTknMgr,
                ZUINT iTknType, ZCHRSET zChrset, ZUINT iClass, 
                ZTKNID *pzTknId);

/* abnf get the token by chrset */
ZFUNC ZINT Abnf_GetTknChrsetX(ST_ABNF_MSG *pstMsg, ZCHRSET zChrset, 
                ZUINT iClass, ST_ABNF_TKN_ITEM *pstItems, ZUINT iItemSize, 
                ZTKNID *pzTknId);

/* abnf get the token by separators */
ZFUNC ZINT Abnf_GetTknSepas(ST_ABNF_MSG *pstMsg, ZTKNMGR zTknMgr,
                ZUINT iTknType, ZUCHAR ucSpea0, ZUCHAR ucSpea1,
                ZUCHAR ucSpea2, ZUCHAR ucSpea3, ZTKNID *pzTknId);

#ifdef __cplusplus
}
#endif

#endif /* _ABNF_DECODE_H__ */

