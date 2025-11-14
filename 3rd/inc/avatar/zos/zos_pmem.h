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
  File name     : zos_pmem.h
  Module        : zos power memory library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the 
      power memory management interface
      
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_PMEM_H__
#define _ZOS_PMEM_H__

/** 
 * @file zos_pmem.h
 * @brief Zos power memory functions.
 *
 * Interfaces in this file are similar to memory interfaces, except these
 * interfaces are based on zos inner "Power Pool".
 * This memory pool was created when Zos system is initialized, see Zos_SysInit.
 * and it will be deleted when Zos system is destroy, see Zos_SysDestroy.
 *
 * User can also access its configuration using interfaces defined 
 * in @ref zos_syscfg.h "System Configuration". Get setting use
 * -# Zos_CfgGetPMemNeedDynaFree
 * -# Zos_CfgGetPMemGrpSize
 * -# Zos_CfgGetPMemBktInfo
 * 
 * Set setting use
 * -# Zos_CfgSetPMemNeedDynaFree
 * -# Zos_CfgSetPMemBkt
 *
 * These interfaces are thread safe.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate memory.
 *
 * @param [in] iBlkSize Data size request.
 *
 * @return Memory address if successfully, or return ZNULL.
 *
 * @see Zos_PMemFree
 */
ZFUNC ZVOID * Zos_PMemAlloc(ZUINT iBlkSize);

/**
 * @brief Free memory allocated.
 *
 * @param [in] iBlkSize Data size request.
 * @param [in] pMem Memory address.
 *
 * @return [none].
 *
 * @see Zos_PMemAlloc
 */
ZFUNC ZVOID Zos_PMemFree(ZUINT iBlkSize, ZVOID *pMem);

/**
 * @brief Get block start address from any address inside the block.
 *
 * @param [in] iBlkSize Data size request.
 * @param [in] pData Memory address.
 *
 * @return Block start address if successfully, or return ZNULL.
 */
ZFUNC ZVOID * Zos_PMemD2M(ZUINT iBlkSize, ZVOID *pData);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_PMEM_H__ */

