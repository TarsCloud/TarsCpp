/************************************************************************

        Copyright (c) 2005-2010 by Juphoon System Software, Inc.
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
  File name     : zpand_ims.h
  Module        : android os http library functions
  Author        : Bob Liu   
  Version       : 1.0   
  Created on    : 2015-07-20    
  Description   :
      Data structure and function definitions required by the ims interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZPAND_IMS_H__
#define _ZPAND_IMS_H__

#ifdef __cplusplus
extern "C" {
#endif

ZFUNC ZIMSID Zpand_ImsHandleCreateConnectionManager();
ZFUNC ZINT Zpand_ImsHandleCreateIMConnection(ZIMSID zObj, ZCHAR *pcFeatureTag);
ZFUNC ZINT Zpand_ImsHandleSendIMMessage(ZIMSID zObj, ZCHAR *pcOutboundProxy, 
                    ZUSHORT wRemotePort, ZINT iProtocol, ZINT iMessageType, 
                    ZCHAR *pcCallId, ZCHAR *pcMessage, ZINT iMessageLen);
ZFUNC ZINT Zpand_ImsHandleTriggerRegistration(ZIMSID zObj);
ZFUNC ZINT Zpand_ImsHandleTriggerDeRegistration(ZIMSID zObj);
ZFUNC ZINT Zpand_ImsHandleAutoconfig(ZIMSID zObj, ZINT pcConfigType, ZINT userdata);
ZFUNC ZINT Zpand_ImsHandleSetStatus(ZIMSID zObj, ZCHAR *pcFeatureTag);

#ifdef __cplusplus
}
#endif

#endif /* _ZPAND_IMS_H__ */

