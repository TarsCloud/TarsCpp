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
  File name     : http_abnf_chrset.h
  Module        : http protocol abnf charaset set definition
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-03-30
  Description   :
    Marcos and structure definitions required by the http protocol.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTP_ABNF_CHRSET_H__
#define _HTTP_ABNF_CHRSET_H__

#ifdef __cplusplus
extern "C" {
#endif

/* basic character set */
#define HTTP_ALPHA           0x00000001 /* ALPHA */
#define HTTP_DIGIT           0x00000002 /* DIGIT */
#define HTTP_RESERVED        0x00000004 /* reserved */
#define HTTP_MARK            0x00000008 /* mark */
#define HTTP_UTF8_CHR1       0x00000010 /* TEXT-UTF8char 1 */
#define HTTP_UTF8_NONASCII1  0x00000020 /* UTF8-NONASCII 1 */
#define HTTP_UTF8_CONT       0x00000040 /* UTF8-CONT */
#define HTTP_LHEX1           0x00000080 /* LHEX 1 */
#define HTTP_TKN1            0x00000100 /* token 1 */
#define HTTP_WORD1           0x00000200 /* word 1 */
#define HTTP_CTEXT1          0x00000400 /* ctext 1 */
#define HTTP_QDTEXT1         0x00000800 /* qdtext 1 */
#define HTTP_QPAIR1          0x00001000 /* quoted-pair 1 */
#define HTTP_USER_UNRESERVED 0x00002000 /* user-unreserved */
#define HTTP_PASS1           0x00004000 /* password 1 */
#define HTTP_URIC_NO_SLASH1  0x00008000 /* uric-no-slash  1 */
#define HTTP_PCHR1           0x00010000 /* pchar 1 */
#define HTTP_SCHEME1         0x00020000 /* scheme 1 */
#define HTTP_REG_NAME1       0x00040000 /* reg-name 1 */
#define HTTP_HEXDIG1         0x00080000 /* HEXDIG */
#define HTTP_HOSTNAME1       0x00100000 /* hostname 1 */
#define HTTP_TKN_NODOT1      0x00200000 /* token-nodot 1 */
#define HTTP_DOMAIN_NAME1    0x00400000 /* domainname 1 */
#define HTTP_RSEGMENT1       0x00800000 /* rel_segment 1 */
#define HTTP_ROUTE1          0x01000000 /* route 1 */
#define HTTP_DOT             0x02000000 /* dot */
#define HTTP_IDENT1          0x04000000 /* identity */

/* composite character set */
#define HTTP_ALPHA_NUM (HTTP_ALPHA | HTTP_DIGIT) /* alphanum */    
#define HTTP_UNRESERVED (HTTP_ALPHA_NUM | HTTP_MARK) /* unreserved */    
#define HTTP_UTF8_NONASCII \
    (HTTP_UTF8_NONASCII1 | HTTP_UTF8_CONT) /* UTF8-NONASCII */
#define HTTP_UTF8_CHR \
    (HTTP_ALPHA_NUM | HTTP_UTF8_CHR1 |  HTTP_UTF8_NONASCII) /* TEXT-UTF8char */
#define HTTP_USER (HTTP_UNRESERVED | HTTP_USER_UNRESERVED) /* user */
#define HTTP_DOMAIN_LABEL HTTP_ALPHA_NUM /* domainlabel */
#define HTTP_HEXDIGIT (HTTP_HEXDIG1 | HTTP_DIGIT) /* HEXDIG */
#define HTTP_LHEX (HTTP_DIGIT | HTTP_LHEX1) /* LHEX */
#define HTTP_TKN (HTTP_ALPHA_NUM | HTTP_TKN1) /* token */
#define HTTP_WORD (HTTP_ALPHA_NUM | HTTP_WORD1) /* word */
#define HTTP_CTEXT \
    (HTTP_CTEXT1 | HTTP_ALPHA_NUM | HTTP_UTF8_NONASCII) /* ctext */
#define HTTP_QDTEXT \
    (HTTP_QDTEXT1 | HTTP_ALPHA_NUM | HTTP_UTF8_NONASCII) /* qdtext */
#define HTTP_PASS (HTTP_PASS1 | HTTP_UNRESERVED) /* password */
#define HTTP_QPAIR (HTTP_QPAIR1 | HTTP_ALPHA_NUM) /* quoted-pair */
#define HTTP_QSTR HTTP_QDTEXT /* quoted-string */
#define HTTP_URIC_NO_SLASH \
    (HTTP_URIC_NO_SLASH1 | HTTP_UNRESERVED) /* uric-no-slash */
#define HTTP_PCHR (HTTP_PCHR1 | HTTP_UNRESERVED) /* pchar*/
#define HTTP_SCHEME (HTTP_SCHEME1 | HTTP_ALPHA_NUM) /* scheme */
#define HTTP_URIC (HTTP_RESERVED| HTTP_UNRESERVED) /* uric */
#define HTTP_RSEGMENT (HTTP_UNRESERVED | HTTP_RSEGMENT1) /* rel_segment */
#define HTTP_REG_NAME (HTTP_UNRESERVED | HTTP_REG_NAME1) /* reg_name */
#define HTTP_MD5_DIGEST (HTTP_RESERVED | HTTP_UNRESERVED) /* md5-digest */
#define HTTP_REASON_PHRASE \
    (HTTP_RESERVED | HTTP_UNRESERVED | HTTP_UTF8_NONASCII \
     | HTTP_UTF8_CONT | HTTP_IDENT1) /* Reason-Phrase */
#define HTTP_HOSTNAME (HTTP_HOSTNAME1 | HTTP_ALPHA_NUM) /* hostname */
#define HTTP_HDRVAL (HTTP_UTF8_CHR | HTTP_UTF8_CONT) /* header-value */
#define HTTP_LANG HTTP_ALPHA /* language */
#define HTTP_DOMAIN_NAME (HTTP_DOMAIN_NAME1 | HTTP_ALPHA_NUM) /* domainname */
#define HTTP_ROUTE (HTTP_ROUTE1 | HTTP_DOMAIN_NAME) /* route */
#define HTTP_LOCAL_PART (HTTP_WORD | HTTP_DOT) /* local-part */
#define HTTP_IDENT (HTTP_ALPHA_NUM | HTTP_IDENT1) /* identity */
#define HTTP_AFLAG HTTP_IDENT /* auth-flag */

/* http abnf get chrset */
ZFUNC ZCHRSET Http_ChrsetGetId(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_ABNF_CHRSET_H__ */

