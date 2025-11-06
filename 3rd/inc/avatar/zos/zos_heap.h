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
  File name     : zos_heap.h
  Module        : zos heap library functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-02    
  Description   :
      Data structure and function definitions required by the memory 
    management interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_HEAP_H__
#define _ZOS_HEAP_H__

/** 
 * @file zos_heap.h
 * @brief Zos system memory functions.
 *
 * In this file, provide operating system memory allocate and free interfaces.
 * They are @b malloc and @b free by default. User can get and set 
 * these functions using interfaces defined in zos_syscfg.h. 
 *
 * @see Zos_CfgSetHeapMalloc, Zos_CfgGetHeapMalloc,
 *      Zos_CfgSetHeapFree, Zos_CfgGetHeapFree
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate memory from operating system.
 *
 * @param [in] zSize Request size.
 *
 * @return New memory allocated if successfully, or return ZNULL.
 *
 * @see Zos_CfgSetHeapMalloc, Zos_CfgGetHeapMalloc 
 */
ZFUNC ZVOID * Zos_HeapAlloc(ZSIZE_T zSize);

/**
 * @brief Reallocate memory from operating system.
 *
 * @param [in] pMem Memory block.
 * @param [in] zSize Request size.
 *
 * @return New memory allocated if successfully, or return ZNULL.
 *
 * @see Zos_CfgSetHeapMalloc, Zos_CfgGetHeapMalloc 
 */
ZFUNC ZVOID * Zos_HeapRealloc(ZVOID *pMem, ZSIZE_T zSize);

/**
 * @brief Free memory allocated from operating system.
 *
 * @param [in] pMem Memory address.
 *
 * @return [none].
 *
 * @see Zos_CfgSetHeapFree, Zos_CfgGetHeapFree 
 */
ZFUNC ZVOID Zos_HeapFree(ZVOID *pMem);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_HEAP_H__ */

