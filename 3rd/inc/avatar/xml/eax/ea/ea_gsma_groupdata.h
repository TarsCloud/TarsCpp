/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : ea_gsma_groupdata.h
  Module        : eax gsma groupdata
  Author        : shuai.xu
  Version       : 0.1
  Created on    : 2017-07-12
  Description   :
    Marcos and structure definitions required by the ea gsma groupdata.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_GSMA_GROUPDATA_H__
#define _EA_GSMA_GROUPDATA_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:oma:xml:cpm:groupdata **************/
typedef enum EN_EA_GSMA_GROUPDATA_TYPE
{
    EN_EA_GSMA_GROUPDATA_ID,
    EN_EA_GSMA_GROUPDATA_SET,
    EN_EA_GSMA_GROUPDATA_ICON,
    EN_EA_GSMA_GROUPDATA_DATA,
    EN_EA_GSMA_GROUPDATA_ACTION,
    EN_EA_GSMA_GROUPDATA_TARGET,
    EN_EA_GSMA_GROUPDATA_DELETE,
    EN_EA_GSMA_GROUPDATA_SUBJECT,
    EN_EA_GSMA_GROUPDATA_REQUEST,
    EN_EA_GSMA_GROUPDATA_ICONURI,
    EN_EA_GSMA_GROUPDATA_FILEINFO,
    EN_EA_GSMA_GROUPDATA_RESPONSE,
    EN_EA_GSMA_GROUPDATA_CHAIRMAN,
    EN_EA_GSMA_GROUPDATA_GROUPDATA,
    EN_EA_GSMA_GROUPDATA_RESPONSETEXT,
    EN_EA_GSMA_GROUPDATA_RESPONSECODE,
    EN_EA_GSMA_GROUPDATA_CPMGROUPMANAGE,
} EN_EA_GSMA_GROUPDATA_TYPE;

/* eax gsma shareddata get token id from string */
ZFUNC ZINT EaGsmaGroupData_TknStr2Id(ST_ZOS_USTR *pstStr, ZTKNID *pzTknId);

/* eax gsma shareddata get token string from id */
ZFUNC ZINT EaGsmaGroupData_TknId2Str(ZTKNID zTknId, ST_ZOS_USTR *pstStr);

/* eax gsma groupdata get the cpm group data management */
ZFUNC ZINT EaGsmaGroupData_GetCpmGrpDataManage(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstCpmGrpDatamanage);

/* eax gsma groupdata cpm group data management get the id */
ZFUNC ZINT EaGsmaGroupData_CpmGrpDataManageGetId(ST_XML_ELEM *pstCpmGrpDatamanage, 
                ZUINT *piId);

/* eax gsma groupdata cpm group data management get the groupdata */
ZFUNC ZINT EaGsmaGroupData_CpmGrpDataManageGetGroupData(ST_XML_ELEM *pstCpmGrpDatamanage, 
                ST_XML_ELEM **ppstGroupData);

/* eax gsma groupdata groupdata get the request */
ZFUNC ZINT EaGsmaGroupData_GroupDataGetRequest(ST_XML_ELEM *pstGroupData, 
                ST_XML_ELEM **ppstRequest);

/* eax gsma groupdata request get the target */
ZFUNC ZINT EaGsmaGroupData_RequestGetTarget(ST_XML_ELEM *pstRequest, 
                ST_ZOS_USTR **ppstTarget);

/* eax gsma groupdata request get the action */
ZFUNC ZINT EaGsmaGroupData_RequestGetActionValue(ST_XML_ELEM *pstRequest, 
                ST_ZOS_USTR **ppstActionData);

/* eax gsma groupdata request get the data */
ZFUNC ZINT EaGsmaGroupData_RequestGetData(ST_XML_ELEM *pstRequest, 
                ST_XML_ELEM **ppstData);

/* eax gsma groupdata data get the subject value */
ZFUNC ZINT EaGsmaGroupData_DataGetSubjectValue(ST_XML_ELEM *pstData, 
                ST_ZOS_USTR **ppstSubjectValue);

/* eax gsma groupdata data get the icon */
ZFUNC ZINT EaGsmaGroupData_DataGetIcon(ST_XML_ELEM *pstData, 
                ST_XML_ELEM **ppstIcon);

/* eax gsma groupdata icon get the icon-uri value */
ZFUNC ZINT EaGsmaGroupData_IconGetIconUriValue(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR **ppstIconUriValue);

