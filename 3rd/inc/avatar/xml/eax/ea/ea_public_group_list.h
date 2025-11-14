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
  File name     : ea_public_group_list.h
  Module        : eax public group list
  Author        : gloria.yang
  Version       : 0.1
  Created on    : 2018-05-25
  Description   :
    Marcos and structure definitions required by the eax public-gourp-list.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_PUBLIC_GROUP_LIST_H__
#define _EA_PUBLIC_GROUP_LIST_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum EN_EA_PUBLIC_GROUP_LIST_TYPE
{
    EN_EA_PUBGRPLST_URI,            /* uri */
    EN_EA_PUBGRPLST_NAME,           /* name */
    EN_EA_PUBGRPLST_TRUE,           /* true */
    EN_EA_PUBGRPLST_FALSE,          /* false */ 
    EN_EA_PUBGRPLST_GROUP,          /* group */
    EN_EA_PUBGRPLST_GROUPTYPE,      /* group-type */
    EN_EA_PUBGRPLST_CONVID,         /* conversationId */
    EN_EA_PUBGRPLST_PUBGRPLST,      /* public-group-list */
} EN_EA_PUBLIC_GROUP_LIST_TYPE;

/* public-group-list get the public-group-list */
ZFUNC ZINT EaPub_GrpLstGetPubGrpLst(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstPubGrpLst);

/* public-group-list get the first group */
ZFUNC ZINT EaPub_GrpLstGetFirstGroup(ST_XML_ELEM *pstPubGrpLst, 
                ST_XML_ELEM **ppstGroup);

/* public-group-list get the next group */
ZFUNC ZINT EaPub_GrpLstGetNextGroup(ST_XML_ELEM *pstGroup, 
                ST_XML_ELEM **ppstGroup);

/* public-group-list get group uri data */
ZFUNC ZINT EaPub_GrpLstGroupGetUriData(ST_XML_ELEM *pstGroup, 
                ST_ZOS_USTR **ppstUri);

/* public-group-list get group name data */
ZFUNC ZINT EaPub_GrpLstGroupGetNameData(ST_XML_ELEM *pstGroup, 
                ST_ZOS_USTR **ppstName);

/* public-group-list get group conversationId data */
ZFUNC ZINT EaPub_GrpLstGroupGetConvIdData(ST_XML_ELEM *pstGroup, 
                ST_ZOS_USTR **ppstConvId);

/* public-group-list get the group group-type data */
ZFUNC ZINT EaPub_GrpLstGroupGetGroupTypeData(ST_XML_ELEM *pstGroup, 
                ZUINT *piGroupType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_PUBLIC_GROUP_LIST_H__ */

