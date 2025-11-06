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
  File name     : zos_omap.h
  Module        : zos object map library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2009-12-04    
  Description   :
      Data structure and function definitions required by the 
      object map interface

  Modify History:   
  1. Date:        Author:         Modification:
  2009/12/04      leo.lv          create the file 

*************************************************/
#ifndef _ZOS_OMAP_H__
#define _ZOS_OMAP_H__

/**
 * @file
 * @brief Zos object map.
 *
 * Object map (omap) provides a method to manager objects, map it to a unique
 * ID. User can assign with several map mechanism, including array, list and
 * hash.
 *
 * Omap interfaces are not thread safe.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Omap ID generate method type. */
typedef enum EN_ZOS_OMAP_ID_TYPE
{
    EN_ZOS_OMAP_ID_FIX_RANGE,        /**< @brief ID from range 1 to (max-1).
                                                 Use the first freed. */
    EN_ZOS_OMAP_ID_INC_CYCLE,        /**< @brief ID from range 1 to (max-1).
                                                 ID increase by 1 each time. */ 
    EN_ZOS_OMAP_ID_UNI_TIME,         /**< @brief ID form by time value. */
    EN_ZOS_OMAP_ID_RANDOM,           /**< @brief ID form by random value. */
    EN_ZOS_OMAP_ID_USR_SPEC,         /**< @brief User defined. */
    EN_ZOS_OMAP_ID_INVALID           /**< @brief Invalid type. */
} EN_ZOS_OMAP_ID_TYPE;

/** @brief Omap storage method type. */
typedef enum EN_ZOS_OMAP_MAP_TYPE
{
    EN_ZOS_OMAP_MAP_ARRAY,           /**< @brief Stored in a array. */
    EN_ZOS_OMAP_MAP_SLIST,           /**< @brief Stored in a singly-linked list. */ 
    EN_ZOS_OMAP_MAP_DLIST,           /**< @brief Stored in a doubly-linked list. */
    EN_ZOS_OMAP_MAP_HASH,            /**< @brief Stored in a hash table. */
    EN_ZOS_OMAP_MAP_DHASH,           /**< @brief Stored in a dynamic hash table. */
    EN_ZOS_OMAP_MAP_RBT,             /**< @brief Stored in a red-black tree. */
    EN_ZOS_OMAP_MAP_INVALID          /**< @brief Invalid type. */
} EN_ZOS_OMAP_MAP_TYPE;

/**
 * @brief Enumerate each item in object map.
 */
#define FOR_ALL_ITEM_IN_OMAP(_mapid, _item) \
    for (_item = Zos_OmapHead(_mapid); _item; \
         _item = Zos_OmapNext(_mapid, _item))

/**
 * @brief Enumerate each item in object map backward.
 */
#define FOR_ALL_ITEM_IN_OMAPX(_mapid, _item) \
    for (_item = Zos_OmapTail(_mapid); _item; \
         _item = Zos_OmapPrev(_mapid, _item))

/**
 * @brief Enumerate each object in object map.
 */
#define FOR_ALL_OBJ_IN_OMAP(_mapid, _item, _obj) \
    for (_item = Zos_OmapHead(_mapid), _obj = Zos_OmapItemObj(_mapid, _item); \
         _item; _item = Zos_OmapNext(_mapid, _item), \
         _obj = Zos_OmapItemObj(_mapid, _item))

/**
 * @brief Enumerate each object in object map, from tail to head.
 */
#define FOR_ALL_OBJ_IN_OMAPX(_mapid, _item, _obj) \
    for (_item = Zos_OmapTail(_mapid), _obj = Zos_OmapItemObj(_mapid, _item); \
         _item; _item = Zos_OmapPrev(_mapid, _item), \
         _obj = Zos_OmapItemObj(_mapid, _item))

/**
 * @brief Create a new omap.
 *
 * @param [in] ucIdType Omap ID generate method type, @ref EN_ZOS_OMAP_ID_TYPE.
 * @param [in] ucMapType Omap storage method type, @ref EN_ZOS_OMAP_MAP_TYPE.
 * @param [in] iMaxIdSize The ID maximum size.
 * @param [in] iIdStart The first ID value.
 * @param [in] iObjSize Object max count, mandatory for arrary and hash,
                        set 0 for dynamic hash.
 *
 * @return New omap ID when successfully, or return ZNULL.
 *
 * @see Zos_OmapCreateX
 */
ZFUNC ZOMAP Zos_OmapCreate(ZUCHAR ucIdType, ZUCHAR ucMapType, 
                ZUINT iMaxIdSize, ZUINT iIdStart, ZUINT iObjSize);

