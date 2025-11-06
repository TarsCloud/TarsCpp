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
  File name     : zos_ovec.h
  Module        : zos object vector
  Author        : leo.lv   
  Version       : 0.1   
  Created on    : 2012-03-13    
  Description   :
      Data structure and function definitions required by the 
      object vector interface

  Modify History:   
  1. Date:        Author:         Modification:
  2012/03/13      leo.lv          create the file 

*************************************************/
#ifndef _ZOS_OVEC_H__
#define _ZOS_OVEC_H__

/**
 * @file
 * @brief Zos object vector.
 *
 * Object vector provides a method to manager objects in a vector.
 *
 * Ovec interfaces are not thread safe.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new object vector.
 *
 * @param [in] iSize The initialize object container size.
 *
 * @return New object vector ID when successfully, or return ZNULL.
 *
 * @see Zos_OvecDelete
 */
ZFUNC ZOVEC Zos_OvecCreate(ZUINT iSize);

/**
 * @brief Create a new object vector.
 *
 * @param [in] zOvec Parent vector attach to.
 * @param [in] iSize The initialize object container size.
 *
 * @return New object vector ID when successfully, or return ZNULL.
 *
 * @see Zos_OvecDelete
 */
ZFUNC ZOVEC Zos_OvecCreateX(ZOVEC zOvec, ZUINT iSize);

/**
 * @brief Create a new object vector.
 *
 * @param [in] iBktSize The Default data block size.
 * @param [in] iSize The initialize object container size.
 *
 * @return New object vector ID when successfully, or return ZNULL.
 *
 * @see Zos_OvecDelete
 */
ZFUNC ZOVEC Zos_OvecCreateXD(ZUINT iBktSize, ZUINT iSize);

/**
 * @brief Delete object vector.
 *
 * @param [in] zOvec Vector ID.
 *
 * @return [none].
 *
 * @see Zos_OvecCreate
 */
ZFUNC ZVOID Zos_OvecDelete(ZOVEC zOvec);

/**
 * @brief Clean object vector.
 *
 * @param [in] zOvec Vector ID.
 *
 * @return [none].
 *
 * @see Zos_OvecDelete
 */
ZFUNC ZVOID Zos_OvecClean(ZOVEC zOvec);

/**
 * @brief Attach vector to another vector as its child.
 *
 * @param [in] zDstOvec Parent vector ID.
 * @param [in] zSrcOvec Child vector ID.
 *
 * @retval ZOK Attach successfully.
 * @retval ZFAILED Attach failed.
 *
 * @see Zos_OvecAttach
 */
ZFUNC ZINT Zos_OvecAttach(ZOVEC zDstOvec, ZOVEC zSrcOvec);

/**
 * @brief Detach vector from its parent vector.
 *
 * @param [in] zOvec Ovec ID.
 *
 * @retval ZOK Detach successfully.
 * @retval ZFAILED Detach failed.
 *
 * @see Zos_OvecAttach
 */
ZFUNC ZINT Zos_OvecDetach(ZOVEC zOvec);

/**
 * @brief Get object size of object vector.
 *
 * @param [in] zOvec Vector ID.
 *
 * @return Object size when successfully, or return 0.
 *
 * @see Zos_OvecIsEmpty
 */
ZFUNC ZSIZE_T Zos_OvecSize(ZOVEC zOvec);

/**
 * @brief Check object vector is empty.
 *
 * @param [in] zOvec Vector ID.
 *
 * @return ZTRUE when object vector is empty, or return ZFALSE.
 *
 * @see Zos_OvecSize
 */
ZFUNC ZBOOL Zos_OvecEmpty(ZOVEC zOvec);

/**
 * @brief Get the first object of object vector.
 *
 * @param [in] zOvec Vector ID.
 *
 * @return The first object when successfully, or return ZNULL.
 *
 * @see Zos_OvecBack
 */
ZFUNC ZVOID * Zos_OvecFront(ZOVEC zOvec);

