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
  File name:  http_abnf cfg.h
  Module:  http abnf configuration
  Author:  Aaron Zhong   
  Version: 0.1   
  Created on : 2005-08-12    
  Description:  
      Configurations to customize HTTP.
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _HTTP_ABNF_CFG_H__
#define _HTTP_ABNF_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* http abnf config get log level, default is ZLOG_LEVEL_ALL */
ZFUNC ZUINT Http_AbnfCfgGetLogLevel(ZFUNC_VOID);

/* http abnf config get strict decode, default is ZTRUE */
ZFUNC ZBOOL Http_AbnfCfgGetStrictDecode(ZFUNC_VOID);

/* http abnf config set log level */
ZFUNC ZINT Http_AbnfCfgSetLogLevel(ZUINT iLevel);

/* http abnf config set strict decode, default is ZTRUE */
ZFUNC ZINT Http_AbnfCfgSetStrictDecode(ZBOOL bStrict);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_ABNF_CFG_H__ */

