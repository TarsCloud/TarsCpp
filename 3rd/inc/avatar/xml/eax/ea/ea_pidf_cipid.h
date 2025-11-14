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
  File name     : ea_pidf_cipid.h
  Module        : eax pidf cipid
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pidf cipid.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_CIPID_H__
#define _EA_PIDF_CIPID_H__

#ifdef __cplusplus
extern "C"{
#endif

/************ urn:ietf:params:xml:ns:pidf:cipid **********/
typedef enum EN_EA_PIDF_CIPID_TYPE
{
    EN_EA_PIDF_CIPID_CARD,        /* card */
    EN_EA_PIDF_CIPID_DISP_NAME,   /* display-name */
    EN_EA_PIDF_CIPID_HOMEPAGE,    /* homepage */
    EN_EA_PIDF_CIPID_ICON,        /* icon */
    EN_EA_PIDF_CIPID_MAP,         /* map */
    EN_EA_PIDF_CIPID_SOUND        /* sound */
} EN_EA_PIDF_CIPID_TYPE;

/* pidf cipid get the card */
ZFUNC ZINT EaPidf_CipidGetCard(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstCard);

/* pidf cipid get the display-name */
ZFUNC ZINT EaPidf_CipidGetDispName(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstDispName);

/* pidf cipid get the homepage */
ZFUNC ZINT EaPidf_CipidGetHomepage(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstHomepage);

/* pidf cipid get the icon */
ZFUNC ZINT EaPidf_CipidGetIcon(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstIcon);

/* pidf cipid get the map */
ZFUNC ZINT EaPidf_CipidGetMap(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstMap);

/* pidf cipid get the sound */
ZFUNC ZINT EaPidf_CipidGetSound(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstSound);

/* pidf cipid set the card */
ZFUNC ZINT EaPidf_CipidSetCard(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstCard);

/* pidf cipid set the display-name */
ZFUNC ZINT EaPidf_CipidSetDispName(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstDispName);

/* pidf cipid set the homepage */
ZFUNC ZINT EaPidf_CipidSetHomepage(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstHomepage);

/* pidf cipid set the icon */
ZFUNC ZINT EaPidf_CipidSetIcon(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstIcon);

/* pidf cipid set the map */
ZFUNC ZINT EaPidf_CipidSetMap(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstMap);

/* pidf cipid set the sound */
ZFUNC ZINT EaPidf_CipidSetSound(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstSound);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_CIPID_H__ */

