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
  File name     : zrandom.h
  Module        : random utilitis
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the 
      random utilitis
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZRANDOM_H__
#define _ZRANDOM_H__

/** 
 * @file zrandom.h
 * @brief Zos random utility functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate a random ZUSHORT value.
 *
 * @return Random ZUSHORT value.
 *
 * @see Zrandom_N32, Zrandom_N64
 */
ZFUNC ZUSHORT Zrandom_N16(ZFUNC_VOID);

/**
 * @brief Generate a random ZUINT value.
 *
 * @return Random ZUINT value.
 *
 * @see Zrandom_N16, Zrandom_N64
 */
ZFUNC ZUINT Zrandom_N32(ZFUNC_VOID);

/**
 * @brief Generate a random ZUINT value.
 *
 * @return Random ZUINT value.
 *
 * @see Zrandom_N16, Zrandom_N32
 */
ZFUNC ZULLONG Zrandom_N64(ZFUNC_VOID);

/**
 * @brief Generate a random id string.
 *  Default random id string using 0-9|a-z|A-Z character, user can specify some add-on
 *  characters.
 *
 * @param [in] pstAddr Locad network address, .
 * @param [in] pcAddOnIds The add-on id character set except 0-9|a-z|A-Z characters.
 * @param [in] iIdLen The minimum rand id length.
 * The recommend rand id length is more than 16. 
 *
 * @return Random id string, it should be freed by Zos_SysStrFree.
 *
 * @note pstAddr is optional parameter, user can input ZNULL.
 * iIdLen should not bigger than 64 bytes.
 *
 * @see Zrandom_RandIdX
 */
ZFUNC ZCHAR * Zrandom_RandId(ST_ZOS_INET_IP *pstAddr, ZCHAR *pcAddOnIds,
                ZINT iIdLen);

/**
 * @brief Generate a random id string which can contain a prefix string.
 *
 * @param [in] pcPrefix The prefix before random id string.
 * @param [in] pstAddr Locad network address, .
 * @param [in] pcAddOnIds The add-on id character set except 0-9|a-z|A-Z characters.
 * @param [in] iIdLen The minimum rand id length.
 * The recommend rand id length is more than 16. 
 *
 * @return Random id string, it should be freed by Zos_SysStrFree.
 *
 * @note pstAddr is optional parameter, user can input ZNULL.
 * iIdLen should not bigger than 64 bytes.
 *
 * @see Zrandom_RandId
 */
ZFUNC ZCHAR * Zrandom_RandIdX(ZCHAR *pcPrefix, ST_ZOS_INET_IP *pstAddr, 
                ZCHAR *pcAddOnIds, ZINT iIdLen);

#ifdef __cplusplus
}
#endif

#endif /* _ZRANDOM_H__ */

