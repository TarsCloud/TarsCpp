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
  File name     : ea_im_isc.h
  Module        : eax im-iscomposing
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax im-iscomposing.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_IM_ISC_H__
#define _EA_IM_ISC_H__

#ifdef __cplusplus
extern "C"{
#endif

/* im-iscomposing get the isComposing */
ZFUNC ZINT EaIm_IscGetIsc(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstIsc);

/* im-iscomposing get the isComposing state */
ZFUNC ZINT EaIm_IscIscGetSta(ST_XML_ELEM *pstIsc, ST_ZOS_USTR **ppstSta);

/* im-iscomposing get the isComposing lastactive */
ZFUNC ZINT EaIm_IscIscGetLastActive(ST_XML_ELEM *pstIsc, 
                ST_ZOS_USTR **ppstLastActive);

/* im-iscomposing get the isComposing contenttype */
ZFUNC ZINT EaIm_IscIscGetLastContentType(ST_XML_ELEM *pstIsc, 
                ST_ZOS_USTR **ppstContentType);

/* im-iscomposing get the isComposing refresh */
ZFUNC ZINT EaIm_IscIscGetRefresh(ST_XML_ELEM *pstIsc, ZUINT *piRefresh);

/* im-iscomposing set the isComposing */
ZFUNC ZINT EaIm_IscSetIsc(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstIsc);

/* im-iscomposing set the isComposing state */
ZFUNC ZINT EaIm_IscIscSetSta(ST_XML_ELEM *pstIsc, ST_ZOS_USTR *pstSta);

/* im-iscomposing set the isComposing lastactive */
ZFUNC ZINT EaIm_IscIscSetLastActive(ST_XML_ELEM *pstIsc, 
                ST_ZOS_USTR *pstLastActive);

/* im-iscomposing set the isComposing contenttype */
ZFUNC ZINT EaIm_IscIscSetLastContentType(ST_XML_ELEM *pstIsc, 
                ST_ZOS_USTR *pstContentType);

/* im-iscomposing set the isComposing refresh */
ZFUNC ZINT EaIm_IscIscSetRefresh(ST_XML_ELEM *pstIsc, ZUINT iRefresh);

#ifdef __cplusplus
}
#endif

#endif /* _EA_IM_ISC_H__ */

