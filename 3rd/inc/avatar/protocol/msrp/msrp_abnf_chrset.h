/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : msrp_abnf_chrset.h
  Module        : msrp abnf charaset set definition
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Marcos and structure definitions required by the msrp protocol.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _MSRP_ABNF_CHRSET_H__
#define _MSRP_ABNF_CHRSET_H__
    
/** 
 * @file msrp_abnf_chrset.h
 * @brief MSRP ABNF charaset interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup group_def_charset MSRP Character Set
 * @{
 */
    
/* Basic character set. */
#define MSRP_ALPHA           0x00000001 /**< @brief ALPHA */
#define MSRP_DIGIT           0x00000002 /**< @brief DIGIT */
#define MSRP_UPALPHA         0x00000004 /**< @brief UPALPHA */
#define MSRP_UNRESERVED1     0x00000008 /**< @brief unreserved 1 */
#define MSRP_UTF8_TEXT1      0x00000010 /**< @brief UTF8text 1 */
#define MSRP_UTF8_NONASCII1  0x00000020 /**< @brief UTF8-NONASCII 1 */
#define MSRP_UTF8_CONT       0x00000040 /**< @brief UTF8-CONT */
#define MSRP_TKN1            0x00000080 /**< @brief token 1 */
#define MSRP_DELIMS1         0x00000100 /**< @brief sub-delims 1 */
#define MSRP_ATOM1           0x00000200 /**< @brief atom 1 */
#define MSRP_QDTEXT1         0x00000400 /**< @brief qdtext 1 */
#define MSRP_QTEXT1          0x00000800 /**< @brief qtext 1 */
#define MSRP_DTEXT1          0x00001000 /**< @brief dtext 1 */
#define MSRP_QPAIR1          0x00002000 /**< @brief quoted-pair 1 */
#define MSRP_IDENT1          0x00004000 /**< @brief ident-cahr 1 */
#define MSRP_COLON           0x00008000 /**< @brief ':' */
#define MSRP_HEXDIG1         0x00010000 /**< @brief HEXDIG */
#define MSRP_SESSION1        0x00020000 /**< @brief session-id 1 */
#define MSRP_QDESC1          0x00040000 /**< @brief qd-esc 1 */

/* Composite character set. */
#define MSRP_ALPHA_NUM (MSRP_ALPHA | MSRP_DIGIT) /**< @brief alphanum */    
#define MSRP_UNRESERVED (MSRP_UNRESERVED1 | MSRP_ALPHA_NUM) /**< @brief unreserved */    
#define MSRP_UTF8_NONASCII \
        (MSRP_UTF8_NONASCII1 | MSRP_UTF8_CONT) /**< @brief UTF8-NONASCII */
#define MSRP_UTF8_TEXT \
        (MSRP_UTF8_TEXT1 | MSRP_ALPHA_NUM | MSRP_UTF8_NONASCII) /**< @brief UTF8text */
#define MSRP_HEXDIGIT (MSRP_HEXDIG1 | MSRP_DIGIT) /**< @brief HEXDIG */
#define MSRP_QTEXT (MSRP_QTEXT1 | MSRP_ALPHA_NUM) /**< @brief qtext */
#define MSRP_CHAR (MSRP_QPAIR1 | MSRP_QTEXT) /**< @brief CHAR */
#define MSRP_IDENT (MSRP_IDENT1 | MSRP_ALPHA_NUM | MSRP_UNRESERVED1) /**< @brief ident-char */
#define MSRP_SESSION (MSRP_SESSION1 | MSRP_UNRESERVED) /**< @brief session-id */
#define MSRP_TKN (MSRP_TKN1 | MSRP_ALPHA_NUM) /**< @brief token */
#define MSRP_QPAIR MSRP_CHAR /**< @brief quoted-pair */
#define MSRP_QDTEXT \
        (MSRP_QDTEXT1 | MSRP_ALPHA_NUM | MSRP_UTF8_NONASCII) /**< @brief qdtext */        
#define MSRP_DTEXT (MSRP_DTEXT1 | MSRP_ALPHA_NUM) /**< @brief dtext */
#define MSRP_REG (MSRP_UNRESERVED | MSRP_DELIMS1) /**< @brief reg-name */
#define MSRP_USER (MSRP_REG | MSRP_COLON) /**< @brief userinfo */ 
#define MSRP_TEXT MSRP_CHAR /**< @brief text */
#define MSRP_ATOM (MSRP_ATOM1 | MSRP_ALPHA_NUM) /**< @brief atom */
/** @} */

/**
 * @brief Get MSRP ABNF character set ID.
 *
 * @return ABNF character set ID used by MSRP on succeed, or return ZNULL.
 *
 * @see
 */
ZFUNC ZCHRSET Msrp_ChrsetGetId(ZFUNC_VOID);
    
#ifdef __cplusplus
    }
#endif
    
#endif /* _MSRP_ABNF_CHRSET_H__ */

