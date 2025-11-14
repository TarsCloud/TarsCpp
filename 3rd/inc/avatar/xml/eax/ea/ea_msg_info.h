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
  File name     : ea_msg_info.h
  Module        : eax msginfo
  Author        : zocol
  Version       : 0.1
  Created on    : 2013-03-04
  Description   :
    Marcos and structure definitions required by the eax msginfo.

  Modify History:
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _EA_MSG_INFO_H__
#define _EA_MSG_INFO_H__
    
#ifdef __cplusplus
    extern "C"{
#endif

/************* urn:ietf:params:xml:ns:msginfo *********/
typedef enum EN_EA_MSG_INFO_TYPE
{
    EN_EA_MSG_INFO_MSG_LST,         /* message-list */
    EN_EA_MSG_INFO_NUMBER,          /* number */
    EN_EA_MSG_INFO_MESSAGE,         /* message */
    EN_EA_MSG_INFO_DATE_TIME,       /* date-time */
    EN_EA_MSG_INFO_MSG_REF,         /* message-reference */
    EN_EA_MSG_INFO_SIZE,            /* size */
    EN_EA_MSG_INFO_EXPIRY,          /* expiry */
    EN_EA_MSG_INFO_SUBJECT,         /* subject */
    EN_EA_MSG_INFO_INFO,            /* info */
    EN_EA_MSG_INFO_FROM,            /* from */
    EN_EA_MSG_INFO_TO,              /* to */
    EN_EA_MSG_INFO_RECP_LST,        /* recipient-list */
    EN_EA_MSG_INFO_ENTRY,           /* entry */
    EN_EA_MSG_INFO_AUTH_ID,         /* auth-id */
    EN_EA_MSG_INFO_URI,             /* auth-id */
} EN_EA_MSG_INFO_TYPE;

/* msginfo get element "message-list" */
ZFUNC ZINT EaMsgInfo_GetMsgLst(ST_XML_MSG *pstMsg, 
                ST_XML_ELEM **ppstMsgLst);

/* msginfo get value of attribute "number" */
ZFUNC ZINT EaMsgInfo_GetNumber(ST_XML_ELEM *pstMsgLst, 
                ZUINT *piNumber);

/* msginfo get element "message" */
ZFUNC ZINT EaMsgInfo_GetMsgElem(ST_XML_ELEM *pstMsgLst, 
                ST_XML_ELEM **ppstMsgElem);

/* msginfo get first element "message" */
ZFUNC ZINT EaMsgInfo_GetMsgFirst(ST_XML_ELEM *pstMsgLst, 
                ST_XML_ELEM **ppstMsg);

/* msginfo get nest element "message" */
ZFUNC ZINT EaMsgInfo_GetMsgNext(ST_XML_ELEM *pstMsg, 
                ST_XML_ELEM **ppstMsg);

/* msginfo get value of attribute "date-time" */
ZFUNC ZINT EaMsgInfo_GetDateTime(ST_XML_ELEM *pstMsg, 
                ST_ZOS_USTR **ppstDateTime);

/* msginfo get value of attribute "message-reference" */
ZFUNC ZINT EaMsgInfo_GetMsgRef(ST_XML_ELEM *pstMsg, 
                ST_ZOS_USTR **ppstMsgRef);

/* msginfo get value of element "size" */
ZFUNC ZINT EaMsgInfo_GetSize(ST_XML_ELEM *pstMsg, ZUINT *piSize);

/* msginfo get value of element "expiry" */
ZFUNC ZINT EaMsgInfo_GetExpiry(ST_XML_ELEM *pstMsg, 
                ST_ZOS_USTR **ppstExpiry);

/* msginfo get value of element "subject" */
ZFUNC ZINT EaMsgInfo_GetSubject(ST_XML_ELEM *pstMsg, 
                ST_ZOS_USTR **ppstSubject);

/* msginfo get element "info" */
ZFUNC ZINT EaMsgInfo_GetInfoElem(ST_XML_ELEM *pstMsg, 
                ST_XML_ELEM **ppstInfoElem);

/* msginfo get value of element "from" */
ZFUNC ZINT EaMsgInfo_GetFrom(ST_XML_ELEM *pstInfo, ST_ZOS_USTR **ppstFrom);

/* msginfo get value of element "to" */
ZFUNC ZINT EaMsgInfo_GetTo(ST_XML_ELEM *pstInfo, ST_ZOS_USTR **ppstTo);

/* msginfo get value of element "auth-id" */
ZFUNC ZINT EaMsgInfo_GetAuthId(ST_XML_ELEM *pstInfo, 
                ST_ZOS_USTR **ppstAuthId);

/* msginfo get element "recipient-list" */
ZFUNC ZINT EaMsgInfo_GetRecpLstElem(ST_XML_ELEM *pstInfo, 
                ST_XML_ELEM **ppstRecpLstElem);

/* msginfo get element "entry" */
ZFUNC ZINT EaMsgInfo_GetEntryElem(ST_XML_ELEM *pstRecpLst, 
                ST_XML_ELEM **ppstEntryElem);

/* msginfo get first element "entry" */
ZFUNC ZINT EaMsgInfo_GetEntryFirst(ST_XML_ELEM *pstRecpLst, 
                ST_XML_ELEM **ppstEntry);

/* msginfo get next element "entry" */
ZFUNC ZINT EaMsgInfo_GetEntryNext(ST_XML_ELEM *pstEntry, 
                ST_XML_ELEM **ppstEntry);

/* msginfo get element "entry" */
ZFUNC ZINT EaMsgInfo_GetUri(ST_XML_ELEM *pstEntry, ST_ZOS_USTR **ppstUri);

#ifdef __cplusplus
}
#endif

#endif /* _EA_MSG_INFO_H__ */

