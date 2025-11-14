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
  File name     : eax_map.h
  Module        : extension api for xml
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-05-22
  Description   :
    Marcos and structure definitions required by the eax map.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EAX_MAP_H__
#define _EAX_MAP_H__

#ifdef __cplusplus
extern "C"{
#endif

/* eax map get namespace id from string */
ZFUNC ZINT Eax_MapGetNsId(ST_ZOS_USTR *pstStr, ZUINT *piNsId);

/* eax map get namespace string from id */
ZFUNC ZINT Eax_MapGetNsStr(ZUINT iNsId, ST_ZOS_USTR *pstStr);

/* eax map get namespace string from id */
ZFUNC ZINT Eax_MapGetNsStrX(ZUINT iNsId, ST_ZOS_SSTR *pstStr);

/* eax map get token id from string */
ZFUNC ZINT Eax_MapGetTknId(ZUINT iNsId, ST_ZOS_USTR *pstStr, 
                ZTKNID *pzTknId);

/* xsp map get token string from id */
ZFUNC ZINT Eax_MapGetTknStr(ZUINT iNsId, ZTKNID zTknId, 
                ST_ZOS_USTR *pstStr);

/* xsp map get token string from id */
ZFUNC ZINT Eax_MapGetTknStrX(ZUINT iNsId, ZTKNID zTknId, 
                ST_ZOS_SSTR *pstStr);

#ifdef __cplusplus
}
#endif

#endif /* _EAX_MAP_H__ */

