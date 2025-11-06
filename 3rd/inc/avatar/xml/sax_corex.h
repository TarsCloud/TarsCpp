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
  File name     : sax_corex.h
  Module        : saxx core interfaces
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Data structure and function definitions required by 
      the saxx core interfaces
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SAX_COREX_H__
#define _SAX_COREX_H__

/** 
 * @file sax_corex.h
 * @brief XML SAX extension interfaces.
 *
 * @see 
 */

#ifdef __cplusplus
extern "C"{
#endif
  
/** @brief Typedef of callback which invoked when parse an element start. */
typedef ZVOID (*PFN_SAXXELEMSTART)(ZVOID *pAct, ST_ZOS_USTR *pstName,
                ST_XML_ATTR_LST *pstAttrLst);

/** @brief Typedef of callback which invoked when parse an element ending. */
typedef ZVOID (*PFN_SAXXELEMEND)(ZVOID *pAct, ST_ZOS_USTR *pstName);

/** @brief Typedef of callback which invoked when parse an element data. */
typedef ZVOID (*PFN_SAXXELEMDATA)(ZVOID *pAct, ST_ZOS_USTR *pstName, 
                ST_ZOS_USTR *pstData);

/**
 * @brief SAXX action set an element action.
 *
 * @param [in] pAct Saxx action.
 * @param [in] pfnStart Callback which invoked when parse an element start.
 * @param [in] pfnEnd Callback which invoked when parse an element end.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetElemAction(ZVOID *pAct, PFN_SAXXELEMSTART pfnStart, 
                PFN_SAXXELEMEND pfnEnd);

/**
 * @brief SAXX action set a chardata action.
 *
 * @param [in] pfnChrData callback which invoked when parse an element data.
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetDataAction(ZVOID *pAct, PFN_SAXXELEMDATA pfnChrData);

/**
 * @brief SAXX local step set the object in the action.
 *
 * @param [in] pAct Saxx action.
 * @param [in] pObj The object in this step.
 *
 * @retval ZOK Set the object successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetStepObject(ZVOID *pAct, ZVOID *pObj);

/**
 * @brief SAXX local gtep set the object in the action.
 *
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Get the object successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZVOID * SaxX_ActGetStepObject(ZVOID *pAct);

/**
 * @brief SAXX global step set the object in the action.
 *
 * @param [in] pAct Saxx action.
 * @param [in] pObj The object in this step.
 *
 * @retval ZOK Set the object successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetUserObject(ZVOID *pAct, ZVOID *pObj);

/**
 * @brief SAXX global gtep set the object in the action.
 *
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Get the object successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZVOID * SaxX_ActGetUserObject(ZVOID *pAct);

/**
 * @brief SAXX action set the step parameter.
 *
 * @param [in] pAct Saxx action.
 * @param [in] iParm The parameter in this step.
 *
 * @retval ZOK set the paremeter successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetStepParm(ZVOID *pAct, ZUINT iParm);

/**
 * @brief SAXX action get the step parameter.
 *
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Get the parameter successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZUINT SaxX_ActGetStepParm(ZVOID *pAct);

/**
 * @brief user step set the parameter in the action.
 *
 * @param [in] pAct Saxx action.
 * @param [in] iParm The parameter in this step.
 *
 * @retval ZOK set the paremeter successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetUserParm(ZVOID *pAct, ZUINT iParm);

/**
 * @brief SAXX user step get the parameter in the action.
 *
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Get the parameter successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZUINT SaxX_ActGetUserParm(ZVOID *pAct);

/**
 * @brief SAXX action get the step parameter address.
 *
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Get the parameter successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZUINT * SaxX_ActGetStepParmAddr(ZVOID *pAct);

/**
 * @brief SAXX action get the child deep, 0 for root element.
 *
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Get the child deep successful.
 * @retval ZFAILED Otherwise. 
 *          
 * @see 
 */
ZFUNC ZUINT SaxX_ActGetChildDeep(ZVOID *pAct);

/**
 * @brief SAXX action get the parent's name, 0 for self element.
 *
 * @param [in] pAct Saxx action.
 * @param [in] iDeep The deep it decides the parents.
 * @param [in] pstName To get the parent's name.
 *
 * @retval ZOK Get the parent's name successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT SaxX_ActGetParent(ZVOID *pAct, ZUINT iDeep, 
            ST_ZOS_USTR *pstName);

/**
 * @brief SAXX action check the child's parent, 0 for self element.
 *
 * @param [in] pAct Saxx action.
 * @param [in] iDeep The deep it decides the parents.
 * @param [in] pcName To get the parent's name for compare.
 *
 * @retval ZOK Check the parent's name successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZBOOL SaxX_ActChkParent(ZVOID *pAct, ZUINT iDeep, ZCHAR *pcName);

/**
 * @brief SAXX get the attribute list of current element.
 *
 * @param [in] pAct Saxx action.
 *
 * @retval ZOK Get the list successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ST_XML_ATTR_LST * SaxX_ActGetAttrLst(ZVOID *pAct);

/**
 * @brief SAXX action set ignore visit element & data of all children.
 *
 * @param [in] pAct Saxx action.
 * @param [in] bIgnVisit A flag,if the value is true then ignore to 
 *                       visit element & data of all children,or not ignore.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetIgnChilds(ZVOID *pAct, ZBOOL bIgnVisit);

/**
 * @brief SAXX action set ignore visit element start of all children.
 *
 * @param [in] pAct Saxx action.
 * @param [in] bIgnVisit A flag.A flag,if the value is true then ignore to 
 *                     visit element start of all children,or not ignore.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetIgnChildsStart(ZVOID *pAct, ZBOOL bIgnVisit);

/**
 * @brief SAXX action set ignore visit element end of all children.
 *
 * @param [in] pAct Saxx action.
 * @param [in] bIgnVisit A flag,if the value is true then ignore to 
 *                       visit element end of all children,or not ignore.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetIgnChildsEnd(ZVOID *pAct, ZBOOL bIgnVisit);

/**
 * @brief SAXX action set ignore visit chardata of all children.
 *
 * @param [in] pAct Saxx action.
 * @param [in] bIgnVisit A flag,if the value is true then ignore to 
 *                       visit chardata  end of all children,or not ignore.
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetIgnChildsData(ZVOID *pAct, ZBOOL bIgnVisit);

/**
 * @brief SAXX action set ignore visit chardata of all siblings.
 *
 * @param [in] pAct Saxx action.
 * @param [in] bIgnVisit A flag,if the value is true then ignore to 
 *                       visit siblings end of all siblings,or not ignore.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see
 */
ZFUNC ZINT SaxX_ActSetIgnSiblings(ZVOID *pAct, ZBOOL bIgnVisit);

/**
 * @brief SAXX action set error status and stop continue parse.
 *
 * @param [in] pAct Saxx action.
 * @param [in] iErrNo User defined error number.
 *
 * @retval ZOK Set successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT SaxX_ActSetErrStop(ZVOID *pAct, ZINT iErrNo);

/************************* Sax Paser ****************************/
/**
 * @brief SAXX parse an xml data.
 *
 * @param [in] pstData An xml data.
 * @param [in] pfnStart Callback which invoked when parse an element start.
 * @param [in] pStepObj The object only in this step.
 * @param [in] iStepParm The parameter only in this step.
 *
 * @retval ZOK parse successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see SaxX_ParseDataX
 */
ZFUNC ZINT SaxX_ParseData(ST_ZOS_USTR *pstData, PFN_SAXXELEMSTART pfnStart,
                ZVOID *pStepObj, ZUINT iStepParm);

/**
 * @brief SAXX parse an xml data.
 *
 * @param [in] pstData An xml data.
 * @param [in] pfnStart Callback which invoked when parse an element start.
 * @param [in] pStepObj The object only in this step.
 * @param [in] pUserObj The user object.
 * @param [in] iStepParm The parameter only in this step.
 * @param [in] iUserParm The user parameter.
 *
 * @retval ZOK parse successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see SaxX_ParseData
 */
ZFUNC ZINT SaxX_ParseDataX(ST_ZOS_USTR *pstData, PFN_SAXXELEMSTART pfnStart,
                ZVOID *pStepObj, ZVOID *pUserObj, ZUINT iStepParm,
                ZUINT iUserParm);

/**
 * @brief SAXX parse an xml file.
 *
 * @param [in] pcFileName XML file name.
 * @param [in] pfnStart Callback which invoked when parse an element start.
 * @param [in] pStepObj The object only in this step.
 * @param [in] iStepParm The parameter only in this step.
 *
 * @retval ZOK parse successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see SaxX_ParseFileX
 */
ZFUNC ZINT SaxX_ParseFile(ZCHAR *pcFileName, PFN_SAXXELEMSTART pfnStart,
                ZVOID *pStepObj, ZUINT iStepParm);

/**
 * @brief SAXX parse an xml file.
 *
 * @param [in] pcFileName XML file name.
 * @param [in] pfnStart Callback which invoked when parse an element start.
 * @param [in] pStepObj The object only in this step.
 * @param [in] pUserObj The user object.
 * @param [in] iStepParm The parameter only in this step.
 * @param [in] iUserParm The user parameter.
 *
 * @retval ZOK parse successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see SaxX_ParseDataX
 */
ZFUNC ZINT SaxX_ParseFileX(ZCHAR *pcFileName, PFN_SAXXELEMSTART pfnStart,
                ZVOID *pStepObj, ZVOID *pUserObj, ZUINT iStepParm,
                ZUINT iUserParm);

#ifdef __cplusplus
}
#endif

#endif /* _SAX_COREX_H__ */