/**
 * @brief Get the last object of object vector.
 *
 * @param [in] zOvec Vector ID.
 *
 * @return The last object when successfully, or return ZNULL.
 *
 * @see Zos_OvecFront
 */
ZFUNC ZVOID * Zos_OvecBack(ZOVEC zOvec);

/**
 * @brief Get an object according to position from object vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] zPos Object position.
 *
 * @return The object when successfully, or return ZNULL.
 *
 * @see Zos_OvecAtX
 */
ZFUNC ZVOID * Zos_OvecAt(ZOVEC zOvec, ZSIZE_T zPos);

/**
 * @brief Get an object according to ID from object vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iId Object ID.
 *
 * @return The object when successfully, or return ZNULL.
 *
 * @see Zos_OvecAt
 */
ZFUNC ZVOID * Zos_OvecAtX(ZOVEC zOvec, ZUINT iId);

/**
 * @brief Get the object id from position.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] zPos Object position.
 *
 * @return The object id when successfully, or return 0.
 *
 * @see Zos_OvecId2Pos
 */
ZFUNC ZUINT Zos_OvecPos2Id(ZOVEC zOvec, ZSIZE_T zPos);

/**
 * @brief Get the object position from id.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iId Object ID.
 *
 * @return The object position when successfully, or return -1.
 *
 * @see Zos_OvecPos2Id
 */
ZFUNC ZSIZE_T Zos_OvecId2Pos(ZOVEC zOvec, ZUINT iId);

/**
 * @brief Get the object memory buffer from id.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iId Object ID.
 *
 * @return The object memory buffer when successfully, or return ZMAXUINT.
 *
 * @see Zos_OvecPos2Id
 */
ZFUNC ZCBUF Zos_OvecId2Cbuf(ZOVEC zOvec, ZUINT iId);

/**
 * @brief Insert an object before a position.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iPos Object position.
 * @param [in] pObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecInsertX
 */
ZFUNC ZINT Zos_OvecInsert(ZOVEC zOvec, ZSIZE_T zPos, ZVOID *pObj);

/**
 * @brief Insert an object with an ID before a position.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iPos Object position.
 * @param [in] pObj Object address.
 * @param [out] piId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecInsert, Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecInsertX(ZOVEC zOvec, ZSIZE_T zPos, ZVOID *pObj, 
                ZUINT *piId);

/**
 * @brief Insert an new object with an ID before a position.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iPos Object position.
 * @param [in] zSize Object size.
 * @param [out] pzMemBuf Memory buffer to object.
 * @param [out] ppObj Object address.
 * @param [out] piId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecInsert, Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecInsertXD(ZOVEC zOvec, ZSIZE_T zPos, ZSIZE_T zSize,
                ZCBUF *pzMemBuf, ZVOID **ppObj, ZUINT *piId);


/**
 * @brief Insert an object with an ID before a position.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iPos Object position.
 * @param [in] pObj Object address.
 * @param [in] iId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecInsert, Zos_OvecAppMapX
 */
ZFUNC ZINT Zos_OvecInsertD(ZOVEC zOvec, ZSIZE_T zPos, ZVOID *pObj, 
                ZUINT iId);

/**
 * @brief Insert an new object with an ID before a position.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] zPos Object position.
 * @param [in] zSize Object size.
 * @param [in] iId Object ID.
 * @param [out] pzMemBuf Memory buffer to object.
 * @param [out] ppObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecInsert, Zos_OvecAppMapX
 */
ZFUNC ZINT Zos_OvecInsertDX(ZOVEC zOvec, ZSIZE_T zPos, ZSIZE_T zSize, 
                 ZUINT iId, ZCBUF *pzMemBuf, ZVOID **ppObj);

/**
 * @brief Insert an object at the first.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushBack
 */
ZFUNC ZINT Zos_OvecPushFront(ZOVEC zOvec, ZVOID *pObj);

