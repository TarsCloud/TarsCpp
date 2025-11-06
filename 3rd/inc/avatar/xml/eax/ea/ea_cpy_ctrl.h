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
  File name     : ea_cpy_ctrl.h
  Module        : eax copy control
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax copy control.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_CPY_CTRL_H__
#define _EA_CPY_CTRL_H__

#ifdef __cplusplus
extern "C"{
#endif

/********* urn:ietf:params:xml:ns:copycontrol *********/
typedef enum EN_EA_CPY_CTRL_TYPE
{
    EN_EA_CPY_CTRL_CTRL,           /* copy control */
    EN_EA_CPY_CTRL_TO,             /* to */
    EN_EA_CPY_CTRL_CC,             /* cc */
    EN_EA_CPY_CTRL_BCC             /* bcc */
} EN_EA_CPY_CTRL_TYPE;

/* eax copycontrol get the control type, type see EN_EA_CPY_CTRL_TYPE */
ZFUNC ZINT EaCpy_CtrlGetCtrl(ST_XML_ELEM *pstElem, ZUCHAR *pucCtrlType);

/* eax copycontrol set the control type */
ZFUNC ZINT EaCpy_CtrlSetCtrl(ST_XML_ELEM *pstElem, ZUCHAR ucCtrlType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_CPY_CTRL_H__ */

