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
  File name     : sdp_abnf_chrset.h
  Module        : sdp abnf chrset define
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2005-04-05    
  Description   :
      Data structure and function definitions required by the sdp abnf chrset

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SDP_ABNF_CHRSET_H__
#define _SDP_ABNF_CHRSET_H__

#ifdef __cplusplus
extern "C"{
#endif

/* basic character set */
#define SDP_ALPHA           0x00000001  /* ALPHA */
#define SDP_PDIGIT          0x00000002  /* POS-DIGIT */
#define SDP_DIGIT1          0x00000004  /* DIGIT 1*/
#define SDP_ESAFE1          0x00000008  /* email-safe 1 */
#define SDP_VCHR1           0x00000010  /* VCHAR 1*/
#define SDP_BSTR1           0x00000020  /* byte-string 1 */
#define SDP_FQDN1           0x00000040  /* FQDN 1 */
#define SDP_PHONE1          0x00000080  /* phone 1*/
#define SDP_PNUMBER1        0x00000100  /* phone number 1 */
#define SDP_KEY_DATA1       0x00000200  /* key-data 1 */
#define SDP_COMMA           0x00000400  /* comma */
#define SDP_HEXDIG1         0x00000800  /* HEXDIG 1 */
#define SDP_TKN1            0x00001000  /* token 1 */
#define SDP_EQUAL           0x00002000  /* equal */
#define SDP_UTF8_NONASCII1  0x00004000  /* nonascii 1 */
#define SDP_UTF8_NONASCII   0x00008000  /* nonascii */
#define SDP_QDTEXT1         0x00010000  /* qdtext 1 */
#define SDP_QDESC1          0x00020000  /* qdesc 1 */
#define SDP_COLON           0x00040000  /* colon */
#define SDP_UNRESERVED1     0x00080000  /* unreserved 1 */
#define SDP_DELIMS1         0x00100000  /* sub-delims 1 */
#define SDP_SESSION1        0x00200000  /* session 1 */
#define SDP_PCENT           0x00400000  /* '%' */
#define SDP_FCHR1           0x00800000  /* filename-char 1 */
#define SDP_FNAME1          0x01000000  /* filename-char 1 */
#define SDP_ATOM1           0x02000000  /* atom 1 */
#define SDP_DQUOTE          0x04000000  /* '"' */
#define SDP_ZONE1           0x08000000  /* zone 1 */
#define SDP_CTEXT1          0x10000000  /* ctext 1 */
#define SDP_FB_ID1          0x20000000  /* fb-id 1 */
#define SDP_ICE_CHAR1       0x40000000  /* ice-char 1 */
#define SDP_BASE641         0x80000000  /* base64 1 */

/* composite character set */
#define SDP_DIGIT (SDP_PDIGIT | SDP_DIGIT1) /* DIGIT */ 
#define SDP_ALPHA_NUM (SDP_ALPHA | SDP_DIGIT) /* alpha-numeric */  
#define SDP_NOWSSTR1 \
        (SDP_UTF8_NONASCII1 | SDP_UTF8_NONASCII) /* non-ws-string 1 */
#define SDP_ESAFE (SDP_ALPHA_NUM | SDP_ESAFE1 | SDP_NOWSSTR1) /* email-safe */
#define SDP_VCHR (SDP_ALPHA_NUM | SDP_VCHR1) /* VCHAR */
#define SDP_NOWSSTR (SDP_VCHR | SDP_NOWSSTR1) /* non-ws-string */
#define SDP_BSTR (SDP_ALPHA_NUM | SDP_NOWSSTR1 | SDP_BSTR1) /* byte-string */
#define SDP_FQDN (SDP_ALPHA_NUM | SDP_FQDN1) /* FQDN */
#define SDP_PHONE (SDP_DIGIT | SDP_PHONE1) /* phone */
#define SDP_PNUMBER (SDP_PHONE | SDP_ESAFE | SDP_PNUMBER1)  /* phone number */
#define SDP_KEY_DATA (SDP_ESAFE | SDP_KEY_DATA1)  /* key-data */
#define SDP_HEXDIG (SDP_DIGIT | SDP_HEXDIG1) /* HEXDIG */
#define SDP_TKN (SDP_ALPHA_NUM | SDP_TKN1) /* token */
#define SDP_UNRESERVED (SDP_UNRESERVED1 | SDP_ALPHA_NUM) /* unreserved */
#define SDP_BASE64 (SDP_ALPHA_NUM | SDP_BASE641) /* base64 */

#define SDP_TXT       SDP_BSTR       /* text */
#define SDP_USERNAME  SDP_NOWSSTR    /* username */
#define SDP_BWTYPE    SDP_TKN        /* bwtype */
#define SDP_MEDIA     SDP_TKN        /* media */
#define SDP_FMT       SDP_TKN        /* fmt */
#define SDP_AFIELD    SDP_TKN        /* att-field */
#define SDP_AVALUE    SDP_BSTR       /* att-value */
#define SDP_ENCODING  SDP_TKN        /* encoding name */
#define SDP_H263_ANX  (SDP_DIGIT | SDP_COMMA) /* h263 annex */
#define SDP_H264_PARM (SDP_TKN | SDP_COMMA | SDP_EQUAL) /* h264 parameter */
#define SDP_4629_STR  (SDP_DIGIT | SDP_COMMA) /* rfc4629 comma string */
#define SDP_3267_STR  (SDP_DIGIT | SDP_COMMA) /* rfc3267 comma string */
#define SDP_QDTEXT \
        (SDP_QDTEXT1 | SDP_TKN1| SDP_ALPHA_NUM | SDP_UTF8_NONASCII) /* qdtext */ 
#define SDP_QSTR (SDP_QDESC1 | SDP_QDTEXT) /* quoted-string */
#define SDP_PENCODE (SDP_HEXDIG | SDP_PCENT) /* pct-encoded */
#define SDP_REG (SDP_UNRESERVED | SDP_DELIMS1) /* reg-name */
#define SDP_REGNAME (SDP_REG | SDP_PENCODE) /* reg-name */
#define SDP_USER (SDP_REG | SDP_COLON ) /* userinfo */
#define SDP_USERINFO (SDP_USER | SDP_PENCODE) /* userinfo */
#define SDP_SESSION (SDP_SESSION1 | SDP_UNRESERVED) /* session-id */
#define SDP_FCHR (SDP_FCHR1 | SDP_FNAME1 | SDP_ALPHA_NUM | SDP_NOWSSTR1) /* filename-char */
#define SDP_ATOM (SDP_ATOM1 | SDP_ALPHA_NUM) /* atom */
#define SDP_QPAIR (SDP_BSTR1 | SDP_ALPHA_NUM) /* quoted-pair */
#define SDP_DTEXT (SDP_DQUOTE | SDP_PCENT | SDP_FCHR1 | SDP_ALPHA_NUM) /* dtext */
#define SDP_QTEXT (SDP_QDTEXT1 | SDP_PCENT | SDP_FCHR1 | SDP_ALPHA_NUM) /* qtext */
#define SDP_ZONE SDP_ZONE1 /* zone */
#define SDP_CTEXT (SDP_CTEXT1 | SDP_ALPHA_NUM) /* ctext */
#define SDP_FB_ID (SDP_ALPHA_NUM | SDP_FB_ID1) /* fb-id */
#define SDP_ICE_CHAR (SDP_ALPHA_NUM | SDP_ICE_CHAR1) /* ice-char */

/* sdp abnf get chrset */
ZFUNC ZCHRSET Sdp_ChrsetGetId(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif

#endif /* _SDP_ABNF_CHRSET_H__ */