/**
 * @brief Insert an object with an ID at the first.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [out] ZUINT Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushBackX, Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecPushFrontX(ZOVEC zOvec, ZVOID *pObj, 
                ZUINT *piId);

/**
 * @brief Insert an new object with an ID at the first.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [out] pzMemBuf Memory buffer to object.
 * @param [out] ppObj Object address.
 * @param [out] piId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushBackXD, Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecPushFrontXD(ZOVEC zOvec, ZSIZE_T zSize,
                ZCBUF *pzMemBuf, ZVOID **ppObj, ZUINT *piId);

/**
 * @brief Insert an object with an ID at the first.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [in] iId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushBackD, Zos_OvecAppMapX
 */
ZFUNC ZINT Zos_OvecPushFrontD(ZOVEC zOvec, ZVOID *pObj, 
                ZUINT iId);

/**
 * @brief Insert an new object with an ID at the first.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [in] iId Object ID.
 * @param [out] pzMemBuf Memory buffer to object.
 * @param [out] ppObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushBackDX, Zos_OvecAppMapX
 */
ZFUNC ZINT Zos_OvecPushFrontDX(ZOVEC zOvec, ZSIZE_T zSize,
                ZUINT iId, ZCBUF *pzMemBuf, ZVOID **ppObj);

/**
 * @brief Insert an object at the end.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushFront
 */
ZFUNC ZINT Zos_OvecPushBack(ZOVEC zOvec, ZVOID *pObj);

/**
 * @brief Insert an object with an ID at the end.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [out] piId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushFrontX, Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecPushBackX(ZOVEC zOvec, ZVOID *pObj, 
                ZUINT *piId);

/**
 * @brief Insert an new object with an ID at the end.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [out] pzMemBuf Memory buffer to object.
 * @param [out] ppObj Object address.
 * @param [out] piId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushFrontXD, Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecPushBackXD(ZOVEC zOvec, ZSIZE_T zSize,
                ZCBUF *pzMemBuf, ZVOID **ppObj, ZUINT *piId);

/**
 * @brief Insert an object with an ID at the end.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [in] iId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushFrontD, Zos_OvecAppMapX
 */
ZFUNC ZINT Zos_OvecPushBackD(ZOVEC zOvec, ZVOID *pObj, 
                    ZUINT iId);

/**
 * @brief Insert an new object with an ID at the end.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] pObj Object address.
 * @param [in] iId Object ID.
 * @param [out] pzMemBuf Memory buffer to object.
 * @param [out] ppObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPushFrontDX, Zos_OvecAppMapX
 */
ZFUNC ZINT Zos_OvecPushBackDX(ZOVEC zOvec, ZSIZE_T zSize,
                ZUINT iId, ZCBUF *pzMemBuf, ZVOID **ppObj);

/**
 * @brief Remove the first object from vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [out] ppObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPopBack
 */
ZFUNC ZINT Zos_OvecPopFront(ZOVEC zOvec, ZVOID **ppObj);

/**
 * @brief Remove the first object with ID from vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [out] ppObj Object address.
 * @param [out] piId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPopBackX
 */
ZFUNC ZINT Zos_OvecPopFrontX(ZOVEC zOvec, ZVOID **ppObj, 
                ZUINT *piId);

/**
 * @brief Remove the last object from vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [out] ppObj Object address.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPopFront
 */
ZFUNC ZINT Zos_OvecPopBack(ZOVEC zOvec, ZVOID **ppObj);

/**
 * @brief Remove the last object with ID from vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [out] ppObj Object address.
 * @param [out] piId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecPopFrontX
 */
ZFUNC ZINT Zos_OvecPopBackX(ZOVEC zOvec, ZVOID **ppObj, 
                ZUINT *piId);

/**
 * @brief Swap the objects between two positions vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] zPos1 Object position 1.
 * @param [in] zPos2 Object position 2.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecSwapX
 */
ZFUNC ZINT Zos_OvecSwap(ZOVEC zOvec, ZSIZE_T zPos1, ZSIZE_T zPos2);

/**
 * @brief Swap the objects between two positions vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iId1 Object ID 1.
 * @param [in] iId2 Object ID 2.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecSwap
 */
