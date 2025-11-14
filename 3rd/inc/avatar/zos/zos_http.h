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
  File name     : zos_http.h
  Module        : zos http library functions
  Author        : Bob Liu 
  Version       : 1.0
  Created on    : 2015-07-20    
  Description   :
      Data structure and function definitions required by the socket interface

  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZOS_HTTP_H__
#define _ZOS_HTTP_H__

/**
 * @file
 * @brief Zos HTTP interfaces.
 *
 * These interfaces provide http operation. They depending on OS interfaces.
 */

/**
 * @brief Calculate delimiter length.
 */
#define ZOS_HTTP_DELIMITER_LEN(_boundary) (Zos_StrLen(_boundary) + 6)

/**
 * @brief Calculate close delimiter length.
 */
#define ZOS_HTTP_CLOSE_DELIMITER_LEN(_boundary) (Zos_StrLen(_boundary) + 6)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HTTP event process callbacks.
 *
 * @param  pcEvntName The event name of HTTP connection.
 * @param  zCookie    The HTTP event cookie.
 * @param  iHttpId    The HTTP id.
 * @param  iStatus    The HTTP connection status.
 */
typedef ZVOID (*PFN_ZHTTPEVENtPROC)(ZCONST ZCHAR *pcEvntName, ZCOOKIE zCookie, 
        ZHTTPID iHttpId, ZINT iStatus);

/**
 * @brief Open an HTTP connection.
 * @param  pcUri     The HTTP server URI.
 * @param  bSecurity ZTRUE to use TLS.
 * @param  bPost     ZTRUE to use POST method, ZFALSE to use GET method.
 * @return           The ID of HTTP connection, ZINVALIDID when failed.
 */
ZFUNC ZHTTPID Zos_HttpOpen(ZCONST ZCHAR *pcUri, ZBOOL bSecurity, ZBOOL bPost);

/**
 * @brief Open an HTTP connection.
 * @param  pcUri     The HTTP server URI.
 * @param  bPost     ZTRUE to use POST method, ZFALSE to use GET metho.
 * @return           The ID of HTTP connection, ZINVALIDID when failed.
 */
ZFUNC ZHTTPID Zos_HttpOpenX(ZCONST ZCHAR *pcUri, ZBOOL bPost);

/**
 * @brief Close HTTP connection.
 * @param  iConnId The ID of HTTP connection.
 */
ZFUNC ZVOID Zos_HttpClose(ZHTTPID iConnId);

/**
 * @brief Start connect and send the request.
 * @param  iConnId The ID of HTTP connection.
 * @return         ZOK on succeed, ZFAILED when failed.
 */
ZFUNC ZINT Zos_HttpConn(ZHTTPID iConnId);

/**
 * @brief Stop connect.
 * @param  iConnId The ID of HTTP connection.
 * @return         ZOK on succeed, ZFAILED when failed.
 */
ZFUNC ZINT Zos_HttpDisconn(ZHTTPID iConnId);

/**
 * @brief Set the header of HTTP message.
 *
 * @param  iConnId The ID of HTTP connection.
 * @param  pcName  The name of header.
 * @param  pcValue The value of header.
 * @return         ZOK on succeed, ZFAILED when failed.
 *
 * Example:
 * @code

 Zos_HttpSetProp(pstSess->zHttpId, "Content-Type", "text/plain");
 Zos_HttpSetProp(pstSess->zHttpId, "Content-Length", "32");
 
 * @endcode
 */
ZFUNC ZINT Zos_HttpSetProp(ZHTTPID iConnId, ZCONST ZCHAR *pcName,
    ZCONST ZCHAR *pcValue);

/**
 * @brief Set the config of HTTP message.
 *
 * @param  iConnId The ID of HTTP connection.
 * @param  pcName  The name of config.
 * @param  pcValue The value of config.
 * @return         ZOK on succeed, ZFAILED when failed.
 *
 * Example
 * @code

 Zos_HttpSetCfg(pstSess->zHttpId, "boundary", "fileuploadboundary");

 * @endcode
 */
ZFUNC ZINT Zos_HttpSetCfg(ZHTTPID iConnId, ZCONST ZCHAR *pcName,
    ZCONST ZCHAR *pcValue);

/**
 * @brief Set the body fo HTTP message
 * @param  iConnId   The ID of HTTP connection.
 * @param  pcHeader  The body header.
 * @param  pcContent The body content.
 * @return         ZOK on succeed, ZFAILED when failed.
 *
 * Example
 * @code

 pcHeader = "Content-Disposition: form-data; name=\"username\"\r\n\r\n";
 Zos_HttpSetBody(pstSess->zHttpId, pcHeader, "alice");

 * @endcode
 * or
 * @code
 
 Zos_HttpSetBody(pstSess->zHttpId, "", pcPostBody);
 
 * @endcode
 * 
 */
ZFUNC ZINT Zos_HttpSetBody(ZHTTPID iConnId, ZCONST ZCHAR *pcHeader,
    ZCONST ZCHAR *pcContent);

/**
 * @brief Set the body of HTTP message, which content is a file data.
 *
 * @param  iConnId    The ID of HTTP connection.
 * @param  pcHeader   The body header.
 * @param  pcFileName The file name where the body content read from.
 * @return         ZOK on succeed, ZFAILED when failed.
 *
 * Example
 * @code

 pcHeader = Zos_SysStrFAlloc("Content-Type: application/x-gzip\r\n"
     "Content-Disposition: form-data; name=\"upfile\"; filename=\"%s\"\r\n\r\n",
     pstSess->pcFileName);
 Zos_HttpSetBody(pstSess->zHttpId, pcHeader, pstSess->pcFilePath);

 * @endcode
 */
ZFUNC ZINT Zos_HttpSetFile(ZHTTPID iConnId, ZCONST ZCHAR *pcHeader,
    ZCONST ZCHAR *pcFileName);

/**
 * @brief Get hte body content of HTTP response.
 * @param  iConnId The ID of HTTP connection.
 * @return         The body content.
 *
 * Example
 * @code

 ZCHAR *pcBody;
 ZUINT iBodyLen;
 ZDBUF zBody = Zos_HttpGetBody(iHttpId);
 Zos_DbufO2D(zBody, 0, &pcBody);
 iBodyLen = Zos_DbufLen(zBody);
 ...
 Zos_DbufDelete(zBody);

 * @endcode
 */
ZFUNC ZDBUF Zos_HttpGetBody(ZHTTPID iConnId);

/**
 * @brief Subscribe HTTP event.
 * @param  iConnId      The ID of HTTP connection.
 * @param  zTaskId      The perform task ID.
 * @param  zCookie      The cookie value.
 * @param  pfnCallback  The event process callback.
 * @return         ZOK on succeed, ZFAILED when failed.
 */
ZFUNC ZINT Zos_HttpSubEvnt(ZHTTPID iConnId, ZTASKID zTaskId, ZCOOKIE zCookie,
    PFN_ZHTTPEVENtPROC pfnCallback);

/**
 * @brief Unsubscribe HTTP event.
 * @param  iConnId      The ID of HTTP connection.
 * @param  zTaskId      The perform task ID.
 * @param  zCookie      The cookie value.
 * @param  pfnCallback  The event process callback.
 * @return         ZOK on succeed, ZFAILED when failed.
 */
ZFUNC ZINT Zos_HttpUnsubEvnt(ZHTTPID iConnId, ZTASKID zTaskId, ZCOOKIE zCookie,
    PFN_ZHTTPEVENtPROC pfnCallback);

#ifdef __cplusplus
}
#endif

#endif /* _ZOS_HTTP_H__ */