/**
 * @brief Create a new omap.
 *
 * @param [in] zMemBuf Memory buffer for store omap.
 * @param [in] ucIdType Omap ID generate method type.
 * @param [in] ucMapType Omap storage method type.
 * @param [in] iMaxIdSize The ID maximum size.
 * @param [in] iIdStart The ID range size, ID from iIdStart to (iIdSize-1).
 * @param [in] iObjSize Object max count, mandatory for arrary and hash,
                        set 0 for dynamic hash.
 *
 * @return New omap ID when successfully, or return ZNULL.
 *
 * @see Zos_OmapCreate
 */
ZFUNC ZOMAP Zos_OmapCreateX(ZCBUF zMemBuf, ZUCHAR ucIdType, 
                ZUCHAR ucMapType, ZUINT iMaxIdSize, ZUINT iIdStart, 
                ZUINT iObjSize);

/**
 * @brief Create a new omap.
 *
 * @param [in] zMemBuf Memory buffer for store omap.
 * @param [in] iBktSize The Default data block size.
 * @param [in] ucIdType Omap ID generate method type.
 * @param [in] ucMapType Omap storage method type.
 * @param [in] iMaxIdSize The ID maximum size.
 * @param [in] iIdStart The ID range size, ID from iIdStart to (iIdSize-1).
 * @param [in] iObjSize Object max count, mandatory for arrary and hash,
                        set 0 for dynamic hash.
 *
 * @return New omap ID when successfully, or return ZNULL.
 *
 * @see Zos_OmapCreate
 */
ZFUNC ZOMAP Zos_OmapCreateXD(ZCBUF zMemBuf, ZUINT iBktSize, 
                ZUCHAR ucIdType, ZUCHAR ucMapType, ZUINT iMaxIdSize, 
                ZUINT iIdStart, ZUINT iObjSize);

/**
 * @brief Delete omap.
 *
 * @param [in] zOmap Omap ID.
 *
 * @return [none].
 *
 * @see Zos_OmapCreate
 */
ZFUNC ZVOID Zos_OmapDelete(ZOMAP zOmap);

/**
 * @brief Get a new ID from omap.
 *
 * @param [in] zOmap Omap ID.
 * @param [out] piId New ID.
 *
 * @retval ZOK Get a new ID successfully.
 * @retval ZFAILED Get a new ID failed.
 *
 * @see Zos_OmapPutId
 */
ZFUNC ZINT Zos_OmapGetId(ZOMAP zOmap, ZUINT *piId);

/**
 * @brief Put a ID back to omap.
 *
 * @param [in] zOmap Omap ID.
 * @param [out] iId ID allocated.
 *
 * @retval ZOK Put ID successfully.
 * @retval ZFAILED Put ID failed.
 *
 * @see Zos_OmapGetId
 */
ZFUNC ZINT Zos_OmapPutId(ZOMAP zOmap, ZUINT iId);

/**
 * @brief Add new object to omap.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] pObj Object address add to omap, which won't retain data.
 * @param [out] piId ID allocated.
 *
 * @retval ZOK Add object successfully.
 * @retval ZFAILED Add object failed.
 *
 * @see Zos_OmapGetId
 */
ZFUNC ZINT Zos_OmapAddObj(ZOMAP zOmap, ZVOID *pObj, ZUINT *piId);

/**
 * @brief Add new object to omap with user defined ID.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iId User defined ID.
 * @param [in] pObj Object address add to omap, which won't retain data.
 *
 * @retval ZOK Add object successfully.
 * @retval ZFAILED Add object failed.
 *
 * @note This interface is only for EN_ZOS_OMAP_ID_USR_SPEC type omap.
 *
 * @see Zos_OmapGetId
 */
ZFUNC ZINT Zos_OmapAddObjX(ZOMAP zOmap, ZUINT iId, ZVOID *pObj);

/**
 * @brief Remove a object from omap.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iId ID to remove.
 * @param [in] pObj Object to remove.
 *
 * @retval ZOK Remove object successfully.
 * @retval ZFAILED Remove object failed.
 *
 * @see Zos_OmapAddObj
 */
ZFUNC ZINT Zos_OmapRmvObj(ZOMAP zOmap, ZUINT iId, ZVOID *pObj);

/**
 * @brief Map a object to a exist ID.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iId ID got by Zos_OmapGetId.
 * @param [in] pObj Object to add.
 *
 * @retval ZOK Map object successfully.
 * @retval ZFAILED Map object failed.
 *
 * @see Zos_OmapAddObj
 */
ZFUNC ZINT Zos_OmapMapObj(ZOMAP zOmap, ZUINT iId, ZVOID *pObj);

/**
 * @brief Un-map a object to ID.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iId Object's ID.
 * @param [in] pObj Object address.
 *
 * @retval ZOK Un-map object successfully.
 * @retval ZFAILED Un-map object failed.
 *
 * @see Zos_OmapMapObj
 */
