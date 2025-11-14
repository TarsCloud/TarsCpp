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
  File name     : zos_brick.h
  Module        : zos brick memory management
  Author        : zocol
  Version       : 0.1
  Created on    : 2006-03-02
  Description   :
    Marcos and structure definitions required by zos brick memory.

  Modify History:
  1. Date:        Author:         Modification:
  2006-03-02      Bob.Liu         
*************************************************/
#ifndef _ZOS_BRICK_H__
#define _ZOS_BRICK_H__

/** 
 * @file
 * @brief Zos brick buffer functions.
 *
 * Brick is used for manage multi data block with same size. These interfaces
 * are not thread safe.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Brick manager type. */
typedef ZVOID * ZBKMGR;

/** @brief Brick data type. */
typedef ZVOID * ZBKDATA;

/** @brief Compare callback function type, see Zos_BkFindIf */
typedef ZINT (*PFN_ZBKMATCH)(ZBKDATA zBkData, ZVOID *pCond);

/** @brief Action callback function type, see Zos_BkForEach */
typedef ZVOID (*PFN_ZBKACTION)(ZBKDATA zBkData, ZVOID *pParm);

/** @brief Brick manager configuration structure. */
typedef struct tagZOS_BKM_CFG
{
    ZUINT iDataSize;               /**< @brief Data size of each brick. */
    ZUINT iMaxCnt;                 /**< @brief Max count limit of brick. */
    ZUINT iIncCnt;                 /**< @brief Request count of brick from
                                         system memory when run out of brick. */
} ST_ZOS_BKM_CFG;

/**
 * @brief Create brick manager.
 *
 * @param [in] pstCfg Brick manager configuration.
 *
 * @return Brick manager if successfully, or return ZNULL.
 *
 * @see Zos_BkDelete
 */
ZFUNC ZBKMGR Zos_BkCreate(ST_ZOS_BKM_CFG *pstCfg);

/**
 * @brief Delete brick manager.
 *
 * @param [in] zBkMgr Brick manager.
 *
 * @return [none].
 *
 * @see Zos_BkCreate
 */
ZFUNC ZVOID Zos_BkDelete(ZBKMGR zBkMgr);

/**
 * @brief Get and hold a brick data blcok from brick manager.
 *
 * @param [in] zBkMgr Brick manager.
 * @param [in] piBkId New brick ID.
 *
 * @return Brick data blcok if successfully, or return ZNULL.
 *
 * @see Zos_BkGetClrd
 */
ZFUNC ZBKDATA Zos_BkGet(ZBKMGR zBkMgr, ZUINT *piBkId);

/**
 * @brief Get and hold a brick data blcok clear by 0 from brick manager.
 *
 * @param [in] zBkMgr Brick manager.
 * @param [in] piBkId New brick ID.
 *
 * @return Brick data blcok if successfully, or return ZNULL.
 *
 * @see Zos_BkGet
 */
ZFUNC ZBKDATA Zos_BkGetClrd(ZBKMGR zBkMgr, ZUINT *piBkId);

/**
 * @brief Release a brick data blcok.
 *
 * @param [in] zBkMgr Brick manager.
 * @param [in] zBkData Brick data blcok.
 *
 * @retval ZOK Release successfully.
 * @retval ZFAILED Release failed.
 *
 * @see Zos_BkGet
 */
ZFUNC ZINT Zos_BkPut(ZBKMGR zBkMgr, ZBKDATA zBkData);

/**
 * @brief Search for brick data block by ID.
 *
 * @param [in] zBkMgr Brick manager.
 * @param [in] iBkId Brick ID.
 *
 * @return Brick data blcok if found in use, or return ZNULL.
 *
 * @see Zos_BkFindIf
 */
ZFUNC ZBKDATA Zos_BkFromId(ZBKMGR zBkMgr, ZUINT iBkId);

/**
 * @brief Query all used brick data block to search brick 
 *        which matchs user defined condition.
 *
 * @param [in] zBkMgr Brick manager.
 * @param [in] pfnMatch Compare function, this function will be called for 
 *                      every brick. If it returns ZOK, then return that 
 *                      brick data blcok.
 * @param [in] pCond User defined data.
 *
 * @return Brick data blcok if found in use, or return ZNULL.
 *
 * @see Zos_BkFromId
 */
ZFUNC ZBKDATA Zos_BkFindIf(ZBKMGR zBkMgr, PFN_ZBKMATCH pfnMatch, ZVOID *pCond);

/**
 * @brief Get brick count allocated.
 *
 * @param [in] zBkMgr Brick manager.
 *
 * @return Brick brick count allocated.
 *
 * @see Zos_BkEnum
 */
ZFUNC ZUINT Zos_BkGetCnt(ZBKMGR zBkMgr);

/**
 * @brief Enumrate brick by index, from 0 to count - 1.
 *
 * @param [in] zBkMgr Brick manager.
 * @param [in] iIndex Brick index from 0 to count - 1.
 *
 * @return Brick brick allocated.
 *
 * @see Zos_BkGetCnt
 */
ZFUNC ZBKDATA Zos_BkEnum(ZBKMGR zBkMgr, ZUINT iIndex);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_BRICK_H__ */

