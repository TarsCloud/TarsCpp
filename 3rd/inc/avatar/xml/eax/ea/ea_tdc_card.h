/************************************************************************

        Copyright (c) 2005-2014 by Juphoon System Software, Inc.
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
  File name     : ea_tdc_card.h
  Module        : eax tdc card
  Author        : zocol
  Version       : 0.1
  Created on    : 2014-09-06
  Description   :
    Marcos and structure definitions required by the eax tdc card.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_TDC_CARD_H__
#define _EA_TDC_CARD_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:oma:xml:twoDimensionCodeCard **************/
typedef enum EN_EA_TDC_CARD_TYPE
{
    EN_EA_TDC_CARD_PCC_CTT,             /* pcc-content */
    EN_EA_TDC_CARD_CTT,                 /* content */
    EN_EA_TDC_CARD_FLAG,                /* flag */
    EN_EA_TDC_CARD_MIME_TYPE,           /* mime-type */
    EN_EA_TDC_CARD_ENCODING,            /* encoding */
    EN_EA_TDC_CARD_DESCRIPTION,         /* description */
    EN_EA_TDC_CARD_DATA,                /* data */
    EN_EA_TDC_CARD_IMAGE_JPG,           /* image/jpg */
    EN_EA_TDC_CARD_IMAGE_PNG,           /* image/png */
    EN_EA_TDC_CARD_IMAGE_GIF,           /* image/gif */
    EN_EA_TDC_CARD_BASE64,              /* base64 */
} EN_EA_TDC_CARD_TYPE;

/* eax tdc card mime type */
typedef enum EN_EA_TDC_CARD_MTYPE_TYPE
{
    EN_EA_TDC_CARD_MTYPE_IMAGE_JPG,     /* image/jpg */
    EN_EA_TDC_CARD_MTYPE_IMAGE_PNG,     /* image/png */
    EN_EA_TDC_CARD_MTYPE_IMAGE_GIF,     /* image/gif */
} EN_EA_TDC_CARD_MTYPE_TYPE;

/* eax tdc card encoding type */
typedef enum EN_EA_TDC_CARD_ETYPE_TYPE
{
    EN_EA_TDC_CARD_ETYPE_BASE64         /* base64 */
} EN_EA_TDC_CARD_ETYPE_TYPE;

/* eax tdc card get the pcc-content */
ZFUNC ZINT EaTdc_CardGetPccCtt(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstPccCtt);

/* eax tdc card get the pcc-content content */
ZFUNC ZINT EaTdc_CardPccCttGetCtt(ST_XML_ELEM *pstPccCtt, 
                ST_XML_ELEM **ppstCtt);

/* eax tdc card get the pcc-content content */
ZFUNC ZINT EaTdc_CardPccCttGetFlag(ST_XML_ELEM *pstPccCtt, 
                ZINT *piFlag);

/* eax tdc card get the content mime-type, type see EN_EA_TDC_CARD_MTYPE_TYPE */
ZFUNC ZINT EaTdc_CardCttGetMimeType(ST_XML_ELEM *pstCtt, 
                ZUCHAR *pucType);

/* eax tdc card get the content encoding, type see EN_EA_TDC_CARD_ETYPE_TYPE */
ZFUNC ZINT EaTdc_CardCttGetEncoding(ST_XML_ELEM *pstCtt, 
                ZUCHAR *pucType);

/* eax tdc card get the content description */
ZFUNC ZINT EaTdc_CardCttGetDesc(ST_XML_ELEM *pstCtt, 
                ST_ZOS_USTR **ppstDesc);

/* eax tdc card get the content data */
ZFUNC ZINT EaTdc_CardCttGetData(ST_XML_ELEM *pstCtt, 
                ST_ZOS_USTR **ppstData);

/* eax tdc card set the pcc-content */
ZFUNC ZINT EaTdc_CardSetPccCtt(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstPccCtt);

/* eax tdc card set the pcc-content content */
ZFUNC ZINT EaTdc_CardPccCttSetCtt(ST_XML_ELEM *pstPccCtt, 
                ST_XML_ELEM **ppstCtt);

/* eax tdc card set the pcc-content content */
ZFUNC ZINT EaTdc_CardPccCttSetFlag(ST_XML_ELEM *pstPccCtt, 
                ZINT iFlag);

/* eax tdc card set the content mime-type, type see EN_EA_TDC_CARD_MTYPE_TYPE */
ZFUNC ZINT EaTdc_CardCttSetMimeType(ST_XML_ELEM *pstCtt, 
                ZUCHAR ucType);

/* eax tdc card set the content encoding, type see EN_EA_TDC_CARD_ETYPE_TYPE */
ZFUNC ZINT EaTdc_CardCttSetEncoding(ST_XML_ELEM *pstCtt, 
                ZUCHAR ucType);

/* eax tdc card set the content description */
ZFUNC ZINT EaTdc_CardCttSetDesc(ST_XML_ELEM *pstCtt, 
                ST_ZOS_USTR *pstDesc);

/* eax tdc card set the content data */
ZFUNC ZINT EaTdc_CardCttSetData(ST_XML_ELEM *pstCtt, 
                ST_ZOS_USTR *pstData);

#ifdef __cplusplus
}
#endif

#endif /* _EA_TDC_CARD_H__ */

