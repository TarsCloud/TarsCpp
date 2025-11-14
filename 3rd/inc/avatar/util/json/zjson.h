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
  File name     : zjson.h
  Module        : json utilitis
  Author        : bob.liu   
  Version       : 0.1   
  Created on    : 2014-04-19    
  Description   :
      Data structure and function definitions required by the 
      json utilitis
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZJSON_H__
#define _ZJSON_H__

#ifdef __cplusplus
extern "C" {
#endif

/* value type */
typedef enum EN_ZJSON_TYPE
{
    EN_ZJSON_BOOL,
    EN_ZJSON_NULL,
    EN_ZJSON_OBJECT,
    EN_ZJSON_ARRAY,
    EN_ZJSON_NUMBER,
    EN_ZJSON_STRING,
    EN_ZJSON_UNKNOWN
} EN_ZJSON_TYPE;

/* json format mode */
typedef enum EN_ZJSON_MODE_TYPE
{
    EN_ZJSON_MODE_COMPACT = 0x01,
    EN_ZJSON_MODE_READABLE = 0x02,
    EN_ZJSON_MODE_AUTO_COMPRESS = 0x04,
    EN_ZJSON_MODE_ALWAYS_COMPRESS = 0x08
} EN_ZJSON_MODE_TYPE;

#define __ZJSON_CREATE__

/* json create for boolean */
ZFUNC ZJSON Zjson_CreateBool(ZBOOL bValue);

/* json create for null */
ZFUNC ZJSON Zjson_CreateNull(ZFUNC_VOID);

/* json create for object */
ZFUNC ZJSON Zjson_CreateObject(ZFUNC_VOID);

/* json create for array */
ZFUNC ZJSON Zjson_CreateArray(ZFUNC_VOID);

/* json create for number */
ZFUNC ZJSON Zjson_CreateNumber(ZDOUBLE dValue, ZBOOL bEfmt);

/* json create for number */
ZFUNC ZJSON Zjson_CreateNumberX(ZLLONG qwValue);

/* json create for string */
ZFUNC ZJSON Zjson_CreateString(ZCONST ZCHAR *pcValue);

/* json create for string with length */
ZFUNC ZJSON Zjson_CreateStringX(ZCONST ZCHAR *pcValue, ZUSHORT wLen);

#define __ZJSON_DELETE__

/* delete json */
ZFUNC ZVOID Zjson_Delete(ZJSON zItem);

#define __ZJSON_PARSE__

/* parse json */
ZFUNC ZJSON Zjson_Parse(ZCBUF zMemBuf, ZCONST ZCHAR *pcText, ZUINT iLen);

#define __ZJSON_PRINT__

/* print json */
ZFUNC ZINT Zjson_Print(ZDBUF zMemBuf, ZJSON zItem, ZUINT iMode);

/* print sys string */
ZFUNC ZCHAR * Zjson_PrintSysStr(ZJSON zItem, ZUINT iMode);

#define __ZJSON_ARRAY__

/* add json to array */
ZFUNC ZINT Zjson_ArrayAdd(ZJSON zArray, ZUINT iIndex, ZJSON zItem);

/* remove json from array */
ZFUNC ZJSON Zjson_ArrayRmv(ZJSON zArray, ZUINT iIndex);

/* get json array size */
ZFUNC ZUINT Zjson_ArraySize(ZJSON zArray);

/* get json from array */
ZFUNC ZJSON Zjson_ArrayGet(ZJSON zArray, ZUINT iIndex);

/* array get item boolean */
ZFUNC ZBOOL Zjson_ArrayGetBool(ZJSON zArray, ZUINT iIndex);

/* array get item number */
ZFUNC ZLLONG Zjson_ArrayGetNumber(ZJSON zArray, ZUINT iIndex);

/* array get item number */
ZFUNC ZDOUBLE Zjson_ArrayGetNumberX(ZJSON zArray, ZUINT iIndex);

/* array get item string */
ZFUNC ZCONST ZCHAR * Zjson_ArrayGetString(ZJSON zArray, ZUINT iIndex);

#define __ZJSON_OBJECT__

/* add json to object */
ZFUNC ZINT Zjson_ObjectAdd(ZJSON zObject, ZCONST ZCHAR *pcName, ZJSON zItem);

/* remove json from object */
ZFUNC ZJSON Zjson_ObjectRmv(ZJSON zObject, ZCONST ZCHAR *pcName);

/* get json object size */
ZFUNC ZUINT Zjson_ObjectSize(ZJSON zObject);

/* get json from array */
ZFUNC ZJSON Zjson_ObjectEnum(ZJSON zObject, ZUINT iIndex);

/* json object get item */
ZFUNC ZJSON Zjson_ObjectGet(ZJSON zObject, ZCONST ZCHAR *pcName);

/* object get item boolean */
ZFUNC ZBOOL Zjson_ObjectGetBool(ZJSON zObject, ZCONST ZCHAR *pcName);

/* object get item number */
ZFUNC ZLLONG Zjson_ObjectGetNumber(ZJSON zObject, ZCONST ZCHAR *pcName);

/* object get item number */
ZFUNC ZDOUBLE Zjson_ObjectGetNumberX(ZJSON zObject, ZCONST ZCHAR *pcName);

/* object get item string */
ZFUNC ZCONST ZCHAR * Zjson_ObjectGetString(ZJSON zObject, ZCONST ZCHAR *pcName);

#define __ZJSON_VALUE__

/* get json value type */
ZFUNC ZUINT Zjson_ValueGetType(ZJSON zItem);

/* get json value name */
ZFUNC ZCONST ZCHAR * Zjson_ValueGetName(ZJSON zItem);

/* get json value for boolean */
ZFUNC ZINT Zjson_ValueGetBool(ZJSON zItem, ZBOOL *pbValue);

/* get json value for number */
ZFUNC ZINT Zjson_ValueGetNumber(ZJSON zItem, ZDOUBLE *pdValue);

/* get json value for string */
ZFUNC ZCONST ZCHAR * Zjson_ValueGetString(ZJSON zItem);

#ifdef __cplusplus
}
#endif

#endif /* _ZJSON_H__ */

