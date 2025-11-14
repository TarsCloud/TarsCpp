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
  File name     : ea_poc_rules.h
  Module        : eax poc poc-rules
  Author        : zocol
  Version       : 0.1
  Created on    : 2007-11-28
  Description   :
    Marcos and structure definitions required by the eax poc poc-rules.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_POC_RULES_H__
#define _EA_POC_RULES_H__

#ifdef __cplusplus
extern "C"{
#endif

/* poc-rules allow-invite type */
typedef enum EN_EA_POC_RULES_TYPE
{
    EN_EA_POC_RULES_REJECT,        
    EN_EA_POC_RULES_PASS,
    EN_EA_POC_RULES_ACCEPT
} EN_EA_POC_RULES_TYPE;

/* poc-rules get the allow-invite */
ZFUNC ZINT EaPoc_RulesGetAllowIvt(ST_XML_ELEM *pstElem, ZUCHAR *pucType);

/* poc-rules set the allow-invite */
ZFUNC ZINT EaPoc_RulesSetAllowIvt(ST_XML_ELEM *pstElem, ZUCHAR ucType);

#ifdef __cplusplus
}
#endif

#endif /* _EA_POC_RULES_H__ */