ZFUNC ZINT Zos_OmapUnmapObj(ZOMAP zOmap, ZUINT iId, ZVOID *pObj);

/**
 * @brief Search object address by ID.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] dwId Object's ID.
 *
 * @return Object address if found, or return ZNULL.
 *
 * @see
 */
ZFUNC ZVOID * Zos_OmapFindObj(ZOMAP zOmap, ZUINT iId);

/**
 * @brief Check if the object's ID is equal to specific ID.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iId Object's ID.
 * @param [in] pObj Object address.
 *
 * @retval ZTRUE Object's ID is equal to specific ID.
 * @retval ZFALSE Otherwise.
 *
 * @see
 */
ZFUNC ZBOOL Zos_OmapExistObj(ZOMAP zOmap, ZUINT iId, ZVOID *pObj);

/**
 * @brief Get object count.
 *
 * @param [in] zOmap Omap ID.
 *
 * @return The object count, only support EN_ZOS_OMAP_MAP_SLIST,
           EN_ZOS_OMAP_MAP_DLIST, EN_ZOS_OMAP_MAP_RBT.
 */
ZFUNC ZUINT Zos_OmapGetCnt(ZOMAP zOmap);

/**
 * @brief Enumerate object by index.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iIndex Object index, from 0 to size - 1;
 * @param [out] pdwId Object ID.
 * @param [out] ppObj Object pointer.
 *
 * @retval ZOK The object corresponding to index is found. Only support 
 *             EN_ZOS_OMAP_MAP_SLIST, EN_ZOS_OMAP_MAP_DLIST, 
 *             EN_ZOS_OMAP_MAP_RBT. When the type is EN_ZOS_OMAP_MAP_RBT,
 *             the ID is ordered by its value.
 * @retval ZFAILED The object is not found.
 */
ZFUNC ZINT Zos_OmapEnum(ZOMAP zOmap, ZUINT iIndex,
                ZUINT *piId, ZVOID **ppObj);

/**
 * @brief Enumerate object by index. Only support 
 *             EN_ZOS_OMAP_MAP_SLIST, EN_ZOS_OMAP_MAP_DLIST, 
 *             EN_ZOS_OMAP_MAP_RBT. When the type is EN_ZOS_OMAP_MAP_RBT,
 *             the ID is ordered by its value.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iIndex Object index, from 0 to size - 1;
 * @param [out] pdwId Object ID.
 *
 * @return The object ID.
 */
ZFUNC ZUINT Zos_OmapEnumId(ZOMAP zOmap, ZUINT iIndex);

/**
 * @brief Enumerate object by index. Only support 
 *             EN_ZOS_OMAP_MAP_SLIST, EN_ZOS_OMAP_MAP_DLIST, 
 *             EN_ZOS_OMAP_MAP_RBT. When the type is EN_ZOS_OMAP_MAP_RBT,
 *             the ID is ordered by its value
 *
 * @param [in] zOmap Omap ID.
 * @param [in] iIndex Object index, from 0 to size - 1;
 *
 * @return The object.
 */
ZFUNC ZVOID * Zos_OmapEnumObj(ZOMAP zOmap, ZUINT iIndex);

/**
 * @brief Get head item in object map, only support EN_ZOS_OMAP_MAP_RBT.
 *
 * @param [in] zOmap Omap ID.
 *
 * @return Head item.
 */
ZFUNC ZOITEM Zos_OmapHead(ZOMAP zOmap);

/**
 * @brief Get tail item in object map, only support EN_ZOS_OMAP_MAP_RBT.
 *
 * @param [in] zOmap Omap ID.
 *
 * @return Tail item.
 */
ZFUNC ZOITEM Zos_OmapTail(ZOMAP zOmap);

/**
 * @brief Get next item in object map, only support EN_ZOS_OMAP_MAP_RBT.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] zItem Current item.
 *
 * @return Next item.
 */
ZFUNC ZOITEM Zos_OmapNext(ZOMAP zOmap, ZOITEM zItem);

/**
 * @brief Get previous item in object map, only support EN_ZOS_OMAP_MAP_RBT.
 *
 * @param [in] zOmap Omap ID.
 * @param [in] zItem Current item.
 *
 * @return Previous item.
 */
ZFUNC ZOITEM Zos_OmapPrev(ZOMAP zOmap, ZOITEM zItem);

/**
 * @brief Get item ID
 *
 * @param [in] zOmap Omap ID.
 * @param [in] zItem The item.
 *
 */
ZFUNC ZUINT Zos_OmapItemId(ZOMAP zOmap, ZOITEM zItem);

/**
 * @brief Get item object
 *
 * @param [in] zOmap Omap object.
 * @param [in] zItem The item.
 *
 */
ZFUNC ZVOID * Zos_OmapItemObj(ZOMAP zOmap, ZOITEM zItem);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_OMAP_H__ */

