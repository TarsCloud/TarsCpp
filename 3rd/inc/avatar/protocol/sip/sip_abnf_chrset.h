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
  File name     : sip_abnf_chrset.h
  Module        : sip protocol abnf charaset set definition
  Author        : zocol
  Version       : 0.1
  Created on    : 2005-05-01
  Description   :
    Marcos and structure definitions required by the sip protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SIP_ABNF_CHRSET_H__
#define _SIP_ABNF_CHRSET_H__

/**
 * @file
 * @brief SIP ABNF character sets.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* basic character set */
#define SIP_ALPHA           0x00000001 /**< @brief ALPHA */
#define SIP_DIGIT           0x00000002 /**< @brief DIGIT */
#define SIP_RESERVED        0x00000004 /**< @brief reserved */
#define SIP_MARK            0x00000008 /**< @brief mark */
#define SIP_UTF8_CHR1       0x00000010 /**< @brief TEXT-UTF8char 1 */
#define SIP_UTF8_NONASCII1  0x00000020 /**< @brief UTF8-NONASCII 1 */
#define SIP_UTF8_CONT       0x00000040 /**< @brief UTF8-CONT */
#define SIP_LHEX1           0x00000080 /**< @brief LHEX 1 */
#define SIP_TKN1            0x00000100 /**< @brief token 1 */
#define SIP_SEPA            0x00000200 /**< @brief separators */
#define SIP_WORD1           0x00000400 /**< @brief word 1 */
#define SIP_CTEXT1          0x00000800 /**< @brief ctext 1 */
#define SIP_QDTEXT1         0x00001000 /**< @brief qdtext 1 */
#define SIP_QPAIR1          0x00002000 /**< @brief quoted-pair 1 */
#define SIP_USER_UNRESERVED 0x00004000 /**< @brief user-unreserved */
#define SIP_PASS1           0x00008000 /**< @brief password 1 */
#define SIP_PARM_UNRESERVED 0x00010000 /**< @brief param-unreserved */
#define SIP_HNV_UNRESERVED  0x00020000 /**< @brief hnv-unreserved */
#define SIP_URIC_NO_SLASH1  0x00040000 /**< @brief uric-no-slash  1 */
#define SIP_PCHR1           0x00080000 /**< @brief pchar 1 */
#define SIP_SCHEME1         0x00100000 /**< @brief scheme 1 */
#define SIP_REG_NAME1       0x00200000 /**< @brief reg-name 1 */
#define SIP_HEXDIG1         0x00400000 /**< @brief HEXDIG */
#define SIP_HOSTNAME1       0x00800000 /**< @brief hostname 1 */
#define SIP_TKN_NODOT1      0x01000000 /**< @brief token-nodot 1 */
#define SIP_VISUAL_SEPA     0x02000000 /**< @brief visual-separator */
#define SIP_PHONEDIGIT_HEX1 0x04000000 /**< @brief phonedigit-hex 1 */
#define SIP_DOMAIN_NAME1    0x08000000 /**< @brief domainname 1 */
#define SIP_DOT             0x10000000 /**< @brief '.' */
#define SIP_QDTEXT_NA1      0x20000000 /* qdtext-no-abkt 1 */
#define SIP_FTAG1           0x40000000 /* ftag-name 1 */
#define SIP_NUMERIC1        0x80000000 /* numeric */

/* extension character set 2*/
#define SIP_2ALPHA          0x00000001 /**< @brief ALPHA */
#define SIP_2DIGIT          0x00000002 /**< @brief DIGIT */
#define SIP_2HEXDIG1        0x00000004 /**< @brief HEXDIG */
#define SIP_2DOMAIN_NAME1   0x00000008 /**< @brief domainname 2 */

/* composite character set */
#define SIP_ALPHA_NUM (SIP_ALPHA | SIP_DIGIT) /**< @brief alphanum */    
#define SIP_UNRESERVED (SIP_ALPHA_NUM | SIP_MARK) /**< @brief unreserved */    
#define SIP_UTF8_NONASCII \
    (SIP_UTF8_NONASCII1 | SIP_UTF8_CONT) /**< @brief UTF8-NONASCII */
#define SIP_UTF8_CHR \
    (SIP_ALPHA_NUM | SIP_UTF8_CHR1 | SIP_UTF8_NONASCII) /**< @brief TEXT-UTF8char */
#define SIP_UTF8_TRIM SIP_UTF8_CHR/**< @brief TEXT-UTF8-TRIM */
#define SIP_USER (SIP_UNRESERVED | SIP_USER_UNRESERVED) /**< @brief user */
#define SIP_DOMAIN_LABEL SIP_ALPHA_NUM /**< @brief domainlabel */
#define SIP_TOP_LABEL SIP_ALPHA_NUM /**< @brief toplabel */
#define SIP_HEXDIGIT (SIP_HEXDIG1 | SIP_DIGIT) /**< @brief HEXDIG */
#define SIP_LHEX (SIP_DIGIT | SIP_LHEX1) /**< @brief LHEX */
#define SIP_TKN (SIP_ALPHA_NUM | SIP_TKN1) /**< @brief token */
#define SIP_TKN_NODOT \
    (SIP_ALPHA_NUM | SIP_TKN_NODOT1) /**< @brief token-nodot */
