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
  File name     : zsh_env.h
  Module        : zos shell environment
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2005-09-30    
  Description   :
   Function implement required by zos shell environment

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZSH_EVAR_H__
#define _ZSH_EVAR_H__

/** 
 * @file zsh_env.h
 * @brief Zos shell enviroment functions.
 *  
 * In this file, it defined the operation function
 * of zos shell enviroment, including get, put and set.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get a environment variable.
 *
 * @param [in] pcName Variable name.
 *
 * @return The variable value if found, or return "" (null string).
 *
 * Call this function to add a command to command list,
 * then you can run command from zsh prompt line.
 *
 * @see Zsh_EvarPut
 */
ZFUNC ZCHAR * Zsh_EvarGet(ZCHAR *pcName);

/**
 * @brief Set a environment variable with name and value pair.
 *
 * @param [in] pcStr Variable name and value pair, it should be "name=value".
 *
 * @retval ZOK Variable set OK.
 * @retval ZFAILED Variable set failed you may check input parameter format.
 *
 * Using "name=value" to set a variable named "name". Using "name=" to unset
 * that variable.
 *
 * @see Zsh_EvarSet
 */
ZFUNC ZINT Zsh_EvarPut(ZCHAR *pcStr);

/**
 * @brief Set a environment variable with name and value pair.
 *
 * @param [in] pcName Variable name and value pair. 
 *                    Using "name=value" to set a variable named "name". 
 *                    Using "name=" to unset that variable.
 * @param [in] pcVal Variable value.
 *
 * @retval ZOK Variable set OK.
 * @retval ZFAILED Variable set failed you may check input parameter format.
 *
 * @see Zsh_EvarGet
 */
ZFUNC ZINT Zsh_EvarSet(ZCHAR *pcName, ZCHAR *pcVal);

#ifdef __cplusplus
}
#endif

#endif /* _ZSH_EVAR_H__ */


