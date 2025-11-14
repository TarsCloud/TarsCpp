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
  File name     : ea_xdm_ausg.h
  Module        : eax oma-appusage
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax oma-appusage.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_XDM_AUSG_H__
#define _EA_XDM_AUSG_H__

#ifdef __cplusplus
extern "C"{
#endif

/* xdm oma-appusage get the appusages */
ZFUNC ZINT EaXdm_AusgGetAusgs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstUusgs);

/* xdm oma-appusage get the first appusages nodeuri */
ZFUNC ZINT EaXdm_AusgAusgsGetFirstNodeUri(ST_XML_ELEM *pstUusgs, 
                ST_XML_ELEM **ppstNodeUri);

/* xdm oma-appusage get the next appusages nodeuri */
ZFUNC ZINT EaXdm_AusgAusgsGetNextNodeUri(ST_XML_ELEM *pstNodeUri, 
                ST_XML_ELEM **ppstNodeUri);

/* xdm oma-appusage get the nodeuri uri */
ZFUNC ZINT EaXdm_AusgNodeUriGetUri(ST_XML_ELEM *pstNodeUri, 
                ST_ZOS_USTR **ppstUri);

/* xdm oma-appusage get the nodeuri auid */
ZFUNC ZINT EaXdm_AusgNodeUriGetAuid(ST_XML_ELEM *pstNodeUri, 
                ST_ZOS_USTR **ppstAuid);

/* xdm oma-appusage set the appusages */
ZFUNC ZINT EaXdm_AusgSetAusgs(ST_XML_ELEM *pstElem, 
                ST_XML_ELEM **ppstUusgs);

/* xdm oma-appusage set the appusages nodeuri */
ZFUNC ZINT EaXdm_AusgAusgsSetNodeUri(ST_XML_ELEM *pstUusgs, 
                ST_XML_ELEM **ppstNodeUri);

/* xdm oma-appusage set the nodeuri uri */
ZFUNC ZINT EaXdm_AusgNodeUriSetUri(ST_XML_ELEM *pstNodeUri, 
                ST_ZOS_USTR *pstUri);

/* xdm oma-appusage set the nodeuri auid */
ZFUNC ZINT EaXdm_AusgNodeUriSetAuid(ST_XML_ELEM *pstNodeUri, 
                ST_ZOS_USTR *pstAuid);

#ifdef __cplusplus
}
#endif

#endif /* _EA_XDM_AUSG_H__ */