/* eax gsma groupdata icon get the file-info value */
ZFUNC ZINT EaGsmaGroupData_IconGetFileInfoValue(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR **ppstFileInfoValue);

/* eax gsma groupdata groupdata get the response */
ZFUNC ZINT EaGsmaGroupData_GroupDataGetResponse(ST_XML_ELEM *pstGroupData, 
                ST_XML_ELEM **ppstResponse);

/* eax gsma groupdata response get the response-code */
ZFUNC ZINT EaGsmaGroupData_ResponseGetResponseCodeValue(ST_XML_ELEM *pstResponse, 
                ZUINT *piResponseCodeValue);

/* eax gsma groupdata response get the response-text */
ZFUNC ZINT EaGsmaGroupData_ResponseGetResponseTextValue(ST_XML_ELEM *pstResponse, 
                ST_ZOS_USTR **ppstResponseTextValue);

/* eax gsma groupdata set the cpm group session data manage */
ZFUNC ZINT EaGsmaGroupData_SetCpmGrpDataManage(ST_XML_MSG *pstMsg, ST_EAX_NS *pstNs, 
                ST_XML_ELEM **ppstEnv);

/* eax gsma groupdata cpm group session data manage set the id */
ZFUNC ZINT EaGsmaGroupData_CpmGrpDataManageSetId(ST_XML_ELEM *pstCpmGrpDatamanage, 
                ZUINT iId);

/* eax gsma groupdata cpm group session data manage set the groupdata */
ZFUNC ZINT EaGsmaGroupData_CpmGrpDataManageSetGroupData(ST_XML_ELEM *pstCpmGrpDatamanage, 
                ST_XML_ELEM **ppstGroupData);

/* eax gsma groupdata groupdata set the request */
ZFUNC ZINT EaGsmaGroupData_GroupDataSetRequest(ST_XML_ELEM *pstGroupData, 
                ST_XML_ELEM **ppstRequest);

/* eax gsma groupdata request set the target */
ZFUNC ZINT EaGsmaGroupData_RequestSetTarget(ST_XML_ELEM *pstRequest, 
                ST_ZOS_USTR *pstTarget);

/* eax gsma groupdata request set the action value */
ZFUNC ZINT EaGsmaGroupData_RequestSetActionValue(ST_XML_ELEM *pstRequest, 
                ST_ZOS_USTR *pstActionValue);

/* eax gsma groupdata request set the data */
ZFUNC ZINT EaGsmaGroupData_RequestSetData(ST_XML_ELEM *pstRequest, 
                ST_XML_ELEM **ppstData);

/* eax gsma groupdata data set the subject value */
ZFUNC ZINT EaGsmaGroupData_DataSetSubjectValue(ST_XML_ELEM *pstData, 
                ST_ZOS_USTR *pstSubjectValue);

/* eax gsma groupdata data set the chairman value */
ZFUNC ZINT EaGsmaGroupData_DataSetChairmanValue(ST_XML_ELEM *pstData, 
                ST_ZOS_USTR *pstChairmanValue);

/* eax gsma groupdata data set the icon */
ZFUNC ZINT EaGsmaGroupData_DataSetIcon(ST_XML_ELEM *pstData, 
                ST_XML_ELEM **ppstIcon);

/* eax gsma groupdata icon set the icon-uri value */
ZFUNC ZINT EaGsmaGroupData_IconSetIconUriValue(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR *pstIconUriValue);

/* eax gsma groupdata icon set the file-info value */
ZFUNC ZINT EaGsmaGroupData_IconSetFileInfoValue(ST_XML_ELEM *pstIcon, 
                ST_ZOS_USTR *pstFileInfoValue);

/* eax gsma groupdata groupdata set the response */
ZFUNC ZINT EaGsmaGroupData_GroupDataSetResponse(ST_XML_ELEM *pstGroupData, 
                ST_XML_ELEM **ppstResponse);

/* eax gsma groupdata reponse set response-code */
ZFUNC ZINT EaGsmaSharedData_ResponseSetResponseCodeValue(ST_XML_ELEM *pstResponse, 
                ZUINT iResponseCode);

/* eax gsma groupdata reponse set response-text */
ZFUNC ZINT EaGsmaSharedData_ResponseSetResponseTextValue(ST_XML_ELEM *pstResponse, 
                ST_ZOS_USTR *pstReponseText);

#ifdef __cplusplus
}
#endif

#endif /* _EA_GSMA_GROUPDATA_H__ */

