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
  File name     : ea_xdm_uusg.h
  Module        : eax oma-uriusage
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax oma-uriusage.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XDM_UUSG_H__
#define _EA_XDM_UUSG_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xdm oma-uriusage get the uriusages */
ZFUNC ZINT EaXdm_UusgGetUusgs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstUusgs);

/* xdm oma-uriusage get the first uriusages uriusage */
ZFUNC ZINT EaXdm_UusgUusgsGetFirstUusg(ST_XML_ELEM *pstLcf, 
                ST_XML_ELEM **ppstUusg);

/* xdm oma-uriusage get the next uriusages uriusage */
ZFUNC ZINT EaXdm_UusgUusgsGetNextUusg(ST_XML_ELEM *pstUusg, 
                ST_XML_ELEM **ppstUusg);

/* xdm oma-uriusage set the uriusages */
ZFUNC ZINT EaXdm_UusgSetUusgs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstUusgs);

/* xdm oma-uriusage set the uriusages uriusage */
ZFUNC ZINT EaXdm_UusgUusgsSetUusg(ST_XML_ELEM *pstLcf, 
                ST_XML_ELEM **ppstUusg);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XDM_UUSG_H__ */

