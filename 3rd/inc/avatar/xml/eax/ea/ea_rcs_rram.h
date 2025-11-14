/************************************************************************

        Copyright (c) 2005-2013 by Juphoon System Software, Inc.
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
  File name     : ea_rcs_rram.h
  Module        : eax rcs rram
  Author        : gloria.yang
  Version       : 1.0
  Created on    : 2019-07-16
  Description   :
    Marcos and structure definitions required by the eax rcs rram.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_RCS_RRAM_H__
#define _EA_RCS_RRAM_H__

#ifdef __cplusplus
extern "C"{
#endif

/************** urn:gsma:params:xml:ns:rcs:rcs:rram **************/
typedef enum EN_EA_RCS_RRAM_TYPE
{
    EN_EA_RCS_RRAM_PLAYING_LENGTH,            /* playing-length */
} EN_EA_RCS_RRAM_TYPE;

/* eax rcs rram get the playing-length data */
ZFUNC ZINT EaRcsRram_FileGetPlayingLengthData(ST_XML_ELEM *pstFile, 
                ZUINT *piPlayingLength);

/* eax rcs fthttp set the file-size data */
ZFUNC ZINT EaRcsRram_FileSetPlayingLengthData(ST_XML_ELEM *pstFile, 
                ZUINT iPlayingLength);

#ifdef __cplusplus
}
#endif

#endif /* _EA_RCS_RRAM_H__ */

