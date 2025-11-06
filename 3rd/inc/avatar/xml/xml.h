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
  File name     : xml.h
  Module        : xml protocol
  Author        : Zocol  
  Version       : 0.1   
  Created on    : 2006-04-17    
  Description   :
      Data structure and function definitions required by the xml

    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _XML_H__
#define _XML_H__

/** 
 * @file
 * @brief XML header files.
 *
 * To include the definitions of XML, use the following directive:
 * @code
 * #include "xml.h"
 * @endcode
 */
 
/* Lint -save -e*. */
#include "zos.h"                     /* ZOS system services interface. */
/* Lint -restore. */

#include "xml_type.h"                /* XML typedef and interface. */
#include "xml_cfg.h"                 /* XML configuration interface. */
#include "xml_util.h"                /* XML utility interfaces. */

/* XML api of sax. */
#include "sax_core1.h"               /* SAX1 core interfaces. */
#include "sax_corex.h"               /* SAXX core interfaces. */

/* XML api of dom. */
#ifdef XML_SUPT_API_DOM
#include "dom_type.h"                /* DOM typedefs. */
#include "dom_core.h"                /* DOM core interfaces. */
#include "dom_util.h"                /* DOM utility interfaces. */
#endif

#ifdef __cplusplus
extern "C"{
#endif 

/**
 * @brief XML initialization.
 *
 * @retval ZOK Initialize successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZINT Xml_Init(ZFUNC_VOID);

/**
 * @brief XML destroy.
 *
 * @retval ZOK destroy successful.
 * @retval ZFAILED Otherwise.
 * @see
 */
ZFUNC ZVOID Xml_Destroy(ZFUNC_VOID);

#ifdef __cplusplus
}
#endif 

#endif /* _XML_H__ */