ZFUNC ZINT Zos_OvecSwapX(ZOVEC zOvec, ZUINT iId1, ZUINT iId2);

/**
 * @brief Move the object to the last postion.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] zPos Object position.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecObj2FirstX
 */
ZFUNC ZINT Zos_OvecObj2Last(ZOVEC zOvec, ZSIZE_T zPos);

/**
 * @brief Move the object to the last postion.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecObj2First
 */
ZFUNC ZINT Zos_OvecObj2LastX(ZOVEC zOvec, ZUINT iId);

/**
 * @brief Erase the object at a position from vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] zPos Object position.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecEraseX
 */
ZFUNC ZINT Zos_OvecErase(ZOVEC zOvec, ZSIZE_T zPos);

/**
 * @brief Erase the object with ID from vector.
 *
 * @param [in] zOvec Vector ID.
 * @param [in] iId Object ID.
 *
 * @return ZOK when successfully, or return ZFAILED.
 *
 * @see Zos_OvecErase
 */
ZFUNC ZINT Zos_OvecEraseX(ZOVEC zOvec, ZUINT iId);

/**
 * @brief Apply map valid in vector.
 *
 * @param [in] zOvec Object vector ID.
 * @param [in] iIdSize The ID size.
 * @param [in] iIdStart The ID start.
 *
 * ID range is from iIdStart to iIdStart + iIdSize
 *
 * @retval ZOK Attach successfully.
 * @retval ZFAILED Attach failed.
 *
 * @see Zos_OvecAppMapX
 */
ZFUNC ZINT Zos_OvecAppMap(ZOVEC zOvec, ZUINT iIdSize, 
                ZUINT iIdStart);

/**
 * @brief Apply map valid in vector.
 *
 * @param [in] zOvec Object vector ID.
 * @param [in] iIdSize The ID size.
 * @param [in] iIdStart The ID start.
 *
 * ID user specified is from iIdStart to iIdStart + iIdSize
 *
 * @retval ZOK Attach successfully.
 * @retval ZFAILED Attach failed.
 *
 * @see Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecAppMapX(ZOVEC zOvec, ZUINT iIdSize, 
                ZUINT iIdStart);

/**
 * @brief Apply map valid in vector.
 *
 * @param [in] zOvec Object vector ID.
 * @param [in] iBktSize The Default data block size.
 * @param [in] iIdSize The ID size.
 * @param [in] iIdStart The ID start.
 *
 * ID user specified is from iIdStart to iIdStart + iIdSize
 *
 * @retval ZOK Attach successfully.
 * @retval ZFAILED Attach failed.
 *
 * @see Zos_OvecAppMap
 */
ZFUNC ZINT Zos_OvecAppMapXD(ZOVEC zOvec, ZUINT iBktSize, 
                ZUINT iIdSize, ZUINT iIdStart);

/**
 * @brief Vector remove an object.
 *
 * @param [in] zOvec Object vector ID.
 * @param [in] pObj The remove object.
 *
 * @retval ZOK remove successfully, othewise return ZFAILED.
 *
 * @see Zos_OvecAddObj
 */
ZFUNC ZINT Zos_OvecRmvObj(ZOVEC zOvec, ZVOID *pObj);

/**
 * @brief Remove an object by object id.
 *
 * @param [in] zOvec Object vector ID.
 * @param [in] iId Object ID.
 *
 * @retval ZOK Detach successfully.
 * @retval ZFAILED Detach failed.
 *
 */
 ZFUNC ZINT Zos_OvecRmvObjById(ZOVEC zOvec, ZUINT iId);

/**
 * @brief Remove an object by object position.
 *
 * @param [in] zOvec Object vector ID.
 * @param [in] iPos Object position.
 *
 * @retval ZOK Detach successfully.
 * @retval ZFAILED Detach failed.
 *
 */
ZFUNC ZINT Zos_OvecRmvObjByPos(ZOVEC zOvec, ZSIZE_T zPos);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_OVEC_H__ */

