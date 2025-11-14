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
  File name     : xml_cfg.h
  Module        : xml configuration
  Author        : Zocol  
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Configurations to customize xml.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XML_CFG_H__
#define _XML_CFG_H__
/** 
 * @file xml_cfg.h
 * @brief XML configuration interfaces.
 *
 * @see 
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief XML config get log level.
 *
 * @retval ZOK The log level, default is ZLOG_LEVEL_ERROR.
 * @see
 */
ZFUNC ZUINT Xml_CfgGetLogLevel(ZFUNC_VOID);

/**
 * @brief XML config set log level.
 *
 * @param [in] iLevel The log level.
 *
 * @retval ZOK Set the log level successful.
 * @see
 */
ZFUNC ZINT Xml_CfgSetLogLevel(ZUINT iLevel);

#ifdef __cplusplus
}
#endif

#endif /* _XML_CFG_H__ */

