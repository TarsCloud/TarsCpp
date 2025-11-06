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
  File name     : imap_abnf_chrset.h
  Module        : imap abnf charaset set definition
  Author        : Cathy Chen   
  Version       : 0.1   
  Created on    : 2010-09-16    
  Description   :
      Marcos and structure definitions required by the imap protocol.

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _IMAP_ABNF_CHRSET_H__
#define _IMAP_ABNF_CHRSET_H__
    
/** 
 * @file imap_abnf_chrset.h
 * @brief IMAP ABNF charaset interfaces.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup group_def_charset IMAP Character Set
 * @{
 */
    
/* Basic character set. */
#define IMAP_ALPHA           0x00000001 /**< @brief ALPHA */
#define IMAP_NZDIGIT         0x00000002 /**< @brief NZ-DIGIT */
#define IMAP_ZERO            0x00000004 /**< @brief ZERO */
#define IMAP_UPALPHA         0x00000008 /**< @brief UPALPHA */
#define IMAP_QCHAR           0x00000010 /**< @brief QUOTED-CHAR */
#define IMAP_ATOM            0x00000020 /**< @brief ATOM */
#define IMAP_NOASCII         0x00000040 /**< @brief non ascII */
#define IMAP_RSP_SPE         0x00000080 /**< @brief resp-specials */
#define IMAP_LST_WILD        0x00000100 /**< @brief list-wildcards */
#define IMAP_CR              0x00000200 /**< @brief CR */
#define IMAP_LF              0x00000400 /**< @brief LF */
#define IMAP_QUOTE           0x00000800 /**< @brief QUOTE */
#define IMAP_BLACK_SLASH     0x00001000 /**< @brief backslash */
#define IMAP_DOT             0x00002000 /**< @brief dot */
#define IMAP_HYPHEN          0x00004000 /**< @brief hyphen */
#define IMAP_PLUS            0x00008000 /**< @brief plis */
#define IMAP_SLASH           0x00010000 /**< @brief flash */
#define IMAP_FTEXT           0x00020000 /**< @brief ftext */
#define IMAP_TKN             0x00040000 /**< @brief taken */

/* Composite character set. */
#define IMAP_QUOTE_SPE (IMAP_QUOTE | IMAP_BLACK_SLASH) /* quoted-specials */
#define IMAP_TEXT_CHAR (IMAP_QCHAR | IMAP_QUOTE_SPE) /* TEXT-CHAR */
#define IMAP_CHAR (IMAP_TEXT_CHAR | IMAP_CR | IMAP_LF) /* CHAR */
#define IMAP_DIGIT (IMAP_NZDIGIT | IMAP_ZERO) /* DIGIT */
#define IMAP_MECH (IMAP_ALPHA | IMAP_DIGIT | IMAP_DOT | IMAP_HYPHEN) /* mechanism name */
#define IMAP_BASE64 (IMAP_ALPHA | IMAP_DIGIT | IMAP_PLUS | IMAP_SLASH) /* base64 */
//#define IMAP_TKN (IMAP_ALPHA | IMAP_DIGIT | IMAP_HYPHEN | IMAP_DOT)

/** @} */

/**
 * @brief Get IMAP ABNF character set ID.
 *
 * @return ABNF character set ID used by IMAP on succeed, or return ZNULL.
 *
 * @see
 */
ZFUNC ZCHRSET Imap_ChrsetGetId(ZFUNC_VOID);
    
#ifdef __cplusplus
    }
#endif
    
#endif /* _IMAP_ABNF_CHRSET_H__ */

