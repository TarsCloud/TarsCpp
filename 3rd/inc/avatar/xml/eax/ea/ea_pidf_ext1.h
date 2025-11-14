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
  File name     : ea_pidf_ext1.h
  Module        : eax pde pidf ext1
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax pde pidf ext1.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PIDF_EXT1_H__
#define _EA_PIDF_EXT1_H__

#ifdef __cplusplus
extern "C"{
#endif

/* pde pidf ext1 get link */
ZFUNC ZINT EaPidf_Ext1GetLink(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstLink);

/* pde pidf ext1 get label */
ZFUNC ZINT EaPidf_Ext1GetLabel(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR **ppstLabel);

/* pde pidf ext1 get fsize */
ZFUNC ZINT EaPidf_Ext1GetPriority(ST_XML_ELEM *pstElem, 
                ZUINT *piPriority);

/* pde pidf ext1 set link */
ZFUNC ZINT EaPidf_Ext1SetLink(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstLink);

/* pde pidf ext1 set label */
ZFUNC ZINT EaPidf_Ext1SetLabel(ST_XML_ELEM *pstElem, 
                ST_ZOS_USTR *pstLabel);

/* pde pidf ext1 set fsize */
ZFUNC ZINT EaPidf_Ext1SetPriority(ST_XML_ELEM *pstElem, 
                ZUINT iPriority);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PIDF_EXT1_H__ */