#define SIP_TKN_UTF8 (SIP_TKN | SIP_UTF8_NONASCII) /**< @brief utf8 token */
#define SIP_WORD (SIP_ALPHA_NUM | SIP_WORD1) /**< @brief word */
#define SIP_CTEXT \
    (SIP_CTEXT1 | SIP_ALPHA_NUM | SIP_UTF8_NONASCII) /**< @brief ctext */
#define SIP_QDTEXT \
    (SIP_QDTEXT1 | SIP_ALPHA_NUM | SIP_UTF8_NONASCII) /**< @brief qdtext */
#define SIP_PASS (SIP_PASS1 | SIP_UNRESERVED) /**< @brief password */
#define SIP_QPAIR (SIP_QPAIR1 | SIP_ALPHA_NUM) /**< @brief quoted-pair */
#define SIP_QSTR (SIP_QDTEXT | SIP_UTF8_NONASCII) /**< @brief quoted-string */
#define SIP_URIC_NO_SLASH \
    (SIP_URIC_NO_SLASH1 | SIP_UNRESERVED) /**< @brief uric-no-slash */
#define SIP_PCHR (SIP_PCHR1 | SIP_UNRESERVED) /**< @brief pchar*/
#define SIP_SCHEME (SIP_SCHEME1 | SIP_ALPHA_NUM) /**< @brief scheme */
#define SIP_PARM_CHR \
    (SIP_PARM_UNRESERVED | SIP_UNRESERVED) /**< @brief paramchar */
#define SIP_PARM_CHR_COMPLY \
    (SIP_TKN | SIP_PARM_CHR) /**< @brief paramchar */
#define SIP_URIC (SIP_RESERVED| SIP_UNRESERVED) /**< @brief uric */
#define SIP_OPAQUE_PART \
    (SIP_URIC_NO_SLASH | SIP_URIC) /**< @brief opaque-part */    
#define SIP_REASON_PHRASE \
    (SIP_RESERVED | SIP_UNRESERVED | SIP_UTF8_NONASCII \
     | SIP_UTF8_CONT) /**< @brief Reason-Phrase */
#define SIP_CONTENT_CODING SIP_TKN /**< @brief content-coding */
#define SIP_OPTION_TAG SIP_TKN /**< @brief option-tag */
#define SIP_PRODUCT_VER SIP_TKN /**< @brief product-version */
#define SIP_PROTOCOL_VER SIP_TKN /**< @brief protocol-version */
#define SIP_PSEUDONYM SIP_TKN /**< @brief pseudonym */
#define SIP_HDR_NAME SIP_TKN /**< @brief header-name */
#define SIP_HOSTNAME (SIP_HOSTNAME1 | SIP_ALPHA_NUM) /**< @brief hostname */
#define SIP_HDRVAL (SIP_UTF8_CHR | SIP_UTF8_CONT) /**< @brief header-value */
#define SIP_HNAME (SIP_HNV_UNRESERVED | SIP_UNRESERVED) /**< @brief hname */
#define SIP_HVAL (SIP_HNV_UNRESERVED | SIP_UNRESERVED) /**< @brief hvalue */
#define SIP_LANG (SIP_ALPHA | SIP_DIGIT) /**< @brief language */
#define SIP_PHONE_DIGIT (SIP_DIGIT | SIP_VISUAL_SEPA) /**< @brief phonedigit */
#define SIP_DOMAIN_NAME \
    (SIP_DOMAIN_NAME1 | SIP_ALPHA_NUM) /**< @brief domainname */
#define SIP_PHONEDIGIT_HEX \
    (SIP_PHONEDIGIT_HEX1 | SIP_HEXDIGIT \
     | SIP_VISUAL_SEPA) /**< @brief phonedigit-hex */
#define SIP_INDEX (SIP_DIGIT | SIP_DOT) /**< @brief hi-index */
#define SIP_FQND SIP_HOSTNAME /**< @brief FQDN */
#define SIP_STRVAL \
    (SIP_PHONEDIGIT_HEX1 | SIP_URIC | SIP_TKN_UTF8 \
     | SIP_QDTEXT_NA1) /*string-value */
#define SIP_FTAG (SIP_ALPHA_NUM | SIP_FTAG1) /* ftag-name */
#define SIP_TAG_VAL (SIP_TKN1 | SIP_ALPHA_NUM | SIP_NUMERIC1) /* tag-value */

/* extension composite character set */
#define SIP_2ALPHA_NUM (SIP_2ALPHA | SIP_2DIGIT) /**< @brief alphanum */    
#define SIP_2HEXDIGIT (SIP_2HEXDIG1 | SIP_2DIGIT) /**< @brief HEXDIG */
#define SIP_2DOMAIN_NAME \
    (SIP_2DOMAIN_NAME1 | SIP_2ALPHA_NUM) /**< @brief domainname */

/** @brief sip chrset id */
ZFUNC ZCHRSET Sip_ChrsetGetId(ZFUNC_VOID);

/** @brief sip chrset id 2 */
ZFUNC ZCHRSET Sip_ChrsetGetId2(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _SIP_ABNF_CHRSET_H__ */

