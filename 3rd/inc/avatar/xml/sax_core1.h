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
  File name     : sax_core1.h
  Module        : sax1 core interfaces
  Author        : Zocol 
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Data structure and function definitions required by 
      the sax1 core interfaces
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _SAX_CORE1_H__
#define _SAX_CORE1_H__

/** 
 * @file sax_core1.h
 * @brief XML SAX interfaces.
 *
 * @see 
 */

#ifdef __cplusplus
extern "C"{
#endif

/********************* DTD action typedefs *************************/
/** @brief typedef action of xml a notation declaration event */
typedef ZVOID (*PFN_SAX1NOTAIONDECL)(ST_XML_NAME *pstName, 
                ST_ZOS_USTR *pstPubId, ST_ZOS_USTR *pstSysId);

/** @brief typedef action of xml an unparsed entity declaration event */
typedef ZVOID (*PFN_SAX1UPENTDECL)(ST_XML_NAME *pstName, 
               ST_ZOS_USTR *pstPubId, ST_ZOS_USTR *pstSysId);

/********************* Document action typedefs *************************/
/** @brief Typedef of callback which invoked when parse an document start. */
typedef ZVOID (*PFN_SAX1DOCSTART)(ZFUNC_VOID);

/** @brief Typedef of callback which invoked when parse an document end. */
typedef ZVOID (*PFN_SAX1DOCEND)(ZFUNC_VOID);

/** @brief Typedef of callback which invoked when parse an element start. */
typedef ZVOID (*PFN_SAX1ELEMSTART)(ST_XML_NAME *pstName, 
                ST_XML_ATTR_LST *pstAttrLst);

/** @brief Typedef of callback which invoked when parse an element end. */
typedef ZVOID (*PFN_SAX1ELEMEND)(ST_XML_NAME *pstName);

/** @brief Typedef of callback when  parse a character data */
typedef ZVOID (*PFN_SAX1CHRDATA)(ST_ZOS_USTR *pstStr);

/** @brief Typedef of callback which invoked when ignorable whitespace */
typedef ZVOID (*PFN_SAX1IGNWS)(ST_ZOS_USTR *pstStr);

/** @brief Typedef of callback which invoked when processing instruction */
typedef ZVOID (*PFN_SAX1PI)(ST_XML_PI_TARGET *pstPiTarget, 
                ST_ZOS_USTR *pstData);

/************************* Error Action typedefs *************************/
/** @brief Typedef of callback which invoked when processing error. */
typedef ZVOID (*PFN_SAX1ERROR)(ZINT iErrNo);

/** @brief Typedef of callback which invoked when processing fatal */
typedef ZVOID (*PFN_SAX1FATAL)(ZINT iFatalNo);

/** @brief Typedef of callback which invoked when processing warning */
typedef ZVOID (*PFN_SAX1WARN)(ZINT iWarnNo);

/******************* resolving entities action typedefs ******************/

/**
*@brief Typedef of callback which invokeds
*        when processing resolving entities.
*/
typedef ST_ZOS_USTR * (*PFN_SAX1REVENT)(ST_ZOS_USTR *pstPubId, 
                ST_ZOS_USTR *pstSysId);

/************************* XML Action *************************/
/** @brief xml sax1 DTD action */
typedef struct tagSAX1_DTD_ACTION
{    
   PFN_SAX1NOTAIONDECL pfnNotationDecl; /**<@brief Notation Declaration */  
   PFN_SAX1UPENTDECL pfnUnparsedEntDecl; /**<@brief Unparsed Entity Declaration */
} ST_SAX1_DTD_ACTION;

/** @brief XML sax1 document action */
typedef struct tagSAX1_DOC_ACTION
{
    PFN_SAX1DOCSTART pfnDocStart;    /**<@brief Document Start */
    PFN_SAX1DOCEND pfnDocEnd;        /**<@brief Document End */
    PFN_SAX1ELEMSTART pfnElemStart;  /**<@brief Element Start */
    PFN_SAX1ELEMEND pfnElemEnd;      /**<@brief Element End */
    PFN_SAX1CHRDATA pfnChrData;      /**<@brief Character Data */
    PFN_SAX1IGNWS pfnIgnWS;          /**<@brief Egnorable Whitespace */
    PFN_SAX1PI pfnPi;                /**<@brief Processing instruction */
} ST_SAX1_DOC_ACTION;

/** @brief XML sax1 error action */
typedef struct tagSAX1_ERR_ACTION
{
    PFN_SAX1ERROR pfnError;         /**<@brief Error */
    PFN_SAX1FATAL pfnFatal;         /**<@brief Fatal */
    PFN_SAX1WARN pfnWarn;           /**<@brief Warning */
} ST_SAX1_ERR_ACTION;

/** @brief XML sax1 resolving entities action */
typedef struct tagSAX1_REV_ACTION
{
    PFN_SAX1REVENT pfnRevEnt;        /**<@brief Resolving entities */
} ST_SAX1_REV_ACTION;

/** @brief XML sax1 action */
typedef struct tagSAX1_ACTION
{
    ST_SAX1_DTD_ACTION stDtd;        /**<@brief DTD action */
    ST_SAX1_DOC_ACTION stDoc;        /**<@brief Document action */
    ST_SAX1_ERR_ACTION stErr;        /**<@brief Error action */
    ST_SAX1_REV_ACTION stRev;        /**<@brief Resolving entities action */
} ST_SAX1_ACTION;

/**
 * @brief Allow an application to register a DTD event action.
 *
 * @param [in] pstAct Sax1 action structure. 
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Sax1_SetDftAction(ST_SAX1_ACTION *pstAct);

/**
 * @brief Allow an application to register a DTD event action.
 *
 * @param [in] pstAct Sax1 action structure.
 * @param [in] pstDtd Sax1 DTD action structure.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Sax1_SetDtdAction(ST_SAX1_ACTION *pstAct, 
                ST_SAX1_DTD_ACTION *pstDtd);

/**
 * @brief Allow an application to register a document event action.
 *
 * @param [in] pstAct Sax1 action structure.
 * @param [in] pstDoc Sax1 document action structure.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Sax1_SetDocAction(ST_SAX1_ACTION *pstAct, 
                ST_SAX1_DOC_ACTION *pstDoc);
 
/**
 * @brief Allow an application to register a custom entity action.
 *
 * @param [in] pstAct Sax1 action structure.
 * @param [in] pstRev Sax1 resolving entities action structure.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Sax1_SetResolveAction(ST_SAX1_ACTION *pstAct, 
                ST_SAX1_REV_ACTION *pstRev);

/**
 * @brief Allow an application to register an error event action.
 *
 * @param [in] pstAct Sax1 action structure.
 * @param [in] pstErr Sax1 error action structure.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Sax1_SetErrAction(ST_SAX1_ACTION *pstAct, 
                ST_SAX1_ERR_ACTION *pstErr);

/**
 * @brief Allow an application to register an element event action.
 *
 * @param [in] pstAct Sax1 action structure.
 * @param [in] pfnElemStart Callback which invoked when parse an element start.
 * @param [in] pfnElemEnd Callback which invoked when parse an element end.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Sax1_SetElemAction(ST_SAX1_ACTION *pstAct, 
                PFN_SAX1ELEMSTART pfnElemStart, PFN_SAX1ELEMEND pfnElemEnd);

/**
 * @brief Allow an application to register an element event action.
 *
 * @param [in] pstAct Sax1 action structure.
 * @param [in] pfnChrData Callback when parse a character data.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see 
 */
ZFUNC ZINT Sax1_SetChrDataAction(ST_SAX1_ACTION *pstAct, 
                PFN_SAX1CHRDATA pfnChrData);

/************************* Sax Paser ****************************/
/**
 * @brief Parse an xml data.
 *
 * @param [in] pstData The data string.
 * @param [in] pstAct Sax1 action structure.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Sax1_SetDftAction
 */
ZFUNC ZINT Sax1_ParseData(ST_ZOS_USTR *pstData, ST_SAX1_ACTION *pstAct);

/**
 * @brief Parse an xml file.
 *
 * @param [in] pcFileName The file name.
 * @param [in] pstAct Sax1 action structure.
 *
 * @retval ZOK set the action successful.
 * @retval ZFAILED Otherwise.
 *          
 * @see Sax1_ParseData
 */
ZFUNC ZINT Sax1_ParseFile(ZCHAR *pcFileName, ST_SAX1_ACTION *pstAct);

#ifdef __cplusplus
}
#endif

#endif /* _SAX_CORE1_H__ */

