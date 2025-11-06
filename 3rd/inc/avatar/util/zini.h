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
  File name     : zini.h
  Module        : initialization file functions
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2003-11-03    
  Description   :
      Data structure and function definitions required by the 
      initialization file interface
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZINI_H__
#define _ZINI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* zini error value of return */
#define ZINI_ERR_INVALID_PARAM   (-1)
#define ZINI_ERR_OPEN_FILE       (-2)
#define ZINI_ERR_READ_FILE       (-3)
#define ZINI_ERR_DECODE          (-4)

/* zini mode type */
typedef enum EN_ZINI_MODE_TYPE
{
    EN_ZINI_MODE_DEC,
    EN_ZINI_MODE_HEX
} EN_ZINI_MODE_TYPE;

/* ini manager create */
ZFUNC ZINT Zini_Create(ZCONST ZCHAR *pcFileName, ZVOID **ppIni);

/* ini manager create */
ZFUNC ZINT Zini_CreateX(ZCONST ZCHAR *pcData, ZUINT iLen, ZVOID **ppIni);

/* ini manager delete */
ZFUNC ZVOID Zini_Delete(ZVOID *pIni);

/* ini get section count */
ZFUNC ZUINT Zini_GetSectCount(ZVOID *pIni);

/* ini get section name */
ZFUNC ZCHAR * Zini_GetSectName(ZVOID *pIni, ZUINT iIndex);

/* ini get key count in specific section */
ZFUNC ZUINT Zini_GetKeyCount(ZVOID *pIni, ZUINT iSectIndex);

/* ini get key name string in specific section */
ZFUNC ZCHAR * Zini_GetKeyName(ZVOID *pIni, ZUINT iSectIndex, ZUINT iKeyIndex);

/* ini get key name string in specific section */
ZFUNC ZUINT Zini_GetKeyCountX(ZVOID *pIni, ZCONST ZCHAR *pcSectName);

/* ini get key value string in specific section */
ZFUNC ZCHAR * Zini_GetKeyStr(ZVOID *pIni, ZUINT iSectIndex, ZUINT iKeyIndex,
                ZCHAR *pcDftStr);

/* ini get key value string in specific section */
ZFUNC ZCHAR * Zini_GetKeyStrX(ZVOID *pIni, ZCONST ZCHAR *pcSectName,
                ZUINT iKeyIndex, ZCHAR *pcDftStr, ZCHAR **ppcKeyName);

/* ini get string by section and key name */
ZFUNC ZCHAR * Zini_GetStr(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZCHAR *pcDftStr);

/* ini get string by section and key name */
ZFUNC ZINT Zini_GetXStr(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZCHAR *pcDftStr, ST_ZOS_SSTR *pstStr);

/* ini get unsigned int value by section and key name */
ZFUNC ZUINT Zini_GetUi(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZUINT iDftVal);

/* ini get profile unsigned short value by section and key name */
ZFUNC ZUSHORT Zini_GetUs(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZUSHORT wDftVal);

/* ini get profile unsigned char value by section and key name */
ZFUNC ZUCHAR Zini_GetUc(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZUCHAR ucDftVal);

/* zini update ini to file */
ZFUNC ZINT Zini_Update(ZVOID *pIni, ZCONST ZCHAR *pcFileName);

/* zini add string to ini */
ZFUNC ZINT Zini_PutStr(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ST_ZOS_SSTR *pstStr);

/* zini add ulong to ini */
ZFUNC ZINT Zini_PutUi(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZUINT iVal, ZUCHAR ucMode);

/* zini add ushort to ini */
ZFUNC ZINT Zini_PutUs(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZUSHORT wVal, ZUCHAR ucMode);

/* zini add uchar to ini */
ZFUNC ZINT Zini_PutUc(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName, 
                ZUCHAR ucVal, ZUCHAR ucMode);

/* zini delete key from ini */
ZFUNC ZINT Zini_DelKey(ZVOID *pIni, ZCHAR *pcSectName, ZCHAR *pcKeyName);

/* zini delete section from ini */
ZFUNC ZINT Zini_DelSect(ZVOID *pIni, ZCHAR *pcSectName);

#ifdef __cplusplus
}
#endif

#endif /* _ZINI_H__ */

