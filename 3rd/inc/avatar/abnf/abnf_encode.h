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
  File name     : abnf_encode.h
  Module        : abnf encode library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2004-12-29 
  Description   :
      Data structure and function definitions required 
      by the abnf encode interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ABNF_ENCODE_H__
#define _ABNF_ENCODE_H__

#ifdef __cplusplus
extern "C" {
#endif
    
/* abnf add one character */
ZFUNC ZINT Abnf_AddPstChr(ST_ABNF_MSG *pstMsg, ZUCHAR ucChr);

/* abnf add string operation */
ZFUNC ZINT Abnf_AddPstStr(ST_ABNF_MSG *pstMsg, ZCHAR *pcStr);
ZFUNC ZINT Abnf_AddPstStrN(ST_ABNF_MSG *pstMsg, ZCHAR *pcStr, ZUINT iLen);

/* abnf add string structure operation */
ZFUNC ZINT Abnf_AddPstSStr(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstStr);
ZFUNC ZINT Abnf_AddPstLSStr(ST_ABNF_MSG *pstMsg, ST_ZOS_SSTR *pstStr);

/* abnf add one paired string */
ZFUNC ZINT Abnf_AddPairSStr(ST_ABNF_MSG *pstMsg, ZUCHAR ucPairChr, 
                ST_ZOS_SSTR *pstStr);

/* abnf add an unsigned int data into decimal digit string */
ZFUNC ZINT Abnf_AddUiDigit(ST_ABNF_MSG *pstMsg, ZUINT iData);

/* abnf add an unsigned short data into decimal digit string */
ZFUNC ZINT Abnf_AddUsDigit(ST_ABNF_MSG *pstMsg, ZUSHORT wData);

/* abnf add an unsigned char data into decimal digit string */
ZFUNC ZINT Abnf_AddUcDigit(ST_ABNF_MSG *pstMsg, ZUCHAR ucData);

/* abnf add an unsigned long data into hexadecimal digit string */
ZFUNC ZINT Abnf_AddXUiDigit(ST_ABNF_MSG *pstMsg, ZUINT iData);

/* abnf add ipv4 address into string */
ZFUNC ZINT Abnf_AddIpV4(ST_ABNF_MSG *pstMsg, ZUINT iAddr);

/* abnf add ipv6 address into string */
ZFUNC ZINT Abnf_AddIpV6(ST_ABNF_MSG *pstMsg, ZUCHAR *pucAddr);

/* abnf add token string */
ZFUNC ZINT Abnf_AddTkn(ST_ABNF_MSG *pstMsg, ST_ABNF_TKN_ITEM *pstItems, 
                ZUINT iItemSize, ZTKNID zTknId);

#ifdef __cplusplus
}
#endif

#endif /* _ABNF_ENCODE_H__ */

