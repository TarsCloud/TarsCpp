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
  File name     : ea_xdm_err.h
  Module        : eax xdm xcap-error
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax xdm xcap-error.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XDM_XERR_H__
#define _EA_XDM_XERR_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xdm xcap-error get the local-constraint-failure */
ZFUNC ZINT EaXdm_XerrGetLocalCnstFail(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstLocalCnstFail);

/* xdm xcap-error get the local-constraint-failure alt-value */
ZFUNC ZINT EaXdm_XerrLcfGetAltVal(ST_XML_ELEM *pstLcf, 
                ST_ZOS_USTR *pstField, ST_XML_ELEM **ppstAltVal);

/* xdm xcap-error get the first local-constraint-failure alt-value */
ZFUNC ZINT EaXdm_XerrLcfGetFirstAltVal(ST_XML_ELEM *pstLcf, 
                ST_XML_ELEM **ppstAltVal);

/* xdm xcap-error get the next local-constraint-failure alt-value */
ZFUNC ZINT EaXdm_XerrLcfGetNextAltVal(ST_XML_ELEM *pstAltVal, 
                ST_XML_ELEM **ppstAltVal);

/* xdm xcap-error get the first local-constraint-failure description */
ZFUNC ZINT EaXdm_XerrLcfGetFirstDesc(ST_XML_ELEM *pstLcf, 
                ST_XML_ELEM **ppstDesc);

/* xdm xcap-error get the next local-constraint-failure description */
ZFUNC ZINT EaXdm_XerrLcfGetNextDesc(ST_XML_ELEM *pstDesc, 
                ST_XML_ELEM **ppstDesc);

/* xdm xcap-error get the local-constraint-failure phrase */
ZFUNC ZINT EaXdm_XerrLcfGetPhrase(ST_XML_ELEM *pstLcf, 
                ST_ZOS_USTR **ppstPhrase);

/* xdm xcap-error get the alt-value field */
ZFUNC ZINT EaXdm_XerrAltValGetField(ST_XML_ELEM *pstAltVal, 
                ST_ZOS_USTR **ppstField);

/* xdm xcap-error get the description lang */
ZFUNC ZINT EaXdm_XerrDescGetLang(ST_XML_ELEM *pstDesc, 
                ST_ZOS_USTR **ppstLang);

/* xdm xcap-error set the local-constraint-failure */
ZFUNC ZINT EaXdm_XerrSetLocalCnstFail(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstLocalCnstFail);

/* xdm xcap-error set the local-constraint-failure alt-value */
ZFUNC ZINT EaXdm_XerrLcfSetAltVal(ST_XML_ELEM *pstLcf, 
                ST_XML_ELEM **ppstAltVal);

/* xdm xcap-error set the local-constraint-failure description */
ZFUNC ZINT EaXdm_XerrLcfSetDesc(ST_XML_ELEM *pstLcf, 
                ST_XML_ELEM **ppstDesc);

/* xdm xcap-error set the local-constraint-failure phrase */
ZFUNC ZINT EaXdm_XerrLcfSetPhrase(ST_XML_ELEM *pstLcf, 
                ST_ZOS_USTR *pstPhrase);

/* xdm xcap-error set the alt-value field */
ZFUNC ZINT EaXdm_XerrAltValSetField(ST_XML_ELEM *pstAltVal, 
                ST_ZOS_USTR *pstField);

/* xdm xcap-error set the description lang */
ZFUNC ZINT EaXdm_XerrDescSetLang(ST_XML_ELEM *pstDesc, 
                ST_ZOS_USTR *pstLang);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XDM_XERR_H__ */

